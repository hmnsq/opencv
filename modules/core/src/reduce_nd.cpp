// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// cv::reduce: reduction of an n-dimensional array over any set of axes.

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencv2/core/hal/intrin.hpp"

#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <cmath>
#അന്തര്ഭാവ <limits>

നാമാകാശ cv {

നാമാകാശ {

എണ്ണൽ
{
    RND_MAX_DIMS = CV_MAX_DIM + 1,   // + the channel axis
    RND_BLOCK = 1024,                // elements converted per call when the source needs conversion
    RND_VCHUNK = 1024,               // innermost-axis chunk in the vertical kernel
    RND_SERIAL_WORK = 64*1024,       // below this (source elements) everything runs in one thread
    RND_STRIPE_WORK = 64*1024
};

//////////////////////////////// vector traits ////////////////////////////////

template<typename T> ഘടന VecT { എണ്ണൽ { ok = 0 }; തരനിർവചനം T vtype; };
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
#define CV_RND_VEC(T, V) template<> ഘടന VecT<T> { എണ്ണൽ { ok = 1 }; തരനിർവചനം V vtype; };
CV_RND_VEC(uchar, v_uint8)
CV_RND_VEC(schar, v_int8)
CV_RND_VEC(ushort, v_uint16)
CV_RND_VEC(short, v_int16)
CV_RND_VEC(unsigned, v_uint32)
CV_RND_VEC(പൂർണ്ണസംഖ്യ, v_int32)
CV_RND_VEC(ദശാംശം, v_float32)
#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
CV_RND_VEC(ഇരട്ട, v_float64)
#endif
#undef CV_RND_VEC
#endif

//////////////////////////////// operations ////////////////////////////////
// apply() folds one element into an accumulator, combine() merges two accumulators,
// finalize() turns an accumulator into the result. The vector versions are only used
// for the accumulator types that have them (see VecT).

template<typename A> ഘടന OpSum
{
    സ്ഥിതം A init() { തിരികെ A(0); }
    സ്ഥിതം A apply(A a, A x) { തിരികെ a + x; }
    സ്ഥിതം A combine(A a, A b) { തിരികെ a + b; }
    സ്ഥിതം A finalize(A a, int64) { തിരികെ a; }
    template<typename V> സ്ഥിതം V vapply(സ്ഥിരം V& a, സ്ഥിരം V& x) { തിരികെ v_add(a, x); }
    template<typename V> സ്ഥിതം V vcombine(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_add(a, b); }
};

template<typename A> ഘടന OpAvg : OpSum<A>
{
    സ്ഥിതം A finalize(A a, int64 n) { തിരികെ n > 0 ? A(a/(ഇരട്ട)n) : A(0); }
};

template<typename A> ഘടന OpLogSum : OpSum<A>
{
    സ്ഥിതം A finalize(A a, int64) { തിരികെ A(std::log(a)); }
};

template<typename A> ഘടന OpSum2 : OpSum<A>
{
    സ്ഥിതം A apply(A a, A x) { തിരികെ a + x*x; }
    template<typename V> സ്ഥിതം V vapply(സ്ഥിരം V& a, സ്ഥിരം V& x) { തിരികെ v_add(a, v_mul(x, x)); }
};

template<typename A> ഘടന OpL2 : OpSum2<A>
{
    സ്ഥിതം A finalize(A a, int64) { തിരികെ A(std::sqrt(a)); }
};

template<typename A> ഘടന OpL1 : OpSum<A>
{
    സ്ഥിതം A apply(A a, A x) { തിരികെ a + (x < A(0) ? -x : x); }
    template<typename V> സ്ഥിതം V vapply(സ്ഥിരം V& a, സ്ഥിരം V& x) { തിരികെ v_add(a, v_abs(x)); }
};

template<typename A> ഘടന OpProd
{
    സ്ഥിതം A init() { തിരികെ A(1); }
    സ്ഥിതം A apply(A a, A x) { തിരികെ a*x; }
    സ്ഥിതം A combine(A a, A b) { തിരികെ a*b; }
    സ്ഥിതം A finalize(A a, int64) { തിരികെ a; }
    template<typename V> സ്ഥിതം V vapply(സ്ഥിരം V& a, സ്ഥിരം V& x) { തിരികെ v_mul(a, x); }
    template<typename V> സ്ഥിതം V vcombine(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_mul(a, b); }
};

// Identity elements of max and min. A trait rather than a ternary on has_infinity, so that the
// unary minus is never compiled for an unsigned A (MSVC C4146).
template<typename A, തർക്കം hasInf = std::numeric_limits<A>::has_infinity> ഘടന Extremes
{
    സ്ഥിതം A താഴെയുള്ള() { തിരികെ std::numeric_limits<A>::lowest(); }
    സ്ഥിതം A മുകളിലെ() { തിരികെ std::numeric_limits<A>::max(); }
};

template<typename A> ഘടന Extremes<A, സത്യം>
{
    സ്ഥിതം A താഴെയുള്ള() { തിരികെ -std::numeric_limits<A>::infinity(); }
    സ്ഥിതം A മുകളിലെ() { തിരികെ std::numeric_limits<A>::infinity(); }
};

template<typename A> ഘടന OpMax
{
    സ്ഥിതം A init() { തിരികെ Extremes<A>::താഴെയുള്ള(); }
    സ്ഥിതം A apply(A a, A x) { തിരികെ a < x ? x : a; }
    സ്ഥിതം A combine(A a, A b) { തിരികെ a < b ? b : a; }
    സ്ഥിതം A finalize(A a, int64) { തിരികെ a; }
    template<typename V> സ്ഥിതം V vapply(സ്ഥിരം V& a, സ്ഥിരം V& x) { തിരികെ v_max(a, x); }
    template<typename V> സ്ഥിതം V vcombine(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_max(a, b); }
};

template<typename A> ഘടന OpMin
{
    സ്ഥിതം A init() { തിരികെ Extremes<A>::മുകളിലെ(); }
    സ്ഥിതം A apply(A a, A x) { തിരികെ x < a ? x : a; }
    സ്ഥിതം A combine(A a, A b) { തിരികെ b < a ? b : a; }
    സ്ഥിതം A finalize(A a, int64) { തിരികെ a; }
    template<typename V> സ്ഥിതം V vapply(സ്ഥിരം V& a, സ്ഥിരം V& x) { തിരികെ v_min(a, x); }
    template<typename V> സ്ഥിതം V vcombine(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_min(a, b); }
};

// second pass of log-sum-exp: sum(exp(x - m)), then m + log(sum), where m is the maximum
template<typename A> ഘടന OpSumExpShifted
{
    സ്ഥിതം A init() { തിരികെ A(0); }
    സ്ഥിതം A apply(A a, A x, A m) { തിരികെ a + A(std::exp(x - m)); }
    സ്ഥിതം A combine(A a, A b) { തിരികെ a + b; }
    സ്ഥിതം A finalize(A a, A m) { തിരികെ std::isinf(m) ? m : m + A(std::log(a)); }
};

//////////////////////////////// row kernels ////////////////////////////////

// horizontal: fold n contiguous elements into a scalar accumulator
template<വർഗ്ഗം Op, typename A, തർക്കം vec = (VecT<A>::ok != 0)> ഘടന HRow
{
    സ്ഥിതം A run(A acc, സ്ഥിരം A* x, int64 n)
    {
        വേണ്ടി (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i]);
        തിരികെ acc;
    }
};

// vertical: acc[i] = apply(acc[i], x[i])
template<വർഗ്ഗം Op, typename A, തർക്കം vec = (VecT<A>::ok != 0)> ഘടന VRow
{
    സ്ഥിതം ശൂന്യം run(A* acc, സ്ഥിരം A* x, int64 n)
    {
        വേണ്ടി (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
template<വർഗ്ഗം Op, typename A> ഘടന HRow<Op, A, സത്യം>
{
    സ്ഥിതം A run(A acc, സ്ഥിരം A* x, int64 n)
    {
        തരനിർവചനം typename VecT<A>::vtype V;
        സ്ഥിരം പൂർണ്ണസംഖ്യ L = VTraits<V>::vlanes();
        int64 i = 0;
        എങ്കിൽ (n >= 2*L)
        {
            A buf[VTraits<V>::max_nlanes];
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < L; k++)
                buf[k] = Op::init();
            V a0 = vx_load(buf), a1 = a0;
            വേണ്ടി (; i + 2*L <= n; i += 2*L)
            {
                a0 = Op::vapply(a0, vx_load(x + i));
                a1 = Op::vapply(a1, vx_load(x + i + L));
            }
            v_store(buf, Op::vcombine(a0, a1));
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < L; k++)
                acc = Op::combine(acc, buf[k]);
        }
        വേണ്ടി (; i < n; i++)
            acc = Op::apply(acc, x[i]);
        തിരികെ acc;
    }
};

template<വർഗ്ഗം Op, typename A> ഘടന VRow<Op, A, സത്യം>
{
    സ്ഥിതം ശൂന്യം run(A* acc, സ്ഥിരം A* x, int64 n)
    {
        തരനിർവചനം typename VecT<A>::vtype V;
        സ്ഥിരം പൂർണ്ണസംഖ്യ L = VTraits<V>::vlanes();
        int64 i = 0;
        വേണ്ടി (; i + L <= n; i += L)
            v_store(acc + i, Op::vapply(vx_load(acc + i), vx_load(x + i)));
        വേണ്ടി (; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i]);
    }
};
#endif

//////////////////////////////// plan ////////////////////////////////

ഘടന Plan
{
    // kept axes (outer first); ksstep in bytes, kdstep in result elements
    പൂർണ്ണസംഖ്യ nk;
    പൂർണ്ണസംഖ്യ ksize[RND_MAX_DIMS];
    ptrdiff_t ksstep[RND_MAX_DIMS], kdstep[RND_MAX_DIMS];
    // reduced axes (outer first), steps in bytes
    പൂർണ്ണസംഖ്യ nr;
    പൂർണ്ണസംഖ്യ rsize[RND_MAX_DIMS];
    ptrdiff_t rsstep[RND_MAX_DIMS];
    തർക്കം innerReduced;      // the innermost axis is reduced (horizontal mode)

    സ്ഥിരം uchar* src;
    പൂർണ്ണസംഖ്യ sdepth, adepth;     // source depth and accumulator depth
    size_t sesz1;
    int64 എണ്ണം_2;            // reduced elements per output
    int64 nout;             // number of outputs
    ദ്വിമാന_Func cvt;         // source -> accumulator conversion, or nullptr
};

// iterator over the flat index space of some axes, tracking a byte offset and an element offset
ഘടന Iter
{
    പൂർണ്ണസംഖ്യ n = 0;
    സ്ഥിരം പൂർണ്ണസംഖ്യ* size = nullptr;
    സ്ഥിരം ptrdiff_t* s = nullptr;
    സ്ഥിരം ptrdiff_t* d = nullptr;
    int64 idx[RND_MAX_DIMS];
    ptrdiff_t soff = 0, doff = 0;

    ശൂന്യം init(പൂർണ്ണസംഖ്യ n_, സ്ഥിരം പൂർണ്ണസംഖ്യ* size_, സ്ഥിരം ptrdiff_t* s_, സ്ഥിരം ptrdiff_t* d_, int64 flat)
    {
        n = n_; size = size_; s = s_; d = d_;
        soff = doff = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = n - 1; k >= 0; k--)
        {
            int64 q = flat / size[k];
            idx[k] = flat - q*size[k];
            soff += (ptrdiff_t)idx[k]*s[k];
            എങ്കിൽ (d)
                doff += (ptrdiff_t)idx[k]*d[k];
            flat = q;
        }
    }

    ശൂന്യം next()
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = n - 1; k >= 0; k--)
        {
            soff += s[k];
            എങ്കിൽ (d)
                doff += d[k];
            എങ്കിൽ (++idx[k] < size[k])
                തിരികെ;
            soff -= (ptrdiff_t)size[k]*s[k];
            എങ്കിൽ (d)
                doff -= (ptrdiff_t)size[k]*d[k];
            idx[k] = 0;
        }
    }
};

// `len` accumulator-typed elements of the source row at `p`: `p` itself when it is already
// contiguous and of the accumulator type, otherwise `tmp`/`buf`.
template<typename A> സ്ഥിതം സ്ഥിരം A* loadRow(സ്ഥിരം Plan& pl, സ്ഥിരം uchar* p, ptrdiff_t step, int64 len,
                                             A* buf, uchar* tmp)
{
    എങ്കിൽ (step != (ptrdiff_t)pl.sesz1)
    {
        വേണ്ടി (int64 i = 0; i < len; i++, p += step)
            memcpy(tmp + i*pl.sesz1, p, pl.sesz1);
        p = tmp;
    }
    എങ്കിൽ (!pl.cvt)
        തിരികെ (സ്ഥിരം A*)p;
    pl.cvt(p, 0, 0, 0, (uchar*)buf, 0, Size((പൂർണ്ണസംഖ്യ)len, 1), 0);
    തിരികെ buf;
}

// Row loop with conversion blocking: calls f(x, len) on consecutive pieces of the row.
template<typename A, typename F> സ്ഥിതം ശൂന്യം forRow(സ്ഥിരം Plan& pl, സ്ഥിരം uchar* p, ptrdiff_t step,
                                                    int64 n, A* buf, uchar* tmp, F f)
{
    സ്ഥിരം തർക്കം direct = !pl.cvt && step == (ptrdiff_t)pl.sesz1;
    സ്ഥിരം int64 blk = direct ? n : (int64)RND_BLOCK;
    വേണ്ടി (int64 e = 0; e < n; e += blk)
    {
        int64 len = std::min(blk, n - e);
        f(loadRow<A>(pl, p + e*step, step, len, buf, tmp), len);
    }
}

//////////////////////////////// drivers ////////////////////////////////

// Parallel-split bookkeeping shared by both modes: nsplit partial results per output.
ഘടന Work
{
    int64 nitems, nsplit;
    ഇരട്ട nstripes;
    തർക്കം serial;
};

// Horizontal mode: outputs are the kept axes, each one folds `count` elements; the flat reduced
// index space [0, count) is split into nsplit ranges when there are too few outputs.
template<typename A, വർഗ്ഗം Op, വർഗ്ഗം OpM> സ്ഥിതം ശൂന്യം runHorizontal(സ്ഥിരം Plan& pl, സ്ഥിരം Work& w, A* res,
                                                                   സ്ഥിരം A* mbuf, A* partial)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ nr = pl.nr;
    സ്ഥിരം int64 n = pl.rsize[nr - 1];                 // innermost reduced axis
    സ്ഥിരം ptrdiff_t rstep = pl.rsstep[nr - 1];
    സ്ഥിരം int64 nsplit = w.nsplit;

    auto body = [&](സ്ഥിരം Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        Iter ko, ro;
        int64 it = range.start;
        എങ്കിൽ (nsplit == 1)
            ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, it);
        ആകുമ്പോൾ (it < range.end)
        {
            സ്ഥിരം int64 o = it / nsplit, s = it - o*nsplit;
            എങ്കിൽ (nsplit > 1)
                ko.init(pl.nk, pl.ksize, pl.ksstep, pl.kdstep, o);
            സ്ഥിരം uchar* base = pl.src + ko.soff;
            സ്ഥിരം A m = mbuf ? mbuf[ko.doff] : A(0);
            int64 a = s*pl.എണ്ണം_2/nsplit, b = (s + 1)*pl.എണ്ണം_2/nsplit;
            A acc = Op::init();
            എങ്കിൽ (a < b)
            {
                ro.init(nr - 1, pl.rsize, pl.rsstep, nullptr, a / n);
                int64 e = a % n;
                ആകുമ്പോൾ (a < b)
                {
                    int64 len = std::min(n - e, b - a);
                    forRow<A>(pl, base + ro.soff + e*rstep, rstep, len, bufA.data(), bufT.data(),
                              [&](സ്ഥിരം A* x, int64 l) { acc = OpM::run(acc, x, l, m); });
                    a += len;
                    e = 0;
                    ro.next();
                }
            }
            എങ്കിൽ (nsplit == 1)
            {
                res[ko.doff] = OpM::finalize(acc, pl.എണ്ണം_2, m);
                ko.next();
            }
            അല്ലെങ്കിൽ
                partial[s*pl.nout + ko.doff] = acc;
            it++;
        }
    };

    എങ്കിൽ (w.serial)
        body(Range(0, (പൂർണ്ണസംഖ്യ)w.nitems));
    അല്ലെങ്കിൽ
        parallel_for_(Range(0, (പൂർണ്ണസംഖ്യ)w.nitems), body, w.nstripes);
}

// Vertical mode: the innermost axis is kept; each item accumulates whole rows of a chunk of it.
template<typename A, വർഗ്ഗം Op, വർഗ്ഗം OpM> സ്ഥിതം ശൂന്യം runVertical(സ്ഥിരം Plan& pl, സ്ഥിരം Work& w, A* res,
                                                                 സ്ഥിരം A* mbuf, A* partial)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ nk = pl.nk;
    സ്ഥിരം int64 m = pl.ksize[nk - 1];
    സ്ഥിരം ptrdiff_t kstep = pl.ksstep[nk - 1], kdstep = pl.kdstep[nk - 1];
    സ്ഥിരം int64 nch = (m + RND_VCHUNK - 1)/RND_VCHUNK;
    സ്ഥിരം int64 nsplit = w.nsplit;
    int64 rcount = 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < pl.nr; i++)
        rcount *= pl.rsize[i];

    auto body = [&](സ്ഥിരം Range& range)
    {
        AutoBuffer<A> bufA(RND_BLOCK), accBuf(RND_VCHUNK);
        AutoBuffer<uchar> bufT(RND_BLOCK*pl.sesz1);
        A* acc = accBuf.data();
        Iter ko, ro;
        വേണ്ടി (int64 it = range.start; it < range.end; it++)
        {
            int64 s = it % nsplit, q = it / nsplit;
            int64 ch = q % nch, o = q / nch;
            ko.init(nk - 1, pl.ksize, pl.ksstep, pl.kdstep, o);
            സ്ഥിരം int64 e0 = ch*RND_VCHUNK, len = std::min((int64)RND_VCHUNK, m - e0);
            സ്ഥിരം ptrdiff_t soff = ko.soff + (ptrdiff_t)e0*kstep, doff = ko.doff + (ptrdiff_t)e0*kdstep;
            സ്ഥിരം A* mrow = mbuf ? mbuf + doff : nullptr;   // kdstep == 1 for the innermost kept axis
            വേണ്ടി (int64 i = 0; i < len; i++)
                acc[i] = Op::init();

            int64 a = s*rcount/nsplit, b = (s + 1)*rcount/nsplit;
            എങ്കിൽ (a < b)
            {
                ro.init(pl.nr, pl.rsize, pl.rsstep, nullptr, a);
                വേണ്ടി (; a < b; a++, ro.next())
                {
                    സ്ഥിരം uchar* p = pl.src + soff + ro.soff;
                    സ്ഥിരം A* x = loadRow<A>(pl, p, kstep, len, bufA.data(), bufT.data());
                    OpM::vrun(acc, x, len, mrow);
                }
            }
            എങ്കിൽ (nsplit == 1)
                വേണ്ടി (int64 i = 0; i < len; i++)
                    res[doff + i] = OpM::finalize(acc[i], pl.എണ്ണം_2, mrow ? mrow[i] : A(0));
            അല്ലെങ്കിൽ
                വേണ്ടി (int64 i = 0; i < len; i++)
                    partial[s*pl.nout + doff + i] = acc[i];
        }
    };

    എങ്കിൽ (w.serial)
        body(Range(0, (പൂർണ്ണസംഖ്യ)w.nitems));
    അല്ലെങ്കിൽ
        parallel_for_(Range(0, (പൂർണ്ണസംഖ്യ)w.nitems), body, w.nstripes);
}

// Adapters that give the plain operations and the shifted-exp pass one interface.
template<typename A, വർഗ്ഗം Op> ഘടന Plain
{
    സ്ഥിതം A run(A acc, സ്ഥിരം A* x, int64 n, A) { തിരികെ HRow<Op, A>::run(acc, x, n); }
    സ്ഥിതം ശൂന്യം vrun(A* acc, സ്ഥിരം A* x, int64 n, സ്ഥിരം A*) { VRow<Op, A>::run(acc, x, n); }
    സ്ഥിതം A finalize(A a, int64 എണ്ണം_2, A) { തിരികെ Op::finalize(a, എണ്ണം_2); }
};

template<typename A> ഘടന ShiftedExp
{
    തരനിർവചനം OpSumExpShifted<A> Op;
    സ്ഥിതം A run(A acc, സ്ഥിരം A* x, int64 n, A m)
    {
        വേണ്ടി (int64 i = 0; i < n; i++)
            acc = Op::apply(acc, x[i], m);
        തിരികെ acc;
    }
    സ്ഥിതം ശൂന്യം vrun(A* acc, സ്ഥിരം A* x, int64 n, സ്ഥിരം A* m)
    {
        വേണ്ടി (int64 i = 0; i < n; i++)
            acc[i] = Op::apply(acc[i], x[i], m[i]);
    }
    സ്ഥിതം A finalize(A a, int64, A m) { തിരികെ Op::finalize(a, m); }
};

template<typename A, വർഗ്ഗം Op, വർഗ്ഗം OpM> സ്ഥിതം ശൂന്യം runPass(സ്ഥിരം Plan& pl, A* res, സ്ഥിരം A* mbuf)
{
    സ്ഥിരം ഇരട്ട work = (ഇരട്ട)pl.nout*(ഇരട്ട)std::max(pl.എണ്ണം_2, (int64)1);
    സ്ഥിരം പൂർണ്ണസംഖ്യ nthreads = work < RND_SERIAL_WORK ? 1 : get_അക്കം_Threads();
    Work w;
    w.serial = nthreads <= 1;
    w.nsplit = 1;
    int64 ഇനങ്ങൾ;
    എങ്കിൽ (pl.innerReduced)
        ഇനങ്ങൾ = pl.nout;
    അല്ലെങ്കിൽ
    {
        int64 nouter = pl.nout/pl.ksize[pl.nk - 1];
        ഇനങ്ങൾ = nouter*((pl.ksize[pl.nk - 1] + RND_VCHUNK - 1)/RND_VCHUNK);
    }
    // too few outputs to keep all the threads busy: split the reduction as well
    സ്ഥിരം int64 want = 4*(int64)nthreads;
    എങ്കിൽ (!w.serial && ഇനങ്ങൾ < want && pl.എണ്ണം_2 > 1)
    {
        int64 rows = pl.innerReduced ? pl.എണ്ണം_2/4096 : pl.എണ്ണം_2/16;
        w.nsplit = std::max((int64)1, std::min((want + ഇനങ്ങൾ - 1)/ഇനങ്ങൾ, std::min(rows, (int64)256)));
        w.nsplit = std::min(w.nsplit, std::max((int64)1, (int64)(4 << 20)/std::max(pl.nout, (int64)1)));
    }
    w.nitems = ഇനങ്ങൾ*w.nsplit;
    w.nstripes = std::min((ഇരട്ട)w.nitems, std::max(1.0, work/RND_STRIPE_WORK));
    CV_Assert(w.nitems <= INT_MAX);

    AutoBuffer<A> partial(w.nsplit > 1 ? (size_t)(w.nsplit*pl.nout) : 1);
    എങ്കിൽ (pl.innerReduced)
        runHorizontal<A, Op, OpM>(pl, w, res, mbuf, partial.data());
    അല്ലെങ്കിൽ
        runVertical<A, Op, OpM>(pl, w, res, mbuf, partial.data());

    എങ്കിൽ (w.nsplit > 1)
    {
        വേണ്ടി (int64 o = 0; o < pl.nout; o++)
        {
            A acc = partial[(size_t)o];
            വേണ്ടി (int64 s = 1; s < w.nsplit; s++)
                acc = Op::combine(acc, partial[(size_t)(s*pl.nout + o)]);
            res[o] = OpM::finalize(acc, pl.എണ്ണം_2, mbuf ? mbuf[o] : A(0));
        }
    }
}

template<typename A> സ്ഥിതം ശൂന്യം fillResult(A* res, int64 n, A v)
{
    വേണ്ടി (int64 i = 0; i < n; i++)
        res[i] = v;
}

template<typename A> സ്ഥിതം ശൂന്യം runOp(സ്ഥിരം Plan& pl, പൂർണ്ണസംഖ്യ op, A* res)
{
    എങ്കിൽ (pl.എണ്ണം_2 == 0)
    {
        // empty reduction: the identity of the operation
        A v = op == REDUCE_MAX ? OpMax<A>::init() : op == REDUCE_MIN ? OpMin<A>::init() :
              op == REDUCE_PROD ? A(1) :
              op == REDUCE_LOG_SUM || op == REDUCE_LOG_SUM_EXP ? OpMax<A>::init() : A(0);
        fillResult(res, pl.nout, v);
        തിരികെ;
    }
    തിരഞ്ഞെടുപ്പ് (op)
    {
    അവസ്ഥ REDUCE_SUM:   runPass<A, OpSum<A>, Plain<A, OpSum<A> > >(pl, res, nullptr); നിർത്തുക;
    അവസ്ഥ REDUCE_AVG:   runPass<A, OpAvg<A>, Plain<A, OpAvg<A> > >(pl, res, nullptr); നിർത്തുക;
    അവസ്ഥ REDUCE_SUM2:  runPass<A, OpSum2<A>, Plain<A, OpSum2<A> > >(pl, res, nullptr); നിർത്തുക;
    അവസ്ഥ REDUCE_L1:    runPass<A, OpL1<A>, Plain<A, OpL1<A> > >(pl, res, nullptr); നിർത്തുക;
    അവസ്ഥ REDUCE_L2:    runPass<A, OpL2<A>, Plain<A, OpL2<A> > >(pl, res, nullptr); നിർത്തുക;
    അവസ്ഥ REDUCE_PROD:  runPass<A, OpProd<A>, Plain<A, OpProd<A> > >(pl, res, nullptr); നിർത്തുക;
    അവസ്ഥ REDUCE_LOG_SUM: runPass<A, OpLogSum<A>, Plain<A, OpLogSum<A> > >(pl, res, nullptr); നിർത്തുക;
    അവസ്ഥ REDUCE_LOG_SUM_EXP:
    {
        AutoBuffer<A> mbuf((size_t)pl.nout);
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, mbuf.data(), nullptr);
        runPass<A, OpSumExpShifted<A>, ShiftedExp<A> >(pl, res, mbuf.data());
        നിർത്തുക;
    }
    സ്ഥിരസ്ഥിതി:
        CV_Error(Error::StsBadArg, "reduce: unknown reduction operation");
    }
}

template<typename A> സ്ഥിതം ശൂന്യം runMinMax(സ്ഥിരം Plan& pl, പൂർണ്ണസംഖ്യ op, A* res)
{
    എങ്കിൽ (pl.എണ്ണം_2 == 0)
        fillResult(res, pl.nout, op == REDUCE_MAX ? OpMax<A>::init() : OpMin<A>::init());
    അല്ലെങ്കിൽ എങ്കിൽ (op == REDUCE_MAX)
        runPass<A, OpMax<A>, Plain<A, OpMax<A> > >(pl, res, nullptr);
    അല്ലെങ്കിൽ
        runPass<A, OpMin<A>, Plain<A, OpMin<A> > >(pl, res, nullptr);
}

സ്ഥിതം ശൂന്യം dispatch(സ്ഥിരം Plan& pl, പൂർണ്ണസംഖ്യ op, uchar* res)
{
    സ്ഥിരം തർക്കം minmax = op == REDUCE_MAX || op == REDUCE_MIN;
    തിരഞ്ഞെടുപ്പ് (pl.adepth)
    {
    അവസ്ഥ CV_32F:
        എങ്കിൽ (minmax) runMinMax<ദശാംശം>(pl, op, (ദശാംശം*)res); അല്ലെങ്കിൽ runOp<ദശാംശം>(pl, op, (ദശാംശം*)res);
        നിർത്തുക;
    അവസ്ഥ CV_64F:
        എങ്കിൽ (minmax) runMinMax<ഇരട്ട>(pl, op, (ഇരട്ട*)res); അല്ലെങ്കിൽ runOp<ഇരട്ട>(pl, op, (ഇരട്ട*)res);
        നിർത്തുക;
    അവസ്ഥ CV_8U:  runMinMax<uchar>(pl, op, (uchar*)res); നിർത്തുക;
    അവസ്ഥ CV_8S:  runMinMax<schar>(pl, op, (schar*)res); നിർത്തുക;
    അവസ്ഥ CV_16U: runMinMax<ushort>(pl, op, (ushort*)res); നിർത്തുക;
    അവസ്ഥ CV_16S: runMinMax<short>(pl, op, (short*)res); നിർത്തുക;
    അവസ്ഥ CV_32U: runMinMax<unsigned>(pl, op, (unsigned*)res); നിർത്തുക;
    അവസ്ഥ CV_32S: runMinMax<പൂർണ്ണസംഖ്യ>(pl, op, (പൂർണ്ണസംഖ്യ*)res); നിർത്തുക;
    അവസ്ഥ CV_64U: runMinMax<uint64_t>(pl, op, (uint64_t*)res); നിർത്തുക;
    അവസ്ഥ CV_64S: runMinMax<int64_t>(pl, op, (int64_t*)res); നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        CV_Error(Error::StsInternal, "reduce: unexpected accumulator depth");
    }
}

സ്ഥിതം പൂർണ്ണസംഖ്യ accumulatorDepth(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth, പൂർണ്ണസംഖ്യ op)
{
    എങ്കിൽ (op == REDUCE_MAX || op == REDUCE_MIN)
    {
        എങ്കിൽ (sdepth == CV_16F || sdepth == CV_16BF)
            തിരികെ CV_32F;
        തിരികെ sdepth == CV_Bool ? CV_8U : sdepth;
    }
    സ്ഥിരം തർക്കം wide = sdepth == CV_32S || sdepth == CV_32U || sdepth == CV_64S || sdepth == CV_64U ||
                      sdepth == CV_64F || ddepth == CV_64F;
    തിരികെ wide ? CV_64F : CV_32F;
}

} // namespace

ReduceParams::ReduceParams(പൂർണ്ണസംഖ്യ _rtype, സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& _axes, തർക്കം _keepdims, പൂർണ്ണസംഖ്യ _dtype)
    : rtype(_rtype), axes(_axes), keepdims(_keepdims), dtype(_dtype)
{
}

ശൂന്യം reduce(നിവേശം_Array _src, OutputArray _dst, സ്ഥിരം ReduceParams& params)
{
    CV_INSTRUMENT_REGION();

    സ്ഥിരം പൂർണ്ണസംഖ്യ op = params.rtype, dtype = params.dtype;
    സ്ഥിരം തർക്കം keepdims = params.keepdims;
    CV_Check(op, op >= REDUCE_SUM && op <= REDUCE_LOG_SUM_EXP, "reduce: unknown reduction operation");

    Mat src = _src.getMat();
    സ്ഥിരം പൂർണ്ണസംഖ്യ dims = src.dims, cn = src.channels();
    സ്ഥിരം പൂർണ്ണസംഖ്യ sdepth = src.depth();
    പൂർണ്ണസംഖ്യ ddepth = dtype < 0 ? sdepth : CV_MAT_DEPTH(dtype);
    എങ്കിൽ ((op == REDUCE_MAX || op == REDUCE_MIN) && sdepth == CV_Bool && ddepth != CV_Bool)
        CV_Error(Error::StsBadArg, "reduce: the output of max/min over bool must be bool");

    // which axes are reduced; empty means all of them
    തർക്കം reduced[CV_MAX_DIM] = {അസത്യം};
    എങ്കിൽ (params.axes.empty())
        std::fill(reduced, reduced + dims, സത്യം);
    വേണ്ടി (പൂർണ്ണസംഖ്യ a : params.axes)
    {
        CV_CheckGE(a, -dims, "reduce: axis is out of range");
        CV_CheckLT(a, dims, "reduce: axis is out of range");
        പൂർണ്ണസംഖ്യ ax = a < 0 ? a + dims : a;
        CV_Check(a, !reduced[ax], "reduce: duplicate axis");
        reduced[ax] = സത്യം;
    }

    പൂർണ്ണസംഖ്യ keepShape[CV_MAX_DIM], outShape[CV_MAX_DIM], nout = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
    {
        keepShape[i] = reduced[i] ? 1 : src.size[i];
        എങ്കിൽ (!reduced[i] || keepdims)
            outShape[nout++] = keepShape[i];
    }
    സ്ഥിരം പൂർണ്ണസംഖ്യ dtype_ = CV_MAKETYPE(ddepth, cn);
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
    പൂർണ്ണസംഖ്യ n = 0;
    പൂർണ്ണസംഖ്യ size[RND_MAX_DIMS];
    ptrdiff_t sstep[RND_MAX_DIMS], dstep[RND_MAX_DIMS];
    തർക്കം red[RND_MAX_DIMS];
    ptrdiff_t dcur = cn;
    ptrdiff_t dsteps[CV_MAX_DIM];
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = dims - 1; i >= 0; i--)
    {
        dsteps[i] = dcur;
        dcur *= keepShape[i];
    }
    pl.എണ്ണം_2 = 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
    {
        എങ്കിൽ (reduced[i])
            pl.എണ്ണം_2 *= src.size[i];
        size[n] = src.size[i];
        sstep[n] = (ptrdiff_t)src.step[i];
        dstep[n] = reduced[i] ? 0 : dsteps[i];
        red[n] = reduced[i];
        n++;
    }
    എങ്കിൽ (cn > 1)
    {
        size[n] = cn;
        sstep[n] = (ptrdiff_t)pl.sesz1;
        dstep[n] = 1;
        red[n] = അസത്യം;
        n++;
    }

    pl.nout = (int64)dst.total()*cn;
    സ്ഥിരം തർക്കം direct = pl.adepth == ddepth && dst.isContinuous();
    Mat resMat;
    uchar* res = dst.data;
    എങ്കിൽ (!direct)
    {
        resMat.create(1, (പൂർണ്ണസംഖ്യ)std::max(pl.nout, (int64)1), CV_MAKETYPE(pl.adepth, 1));
        res = resMat.data;
    }
    എങ്കിൽ (pl.nout == 0)
        തിരികെ;

    // drop size-1 axes, then merge neighbours of the same kind that are contiguous in both views
    പൂർണ്ണസംഖ്യ m = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
        എങ്കിൽ (size[i] == 0)
        {
            // empty source along this axis
            എങ്കിൽ (red[i])
            {
                pl.എണ്ണം_2 = 0;
                തുടരുക;
            }
            തിരികെ;
        }
        എങ്കിൽ (size[i] == 1)
            തുടരുക;
        എങ്കിൽ (m > 0 && red[m-1] == red[i] && sstep[m-1] == sstep[i]*size[i] &&
            dstep[m-1] == dstep[i]*size[i])
        {
            size[m-1] *= size[i];
            sstep[m-1] = sstep[i];
            dstep[m-1] = dstep[i];
        }
        അല്ലെങ്കിൽ
        {
            size[m] = size[i];
            sstep[m] = sstep[i];
            dstep[m] = dstep[i];
            red[m] = red[i];
            m++;
        }
    }
    എങ്കിൽ (m == 0)
    {
        // a single element
        size[0] = 1;
        sstep[0] = (ptrdiff_t)pl.sesz1;
        dstep[0] = 1;
        red[0] = അസത്യം;
        m = 1;
    }
    pl.innerReduced = red[m-1];
    pl.nk = pl.nr = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < m; i++)
    {
        എങ്കിൽ (red[i])
        {
            pl.rsize[pl.nr] = size[i];
            pl.rsstep[pl.nr] = sstep[i];
            pl.nr++;
        }
        അല്ലെങ്കിൽ
        {
            pl.ksize[pl.nk] = size[i];
            pl.ksstep[pl.nk] = sstep[i];
            pl.kdstep[pl.nk] = dstep[i];
            pl.nk++;
        }
    }
    എങ്കിൽ (pl.nk == 0)
    {
        pl.ksize[0] = 1;
        pl.ksstep[0] = 0;
        pl.kdstep[0] = 0;
        pl.nk = 1;
    }

    dispatch(pl, op, res);

    എങ്കിൽ (!direct)
    {
        Mat r(dst.dims, dst.size.p, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        എങ്കിൽ (dst.dims == 0)
            r = Mat(0, nullptr, CV_MAKETYPE(pl.adepth, cn), resMat.data);
        r.convertTo(dst, ddepth);
    }
}

} // namespace cv
