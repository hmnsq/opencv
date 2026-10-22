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

#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن <atomic>
#ضمن <limits>
#ضمن <iostream>
#ضمن <algorithm>
#ضمن <cmath>
#ضمن "mathfuncs.hpp"
#ضمن "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

نطاق cv
{

تعريفنوع فراغ (*MathFunc)(ثابت فراغ* src, فراغ* dst, صحيح len);

#که_تعریف_شوی HAVE_OPENCL

تعداد { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

ساكن ثابت حرف* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

ساكن منطقي ocl_math_op(ننوت_Array _src1, ننوت_Array _src2, OutputArray _dst, صحيح oclop)
{
    صحيح type = _src1.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type);
    صحيح kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predict_Optimal_ویکټور_پلنوالی(_src1, _src2, _dst);

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي double_support = d.doubleFPConfig() > 0;
    إذا (!double_support && ژوروالی == CV_64F)
        أرجع خطأ;
    صحيح لیکې_فی_WI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, kercn)), ژوروالی, لیکې_فی_WI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.create_Same_اندازه(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_اندازه(src1),
            src2arg = ocl::KernelArg::Read_Only_No_اندازه(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    إذا (src2.empty())
        k.دلیلونه(src1arg, dstarg);
    وإلا
        k.دلیلونه(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.ستون_شمېر * cn / kercn, ((size_t)src1.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, 0, خطأ);
}

#د_که_پای

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN عائم cube_ریښه( عائم value )
{
    CV_INSTRUMENT_REGION();

    عائم fr;
    Cv32suf v, m;
    صحيح ix, s;
    صحيح ex, shx;

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
    fr = (عائم)(((((45.2548339756803022511987494 * fr +
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
    أرجع v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

فراغ magnitude( ننوت_Array src1, ننوت_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    صحيح type = src1.type(), ژوروالی = src1.ژوروالی(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (ژوروالی == CV_32F || ژوروالی == CV_64F));

    CV_OCL_پرلپسې(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح len = (صحيح)it.size*cn;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        إذا( ژوروالی == CV_32F )
        {
            ثابت عائم *x = (ثابت عائم*)ptrs[0], *y = (ثابت عائم*)ptrs[1];
            عائم *mag = (عائم*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        وإلا
        {
            ثابت مزدوج *x = (ثابت مزدوج*)ptrs[0], *y = (ثابت مزدوج*)ptrs[1];
            مزدوج *mag = (مزدوج*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

فراغ phase( ننوت_Array src1, ننوت_Array src2, OutputArray dst, منطقي angle_دننه_Degrees )
{
    CV_INSTRUMENT_REGION();

    صحيح type = src1.type(), ژوروالی = src1.ژوروالی(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (ژوروالی == CV_32F || ژوروالی == CV_64F));

    CV_OCL_پرلپسې(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angle_دننه_Degrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح j, ټول = (صحيح)(it.size*cn), بلاک_اندازه_2 = ټول;
    size_t esz1 = X.elemSize1();
    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < ټول; j += بلاک_اندازه_2 )
        {
            صحيح len = std::min(ټول - j, بلاک_اندازه_2);
            إذا( ژوروالی == CV_32F )
            {
                ثابت عائم *x = (ثابت عائم*)ptrs[0], *y = (ثابت عائم*)ptrs[1];
                عائم *angle = (عائم*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angle_دننه_Degrees );
            }
            وإلا
            {
                ثابت مزدوج *x = (ثابت مزدوج*)ptrs[0], *y = (ثابت مزدوج*)ptrs[1];
                مزدوج *angle = (مزدوج*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angle_دننه_Degrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_cart_منزل_Polar( ننوت_Array _src1, ننوت_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, منطقي angle_دننه_Degrees )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحيح type = _src1.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type),
            لیکې_فی_WI = d.isIntel() ? 4 : 1;
    منطقي doubleSupport = d.doubleFPConfig() > 0;

    ثابت منطقي _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    ثابت منطقي _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    ثابت منطقي _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    ثابت منطقي _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    إذا ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (ژوروالی == CV_32F || ژوروالی == CV_64F) && type == _src2.type()) ||
         (ژوروالی == CV_64F && !doubleSupport) )
        أرجع خطأ;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D OP_CTP_%s%s%s%s%s%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, 1)), ژوروالی,
                         لیکې_فی_WI, angle_دننه_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstMag   ? " -D SRC1_IS_DST_MAG" : "",
                         _src1IsDstAngle ? " -D SRC1_IS_DST_ANGLE" : "",
                         _src2IsDstMag   ? " -D SRC2_IS_DST_MAG" : "",
                         _src2IsDstAngle ? " -D SRC2_IS_DST_ANGLE" : ""
                         ));
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    اندازه size = src1.size();
    CV_Assert( size == src2.size() );

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.دلیلونه(_src1IsDstMag || _src1IsDstAngle ? ocl::KernelArg::Read_Write_No_اندازه(src1) : ocl::KernelArg::Read_Only_No_اندازه(src1),
           _src2IsDstMag || _src2IsDstAngle ? ocl::KernelArg::Read_Write_No_اندازه(src2) : ocl::KernelArg::Read_Only_No_اندازه(src2),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_اندازه(dst2));

    size_t globalsize[2] = { (size_t)dst1.ستون_شمېر * cn, ((size_t)dst1.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
}

#د_که_پای

فراغ cart_منزل_Polar( ننوت_Array src1, ننوت_Array src2,
                  OutputArray dst1, OutputArray dst2, منطقي angle_دننه_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_پرلپسې(dst1.isUMat() && dst2.isUMat(),
            ocl_cart_منزل_Polar(src1, src2, dst1, dst2, angle_دننه_Degrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    صحيح type = X.type(), ژوروالی = X.ژوروالی(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (ژوروالی == CV_32F || ژوروالی == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    ثابت Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح j, ټول = (صحيح)(it.size*cn), بلاک_اندازه_2 = std::min(ټول, ((بلاک_اندازه+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < ټول; j += بلاک_اندازه_2 )
        {
            صحيح len = std::min(ټول - j, بلاک_اندازه_2);
            إذا( ژوروالی == CV_32F )
            {
                ثابت عائم *x = (ثابت عائم*)ptrs[0], *y = (ثابت عائم*)ptrs[1];
                عائم *mag = (عائم*)ptrs[2], *angle = (عائم*)ptrs[3];
                hal::cart_منزل_Polar32f( x, y, mag, angle, len, angle_دننه_Degrees );
            }
            وإلا
            {
                ثابت مزدوج *x = (ثابت مزدوج*)ptrs[0], *y = (ثابت مزدوج*)ptrs[1];
                مزدوج *mag = (مزدوج*)ptrs[2], *angle = (مزدوج*)ptrs[3];
                hal::cart_منزل_Polar64f(x, y, mag, angle, len, angle_دننه_Degrees);
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

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_polar_منزل_Cart( ننوت_Array _mag, ننوت_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, منطقي angle_دننه_Degrees )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحيح type = _angle.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type),
            لیکې_فی_WI = d.isIntel() ? 4 : 1;
    منطقي doubleSupport = d.doubleFPConfig() > 0;

    ثابت منطقي _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    ثابت منطقي _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    ثابت منطقي _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    ثابت منطقي _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    إذا ( !doubleSupport && ژوروالی == CV_64F )
        أرجع خطأ;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D BINARY_OP -D OP_PTC_%s%s%s%s%s%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, 1)), ژوروالی,
                         لیکې_فی_WI,
                         angle_دننه_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstX   ? " -D SRC1_IS_DST_X" : "",
                         _src1IsDstY ? " -D SRC1_IS_DST_Y" : "",
                         _src2IsDstX   ? " -D SRC2_IS_DST_X" : "",
                         _src2IsDstY ? " -D SRC2_IS_DST_Y" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat mag = _mag.getUMat(), angle = _angle.getUMat();
    اندازه size = angle.size();
    CV_Assert(mag.size() == size);

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.دلیلونه(_src1IsDstX || _src1IsDstY ? ocl::KernelArg::Read_Write_No_اندازه(mag) : ocl::KernelArg::Read_Only_No_اندازه(mag),
           _src2IsDstX || _src2IsDstY  ? ocl::KernelArg::Read_Write_No_اندازه(angle) : ocl::KernelArg::Read_Only_No_اندازه(angle),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_اندازه(dst2));

    size_t globalsize[2] = { (size_t)dst1.ستون_شمېر * cn, ((size_t)dst1.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
}

#د_که_پای

فراغ polar_منزل_Cart( ننوت_Array src1, ننوت_Array src2,
                  OutputArray dst1, OutputArray dst2, منطقي angle_دننه_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    صحيح type = src2.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type);
    CV_Assert((ژوروالی == CV_32F || ژوروالی == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_پرلپسې(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polar_منزل_Cart(src1, src2, dst1, dst2, angle_دننه_Degrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    ثابت Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح j, ټول = (صحيح)(it.size*cn), بلاک_اندازه_2 = std::min(ټول, ((بلاک_اندازه+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < ټول; j += بلاک_اندازه_2 )
        {
            صحيح len = std::min(ټول - j, بلاک_اندازه_2);
            إذا ( ژوروالی == CV_32F )
            {
                ثابت عائم *mag = (ثابت عائم*)ptrs[0], *angle = (ثابت عائم*)ptrs[1];
                عائم *x = (عائم*)ptrs[2], *y = (عائم*)ptrs[3];
                hal::polar_منزل_Cart32f( mag, angle, x, y, len, angle_دننه_Degrees );
            }
            وإلا
            {
                ثابت مزدوج *mag = (ثابت مزدوج*)ptrs[0], *angle = (ثابت مزدوج*)ptrs[1];
                مزدوج *x = (مزدوج*)ptrs[2], *y = (مزدوج*)ptrs[3];
                hal::polar_منزل_Cart64f( mag, angle, x, y, len, angle_دننه_Degrees );
            }

            إذا( ptrs[0] )
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
تعداد { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

ساكن فراغ math_op(ew::سر op, ننوت_Array _src, OutputArray _dst)
{
    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type);
    CV_Assert(ژوروالی == CV_16F || ژوروالی == CV_16BF || ژوروالی == CV_32F || ژوروالی == CV_64F);

    Mat src = _src.getMat();
    _dst.create_Same_اندازه(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    إذا (src.empty())
        أرجع;

    ثابت size_t ټول = src.ټول() * src.channels();
    إذا (src.isContinuous() && dst.isContinuous() && ټول <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, ژوروالی);
        CV_Assert(k.fptr);
        ساكن ثابت مزدوج noparams[4] = {};
        k.fptr(src.معلومات, 0, 1, تش_اشاره, 0, 0, تش_اشاره, 0, 0,
               dst.معلومات, 0, (صحيح)ټول, 1, noparams, k.flags, k.userdata);
        أرجع;
    }

    ew::TExpr p;
    ثابت صحيح a = p.جمع_کړه_ننوت(ژوروالی);
    ثابت صحيح out = p.جمع_کړه_Output(ژوروالی);
    p.move_منزل_Output(p.emitUnary(op, a, ژوروالی), out);
    p.compile();
    ثابت Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

فراغ توانیز_تابع( ننوت_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] صحيح ژوروالی = _src.ژوروالی();   // consumed by CV_OCL_RUN only

    CV_OCL_پرلپسې(_dst.isUMat() && _src.dims() <= 2 && (ژوروالی == CV_32F || ژوروالی == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

فراغ لوګاریتم( ننوت_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] صحيح ژوروالی = _src.ژوروالی();   // consumed by CV_OCL_RUN only

    CV_OCL_پرلپسې( _dst.isUMat() && _src.dims() <= 2 && (ژوروالی == CV_32F || ژوروالی == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

کينډۍ <ډول_نوم T, ډول_نوم WT>
بنية iPow_SIMD
{
    صحيح چلوونکی() ( ثابت T *, T *, صحيح, صحيح)
    {
        أرجع 0;
    }
};

#إذا (CV_SIMD || CV_SIMD_SCALABLE)

کينډۍ <>
بنية iPow_SIMD<uchar, بینښه>
{
    صحيح چلوونکی() ( ثابت uchar * src, uchar * dst, صحيح len, صحيح power )
    {
        صحيح i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        لكل ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_بارول_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحيح p = power;

            طالما( p > 1 )
            {
                إذا (p & 1)
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

        أرجع i;
    }
};

کينډۍ <>
بنية iPow_SIMD<schar, صحيح>
{
    صحيح چلوونکی() ( ثابت schar * src, schar * dst, صحيح len, صحيح power)
    {
        صحيح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        لكل ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_بارول_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحيح p = power;

            طالما( p > 1 )
            {
                إذا (p & 1)
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

        أرجع i;
    }
};

کينډۍ <>
بنية iPow_SIMD<ushort, بینښه>
{
    صحيح چلوونکی() ( ثابت ushort * src, ushort * dst, صحيح len, صحيح power)
    {
        صحيح i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        لكل ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_بارول(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحيح p = power;

            طالما( p > 1 )
            {
                إذا (p & 1)
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

        أرجع i;
    }
};

کينډۍ <>
بنية iPow_SIMD<لنډ, صحيح>
{
    صحيح چلوونکی() ( ثابت لنډ * src, لنډ * dst, صحيح len, صحيح power)
    {
        صحيح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        لكل ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_بارول(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            صحيح p = power;

            طالما( p > 1 )
            {
                إذا (p & 1)
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

        أرجع i;
    }
};

کينډۍ <>
بنية iPow_SIMD<صحيح, صحيح>
{
    صحيح چلوونکی() ( ثابت صحيح * src, صحيح * dst, صحيح len, صحيح power)
    {
        صحيح i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        لكل ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_بارول(src + i), v_b2 = vx_بارول(src + i + VTraits<v_int32>::vlanes());
            صحيح p = power;

            طالما( p > 1 )
            {
                إذا (p & 1)
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

        أرجع i;
    }
};

کينډۍ <>
بنية iPow_SIMD<عائم, عائم>
{
    صحيح چلوونکی() ( ثابت عائم * src, عائم * dst, صحيح len, صحيح power)
    {
        صحيح i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        لكل ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_بارول(src + i), v_b2 = vx_بارول(src + i + VTraits<v_float32>::vlanes());
            صحيح p = std::مطلق_قیمت(power);
            إذا( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            طالما( p > 1 )
            {
                إذا (p & 1)
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

        أرجع i;
    }
};

#إذا (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
کينډۍ <>
بنية iPow_SIMD<مزدوج, مزدوج>
{
    صحيح چلوونکی() ( ثابت مزدوج * src, مزدوج * dst, صحيح len, صحيح power)
    {
        صحيح i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        لكل ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_بارول(src + i), v_b2 = vx_بارول(src + i + VTraits<v_float64>::vlanes());
            صحيح p = std::مطلق_قیمت(power);
            إذا( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            طالما( p > 1 )
            {
                إذا (p & 1)
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

        أرجع i;
    }
};
#د_که_پای

#د_که_پای

کينډۍ<ډول_نوم T, ډول_نوم WT>
ساكن فراغ
iPow_i( ثابت فراغ* _src, فراغ* _dst, صحيح len, صحيح power )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    إذا( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        لكل( صحيح i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    وإلا
    {
        iPow_SIMD<T, WT> vop;
        صحيح i = vop(src, dst, len, power);

        لكل( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            صحيح p = power;
            طالما( p > 1 )
            {
                إذا( p & 1 )
                    a *= b;
                b *= b;
                p >>= 1;
            }

            a *= b;
            dst[i] = saturate_cast<T>(a);
        }
    }
}

کينډۍ<ډول_نوم T>
ساكن فراغ
iPow_f( ثابت فراغ* _src, فراغ* _dst, صحيح len, صحيح power0 )
{
    ثابت T* src = (ثابت T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    صحيح i = vop(src, dst, len, power0);
    صحيح power = std::مطلق_قیمت(power0);

    لكل( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        صحيح p = power;
        إذا( power0 < 0 )
            b = 1/b;

        طالما( p > 1 )
        {
            إذا( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

تعريفنوع فراغ (*IPowFunc)( ثابت فراغ* src, فراغ* dst, صحيح len, صحيح power );

ساكن IPowFunc ipowTab[CV_ژوروالی_MAX] =
{
    iPow_i<uchar, بینښه>, iPow_i<schar, صحيح>, iPow_i<ushort, بینښه>, iPow_i<لنډ, صحيح>,
    iPow_i<صحيح, صحيح>, iPow_f<عائم>, iPow_f<مزدوج>, 0
};

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_pow(ننوت_Array _src, مزدوج power, OutputArray _dst,
                    منطقي is_ipower, صحيح ipower)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();
    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type),
            لیکې_فی_WI = d.isIntel() ? 4 : 1;
    منطقي doubleSupport = d.doubleFPConfig() > 0;

    _dst.create_Same_اندازه(_src, type);
    إذا (is_ipower)
    {
        إذا( ipower < 0 )
        {
            إذا( ژوروالی == CV_32F || ژوروالی == CV_64F )
                is_ipower = خطأ;
            وإلا
                أرجع خطأ;
        }
    }

    إذا (ژوروالی == CV_64F && !doubleSupport)
        أرجع خطأ;

    منطقي issqrt = std::مطلق_قیمت(power - 0.5) < DBL_EPSILON;
    ثابت حرف * ثابت op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    إذا (is_ipower)
    {
        صحيح wdepth = CV_32F;
        إذا (ژوروالی == CV_64F)
            wdepth = CV_64F;
        وإلا إذا (ژوروالی == CV_16F)
            wdepth = CV_16F;

        حرف cvt[2][50];
        extra_opts = format(
            " -D srcT1=%s -DsrcT1_C1=%s"
            " -D srcT2=int -D workST=int"
            " -D workT=%s -D wdepth=%d -D convertToWT1=%s"
            " -D convertToDT=%s"
            " -D workT1=%s",
            ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, 1)),
            ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, 1)),
            ocl::type_منزل_Str(CV_MAKE_TYPE(wdepth, 1)),
            wdepth,
            ocl::convertTypeStr(ژوروالی, wdepth, 1, cvt[0], حجم(cvt[0])),
            ocl::convertTypeStr(wdepth, ژوروالی, 1, cvt[1], حجم(cvt[1])),
            ocl::type_منزل_Str(wdepth)
        );
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D cn=%d -D dstT=%s -D dstT_C1=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D %s%s%s%s",
                         1,
                         ocl::type_منزل_Str(ژوروالی), ocl::type_منزل_Str(ژوروالی), ژوروالی, لیکې_فی_WI, op,
                         " -D UNARY_OP=1",
                         extra_opts.empty() ? "" : extra_opts.c_str(),
                         doubleSupport ? " -D DOUBLE_SUPPORT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat();
    _dst.create_Same_اندازه(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_اندازه(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    إذا (issqrt)
        k.دلیلونه(srcarg, dstarg);
    وإلا إذا (is_ipower)
        k.دلیلونه(srcarg, dstarg, ipower);
    وإلا
    {
        إذا (ژوروالی == CV_32F)
            k.دلیلونه(srcarg, dstarg, (عائم)power);
        وإلا
            k.دلیلونه(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.ستون_شمېر *  cn, ((size_t)dst.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
}

#د_که_پای

فراغ توان( ننوت_Array _src, مزدوج power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type),
            cn = CV_MAT_CN(type), ipower = cv_پړاو(power);
    منطقي is_ipower = fabs(ipower - power) < DBL_EPSILON;
#که_تعریف_شوی HAVE_OPENCL
    منطقي useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#د_که_پای

    إذا (is_ipower)
    {
        بدل( ipower )
        {
        حالة 0:
            _dst.create_Same_اندازه(_src, type);
            _dst.سېټ_منزل(Scalar::all(1));
            أرجع;
        حالة 1:
            _src.copy_منزل(_dst);
            أرجع;
        حالة 2:
            multiply(_src, _src, _dst);
            أرجع;
        }
    }

    CV_OCL_پرلپسې(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    ثابت منطقي float_ژوروالی = ژوروالی == CV_16F || ژوروالی == CV_16BF || ژوروالی == CV_32F || ژوروالی == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    إذا( is_ipower && !float_ژوروالی && ipowTab[ژوروالی] )
    {
        Mat src = _src.getMat();
        _dst.create_Same_اندازه(_src, type);
        Mat dst = _dst.getMat();

        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح len = (صحيح)(it.size*cn);
        IPowFunc func = ipowTab[ژوروالی];

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        أرجع;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.create_Same_اندازه(_src, type);
    Mat dst = _dst.getMat();
    إذا (src.empty())
        أرجع;

    ثابت size_t ټول = src.ټول() * cn;
    إذا (float_ژوروالی && src.isContinuous() && dst.isContinuous() && ټول <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(ژوروالی, ژوروالی);
        إذا (k.fptr)
        {
            مزدوج pvstore;                    // the broadcast exponent, stored as T
            فراغ* pv = &pvstore;
            بدل (ژوروالی)
            {
            حالة CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); اكسر;
            حالة CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); اكسر;
            حالة CV_32F:  *(عائم*)pv = (عائم)power; اكسر;
            افتراضي:      pvstore = power; اكسر;
            }
            ساكن ثابت مزدوج noparams[4] = {};
            k.fptr(src.معلومات, 0, 1, pv, 0, 0, تش_اشاره, 0, 0,
                   dst.معلومات, 0, (صحيح)ټول, 1, noparams, k.flags, k.userdata);
            أرجع;
        }
    }

    ew::TExpr prog;
    ثابت صحيح a = prog.جمع_کړه_ننوت(ژوروالی);
    ثابت صحيح c = prog.جمع_کړه_Const(ew::EW_ژوروالی_NONE, Scalar(power), 1);
    ثابت صحيح out = prog.جمع_کړه_Output(ژوروالی);
    prog.move_منزل_Output(prog.emit_دوګونی(ew::OP_POW, a, c, ژوروالی), out);
    prog.compile();
    ثابت Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

فراغ جذر(ننوت_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    إذا (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::توان(a, 0.5, b);
        أرجع;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

کينډۍ<صحيح cv_mat_type> بنية mat_type_assotiations{};

کينډۍ<> بنية mat_type_assotiations<CV_8U>
{
    تعريفنوع بینښه حرف type;
    ساكن ثابت type min_allowable = 0x0;
    ساكن ثابت type max_allowable = 0xFF;
};

کينډۍ<> بنية mat_type_assotiations<CV_8S>
{
    تعريفنوع نښهدار حرف type;
    ساكن ثابت type min_allowable = SCHAR_MIN;
    ساكن ثابت type max_allowable = SCHAR_MAX;
};

کينډۍ<> بنية mat_type_assotiations<CV_16U>
{
    تعريفنوع بینښه لنډ type;
    ساكن ثابت type min_allowable = 0x0;
    ساكن ثابت type max_allowable = USHRT_MAX;
};
کينډۍ<> بنية mat_type_assotiations<CV_16S>
{
    تعريفنوع نښهدار لنډ type;
    ساكن ثابت type min_allowable = SHRT_MIN;
    ساكن ثابت type max_allowable = SHRT_MAX;
};

کينډۍ<> بنية mat_type_assotiations<CV_32S>
{
    تعريفنوع صحيح type;
    ساكن ثابت type min_allowable = (-لوی_عدد - 1);
    ساكن ثابت type max_allowable = لوی_عدد;
};

کينډۍ<صحيح ژوروالی>
ساكن منطقي checkIntegerRange(cv::Mat src, ټکی& bad_pt, صحيح minVal, صحيح maxVal)
{
    تعريفنوع mat_type_assotiations<ژوروالی> type_ass;

    إذا (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        أرجع ریښتیا;
    }
    وإلا إذا (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::ټکی(0,0);
        أرجع خطأ;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    لكل (صحيح j = 0; j < as_one_channel.لیکې; ++j)
        لكل (صحيح i = 0; i < as_one_channel.ستون_شمېر; ++i)
        {
            ډول_نوم type_ass::type v = as_one_channel.at<ډول_نوم type_ass::type>(j ,i);
            إذا (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                أرجع خطأ;
            }
        }

    أرجع ریښتیا;
}

تعريفنوع منطقي (*check_range_function)(cv::Mat src, ټکی& bad_pt, صحيح minVal, صحيح maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

کينډۍ<ډول_نوم T>
ساكن فراغ checkTypedRange(Mat& src, ټکی& badPt, مزدوج minVal, مزدوج maxVal)
{
    صحيح loc = 0;
    صحيح cn = src.channels();
    اندازه size = getContinuousSize2D(src, cn);
    ثابت T* isrc = src.ptr<T>();
    size_t ګام = src.ګام / حجم(isrc[0]);

    لكل( ; badPt.x < 0 && size.لوړوالی--; loc += size.پلنوالی, isrc += ګام )
    {
        لكل( صحيح i = 0; i < size.پلنوالی; i++ )
        {
            مزدوج val = (مزدوج)isrc[i];
            إذا( !(val >= minVal && val < maxVal) )
            {
                صحيح pixelId = (loc + i) / cn;
                badPt = ټکی(pixelId % src.ستون_شمېر, pixelId / src.ستون_شمېر);
                اكسر;
            }
        }
    }
}

منطقي checkRange(ننوت_Array _src, منطقي quiet, ټکی* pt, مزدوج minVal, مزدوج maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    إذا ( src.dims > 2 )
    {
        CV_Assert(pt == عدم); // no way to provide location info

        ثابت Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        لكل ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا (!checkRange( it.planes[0], quiet, عدم, minVal, maxVal ))
            {
                أرجع خطأ;
            }
        }
        أرجع ریښتیا;
    }

    صحيح ژوروالی = src.ژوروالی();
    ټکی badPt(-1, -1);

    إذا (ژوروالی < CV_32F)
    {
        صحيح minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        صحيح maxVali = maxVal > لوی_عدد ? لوی_عدد : cvCeil(maxVal) - 1;

        (check_range_functions[ژوروالی])(src, badPt, minVali, maxVali);
    }
    وإلا
    {
        صحيح i, loc = 0;
        صحيح cn = src.channels();
        اندازه size = getContinuousSize2D(src, cn);

        إذا( ژوروالی == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        وإلا إذا( ژوروالی == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        وإلا إذا( ژوروالی == CV_32F )
        {
            Cv32suf a, b;
            صحيح ia, ib;
            ثابت صحيح* isrc = src.ptr<صحيح>();
            size_t ګام = src.ګام/حجم(isrc[0]);

            a.f = (عائم)std::max(minVal, (مزدوج)-FLT_MAX);
            b.f = (عائم)std::min(maxVal, (مزدوج)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            لكل( ; badPt.x < 0 && size.لوړوالی--; loc += size.پلنوالی, isrc += ګام )
            {
                لكل( i = 0; i < size.پلنوالی; i++ )
                {
                    صحيح val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    إذا( val < ia || val >= ib )
                    {
                        صحيح pixelId = (loc + i) / cn;
                        badPt = ټکی(pixelId % src.ستون_شمېر, pixelId / src.ستون_شمېر);
                        اكسر;
                    }
                }
            }
        }
        وإلا إذا( ژوروالی == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            ثابت int64* isrc = src.ptr<int64>();
            size_t ګام = src.ګام/حجم(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            لكل( ; badPt.x < 0 && size.لوړوالی--; loc += size.پلنوالی, isrc += ګام )
            {
                لكل( i = 0; i < size.پلنوالی; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    إذا( val < ia || val >= ib )
                    {
                        صحيح pixelId = (loc + i) / cn;
                        badPt = ټکی(pixelId % src.ستون_شمېر, pixelId / src.ستون_شمېر);
                        اكسر;
                    }
                }
            }
        }
        وإلا إذا( ژوروالی == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        وإلا إذا( ژوروالی == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        وإلا إذا( ژوروالی == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        وإلا إذا( ژوروالی == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        وإلا
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    إذا( badPt.x >= 0 )
    {
        إذا( pt )
            *pt = badPt;
        إذا( !quiet )
        {
            cv::String ارزښت_str;
            ارزښت_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::Sts_بهر_د_Range,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, ارزښت_str.c_str(), minVal, maxVal));
        }
        أرجع خطأ;
    }
    أرجع ریښتیا;
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

صحيح cv::solveCubic( ننوت_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    ثابت صحيح n0 = 3;
    Mat coeffs = _coeffs.getMat();
    صحيح ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == اندازه(n0, 1) ||
                coeffs.size() == اندازه(n0+1, 1) ||
                coeffs.size() == اندازه(1, n0) ||
                coeffs.size() == اندازه(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, ریښتیا, _OutputArray::ژوروالی_ماسک_FLT);
    Mat roots = _roots.getMat();

    صحيح i = -1, n = 0;
    مزدوج a0 = 1., a1, a2, a3;
    مزدوج x0 = 0., x1 = 0., x2 = 0.;
    صحيح ncoeffs = coeffs.لیکې + coeffs.ستون_شمېر - 1;

    إذا( ctype == CV_32FC1 )
    {
        إذا( ncoeffs == 4 )
            a0 = coeffs.at<عائم>(++i);

        a1 = coeffs.at<عائم>(i+1);
        a2 = coeffs.at<عائم>(i+2);
        a3 = coeffs.at<عائم>(i+3);
    }
    وإلا
    {
        إذا( ncoeffs == 4 )
            a0 = coeffs.at<مزدوج>(++i);

        a1 = coeffs.at<مزدوج>(i+1);
        a2 = coeffs.at<مزدوج>(i+2);
        a3 = coeffs.at<مزدوج>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    مزدوج max_coeff = std::max({std::مطلق_قیمت(a0), std::مطلق_قیمت(a1), std::مطلق_قیمت(a2), std::مطلق_قیمت(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    إذا (max_coeff < std::numeric_limits<مزدوج>::epsilon())
        أرجع -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    إذا( std::مطلق_قیمت(a0) < std::numeric_limits<مزدوج>::epsilon() * max_coeff )
    {
        إذا( std::مطلق_قیمت(a1) < std::numeric_limits<مزدوج>::epsilon() * max_coeff )
        {
            إذا( std::مطلق_قیمت(a2) < std::numeric_limits<مزدوج>::epsilon() * max_coeff )
                n = std::مطلق_قیمت(a3) < std::numeric_limits<مزدوج>::epsilon() * max_coeff ? -1 : 0;
            وإلا
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        وإلا
        {
            // quadratic equation
            مزدوج d = a2*a2 - 4*a1*a3;
            إذا( d >= 0 )
            {
                d = std::جذر(d);
                مزدوج q1 = (-a2 + d) * 0.5;
                مزدوج q2 = (a2 + d) * -0.5;
                إذا( std::مطلق_قیمت(q1) > std::مطلق_قیمت(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                وإلا
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    وإلا
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        مزدوج scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        مزدوج Q = (a1 * a1 - 3 * a2) * (1./9);
        مزدوج R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        مزدوج Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        مزدوج d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        إذا( d > 0 )
        {
            مزدوج theta = acos(R / جذر(Qcubed));
            مزدوج sqrtQ = جذر(Q);
            مزدوج t0 = -2 * sqrtQ;
            مزدوج t1 = theta * (1./3);
            مزدوج t2 = a1 * (1./3);
            x0 = t0 * کوساین(t1) - t2;
            x1 = t0 * کوساین(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * کوساین(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        وإلا إذا( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        وإلا
        {
            مزدوج e;
            d = جذر(-d);
            e = std::cbrt(d + fabs(R));
            إذا( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    إذا( roots.type() == CV_32FC1 )
    {
        roots.at<عائم>(0) = (عائم)x0;
        roots.at<عائم>(1) = (عائم)x1;
        roots.at<عائم>(2) = (عائم)x2;
    }
    وإلا
    {
        roots.at<مزدوج>(0) = x0;
        roots.at<مزدوج>(1) = x1;
        roots.at<مزدوج>(2) = x2;
    }

    أرجع n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
مزدوج cv::solvePoly( ننوت_Array _coeffs0, OutputArray _roots0, صحيح maxIters )
{
    CV_INSTRUMENT_REGION();

    تعريفنوع Complex<مزدوج> C;

    مزدوج maxDiff = 0;
    صحيح iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    صحيح ctype = _coeffs0.type();
    صحيح cdepth = CV_MAT_ژوروالی(ctype);

    CV_Assert( CV_MAT_ژوروالی(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.لیکې == 1 || coeffs0.ستون_شمېر == 1) && coeffs0.ټول() >= 2 );

    صحيح n0 = coeffs0.ستون_شمېر + coeffs0.لیکې - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, ریښتیا, _OutputArray::ژوروالی_ماسک_FLT);
    Mat roots0 = _roots0.getMat();

    Auto_بفر<C> buf(n*2+2);
    C *coeffs = buf.معلومات(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convert_منزل(coeffs1, coeffs1.type());
    إذا( coeffs0.channels() == 1 )
    {
        ثابت مزدوج* rcoeffs = (ثابت مزدوج*)roots;
        لكل( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    لكل( ; n > 1; n-- )
    {
        إذا( std::مطلق_قیمت(coeffs[n].re) + std::مطلق_قیمت(coeffs[n].im) > DBL_EPSILON )
            اكسر;
    }
    CV_Assert( std::مطلق_قیمت(coeffs[n].re) + std::مطلق_قیمت(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    لكل( i = 0; i <= n; i++ )
        absCoeffs.at<مزدوج>(i) = مطلق_قیمت(coeffs[i]);

    صحيح nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    لكل( i = 0; i <= n; i++ )
    {
        مزدوج coeff = absCoeffs.at<مزدوج>(i);
        إذا( coeff > DBL_EPSILON )
        {
            إذا( i != n )
                u.at<مزدوج>(i) = 2.0 * توان(coeff / absCoeffs.at<مزدوج>(n), 1.0 / (n - i));
            إذا( i != 0 )
                v.at<مزدوج>(i - 1) = 0.5 * توان(absCoeffs.at<مزدوج>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    مزدوج scale = 1;
    إذا( nonZeroCoeffs > 2 )
    {
        ټکی maxU, minV;
        minMaxLoc(u, تش_اشاره, تش_اشاره, تش_اشاره, &maxU);
        minMaxLoc(v, تش_اشاره, تش_اشاره, &minV);
        u.at<مزدوج>(maxU) = 0;
        v.at<مزدوج>(minV) = 0;
        scale = (مجموعه(u).val[0] + مجموعه(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(کوساین(CV_2PI / n), ساین(CV_2PI / n));

    لكل( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    لكل( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        لكل( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            صحيح عدد_same_ریښه = 1;
            لكل( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                إذا( j != i )
                {
                    إذا ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    وإلا
                        عدد_same_ریښه++;
                }
            }
            num /= denom;
            إذا( عدد_same_ریښه > 1)
            {
                مزدوج old_عدد_re = num.re;
                مزدوج old_عدد_im = num.im;
                صحيح مربع_ریښه_times = عدد_same_ریښه % 2 == 0 ? عدد_same_ریښه / 2 : عدد_same_ریښه / 2 - 1;

                لكل( j = 0; j < مربع_ریښه_times; j++)
                {
                    num.re = old_عدد_re*old_عدد_re + old_عدد_im*old_عدد_im;
                    num.re = جذر(num.re);
                    num.re += old_عدد_re;
                    num.im = num.re - old_عدد_re;
                    num.re /= 2;
                    num.re = جذر(num.re);

                    num.im /= 2;
                    num.im = جذر(num.im);
                    إذا( old_عدد_re < 0 ) num.im = -num.im;
                }

                إذا( عدد_same_ریښه % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<مزدوج>(3) = -(std::توان(old_عدد_re, 3));
                    cube_coefs.at<مزدوج>(2) = -(15*std::توان(old_عدد_re, 2) + 27*std::توان(old_عدد_im, 2));
                    cube_coefs.at<مزدوج>(1) = -48*old_عدد_re;
                    cube_coefs.at<مزدوج>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<مزدوج>(0));
                    num.im = جذر(std::توان(num.re, 2) / 3 - old_عدد_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::مطلق_قیمت(num));
        }
        إذا( maxDiff <= 0 )
            اكسر;
    }

    إذا( coeffs0.channels() == 1 )
    {
        ثابت مزدوج verySmallEps = 1e-100;
        لكل( i = 0; i < n; i++ )
            إذا( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    لكل( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convert_منزل(roots0, roots0.type());
    أرجع maxDiff;
}


/* End of file. */
