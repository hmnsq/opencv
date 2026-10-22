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
    만약 (work < ((size_t)1 << 18))
        반환 false;
    const 정수 nthreads = getNumThreads();
    만약 (nthreads <= 1)
        반환 false;
    const 정수 n = (정수)std::min((size_t)nthreads*2, work >> 16);
    만약 (n < 2)
        반환 false;

    chunks.clear();
    만약 (src.dims <= 2 && src.rows >= n)
    {
        위해 (정수 i = 0; i < n; i++)
        {
            정수 r0 = (정수)((int64)src.rows*i/n), r1 = (정수)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            만약 (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        반환 true;
    }
    만약 (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        위해 (정수 i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (정수)(e1 - e0), src.type(), (void*)(src.data + e0*src.elemSize()));
            만약 (!mask.empty())
                c.mask = Mat(1, (정수)(e1 - e0), mask.type(), (void*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        반환 true;
    }
    반환 false;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
static inline size_t chunkIndexToElem(const StatChunk& c, const 정수* idx)
{
    반환 c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
