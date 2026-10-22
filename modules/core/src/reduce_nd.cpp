// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"

#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <cmath>
#ಅನ್ತರ್ಭಾವ <limits>

ನಾಮಾಕಾಶ cv {

ನಾಮಾಕಾಶ {

ಗಣನಾ
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> ಸಂರಚನಾ VecT { ಗಣನಾ { ok = 0 }; ಪ್ರಕಾರನಿರ್ದೇಶ T vtype; };
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> ಸಂರಚನಾ VecT<T> { ಗಣನಾ { ok = 1 }; ಪ್ರಕಾರನಿರ್ದೇಶ V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(ಪೂರ್ಣಾಙ್ಕ, v_int32)
CV_RND_VEC(ಭಿನ್ನ, v_float32)
#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(ದ್ವಿಗುಣ, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> ಸಂರಚನಾ OpSum
{
    ಸ್ಥಿರ A init() { ಪ್ರತ್ಯಾಗಮ A(0); }
    ಸ್ಥಿರ A apply(A a, A x) { ಪ್ರತ್ಯಾಗಮ a + x; }
    ಸ್ಥಿರ A combine(A a, A b) { ಪ್ರತ್ಯಾಗಮ a + b; }
    ಸ್ಥಿರ A finalize(A a, int64) { ಪ್ರತ್ಯಾಗಮ a; }
    template<typename V> ಸ್ಥಿರ V vapply(ಧ್ರುವ V& a, ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_add(a, x); }
    template<typename V> ಸ್ಥಿರ V vcombine(ಧ್ರುವ V& a, ಧ್ರುವ V& b) { ಪ್ರತ್ಯಾಗಮ v_add(a, b); }
};

template<typename A> ಸಂರಚನಾ OpAvg : OpSum<A>
{
    ಸ್ಥಿರ A finalize(A a, int64 n) { ಪ್ರತ್ಯಾಗಮ n > 0 ? A(a/(ದ್ವಿಗುಣ)n) : A(0); }
};

template<typename A> ಸಂರಚನಾ OpLogSum : OpSum<A>
{
    ಸ್ಥಿರ A finalize(A a, int64) { ಪ್ರತ್ಯಾಗಮ A(std::log(a)); }
};

template<typename A> ಸಂರಚನಾ OpSum2 : OpSum<A>
{
    ಸ್ಥಿರ A apply(A a, A x) { ಪ್ರತ್ಯಾಗಮ a + x*x; }
    template<typename V> ಸ್ಥಿರ V vapply(ಧ್ರುವ V& a, ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_add(a, v_mul(x, x)); }
};

template<typename A> ಸಂರಚನಾ OpL2 : OpSum2<A>
{
    ಸ್ಥಿರ A finalize(A a, int64) { ಪ್ರತ್ಯಾಗಮ A(std::sqrt(a)); }
};

template<typename A> ಸಂರಚನಾ OpL1 : OpSum<A>
{
    ಸ್ಥಿರ A apply(A a, A x) { ಪ್ರತ್ಯಾಗಮ a + (x < A(0) ? -x : x); }
    template<typename V> ಸ್ಥಿರ V vapply(ಧ್ರುವ V& a, ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_add(a, v_abs(x)); }
};

template<typename A> ಸಂರಚನಾ OpProd
{
    ಸ್ಥಿರ A init() { ಪ್ರತ್ಯಾಗಮ A(1); }
    ಸ್ಥಿರ A apply(A a, A x) { ಪ್ರತ್ಯಾಗಮ a*x; }
    ಸ್ಥಿರ A combine(A a, A b) { ಪ್ರತ್ಯಾಗಮ a*b; }
    ಸ್ಥಿರ A finalize(A a, int64) { ಪ್ರತ್ಯಾಗಮ a; }
    template<typename V> ಸ್ಥಿರ V vapply(ಧ್ರುವ V& a, ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_mul(a, x); }
    template<typename V> ಸ್ಥಿರ V vcombine(ಧ್ರುವ V& a, ಧ್ರುವ V& b) { ಪ್ರತ್ಯಾಗಮ v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, ತಾರ್ಕಿಕ hasInf = std::numeric_limits<A>::has_infinity> ಸಂರಚನಾ Extremes
{
    ಸ್ಥಿರ A ಕೆಳಗಿನ() { ಪ್ರತ್ಯಾಗಮ std::numeric_limits<A>::lowest(); }
    ಸ್ಥಿರ A ಮೇಲಿನ() { ಪ್ರತ್ಯಾಗಮ std::numeric_limits<A>::max(); }
};

template<typename A> ಸಂರಚನಾ Extremes<A, ಸತ್ಯ>
{
    ಸ್ಥಿರ A ಕೆಳಗಿನ() { ಪ್ರತ್ಯಾಗಮ -std::numeric_limits<A>::infinity(); }
    ಸ್ಥಿರ A ಮೇಲಿನ() { ಪ್ರತ್ಯಾಗಮ std::numeric_limits<A>::infinity(); }
};

template<typename A> ಸಂರಚನಾ OpMax
{
    ಸ್ಥಿರ A init() { ಪ್ರತ್ಯಾಗಮ Extremes<A>::ಕೆಳಗಿನ(); }
    ಸ್ಥಿರ A apply(A a, A x) { ಪ್ರತ್ಯಾಗಮ a < x ? x : a; }
    ಸ್ಥಿರ A combine(A a, A b) { ಪ್ರತ್ಯಾಗಮ a < b ? b : a; }
    ಸ್ಥಿರ A finalize(A a, int64) { ಪ್ರತ್ಯಾಗಮ a; }
    template<typename V> ಸ್ಥಿರ V vapply(ಧ್ರುವ V& a, ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_max(a, x); }
    template<typename V> ಸ್ಥಿರ V vcombine(ಧ್ರುವ V& a, ಧ್ರುವ V& b) { ಪ್ರತ್ಯಾಗಮ v_max(a, b); }
};

template<typename A> ಸಂರಚನಾ OpMin
{
    ಸ್ಥಿರ A init() { ಪ್ರತ್ಯಾಗಮ Extremes<A>::ಮೇಲಿನ(); }
    ಸ್ಥಿರ A apply(A a, A x) { ಪ್ರತ್ಯಾಗಮ x < a ? x : a; }
    ಸ್ಥಿರ A combine(A a, A b) { ಪ್ರತ್ಯಾಗಮ b < a ? b : a; }
    ಸ್ಥಿರ A finalize(A a, int64) { ಪ್ರತ್ಯಾಗಮ a; }
    template<typename V> ಸ್ಥಿರ V vapply(ಧ್ರುವ V& a, ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_min(a, x); }
    template<typename V> ಸ್ಥಿರ V vcombine(ಧ್ರುವ V& a, ಧ್ರುವ V& b) { ಪ್ರತ್ಯಾಗಮ v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> ಸಂರಚನಾ OpSumExpShifted
{
    ಸ್ಥಿರ A init() { ಪ್ರತ್ಯಾಗಮ A(0); }
    ಸ್ಥಿರ A apply(A a, A x, A m) { ಪ್ರತ್ಯಾಗಮ a + A(std::exp(x - m)); }
    ಸ್ಥಿರ A combine(A a, A b) { ಪ್ರತ್ಯಾಗಮ a + b; }
    ಸ್ಥಿರ A finalize(A a, A m) { ಪ್ರತ್ಯಾಗಮ std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<ವರ್ಗ Op, typename A, ತಾರ್ಕಿಕ vec = (VecT<A>::ok != 0)> ಸಂರಚನಾ HRow
{
    ಸ್ಥಿರ A run(A acc, ಧ್ರುವ A* x, int64 n)
    {
        ಪ್ರತಿ (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ಪ್ರತ್ಯಾಗಮ acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<ವರ್ಗ Op, typename A, ತಾರ್ಕಿಕ vec = (VecT<A>::ok != 0)> ಸಂರಚನಾ VRow
{
    ಸ್ಥಿರ ಶೂನ್ಯ run(A* acc, ಧ್ರುವ A* x, int64 n)
    {
        ಪ್ರತಿ (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
template<ವರ್ಗ Op, typename A> ಸಂರಚನಾ HRow<Op, A, ಸತ್ಯ>
{
    ಸ್ಥಿರ A run(A acc, ಧ್ರುವ A* x, int64 n)
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ typename VecT<A>::vtype V;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ L = VTraits<V>::vlanes();
        int64 i = 0;
        ಯದಿ (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            ಪ್ರತಿ (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        ಪ್ರತಿ (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ಪ್ರತ್ಯಾಗಮ acc;
    }
};

template<ವರ್ಗ Op, typename A> ಸಂರಚನಾ VRow<Op, A, ಸತ್ಯ>
{
    ಸ್ಥಿರ ಶೂನ್ಯ run(A* acc, ಧ್ರುವ A* x, int64 n)
    {
        ಪ್ರಕಾರನಿರ್ದೇಶ typename VecT<A>::vtype V;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ L = VTraits<V>::vlanes();
        int64 i = 0;
        ಪ್ರತಿ (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        ಪ್ರತಿ (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

ಸಂರಚನಾ Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    ಪೂರ್ಣಾಙ್ಕ nk;
    ಪೂರ್ಣಾಙ್ಕ ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    ಪೂರ್ಣಾಙ್ಕ nr;
    ಪೂರ್ಣಾಙ್ಕ rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    ತಾರ್ಕಿಕ innerReduced;      // the innermost axis is reduced (horizontal mode)

    ಧ್ರುವ uchar* src;
    ಪೂರ್ಣಾಙ್ಕ sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 ಎಣಿಕೆ_2;            // reduced elements per output
    int64 nout;             // number of outputs
    ದ್ವಿಮಾನ_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
ಸಂರಚನಾ Iter
{
    ಪೂರ್ಣಾಙ್ಕ n = 0;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* size = nullptr;
    ಧ್ರುವ ptrdiff_t* s = nullptr;
    ಧ್ರುವ ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    ಶೂನ್ಯ init(ಪೂರ್ಣಾಙ್ಕ n_, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* size_, ಧ್ರುವ ptrdiff_t* s_, ಧ್ರುವ ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            ಯದಿ (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    ಶೂನ್ಯ next()
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            ಯದಿ (d)
                doff += d[k];
            ಯದಿ (++idx[k] < size[k])
                ಪ್ರತ್ಯಾಗಮ;
            soff -= (ptrdiff_t)size[k]*s[k];
            ಯದಿ (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> ಸ್ಥಿರ ಧ್ರುವ A* loadRow(ಧ್ರುವ Plan& pl, ಧ್ರುವ uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    ಯದಿ (step != (ptrdiff_t)pl.sesz1)
    {
        ಪ್ರತಿ (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    ಯದಿ (!pl.cvt)
        ಪ್ರತ್ಯಾಗಮ (ಧ್ರುವ A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((ಪೂರ್ಣಾಙ್ಕ)len, 1), 0);
    ಪ್ರತ್ಯಾಗಮ buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> ಸ್ಥಿರ ಶೂನ್ಯ forRow(ಧ್ರುವ Plan& pl, ಧ್ರುವ uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ಧ್ರುವ ತಾರ್ಕಿಕ direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    ಧ್ರುವ int64 blk = direct ? n : (int64)RND_BLOCK;
    ಪ್ರತಿ (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
ಸಂರಚನಾ Work
{
    int64 nitems, nsplit;
    ದ್ವಿಗುಣ nstripes;
    ತಾರ್ಕಿಕ serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, ವರ್ಗ Op, ವರ್ಗ OpM> ಸ್ಥಿರ ಶೂನ್ಯ runHorizontal(ಧ್ರುವ Plan& pl, ಧ್ರುವ Work& w, A* res,
                                                                   ಧ್ರುವ A* mbuf, A* partial)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nr = pl.nr;
    ಧ್ರುವ int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ಧ್ರುವ ptrdiff_t rstep = pl.rsstep[nr - 1];
    ಧ್ರುವ int64 nsplit = w.nsplit;

    auto body = [&](ಧ್ರುವ Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        ಯದಿ (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        ಯಾವತ್ (it < range.end)
        {
            ಧ್ರುವ int64 o = it / nsplit, s = it - o*nsplit;
            ಯದಿ (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ಧ್ರುವ uchar* base = pl.src + ko.soff;
            ಧ್ರುವ A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.ಎಣಿಕೆ_2/nsplit, b = (s + 1)*pl.ಎಣಿಕೆ_2/nsplit;
            A acc = Op::init();
            ಯದಿ (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                ಯಾವತ್ (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](ಧ್ರುವ A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            ಯದಿ (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.ಎಣಿಕೆ_2, m);
                ko.next();
            }
            ಅನ್ಯಥಾ
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    ಯದಿ (w.serial)
        body(Range(0, (ಪೂರ್ಣಾಙ್ಕ)w.nitems));
    ಅನ್ಯಥಾ
        parallel_for_(Range(0, (ಪೂರ್ಣಾಙ್ಕ)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, ವರ್ಗ Op, ವರ್ಗ OpM> ಸ್ಥಿರ ಶೂನ್ಯ runVertical(ಧ್ರುವ Plan& pl, ಧ್ರುವ Work& w, A* res,
                                                                 ಧ್ರುವ A* mbuf, A* partial)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nk = pl.nk;
    ಧ್ರುವ int64 m = pl.ksize[nk - 1];
    ಧ್ರುವ ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ಧ್ರುವ int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ಧ್ರುವ int64 nsplit = w.nsplit;
    int64 rcount = 1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](ಧ್ರುವ Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        ಪ್ರತಿ (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ಧ್ರುವ int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ಧ್ರುವ ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ಧ್ರುವ A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            ಪ್ರತಿ (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            ಯದಿ (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                ಪ್ರತಿ (; a < b; a++, ro.next())
                {
                    ಧ್ರುವ uchar* p = pl.src + soff + ro.soff;
                    ಧ್ರುವ A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            ಯದಿ (nsplit == 1)
                ಪ್ರತಿ (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.ಎಣಿಕೆ_2, mrow ? mrow[i] : A(0));
            ಅನ್ಯಥಾ
                ಪ್ರತಿ (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    ಯದಿ (w.serial)
        body(Range(0, (ಪೂರ್ಣಾಙ್ಕ)w.nitems));
    ಅನ್ಯಥಾ
        parallel_for_(Range(0, (ಪೂರ್ಣಾಙ್ಕ)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, ವರ್ಗ Op> ಸಂರಚನಾ Plain
{
    ಸ್ಥಿರ A run(A acc, ಧ್ರುವ A* x, int64 n, A) { ಪ್ರತ್ಯಾಗಮ HRow<Op, A>::run(acc, x, n); }
    ಸ್ಥಿರ ಶೂನ್ಯ vrun(A* acc, ಧ್ರುವ A* x, int64 n, ಧ್ರುವ A*) { VRow<Op, A>::run(acc, x, n); }
    ಸ್ಥಿರ A finalize(A a, int64 ಎಣಿಕೆ_2, A) { ಪ್ರತ್ಯಾಗಮ Op::finalize(a, ಎಣಿಕೆ_2); }
};

template<typename A> ಸಂರಚನಾ ShiftedExp
{
    ಪ್ರಕಾರನಿರ್ದೇಶ OpSumExpShifted<A> Op;
    ಸ್ಥಿರ A run(A acc, ಧ್ರುವ A* x, int64 n, A m)
    {
        ಪ್ರತಿ (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        ಪ್ರತ್ಯಾಗಮ acc;
    }
    ಸ್ಥಿರ ಶೂನ್ಯ vrun(A* acc, ಧ್ರುವ A* x, int64 n, ಧ್ರುವ A* m)
    {
        ಪ್ರತಿ (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    ಸ್ಥಿರ A finalize(A a, int64, A m) { ಪ್ರತ್ಯಾಗಮ Op::finalize(a, m); }
};

template<typename A, ವರ್ಗ Op, ವರ್ಗ OpM> ಸ್ಥಿರ ಶೂನ್ಯ runPass(ಧ್ರುವ Plan& pl, A* res, ಧ್ರುವ A* mbuf)
{
    ಧ್ರುವ ದ್ವಿಗುಣ work = (ದ್ವಿಗುಣ)pl.nout*(ದ್ವಿಗುಣ)std::max(pl.ಎಣಿಕೆ_2, (int64)1);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nthreads = work < RND_SERIAL_WORK ? 1 : get_ಅಂಕೆ_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 ವಸ್ತುಗಳು;
    ಯದಿ (pl.innerReduced)
        ವಸ್ತುಗಳು = pl.nout;
    ಅನ್ಯಥಾ
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        ವಸ್ತುಗಳು = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ಧ್ರುವ int64 want = 4*(int64)nthreads;
    ಯದಿ (!w.serial && ವಸ್ತುಗಳು < want && pl.ಎಣಿಕೆ_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.ಎಣಿಕೆ_2/4096 : pl.ಎಣಿಕೆ_2/16;
        w.nsplit = std::max((int64)1, std::min((want + ವಸ್ತುಗಳು - 1)/ವಸ್ತುಗಳು, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = ವಸ್ತುಗಳು*w.nsplit;
    w.nstripes = std::min((ದ್ವಿಗುಣ)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    ಯದಿ (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    ಅನ್ಯಥಾ
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    ಯದಿ (w.nsplit > 1)
    {
        ಪ್ರತಿ (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            ಪ್ರತಿ (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.ಎಣಿಕೆ_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> ಸ್ಥಿರ ಶೂನ್ಯ fillResult(A* res, int64 n, A v)
{
    ಪ್ರತಿ (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> ಸ್ಥಿರ ಶೂನ್ಯ runOp(ಧ್ರುವ Plan& pl, ಪೂರ್ಣಾಙ್ಕ op, A* res)
{
    ಯದಿ (pl.ಎಣಿಕೆ_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ವಿಕಲ್ಪ (op)
    {
    ಪಕ್ಷ REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); ಭಙ್ಗ;
    ಪಕ್ಷ REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); ಭಙ್ಗ;
    ಪಕ್ಷ REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); ಭಙ್ಗ;
    ಪಕ್ಷ REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); ಭಙ್ಗ;
    ಪಕ್ಷ REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); ಭಙ್ಗ;
    ಪಕ್ಷ REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); ಭಙ್ಗ;
    ಪಕ್ಷ REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); ಭಙ್ಗ;
    ಪಕ್ಷ REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        ಭಙ್ಗ;
    }
    ಸ್ವಭಾವ:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> ಸ್ಥಿರ ಶೂನ್ಯ runMinMax(ಧ್ರುವ Plan& pl, ಪೂರ್ಣಾಙ್ಕ op, A* res)
{
    ಯದಿ (pl.ಎಣಿಕೆ_2 == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    ಅನ್ಯಥಾ ಯದಿ (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    ಅನ್ಯಥಾ
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

ಸ್ಥಿರ ಶೂನ್ಯ dispatch(ಧ್ರುವ Plan& pl, ಪೂರ್ಣಾಙ್ಕ op, uchar* res)
{
    ಧ್ರುವ ತಾರ್ಕಿಕ minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    ವಿಕಲ್ಪ (pl.adepth)
    {
    ಪಕ್ಷ CV_32F:
        ಯದಿ (minmax) runMinMax<ಭಿನ್ನ>(pl, op, (ಭಿನ್ನ*)res); ಅನ್ಯಥಾ runOp<ಭಿನ್ನ>(pl, op, (ಭಿನ್ನ*)res);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_64F:
        ಯದಿ (minmax) runMinMax<ದ್ವಿಗುಣ>(pl, op, (ದ್ವಿಗುಣ*)res); ಅನ್ಯಥಾ runOp<ದ್ವಿಗುಣ>(pl, op, (ದ್ವಿಗುಣ*)res);
        ಭಙ್ಗ;
    ಪಕ್ಷ CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); ಭಙ್ಗ;
    ಪಕ್ಷ CV_8S:  runMinMax<schar>(pl, op, (schar*)res); ಭಙ್ಗ;
    ಪಕ್ಷ CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); ಭಙ್ಗ;
    ಪಕ್ಷ CV_16S: runMinMax<short>(pl, op, (short*)res); ಭಙ್ಗ;
    ಪಕ್ಷ CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); ಭಙ್ಗ;
    ಪಕ್ಷ CV_32S: runMinMax<ಪೂರ್ಣಾಙ್ಕ>(pl, op, (ಪೂರ್ಣಾಙ್ಕ*)res); ಭಙ್ಗ;
    ಪಕ್ಷ CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); ಭಙ್ಗ;
    ಪಕ್ಷ CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); ಭಙ್ಗ;
    ಸ್ವಭಾವ:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ accumulatorDepth(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth, ಪೂರ್ಣಾಙ್ಕ op)
{
    ಯದಿ (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        ಯದಿ (sdepth == CV_16F || sdepth == CV_16BF)
            ಪ್ರತ್ಯಾಗಮ CV_32F;
        ಪ್ರತ್ಯಾಗಮ sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ಧ್ರುವ ತಾರ್ಕಿಕ wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    ಪ್ರತ್ಯಾಗಮ wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(ಪೂರ್ಣಾಙ್ಕ _rtype, ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& _axes, ತಾರ್ಕಿಕ _keepdims, ಪೂರ್ಣಾಙ್ಕ _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

ಶೂನ್ಯ reduce(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಧ್ರುವ ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ op = params.rtype, dtype = params.dtype;
    ಧ್ರುವ ತಾರ್ಕಿಕ keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dims = src.dims, cn = src.channels();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ sdepth = src.depth();
    ಪೂರ್ಣಾಙ್ಕ ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    ಯದಿ ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    ತಾರ್ಕಿಕ reduced[CV_MAX_DIM] = {ಅಸತ್ಯ};
    ಯದಿ (params.axes.empty())
        std::fill(reduced, reduced + dims, ಸತ್ಯ);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        ಪೂರ್ಣಾಙ್ಕ ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = ಸತ್ಯ;
    }

    ಪೂರ್ಣಾಙ್ಕ keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        ಯದಿ (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dtype_ = CV_MAKETYPE(ddepth, cn);
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
    ಪೂರ್ಣಾಙ್ಕ n = 0;
    ಪೂರ್ಣಾಙ್ಕ size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    ತಾರ್ಕಿಕ red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.ಎಣಿಕೆ_2 = 1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
    {
        ಯದಿ (reduced[i])
            pl.ಎಣಿಕೆ_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    ಯದಿ (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = ಅಸತ್ಯ;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    ಧ್ರುವ ತಾರ್ಕಿಕ direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    ಯದಿ (!direct)
    {
        resMat.create(1, (ಪೂರ್ಣಾಙ್ಕ)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    ಯದಿ (pl.nout == 0)
        ಪ್ರತ್ಯಾಗಮ;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    ಪೂರ್ಣಾಙ್ಕ m = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
        ಯದಿ (size[i] == 0)
        {
            // empty source along this axis
            ಯದಿ (red[i])
            {
                pl.ಎಣಿಕೆ_2 = 0;
                ಅನುವರ್ತ;
            }
            ಪ್ರತ್ಯಾಗಮ;
        }
        ಯದಿ (size[i] == 1)
            ಅನುವರ್ತ;
        ಯದಿ (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        ಅನ್ಯಥಾ
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    ಯದಿ (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = ಅಸತ್ಯ;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < m; i++)
    {
        ಯದಿ (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        ಅನ್ಯಥಾ
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    ಯದಿ (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    ಯದಿ (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        ಯದಿ (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
