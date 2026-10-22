// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "nd_copy.hpp"
#अन्तर्भाव "hal_replacement.hpp"

#अन्तर्भाव "opencv2/core/hal/hal.hpp"

#अन्तर्भाव <algorithm>

नामाकाश cv { नामाकाश nd {

View viewOf(अचर Mat& m)
{
    View v;
    v.data = m.data;
    v.dims = m.dims;
    v.esz = m.elemSize();
    लागि (पूर्णाङ्क i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.step[i] = (सूचक_भेद_प्रकार)m.step[i];
    }
    फर्काऊ v;
}

View permute(अचर View& v, अचर पूर्णाङ्क* order)
{
    View r;
    r.data = v.data;
    r.dims = v.dims;
    r.esz = v.esz;
    लागि (पूर्णाङ्क i = 0; i < v.dims; i++)
    {
        पूर्णाङ्क j = order[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.step[i] = v.step[j];
    }
    फर्काऊ r;
}

शून्य flip(View& v, पूर्णाङ्क axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    यदि (v.size[axis] > 0)
        v.data += (सूचक_भेद_प्रकार)(v.size[axis] - 1)*v.step[axis];
    v.step[axis] = -v.step[axis];
}

शून्य slice(View& v, पूर्णाङ्क axis, पूर्णाङ्क start, पूर्णाङ्क step, पूर्णाङ्क गन्ती_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && गन्ती_2 >= 0);
    यदि (गन्ती_2 > 0)
        v.data += (सूचक_भेद_प्रकार)start*v.step[axis];
    v.step[axis] *= step;
    v.size[axis] = गन्ती_2;
}

नामाकाश {

गणना Kind
{
    KIND_MEMCPY,    // the innermost axis is contiguous in both views
    KIND_FILL,      // the innermost axis is broadcast (step 0) in the source
    KIND_STRIDED,
    KIND_TRANSPOSE, // the two innermost axes are swapped between source and destination
    KIND_SPLIT,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    KIND_MERGE      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

गणना
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

संरचना Plan
{
    अचर uchar* src;
    uchar* dst;
    माप_प्रकार esz;
    पूर्णाङ्क dims;
    पूर्णाङ्क kind;
    int64 size[MAX_VIEW_DIMS];
    सूचक_भेद_प्रकार sstep[MAX_VIEW_DIMS];
    सूचक_भेद_प्रकार dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    तार्किक chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    द्विगुण cost;
};

स्थिर शून्य swapAxes(Plan& p, पूर्णाङ्क i, पूर्णाङ्क j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

स्थिर शून्य moveAxis(Plan& p, पूर्णाङ्क k, पूर्णाङ्क to)
{
    लागि (पूर्णाङ्क i = k; i < to; i++)
        swapAxes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
स्थिर माप_प्रकार addressBits(अचर Plan& p)
{
    माप_प्रकार addrs = (माप_प्रकार)p.src | (माप_प्रकार)p.dst;
    लागि (पूर्णाङ्क i = 0; i < p.dims; i++)
        addrs |= (माप_प्रकार)p.sstep[i] | (माप_प्रकार)p.dstep[i];
    फर्काऊ addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
स्थिर तार्किक trySplitMerge(Plan& p)
{
    अचर सूचक_भेद_प्रकार esz = (सूचक_भेद_प्रकार)p.esz;
    अचर पूर्णाङ्क L = p.dims - 1;
    यदि (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        फर्काऊ असत्य;
    यदि ((addressBits(p) & (esz - 1)) != 0)
        फर्काऊ असत्य;

    // split: axis k holds cn planes interleaved in the source
    लागि (पूर्णाङ्क k = L - 1; k >= 0; k--)
    {
        अचर int64 cn = p.size[k];
        यदि (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.kind = KIND_SPLIT;
            फर्काऊ सत्य;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    अचर int64 cn = p.size[L];
    यदि (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        लागि (पूर्णाङ्क k = L - 1; k >= 0; k--)
        {
            यदि (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_MERGE;
                फर्काऊ सत्य;
            }
        }
    }
    फर्काऊ असत्य;
}

// Returns false when there is nothing to copy.
स्थिर तार्किक buildPlan(अचर View& s, अचर View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    अचर सूचक_भेद_प्रकार esz = (सूचक_भेद_प्रकार)s.esz;
    p.src = s.data;
    p.dst = d.data;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    पूर्णाङ्क n = 0;
    द्विगुण total = 1;
    लागि (पूर्णाङ्क i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        पूर्णाङ्क sz = s.size[i];
        CV_Assert(sz >= 0);
        यदि (sz == 0)
            फर्काऊ असत्य;
        total *= sz;
        यदि (sz == 1)
            जारी;
        सूचक_भेद_प्रकार ss = s.step[i], ds = d.step[i];
        यदि (ds < 0)
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
    लागि (पूर्णाङ्क i = 1; i < n; i++)
        लागि (पूर्णाङ्क j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            swapAxes(p, j-1, j);

    पूर्णाङ्क m = 0;
    लागि (पूर्णाङ्क i = 0; i < n; i++)
    {
        यदि (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
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
    यदि (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    अचर पूर्णाङ्क L = m - 1;
    p.tfunc = nullptr;
    यदि (p.sstep[L] == esz && p.dstep[L] == esz)
        p.kind = KIND_MEMCPY;
    अन्यथा यदि (p.sstep[L] == 0)
        p.kind = KIND_FILL;
    अन्यथा यदि (!trySplitMerge(p))
    {
        p.kind = KIND_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        यदि (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            पूर्णाङ्क k = L - 1;
            जबसम्म (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            अचर माप_प्रकार align = std::min(s.esz & (0 - s.esz), (माप_प्रकार)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            यदि (k >= 0 && f && p.size[k] >= 8 && (addressBits(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.kind = KIND_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    अचर द्विगुण bytes = total*(द्विगुण)esz;
    यदि (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
    {
        अचर पूर्णाङ्क X = p.kind == KIND_SPLIT ? L : L - 1;
        int64 nouter = 1;
        लागि (पूर्णाङ्क i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = SPLIT_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = असत्य;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.cost = bytes*2;
    }
    अन्यथा यदि (p.kind == KIND_TRANSPOSE)
    {
        int64 nouter = 1;
        लागि (पूर्णाङ्क i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = असत्य;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.cost = bytes*3;
    }
    अन्यथा
    {
        int64 nrows = 1;
        लागि (पूर्णाङ्क i = 0; i < L; i++)
            nrows *= p.size[i];
        अचर int64 rowLen = p.size[L];
        अचर द्विगुण rowBytes = (द्विगुण)rowLen*esz;
        अचर सूचक_भेद_प्रकार sstepL = std::असल(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = rowLen;
        p.chunkMajor = असत्य;
        यदि (p.kind == KIND_STRIDED && L > 0 && nrows <= 64 && std::असल(p.sstep[L-1]) < sstepL &&
            (द्विगुण)rowLen*sstepL > 2.0*GATHER_BLOCK_BYTES)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_BLOCK_BYTES/sstepL));
            p.nchunks = (rowLen + p.chunk - 1)/p.chunk;
            p.chunkMajor = सत्य;
        }
        अन्यथा यदि (nrows < 64 && rowBytes >= 2.0*ROW_CHUNK_BYTES)
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
    फर्काऊ सत्य;
}

ढाँचा<typename T> स्थिर अन्तःस्थ तार्किक isAligned(अचर शून्य* s, अचर शून्य* d, सूचक_भेद_प्रकार ss, सूचक_भेद_प्रकार ds)
{
    फर्काऊ (((माप_प्रकार)s | (माप_प्रकार)d | (माप_प्रकार)ss | (माप_प्रकार)ds) & (alignof(T) - 1)) == 0;
}

ढाँचा<typename T> स्थिर शून्य copyStrided_(अचर uchar* s, सूचक_भेद_प्रकार ss, uchar* d, सूचक_भेद_प्रकार ds, int64 n)
{
    int64 i = 0;
    लागि (; i + 4 <= n; i += 4)
    {
        T t0 = *(अचर T*)s, t1 = *(अचर T*)(s + ss);
        T t2 = *(अचर T*)(s + ss*2), t3 = *(अचर T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    लागि (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(अचर T*)s;
}

ढाँचा<typename T> स्थिर शून्य fill_(अचर uchar* s, uchar* d, सूचक_भेद_प्रकार ds, int64 n)
{
    अचर T v = *(अचर T*)s;
    लागि (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

संरचना Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
स्थिर शून्य copyRow(अचर Plan& p, अचर uchar* s, uchar* d, int64 n)
{
    अचर माप_प्रकार esz = p.esz;
    अचर पूर्णाङ्क L = p.dims - 1;
    अचर सूचक_भेद_प्रकार ss = p.sstep[L], ds = p.dstep[L];

    यदि (p.kind == KIND_FILL)
    {
        यदि (esz == 1 && ds == 1)
            स_रखो(d, *s, (माप_प्रकार)n);
        अन्यथा यदि (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        अन्यथा यदि (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        अन्यथा यदि (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        अन्यथा
            लागि (int64 i = 0; i < n; i++, d += ds)
                स_नकल(d, s, esz);
        फर्काऊ;
    }

    यदि (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    अन्यथा यदि (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    अन्यथा यदि (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    अन्यथा यदि (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    अन्यथा यदि (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    अन्यथा
        लागि (int64 i = 0; i < n; i++, s += ss, d += ds)
            स_नकल(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
स्थिर शून्य decode(अचर Plan& p, पूर्णाङ्क naxes, int64 idx, int64* counters,
                   अचर uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    लागि (पूर्णाङ्क k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        यदि (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

ढाँचा<तार्किक MEMCPY> स्थिर अन्तःस्थ शून्य copyRowT(अचर Plan& p, अचर uchar* s, uchar* d, int64 n)
{
    यदि (MEMCPY)
        स_नकल(d, s, (माप_प्रकार)n*p.esz);
    अन्यथा
        copyRow(p, s, d, n);
}

ढाँचा<तार्किक MEMCPY> स्थिर शून्य runRows(अचर Plan& p, int64 i0, int64 i1)
{
    अचर पूर्णाङ्क L = p.dims - 1;
    अचर int64 nch = p.nchunks, rowLen = p.size[L];
    अचर सूचक_भेद_प्रकार sstepL = p.sstep[L], dstepL = p.dstep[L];
    अचर uchar* s;
    uchar* d;

    यदि (p.chunkMajor)
    {
        लागि (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, row = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, rowLen);
            decode(p, L, row, nullptr, s, d);
            copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        फर्काऊ;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    यदि (nch == 1 && L == 1)
    {
        अचर सूचक_भेद_प्रकार ss = p.sstep[0], ds = p.dstep[0];
        लागि (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copyRowT<MEMCPY>(p, s, d, rowLen);
        फर्काऊ;
    }

    लागि (int64 it = i0; it < i1; )
    {
        int64 chEnd = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(chEnd*p.chunk, rowLen);
        copyRowT<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += chEnd - ch;
        यदि (chEnd < nch)
            रोक;
        ch = 0;
        लागि (पूर्णाङ्क k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            यदि (++counters[k] < p.size[k])
                रोक;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

स्थिर शून्य runTranspose(अचर Plan& p, int64 i0, int64 i1)
{
    अचर पूर्णाङ्क A = p.dims - 2, B = p.dims - 1;
    अचर int64 ntiles = p.tilesA*p.tilesB;
    लागि (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        अचर uchar* s;
        uchar* d;
        decode(p, A, o, nullptr, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        पूर्णाङ्क wa = (पूर्णाङ्क)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        पूर्णाङ्क hb = (पूर्णाङ्क)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        यदि (cv_hal_transpose2d(s, (माप_प्रकार)p.sstep[B], d, (माप_प्रकार)p.dstep[A], wa, hb, (पूर्णाङ्क)p.esz) == CV_HAL_ERROR_OK)
            जारी;
        p.tfunc(s, (माप_प्रकार)p.sstep[B], d, (माप_प्रकार)p.dstep[A], Size(wa, hb));
    }
}

स्थिर शून्य runSplitMerge(अचर Plan& p, int64 i0, int64 i1)
{
    अचर पूर्णाङ्क L = p.dims - 1;
    अचर तार्किक isSplit = p.kind == KIND_SPLIT;
    अचर पूर्णाङ्क C = isSplit ? L - 1 : L;    // the plane axis
    अचर पूर्णाङ्क X = isSplit ? L : L - 1;    // the long axis
    अचर पूर्णाङ्क cn = (पूर्णाङ्क)p.size[C];
    अचर माप_प्रकार esz = p.esz;
    लागि (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        अचर uchar* s;
        uchar* d;
        decode(p, L - 1, o, nullptr, s, d);
        अचर int64 e0 = ch*p.chunk;
        अचर पूर्णाङ्क len = (पूर्णाङ्क)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        यदि (isSplit)
        {
            uchar* dptr[4];
            लागि (पूर्णाङ्क c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            यदि (esz == 1) hal::split8u(s, dptr, len, cn);
            अन्यथा यदि (esz == 2) hal::split16u((अचर ushort*)s, (ushort**)dptr, len, cn);
            अन्यथा यदि (esz == 4) hal::split32s((अचर पूर्णाङ्क*)s, (पूर्णाङ्क**)dptr, len, cn);
            अन्यथा hal::split64s((अचर int64*)s, (int64**)dptr, len, cn);
        }
        अन्यथा
        {
            अचर uchar* sptr[4];
            लागि (पूर्णाङ्क c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            यदि (esz == 1) hal::merge8u(sptr, d, len, cn);
            अन्यथा यदि (esz == 2) hal::merge16u((अचर ushort**)sptr, (ushort*)d, len, cn);
            अन्यथा यदि (esz == 4) hal::merge32s((अचर पूर्णाङ्क**)sptr, (पूर्णाङ्क*)d, len, cn);
            अन्यथा hal::merge64s((अचर int64**)sptr, (int64*)d, len, cn);
        }
    }
}

स्थिर शून्य runPlan(अचर Plan& p, int64 i0, int64 i1)
{
    यदि (p.kind == KIND_SPLIT || p.kind == KIND_MERGE)
        runSplitMerge(p, i0, i1);
    अन्यथा यदि (p.kind == KIND_TRANSPOSE)
        runTranspose(p, i0, i1);
    अन्यथा यदि (p.kind == KIND_MEMCPY)
        runRows<सत्य>(p, i0, i1);
    अन्यथा
        runRows<असत्य>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
स्थिर तार्किक canRunInLockstep(अचर Plan* plans, माप_प्रकार np)
{
    यदि (np < 2)
        फर्काऊ असत्य;
    अचर Plan& p0 = plans[0];
    लागि (माप_प्रकार k = 0; k < np; k++)
    {
        अचर Plan& p = plans[k];
        यदि (p.kind == KIND_TRANSPOSE || p.kind == KIND_SPLIT || p.kind == KIND_MERGE ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            फर्काऊ असत्य;
        लागि (पूर्णाङ्क i = 0; i < p.dims - 1; i++)
            यदि (p.size[i] != p0.size[i])
                फर्काऊ असत्य;
    }
    फर्काऊ सत्य;
}

स्थिर शून्य runPlans(अचर Plan* plans, माप_प्रकार np)
{
    यदि (np == 0)
        फर्काऊ;
    AutoBuffer<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    द्विगुण cost = 0, rowBytes = 0;
    लागि (माप_प्रकार k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        cost += plans[k].cost;
        rowBytes += (द्विगुण)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    अचर तार्किक lockstep = canRunInLockstep(plans, np);
    अचर int64 total = lockstep ? plans[0].nrows : ofs[np];
    अचर int64 blockRows = std::max((int64)1, (int64)(16*1024/std::max(rowBytes, 1.)));

    स्वतः body = [&](int64 a, int64 b)
    {
        यदि (lockstep)
        {
            लागि (int64 r = a; r < b; r += blockRows)
            {
                int64 re = std::min(b, r + blockRows);
                लागि (माप_प्रकार k = 0; k < np; k++)
                    runPlan(plans[k], r, re);
            }
            फर्काऊ;
        }
        माप_प्रकार k = std::upper_bound(ofs.data(), ofs.data() + np + 1, a) - ofs.data() - 1;
        जबसम्म (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            runPlan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    यदि (cost < PARALLEL_MIN_COST || total < 2 || get_अङ्क_Threads() <= 1)
    {
        body(0, total);
        फर्काऊ;
    }

    // grouped so that the range fits into int
    अचर int64 ngroups = std::min(total, (int64)1 << 20);
    अचर द्विगुण nstripes = std::min((द्विगुण)ngroups, std::max(2.0, cost/STRIPE_COST));
    parallel_for_(Range(0, (पूर्णाङ्क)ngroups), [&](अचर Range& r)
    {
        body(r.start*total/ngroups, r.end*total/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
स्थिर शून्य viewRange(अचर View& v, अचर uchar*& lo, अचर uchar*& hi)
{
    सूचक_भेद_प्रकार a = 0, b = 0;
    लागि (पूर्णाङ्क i = 0; i < v.dims; i++)
    {
        सूचक_भेद_प्रकार ext = (सूचक_भेद_प्रकार)(v.size[i] - 1)*v.step[i];
        यदि (ext < 0) a += ext; अन्यथा b += ext;
    }
    lo = v.data + a;
    hi = v.data + b + v.esz;
}

स्थिर तार्किक hasZeroSize(अचर View& v)
{
    लागि (पूर्णाङ्क i = 0; i < v.dims; i++)
        यदि (v.size[i] == 0)
            फर्काऊ सत्य;
    फर्काऊ असत्य;
}

स्थिर तार्किक sameMapping(अचर View& s, अचर View& d)
{
    यदि (s.data != d.data)
        फर्काऊ असत्य;
    लागि (पूर्णाङ्क i = 0; i < s.dims; i++)
        यदि (s.size[i] > 1 && s.step[i] != d.step[i])
            फर्काऊ असत्य;
    फर्काऊ सत्य;
}

स्थिर तार्किक overlap(अचर View& s, अचर View& d)
{
    अचर uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    फर्काऊ slo < dhi && dlo < shi;
}

} // namespace

शून्य copyBatch(अचर View* src, अचर View* dst, पूर्णाङ्क n)
{
    CV_Assert(n >= 0);
    AutoBuffer<Plan, 4> plans(n);
    माप_प्रकार np = 0;
    std::vector<AutoBuffer<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    लागि (पूर्णाङ्क i = 0; i < n; i++)
    {
        View s = src[i];
        अचर View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (पूर्णाङ्क)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        यदि (hasZeroSize(s) || sameMapping(s, d))
            जारी;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        तार्किक needTemp = असत्य;
        लागि (पूर्णाङ्क j = 0; j < n && !needTemp; j++)
            needTemp = !hasZeroSize(dst[j]) && overlap(s, dst[j]);
        यदि (needTemp)
        {
            माप_प्रकार total = 1;
            लागि (पूर्णाङ्क k = 0; k < s.dims; k++)
                total *= (माप_प्रकार)s.size[k];
            temps.emplace_back(total*s.esz);
            View t = s;
            t.data = temps.back().data();
            सूचक_भेद_प्रकार st = (सूचक_भेद_प्रकार)s.esz;
            लागि (पूर्णाङ्क k = s.dims - 1; k >= 0; k--)
            {
                t.step[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        यदि (buildPlan(s, d, plans[np]))
            np++;
    }
    runPlans(plans.data(), np);
}

शून्य copy(अचर View& src, अचर View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
