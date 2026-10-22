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

View view_جو(ثابت Mat& m)
{
    View v;
    v.معلومات = m.معلومات;
    v.dims = m.dims;
    v.esz = m.elem_سائيز();
    لكل (صحيح i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.قدم[i] = (ptrdiff_t)m.قدم[i];
    }
    أرجع v;
}

View ترتيب_بدلايو(ثابت View& v, ثابت صحيح* ترتيب)
{
    View r;
    r.معلومات = v.معلومات;
    r.dims = v.dims;
    r.esz = v.esz;
    لكل (صحيح i = 0; i < v.dims; i++)
    {
        صحيح j = ترتيب[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.قدم[i] = v.قدم[j];
    }
    أرجع r;
}

فراغ flip(View& v, صحيح axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    إذا (v.size[axis] > 0)
        v.معلومات += (ptrdiff_t)(v.size[axis] - 1)*v.قدم[axis];
    v.قدم[axis] = -v.قدم[axis];
}

فراغ ٽڪرو_2(View& v, صحيح axis, صحيح شروعات, صحيح قدم, صحيح ڳڻپ_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && ڳڻپ_2 >= 0);
    إذا (ڳڻپ_2 > 0)
        v.معلومات += (ptrdiff_t)شروعات*v.قدم[axis];
    v.قدم[axis] *= قدم;
    v.size[axis] = ڳڻپ_2;
}

نطاق {

تعداد قسم_3
{
    قسم_MEMCPY,    // the innermost axis is contiguous in both views
    قسم_FILL,      // the innermost axis is broadcast (step 0) in the source
    قسم_STRIDED,
    قسم_TRANSPOSE, // the two innermost axes are swapped between source and destination
    قسم_ورهايو,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    قسم_ملايو      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

تعداد
{
    TRANSPOSE_TILE = 64,                // in elements
    ورهايو_CHUNK = 1024,                 // in elements
    صف_CHUNK_بائيٽون = 64*1024,
    GATHER_بلاڪ_بائيٽون = 8*1024,
    // costs are bytes weighted by the kernel; the copies are memory-bound,
    // so small stripes only add overhead
    PARALLEL_MIN_لاڳت = 1024*1024,
    STRIPE_لاڳت = 1024*1024
};

بنية Plan
{
    ثابت uchar* src;
    uchar* dst;
    size_t esz;
    صحيح dims;
    صحيح قسم;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    منطقي chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    مزدوج لاڳت_2;
};

ساكن فراغ مٽا_سٽا_Axes(Plan& p, صحيح i, صحيح j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

ساكن فراغ moveAxis(Plan& p, صحيح k, صحيح منزل)
{
    لكل (صحيح i = k; i < منزل; i++)
        مٽا_سٽا_Axes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
ساكن size_t پتو_بٽ(ثابت Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    لكل (صحيح i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    أرجع addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
ساكن منطقي try_ورهايو_ملايو(Plan& p)
{
    ثابت ptrdiff_t esz = (ptrdiff_t)p.esz;
    ثابت صحيح L = p.dims - 1;
    إذا (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        أرجع خطأ;
    إذا ((پتو_بٽ(p) & (esz - 1)) != 0)
        أرجع خطأ;

    // split: axis k holds cn planes interleaved in the source
    لكل (صحيح k = L - 1; k >= 0; k--)
    {
        ثابت int64 cn = p.size[k];
        إذا (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.قسم = قسم_ورهايو;
            أرجع سچ;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ثابت int64 cn = p.size[L];
    إذا (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        لكل (صحيح k = L - 1; k >= 0; k--)
        {
            إذا (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.قسم = قسم_ملايو;
                أرجع سچ;
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
    p.src = s.معلومات;
    p.dst = d.معلومات;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    صحيح n = 0;
    مزدوج ڪل = 1;
    لكل (صحيح i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        صحيح sz = s.size[i];
        CV_Assert(sz >= 0);
        إذا (sz == 0)
            أرجع خطأ;
        ڪل *= sz;
        إذا (sz == 1)
            استمر;
        ptrdiff_t ss = s.قدم[i], ds = d.قدم[i];
        إذا (ds < 0)
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
            مٽا_سٽا_Axes(p, j-1, j);

    صحيح m = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        إذا (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
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
    إذا (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    ثابت صحيح L = m - 1;
    p.tfunc = خالي_اشارو;
    إذا (p.sstep[L] == esz && p.dstep[L] == esz)
        p.قسم = قسم_MEMCPY;
    وإلا إذا (p.sstep[L] == 0)
        p.قسم = قسم_FILL;
    وإلا إذا (!try_ورهايو_ملايو(p))
    {
        p.قسم = قسم_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        إذا (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            صحيح k = L - 1;
            طالما (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ثابت size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            إذا (k >= 0 && f && p.size[k] >= 8 && (پتو_بٽ(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.قسم = قسم_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ثابت مزدوج بائيٽون = ڪل*(مزدوج)esz;
    إذا (p.قسم == قسم_ورهايو || p.قسم == قسم_ملايو)
    {
        ثابت صحيح X = p.قسم == قسم_ورهايو ? L : L - 1;
        int64 nouter = 1;
        لكل (صحيح i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = ورهايو_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = خطأ;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.لاڳت_2 = بائيٽون*2;
    }
    وإلا إذا (p.قسم == قسم_TRANSPOSE)
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
        p.لاڳت_2 = بائيٽون*3;
    }
    وإلا
    {
        int64 nrows = 1;
        لكل (صحيح i = 0; i < L; i++)
            nrows *= p.size[i];
        ثابت int64 صف_Len = p.size[L];
        ثابت مزدوج صف_بائيٽون = (مزدوج)صف_Len*esz;
        ثابت ptrdiff_t sstepL = std::مطلق_قدر(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = صف_Len;
        p.chunkMajor = خطأ;
        إذا (p.قسم == قسم_STRIDED && L > 0 && nrows <= 64 && std::مطلق_قدر(p.sstep[L-1]) < sstepL &&
            (مزدوج)صف_Len*sstepL > 2.0*GATHER_بلاڪ_بائيٽون)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_بلاڪ_بائيٽون/sstepL));
            p.nchunks = (صف_Len + p.chunk - 1)/p.chunk;
            p.chunkMajor = سچ;
        }
        وإلا إذا (nrows < 64 && صف_بائيٽون >= 2.0*صف_CHUNK_بائيٽون)
        {
            // few long rows: split them to have something to parallelize
            int64 nch = std::min((int64)(صف_بائيٽون/صف_CHUNK_بائيٽون), (int64)1024);
            p.chunk = (صف_Len + nch - 1)/nch;
            p.nchunks = (صف_Len + p.chunk - 1)/p.chunk;
        }
        p.tilesA = p.tilesB = 1;
        p.nitems = nrows*p.nchunks;
        p.لاڳت_2 = p.قسم == قسم_MEMCPY ? بائيٽون : بائيٽون*4;
    }
    أرجع سچ;
}

سانچو<قسم_نالو T> ساكن مضمن منطقي isAligned(ثابت فراغ* s, ثابت فراغ* d, ptrdiff_t ss, ptrdiff_t ds)
{
    أرجع (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

سانچو<قسم_نالو T> ساكن فراغ copyStrided_(ثابت uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
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

سانچو<قسم_نالو T> ساكن فراغ fill_(ثابت uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ثابت T v = *(ثابت T*)s;
    لكل (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

بنية Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
ساكن فراغ copy_صف(ثابت Plan& p, ثابت uchar* s, uchar* d, int64 n)
{
    ثابت size_t esz = p.esz;
    ثابت صحيح L = p.dims - 1;
    ثابت ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    إذا (p.قسم == قسم_FILL)
    {
        إذا (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        وإلا إذا (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        وإلا إذا (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        وإلا إذا (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        وإلا
            لكل (int64 i = 0; i < n; i++, d += ds)
                يادگيري_نقل(d, s, esz);
        أرجع;
    }

    إذا (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    وإلا إذا (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    وإلا إذا (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    وإلا إذا (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    وإلا إذا (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    وإلا
        لكل (int64 i = 0; i < n; i++, s += ss, d += ds)
            يادگيري_نقل(d, s, esz);
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
        إذا (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

سانچو<منطقي MEMCPY> ساكن مضمن فراغ copy_صف_T(ثابت Plan& p, ثابت uchar* s, uchar* d, int64 n)
{
    إذا (MEMCPY)
        يادگيري_نقل(d, s, (size_t)n*p.esz);
    وإلا
        copy_صف(p, s, d, n);
}

سانچو<منطقي MEMCPY> ساكن فراغ لاڳيتو_صفون(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحيح L = p.dims - 1;
    ثابت int64 nch = p.nchunks, صف_Len = p.size[L];
    ثابت ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ثابت uchar* s;
    uchar* d;

    إذا (p.chunkMajor)
    {
        لكل (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, صف = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, صف_Len);
            decode(p, L, صف, خالي_اشارو, s, d);
            copy_صف_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        أرجع;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    إذا (nch == 1 && L == 1)
    {
        ثابت ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        لكل (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copy_صف_T<MEMCPY>(p, s, d, صف_Len);
        أرجع;
    }

    لكل (int64 it = i0; it < i1; )
    {
        int64 ch_پڇاڙي = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(ch_پڇاڙي*p.chunk, صف_Len);
        copy_صف_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += ch_پڇاڙي - ch;
        إذا (ch_پڇاڙي < nch)
            اكسر;
        ch = 0;
        لكل (صحيح k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            إذا (++counters[k] < p.size[k])
                اكسر;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

ساكن فراغ لاڳيتو_Transpose(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحيح A = p.dims - 2, B = p.dims - 1;
    ثابت int64 ntiles = p.tilesA*p.tilesB;
    لكل (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ثابت uchar* s;
        uchar* d;
        decode(p, A, o, خالي_اشارو, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        صحيح wa = (صحيح)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        صحيح hb = (صحيح)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        إذا (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (صحيح)p.esz) == CV_HAL_ERROR_OK)
            استمر;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], سائيز(wa, hb));
    }
}

ساكن فراغ لاڳيتو_ورهايو_ملايو(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحيح L = p.dims - 1;
    ثابت منطقي is_ورهايو = p.قسم == قسم_ورهايو;
    ثابت صحيح C = is_ورهايو ? L - 1 : L;    // the plane axis
    ثابت صحيح X = is_ورهايو ? L : L - 1;    // the long axis
    ثابت صحيح cn = (صحيح)p.size[C];
    ثابت size_t esz = p.esz;
    لكل (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ثابت uchar* s;
        uchar* d;
        decode(p, L - 1, o, خالي_اشارو, s, d);
        ثابت int64 e0 = ch*p.chunk;
        ثابت صحيح len = (صحيح)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        إذا (is_ورهايو)
        {
            uchar* dptr[4];
            لكل (صحيح c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            إذا (esz == 1) hal::split8u(s, dptr, len, cn);
            وإلا إذا (esz == 2) hal::split16u((ثابت ushort*)s, (ushort**)dptr, len, cn);
            وإلا إذا (esz == 4) hal::split32s((ثابت صحيح*)s, (صحيح**)dptr, len, cn);
            وإلا hal::split64s((ثابت int64*)s, (int64**)dptr, len, cn);
        }
        وإلا
        {
            ثابت uchar* sptr[4];
            لكل (صحيح c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            إذا (esz == 1) hal::merge8u(sptr, d, len, cn);
            وإلا إذا (esz == 2) hal::merge16u((ثابت ushort**)sptr, (ushort*)d, len, cn);
            وإلا إذا (esz == 4) hal::merge32s((ثابت صحيح**)sptr, (صحيح*)d, len, cn);
            وإلا hal::merge64s((ثابت int64**)sptr, (int64*)d, len, cn);
        }
    }
}

ساكن فراغ لاڳيتو_Plan(ثابت Plan& p, int64 i0, int64 i1)
{
    إذا (p.قسم == قسم_ورهايو || p.قسم == قسم_ملايو)
        لاڳيتو_ورهايو_ملايو(p, i0, i1);
    وإلا إذا (p.قسم == قسم_TRANSPOSE)
        لاڳيتو_Transpose(p, i0, i1);
    وإلا إذا (p.قسم == قسم_MEMCPY)
        لاڳيتو_صفون<سچ>(p, i0, i1);
    وإلا
        لاڳيتو_صفون<خطأ>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
ساكن منطقي can_لاڳيتو_اندريون_Lockstep(ثابت Plan* plans, size_t np)
{
    إذا (np < 2)
        أرجع خطأ;
    ثابت Plan& p0 = plans[0];
    لكل (size_t k = 0; k < np; k++)
    {
        ثابت Plan& p = plans[k];
        إذا (p.قسم == قسم_TRANSPOSE || p.قسم == قسم_ورهايو || p.قسم == قسم_ملايو ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            أرجع خطأ;
        لكل (صحيح i = 0; i < p.dims - 1; i++)
            إذا (p.size[i] != p0.size[i])
                أرجع خطأ;
    }
    أرجع سچ;
}

ساكن فراغ لاڳيتو_Plans(ثابت Plan* plans, size_t np)
{
    إذا (np == 0)
        أرجع;
    Auto_بفر<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    مزدوج لاڳت_2 = 0, صف_بائيٽون = 0;
    لكل (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        لاڳت_2 += plans[k].لاڳت_2;
        صف_بائيٽون += (مزدوج)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ثابت منطقي lockstep = can_لاڳيتو_اندريون_Lockstep(plans, np);
    ثابت int64 ڪل = lockstep ? plans[0].nrows : ofs[np];
    ثابت int64 بلاڪ_صفون_2 = std::max((int64)1, (int64)(16*1024/std::max(صف_بائيٽون, 1.)));

    خودڪار body = [&](int64 a, int64 b)
    {
        إذا (lockstep)
        {
            لكل (int64 r = a; r < b; r += بلاڪ_صفون_2)
            {
                int64 re = std::min(b, r + بلاڪ_صفون_2);
                لكل (size_t k = 0; k < np; k++)
                    لاڳيتو_Plan(plans[k], r, re);
            }
            أرجع;
        }
        size_t k = std::upper_bound(ofs.معلومات(), ofs.معلومات() + np + 1, a) - ofs.معلومات() - 1;
        طالما (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            لاڳيتو_Plan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    إذا (لاڳت_2 < PARALLEL_MIN_لاڳت || ڪل < 2 || get_انگ_ڌاڳا() <= 1)
    {
        body(0, ڪل);
        أرجع;
    }

    // grouped so that the range fits into int
    ثابت int64 ngroups = std::min(ڪل, (int64)1 << 20);
    ثابت مزدوج nstripes = std::min((مزدوج)ngroups, std::max(2.0, لاڳت_2/STRIPE_لاڳت));
    parallel_for_(Range(0, (صحيح)ngroups), [&](ثابت Range& r)
    {
        body(r.شروعات*ڪل/ngroups, r.end*ڪل/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
ساكن فراغ viewRange(ثابت View& v, ثابت uchar*& lo, ثابت uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    لكل (صحيح i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.قدم[i];
        إذا (ext < 0) a += ext; وإلا b += ext;
    }
    lo = v.معلومات + a;
    hi = v.معلومات + b + v.esz;
}

ساكن منطقي has_Zero_سائيز(ثابت View& v)
{
    لكل (صحيح i = 0; i < v.dims; i++)
        إذا (v.size[i] == 0)
            أرجع سچ;
    أرجع خطأ;
}

ساكن منطقي sameMapping(ثابت View& s, ثابت View& d)
{
    إذا (s.معلومات != d.معلومات)
        أرجع خطأ;
    لكل (صحيح i = 0; i < s.dims; i++)
        إذا (s.size[i] > 1 && s.قدم[i] != d.قدم[i])
            أرجع خطأ;
    أرجع سچ;
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
    Auto_بفر<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<Auto_بفر<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    لكل (صحيح i = 0; i < n; i++)
    {
        View s = src[i];
        ثابت View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (صحيح)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        إذا (has_Zero_سائيز(s) || sameMapping(s, d))
            استمر;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        منطقي need_عارضي = خطأ;
        لكل (صحيح j = 0; j < n && !need_عارضي; j++)
            need_عارضي = !has_Zero_سائيز(dst[j]) && overlap(s, dst[j]);
        إذا (need_عارضي)
        {
            size_t ڪل = 1;
            لكل (صحيح k = 0; k < s.dims; k++)
                ڪل *= (size_t)s.size[k];
            temps.emplace_back(ڪل*s.esz);
            View t = s;
            t.معلومات = temps.back().معلومات();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            لكل (صحيح k = s.dims - 1; k >= 0; k--)
            {
                t.قدم[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        إذا (buildPlan(s, d, plans[np]))
            np++;
    }
    لاڳيتو_Plans(plans.معلومات(), np);
}

فراغ copy(ثابت View& src, ثابت View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
