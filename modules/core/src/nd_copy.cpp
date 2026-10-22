// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "nd_copy.hpp"
#ଅନ୍ତର୍ଭାଵ "hal_replacement.hpp"

#ଅନ୍ତର୍ଭାଵ "opencv2/core/hal/hal.hpp"

#ଅନ୍ତର୍ଭାଵ <algorithm>

ନାମାକାଶ cv { ନାମାକାଶ nd {

View viewOf(ଅଚଳ Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    ଫେରନ୍ତୁ v;
}

View permute(ଅଚଳ View& v, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < v.dims; i++)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    ଫେରନ୍ତୁ r;
}

ଶୂନ୍ୟ flip(View& v, ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    ଯଦି (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

ଶୂନ୍ୟ slice(View& v, ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis, ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, ପୂର୍ଣ୍ଣସଂଖ୍ୟା step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && ଗଣନା_3 >= 0);
    ଯଦି (ଗଣନା_3 > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = ଗଣନା_3;
}

ନାମାକାଶ {

ଗଣନା Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

ଗଣନା
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

ସଂରଚନା Plan
{
    ଅଚଳ uchar* src;
    uchar* dst;
    size_t esz;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    ତାର୍କିକ chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    ଦ୍ୱିଗୁଣ cost;
};

ସ୍ଥିର ଶୂନ୍ୟ swapAxes(Plan& p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ପୂର୍ଣ୍ଣସଂଖ୍ୟା j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

ସ୍ଥିର ଶୂନ୍ୟ moveAxis(Plan& p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା to)
{
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
ସ୍ଥିର size_t addressBits(ଅଚଳ Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    ଫେରନ୍ତୁ addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
ସ୍ଥିର ତାର୍କିକ trySplitMerge(Plan& p)
{
    ଅଚଳ ptrdiff_t esz = (ptrdiff_t)p.esz;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା L = p.dims - 1;
    ଯଦି (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି ((addressBits(p) & (esz - 1)) != 0)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    // split: axis k holds cn planes interleaved in the source
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = L - 1; k >= 0; k--)
    {
        ଅଚଳ int64 cn = p.size[k];
        ଯଦି (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ଅଚଳ int64 cn = p.size[L];
    ଯଦି (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = L - 1; k >= 0; k--)
        {
            ଯଦି (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                ଫେରନ୍ତୁ ସତ୍ୟ;
            }
        }
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

// Returns false when there is nothing to copy.
ସ୍ଥିର ତାର୍କିକ buildPlan(ଅଚଳ View& s, ଅଚଳ View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ଅଚଳ ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
    ଦ୍ୱିଗୁଣ total = 1;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz = s.size[i];
        CV_Assert(sz >= 0);
        ଯଦି (sz == 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        total *= sz;
        ଯଦି (sz == 1)
            ଜାରି;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        ଯଦି (ds < 0)
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
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < n; i++)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
    {
        ଯଦି (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        ଅନ୍ୟଥା
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    ଯଦି (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା L = m - 1;
    p.tfunc = nullptr;
    ଯଦି (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    ଅନ୍ୟଥା ଯଦି (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    ଅନ୍ୟଥା ଯଦି (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        ଯଦି (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = L - 1;
            ଯେତେବେଳେ (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ଅଚଳ size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            ଯଦି (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ଅଚଳ ଦ୍ୱିଗୁଣ bytes = total*(ଦ୍ୱିଗୁଣ)esz;
    ଯଦି (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = ମିଥ୍ୟା;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    ଅନ୍ୟଥା ଯଦି (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = ମିଥ୍ୟା;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    ଅନ୍ୟଥା
    {
        int64 nrows = 1;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < L; i++)
            nrows *= p.size[i];
        ଅଚଳ int64 rowLen = p.size[L];
        ଅଚଳ ଦ୍ୱିଗୁଣ rowBytes = (ଦ୍ୱିଗୁଣ)rowLen*esz;
        ଅଚଳ ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = ମିଥ୍ୟା;
        ଯଦି (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (ଦ୍ୱିଗୁଣ)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = ସତ୍ୟ;
        }
        ଅନ୍ୟଥା ଯଦି (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

template<typename T> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ isAligned(ଅଚଳ ଶୂନ୍ୟ* s, ଅଚଳ ଶୂନ୍ୟ* d, ptrdiff_t ss, ptrdiff_t ds)
{
    ଫେରନ୍ତୁ (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ copyStrided_(ଅଚଳ uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    ପାଇଁ (; i + 4 <= n; i += 4)
    {
        T t0 = *(ଅଚଳ T*)s, t1 = *(ଅଚଳ T*)(s + ss);
        T t2 = *(ଅଚଳ T*)(s + ss*2), t3 = *(ଅଚଳ T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    ପାଇଁ (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ଅଚଳ T*)s;
}

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ fill_(ଅଚଳ uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ଅଚଳ T v = *(ଅଚଳ T*)s;
    ପାଇଁ (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

ସଂରଚନା Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
ସ୍ଥିର ଶୂନ୍ୟ copyRow(ଅଚଳ Plan& p, ଅଚଳ uchar* s, uchar* d, int64 n)
{
    ଅଚଳ size_t esz = p.esz;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା L = p.dims - 1;
    ଅଚଳ ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    ଯଦି (p.kind == KIND_FILL)
    {
        ଯଦି (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        ଅନ୍ୟଥା ଯଦି (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        ଅନ୍ୟଥା ଯଦି (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        ଅନ୍ୟଥା ଯଦି (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        ଅନ୍ୟଥା
            ପାଇଁ (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        ଫେରନ୍ତୁ;
    }

    ଯଦି (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    ଅନ୍ୟଥା ଯଦି (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    ଅନ୍ୟଥା ଯଦି (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    ଅନ୍ୟଥା ଯଦି (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    ଅନ୍ୟଥା ଯଦି (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    ଅନ୍ୟଥା
        ପାଇଁ (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
ସ୍ଥିର ଶୂନ୍ୟ decode(ଅଚଳ Plan& p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା naxes, int64 idx, int64* counters,
                   ଅଚଳ uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        ଯଦି (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<ତାର୍କିକ MEMCPY> ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ copyRowT(ଅଚଳ Plan& p, ଅଚଳ uchar* s, uchar* d, int64 n)
{
    ଯଦି (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    ଅନ୍ୟଥା
        copyRow(p, s, d, n);
}

template<ତାର୍କିକ MEMCPY> ସ୍ଥିର ଶୂନ୍ୟ runRows(ଅଚଳ Plan& p, int64 i0, int64 i1)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା L = p.dims - 1;
    ଅଚଳ int64 nch = p.nchunks, rowLen = p.size[L];
    ଅଚଳ ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ଅଚଳ uchar* s;
    uchar* d;

    ଯଦି (p.chunkMajor)
    {
        ପାଇଁ (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        ଫେରନ୍ତୁ;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    ଯଦି (nch == 1 && L == 1)
    {
        ଅଚଳ ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        ପାଇଁ (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        ଫେରନ୍ତୁ;
    }

    ପାଇଁ (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        ଯଦି (chEnd < nch)
            ଭାଙ୍ଗ;
        ch = 0;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            ଯଦି (++counters[k] < p.size[k])
                ଭାଙ୍ଗ;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ runTranspose(ଅଚଳ Plan& p, int64 i0, int64 i1)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା A = p.dims - 2, B = p.dims - 1;
    ଅଚଳ int64 ntiles = p.tilesA*p.tilesB;
    ପାଇଁ (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ଅଚଳ uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା wa = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା hb = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        ଯଦି (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)p.esz) == CV_HAL_ERROR_OK)
            ଜାରି;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

ସ୍ଥିର ଶୂନ୍ୟ runSplitMerge(ଅଚଳ Plan& p, int64 i0, int64 i1)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା L = p.dims - 1;
    ଅଚଳ ତାର୍କିକ isSplit = p.kind == KIND_SPLIT;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା C = isSplit ? L - 1 : L;    // the plane axis
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା X = isSplit ? L : L - 1;    // the long axis
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)p.size[C];
    ଅଚଳ size_t esz = p.esz;
    ପାଇଁ (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ଅଚଳ uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        ଅଚଳ int64 e0 = ch*p.chunk;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        ଯଦି (isSplit)
        {
            uchar* dptr[4];
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            ଯଦି (esz == 1) hal::split8u(s, dptr, len, cn);
            ଅନ୍ୟଥା ଯଦି (esz == 2) hal::split16u((ଅଚଳ ushort*)s, (ushort**)dptr, len, cn);
            ଅନ୍ୟଥା ଯଦି (esz == 4) hal::split32s((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)s, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା**)dptr, len, cn);
            ଅନ୍ୟଥା hal::split64s((ଅଚଳ int64*)s, (int64**)dptr, len, cn);
        }
        ଅନ୍ୟଥା
        {
            ଅଚଳ uchar* sptr[4];
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            ଯଦି (esz == 1) hal::merge8u(sptr, d, len, cn);
            ଅନ୍ୟଥା ଯଦି (esz == 2) hal::merge16u((ଅଚଳ ushort**)sptr, (ushort*)d, len, cn);
            ଅନ୍ୟଥା ଯଦି (esz == 4) hal::merge32s((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା**)sptr, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)d, len, cn);
            ଅନ୍ୟଥା hal::merge64s((ଅଚଳ int64**)sptr, (int64*)d, len, cn);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ runPlan(ଅଚଳ Plan& p, int64 i0, int64 i1)
{
    ଯଦି (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    ଅନ୍ୟଥା ଯଦି (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    ଅନ୍ୟଥା ଯଦି (p.kind == KIND_MEMCPY)
        runRows<ସତ୍ୟ>(p, i0, i1);
    ଅନ୍ୟଥା
        runRows<ମିଥ୍ୟା>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
ସ୍ଥିର ତାର୍କିକ canRunInLockstep(ଅଚଳ Plan* plans, size_t np)
{
    ଯଦି (np < 2)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ Plan& p0 = plans[0];
    ପାଇଁ (size_t k = 0; k < np; k++)
    {
        ଅଚଳ Plan& p = plans[k];
        ଯଦି (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < p.dims - 1; i++)
            ଯଦି (p.size[i] != p0.size[i])
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ runPlans(ଅଚଳ Plan* plans, size_t np)
{
    ଯଦି (np == 0)
        ଫେରନ୍ତୁ;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    ଦ୍ୱିଗୁଣ cost = 0, rowBytes = 0;
    ପାଇଁ (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (ଦ୍ୱିଗୁଣ)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ଅଚଳ ତାର୍କିକ lockstep = canRunInLockstep(plans, np);
    ଅଚଳ int64 total = lockstep ? plans[0].nrows : ofs[np];
    ଅଚଳ int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        ଯଦି (lockstep)
        {
            ପାଇଁ (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                ପାଇଁ (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            ଫେରନ୍ତୁ;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        ଯେତେବେଳେ (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    ଯଦି (cost < PARALLEL_MIN_COST || total < 2 || get_ଅଙ୍କ_Threads() <= 1)
    {
        body(0, total);
        ଫେରନ୍ତୁ;
    }

    // grouped so that the range fits into int
    ଅଚଳ int64 ngroups = std::min(total, (int64)1 << 20);
    ଅଚଳ ଦ୍ୱିଗୁଣ nstripes = std::min((ଦ୍ୱିଗୁଣ)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ngroups), [&](ଅଚଳ Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
ସ୍ଥିର ଶୂନ୍ୟ viewRange(ଅଚଳ View& v, ଅଚଳ uchar*& lo, ଅଚଳ uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        ଯଦି (ext < 0) a += ext; ଅନ୍ୟଥା b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

ସ୍ଥିର ତାର୍କିକ hasZeroSize(ଅଚଳ View& v)
{
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < v.dims; i++)
        ଯଦି (v.size[i] == 0)
            ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର ତାର୍କିକ sameMapping(ଅଚଳ View& s, ଅଚଳ View& d)
{
    ଯଦି (s.data != d.data)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < s.dims; i++)
        ଯଦି (s.size[i] > 1 && s.step[i] != d.step[i])
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ତାର୍କିକ overlap(ଅଚଳ View& s, ଅଚଳ View& d)
{
    ଅଚଳ uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    ଫେରନ୍ତୁ slo < dhi && dlo < shi;
}

} // namespace

ଶୂନ୍ୟ copyBatch(ଅଚଳ View* src, ଅଚଳ View* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
    {
        View s = src[i];
        ଅଚଳ View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        ଯଦି (hasZeroSize(s) || sameMapping(s, d))
            ଜାରି;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        ତାର୍କିକ needTemp = ମିଥ୍ୟା;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        ଯଦି (needTemp)
        {
            size_t total = 1;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        ଯଦି (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

ଶୂନ୍ୟ copy(ଅଚଳ View& src, ଅଚଳ View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
