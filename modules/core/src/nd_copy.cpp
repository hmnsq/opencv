// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "nd_copy.hpp"
#అన్తర్భావ "hal_replacement.hpp"

#అన్తర్భావ "opencv2/core/hal/hal.hpp"

#అన్తర్భావ <algorithm>

నామాకాశ cv { నామాకాశ nd {

View view_యొక్క(స్థిరరాశి Mat& m)
{
    View v;
    v.దత్తాంశం = m.దత్తాంశం;
    v.dims = m.dims;
    v.esz = m.elem_కొలత();
    కొరకు (పూర్ణాంకం i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.అడుగు[i] = (ptrdiff_t)m.అడుగు[i];
    }
    తిరిగి v;
}

View క్రమమార్పు(స్థిరరాశి View& v, స్థిరరాశి పూర్ణాంకం* క్రమం)
{
    View r;
    r.దత్తాంశం = v.దత్తాంశం;
    r.dims = v.dims;
    r.esz = v.esz;
    కొరకు (పూర్ణాంకం i = 0; i < v.dims; i++)
    {
        పూర్ణాంకం j = క్రమం[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.అడుగు[i] = v.అడుగు[j];
    }
    తిరిగి r;
}

శూన్య flip(View& v, పూర్ణాంకం axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    ఒకవేళ (v.size[axis] > 0)
        v.దత్తాంశం += (ptrdiff_t)(v.size[axis] - 1)*v.అడుగు[axis];
    v.అడుగు[axis] = -v.అడుగు[axis];
}

శూన్య ముక్క_2(View& v, పూర్ణాంకం axis, పూర్ణాంకం ప్రారంభం, పూర్ణాంకం అడుగు, పూర్ణాంకం లెక్క_2)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && లెక్క_2 >= 0);
    ఒకవేళ (లెక్క_2 > 0)
        v.దత్తాంశం += (ptrdiff_t)ప్రారంభం*v.అడుగు[axis];
    v.అడుగు[axis] *= అడుగు;
    v.size[axis] = లెక్క_2;
}

నామాకాశ {

గణన రకం_3
{
    రకం_MEMCPY,    // the innermost axis is contiguous in both views
    రకం_FILL,      // the innermost axis is broadcast (step 0) in the source
    రకం_STRIDED,
    రకం_TRANSPOSE, // the two innermost axes are swapped between source and destination
    రకం_విభజించు,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    రకం_విలీనం      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

గణన
{
    TRANSPOSE_TILE = 64,                // in elements
    విభజించు_CHUNK = 1024,                 // in elements
    అడ్డువరుస_CHUNK_బైట్లు = 64*1024,
    GATHER_ఖండం_బైట్లు = 8*1024,
    // costs are bytes weighted by the kernel; the copies are memory-bound,
    // so small stripes only add overhead
    PARALLEL_MIN_ఖర్చు = 1024*1024,
    STRIPE_ఖర్చు = 1024*1024
};

నిర్మాణం Plan
{
    స్థిరరాశి uchar* src;
    uchar* dst;
    size_t esz;
    పూర్ణాంకం dims;
    పూర్ణాంకం రకం;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    తార్కిక chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    ద్విగుణ ఖర్చు_2;
};

స్థిర శూన్య మార్పిడి_Axes(Plan& p, పూర్ణాంకం i, పూర్ణాంకం j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

స్థిర శూన్య moveAxis(Plan& p, పూర్ణాంకం k, పూర్ణాంకం గమ్యం)
{
    కొరకు (పూర్ణాంకం i = k; i < గమ్యం; i++)
        మార్పిడి_Axes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
స్థిర size_t చిరునామా_బిట్లు(స్థిరరాశి Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    కొరకు (పూర్ణాంకం i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    తిరిగి addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
స్థిర తార్కిక try_విభజించు_విలీనం(Plan& p)
{
    స్థిరరాశి ptrdiff_t esz = (ptrdiff_t)p.esz;
    స్థిరరాశి పూర్ణాంకం L = p.dims - 1;
    ఒకవేళ (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        తిరిగి అబద్ధం;
    ఒకవేళ ((చిరునామా_బిట్లు(p) & (esz - 1)) != 0)
        తిరిగి అబద్ధం;

    // split: axis k holds cn planes interleaved in the source
    కొరకు (పూర్ణాంకం k = L - 1; k >= 0; k--)
    {
        స్థిరరాశి int64 cn = p.size[k];
        ఒకవేళ (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.రకం = రకం_విభజించు;
            తిరిగి నిజం;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    స్థిరరాశి int64 cn = p.size[L];
    ఒకవేళ (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        కొరకు (పూర్ణాంకం k = L - 1; k >= 0; k--)
        {
            ఒకవేళ (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.రకం = రకం_విలీనం;
                తిరిగి నిజం;
            }
        }
    }
    తిరిగి అబద్ధం;
}

// Returns false when there is nothing to copy.
స్థిర తార్కిక buildPlan(స్థిరరాశి View& s, స్థిరరాశి View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    స్థిరరాశి ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.దత్తాంశం;
    p.dst = d.దత్తాంశం;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    పూర్ణాంకం n = 0;
    ద్విగుణ మొత్తం = 1;
    కొరకు (పూర్ణాంకం i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        పూర్ణాంకం sz = s.size[i];
        CV_Assert(sz >= 0);
        ఒకవేళ (sz == 0)
            తిరిగి అబద్ధం;
        మొత్తం *= sz;
        ఒకవేళ (sz == 1)
            కొనసాగు;
        ptrdiff_t ss = s.అడుగు[i], ds = d.అడుగు[i];
        ఒకవేళ (ds < 0)
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
    కొరకు (పూర్ణాంకం i = 1; i < n; i++)
        కొరకు (పూర్ణాంకం j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            మార్పిడి_Axes(p, j-1, j);

    పూర్ణాంకం m = 0;
    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
    {
        ఒకవేళ (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        లేకపోతే
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    ఒకవేళ (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    స్థిరరాశి పూర్ణాంకం L = m - 1;
    p.tfunc = శూన్యసూచిక;
    ఒకవేళ (p.sstep[L] == esz && p.dstep[L] == esz)
        p.రకం = రకం_MEMCPY;
    లేకపోతే ఒకవేళ (p.sstep[L] == 0)
        p.రకం = రకం_FILL;
    లేకపోతే ఒకవేళ (!try_విభజించు_విలీనం(p))
    {
        p.రకం = రకం_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        ఒకవేళ (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            పూర్ణాంకం k = L - 1;
            అయితే (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            స్థిరరాశి size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            ఒకవేళ (k >= 0 && f && p.size[k] >= 8 && (చిరునామా_బిట్లు(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.రకం = రకం_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    స్థిరరాశి ద్విగుణ బైట్లు = మొత్తం*(ద్విగుణ)esz;
    ఒకవేళ (p.రకం == రకం_విభజించు || p.రకం == రకం_విలీనం)
    {
        స్థిరరాశి పూర్ణాంకం X = p.రకం == రకం_విభజించు ? L : L - 1;
        int64 nouter = 1;
        కొరకు (పూర్ణాంకం i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = విభజించు_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = అబద్ధం;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.ఖర్చు_2 = బైట్లు*2;
    }
    లేకపోతే ఒకవేళ (p.రకం == రకం_TRANSPOSE)
    {
        int64 nouter = 1;
        కొరకు (పూర్ణాంకం i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = అబద్ధం;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.ఖర్చు_2 = బైట్లు*3;
    }
    లేకపోతే
    {
        int64 nrows = 1;
        కొరకు (పూర్ణాంకం i = 0; i < L; i++)
            nrows *= p.size[i];
        స్థిరరాశి int64 అడ్డువరుస_Len = p.size[L];
        స్థిరరాశి ద్విగుణ అడ్డువరుస_బైట్లు = (ద్విగుణ)అడ్డువరుస_Len*esz;
        స్థిరరాశి ptrdiff_t sstepL = std::నిరపేక్ష(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = అడ్డువరుస_Len;
        p.chunkMajor = అబద్ధం;
        ఒకవేళ (p.రకం == రకం_STRIDED && L > 0 && nrows <= 64 && std::నిరపేక్ష(p.sstep[L-1]) < sstepL &&
            (ద్విగుణ)అడ్డువరుస_Len*sstepL > 2.0*GATHER_ఖండం_బైట్లు)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_ఖండం_బైట్లు/sstepL));
            p.nchunks = (అడ్డువరుస_Len + p.chunk - 1)/p.chunk;
            p.chunkMajor = నిజం;
        }
        లేకపోతే ఒకవేళ (nrows < 64 && అడ్డువరుస_బైట్లు >= 2.0*అడ్డువరుస_CHUNK_బైట్లు)
        {
            // few long rows: split them to have something to parallelize
            int64 nch = std::min((int64)(అడ్డువరుస_బైట్లు/అడ్డువరుస_CHUNK_బైట్లు), (int64)1024);
            p.chunk = (అడ్డువరుస_Len + nch - 1)/nch;
            p.nchunks = (అడ్డువరుస_Len + p.chunk - 1)/p.chunk;
        }
        p.tilesA = p.tilesB = 1;
        p.nitems = nrows*p.nchunks;
        p.ఖర్చు_2 = p.రకం == రకం_MEMCPY ? బైట్లు : బైట్లు*4;
    }
    తిరిగి నిజం;
}

మూస<రకం_పేరు T> స్థిర అంతఃవరుస తార్కిక isAligned(స్థిరరాశి శూన్య* s, స్థిరరాశి శూన్య* d, ptrdiff_t ss, ptrdiff_t ds)
{
    తిరిగి (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

మూస<రకం_పేరు T> స్థిర శూన్య copyStrided_(స్థిరరాశి uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    కొరకు (; i + 4 <= n; i += 4)
    {
        T t0 = *(స్థిరరాశి T*)s, t1 = *(స్థిరరాశి T*)(s + ss);
        T t2 = *(స్థిరరాశి T*)(s + ss*2), t3 = *(స్థిరరాశి T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    కొరకు (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(స్థిరరాశి T*)s;
}

మూస<రకం_పేరు T> స్థిర శూన్య fill_(స్థిరరాశి uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    స్థిరరాశి T v = *(స్థిరరాశి T*)s;
    కొరకు (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

నిర్మాణం Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
స్థిర శూన్య copy_అడ్డువరుస(స్థిరరాశి Plan& p, స్థిరరాశి uchar* s, uchar* d, int64 n)
{
    స్థిరరాశి size_t esz = p.esz;
    స్థిరరాశి పూర్ణాంకం L = p.dims - 1;
    స్థిరరాశి ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    ఒకవేళ (p.రకం == రకం_FILL)
    {
        ఒకవేళ (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        లేకపోతే ఒకవేళ (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        లేకపోతే ఒకవేళ (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        లేకపోతే ఒకవేళ (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        లేకపోతే
            కొరకు (int64 i = 0; i < n; i++, d += ds)
                జ్ఞాపకం_నకలు(d, s, esz);
        తిరిగి;
    }

    ఒకవేళ (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    లేకపోతే ఒకవేళ (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    లేకపోతే ఒకవేళ (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    లేకపోతే ఒకవేళ (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    లేకపోతే ఒకవేళ (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    లేకపోతే
        కొరకు (int64 i = 0; i < n; i++, s += ss, d += ds)
            జ్ఞాపకం_నకలు(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
స్థిర శూన్య decode(స్థిరరాశి Plan& p, పూర్ణాంకం naxes, int64 idx, int64* counters,
                   స్థిరరాశి uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    కొరకు (పూర్ణాంకం k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        ఒకవేళ (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

మూస<తార్కిక MEMCPY> స్థిర అంతఃవరుస శూన్య copy_అడ్డువరుస_T(స్థిరరాశి Plan& p, స్థిరరాశి uchar* s, uchar* d, int64 n)
{
    ఒకవేళ (MEMCPY)
        జ్ఞాపకం_నకలు(d, s, (size_t)n*p.esz);
    లేకపోతే
        copy_అడ్డువరుస(p, s, d, n);
}

మూస<తార్కిక MEMCPY> స్థిర శూన్య నిరంతరం_అడ్డువరుసలు(స్థిరరాశి Plan& p, int64 i0, int64 i1)
{
    స్థిరరాశి పూర్ణాంకం L = p.dims - 1;
    స్థిరరాశి int64 nch = p.nchunks, అడ్డువరుస_Len = p.size[L];
    స్థిరరాశి ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    స్థిరరాశి uchar* s;
    uchar* d;

    ఒకవేళ (p.chunkMajor)
    {
        కొరకు (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, అడ్డువరుస = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, అడ్డువరుస_Len);
            decode(p, L, అడ్డువరుస, శూన్యసూచిక, s, d);
            copy_అడ్డువరుస_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        తిరిగి;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    ఒకవేళ (nch == 1 && L == 1)
    {
        స్థిరరాశి ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        కొరకు (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copy_అడ్డువరుస_T<MEMCPY>(p, s, d, అడ్డువరుస_Len);
        తిరిగి;
    }

    కొరకు (int64 it = i0; it < i1; )
    {
        int64 ch_చివర = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(ch_చివర*p.chunk, అడ్డువరుస_Len);
        copy_అడ్డువరుస_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += ch_చివర - ch;
        ఒకవేళ (ch_చివర < nch)
            ఆపు;
        ch = 0;
        కొరకు (పూర్ణాంకం k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            ఒకవేళ (++counters[k] < p.size[k])
                ఆపు;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

స్థిర శూన్య నిరంతరం_Transpose(స్థిరరాశి Plan& p, int64 i0, int64 i1)
{
    స్థిరరాశి పూర్ణాంకం A = p.dims - 2, B = p.dims - 1;
    స్థిరరాశి int64 ntiles = p.tilesA*p.tilesB;
    కొరకు (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        స్థిరరాశి uchar* s;
        uchar* d;
        decode(p, A, o, శూన్యసూచిక, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        పూర్ణాంకం wa = (పూర్ణాంకం)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        పూర్ణాంకం hb = (పూర్ణాంకం)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        ఒకవేళ (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (పూర్ణాంకం)p.esz) == CV_HAL_ERROR_OK)
            కొనసాగు;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], కొలత(wa, hb));
    }
}

స్థిర శూన్య నిరంతరం_విభజించు_విలీనం(స్థిరరాశి Plan& p, int64 i0, int64 i1)
{
    స్థిరరాశి పూర్ణాంకం L = p.dims - 1;
    స్థిరరాశి తార్కిక is_విభజించు = p.రకం == రకం_విభజించు;
    స్థిరరాశి పూర్ణాంకం C = is_విభజించు ? L - 1 : L;    // the plane axis
    స్థిరరాశి పూర్ణాంకం X = is_విభజించు ? L : L - 1;    // the long axis
    స్థిరరాశి పూర్ణాంకం cn = (పూర్ణాంకం)p.size[C];
    స్థిరరాశి size_t esz = p.esz;
    కొరకు (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        స్థిరరాశి uchar* s;
        uchar* d;
        decode(p, L - 1, o, శూన్యసూచిక, s, d);
        స్థిరరాశి int64 e0 = ch*p.chunk;
        స్థిరరాశి పూర్ణాంకం len = (పూర్ణాంకం)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        ఒకవేళ (is_విభజించు)
        {
            uchar* dptr[4];
            కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            ఒకవేళ (esz == 1) hal::split8u(s, dptr, len, cn);
            లేకపోతే ఒకవేళ (esz == 2) hal::split16u((స్థిరరాశి ushort*)s, (ushort**)dptr, len, cn);
            లేకపోతే ఒకవేళ (esz == 4) hal::split32s((స్థిరరాశి పూర్ణాంకం*)s, (పూర్ణాంకం**)dptr, len, cn);
            లేకపోతే hal::split64s((స్థిరరాశి int64*)s, (int64**)dptr, len, cn);
        }
        లేకపోతే
        {
            స్థిరరాశి uchar* sptr[4];
            కొరకు (పూర్ణాంకం c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            ఒకవేళ (esz == 1) hal::merge8u(sptr, d, len, cn);
            లేకపోతే ఒకవేళ (esz == 2) hal::merge16u((స్థిరరాశి ushort**)sptr, (ushort*)d, len, cn);
            లేకపోతే ఒకవేళ (esz == 4) hal::merge32s((స్థిరరాశి పూర్ణాంకం**)sptr, (పూర్ణాంకం*)d, len, cn);
            లేకపోతే hal::merge64s((స్థిరరాశి int64**)sptr, (int64*)d, len, cn);
        }
    }
}

స్థిర శూన్య నిరంతరం_Plan(స్థిరరాశి Plan& p, int64 i0, int64 i1)
{
    ఒకవేళ (p.రకం == రకం_విభజించు || p.రకం == రకం_విలీనం)
        నిరంతరం_విభజించు_విలీనం(p, i0, i1);
    లేకపోతే ఒకవేళ (p.రకం == రకం_TRANSPOSE)
        నిరంతరం_Transpose(p, i0, i1);
    లేకపోతే ఒకవేళ (p.రకం == రకం_MEMCPY)
        నిరంతరం_అడ్డువరుసలు<నిజం>(p, i0, i1);
    లేకపోతే
        నిరంతరం_అడ్డువరుసలు<అబద్ధం>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
స్థిర తార్కిక can_నిరంతరం_లోపల_Lockstep(స్థిరరాశి Plan* plans, size_t np)
{
    ఒకవేళ (np < 2)
        తిరిగి అబద్ధం;
    స్థిరరాశి Plan& p0 = plans[0];
    కొరకు (size_t k = 0; k < np; k++)
    {
        స్థిరరాశి Plan& p = plans[k];
        ఒకవేళ (p.రకం == రకం_TRANSPOSE || p.రకం == రకం_విభజించు || p.రకం == రకం_విలీనం ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            తిరిగి అబద్ధం;
        కొరకు (పూర్ణాంకం i = 0; i < p.dims - 1; i++)
            ఒకవేళ (p.size[i] != p0.size[i])
                తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
}

స్థిర శూన్య నిరంతరం_Plans(స్థిరరాశి Plan* plans, size_t np)
{
    ఒకవేళ (np == 0)
        తిరిగి;
    Auto_బఫరు<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    ద్విగుణ ఖర్చు_2 = 0, అడ్డువరుస_బైట్లు = 0;
    కొరకు (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        ఖర్చు_2 += plans[k].ఖర్చు_2;
        అడ్డువరుస_బైట్లు += (ద్విగుణ)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    స్థిరరాశి తార్కిక lockstep = can_నిరంతరం_లోపల_Lockstep(plans, np);
    స్థిరరాశి int64 మొత్తం = lockstep ? plans[0].nrows : ofs[np];
    స్థిరరాశి int64 ఖండం_అడ్డువరుసలు_2 = std::max((int64)1, (int64)(16*1024/std::max(అడ్డువరుస_బైట్లు, 1.)));

    స్వయంచాలక body = [&](int64 a, int64 b)
    {
        ఒకవేళ (lockstep)
        {
            కొరకు (int64 r = a; r < b; r += ఖండం_అడ్డువరుసలు_2)
            {
                int64 re = std::min(b, r + ఖండం_అడ్డువరుసలు_2);
                కొరకు (size_t k = 0; k < np; k++)
                    నిరంతరం_Plan(plans[k], r, re);
            }
            తిరిగి;
        }
        size_t k = std::upper_bound(ofs.దత్తాంశం(), ofs.దత్తాంశం() + np + 1, a) - ofs.దత్తాంశం() - 1;
        అయితే (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            నిరంతరం_Plan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    ఒకవేళ (ఖర్చు_2 < PARALLEL_MIN_ఖర్చు || మొత్తం < 2 || get_అంకె_దారాలు() <= 1)
    {
        body(0, మొత్తం);
        తిరిగి;
    }

    // grouped so that the range fits into int
    స్థిరరాశి int64 ngroups = std::min(మొత్తం, (int64)1 << 20);
    స్థిరరాశి ద్విగుణ nstripes = std::min((ద్విగుణ)ngroups, std::max(2.0, ఖర్చు_2/STRIPE_ఖర్చు));
    parallel_for_(Range(0, (పూర్ణాంకం)ngroups), [&](స్థిరరాశి Range& r)
    {
        body(r.ప్రారంభం*మొత్తం/ngroups, r.end*మొత్తం/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
స్థిర శూన్య viewRange(స్థిరరాశి View& v, స్థిరరాశి uchar*& lo, స్థిరరాశి uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    కొరకు (పూర్ణాంకం i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.అడుగు[i];
        ఒకవేళ (ext < 0) a += ext; లేకపోతే b += ext;
    }
    lo = v.దత్తాంశం + a;
    hi = v.దత్తాంశం + b + v.esz;
}

స్థిర తార్కిక has_Zero_కొలత(స్థిరరాశి View& v)
{
    కొరకు (పూర్ణాంకం i = 0; i < v.dims; i++)
        ఒకవేళ (v.size[i] == 0)
            తిరిగి నిజం;
    తిరిగి అబద్ధం;
}

స్థిర తార్కిక sameMapping(స్థిరరాశి View& s, స్థిరరాశి View& d)
{
    ఒకవేళ (s.దత్తాంశం != d.దత్తాంశం)
        తిరిగి అబద్ధం;
    కొరకు (పూర్ణాంకం i = 0; i < s.dims; i++)
        ఒకవేళ (s.size[i] > 1 && s.అడుగు[i] != d.అడుగు[i])
            తిరిగి అబద్ధం;
    తిరిగి నిజం;
}

స్థిర తార్కిక overlap(స్థిరరాశి View& s, స్థిరరాశి View& d)
{
    స్థిరరాశి uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    తిరిగి slo < dhi && dlo < shi;
}

} // namespace

శూన్య copyBatch(స్థిరరాశి View* src, స్థిరరాశి View* dst, పూర్ణాంకం n)
{
    CV_Assert(n >= 0);
    Auto_బఫరు<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<Auto_బఫరు<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
    {
        View s = src[i];
        స్థిరరాశి View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (పూర్ణాంకం)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        ఒకవేళ (has_Zero_కొలత(s) || sameMapping(s, d))
            కొనసాగు;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        తార్కిక need_తాత్కాలిక = అబద్ధం;
        కొరకు (పూర్ణాంకం j = 0; j < n && !need_తాత్కాలిక; j++)
            need_తాత్కాలిక = !has_Zero_కొలత(dst[j]) && overlap(s, dst[j]);
        ఒకవేళ (need_తాత్కాలిక)
        {
            size_t మొత్తం = 1;
            కొరకు (పూర్ణాంకం k = 0; k < s.dims; k++)
                మొత్తం *= (size_t)s.size[k];
            temps.emplace_back(మొత్తం*s.esz);
            View t = s;
            t.దత్తాంశం = temps.back().దత్తాంశం();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            కొరకు (పూర్ణాంకం k = s.dims - 1; k >= 0; k--)
            {
                t.అడుగు[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        ఒకవేళ (buildPlan(s, d, plans[np]))
            np++;
    }
    నిరంతరం_Plans(plans.దత్తాంశం(), np);
}

శూన్య copy(స్థిరరాశి View& src, స్థిరరాశి View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
