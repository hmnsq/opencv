// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#شامل "precomp.hpp"
#شامل "nd_copy.hpp"
#شامل "hal_replacement.hpp"

#شامل "opencv2/core/hal/hal.hpp"

#شامل <algorithm>

فضاینام cv { فضاینام nd {

View viewOf(ثابت Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    برای (صحیح i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    بازگشت v;
}

View permute(ثابت View& v, ثابت صحیح* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    برای (صحیح i = 0; i < v.dims; i++)
    {
        صحیح j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    بازگشت r;
}

تهی flip(View& v, صحیح axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    اگر (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

تهی slice(View& v, صحیح axis, صحیح start, صحیح step, صحیح شمار_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && شمار_2 >= 0);
    اگر (شمار_2 > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = شمار_2;
}

فضاینام {

شمارش Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

شمارش
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

ساختار Plan
{
    ثابت uchar* src;
    uchar* dst;
    size_t esz;
    صحیح dims;
    صحیح kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    منطقی chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    دوگانه cost;
};

ایستا تهی swapAxes(Plan& p, صحیح i, صحیح j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

ایستا تهی moveAxis(Plan& p, صحیح k, صحیح to)
{
    برای (صحیح i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
ایستا size_t addressBits(ثابت Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    برای (صحیح i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    بازگشت addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
ایستا منطقی trySplitMerge(Plan& p)
{
    ثابت ptrdiff_t esz = (ptrdiff_t)p.esz;
    ثابت صحیح L = p.dims - 1;
    اگر (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        بازگشت نادرست;
    اگر ((addressBits(p) & (esz - 1)) != 0)
        بازگشت نادرست;

    // split: axis k holds cn planes interleaved in the source
    برای (صحیح k = L - 1; k >= 0; k--)
    {
        ثابت int64 cn = p.size[k];
        اگر (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            بازگشت درست;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ثابت int64 cn = p.size[L];
    اگر (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        برای (صحیح k = L - 1; k >= 0; k--)
        {
            اگر (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                بازگشت درست;
            }
        }
    }
    بازگشت نادرست;
}

// Returns false when there is nothing to copy.
ایستا منطقی buildPlan(ثابت View& s, ثابت View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ثابت ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    صحیح n = 0;
    دوگانه total = 1;
    برای (صحیح i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        صحیح sz = s.size[i];
        CV_Assert(sz >= 0);
        اگر (sz == 0)
            بازگشت نادرست;
        total *= sz;
        اگر (sz == 1)
            ادامه;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        اگر (ds < 0)
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
    برای (صحیح i = 1; i < n; i++)
        برای (صحیح j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    صحیح m = 0;
    برای (صحیح i = 0; i < n; i++)
    {
        اگر (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        وگرنه
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    اگر (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    ثابت صحیح L = m - 1;
    p.tfunc = nullptr;
    اگر (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    وگرنه اگر (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    وگرنه اگر (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        اگر (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            صحیح k = L - 1;
            وقتی (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ثابت size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            اگر (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ثابت دوگانه bytes = total*(دوگانه)esz;
    اگر (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        ثابت صحیح X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        برای (صحیح i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = نادرست;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    وگرنه اگر (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        برای (صحیح i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = نادرست;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    وگرنه
    {
        int64 nrows = 1;
        برای (صحیح i = 0; i < L; i++)
            nrows *= p.size[i];
        ثابت int64 rowLen = p.size[L];
        ثابت دوگانه rowBytes = (دوگانه)rowLen*esz;
        ثابت ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = نادرست;
        اگر (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (دوگانه)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = درست;
        }
        وگرنه اگر (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    بازگشت درست;
}

template<typename T> ایستا درونخطی منطقی isAligned(ثابت تهی* s, ثابت تهی* d, ptrdiff_t ss, ptrdiff_t ds)
{
    بازگشت (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> ایستا تهی copyStrided_(ثابت uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    برای (; i + 4 <= n; i += 4)
    {
        T t0 = *(ثابت T*)s, t1 = *(ثابت T*)(s + ss);
        T t2 = *(ثابت T*)(s + ss*2), t3 = *(ثابت T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    برای (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ثابت T*)s;
}

template<typename T> ایستا تهی fill_(ثابت uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ثابت T v = *(ثابت T*)s;
    برای (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

ساختار Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
ایستا تهی copyRow(ثابت Plan& p, ثابت uchar* s, uchar* d, int64 n)
{
    ثابت size_t esz = p.esz;
    ثابت صحیح L = p.dims - 1;
    ثابت ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    اگر (p.kind == KIND_FILL)
    {
        اگر (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        وگرنه اگر (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        وگرنه اگر (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        وگرنه اگر (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        وگرنه
            برای (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        بازگشت;
    }

    اگر (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    وگرنه اگر (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    وگرنه اگر (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    وگرنه اگر (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    وگرنه اگر (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    وگرنه
        برای (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
ایستا تهی decode(ثابت Plan& p, صحیح naxes, int64 idx, int64* counters,
                   ثابت uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    برای (صحیح k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        اگر (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<منطقی MEMCPY> ایستا درونخطی تهی copyRowT(ثابت Plan& p, ثابت uchar* s, uchar* d, int64 n)
{
    اگر (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    وگرنه
        copyRow(p, s, d, n);
}

template<منطقی MEMCPY> ایستا تهی runRows(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحیح L = p.dims - 1;
    ثابت int64 nch = p.nchunks, rowLen = p.size[L];
    ثابت ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ثابت uchar* s;
    uchar* d;

    اگر (p.chunkMajor)
    {
        برای (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        بازگشت;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    اگر (nch == 1 && L == 1)
    {
        ثابت ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        برای (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        بازگشت;
    }

    برای (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        اگر (chEnd < nch)
            بشکن;
        ch = 0;
        برای (صحیح k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            اگر (++counters[k] < p.size[k])
                بشکن;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

ایستا تهی runTranspose(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحیح A = p.dims - 2, B = p.dims - 1;
    ثابت int64 ntiles = p.tilesA*p.tilesB;
    برای (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ثابت uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        صحیح wa = (صحیح)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        صحیح hb = (صحیح)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        اگر (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (صحیح)p.esz) == CV_HAL_ERROR_OK)
            ادامه;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

ایستا تهی runSplitMerge(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحیح L = p.dims - 1;
    ثابت منطقی isSplit = p.kind == KIND_SPLIT;
    ثابت صحیح C = isSplit ? L - 1 : L;    // the plane axis
    ثابت صحیح X = isSplit ? L : L - 1;    // the long axis
    ثابت صحیح cn = (صحیح)p.size[C];
    ثابت size_t esz = p.esz;
    برای (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ثابت uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        ثابت int64 e0 = ch*p.chunk;
        ثابت صحیح len = (صحیح)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        اگر (isSplit)
        {
            uchar* dptr[4];
            برای (صحیح c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            اگر (esz == 1) hal::split8u(s, dptr, len, cn);
            وگرنه اگر (esz == 2) hal::split16u((ثابت ushort*)s, (ushort**)dptr, len, cn);
            وگرنه اگر (esz == 4) hal::split32s((ثابت صحیح*)s, (صحیح**)dptr, len, cn);
            وگرنه hal::split64s((ثابت int64*)s, (int64**)dptr, len, cn);
        }
        وگرنه
        {
            ثابت uchar* sptr[4];
            برای (صحیح c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            اگر (esz == 1) hal::merge8u(sptr, d, len, cn);
            وگرنه اگر (esz == 2) hal::merge16u((ثابت ushort**)sptr, (ushort*)d, len, cn);
            وگرنه اگر (esz == 4) hal::merge32s((ثابت صحیح**)sptr, (صحیح*)d, len, cn);
            وگرنه hal::merge64s((ثابت int64**)sptr, (int64*)d, len, cn);
        }
    }
}

ایستا تهی runPlan(ثابت Plan& p, int64 i0, int64 i1)
{
    اگر (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    وگرنه اگر (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    وگرنه اگر (p.kind == KIND_MEMCPY)
        runRows<درست>(p, i0, i1);
    وگرنه
        runRows<نادرست>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
ایستا منطقی canRunInLockstep(ثابت Plan* plans, size_t np)
{
    اگر (np < 2)
        بازگشت نادرست;
    ثابت Plan& p0 = plans[0];
    برای (size_t k = 0; k < np; k++)
    {
        ثابت Plan& p = plans[k];
        اگر (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            بازگشت نادرست;
        برای (صحیح i = 0; i < p.dims - 1; i++)
            اگر (p.size[i] != p0.size[i])
                بازگشت نادرست;
    }
    بازگشت درست;
}

ایستا تهی runPlans(ثابت Plan* plans, size_t np)
{
    اگر (np == 0)
        بازگشت;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    دوگانه cost = 0, rowBytes = 0;
    برای (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (دوگانه)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ثابت منطقی lockstep = canRunInLockstep(plans, np);
    ثابت int64 total = lockstep ? plans[0].nrows : ofs[np];
    ثابت int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        اگر (lockstep)
        {
            برای (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                برای (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            بازگشت;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        وقتی (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    اگر (cost < PARALLEL_MIN_COST || total < 2 || get_رقم_Threads() <= 1)
    {
        body(0, total);
        بازگشت;
    }

    // grouped so that the range fits into int
    ثابت int64 ngroups = std::min(total, (int64)1 << 20);
    ثابت دوگانه nstripes = std::min((دوگانه)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (صحیح)ngroups), [&](ثابت Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
ایستا تهی viewRange(ثابت View& v, ثابت uchar*& lo, ثابت uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    برای (صحیح i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        اگر (ext < 0) a += ext; وگرنه b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

ایستا منطقی hasZeroSize(ثابت View& v)
{
    برای (صحیح i = 0; i < v.dims; i++)
        اگر (v.size[i] == 0)
            بازگشت درست;
    بازگشت نادرست;
}

ایستا منطقی sameMapping(ثابت View& s, ثابت View& d)
{
    اگر (s.data != d.data)
        بازگشت نادرست;
    برای (صحیح i = 0; i < s.dims; i++)
        اگر (s.size[i] > 1 && s.step[i] != d.step[i])
            بازگشت نادرست;
    بازگشت درست;
}

ایستا منطقی overlap(ثابت View& s, ثابت View& d)
{
    ثابت uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    بازگشت slo < dhi && dlo < shi;
}

} // namespace

تهی copyBatch(ثابت View* src, ثابت View* dst, صحیح n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    برای (صحیح i = 0; i < n; i++)
    {
        View s = src[i];
        ثابت View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (صحیح)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        اگر (hasZeroSize(s) || sameMapping(s, d))
            ادامه;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        منطقی needTemp = نادرست;
        برای (صحیح j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        اگر (needTemp)
        {
            size_t total = 1;
            برای (صحیح k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            برای (صحیح k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        اگر (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

تهی copy(ثابت View& src, ثابت View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
