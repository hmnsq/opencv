// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Splitting of an array into pieces for the parallel versions of sum, mean, meanStdDev and minMaxIdx.
// Each piece is reduced by the existing serial code (including the HAL), and the results are
// combined in piece order.

#اگر_غیر_تعریف_شدہ OPENCV_CORE_STAT_PARALLEL_HPP
#تعریف_کرو OPENCV_CORE_STAT_PARALLEL_HPP

نامخلا cv {

ساخت StatChunk
{
    Mat src, نقاب;
    size_t پہلا_Elem;   // row-major index of the first element of the piece in the whole array
};

// Returns false when the array is too small to be worth splitting, or cannot be split.
ساکن اندرونی منطقی بانٹو_For_Parallel_Stat(ثابت Mat& src, ثابت Mat& نقاب, std::vector<StatChunk>& chunks)
{
    ثابت size_t کل = src.کل(), work = کل*src.channels();
    اگر (work < ((size_t)1 << 18))
        واپس جھوٹ;
    ثابت صحیح nthreads = get_عدد_دھاگے();
    اگر (nthreads <= 1)
        واپس جھوٹ;
    ثابت صحیح n = (صحیح)std::min((size_t)nthreads*2, work >> 16);
    اگر (n < 2)
        واپس جھوٹ;

    chunks.clear();
    اگر (src.dims <= 2 && src.صفیں >= n)
    {
        برائے (صحیح i = 0; i < n; i++)
        {
            صحیح r0 = (صحیح)((int64)src.صفیں*i/n), r1 = (صحیح)((int64)src.صفیں*(i + 1)/n);
            StatChunk c;
            c.src = src.صف_Range_2(r0, r1);
            اگر (!نقاب.empty())
                c.نقاب = نقاب.صف_Range_2(r0, r1);
            c.پہلا_Elem = (size_t)r0*src.ستون_تعداد;
            chunks.push_back(c);
        }
        واپس سچ;
    }
    اگر (src.isContinuous() && (نقاب.empty() || نقاب.isContinuous()) && کل <= (size_t)زیادہ_سے_زیادہ_عدد)
    {
        برائے (صحیح i = 0; i < n; i++)
        {
            size_t e0 = کل*i/n, e1 = کل*(i + 1)/n;
            StatChunk c;
            c.src = Mat(1, (صحیح)(e1 - e0), src.type(), (خالی*)(src.ڈیٹا + e0*src.elem_سائز()));
            اگر (!نقاب.empty())
                c.نقاب = Mat(1, (صحیح)(e1 - e0), نقاب.type(), (خالی*)(نقاب.ڈیٹا + e0*نقاب.elem_سائز()));
            c.پہلا_Elem = e0;
            chunks.push_back(c);
        }
        واپس سچ;
    }
    واپس جھوٹ;
}

// row-major index of an element given by the per-dimension indices of a 2D piece
ساکن اندرونی size_t chunk_اشاریہ_مقصد_Elem(ثابت StatChunk& c, ثابت صحیح* idx)
{
    واپس c.پہلا_Elem + (size_t)idx[0]*c.src.ستون_تعداد + idx[1];
}

} // namespace cv

#اگر_ختم
