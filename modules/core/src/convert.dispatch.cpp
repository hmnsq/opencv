// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "convert.simd.hpp"
#شامل "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv {

نامخلا hal {
خالی cvt16f32f(ثابت hfloat* src, اعشاری* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
خالی cvt32f16f(ثابت اعشاری* src, hfloat* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
خالی cvt32f16bf(ثابت اعشاری* src, bfloat* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
خالی جوڑو_RNGBias32f(اعشاری* arr, ثابت اعشاری* scale_Bias_جوڑے, صحیح len, صحیح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(جوڑو_RNGBias32f, (arr, scale_Bias_جوڑے, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
خالی جوڑو_RNGBias64f(دوہرا* arr, ثابت دوہرا* scale_Bias_جوڑے, صحیح len, صحیح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(جوڑو_RNGBias64f, (arr, scale_Bias_جوڑے, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


ثنائی_Func getConvertFunc(صحیح sdepth, صحیح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شدہ HAVE_OPENCL
ساکن منطقی ocl_convert_مقصد(داخلہ_Array src_, OutputArray dst_, صحیح ddepth, منطقی noScale, دوہرا alpha, دوہرا beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    صحیح stype = src_.type();
    صحیح sdepth = CV_MAT_گہرائی(stype);
    صحیح cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    اگر ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        واپس جھوٹ;

    صحیح dtype = CV_MAKETYPE(ddepth, cn);

    صحیح wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    منطقی needDouble = sdepth == CV_64F || ddepth == CV_64F;
    منطقی doubleCheck = سچ;
    اگر (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    منطقی halfCheck = سچ;
    منطقی needHalf = sdepth == CV_16F || ddepth == CV_16F;
    اگر (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    اگر (!doubleCheck)
        واپس جھوٹ;
    اگر (!halfCheck)
        واپس جھوٹ;

    ثابت صحیح صفیں_فی_WI = 4;

    حرف cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::type_مقصد_Str(sdepth), ocl::type_مقصد_Str(wdepth), ocl::type_مقصد_Str(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], حجم(cvt[1])),
                         صفیں_فی_WI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    اگر (k.empty())
        واپس جھوٹ;

    UMat src = src_.getUMat();
    dst_.create_Same_سائز(src_, dtype);
    UMat dst = dst_.getUMat();

    اعشاری alphaf = (اعشاری)alpha, betaf = (اعشاری)beta;

    اگر (noScale)
        k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), ocl::KernelArg::WriteOnly(dst, cn));
    ورنہ اگر (wdepth == CV_32F)
        k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    ورنہ
        k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.ستون_تعداد * cn,
        div_اوپر((size_t)dst.صفیں, صفیں_فی_WI)
    };
    اگر (!k.تسلسل(2, globalsize, تہی, جھوٹ))
        واپس جھوٹ;

    CV_IMPL_جوڑو(CV_IMPL_OCL);
    واپس سچ;
}
#اگر_ختم

خالی Mat::convert_مقصد(OutputArray dst, صحیح type_, دوہرا alpha, دوہرا beta) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر (empty())
    {
        صحیح ddepth = CV_MAT_گہرائی(type_);
        اگر (ddepth < 0)
            ddepth = dst.fixedType() ? dst.گہرائی() : گہرائی();
        ثابت صحیح dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        منطقی allowTransposed = dims == 1 ||
            dst.قسم() == داخلہ_Array_2::STD_سمتیہ ||
            (dst.fixed_سائز() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        واپس;
    }

    صحیح stype = type();
    صحیح sdepth = CV_MAT_گہرائی(stype);

    صحیح ddepth = sdepth;
    اگر (type_ >= 0)
        ddepth = CV_MAT_گہرائی(type_);
    ورنہ
        ddepth = dst.fixedType() ? dst.گہرائی() : sdepth;

    منطقی noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    اگر (sdepth == ddepth && noScale)
    {
        copy_مقصد(dst);
        واپس;
    }

    CV_OCL_تسلسل(dims <= 2 && dst.isUMat(),
               ocl_convert_مقصد(*یہ, dst, ddepth, noScale, alpha, beta))

    صحیح cn = channels();
    صحیح dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *یہ;
    منطقی allowTransposed = dims == 1 ||
        dst.قسم() == داخلہ_Array_2::STD_سمتیہ ||
        (dst.fixed_سائز() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    اگر( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.ڈیٹا, src.قدم, dstMat.ڈیٹا, dstMat.قدم, src.ستون_تعداد * cn, src.صفیں, sdepth, ddepth, alpha, beta);
    }
    ورنہ اگر( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.ڈیٹا, 0, dstMat.ڈیٹا, 0, (صحیح)src.کل() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    ثنائی_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    دوہرا scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    اگر( dims <= 2 )
    {
        سائز sz = getContinuousSize2D(src, dstMat, cn);
        func(src.ڈیٹا, src.قدم, 0, 0, dstMat.ڈیٹا, dstMat.قدم, sz, scale);
    }
    ورنہ
    {
        ثابت Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        سائز sz((صحیح)(it.size*cn), 1);

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

خالی UMat::convert_مقصد(OutputArray dst, صحیح type_, دوہرا alpha, دوہرا beta) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        واپس;
    }

#اگر_تعریف_شدہ HAVE_OPENCL
    صحیح stype = type();
    صحیح sdepth = CV_MAT_گہرائی(stype);

    صحیح ddepth = sdepth;
    اگر (type_ >= 0)
        ddepth = CV_MAT_گہرائی(type_);
    ورنہ
        ddepth = dst.fixedType() ? dst.گہرائی() : sdepth;

    منطقی noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    اگر (sdepth == ddepth && noScale)
    {
        copy_مقصد(dst);
        واپس;
    }

    CV_OCL_تسلسل(dims <= 2,
               ocl_convert_مقصد(*یہ, dst, ddepth, noScale, alpha, beta))
#اگر_ختم // HAVE_OPENCL

    UMat src = *یہ;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convert_مقصد(dst, type_, alpha, beta);
    (خالی)src;
}

} // namespace cv
