// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#که_نه_تعریف_شوی SRC_STAT_HPP
#تعریف_کړه SRC_STAT_HPP

#ضمن "opencv2/core/mat.hpp"

نطاق cv {

#که_تعریف_شوی HAVE_OPENCL

تعداد { OCL_OP_مجموعه = 0, OCL_OP_مجموعه_ABS =  1, OCL_OP_مجموعه_SQR = 2 };
منطقي ocl_مجموعه( ننوت_Array _src, Scalar & res, صحيح مجموعه_op, ننوت_Array ماسک_2 = noArray(),
                     ننوت_Array _src2 = noArray(), منطقي calc2 = خطأ, ثابت Scalar & res2 = Scalar() );
منطقي ocl_minMaxIdx( ننوت_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, ننوت_Array ماسک_2,
                           صحيح ddepth = -1, منطقي abs_ارزښتونه = خطأ, ننوت_Array _src2 = noArray(), مزدوج * maxVal2 = عدم);

کينډۍ <ډول_نوم T> Scalar ocl_part_مجموعه(Mat m)
{
    CV_Assert(m.لیکې == 1);

    Scalar s = Scalar::all(0);
    صحيح cn = m.channels();
    ثابت T * ثابت ptr = m.ptr<T>(0);

    لكل (صحيح x = 0, w = m.ستون_شمېر * cn; x < w; )
        لكل (صحيح c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    أرجع s;
}

#د_که_پای

تعريفنوع صحيح (*مجموعه_Func)(ثابت uchar*, ثابت uchar* ماسک, uchar*, صحيح, صحيح);
مجموعه_Func get_مجموعه_Func(صحيح ژوروالی);

}

#د_که_پای // SRC_STAT_HPP
