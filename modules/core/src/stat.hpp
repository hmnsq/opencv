// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#include "opencv2/core/mat.hpp"

namespace cv {

#ifdef HAVE_OPENCL

enum { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
bool ocl_sum( InputArray _src, Scalar & res, 整数 sum_op, InputArray _mask = noArray(),
                     InputArray _src2 = noArray(), bool calc2 = false, const Scalar & res2 = Scalar() );
bool ocl_minMaxIdx( InputArray _src, double* minVal, double* maxVal, 整数* minLoc, 整数* maxLoc, InputArray _mask,
                           整数 ddepth = -1, bool absValues = false, InputArray _src2 = noArray(), double * maxVal2 = NULL);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    整数 cn = m.channels();
    const T * const ptr = m.ptr<T>(0);

    为 (整数 x = 0, w = m.cols * cn; x < w; )
        为 (整数 c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    返回 s;
}

#endif

typedef 整数 (*SumFunc)(const uchar*, const uchar* mask, uchar*, 整数, 整数);
SumFunc getSumFunc(整数 depth);

}

#endif // SRC_STAT_HPP
