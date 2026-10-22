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
स्थिर अन्तःस्थ तार्किक splitForParallelStat(अचर Mat& src, अचर Mat& mask, std::vector<StatChunk>& chunks)
{
    अचर माप_प्रकार total = src.total(), work = total*src.channels();
    यदि (work < ((माप_प्रकार)1 << 18))
        फर्काऊ असत्य;
    अचर पूर्णाङ्क nthreads = get_अङ्क_Threads();
    यदि (nthreads <= 1)
        फर्काऊ असत्य;
    अचर पूर्णाङ्क n = (पूर्णाङ्क)std::min((माप_प्रकार)nthreads*2, work >> 16);
    यदि (n < 2)
        फर्काऊ असत्य;

    chunks.clear();
    यदि (src.dims <= 2 && src.rows >= n)
    {
        लागि (पूर्णाङ्क i = 0; i < n; i++)
        {
            पूर्णाङ्क r0 = (पूर्णाङ्क)((int64)src.rows*i/n), r1 = (पूर्णाङ्क)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            यदि (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (माप_प्रकार)r0*src.cols;
            chunks.push_back(c);
        }
        फर्काऊ सत्य;
    }
    यदि (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (माप_प्रकार)पूर्णांक_उच्च)
    {
        लागि (पूर्णाङ्क i = 0; i < n; i++)
        {
            माप_प्रकार e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (पूर्णाङ्क)(e1 - e0), src.type(), (शून्य*)(src.data + e0*src.elemSize()));
            यदि (!mask.empty())
                c.mask = Mat(1, (पूर्णाङ्क)(e1 - e0), mask.type(), (शून्य*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        फर्काऊ सत्य;
    }
    फर्काऊ असत्य;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
स्थिर अन्तःस्थ माप_प्रकार chunkIndexToElem(अचर StatChunk& c, अचर पूर्णाङ्क* idx)
{
    फर्काऊ c.firstElem + (माप_प्रकार)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
