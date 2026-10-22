// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#समावेश "precomp.hpp"
#समावेश "opencv2/core/hal/intrin.hpp"

#समावेश <algorithm>
#समावेश <cmath>
#समावेश <limits>

namespace cv {

namespace {

क्रमागत
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

ढाँचा<typename T> काष्ठा VecT { क्रमागत { ok = 0 }; प्रकार T vtype; };
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
#घोषणा CV_RND_VEC(T, V) ढाँचा<> काष्ठा VecT<T> { क्रमागत { ok = 1 }; प्रकार V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(लघु, v_int16)
CV_RND_VEC(अचिन्हित, v_uint32)
CV_RND_VEC(पूर्णांक, v_int32)
CV_RND_VEC(भग्न, v_float32)
#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(द्विगुन, v_float64)
#पूर्ण_अगर
#अघोषित CV_RND_VEC
#पूर्ण_अगर

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

ढाँचा<typename A> काष्ठा Op_योग
{
    अटल A init() { वापस A(0); }
    अटल A apply(A a, A x) { वापस a + x; }
    अटल A combine(A a, A b) { वापस a + b; }
    अटल A finalize(A a, int64) { वापस a; }
    ढाँचा<typename V> अटल V vapply(स्थिर V& a, स्थिर V& x) { वापस v_add(a, x); }
    ढाँचा<typename V> अटल V vcombine(स्थिर V& a, स्थिर V& b) { वापस v_add(a, b); }
};

ढाँचा<typename A> काष्ठा OpAvg : Op_योग<A>
{
    अटल A finalize(A a, int64 n) { वापस n > 0 ? A(a/(द्विगुन)n) : A(0); }
};

ढाँचा<typename A> काष्ठा Op_Log_योग : Op_योग<A>
{
    अटल A finalize(A a, int64) { वापस A(std::log(a)); }
};

ढाँचा<typename A> काष्ठा OpSum2 : Op_योग<A>
{
    अटल A apply(A a, A x) { वापस a + x*x; }
    ढाँचा<typename V> अटल V vapply(स्थिर V& a, स्थिर V& x) { वापस v_add(a, v_mul(x, x)); }
};

ढाँचा<typename A> काष्ठा OpL2 : OpSum2<A>
{
    अटल A finalize(A a, int64) { वापस A(std::वर्ग_मूल(a)); }
};

ढाँचा<typename A> काष्ठा OpL1 : Op_योग<A>
{
    अटल A apply(A a, A x) { वापस a + (x < A(0) ? -x : x); }
    ढाँचा<typename V> अटल V vapply(स्थिर V& a, स्थिर V& x) { वापस v_add(a, v_abs(x)); }
};

ढाँचा<typename A> काष्ठा OpProd
{
    अटल A init() { वापस A(1); }
    अटल A apply(A a, A x) { वापस a*x; }
    अटल A combine(A a, A b) { वापस a*b; }
    अटल A finalize(A a, int64) { वापस a; }
    ढाँचा<typename V> अटल V vapply(स्थिर V& a, स्थिर V& x) { वापस v_mul(a, x); }
    ढाँचा<typename V> अटल V vcombine(स्थिर V& a, स्थिर V& b) { वापस v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
ढाँचा<typename A, bool hasInf = std::numeric_limits<A>::has_infinity> काष्ठा Extremes
{
    अटल A निचला() { वापस std::numeric_limits<A>::lowest(); }
    अटल A ऊपरी() { वापस std::numeric_limits<A>::max(); }
};

ढाँचा<typename A> काष्ठा Extremes<A, true>
{
    अटल A निचला() { वापस -std::numeric_limits<A>::infinity(); }
    अटल A ऊपरी() { वापस std::numeric_limits<A>::infinity(); }
};

ढाँचा<typename A> काष्ठा OpMax
{
    अटल A init() { वापस Extremes<A>::निचला(); }
    अटल A apply(A a, A x) { वापस a < x ? x : a; }
    अटल A combine(A a, A b) { वापस a < b ? b : a; }
    अटल A finalize(A a, int64) { वापस a; }
    ढाँचा<typename V> अटल V vapply(स्थिर V& a, स्थिर V& x) { वापस v_max(a, x); }
    ढाँचा<typename V> अटल V vcombine(स्थिर V& a, स्थिर V& b) { वापस v_max(a, b); }
};

ढाँचा<typename A> काष्ठा OpMin
{
    अटल A init() { वापस Extremes<A>::ऊपरी(); }
    अटल A apply(A a, A x) { वापस x < a ? x : a; }
    अटल A combine(A a, A b) { वापस b < a ? b : a; }
    अटल A finalize(A a, int64) { वापस a; }
    ढाँचा<typename V> अटल V vapply(स्थिर V& a, स्थिर V& x) { वापस v_min(a, x); }
    ढाँचा<typename V> अटल V vcombine(स्थिर V& a, स्थिर V& b) { वापस v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
ढाँचा<typename A> काष्ठा Op_योग_Exp_Shifted
{
    अटल A init() { वापस A(0); }
    अटल A apply(A a, A x, A m) { वापस a + A(std::exp(x - m)); }
    अटल A combine(A a, A b) { वापस a + b; }
    अटल A finalize(A a, A m) { वापस std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
ढाँचा<श्रेणी Op, typename A, bool vec = (VecT<A>::ok != 0)> काष्ठा HRow
{
    अटल A run(A acc, स्थिर A* x, int64 n)
    {
        क्रम (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        वापस acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
ढाँचा<श्रेणी Op, typename A, bool vec = (VecT<A>::ok != 0)> काष्ठा VRow
{
    अटल व्योम run(A* acc, स्थिर A* x, int64 n)
    {
        क्रम (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#अगर (CV_SIMD || CV_SIMD_SCALABLE)
ढाँचा<श्रेणी Op, typename A> काष्ठा HRow<Op, A, true>
{
    अटल A run(A acc, स्थिर A* x, int64 n)
    {
        प्रकार typename VecT<A>::vtype V;
        स्थिर पूर्णांक L = VTraits<V>::vlanes();
        int64 i = 0;
        अगर (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            क्रम (पूर्णांक k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            क्रम (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            क्रम (पूर्णांक k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        क्रम (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        वापस acc;
    }
};

ढाँचा<श्रेणी Op, typename A> काष्ठा VRow<Op, A, true>
{
    अटल व्योम run(A* acc, स्थिर A* x, int64 n)
    {
        प्रकार typename VecT<A>::vtype V;
        स्थिर पूर्णांक L = VTraits<V>::vlanes();
        int64 i = 0;
        क्रम (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        क्रम (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#पूर्ण_अगर

//////////////////////////////// plan ////////////////////////////////

काष्ठा Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    पूर्णांक nk;
    पूर्णांक ksize[RND_MAX_DIMS];
    सूचक_भेद_प्रकार ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    पूर्णांक nr;
    पूर्णांक rsize[RND_MAX_DIMS];
    सूचक_भेद_प्रकार rsstep[RND_MAX_DIMS];
    bool innerReduced;      // the innermost axis is reduced (horizontal mode)

    स्थिर uchar* src;
    पूर्णांक sdepth, adepth;     // source depth and accumulator depth
    माप_प्रकार sesz1;
    int64 गिनती_2;            // reduced elements per output
    int64 nout;             // number of outputs
    द्विआधारी_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
काष्ठा Iter
{
    पूर्णांक n = 0;
    स्थिर पूर्णांक* size = nullptr;
    स्थिर सूचक_भेद_प्रकार* s = nullptr;
    स्थिर सूचक_भेद_प्रकार* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    सूचक_भेद_प्रकार soff = 0, doff = 0;

    व्योम init(पूर्णांक n_, स्थिर पूर्णांक* आकार_3, स्थिर सूचक_भेद_प्रकार* s_, स्थिर सूचक_भेद_प्रकार* d_, int64 flat)
    {
        n = n_; size = आकार_3; s = s_; d = d_;
        soff = doff = 0;
        क्रम (पूर्णांक k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (सूचक_भेद_प्रकार)idx[k]*s[k];
            अगर (d)
                doff += (सूचक_भेद_प्रकार)idx[k]*d[k];
            flat = q;
        }
    }

    व्योम next()
    {
        क्रम (पूर्णांक k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            अगर (d)
                doff += d[k];
            अगर (++idx[k] < size[k])
                वापस;
            soff -= (सूचक_भेद_प्रकार)size[k]*s[k];
            अगर (d)
                doff -= (सूचक_भेद_प्रकार)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
ढाँचा<typename A> अटल स्थिर A* loadRow(स्थिर Plan& pl, स्थिर uchar* p, सूचक_भेद_प्रकार step, int64 len,
                                             A* buf, uchar* tmp)
{
    अगर (step != (सूचक_भेद_प्रकार)pl.sesz1)
    {
        क्रम (int64 i = 0; i < len; i++, p += step)
            स_नकल(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    अगर (!pl.cvt)
        वापस (स्थिर A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, आकार((पूर्णांक)len, 1), 0);
    वापस buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
ढाँचा<typename A, typename F> अटल व्योम forRow(स्थिर Plan& pl, स्थिर uchar* p, सूचक_भेद_प्रकार step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    स्थिर bool direct = !pl.cvt && step == (सूचक_भेद_प्रकार)pl.sesz1;
    स्थिर int64 blk = direct ? n : (int64)RND_BLOCK;
    क्रम (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
काष्ठा Work
{
    int64 nitems, nsplit;
    द्विगुन nstripes;
    bool serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
ढाँचा<typename A, श्रेणी Op, श्रेणी OpM> अटल व्योम runHorizontal(स्थिर Plan& pl, स्थिर Work& w, A* res,
                                                                   स्थिर A* mbuf, A* partial)
{
    स्थिर पूर्णांक nr = pl.nr;
    स्थिर int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    स्थिर सूचक_भेद_प्रकार rstep = pl.rsstep[nr - 1];
    स्थिर int64 nsplit = w.nsplit;

    स्वतः body = [&](स्थिर Range& range)
    {
        Auto_बफर<A> bufA(RND_BLOCK);
        Auto_बफर<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        अगर (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        जबतक (it < range.end)
        {
            स्थिर int64 o = it / nsplit, s = it - o*nsplit;
            अगर (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            स्थिर uchar* base = pl.src + ko.soff;
            स्थिर A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.गिनती_2/nsplit, b = (s + 1)*pl.गिनती_2/nsplit;
            A acc = Op::init();
            अगर (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                जबतक (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.तथ्य(), bufT.तथ्य(),
                              [&](स्थिर A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            अगर (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.गिनती_2, m);
                ko.next();
            }
            अन्यथा
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    अगर (w.serial)
        body(Range(0, (पूर्णांक)w.nitems));
    अन्यथा
        parallel_for_(Range(0, (पूर्णांक)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
ढाँचा<typename A, श्रेणी Op, श्रेणी OpM> अटल व्योम runVertical(स्थिर Plan& pl, स्थिर Work& w, A* res,
                                                                 स्थिर A* mbuf, A* partial)
{
    स्थिर पूर्णांक nk = pl.nk;
    स्थिर int64 m = pl.ksize[nk - 1];
    स्थिर सूचक_भेद_प्रकार kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    स्थिर int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    स्थिर int64 nsplit = w.nsplit;
    int64 rcount = 1;
    क्रम (पूर्णांक i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    स्वतः body = [&](स्थिर Range& range)
    {
        Auto_बफर<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        Auto_बफर<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.तथ्य();
        Iter ko, ro;
        क्रम (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            स्थिर int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            स्थिर सूचक_भेद_प्रकार soff = ko.soff + (सूचक_भेद_प्रकार)e0*kstep, doff = ko.doff + (सूचक_भेद_प्रकार)e0*kdstep;
            स्थिर A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            क्रम (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            अगर (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                क्रम (; a < b; a++, ro.next())
                {
                    स्थिर uchar* p = pl.src + soff + ro.soff;
                    स्थिर A* x = loadRow<A>(pl, p, kstep, len, bufA.तथ्य(), bufT.तथ्य());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            अगर (nsplit == 1)
                क्रम (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.गिनती_2, mrow ? mrow[i] : A(0));
            अन्यथा
                क्रम (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    अगर (w.serial)
        body(Range(0, (पूर्णांक)w.nitems));
    अन्यथा
        parallel_for_(Range(0, (पूर्णांक)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
ढाँचा<typename A, श्रेणी Op> काष्ठा Plain
{
    अटल A run(A acc, स्थिर A* x, int64 n, A) { वापस HRow<Op, A>::run(acc, x, n); }
    अटल व्योम vrun(A* acc, स्थिर A* x, int64 n, स्थिर A*) { VRow<Op, A>::run(acc, x, n); }
    अटल A finalize(A a, int64 गिनती_2, A) { वापस Op::finalize(a, गिनती_2); }
};

ढाँचा<typename A> काष्ठा ShiftedExp
{
    प्रकार Op_योग_Exp_Shifted<A> Op;
    अटल A run(A acc, स्थिर A* x, int64 n, A m)
    {
        क्रम (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        वापस acc;
    }
    अटल व्योम vrun(A* acc, स्थिर A* x, int64 n, स्थिर A* m)
    {
        क्रम (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    अटल A finalize(A a, int64, A m) { वापस Op::finalize(a, m); }
};

ढाँचा<typename A, श्रेणी Op, श्रेणी OpM> अटल व्योम runPass(स्थिर Plan& pl, A* res, स्थिर A* mbuf)
{
    स्थिर द्विगुन work = (द्विगुन)pl.nout*(द्विगुन)std::max(pl.गिनती_2, (int64)1);
    स्थिर पूर्णांक nthreads = work < RND_SERIAL_WORK ? 1 : get_अंक_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 वस्तुएँ;
    अगर (pl.innerReduced)
        वस्तुएँ = pl.nout;
    अन्यथा
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        वस्तुएँ = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    स्थिर int64 want = 4*(int64)nthreads;
    अगर (!w.serial && वस्तुएँ < want && pl.गिनती_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.गिनती_2/4096 : pl.गिनती_2/16;
        w.nsplit = std::max((int64)1, std::min((want + वस्तुएँ - 1)/वस्तुएँ, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = वस्तुएँ*w.nsplit;
    w.nstripes = std::min((द्विगुन)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= पूर्णांक_उच्च);

    Auto_बफर<A> partial(w.nsplit > 1 ? (माप_प्रकार)(w.nsplit*pl.nout) : 1);
    अगर (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.तथ्य());
    अन्यथा
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.तथ्य());

    अगर (w.nsplit > 1)
    {
        क्रम (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(माप_प्रकार)o];
            क्रम (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(माप_प्रकार)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.गिनती_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

ढाँचा<typename A> अटल व्योम fill_परिणाम(A* res, int64 n, A v)
{
    क्रम (int64 i = 0; i < n; i++)
        res[i] = v;
}

ढाँचा<typename A> अटल व्योम runOp(स्थिर Plan& pl, पूर्णांक op, A* res)
{
    अगर (pl.गिनती_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_योग || op == REDUCE_LOG_योग_EXP ? OpMax<A>::init() : A(0);
        fill_परिणाम(res, pl.nout, v);
        वापस;
    }
    चयन (op)
    {
    हाल REDUCE_योग:   runPass<A, Op_योग<A>, Plain<A, Op_योग<A> > >(pl, res, nullptr); अवरोध;
    हाल REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); अवरोध;
    हाल REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); अवरोध;
    हाल REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); अवरोध;
    हाल REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); अवरोध;
    हाल REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); अवरोध;
    हाल REDUCE_LOG_योग: runPass<A, Op_Log_योग<A>, Plain<A, Op_Log_योग<A> > >(pl, res, nullptr); अवरोध;
    हाल REDUCE_LOG_योग_EXP:
    {
        Auto_बफर<A> mbuf((माप_प्रकार)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.तथ्य(), nullptr);
        runPass<A, Op_योग_Exp_Shifted<A>, ShiftedExp<A> >(pl, res, mbuf.तथ्य());
        अवरोध;
    }
    शेष:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

ढाँचा<typename A> अटल व्योम runMinMax(स्थिर Plan& pl, पूर्णांक op, A* res)
{
    अगर (pl.गिनती_2 == 0)
        fill_परिणाम(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    अन्यथा अगर (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    अन्यथा
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

अटल व्योम dispatch(स्थिर Plan& pl, पूर्णांक op, uchar* res)
{
    स्थिर bool minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    चयन (pl.adepth)
    {
    हाल CV_32F:
        अगर (minmax) runMinMax<भग्न>(pl, op, (भग्न*)res); अन्यथा runOp<भग्न>(pl, op, (भग्न*)res);
        अवरोध;
    हाल CV_64F:
        अगर (minmax) runMinMax<द्विगुन>(pl, op, (द्विगुन*)res); अन्यथा runOp<द्विगुन>(pl, op, (द्विगुन*)res);
        अवरोध;
    हाल CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); अवरोध;
    हाल CV_8S:  runMinMax<schar>(pl, op, (schar*)res); अवरोध;
    हाल CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); अवरोध;
    हाल CV_16S: runMinMax<लघु>(pl, op, (लघु*)res); अवरोध;
    हाल CV_32U: runMinMax<अचिन्हित>(pl, op, (अचिन्हित*)res); अवरोध;
    हाल CV_32S: runMinMax<पूर्णांक>(pl, op, (पूर्णांक*)res); अवरोध;
    हाल CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); अवरोध;
    हाल CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); अवरोध;
    शेष:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

अटल पूर्णांक accumulatorDepth(पूर्णांक sdepth, पूर्णांक ddepth, पूर्णांक op)
{
    अगर (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        अगर (sdepth == CV_16F || sdepth == CV_16BF)
            वापस CV_32F;
        वापस sdepth == CV_Bool ? CV_8U : sdepth;
    }
    स्थिर bool wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    वापस wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(पूर्णांक _rtype, स्थिर std::vector<पूर्णांक>& _axes, bool _keepdims, पूर्णांक _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

व्योम reduce(निवेश_Array _src, OutputArray _dst, स्थिर ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    स्थिर पूर्णांक op = params.rtype, dtype = params.dtype;
    स्थिर bool keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_योग && op <= REDUCE_LOG_योग_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    स्थिर पूर्णांक dims = src.dims, cn = src.channels();
    स्थिर पूर्णांक sdepth = src.depth();
    पूर्णांक ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    अगर ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    bool reduced[CV_MAX_DIM] = {false};
    अगर (params.axes.empty())
        std::fill(reduced, reduced + dims, true);
    क्रम (पूर्णांक a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        पूर्णांक ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = true;
    }

    पूर्णांक keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    क्रम (पूर्णांक i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        अगर (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    स्थिर पूर्णांक dtype_ = CV_MAKETYPE(ddepth, cn);
    _dst.create(nout, outShape, dtype_);
    Mat dst = _dst.getMat();

    // The results are computed into a contiguous accumulator-typed buffer laid out like the
    // keepdims output (channels innermost), then converted into dst if needed.
    Plan pl;
    pl.src = src.तथ्य;
    pl.sdepth = sdepth == CV_Bool ? CV_8U : sdepth;
    pl.adepth = accumulatorDepth(sdepth, ddepth, op);
    pl.sesz1 = CV_ELEM_SIZE1(sdepth);
    pl.cvt = pl.sdepth == pl.adepth ? nullptr : getConvertFunc(pl.sdepth, pl.adepth);
    CV_Assert(pl.sdepth == pl.adepth || pl.cvt);

    // gather the axes (+ the channel axis), outer first
    पूर्णांक n = 0;
    पूर्णांक size[RND_MAX_DIMS];
    सूचक_भेद_प्रकार sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    bool red[RND_MAX_DIMS];
    सूचक_भेद_प्रकार dcur = cn;
    सूचक_भेद_प्रकार dsteps[CV_MAX_DIM];
    क्रम (पूर्णांक i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.गिनती_2 = 1;
    क्रम (पूर्णांक i = 0; i < dims; i++)
    {
        अगर (reduced[i])
            pl.गिनती_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (सूचक_भेद_प्रकार)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    अगर (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (सूचक_भेद_प्रकार)pl.sesz1;
        dstep[n] = 1;
        red[n] = false;
        n++;
    }

    pl.nout = (int64)dst.योग()*cn;
    स्थिर bool direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.तथ्य;
    अगर (!direct)
    {
        resMat.create(1, (पूर्णांक)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.तथ्य;
    }
    अगर (pl.nout == 0)
        वापस;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    पूर्णांक m = 0;
    क्रम (पूर्णांक i = 0; i < n; i++)
    {
        अगर (size[i] == 0)
        {
            // empty source along this axis
            अगर (red[i])
            {
                pl.गिनती_2 = 0;
                जारी;
            }
            वापस;
        }
        अगर (size[i] == 1)
            जारी;
        अगर (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
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
    अगर (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (सूचक_भेद_प्रकार)pl.sesz1;
        dstep[0] = 1;
        red[0] = false;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    क्रम (पूर्णांक i = 0; i < m; i++)
    {
        अगर (red[i])
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
    अगर (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    अगर (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.तथ्य);
        अगर (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.तथ्य);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
