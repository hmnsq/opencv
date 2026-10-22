// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_STAT_PARALLEL_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_STAT_PARALLEL_HPP

ನಾಮಾಕಾಶ cv {

ರಚನೆ StatChunk
{
    Mat src, ಮುಖವಾಡ;
    size_t ಮೊದಲ_Elem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ ವಿಭಜಿಸು_For_Parallel_Stat(ಸ್ಥಿರಾಂಕ Mat& src, ಸ್ಥಿರಾಂಕ Mat& ಮುಖವಾಡ, std::vector<StatChunk>& chunks)
{
    ಸ್ಥಿರಾಂಕ size_t ಒಟ್ಟು = src.ಒಟ್ಟು(), work = ಒಟ್ಟು*src.channels();
    ಆದರೆ (work < ((size_t)1 << 18))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nthreads = get_ಅಂಕೆ_ಎಳೆಗಳು();
    ಆದರೆ (nthreads <= 1)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = (ಪೂರ್ಣಾಂಕ)std::min((size_t)nthreads*2, work >> 16);
    ಆದರೆ (n < 2)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    chunks.clear();
    ಆದರೆ (src.dims <= 2 && src.ಅಡ್ಡಸಾಲುಗಳು >= n)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
        {
            ಪೂರ್ಣಾಂಕ r0 = (ಪೂರ್ಣಾಂಕ)((int64)src.ಅಡ್ಡಸಾಲುಗಳು*i/n), r1 = (ಪೂರ್ಣಾಂಕ)((int64)src.ಅಡ್ಡಸಾಲುಗಳು*(i + 1)/n);
            StatChunk c;
            c.src = src.ಅಡ್ಡಸಾಲು_Range_2(r0, r1);
            ಆದರೆ (!ಮುಖವಾಡ.empty())
                c.ಮುಖವಾಡ = ಮುಖವಾಡ.ಅಡ್ಡಸಾಲು_Range_2(r0, r1);
            c.ಮೊದಲ_Elem = (size_t)r0*src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
            chunks.push_back(c);
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಆದರೆ (src.isContinuous() && (ಮುಖವಾಡ.empty() || ಮುಖವಾಡ.isContinuous()) && ಒಟ್ಟು <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
        {
            size_t e0 = ಒಟ್ಟು*i/n, e1 = ಒಟ್ಟು*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (ಪೂರ್ಣಾಂಕ)(e1 - e0), src.type(), (ಶೂನ್ಯ*)(src.ದತ್ತಾಂಶ + e0*src.elem_ಗಾತ್ರ()));
            ಆದರೆ (!ಮುಖವಾಡ.empty())
                c.ಮುಖವಾಡ = Mat(1, (ಪೂರ್ಣಾಂಕ)(e1 - e0), ಮುಖವಾಡ.type(), (ಶೂನ್ಯ*)(ಮುಖವಾಡ.ದತ್ತಾಂಶ + e0*ಮುಖವಾಡ.elem_ಗಾತ್ರ()));
            c.ಮೊದಲ_Elem = e0;
            chunks.push_back(c);
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ಸ್ಥಿರ ಸಾಲೊಳಗೆ size_t chunk_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Elem(ಸ್ಥಿರಾಂಕ StatChunk& c, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* idx)
{
    ಹಿಂತಿರುಗಿ c.ಮೊದಲ_Elem + (size_t)idx[0]*c.src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + idx[1];
}

} // namespace cv

#ಆದರೆ_ಅಂತ್ಯ
