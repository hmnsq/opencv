// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/hal/intrin.hpp"

#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <cmath>
#অন্তর্ভাব <limits>

নামাকাশ cv {

নামাকাশ {

গণনা
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> কাঠামো VecT { গণনা { ok = 0 }; প্রকারলেখ T vtype; };
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> কাঠামো VecT<T> { গণনা { ok = 1 }; প্রকারলেখ V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(পূর্ণসংখ্যা, v_int32)
CV_RND_VEC(দশমিক, v_float32)
#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(দ্বিগুণ, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> কাঠামো Op_যোগ
{
    স্থির A init() { ফেরত A(0); }
    স্থির A apply(A a, A x) { ফেরত a + x; }
    স্থির A combine(A a, A b) { ফেরত a + b; }
    স্থির A finalize(A a, int64) { ফেরত a; }
    template<typename V> স্থির V vapply(ধ্রুবক V& a, ধ্রুবক V& x) { ফেরত v_add(a, x); }
    template<typename V> স্থির V vcombine(ধ্রুবক V& a, ধ্রুবক V& b) { ফেরত v_add(a, b); }
};

template<typename A> কাঠামো OpAvg : Op_যোগ<A>
{
    স্থির A finalize(A a, int64 n) { ফেরত n > 0 ? A(a/(দ্বিগুণ)n) : A(0); }
};

template<typename A> কাঠামো Op_Log_যোগ : Op_যোগ<A>
{
    স্থির A finalize(A a, int64) { ফেরত A(std::log(a)); }
};

template<typename A> কাঠামো OpSum2 : Op_যোগ<A>
{
    স্থির A apply(A a, A x) { ফেরত a + x*x; }
    template<typename V> স্থির V vapply(ধ্রুবক V& a, ধ্রুবক V& x) { ফেরত v_add(a, v_mul(x, x)); }
};

template<typename A> কাঠামো OpL2 : OpSum2<A>
{
    স্থির A finalize(A a, int64) { ফেরত A(std::sqrt(a)); }
};

template<typename A> কাঠামো OpL1 : Op_যোগ<A>
{
    স্থির A apply(A a, A x) { ফেরত a + (x < A(0) ? -x : x); }
    template<typename V> স্থির V vapply(ধ্রুবক V& a, ধ্রুবক V& x) { ফেরত v_add(a, v_abs(x)); }
};

template<typename A> কাঠামো OpProd
{
    স্থির A init() { ফেরত A(1); }
    স্থির A apply(A a, A x) { ফেরত a*x; }
    স্থির A combine(A a, A b) { ফেরত a*b; }
    স্থির A finalize(A a, int64) { ফেরত a; }
    template<typename V> স্থির V vapply(ধ্রুবক V& a, ধ্রুবক V& x) { ফেরত v_mul(a, x); }
    template<typename V> স্থির V vcombine(ধ্রুবক V& a, ধ্রুবক V& b) { ফেরত v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, যৌক্তিক hasInf = std::numeric_limits<A>::has_infinity> কাঠামো Extremes
{
    স্থির A নিচের() { ফেরত std::numeric_limits<A>::lowest(); }
    স্থির A উপরের() { ফেরত std::numeric_limits<A>::max(); }
};

template<typename A> কাঠামো Extremes<A, সত্য>
{
    স্থির A নিচের() { ফেরত -std::numeric_limits<A>::infinity(); }
    স্থির A উপরের() { ফেরত std::numeric_limits<A>::infinity(); }
};

template<typename A> কাঠামো OpMax
{
    স্থির A init() { ফেরত Extremes<A>::নিচের(); }
    স্থির A apply(A a, A x) { ফেরত a < x ? x : a; }
    স্থির A combine(A a, A b) { ফেরত a < b ? b : a; }
    স্থির A finalize(A a, int64) { ফেরত a; }
    template<typename V> স্থির V vapply(ধ্রুবক V& a, ধ্রুবক V& x) { ফেরত v_max(a, x); }
    template<typename V> স্থির V vcombine(ধ্রুবক V& a, ধ্রুবক V& b) { ফেরত v_max(a, b); }
};

template<typename A> কাঠামো OpMin
{
    স্থির A init() { ফেরত Extremes<A>::উপরের(); }
    স্থির A apply(A a, A x) { ফেরত x < a ? x : a; }
    স্থির A combine(A a, A b) { ফেরত b < a ? b : a; }
    স্থির A finalize(A a, int64) { ফেরত a; }
    template<typename V> স্থির V vapply(ধ্রুবক V& a, ধ্রুবক V& x) { ফেরত v_min(a, x); }
    template<typename V> স্থির V vcombine(ধ্রুবক V& a, ধ্রুবক V& b) { ফেরত v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> কাঠামো Op_যোগ_Exp_Shifted
{
    স্থির A init() { ফেরত A(0); }
    স্থির A apply(A a, A x, A m) { ফেরত a + A(std::exp(x - m)); }
    স্থির A combine(A a, A b) { ফেরত a + b; }
    স্থির A finalize(A a, A m) { ফেরত std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<শ্রেণী Op, typename A, যৌক্তিক vec = (VecT<A>::ok != 0)> কাঠামো HRow
{
    স্থির A run(A acc, ধ্রুবক A* x, int64 n)
    {
        জন্য (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ফেরত acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<শ্রেণী Op, typename A, যৌক্তিক vec = (VecT<A>::ok != 0)> কাঠামো VRow
{
    স্থির শূন্য run(A* acc, ধ্রুবক A* x, int64 n)
    {
        জন্য (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)
template<শ্রেণী Op, typename A> কাঠামো HRow<Op, A, সত্য>
{
    স্থির A run(A acc, ধ্রুবক A* x, int64 n)
    {
        প্রকারলেখ typename VecT<A>::vtype V;
        ধ্রুবক পূর্ণসংখ্যা L = VTraits<V>::vlanes();
        int64 i = 0;
        যদি (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            জন্য (পূর্ণসংখ্যা k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            জন্য (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            জন্য (পূর্ণসংখ্যা k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        জন্য (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ফেরত acc;
    }
};

template<শ্রেণী Op, typename A> কাঠামো VRow<Op, A, সত্য>
{
    স্থির শূন্য run(A* acc, ধ্রুবক A* x, int64 n)
    {
        প্রকারলেখ typename VecT<A>::vtype V;
        ধ্রুবক পূর্ণসংখ্যা L = VTraits<V>::vlanes();
        int64 i = 0;
        জন্য (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        জন্য (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

কাঠামো Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    পূর্ণসংখ্যা nk;
    পূর্ণসংখ্যা ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    পূর্ণসংখ্যা nr;
    পূর্ণসংখ্যা rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    যৌক্তিক innerReduced;      // the innermost axis is reduced (horizontal mode)

    ধ্রুবক uchar* src;
    পূর্ণসংখ্যা sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 গুনতি_2;            // reduced elements per output
    int64 nout;             // number of outputs
    দ্বিমিক_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
কাঠামো Iter
{
    পূর্ণসংখ্যা n = 0;
    ধ্রুবক পূর্ণসংখ্যা* size = nullptr;
    ধ্রুবক ptrdiff_t* s = nullptr;
    ধ্রুবক ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    শূন্য init(পূর্ণসংখ্যা n_, ধ্রুবক পূর্ণসংখ্যা* আকার_4, ধ্রুবক ptrdiff_t* s_, ধ্রুবক ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = আকার_4; s = s_; d = d_;
        soff = doff = 0;
        জন্য (পূর্ণসংখ্যা k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            যদি (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    শূন্য next()
    {
        জন্য (পূর্ণসংখ্যা k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            যদি (d)
                doff += d[k];
            যদি (++idx[k] < size[k])
                ফেরত;
            soff -= (ptrdiff_t)size[k]*s[k];
            যদি (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> স্থির ধ্রুবক A* loadRow(ধ্রুবক Plan& pl, ধ্রুবক uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    যদি (step != (ptrdiff_t)pl.sesz1)
    {
        জন্য (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    যদি (!pl.cvt)
        ফেরত (ধ্রুবক A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, আকার_2((পূর্ণসংখ্যা)len, 1), 0);
    ফেরত buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> স্থির শূন্য forRow(ধ্রুবক Plan& pl, ধ্রুবক uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ধ্রুবক যৌক্তিক direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    ধ্রুবক int64 blk = direct ? n : (int64)RND_BLOCK;
    জন্য (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
কাঠামো Work
{
    int64 nitems, nsplit;
    দ্বিগুণ nstripes;
    যৌক্তিক serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, শ্রেণী Op, শ্রেণী OpM> স্থির শূন্য runHorizontal(ধ্রুবক Plan& pl, ধ্রুবক Work& w, A* res,
                                                                   ধ্রুবক A* mbuf, A* partial)
{
    ধ্রুবক পূর্ণসংখ্যা nr = pl.nr;
    ধ্রুবক int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ধ্রুবক ptrdiff_t rstep = pl.rsstep[nr - 1];
    ধ্রুবক int64 nsplit = w.nsplit;

    auto body = [&](ধ্রুবক Range& range)
    {
        Auto_বাফার<A> bufA(RND_BLOCK);
        Auto_বাফার<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        যদি (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        যখন (it < range.end)
        {
            ধ্রুবক int64 o = it / nsplit, s = it - o*nsplit;
            যদি (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ধ্রুবক uchar* base = pl.src + ko.soff;
            ধ্রুবক A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.গুনতি_2/nsplit, b = (s + 1)*pl.গুনতি_2/nsplit;
            A acc = Op::init();
            যদি (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                যখন (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.তথ্য(), bufT.তথ্য(),
                              [&](ধ্রুবক A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            যদি (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.গুনতি_2, m);
                ko.next();
            }
            অন্যথা
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    যদি (w.serial)
        body(Range(0, (পূর্ণসংখ্যা)w.nitems));
    অন্যথা
        parallel_for_(Range(0, (পূর্ণসংখ্যা)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, শ্রেণী Op, শ্রেণী OpM> স্থির শূন্য runVertical(ধ্রুবক Plan& pl, ধ্রুবক Work& w, A* res,
                                                                 ধ্রুবক A* mbuf, A* partial)
{
    ধ্রুবক পূর্ণসংখ্যা nk = pl.nk;
    ধ্রুবক int64 m = pl.ksize[nk - 1];
    ধ্রুবক ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ধ্রুবক int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ধ্রুবক int64 nsplit = w.nsplit;
    int64 rcount = 1;
    জন্য (পূর্ণসংখ্যা i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](ধ্রুবক Range& range)
    {
        Auto_বাফার<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        Auto_বাফার<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.তথ্য();
        Iter ko, ro;
        জন্য (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ধ্রুবক int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ধ্রুবক ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ধ্রুবক A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            জন্য (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            যদি (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                জন্য (; a < b; a++, ro.next())
                {
                    ধ্রুবক uchar* p = pl.src + soff + ro.soff;
                    ধ্রুবক A* x = loadRow<A>(pl, p, kstep, len, bufA.তথ্য(), bufT.তথ্য());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            যদি (nsplit == 1)
                জন্য (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.গুনতি_2, mrow ? mrow[i] : A(0));
            অন্যথা
                জন্য (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    যদি (w.serial)
        body(Range(0, (পূর্ণসংখ্যা)w.nitems));
    অন্যথা
        parallel_for_(Range(0, (পূর্ণসংখ্যা)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, শ্রেণী Op> কাঠামো Plain
{
    স্থির A run(A acc, ধ্রুবক A* x, int64 n, A) { ফেরত HRow<Op, A>::run(acc, x, n); }
    স্থির শূন্য vrun(A* acc, ধ্রুবক A* x, int64 n, ধ্রুবক A*) { VRow<Op, A>::run(acc, x, n); }
    স্থির A finalize(A a, int64 গুনতি_2, A) { ফেরত Op::finalize(a, গুনতি_2); }
};

template<typename A> কাঠামো ShiftedExp
{
    প্রকারলেখ Op_যোগ_Exp_Shifted<A> Op;
    স্থির A run(A acc, ধ্রুবক A* x, int64 n, A m)
    {
        জন্য (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        ফেরত acc;
    }
    স্থির শূন্য vrun(A* acc, ধ্রুবক A* x, int64 n, ধ্রুবক A* m)
    {
        জন্য (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    স্থির A finalize(A a, int64, A m) { ফেরত Op::finalize(a, m); }
};

template<typename A, শ্রেণী Op, শ্রেণী OpM> স্থির শূন্য runPass(ধ্রুবক Plan& pl, A* res, ধ্রুবক A* mbuf)
{
    ধ্রুবক দ্বিগুণ work = (দ্বিগুণ)pl.nout*(দ্বিগুণ)std::max(pl.গুনতি_2, (int64)1);
    ধ্রুবক পূর্ণসংখ্যা nthreads = work < RND_SERIAL_WORK ? 1 : get_অঙ্ক_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 বস্তুসমূহ;
    যদি (pl.innerReduced)
        বস্তুসমূহ = pl.nout;
    অন্যথা
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        বস্তুসমূহ = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ধ্রুবক int64 want = 4*(int64)nthreads;
    যদি (!w.serial && বস্তুসমূহ < want && pl.গুনতি_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.গুনতি_2/4096 : pl.গুনতি_2/16;
        w.nsplit = std::max((int64)1, std::min((want + বস্তুসমূহ - 1)/বস্তুসমূহ, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = বস্তুসমূহ*w.nsplit;
    w.nstripes = std::min((দ্বিগুণ)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    Auto_বাফার<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    যদি (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.তথ্য());
    অন্যথা
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.তথ্য());

    যদি (w.nsplit > 1)
    {
        জন্য (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            জন্য (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.গুনতি_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> স্থির শূন্য fill_ফলাফল(A* res, int64 n, A v)
{
    জন্য (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> স্থির শূন্য runOp(ধ্রুবক Plan& pl, পূর্ণসংখ্যা op, A* res)
{
    যদি (pl.গুনতি_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_যোগ || op == REDUCE_LOG_যোগ_EXP ? OpMax<A>::init() : A(0);
        fill_ফলাফল(res, pl.nout, v);
        ফেরত;
    }
    নির্বাচন (op)
    {
    ক্ষেত্র REDUCE_যোগ:   runPass<A, Op_যোগ<A>, Plain<A, Op_যোগ<A> > >(pl, res, nullptr); বিরতি;
    ক্ষেত্র REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); বিরতি;
    ক্ষেত্র REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); বিরতি;
    ক্ষেত্র REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); বিরতি;
    ক্ষেত্র REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); বিরতি;
    ক্ষেত্র REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); বিরতি;
    ক্ষেত্র REDUCE_LOG_যোগ: runPass<A, Op_Log_যোগ<A>, Plain<A, Op_Log_যোগ<A> > >(pl, res, nullptr); বিরতি;
    ক্ষেত্র REDUCE_LOG_যোগ_EXP:
    {
        Auto_বাফার<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.তথ্য(), nullptr);
        runPass<A, Op_যোগ_Exp_Shifted<A>, ShiftedExp<A> >(pl, res, mbuf.তথ্য());
        বিরতি;
    }
    পূর্বনির্ধারিত:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> স্থির শূন্য runMinMax(ধ্রুবক Plan& pl, পূর্ণসংখ্যা op, A* res)
{
    যদি (pl.গুনতি_2 == 0)
        fill_ফলাফল(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    অন্যথা যদি (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    অন্যথা
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

স্থির শূন্য dispatch(ধ্রুবক Plan& pl, পূর্ণসংখ্যা op, uchar* res)
{
    ধ্রুবক যৌক্তিক minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    নির্বাচন (pl.adepth)
    {
    ক্ষেত্র CV_32F:
        যদি (minmax) runMinMax<দশমিক>(pl, op, (দশমিক*)res); অন্যথা runOp<দশমিক>(pl, op, (দশমিক*)res);
        বিরতি;
    ক্ষেত্র CV_64F:
        যদি (minmax) runMinMax<দ্বিগুণ>(pl, op, (দ্বিগুণ*)res); অন্যথা runOp<দ্বিগুণ>(pl, op, (দ্বিগুণ*)res);
        বিরতি;
    ক্ষেত্র CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); বিরতি;
    ক্ষেত্র CV_8S:  runMinMax<schar>(pl, op, (schar*)res); বিরতি;
    ক্ষেত্র CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); বিরতি;
    ক্ষেত্র CV_16S: runMinMax<short>(pl, op, (short*)res); বিরতি;
    ক্ষেত্র CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); বিরতি;
    ক্ষেত্র CV_32S: runMinMax<পূর্ণসংখ্যা>(pl, op, (পূর্ণসংখ্যা*)res); বিরতি;
    ক্ষেত্র CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); বিরতি;
    ক্ষেত্র CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); বিরতি;
    পূর্বনির্ধারিত:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

স্থির পূর্ণসংখ্যা accumulatorDepth(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth, পূর্ণসংখ্যা op)
{
    যদি (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        যদি (sdepth == CV_16F || sdepth == CV_16BF)
            ফেরত CV_32F;
        ফেরত sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ধ্রুবক যৌক্তিক wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    ফেরত wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(পূর্ণসংখ্যা _rtype, ধ্রুবক std::vector<পূর্ণসংখ্যা>& _axes, যৌক্তিক _keepdims, পূর্ণসংখ্যা _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

শূন্য reduce(প্রবেশ_Array _src, OutputArray _dst, ধ্রুবক ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ধ্রুবক পূর্ণসংখ্যা op = params.rtype, dtype = params.dtype;
    ধ্রুবক যৌক্তিক keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_যোগ && op <= REDUCE_LOG_যোগ_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ধ্রুবক পূর্ণসংখ্যা dims = src.dims, cn = src.channels();
    ধ্রুবক পূর্ণসংখ্যা sdepth = src.depth();
    পূর্ণসংখ্যা ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    যদি ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    যৌক্তিক reduced[CV_MAX_DIM] = {মিথ্যা};
    যদি (params.axes.empty())
        std::fill(reduced, reduced + dims, সত্য);
    জন্য (পূর্ণসংখ্যা a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        পূর্ণসংখ্যা ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = সত্য;
    }

    পূর্ণসংখ্যা keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        যদি (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    ধ্রুবক পূর্ণসংখ্যা dtype_ = CV_MAKETYPE(ddepth, cn);
    _dst.create(nout, outShape, dtype_);
    Mat dst = _dst.getMat();

    // The results are computed into a contiguous accumulator-typed buffer laid out like the
    // keepdims output (channels innermost), then converted into dst if needed.
    Plan pl;
    pl.src = src.তথ্য;
    pl.sdepth = sdepth == CV_Bool ? CV_8U : sdepth;
    pl.adepth = accumulatorDepth(sdepth, ddepth, op);
    pl.sesz1 = CV_ELEM_SIZE1(sdepth);
    pl.cvt = pl.sdepth == pl.adepth ? nullptr : getConvertFunc(pl.sdepth, pl.adepth);
    CV_Assert(pl.sdepth == pl.adepth || pl.cvt);

    // gather the axes (+ the channel axis), outer first
    পূর্ণসংখ্যা n = 0;
    পূর্ণসংখ্যা size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    যৌক্তিক red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    জন্য (পূর্ণসংখ্যা i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.গুনতি_2 = 1;
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
    {
        যদি (reduced[i])
            pl.গুনতি_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    যদি (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = মিথ্যা;
        n++;
    }

    pl.nout = (int64)dst.যোগ()*cn;
    ধ্রুবক যৌক্তিক direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.তথ্য;
    যদি (!direct)
    {
        resMat.create(1, (পূর্ণসংখ্যা)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.তথ্য;
    }
    যদি (pl.nout == 0)
        ফেরত;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    পূর্ণসংখ্যা m = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < n; i++)
    {
        যদি (size[i] == 0)
        {
            // empty source along this axis
            যদি (red[i])
            {
                pl.গুনতি_2 = 0;
                চালু;
            }
            ফেরত;
        }
        যদি (size[i] == 1)
            চালু;
        যদি (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        অন্যথা
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    যদি (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = মিথ্যা;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < m; i++)
    {
        যদি (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        অন্যথা
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    যদি (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    যদি (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.তথ্য);
        যদি (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.তথ্য);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
