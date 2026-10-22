// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#अगर_अघोषित OPENCV_CORE_STAT_PARALLEL_HPP
#घोषणा OPENCV_CORE_STAT_PARALLEL_HPP

namespace cv {

काष्ठा StatChunk
{
    Mat src, mask;
    माप_प्रकार firstElem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
अटल अंतरभूत bool splitForParallelStat(स्थिर Mat& src, स्थिर Mat& mask, std::vector<StatChunk>& chunks)
{
    स्थिर माप_प्रकार योग = src.योग(), work = योग*src.channels();
    अगर (work < ((माप_प्रकार)1 << 18))
        वापस false;
    स्थिर पूर्णांक nthreads = get_अंक_Threads();
    अगर (nthreads <= 1)
        वापस false;
    स्थिर पूर्णांक n = (पूर्णांक)std::min((माप_प्रकार)nthreads*2, work >> 16);
    अगर (n < 2)
        वापस false;

    chunks.सफा();
    अगर (src.dims <= 2 && src.rows >= n)
    {
        क्रम (पूर्णांक i = 0; i < n; i++)
        {
            पूर्णांक r0 = (पूर्णांक)((int64)src.rows*i/n), r1 = (पूर्णांक)((int64)src.rows*(i + 1)/n);
            StatChunk c;
            c.src = src.rowRange(r0, r1);
            अगर (!mask.empty())
                c.mask = mask.rowRange(r0, r1);
            c.firstElem = (माप_प्रकार)r0*src.cols;
            chunks.push_back(c);
        }
        वापस true;
    }
    अगर (src.isContinuous() && (mask.empty() || mask.isContinuous()) && योग <= (माप_प्रकार)पूर्णांक_उच्च)
    {
        क्रम (पूर्णांक i = 0; i < n; i++)
        {
            माप_प्रकार e0 = योग*i/n, e1 = योग*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (पूर्णांक)(e1 - e0), src.type(), (व्योम*)(src.तथ्य + e0*src.elem_आकार()));
            अगर (!mask.empty())
                c.mask = Mat(1, (पूर्णांक)(e1 - e0), mask.type(), (व्योम*)(mask.तथ्य + e0*mask.elem_आकार()));
            c.firstElem = e0;
            chunks.push_back(c);
        }
        वापस true;
    }
    वापस false;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
अटल अंतरभूत माप_प्रकार chunk_सूचकांक_To_Elem(स्थिर StatChunk& c, स्थिर पूर्णांक* idx)
{
    वापस c.firstElem + (माप_प्रकार)idx[0]*c.src.cols + idx[1];
}

} // namespace cv

#पूर्ण_अगर
