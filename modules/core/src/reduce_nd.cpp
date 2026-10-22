// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/hal/intrin.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits>

ਨਾਮਾਕਾਸ਼ cv {

ਨਾਮਾਕਾਸ਼ {

ਗਿਣਤੀ
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_ਖੰਡ = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ VecT { ਗਿਣਤੀ { ok = 0 }; ਕਿਸਮਪਰਿਭਾਸ਼ਾ T vtype; };
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_RND_VEC(T, V) ਸਾਂਚਾ<> ਬਣਤਰ VecT<T> { ਗਿਣਤੀ { ok = 1 }; ਕਿਸਮਪਰਿਭਾਸ਼ਾ V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(ਛੋਟਾ, v_int16)
CV_RND_VEC(ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32)
CV_RND_VEC(ਪੂਰਨ, v_int32)
CV_RND_VEC(ਦਸ਼ਮਲਵ, v_float32)
#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(ਦੁੱਗਣਾ, v_float64)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_RND_VEC
#ਜੇ_ਅੰਤ

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ Op_ਜੋੜ
{
    ਸਥਿਰ A init() { ਵਾਪਸ A(0); }
    ਸਥਿਰ A apply(A a, A x) { ਵਾਪਸ a + x; }
    ਸਥਿਰ A combine(A a, A b) { ਵਾਪਸ a + b; }
    ਸਥਿਰ A finalize(A a, int64) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vapply(ਅਚਲ V& a, ਅਚਲ V& x) { ਵਾਪਸ v_ਜੋੜੋ(a, x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vcombine(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_ਜੋੜੋ(a, b); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ OpAvg : Op_ਜੋੜ<A>
{
    ਸਥਿਰ A finalize(A a, int64 n) { ਵਾਪਸ n > 0 ? A(a/(ਦੁੱਗਣਾ)n) : A(0); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ Op_Log_ਜੋੜ : Op_ਜੋੜ<A>
{
    ਸਥਿਰ A finalize(A a, int64) { ਵਾਪਸ A(std::ਲਘੂਗਣਕ(a)); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ OpSum2 : Op_ਜੋੜ<A>
{
    ਸਥਿਰ A apply(A a, A x) { ਵਾਪਸ a + x*x; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vapply(ਅਚਲ V& a, ਅਚਲ V& x) { ਵਾਪਸ v_ਜੋੜੋ(a, v_mul(x, x)); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ OpL2 : OpSum2<A>
{
    ਸਥਿਰ A finalize(A a, int64) { ਵਾਪਸ A(std::ਵਰਗਮੂਲ(a)); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ OpL1 : Op_ਜੋੜ<A>
{
    ਸਥਿਰ A apply(A a, A x) { ਵਾਪਸ a + (x < A(0) ? -x : x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vapply(ਅਚਲ V& a, ਅਚਲ V& x) { ਵਾਪਸ v_ਜੋੜੋ(a, v_abs(x)); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ OpProd
{
    ਸਥਿਰ A init() { ਵਾਪਸ A(1); }
    ਸਥਿਰ A apply(A a, A x) { ਵਾਪਸ a*x; }
    ਸਥਿਰ A combine(A a, A b) { ਵਾਪਸ a*b; }
    ਸਥਿਰ A finalize(A a, int64) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vapply(ਅਚਲ V& a, ਅਚਲ V& x) { ਵਾਪਸ v_mul(a, x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vcombine(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A, ਤਾਰਕਿਕ hasInf = std::numeric_limits<A>::has_infinity> ਬਣਤਰ Extremes
{
    ਸਥਿਰ A ਨੀਵਾਂ() { ਵਾਪਸ std::numeric_limits<A>::lowest(); }
    ਸਥਿਰ A ਉੱਚਾ() { ਵਾਪਸ std::numeric_limits<A>::max(); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ Extremes<A, ਸੱਚ>
{
    ਸਥਿਰ A ਨੀਵਾਂ() { ਵਾਪਸ -std::numeric_limits<A>::infinity(); }
    ਸਥਿਰ A ਉੱਚਾ() { ਵਾਪਸ std::numeric_limits<A>::infinity(); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ OpMax
{
    ਸਥਿਰ A init() { ਵਾਪਸ Extremes<A>::ਨੀਵਾਂ(); }
    ਸਥਿਰ A apply(A a, A x) { ਵਾਪਸ a < x ? x : a; }
    ਸਥਿਰ A combine(A a, A b) { ਵਾਪਸ a < b ? b : a; }
    ਸਥਿਰ A finalize(A a, int64) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vapply(ਅਚਲ V& a, ਅਚਲ V& x) { ਵਾਪਸ v_max(a, x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vcombine(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_max(a, b); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ OpMin
{
    ਸਥਿਰ A init() { ਵਾਪਸ Extremes<A>::ਉੱਚਾ(); }
    ਸਥਿਰ A apply(A a, A x) { ਵਾਪਸ x < a ? x : a; }
    ਸਥਿਰ A combine(A a, A b) { ਵਾਪਸ b < a ? b : a; }
    ਸਥਿਰ A finalize(A a, int64) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vapply(ਅਚਲ V& a, ਅਚਲ V& x) { ਵਾਪਸ v_min(a, x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vcombine(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ Op_ਜੋੜ_Exp_Shifted
{
    ਸਥਿਰ A init() { ਵਾਪਸ A(0); }
    ਸਥਿਰ A apply(A a, A x, A m) { ਵਾਪਸ a + A(std::ਘਾਤਾਂਕ(x - m)); }
    ਸਥਿਰ A combine(A a, A b) { ਵਾਪਸ a + b; }
    ਸਥਿਰ A finalize(A a, A m) { ਵਾਪਸ std::isinf(m) ? m : m + A(std::ਲਘੂਗਣਕ(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
ਸਾਂਚਾ<ਵਰਗ Op, ਕਿਸਮ_ਨਾਂ A, ਤਾਰਕਿਕ vec = (VecT<A>::ok != 0)> ਬਣਤਰ HRow
{
    ਸਥਿਰ A ਲੜੀਵਾਰ(A acc, ਅਚਲ A* x, int64 n)
    {
        ਲਈ (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ਵਾਪਸ acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
ਸਾਂਚਾ<ਵਰਗ Op, ਕਿਸਮ_ਨਾਂ A, ਤਾਰਕਿਕ vec = (VecT<A>::ok != 0)> ਬਣਤਰ VRow
{
    ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ(A* acc, ਅਚਲ A* x, int64 n)
    {
        ਲਈ (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਾਂਚਾ<ਵਰਗ Op, ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ HRow<Op, A, ਸੱਚ>
{
    ਸਥਿਰ A ਲੜੀਵਾਰ(A acc, ਅਚਲ A* x, int64 n)
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ VecT<A>::vtype V;
        ਅਚਲ ਪੂਰਨ L = VTraits<V>::vlanes();
        int64 i = 0;
        ਜੇ (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            ਲਈ (ਪੂਰਨ k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_ਲੱਦੋ(buf), a1 = a0;
            ਲਈ (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_ਲੱਦੋ(x + i));
                a1 = Op::vapply(a1, vx_ਲੱਦੋ(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            ਲਈ (ਪੂਰਨ k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        ਲਈ (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ਵਾਪਸ acc;
    }
};

ਸਾਂਚਾ<ਵਰਗ Op, ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ VRow<Op, A, ਸੱਚ>
{
    ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ(A* acc, ਅਚਲ A* x, int64 n)
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ VecT<A>::vtype V;
        ਅਚਲ ਪੂਰਨ L = VTraits<V>::vlanes();
        int64 i = 0;
        ਲਈ (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_ਲੱਦੋ(acc + i), vx_ਲੱਦੋ(x + i)));
        ਲਈ (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#ਜੇ_ਅੰਤ

//////////////////////////////// plan ////////////////////////////////

ਬਣਤਰ Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    ਪੂਰਨ nk;
    ਪੂਰਨ ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    ਪੂਰਨ nr;
    ਪੂਰਨ rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    ਤਾਰਕਿਕ innerReduced;      // the innermost axis is reduced (horizontal mode)

    ਅਚਲ uchar* src;
    ਪੂਰਨ sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 ਗਿਣਤੀ_3;            // reduced elements per output
    int64 nout;             // number of outputs
    ਦੁਆਧਾਰੀ_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
ਬਣਤਰ Iter
{
    ਪੂਰਨ n = 0;
    ਅਚਲ ਪੂਰਨ* size = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ptrdiff_t* s = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ptrdiff_t* d = ਖਾਲੀ_ਸੂਚਕ;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    ਖਾਲੀ init(ਪੂਰਨ n_, ਅਚਲ ਪੂਰਨ* ਮਾਪ_3, ਅਚਲ ptrdiff_t* s_, ਅਚਲ ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = ਮਾਪ_3; s = s_; d = d_;
        soff = doff = 0;
        ਲਈ (ਪੂਰਨ k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            ਜੇ (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    ਖਾਲੀ next()
    {
        ਲਈ (ਪੂਰਨ k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            ਜੇ (d)
                doff += d[k];
            ਜੇ (++idx[k] < size[k])
                ਵਾਪਸ;
            soff -= (ptrdiff_t)size[k]*s[k];
            ਜੇ (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਸਥਿਰ ਅਚਲ A* ਲੱਦੋ_ਪੰਕਤੀ(ਅਚਲ Plan& pl, ਅਚਲ uchar* p, ptrdiff_t ਕਦਮ, int64 len,
                                             A* buf, uchar* tmp)
{
    ਜੇ (ਕਦਮ != (ptrdiff_t)pl.sesz1)
    {
        ਲਈ (int64 i = 0; i < len; i++, p += ਕਦਮ)
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    ਜੇ (!pl.cvt)
        ਵਾਪਸ (ਅਚਲ A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, ਮਾਪ((ਪੂਰਨ)len, 1), 0);
    ਵਾਪਸ buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A, ਕਿਸਮ_ਨਾਂ F> ਸਥਿਰ ਖਾਲੀ for_ਪੰਕਤੀ(ਅਚਲ Plan& pl, ਅਚਲ uchar* p, ptrdiff_t ਕਦਮ,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ਅਚਲ ਤਾਰਕਿਕ ਸਿੱਧਾ = !pl.cvt && ਕਦਮ == (ptrdiff_t)pl.sesz1;
    ਅਚਲ int64 blk = ਸਿੱਧਾ ? n : (int64)RND_ਖੰਡ;
    ਲਈ (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(ਲੱਦੋ_ਪੰਕਤੀ<A>(pl, p + e*ਕਦਮ, ਕਦਮ, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
ਬਣਤਰ Work
{
    int64 nitems, nsplit;
    ਦੁੱਗਣਾ nstripes;
    ਤਾਰਕਿਕ serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A, ਵਰਗ Op, ਵਰਗ OpM> ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Horizontal(ਅਚਲ Plan& pl, ਅਚਲ Work& w, A* res,
                                                                   ਅਚਲ A* mbuf, A* partial)
{
    ਅਚਲ ਪੂਰਨ nr = pl.nr;
    ਅਚਲ int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ਅਚਲ ptrdiff_t rstep = pl.rsstep[nr - 1];
    ਅਚਲ int64 nsplit = w.nsplit;

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range)
    {
        Auto_ਬਫਰ<A> bufA(RND_ਖੰਡ);
        Auto_ਬਫਰ<uchar> bufT(RND_ਖੰਡ*pl.sesz1);
        Iter ko, ro;
        int64 it = range.ਸ਼ੁਰੂ;
        ਜੇ (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        ਜਦੋਂ (it < range.end)
        {
            ਅਚਲ int64 o = it / nsplit, s = it - o*nsplit;
            ਜੇ (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ਅਚਲ uchar* ਅਧਾਰ = pl.src + ko.soff;
            ਅਚਲ A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.ਗਿਣਤੀ_3/nsplit, b = (s + 1)*pl.ਗਿਣਤੀ_3/nsplit;
            A acc = Op::init();
            ਜੇ (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, ਖਾਲੀ_ਸੂਚਕ, a / n);
                int64 e = a % n;
                ਜਦੋਂ (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    for_ਪੰਕਤੀ<A>(pl, ਅਧਾਰ + ro.soff + e*rstep, rstep, len, bufA.ਅੰਕੜੇ(), bufT.ਅੰਕੜੇ(),
                              [&](ਅਚਲ A* x, int64 l) { acc = OpM::ਲੜੀਵਾਰ(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            ਜੇ (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.ਗਿਣਤੀ_3, m);
                ko.next();
            }
            ਨਹੀਂਤਾਂ
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    ਜੇ (w.serial)
        body(Range(0, (ਪੂਰਨ)w.nitems));
    ਨਹੀਂਤਾਂ
        parallel_for_(Range(0, (ਪੂਰਨ)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A, ਵਰਗ Op, ਵਰਗ OpM> ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Vertical(ਅਚਲ Plan& pl, ਅਚਲ Work& w, A* res,
                                                                 ਅਚਲ A* mbuf, A* partial)
{
    ਅਚਲ ਪੂਰਨ nk = pl.nk;
    ਅਚਲ int64 m = pl.ksize[nk - 1];
    ਅਚਲ ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ਅਚਲ int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ਅਚਲ int64 nsplit = w.nsplit;
    int64 rcount = 1;
    ਲਈ (ਪੂਰਨ i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    ਸਵੈਚਾਲਿਤ body = [&](ਅਚਲ Range& range)
    {
        Auto_ਬਫਰ<A> bufA(RND_ਖੰਡ), accBuf(RND_VCHUNK);
        Auto_ਬਫਰ<uchar> bufT(RND_ਖੰਡ*pl.sesz1);
        A* acc = accBuf.ਅੰਕੜੇ();
        Iter ko, ro;
        ਲਈ (int64 it = range.ਸ਼ੁਰੂ; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ਅਚਲ int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ਅਚਲ ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ਅਚਲ A* mrow = mbuf ? mbuf + doff : ਖਾਲੀ_ਸੂਚਕ;   // kdstep == 1 for the innermost kept axis
            ਲਈ (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            ਜੇ (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, ਖਾਲੀ_ਸੂਚਕ, a);
                ਲਈ (; a < b; a++, ro.next())
                {
                    ਅਚਲ uchar* p = pl.src + soff + ro.soff;
                    ਅਚਲ A* x = ਲੱਦੋ_ਪੰਕਤੀ<A>(pl, p, kstep, len, bufA.ਅੰਕੜੇ(), bufT.ਅੰਕੜੇ());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            ਜੇ (nsplit == 1)
                ਲਈ (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.ਗਿਣਤੀ_3, mrow ? mrow[i] : A(0));
            ਨਹੀਂਤਾਂ
                ਲਈ (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    ਜੇ (w.serial)
        body(Range(0, (ਪੂਰਨ)w.nitems));
    ਨਹੀਂਤਾਂ
        parallel_for_(Range(0, (ਪੂਰਨ)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A, ਵਰਗ Op> ਬਣਤਰ Plain
{
    ਸਥਿਰ A ਲੜੀਵਾਰ(A acc, ਅਚਲ A* x, int64 n, A) { ਵਾਪਸ HRow<Op, A>::ਲੜੀਵਾਰ(acc, x, n); }
    ਸਥਿਰ ਖਾਲੀ vrun(A* acc, ਅਚਲ A* x, int64 n, ਅਚਲ A*) { VRow<Op, A>::ਲੜੀਵਾਰ(acc, x, n); }
    ਸਥਿਰ A finalize(A a, int64 ਗਿਣਤੀ_3, A) { ਵਾਪਸ Op::finalize(a, ਗਿਣਤੀ_3); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਬਣਤਰ ShiftedExp
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ Op_ਜੋੜ_Exp_Shifted<A> Op;
    ਸਥਿਰ A ਲੜੀਵਾਰ(A acc, ਅਚਲ A* x, int64 n, A m)
    {
        ਲਈ (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        ਵਾਪਸ acc;
    }
    ਸਥਿਰ ਖਾਲੀ vrun(A* acc, ਅਚਲ A* x, int64 n, ਅਚਲ A* m)
    {
        ਲਈ (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    ਸਥਿਰ A finalize(A a, int64, A m) { ਵਾਪਸ Op::finalize(a, m); }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A, ਵਰਗ Op, ਵਰਗ OpM> ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Pass(ਅਚਲ Plan& pl, A* res, ਅਚਲ A* mbuf)
{
    ਅਚਲ ਦੁੱਗਣਾ work = (ਦੁੱਗਣਾ)pl.nout*(ਦੁੱਗਣਾ)std::max(pl.ਗਿਣਤੀ_3, (int64)1);
    ਅਚਲ ਪੂਰਨ nthreads = work < RND_SERIAL_WORK ? 1 : get_ਅੰਕ_ਧਾਗੇ();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 ਚੀਜ਼ਾਂ;
    ਜੇ (pl.innerReduced)
        ਚੀਜ਼ਾਂ = pl.nout;
    ਨਹੀਂਤਾਂ
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        ਚੀਜ਼ਾਂ = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ਅਚਲ int64 want = 4*(int64)nthreads;
    ਜੇ (!w.serial && ਚੀਜ਼ਾਂ < want && pl.ਗਿਣਤੀ_3 > 1)
    {
        int64 ਪੰਕਤੀਆਂ = pl.innerReduced ? pl.ਗਿਣਤੀ_3/4096 : pl.ਗਿਣਤੀ_3/16;
        w.nsplit = std::max((int64)1, std::min((want + ਚੀਜ਼ਾਂ - 1)/ਚੀਜ਼ਾਂ, std::min(ਪੰਕਤੀਆਂ, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = ਚੀਜ਼ਾਂ*w.nsplit;
    w.nstripes = std::min((ਦੁੱਗਣਾ)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);

    Auto_ਬਫਰ<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    ਜੇ (pl.innerReduced)
        ਲੜੀਵਾਰ_Horizontal<A, Op, OpM>(pl, w, res, mbuf, partial.ਅੰਕੜੇ());
    ਨਹੀਂਤਾਂ
        ਲੜੀਵਾਰ_Vertical<A, Op, OpM>(pl, w, res, mbuf, partial.ਅੰਕੜੇ());

    ਜੇ (w.nsplit > 1)
    {
        ਲਈ (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            ਲਈ (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.ਗਿਣਤੀ_3, mbuf ? mbuf[o] : A(0));
        }
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਸਥਿਰ ਖਾਲੀ fill_ਨਤੀਜਾ(A* res, int64 n, A v)
{
    ਲਈ (int64 i = 0; i < n; i++)
        res[i] = v;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Op(ਅਚਲ Plan& pl, ਪੂਰਨ op, A* res)
{
    ਜੇ (pl.ਗਿਣਤੀ_3 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_ਜੋੜ || op == REDUCE_LOG_ਜੋੜ_EXP ? OpMax<A>::init() : A(0);
        fill_ਨਤੀਜਾ(res, pl.nout, v);
        ਵਾਪਸ;
    }
    ਚੋਣ (op)
    {
    ਹਾਲਤ REDUCE_ਜੋੜ:   ਲੜੀਵਾਰ_Pass<A, Op_ਜੋੜ<A>, Plain<A, Op_ਜੋੜ<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ); ਰੋਕੋ;
    ਹਾਲਤ REDUCE_AVG:   ਲੜੀਵਾਰ_Pass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ); ਰੋਕੋ;
    ਹਾਲਤ REDUCE_SUM2:  ਲੜੀਵਾਰ_Pass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ); ਰੋਕੋ;
    ਹਾਲਤ REDUCE_L1:    ਲੜੀਵਾਰ_Pass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ); ਰੋਕੋ;
    ਹਾਲਤ REDUCE_L2:    ਲੜੀਵਾਰ_Pass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ); ਰੋਕੋ;
    ਹਾਲਤ REDUCE_PROD:  ਲੜੀਵਾਰ_Pass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ); ਰੋਕੋ;
    ਹਾਲਤ REDUCE_LOG_ਜੋੜ: ਲੜੀਵਾਰ_Pass<A, Op_Log_ਜੋੜ<A>, Plain<A, Op_Log_ਜੋੜ<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ); ਰੋਕੋ;
    ਹਾਲਤ REDUCE_LOG_ਜੋੜ_EXP:
    {
        Auto_ਬਫਰ<A> mbuf((size_t)pl.nout);
        ਲੜੀਵਾਰ_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.ਅੰਕੜੇ(), ਖਾਲੀ_ਸੂਚਕ);
        ਲੜੀਵਾਰ_Pass<A, Op_ਜੋੜ_Exp_Shifted<A>, ShiftedExp<A> >(pl, res, mbuf.ਅੰਕੜੇ());
        ਰੋਕੋ;
    }
    ਮੂਲ:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ A> ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Min_Max(ਅਚਲ Plan& pl, ਪੂਰਨ op, A* res)
{
    ਜੇ (pl.ਗਿਣਤੀ_3 == 0)
        fill_ਨਤੀਜਾ(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    ਨਹੀਂਤਾਂ ਜੇ (op == REDUCE_MAX)
        ਲੜੀਵਾਰ_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ);
    ਨਹੀਂਤਾਂ
        ਲੜੀਵਾਰ_Pass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, ਖਾਲੀ_ਸੂਚਕ);
}

ਸਥਿਰ ਖਾਲੀ dispatch(ਅਚਲ Plan& pl, ਪੂਰਨ op, uchar* res)
{
    ਅਚਲ ਤਾਰਕਿਕ minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    ਚੋਣ (pl.adepth)
    {
    ਹਾਲਤ CV_32F:
        ਜੇ (minmax) ਲੜੀਵਾਰ_Min_Max<ਦਸ਼ਮਲਵ>(pl, op, (ਦਸ਼ਮਲਵ*)res); ਨਹੀਂਤਾਂ ਲੜੀਵਾਰ_Op<ਦਸ਼ਮਲਵ>(pl, op, (ਦਸ਼ਮਲਵ*)res);
        ਰੋਕੋ;
    ਹਾਲਤ CV_64F:
        ਜੇ (minmax) ਲੜੀਵਾਰ_Min_Max<ਦੁੱਗਣਾ>(pl, op, (ਦੁੱਗਣਾ*)res); ਨਹੀਂਤਾਂ ਲੜੀਵਾਰ_Op<ਦੁੱਗਣਾ>(pl, op, (ਦੁੱਗਣਾ*)res);
        ਰੋਕੋ;
    ਹਾਲਤ CV_8U:  ਲੜੀਵਾਰ_Min_Max<uchar>(pl, op, (uchar*)res); ਰੋਕੋ;
    ਹਾਲਤ CV_8S:  ਲੜੀਵਾਰ_Min_Max<schar>(pl, op, (schar*)res); ਰੋਕੋ;
    ਹਾਲਤ CV_16U: ਲੜੀਵਾਰ_Min_Max<ushort>(pl, op, (ushort*)res); ਰੋਕੋ;
    ਹਾਲਤ CV_16S: ਲੜੀਵਾਰ_Min_Max<ਛੋਟਾ>(pl, op, (ਛੋਟਾ*)res); ਰੋਕੋ;
    ਹਾਲਤ CV_32U: ਲੜੀਵਾਰ_Min_Max<ਬਿਨਾਨਿਸ਼ਾਨੀ>(pl, op, (ਬਿਨਾਨਿਸ਼ਾਨੀ*)res); ਰੋਕੋ;
    ਹਾਲਤ CV_32S: ਲੜੀਵਾਰ_Min_Max<ਪੂਰਨ>(pl, op, (ਪੂਰਨ*)res); ਰੋਕੋ;
    ਹਾਲਤ CV_64U: ਲੜੀਵਾਰ_Min_Max<uint64_t>(pl, op, (uint64_t*)res); ਰੋਕੋ;
    ਹਾਲਤ CV_64S: ਲੜੀਵਾਰ_Min_Max<int64_t>(pl, op, (int64_t*)res); ਰੋਕੋ;
    ਮੂਲ:
        CV_Error(Error::Sts_ਅੰਦਰੂਨੀ, "reduce: unexpected accumulator depth");
    }
}

ਸਥਿਰ ਪੂਰਨ ਸੰਚਾਇਕ_ਡੂੰਘਾਈ(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth, ਪੂਰਨ op)
{
    ਜੇ (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        ਜੇ (sdepth == CV_16F || sdepth == CV_16BF)
            ਵਾਪਸ CV_32F;
        ਵਾਪਸ sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ਅਚਲ ਤਾਰਕਿਕ wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    ਵਾਪਸ wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(ਪੂਰਨ _rtype, ਅਚਲ std::vector<ਪੂਰਨ>& _axes, ਤਾਰਕਿਕ _keepdims, ਪੂਰਨ _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

ਖਾਲੀ reduce(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਅਚਲ ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ਅਚਲ ਪੂਰਨ op = params.rtype, dtype = params.dtype;
    ਅਚਲ ਤਾਰਕਿਕ keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_ਜੋੜ && op <= REDUCE_LOG_ਜੋੜ_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ਅਚਲ ਪੂਰਨ dims = src.dims, cn = src.channels();
    ਅਚਲ ਪੂਰਨ sdepth = src.ਡੂੰਘਾਈ();
    ਪੂਰਨ ddepth = dtype < 0 ? sdepth : CV_MAT_ਡੂੰਘਾਈ(dtype);
    ਜੇ ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    ਤਾਰਕਿਕ reduced[CV_MAX_DIM] = {ਝੂਠ};
    ਜੇ (params.axes.empty())
        std::fill(reduced, reduced + dims, ਸੱਚ);
    ਲਈ (ਪੂਰਨ a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        ਪੂਰਨ ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = ਸੱਚ;
    }

    ਪੂਰਨ keepShape[CV_MAX_DIM], ਬਾਹਰਲਾ_Shape[CV_MAX_DIM], nout = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        ਜੇ (!reduced[i] || keepdims)
            ਬਾਹਰਲਾ_Shape[nout++] = keepShape[i];
    }
    ਅਚਲ ਪੂਰਨ dtype_ = CV_MAKETYPE(ddepth, cn);
    _dst.create(nout, ਬਾਹਰਲਾ_Shape, dtype_);
    Mat dst = _dst.getMat();

    // The results are computed into a contiguous accumulator-typed buffer laid out like the
    // keepdims output (channels innermost), then converted into dst if needed.
    Plan pl;
    pl.src = src.ਅੰਕੜੇ;
    pl.sdepth = sdepth == CV_Bool ? CV_8U : sdepth;
    pl.adepth = ਸੰਚਾਇਕ_ਡੂੰਘਾਈ(sdepth, ddepth, op);
    pl.sesz1 = CV_ELEM_SIZE1(sdepth);
    pl.cvt = pl.sdepth == pl.adepth ? ਖਾਲੀ_ਸੂਚਕ : getConvertFunc(pl.sdepth, pl.adepth);
    CV_Assert(pl.sdepth == pl.adepth || pl.cvt);

    // gather the axes (+ the channel axis), outer first
    ਪੂਰਨ n = 0;
    ਪੂਰਨ size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    ਤਾਰਕਿਕ red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    ਲਈ (ਪੂਰਨ i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.ਗਿਣਤੀ_3 = 1;
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
    {
        ਜੇ (reduced[i])
            pl.ਗਿਣਤੀ_3 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.ਕਦਮ[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    ਜੇ (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = ਝੂਠ;
        n++;
    }

    pl.nout = (int64)dst.ਕੁੱਲ()*cn;
    ਅਚਲ ਤਾਰਕਿਕ ਸਿੱਧਾ = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.ਅੰਕੜੇ;
    ਜੇ (!ਸਿੱਧਾ)
    {
        resMat.create(1, (ਪੂਰਨ)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.ਅੰਕੜੇ;
    }
    ਜੇ (pl.nout == 0)
        ਵਾਪਸ;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    ਪੂਰਨ m = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
    {
        ਜੇ (size[i] == 0)
        {
            // empty source along this axis
            ਜੇ (red[i])
            {
                pl.ਗਿਣਤੀ_3 = 0;
                ਜਾਰੀ;
            }
            ਵਾਪਸ;
        }
        ਜੇ (size[i] == 1)
            ਜਾਰੀ;
        ਜੇ (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        ਨਹੀਂਤਾਂ
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    ਜੇ (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = ਝੂਠ;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < m; i++)
    {
        ਜੇ (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        ਨਹੀਂਤਾਂ
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    ਜੇ (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    ਜੇ (!ਸਿੱਧਾ)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.ਅੰਕੜੇ);
        ਜੇ (dst.dims == 0)
            r = Mat(0, ਖਾਲੀ_ਸੂਚਕ, CV_MAKETYPE(pl.adepth, cn), resMat.ਅੰਕੜੇ);
        r.convert_ਮੰਜ਼ਿਲ(dst, ddepth);
    }
}

} // namespace cv
