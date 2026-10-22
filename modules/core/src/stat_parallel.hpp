// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#నిర్వచితం_కాకపోతే OPENCV_CORE_STAT_PARALLEL_HPP
#నిర్వచించు OPENCV_CORE_STAT_PARALLEL_HPP

నామాకాశ cv {

నిర్మాణం StatChunk
{
    Mat src, ముసుగు;
    size_t మొదటి_Elem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
స్థిర అంతఃవరుస తార్కిక విభజించు_For_Parallel_Stat(స్థిరరాశి Mat& src, స్థిరరాశి Mat& ముసుగు, std::vector<StatChunk>& chunks)
{
    స్థిరరాశి size_t మొత్తం = src.మొత్తం(), work = మొత్తం*src.channels();
    ఒకవేళ (work < ((size_t)1 << 18))
        తిరిగి అబద్ధం;
    స్థిరరాశి పూర్ణాంకం nthreads = get_అంకె_దారాలు();
    ఒకవేళ (nthreads <= 1)
        తిరిగి అబద్ధం;
    స్థిరరాశి పూర్ణాంకం n = (పూర్ణాంకం)std::min((size_t)nthreads*2, work >> 16);
    ఒకవేళ (n < 2)
        తిరిగి అబద్ధం;

    chunks.clear();
    ఒకవేళ (src.dims <= 2 && src.అడ్డువరుసలు >= n)
    {
        కొరకు (పూర్ణాంకం i = 0; i < n; i++)
        {
            పూర్ణాంకం r0 = (పూర్ణాంకం)((int64)src.అడ్డువరుసలు*i/n), r1 = (పూర్ణాంకం)((int64)src.అడ్డువరుసలు*(i + 1)/n);
            StatChunk c;
            c.src = src.అడ్డువరుస_Range_2(r0, r1);
            ఒకవేళ (!ముసుగు.empty())
                c.ముసుగు = ముసుగు.అడ్డువరుస_Range_2(r0, r1);
            c.మొదటి_Elem = (size_t)r0*src.నిలువుల_సంఖ్య;
            chunks.push_back(c);
        }
        తిరిగి నిజం;
    }
    ఒకవేళ (src.isContinuous() && (ముసుగు.empty() || ముసుగు.isContinuous()) && మొత్తం <= (size_t)గరిష్ఠ_పూర్ణాంకం)
    {
        కొరకు (పూర్ణాంకం i = 0; i < n; i++)
        {
            size_t e0 = మొత్తం*i/n, e1 = మొత్తం*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (పూర్ణాంకం)(e1 - e0), src.type(), (శూన్య*)(src.దత్తాంశం + e0*src.elem_కొలత()));
            ఒకవేళ (!ముసుగు.empty())
                c.ముసుగు = Mat(1, (పూర్ణాంకం)(e1 - e0), ముసుగు.type(), (శూన్య*)(ముసుగు.దత్తాంశం + e0*ముసుగు.elem_కొలత()));
            c.మొదటి_Elem = e0;
            chunks.push_back(c);
        }
        తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
స్థిర అంతఃవరుస size_t chunk_సూచీ_గమ్యం_Elem(స్థిరరాశి StatChunk& c, స్థిరరాశి పూర్ణాంకం* idx)
{
    తిరిగి c.మొదటి_Elem + (size_t)idx[0]*c.src.నిలువుల_సంఖ్య + idx[1];
}

} // namespace cv

#అయితే_ముగింపు
