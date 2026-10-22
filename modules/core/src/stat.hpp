// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#অন্তর্ভাৱ "opencv2/core/mat.hpp"

নামাকাশ cv {

#ifdef HAVE_OPENCL

গণনা { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
তার্কিক ocl_sum( প্ৰবেশ_Array _src, Scalar & res, পূর্ণাঙ্ক sum_op, প্ৰবেশ_Array _mask = noArray(),
                     প্ৰবেশ_Array _src2 = noArray(), তার্কিক calc2 = মিছা, ধ্রুৱ Scalar & res2 = Scalar() );
তার্কিক ocl_minMaxIdx( প্ৰবেশ_Array _src, দ্ৱিগুণ* minVal, দ্ৱিগুণ* maxVal, পূর্ণাঙ্ক* minLoc, পূর্ণাঙ্ক* maxLoc, প্ৰবেশ_Array _mask,
                           পূর্ণাঙ্ক ddepth = -1, তার্কিক abs_মানসমূহ = মিছা, প্ৰবেশ_Array _src2 = noArray(), দ্ৱিগুণ * maxVal2 = রিক্ত);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    পূর্ণাঙ্ক cn = m.channels();
    ধ্রুৱ T * ধ্রুৱ ptr = m.ptr<T>(0);

    বাবে (পূর্ণাঙ্ক x = 0, w = m.cols * cn; x < w; )
        বাবে (পূর্ণাঙ্ক c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    ঘূৰাই s;
}

#endif

প্রকারনির্দেশ পূর্ণাঙ্ক (*SumFunc)(ধ্রুৱ uchar*, ধ্রুৱ uchar* mask, uchar*, পূর্ণাঙ্ক, পূর্ণাঙ্ক);
SumFunc getSumFunc(পূর্ণাঙ্ক depth);

}

#endif // SRC_STAT_HPP
