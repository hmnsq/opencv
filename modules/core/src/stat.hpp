// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#অন্তর্ভাব "opencv2/core/mat.hpp"

নামাকাশ cv {

#ifdef HAVE_OPENCL

গণনা { OCL_OP_যোগ = 0, OCL_OP_যোগ_ABS =  1, OCL_OP_যোগ_SQR = 2 };
যৌক্তিক ocl_যোগ( প্রবেশ_Array _src, Scalar & res, পূর্ণসংখ্যা যোগ_op, প্রবেশ_Array _mask = noArray(),
                     প্রবেশ_Array _src2 = noArray(), যৌক্তিক calc2 = মিথ্যা, ধ্রুবক Scalar & res2 = Scalar() );
যৌক্তিক ocl_minMaxIdx( প্রবেশ_Array _src, দ্বিগুণ* minVal, দ্বিগুণ* maxVal, পূর্ণসংখ্যা* minLoc, পূর্ণসংখ্যা* maxLoc, প্রবেশ_Array _mask,
                           পূর্ণসংখ্যা ddepth = -1, যৌক্তিক abs_মানসমূহ = মিথ্যা, প্রবেশ_Array _src2 = noArray(), দ্বিগুণ * maxVal2 = ফাঁকা);

template <typename T> Scalar ocl_part_যোগ(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    পূর্ণসংখ্যা cn = m.channels();
    ধ্রুবক T * ধ্রুবক ptr = m.ptr<T>(0);

    জন্য (পূর্ণসংখ্যা x = 0, w = m.cols * cn; x < w; )
        জন্য (পূর্ণসংখ্যা c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    ফেরত s;
}

#endif

প্রকারলেখ পূর্ণসংখ্যা (*যোগ_Func)(ধ্রুবক uchar*, ধ্রুবক uchar* mask, uchar*, পূর্ণসংখ্যা, পূর্ণসংখ্যা);
যোগ_Func get_যোগ_Func(পূর্ণসংখ্যা depth);

}

#endif // SRC_STAT_HPP
