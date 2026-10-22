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

ନାମାକାଶ cv {

ସଂରଚନା StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ splitForParallelStat(ଅଚଳ Mat& src, ଅଚଳ Mat& mask, std::vector<StatChunk>& chunks)
{
    ଅଚଳ size_t total = src.total(), work = total*src.channels();
    ଯଦି (work < ((size_t)1 << 18))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nthreads = get_ଅଙ୍କ_Threads();
    ଯଦି (nthreads <= 1)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)std::min((size_t)nthreads*2, work >> 16);
    ଯଦି (n < 2)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    chunks.clear();
    ଯଦି (src.dims <= 2 && src.rows >= n)
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା r0 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((int64)src.rows*i/n), r1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            ଯଦି (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଯଦି (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(e1 - e0), src.type(), (ଶୂନ୍ୟ*)(src.data + e0*src.elemSize()));
            ଯଦି (!mask.empty())
                c.mask = Mat(1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(e1 - e0), mask.type(), (ଶୂନ୍ୟ*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t chunkIndexToElem(ଅଚଳ StatChunk& c, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* idx)
{
    ଫେରନ୍ତୁ c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
