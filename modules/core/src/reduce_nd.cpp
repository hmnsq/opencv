// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#شامل "precomp.hpp"
#شامل "opencv2/core/hal/intrin.hpp"

#شامل <algorithm>
#شامل <cmath>
#شامل <limits>

فضاینام cv {

فضاینام {

شمارش
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> ساختار VecT { شمارش { ok = 0 }; تعریفنوع T vtype; };
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> ساختار VecT<T> { شمارش { ok = 1 }; تعریفنوع V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(صحیح, v_int32)
CV_RND_VEC(اعشاری, v_float32)
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(دوگانه, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> ساختار OpSum
{
    ایستا A init() { بازگشت A(0); }
    ایستا A apply(A a, A x) { بازگشت a + x; }
    ایستا A combine(A a, A b) { بازگشت a + b; }
    ایستا A finalize(A a, int64) { بازگشت a; }
    template<typename V> ایستا V vapply(ثابت V& a, ثابت V& x) { بازگشت v_add(a, x); }
    template<typename V> ایستا V vcombine(ثابت V& a, ثابت V& b) { بازگشت v_add(a, b); }
};

template<typename A> ساختار OpAvg : OpSum<A>
{
    ایستا A finalize(A a, int64 n) { بازگشت n > 0 ? A(a/(دوگانه)n) : A(0); }
};

template<typename A> ساختار OpLogSum : OpSum<A>
{
    ایستا A finalize(A a, int64) { بازگشت A(std::log(a)); }
};

template<typename A> ساختار OpSum2 : OpSum<A>
{
    ایستا A apply(A a, A x) { بازگشت a + x*x; }
    template<typename V> ایستا V vapply(ثابت V& a, ثابت V& x) { بازگشت v_add(a, v_mul(x, x)); }
};

template<typename A> ساختار OpL2 : OpSum2<A>
{
    ایستا A finalize(A a, int64) { بازگشت A(std::sqrt(a)); }
};

template<typename A> ساختار OpL1 : OpSum<A>
{
    ایستا A apply(A a, A x) { بازگشت a + (x < A(0) ? -x : x); }
    template<typename V> ایستا V vapply(ثابت V& a, ثابت V& x) { بازگشت v_add(a, v_abs(x)); }
};

template<typename A> ساختار OpProd
{
    ایستا A init() { بازگشت A(1); }
    ایستا A apply(A a, A x) { بازگشت a*x; }
    ایستا A combine(A a, A b) { بازگشت a*b; }
    ایستا A finalize(A a, int64) { بازگشت a; }
    template<typename V> ایستا V vapply(ثابت V& a, ثابت V& x) { بازگشت v_mul(a, x); }
    template<typename V> ایستا V vcombine(ثابت V& a, ثابت V& b) { بازگشت v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, منطقی hasInf = std::numeric_limits<A>::has_infinity> ساختار Extremes
{
    ایستا A کم() { بازگشت std::numeric_limits<A>::lowest(); }
    ایستا A بالا() { بازگشت std::numeric_limits<A>::max(); }
};

template<typename A> ساختار Extremes<A, درست>
{
    ایستا A کم() { بازگشت -std::numeric_limits<A>::infinity(); }
    ایستا A بالا() { بازگشت std::numeric_limits<A>::infinity(); }
};

template<typename A> ساختار OpMax
{
    ایستا A init() { بازگشت Extremes<A>::کم(); }
    ایستا A apply(A a, A x) { بازگشت a < x ? x : a; }
    ایستا A combine(A a, A b) { بازگشت a < b ? b : a; }
    ایستا A finalize(A a, int64) { بازگشت a; }
    template<typename V> ایستا V vapply(ثابت V& a, ثابت V& x) { بازگشت v_max(a, x); }
    template<typename V> ایستا V vcombine(ثابت V& a, ثابت V& b) { بازگشت v_max(a, b); }
};

template<typename A> ساختار OpMin
{
    ایستا A init() { بازگشت Extremes<A>::بالا(); }
    ایستا A apply(A a, A x) { بازگشت x < a ? x : a; }
    ایستا A combine(A a, A b) { بازگشت b < a ? b : a; }
    ایستا A finalize(A a, int64) { بازگشت a; }
    template<typename V> ایستا V vapply(ثابت V& a, ثابت V& x) { بازگشت v_min(a, x); }
    template<typename V> ایستا V vcombine(ثابت V& a, ثابت V& b) { بازگشت v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> ساختار OpSumExpShifted
{
    ایستا A init() { بازگشت A(0); }
    ایستا A apply(A a, A x, A m) { بازگشت a + A(std::exp(x - m)); }
    ایستا A combine(A a, A b) { بازگشت a + b; }
    ایستا A finalize(A a, A m) { بازگشت std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<رده Op, typename A, منطقی vec = (VecT<A>::ok != 0)> ساختار HRow
{
    ایستا A run(A acc, ثابت A* x, int64 n)
    {
        برای (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        بازگشت acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<رده Op, typename A, منطقی vec = (VecT<A>::ok != 0)> ساختار VRow
{
    ایستا تهی run(A* acc, ثابت A* x, int64 n)
    {
        برای (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
template<رده Op, typename A> ساختار HRow<Op, A, درست>
{
    ایستا A run(A acc, ثابت A* x, int64 n)
    {
        تعریفنوع typename VecT<A>::vtype V;
        ثابت صحیح L = VTraits<V>::vlanes();
        int64 i = 0;
        اگر (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            برای (صحیح k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            برای (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            برای (صحیح k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        برای (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        بازگشت acc;
    }
};

template<رده Op, typename A> ساختار VRow<Op, A, درست>
{
    ایستا تهی run(A* acc, ثابت A* x, int64 n)
    {
        تعریفنوع typename VecT<A>::vtype V;
        ثابت صحیح L = VTraits<V>::vlanes();
        int64 i = 0;
        برای (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        برای (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

ساختار Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    صحیح nk;
    صحیح ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    صحیح nr;
    صحیح rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    منطقی innerReduced;      // the innermost axis is reduced (horizontal mode)

    ثابت uchar* src;
    صحیح sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 شمار_2;            // reduced elements per output
    int64 nout;             // number of outputs
    دودویی_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
ساختار Iter
{
    صحیح n = 0;
    ثابت صحیح* size = nullptr;
    ثابت ptrdiff_t* s = nullptr;
    ثابت ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    تهی init(صحیح n_, ثابت صحیح* size_, ثابت ptrdiff_t* s_, ثابت ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        برای (صحیح k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            اگر (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    تهی next()
    {
        برای (صحیح k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            اگر (d)
                doff += d[k];
            اگر (++idx[k] < size[k])
                بازگشت;
            soff -= (ptrdiff_t)size[k]*s[k];
            اگر (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> ایستا ثابت A* loadRow(ثابت Plan& pl, ثابت uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    اگر (step != (ptrdiff_t)pl.sesz1)
    {
        برای (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    اگر (!pl.cvt)
        بازگشت (ثابت A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((صحیح)len, 1), 0);
    بازگشت buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> ایستا تهی forRow(ثابت Plan& pl, ثابت uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ثابت منطقی direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    ثابت int64 blk = direct ? n : (int64)RND_BLOCK;
    برای (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
ساختار Work
{
    int64 nitems, nsplit;
    دوگانه nstripes;
    منطقی serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, رده Op, رده OpM> ایستا تهی runHorizontal(ثابت Plan& pl, ثابت Work& w, A* res,
                                                                   ثابت A* mbuf, A* partial)
{
    ثابت صحیح nr = pl.nr;
    ثابت int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ثابت ptrdiff_t rstep = pl.rsstep[nr - 1];
    ثابت int64 nsplit = w.nsplit;

    auto body = [&](ثابت Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        اگر (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        وقتی (it < range.end)
        {
            ثابت int64 o = it / nsplit, s = it - o*nsplit;
            اگر (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ثابت uchar* base = pl.src + ko.soff;
            ثابت A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.شمار_2/nsplit, b = (s + 1)*pl.شمار_2/nsplit;
            A acc = Op::init();
            اگر (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                وقتی (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](ثابت A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            اگر (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.شمار_2, m);
                ko.next();
            }
            وگرنه
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    اگر (w.serial)
        body(Range(0, (صحیح)w.nitems));
    وگرنه
        parallel_for_(Range(0, (صحیح)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, رده Op, رده OpM> ایستا تهی runVertical(ثابت Plan& pl, ثابت Work& w, A* res,
                                                                 ثابت A* mbuf, A* partial)
{
    ثابت صحیح nk = pl.nk;
    ثابت int64 m = pl.ksize[nk - 1];
    ثابت ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ثابت int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ثابت int64 nsplit = w.nsplit;
    int64 rcount = 1;
    برای (صحیح i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](ثابت Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        برای (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ثابت int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ثابت ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ثابت A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            برای (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            اگر (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                برای (; a < b; a++, ro.next())
                {
                    ثابت uchar* p = pl.src + soff + ro.soff;
                    ثابت A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            اگر (nsplit == 1)
                برای (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.شمار_2, mrow ? mrow[i] : A(0));
            وگرنه
                برای (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    اگر (w.serial)
        body(Range(0, (صحیح)w.nitems));
    وگرنه
        parallel_for_(Range(0, (صحیح)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, رده Op> ساختار Plain
{
    ایستا A run(A acc, ثابت A* x, int64 n, A) { بازگشت HRow<Op, A>::run(acc, x, n); }
    ایستا تهی vrun(A* acc, ثابت A* x, int64 n, ثابت A*) { VRow<Op, A>::run(acc, x, n); }
    ایستا A finalize(A a, int64 شمار_2, A) { بازگشت Op::finalize(a, شمار_2); }
};

template<typename A> ساختار ShiftedExp
{
    تعریفنوع OpSumExpShifted<A> Op;
    ایستا A run(A acc, ثابت A* x, int64 n, A m)
    {
        برای (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        بازگشت acc;
    }
    ایستا تهی vrun(A* acc, ثابت A* x, int64 n, ثابت A* m)
    {
        برای (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    ایستا A finalize(A a, int64, A m) { بازگشت Op::finalize(a, m); }
};

template<typename A, رده Op, رده OpM> ایستا تهی runPass(ثابت Plan& pl, A* res, ثابت A* mbuf)
{
    ثابت دوگانه work = (دوگانه)pl.nout*(دوگانه)std::max(pl.شمار_2, (int64)1);
    ثابت صحیح nthreads = work < RND_SERIAL_WORK ? 1 : get_رقم_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 اقلام;
    اگر (pl.innerReduced)
        اقلام = pl.nout;
    وگرنه
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        اقلام = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ثابت int64 want = 4*(int64)nthreads;
    اگر (!w.serial && اقلام < want && pl.شمار_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.شمار_2/4096 : pl.شمار_2/16;
        w.nsplit = std::max((int64)1, std::min((want + اقلام - 1)/اقلام, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = اقلام*w.nsplit;
    w.nstripes = std::min((دوگانه)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    اگر (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    وگرنه
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    اگر (w.nsplit > 1)
    {
        برای (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            برای (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.شمار_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> ایستا تهی fillResult(A* res, int64 n, A v)
{
    برای (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> ایستا تهی runOp(ثابت Plan& pl, صحیح op, A* res)
{
    اگر (pl.شمار_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        بازگشت;
    }
    انتخاب (op)
    {
    حالت REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); بشکن;
    حالت REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); بشکن;
    حالت REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); بشکن;
    حالت REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); بشکن;
    حالت REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); بشکن;
    حالت REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); بشکن;
    حالت REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); بشکن;
    حالت REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        بشکن;
    }
    پیشفرض:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> ایستا تهی runMinMax(ثابت Plan& pl, صحیح op, A* res)
{
    اگر (pl.شمار_2 == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    وگرنه اگر (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    وگرنه
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

ایستا تهی dispatch(ثابت Plan& pl, صحیح op, uchar* res)
{
    ثابت منطقی minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    انتخاب (pl.adepth)
    {
    حالت CV_32F:
        اگر (minmax) runMinMax<اعشاری>(pl, op, (اعشاری*)res); وگرنه runOp<اعشاری>(pl, op, (اعشاری*)res);
        بشکن;
    حالت CV_64F:
        اگر (minmax) runMinMax<دوگانه>(pl, op, (دوگانه*)res); وگرنه runOp<دوگانه>(pl, op, (دوگانه*)res);
        بشکن;
    حالت CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); بشکن;
    حالت CV_8S:  runMinMax<schar>(pl, op, (schar*)res); بشکن;
    حالت CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); بشکن;
    حالت CV_16S: runMinMax<short>(pl, op, (short*)res); بشکن;
    حالت CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); بشکن;
    حالت CV_32S: runMinMax<صحیح>(pl, op, (صحیح*)res); بشکن;
    حالت CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); بشکن;
    حالت CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); بشکن;
    پیشفرض:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

ایستا صحیح accumulatorDepth(صحیح sdepth, صحیح ddepth, صحیح op)
{
    اگر (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        اگر (sdepth == CV_16F || sdepth == CV_16BF)
            بازگشت CV_32F;
        بازگشت sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ثابت منطقی wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    بازگشت wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(صحیح _rtype, ثابت std::vector<صحیح>& _axes, منطقی _keepdims, صحیح _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

تهی reduce(درون_داد_Array _src, OutputArray _dst, ثابت ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح op = params.rtype, dtype = params.dtype;
    ثابت منطقی keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ثابت صحیح dims = src.dims, cn = src.channels();
    ثابت صحیح sdepth = src.depth();
    صحیح ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    اگر ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    منطقی reduced[CV_MAX_DIM] = {نادرست};
    اگر (params.axes.empty())
        std::fill(reduced, reduced + dims, درست);
    برای (صحیح a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        صحیح ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = درست;
    }

    صحیح keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    برای (صحیح i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        اگر (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    ثابت صحیح dtype_ = CV_MAKETYPE(ddepth, cn);
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
    صحیح n = 0;
    صحیح size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    منطقی red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    برای (صحیح i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.شمار_2 = 1;
    برای (صحیح i = 0; i < dims; i++)
    {
        اگر (reduced[i])
            pl.شمار_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    اگر (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = نادرست;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    ثابت منطقی direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    اگر (!direct)
    {
        resMat.create(1, (صحیح)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    اگر (pl.nout == 0)
        بازگشت;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    صحیح m = 0;
    برای (صحیح i = 0; i < n; i++)
    {
        اگر (size[i] == 0)
        {
            // empty source along this axis
            اگر (red[i])
            {
                pl.شمار_2 = 0;
                ادامه;
            }
            بازگشت;
        }
        اگر (size[i] == 1)
            ادامه;
        اگر (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        وگرنه
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    اگر (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = نادرست;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    برای (صحیح i = 0; i < m; i++)
    {
        اگر (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        وگرنه
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    اگر (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    اگر (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        اگر (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
