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

نطاق cv {

بنية StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ساكن مضمن منطقي splitForParallelStat(ثابت Mat& src, ثابت Mat& mask, std::vector<StatChunk>& chunks)
{
    ثابت size_t total = src.total(), work = total*src.channels();
    إذا (work < ((size_t)1 << 18))
        أرجع خطأ;
    ثابت صحيح nthreads = get_رەقەم_Threads();
    إذا (nthreads <= 1)
        أرجع خطأ;
    ثابت صحيح n = (صحيح)std::min((size_t)nthreads*2, work >> 16);
    إذا (n < 2)
        أرجع خطأ;

    chunks.clear();
    إذا (src.dims <= 2 && src.rows >= n)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            صحيح r0 = (صحيح)((int64)src.rows*i/n), r1 = (صحيح)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            إذا (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        أرجع true;
    }
    إذا (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (صحيح)(e1 - e0), src.type(), (فراغ*)(src.data + e0*src.elemSize()));
            إذا (!mask.empty())
                c.mask = Mat(1, (صحيح)(e1 - e0), mask.type(), (فراغ*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        أرجع true;
    }
    أرجع خطأ;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ساكن مضمن size_t chunkIndexToElem(ثابت StatChunk& c, ثابت صحيح* idx)
{
    أرجع c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
