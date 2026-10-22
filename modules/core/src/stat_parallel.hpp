// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#ifndef OPENCV_CORE_STAT_PARALLEL_HPP
#define OPENCV_CORE_STAT_PARALLEL_HPP

ನಾಮಾಕಾಶ cv {

ಸಂರಚನಾ StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ splitForParallelStat(ಧ್ರುವ Mat& src, ಧ್ರುವ Mat& mask, std::vector<StatChunk>& chunks)
{
    ಧ್ರುವ size_t total = src.total(), work = total*src.channels();
    ಯದಿ (work < ((size_t)1 << 18))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nthreads = get_ಅಂಕೆ_Threads();
    ಯದಿ (nthreads <= 1)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n = (ಪೂರ್ಣಾಙ್ಕ)std::min((size_t)nthreads*2, work >> 16);
    ಯದಿ (n < 2)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    chunks.clear();
    ಯದಿ (src.dims <= 2 && src.rows >= n)
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
        {
            ಪೂರ್ಣಾಙ್ಕ r0 = (ಪೂರ್ಣಾಙ್ಕ)((int64)src.rows*i/n), r1 = (ಪೂರ್ಣಾಙ್ಕ)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            ಯದಿ (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಯದಿ (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (ಪೂರ್ಣಾಙ್ಕ)(e1 - e0), src.type(), (ಶೂನ್ಯ*)(src.data + e0*src.elemSize()));
            ಯದಿ (!mask.empty())
                c.mask = Mat(1, (ಪೂರ್ಣಾಙ್ಕ)(e1 - e0), mask.type(), (ಶೂನ್ಯ*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t chunkIndexToElem(ಧ್ರುವ StatChunk& c, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* idx)
{
    ಪ್ರತ್ಯಾಗಮ c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
