// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#include "opencv2/core/mat.hpp"

namespace cv {

#ifdef HAVE_OPENCL

enum { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
bool ocl_sum( InputArray _src, Scalar & res, цео sum_op, InputArray _mask = noArray(),
                     InputArray _src2 = noArray(), bool calc2 = false, const Scalar & res2 = Scalar() );
bool ocl_minMaxIdx( InputArray _src, double* minVal, double* maxVal, цео* minLoc, цео* maxLoc, InputArray _mask,
                           цео ddepth = -1, bool absValues = false, InputArray _src2 = noArray(), double * maxVal2 = NULL);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    цео cn = m.channels();
    const T * const ptr = m.ptr<T>(0);

    за (цео x = 0, w = m.cols * cn; x < w; )
        за (цео c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    врати s;
}

#endif

typedef цео (*SumFunc)(const uchar*, const uchar* mask, uchar*, цео, цео);
SumFunc getSumFunc(цео depth);

}

#endif // SRC_STAT_HPP
