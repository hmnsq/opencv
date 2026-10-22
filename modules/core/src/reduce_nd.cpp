// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencv2/core/hal/intrin.hpp"

#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <cmath>
#அந்தர்பாவ <limits>

நாமாகாஶ cv {

நாமாகாஶ {

எண்ணிக்கை
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> கட்டமைப்பு VecT { எண்ணிக்கை { ok = 0 }; வகைவரை T vtype; };
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> கட்டமைப்பு VecT<T> { எண்ணிக்கை { ok = 1 }; வகைவரை V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(முழு, v_int32)
CV_RND_VEC(தசம, v_float32)
#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(இரட்டை, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> கட்டமைப்பு OpSum
{
    நிலையான A init() { திரும்பு A(0); }
    நிலையான A apply(A a, A x) { திரும்பு a + x; }
    நிலையான A combine(A a, A b) { திரும்பு a + b; }
    நிலையான A finalize(A a, int64) { திரும்பு a; }
    template<typename V> நிலையான V vapply(மாறிலி V& a, மாறிலி V& x) { திரும்பு v_add(a, x); }
    template<typename V> நிலையான V vcombine(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_add(a, b); }
};

template<typename A> கட்டமைப்பு OpAvg : OpSum<A>
{
    நிலையான A finalize(A a, int64 n) { திரும்பு n > 0 ? A(a/(இரட்டை)n) : A(0); }
};

template<typename A> கட்டமைப்பு OpLogSum : OpSum<A>
{
    நிலையான A finalize(A a, int64) { திரும்பு A(std::log(a)); }
};

template<typename A> கட்டமைப்பு OpSum2 : OpSum<A>
{
    நிலையான A apply(A a, A x) { திரும்பு a + x*x; }
    template<typename V> நிலையான V vapply(மாறிலி V& a, மாறிலி V& x) { திரும்பு v_add(a, v_mul(x, x)); }
};

template<typename A> கட்டமைப்பு OpL2 : OpSum2<A>
{
    நிலையான A finalize(A a, int64) { திரும்பு A(std::sqrt(a)); }
};

template<typename A> கட்டமைப்பு OpL1 : OpSum<A>
{
    நிலையான A apply(A a, A x) { திரும்பு a + (x < A(0) ? -x : x); }
    template<typename V> நிலையான V vapply(மாறிலி V& a, மாறிலி V& x) { திரும்பு v_add(a, v_abs(x)); }
};

template<typename A> கட்டமைப்பு OpProd
{
    நிலையான A init() { திரும்பு A(1); }
    நிலையான A apply(A a, A x) { திரும்பு a*x; }
    நிலையான A combine(A a, A b) { திரும்பு a*b; }
    நிலையான A finalize(A a, int64) { திரும்பு a; }
    template<typename V> நிலையான V vapply(மாறிலி V& a, மாறிலி V& x) { திரும்பு v_mul(a, x); }
    template<typename V> நிலையான V vcombine(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, தருக்க hasInf = std::numeric_limits<A>::has_infinity> கட்டமைப்பு Extremes
{
    நிலையான A கீழ்() { திரும்பு std::numeric_limits<A>::lowest(); }
    நிலையான A உயர்வு() { திரும்பு std::numeric_limits<A>::max(); }
};

template<typename A> கட்டமைப்பு Extremes<A, உண்மை>
{
    நிலையான A கீழ்() { திரும்பு -std::numeric_limits<A>::infinity(); }
    நிலையான A உயர்வு() { திரும்பு std::numeric_limits<A>::infinity(); }
};

template<typename A> கட்டமைப்பு OpMax
{
    நிலையான A init() { திரும்பு Extremes<A>::கீழ்(); }
    நிலையான A apply(A a, A x) { திரும்பு a < x ? x : a; }
    நிலையான A combine(A a, A b) { திரும்பு a < b ? b : a; }
    நிலையான A finalize(A a, int64) { திரும்பு a; }
    template<typename V> நிலையான V vapply(மாறிலி V& a, மாறிலி V& x) { திரும்பு v_max(a, x); }
    template<typename V> நிலையான V vcombine(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_max(a, b); }
};

template<typename A> கட்டமைப்பு OpMin
{
    நிலையான A init() { திரும்பு Extremes<A>::உயர்வு(); }
    நிலையான A apply(A a, A x) { திரும்பு x < a ? x : a; }
    நிலையான A combine(A a, A b) { திரும்பு b < a ? b : a; }
    நிலையான A finalize(A a, int64) { திரும்பு a; }
    template<typename V> நிலையான V vapply(மாறிலி V& a, மாறிலி V& x) { திரும்பு v_min(a, x); }
    template<typename V> நிலையான V vcombine(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> கட்டமைப்பு OpSumExpShifted
{
    நிலையான A init() { திரும்பு A(0); }
    நிலையான A apply(A a, A x, A m) { திரும்பு a + A(std::exp(x - m)); }
    நிலையான A combine(A a, A b) { திரும்பு a + b; }
    நிலையான A finalize(A a, A m) { திரும்பு std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<வகுப்பு Op, typename A, தருக்க vec = (VecT<A>::ok != 0)> கட்டமைப்பு HRow
{
    நிலையான A run(A acc, மாறிலி A* x, int64 n)
    {
        க்கு (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        திரும்பு acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<வகுப்பு Op, typename A, தருக்க vec = (VecT<A>::ok != 0)> கட்டமைப்பு VRow
{
    நிலையான வெறுமை run(A* acc, மாறிலி A* x, int64 n)
    {
        க்கு (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
template<வகுப்பு Op, typename A> கட்டமைப்பு HRow<Op, A, உண்மை>
{
    நிலையான A run(A acc, மாறிலி A* x, int64 n)
    {
        வகைவரை typename VecT<A>::vtype V;
        மாறிலி முழு L = VTraits<V>::vlanes();
        int64 i = 0;
        எனில் (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            க்கு (முழு k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            க்கு (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            க்கு (முழு k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        க்கு (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        திரும்பு acc;
    }
};

template<வகுப்பு Op, typename A> கட்டமைப்பு VRow<Op, A, உண்மை>
{
    நிலையான வெறுமை run(A* acc, மாறிலி A* x, int64 n)
    {
        வகைவரை typename VecT<A>::vtype V;
        மாறிலி முழு L = VTraits<V>::vlanes();
        int64 i = 0;
        க்கு (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        க்கு (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

கட்டமைப்பு Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    முழு nk;
    முழு ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    முழு nr;
    முழு rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    தருக்க innerReduced;      // the innermost axis is reduced (horizontal mode)

    மாறிலி uchar* src;
    முழு sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 மொத்தம்_2;            // reduced elements per output
    int64 nout;             // number of outputs
    இருமம்_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
கட்டமைப்பு Iter
{
    முழு n = 0;
    மாறிலி முழு* size = nullptr;
    மாறிலி ptrdiff_t* s = nullptr;
    மாறிலி ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    வெறுமை init(முழு n_, மாறிலி முழு* size_, மாறிலி ptrdiff_t* s_, மாறிலி ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        க்கு (முழு k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            எனில் (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    வெறுமை next()
    {
        க்கு (முழு k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            எனில் (d)
                doff += d[k];
            எனில் (++idx[k] < size[k])
                திரும்பு;
            soff -= (ptrdiff_t)size[k]*s[k];
            எனில் (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> நிலையான மாறிலி A* loadRow(மாறிலி Plan& pl, மாறிலி uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    எனில் (step != (ptrdiff_t)pl.sesz1)
    {
        க்கு (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    எனில் (!pl.cvt)
        திரும்பு (மாறிலி A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((முழு)len, 1), 0);
    திரும்பு buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> நிலையான வெறுமை forRow(மாறிலி Plan& pl, மாறிலி uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    மாறிலி தருக்க direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    மாறிலி int64 blk = direct ? n : (int64)RND_BLOCK;
    க்கு (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
கட்டமைப்பு Work
{
    int64 nitems, nsplit;
    இரட்டை nstripes;
    தருக்க serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, வகுப்பு Op, வகுப்பு OpM> நிலையான வெறுமை runHorizontal(மாறிலி Plan& pl, மாறிலி Work& w, A* res,
                                                                   மாறிலி A* mbuf, A* partial)
{
    மாறிலி முழு nr = pl.nr;
    மாறிலி int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    மாறிலி ptrdiff_t rstep = pl.rsstep[nr - 1];
    மாறிலி int64 nsplit = w.nsplit;

    auto body = [&](மாறிலி Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        எனில் (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        போது (it < range.end)
        {
            மாறிலி int64 o = it / nsplit, s = it - o*nsplit;
            எனில் (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            மாறிலி uchar* base = pl.src + ko.soff;
            மாறிலி A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.மொத்தம்_2/nsplit, b = (s + 1)*pl.மொத்தம்_2/nsplit;
            A acc = Op::init();
            எனில் (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                போது (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](மாறிலி A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            எனில் (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.மொத்தம்_2, m);
                ko.next();
            }
            இல்லையெனில்
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    எனில் (w.serial)
        body(Range(0, (முழு)w.nitems));
    இல்லையெனில்
        parallel_for_(Range(0, (முழு)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, வகுப்பு Op, வகுப்பு OpM> நிலையான வெறுமை runVertical(மாறிலி Plan& pl, மாறிலி Work& w, A* res,
                                                                 மாறிலி A* mbuf, A* partial)
{
    மாறிலி முழு nk = pl.nk;
    மாறிலி int64 m = pl.ksize[nk - 1];
    மாறிலி ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    மாறிலி int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    மாறிலி int64 nsplit = w.nsplit;
    int64 rcount = 1;
    க்கு (முழு i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](மாறிலி Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        க்கு (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            மாறிலி int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            மாறிலி ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            மாறிலி A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            க்கு (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            எனில் (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                க்கு (; a < b; a++, ro.next())
                {
                    மாறிலி uchar* p = pl.src + soff + ro.soff;
                    மாறிலி A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            எனில் (nsplit == 1)
                க்கு (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.மொத்தம்_2, mrow ? mrow[i] : A(0));
            இல்லையெனில்
                க்கு (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    எனில் (w.serial)
        body(Range(0, (முழு)w.nitems));
    இல்லையெனில்
        parallel_for_(Range(0, (முழு)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, வகுப்பு Op> கட்டமைப்பு Plain
{
    நிலையான A run(A acc, மாறிலி A* x, int64 n, A) { திரும்பு HRow<Op, A>::run(acc, x, n); }
    நிலையான வெறுமை vrun(A* acc, மாறிலி A* x, int64 n, மாறிலி A*) { VRow<Op, A>::run(acc, x, n); }
    நிலையான A finalize(A a, int64 மொத்தம்_2, A) { திரும்பு Op::finalize(a, மொத்தம்_2); }
};

template<typename A> கட்டமைப்பு ShiftedExp
{
    வகைவரை OpSumExpShifted<A> Op;
    நிலையான A run(A acc, மாறிலி A* x, int64 n, A m)
    {
        க்கு (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        திரும்பு acc;
    }
    நிலையான வெறுமை vrun(A* acc, மாறிலி A* x, int64 n, மாறிலி A* m)
    {
        க்கு (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    நிலையான A finalize(A a, int64, A m) { திரும்பு Op::finalize(a, m); }
};

template<typename A, வகுப்பு Op, வகுப்பு OpM> நிலையான வெறுமை runPass(மாறிலி Plan& pl, A* res, மாறிலி A* mbuf)
{
    மாறிலி இரட்டை work = (இரட்டை)pl.nout*(இரட்டை)std::max(pl.மொத்தம்_2, (int64)1);
    மாறிலி முழு nthreads = work < RND_SERIAL_WORK ? 1 : get_இலக்கம்_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 பொருட்கள்;
    எனில் (pl.innerReduced)
        பொருட்கள் = pl.nout;
    இல்லையெனில்
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        பொருட்கள் = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    மாறிலி int64 want = 4*(int64)nthreads;
    எனில் (!w.serial && பொருட்கள் < want && pl.மொத்தம்_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.மொத்தம்_2/4096 : pl.மொத்தம்_2/16;
        w.nsplit = std::max((int64)1, std::min((want + பொருட்கள் - 1)/பொருட்கள், std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = பொருட்கள்*w.nsplit;
    w.nstripes = std::min((இரட்டை)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    எனில் (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    இல்லையெனில்
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    எனில் (w.nsplit > 1)
    {
        க்கு (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            க்கு (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.மொத்தம்_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> நிலையான வெறுமை fillResult(A* res, int64 n, A v)
{
    க்கு (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> நிலையான வெறுமை runOp(மாறிலி Plan& pl, முழு op, A* res)
{
    எனில் (pl.மொத்தம்_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        திரும்பு;
    }
    தேர்வு (op)
    {
    நிலை REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); நிறுத்து;
    நிலை REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); நிறுத்து;
    நிலை REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); நிறுத்து;
    நிலை REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); நிறுத்து;
    நிலை REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); நிறுத்து;
    நிலை REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); நிறுத்து;
    நிலை REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); நிறுத்து;
    நிலை REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        நிறுத்து;
    }
    இயல்பு:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> நிலையான வெறுமை runMinMax(மாறிலி Plan& pl, முழு op, A* res)
{
    எனில் (pl.மொத்தம்_2 == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    இல்லையெனில் எனில் (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    இல்லையெனில்
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

நிலையான வெறுமை dispatch(மாறிலி Plan& pl, முழு op, uchar* res)
{
    மாறிலி தருக்க minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    தேர்வு (pl.adepth)
    {
    நிலை CV_32F:
        எனில் (minmax) runMinMax<தசம>(pl, op, (தசம*)res); இல்லையெனில் runOp<தசம>(pl, op, (தசம*)res);
        நிறுத்து;
    நிலை CV_64F:
        எனில் (minmax) runMinMax<இரட்டை>(pl, op, (இரட்டை*)res); இல்லையெனில் runOp<இரட்டை>(pl, op, (இரட்டை*)res);
        நிறுத்து;
    நிலை CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); நிறுத்து;
    நிலை CV_8S:  runMinMax<schar>(pl, op, (schar*)res); நிறுத்து;
    நிலை CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); நிறுத்து;
    நிலை CV_16S: runMinMax<short>(pl, op, (short*)res); நிறுத்து;
    நிலை CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); நிறுத்து;
    நிலை CV_32S: runMinMax<முழு>(pl, op, (முழு*)res); நிறுத்து;
    நிலை CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); நிறுத்து;
    நிலை CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); நிறுத்து;
    இயல்பு:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

நிலையான முழு accumulatorDepth(முழு sdepth, முழு ddepth, முழு op)
{
    எனில் (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        எனில் (sdepth == CV_16F || sdepth == CV_16BF)
            திரும்பு CV_32F;
        திரும்பு sdepth == CV_Bool ? CV_8U : sdepth;
    }
    மாறிலி தருக்க wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    திரும்பு wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(முழு _rtype, மாறிலி std::vector<முழு>& _axes, தருக்க _keepdims, முழு _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

வெறுமை reduce(உள்ளீடு_Array _src, OutputArray _dst, மாறிலி ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    மாறிலி முழு op = params.rtype, dtype = params.dtype;
    மாறிலி தருக்க keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    மாறிலி முழு dims = src.dims, cn = src.channels();
    மாறிலி முழு sdepth = src.depth();
    முழு ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    எனில் ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    தருக்க reduced[CV_MAX_DIM] = {பொய்};
    எனில் (params.axes.empty())
        std::fill(reduced, reduced + dims, உண்மை);
    க்கு (முழு a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        முழு ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = உண்மை;
    }

    முழு keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    க்கு (முழு i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        எனில் (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    மாறிலி முழு dtype_ = CV_MAKETYPE(ddepth, cn);
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
    முழு n = 0;
    முழு size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    தருக்க red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    க்கு (முழு i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.மொத்தம்_2 = 1;
    க்கு (முழு i = 0; i < dims; i++)
    {
        எனில் (reduced[i])
            pl.மொத்தம்_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    எனில் (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = பொய்;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    மாறிலி தருக்க direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    எனில் (!direct)
    {
        resMat.create(1, (முழு)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    எனில் (pl.nout == 0)
        திரும்பு;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    முழு m = 0;
    க்கு (முழு i = 0; i < n; i++)
    {
        எனில் (size[i] == 0)
        {
            // empty source along this axis
            எனில் (red[i])
            {
                pl.மொத்தம்_2 = 0;
                தொடர்;
            }
            திரும்பு;
        }
        எனில் (size[i] == 1)
            தொடர்;
        எனில் (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        இல்லையெனில்
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    எனில் (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = பொய்;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    க்கு (முழு i = 0; i < m; i++)
    {
        எனில் (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        இல்லையெனில்
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    எனில் (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    எனில் (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        எனில் (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
