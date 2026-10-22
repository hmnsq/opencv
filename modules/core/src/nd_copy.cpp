// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "nd_copy.hpp"
#ಅನ್ತರ್ಭಾವ "hal_replacement.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/hal.hpp"

#ಅನ್ತರ್ಭಾವ <algorithm>

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ nd {

View viewOf(ಧ್ರುವ Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    ಪ್ರತ್ಯಾಗಮ v;
}

View permute(ಧ್ರುವ View& v, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < v.dims; i++)
    {
        ಪೂರ್ಣಾಙ್ಕ j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    ಪ್ರತ್ಯಾಗಮ r;
}

ಶೂನ್ಯ flip(View& v, ಪೂರ್ಣಾಙ್ಕ axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    ಯದಿ (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

ಶೂನ್ಯ slice(View& v, ಪೂರ್ಣಾಙ್ಕ axis, ಪೂರ್ಣಾಙ್ಕ start, ಪೂರ್ಣಾಙ್ಕ step, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && ಎಣಿಕೆ_2 >= 0);
    ಯದಿ (ಎಣಿಕೆ_2 > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = ಎಣಿಕೆ_2;
}

ನಾಮಾಕಾಶ {

ಗಣನಾ Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

ಗಣನಾ
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

ಸಂರಚನಾ Plan
{
    ಧ್ರುವ uchar* src;
    uchar* dst;
    size_t esz;
    ಪೂರ್ಣಾಙ್ಕ dims;
    ಪೂರ್ಣಾಙ್ಕ kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    ತಾರ್ಕಿಕ chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    ದ್ವಿಗುಣ cost;
};

ಸ್ಥಿರ ಶೂನ್ಯ swapAxes(Plan& p, ಪೂರ್ಣಾಙ್ಕ i, ಪೂರ್ಣಾಙ್ಕ j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

ಸ್ಥಿರ ಶೂನ್ಯ moveAxis(Plan& p, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ to)
{
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
ಸ್ಥಿರ size_t addressBits(ಧ್ರುವ Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    ಪ್ರತ್ಯಾಗಮ addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
ಸ್ಥಿರ ತಾರ್ಕಿಕ trySplitMerge(Plan& p)
{
    ಧ್ರುವ ptrdiff_t esz = (ptrdiff_t)p.esz;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ L = p.dims - 1;
    ಯದಿ (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ ((addressBits(p) & (esz - 1)) != 0)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    // split: axis k holds cn planes interleaved in the source
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = L - 1; k >= 0; k--)
    {
        ಧ್ರುವ int64 cn = p.size[k];
        ಯದಿ (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ಧ್ರುವ int64 cn = p.size[L];
    ಯದಿ (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = L - 1; k >= 0; k--)
        {
            ಯದಿ (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

// Returns false when there is nothing to copy.
ಸ್ಥಿರ ತಾರ್ಕಿಕ buildPlan(ಧ್ರುವ View& s, ಧ್ರುವ View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ಧ್ರುವ ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    ಪೂರ್ಣಾಙ್ಕ n = 0;
    ದ್ವಿಗುಣ total = 1;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        ಪೂರ್ಣಾಙ್ಕ sz = s.size[i];
        CV_Assert(sz >= 0);
        ಯದಿ (sz == 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        total *= sz;
        ಯದಿ (sz == 1)
            ಅನುವರ್ತ;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        ಯದಿ (ds < 0)
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
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < n; i++)
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    ಪೂರ್ಣಾಙ್ಕ m = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
        ಯದಿ (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        ಅನ್ಯಥಾ
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    ಯದಿ (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ L = m - 1;
    p.tfunc = nullptr;
    ಯದಿ (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    ಅನ್ಯಥಾ ಯದಿ (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    ಅನ್ಯಥಾ ಯದಿ (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        ಯದಿ (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            ಪೂರ್ಣಾಙ್ಕ k = L - 1;
            ಯಾವತ್ (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ಧ್ರುವ size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            ಯದಿ (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ಧ್ರುವ ದ್ವಿಗುಣ bytes = total*(ದ್ವಿಗುಣ)esz;
    ಯದಿ (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = ಅಸತ್ಯ;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    ಅನ್ಯಥಾ ಯದಿ (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = ಅಸತ್ಯ;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    ಅನ್ಯಥಾ
    {
        int64 nrows = 1;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < L; i++)
            nrows *= p.size[i];
        ಧ್ರುವ int64 rowLen = p.size[L];
        ಧ್ರುವ ದ್ವಿಗುಣ rowBytes = (ದ್ವಿಗುಣ)rowLen*esz;
        ಧ್ರುವ ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = ಅಸತ್ಯ;
        ಯದಿ (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (ದ್ವಿಗುಣ)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = ಸತ್ಯ;
        }
        ಅನ್ಯಥಾ ಯದಿ (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

template<typename T> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isAligned(ಧ್ರುವ ಶೂನ್ಯ* s, ಧ್ರುವ ಶೂನ್ಯ* d, ptrdiff_t ss, ptrdiff_t ds)
{
    ಪ್ರತ್ಯಾಗಮ (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ copyStrided_(ಧ್ರುವ uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    ಪ್ರತಿ (; i + 4 <= n; i += 4)
    {
        T t0 = *(ಧ್ರುವ T*)s, t1 = *(ಧ್ರುವ T*)(s + ss);
        T t2 = *(ಧ್ರುವ T*)(s + ss*2), t3 = *(ಧ್ರುವ T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    ಪ್ರತಿ (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ಧ್ರುವ T*)s;
}

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ fill_(ಧ್ರುವ uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ಧ್ರುವ T v = *(ಧ್ರುವ T*)s;
    ಪ್ರತಿ (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

ಸಂರಚನಾ Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
ಸ್ಥಿರ ಶೂನ್ಯ copyRow(ಧ್ರುವ Plan& p, ಧ್ರುವ uchar* s, uchar* d, int64 n)
{
    ಧ್ರುವ size_t esz = p.esz;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ L = p.dims - 1;
    ಧ್ರುವ ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    ಯದಿ (p.kind == KIND_FILL)
    {
        ಯದಿ (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        ಅನ್ಯಥಾ ಯದಿ (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        ಅನ್ಯಥಾ ಯದಿ (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        ಅನ್ಯಥಾ ಯದಿ (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        ಅನ್ಯಥಾ
            ಪ್ರತಿ (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    ಅನ್ಯಥಾ ಯದಿ (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    ಅನ್ಯಥಾ ಯದಿ (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    ಅನ್ಯಥಾ ಯದಿ (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    ಅನ್ಯಥಾ ಯದಿ (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    ಅನ್ಯಥಾ
        ಪ್ರತಿ (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
ಸ್ಥಿರ ಶೂನ್ಯ decode(ಧ್ರುವ Plan& p, ಪೂರ್ಣಾಙ್ಕ naxes, int64 idx, int64* counters,
                   ಧ್ರುವ uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        ಯದಿ (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<ತಾರ್ಕಿಕ MEMCPY> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ copyRowT(ಧ್ರುವ Plan& p, ಧ್ರುವ uchar* s, uchar* d, int64 n)
{
    ಯದಿ (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    ಅನ್ಯಥಾ
        copyRow(p, s, d, n);
}

template<ತಾರ್ಕಿಕ MEMCPY> ಸ್ಥಿರ ಶೂನ್ಯ runRows(ಧ್ರುವ Plan& p, int64 i0, int64 i1)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ L = p.dims - 1;
    ಧ್ರುವ int64 nch = p.nchunks, rowLen = p.size[L];
    ಧ್ರುವ ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ಧ್ರುವ uchar* s;
    uchar* d;

    ಯದಿ (p.chunkMajor)
    {
        ಪ್ರತಿ (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        ಪ್ರತ್ಯಾಗಮ;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    ಯದಿ (nch == 1 && L == 1)
    {
        ಧ್ರುವ ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        ಪ್ರತಿ (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿ (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        ಯದಿ (chEnd < nch)
            ಭಙ್ಗ;
        ch = 0;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            ಯದಿ (++counters[k] < p.size[k])
                ಭಙ್ಗ;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ runTranspose(ಧ್ರುವ Plan& p, int64 i0, int64 i1)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ A = p.dims - 2, B = p.dims - 1;
    ಧ್ರುವ int64 ntiles = p.tilesA*p.tilesB;
    ಪ್ರತಿ (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ಧ್ರುವ uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        ಪೂರ್ಣಾಙ್ಕ wa = (ಪೂರ್ಣಾಙ್ಕ)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        ಪೂರ್ಣಾಙ್ಕ hb = (ಪೂರ್ಣಾಙ್ಕ)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        ಯದಿ (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (ಪೂರ್ಣಾಙ್ಕ)p.esz) == CV_HAL_ERROR_OK)
            ಅನುವರ್ತ;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ runSplitMerge(ಧ್ರುವ Plan& p, int64 i0, int64 i1)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ L = p.dims - 1;
    ಧ್ರುವ ತಾರ್ಕಿಕ isSplit = p.kind == KIND_SPLIT;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ C = isSplit ? L - 1 : L;    // the plane axis
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ X = isSplit ? L : L - 1;    // the long axis
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = (ಪೂರ್ಣಾಙ್ಕ)p.size[C];
    ಧ್ರುವ size_t esz = p.esz;
    ಪ್ರತಿ (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ಧ್ರುವ uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        ಧ್ರುವ int64 e0 = ch*p.chunk;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ len = (ಪೂರ್ಣಾಙ್ಕ)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        ಯದಿ (isSplit)
        {
            uchar* dptr[4];
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            ಯದಿ (esz == 1) hal::split8u(s, dptr, len, cn);
            ಅನ್ಯಥಾ ಯದಿ (esz == 2) hal::split16u((ಧ್ರುವ ushort*)s, (ushort**)dptr, len, cn);
            ಅನ್ಯಥಾ ಯದಿ (esz == 4) hal::split32s((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)s, (ಪೂರ್ಣಾಙ್ಕ**)dptr, len, cn);
            ಅನ್ಯಥಾ hal::split64s((ಧ್ರುವ int64*)s, (int64**)dptr, len, cn);
        }
        ಅನ್ಯಥಾ
        {
            ಧ್ರುವ uchar* sptr[4];
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            ಯದಿ (esz == 1) hal::merge8u(sptr, d, len, cn);
            ಅನ್ಯಥಾ ಯದಿ (esz == 2) hal::merge16u((ಧ್ರುವ ushort**)sptr, (ushort*)d, len, cn);
            ಅನ್ಯಥಾ ಯದಿ (esz == 4) hal::merge32s((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ**)sptr, (ಪೂರ್ಣಾಙ್ಕ*)d, len, cn);
            ಅನ್ಯಥಾ hal::merge64s((ಧ್ರುವ int64**)sptr, (int64*)d, len, cn);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ runPlan(ಧ್ರುವ Plan& p, int64 i0, int64 i1)
{
    ಯದಿ (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    ಅನ್ಯಥಾ ಯದಿ (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    ಅನ್ಯಥಾ ಯದಿ (p.kind == KIND_MEMCPY)
        runRows<ಸತ್ಯ>(p, i0, i1);
    ಅನ್ಯಥಾ
        runRows<ಅಸತ್ಯ>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
ಸ್ಥಿರ ತಾರ್ಕಿಕ canRunInLockstep(ಧ್ರುವ Plan* plans, size_t np)
{
    ಯದಿ (np < 2)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ Plan& p0 = plans[0];
    ಪ್ರತಿ (size_t k = 0; k < np; k++)
    {
        ಧ್ರುವ Plan& p = plans[k];
        ಯದಿ (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < p.dims - 1; i++)
            ಯದಿ (p.size[i] != p0.size[i])
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ runPlans(ಧ್ರುವ Plan* plans, size_t np)
{
    ಯದಿ (np == 0)
        ಪ್ರತ್ಯಾಗಮ;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    ದ್ವಿಗುಣ cost = 0, rowBytes = 0;
    ಪ್ರತಿ (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (ದ್ವಿಗುಣ)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ಧ್ರುವ ತಾರ್ಕಿಕ lockstep = canRunInLockstep(plans, np);
    ಧ್ರುವ int64 total = lockstep ? plans[0].nrows : ofs[np];
    ಧ್ರುವ int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        ಯದಿ (lockstep)
        {
            ಪ್ರತಿ (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                ಪ್ರತಿ (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            ಪ್ರತ್ಯಾಗಮ;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        ಯಾವತ್ (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    ಯದಿ (cost < PARALLEL_MIN_COST || total < 2 || get_ಅಂಕೆ_Threads() <= 1)
    {
        body(0, total);
        ಪ್ರತ್ಯಾಗಮ;
    }

    // grouped so that the range fits into int
    ಧ್ರುವ int64 ngroups = std::min(total, (int64)1 << 20);
    ಧ್ರುವ ದ್ವಿಗುಣ nstripes = std::min((ದ್ವಿಗುಣ)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (ಪೂರ್ಣಾಙ್ಕ)ngroups), [&](ಧ್ರುವ Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
ಸ್ಥಿರ ಶೂನ್ಯ viewRange(ಧ್ರುವ View& v, ಧ್ರುವ uchar*& lo, ಧ್ರುವ uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        ಯದಿ (ext < 0) a += ext; ಅನ್ಯಥಾ b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ hasZeroSize(ಧ್ರುವ View& v)
{
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < v.dims; i++)
        ಯದಿ (v.size[i] == 0)
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ sameMapping(ಧ್ರುವ View& s, ಧ್ರುವ View& d)
{
    ಯದಿ (s.data != d.data)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < s.dims; i++)
        ಯದಿ (s.size[i] > 1 && s.step[i] != d.step[i])
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ overlap(ಧ್ರುವ View& s, ಧ್ರುವ View& d)
{
    ಧ್ರುವ uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    ಪ್ರತ್ಯಾಗಮ slo < dhi && dlo < shi;
}

} // namespace

ಶೂನ್ಯ copyBatch(ಧ್ರುವ View* src, ಧ್ರುವ View* dst, ಪೂರ್ಣಾಙ್ಕ n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
        View s = src[i];
        ಧ್ರುವ View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (ಪೂರ್ಣಾಙ್ಕ)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        ಯದಿ (hasZeroSize(s) || sameMapping(s, d))
            ಅನುವರ್ತ;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        ತಾರ್ಕಿಕ needTemp = ಅಸತ್ಯ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        ಯದಿ (needTemp)
        {
            size_t total = 1;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        ಯದಿ (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

ಶೂನ್ಯ copy(ಧ್ರುವ View& src, ಧ್ರುವ View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
