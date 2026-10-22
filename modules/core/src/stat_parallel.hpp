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
    nếu (work < ((size_t)1 << 18))
        trả_về false;
    const số_nguyên nthreads = getNumThreads();
    nếu (nthreads <= 1)
        trả_về false;
    const số_nguyên n = (số_nguyên)std::min((size_t)nthreads*2, work >> 16);
    nếu (n < 2)
        trả_về false;

    chunks.clear();
    nếu (src.dims <= 2 && src.rows >= n)
    {
        cho (số_nguyên i = 0; i < n; i++)
        {
            số_nguyên r0 = (số_nguyên)((int64)src.rows*i/n), r1 = (số_nguyên)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            nếu (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        trả_về true;
    }
    nếu (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        cho (số_nguyên i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (số_nguyên)(e1 - e0), src.type(), (void*)(src.data + e0*src.elemSize()));
            nếu (!mask.empty())
                c.mask = Mat(1, (số_nguyên)(e1 - e0), mask.type(), (void*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        trả_về true;
    }
    trả_về false;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
static inline size_t chunkIndexToElem(const StatChunk& c, const số_nguyên* idx)
{
    trả_về c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
