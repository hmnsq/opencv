// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/hal/intrin.hpp"

#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <cmath>
#ଅନ୍ତର୍ଭାଵ <limits>

ନାମାକାଶ cv {

ନାମାକାଶ {

ଗଣନା
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> ସଂରଚନା VecT { ଗଣନା { ok = 0 }; ପ୍ରକାରବ୍ୟାଖ୍ୟା T vtype; };
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> ସଂରଚନା VecT<T> { ଗଣନା { ok = 1 }; ପ୍ରକାରବ୍ୟାଖ୍ୟା V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, v_int32)
CV_RND_VEC(ଦଶମିକ, v_float32)
#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(ଦ୍ୱିଗୁଣ, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> ସଂରଚନା OpSum
{
    ସ୍ଥିର A init() { ଫେରନ୍ତୁ A(0); }
    ସ୍ଥିର A apply(A a, A x) { ଫେରନ୍ତୁ a + x; }
    ସ୍ଥିର A combine(A a, A b) { ଫେରନ୍ତୁ a + b; }
    ସ୍ଥିର A finalize(A a, int64) { ଫେରନ୍ତୁ a; }
    template<typename V> ସ୍ଥିର V vapply(ଅଚଳ V& a, ଅଚଳ V& x) { ଫେରନ୍ତୁ v_add(a, x); }
    template<typename V> ସ୍ଥିର V vcombine(ଅଚଳ V& a, ଅଚଳ V& b) { ଫେରନ୍ତୁ v_add(a, b); }
};

template<typename A> ସଂରଚନା OpAvg : OpSum<A>
{
    ସ୍ଥିର A finalize(A a, int64 n) { ଫେରନ୍ତୁ n > 0 ? A(a/(ଦ୍ୱିଗୁଣ)n) : A(0); }
};

template<typename A> ସଂରଚନା OpLogSum : OpSum<A>
{
    ସ୍ଥିର A finalize(A a, int64) { ଫେରନ୍ତୁ A(std::log(a)); }
};

template<typename A> ସଂରଚନା OpSum2 : OpSum<A>
{
    ସ୍ଥିର A apply(A a, A x) { ଫେରନ୍ତୁ a + x*x; }
    template<typename V> ସ୍ଥିର V vapply(ଅଚଳ V& a, ଅଚଳ V& x) { ଫେରନ୍ତୁ v_add(a, v_mul(x, x)); }
};

template<typename A> ସଂରଚନା OpL2 : OpSum2<A>
{
    ସ୍ଥିର A finalize(A a, int64) { ଫେରନ୍ତୁ A(std::sqrt(a)); }
};

template<typename A> ସଂରଚନା OpL1 : OpSum<A>
{
    ସ୍ଥିର A apply(A a, A x) { ଫେରନ୍ତୁ a + (x < A(0) ? -x : x); }
    template<typename V> ସ୍ଥିର V vapply(ଅଚଳ V& a, ଅଚଳ V& x) { ଫେରନ୍ତୁ v_add(a, v_abs(x)); }
};

template<typename A> ସଂରଚନା OpProd
{
    ସ୍ଥିର A init() { ଫେରନ୍ତୁ A(1); }
    ସ୍ଥିର A apply(A a, A x) { ଫେରନ୍ତୁ a*x; }
    ସ୍ଥିର A combine(A a, A b) { ଫେରନ୍ତୁ a*b; }
    ସ୍ଥିର A finalize(A a, int64) { ଫେରନ୍ତୁ a; }
    template<typename V> ସ୍ଥିର V vapply(ଅଚଳ V& a, ଅଚଳ V& x) { ଫେରନ୍ତୁ v_mul(a, x); }
    template<typename V> ସ୍ଥିର V vcombine(ଅଚଳ V& a, ଅଚଳ V& b) { ଫେରନ୍ତୁ v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, ତାର୍କିକ hasInf = std::numeric_limits<A>::has_infinity> ସଂରଚନା Extremes
{
    ସ୍ଥିର A ତଳ() { ଫେରନ୍ତୁ std::numeric_limits<A>::lowest(); }
    ସ୍ଥିର A ଉପର() { ଫେରନ୍ତୁ std::numeric_limits<A>::max(); }
};

template<typename A> ସଂରଚନା Extremes<A, ସତ୍ୟ>
{
    ସ୍ଥିର A ତଳ() { ଫେରନ୍ତୁ -std::numeric_limits<A>::infinity(); }
    ସ୍ଥିର A ଉପର() { ଫେରନ୍ତୁ std::numeric_limits<A>::infinity(); }
};

template<typename A> ସଂରଚନା OpMax
{
    ସ୍ଥିର A init() { ଫେରନ୍ତୁ Extremes<A>::ତଳ(); }
    ସ୍ଥିର A apply(A a, A x) { ଫେରନ୍ତୁ a < x ? x : a; }
    ସ୍ଥିର A combine(A a, A b) { ଫେରନ୍ତୁ a < b ? b : a; }
    ସ୍ଥିର A finalize(A a, int64) { ଫେରନ୍ତୁ a; }
    template<typename V> ସ୍ଥିର V vapply(ଅଚଳ V& a, ଅଚଳ V& x) { ଫେରନ୍ତୁ v_max(a, x); }
    template<typename V> ସ୍ଥିର V vcombine(ଅଚଳ V& a, ଅଚଳ V& b) { ଫେରନ୍ତୁ v_max(a, b); }
};

template<typename A> ସଂରଚନା OpMin
{
    ସ୍ଥିର A init() { ଫେରନ୍ତୁ Extremes<A>::ଉପର(); }
    ସ୍ଥିର A apply(A a, A x) { ଫେରନ୍ତୁ x < a ? x : a; }
    ସ୍ଥିର A combine(A a, A b) { ଫେରନ୍ତୁ b < a ? b : a; }
    ସ୍ଥିର A finalize(A a, int64) { ଫେରନ୍ତୁ a; }
    template<typename V> ସ୍ଥିର V vapply(ଅଚଳ V& a, ଅଚଳ V& x) { ଫେରନ୍ତୁ v_min(a, x); }
    template<typename V> ସ୍ଥିର V vcombine(ଅଚଳ V& a, ଅଚଳ V& b) { ଫେରନ୍ତୁ v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> ସଂରଚନା OpSumExpShifted
{
    ସ୍ଥିର A init() { ଫେରନ୍ତୁ A(0); }
    ସ୍ଥିର A apply(A a, A x, A m) { ଫେରନ୍ତୁ a + A(std::exp(x - m)); }
    ସ୍ଥିର A combine(A a, A b) { ଫେରନ୍ତୁ a + b; }
    ସ୍ଥିର A finalize(A a, A m) { ଫେରନ୍ତୁ std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<ଶ୍ରେଣୀ Op, typename A, ତାର୍କିକ vec = (VecT<A>::ok != 0)> ସଂରଚନା HRow
{
    ସ୍ଥିର A run(A acc, ଅଚଳ A* x, int64 n)
    {
        ପାଇଁ (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ଫେରନ୍ତୁ acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<ଶ୍ରେଣୀ Op, typename A, ତାର୍କିକ vec = (VecT<A>::ok != 0)> ସଂରଚନା VRow
{
    ସ୍ଥିର ଶୂନ୍ୟ run(A* acc, ଅଚଳ A* x, int64 n)
    {
        ପାଇଁ (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
template<ଶ୍ରେଣୀ Op, typename A> ସଂରଚନା HRow<Op, A, ସତ୍ୟ>
{
    ସ୍ଥିର A run(A acc, ଅଚଳ A* x, int64 n)
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା typename VecT<A>::vtype V;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା L = VTraits<V>::vlanes();
        int64 i = 0;
        ଯଦି (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            ପାଇଁ (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        ପାଇଁ (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        ଫେରନ୍ତୁ acc;
    }
};

template<ଶ୍ରେଣୀ Op, typename A> ସଂରଚନା VRow<Op, A, ସତ୍ୟ>
{
    ସ୍ଥିର ଶୂନ୍ୟ run(A* acc, ଅଚଳ A* x, int64 n)
    {
        ପ୍ରକାରବ୍ୟାଖ୍ୟା typename VecT<A>::vtype V;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା L = VTraits<V>::vlanes();
        int64 i = 0;
        ପାଇଁ (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        ପାଇଁ (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

ସଂରଚନା Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nk;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    ତାର୍କିକ innerReduced;      // the innermost axis is reduced (horizontal mode)

    ଅଚଳ uchar* src;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 ଗଣନା_3;            // reduced elements per output
    int64 nout;             // number of outputs
    ଦ୍ୱିମିକ_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
ସଂରଚନା Iter
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* size = nullptr;
    ଅଚଳ ptrdiff_t* s = nullptr;
    ଅଚଳ ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    ଶୂନ୍ୟ init(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* size_, ଅଚଳ ptrdiff_t* s_, ଅଚଳ ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            ଯଦି (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    ଶୂନ୍ୟ next()
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            ଯଦି (d)
                doff += d[k];
            ଯଦି (++idx[k] < size[k])
                ଫେରନ୍ତୁ;
            soff -= (ptrdiff_t)size[k]*s[k];
            ଯଦି (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> ସ୍ଥିର ଅଚଳ A* loadRow(ଅଚଳ Plan& pl, ଅଚଳ uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    ଯଦି (step != (ptrdiff_t)pl.sesz1)
    {
        ପାଇଁ (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    ଯଦି (!pl.cvt)
        ଫେରନ୍ତୁ (ଅଚଳ A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, 1), 0);
    ଫେରନ୍ତୁ buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> ସ୍ଥିର ଶୂନ୍ୟ forRow(ଅଚଳ Plan& pl, ଅଚଳ uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ଅଚଳ ତାର୍କିକ direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    ଅଚଳ int64 blk = direct ? n : (int64)RND_BLOCK;
    ପାଇଁ (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
ସଂରଚନା Work
{
    int64 nitems, nsplit;
    ଦ୍ୱିଗୁଣ nstripes;
    ତାର୍କିକ serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, ଶ୍ରେଣୀ Op, ଶ୍ରେଣୀ OpM> ସ୍ଥିର ଶୂନ୍ୟ runHorizontal(ଅଚଳ Plan& pl, ଅଚଳ Work& w, A* res,
                                                                   ଅଚଳ A* mbuf, A* partial)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr = pl.nr;
    ଅଚଳ int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ଅଚଳ ptrdiff_t rstep = pl.rsstep[nr - 1];
    ଅଚଳ int64 nsplit = w.nsplit;

    auto body = [&](ଅଚଳ Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        ଯଦି (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        ଯେତେବେଳେ (it < range.end)
        {
            ଅଚଳ int64 o = it / nsplit, s = it - o*nsplit;
            ଯଦି (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ଅଚଳ uchar* base = pl.src + ko.soff;
            ଅଚଳ A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.ଗଣନା_3/nsplit, b = (s + 1)*pl.ଗଣନା_3/nsplit;
            A acc = Op::init();
            ଯଦି (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                ଯେତେବେଳେ (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](ଅଚଳ A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            ଯଦି (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.ଗଣନା_3, m);
                ko.next();
            }
            ଅନ୍ୟଥା
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    ଯଦି (w.serial)
        body(Range(0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)w.nitems));
    ଅନ୍ୟଥା
        parallel_for_(Range(0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, ଶ୍ରେଣୀ Op, ଶ୍ରେଣୀ OpM> ସ୍ଥିର ଶୂନ୍ୟ runVertical(ଅଚଳ Plan& pl, ଅଚଳ Work& w, A* res,
                                                                 ଅଚଳ A* mbuf, A* partial)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nk = pl.nk;
    ଅଚଳ int64 m = pl.ksize[nk - 1];
    ଅଚଳ ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ଅଚଳ int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ଅଚଳ int64 nsplit = w.nsplit;
    int64 rcount = 1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](ଅଚଳ Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        ପାଇଁ (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ଅଚଳ int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ଅଚଳ ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ଅଚଳ A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            ପାଇଁ (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            ଯଦି (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                ପାଇଁ (; a < b; a++, ro.next())
                {
                    ଅଚଳ uchar* p = pl.src + soff + ro.soff;
                    ଅଚଳ A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            ଯଦି (nsplit == 1)
                ପାଇଁ (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.ଗଣନା_3, mrow ? mrow[i] : A(0));
            ଅନ୍ୟଥା
                ପାଇଁ (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    ଯଦି (w.serial)
        body(Range(0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)w.nitems));
    ଅନ୍ୟଥା
        parallel_for_(Range(0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, ଶ୍ରେଣୀ Op> ସଂରଚନା Plain
{
    ସ୍ଥିର A run(A acc, ଅଚଳ A* x, int64 n, A) { ଫେରନ୍ତୁ HRow<Op, A>::run(acc, x, n); }
    ସ୍ଥିର ଶୂନ୍ୟ vrun(A* acc, ଅଚଳ A* x, int64 n, ଅଚଳ A*) { VRow<Op, A>::run(acc, x, n); }
    ସ୍ଥିର A finalize(A a, int64 ଗଣନା_3, A) { ଫେରନ୍ତୁ Op::finalize(a, ଗଣନା_3); }
};

template<typename A> ସଂରଚନା ShiftedExp
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା OpSumExpShifted<A> Op;
    ସ୍ଥିର A run(A acc, ଅଚଳ A* x, int64 n, A m)
    {
        ପାଇଁ (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        ଫେରନ୍ତୁ acc;
    }
    ସ୍ଥିର ଶୂନ୍ୟ vrun(A* acc, ଅଚଳ A* x, int64 n, ଅଚଳ A* m)
    {
        ପାଇଁ (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    ସ୍ଥିର A finalize(A a, int64, A m) { ଫେରନ୍ତୁ Op::finalize(a, m); }
};

template<typename A, ଶ୍ରେଣୀ Op, ଶ୍ରେଣୀ OpM> ସ୍ଥିର ଶୂନ୍ୟ runPass(ଅଚଳ Plan& pl, A* res, ଅଚଳ A* mbuf)
{
    ଅଚଳ ଦ୍ୱିଗୁଣ work = (ଦ୍ୱିଗୁଣ)pl.nout*(ଦ୍ୱିଗୁଣ)std::max(pl.ଗଣନା_3, (int64)1);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nthreads = work < RND_SERIAL_WORK ? 1 : get_ଅଙ୍କ_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 ବସ୍ତୁଗୁଡ଼ିକ;
    ଯଦି (pl.innerReduced)
        ବସ୍ତୁଗୁଡ଼ିକ = pl.nout;
    ଅନ୍ୟଥା
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        ବସ୍ତୁଗୁଡ଼ିକ = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ଅଚଳ int64 want = 4*(int64)nthreads;
    ଯଦି (!w.serial && ବସ୍ତୁଗୁଡ଼ିକ < want && pl.ଗଣନା_3 > 1)
    {
        int64 rows = pl.innerReduced ? pl.ଗଣନା_3/4096 : pl.ଗଣନା_3/16;
        w.nsplit = std::max((int64)1, std::min((want + ବସ୍ତୁଗୁଡ଼ିକ - 1)/ବସ୍ତୁଗୁଡ଼ିକ, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = ବସ୍ତୁଗୁଡ଼ିକ*w.nsplit;
    w.nstripes = std::min((ଦ୍ୱିଗୁଣ)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    ଯଦି (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    ଅନ୍ୟଥା
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    ଯଦି (w.nsplit > 1)
    {
        ପାଇଁ (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            ପାଇଁ (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.ଗଣନା_3, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> ସ୍ଥିର ଶୂନ୍ୟ fillResult(A* res, int64 n, A v)
{
    ପାଇଁ (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> ସ୍ଥିର ଶୂନ୍ୟ runOp(ଅଚଳ Plan& pl, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op, A* res)
{
    ଯଦି (pl.ଗଣନା_3 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        ଫେରନ୍ତୁ;
    }
    ବାଛ (op)
    {
    ସ୍ଥିତି REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); ଭାଙ୍ଗ;
    ସ୍ଥିତି REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); ଭାଙ୍ଗ;
    ସ୍ଥିତି REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); ଭାଙ୍ଗ;
    ସ୍ଥିତି REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); ଭାଙ୍ଗ;
    ସ୍ଥିତି REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); ଭାଙ୍ଗ;
    ସ୍ଥିତି REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); ଭାଙ୍ଗ;
    ସ୍ଥିତି REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); ଭାଙ୍ଗ;
    ସ୍ଥିତି REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        ଭାଙ୍ଗ;
    }
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> ସ୍ଥିର ଶୂନ୍ୟ runMinMax(ଅଚଳ Plan& pl, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op, A* res)
{
    ଯଦି (pl.ଗଣନା_3 == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    ଅନ୍ୟଥା ଯଦି (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    ଅନ୍ୟଥା
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

ସ୍ଥିର ଶୂନ୍ୟ dispatch(ଅଚଳ Plan& pl, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op, uchar* res)
{
    ଅଚଳ ତାର୍କିକ minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    ବାଛ (pl.adepth)
    {
    ସ୍ଥିତି CV_32F:
        ଯଦି (minmax) runMinMax<ଦଶମିକ>(pl, op, (ଦଶମିକ*)res); ଅନ୍ୟଥା runOp<ଦଶମିକ>(pl, op, (ଦଶମିକ*)res);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_64F:
        ଯଦି (minmax) runMinMax<ଦ୍ୱିଗୁଣ>(pl, op, (ଦ୍ୱିଗୁଣ*)res); ଅନ୍ୟଥା runOp<ଦ୍ୱିଗୁଣ>(pl, op, (ଦ୍ୱିଗୁଣ*)res);
        ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_8S:  runMinMax<schar>(pl, op, (schar*)res); ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16S: runMinMax<short>(pl, op, (short*)res); ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_32S: runMinMax<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(pl, op, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)res); ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା accumulatorDepth(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op)
{
    ଯଦି (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        ଯଦି (sdepth == CV_16F || sdepth == CV_16BF)
            ଫେରନ୍ତୁ CV_32F;
        ଫେରନ୍ତୁ sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ଅଚଳ ତାର୍କିକ wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    ଫେରନ୍ତୁ wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rtype, ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& _axes, ତାର୍କିକ _keepdims, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

ଶୂନ୍ୟ reduce(ନିବେଶ_Array _src, OutputArray _dst, ଅଚଳ ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା op = params.rtype, dtype = params.dtype;
    ଅଚଳ ତାର୍କିକ keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = src.dims, cn = src.channels();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth = src.depth();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    ଯଦି ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    ତାର୍କିକ reduced[CV_MAX_DIM] = {ମିଥ୍ୟା};
    ଯଦି (params.axes.empty())
        std::fill(reduced, reduced + dims, ସତ୍ୟ);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = ସତ୍ୟ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        ଯଦି (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype_ = CV_MAKETYPE(ddepth, cn);
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    ତାର୍କିକ red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.ଗଣନା_3 = 1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
    {
        ଯଦି (reduced[i])
            pl.ଗଣନା_3 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    ଯଦି (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = ମିଥ୍ୟା;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    ଅଚଳ ତାର୍କିକ direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    ଯଦି (!direct)
    {
        resMat.create(1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    ଯଦି (pl.nout == 0)
        ଫେରନ୍ତୁ;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
    {
        ଯଦି (size[i] == 0)
        {
            // empty source along this axis
            ଯଦି (red[i])
            {
                pl.ଗଣନା_3 = 0;
                ଜାରି;
            }
            ଫେରନ୍ତୁ;
        }
        ଯଦି (size[i] == 1)
            ଜାରି;
        ଯଦି (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        ଅନ୍ୟଥା
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    ଯଦି (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = ମିଥ୍ୟା;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m; i++)
    {
        ଯଦି (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        ଅନ୍ୟଥା
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    ଯଦି (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    ଯଦି (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        ଯଦି (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
