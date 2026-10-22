// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "nd_copy.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal_replacement.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/hal/hal.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ nd {

View view_ਦਾ(ਅਚਲ Mat& m)
{
    View v;
    v.ਅੰਕੜੇ = m.ਅੰਕੜੇ;
    v.dims = m.dims;
    v.esz = m.elem_ਮਾਪ();
    ਲਈ (ਪੂਰਨ i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.ਕਦਮ[i] = (ptrdiff_t)m.ਕਦਮ[i];
    }
    ਵਾਪਸ v;
}

View ਕ੍ਰਮ_ਬਦਲੋ(ਅਚਲ View& v, ਅਚਲ ਪੂਰਨ* ਕ੍ਰਮ)
{
    View r;
    r.ਅੰਕੜੇ = v.ਅੰਕੜੇ;
    r.dims = v.dims;
    r.esz = v.esz;
    ਲਈ (ਪੂਰਨ i = 0; i < v.dims; i++)
    {
        ਪੂਰਨ j = ਕ੍ਰਮ[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.ਕਦਮ[i] = v.ਕਦਮ[j];
    }
    ਵਾਪਸ r;
}

ਖਾਲੀ flip(View& v, ਪੂਰਨ axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    ਜੇ (v.size[axis] > 0)
        v.ਅੰਕੜੇ += (ptrdiff_t)(v.size[axis] - 1)*v.ਕਦਮ[axis];
    v.ਕਦਮ[axis] = -v.ਕਦਮ[axis];
}

ਖਾਲੀ ਟੁਕੜਾ_2(View& v, ਪੂਰਨ axis, ਪੂਰਨ ਸ਼ੁਰੂ, ਪੂਰਨ ਕਦਮ, ਪੂਰਨ ਗਿਣਤੀ_3)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && ਗਿਣਤੀ_3 >= 0);
    ਜੇ (ਗਿਣਤੀ_3 > 0)
        v.ਅੰਕੜੇ += (ptrdiff_t)ਸ਼ੁਰੂ*v.ਕਦਮ[axis];
    v.ਕਦਮ[axis] *= ਕਦਮ;
    v.size[axis] = ਗਿਣਤੀ_3;
}

ਨਾਮਾਕਾਸ਼ {

ਗਿਣਤੀ ਕਿਸਮ_3
{
    ਕਿਸਮ_MEMCPY,    // the innermost axis is contiguous in both views
    ਕਿਸਮ_FILL,      // the innermost axis is broadcast (step 0) in the source
    ਕਿਸਮ_STRIDED,
    ਕਿਸਮ_TRANSPOSE, // the two innermost axes are swapped between source and destination
    ਕਿਸਮ_ਵੰਡੋ,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    ਕਿਸਮ_ਮਿਲਾਓ      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

ਗਿਣਤੀ
{
    TRANSPOSE_TILE = 64,                // in elements
    ਵੰਡੋ_CHUNK = 1024,                 // in elements
    ਪੰਕਤੀ_CHUNK_ਬਾਈਟਾਂ = 64*1024,
    GATHER_ਖੰਡ_ਬਾਈਟਾਂ = 8*1024,
    // costs are bytes weighted by the kernel; the copies are memory-bound,
    // so small stripes only add overhead
    PARALLEL_MIN_ਲਾਗਤ = 1024*1024,
    STRIPE_ਲਾਗਤ = 1024*1024
};

ਬਣਤਰ Plan
{
    ਅਚਲ uchar* src;
    uchar* dst;
    size_t esz;
    ਪੂਰਨ dims;
    ਪੂਰਨ ਕਿਸਮ;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    ਤਾਰਕਿਕ chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    ਦੁੱਗਣਾ ਲਾਗਤ_2;
};

ਸਥਿਰ ਖਾਲੀ ਅਦਲਾ_ਬਦਲੀ_Axes(Plan& p, ਪੂਰਨ i, ਪੂਰਨ j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

ਸਥਿਰ ਖਾਲੀ moveAxis(Plan& p, ਪੂਰਨ k, ਪੂਰਨ ਮੰਜ਼ਿਲ)
{
    ਲਈ (ਪੂਰਨ i = k; i < ਮੰਜ਼ਿਲ; i++)
        ਅਦਲਾ_ਬਦਲੀ_Axes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
ਸਥਿਰ size_t ਪਤਾ_ਬਿੱਟ(ਅਚਲ Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    ਲਈ (ਪੂਰਨ i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    ਵਾਪਸ addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
ਸਥਿਰ ਤਾਰਕਿਕ try_ਵੰਡੋ_ਮਿਲਾਓ(Plan& p)
{
    ਅਚਲ ptrdiff_t esz = (ptrdiff_t)p.esz;
    ਅਚਲ ਪੂਰਨ L = p.dims - 1;
    ਜੇ (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        ਵਾਪਸ ਝੂਠ;
    ਜੇ ((ਪਤਾ_ਬਿੱਟ(p) & (esz - 1)) != 0)
        ਵਾਪਸ ਝੂਠ;

    // split: axis k holds cn planes interleaved in the source
    ਲਈ (ਪੂਰਨ k = L - 1; k >= 0; k--)
    {
        ਅਚਲ int64 cn = p.size[k];
        ਜੇ (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.ਕਿਸਮ = ਕਿਸਮ_ਵੰਡੋ;
            ਵਾਪਸ ਸੱਚ;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ਅਚਲ int64 cn = p.size[L];
    ਜੇ (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        ਲਈ (ਪੂਰਨ k = L - 1; k >= 0; k--)
        {
            ਜੇ (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.ਕਿਸਮ = ਕਿਸਮ_ਮਿਲਾਓ;
                ਵਾਪਸ ਸੱਚ;
            }
        }
    }
    ਵਾਪਸ ਝੂਠ;
}

// Returns false when there is nothing to copy.
ਸਥਿਰ ਤਾਰਕਿਕ buildPlan(ਅਚਲ View& s, ਅਚਲ View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ਅਚਲ ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.ਅੰਕੜੇ;
    p.dst = d.ਅੰਕੜੇ;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    ਪੂਰਨ n = 0;
    ਦੁੱਗਣਾ ਕੁੱਲ = 1;
    ਲਈ (ਪੂਰਨ i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        ਪੂਰਨ sz = s.size[i];
        CV_Assert(sz >= 0);
        ਜੇ (sz == 0)
            ਵਾਪਸ ਝੂਠ;
        ਕੁੱਲ *= sz;
        ਜੇ (sz == 1)
            ਜਾਰੀ;
        ptrdiff_t ss = s.ਕਦਮ[i], ds = d.ਕਦਮ[i];
        ਜੇ (ds < 0)
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
    ਲਈ (ਪੂਰਨ i = 1; i < n; i++)
        ਲਈ (ਪੂਰਨ j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            ਅਦਲਾ_ਬਦਲੀ_Axes(p, j-1, j);

    ਪੂਰਨ m = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
    {
        ਜੇ (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        ਨਹੀਂਤਾਂ
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    ਜੇ (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    ਅਚਲ ਪੂਰਨ L = m - 1;
    p.tfunc = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (p.sstep[L] == esz && p.dstep[L] == esz)
        p.ਕਿਸਮ = ਕਿਸਮ_MEMCPY;
    ਨਹੀਂਤਾਂ ਜੇ (p.sstep[L] == 0)
        p.ਕਿਸਮ = ਕਿਸਮ_FILL;
    ਨਹੀਂਤਾਂ ਜੇ (!try_ਵੰਡੋ_ਮਿਲਾਓ(p))
    {
        p.ਕਿਸਮ = ਕਿਸਮ_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        ਜੇ (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            ਪੂਰਨ k = L - 1;
            ਜਦੋਂ (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ਅਚਲ size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            ਜੇ (k >= 0 && f && p.size[k] >= 8 && (ਪਤਾ_ਬਿੱਟ(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.ਕਿਸਮ = ਕਿਸਮ_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ਅਚਲ ਦੁੱਗਣਾ ਬਾਈਟਾਂ = ਕੁੱਲ*(ਦੁੱਗਣਾ)esz;
    ਜੇ (p.ਕਿਸਮ == ਕਿਸਮ_ਵੰਡੋ || p.ਕਿਸਮ == ਕਿਸਮ_ਮਿਲਾਓ)
    {
        ਅਚਲ ਪੂਰਨ X = p.ਕਿਸਮ == ਕਿਸਮ_ਵੰਡੋ ? L : L - 1;
        int64 nouter = 1;
        ਲਈ (ਪੂਰਨ i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = ਵੰਡੋ_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = ਝੂਠ;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.ਲਾਗਤ_2 = ਬਾਈਟਾਂ*2;
    }
    ਨਹੀਂਤਾਂ ਜੇ (p.ਕਿਸਮ == ਕਿਸਮ_TRANSPOSE)
    {
        int64 nouter = 1;
        ਲਈ (ਪੂਰਨ i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = ਝੂਠ;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.ਲਾਗਤ_2 = ਬਾਈਟਾਂ*3;
    }
    ਨਹੀਂਤਾਂ
    {
        int64 nrows = 1;
        ਲਈ (ਪੂਰਨ i = 0; i < L; i++)
            nrows *= p.size[i];
        ਅਚਲ int64 ਪੰਕਤੀ_Len = p.size[L];
        ਅਚਲ ਦੁੱਗਣਾ ਪੰਕਤੀ_ਬਾਈਟਾਂ = (ਦੁੱਗਣਾ)ਪੰਕਤੀ_Len*esz;
        ਅਚਲ ptrdiff_t sstepL = std::ਨਿਰਪੇਖ(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = ਪੰਕਤੀ_Len;
        p.chunkMajor = ਝੂਠ;
        ਜੇ (p.ਕਿਸਮ == ਕਿਸਮ_STRIDED && L > 0 && nrows <= 64 && std::ਨਿਰਪੇਖ(p.sstep[L-1]) < sstepL &&
            (ਦੁੱਗਣਾ)ਪੰਕਤੀ_Len*sstepL > 2.0*GATHER_ਖੰਡ_ਬਾਈਟਾਂ)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_ਖੰਡ_ਬਾਈਟਾਂ/sstepL));
            p.nchunks = (ਪੰਕਤੀ_Len + p.chunk - 1)/p.chunk;
            p.chunkMajor = ਸੱਚ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (nrows < 64 && ਪੰਕਤੀ_ਬਾਈਟਾਂ >= 2.0*ਪੰਕਤੀ_CHUNK_ਬਾਈਟਾਂ)
        {
            // few long rows: split them to have something to parallelize
            int64 nch = std::min((int64)(ਪੰਕਤੀ_ਬਾਈਟਾਂ/ਪੰਕਤੀ_CHUNK_ਬਾਈਟਾਂ), (int64)1024);
            p.chunk = (ਪੰਕਤੀ_Len + nch - 1)/nch;
            p.nchunks = (ਪੰਕਤੀ_Len + p.chunk - 1)/p.chunk;
        }
        p.tilesA = p.tilesB = 1;
        p.nitems = nrows*p.nchunks;
        p.ਲਾਗਤ_2 = p.ਕਿਸਮ == ਕਿਸਮ_MEMCPY ? ਬਾਈਟਾਂ : ਬਾਈਟਾਂ*4;
    }
    ਵਾਪਸ ਸੱਚ;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ isAligned(ਅਚਲ ਖਾਲੀ* s, ਅਚਲ ਖਾਲੀ* d, ptrdiff_t ss, ptrdiff_t ds)
{
    ਵਾਪਸ (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ copyStrided_(ਅਚਲ uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    ਲਈ (; i + 4 <= n; i += 4)
    {
        T t0 = *(ਅਚਲ T*)s, t1 = *(ਅਚਲ T*)(s + ss);
        T t2 = *(ਅਚਲ T*)(s + ss*2), t3 = *(ਅਚਲ T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    ਲਈ (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ਅਚਲ T*)s;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ fill_(ਅਚਲ uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ਅਚਲ T v = *(ਅਚਲ T*)s;
    ਲਈ (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

ਬਣਤਰ Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
ਸਥਿਰ ਖਾਲੀ copy_ਪੰਕਤੀ(ਅਚਲ Plan& p, ਅਚਲ uchar* s, uchar* d, int64 n)
{
    ਅਚਲ size_t esz = p.esz;
    ਅਚਲ ਪੂਰਨ L = p.dims - 1;
    ਅਚਲ ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    ਜੇ (p.ਕਿਸਮ == ਕਿਸਮ_FILL)
    {
        ਜੇ (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        ਨਹੀਂਤਾਂ ਜੇ (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        ਨਹੀਂਤਾਂ ਜੇ (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        ਨਹੀਂਤਾਂ ਜੇ (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        ਨਹੀਂਤਾਂ
            ਲਈ (int64 i = 0; i < n; i++, d += ds)
                ਸਮ੍ਰਿਤੀ_ਨਕਲ(d, s, esz);
        ਵਾਪਸ;
    }

    ਜੇ (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    ਨਹੀਂਤਾਂ ਜੇ (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    ਨਹੀਂਤਾਂ ਜੇ (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    ਨਹੀਂਤਾਂ ਜੇ (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    ਨਹੀਂਤਾਂ ਜੇ (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    ਨਹੀਂਤਾਂ
        ਲਈ (int64 i = 0; i < n; i++, s += ss, d += ds)
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
ਸਥਿਰ ਖਾਲੀ decode(ਅਚਲ Plan& p, ਪੂਰਨ naxes, int64 idx, int64* counters,
                   ਅਚਲ uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    ਲਈ (ਪੂਰਨ k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        ਜੇ (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

ਸਾਂਚਾ<ਤਾਰਕਿਕ MEMCPY> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ copy_ਪੰਕਤੀ_T(ਅਚਲ Plan& p, ਅਚਲ uchar* s, uchar* d, int64 n)
{
    ਜੇ (MEMCPY)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(d, s, (size_t)n*p.esz);
    ਨਹੀਂਤਾਂ
        copy_ਪੰਕਤੀ(p, s, d, n);
}

ਸਾਂਚਾ<ਤਾਰਕਿਕ MEMCPY> ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_ਪੰਕਤੀਆਂ(ਅਚਲ Plan& p, int64 i0, int64 i1)
{
    ਅਚਲ ਪੂਰਨ L = p.dims - 1;
    ਅਚਲ int64 nch = p.nchunks, ਪੰਕਤੀ_Len = p.size[L];
    ਅਚਲ ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ਅਚਲ uchar* s;
    uchar* d;

    ਜੇ (p.chunkMajor)
    {
        ਲਈ (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, ਪੰਕਤੀ = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, ਪੰਕਤੀ_Len);
            decode(p, L, ਪੰਕਤੀ, ਖਾਲੀ_ਸੂਚਕ, s, d);
            copy_ਪੰਕਤੀ_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        ਵਾਪਸ;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    ਜੇ (nch == 1 && L == 1)
    {
        ਅਚਲ ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        ਲਈ (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copy_ਪੰਕਤੀ_T<MEMCPY>(p, s, d, ਪੰਕਤੀ_Len);
        ਵਾਪਸ;
    }

    ਲਈ (int64 it = i0; it < i1; )
    {
        int64 ch_ਸਿਰਾ = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(ch_ਸਿਰਾ*p.chunk, ਪੰਕਤੀ_Len);
        copy_ਪੰਕਤੀ_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += ch_ਸਿਰਾ - ch;
        ਜੇ (ch_ਸਿਰਾ < nch)
            ਰੋਕੋ;
        ch = 0;
        ਲਈ (ਪੂਰਨ k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            ਜੇ (++counters[k] < p.size[k])
                ਰੋਕੋ;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Transpose(ਅਚਲ Plan& p, int64 i0, int64 i1)
{
    ਅਚਲ ਪੂਰਨ A = p.dims - 2, B = p.dims - 1;
    ਅਚਲ int64 ntiles = p.tilesA*p.tilesB;
    ਲਈ (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ਅਚਲ uchar* s;
        uchar* d;
        decode(p, A, o, ਖਾਲੀ_ਸੂਚਕ, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        ਪੂਰਨ wa = (ਪੂਰਨ)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        ਪੂਰਨ hb = (ਪੂਰਨ)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        ਜੇ (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (ਪੂਰਨ)p.esz) == CV_HAL_ERROR_OK)
            ਜਾਰੀ;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], ਮਾਪ(wa, hb));
    }
}

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_ਵੰਡੋ_ਮਿਲਾਓ(ਅਚਲ Plan& p, int64 i0, int64 i1)
{
    ਅਚਲ ਪੂਰਨ L = p.dims - 1;
    ਅਚਲ ਤਾਰਕਿਕ is_ਵੰਡੋ = p.ਕਿਸਮ == ਕਿਸਮ_ਵੰਡੋ;
    ਅਚਲ ਪੂਰਨ C = is_ਵੰਡੋ ? L - 1 : L;    // the plane axis
    ਅਚਲ ਪੂਰਨ X = is_ਵੰਡੋ ? L : L - 1;    // the long axis
    ਅਚਲ ਪੂਰਨ cn = (ਪੂਰਨ)p.size[C];
    ਅਚਲ size_t esz = p.esz;
    ਲਈ (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ਅਚਲ uchar* s;
        uchar* d;
        decode(p, L - 1, o, ਖਾਲੀ_ਸੂਚਕ, s, d);
        ਅਚਲ int64 e0 = ch*p.chunk;
        ਅਚਲ ਪੂਰਨ len = (ਪੂਰਨ)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        ਜੇ (is_ਵੰਡੋ)
        {
            uchar* dptr[4];
            ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            ਜੇ (esz == 1) hal::split8u(s, dptr, len, cn);
            ਨਹੀਂਤਾਂ ਜੇ (esz == 2) hal::split16u((ਅਚਲ ushort*)s, (ushort**)dptr, len, cn);
            ਨਹੀਂਤਾਂ ਜੇ (esz == 4) hal::split32s((ਅਚਲ ਪੂਰਨ*)s, (ਪੂਰਨ**)dptr, len, cn);
            ਨਹੀਂਤਾਂ hal::split64s((ਅਚਲ int64*)s, (int64**)dptr, len, cn);
        }
        ਨਹੀਂਤਾਂ
        {
            ਅਚਲ uchar* sptr[4];
            ਲਈ (ਪੂਰਨ c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            ਜੇ (esz == 1) hal::merge8u(sptr, d, len, cn);
            ਨਹੀਂਤਾਂ ਜੇ (esz == 2) hal::merge16u((ਅਚਲ ushort**)sptr, (ushort*)d, len, cn);
            ਨਹੀਂਤਾਂ ਜੇ (esz == 4) hal::merge32s((ਅਚਲ ਪੂਰਨ**)sptr, (ਪੂਰਨ*)d, len, cn);
            ਨਹੀਂਤਾਂ hal::merge64s((ਅਚਲ int64**)sptr, (int64*)d, len, cn);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Plan(ਅਚਲ Plan& p, int64 i0, int64 i1)
{
    ਜੇ (p.ਕਿਸਮ == ਕਿਸਮ_ਵੰਡੋ || p.ਕਿਸਮ == ਕਿਸਮ_ਮਿਲਾਓ)
        ਲੜੀਵਾਰ_ਵੰਡੋ_ਮਿਲਾਓ(p, i0, i1);
    ਨਹੀਂਤਾਂ ਜੇ (p.ਕਿਸਮ == ਕਿਸਮ_TRANSPOSE)
        ਲੜੀਵਾਰ_Transpose(p, i0, i1);
    ਨਹੀਂਤਾਂ ਜੇ (p.ਕਿਸਮ == ਕਿਸਮ_MEMCPY)
        ਲੜੀਵਾਰ_ਪੰਕਤੀਆਂ<ਸੱਚ>(p, i0, i1);
    ਨਹੀਂਤਾਂ
        ਲੜੀਵਾਰ_ਪੰਕਤੀਆਂ<ਝੂਠ>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
ਸਥਿਰ ਤਾਰਕਿਕ can_ਲੜੀਵਾਰ_ਵਿੱਚ_Lockstep(ਅਚਲ Plan* plans, size_t np)
{
    ਜੇ (np < 2)
        ਵਾਪਸ ਝੂਠ;
    ਅਚਲ Plan& p0 = plans[0];
    ਲਈ (size_t k = 0; k < np; k++)
    {
        ਅਚਲ Plan& p = plans[k];
        ਜੇ (p.ਕਿਸਮ == ਕਿਸਮ_TRANSPOSE || p.ਕਿਸਮ == ਕਿਸਮ_ਵੰਡੋ || p.ਕਿਸਮ == ਕਿਸਮ_ਮਿਲਾਓ ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            ਵਾਪਸ ਝੂਠ;
        ਲਈ (ਪੂਰਨ i = 0; i < p.dims - 1; i++)
            ਜੇ (p.size[i] != p0.size[i])
                ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_Plans(ਅਚਲ Plan* plans, size_t np)
{
    ਜੇ (np == 0)
        ਵਾਪਸ;
    Auto_ਬਫਰ<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    ਦੁੱਗਣਾ ਲਾਗਤ_2 = 0, ਪੰਕਤੀ_ਬਾਈਟਾਂ = 0;
    ਲਈ (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        ਲਾਗਤ_2 += plans[k].ਲਾਗਤ_2;
        ਪੰਕਤੀ_ਬਾਈਟਾਂ += (ਦੁੱਗਣਾ)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ਅਚਲ ਤਾਰਕਿਕ lockstep = can_ਲੜੀਵਾਰ_ਵਿੱਚ_Lockstep(plans, np);
    ਅਚਲ int64 ਕੁੱਲ = lockstep ? plans[0].nrows : ofs[np];
    ਅਚਲ int64 ਖੰਡ_ਪੰਕਤੀਆਂ_2 = std::max((int64)1, (int64)(16*1024/std::max(ਪੰਕਤੀ_ਬਾਈਟਾਂ, 1.)));

    ਸਵੈਚਾਲਿਤ body = [&](int64 a, int64 b)
    {
        ਜੇ (lockstep)
        {
            ਲਈ (int64 r = a; r < b; r += ਖੰਡ_ਪੰਕਤੀਆਂ_2)
            {
                int64 re = std::min(b, r + ਖੰਡ_ਪੰਕਤੀਆਂ_2);
                ਲਈ (size_t k = 0; k < np; k++)
                    ਲੜੀਵਾਰ_Plan(plans[k], r, re);
            }
            ਵਾਪਸ;
        }
        size_t k = std::upper_bound(ofs.ਅੰਕੜੇ(), ofs.ਅੰਕੜੇ() + np + 1, a) - ofs.ਅੰਕੜੇ() - 1;
        ਜਦੋਂ (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            ਲੜੀਵਾਰ_Plan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    ਜੇ (ਲਾਗਤ_2 < PARALLEL_MIN_ਲਾਗਤ || ਕੁੱਲ < 2 || get_ਅੰਕ_ਧਾਗੇ() <= 1)
    {
        body(0, ਕੁੱਲ);
        ਵਾਪਸ;
    }

    // grouped so that the range fits into int
    ਅਚਲ int64 ngroups = std::min(ਕੁੱਲ, (int64)1 << 20);
    ਅਚਲ ਦੁੱਗਣਾ nstripes = std::min((ਦੁੱਗਣਾ)ngroups, std::max(2.0, ਲਾਗਤ_2/STRIPE_ਲਾਗਤ));
    parallel_for_(Range(0, (ਪੂਰਨ)ngroups), [&](ਅਚਲ Range& r)
    {
        body(r.ਸ਼ੁਰੂ*ਕੁੱਲ/ngroups, r.end*ਕੁੱਲ/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
ਸਥਿਰ ਖਾਲੀ viewRange(ਅਚਲ View& v, ਅਚਲ uchar*& lo, ਅਚਲ uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.ਕਦਮ[i];
        ਜੇ (ext < 0) a += ext; ਨਹੀਂਤਾਂ b += ext;
    }
    lo = v.ਅੰਕੜੇ + a;
    hi = v.ਅੰਕੜੇ + b + v.esz;
}

ਸਥਿਰ ਤਾਰਕਿਕ has_Zero_ਮਾਪ(ਅਚਲ View& v)
{
    ਲਈ (ਪੂਰਨ i = 0; i < v.dims; i++)
        ਜੇ (v.size[i] == 0)
            ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਤਾਰਕਿਕ sameMapping(ਅਚਲ View& s, ਅਚਲ View& d)
{
    ਜੇ (s.ਅੰਕੜੇ != d.ਅੰਕੜੇ)
        ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਪੂਰਨ i = 0; i < s.dims; i++)
        ਜੇ (s.size[i] > 1 && s.ਕਦਮ[i] != d.ਕਦਮ[i])
            ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ overlap(ਅਚਲ View& s, ਅਚਲ View& d)
{
    ਅਚਲ uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    ਵਾਪਸ slo < dhi && dlo < shi;
}

} // namespace

ਖਾਲੀ copyBatch(ਅਚਲ View* src, ਅਚਲ View* dst, ਪੂਰਨ n)
{
    CV_Assert(n >= 0);
    Auto_ਬਫਰ<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<Auto_ਬਫਰ<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
    {
        View s = src[i];
        ਅਚਲ View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (ਪੂਰਨ)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        ਜੇ (has_Zero_ਮਾਪ(s) || sameMapping(s, d))
            ਜਾਰੀ;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        ਤਾਰਕਿਕ need_ਆਰਜ਼ੀ = ਝੂਠ;
        ਲਈ (ਪੂਰਨ j = 0; j < n && !need_ਆਰਜ਼ੀ; j++)
            need_ਆਰਜ਼ੀ = !has_Zero_ਮਾਪ(dst[j]) && overlap(s, dst[j]);
        ਜੇ (need_ਆਰਜ਼ੀ)
        {
            size_t ਕੁੱਲ = 1;
            ਲਈ (ਪੂਰਨ k = 0; k < s.dims; k++)
                ਕੁੱਲ *= (size_t)s.size[k];
            temps.emplace_back(ਕੁੱਲ*s.esz);
            View t = s;
            t.ਅੰਕੜੇ = temps.back().ਅੰਕੜੇ();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            ਲਈ (ਪੂਰਨ k = s.dims - 1; k >= 0; k--)
            {
                t.ਕਦਮ[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        ਜੇ (buildPlan(s, d, plans[np]))
            np++;
    }
    ਲੜੀਵਾਰ_Plans(plans.ਅੰਕੜੇ(), np);
}

ਖਾਲੀ copy(ਅਚਲ View& src, ਅਚਲ View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
