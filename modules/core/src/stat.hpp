// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#यदि_अपरिभाषितम् SRC_STAT_HPP
#परिभाषय SRC_STAT_HPP

#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

#यदि_परिभाषितम् HAVE_OPENCL

गणना { OCL_OP_योग = 0, OCL_OP_योग_ABS =  1, OCL_OP_योग_SQR = 2 };
तार्किक ocl_योग( निवेशः_Array _src, Scalar & res, पूर्णाङ्क योग_op, निवेशः_Array आवरणम्_2 = noArray(),
                     निवेशः_Array _src2 = noArray(), तार्किक calc2 = असत्य, ध्रुव Scalar & res2 = Scalar() );
तार्किक ocl_minMaxIdx( निवेशः_Array _src, द्विगुण* minVal, द्विगुण* maxVal, पूर्णाङ्क* minLoc, पूर्णाङ्क* maxLoc, निवेशः_Array आवरणम्_2,
                           पूर्णाङ्क ddepth = -1, तार्किक abs_मानानि = असत्य, निवेशः_Array _src2 = noArray(), द्विगुण * maxVal2 = रिक्त);

प्रतिरूपम् <प्रकारनाम T> Scalar ocl_part_योग(Mat m)
{
    CV_Assert(m.पङ्क्तिकाः == 1);

    Scalar s = Scalar::all(0);
    पूर्णाङ्क cn = m.channels();
    ध्रुव T * ध्रुव ptr = m.ptr<T>(0);

    प्रति (पूर्णाङ्क x = 0, w = m.स्तम्भसङ्ख्या * cn; x < w; )
        प्रति (पूर्णाङ्क c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    प्रत्यागम s;
}

#यदि_अन्तः

प्रकारनिर्देश पूर्णाङ्क (*योग_Func)(ध्रुव uchar*, ध्रुव uchar* आवरणम्, uchar*, पूर्णाङ्क, पूर्णाङ्क);
योग_Func get_योग_Func(पूर्णाङ्क गभीरता);

}

#यदि_अन्तः // SRC_STAT_HPP
