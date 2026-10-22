// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "convert.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv {

ਨਾਮਾਕਾਸ਼ hal {
ਖਾਲੀ cvt16f32f(ਅਚਲ hfloat* src, ਦਸ਼ਮਲਵ* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਖਾਲੀ cvt32f16f(ਅਚਲ ਦਸ਼ਮਲਵ* src, hfloat* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਖਾਲੀ cvt32f16bf(ਅਚਲ ਦਸ਼ਮਲਵ* src, bfloat* dst, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਖਾਲੀ ਜੋੜੋ_RNGBias32f(ਦਸ਼ਮਲਵ* arr, ਅਚਲ ਦਸ਼ਮਲਵ* scale_Bias_ਜੋੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(ਜੋੜੋ_RNGBias32f, (arr, scale_Bias_ਜੋੜੇ, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਖਾਲੀ ਜੋੜੋ_RNGBias64f(ਦੁੱਗਣਾ* arr, ਅਚਲ ਦੁੱਗਣਾ* scale_Bias_ਜੋੜੇ, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(ਜੋੜੋ_RNGBias64f, (arr, scale_Bias_ਜੋੜੇ, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


ਦੁਆਧਾਰੀ_Func getConvertFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ ocl_convert_ਮੰਜ਼ਿਲ(ਨਿਵੇਸ਼_Array src_, OutputArray dst_, ਪੂਰਨ ddepth, ਤਾਰਕਿਕ noScale, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    ਪੂਰਨ stype = src_.type();
    ਪੂਰਨ sdepth = CV_MAT_ਡੂੰਘਾਈ(stype);
    ਪੂਰਨ cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    ਜੇ ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ dtype = CV_MAKETYPE(ddepth, cn);

    ਪੂਰਨ wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    ਤਾਰਕਿਕ needDouble = sdepth == CV_64F || ddepth == CV_64F;
    ਤਾਰਕਿਕ doubleCheck = ਸੱਚ;
    ਜੇ (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    ਤਾਰਕਿਕ halfCheck = ਸੱਚ;
    ਤਾਰਕਿਕ needHalf = sdepth == CV_16F || ddepth == CV_16F;
    ਜੇ (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    ਜੇ (!doubleCheck)
        ਵਾਪਸ ਝੂਠ;
    ਜੇ (!halfCheck)
        ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਪੂਰਨ ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = 4;

    ਅੱਖਰ cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(sdepth), ocl::type_ਮੰਜ਼ਿਲ_Str(wdepth), ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], ਆਕਾਰ(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], ਆਕਾਰ(cvt[1])),
                         ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src = src_.getUMat();
    dst_.create_Same_ਮਾਪ(src_, dtype);
    UMat dst = dst_.getUMat();

    ਦਸ਼ਮਲਵ alphaf = (ਦਸ਼ਮਲਵ)alpha, betaf = (ਦਸ਼ਮਲਵ)beta;

    ਜੇ (noScale)
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), ocl::KernelArg::WriteOnly(dst, cn));
    ਨਹੀਂਤਾਂ ਜੇ (wdepth == CV_32F)
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    ਨਹੀਂਤਾਂ
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.ਖਾਨੇ_ਗਿਣਤੀ * cn,
        div_ਉੱਪਰ((size_t)dst.ਪੰਕਤੀਆਂ, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI)
    };
    ਜੇ (!k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ))
        ਵਾਪਸ ਝੂਠ;

    CV_IMPL_ਜੋੜੋ(CV_IMPL_OCL);
    ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ Mat::convert_ਮੰਜ਼ਿਲ(OutputArray dst, ਪੂਰਨ type_, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ (empty())
    {
        ਪੂਰਨ ddepth = CV_MAT_ਡੂੰਘਾਈ(type_);
        ਜੇ (ddepth < 0)
            ddepth = dst.fixedType() ? dst.ਡੂੰਘਾਈ() : ਡੂੰਘਾਈ();
        ਅਚਲ ਪੂਰਨ dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        ਤਾਰਕਿਕ allowTransposed = dims == 1 ||
            dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼ ||
            (dst.fixed_ਮਾਪ() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ਵਾਪਸ;
    }

    ਪੂਰਨ stype = type();
    ਪੂਰਨ sdepth = CV_MAT_ਡੂੰਘਾਈ(stype);

    ਪੂਰਨ ddepth = sdepth;
    ਜੇ (type_ >= 0)
        ddepth = CV_MAT_ਡੂੰਘਾਈ(type_);
    ਨਹੀਂਤਾਂ
        ddepth = dst.fixedType() ? dst.ਡੂੰਘਾਈ() : sdepth;

    ਤਾਰਕਿਕ noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ਜੇ (sdepth == ddepth && noScale)
    {
        copy_ਮੰਜ਼ਿਲ(dst);
        ਵਾਪਸ;
    }

    CV_OCL_ਲੜੀਵਾਰ(dims <= 2 && dst.isUMat(),
               ocl_convert_ਮੰਜ਼ਿਲ(*ਇਹ, dst, ddepth, noScale, alpha, beta))

    ਪੂਰਨ cn = channels();
    ਪੂਰਨ dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *ਇਹ;
    ਤਾਰਕਿਕ allowTransposed = dims == 1 ||
        dst.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼ ||
        (dst.fixed_ਮਾਪ() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    ਜੇ( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.ਅੰਕੜੇ, src.ਕਦਮ, dstMat.ਅੰਕੜੇ, dstMat.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ * cn, src.ਪੰਕਤੀਆਂ, sdepth, ddepth, alpha, beta);
    }
    ਨਹੀਂਤਾਂ ਜੇ( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.ਅੰਕੜੇ, 0, dstMat.ਅੰਕੜੇ, 0, (ਪੂਰਨ)src.ਕੁੱਲ() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    ਦੁਆਧਾਰੀ_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    ਦੁੱਗਣਾ scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    ਜੇ( dims <= 2 )
    {
        ਮਾਪ sz = getContinuousSize2D(src, dstMat, cn);
        func(src.ਅੰਕੜੇ, src.ਕਦਮ, 0, 0, dstMat.ਅੰਕੜੇ, dstMat.ਕਦਮ, sz, scale);
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ਮਾਪ sz((ਪੂਰਨ)(it.size*cn), 1);

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

ਖਾਲੀ UMat::convert_ਮੰਜ਼ਿਲ(OutputArray dst, ਪੂਰਨ type_, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    ਜੇ (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ਵਾਪਸ;
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਪੂਰਨ stype = type();
    ਪੂਰਨ sdepth = CV_MAT_ਡੂੰਘਾਈ(stype);

    ਪੂਰਨ ddepth = sdepth;
    ਜੇ (type_ >= 0)
        ddepth = CV_MAT_ਡੂੰਘਾਈ(type_);
    ਨਹੀਂਤਾਂ
        ddepth = dst.fixedType() ? dst.ਡੂੰਘਾਈ() : sdepth;

    ਤਾਰਕਿਕ noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ਜੇ (sdepth == ddepth && noScale)
    {
        copy_ਮੰਜ਼ਿਲ(dst);
        ਵਾਪਸ;
    }

    CV_OCL_ਲੜੀਵਾਰ(dims <= 2,
               ocl_convert_ਮੰਜ਼ਿਲ(*ਇਹ, dst, ddepth, noScale, alpha, beta))
#ਜੇ_ਅੰਤ // HAVE_OPENCL

    UMat src = *ਇਹ;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convert_ਮੰਜ਼ਿਲ(dst, type_, alpha, beta);
    (ਖਾਲੀ)src;
}

} // namespace cv
