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

நாமாகாஶ cv {

கட்டமைப்பு StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
நிலையான உள்வரி தருக்க splitForParallelStat(மாறிலி Mat& src, மாறிலி Mat& mask, std::vector<StatChunk>& chunks)
{
    மாறிலி size_t total = src.total(), work = total*src.channels();
    எனில் (work < ((size_t)1 << 18))
        திரும்பு பொய்;
    மாறிலி முழு nthreads = get_இலக்கம்_Threads();
    எனில் (nthreads <= 1)
        திரும்பு பொய்;
    மாறிலி முழு n = (முழு)std::min((size_t)nthreads*2, work >> 16);
    எனில் (n < 2)
        திரும்பு பொய்;

    chunks.clear();
    எனில் (src.dims <= 2 && src.rows >= n)
    {
        க்கு (முழு i = 0; i < n; i++)
        {
            முழு r0 = (முழு)((int64)src.rows*i/n), r1 = (முழு)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            எனில் (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        திரும்பு உண்மை;
    }
    எனில் (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        க்கு (முழு i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (முழு)(e1 - e0), src.type(), (வெறுமை*)(src.data + e0*src.elemSize()));
            எனில் (!mask.empty())
                c.mask = Mat(1, (முழு)(e1 - e0), mask.type(), (வெறுமை*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        திரும்பு உண்மை;
    }
    திரும்பு பொய்;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
நிலையான உள்வரி size_t chunkIndexToElem(மாறிலி StatChunk& c, மாறிலி முழு* idx)
{
    திரும்பு c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
