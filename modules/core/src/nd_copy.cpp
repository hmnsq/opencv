// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "nd_copy.hpp"
#ಅನ್ತರ್ಭಾವ "hal_replacement.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/hal.hpp"

#ಅನ್ತರ್ಭಾವ <algorithm>

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ nd {

View view_ದ(ಸ್ಥಿರಾಂಕ Mat& m)
{
    View v;
    v.ದತ್ತಾಂಶ = m.ದತ್ತಾಂಶ;
    v.dims = m.dims;
    v.esz = m.elem_ಗಾತ್ರ();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < m.dims; i++)
    {
        v.size[i] = m.size[i];
        v.ಹೆಜ್ಜೆ[i] = (ptrdiff_t)m.ಹೆಜ್ಜೆ[i];
    }
    ಹಿಂತಿರುಗಿ v;
}

View ಕ್ರಮಬದಲಾವಣೆ(ಸ್ಥಿರಾಂಕ View& v, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ಕ್ರಮ)
{
    View r;
    r.ದತ್ತಾಂಶ = v.ದತ್ತಾಂಶ;
    r.dims = v.dims;
    r.esz = v.esz;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < v.dims; i++)
    {
        ಪೂರ್ಣಾಂಕ j = ಕ್ರಮ[i];
        CV_DbgAssert(0 <= j && j < v.dims);
        r.size[i] = v.size[j];
        r.ಹೆಜ್ಜೆ[i] = v.ಹೆಜ್ಜೆ[j];
    }
    ಹಿಂತಿರುಗಿ r;
}

ಶೂನ್ಯ flip(View& v, ಪೂರ್ಣಾಂಕ axis)
{
    CV_DbgAssert(0 <= axis && axis < v.dims);
    ಆದರೆ (v.size[axis] > 0)
        v.ದತ್ತಾಂಶ += (ptrdiff_t)(v.size[axis] - 1)*v.ಹೆಜ್ಜೆ[axis];
    v.ಹೆಜ್ಜೆ[axis] = -v.ಹೆಜ್ಜೆ[axis];
}

ಶೂನ್ಯ ತುಂಡು_2(View& v, ಪೂರ್ಣಾಂಕ axis, ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3)
{
    CV_DbgAssert(0 <= axis && axis < v.dims && ಎಣಿಕೆ_3 >= 0);
    ಆದರೆ (ಎಣಿಕೆ_3 > 0)
        v.ದತ್ತಾಂಶ += (ptrdiff_t)ಆರಂಭ*v.ಹೆಜ್ಜೆ[axis];
    v.ಹೆಜ್ಜೆ[axis] *= ಹೆಜ್ಜೆ;
    v.size[axis] = ಎಣಿಕೆ_3;
}

ನಾಮಾಕಾಶ {

ಎಣಿಕೆ ಬಗೆ_3
{
    ಬಗೆ_MEMCPY,    // the innermost axis is contiguous in both views
    ಬಗೆ_FILL,      // the innermost axis is broadcast (step 0) in the source
    ಬಗೆ_STRIDED,
    ಬಗೆ_TRANSPOSE, // the two innermost axes are swapped between source and destination
    ಬಗೆ_ವಿಭಜಿಸು,     // 2..4 planes are read from one interleaved stream (e.g. NHWC -> NCHW)
    ಬಗೆ_ವಿಲೀನ      // 2..4 planes are written into one interleaved stream (e.g. NCHW -> NHWC)
};

ಎಣಿಕೆ
{
    TRANSPOSE_TILE = 64,                // in elements
    ವಿಭಜಿಸು_CHUNK = 1024,                 // in elements
    ಅಡ್ಡಸಾಲು_CHUNK_ಬೈಟುಗಳು = 64*1024,
    GATHER_ಖಂಡ_ಬೈಟುಗಳು = 8*1024,
    // costs are bytes weighted by the kernel; the copies are memory-bound,
    // so small stripes only add overhead
    PARALLEL_MIN_ವೆಚ್ಚ = 1024*1024,
    STRIPE_ವೆಚ್ಚ = 1024*1024
};

ರಚನೆ Plan
{
    ಸ್ಥಿರಾಂಕ uchar* src;
    uchar* dst;
    size_t esz;
    ಪೂರ್ಣಾಂಕ dims;
    ಪೂರ್ಣಾಂಕ ಬಗೆ;
    int64 size[MAX_VIEW_DIMS];
    ptrdiff_t sstep[MAX_VIEW_DIMS];
    ptrdiff_t dstep[MAX_VIEW_DIMS];
    TransposeFunc tfunc;

    // an item is a chunk of an innermost row, or a tile for KIND_TRANSPOSE
    int64 nrows, chunk, nchunks;
    ತಾರ್ಕಿಕ chunkMajor;
    int64 tilesA, tilesB;
    int64 nitems;
    ದ್ವಿಗುಣ ವೆಚ್ಚ_2;
};

ಸ್ಥಿರ ಶೂನ್ಯ ಅದಲುಬದಲು_Axes(Plan& p, ಪೂರ್ಣಾಂಕ i, ಪೂರ್ಣಾಂಕ j)
{
    std::swap(p.size[i], p.size[j]);
    std::swap(p.sstep[i], p.sstep[j]);
    std::swap(p.dstep[i], p.dstep[j]);
}

ಸ್ಥಿರ ಶೂನ್ಯ moveAxis(Plan& p, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ ಗಮ್ಯ)
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = k; i < ಗಮ್ಯ; i++)
        ಅದಲುಬದಲು_Axes(p, i, i + 1);
}

// OR of the base pointers and all the steps: its low bits give the common alignment.
ಸ್ಥಿರ size_t ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು(ಸ್ಥಿರಾಂಕ Plan& p)
{
    size_t addrs = (size_t)p.src | (size_t)p.dst;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < p.dims; i++)
        addrs |= (size_t)p.sstep[i] | (size_t)p.dstep[i];
    ಹಿಂತಿರುಗಿ addrs;
}

// 2..4 planes via the cv::split / cv::merge HAL kernels
ಸ್ಥಿರ ತಾರ್ಕಿಕ try_ವಿಭಜಿಸು_ವಿಲೀನ(Plan& p)
{
    ಸ್ಥಿರಾಂಕ ptrdiff_t esz = (ptrdiff_t)p.esz;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ L = p.dims - 1;
    ಆದರೆ (L < 1 || !(esz == 1 || esz == 2 || esz == 4 || esz == 8) || p.dstep[L] != esz)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಆದರೆ ((ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು(p) & (esz - 1)) != 0)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    // split: axis k holds cn planes interleaved in the source
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = L - 1; k >= 0; k--)
    {
        ಸ್ಥಿರಾಂಕ int64 cn = p.size[k];
        ಆದರೆ (cn >= 2 && cn <= 4 && p.sstep[k] == esz && p.sstep[L] == cn*esz &&
            p.dstep[k] > 0 && p.dstep[k] % esz == 0)
        {
            moveAxis(p, k, L - 1);
            p.ಬಗೆ = ಬಗೆ_ವಿಭಜಿಸು;
            ಹಿಂತಿರುಗಿ ನಿಜ;
        }
    }

    // merge: the innermost axis holds cn planes interleaved in the destination
    ಸ್ಥಿರಾಂಕ int64 cn = p.size[L];
    ಆದರೆ (cn >= 2 && cn <= 4 && p.sstep[L] > 0 && p.sstep[L] % esz == 0)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = L - 1; k >= 0; k--)
        {
            ಆದರೆ (p.sstep[k] == esz && p.dstep[k] == cn*esz)
            {
                moveAxis(p, k, L - 1);
                p.ಬಗೆ = ಬಗೆ_ವಿಲೀನ;
                ಹಿಂತಿರುಗಿ ನಿಜ;
            }
        }
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

// Returns false when there is nothing to copy.
ಸ್ಥಿರ ತಾರ್ಕಿಕ buildPlan(ಸ್ಥಿರಾಂಕ View& s, ಸ್ಥಿರಾಂಕ View& d, Plan& p)
{
    CV_CheckEQ(s.esz, d.esz, "nd::copy: the element sizes must match");
    CV_Assert(s.esz > 0);

    ಸ್ಥಿರಾಂಕ ptrdiff_t esz = (ptrdiff_t)s.esz;
    p.src = s.ದತ್ತಾಂಶ;
    p.dst = d.ದತ್ತಾಂಶ;
    p.esz = s.esz;

    // Drop size-1 axes and make the destination steps positive
    // (reversing an axis in both views keeps the mapping).
    ಪೂರ್ಣಾಂಕ n = 0;
    ದ್ವಿಗುಣ ಒಟ್ಟು = 1;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < s.dims; i++)
    {
        CV_CheckEQ(s.size[i], d.size[i], "nd::copy: the shapes must match");
        ಪೂರ್ಣಾಂಕ sz = s.size[i];
        CV_Assert(sz >= 0);
        ಆದರೆ (sz == 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಒಟ್ಟು *= sz;
        ಆದರೆ (sz == 1)
            ಮುಂದುವರಿ;
        ptrdiff_t ss = s.ಹೆಜ್ಜೆ[i], ds = d.ಹೆಜ್ಜೆ[i];
        ಆದರೆ (ds < 0)
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
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < n; i++)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = i; j > 0 && p.dstep[j-1] < p.dstep[j]; j--)
            ಅದಲುಬದಲು_Axes(p, j-1, j);

    ಪೂರ್ಣಾಂಕ m = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
    {
        ಆದರೆ (m > 0 && p.sstep[m-1] == p.sstep[i]*p.size[i] && p.dstep[m-1] == p.dstep[i]*p.size[i])
        {
            p.size[m-1] *= p.size[i];
            p.sstep[m-1] = p.sstep[i];
            p.dstep[m-1] = p.dstep[i];
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            p.size[m] = p.size[i];
            p.sstep[m] = p.sstep[i];
            p.dstep[m] = p.dstep[i];
            m++;
        }
    }
    ಆದರೆ (m == 0)
    {
        p.size[0] = 1;
        p.sstep[0] = p.dstep[0] = esz;
        m = 1;
    }
    p.dims = m;

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ L = m - 1;
    p.tfunc = ಶೂನ್ಯಸೂಚಕ;
    ಆದರೆ (p.sstep[L] == esz && p.dstep[L] == esz)
        p.ಬಗೆ = ಬಗೆ_MEMCPY;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p.sstep[L] == 0)
        p.ಬಗೆ = ಬಗೆ_FILL;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!try_ವಿಭಜಿಸು_ವಿಲೀನ(p))
    {
        p.ಬಗೆ = ಬಗೆ_STRIDED;
        // An outer axis contiguous in the source makes it a batched 2D transpose.
        ಆದರೆ (m >= 2 && p.dstep[L] == esz && p.sstep[L] > 0)
        {
            ಪೂರ್ಣಾಂಕ k = L - 1;
            ಆಗ (k >= 0 && p.sstep[k] != esz)
                k--;
            TransposeFunc f = getTransposeFunc(s.esz);
            // the kernels access the elements as uchar, ushort or int based types
            ಸ್ಥಿರಾಂಕ size_t align = std::min(s.esz & (0 - s.esz), (size_t)4);
            // a short axis cannot fill SIMD registers; the gather is faster then
            ಆದರೆ (k >= 0 && f && p.size[k] >= 8 && (ವಿಳಾಸ_ಬಿಟ್ಟುಗಳು(p) & (align - 1)) == 0)
            {
                moveAxis(p, k, L - 1);
                p.ಬಗೆ = ಬಗೆ_TRANSPOSE;
                p.tfunc = f;
            }
        }
    }

    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ ಬೈಟುಗಳು = ಒಟ್ಟು*(ದ್ವಿಗುಣ)esz;
    ಆದರೆ (p.ಬಗೆ == ಬಗೆ_ವಿಭಜಿಸು || p.ಬಗೆ == ಬಗೆ_ವಿಲೀನ)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ X = p.ಬಗೆ == ಬಗೆ_ವಿಭಜಿಸು ? L : L - 1;
        int64 nouter = 1;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.nrows = nouter;
        p.chunk = ವಿಭಜಿಸು_CHUNK;
        p.nchunks = (p.size[X] + p.chunk - 1)/p.chunk;
        p.chunkMajor = ಸುಳ್ಳು;
        p.tilesA = p.tilesB = 1;
        p.nitems = nouter*p.nchunks;
        p.ವೆಚ್ಚ_2 = ಬೈಟುಗಳು*2;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p.ಬಗೆ == ಬಗೆ_TRANSPOSE)
    {
        int64 nouter = 1;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < L - 1; i++)
            nouter *= p.size[i];
        p.tilesA = (p.size[L-1] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.tilesB = (p.size[L] + TRANSPOSE_TILE - 1)/TRANSPOSE_TILE;
        p.nrows = nouter;
        p.chunk = p.nchunks = 1;
        p.chunkMajor = ಸುಳ್ಳು;
        p.nitems = nouter*p.tilesA*p.tilesB;
        p.ವೆಚ್ಚ_2 = ಬೈಟುಗಳು*3;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        int64 nrows = 1;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < L; i++)
            nrows *= p.size[i];
        ಸ್ಥಿರಾಂಕ int64 ಅಡ್ಡಸಾಲು_Len = p.size[L];
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ ಅಡ್ಡಸಾಲು_ಬೈಟುಗಳು = (ದ್ವಿಗುಣ)ಅಡ್ಡಸಾಲು_Len*esz;
        ಸ್ಥಿರಾಂಕ ptrdiff_t sstepL = std::ನಿರಪೇಕ್ಷ(p.sstep[L]);
        p.nrows = nrows;
        p.nchunks = 1;
        p.chunk = ಅಡ್ಡಸಾಲು_Len;
        p.chunkMajor = ಸುಳ್ಳು;
        ಆದರೆ (p.ಬಗೆ == ಬಗೆ_STRIDED && L > 0 && nrows <= 64 && std::ನಿರಪೇಕ್ಷ(p.sstep[L-1]) < sstepL &&
            (ದ್ವಿಗುಣ)ಅಡ್ಡಸಾಲು_Len*sstepL > 2.0*GATHER_ಖಂಡ_ಬೈಟುಗಳು)
        {
            // Rows reading interleaved source data (NHWC -> NCHW with a small C):
            // go chunk by chunk so that the rows share the cached source lines.
            p.chunk = std::max((int64)16, (int64)(GATHER_ಖಂಡ_ಬೈಟುಗಳು/sstepL));
            p.nchunks = (ಅಡ್ಡಸಾಲು_Len + p.chunk - 1)/p.chunk;
            p.chunkMajor = ನಿಜ;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nrows < 64 && ಅಡ್ಡಸಾಲು_ಬೈಟುಗಳು >= 2.0*ಅಡ್ಡಸಾಲು_CHUNK_ಬೈಟುಗಳು)
        {
            // few long rows: split them to have something to parallelize
            int64 nch = std::min((int64)(ಅಡ್ಡಸಾಲು_ಬೈಟುಗಳು/ಅಡ್ಡಸಾಲು_CHUNK_ಬೈಟುಗಳು), (int64)1024);
            p.chunk = (ಅಡ್ಡಸಾಲು_Len + nch - 1)/nch;
            p.nchunks = (ಅಡ್ಡಸಾಲು_Len + p.chunk - 1)/p.chunk;
        }
        p.tilesA = p.tilesB = 1;
        p.nitems = nrows*p.nchunks;
        p.ವೆಚ್ಚ_2 = p.ಬಗೆ == ಬಗೆ_MEMCPY ? ಬೈಟುಗಳು : ಬೈಟುಗಳು*4;
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isAligned(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* s, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* d, ptrdiff_t ss, ptrdiff_t ds)
{
    ಹಿಂತಿರುಗಿ (((size_t)s | (size_t)d | (size_t)ss | (size_t)ds) & (alignof(T) - 1)) == 0;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ copyStrided_(ಸ್ಥಿರಾಂಕ uchar* s, ptrdiff_t ss, uchar* d, ptrdiff_t ds, int64 n)
{
    int64 i = 0;
    ಗಾಗಿ (; i + 4 <= n; i += 4)
    {
        T t0 = *(ಸ್ಥಿರಾಂಕ T*)s, t1 = *(ಸ್ಥಿರಾಂಕ T*)(s + ss);
        T t2 = *(ಸ್ಥಿರಾಂಕ T*)(s + ss*2), t3 = *(ಸ್ಥಿರಾಂಕ T*)(s + ss*3);
        *(T*)d = t0; *(T*)(d + ds) = t1;
        *(T*)(d + ds*2) = t2; *(T*)(d + ds*3) = t3;
        s += ss*4;
        d += ds*4;
    }
    ಗಾಗಿ (; i < n; i++, s += ss, d += ds)
        *(T*)d = *(ಸ್ಥಿರಾಂಕ T*)s;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ fill_(ಸ್ಥಿರಾಂಕ uchar* s, uchar* d, ptrdiff_t ds, int64 n)
{
    ಸ್ಥಿರಾಂಕ T v = *(ಸ್ಥಿರಾಂಕ T*)s;
    ಗಾಗಿ (int64 i = 0; i < n; i++, d += ds)
        *(T*)d = v;
}

ರಚನೆ Elem16 { uint64_t a, b; };

// KIND_FILL and KIND_STRIDED
ಸ್ಥಿರ ಶೂನ್ಯ copy_ಅಡ್ಡಸಾಲು(ಸ್ಥಿರಾಂಕ Plan& p, ಸ್ಥಿರಾಂಕ uchar* s, uchar* d, int64 n)
{
    ಸ್ಥಿರಾಂಕ size_t esz = p.esz;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ L = p.dims - 1;
    ಸ್ಥಿರಾಂಕ ptrdiff_t ss = p.sstep[L], ds = p.dstep[L];

    ಆದರೆ (p.ಬಗೆ == ಬಗೆ_FILL)
    {
        ಆದರೆ (esz == 1 && ds == 1)
            memset(d, *s, (size_t)n);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 2 && isAligned<uint16_t>(s, d, 0, ds))
            fill_<uint16_t>(s, d, ds, n);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 4 && isAligned<uint32_t>(s, d, 0, ds))
            fill_<uint32_t>(s, d, ds, n);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 8 && isAligned<uint64_t>(s, d, 0, ds))
            fill_<uint64_t>(s, d, ds, n);
        ಇಲ್ಲದಿದ್ದರೆ
            ಗಾಗಿ (int64 i = 0; i < n; i++, d += ds)
                ಸ್ಮರಣೆ_ನಕಲು(d, s, esz);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (esz == 1)
        copyStrided_<uchar>(s, ss, d, ds, n);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 2 && isAligned<uint16_t>(s, d, ss, ds))
        copyStrided_<uint16_t>(s, ss, d, ds, n);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 4 && isAligned<uint32_t>(s, d, ss, ds))
        copyStrided_<uint32_t>(s, ss, d, ds, n);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 8 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<uint64_t>(s, ss, d, ds, n);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 16 && isAligned<uint64_t>(s, d, ss, ds))
        copyStrided_<Elem16>(s, ss, d, ds, n);
    ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ (int64 i = 0; i < n; i++, s += ss, d += ds)
            ಸ್ಮರಣೆ_ನಕಲು(d, s, esz);
}

// Offsets of the given flat index over the first `naxes` axes.
ಸ್ಥಿರ ಶೂನ್ಯ decode(ಸ್ಥಿರಾಂಕ Plan& p, ಪೂರ್ಣಾಂಕ naxes, int64 idx, int64* counters,
                   ಸ್ಥಿರಾಂಕ uchar*& s, uchar*& d)
{
    s = p.src;
    d = p.dst;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = naxes - 1; k >= 0; k--)
    {
        int64 q = idx / p.size[k];
        int64 i = idx - q*p.size[k];
        ಆದರೆ (counters)
            counters[k] = i;
        s += i*p.sstep[k];
        d += i*p.dstep[k];
        idx = q;
    }
}

ಮಾದರಿ<ತಾರ್ಕಿಕ MEMCPY> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ copy_ಅಡ್ಡಸಾಲು_T(ಸ್ಥಿರಾಂಕ Plan& p, ಸ್ಥಿರಾಂಕ uchar* s, uchar* d, int64 n)
{
    ಆದರೆ (MEMCPY)
        ಸ್ಮರಣೆ_ನಕಲು(d, s, (size_t)n*p.esz);
    ಇಲ್ಲದಿದ್ದರೆ
        copy_ಅಡ್ಡಸಾಲು(p, s, d, n);
}

ಮಾದರಿ<ತಾರ್ಕಿಕ MEMCPY> ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_ಅಡ್ಡಸಾಲುಗಳು(ಸ್ಥಿರಾಂಕ Plan& p, int64 i0, int64 i1)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ L = p.dims - 1;
    ಸ್ಥಿರಾಂಕ int64 nch = p.nchunks, ಅಡ್ಡಸಾಲು_Len = p.size[L];
    ಸ್ಥಿರಾಂಕ ptrdiff_t sstepL = p.sstep[L], dstepL = p.dstep[L];
    ಸ್ಥಿರಾಂಕ uchar* s;
    uchar* d;

    ಆದರೆ (p.chunkMajor)
    {
        ಗಾಗಿ (int64 it = i0; it < i1; it++)
        {
            int64 ch = it / p.nrows, ಅಡ್ಡಸಾಲು = it - ch*p.nrows;
            int64 e0 = ch*p.chunk, e1 = std::min(e0 + p.chunk, ಅಡ್ಡಸಾಲು_Len);
            decode(p, L, ಅಡ್ಡಸಾಲು, ಶೂನ್ಯಸೂಚಕ, s, d);
            copy_ಅಡ್ಡಸಾಲು_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        }
        ಹಿಂತಿರುಗಿ;
    }

    int64 counters[MAX_VIEW_DIMS];
    int64 ch = i0 % nch;
    decode(p, L, i0 / nch, counters, s, d);

    ಆದರೆ (nch == 1 && L == 1)
    {
        ಸ್ಥಿರಾಂಕ ptrdiff_t ss = p.sstep[0], ds = p.dstep[0];
        ಗಾಗಿ (int64 it = i0; it < i1; it++, s += ss, d += ds)
            copy_ಅಡ್ಡಸಾಲು_T<MEMCPY>(p, s, d, ಅಡ್ಡಸಾಲು_Len);
        ಹಿಂತಿರುಗಿ;
    }

    ಗಾಗಿ (int64 it = i0; it < i1; )
    {
        int64 ch_ಕೊನೆ = std::min(nch, ch + (i1 - it));
        int64 e0 = ch*p.chunk, e1 = std::min(ch_ಕೊನೆ*p.chunk, ಅಡ್ಡಸಾಲು_Len);
        copy_ಅಡ್ಡಸಾಲು_T<MEMCPY>(p, s + e0*sstepL, d + e0*dstepL, e1 - e0);
        it += ch_ಕೊನೆ - ch;
        ಆದರೆ (ch_ಕೊನೆ < nch)
            ನಿಲ್ಲಿಸು;
        ch = 0;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = L - 1; k >= 0; k--)
        {
            s += p.sstep[k];
            d += p.dstep[k];
            ಆದರೆ (++counters[k] < p.size[k])
                ನಿಲ್ಲಿಸು;
            s -= p.size[k]*p.sstep[k];
            d -= p.size[k]*p.dstep[k];
            counters[k] = 0;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Transpose(ಸ್ಥಿರಾಂಕ Plan& p, int64 i0, int64 i1)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ A = p.dims - 2, B = p.dims - 1;
    ಸ್ಥಿರಾಂಕ int64 ntiles = p.tilesA*p.tilesB;
    ಗಾಗಿ (int64 it = i0; it < i1; it++)
    {
        int64 o = it / ntiles, t = it - o*ntiles;
        int64 ta = t / p.tilesB, tb = t - ta*p.tilesB;
        ಸ್ಥಿರಾಂಕ uchar* s;
        uchar* d;
        decode(p, A, o, ಶೂನ್ಯಸೂಚಕ, s, d);
        int64 a0 = ta*TRANSPOSE_TILE, b0 = tb*TRANSPOSE_TILE;
        ಪೂರ್ಣಾಂಕ wa = (ಪೂರ್ಣಾಂಕ)std::min((int64)TRANSPOSE_TILE, p.size[A] - a0);
        ಪೂರ್ಣಾಂಕ hb = (ಪೂರ್ಣಾಂಕ)std::min((int64)TRANSPOSE_TILE, p.size[B] - b0);
        s += a0*p.sstep[A] + b0*p.sstep[B];
        d += a0*p.dstep[A] + b0*p.dstep[B];
        ಆದರೆ (cv_hal_transpose2d(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], wa, hb, (ಪೂರ್ಣಾಂಕ)p.esz) == CV_HAL_ERROR_OK)
            ಮುಂದುವರಿ;
        p.tfunc(s, (size_t)p.sstep[B], d, (size_t)p.dstep[A], ಗಾತ್ರ_2(wa, hb));
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_ವಿಭಜಿಸು_ವಿಲೀನ(ಸ್ಥಿರಾಂಕ Plan& p, int64 i0, int64 i1)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ L = p.dims - 1;
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ is_ವಿಭಜಿಸು = p.ಬಗೆ == ಬಗೆ_ವಿಭಜಿಸು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ C = is_ವಿಭಜಿಸು ? L - 1 : L;    // the plane axis
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ X = is_ವಿಭಜಿಸು ? L : L - 1;    // the long axis
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = (ಪೂರ್ಣಾಂಕ)p.size[C];
    ಸ್ಥಿರಾಂಕ size_t esz = p.esz;
    ಗಾಗಿ (int64 it = i0; it < i1; it++)
    {
        int64 o = it / p.nchunks, ch = it - o*p.nchunks;
        ಸ್ಥಿರಾಂಕ uchar* s;
        uchar* d;
        decode(p, L - 1, o, ಶೂನ್ಯಸೂಚಕ, s, d);
        ಸ್ಥಿರಾಂಕ int64 e0 = ch*p.chunk;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ len = (ಪೂರ್ಣಾಂಕ)std::min(p.chunk, p.size[X] - e0);
        s += e0*p.sstep[X];
        d += e0*p.dstep[X];
        ಆದರೆ (is_ವಿಭಜಿಸು)
        {
            uchar* dptr[4];
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                dptr[c] = d + c*p.dstep[C];
            ಆದರೆ (esz == 1) hal::split8u(s, dptr, len, cn);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 2) hal::split16u((ಸ್ಥಿರಾಂಕ ushort*)s, (ushort**)dptr, len, cn);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 4) hal::split32s((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)s, (ಪೂರ್ಣಾಂಕ**)dptr, len, cn);
            ಇಲ್ಲದಿದ್ದರೆ hal::split64s((ಸ್ಥಿರಾಂಕ int64*)s, (int64**)dptr, len, cn);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಸ್ಥಿರಾಂಕ uchar* sptr[4];
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < cn; c++)
                sptr[c] = s + c*p.sstep[C];
            ಆದರೆ (esz == 1) hal::merge8u(sptr, d, len, cn);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 2) hal::merge16u((ಸ್ಥಿರಾಂಕ ushort**)sptr, (ushort*)d, len, cn);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz == 4) hal::merge32s((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ**)sptr, (ಪೂರ್ಣಾಂಕ*)d, len, cn);
            ಇಲ್ಲದಿದ್ದರೆ hal::merge64s((ಸ್ಥಿರಾಂಕ int64**)sptr, (int64*)d, len, cn);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Plan(ಸ್ಥಿರಾಂಕ Plan& p, int64 i0, int64 i1)
{
    ಆದರೆ (p.ಬಗೆ == ಬಗೆ_ವಿಭಜಿಸು || p.ಬಗೆ == ಬಗೆ_ವಿಲೀನ)
        ಸತತ_ವಿಭಜಿಸು_ವಿಲೀನ(p, i0, i1);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p.ಬಗೆ == ಬಗೆ_TRANSPOSE)
        ಸತತ_Transpose(p, i0, i1);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p.ಬಗೆ == ಬಗೆ_MEMCPY)
        ಸತತ_ಅಡ್ಡಸಾಲುಗಳು<ನಿಜ>(p, i0, i1);
    ಇಲ್ಲದಿದ್ದರೆ
        ಸತತ_ಅಡ್ಡಸಾಲುಗಳು<ಸುಳ್ಳು>(p, i0, i1);
}

// Plans over the same rows (concat inputs, split outputs) run in lockstep, block by block,
// so that the shared destination (or source) is accessed sequentially.
ಸ್ಥಿರ ತಾರ್ಕಿಕ can_ಸತತ_ಒಳ_Lockstep(ಸ್ಥಿರಾಂಕ Plan* plans, size_t np)
{
    ಆದರೆ (np < 2)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಸ್ಥಿರಾಂಕ Plan& p0 = plans[0];
    ಗಾಗಿ (size_t k = 0; k < np; k++)
    {
        ಸ್ಥಿರಾಂಕ Plan& p = plans[k];
        ಆದರೆ (p.ಬಗೆ == ಬಗೆ_TRANSPOSE || p.ಬಗೆ == ಬಗೆ_ವಿಭಜಿಸು || p.ಬಗೆ == ಬಗೆ_ವಿಲೀನ ||
            p.chunkMajor || p.nchunks != 1 || p.dims != p0.dims)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < p.dims - 1; i++)
            ಆದರೆ (p.size[i] != p0.size[i])
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_Plans(ಸ್ಥಿರಾಂಕ Plan* plans, size_t np)
{
    ಆದರೆ (np == 0)
        ಹಿಂತಿರುಗಿ;
    Auto_ಬಫರ್<int64, 16> ofs(np + 1);
    ofs[0] = 0;
    ದ್ವಿಗುಣ ವೆಚ್ಚ_2 = 0, ಅಡ್ಡಸಾಲು_ಬೈಟುಗಳು = 0;
    ಗಾಗಿ (size_t k = 0; k < np; k++)
    {
        ofs[k+1] = ofs[k] + plans[k].nitems;
        ವೆಚ್ಚ_2 += plans[k].ವೆಚ್ಚ_2;
        ಅಡ್ಡಸಾಲು_ಬೈಟುಗಳು += (ದ್ವಿಗುಣ)plans[k].size[plans[k].dims - 1]*plans[k].esz;
    }

    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ lockstep = can_ಸತತ_ಒಳ_Lockstep(plans, np);
    ಸ್ಥಿರಾಂಕ int64 ಒಟ್ಟು = lockstep ? plans[0].nrows : ofs[np];
    ಸ್ಥಿರಾಂಕ int64 ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು_2 = std::max((int64)1, (int64)(16*1024/std::max(ಅಡ್ಡಸಾಲು_ಬೈಟುಗಳು, 1.)));

    ಸ್ವಯಂಚಾಲಿತ body = [&](int64 a, int64 b)
    {
        ಆದರೆ (lockstep)
        {
            ಗಾಗಿ (int64 r = a; r < b; r += ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು_2)
            {
                int64 re = std::min(b, r + ಖಂಡ_ಅಡ್ಡಸಾಲುಗಳು_2);
                ಗಾಗಿ (size_t k = 0; k < np; k++)
                    ಸತತ_Plan(plans[k], r, re);
            }
            ಹಿಂತಿರುಗಿ;
        }
        size_t k = std::upper_bound(ofs.ದತ್ತಾಂಶ(), ofs.ದತ್ತಾಂಶ() + np + 1, a) - ofs.ದತ್ತಾಂಶ() - 1;
        ಆಗ (a < b)
        {
            int64 e = std::min(b, ofs[k+1]);
            ಸತತ_Plan(plans[k], a - ofs[k], e - ofs[k]);
            a = e;
            k++;
        }
    };

    ಆದರೆ (ವೆಚ್ಚ_2 < PARALLEL_MIN_ವೆಚ್ಚ || ಒಟ್ಟು < 2 || get_ಅಂಕೆ_ಎಳೆಗಳು() <= 1)
    {
        body(0, ಒಟ್ಟು);
        ಹಿಂತಿರುಗಿ;
    }

    // grouped so that the range fits into int
    ಸ್ಥಿರಾಂಕ int64 ngroups = std::min(ಒಟ್ಟು, (int64)1 << 20);
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ nstripes = std::min((ದ್ವಿಗುಣ)ngroups, std::max(2.0, ವೆಚ್ಚ_2/STRIPE_ವೆಚ್ಚ));
    parallel_for_(Range(0, (ಪೂರ್ಣಾಂಕ)ngroups), [&](ಸ್ಥಿರಾಂಕ Range& r)
    {
        body(r.ಆರಂಭ*ಒಟ್ಟು/ngroups, r.end*ಒಟ್ಟು/ngroups);
    }, nstripes);
}

// Address range [lo, hi) touched by the view.
ಸ್ಥಿರ ಶೂನ್ಯ viewRange(ಸ್ಥಿರಾಂಕ View& v, ಸ್ಥಿರಾಂಕ uchar*& lo, ಸ್ಥಿರಾಂಕ uchar*& hi)
{
    ptrdiff_t a = 0, b = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < v.dims; i++)
    {
        ptrdiff_t ext = (ptrdiff_t)(v.size[i] - 1)*v.ಹೆಜ್ಜೆ[i];
        ಆದರೆ (ext < 0) a += ext; ಇಲ್ಲದಿದ್ದರೆ b += ext;
    }
    lo = v.ದತ್ತಾಂಶ + a;
    hi = v.ದತ್ತಾಂಶ + b + v.esz;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ has_Zero_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ View& v)
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < v.dims; i++)
        ಆದರೆ (v.size[i] == 0)
            ಹಿಂತಿರುಗಿ ನಿಜ;
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ sameMapping(ಸ್ಥಿರಾಂಕ View& s, ಸ್ಥಿರಾಂಕ View& d)
{
    ಆದರೆ (s.ದತ್ತಾಂಶ != d.ದತ್ತಾಂಶ)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < s.dims; i++)
        ಆದರೆ (s.size[i] > 1 && s.ಹೆಜ್ಜೆ[i] != d.ಹೆಜ್ಜೆ[i])
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ overlap(ಸ್ಥಿರಾಂಕ View& s, ಸ್ಥಿರಾಂಕ View& d)
{
    ಸ್ಥಿರಾಂಕ uchar *slo, *shi, *dlo, *dhi;
    viewRange(s, slo, shi);
    viewRange(d, dlo, dhi);
    ಹಿಂತಿರುಗಿ slo < dhi && dlo < shi;
}

} // namespace

ಶೂನ್ಯ copyBatch(ಸ್ಥಿರಾಂಕ View* src, ಸ್ಥಿರಾಂಕ View* dst, ಪೂರ್ಣಾಂಕ n)
{
    CV_Assert(n >= 0);
    Auto_ಬಫರ್<Plan, 4> plans(n);
    size_t np = 0;
    std::vector<Auto_ಬಫರ್<uchar> > temps;
    temps.reserve(n);  // the views below point into the buffers, so they must not move

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
    {
        View s = src[i];
        ಸ್ಥಿರಾಂಕ View& d = dst[i];
        CV_CheckEQ(s.dims, d.dims, "nd::copy: the number of dimensions must match");
        CV_CheckLE(s.dims, (ಪೂರ್ಣಾಂಕ)MAX_VIEW_DIMS, "nd::copy: too many dimensions");
        ಆದರೆ (has_Zero_ಗಾತ್ರ(s) || sameMapping(s, d))
            ಮುಂದುವರಿ;
        // All the copies run after this loop, so a source that overlaps any of the
        // destinations is saved first.
        ತಾರ್ಕಿಕ need_ತಾತ್ಕಾಲಿಕ = ಸುಳ್ಳು;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < n && !need_ತಾತ್ಕಾಲಿಕ; j++)
            need_ತಾತ್ಕಾಲಿಕ = !has_Zero_ಗಾತ್ರ(dst[j]) && overlap(s, dst[j]);
        ಆದರೆ (need_ತಾತ್ಕಾಲಿಕ)
        {
            size_t ಒಟ್ಟು = 1;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < s.dims; k++)
                ಒಟ್ಟು *= (size_t)s.size[k];
            temps.emplace_back(ಒಟ್ಟು*s.esz);
            View t = s;
            t.ದತ್ತಾಂಶ = temps.back().ದತ್ತಾಂಶ();
            ptrdiff_t st = (ptrdiff_t)s.esz;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = s.dims - 1; k >= 0; k--)
            {
                t.ಹೆಜ್ಜೆ[k] = st;
                st *= s.size[k];
            }
            copy(s, t);
            s = t;
        }
        ಆದರೆ (buildPlan(s, d, plans[np]))
            np++;
    }
    ಸತತ_Plans(plans.ದತ್ತಾಂಶ(), np);
}

ಶೂನ್ಯ copy(ಸ್ಥಿರಾಂಕ View& src, ಸ್ಥಿರಾಂಕ View& dst)
{
    copyBatch(&src, &dst, 1);
}

}} // namespace cv::nd
