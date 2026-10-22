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

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencl_kernels_core.hpp"
#অন্তর্ভাৱ <atomic>
#অন্তর্ভাৱ <limits>
#অন্তর্ভাৱ <iostream>
#অন্তর্ভাৱ <algorithm>
#অন্তর্ভাৱ <cmath>
#অন্তর্ভাৱ "mathfuncs.hpp"
#অন্তর্ভাৱ "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

নামাকাশ cv
{

প্রকারনির্দেশ শূন্য (*MathFunc)(ধ্রুৱ শূন্য* src, শূন্য* dst, পূর্ণাঙ্ক len);

#ifdef HAVE_OPENCL

গণনা { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

স্থির ধ্রুৱ ৱর্ণ* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

স্থির তার্কিক ocl_math_op(প্ৰবেশ_Array _src1, প্ৰবেশ_Array _src2, OutputArray _dst, পূর্ণাঙ্ক oclop)
{
    পূর্ণাঙ্ক type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    পূর্ণাঙ্ক kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    ধ্রুৱ ocl::Device d = ocl::Device::getDefault();
    তার্কিক double_support = d.doubleFPConfig() > 0;
    যদি (!double_support && depth == CV_64F)
        ঘূৰাই মিছা;
    পূর্ণাঙ্ক rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    যদি (k.empty())
        ঘূৰাই মিছা;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.createSameSize(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    যদি (src2.empty())
        k.args(src1arg, dstarg);
    অন্যথা
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ঘূৰাই k.run(2, globalsize, 0, মিছা);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN ভিন্ন cube_মূল( ভিন্ন value )
{
    CV_INSTRUMENT_REGION();

    ভিন্ন fr;
    Cv32suf v, m;
    পূর্ণাঙ্ক ix, s;
    পূর্ণাঙ্ক ex, shx;

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
    fr = (ভিন্ন)(((((45.2548339756803022511987494 * fr +
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
    ঘূৰাই v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

শূন্য magnitude( প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ধ্রুৱ Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক len = (পূর্ণাঙ্ক)it.size*cn;

    বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        যদি( depth == CV_32F )
        {
            ধ্রুৱ ভিন্ন *x = (ধ্রুৱ ভিন্ন*)ptrs[0], *y = (ধ্রুৱ ভিন্ন*)ptrs[1];
            ভিন্ন *mag = (ভিন্ন*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        অন্যথা
        {
            ধ্রুৱ দ্ৱিগুণ *x = (ধ্রুৱ দ্ৱিগুণ*)ptrs[0], *y = (ধ্রুৱ দ্ৱিগুণ*)ptrs[1];
            দ্ৱিগুণ *mag = (দ্ৱিগুণ*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

শূন্য phase( প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, OutputArray dst, তার্কিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ধ্রুৱ Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক j, total = (পূর্ণাঙ্ক)(it.size*cn), blockSize = total;
    size_t esz1 = X.elemSize1();
    বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        বাবে( j = 0; j < total; j += blockSize )
        {
            পূর্ণাঙ্ক len = std::min(total - j, blockSize);
            যদি( depth == CV_32F )
            {
                ধ্রুৱ ভিন্ন *x = (ধ্রুৱ ভিন্ন*)ptrs[0], *y = (ধ্রুৱ ভিন্ন*)ptrs[1];
                ভিন্ন *angle = (ভিন্ন*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            অন্যথা
            {
                ধ্রুৱ দ্ৱিগুণ *x = (ধ্রুৱ দ্ৱিগুণ*)ptrs[0], *y = (ধ্রুৱ দ্ৱিগুণ*)ptrs[1];
                দ্ৱিগুণ *angle = (দ্ৱিগুণ*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_cartToPolar( প্ৰবেশ_Array _src1, প্ৰবেশ_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, তার্কিক angleInDegrees )
{
    ধ্রুৱ ocl::Device & d = ocl::Device::getDefault();
    পূর্ণাঙ্ক type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    তার্কিক doubleSupport = d.doubleFPConfig() > 0;

    ধ্রুৱ তার্কিক _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ধ্রুৱ তার্কিক _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ধ্রুৱ তার্কিক _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ধ্রুৱ তার্কিক _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    যদি ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        ঘূৰাই মিছা;

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
        ঘূৰাই মিছা;

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
    ঘূৰাই k.run(2, globalsize, রিক্ত, মিছা);
}

#endif

শূন্য cartToPolar( প্ৰবেশ_Array src1, প্ৰবেশ_Array src2,
                  OutputArray dst1, OutputArray dst2, তার্কিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    পূর্ণাঙ্ক type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ধ্রুৱ Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক j, total = (পূর্ণাঙ্ক)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        বাবে( j = 0; j < total; j += blockSize )
        {
            পূর্ণাঙ্ক len = std::min(total - j, blockSize);
            যদি( depth == CV_32F )
            {
                ধ্রুৱ ভিন্ন *x = (ধ্রুৱ ভিন্ন*)ptrs[0], *y = (ধ্রুৱ ভিন্ন*)ptrs[1];
                ভিন্ন *mag = (ভিন্ন*)ptrs[2], *angle = (ভিন্ন*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            অন্যথা
            {
                ধ্রুৱ দ্ৱিগুণ *x = (ধ্রুৱ দ্ৱিগুণ*)ptrs[0], *y = (ধ্রুৱ দ্ৱিগুণ*)ptrs[1];
                দ্ৱিগুণ *mag = (দ্ৱিগুণ*)ptrs[2], *angle = (দ্ৱিগুণ*)ptrs[3];
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

স্থির তার্কিক ocl_polarToCart( প্ৰবেশ_Array _mag, প্ৰবেশ_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, তার্কিক angleInDegrees )
{
    ধ্রুৱ ocl::Device & d = ocl::Device::getDefault();
    পূর্ণাঙ্ক type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    তার্কিক doubleSupport = d.doubleFPConfig() > 0;

    ধ্রুৱ তার্কিক _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ধ্রুৱ তার্কিক _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ধ্রুৱ তার্কিক _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ধ্রুৱ তার্কিক _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    যদি ( !doubleSupport && depth == CV_64F )
        ঘূৰাই মিছা;

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
        ঘূৰাই মিছা;

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
    ঘূৰাই k.run(2, globalsize, রিক্ত, মিছা);
}

#endif

শূন্য polarToCart( প্ৰবেশ_Array src1, প্ৰবেশ_Array src2,
                  OutputArray dst1, OutputArray dst2, তার্কিক angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    পূর্ণাঙ্ক type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ধ্রুৱ Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক j, total = (পূর্ণাঙ্ক)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        বাবে( j = 0; j < total; j += blockSize )
        {
            পূর্ণাঙ্ক len = std::min(total - j, blockSize);
            যদি ( depth == CV_32F )
            {
                ধ্রুৱ ভিন্ন *mag = (ধ্রুৱ ভিন্ন*)ptrs[0], *angle = (ধ্রুৱ ভিন্ন*)ptrs[1];
                ভিন্ন *x = (ভিন্ন*)ptrs[2], *y = (ভিন্ন*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            অন্যথা
            {
                ধ্রুৱ দ্ৱিগুণ *mag = (ধ্রুৱ দ্ৱিগুণ*)ptrs[0], *angle = (ধ্রুৱ দ্ৱিগুণ*)ptrs[1];
                দ্ৱিগুণ *x = (দ্ৱিগুণ*)ptrs[2], *y = (দ্ৱিগুণ*)ptrs[3];
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

স্থির শূন্য math_op(ew::শীৰ্ষ op, প্ৰবেশ_Array _src, OutputArray _dst)
{
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    যদি (src.empty())
        ঘূৰাই;

    ধ্রুৱ size_t total = src.total() * src.channels();
    যদি (src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        স্থির ধ্রুৱ দ্ৱিগুণ noparams[4] = {};
        k.fptr(src.data, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.data, 0, (পূর্ণাঙ্ক)total, 1, noparams, k.flags, k.userdata);
        ঘূৰাই;
    }

    ew::TExpr p;
    ধ্রুৱ পূর্ণাঙ্ক a = p.add_প্ৰবেশ(depth);
    ধ্রুৱ পূর্ণাঙ্ক out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    ধ্রুৱ Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

শূন্য exp( প্ৰবেশ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] পূর্ণাঙ্ক depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

শূন্য log( প্ৰবেশ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] পূর্ণাঙ্ক depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
সংরচনা iPow_SIMD
{
    পূর্ণাঙ্ক operator() ( ধ্রুৱ T *, T *, পূর্ণাঙ্ক, পূর্ণাঙ্ক)
    {
        ঘূৰাই 0;
    }
};

#যদি (CV_SIMD || CV_SIMD_SCALABLE)

template <>
সংরচনা iPow_SIMD<uchar, unsigned>
{
    পূর্ণাঙ্ক operator() ( ধ্রুৱ uchar * src, uchar * dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power )
    {
        পূর্ণাঙ্ক i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        বাবে ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            পূর্ণাঙ্ক p = power;

            যেতিয়ালৈ( p > 1 )
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

        ঘূৰাই i;
    }
};

template <>
সংরচনা iPow_SIMD<schar, পূর্ণাঙ্ক>
{
    পূর্ণাঙ্ক operator() ( ধ্রুৱ schar * src, schar * dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power)
    {
        পূর্ণাঙ্ক i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        বাবে ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            পূর্ণাঙ্ক p = power;

            যেতিয়ালৈ( p > 1 )
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

        ঘূৰাই i;
    }
};

template <>
সংরচনা iPow_SIMD<ushort, unsigned>
{
    পূর্ণাঙ্ক operator() ( ধ্রুৱ ushort * src, ushort * dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power)
    {
        পূর্ণাঙ্ক i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        বাবে ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            পূর্ণাঙ্ক p = power;

            যেতিয়ালৈ( p > 1 )
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

        ঘূৰাই i;
    }
};

template <>
সংরচনা iPow_SIMD<short, পূর্ণাঙ্ক>
{
    পূর্ণাঙ্ক operator() ( ধ্রুৱ short * src, short * dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power)
    {
        পূর্ণাঙ্ক i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        বাবে ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            পূর্ণাঙ্ক p = power;

            যেতিয়ালৈ( p > 1 )
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

        ঘূৰাই i;
    }
};

template <>
সংরচনা iPow_SIMD<পূর্ণাঙ্ক, পূর্ণাঙ্ক>
{
    পূর্ণাঙ্ক operator() ( ধ্রুৱ পূর্ণাঙ্ক * src, পূর্ণাঙ্ক * dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power)
    {
        পূর্ণাঙ্ক i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        বাবে ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            পূর্ণাঙ্ক p = power;

            যেতিয়ালৈ( p > 1 )
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

        ঘূৰাই i;
    }
};

template <>
সংরচনা iPow_SIMD<ভিন্ন, ভিন্ন>
{
    পূর্ণাঙ্ক operator() ( ধ্রুৱ ভিন্ন * src, ভিন্ন * dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power)
    {
        পূর্ণাঙ্ক i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        বাবে ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            পূর্ণাঙ্ক p = std::abs(power);
            যদি( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            যেতিয়ালৈ( p > 1 )
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

        ঘূৰাই i;
    }
};

#যদি (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
সংরচনা iPow_SIMD<দ্ৱিগুণ, দ্ৱিগুণ>
{
    পূর্ণাঙ্ক operator() ( ধ্রুৱ দ্ৱিগুণ * src, দ্ৱিগুণ * dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power)
    {
        পূর্ণাঙ্ক i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        বাবে ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            পূর্ণাঙ্ক p = std::abs(power);
            যদি( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            যেতিয়ালৈ( p > 1 )
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

        ঘূৰাই i;
    }
};
#endif

#endif

template<typename T, typename WT>
স্থির শূন্য
iPow_i( ধ্রুৱ শূন্য* _src, শূন্য* _dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power )
{
    ধ্রুৱ T* src = (ধ্রুৱ T*)_src;
    T* dst = (T*)_dst;
    যদি( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        বাবে( পূর্ণাঙ্ক i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    অন্যথা
    {
        iPow_SIMD<T, WT> vop;
        পূর্ণাঙ্ক i = vop(src, dst, len, power);

        বাবে( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            পূর্ণাঙ্ক p = power;
            যেতিয়ালৈ( p > 1 )
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
iPow_f( ধ্রুৱ শূন্য* _src, শূন্য* _dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power0 )
{
    ধ্রুৱ T* src = (ধ্রুৱ T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    পূর্ণাঙ্ক i = vop(src, dst, len, power0);
    পূর্ণাঙ্ক power = std::abs(power0);

    বাবে( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        পূর্ণাঙ্ক p = power;
        যদি( power0 < 0 )
            b = 1/b;

        যেতিয়ালৈ( p > 1 )
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

প্রকারনির্দেশ শূন্য (*IPowFunc)( ধ্রুৱ শূন্য* src, শূন্য* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক power );

স্থির IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, পূর্ণাঙ্ক>, iPow_i<ushort, unsigned>, iPow_i<short, পূর্ণাঙ্ক>,
    iPow_i<পূর্ণাঙ্ক, পূর্ণাঙ্ক>, iPow_f<ভিন্ন>, iPow_f<দ্ৱিগুণ>, 0
};

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_pow(প্ৰবেশ_Array _src, দ্ৱিগুণ power, OutputArray _dst,
                    তার্কিক is_ipower, পূর্ণাঙ্ক ipower)
{
    ধ্রুৱ ocl::Device & d = ocl::Device::getDefault();
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    তার্কিক doubleSupport = d.doubleFPConfig() > 0;

    _dst.createSameSize(_src, type);
    যদি (is_ipower)
    {
        যদি( ipower < 0 )
        {
            যদি( depth == CV_32F || depth == CV_64F )
                is_ipower = মিছা;
            অন্যথা
                ঘূৰাই মিছা;
        }
    }

    যদি (depth == CV_64F && !doubleSupport)
        ঘূৰাই মিছা;

    তার্কিক issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    ধ্রুৱ ৱর্ণ * ধ্রুৱ op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    যদি (is_ipower)
    {
        পূর্ণাঙ্ক wdepth = CV_32F;
        যদি (depth == CV_64F)
            wdepth = CV_64F;
        অন্যথা যদি (depth == CV_16F)
            wdepth = CV_16F;

        ৱর্ণ cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], পরিমাণ(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], পরিমাণ(cvt[1])),
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
        ঘূৰাই মিছা;

    UMat src = _src.getUMat();
    _dst.createSameSize(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    যদি (issqrt)
        k.args(srcarg, dstarg);
    অন্যথা যদি (is_ipower)
        k.args(srcarg, dstarg, ipower);
    অন্যথা
    {
        যদি (depth == CV_32F)
            k.args(srcarg, dstarg, (ভিন্ন)power);
        অন্যথা
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ঘূৰাই k.run(2, globalsize, রিক্ত, মিছা);
}

#endif

শূন্য pow( প্ৰবেশ_Array _src, দ্ৱিগুণ power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    তার্কিক is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    তার্কিক useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    যদি (is_ipower)
    {
        ৱিকল্প( ipower )
        {
        পক্ষ 0:
            _dst.createSameSize(_src, type);
            _dst.setTo(Scalar::all(1));
            ঘূৰাই;
        পক্ষ 1:
            _src.copyTo(_dst);
            ঘূৰাই;
        পক্ষ 2:
            multiply(_src, _src, _dst);
            ঘূৰাই;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ধ্রুৱ তার্কিক floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    যদি( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.createSameSize(_src, type);
        Mat dst = _dst.getMat();

        ধ্রুৱ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        পূর্ণাঙ্ক len = (পূর্ণাঙ্ক)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        ঘূৰাই;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);
    Mat dst = _dst.getMat();
    যদি (src.empty())
        ঘূৰাই;

    ধ্রুৱ size_t total = src.total() * cn;
    যদি (floatDepth && src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        যদি (k.fptr)
        {
            দ্ৱিগুণ pvstore;                    // the broadcast exponent, stored as T
            শূন্য* pv = &pvstore;
            ৱিকল্প (depth)
            {
            পক্ষ CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); ভঙ্গ;
            পক্ষ CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); ভঙ্গ;
            পক্ষ CV_32F:  *(ভিন্ন*)pv = (ভিন্ন)power; ভঙ্গ;
            স্ৱভাৱ:      pvstore = power; ভঙ্গ;
            }
            স্থির ধ্রুৱ দ্ৱিগুণ noparams[4] = {};
            k.fptr(src.data, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.data, 0, (পূর্ণাঙ্ক)total, 1, noparams, k.flags, k.userdata);
            ঘূৰাই;
        }
    }

    ew::TExpr prog;
    ধ্রুৱ পূর্ণাঙ্ক a = prog.add_প্ৰবেশ(depth);
    ধ্রুৱ পূর্ণাঙ্ক c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    ধ্রুৱ পূর্ণাঙ্ক out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_দ্বিমিক(ew::OP_POW, a, c, depth), out);
    prog.compile();
    ধ্রুৱ Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

শূন্য sqrt(প্ৰবেশ_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    যদি (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        ঘূৰাই;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<পূর্ণাঙ্ক cv_mat_type> সংরচনা mat_type_assotiations{};

template<> সংরচনা mat_type_assotiations<CV_8U>
{
    প্রকারনির্দেশ unsigned ৱর্ণ type;
    স্থির ধ্রুৱ type min_allowable = 0x0;
    স্থির ধ্রুৱ type max_allowable = 0xFF;
};

template<> সংরচনা mat_type_assotiations<CV_8S>
{
    প্রকারনির্দেশ signed ৱর্ণ type;
    স্থির ধ্রুৱ type min_allowable = SCHAR_MIN;
    স্থির ধ্রুৱ type max_allowable = SCHAR_MAX;
};

template<> সংরচনা mat_type_assotiations<CV_16U>
{
    প্রকারনির্দেশ unsigned short type;
    স্থির ধ্রুৱ type min_allowable = 0x0;
    স্থির ধ্রুৱ type max_allowable = USHRT_MAX;
};
template<> সংরচনা mat_type_assotiations<CV_16S>
{
    প্রকারনির্দেশ signed short type;
    স্থির ধ্রুৱ type min_allowable = SHRT_MIN;
    স্থির ধ্রুৱ type max_allowable = SHRT_MAX;
};

template<> সংরচনা mat_type_assotiations<CV_32S>
{
    প্রকারনির্দেশ পূর্ণাঙ্ক type;
    স্থির ধ্রুৱ type min_allowable = (-INT_MAX - 1);
    স্থির ধ্রুৱ type max_allowable = INT_MAX;
};

template<পূর্ণাঙ্ক depth>
স্থির তার্কিক checkIntegerRange(cv::Mat src, Point& bad_pt, পূর্ণাঙ্ক minVal, পূর্ণাঙ্ক maxVal)
{
    প্রকারনির্দেশ mat_type_assotiations<depth> type_ass;

    যদি (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        ঘূৰাই সঁচা;
    }
    অন্যথা যদি (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        ঘূৰাই মিছা;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    বাবে (পূর্ণাঙ্ক j = 0; j < as_one_channel.rows; ++j)
        বাবে (পূর্ণাঙ্ক i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            যদি (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                ঘূৰাই মিছা;
            }
        }

    ঘূৰাই সঁচা;
}

প্রকারনির্দেশ তার্কিক (*check_range_function)(cv::Mat src, Point& bad_pt, পূর্ণাঙ্ক minVal, পূর্ণাঙ্ক maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
স্থির শূন্য checkTypedRange(Mat& src, Point& badPt, দ্ৱিগুণ minVal, দ্ৱিগুণ maxVal)
{
    পূর্ণাঙ্ক loc = 0;
    পূর্ণাঙ্ক cn = src.channels();
    Size size = getContinuousSize2D(src, cn);
    ধ্রুৱ T* isrc = src.ptr<T>();
    size_t step = src.step / পরিমাণ(isrc[0]);

    বাবে( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        বাবে( পূর্ণাঙ্ক i = 0; i < size.width; i++ )
        {
            দ্ৱিগুণ val = (দ্ৱিগুণ)isrc[i];
            যদি( !(val >= minVal && val < maxVal) )
            {
                পূর্ণাঙ্ক pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                ভঙ্গ;
            }
        }
    }
}

তার্কিক checkRange(প্ৰবেশ_Array _src, তার্কিক quiet, Point* pt, দ্ৱিগুণ minVal, দ্ৱিগুণ maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    যদি ( src.dims > 2 )
    {
        CV_Assert(pt == রিক্ত); // no way to provide location info

        ধ্রুৱ Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        বাবে ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি (!checkRange( it.planes[0], quiet, রিক্ত, minVal, maxVal ))
            {
                ঘূৰাই মিছা;
            }
        }
        ঘূৰাই সঁচা;
    }

    পূর্ণাঙ্ক depth = src.depth();
    Point badPt(-1, -1);

    যদি (depth < CV_32F)
    {
        পূর্ণাঙ্ক minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        পূর্ণাঙ্ক maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    অন্যথা
    {
        পূর্ণাঙ্ক i, loc = 0;
        পূর্ণাঙ্ক cn = src.channels();
        Size size = getContinuousSize2D(src, cn);

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
            পূর্ণাঙ্ক ia, ib;
            ধ্রুৱ পূর্ণাঙ্ক* isrc = src.ptr<পূর্ণাঙ্ক>();
            size_t step = src.step/পরিমাণ(isrc[0]);

            a.f = (ভিন্ন)std::max(minVal, (দ্ৱিগুণ)-FLT_MAX);
            b.f = (ভিন্ন)std::min(maxVal, (দ্ৱিগুণ)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            বাবে( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                বাবে( i = 0; i < size.width; i++ )
                {
                    পূর্ণাঙ্ক val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    যদি( val < ia || val >= ib )
                    {
                        পূর্ণাঙ্ক pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        ভঙ্গ;
                    }
                }
            }
        }
        অন্যথা যদি( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ধ্রুৱ int64* isrc = src.ptr<int64>();
            size_t step = src.step/পরিমাণ(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            বাবে( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                বাবে( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    যদি( val < ia || val >= ib )
                    {
                        পূর্ণাঙ্ক pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        ভঙ্গ;
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
        ঘূৰাই মিছা;
    }
    ঘূৰাই সঁচা;
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

পূর্ণাঙ্ক cv::solveCubic( প্ৰবেশ_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ধ্রুৱ পূর্ণাঙ্ক n0 = 3;
    Mat coeffs = _coeffs.getMat();
    পূর্ণাঙ্ক ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == Size(n0, 1) ||
                coeffs.size() == Size(n0+1, 1) ||
                coeffs.size() == Size(1, n0) ||
                coeffs.size() == Size(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, সঁচা, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    পূর্ণাঙ্ক i = -1, n = 0;
    দ্ৱিগুণ a0 = 1., a1, a2, a3;
    দ্ৱিগুণ x0 = 0., x1 = 0., x2 = 0.;
    পূর্ণাঙ্ক ncoeffs = coeffs.rows + coeffs.cols - 1;

    যদি( ctype == CV_32FC1 )
    {
        যদি( ncoeffs == 4 )
            a0 = coeffs.at<ভিন্ন>(++i);

        a1 = coeffs.at<ভিন্ন>(i+1);
        a2 = coeffs.at<ভিন্ন>(i+2);
        a3 = coeffs.at<ভিন্ন>(i+3);
    }
    অন্যথা
    {
        যদি( ncoeffs == 4 )
            a0 = coeffs.at<দ্ৱিগুণ>(++i);

        a1 = coeffs.at<দ্ৱিগুণ>(i+1);
        a2 = coeffs.at<দ্ৱিগুণ>(i+2);
        a3 = coeffs.at<দ্ৱিগুণ>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    দ্ৱিগুণ max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    যদি (max_coeff < std::numeric_limits<দ্ৱিগুণ>::epsilon())
        ঘূৰাই -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    যদি( std::abs(a0) < std::numeric_limits<দ্ৱিগুণ>::epsilon() * max_coeff )
    {
        যদি( std::abs(a1) < std::numeric_limits<দ্ৱিগুণ>::epsilon() * max_coeff )
        {
            যদি( std::abs(a2) < std::numeric_limits<দ্ৱিগুণ>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<দ্ৱিগুণ>::epsilon() * max_coeff ? -1 : 0;
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
            দ্ৱিগুণ d = a2*a2 - 4*a1*a3;
            যদি( d >= 0 )
            {
                d = std::sqrt(d);
                দ্ৱিগুণ q1 = (-a2 + d) * 0.5;
                দ্ৱিগুণ q2 = (a2 + d) * -0.5;
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
        দ্ৱিগুণ scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        দ্ৱিগুণ Q = (a1 * a1 - 3 * a2) * (1./9);
        দ্ৱিগুণ R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        দ্ৱিগুণ Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        দ্ৱিগুণ d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        যদি( d > 0 )
        {
            দ্ৱিগুণ theta = acos(R / sqrt(Qcubed));
            দ্ৱিগুণ sqrtQ = sqrt(Q);
            দ্ৱিগুণ t0 = -2 * sqrtQ;
            দ্ৱিগুণ t1 = theta * (1./3);
            দ্ৱিগুণ t2 = a1 * (1./3);
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
            দ্ৱিগুণ e;
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
        roots.at<ভিন্ন>(0) = (ভিন্ন)x0;
        roots.at<ভিন্ন>(1) = (ভিন্ন)x1;
        roots.at<ভিন্ন>(2) = (ভিন্ন)x2;
    }
    অন্যথা
    {
        roots.at<দ্ৱিগুণ>(0) = x0;
        roots.at<দ্ৱিগুণ>(1) = x1;
        roots.at<দ্ৱিগুণ>(2) = x2;
    }

    ঘূৰাই n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
দ্ৱিগুণ cv::solvePoly( প্ৰবেশ_Array _coeffs0, OutputArray _roots0, পূর্ণাঙ্ক maxIters )
{
    CV_INSTRUMENT_REGION();

    প্রকারনির্দেশ Complex<দ্ৱিগুণ> C;

    দ্ৱিগুণ maxDiff = 0;
    পূর্ণাঙ্ক iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    পূর্ণাঙ্ক ctype = _coeffs0.type();
    পূর্ণাঙ্ক cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.total() >= 2 );

    পূর্ণাঙ্ক n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, সঁচা, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    AutoBuffer<C> buf(n*2+2);
    C *coeffs = buf.data(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    যদি( coeffs0.channels() == 1 )
    {
        ধ্রুৱ দ্ৱিগুণ* rcoeffs = (ধ্রুৱ দ্ৱিগুণ*)roots;
        বাবে( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    বাবে( ; n > 1; n-- )
    {
        যদি( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            ভঙ্গ;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    বাবে( i = 0; i <= n; i++ )
        absCoeffs.at<দ্ৱিগুণ>(i) = abs(coeffs[i]);

    পূর্ণাঙ্ক nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    বাবে( i = 0; i <= n; i++ )
    {
        দ্ৱিগুণ coeff = absCoeffs.at<দ্ৱিগুণ>(i);
        যদি( coeff > DBL_EPSILON )
        {
            যদি( i != n )
                u.at<দ্ৱিগুণ>(i) = 2.0 * pow(coeff / absCoeffs.at<দ্ৱিগুণ>(n), 1.0 / (n - i));
            যদি( i != 0 )
                v.at<দ্ৱিগুণ>(i - 1) = 0.5 * pow(absCoeffs.at<দ্ৱিগুণ>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    দ্ৱিগুণ scale = 1;
    যদি( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<দ্ৱিগুণ>(maxU) = 0;
        v.at<দ্ৱিগুণ>(minV) = 0;
        scale = (sum(u).val[0] + sum(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    বাবে( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    বাবে( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        বাবে( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            পূর্ণাঙ্ক অঙ্ক_same_মূল = 1;
            বাবে( j = 0; j < n; j++ )
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
                দ্ৱিগুণ old_অঙ্ক_re = num.re;
                দ্ৱিগুণ old_অঙ্ক_im = num.im;
                পূর্ণাঙ্ক square_মূল_times = অঙ্ক_same_মূল % 2 == 0 ? অঙ্ক_same_মূল / 2 : অঙ্ক_same_মূল / 2 - 1;

                বাবে( j = 0; j < square_মূল_times; j++)
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
                    cube_coefs.at<দ্ৱিগুণ>(3) = -(std::pow(old_অঙ্ক_re, 3));
                    cube_coefs.at<দ্ৱিগুণ>(2) = -(15*std::pow(old_অঙ্ক_re, 2) + 27*std::pow(old_অঙ্ক_im, 2));
                    cube_coefs.at<দ্ৱিগুণ>(1) = -48*old_অঙ্ক_re;
                    cube_coefs.at<দ্ৱিগুণ>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<দ্ৱিগুণ>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_অঙ্ক_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        যদি( maxDiff <= 0 )
            ভঙ্গ;
    }

    যদি( coeffs0.channels() == 1 )
    {
        ধ্রুৱ দ্ৱিগুণ verySmallEps = 1e-100;
        বাবে( i = 0; i < n; i++ )
            যদি( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    বাবে( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    ঘূৰাই maxDiff;
}


/* End of file. */
