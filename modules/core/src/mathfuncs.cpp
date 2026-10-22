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

#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل <atomic>
#شامل <limits>
#شامل <iostream>
#شامل <algorithm>
#شامل <cmath>
#شامل "mathfuncs.hpp"
#شامل "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

فضاینام cv
{

تعریفنوع تهی (*MathFunc)(ثابت تهی* src, تهی* dst, صحیح len);

#ifdef HAVE_OPENCL

شمارش { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

ایستا ثابت نویسه* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

ایستا منطقی ocl_math_op(درون_داد_Array _src1, درون_داد_Array _src2, OutputArray _dst, صحیح oclop)
{
    صحیح type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    صحیح kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predictOptimalVectorWidth(_src1, _src2, _dst);

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقی double_support = d.doubleFPConfig() > 0;
    اگر (!double_support && depth == CV_64F)
        بازگشت نادرست;
    صحیح rowsPerWI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth, rowsPerWI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    اگر (k.empty())
        بازگشت نادرست;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.createSameSize(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    اگر (src2.empty())
        k.args(src1arg, dstarg);
    وگرنه
        k.args(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.cols * cn / kercn, ((size_t)src1.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, 0, نادرست);
}

#endif

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN اعشاری cube_ریشه( اعشاری value )
{
    CV_INSTRUMENT_REGION();

    اعشاری fr;
    Cv32suf v, m;
    صحیح ix, s;
    صحیح ex, shx;

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
    fr = (اعشاری)(((((45.2548339756803022511987494 * fr +
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
    بازگشت v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

تهی magnitude( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    صحیح type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح len = (صحیح)it.size*cn;

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        اگر( depth == CV_32F )
        {
            ثابت اعشاری *x = (ثابت اعشاری*)ptrs[0], *y = (ثابت اعشاری*)ptrs[1];
            اعشاری *mag = (اعشاری*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        وگرنه
        {
            ثابت دوگانه *x = (ثابت دوگانه*)ptrs[0], *y = (ثابت دوگانه*)ptrs[1];
            دوگانه *mag = (دوگانه*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

تهی phase( درون_داد_Array src1, درون_داد_Array src2, OutputArray dst, منطقی angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    صحیح type = src1.type(), depth = src1.depth(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (depth == CV_32F || depth == CV_64F));

    CV_OCL_RUN(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angleInDegrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح j, total = (صحیح)(it.size*cn), blockSize = total;
    size_t esz1 = X.elemSize1();
    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( j = 0; j < total; j += blockSize )
        {
            صحیح len = std::min(total - j, blockSize);
            اگر( depth == CV_32F )
            {
                ثابت اعشاری *x = (ثابت اعشاری*)ptrs[0], *y = (ثابت اعشاری*)ptrs[1];
                اعشاری *angle = (اعشاری*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angleInDegrees );
            }
            وگرنه
            {
                ثابت دوگانه *x = (ثابت دوگانه*)ptrs[0], *y = (ثابت دوگانه*)ptrs[1];
                دوگانه *angle = (دوگانه*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angleInDegrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#ifdef HAVE_OPENCL

ایستا منطقی ocl_cartToPolar( درون_داد_Array _src1, درون_داد_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, منطقی angleInDegrees )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحیح type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    منطقی doubleSupport = d.doubleFPConfig() > 0;

    ثابت منطقی _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ثابت منطقی _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ثابت منطقی _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ثابت منطقی _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    اگر ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (depth == CV_32F || depth == CV_64F) && type == _src2.type()) ||
         (depth == CV_64F && !doubleSupport) )
        بازگشت نادرست;

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
    اگر (k.empty())
        بازگشت نادرست;

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
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

تهی cartToPolar( درون_داد_Array src1, درون_داد_Array src2,
                  OutputArray dst1, OutputArray dst2, منطقی angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_RUN(dst1.isUMat() && dst2.isUMat(),
            ocl_cartToPolar(src1, src2, dst1, dst2, angleInDegrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    صحیح type = X.type(), depth = X.depth(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (depth == CV_32F || depth == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح j, total = (صحیح)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( j = 0; j < total; j += blockSize )
        {
            صحیح len = std::min(total - j, blockSize);
            اگر( depth == CV_32F )
            {
                ثابت اعشاری *x = (ثابت اعشاری*)ptrs[0], *y = (ثابت اعشاری*)ptrs[1];
                اعشاری *mag = (اعشاری*)ptrs[2], *angle = (اعشاری*)ptrs[3];
                hal::cartToPolar32f( x, y, mag, angle, len, angleInDegrees );
            }
            وگرنه
            {
                ثابت دوگانه *x = (ثابت دوگانه*)ptrs[0], *y = (ثابت دوگانه*)ptrs[1];
                دوگانه *mag = (دوگانه*)ptrs[2], *angle = (دوگانه*)ptrs[3];
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

ایستا منطقی ocl_polarToCart( درون_داد_Array _mag, درون_داد_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, منطقی angleInDegrees )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحیح type = _angle.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    منطقی doubleSupport = d.doubleFPConfig() > 0;

    ثابت منطقی _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ثابت منطقی _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ثابت منطقی _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ثابت منطقی _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    اگر ( !doubleSupport && depth == CV_64F )
        بازگشت نادرست;

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
    اگر (k.empty())
        بازگشت نادرست;

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
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

تهی polarToCart( درون_داد_Array src1, درون_داد_Array src2,
                  OutputArray dst1, OutputArray dst2, منطقی angleInDegrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    صحیح type = src2.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert((depth == CV_32F || depth == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_RUN(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polarToCart(src1, src2, dst1, dst2, angleInDegrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ثابت Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح j, total = (صحیح)(it.size*cn), blockSize = std::min(total, ((BLOCK_SIZE+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( j = 0; j < total; j += blockSize )
        {
            صحیح len = std::min(total - j, blockSize);
            اگر ( depth == CV_32F )
            {
                ثابت اعشاری *mag = (ثابت اعشاری*)ptrs[0], *angle = (ثابت اعشاری*)ptrs[1];
                اعشاری *x = (اعشاری*)ptrs[2], *y = (اعشاری*)ptrs[3];
                hal::polarToCart32f( mag, angle, x, y, len, angleInDegrees );
            }
            وگرنه
            {
                ثابت دوگانه *mag = (ثابت دوگانه*)ptrs[0], *angle = (ثابت دوگانه*)ptrs[1];
                دوگانه *x = (دوگانه*)ptrs[2], *y = (دوگانه*)ptrs[3];
                hal::polarToCart64f( mag, angle, x, y, len, angleInDegrees );
            }

            اگر( ptrs[0] )
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
شمارش { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

ایستا تهی math_op(ew::بالاترین op, درون_داد_Array _src, OutputArray _dst)
{
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type);
    CV_Assert(depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F);

    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    اگر (src.empty())
        بازگشت;

    ثابت size_t total = src.total() * src.channels();
    اگر (src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, depth);
        CV_Assert(k.fptr);
        ایستا ثابت دوگانه noparams[4] = {};
        k.fptr(src.data, 0, 1, nullptr, 0, 0, nullptr, 0, 0,
               dst.data, 0, (صحیح)total, 1, noparams, k.flags, k.userdata);
        بازگشت;
    }

    ew::TExpr p;
    ثابت صحیح a = p.add_درون_داد(depth);
    ثابت صحیح out = p.addOutput(depth);
    p.moveToOutput(p.emitUnary(op, a, depth), out);
    p.compile();
    ثابت Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

تهی exp( درون_داد_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] صحیح depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

تهی log( درون_داد_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] صحیح depth = _src.depth();   // consumed by CV_OCL_RUN only

    CV_OCL_RUN( _dst.isUMat() && _src.dims() <= 2 && (depth == CV_32F || depth == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template <typename T, typename WT>
ساختار iPow_SIMD
{
    صحیح operator() ( ثابت T *, T *, صحیح, صحیح)
    {
        بازگشت 0;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

template <>
ساختار iPow_SIMD<uchar, unsigned>
{
    صحیح operator() ( ثابت uchar * src, uchar * dst, صحیح len, صحیح power )
    {
        صحیح i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        برای ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحیح p = power;

            وقتی( p > 1 )
            {
                اگر (p & 1)
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

        بازگشت i;
    }
};

template <>
ساختار iPow_SIMD<schar, صحیح>
{
    صحیح operator() ( ثابت schar * src, schar * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        برای ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحیح p = power;

            وقتی( p > 1 )
            {
                اگر (p & 1)
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

        بازگشت i;
    }
};

template <>
ساختار iPow_SIMD<ushort, unsigned>
{
    صحیح operator() ( ثابت ushort * src, ushort * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        برای ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_load(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحیح p = power;

            وقتی( p > 1 )
            {
                اگر (p & 1)
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

        بازگشت i;
    }
};

template <>
ساختار iPow_SIMD<short, صحیح>
{
    صحیح operator() ( ثابت short * src, short * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        برای ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_load(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحیح p = power;

            وقتی( p > 1 )
            {
                اگر (p & 1)
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

        بازگشت i;
    }
};

template <>
ساختار iPow_SIMD<صحیح, صحیح>
{
    صحیح operator() ( ثابت صحیح * src, صحیح * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        برای ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_int32>::vlanes());
            صحیح p = power;

            وقتی( p > 1 )
            {
                اگر (p & 1)
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

        بازگشت i;
    }
};

template <>
ساختار iPow_SIMD<اعشاری, اعشاری>
{
    صحیح operator() ( ثابت اعشاری * src, اعشاری * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        برای ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float32>::vlanes());
            صحیح p = std::abs(power);
            اگر( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            وقتی( p > 1 )
            {
                اگر (p & 1)
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

        بازگشت i;
    }
};

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
template <>
ساختار iPow_SIMD<دوگانه, دوگانه>
{
    صحیح operator() ( ثابت دوگانه * src, دوگانه * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        برای ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_load(src + i), v_b2 = vx_load(src + i + VTraits<v_float64>::vlanes());
            صحیح p = std::abs(power);
            اگر( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            وقتی( p > 1 )
            {
                اگر (p & 1)
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

        بازگشت i;
    }
};
#endif

#endif

template<typename T, typename WT>
ایستا تهی
iPow_i( ثابت تهی* _src, تهی* _dst, صحیح len, صحیح power )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    اگر( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        برای( صحیح i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    وگرنه
    {
        iPow_SIMD<T, WT> vop;
        صحیح i = vop(src, dst, len, power);

        برای( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            صحیح p = power;
            وقتی( p > 1 )
            {
                اگر( p & 1 )
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
ایستا تهی
iPow_f( ثابت تهی* _src, تهی* _dst, صحیح len, صحیح power0 )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    صحیح i = vop(src, dst, len, power0);
    صحیح power = std::abs(power0);

    برای( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        صحیح p = power;
        اگر( power0 < 0 )
            b = 1/b;

        وقتی( p > 1 )
        {
            اگر( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

تعریفنوع تهی (*IPowFunc)( ثابت تهی* src, تهی* dst, صحیح len, صحیح power );

ایستا IPowFunc ipowTab[CV_DEPTH_MAX] =
{
    iPow_i<uchar, unsigned>, iPow_i<schar, صحیح>, iPow_i<ushort, unsigned>, iPow_i<short, صحیح>,
    iPow_i<صحیح, صحیح>, iPow_f<اعشاری>, iPow_f<دوگانه>, 0
};

#ifdef HAVE_OPENCL

ایستا منطقی ocl_pow(درون_داد_Array _src, دوگانه power, OutputArray _dst,
                    منطقی is_ipower, صحیح ipower)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = d.isIntel() ? 4 : 1;
    منطقی doubleSupport = d.doubleFPConfig() > 0;

    _dst.createSameSize(_src, type);
    اگر (is_ipower)
    {
        اگر( ipower < 0 )
        {
            اگر( depth == CV_32F || depth == CV_64F )
                is_ipower = نادرست;
            وگرنه
                بازگشت نادرست;
        }
    }

    اگر (depth == CV_64F && !doubleSupport)
        بازگشت نادرست;

    منطقی issqrt = std::abs(power - 0.5) < DBL_EPSILON;
    ثابت نویسه * ثابت op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    اگر (is_ipower)
    {
        صحیح wdepth = CV_32F;
        اگر (depth == CV_64F)
            wdepth = CV_64F;
        وگرنه اگر (depth == CV_16F)
            wdepth = CV_16F;

        نویسه cvt[2][50];
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
            ocl::convertTypeStr(depth, wdepth, 1, cvt[0], اندازه(cvt[0])),
            ocl::convertTypeStr(wdepth, depth, 1, cvt[1], اندازه(cvt[1])),
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
    اگر (k.empty())
        بازگشت نادرست;

    UMat src = _src.getUMat();
    _dst.createSameSize(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    اگر (issqrt)
        k.args(srcarg, dstarg);
    وگرنه اگر (is_ipower)
        k.args(srcarg, dstarg, ipower);
    وگرنه
    {
        اگر (depth == CV_32F)
            k.args(srcarg, dstarg, (اعشاری)power);
        وگرنه
            k.args(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.cols *  cn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

تهی pow( درون_داد_Array _src, دوگانه power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type),
            cn = CV_MAT_CN(type), ipower = cvRound(power);
    منطقی is_ipower = fabs(ipower - power) < DBL_EPSILON;
#ifdef HAVE_OPENCL
    منطقی useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#endif

    اگر (is_ipower)
    {
        انتخاب( ipower )
        {
        حالت 0:
            _dst.createSameSize(_src, type);
            _dst.setTo(Scalar::all(1));
            بازگشت;
        حالت 1:
            _src.copyTo(_dst);
            بازگشت;
        حالت 2:
            multiply(_src, _src, _dst);
            بازگشت;
        }
    }

    CV_OCL_RUN(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ثابت منطقی floatDepth = depth == CV_16F || depth == CV_16BF || depth == CV_32F || depth == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    اگر( is_ipower && !floatDepth && ipowTab[depth] )
    {
        Mat src = _src.getMat();
        _dst.createSameSize(_src, type);
        Mat dst = _dst.getMat();

        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحیح len = (صحیح)(it.size*cn);
        IPowFunc func = ipowTab[depth];

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        بازگشت;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.createSameSize(_src, type);
    Mat dst = _dst.getMat();
    اگر (src.empty())
        بازگشت;

    ثابت size_t total = src.total() * cn;
    اگر (floatDepth && src.isContinuous() && dst.isContinuous() && total <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(depth, depth);
        اگر (k.fptr)
        {
            دوگانه pvstore;                    // the broadcast exponent, stored as T
            تهی* pv = &pvstore;
            انتخاب (depth)
            {
            حالت CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); بشکن;
            حالت CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); بشکن;
            حالت CV_32F:  *(اعشاری*)pv = (اعشاری)power; بشکن;
            پیشفرض:      pvstore = power; بشکن;
            }
            ایستا ثابت دوگانه noparams[4] = {};
            k.fptr(src.data, 0, 1, pv, 0, 0, nullptr, 0, 0,
                   dst.data, 0, (صحیح)total, 1, noparams, k.flags, k.userdata);
            بازگشت;
        }
    }

    ew::TExpr prog;
    ثابت صحیح a = prog.add_درون_داد(depth);
    ثابت صحیح c = prog.addConst(ew::EW_DEPTH_NONE, Scalar(power), 1);
    ثابت صحیح out = prog.addOutput(depth);
    prog.moveToOutput(prog.emit_دودویی(ew::OP_POW, a, c, depth), out);
    prog.compile();
    ثابت Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

تهی sqrt(درون_داد_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    اگر (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::pow(a, 0.5, b);
        بازگشت;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

template<صحیح cv_mat_type> ساختار mat_type_assotiations{};

template<> ساختار mat_type_assotiations<CV_8U>
{
    تعریفنوع unsigned نویسه type;
    ایستا ثابت type min_allowable = 0x0;
    ایستا ثابت type max_allowable = 0xFF;
};

template<> ساختار mat_type_assotiations<CV_8S>
{
    تعریفنوع signed نویسه type;
    ایستا ثابت type min_allowable = SCHAR_MIN;
    ایستا ثابت type max_allowable = SCHAR_MAX;
};

template<> ساختار mat_type_assotiations<CV_16U>
{
    تعریفنوع unsigned short type;
    ایستا ثابت type min_allowable = 0x0;
    ایستا ثابت type max_allowable = USHRT_MAX;
};
template<> ساختار mat_type_assotiations<CV_16S>
{
    تعریفنوع signed short type;
    ایستا ثابت type min_allowable = SHRT_MIN;
    ایستا ثابت type max_allowable = SHRT_MAX;
};

template<> ساختار mat_type_assotiations<CV_32S>
{
    تعریفنوع صحیح type;
    ایستا ثابت type min_allowable = (-INT_MAX - 1);
    ایستا ثابت type max_allowable = INT_MAX;
};

template<صحیح depth>
ایستا منطقی checkIntegerRange(cv::Mat src, Point& bad_pt, صحیح minVal, صحیح maxVal)
{
    تعریفنوع mat_type_assotiations<depth> type_ass;

    اگر (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        بازگشت درست;
    }
    وگرنه اگر (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::Point(0,0);
        بازگشت نادرست;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    برای (صحیح j = 0; j < as_one_channel.rows; ++j)
        برای (صحیح i = 0; i < as_one_channel.cols; ++i)
        {
            typename type_ass::type v = as_one_channel.at<typename type_ass::type>(j ,i);
            اگر (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                بازگشت نادرست;
            }
        }

    بازگشت درست;
}

تعریفنوع منطقی (*check_range_function)(cv::Mat src, Point& bad_pt, صحیح minVal, صحیح maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

template<typename T>
ایستا تهی checkTypedRange(Mat& src, Point& badPt, دوگانه minVal, دوگانه maxVal)
{
    صحیح loc = 0;
    صحیح cn = src.channels();
    Size size = getContinuousSize2D(src, cn);
    ثابت T* isrc = src.ptr<T>();
    size_t step = src.step / اندازه(isrc[0]);

    برای( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
    {
        برای( صحیح i = 0; i < size.width; i++ )
        {
            دوگانه val = (دوگانه)isrc[i];
            اگر( !(val >= minVal && val < maxVal) )
            {
                صحیح pixelId = (loc + i) / cn;
                badPt = Point(pixelId % src.cols, pixelId / src.cols);
                بشکن;
            }
        }
    }
}

منطقی checkRange(درون_داد_Array _src, منطقی quiet, Point* pt, دوگانه minVal, دوگانه maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    اگر ( src.dims > 2 )
    {
        CV_Assert(pt == پوچ); // no way to provide location info

        ثابت Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        برای ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر (!checkRange( it.planes[0], quiet, پوچ, minVal, maxVal ))
            {
                بازگشت نادرست;
            }
        }
        بازگشت درست;
    }

    صحیح depth = src.depth();
    Point badPt(-1, -1);

    اگر (depth < CV_32F)
    {
        صحیح minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        صحیح maxVali = maxVal > INT_MAX ? INT_MAX : cvCeil(maxVal) - 1;

        (check_range_functions[depth])(src, badPt, minVali, maxVali);
    }
    وگرنه
    {
        صحیح i, loc = 0;
        صحیح cn = src.channels();
        Size size = getContinuousSize2D(src, cn);

        اگر( depth == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        وگرنه اگر( depth == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        وگرنه اگر( depth == CV_32F )
        {
            Cv32suf a, b;
            صحیح ia, ib;
            ثابت صحیح* isrc = src.ptr<صحیح>();
            size_t step = src.step/اندازه(isrc[0]);

            a.f = (اعشاری)std::max(minVal, (دوگانه)-FLT_MAX);
            b.f = (اعشاری)std::min(maxVal, (دوگانه)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            برای( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                برای( i = 0; i < size.width; i++ )
                {
                    صحیح val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    اگر( val < ia || val >= ib )
                    {
                        صحیح pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        بشکن;
                    }
                }
            }
        }
        وگرنه اگر( depth == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ثابت int64* isrc = src.ptr<int64>();
            size_t step = src.step/اندازه(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            برای( ; badPt.x < 0 && size.height--; loc += size.width, isrc += step )
            {
                برای( i = 0; i < size.width; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    اگر( val < ia || val >= ib )
                    {
                        صحیح pixelId = (loc + i) / cn;
                        badPt = Point(pixelId % src.cols, pixelId / src.cols);
                        بشکن;
                    }
                }
            }
        }
        وگرنه اگر( depth == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        وگرنه اگر( depth == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        وگرنه اگر( depth == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        وگرنه اگر( depth == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        وگرنه
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    اگر( badPt.x >= 0 )
    {
        اگر( pt )
            *pt = badPt;
        اگر( !quiet )
        {
            cv::String مقدار_str;
            مقدار_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::StsOutOfRange,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, مقدار_str.c_str(), minVal, maxVal));
        }
        بازگشت نادرست;
    }
    بازگشت درست;
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

صحیح cv::solveCubic( درون_داد_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح n0 = 3;
    Mat coeffs = _coeffs.getMat();
    صحیح ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == Size(n0, 1) ||
                coeffs.size() == Size(n0+1, 1) ||
                coeffs.size() == Size(1, n0) ||
                coeffs.size() == Size(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, درست, _OutputArray::DEPTH_MASK_FLT);
    Mat roots = _roots.getMat();

    صحیح i = -1, n = 0;
    دوگانه a0 = 1., a1, a2, a3;
    دوگانه x0 = 0., x1 = 0., x2 = 0.;
    صحیح ncoeffs = coeffs.rows + coeffs.cols - 1;

    اگر( ctype == CV_32FC1 )
    {
        اگر( ncoeffs == 4 )
            a0 = coeffs.at<اعشاری>(++i);

        a1 = coeffs.at<اعشاری>(i+1);
        a2 = coeffs.at<اعشاری>(i+2);
        a3 = coeffs.at<اعشاری>(i+3);
    }
    وگرنه
    {
        اگر( ncoeffs == 4 )
            a0 = coeffs.at<دوگانه>(++i);

        a1 = coeffs.at<دوگانه>(i+1);
        a2 = coeffs.at<دوگانه>(i+2);
        a3 = coeffs.at<دوگانه>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    دوگانه max_coeff = std::max({std::abs(a0), std::abs(a1), std::abs(a2), std::abs(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    اگر (max_coeff < std::numeric_limits<دوگانه>::epsilon())
        بازگشت -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    اگر( std::abs(a0) < std::numeric_limits<دوگانه>::epsilon() * max_coeff )
    {
        اگر( std::abs(a1) < std::numeric_limits<دوگانه>::epsilon() * max_coeff )
        {
            اگر( std::abs(a2) < std::numeric_limits<دوگانه>::epsilon() * max_coeff )
                n = std::abs(a3) < std::numeric_limits<دوگانه>::epsilon() * max_coeff ? -1 : 0;
            وگرنه
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        وگرنه
        {
            // quadratic equation
            دوگانه d = a2*a2 - 4*a1*a3;
            اگر( d >= 0 )
            {
                d = std::sqrt(d);
                دوگانه q1 = (-a2 + d) * 0.5;
                دوگانه q2 = (a2 + d) * -0.5;
                اگر( std::abs(q1) > std::abs(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                وگرنه
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    وگرنه
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        دوگانه scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        دوگانه Q = (a1 * a1 - 3 * a2) * (1./9);
        دوگانه R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        دوگانه Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        دوگانه d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        اگر( d > 0 )
        {
            دوگانه theta = acos(R / sqrt(Qcubed));
            دوگانه sqrtQ = sqrt(Q);
            دوگانه t0 = -2 * sqrtQ;
            دوگانه t1 = theta * (1./3);
            دوگانه t2 = a1 * (1./3);
            x0 = t0 * cos(t1) - t2;
            x1 = t0 * cos(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * cos(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        وگرنه اگر( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        وگرنه
        {
            دوگانه e;
            d = sqrt(-d);
            e = std::cbrt(d + fabs(R));
            اگر( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    اگر( roots.type() == CV_32FC1 )
    {
        roots.at<اعشاری>(0) = (اعشاری)x0;
        roots.at<اعشاری>(1) = (اعشاری)x1;
        roots.at<اعشاری>(2) = (اعشاری)x2;
    }
    وگرنه
    {
        roots.at<دوگانه>(0) = x0;
        roots.at<دوگانه>(1) = x1;
        roots.at<دوگانه>(2) = x2;
    }

    بازگشت n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
دوگانه cv::solvePoly( درون_داد_Array _coeffs0, OutputArray _roots0, صحیح maxIters )
{
    CV_INSTRUMENT_REGION();

    تعریفنوع Complex<دوگانه> C;

    دوگانه maxDiff = 0;
    صحیح iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    صحیح ctype = _coeffs0.type();
    صحیح cdepth = CV_MAT_DEPTH(ctype);

    CV_Assert( CV_MAT_DEPTH(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.rows == 1 || coeffs0.cols == 1) && coeffs0.total() >= 2 );

    صحیح n0 = coeffs0.cols + coeffs0.rows - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, درست, _OutputArray::DEPTH_MASK_FLT);
    Mat roots0 = _roots0.getMat();

    AutoBuffer<C> buf(n*2+2);
    C *coeffs = buf.data(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convertTo(coeffs1, coeffs1.type());
    اگر( coeffs0.channels() == 1 )
    {
        ثابت دوگانه* rcoeffs = (ثابت دوگانه*)roots;
        برای( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    برای( ; n > 1; n-- )
    {
        اگر( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON )
            بشکن;
    }
    CV_Assert( std::abs(coeffs[n].re) + std::abs(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    برای( i = 0; i <= n; i++ )
        absCoeffs.at<دوگانه>(i) = abs(coeffs[i]);

    صحیح nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    برای( i = 0; i <= n; i++ )
    {
        دوگانه coeff = absCoeffs.at<دوگانه>(i);
        اگر( coeff > DBL_EPSILON )
        {
            اگر( i != n )
                u.at<دوگانه>(i) = 2.0 * pow(coeff / absCoeffs.at<دوگانه>(n), 1.0 / (n - i));
            اگر( i != 0 )
                v.at<دوگانه>(i - 1) = 0.5 * pow(absCoeffs.at<دوگانه>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    دوگانه scale = 1;
    اگر( nonZeroCoeffs > 2 )
    {
        Point maxU, minV;
        minMaxLoc(u, nullptr, nullptr, nullptr, &maxU);
        minMaxLoc(v, nullptr, nullptr, &minV);
        u.at<دوگانه>(maxU) = 0;
        v.at<دوگانه>(minV) = 0;
        scale = (sum(u).val[0] + sum(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(cos(CV_2PI / n), sin(CV_2PI / n));

    برای( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    برای( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        برای( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            صحیح رقم_same_ریشه = 1;
            برای( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                اگر( j != i )
                {
                    اگر ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    وگرنه
                        رقم_same_ریشه++;
                }
            }
            num /= denom;
            اگر( رقم_same_ریشه > 1)
            {
                دوگانه old_رقم_re = num.re;
                دوگانه old_رقم_im = num.im;
                صحیح square_ریشه_times = رقم_same_ریشه % 2 == 0 ? رقم_same_ریشه / 2 : رقم_same_ریشه / 2 - 1;

                برای( j = 0; j < square_ریشه_times; j++)
                {
                    num.re = old_رقم_re*old_رقم_re + old_رقم_im*old_رقم_im;
                    num.re = sqrt(num.re);
                    num.re += old_رقم_re;
                    num.im = num.re - old_رقم_re;
                    num.re /= 2;
                    num.re = sqrt(num.re);

                    num.im /= 2;
                    num.im = sqrt(num.im);
                    اگر( old_رقم_re < 0 ) num.im = -num.im;
                }

                اگر( رقم_same_ریشه % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<دوگانه>(3) = -(std::pow(old_رقم_re, 3));
                    cube_coefs.at<دوگانه>(2) = -(15*std::pow(old_رقم_re, 2) + 27*std::pow(old_رقم_im, 2));
                    cube_coefs.at<دوگانه>(1) = -48*old_رقم_re;
                    cube_coefs.at<دوگانه>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<دوگانه>(0));
                    num.im = sqrt(std::pow(num.re, 2) / 3 - old_رقم_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::abs(num));
        }
        اگر( maxDiff <= 0 )
            بشکن;
    }

    اگر( coeffs0.channels() == 1 )
    {
        ثابت دوگانه verySmallEps = 1e-100;
        برای( i = 0; i < n; i++ )
            اگر( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    برای( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convertTo(roots0, roots0.type());
    بازگشت maxDiff;
}


/* End of file. */
