// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SRC_STAT_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SRC_STAT_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/mat.hpp"

ਨਾਮਾਕਾਸ਼ cv {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਗਿਣਤੀ { OCL_OP_ਜੋੜ = 0, OCL_OP_ਜੋੜ_ABS =  1, OCL_OP_ਜੋੜ_SQR = 2 };
ਤਾਰਕਿਕ ocl_ਜੋੜ( ਨਿਵੇਸ਼_Array _src, Scalar & res, ਪੂਰਨ ਜੋੜ_op, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2 = noArray(),
                     ਨਿਵੇਸ਼_Array _src2 = noArray(), ਤਾਰਕਿਕ calc2 = ਝੂਠ, ਅਚਲ Scalar & res2 = Scalar() );
ਤਾਰਕਿਕ ocl_minMaxIdx( ਨਿਵੇਸ਼_Array _src, ਦੁੱਗਣਾ* minVal, ਦੁੱਗਣਾ* maxVal, ਪੂਰਨ* minLoc, ਪੂਰਨ* maxLoc, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2,
                           ਪੂਰਨ ddepth = -1, ਤਾਰਕਿਕ abs_ਮੁੱਲਾਂ = ਝੂਠ, ਨਿਵੇਸ਼_Array _src2 = noArray(), ਦੁੱਗਣਾ * maxVal2 = ਨਿਰਾ);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> Scalar ocl_part_ਜੋੜ(Mat m)
{
    CV_Assert(m.ਪੰਕਤੀਆਂ == 1);

    Scalar s = Scalar::all(0);
    ਪੂਰਨ cn = m.channels();
    ਅਚਲ T * ਅਚਲ ptr = m.ptr<T>(0);

    ਲਈ (ਪੂਰਨ x = 0, w = m.ਖਾਨੇ_ਗਿਣਤੀ * cn; x < w; )
        ਲਈ (ਪੂਰਨ c = 0; c < cn; ++c, ++x)
            s[c] += ptr[x];

    ਵਾਪਸ s;
}

#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*ਜੋੜ_Func)(ਅਚਲ uchar*, ਅਚਲ uchar* ਨਕਾਬ, uchar*, ਪੂਰਨ, ਪੂਰਨ);
ਜੋੜ_Func get_ਜੋੜ_Func(ਪੂਰਨ ਡੂੰਘਾਈ);

}

#ਜੇ_ਅੰਤ // SRC_STAT_HPP
