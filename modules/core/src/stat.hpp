// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अगर_अघोषित SRC_STAT_HPP
#घोषणा SRC_STAT_HPP

#समावेश "opencv2/core/mat.hpp"

namespace cv {

#अगर_घोषित HAVE_OPENCL

क्रमागत { OCL_OP_योग = 0, OCL_OP_योग_ABS =  1, OCL_OP_योग_SQR = 2 };
bool ocl_योग( निवेश_Array _src, Scalar & res, पूर्णांक योग_op, निवेश_Array _mask = noArray(),
                     निवेश_Array _src2 = noArray(), bool calc2 = false, स्थिर Scalar & res2 = Scalar() );
bool ocl_minMaxIdx( निवेश_Array _src, द्विगुन* minVal, द्विगुन* maxVal, पूर्णांक* minLoc, पूर्णांक* maxLoc, निवेश_Array _mask,
                           पूर्णांक ddepth = -1, bool abs_मूल्यावली = false, निवेश_Array _src2 = noArray(), द्विगुन * maxVal2 = शून्य);

ढाँचा <typename T> Scalar ocl_part_योग(Mat m)
{
    CV_Assert(m.rows == 1);

    Scalar s = Scalar::all(0);
    पूर्णांक cn = m.channels();
    स्थिर T * स्थिर ptr = m.ptr<T>(0);

    क्रम (पूर्णांक x = 0, w = m.cols * cn; x < w; )
        क्रम (पूर्णांक c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    वापस s;
}

#पूर्ण_अगर

प्रकार पूर्णांक (*योग_Func)(स्थिर uchar*, स्थिर uchar* mask, uchar*, पूर्णांक, पूर्णांक);
योग_Func get_योग_Func(पूर्णांक depth);

}

#पूर्ण_अगर // SRC_STAT_HPP
