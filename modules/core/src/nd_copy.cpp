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

نامخلا cv { نامخلا nd {

View view_کا(ثابت Mat& m)
{
    View v;
    v.ڈیٹا = m.ڈیٹا;
    v.dims = m.dims;
    v.esz = m.elem_سائز();
    برائے (صحیح i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.قدم[i] = (ptrdiff_t)m.قدم[i];
    }
    واپس v;
}

View ترتیب_بدلو(ثابت View& v, ثابت صحیح* ترتیب)
{
    View r;
    r.ڈیٹا = v.ڈیٹا;
    r.dims = v.dims;
    r.esz = v.esz;
    برائے (صحیح i = 0; i < v.dims; i++)
    {
        صحیح j = ترتیب[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.قدم[i] = v.قدم[j];
    }
    واپس r;
}

خالی flip(View& v, صحیح axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    اگر (v.size[axis] > 0)
        v.ڈیٹا += (ptrdiff_t)(v.size[axis] - 1)*v.قدم[axis];
    v.قدم[axis] = -v.قدم[axis];
}

خالی قاش_2(View& v, صحیح axis, صحیح آغاز, صحیح قدم, صحیح گنتی_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && گنتی_2 >= 0);
    اگر (گنتی_2 > 0)
        v.ڈیٹا += (ptrdiff_t)آغاز*v.قدم[axis];
    v.قدم[axis] *= قدم;
    v.size[axis] = گنتی_2;
}

نامخلا {

شمار قسم_3
{
    قسم_MEMCPY,    // the innermost axis is contiguous in both views
    قسم_FILL,      // the innermost axis is broadcast (step 0) in the source
    قسم_STRIDED,
    قسم_TRANSPOSE, // the two innermost axes are swapped between source and destination
    قسم_بانٹو,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    قسم_ملاؤ      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

شمار
{
    TRANSPOSE_TILE = 64,                // in elements
    بانٹو_CHUNK = 1024,                 // in elements
    صف_CHUNK_بائٹیں = 64*1024,
    GATHER_بلاک_بائٹیں = 8*1024,
    // costs are bytes weighted by the kernel; the copies are memory-bound,
    // so small stripes only add overhead
    PARALLEL_MIN_لاگت = 1024*1024,
    STRIPE_لاگت = 1024*1024
};

ساخت Plan
{
    ثابت uchar* src;
    uchar* dst;
    size_t esz;
    صحیح dims;
    صحیح قسم;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    منطقی chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    دوہرا لاگت_2;
};

ساکن خالی تبادلہ_Axes(Plan& p, صحیح i, صحیح j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

ساکن خالی moveAxis(Plan& p, صحیح k, صحیح مقصد)
{
    برائے (صحیح i = k; i < مقصد; i++)
        تبادلہ_Axes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
ساکن size_t پتہ_بٹ(ثابت Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    برائے (صحیح i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    واپس addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
ساکن منطقی try_بانٹو_ملاؤ(Plan& p)
{
    ثابت ptrdiff_t esz = (ptrdiff_t)p.esz;
    ثابت صحیح L = p.dims - 1;
    اگر (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        واپس جھوٹ;
    اگر ((پتہ_بٹ(p) & (esz - 1)) != 0)
        واپس جھوٹ;

    // split: axis k holds cn planes interleaved in the source
    برائے (صحیح k = L - 1; k >= 0; k--)
    {
        ثابت int64 cn = p.size[k];
        اگر (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.قسم = قسم_بانٹو;
            واپس سچ;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ثابت int64 cn = p.size[L];
    اگر (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        برائے (صحیح k = L - 1; k >= 0; k--)
        {
            اگر (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.قسم = قسم_ملاؤ;
                واپس سچ;
            }
        }
    }
    واپس جھوٹ;
}

// Returns false when there is nothing to copy.
ساکن منطقی buildPlan(ثابت View& s, ثابت View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ثابت ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.ڈیٹا;
    p.dst = d.ڈیٹا;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    صحیح n = 0;
    دوہرا کل = 1;
    برائے (صحیح i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        صحیح sz = s.size[i];
        CV_Assert(sz >= 0);
        اگر (sz == 0)
            واپس جھوٹ;
        کل *= sz;
        اگر (sz == 1)
            جاری;
        ptrdiff_t ss = s.قدم[i], ds = d.قدم[i];
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
    برائے (صحیح i = 1; i < n; i++)
        برائے (صحیح j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            تبادلہ_Axes(p, j-1, j);

    صحیح m = 0;
    برائے (صحیح i = 0; i < n; i++)
    {
        اگر (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        ورنہ
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
    p.tfunc = خالی_اشاریہ;
    اگر (p.sstep[L] == esz && p.dstep[L] == esz)
        p.قسم = قسم_MEMCPY;
    ورنہ اگر (p.sstep[L] == 0)
        p.قسم = قسم_FILL;
    ورنہ اگر (!try_بانٹو_ملاؤ(p))
    {
        p.قسم = قسم_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        اگر (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            صحیح k = L - 1;
            جب (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ثابت size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            اگر (k >= 0 && f && p.size[k] >= 8 && (پتہ_بٹ(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.قسم = قسم_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ثابت دوہرا بائٹیں = کل*(دوہرا)esz;
    اگر (p.قسم == قسم_بانٹو || p.قسم == قسم_ملاؤ)
    {
        ثابت صحیح X = p.قسم == قسم_بانٹو ? L : L - 1;
        int64 nouter = 1;
        برائے (صحیح i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = بانٹو_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = جھوٹ;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.لاگت_2 = بائٹیں*2;
    }
    ورنہ اگر (p.قسم == قسم_TRANSPOSE)
    {
        int64 nouter = 1;
        برائے (صحیح i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = جھوٹ;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.لاگت_2 = بائٹیں*3;
    }
    ورنہ
    {
        int64 nrows = 1;
        برائے (صحیح i = 0; i < L; i++)
            nrows *= p.size[i];
        ثابت int64 صف_Len = p.size[L];
        ثابت دوہرا صف_بائٹیں = (دوہرا)صف_Len*esz;
        ثابت ptrdiff_t sstepL = std::مطلق_قدر(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = صف_Len;
        p.chunkMajor = جھوٹ;
        اگر (p.قسم == قسم_STRIDED && L > 0 && nrows <= 64 && std::مطلق_قدر(p.sstep[L-1]) < sstepL &&
            (دوہرا)صف_Len*sstepL > 2.0*GATHER_بلاک_بائٹیں)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_بلاک_بائٹیں/sstepL));
            p.nchunks = (صف_Len + p.chunk - 1)/p.chunk;
            p.chunkMajor = سچ;
        }
        ورنہ اگر (nrows < 64 && صف_بائٹیں >= 2.0*صف_CHUNK_بائٹیں)
        {
            // few long rows: split them to have something to parallelize
            int64 nch = std::min((int64)(صف_بائٹیں/صف_CHUNK_بائٹیں), (int64)1024);
            p.chunk = (صف_Len + nch - 1)/nch;
            p.nchunks = (صف_Len + p.chunk - 1)/p.chunk;
        }
        p.tilesA = p.tilesB = 1;
        p.nitems = nrows*p.nchunks;
        p.لاگت_2 = p.قسم == قسم_MEMCPY ? بائٹیں : بائٹیں*4;
    }
    واپس سچ;
}

سانچہ<قسم_نام T> ساکن اندرونی منطقی isAligned(ثابت خالی* s, ثابت خالی* d, ptrdiff_t ss, ptrdiff_t ds)
{
    واپس (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

سانچہ<قسم_نام T> ساکن خالی copyStrided_(ثابت uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    برائے (; i + 4 <= n; i += 4)
    {
        T t0 = *(ثابت T*)s, t1 = *(ثابت T*)(s + ss);
        T t2 = *(ثابت T*)(s + ss*2), t3 = *(ثابت T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    برائے (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ثابت T*)s;
}

سانچہ<قسم_نام T> ساکن خالی fill_(ثابت uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ثابت T v = *(ثابت T*)s;
    برائے (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

ساخت Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
ساکن خالی copy_صف(ثابت Plan& p, ثابت uchar* s, uchar* d, int64 n)
{
    ثابت size_t esz = p.esz;
    ثابت صحیح L = p.dims - 1;
    ثابت ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    اگر (p.قسم == قسم_FILL)
    {
        اگر (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        ورنہ اگر (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        ورنہ اگر (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        ورنہ اگر (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        ورنہ
            برائے (int64 i = 0; i < n; i++, d += ds)
                حافظہ_نقل(d, s, esz);
        واپس;
    }

    اگر (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    ورنہ اگر (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    ورنہ اگر (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    ورنہ اگر (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    ورنہ اگر (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    ورنہ
        برائے (int64 i = 0; i < n; i++, s += ss, d += ds)
            حافظہ_نقل(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
ساکن خالی decode(ثابت Plan& p, صحیح naxes, int64 idx, int64* counters,
                   ثابت uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    برائے (صحیح k = naxes - 1; k >= 0; k--)
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

سانچہ<منطقی MEMCPY> ساکن اندرونی خالی copy_صف_T(ثابت Plan& p, ثابت uchar* s, uchar* d, int64 n)
{
    اگر (MEMCPY)
        حافظہ_نقل(d, s, (size_t)n*p.esz);
    ورنہ
        copy_صف(p, s, d, n);
}

سانچہ<منطقی MEMCPY> ساکن خالی تسلسل_صفیں(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحیح L = p.dims - 1;
    ثابت int64 nch = p.nchunks, صف_Len = p.size[L];
    ثابت ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ثابت uchar* s;
    uchar* d;

    اگر (p.chunkMajor)
    {
        برائے (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, صف = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, صف_Len);
            decode(p, L, صف, خالی_اشاریہ, s, d);
            copy_صف_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        واپس;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    اگر (nch == 1 && L == 1)
    {
        ثابت ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        برائے (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copy_صف_T<MEMCPY>(p, s, d, صف_Len);
        واپس;
    }

    برائے (int64 it = i0; it < i1; )
    {
        int64 ch_انتہا = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(ch_انتہا*p.chunk, صف_Len);
        copy_صف_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += ch_انتہا - ch;
        اگر (ch_انتہا < nch)
            توڑو;
        ch = 0;
        برائے (صحیح k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            اگر (++counters[k] < p.size[k])
                توڑو;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

ساکن خالی تسلسل_Transpose(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحیح A = p.dims - 2, B = p.dims - 1;
    ثابت int64 ntiles = p.tilesA*p.tilesB;
    برائے (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ثابت uchar* s;
        uchar* d;
        decode(p, A, o, خالی_اشاریہ, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        صحیح wa = (صحیح)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        صحیح hb = (صحیح)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        اگر (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (صحیح)p.esz) == CV_HAL_ERROR_OK)
            جاری;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], سائز(wa, hb));
    }
}

ساکن خالی تسلسل_بانٹو_ملاؤ(ثابت Plan& p, int64 i0, int64 i1)
{
    ثابت صحیح L = p.dims - 1;
    ثابت منطقی is_بانٹو = p.قسم == قسم_بانٹو;
    ثابت صحیح C = is_بانٹو ? L - 1 : L;    // the plane axis
    ثابت صحیح X = is_بانٹو ? L : L - 1;    // the long axis
    ثابت صحیح cn = (صحیح)p.size[C];
    ثابت size_t esz = p.esz;
    برائے (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ثابت uchar* s;
        uchar* d;
        decode(p, L - 1, o, خالی_اشاریہ, s, d);
        ثابت int64 e0 = ch*p.chunk;
        ثابت صحیح len = (صحیح)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        اگر (is_بانٹو)
        {
            uchar* dptr[4];
            برائے (صحیح c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            اگر (esz == 1) hal::split8u(s, dptr, len, cn);
            ورنہ اگر (esz == 2) hal::split16u((ثابت ushort*)s, (ushort**)dptr, len, cn);
            ورنہ اگر (esz == 4) hal::split32s((ثابت صحیح*)s, (صحیح**)dptr, len, cn);
            ورنہ hal::split64s((ثابت int64*)s, (int64**)dptr, len, cn);
        }
        ورنہ
        {
            ثابت uchar* sptr[4];
            برائے (صحیح c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            اگر (esz == 1) hal::merge8u(sptr, d, len, cn);
            ورنہ اگر (esz == 2) hal::merge16u((ثابت ushort**)sptr, (ushort*)d, len, cn);
            ورنہ اگر (esz == 4) hal::merge32s((ثابت صحیح**)sptr, (صحیح*)d, len, cn);
            ورنہ hal::merge64s((ثابت int64**)sptr, (int64*)d, len, cn);
        }
    }
}

ساکن خالی تسلسل_Plan(ثابت Plan& p, int64 i0, int64 i1)
{
    اگر (p.قسم == قسم_بانٹو || p.قسم == قسم_ملاؤ)
        تسلسل_بانٹو_ملاؤ(p, i0, i1);
    ورنہ اگر (p.قسم == قسم_TRANSPOSE)
        تسلسل_Transpose(p, i0, i1);
    ورنہ اگر (p.قسم == قسم_MEMCPY)
        تسلسل_صفیں<سچ>(p, i0, i1);
    ورنہ
        تسلسل_صفیں<جھوٹ>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
ساکن منطقی can_تسلسل_اندر_Lockstep(ثابت Plan* plans, size_t np)
{
    اگر (np < 2)
        واپس جھوٹ;
    ثابت Plan& p0 = plans[0];
    برائے (size_t k = 0; k < np; k++)
    {
        ثابت Plan& p = plans[k];
        اگر (p.قسم == قسم_TRANSPOSE || p.قسم == قسم_بانٹو || p.قسم == قسم_ملاؤ ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            واپس جھوٹ;
        برائے (صحیح i = 0; i < p.dims - 1; i++)
            اگر (p.size[i] != p0.size[i])
                واپس جھوٹ;
    }
    واپس سچ;
}

ساکن خالی تسلسل_Plans(ثابت Plan* plans, size_t np)
{
    اگر (np == 0)
        واپس;
    Auto_بفر<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    دوہرا لاگت_2 = 0, صف_بائٹیں = 0;
    برائے (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        لاگت_2 += plans[k].لاگت_2;
        صف_بائٹیں += (دوہرا)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ثابت منطقی lockstep = can_تسلسل_اندر_Lockstep(plans, np);
    ثابت int64 کل = lockstep ? plans[0].nrows : ofs[np];
    ثابت int64 بلاک_صفیں_2 = std::max((int64)1, (int64)(16*1024/std::max(صف_بائٹیں, 1.)));

    خودکار body = [&](int64 a, int64 b)
    {
        اگر (lockstep)
        {
            برائے (int64 r = a; r < b; r += بلاک_صفیں_2)
            {
                int64 re = std::min(b, r + بلاک_صفیں_2);
                برائے (size_t k = 0; k < np; k++)
                    تسلسل_Plan(plans[k], r, re);
            }
            واپس;
        }
        size_t k = std::upper_bound(ofs.ڈیٹا(), ofs.ڈیٹا() + np + 1, a) - ofs.ڈیٹا() - 1;
        جب (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            تسلسل_Plan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    اگر (لاگت_2 < PARALLEL_MIN_لاگت || کل < 2 || get_عدد_دھاگے() <= 1)
    {
        body(0, کل);
        واپس;
    }

    // grouped so that the range fits into int
    ثابت int64 ngroups = std::min(کل, (int64)1 << 20);
    ثابت دوہرا nstripes = std::min((دوہرا)ngroups, std::max(2.0, لاگت_2/STRIPE_لاگت));
    parallel_for_(Range(0, (صحیح)ngroups), [&](ثابت Range& r)
    {
        body(r.آغاز*کل/ngroups, r.end*کل/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
ساکن خالی viewRange(ثابت View& v, ثابت uchar*& lo, ثابت uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    برائے (صحیح i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.قدم[i];
        اگر (ext < 0) a += ext; ورنہ b += ext;
    }
    lo = v.ڈیٹا + a;
    hi = v.ڈیٹا + b + v.esz;
}

ساکن منطقی has_Zero_سائز(ثابت View& v)
{
    برائے (صحیح i = 0; i < v.dims; i++)
        اگر (v.size[i] == 0)
            واپس سچ;
    واپس جھوٹ;
}

ساکن منطقی sameMapping(ثابت View& s, ثابت View& d)
{
    اگر (s.ڈیٹا != d.ڈیٹا)
        واپس جھوٹ;
    برائے (صحیح i = 0; i < s.dims; i++)
        اگر (s.size[i] > 1 && s.قدم[i] != d.قدم[i])
            واپس جھوٹ;
    واپس سچ;
}

ساکن منطقی overlap(ثابت View& s, ثابت View& d)
{
    ثابت uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    واپس slo < dhi && dlo < shi;
}

} // namespace

خالی copyBatch(ثابت View* src, ثابت View* dst, صحیح n)
{
    CV_Assert(n >= 0);
    Auto_بفر<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<Auto_بفر<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    برائے (صحیح i = 0; i < n; i++)
    {
        View s = src[i];
        ثابت View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (صحیح)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        اگر (has_Zero_سائز(s) || sameMapping(s, d))
            جاری;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        منطقی need_عارضی = جھوٹ;
        برائے (صحیح j = 0; j < n && !need_عارضی; j++)
            need_عارضی = !has_Zero_سائز(dst[j]) && overlap(s, dst[j]);
        اگر (need_عارضی)
        {
            size_t کل = 1;
            برائے (صحیح k = 0; k < s.dims; k++)
                کل *= (size_t)s.size[k];
            temps.emplace_back(کل*s.esz);
            View t = s;
            t.ڈیٹا = temps.back().ڈیٹا();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            برائے (صحیح k = s.dims - 1; k >= 0; k--)
            {
                t.قدم[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        اگر (buildPlan(s, d, plans[np]))
            np++;
    }
    تسلسل_Plans(plans.ڈیٹا(), np);
}

خالی copy(ثابت View& src, ثابت View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
