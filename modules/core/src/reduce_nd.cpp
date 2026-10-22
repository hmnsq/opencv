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

ಎಣಿಕೆ
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_ಖಂಡ = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ VecT { ಎಣಿಕೆ { ok = 0 }; ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T vtype; };
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_RND_VEC(T, V) ಮಾದರಿ<> ರಚನೆ VecT<T> { ಎಣಿಕೆ { ok = 1 }; ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(ಗಿಡ್ಡ, v_int16)
CV_RND_VEC(ಚಿಹ್ನೆರಹಿತ, v_uint32)
CV_RND_VEC(ಪೂರ್ಣಾಂಕ, v_int32)
CV_RND_VEC(ದಶಮಾಂಶ, v_float32)
#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(ದ್ವಿಗುಣ, v_float64)
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_RND_VEC
#ಆದರೆ_ಅಂತ್ಯ

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ Op_ಮೊತ್ತ
{
    ಸ್ಥಿರ A init() { ಹಿಂತಿರುಗಿ A(0); }
    ಸ್ಥಿರ A apply(A a, A x) { ಹಿಂತಿರುಗಿ a + x; }
    ಸ್ಥಿರ A combine(A a, A b) { ಹಿಂತಿರುಗಿ a + b; }
    ಸ್ಥಿರ A finalize(A a, int64) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vapply(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_ಕೂಡಿಸು(a, x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vcombine(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_ಕೂಡಿಸು(a, b); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ OpAvg : Op_ಮೊತ್ತ<A>
{
    ಸ್ಥಿರ A finalize(A a, int64 n) { ಹಿಂತಿರುಗಿ n > 0 ? A(a/(ದ್ವಿಗುಣ)n) : A(0); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ Op_Log_ಮೊತ್ತ : Op_ಮೊತ್ತ<A>
{
    ಸ್ಥಿರ A finalize(A a, int64) { ಹಿಂತಿರುಗಿ A(std::ಲಘುಗಣಕ(a)); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ OpSum2 : Op_ಮೊತ್ತ<A>
{
    ಸ್ಥಿರ A apply(A a, A x) { ಹಿಂತಿರುಗಿ a + x*x; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vapply(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_ಕೂಡಿಸು(a, v_mul(x, x)); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ OpL2 : OpSum2<A>
{
    ಸ್ಥಿರ A finalize(A a, int64) { ಹಿಂತಿರುಗಿ A(std::ವರ್ಗಮೂಲ(a)); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ OpL1 : Op_ಮೊತ್ತ<A>
{
    ಸ್ಥಿರ A apply(A a, A x) { ಹಿಂತಿರುಗಿ a + (x < A(0) ? -x : x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vapply(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_ಕೂಡಿಸು(a, v_abs(x)); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ OpProd
{
    ಸ್ಥಿರ A init() { ಹಿಂತಿರುಗಿ A(1); }
    ಸ್ಥಿರ A apply(A a, A x) { ಹಿಂತಿರುಗಿ a*x; }
    ಸ್ಥಿರ A combine(A a, A b) { ಹಿಂತಿರುಗಿ a*b; }
    ಸ್ಥಿರ A finalize(A a, int64) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vapply(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_mul(a, x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vcombine(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A, ತಾರ್ಕಿಕ hasInf = std::numeric_limits<A>::has_infinity> ರಚನೆ Extremes
{
    ಸ್ಥಿರ A ಕೆಳಗಿನ() { ಹಿಂತಿರುಗಿ std::numeric_limits<A>::lowest(); }
    ಸ್ಥಿರ A ಮೇಲಿನ() { ಹಿಂತಿರುಗಿ std::numeric_limits<A>::max(); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ Extremes<A, ನಿಜ>
{
    ಸ್ಥಿರ A ಕೆಳಗಿನ() { ಹಿಂತಿರುಗಿ -std::numeric_limits<A>::infinity(); }
    ಸ್ಥಿರ A ಮೇಲಿನ() { ಹಿಂತಿರುಗಿ std::numeric_limits<A>::infinity(); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ OpMax
{
    ಸ್ಥಿರ A init() { ಹಿಂತಿರುಗಿ Extremes<A>::ಕೆಳಗಿನ(); }
    ಸ್ಥಿರ A apply(A a, A x) { ಹಿಂತಿರುಗಿ a < x ? x : a; }
    ಸ್ಥಿರ A combine(A a, A b) { ಹಿಂತಿರುಗಿ a < b ? b : a; }
    ಸ್ಥಿರ A finalize(A a, int64) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vapply(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_max(a, x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vcombine(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_max(a, b); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ OpMin
{
    ಸ್ಥಿರ A init() { ಹಿಂತಿರುಗಿ Extremes<A>::ಮೇಲಿನ(); }
    ಸ್ಥಿರ A apply(A a, A x) { ಹಿಂತಿರುಗಿ x < a ? x : a; }
    ಸ್ಥಿರ A combine(A a, A b) { ಹಿಂತಿರುಗಿ b < a ? b : a; }
    ಸ್ಥಿರ A finalize(A a, int64) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vapply(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_min(a, x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vcombine(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ Op_ಮೊತ್ತ_Exp_Shifted
{
    ಸ್ಥಿರ A init() { ಹಿಂತಿರುಗಿ A(0); }
    ಸ್ಥಿರ A apply(A a, A x, A m) { ಹಿಂತಿರುಗಿ a + A(std::ಘಾತಾಂಕ(x - m)); }
    ಸ್ಥಿರ A combine(A a, A b) { ಹಿಂತಿರುಗಿ a + b; }
    ಸ್ಥಿರ A finalize(A a, A m) { ಹಿಂತಿರುಗಿ std::isinf(m) ? m : m + A(std::ಲಘುಗಣಕ(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
ಮಾದರಿ<ವರ್ಗ Op, ಬಗೆ_ಹೆಸರು A, ತಾರ್ಕಿಕ vec = (VecT<A>::ok != 0)> ರಚನೆ HRow
{
    ಸ್ಥಿರ A ಸತತ(A acc, ಸ್ಥಿರಾಂಕ A* x, int64 n)
    {
        ಗಾಗಿ (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ಹಿಂತಿರುಗಿ acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
ಮಾದರಿ<ವರ್ಗ Op, ಬಗೆ_ಹೆಸರು A, ತಾರ್ಕಿಕ vec = (VecT<A>::ok != 0)> ರಚನೆ VRow
{
    ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ(A* acc, ಸ್ಥಿರಾಂಕ A* x, int64 n)
    {
        ಗಾಗಿ (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಮಾದರಿ<ವರ್ಗ Op, ಬಗೆ_ಹೆಸರು A> ರಚನೆ HRow<Op, A, ನಿಜ>
{
    ಸ್ಥಿರ A ಸತತ(A acc, ಸ್ಥಿರಾಂಕ A* x, int64 n)
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು VecT<A>::vtype V;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ L = VTraits<V>::vlanes();
        int64 i = 0;
        ಆದರೆ (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_ತುಂಬು(buf), a1 = a0;
            ಗಾಗಿ (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_ತುಂಬು(x + i));
                a1 = Op::vapply(a1, vx_ತುಂಬು(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        ಗಾಗಿ (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ಹಿಂತಿರುಗಿ acc;
    }
};

ಮಾದರಿ<ವರ್ಗ Op, ಬಗೆ_ಹೆಸರು A> ರಚನೆ VRow<Op, A, ನಿಜ>
{
    ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ(A* acc, ಸ್ಥಿರಾಂಕ A* x, int64 n)
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಬಗೆ_ಹೆಸರು VecT<A>::vtype V;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ L = VTraits<V>::vlanes();
        int64 i = 0;
        ಗಾಗಿ (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_ತುಂಬು(acc + i), vx_ತುಂಬು(x + i)));
        ಗಾಗಿ (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#ಆದರೆ_ಅಂತ್ಯ

//////////////////////////////// plan ////////////////////////////////

ರಚನೆ Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    ಪೂರ್ಣಾಂಕ nk;
    ಪೂರ್ಣಾಂಕ ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    ಪೂರ್ಣಾಂಕ nr;
    ಪೂರ್ಣಾಂಕ rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    ತಾರ್ಕಿಕ innerReduced;      // the innermost axis is reduced (horizontal mode)

    ಸ್ಥಿರಾಂಕ uchar* src;
    ಪೂರ್ಣಾಂಕ sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 ಎಣಿಕೆ_3;            // reduced elements per output
    int64 nout;             // number of outputs
    ದ್ವಿಮಾನ_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
ರಚನೆ Iter
{
    ಪೂರ್ಣಾಂಕ n = 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* size = ಶೂನ್ಯಸೂಚಕ;
    ಸ್ಥಿರಾಂಕ ptrdiff_t* s = ಶೂನ್ಯಸೂಚಕ;
    ಸ್ಥಿರಾಂಕ ptrdiff_t* d = ಶೂನ್ಯಸೂಚಕ;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    ಶೂನ್ಯ init(ಪೂರ್ಣಾಂಕ n_, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ಗಾತ್ರ_4, ಸ್ಥಿರಾಂಕ ptrdiff_t* s_, ಸ್ಥಿರಾಂಕ ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = ಗಾತ್ರ_4; s = s_; d = d_;
        soff = doff = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            ಆದರೆ (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    ಶೂನ್ಯ next()
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            ಆದರೆ (d)
                doff += d[k];
            ಆದರೆ (++idx[k] < size[k])
                ಹಿಂತಿರುಗಿ;
            soff -= (ptrdiff_t)size[k]*s[k];
            ಆದರೆ (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ A* ತುಂಬು_ಅಡ್ಡಸಾಲು(ಸ್ಥಿರಾಂಕ Plan& pl, ಸ್ಥಿರಾಂಕ uchar* p, ptrdiff_t ಹೆಜ್ಜೆ, int64 len,
                                             A* buf, uchar* tmp)
{
    ಆದರೆ (ಹೆಜ್ಜೆ != (ptrdiff_t)pl.sesz1)
    {
        ಗಾಗಿ (int64 i = 0; i < len; i++, p += ಹೆಜ್ಜೆ)
            ಸ್ಮರಣೆ_ನಕಲು(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    ಆದರೆ (!pl.cvt)
        ಹಿಂತಿರುಗಿ (ಸ್ಥಿರಾಂಕ A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, ಗಾತ್ರ_2((ಪೂರ್ಣಾಂಕ)len, 1), 0);
    ಹಿಂತಿರುಗಿ buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A, ಬಗೆ_ಹೆಸರು F> ಸ್ಥಿರ ಶೂನ್ಯ for_ಅಡ್ಡಸಾಲು(ಸ್ಥಿರಾಂಕ Plan& pl, ಸ್ಥಿರಾಂಕ uchar* p, ptrdiff_t ಹೆಜ್ಜೆ,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ನೇರ = !pl.cvt && ಹೆಜ್ಜೆ == (ptrdiff_t)pl.sesz1;
    ಸ್ಥಿರಾಂಕ int64 blk = ನೇರ ? n : (int64)RND_ಖಂಡ;
    ಗಾಗಿ (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(ತುಂಬು_ಅಡ್ಡಸಾಲು<A>(pl, p + e*ಹೆಜ್ಜೆ, ಹೆಜ್ಜೆ, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
ರಚನೆ Work
{
    int64 nitems, nsplit;
    ದ್ವಿಗುಣ nstripes;
    ತಾರ್ಕಿಕ serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A, ವರ್ಗ Op, ವರ್ಗ OpM> ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Horizontal(ಸ್ಥಿರಾಂಕ Plan& pl, ಸ್ಥಿರಾಂಕ Work& w, A* res,
                                                                   ಸ್ಥಿರಾಂಕ A* mbuf, A* partial)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nr = pl.nr;
    ಸ್ಥಿರಾಂಕ int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ಸ್ಥಿರಾಂಕ ptrdiff_t rstep = pl.rsstep[nr - 1];
    ಸ್ಥಿರಾಂಕ int64 nsplit = w.nsplit;

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range)
    {
        Auto_ಬಫರ್<A> bufA(RND_ಖಂಡ);
        Auto_ಬಫರ್<uchar> bufT(RND_ಖಂಡ*pl.sesz1);
        Iter ko, ro;
        int64 it = range.ಆರಂಭ;
        ಆದರೆ (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        ಆಗ (it < range.end)
        {
            ಸ್ಥಿರಾಂಕ int64 o = it / nsplit, s = it - o*nsplit;
            ಆದರೆ (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ಸ್ಥಿರಾಂಕ uchar* ತಳ = pl.src + ko.soff;
            ಸ್ಥಿರಾಂಕ A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.ಎಣಿಕೆ_3/nsplit, b = (s + 1)*pl.ಎಣಿಕೆ_3/nsplit;
            A acc = Op::init();
            ಆದರೆ (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, ಶೂನ್ಯಸೂಚಕ, a / n);
                int64 e = a % n;
                ಆಗ (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    for_ಅಡ್ಡಸಾಲು<A>(pl, ತಳ + ro.soff + e*rstep, rstep, len, bufA.ದತ್ತಾಂಶ(), bufT.ದತ್ತಾಂಶ(),
                              [&](ಸ್ಥಿರಾಂಕ A* x, int64 l) { acc = OpM::ಸತತ(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            ಆದರೆ (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.ಎಣಿಕೆ_3, m);
                ko.next();
            }
            ಇಲ್ಲದಿದ್ದರೆ
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    ಆದರೆ (w.serial)
        body(Range(0, (ಪೂರ್ಣಾಂಕ)w.nitems));
    ಇಲ್ಲದಿದ್ದರೆ
        parallel_for_(Range(0, (ಪೂರ್ಣಾಂಕ)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A, ವರ್ಗ Op, ವರ್ಗ OpM> ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Vertical(ಸ್ಥಿರಾಂಕ Plan& pl, ಸ್ಥಿರಾಂಕ Work& w, A* res,
                                                                 ಸ್ಥಿರಾಂಕ A* mbuf, A* partial)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nk = pl.nk;
    ಸ್ಥಿರಾಂಕ int64 m = pl.ksize[nk - 1];
    ಸ್ಥಿರಾಂಕ ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ಸ್ಥಿರಾಂಕ int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ಸ್ಥಿರಾಂಕ int64 nsplit = w.nsplit;
    int64 rcount = 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    ಸ್ವಯಂಚಾಲಿತ body = [&](ಸ್ಥಿರಾಂಕ Range& range)
    {
        Auto_ಬಫರ್<A> bufA(RND_ಖಂಡ), accBuf(RND_VCHUNK);
        Auto_ಬಫರ್<uchar> bufT(RND_ಖಂಡ*pl.sesz1);
        A* acc = accBuf.ದತ್ತಾಂಶ();
        Iter ko, ro;
        ಗಾಗಿ (int64 it = range.ಆರಂಭ; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ಸ್ಥಿರಾಂಕ int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ಸ್ಥಿರಾಂಕ ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ಸ್ಥಿರಾಂಕ A* mrow = mbuf ? mbuf + doff : ಶೂನ್ಯಸೂಚಕ;   // kdstep == 1 for the innermost kept axis
            ಗಾಗಿ (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            ಆದರೆ (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, ಶೂನ್ಯಸೂಚಕ, a);
                ಗಾಗಿ (; a < b; a++, ro.next())
                {
                    ಸ್ಥಿರಾಂಕ uchar* p = pl.src + soff + ro.soff;
                    ಸ್ಥಿರಾಂಕ A* x = ತುಂಬು_ಅಡ್ಡಸಾಲು<A>(pl, p, kstep, len, bufA.ದತ್ತಾಂಶ(), bufT.ದತ್ತಾಂಶ());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            ಆದರೆ (nsplit == 1)
                ಗಾಗಿ (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.ಎಣಿಕೆ_3, mrow ? mrow[i] : A(0));
            ಇಲ್ಲದಿದ್ದರೆ
                ಗಾಗಿ (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    ಆದರೆ (w.serial)
        body(Range(0, (ಪೂರ್ಣಾಂಕ)w.nitems));
    ಇಲ್ಲದಿದ್ದರೆ
        parallel_for_(Range(0, (ಪೂರ್ಣಾಂಕ)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A, ವರ್ಗ Op> ರಚನೆ Plain
{
    ಸ್ಥಿರ A ಸತತ(A acc, ಸ್ಥಿರಾಂಕ A* x, int64 n, A) { ಹಿಂತಿರುಗಿ HRow<Op, A>::ಸತತ(acc, x, n); }
    ಸ್ಥಿರ ಶೂನ್ಯ vrun(A* acc, ಸ್ಥಿರಾಂಕ A* x, int64 n, ಸ್ಥಿರಾಂಕ A*) { VRow<Op, A>::ಸತತ(acc, x, n); }
    ಸ್ಥಿರ A finalize(A a, int64 ಎಣಿಕೆ_3, A) { ಹಿಂತಿರುಗಿ Op::finalize(a, ಎಣಿಕೆ_3); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ರಚನೆ ShiftedExp
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Op_ಮೊತ್ತ_Exp_Shifted<A> Op;
    ಸ್ಥಿರ A ಸತತ(A acc, ಸ್ಥಿರಾಂಕ A* x, int64 n, A m)
    {
        ಗಾಗಿ (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        ಹಿಂತಿರುಗಿ acc;
    }
    ಸ್ಥಿರ ಶೂನ್ಯ vrun(A* acc, ಸ್ಥಿರಾಂಕ A* x, int64 n, ಸ್ಥಿರಾಂಕ A* m)
    {
        ಗಾಗಿ (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    ಸ್ಥಿರ A finalize(A a, int64, A m) { ಹಿಂತಿರುಗಿ Op::finalize(a, m); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A, ವರ್ಗ Op, ವರ್ಗ OpM> ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Pass(ಸ್ಥಿರಾಂಕ Plan& pl, A* res, ಸ್ಥಿರಾಂಕ A* mbuf)
{
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ work = (ದ್ವಿಗುಣ)pl.nout*(ದ್ವಿಗುಣ)std::max(pl.ಎಣಿಕೆ_3, (int64)1);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nthreads = work < RND_SERIAL_WORK ? 1 : get_ಅಂಕೆ_ಎಳೆಗಳು();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 ವಸ್ತುಗಳು;
    ಆದರೆ (pl.innerReduced)
        ವಸ್ತುಗಳು = pl.nout;
    ಇಲ್ಲದಿದ್ದರೆ
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        ವಸ್ತುಗಳು = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ಸ್ಥಿರಾಂಕ int64 want = 4*(int64)nthreads;
    ಆದರೆ (!w.serial && ವಸ್ತುಗಳು < want && pl.ಎಣಿಕೆ_3 > 1)
    {
        int64 ಅಡ್ಡಸಾಲುಗಳು = pl.innerReduced ? pl.ಎಣಿಕೆ_3/4096 : pl.ಎಣಿಕೆ_3/16;
        w.nsplit = std::max((int64)1, std::min((want + ವಸ್ತುಗಳು - 1)/ವಸ್ತುಗಳು, std::min(ಅಡ್ಡಸಾಲುಗಳು, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = ವಸ್ತುಗಳು*w.nsplit;
    w.nstripes = std::min((ದ್ವಿಗುಣ)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);

    Auto_ಬಫರ್<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    ಆದರೆ (pl.innerReduced)
        ಸತತ_Horizontal<A, Op, OpM>(pl, w, res, mbuf, partial.ದತ್ತಾಂಶ());
    ಇಲ್ಲದಿದ್ದರೆ
        ಸತತ_Vertical<A, Op, OpM>(pl, w, res, mbuf, partial.ದತ್ತಾಂಶ());

    ಆದರೆ (w.nsplit > 1)
    {
        ಗಾಗಿ (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            ಗಾಗಿ (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.ಎಣಿಕೆ_3, mbuf ? mbuf[o] : A(0));
        }
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ಸ್ಥಿರ ಶೂನ್ಯ fill_ಫಲಿತಾಂಶ(A* res, int64 n, A v)
{
    ಗಾಗಿ (int64 i = 0; i < n; i++)
        res[i] = v;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Op(ಸ್ಥಿರಾಂಕ Plan& pl, ಪೂರ್ಣಾಂಕ op, A* res)
{
    ಆದರೆ (pl.ಎಣಿಕೆ_3 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_ಮೊತ್ತ || op == REDUCE_LOG_ಮೊತ್ತ_EXP ? OpMax<A>::init() : A(0);
        fill_ಫಲಿತಾಂಶ(res, pl.nout, v);
        ಹಿಂತಿರುಗಿ;
    }
    ಆಯ್ಕೆ (op)
    {
    ಸ್ಥಿತಿ REDUCE_ಮೊತ್ತ:   ಸತತ_Pass<A, Op_ಮೊತ್ತ<A>, Plain<A, Op_ಮೊತ್ತ<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ REDUCE_AVG:   ಸತತ_Pass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ REDUCE_SUM2:  ಸತತ_Pass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ REDUCE_L1:    ಸತತ_Pass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ REDUCE_L2:    ಸತತ_Pass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ REDUCE_PROD:  ಸತತ_Pass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ REDUCE_LOG_ಮೊತ್ತ: ಸತತ_Pass<A, Op_Log_ಮೊತ್ತ<A>, Plain<A, Op_Log_ಮೊತ್ತ<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ REDUCE_LOG_ಮೊತ್ತ_EXP:
    {
        Auto_ಬಫರ್<A> mbuf((size_t)pl.nout);
        ಸತತ_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.ದತ್ತಾಂಶ(), ಶೂನ್ಯಸೂಚಕ);
        ಸತತ_Pass<A, Op_ಮೊತ್ತ_Exp_Shifted<A>, ShiftedExp<A> >(pl, res, mbuf.ದತ್ತಾಂಶ());
        ನಿಲ್ಲಿಸು;
    }
    ಪೂರ್ವನಿಯೋಜಿತ:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು A> ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Min_Max(ಸ್ಥಿರಾಂಕ Plan& pl, ಪೂರ್ಣಾಂಕ op, A* res)
{
    ಆದರೆ (pl.ಎಣಿಕೆ_3 == 0)
        fill_ಫಲಿತಾಂಶ(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (op == REDUCE_MAX)
        ಸತತ_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ);
    ಇಲ್ಲದಿದ್ದರೆ
        ಸತತ_Pass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, ಶೂನ್ಯಸೂಚಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ dispatch(ಸ್ಥಿರಾಂಕ Plan& pl, ಪೂರ್ಣಾಂಕ op, uchar* res)
{
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    ಆಯ್ಕೆ (pl.adepth)
    {
    ಸ್ಥಿತಿ CV_32F:
        ಆದರೆ (minmax) ಸತತ_Min_Max<ದಶಮಾಂಶ>(pl, op, (ದಶಮಾಂಶ*)res); ಇಲ್ಲದಿದ್ದರೆ ಸತತ_Op<ದಶಮಾಂಶ>(pl, op, (ದಶಮಾಂಶ*)res);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64F:
        ಆದರೆ (minmax) ಸತತ_Min_Max<ದ್ವಿಗುಣ>(pl, op, (ದ್ವಿಗುಣ*)res); ಇಲ್ಲದಿದ್ದರೆ ಸತತ_Op<ದ್ವಿಗುಣ>(pl, op, (ದ್ವಿಗುಣ*)res);
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8U:  ಸತತ_Min_Max<uchar>(pl, op, (uchar*)res); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:  ಸತತ_Min_Max<schar>(pl, op, (schar*)res); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U: ಸತತ_Min_Max<ushort>(pl, op, (ushort*)res); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S: ಸತತ_Min_Max<ಗಿಡ್ಡ>(pl, op, (ಗಿಡ್ಡ*)res); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U: ಸತತ_Min_Max<ಚಿಹ್ನೆರಹಿತ>(pl, op, (ಚಿಹ್ನೆರಹಿತ*)res); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S: ಸತತ_Min_Max<ಪೂರ್ಣಾಂಕ>(pl, op, (ಪೂರ್ಣಾಂಕ*)res); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U: ಸತತ_Min_Max<uint64_t>(pl, op, (uint64_t*)res); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S: ಸತತ_Min_Max<int64_t>(pl, op, (int64_t*)res); ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        CV_Error(Error::Sts_ಆಂತರಿಕ, "reduce: unexpected accumulator depth");
    }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಂಚಯಕ_ಆಳ(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth, ಪೂರ್ಣಾಂಕ op)
{
    ಆದರೆ (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        ಆದರೆ (sdepth == CV_16F || sdepth == CV_16BF)
            ಹಿಂತಿರುಗಿ CV_32F;
        ಹಿಂತಿರುಗಿ sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    ಹಿಂತಿರುಗಿ wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(ಪೂರ್ಣಾಂಕ _rtype, ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& _axes, ತಾರ್ಕಿಕ _keepdims, ಪೂರ್ಣಾಂಕ _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

ಶೂನ್ಯ reduce(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಸ್ಥಿರಾಂಕ ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ op = params.rtype, dtype = params.dtype;
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_ಮೊತ್ತ && op <= REDUCE_LOG_ಮೊತ್ತ_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dims = src.dims, cn = src.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ sdepth = src.ಆಳ();
    ಪೂರ್ಣಾಂಕ ddepth = dtype < 0 ? sdepth : CV_MAT_ಆಳ(dtype);
    ಆದರೆ ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    ತಾರ್ಕಿಕ reduced[CV_MAX_DIM] = {ಸುಳ್ಳು};
    ಆದರೆ (params.axes.empty())
        std::fill(reduced, reduced + dims, ನಿಜ);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        ಪೂರ್ಣಾಂಕ ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = ನಿಜ;
    }

    ಪೂರ್ಣಾಂಕ keepShape[CV_MAX_DIM], ಹೊರಹೋಗು_Shape[CV_MAX_DIM], nout = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        ಆದರೆ (!reduced[i] || keepdims)
            ಹೊರಹೋಗು_Shape[nout++] = keepShape[i];
    }
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dtype_ = CV_MAKETYPE(ddepth, cn);
    _dst.create(nout, ಹೊರಹೋಗು_Shape, dtype_);
    Mat dst = _dst.getMat();

    // The results are computed into a contiguous accumulator-typed buffer laid out like the
    // keepdims output (channels innermost), then converted into dst if needed.
    Plan pl;
    pl.src = src.ದತ್ತಾಂಶ;
    pl.sdepth = sdepth == CV_Bool ? CV_8U : sdepth;
    pl.adepth = ಸಂಚಯಕ_ಆಳ(sdepth, ddepth, op);
    pl.sesz1 = CV_ELEM_SIZE1(sdepth);
    pl.cvt = pl.sdepth == pl.adepth ? ಶೂನ್ಯಸೂಚಕ : getConvertFunc(pl.sdepth, pl.adepth);
    CV_Assert(pl.sdepth == pl.adepth || pl.cvt);

    // gather the axes (+ the channel axis), outer first
    ಪೂರ್ಣಾಂಕ n = 0;
    ಪೂರ್ಣಾಂಕ size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    ತಾರ್ಕಿಕ red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.ಎಣಿಕೆ_3 = 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dims; i++)
    {
        ಆದರೆ (reduced[i])
            pl.ಎಣಿಕೆ_3 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.ಹೆಜ್ಜೆ[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    ಆದರೆ (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = ಸುಳ್ಳು;
        n++;
    }

    pl.nout = (int64)dst.ಒಟ್ಟು()*cn;
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ನೇರ = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.ದತ್ತಾಂಶ;
    ಆದರೆ (!ನೇರ)
    {
        resMat.create(1, (ಪೂರ್ಣಾಂಕ)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.ದತ್ತಾಂಶ;
    }
    ಆದರೆ (pl.nout == 0)
        ಹಿಂತಿರುಗಿ;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    ಪೂರ್ಣಾಂಕ m = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
    {
        ಆದರೆ (size[i] == 0)
        {
            // empty source along this axis
            ಆದರೆ (red[i])
            {
                pl.ಎಣಿಕೆ_3 = 0;
                ಮುಂದುವರಿ;
            }
            ಹಿಂತಿರುಗಿ;
        }
        ಆದರೆ (size[i] == 1)
            ಮುಂದುವರಿ;
        ಆದರೆ (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    ಆದರೆ (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = ಸುಳ್ಳು;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < m; i++)
    {
        ಆದರೆ (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    ಆದರೆ (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    ಆದರೆ (!ನೇರ)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.ದತ್ತಾಂಶ);
        ಆದರೆ (dst.dims == 0)
            r = Mat(0, ಶೂನ್ಯಸೂಚಕ, CV_MAKETYPE(pl.adepth, cn), resMat.ದತ್ತಾಂಶ);
        r.convert_ಗಮ್ಯ(dst, ddepth);
    }
}

} // namespace cv
