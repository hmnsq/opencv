// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#شامل "opencv2/core/mat.hpp"

فضاینام cv {

#ifdef HAVE_OPENCL

شمارش { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
منطقی ocl_sum( درون_داد_Array _src, Scalar & res, صحیح sum_op, درون_داد_Array _mask = noArray(),
                     درون_داد_Array _src2 = noArray(), منطقی calc2 = نادرست, ثابت Scalar & res2 = Scalar() );
منطقی ocl_minMaxIdx( درون_داد_Array _src, دوگانه* minVal, دوگانه* maxVal, صحیح* minLoc, صحیح* maxLoc, درون_داد_Array _mask,
                           صحیح ddepth = -1, منطقی abs_مقادیر = نادرست, درون_داد_Array _src2 = noArray(), دوگانه * maxVal2 = پوچ);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    صحیح cn = m.channels();
    ثابت T * ثابت ptr = m.ptr<T>(0);

    برای (صحیح x = 0, w = m.cols * cn; x < w; )
        برای (صحیح c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    بازگشت s;
}

#endif

تعریفنوع صحیح (*SumFunc)(ثابت uchar*, ثابت uchar* mask, uchar*, صحیح, صحیح);
SumFunc getSumFunc(صحیح depth);

}

#endif // SRC_STAT_HPP
