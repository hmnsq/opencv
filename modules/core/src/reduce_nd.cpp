// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#ضمن "precomp.hpp"
#ضمن "opencv2/core/hal/intrin.hpp"

#ضمن <algorithm>
#ضمن <cmath>
#ضمن <limits>

نطاق cv {

نطاق {

تعداد
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_بلاک = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

سانچا<قسم_ناں T> بنية VecT { تعداد { ok = 0 }; تعريفنوع T vtype; };
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
#تعریف_کرو CV_RND_VEC(T, V) سانچا<> بنية VecT<T> { تعداد { ok = 1 }; تعريفنوع V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(چھوٹا, v_int16)
CV_RND_VEC(بنانشانی, v_uint32)
CV_RND_VEC(صحيح, v_int32)
CV_RND_VEC(عائم, v_float32)
#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(مزدوج, v_float64)
#جے_انت
#تعریف_رد CV_RND_VEC
#جے_انت

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

سانچا<قسم_ناں A> بنية Op_جوڑ
{
    ساكن A init() { أرجع A(0); }
    ساكن A apply(A a, A x) { أرجع a + x; }
    ساكن A combine(A a, A b) { أرجع a + b; }
    ساكن A finalize(A a, int64) { أرجع a; }
    سانچا<قسم_ناں V> ساكن V vapply(ثابت V& a, ثابت V& x) { أرجع v_جوڑو(a, x); }
    سانچا<قسم_ناں V> ساكن V vcombine(ثابت V& a, ثابت V& b) { أرجع v_جوڑو(a, b); }
};

سانچا<قسم_ناں A> بنية OpAvg : Op_جوڑ<A>
{
    ساكن A finalize(A a, int64 n) { أرجع n > 0 ? A(a/(مزدوج)n) : A(0); }
};

سانچا<قسم_ناں A> بنية Op_Log_جوڑ : Op_جوڑ<A>
{
    ساكن A finalize(A a, int64) { أرجع A(std::لوگارتھم(a)); }
};

سانچا<قسم_ناں A> بنية OpSum2 : Op_جوڑ<A>
{
    ساكن A apply(A a, A x) { أرجع a + x*x; }
    سانچا<قسم_ناں V> ساكن V vapply(ثابت V& a, ثابت V& x) { أرجع v_جوڑو(a, v_mul(x, x)); }
};

سانچا<قسم_ناں A> بنية OpL2 : OpSum2<A>
{
    ساكن A finalize(A a, int64) { أرجع A(std::جذر(a)); }
};

سانچا<قسم_ناں A> بنية OpL1 : Op_جوڑ<A>
{
    ساكن A apply(A a, A x) { أرجع a + (x < A(0) ? -x : x); }
    سانچا<قسم_ناں V> ساكن V vapply(ثابت V& a, ثابت V& x) { أرجع v_جوڑو(a, v_abs(x)); }
};

سانچا<قسم_ناں A> بنية OpProd
{
    ساكن A init() { أرجع A(1); }
    ساكن A apply(A a, A x) { أرجع a*x; }
    ساكن A combine(A a, A b) { أرجع a*b; }
    ساكن A finalize(A a, int64) { أرجع a; }
    سانچا<قسم_ناں V> ساكن V vapply(ثابت V& a, ثابت V& x) { أرجع v_mul(a, x); }
    سانچا<قسم_ناں V> ساكن V vcombine(ثابت V& a, ثابت V& b) { أرجع v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
سانچا<قسم_ناں A, منطقي hasInf = std::numeric_limits<A>::has_infinity> بنية Extremes
{
    ساكن A کم() { أرجع std::numeric_limits<A>::lowest(); }
    ساكن A بلند() { أرجع std::numeric_limits<A>::max(); }
};

سانچا<قسم_ناں A> بنية Extremes<A, سچ>
{
    ساكن A کم() { أرجع -std::numeric_limits<A>::infinity(); }
    ساكن A بلند() { أرجع std::numeric_limits<A>::infinity(); }
};

سانچا<قسم_ناں A> بنية OpMax
{
    ساكن A init() { أرجع Extremes<A>::کم(); }
    ساكن A apply(A a, A x) { أرجع a < x ? x : a; }
    ساكن A combine(A a, A b) { أرجع a < b ? b : a; }
    ساكن A finalize(A a, int64) { أرجع a; }
    سانچا<قسم_ناں V> ساكن V vapply(ثابت V& a, ثابت V& x) { أرجع v_max(a, x); }
    سانچا<قسم_ناں V> ساكن V vcombine(ثابت V& a, ثابت V& b) { أرجع v_max(a, b); }
};

سانچا<قسم_ناں A> بنية OpMin
{
    ساكن A init() { أرجع Extremes<A>::بلند(); }
    ساكن A apply(A a, A x) { أرجع x < a ? x : a; }
    ساكن A combine(A a, A b) { أرجع b < a ? b : a; }
    ساكن A finalize(A a, int64) { أرجع a; }
    سانچا<قسم_ناں V> ساكن V vapply(ثابت V& a, ثابت V& x) { أرجع v_min(a, x); }
    سانچا<قسم_ناں V> ساكن V vcombine(ثابت V& a, ثابت V& b) { أرجع v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
سانچا<قسم_ناں A> بنية Op_جوڑ_Exp_Shifted
{
    ساكن A init() { أرجع A(0); }
    ساكن A apply(A a, A x, A m) { أرجع a + A(std::قوت_نما(x - m)); }
    ساكن A combine(A a, A b) { أرجع a + b; }
    ساكن A finalize(A a, A m) { أرجع std::isinf(m) ? m : m + A(std::لوگارتھم(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
سانچا<صنف Op, قسم_ناں A, منطقي vec = (VecT<A>::ok != 0)> بنية HRow
{
    ساكن A لگاتار(A acc, ثابت A* x, int64 n)
    {
        لكل (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        أرجع acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
سانچا<صنف Op, قسم_ناں A, منطقي vec = (VecT<A>::ok != 0)> بنية VRow
{
    ساكن فراغ لگاتار(A* acc, ثابت A* x, int64 n)
    {
        لكل (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)
سانچا<صنف Op, قسم_ناں A> بنية HRow<Op, A, سچ>
{
    ساكن A لگاتار(A acc, ثابت A* x, int64 n)
    {
        تعريفنوع قسم_ناں VecT<A>::vtype V;
        ثابت صحيح L = VTraits<V>::vlanes();
        int64 i = 0;
        إذا (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            لكل (صحيح k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_لدو(buf), a1 = a0;
            لكل (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_لدو(x + i));
                a1 = Op::vapply(a1, vx_لدو(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            لكل (صحيح k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        لكل (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        أرجع acc;
    }
};

سانچا<صنف Op, قسم_ناں A> بنية VRow<Op, A, سچ>
{
    ساكن فراغ لگاتار(A* acc, ثابت A* x, int64 n)
    {
        تعريفنوع قسم_ناں VecT<A>::vtype V;
        ثابت صحيح L = VTraits<V>::vlanes();
        int64 i = 0;
        لكل (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_لدو(acc + i), vx_لدو(x + i)));
        لكل (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#جے_انت

//////////////////////////////// plan ////////////////////////////////

بنية Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    صحيح nk;
    صحيح ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    صحيح nr;
    صحيح rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    منطقي innerReduced;      // the innermost axis is reduced (horizontal mode)

    ثابت uchar* src;
    صحيح sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 گنتی_2;            // reduced elements per output
    int64 nout;             // number of outputs
    ثنائی_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
بنية Iter
{
    صحيح n = 0;
    ثابت صحيح* size = خالی_سوچک;
    ثابت ptrdiff_t* s = خالی_سوچک;
    ثابت ptrdiff_t* d = خالی_سوچک;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    فراغ init(صحيح n_, ثابت صحيح* ناپ_3, ثابت ptrdiff_t* s_, ثابت ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = ناپ_3; s = s_; d = d_;
        soff = doff = 0;
        لكل (صحيح k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            إذا (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    فراغ next()
    {
        لكل (صحيح k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            إذا (d)
                doff += d[k];
            إذا (++idx[k] < size[k])
                أرجع;
            soff -= (ptrdiff_t)size[k]*s[k];
            إذا (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
سانچا<قسم_ناں A> ساكن ثابت A* لدو_صف(ثابت Plan& pl, ثابت uchar* p, ptrdiff_t قدم, int64 len,
                                             A* buf, uchar* tmp)
{
    إذا (قدم != (ptrdiff_t)pl.sesz1)
    {
        لكل (int64 i = 0; i < len; i++, p += قدم)
            حافظہ_نقل(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    إذا (!pl.cvt)
        أرجع (ثابت A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, ناپ((صحيح)len, 1), 0);
    أرجع buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
سانچا<قسم_ناں A, قسم_ناں F> ساكن فراغ for_صف(ثابت Plan& pl, ثابت uchar* p, ptrdiff_t قدم,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ثابت منطقي سدھا = !pl.cvt && قدم == (ptrdiff_t)pl.sesz1;
    ثابت int64 blk = سدھا ? n : (int64)RND_بلاک;
    لكل (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(لدو_صف<A>(pl, p + e*قدم, قدم, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
بنية Work
{
    int64 nitems, nsplit;
    مزدوج nstripes;
    منطقي serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
سانچا<قسم_ناں A, صنف Op, صنف OpM> ساكن فراغ لگاتار_Horizontal(ثابت Plan& pl, ثابت Work& w, A* res,
                                                                   ثابت A* mbuf, A* partial)
{
    ثابت صحيح nr = pl.nr;
    ثابت int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ثابت ptrdiff_t rstep = pl.rsstep[nr - 1];
    ثابت int64 nsplit = w.nsplit;

    خودکار body = [&](ثابت Range& range)
    {
        Auto_بفر<A> bufA(RND_بلاک);
        Auto_بفر<uchar> bufT(RND_بلاک*pl.sesz1);
        Iter ko, ro;
        int64 it = range.شروع;
        إذا (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        طالما (it < range.end)
        {
            ثابت int64 o = it / nsplit, s = it - o*nsplit;
            إذا (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ثابت uchar* ادھار = pl.src + ko.soff;
            ثابت A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.گنتی_2/nsplit, b = (s + 1)*pl.گنتی_2/nsplit;
            A acc = Op::init();
            إذا (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, خالی_سوچک, a / n);
                int64 e = a % n;
                طالما (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    for_صف<A>(pl, ادھار + ro.soff + e*rstep, rstep, len, bufA.انکڑے(), bufT.انکڑے(),
                              [&](ثابت A* x, int64 l) { acc = OpM::لگاتار(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            إذا (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.گنتی_2, m);
                ko.next();
            }
            وإلا
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    إذا (w.serial)
        body(Range(0, (صحيح)w.nitems));
    وإلا
        parallel_for_(Range(0, (صحيح)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
سانچا<قسم_ناں A, صنف Op, صنف OpM> ساكن فراغ لگاتار_Vertical(ثابت Plan& pl, ثابت Work& w, A* res,
                                                                 ثابت A* mbuf, A* partial)
{
    ثابت صحيح nk = pl.nk;
    ثابت int64 m = pl.ksize[nk - 1];
    ثابت ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ثابت int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ثابت int64 nsplit = w.nsplit;
    int64 rcount = 1;
    لكل (صحيح i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    خودکار body = [&](ثابت Range& range)
    {
        Auto_بفر<A> bufA(RND_بلاک), accBuf(RND_VCHUNK);
        Auto_بفر<uchar> bufT(RND_بلاک*pl.sesz1);
        A* acc = accBuf.انکڑے();
        Iter ko, ro;
        لكل (int64 it = range.شروع; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ثابت int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ثابت ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ثابت A* mrow = mbuf ? mbuf + doff : خالی_سوچک;   // kdstep == 1 for the innermost kept axis
            لكل (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            إذا (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, خالی_سوچک, a);
                لكل (; a < b; a++, ro.next())
                {
                    ثابت uchar* p = pl.src + soff + ro.soff;
                    ثابت A* x = لدو_صف<A>(pl, p, kstep, len, bufA.انکڑے(), bufT.انکڑے());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            إذا (nsplit == 1)
                لكل (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.گنتی_2, mrow ? mrow[i] : A(0));
            وإلا
                لكل (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    إذا (w.serial)
        body(Range(0, (صحيح)w.nitems));
    وإلا
        parallel_for_(Range(0, (صحيح)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
سانچا<قسم_ناں A, صنف Op> بنية Plain
{
    ساكن A لگاتار(A acc, ثابت A* x, int64 n, A) { أرجع HRow<Op, A>::لگاتار(acc, x, n); }
    ساكن فراغ vrun(A* acc, ثابت A* x, int64 n, ثابت A*) { VRow<Op, A>::لگاتار(acc, x, n); }
    ساكن A finalize(A a, int64 گنتی_2, A) { أرجع Op::finalize(a, گنتی_2); }
};

سانچا<قسم_ناں A> بنية ShiftedExp
{
    تعريفنوع Op_جوڑ_Exp_Shifted<A> Op;
    ساكن A لگاتار(A acc, ثابت A* x, int64 n, A m)
    {
        لكل (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        أرجع acc;
    }
    ساكن فراغ vrun(A* acc, ثابت A* x, int64 n, ثابت A* m)
    {
        لكل (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    ساكن A finalize(A a, int64, A m) { أرجع Op::finalize(a, m); }
};

سانچا<قسم_ناں A, صنف Op, صنف OpM> ساكن فراغ لگاتار_Pass(ثابت Plan& pl, A* res, ثابت A* mbuf)
{
    ثابت مزدوج work = (مزدوج)pl.nout*(مزدوج)std::max(pl.گنتی_2, (int64)1);
    ثابت صحيح nthreads = work < RND_SERIAL_WORK ? 1 : get_عدد_دھاگے();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 اشیاء;
    إذا (pl.innerReduced)
        اشیاء = pl.nout;
    وإلا
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        اشیاء = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ثابت int64 want = 4*(int64)nthreads;
    إذا (!w.serial && اشیاء < want && pl.گنتی_2 > 1)
    {
        int64 صفاں = pl.innerReduced ? pl.گنتی_2/4096 : pl.گنتی_2/16;
        w.nsplit = std::max((int64)1, std::min((want + اشیاء - 1)/اشیاء, std::min(صفاں, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = اشیاء*w.nsplit;
    w.nstripes = std::min((مزدوج)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= ودھ_توں_ودھ_عدد);

    Auto_بفر<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    إذا (pl.innerReduced)
        لگاتار_Horizontal<A, Op, OpM>(pl, w, res, mbuf, partial.انکڑے());
    وإلا
        لگاتار_Vertical<A, Op, OpM>(pl, w, res, mbuf, partial.انکڑے());

    إذا (w.nsplit > 1)
    {
        لكل (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            لكل (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.گنتی_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

سانچا<قسم_ناں A> ساكن فراغ fill_نتیجہ(A* res, int64 n, A v)
{
    لكل (int64 i = 0; i < n; i++)
        res[i] = v;
}

سانچا<قسم_ناں A> ساكن فراغ لگاتار_Op(ثابت Plan& pl, صحيح op, A* res)
{
    إذا (pl.گنتی_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_جوڑ || op == REDUCE_LOG_جوڑ_EXP ? OpMax<A>::init() : A(0);
        fill_نتیجہ(res, pl.nout, v);
        أرجع;
    }
    بدل (op)
    {
    حالة REDUCE_جوڑ:   لگاتار_Pass<A, Op_جوڑ<A>, Plain<A, Op_جوڑ<A> > >(pl, res, خالی_سوچک); اكسر;
    حالة REDUCE_AVG:   لگاتار_Pass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, خالی_سوچک); اكسر;
    حالة REDUCE_SUM2:  لگاتار_Pass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, خالی_سوچک); اكسر;
    حالة REDUCE_L1:    لگاتار_Pass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, خالی_سوچک); اكسر;
    حالة REDUCE_L2:    لگاتار_Pass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, خالی_سوچک); اكسر;
    حالة REDUCE_PROD:  لگاتار_Pass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, خالی_سوچک); اكسر;
    حالة REDUCE_LOG_جوڑ: لگاتار_Pass<A, Op_Log_جوڑ<A>, Plain<A, Op_Log_جوڑ<A> > >(pl, res, خالی_سوچک); اكسر;
    حالة REDUCE_LOG_جوڑ_EXP:
    {
        Auto_بفر<A> mbuf((size_t)pl.nout);
        لگاتار_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.انکڑے(), خالی_سوچک);
        لگاتار_Pass<A, Op_جوڑ_Exp_Shifted<A>, ShiftedExp<A> >(pl, res, mbuf.انکڑے());
        اكسر;
    }
    افتراضي:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

سانچا<قسم_ناں A> ساكن فراغ لگاتار_Min_Max(ثابت Plan& pl, صحيح op, A* res)
{
    إذا (pl.گنتی_2 == 0)
        fill_نتیجہ(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    وإلا إذا (op == REDUCE_MAX)
        لگاتار_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, خالی_سوچک);
    وإلا
        لگاتار_Pass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, خالی_سوچک);
}

ساكن فراغ dispatch(ثابت Plan& pl, صحيح op, uchar* res)
{
    ثابت منطقي minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    بدل (pl.adepth)
    {
    حالة CV_32F:
        إذا (minmax) لگاتار_Min_Max<عائم>(pl, op, (عائم*)res); وإلا لگاتار_Op<عائم>(pl, op, (عائم*)res);
        اكسر;
    حالة CV_64F:
        إذا (minmax) لگاتار_Min_Max<مزدوج>(pl, op, (مزدوج*)res); وإلا لگاتار_Op<مزدوج>(pl, op, (مزدوج*)res);
        اكسر;
    حالة CV_8U:  لگاتار_Min_Max<uchar>(pl, op, (uchar*)res); اكسر;
    حالة CV_8S:  لگاتار_Min_Max<schar>(pl, op, (schar*)res); اكسر;
    حالة CV_16U: لگاتار_Min_Max<ushort>(pl, op, (ushort*)res); اكسر;
    حالة CV_16S: لگاتار_Min_Max<چھوٹا>(pl, op, (چھوٹا*)res); اكسر;
    حالة CV_32U: لگاتار_Min_Max<بنانشانی>(pl, op, (بنانشانی*)res); اكسر;
    حالة CV_32S: لگاتار_Min_Max<صحيح>(pl, op, (صحيح*)res); اكسر;
    حالة CV_64U: لگاتار_Min_Max<uint64_t>(pl, op, (uint64_t*)res); اكسر;
    حالة CV_64S: لگاتار_Min_Max<int64_t>(pl, op, (int64_t*)res); اكسر;
    افتراضي:
        CV_Error(Error::Sts_اندرونی, "reduce: unexpected accumulator depth");
    }
}

ساكن صحيح سنچائک_ڈونگھائی(صحيح sdepth, صحيح ddepth, صحيح op)
{
    إذا (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        إذا (sdepth == CV_16F || sdepth == CV_16BF)
            أرجع CV_32F;
        أرجع sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ثابت منطقي wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    أرجع wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(صحيح _rtype, ثابت std::vector<صحيح>& _axes, منطقي _keepdims, صحيح _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

فراغ reduce(داخلہ_Array _src, OutputArray _dst, ثابت ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح op = params.rtype, dtype = params.dtype;
    ثابت منطقي keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_جوڑ && op <= REDUCE_LOG_جوڑ_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ثابت صحيح dims = src.dims, cn = src.channels();
    ثابت صحيح sdepth = src.ڈونگھائی();
    صحيح ddepth = dtype < 0 ? sdepth : CV_MAT_ڈونگھائی(dtype);
    إذا ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    منطقي reduced[CV_MAX_DIM] = {خطأ};
    إذا (params.axes.empty())
        std::fill(reduced, reduced + dims, سچ);
    لكل (صحيح a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        صحيح ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = سچ;
    }

    صحيح keepShape[CV_MAX_DIM], باہرلا_Shape[CV_MAX_DIM], nout = 0;
    لكل (صحيح i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        إذا (!reduced[i] || keepdims)
            باہرلا_Shape[nout++] = keepShape[i];
    }
    ثابت صحيح dtype_ = CV_MAKETYPE(ddepth, cn);
    _dst.create(nout, باہرلا_Shape, dtype_);
    Mat dst = _dst.getMat();

    // The results are computed into a contiguous accumulator-typed buffer laid out like the
    // keepdims output (channels innermost), then converted into dst if needed.
    Plan pl;
    pl.src = src.انکڑے;
    pl.sdepth = sdepth == CV_Bool ? CV_8U : sdepth;
    pl.adepth = سنچائک_ڈونگھائی(sdepth, ddepth, op);
    pl.sesz1 = CV_ELEM_SIZE1(sdepth);
    pl.cvt = pl.sdepth == pl.adepth ? خالی_سوچک : getConvertFunc(pl.sdepth, pl.adepth);
    CV_Assert(pl.sdepth == pl.adepth || pl.cvt);

    // gather the axes (+ the channel axis), outer first
    صحيح n = 0;
    صحيح size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    منطقي red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    لكل (صحيح i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.گنتی_2 = 1;
    لكل (صحيح i = 0; i < dims; i++)
    {
        إذا (reduced[i])
            pl.گنتی_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.قدم[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    إذا (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = خطأ;
        n++;
    }

    pl.nout = (int64)dst.کل()*cn;
    ثابت منطقي سدھا = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.انکڑے;
    إذا (!سدھا)
    {
        resMat.create(1, (صحيح)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.انکڑے;
    }
    إذا (pl.nout == 0)
        أرجع;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    صحيح m = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        إذا (size[i] == 0)
        {
            // empty source along this axis
            إذا (red[i])
            {
                pl.گنتی_2 = 0;
                استمر;
            }
            أرجع;
        }
        إذا (size[i] == 1)
            استمر;
        إذا (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        وإلا
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    إذا (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = خطأ;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    لكل (صحيح i = 0; i < m; i++)
    {
        إذا (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        وإلا
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    إذا (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    إذا (!سدھا)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.انکڑے);
        إذا (dst.dims == 0)
            r = Mat(0, خالی_سوچک, CV_MAKETYPE(pl.adepth, cn), resMat.انکڑے);
        r.convert_منزل(dst, ddepth);
    }
}

} // namespace cv
