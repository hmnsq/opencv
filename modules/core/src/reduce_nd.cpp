// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencv2/core/hal/intrin.hpp"

#અન્તર્ભાવ <algorithm>
#અન્તર્ભાવ <cmath>
#અન્તર્ભાવ <limits>

નામાકાશ cv {

નામાકાશ {

ગણના
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> સંરચના VecT { ગણના { ok = 0 }; પ્રકારવ્યાખ્યા T vtype; };
#જો (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> સંરચના VecT<T> { ગણના { ok = 1 }; પ્રકારવ્યાખ્યા V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(પૂર્ણાંક, v_int32)
CV_RND_VEC(દશાંશ, v_float32)
#જો (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(દ્વિગુણ, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> સંરચના OpSum
{
    સ્થિર A init() { પાછા A(0); }
    સ્થિર A apply(A a, A x) { પાછા a + x; }
    સ્થિર A combine(A a, A b) { પાછા a + b; }
    સ્થિર A finalize(A a, int64) { પાછા a; }
    template<typename V> સ્થિર V vapply(અચલ V& a, અચલ V& x) { પાછા v_add(a, x); }
    template<typename V> સ્થિર V vcombine(અચલ V& a, અચલ V& b) { પાછા v_add(a, b); }
};

template<typename A> સંરચના OpAvg : OpSum<A>
{
    સ્થિર A finalize(A a, int64 n) { પાછા n > 0 ? A(a/(દ્વિગુણ)n) : A(0); }
};

template<typename A> સંરચના OpLogSum : OpSum<A>
{
    સ્થિર A finalize(A a, int64) { પાછા A(std::log(a)); }
};

template<typename A> સંરચના OpSum2 : OpSum<A>
{
    સ્થિર A apply(A a, A x) { પાછા a + x*x; }
    template<typename V> સ્થિર V vapply(અચલ V& a, અચલ V& x) { પાછા v_add(a, v_mul(x, x)); }
};

template<typename A> સંરચના OpL2 : OpSum2<A>
{
    સ્થિર A finalize(A a, int64) { પાછા A(std::sqrt(a)); }
};

template<typename A> સંરચના OpL1 : OpSum<A>
{
    સ્થિર A apply(A a, A x) { પાછા a + (x < A(0) ? -x : x); }
    template<typename V> સ્થિર V vapply(અચલ V& a, અચલ V& x) { પાછા v_add(a, v_abs(x)); }
};

template<typename A> સંરચના OpProd
{
    સ્થિર A init() { પાછા A(1); }
    સ્થિર A apply(A a, A x) { પાછા a*x; }
    સ્થિર A combine(A a, A b) { પાછા a*b; }
    સ્થિર A finalize(A a, int64) { પાછા a; }
    template<typename V> સ્થિર V vapply(અચલ V& a, અચલ V& x) { પાછા v_mul(a, x); }
    template<typename V> સ્થિર V vcombine(અચલ V& a, અચલ V& b) { પાછા v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, તાર્કિક hasInf = std::numeric_limits<A>::has_infinity> સંરચના Extremes
{
    સ્થિર A નીચલું() { પાછા std::numeric_limits<A>::lowest(); }
    સ્થિર A ઉપલું() { પાછા std::numeric_limits<A>::max(); }
};

template<typename A> સંરચના Extremes<A, સાચું>
{
    સ્થિર A નીચલું() { પાછા -std::numeric_limits<A>::infinity(); }
    સ્થિર A ઉપલું() { પાછા std::numeric_limits<A>::infinity(); }
};

template<typename A> સંરચના OpMax
{
    સ્થિર A init() { પાછા Extremes<A>::નીચલું(); }
    સ્થિર A apply(A a, A x) { પાછા a < x ? x : a; }
    સ્થિર A combine(A a, A b) { પાછા a < b ? b : a; }
    સ્થિર A finalize(A a, int64) { પાછા a; }
    template<typename V> સ્થિર V vapply(અચલ V& a, અચલ V& x) { પાછા v_max(a, x); }
    template<typename V> સ્થિર V vcombine(અચલ V& a, અચલ V& b) { પાછા v_max(a, b); }
};

template<typename A> સંરચના OpMin
{
    સ્થિર A init() { પાછા Extremes<A>::ઉપલું(); }
    સ્થિર A apply(A a, A x) { પાછા x < a ? x : a; }
    સ્થિર A combine(A a, A b) { પાછા b < a ? b : a; }
    સ્થિર A finalize(A a, int64) { પાછા a; }
    template<typename V> સ્થિર V vapply(અચલ V& a, અચલ V& x) { પાછા v_min(a, x); }
    template<typename V> સ્થિર V vcombine(અચલ V& a, અચલ V& b) { પાછા v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> સંરચના OpSumExpShifted
{
    સ્થિર A init() { પાછા A(0); }
    સ્થિર A apply(A a, A x, A m) { પાછા a + A(std::exp(x - m)); }
    સ્થિર A combine(A a, A b) { પાછા a + b; }
    સ્થિર A finalize(A a, A m) { પાછા std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<વર્ગ Op, typename A, તાર્કિક vec = (VecT<A>::ok != 0)> સંરચના HRow
{
    સ્થિર A run(A acc, અચલ A* x, int64 n)
    {
        માટે (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        પાછા acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<વર્ગ Op, typename A, તાર્કિક vec = (VecT<A>::ok != 0)> સંરચના VRow
{
    સ્થિર શૂન્ય run(A* acc, અચલ A* x, int64 n)
    {
        માટે (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#જો (CV_SIMD || CV_SIMD_SCALABLE)
template<વર્ગ Op, typename A> સંરચના HRow<Op, A, સાચું>
{
    સ્થિર A run(A acc, અચલ A* x, int64 n)
    {
        પ્રકારવ્યાખ્યા typename VecT<A>::vtype V;
        અચલ પૂર્ણાંક L = VTraits<V>::vlanes();
        int64 i = 0;
        જો (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            માટે (પૂર્ણાંક k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            માટે (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            માટે (પૂર્ણાંક k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        માટે (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        પાછા acc;
    }
};

template<વર્ગ Op, typename A> સંરચના VRow<Op, A, સાચું>
{
    સ્થિર શૂન્ય run(A* acc, અચલ A* x, int64 n)
    {
        પ્રકારવ્યાખ્યા typename VecT<A>::vtype V;
        અચલ પૂર્ણાંક L = VTraits<V>::vlanes();
        int64 i = 0;
        માટે (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        માટે (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

સંરચના Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    પૂર્ણાંક nk;
    પૂર્ણાંક ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    પૂર્ણાંક nr;
    પૂર્ણાંક rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    તાર્કિક innerReduced;      // the innermost axis is reduced (horizontal mode)

    અચલ uchar* src;
    પૂર્ણાંક sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 ગણતરી_2;            // reduced elements per output
    int64 nout;             // number of outputs
    દ્વિઅંકી_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
સંરચના Iter
{
    પૂર્ણાંક n = 0;
    અચલ પૂર્ણાંક* size = nullptr;
    અચલ ptrdiff_t* s = nullptr;
    અચલ ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    શૂન્ય init(પૂર્ણાંક n_, અચલ પૂર્ણાંક* size_, અચલ ptrdiff_t* s_, અચલ ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        માટે (પૂર્ણાંક k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            જો (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    શૂન્ય next()
    {
        માટે (પૂર્ણાંક k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            જો (d)
                doff += d[k];
            જો (++idx[k] < size[k])
                પાછા;
            soff -= (ptrdiff_t)size[k]*s[k];
            જો (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> સ્થિર અચલ A* loadRow(અચલ Plan& pl, અચલ uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    જો (step != (ptrdiff_t)pl.sesz1)
    {
        માટે (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    જો (!pl.cvt)
        પાછા (અચલ A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((પૂર્ણાંક)len, 1), 0);
    પાછા buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> સ્થિર શૂન્ય forRow(અચલ Plan& pl, અચલ uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    અચલ તાર્કિક direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    અચલ int64 blk = direct ? n : (int64)RND_BLOCK;
    માટે (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
સંરચના Work
{
    int64 nitems, nsplit;
    દ્વિગુણ nstripes;
    તાર્કિક serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, વર્ગ Op, વર્ગ OpM> સ્થિર શૂન્ય runHorizontal(અચલ Plan& pl, અચલ Work& w, A* res,
                                                                   અચલ A* mbuf, A* partial)
{
    અચલ પૂર્ણાંક nr = pl.nr;
    અચલ int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    અચલ ptrdiff_t rstep = pl.rsstep[nr - 1];
    અચલ int64 nsplit = w.nsplit;

    auto body = [&](અચલ Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        જો (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        જ્યારે (it < range.end)
        {
            અચલ int64 o = it / nsplit, s = it - o*nsplit;
            જો (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            અચલ uchar* base = pl.src + ko.soff;
            અચલ A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.ગણતરી_2/nsplit, b = (s + 1)*pl.ગણતરી_2/nsplit;
            A acc = Op::init();
            જો (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                જ્યારે (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](અચલ A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            જો (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.ગણતરી_2, m);
                ko.next();
            }
            નહીંતર
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    જો (w.serial)
        body(Range(0, (પૂર્ણાંક)w.nitems));
    નહીંતર
        parallel_for_(Range(0, (પૂર્ણાંક)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, વર્ગ Op, વર્ગ OpM> સ્થિર શૂન્ય runVertical(અચલ Plan& pl, અચલ Work& w, A* res,
                                                                 અચલ A* mbuf, A* partial)
{
    અચલ પૂર્ણાંક nk = pl.nk;
    અચલ int64 m = pl.ksize[nk - 1];
    અચલ ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    અચલ int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    અચલ int64 nsplit = w.nsplit;
    int64 rcount = 1;
    માટે (પૂર્ણાંક i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](અચલ Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        માટે (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            અચલ int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            અચલ ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            અચલ A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            માટે (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            જો (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                માટે (; a < b; a++, ro.next())
                {
                    અચલ uchar* p = pl.src + soff + ro.soff;
                    અચલ A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            જો (nsplit == 1)
                માટે (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.ગણતરી_2, mrow ? mrow[i] : A(0));
            નહીંતર
                માટે (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    જો (w.serial)
        body(Range(0, (પૂર્ણાંક)w.nitems));
    નહીંતર
        parallel_for_(Range(0, (પૂર્ણાંક)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, વર્ગ Op> સંરચના Plain
{
    સ્થિર A run(A acc, અચલ A* x, int64 n, A) { પાછા HRow<Op, A>::run(acc, x, n); }
    સ્થિર શૂન્ય vrun(A* acc, અચલ A* x, int64 n, અચલ A*) { VRow<Op, A>::run(acc, x, n); }
    સ્થિર A finalize(A a, int64 ગણતરી_2, A) { પાછા Op::finalize(a, ગણતરી_2); }
};

template<typename A> સંરચના ShiftedExp
{
    પ્રકારવ્યાખ્યા OpSumExpShifted<A> Op;
    સ્થિર A run(A acc, અચલ A* x, int64 n, A m)
    {
        માટે (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        પાછા acc;
    }
    સ્થિર શૂન્ય vrun(A* acc, અચલ A* x, int64 n, અચલ A* m)
    {
        માટે (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    સ્થિર A finalize(A a, int64, A m) { પાછા Op::finalize(a, m); }
};

template<typename A, વર્ગ Op, વર્ગ OpM> સ્થિર શૂન્ય runPass(અચલ Plan& pl, A* res, અચલ A* mbuf)
{
    અચલ દ્વિગુણ work = (દ્વિગુણ)pl.nout*(દ્વિગુણ)std::max(pl.ગણતરી_2, (int64)1);
    અચલ પૂર્ણાંક nthreads = work < RND_SERIAL_WORK ? 1 : get_અંક_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 વસ્તુઓ;
    જો (pl.innerReduced)
        વસ્તુઓ = pl.nout;
    નહીંતર
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        વસ્તુઓ = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    અચલ int64 want = 4*(int64)nthreads;
    જો (!w.serial && વસ્તુઓ < want && pl.ગણતરી_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.ગણતરી_2/4096 : pl.ગણતરી_2/16;
        w.nsplit = std::max((int64)1, std::min((want + વસ્તુઓ - 1)/વસ્તુઓ, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = વસ્તુઓ*w.nsplit;
    w.nstripes = std::min((દ્વિગુણ)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    જો (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    નહીંતર
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    જો (w.nsplit > 1)
    {
        માટે (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            માટે (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.ગણતરી_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> સ્થિર શૂન્ય fillResult(A* res, int64 n, A v)
{
    માટે (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> સ્થિર શૂન્ય runOp(અચલ Plan& pl, પૂર્ણાંક op, A* res)
{
    જો (pl.ગણતરી_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        પાછા;
    }
    પસંદગી (op)
    {
    સ્થિતિ REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); તોડો;
    સ્થિતિ REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); તોડો;
    સ્થિતિ REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); તોડો;
    સ્થિતિ REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); તોડો;
    સ્થિતિ REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); તોડો;
    સ્થિતિ REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); તોડો;
    સ્થિતિ REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); તોડો;
    સ્થિતિ REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        તોડો;
    }
    મૂળભૂત:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> સ્થિર શૂન્ય runMinMax(અચલ Plan& pl, પૂર્ણાંક op, A* res)
{
    જો (pl.ગણતરી_2 == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    નહીંતર જો (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    નહીંતર
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

સ્થિર શૂન્ય dispatch(અચલ Plan& pl, પૂર્ણાંક op, uchar* res)
{
    અચલ તાર્કિક minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    પસંદગી (pl.adepth)
    {
    સ્થિતિ CV_32F:
        જો (minmax) runMinMax<દશાંશ>(pl, op, (દશાંશ*)res); નહીંતર runOp<દશાંશ>(pl, op, (દશાંશ*)res);
        તોડો;
    સ્થિતિ CV_64F:
        જો (minmax) runMinMax<દ્વિગુણ>(pl, op, (દ્વિગુણ*)res); નહીંતર runOp<દ્વિગુણ>(pl, op, (દ્વિગુણ*)res);
        તોડો;
    સ્થિતિ CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); તોડો;
    સ્થિતિ CV_8S:  runMinMax<schar>(pl, op, (schar*)res); તોડો;
    સ્થિતિ CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); તોડો;
    સ્થિતિ CV_16S: runMinMax<short>(pl, op, (short*)res); તોડો;
    સ્થિતિ CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); તોડો;
    સ્થિતિ CV_32S: runMinMax<પૂર્ણાંક>(pl, op, (પૂર્ણાંક*)res); તોડો;
    સ્થિતિ CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); તોડો;
    સ્થિતિ CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); તોડો;
    મૂળભૂત:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

સ્થિર પૂર્ણાંક accumulatorDepth(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth, પૂર્ણાંક op)
{
    જો (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        જો (sdepth == CV_16F || sdepth == CV_16BF)
            પાછા CV_32F;
        પાછા sdepth == CV_Bool ? CV_8U : sdepth;
    }
    અચલ તાર્કિક wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    પાછા wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(પૂર્ણાંક _rtype, અચલ std::vector<પૂર્ણાંક>& _axes, તાર્કિક _keepdims, પૂર્ણાંક _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

શૂન્ય reduce(આગત_Array _src, OutputArray _dst, અચલ ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    અચલ પૂર્ણાંક op = params.rtype, dtype = params.dtype;
    અચલ તાર્કિક keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    અચલ પૂર્ણાંક dims = src.dims, cn = src.channels();
    અચલ પૂર્ણાંક sdepth = src.depth();
    પૂર્ણાંક ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    જો ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    તાર્કિક reduced[CV_MAX_DIM] = {ખોટું};
    જો (params.axes.empty())
        std::fill(reduced, reduced + dims, સાચું);
    માટે (પૂર્ણાંક a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        પૂર્ણાંક ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = સાચું;
    }

    પૂર્ણાંક keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    માટે (પૂર્ણાંક i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        જો (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    અચલ પૂર્ણાંક dtype_ = CV_MAKETYPE(ddepth, cn);
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
    પૂર્ણાંક n = 0;
    પૂર્ણાંક size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    તાર્કિક red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    માટે (પૂર્ણાંક i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.ગણતરી_2 = 1;
    માટે (પૂર્ણાંક i = 0; i < dims; i++)
    {
        જો (reduced[i])
            pl.ગણતરી_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    જો (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = ખોટું;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    અચલ તાર્કિક direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    જો (!direct)
    {
        resMat.create(1, (પૂર્ણાંક)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    જો (pl.nout == 0)
        પાછા;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    પૂર્ણાંક m = 0;
    માટે (પૂર્ણાંક i = 0; i < n; i++)
    {
        જો (size[i] == 0)
        {
            // empty source along this axis
            જો (red[i])
            {
                pl.ગણતરી_2 = 0;
                ચાલુ;
            }
            પાછા;
        }
        જો (size[i] == 1)
            ચાલુ;
        જો (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        નહીંતર
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    જો (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = ખોટું;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    માટે (પૂર્ણાંક i = 0; i < m; i++)
    {
        જો (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        નહીંતર
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    જો (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    જો (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        જો (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
