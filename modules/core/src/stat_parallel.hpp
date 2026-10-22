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

sa_nracanaa StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
static anta_hstha bool splitForParallelStat(const Mat& src, const Mat& mask, std::vector<StatChunk>& chunks)
{
    const size_t total = src.total(), work = total*src.channels();
    အကယ်၍ (work < ((size_t)1 << 18))
        ပြန်ပေး false;
    const ကိန်းပြည့် nthreads = getNumThreads();
    အကယ်၍ (nthreads <= 1)
        ပြန်ပေး false;
    const ကိန်းပြည့် n = (ကိန်းပြည့်)std::min((size_t)nthreads*2, work >> 16);
    အကယ်၍ (n < 2)
        ပြန်ပေး false;

    chunks.clear();
    အကယ်၍ (src.dims <= 2 && src.rows >= n)
    {
        အတွက် (ကိန်းပြည့် i = 0; i < n; i++)
        {
            ကိန်းပြည့် r0 = (ကိန်းပြည့်)((int64)src.rows*i/n), r1 = (ကိန်းပြည့်)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            အကယ်၍ (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        ပြန်ပေး true;
    }
    အကယ်၍ (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        အတွက် (ကိန်းပြည့် i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (ကိန်းပြည့်)(e1 - e0), src.type(), (void*)(src.data + e0*src.elemSize()));
            အကယ်၍ (!mask.empty())
                c.mask = Mat(1, (ကိန်းပြည့်)(e1 - e0), mask.type(), (void*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        ပြန်ပေး true;
    }
    ပြန်ပေး false;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
static anta_hstha size_t chunkIndexToElem(const StatChunk& c, const ကိန်းပြည့်* idx)
{
    ပြန်ပေး c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
