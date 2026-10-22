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

नामाकाश cv {

संरचना StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
स्थिर अन्तःस्थ तार्किक splitForParallelStat(ध्रुव Mat& src, ध्रुव Mat& mask, std::vector<StatChunk>& chunks)
{
    ध्रुव size_t total = src.total(), work = total*src.channels();
    सचे (work < ((size_t)1 << 18))
        पटिनिवत्त मिच्छा;
    ध्रुव पूर्णाङ्क nthreads = get_अङ्कः_Threads();
    सचे (nthreads <= 1)
        पटिनिवत्त मिच्छा;
    ध्रुव पूर्णाङ्क n = (पूर्णाङ्क)std::min((size_t)nthreads*2, work >> 16);
    सचे (n < 2)
        पटिनिवत्त मिच्छा;

    chunks.clear();
    सचे (src.dims <= 2 && src.rows >= n)
    {
        अत्थाय (पूर्णाङ्क i = 0; i < n; i++)
        {
            पूर्णाङ्क r0 = (पूर्णाङ्क)((int64)src.rows*i/n), r1 = (पूर्णाङ्क)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            सचे (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        पटिनिवत्त सच्च;
    }
    सचे (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        अत्थाय (पूर्णाङ्क i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (पूर्णाङ्क)(e1 - e0), src.type(), (शून्य*)(src.data + e0*src.elemSize()));
            सचे (!mask.empty())
                c.mask = Mat(1, (पूर्णाङ्क)(e1 - e0), mask.type(), (शून्य*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        पटिनिवत्त सच्च;
    }
    पटिनिवत्त मिच्छा;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
स्थिर अन्तःस्थ size_t chunkIndexToElem(ध्रुव StatChunk& c, ध्रुव पूर्णाङ्क* idx)
{
    पटिनिवत्त c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
