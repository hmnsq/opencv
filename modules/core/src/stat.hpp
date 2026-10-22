// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#அந்தர்பாவ "opencv2/core/mat.hpp"

நாமாகாஶ cv {

#ifdef HAVE_OPENCL

எண்ணிக்கை { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
தருக்க ocl_sum( உள்ளீடு_Array _src, Scalar & res, முழு sum_op, உள்ளீடு_Array _mask = noArray(),
                     உள்ளீடு_Array _src2 = noArray(), தருக்க calc2 = பொய், மாறிலி Scalar & res2 = Scalar() );
தருக்க ocl_minMaxIdx( உள்ளீடு_Array _src, இரட்டை* minVal, இரட்டை* maxVal, முழு* minLoc, முழு* maxLoc, உள்ளீடு_Array _mask,
                           முழு ddepth = -1, தருக்க abs_மதிப்புகள் = பொய், உள்ளீடு_Array _src2 = noArray(), இரட்டை * maxVal2 = வெற்று);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    முழு cn = m.channels();
    மாறிலி T * மாறிலி ptr = m.ptr<T>(0);

    க்கு (முழு x = 0, w = m.cols * cn; x < w; )
        க்கு (முழு c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    திரும்பு s;
}

#endif

வகைவரை முழு (*SumFunc)(மாறிலி uchar*, மாறிலி uchar* mask, uchar*, முழு, முழு);
SumFunc getSumFunc(முழு depth);

}

#endif // SRC_STAT_HPP
