// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

#అన్తర్భావ "convert.simd.hpp"
#అన్తర్భావ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv {

నామాకాశ hal {
శూన్య cvt16f32f(స్థిరరాశి hfloat* src, దశాంశం* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
శూన్య cvt32f16f(స్థిరరాశి దశాంశం* src, hfloat* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
శూన్య cvt32f16bf(స్థిరరాశి దశాంశం* src, bfloat* dst, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
శూన్య కూడు_RNGBias32f(దశాంశం* arr, స్థిరరాశి దశాంశం* scale_Bias_జతలు, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(కూడు_RNGBias32f, (arr, scale_Bias_జతలు, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
శూన్య కూడు_RNGBias64f(ద్విగుణ* arr, స్థిరరాశి ద్విగుణ* scale_Bias_జతలు, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(కూడు_RNGBias64f, (arr, scale_Bias_జతలు, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


ద్విమాన_Func getConvertFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక ocl_convert_గమ్యం(ఆదానం_Array src_, OutputArray dst_, పూర్ణాంకం ddepth, తార్కిక noScale, ద్విగుణ alpha, ద్విగుణ beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    పూర్ణాంకం stype = src_.type();
    పూర్ణాంకం sdepth = CV_MAT_లోతు(stype);
    పూర్ణాంకం cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    ఒకవేళ ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        తిరిగి అబద్ధం;

    పూర్ణాంకం dtype = CV_MAKETYPE(ddepth, cn);

    పూర్ణాంకం wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    తార్కిక needDouble = sdepth == CV_64F || ddepth == CV_64F;
    తార్కిక doubleCheck = నిజం;
    ఒకవేళ (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    తార్కిక halfCheck = నిజం;
    తార్కిక needHalf = sdepth == CV_16F || ddepth == CV_16F;
    ఒకవేళ (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    ఒకవేళ (!doubleCheck)
        తిరిగి అబద్ధం;
    ఒకవేళ (!halfCheck)
        తిరిగి అబద్ధం;

    స్థిరరాశి పూర్ణాంకం అడ్డువరుసలు_చొప్పున_WI = 4;

    అక్షరం cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::type_గమ్యం_Str(sdepth), ocl::type_గమ్యం_Str(wdepth), ocl::type_గమ్యం_Str(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], పరిమాణం(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], పరిమాణం(cvt[1])),
                         అడ్డువరుసలు_చొప్పున_WI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src = src_.getUMat();
    dst_.create_Same_కొలత(src_, dtype);
    UMat dst = dst_.getUMat();

    దశాంశం alphaf = (దశాంశం)alpha, betaf = (దశాంశం)beta;

    ఒకవేళ (noScale)
        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), ocl::KernelArg::WriteOnly(dst, cn));
    లేకపోతే ఒకవేళ (wdepth == CV_32F)
        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    లేకపోతే
        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.నిలువుల_సంఖ్య * cn,
        div_పైకి((size_t)dst.అడ్డువరుసలు, అడ్డువరుసలు_చొప్పున_WI)
    };
    ఒకవేళ (!k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం))
        తిరిగి అబద్ధం;

    CV_IMPL_కూడు(CV_IMPL_OCL);
    తిరిగి నిజం;
}
#అయితే_ముగింపు

శూన్య Mat::convert_గమ్యం(OutputArray dst, పూర్ణాంకం type_, ద్విగుణ alpha, ద్విగుణ beta) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (empty())
    {
        పూర్ణాంకం ddepth = CV_MAT_లోతు(type_);
        ఒకవేళ (ddepth < 0)
            ddepth = dst.fixedType() ? dst.లోతు() : లోతు();
        స్థిరరాశి పూర్ణాంకం dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        తార్కిక allowTransposed = dims == 1 ||
            dst.రకం() == ఆదానం_Array_2::STD_సదిశ ||
            (dst.fixed_కొలత() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        తిరిగి;
    }

    పూర్ణాంకం stype = type();
    పూర్ణాంకం sdepth = CV_MAT_లోతు(stype);

    పూర్ణాంకం ddepth = sdepth;
    ఒకవేళ (type_ >= 0)
        ddepth = CV_MAT_లోతు(type_);
    లేకపోతే
        ddepth = dst.fixedType() ? dst.లోతు() : sdepth;

    తార్కిక noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ఒకవేళ (sdepth == ddepth && noScale)
    {
        copy_గమ్యం(dst);
        తిరిగి;
    }

    CV_OCL_నిరంతరం(dims <= 2 && dst.isUMat(),
               ocl_convert_గమ్యం(*ఇది, dst, ddepth, noScale, alpha, beta))

    పూర్ణాంకం cn = channels();
    పూర్ణాంకం dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *ఇది;
    తార్కిక allowTransposed = dims == 1 ||
        dst.రకం() == ఆదానం_Array_2::STD_సదిశ ||
        (dst.fixed_కొలత() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    ఒకవేళ( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.దత్తాంశం, src.అడుగు, dstMat.దత్తాంశం, dstMat.అడుగు, src.నిలువుల_సంఖ్య * cn, src.అడ్డువరుసలు, sdepth, ddepth, alpha, beta);
    }
    లేకపోతే ఒకవేళ( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.దత్తాంశం, 0, dstMat.దత్తాంశం, 0, (పూర్ణాంకం)src.మొత్తం() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    ద్విమాన_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    ద్విగుణ scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    ఒకవేళ( dims <= 2 )
    {
        కొలత sz = getContinuousSize2D(src, dstMat, cn);
        func(src.దత్తాంశం, src.అడుగు, 0, 0, dstMat.దత్తాంశం, dstMat.అడుగు, sz, scale);
    }
    లేకపోతే
    {
        స్థిరరాశి Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        కొలత sz((పూర్ణాంకం)(it.size*cn), 1);

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

శూన్య UMat::convert_గమ్యం(OutputArray dst, పూర్ణాంకం type_, ద్విగుణ alpha, ద్విగుణ beta) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        తిరిగి;
    }

#నిర్వచితమైతే HAVE_OPENCL
    పూర్ణాంకం stype = type();
    పూర్ణాంకం sdepth = CV_MAT_లోతు(stype);

    పూర్ణాంకం ddepth = sdepth;
    ఒకవేళ (type_ >= 0)
        ddepth = CV_MAT_లోతు(type_);
    లేకపోతే
        ddepth = dst.fixedType() ? dst.లోతు() : sdepth;

    తార్కిక noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ఒకవేళ (sdepth == ddepth && noScale)
    {
        copy_గమ్యం(dst);
        తిరిగి;
    }

    CV_OCL_నిరంతరం(dims <= 2,
               ocl_convert_గమ్యం(*ఇది, dst, ddepth, noScale, alpha, beta))
#అయితే_ముగింపు // HAVE_OPENCL

    UMat src = *ఇది;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convert_గమ్యం(dst, type_, alpha, beta);
    (శూన్య)src;
}

} // namespace cv
