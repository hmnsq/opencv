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

نامخلا cv
{

قسمتعریف خالی (*MathFunc)(ثابت خالی* src, خالی* dst, صحیح len);

#اگر_تعریف_شدہ HAVE_OPENCL

شمار { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

ساکن ثابت حرف* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

ساکن منطقی ocl_math_op(داخلہ_Array _src1, داخلہ_Array _src2, OutputArray _dst, صحیح oclop)
{
    صحیح type = _src1.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type);
    صحیح kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predict_Optimal_سمتیہ_چوڑائی(_src1, _src2, _dst);

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقی double_support = d.doubleFPConfig() > 0;
    اگر (!double_support && گہرائی == CV_64F)
        واپس جھوٹ;
    صحیح صفیں_فی_WI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)), گہرائی, صفیں_فی_WI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.create_Same_سائز(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_سائز(src1),
            src2arg = ocl::KernelArg::Read_Only_No_سائز(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    اگر (src2.empty())
        k.دلائل(src1arg, dstarg);
    ورنہ
        k.دلائل(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.ستون_تعداد * cn / kercn, ((size_t)src1.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, 0, جھوٹ);
}

#اگر_ختم

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN اعشاری cube_جڑ( اعشاری value )
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
    واپس v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

خالی magnitude( داخلہ_Array src1, داخلہ_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    صحیح type = src1.type(), گہرائی = src1.گہرائی(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (گہرائی == CV_32F || گہرائی == CV_64F));

    CV_OCL_تسلسل(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح len = (صحیح)it.size*cn;

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        اگر( گہرائی == CV_32F )
        {
            ثابت اعشاری *x = (ثابت اعشاری*)ptrs[0], *y = (ثابت اعشاری*)ptrs[1];
            اعشاری *mag = (اعشاری*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        ورنہ
        {
            ثابت دوہرا *x = (ثابت دوہرا*)ptrs[0], *y = (ثابت دوہرا*)ptrs[1];
            دوہرا *mag = (دوہرا*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

خالی phase( داخلہ_Array src1, داخلہ_Array src2, OutputArray dst, منطقی angle_اندر_Degrees )
{
    CV_INSTRUMENT_REGION();

    صحیح type = src1.type(), گہرائی = src1.گہرائی(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (گہرائی == CV_32F || گہرائی == CV_64F));

    CV_OCL_تسلسل(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angle_اندر_Degrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح j, کل = (صحیح)(it.size*cn), بلاک_سائز_2 = کل;
    size_t esz1 = X.elemSize1();
    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( j = 0; j < کل; j += بلاک_سائز_2 )
        {
            صحیح len = std::min(کل - j, بلاک_سائز_2);
            اگر( گہرائی == CV_32F )
            {
                ثابت اعشاری *x = (ثابت اعشاری*)ptrs[0], *y = (ثابت اعشاری*)ptrs[1];
                اعشاری *angle = (اعشاری*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angle_اندر_Degrees );
            }
            ورنہ
            {
                ثابت دوہرا *x = (ثابت دوہرا*)ptrs[0], *y = (ثابت دوہرا*)ptrs[1];
                دوہرا *angle = (دوہرا*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angle_اندر_Degrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_cart_مقصد_Polar( داخلہ_Array _src1, داخلہ_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, منطقی angle_اندر_Degrees )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحیح type = _src1.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type),
            صفیں_فی_WI = d.isIntel() ? 4 : 1;
    منطقی doubleSupport = d.doubleFPConfig() > 0;

    ثابت منطقی _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ثابت منطقی _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ثابت منطقی _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ثابت منطقی _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    اگر ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (گہرائی == CV_32F || گہرائی == CV_64F) && type == _src2.type()) ||
         (گہرائی == CV_64F && !doubleSupport) )
        واپس جھوٹ;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D OP_CTP_%s%s%s%s%s%s",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, 1)), گہرائی,
                         صفیں_فی_WI, angle_اندر_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstMag   ? " -D SRC1_IS_DST_MAG" : "",
                         _src1IsDstAngle ? " -D SRC1_IS_DST_ANGLE" : "",
                         _src2IsDstMag   ? " -D SRC2_IS_DST_MAG" : "",
                         _src2IsDstAngle ? " -D SRC2_IS_DST_ANGLE" : ""
                         ));
    اگر (k.empty())
        واپس جھوٹ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    سائز size = src1.size();
    CV_Assert( size == src2.size() );

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.دلائل(_src1IsDstMag || _src1IsDstAngle ? ocl::KernelArg::Read_Write_No_سائز(src1) : ocl::KernelArg::Read_Only_No_سائز(src1),
           _src2IsDstMag || _src2IsDstAngle ? ocl::KernelArg::Read_Write_No_سائز(src2) : ocl::KernelArg::Read_Only_No_سائز(src2),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_سائز(dst2));

    size_t globalsize[2] = { (size_t)dst1.ستون_تعداد * cn, ((size_t)dst1.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

خالی cart_مقصد_Polar( داخلہ_Array src1, داخلہ_Array src2,
                  OutputArray dst1, OutputArray dst2, منطقی angle_اندر_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_تسلسل(dst1.isUMat() && dst2.isUMat(),
            ocl_cart_مقصد_Polar(src1, src2, dst1, dst2, angle_اندر_Degrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    صحیح type = X.type(), گہرائی = X.گہرائی(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (گہرائی == CV_32F || گہرائی == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح j, کل = (صحیح)(it.size*cn), بلاک_سائز_2 = std::min(کل, ((بلاک_سائز+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( j = 0; j < کل; j += بلاک_سائز_2 )
        {
            صحیح len = std::min(کل - j, بلاک_سائز_2);
            اگر( گہرائی == CV_32F )
            {
                ثابت اعشاری *x = (ثابت اعشاری*)ptrs[0], *y = (ثابت اعشاری*)ptrs[1];
                اعشاری *mag = (اعشاری*)ptrs[2], *angle = (اعشاری*)ptrs[3];
                hal::cart_مقصد_Polar32f( x, y, mag, angle, len, angle_اندر_Degrees );
            }
            ورنہ
            {
                ثابت دوہرا *x = (ثابت دوہرا*)ptrs[0], *y = (ثابت دوہرا*)ptrs[1];
                دوہرا *mag = (دوہرا*)ptrs[2], *angle = (دوہرا*)ptrs[3];
                hal::cart_مقصد_Polar64f(x, y, mag, angle, len, angle_اندر_Degrees);
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

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_polar_مقصد_Cart( داخلہ_Array _mag, داخلہ_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, منطقی angle_اندر_Degrees )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحیح type = _angle.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type),
            صفیں_فی_WI = d.isIntel() ? 4 : 1;
    منطقی doubleSupport = d.doubleFPConfig() > 0;

    ثابت منطقی _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ثابت منطقی _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ثابت منطقی _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ثابت منطقی _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    اگر ( !doubleSupport && گہرائی == CV_64F )
        واپس جھوٹ;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D BINARY_OP -D OP_PTC_%s%s%s%s%s%s",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, 1)), گہرائی,
                         صفیں_فی_WI,
                         angle_اندر_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstX   ? " -D SRC1_IS_DST_X" : "",
                         _src1IsDstY ? " -D SRC1_IS_DST_Y" : "",
                         _src2IsDstX   ? " -D SRC2_IS_DST_X" : "",
                         _src2IsDstY ? " -D SRC2_IS_DST_Y" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    UMat mag = _mag.getUMat(), angle = _angle.getUMat();
    سائز size = angle.size();
    CV_Assert(mag.size() == size);

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.دلائل(_src1IsDstX || _src1IsDstY ? ocl::KernelArg::Read_Write_No_سائز(mag) : ocl::KernelArg::Read_Only_No_سائز(mag),
           _src2IsDstX || _src2IsDstY  ? ocl::KernelArg::Read_Write_No_سائز(angle) : ocl::KernelArg::Read_Only_No_سائز(angle),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_سائز(dst2));

    size_t globalsize[2] = { (size_t)dst1.ستون_تعداد * cn, ((size_t)dst1.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

خالی polar_مقصد_Cart( داخلہ_Array src1, داخلہ_Array src2,
                  OutputArray dst1, OutputArray dst2, منطقی angle_اندر_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    صحیح type = src2.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type);
    CV_Assert((گہرائی == CV_32F || گہرائی == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_تسلسل(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polar_مقصد_Cart(src1, src2, dst1, dst2, angle_اندر_Degrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ثابت Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح j, کل = (صحیح)(it.size*cn), بلاک_سائز_2 = std::min(کل, ((بلاک_سائز+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( j = 0; j < کل; j += بلاک_سائز_2 )
        {
            صحیح len = std::min(کل - j, بلاک_سائز_2);
            اگر ( گہرائی == CV_32F )
            {
                ثابت اعشاری *mag = (ثابت اعشاری*)ptrs[0], *angle = (ثابت اعشاری*)ptrs[1];
                اعشاری *x = (اعشاری*)ptrs[2], *y = (اعشاری*)ptrs[3];
                hal::polar_مقصد_Cart32f( mag, angle, x, y, len, angle_اندر_Degrees );
            }
            ورنہ
            {
                ثابت دوہرا *mag = (ثابت دوہرا*)ptrs[0], *angle = (ثابت دوہرا*)ptrs[1];
                دوہرا *x = (دوہرا*)ptrs[2], *y = (دوہرا*)ptrs[3];
                hal::polar_مقصد_Cart64f( mag, angle, x, y, len, angle_اندر_Degrees );
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
شمار { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

ساکن خالی math_op(ew::چوٹی op, داخلہ_Array _src, OutputArray _dst)
{
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type);
    CV_Assert(گہرائی == CV_16F || گہرائی == CV_16BF || گہرائی == CV_32F || گہرائی == CV_64F);

    Mat src = _src.getMat();
    _dst.create_Same_سائز(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    اگر (src.empty())
        واپس;

    ثابت size_t کل = src.کل() * src.channels();
    اگر (src.isContinuous() && dst.isContinuous() && کل <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, گہرائی);
        CV_Assert(k.fptr);
        ساکن ثابت دوہرا noparams[4] = {};
        k.fptr(src.ڈیٹا, 0, 1, خالی_اشاریہ, 0, 0, خالی_اشاریہ, 0, 0,
               dst.ڈیٹا, 0, (صحیح)کل, 1, noparams, k.flags, k.userdata);
        واپس;
    }

    ew::TExpr p;
    ثابت صحیح a = p.جوڑو_داخلہ(گہرائی);
    ثابت صحیح out = p.جوڑو_Output(گہرائی);
    p.move_مقصد_Output(p.emitUnary(op, a, گہرائی), out);
    p.compile();
    ثابت Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

خالی قوت_نما( داخلہ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] صحیح گہرائی = _src.گہرائی();   // consumed by CV_OCL_RUN only

    CV_OCL_تسلسل(_dst.isUMat() && _src.dims() <= 2 && (گہرائی == CV_32F || گہرائی == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

خالی لوگارتھم( داخلہ_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] صحیح گہرائی = _src.گہرائی();   // consumed by CV_OCL_RUN only

    CV_OCL_تسلسل( _dst.isUMat() && _src.dims() <= 2 && (گہرائی == CV_32F || گہرائی == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

سانچہ <قسم_نام T, قسم_نام WT>
ساخت iPow_SIMD
{
    صحیح عامل() ( ثابت T *, T *, صحیح, صحیح)
    {
        واپس 0;
    }
};

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

سانچہ <>
ساخت iPow_SIMD<uchar, غیرعلامتی>
{
    صحیح عامل() ( ثابت uchar * src, uchar * dst, صحیح len, صحیح power )
    {
        صحیح i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        برائے ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_بھرو_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحیح p = power;

            جب( p > 1 )
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

        واپس i;
    }
};

سانچہ <>
ساخت iPow_SIMD<schar, صحیح>
{
    صحیح عامل() ( ثابت schar * src, schar * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        برائے ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_بھرو_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحیح p = power;

            جب( p > 1 )
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

        واپس i;
    }
};

سانچہ <>
ساخت iPow_SIMD<ushort, غیرعلامتی>
{
    صحیح عامل() ( ثابت ushort * src, ushort * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        برائے ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_بھرو(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحیح p = power;

            جب( p > 1 )
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

        واپس i;
    }
};

سانچہ <>
ساخت iPow_SIMD<مختصر, صحیح>
{
    صحیح عامل() ( ثابت مختصر * src, مختصر * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        برائے ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_بھرو(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحیح p = power;

            جب( p > 1 )
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

        واپس i;
    }
};

سانچہ <>
ساخت iPow_SIMD<صحیح, صحیح>
{
    صحیح عامل() ( ثابت صحیح * src, صحیح * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        برائے ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_بھرو(src + i), v_b2 = vx_بھرو(src + i + VTraits<v_int32>::vlanes());
            صحیح p = power;

            جب( p > 1 )
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

        واپس i;
    }
};

سانچہ <>
ساخت iPow_SIMD<اعشاری, اعشاری>
{
    صحیح عامل() ( ثابت اعشاری * src, اعشاری * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        برائے ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_بھرو(src + i), v_b2 = vx_بھرو(src + i + VTraits<v_float32>::vlanes());
            صحیح p = std::مطلق_قدر(power);
            اگر( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            جب( p > 1 )
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

        واپس i;
    }
};

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
سانچہ <>
ساخت iPow_SIMD<دوہرا, دوہرا>
{
    صحیح عامل() ( ثابت دوہرا * src, دوہرا * dst, صحیح len, صحیح power)
    {
        صحیح i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        برائے ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_بھرو(src + i), v_b2 = vx_بھرو(src + i + VTraits<v_float64>::vlanes());
            صحیح p = std::مطلق_قدر(power);
            اگر( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            جب( p > 1 )
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

        واپس i;
    }
};
#اگر_ختم

#اگر_ختم

سانچہ<قسم_نام T, قسم_نام WT>
ساکن خالی
iPow_i( ثابت خالی* _src, خالی* _dst, صحیح len, صحیح power )
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
        برائے( صحیح i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    ورنہ
    {
        iPow_SIMD<T, WT> vop;
        صحیح i = vop(src, dst, len, power);

        برائے( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            صحیح p = power;
            جب( p > 1 )
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

سانچہ<قسم_نام T>
ساکن خالی
iPow_f( ثابت خالی* _src, خالی* _dst, صحیح len, صحیح power0 )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    صحیح i = vop(src, dst, len, power0);
    صحیح power = std::مطلق_قدر(power0);

    برائے( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        صحیح p = power;
        اگر( power0 < 0 )
            b = 1/b;

        جب( p > 1 )
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

قسمتعریف خالی (*IPowFunc)( ثابت خالی* src, خالی* dst, صحیح len, صحیح power );

ساکن IPowFunc ipowTab[CV_گہرائی_MAX] =
{
    iPow_i<uchar, غیرعلامتی>, iPow_i<schar, صحیح>, iPow_i<ushort, غیرعلامتی>, iPow_i<مختصر, صحیح>,
    iPow_i<صحیح, صحیح>, iPow_f<اعشاری>, iPow_f<دوہرا>, 0
};

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_pow(داخلہ_Array _src, دوہرا power, OutputArray _dst,
                    منطقی is_ipower, صحیح ipower)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type),
            صفیں_فی_WI = d.isIntel() ? 4 : 1;
    منطقی doubleSupport = d.doubleFPConfig() > 0;

    _dst.create_Same_سائز(_src, type);
    اگر (is_ipower)
    {
        اگر( ipower < 0 )
        {
            اگر( گہرائی == CV_32F || گہرائی == CV_64F )
                is_ipower = جھوٹ;
            ورنہ
                واپس جھوٹ;
        }
    }

    اگر (گہرائی == CV_64F && !doubleSupport)
        واپس جھوٹ;

    منطقی issqrt = std::مطلق_قدر(power - 0.5) < DBL_EPSILON;
    ثابت حرف * ثابت op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    اگر (is_ipower)
    {
        صحیح wdepth = CV_32F;
        اگر (گہرائی == CV_64F)
            wdepth = CV_64F;
        ورنہ اگر (گہرائی == CV_16F)
            wdepth = CV_16F;

        حرف cvt[2][50];
        extra_opts = format(
            " -D srcT1=%s -DsrcT1_C1=%s"
            " -D srcT2=int -D workST=int"
            " -D workT=%s -D wdepth=%d -D convertToWT1=%s"
            " -D convertToDT=%s"
            " -D workT1=%s",
            ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, 1)),
            ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, 1)),
            ocl::type_مقصد_Str(CV_MAKE_TYPE(wdepth, 1)),
            wdepth,
            ocl::convertTypeStr(گہرائی, wdepth, 1, cvt[0], حجم(cvt[0])),
            ocl::convertTypeStr(wdepth, گہرائی, 1, cvt[1], حجم(cvt[1])),
            ocl::type_مقصد_Str(wdepth)
        );
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D cn=%d -D dstT=%s -D dstT_C1=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D %s%s%s%s",
                         1,
                         ocl::type_مقصد_Str(گہرائی), ocl::type_مقصد_Str(گہرائی), گہرائی, صفیں_فی_WI, op,
                         " -D UNARY_OP=1",
                         extra_opts.empty() ? "" : extra_opts.c_str(),
                         doubleSupport ? " -D DOUBLE_SUPPORT" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    UMat src = _src.getUMat();
    _dst.create_Same_سائز(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_سائز(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    اگر (issqrt)
        k.دلائل(srcarg, dstarg);
    ورنہ اگر (is_ipower)
        k.دلائل(srcarg, dstarg, ipower);
    ورنہ
    {
        اگر (گہرائی == CV_32F)
            k.دلائل(srcarg, dstarg, (اعشاری)power);
        ورنہ
            k.دلائل(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.ستون_تعداد *  cn, ((size_t)dst.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

خالی قوت( داخلہ_Array _src, دوہرا power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type),
            cn = CV_MAT_CN(type), ipower = cv_باری(power);
    منطقی is_ipower = fabs(ipower - power) < DBL_EPSILON;
#اگر_تعریف_شدہ HAVE_OPENCL
    منطقی useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#اگر_ختم

    اگر (is_ipower)
    {
        انتخاب( ipower )
        {
        حالت 0:
            _dst.create_Same_سائز(_src, type);
            _dst.سیٹ_مقصد(Scalar::all(1));
            واپس;
        حالت 1:
            _src.copy_مقصد(_dst);
            واپس;
        حالت 2:
            multiply(_src, _src, _dst);
            واپس;
        }
    }

    CV_OCL_تسلسل(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ثابت منطقی float_گہرائی = گہرائی == CV_16F || گہرائی == CV_16BF || گہرائی == CV_32F || گہرائی == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    اگر( is_ipower && !float_گہرائی && ipowTab[گہرائی] )
    {
        Mat src = _src.getMat();
        _dst.create_Same_سائز(_src, type);
        Mat dst = _dst.getMat();

        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحیح len = (صحیح)(it.size*cn);
        IPowFunc func = ipowTab[گہرائی];

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        واپس;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.create_Same_سائز(_src, type);
    Mat dst = _dst.getMat();
    اگر (src.empty())
        واپس;

    ثابت size_t کل = src.کل() * cn;
    اگر (float_گہرائی && src.isContinuous() && dst.isContinuous() && کل <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(گہرائی, گہرائی);
        اگر (k.fptr)
        {
            دوہرا pvstore;                    // the broadcast exponent, stored as T
            خالی* pv = &pvstore;
            انتخاب (گہرائی)
            {
            حالت CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); توڑو;
            حالت CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); توڑو;
            حالت CV_32F:  *(اعشاری*)pv = (اعشاری)power; توڑو;
            طےشدہ:      pvstore = power; توڑو;
            }
            ساکن ثابت دوہرا noparams[4] = {};
            k.fptr(src.ڈیٹا, 0, 1, pv, 0, 0, خالی_اشاریہ, 0, 0,
                   dst.ڈیٹا, 0, (صحیح)کل, 1, noparams, k.flags, k.userdata);
            واپس;
        }
    }

    ew::TExpr prog;
    ثابت صحیح a = prog.جوڑو_داخلہ(گہرائی);
    ثابت صحیح c = prog.جوڑو_Const(ew::EW_گہرائی_NONE, Scalar(power), 1);
    ثابت صحیح out = prog.جوڑو_Output(گہرائی);
    prog.move_مقصد_Output(prog.emit_ثنائی(ew::OP_POW, a, c, گہرائی), out);
    prog.compile();
    ثابت Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

خالی جذر(داخلہ_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    اگر (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::قوت(a, 0.5, b);
        واپس;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

سانچہ<صحیح cv_mat_type> ساخت mat_type_assotiations{};

سانچہ<> ساخت mat_type_assotiations<CV_8U>
{
    قسمتعریف غیرعلامتی حرف type;
    ساکن ثابت type min_allowable = 0x0;
    ساکن ثابت type max_allowable = 0xFF;
};

سانچہ<> ساخت mat_type_assotiations<CV_8S>
{
    قسمتعریف علامتی حرف type;
    ساکن ثابت type min_allowable = SCHAR_MIN;
    ساکن ثابت type max_allowable = SCHAR_MAX;
};

سانچہ<> ساخت mat_type_assotiations<CV_16U>
{
    قسمتعریف غیرعلامتی مختصر type;
    ساکن ثابت type min_allowable = 0x0;
    ساکن ثابت type max_allowable = USHRT_MAX;
};
سانچہ<> ساخت mat_type_assotiations<CV_16S>
{
    قسمتعریف علامتی مختصر type;
    ساکن ثابت type min_allowable = SHRT_MIN;
    ساکن ثابت type max_allowable = SHRT_MAX;
};

سانچہ<> ساخت mat_type_assotiations<CV_32S>
{
    قسمتعریف صحیح type;
    ساکن ثابت type min_allowable = (-زیادہ_سے_زیادہ_عدد - 1);
    ساکن ثابت type max_allowable = زیادہ_سے_زیادہ_عدد;
};

سانچہ<صحیح گہرائی>
ساکن منطقی checkIntegerRange(cv::Mat src, نقطہ& bad_pt, صحیح minVal, صحیح maxVal)
{
    قسمتعریف mat_type_assotiations<گہرائی> type_ass;

    اگر (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        واپس سچ;
    }
    ورنہ اگر (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::نقطہ(0,0);
        واپس جھوٹ;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    برائے (صحیح j = 0; j < as_one_channel.صفیں; ++j)
        برائے (صحیح i = 0; i < as_one_channel.ستون_تعداد; ++i)
        {
            قسم_نام type_ass::type v = as_one_channel.at<قسم_نام type_ass::type>(j ,i);
            اگر (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                واپس جھوٹ;
            }
        }

    واپس سچ;
}

قسمتعریف منطقی (*check_range_function)(cv::Mat src, نقطہ& bad_pt, صحیح minVal, صحیح maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

سانچہ<قسم_نام T>
ساکن خالی checkTypedRange(Mat& src, نقطہ& badPt, دوہرا minVal, دوہرا maxVal)
{
    صحیح loc = 0;
    صحیح cn = src.channels();
    سائز size = getContinuousSize2D(src, cn);
    ثابت T* isrc = src.ptr<T>();
    size_t قدم = src.قدم / حجم(isrc[0]);

    برائے( ; badPt.x < 0 && size.اونچائی--; loc += size.چوڑائی, isrc += قدم )
    {
        برائے( صحیح i = 0; i < size.چوڑائی; i++ )
        {
            دوہرا val = (دوہرا)isrc[i];
            اگر( !(val >= minVal && val < maxVal) )
            {
                صحیح pixelId = (loc + i) / cn;
                badPt = نقطہ(pixelId % src.ستون_تعداد, pixelId / src.ستون_تعداد);
                توڑو;
            }
        }
    }
}

منطقی checkRange(داخلہ_Array _src, منطقی quiet, نقطہ* pt, دوہرا minVal, دوہرا maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    اگر ( src.dims > 2 )
    {
        CV_Assert(pt == تہی); // no way to provide location info

        ثابت Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        برائے ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر (!checkRange( it.planes[0], quiet, تہی, minVal, maxVal ))
            {
                واپس جھوٹ;
            }
        }
        واپس سچ;
    }

    صحیح گہرائی = src.گہرائی();
    نقطہ badPt(-1, -1);

    اگر (گہرائی < CV_32F)
    {
        صحیح minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        صحیح maxVali = maxVal > زیادہ_سے_زیادہ_عدد ? زیادہ_سے_زیادہ_عدد : cvCeil(maxVal) - 1;

        (check_range_functions[گہرائی])(src, badPt, minVali, maxVali);
    }
    ورنہ
    {
        صحیح i, loc = 0;
        صحیح cn = src.channels();
        سائز size = getContinuousSize2D(src, cn);

        اگر( گہرائی == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        ورنہ اگر( گہرائی == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        ورنہ اگر( گہرائی == CV_32F )
        {
            Cv32suf a, b;
            صحیح ia, ib;
            ثابت صحیح* isrc = src.ptr<صحیح>();
            size_t قدم = src.قدم/حجم(isrc[0]);

            a.f = (اعشاری)std::max(minVal, (دوہرا)-FLT_MAX);
            b.f = (اعشاری)std::min(maxVal, (دوہرا)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            برائے( ; badPt.x < 0 && size.اونچائی--; loc += size.چوڑائی, isrc += قدم )
            {
                برائے( i = 0; i < size.چوڑائی; i++ )
                {
                    صحیح val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    اگر( val < ia || val >= ib )
                    {
                        صحیح pixelId = (loc + i) / cn;
                        badPt = نقطہ(pixelId % src.ستون_تعداد, pixelId / src.ستون_تعداد);
                        توڑو;
                    }
                }
            }
        }
        ورنہ اگر( گہرائی == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ثابت int64* isrc = src.ptr<int64>();
            size_t قدم = src.قدم/حجم(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            برائے( ; badPt.x < 0 && size.اونچائی--; loc += size.چوڑائی, isrc += قدم )
            {
                برائے( i = 0; i < size.چوڑائی; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    اگر( val < ia || val >= ib )
                    {
                        صحیح pixelId = (loc + i) / cn;
                        badPt = نقطہ(pixelId % src.ستون_تعداد, pixelId / src.ستون_تعداد);
                        توڑو;
                    }
                }
            }
        }
        ورنہ اگر( گہرائی == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        ورنہ اگر( گہرائی == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        ورنہ اگر( گہرائی == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        ورنہ اگر( گہرائی == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        ورنہ
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
            cv::String قدر_str;
            قدر_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::Sts_برآمد_کا_Range,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, قدر_str.c_str(), minVal, maxVal));
        }
        واپس جھوٹ;
    }
    واپس سچ;
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

صحیح cv::solveCubic( داخلہ_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ثابت صحیح n0 = 3;
    Mat coeffs = _coeffs.getMat();
    صحیح ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == سائز(n0, 1) ||
                coeffs.size() == سائز(n0+1, 1) ||
                coeffs.size() == سائز(1, n0) ||
                coeffs.size() == سائز(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, سچ, _OutputArray::گہرائی_نقاب_FLT);
    Mat roots = _roots.getMat();

    صحیح i = -1, n = 0;
    دوہرا a0 = 1., a1, a2, a3;
    دوہرا x0 = 0., x1 = 0., x2 = 0.;
    صحیح ncoeffs = coeffs.صفیں + coeffs.ستون_تعداد - 1;

    اگر( ctype == CV_32FC1 )
    {
        اگر( ncoeffs == 4 )
            a0 = coeffs.at<اعشاری>(++i);

        a1 = coeffs.at<اعشاری>(i+1);
        a2 = coeffs.at<اعشاری>(i+2);
        a3 = coeffs.at<اعشاری>(i+3);
    }
    ورنہ
    {
        اگر( ncoeffs == 4 )
            a0 = coeffs.at<دوہرا>(++i);

        a1 = coeffs.at<دوہرا>(i+1);
        a2 = coeffs.at<دوہرا>(i+2);
        a3 = coeffs.at<دوہرا>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    دوہرا max_coeff = std::max({std::مطلق_قدر(a0), std::مطلق_قدر(a1), std::مطلق_قدر(a2), std::مطلق_قدر(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    اگر (max_coeff < std::numeric_limits<دوہرا>::epsilon())
        واپس -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    اگر( std::مطلق_قدر(a0) < std::numeric_limits<دوہرا>::epsilon() * max_coeff )
    {
        اگر( std::مطلق_قدر(a1) < std::numeric_limits<دوہرا>::epsilon() * max_coeff )
        {
            اگر( std::مطلق_قدر(a2) < std::numeric_limits<دوہرا>::epsilon() * max_coeff )
                n = std::مطلق_قدر(a3) < std::numeric_limits<دوہرا>::epsilon() * max_coeff ? -1 : 0;
            ورنہ
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        ورنہ
        {
            // quadratic equation
            دوہرا d = a2*a2 - 4*a1*a3;
            اگر( d >= 0 )
            {
                d = std::جذر(d);
                دوہرا q1 = (-a2 + d) * 0.5;
                دوہرا q2 = (a2 + d) * -0.5;
                اگر( std::مطلق_قدر(q1) > std::مطلق_قدر(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                ورنہ
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    ورنہ
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        دوہرا scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        دوہرا Q = (a1 * a1 - 3 * a2) * (1./9);
        دوہرا R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        دوہرا Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        دوہرا d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        اگر( d > 0 )
        {
            دوہرا theta = acos(R / جذر(Qcubed));
            دوہرا sqrtQ = جذر(Q);
            دوہرا t0 = -2 * sqrtQ;
            دوہرا t1 = theta * (1./3);
            دوہرا t2 = a1 * (1./3);
            x0 = t0 * جیب_تمام(t1) - t2;
            x1 = t0 * جیب_تمام(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * جیب_تمام(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        ورنہ اگر( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        ورنہ
        {
            دوہرا e;
            d = جذر(-d);
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
    ورنہ
    {
        roots.at<دوہرا>(0) = x0;
        roots.at<دوہرا>(1) = x1;
        roots.at<دوہرا>(2) = x2;
    }

    واپس n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
دوہرا cv::solvePoly( داخلہ_Array _coeffs0, OutputArray _roots0, صحیح maxIters )
{
    CV_INSTRUMENT_REGION();

    قسمتعریف Complex<دوہرا> C;

    دوہرا maxDiff = 0;
    صحیح iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    صحیح ctype = _coeffs0.type();
    صحیح cdepth = CV_MAT_گہرائی(ctype);

    CV_Assert( CV_MAT_گہرائی(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.صفیں == 1 || coeffs0.ستون_تعداد == 1) && coeffs0.کل() >= 2 );

    صحیح n0 = coeffs0.ستون_تعداد + coeffs0.صفیں - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, سچ, _OutputArray::گہرائی_نقاب_FLT);
    Mat roots0 = _roots0.getMat();

    Auto_بفر<C> buf(n*2+2);
    C *coeffs = buf.ڈیٹا(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convert_مقصد(coeffs1, coeffs1.type());
    اگر( coeffs0.channels() == 1 )
    {
        ثابت دوہرا* rcoeffs = (ثابت دوہرا*)roots;
        برائے( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    برائے( ; n > 1; n-- )
    {
        اگر( std::مطلق_قدر(coeffs[n].re) + std::مطلق_قدر(coeffs[n].im) > DBL_EPSILON )
            توڑو;
    }
    CV_Assert( std::مطلق_قدر(coeffs[n].re) + std::مطلق_قدر(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    برائے( i = 0; i <= n; i++ )
        absCoeffs.at<دوہرا>(i) = مطلق_قدر(coeffs[i]);

    صحیح nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    برائے( i = 0; i <= n; i++ )
    {
        دوہرا coeff = absCoeffs.at<دوہرا>(i);
        اگر( coeff > DBL_EPSILON )
        {
            اگر( i != n )
                u.at<دوہرا>(i) = 2.0 * قوت(coeff / absCoeffs.at<دوہرا>(n), 1.0 / (n - i));
            اگر( i != 0 )
                v.at<دوہرا>(i - 1) = 0.5 * قوت(absCoeffs.at<دوہرا>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    دوہرا scale = 1;
    اگر( nonZeroCoeffs > 2 )
    {
        نقطہ maxU, minV;
        minMaxLoc(u, خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ, &maxU);
        minMaxLoc(v, خالی_اشاریہ, خالی_اشاریہ, &minV);
        u.at<دوہرا>(maxU) = 0;
        v.at<دوہرا>(minV) = 0;
        scale = (مجموع(u).val[0] + مجموع(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(جیب_تمام(CV_2PI / n), جیب(CV_2PI / n));

    برائے( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    برائے( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        برائے( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            صحیح عدد_same_جڑ = 1;
            برائے( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                اگر( j != i )
                {
                    اگر ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    ورنہ
                        عدد_same_جڑ++;
                }
            }
            num /= denom;
            اگر( عدد_same_جڑ > 1)
            {
                دوہرا old_عدد_re = num.re;
                دوہرا old_عدد_im = num.im;
                صحیح مربع_جڑ_times = عدد_same_جڑ % 2 == 0 ? عدد_same_جڑ / 2 : عدد_same_جڑ / 2 - 1;

                برائے( j = 0; j < مربع_جڑ_times; j++)
                {
                    num.re = old_عدد_re*old_عدد_re + old_عدد_im*old_عدد_im;
                    num.re = جذر(num.re);
                    num.re += old_عدد_re;
                    num.im = num.re - old_عدد_re;
                    num.re /= 2;
                    num.re = جذر(num.re);

                    num.im /= 2;
                    num.im = جذر(num.im);
                    اگر( old_عدد_re < 0 ) num.im = -num.im;
                }

                اگر( عدد_same_جڑ % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<دوہرا>(3) = -(std::قوت(old_عدد_re, 3));
                    cube_coefs.at<دوہرا>(2) = -(15*std::قوت(old_عدد_re, 2) + 27*std::قوت(old_عدد_im, 2));
                    cube_coefs.at<دوہرا>(1) = -48*old_عدد_re;
                    cube_coefs.at<دوہرا>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<دوہرا>(0));
                    num.im = جذر(std::قوت(num.re, 2) / 3 - old_عدد_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::مطلق_قدر(num));
        }
        اگر( maxDiff <= 0 )
            توڑو;
    }

    اگر( coeffs0.channels() == 1 )
    {
        ثابت دوہرا verySmallEps = 1e-100;
        برائے( i = 0; i < n; i++ )
            اگر( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    برائے( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convert_مقصد(roots0, roots0.type());
    واپس maxDiff;
}


/* End of file. */
