// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_STAT_PARALLEL_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_STAT_PARALLEL_HPP

ਨਾਮਾਕਾਸ਼ cv {

ਬਣਤਰ StatChunk
{
    Mat src, ਨਕਾਬ;
    size_t ਪਹਿਲਾ_Elem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਵੰਡੋ_For_Parallel_Stat(ਅਚਲ Mat& src, ਅਚਲ Mat& ਨਕਾਬ, std::vector<StatChunk>& chunks)
{
    ਅਚਲ size_t ਕੁੱਲ = src.ਕੁੱਲ(), work = ਕੁੱਲ*src.channels();
    ਜੇ (work < ((size_t)1 << 18))
        ਵਾਪਸ ਝੂਠ;
    ਅਚਲ ਪੂਰਨ nthreads = get_ਅੰਕ_ਧਾਗੇ();
    ਜੇ (nthreads <= 1)
        ਵਾਪਸ ਝੂਠ;
    ਅਚਲ ਪੂਰਨ n = (ਪੂਰਨ)std::min((size_t)nthreads*2, work >> 16);
    ਜੇ (n < 2)
        ਵਾਪਸ ਝੂਠ;

    chunks.clear();
    ਜੇ (src.dims <= 2 && src.ਪੰਕਤੀਆਂ >= n)
    {
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
        {
            ਪੂਰਨ r0 = (ਪੂਰਨ)((int64)src.ਪੰਕਤੀਆਂ*i/n), r1 = (ਪੂਰਨ)((int64)src.ਪੰਕਤੀਆਂ*(i + 1)/n);
            StatChunk c;
            c.src = src.ਪੰਕਤੀ_Range_2(r0, r1);
            ਜੇ (!ਨਕਾਬ.empty())
                c.ਨਕਾਬ = ਨਕਾਬ.ਪੰਕਤੀ_Range_2(r0, r1);
            c.ਪਹਿਲਾ_Elem = (size_t)r0*src.ਖਾਨੇ_ਗਿਣਤੀ;
            chunks.push_back(c);
        }
        ਵਾਪਸ ਸੱਚ;
    }
    ਜੇ (src.isContinuous() && (ਨਕਾਬ.empty() || ਨਕਾਬ.isContinuous()) && ਕੁੱਲ <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
    {
        ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
        {
            size_t e0 = ਕੁੱਲ*i/n, e1 = ਕੁੱਲ*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (ਪੂਰਨ)(e1 - e0), src.type(), (ਖਾਲੀ*)(src.ਅੰਕੜੇ + e0*src.elem_ਮਾਪ()));
            ਜੇ (!ਨਕਾਬ.empty())
                c.ਨਕਾਬ = Mat(1, (ਪੂਰਨ)(e1 - e0), ਨਕਾਬ.type(), (ਖਾਲੀ*)(ਨਕਾਬ.ਅੰਕੜੇ + e0*ਨਕਾਬ.elem_ਮਾਪ()));
            c.ਪਹਿਲਾ_Elem = e0;
            chunks.push_back(c);
        }
        ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ਸਥਿਰ ਸਤਰਅੰਦਰ size_t chunk_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Elem(ਅਚਲ StatChunk& c, ਅਚਲ ਪੂਰਨ* idx)
{
    ਵਾਪਸ c.ਪਹਿਲਾ_Elem + (size_t)idx[0]*c.src.ਖਾਨੇ_ਗਿਣਤੀ + idx[1];
}

} // namespace cv

#ਜੇ_ਅੰਤ
