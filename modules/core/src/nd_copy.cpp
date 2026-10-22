// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#include "precomp.hpp"
#include "nd_copy.hpp"
#include "hal_replacement.hpp"

#include "opencv2/core/hal/hal.hpp"

#include <algorithm>

namespace cv { namespace nd {

View viewOf(const Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    위해 (정수 i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (ptrdiff_t)m.step[i];
    }
    반환 v;
}

View permute(const View& v, const 정수* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    위해 (정수 i = 0; i < v.dims; i++)
    {
        정수 j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    반환 r;
}

void flip(View& v, 정수 axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    만약 (v.size[axis] > 0)
        v.data += (ptrdiff_t)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

void slice(View& v, 정수 axis, 정수 start, 정수 step, 정수 count)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && count >= 0);
    만약 (count > 0)
        v.data += (ptrdiff_t)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = count;
}

namespace {

enum Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

enum
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

struct Plan
{
    const uchar* src;
    uchar* dst;
    size_t esz;
    정수 dims;
    정수 kind;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    bool chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    double cost;
};

static void swapAxes(Plan& p, 정수 i, 정수 j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

static void moveAxis(Plan& p, 정수 k, 정수 to)
{
    위해 (정수 i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
static size_t addressBits(const Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    위해 (정수 i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    반환 addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
static bool trySplitMerge(Plan& p)
{
    const ptrdiff_t esz = (ptrdiff_t)p.esz;
    const 정수 L = p.dims - 1;
    만약 (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        반환 false;
    만약 ((addressBits(p) & (esz - 1)) != 0)
        반환 false;

    // split: axis k holds cn planes interleaved in the source
    위해 (정수 k = L - 1; k >= 0; k--)
    {
        const int64 cn = p.size[k];
        만약 (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            반환 true;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    const int64 cn = p.size[L];
    만약 (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        위해 (정수 k = L - 1; k >= 0; k--)
        {
            만약 (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                반환 true;
            }
        }
    }
    반환 false;
}

// Returns false when there is nothing to copy.
static bool buildPlan(const View& s, const View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    const ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    정수 n = 0;
    double total = 1;
    위해 (정수 i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        정수 sz = s.size[i];
        CV_Assert(sz >= 0);
        만약 (sz == 0)
            반환 false;
        total *= sz;
        만약 (sz == 1)
            continue;
        ptrdiff_t ss = s.step[i], ds = d.step[i];
        만약 (ds < 0)
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
    위해 (정수 i = 1; i < n; i++)
        위해 (정수 j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    정수 m = 0;
    위해 (정수 i = 0; i < n; i++)
    {
        만약 (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        아니면
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    만약 (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    const 정수 L = m - 1;
    p.tfunc = nullptr;
    만약 (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    아니면 만약 (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    아니면 만약 (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        만약 (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            정수 k = L - 1;
            동안 (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            const size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            만약 (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    const double bytes = total*(double)esz;
    만약 (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        const 정수 X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        위해 (정수 i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = false;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    아니면 만약 (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        위해 (정수 i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = false;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    아니면
    {
        int64 nrows = 1;
        위해 (정수 i = 0; i < L; i++)
            nrows *= p.size[i];
        const int64 rowLen = p.size[L];
        const double rowBytes = (double)rowLen*esz;
        const ptrdiff_t sstepL = std::abs(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = false;
        만약 (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::abs(p.sstep[L-1]) < sstepL &&
            (double)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = true;
        }
        아니면 만약 (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    반환 true;
}

template<typename T> static inline bool isAligned(const void* s, const void* d, ptrdiff_t ss, ptrdiff_t ds)
{
    반환 (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

template<typename T> static void copyStrided_(const uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    위해 (; i + 4 <= n; i += 4)
    {
        T t0 = *(const T*)s, t1 = *(const T*)(s + ss);
        T t2 = *(const T*)(s + ss*2), t3 = *(const T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    위해 (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(const T*)s;
}

template<typename T> static void fill_(const uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    const T v = *(const T*)s;
    위해 (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

struct Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
static void copyRow(const Plan& p, const uchar* s, uchar* d, int64 n)
{
    const size_t esz = p.esz;
    const 정수 L = p.dims - 1;
    const ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    만약 (p.kind == KIND_FILL)
    {
        만약 (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        아니면 만약 (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        아니면 만약 (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        아니면 만약 (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        아니면
            위해 (int64 i = 0; i < n; i++, d += ds)
                memcpy(d, s, esz);
        반환;
    }

    만약 (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    아니면 만약 (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    아니면 만약 (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    아니면 만약 (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    아니면 만약 (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    아니면
        위해 (int64 i = 0; i < n; i++, s += ss, d += ds)
            memcpy(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
static void decode(const Plan& p, 정수 naxes, int64 idx, int64* counters,
                   const uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    위해 (정수 k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        만약 (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

template<bool MEMCPY> static inline void copyRowT(const Plan& p, const uchar* s, uchar* d, int64 n)
{
    만약 (MEMCPY)
        memcpy(d, s, (size_t)n*p.esz);
    아니면
        copyRow(p, s, d, n);
}

template<bool MEMCPY> static void runRows(const Plan& p, int64 i0, int64 i1)
{
    const 정수 L = p.dims - 1;
    const int64 nch = p.nchunks, rowLen = p.size[L];
    const ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    const uchar* s;
    uchar* d;

    만약 (p.chunkMajor)
    {
        위해 (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        반환;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    만약 (nch == 1 && L == 1)
    {
        const ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        위해 (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        반환;
    }

    위해 (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        만약 (chEnd < nch)
            break;
        ch = 0;
        위해 (정수 k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            만약 (++counters[k] < p.size[k])
                break;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

static void runTranspose(const Plan& p, int64 i0, int64 i1)
{
    const 정수 A = p.dims - 2, B = p.dims - 1;
    const int64 ntiles = p.tilesA*p.tilesB;
    위해 (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        const uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        정수 wa = (정수)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        정수 hb = (정수)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        만약 (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (정수)p.esz) == CV_HAL_ERROR_OK)
            continue;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], Size(wa, hb));
    }
}

static void runSplitMerge(const Plan& p, int64 i0, int64 i1)
{
    const 정수 L = p.dims - 1;
    const bool isSplit = p.kind == KIND_SPLIT;
    const 정수 C = isSplit ? L - 1 : L;    // the plane axis
    const 정수 X = isSplit ? L : L - 1;    // the long axis
    const 정수 cn = (정수)p.size[C];
    const size_t esz = p.esz;
    위해 (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        const uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        const int64 e0 = ch*p.chunk;
        const 정수 len = (정수)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        만약 (isSplit)
        {
            uchar* dptr[4];
            위해 (정수 c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            만약 (esz == 1) hal::split8u(s, dptr, len, cn);
            아니면 만약 (esz == 2) hal::split16u((const ushort*)s, (ushort**)dptr, len, cn);
            아니면 만약 (esz == 4) hal::split32s((const 정수*)s, (정수**)dptr, len, cn);
            아니면 hal::split64s((const int64*)s, (int64**)dptr, len, cn);
        }
        아니면
        {
            const uchar* sptr[4];
            위해 (정수 c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            만약 (esz == 1) hal::merge8u(sptr, d, len, cn);
            아니면 만약 (esz == 2) hal::merge16u((const ushort**)sptr, (ushort*)d, len, cn);
            아니면 만약 (esz == 4) hal::merge32s((const 정수**)sptr, (정수*)d, len, cn);
            아니면 hal::merge64s((const int64**)sptr, (int64*)d, len, cn);
        }
    }
}

static void runPlan(const Plan& p, int64 i0, int64 i1)
{
    만약 (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    아니면 만약 (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    아니면 만약 (p.kind == KIND_MEMCPY)
        runRows<true>(p, i0, i1);
    아니면
        runRows<false>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
static bool canRunInLockstep(const Plan* plans, size_t np)
{
    만약 (np < 2)
        반환 false;
    const Plan& p0 = plans[0];
    위해 (size_t k = 0; k < np; k++)
    {
        const Plan& p = plans[k];
        만약 (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            반환 false;
        위해 (정수 i = 0; i < p.dims - 1; i++)
            만약 (p.size[i] != p0.size[i])
                반환 false;
    }
    반환 true;
}

static void runPlans(const Plan* plans, size_t np)
{
    만약 (np == 0)
        반환;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    double cost = 0, rowBytes = 0;
    위해 (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (double)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    const bool lockstep = canRunInLockstep(plans, np);
    const int64 total = lockstep ? plans[0].nrows : ofs[np];
    const int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    auto body = [&](int64 a, int64 b)
    {
        만약 (lockstep)
        {
            위해 (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                위해 (size_t k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            반환;
        }
        size_t k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        동안 (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    만약 (cost < PARALLEL_MIN_COST || total < 2 || getNumThreads() <= 1)
    {
        body(0, total);
        반환;
    }

    // grouped so that the range fits into int
    const int64 ngroups = std::min(total, (int64)1 << 20);
    const double nstripes = std::min((double)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (정수)ngroups), [&](const Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
static void viewRange(const View& v, const uchar*& lo, const uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    위해 (정수 i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.step[i];
        만약 (ext < 0) a += ext; 아니면 b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

static bool hasZeroSize(const View& v)
{
    위해 (정수 i = 0; i < v.dims; i++)
        만약 (v.size[i] == 0)
            반환 true;
    반환 false;
}

static bool sameMapping(const View& s, const View& d)
{
    만약 (s.data != d.data)
        반환 false;
    위해 (정수 i = 0; i < s.dims; i++)
        만약 (s.size[i] > 1 && s.step[i] != d.step[i])
            반환 false;
    반환 true;
}

static bool overlap(const View& s, const View& d)
{
    const uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    반환 slo < dhi && dlo < shi;
}

} // namespace

void copyBatch(const View* src, const View* dst, 정수 n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    위해 (정수 i = 0; i < n; i++)
    {
        View s = src[i];
        const View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (정수)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        만약 (hasZeroSize(s) || sameMapping(s, d))
            continue;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        bool needTemp = false;
        위해 (정수 j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        만약 (needTemp)
        {
            size_t total = 1;
            위해 (정수 k = 0; k < s.dims; k++)
                total *= (size_t)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            위해 (정수 k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        만약 (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

void copy(const View& src, const View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
