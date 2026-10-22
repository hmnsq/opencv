// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#समावेश "precomp.hpp"
#समावेश "nd_copy.hpp"
#समावेश "hal_replacement.hpp"

#समावेश "opencv2/core/hal/hal.hpp"

#समावेश <algorithm>

namespace cv { namespace nd {

View viewOf(स्थिर Mat& m)
{
    View v;
    v.तथ्य = m.तथ्य;
    v.dims = m.dims;
    v.esz = m.elem_आकार();
    क्रम (पूर्णांक i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (सूचक_भेद_प्रकार)m.step[i];
    }
    वापस v;
}

View permute(स्थिर View& v, स्थिर पूर्णांक* order)
{
    View r;
    r.तथ्य = v.तथ्य;
    r.dims = v.dims;
    r.esz = v.esz;
    क्रम (पूर्णांक i = 0; i < v.dims; i++)
    {
        पूर्णांक j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    वापस r;
}

व्योम flip(View& v, पूर्णांक axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    अगर (v.size[axis] > 0)
        v.तथ्य += (सूचक_भेद_प्रकार)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

व्योम slice(View& v, पूर्णांक axis, पूर्णांक start, पूर्णांक step, पूर्णांक गिनती_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && गिनती_2 >= 0);
    अगर (गिनती_2 > 0)
        v.तथ्य += (सूचक_भेद_प्रकार)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = गिनती_2;
}

namespace {

क्रमागत Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

क्रमागत
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

काष्ठा Plan
{
    स्थिर uchar* src;
    uchar* dst;
    माप_प्रकार esz;
    पूर्णांक dims;
    पूर्णांक kind;
    int64 size[MAX_VIEW_DIMS];
    सूचक_भेद_प्रकार sstep[MAX_VIEW_DIMS];
    सूचक_भेद_प्रकार dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    bool chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    द्विगुन cost;
};

अटल व्योम swapAxes(Plan& p, पूर्णांक i, पूर्णांक j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

अटल व्योम moveAxis(Plan& p, पूर्णांक k, पूर्णांक to)
{
    क्रम (पूर्णांक i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
अटल माप_प्रकार addressBits(स्थिर Plan& p)
{
    माप_प्रकार addrs = (माप_प्रकार)p.src | (माप_प्रकार)p.dst;
    क्रम (पूर्णांक i = 0; i < p.dims; i++)
        addrs |= (माप_प्रकार)p.sstep[i] | (माप_प्रकार)p.dstep[i];
    वापस addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
अटल bool trySplitMerge(Plan& p)
{
    स्थिर सूचक_भेद_प्रकार esz = (सूचक_भेद_प्रकार)p.esz;
    स्थिर पूर्णांक L = p.dims - 1;
    अगर (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        वापस false;
    अगर ((addressBits(p) & (esz - 1)) != 0)
        वापस false;

    // split: axis k holds cn planes interleaved in the source
    क्रम (पूर्णांक k = L - 1; k >= 0; k--)
    {
        स्थिर int64 cn = p.size[k];
        अगर (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            वापस true;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    स्थिर int64 cn = p.size[L];
    अगर (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        क्रम (पूर्णांक k = L - 1; k >= 0; k--)
        {
            अगर (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                वापस true;
            }
        }
    }
    वापस false;
}

// Returns false when there is nothing to copy.
अटल bool buildPlan(स्थिर View& s, स्थिर View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    स्थिर सूचक_भेद_प्रकार esz = (सूचक_भेद_प्रकार)s.esz;
    p.src = s.तथ्य;
    p.dst = d.तथ्य;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    पूर्णांक n = 0;
    द्विगुन योग = 1;
    क्रम (पूर्णांक i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        पूर्णांक sz = s.size[i];
        CV_Assert(sz >= 0);
        अगर (sz == 0)
            वापस false;
        योग *= sz;
        अगर (sz == 1)
            जारी;
        सूचक_भेद_प्रकार ss = s.step[i], ds = d.step[i];
        अगर (ds < 0)
        {
            p.src += (सूचक_भेद_प्रकार)(sz - 1)*ss;
            p.dst += (सूचक_भेद_प्रकार)(sz - 1)*ds;
            ss = -ss;
            ds = -ds;
        }
        p.size[n] = sz;
        p.sstep[n] = ss;
        p.dstep[n] = ds;
        n++;
    }

    // Write the destination as sequentially as possible.
    क्रम (पूर्णांक i = 1; i < n; i++)
        क्रम (पूर्णांक j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    पूर्णांक m = 0;
    क्रम (पूर्णांक i = 0; i < n; i++)
    {
        अगर (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        अन्यथा
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    अगर (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    स्थिर पूर्णांक L = m - 1;
    p.tfunc = nullptr;
    अगर (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    अन्यथा अगर (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    अन्यथा अगर (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        अगर (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            पूर्णांक k = L - 1;
            जबतक (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            स्थिर माप_प्रकार align = std::min(s.esz & (0 - s.esz), (माप_प्रकार)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            अगर (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    स्थिर द्विगुन bytes = योग*(द्विगुन)esz;
    अगर (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        स्थिर पूर्णांक X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        क्रम (पूर्णांक i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = false;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    अन्यथा अगर (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        क्रम (पूर्णांक i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = false;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    अन्यथा
    {
        int64 nrows = 1;
        क्रम (पूर्णांक i = 0; i < L; i++)
            nrows *= p.size[i];
        स्थिर int64 rowLen = p.size[L];
        स्थिर द्विगुन rowBytes = (द्विगुन)rowLen*esz;
        स्थिर सूचक_भेद_प्रकार sstepL = std::असल(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = false;
        अगर (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::असल(p.sstep[L-1]) < sstepL &&
            (द्विगुन)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = true;
        }
        अन्यथा अगर (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    वापस true;
}

ढाँचा<typename T> अटल अंतरभूत bool isAligned(स्थिर व्योम* s, स्थिर व्योम* d, सूचक_भेद_प्रकार ss, सूचक_भेद_प्रकार ds)
{
    वापस (((माप_प्रकार)s | (माप_प्रकार)d | (माप_प्रकार)ss | (माप_प्रकार)ds) & (alignof(T) - 1)) == 0;
}

ढाँचा<typename T> अटल व्योम copyStrided_(स्थिर uchar* s, सूचक_भेद_प्रकार ss, uchar* d, सूचक_भेद_प्रकार ds, int64 n)
{
    int64 i = 0;
    क्रम (; i + 4 <= n; i += 4)
    {
        T t0 = *(स्थिर T*)s, t1 = *(स्थिर T*)(s + ss);
        T t2 = *(स्थिर T*)(s + ss*2), t3 = *(स्थिर T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    क्रम (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(स्थिर T*)s;
}

ढाँचा<typename T> अटल व्योम fill_(स्थिर uchar* s, uchar* d, सूचक_भेद_प्रकार ds, int64 n)
{
    स्थिर T v = *(स्थिर T*)s;
    क्रम (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

काष्ठा Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
अटल व्योम copyRow(स्थिर Plan& p, स्थिर uchar* s, uchar* d, int64 n)
{
    स्थिर माप_प्रकार esz = p.esz;
    स्थिर पूर्णांक L = p.dims - 1;
    स्थिर सूचक_भेद_प्रकार ss = p.sstep[L], ds = p.dstep[L];

    अगर (p.kind == KIND_FILL)
    {
        अगर (esz == 1 && ds == 1)
            स_रखो(d, *s, (माप_प्रकार)n);
        अन्यथा अगर (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        अन्यथा अगर (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        अन्यथा अगर (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        अन्यथा
            क्रम (int64 i = 0; i < n; i++, d += ds)
                स_नकल(d, s, esz);
        वापस;
    }

    अगर (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    अन्यथा अगर (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    अन्यथा अगर (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    अन्यथा अगर (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    अन्यथा अगर (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    अन्यथा
        क्रम (int64 i = 0; i < n; i++, s += ss, d += ds)
            स_नकल(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
अटल व्योम decode(स्थिर Plan& p, पूर्णांक naxes, int64 idx, int64* counters,
                   स्थिर uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    क्रम (पूर्णांक k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        अगर (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

ढाँचा<bool MEMCPY> अटल अंतरभूत व्योम copyRowT(स्थिर Plan& p, स्थिर uchar* s, uchar* d, int64 n)
{
    अगर (MEMCPY)
        स_नकल(d, s, (माप_प्रकार)n*p.esz);
    अन्यथा
        copyRow(p, s, d, n);
}

ढाँचा<bool MEMCPY> अटल व्योम runRows(स्थिर Plan& p, int64 i0, int64 i1)
{
    स्थिर पूर्णांक L = p.dims - 1;
    स्थिर int64 nch = p.nchunks, rowLen = p.size[L];
    स्थिर सूचक_भेद_प्रकार sstepL = p.sstep[L], dstepL = p.dstep[L];
    स्थिर uchar* s;
    uchar* d;

    अगर (p.chunkMajor)
    {
        क्रम (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        वापस;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    अगर (nch == 1 && L == 1)
    {
        स्थिर सूचक_भेद_प्रकार ss = p.sstep[0], ds = p.dstep[0];
        क्रम (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        वापस;
    }

    क्रम (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        अगर (chEnd < nch)
            अवरोध;
        ch = 0;
        क्रम (पूर्णांक k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            अगर (++counters[k] < p.size[k])
                अवरोध;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

अटल व्योम runTranspose(स्थिर Plan& p, int64 i0, int64 i1)
{
    स्थिर पूर्णांक A = p.dims - 2, B = p.dims - 1;
    स्थिर int64 ntiles = p.tilesA*p.tilesB;
    क्रम (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        स्थिर uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        पूर्णांक wa = (पूर्णांक)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        पूर्णांक hb = (पूर्णांक)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        अगर (cv_hal_transpose2d(s, (माप_प्रकार)p.sstep[B], d, (माप_प्रकार)p.dstep[A], wa, hb, (पूर्णांक)p.esz) == CV_HAL_ERROR_OK)
            जारी;
        p.tfunc(s, (माप_प्रकार)p.sstep[B], d, (माप_प्रकार)p.dstep[A], आकार(wa, hb));
    }
}

अटल व्योम runSplitMerge(स्थिर Plan& p, int64 i0, int64 i1)
{
    स्थिर पूर्णांक L = p.dims - 1;
    स्थिर bool isSplit = p.kind == KIND_SPLIT;
    स्थिर पूर्णांक C = isSplit ? L - 1 : L;    // the plane axis
    स्थिर पूर्णांक X = isSplit ? L : L - 1;    // the long axis
    स्थिर पूर्णांक cn = (पूर्णांक)p.size[C];
    स्थिर माप_प्रकार esz = p.esz;
    क्रम (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        स्थिर uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        स्थिर int64 e0 = ch*p.chunk;
        स्थिर पूर्णांक len = (पूर्णांक)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        अगर (isSplit)
        {
            uchar* dptr[4];
            क्रम (पूर्णांक c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            अगर (esz == 1) hal::split8u(s, dptr, len, cn);
            अन्यथा अगर (esz == 2) hal::split16u((स्थिर ushort*)s, (ushort**)dptr, len, cn);
            अन्यथा अगर (esz == 4) hal::split32s((स्थिर पूर्णांक*)s, (पूर्णांक**)dptr, len, cn);
            अन्यथा hal::split64s((स्थिर int64*)s, (int64**)dptr, len, cn);
        }
        अन्यथा
        {
            स्थिर uchar* sptr[4];
            क्रम (पूर्णांक c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            अगर (esz == 1) hal::merge8u(sptr, d, len, cn);
            अन्यथा अगर (esz == 2) hal::merge16u((स्थिर ushort**)sptr, (ushort*)d, len, cn);
            अन्यथा अगर (esz == 4) hal::merge32s((स्थिर पूर्णांक**)sptr, (पूर्णांक*)d, len, cn);
            अन्यथा hal::merge64s((स्थिर int64**)sptr, (int64*)d, len, cn);
        }
    }
}

अटल व्योम runPlan(स्थिर Plan& p, int64 i0, int64 i1)
{
    अगर (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    अन्यथा अगर (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    अन्यथा अगर (p.kind == KIND_MEMCPY)
        runRows<true>(p, i0, i1);
    अन्यथा
        runRows<false>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
अटल bool canRunInLockstep(स्थिर Plan* plans, माप_प्रकार np)
{
    अगर (np < 2)
        वापस false;
    स्थिर Plan& p0 = plans[0];
    क्रम (माप_प्रकार k = 0; k < np; k++)
    {
        स्थिर Plan& p = plans[k];
        अगर (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            वापस false;
        क्रम (पूर्णांक i = 0; i < p.dims - 1; i++)
            अगर (p.size[i] != p0.size[i])
                वापस false;
    }
    वापस true;
}

अटल व्योम runPlans(स्थिर Plan* plans, माप_प्रकार np)
{
    अगर (np == 0)
        वापस;
    Auto_बफर<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    द्विगुन cost = 0, rowBytes = 0;
    क्रम (माप_प्रकार k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (द्विगुन)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    स्थिर bool lockstep = canRunInLockstep(plans, np);
    स्थिर int64 योग = lockstep ? plans[0].nrows : ofs[np];
    स्थिर int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    स्वतः body = [&](int64 a, int64 b)
    {
        अगर (lockstep)
        {
            क्रम (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                क्रम (माप_प्रकार k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            वापस;
        }
        माप_प्रकार k = std::upper_bound(ofs.तथ्य(), ofs.तथ्य() + np + 1, a) - ofs.तथ्य() - 1;
        जबतक (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    अगर (cost < PARALLEL_MIN_COST || योग < 2 || get_अंक_Threads() <= 1)
    {
        body(0, योग);
        वापस;
    }

    // grouped so that the range fits into int
    स्थिर int64 ngroups = std::min(योग, (int64)1 << 20);
    स्थिर द्विगुन nstripes = std::min((द्विगुन)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (पूर्णांक)ngroups), [&](स्थिर Range& r)
    {
        body(r.start*योग/ngroups, r.end*योग/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
अटल व्योम viewRange(स्थिर View& v, स्थिर uchar*& lo, स्थिर uchar*& hi)
{
    सूचक_भेद_प्रकार a = 0, b = 0;
    क्रम (पूर्णांक i = 0; i < v.dims; i++)
    {
        सूचक_भेद_प्रकार ext = (सूचक_भेद_प्रकार)(v.size[i] - 1)*v.step[i];
        अगर (ext < 0) a += ext; अन्यथा b += ext;
    }
    lo = v.तथ्य + a;
    hi = v.तथ्य + b + v.esz;
}

अटल bool has_Zero_आकार(स्थिर View& v)
{
    क्रम (पूर्णांक i = 0; i < v.dims; i++)
        अगर (v.size[i] == 0)
            वापस true;
    वापस false;
}

अटल bool sameMapping(स्थिर View& s, स्थिर View& d)
{
    अगर (s.तथ्य != d.तथ्य)
        वापस false;
    क्रम (पूर्णांक i = 0; i < s.dims; i++)
        अगर (s.size[i] > 1 && s.step[i] != d.step[i])
            वापस false;
    वापस true;
}

अटल bool overlap(स्थिर View& s, स्थिर View& d)
{
    स्थिर uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    वापस slo < dhi && dlo < shi;
}

} // namespace

व्योम copyBatch(स्थिर View* src, स्थिर View* dst, पूर्णांक n)
{
    CV_Assert(n >= 0);
    Auto_बफर<Plan, 4> plans(n);
    माप_प्रकार np = 0;
    std::vector<Auto_बफर<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    क्रम (पूर्णांक i = 0; i < n; i++)
    {
        View s = src[i];
        स्थिर View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (पूर्णांक)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        अगर (has_Zero_आकार(s) || sameMapping(s, d))
            जारी;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        bool need_अस्थायी = false;
        क्रम (पूर्णांक j = 0; j < n && !need_अस्थायी; j++)
            need_अस्थायी = !has_Zero_आकार(dst[j]) && overlap(s, dst[j]);
        अगर (need_अस्थायी)
        {
            माप_प्रकार योग = 1;
            क्रम (पूर्णांक k = 0; k < s.dims; k++)
                योग *= (माप_प्रकार)s.size[k];
            temps.emplace_back(योग*s.esz);
            View t = s;
            t.तथ्य = temps.back().तथ्य();
            सूचक_भेद_प्रकार st = (सूचक_भेद_प्रकार)s.esz;
            क्रम (पूर्णांक k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        अगर (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.तथ्य(), np);
}

व्योम copy(स्थिर View& src, स्थिर View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
