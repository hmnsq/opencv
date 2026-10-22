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

namespace cv {

struct StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
static inline bool splitForParallelStat(const Mat& src, const Mat& mask, std::vector<StatChunk>& chunks)
{
    const size_t total = src.total(), work = total*src.channels();
    もし (work < ((size_t)1 << 18))
        戻る false;
    const 整数 nthreads = getNumThreads();
    もし (nthreads <= 1)
        戻る false;
    const 整数 n = (整数)std::min((size_t)nthreads*2, work >> 16);
    もし (n < 2)
        戻る false;

    chunks.clear();
    もし (src.dims <= 2 && src.rows >= n)
    {
        ために (整数 i = 0; i < n; i++)
        {
            整数 r0 = (整数)((int64)src.rows*i/n), r1 = (整数)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            もし (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        戻る true;
    }
    もし (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        ために (整数 i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (整数)(e1 - e0), src.type(), (void*)(src.data + e0*src.elemSize()));
            もし (!mask.empty())
                c.mask = Mat(1, (整数)(e1 - e0), mask.type(), (void*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        戻る true;
    }
    戻る false;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
static inline size_t chunkIndexToElem(const StatChunk& c, const 整数* idx)
{
    戻る c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
