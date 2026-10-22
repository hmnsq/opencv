// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ifndef SRC_STAT_HPP
#define SRC_STAT_HPP

#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

#ifdef HAVE_OPENCL

गणना { OCL_OP_SUM = 0, OCL_OP_SUM_ABS =  1, OCL_OP_SUM_SQR = 2 };
तार्किक ocl_sum( आदान_Array _src, Scalar & res, पूर्णांक sum_op, आदान_Array _mask = noArray(),
                     आदान_Array _src2 = noArray(), तार्किक calc2 = असत्य, स्थिरांक Scalar & res2 = Scalar() );
तार्किक ocl_minMaxIdx( आदान_Array _src, द्विगुण* minVal, द्विगुण* maxVal, पूर्णांक* minLoc, पूर्णांक* maxLoc, आदान_Array _mask,
                           पूर्णांक ddepth = -1, तार्किक abs_मूल्ये = असत्य, आदान_Array _src2 = noArray(), द्विगुण * maxVal2 = रिक्त);

ढाँचा <typename T> Scalar ocl_part_sum(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    पूर्णांक cn = m.channels();
    स्थिरांक T * स्थिरांक ptr = m.ptr<T>(0);

    साठी (पूर्णांक x = 0, w = m.cols * cn; x < w; )
        साठी (पूर्णांक c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    परत s;
}

#endif

प्रकारव्याख्या पूर्णांक (*SumFunc)(स्थिरांक uchar*, स्थिरांक uchar* mask, uchar*, पूर्णांक, पूर्णांक);
SumFunc getSumFunc(पूर्णांक depth);

}

#endif // SRC_STAT_HPP
