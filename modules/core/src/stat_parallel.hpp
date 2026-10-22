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

নামাকাশ cv {

সংরচনা StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
স্থির অন্তঃস্থ তার্কিক splitForParallelStat(ধ্রুৱ Mat& src, ধ্রুৱ Mat& mask, std::vector<StatChunk>& chunks)
{
    ধ্রুৱ size_t total = src.total(), work = total*src.channels();
    যদি (work < ((size_t)1 << 18))
        ঘূৰাই মিছা;
    ধ্রুৱ পূর্ণাঙ্ক nthreads = get_অঙ্ক_Threads();
    যদি (nthreads <= 1)
        ঘূৰাই মিছা;
    ধ্রুৱ পূর্ণাঙ্ক n = (পূর্ণাঙ্ক)std::min((size_t)nthreads*2, work >> 16);
    যদি (n < 2)
        ঘূৰাই মিছা;

    chunks.clear();
    যদি (src.dims <= 2 && src.rows >= n)
    {
        বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
        {
            পূর্ণাঙ্ক r0 = (পূর্ণাঙ্ক)((int64)src.rows*i/n), r1 = (পূর্ণাঙ্ক)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            যদি (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        ঘূৰাই সঁচা;
    }
    যদি (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (পূর্ণাঙ্ক)(e1 - e0), src.type(), (শূন্য*)(src.data + e0*src.elemSize()));
            যদি (!mask.empty())
                c.mask = Mat(1, (পূর্ণাঙ্ক)(e1 - e0), mask.type(), (শূন্য*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
স্থির অন্তঃস্থ size_t chunkIndexToElem(ধ্রুৱ StatChunk& c, ধ্রুৱ পূর্ণাঙ্ক* idx)
{
    ঘূৰাই c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
