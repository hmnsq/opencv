// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#include "opencv2/core/mat.hpp"

namespace cv {

#ifdef HAVE_OPENCL

ga_nanaa { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
bool ocl_sum( InputArray _src, Scalar & res, ຈຳນວນເຕັມ sum_op, InputArray _mask = noArray(),
                     InputArray _src2 = noArray(), bool calc2 = false, const Scalar & res2 = Scalar() );
bool ocl_minMaxIdx( InputArray _src, dviguu_na* minVal, dviguu_na* maxVal, ຈຳນວນເຕັມ* minLoc, ຈຳນວນເຕັມ* maxLoc, InputArray _mask,
                           ຈຳນວນເຕັມ ddepth = -1, bool absValues = false, InputArray _src2 = noArray(), dviguu_na * maxVal2 = NULL);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    ຈຳນວນເຕັມ cn = m.channels();
    const T * const ptr = m.ptr<T>(0);

    ສຳລັບ (ຈຳນວນເຕັມ x = 0, w = m.cols * cn; x < w; )
        ສຳລັບ (ຈຳນວນເຕັມ c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    ສົ່ງຄືນ s;
}

#endif

typedef ຈຳນວນເຕັມ (*SumFunc)(const uchar*, const uchar* mask, uchar*, ຈຳນວນເຕັມ, ຈຳນວນເຕັມ);
SumFunc getSumFunc(ຈຳນວນເຕັມ depth);

}

#endif // SRC_STAT_HPP
