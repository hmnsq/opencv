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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ <atomic>
#ಅನ್ತರ್ಭಾವ <limits>
#ಅನ್ತರ್ಭಾವ <iostream>
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <cmath>
#ಅನ್ತರ್ಭಾವ "mathfuncs.hpp"
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

ನಾಮಾಕಾಶ cv
{

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*MathFunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಎಣಿಕೆ { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_math_op(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, OutputArray _dst, ಪೂರ್ಣಾಂಕ oclop)
{
    ಪೂರ್ಣಾಂಕ type = _src1.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type);
    ಪೂರ್ಣಾಂಕ kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src1, _src2, _dst);

    ಸ್ಥಿರಾಂಕ ocl::Device d = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ double_support = d.doubleFPConfig() > 0;
    ಆದರೆ (!double_support && ಆಳ == CV_64F)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)), ಆಳ, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.create_Same_ಗಾತ್ರ(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src1),
            src2arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ಆದರೆ (src2.empty())
        k.ವಾದಗಳು(src1arg, dstarg);
    ಇಲ್ಲದಿದ್ದರೆ
        k.ವಾದಗಳು(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)src1.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, 0, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN ದಶಮಾಂಶ cube_ಬೇರು( ದಶಮಾಂಶ value )
{
    CV_INSTRUMENT_REGION();

    ದಶಮಾಂಶ fr;
    Cv32suf v, m;
    ಪೂರ್ಣಾಂಕ ix, s;
    ಪೂರ್ಣಾಂಕ ex, shx;

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
    fr = (ದಶಮಾಂಶ)(((((45.2548339756803022511987494 * fr +
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
    ಹಿಂತಿರುಗಿ v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

ಶೂನ್ಯ magnitude( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ type = src1.type(), ಆಳ = src1.ಆಳ(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (ಆಳ == CV_32F || ಆಳ == CV_64F));

    CV_OCL_ಸತತ(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ len = (ಪೂರ್ಣಾಂಕ)it.size*cn;

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಆದರೆ( ಆಳ == CV_32F )
        {
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *x = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ptrs[0], *y = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ptrs[1];
            ದಶಮಾಂಶ *mag = (ದಶಮಾಂಶ*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *x = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ptrs[0], *y = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ptrs[1];
            ದ್ವಿಗುಣ *mag = (ದ್ವಿಗುಣ*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

ಶೂನ್ಯ phase( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ type = src1.type(), ಆಳ = src1.ಆಳ(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (ಆಳ == CV_32F || ಆಳ == CV_64F));

    CV_OCL_ಸತತ(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angle_ಒಳ_Degrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ j, ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)(it.size*cn), ಖಂಡ_ಗಾತ್ರ_2 = ಒಟ್ಟು;
    size_t esz1 = X.elemSize1();
    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( j = 0; j < ಒಟ್ಟು; j += ಖಂಡ_ಗಾತ್ರ_2 )
        {
            ಪೂರ್ಣಾಂಕ len = std::min(ಒಟ್ಟು - j, ಖಂಡ_ಗಾತ್ರ_2);
            ಆದರೆ( ಆಳ == CV_32F )
            {
                ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *x = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ptrs[0], *y = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ptrs[1];
                ದಶಮಾಂಶ *angle = (ದಶಮಾಂಶ*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angle_ಒಳ_Degrees );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *x = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ptrs[0], *y = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ptrs[1];
                ದ್ವಿಗುಣ *angle = (ದ್ವಿಗುಣ*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angle_ಒಳ_Degrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_cart_ಗಮ್ಯ_Polar( ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    ಸ್ಥಿರಾಂಕ ocl::Device & d = ocl::Device::getDefault();
    ಪೂರ್ಣಾಂಕ type = _src1.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;

    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    ಆದರೆ ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (ಆಳ == CV_32F || ಆಳ == CV_64F) && type == _src2.type()) ||
         (ಆಳ == CV_64F && !doubleSupport) )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D OP_CTP_%s%s%s%s%s%s",
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, 1)), ಆಳ,
                         ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI, angle_ಒಳ_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstMag   ? " -D SRC1_IS_DST_MAG" : "",
                         _src1IsDstAngle ? " -D SRC1_IS_DST_ANGLE" : "",
                         _src2IsDstMag   ? " -D SRC2_IS_DST_MAG" : "",
                         _src2IsDstAngle ? " -D SRC2_IS_DST_ANGLE" : ""
                         ));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    ಗಾತ್ರ_2 size = src1.size();
    CV_Assert( size == src2.size() );

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.ವಾದಗಳು(_src1IsDstMag || _src1IsDstAngle ? ocl::KernelArg::Read_Write_No_ಗಾತ್ರ(src1) : ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src1),
           _src2IsDstMag || _src2IsDstAngle ? ocl::KernelArg::Read_Write_No_ಗಾತ್ರ(src2) : ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dst2));

    size_t globalsize[2] = { (size_t)dst1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn, ((size_t)dst1.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cart_ಗಮ್ಯ_Polar( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2,
                  OutputArray dst1, OutputArray dst2, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_ಸತತ(dst1.isUMat() && dst2.isUMat(),
            ocl_cart_ಗಮ್ಯ_Polar(src1, src2, dst1, dst2, angle_ಒಳ_Degrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    ಪೂರ್ಣಾಂಕ type = X.type(), ಆಳ = X.ಆಳ(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (ಆಳ == CV_32F || ಆಳ == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ j, ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)(it.size*cn), ಖಂಡ_ಗಾತ್ರ_2 = std::min(ಒಟ್ಟು, ((ಖಂಡ_ಗಾತ್ರ+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( j = 0; j < ಒಟ್ಟು; j += ಖಂಡ_ಗಾತ್ರ_2 )
        {
            ಪೂರ್ಣಾಂಕ len = std::min(ಒಟ್ಟು - j, ಖಂಡ_ಗಾತ್ರ_2);
            ಆದರೆ( ಆಳ == CV_32F )
            {
                ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *x = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ptrs[0], *y = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ptrs[1];
                ದಶಮಾಂಶ *mag = (ದಶಮಾಂಶ*)ptrs[2], *angle = (ದಶಮಾಂಶ*)ptrs[3];
                hal::cart_ಗಮ್ಯ_Polar32f( x, y, mag, angle, len, angle_ಒಳ_Degrees );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *x = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ptrs[0], *y = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ptrs[1];
                ದ್ವಿಗುಣ *mag = (ದ್ವಿಗುಣ*)ptrs[2], *angle = (ದ್ವಿಗುಣ*)ptrs[3];
                hal::cart_ಗಮ್ಯ_Polar64f(x, y, mag, angle, len, angle_ಒಳ_Degrees);
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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_polar_ಗಮ್ಯ_Cart( ಒಳಹರಿವು_Array _mag, ಒಳಹರಿವು_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    ಸ್ಥಿರಾಂಕ ocl::Device & d = ocl::Device::getDefault();
    ಪೂರ್ಣಾಂಕ type = _angle.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;

    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    ಆದರೆ ( !doubleSupport && ಆಳ == CV_64F )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D BINARY_OP -D OP_PTC_%s%s%s%s%s%s",
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, 1)), ಆಳ,
                         ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
                         angle_ಒಳ_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstX   ? " -D SRC1_IS_DST_X" : "",
                         _src1IsDstY ? " -D SRC1_IS_DST_Y" : "",
                         _src2IsDstX   ? " -D SRC2_IS_DST_X" : "",
                         _src2IsDstY ? " -D SRC2_IS_DST_Y" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat mag = _mag.getUMat(), angle = _angle.getUMat();
    ಗಾತ್ರ_2 size = angle.size();
    CV_Assert(mag.size() == size);

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.ವಾದಗಳು(_src1IsDstX || _src1IsDstY ? ocl::KernelArg::Read_Write_No_ಗಾತ್ರ(mag) : ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(mag),
           _src2IsDstX || _src2IsDstY  ? ocl::KernelArg::Read_Write_No_ಗಾತ್ರ(angle) : ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(angle),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dst2));

    size_t globalsize[2] = { (size_t)dst1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn, ((size_t)dst1.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ polar_ಗಮ್ಯ_Cart( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2,
                  OutputArray dst1, OutputArray dst2, ತಾರ್ಕಿಕ angle_ಒಳ_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    ಪೂರ್ಣಾಂಕ type = src2.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type);
    CV_Assert((ಆಳ == CV_32F || ಆಳ == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_ಸತತ(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polar_ಗಮ್ಯ_Cart(src1, src2, dst1, dst2, angle_ಒಳ_Degrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ j, ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)(it.size*cn), ಖಂಡ_ಗಾತ್ರ_2 = std::min(ಒಟ್ಟು, ((ಖಂಡ_ಗಾತ್ರ+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( j = 0; j < ಒಟ್ಟು; j += ಖಂಡ_ಗಾತ್ರ_2 )
        {
            ಪೂರ್ಣಾಂಕ len = std::min(ಒಟ್ಟು - j, ಖಂಡ_ಗಾತ್ರ_2);
            ಆದರೆ ( ಆಳ == CV_32F )
            {
                ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *mag = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ptrs[0], *angle = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)ptrs[1];
                ದಶಮಾಂಶ *x = (ದಶಮಾಂಶ*)ptrs[2], *y = (ದಶಮಾಂಶ*)ptrs[3];
                hal::polar_ಗಮ್ಯ_Cart32f( mag, angle, x, y, len, angle_ಒಳ_Degrees );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ *mag = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ptrs[0], *angle = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)ptrs[1];
                ದ್ವಿಗುಣ *x = (ದ್ವಿಗುಣ*)ptrs[2], *y = (ದ್ವಿಗುಣ*)ptrs[3];
                hal::polar_ಗಮ್ಯ_Cart64f( mag, angle, x, y, len, angle_ಒಳ_Degrees );
            }

            ಆದರೆ( ptrs[0] )
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
ಎಣಿಕೆ { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

ಸ್ಥಿರ ಶೂನ್ಯ math_op(ew::ತುದಿ op, ಒಳಹರಿವು_Array _src, OutputArray _dst)
{
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type);
    CV_Assert(ಆಳ == CV_16F || ಆಳ == CV_16BF || ಆಳ == CV_32F || ಆಳ == CV_64F);

    Mat src = _src.getMat();
    _dst.create_Same_ಗಾತ್ರ(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    ಆದರೆ (src.empty())
        ಹಿಂತಿರುಗಿ;

    ಸ್ಥಿರಾಂಕ size_t ಒಟ್ಟು = src.ಒಟ್ಟು() * src.channels();
    ಆದರೆ (src.isContinuous() && dst.isContinuous() && ಒಟ್ಟು <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, ಆಳ);
        CV_Assert(k.fptr);
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ noparams[4] = {};
        k.fptr(src.ದತ್ತಾಂಶ, 0, 1, ಶೂನ್ಯಸೂಚಕ, 0, 0, ಶೂನ್ಯಸೂಚಕ, 0, 0,
               dst.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು, 1, noparams, k.flags, k.userdata);
        ಹಿಂತಿರುಗಿ;
    }

    ew::TExpr p;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ a = p.ಕೂಡಿಸು_ಒಳಹರಿವು(ಆಳ);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ out = p.ಕೂಡಿಸು_Output(ಆಳ);
    p.move_ಗಮ್ಯ_Output(p.emitUnary(op, a, ಆಳ), out);
    p.compile();
    ಸ್ಥಿರಾಂಕ Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

ಶೂನ್ಯ ಘಾತಾಂಕ( ಒಳಹರಿವು_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ಪೂರ್ಣಾಂಕ ಆಳ = _src.ಆಳ();   // consumed by CV_OCL_RUN only

    CV_OCL_ಸತತ(_dst.isUMat() && _src.dims() <= 2 && (ಆಳ == CV_32F || ಆಳ == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

ಶೂನ್ಯ ಲಘುಗಣಕ( ಒಳಹರಿವು_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ಪೂರ್ಣಾಂಕ ಆಳ = _src.ಆಳ();   // consumed by CV_OCL_RUN only

    CV_OCL_ಸತತ( _dst.isUMat() && _src.dims() <= 2 && (ಆಳ == CV_32F || ಆಳ == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT>
ರಚನೆ iPow_SIMD
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() ( ಸ್ಥಿರಾಂಕ T *, T *, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)
    {
        ಹಿಂತಿರುಗಿ 0;
    }
};

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

ಮಾದರಿ <>
ರಚನೆ iPow_SIMD<uchar, ಚಿಹ್ನೆರಹಿತ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() ( ಸ್ಥಿರಾಂಕ uchar * src, uchar * dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power )
    {
        ಪೂರ್ಣಾಂಕ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ಗಾಗಿ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_ತುಂಬು_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ಪೂರ್ಣಾಂಕ p = power;

            ಆಗ( p > 1 )
            {
                ಆದರೆ (p & 1)
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

        ಹಿಂತಿರುಗಿ i;
    }
};

ಮಾದರಿ <>
ರಚನೆ iPow_SIMD<schar, ಪೂರ್ಣಾಂಕ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() ( ಸ್ಥಿರಾಂಕ schar * src, schar * dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power)
    {
        ಪೂರ್ಣಾಂಕ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ಗಾಗಿ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_ತುಂಬು_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ಪೂರ್ಣಾಂಕ p = power;

            ಆಗ( p > 1 )
            {
                ಆದರೆ (p & 1)
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

        ಹಿಂತಿರುಗಿ i;
    }
};

ಮಾದರಿ <>
ರಚನೆ iPow_SIMD<ushort, ಚಿಹ್ನೆರಹಿತ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() ( ಸ್ಥಿರಾಂಕ ushort * src, ushort * dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power)
    {
        ಪೂರ್ಣಾಂಕ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ಗಾಗಿ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_ತುಂಬು(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ಪೂರ್ಣಾಂಕ p = power;

            ಆಗ( p > 1 )
            {
                ಆದರೆ (p & 1)
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

        ಹಿಂತಿರುಗಿ i;
    }
};

ಮಾದರಿ <>
ರಚನೆ iPow_SIMD<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() ( ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ * src, ಗಿಡ್ಡ * dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power)
    {
        ಪೂರ್ಣಾಂಕ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ಗಾಗಿ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_ತುಂಬು(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ಪೂರ್ಣಾಂಕ p = power;

            ಆಗ( p > 1 )
            {
                ಆದರೆ (p & 1)
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

        ಹಿಂತಿರುಗಿ i;
    }
};

ಮಾದರಿ <>
ರಚನೆ iPow_SIMD<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() ( ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ * src, ಪೂರ್ಣಾಂಕ * dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power)
    {
        ಪೂರ್ಣಾಂಕ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ಗಾಗಿ ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_ತುಂಬು(src + i), v_b2 = vx_ತುಂಬು(src + i + VTraits<v_int32>::vlanes());
            ಪೂರ್ಣಾಂಕ p = power;

            ಆಗ( p > 1 )
            {
                ಆದರೆ (p & 1)
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

        ಹಿಂತಿರುಗಿ i;
    }
};

ಮಾದರಿ <>
ರಚನೆ iPow_SIMD<ದಶಮಾಂಶ, ದಶಮಾಂಶ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() ( ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ * src, ದಶಮಾಂಶ * dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power)
    {
        ಪೂರ್ಣಾಂಕ i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        ಗಾಗಿ ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_ತುಂಬು(src + i), v_b2 = vx_ತುಂಬು(src + i + VTraits<v_float32>::vlanes());
            ಪೂರ್ಣಾಂಕ p = std::ನಿರಪೇಕ್ಷ(power);
            ಆದರೆ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ಆಗ( p > 1 )
            {
                ಆದರೆ (p & 1)
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

        ಹಿಂತಿರುಗಿ i;
    }
};

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ಮಾದರಿ <>
ರಚನೆ iPow_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>
{
    ಪೂರ್ಣಾಂಕ ಕ್ರಿಯಾಕಾರಕ() ( ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ * src, ದ್ವಿಗುಣ * dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power)
    {
        ಪೂರ್ಣಾಂಕ i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        ಗಾಗಿ ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_ತುಂಬು(src + i), v_b2 = vx_ತುಂಬು(src + i + VTraits<v_float64>::vlanes());
            ಪೂರ್ಣಾಂಕ p = std::ನಿರಪೇಕ್ಷ(power);
            ಆದರೆ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ಆಗ( p > 1 )
            {
                ಆದರೆ (p & 1)
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

        ಹಿಂತಿರುಗಿ i;
    }
};
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT>
ಸ್ಥಿರ ಶೂನ್ಯ
iPow_i( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power )
{
    ಸ್ಥಿರಾಂಕ T* src = (ಸ್ಥಿರಾಂಕ T*)_src;
    T* dst = (T*)_dst;
    ಆದರೆ( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        iPow_SIMD<T, WT> vop;
        ಪೂರ್ಣಾಂಕ i = vop(src, dst, len, power);

        ಗಾಗಿ( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            ಪೂರ್ಣಾಂಕ p = power;
            ಆಗ( p > 1 )
            {
                ಆದರೆ( p & 1 )
                    a *= b;
                b *= b;
                p >>= 1;
            }

            a *= b;
            dst[i] = saturate_cast<T>(a);
        }
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಶೂನ್ಯ
iPow_f( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power0 )
{
    ಸ್ಥಿರಾಂಕ T* src = (ಸ್ಥಿರಾಂಕ T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    ಪೂರ್ಣಾಂಕ i = vop(src, dst, len, power0);
    ಪೂರ್ಣಾಂಕ power = std::ನಿರಪೇಕ್ಷ(power0);

    ಗಾಗಿ( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        ಪೂರ್ಣಾಂಕ p = power;
        ಆದರೆ( power0 < 0 )
            b = 1/b;

        ಆಗ( p > 1 )
        {
            ಆದರೆ( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*IPowFunc)( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ power );

ಸ್ಥಿರ IPowFunc ipowTab[CV_ಆಳ_MAX] =
{
    iPow_i<uchar, ಚಿಹ್ನೆರಹಿತ>, iPow_i<schar, ಪೂರ್ಣಾಂಕ>, iPow_i<ushort, ಚಿಹ್ನೆರಹಿತ>, iPow_i<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ>,
    iPow_i<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ>, iPow_f<ದಶಮಾಂಶ>, iPow_f<ದ್ವಿಗುಣ>, 0
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_pow(ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ power, OutputArray _dst,
                    ತಾರ್ಕಿಕ is_ipower, ಪೂರ್ಣಾಂಕ ipower)
{
    ಸ್ಥಿರಾಂಕ ocl::Device & d = ocl::Device::getDefault();
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;

    _dst.create_Same_ಗಾತ್ರ(_src, type);
    ಆದರೆ (is_ipower)
    {
        ಆದರೆ( ipower < 0 )
        {
            ಆದರೆ( ಆಳ == CV_32F || ಆಳ == CV_64F )
                is_ipower = ಸುಳ್ಳು;
            ಇಲ್ಲದಿದ್ದರೆ
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
    }

    ಆದರೆ (ಆಳ == CV_64F && !doubleSupport)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ತಾರ್ಕಿಕ issqrt = std::ನಿರಪೇಕ್ಷ(power - 0.5) < DBL_EPSILON;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    ಆದರೆ (is_ipower)
    {
        ಪೂರ್ಣಾಂಕ wdepth = CV_32F;
        ಆದರೆ (ಆಳ == CV_64F)
            wdepth = CV_64F;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ == CV_16F)
            wdepth = CV_16F;

        ಅಕ್ಷರ cvt[2][50];
        extra_opts = format(
            " -D srcT1=%s -DsrcT1_C1=%s"
            " -D srcT2=int -D workST=int"
            " -D workT=%s -D wdepth=%d -D convertToWT1=%s"
            " -D convertToDT=%s"
            " -D workT1=%s",
            ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, 1)),
            ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, 1)),
            ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(wdepth, 1)),
            wdepth,
            ocl::convertTypeStr(ಆಳ, wdepth, 1, cvt[0], ಗಾತ್ರ(cvt[0])),
            ocl::convertTypeStr(wdepth, ಆಳ, 1, cvt[1], ಗಾತ್ರ(cvt[1])),
            ocl::type_ಗಮ್ಯ_Str(wdepth)
        );
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D cn=%d -D dstT=%s -D dstT_C1=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D %s%s%s%s",
                         1,
                         ocl::type_ಗಮ್ಯ_Str(ಆಳ), ocl::type_ಗಮ್ಯ_Str(ಆಳ), ಆಳ, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI, op,
                         " -D UNARY_OP=1",
                         extra_opts.empty() ? "" : extra_opts.c_str(),
                         doubleSupport ? " -D DOUBLE_SUPPORT" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat();
    _dst.create_Same_ಗಾತ್ರ(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    ಆದರೆ (issqrt)
        k.ವಾದಗಳು(srcarg, dstarg);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_ipower)
        k.ವಾದಗಳು(srcarg, dstarg, ipower);
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (ಆಳ == CV_32F)
            k.ವಾದಗಳು(srcarg, dstarg, (ದಶಮಾಂಶ)power);
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ *  cn, ((size_t)dst.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ಘಾತ( ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type),
            cn = CV_MAT_CN(type), ipower = cv_ಸುತ್ತು(power);
    ತಾರ್ಕಿಕ is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ತಾರ್ಕಿಕ useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (is_ipower)
    {
        ಆಯ್ಕೆ( ipower )
        {
        ಸ್ಥಿತಿ 0:
            _dst.create_Same_ಗಾತ್ರ(_src, type);
            _dst.ಗಣ_ಗಮ್ಯ(Scalar::all(1));
            ಹಿಂತಿರುಗಿ;
        ಸ್ಥಿತಿ 1:
            _src.copy_ಗಮ್ಯ(_dst);
            ಹಿಂತಿರುಗಿ;
        ಸ್ಥಿತಿ 2:
            multiply(_src, _src, _dst);
            ಹಿಂತಿರುಗಿ;
        }
    }

    CV_OCL_ಸತತ(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ float_ಆಳ = ಆಳ == CV_16F || ಆಳ == CV_16BF || ಆಳ == CV_32F || ಆಳ == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    ಆದರೆ( is_ipower && !float_ಆಳ && ipowTab[ಆಳ] )
    {
        Mat src = _src.getMat();
        _dst.create_Same_ಗಾತ್ರ(_src, type);
        Mat dst = _dst.getMat();

        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ಪೂರ್ಣಾಂಕ len = (ಪೂರ್ಣಾಂಕ)(it.size*cn);
        IPowFunc func = ipowTab[ಆಳ];

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        ಹಿಂತಿರುಗಿ;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.create_Same_ಗಾತ್ರ(_src, type);
    Mat dst = _dst.getMat();
    ಆದರೆ (src.empty())
        ಹಿಂತಿರುಗಿ;

    ಸ್ಥಿರಾಂಕ size_t ಒಟ್ಟು = src.ಒಟ್ಟು() * cn;
    ಆದರೆ (float_ಆಳ && src.isContinuous() && dst.isContinuous() && ಒಟ್ಟು <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(ಆಳ, ಆಳ);
        ಆದರೆ (k.fptr)
        {
            ದ್ವಿಗುಣ pvstore;                    // the broadcast exponent, stored as T
            ಶೂನ್ಯ* pv = &pvstore;
            ಆಯ್ಕೆ (ಆಳ)
            {
            ಸ್ಥಿತಿ CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_32F:  *(ದಶಮಾಂಶ*)pv = (ದಶಮಾಂಶ)power; ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:      pvstore = power; ನಿಲ್ಲಿಸು;
            }
            ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ noparams[4] = {};
            k.fptr(src.ದತ್ತಾಂಶ, 0, 1, pv, 0, 0, ಶೂನ್ಯಸೂಚಕ, 0, 0,
                   dst.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು, 1, noparams, k.flags, k.userdata);
            ಹಿಂತಿರುಗಿ;
        }
    }

    ew::TExpr prog;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ a = prog.ಕೂಡಿಸು_ಒಳಹರಿವು(ಆಳ);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ c = prog.ಕೂಡಿಸು_Const(ew::EW_ಆಳ_NONE, Scalar(power), 1);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ out = prog.ಕೂಡಿಸು_Output(ಆಳ);
    prog.move_ಗಮ್ಯ_Output(prog.emit_ದ್ವಿಮಾನ(ew::OP_POW, a, c, ಆಳ), out);
    prog.compile();
    ಸ್ಥಿರಾಂಕ Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

ಶೂನ್ಯ ವರ್ಗಮೂಲ(ಒಳಹರಿವು_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::ಘಾತ(a, 0.5, b);
        ಹಿಂತಿರುಗಿ;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

ಮಾದರಿ<ಪೂರ್ಣಾಂಕ cv_mat_type> ರಚನೆ mat_type_assotiations{};

ಮಾದರಿ<> ರಚನೆ mat_type_assotiations<CV_8U>
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ type;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type min_allowable = 0x0;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type max_allowable = 0xFF;
};

ಮಾದರಿ<> ರಚನೆ mat_type_assotiations<CV_8S>
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ type;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type min_allowable = SCHAR_MIN;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type max_allowable = SCHAR_MAX;
};

ಮಾದರಿ<> ರಚನೆ mat_type_assotiations<CV_16U>
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ type;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type min_allowable = 0x0;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type max_allowable = USHRT_MAX;
};
ಮಾದರಿ<> ರಚನೆ mat_type_assotiations<CV_16S>
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆಯುಳ್ಳ ಗಿಡ್ಡ type;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type min_allowable = SHRT_MIN;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type max_allowable = SHRT_MAX;
};

ಮಾದರಿ<> ರಚನೆ mat_type_assotiations<CV_32S>
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ type;
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type min_allowable = (-ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ - 1);
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ type max_allowable = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
};

ಮಾದರಿ<ಪೂರ್ಣಾಂಕ ಆಳ>
ಸ್ಥಿರ ತಾರ್ಕಿಕ checkIntegerRange(cv::Mat src, ಬಿಂದು& bad_pt, ಪೂರ್ಣಾಂಕ minVal, ಪೂರ್ಣಾಂಕ maxVal)
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ mat_type_assotiations<ಆಳ> type_ass;

    ಆದರೆ (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::ಬಿಂದು(0,0);
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < as_one_channel.ಅಡ್ಡಸಾಲುಗಳು; ++j)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < as_one_channel.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ++i)
        {
            ಬಗೆ_ಹೆಸರು type_ass::type v = as_one_channel.at<ಬಗೆ_ಹೆಸರು type_ass::type>(j ,i);
            ಆದರೆ (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        }

    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ತಾರ್ಕಿಕ (*check_range_function)(cv::Mat src, ಬಿಂದು& bad_pt, ಪೂರ್ಣಾಂಕ minVal, ಪೂರ್ಣಾಂಕ maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸ್ಥಿರ ಶೂನ್ಯ checkTypedRange(Mat& src, ಬಿಂದು& badPt, ದ್ವಿಗುಣ minVal, ದ್ವಿಗುಣ maxVal)
{
    ಪೂರ್ಣಾಂಕ loc = 0;
    ಪೂರ್ಣಾಂಕ cn = src.channels();
    ಗಾತ್ರ_2 size = getContinuousSize2D(src, cn);
    ಸ್ಥಿರಾಂಕ T* isrc = src.ptr<T>();
    size_t ಹೆಜ್ಜೆ = src.ಹೆಜ್ಜೆ / ಗಾತ್ರ(isrc[0]);

    ಗಾಗಿ( ; badPt.x < 0 && size.ಎತ್ತರ--; loc += size.ಅಗಲ, isrc += ಹೆಜ್ಜೆ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < size.ಅಗಲ; i++ )
        {
            ದ್ವಿಗುಣ val = (ದ್ವಿಗುಣ)isrc[i];
            ಆದರೆ( !(val >= minVal && val < maxVal) )
            {
                ಪೂರ್ಣಾಂಕ pixelId = (loc + i) / cn;
                badPt = ಬಿಂದು(pixelId % src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, pixelId / src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
                ನಿಲ್ಲಿಸು;
            }
        }
    }
}

ತಾರ್ಕಿಕ checkRange(ಒಳಹರಿವು_Array _src, ತಾರ್ಕಿಕ quiet, ಬಿಂದು* pt, ದ್ವಿಗುಣ minVal, ದ್ವಿಗುಣ maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    ಆದರೆ ( src.dims > 2 )
    {
        CV_Assert(pt == ಖಾಲಿ); // no way to provide location info

        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        ಗಾಗಿ ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಆದರೆ (!checkRange( it.planes[0], quiet, ಖಾಲಿ, minVal, maxVal ))
            {
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ಪೂರ್ಣಾಂಕ ಆಳ = src.ಆಳ();
    ಬಿಂದು badPt(-1, -1);

    ಆದರೆ (ಆಳ < CV_32F)
    {
        ಪೂರ್ಣಾಂಕ minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        ಪೂರ್ಣಾಂಕ maxVali = maxVal > ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ ? ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ : cvCeil(maxVal) - 1;

        (check_range_functions[ಆಳ])(src, badPt, minVali, maxVali);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ i, loc = 0;
        ಪೂರ್ಣಾಂಕ cn = src.channels();
        ಗಾತ್ರ_2 size = getContinuousSize2D(src, cn);

        ಆದರೆ( ಆಳ == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_32F )
        {
            Cv32suf a, b;
            ಪೂರ್ಣಾಂಕ ia, ib;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* isrc = src.ptr<ಪೂರ್ಣಾಂಕ>();
            size_t ಹೆಜ್ಜೆ = src.ಹೆಜ್ಜೆ/ಗಾತ್ರ(isrc[0]);

            a.f = (ದಶಮಾಂಶ)std::max(minVal, (ದ್ವಿಗುಣ)-FLT_MAX);
            b.f = (ದಶಮಾಂಶ)std::min(maxVal, (ದ್ವಿಗುಣ)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            ಗಾಗಿ( ; badPt.x < 0 && size.ಎತ್ತರ--; loc += size.ಅಗಲ, isrc += ಹೆಜ್ಜೆ )
            {
                ಗಾಗಿ( i = 0; i < size.ಅಗಲ; i++ )
                {
                    ಪೂರ್ಣಾಂಕ val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    ಆದರೆ( val < ia || val >= ib )
                    {
                        ಪೂರ್ಣಾಂಕ pixelId = (loc + i) / cn;
                        badPt = ಬಿಂದು(pixelId % src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, pixelId / src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
                        ನಿಲ್ಲಿಸು;
                    }
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ಸ್ಥಿರಾಂಕ int64* isrc = src.ptr<int64>();
            size_t ಹೆಜ್ಜೆ = src.ಹೆಜ್ಜೆ/ಗಾತ್ರ(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            ಗಾಗಿ( ; badPt.x < 0 && size.ಎತ್ತರ--; loc += size.ಅಗಲ, isrc += ಹೆಜ್ಜೆ )
            {
                ಗಾಗಿ( i = 0; i < size.ಅಗಲ; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    ಆದರೆ( val < ia || val >= ib )
                    {
                        ಪೂರ್ಣಾಂಕ pixelId = (loc + i) / cn;
                        badPt = ಬಿಂದು(pixelId % src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, pixelId / src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
                        ನಿಲ್ಲಿಸು;
                    }
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    ಆದರೆ( badPt.x >= 0 )
    {
        ಆದರೆ( pt )
            *pt = badPt;
        ಆದರೆ( !quiet )
        {
            cv::String ಮೌಲ್ಯ_str;
            ಮೌಲ್ಯ_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::Sts_ಹೊರಹೋಗು_ದ_Range,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, ಮೌಲ್ಯ_str.c_str(), minVal, maxVal));
        }
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
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

ಪೂರ್ಣಾಂಕ cv::solveCubic( ಒಳಹರಿವು_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n0 = 3;
    Mat coeffs = _coeffs.getMat();
    ಪೂರ್ಣಾಂಕ ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == ಗಾತ್ರ_2(n0, 1) ||
                coeffs.size() == ಗಾತ್ರ_2(n0+1, 1) ||
                coeffs.size() == ಗಾತ್ರ_2(1, n0) ||
                coeffs.size() == ಗಾತ್ರ_2(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, ನಿಜ, _OutputArray::ಆಳ_ಮುಖವಾಡ_FLT);
    Mat roots = _roots.getMat();

    ಪೂರ್ಣಾಂಕ i = -1, n = 0;
    ದ್ವಿಗುಣ a0 = 1., a1, a2, a3;
    ದ್ವಿಗುಣ x0 = 0., x1 = 0., x2 = 0.;
    ಪೂರ್ಣಾಂಕ ncoeffs = coeffs.ಅಡ್ಡಸಾಲುಗಳು + coeffs.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - 1;

    ಆದರೆ( ctype == CV_32FC1 )
    {
        ಆದರೆ( ncoeffs == 4 )
            a0 = coeffs.at<ದಶಮಾಂಶ>(++i);

        a1 = coeffs.at<ದಶಮಾಂಶ>(i+1);
        a2 = coeffs.at<ದಶಮಾಂಶ>(i+2);
        a3 = coeffs.at<ದಶಮಾಂಶ>(i+3);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ( ncoeffs == 4 )
            a0 = coeffs.at<ದ್ವಿಗುಣ>(++i);

        a1 = coeffs.at<ದ್ವಿಗುಣ>(i+1);
        a2 = coeffs.at<ದ್ವಿಗುಣ>(i+2);
        a3 = coeffs.at<ದ್ವಿಗುಣ>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    ದ್ವಿಗುಣ max_coeff = std::max({std::ನಿರಪೇಕ್ಷ(a0), std::ನಿರಪೇಕ್ಷ(a1), std::ನಿರಪೇಕ್ಷ(a2), std::ನಿರಪೇಕ್ಷ(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    ಆದರೆ (max_coeff < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon())
        ಹಿಂತಿರುಗಿ -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    ಆದರೆ( std::ನಿರಪೇಕ್ಷ(a0) < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon() * max_coeff )
    {
        ಆದರೆ( std::ನಿರಪೇಕ್ಷ(a1) < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon() * max_coeff )
        {
            ಆದರೆ( std::ನಿರಪೇಕ್ಷ(a2) < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon() * max_coeff )
                n = std::ನಿರಪೇಕ್ಷ(a3) < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon() * max_coeff ? -1 : 0;
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            // quadratic equation
            ದ್ವಿಗುಣ d = a2*a2 - 4*a1*a3;
            ಆದರೆ( d >= 0 )
            {
                d = std::ವರ್ಗಮೂಲ(d);
                ದ್ವಿಗುಣ q1 = (-a2 + d) * 0.5;
                ದ್ವಿಗುಣ q2 = (a2 + d) * -0.5;
                ಆದರೆ( std::ನಿರಪೇಕ್ಷ(q1) > std::ನಿರಪೇಕ್ಷ(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        ದ್ವಿಗುಣ scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        ದ್ವಿಗುಣ Q = (a1 * a1 - 3 * a2) * (1./9);
        ದ್ವಿಗುಣ R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        ದ್ವಿಗುಣ Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        ದ್ವಿಗುಣ d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        ಆದರೆ( d > 0 )
        {
            ದ್ವಿಗುಣ theta = acos(R / ವರ್ಗಮೂಲ(Qcubed));
            ದ್ವಿಗುಣ sqrtQ = ವರ್ಗಮೂಲ(Q);
            ದ್ವಿಗುಣ t0 = -2 * sqrtQ;
            ದ್ವಿಗುಣ t1 = theta * (1./3);
            ದ್ವಿಗುಣ t2 = a1 * (1./3);
            x0 = t0 * ಕೋಜ್ಯಾ(t1) - t2;
            x1 = t0 * ಕೋಜ್ಯಾ(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * ಕೋಜ್ಯಾ(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ದ್ವಿಗುಣ e;
            d = ವರ್ಗಮೂಲ(-d);
            e = std::cbrt(d + fabs(R));
            ಆದರೆ( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    ಆದರೆ( roots.type() == CV_32FC1 )
    {
        roots.at<ದಶಮಾಂಶ>(0) = (ದಶಮಾಂಶ)x0;
        roots.at<ದಶಮಾಂಶ>(1) = (ದಶಮಾಂಶ)x1;
        roots.at<ದಶಮಾಂಶ>(2) = (ದಶಮಾಂಶ)x2;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        roots.at<ದ್ವಿಗುಣ>(0) = x0;
        roots.at<ದ್ವಿಗುಣ>(1) = x1;
        roots.at<ದ್ವಿಗುಣ>(2) = x2;
    }

    ಹಿಂತಿರುಗಿ n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
ದ್ವಿಗುಣ cv::solvePoly( ಒಳಹರಿವು_Array _coeffs0, OutputArray _roots0, ಪೂರ್ಣಾಂಕ maxIters )
{
    CV_INSTRUMENT_REGION();

    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Complex<ದ್ವಿಗುಣ> C;

    ದ್ವಿಗುಣ maxDiff = 0;
    ಪೂರ್ಣಾಂಕ iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    ಪೂರ್ಣಾಂಕ ctype = _coeffs0.type();
    ಪೂರ್ಣಾಂಕ cdepth = CV_MAT_ಆಳ(ctype);

    CV_Assert( CV_MAT_ಆಳ(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.ಅಡ್ಡಸಾಲುಗಳು == 1 || coeffs0.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1) && coeffs0.ಒಟ್ಟು() >= 2 );

    ಪೂರ್ಣಾಂಕ n0 = coeffs0.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + coeffs0.ಅಡ್ಡಸಾಲುಗಳು - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, ನಿಜ, _OutputArray::ಆಳ_ಮುಖವಾಡ_FLT);
    Mat roots0 = _roots0.getMat();

    Auto_ಬಫರ್<C> buf(n*2+2);
    C *coeffs = buf.ದತ್ತಾಂಶ(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convert_ಗಮ್ಯ(coeffs1, coeffs1.type());
    ಆದರೆ( coeffs0.channels() == 1 )
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* rcoeffs = (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)roots;
        ಗಾಗಿ( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    ಗಾಗಿ( ; n > 1; n-- )
    {
        ಆದರೆ( std::ನಿರಪೇಕ್ಷ(coeffs[n].re) + std::ನಿರಪೇಕ್ಷ(coeffs[n].im) > DBL_EPSILON )
            ನಿಲ್ಲಿಸು;
    }
    CV_Assert( std::ನಿರಪೇಕ್ಷ(coeffs[n].re) + std::ನಿರಪೇಕ್ಷ(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    ಗಾಗಿ( i = 0; i <= n; i++ )
        absCoeffs.at<ದ್ವಿಗುಣ>(i) = ನಿರಪೇಕ್ಷ(coeffs[i]);

    ಪೂರ್ಣಾಂಕ nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    ಗಾಗಿ( i = 0; i <= n; i++ )
    {
        ದ್ವಿಗುಣ coeff = absCoeffs.at<ದ್ವಿಗುಣ>(i);
        ಆದರೆ( coeff > DBL_EPSILON )
        {
            ಆದರೆ( i != n )
                u.at<ದ್ವಿಗುಣ>(i) = 2.0 * ಘಾತ(coeff / absCoeffs.at<ದ್ವಿಗುಣ>(n), 1.0 / (n - i));
            ಆದರೆ( i != 0 )
                v.at<ದ್ವಿಗುಣ>(i - 1) = 0.5 * ಘಾತ(absCoeffs.at<ದ್ವಿಗುಣ>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    ದ್ವಿಗುಣ scale = 1;
    ಆದರೆ( nonZeroCoeffs > 2 )
    {
        ಬಿಂದು maxU, minV;
        minMaxLoc(u, ಶೂನ್ಯಸೂಚಕ, ಶೂನ್ಯಸೂಚಕ, ಶೂನ್ಯಸೂಚಕ, &maxU);
        minMaxLoc(v, ಶೂನ್ಯಸೂಚಕ, ಶೂನ್ಯಸೂಚಕ, &minV);
        u.at<ದ್ವಿಗುಣ>(maxU) = 0;
        v.at<ದ್ವಿಗುಣ>(minV) = 0;
        scale = (ಮೊತ್ತ(u).val[0] + ಮೊತ್ತ(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(ಕೋಜ್ಯಾ(CV_2PI / n), ಜ್ಯಾ(CV_2PI / n));

    ಗಾಗಿ( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    ಗಾಗಿ( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        ಗಾಗಿ( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            ಪೂರ್ಣಾಂಕ ಅಂಕೆ_same_ಬೇರು = 1;
            ಗಾಗಿ( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                ಆದರೆ( j != i )
                {
                    ಆದರೆ ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    ಇಲ್ಲದಿದ್ದರೆ
                        ಅಂಕೆ_same_ಬೇರು++;
                }
            }
            num /= denom;
            ಆದರೆ( ಅಂಕೆ_same_ಬೇರು > 1)
            {
                ದ್ವಿಗುಣ old_ಅಂಕೆ_re = num.re;
                ದ್ವಿಗುಣ old_ಅಂಕೆ_im = num.im;
                ಪೂರ್ಣಾಂಕ ವರ್ಗಫಲ_ಬೇರು_times = ಅಂಕೆ_same_ಬೇರು % 2 == 0 ? ಅಂಕೆ_same_ಬೇರು / 2 : ಅಂಕೆ_same_ಬೇರು / 2 - 1;

                ಗಾಗಿ( j = 0; j < ವರ್ಗಫಲ_ಬೇರು_times; j++)
                {
                    num.re = old_ಅಂಕೆ_re*old_ಅಂಕೆ_re + old_ಅಂಕೆ_im*old_ಅಂಕೆ_im;
                    num.re = ವರ್ಗಮೂಲ(num.re);
                    num.re += old_ಅಂಕೆ_re;
                    num.im = num.re - old_ಅಂಕೆ_re;
                    num.re /= 2;
                    num.re = ವರ್ಗಮೂಲ(num.re);

                    num.im /= 2;
                    num.im = ವರ್ಗಮೂಲ(num.im);
                    ಆದರೆ( old_ಅಂಕೆ_re < 0 ) num.im = -num.im;
                }

                ಆದರೆ( ಅಂಕೆ_same_ಬೇರು % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<ದ್ವಿಗುಣ>(3) = -(std::ಘಾತ(old_ಅಂಕೆ_re, 3));
                    cube_coefs.at<ದ್ವಿಗುಣ>(2) = -(15*std::ಘಾತ(old_ಅಂಕೆ_re, 2) + 27*std::ಘಾತ(old_ಅಂಕೆ_im, 2));
                    cube_coefs.at<ದ್ವಿಗುಣ>(1) = -48*old_ಅಂಕೆ_re;
                    cube_coefs.at<ದ್ವಿಗುಣ>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<ದ್ವಿಗುಣ>(0));
                    num.im = ವರ್ಗಮೂಲ(std::ಘಾತ(num.re, 2) / 3 - old_ಅಂಕೆ_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::ನಿರಪೇಕ್ಷ(num));
        }
        ಆದರೆ( maxDiff <= 0 )
            ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ( coeffs0.channels() == 1 )
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ verySmallEps = 1e-100;
        ಗಾಗಿ( i = 0; i < n; i++ )
            ಆದರೆ( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    ಗಾಗಿ( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convert_ಗಮ್ಯ(roots0, roots0.type());
    ಹಿಂತಿರುಗಿ maxDiff;
}


/* End of file. */
