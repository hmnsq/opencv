// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#جيڪڏهن_وصف_نه_ٿيل OPENCV_CORE_STAT_PARALLEL_HPP
#وصف_ڪريو OPENCV_CORE_STAT_PARALLEL_HPP

نطاق cv {

بنية StatChunk
{
    Mat src, نقاب;
    size_t پهريون_Elem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ساكن مضمن منطقي ورهايو_For_Parallel_Stat(ثابت Mat& src, ثابت Mat& نقاب, std::vector<StatChunk>& chunks)
{
    ثابت size_t ڪل = src.ڪل(), work = ڪل*src.channels();
    إذا (work < ((size_t)1 << 18))
        أرجع خطأ;
    ثابت صحيح nthreads = get_انگ_ڌاڳا();
    إذا (nthreads <= 1)
        أرجع خطأ;
    ثابت صحيح n = (صحيح)std::min((size_t)nthreads*2, work >> 16);
    إذا (n < 2)
        أرجع خطأ;

    chunks.clear();
    إذا (src.dims <= 2 && src.صفون >= n)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            صحيح r0 = (صحيح)((int64)src.صفون*i/n), r1 = (صحيح)((int64)src.صفون*(i + 1)/n);
            StatChunk c;
            c.src = src.صف_Range_2(r0, r1);
            إذا (!نقاب.empty())
                c.نقاب = نقاب.صف_Range_2(r0, r1);
            c.پهريون_Elem = (size_t)r0*src.کانن_ڳڻپ;
            chunks.push_back(c);
        }
        أرجع سچ;
    }
    إذا (src.isContinuous() && (نقاب.empty() || نقاب.isContinuous()) && ڪل <= (size_t)وڌ_کان_وڌ_عدد)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            size_t e0 = ڪل*i/n, e1 = ڪل*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (صحيح)(e1 - e0), src.type(), (فراغ*)(src.معلومات + e0*src.elem_سائيز()));
            إذا (!نقاب.empty())
                c.نقاب = Mat(1, (صحيح)(e1 - e0), نقاب.type(), (فراغ*)(نقاب.معلومات + e0*نقاب.elem_سائيز()));
            c.پهريون_Elem = e0;
            chunks.push_back(c);
        }
        أرجع سچ;
    }
    أرجع خطأ;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ساكن مضمن size_t chunk_اشاريو_منزل_Elem(ثابت StatChunk& c, ثابت صحيح* idx)
{
    أرجع c.پهريون_Elem + (size_t)idx[0]*c.src.کانن_ڳڻپ + idx[1];
}

} // namespace cv

#جيڪڏهن_پورو
