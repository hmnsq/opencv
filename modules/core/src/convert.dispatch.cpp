// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"

#அந்தர்பாவ "convert.simd.hpp"
#அந்தர்பாவ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv {

நாமாகாஶ hal {
வெறுமை cvt16f32f(மாறிலி hfloat* src, தசம* dst, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
வெறுமை cvt32f16f(மாறிலி தசம* src, hfloat* dst, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
வெறுமை cvt32f16bf(மாறிலி தசம* src, bfloat* dst, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
வெறுமை addRNGBias32f(தசம* arr, மாறிலி தசம* scaleBiasPairs, முழு len, முழு cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
வெறுமை addRNGBias64f(இரட்டை* arr, மாறிலி இரட்டை* scaleBiasPairs, முழு len, முழு cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


இருமம்_Func getConvertFunc(முழு sdepth, முழு ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
நிலையான தருக்க ocl_convertTo(உள்ளீடு_Array src_, OutputArray dst_, முழு ddepth, தருக்க noScale, இரட்டை alpha, இரட்டை beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    முழு stype = src_.type();
    முழு sdepth = CV_MAT_DEPTH(stype);
    முழு cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    எனில் ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        திரும்பு பொய்;

    முழு dtype = CV_MAKETYPE(ddepth, cn);

    முழு wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    தருக்க needDouble = sdepth == CV_64F || ddepth == CV_64F;
    தருக்க doubleCheck = உண்மை;
    எனில் (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    தருக்க halfCheck = உண்மை;
    தருக்க needHalf = sdepth == CV_16F || ddepth == CV_16F;
    எனில் (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    எனில் (!doubleCheck)
        திரும்பு பொய்;
    எனில் (!halfCheck)
        திரும்பு பொய்;

    மாறிலி முழு rowsPerWI = 4;

    எழுத்து cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], அளவு(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], அளவு(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    தசம alphaf = (தசம)alpha, betaf = (தசம)beta;

    எனில் (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    இல்லையெனில் எனில் (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    இல்லையெனில்
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    எனில் (!k.run(2, globalsize, வெற்று, பொய்))
        திரும்பு பொய்;

    CV_IMPL_ADD(CV_IMPL_OCL);
    திரும்பு உண்மை;
}
#endif

வெறுமை Mat::convertTo(OutputArray dst, முழு type_, இரட்டை alpha, இரட்டை beta) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில் (empty())
    {
        முழு ddepth = CV_MAT_DEPTH(type_);
        எனில் (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        மாறிலி முழு dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        தருக்க allowTransposed = dims == 1 ||
            dst.kind() == உள்ளீடு_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        திரும்பு;
    }

    முழு stype = type();
    முழு sdepth = CV_MAT_DEPTH(stype);

    முழு ddepth = sdepth;
    எனில் (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    இல்லையெனில்
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    தருக்க noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    எனில் (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        திரும்பு;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*இது, dst, ddepth, noScale, alpha, beta))

    முழு cn = channels();
    முழு dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *இது;
    தருக்க allowTransposed = dims == 1 ||
        dst.kind() == உள்ளீடு_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    எனில்( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    இல்லையெனில் எனில்( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (முழு)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    இருமம்_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    இரட்டை scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    எனில்( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    இல்லையெனில்
    {
        மாறிலி Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((முழு)(it.size*cn), 1);

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

வெறுமை UMat::convertTo(OutputArray dst, முழு type_, இரட்டை alpha, இரட்டை beta) மாறிலி
{
    CV_INSTRUMENT_REGION();

    எனில் (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        திரும்பு;
    }

#ifdef HAVE_OPENCL
    முழு stype = type();
    முழு sdepth = CV_MAT_DEPTH(stype);

    முழு ddepth = sdepth;
    எனில் (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    இல்லையெனில்
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    தருக்க noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    எனில் (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        திரும்பு;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*இது, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *இது;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (வெறுமை)src;
}

} // namespace cv
