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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব <atomic>
#অন্তর্ভাব <limits>
#অন্তর্ভাব <iostream>
#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <cmath>
#অন্তর্ভাব "mathfuncs.hpp"
#অন্তর্ভাব "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

নামাকাশ cv
{

প্রকারলেখ শূন্য (*MathFunc)(ধ্রুবক শূন্য* src, শূন্য* dst, পূর্ণসংখ্যা len);

#ifdef HAVE_OPENCL

গণনা { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

স্থির ধ্রুবক অক্ষর* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

স্থির যৌক্তিক ocl_math_op(প্রবেশ_Array _src1, প্রবেশ_Array _src2, OutputArray _dst, পূর্ণসংখ্যা oclop)
{
    পূর্ণসংখ্যা type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    পূর্ণসংখ্যা kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    ধ্রুবক ocl::Device d = ocl::Device::getDefault();
    যৌক্তিক double_support = d.doubleFPConfig() > 0;
    যদি (!double_support && depth == CV_64F)
        ফেরত মিথ্যা;
    পূর্ণসংখ্যা rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.create_Same_আকার(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_আকার(src1),
            src2arg = ocl::KernelArg::Read_Only_No_আকার(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    যদি (src2.empty())
        k.args(src1arg, dstarg);
    অন্যথা
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, 0, মিথ্যা);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN দশমিক cube_মূল( দশমিক value )
{
    CV_INSTRUMENT_REGION();

    দশমিক fr;
    Cv32suf v, m;
    পূর্ণসংখ্যা ix, s;
    পূর্ণসংখ্যা ex, shx;

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
    fr = (দশমিক)(((((45.2548339756803022511987494 * fr +
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
    ফেরত v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

শূন্য magnitude( প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ধ্রুবক Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা len = (পূর্ণসংখ্যা)it.size*cn;

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        যদি( depth == CV_32F )
        {
            ধ্রুবক দশমিক *x = (ধ্রুবক দশমিক*)ptrs[0], *y = (ধ্রুবক দশমিক*)ptrs[1];
            দশমিক *mag = (দশমিক*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        অন্যথা
        {
            ধ্রুবক দ্বিগুণ *x = (ধ্রুবক দ্বিগুণ*)ptrs[0], *y = (ধ্রুবক দ্বিগুণ*)ptrs[1];
            দ্বিগুণ *mag = (দ্বিগুণ*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

শূন্য phase( প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst, যৌক্তিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ধ্রুবক Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা j, যোগ = (পূর্ণসংখ্যা)(it.size*cn), block_আকার = যোগ;
    size_t esz1 = X.elemSize1();
    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( j = 0; j < যোগ; j += block_আকার )
        {
            পূর্ণসংখ্যা len = std::min(যোগ - j, block_আকার);
            যদি( depth == CV_32F )
            {
                ধ্রুবক দশমিক *x = (ধ্রুবক দশমিক*)ptrs[0], *y = (ধ্রুবক দশমিক*)ptrs[1];
                দশমিক *angle = (দশমিক*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            অন্যথা
            {
                ধ্রুবক দ্বিগুণ *x = (ধ্রুবক দ্বিগুণ*)ptrs[0], *y = (ধ্রুবক দ্বিগুণ*)ptrs[1];
                দ্বিগুণ *angle = (দ্বিগুণ*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_cartToPolar( প্রবেশ_Array _src1, প্রবেশ_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, যৌক্তিক angleInDegrees )
{
    ধ্রুবক ocl::Device & d = ocl::Device::getDefault();
    পূর্ণসংখ্যা type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;

    ধ্রুবক যৌক্তিক _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ধ্রুবক যৌক্তিক _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ধ্রুবক যৌক্তিক _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ধ্রুবক যৌক্তিক _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    যদি ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        ফেরত মিথ্যা;

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
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    আকার_2 size = src1.size();
    CV_Assert( size == src2.size() );

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.args(_src1IsDstMag || _src1IsDstAngle ? ocl::KernelArg::Read_Write_No_আকার(src1) : ocl::KernelArg::Read_Only_No_আকার(src1),
           _src2IsDstMag || _src2IsDstAngle ? ocl::KernelArg::Read_Write_No_আকার(src2) : ocl::KernelArg::Read_Only_No_আকার(src2),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_আকার(dst2));

    size_t globalsize[2] = { (size_t)dst1.cols * cn, ((size_t)dst1.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

শূন্য cartToPolar( প্রবেশ_Array src1, প্রবেশ_Array src2,
                  OutputArray dst1, OutputArray dst2, যৌক্তিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    পূর্ণসংখ্যা type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ধ্রুবক Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা j, যোগ = (পূর্ণসংখ্যা)(it.size*cn), block_আকার = std::min(যোগ, ((BLOCK_আকার+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( j = 0; j < যোগ; j += block_আকার )
        {
            পূর্ণসংখ্যা len = std::min(যোগ - j, block_আকার);
            যদি( depth == CV_32F )
            {
                ধ্রুবক দশমিক *x = (ধ্রুবক দশমিক*)ptrs[0], *y = (ধ্রুবক দশমিক*)ptrs[1];
                দশমিক *mag = (দশমিক*)ptrs[2], *angle = (দশমিক*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            অন্যথা
            {
                ধ্রুবক দ্বিগুণ *x = (ধ্রুবক দ্বিগুণ*)ptrs[0], *y = (ধ্রুবক দ্বিগুণ*)ptrs[1];
                দ্বিগুণ *mag = (দ্বিগুণ*)ptrs[2], *angle = (দ্বিগুণ*)ptrs[3];
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

স্থির যৌক্তিক ocl_polarToCart( প্রবেশ_Array _mag, প্রবেশ_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, যৌক্তিক angleInDegrees )
{
    ধ্রুবক ocl::Device & d = ocl::Device::getDefault();
    পূর্ণসংখ্যা type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;

    ধ্রুবক যৌক্তিক _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ধ্রুবক যৌক্তিক _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ধ্রুবক যৌক্তিক _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ধ্রুবক যৌক্তিক _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    যদি ( !doubleSupport && depth == CV_64F )
        ফেরত মিথ্যা;

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
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat mag = _mag.getUMat(), angle = _angle.getUMat();
    আকার_2 size = angle.size();
    CV_Assert(mag.size() == size);

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.args(_src1IsDstX || _src1IsDstY ? ocl::KernelArg::Read_Write_No_আকার(mag) : ocl::KernelArg::Read_Only_No_আকার(mag),
           _src2IsDstX || _src2IsDstY  ? ocl::KernelArg::Read_Write_No_আকার(angle) : ocl::KernelArg::Read_Only_No_আকার(angle),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_আকার(dst2));

    size_t globalsize[2] = { (size_t)dst1.cols * cn, ((size_t)dst1.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

শূন্য polarToCart( প্রবেশ_Array src1, প্রবেশ_Array src2,
                  OutputArray dst1, OutputArray dst2, যৌক্তিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    পূর্ণসংখ্যা type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ধ্রুবক Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা j, যোগ = (পূর্ণসংখ্যা)(it.size*cn), block_আকার = std::min(যোগ, ((BLOCK_আকার+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( j = 0; j < যোগ; j += block_আকার )
        {
            পূর্ণসংখ্যা len = std::min(যোগ - j, block_আকার);
            যদি ( depth == CV_32F )
            {
                ধ্রুবক দশমিক *mag = (ধ্রুবক দশমিক*)ptrs[0], *angle = (ধ্রুবক দশমিক*)ptrs[1];
                দশমিক *x = (দশমিক*)ptrs[2], *y = (দশমিক*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            অন্যথা
            {
                ধ্রুবক দ্বিগুণ *mag = (ধ্রুবক দ্বিগুণ*)ptrs[0], *angle = (ধ্রুবক দ্বিগুণ*)ptrs[1];
                দ্বিগুণ *x = (দ্বিগুণ*)ptrs[2], *y = (দ্বিগুণ*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            যদি( ptrs[0] )
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
গণনা { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

স্থির শূন্য math_op(ew::শীর্ষ op, প্রবেশ_Array _src, OutputArray _dst)
{
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.create_Same_আকার(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    যদি (src.empty())
        ফেরত;

    ধ্রুবক size_t যোগ = src.যোগ() * src.channels();
    যদি (src.isContinuous() && dst.isContinuous() && যোগ <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        স্থির ধ্রুবক দ্বিগুণ noparams[4] = {};
        k.fptr(src.তথ্য, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.তথ্য, 0, (পূর্ণসংখ্যা)যোগ, 1, noparams, k.flags, k.userdata);
        ফেরত;
    }

    ew::TExpr p;
    ধ্রুবক পূর্ণসংখ্যা a = p.add_প্রবেশ(depth);
    ধ্রুবক পূর্ণসংখ্যা out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    ধ্রুবক Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

শূন্য exp( প্রবেশ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] পূর্ণসংখ্যা depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

শূন্য log( প্রবেশ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] পূর্ণসংখ্যা depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
কাঠামো iPow_SIMD
{
    পূর্ণসংখ্যা operator() ( ধ্রুবক T *, T *, পূর্ণসংখ্যা, পূর্ণসংখ্যা)
    {
        ফেরত 0;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)

template <>
কাঠামো iPow_SIMD<uchar, unsigned>
{
    পূর্ণসংখ্যা operator() ( ধ্রুবক uchar * src, uchar * dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power )
    {
        পূর্ণসংখ্যা i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        জন্য ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            পূর্ণসংখ্যা p = power;

            যখন( p > 1 )
            {
                যদি (p & 1)
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

        ফেরত i;
    }
};

template <>
কাঠামো iPow_SIMD<schar, পূর্ণসংখ্যা>
{
    পূর্ণসংখ্যা operator() ( ধ্রুবক schar * src, schar * dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power)
    {
        পূর্ণসংখ্যা i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        জন্য ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            পূর্ণসংখ্যা p = power;

            যখন( p > 1 )
            {
                যদি (p & 1)
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

        ফেরত i;
    }
};

template <>
কাঠামো iPow_SIMD<ushort, unsigned>
{
    পূর্ণসংখ্যা operator() ( ধ্রুবক ushort * src, ushort * dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power)
    {
        পূর্ণসংখ্যা i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        জন্য ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            পূর্ণসংখ্যা p = power;

            যখন( p > 1 )
            {
                যদি (p & 1)
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

        ফেরত i;
    }
};

template <>
কাঠামো iPow_SIMD<short, পূর্ণসংখ্যা>
{
    পূর্ণসংখ্যা operator() ( ধ্রুবক short * src, short * dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power)
    {
        পূর্ণসংখ্যা i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        জন্য ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            পূর্ণসংখ্যা p = power;

            যখন( p > 1 )
            {
                যদি (p & 1)
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

        ফেরত i;
    }
};

template <>
কাঠামো iPow_SIMD<পূর্ণসংখ্যা, পূর্ণসংখ্যা>
{
    পূর্ণসংখ্যা operator() ( ধ্রুবক পূর্ণসংখ্যা * src, পূর্ণসংখ্যা * dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power)
    {
        পূর্ণসংখ্যা i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        জন্য ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            পূর্ণসংখ্যা p = power;

            যখন( p > 1 )
            {
                যদি (p & 1)
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

        ফেরত i;
    }
};

template <>
কাঠামো iPow_SIMD<দশমিক, দশমিক>
{
    পূর্ণসংখ্যা operator() ( ধ্রুবক দশমিক * src, দশমিক * dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power)
    {
        পূর্ণসংখ্যা i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        জন্য ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            পূর্ণসংখ্যা p = std::abs(power);
            যদি( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            যখন( p > 1 )
            {
                যদি (p & 1)
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

        ফেরত i;
    }
};

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
কাঠামো iPow_SIMD<দ্বিগুণ, দ্বিগুণ>
{
    পূর্ণসংখ্যা operator() ( ধ্রুবক দ্বিগুণ * src, দ্বিগুণ * dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power)
    {
        পূর্ণসংখ্যা i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        জন্য ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            পূর্ণসংখ্যা p = std::abs(power);
            যদি( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            যখন( p > 1 )
            {
                যদি (p & 1)
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

        ফেরত i;
    }
};
#endif

#endif

template<typename T, typename WT>
স্থির শূন্য
iPow_i( ধ্রুবক শূন্য* _src, শূন্য* _dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power )
{
    ধ্রুবক T* src = (ধ্রুবক T*)_src;
    T* dst = (T*)_dst;
    যদি( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        জন্য( পূর্ণসংখ্যা i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    অন্যথা
    {
        iPow_SIMD<T, WT> vop;
        পূর্ণসংখ্যা i = vop(src, dst, len, power);

        জন্য( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            পূর্ণসংখ্যা p = power;
            যখন( p > 1 )
            {
                যদি( p & 1 )
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
স্থির শূন্য
iPow_f( ধ্রুবক শূন্য* _src, শূন্য* _dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power0 )
{
    ধ্রুবক T* src = (ধ্রুবক T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    পূর্ণসংখ্যা i = vop(src, dst, len, power0);
    পূর্ণসংখ্যা power = std::abs(power0);

    জন্য( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        পূর্ণসংখ্যা p = power;
        যদি( power0 < 0 )
            b = 1/b;

        যখন( p > 1 )
        {
            যদি( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

প্রকারলেখ শূন্য (*IPowFunc)( ধ্রুবক শূন্য* src, শূন্য* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা power );

স্থির IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, পূর্ণসংখ্যা>, iPow_i<ushort, unsigned>, iPow_i<short, পূর্ণসংখ্যা>,
    iPow_i<পূর্ণসংখ্যা, পূর্ণসংখ্যা>, iPow_f<দশমিক>, iPow_f<দ্বিগুণ>, 0
};

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_pow(প্রবেশ_Array _src, দ্বিগুণ power, OutputArray _dst,
                    যৌক্তিক is_ipower, পূর্ণসংখ্যা ipower)
{
    ধ্রুবক ocl::Device & d = ocl::Device::getDefault();
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;

    _dst.create_Same_আকার(_src, type);
    যদি (is_ipower)
    {
        যদি( ipower < 0 )
        {
            যদি( depth == CV_32F || depth == CV_64F )
                is_ipower = মিথ্যা;
            অন্যথা
                ফেরত মিথ্যা;
        }
    }

    যদি (depth == CV_64F && !doubleSupport)
        ফেরত মিথ্যা;

    যৌক্তিক issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    ধ্রুবক অক্ষর * ধ্রুবক op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    যদি (is_ipower)
    {
        পূর্ণসংখ্যা wdepth = CV_32F;
        যদি (depth == CV_64F)
            wdepth = CV_64F;
        অন্যথা যদি (depth == CV_16F)
            wdepth = CV_16F;

        অক্ষর cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], আকার(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], আকার(cvt[1])),
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
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src = _src.getUMat();
    _dst.create_Same_আকার(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_আকার(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    যদি (issqrt)
        k.args(srcarg, dstarg);
    অন্যথা যদি (is_ipower)
        k.args(srcarg, dstarg, ipower);
    অন্যথা
    {
        যদি (depth == CV_32F)
            k.args(srcarg, dstarg, (দশমিক)power);
        অন্যথা
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

শূন্য pow( প্রবেশ_Array _src, দ্বিগুণ power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    যৌক্তিক is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    যৌক্তিক useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    যদি (is_ipower)
    {
        নির্বাচন( ipower )
        {
        ক্ষেত্র 0:
            _dst.create_Same_আকার(_src, type);
            _dst.setTo(Scalar::all(1));
            ফেরত;
        ক্ষেত্র 1:
            _src.copyTo(_dst);
            ফেরত;
        ক্ষেত্র 2:
            multiply(_src, _src, _dst);
            ফেরত;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ধ্রুবক যৌক্তিক floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    যদি( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.create_Same_আকার(_src, type);
        Mat dst = _dst.getMat();

        ধ্রুবক Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        পূর্ণসংখ্যা len = (পূর্ণসংখ্যা)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        ফেরত;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.create_Same_আকার(_src, type);
    Mat dst = _dst.getMat();
    যদি (src.empty())
        ফেরত;

    ধ্রুবক size_t যোগ = src.যোগ() * cn;
    যদি (floatDepth && src.isContinuous() && dst.isContinuous() && যোগ <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        যদি (k.fptr)
        {
            দ্বিগুণ pvstore;                    // the broadcast exponent, stored as T
            শূন্য* pv = &pvstore;
            নির্বাচন (depth)
            {
            ক্ষেত্র CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); বিরতি;
            ক্ষেত্র CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); বিরতি;
            ক্ষেত্র CV_32F:  *(দশমিক*)pv = (দশমিক)power; বিরতি;
            পূর্বনির্ধারিত:      pvstore = power; বিরতি;
            }
            স্থির ধ্রুবক দ্বিগুণ noparams[4] = {};
            k.fptr(src.তথ্য, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.তথ্য, 0, (পূর্ণসংখ্যা)যোগ, 1, noparams, k.flags, k.userdata);
            ফেরত;
        }
    }

    ew::TExpr prog;
    ধ্রুবক পূর্ণসংখ্যা a = prog.add_প্রবেশ(depth);
    ধ্রুবক পূর্ণসংখ্যা c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    ধ্রুবক পূর্ণসংখ্যা out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_দ্বিমিক(ew::OP_POW, a, c, depth), out);
    prog.compile();
    ধ্রুবক Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

শূন্য sqrt(প্রবেশ_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    যদি (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        ফেরত;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<পূর্ণসংখ্যা cv_mat_type> কাঠামো mat_type_assotiations{};

template<> কাঠামো mat_type_assotiations<CV_8U>
{
    প্রকারলেখ unsigned অক্ষর type;
    স্থির ধ্রুবক type min_allowable = 0x0;
    স্থির ধ্রুবক type max_allowable = 0xFF;
};

template<> কাঠামো mat_type_assotiations<CV_8S>
{
    প্রকারলেখ signed অক্ষর type;
    স্থির ধ্রুবক type min_allowable = SCHAR_MIN;
    স্থির ধ্রুবক type max_allowable = SCHAR_MAX;
};

template<> কাঠামো mat_type_assotiations<CV_16U>
{
    প্রকারলেখ unsigned short type;
    স্থির ধ্রুবক type min_allowable = 0x0;
    স্থির ধ্রুবক type max_allowable = USHRT_MAX;
};
template<> কাঠামো mat_type_assotiations<CV_16S>
{
    প্রকারলেখ signed short type;
    স্থির ধ্রুবক type min_allowable = SHRT_MIN;
    স্থির ধ্রুবক type max_allowable = SHRT_MAX;
};

template<> কাঠামো mat_type_assotiations<CV_32S>
{
    প্রকারলেখ পূর্ণসংখ্যা type;
    স্থির ধ্রুবক type min_allowable = (-INT_MAX - 1);
    স্থির ধ্রুবক type max_allowable = INT_MAX;
};

template<পূর্ণসংখ্যা depth>
স্থির যৌক্তিক checkIntegerRange(cv::Mat src, Point& bad_pt, পূর্ণসংখ্যা minVal, পূর্ণসংখ্যা maxVal)
{
    প্রকারলেখ mat_type_assotiations<depth> type_ass;

    যদি (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        ফেরত সত্য;
    }
    অন্যথা যদি (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        ফেরত মিথ্যা;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    জন্য (পূর্ণসংখ্যা j = 0; j < as_one_channel.rows; ++j)
        জন্য (পূর্ণসংখ্যা i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            যদি (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                ফেরত মিথ্যা;
            }
        }

    ফেরত সত্য;
}

প্রকারলেখ যৌক্তিক (*check_range_function)(cv::Mat src, Point& bad_pt, পূর্ণসংখ্যা minVal, পূর্ণসংখ্যা maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
স্থির শূন্য checkTypedRange(Mat& src, Point& badPt, দ্বিগুণ minVal, দ্বিগুণ maxVal)
{
    পূর্ণসংখ্যা loc = 0;
    পূর্ণসংখ্যা cn = src.channels();
    আকার_2 size = getContinuousSize2D(src, cn);
    ধ্রুবক T* isrc = src.ptr<T>();
    size_t step = src.step / আকার(isrc[0]);

    জন্য( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < size.width; i++ )
        {
            দ্বিগুণ val = (দ্বিগুণ)isrc[i];
            যদি( !(val >= minVal && val < maxVal) )
            {
                পূর্ণসংখ্যা pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                বিরতি;
            }
        }
    }
}

যৌক্তিক checkRange(প্রবেশ_Array _src, যৌক্তিক quiet, Point* pt, দ্বিগুণ minVal, দ্বিগুণ maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    যদি ( src.dims > 2 )
    {
        CV_Assert(pt == ফাঁকা); // no way to provide location info

        ধ্রুবক Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        জন্য ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি (!checkRange( it.planes[0], quiet, ফাঁকা, minVal, maxVal ))
            {
                ফেরত মিথ্যা;
            }
        }
        ফেরত সত্য;
    }

    পূর্ণসংখ্যা depth = src.depth();
    Point badPt(-1, -1);

    যদি (depth < CV_32F)
    {
        পূর্ণসংখ্যা minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        পূর্ণসংখ্যা maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    অন্যথা
    {
        পূর্ণসংখ্যা i, loc = 0;
        পূর্ণসংখ্যা cn = src.channels();
        আকার_2 size = getContinuousSize2D(src, cn);

        যদি( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        অন্যথা যদি( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        অন্যথা যদি( depth == CV_32F )
        {
            Cv32suf a, b;
            পূর্ণসংখ্যা ia, ib;
            ধ্রুবক পূর্ণসংখ্যা* isrc = src.ptr<পূর্ণসংখ্যা>();
            size_t step = src.step/আকার(isrc[0]);

            a.f = (দশমিক)std::max(minVal, (দ্বিগুণ)-FLT_MAX);
            b.f = (দশমিক)std::min(maxVal, (দ্বিগুণ)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            জন্য( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                জন্য( i = 0; i < size.width; i++ )
                {
                    পূর্ণসংখ্যা val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    যদি( val < ia || val >= ib )
                    {
                        পূর্ণসংখ্যা pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        বিরতি;
                    }
                }
            }
        }
        অন্যথা যদি( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ধ্রুবক int64* isrc = src.ptr<int64>();
            size_t step = src.step/আকার(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            জন্য( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                জন্য( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    যদি( val < ia || val >= ib )
                    {
                        পূর্ণসংখ্যা pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        বিরতি;
                    }
                }
            }
        }
        অন্যথা যদি( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        অন্যথা যদি( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        অন্যথা যদি( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        অন্যথা যদি( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        অন্যথা
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    যদি( badPt.x >= 0 )
    {
        যদি( pt )
            *pt = badPt;
        যদি( !quiet )
        {
            cv::String মান_str;
            মান_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, মান_str.c_str(), minVal, maxVal));
        }
        ফেরত মিথ্যা;
    }
    ফেরত সত্য;
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

পূর্ণসংখ্যা cv::solveCubic( প্রবেশ_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ধ্রুবক পূর্ণসংখ্যা n0 = 3;
    Mat coeffs = _coeffs.getMat();
    পূর্ণসংখ্যা ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == আকার_2(n0, 1) ||
                coeffs.size() == আকার_2(n0+1, 1) ||
                coeffs.size() == আকার_2(1, n0) ||
                coeffs.size() == আকার_2(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, সত্য, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    পূর্ণসংখ্যা i = -1, n = 0;
    দ্বিগুণ a0 = 1., a1, a2, a3;
    দ্বিগুণ x0 = 0., x1 = 0., x2 = 0.;
    পূর্ণসংখ্যা ncoeffs = coeffs.rows + coeffs.cols - 1;

    যদি( ctype == CV_32FC1 )
    {
        যদি( ncoeffs == 4 )
            a0 = coeffs.at<দশমিক>(++i);

        a1 = coeffs.at<দশমিক>(i+1);
        a2 = coeffs.at<দশমিক>(i+2);
        a3 = coeffs.at<দশমিক>(i+3);
    }
    অন্যথা
    {
        যদি( ncoeffs == 4 )
            a0 = coeffs.at<দ্বিগুণ>(++i);

        a1 = coeffs.at<দ্বিগুণ>(i+1);
        a2 = coeffs.at<দ্বিগুণ>(i+2);
        a3 = coeffs.at<দ্বিগুণ>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    দ্বিগুণ max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    যদি (max_coeff < std::numeric_limits<দ্বিগুণ>::epsilon())
        ফেরত -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    যদি( std::abs(a0) < std::numeric_limits<দ্বিগুণ>::epsilon() * max_coeff )
    {
        যদি( std::abs(a1) < std::numeric_limits<দ্বিগুণ>::epsilon() * max_coeff )
        {
            যদি( std::abs(a2) < std::numeric_limits<দ্বিগুণ>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<দ্বিগুণ>::epsilon() * max_coeff ? -1 : 0;
            অন্যথা
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        অন্যথা
        {
            // quadratic equation
            দ্বিগুণ d = a2*a2 - 4*a1*a3;
            যদি( d >= 0 )
            {
                d = std::sqrt(d);
                দ্বিগুণ q1 = (-a2 + d) * 0.5;
                দ্বিগুণ q2 = (a2 + d) * -0.5;
                যদি( std::abs(q1) > std::abs(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                অন্যথা
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    অন্যথা
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        দ্বিগুণ scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        দ্বিগুণ Q = (a1 * a1 - 3 * a2) * (1./9);
        দ্বিগুণ R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        দ্বিগুণ Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        দ্বিগুণ d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        যদি( d > 0 )
        {
            দ্বিগুণ theta = acos(R / sqrt(Qcubed));
            দ্বিগুণ sqrtQ = sqrt(Q);
            দ্বিগুণ t0 = -2 * sqrtQ;
            দ্বিগুণ t1 = theta * (1./3);
            দ্বিগুণ t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        অন্যথা যদি( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        অন্যথা
        {
            দ্বিগুণ e;
            d = sqrt(-d);
            e = std::cbrt(d + fabs(R));
            যদি( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    যদি( roots.type() == CV_32FC1 )
    {
        roots.at<দশমিক>(0) = (দশমিক)x0;
        roots.at<দশমিক>(1) = (দশমিক)x1;
        roots.at<দশমিক>(2) = (দশমিক)x2;
    }
    অন্যথা
    {
        roots.at<দ্বিগুণ>(0) = x0;
        roots.at<দ্বিগুণ>(1) = x1;
        roots.at<দ্বিগুণ>(2) = x2;
    }

    ফেরত n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
দ্বিগুণ cv::solvePoly( প্রবেশ_Array _coeffs0, OutputArray _roots0, পূর্ণসংখ্যা maxIters )
{
    CV_INSTRUMENT_REGION();

    প্রকারলেখ Complex<দ্বিগুণ> C;

    দ্বিগুণ maxDiff = 0;
    পূর্ণসংখ্যা iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    পূর্ণসংখ্যা ctype = _coeffs0.type();
    পূর্ণসংখ্যা cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.যোগ() >= 2 );

    পূর্ণসংখ্যা n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, সত্য, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    Auto_বাফার<C> buf(n*2+2);
    C *coeffs = buf.তথ্য(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    যদি( coeffs0.channels() == 1 )
    {
        ধ্রুবক দ্বিগুণ* rcoeffs = (ধ্রুবক দ্বিগুণ*)roots;
        জন্য( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    জন্য( ; n > 1; n-- )
    {
        যদি( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            বিরতি;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    জন্য( i = 0; i <= n; i++ )
        absCoeffs.at<দ্বিগুণ>(i) = abs(coeffs[i]);

    পূর্ণসংখ্যা nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    জন্য( i = 0; i <= n; i++ )
    {
        দ্বিগুণ coeff = absCoeffs.at<দ্বিগুণ>(i);
        যদি( coeff > DBL_EPSILON )
        {
            যদি( i != n )
                u.at<দ্বিগুণ>(i) = 2.0 * pow(coeff / absCoeffs.at<দ্বিগুণ>(n), 1.0 / (n - i));
            যদি( i != 0 )
                v.at<দ্বিগুণ>(i - 1) = 0.5 * pow(absCoeffs.at<দ্বিগুণ>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    দ্বিগুণ scale = 1;
    যদি( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<দ্বিগুণ>(maxU) = 0;
        v.at<দ্বিগুণ>(minV) = 0;
        scale = (যোগ_2(u).val[0] + যোগ_2(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    জন্য( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    জন্য( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        জন্য( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            পূর্ণসংখ্যা অঙ্ক_same_মূল = 1;
            জন্য( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                যদি( j != i )
                {
                    যদি ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    অন্যথা
                        অঙ্ক_same_মূল++;
                }
            }
            num /= denom;
            যদি( অঙ্ক_same_মূল > 1)
            {
                দ্বিগুণ old_অঙ্ক_re = num.re;
                দ্বিগুণ old_অঙ্ক_im = num.im;
                পূর্ণসংখ্যা square_মূল_times = অঙ্ক_same_মূল % 2 == 0 ? অঙ্ক_same_মূল / 2 : অঙ্ক_same_মূল / 2 - 1;

                জন্য( j = 0; j < square_মূল_times; j++)
                {
                    num.re = old_অঙ্ক_re*old_অঙ্ক_re + old_অঙ্ক_im*old_অঙ্ক_im;
                    num.re = sqrt(num.re);
                    num.re += old_অঙ্ক_re;
                    num.im = num.re - old_অঙ্ক_re;
                    num.re /= 2;
                    num.re = sqrt(num.re);

                    num.im /= 2;
                    num.im = sqrt(num.im);
                    যদি( old_অঙ্ক_re < 0 ) num.im = -num.im;
                }

                যদি( অঙ্ক_same_মূল % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<দ্বিগুণ>(3) = -(std::pow(old_অঙ্ক_re, 3));
                    cube_coefs.at<দ্বিগুণ>(2) = -(15*std::pow(old_অঙ্ক_re, 2) + 27*std::pow(old_অঙ্ক_im, 2));
                    cube_coefs.at<দ্বিগুণ>(1) = -48*old_অঙ্ক_re;
                    cube_coefs.at<দ্বিগুণ>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<দ্বিগুণ>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_অঙ্ক_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        যদি( maxDiff <= 0 )
            বিরতি;
    }

    যদি( coeffs0.channels() == 1 )
    {
        ধ্রুবক দ্বিগুণ verySmallEps = 1e-100;
        জন্য( i = 0; i < n; i++ )
            যদি( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    জন্য( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    ফেরত maxDiff;
}


/* End of file. */
