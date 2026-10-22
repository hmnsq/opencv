// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#ضمن "opencv2/core/mat.hpp"

نطاق cv {

#ifdef HAVE_OPENCL

تعداد { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
منطقي ocl_sum( كىرگۈزۈش_Array _src, Scalar & res, صحيح sum_op, كىرگۈزۈش_Array _mask = noArray(),
                     كىرگۈزۈش_Array _src2 = noArray(), منطقي calc2 = خطأ, ثابت Scalar & res2 = Scalar() );
منطقي ocl_minMaxIdx( كىرگۈزۈش_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, كىرگۈزۈش_Array _mask,
                           صحيح ddepth = -1, منطقي abs_قىممەتلەر = خطأ, كىرگۈزۈش_Array _src2 = noArray(), مزدوج * maxVal2 = عدم);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    صحيح cn = m.channels();
    ثابت T * ثابت ptr = m.ptr<T>(0);

    لكل (صحيح x = 0, w = m.cols * cn; x < w; )
        لكل (صحيح c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    أرجع s;
}

#endif

تعريفنوع صحيح (*SumFunc)(ثابت uchar*, ثابت uchar* mask, uchar*, صحيح, صحيح);
SumFunc getSumFunc(صحيح depth);

}

#endif // SRC_STAT_HPP
