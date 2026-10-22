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

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*MathFunc)(ಧ್ರುವ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len);

#ifdef HAVE_OPENCL

ಗಣನಾ { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_math_op(ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ oclop)
{
    ಪೂರ್ಣಾಙ್ಕ type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    ಪೂರ್ಣಾಙ್ಕ kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    ಧ್ರುವ ocl::Device d = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ double_support = d.doubleFPConfig() > 0;
    ಯದಿ (!double_support && depth == CV_64F)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.createSameSize(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ಯದಿ (src2.empty())
        k.args(src1arg, dstarg);
    ಅನ್ಯಥಾ
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, 0, ಅಸತ್ಯ);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN ಭಿನ್ನ cube_ಬೇರು( ಭಿನ್ನ value )
{
    CV_INSTRUMENT_REGION();

    ಭಿನ್ನ fr;
    Cv32suf v, m;
    ಪೂರ್ಣಾಙ್ಕ ix, s;
    ಪೂರ್ಣಾಙ್ಕ ex, shx;

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
    fr = (ಭಿನ್ನ)(((((45.2548339756803022511987494 * fr +
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
    ಪ್ರತ್ಯಾಗಮ v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

ಶೂನ್ಯ magnitude( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ಧ್ರುವ Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ len = (ಪೂರ್ಣಾಙ್ಕ)it.size*cn;

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಯದಿ( depth == CV_32F )
        {
            ಧ್ರುವ ಭಿನ್ನ *x = (ಧ್ರುವ ಭಿನ್ನ*)ptrs[0], *y = (ಧ್ರುವ ಭಿನ್ನ*)ptrs[1];
            ಭಿನ್ನ *mag = (ಭಿನ್ನ*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        ಅನ್ಯಥಾ
        {
            ಧ್ರುವ ದ್ವಿಗುಣ *x = (ಧ್ರುವ ದ್ವಿಗುಣ*)ptrs[0], *y = (ಧ್ರುವ ದ್ವಿಗುಣ*)ptrs[1];
            ದ್ವಿಗುಣ *mag = (ದ್ವಿಗುಣ*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

ಶೂನ್ಯ phase( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst, ತಾರ್ಕಿಕ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ಧ್ರುವ Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ j, total = (ಪೂರ್ಣಾಙ್ಕ)(it.size*cn), blockSize = total;
    size_t esz1 = X.elemSize1();
    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( j = 0; j < total; j += blockSize )
        {
            ಪೂರ್ಣಾಙ್ಕ len = std::min(total - j, blockSize);
            ಯದಿ( depth == CV_32F )
            {
                ಧ್ರುವ ಭಿನ್ನ *x = (ಧ್ರುವ ಭಿನ್ನ*)ptrs[0], *y = (ಧ್ರುವ ಭಿನ್ನ*)ptrs[1];
                ಭಿನ್ನ *angle = (ಭಿನ್ನ*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            ಅನ್ಯಥಾ
            {
                ಧ್ರುವ ದ್ವಿಗುಣ *x = (ಧ್ರುವ ದ್ವಿಗುಣ*)ptrs[0], *y = (ಧ್ರುವ ದ್ವಿಗುಣ*)ptrs[1];
                ದ್ವಿಗುಣ *angle = (ದ್ವಿಗುಣ*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_cartToPolar( ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, ತಾರ್ಕಿಕ angleInDegrees )
{
    ಧ್ರುವ ocl::Device & d = ocl::Device::getDefault();
    ಪೂರ್ಣಾಙ್ಕ type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;

    ಧ್ರುವ ತಾರ್ಕಿಕ _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ಧ್ರುವ ತಾರ್ಕಿಕ _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ಧ್ರುವ ತಾರ್ಕಿಕ _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ಧ್ರುವ ತಾರ್ಕಿಕ _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    ಯದಿ ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D OP_CTP_%s%s%s%s%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, 1)), depth,
                         rowsPerWI, angleInDegrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstMag   ? " -D SRC1_IS_DST_MAG" : "",
                         _src1IsDstAngle ? " -D SRC1_IS_DST_ANGLE" : "",
                         _src2IsDstMag   ? " -D SRC2_IS_DST_MAG" : "",
                         _src2IsDstAngle ? " -D SRC2_IS_DST_ANGLE" : ""
                         ));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    Size size = src1.size();
    CV_Assert( size == src2.size() );

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.args(_src1IsDstMag || _src1IsDstAngle ? ocl::KernelArg::ReadWriteNoSize(src1) : ocl::KernelArg::ReadOnlyNoSize(src1),
           _src2IsDstMag || _src2IsDstAngle ? ocl::KernelArg::ReadWriteNoSize(src2) : ocl::KernelArg::ReadOnlyNoSize(src2),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::WriteOnlyNoSize(dst2));

    size_t globalsize[2] = { (size_t)dst1.cols * cn, ((size_t)dst1.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಶೂನ್ಯ cartToPolar( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2,
                  OutputArray dst1, OutputArray dst2, ತಾರ್ಕಿಕ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    ಪೂರ್ಣಾಙ್ಕ type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ಧ್ರುವ Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ j, total = (ಪೂರ್ಣಾಙ್ಕ)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( j = 0; j < total; j += blockSize )
        {
            ಪೂರ್ಣಾಙ್ಕ len = std::min(total - j, blockSize);
            ಯದಿ( depth == CV_32F )
            {
                ಧ್ರುವ ಭಿನ್ನ *x = (ಧ್ರುವ ಭಿನ್ನ*)ptrs[0], *y = (ಧ್ರುವ ಭಿನ್ನ*)ptrs[1];
                ಭಿನ್ನ *mag = (ಭಿನ್ನ*)ptrs[2], *angle = (ಭಿನ್ನ*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            ಅನ್ಯಥಾ
            {
                ಧ್ರುವ ದ್ವಿಗುಣ *x = (ಧ್ರುವ ದ್ವಿಗುಣ*)ptrs[0], *y = (ಧ್ರುವ ದ್ವಿಗುಣ*)ptrs[1];
                ದ್ವಿಗುಣ *mag = (ದ್ವಿಗುಣ*)ptrs[2], *angle = (ದ್ವಿಗುಣ*)ptrs[3];
                hal::cartToPolar64f(x, y, mag, angle, len, angleInDegrees);
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

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_polarToCart( ಒಳಹರಿವು_Array _mag, ಒಳಹರಿವು_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, ತಾರ್ಕಿಕ angleInDegrees )
{
    ಧ್ರುವ ocl::Device & d = ocl::Device::getDefault();
    ಪೂರ್ಣಾಙ್ಕ type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;

    ಧ್ರುವ ತಾರ್ಕಿಕ _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ಧ್ರುವ ತಾರ್ಕಿಕ _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ಧ್ರುವ ತಾರ್ಕಿಕ _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ಧ್ರುವ ತಾರ್ಕಿಕ _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    ಯದಿ ( !doubleSupport && depth == CV_64F )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D BINARY_OP -D OP_PTC_%s%s%s%s%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, 1)), depth,
                         rowsPerWI,
                         angleInDegrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstX   ? " -D SRC1_IS_DST_X" : "",
                         _src1IsDstY ? " -D SRC1_IS_DST_Y" : "",
                         _src2IsDstX   ? " -D SRC2_IS_DST_X" : "",
                         _src2IsDstY ? " -D SRC2_IS_DST_Y" : ""));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat mag = _mag.getUMat(), angle = _angle.getUMat();
    Size size = angle.size();
    CV_Assert(mag.size() == size);

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.args(_src1IsDstX || _src1IsDstY ? ocl::KernelArg::ReadWriteNoSize(mag) : ocl::KernelArg::ReadOnlyNoSize(mag),
           _src2IsDstX || _src2IsDstY  ? ocl::KernelArg::ReadWriteNoSize(angle) : ocl::KernelArg::ReadOnlyNoSize(angle),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::WriteOnlyNoSize(dst2));

    size_t globalsize[2] = { (size_t)dst1.cols * cn, ((size_t)dst1.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಶೂನ್ಯ polarToCart( ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2,
                  OutputArray dst1, OutputArray dst2, ತಾರ್ಕಿಕ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    ಪೂರ್ಣಾಙ್ಕ type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ಧ್ರುವ Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ j, total = (ಪೂರ್ಣಾಙ್ಕ)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( j = 0; j < total; j += blockSize )
        {
            ಪೂರ್ಣಾಙ್ಕ len = std::min(total - j, blockSize);
            ಯದಿ ( depth == CV_32F )
            {
                ಧ್ರುವ ಭಿನ್ನ *mag = (ಧ್ರುವ ಭಿನ್ನ*)ptrs[0], *angle = (ಧ್ರುವ ಭಿನ್ನ*)ptrs[1];
                ಭಿನ್ನ *x = (ಭಿನ್ನ*)ptrs[2], *y = (ಭಿನ್ನ*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            ಅನ್ಯಥಾ
            {
                ಧ್ರುವ ದ್ವಿಗುಣ *mag = (ಧ್ರುವ ದ್ವಿಗುಣ*)ptrs[0], *angle = (ಧ್ರುವ ದ್ವಿಗುಣ*)ptrs[1];
                ದ್ವಿಗುಣ *x = (ದ್ವಿಗುಣ*)ptrs[2], *y = (ದ್ವಿಗುಣ*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            ಯದಿ( ptrs[0] )
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
ಗಣನಾ { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

ಸ್ಥಿರ ಶೂನ್ಯ math_op(ew::ತುದಿ op, ಒಳಹರಿವು_Array _src, OutputArray _dst)
{
    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    ಯದಿ (src.empty())
        ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ size_t total = src.total() * src.channels();
    ಯದಿ (src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ noparams[4] = {};
        k.fptr(src.data, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.data, 0, (ಪೂರ್ಣಾಙ್ಕ)total, 1, noparams, k.flags, k.userdata);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ew::TExpr p;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ a = p.add_ಒಳಹರಿವು(depth);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    ಧ್ರುವ Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

ಶೂನ್ಯ exp( ಒಳಹರಿವು_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ಪೂರ್ಣಾಙ್ಕ depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

ಶೂನ್ಯ log( ಒಳಹರಿವು_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ಪೂರ್ಣಾಙ್ಕ depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
ಸಂರಚನಾ iPow_SIMD
{
    ಪೂರ್ಣಾಙ್ಕ operator() ( ಧ್ರುವ T *, T *, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)
    {
        ಪ್ರತ್ಯಾಗಮ 0;
    }
};

#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)

template <>
ಸಂರಚನಾ iPow_SIMD<uchar, unsigned>
{
    ಪೂರ್ಣಾಙ್ಕ operator() ( ಧ್ರುವ uchar * src, uchar * dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power )
    {
        ಪೂರ್ಣಾಙ್ಕ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ಪ್ರತಿ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ಪೂರ್ಣಾಙ್ಕ p = power;

            ಯಾವತ್( p > 1 )
            {
                ಯದಿ (p & 1)
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

        ಪ್ರತ್ಯಾಗಮ i;
    }
};

template <>
ಸಂರಚನಾ iPow_SIMD<schar, ಪೂರ್ಣಾಙ್ಕ>
{
    ಪೂರ್ಣಾಙ್ಕ operator() ( ಧ್ರುವ schar * src, schar * dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power)
    {
        ಪೂರ್ಣಾಙ್ಕ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ಪ್ರತಿ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ಪೂರ್ಣಾಙ್ಕ p = power;

            ಯಾವತ್( p > 1 )
            {
                ಯದಿ (p & 1)
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

        ಪ್ರತ್ಯಾಗಮ i;
    }
};

template <>
ಸಂರಚನಾ iPow_SIMD<ushort, unsigned>
{
    ಪೂರ್ಣಾಙ್ಕ operator() ( ಧ್ರುವ ushort * src, ushort * dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power)
    {
        ಪೂರ್ಣಾಙ್ಕ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ಪ್ರತಿ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ಪೂರ್ಣಾಙ್ಕ p = power;

            ಯಾವತ್( p > 1 )
            {
                ಯದಿ (p & 1)
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

        ಪ್ರತ್ಯಾಗಮ i;
    }
};

template <>
ಸಂರಚನಾ iPow_SIMD<short, ಪೂರ್ಣಾಙ್ಕ>
{
    ಪೂರ್ಣಾಙ್ಕ operator() ( ಧ್ರುವ short * src, short * dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power)
    {
        ಪೂರ್ಣಾಙ್ಕ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ಪ್ರತಿ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ಪೂರ್ಣಾಙ್ಕ p = power;

            ಯಾವತ್( p > 1 )
            {
                ಯದಿ (p & 1)
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

        ಪ್ರತ್ಯಾಗಮ i;
    }
};

template <>
ಸಂರಚನಾ iPow_SIMD<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ>
{
    ಪೂರ್ಣಾಙ್ಕ operator() ( ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ * src, ಪೂರ್ಣಾಙ್ಕ * dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power)
    {
        ಪೂರ್ಣಾಙ್ಕ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ಪ್ರತಿ ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            ಪೂರ್ಣಾಙ್ಕ p = power;

            ಯಾವತ್( p > 1 )
            {
                ಯದಿ (p & 1)
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

        ಪ್ರತ್ಯಾಗಮ i;
    }
};

template <>
ಸಂರಚನಾ iPow_SIMD<ಭಿನ್ನ, ಭಿನ್ನ>
{
    ಪೂರ್ಣಾಙ್ಕ operator() ( ಧ್ರುವ ಭಿನ್ನ * src, ಭಿನ್ನ * dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power)
    {
        ಪೂರ್ಣಾಙ್ಕ i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        ಪ್ರತಿ ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            ಪೂರ್ಣಾಙ್ಕ p = std::abs(power);
            ಯದಿ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ಯಾವತ್( p > 1 )
            {
                ಯದಿ (p & 1)
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

        ಪ್ರತ್ಯಾಗಮ i;
    }
};

#ಯದಿ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
ಸಂರಚನಾ iPow_SIMD<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>
{
    ಪೂರ್ಣಾಙ್ಕ operator() ( ಧ್ರುವ ದ್ವಿಗುಣ * src, ದ್ವಿಗುಣ * dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power)
    {
        ಪೂರ್ಣಾಙ್ಕ i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        ಪ್ರತಿ ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            ಪೂರ್ಣಾಙ್ಕ p = std::abs(power);
            ಯದಿ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ಯಾವತ್( p > 1 )
            {
                ಯದಿ (p & 1)
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

        ಪ್ರತ್ಯಾಗಮ i;
    }
};
#endif

#endif

template<typename T, typename WT>
ಸ್ಥಿರ ಶೂನ್ಯ
iPow_i( ಧ್ರುವ ಶೂನ್ಯ* _src, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power )
{
    ಧ್ರುವ T* src = (ಧ್ರುವ T*)_src;
    T* dst = (T*)_dst;
    ಯದಿ( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    ಅನ್ಯಥಾ
    {
        iPow_SIMD<T, WT> vop;
        ಪೂರ್ಣಾಙ್ಕ i = vop(src, dst, len, power);

        ಪ್ರತಿ( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            ಪೂರ್ಣಾಙ್ಕ p = power;
            ಯಾವತ್( p > 1 )
            {
                ಯದಿ( p & 1 )
                    a *= b;
                b *= b;
                p >>= 1;
            }

            a *= b;
            dst[i] = saturate_cast<T>(a);
        }
    }
}

template<typename T>
ಸ್ಥಿರ ಶೂನ್ಯ
iPow_f( ಧ್ರುವ ಶೂನ್ಯ* _src, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power0 )
{
    ಧ್ರುವ T* src = (ಧ್ರುವ T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    ಪೂರ್ಣಾಙ್ಕ i = vop(src, dst, len, power0);
    ಪೂರ್ಣಾಙ್ಕ power = std::abs(power0);

    ಪ್ರತಿ( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        ಪೂರ್ಣಾಙ್ಕ p = power;
        ಯದಿ( power0 < 0 )
            b = 1/b;

        ಯಾವತ್( p > 1 )
        {
            ಯದಿ( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*IPowFunc)( ಧ್ರುವ ಶೂನ್ಯ* src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ power );

ಸ್ಥಿರ IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, ಪೂರ್ಣಾಙ್ಕ>, iPow_i<ushort, unsigned>, iPow_i<short, ಪೂರ್ಣಾಙ್ಕ>,
    iPow_i<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ>, iPow_f<ಭಿನ್ನ>, iPow_f<ದ್ವಿಗುಣ>, 0
};

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_pow(ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ power, OutputArray _dst,
                    ತಾರ್ಕಿಕ is_ipower, ಪೂರ್ಣಾಙ್ಕ ipower)
{
    ಧ್ರುವ ocl::Device & d = ocl::Device::getDefault();
    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;

    _dst.createSameSize(_src, type);
    ಯದಿ (is_ipower)
    {
        ಯದಿ( ipower < 0 )
        {
            ಯದಿ( depth == CV_32F || depth == CV_64F )
                is_ipower = ಅಸತ್ಯ;
            ಅನ್ಯಥಾ
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
    }

    ಯದಿ (depth == CV_64F && !doubleSupport)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ತಾರ್ಕಿಕ issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    ಯದಿ (is_ipower)
    {
        ಪೂರ್ಣಾಙ್ಕ wdepth = CV_32F;
        ಯದಿ (depth == CV_64F)
            wdepth = CV_64F;
        ಅನ್ಯಥಾ ಯದಿ (depth == CV_16F)
            wdepth = CV_16F;

        ವರ್ಣ cvt[2][50];
        extra_opts = format(
            " -D srcT1=%s -DsrcT1_C1=%s"
            " -D srcT2=int -D workST=int"
            " -D workT=%s -D wdepth=%d -D convertToWT1=%s"
            " -D convertToDT=%s"
            " -D workT1=%s",
            ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            ocl::typeToStr(CV_MAKE_TYPE(wdepth, 1)),
            wdepth,
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], ಪರಿಮಾಣ(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], ಪರಿಮಾಣ(cvt[1])),
            ocl::typeToStr(wdepth)
        );
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D cn=%d -D dstT=%s -D dstT_C1=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D %s%s%s%s",
                         1,
                         ocl::typeToStr(depth), ocl::typeToStr(depth), depth, rowsPerWI, op,
                         " -D UNARY_OP=1",
                         extra_opts.empty() ? "" : extra_opts.c_str(),
                         doubleSupport ? " -D DOUBLE_SUPPORT" : ""));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src = _src.getUMat();
    _dst.createSameSize(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    ಯದಿ (issqrt)
        k.args(srcarg, dstarg);
    ಅನ್ಯಥಾ ಯದಿ (is_ipower)
        k.args(srcarg, dstarg, ipower);
    ಅನ್ಯಥಾ
    {
        ಯದಿ (depth == CV_32F)
            k.args(srcarg, dstarg, (ಭಿನ್ನ)power);
        ಅನ್ಯಥಾ
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಶೂನ್ಯ pow( ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    ತಾರ್ಕಿಕ is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    ತಾರ್ಕಿಕ useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    ಯದಿ (is_ipower)
    {
        ವಿಕಲ್ಪ( ipower )
        {
        ಪಕ್ಷ 0:
            _dst.createSameSize(_src, type);
            _dst.setTo(Scalar::all(1));
            ಪ್ರತ್ಯಾಗಮ;
        ಪಕ್ಷ 1:
            _src.copyTo(_dst);
            ಪ್ರತ್ಯಾಗಮ;
        ಪಕ್ಷ 2:
            multiply(_src, _src, _dst);
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ಧ್ರುವ ತಾರ್ಕಿಕ floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    ಯದಿ( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.createSameSize(_src, type);
        Mat dst = _dst.getMat();

        ಧ್ರುವ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ಪೂರ್ಣಾಙ್ಕ len = (ಪೂರ್ಣಾಙ್ಕ)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        ಪ್ರತ್ಯಾಗಮ;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);
    Mat dst = _dst.getMat();
    ಯದಿ (src.empty())
        ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ size_t total = src.total() * cn;
    ಯದಿ (floatDepth && src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        ಯದಿ (k.fptr)
        {
            ದ್ವಿಗುಣ pvstore;                    // the broadcast exponent, stored as T
            ಶೂನ್ಯ* pv = &pvstore;
            ವಿಕಲ್ಪ (depth)
            {
            ಪಕ್ಷ CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); ಭಙ್ಗ;
            ಪಕ್ಷ CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); ಭಙ್ಗ;
            ಪಕ್ಷ CV_32F:  *(ಭಿನ್ನ*)pv = (ಭಿನ್ನ)power; ಭಙ್ಗ;
            ಸ್ವಭಾವ:      pvstore = power; ಭಙ್ಗ;
            }
            ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ noparams[4] = {};
            k.fptr(src.data, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.data, 0, (ಪೂರ್ಣಾಙ್ಕ)total, 1, noparams, k.flags, k.userdata);
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    ew::TExpr prog;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ a = prog.add_ಒಳಹರಿವು(depth);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_ದ್ವಿಮಾನ(ew::OP_POW, a, c, depth), out);
    prog.compile();
    ಧ್ರುವ Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

ಶೂನ್ಯ sqrt(ಒಳಹರಿವು_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    ಯದಿ (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        ಪ್ರತ್ಯಾಗಮ;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<ಪೂರ್ಣಾಙ್ಕ cv_mat_type> ಸಂರಚನಾ mat_type_assotiations{};

template<> ಸಂರಚನಾ mat_type_assotiations<CV_8U>
{
    ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ವರ್ಣ type;
    ಸ್ಥಿರ ಧ್ರುವ type min_allowable = 0x0;
    ಸ್ಥಿರ ಧ್ರುವ type max_allowable = 0xFF;
};

template<> ಸಂರಚನಾ mat_type_assotiations<CV_8S>
{
    ಪ್ರಕಾರನಿರ್ದೇಶ signed ವರ್ಣ type;
    ಸ್ಥಿರ ಧ್ರುವ type min_allowable = SCHAR_MIN;
    ಸ್ಥಿರ ಧ್ರುವ type max_allowable = SCHAR_MAX;
};

template<> ಸಂರಚನಾ mat_type_assotiations<CV_16U>
{
    ಪ್ರಕಾರನಿರ್ದೇಶ unsigned short type;
    ಸ್ಥಿರ ಧ್ರುವ type min_allowable = 0x0;
    ಸ್ಥಿರ ಧ್ರುವ type max_allowable = USHRT_MAX;
};
template<> ಸಂರಚನಾ mat_type_assotiations<CV_16S>
{
    ಪ್ರಕಾರನಿರ್ದೇಶ signed short type;
    ಸ್ಥಿರ ಧ್ರುವ type min_allowable = SHRT_MIN;
    ಸ್ಥಿರ ಧ್ರುವ type max_allowable = SHRT_MAX;
};

template<> ಸಂರಚನಾ mat_type_assotiations<CV_32S>
{
    ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ type;
    ಸ್ಥಿರ ಧ್ರುವ type min_allowable = (-INT_MAX - 1);
    ಸ್ಥಿರ ಧ್ರುವ type max_allowable = INT_MAX;
};

template<ಪೂರ್ಣಾಙ್ಕ depth>
ಸ್ಥಿರ ತಾರ್ಕಿಕ checkIntegerRange(cv::Mat src, Point& bad_pt, ಪೂರ್ಣಾಙ್ಕ minVal, ಪೂರ್ಣಾಙ್ಕ maxVal)
{
    ಪ್ರಕಾರನಿರ್ದೇಶ mat_type_assotiations<depth> type_ass;

    ಯದಿ (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಅನ್ಯಥಾ ಯದಿ (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < as_one_channel.rows; ++j)
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            ಯದಿ (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ತಾರ್ಕಿಕ (*check_range_function)(cv::Mat src, Point& bad_pt, ಪೂರ್ಣಾಙ್ಕ minVal, ಪೂರ್ಣಾಙ್ಕ maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
ಸ್ಥಿರ ಶೂನ್ಯ checkTypedRange(Mat& src, Point& badPt, ದ್ವಿಗುಣ minVal, ದ್ವಿಗುಣ maxVal)
{
    ಪೂರ್ಣಾಙ್ಕ loc = 0;
    ಪೂರ್ಣಾಙ್ಕ cn = src.channels();
    Size size = getContinuousSize2D(src, cn);
    ಧ್ರುವ T* isrc = src.ptr<T>();
    size_t step = src.step / ಪರಿಮಾಣ(isrc[0]);

    ಪ್ರತಿ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < size.width; i++ )
        {
            ದ್ವಿಗುಣ val = (ದ್ವಿಗುಣ)isrc[i];
            ಯದಿ( !(val >= minVal && val < maxVal) )
            {
                ಪೂರ್ಣಾಙ್ಕ pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                ಭಙ್ಗ;
            }
        }
    }
}

ತಾರ್ಕಿಕ checkRange(ಒಳಹರಿವು_Array _src, ತಾರ್ಕಿಕ quiet, Point* pt, ದ್ವಿಗುಣ minVal, ದ್ವಿಗುಣ maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    ಯದಿ ( src.dims > 2 )
    {
        CV_Assert(pt == ರಿಕ್ತ); // no way to provide location info

        ಧ್ರುವ Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        ಪ್ರತಿ ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಯದಿ (!checkRange( it.planes[0], quiet, ರಿಕ್ತ, minVal, maxVal ))
            {
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಪೂರ್ಣಾಙ್ಕ depth = src.depth();
    Point badPt(-1, -1);

    ಯದಿ (depth < CV_32F)
    {
        ಪೂರ್ಣಾಙ್ಕ minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        ಪೂರ್ಣಾಙ್ಕ maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ i, loc = 0;
        ಪೂರ್ಣಾಙ್ಕ cn = src.channels();
        Size size = getContinuousSize2D(src, cn);

        ಯದಿ( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_32F )
        {
            Cv32suf a, b;
            ಪೂರ್ಣಾಙ್ಕ ia, ib;
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* isrc = src.ptr<ಪೂರ್ಣಾಙ್ಕ>();
            size_t step = src.step/ಪರಿಮಾಣ(isrc[0]);

            a.f = (ಭಿನ್ನ)std::max(minVal, (ದ್ವಿಗುಣ)-FLT_MAX);
            b.f = (ಭಿನ್ನ)std::min(maxVal, (ದ್ವಿಗುಣ)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            ಪ್ರತಿ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                ಪ್ರತಿ( i = 0; i < size.width; i++ )
                {
                    ಪೂರ್ಣಾಙ್ಕ val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    ಯದಿ( val < ia || val >= ib )
                    {
                        ಪೂರ್ಣಾಙ್ಕ pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        ಭಙ್ಗ;
                    }
                }
            }
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ಧ್ರುವ int64* isrc = src.ptr<int64>();
            size_t step = src.step/ಪರಿಮಾಣ(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            ಪ್ರತಿ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                ಪ್ರತಿ( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    ಯದಿ( val < ia || val >= ib )
                    {
                        ಪೂರ್ಣಾಙ್ಕ pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        ಭಙ್ಗ;
                    }
                }
            }
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        ಅನ್ಯಥಾ
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    ಯದಿ( badPt.x >= 0 )
    {
        ಯದಿ( pt )
            *pt = badPt;
        ಯದಿ( !quiet )
        {
            cv::String ಮೌಲ್ಯ_str;
            ಮೌಲ್ಯ_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, ಮೌಲ್ಯ_str.c_str(), minVal, maxVal));
        }
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
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

ಪೂರ್ಣಾಙ್ಕ cv::solveCubic( ಒಳಹರಿವು_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n0 = 3;
    Mat coeffs = _coeffs.getMat();
    ಪೂರ್ಣಾಙ್ಕ ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == Size(n0, 1) ||
                coeffs.size() == Size(n0+1, 1) ||
                coeffs.size() == Size(1, n0) ||
                coeffs.size() == Size(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, ಸತ್ಯ, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    ಪೂರ್ಣಾಙ್ಕ i = -1, n = 0;
    ದ್ವಿಗುಣ a0 = 1., a1, a2, a3;
    ದ್ವಿಗುಣ x0 = 0., x1 = 0., x2 = 0.;
    ಪೂರ್ಣಾಙ್ಕ ncoeffs = coeffs.rows + coeffs.cols - 1;

    ಯದಿ( ctype == CV_32FC1 )
    {
        ಯದಿ( ncoeffs == 4 )
            a0 = coeffs.at<ಭಿನ್ನ>(++i);

        a1 = coeffs.at<ಭಿನ್ನ>(i+1);
        a2 = coeffs.at<ಭಿನ್ನ>(i+2);
        a3 = coeffs.at<ಭಿನ್ನ>(i+3);
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ( ncoeffs == 4 )
            a0 = coeffs.at<ದ್ವಿಗುಣ>(++i);

        a1 = coeffs.at<ದ್ವಿಗುಣ>(i+1);
        a2 = coeffs.at<ದ್ವಿಗುಣ>(i+2);
        a3 = coeffs.at<ದ್ವಿಗುಣ>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    ದ್ವಿಗುಣ max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    ಯದಿ (max_coeff < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon())
        ಪ್ರತ್ಯಾಗಮ -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    ಯದಿ( std::abs(a0) < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon() * max_coeff )
    {
        ಯದಿ( std::abs(a1) < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon() * max_coeff )
        {
            ಯದಿ( std::abs(a2) < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<ದ್ವಿಗುಣ>::epsilon() * max_coeff ? -1 : 0;
            ಅನ್ಯಥಾ
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        ಅನ್ಯಥಾ
        {
            // quadratic equation
            ದ್ವಿಗುಣ d = a2*a2 - 4*a1*a3;
            ಯದಿ( d >= 0 )
            {
                d = std::sqrt(d);
                ದ್ವಿಗುಣ q1 = (-a2 + d) * 0.5;
                ದ್ವಿಗುಣ q2 = (a2 + d) * -0.5;
                ಯದಿ( std::abs(q1) > std::abs(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                ಅನ್ಯಥಾ
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    ಅನ್ಯಥಾ
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

        ಯದಿ( d > 0 )
        {
            ದ್ವಿಗುಣ theta = acos(R / sqrt(Qcubed));
            ದ್ವಿಗುಣ sqrtQ = sqrt(Q);
            ದ್ವಿಗುಣ t0 = -2 * sqrtQ;
            ದ್ವಿಗುಣ t1 = theta * (1./3);
            ದ್ವಿಗುಣ t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        ಅನ್ಯಥಾ ಯದಿ( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        ಅನ್ಯಥಾ
        {
            ದ್ವಿಗುಣ e;
            d = sqrt(-d);
            e = std::cbrt(d + fabs(R));
            ಯದಿ( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    ಯದಿ( roots.type() == CV_32FC1 )
    {
        roots.at<ಭಿನ್ನ>(0) = (ಭಿನ್ನ)x0;
        roots.at<ಭಿನ್ನ>(1) = (ಭಿನ್ನ)x1;
        roots.at<ಭಿನ್ನ>(2) = (ಭಿನ್ನ)x2;
    }
    ಅನ್ಯಥಾ
    {
        roots.at<ದ್ವಿಗುಣ>(0) = x0;
        roots.at<ದ್ವಿಗುಣ>(1) = x1;
        roots.at<ದ್ವಿಗುಣ>(2) = x2;
    }

    ಪ್ರತ್ಯಾಗಮ n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
ದ್ವಿಗುಣ cv::solvePoly( ಒಳಹರಿವು_Array _coeffs0, OutputArray _roots0, ಪೂರ್ಣಾಙ್ಕ maxIters )
{
    CV_INSTRUMENT_REGION();

    ಪ್ರಕಾರನಿರ್ದೇಶ Complex<ದ್ವಿಗುಣ> C;

    ದ್ವಿಗುಣ maxDiff = 0;
    ಪೂರ್ಣಾಙ್ಕ iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    ಪೂರ್ಣಾಙ್ಕ ctype = _coeffs0.type();
    ಪೂರ್ಣಾಙ್ಕ cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.total() >= 2 );

    ಪೂರ್ಣಾಙ್ಕ n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, ಸತ್ಯ, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    AutoBuffer<C> buf(n*2+2);
    C *coeffs = buf.data(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    ಯದಿ( coeffs0.channels() == 1 )
    {
        ಧ್ರುವ ದ್ವಿಗುಣ* rcoeffs = (ಧ್ರುವ ದ್ವಿಗುಣ*)roots;
        ಪ್ರತಿ( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    ಪ್ರತಿ( ; n > 1; n-- )
    {
        ಯದಿ( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            ಭಙ್ಗ;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    ಪ್ರತಿ( i = 0; i <= n; i++ )
        absCoeffs.at<ದ್ವಿಗುಣ>(i) = abs(coeffs[i]);

    ಪೂರ್ಣಾಙ್ಕ nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    ಪ್ರತಿ( i = 0; i <= n; i++ )
    {
        ದ್ವಿಗುಣ coeff = absCoeffs.at<ದ್ವಿಗುಣ>(i);
        ಯದಿ( coeff > DBL_EPSILON )
        {
            ಯದಿ( i != n )
                u.at<ದ್ವಿಗುಣ>(i) = 2.0 * pow(coeff / absCoeffs.at<ದ್ವಿಗುಣ>(n), 1.0 / (n - i));
            ಯದಿ( i != 0 )
                v.at<ದ್ವಿಗುಣ>(i - 1) = 0.5 * pow(absCoeffs.at<ದ್ವಿಗುಣ>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    ದ್ವಿಗುಣ scale = 1;
    ಯದಿ( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<ದ್ವಿಗುಣ>(maxU) = 0;
        v.at<ದ್ವಿಗುಣ>(minV) = 0;
        scale = (sum(u).val[0] + sum(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    ಪ್ರತಿ( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    ಪ್ರತಿ( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        ಪ್ರತಿ( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_same_ಬೇರು = 1;
            ಪ್ರತಿ( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                ಯದಿ( j != i )
                {
                    ಯದಿ ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    ಅನ್ಯಥಾ
                        ಅಂಕೆ_same_ಬೇರು++;
                }
            }
            num /= denom;
            ಯದಿ( ಅಂಕೆ_same_ಬೇರು > 1)
            {
                ದ್ವಿಗುಣ old_ಅಂಕೆ_re = num.re;
                ದ್ವಿಗುಣ old_ಅಂಕೆ_im = num.im;
                ಪೂರ್ಣಾಙ್ಕ square_ಬೇರು_times = ಅಂಕೆ_same_ಬೇರು % 2 == 0 ? ಅಂಕೆ_same_ಬೇರು / 2 : ಅಂಕೆ_same_ಬೇರು / 2 - 1;

                ಪ್ರತಿ( j = 0; j < square_ಬೇರು_times; j++)
                {
                    num.re = old_ಅಂಕೆ_re*old_ಅಂಕೆ_re + old_ಅಂಕೆ_im*old_ಅಂಕೆ_im;
                    num.re = sqrt(num.re);
                    num.re += old_ಅಂಕೆ_re;
                    num.im = num.re - old_ಅಂಕೆ_re;
                    num.re /= 2;
                    num.re = sqrt(num.re);

                    num.im /= 2;
                    num.im = sqrt(num.im);
                    ಯದಿ( old_ಅಂಕೆ_re < 0 ) num.im = -num.im;
                }

                ಯದಿ( ಅಂಕೆ_same_ಬೇರು % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<ದ್ವಿಗುಣ>(3) = -(std::pow(old_ಅಂಕೆ_re, 3));
                    cube_coefs.at<ದ್ವಿಗುಣ>(2) = -(15*std::pow(old_ಅಂಕೆ_re, 2) + 27*std::pow(old_ಅಂಕೆ_im, 2));
                    cube_coefs.at<ದ್ವಿಗುಣ>(1) = -48*old_ಅಂಕೆ_re;
                    cube_coefs.at<ದ್ವಿಗುಣ>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<ದ್ವಿಗುಣ>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_ಅಂಕೆ_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        ಯದಿ( maxDiff <= 0 )
            ಭಙ್ಗ;
    }

    ಯದಿ( coeffs0.channels() == 1 )
    {
        ಧ್ರುವ ದ್ವಿಗುಣ verySmallEps = 1e-100;
        ಪ್ರತಿ( i = 0; i < n; i++ )
            ಯದಿ( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    ಪ್ರತಿ( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    ಪ್ರತ್ಯಾಗಮ maxDiff;
}


/* End of file. */
