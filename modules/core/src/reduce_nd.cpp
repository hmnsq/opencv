// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#شامل "precomp.hpp"
#شامل "opencv2/core/hal/intrin.hpp"

#شامل <algorithm>
#شامل <cmath>
#شامل <limits>

نامخلا cv {

نامخلا {

شمار
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_بلاک = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

سانچہ<قسم_نام T> ساخت VecT { شمار { ok = 0 }; قسمتعریف T vtype; };
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
#تعریف_کرو CV_RND_VEC(T, V) سانچہ<> ساخت VecT<T> { شمار { ok = 1 }; قسمتعریف V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(مختصر, v_int16)
CV_RND_VEC(غیرعلامتی, v_uint32)
CV_RND_VEC(صحیح, v_int32)
CV_RND_VEC(اعشاری, v_float32)
#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(دوہرا, v_float64)
#اگر_ختم
#تعریف_منسوخ CV_RND_VEC
#اگر_ختم

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

سانچہ<قسم_نام A> ساخت Op_مجموع
{
    ساکن A init() { واپس A(0); }
    ساکن A apply(A a, A x) { واپس a + x; }
    ساکن A combine(A a, A b) { واپس a + b; }
    ساکن A finalize(A a, int64) { واپس a; }
    سانچہ<قسم_نام V> ساکن V vapply(ثابت V& a, ثابت V& x) { واپس v_جوڑو(a, x); }
    سانچہ<قسم_نام V> ساکن V vcombine(ثابت V& a, ثابت V& b) { واپس v_جوڑو(a, b); }
};

سانچہ<قسم_نام A> ساخت OpAvg : Op_مجموع<A>
{
    ساکن A finalize(A a, int64 n) { واپس n > 0 ? A(a/(دوہرا)n) : A(0); }
};

سانچہ<قسم_نام A> ساخت Op_Log_مجموع : Op_مجموع<A>
{
    ساکن A finalize(A a, int64) { واپس A(std::لوگارتھم(a)); }
};

سانچہ<قسم_نام A> ساخت OpSum2 : Op_مجموع<A>
{
    ساکن A apply(A a, A x) { واپس a + x*x; }
    سانچہ<قسم_نام V> ساکن V vapply(ثابت V& a, ثابت V& x) { واپس v_جوڑو(a, v_mul(x, x)); }
};

سانچہ<قسم_نام A> ساخت OpL2 : OpSum2<A>
{
    ساکن A finalize(A a, int64) { واپس A(std::جذر(a)); }
};

سانچہ<قسم_نام A> ساخت OpL1 : Op_مجموع<A>
{
    ساکن A apply(A a, A x) { واپس a + (x < A(0) ? -x : x); }
    سانچہ<قسم_نام V> ساکن V vapply(ثابت V& a, ثابت V& x) { واپس v_جوڑو(a, v_abs(x)); }
};

سانچہ<قسم_نام A> ساخت OpProd
{
    ساکن A init() { واپس A(1); }
    ساکن A apply(A a, A x) { واپس a*x; }
    ساکن A combine(A a, A b) { واپس a*b; }
    ساکن A finalize(A a, int64) { واپس a; }
    سانچہ<قسم_نام V> ساکن V vapply(ثابت V& a, ثابت V& x) { واپس v_mul(a, x); }
    سانچہ<قسم_نام V> ساکن V vcombine(ثابت V& a, ثابت V& b) { واپس v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
سانچہ<قسم_نام A, منطقی hasInf = std::numeric_limits<A>::has_infinity> ساخت Extremes
{
    ساکن A کم() { واپس std::numeric_limits<A>::lowest(); }
    ساکن A بلند() { واپس std::numeric_limits<A>::max(); }
};

سانچہ<قسم_نام A> ساخت Extremes<A, سچ>
{
    ساکن A کم() { واپس -std::numeric_limits<A>::infinity(); }
    ساکن A بلند() { واپس std::numeric_limits<A>::infinity(); }
};

سانچہ<قسم_نام A> ساخت OpMax
{
    ساکن A init() { واپس Extremes<A>::کم(); }
    ساکن A apply(A a, A x) { واپس a < x ? x : a; }
    ساکن A combine(A a, A b) { واپس a < b ? b : a; }
    ساکن A finalize(A a, int64) { واپس a; }
    سانچہ<قسم_نام V> ساکن V vapply(ثابت V& a, ثابت V& x) { واپس v_max(a, x); }
    سانچہ<قسم_نام V> ساکن V vcombine(ثابت V& a, ثابت V& b) { واپس v_max(a, b); }
};

سانچہ<قسم_نام A> ساخت OpMin
{
    ساکن A init() { واپس Extremes<A>::بلند(); }
    ساکن A apply(A a, A x) { واپس x < a ? x : a; }
    ساکن A combine(A a, A b) { واپس b < a ? b : a; }
    ساکن A finalize(A a, int64) { واپس a; }
    سانچہ<قسم_نام V> ساکن V vapply(ثابت V& a, ثابت V& x) { واپس v_min(a, x); }
    سانچہ<قسم_نام V> ساکن V vcombine(ثابت V& a, ثابت V& b) { واپس v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
سانچہ<قسم_نام A> ساخت Op_مجموع_Exp_Shifted
{
    ساکن A init() { واپس A(0); }
    ساکن A apply(A a, A x, A m) { واپس a + A(std::قوت_نما(x - m)); }
    ساکن A combine(A a, A b) { واپس a + b; }
    ساکن A finalize(A a, A m) { واپس std::isinf(m) ? m : m + A(std::لوگارتھم(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
سانچہ<جماعت Op, قسم_نام A, منطقی vec = (VecT<A>::ok != 0)> ساخت HRow
{
    ساکن A تسلسل(A acc, ثابت A* x, int64 n)
    {
        برائے (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        واپس acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
سانچہ<جماعت Op, قسم_نام A, منطقی vec = (VecT<A>::ok != 0)> ساخت VRow
{
    ساکن خالی تسلسل(A* acc, ثابت A* x, int64 n)
    {
        برائے (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
سانچہ<جماعت Op, قسم_نام A> ساخت HRow<Op, A, سچ>
{
    ساکن A تسلسل(A acc, ثابت A* x, int64 n)
    {
        قسمتعریف قسم_نام VecT<A>::vtype V;
        ثابت صحیح L = VTraits<V>::vlanes();
        int64 i = 0;
        اگر (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            برائے (صحیح k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_بھرو(buf), a1 = a0;
            برائے (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_بھرو(x + i));
                a1 = Op::vapply(a1, vx_بھرو(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            برائے (صحیح k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        برائے (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        واپس acc;
    }
};

سانچہ<جماعت Op, قسم_نام A> ساخت VRow<Op, A, سچ>
{
    ساکن خالی تسلسل(A* acc, ثابت A* x, int64 n)
    {
        قسمتعریف قسم_نام VecT<A>::vtype V;
        ثابت صحیح L = VTraits<V>::vlanes();
        int64 i = 0;
        برائے (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_بھرو(acc + i), vx_بھرو(x + i)));
        برائے (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#اگر_ختم

//////////////////////////////// plan ////////////////////////////////

ساخت Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    صحیح nk;
    صحیح ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    صحیح nr;
    صحیح rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    منطقی innerReduced;      // the innermost axis is reduced (horizontal mode)

    ثابت uchar* src;
    صحیح sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 گنتی_2;            // reduced elements per output
    int64 nout;             // number of outputs
    ثنائی_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
ساخت Iter
{
    صحیح n = 0;
    ثابت صحیح* size = خالی_اشاریہ;
    ثابت ptrdiff_t* s = خالی_اشاریہ;
    ثابت ptrdiff_t* d = خالی_اشاریہ;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    خالی init(صحیح n_, ثابت صحیح* سائز_3, ثابت ptrdiff_t* s_, ثابت ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = سائز_3; s = s_; d = d_;
        soff = doff = 0;
        برائے (صحیح k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            اگر (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    خالی next()
    {
        برائے (صحیح k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            اگر (d)
                doff += d[k];
            اگر (++idx[k] < size[k])
                واپس;
            soff -= (ptrdiff_t)size[k]*s[k];
            اگر (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
سانچہ<قسم_نام A> ساکن ثابت A* بھرو_صف(ثابت Plan& pl, ثابت uchar* p, ptrdiff_t قدم, int64 len,
                                             A* buf, uchar* tmp)
{
    اگر (قدم != (ptrdiff_t)pl.sesz1)
    {
        برائے (int64 i = 0; i < len; i++, p += قدم)
            حافظہ_نقل(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    اگر (!pl.cvt)
        واپس (ثابت A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, سائز((صحیح)len, 1), 0);
    واپس buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
سانچہ<قسم_نام A, قسم_نام F> ساکن خالی for_صف(ثابت Plan& pl, ثابت uchar* p, ptrdiff_t قدم,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    ثابت منطقی براہ_راست = !pl.cvt && قدم == (ptrdiff_t)pl.sesz1;
    ثابت int64 blk = براہ_راست ? n : (int64)RND_بلاک;
    برائے (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(بھرو_صف<A>(pl, p + e*قدم, قدم, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
ساخت Work
{
    int64 nitems, nsplit;
    دوہرا nstripes;
    منطقی serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
سانچہ<قسم_نام A, جماعت Op, جماعت OpM> ساکن خالی تسلسل_Horizontal(ثابت Plan& pl, ثابت Work& w, A* res,
                                                                   ثابت A* mbuf, A* partial)
{
    ثابت صحیح nr = pl.nr;
    ثابت int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    ثابت ptrdiff_t rstep = pl.rsstep[nr - 1];
    ثابت int64 nsplit = w.nsplit;

    خودکار body = [&](ثابت Range& range)
    {
        Auto_بفر<A> bufA(RND_بلاک);
        Auto_بفر<uchar> bufT(RND_بلاک*pl.sesz1);
        Iter ko, ro;
        int64 it = range.آغاز;
        اگر (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        جب (it < range.end)
        {
            ثابت int64 o = it / nsplit, s = it - o*nsplit;
            اگر (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            ثابت uchar* بنیاد = pl.src + ko.soff;
            ثابت A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.گنتی_2/nsplit, b = (s + 1)*pl.گنتی_2/nsplit;
            A acc = Op::init();
            اگر (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, خالی_اشاریہ, a / n);
                int64 e = a % n;
                جب (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    for_صف<A>(pl, بنیاد + ro.soff + e*rstep, rstep, len, bufA.ڈیٹا(), bufT.ڈیٹا(),
                              [&](ثابت A* x, int64 l) { acc = OpM::تسلسل(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            اگر (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.گنتی_2, m);
                ko.next();
            }
            ورنہ
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    اگر (w.serial)
        body(Range(0, (صحیح)w.nitems));
    ورنہ
        parallel_for_(Range(0, (صحیح)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
سانچہ<قسم_نام A, جماعت Op, جماعت OpM> ساکن خالی تسلسل_Vertical(ثابت Plan& pl, ثابت Work& w, A* res,
                                                                 ثابت A* mbuf, A* partial)
{
    ثابت صحیح nk = pl.nk;
    ثابت int64 m = pl.ksize[nk - 1];
    ثابت ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    ثابت int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    ثابت int64 nsplit = w.nsplit;
    int64 rcount = 1;
    برائے (صحیح i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    خودکار body = [&](ثابت Range& range)
    {
        Auto_بفر<A> bufA(RND_بلاک), accBuf(RND_VCHUNK);
        Auto_بفر<uchar> bufT(RND_بلاک*pl.sesz1);
        A* acc = accBuf.ڈیٹا();
        Iter ko, ro;
        برائے (int64 it = range.آغاز; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            ثابت int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            ثابت ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            ثابت A* mrow = mbuf ? mbuf + doff : خالی_اشاریہ;   // kdstep == 1 for the innermost kept axis
            برائے (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            اگر (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, خالی_اشاریہ, a);
                برائے (; a < b; a++, ro.next())
                {
                    ثابت uchar* p = pl.src + soff + ro.soff;
                    ثابت A* x = بھرو_صف<A>(pl, p, kstep, len, bufA.ڈیٹا(), bufT.ڈیٹا());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            اگر (nsplit == 1)
                برائے (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.گنتی_2, mrow ? mrow[i] : A(0));
            ورنہ
                برائے (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    اگر (w.serial)
        body(Range(0, (صحیح)w.nitems));
    ورنہ
        parallel_for_(Range(0, (صحیح)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
سانچہ<قسم_نام A, جماعت Op> ساخت Plain
{
    ساکن A تسلسل(A acc, ثابت A* x, int64 n, A) { واپس HRow<Op, A>::تسلسل(acc, x, n); }
    ساکن خالی vrun(A* acc, ثابت A* x, int64 n, ثابت A*) { VRow<Op, A>::تسلسل(acc, x, n); }
    ساکن A finalize(A a, int64 گنتی_2, A) { واپس Op::finalize(a, گنتی_2); }
};

سانچہ<قسم_نام A> ساخت ShiftedExp
{
    قسمتعریف Op_مجموع_Exp_Shifted<A> Op;
    ساکن A تسلسل(A acc, ثابت A* x, int64 n, A m)
    {
        برائے (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        واپس acc;
    }
    ساکن خالی vrun(A* acc, ثابت A* x, int64 n, ثابت A* m)
    {
        برائے (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    ساکن A finalize(A a, int64, A m) { واپس Op::finalize(a, m); }
};

سانچہ<قسم_نام A, جماعت Op, جماعت OpM> ساکن خالی تسلسل_Pass(ثابت Plan& pl, A* res, ثابت A* mbuf)
{
    ثابت دوہرا work = (دوہرا)pl.nout*(دوہرا)std::max(pl.گنتی_2, (int64)1);
    ثابت صحیح nthreads = work < RND_SERIAL_WORK ? 1 : get_عدد_دھاگے();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 اشیاء;
    اگر (pl.innerReduced)
        اشیاء = pl.nout;
    ورنہ
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        اشیاء = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    ثابت int64 want = 4*(int64)nthreads;
    اگر (!w.serial && اشیاء < want && pl.گنتی_2 > 1)
    {
        int64 صفیں = pl.innerReduced ? pl.گنتی_2/4096 : pl.گنتی_2/16;
        w.nsplit = std::max((int64)1, std::min((want + اشیاء - 1)/اشیاء, std::min(صفیں, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = اشیاء*w.nsplit;
    w.nstripes = std::min((دوہرا)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= زیادہ_سے_زیادہ_عدد);

    Auto_بفر<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    اگر (pl.innerReduced)
        تسلسل_Horizontal<A, Op, OpM>(pl, w, res, mbuf, partial.ڈیٹا());
    ورنہ
        تسلسل_Vertical<A, Op, OpM>(pl, w, res, mbuf, partial.ڈیٹا());

    اگر (w.nsplit > 1)
    {
        برائے (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            برائے (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.گنتی_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

سانچہ<قسم_نام A> ساکن خالی fill_نتیجہ(A* res, int64 n, A v)
{
    برائے (int64 i = 0; i < n; i++)
        res[i] = v;
}

سانچہ<قسم_نام A> ساکن خالی تسلسل_Op(ثابت Plan& pl, صحیح op, A* res)
{
    اگر (pl.گنتی_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_مجموع || op == REDUCE_LOG_مجموع_EXP ? OpMax<A>::init() : A(0);
        fill_نتیجہ(res, pl.nout, v);
        واپس;
    }
    انتخاب (op)
    {
    حالت REDUCE_مجموع:   تسلسل_Pass<A, Op_مجموع<A>, Plain<A, Op_مجموع<A> > >(pl, res, خالی_اشاریہ); توڑو;
    حالت REDUCE_AVG:   تسلسل_Pass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, خالی_اشاریہ); توڑو;
    حالت REDUCE_SUM2:  تسلسل_Pass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, خالی_اشاریہ); توڑو;
    حالت REDUCE_L1:    تسلسل_Pass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, خالی_اشاریہ); توڑو;
    حالت REDUCE_L2:    تسلسل_Pass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, خالی_اشاریہ); توڑو;
    حالت REDUCE_PROD:  تسلسل_Pass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, خالی_اشاریہ); توڑو;
    حالت REDUCE_LOG_مجموع: تسلسل_Pass<A, Op_Log_مجموع<A>, Plain<A, Op_Log_مجموع<A> > >(pl, res, خالی_اشاریہ); توڑو;
    حالت REDUCE_LOG_مجموع_EXP:
    {
        Auto_بفر<A> mbuf((size_t)pl.nout);
        تسلسل_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.ڈیٹا(), خالی_اشاریہ);
        تسلسل_Pass<A, Op_مجموع_Exp_Shifted<A>, ShiftedExp<A> >(pl, res, mbuf.ڈیٹا());
        توڑو;
    }
    طےشدہ:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

سانچہ<قسم_نام A> ساکن خالی تسلسل_Min_Max(ثابت Plan& pl, صحیح op, A* res)
{
    اگر (pl.گنتی_2 == 0)
        fill_نتیجہ(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    ورنہ اگر (op == REDUCE_MAX)
        تسلسل_Pass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, خالی_اشاریہ);
    ورنہ
        تسلسل_Pass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, خالی_اشاریہ);
}

ساکن خالی dispatch(ثابت Plan& pl, صحیح op, uchar* res)
{
    ثابت منطقی minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    انتخاب (pl.adepth)
    {
    حالت CV_32F:
        اگر (minmax) تسلسل_Min_Max<اعشاری>(pl, op, (اعشاری*)res); ورنہ تسلسل_Op<اعشاری>(pl, op, (اعشاری*)res);
        توڑو;
    حالت CV_64F:
        اگر (minmax) تسلسل_Min_Max<دوہرا>(pl, op, (دوہرا*)res); ورنہ تسلسل_Op<دوہرا>(pl, op, (دوہرا*)res);
        توڑو;
    حالت CV_8U:  تسلسل_Min_Max<uchar>(pl, op, (uchar*)res); توڑو;
    حالت CV_8S:  تسلسل_Min_Max<schar>(pl, op, (schar*)res); توڑو;
    حالت CV_16U: تسلسل_Min_Max<ushort>(pl, op, (ushort*)res); توڑو;
    حالت CV_16S: تسلسل_Min_Max<مختصر>(pl, op, (مختصر*)res); توڑو;
    حالت CV_32U: تسلسل_Min_Max<غیرعلامتی>(pl, op, (غیرعلامتی*)res); توڑو;
    حالت CV_32S: تسلسل_Min_Max<صحیح>(pl, op, (صحیح*)res); توڑو;
    حالت CV_64U: تسلسل_Min_Max<uint64_t>(pl, op, (uint64_t*)res); توڑو;
    حالت CV_64S: تسلسل_Min_Max<int64_t>(pl, op, (int64_t*)res); توڑو;
    طےشدہ:
        CV_Error(Error::Sts_اندرونی, "reduce: unexpected accumulator depth");
    }
}

ساکن صحیح مجمع_گہرائی(صحیح sdepth, صحیح ddepth, صحیح op)
{
    اگر (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        اگر (sdepth == CV_16F || sdepth == CV_16BF)
            واپس CV_32F;
        واپس sdepth == CV_Bool ? CV_8U : sdepth;
    }
    ثابت منطقی wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    واپس wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(صحیح _rtype, ثابت std::vector<صحیح>& _axes, منطقی _keepdims, صحیح _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

خالی reduce(داخلہ_Array _src, OutputArray _dst, ثابت ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح op = params.rtype, dtype = params.dtype;
    ثابت منطقی keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_مجموع && op <= REDUCE_LOG_مجموع_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    ثابت صحیح dims = src.dims, cn = src.channels();
    ثابت صحیح sdepth = src.گہرائی();
    صحیح ddepth = dtype < 0 ? sdepth : CV_MAT_گہرائی(dtype);
    اگر ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    منطقی reduced[CV_MAX_DIM] = {جھوٹ};
    اگر (params.axes.empty())
        std::fill(reduced, reduced + dims, سچ);
    برائے (صحیح a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        صحیح ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = سچ;
    }

    صحیح keepShape[CV_MAX_DIM], برآمد_Shape[CV_MAX_DIM], nout = 0;
    برائے (صحیح i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        اگر (!reduced[i] || keepdims)
            برآمد_Shape[nout++] = keepShape[i];
    }
    ثابت صحیح dtype_ = CV_MAKETYPE(ddepth, cn);
    _dst.create(nout, برآمد_Shape, dtype_);
    Mat dst = _dst.getMat();

    // The results are computed into a contiguous accumulator-typed buffer laid out like the
    // keepdims output (channels innermost), then converted into dst if needed.
    Plan pl;
    pl.src = src.ڈیٹا;
    pl.sdepth = sdepth == CV_Bool ? CV_8U : sdepth;
    pl.adepth = مجمع_گہرائی(sdepth, ddepth, op);
    pl.sesz1 = CV_ELEM_SIZE1(sdepth);
    pl.cvt = pl.sdepth == pl.adepth ? خالی_اشاریہ : getConvertFunc(pl.sdepth, pl.adepth);
    CV_Assert(pl.sdepth == pl.adepth || pl.cvt);

    // gather the axes (+ the channel axis), outer first
    صحیح n = 0;
    صحیح size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    منطقی red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    برائے (صحیح i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.گنتی_2 = 1;
    برائے (صحیح i = 0; i < dims; i++)
    {
        اگر (reduced[i])
            pl.گنتی_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.قدم[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    اگر (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = جھوٹ;
        n++;
    }

    pl.nout = (int64)dst.کل()*cn;
    ثابت منطقی براہ_راست = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.ڈیٹا;
    اگر (!براہ_راست)
    {
        resMat.create(1, (صحیح)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.ڈیٹا;
    }
    اگر (pl.nout == 0)
        واپس;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    صحیح m = 0;
    برائے (صحیح i = 0; i < n; i++)
    {
        اگر (size[i] == 0)
        {
            // empty source along this axis
            اگر (red[i])
            {
                pl.گنتی_2 = 0;
                جاری;
            }
            واپس;
        }
        اگر (size[i] == 1)
            جاری;
        اگر (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        ورنہ
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    اگر (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = جھوٹ;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    برائے (صحیح i = 0; i < m; i++)
    {
        اگر (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        ورنہ
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    اگر (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    اگر (!براہ_راست)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.ڈیٹا);
        اگر (dst.dims == 0)
            r = Mat(0, خالی_اشاریہ, CV_MAKETYPE(pl.adepth, cn), resMat.ڈیٹا);
        r.convert_مقصد(dst, ddepth);
    }
}

} // namespace cv
