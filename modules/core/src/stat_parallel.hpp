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
    माप_प्रकार firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
स्थिर ओळीत तार्किक splitForParallelStat(स्थिरांक Mat& src, स्थिरांक Mat& mask, std::vector<StatChunk>& chunks)
{
    स्थिरांक माप_प्रकार total = src.total(), work = total*src.channels();
    जर (work < ((माप_प्रकार)1 << 18))
        परत असत्य;
    स्थिरांक पूर्णांक nthreads = get_अंक_Threads();
    जर (nthreads <= 1)
        परत असत्य;
    स्थिरांक पूर्णांक n = (पूर्णांक)std::min((माप_प्रकार)nthreads*2, work >> 16);
    जर (n < 2)
        परत असत्य;

    chunks.clear();
    जर (src.dims <= 2 && src.rows >= n)
    {
        साठी (पूर्णांक i = 0; i < n; i++)
        {
            पूर्णांक r0 = (पूर्णांक)((int64)src.rows*i/n), r1 = (पूर्णांक)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            जर (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (माप_प्रकार)r0*src.cols;
            chunks.push_back(c);
        }
        परत सत्य;
    }
    जर (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (माप_प्रकार)पूर्णांक_उच्च)
    {
        साठी (पूर्णांक i = 0; i < n; i++)
        {
            माप_प्रकार e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (पूर्णांक)(e1 - e0), src.type(), (शून्य*)(src.data + e0*src.elemSize()));
            जर (!mask.empty())
                c.mask = Mat(1, (पूर्णांक)(e1 - e0), mask.type(), (शून्य*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        परत सत्य;
    }
    परत असत्य;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
स्थिर ओळीत माप_प्रकार chunkIndexToElem(स्थिरांक StatChunk& c, स्थिरांक पूर्णांक* idx)
{
    परत c.firstElem + (माप_प्रकार)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
