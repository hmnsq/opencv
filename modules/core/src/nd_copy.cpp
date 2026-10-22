// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"
#ضمن "nd_copy.hpp"
#ضمن "hal_replacement.hpp"

#ضمن "opencv2/core/hal/hal.hpp"

#ضمن <algorithm>

نطاق cv { نطاق nd {

View viewOf(ثابت Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    لكل (صحيح i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    أرجع v;
}

View permute(ثابت View& v, ثابت صحيح* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    لكل (صحيح i = 0; i < v.dims; i++)
    {
        صحيح j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    أرجع r;
}

فراغ flip(View& v, صحيح axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    𐤀𐤌 (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

فراغ slice(View& v, صحيح axis, صحيح start, صحيح step, صحيح count)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && count >= 0);
    𐤀𐤌 (count > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = count;
}

نطاق {

تعداد Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

تعداد
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

بنية Plan
{
    ثابت uchar* src;
    uchar* dst;
    size_t esz;
    صحيح dims;
    صحيح kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    منطقي chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    مزدوج cost;
};

ساكن فراغ swapAxes(Plan& p, صحيح i, صحيح j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

ساكن فراغ moveAxis(Plan& p, صحيح k, صحيح to)
{
    لكل (صحيح i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
ساكن size_t addressBits(ثابت Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    لكل (صحيح i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    أرجع addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
ساكن منطقي trySplitMerge(Plan& p)
{
    ثابت ptrdiff_t esz = (ptrdiff_t)p.esz;
    ثابت صحيح L = p.dims - 1;
    𐤀𐤌 (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        أرجع خطأ;
    𐤀𐤌 ((addressBits(p) & (esz - 1)) != 0)
        أرجع خطأ;

    // split: axis k holds cn planes interleaved in the source
    لكل (صحيح k = L - 1; k >= 0; k--)
    {
        ثابت int64 cn = p.size[k];
        𐤀𐤌 (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            أرجع true;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ثابت int64 cn = p.size[L];
    𐤀𐤌 (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        لكل (صحيح k = L - 1; k >= 0; k--)
        {
            𐤀𐤌 (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                أرجع true;
            }
        }
    }
    أرجع خطأ;
}

// Returns false when there is nothing to copy.
ساكن منطقي buildPlan(ثابت View& s, ثابت View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ثابت ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    صحيح n = 0;
    مزدوج total = 1;
    لكل (صحيح i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        صحيح sz = s.size[i];
        CV_Assert(sz >= 0);
        𐤀𐤌 (sz == 0)
            أرجع خطأ;
        total *= sz;
        𐤀𐤌 (sz == 1)
            استمر;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        𐤀𐤌 (ds < 0)
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
    لكل (صحيح i = 1; i < n; i++)
        لكل (صحيح j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    صحيح m = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        𐤀𐤌 (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        وإلا
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    𐤀𐤌 (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    ثابت صحيح L = m - 1;
    p.tfunc = nullptr;
    𐤀𐤌 (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    وإلا 𐤀𐤌 (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    وإلا 𐤀𐤌 (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        𐤀𐤌 (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            صحيح k = L - 1;
            طالما (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ثابت size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            𐤀𐤌 (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ثابت مزدوج bytes = total*(مزدوج)esz;
    𐤀𐤌 (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        ثابت صحيح X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        لكل (صحيح i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = خطأ;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    وإلا 𐤀𐤌 (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        لكل (صحيح i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = خطأ;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    وإلا
    {
        int64 nrows = 1;
        لكل (صحيح i = 0; i < L; i++)
            nrows *= p.size[i];
        ثابت int64 rowLen = p.size[L];
        ثابت مزدوج rowBytes = (مزدوج)rowLen*esz;
        ثابت ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = خطأ;
        𐤀𐤌 (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (مزدوج)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = true;
        }
        وإلا 𐤀𐤌 (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    أرجع true;
}

template<typename T> ساكن مضمن منطقي isAligned(ثابت فراغ* s, ثابت فراغ* d, ptrdiff_t ss, ptrdiff_t ds)
{
    أرجع (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> ساكن فراغ copyStrided_(ثابت uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    لكل (; i + 4 <= n; i += 4)
    {
        T t0 = *(ثابت T*)s, t1 = *(ثابت T*)(s + ss);
        T t2 = *(ثابت T*)(s + ss*2), t3 = *(ثابت T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    لكل (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ثابت T*)s;
}

template<typename T> ساكن فراغ fill_(ثابت uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ثابت T v = *(ثابت T*)s;
    لكل (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

بنية Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
ساكن فراغ copyRow(ثابت Plan& p, ثابت uchar* s, uchar* d, int64 n)
{
    ثابت size_t esz = p.esz;
    ثابت صحيح L = p.dims - 1;
    ثابت ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    𐤀𐤌 (p.kind == KIND_FILL)
    {
        𐤀𐤌 (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        وإلا 𐤀𐤌 (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        وإلا 𐤀𐤌 (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        وإلا 𐤀𐤌 (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        وإلا
            لكل (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        أرجع;
    }

    𐤀𐤌 (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    وإلا 𐤀𐤌 (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    وإلا 𐤀𐤌 (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    وإلا 𐤀𐤌 (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    وإلا 𐤀𐤌 (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    وإلا
        لكل (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
ساكن فراغ decode(ثابت Plan& p, صحيح naxes, int64 idx, int64* counters,
                   ثابت uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    لكل (صحيح k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        𐤀𐤌 (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<منطقي MEMCPY> ساكن مضمن فراغ copyRowT(ثابت Plan& p, ثابت uchar* s, uchar* d, int64 n)
{
    𐤀𐤌 (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    وإلا
        copyRow(p, s, d, n);
}

template<منطقي MEMCPY> ساكن فراغ runRows(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحيح L = p.dims - 1;
    ثابت int64 nch = p.nchunks, rowLen = p.size[L];
    ثابت ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ثابت uchar* s;
    uchar* d;

    𐤀𐤌 (p.chunkMajor)
    {
        لكل (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        أرجع;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    𐤀𐤌 (nch == 1 && L == 1)
    {
        ثابت ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        لكل (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        أرجع;
    }

    لكل (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        𐤀𐤌 (chEnd < nch)
            اكسر;
        ch = 0;
        لكل (صحيح k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            𐤀𐤌 (++counters[k] < p.size[k])
                اكسر;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

ساكن فراغ runTranspose(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحيح A = p.dims - 2, B = p.dims - 1;
    ثابت int64 ntiles = p.tilesA*p.tilesB;
    لكل (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ثابت uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        صحيح wa = (صحيح)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        صحيح hb = (صحيح)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        𐤀𐤌 (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (صحيح)p.esz) == CV_HAL_ERROR_OK)
            استمر;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

ساكن فراغ runSplitMerge(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحيح L = p.dims - 1;
    ثابت منطقي isSplit = p.kind == KIND_SPLIT;
    ثابت صحيح C = isSplit ? L - 1 : L;    // the plane axis
    ثابت صحيح X = isSplit ? L : L - 1;    // the long axis
    ثابت صحيح cn = (صحيح)p.size[C];
    ثابت size_t esz = p.esz;
    لكل (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ثابت uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        ثابت int64 e0 = ch*p.chunk;
        ثابت صحيح len = (صحيح)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        𐤀𐤌 (isSplit)
        {
            uchar* dptr[4];
            لكل (صحيح c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            𐤀𐤌 (esz == 1) hal::split8u(s, dptr, len, cn);
            وإلا 𐤀𐤌 (esz == 2) hal::split16u((ثابت ushort*)s, (ushort**)dptr, len, cn);
            وإلا 𐤀𐤌 (esz == 4) hal::split32s((ثابت صحيح*)s, (صحيح**)dptr, len, cn);
            وإلا hal::split64s((ثابت int64*)s, (int64**)dptr, len, cn);
        }
        وإلا
        {
            ثابت uchar* sptr[4];
            لكل (صحيح c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            𐤀𐤌 (esz == 1) hal::merge8u(sptr, d, len, cn);
            وإلا 𐤀𐤌 (esz == 2) hal::merge16u((ثابت ushort**)sptr, (ushort*)d, len, cn);
            وإلا 𐤀𐤌 (esz == 4) hal::merge32s((ثابت صحيح**)sptr, (صحيح*)d, len, cn);
            وإلا hal::merge64s((ثابت int64**)sptr, (int64*)d, len, cn);
        }
    }
}

ساكن فراغ runPlan(ثابت Plan& p, int64 i0, int64 i1)
{
    𐤀𐤌 (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    وإلا 𐤀𐤌 (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    وإلا 𐤀𐤌 (p.kind == KIND_MEMCPY)
        runRows<true>(p, i0, i1);
    وإلا
        runRows<خطأ>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
ساكن منطقي canRunInLockstep(ثابت Plan* plans, size_t np)
{
    𐤀𐤌 (np < 2)
        أرجع خطأ;
    ثابت Plan& p0 = plans[0];
    لكل (size_t k = 0; k < np; k++)
    {
        ثابت Plan& p = plans[k];
        𐤀𐤌 (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            أرجع خطأ;
        لكل (صحيح i = 0; i < p.dims - 1; i++)
            𐤀𐤌 (p.size[i] != p0.size[i])
                أرجع خطأ;
    }
    أرجع true;
}

ساكن فراغ runPlans(ثابت Plan* plans, size_t np)
{
    𐤀𐤌 (np == 0)
        أرجع;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    مزدوج cost = 0, rowBytes = 0;
    لكل (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (مزدوج)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ثابت منطقي lockstep = canRunInLockstep(plans, np);
    ثابت int64 total = lockstep ? plans[0].nrows : ofs[np];
    ثابت int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        𐤀𐤌 (lockstep)
        {
            لكل (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                لكل (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            أرجع;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        طالما (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    𐤀𐤌 (cost < PARALLEL_MIN_COST || total < 2 || getNumThreads() <= 1)
    {
        body(0, total);
        أرجع;
    }

    // grouped so that the range fits into int
    ثابت int64 ngroups = std::min(total, (int64)1 << 20);
    ثابت مزدوج nstripes = std::min((مزدوج)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (صحيح)ngroups), [&](ثابت Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
ساكن فراغ viewRange(ثابت View& v, ثابت uchar*& lo, ثابت uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    لكل (صحيح i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        𐤀𐤌 (ext < 0) a += ext; وإلا b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

ساكن منطقي hasZeroSize(ثابت View& v)
{
    لكل (صحيح i = 0; i < v.dims; i++)
        𐤀𐤌 (v.size[i] == 0)
            أرجع true;
    أرجع خطأ;
}

ساكن منطقي sameMapping(ثابت View& s, ثابت View& d)
{
    𐤀𐤌 (s.data != d.data)
        أرجع خطأ;
    لكل (صحيح i = 0; i < s.dims; i++)
        𐤀𐤌 (s.size[i] > 1 && s.step[i] != d.step[i])
            أرجع خطأ;
    أرجع true;
}

ساكن منطقي overlap(ثابت View& s, ثابت View& d)
{
    ثابت uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    أرجع slo < dhi && dlo < shi;
}

} // namespace

فراغ copyBatch(ثابت View* src, ثابت View* dst, صحيح n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    لكل (صحيح i = 0; i < n; i++)
    {
        View s = src[i];
        ثابت View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (صحيح)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        𐤀𐤌 (hasZeroSize(s) || sameMapping(s, d))
            استمر;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        منطقي needTemp = خطأ;
        لكل (صحيح j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        𐤀𐤌 (needTemp)
        {
            size_t total = 1;
            لكل (صحيح k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            لكل (صحيح k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        𐤀𐤌 (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

فراغ copy(ثابت View& src, ثابت View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
