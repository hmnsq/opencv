// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#اگر_غیر_تعریف_شدہ SRC_STAT_HPP
#تعریف_کرو SRC_STAT_HPP

#شامل "opencv2/core/mat.hpp"

نامخلا cv {

#اگر_تعریف_شدہ HAVE_OPENCL

شمار { OCL_OP_مجموع = 0, OCL_OP_مجموع_ABS =  1, OCL_OP_مجموع_SQR = 2 };
منطقی ocl_مجموع( داخلہ_Array _src, Scalar & res, صحیح مجموع_op, داخلہ_Array نقاب_2 = noArray(),
                     داخلہ_Array _src2 = noArray(), منطقی calc2 = جھوٹ, ثابت Scalar & res2 = Scalar() );
منطقی ocl_minMaxIdx( داخلہ_Array _src, دوہرا* minVal, دوہرا* maxVal, صحیح* minLoc, صحیح* maxLoc, داخلہ_Array نقاب_2,
                           صحیح ddepth = -1, منطقی abs_اقدار = جھوٹ, داخلہ_Array _src2 = noArray(), دوہرا * maxVal2 = تہی);

سانچہ <قسم_نام T> Scalar ocl_part_مجموع(Mat m)
{
    CV_Assert(m.صفیں == 1);

    Scalar s = Scalar::all(0);
    صحیح cn = m.channels();
    ثابت T * ثابت ptr = m.ptr<T>(0);

    برائے (صحیح x = 0, w = m.ستون_تعداد * cn; x < w; )
        برائے (صحیح c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    واپس s;
}

#اگر_ختم

قسمتعریف صحیح (*مجموع_Func)(ثابت uchar*, ثابت uchar* نقاب, uchar*, صحیح, صحیح);
مجموع_Func get_مجموع_Func(صحیح گہرائی);

}

#اگر_ختم // SRC_STAT_HPP
