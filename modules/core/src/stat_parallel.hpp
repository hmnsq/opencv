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

નામાકાશ cv {

સંરચના StatChunk
{
    Mat src, mask;
    size_t firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
સ્થિર અંતઃરેખ તાર્કિક splitForParallelStat(અચલ Mat& src, અચલ Mat& mask, std::vector<StatChunk>& chunks)
{
    અચલ size_t total = src.total(), work = total*src.channels();
    જો (work < ((size_t)1 << 18))
        પાછા ખોટું;
    અચલ પૂર્ણાંક nthreads = get_અંક_Threads();
    જો (nthreads <= 1)
        પાછા ખોટું;
    અચલ પૂર્ણાંક n = (પૂર્ણાંક)std::min((size_t)nthreads*2, work >> 16);
    જો (n < 2)
        પાછા ખોટું;

    chunks.clear();
    જો (src.dims <= 2 && src.rows >= n)
    {
        માટે (પૂર્ણાંક i = 0; i < n; i++)
        {
            પૂર્ણાંક r0 = (પૂર્ણાંક)((int64)src.rows*i/n), r1 = (પૂર્ણાંક)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            જો (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (size_t)r0*src.cols;
            chunks.push_back(c);
        }
        પાછા સાચું;
    }
    જો (src.isContinuous() && (mask.empty() || mask.isContinuous()) && total <= (size_t)INT_MAX)
    {
        માટે (પૂર્ણાંક i = 0; i < n; i++)
        {
            size_t e0 = total*i/n, e1 = total*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (પૂર્ણાંક)(e1 - e0), src.type(), (શૂન્ય*)(src.data + e0*src.elemSize()));
            જો (!mask.empty())
                c.mask = Mat(1, (પૂર્ણાંક)(e1 - e0), mask.type(), (શૂન્ય*)(mask.data + e0*mask.elemSize()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        પાછા સાચું;
    }
    પાછા ખોટું;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
સ્થિર અંતઃરેખ size_t chunkIndexToElem(અચલ StatChunk& c, અચલ પૂર્ણાંક* idx)
{
    પાછા c.firstElem + (size_t)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#endif
