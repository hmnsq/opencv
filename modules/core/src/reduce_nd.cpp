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

ढाँचा<typename T> संरचना VecT { गणना { ok = 0 }; प्रकारनिर्देश T vtype; };
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) ढाँचा<> संरचना VecT<T> { गणना { ok = 1 }; प्रकारनिर्देश V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(लघु, v_int16)
CV_RND_VEC(अचिन्हित, v_uint32)
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

ढाँचा<typename A> संरचना OpSum
{
    स्थिर A init() { फर्काऊ A(0); }
    स्थिर A apply(A a, A x) { फर्काऊ a + x; }
    स्थिर A combine(A a, A b) { फर्काऊ a + b; }
    स्थिर A finalize(A a, int64) { फर्काऊ a; }
    ढाँचा<typename V> स्थिर V vapply(अचर V& a, अचर V& x) { फर्काऊ v_add(a, x); }
    ढाँचा<typename V> स्थिर V vcombine(अचर V& a, अचर V& b) { फर्काऊ v_add(a, b); }
};

ढाँचा<typename A> संरचना OpAvg : OpSum<A>
{
    स्थिर A finalize(A a, int64 n) { फर्काऊ n > 0 ? A(a/(द्विगुण)n) : A(0); }
};

ढाँचा<typename A> संरचना OpLogSum : OpSum<A>
{
    स्थिर A finalize(A a, int64) { फर्काऊ A(std::log(a)); }
};

ढाँचा<typename A> संरचना OpSum2 : OpSum<A>
{
    स्थिर A apply(A a, A x) { फर्काऊ a + x*x; }
    ढाँचा<typename V> स्थिर V vapply(अचर V& a, अचर V& x) { फर्काऊ v_add(a, v_mul(x, x)); }
};

ढाँचा<typename A> संरचना OpL2 : OpSum2<A>
{
    स्थिर A finalize(A a, int64) { फर्काऊ A(std::वर्ग_मूल(a)); }
};

ढाँचा<typename A> संरचना OpL1 : OpSum<A>
{
    स्थिर A apply(A a, A x) { फर्काऊ a + (x < A(0) ? -x : x); }
    ढाँचा<typename V> स्थिर V vapply(अचर V& a, अचर V& x) { फर्काऊ v_add(a, v_abs(x)); }
};

ढाँचा<typename A> संरचना OpProd
{
    स्थिर A init() { फर्काऊ A(1); }
    स्थिर A apply(A a, A x) { फर्काऊ a*x; }
    स्थिर A combine(A a, A b) { फर्काऊ a*b; }
    स्थिर A finalize(A a, int64) { फर्काऊ a; }
    ढाँचा<typename V> स्थिर V vapply(अचर V& a, अचर V& x) { फर्काऊ v_mul(a, x); }
    ढाँचा<typename V> स्थिर V vcombine(अचर V& a, अचर V& b) { फर्काऊ v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
ढाँचा<typename A, तार्किक hasInf = std::numeric_limits<A>::has_infinity> संरचना Extremes
{
    स्थिर A तल्लो() { फर्काऊ std::numeric_limits<A>::lowest(); }
    स्थिर A माथिल्लो() { फर्काऊ std::numeric_limits<A>::max(); }
};

ढाँचा<typename A> संरचना Extremes<A, सत्य>
{
    स्थिर A तल्लो() { फर्काऊ -std::numeric_limits<A>::infinity(); }
    स्थिर A माथिल्लो() { फर्काऊ std::numeric_limits<A>::infinity(); }
};

ढाँचा<typename A> संरचना OpMax
{
    स्थिर A init() { फर्काऊ Extremes<A>::तल्लो(); }
    स्थिर A apply(A a, A x) { फर्काऊ a < x ? x : a; }
    स्थिर A combine(A a, A b) { फर्काऊ a < b ? b : a; }
    स्थिर A finalize(A a, int64) { फर्काऊ a; }
    ढाँचा<typename V> स्थिर V vapply(अचर V& a, अचर V& x) { फर्काऊ v_max(a, x); }
    ढाँचा<typename V> स्थिर V vcombine(अचर V& a, अचर V& b) { फर्काऊ v_max(a, b); }
};

ढाँचा<typename A> संरचना OpMin
{
    स्थिर A init() { फर्काऊ Extremes<A>::माथिल्लो(); }
    स्थिर A apply(A a, A x) { फर्काऊ x < a ? x : a; }
    स्थिर A combine(A a, A b) { फर्काऊ b < a ? b : a; }
    स्थिर A finalize(A a, int64) { फर्काऊ a; }
    ढाँचा<typename V> स्थिर V vapply(अचर V& a, अचर V& x) { फर्काऊ v_min(a, x); }
    ढाँचा<typename V> स्थिर V vcombine(अचर V& a, अचर V& b) { फर्काऊ v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
ढाँचा<typename A> संरचना OpSumExpShifted
{
    स्थिर A init() { फर्काऊ A(0); }
    स्थिर A apply(A a, A x, A m) { फर्काऊ a + A(std::exp(x - m)); }
    स्थिर A combine(A a, A b) { फर्काऊ a + b; }
    स्थिर A finalize(A a, A m) { फर्काऊ std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
ढाँचा<वर्ग Op, typename A, तार्किक vec = (VecT<A>::ok != 0)> संरचना HRow
{
    स्थिर A run(A acc, अचर A* x, int64 n)
    {
        लागि (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        फर्काऊ acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
ढाँचा<वर्ग Op, typename A, तार्किक vec = (VecT<A>::ok != 0)> संरचना VRow
{
    स्थिर शून्य run(A* acc, अचर A* x, int64 n)
    {
        लागि (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<वर्ग Op, typename A> संरचना HRow<Op, A, सत्य>
{
    स्थिर A run(A acc, अचर A* x, int64 n)
    {
        प्रकारनिर्देश typename VecT<A>::vtype V;
        अचर पूर्णाङ्क L = VTraits<V>::vlanes();
        int64 i = 0;
        यदि (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            लागि (पूर्णाङ्क k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            लागि (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            लागि (पूर्णाङ्क k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        लागि (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        फर्काऊ acc;
    }
};

ढाँचा<वर्ग Op, typename A> संरचना VRow<Op, A, सत्य>
{
    स्थिर शून्य run(A* acc, अचर A* x, int64 n)
    {
        प्रकारनिर्देश typename VecT<A>::vtype V;
        अचर पूर्णाङ्क L = VTraits<V>::vlanes();
        int64 i = 0;
        लागि (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        लागि (; i < n; i++)
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
    सूचक_भेद_प्रकार ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    पूर्णाङ्क nr;
    पूर्णाङ्क rsize[RND_MAX_DIMS];
    सूचक_भेद_प्रकार rsstep[RND_MAX_DIMS];
    तार्किक innerReduced;      // the innermost axis is reduced (horizontal mode)

    अचर uchar* src;
    पूर्णाङ्क sdepth, adepth;     // source depth and accumulator depth
    माप_प्रकार sesz1;
    int64 गन्ती_2;            // reduced elements per output
    int64 nout;             // number of outputs
    द्विआधारी_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
संरचना Iter
{
    पूर्णाङ्क n = 0;
    अचर पूर्णाङ्क* size = nullptr;
    अचर सूचक_भेद_प्रकार* s = nullptr;
    अचर सूचक_भेद_प्रकार* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    सूचक_भेद_प्रकार soff = 0, doff = 0;

    शून्य init(पूर्णाङ्क n_, अचर पूर्णाङ्क* size_, अचर सूचक_भेद_प्रकार* s_, अचर सूचक_भेद_प्रकार* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        लागि (पूर्णाङ्क k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (सूचक_भेद_प्रकार)idx[k]*s[k];
            यदि (d)
                doff += (सूचक_भेद_प्रकार)idx[k]*d[k];
            flat = q;
        }
    }

    शून्य next()
    {
        लागि (पूर्णाङ्क k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            यदि (d)
                doff += d[k];
            यदि (++idx[k] < size[k])
                फर्काऊ;
            soff -= (सूचक_भेद_प्रकार)size[k]*s[k];
            यदि (d)
                doff -= (सूचक_भेद_प्रकार)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
ढाँचा<typename A> स्थिर अचर A* loadRow(अचर Plan& pl, अचर uchar* p, सूचक_भेद_प्रकार step, int64 len,
                                             A* buf, uchar* tmp)
{
    यदि (step != (सूचक_भेद_प्रकार)pl.sesz1)
    {
        लागि (int64 i = 0; i < len; i++, p += step)
            स_नकल(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    यदि (!pl.cvt)
        फर्काऊ (अचर A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((पूर्णाङ्क)len, 1), 0);
    फर्काऊ buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
ढाँचा<typename A, typename F> स्थिर शून्य forRow(अचर Plan& pl, अचर uchar* p, सूचक_भेद_प्रकार step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    अचर तार्किक direct = !pl.cvt && step == (सूचक_भेद_प्रकार)pl.sesz1;
    अचर int64 blk = direct ? n : (int64)RND_BLOCK;
    लागि (int64 e = 0; e < n; e += blk)
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
ढाँचा<typename A, वर्ग Op, वर्ग OpM> स्थिर शून्य runHorizontal(अचर Plan& pl, अचर Work& w, A* res,
                                                                   अचर A* mbuf, A* partial)
{
    अचर पूर्णाङ्क nr = pl.nr;
    अचर int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    अचर सूचक_भेद_प्रकार rstep = pl.rsstep[nr - 1];
    अचर int64 nsplit = w.nsplit;

    स्वतः body = [&](अचर Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        यदि (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        जबसम्म (it < range.end)
        {
            अचर int64 o = it / nsplit, s = it - o*nsplit;
            यदि (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            अचर uchar* base = pl.src + ko.soff;
            अचर A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.गन्ती_2/nsplit, b = (s + 1)*pl.गन्ती_2/nsplit;
            A acc = Op::init();
            यदि (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                जबसम्म (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](अचर A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            यदि (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.गन्ती_2, m);
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
ढाँचा<typename A, वर्ग Op, वर्ग OpM> स्थिर शून्य runVertical(अचर Plan& pl, अचर Work& w, A* res,
                                                                 अचर A* mbuf, A* partial)
{
    अचर पूर्णाङ्क nk = pl.nk;
    अचर int64 m = pl.ksize[nk - 1];
    अचर सूचक_भेद_प्रकार kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    अचर int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    अचर int64 nsplit = w.nsplit;
    int64 rcount = 1;
    लागि (पूर्णाङ्क i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    स्वतः body = [&](अचर Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        लागि (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            अचर int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            अचर सूचक_भेद_प्रकार soff = ko.soff + (सूचक_भेद_प्रकार)e0*kstep, doff = ko.doff + (सूचक_भेद_प्रकार)e0*kdstep;
            अचर A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            लागि (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            यदि (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                लागि (; a < b; a++, ro.next())
                {
                    अचर uchar* p = pl.src + soff + ro.soff;
                    अचर A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            यदि (nsplit == 1)
                लागि (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.गन्ती_2, mrow ? mrow[i] : A(0));
            अन्यथा
                लागि (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    यदि (w.serial)
        body(Range(0, (पूर्णाङ्क)w.nitems));
    अन्यथा
        parallel_for_(Range(0, (पूर्णाङ्क)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
ढाँचा<typename A, वर्ग Op> संरचना Plain
{
    स्थिर A run(A acc, अचर A* x, int64 n, A) { फर्काऊ HRow<Op, A>::run(acc, x, n); }
    स्थिर शून्य vrun(A* acc, अचर A* x, int64 n, अचर A*) { VRow<Op, A>::run(acc, x, n); }
    स्थिर A finalize(A a, int64 गन्ती_2, A) { फर्काऊ Op::finalize(a, गन्ती_2); }
};

ढाँचा<typename A> संरचना ShiftedExp
{
    प्रकारनिर्देश OpSumExpShifted<A> Op;
    स्थिर A run(A acc, अचर A* x, int64 n, A m)
    {
        लागि (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        फर्काऊ acc;
    }
    स्थिर शून्य vrun(A* acc, अचर A* x, int64 n, अचर A* m)
    {
        लागि (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    स्थिर A finalize(A a, int64, A m) { फर्काऊ Op::finalize(a, m); }
};

ढाँचा<typename A, वर्ग Op, वर्ग OpM> स्थिर शून्य runPass(अचर Plan& pl, A* res, अचर A* mbuf)
{
    अचर द्विगुण work = (द्विगुण)pl.nout*(द्विगुण)std::max(pl.गन्ती_2, (int64)1);
    अचर पूर्णाङ्क nthreads = work < RND_SERIAL_WORK ? 1 : get_अङ्क_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 वस्तुहरू;
    यदि (pl.innerReduced)
        वस्तुहरू = pl.nout;
    अन्यथा
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        वस्तुहरू = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    अचर int64 want = 4*(int64)nthreads;
    यदि (!w.serial && वस्तुहरू < want && pl.गन्ती_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.गन्ती_2/4096 : pl.गन्ती_2/16;
        w.nsplit = std::max((int64)1, std::min((want + वस्तुहरू - 1)/वस्तुहरू, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = वस्तुहरू*w.nsplit;
    w.nstripes = std::min((द्विगुण)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= पूर्णांक_उच्च);

    AutoBuffer<A> partial(w.nsplit > 1 ? (माप_प्रकार)(w.nsplit*pl.nout) : 1);
    यदि (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    अन्यथा
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    यदि (w.nsplit > 1)
    {
        लागि (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(माप_प्रकार)o];
            लागि (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(माप_प्रकार)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.गन्ती_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

ढाँचा<typename A> स्थिर शून्य fillResult(A* res, int64 n, A v)
{
    लागि (int64 i = 0; i < n; i++)
        res[i] = v;
}

ढाँचा<typename A> स्थिर शून्य runOp(अचर Plan& pl, पूर्णाङ्क op, A* res)
{
    यदि (pl.गन्ती_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        फर्काऊ;
    }
    विकल्प (op)
    {
    पक्ष REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); रोक;
    पक्ष REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); रोक;
    पक्ष REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); रोक;
    पक्ष REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); रोक;
    पक्ष REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); रोक;
    पक्ष REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); रोक;
    पक्ष REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); रोक;
    पक्ष REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((माप_प्रकार)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        रोक;
    }
    स्वभाव:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

ढाँचा<typename A> स्थिर शून्य runMinMax(अचर Plan& pl, पूर्णाङ्क op, A* res)
{
    यदि (pl.गन्ती_2 == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    अन्यथा यदि (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    अन्यथा
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

स्थिर शून्य dispatch(अचर Plan& pl, पूर्णाङ्क op, uchar* res)
{
    अचर तार्किक minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    विकल्प (pl.adepth)
    {
    पक्ष CV_32F:
        यदि (minmax) runMinMax<भिन्न>(pl, op, (भिन्न*)res); अन्यथा runOp<भिन्न>(pl, op, (भिन्न*)res);
        रोक;
    पक्ष CV_64F:
        यदि (minmax) runMinMax<द्विगुण>(pl, op, (द्विगुण*)res); अन्यथा runOp<द्विगुण>(pl, op, (द्विगुण*)res);
        रोक;
    पक्ष CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); रोक;
    पक्ष CV_8S:  runMinMax<schar>(pl, op, (schar*)res); रोक;
    पक्ष CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); रोक;
    पक्ष CV_16S: runMinMax<लघु>(pl, op, (लघु*)res); रोक;
    पक्ष CV_32U: runMinMax<अचिन्हित>(pl, op, (अचिन्हित*)res); रोक;
    पक्ष CV_32S: runMinMax<पूर्णाङ्क>(pl, op, (पूर्णाङ्क*)res); रोक;
    पक्ष CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); रोक;
    पक्ष CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); रोक;
    स्वभाव:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

स्थिर पूर्णाङ्क accumulatorDepth(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क op)
{
    यदि (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        यदि (sdepth == CV_16F || sdepth == CV_16BF)
            फर्काऊ CV_32F;
        फर्काऊ sdepth == CV_Bool ? CV_8U : sdepth;
    }
    अचर तार्किक wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    फर्काऊ wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(पूर्णाङ्क _rtype, अचर std::vector<पूर्णाङ्क>& _axes, तार्किक _keepdims, पूर्णाङ्क _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

शून्य reduce(आगत_Array _src, OutputArray _dst, अचर ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    अचर पूर्णाङ्क op = params.rtype, dtype = params.dtype;
    अचर तार्किक keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    अचर पूर्णाङ्क dims = src.dims, cn = src.channels();
    अचर पूर्णाङ्क sdepth = src.depth();
    पूर्णाङ्क ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    यदि ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    तार्किक reduced[CV_MAX_DIM] = {असत्य};
    यदि (params.axes.empty())
        std::fill(reduced, reduced + dims, सत्य);
    लागि (पूर्णाङ्क a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        पूर्णाङ्क ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = सत्य;
    }

    पूर्णाङ्क keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    लागि (पूर्णाङ्क i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        यदि (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    अचर पूर्णाङ्क dtype_ = CV_MAKETYPE(ddepth, cn);
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
    सूचक_भेद_प्रकार sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    तार्किक red[RND_MAX_DIMS];
    सूचक_भेद_प्रकार dcur = cn;
    सूचक_भेद_प्रकार dsteps[CV_MAX_DIM];
    लागि (पूर्णाङ्क i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.गन्ती_2 = 1;
    लागि (पूर्णाङ्क i = 0; i < dims; i++)
    {
        यदि (reduced[i])
            pl.गन्ती_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (सूचक_भेद_प्रकार)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    यदि (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (सूचक_भेद_प्रकार)pl.sesz1;
        dstep[n] = 1;
        red[n] = असत्य;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    अचर तार्किक direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    यदि (!direct)
    {
        resMat.create(1, (पूर्णाङ्क)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    यदि (pl.nout == 0)
        फर्काऊ;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    पूर्णाङ्क m = 0;
    लागि (पूर्णाङ्क i = 0; i < n; i++)
    {
        यदि (size[i] == 0)
        {
            // empty source along this axis
            यदि (red[i])
            {
                pl.गन्ती_2 = 0;
                जारी;
            }
            फर्काऊ;
        }
        यदि (size[i] == 1)
            जारी;
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
        sstep[0] = (सूचक_भेद_प्रकार)pl.sesz1;
        dstep[0] = 1;
        red[0] = असत्य;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    लागि (पूर्णाङ्क i = 0; i < m; i++)
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
