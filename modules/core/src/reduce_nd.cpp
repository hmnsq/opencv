// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#ضمن "precomp.hpp"
#ضمن "opencv2/core/hal/intrin.hpp"

#ضمن <algorithm>
#ضمن <cmath>
#ضمن <limits>

نطاق cv {

نطاق {

تعداد
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> بنية VecT { تعداد { ok = 0 }; تعريفنوع T vtype; };
#ܐܢ (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> بنية VecT<T> { تعداد { ok = 1 }; تعريفنوع V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(صحيح, v_int32)
CV_RND_VEC(عائم, v_float32)
#ܐܢ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(مزدوج, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> بنية OpSum
{
    ساكن A init() { ܦܢܐ A(0); }
    ساكن A apply(A a, A x) { ܦܢܐ a + x; }
    ساكن A combine(A a, A b) { ܦܢܐ a + b; }
    ساكن A finalize(A a, int64) { ܦܢܐ a; }
    template<typename V> ساكن V vapply(ثابت V& a, ثابت V& x) { ܦܢܐ v_add(a, x); }
    template<typename V> ساكن V vcombine(ثابت V& a, ثابت V& b) { ܦܢܐ v_add(a, b); }
};

template<typename A> بنية OpAvg : OpSum<A>
{
    ساكن A finalize(A a, int64 n) { ܦܢܐ n > 0 ? A(a/(مزدوج)n) : A(0); }
};

template<typename A> بنية OpLogSum : OpSum<A>
{
    ساكن A finalize(A a, int64) { ܦܢܐ A(std::log(a)); }
};

template<typename A> بنية OpSum2 : OpSum<A>
{
    ساكن A apply(A a, A x) { ܦܢܐ a + x*x; }
    template<typename V> ساكن V vapply(ثابت V& a, ثابت V& x) { ܦܢܐ v_add(a, v_mul(x, x)); }
};

template<typename A> بنية OpL2 : OpSum2<A>
{
    ساكن A finalize(A a, int64) { ܦܢܐ A(std::sqrt(a)); }
};

template<typename A> بنية OpL1 : OpSum<A>
{
    ساكن A apply(A a, A x) { ܦܢܐ a + (x < A(0) ? -x : x); }
    template<typename V> ساكن V vapply(ثابت V& a, ثابت V& x) { ܦܢܐ v_add(a, v_abs(x)); }
};

template<typename A> بنية OpProd
{
    ساكن A init() { ܦܢܐ A(1); }
    ساكن A apply(A a, A x) { ܦܢܐ a*x; }
    ساكن A combine(A a, A b) { ܦܢܐ a*b; }
    ساكن A finalize(A a, int64) { ܦܢܐ a; }
    template<typename V> ساكن V vapply(ثابت V& a, ثابت V& x) { ܦܢܐ v_mul(a, x); }
    template<typename V> ساكن V vcombine(ثابت V& a, ثابت V& b) { ܦܢܐ v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, منطقي hasInf = std::numeric_limits<A>::has_infinity> بنية Extremes
{
    ساكن A low() { ܦܢܐ std::numeric_limits<A>::lowest(); }
    ساكن A high() { ܦܢܐ std::numeric_limits<A>::max(); }
};

template<typename A> بنية Extremes<A, ܫܪܪܐ>
{
    ساكن A low() { ܦܢܐ -std::numeric_limits<A>::infinity(); }
    ساكن A high() { ܦܢܐ std::numeric_limits<A>::infinity(); }
};

template<typename A> بنية OpMax
{
    ساكن A init() { ܦܢܐ Extremes<A>::low(); }
    ساكن A apply(A a, A x) { ܦܢܐ a < x ? x : a; }
    ساكن A combine(A a, A b) { ܦܢܐ a < b ? b : a; }
    ساكن A finalize(A a, int64) { ܦܢܐ a; }
    template<typename V> ساكن V vapply(ثابت V& a, ثابت V& x) { ܦܢܐ v_max(a, x); }
    template<typename V> ساكن V vcombine(ثابت V& a, ثابت V& b) { ܦܢܐ v_max(a, b); }
};

template<typename A> بنية OpMin
{
    ساكن A init() { ܦܢܐ Extremes<A>::high(); }
    ساكن A apply(A a, A x) { ܦܢܐ x < a ? x : a; }
    ساكن A combine(A a, A b) { ܦܢܐ b < a ? b : a; }
    ساكن A finalize(A a, int64) { ܦܢܐ a; }
    template<typename V> ساكن V vapply(ثابت V& a, ثابت V& x) { ܦܢܐ v_min(a, x); }
    template<typename V> ساكن V vcombine(ثابت V& a, ثابت V& b) { ܦܢܐ v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> بنية OpSumExpShifted
{
    ساكن A init() { ܦܢܐ A(0); }
    ساكن A apply(A a, A x, A m) { ܦܢܐ a + A(std::exp(x - m)); }
    ساكن A combine(A a, A b) { ܦܢܐ a + b; }
    ساكن A finalize(A a, A m) { ܦܢܐ std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<صنف Op, typename A, منطقي vec = (VecT<A>::ok != 0)> بنية HRow
{
    ساكن A run(A acc, ثابت A* x, int64 n)
    {
        لكل (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ܦܢܐ acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<صنف Op, typename A, منطقي vec = (VecT<A>::ok != 0)> بنية VRow
{
    ساكن فراغ run(A* acc, ثابت A* x, int64 n)
    {
        لكل (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#ܐܢ (CV_SIMD || CV_SIMD_SCALABLE)
template<صنف Op, typename A> بنية HRow<Op, A, ܫܪܪܐ>
{
    ساكن A run(A acc, ثابت A* x, int64 n)
    {
        تعريفنوع typename VecT<A>::vtype V;
        ثابت صحيح L = VTraits<V>::vlanes();
        int64 i = 0;
        ܐܢ (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            لكل (صحيح k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            لكل (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            لكل (صحيح k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        لكل (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ܦܢܐ acc;
    }
};

template<صنف Op, typename A> بنية VRow<Op, A, ܫܪܪܐ>
{
    ساكن فراغ run(A* acc, ثابت A* x, int64 n)
    {
        تعريفنوع typename VecT<A>::vtype V;
        ثابت صحيح L = VTraits<V>::vlanes();
        int64 i = 0;
        لكل (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        لكل (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

بنية Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    صحيح nk;
    صحيح ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    صحيح nr;
    صحيح rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    منطقي innerReduced;      // the innermost axis is reduced (horizontal mode)

    ثابت uchar* src;
    صحيح sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 count;            // reduced elements per output
    int64 nout;             // number of outputs
    BinaryFunc cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
بنية Iter
{
    صحيح n = 0;
    ثابت صحيح* size = nullptr;
    ثابت ptrdiff_t* s = nullptr;
    ثابت ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    فراغ init(صحيح n_, ثابت صحيح* size_, ثابت ptrdiff_t* s_, ثابت ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        لكل (صحيح k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            ܐܢ (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    فراغ next()
    {
        لكل (صحيح k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            ܐܢ (d)
                doff += d[k];
            ܐܢ (++idx[k] < size[k])
                ܦܢܐ;
            soff -= (ptrdiff_t)size[k]*s[k];
            ܐܢ (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> ساكن ثابت A* loadRow(ثابت Plan& pl, ثابت uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    ܐܢ (step != (ptrdiff_t)pl.sesz1)
    {
        لكل (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    ܐܢ (!pl.cvt)
        ܦܢܐ (ثابت A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((صحيح)len, 1), 0);
    ܦܢܐ buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> ساكن فراغ forRow(ثابت Plan& pl, ثابت uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ثابت منطقي direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    ثابت int64 blk = direct ? n : (int64)RND_BLOCK;
    لكل (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
بنية Work
{
    int64 nitems, nsplit;
    مزدوج nstripes;
    منطقي serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, صنف Op, صنف OpM> ساكن فراغ runHorizontal(ثابت Plan& pl, ثابت Work& w, A* res,
                                                                   ثابت A* mbuf, A* partial)
{
    ثابت صحيح nr = pl.nr;
    ثابت int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ثابت ptrdiff_t rstep = pl.rsstep[nr - 1];
    ثابت int64 nsplit = w.nsplit;

    auto body = [&](ثابت Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        ܐܢ (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        ܟܡܐ (it < range.end)
        {
            ثابت int64 o = it / nsplit, s = it - o*nsplit;
            ܐܢ (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ثابت uchar* base = pl.src + ko.soff;
            ثابت A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.count/nsplit, b = (s + 1)*pl.count/nsplit;
            A acc = Op::init();
            ܐܢ (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                ܟܡܐ (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](ثابت A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            ܐܢ (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.count, m);
                ko.next();
            }
            ܐܚܪܢܐ
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    ܐܢ (w.serial)
        body(Range(0, (صحيح)w.nitems));
    ܐܚܪܢܐ
        parallel_for_(Range(0, (صحيح)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, صنف Op, صنف OpM> ساكن فراغ runVertical(ثابت Plan& pl, ثابت Work& w, A* res,
                                                                 ثابت A* mbuf, A* partial)
{
    ثابت صحيح nk = pl.nk;
    ثابت int64 m = pl.ksize[nk - 1];
    ثابت ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ثابت int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ثابت int64 nsplit = w.nsplit;
    int64 rcount = 1;
    لكل (صحيح i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](ثابت Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        لكل (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ثابت int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ثابت ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ثابت A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            لكل (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            ܐܢ (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                لكل (; a < b; a++, ro.next())
                {
                    ثابت uchar* p = pl.src + soff + ro.soff;
                    ثابت A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            ܐܢ (nsplit == 1)
                لكل (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.count, mrow ? mrow[i] : A(0));
            ܐܚܪܢܐ
                لكل (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    ܐܢ (w.serial)
        body(Range(0, (صحيح)w.nitems));
    ܐܚܪܢܐ
        parallel_for_(Range(0, (صحيح)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, صنف Op> بنية Plain
{
    ساكن A run(A acc, ثابت A* x, int64 n, A) { ܦܢܐ HRow<Op, A>::run(acc, x, n); }
    ساكن فراغ vrun(A* acc, ثابت A* x, int64 n, ثابت A*) { VRow<Op, A>::run(acc, x, n); }
    ساكن A finalize(A a, int64 count, A) { ܦܢܐ Op::finalize(a, count); }
};

template<typename A> بنية ShiftedExp
{
    تعريفنوع OpSumExpShifted<A> Op;
    ساكن A run(A acc, ثابت A* x, int64 n, A m)
    {
        لكل (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        ܦܢܐ acc;
    }
    ساكن فراغ vrun(A* acc, ثابت A* x, int64 n, ثابت A* m)
    {
        لكل (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    ساكن A finalize(A a, int64, A m) { ܦܢܐ Op::finalize(a, m); }
};

template<typename A, صنف Op, صنف OpM> ساكن فراغ runPass(ثابت Plan& pl, A* res, ثابت A* mbuf)
{
    ثابت مزدوج work = (مزدوج)pl.nout*(مزدوج)std::max(pl.count, (int64)1);
    ثابت صحيح nthreads = work < RND_SERIAL_WORK ? 1 : getNumThreads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 items;
    ܐܢ (pl.innerReduced)
        items = pl.nout;
    ܐܚܪܢܐ
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        items = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ثابت int64 want = 4*(int64)nthreads;
    ܐܢ (!w.serial && items < want && pl.count > 1)
    {
        int64 rows = pl.innerReduced ? pl.count/4096 : pl.count/16;
        w.nsplit = std::max((int64)1, std::min((want + items - 1)/items, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = items*w.nsplit;
    w.nstripes = std::min((مزدوج)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    ܐܢ (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    ܐܚܪܢܐ
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    ܐܢ (w.nsplit > 1)
    {
        لكل (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            لكل (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.count, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> ساكن فراغ fillResult(A* res, int64 n, A v)
{
    لكل (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> ساكن فراغ runOp(ثابت Plan& pl, صحيح op, A* res)
{
    ܐܢ (pl.count == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        ܦܢܐ;
    }
    بدل (op)
    {
    حالة REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); اكسر;
    حالة REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); اكسر;
    حالة REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); اكسر;
    حالة REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); اكسر;
    حالة REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); اكسر;
    حالة REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); اكسر;
    حالة REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); اكسر;
    حالة REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        اكسر;
    }
    افتراضي:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> ساكن فراغ runMinMax(ثابت Plan& pl, صحيح op, A* res)
{
    ܐܢ (pl.count == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    ܐܚܪܢܐ ܐܢ (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    ܐܚܪܢܐ
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

ساكن فراغ dispatch(ثابت Plan& pl, صحيح op, uchar* res)
{
    ثابت منطقي minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    بدل (pl.adepth)
    {
    حالة CV_32F:
        ܐܢ (minmax) runMinMax<عائم>(pl, op, (عائم*)res); ܐܚܪܢܐ runOp<عائم>(pl, op, (عائم*)res);
        اكسر;
    حالة CV_64F:
        ܐܢ (minmax) runMinMax<مزدوج>(pl, op, (مزدوج*)res); ܐܚܪܢܐ runOp<مزدوج>(pl, op, (مزدوج*)res);
        اكسر;
    حالة CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); اكسر;
    حالة CV_8S:  runMinMax<schar>(pl, op, (schar*)res); اكسر;
    حالة CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); اكسر;
    حالة CV_16S: runMinMax<short>(pl, op, (short*)res); اكسر;
    حالة CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); اكسر;
    حالة CV_32S: runMinMax<صحيح>(pl, op, (صحيح*)res); اكسر;
    حالة CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); اكسر;
    حالة CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); اكسر;
    افتراضي:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

ساكن صحيح accumulatorDepth(صحيح sdepth, صحيح ddepth, صحيح op)
{
    ܐܢ (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        ܐܢ (sdepth == CV_16F || sdepth == CV_16BF)
            ܦܢܐ CV_32F;
        ܦܢܐ sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ثابت منطقي wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    ܦܢܐ wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(صحيح _rtype, ثابت std::vector<صحيح>& _axes, منطقي _keepdims, صحيح _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

فراغ reduce(InputArray _src, OutputArray _dst, ثابت ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح op = params.rtype, dtype = params.dtype;
    ثابت منطقي keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ثابت صحيح dims = src.dims, cn = src.channels();
    ثابت صحيح sdepth = src.depth();
    صحيح ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    ܐܢ ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    منطقي reduced[CV_MAX_DIM] = {ܕܓܠܐ};
    ܐܢ (params.axes.empty())
        std::fill(reduced, reduced + dims, ܫܪܪܐ);
    لكل (صحيح a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        صحيح ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = ܫܪܪܐ;
    }

    صحيح keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    لكل (صحيح i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        ܐܢ (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    ثابت صحيح dtype_ = CV_MAKETYPE(ddepth, cn);
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
    صحيح n = 0;
    صحيح size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    منطقي red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    لكل (صحيح i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.count = 1;
    لكل (صحيح i = 0; i < dims; i++)
    {
        ܐܢ (reduced[i])
            pl.count *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    ܐܢ (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = ܕܓܠܐ;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    ثابت منطقي direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    ܐܢ (!direct)
    {
        resMat.create(1, (صحيح)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    ܐܢ (pl.nout == 0)
        ܦܢܐ;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    صحيح m = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        ܐܢ (size[i] == 0)
        {
            // empty source along this axis
            ܐܢ (red[i])
            {
                pl.count = 0;
                استمر;
            }
            ܦܢܐ;
        }
        ܐܢ (size[i] == 1)
            استمر;
        ܐܢ (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        ܐܚܪܢܐ
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    ܐܢ (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = ܕܓܠܐ;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    لكل (صحيح i = 0; i < m; i++)
    {
        ܐܢ (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        ܐܚܪܢܐ
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    ܐܢ (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    ܐܢ (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        ܐܢ (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
