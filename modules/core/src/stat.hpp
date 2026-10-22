// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#اگر_تعریف_نشده SRC_STAT_HPP
#تعریف_کن SRC_STAT_HPP

#ضمن "opencv2/core/mat.hpp"

نطاق cv {

#اگر_تعریف_شده HAVE_OPENCL

تعداد { OCL_OP_جمع = 0, OCL_OP_جمع_ABS =  1, OCL_OP_جمع_SQR = 2 };
منطقي ocl_جمع( درون_داد_Array _src, Scalar & res, صحيح جمع_op, درون_داد_Array ماسک_2 = noArray(),
                     درون_داد_Array _src2 = noArray(), منطقي calc2 = خطأ, ثابت Scalar & res2 = Scalar() );
منطقي ocl_minMaxIdx( درون_داد_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, درون_داد_Array ماسک_2,
                           صحيح ddepth = -1, منطقي abs_مقادیر = خطأ, درون_داد_Array _src2 = noArray(), مزدوج * maxVal2 = عدم);

قالب <نام_نوع T> Scalar ocl_part_جمع(Mat m)
{
    CV_Assert(m.ردیفها == 1);

    Scalar s = Scalar::all(0);
    صحيح cn = m.channels();
    ثابت T * ثابت ptr = m.ptr<T>(0);

    لكل (صحيح x = 0, w = m.تعداد_ستون * cn; x < w; )
        لكل (صحيح c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    أرجع s;
}

#پایان_اگر

تعريفنوع صحيح (*جمع_Func)(ثابت uchar*, ثابت uchar* ماسک, uchar*, صحيح, صحيح);
جمع_Func get_جمع_Func(صحيح عمق);

}

#پایان_اگر // SRC_STAT_HPP
