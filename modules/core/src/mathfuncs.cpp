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

#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश <atomic>
#समावेश <limits>
#समावेश <iostream>
#समावेश <algorithm>
#समावेश <cmath>
#समावेश "mathfuncs.hpp"
#समावेश "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

namespace cv
{

प्रकार व्योम (*MathFunc)(स्थिर व्योम* src, व्योम* dst, पूर्णांक len);

#अगर_घोषित HAVE_OPENCL

क्रमागत { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

अटल स्थिर अक्षर* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

अटल bool ocl_math_op(निवेश_Array _src1, निवेश_Array _src2, OutputArray _dst, पूर्णांक oclop)
{
    पूर्णांक type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    पूर्णांक kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    स्थिर ocl::Device d = ocl::Device::getDefault();
    bool double_support = d.doubleFPConfig() > 0;
    अगर (!double_support && depth == CV_64F)
        वापस false;
    पूर्णांक rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    अगर (k.empty())
        वापस false;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.create_Same_आकार(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_आकार(src1),
            src2arg = ocl::KernelArg::Read_Only_No_आकार(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    अगर (src2.empty())
        k.args(src1arg, dstarg);
    अन्यथा
        k.args(src1arg, src2arg, dstarg);

    माप_प्रकार globalsize[] = { (माप_प्रकार)src1.cols * cn / kercn, ((माप_प्रकार)src1.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, 0, false);
}

#पूर्ण_अगर

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN भग्न cube_मूल( भग्न value )
{
    CV_INSTRUMENT_REGION();

    भग्न fr;
    Cv32suf v, m;
    पूर्णांक ix, s;
    पूर्णांक ex, shx;

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
    fr = (भग्न)(((((45.2548339756803022511987494 * fr +
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
    वापस v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

व्योम magnitude( निवेश_Array src1, निवेश_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    स्थिर Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक len = (पूर्णांक)it.size*cn;

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        अगर( depth == CV_32F )
        {
            स्थिर भग्न *x = (स्थिर भग्न*)ptrs[0], *y = (स्थिर भग्न*)ptrs[1];
            भग्न *mag = (भग्न*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        अन्यथा
        {
            स्थिर द्विगुन *x = (स्थिर द्विगुन*)ptrs[0], *y = (स्थिर द्विगुन*)ptrs[1];
            द्विगुन *mag = (द्विगुन*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

व्योम phase( निवेश_Array src1, निवेश_Array src2, OutputArray dst, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    स्थिर Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक j, योग = (पूर्णांक)(it.size*cn), block_आकार = योग;
    माप_प्रकार esz1 = X.elemSize1();
    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( j = 0; j < योग; j += block_आकार )
        {
            पूर्णांक len = std::min(योग - j, block_आकार);
            अगर( depth == CV_32F )
            {
                स्थिर भग्न *x = (स्थिर भग्न*)ptrs[0], *y = (स्थिर भग्न*)ptrs[1];
                भग्न *angle = (भग्न*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            अन्यथा
            {
                स्थिर द्विगुन *x = (स्थिर द्विगुन*)ptrs[0], *y = (स्थिर द्विगुन*)ptrs[1];
                द्विगुन *angle = (द्विगुन*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_cartToPolar( निवेश_Array _src1, निवेश_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, bool angleInDegrees )
{
    स्थिर ocl::Device & d = ocl::Device::getDefault();
    पूर्णांक type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    bool doubleSupport = d.doubleFPConfig() > 0;

    स्थिर bool _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    स्थिर bool _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    स्थिर bool _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    स्थिर bool _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    अगर ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        वापस false;

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
    अगर (k.empty())
        वापस false;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    आकार size = src1.size();
    CV_Assert( size == src2.size() );

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.args(_src1IsDstMag || _src1IsDstAngle ? ocl::KernelArg::Read_Write_No_आकार(src1) : ocl::KernelArg::Read_Only_No_आकार(src1),
           _src2IsDstMag || _src2IsDstAngle ? ocl::KernelArg::Read_Write_No_आकार(src2) : ocl::KernelArg::Read_Only_No_आकार(src2),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_आकार(dst2));

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst1.cols * cn, ((माप_प्रकार)dst1.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

व्योम cartToPolar( निवेश_Array src1, निवेश_Array src2,
                  OutputArray dst1, OutputArray dst2, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    पूर्णांक type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    स्थिर Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक j, योग = (पूर्णांक)(it.size*cn), block_आकार = std::min(योग, ((BLOCK_आकार+cn-1)/cn)*cn);
    माप_प्रकार esz1 = X.elemSize1();

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( j = 0; j < योग; j += block_आकार )
        {
            पूर्णांक len = std::min(योग - j, block_आकार);
            अगर( depth == CV_32F )
            {
                स्थिर भग्न *x = (स्थिर भग्न*)ptrs[0], *y = (स्थिर भग्न*)ptrs[1];
                भग्न *mag = (भग्न*)ptrs[2], *angle = (भग्न*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            अन्यथा
            {
                स्थिर द्विगुन *x = (स्थिर द्विगुन*)ptrs[0], *y = (स्थिर द्विगुन*)ptrs[1];
                द्विगुन *mag = (द्विगुन*)ptrs[2], *angle = (द्विगुन*)ptrs[3];
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

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_polarToCart( निवेश_Array _mag, निवेश_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, bool angleInDegrees )
{
    स्थिर ocl::Device & d = ocl::Device::getDefault();
    पूर्णांक type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    bool doubleSupport = d.doubleFPConfig() > 0;

    स्थिर bool _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    स्थिर bool _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    स्थिर bool _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    स्थिर bool _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    अगर ( !doubleSupport && depth == CV_64F )
        वापस false;

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
    अगर (k.empty())
        वापस false;

    UMat mag = _mag.getUMat(), angle = _angle.getUMat();
    आकार size = angle.size();
    CV_Assert(mag.size() == size);

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.args(_src1IsDstX || _src1IsDstY ? ocl::KernelArg::Read_Write_No_आकार(mag) : ocl::KernelArg::Read_Only_No_आकार(mag),
           _src2IsDstX || _src2IsDstY  ? ocl::KernelArg::Read_Write_No_आकार(angle) : ocl::KernelArg::Read_Only_No_आकार(angle),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_आकार(dst2));

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst1.cols * cn, ((माप_प्रकार)dst1.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

व्योम polarToCart( निवेश_Array src1, निवेश_Array src2,
                  OutputArray dst1, OutputArray dst2, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    पूर्णांक type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    स्थिर Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक j, योग = (पूर्णांक)(it.size*cn), block_आकार = std::min(योग, ((BLOCK_आकार+cn-1)/cn)*cn);
    माप_प्रकार esz1 = Angle.elemSize1();

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( j = 0; j < योग; j += block_आकार )
        {
            पूर्णांक len = std::min(योग - j, block_आकार);
            अगर ( depth == CV_32F )
            {
                स्थिर भग्न *mag = (स्थिर भग्न*)ptrs[0], *angle = (स्थिर भग्न*)ptrs[1];
                भग्न *x = (भग्न*)ptrs[2], *y = (भग्न*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            अन्यथा
            {
                स्थिर द्विगुन *mag = (स्थिर द्विगुन*)ptrs[0], *angle = (स्थिर द्विगुन*)ptrs[1];
                द्विगुन *x = (द्विगुन*)ptrs[2], *y = (द्विगुन*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            अगर( ptrs[0] )
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
क्रमागत { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

अटल व्योम math_op(ew::शीर्ष op, निवेश_Array _src, OutputArray _dst)
{
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.create_Same_आकार(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    अगर (src.empty())
        वापस;

    स्थिर माप_प्रकार योग = src.योग() * src.channels();
    अगर (src.isContinuous() && dst.isContinuous() && योग <= (माप_प्रकार)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        अटल स्थिर द्विगुन noparams[4] = {};
        k.fptr(src.तथ्य, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.तथ्य, 0, (पूर्णांक)योग, 1, noparams, k.flags, k.userdata);
        वापस;
    }

    ew::TExpr p;
    स्थिर पूर्णांक a = p.add_निवेश(depth);
    स्थिर पूर्णांक out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    स्थिर Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

व्योम exp( निवेश_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] पूर्णांक depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

व्योम log( निवेश_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] पूर्णांक depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

ढाँचा <typename T, typename WT>
काष्ठा iPow_SIMD
{
    पूर्णांक चालक() ( स्थिर T *, T *, पूर्णांक, पूर्णांक)
    {
        वापस 0;
    }
};

#अगर (CV_SIMD || CV_SIMD_SCALABLE)

ढाँचा <>
काष्ठा iPow_SIMD<uchar, अचिन्हित>
{
    पूर्णांक चालक() ( स्थिर uchar * src, uchar * dst, पूर्णांक len, पूर्णांक power )
    {
        पूर्णांक i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        क्रम ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            पूर्णांक p = power;

            जबतक( p > 1 )
            {
                अगर (p & 1)
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

        वापस i;
    }
};

ढाँचा <>
काष्ठा iPow_SIMD<schar, पूर्णांक>
{
    पूर्णांक चालक() ( स्थिर schar * src, schar * dst, पूर्णांक len, पूर्णांक power)
    {
        पूर्णांक i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        क्रम ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            पूर्णांक p = power;

            जबतक( p > 1 )
            {
                अगर (p & 1)
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

        वापस i;
    }
};

ढाँचा <>
काष्ठा iPow_SIMD<ushort, अचिन्हित>
{
    पूर्णांक चालक() ( स्थिर ushort * src, ushort * dst, पूर्णांक len, पूर्णांक power)
    {
        पूर्णांक i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        क्रम ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            पूर्णांक p = power;

            जबतक( p > 1 )
            {
                अगर (p & 1)
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

        वापस i;
    }
};

ढाँचा <>
काष्ठा iPow_SIMD<लघु, पूर्णांक>
{
    पूर्णांक चालक() ( स्थिर लघु * src, लघु * dst, पूर्णांक len, पूर्णांक power)
    {
        पूर्णांक i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        क्रम ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            पूर्णांक p = power;

            जबतक( p > 1 )
            {
                अगर (p & 1)
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

        वापस i;
    }
};

ढाँचा <>
काष्ठा iPow_SIMD<पूर्णांक, पूर्णांक>
{
    पूर्णांक चालक() ( स्थिर पूर्णांक * src, पूर्णांक * dst, पूर्णांक len, पूर्णांक power)
    {
        पूर्णांक i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        क्रम ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            पूर्णांक p = power;

            जबतक( p > 1 )
            {
                अगर (p & 1)
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

        वापस i;
    }
};

ढाँचा <>
काष्ठा iPow_SIMD<भग्न, भग्न>
{
    पूर्णांक चालक() ( स्थिर भग्न * src, भग्न * dst, पूर्णांक len, पूर्णांक power)
    {
        पूर्णांक i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        क्रम ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            पूर्णांक p = std::असल(power);
            अगर( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            जबतक( p > 1 )
            {
                अगर (p & 1)
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

        वापस i;
    }
};

#अगर (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
ढाँचा <>
काष्ठा iPow_SIMD<द्विगुन, द्विगुन>
{
    पूर्णांक चालक() ( स्थिर द्विगुन * src, द्विगुन * dst, पूर्णांक len, पूर्णांक power)
    {
        पूर्णांक i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        क्रम ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            पूर्णांक p = std::असल(power);
            अगर( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            जबतक( p > 1 )
            {
                अगर (p & 1)
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

        वापस i;
    }
};
#पूर्ण_अगर

#पूर्ण_अगर

ढाँचा<typename T, typename WT>
अटल व्योम
iPow_i( स्थिर व्योम* _src, व्योम* _dst, पूर्णांक len, पूर्णांक power )
{
    स्थिर T* src = (स्थिर T*)_src;
    T* dst = (T*)_dst;
    अगर( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        क्रम( पूर्णांक i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    अन्यथा
    {
        iPow_SIMD<T, WT> vop;
        पूर्णांक i = vop(src, dst, len, power);

        क्रम( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            पूर्णांक p = power;
            जबतक( p > 1 )
            {
                अगर( p & 1 )
                    a *= b;
                b *= b;
                p >>= 1;
            }

            a *= b;
            dst[i] = saturate_cast<T>(a);
        }
    }
}

ढाँचा<typename T>
अटल व्योम
iPow_f( स्थिर व्योम* _src, व्योम* _dst, पूर्णांक len, पूर्णांक power0 )
{
    स्थिर T* src = (स्थिर T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    पूर्णांक i = vop(src, dst, len, power0);
    पूर्णांक power = std::असल(power0);

    क्रम( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        पूर्णांक p = power;
        अगर( power0 < 0 )
            b = 1/b;

        जबतक( p > 1 )
        {
            अगर( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

प्रकार व्योम (*IPowFunc)( स्थिर व्योम* src, व्योम* dst, पूर्णांक len, पूर्णांक power );

अटल IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, अचिन्हित>, iPow_i<schar, पूर्णांक>, iPow_i<ushort, अचिन्हित>, iPow_i<लघु, पूर्णांक>,
    iPow_i<पूर्णांक, पूर्णांक>, iPow_f<भग्न>, iPow_f<द्विगुन>, 0
};

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_pow(निवेश_Array _src, द्विगुन power, OutputArray _dst,
                    bool is_ipower, पूर्णांक ipower)
{
    स्थिर ocl::Device & d = ocl::Device::getDefault();
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    bool doubleSupport = d.doubleFPConfig() > 0;

    _dst.create_Same_आकार(_src, type);
    अगर (is_ipower)
    {
        अगर( ipower < 0 )
        {
            अगर( depth == CV_32F || depth == CV_64F )
                is_ipower = false;
            अन्यथा
                वापस false;
        }
    }

    अगर (depth == CV_64F && !doubleSupport)
        वापस false;

    bool issqrt = std::असल(power - 0.5) < द्विग_अंतर;
    स्थिर अक्षर * स्थिर op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    अगर (is_ipower)
    {
        पूर्णांक wdepth = CV_32F;
        अगर (depth == CV_64F)
            wdepth = CV_64F;
        अन्यथा अगर (depth == CV_16F)
            wdepth = CV_16F;

        अक्षर cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], माप(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], माप(cvt[1])),
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
    अगर (k.empty())
        वापस false;

    UMat src = _src.getUMat();
    _dst.create_Same_आकार(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    अगर (issqrt)
        k.args(srcarg, dstarg);
    अन्यथा अगर (is_ipower)
        k.args(srcarg, dstarg, ipower);
    अन्यथा
    {
        अगर (depth == CV_32F)
            k.args(srcarg, dstarg, (भग्न)power);
        अन्यथा
            k.args(srcarg, dstarg, power);
    }

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst.cols *  cn, ((माप_प्रकार)dst.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

व्योम घात( निवेश_Array _src, द्विगुन power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    bool is_ipower = भ_असल(ipower - power) < द्विग_अंतर;
#अगर_घोषित HAVE_OPENCL
    bool useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#पूर्ण_अगर

    अगर (is_ipower)
    {
        चयन( ipower )
        {
        हाल 0:
            _dst.create_Same_आकार(_src, type);
            _dst.setTo(Scalar::all(1));
            वापस;
        हाल 1:
            _src.copyTo(_dst);
            वापस;
        हाल 2:
            multiply(_src, _src, _dst);
            वापस;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    स्थिर bool floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    अगर( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.create_Same_आकार(_src, type);
        Mat dst = _dst.getMat();

        स्थिर Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णांक len = (पूर्णांक)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        वापस;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.create_Same_आकार(_src, type);
    Mat dst = _dst.getMat();
    अगर (src.empty())
        वापस;

    स्थिर माप_प्रकार योग = src.योग() * cn;
    अगर (floatDepth && src.isContinuous() && dst.isContinuous() && योग <= (माप_प्रकार)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        अगर (k.fptr)
        {
            द्विगुन pvstore;                    // the broadcast exponent, stored as T
            व्योम* pv = &pvstore;
            चयन (depth)
            {
            हाल CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); अवरोध;
            हाल CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); अवरोध;
            हाल CV_32F:  *(भग्न*)pv = (भग्न)power; अवरोध;
            शेष:      pvstore = power; अवरोध;
            }
            अटल स्थिर द्विगुन noparams[4] = {};
            k.fptr(src.तथ्य, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.तथ्य, 0, (पूर्णांक)योग, 1, noparams, k.flags, k.userdata);
            वापस;
        }
    }

    ew::TExpr prog;
    स्थिर पूर्णांक a = prog.add_निवेश(depth);
    स्थिर पूर्णांक c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    स्थिर पूर्णांक out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_द्विआधारी(ew::OP_POW, a, c, depth), out);
    prog.compile();
    स्थिर Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

व्योम वर्ग_मूल(निवेश_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    अगर (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::घात(a, 0.5, b);
        वापस;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

ढाँचा<पूर्णांक cv_mat_type> काष्ठा mat_type_assotiations{};

ढाँचा<> काष्ठा mat_type_assotiations<CV_8U>
{
    प्रकार अचिन्हित अक्षर type;
    अटल स्थिर type min_allowable = 0x0;
    अटल स्थिर type max_allowable = 0xFF;
};

ढाँचा<> काष्ठा mat_type_assotiations<CV_8S>
{
    प्रकार चिन्हित अक्षर type;
    अटल स्थिर type min_allowable = च_अक्षर_न्यून;
    अटल स्थिर type max_allowable = च_अक्षर_उच्च;
};

ढाँचा<> काष्ठा mat_type_assotiations<CV_16U>
{
    प्रकार अचिन्हित लघु type;
    अटल स्थिर type min_allowable = 0x0;
    अटल स्थिर type max_allowable = अच_लघु_उच्च;
};
ढाँचा<> काष्ठा mat_type_assotiations<CV_16S>
{
    प्रकार चिन्हित लघु type;
    अटल स्थिर type min_allowable = लघु_न्यून;
    अटल स्थिर type max_allowable = लघु_उच्च;
};

ढाँचा<> काष्ठा mat_type_assotiations<CV_32S>
{
    प्रकार पूर्णांक type;
    अटल स्थिर type min_allowable = (-पूर्णांक_उच्च - 1);
    अटल स्थिर type max_allowable = पूर्णांक_उच्च;
};

ढाँचा<पूर्णांक depth>
अटल bool checkIntegerRange(cv::Mat src, Point& bad_pt, पूर्णांक minVal, पूर्णांक maxVal)
{
    प्रकार mat_type_assotiations<depth> type_ass;

    अगर (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        वापस true;
    }
    अन्यथा अगर (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        वापस false;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    क्रम (पूर्णांक j = 0; j < as_one_channel.rows; ++j)
        क्रम (पूर्णांक i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            अगर (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                वापस false;
            }
        }

    वापस true;
}

प्रकार bool (*check_range_function)(cv::Mat src, Point& bad_pt, पूर्णांक minVal, पूर्णांक maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

ढाँचा<typename T>
अटल व्योम checkTypedRange(Mat& src, Point& badPt, द्विगुन minVal, द्विगुन maxVal)
{
    पूर्णांक loc = 0;
    पूर्णांक cn = src.channels();
    आकार size = getContinuousSize2D(src, cn);
    स्थिर T* isrc = src.ptr<T>();
    माप_प्रकार step = src.step / माप(isrc[0]);

    क्रम( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        क्रम( पूर्णांक i = 0; i < size.width; i++ )
        {
            द्विगुन val = (द्विगुन)isrc[i];
            अगर( !(val >= minVal && val < maxVal) )
            {
                पूर्णांक pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                अवरोध;
            }
        }
    }
}

bool checkRange(निवेश_Array _src, bool quiet, Point* pt, द्विगुन minVal, द्विगुन maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    अगर ( src.dims > 2 )
    {
        CV_Assert(pt == शून्य); // no way to provide location info

        स्थिर Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        क्रम ( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            अगर (!checkRange( it.planes[0], quiet, शून्य, minVal, maxVal ))
            {
                वापस false;
            }
        }
        वापस true;
    }

    पूर्णांक depth = src.depth();
    Point badPt(-1, -1);

    अगर (depth < CV_32F)
    {
        पूर्णांक minVali = minVal <= पूर्णांक_न्यून ? पूर्णांक_न्यून : cvFloor(minVal);
        पूर्णांक maxVali = maxVal > पूर्णांक_उच्च ? पूर्णांक_उच्च : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    अन्यथा
    {
        पूर्णांक i, loc = 0;
        पूर्णांक cn = src.channels();
        आकार size = getContinuousSize2D(src, cn);

        अगर( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        अन्यथा अगर( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        अन्यथा अगर( depth == CV_32F )
        {
            Cv32suf a, b;
            पूर्णांक ia, ib;
            स्थिर पूर्णांक* isrc = src.ptr<पूर्णांक>();
            माप_प्रकार step = src.step/माप(isrc[0]);

            a.f = (भग्न)std::max(minVal, (द्विगुन)-भग्न_उच्च);
            b.f = (भग्न)std::min(maxVal, (द्विगुन)भग्न_उच्च);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            क्रम( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                क्रम( i = 0; i < size.width; i++ )
                {
                    पूर्णांक val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    अगर( val < ia || val >= ib )
                    {
                        पूर्णांक pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        अवरोध;
                    }
                }
            }
        }
        अन्यथा अगर( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            स्थिर int64* isrc = src.ptr<int64>();
            माप_प्रकार step = src.step/माप(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            क्रम( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                क्रम( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    अगर( val < ia || val >= ib )
                    {
                        पूर्णांक pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        अवरोध;
                    }
                }
            }
        }
        अन्यथा अगर( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        अन्यथा अगर( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        अन्यथा अगर( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        अन्यथा अगर( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        अन्यथा
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    अगर( badPt.x >= 0 )
    {
        अगर( pt )
            *pt = badPt;
        अगर( !quiet )
        {
            cv::String मूल्य_str;
            मूल्य_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, मूल्य_str.c_str(), minVal, maxVal));
        }
        वापस false;
    }
    वापस true;
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

पूर्णांक cv::solveCubic( निवेश_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    स्थिर पूर्णांक n0 = 3;
    Mat coeffs = _coeffs.getMat();
    पूर्णांक ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == आकार(n0, 1) ||
                coeffs.size() == आकार(n0+1, 1) ||
                coeffs.size() == आकार(1, n0) ||
                coeffs.size() == आकार(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, true, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    पूर्णांक i = -1, n = 0;
    द्विगुन a0 = 1., a1, a2, a3;
    द्विगुन x0 = 0., x1 = 0., x2 = 0.;
    पूर्णांक ncoeffs = coeffs.rows + coeffs.cols - 1;

    अगर( ctype == CV_32FC1 )
    {
        अगर( ncoeffs == 4 )
            a0 = coeffs.at<भग्न>(++i);

        a1 = coeffs.at<भग्न>(i+1);
        a2 = coeffs.at<भग्न>(i+2);
        a3 = coeffs.at<भग्न>(i+3);
    }
    अन्यथा
    {
        अगर( ncoeffs == 4 )
            a0 = coeffs.at<द्विगुन>(++i);

        a1 = coeffs.at<द्विगुन>(i+1);
        a2 = coeffs.at<द्विगुन>(i+2);
        a3 = coeffs.at<द्विगुन>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    द्विगुन max_coeff = std::max({std::असल(a0), std::असल(a1), std::असल(a2), std::असल(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    अगर (max_coeff < std::numeric_limits<द्विगुन>::epsilon())
        वापस -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    अगर( std::असल(a0) < std::numeric_limits<द्विगुन>::epsilon() * max_coeff )
    {
        अगर( std::असल(a1) < std::numeric_limits<द्विगुन>::epsilon() * max_coeff )
        {
            अगर( std::असल(a2) < std::numeric_limits<द्विगुन>::epsilon() * max_coeff )
                n = std::असल(a3) < std::numeric_limits<द्विगुन>::epsilon() * max_coeff ? -1 : 0;
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
            द्विगुन d = a2*a2 - 4*a1*a3;
            अगर( d >= 0 )
            {
                d = std::वर्ग_मूल(d);
                द्विगुन q1 = (-a2 + d) * 0.5;
                द्विगुन q2 = (a2 + d) * -0.5;
                अगर( std::असल(q1) > std::असल(q2) )
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
        द्विगुन scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        द्विगुन Q = (a1 * a1 - 3 * a2) * (1./9);
        द्विगुन R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        द्विगुन Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        द्विगुन d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        अगर( d > 0 )
        {
            द्विगुन theta = acos(R / वर्ग_मूल(Qcubed));
            द्विगुन sqrtQ = वर्ग_मूल(Q);
            द्विगुन t0 = -2 * sqrtQ;
            द्विगुन t1 = theta * (1./3);
            द्विगुन t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        अन्यथा अगर( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        अन्यथा
        {
            द्विगुन e;
            d = वर्ग_मूल(-d);
            e = std::cbrt(d + भ_असल(R));
            अगर( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    अगर( roots.type() == CV_32FC1 )
    {
        roots.at<भग्न>(0) = (भग्न)x0;
        roots.at<भग्न>(1) = (भग्न)x1;
        roots.at<भग्न>(2) = (भग्न)x2;
    }
    अन्यथा
    {
        roots.at<द्विगुन>(0) = x0;
        roots.at<द्विगुन>(1) = x1;
        roots.at<द्विगुन>(2) = x2;
    }

    वापस n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
द्विगुन cv::solvePoly( निवेश_Array _coeffs0, OutputArray _roots0, पूर्णांक maxIters )
{
    CV_INSTRUMENT_REGION();

    प्रकार Complex<द्विगुन> C;

    द्विगुन maxDiff = 0;
    पूर्णांक iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    पूर्णांक ctype = _coeffs0.type();
    पूर्णांक cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.योग() >= 2 );

    पूर्णांक n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, true, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    Auto_बफर<C> buf(n*2+2);
    C *coeffs = buf.तथ्य(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    अगर( coeffs0.channels() == 1 )
    {
        स्थिर द्विगुन* rcoeffs = (स्थिर द्विगुन*)roots;
        क्रम( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    क्रम( ; n > 1; n-- )
    {
        अगर( std::असल(coeffs[n].re) + std::असल(coeffs[n].im) > द्विग_अंतर )
            अवरोध;
    }
    CV_Assert( std::असल(coeffs[n].re) + std::असल(coeffs[n].im) > द्विग_अंतर );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    क्रम( i = 0; i <= n; i++ )
        absCoeffs.at<द्विगुन>(i) = असल(coeffs[i]);

    पूर्णांक nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    क्रम( i = 0; i <= n; i++ )
    {
        द्विगुन coeff = absCoeffs.at<द्विगुन>(i);
        अगर( coeff > द्विग_अंतर )
        {
            अगर( i != n )
                u.at<द्विगुन>(i) = 2.0 * घात(coeff / absCoeffs.at<द्विगुन>(n), 1.0 / (n - i));
            अगर( i != 0 )
                v.at<द्विगुन>(i - 1) = 0.5 * घात(absCoeffs.at<द्विगुन>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    द्विगुन scale = 1;
    अगर( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<द्विगुन>(maxU) = 0;
        v.at<द्विगुन>(minV) = 0;
        scale = (योग_2(u).val[0] + योग_2(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    क्रम( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    क्रम( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        क्रम( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            पूर्णांक अंक_same_मूल = 1;
            क्रम( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                अगर( j != i )
                {
                    अगर ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    अन्यथा
                        अंक_same_मूल++;
                }
            }
            num /= denom;
            अगर( अंक_same_मूल > 1)
            {
                द्विगुन old_अंक_re = num.re;
                द्विगुन old_अंक_im = num.im;
                पूर्णांक square_मूल_times = अंक_same_मूल % 2 == 0 ? अंक_same_मूल / 2 : अंक_same_मूल / 2 - 1;

                क्रम( j = 0; j < square_मूल_times; j++)
                {
                    num.re = old_अंक_re*old_अंक_re + old_अंक_im*old_अंक_im;
                    num.re = वर्ग_मूल(num.re);
                    num.re += old_अंक_re;
                    num.im = num.re - old_अंक_re;
                    num.re /= 2;
                    num.re = वर्ग_मूल(num.re);

                    num.im /= 2;
                    num.im = वर्ग_मूल(num.im);
                    अगर( old_अंक_re < 0 ) num.im = -num.im;
                }

                अगर( अंक_same_मूल % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<द्विगुन>(3) = -(std::घात(old_अंक_re, 3));
                    cube_coefs.at<द्विगुन>(2) = -(15*std::घात(old_अंक_re, 2) + 27*std::घात(old_अंक_im, 2));
                    cube_coefs.at<द्विगुन>(1) = -48*old_अंक_re;
                    cube_coefs.at<द्विगुन>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<द्विगुन>(0));
                    num.im = वर्ग_मूल(std::घात(num.re, 2) / 3 - old_अंक_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::असल(num));
        }
        अगर( maxDiff <= 0 )
            अवरोध;
    }

    अगर( coeffs0.channels() == 1 )
    {
        स्थिर द्विगुन verySmallEps = 1e-100;
        क्रम( i = 0; i < n; i++ )
            अगर( भ_असल(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    क्रम( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    वापस maxDiff;
}


/* End of file. */
