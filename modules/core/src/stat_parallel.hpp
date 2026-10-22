// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#جے_بنا_تعریف OPENCV_CORE_STAT_PARALLEL_HPP
#تعریف_کرو OPENCV_CORE_STAT_PARALLEL_HPP

نطاق cv {

بنية StatChunk
{
    Mat src, نقاب;
    size_t پہلا_Elem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ساكن مضمن منطقي ونڈو_For_Parallel_Stat(ثابت Mat& src, ثابت Mat& نقاب, std::vector<StatChunk>& chunks)
{
    ثابت size_t کل = src.کل(), work = کل*src.channels();
    إذا (work < ((size_t)1 << 18))
        أرجع خطأ;
    ثابت صحيح nthreads = get_عدد_دھاگے();
    إذا (nthreads <= 1)
        أرجع خطأ;
    ثابت صحيح n = (صحيح)std::min((size_t)nthreads*2, work >> 16);
    إذا (n < 2)
        أرجع خطأ;

    chunks.clear();
    إذا (src.dims <= 2 && src.صفاں >= n)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            صحيح r0 = (صحيح)((int64)src.صفاں*i/n), r1 = (صحيح)((int64)src.صفاں*(i + 1)/n);
            StatChunk c;
            c.src = src.صف_Range_2(r0, r1);
            إذا (!نقاب.empty())
                c.نقاب = نقاب.صف_Range_2(r0, r1);
            c.پہلا_Elem = (size_t)r0*src.خانے_گنتی;
            chunks.push_back(c);
        }
        أرجع سچ;
    }
    إذا (src.isContinuous() && (نقاب.empty() || نقاب.isContinuous()) && کل <= (size_t)ودھ_توں_ودھ_عدد)
    {
        لكل (صحيح i = 0; i < n; i++)
        {
            size_t e0 = کل*i/n, e1 = کل*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (صحيح)(e1 - e0), src.type(), (فراغ*)(src.انکڑے + e0*src.elem_ناپ()));
            إذا (!نقاب.empty())
                c.نقاب = Mat(1, (صحيح)(e1 - e0), نقاب.type(), (فراغ*)(نقاب.انکڑے + e0*نقاب.elem_ناپ()));
            c.پہلا_Elem = e0;
            chunks.push_back(c);
        }
        أرجع سچ;
    }
    أرجع خطأ;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ساكن مضمن size_t chunk_اشاریہ_منزل_Elem(ثابت StatChunk& c, ثابت صحيح* idx)
{
    أرجع c.پہلا_Elem + (size_t)idx[0]*c.src.خانے_گنتی + idx[1];
}

} // namespace cv

#جے_انت
