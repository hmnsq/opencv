// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"

ನಾಮಾಕಾಶ cv {

#ifdef HAVE_OPENCL

ಗಣನಾ { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
ತಾರ್ಕಿಕ ocl_sum( ಒಳಹರಿವು_Array _src, Scalar & res, ಪೂರ್ಣಾಙ್ಕ sum_op, ಒಳಹರಿವು_Array _mask = noArray(),
                     ಒಳಹರಿವು_Array _src2 = noArray(), ತಾರ್ಕಿಕ calc2 = ಅಸತ್ಯ, ಧ್ರುವ Scalar & res2 = Scalar() );
ತಾರ್ಕಿಕ ocl_minMaxIdx( ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ* minVal, ದ್ವಿಗುಣ* maxVal, ಪೂರ್ಣಾಙ್ಕ* minLoc, ಪೂರ್ಣಾಙ್ಕ* maxLoc, ಒಳಹರಿವು_Array _mask,
                           ಪೂರ್ಣಾಙ್ಕ ddepth = -1, ತಾರ್ಕಿಕ abs_ಮೌಲ್ಯಗಳು = ಅಸತ್ಯ, ಒಳಹರಿವು_Array _src2 = noArray(), ದ್ವಿಗುಣ * maxVal2 = ರಿಕ್ತ);

template <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    ಪೂರ್ಣಾಙ್ಕ cn = m.channels();
    ಧ್ರುವ T * ಧ್ರುವ ptr = m.ptr<T>(0);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0, w = m.cols * cn; x < w; )
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    ಪ್ರತ್ಯಾಗಮ s;
}

#endif

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*SumFunc)(ಧ್ರುವ uchar*, ಧ್ರುವ uchar* mask, uchar*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ);
SumFunc getSumFunc(ಪೂರ್ಣಾಙ್ಕ depth);

}

#endif // SRC_STAT_HPP
