/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Copyright (C) 2014-2015, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <atomic>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits>
#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>
#ਅਨ੍ਤਰ੍ਭਾਵ "mathfuncs.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

ਨਾਮਾਕਾਸ਼ cv
{

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*MathFunc)(ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ* dst, ਪੂਰਨ len);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਗਿਣਤੀ { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

ਸਥਿਰ ਅਚਲ ਅੱਖਰ* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

ਸਥਿਰ ਤਾਰਕਿਕ ocl_math_op(ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst, ਪੂਰਨ oclop)
{
    ਪੂਰਨ type = _src1.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type);
    ਪੂਰਨ kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src1, _src2, _dst);

    ਅਚਲ ocl::Device d = ocl::Device::getDefault();
    ਤਾਰਕਿਕ double_support = d.doubleFPConfig() > 0;
    ਜੇ (!double_support && ਡੂੰਘਾਈ == CV_64F)
        ਵਾਪਸ ਝੂਠ;
    ਪੂਰਨ ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)), ਡੂੰਘਾਈ, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.create_Same_ਮਾਪ(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src1),
            src2arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ਜੇ (src2.empty())
        k.ਦਲੀਲਾਂ(src1arg, dstarg);
    ਨਹੀਂਤਾਂ
        k.ਦਲੀਲਾਂ(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)src1.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, 0, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN ਦਸ਼ਮਲਵ cube_ਜੜ੍ਹ( ਦਸ਼ਮਲਵ value )
{
    CV_INSTRUMENT_REGION();

    ਦਸ਼ਮਲਵ fr;
    Cv32suf v, m;
    ਪੂਰਨ ix, s;
    ਪੂਰਨ ex, shx;

    v.f = value;
    ix = v.i & 0x7fffffff;
    s = v.i & 0x80000000;
    ex = (ix >> 23) - 127;
    shx = ex % 3;
    shx -= shx >= 0 ? 3 : 0;
    ex = (ex - shx) / 3; /* exponent of cube root */
    v.i = (ix & ((1<<23)-1)) | ((shx + 127)<<23);
    fr = v.f;

    /* 0.125 <= fr < 1.0 */
    /* Use quartic rational polynomial with error < 2^(-24) */
    fr = (ਦਸ਼ਮਲਵ)(((((45.2548339756803022511987494 * fr +
    192.2798368355061050458134625) * fr +
    119.1654824285581628956914143) * fr +
    13.43250139086239872172837314) * fr +
    0.1636161226585754240958355063)/
    ((((14.80884093219134573786480845 * fr +
    151.9714051044435648658557668) * fr +
    168.5254414101568283957668343) * fr +
    33.9905941350215598754191872) * fr +
    1.0));

    /* fr *= 2^ex * sign */
    m.f = value;
    v.f = fr;
    v.i = (v.i + (ex << 23) + s) & (m.i*2 != 0 ? -1 : 0);
    ਵਾਪਸ v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

ਖਾਲੀ magnitude( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ type = src1.type(), ਡੂੰਘਾਈ = src1.ਡੂੰਘਾਈ(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F));

    CV_OCL_ਲੜੀਵਾਰ(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ਅਚਲ Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ len = (ਪੂਰਨ)it.size*cn;

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਜੇ( ਡੂੰਘਾਈ == CV_32F )
        {
            ਅਚਲ ਦਸ਼ਮਲਵ *x = (ਅਚਲ ਦਸ਼ਮਲਵ*)ptrs[0], *y = (ਅਚਲ ਦਸ਼ਮਲਵ*)ptrs[1];
            ਦਸ਼ਮਲਵ *mag = (ਦਸ਼ਮਲਵ*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        ਨਹੀਂਤਾਂ
        {
            ਅਚਲ ਦੁੱਗਣਾ *x = (ਅਚਲ ਦੁੱਗਣਾ*)ptrs[0], *y = (ਅਚਲ ਦੁੱਗਣਾ*)ptrs[1];
            ਦੁੱਗਣਾ *mag = (ਦੁੱਗਣਾ*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

ਖਾਲੀ phase( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ type = src1.type(), ਡੂੰਘਾਈ = src1.ਡੂੰਘਾਈ(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F));

    CV_OCL_ਲੜੀਵਾਰ(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angle_ਵਿੱਚ_Degrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ਅਚਲ Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ j, ਕੁੱਲ = (ਪੂਰਨ)(it.size*cn), ਖੰਡ_ਮਾਪ_2 = ਕੁੱਲ;
    size_t esz1 = X.elemSize1();
    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( j = 0; j < ਕੁੱਲ; j += ਖੰਡ_ਮਾਪ_2 )
        {
            ਪੂਰਨ len = std::min(ਕੁੱਲ - j, ਖੰਡ_ਮਾਪ_2);
            ਜੇ( ਡੂੰਘਾਈ == CV_32F )
            {
                ਅਚਲ ਦਸ਼ਮਲਵ *x = (ਅਚਲ ਦਸ਼ਮਲਵ*)ptrs[0], *y = (ਅਚਲ ਦਸ਼ਮਲਵ*)ptrs[1];
                ਦਸ਼ਮਲਵ *angle = (ਦਸ਼ਮਲਵ*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angle_ਵਿੱਚ_Degrees );
            }
            ਨਹੀਂਤਾਂ
            {
                ਅਚਲ ਦੁੱਗਣਾ *x = (ਅਚਲ ਦੁੱਗਣਾ*)ptrs[0], *y = (ਅਚਲ ਦੁੱਗਣਾ*)ptrs[1];
                ਦੁੱਗਣਾ *angle = (ਦੁੱਗਣਾ*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angle_ਵਿੱਚ_Degrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_cart_ਮੰਜ਼ਿਲ_Polar( ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    ਅਚਲ ocl::Device & d = ocl::Device::getDefault();
    ਪੂਰਨ type = _src1.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;

    ਅਚਲ ਤਾਰਕਿਕ _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ਅਚਲ ਤਾਰਕਿਕ _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ਅਚਲ ਤਾਰਕਿਕ _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ਅਚਲ ਤਾਰਕਿਕ _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    ਜੇ ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F) && type == _src2.type()) ||
         (ਡੂੰਘਾਈ == CV_64F && !doubleSupport) )
        ਵਾਪਸ ਝੂਠ;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D OP_CTP_%s%s%s%s%s%s",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, 1)), ਡੂੰਘਾਈ,
                         ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI, angle_ਵਿੱਚ_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstMag   ? " -D SRC1_IS_DST_MAG" : "",
                         _src1IsDstAngle ? " -D SRC1_IS_DST_ANGLE" : "",
                         _src2IsDstMag   ? " -D SRC2_IS_DST_MAG" : "",
                         _src2IsDstAngle ? " -D SRC2_IS_DST_ANGLE" : ""
                         ));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    ਮਾਪ size = src1.size();
    CV_Assert( size == src2.size() );

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.ਦਲੀਲਾਂ(_src1IsDstMag || _src1IsDstAngle ? ocl::KernelArg::Read_Write_No_ਮਾਪ(src1) : ocl::KernelArg::Read_Only_No_ਮਾਪ(src1),
           _src2IsDstMag || _src2IsDstAngle ? ocl::KernelArg::Read_Write_No_ਮਾਪ(src2) : ocl::KernelArg::Read_Only_No_ਮਾਪ(src2),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_ਮਾਪ(dst2));

    size_t globalsize[2] = { (size_t)dst1.ਖਾਨੇ_ਗਿਣਤੀ * cn, ((size_t)dst1.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ cart_ਮੰਜ਼ਿਲ_Polar( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2,
                  OutputArray dst1, OutputArray dst2, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_ਲੜੀਵਾਰ(dst1.isUMat() && dst2.isUMat(),
            ocl_cart_ਮੰਜ਼ਿਲ_Polar(src1, src2, dst1, dst2, angle_ਵਿੱਚ_Degrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    ਪੂਰਨ type = X.type(), ਡੂੰਘਾਈ = X.ਡੂੰਘਾਈ(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ਅਚਲ Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ j, ਕੁੱਲ = (ਪੂਰਨ)(it.size*cn), ਖੰਡ_ਮਾਪ_2 = std::min(ਕੁੱਲ, ((ਖੰਡ_ਮਾਪ+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( j = 0; j < ਕੁੱਲ; j += ਖੰਡ_ਮਾਪ_2 )
        {
            ਪੂਰਨ len = std::min(ਕੁੱਲ - j, ਖੰਡ_ਮਾਪ_2);
            ਜੇ( ਡੂੰਘਾਈ == CV_32F )
            {
                ਅਚਲ ਦਸ਼ਮਲਵ *x = (ਅਚਲ ਦਸ਼ਮਲਵ*)ptrs[0], *y = (ਅਚਲ ਦਸ਼ਮਲਵ*)ptrs[1];
                ਦਸ਼ਮਲਵ *mag = (ਦਸ਼ਮਲਵ*)ptrs[2], *angle = (ਦਸ਼ਮਲਵ*)ptrs[3];
                hal::cart_ਮੰਜ਼ਿਲ_Polar32f( x, y, mag, angle, len, angle_ਵਿੱਚ_Degrees );
            }
            ਨਹੀਂਤਾਂ
            {
                ਅਚਲ ਦੁੱਗਣਾ *x = (ਅਚਲ ਦੁੱਗਣਾ*)ptrs[0], *y = (ਅਚਲ ਦੁੱਗਣਾ*)ptrs[1];
                ਦੁੱਗਣਾ *mag = (ਦੁੱਗਣਾ*)ptrs[2], *angle = (ਦੁੱਗਣਾ*)ptrs[3];
                hal::cart_ਮੰਜ਼ਿਲ_Polar64f(x, y, mag, angle, len, angle_ਵਿੱਚ_Degrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
            ptrs[3] += len*esz1;
        }
    }
}


/****************************************************************************************\
*                                  Polar -> Cartezian                                    *
\****************************************************************************************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_polar_ਮੰਜ਼ਿਲ_Cart( ਨਿਵੇਸ਼_Array _mag, ਨਿਵੇਸ਼_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    ਅਚਲ ocl::Device & d = ocl::Device::getDefault();
    ਪੂਰਨ type = _angle.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;

    ਅਚਲ ਤਾਰਕਿਕ _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ਅਚਲ ਤਾਰਕਿਕ _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ਅਚਲ ਤਾਰਕਿਕ _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ਅਚਲ ਤਾਰਕਿਕ _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    ਜੇ ( !doubleSupport && ਡੂੰਘਾਈ == CV_64F )
        ਵਾਪਸ ਝੂਠ;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D BINARY_OP -D OP_PTC_%s%s%s%s%s%s",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, 1)), ਡੂੰਘਾਈ,
                         ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
                         angle_ਵਿੱਚ_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstX   ? " -D SRC1_IS_DST_X" : "",
                         _src1IsDstY ? " -D SRC1_IS_DST_Y" : "",
                         _src2IsDstX   ? " -D SRC2_IS_DST_X" : "",
                         _src2IsDstY ? " -D SRC2_IS_DST_Y" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat mag = _mag.getUMat(), angle = _angle.getUMat();
    ਮਾਪ size = angle.size();
    CV_Assert(mag.size() == size);

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.ਦਲੀਲਾਂ(_src1IsDstX || _src1IsDstY ? ocl::KernelArg::Read_Write_No_ਮਾਪ(mag) : ocl::KernelArg::Read_Only_No_ਮਾਪ(mag),
           _src2IsDstX || _src2IsDstY  ? ocl::KernelArg::Read_Write_No_ਮਾਪ(angle) : ocl::KernelArg::Read_Only_No_ਮਾਪ(angle),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_ਮਾਪ(dst2));

    size_t globalsize[2] = { (size_t)dst1.ਖਾਨੇ_ਗਿਣਤੀ * cn, ((size_t)dst1.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ polar_ਮੰਜ਼ਿਲ_Cart( ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2,
                  OutputArray dst1, OutputArray dst2, ਤਾਰਕਿਕ angle_ਵਿੱਚ_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    ਪੂਰਨ type = src2.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type);
    CV_Assert((ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_ਲੜੀਵਾਰ(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polar_ਮੰਜ਼ਿਲ_Cart(src1, src2, dst1, dst2, angle_ਵਿੱਚ_Degrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ਅਚਲ Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ j, ਕੁੱਲ = (ਪੂਰਨ)(it.size*cn), ਖੰਡ_ਮਾਪ_2 = std::min(ਕੁੱਲ, ((ਖੰਡ_ਮਾਪ+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( j = 0; j < ਕੁੱਲ; j += ਖੰਡ_ਮਾਪ_2 )
        {
            ਪੂਰਨ len = std::min(ਕੁੱਲ - j, ਖੰਡ_ਮਾਪ_2);
            ਜੇ ( ਡੂੰਘਾਈ == CV_32F )
            {
                ਅਚਲ ਦਸ਼ਮਲਵ *mag = (ਅਚਲ ਦਸ਼ਮਲਵ*)ptrs[0], *angle = (ਅਚਲ ਦਸ਼ਮਲਵ*)ptrs[1];
                ਦਸ਼ਮਲਵ *x = (ਦਸ਼ਮਲਵ*)ptrs[2], *y = (ਦਸ਼ਮਲਵ*)ptrs[3];
                hal::polar_ਮੰਜ਼ਿਲ_Cart32f( mag, angle, x, y, len, angle_ਵਿੱਚ_Degrees );
            }
            ਨਹੀਂਤਾਂ
            {
                ਅਚਲ ਦੁੱਗਣਾ *mag = (ਅਚਲ ਦੁੱਗਣਾ*)ptrs[0], *angle = (ਅਚਲ ਦੁੱਗਣਾ*)ptrs[1];
                ਦੁੱਗਣਾ *x = (ਦੁੱਗਣਾ*)ptrs[2], *y = (ਦੁੱਗਣਾ*)ptrs[3];
                hal::polar_ਮੰਜ਼ਿਲ_Cart64f( mag, angle, x, y, len, angle_ਵਿੱਚ_Degrees );
            }

            ਜੇ( ptrs[0] )
                ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
            ptrs[3] += len*esz1;
        }
    }
}

/****************************************************************************************\
*                                          E X P                                         *
\****************************************************************************************/

// The master function of the unary math family (cv::exp/log/sqrt - the analogue of arithm_op):
// same-shape same-type output over the four float depths, computed by the element-wise engine's
// kernels (math.simd.hpp). Two tiers:
//  - SMALL and continuous (a common pattern - exp() over one image row as a lookup substitute):
//    call the kernel DIRECTLY over the flattened elements. No TExpr, no broadcastOp, no
//    parallel_for machinery - their setup dominates at these sizes.
//  - everything else (large arrays - worth parallelizing; ROIs - need real steps): the usual
//    1-instruction program via compile()/exec().
ਗਿਣਤੀ { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

ਸਥਿਰ ਖਾਲੀ math_op(ew::ਸਿਖਰ op, ਨਿਵੇਸ਼_Array _src, OutputArray _dst)
{
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    CV_Assert(ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF || ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F);

    Mat src = _src.getMat();
    _dst.create_Same_ਮਾਪ(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    ਜੇ (src.empty())
        ਵਾਪਸ;

    ਅਚਲ size_t ਕੁੱਲ = src.ਕੁੱਲ() * src.channels();
    ਜੇ (src.isContinuous() && dst.isContinuous() && ਕੁੱਲ <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, ਡੂੰਘਾਈ);
        CV_Assert(k.fptr);
        ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ noparams[4] = {};
        k.fptr(src.ਅੰਕੜੇ, 0, 1, ਖਾਲੀ_ਸੂਚਕ, 0, 0, ਖਾਲੀ_ਸੂਚਕ, 0, 0,
               dst.ਅੰਕੜੇ, 0, (ਪੂਰਨ)ਕੁੱਲ, 1, noparams, k.flags, k.userdata);
        ਵਾਪਸ;
    }

    ew::TExpr p;
    ਅਚਲ ਪੂਰਨ a = p.ਜੋੜੋ_ਨਿਵੇਸ਼(ਡੂੰਘਾਈ);
    ਅਚਲ ਪੂਰਨ out = p.ਜੋੜੋ_Output(ਡੂੰਘਾਈ);
    p.move_ਮੰਜ਼ਿਲ_Output(p.emitUnary(op, a, ਡੂੰਘਾਈ), out);
    p.compile();
    ਅਚਲ Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

ਖਾਲੀ ਘਾਤਾਂਕ( ਨਿਵੇਸ਼_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ਪੂਰਨ ਡੂੰਘਾਈ = _src.ਡੂੰਘਾਈ();   // consumed by CV_OCL_RUN only

    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat() && _src.dims() <= 2 && (ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

ਖਾਲੀ ਲਘੂਗਣਕ( ਨਿਵੇਸ਼_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ਪੂਰਨ ਡੂੰਘਾਈ = _src.ਡੂੰਘਾਈ();   // consumed by CV_OCL_RUN only

    CV_OCL_ਲੜੀਵਾਰ( _dst.isUMat() && _src.dims() <= 2 && (ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT>
ਬਣਤਰ iPow_SIMD
{
    ਪੂਰਨ ਸੰਚਾਲਕ() ( ਅਚਲ T *, T *, ਪੂਰਨ, ਪੂਰਨ)
    {
        ਵਾਪਸ 0;
    }
};

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

ਸਾਂਚਾ <>
ਬਣਤਰ iPow_SIMD<uchar, ਬਿਨਾਨਿਸ਼ਾਨੀ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ() ( ਅਚਲ uchar * src, uchar * dst, ਪੂਰਨ len, ਪੂਰਨ power )
    {
        ਪੂਰਨ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ਲਈ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_ਲੱਦੋ_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ਪੂਰਨ p = power;

            ਜਦੋਂ( p > 1 )
            {
                ਜੇ (p & 1)
                {
                    v_a1 = v_mul(v_a1, v_b1);
                    v_a2 = v_mul(v_a2, v_b2);
                }
                v_b1 = v_mul(v_b1, v_b1);
                v_b2 = v_mul(v_b2, v_b2);
                p >>= 1;
            }

            v_a1 = v_mul(v_a1, v_b1);
            v_a2 = v_mul(v_a2, v_b2);

            v = v_pack(v_a1, v_a2);
            v_pack_store(dst + i, v);
        }
        vx_cleanup();

        ਵਾਪਸ i;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ iPow_SIMD<schar, ਪੂਰਨ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ() ( ਅਚਲ schar * src, schar * dst, ਪੂਰਨ len, ਪੂਰਨ power)
    {
        ਪੂਰਨ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ਲਈ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_ਲੱਦੋ_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ਪੂਰਨ p = power;

            ਜਦੋਂ( p > 1 )
            {
                ਜੇ (p & 1)
                {
                    v_a1 = v_mul(v_a1, v_b1);
                    v_a2 = v_mul(v_a2, v_b2);
                }
                v_b1 = v_mul(v_b1, v_b1);
                v_b2 = v_mul(v_b2, v_b2);
                p >>= 1;
            }

            v_a1 = v_mul(v_a1, v_b1);
            v_a2 = v_mul(v_a2, v_b2);

            v = v_pack(v_a1, v_a2);
            v_pack_store(dst + i, v);
        }
        vx_cleanup();

        ਵਾਪਸ i;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ iPow_SIMD<ushort, ਬਿਨਾਨਿਸ਼ਾਨੀ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ() ( ਅਚਲ ushort * src, ushort * dst, ਪੂਰਨ len, ਪੂਰਨ power)
    {
        ਪੂਰਨ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ਲਈ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_ਲੱਦੋ(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ਪੂਰਨ p = power;

            ਜਦੋਂ( p > 1 )
            {
                ਜੇ (p & 1)
                {
                    v_a1 = v_mul(v_a1, v_b1);
                    v_a2 = v_mul(v_a2, v_b2);
                }
                v_b1 = v_mul(v_b1, v_b1);
                v_b2 = v_mul(v_b2, v_b2);
                p >>= 1;
            }

            v_a1 = v_mul(v_a1, v_b1);
            v_a2 = v_mul(v_a2, v_b2);

            v = v_pack(v_a1, v_a2);
            v_store(dst + i, v);
        }
        vx_cleanup();

        ਵਾਪਸ i;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ iPow_SIMD<ਛੋਟਾ, ਪੂਰਨ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ() ( ਅਚਲ ਛੋਟਾ * src, ਛੋਟਾ * dst, ਪੂਰਨ len, ਪੂਰਨ power)
    {
        ਪੂਰਨ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ਲਈ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_ਲੱਦੋ(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ਪੂਰਨ p = power;

            ਜਦੋਂ( p > 1 )
            {
                ਜੇ (p & 1)
                {
                    v_a1 = v_mul(v_a1, v_b1);
                    v_a2 = v_mul(v_a2, v_b2);
                }
                v_b1 = v_mul(v_b1, v_b1);
                v_b2 = v_mul(v_b2, v_b2);
                p >>= 1;
            }

            v_a1 = v_mul(v_a1, v_b1);
            v_a2 = v_mul(v_a2, v_b2);

            v = v_pack(v_a1, v_a2);
            v_store(dst + i, v);
        }
        vx_cleanup();

        ਵਾਪਸ i;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ iPow_SIMD<ਪੂਰਨ, ਪੂਰਨ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ() ( ਅਚਲ ਪੂਰਨ * src, ਪੂਰਨ * dst, ਪੂਰਨ len, ਪੂਰਨ power)
    {
        ਪੂਰਨ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ਲਈ ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_ਲੱਦੋ(src + i), v_b2 = vx_ਲੱਦੋ(src + i + VTraits<v_int32>::vlanes());
            ਪੂਰਨ p = power;

            ਜਦੋਂ( p > 1 )
            {
                ਜੇ (p & 1)
                {
                    v_a1 = v_mul(v_a1, v_b1);
                    v_a2 = v_mul(v_a2, v_b2);
                }
                v_b1 = v_mul(v_b1, v_b1);
                v_b2 = v_mul(v_b2, v_b2);
                p >>= 1;
            }

            v_a1 = v_mul(v_a1, v_b1);
            v_a2 = v_mul(v_a2, v_b2);

            v_store(dst + i, v_a1);
            v_store(dst + i + VTraits<v_int32>::vlanes(), v_a2);
        }
        vx_cleanup();

        ਵਾਪਸ i;
    }
};

ਸਾਂਚਾ <>
ਬਣਤਰ iPow_SIMD<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ() ( ਅਚਲ ਦਸ਼ਮਲਵ * src, ਦਸ਼ਮਲਵ * dst, ਪੂਰਨ len, ਪੂਰਨ power)
    {
        ਪੂਰਨ i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        ਲਈ ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_ਲੱਦੋ(src + i), v_b2 = vx_ਲੱਦੋ(src + i + VTraits<v_float32>::vlanes());
            ਪੂਰਨ p = std::ਨਿਰਪੇਖ(power);
            ਜੇ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ਜਦੋਂ( p > 1 )
            {
                ਜੇ (p & 1)
                {
                    v_a1 = v_mul(v_a1, v_b1);
                    v_a2 = v_mul(v_a2, v_b2);
                }
                v_b1 = v_mul(v_b1, v_b1);
                v_b2 = v_mul(v_b2, v_b2);
                p >>= 1;
            }

            v_a1 = v_mul(v_a1, v_b1);
            v_a2 = v_mul(v_a2, v_b2);

            v_store(dst + i, v_a1);
            v_store(dst + i + VTraits<v_float32>::vlanes(), v_a2);
        }
        vx_cleanup();

        ਵਾਪਸ i;
    }
};

#ਜੇ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ਸਾਂਚਾ <>
ਬਣਤਰ iPow_SIMD<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>
{
    ਪੂਰਨ ਸੰਚਾਲਕ() ( ਅਚਲ ਦੁੱਗਣਾ * src, ਦੁੱਗਣਾ * dst, ਪੂਰਨ len, ਪੂਰਨ power)
    {
        ਪੂਰਨ i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        ਲਈ ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_ਲੱਦੋ(src + i), v_b2 = vx_ਲੱਦੋ(src + i + VTraits<v_float64>::vlanes());
            ਪੂਰਨ p = std::ਨਿਰਪੇਖ(power);
            ਜੇ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ਜਦੋਂ( p > 1 )
            {
                ਜੇ (p & 1)
                {
                    v_a1 = v_mul(v_a1, v_b1);
                    v_a2 = v_mul(v_a2, v_b2);
                }
                v_b1 = v_mul(v_b1, v_b1);
                v_b2 = v_mul(v_b2, v_b2);
                p >>= 1;
            }

            v_a1 = v_mul(v_a1, v_b1);
            v_a2 = v_mul(v_a2, v_b2);

            v_store(dst + i, v_a1);
            v_store(dst + i + VTraits<v_float64>::vlanes(), v_a2);
        }
        vx_cleanup();

        ਵਾਪਸ i;
    }
};
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT>
ਸਥਿਰ ਖਾਲੀ
iPow_i( ਅਚਲ ਖਾਲੀ* _src, ਖਾਲੀ* _dst, ਪੂਰਨ len, ਪੂਰਨ power )
{
    ਅਚਲ T* src = (ਅਚਲ T*)_src;
    T* dst = (T*)_dst;
    ਜੇ( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        ਲਈ( ਪੂਰਨ i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        iPow_SIMD<T, WT> vop;
        ਪੂਰਨ i = vop(src, dst, len, power);

        ਲਈ( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            ਪੂਰਨ p = power;
            ਜਦੋਂ( p > 1 )
            {
                ਜੇ( p & 1 )
                    a *= b;
                b *= b;
                p >>= 1;
            }

            a *= b;
            dst[i] = saturate_cast<T>(a);
        }
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਖਾਲੀ
iPow_f( ਅਚਲ ਖਾਲੀ* _src, ਖਾਲੀ* _dst, ਪੂਰਨ len, ਪੂਰਨ power0 )
{
    ਅਚਲ T* src = (ਅਚਲ T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    ਪੂਰਨ i = vop(src, dst, len, power0);
    ਪੂਰਨ power = std::ਨਿਰਪੇਖ(power0);

    ਲਈ( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        ਪੂਰਨ p = power;
        ਜੇ( power0 < 0 )
            b = 1/b;

        ਜਦੋਂ( p > 1 )
        {
            ਜੇ( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*IPowFunc)( ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ* dst, ਪੂਰਨ len, ਪੂਰਨ power );

ਸਥਿਰ IPowFunc ipowTab[CV_ਡੂੰਘਾਈ_MAX] =
{
    iPow_i<uchar, ਬਿਨਾਨਿਸ਼ਾਨੀ>, iPow_i<schar, ਪੂਰਨ>, iPow_i<ushort, ਬਿਨਾਨਿਸ਼ਾਨੀ>, iPow_i<ਛੋਟਾ, ਪੂਰਨ>,
    iPow_i<ਪੂਰਨ, ਪੂਰਨ>, iPow_f<ਦਸ਼ਮਲਵ>, iPow_f<ਦੁੱਗਣਾ>, 0
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_pow(ਨਿਵੇਸ਼_Array _src, ਦੁੱਗਣਾ power, OutputArray _dst,
                    ਤਾਰਕਿਕ is_ipower, ਪੂਰਨ ipower)
{
    ਅਚਲ ocl::Device & d = ocl::Device::getDefault();
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;

    _dst.create_Same_ਮਾਪ(_src, type);
    ਜੇ (is_ipower)
    {
        ਜੇ( ipower < 0 )
        {
            ਜੇ( ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F )
                is_ipower = ਝੂਠ;
            ਨਹੀਂਤਾਂ
                ਵਾਪਸ ਝੂਠ;
        }
    }

    ਜੇ (ਡੂੰਘਾਈ == CV_64F && !doubleSupport)
        ਵਾਪਸ ਝੂਠ;

    ਤਾਰਕਿਕ issqrt = std::ਨਿਰਪੇਖ(power - 0.5) < DBL_EPSILON;
    ਅਚਲ ਅੱਖਰ * ਅਚਲ op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    ਜੇ (is_ipower)
    {
        ਪੂਰਨ wdepth = CV_32F;
        ਜੇ (ਡੂੰਘਾਈ == CV_64F)
            wdepth = CV_64F;
        ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == CV_16F)
            wdepth = CV_16F;

        ਅੱਖਰ cvt[2][50];
        extra_opts = format(
            " -D srcT1=%s -DsrcT1_C1=%s"
            " -D srcT2=int -D workST=int"
            " -D workT=%s -D wdepth=%d -D convertToWT1=%s"
            " -D convertToDT=%s"
            " -D workT1=%s",
            ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, 1)),
            ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, 1)),
            ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(wdepth, 1)),
            wdepth,
            ocl::convertTypeStr(ਡੂੰਘਾਈ, wdepth, 1, cvt[0], ਆਕਾਰ(cvt[0])),
            ocl::convertTypeStr(wdepth, ਡੂੰਘਾਈ, 1, cvt[1], ਆਕਾਰ(cvt[1])),
            ocl::type_ਮੰਜ਼ਿਲ_Str(wdepth)
        );
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D cn=%d -D dstT=%s -D dstT_C1=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D %s%s%s%s",
                         1,
                         ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), ਡੂੰਘਾਈ, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI, op,
                         " -D UNARY_OP=1",
                         extra_opts.empty() ? "" : extra_opts.c_str(),
                         doubleSupport ? " -D DOUBLE_SUPPORT" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat();
    _dst.create_Same_ਮਾਪ(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    ਜੇ (issqrt)
        k.ਦਲੀਲਾਂ(srcarg, dstarg);
    ਨਹੀਂਤਾਂ ਜੇ (is_ipower)
        k.ਦਲੀਲਾਂ(srcarg, dstarg, ipower);
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (ਡੂੰਘਾਈ == CV_32F)
            k.ਦਲੀਲਾਂ(srcarg, dstarg, (ਦਸ਼ਮਲਵ)power);
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.ਖਾਨੇ_ਗਿਣਤੀ *  cn, ((size_t)dst.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ ਘਾਤ( ਨਿਵੇਸ਼_Array _src, ਦੁੱਗਣਾ power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type),
            cn = CV_MAT_CN(type), ipower = cv_ਗੇੜਾ(power);
    ਤਾਰਕਿਕ is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਤਾਰਕਿਕ useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#ਜੇ_ਅੰਤ

    ਜੇ (is_ipower)
    {
        ਚੋਣ( ipower )
        {
        ਹਾਲਤ 0:
            _dst.create_Same_ਮਾਪ(_src, type);
            _dst.ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar::all(1));
            ਵਾਪਸ;
        ਹਾਲਤ 1:
            _src.copy_ਮੰਜ਼ਿਲ(_dst);
            ਵਾਪਸ;
        ਹਾਲਤ 2:
            multiply(_src, _src, _dst);
            ਵਾਪਸ;
        }
    }

    CV_OCL_ਲੜੀਵਾਰ(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ਅਚਲ ਤਾਰਕਿਕ float_ਡੂੰਘਾਈ = ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF || ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    ਜੇ( is_ipower && !float_ਡੂੰਘਾਈ && ipowTab[ਡੂੰਘਾਈ] )
    {
        Mat src = _src.getMat();
        _dst.create_Same_ਮਾਪ(_src, type);
        Mat dst = _dst.getMat();

        ਅਚਲ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ਪੂਰਨ len = (ਪੂਰਨ)(it.size*cn);
        IPowFunc func = ipowTab[ਡੂੰਘਾਈ];

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        ਵਾਪਸ;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.create_Same_ਮਾਪ(_src, type);
    Mat dst = _dst.getMat();
    ਜੇ (src.empty())
        ਵਾਪਸ;

    ਅਚਲ size_t ਕੁੱਲ = src.ਕੁੱਲ() * cn;
    ਜੇ (float_ਡੂੰਘਾਈ && src.isContinuous() && dst.isContinuous() && ਕੁੱਲ <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(ਡੂੰਘਾਈ, ਡੂੰਘਾਈ);
        ਜੇ (k.fptr)
        {
            ਦੁੱਗਣਾ pvstore;                    // the broadcast exponent, stored as T
            ਖਾਲੀ* pv = &pvstore;
            ਚੋਣ (ਡੂੰਘਾਈ)
            {
            ਹਾਲਤ CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); ਰੋਕੋ;
            ਹਾਲਤ CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); ਰੋਕੋ;
            ਹਾਲਤ CV_32F:  *(ਦਸ਼ਮਲਵ*)pv = (ਦਸ਼ਮਲਵ)power; ਰੋਕੋ;
            ਮੂਲ:      pvstore = power; ਰੋਕੋ;
            }
            ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ noparams[4] = {};
            k.fptr(src.ਅੰਕੜੇ, 0, 1, pv, 0, 0, ਖਾਲੀ_ਸੂਚਕ, 0, 0,
                   dst.ਅੰਕੜੇ, 0, (ਪੂਰਨ)ਕੁੱਲ, 1, noparams, k.flags, k.userdata);
            ਵਾਪਸ;
        }
    }

    ew::TExpr prog;
    ਅਚਲ ਪੂਰਨ a = prog.ਜੋੜੋ_ਨਿਵੇਸ਼(ਡੂੰਘਾਈ);
    ਅਚਲ ਪੂਰਨ c = prog.ਜੋੜੋ_Const(ew::EW_ਡੂੰਘਾਈ_NONE, Scalar(power), 1);
    ਅਚਲ ਪੂਰਨ out = prog.ਜੋੜੋ_Output(ਡੂੰਘਾਈ);
    prog.move_ਮੰਜ਼ਿਲ_Output(prog.emit_ਦੁਆਧਾਰੀ(ew::OP_POW, a, c, ਡੂੰਘਾਈ), out);
    prog.compile();
    ਅਚਲ Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

ਖਾਲੀ ਵਰਗਮੂਲ(ਨਿਵੇਸ਼_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    ਜੇ (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::ਘਾਤ(a, 0.5, b);
        ਵਾਪਸ;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

ਸਾਂਚਾ<ਪੂਰਨ cv_mat_type> ਬਣਤਰ mat_type_assotiations{};

ਸਾਂਚਾ<> ਬਣਤਰ mat_type_assotiations<CV_8U>
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ type;
    ਸਥਿਰ ਅਚਲ type min_allowable = 0x0;
    ਸਥਿਰ ਅਚਲ type max_allowable = 0xFF;
};

ਸਾਂਚਾ<> ਬਣਤਰ mat_type_assotiations<CV_8S>
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ type;
    ਸਥਿਰ ਅਚਲ type min_allowable = SCHAR_MIN;
    ਸਥਿਰ ਅਚਲ type max_allowable = SCHAR_MAX;
};

ਸਾਂਚਾ<> ਬਣਤਰ mat_type_assotiations<CV_16U>
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ type;
    ਸਥਿਰ ਅਚਲ type min_allowable = 0x0;
    ਸਥਿਰ ਅਚਲ type max_allowable = USHRT_MAX;
};
ਸਾਂਚਾ<> ਬਣਤਰ mat_type_assotiations<CV_16S>
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਨਿਸ਼ਾਨੀਵਾਲਾ ਛੋਟਾ type;
    ਸਥਿਰ ਅਚਲ type min_allowable = SHRT_MIN;
    ਸਥਿਰ ਅਚਲ type max_allowable = SHRT_MAX;
};

ਸਾਂਚਾ<> ਬਣਤਰ mat_type_assotiations<CV_32S>
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ type;
    ਸਥਿਰ ਅਚਲ type min_allowable = (-ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ - 1);
    ਸਥਿਰ ਅਚਲ type max_allowable = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
};

ਸਾਂਚਾ<ਪੂਰਨ ਡੂੰਘਾਈ>
ਸਥਿਰ ਤਾਰਕਿਕ checkIntegerRange(cv::Mat src, ਬਿੰਦੂ& bad_pt, ਪੂਰਨ minVal, ਪੂਰਨ maxVal)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ mat_type_assotiations<ਡੂੰਘਾਈ> type_ass;

    ਜੇ (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        ਵਾਪਸ ਸੱਚ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::ਬਿੰਦੂ(0,0);
        ਵਾਪਸ ਝੂਠ;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    ਲਈ (ਪੂਰਨ j = 0; j < as_one_channel.ਪੰਕਤੀਆਂ; ++j)
        ਲਈ (ਪੂਰਨ i = 0; i < as_one_channel.ਖਾਨੇ_ਗਿਣਤੀ; ++i)
        {
            ਕਿਸਮ_ਨਾਂ type_ass::type v = as_one_channel.at<ਕਿਸਮ_ਨਾਂ type_ass::type>(j ,i);
            ਜੇ (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                ਵਾਪਸ ਝੂਠ;
            }
        }

    ਵਾਪਸ ਸੱਚ;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਤਾਰਕਿਕ (*check_range_function)(cv::Mat src, ਬਿੰਦੂ& bad_pt, ਪੂਰਨ minVal, ਪੂਰਨ maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਖਾਲੀ checkTypedRange(Mat& src, ਬਿੰਦੂ& badPt, ਦੁੱਗਣਾ minVal, ਦੁੱਗਣਾ maxVal)
{
    ਪੂਰਨ loc = 0;
    ਪੂਰਨ cn = src.channels();
    ਮਾਪ size = getContinuousSize2D(src, cn);
    ਅਚਲ T* isrc = src.ptr<T>();
    size_t ਕਦਮ = src.ਕਦਮ / ਆਕਾਰ(isrc[0]);

    ਲਈ( ; badPt.x < 0 && size.ਉਚਾਈ--; loc += size.ਚੌੜਾਈ, isrc += ਕਦਮ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < size.ਚੌੜਾਈ; i++ )
        {
            ਦੁੱਗਣਾ val = (ਦੁੱਗਣਾ)isrc[i];
            ਜੇ( !(val >= minVal && val < maxVal) )
            {
                ਪੂਰਨ pixelId = (loc + i) / cn;
                badPt = ਬਿੰਦੂ(pixelId % src.ਖਾਨੇ_ਗਿਣਤੀ, pixelId / src.ਖਾਨੇ_ਗਿਣਤੀ);
                ਰੋਕੋ;
            }
        }
    }
}

ਤਾਰਕਿਕ checkRange(ਨਿਵੇਸ਼_Array _src, ਤਾਰਕਿਕ quiet, ਬਿੰਦੂ* pt, ਦੁੱਗਣਾ minVal, ਦੁੱਗਣਾ maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    ਜੇ ( src.dims > 2 )
    {
        CV_Assert(pt == ਨਿਰਾ); // no way to provide location info

        ਅਚਲ Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        ਲਈ ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ਜੇ (!checkRange( it.planes[0], quiet, ਨਿਰਾ, minVal, maxVal ))
            {
                ਵਾਪਸ ਝੂਠ;
            }
        }
        ਵਾਪਸ ਸੱਚ;
    }

    ਪੂਰਨ ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();
    ਬਿੰਦੂ badPt(-1, -1);

    ਜੇ (ਡੂੰਘਾਈ < CV_32F)
    {
        ਪੂਰਨ minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        ਪੂਰਨ maxVali = maxVal > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ ? ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ : cvCeil(maxVal) - 1;

        (check_range_functions[ਡੂੰਘਾਈ])(src, badPt, minVali, maxVali);
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ i, loc = 0;
        ਪੂਰਨ cn = src.channels();
        ਮਾਪ size = getContinuousSize2D(src, cn);

        ਜੇ( ਡੂੰਘਾਈ == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_32F )
        {
            Cv32suf a, b;
            ਪੂਰਨ ia, ib;
            ਅਚਲ ਪੂਰਨ* isrc = src.ptr<ਪੂਰਨ>();
            size_t ਕਦਮ = src.ਕਦਮ/ਆਕਾਰ(isrc[0]);

            a.f = (ਦਸ਼ਮਲਵ)std::max(minVal, (ਦੁੱਗਣਾ)-FLT_MAX);
            b.f = (ਦਸ਼ਮਲਵ)std::min(maxVal, (ਦੁੱਗਣਾ)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            ਲਈ( ; badPt.x < 0 && size.ਉਚਾਈ--; loc += size.ਚੌੜਾਈ, isrc += ਕਦਮ )
            {
                ਲਈ( i = 0; i < size.ਚੌੜਾਈ; i++ )
                {
                    ਪੂਰਨ val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    ਜੇ( val < ia || val >= ib )
                    {
                        ਪੂਰਨ pixelId = (loc + i) / cn;
                        badPt = ਬਿੰਦੂ(pixelId % src.ਖਾਨੇ_ਗਿਣਤੀ, pixelId / src.ਖਾਨੇ_ਗਿਣਤੀ);
                        ਰੋਕੋ;
                    }
                }
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ਅਚਲ int64* isrc = src.ptr<int64>();
            size_t ਕਦਮ = src.ਕਦਮ/ਆਕਾਰ(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            ਲਈ( ; badPt.x < 0 && size.ਉਚਾਈ--; loc += size.ਚੌੜਾਈ, isrc += ਕਦਮ )
            {
                ਲਈ( i = 0; i < size.ਚੌੜਾਈ; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    ਜੇ( val < ia || val >= ib )
                    {
                        ਪੂਰਨ pixelId = (loc + i) / cn;
                        badPt = ਬਿੰਦੂ(pixelId % src.ਖਾਨੇ_ਗਿਣਤੀ, pixelId / src.ਖਾਨੇ_ਗਿਣਤੀ);
                        ਰੋਕੋ;
                    }
                }
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        ਨਹੀਂਤਾਂ
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    ਜੇ( badPt.x >= 0 )
    {
        ਜੇ( pt )
            *pt = badPt;
        ਜੇ( !quiet )
        {
            cv::String ਮੁੱਲ_str;
            ਮੁੱਲ_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::Sts_ਬਾਹਰਲਾ_ਦਾ_Range,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, ਮੁੱਲ_str.c_str(), minVal, maxVal));
        }
        ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
}

} // namespace cv

/*
  Finds real roots of cubic, quadratic or linear equation.
  The original code has been taken from Ken Turkowski web page
  (http://www.worldserver.com/turk/opensource/) and adopted for OpenCV.
  Here is the copyright notice.

  -----------------------------------------------------------------------
  Copyright (C) 1978-1999 Ken Turkowski. <turk@computer.org>

    All rights reserved.

    Warranty Information
      Even though I have reviewed this software, I make no warranty
      or representation, either express or implied, with respect to this
      software, its quality, accuracy, merchantability, or fitness for a
      particular purpose.  As a result, this software is provided "as is,"
      and you, its user, are assuming the entire risk as to its quality
      and accuracy.

    This code may be used and freely distributed as long as it includes
    this copyright notice and the above warranty information.
  -----------------------------------------------------------------------
*/

ਪੂਰਨ cv::solveCubic( ਨਿਵੇਸ਼_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ਅਚਲ ਪੂਰਨ n0 = 3;
    Mat coeffs = _coeffs.getMat();
    ਪੂਰਨ ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == ਮਾਪ(n0, 1) ||
                coeffs.size() == ਮਾਪ(n0+1, 1) ||
                coeffs.size() == ਮਾਪ(1, n0) ||
                coeffs.size() == ਮਾਪ(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, ਸੱਚ, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ_FLT);
    Mat roots = _roots.getMat();

    ਪੂਰਨ i = -1, n = 0;
    ਦੁੱਗਣਾ a0 = 1., a1, a2, a3;
    ਦੁੱਗਣਾ x0 = 0., x1 = 0., x2 = 0.;
    ਪੂਰਨ ncoeffs = coeffs.ਪੰਕਤੀਆਂ + coeffs.ਖਾਨੇ_ਗਿਣਤੀ - 1;

    ਜੇ( ctype == CV_32FC1 )
    {
        ਜੇ( ncoeffs == 4 )
            a0 = coeffs.at<ਦਸ਼ਮਲਵ>(++i);

        a1 = coeffs.at<ਦਸ਼ਮਲਵ>(i+1);
        a2 = coeffs.at<ਦਸ਼ਮਲਵ>(i+2);
        a3 = coeffs.at<ਦਸ਼ਮਲਵ>(i+3);
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ( ncoeffs == 4 )
            a0 = coeffs.at<ਦੁੱਗਣਾ>(++i);

        a1 = coeffs.at<ਦੁੱਗਣਾ>(i+1);
        a2 = coeffs.at<ਦੁੱਗਣਾ>(i+2);
        a3 = coeffs.at<ਦੁੱਗਣਾ>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    ਦੁੱਗਣਾ max_coeff = std::max({std::ਨਿਰਪੇਖ(a0), std::ਨਿਰਪੇਖ(a1), std::ਨਿਰਪੇਖ(a2), std::ਨਿਰਪੇਖ(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    ਜੇ (max_coeff < std::numeric_limits<ਦੁੱਗਣਾ>::epsilon())
        ਵਾਪਸ -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    ਜੇ( std::ਨਿਰਪੇਖ(a0) < std::numeric_limits<ਦੁੱਗਣਾ>::epsilon() * max_coeff )
    {
        ਜੇ( std::ਨਿਰਪੇਖ(a1) < std::numeric_limits<ਦੁੱਗਣਾ>::epsilon() * max_coeff )
        {
            ਜੇ( std::ਨਿਰਪੇਖ(a2) < std::numeric_limits<ਦੁੱਗਣਾ>::epsilon() * max_coeff )
                n = std::ਨਿਰਪੇਖ(a3) < std::numeric_limits<ਦੁੱਗਣਾ>::epsilon() * max_coeff ? -1 : 0;
            ਨਹੀਂਤਾਂ
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        ਨਹੀਂਤਾਂ
        {
            // quadratic equation
            ਦੁੱਗਣਾ d = a2*a2 - 4*a1*a3;
            ਜੇ( d >= 0 )
            {
                d = std::ਵਰਗਮੂਲ(d);
                ਦੁੱਗਣਾ q1 = (-a2 + d) * 0.5;
                ਦੁੱਗਣਾ q2 = (a2 + d) * -0.5;
                ਜੇ( std::ਨਿਰਪੇਖ(q1) > std::ਨਿਰਪੇਖ(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                ਨਹੀਂਤਾਂ
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        ਦੁੱਗਣਾ scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        ਦੁੱਗਣਾ Q = (a1 * a1 - 3 * a2) * (1./9);
        ਦੁੱਗਣਾ R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        ਦੁੱਗਣਾ Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        ਦੁੱਗਣਾ d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        ਜੇ( d > 0 )
        {
            ਦੁੱਗਣਾ theta = acos(R / ਵਰਗਮੂਲ(Qcubed));
            ਦੁੱਗਣਾ sqrtQ = ਵਰਗਮੂਲ(Q);
            ਦੁੱਗਣਾ t0 = -2 * sqrtQ;
            ਦੁੱਗਣਾ t1 = theta * (1./3);
            ਦੁੱਗਣਾ t2 = a1 * (1./3);
            x0 = t0 * ਕੋਜੀਵਾ(t1) - t2;
            x1 = t0 * ਕੋਜੀਵਾ(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * ਕੋਜੀਵਾ(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        ਨਹੀਂਤਾਂ ਜੇ( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        ਨਹੀਂਤਾਂ
        {
            ਦੁੱਗਣਾ e;
            d = ਵਰਗਮੂਲ(-d);
            e = std::cbrt(d + fabs(R));
            ਜੇ( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    ਜੇ( roots.type() == CV_32FC1 )
    {
        roots.at<ਦਸ਼ਮਲਵ>(0) = (ਦਸ਼ਮਲਵ)x0;
        roots.at<ਦਸ਼ਮਲਵ>(1) = (ਦਸ਼ਮਲਵ)x1;
        roots.at<ਦਸ਼ਮਲਵ>(2) = (ਦਸ਼ਮਲਵ)x2;
    }
    ਨਹੀਂਤਾਂ
    {
        roots.at<ਦੁੱਗਣਾ>(0) = x0;
        roots.at<ਦੁੱਗਣਾ>(1) = x1;
        roots.at<ਦੁੱਗਣਾ>(2) = x2;
    }

    ਵਾਪਸ n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
ਦੁੱਗਣਾ cv::solvePoly( ਨਿਵੇਸ਼_Array _coeffs0, OutputArray _roots0, ਪੂਰਨ maxIters )
{
    CV_INSTRUMENT_REGION();

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ Complex<ਦੁੱਗਣਾ> C;

    ਦੁੱਗਣਾ maxDiff = 0;
    ਪੂਰਨ iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    ਪੂਰਨ ctype = _coeffs0.type();
    ਪੂਰਨ cdepth = CV_MAT_ਡੂੰਘਾਈ(ctype);

    CV_Assert( CV_MAT_ਡੂੰਘਾਈ(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.ਪੰਕਤੀਆਂ == 1 || coeffs0.ਖਾਨੇ_ਗਿਣਤੀ == 1) && coeffs0.ਕੁੱਲ() >= 2 );

    ਪੂਰਨ n0 = coeffs0.ਖਾਨੇ_ਗਿਣਤੀ + coeffs0.ਪੰਕਤੀਆਂ - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, ਸੱਚ, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ_FLT);
    Mat roots0 = _roots0.getMat();

    Auto_ਬਫਰ<C> buf(n*2+2);
    C *coeffs = buf.ਅੰਕੜੇ(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convert_ਮੰਜ਼ਿਲ(coeffs1, coeffs1.type());
    ਜੇ( coeffs0.channels() == 1 )
    {
        ਅਚਲ ਦੁੱਗਣਾ* rcoeffs = (ਅਚਲ ਦੁੱਗਣਾ*)roots;
        ਲਈ( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    ਲਈ( ; n > 1; n-- )
    {
        ਜੇ( std::ਨਿਰਪੇਖ(coeffs[n].re) + std::ਨਿਰਪੇਖ(coeffs[n].im) > DBL_EPSILON )
            ਰੋਕੋ;
    }
    CV_Assert( std::ਨਿਰਪੇਖ(coeffs[n].re) + std::ਨਿਰਪੇਖ(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    ਲਈ( i = 0; i <= n; i++ )
        absCoeffs.at<ਦੁੱਗਣਾ>(i) = ਨਿਰਪੇਖ(coeffs[i]);

    ਪੂਰਨ nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    ਲਈ( i = 0; i <= n; i++ )
    {
        ਦੁੱਗਣਾ coeff = absCoeffs.at<ਦੁੱਗਣਾ>(i);
        ਜੇ( coeff > DBL_EPSILON )
        {
            ਜੇ( i != n )
                u.at<ਦੁੱਗਣਾ>(i) = 2.0 * ਘਾਤ(coeff / absCoeffs.at<ਦੁੱਗਣਾ>(n), 1.0 / (n - i));
            ਜੇ( i != 0 )
                v.at<ਦੁੱਗਣਾ>(i - 1) = 0.5 * ਘਾਤ(absCoeffs.at<ਦੁੱਗਣਾ>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    ਦੁੱਗਣਾ scale = 1;
    ਜੇ( nonZeroCoeffs > 2 )
    {
        ਬਿੰਦੂ maxU, minV;
        minMaxLoc(u, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, &maxU);
        minMaxLoc(v, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, &minV);
        u.at<ਦੁੱਗਣਾ>(maxU) = 0;
        v.at<ਦੁੱਗਣਾ>(minV) = 0;
        scale = (ਜੋੜ(u).val[0] + ਜੋੜ(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(ਕੋਜੀਵਾ(CV_2PI / n), ਜੀਵਾ(CV_2PI / n));

    ਲਈ( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    ਲਈ( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        ਲਈ( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            ਪੂਰਨ ਅੰਕ_same_ਜੜ੍ਹ = 1;
            ਲਈ( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                ਜੇ( j != i )
                {
                    ਜੇ ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    ਨਹੀਂਤਾਂ
                        ਅੰਕ_same_ਜੜ੍ਹ++;
                }
            }
            num /= denom;
            ਜੇ( ਅੰਕ_same_ਜੜ੍ਹ > 1)
            {
                ਦੁੱਗਣਾ old_ਅੰਕ_re = num.re;
                ਦੁੱਗਣਾ old_ਅੰਕ_im = num.im;
                ਪੂਰਨ ਵਰਗਫਲ_ਜੜ੍ਹ_times = ਅੰਕ_same_ਜੜ੍ਹ % 2 == 0 ? ਅੰਕ_same_ਜੜ੍ਹ / 2 : ਅੰਕ_same_ਜੜ੍ਹ / 2 - 1;

                ਲਈ( j = 0; j < ਵਰਗਫਲ_ਜੜ੍ਹ_times; j++)
                {
                    num.re = old_ਅੰਕ_re*old_ਅੰਕ_re + old_ਅੰਕ_im*old_ਅੰਕ_im;
                    num.re = ਵਰਗਮੂਲ(num.re);
                    num.re += old_ਅੰਕ_re;
                    num.im = num.re - old_ਅੰਕ_re;
                    num.re /= 2;
                    num.re = ਵਰਗਮੂਲ(num.re);

                    num.im /= 2;
                    num.im = ਵਰਗਮੂਲ(num.im);
                    ਜੇ( old_ਅੰਕ_re < 0 ) num.im = -num.im;
                }

                ਜੇ( ਅੰਕ_same_ਜੜ੍ਹ % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<ਦੁੱਗਣਾ>(3) = -(std::ਘਾਤ(old_ਅੰਕ_re, 3));
                    cube_coefs.at<ਦੁੱਗਣਾ>(2) = -(15*std::ਘਾਤ(old_ਅੰਕ_re, 2) + 27*std::ਘਾਤ(old_ਅੰਕ_im, 2));
                    cube_coefs.at<ਦੁੱਗਣਾ>(1) = -48*old_ਅੰਕ_re;
                    cube_coefs.at<ਦੁੱਗਣਾ>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<ਦੁੱਗਣਾ>(0));
                    num.im = ਵਰਗਮੂਲ(std::ਘਾਤ(num.re, 2) / 3 - old_ਅੰਕ_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::ਨਿਰਪੇਖ(num));
        }
        ਜੇ( maxDiff <= 0 )
            ਰੋਕੋ;
    }

    ਜੇ( coeffs0.channels() == 1 )
    {
        ਅਚਲ ਦੁੱਗਣਾ verySmallEps = 1e-100;
        ਲਈ( i = 0; i < n; i++ )
            ਜੇ( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    ਲਈ( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convert_ਮੰਜ਼ਿਲ(roots0, roots0.type());
    ਵਾਪਸ maxDiff;
}


/* End of file. */
