// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "nd_copy.hpp"
#અન્તર્ભાવ "hal_replacement.hpp"

#અન્તર્ભાવ "opencv2/core/hal/hal.hpp"

#અન્તર્ભાવ <algorithm>

નામાકાશ cv { નામાકાશ nd {

View viewOf(અચલ Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    માટે (પૂર્ણાંક i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    પાછા v;
}

View permute(અચલ View& v, અચલ પૂર્ણાંક* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    માટે (પૂર્ણાંક i = 0; i < v.dims; i++)
    {
        પૂર્ણાંક j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    પાછા r;
}

શૂન્ય flip(View& v, પૂર્ણાંક axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    જો (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

શૂન્ય slice(View& v, પૂર્ણાંક axis, પૂર્ણાંક start, પૂર્ણાંક step, પૂર્ણાંક ગણતરી_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && ગણતરી_2 >= 0);
    જો (ગણતરી_2 > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = ગણતરી_2;
}

નામાકાશ {

ગણના Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

ગણના
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

સંરચના Plan
{
    અચલ uchar* src;
    uchar* dst;
    size_t esz;
    પૂર્ણાંક dims;
    પૂર્ણાંક kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    તાર્કિક chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    દ્વિગુણ cost;
};

સ્થિર શૂન્ય swapAxes(Plan& p, પૂર્ણાંક i, પૂર્ણાંક j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

સ્થિર શૂન્ય moveAxis(Plan& p, પૂર્ણાંક k, પૂર્ણાંક to)
{
    માટે (પૂર્ણાંક i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
સ્થિર size_t addressBits(અચલ Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    માટે (પૂર્ણાંક i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    પાછા addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
સ્થિર તાર્કિક trySplitMerge(Plan& p)
{
    અચલ ptrdiff_t esz = (ptrdiff_t)p.esz;
    અચલ પૂર્ણાંક L = p.dims - 1;
    જો (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        પાછા ખોટું;
    જો ((addressBits(p) & (esz - 1)) != 0)
        પાછા ખોટું;

    // split: axis k holds cn planes interleaved in the source
    માટે (પૂર્ણાંક k = L - 1; k >= 0; k--)
    {
        અચલ int64 cn = p.size[k];
        જો (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            પાછા સાચું;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    અચલ int64 cn = p.size[L];
    જો (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        માટે (પૂર્ણાંક k = L - 1; k >= 0; k--)
        {
            જો (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                પાછા સાચું;
            }
        }
    }
    પાછા ખોટું;
}

// Returns false when there is nothing to copy.
સ્થિર તાર્કિક buildPlan(અચલ View& s, અચલ View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    અચલ ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    પૂર્ણાંક n = 0;
    દ્વિગુણ total = 1;
    માટે (પૂર્ણાંક i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        પૂર્ણાંક sz = s.size[i];
        CV_Assert(sz >= 0);
        જો (sz == 0)
            પાછા ખોટું;
        total *= sz;
        જો (sz == 1)
            ચાલુ;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        જો (ds < 0)
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
    માટે (પૂર્ણાંક i = 1; i < n; i++)
        માટે (પૂર્ણાંક j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    પૂર્ણાંક m = 0;
    માટે (પૂર્ણાંક i = 0; i < n; i++)
    {
        જો (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        નહીંતર
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    જો (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    અચલ પૂર્ણાંક L = m - 1;
    p.tfunc = nullptr;
    જો (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    નહીંતર જો (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    નહીંતર જો (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        જો (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            પૂર્ણાંક k = L - 1;
            જ્યારે (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            અચલ size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            જો (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    અચલ દ્વિગુણ bytes = total*(દ્વિગુણ)esz;
    જો (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        અચલ પૂર્ણાંક X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        માટે (પૂર્ણાંક i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = ખોટું;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    નહીંતર જો (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        માટે (પૂર્ણાંક i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = ખોટું;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    નહીંતર
    {
        int64 nrows = 1;
        માટે (પૂર્ણાંક i = 0; i < L; i++)
            nrows *= p.size[i];
        અચલ int64 rowLen = p.size[L];
        અચલ દ્વિગુણ rowBytes = (દ્વિગુણ)rowLen*esz;
        અચલ ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = ખોટું;
        જો (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (દ્વિગુણ)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = સાચું;
        }
        નહીંતર જો (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    પાછા સાચું;
}

template<typename T> સ્થિર અંતઃરેખ તાર્કિક isAligned(અચલ શૂન્ય* s, અચલ શૂન્ય* d, ptrdiff_t ss, ptrdiff_t ds)
{
    પાછા (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> સ્થિર શૂન્ય copyStrided_(અચલ uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    માટે (; i + 4 <= n; i += 4)
    {
        T t0 = *(અચલ T*)s, t1 = *(અચલ T*)(s + ss);
        T t2 = *(અચલ T*)(s + ss*2), t3 = *(અચલ T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    માટે (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(અચલ T*)s;
}

template<typename T> સ્થિર શૂન્ય fill_(અચલ uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    અચલ T v = *(અચલ T*)s;
    માટે (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

સંરચના Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
સ્થિર શૂન્ય copyRow(અચલ Plan& p, અચલ uchar* s, uchar* d, int64 n)
{
    અચલ size_t esz = p.esz;
    અચલ પૂર્ણાંક L = p.dims - 1;
    અચલ ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    જો (p.kind == KIND_FILL)
    {
        જો (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        નહીંતર જો (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        નહીંતર જો (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        નહીંતર જો (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        નહીંતર
            માટે (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        પાછા;
    }

    જો (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    નહીંતર જો (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    નહીંતર જો (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    નહીંતર જો (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    નહીંતર જો (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    નહીંતર
        માટે (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
સ્થિર શૂન્ય decode(અચલ Plan& p, પૂર્ણાંક naxes, int64 idx, int64* counters,
                   અચલ uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    માટે (પૂર્ણાંક k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        જો (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<તાર્કિક MEMCPY> સ્થિર અંતઃરેખ શૂન્ય copyRowT(અચલ Plan& p, અચલ uchar* s, uchar* d, int64 n)
{
    જો (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    નહીંતર
        copyRow(p, s, d, n);
}

template<તાર્કિક MEMCPY> સ્થિર શૂન્ય runRows(અચલ Plan& p, int64 i0, int64 i1)
{
    અચલ પૂર્ણાંક L = p.dims - 1;
    અચલ int64 nch = p.nchunks, rowLen = p.size[L];
    અચલ ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    અચલ uchar* s;
    uchar* d;

    જો (p.chunkMajor)
    {
        માટે (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        પાછા;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    જો (nch == 1 && L == 1)
    {
        અચલ ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        માટે (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        પાછા;
    }

    માટે (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        જો (chEnd < nch)
            તોડો;
        ch = 0;
        માટે (પૂર્ણાંક k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            જો (++counters[k] < p.size[k])
                તોડો;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

સ્થિર શૂન્ય runTranspose(અચલ Plan& p, int64 i0, int64 i1)
{
    અચલ પૂર્ણાંક A = p.dims - 2, B = p.dims - 1;
    અચલ int64 ntiles = p.tilesA*p.tilesB;
    માટે (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        અચલ uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        પૂર્ણાંક wa = (પૂર્ણાંક)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        પૂર્ણાંક hb = (પૂર્ણાંક)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        જો (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (પૂર્ણાંક)p.esz) == CV_HAL_ERROR_OK)
            ચાલુ;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

સ્થિર શૂન્ય runSplitMerge(અચલ Plan& p, int64 i0, int64 i1)
{
    અચલ પૂર્ણાંક L = p.dims - 1;
    અચલ તાર્કિક isSplit = p.kind == KIND_SPLIT;
    અચલ પૂર્ણાંક C = isSplit ? L - 1 : L;    // the plane axis
    અચલ પૂર્ણાંક X = isSplit ? L : L - 1;    // the long axis
    અચલ પૂર્ણાંક cn = (પૂર્ણાંક)p.size[C];
    અચલ size_t esz = p.esz;
    માટે (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        અચલ uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        અચલ int64 e0 = ch*p.chunk;
        અચલ પૂર્ણાંક len = (પૂર્ણાંક)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        જો (isSplit)
        {
            uchar* dptr[4];
            માટે (પૂર્ણાંક c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            જો (esz == 1) hal::split8u(s, dptr, len, cn);
            નહીંતર જો (esz == 2) hal::split16u((અચલ ushort*)s, (ushort**)dptr, len, cn);
            નહીંતર જો (esz == 4) hal::split32s((અચલ પૂર્ણાંક*)s, (પૂર્ણાંક**)dptr, len, cn);
            નહીંતર hal::split64s((અચલ int64*)s, (int64**)dptr, len, cn);
        }
        નહીંતર
        {
            અચલ uchar* sptr[4];
            માટે (પૂર્ણાંક c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            જો (esz == 1) hal::merge8u(sptr, d, len, cn);
            નહીંતર જો (esz == 2) hal::merge16u((અચલ ushort**)sptr, (ushort*)d, len, cn);
            નહીંતર જો (esz == 4) hal::merge32s((અચલ પૂર્ણાંક**)sptr, (પૂર્ણાંક*)d, len, cn);
            નહીંતર hal::merge64s((અચલ int64**)sptr, (int64*)d, len, cn);
        }
    }
}

સ્થિર શૂન્ય runPlan(અચલ Plan& p, int64 i0, int64 i1)
{
    જો (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    નહીંતર જો (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    નહીંતર જો (p.kind == KIND_MEMCPY)
        runRows<સાચું>(p, i0, i1);
    નહીંતર
        runRows<ખોટું>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
સ્થિર તાર્કિક canRunInLockstep(અચલ Plan* plans, size_t np)
{
    જો (np < 2)
        પાછા ખોટું;
    અચલ Plan& p0 = plans[0];
    માટે (size_t k = 0; k < np; k++)
    {
        અચલ Plan& p = plans[k];
        જો (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            પાછા ખોટું;
        માટે (પૂર્ણાંક i = 0; i < p.dims - 1; i++)
            જો (p.size[i] != p0.size[i])
                પાછા ખોટું;
    }
    પાછા સાચું;
}

સ્થિર શૂન્ય runPlans(અચલ Plan* plans, size_t np)
{
    જો (np == 0)
        પાછા;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    દ્વિગુણ cost = 0, rowBytes = 0;
    માટે (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (દ્વિગુણ)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    અચલ તાર્કિક lockstep = canRunInLockstep(plans, np);
    અચલ int64 total = lockstep ? plans[0].nrows : ofs[np];
    અચલ int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        જો (lockstep)
        {
            માટે (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                માટે (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            પાછા;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        જ્યારે (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    જો (cost < PARALLEL_MIN_COST || total < 2 || get_અંક_Threads() <= 1)
    {
        body(0, total);
        પાછા;
    }

    // grouped so that the range fits into int
    અચલ int64 ngroups = std::min(total, (int64)1 << 20);
    અચલ દ્વિગુણ nstripes = std::min((દ્વિગુણ)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (પૂર્ણાંક)ngroups), [&](અચલ Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
સ્થિર શૂન્ય viewRange(અચલ View& v, અચલ uchar*& lo, અચલ uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    માટે (પૂર્ણાંક i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        જો (ext < 0) a += ext; નહીંતર b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

સ્થિર તાર્કિક hasZeroSize(અચલ View& v)
{
    માટે (પૂર્ણાંક i = 0; i < v.dims; i++)
        જો (v.size[i] == 0)
            પાછા સાચું;
    પાછા ખોટું;
}

સ્થિર તાર્કિક sameMapping(અચલ View& s, અચલ View& d)
{
    જો (s.data != d.data)
        પાછા ખોટું;
    માટે (પૂર્ણાંક i = 0; i < s.dims; i++)
        જો (s.size[i] > 1 && s.step[i] != d.step[i])
            પાછા ખોટું;
    પાછા સાચું;
}

સ્થિર તાર્કિક overlap(અચલ View& s, અચલ View& d)
{
    અચલ uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    પાછા slo < dhi && dlo < shi;
}

} // namespace

શૂન્ય copyBatch(અચલ View* src, અચલ View* dst, પૂર્ણાંક n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    માટે (પૂર્ણાંક i = 0; i < n; i++)
    {
        View s = src[i];
        અચલ View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (પૂર્ણાંક)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        જો (hasZeroSize(s) || sameMapping(s, d))
            ચાલુ;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        તાર્કિક needTemp = ખોટું;
        માટે (પૂર્ણાંક j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        જો (needTemp)
        {
            size_t total = 1;
            માટે (પૂર્ણાંક k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            માટે (પૂર્ણાંક k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        જો (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

શૂન્ય copy(અચલ View& src, અચલ View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
