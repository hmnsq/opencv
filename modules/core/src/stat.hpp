// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#include "opencv2/core/mat.hpp"

namespace cv {

#ifdef HAVE_OPENCL

ga_nanaa { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
bool ocl_sum( InputArray _src, Scalar & res, puur_naa_nka sum_op, InputArray _mask = noArray(),
                     InputArray _src2 = noArray(), bool calc2 = false, const Scalar & res2 = Scalar() );
bool ocl_minMaxIdx( InputArray _src, dviguu_na* minVal, dviguu_na* maxVal, puur_naa_nka* minLoc, puur_naa_nka* maxLoc, InputArray _mask,
                           puur_naa_nka ddepth = -1, bool absValues = false, InputArray _src2 = noArray(), dviguu_na * maxVal2 = NULL);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    puur_naa_nka cn = m.channels();
    const T * const ptr = m.ptr<T>(0);

    for (puur_naa_nka x = 0, w = m.cols * cn; x < w; )
        for (puur_naa_nka c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    ᯀᯮᯞ s;
}

#endif

typedef puur_naa_nka (*SumFunc)(const uchar*, const uchar* mask, uchar*, puur_naa_nka, puur_naa_nka);
SumFunc getSumFunc(puur_naa_nka depth);

}

#endif // SRC_STAT_HPP
