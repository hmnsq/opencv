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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ <atomic>
#అన్తర్భావ <limits>
#అన్తర్భావ <iostream>
#అన్తర్భావ <algorithm>
#అన్తర్భావ <cmath>
#అన్తర్భావ "mathfuncs.hpp"
#అన్తర్భావ "arithm_expr.hpp"      // the element-wise engine: getMathFunc + TExpr for cv::exp/log/sqrt

నామాకాశ cv
{

రకనిర్వచనం శూన్య (*MathFunc)(స్థిరరాశి శూన్య* src, శూన్య* dst, పూర్ణాంకం len);

#నిర్వచితమైతే HAVE_OPENCL

గణన { OCL_OP_LOG=0, OCL_OP_EXP=1, OCL_OP_MAG=2, OCL_OP_PHASE_DEGREES=3, OCL_OP_PHASE_RADIANS=4 };

స్థిర స్థిరరాశి అక్షరం* oclop2str[] = { "OP_LOG", "OP_EXP", "OP_MAG", "OP_PHASE_DEGREES", "OP_PHASE_RADIANS", 0 };

స్థిర తార్కిక ocl_math_op(ఆదానం_Array _src1, ఆదానం_Array _src2, OutputArray _dst, పూర్ణాంకం oclop)
{
    పూర్ణాంకం type = _src1.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type);
    పూర్ణాంకం kercn = oclop == OCL_OP_PHASE_DEGREES ||
            oclop == OCL_OP_PHASE_RADIANS ? 1 : ocl::predict_Optimal_సదిశ_వెడల్పు(_src1, _src2, _dst);

    స్థిరరాశి ocl::Device d = ocl::Device::getDefault();
    తార్కిక double_support = d.doubleFPConfig() > 0;
    ఒకవేళ (!double_support && లోతు == CV_64F)
        తిరిగి అబద్ధం;
    పూర్ణాంకం అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D %s -D %s -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d%s", _src2.empty() ? "UNARY_OP" : "BINARY_OP",
                         oclop2str[oclop], ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)), లోతు, అడ్డువరుసలు_చొప్పున_WI,
                         double_support ? " -D DOUBLE_SUPPORT" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    _dst.create_Same_కొలత(src1, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_కొలత(src1),
            src2arg = ocl::KernelArg::Read_Only_No_కొలత(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ఒకవేళ (src2.empty())
        k.వాదనలు(src1arg, dstarg);
    లేకపోతే
        k.వాదనలు(src1arg, src2arg, dstarg);

    size_t globalsize[] = { (size_t)src1.నిలువుల_సంఖ్య * cn / kercn, ((size_t)src1.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, 0, అబద్ధం);
}

#అయితే_ముగింపు

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski
   (http://www.worldserver.com/turk/computergraphics/papers.html)
\* ************************************************************************** */
CV_DISABLE_UBSAN దశాంశం cube_మూలం( దశాంశం value )
{
    CV_INSTRUMENT_REGION();

    దశాంశం fr;
    Cv32suf v, m;
    పూర్ణాంకం ix, s;
    పూర్ణాంకం ex, shx;

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
    fr = (దశాంశం)(((((45.2548339756803022511987494 * fr +
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
    తిరిగి v.f;
}

/****************************************************************************************\
*                                  Cartezian -> Polar                                    *
\****************************************************************************************/

శూన్య magnitude( ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst )
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం type = src1.type(), లోతు = src1.లోతు(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (లోతు == CV_32F || లోతు == CV_64F));

    CV_OCL_నిరంతరం(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, OCL_OP_MAG))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create(X.size, X.type());
    Mat Mag = dst.getMat();

    స్థిరరాశి Mat* arrays[] = {&X, &Y, &Mag, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం len = (పూర్ణాంకం)it.size*cn;

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ఒకవేళ( లోతు == CV_32F )
        {
            స్థిరరాశి దశాంశం *x = (స్థిరరాశి దశాంశం*)ptrs[0], *y = (స్థిరరాశి దశాంశం*)ptrs[1];
            దశాంశం *mag = (దశాంశం*)ptrs[2];
            hal::magnitude32f( x, y, mag, len );
        }
        లేకపోతే
        {
            స్థిరరాశి ద్విగుణ *x = (స్థిరరాశి ద్విగుణ*)ptrs[0], *y = (స్థిరరాశి ద్విగుణ*)ptrs[1];
            ద్విగుణ *mag = (ద్విగుణ*)ptrs[2];
            hal::magnitude64f( x, y, mag, len );
        }
    }
}

శూన్య phase( ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst, తార్కిక angle_లోపల_Degrees )
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం type = src1.type(), లోతు = src1.లోతు(), cn = src1.channels();
    CV_Assert( src1.size() == src2.size() && type == src2.type() && (లోతు == CV_32F || లోతు == CV_64F));

    CV_OCL_నిరంతరం(dst.isUMat() && src1.dims() <= 2 && src2.dims() <= 2,
               ocl_math_op(src1, src2, dst, angle_లోపల_Degrees ? OCL_OP_PHASE_DEGREES : OCL_OP_PHASE_RADIANS))

    Mat X = src1.getMat(), Y = src2.getMat();
    dst.create( X.size, type );
    Mat Angle = dst.getMat();

    స్థిరరాశి Mat* arrays[] = {&X, &Y, &Angle, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం j, మొత్తం = (పూర్ణాంకం)(it.size*cn), ఖండం_కొలత_2 = మొత్తం;
    size_t esz1 = X.elemSize1();
    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( j = 0; j < మొత్తం; j += ఖండం_కొలత_2 )
        {
            పూర్ణాంకం len = std::min(మొత్తం - j, ఖండం_కొలత_2);
            ఒకవేళ( లోతు == CV_32F )
            {
                స్థిరరాశి దశాంశం *x = (స్థిరరాశి దశాంశం*)ptrs[0], *y = (స్థిరరాశి దశాంశం*)ptrs[1];
                దశాంశం *angle = (దశాంశం*)ptrs[2];
                hal::fastAtan32f( y, x, angle, len, angle_లోపల_Degrees );
            }
            లేకపోతే
            {
                స్థిరరాశి ద్విగుణ *x = (స్థిరరాశి ద్విగుణ*)ptrs[0], *y = (స్థిరరాశి ద్విగుణ*)ptrs[1];
                ద్విగుణ *angle = (ద్విగుణ*)ptrs[2];
                hal::fastAtan64f(y, x, angle, len, angle_లోపల_Degrees);
            }
            ptrs[0] += len*esz1;
            ptrs[1] += len*esz1;
            ptrs[2] += len*esz1;
        }
    }
}

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_cart_గమ్యం_Polar( ఆదానం_Array _src1, ఆదానం_Array _src2,
                             OutputArray _dst1, OutputArray _dst2, తార్కిక angle_లోపల_Degrees )
{
    స్థిరరాశి ocl::Device & d = ocl::Device::getDefault();
    పూర్ణాంకం type = _src1.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type),
            అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;
    తార్కిక doubleSupport = d.doubleFPConfig() > 0;

    స్థిరరాశి తార్కిక _src1IsDstMag = (_src1.getObj() == _dst1.getObj());
    స్థిరరాశి తార్కిక _src1IsDstAngle = (_src1.getObj() == _dst2.getObj());
    స్థిరరాశి తార్కిక _src2IsDstMag = (_src2.getObj() == _dst1.getObj());
    స్థిరరాశి తార్కిక _src2IsDstAngle = (_src2.getObj() == _dst2.getObj());

    ఒకవేళ ( !(_src1.dims() <= 2 && _src2.dims() <= 2 &&
           (లోతు == CV_32F || లోతు == CV_64F) && type == _src2.type()) ||
         (లోతు == CV_64F && !doubleSupport) )
        తిరిగి అబద్ధం;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D OP_CTP_%s%s%s%s%s%s",
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, 1)), లోతు,
                         అడ్డువరుసలు_చొప్పున_WI, angle_లోపల_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstMag   ? " -D SRC1_IS_DST_MAG" : "",
                         _src1IsDstAngle ? " -D SRC1_IS_DST_ANGLE" : "",
                         _src2IsDstMag   ? " -D SRC2_IS_DST_MAG" : "",
                         _src2IsDstAngle ? " -D SRC2_IS_DST_ANGLE" : ""
                         ));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat();
    కొలత size = src1.size();
    CV_Assert( size == src2.size() );

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.వాదనలు(_src1IsDstMag || _src1IsDstAngle ? ocl::KernelArg::Read_Write_No_కొలత(src1) : ocl::KernelArg::Read_Only_No_కొలత(src1),
           _src2IsDstMag || _src2IsDstAngle ? ocl::KernelArg::Read_Write_No_కొలత(src2) : ocl::KernelArg::Read_Only_No_కొలత(src2),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_కొలత(dst2));

    size_t globalsize[2] = { (size_t)dst1.నిలువుల_సంఖ్య * cn, ((size_t)dst1.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

శూన్య cart_గమ్యం_Polar( ఆదానం_Array src1, ఆదానం_Array src2,
                  OutputArray dst1, OutputArray dst2, తార్కిక angle_లోపల_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    CV_OCL_నిరంతరం(dst1.isUMat() && dst2.isUMat(),
            ocl_cart_గమ్యం_Polar(src1, src2, dst1, dst2, angle_లోపల_Degrees))

    Mat X = src1.getMat(), Y = src2.getMat();
    పూర్ణాంకం type = X.type(), లోతు = X.లోతు(), cn = X.channels();
    CV_Assert( X.size == Y.size && type == Y.type() && (లోతు == CV_32F || లోతు == CV_64F));
    dst1.create( X.size, type );
    dst2.create( X.size, type );
    Mat Mag = dst1.getMat(), Angle = dst2.getMat();

    స్థిరరాశి Mat* arrays[] = {&X, &Y, &Mag, &Angle, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం j, మొత్తం = (పూర్ణాంకం)(it.size*cn), ఖండం_కొలత_2 = std::min(మొత్తం, ((ఖండం_కొలత+cn-1)/cn)*cn);
    size_t esz1 = X.elemSize1();

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( j = 0; j < మొత్తం; j += ఖండం_కొలత_2 )
        {
            పూర్ణాంకం len = std::min(మొత్తం - j, ఖండం_కొలత_2);
            ఒకవేళ( లోతు == CV_32F )
            {
                స్థిరరాశి దశాంశం *x = (స్థిరరాశి దశాంశం*)ptrs[0], *y = (స్థిరరాశి దశాంశం*)ptrs[1];
                దశాంశం *mag = (దశాంశం*)ptrs[2], *angle = (దశాంశం*)ptrs[3];
                hal::cart_గమ్యం_Polar32f( x, y, mag, angle, len, angle_లోపల_Degrees );
            }
            లేకపోతే
            {
                స్థిరరాశి ద్విగుణ *x = (స్థిరరాశి ద్విగుణ*)ptrs[0], *y = (స్థిరరాశి ద్విగుణ*)ptrs[1];
                ద్విగుణ *mag = (ద్విగుణ*)ptrs[2], *angle = (ద్విగుణ*)ptrs[3];
                hal::cart_గమ్యం_Polar64f(x, y, mag, angle, len, angle_లోపల_Degrees);
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

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_polar_గమ్యం_Cart( ఆదానం_Array _mag, ఆదానం_Array _angle,
                             OutputArray _dst1, OutputArray _dst2, తార్కిక angle_లోపల_Degrees )
{
    స్థిరరాశి ocl::Device & d = ocl::Device::getDefault();
    పూర్ణాంకం type = _angle.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type),
            అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;
    తార్కిక doubleSupport = d.doubleFPConfig() > 0;

    స్థిరరాశి తార్కిక _src1IsDstX = (_mag.getObj() == _dst1.getObj());
    స్థిరరాశి తార్కిక _src1IsDstY = (_mag.getObj() == _dst2.getObj());
    స్థిరరాశి తార్కిక _src2IsDstX = (_angle.getObj() == _dst1.getObj());
    స్థిరరాశి తార్కిక _src2IsDstY = (_angle.getObj() == _dst2.getObj());

    ఒకవేళ ( !doubleSupport && లోతు == CV_64F )
        తిరిగి అబద్ధం;

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D BINARY_OP -D OP_PTC_%s%s%s%s%s%s",
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, 1)), లోతు,
                         అడ్డువరుసలు_చొప్పున_WI,
                         angle_లోపల_Degrees ? "AD" : "AR",
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1IsDstX   ? " -D SRC1_IS_DST_X" : "",
                         _src1IsDstY ? " -D SRC1_IS_DST_Y" : "",
                         _src2IsDstX   ? " -D SRC2_IS_DST_X" : "",
                         _src2IsDstY ? " -D SRC2_IS_DST_Y" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat mag = _mag.getUMat(), angle = _angle.getUMat();
    కొలత size = angle.size();
    CV_Assert(mag.size() == size);

    _dst1.create(size, type);
    _dst2.create(size, type);
    UMat dst1 = _dst1.getUMat(), dst2 = _dst2.getUMat();

    k.వాదనలు(_src1IsDstX || _src1IsDstY ? ocl::KernelArg::Read_Write_No_కొలత(mag) : ocl::KernelArg::Read_Only_No_కొలత(mag),
           _src2IsDstX || _src2IsDstY  ? ocl::KernelArg::Read_Write_No_కొలత(angle) : ocl::KernelArg::Read_Only_No_కొలత(angle),
           ocl::KernelArg::WriteOnly(dst1, cn),
           ocl::KernelArg::Write_Only_No_కొలత(dst2));

    size_t globalsize[2] = { (size_t)dst1.నిలువుల_సంఖ్య * cn, ((size_t)dst1.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

శూన్య polar_గమ్యం_Cart( ఆదానం_Array src1, ఆదానం_Array src2,
                  OutputArray dst1, OutputArray dst2, తార్కిక angle_లోపల_Degrees )
{
    CV_INSTRUMENT_REGION();

    CV_Assert(dst1.getObj() != dst2.getObj());

    పూర్ణాంకం type = src2.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type);
    CV_Assert((లోతు == CV_32F || లోతు == CV_64F) && (src1.empty() || src1.type() == type));

    CV_OCL_నిరంతరం(!src1.empty() && src2.dims() <= 2 && dst1.isUMat() && dst2.isUMat(),
               ocl_polar_గమ్యం_Cart(src1, src2, dst1, dst2, angle_లోపల_Degrees))

    Mat Mag = src1.getMat(), Angle = src2.getMat();
    CV_Assert( Mag.empty() || Angle.size == Mag.size);
    dst1.create( Angle.size, type );
    dst2.create( Angle.size, type );
    Mat X = dst1.getMat(), Y = dst2.getMat();

    స్థిరరాశి Mat* arrays[] = {&Mag, &Angle, &X, &Y, 0};
    uchar* ptrs[4] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం j, మొత్తం = (పూర్ణాంకం)(it.size*cn), ఖండం_కొలత_2 = std::min(మొత్తం, ((ఖండం_కొలత+cn-1)/cn)*cn);
    size_t esz1 = Angle.elemSize1();

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( j = 0; j < మొత్తం; j += ఖండం_కొలత_2 )
        {
            పూర్ణాంకం len = std::min(మొత్తం - j, ఖండం_కొలత_2);
            ఒకవేళ ( లోతు == CV_32F )
            {
                స్థిరరాశి దశాంశం *mag = (స్థిరరాశి దశాంశం*)ptrs[0], *angle = (స్థిరరాశి దశాంశం*)ptrs[1];
                దశాంశం *x = (దశాంశం*)ptrs[2], *y = (దశాంశం*)ptrs[3];
                hal::polar_గమ్యం_Cart32f( mag, angle, x, y, len, angle_లోపల_Degrees );
            }
            లేకపోతే
            {
                స్థిరరాశి ద్విగుణ *mag = (స్థిరరాశి ద్విగుణ*)ptrs[0], *angle = (స్థిరరాశి ద్విగుణ*)ptrs[1];
                ద్విగుణ *x = (ద్విగుణ*)ptrs[2], *y = (ద్విగుణ*)ptrs[3];
                hal::polar_గమ్యం_Cart64f( mag, angle, x, y, len, angle_లోపల_Degrees );
            }

            ఒకవేళ( ptrs[0] )
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
గణన { MATH_OP_SMALL = 100000 };     // elements; tune with a benchmark if the crossover moves

స్థిర శూన్య math_op(ew::శిఖరం op, ఆదానం_Array _src, OutputArray _dst)
{
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type);
    CV_Assert(లోతు == CV_16F || లోతు == CV_16BF || లోతు == CV_32F || లోతు == CV_64F);

    Mat src = _src.getMat();
    _dst.create_Same_కొలత(_src, type);   // whole-shape transfer (layout & future metadata included)
    Mat dst = _dst.getMat();
    ఒకవేళ (src.empty())
        తిరిగి;

    స్థిరరాశి size_t మొత్తం = src.మొత్తం() * src.channels();
    ఒకవేళ (src.isContinuous() && dst.isContinuous() && మొత్తం <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getMathFunc(op, లోతు);
        CV_Assert(k.fptr);
        స్థిర స్థిరరాశి ద్విగుణ noparams[4] = {};
        k.fptr(src.దత్తాంశం, 0, 1, శూన్యసూచిక, 0, 0, శూన్యసూచిక, 0, 0,
               dst.దత్తాంశం, 0, (పూర్ణాంకం)మొత్తం, 1, noparams, k.flags, k.userdata);
        తిరిగి;
    }

    ew::TExpr p;
    స్థిరరాశి పూర్ణాంకం a = p.కూడు_ఆదానం(లోతు);
    స్థిరరాశి పూర్ణాంకం out = p.కూడు_Output(లోతు);
    p.move_గమ్యం_Output(p.emitUnary(op, a, లోతు), out);
    p.compile();
    స్థిరరాశి Mat* inputs[] = { &src };
    p.exec(inputs, &dst);
}

శూన్య ఘాతాంకం( ఆదానం_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] పూర్ణాంకం లోతు = _src.లోతు();   // consumed by CV_OCL_RUN only

    CV_OCL_నిరంతరం(_dst.isUMat() && _src.dims() <= 2 && (లోతు == CV_32F || లోతు == CV_64F),
               ocl_math_op(_src, noArray(), _dst, OCL_OP_EXP))

    math_op(ew::OP_EXP, _src, _dst);
}


/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

శూన్య సంవర్గమానం( ఆదానం_Array _src, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    [[maybe_unused]] పూర్ణాంకం లోతు = _src.లోతు();   // consumed by CV_OCL_RUN only

    CV_OCL_నిరంతరం( _dst.isUMat() && _src.dims() <= 2 && (లోతు == CV_32F || లోతు == CV_64F),
                ocl_math_op(_src, noArray(), _dst, OCL_OP_LOG))

    math_op(ew::OP_LOG, _src, _dst);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

మూస <రకం_పేరు T, రకం_పేరు WT>
నిర్మాణం iPow_SIMD
{
    పూర్ణాంకం క్రియాకారకం() ( స్థిరరాశి T *, T *, పూర్ణాంకం, పూర్ణాంకం)
    {
        తిరిగి 0;
    }
};

#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)

మూస <>
నిర్మాణం iPow_SIMD<uchar, గుర్తులేని>
{
    పూర్ణాంకం క్రియాకారకం() ( స్థిరరాశి uchar * src, uchar * dst, పూర్ణాంకం len, పూర్ణాంకం power )
    {
        పూర్ణాంకం i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        కొరకు ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_ఎక్కించు_expand(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            పూర్ణాంకం p = power;

            అయితే( p > 1 )
            {
                ఒకవేళ (p & 1)
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

        తిరిగి i;
    }
};

మూస <>
నిర్మాణం iPow_SIMD<schar, పూర్ణాంకం>
{
    పూర్ణాంకం క్రియాకారకం() ( స్థిరరాశి schar * src, schar * dst, పూర్ణాంకం len, పూర్ణాంకం power)
    {
        పూర్ణాంకం i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        కొరకు ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_ఎక్కించు_expand(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            పూర్ణాంకం p = power;

            అయితే( p > 1 )
            {
                ఒకవేళ (p & 1)
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

        తిరిగి i;
    }
};

మూస <>
నిర్మాణం iPow_SIMD<ushort, గుర్తులేని>
{
    పూర్ణాంకం క్రియాకారకం() ( స్థిరరాశి ushort * src, ushort * dst, పూర్ణాంకం len, పూర్ణాంకం power)
    {
        పూర్ణాంకం i = 0;
        v_uint32 v_1 = vx_setall_u32(1u);

        కొరకు ( ; i <= len - VTraits<v_uint16>::vlanes(); i += VTraits<v_uint16>::vlanes())
        {
            v_uint32 v_a1 = v_1, v_a2 = v_1;
            v_uint16 v = vx_ఎక్కించు(src + i);
            v_uint32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            పూర్ణాంకం p = power;

            అయితే( p > 1 )
            {
                ఒకవేళ (p & 1)
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

        తిరిగి i;
    }
};

మూస <>
నిర్మాణం iPow_SIMD<పొట్టి, పూర్ణాంకం>
{
    పూర్ణాంకం క్రియాకారకం() ( స్థిరరాశి పొట్టి * src, పొట్టి * dst, పూర్ణాంకం len, పూర్ణాంకం power)
    {
        పూర్ణాంకం i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        కొరకు ( ; i <= len - VTraits<v_int16>::vlanes(); i += VTraits<v_int16>::vlanes())
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int16 v = vx_ఎక్కించు(src + i);
            v_int32 v_b1, v_b2;
            v_expand(v, v_b1, v_b2);
            పూర్ణాంకం p = power;

            అయితే( p > 1 )
            {
                ఒకవేళ (p & 1)
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

        తిరిగి i;
    }
};

మూస <>
నిర్మాణం iPow_SIMD<పూర్ణాంకం, పూర్ణాంకం>
{
    పూర్ణాంకం క్రియాకారకం() ( స్థిరరాశి పూర్ణాంకం * src, పూర్ణాంకం * dst, పూర్ణాంకం len, పూర్ణాంకం power)
    {
        పూర్ణాంకం i = 0;
        v_int32 v_1 = vx_setall_s32(1);

        కొరకు ( ; i <= len - VTraits<v_int32>::vlanes()*2; i += VTraits<v_int32>::vlanes()*2)
        {
            v_int32 v_a1 = v_1, v_a2 = v_1;
            v_int32 v_b1 = vx_ఎక్కించు(src + i), v_b2 = vx_ఎక్కించు(src + i + VTraits<v_int32>::vlanes());
            పూర్ణాంకం p = power;

            అయితే( p > 1 )
            {
                ఒకవేళ (p & 1)
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

        తిరిగి i;
    }
};

మూస <>
నిర్మాణం iPow_SIMD<దశాంశం, దశాంశం>
{
    పూర్ణాంకం క్రియాకారకం() ( స్థిరరాశి దశాంశం * src, దశాంశం * dst, పూర్ణాంకం len, పూర్ణాంకం power)
    {
        పూర్ణాంకం i = 0;
        v_float32 v_1 = vx_setall_f32(1.f);

        కొరకు ( ; i <= len - VTraits<v_float32>::vlanes()*2; i += VTraits<v_float32>::vlanes()*2)
        {
            v_float32 v_a1 = v_1, v_a2 = v_1;
            v_float32 v_b1 = vx_ఎక్కించు(src + i), v_b2 = vx_ఎక్కించు(src + i + VTraits<v_float32>::vlanes());
            పూర్ణాంకం p = std::నిరపేక్ష(power);
            ఒకవేళ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            అయితే( p > 1 )
            {
                ఒకవేళ (p & 1)
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

        తిరిగి i;
    }
};

#ఒకవేళ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
మూస <>
నిర్మాణం iPow_SIMD<ద్విగుణ, ద్విగుణ>
{
    పూర్ణాంకం క్రియాకారకం() ( స్థిరరాశి ద్విగుణ * src, ద్విగుణ * dst, పూర్ణాంకం len, పూర్ణాంకం power)
    {
        పూర్ణాంకం i = 0;
        v_float64 v_1 = vx_setall_f64(1.);

        కొరకు ( ; i <= len - VTraits<v_float64>::vlanes()*2; i += VTraits<v_float64>::vlanes()*2)
        {
            v_float64 v_a1 = v_1, v_a2 = v_1;
            v_float64 v_b1 = vx_ఎక్కించు(src + i), v_b2 = vx_ఎక్కించు(src + i + VTraits<v_float64>::vlanes());
            పూర్ణాంకం p = std::నిరపేక్ష(power);
            ఒకవేళ( power < 0 )
            {
                v_b1 = v_div(v_1, v_b1);
                v_b2 = v_div(v_1, v_b2);
            }

            అయితే( p > 1 )
            {
                ఒకవేళ (p & 1)
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

        తిరిగి i;
    }
};
#అయితే_ముగింపు

#అయితే_ముగింపు

మూస<రకం_పేరు T, రకం_పేరు WT>
స్థిర శూన్య
iPow_i( స్థిరరాశి శూన్య* _src, శూన్య* _dst, పూర్ణాంకం len, పూర్ణాంకం power )
{
    స్థిరరాశి T* src = (స్థిరరాశి T*)_src;
    T* dst = (T*)_dst;
    ఒకవేళ( power < 0 )
    {
        T tab[5] =
        {
            saturate_cast<T>(power == -1 ? -1 : 0), saturate_cast<T>((power & 1) ? -1 : 1),
            std::numeric_limits<T>::max(), 1, saturate_cast<T>(power == -1 ? 1 : 0)
        };
        కొరకు( పూర్ణాంకం i = 0; i < len; i++ )
        {
            T val = src[i];
            dst[i] = cv_abs(val) <= 2 ? tab[val + 2] : (T)0;
        }
    }
    లేకపోతే
    {
        iPow_SIMD<T, WT> vop;
        పూర్ణాంకం i = vop(src, dst, len, power);

        కొరకు( ; i < len; i++ )
        {
            WT a = 1, b = src[i];
            పూర్ణాంకం p = power;
            అయితే( p > 1 )
            {
                ఒకవేళ( p & 1 )
                    a *= b;
                b *= b;
                p >>= 1;
            }

            a *= b;
            dst[i] = saturate_cast<T>(a);
        }
    }
}

మూస<రకం_పేరు T>
స్థిర శూన్య
iPow_f( స్థిరరాశి శూన్య* _src, శూన్య* _dst, పూర్ణాంకం len, పూర్ణాంకం power0 )
{
    స్థిరరాశి T* src = (స్థిరరాశి T*)_src;
    T* dst = (T*)_dst;
    iPow_SIMD<T, T> vop;
    పూర్ణాంకం i = vop(src, dst, len, power0);
    పూర్ణాంకం power = std::నిరపేక్ష(power0);

    కొరకు( ; i < len; i++ )
    {
        T a = 1, b = src[i];
        పూర్ణాంకం p = power;
        ఒకవేళ( power0 < 0 )
            b = 1/b;

        అయితే( p > 1 )
        {
            ఒకవేళ( p & 1 )
                a *= b;
            b *= b;
            p >>= 1;
        }

        a *= b;
        dst[i] = a;
    }
}

రకనిర్వచనం శూన్య (*IPowFunc)( స్థిరరాశి శూన్య* src, శూన్య* dst, పూర్ణాంకం len, పూర్ణాంకం power );

స్థిర IPowFunc ipowTab[CV_లోతు_MAX] =
{
    iPow_i<uchar, గుర్తులేని>, iPow_i<schar, పూర్ణాంకం>, iPow_i<ushort, గుర్తులేని>, iPow_i<పొట్టి, పూర్ణాంకం>,
    iPow_i<పూర్ణాంకం, పూర్ణాంకం>, iPow_f<దశాంశం>, iPow_f<ద్విగుణ>, 0
};

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_pow(ఆదానం_Array _src, ద్విగుణ power, OutputArray _dst,
                    తార్కిక is_ipower, పూర్ణాంకం ipower)
{
    స్థిరరాశి ocl::Device & d = ocl::Device::getDefault();
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type),
            అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;
    తార్కిక doubleSupport = d.doubleFPConfig() > 0;

    _dst.create_Same_కొలత(_src, type);
    ఒకవేళ (is_ipower)
    {
        ఒకవేళ( ipower < 0 )
        {
            ఒకవేళ( లోతు == CV_32F || లోతు == CV_64F )
                is_ipower = అబద్ధం;
            లేకపోతే
                తిరిగి అబద్ధం;
        }
    }

    ఒకవేళ (లోతు == CV_64F && !doubleSupport)
        తిరిగి అబద్ధం;

    తార్కిక issqrt = std::నిరపేక్ష(power - 0.5) < DBL_EPSILON;
    స్థిరరాశి అక్షరం * స్థిరరాశి op = issqrt ? "OP_SQRT" : is_ipower ? "OP_POWN" : "OP_POW";

    // Note: channels are unrolled

    std::string extra_opts ="";
    ఒకవేళ (is_ipower)
    {
        పూర్ణాంకం wdepth = CV_32F;
        ఒకవేళ (లోతు == CV_64F)
            wdepth = CV_64F;
        లేకపోతే ఒకవేళ (లోతు == CV_16F)
            wdepth = CV_16F;

        అక్షరం cvt[2][50];
        extra_opts = format(
            " -D srcT1=%s -DsrcT1_C1=%s"
            " -D srcT2=int -D workST=int"
            " -D workT=%s -D wdepth=%d -D convertToWT1=%s"
            " -D convertToDT=%s"
            " -D workT1=%s",
            ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, 1)),
            ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, 1)),
            ocl::type_గమ్యం_Str(CV_MAKE_TYPE(wdepth, 1)),
            wdepth,
            ocl::convertTypeStr(లోతు, wdepth, 1, cvt[0], పరిమాణం(cvt[0])),
            ocl::convertTypeStr(wdepth, లోతు, 1, cvt[1], పరిమాణం(cvt[1])),
            ocl::type_గమ్యం_Str(wdepth)
        );
    }

    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D cn=%d -D dstT=%s -D dstT_C1=%s -D DEPTH_dst=%d -D rowsPerWI=%d -D %s%s%s%s",
                         1,
                         ocl::type_గమ్యం_Str(లోతు), ocl::type_గమ్యం_Str(లోతు), లోతు, అడ్డువరుసలు_చొప్పున_WI, op,
                         " -D UNARY_OP=1",
                         extra_opts.empty() ? "" : extra_opts.c_str(),
                         doubleSupport ? " -D DOUBLE_SUPPORT" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat();
    _dst.create_Same_కొలత(src, type);
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_కొలత(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn);

    ఒకవేళ (issqrt)
        k.వాదనలు(srcarg, dstarg);
    లేకపోతే ఒకవేళ (is_ipower)
        k.వాదనలు(srcarg, dstarg, ipower);
    లేకపోతే
    {
        ఒకవేళ (లోతు == CV_32F)
            k.వాదనలు(srcarg, dstarg, (దశాంశం)power);
        లేకపోతే
            k.వాదనలు(srcarg, dstarg, power);
    }

    size_t globalsize[2] = { (size_t)dst.నిలువుల_సంఖ్య *  cn, ((size_t)dst.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

శూన్య ఘాతం( ఆదానం_Array _src, ద్విగుణ power, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type),
            cn = CV_MAT_CN(type), ipower = cv_విడత(power);
    తార్కిక is_ipower = fabs(ipower - power) < DBL_EPSILON;
#నిర్వచితమైతే HAVE_OPENCL
    తార్కిక useOpenCL = _dst.isUMat() && _src.dims() <= 2;
#అయితే_ముగింపు

    ఒకవేళ (is_ipower)
    {
        ఎంపిక( ipower )
        {
        స్థితి 0:
            _dst.create_Same_కొలత(_src, type);
            _dst.సమితి_గమ్యం(Scalar::all(1));
            తిరిగి;
        స్థితి 1:
            _src.copy_గమ్యం(_dst);
            తిరిగి;
        స్థితి 2:
            multiply(_src, _src, _dst);
            తిరిగి;
        }
    }

    CV_OCL_నిరంతరం(useOpenCL, ocl_pow(_src, power, _dst, is_ipower, ipower))

    స్థిరరాశి తార్కిక float_లోతు = లోతు == CV_16F || లోతు == CV_16BF || లోతు == CV_32F || లోతు == CV_64F;

    // INTEGER array ** INTEGER power: keep the classic iPow kernels (an exact multiply chain with
    // the classic wrap-around semantics) - full bit-exact compatibility for whoever relies on it.
    // Everything else - any power on a float array, a fractional power on an integer one (computed
    // in the float domain and saturated back), plus the 32U/64-bit depths iPow never supported -
    // goes through the engine below.
    ఒకవేళ( is_ipower && !float_లోతు && ipowTab[లోతు] )
    {
        Mat src = _src.getMat();
        _dst.create_Same_కొలత(_src, type);
        Mat dst = _dst.getMat();

        స్థిరరాశి Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        పూర్ణాంకం len = (పూర్ణాంకం)(it.size*cn);
        IPowFunc func = ipowTab[లోతు];

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], ptrs[1], len, ipower );
        తిరిగి;
    }

    // The engine path, two tiers like math_op: the pow kernel special-cases the exponents
    // 3/0.5 (2/1/0 never reach here) per row and vectorizes the general exp(p*log x) with an exact
    // std::pow patch for non-positive bases (0^negative -> inf, negative^fractional -> NaN).
    Mat src = _src.getMat();
    _dst.create_Same_కొలత(_src, type);
    Mat dst = _dst.getMat();
    ఒకవేళ (src.empty())
        తిరిగి;

    స్థిరరాశి size_t మొత్తం = src.మొత్తం() * cn;
    ఒకవేళ (float_లోతు && src.isContinuous() && dst.isContinuous() && మొత్తం <= (size_t)MATH_OP_SMALL)
    {
        ew::TKernel k = ew::getPowFunc(లోతు, లోతు);
        ఒకవేళ (k.fptr)
        {
            ద్విగుణ pvstore;                    // the broadcast exponent, stored as T
            శూన్య* pv = &pvstore;
            ఎంపిక (లోతు)
            {
            స్థితి CV_16F:  *(hfloat*)pv = saturate_cast<hfloat>(power); ఆపు;
            స్థితి CV_16BF: *(bfloat*)pv = saturate_cast<bfloat>(power); ఆపు;
            స్థితి CV_32F:  *(దశాంశం*)pv = (దశాంశం)power; ఆపు;
            అప్రమేయ:      pvstore = power; ఆపు;
            }
            స్థిర స్థిరరాశి ద్విగుణ noparams[4] = {};
            k.fptr(src.దత్తాంశం, 0, 1, pv, 0, 0, శూన్యసూచిక, 0, 0,
                   dst.దత్తాంశం, 0, (పూర్ణాంకం)మొత్తం, 1, noparams, k.flags, k.userdata);
            తిరిగి;
        }
    }

    ew::TExpr prog;
    స్థిరరాశి పూర్ణాంకం a = prog.కూడు_ఆదానం(లోతు);
    స్థిరరాశి పూర్ణాంకం c = prog.కూడు_Const(ew::EW_లోతు_NONE, Scalar(power), 1);
    స్థిరరాశి పూర్ణాంకం out = prog.కూడు_Output(లోతు);
    prog.move_గమ్యం_Output(prog.emit_ద్విమాన(ew::OP_POW, a, c, లోతు), out);
    prog.compile();
    స్థిరరాశి Mat* inputs[] = { &src };
    prog.exec(inputs, &dst);
}

శూన్య వర్గమూలం(ఆదానం_Array a, OutputArray b)
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (b.isUMat() && a.dims() <= 2)     // the OpenCL route (via ocl_pow) is unchanged
    {
        cv::ఘాతం(a, 0.5, b);
        తిరిగి;
    }
    math_op(ew::OP_SQRT, a, b);
}

/************************** CheckArray for NaN's, Inf's *********************************/

మూస<పూర్ణాంకం cv_mat_type> నిర్మాణం mat_type_assotiations{};

మూస<> నిర్మాణం mat_type_assotiations<CV_8U>
{
    రకనిర్వచనం గుర్తులేని అక్షరం type;
    స్థిర స్థిరరాశి type min_allowable = 0x0;
    స్థిర స్థిరరాశి type max_allowable = 0xFF;
};

మూస<> నిర్మాణం mat_type_assotiations<CV_8S>
{
    రకనిర్వచనం గుర్తుగల అక్షరం type;
    స్థిర స్థిరరాశి type min_allowable = SCHAR_MIN;
    స్థిర స్థిరరాశి type max_allowable = SCHAR_MAX;
};

మూస<> నిర్మాణం mat_type_assotiations<CV_16U>
{
    రకనిర్వచనం గుర్తులేని పొట్టి type;
    స్థిర స్థిరరాశి type min_allowable = 0x0;
    స్థిర స్థిరరాశి type max_allowable = USHRT_MAX;
};
మూస<> నిర్మాణం mat_type_assotiations<CV_16S>
{
    రకనిర్వచనం గుర్తుగల పొట్టి type;
    స్థిర స్థిరరాశి type min_allowable = SHRT_MIN;
    స్థిర స్థిరరాశి type max_allowable = SHRT_MAX;
};

మూస<> నిర్మాణం mat_type_assotiations<CV_32S>
{
    రకనిర్వచనం పూర్ణాంకం type;
    స్థిర స్థిరరాశి type min_allowable = (-గరిష్ఠ_పూర్ణాంకం - 1);
    స్థిర స్థిరరాశి type max_allowable = గరిష్ఠ_పూర్ణాంకం;
};

మూస<పూర్ణాంకం లోతు>
స్థిర తార్కిక checkIntegerRange(cv::Mat src, బిందువు& bad_pt, పూర్ణాంకం minVal, పూర్ణాంకం maxVal)
{
    రకనిర్వచనం mat_type_assotiations<లోతు> type_ass;

    ఒకవేళ (minVal < type_ass::min_allowable && maxVal > type_ass::max_allowable)
    {
        తిరిగి నిజం;
    }
    లేకపోతే ఒకవేళ (minVal > type_ass::max_allowable || maxVal < type_ass::min_allowable || maxVal < minVal)
    {
        bad_pt = cv::బిందువు(0,0);
        తిరిగి అబద్ధం;
    }
    cv::Mat as_one_channel = src.reshape(1,0);

    కొరకు (పూర్ణాంకం j = 0; j < as_one_channel.అడ్డువరుసలు; ++j)
        కొరకు (పూర్ణాంకం i = 0; i < as_one_channel.నిలువుల_సంఖ్య; ++i)
        {
            రకం_పేరు type_ass::type v = as_one_channel.at<రకం_పేరు type_ass::type>(j ,i);
            ఒకవేళ (v < minVal || v > maxVal)
            {
                bad_pt.y = j;
                bad_pt.x = i / src.channels();
                తిరిగి అబద్ధం;
            }
        }

    తిరిగి నిజం;
}

రకనిర్వచనం తార్కిక (*check_range_function)(cv::Mat src, బిందువు& bad_pt, పూర్ణాంకం minVal, పూర్ణాంకం maxVal);

check_range_function check_range_functions[] =
{
    &checkIntegerRange<CV_8U>,
    &checkIntegerRange<CV_8S>,
    &checkIntegerRange<CV_16U>,
    &checkIntegerRange<CV_16S>,
    &checkIntegerRange<CV_32S>
};

మూస<రకం_పేరు T>
స్థిర శూన్య checkTypedRange(Mat& src, బిందువు& badPt, ద్విగుణ minVal, ద్విగుణ maxVal)
{
    పూర్ణాంకం loc = 0;
    పూర్ణాంకం cn = src.channels();
    కొలత size = getContinuousSize2D(src, cn);
    స్థిరరాశి T* isrc = src.ptr<T>();
    size_t అడుగు = src.అడుగు / పరిమాణం(isrc[0]);

    కొరకు( ; badPt.x < 0 && size.ఎత్తు--; loc += size.వెడల్పు, isrc += అడుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < size.వెడల్పు; i++ )
        {
            ద్విగుణ val = (ద్విగుణ)isrc[i];
            ఒకవేళ( !(val >= minVal && val < maxVal) )
            {
                పూర్ణాంకం pixelId = (loc + i) / cn;
                badPt = బిందువు(pixelId % src.నిలువుల_సంఖ్య, pixelId / src.నిలువుల_సంఖ్య);
                ఆపు;
            }
        }
    }
}

తార్కిక checkRange(ఆదానం_Array _src, తార్కిక quiet, బిందువు* pt, ద్విగుణ minVal, ద్విగుణ maxVal)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();

    ఒకవేళ ( src.dims > 2 )
    {
        CV_Assert(pt == ఖాళీ); // no way to provide location info

        స్థిరరాశి Mat* arrays[] = {&src, 0};
        Mat planes[1];
        NAryMatIterator it(arrays, planes);

        కొరకు ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ఒకవేళ (!checkRange( it.planes[0], quiet, ఖాళీ, minVal, maxVal ))
            {
                తిరిగి అబద్ధం;
            }
        }
        తిరిగి నిజం;
    }

    పూర్ణాంకం లోతు = src.లోతు();
    బిందువు badPt(-1, -1);

    ఒకవేళ (లోతు < CV_32F)
    {
        పూర్ణాంకం minVali = minVal <= INT_MIN ? INT_MIN : cvFloor(minVal);
        పూర్ణాంకం maxVali = maxVal > గరిష్ఠ_పూర్ణాంకం ? గరిష్ఠ_పూర్ణాంకం : cvCeil(maxVal) - 1;

        (check_range_functions[లోతు])(src, badPt, minVali, maxVali);
    }
    లేకపోతే
    {
        పూర్ణాంకం i, loc = 0;
        పూర్ణాంకం cn = src.channels();
        కొలత size = getContinuousSize2D(src, cn);

        ఒకవేళ( లోతు == CV_16F )
        {
            checkTypedRange<hfloat>(src, badPt, minVal, maxVal);
        }
        లేకపోతే ఒకవేళ( లోతు == CV_16BF )
        {
            checkTypedRange<bfloat>(src, badPt, minVal, maxVal);
        }
        లేకపోతే ఒకవేళ( లోతు == CV_32F )
        {
            Cv32suf a, b;
            పూర్ణాంకం ia, ib;
            స్థిరరాశి పూర్ణాంకం* isrc = src.ptr<పూర్ణాంకం>();
            size_t అడుగు = src.అడుగు/పరిమాణం(isrc[0]);

            a.f = (దశాంశం)std::max(minVal, (ద్విగుణ)-FLT_MAX);
            b.f = (దశాంశం)std::min(maxVal, (ద్విగుణ)FLT_MAX);

            ia = CV_TOGGLE_FLT(a.i);
            ib = CV_TOGGLE_FLT(b.i);

            కొరకు( ; badPt.x < 0 && size.ఎత్తు--; loc += size.వెడల్పు, isrc += అడుగు )
            {
                కొరకు( i = 0; i < size.వెడల్పు; i++ )
                {
                    పూర్ణాంకం val = isrc[i];
                    val = CV_TOGGLE_FLT(val);

                    ఒకవేళ( val < ia || val >= ib )
                    {
                        పూర్ణాంకం pixelId = (loc + i) / cn;
                        badPt = బిందువు(pixelId % src.నిలువుల_సంఖ్య, pixelId / src.నిలువుల_సంఖ్య);
                        ఆపు;
                    }
                }
            }
        }
        లేకపోతే ఒకవేళ( లోతు == CV_64F )
        {
            Cv64suf a, b;
            int64 ia, ib;
            స్థిరరాశి int64* isrc = src.ptr<int64>();
            size_t అడుగు = src.అడుగు/పరిమాణం(isrc[0]);

            a.f = minVal;
            b.f = maxVal;

            ia = CV_TOGGLE_DBL(a.i);
            ib = CV_TOGGLE_DBL(b.i);

            కొరకు( ; badPt.x < 0 && size.ఎత్తు--; loc += size.వెడల్పు, isrc += అడుగు )
            {
                కొరకు( i = 0; i < size.వెడల్పు; i++ )
                {
                    int64 val = isrc[i];
                    val = CV_TOGGLE_DBL(val);

                    ఒకవేళ( val < ia || val >= ib )
                    {
                        పూర్ణాంకం pixelId = (loc + i) / cn;
                        badPt = బిందువు(pixelId % src.నిలువుల_సంఖ్య, pixelId / src.నిలువుల_సంఖ్య);
                        ఆపు;
                    }
                }
            }
        }
        లేకపోతే ఒకవేళ( లోతు == CV_Bool )
        {
            checkTypedRange<uchar>(src, badPt, minVal, maxVal);
        }
        లేకపోతే ఒకవేళ( లోతు == CV_32U )
        {
            checkTypedRange<uint32_t>(src, badPt, minVal, maxVal);
        }
        లేకపోతే ఒకవేళ( లోతు == CV_64U )
        {
            checkTypedRange<uint64_t>(src, badPt, minVal, maxVal);
        }
        లేకపోతే ఒకవేళ( లోతు == CV_64S )
        {
            checkTypedRange<int64_t>(src, badPt, minVal, maxVal);
        }
        లేకపోతే
        {
            CV_Error(Error::StsUnsupportedFormat, "Unsupported matrix depth in checkRange");
        }
    }

    ఒకవేళ( badPt.x >= 0 )
    {
        ఒకవేళ( pt )
            *pt = badPt;
        ఒకవేళ( !quiet )
        {
            cv::String విలువ_str;
            విలువ_str << src(cv::Range(badPt.y, badPt.y + 1), cv::Range(badPt.x, badPt.x + 1));
            CV_Error_( cv::Error::Sts_బహిర్గతం_యొక్క_Range,
            ("the value at (%d, %d)=%s is out of range [%f, %f)", badPt.x, badPt.y, విలువ_str.c_str(), minVal, maxVal));
        }
        తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
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

పూర్ణాంకం cv::solveCubic( ఆదానం_Array _coeffs, OutputArray _roots )
{
    CV_INSTRUMENT_REGION();

    స్థిరరాశి పూర్ణాంకం n0 = 3;
    Mat coeffs = _coeffs.getMat();
    పూర్ణాంకం ctype = coeffs.type();

    CV_Assert( ctype == CV_32F || ctype == CV_64F );
    CV_Assert( (coeffs.size() == కొలత(n0, 1) ||
                coeffs.size() == కొలత(n0+1, 1) ||
                coeffs.size() == కొలత(1, n0) ||
                coeffs.size() == కొలత(1, n0+1)) );

    _roots.create(n0, 1, ctype, -1, నిజం, _OutputArray::లోతు_ముసుగు_FLT);
    Mat roots = _roots.getMat();

    పూర్ణాంకం i = -1, n = 0;
    ద్విగుణ a0 = 1., a1, a2, a3;
    ద్విగుణ x0 = 0., x1 = 0., x2 = 0.;
    పూర్ణాంకం ncoeffs = coeffs.అడ్డువరుసలు + coeffs.నిలువుల_సంఖ్య - 1;

    ఒకవేళ( ctype == CV_32FC1 )
    {
        ఒకవేళ( ncoeffs == 4 )
            a0 = coeffs.at<దశాంశం>(++i);

        a1 = coeffs.at<దశాంశం>(i+1);
        a2 = coeffs.at<దశాంశం>(i+2);
        a3 = coeffs.at<దశాంశం>(i+3);
    }
    లేకపోతే
    {
        ఒకవేళ( ncoeffs == 4 )
            a0 = coeffs.at<ద్విగుణ>(++i);

        a1 = coeffs.at<ద్విగుణ>(i+1);
        a2 = coeffs.at<ద్విగుణ>(i+2);
        a3 = coeffs.at<ద్విగుణ>(i+3);
    }

    // Fix for Issue #27748: Normalize coefficients to avoid overflow/underflow
    // and correctly detect negligible cubic terms.
    ద్విగుణ max_coeff = std::max({std::నిరపేక్ష(a0), std::నిరపేక్ష(a1), std::నిరపేక్ష(a2), std::నిరపేక్ష(a3)});

    // If max_coeff is effectively zero, the equation is 0=0
    ఒకవేళ (max_coeff < std::numeric_limits<ద్విగుణ>::epsilon())
        తిరిగి -1;

    // Check if the cubic term is negligible relative to the largest coefficient
    ఒకవేళ( std::నిరపేక్ష(a0) < std::numeric_limits<ద్విగుణ>::epsilon() * max_coeff )
    {
        ఒకవేళ( std::నిరపేక్ష(a1) < std::numeric_limits<ద్విగుణ>::epsilon() * max_coeff )
        {
            ఒకవేళ( std::నిరపేక్ష(a2) < std::numeric_limits<ద్విగుణ>::epsilon() * max_coeff )
                n = std::నిరపేక్ష(a3) < std::numeric_limits<ద్విగుణ>::epsilon() * max_coeff ? -1 : 0;
            లేకపోతే
            {
                // linear equation
                x0 = -a3/a2;
                n = 1;
            }
        }
        లేకపోతే
        {
            // quadratic equation
            ద్విగుణ d = a2*a2 - 4*a1*a3;
            ఒకవేళ( d >= 0 )
            {
                d = std::వర్గమూలం(d);
                ద్విగుణ q1 = (-a2 + d) * 0.5;
                ద్విగుణ q2 = (a2 + d) * -0.5;
                ఒకవేళ( std::నిరపేక్ష(q1) > std::నిరపేక్ష(q2) )
                {
                    x0 = q1 / a1;
                    x1 = a3 / q1;
                }
                లేకపోతే
                {
                    x0 = q2 / a1;
                    x1 = a3 / q2;
                }
                n = d > 0 ? 2 : 1;
            }
        }
    }
    లేకపోతే
    {
        // cubic equation
        // Normalize coefficients in-place to prevent overflow/instability
        ద్విగుణ scale = 1.0 / max_coeff;
        a0 *= scale;
        a1 *= scale;
        a2 *= scale;
        a3 *= scale;

        a0 = 1./a0;
        a1 *= a0;
        a2 *= a0;
        a3 *= a0;

        ద్విగుణ Q = (a1 * a1 - 3 * a2) * (1./9);
        ద్విగుణ R = (a1 * (2 * a1 * a1 - 9 * a2) + 27 * a3) * (1./54);
        ద్విగుణ Qcubed = Q * Q * Q;

        /*
          Here we expand expression `Qcubed - R * R` for `d` variable
          to reduce common terms `a1^6 / 729` and `-a1^4 * a2 / 81`
          and thus decrease rounding error (in case of quite big coefficients).

          And then we additionally group terms to further reduce rounding error.
        */
        ద్విగుణ d = (a1 * a1 * (a2 * a2 - 4 * a1 * a3) + 2 * a2 * (9 * a1 * a3 - 2 * a2 * a2) - 27 * a3 * a3) * (1./108);

        ఒకవేళ( d > 0 )
        {
            ద్విగుణ theta = acos(R / వర్గమూలం(Qcubed));
            ద్విగుణ sqrtQ = వర్గమూలం(Q);
            ద్విగుణ t0 = -2 * sqrtQ;
            ద్విగుణ t1 = theta * (1./3);
            ద్విగుణ t2 = a1 * (1./3);
            x0 = t0 * కోజ్యా(t1) - t2;
            x1 = t0 * కోజ్యా(t1 + (2.*CV_PI/3)) - t2;
            x2 = t0 * కోజ్యా(t1 + (4.*CV_PI/3)) - t2;
            n = 3;
        }
        లేకపోతే ఒకవేళ( d == 0 )
        {
            x0 = -2*std::cbrt(R) - a1/3;
            x1 = std::cbrt(R) - a1/3;
            x2 = 0;
            n = x0 == x1 ? 1 : 2;
            x1 = x0 == x1 ? 0 : x1;
        }
        లేకపోతే
        {
            ద్విగుణ e;
            d = వర్గమూలం(-d);
            e = std::cbrt(d + fabs(R));
            ఒకవేళ( R > 0 )
                e = -e;
            x0 = (e + Q / e) - a1 * (1./3);
            n = 1;
        }
    }

    ఒకవేళ( roots.type() == CV_32FC1 )
    {
        roots.at<దశాంశం>(0) = (దశాంశం)x0;
        roots.at<దశాంశం>(1) = (దశాంశం)x1;
        roots.at<దశాంశం>(2) = (దశాంశం)x2;
    }
    లేకపోతే
    {
        roots.at<ద్విగుణ>(0) = x0;
        roots.at<ద్విగుణ>(1) = x1;
        roots.at<ద్విగుణ>(2) = x2;
    }

    తిరిగి n;
}

/* finds complex roots of a polynomial using Durand-Kerner method:
   http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method */
ద్విగుణ cv::solvePoly( ఆదానం_Array _coeffs0, OutputArray _roots0, పూర్ణాంకం maxIters )
{
    CV_INSTRUMENT_REGION();

    రకనిర్వచనం Complex<ద్విగుణ> C;

    ద్విగుణ maxDiff = 0;
    పూర్ణాంకం iter, i, j;
    Mat coeffs0 = _coeffs0.getMat();
    పూర్ణాంకం ctype = _coeffs0.type();
    పూర్ణాంకం cdepth = CV_MAT_లోతు(ctype);

    CV_Assert( CV_MAT_లోతు(ctype) >= CV_32F && CV_MAT_CN(ctype) <= 2 );
    CV_Assert( (coeffs0.అడ్డువరుసలు == 1 || coeffs0.నిలువుల_సంఖ్య == 1) && coeffs0.మొత్తం() >= 2 );

    పూర్ణాంకం n0 = coeffs0.నిలువుల_సంఖ్య + coeffs0.అడ్డువరుసలు - 2, n = n0;

    _roots0.create(n, 1, CV_MAKETYPE(cdepth, 2), -1, నిజం, _OutputArray::లోతు_ముసుగు_FLT);
    Mat roots0 = _roots0.getMat();

    Auto_బఫరు<C> buf(n*2+2);
    C *coeffs = buf.దత్తాంశం(), *roots = coeffs + n + 1;
    Mat coeffs1(coeffs0.size(), CV_MAKETYPE(CV_64F, coeffs0.channels()), coeffs0.channels() == 2 ? coeffs : roots);
    coeffs0.convert_గమ్యం(coeffs1, coeffs1.type());
    ఒకవేళ( coeffs0.channels() == 1 )
    {
        స్థిరరాశి ద్విగుణ* rcoeffs = (స్థిరరాశి ద్విగుణ*)roots;
        కొరకు( i = 0; i <= n; i++ )
            coeffs[i] = C(rcoeffs[i], 0);
    }

    కొరకు( ; n > 1; n-- )
    {
        ఒకవేళ( std::నిరపేక్ష(coeffs[n].re) + std::నిరపేక్ష(coeffs[n].im) > DBL_EPSILON )
            ఆపు;
    }
    CV_Assert( std::నిరపేక్ష(coeffs[n].re) + std::నిరపేక్ష(coeffs[n].im) > DBL_EPSILON );

    // Related issue: https://github.com/opencv/opencv/issues/23644,
    // This the initialization scheme of "Initial approximations in Durand-Kerner's root finding method" by Guggenheimer.
    // https://link.springer.com/article/10.1007/BF01935059
    // We put the initial points equidistantly on a circle on the complex plane. This code computes the circle radius as in the paper.
    Mat absCoeffs(n + 1, 1, CV_64F);
    కొరకు( i = 0; i <= n; i++ )
        absCoeffs.at<ద్విగుణ>(i) = నిరపేక్ష(coeffs[i]);

    పూర్ణాంకం nonZeroCoeffs = 0;
    Mat u(n, 1, CV_64F, Scalar(0)), v(n, 1, CV_64F, Scalar(0));
    కొరకు( i = 0; i <= n; i++ )
    {
        ద్విగుణ coeff = absCoeffs.at<ద్విగుణ>(i);
        ఒకవేళ( coeff > DBL_EPSILON )
        {
            ఒకవేళ( i != n )
                u.at<ద్విగుణ>(i) = 2.0 * ఘాతం(coeff / absCoeffs.at<ద్విగుణ>(n), 1.0 / (n - i));
            ఒకవేళ( i != 0 )
                v.at<ద్విగుణ>(i - 1) = 0.5 * ఘాతం(absCoeffs.at<ద్విగుణ>(0) / coeff, 1.0 / i);
            nonZeroCoeffs++;
        }
    }
    ద్విగుణ scale = 1;
    ఒకవేళ( nonZeroCoeffs > 2 )
    {
        బిందువు maxU, minV;
        minMaxLoc(u, శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక, &maxU);
        minMaxLoc(v, శూన్యసూచిక, శూన్యసూచిక, &minV);
        u.at<ద్విగుణ>(maxU) = 0;
        v.at<ద్విగుణ>(minV) = 0;
        scale = (కూడిక(u).val[0] + కూడిక(v).val[0]) / (2 * nonZeroCoeffs - 2);
    }

    C p(scale, 0), r(కోజ్యా(CV_2PI / n), జ్యా(CV_2PI / n));

    కొరకు( i = 0; i < n; i++ )
    {
        roots[i] = p;
        p = p * r;
    }

    maxIters = maxIters <= 0 ? 1000 : maxIters;
    కొరకు( iter = 0; iter < maxIters; iter++ )
    {
        maxDiff = 0;
        కొరకు( i = 0; i < n; i++ )
        {
            p = roots[i];
            C num = coeffs[n], denom = coeffs[n];
            పూర్ణాంకం అంకె_same_మూలం = 1;
            కొరకు( j = 0; j < n; j++ )
            {
                num = num*p + coeffs[n-j-1];
                ఒకవేళ( j != i )
                {
                    ఒకవేళ ( (p - roots[j]).re != 0 || (p - roots[j]).im != 0 )
                        denom = denom * (p - roots[j]);
                    లేకపోతే
                        అంకె_same_మూలం++;
                }
            }
            num /= denom;
            ఒకవేళ( అంకె_same_మూలం > 1)
            {
                ద్విగుణ old_అంకె_re = num.re;
                ద్విగుణ old_అంకె_im = num.im;
                పూర్ణాంకం వర్గం_మూలం_times = అంకె_same_మూలం % 2 == 0 ? అంకె_same_మూలం / 2 : అంకె_same_మూలం / 2 - 1;

                కొరకు( j = 0; j < వర్గం_మూలం_times; j++)
                {
                    num.re = old_అంకె_re*old_అంకె_re + old_అంకె_im*old_అంకె_im;
                    num.re = వర్గమూలం(num.re);
                    num.re += old_అంకె_re;
                    num.im = num.re - old_అంకె_re;
                    num.re /= 2;
                    num.re = వర్గమూలం(num.re);

                    num.im /= 2;
                    num.im = వర్గమూలం(num.im);
                    ఒకవేళ( old_అంకె_re < 0 ) num.im = -num.im;
                }

                ఒకవేళ( అంకె_same_మూలం % 2 != 0){
                    Mat cube_coefs(4, 1, CV_64FC1);
                    Mat cube_roots(3, 1, CV_64FC2);
                    cube_coefs.at<ద్విగుణ>(3) = -(std::ఘాతం(old_అంకె_re, 3));
                    cube_coefs.at<ద్విగుణ>(2) = -(15*std::ఘాతం(old_అంకె_re, 2) + 27*std::ఘాతం(old_అంకె_im, 2));
                    cube_coefs.at<ద్విగుణ>(1) = -48*old_అంకె_re;
                    cube_coefs.at<ద్విగుణ>(0) = 64;
                    solveCubic(cube_coefs, cube_roots);

                    num.re = std::cbrt(cube_roots.at<ద్విగుణ>(0));
                    num.im = వర్గమూలం(std::ఘాతం(num.re, 2) / 3 - old_అంకె_re / (3*num.re));
                }
            }
            roots[i] = p - num;
            maxDiff = std::max(maxDiff, cv::నిరపేక్ష(num));
        }
        ఒకవేళ( maxDiff <= 0 )
            ఆపు;
    }

    ఒకవేళ( coeffs0.channels() == 1 )
    {
        స్థిరరాశి ద్విగుణ verySmallEps = 1e-100;
        కొరకు( i = 0; i < n; i++ )
            ఒకవేళ( fabs(roots[i].im) < verySmallEps )
                roots[i].im = 0;
    }

    కొరకు( ; n < n0; n++ )
        roots[n+1] = roots[n];

    Mat(roots0.size(), CV_64FC2, roots).convert_గమ్యం(roots0, roots0.type());
    తిరిగి maxDiff;
}


/* End of file. */
