// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#جيڪڏهن_وصف_نه_ٿيل SRC_STAT_HPP
#وصف_ڪريو SRC_STAT_HPP

#ضمن "opencv2/core/mat.hpp"

نطاق cv {

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

تعداد { OCL_OP_جوڙ = 0, OCL_OP_جوڙ_ABS =  1, OCL_OP_جوڙ_SQR = 2 };
منطقي ocl_جوڙ( داخلا_Array _src, Scalar & res, صحيح جوڙ_op, داخلا_Array نقاب_2 = noArray(),
                     داخلا_Array _src2 = noArray(), منطقي calc2 = خطأ, ثابت Scalar & res2 = Scalar() );
منطقي ocl_minMaxIdx( داخلا_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, داخلا_Array نقاب_2,
                           صحيح ddepth = -1, منطقي abs_قدرون = خطأ, داخلا_Array _src2 = noArray(), مزدوج * maxVal2 = عدم);

سانچو <قسم_نالو T> Scalar ocl_part_جوڙ(Mat m)
{
    CV_Assert(m.صفون == 1);

    Scalar s = Scalar::all(0);
    صحيح cn = m.channels();
    ثابت T * ثابت ptr = m.ptr<T>(0);

    لكل (صحيح x = 0, w = m.کانن_ڳڻپ * cn; x < w; )
        لكل (صحيح c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    أرجع s;
}

#جيڪڏهن_پورو

تعريفنوع صحيح (*جوڙ_Func)(ثابت uchar*, ثابت uchar* نقاب, uchar*, صحيح, صحيح);
جوڙ_Func get_جوڙ_Func(صحيح اونهائي);

}

#جيڪڏهن_پورو // SRC_STAT_HPP
