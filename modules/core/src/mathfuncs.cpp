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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ <atomic>
#அந்தர்பாவ <limits>
#அந்தர்பாவ <iostream>
#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <cmath>
#அந்தர்பாவ "mathfuncs.hpp"
#அந்தர்பாவ "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

நாமாகாஶ cv
{

வகைவரை வெறுமை (*MathFunc)(மாறிலி வெறுமை* src, வெறுமை* dst, முழு len);

#ifdef HAVE_OPENCL

எண்ணிக்கை { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

நிலையான மாறிலி எழுத்து* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

நிலையான தருக்க ocl_math_op(உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, OutputArray _dst, முழு oclop)
{
    முழு type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    முழு kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    மாறிலி ocl::Device d = ocl::Device::getDefault();
    தருக்க double_support = d.doubleFPConfig() > 0;
    எனில் (!double_support && depth == CV_64F)
        திரும்பு பொய்;
    முழு rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.createSameSize(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    எனில் (src2.empty())
        k.args(src1arg, dstarg);
    இல்லையெனில்
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, 0, பொய்);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN தசம cube_வேர்( தசம value )
{
    CV_INSTRUMENT_REGION();

    தசம fr;
    Cv32suf v, m;
    முழு ix, s;
    முழு ex, shx;

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
    fr = (தசம)(((((45.2548339756803022511987494 * fr +
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
    திரும்பு v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

வெறுமை magnitude( உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    முழு type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    மாறிலி Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு len = (முழு)it.size*cn;

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        எனில்( depth == CV_32F )
        {
            மாறிலி தசம *x = (மாறிலி தசம*)ptrs[0], *y = (மாறிலி தசம*)ptrs[1];
            தசம *mag = (தசம*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        இல்லையெனில்
        {
            மாறிலி இரட்டை *x = (மாறிலி இரட்டை*)ptrs[0], *y = (மாறிலி இரட்டை*)ptrs[1];
            இரட்டை *mag = (இரட்டை*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

வெறுமை phase( உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst, தருக்க angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    முழு type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    மாறிலி Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு j, total = (முழு)(it.size*cn), blockSize = total;
    size_t esz1 = X.elemSize1();
    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( j = 0; j < total; j += blockSize )
        {
            முழு len = std::min(total - j, blockSize);
            எனில்( depth == CV_32F )
            {
                மாறிலி தசம *x = (மாறிலி தசம*)ptrs[0], *y = (மாறிலி தசம*)ptrs[1];
                தசம *angle = (தசம*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            இல்லையெனில்
            {
                மாறிலி இரட்டை *x = (மாறிலி இரட்டை*)ptrs[0], *y = (மாறிலி இரட்டை*)ptrs[1];
                இரட்டை *angle = (இரட்டை*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_cartToPolar( உள்ளீடு_Array _src1, உள்ளீடு_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, தருக்க angleInDegrees )
{
    மாறிலி ocl::Device & d = ocl::Device::getDefault();
    முழு type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    தருக்க doubleSupport = d.doubleFPConfig() > 0;

    மாறிலி தருக்க _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    மாறிலி தருக்க _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    மாறிலி தருக்க _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    மாறிலி தருக்க _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    எனில் ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        திரும்பு பொய்;

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
    எனில் (k.empty())
        திரும்பு பொய்;

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
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

வெறுமை cartToPolar( உள்ளீடு_Array src1, உள்ளீடு_Array src2,
                  OutputArray dst1, OutputArray dst2, தருக்க angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    முழு type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    மாறிலி Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு j, total = (முழு)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( j = 0; j < total; j += blockSize )
        {
            முழு len = std::min(total - j, blockSize);
            எனில்( depth == CV_32F )
            {
                மாறிலி தசம *x = (மாறிலி தசம*)ptrs[0], *y = (மாறிலி தசம*)ptrs[1];
                தசம *mag = (தசம*)ptrs[2], *angle = (தசம*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            இல்லையெனில்
            {
                மாறிலி இரட்டை *x = (மாறிலி இரட்டை*)ptrs[0], *y = (மாறிலி இரட்டை*)ptrs[1];
                இரட்டை *mag = (இரட்டை*)ptrs[2], *angle = (இரட்டை*)ptrs[3];
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

நிலையான தருக்க ocl_polarToCart( உள்ளீடு_Array _mag, உள்ளீடு_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, தருக்க angleInDegrees )
{
    மாறிலி ocl::Device & d = ocl::Device::getDefault();
    முழு type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    தருக்க doubleSupport = d.doubleFPConfig() > 0;

    மாறிலி தருக்க _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    மாறிலி தருக்க _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    மாறிலி தருக்க _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    மாறிலி தருக்க _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    எனில் ( !doubleSupport && depth == CV_64F )
        திரும்பு பொய்;

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
    எனில் (k.empty())
        திரும்பு பொய்;

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
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

வெறுமை polarToCart( உள்ளீடு_Array src1, உள்ளீடு_Array src2,
                  OutputArray dst1, OutputArray dst2, தருக்க angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    முழு type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    மாறிலி Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு j, total = (முழு)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( j = 0; j < total; j += blockSize )
        {
            முழு len = std::min(total - j, blockSize);
            எனில் ( depth == CV_32F )
            {
                மாறிலி தசம *mag = (மாறிலி தசம*)ptrs[0], *angle = (மாறிலி தசம*)ptrs[1];
                தசம *x = (தசம*)ptrs[2], *y = (தசம*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            இல்லையெனில்
            {
                மாறிலி இரட்டை *mag = (மாறிலி இரட்டை*)ptrs[0], *angle = (மாறிலி இரட்டை*)ptrs[1];
                இரட்டை *x = (இரட்டை*)ptrs[2], *y = (இரட்டை*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            எனில்( ptrs[0] )
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
எண்ணிக்கை { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

நிலையான வெறுமை math_op(ew::உச்சி op, உள்ளீடு_Array _src, OutputArray _dst)
{
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    எனில் (src.empty())
        திரும்பு;

    மாறிலி size_t total = src.total() * src.channels();
    எனில் (src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        நிலையான மாறிலி இரட்டை noparams[4] = {};
        k.fptr(src.data, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.data, 0, (முழு)total, 1, noparams, k.flags, k.userdata);
        திரும்பு;
    }

    ew::TExpr p;
    மாறிலி முழு a = p.add_உள்ளீடு(depth);
    மாறிலி முழு out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    மாறிலி Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

வெறுமை exp( உள்ளீடு_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] முழு depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

வெறுமை log( உள்ளீடு_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] முழு depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
கட்டமைப்பு iPow_SIMD
{
    முழு operator() ( மாறிலி T *, T *, முழு, முழு)
    {
        திரும்பு 0;
    }
};

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)

template <>
கட்டமைப்பு iPow_SIMD<uchar, unsigned>
{
    முழு operator() ( மாறிலி uchar * src, uchar * dst, முழு len, முழு power )
    {
        முழு i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        க்கு ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            முழு p = power;

            போது( p > 1 )
            {
                எனில் (p & 1)
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

        திரும்பு i;
    }
};

template <>
கட்டமைப்பு iPow_SIMD<schar, முழு>
{
    முழு operator() ( மாறிலி schar * src, schar * dst, முழு len, முழு power)
    {
        முழு i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        க்கு ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            முழு p = power;

            போது( p > 1 )
            {
                எனில் (p & 1)
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

        திரும்பு i;
    }
};

template <>
கட்டமைப்பு iPow_SIMD<ushort, unsigned>
{
    முழு operator() ( மாறிலி ushort * src, ushort * dst, முழு len, முழு power)
    {
        முழு i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        க்கு ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            முழு p = power;

            போது( p > 1 )
            {
                எனில் (p & 1)
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

        திரும்பு i;
    }
};

template <>
கட்டமைப்பு iPow_SIMD<short, முழு>
{
    முழு operator() ( மாறிலி short * src, short * dst, முழு len, முழு power)
    {
        முழு i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        க்கு ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            முழு p = power;

            போது( p > 1 )
            {
                எனில் (p & 1)
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

        திரும்பு i;
    }
};

template <>
கட்டமைப்பு iPow_SIMD<முழு, முழு>
{
    முழு operator() ( மாறிலி முழு * src, முழு * dst, முழு len, முழு power)
    {
        முழு i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        க்கு ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            முழு p = power;

            போது( p > 1 )
            {
                எனில் (p & 1)
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

        திரும்பு i;
    }
};

template <>
கட்டமைப்பு iPow_SIMD<தசம, தசம>
{
    முழு operator() ( மாறிலி தசம * src, தசம * dst, முழு len, முழு power)
    {
        முழு i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        க்கு ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            முழு p = std::abs(power);
            எனில்( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            போது( p > 1 )
            {
                எனில் (p & 1)
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

        திரும்பு i;
    }
};

#எனில் (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
கட்டமைப்பு iPow_SIMD<இரட்டை, இரட்டை>
{
    முழு operator() ( மாறிலி இரட்டை * src, இரட்டை * dst, முழு len, முழு power)
    {
        முழு i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        க்கு ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            முழு p = std::abs(power);
            எனில்( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            போது( p > 1 )
            {
                எனில் (p & 1)
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

        திரும்பு i;
    }
};
#endif

#endif

template<typename T, typename WT>
நிலையான வெறுமை
iPow_i( மாறிலி வெறுமை* _src, வெறுமை* _dst, முழு len, முழு power )
{
    மாறிலி T* src = (மாறிலி T*)_src;
    T* dst = (T*)_dst;
    எனில்( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        க்கு( முழு i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    இல்லையெனில்
    {
        iPow_SIMD<T, WT> vop;
        முழு i = vop(src, dst, len, power);

        க்கு( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            முழு p = power;
            போது( p > 1 )
            {
                எனில்( p & 1 )
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
நிலையான வெறுமை
iPow_f( மாறிலி வெறுமை* _src, வெறுமை* _dst, முழு len, முழு power0 )
{
    மாறிலி T* src = (மாறிலி T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    முழு i = vop(src, dst, len, power0);
    முழு power = std::abs(power0);

    க்கு( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        முழு p = power;
        எனில்( power0 < 0 )
            b = 1/b;

        போது( p > 1 )
        {
            எனில்( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

வகைவரை வெறுமை (*IPowFunc)( மாறிலி வெறுமை* src, வெறுமை* dst, முழு len, முழு power );

நிலையான IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, முழு>, iPow_i<ushort, unsigned>, iPow_i<short, முழு>,
    iPow_i<முழு, முழு>, iPow_f<தசம>, iPow_f<இரட்டை>, 0
};

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_pow(உள்ளீடு_Array _src, இரட்டை power, OutputArray _dst,
                    தருக்க is_ipower, முழு ipower)
{
    மாறிலி ocl::Device & d = ocl::Device::getDefault();
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    தருக்க doubleSupport = d.doubleFPConfig() > 0;

    _dst.createSameSize(_src, type);
    எனில் (is_ipower)
    {
        எனில்( ipower < 0 )
        {
            எனில்( depth == CV_32F || depth == CV_64F )
                is_ipower = பொய்;
            இல்லையெனில்
                திரும்பு பொய்;
        }
    }

    எனில் (depth == CV_64F && !doubleSupport)
        திரும்பு பொய்;

    தருக்க issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    மாறிலி எழுத்து * மாறிலி op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    எனில் (is_ipower)
    {
        முழு wdepth = CV_32F;
        எனில் (depth == CV_64F)
            wdepth = CV_64F;
        இல்லையெனில் எனில் (depth == CV_16F)
            wdepth = CV_16F;

        எழுத்து cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], அளவு(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], அளவு(cvt[1])),
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
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src = _src.getUMat();
    _dst.createSameSize(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    எனில் (issqrt)
        k.args(srcarg, dstarg);
    இல்லையெனில் எனில் (is_ipower)
        k.args(srcarg, dstarg, ipower);
    இல்லையெனில்
    {
        எனில் (depth == CV_32F)
            k.args(srcarg, dstarg, (தசம)power);
        இல்லையெனில்
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

வெறுமை pow( உள்ளீடு_Array _src, இரட்டை power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    முழு type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    தருக்க is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    தருக்க useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    எனில் (is_ipower)
    {
        தேர்வு( ipower )
        {
        நிலை 0:
            _dst.createSameSize(_src, type);
            _dst.setTo(Scalar::all(1));
            திரும்பு;
        நிலை 1:
            _src.copyTo(_dst);
            திரும்பு;
        நிலை 2:
            multiply(_src, _src, _dst);
            திரும்பு;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    மாறிலி தருக்க floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    எனில்( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.createSameSize(_src, type);
        Mat dst = _dst.getMat();

        மாறிலி Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        முழு len = (முழு)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        திரும்பு;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);
    Mat dst = _dst.getMat();
    எனில் (src.empty())
        திரும்பு;

    மாறிலி size_t total = src.total() * cn;
    எனில் (floatDepth && src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        எனில் (k.fptr)
        {
            இரட்டை pvstore;                    // the broadcast exponent, stored as T
            வெறுமை* pv = &pvstore;
            தேர்வு (depth)
            {
            நிலை CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); நிறுத்து;
            நிலை CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); நிறுத்து;
            நிலை CV_32F:  *(தசம*)pv = (தசம)power; நிறுத்து;
            இயல்பு:      pvstore = power; நிறுத்து;
            }
            நிலையான மாறிலி இரட்டை noparams[4] = {};
            k.fptr(src.data, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.data, 0, (முழு)total, 1, noparams, k.flags, k.userdata);
            திரும்பு;
        }
    }

    ew::TExpr prog;
    மாறிலி முழு a = prog.add_உள்ளீடு(depth);
    மாறிலி முழு c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    மாறிலி முழு out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_இருமம்(ew::OP_POW, a, c, depth), out);
    prog.compile();
    மாறிலி Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

வெறுமை sqrt(உள்ளீடு_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    எனில் (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        திரும்பு;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<முழு cv_mat_type> கட்டமைப்பு mat_type_assotiations{};

template<> கட்டமைப்பு mat_type_assotiations<CV_8U>
{
    வகைவரை unsigned எழுத்து type;
    நிலையான மாறிலி type min_allowable = 0x0;
    நிலையான மாறிலி type max_allowable = 0xFF;
};

template<> கட்டமைப்பு mat_type_assotiations<CV_8S>
{
    வகைவரை signed எழுத்து type;
    நிலையான மாறிலி type min_allowable = SCHAR_MIN;
    நிலையான மாறிலி type max_allowable = SCHAR_MAX;
};

template<> கட்டமைப்பு mat_type_assotiations<CV_16U>
{
    வகைவரை unsigned short type;
    நிலையான மாறிலி type min_allowable = 0x0;
    நிலையான மாறிலி type max_allowable = USHRT_MAX;
};
template<> கட்டமைப்பு mat_type_assotiations<CV_16S>
{
    வகைவரை signed short type;
    நிலையான மாறிலி type min_allowable = SHRT_MIN;
    நிலையான மாறிலி type max_allowable = SHRT_MAX;
};

template<> கட்டமைப்பு mat_type_assotiations<CV_32S>
{
    வகைவரை முழு type;
    நிலையான மாறிலி type min_allowable = (-INT_MAX - 1);
    நிலையான மாறிலி type max_allowable = INT_MAX;
};

template<முழு depth>
நிலையான தருக்க checkIntegerRange(cv::Mat src, Point& bad_pt, முழு minVal, முழு maxVal)
{
    வகைவரை mat_type_assotiations<depth> type_ass;

    எனில் (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        திரும்பு உண்மை;
    }
    இல்லையெனில் எனில் (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        திரும்பு பொய்;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    க்கு (முழு j = 0; j < as_one_channel.rows; ++j)
        க்கு (முழு i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            எனில் (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                திரும்பு பொய்;
            }
        }

    திரும்பு உண்மை;
}

வகைவரை தருக்க (*check_range_function)(cv::Mat src, Point& bad_pt, முழு minVal, முழு maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
நிலையான வெறுமை checkTypedRange(Mat& src, Point& badPt, இரட்டை minVal, இரட்டை maxVal)
{
    முழு loc = 0;
    முழு cn = src.channels();
    Size size = getContinuousSize2D(src, cn);
    மாறிலி T* isrc = src.ptr<T>();
    size_t step = src.step / அளவு(isrc[0]);

    க்கு( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        க்கு( முழு i = 0; i < size.width; i++ )
        {
            இரட்டை val = (இரட்டை)isrc[i];
            எனில்( !(val >= minVal && val < maxVal) )
            {
                முழு pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                நிறுத்து;
            }
        }
    }
}

தருக்க checkRange(உள்ளீடு_Array _src, தருக்க quiet, Point* pt, இரட்டை minVal, இரட்டை maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    எனில் ( src.dims > 2 )
    {
        CV_Assert(pt == வெற்று); // no way to provide location info

        மாறிலி Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        க்கு ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            எனில் (!checkRange( it.planes[0], quiet, வெற்று, minVal, maxVal ))
            {
                திரும்பு பொய்;
            }
        }
        திரும்பு உண்மை;
    }

    முழு depth = src.depth();
    Point badPt(-1, -1);

    எனில் (depth < CV_32F)
    {
        முழு minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        முழு maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    இல்லையெனில்
    {
        முழு i, loc = 0;
        முழு cn = src.channels();
        Size size = getContinuousSize2D(src, cn);

        எனில்( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        இல்லையெனில் எனில்( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        இல்லையெனில் எனில்( depth == CV_32F )
        {
            Cv32suf a, b;
            முழு ia, ib;
            மாறிலி முழு* isrc = src.ptr<முழு>();
            size_t step = src.step/அளவு(isrc[0]);

            a.f = (தசம)std::max(minVal, (இரட்டை)-FLT_MAX);
            b.f = (தசம)std::min(maxVal, (இரட்டை)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            க்கு( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                க்கு( i = 0; i < size.width; i++ )
                {
                    முழு val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    எனில்( val < ia || val >= ib )
                    {
                        முழு pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        நிறுத்து;
                    }
                }
            }
        }
        இல்லையெனில் எனில்( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            மாறிலி int64* isrc = src.ptr<int64>();
            size_t step = src.step/அளவு(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            க்கு( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                க்கு( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    எனில்( val < ia || val >= ib )
                    {
                        முழு pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        நிறுத்து;
                    }
                }
            }
        }
        இல்லையெனில் எனில்( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        இல்லையெனில் எனில்( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        இல்லையெனில் எனில்( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        இல்லையெனில் எனில்( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        இல்லையெனில்
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    எனில்( badPt.x >= 0 )
    {
        எனில்( pt )
            *pt = badPt;
        எனில்( !quiet )
        {
            cv::String மதிப்பு_str;
            மதிப்பு_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, மதிப்பு_str.c_str(), minVal, maxVal));
        }
        திரும்பு பொய்;
    }
    திரும்பு உண்மை;
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

முழு cv::solveCubic( உள்ளீடு_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    மாறிலி முழு n0 = 3;
    Mat coeffs = _coeffs.getMat();
    முழு ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == Size(n0, 1) ||
                coeffs.size() == Size(n0+1, 1) ||
                coeffs.size() == Size(1, n0) ||
                coeffs.size() == Size(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, உண்மை, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    முழு i = -1, n = 0;
    இரட்டை a0 = 1., a1, a2, a3;
    இரட்டை x0 = 0., x1 = 0., x2 = 0.;
    முழு ncoeffs = coeffs.rows + coeffs.cols - 1;

    எனில்( ctype == CV_32FC1 )
    {
        எனில்( ncoeffs == 4 )
            a0 = coeffs.at<தசம>(++i);

        a1 = coeffs.at<தசம>(i+1);
        a2 = coeffs.at<தசம>(i+2);
        a3 = coeffs.at<தசம>(i+3);
    }
    இல்லையெனில்
    {
        எனில்( ncoeffs == 4 )
            a0 = coeffs.at<இரட்டை>(++i);

        a1 = coeffs.at<இரட்டை>(i+1);
        a2 = coeffs.at<இரட்டை>(i+2);
        a3 = coeffs.at<இரட்டை>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    இரட்டை max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    எனில் (max_coeff < std::numeric_limits<இரட்டை>::epsilon())
        திரும்பு -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    எனில்( std::abs(a0) < std::numeric_limits<இரட்டை>::epsilon() * max_coeff )
    {
        எனில்( std::abs(a1) < std::numeric_limits<இரட்டை>::epsilon() * max_coeff )
        {
            எனில்( std::abs(a2) < std::numeric_limits<இரட்டை>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<இரட்டை>::epsilon() * max_coeff ? -1 : 0;
            இல்லையெனில்
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        இல்லையெனில்
        {
            // quadratic equation
            இரட்டை d = a2*a2 - 4*a1*a3;
            எனில்( d >= 0 )
            {
                d = std::sqrt(d);
                இரட்டை q1 = (-a2 + d) * 0.5;
                இரட்டை q2 = (a2 + d) * -0.5;
                எனில்( std::abs(q1) > std::abs(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                இல்லையெனில்
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    இல்லையெனில்
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        இரட்டை scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        இரட்டை Q = (a1 * a1 - 3 * a2) * (1./9);
        இரட்டை R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        இரட்டை Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        இரட்டை d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        எனில்( d > 0 )
        {
            இரட்டை theta = acos(R / sqrt(Qcubed));
            இரட்டை sqrtQ = sqrt(Q);
            இரட்டை t0 = -2 * sqrtQ;
            இரட்டை t1 = theta * (1./3);
            இரட்டை t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        இல்லையெனில் எனில்( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        இல்லையெனில்
        {
            இரட்டை e;
            d = sqrt(-d);
            e = std::cbrt(d + fabs(R));
            எனில்( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    எனில்( roots.type() == CV_32FC1 )
    {
        roots.at<தசம>(0) = (தசம)x0;
        roots.at<தசம>(1) = (தசம)x1;
        roots.at<தசம>(2) = (தசம)x2;
    }
    இல்லையெனில்
    {
        roots.at<இரட்டை>(0) = x0;
        roots.at<இரட்டை>(1) = x1;
        roots.at<இரட்டை>(2) = x2;
    }

    திரும்பு n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
இரட்டை cv::solvePoly( உள்ளீடு_Array _coeffs0, OutputArray _roots0, முழு maxIters )
{
    CV_INSTRUMENT_REGION();

    வகைவரை Complex<இரட்டை> C;

    இரட்டை maxDiff = 0;
    முழு iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    முழு ctype = _coeffs0.type();
    முழு cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.total() >= 2 );

    முழு n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, உண்மை, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    AutoBuffer<C> buf(n*2+2);
    C *coeffs = buf.data(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    எனில்( coeffs0.channels() == 1 )
    {
        மாறிலி இரட்டை* rcoeffs = (மாறிலி இரட்டை*)roots;
        க்கு( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    க்கு( ; n > 1; n-- )
    {
        எனில்( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            நிறுத்து;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    க்கு( i = 0; i <= n; i++ )
        absCoeffs.at<இரட்டை>(i) = abs(coeffs[i]);

    முழு nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    க்கு( i = 0; i <= n; i++ )
    {
        இரட்டை coeff = absCoeffs.at<இரட்டை>(i);
        எனில்( coeff > DBL_EPSILON )
        {
            எனில்( i != n )
                u.at<இரட்டை>(i) = 2.0 * pow(coeff / absCoeffs.at<இரட்டை>(n), 1.0 / (n - i));
            எனில்( i != 0 )
                v.at<இரட்டை>(i - 1) = 0.5 * pow(absCoeffs.at<இரட்டை>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    இரட்டை scale = 1;
    எனில்( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<இரட்டை>(maxU) = 0;
        v.at<இரட்டை>(minV) = 0;
        scale = (sum(u).val[0] + sum(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    க்கு( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    க்கு( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        க்கு( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            முழு இலக்கம்_same_வேர் = 1;
            க்கு( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                எனில்( j != i )
                {
                    எனில் ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    இல்லையெனில்
                        இலக்கம்_same_வேர்++;
                }
            }
            num /= denom;
            எனில்( இலக்கம்_same_வேர் > 1)
            {
                இரட்டை old_இலக்கம்_re = num.re;
                இரட்டை old_இலக்கம்_im = num.im;
                முழு square_வேர்_times = இலக்கம்_same_வேர் % 2 == 0 ? இலக்கம்_same_வேர் / 2 : இலக்கம்_same_வேர் / 2 - 1;

                க்கு( j = 0; j < square_வேர்_times; j++)
                {
                    num.re = old_இலக்கம்_re*old_இலக்கம்_re + old_இலக்கம்_im*old_இலக்கம்_im;
                    num.re = sqrt(num.re);
                    num.re += old_இலக்கம்_re;
                    num.im = num.re - old_இலக்கம்_re;
                    num.re /= 2;
                    num.re = sqrt(num.re);

                    num.im /= 2;
                    num.im = sqrt(num.im);
                    எனில்( old_இலக்கம்_re < 0 ) num.im = -num.im;
                }

                எனில்( இலக்கம்_same_வேர் % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<இரட்டை>(3) = -(std::pow(old_இலக்கம்_re, 3));
                    cube_coefs.at<இரட்டை>(2) = -(15*std::pow(old_இலக்கம்_re, 2) + 27*std::pow(old_இலக்கம்_im, 2));
                    cube_coefs.at<இரட்டை>(1) = -48*old_இலக்கம்_re;
                    cube_coefs.at<இரட்டை>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<இரட்டை>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_இலக்கம்_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        எனில்( maxDiff <= 0 )
            நிறுத்து;
    }

    எனில்( coeffs0.channels() == 1 )
    {
        மாறிலி இரட்டை verySmallEps = 1e-100;
        க்கு( i = 0; i < n; i++ )
            எனில்( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    க்கு( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    திரும்பு maxDiff;
}


/* End of file. */
