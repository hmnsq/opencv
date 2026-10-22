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

#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include <atomic>
#include <limits>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "mathfuncs.hpp"
#include "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

namespace cv
{

typedef void (*MathFunc)(const void* src, void* dst, ຈຳນວນເຕັມ len);

#ifdef HAVE_OPENCL

ga_nanaa { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

static const var_na* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

static bool ocl_math_op(InputArray _src1, InputArray _src2, OutputArray _dst, ຈຳນວນເຕັມ oclop)
{
    ຈຳນວນເຕັມ type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    ຈຳນວນເຕັມ kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    const ocl::Device d = ocl::Device::getDefault();
    bool double_support = d.doubleFPConfig() > 0;
    ຖ້າ (!double_support && depth == CV_64F)
        ສົ່ງຄືນ false;
    ຈຳນວນເຕັມ rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    ຖ້າ (k.empty())
        ສົ່ງຄືນ false;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.createSameSize(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ຖ້າ (src2.empty())
        k.args(src1arg, dstarg);
    ບໍ່ດັ່ງນັ້ນ
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    ສົ່ງຄືນ k.run(2, globalsize, 0, false);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN float cubeRoot( float value )
{
    CV_INSTRUMENT_REGION();

    float fr;
    Cv32suf v, m;
    ຈຳນວນເຕັມ ix, s;
    ຈຳນວນເຕັມ ex, shx;

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
    fr = (float)(((((45.2548339756803022511987494 * fr +
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
    ສົ່ງຄືນ v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

void magnitude( InputArray src1, InputArray src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    const Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ຈຳນວນເຕັມ len = (ຈຳນວນເຕັມ)it.size*cn;

    ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ຖ້າ( depth == CV_32F )
        {
            const float *x = (const float*)ptrs[0], *y = (const float*)ptrs[1];
            float *mag = (float*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            const dviguu_na *x = (const dviguu_na*)ptrs[0], *y = (const dviguu_na*)ptrs[1];
            dviguu_na *mag = (dviguu_na*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

void phase( InputArray src1, InputArray src2, OutputArray dst, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    const Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    ຈຳນວນເຕັມ j, total = (ຈຳນວນເຕັມ)(it.size*cn), blockSize = total;
    size_t esz1 = X.elemSize1();
    ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ສຳລັບ( j = 0; j < total; j += blockSize )
        {
            ຈຳນວນເຕັມ len = std::min(total - j, blockSize);
            ຖ້າ( depth == CV_32F )
            {
                const float *x = (const float*)ptrs[0], *y = (const float*)ptrs[1];
                float *angle = (float*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                const dviguu_na *x = (const dviguu_na*)ptrs[0], *y = (const dviguu_na*)ptrs[1];
                dviguu_na *angle = (dviguu_na*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

static bool ocl_cartToPolar( InputArray _src1, InputArray _src2,
                             OutputArray _dst1, OutputArray _dst2, bool angleInDegrees )
{
    const ocl::Device & d = ocl::Device::getDefault();
    ຈຳນວນເຕັມ type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    bool doubleSupport = d.doubleFPConfig() > 0;

    const bool _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    const bool _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    const bool _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    const bool _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    ຖ້າ ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        ສົ່ງຄືນ false;

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
    ຖ້າ (k.empty())
        ສົ່ງຄືນ false;

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
    ສົ່ງຄືນ k.run(2, globalsize, NULL, false);
}

#endif

void cartToPolar( InputArray src1, InputArray src2,
                  OutputArray dst1, OutputArray dst2, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    ຈຳນວນເຕັມ type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    const Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ຈຳນວນເຕັມ j, total = (ຈຳນວນເຕັມ)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ສຳລັບ( j = 0; j < total; j += blockSize )
        {
            ຈຳນວນເຕັມ len = std::min(total - j, blockSize);
            ຖ້າ( depth == CV_32F )
            {
                const float *x = (const float*)ptrs[0], *y = (const float*)ptrs[1];
                float *mag = (float*)ptrs[2], *angle = (float*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                const dviguu_na *x = (const dviguu_na*)ptrs[0], *y = (const dviguu_na*)ptrs[1];
                dviguu_na *mag = (dviguu_na*)ptrs[2], *angle = (dviguu_na*)ptrs[3];
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

static bool ocl_polarToCart( InputArray _mag, InputArray _angle,
                             OutputArray _dst1, OutputArray _dst2, bool angleInDegrees )
{
    const ocl::Device & d = ocl::Device::getDefault();
    ຈຳນວນເຕັມ type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    bool doubleSupport = d.doubleFPConfig() > 0;

    const bool _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    const bool _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    const bool _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    const bool _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    ຖ້າ ( !doubleSupport && depth == CV_64F )
        ສົ່ງຄືນ false;

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
    ຖ້າ (k.empty())
        ສົ່ງຄືນ false;

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
    ສົ່ງຄືນ k.run(2, globalsize, NULL, false);
}

#endif

void polarToCart( InputArray src1, InputArray src2,
                  OutputArray dst1, OutputArray dst2, bool angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    ຈຳນວນເຕັມ type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    const Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    ຈຳນວນເຕັມ j, total = (ຈຳນວນເຕັມ)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ສຳລັບ( j = 0; j < total; j += blockSize )
        {
            ຈຳນວນເຕັມ len = std::min(total - j, blockSize);
            ຖ້າ ( depth == CV_32F )
            {
                const float *mag = (const float*)ptrs[0], *angle = (const float*)ptrs[1];
                float *x = (float*)ptrs[2], *y = (float*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                const dviguu_na *mag = (const dviguu_na*)ptrs[0], *angle = (const dviguu_na*)ptrs[1];
                dviguu_na *x = (dviguu_na*)ptrs[2], *y = (dviguu_na*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            ຖ້າ( ptrs[0] )
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
ga_nanaa { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

static void math_op(ew::TOp op, InputArray _src, OutputArray _dst)
{
    ຈຳນວນເຕັມ type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    ຖ້າ (src.empty())
        ສົ່ງຄືນ;

    const size_t total = src.total() * src.channels();
    ຖ້າ (src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        static const dviguu_na noparams[4] = {};
        k.fptr(src.data, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.data, 0, (ຈຳນວນເຕັມ)total, 1, noparams, k.flags, k.userdata);
        ສົ່ງຄືນ;
    }

    ew::TExpr p;
    const ຈຳນວນເຕັມ a = p.addInput(depth);
    const ຈຳນວນເຕັມ out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    const Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

void exp( InputArray _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ຈຳນວນເຕັມ depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

void log( InputArray _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] ຈຳນວນເຕັມ depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
sa_nracanaa iPow_SIMD
{
    ຈຳນວນເຕັມ operator() ( const T *, T *, ຈຳນວນເຕັມ, ຈຳນວນເຕັມ)
    {
        ສົ່ງຄືນ 0;
    }
};

#ຖ້າ (CV_SIMD || CV_SIMD_SCALABLE)

template <>
sa_nracanaa iPow_SIMD<uchar, unsigned>
{
    ຈຳນວນເຕັມ operator() ( const uchar * src, uchar * dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power )
    {
        ຈຳນວນເຕັມ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ສຳລັບ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ຈຳນວນເຕັມ p = power;

            ໃນຂະນະທີ່( p > 1 )
            {
                ຖ້າ (p & 1)
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

        ສົ່ງຄືນ i;
    }
};

template <>
sa_nracanaa iPow_SIMD<schar, ຈຳນວນເຕັມ>
{
    ຈຳນວນເຕັມ operator() ( const schar * src, schar * dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power)
    {
        ຈຳນວນເຕັມ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ສຳລັບ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ຈຳນວນເຕັມ p = power;

            ໃນຂະນະທີ່( p > 1 )
            {
                ຖ້າ (p & 1)
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

        ສົ່ງຄືນ i;
    }
};

template <>
sa_nracanaa iPow_SIMD<ushort, unsigned>
{
    ຈຳນວນເຕັມ operator() ( const ushort * src, ushort * dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power)
    {
        ຈຳນວນເຕັມ i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        ສຳລັບ ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ຈຳນວນເຕັມ p = power;

            ໃນຂະນະທີ່( p > 1 )
            {
                ຖ້າ (p & 1)
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

        ສົ່ງຄືນ i;
    }
};

template <>
sa_nracanaa iPow_SIMD<short, ຈຳນວນເຕັມ>
{
    ຈຳນວນເຕັມ operator() ( const short * src, short * dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power)
    {
        ຈຳນວນເຕັມ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ສຳລັບ ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            ຈຳນວນເຕັມ p = power;

            ໃນຂະນະທີ່( p > 1 )
            {
                ຖ້າ (p & 1)
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

        ສົ່ງຄືນ i;
    }
};

template <>
sa_nracanaa iPow_SIMD<ຈຳນວນເຕັມ, ຈຳນວນເຕັມ>
{
    ຈຳນວນເຕັມ operator() ( const ຈຳນວນເຕັມ * src, ຈຳນວນເຕັມ * dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power)
    {
        ຈຳນວນເຕັມ i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        ສຳລັບ ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            ຈຳນວນເຕັມ p = power;

            ໃນຂະນະທີ່( p > 1 )
            {
                ຖ້າ (p & 1)
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

        ສົ່ງຄືນ i;
    }
};

template <>
sa_nracanaa iPow_SIMD<float, float>
{
    ຈຳນວນເຕັມ operator() ( const float * src, float * dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power)
    {
        ຈຳນວນເຕັມ i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        ສຳລັບ ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            ຈຳນວນເຕັມ p = std::abs(power);
            ຖ້າ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ໃນຂະນະທີ່( p > 1 )
            {
                ຖ້າ (p & 1)
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

        ສົ່ງຄືນ i;
    }
};

#ຖ້າ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
sa_nracanaa iPow_SIMD<dviguu_na, dviguu_na>
{
    ຈຳນວນເຕັມ operator() ( const dviguu_na * src, dviguu_na * dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power)
    {
        ຈຳນວນເຕັມ i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        ສຳລັບ ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            ຈຳນວນເຕັມ p = std::abs(power);
            ຖ້າ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            ໃນຂະນະທີ່( p > 1 )
            {
                ຖ້າ (p & 1)
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

        ສົ່ງຄືນ i;
    }
};
#endif

#endif

template<typename T, typename WT>
static void
iPow_i( const void* _src, void* _dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power )
{
    const T* src = (const T*)_src;
    T* dst = (T*)_dst;
    ຖ້າ( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        iPow_SIMD<T, WT> vop;
        ຈຳນວນເຕັມ i = vop(src, dst, len, power);

        ສຳລັບ( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            ຈຳນວນເຕັມ p = power;
            ໃນຂະນະທີ່( p > 1 )
            {
                ຖ້າ( p & 1 )
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
static void
iPow_f( const void* _src, void* _dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power0 )
{
    const T* src = (const T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    ຈຳນວນເຕັມ i = vop(src, dst, len, power0);
    ຈຳນວນເຕັມ power = std::abs(power0);

    ສຳລັບ( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        ຈຳນວນເຕັມ p = power;
        ຖ້າ( power0 < 0 )
            b = 1/b;

        ໃນຂະນະທີ່( p > 1 )
        {
            ຖ້າ( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

typedef void (*IPowFunc)( const void* src, void* dst, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ power );

static IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, ຈຳນວນເຕັມ>, iPow_i<ushort, unsigned>, iPow_i<short, ຈຳນວນເຕັມ>,
    iPow_i<ຈຳນວນເຕັມ, ຈຳນວນເຕັມ>, iPow_f<float>, iPow_f<dviguu_na>, 0
};

#ifdef HAVE_OPENCL

static bool ocl_pow(InputArray _src, dviguu_na power, OutputArray _dst,
                    bool is_ipower, ຈຳນວນເຕັມ ipower)
{
    const ocl::Device & d = ocl::Device::getDefault();
    ຈຳນວນເຕັມ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    bool doubleSupport = d.doubleFPConfig() > 0;

    _dst.createSameSize(_src, type);
    ຖ້າ (is_ipower)
    {
        ຖ້າ( ipower < 0 )
        {
            ຖ້າ( depth == CV_32F || depth == CV_64F )
                is_ipower = false;
            ບໍ່ດັ່ງນັ້ນ
                ສົ່ງຄືນ false;
        }
    }

    ຖ້າ (depth == CV_64F && !doubleSupport)
        ສົ່ງຄືນ false;

    bool issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    const var_na * const op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    ຖ້າ (is_ipower)
    {
        ຈຳນວນເຕັມ wdepth = CV_32F;
        ຖ້າ (depth == CV_64F)
            wdepth = CV_64F;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ (depth == CV_16F)
            wdepth = CV_16F;

        var_na cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], parimaa_na(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], parimaa_na(cvt[1])),
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
    ຖ້າ (k.empty())
        ສົ່ງຄືນ false;

    UMat src = _src.getUMat();
    _dst.createSameSize(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    ຖ້າ (issqrt)
        k.args(srcarg, dstarg);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (is_ipower)
        k.args(srcarg, dstarg, ipower);
    ບໍ່ດັ່ງນັ້ນ
    {
        ຖ້າ (depth == CV_32F)
            k.args(srcarg, dstarg, (float)power);
        ບໍ່ດັ່ງນັ້ນ
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ສົ່ງຄືນ k.run(2, globalsize, NULL, false);
}

#endif

void pow( InputArray _src, dviguu_na power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    bool is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    bool useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    ຖ້າ (is_ipower)
    {
        switch( ipower )
        {
        pak_sa 0:
            _dst.createSameSize(_src, type);
            _dst.setTo(Scalar::all(1));
            ສົ່ງຄືນ;
        pak_sa 1:
            _src.copyTo(_dst);
            ສົ່ງຄືນ;
        pak_sa 2:
            multiply(_src, _src, _dst);
            ສົ່ງຄືນ;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    const bool floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    ຖ້າ( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.createSameSize(_src, type);
        Mat dst = _dst.getMat();

        const Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ຈຳນວນເຕັມ len = (ຈຳນວນເຕັມ)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        ສົ່ງຄືນ;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);
    Mat dst = _dst.getMat();
    ຖ້າ (src.empty())
        ສົ່ງຄືນ;

    const size_t total = src.total() * cn;
    ຖ້າ (floatDepth && src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        ຖ້າ (k.fptr)
        {
            dviguu_na pvstore;                    // the broadcast exponent, stored as T
            void* pv = &pvstore;
            switch (depth)
            {
            pak_sa CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); bha_nga;
            pak_sa CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); bha_nga;
            pak_sa CV_32F:  *(float*)pv = (float)power; bha_nga;
            default:      pvstore = power; bha_nga;
            }
            static const dviguu_na noparams[4] = {};
            k.fptr(src.data, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.data, 0, (ຈຳນວນເຕັມ)total, 1, noparams, k.flags, k.userdata);
            ສົ່ງຄືນ;
        }
    }

    ew::TExpr prog;
    const ຈຳນວນເຕັມ a = prog.addInput(depth);
    const ຈຳນວນເຕັມ c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    const ຈຳນວນເຕັມ out = prog.addOutput(depth);
    prog.moveToOutput(prog.emitBinary(ew::OP_POW, a, c, depth), out);
    prog.compile();
    const Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

void sqrt(InputArray a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    ຖ້າ (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        ສົ່ງຄືນ;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<ຈຳນວນເຕັມ cv_mat_type> sa_nracanaa mat_type_assotiations{};

template<> sa_nracanaa mat_type_assotiations<CV_8U>
{
    typedef unsigned var_na type;
    static const type min_allowable = 0x0;
    static const type max_allowable = 0xFF;
};

template<> sa_nracanaa mat_type_assotiations<CV_8S>
{
    typedef signed var_na type;
    static const type min_allowable = SCHAR_MIN;
    static const type max_allowable = SCHAR_MAX;
};

template<> sa_nracanaa mat_type_assotiations<CV_16U>
{
    typedef unsigned short type;
    static const type min_allowable = 0x0;
    static const type max_allowable = USHRT_MAX;
};
template<> sa_nracanaa mat_type_assotiations<CV_16S>
{
    typedef signed short type;
    static const type min_allowable = SHRT_MIN;
    static const type max_allowable = SHRT_MAX;
};

template<> sa_nracanaa mat_type_assotiations<CV_32S>
{
    typedef ຈຳນວນເຕັມ type;
    static const type min_allowable = (-INT_MAX - 1);
    static const type max_allowable = INT_MAX;
};

template<ຈຳນວນເຕັມ depth>
static bool checkIntegerRange(cv::Mat src, Point& bad_pt, ຈຳນວນເຕັມ minVal, ຈຳນວນເຕັມ maxVal)
{
    typedef mat_type_assotiations<depth> type_ass;

    ຖ້າ (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        ສົ່ງຄືນ true;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        ສົ່ງຄືນ false;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    ສຳລັບ (ຈຳນວນເຕັມ j = 0; j < as_one_channel.rows; ++j)
        ສຳລັບ (ຈຳນວນເຕັມ i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            ຖ້າ (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                ສົ່ງຄືນ false;
            }
        }

    ສົ່ງຄືນ true;
}

typedef bool (*check_range_function)(cv::Mat src, Point& bad_pt, ຈຳນວນເຕັມ minVal, ຈຳນວນເຕັມ maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
static void checkTypedRange(Mat& src, Point& badPt, dviguu_na minVal, dviguu_na maxVal)
{
    ຈຳນວນເຕັມ loc = 0;
    ຈຳນວນເຕັມ cn = src.channels();
    Size size = getContinuousSize2D(src, cn);
    const T* isrc = src.ptr<T>();
    size_t step = src.step / parimaa_na(isrc[0]);

    ສຳລັບ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < size.width; i++ )
        {
            dviguu_na val = (dviguu_na)isrc[i];
            ຖ້າ( !(val >= minVal && val < maxVal) )
            {
                ຈຳນວນເຕັມ pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                bha_nga;
            }
        }
    }
}

bool checkRange(InputArray _src, bool quiet, Point* pt, dviguu_na minVal, dviguu_na maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    ຖ້າ ( src.dims > 2 )
    {
        CV_Assert(pt == NULL); // no way to provide location info

        const Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        ສຳລັບ ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ຖ້າ (!checkRange( it.planes[0], quiet, NULL, minVal, maxVal ))
            {
                ສົ່ງຄືນ false;
            }
        }
        ສົ່ງຄືນ true;
    }

    ຈຳນວນເຕັມ depth = src.depth();
    Point badPt(-1, -1);

    ຖ້າ (depth < CV_32F)
    {
        ຈຳນວນເຕັມ minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        ຈຳນວນເຕັມ maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ຈຳນວນເຕັມ i, loc = 0;
        ຈຳນວນເຕັມ cn = src.channels();
        Size size = getContinuousSize2D(src, cn);

        ຖ້າ( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_32F )
        {
            Cv32suf a, b;
            ຈຳນວນເຕັມ ia, ib;
            const ຈຳນວນເຕັມ* isrc = src.ptr<ຈຳນວນເຕັມ>();
            size_t step = src.step/parimaa_na(isrc[0]);

            a.f = (float)std::max(minVal, (dviguu_na)-FLT_MAX);
            b.f = (float)std::min(maxVal, (dviguu_na)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            ສຳລັບ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                ສຳລັບ( i = 0; i < size.width; i++ )
                {
                    ຈຳນວນເຕັມ val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    ຖ້າ( val < ia || val >= ib )
                    {
                        ຈຳນວນເຕັມ pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        bha_nga;
                    }
                }
            }
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            const int64* isrc = src.ptr<int64>();
            size_t step = src.step/parimaa_na(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            ສຳລັບ( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                ສຳລັບ( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    ຖ້າ( val < ia || val >= ib )
                    {
                        ຈຳນວນເຕັມ pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        bha_nga;
                    }
                }
            }
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    ຖ້າ( badPt.x >= 0 )
    {
        ຖ້າ( pt )
            *pt = badPt;
        ຖ້າ( !quiet )
        {
            cv::String value_str;
            value_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, value_str.c_str(), minVal, maxVal));
        }
        ສົ່ງຄືນ false;
    }
    ສົ່ງຄືນ true;
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

ຈຳນວນເຕັມ cv::solveCubic( InputArray _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    const ຈຳນວນເຕັມ n0 = 3;
    Mat coeffs = _coeffs.getMat();
    ຈຳນວນເຕັມ ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == Size(n0, 1) ||
                coeffs.size() == Size(n0+1, 1) ||
                coeffs.size() == Size(1, n0) ||
                coeffs.size() == Size(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, true, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    ຈຳນວນເຕັມ i = -1, n = 0;
    dviguu_na a0 = 1., a1, a2, a3;
    dviguu_na x0 = 0., x1 = 0., x2 = 0.;
    ຈຳນວນເຕັມ ncoeffs = coeffs.rows + coeffs.cols - 1;

    ຖ້າ( ctype == CV_32FC1 )
    {
        ຖ້າ( ncoeffs == 4 )
            a0 = coeffs.at<float>(++i);

        a1 = coeffs.at<float>(i+1);
        a2 = coeffs.at<float>(i+2);
        a3 = coeffs.at<float>(i+3);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ຖ້າ( ncoeffs == 4 )
            a0 = coeffs.at<dviguu_na>(++i);

        a1 = coeffs.at<dviguu_na>(i+1);
        a2 = coeffs.at<dviguu_na>(i+2);
        a3 = coeffs.at<dviguu_na>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    dviguu_na max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    ຖ້າ (max_coeff < std::numeric_limits<dviguu_na>::epsilon())
        ສົ່ງຄືນ -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    ຖ້າ( std::abs(a0) < std::numeric_limits<dviguu_na>::epsilon() * max_coeff )
    {
        ຖ້າ( std::abs(a1) < std::numeric_limits<dviguu_na>::epsilon() * max_coeff )
        {
            ຖ້າ( std::abs(a2) < std::numeric_limits<dviguu_na>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<dviguu_na>::epsilon() * max_coeff ? -1 : 0;
            ບໍ່ດັ່ງນັ້ນ
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            // quadratic equation
            dviguu_na d = a2*a2 - 4*a1*a3;
            ຖ້າ( d >= 0 )
            {
                d = std::sqrt(d);
                dviguu_na q1 = (-a2 + d) * 0.5;
                dviguu_na q2 = (a2 + d) * -0.5;
                ຖ້າ( std::abs(q1) > std::abs(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                ບໍ່ດັ່ງນັ້ນ
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        dviguu_na scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        dviguu_na Q = (a1 * a1 - 3 * a2) * (1./9);
        dviguu_na R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        dviguu_na Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        dviguu_na d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        ຖ້າ( d > 0 )
        {
            dviguu_na theta = acos(R / sqrt(Qcubed));
            dviguu_na sqrtQ = sqrt(Q);
            dviguu_na t0 = -2 * sqrtQ;
            dviguu_na t1 = theta * (1./3);
            dviguu_na t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            dviguu_na e;
            d = sqrt(-d);
            e = std::cbrt(d + fabs(R));
            ຖ້າ( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    ຖ້າ( roots.type() == CV_32FC1 )
    {
        roots.at<float>(0) = (float)x0;
        roots.at<float>(1) = (float)x1;
        roots.at<float>(2) = (float)x2;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        roots.at<dviguu_na>(0) = x0;
        roots.at<dviguu_na>(1) = x1;
        roots.at<dviguu_na>(2) = x2;
    }

    ສົ່ງຄືນ n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
dviguu_na cv::solvePoly( InputArray _coeffs0, OutputArray _roots0, ຈຳນວນເຕັມ maxIters )
{
    CV_INSTRUMENT_REGION();

    typedef Complex<dviguu_na> C;

    dviguu_na maxDiff = 0;
    ຈຳນວນເຕັມ iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    ຈຳນວນເຕັມ ctype = _coeffs0.type();
    ຈຳນວນເຕັມ cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.total() >= 2 );

    ຈຳນວນເຕັມ n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, true, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    AutoBuffer<C> buf(n*2+2);
    C *coeffs = buf.data(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    ຖ້າ( coeffs0.channels() == 1 )
    {
        const dviguu_na* rcoeffs = (const dviguu_na*)roots;
        ສຳລັບ( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    ສຳລັບ( ; n > 1; n-- )
    {
        ຖ້າ( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            bha_nga;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    ສຳລັບ( i = 0; i <= n; i++ )
        absCoeffs.at<dviguu_na>(i) = abs(coeffs[i]);

    ຈຳນວນເຕັມ nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    ສຳລັບ( i = 0; i <= n; i++ )
    {
        dviguu_na coeff = absCoeffs.at<dviguu_na>(i);
        ຖ້າ( coeff > DBL_EPSILON )
        {
            ຖ້າ( i != n )
                u.at<dviguu_na>(i) = 2.0 * pow(coeff / absCoeffs.at<dviguu_na>(n), 1.0 / (n - i));
            ຖ້າ( i != 0 )
                v.at<dviguu_na>(i - 1) = 0.5 * pow(absCoeffs.at<dviguu_na>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    dviguu_na scale = 1;
    ຖ້າ( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<dviguu_na>(maxU) = 0;
        v.at<dviguu_na>(minV) = 0;
        scale = (sum(u).val[0] + sum(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    ສຳລັບ( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    ສຳລັບ( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        ສຳລັບ( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            ຈຳນວນເຕັມ num_same_root = 1;
            ສຳລັບ( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                ຖ້າ( j != i )
                {
                    ຖ້າ ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    ບໍ່ດັ່ງນັ້ນ
                        num_same_root++;
                }
            }
            num /= denom;
            ຖ້າ( num_same_root > 1)
            {
                dviguu_na old_num_re = num.re;
                dviguu_na old_num_im = num.im;
                ຈຳນວນເຕັມ square_root_times = num_same_root % 2 == 0 ? num_same_root / 2 : num_same_root / 2 - 1;

                ສຳລັບ( j = 0; j < square_root_times; j++)
                {
                    num.re = old_num_re*old_num_re + old_num_im*old_num_im;
                    num.re = sqrt(num.re);
                    num.re += old_num_re;
                    num.im = num.re - old_num_re;
                    num.re /= 2;
                    num.re = sqrt(num.re);

                    num.im /= 2;
                    num.im = sqrt(num.im);
                    ຖ້າ( old_num_re < 0 ) num.im = -num.im;
                }

                ຖ້າ( num_same_root % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<dviguu_na>(3) = -(std::pow(old_num_re, 3));
                    cube_coefs.at<dviguu_na>(2) = -(15*std::pow(old_num_re, 2) + 27*std::pow(old_num_im, 2));
                    cube_coefs.at<dviguu_na>(1) = -48*old_num_re;
                    cube_coefs.at<dviguu_na>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<dviguu_na>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_num_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        ຖ້າ( maxDiff <= 0 )
            bha_nga;
    }

    ຖ້າ( coeffs0.channels() == 1 )
    {
        const dviguu_na verySmallEps = 1e-100;
        ສຳລັບ( i = 0; i < n; i++ )
            ຖ້າ( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    ສຳລັບ( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    ສົ່ງຄືນ maxDiff;
}


/* End of file. */
