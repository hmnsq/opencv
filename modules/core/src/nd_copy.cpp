// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "nd_copy.hpp"
#অন্তর্ভাৱ "hal_replacement.hpp"

#অন্তর্ভাৱ "opencv2/core/hal/hal.hpp"

#অন্তর্ভাৱ <algorithm>

নামাকাশ cv { নামাকাশ nd {

View viewOf(ধ্রুৱ Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    বাবে (পূর্ণাঙ্ক i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    ঘূৰাই v;
}

View permute(ধ্রুৱ View& v, ধ্রুৱ পূর্ণাঙ্ক* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    বাবে (পূর্ণাঙ্ক i = 0; i < v.dims; i++)
    {
        পূর্ণাঙ্ক j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    ঘূৰাই r;
}

শূন্য flip(View& v, পূর্ণাঙ্ক axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    যদি (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

শূন্য slice(View& v, পূর্ণাঙ্ক axis, পূর্ণাঙ্ক start, পূর্ণাঙ্ক step, পূর্ণাঙ্ক গুনতি_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && গুনতি_2 >= 0);
    যদি (গুনতি_2 > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
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

সংরচনা Plan
{
    ধ্রুৱ uchar* src;
    uchar* dst;
    size_t esz;
    পূর্ণাঙ্ক dims;
    পূর্ণাঙ্ক kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    তার্কিক chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    দ্ৱিগুণ cost;
};

স্থির শূন্য swapAxes(Plan& p, পূর্ণাঙ্ক i, পূর্ণাঙ্ক j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

স্থির শূন্য moveAxis(Plan& p, পূর্ণাঙ্ক k, পূর্ণাঙ্ক to)
{
    বাবে (পূর্ণাঙ্ক i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
স্থির size_t addressBits(ধ্রুৱ Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    বাবে (পূর্ণাঙ্ক i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    ঘূৰাই addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
স্থির তার্কিক trySplitMerge(Plan& p)
{
    ধ্রুৱ ptrdiff_t esz = (ptrdiff_t)p.esz;
    ধ্রুৱ পূর্ণাঙ্ক L = p.dims - 1;
    যদি (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        ঘূৰাই মিছা;
    যদি ((addressBits(p) & (esz - 1)) != 0)
        ঘূৰাই মিছা;

    // split: axis k holds cn planes interleaved in the source
    বাবে (পূর্ণাঙ্ক k = L - 1; k >= 0; k--)
    {
        ধ্রুৱ int64 cn = p.size[k];
        যদি (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            ঘূৰাই সঁচা;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ধ্রুৱ int64 cn = p.size[L];
    যদি (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        বাবে (পূর্ণাঙ্ক k = L - 1; k >= 0; k--)
        {
            যদি (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                ঘূৰাই সঁচা;
            }
        }
    }
    ঘূৰাই মিছা;
}

// Returns false when there is nothing to copy.
স্থির তার্কিক buildPlan(ধ্রুৱ View& s, ধ্রুৱ View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ধ্রুৱ ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    পূর্ণাঙ্ক n = 0;
    দ্ৱিগুণ total = 1;
    বাবে (পূর্ণাঙ্ক i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        পূর্ণাঙ্ক sz = s.size[i];
        CV_Assert(sz >= 0);
        যদি (sz == 0)
            ঘূৰাই মিছা;
        total *= sz;
        যদি (sz == 1)
            অনুৱর্ত;
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
    বাবে (পূর্ণাঙ্ক i = 1; i < n; i++)
        বাবে (পূর্ণাঙ্ক j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    পূর্ণাঙ্ক m = 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
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

    ধ্রুৱ পূর্ণাঙ্ক L = m - 1;
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
            পূর্ণাঙ্ক k = L - 1;
            যেতিয়ালৈ (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ধ্রুৱ size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            যদি (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ধ্রুৱ দ্ৱিগুণ bytes = total*(দ্ৱিগুণ)esz;
    যদি (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        ধ্রুৱ পূর্ণাঙ্ক X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        বাবে (পূর্ণাঙ্ক i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = মিছা;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    অন্যথা যদি (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        বাবে (পূর্ণাঙ্ক i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = মিছা;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    অন্যথা
    {
        int64 nrows = 1;
        বাবে (পূর্ণাঙ্ক i = 0; i < L; i++)
            nrows *= p.size[i];
        ধ্রুৱ int64 rowLen = p.size[L];
        ধ্রুৱ দ্ৱিগুণ rowBytes = (দ্ৱিগুণ)rowLen*esz;
        ধ্রুৱ ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = মিছা;
        যদি (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (দ্ৱিগুণ)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = সঁচা;
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
    ঘূৰাই সঁচা;
}

template<typename T> স্থির অন্তঃস্থ তার্কিক isAligned(ধ্রুৱ শূন্য* s, ধ্রুৱ শূন্য* d, ptrdiff_t ss, ptrdiff_t ds)
{
    ঘূৰাই (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> স্থির শূন্য copyStrided_(ধ্রুৱ uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    বাবে (; i + 4 <= n; i += 4)
    {
        T t0 = *(ধ্রুৱ T*)s, t1 = *(ধ্রুৱ T*)(s + ss);
        T t2 = *(ধ্রুৱ T*)(s + ss*2), t3 = *(ধ্রুৱ T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    বাবে (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ধ্রুৱ T*)s;
}

template<typename T> স্থির শূন্য fill_(ধ্রুৱ uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ধ্রুৱ T v = *(ধ্রুৱ T*)s;
    বাবে (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

সংরচনা Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
স্থির শূন্য copyRow(ধ্রুৱ Plan& p, ধ্রুৱ uchar* s, uchar* d, int64 n)
{
    ধ্রুৱ size_t esz = p.esz;
    ধ্রুৱ পূর্ণাঙ্ক L = p.dims - 1;
    ধ্রুৱ ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

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
            বাবে (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        ঘূৰাই;
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
        বাবে (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
স্থির শূন্য decode(ধ্রুৱ Plan& p, পূর্ণাঙ্ক naxes, int64 idx, int64* counters,
                   ধ্রুৱ uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    বাবে (পূর্ণাঙ্ক k = naxes - 1; k >= 0; k--)
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

template<তার্কিক MEMCPY> স্থির অন্তঃস্থ শূন্য copyRowT(ধ্রুৱ Plan& p, ধ্রুৱ uchar* s, uchar* d, int64 n)
{
    যদি (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    অন্যথা
        copyRow(p, s, d, n);
}

template<তার্কিক MEMCPY> স্থির শূন্য runRows(ধ্রুৱ Plan& p, int64 i0, int64 i1)
{
    ধ্রুৱ পূর্ণাঙ্ক L = p.dims - 1;
    ধ্রুৱ int64 nch = p.nchunks, rowLen = p.size[L];
    ধ্রুৱ ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ধ্রুৱ uchar* s;
    uchar* d;

    যদি (p.chunkMajor)
    {
        বাবে (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        ঘূৰাই;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    যদি (nch == 1 && L == 1)
    {
        ধ্রুৱ ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        বাবে (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        ঘূৰাই;
    }

    বাবে (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        যদি (chEnd < nch)
            ভঙ্গ;
        ch = 0;
        বাবে (পূর্ণাঙ্ক k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            যদি (++counters[k] < p.size[k])
                ভঙ্গ;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

স্থির শূন্য runTranspose(ধ্রুৱ Plan& p, int64 i0, int64 i1)
{
    ধ্রুৱ পূর্ণাঙ্ক A = p.dims - 2, B = p.dims - 1;
    ধ্রুৱ int64 ntiles = p.tilesA*p.tilesB;
    বাবে (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ধ্রুৱ uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        পূর্ণাঙ্ক wa = (পূর্ণাঙ্ক)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        পূর্ণাঙ্ক hb = (পূর্ণাঙ্ক)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        যদি (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (পূর্ণাঙ্ক)p.esz) == CV_HAL_ERROR_OK)
            অনুৱর্ত;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

স্থির শূন্য runSplitMerge(ধ্রুৱ Plan& p, int64 i0, int64 i1)
{
    ধ্রুৱ পূর্ণাঙ্ক L = p.dims - 1;
    ধ্রুৱ তার্কিক isSplit = p.kind == KIND_SPLIT;
    ধ্রুৱ পূর্ণাঙ্ক C = isSplit ? L - 1 : L;    // the plane axis
    ধ্রুৱ পূর্ণাঙ্ক X = isSplit ? L : L - 1;    // the long axis
    ধ্রুৱ পূর্ণাঙ্ক cn = (পূর্ণাঙ্ক)p.size[C];
    ধ্রুৱ size_t esz = p.esz;
    বাবে (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ধ্রুৱ uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        ধ্রুৱ int64 e0 = ch*p.chunk;
        ধ্রুৱ পূর্ণাঙ্ক len = (পূর্ণাঙ্ক)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        যদি (isSplit)
        {
            uchar* dptr[4];
            বাবে (পূর্ণাঙ্ক c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            যদি (esz == 1) hal::split8u(s, dptr, len, cn);
            অন্যথা যদি (esz == 2) hal::split16u((ধ্রুৱ ushort*)s, (ushort**)dptr, len, cn);
            অন্যথা যদি (esz == 4) hal::split32s((ধ্রুৱ পূর্ণাঙ্ক*)s, (পূর্ণাঙ্ক**)dptr, len, cn);
            অন্যথা hal::split64s((ধ্রুৱ int64*)s, (int64**)dptr, len, cn);
        }
        অন্যথা
        {
            ধ্রুৱ uchar* sptr[4];
            বাবে (পূর্ণাঙ্ক c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            যদি (esz == 1) hal::merge8u(sptr, d, len, cn);
            অন্যথা যদি (esz == 2) hal::merge16u((ধ্রুৱ ushort**)sptr, (ushort*)d, len, cn);
            অন্যথা যদি (esz == 4) hal::merge32s((ধ্রুৱ পূর্ণাঙ্ক**)sptr, (পূর্ণাঙ্ক*)d, len, cn);
            অন্যথা hal::merge64s((ধ্রুৱ int64**)sptr, (int64*)d, len, cn);
        }
    }
}

স্থির শূন্য runPlan(ধ্রুৱ Plan& p, int64 i0, int64 i1)
{
    যদি (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    অন্যথা যদি (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    অন্যথা যদি (p.kind == KIND_MEMCPY)
        runRows<সঁচা>(p, i0, i1);
    অন্যথা
        runRows<মিছা>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
স্থির তার্কিক canRunInLockstep(ধ্রুৱ Plan* plans, size_t np)
{
    যদি (np < 2)
        ঘূৰাই মিছা;
    ধ্রুৱ Plan& p0 = plans[0];
    বাবে (size_t k = 0; k < np; k++)
    {
        ধ্রুৱ Plan& p = plans[k];
        যদি (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            ঘূৰাই মিছা;
        বাবে (পূর্ণাঙ্ক i = 0; i < p.dims - 1; i++)
            যদি (p.size[i] != p0.size[i])
                ঘূৰাই মিছা;
    }
    ঘূৰাই সঁচা;
}

স্থির শূন্য runPlans(ধ্রুৱ Plan* plans, size_t np)
{
    যদি (np == 0)
        ঘূৰাই;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    দ্ৱিগুণ cost = 0, rowBytes = 0;
    বাবে (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (দ্ৱিগুণ)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ধ্রুৱ তার্কিক lockstep = canRunInLockstep(plans, np);
    ধ্রুৱ int64 total = lockstep ? plans[0].nrows : ofs[np];
    ধ্রুৱ int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        যদি (lockstep)
        {
            বাবে (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                বাবে (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            ঘূৰাই;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        যেতিয়ালৈ (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    যদি (cost < PARALLEL_MIN_COST || total < 2 || get_অঙ্ক_Threads() <= 1)
    {
        body(0, total);
        ঘূৰাই;
    }

    // grouped so that the range fits into int
    ধ্রুৱ int64 ngroups = std::min(total, (int64)1 << 20);
    ধ্রুৱ দ্ৱিগুণ nstripes = std::min((দ্ৱিগুণ)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (পূর্ণাঙ্ক)ngroups), [&](ধ্রুৱ Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
স্থির শূন্য viewRange(ধ্রুৱ View& v, ধ্রুৱ uchar*& lo, ধ্রুৱ uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        যদি (ext < 0) a += ext; অন্যথা b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

স্থির তার্কিক hasZeroSize(ধ্রুৱ View& v)
{
    বাবে (পূর্ণাঙ্ক i = 0; i < v.dims; i++)
        যদি (v.size[i] == 0)
            ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
}

স্থির তার্কিক sameMapping(ধ্রুৱ View& s, ধ্রুৱ View& d)
{
    যদি (s.data != d.data)
        ঘূৰাই মিছা;
    বাবে (পূর্ণাঙ্ক i = 0; i < s.dims; i++)
        যদি (s.size[i] > 1 && s.step[i] != d.step[i])
            ঘূৰাই মিছা;
    ঘূৰাই সঁচা;
}

স্থির তার্কিক overlap(ধ্রুৱ View& s, ধ্রুৱ View& d)
{
    ধ্রুৱ uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    ঘূৰাই slo < dhi && dlo < shi;
}

} // namespace

শূন্য copyBatch(ধ্রুৱ View* src, ধ্রুৱ View* dst, পূর্ণাঙ্ক n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
    {
        View s = src[i];
        ধ্রুৱ View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (পূর্ণাঙ্ক)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        যদি (hasZeroSize(s) || sameMapping(s, d))
            অনুৱর্ত;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        তার্কিক needTemp = মিছা;
        বাবে (পূর্ণাঙ্ক j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        যদি (needTemp)
        {
            size_t total = 1;
            বাবে (পূর্ণাঙ্ক k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            বাবে (পূর্ণাঙ্ক k = s.dims - 1; k >= 0; k--)
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
    runPlans(plans.data(), np);
}

শূন্য copy(ধ্রুৱ View& src, ধ্রুৱ View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
