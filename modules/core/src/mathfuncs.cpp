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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ <atomic>
#അന്തര്ഭാവ <limits>
#അന്തര്ഭാവ <iostream>
#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <cmath>
#അന്തര്ഭാവ "mathfuncs.hpp"
#അന്തര്ഭാവ "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

നാമാകാശ cv
{

തരനിർവചനം ശൂന്യം (*MathFunc)(സ്ഥിരം ശൂന്യം* src, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len);

#ifdef HAVE_OPENCL

എണ്ണൽ { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

സ്ഥിതം സ്ഥിരം അക്ഷരം* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

സ്ഥിതം തർക്കം ocl_math_op(നിവേശം_Array _src1, നിവേശം_Array _src2, OutputArray _dst, പൂർണ്ണസംഖ്യ oclop)
{
    പൂർണ്ണസംഖ്യ type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    പൂർണ്ണസംഖ്യ kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    സ്ഥിരം ocl::Device d = ocl::Device::getDefault();
    തർക്കം double_support = d.doubleFPConfig() > 0;
    എങ്കിൽ (!double_support && depth == CV_64F)
        തിരികെ അസത്യം;
    പൂർണ്ണസംഖ്യ rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.createSameSize(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    എങ്കിൽ (src2.empty())
        k.args(src1arg, dstarg);
    അല്ലെങ്കിൽ
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, 0, അസത്യം);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN ദശാംശം cube_വേര്( ദശാംശം value )
{
    CV_INSTRUMENT_REGION();

    ദശാംശം fr;
    Cv32suf v, m;
    പൂർണ്ണസംഖ്യ ix, s;
    പൂർണ്ണസംഖ്യ ex, shx;

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
    fr = (ദശാംശം)(((((45.2548339756803022511987494 * fr +
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
    തിരികെ v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

ശൂന്യം magnitude( നിവേശം_Array src1, നിവേശം_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    സ്ഥിരം Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ len = (പൂർണ്ണസംഖ്യ)it.size*cn;

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        എങ്കിൽ( depth == CV_32F )
        {
            സ്ഥിരം ദശാംശം *x = (സ്ഥിരം ദശാംശം*)ptrs[0], *y = (സ്ഥിരം ദശാംശം*)ptrs[1];
            ദശാംശം *mag = (ദശാംശം*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        അല്ലെങ്കിൽ
        {
            സ്ഥിരം ഇരട്ട *x = (സ്ഥിരം ഇരട്ട*)ptrs[0], *y = (സ്ഥിരം ഇരട്ട*)ptrs[1];
            ഇരട്ട *mag = (ഇരട്ട*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

ശൂന്യം phase( നിവേശം_Array src1, നിവേശം_Array src2, OutputArray dst, തർക്കം angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    സ്ഥിരം Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ j, total = (പൂർണ്ണസംഖ്യ)(it.size*cn), blockSize = total;
    size_t esz1 = X.elemSize1();
    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( j = 0; j < total; j += blockSize )
        {
            പൂർണ്ണസംഖ്യ len = std::min(total - j, blockSize);
            എങ്കിൽ( depth == CV_32F )
            {
                സ്ഥിരം ദശാംശം *x = (സ്ഥിരം ദശാംശം*)ptrs[0], *y = (സ്ഥിരം ദശാംശം*)ptrs[1];
                ദശാംശം *angle = (ദശാംശം*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            അല്ലെങ്കിൽ
            {
                സ്ഥിരം ഇരട്ട *x = (സ്ഥിരം ഇരട്ട*)ptrs[0], *y = (സ്ഥിരം ഇരട്ട*)ptrs[1];
                ഇരട്ട *angle = (ഇരട്ട*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_cartToPolar( നിവേശം_Array _src1, നിവേശം_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, തർക്കം angleInDegrees )
{
    സ്ഥിരം ocl::Device & d = ocl::Device::getDefault();
    പൂർണ്ണസംഖ്യ type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    തർക്കം doubleSupport = d.doubleFPConfig() > 0;

    സ്ഥിരം തർക്കം _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    സ്ഥിരം തർക്കം _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    സ്ഥിരം തർക്കം _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    സ്ഥിരം തർക്കം _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    എങ്കിൽ ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        തിരികെ അസത്യം;

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
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

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
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

ശൂന്യം cartToPolar( നിവേശം_Array src1, നിവേശം_Array src2,
                  OutputArray dst1, OutputArray dst2, തർക്കം angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    പൂർണ്ണസംഖ്യ type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    സ്ഥിരം Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ j, total = (പൂർണ്ണസംഖ്യ)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( j = 0; j < total; j += blockSize )
        {
            പൂർണ്ണസംഖ്യ len = std::min(total - j, blockSize);
            എങ്കിൽ( depth == CV_32F )
            {
                സ്ഥിരം ദശാംശം *x = (സ്ഥിരം ദശാംശം*)ptrs[0], *y = (സ്ഥിരം ദശാംശം*)ptrs[1];
                ദശാംശം *mag = (ദശാംശം*)ptrs[2], *angle = (ദശാംശം*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            അല്ലെങ്കിൽ
            {
                സ്ഥിരം ഇരട്ട *x = (സ്ഥിരം ഇരട്ട*)ptrs[0], *y = (സ്ഥിരം ഇരട്ട*)ptrs[1];
                ഇരട്ട *mag = (ഇരട്ട*)ptrs[2], *angle = (ഇരട്ട*)ptrs[3];
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

സ്ഥിതം തർക്കം ocl_polarToCart( നിവേശം_Array _mag, നിവേശം_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, തർക്കം angleInDegrees )
{
    സ്ഥിരം ocl::Device & d = ocl::Device::getDefault();
    പൂർണ്ണസംഖ്യ type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    തർക്കം doubleSupport = d.doubleFPConfig() > 0;

    സ്ഥിരം തർക്കം _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    സ്ഥിരം തർക്കം _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    സ്ഥിരം തർക്കം _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    സ്ഥിരം തർക്കം _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    എങ്കിൽ ( !doubleSupport && depth == CV_64F )
        തിരികെ അസത്യം;

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
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

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
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

ശൂന്യം polarToCart( നിവേശം_Array src1, നിവേശം_Array src2,
                  OutputArray dst1, OutputArray dst2, തർക്കം angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    പൂർണ്ണസംഖ്യ type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    സ്ഥിരം Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ j, total = (പൂർണ്ണസംഖ്യ)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( j = 0; j < total; j += blockSize )
        {
            പൂർണ്ണസംഖ്യ len = std::min(total - j, blockSize);
            എങ്കിൽ ( depth == CV_32F )
            {
                സ്ഥിരം ദശാംശം *mag = (സ്ഥിരം ദശാംശം*)ptrs[0], *angle = (സ്ഥിരം ദശാംശം*)ptrs[1];
                ദശാംശം *x = (ദശാംശം*)ptrs[2], *y = (ദശാംശം*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            അല്ലെങ്കിൽ
            {
                സ്ഥിരം ഇരട്ട *mag = (സ്ഥിരം ഇരട്ട*)ptrs[0], *angle = (സ്ഥിരം ഇരട്ട*)ptrs[1];
                ഇരട്ട *x = (ഇരട്ട*)ptrs[2], *y = (ഇരട്ട*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            എങ്കിൽ( ptrs[0] )
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
എണ്ണൽ { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

സ്ഥിതം ശൂന്യം math_op(ew::മുകൾ op, നിവേശം_Array _src, OutputArray _dst)
{
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    എങ്കിൽ (src.empty())
        തിരികെ;

    സ്ഥിരം size_t total = src.total() * src.channels();
    എങ്കിൽ (src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        സ്ഥിതം സ്ഥിരം ഇരട്ട noparams[4] = {};
        k.fptr(src.data, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.data, 0, (പൂർണ്ണസംഖ്യ)total, 1, noparams, k.flags, k.userdata);
        തിരികെ;
    }

    ew::TExpr p;
    സ്ഥിരം പൂർണ്ണസംഖ്യ a = p.add_നിവേശം(depth);
    സ്ഥിരം പൂർണ്ണസംഖ്യ out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    സ്ഥിരം Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

ശൂന്യം exp( നിവേശം_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] പൂർണ്ണസംഖ്യ depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

ശൂന്യം log( നിവേശം_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] പൂർണ്ണസംഖ്യ depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
ഘടന iPow_SIMD
{
    പൂർണ്ണസംഖ്യ operator() ( സ്ഥിരം T *, T *, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)
    {
        തിരികെ 0;
    }
};

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)

template <>
ഘടന iPow_SIMD<uchar, unsigned>
{
    പൂർണ്ണസംഖ്യ operator() ( സ്ഥിരം uchar * src, uchar * dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power )
    {
        പൂർണ്ണസംഖ്യ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        വേണ്ടി ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            പൂർണ്ണസംഖ്യ p = power;

            ആകുമ്പോൾ( p > 1 )
            {
                എങ്കിൽ (p & 1)
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

        തിരികെ i;
    }
};

template <>
ഘടന iPow_SIMD<schar, പൂർണ്ണസംഖ്യ>
{
    പൂർണ്ണസംഖ്യ operator() ( സ്ഥിരം schar * src, schar * dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power)
    {
        പൂർണ്ണസംഖ്യ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        വേണ്ടി ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            പൂർണ്ണസംഖ്യ p = power;

            ആകുമ്പോൾ( p > 1 )
            {
                എങ്കിൽ (p & 1)
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

        തിരികെ i;
    }
};

template <>
ഘടന iPow_SIMD<ushort, unsigned>
{
    പൂർണ്ണസംഖ്യ operator() ( സ്ഥിരം ushort * src, ushort * dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power)
    {
        പൂർണ്ണസംഖ്യ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        വേണ്ടി ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            പൂർണ്ണസംഖ്യ p = power;

            ആകുമ്പോൾ( p > 1 )
            {
                എങ്കിൽ (p & 1)
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

        തിരികെ i;
    }
};

template <>
ഘടന iPow_SIMD<short, പൂർണ്ണസംഖ്യ>
{
    പൂർണ്ണസംഖ്യ operator() ( സ്ഥിരം short * src, short * dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power)
    {
        പൂർണ്ണസംഖ്യ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        വേണ്ടി ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            പൂർണ്ണസംഖ്യ p = power;

            ആകുമ്പോൾ( p > 1 )
            {
                എങ്കിൽ (p & 1)
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

        തിരികെ i;
    }
};

template <>
ഘടന iPow_SIMD<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ>
{
    പൂർണ്ണസംഖ്യ operator() ( സ്ഥിരം പൂർണ്ണസംഖ്യ * src, പൂർണ്ണസംഖ്യ * dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power)
    {
        പൂർണ്ണസംഖ്യ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        വേണ്ടി ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            പൂർണ്ണസംഖ്യ p = power;

            ആകുമ്പോൾ( p > 1 )
            {
                എങ്കിൽ (p & 1)
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

        തിരികെ i;
    }
};

template <>
ഘടന iPow_SIMD<ദശാംശം, ദശാംശം>
{
    പൂർണ്ണസംഖ്യ operator() ( സ്ഥിരം ദശാംശം * src, ദശാംശം * dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power)
    {
        പൂർണ്ണസംഖ്യ i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        വേണ്ടി ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            പൂർണ്ണസംഖ്യ p = std::abs(power);
            എങ്കിൽ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ആകുമ്പോൾ( p > 1 )
            {
                എങ്കിൽ (p & 1)
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

        തിരികെ i;
    }
};

#എങ്കിൽ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
ഘടന iPow_SIMD<ഇരട്ട, ഇരട്ട>
{
    പൂർണ്ണസംഖ്യ operator() ( സ്ഥിരം ഇരട്ട * src, ഇരട്ട * dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power)
    {
        പൂർണ്ണസംഖ്യ i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        വേണ്ടി ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            പൂർണ്ണസംഖ്യ p = std::abs(power);
            എങ്കിൽ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ആകുമ്പോൾ( p > 1 )
            {
                എങ്കിൽ (p & 1)
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

        തിരികെ i;
    }
};
#endif

#endif

template<typename T, typename WT>
സ്ഥിതം ശൂന്യം
iPow_i( സ്ഥിരം ശൂന്യം* _src, ശൂന്യം* _dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power )
{
    സ്ഥിരം T* src = (സ്ഥിരം T*)_src;
    T* dst = (T*)_dst;
    എങ്കിൽ( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    അല്ലെങ്കിൽ
    {
        iPow_SIMD<T, WT> vop;
        പൂർണ്ണസംഖ്യ i = vop(src, dst, len, power);

        വേണ്ടി( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            പൂർണ്ണസംഖ്യ p = power;
            ആകുമ്പോൾ( p > 1 )
            {
                എങ്കിൽ( p & 1 )
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
സ്ഥിതം ശൂന്യം
iPow_f( സ്ഥിരം ശൂന്യം* _src, ശൂന്യം* _dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power0 )
{
    സ്ഥിരം T* src = (സ്ഥിരം T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    പൂർണ്ണസംഖ്യ i = vop(src, dst, len, power0);
    പൂർണ്ണസംഖ്യ power = std::abs(power0);

    വേണ്ടി( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        പൂർണ്ണസംഖ്യ p = power;
        എങ്കിൽ( power0 < 0 )
            b = 1/b;

        ആകുമ്പോൾ( p > 1 )
        {
            എങ്കിൽ( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

തരനിർവചനം ശൂന്യം (*IPowFunc)( സ്ഥിരം ശൂന്യം* src, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ power );

സ്ഥിതം IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, പൂർണ്ണസംഖ്യ>, iPow_i<ushort, unsigned>, iPow_i<short, പൂർണ്ണസംഖ്യ>,
    iPow_i<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ>, iPow_f<ദശാംശം>, iPow_f<ഇരട്ട>, 0
};

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_pow(നിവേശം_Array _src, ഇരട്ട power, OutputArray _dst,
                    തർക്കം is_ipower, പൂർണ്ണസംഖ്യ ipower)
{
    സ്ഥിരം ocl::Device & d = ocl::Device::getDefault();
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    തർക്കം doubleSupport = d.doubleFPConfig() > 0;

    _dst.createSameSize(_src, type);
    എങ്കിൽ (is_ipower)
    {
        എങ്കിൽ( ipower < 0 )
        {
            എങ്കിൽ( depth == CV_32F || depth == CV_64F )
                is_ipower = അസത്യം;
            അല്ലെങ്കിൽ
                തിരികെ അസത്യം;
        }
    }

    എങ്കിൽ (depth == CV_64F && !doubleSupport)
        തിരികെ അസത്യം;

    തർക്കം issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    സ്ഥിരം അക്ഷരം * സ്ഥിരം op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    എങ്കിൽ (is_ipower)
    {
        പൂർണ്ണസംഖ്യ wdepth = CV_32F;
        എങ്കിൽ (depth == CV_64F)
            wdepth = CV_64F;
        അല്ലെങ്കിൽ എങ്കിൽ (depth == CV_16F)
            wdepth = CV_16F;

        അക്ഷരം cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], വലുപ്പം(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], വലുപ്പം(cvt[1])),
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
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src = _src.getUMat();
    _dst.createSameSize(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    എങ്കിൽ (issqrt)
        k.args(srcarg, dstarg);
    അല്ലെങ്കിൽ എങ്കിൽ (is_ipower)
        k.args(srcarg, dstarg, ipower);
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (depth == CV_32F)
            k.args(srcarg, dstarg, (ദശാംശം)power);
        അല്ലെങ്കിൽ
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

ശൂന്യം pow( നിവേശം_Array _src, ഇരട്ട power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    തർക്കം is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    തർക്കം useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    എങ്കിൽ (is_ipower)
    {
        തിരഞ്ഞെടുപ്പ്( ipower )
        {
        അവസ്ഥ 0:
            _dst.createSameSize(_src, type);
            _dst.setTo(Scalar::all(1));
            തിരികെ;
        അവസ്ഥ 1:
            _src.copyTo(_dst);
            തിരികെ;
        അവസ്ഥ 2:
            multiply(_src, _src, _dst);
            തിരികെ;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    സ്ഥിരം തർക്കം floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    എങ്കിൽ( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.createSameSize(_src, type);
        Mat dst = _dst.getMat();

        സ്ഥിരം Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        പൂർണ്ണസംഖ്യ len = (പൂർണ്ണസംഖ്യ)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        തിരികെ;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);
    Mat dst = _dst.getMat();
    എങ്കിൽ (src.empty())
        തിരികെ;

    സ്ഥിരം size_t total = src.total() * cn;
    എങ്കിൽ (floatDepth && src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        എങ്കിൽ (k.fptr)
        {
            ഇരട്ട pvstore;                    // the broadcast exponent, stored as T
            ശൂന്യം* pv = &pvstore;
            തിരഞ്ഞെടുപ്പ് (depth)
            {
            അവസ്ഥ CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); നിർത്തുക;
            അവസ്ഥ CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); നിർത്തുക;
            അവസ്ഥ CV_32F:  *(ദശാംശം*)pv = (ദശാംശം)power; നിർത്തുക;
            സ്ഥിരസ്ഥിതി:      pvstore = power; നിർത്തുക;
            }
            സ്ഥിതം സ്ഥിരം ഇരട്ട noparams[4] = {};
            k.fptr(src.data, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.data, 0, (പൂർണ്ണസംഖ്യ)total, 1, noparams, k.flags, k.userdata);
            തിരികെ;
        }
    }

    ew::TExpr prog;
    സ്ഥിരം പൂർണ്ണസംഖ്യ a = prog.add_നിവേശം(depth);
    സ്ഥിരം പൂർണ്ണസംഖ്യ c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    സ്ഥിരം പൂർണ്ണസംഖ്യ out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_ദ്വിമാന(ew::OP_POW, a, c, depth), out);
    prog.compile();
    സ്ഥിരം Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

ശൂന്യം sqrt(നിവേശം_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        തിരികെ;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<പൂർണ്ണസംഖ്യ cv_mat_type> ഘടന mat_type_assotiations{};

template<> ഘടന mat_type_assotiations<CV_8U>
{
    തരനിർവചനം unsigned അക്ഷരം type;
    സ്ഥിതം സ്ഥിരം type min_allowable = 0x0;
    സ്ഥിതം സ്ഥിരം type max_allowable = 0xFF;
};

template<> ഘടന mat_type_assotiations<CV_8S>
{
    തരനിർവചനം signed അക്ഷരം type;
    സ്ഥിതം സ്ഥിരം type min_allowable = SCHAR_MIN;
    സ്ഥിതം സ്ഥിരം type max_allowable = SCHAR_MAX;
};

template<> ഘടന mat_type_assotiations<CV_16U>
{
    തരനിർവചനം unsigned short type;
    സ്ഥിതം സ്ഥിരം type min_allowable = 0x0;
    സ്ഥിതം സ്ഥിരം type max_allowable = USHRT_MAX;
};
template<> ഘടന mat_type_assotiations<CV_16S>
{
    തരനിർവചനം signed short type;
    സ്ഥിതം സ്ഥിരം type min_allowable = SHRT_MIN;
    സ്ഥിതം സ്ഥിരം type max_allowable = SHRT_MAX;
};

template<> ഘടന mat_type_assotiations<CV_32S>
{
    തരനിർവചനം പൂർണ്ണസംഖ്യ type;
    സ്ഥിതം സ്ഥിരം type min_allowable = (-INT_MAX - 1);
    സ്ഥിതം സ്ഥിരം type max_allowable = INT_MAX;
};

template<പൂർണ്ണസംഖ്യ depth>
സ്ഥിതം തർക്കം checkIntegerRange(cv::Mat src, Point& bad_pt, പൂർണ്ണസംഖ്യ minVal, പൂർണ്ണസംഖ്യ maxVal)
{
    തരനിർവചനം mat_type_assotiations<depth> type_ass;

    എങ്കിൽ (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        തിരികെ സത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        തിരികെ അസത്യം;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < as_one_channel.rows; ++j)
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            എങ്കിൽ (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                തിരികെ അസത്യം;
            }
        }

    തിരികെ സത്യം;
}

തരനിർവചനം തർക്കം (*check_range_function)(cv::Mat src, Point& bad_pt, പൂർണ്ണസംഖ്യ minVal, പൂർണ്ണസംഖ്യ maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
സ്ഥിതം ശൂന്യം checkTypedRange(Mat& src, Point& badPt, ഇരട്ട minVal, ഇരട്ട maxVal)
{
    പൂർണ്ണസംഖ്യ loc = 0;
    പൂർണ്ണസംഖ്യ cn = src.channels();
    Size size = getContinuousSize2D(src, cn);
    സ്ഥിരം T* isrc = src.ptr<T>();
    size_t step = src.step / വലുപ്പം(isrc[0]);

    വേണ്ടി( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < size.width; i++ )
        {
            ഇരട്ട val = (ഇരട്ട)isrc[i];
            എങ്കിൽ( !(val >= minVal && val < maxVal) )
            {
                പൂർണ്ണസംഖ്യ pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                നിർത്തുക;
            }
        }
    }
}

തർക്കം checkRange(നിവേശം_Array _src, തർക്കം quiet, Point* pt, ഇരട്ട minVal, ഇരട്ട maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    എങ്കിൽ ( src.dims > 2 )
    {
        CV_Assert(pt == ശൂന്യ); // no way to provide location info

        സ്ഥിരം Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        വേണ്ടി ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            എങ്കിൽ (!checkRange( it.planes[0], quiet, ശൂന്യ, minVal, maxVal ))
            {
                തിരികെ അസത്യം;
            }
        }
        തിരികെ സത്യം;
    }

    പൂർണ്ണസംഖ്യ depth = src.depth();
    Point badPt(-1, -1);

    എങ്കിൽ (depth < CV_32F)
    {
        പൂർണ്ണസംഖ്യ minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        പൂർണ്ണസംഖ്യ maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ i, loc = 0;
        പൂർണ്ണസംഖ്യ cn = src.channels();
        Size size = getContinuousSize2D(src, cn);

        എങ്കിൽ( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_32F )
        {
            Cv32suf a, b;
            പൂർണ്ണസംഖ്യ ia, ib;
            സ്ഥിരം പൂർണ്ണസംഖ്യ* isrc = src.ptr<പൂർണ്ണസംഖ്യ>();
            size_t step = src.step/വലുപ്പം(isrc[0]);

            a.f = (ദശാംശം)std::max(minVal, (ഇരട്ട)-FLT_MAX);
            b.f = (ദശാംശം)std::min(maxVal, (ഇരട്ട)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            വേണ്ടി( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                വേണ്ടി( i = 0; i < size.width; i++ )
                {
                    പൂർണ്ണസംഖ്യ val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    എങ്കിൽ( val < ia || val >= ib )
                    {
                        പൂർണ്ണസംഖ്യ pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        നിർത്തുക;
                    }
                }
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            സ്ഥിരം int64* isrc = src.ptr<int64>();
            size_t step = src.step/വലുപ്പം(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            വേണ്ടി( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                വേണ്ടി( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    എങ്കിൽ( val < ia || val >= ib )
                    {
                        പൂർണ്ണസംഖ്യ pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        നിർത്തുക;
                    }
                }
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        അല്ലെങ്കിൽ
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    എങ്കിൽ( badPt.x >= 0 )
    {
        എങ്കിൽ( pt )
            *pt = badPt;
        എങ്കിൽ( !quiet )
        {
            cv::String മൂല്യം_str;
            മൂല്യം_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, മൂല്യം_str.c_str(), minVal, maxVal));
        }
        തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
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

പൂർണ്ണസംഖ്യ cv::solveCubic( നിവേശം_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    സ്ഥിരം പൂർണ്ണസംഖ്യ n0 = 3;
    Mat coeffs = _coeffs.getMat();
    പൂർണ്ണസംഖ്യ ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == Size(n0, 1) ||
                coeffs.size() == Size(n0+1, 1) ||
                coeffs.size() == Size(1, n0) ||
                coeffs.size() == Size(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, സത്യം, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    പൂർണ്ണസംഖ്യ i = -1, n = 0;
    ഇരട്ട a0 = 1., a1, a2, a3;
    ഇരട്ട x0 = 0., x1 = 0., x2 = 0.;
    പൂർണ്ണസംഖ്യ ncoeffs = coeffs.rows + coeffs.cols - 1;

    എങ്കിൽ( ctype == CV_32FC1 )
    {
        എങ്കിൽ( ncoeffs == 4 )
            a0 = coeffs.at<ദശാംശം>(++i);

        a1 = coeffs.at<ദശാംശം>(i+1);
        a2 = coeffs.at<ദശാംശം>(i+2);
        a3 = coeffs.at<ദശാംശം>(i+3);
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ( ncoeffs == 4 )
            a0 = coeffs.at<ഇരട്ട>(++i);

        a1 = coeffs.at<ഇരട്ട>(i+1);
        a2 = coeffs.at<ഇരട്ട>(i+2);
        a3 = coeffs.at<ഇരട്ട>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    ഇരട്ട max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    എങ്കിൽ (max_coeff < std::numeric_limits<ഇരട്ട>::epsilon())
        തിരികെ -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    എങ്കിൽ( std::abs(a0) < std::numeric_limits<ഇരട്ട>::epsilon() * max_coeff )
    {
        എങ്കിൽ( std::abs(a1) < std::numeric_limits<ഇരട്ട>::epsilon() * max_coeff )
        {
            എങ്കിൽ( std::abs(a2) < std::numeric_limits<ഇരട്ട>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<ഇരട്ട>::epsilon() * max_coeff ? -1 : 0;
            അല്ലെങ്കിൽ
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        അല്ലെങ്കിൽ
        {
            // quadratic equation
            ഇരട്ട d = a2*a2 - 4*a1*a3;
            എങ്കിൽ( d >= 0 )
            {
                d = std::sqrt(d);
                ഇരട്ട q1 = (-a2 + d) * 0.5;
                ഇരട്ട q2 = (a2 + d) * -0.5;
                എങ്കിൽ( std::abs(q1) > std::abs(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                അല്ലെങ്കിൽ
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        ഇരട്ട scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        ഇരട്ട Q = (a1 * a1 - 3 * a2) * (1./9);
        ഇരട്ട R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        ഇരട്ട Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        ഇരട്ട d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        എങ്കിൽ( d > 0 )
        {
            ഇരട്ട theta = acos(R / sqrt(Qcubed));
            ഇരട്ട sqrtQ = sqrt(Q);
            ഇരട്ട t0 = -2 * sqrtQ;
            ഇരട്ട t1 = theta * (1./3);
            ഇരട്ട t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        അല്ലെങ്കിൽ
        {
            ഇരട്ട e;
            d = sqrt(-d);
            e = std::cbrt(d + fabs(R));
            എങ്കിൽ( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    എങ്കിൽ( roots.type() == CV_32FC1 )
    {
        roots.at<ദശാംശം>(0) = (ദശാംശം)x0;
        roots.at<ദശാംശം>(1) = (ദശാംശം)x1;
        roots.at<ദശാംശം>(2) = (ദശാംശം)x2;
    }
    അല്ലെങ്കിൽ
    {
        roots.at<ഇരട്ട>(0) = x0;
        roots.at<ഇരട്ട>(1) = x1;
        roots.at<ഇരട്ട>(2) = x2;
    }

    തിരികെ n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
ഇരട്ട cv::solvePoly( നിവേശം_Array _coeffs0, OutputArray _roots0, പൂർണ്ണസംഖ്യ maxIters )
{
    CV_INSTRUMENT_REGION();

    തരനിർവചനം Complex<ഇരട്ട> C;

    ഇരട്ട maxDiff = 0;
    പൂർണ്ണസംഖ്യ iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    പൂർണ്ണസംഖ്യ ctype = _coeffs0.type();
    പൂർണ്ണസംഖ്യ cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.total() >= 2 );

    പൂർണ്ണസംഖ്യ n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, സത്യം, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    AutoBuffer<C> buf(n*2+2);
    C *coeffs = buf.data(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    എങ്കിൽ( coeffs0.channels() == 1 )
    {
        സ്ഥിരം ഇരട്ട* rcoeffs = (സ്ഥിരം ഇരട്ട*)roots;
        വേണ്ടി( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    വേണ്ടി( ; n > 1; n-- )
    {
        എങ്കിൽ( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            നിർത്തുക;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    വേണ്ടി( i = 0; i <= n; i++ )
        absCoeffs.at<ഇരട്ട>(i) = abs(coeffs[i]);

    പൂർണ്ണസംഖ്യ nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    വേണ്ടി( i = 0; i <= n; i++ )
    {
        ഇരട്ട coeff = absCoeffs.at<ഇരട്ട>(i);
        എങ്കിൽ( coeff > DBL_EPSILON )
        {
            എങ്കിൽ( i != n )
                u.at<ഇരട്ട>(i) = 2.0 * pow(coeff / absCoeffs.at<ഇരട്ട>(n), 1.0 / (n - i));
            എങ്കിൽ( i != 0 )
                v.at<ഇരട്ട>(i - 1) = 0.5 * pow(absCoeffs.at<ഇരട്ട>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    ഇരട്ട scale = 1;
    എങ്കിൽ( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<ഇരട്ട>(maxU) = 0;
        v.at<ഇരട്ട>(minV) = 0;
        scale = (sum(u).val[0] + sum(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    വേണ്ടി( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    വേണ്ടി( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        വേണ്ടി( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            പൂർണ്ണസംഖ്യ അക്കം_same_വേര് = 1;
            വേണ്ടി( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                എങ്കിൽ( j != i )
                {
                    എങ്കിൽ ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    അല്ലെങ്കിൽ
                        അക്കം_same_വേര്++;
                }
            }
            num /= denom;
            എങ്കിൽ( അക്കം_same_വേര് > 1)
            {
                ഇരട്ട old_അക്കം_re = num.re;
                ഇരട്ട old_അക്കം_im = num.im;
                പൂർണ്ണസംഖ്യ square_വേര്_times = അക്കം_same_വേര് % 2 == 0 ? അക്കം_same_വേര് / 2 : അക്കം_same_വേര് / 2 - 1;

                വേണ്ടി( j = 0; j < square_വേര്_times; j++)
                {
                    num.re = old_അക്കം_re*old_അക്കം_re + old_അക്കം_im*old_അക്കം_im;
                    num.re = sqrt(num.re);
                    num.re += old_അക്കം_re;
                    num.im = num.re - old_അക്കം_re;
                    num.re /= 2;
                    num.re = sqrt(num.re);

                    num.im /= 2;
                    num.im = sqrt(num.im);
                    എങ്കിൽ( old_അക്കം_re < 0 ) num.im = -num.im;
                }

                എങ്കിൽ( അക്കം_same_വേര് % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<ഇരട്ട>(3) = -(std::pow(old_അക്കം_re, 3));
                    cube_coefs.at<ഇരട്ട>(2) = -(15*std::pow(old_അക്കം_re, 2) + 27*std::pow(old_അക്കം_im, 2));
                    cube_coefs.at<ഇരട്ട>(1) = -48*old_അക്കം_re;
                    cube_coefs.at<ഇരട്ട>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<ഇരട്ട>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_അക്കം_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        എങ്കിൽ( maxDiff <= 0 )
            നിർത്തുക;
    }

    എങ്കിൽ( coeffs0.channels() == 1 )
    {
        സ്ഥിരം ഇരട്ട verySmallEps = 1e-100;
        വേണ്ടി( i = 0; i < n; i++ )
            എങ്കിൽ( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    വേണ്ടി( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    തിരികെ maxDiff;
}


/* End of file. */
