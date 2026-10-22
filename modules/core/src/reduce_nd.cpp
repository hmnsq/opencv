// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencv2/core/hal/intrin.hpp"

#అన్తర్భావ <algorithm>
#అన్తర్భావ <cmath>
#అన్తర్భావ <limits>

నామాకాశ cv {

నామాకాశ {

గణన
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_ఖండం = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

మూస<రకం_పేరు T> నిర్మాణం VecT { గణన { ok = 0 }; రకనిర్వచనం T vtype; };
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
#నిర్వచించు CV_RND_VEC(T, V) మూస<> నిర్మాణం VecT<T> { గణన { ok = 1 }; రకనిర్వచనం V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(పొట్టి, v_int16)
CV_RND_VEC(గుర్తులేని, v_uint32)
CV_RND_VEC(పూర్ణాంకం, v_int32)
CV_RND_VEC(దశాంశం, v_float32)
#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(ద్విగుణ, v_float64)
#అయితే_ముగింపు
#నిర్వచనం_రద్దు CV_RND_VEC
#అయితే_ముగింపు

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

మూస<రకం_పేరు A> నిర్మాణం Op_కూడిక
{
    స్థిర A init() { తిరిగి A(0); }
    స్థిర A apply(A a, A x) { తిరిగి a + x; }
    స్థిర A combine(A a, A b) { తిరిగి a + b; }
    స్థిర A finalize(A a, int64) { తిరిగి a; }
    మూస<రకం_పేరు V> స్థిర V vapply(స్థిరరాశి V& a, స్థిరరాశి V& x) { తిరిగి v_కూడు(a, x); }
    మూస<రకం_పేరు V> స్థిర V vcombine(స్థిరరాశి V& a, స్థిరరాశి V& b) { తిరిగి v_కూడు(a, b); }
};

మూస<రకం_పేరు A> నిర్మాణం OpAvg : Op_కూడిక<A>
{
    స్థిర A finalize(A a, int64 n) { తిరిగి n > 0 ? A(a/(ద్విగుణ)n) : A(0); }
};

మూస<రకం_పేరు A> నిర్మాణం Op_Log_కూడిక : Op_కూడిక<A>
{
    స్థిర A finalize(A a, int64) { తిరిగి A(std::సంవర్గమానం(a)); }
};

మూస<రకం_పేరు A> నిర్మాణం OpSum2 : Op_కూడిక<A>
{
    స్థిర A apply(A a, A x) { తిరిగి a + x*x; }
    మూస<రకం_పేరు V> స్థిర V vapply(స్థిరరాశి V& a, స్థిరరాశి V& x) { తిరిగి v_కూడు(a, v_mul(x, x)); }
};

మూస<రకం_పేరు A> నిర్మాణం OpL2 : OpSum2<A>
{
    స్థిర A finalize(A a, int64) { తిరిగి A(std::వర్గమూలం(a)); }
};

మూస<రకం_పేరు A> నిర్మాణం OpL1 : Op_కూడిక<A>
{
    స్థిర A apply(A a, A x) { తిరిగి a + (x < A(0) ? -x : x); }
    మూస<రకం_పేరు V> స్థిర V vapply(స్థిరరాశి V& a, స్థిరరాశి V& x) { తిరిగి v_కూడు(a, v_abs(x)); }
};

మూస<రకం_పేరు A> నిర్మాణం OpProd
{
    స్థిర A init() { తిరిగి A(1); }
    స్థిర A apply(A a, A x) { తిరిగి a*x; }
    స్థిర A combine(A a, A b) { తిరిగి a*b; }
    స్థిర A finalize(A a, int64) { తిరిగి a; }
    మూస<రకం_పేరు V> స్థిర V vapply(స్థిరరాశి V& a, స్థిరరాశి V& x) { తిరిగి v_mul(a, x); }
    మూస<రకం_పేరు V> స్థిర V vcombine(స్థిరరాశి V& a, స్థిరరాశి V& b) { తిరిగి v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
మూస<రకం_పేరు A, తార్కిక hasInf = std::numeric_limits<A>::has_infinity> నిర్మాణం Extremes
{
    స్థిర A దిగువ() { తిరిగి std::numeric_limits<A>::lowest(); }
    స్థిర A ఎగువ() { తిరిగి std::numeric_limits<A>::max(); }
};

మూస<రకం_పేరు A> నిర్మాణం Extremes<A, నిజం>
{
    స్థిర A దిగువ() { తిరిగి -std::numeric_limits<A>::infinity(); }
    స్థిర A ఎగువ() { తిరిగి std::numeric_limits<A>::infinity(); }
};

మూస<రకం_పేరు A> నిర్మాణం OpMax
{
    స్థిర A init() { తిరిగి Extremes<A>::దిగువ(); }
    స్థిర A apply(A a, A x) { తిరిగి a < x ? x : a; }
    స్థిర A combine(A a, A b) { తిరిగి a < b ? b : a; }
    స్థిర A finalize(A a, int64) { తిరిగి a; }
    మూస<రకం_పేరు V> స్థిర V vapply(స్థిరరాశి V& a, స్థిరరాశి V& x) { తిరిగి v_max(a, x); }
    మూస<రకం_పేరు V> స్థిర V vcombine(స్థిరరాశి V& a, స్థిరరాశి V& b) { తిరిగి v_max(a, b); }
};

మూస<రకం_పేరు A> నిర్మాణం OpMin
{
    స్థిర A init() { తిరిగి Extremes<A>::ఎగువ(); }
    స్థిర A apply(A a, A x) { తిరిగి x < a ? x : a; }
    స్థిర A combine(A a, A b) { తిరిగి b < a ? b : a; }
    స్థిర A finalize(A a, int64) { తిరిగి a; }
    మూస<రకం_పేరు V> స్థిర V vapply(స్థిరరాశి V& a, స్థిరరాశి V& x) { తిరిగి v_min(a, x); }
    మూస<రకం_పేరు V> స్థిర V vcombine(స్థిరరాశి V& a, స్థిరరాశి V& b) { తిరిగి v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
మూస<రకం_పేరు A> నిర్మాణం Op_కూడిక_Exp_Shifted
{
    స్థిర A init() { తిరిగి A(0); }
    స్థిర A apply(A a, A x, A m) { తిరిగి a + A(std::ఘాతాంకం(x - m)); }
    స్థిర A combine(A a, A b) { తిరిగి a + b; }
    స్థిర A finalize(A a, A m) { తిరిగి std::isinf(m) ? m : m + A(std::సంవర్గమానం(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
మూస<తరగతి Op, రకం_పేరు A, తార్కిక vec = (VecT<A>::ok != 0)> నిర్మాణం HRow
{
    స్థిర A నిరంతరం(A acc, స్థిరరాశి A* x, int64 n)
    {
        కొరకు (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        తిరిగి acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
మూస<తరగతి Op, రకం_పేరు A, తార్కిక vec = (VecT<A>::ok != 0)> నిర్మాణం VRow
{
    స్థిర శూన్య నిరంతరం(A* acc, స్థిరరాశి A* x, int64 n)
    {
        కొరకు (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
మూస<తరగతి Op, రకం_పేరు A> నిర్మాణం HRow<Op, A, నిజం>
{
    స్థిర A నిరంతరం(A acc, స్థిరరాశి A* x, int64 n)
    {
        రకనిర్వచనం రకం_పేరు VecT<A>::vtype V;
        స్థిరరాశి పూర్ణాంకం L = VTraits<V>::vlanes();
        int64 i = 0;
        ఒకవేళ (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            కొరకు (పూర్ణాంకం k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_ఎక్కించు(buf), a1 = a0;
            కొరకు (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_ఎక్కించు(x + i));
                a1 = Op::vapply(a1, vx_ఎక్కించు(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            కొరకు (పూర్ణాంకం k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        కొరకు (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        తిరిగి acc;
    }
};

మూస<తరగతి Op, రకం_పేరు A> నిర్మాణం VRow<Op, A, నిజం>
{
    స్థిర శూన్య నిరంతరం(A* acc, స్థిరరాశి A* x, int64 n)
    {
        రకనిర్వచనం రకం_పేరు VecT<A>::vtype V;
        స్థిరరాశి పూర్ణాంకం L = VTraits<V>::vlanes();
        int64 i = 0;
        కొరకు (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_ఎక్కించు(acc + i), vx_ఎక్కించు(x + i)));
        కొరకు (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#అయితే_ముగింపు

//////////////////////////////// plan ////////////////////////////////

నిర్మాణం Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    పూర్ణాంకం nk;
    పూర్ణాంకం ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    పూర్ణాంకం nr;
    పూర్ణాంకం rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    తార్కిక innerReduced;      // the innermost axis is reduced (horizontal mode)

    స్థిరరాశి uchar* src;
    పూర్ణాంకం sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 లెక్క_2;            // reduced elements per output
    int64 nout;             // number of outputs
    ద్విమాన_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
నిర్మాణం Iter
{
    పూర్ణాంకం n = 0;
    స్థిరరాశి పూర్ణాంకం* size = శూన్యసూచిక;
    స్థిరరాశి ptrdiff_t* s = శూన్యసూచిక;
    స్థిరరాశి ptrdiff_t* d = శూన్యసూచిక;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    శూన్య init(పూర్ణాంకం n_, స్థిరరాశి పూర్ణాంకం* కొలత_3, స్థిరరాశి ptrdiff_t* s_, స్థిరరాశి ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = కొలత_3; s = s_; d = d_;
        soff = doff = 0;
        కొరకు (పూర్ణాంకం k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            ఒకవేళ (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    శూన్య next()
    {
        కొరకు (పూర్ణాంకం k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            ఒకవేళ (d)
                doff += d[k];
            ఒకవేళ (++idx[k] < size[k])
                తిరిగి;
            soff -= (ptrdiff_t)size[k]*s[k];
            ఒకవేళ (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
మూస<రకం_పేరు A> స్థిర స్థిరరాశి A* ఎక్కించు_అడ్డువరుస(స్థిరరాశి Plan& pl, స్థిరరాశి uchar* p, ptrdiff_t అడుగు, int64 len,
                                             A* buf, uchar* tmp)
{
    ఒకవేళ (అడుగు != (ptrdiff_t)pl.sesz1)
    {
        కొరకు (int64 i = 0; i < len; i++, p += అడుగు)
            జ్ఞాపకం_నకలు(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    ఒకవేళ (!pl.cvt)
        తిరిగి (స్థిరరాశి A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, కొలత((పూర్ణాంకం)len, 1), 0);
    తిరిగి buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
మూస<రకం_పేరు A, రకం_పేరు F> స్థిర శూన్య for_అడ్డువరుస(స్థిరరాశి Plan& pl, స్థిరరాశి uchar* p, ptrdiff_t అడుగు,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    స్థిరరాశి తార్కిక ప్రత్యక్ష = !pl.cvt && అడుగు == (ptrdiff_t)pl.sesz1;
    స్థిరరాశి int64 blk = ప్రత్యక్ష ? n : (int64)RND_ఖండం;
    కొరకు (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(ఎక్కించు_అడ్డువరుస<A>(pl, p + e*అడుగు, అడుగు, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
నిర్మాణం Work
{
    int64 nitems, nsplit;
    ద్విగుణ nstripes;
    తార్కిక serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
మూస<రకం_పేరు A, తరగతి Op, తరగతి OpM> స్థిర శూన్య నిరంతరం_Horizontal(స్థిరరాశి Plan& pl, స్థిరరాశి Work& w, A* res,
                                                                   స్థిరరాశి A* mbuf, A* partial)
{
    స్థిరరాశి పూర్ణాంకం nr = pl.nr;
    స్థిరరాశి int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    స్థిరరాశి ptrdiff_t rstep = pl.rsstep[nr - 1];
    స్థిరరాశి int64 nsplit = w.nsplit;

    స్వయంచాలక body = [&](స్థిరరాశి Range& range)
    {
        Auto_బఫరు<A> bufA(RND_ఖండం);
        Auto_బఫరు<uchar> bufT(RND_ఖండం*pl.sesz1);
        Iter ko, ro;
        int64 it = range.ప్రారంభం;
        ఒకవేళ (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        అయితే (it < range.end)
        {
            స్థిరరాశి int64 o = it / nsplit, s = it - o*nsplit;
            ఒకవేళ (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            స్థిరరాశి uchar* పునాది = pl.src + ko.soff;
            స్థిరరాశి A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.లెక్క_2/nsplit, b = (s + 1)*pl.లెక్క_2/nsplit;
            A acc = Op::init();
            ఒకవేళ (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, శూన్యసూచిక, a / n);
                int64 e = a % n;
                అయితే (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    for_అడ్డువరుస<A>(pl, పునాది + ro.soff + e*rstep, rstep, len, bufA.దత్తాంశం(), bufT.దత్తాంశం(),
                              [&](స్థిరరాశి A* x, int64 l) { acc = OpM::నిరంతరం(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            ఒకవేళ (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.లెక్క_2, m);
                ko.next();
            }
            లేకపోతే
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    ఒకవేళ (w.serial)
        body(Range(0, (పూర్ణాంకం)w.nitems));
    లేకపోతే
        parallel_for_(Range(0, (పూర్ణాంకం)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
మూస<రకం_పేరు A, తరగతి Op, తరగతి OpM> స్థిర శూన్య నిరంతరం_Vertical(స్థిరరాశి Plan& pl, స్థిరరాశి Work& w, A* res,
                                                                 స్థిరరాశి A* mbuf, A* partial)
{
    స్థిరరాశి పూర్ణాంకం nk = pl.nk;
    స్థిరరాశి int64 m = pl.ksize[nk - 1];
    స్థిరరాశి ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    స్థిరరాశి int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    స్థిరరాశి int64 nsplit = w.nsplit;
    int64 rcount = 1;
    కొరకు (పూర్ణాంకం i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    స్వయంచాలక body = [&](స్థిరరాశి Range& range)
    {
        Auto_బఫరు<A> bufA(RND_ఖండం), accBuf(RND_VCHUNK);
        Auto_బఫరు<uchar> bufT(RND_ఖండం*pl.sesz1);
        A* acc = accBuf.దత్తాంశం();
        Iter ko, ro;
        కొరకు (int64 it = range.ప్రారంభం; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            స్థిరరాశి int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            స్థిరరాశి ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            స్థిరరాశి A* mrow = mbuf ? mbuf + doff : శూన్యసూచిక;   // kdstep == 1 for the innermost kept axis
            కొరకు (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            ఒకవేళ (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, శూన్యసూచిక, a);
                కొరకు (; a < b; a++, ro.next())
                {
                    స్థిరరాశి uchar* p = pl.src + soff + ro.soff;
                    స్థిరరాశి A* x = ఎక్కించు_అడ్డువరుస<A>(pl, p, kstep, len, bufA.దత్తాంశం(), bufT.దత్తాంశం());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            ఒకవేళ (nsplit == 1)
                కొరకు (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.లెక్క_2, mrow ? mrow[i] : A(0));
            లేకపోతే
                కొరకు (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    ఒకవేళ (w.serial)
        body(Range(0, (పూర్ణాంకం)w.nitems));
    లేకపోతే
        parallel_for_(Range(0, (పూర్ణాంకం)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
మూస<రకం_పేరు A, తరగతి Op> నిర్మాణం Plain
{
    స్థిర A నిరంతరం(A acc, స్థిరరాశి A* x, int64 n, A) { తిరిగి HRow<Op, A>::నిరంతరం(acc, x, n); }
    స్థిర శూన్య vrun(A* acc, స్థిరరాశి A* x, int64 n, స్థిరరాశి A*) { VRow<Op, A>::నిరంతరం(acc, x, n); }
    స్థిర A finalize(A a, int64 లెక్క_2, A) { తిరిగి Op::finalize(a, లెక్క_2); }
};

మూస<రకం_పేరు A> నిర్మాణం ShiftedExp
{
    రకనిర్వచనం Op_కూడిక_Exp_Shifted<A> Op;
    స్థిర A నిరంతరం(A acc, స్థిరరాశి A* x, int64 n, A m)
    {
        కొరకు (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        తిరిగి acc;
    }
    స్థిర శూన్య vrun(A* acc, స్థిరరాశి A* x, int64 n, స్థిరరాశి A* m)
    {
        కొరకు (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    స్థిర A finalize(A a, int64, A m) { తిరిగి Op::finalize(a, m); }
};

మూస<రకం_పేరు A, తరగతి Op, తరగతి OpM> స్థిర శూన్య నిరంతరం_Pass(స్థిరరాశి Plan& pl, A* res, స్థిరరాశి A* mbuf)
{
    స్థిరరాశి ద్విగుణ work = (ద్విగుణ)pl.nout*(ద్విగుణ)std::max(pl.లెక్క_2, (int64)1);
    స్థిరరాశి పూర్ణాంకం nthreads = work < RND_SERIAL_WORK ? 1 : get_అంకె_దారాలు();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 వస్తువులు;
    ఒకవేళ (pl.innerReduced)
        వస్తువులు = pl.nout;
    లేకపోతే
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        వస్తువులు = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    స్థిరరాశి int64 want = 4*(int64)nthreads;
    ఒకవేళ (!w.serial && వస్తువులు < want && pl.లెక్క_2 > 1)
    {
        int64 అడ్డువరుసలు = pl.innerReduced ? pl.లెక్క_2/4096 : pl.లెక్క_2/16;
        w.nsplit = std::max((int64)1, std::min((want + వస్తువులు - 1)/వస్తువులు, std::min(అడ్డువరుసలు, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = వస్తువులు*w.nsplit;
    w.nstripes = std::min((ద్విగుణ)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= గరిష్ఠ_పూర్ణాంకం);

    Auto_బఫరు<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    ఒకవేళ (pl.innerReduced)
        నిరంతరం_Horizontal<A, Op, OpM>(pl, w, res, mbuf, partial.దత్తాంశం());
    లేకపోతే
        నిరంతరం_Vertical<A, Op, OpM>(pl, w, res, mbuf, partial.దత్తాంశం());

    ఒకవేళ (w.nsplit > 1)
    {
        కొరకు (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            కొరకు (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.లెక్క_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

మూస<రకం_పేరు A> స్థిర శూన్య fill_ఫలితం(A* res, int64 n, A v)
{
    కొరకు (int64 i = 0; i < n; i++)
        res[i] = v;
}

మూస<రకం_పేరు A> స్థిర శూన్య నిరంతరం_Op(స్థిరరాశి Plan& pl, పూర్ణాంకం op, A* res)
{
    ఒకవేళ (pl.లెక్క_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_కూడిక || op == REDUCE_LOG_కూడిక_EXP ? OpMax<A>::init() : A(0);
        fill_ఫలితం(res, pl.nout, v);
        తిరిగి;
    }
    ఎంపిక (op)
    {
    స్థితి REDUCE_కూడిక:   నిరంతరం_Pass<A, Op_కూడిక<A>, Plain<A, Op_కూడిక<A> > >(pl, res, శూన్యసూచిక); ఆపు;
    స్థితి REDUCE_AVG:   నిరంతరం_Pass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, శూన్యసూచిక); ఆపు;
    స్థితి REDUCE_SUM2:  నిరంతరం_Pass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, శూన్యసూచిక); ఆపు;
    స్థితి REDUCE_L1:    నిరంతరం_Pass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, శూన్యసూచిక); ఆపు;
    స్థితి REDUCE_L2:    నిరంతరం_Pass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, శూన్యసూచిక); ఆపు;
    స్థితి REDUCE_PROD:  నిరంతరం_Pass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, శూన్యసూచిక); ఆపు;
    స్థితి REDUCE_LOG_కూడిక: నిరంతరం_Pass<A, Op_Log_కూడిక<A>, Plain<A, Op_Log_కూడిక<A> > >(pl, res, శూన్యసూచిక); ఆపు;
    స్థితి REDUCE_LOG_కూడిక_EXP:
    {
        Auto_బఫరు<A> mbuf((size_t)pl.nout);
        నిరంతరం_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.దత్తాంశం(), శూన్యసూచిక);
        నిరంతరం_Pass<A, Op_కూడిక_Exp_Shifted<A>, ShiftedExp<A> >(pl, res, mbuf.దత్తాంశం());
        ఆపు;
    }
    అప్రమేయ:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

మూస<రకం_పేరు A> స్థిర శూన్య నిరంతరం_Min_Max(స్థిరరాశి Plan& pl, పూర్ణాంకం op, A* res)
{
    ఒకవేళ (pl.లెక్క_2 == 0)
        fill_ఫలితం(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    లేకపోతే ఒకవేళ (op == REDUCE_MAX)
        నిరంతరం_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, శూన్యసూచిక);
    లేకపోతే
        నిరంతరం_Pass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, శూన్యసూచిక);
}

స్థిర శూన్య dispatch(స్థిరరాశి Plan& pl, పూర్ణాంకం op, uchar* res)
{
    స్థిరరాశి తార్కిక minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    ఎంపిక (pl.adepth)
    {
    స్థితి CV_32F:
        ఒకవేళ (minmax) నిరంతరం_Min_Max<దశాంశం>(pl, op, (దశాంశం*)res); లేకపోతే నిరంతరం_Op<దశాంశం>(pl, op, (దశాంశం*)res);
        ఆపు;
    స్థితి CV_64F:
        ఒకవేళ (minmax) నిరంతరం_Min_Max<ద్విగుణ>(pl, op, (ద్విగుణ*)res); లేకపోతే నిరంతరం_Op<ద్విగుణ>(pl, op, (ద్విగుణ*)res);
        ఆపు;
    స్థితి CV_8U:  నిరంతరం_Min_Max<uchar>(pl, op, (uchar*)res); ఆపు;
    స్థితి CV_8S:  నిరంతరం_Min_Max<schar>(pl, op, (schar*)res); ఆపు;
    స్థితి CV_16U: నిరంతరం_Min_Max<ushort>(pl, op, (ushort*)res); ఆపు;
    స్థితి CV_16S: నిరంతరం_Min_Max<పొట్టి>(pl, op, (పొట్టి*)res); ఆపు;
    స్థితి CV_32U: నిరంతరం_Min_Max<గుర్తులేని>(pl, op, (గుర్తులేని*)res); ఆపు;
    స్థితి CV_32S: నిరంతరం_Min_Max<పూర్ణాంకం>(pl, op, (పూర్ణాంకం*)res); ఆపు;
    స్థితి CV_64U: నిరంతరం_Min_Max<uint64_t>(pl, op, (uint64_t*)res); ఆపు;
    స్థితి CV_64S: నిరంతరం_Min_Max<int64_t>(pl, op, (int64_t*)res); ఆపు;
    అప్రమేయ:
        CV_Error(Error::Sts_అంతర్గత, "reduce: unexpected accumulator depth");
    }
}

స్థిర పూర్ణాంకం సంచాయకం_లోతు(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth, పూర్ణాంకం op)
{
    ఒకవేళ (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        ఒకవేళ (sdepth == CV_16F || sdepth == CV_16BF)
            తిరిగి CV_32F;
        తిరిగి sdepth == CV_Bool ? CV_8U : sdepth;
    }
    స్థిరరాశి తార్కిక wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    తిరిగి wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(పూర్ణాంకం _rtype, స్థిరరాశి std::vector<పూర్ణాంకం>& _axes, తార్కిక _keepdims, పూర్ణాంకం _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

శూన్య reduce(ఆదానం_Array _src, OutputArray _dst, స్థిరరాశి ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    స్థిరరాశి పూర్ణాంకం op = params.rtype, dtype = params.dtype;
    స్థిరరాశి తార్కిక keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_కూడిక && op <= REDUCE_LOG_కూడిక_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    స్థిరరాశి పూర్ణాంకం dims = src.dims, cn = src.channels();
    స్థిరరాశి పూర్ణాంకం sdepth = src.లోతు();
    పూర్ణాంకం ddepth = dtype < 0 ? sdepth : CV_MAT_లోతు(dtype);
    ఒకవేళ ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    తార్కిక reduced[CV_MAX_DIM] = {అబద్ధం};
    ఒకవేళ (params.axes.empty())
        std::fill(reduced, reduced + dims, నిజం);
    కొరకు (పూర్ణాంకం a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        పూర్ణాంకం ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = నిజం;
    }

    పూర్ణాంకం keepShape[CV_MAX_DIM], బహిర్గతం_Shape[CV_MAX_DIM], nout = 0;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        ఒకవేళ (!reduced[i] || keepdims)
            బహిర్గతం_Shape[nout++] = keepShape[i];
    }
    స్థిరరాశి పూర్ణాంకం dtype_ = CV_MAKETYPE(ddepth, cn);
    _dst.create(nout, బహిర్గతం_Shape, dtype_);
    Mat dst = _dst.getMat();

    // The results are computed into a contiguous accumulator-typed buffer laid out like the
    // keepdims output (channels innermost), then converted into dst if needed.
    Plan pl;
    pl.src = src.దత్తాంశం;
    pl.sdepth = sdepth == CV_Bool ? CV_8U : sdepth;
    pl.adepth = సంచాయకం_లోతు(sdepth, ddepth, op);
    pl.sesz1 = CV_ELEM_SIZE1(sdepth);
    pl.cvt = pl.sdepth == pl.adepth ? శూన్యసూచిక : getConvertFunc(pl.sdepth, pl.adepth);
    CV_Assert(pl.sdepth == pl.adepth || pl.cvt);

    // gather the axes (+ the channel axis), outer first
    పూర్ణాంకం n = 0;
    పూర్ణాంకం size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    తార్కిక red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    కొరకు (పూర్ణాంకం i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.లెక్క_2 = 1;
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
    {
        ఒకవేళ (reduced[i])
            pl.లెక్క_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.అడుగు[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    ఒకవేళ (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = అబద్ధం;
        n++;
    }

    pl.nout = (int64)dst.మొత్తం()*cn;
    స్థిరరాశి తార్కిక ప్రత్యక్ష = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.దత్తాంశం;
    ఒకవేళ (!ప్రత్యక్ష)
    {
        resMat.create(1, (పూర్ణాంకం)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.దత్తాంశం;
    }
    ఒకవేళ (pl.nout == 0)
        తిరిగి;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    పూర్ణాంకం m = 0;
    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
    {
        ఒకవేళ (size[i] == 0)
        {
            // empty source along this axis
            ఒకవేళ (red[i])
            {
                pl.లెక్క_2 = 0;
                కొనసాగు;
            }
            తిరిగి;
        }
        ఒకవేళ (size[i] == 1)
            కొనసాగు;
        ఒకవేళ (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        లేకపోతే
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    ఒకవేళ (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = అబద్ధం;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    కొరకు (పూర్ణాంకం i = 0; i < m; i++)
    {
        ఒకవేళ (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        లేకపోతే
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    ఒకవేళ (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    ఒకవేళ (!ప్రత్యక్ష)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.దత్తాంశం);
        ఒకవేళ (dst.dims == 0)
            r = Mat(0, శూన్యసూచిక, CV_MAKETYPE(pl.adepth, cn), resMat.దత్తాంశం);
        r.convert_గమ్యం(dst, ddepth);
    }
}

} // namespace cv
