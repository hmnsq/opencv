// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#অন্তর্ভাব "opencv2/core/mat.hpp"

নামাকাশ cv {

#ifdef HAVE_OPENCL

গণনা { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
তার্কিক ocl_sum( প্রবেশ_Array _src, Scalar & res, পূর্ণাঙ্ক sum_op, প্রবেশ_Array _mask = noArray(),
                     প্রবেশ_Array _src2 = noArray(), তার্কিক calc2 = অসত্য, ধ্রুব Scalar & res2 = Scalar() );
তার্কিক ocl_minMaxIdx( প্রবেশ_Array _src, দ্বিগুণ* minVal, দ্বিগুণ* maxVal, পূর্ণাঙ্ক* minLoc, পূর্ণাঙ্ক* maxLoc, প্রবেশ_Array _mask,
                           পূর্ণাঙ্ক ddepth = -1, তার্কিক abs_মানসমূহ = অসত্য, প্রবেশ_Array _src2 = noArray(), দ্বিগুণ * maxVal2 = রিক্ত);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    পূর্ণাঙ্ক cn = m.channels();
    ধ্রুব T * ধ্রুব ptr = m.ptr<T>(0);

    প্রতি (পূর্ণাঙ্ক x = 0, w = m.cols * cn; x < w; )
        প্রতি (পূর্ণাঙ্ক c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    প্রত্যাগম s;
}

#endif

প্রকারনির্দেশ পূর্ণাঙ্ক (*SumFunc)(ধ্রুব uchar*, ধ্রুব uchar* mask, uchar*, পূর্ণাঙ্ক, পূর্ণাঙ্ক);
SumFunc getSumFunc(পূর্ণাঙ্ক depth);

}

#endif // SRC_STAT_HPP
