// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "convert.simd.hpp"
#ضمن "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

نطاق hal {
فراغ cvt16f32f(ثابت hfloat* src, عائم* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
فراغ cvt32f16f(ثابت عائم* src, hfloat* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
فراغ cvt32f16bf(ثابت عائم* src, bfloat* dst, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
فراغ جمع_کړه_RNGBias32f(عائم* arr, ثابت عائم* scale_Bias_جوړې, صحيح len, صحيح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(جمع_کړه_RNGBias32f, (arr, scale_Bias_جوړې, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
فراغ جمع_کړه_RNGBias64f(مزدوج* arr, ثابت مزدوج* scale_Bias_جوړې, صحيح len, صحيح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(جمع_کړه_RNGBias64f, (arr, scale_Bias_جوړې, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


دوګونی_Func getConvertFunc(صحيح sdepth, صحيح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#که_تعریف_شوی HAVE_OPENCL
ساكن منطقي ocl_convert_منزل(ننوت_Array src_, OutputArray dst_, صحيح ddepth, منطقي noScale, مزدوج alpha, مزدوج beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    صحيح stype = src_.type();
    صحيح sdepth = CV_MAT_ژوروالی(stype);
    صحيح cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    إذا ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        أرجع خطأ;

    صحيح dtype = CV_MAKETYPE(ddepth, cn);

    صحيح wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    منطقي needDouble = sdepth == CV_64F || ddepth == CV_64F;
    منطقي doubleCheck = ریښتیا;
    إذا (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    منطقي halfCheck = ریښتیا;
    منطقي needHalf = sdepth == CV_16F || ddepth == CV_16F;
    إذا (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    إذا (!doubleCheck)
        أرجع خطأ;
    إذا (!halfCheck)
        أرجع خطأ;

    ثابت صحيح لیکې_فی_WI = 4;

    حرف cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::type_منزل_Str(sdepth), ocl::type_منزل_Str(wdepth), ocl::type_منزل_Str(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], حجم(cvt[1])),
                         لیکې_فی_WI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    إذا (k.empty())
        أرجع خطأ;

    UMat src = src_.getUMat();
    dst_.create_Same_اندازه(src_, dtype);
    UMat dst = dst_.getUMat();

    عائم alphaf = (عائم)alpha, betaf = (عائم)beta;

    إذا (noScale)
        k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(src), ocl::KernelArg::WriteOnly(dst, cn));
    وإلا إذا (wdepth == CV_32F)
        k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    وإلا
        k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.ستون_شمېر * cn,
        div_پورته((size_t)dst.لیکې, لیکې_فی_WI)
    };
    إذا (!k.پرلپسې(2, globalsize, عدم, خطأ))
        أرجع خطأ;

    CV_IMPL_جمع_کړه(CV_IMPL_OCL);
    أرجع ریښتیا;
}
#د_که_پای

فراغ Mat::convert_منزل(OutputArray dst, صحيح type_, مزدوج alpha, مزدوج beta) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا (empty())
    {
        صحيح ddepth = CV_MAT_ژوروالی(type_);
        إذا (ddepth < 0)
            ddepth = dst.fixedType() ? dst.ژوروالی() : ژوروالی();
        ثابت صحيح dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        منطقي allowTransposed = dims == 1 ||
            dst.ډول() == ننوت_Array_2::STD_ویکټور ||
            (dst.fixed_اندازه() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        أرجع;
    }

    صحيح stype = type();
    صحيح sdepth = CV_MAT_ژوروالی(stype);

    صحيح ddepth = sdepth;
    إذا (type_ >= 0)
        ddepth = CV_MAT_ژوروالی(type_);
    وإلا
        ddepth = dst.fixedType() ? dst.ژوروالی() : sdepth;

    منطقي noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    إذا (sdepth == ddepth && noScale)
    {
        copy_منزل(dst);
        أرجع;
    }

    CV_OCL_پرلپسې(dims <= 2 && dst.isUMat(),
               ocl_convert_منزل(*هذا, dst, ddepth, noScale, alpha, beta))

    صحيح cn = channels();
    صحيح dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *هذا;
    منطقي allowTransposed = dims == 1 ||
        dst.ډول() == ننوت_Array_2::STD_ویکټور ||
        (dst.fixed_اندازه() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    إذا( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.معلومات, src.ګام, dstMat.معلومات, dstMat.ګام, src.ستون_شمېر * cn, src.لیکې, sdepth, ddepth, alpha, beta);
    }
    وإلا إذا( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.معلومات, 0, dstMat.معلومات, 0, (صحيح)src.ټول() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    دوګونی_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    مزدوج scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    إذا( dims <= 2 )
    {
        اندازه sz = getContinuousSize2D(src, dstMat, cn);
        func(src.معلومات, src.ګام, 0, 0, dstMat.معلومات, dstMat.ګام, sz, scale);
    }
    وإلا
    {
        ثابت Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        اندازه sz((صحيح)(it.size*cn), 1);

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

فراغ UMat::convert_منزل(OutputArray dst, صحيح type_, مزدوج alpha, مزدوج beta) ثابت
{
    CV_INSTRUMENT_REGION();

    إذا (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        أرجع;
    }

#که_تعریف_شوی HAVE_OPENCL
    صحيح stype = type();
    صحيح sdepth = CV_MAT_ژوروالی(stype);

    صحيح ddepth = sdepth;
    إذا (type_ >= 0)
        ddepth = CV_MAT_ژوروالی(type_);
    وإلا
        ddepth = dst.fixedType() ? dst.ژوروالی() : sdepth;

    منطقي noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    إذا (sdepth == ddepth && noScale)
    {
        copy_منزل(dst);
        أرجع;
    }

    CV_OCL_پرلپسې(dims <= 2,
               ocl_convert_منزل(*هذا, dst, ddepth, noScale, alpha, beta))
#د_که_پای // HAVE_OPENCL

    UMat src = *هذا;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convert_منزل(dst, type_, alpha, beta);
    (فراغ)src;
}

} // namespace cv
