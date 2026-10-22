// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#اگر_تعریف_نشده OPENCV_CORE_STAT_PARALLEL_HPP
#تعریف_کن OPENCV_CORE_STAT_PARALLEL_HPP

نطاق cv {

بنية StatChunk
{
    Mat src, ماسک;
    size_t اولی_Elem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ساكن مضمن منطقي تقسیم_For_Parallel_Stat(ثابت Mat& src, ثابت Mat& ماسک, std::vector<StatChunk>& chunks)
{
    ثابت size_t مجموع = src.مجموع(), work = مجموع*src.channels();
    إذا (work < ((size_t)1 << 18))
        أرجع خطأ;
    ثابت صحيح nthreads = get_رقم_نخها();
    إذا (nthreads <= 1)
        أرجع خطأ;
    ثابت صحيح n = (صحيح)std::min((size_t)nthreads*2, work >> 16);
    إذا (n < 2)
        أرجع خطأ;

    chunks.clear();
    إذا (src.dims <= 2 && src.ردیفها >= n)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            صحيح r0 = (صحيح)((int64)src.ردیفها*i/n), r1 = (صحيح)((int64)src.ردیفها*(i + 1)/n);
            StatChunk c;
            c.src = src.ردیف_Range_2(r0, r1);
            إذا (!ماسک.empty())
                c.ماسک = ماسک.ردیف_Range_2(r0, r1);
            c.اولی_Elem = (size_t)r0*src.تعداد_ستون;
            chunks.push_back(c);
        }
        أرجع درست;
    }
    إذا (src.isContinuous() && (ماسک.empty() || ماسک.isContinuous()) && مجموع <= (size_t)بیشترین_صحیح)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            size_t e0 = مجموع*i/n, e1 = مجموع*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (صحيح)(e1 - e0), src.type(), (فراغ*)(src.داده + e0*src.elem_اندازه()));
            إذا (!ماسک.empty())
                c.ماسک = Mat(1, (صحيح)(e1 - e0), ماسک.type(), (فراغ*)(ماسک.داده + e0*ماسک.elem_اندازه()));
            c.اولی_Elem = e0;
            chunks.push_back(c);
        }
        أرجع درست;
    }
    أرجع خطأ;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ساكن مضمن size_t chunk_شاخص_مقصد_Elem(ثابت StatChunk& c, ثابت صحيح* idx)
{
    أرجع c.اولی_Elem + (size_t)idx[0]*c.src.تعداد_ستون + idx[1];
}

} // namespace cv

#پایان_اگر
