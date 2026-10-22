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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव <atomic>
#अन्तर्भाव <limits>
#अन्तर्भाव <iostream>
#अन्तर्भाव <algorithm>
#अन्तर्भाव <cmath>
#अन्तर्भाव "mathfuncs.hpp"
#अन्तर्भाव "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

नामाकाश cv
{

प्रकारनिर्देश शून्य (*MathFunc)(ध्रुव शून्य* src, शून्य* dst, पूर्णाङ्क len);

#ifdef HAVE_OPENCL

गणना { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

स्थिर ध्रुव वर्ण* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

स्थिर तार्किक ocl_math_op(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst, पूर्णाङ्क oclop)
{
    पूर्णाङ्क type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    पूर्णाङ्क kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    ध्रुव ocl::Device d = ocl::Device::getDefault();
    तार्किक double_support = d.doubleFPConfig() > 0;
    यदि (!double_support && depth == CV_64F)
        प्रत्यागम असत्य;
    पूर्णाङ्क rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.createSameSize(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    यदि (src2.empty())
        k.args(src1arg, dstarg);
    अन्यथा
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, 0, असत्य);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN भिन्न cube_मूल( भिन्न value )
{
    CV_INSTRUMENT_REGION();

    भिन्न fr;
    Cv32suf v, m;
    पूर्णाङ्क ix, s;
    पूर्णाङ्क ex, shx;

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
    fr = (भिन्न)(((((45.2548339756803022511987494 * fr +
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
    प्रत्यागम v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

शून्य magnitude( निवेश_Array src1, निवेश_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ध्रुव Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क len = (पूर्णाङ्क)it.size*cn;

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        यदि( depth == CV_32F )
        {
            ध्रुव भिन्न *x = (ध्रुव भिन्न*)ptrs[0], *y = (ध्रुव भिन्न*)ptrs[1];
            भिन्न *mag = (भिन्न*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        अन्यथा
        {
            ध्रुव द्विगुण *x = (ध्रुव द्विगुण*)ptrs[0], *y = (ध्रुव द्विगुण*)ptrs[1];
            द्विगुण *mag = (द्विगुण*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

शून्य phase( निवेश_Array src1, निवेश_Array src2, OutputArray dst, तार्किक angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ध्रुव Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क j, total = (पूर्णाङ्क)(it.size*cn), blockSize = total;
    size_t esz1 = X.elemSize1();
    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( j = 0; j < total; j += blockSize )
        {
            पूर्णाङ्क len = std::min(total - j, blockSize);
            यदि( depth == CV_32F )
            {
                ध्रुव भिन्न *x = (ध्रुव भिन्न*)ptrs[0], *y = (ध्रुव भिन्न*)ptrs[1];
                भिन्न *angle = (भिन्न*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            अन्यथा
            {
                ध्रुव द्विगुण *x = (ध्रुव द्विगुण*)ptrs[0], *y = (ध्रुव द्विगुण*)ptrs[1];
                द्विगुण *angle = (द्विगुण*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_cartToPolar( निवेश_Array _src1, निवेश_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, तार्किक angleInDegrees )
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();
    पूर्णाङ्क type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    तार्किक doubleSupport = d.doubleFPConfig() > 0;

    ध्रुव तार्किक _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ध्रुव तार्किक _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ध्रुव तार्किक _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ध्रुव तार्किक _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    यदि ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        प्रत्यागम असत्य;

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
    यदि (k.empty())
        प्रत्यागम असत्य;

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
    प्रत्यागम k.run(2, globalsize, रिक्त, असत्य);
}

#endif

शून्य cartToPolar( निवेश_Array src1, निवेश_Array src2,
                  OutputArray dst1, OutputArray dst2, तार्किक angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    पूर्णाङ्क type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ध्रुव Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क j, total = (पूर्णाङ्क)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( j = 0; j < total; j += blockSize )
        {
            पूर्णाङ्क len = std::min(total - j, blockSize);
            यदि( depth == CV_32F )
            {
                ध्रुव भिन्न *x = (ध्रुव भिन्न*)ptrs[0], *y = (ध्रुव भिन्न*)ptrs[1];
                भिन्न *mag = (भिन्न*)ptrs[2], *angle = (भिन्न*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            अन्यथा
            {
                ध्रुव द्विगुण *x = (ध्रुव द्विगुण*)ptrs[0], *y = (ध्रुव द्विगुण*)ptrs[1];
                द्विगुण *mag = (द्विगुण*)ptrs[2], *angle = (द्विगुण*)ptrs[3];
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

स्थिर तार्किक ocl_polarToCart( निवेश_Array _mag, निवेश_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, तार्किक angleInDegrees )
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();
    पूर्णाङ्क type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    तार्किक doubleSupport = d.doubleFPConfig() > 0;

    ध्रुव तार्किक _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ध्रुव तार्किक _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ध्रुव तार्किक _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ध्रुव तार्किक _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    यदि ( !doubleSupport && depth == CV_64F )
        प्रत्यागम असत्य;

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
    यदि (k.empty())
        प्रत्यागम असत्य;

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
    प्रत्यागम k.run(2, globalsize, रिक्त, असत्य);
}

#endif

शून्य polarToCart( निवेश_Array src1, निवेश_Array src2,
                  OutputArray dst1, OutputArray dst2, तार्किक angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    पूर्णाङ्क type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ध्रुव Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क j, total = (पूर्णाङ्क)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( j = 0; j < total; j += blockSize )
        {
            पूर्णाङ्क len = std::min(total - j, blockSize);
            यदि ( depth == CV_32F )
            {
                ध्रुव भिन्न *mag = (ध्रुव भिन्न*)ptrs[0], *angle = (ध्रुव भिन्न*)ptrs[1];
                भिन्न *x = (भिन्न*)ptrs[2], *y = (भिन्न*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            अन्यथा
            {
                ध्रुव द्विगुण *mag = (ध्रुव द्विगुण*)ptrs[0], *angle = (ध्रुव द्विगुण*)ptrs[1];
                द्विगुण *x = (द्विगुण*)ptrs[2], *y = (द्विगुण*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            यदि( ptrs[0] )
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
गणना { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

स्थिर शून्य math_op(ew::शीर्ष op, निवेश_Array _src, OutputArray _dst)
{
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    यदि (src.empty())
        प्रत्यागम;

    ध्रुव size_t total = src.total() * src.channels();
    यदि (src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        स्थिर ध्रुव द्विगुण noparams[4] = {};
        k.fptr(src.data, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.data, 0, (पूर्णाङ्क)total, 1, noparams, k.flags, k.userdata);
        प्रत्यागम;
    }

    ew::TExpr p;
    ध्रुव पूर्णाङ्क a = p.add_निवेश(depth);
    ध्रुव पूर्णाङ्क out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    ध्रुव Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

शून्य exp( निवेश_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] पूर्णाङ्क depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

शून्य log( निवेश_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] पूर्णाङ्क depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
संरचना iPow_SIMD
{
    पूर्णाङ्क operator() ( ध्रुव T *, T *, पूर्णाङ्क, पूर्णाङ्क)
    {
        प्रत्यागम 0;
    }
};

#यदि (CV_SIMD || CV_SIMD_SCALABLE)

template <>
संरचना iPow_SIMD<uchar, unsigned>
{
    पूर्णाङ्क operator() ( ध्रुव uchar * src, uchar * dst, पूर्णाङ्क len, पूर्णाङ्क power )
    {
        पूर्णाङ्क i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        प्रति ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            पूर्णाङ्क p = power;

            यावत्( p > 1 )
            {
                यदि (p & 1)
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

        प्रत्यागम i;
    }
};

template <>
संरचना iPow_SIMD<schar, पूर्णाङ्क>
{
    पूर्णाङ्क operator() ( ध्रुव schar * src, schar * dst, पूर्णाङ्क len, पूर्णाङ्क power)
    {
        पूर्णाङ्क i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        प्रति ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            पूर्णाङ्क p = power;

            यावत्( p > 1 )
            {
                यदि (p & 1)
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

        प्रत्यागम i;
    }
};

template <>
संरचना iPow_SIMD<ushort, unsigned>
{
    पूर्णाङ्क operator() ( ध्रुव ushort * src, ushort * dst, पूर्णाङ्क len, पूर्णाङ्क power)
    {
        पूर्णाङ्क i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        प्रति ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            पूर्णाङ्क p = power;

            यावत्( p > 1 )
            {
                यदि (p & 1)
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

        प्रत्यागम i;
    }
};

template <>
संरचना iPow_SIMD<short, पूर्णाङ्क>
{
    पूर्णाङ्क operator() ( ध्रुव short * src, short * dst, पूर्णाङ्क len, पूर्णाङ्क power)
    {
        पूर्णाङ्क i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        प्रति ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            पूर्णाङ्क p = power;

            यावत्( p > 1 )
            {
                यदि (p & 1)
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

        प्रत्यागम i;
    }
};

template <>
संरचना iPow_SIMD<पूर्णाङ्क, पूर्णाङ्क>
{
    पूर्णाङ्क operator() ( ध्रुव पूर्णाङ्क * src, पूर्णाङ्क * dst, पूर्णाङ्क len, पूर्णाङ्क power)
    {
        पूर्णाङ्क i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        प्रति ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            पूर्णाङ्क p = power;

            यावत्( p > 1 )
            {
                यदि (p & 1)
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

        प्रत्यागम i;
    }
};

template <>
संरचना iPow_SIMD<भिन्न, भिन्न>
{
    पूर्णाङ्क operator() ( ध्रुव भिन्न * src, भिन्न * dst, पूर्णाङ्क len, पूर्णाङ्क power)
    {
        पूर्णाङ्क i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        प्रति ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            पूर्णाङ्क p = std::abs(power);
            यदि( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            यावत्( p > 1 )
            {
                यदि (p & 1)
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

        प्रत्यागम i;
    }
};

#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
संरचना iPow_SIMD<द्विगुण, द्विगुण>
{
    पूर्णाङ्क operator() ( ध्रुव द्विगुण * src, द्विगुण * dst, पूर्णाङ्क len, पूर्णाङ्क power)
    {
        पूर्णाङ्क i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        प्रति ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            पूर्णाङ्क p = std::abs(power);
            यदि( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            यावत्( p > 1 )
            {
                यदि (p & 1)
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

        प्रत्यागम i;
    }
};
#endif

#endif

template<typename T, typename WT>
स्थिर शून्य
iPow_i( ध्रुव शून्य* _src, शून्य* _dst, पूर्णाङ्क len, पूर्णाङ्क power )
{
    ध्रुव T* src = (ध्रुव T*)_src;
    T* dst = (T*)_dst;
    यदि( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        प्रति( पूर्णाङ्क i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    अन्यथा
    {
        iPow_SIMD<T, WT> vop;
        पूर्णाङ्क i = vop(src, dst, len, power);

        प्रति( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            पूर्णाङ्क p = power;
            यावत्( p > 1 )
            {
                यदि( p & 1 )
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
स्थिर शून्य
iPow_f( ध्रुव शून्य* _src, शून्य* _dst, पूर्णाङ्क len, पूर्णाङ्क power0 )
{
    ध्रुव T* src = (ध्रुव T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    पूर्णाङ्क i = vop(src, dst, len, power0);
    पूर्णाङ्क power = std::abs(power0);

    प्रति( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        पूर्णाङ्क p = power;
        यदि( power0 < 0 )
            b = 1/b;

        यावत्( p > 1 )
        {
            यदि( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

प्रकारनिर्देश शून्य (*IPowFunc)( ध्रुव शून्य* src, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क power );

स्थिर IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, पूर्णाङ्क>, iPow_i<ushort, unsigned>, iPow_i<short, पूर्णाङ्क>,
    iPow_i<पूर्णाङ्क, पूर्णाङ्क>, iPow_f<भिन्न>, iPow_f<द्विगुण>, 0
};

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_pow(निवेश_Array _src, द्विगुण power, OutputArray _dst,
                    तार्किक is_ipower, पूर्णाङ्क ipower)
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    तार्किक doubleSupport = d.doubleFPConfig() > 0;

    _dst.createSameSize(_src, type);
    यदि (is_ipower)
    {
        यदि( ipower < 0 )
        {
            यदि( depth == CV_32F || depth == CV_64F )
                is_ipower = असत्य;
            अन्यथा
                प्रत्यागम असत्य;
        }
    }

    यदि (depth == CV_64F && !doubleSupport)
        प्रत्यागम असत्य;

    तार्किक issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    ध्रुव वर्ण * ध्रुव op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    यदि (is_ipower)
    {
        पूर्णाङ्क wdepth = CV_32F;
        यदि (depth == CV_64F)
            wdepth = CV_64F;
        अन्यथा यदि (depth == CV_16F)
            wdepth = CV_16F;

        वर्ण cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], परिमाण(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], परिमाण(cvt[1])),
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
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src = _src.getUMat();
    _dst.createSameSize(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    यदि (issqrt)
        k.args(srcarg, dstarg);
    अन्यथा यदि (is_ipower)
        k.args(srcarg, dstarg, ipower);
    अन्यथा
    {
        यदि (depth == CV_32F)
            k.args(srcarg, dstarg, (भिन्न)power);
        अन्यथा
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, रिक्त, असत्य);
}

#endif

शून्य pow( निवेश_Array _src, द्विगुण power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    तार्किक is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    तार्किक useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    यदि (is_ipower)
    {
        विकल्प( ipower )
        {
        पक्ष 0:
            _dst.createSameSize(_src, type);
            _dst.setTo(Scalar::all(1));
            प्रत्यागम;
        पक्ष 1:
            _src.copyTo(_dst);
            प्रत्यागम;
        पक्ष 2:
            multiply(_src, _src, _dst);
            प्रत्यागम;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ध्रुव तार्किक floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    यदि( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.createSameSize(_src, type);
        Mat dst = _dst.getMat();

        ध्रुव Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णाङ्क len = (पूर्णाङ्क)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        प्रत्यागम;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);
    Mat dst = _dst.getMat();
    यदि (src.empty())
        प्रत्यागम;

    ध्रुव size_t total = src.total() * cn;
    यदि (floatDepth && src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        यदि (k.fptr)
        {
            द्विगुण pvstore;                    // the broadcast exponent, stored as T
            शून्य* pv = &pvstore;
            विकल्प (depth)
            {
            पक्ष CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); भङ्ग;
            पक्ष CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); भङ्ग;
            पक्ष CV_32F:  *(भिन्न*)pv = (भिन्न)power; भङ्ग;
            स्वभाव:      pvstore = power; भङ्ग;
            }
            स्थिर ध्रुव द्विगुण noparams[4] = {};
            k.fptr(src.data, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.data, 0, (पूर्णाङ्क)total, 1, noparams, k.flags, k.userdata);
            प्रत्यागम;
        }
    }

    ew::TExpr prog;
    ध्रुव पूर्णाङ्क a = prog.add_निवेश(depth);
    ध्रुव पूर्णाङ्क c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    ध्रुव पूर्णाङ्क out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_द्विआधारी(ew::OP_POW, a, c, depth), out);
    prog.compile();
    ध्रुव Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

शून्य sqrt(निवेश_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    यदि (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        प्रत्यागम;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<पूर्णाङ्क cv_mat_type> संरचना mat_type_assotiations{};

template<> संरचना mat_type_assotiations<CV_8U>
{
    प्रकारनिर्देश unsigned वर्ण type;
    स्थिर ध्रुव type min_allowable = 0x0;
    स्थिर ध्रुव type max_allowable = 0xFF;
};

template<> संरचना mat_type_assotiations<CV_8S>
{
    प्रकारनिर्देश signed वर्ण type;
    स्थिर ध्रुव type min_allowable = SCHAR_MIN;
    स्थिर ध्रुव type max_allowable = SCHAR_MAX;
};

template<> संरचना mat_type_assotiations<CV_16U>
{
    प्रकारनिर्देश unsigned short type;
    स्थिर ध्रुव type min_allowable = 0x0;
    स्थिर ध्रुव type max_allowable = USHRT_MAX;
};
template<> संरचना mat_type_assotiations<CV_16S>
{
    प्रकारनिर्देश signed short type;
    स्थिर ध्रुव type min_allowable = SHRT_MIN;
    स्थिर ध्रुव type max_allowable = SHRT_MAX;
};

template<> संरचना mat_type_assotiations<CV_32S>
{
    प्रकारनिर्देश पूर्णाङ्क type;
    स्थिर ध्रुव type min_allowable = (-INT_MAX - 1);
    स्थिर ध्रुव type max_allowable = INT_MAX;
};

template<पूर्णाङ्क depth>
स्थिर तार्किक checkIntegerRange(cv::Mat src, Point& bad_pt, पूर्णाङ्क minVal, पूर्णाङ्क maxVal)
{
    प्रकारनिर्देश mat_type_assotiations<depth> type_ass;

    यदि (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        प्रत्यागम सत्य;
    }
    अन्यथा यदि (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        प्रत्यागम असत्य;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    प्रति (पूर्णाङ्क j = 0; j < as_one_channel.rows; ++j)
        प्रति (पूर्णाङ्क i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            यदि (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                प्रत्यागम असत्य;
            }
        }

    प्रत्यागम सत्य;
}

प्रकारनिर्देश तार्किक (*check_range_function)(cv::Mat src, Point& bad_pt, पूर्णाङ्क minVal, पूर्णाङ्क maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
स्थिर शून्य checkTypedRange(Mat& src, Point& badPt, द्विगुण minVal, द्विगुण maxVal)
{
    पूर्णाङ्क loc = 0;
    पूर्णाङ्क cn = src.channels();
    Size size = getContinuousSize2D(src, cn);
    ध्रुव T* isrc = src.ptr<T>();
    size_t step = src.step / परिमाण(isrc[0]);

    प्रति( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        प्रति( पूर्णाङ्क i = 0; i < size.width; i++ )
        {
            द्विगुण val = (द्विगुण)isrc[i];
            यदि( !(val >= minVal && val < maxVal) )
            {
                पूर्णाङ्क pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                भङ्ग;
            }
        }
    }
}

तार्किक checkRange(निवेश_Array _src, तार्किक quiet, Point* pt, द्विगुण minVal, द्विगुण maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    यदि ( src.dims > 2 )
    {
        CV_Assert(pt == रिक्त); // no way to provide location info

        ध्रुव Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        प्रति ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            यदि (!checkRange( it.planes[0], quiet, रिक्त, minVal, maxVal ))
            {
                प्रत्यागम असत्य;
            }
        }
        प्रत्यागम सत्य;
    }

    पूर्णाङ्क depth = src.depth();
    Point badPt(-1, -1);

    यदि (depth < CV_32F)
    {
        पूर्णाङ्क minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        पूर्णाङ्क maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    अन्यथा
    {
        पूर्णाङ्क i, loc = 0;
        पूर्णाङ्क cn = src.channels();
        Size size = getContinuousSize2D(src, cn);

        यदि( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        अन्यथा यदि( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        अन्यथा यदि( depth == CV_32F )
        {
            Cv32suf a, b;
            पूर्णाङ्क ia, ib;
            ध्रुव पूर्णाङ्क* isrc = src.ptr<पूर्णाङ्क>();
            size_t step = src.step/परिमाण(isrc[0]);

            a.f = (भिन्न)std::max(minVal, (द्विगुण)-FLT_MAX);
            b.f = (भिन्न)std::min(maxVal, (द्विगुण)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            प्रति( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                प्रति( i = 0; i < size.width; i++ )
                {
                    पूर्णाङ्क val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    यदि( val < ia || val >= ib )
                    {
                        पूर्णाङ्क pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        भङ्ग;
                    }
                }
            }
        }
        अन्यथा यदि( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ध्रुव int64* isrc = src.ptr<int64>();
            size_t step = src.step/परिमाण(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            प्रति( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                प्रति( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    यदि( val < ia || val >= ib )
                    {
                        पूर्णाङ्क pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        भङ्ग;
                    }
                }
            }
        }
        अन्यथा यदि( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        अन्यथा यदि( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        अन्यथा यदि( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        अन्यथा यदि( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        अन्यथा
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    यदि( badPt.x >= 0 )
    {
        यदि( pt )
            *pt = badPt;
        यदि( !quiet )
        {
            cv::String मूल्य_str;
            मूल्य_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, मूल्य_str.c_str(), minVal, maxVal));
        }
        प्रत्यागम असत्य;
    }
    प्रत्यागम सत्य;
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

पूर्णाङ्क cv::solveCubic( निवेश_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ध्रुव पूर्णाङ्क n0 = 3;
    Mat coeffs = _coeffs.getMat();
    पूर्णाङ्क ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == Size(n0, 1) ||
                coeffs.size() == Size(n0+1, 1) ||
                coeffs.size() == Size(1, n0) ||
                coeffs.size() == Size(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, सत्य, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    पूर्णाङ्क i = -1, n = 0;
    द्विगुण a0 = 1., a1, a2, a3;
    द्विगुण x0 = 0., x1 = 0., x2 = 0.;
    पूर्णाङ्क ncoeffs = coeffs.rows + coeffs.cols - 1;

    यदि( ctype == CV_32FC1 )
    {
        यदि( ncoeffs == 4 )
            a0 = coeffs.at<भिन्न>(++i);

        a1 = coeffs.at<भिन्न>(i+1);
        a2 = coeffs.at<भिन्न>(i+2);
        a3 = coeffs.at<भिन्न>(i+3);
    }
    अन्यथा
    {
        यदि( ncoeffs == 4 )
            a0 = coeffs.at<द्विगुण>(++i);

        a1 = coeffs.at<द्विगुण>(i+1);
        a2 = coeffs.at<द्विगुण>(i+2);
        a3 = coeffs.at<द्विगुण>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    द्विगुण max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    यदि (max_coeff < std::numeric_limits<द्विगुण>::epsilon())
        प्रत्यागम -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    यदि( std::abs(a0) < std::numeric_limits<द्विगुण>::epsilon() * max_coeff )
    {
        यदि( std::abs(a1) < std::numeric_limits<द्विगुण>::epsilon() * max_coeff )
        {
            यदि( std::abs(a2) < std::numeric_limits<द्विगुण>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<द्विगुण>::epsilon() * max_coeff ? -1 : 0;
            अन्यथा
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        अन्यथा
        {
            // quadratic equation
            द्विगुण d = a2*a2 - 4*a1*a3;
            यदि( d >= 0 )
            {
                d = std::sqrt(d);
                द्विगुण q1 = (-a2 + d) * 0.5;
                द्विगुण q2 = (a2 + d) * -0.5;
                यदि( std::abs(q1) > std::abs(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                अन्यथा
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    अन्यथा
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        द्विगुण scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        द्विगुण Q = (a1 * a1 - 3 * a2) * (1./9);
        द्विगुण R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        द्विगुण Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        द्विगुण d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        यदि( d > 0 )
        {
            द्विगुण theta = acos(R / sqrt(Qcubed));
            द्विगुण sqrtQ = sqrt(Q);
            द्विगुण t0 = -2 * sqrtQ;
            द्विगुण t1 = theta * (1./3);
            द्विगुण t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        अन्यथा यदि( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        अन्यथा
        {
            द्विगुण e;
            d = sqrt(-d);
            e = std::cbrt(d + fabs(R));
            यदि( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    यदि( roots.type() == CV_32FC1 )
    {
        roots.at<भिन्न>(0) = (भिन्न)x0;
        roots.at<भिन्न>(1) = (भिन्न)x1;
        roots.at<भिन्न>(2) = (भिन्न)x2;
    }
    अन्यथा
    {
        roots.at<द्विगुण>(0) = x0;
        roots.at<द्विगुण>(1) = x1;
        roots.at<द्विगुण>(2) = x2;
    }

    प्रत्यागम n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
द्विगुण cv::solvePoly( निवेश_Array _coeffs0, OutputArray _roots0, पूर्णाङ्क maxIters )
{
    CV_INSTRUMENT_REGION();

    प्रकारनिर्देश Complex<द्विगुण> C;

    द्विगुण maxDiff = 0;
    पूर्णाङ्क iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    पूर्णाङ्क ctype = _coeffs0.type();
    पूर्णाङ्क cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.total() >= 2 );

    पूर्णाङ्क n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, सत्य, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    AutoBuffer<C> buf(n*2+2);
    C *coeffs = buf.data(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    यदि( coeffs0.channels() == 1 )
    {
        ध्रुव द्विगुण* rcoeffs = (ध्रुव द्विगुण*)roots;
        प्रति( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    प्रति( ; n > 1; n-- )
    {
        यदि( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            भङ्ग;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    प्रति( i = 0; i <= n; i++ )
        absCoeffs.at<द्विगुण>(i) = abs(coeffs[i]);

    पूर्णाङ्क nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    प्रति( i = 0; i <= n; i++ )
    {
        द्विगुण coeff = absCoeffs.at<द्विगुण>(i);
        यदि( coeff > DBL_EPSILON )
        {
            यदि( i != n )
                u.at<द्विगुण>(i) = 2.0 * pow(coeff / absCoeffs.at<द्विगुण>(n), 1.0 / (n - i));
            यदि( i != 0 )
                v.at<द्विगुण>(i - 1) = 0.5 * pow(absCoeffs.at<द्विगुण>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    द्विगुण scale = 1;
    यदि( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<द्विगुण>(maxU) = 0;
        v.at<द्विगुण>(minV) = 0;
        scale = (sum(u).val[0] + sum(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    प्रति( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    प्रति( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        प्रति( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            पूर्णाङ्क अंक_same_मूल = 1;
            प्रति( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                यदि( j != i )
                {
                    यदि ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    अन्यथा
                        अंक_same_मूल++;
                }
            }
            num /= denom;
            यदि( अंक_same_मूल > 1)
            {
                द्विगुण old_अंक_re = num.re;
                द्विगुण old_अंक_im = num.im;
                पूर्णाङ्क square_मूल_times = अंक_same_मूल % 2 == 0 ? अंक_same_मूल / 2 : अंक_same_मूल / 2 - 1;

                प्रति( j = 0; j < square_मूल_times; j++)
                {
                    num.re = old_अंक_re*old_अंक_re + old_अंक_im*old_अंक_im;
                    num.re = sqrt(num.re);
                    num.re += old_अंक_re;
                    num.im = num.re - old_अंक_re;
                    num.re /= 2;
                    num.re = sqrt(num.re);

                    num.im /= 2;
                    num.im = sqrt(num.im);
                    यदि( old_अंक_re < 0 ) num.im = -num.im;
                }

                यदि( अंक_same_मूल % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<द्विगुण>(3) = -(std::pow(old_अंक_re, 3));
                    cube_coefs.at<द्विगुण>(2) = -(15*std::pow(old_अंक_re, 2) + 27*std::pow(old_अंक_im, 2));
                    cube_coefs.at<द्विगुण>(1) = -48*old_अंक_re;
                    cube_coefs.at<द्विगुण>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<द्विगुण>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_अंक_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        यदि( maxDiff <= 0 )
            भङ्ग;
    }

    यदि( coeffs0.channels() == 1 )
    {
        ध्रुव द्विगुण verySmallEps = 1e-100;
        प्रति( i = 0; i < n; i++ )
            यदि( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    प्रति( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    प्रत्यागम maxDiff;
}


/* End of file. */
