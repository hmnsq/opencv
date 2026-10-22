// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "nd_copy.hpp"
#அந்தர்பாவ "hal_replacement.hpp"

#அந்தர்பாவ "opencv2/core/hal/hal.hpp"

#அந்தர்பாவ <algorithm>

நாமாகாஶ cv { நாமாகாஶ nd {

View viewOf(மாறிலி Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    க்கு (முழு i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    திரும்பு v;
}

View permute(மாறிலி View& v, மாறிலி முழு* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    க்கு (முழு i = 0; i < v.dims; i++)
    {
        முழு j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    திரும்பு r;
}

வெறுமை flip(View& v, முழு axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    எனில் (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

வெறுமை slice(View& v, முழு axis, முழு start, முழு step, முழு மொத்தம்_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && மொத்தம்_2 >= 0);
    எனில் (மொத்தம்_2 > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = மொத்தம்_2;
}

நாமாகாஶ {

எண்ணிக்கை Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

எண்ணிக்கை
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

கட்டமைப்பு Plan
{
    மாறிலி uchar* src;
    uchar* dst;
    size_t esz;
    முழு dims;
    முழு kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    தருக்க chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    இரட்டை cost;
};

நிலையான வெறுமை swapAxes(Plan& p, முழு i, முழு j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

நிலையான வெறுமை moveAxis(Plan& p, முழு k, முழு to)
{
    க்கு (முழு i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
நிலையான size_t addressBits(மாறிலி Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    க்கு (முழு i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    திரும்பு addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
நிலையான தருக்க trySplitMerge(Plan& p)
{
    மாறிலி ptrdiff_t esz = (ptrdiff_t)p.esz;
    மாறிலி முழு L = p.dims - 1;
    எனில் (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        திரும்பு பொய்;
    எனில் ((addressBits(p) & (esz - 1)) != 0)
        திரும்பு பொய்;

    // split: axis k holds cn planes interleaved in the source
    க்கு (முழு k = L - 1; k >= 0; k--)
    {
        மாறிலி int64 cn = p.size[k];
        எனில் (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            திரும்பு உண்மை;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    மாறிலி int64 cn = p.size[L];
    எனில் (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        க்கு (முழு k = L - 1; k >= 0; k--)
        {
            எனில் (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                திரும்பு உண்மை;
            }
        }
    }
    திரும்பு பொய்;
}

// Returns false when there is nothing to copy.
நிலையான தருக்க buildPlan(மாறிலி View& s, மாறிலி View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    மாறிலி ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    முழு n = 0;
    இரட்டை total = 1;
    க்கு (முழு i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        முழு sz = s.size[i];
        CV_Assert(sz >= 0);
        எனில் (sz == 0)
            திரும்பு பொய்;
        total *= sz;
        எனில் (sz == 1)
            தொடர்;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        எனில் (ds < 0)
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
    க்கு (முழு i = 1; i < n; i++)
        க்கு (முழு j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    முழு m = 0;
    க்கு (முழு i = 0; i < n; i++)
    {
        எனில் (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        இல்லையெனில்
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    எனில் (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    மாறிலி முழு L = m - 1;
    p.tfunc = nullptr;
    எனில் (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    இல்லையெனில் எனில் (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    இல்லையெனில் எனில் (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        எனில் (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            முழு k = L - 1;
            போது (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            மாறிலி size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            எனில் (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    மாறிலி இரட்டை bytes = total*(இரட்டை)esz;
    எனில் (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        மாறிலி முழு X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        க்கு (முழு i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = பொய்;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    இல்லையெனில் எனில் (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        க்கு (முழு i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = பொய்;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    இல்லையெனில்
    {
        int64 nrows = 1;
        க்கு (முழு i = 0; i < L; i++)
            nrows *= p.size[i];
        மாறிலி int64 rowLen = p.size[L];
        மாறிலி இரட்டை rowBytes = (இரட்டை)rowLen*esz;
        மாறிலி ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = பொய்;
        எனில் (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (இரட்டை)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = உண்மை;
        }
        இல்லையெனில் எனில் (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    திரும்பு உண்மை;
}

template<typename T> நிலையான உள்வரி தருக்க isAligned(மாறிலி வெறுமை* s, மாறிலி வெறுமை* d, ptrdiff_t ss, ptrdiff_t ds)
{
    திரும்பு (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> நிலையான வெறுமை copyStrided_(மாறிலி uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    க்கு (; i + 4 <= n; i += 4)
    {
        T t0 = *(மாறிலி T*)s, t1 = *(மாறிலி T*)(s + ss);
        T t2 = *(மாறிலி T*)(s + ss*2), t3 = *(மாறிலி T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    க்கு (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(மாறிலி T*)s;
}

template<typename T> நிலையான வெறுமை fill_(மாறிலி uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    மாறிலி T v = *(மாறிலி T*)s;
    க்கு (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

கட்டமைப்பு Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
நிலையான வெறுமை copyRow(மாறிலி Plan& p, மாறிலி uchar* s, uchar* d, int64 n)
{
    மாறிலி size_t esz = p.esz;
    மாறிலி முழு L = p.dims - 1;
    மாறிலி ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    எனில் (p.kind == KIND_FILL)
    {
        எனில் (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        இல்லையெனில் எனில் (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        இல்லையெனில் எனில் (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        இல்லையெனில் எனில் (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        இல்லையெனில்
            க்கு (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        திரும்பு;
    }

    எனில் (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    இல்லையெனில் எனில் (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    இல்லையெனில் எனில் (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    இல்லையெனில் எனில் (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    இல்லையெனில் எனில் (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    இல்லையெனில்
        க்கு (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
நிலையான வெறுமை decode(மாறிலி Plan& p, முழு naxes, int64 idx, int64* counters,
                   மாறிலி uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    க்கு (முழு k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        எனில் (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<தருக்க MEMCPY> நிலையான உள்வரி வெறுமை copyRowT(மாறிலி Plan& p, மாறிலி uchar* s, uchar* d, int64 n)
{
    எனில் (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    இல்லையெனில்
        copyRow(p, s, d, n);
}

template<தருக்க MEMCPY> நிலையான வெறுமை runRows(மாறிலி Plan& p, int64 i0, int64 i1)
{
    மாறிலி முழு L = p.dims - 1;
    மாறிலி int64 nch = p.nchunks, rowLen = p.size[L];
    மாறிலி ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    மாறிலி uchar* s;
    uchar* d;

    எனில் (p.chunkMajor)
    {
        க்கு (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        திரும்பு;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    எனில் (nch == 1 && L == 1)
    {
        மாறிலி ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        க்கு (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        திரும்பு;
    }

    க்கு (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        எனில் (chEnd < nch)
            நிறுத்து;
        ch = 0;
        க்கு (முழு k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            எனில் (++counters[k] < p.size[k])
                நிறுத்து;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

நிலையான வெறுமை runTranspose(மாறிலி Plan& p, int64 i0, int64 i1)
{
    மாறிலி முழு A = p.dims - 2, B = p.dims - 1;
    மாறிலி int64 ntiles = p.tilesA*p.tilesB;
    க்கு (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        மாறிலி uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        முழு wa = (முழு)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        முழு hb = (முழு)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        எனில் (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (முழு)p.esz) == CV_HAL_ERROR_OK)
            தொடர்;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

நிலையான வெறுமை runSplitMerge(மாறிலி Plan& p, int64 i0, int64 i1)
{
    மாறிலி முழு L = p.dims - 1;
    மாறிலி தருக்க isSplit = p.kind == KIND_SPLIT;
    மாறிலி முழு C = isSplit ? L - 1 : L;    // the plane axis
    மாறிலி முழு X = isSplit ? L : L - 1;    // the long axis
    மாறிலி முழு cn = (முழு)p.size[C];
    மாறிலி size_t esz = p.esz;
    க்கு (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        மாறிலி uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        மாறிலி int64 e0 = ch*p.chunk;
        மாறிலி முழு len = (முழு)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        எனில் (isSplit)
        {
            uchar* dptr[4];
            க்கு (முழு c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            எனில் (esz == 1) hal::split8u(s, dptr, len, cn);
            இல்லையெனில் எனில் (esz == 2) hal::split16u((மாறிலி ushort*)s, (ushort**)dptr, len, cn);
            இல்லையெனில் எனில் (esz == 4) hal::split32s((மாறிலி முழு*)s, (முழு**)dptr, len, cn);
            இல்லையெனில் hal::split64s((மாறிலி int64*)s, (int64**)dptr, len, cn);
        }
        இல்லையெனில்
        {
            மாறிலி uchar* sptr[4];
            க்கு (முழு c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            எனில் (esz == 1) hal::merge8u(sptr, d, len, cn);
            இல்லையெனில் எனில் (esz == 2) hal::merge16u((மாறிலி ushort**)sptr, (ushort*)d, len, cn);
            இல்லையெனில் எனில் (esz == 4) hal::merge32s((மாறிலி முழு**)sptr, (முழு*)d, len, cn);
            இல்லையெனில் hal::merge64s((மாறிலி int64**)sptr, (int64*)d, len, cn);
        }
    }
}

நிலையான வெறுமை runPlan(மாறிலி Plan& p, int64 i0, int64 i1)
{
    எனில் (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    இல்லையெனில் எனில் (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    இல்லையெனில் எனில் (p.kind == KIND_MEMCPY)
        runRows<உண்மை>(p, i0, i1);
    இல்லையெனில்
        runRows<பொய்>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
நிலையான தருக்க canRunInLockstep(மாறிலி Plan* plans, size_t np)
{
    எனில் (np < 2)
        திரும்பு பொய்;
    மாறிலி Plan& p0 = plans[0];
    க்கு (size_t k = 0; k < np; k++)
    {
        மாறிலி Plan& p = plans[k];
        எனில் (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            திரும்பு பொய்;
        க்கு (முழு i = 0; i < p.dims - 1; i++)
            எனில் (p.size[i] != p0.size[i])
                திரும்பு பொய்;
    }
    திரும்பு உண்மை;
}

நிலையான வெறுமை runPlans(மாறிலி Plan* plans, size_t np)
{
    எனில் (np == 0)
        திரும்பு;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    இரட்டை cost = 0, rowBytes = 0;
    க்கு (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (இரட்டை)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    மாறிலி தருக்க lockstep = canRunInLockstep(plans, np);
    மாறிலி int64 total = lockstep ? plans[0].nrows : ofs[np];
    மாறிலி int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        எனில் (lockstep)
        {
            க்கு (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                க்கு (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            திரும்பு;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        போது (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    எனில் (cost < PARALLEL_MIN_COST || total < 2 || get_இலக்கம்_Threads() <= 1)
    {
        body(0, total);
        திரும்பு;
    }

    // grouped so that the range fits into int
    மாறிலி int64 ngroups = std::min(total, (int64)1 << 20);
    மாறிலி இரட்டை nstripes = std::min((இரட்டை)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (முழு)ngroups), [&](மாறிலி Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
நிலையான வெறுமை viewRange(மாறிலி View& v, மாறிலி uchar*& lo, மாறிலி uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    க்கு (முழு i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        எனில் (ext < 0) a += ext; இல்லையெனில் b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

நிலையான தருக்க hasZeroSize(மாறிலி View& v)
{
    க்கு (முழு i = 0; i < v.dims; i++)
        எனில் (v.size[i] == 0)
            திரும்பு உண்மை;
    திரும்பு பொய்;
}

நிலையான தருக்க sameMapping(மாறிலி View& s, மாறிலி View& d)
{
    எனில் (s.data != d.data)
        திரும்பு பொய்;
    க்கு (முழு i = 0; i < s.dims; i++)
        எனில் (s.size[i] > 1 && s.step[i] != d.step[i])
            திரும்பு பொய்;
    திரும்பு உண்மை;
}

நிலையான தருக்க overlap(மாறிலி View& s, மாறிலி View& d)
{
    மாறிலி uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    திரும்பு slo < dhi && dlo < shi;
}

} // namespace

வெறுமை copyBatch(மாறிலி View* src, மாறிலி View* dst, முழு n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    க்கு (முழு i = 0; i < n; i++)
    {
        View s = src[i];
        மாறிலி View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (முழு)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        எனில் (hasZeroSize(s) || sameMapping(s, d))
            தொடர்;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        தருக்க needTemp = பொய்;
        க்கு (முழு j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        எனில் (needTemp)
        {
            size_t total = 1;
            க்கு (முழு k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            க்கு (முழு k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        எனில் (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

வெறுமை copy(மாறிலி View& src, மாறிலி View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
