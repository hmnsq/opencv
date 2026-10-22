// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "nd_copy.hpp"
#অন্তর্ভাব "hal_replacement.hpp"

#অন্তর্ভাব "opencv2/core/hal/hal.hpp"

#অন্তর্ভাব <algorithm>

নামাকাশ cv { নামাকাশ nd {

View viewOf(ধ্রুবক Mat& m)
{
    View v;
    v.তথ্য = m.তথ্য;
    v.dims = m.dims;
    v.esz = m.elem_আকার();
    জন্য (পূর্ণসংখ্যা i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    ফেরত v;
}

View permute(ধ্রুবক View& v, ধ্রুবক পূর্ণসংখ্যা* order)
{
    View r;
    r.তথ্য = v.তথ্য;
    r.dims = v.dims;
    r.esz = v.esz;
    জন্য (পূর্ণসংখ্যা i = 0; i < v.dims; i++)
    {
        পূর্ণসংখ্যা j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    ফেরত r;
}

শূন্য flip(View& v, পূর্ণসংখ্যা axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    যদি (v.size[axis] > 0)
        v.তথ্য += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

শূন্য slice(View& v, পূর্ণসংখ্যা axis, পূর্ণসংখ্যা start, পূর্ণসংখ্যা step, পূর্ণসংখ্যা গুনতি_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && গুনতি_2 >= 0);
    যদি (গুনতি_2 > 0)
        v.তথ্য += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = গুনতি_2;
}

নামাকাশ {

গণনা Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

গণনা
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

কাঠামো Plan
{
    ধ্রুবক uchar* src;
    uchar* dst;
    size_t esz;
    পূর্ণসংখ্যা dims;
    পূর্ণসংখ্যা kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    যৌক্তিক chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    দ্বিগুণ cost;
};

স্থির শূন্য swapAxes(Plan& p, পূর্ণসংখ্যা i, পূর্ণসংখ্যা j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

স্থির শূন্য moveAxis(Plan& p, পূর্ণসংখ্যা k, পূর্ণসংখ্যা to)
{
    জন্য (পূর্ণসংখ্যা i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
স্থির size_t addressBits(ধ্রুবক Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    জন্য (পূর্ণসংখ্যা i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    ফেরত addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
স্থির যৌক্তিক trySplitMerge(Plan& p)
{
    ধ্রুবক ptrdiff_t esz = (ptrdiff_t)p.esz;
    ধ্রুবক পূর্ণসংখ্যা L = p.dims - 1;
    যদি (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        ফেরত মিথ্যা;
    যদি ((addressBits(p) & (esz - 1)) != 0)
        ফেরত মিথ্যা;

    // split: axis k holds cn planes interleaved in the source
    জন্য (পূর্ণসংখ্যা k = L - 1; k >= 0; k--)
    {
        ধ্রুবক int64 cn = p.size[k];
        যদি (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            ফেরত সত্য;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ধ্রুবক int64 cn = p.size[L];
    যদি (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        জন্য (পূর্ণসংখ্যা k = L - 1; k >= 0; k--)
        {
            যদি (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                ফেরত সত্য;
            }
        }
    }
    ফেরত মিথ্যা;
}

// Returns false when there is nothing to copy.
স্থির যৌক্তিক buildPlan(ধ্রুবক View& s, ধ্রুবক View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ধ্রুবক ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.তথ্য;
    p.dst = d.তথ্য;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    পূর্ণসংখ্যা n = 0;
    দ্বিগুণ যোগ = 1;
    জন্য (পূর্ণসংখ্যা i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        পূর্ণসংখ্যা sz = s.size[i];
        CV_Assert(sz >= 0);
        যদি (sz == 0)
            ফেরত মিথ্যা;
        যোগ *= sz;
        যদি (sz == 1)
            চালু;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        যদি (ds < 0)
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
    জন্য (পূর্ণসংখ্যা i = 1; i < n; i++)
        জন্য (পূর্ণসংখ্যা j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    পূর্ণসংখ্যা m = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < n; i++)
    {
        যদি (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        অন্যথা
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    যদি (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    ধ্রুবক পূর্ণসংখ্যা L = m - 1;
    p.tfunc = nullptr;
    যদি (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    অন্যথা যদি (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    অন্যথা যদি (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        যদি (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            পূর্ণসংখ্যা k = L - 1;
            যখন (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ধ্রুবক size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            যদি (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ধ্রুবক দ্বিগুণ bytes = যোগ*(দ্বিগুণ)esz;
    যদি (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        ধ্রুবক পূর্ণসংখ্যা X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        জন্য (পূর্ণসংখ্যা i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = মিথ্যা;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    অন্যথা যদি (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        জন্য (পূর্ণসংখ্যা i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = মিথ্যা;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    অন্যথা
    {
        int64 nrows = 1;
        জন্য (পূর্ণসংখ্যা i = 0; i < L; i++)
            nrows *= p.size[i];
        ধ্রুবক int64 rowLen = p.size[L];
        ধ্রুবক দ্বিগুণ rowBytes = (দ্বিগুণ)rowLen*esz;
        ধ্রুবক ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = মিথ্যা;
        যদি (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (দ্বিগুণ)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = সত্য;
        }
        অন্যথা যদি (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    ফেরত সত্য;
}

template<typename T> স্থির অন্তঃরেখ যৌক্তিক isAligned(ধ্রুবক শূন্য* s, ধ্রুবক শূন্য* d, ptrdiff_t ss, ptrdiff_t ds)
{
    ফেরত (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> স্থির শূন্য copyStrided_(ধ্রুবক uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    জন্য (; i + 4 <= n; i += 4)
    {
        T t0 = *(ধ্রুবক T*)s, t1 = *(ধ্রুবক T*)(s + ss);
        T t2 = *(ধ্রুবক T*)(s + ss*2), t3 = *(ধ্রুবক T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    জন্য (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ধ্রুবক T*)s;
}

template<typename T> স্থির শূন্য fill_(ধ্রুবক uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ধ্রুবক T v = *(ধ্রুবক T*)s;
    জন্য (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

কাঠামো Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
স্থির শূন্য copyRow(ধ্রুবক Plan& p, ধ্রুবক uchar* s, uchar* d, int64 n)
{
    ধ্রুবক size_t esz = p.esz;
    ধ্রুবক পূর্ণসংখ্যা L = p.dims - 1;
    ধ্রুবক ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    যদি (p.kind == KIND_FILL)
    {
        যদি (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        অন্যথা যদি (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        অন্যথা যদি (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        অন্যথা যদি (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        অন্যথা
            জন্য (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        ফেরত;
    }

    যদি (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    অন্যথা যদি (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    অন্যথা যদি (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    অন্যথা যদি (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    অন্যথা যদি (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    অন্যথা
        জন্য (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
স্থির শূন্য decode(ধ্রুবক Plan& p, পূর্ণসংখ্যা naxes, int64 idx, int64* counters,
                   ধ্রুবক uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    জন্য (পূর্ণসংখ্যা k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        যদি (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<যৌক্তিক MEMCPY> স্থির অন্তঃরেখ শূন্য copyRowT(ধ্রুবক Plan& p, ধ্রুবক uchar* s, uchar* d, int64 n)
{
    যদি (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    অন্যথা
        copyRow(p, s, d, n);
}

template<যৌক্তিক MEMCPY> স্থির শূন্য runRows(ধ্রুবক Plan& p, int64 i0, int64 i1)
{
    ধ্রুবক পূর্ণসংখ্যা L = p.dims - 1;
    ধ্রুবক int64 nch = p.nchunks, rowLen = p.size[L];
    ধ্রুবক ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ধ্রুবক uchar* s;
    uchar* d;

    যদি (p.chunkMajor)
    {
        জন্য (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        ফেরত;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    যদি (nch == 1 && L == 1)
    {
        ধ্রুবক ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        জন্য (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        ফেরত;
    }

    জন্য (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        যদি (chEnd < nch)
            বিরতি;
        ch = 0;
        জন্য (পূর্ণসংখ্যা k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            যদি (++counters[k] < p.size[k])
                বিরতি;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

স্থির শূন্য runTranspose(ধ্রুবক Plan& p, int64 i0, int64 i1)
{
    ধ্রুবক পূর্ণসংখ্যা A = p.dims - 2, B = p.dims - 1;
    ধ্রুবক int64 ntiles = p.tilesA*p.tilesB;
    জন্য (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ধ্রুবক uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        পূর্ণসংখ্যা wa = (পূর্ণসংখ্যা)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        পূর্ণসংখ্যা hb = (পূর্ণসংখ্যা)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        যদি (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (পূর্ণসংখ্যা)p.esz) == CV_HAL_ERROR_OK)
            চালু;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], আকার_2(wa, hb));
    }
}

স্থির শূন্য runSplitMerge(ধ্রুবক Plan& p, int64 i0, int64 i1)
{
    ধ্রুবক পূর্ণসংখ্যা L = p.dims - 1;
    ধ্রুবক যৌক্তিক isSplit = p.kind == KIND_SPLIT;
    ধ্রুবক পূর্ণসংখ্যা C = isSplit ? L - 1 : L;    // the plane axis
    ধ্রুবক পূর্ণসংখ্যা X = isSplit ? L : L - 1;    // the long axis
    ধ্রুবক পূর্ণসংখ্যা cn = (পূর্ণসংখ্যা)p.size[C];
    ধ্রুবক size_t esz = p.esz;
    জন্য (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ধ্রুবক uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        ধ্রুবক int64 e0 = ch*p.chunk;
        ধ্রুবক পূর্ণসংখ্যা len = (পূর্ণসংখ্যা)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        যদি (isSplit)
        {
            uchar* dptr[4];
            জন্য (পূর্ণসংখ্যা c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            যদি (esz == 1) hal::split8u(s, dptr, len, cn);
            অন্যথা যদি (esz == 2) hal::split16u((ধ্রুবক ushort*)s, (ushort**)dptr, len, cn);
            অন্যথা যদি (esz == 4) hal::split32s((ধ্রুবক পূর্ণসংখ্যা*)s, (পূর্ণসংখ্যা**)dptr, len, cn);
            অন্যথা hal::split64s((ধ্রুবক int64*)s, (int64**)dptr, len, cn);
        }
        অন্যথা
        {
            ধ্রুবক uchar* sptr[4];
            জন্য (পূর্ণসংখ্যা c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            যদি (esz == 1) hal::merge8u(sptr, d, len, cn);
            অন্যথা যদি (esz == 2) hal::merge16u((ধ্রুবক ushort**)sptr, (ushort*)d, len, cn);
            অন্যথা যদি (esz == 4) hal::merge32s((ধ্রুবক পূর্ণসংখ্যা**)sptr, (পূর্ণসংখ্যা*)d, len, cn);
            অন্যথা hal::merge64s((ধ্রুবক int64**)sptr, (int64*)d, len, cn);
        }
    }
}

স্থির শূন্য runPlan(ধ্রুবক Plan& p, int64 i0, int64 i1)
{
    যদি (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    অন্যথা যদি (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    অন্যথা যদি (p.kind == KIND_MEMCPY)
        runRows<সত্য>(p, i0, i1);
    অন্যথা
        runRows<মিথ্যা>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
স্থির যৌক্তিক canRunInLockstep(ধ্রুবক Plan* plans, size_t np)
{
    যদি (np < 2)
        ফেরত মিথ্যা;
    ধ্রুবক Plan& p0 = plans[0];
    জন্য (size_t k = 0; k < np; k++)
    {
        ধ্রুবক Plan& p = plans[k];
        যদি (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            ফেরত মিথ্যা;
        জন্য (পূর্ণসংখ্যা i = 0; i < p.dims - 1; i++)
            যদি (p.size[i] != p0.size[i])
                ফেরত মিথ্যা;
    }
    ফেরত সত্য;
}

স্থির শূন্য runPlans(ধ্রুবক Plan* plans, size_t np)
{
    যদি (np == 0)
        ফেরত;
    Auto_বাফার<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    দ্বিগুণ cost = 0, rowBytes = 0;
    জন্য (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (দ্বিগুণ)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ধ্রুবক যৌক্তিক lockstep = canRunInLockstep(plans, np);
    ধ্রুবক int64 যোগ = lockstep ? plans[0].nrows : ofs[np];
    ধ্রুবক int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        যদি (lockstep)
        {
            জন্য (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                জন্য (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            ফেরত;
        }
        size_t k = std::upper_bound(ofs.তথ্য(), ofs.তথ্য() + np + 1, a) - ofs.তথ্য() - 1;
        যখন (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    যদি (cost < PARALLEL_MIN_COST || যোগ < 2 || get_অঙ্ক_Threads() <= 1)
    {
        body(0, যোগ);
        ফেরত;
    }

    // grouped so that the range fits into int
    ধ্রুবক int64 ngroups = std::min(যোগ, (int64)1 << 20);
    ধ্রুবক দ্বিগুণ nstripes = std::min((দ্বিগুণ)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (পূর্ণসংখ্যা)ngroups), [&](ধ্রুবক Range& r)
    {
        body(r.start*যোগ/ngroups, r.end*যোগ/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
স্থির শূন্য viewRange(ধ্রুবক View& v, ধ্রুবক uchar*& lo, ধ্রুবক uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        যদি (ext < 0) a += ext; অন্যথা b += ext;
    }
    lo = v.তথ্য + a;
    hi = v.তথ্য + b + v.esz;
}

স্থির যৌক্তিক has_Zero_আকার(ধ্রুবক View& v)
{
    জন্য (পূর্ণসংখ্যা i = 0; i < v.dims; i++)
        যদি (v.size[i] == 0)
            ফেরত সত্য;
    ফেরত মিথ্যা;
}

স্থির যৌক্তিক sameMapping(ধ্রুবক View& s, ধ্রুবক View& d)
{
    যদি (s.তথ্য != d.তথ্য)
        ফেরত মিথ্যা;
    জন্য (পূর্ণসংখ্যা i = 0; i < s.dims; i++)
        যদি (s.size[i] > 1 && s.step[i] != d.step[i])
            ফেরত মিথ্যা;
    ফেরত সত্য;
}

স্থির যৌক্তিক overlap(ধ্রুবক View& s, ধ্রুবক View& d)
{
    ধ্রুবক uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    ফেরত slo < dhi && dlo < shi;
}

} // namespace

শূন্য copyBatch(ধ্রুবক View* src, ধ্রুবক View* dst, পূর্ণসংখ্যা n)
{
    CV_Assert(n >= 0);
    Auto_বাফার<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<Auto_বাফার<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    জন্য (পূর্ণসংখ্যা i = 0; i < n; i++)
    {
        View s = src[i];
        ধ্রুবক View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (পূর্ণসংখ্যা)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        যদি (has_Zero_আকার(s) || sameMapping(s, d))
            চালু;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        যৌক্তিক need_অস্থায়ী = মিথ্যা;
        জন্য (পূর্ণসংখ্যা j = 0; j < n && !need_অস্থায়ী; j++)
            need_অস্থায়ী = !has_Zero_আকার(dst[j]) && overlap(s, dst[j]);
        যদি (need_অস্থায়ী)
        {
            size_t যোগ = 1;
            জন্য (পূর্ণসংখ্যা k = 0; k < s.dims; k++)
                যোগ *= (size_t)s.size[k];
            temps.emplace_back(যোগ*s.esz);
            View t = s;
            t.তথ্য = temps.back().তথ্য();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            জন্য (পূর্ণসংখ্যা k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        যদি (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.তথ্য(), np);
}

শূন্য copy(ধ্রুবক View& src, ধ্রুবক View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
