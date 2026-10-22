// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#جے_بنا_تعریف SRC_STAT_HPP
#تعریف_کرو SRC_STAT_HPP

#ضمن "opencv2/core/mat.hpp"

نطاق cv {

#جے_تعریف_شدہ HAVE_OPENCL

تعداد { OCL_OP_جوڑ = 0, OCL_OP_جوڑ_ABS =  1, OCL_OP_جوڑ_SQR = 2 };
منطقي ocl_جوڑ( داخلہ_Array _src, Scalar & res, صحيح جوڑ_op, داخلہ_Array نقاب_2 = noArray(),
                     داخلہ_Array _src2 = noArray(), منطقي calc2 = خطأ, ثابت Scalar & res2 = Scalar() );
منطقي ocl_minMaxIdx( داخلہ_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, داخلہ_Array نقاب_2,
                           صحيح ddepth = -1, منطقي abs_اقدار = خطأ, داخلہ_Array _src2 = noArray(), مزدوج * maxVal2 = عدم);

سانچا <قسم_ناں T> Scalar ocl_part_جوڑ(Mat m)
{
    CV_Assert(m.صفاں == 1);

    Scalar s = Scalar::all(0);
    صحيح cn = m.channels();
    ثابت T * ثابت ptr = m.ptr<T>(0);

    لكل (صحيح x = 0, w = m.خانے_گنتی * cn; x < w; )
        لكل (صحيح c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    أرجع s;
}

#جے_انت

تعريفنوع صحيح (*جوڑ_Func)(ثابت uchar*, ثابت uchar* نقاب, uchar*, صحيح, صحيح);
جوڑ_Func get_جوڑ_Func(صحيح ڈونگھائی);

}

#جے_انت // SRC_STAT_HPP
