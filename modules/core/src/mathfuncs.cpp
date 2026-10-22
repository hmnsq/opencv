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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ <atomic>
#ଅନ୍ତର୍ଭାଵ <limits>
#ଅନ୍ତର୍ଭାଵ <iostream>
#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <cmath>
#ଅନ୍ତର୍ଭାଵ "mathfuncs.hpp"
#ଅନ୍ତର୍ଭାଵ "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

ନାମାକାଶ cv
{

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*MathFunc)(ଅଚଳ ଶୂନ୍ୟ* src, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);

#ifdef HAVE_OPENCL

ଗଣନା { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

ସ୍ଥିର ତାର୍କିକ ocl_math_op(ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା oclop)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    ଅଚଳ ocl::Device d = ocl::Device::getDefault();
    ତାର୍କିକ double_support = d.doubleFPConfig() > 0;
    ଯଦି (!double_support && depth == CV_64F)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.createSameSize(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ଯଦି (src2.empty())
        k.args(src1arg, dstarg);
    ଅନ୍ୟଥା
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, 0, ମିଥ୍ୟା);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN ଦଶମିକ cube_ମୂଳ( ଦଶମିକ value )
{
    CV_INSTRUMENT_REGION();

    ଦଶମିକ fr;
    Cv32suf v, m;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ix, s;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ex, shx;

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
    fr = (ଦଶମିକ)(((((45.2548339756803022511987494 * fr +
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
    ଫେରନ୍ତୁ v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

ଶୂନ୍ୟ magnitude( ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ଅଚଳ Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size*cn;

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ଯଦି( depth == CV_32F )
        {
            ଅଚଳ ଦଶମିକ *x = (ଅଚଳ ଦଶମିକ*)ptrs[0], *y = (ଅଚଳ ଦଶମିକ*)ptrs[1];
            ଦଶମିକ *mag = (ଦଶମିକ*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        ଅନ୍ୟଥା
        {
            ଅଚଳ ଦ୍ୱିଗୁଣ *x = (ଅଚଳ ଦ୍ୱିଗୁଣ*)ptrs[0], *y = (ଅଚଳ ଦ୍ୱିଗୁଣ*)ptrs[1];
            ଦ୍ୱିଗୁଣ *mag = (ଦ୍ୱିଗୁଣ*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

ଶୂନ୍ୟ phase( ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst, ତାର୍କିକ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ଅଚଳ Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(it.size*cn), blockSize = total;
    size_t esz1 = X.elemSize1();
    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( j = 0; j < total; j += blockSize )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = std::min(total - j, blockSize);
            ଯଦି( depth == CV_32F )
            {
                ଅଚଳ ଦଶମିକ *x = (ଅଚଳ ଦଶମିକ*)ptrs[0], *y = (ଅଚଳ ଦଶମିକ*)ptrs[1];
                ଦଶମିକ *angle = (ଦଶମିକ*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            ଅନ୍ୟଥା
            {
                ଅଚଳ ଦ୍ୱିଗୁଣ *x = (ଅଚଳ ଦ୍ୱିଗୁଣ*)ptrs[0], *y = (ଅଚଳ ଦ୍ୱିଗୁଣ*)ptrs[1];
                ଦ୍ୱିଗୁଣ *angle = (ଦ୍ୱିଗୁଣ*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_cartToPolar( ନିବେଶ_Array _src1, ନିବେଶ_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, ତାର୍କିକ angleInDegrees )
{
    ଅଚଳ ocl::Device & d = ocl::Device::getDefault();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;

    ଅଚଳ ତାର୍କିକ _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ଅଚଳ ତାର୍କିକ _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ଅଚଳ ତାର୍କିକ _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ଅଚଳ ତାର୍କିକ _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    ଯଦି ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

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
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

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
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ଶୂନ୍ୟ cartToPolar( ନିବେଶ_Array src1, ନିବେଶ_Array src2,
                  OutputArray dst1, OutputArray dst2, ତାର୍କିକ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ଅଚଳ Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( j = 0; j < total; j += blockSize )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = std::min(total - j, blockSize);
            ଯଦି( depth == CV_32F )
            {
                ଅଚଳ ଦଶମିକ *x = (ଅଚଳ ଦଶମିକ*)ptrs[0], *y = (ଅଚଳ ଦଶମିକ*)ptrs[1];
                ଦଶମିକ *mag = (ଦଶମିକ*)ptrs[2], *angle = (ଦଶମିକ*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            ଅନ୍ୟଥା
            {
                ଅଚଳ ଦ୍ୱିଗୁଣ *x = (ଅଚଳ ଦ୍ୱିଗୁଣ*)ptrs[0], *y = (ଅଚଳ ଦ୍ୱିଗୁଣ*)ptrs[1];
                ଦ୍ୱିଗୁଣ *mag = (ଦ୍ୱିଗୁଣ*)ptrs[2], *angle = (ଦ୍ୱିଗୁଣ*)ptrs[3];
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

ସ୍ଥିର ତାର୍କିକ ocl_polarToCart( ନିବେଶ_Array _mag, ନିବେଶ_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, ତାର୍କିକ angleInDegrees )
{
    ଅଚଳ ocl::Device & d = ocl::Device::getDefault();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;

    ଅଚଳ ତାର୍କିକ _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ଅଚଳ ତାର୍କିକ _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ଅଚଳ ତାର୍କିକ _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ଅଚଳ ତାର୍କିକ _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    ଯଦି ( !doubleSupport && depth == CV_64F )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

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
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

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
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ଶୂନ୍ୟ polarToCart( ନିବେଶ_Array src1, ନିବେଶ_Array src2,
                  OutputArray dst1, OutputArray dst2, ତାର୍କିକ angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ଅଚଳ Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( j = 0; j < total; j += blockSize )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = std::min(total - j, blockSize);
            ଯଦି ( depth == CV_32F )
            {
                ଅଚଳ ଦଶମିକ *mag = (ଅଚଳ ଦଶମିକ*)ptrs[0], *angle = (ଅଚଳ ଦଶମିକ*)ptrs[1];
                ଦଶମିକ *x = (ଦଶମିକ*)ptrs[2], *y = (ଦଶମିକ*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            ଅନ୍ୟଥା
            {
                ଅଚଳ ଦ୍ୱିଗୁଣ *mag = (ଅଚଳ ଦ୍ୱିଗୁଣ*)ptrs[0], *angle = (ଅଚଳ ଦ୍ୱିଗୁଣ*)ptrs[1];
                ଦ୍ୱିଗୁଣ *x = (ଦ୍ୱିଗୁଣ*)ptrs[2], *y = (ଦ୍ୱିଗୁଣ*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            ଯଦି( ptrs[0] )
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
ଗଣନା { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

ସ୍ଥିର ଶୂନ୍ୟ math_op(ew::ଶୀର୍ଷ op, ନିବେଶ_Array _src, OutputArray _dst)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    ଯଦି (src.empty())
        ଫେରନ୍ତୁ;

    ଅଚଳ size_t total = src.total() * src.channels();
    ଯଦି (src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        ସ୍ଥିର ଅଚଳ ଦ୍ୱିଗୁଣ noparams[4] = {};
        k.fptr(src.data, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total, 1, noparams, k.flags, k.userdata);
        ଫେରନ୍ତୁ;
    }

    ew::TExpr p;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = p.add_ନିବେଶ(depth);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    ଅଚଳ Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

ଶୂନ୍ୟ exp( ନିବେଶ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

ଶୂନ୍ୟ log( ନିବେଶ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
ସଂରଚନା iPow_SIMD
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() ( ଅଚଳ T *, T *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
    {
        ଫେରନ୍ତୁ 0;
    }
};

#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)

template <>
ସଂରଚନା iPow_SIMD<uchar, unsigned>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() ( ଅଚଳ uchar * src, uchar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ପାଇଁ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = power;

            ଯେତେବେଳେ( p > 1 )
            {
                ଯଦି (p & 1)
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

        ଫେରନ୍ତୁ i;
    }
};

template <>
ସଂରଚନା iPow_SIMD<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() ( ଅଚଳ schar * src, schar * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ପାଇଁ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = power;

            ଯେତେବେଳେ( p > 1 )
            {
                ଯଦି (p & 1)
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

        ଫେରନ୍ତୁ i;
    }
};

template <>
ସଂରଚନା iPow_SIMD<ushort, unsigned>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() ( ଅଚଳ ushort * src, ushort * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ପାଇଁ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = power;

            ଯେତେବେଳେ( p > 1 )
            {
                ଯଦି (p & 1)
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

        ଫେରନ୍ତୁ i;
    }
};

template <>
ସଂରଚନା iPow_SIMD<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() ( ଅଚଳ short * src, short * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ପାଇଁ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = power;

            ଯେତେବେଳେ( p > 1 )
            {
                ଯଦି (p & 1)
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

        ଫେରନ୍ତୁ i;
    }
};

template <>
ସଂରଚନା iPow_SIMD<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() ( ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା * src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ପାଇଁ ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = power;

            ଯେତେବେଳେ( p > 1 )
            {
                ଯଦି (p & 1)
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

        ଫେରନ୍ତୁ i;
    }
};

template <>
ସଂରଚନା iPow_SIMD<ଦଶମିକ, ଦଶମିକ>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() ( ଅଚଳ ଦଶମିକ * src, ଦଶମିକ * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        ପାଇଁ ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = std::abs(power);
            ଯଦି( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ଯେତେବେଳେ( p > 1 )
            {
                ଯଦି (p & 1)
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

        ଫେରନ୍ତୁ i;
    }
};

#ଯଦି (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
ସଂରଚନା iPow_SIMD<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ>
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator() ( ଅଚଳ ଦ୍ୱିଗୁଣ * src, ଦ୍ୱିଗୁଣ * dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        ପାଇଁ ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = std::abs(power);
            ଯଦି( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ଯେତେବେଳେ( p > 1 )
            {
                ଯଦି (p & 1)
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

        ଫେରନ୍ତୁ i;
    }
};
#endif

#endif

template<typename T, typename WT>
ସ୍ଥିର ଶୂନ୍ୟ
iPow_i( ଅଚଳ ଶୂନ୍ୟ* _src, ଶୂନ୍ୟ* _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power )
{
    ଅଚଳ T* src = (ଅଚଳ T*)_src;
    T* dst = (T*)_dst;
    ଯଦି( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    ଅନ୍ୟଥା
    {
        iPow_SIMD<T, WT> vop;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = vop(src, dst, len, power);

        ପାଇଁ( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = power;
            ଯେତେବେଳେ( p > 1 )
            {
                ଯଦି( p & 1 )
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
ସ୍ଥିର ଶୂନ୍ୟ
iPow_f( ଅଚଳ ଶୂନ୍ୟ* _src, ଶୂନ୍ୟ* _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power0 )
{
    ଅଚଳ T* src = (ଅଚଳ T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = vop(src, dst, len, power0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା power = std::abs(power0);

    ପାଇଁ( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = power;
        ଯଦି( power0 < 0 )
            b = 1/b;

        ଯେତେବେଳେ( p > 1 )
        {
            ଯଦି( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*IPowFunc)( ଅଚଳ ଶୂନ୍ୟ* src, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା power );

ସ୍ଥିର IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, iPow_i<ushort, unsigned>, iPow_i<short, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,
    iPow_i<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, iPow_f<ଦଶମିକ>, iPow_f<ଦ୍ୱିଗୁଣ>, 0
};

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_pow(ନିବେଶ_Array _src, ଦ୍ୱିଗୁଣ power, OutputArray _dst,
                    ତାର୍କିକ is_ipower, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ipower)
{
    ଅଚଳ ocl::Device & d = ocl::Device::getDefault();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;

    _dst.createSameSize(_src, type);
    ଯଦି (is_ipower)
    {
        ଯଦି( ipower < 0 )
        {
            ଯଦି( depth == CV_32F || depth == CV_64F )
                is_ipower = ମିଥ୍ୟା;
            ଅନ୍ୟଥା
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
    }

    ଯଦି (depth == CV_64F && !doubleSupport)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ତାର୍କିକ issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    ଅଚଳ ଅକ୍ଷର * ଅଚଳ op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    ଯଦି (is_ipower)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା wdepth = CV_32F;
        ଯଦି (depth == CV_64F)
            wdepth = CV_64F;
        ଅନ୍ୟଥା ଯଦି (depth == CV_16F)
            wdepth = CV_16F;

        ଅକ୍ଷର cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], ଆକାର(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], ଆକାର(cvt[1])),
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
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src = _src.getUMat();
    _dst.createSameSize(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    ଯଦି (issqrt)
        k.args(srcarg, dstarg);
    ଅନ୍ୟଥା ଯଦି (is_ipower)
        k.args(srcarg, dstarg, ipower);
    ଅନ୍ୟଥା
    {
        ଯଦି (depth == CV_32F)
            k.args(srcarg, dstarg, (ଦଶମିକ)power);
        ଅନ୍ୟଥା
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ଶୂନ୍ୟ pow( ନିବେଶ_Array _src, ଦ୍ୱିଗୁଣ power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    ତାର୍କିକ is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    ତାର୍କିକ useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    ଯଦି (is_ipower)
    {
        ବାଛ( ipower )
        {
        ସ୍ଥିତି 0:
            _dst.createSameSize(_src, type);
            _dst.setTo(Scalar::all(1));
            ଫେରନ୍ତୁ;
        ସ୍ଥିତି 1:
            _src.copyTo(_dst);
            ଫେରନ୍ତୁ;
        ସ୍ଥିତି 2:
            multiply(_src, _src, _dst);
            ଫେରନ୍ତୁ;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ଅଚଳ ତାର୍କିକ floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    ଯଦି( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.createSameSize(_src, type);
        Mat dst = _dst.getMat();

        ଅଚଳ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        ଫେରନ୍ତୁ;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);
    Mat dst = _dst.getMat();
    ଯଦି (src.empty())
        ଫେରନ୍ତୁ;

    ଅଚଳ size_t total = src.total() * cn;
    ଯଦି (floatDepth && src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        ଯଦି (k.fptr)
        {
            ଦ୍ୱିଗୁଣ pvstore;                    // the broadcast exponent, stored as T
            ଶୂନ୍ୟ* pv = &pvstore;
            ବାଛ (depth)
            {
            ସ୍ଥିତି CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_32F:  *(ଦଶମିକ*)pv = (ଦଶମିକ)power; ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:      pvstore = power; ଭାଙ୍ଗ;
            }
            ସ୍ଥିର ଅଚଳ ଦ୍ୱିଗୁଣ noparams[4] = {};
            k.fptr(src.data, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total, 1, noparams, k.flags, k.userdata);
            ଫେରନ୍ତୁ;
        }
    }

    ew::TExpr prog;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = prog.add_ନିବେଶ(depth);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_ଦ୍ୱିମିକ(ew::OP_POW, a, c, depth), out);
    prog.compile();
    ଅଚଳ Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

ଶୂନ୍ୟ sqrt(ନିବେଶ_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    ଯଦି (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        ଫେରନ୍ତୁ;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv_mat_type> ସଂରଚନା mat_type_assotiations{};

template<> ସଂରଚନା mat_type_assotiations<CV_8U>
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ଅକ୍ଷର type;
    ସ୍ଥିର ଅଚଳ type min_allowable = 0x0;
    ସ୍ଥିର ଅଚଳ type max_allowable = 0xFF;
};

template<> ସଂରଚନା mat_type_assotiations<CV_8S>
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା signed ଅକ୍ଷର type;
    ସ୍ଥିର ଅଚଳ type min_allowable = SCHAR_MIN;
    ସ୍ଥିର ଅଚଳ type max_allowable = SCHAR_MAX;
};

template<> ସଂରଚନା mat_type_assotiations<CV_16U>
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned short type;
    ସ୍ଥିର ଅଚଳ type min_allowable = 0x0;
    ସ୍ଥିର ଅଚଳ type max_allowable = USHRT_MAX;
};
template<> ସଂରଚନା mat_type_assotiations<CV_16S>
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା signed short type;
    ସ୍ଥିର ଅଚଳ type min_allowable = SHRT_MIN;
    ସ୍ଥିର ଅଚଳ type max_allowable = SHRT_MAX;
};

template<> ସଂରଚନା mat_type_assotiations<CV_32S>
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା type;
    ସ୍ଥିର ଅଚଳ type min_allowable = (-INT_MAX - 1);
    ସ୍ଥିର ଅଚଳ type max_allowable = INT_MAX;
};

template<ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth>
ସ୍ଥିର ତାର୍କିକ checkIntegerRange(cv::Mat src, Point& bad_pt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minVal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxVal)
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା mat_type_assotiations<depth> type_ass;

    ଯଦି (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଅନ୍ୟଥା ଯଦି (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < as_one_channel.rows; ++j)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            ଯଦି (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        }

    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ତାର୍କିକ (*check_range_function)(cv::Mat src, Point& bad_pt, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minVal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
ସ୍ଥିର ଶୂନ୍ୟ checkTypedRange(Mat& src, Point& badPt, ଦ୍ୱିଗୁଣ minVal, ଦ୍ୱିଗୁଣ maxVal)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = src.channels();
    Size size = getContinuousSize2D(src, cn);
    ଅଚଳ T* isrc = src.ptr<T>();
    size_t step = src.step / ଆକାର(isrc[0]);

    ପାଇଁ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < size.width; i++ )
        {
            ଦ୍ୱିଗୁଣ val = (ଦ୍ୱିଗୁଣ)isrc[i];
            ଯଦି( !(val >= minVal && val < maxVal) )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                ଭାଙ୍ଗ;
            }
        }
    }
}

ତାର୍କିକ checkRange(ନିବେଶ_Array _src, ତାର୍କିକ quiet, Point* pt, ଦ୍ୱିଗୁଣ minVal, ଦ୍ୱିଗୁଣ maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    ଯଦି ( src.dims > 2 )
    {
        CV_Assert(pt == ଖାଲି); // no way to provide location info

        ଅଚଳ Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        ପାଇଁ ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ଯଦି (!checkRange( it.planes[0], quiet, ଖାଲି, minVal, maxVal ))
            {
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = src.depth();
    Point badPt(-1, -1);

    ଯଦି (depth < CV_32F)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, loc = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = src.channels();
        Size size = getContinuousSize2D(src, cn);

        ଯଦି( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_32F )
        {
            Cv32suf a, b;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ia, ib;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* isrc = src.ptr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>();
            size_t step = src.step/ଆକାର(isrc[0]);

            a.f = (ଦଶମିକ)std::max(minVal, (ଦ୍ୱିଗୁଣ)-FLT_MAX);
            b.f = (ଦଶମିକ)std::min(maxVal, (ଦ୍ୱିଗୁଣ)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            ପାଇଁ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                ପାଇଁ( i = 0; i < size.width; i++ )
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    ଯଦି( val < ia || val >= ib )
                    {
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        ଭାଙ୍ଗ;
                    }
                }
            }
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ଅଚଳ int64* isrc = src.ptr<int64>();
            size_t step = src.step/ଆକାର(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            ପାଇଁ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                ପାଇଁ( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    ଯଦି( val < ia || val >= ib )
                    {
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        ଭାଙ୍ଗ;
                    }
                }
            }
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        ଅନ୍ୟଥା
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    ଯଦି( badPt.x >= 0 )
    {
        ଯଦି( pt )
            *pt = badPt;
        ଯଦି( !quiet )
        {
            cv::String ମୂଲ୍ୟ_str;
            ମୂଲ୍ୟ_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, ମୂଲ୍ୟ_str.c_str(), minVal, maxVal));
        }
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::solveCubic( ନିବେଶ_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n0 = 3;
    Mat coeffs = _coeffs.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == Size(n0, 1) ||
                coeffs.size() == Size(n0+1, 1) ||
                coeffs.size() == Size(1, n0) ||
                coeffs.size() == Size(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, ସତ୍ୟ, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = -1, n = 0;
    ଦ୍ୱିଗୁଣ a0 = 1., a1, a2, a3;
    ଦ୍ୱିଗୁଣ x0 = 0., x1 = 0., x2 = 0.;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ncoeffs = coeffs.rows + coeffs.cols - 1;

    ଯଦି( ctype == CV_32FC1 )
    {
        ଯଦି( ncoeffs == 4 )
            a0 = coeffs.at<ଦଶମିକ>(++i);

        a1 = coeffs.at<ଦଶମିକ>(i+1);
        a2 = coeffs.at<ଦଶମିକ>(i+2);
        a3 = coeffs.at<ଦଶମିକ>(i+3);
    }
    ଅନ୍ୟଥା
    {
        ଯଦି( ncoeffs == 4 )
            a0 = coeffs.at<ଦ୍ୱିଗୁଣ>(++i);

        a1 = coeffs.at<ଦ୍ୱିଗୁଣ>(i+1);
        a2 = coeffs.at<ଦ୍ୱିଗୁଣ>(i+2);
        a3 = coeffs.at<ଦ୍ୱିଗୁଣ>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    ଦ୍ୱିଗୁଣ max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    ଯଦି (max_coeff < std::numeric_limits<ଦ୍ୱିଗୁଣ>::epsilon())
        ଫେରନ୍ତୁ -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    ଯଦି( std::abs(a0) < std::numeric_limits<ଦ୍ୱିଗୁଣ>::epsilon() * max_coeff )
    {
        ଯଦି( std::abs(a1) < std::numeric_limits<ଦ୍ୱିଗୁଣ>::epsilon() * max_coeff )
        {
            ଯଦି( std::abs(a2) < std::numeric_limits<ଦ୍ୱିଗୁଣ>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<ଦ୍ୱିଗୁଣ>::epsilon() * max_coeff ? -1 : 0;
            ଅନ୍ୟଥା
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        ଅନ୍ୟଥା
        {
            // quadratic equation
            ଦ୍ୱିଗୁଣ d = a2*a2 - 4*a1*a3;
            ଯଦି( d >= 0 )
            {
                d = std::sqrt(d);
                ଦ୍ୱିଗୁଣ q1 = (-a2 + d) * 0.5;
                ଦ୍ୱିଗୁଣ q2 = (a2 + d) * -0.5;
                ଯଦି( std::abs(q1) > std::abs(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                ଅନ୍ୟଥା
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    ଅନ୍ୟଥା
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        ଦ୍ୱିଗୁଣ scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        ଦ୍ୱିଗୁଣ Q = (a1 * a1 - 3 * a2) * (1./9);
        ଦ୍ୱିଗୁଣ R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        ଦ୍ୱିଗୁଣ Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        ଦ୍ୱିଗୁଣ d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        ଯଦି( d > 0 )
        {
            ଦ୍ୱିଗୁଣ theta = acos(R / sqrt(Qcubed));
            ଦ୍ୱିଗୁଣ sqrtQ = sqrt(Q);
            ଦ୍ୱିଗୁଣ t0 = -2 * sqrtQ;
            ଦ୍ୱିଗୁଣ t1 = theta * (1./3);
            ଦ୍ୱିଗୁଣ t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        ଅନ୍ୟଥା ଯଦି( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        ଅନ୍ୟଥା
        {
            ଦ୍ୱିଗୁଣ e;
            d = sqrt(-d);
            e = std::cbrt(d + fabs(R));
            ଯଦି( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    ଯଦି( roots.type() == CV_32FC1 )
    {
        roots.at<ଦଶମିକ>(0) = (ଦଶମିକ)x0;
        roots.at<ଦଶମିକ>(1) = (ଦଶମିକ)x1;
        roots.at<ଦଶମିକ>(2) = (ଦଶମିକ)x2;
    }
    ଅନ୍ୟଥା
    {
        roots.at<ଦ୍ୱିଗୁଣ>(0) = x0;
        roots.at<ଦ୍ୱିଗୁଣ>(1) = x1;
        roots.at<ଦ୍ୱିଗୁଣ>(2) = x2;
    }

    ଫେରନ୍ତୁ n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
ଦ୍ୱିଗୁଣ cv::solvePoly( ନିବେଶ_Array _coeffs0, OutputArray _roots0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxIters )
{
    CV_INSTRUMENT_REGION();

    ପ୍ରକାରବ୍ୟାଖ୍ୟା Complex<ଦ୍ୱିଗୁଣ> C;

    ଦ୍ୱିଗୁଣ maxDiff = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ctype = _coeffs0.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.total() >= 2 );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, ସତ୍ୟ, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    AutoBuffer<C> buf(n*2+2);
    C *coeffs = buf.data(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    ଯଦି( coeffs0.channels() == 1 )
    {
        ଅଚଳ ଦ୍ୱିଗୁଣ* rcoeffs = (ଅଚଳ ଦ୍ୱିଗୁଣ*)roots;
        ପାଇଁ( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    ପାଇଁ( ; n > 1; n-- )
    {
        ଯଦି( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            ଭାଙ୍ଗ;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    ପାଇଁ( i = 0; i <= n; i++ )
        absCoeffs.at<ଦ୍ୱିଗୁଣ>(i) = abs(coeffs[i]);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    ପାଇଁ( i = 0; i <= n; i++ )
    {
        ଦ୍ୱିଗୁଣ coeff = absCoeffs.at<ଦ୍ୱିଗୁଣ>(i);
        ଯଦି( coeff > DBL_EPSILON )
        {
            ଯଦି( i != n )
                u.at<ଦ୍ୱିଗୁଣ>(i) = 2.0 * pow(coeff / absCoeffs.at<ଦ୍ୱିଗୁଣ>(n), 1.0 / (n - i));
            ଯଦି( i != 0 )
                v.at<ଦ୍ୱିଗୁଣ>(i - 1) = 0.5 * pow(absCoeffs.at<ଦ୍ୱିଗୁଣ>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    ଦ୍ୱିଗୁଣ scale = 1;
    ଯଦି( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<ଦ୍ୱିଗୁଣ>(maxU) = 0;
        v.at<ଦ୍ୱିଗୁଣ>(minV) = 0;
        scale = (sum(u).val[0] + sum(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    ପାଇଁ( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    ପାଇଁ( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        ପାଇଁ( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_same_ମୂଳ = 1;
            ପାଇଁ( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                ଯଦି( j != i )
                {
                    ଯଦି ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    ଅନ୍ୟଥା
                        ଅଙ୍କ_same_ମୂଳ++;
                }
            }
            num /= denom;
            ଯଦି( ଅଙ୍କ_same_ମୂଳ > 1)
            {
                ଦ୍ୱିଗୁଣ old_ଅଙ୍କ_re = num.re;
                ଦ୍ୱିଗୁଣ old_ଅଙ୍କ_im = num.im;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା square_ମୂଳ_times = ଅଙ୍କ_same_ମୂଳ % 2 == 0 ? ଅଙ୍କ_same_ମୂଳ / 2 : ଅଙ୍କ_same_ମୂଳ / 2 - 1;

                ପାଇଁ( j = 0; j < square_ମୂଳ_times; j++)
                {
                    num.re = old_ଅଙ୍କ_re*old_ଅଙ୍କ_re + old_ଅଙ୍କ_im*old_ଅଙ୍କ_im;
                    num.re = sqrt(num.re);
                    num.re += old_ଅଙ୍କ_re;
                    num.im = num.re - old_ଅଙ୍କ_re;
                    num.re /= 2;
                    num.re = sqrt(num.re);

                    num.im /= 2;
                    num.im = sqrt(num.im);
                    ଯଦି( old_ଅଙ୍କ_re < 0 ) num.im = -num.im;
                }

                ଯଦି( ଅଙ୍କ_same_ମୂଳ % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<ଦ୍ୱିଗୁଣ>(3) = -(std::pow(old_ଅଙ୍କ_re, 3));
                    cube_coefs.at<ଦ୍ୱିଗୁଣ>(2) = -(15*std::pow(old_ଅଙ୍କ_re, 2) + 27*std::pow(old_ଅଙ୍କ_im, 2));
                    cube_coefs.at<ଦ୍ୱିଗୁଣ>(1) = -48*old_ଅଙ୍କ_re;
                    cube_coefs.at<ଦ୍ୱିଗୁଣ>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<ଦ୍ୱିଗୁଣ>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_ଅଙ୍କ_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        ଯଦି( maxDiff <= 0 )
            ଭାଙ୍ଗ;
    }

    ଯଦି( coeffs0.channels() == 1 )
    {
        ଅଚଳ ଦ୍ୱିଗୁଣ verySmallEps = 1e-100;
        ପାଇଁ( i = 0; i < n; i++ )
            ଯଦି( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    ପାଇଁ( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    ଫେରନ୍ତୁ maxDiff;
}


/* End of file. */
