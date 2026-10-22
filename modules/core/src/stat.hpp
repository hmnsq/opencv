// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

#ifdef HAVE_OPENCL

गणना { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
तार्किक ocl_sum( निवेशः_Array _src, Scalar & res, पूर्णाङ्क sum_op, निवेशः_Array _mask = noArray(),
                     निवेशः_Array _src2 = noArray(), तार्किक calc2 = मिच्छ, ध्रुव Scalar & res2 = Scalar() );
तार्किक ocl_minMaxIdx( निवेशः_Array _src, द्विगुण* minVal, द्विगुण* maxVal, पूर्णाङ्क* minLoc, पूर्णाङ्क* maxLoc, निवेशः_Array _mask,
                           पूर्णाङ्क ddepth = -1, तार्किक abs_मानानि = मिच्छ, निवेशः_Array _src2 = noArray(), द्विगुण * maxVal2 = रिक्त);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    पूर्णाङ्क cn = m.channels();
    ध्रुव T * ध्रुव ptr = m.ptr<T>(0);

    प्रति (पूर्णाङ्क x = 0, w = m.cols * cn; x < w; )
        प्रति (पूर्णाङ्क c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    प्रत्यागम s;
}

#endif

प्रकारनिर्देश पूर्णाङ्क (*SumFunc)(ध्रुव uchar*, ध्रुव uchar* mask, uchar*, पूर्णाङ्क, पूर्णाङ्क);
SumFunc getSumFunc(पूर्णाङ्क depth);

}

#endif // SRC_STAT_HPP
