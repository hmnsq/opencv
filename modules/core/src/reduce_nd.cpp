// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#include "precomp.hpp"
#include "opencv2/core/hal/intrin.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace cv {

namespace {

enum
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> struct VecT { enum { ok = 0 }; typedef T vtype; };
#もし (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> struct VecT<T> { enum { ok = 1 }; typedef V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(整数, v_int32)
CV_RND_VEC(float, v_float32)
#もし (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(double, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> struct OpSum
{
    static A init() { 戻る A(0); }
    static A apply(A a, A x) { 戻る a + x; }
    static A combine(A a, A b) { 戻る a + b; }
    static A finalize(A a, int64) { 戻る a; }
    template<typename V> static V vapply(const V& a, const V& x) { 戻る v_add(a, x); }
    template<typename V> static V vcombine(const V& a, const V& b) { 戻る v_add(a, b); }
};

template<typename A> struct OpAvg : OpSum<A>
{
    static A finalize(A a, int64 n) { 戻る n > 0 ? A(a/(double)n) : A(0); }
};

template<typename A> struct OpLogSum : OpSum<A>
{
    static A finalize(A a, int64) { 戻る A(std::log(a)); }
};

template<typename A> struct OpSum2 : OpSum<A>
{
    static A apply(A a, A x) { 戻る a + x*x; }
    template<typename V> static V vapply(const V& a, const V& x) { 戻る v_add(a, v_mul(x, x)); }
};

template<typename A> struct OpL2 : OpSum2<A>
{
    static A finalize(A a, int64) { 戻る A(std::sqrt(a)); }
};

template<typename A> struct OpL1 : OpSum<A>
{
    static A apply(A a, A x) { 戻る a + (x < A(0) ? -x : x); }
    template<typename V> static V vapply(const V& a, const V& x) { 戻る v_add(a, v_abs(x)); }
};

template<typename A> struct OpProd
{
    static A init() { 戻る A(1); }
    static A apply(A a, A x) { 戻る a*x; }
    static A combine(A a, A b) { 戻る a*b; }
    static A finalize(A a, int64) { 戻る a; }
    template<typename V> static V vapply(const V& a, const V& x) { 戻る v_mul(a, x); }
    template<typename V> static V vcombine(const V& a, const V& b) { 戻る v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, bool hasInf = std::numeric_limits<A>::has_infinity> struct Extremes
{
    static A low() { 戻る std::numeric_limits<A>::lowest(); }
    static A high() { 戻る std::numeric_limits<A>::max(); }
};

template<typename A> struct Extremes<A, true>
{
    static A low() { 戻る -std::numeric_limits<A>::infinity(); }
    static A high() { 戻る std::numeric_limits<A>::infinity(); }
};

template<typename A> struct OpMax
{
    static A init() { 戻る Extremes<A>::low(); }
    static A apply(A a, A x) { 戻る a < x ? x : a; }
    static A combine(A a, A b) { 戻る a < b ? b : a; }
    static A finalize(A a, int64) { 戻る a; }
    template<typename V> static V vapply(const V& a, const V& x) { 戻る v_max(a, x); }
    template<typename V> static V vcombine(const V& a, const V& b) { 戻る v_max(a, b); }
};

template<typename A> struct OpMin
{
    static A init() { 戻る Extremes<A>::high(); }
    static A apply(A a, A x) { 戻る x < a ? x : a; }
    static A combine(A a, A b) { 戻る b < a ? b : a; }
    static A finalize(A a, int64) { 戻る a; }
    template<typename V> static V vapply(const V& a, const V& x) { 戻る v_min(a, x); }
    template<typename V> static V vcombine(const V& a, const V& b) { 戻る v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> struct OpSumExpShifted
{
    static A init() { 戻る A(0); }
    static A apply(A a, A x, A m) { 戻る a + A(std::exp(x - m)); }
    static A combine(A a, A b) { 戻る a + b; }
    static A finalize(A a, A m) { 戻る std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<class Op, typename A, bool vec = (VecT<A>::ok != 0)> struct HRow
{
    static A run(A acc, const A* x, int64 n)
    {
        ために (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        戻る acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<class Op, typename A, bool vec = (VecT<A>::ok != 0)> struct VRow
{
    static void run(A* acc, const A* x, int64 n)
    {
        ために (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#もし (CV_SIMD || CV_SIMD_SCALABLE)
template<class Op, typename A> struct HRow<Op, A, true>
{
    static A run(A acc, const A* x, int64 n)
    {
        typedef typename VecT<A>::vtype V;
        const 整数 L = VTraits<V>::vlanes();
        int64 i = 0;
        もし (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            ために (整数 k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            ために (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            ために (整数 k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        ために (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        戻る acc;
    }
};

template<class Op, typename A> struct VRow<Op, A, true>
{
    static void run(A* acc, const A* x, int64 n)
    {
        typedef typename VecT<A>::vtype V;
        const 整数 L = VTraits<V>::vlanes();
        int64 i = 0;
        ために (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        ために (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

struct Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    整数 nk;
    整数 ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    整数 nr;
    整数 rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    bool innerReduced;      // the innermost axis is reduced (horizontal mode)

    const uchar* src;
    整数 sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 count;            // reduced elements per output
    int64 nout;             // number of outputs
    BinaryFunc cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
struct Iter
{
    整数 n = 0;
    const 整数* size = nullptr;
    const ptrdiff_t* s = nullptr;
    const ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    void init(整数 n_, const 整数* size_, const ptrdiff_t* s_, const ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        ために (整数 k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            もし (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    void next()
    {
        ために (整数 k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            もし (d)
                doff += d[k];
            もし (++idx[k] < size[k])
                戻る;
            soff -= (ptrdiff_t)size[k]*s[k];
            もし (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> static const A* loadRow(const Plan& pl, const uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    もし (step != (ptrdiff_t)pl.sesz1)
    {
        ために (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    もし (!pl.cvt)
        戻る (const A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((整数)len, 1), 0);
    戻る buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> static void forRow(const Plan& pl, const uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    const bool direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    const int64 blk = direct ? n : (int64)RND_BLOCK;
    ために (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
struct Work
{
    int64 nitems, nsplit;
    double nstripes;
    bool serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, class Op, class OpM> static void runHorizontal(const Plan& pl, const Work& w, A* res,
                                                                   const A* mbuf, A* partial)
{
    const 整数 nr = pl.nr;
    const int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    const ptrdiff_t rstep = pl.rsstep[nr - 1];
    const int64 nsplit = w.nsplit;

    auto body = [&](const Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        もし (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        ながら (it < range.end)
        {
            const int64 o = it / nsplit, s = it - o*nsplit;
            もし (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            const uchar* base = pl.src + ko.soff;
            const A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.count/nsplit, b = (s + 1)*pl.count/nsplit;
            A acc = Op::init();
            もし (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                ながら (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](const A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            もし (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.count, m);
                ko.next();
            }
            否则
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    もし (w.serial)
        body(Range(0, (整数)w.nitems));
    否则
        parallel_for_(Range(0, (整数)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, class Op, class OpM> static void runVertical(const Plan& pl, const Work& w, A* res,
                                                                 const A* mbuf, A* partial)
{
    const 整数 nk = pl.nk;
    const int64 m = pl.ksize[nk - 1];
    const ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    const int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    const int64 nsplit = w.nsplit;
    int64 rcount = 1;
    ために (整数 i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](const Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        ために (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            const int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            const ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            const A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            ために (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            もし (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                ために (; a < b; a++, ro.next())
                {
                    const uchar* p = pl.src + soff + ro.soff;
                    const A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            もし (nsplit == 1)
                ために (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.count, mrow ? mrow[i] : A(0));
            否则
                ために (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    もし (w.serial)
        body(Range(0, (整数)w.nitems));
    否则
        parallel_for_(Range(0, (整数)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, class Op> struct Plain
{
    static A run(A acc, const A* x, int64 n, A) { 戻る HRow<Op, A>::run(acc, x, n); }
    static void vrun(A* acc, const A* x, int64 n, const A*) { VRow<Op, A>::run(acc, x, n); }
    static A finalize(A a, int64 count, A) { 戻る Op::finalize(a, count); }
};

template<typename A> struct ShiftedExp
{
    typedef OpSumExpShifted<A> Op;
    static A run(A acc, const A* x, int64 n, A m)
    {
        ために (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        戻る acc;
    }
    static void vrun(A* acc, const A* x, int64 n, const A* m)
    {
        ために (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    static A finalize(A a, int64, A m) { 戻る Op::finalize(a, m); }
};

template<typename A, class Op, class OpM> static void runPass(const Plan& pl, A* res, const A* mbuf)
{
    const double work = (double)pl.nout*(double)std::max(pl.count, (int64)1);
    const 整数 nthreads = work < RND_SERIAL_WORK ? 1 : getNumThreads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 items;
    もし (pl.innerReduced)
        items = pl.nout;
    否则
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        items = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    const int64 want = 4*(int64)nthreads;
    もし (!w.serial && items < want && pl.count > 1)
    {
        int64 rows = pl.innerReduced ? pl.count/4096 : pl.count/16;
        w.nsplit = std::max((int64)1, std::min((want + items - 1)/items, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = items*w.nsplit;
    w.nstripes = std::min((double)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    もし (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    否则
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    もし (w.nsplit > 1)
    {
        ために (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            ために (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.count, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> static void fillResult(A* res, int64 n, A v)
{
    ために (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> static void runOp(const Plan& pl, 整数 op, A* res)
{
    もし (pl.count == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        戻る;
    }
    switch (op)
    {
    case REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); break;
    case REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); break;
    case REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); break;
    case REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); break;
    case REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); break;
    case REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); break;
    case REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); break;
    case REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        break;
    }
    default:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> static void runMinMax(const Plan& pl, 整数 op, A* res)
{
    もし (pl.count == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    否则 もし (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    否则
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

static void dispatch(const Plan& pl, 整数 op, uchar* res)
{
    const bool minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    switch (pl.adepth)
    {
    case CV_32F:
        もし (minmax) runMinMax<float>(pl, op, (float*)res); 否则 runOp<float>(pl, op, (float*)res);
        break;
    case CV_64F:
        もし (minmax) runMinMax<double>(pl, op, (double*)res); 否则 runOp<double>(pl, op, (double*)res);
        break;
    case CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); break;
    case CV_8S:  runMinMax<schar>(pl, op, (schar*)res); break;
    case CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); break;
    case CV_16S: runMinMax<short>(pl, op, (short*)res); break;
    case CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); break;
    case CV_32S: runMinMax<整数>(pl, op, (整数*)res); break;
    case CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); break;
    case CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); break;
    default:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

static 整数 accumulatorDepth(整数 sdepth, 整数 ddepth, 整数 op)
{
    もし (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        もし (sdepth == CV_16F || sdepth == CV_16BF)
            戻る CV_32F;
        戻る sdepth == CV_Bool ? CV_8U : sdepth;
    }
    const bool wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    戻る wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(整数 _rtype, const std::vector<整数>& _axes, bool _keepdims, 整数 _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

void reduce(InputArray _src, OutputArray _dst, const ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    const 整数 op = params.rtype, dtype = params.dtype;
    const bool keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    const 整数 dims = src.dims, cn = src.channels();
    const 整数 sdepth = src.depth();
    整数 ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    もし ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    bool reduced[CV_MAX_DIM] = {false};
    もし (params.axes.empty())
        std::fill(reduced, reduced + dims, true);
    ために (整数 a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        整数 ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = true;
    }

    整数 keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    ために (整数 i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        もし (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    const 整数 dtype_ = CV_MAKETYPE(ddepth, cn);
    _dst.create(nout, outShape, dtype_);
    Mat dst = _dst.getMat();

    // The results are computed into a contiguous accumulator-typed buffer laid out like the
    // keepdims output (channels innermost), then converted into dst if needed.
    Plan pl;
    pl.src = src.data;
    pl.sdepth = sdepth == CV_Bool ? CV_8U : sdepth;
    pl.adepth = accumulatorDepth(sdepth, ddepth, op);
    pl.sesz1 = CV_ELEM_SIZE1(sdepth);
    pl.cvt = pl.sdepth == pl.adepth ? nullptr : getConvertFunc(pl.sdepth, pl.adepth);
    CV_Assert(pl.sdepth == pl.adepth || pl.cvt);

    // gather the axes (+ the channel axis), outer first
    整数 n = 0;
    整数 size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    bool red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    ために (整数 i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.count = 1;
    ために (整数 i = 0; i < dims; i++)
    {
        もし (reduced[i])
            pl.count *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    もし (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = false;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    const bool direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    もし (!direct)
    {
        resMat.create(1, (整数)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    もし (pl.nout == 0)
        戻る;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    整数 m = 0;
    ために (整数 i = 0; i < n; i++)
    {
        もし (size[i] == 0)
        {
            // empty source along this axis
            もし (red[i])
            {
                pl.count = 0;
                continue;
            }
            戻る;
        }
        もし (size[i] == 1)
            continue;
        もし (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        否则
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    もし (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = false;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    ために (整数 i = 0; i < m; i++)
    {
        もし (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        否则
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    もし (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    もし (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        もし (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
