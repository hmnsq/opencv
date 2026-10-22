// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/hal/intrin.hpp"

#अन्तर्भाव <algorithm>
#अन्तर्भाव <cmath>
#अन्तर्भाव <limits>

नामाकाश cv {

नामाकाश {

गणना
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> संरचना VecT { गणना { ok = 0 }; प्रकारनिर्देश T vtype; };
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> संरचना VecT<T> { गणना { ok = 1 }; प्रकारनिर्देश V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(पूर्णाङ्क, v_int32)
CV_RND_VEC(भिन्न, v_float32)
#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(द्विगुण, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> संरचना OpSum
{
    स्थिर A init() { प्रत्यागम A(0); }
    स्थिर A apply(A a, A x) { प्रत्यागम a + x; }
    स्थिर A combine(A a, A b) { प्रत्यागम a + b; }
    स्थिर A finalize(A a, int64) { प्रत्यागम a; }
    template<typename V> स्थिर V vapply(ध्रुव V& a, ध्रुव V& x) { प्रत्यागम v_add(a, x); }
    template<typename V> स्थिर V vcombine(ध्रुव V& a, ध्रुव V& b) { प्रत्यागम v_add(a, b); }
};

template<typename A> संरचना OpAvg : OpSum<A>
{
    स्थिर A finalize(A a, int64 n) { प्रत्यागम n > 0 ? A(a/(द्विगुण)n) : A(0); }
};

template<typename A> संरचना OpLogSum : OpSum<A>
{
    स्थिर A finalize(A a, int64) { प्रत्यागम A(std::log(a)); }
};

template<typename A> संरचना OpSum2 : OpSum<A>
{
    स्थिर A apply(A a, A x) { प्रत्यागम a + x*x; }
    template<typename V> स्थिर V vapply(ध्रुव V& a, ध्रुव V& x) { प्रत्यागम v_add(a, v_mul(x, x)); }
};

template<typename A> संरचना OpL2 : OpSum2<A>
{
    स्थिर A finalize(A a, int64) { प्रत्यागम A(std::sqrt(a)); }
};

template<typename A> संरचना OpL1 : OpSum<A>
{
    स्थिर A apply(A a, A x) { प्रत्यागम a + (x < A(0) ? -x : x); }
    template<typename V> स्थिर V vapply(ध्रुव V& a, ध्रुव V& x) { प्रत्यागम v_add(a, v_abs(x)); }
};

template<typename A> संरचना OpProd
{
    स्थिर A init() { प्रत्यागम A(1); }
    स्थिर A apply(A a, A x) { प्रत्यागम a*x; }
    स्थिर A combine(A a, A b) { प्रत्यागम a*b; }
    स्थिर A finalize(A a, int64) { प्रत्यागम a; }
    template<typename V> स्थिर V vapply(ध्रुव V& a, ध्रुव V& x) { प्रत्यागम v_mul(a, x); }
    template<typename V> स्थिर V vcombine(ध्रुव V& a, ध्रुव V& b) { प्रत्यागम v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, तार्किक hasInf = std::numeric_limits<A>::has_infinity> संरचना Extremes
{
    स्थिर A निचला() { प्रत्यागम std::numeric_limits<A>::lowest(); }
    स्थिर A ऊपरी() { प्रत्यागम std::numeric_limits<A>::max(); }
};

template<typename A> संरचना Extremes<A, सत्य>
{
    स्थिर A निचला() { प्रत्यागम -std::numeric_limits<A>::infinity(); }
    स्थिर A ऊपरी() { प्रत्यागम std::numeric_limits<A>::infinity(); }
};

template<typename A> संरचना OpMax
{
    स्थिर A init() { प्रत्यागम Extremes<A>::निचला(); }
    स्थिर A apply(A a, A x) { प्रत्यागम a < x ? x : a; }
    स्थिर A combine(A a, A b) { प्रत्यागम a < b ? b : a; }
    स्थिर A finalize(A a, int64) { प्रत्यागम a; }
    template<typename V> स्थिर V vapply(ध्रुव V& a, ध्रुव V& x) { प्रत्यागम v_max(a, x); }
    template<typename V> स्थिर V vcombine(ध्रुव V& a, ध्रुव V& b) { प्रत्यागम v_max(a, b); }
};

template<typename A> संरचना OpMin
{
    स्थिर A init() { प्रत्यागम Extremes<A>::ऊपरी(); }
    स्थिर A apply(A a, A x) { प्रत्यागम x < a ? x : a; }
    स्थिर A combine(A a, A b) { प्रत्यागम b < a ? b : a; }
    स्थिर A finalize(A a, int64) { प्रत्यागम a; }
    template<typename V> स्थिर V vapply(ध्रुव V& a, ध्रुव V& x) { प्रत्यागम v_min(a, x); }
    template<typename V> स्थिर V vcombine(ध्रुव V& a, ध्रुव V& b) { प्रत्यागम v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> संरचना OpSumExpShifted
{
    स्थिर A init() { प्रत्यागम A(0); }
    स्थिर A apply(A a, A x, A m) { प्रत्यागम a + A(std::exp(x - m)); }
    स्थिर A combine(A a, A b) { प्रत्यागम a + b; }
    स्थिर A finalize(A a, A m) { प्रत्यागम std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<वर्ग Op, typename A, तार्किक vec = (VecT<A>::ok != 0)> संरचना HRow
{
    स्थिर A run(A acc, ध्रुव A* x, int64 n)
    {
        प्रति (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        प्रत्यागम acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<वर्ग Op, typename A, तार्किक vec = (VecT<A>::ok != 0)> संरचना VRow
{
    स्थिर शून्य run(A* acc, ध्रुव A* x, int64 n)
    {
        प्रति (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
template<वर्ग Op, typename A> संरचना HRow<Op, A, सत्य>
{
    स्थिर A run(A acc, ध्रुव A* x, int64 n)
    {
        प्रकारनिर्देश typename VecT<A>::vtype V;
        ध्रुव पूर्णाङ्क L = VTraits<V>::vlanes();
        int64 i = 0;
        यदि (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            प्रति (पूर्णाङ्क k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            प्रति (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            प्रति (पूर्णाङ्क k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        प्रति (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        प्रत्यागम acc;
    }
};

template<वर्ग Op, typename A> संरचना VRow<Op, A, सत्य>
{
    स्थिर शून्य run(A* acc, ध्रुव A* x, int64 n)
    {
        प्रकारनिर्देश typename VecT<A>::vtype V;
        ध्रुव पूर्णाङ्क L = VTraits<V>::vlanes();
        int64 i = 0;
        प्रति (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        प्रति (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

संरचना Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    पूर्णाङ्क nk;
    पूर्णाङ्क ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    पूर्णाङ्क nr;
    पूर्णाङ्क rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    तार्किक innerReduced;      // the innermost axis is reduced (horizontal mode)

    ध्रुव uchar* src;
    पूर्णाङ्क sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 गिनती_2;            // reduced elements per output
    int64 nout;             // number of outputs
    द्विआधारी_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
संरचना Iter
{
    पूर्णाङ्क n = 0;
    ध्रुव पूर्णाङ्क* size = nullptr;
    ध्रुव ptrdiff_t* s = nullptr;
    ध्रुव ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    शून्य init(पूर्णाङ्क n_, ध्रुव पूर्णाङ्क* size_, ध्रुव ptrdiff_t* s_, ध्रुव ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        प्रति (पूर्णाङ्क k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            यदि (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    शून्य next()
    {
        प्रति (पूर्णाङ्क k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            यदि (d)
                doff += d[k];
            यदि (++idx[k] < size[k])
                प्रत्यागम;
            soff -= (ptrdiff_t)size[k]*s[k];
            यदि (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> स्थिर ध्रुव A* loadRow(ध्रुव Plan& pl, ध्रुव uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    यदि (step != (ptrdiff_t)pl.sesz1)
    {
        प्रति (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    यदि (!pl.cvt)
        प्रत्यागम (ध्रुव A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((पूर्णाङ्क)len, 1), 0);
    प्रत्यागम buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> स्थिर शून्य forRow(ध्रुव Plan& pl, ध्रुव uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ध्रुव तार्किक direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    ध्रुव int64 blk = direct ? n : (int64)RND_BLOCK;
    प्रति (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
संरचना Work
{
    int64 nitems, nsplit;
    द्विगुण nstripes;
    तार्किक serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, वर्ग Op, वर्ग OpM> स्थिर शून्य runHorizontal(ध्रुव Plan& pl, ध्रुव Work& w, A* res,
                                                                   ध्रुव A* mbuf, A* partial)
{
    ध्रुव पूर्णाङ्क nr = pl.nr;
    ध्रुव int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ध्रुव ptrdiff_t rstep = pl.rsstep[nr - 1];
    ध्रुव int64 nsplit = w.nsplit;

    auto body = [&](ध्रुव Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        यदि (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        यावत् (it < range.end)
        {
            ध्रुव int64 o = it / nsplit, s = it - o*nsplit;
            यदि (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ध्रुव uchar* base = pl.src + ko.soff;
            ध्रुव A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.गिनती_2/nsplit, b = (s + 1)*pl.गिनती_2/nsplit;
            A acc = Op::init();
            यदि (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                यावत् (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](ध्रुव A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            यदि (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.गिनती_2, m);
                ko.next();
            }
            अन्यथा
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    यदि (w.serial)
        body(Range(0, (पूर्णाङ्क)w.nitems));
    अन्यथा
        parallel_for_(Range(0, (पूर्णाङ्क)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, वर्ग Op, वर्ग OpM> स्थिर शून्य runVertical(ध्रुव Plan& pl, ध्रुव Work& w, A* res,
                                                                 ध्रुव A* mbuf, A* partial)
{
    ध्रुव पूर्णाङ्क nk = pl.nk;
    ध्रुव int64 m = pl.ksize[nk - 1];
    ध्रुव ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ध्रुव int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ध्रुव int64 nsplit = w.nsplit;
    int64 rcount = 1;
    प्रति (पूर्णाङ्क i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](ध्रुव Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        प्रति (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ध्रुव int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ध्रुव ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ध्रुव A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            प्रति (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            यदि (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                प्रति (; a < b; a++, ro.next())
                {
                    ध्रुव uchar* p = pl.src + soff + ro.soff;
                    ध्रुव A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            यदि (nsplit == 1)
                प्रति (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.गिनती_2, mrow ? mrow[i] : A(0));
            अन्यथा
                प्रति (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    यदि (w.serial)
        body(Range(0, (पूर्णाङ्क)w.nitems));
    अन्यथा
        parallel_for_(Range(0, (पूर्णाङ्क)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, वर्ग Op> संरचना Plain
{
    स्थिर A run(A acc, ध्रुव A* x, int64 n, A) { प्रत्यागम HRow<Op, A>::run(acc, x, n); }
    स्थिर शून्य vrun(A* acc, ध्रुव A* x, int64 n, ध्रुव A*) { VRow<Op, A>::run(acc, x, n); }
    स्थिर A finalize(A a, int64 गिनती_2, A) { प्रत्यागम Op::finalize(a, गिनती_2); }
};

template<typename A> संरचना ShiftedExp
{
    प्रकारनिर्देश OpSumExpShifted<A> Op;
    स्थिर A run(A acc, ध्रुव A* x, int64 n, A m)
    {
        प्रति (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        प्रत्यागम acc;
    }
    स्थिर शून्य vrun(A* acc, ध्रुव A* x, int64 n, ध्रुव A* m)
    {
        प्रति (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    स्थिर A finalize(A a, int64, A m) { प्रत्यागम Op::finalize(a, m); }
};

template<typename A, वर्ग Op, वर्ग OpM> स्थिर शून्य runPass(ध्रुव Plan& pl, A* res, ध्रुव A* mbuf)
{
    ध्रुव द्विगुण work = (द्विगुण)pl.nout*(द्विगुण)std::max(pl.गिनती_2, (int64)1);
    ध्रुव पूर्णाङ्क nthreads = work < RND_SERIAL_WORK ? 1 : get_अंक_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 वस्तुएँ;
    यदि (pl.innerReduced)
        वस्तुएँ = pl.nout;
    अन्यथा
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        वस्तुएँ = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ध्रुव int64 want = 4*(int64)nthreads;
    यदि (!w.serial && वस्तुएँ < want && pl.गिनती_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.गिनती_2/4096 : pl.गिनती_2/16;
        w.nsplit = std::max((int64)1, std::min((want + वस्तुएँ - 1)/वस्तुएँ, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = वस्तुएँ*w.nsplit;
    w.nstripes = std::min((द्विगुण)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    यदि (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    अन्यथा
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    यदि (w.nsplit > 1)
    {
        प्रति (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            प्रति (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.गिनती_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> स्थिर शून्य fillResult(A* res, int64 n, A v)
{
    प्रति (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> स्थिर शून्य runOp(ध्रुव Plan& pl, पूर्णाङ्क op, A* res)
{
    यदि (pl.गिनती_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        प्रत्यागम;
    }
    विकल्प (op)
    {
    पक्ष REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); भङ्ग;
    पक्ष REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); भङ्ग;
    पक्ष REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); भङ्ग;
    पक्ष REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); भङ्ग;
    पक्ष REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); भङ्ग;
    पक्ष REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); भङ्ग;
    पक्ष REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); भङ्ग;
    पक्ष REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        भङ्ग;
    }
    स्वभाव:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> स्थिर शून्य runMinMax(ध्रुव Plan& pl, पूर्णाङ्क op, A* res)
{
    यदि (pl.गिनती_2 == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    अन्यथा यदि (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    अन्यथा
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

स्थिर शून्य dispatch(ध्रुव Plan& pl, पूर्णाङ्क op, uchar* res)
{
    ध्रुव तार्किक minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    विकल्प (pl.adepth)
    {
    पक्ष CV_32F:
        यदि (minmax) runMinMax<भिन्न>(pl, op, (भिन्न*)res); अन्यथा runOp<भिन्न>(pl, op, (भिन्न*)res);
        भङ्ग;
    पक्ष CV_64F:
        यदि (minmax) runMinMax<द्विगुण>(pl, op, (द्विगुण*)res); अन्यथा runOp<द्विगुण>(pl, op, (द्विगुण*)res);
        भङ्ग;
    पक्ष CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); भङ्ग;
    पक्ष CV_8S:  runMinMax<schar>(pl, op, (schar*)res); भङ्ग;
    पक्ष CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); भङ्ग;
    पक्ष CV_16S: runMinMax<short>(pl, op, (short*)res); भङ्ग;
    पक्ष CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); भङ्ग;
    पक्ष CV_32S: runMinMax<पूर्णाङ्क>(pl, op, (पूर्णाङ्क*)res); भङ्ग;
    पक्ष CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); भङ्ग;
    पक्ष CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); भङ्ग;
    स्वभाव:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

स्थिर पूर्णाङ्क accumulatorDepth(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क op)
{
    यदि (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        यदि (sdepth == CV_16F || sdepth == CV_16BF)
            प्रत्यागम CV_32F;
        प्रत्यागम sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ध्रुव तार्किक wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    प्रत्यागम wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(पूर्णाङ्क _rtype, ध्रुव std::vector<पूर्णाङ्क>& _axes, तार्किक _keepdims, पूर्णाङ्क _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

शून्य reduce(निवेश_Array _src, OutputArray _dst, ध्रुव ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ध्रुव पूर्णाङ्क op = params.rtype, dtype = params.dtype;
    ध्रुव तार्किक keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ध्रुव पूर्णाङ्क dims = src.dims, cn = src.channels();
    ध्रुव पूर्णाङ्क sdepth = src.depth();
    पूर्णाङ्क ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    यदि ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    तार्किक reduced[CV_MAX_DIM] = {असत्य};
    यदि (params.axes.empty())
        std::fill(reduced, reduced + dims, सत्य);
    प्रति (पूर्णाङ्क a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        पूर्णाङ्क ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = सत्य;
    }

    पूर्णाङ्क keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        यदि (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    ध्रुव पूर्णाङ्क dtype_ = CV_MAKETYPE(ddepth, cn);
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
    पूर्णाङ्क n = 0;
    पूर्णाङ्क size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    तार्किक red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    प्रति (पूर्णाङ्क i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.गिनती_2 = 1;
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
    {
        यदि (reduced[i])
            pl.गिनती_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    यदि (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = असत्य;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    ध्रुव तार्किक direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    यदि (!direct)
    {
        resMat.create(1, (पूर्णाङ्क)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    यदि (pl.nout == 0)
        प्रत्यागम;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    पूर्णाङ्क m = 0;
    प्रति (पूर्णाङ्क i = 0; i < n; i++)
    {
        यदि (size[i] == 0)
        {
            // empty source along this axis
            यदि (red[i])
            {
                pl.गिनती_2 = 0;
                अनुवर्त;
            }
            प्रत्यागम;
        }
        यदि (size[i] == 1)
            अनुवर्त;
        यदि (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        अन्यथा
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    यदि (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = असत्य;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    प्रति (पूर्णाङ्क i = 0; i < m; i++)
    {
        यदि (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        अन्यथा
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    यदि (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    यदि (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        यदि (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
