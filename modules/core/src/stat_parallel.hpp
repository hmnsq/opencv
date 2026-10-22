// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#که_نه_تعریف_شوی OPENCV_CORE_STAT_PARALLEL_HPP
#تعریف_کړه OPENCV_CORE_STAT_PARALLEL_HPP

نطاق cv {

بنية StatChunk
{
    Mat src, ماسک;
    size_t لومړی_Elem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ساكن مضمن منطقي وېشل_For_Parallel_Stat(ثابت Mat& src, ثابت Mat& ماسک, std::vector<StatChunk>& chunks)
{
    ثابت size_t ټول = src.ټول(), work = ټول*src.channels();
    إذا (work < ((size_t)1 << 18))
        أرجع خطأ;
    ثابت صحيح nthreads = get_عدد_تارونه();
    إذا (nthreads <= 1)
        أرجع خطأ;
    ثابت صحيح n = (صحيح)std::min((size_t)nthreads*2, work >> 16);
    إذا (n < 2)
        أرجع خطأ;

    chunks.clear();
    إذا (src.dims <= 2 && src.لیکې >= n)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            صحيح r0 = (صحيح)((int64)src.لیکې*i/n), r1 = (صحيح)((int64)src.لیکې*(i + 1)/n);
            StatChunk c;
            c.src = src.لیکه_Range_2(r0, r1);
            إذا (!ماسک.empty())
                c.ماسک = ماسک.لیکه_Range_2(r0, r1);
            c.لومړی_Elem = (size_t)r0*src.ستون_شمېر;
            chunks.push_back(c);
        }
        أرجع ریښتیا;
    }
    إذا (src.isContinuous() && (ماسک.empty() || ماسک.isContinuous()) && ټول <= (size_t)لوی_عدد)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            size_t e0 = ټول*i/n, e1 = ټول*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (صحيح)(e1 - e0), src.type(), (فراغ*)(src.معلومات + e0*src.elem_اندازه()));
            إذا (!ماسک.empty())
                c.ماسک = Mat(1, (صحيح)(e1 - e0), ماسک.type(), (فراغ*)(ماسک.معلومات + e0*ماسک.elem_اندازه()));
            c.لومړی_Elem = e0;
            chunks.push_back(c);
        }
        أرجع ریښتیا;
    }
    أرجع خطأ;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ساكن مضمن size_t chunk_شاخص_منزل_Elem(ثابت StatChunk& c, ثابت صحيح* idx)
{
    أرجع c.لومړی_Elem + (size_t)idx[0]*c.src.ستون_شمېر + idx[1];
}

} // namespace cv

#د_که_پای
