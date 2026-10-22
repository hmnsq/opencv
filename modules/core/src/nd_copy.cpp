// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "nd_copy.hpp"
#അന്തര്ഭാവ "hal_replacement.hpp"

#അന്തര്ഭാവ "opencv2/core/hal/hal.hpp"

#അന്തര്ഭാവ <algorithm>

നാമാകാശ cv { നാമാകാശ nd {

View viewOf(സ്ഥിരം Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    തിരികെ v;
}

View permute(സ്ഥിരം View& v, സ്ഥിരം പൂർണ്ണസംഖ്യ* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < v.dims; i++)
    {
        പൂർണ്ണസംഖ്യ j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    തിരികെ r;
}

ശൂന്യം flip(View& v, പൂർണ്ണസംഖ്യ axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    എങ്കിൽ (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

ശൂന്യം slice(View& v, പൂർണ്ണസംഖ്യ axis, പൂർണ്ണസംഖ്യ start, പൂർണ്ണസംഖ്യ step, പൂർണ്ണസംഖ്യ എണ്ണം_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && എണ്ണം_2 >= 0);
    എങ്കിൽ (എണ്ണം_2 > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = എണ്ണം_2;
}

നാമാകാശ {

എണ്ണൽ Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

എണ്ണൽ
{
    TRANSPOSE_TILE = 64,                // in elements
    SPLIT_CHUNK = 1024,                 // in elements
    ROW_CHUNK_BYTES = 64*1024,
    GATHER_BLOCK_BYTES = 8*1024,
    // costs are bytes weighted by the kernel; the copies are memory-bound,
    // so small stripes only add overhead
    PARALLEL_MIN_COST = 1024*1024,
    STRIPE_COST = 1024*1024
};

ഘടന Plan
{
    സ്ഥിരം uchar* src;
    uchar* dst;
    size_t esz;
    പൂർണ്ണസംഖ്യ dims;
    പൂർണ്ണസംഖ്യ kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    തർക്കം chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    ഇരട്ട cost;
};

സ്ഥിതം ശൂന്യം swapAxes(Plan& p, പൂർണ്ണസംഖ്യ i, പൂർണ്ണസംഖ്യ j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

സ്ഥിതം ശൂന്യം moveAxis(Plan& p, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ to)
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
സ്ഥിതം size_t addressBits(സ്ഥിരം Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    തിരികെ addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
സ്ഥിതം തർക്കം trySplitMerge(Plan& p)
{
    സ്ഥിരം ptrdiff_t esz = (ptrdiff_t)p.esz;
    സ്ഥിരം പൂർണ്ണസംഖ്യ L = p.dims - 1;
    എങ്കിൽ (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        തിരികെ അസത്യം;
    എങ്കിൽ ((addressBits(p) & (esz - 1)) != 0)
        തിരികെ അസത്യം;

    // split: axis k holds cn planes interleaved in the source
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = L - 1; k >= 0; k--)
    {
        സ്ഥിരം int64 cn = p.size[k];
        എങ്കിൽ (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            തിരികെ സത്യം;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    സ്ഥിരം int64 cn = p.size[L];
    എങ്കിൽ (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = L - 1; k >= 0; k--)
        {
            എങ്കിൽ (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                തിരികെ സത്യം;
            }
        }
    }
    തിരികെ അസത്യം;
}

// Returns false when there is nothing to copy.
സ്ഥിതം തർക്കം buildPlan(സ്ഥിരം View& s, സ്ഥിരം View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    സ്ഥിരം ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    പൂർണ്ണസംഖ്യ n = 0;
    ഇരട്ട total = 1;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        പൂർണ്ണസംഖ്യ sz = s.size[i];
        CV_Assert(sz >= 0);
        എങ്കിൽ (sz == 0)
            തിരികെ അസത്യം;
        total *= sz;
        എങ്കിൽ (sz == 1)
            തുടരുക;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        എങ്കിൽ (ds < 0)
        {
            p.src += (ptrdiff_t)(sz - 1)*ss;
            p.dst += (ptrdiff_t)(sz - 1)*ds;
            ss = -ss;
            ds = -ds;
        }
        p.size[n] = sz;
        p.sstep[n] = ss;
        p.dstep[n] = ds;
        n++;
    }

    // Write the destination as sequentially as possible.
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < n; i++)
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    പൂർണ്ണസംഖ്യ m = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
        എങ്കിൽ (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        അല്ലെങ്കിൽ
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    എങ്കിൽ (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    സ്ഥിരം പൂർണ്ണസംഖ്യ L = m - 1;
    p.tfunc = nullptr;
    എങ്കിൽ (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    അല്ലെങ്കിൽ എങ്കിൽ (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    അല്ലെങ്കിൽ എങ്കിൽ (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        എങ്കിൽ (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            പൂർണ്ണസംഖ്യ k = L - 1;
            ആകുമ്പോൾ (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            സ്ഥിരം size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            എങ്കിൽ (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    സ്ഥിരം ഇരട്ട bytes = total*(ഇരട്ട)esz;
    എങ്കിൽ (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = അസത്യം;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = അസത്യം;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    അല്ലെങ്കിൽ
    {
        int64 nrows = 1;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < L; i++)
            nrows *= p.size[i];
        സ്ഥിരം int64 rowLen = p.size[L];
        സ്ഥിരം ഇരട്ട rowBytes = (ഇരട്ട)rowLen*esz;
        സ്ഥിരം ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = അസത്യം;
        എങ്കിൽ (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (ഇരട്ട)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = സത്യം;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
        {
            // few long rows: split them to have something to parallelize
            int64 nch = std::min((int64)(rowBytes/ROW_CHUNK_BYTES), (int64)1024);
            p.chunk = (rowLen + nch - 1)/nch;
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
        }
        p.tilesA = p.tilesB = 1;
        p.nitems = nrows*p.nchunks;
        p.cost = p.kind == KIND_MEMCPY ? bytes : bytes*4;
    }
    തിരികെ സത്യം;
}

template<typename T> സ്ഥിതം വരിയിൽ തർക്കം isAligned(സ്ഥിരം ശൂന്യം* s, സ്ഥിരം ശൂന്യം* d, ptrdiff_t ss, ptrdiff_t ds)
{
    തിരികെ (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> സ്ഥിതം ശൂന്യം copyStrided_(സ്ഥിരം uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    വേണ്ടി (; i + 4 <= n; i += 4)
    {
        T t0 = *(സ്ഥിരം T*)s, t1 = *(സ്ഥിരം T*)(s + ss);
        T t2 = *(സ്ഥിരം T*)(s + ss*2), t3 = *(സ്ഥിരം T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    വേണ്ടി (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(സ്ഥിരം T*)s;
}

template<typename T> സ്ഥിതം ശൂന്യം fill_(സ്ഥിരം uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    സ്ഥിരം T v = *(സ്ഥിരം T*)s;
    വേണ്ടി (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

ഘടന Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
സ്ഥിതം ശൂന്യം copyRow(സ്ഥിരം Plan& p, സ്ഥിരം uchar* s, uchar* d, int64 n)
{
    സ്ഥിരം size_t esz = p.esz;
    സ്ഥിരം പൂർണ്ണസംഖ്യ L = p.dims - 1;
    സ്ഥിരം ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    എങ്കിൽ (p.kind == KIND_FILL)
    {
        എങ്കിൽ (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        അല്ലെങ്കിൽ എങ്കിൽ (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        അല്ലെങ്കിൽ എങ്കിൽ (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        അല്ലെങ്കിൽ എങ്കിൽ (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        അല്ലെങ്കിൽ
            വേണ്ടി (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        തിരികെ;
    }

    എങ്കിൽ (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    അല്ലെങ്കിൽ എങ്കിൽ (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    അല്ലെങ്കിൽ എങ്കിൽ (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    അല്ലെങ്കിൽ എങ്കിൽ (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    അല്ലെങ്കിൽ എങ്കിൽ (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    അല്ലെങ്കിൽ
        വേണ്ടി (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
സ്ഥിതം ശൂന്യം decode(സ്ഥിരം Plan& p, പൂർണ്ണസംഖ്യ naxes, int64 idx, int64* counters,
                   സ്ഥിരം uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        എങ്കിൽ (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<തർക്കം MEMCPY> സ്ഥിതം വരിയിൽ ശൂന്യം copyRowT(സ്ഥിരം Plan& p, സ്ഥിരം uchar* s, uchar* d, int64 n)
{
    എങ്കിൽ (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    അല്ലെങ്കിൽ
        copyRow(p, s, d, n);
}

template<തർക്കം MEMCPY> സ്ഥിതം ശൂന്യം runRows(സ്ഥിരം Plan& p, int64 i0, int64 i1)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ L = p.dims - 1;
    സ്ഥിരം int64 nch = p.nchunks, rowLen = p.size[L];
    സ്ഥിരം ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    സ്ഥിരം uchar* s;
    uchar* d;

    എങ്കിൽ (p.chunkMajor)
    {
        വേണ്ടി (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        തിരികെ;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    എങ്കിൽ (nch == 1 && L == 1)
    {
        സ്ഥിരം ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        വേണ്ടി (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        തിരികെ;
    }

    വേണ്ടി (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        എങ്കിൽ (chEnd < nch)
            നിർത്തുക;
        ch = 0;
        വേണ്ടി (പൂർണ്ണസംഖ്യ k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            എങ്കിൽ (++counters[k] < p.size[k])
                നിർത്തുക;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

സ്ഥിതം ശൂന്യം runTranspose(സ്ഥിരം Plan& p, int64 i0, int64 i1)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ A = p.dims - 2, B = p.dims - 1;
    സ്ഥിരം int64 ntiles = p.tilesA*p.tilesB;
    വേണ്ടി (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        സ്ഥിരം uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        പൂർണ്ണസംഖ്യ wa = (പൂർണ്ണസംഖ്യ)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        പൂർണ്ണസംഖ്യ hb = (പൂർണ്ണസംഖ്യ)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        എങ്കിൽ (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (പൂർണ്ണസംഖ്യ)p.esz) == CV_HAL_ERROR_OK)
            തുടരുക;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

സ്ഥിതം ശൂന്യം runSplitMerge(സ്ഥിരം Plan& p, int64 i0, int64 i1)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ L = p.dims - 1;
    സ്ഥിരം തർക്കം isSplit = p.kind == KIND_SPLIT;
    സ്ഥിരം പൂർണ്ണസംഖ്യ C = isSplit ? L - 1 : L;    // the plane axis
    സ്ഥിരം പൂർണ്ണസംഖ്യ X = isSplit ? L : L - 1;    // the long axis
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = (പൂർണ്ണസംഖ്യ)p.size[C];
    സ്ഥിരം size_t esz = p.esz;
    വേണ്ടി (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        സ്ഥിരം uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        സ്ഥിരം int64 e0 = ch*p.chunk;
        സ്ഥിരം പൂർണ്ണസംഖ്യ len = (പൂർണ്ണസംഖ്യ)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        എങ്കിൽ (isSplit)
        {
            uchar* dptr[4];
            വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            എങ്കിൽ (esz == 1) hal::split8u(s, dptr, len, cn);
            അല്ലെങ്കിൽ എങ്കിൽ (esz == 2) hal::split16u((സ്ഥിരം ushort*)s, (ushort**)dptr, len, cn);
            അല്ലെങ്കിൽ എങ്കിൽ (esz == 4) hal::split32s((സ്ഥിരം പൂർണ്ണസംഖ്യ*)s, (പൂർണ്ണസംഖ്യ**)dptr, len, cn);
            അല്ലെങ്കിൽ hal::split64s((സ്ഥിരം int64*)s, (int64**)dptr, len, cn);
        }
        അല്ലെങ്കിൽ
        {
            സ്ഥിരം uchar* sptr[4];
            വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            എങ്കിൽ (esz == 1) hal::merge8u(sptr, d, len, cn);
            അല്ലെങ്കിൽ എങ്കിൽ (esz == 2) hal::merge16u((സ്ഥിരം ushort**)sptr, (ushort*)d, len, cn);
            അല്ലെങ്കിൽ എങ്കിൽ (esz == 4) hal::merge32s((സ്ഥിരം പൂർണ്ണസംഖ്യ**)sptr, (പൂർണ്ണസംഖ്യ*)d, len, cn);
            അല്ലെങ്കിൽ hal::merge64s((സ്ഥിരം int64**)sptr, (int64*)d, len, cn);
        }
    }
}

സ്ഥിതം ശൂന്യം runPlan(സ്ഥിരം Plan& p, int64 i0, int64 i1)
{
    എങ്കിൽ (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    അല്ലെങ്കിൽ എങ്കിൽ (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    അല്ലെങ്കിൽ എങ്കിൽ (p.kind == KIND_MEMCPY)
        runRows<സത്യം>(p, i0, i1);
    അല്ലെങ്കിൽ
        runRows<അസത്യം>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
സ്ഥിതം തർക്കം canRunInLockstep(സ്ഥിരം Plan* plans, size_t np)
{
    എങ്കിൽ (np < 2)
        തിരികെ അസത്യം;
    സ്ഥിരം Plan& p0 = plans[0];
    വേണ്ടി (size_t k = 0; k < np; k++)
    {
        സ്ഥിരം Plan& p = plans[k];
        എങ്കിൽ (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            തിരികെ അസത്യം;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < p.dims - 1; i++)
            എങ്കിൽ (p.size[i] != p0.size[i])
                തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
}

സ്ഥിതം ശൂന്യം runPlans(സ്ഥിരം Plan* plans, size_t np)
{
    എങ്കിൽ (np == 0)
        തിരികെ;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    ഇരട്ട cost = 0, rowBytes = 0;
    വേണ്ടി (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (ഇരട്ട)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    സ്ഥിരം തർക്കം lockstep = canRunInLockstep(plans, np);
    സ്ഥിരം int64 total = lockstep ? plans[0].nrows : ofs[np];
    സ്ഥിരം int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        എങ്കിൽ (lockstep)
        {
            വേണ്ടി (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                വേണ്ടി (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            തിരികെ;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        ആകുമ്പോൾ (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    എങ്കിൽ (cost < PARALLEL_MIN_COST || total < 2 || get_അക്കം_Threads() <= 1)
    {
        body(0, total);
        തിരികെ;
    }

    // grouped so that the range fits into int
    സ്ഥിരം int64 ngroups = std::min(total, (int64)1 << 20);
    സ്ഥിരം ഇരട്ട nstripes = std::min((ഇരട്ട)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (പൂർണ്ണസംഖ്യ)ngroups), [&](സ്ഥിരം Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
സ്ഥിതം ശൂന്യം viewRange(സ്ഥിരം View& v, സ്ഥിരം uchar*& lo, സ്ഥിരം uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        എങ്കിൽ (ext < 0) a += ext; അല്ലെങ്കിൽ b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

സ്ഥിതം തർക്കം hasZeroSize(സ്ഥിരം View& v)
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < v.dims; i++)
        എങ്കിൽ (v.size[i] == 0)
            തിരികെ സത്യം;
    തിരികെ അസത്യം;
}

സ്ഥിതം തർക്കം sameMapping(സ്ഥിരം View& s, സ്ഥിരം View& d)
{
    എങ്കിൽ (s.data != d.data)
        തിരികെ അസത്യം;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < s.dims; i++)
        എങ്കിൽ (s.size[i] > 1 && s.step[i] != d.step[i])
            തിരികെ അസത്യം;
    തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം overlap(സ്ഥിരം View& s, സ്ഥിരം View& d)
{
    സ്ഥിരം uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    തിരികെ slo < dhi && dlo < shi;
}

} // namespace

ശൂന്യം copyBatch(സ്ഥിരം View* src, സ്ഥിരം View* dst, പൂർണ്ണസംഖ്യ n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
        View s = src[i];
        സ്ഥിരം View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (പൂർണ്ണസംഖ്യ)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        എങ്കിൽ (hasZeroSize(s) || sameMapping(s, d))
            തുടരുക;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        തർക്കം needTemp = അസത്യം;
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        എങ്കിൽ (needTemp)
        {
            size_t total = 1;
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        എങ്കിൽ (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

ശൂന്യം copy(സ്ഥിരം View& src, സ്ഥിരം View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
