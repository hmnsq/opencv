// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

#ଅନ୍ତର୍ଭାଵ "convert.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv {

ନାମାକାଶ hal {
ଶୂନ୍ୟ cvt16f32f(ଅଚଳ hfloat* src, ଦଶମିକ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ଶୂନ୍ୟ cvt32f16f(ଅଚଳ ଦଶମିକ* src, hfloat* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ଶୂନ୍ୟ cvt32f16bf(ଅଚଳ ଦଶମିକ* src, bfloat* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ଶୂନ୍ୟ addRNGBias32f(ଦଶମିକ* arr, ଅଚଳ ଦଶମିକ* scaleBiasPairs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
ଶୂନ୍ୟ addRNGBias64f(ଦ୍ୱିଗୁଣ* arr, ଅଚଳ ଦ୍ୱିଗୁଣ* scaleBiasPairs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


ଦ୍ୱିମିକ_Func getConvertFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ ocl_convertTo(ନିବେଶ_Array src_, OutputArray dst_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ତାର୍କିକ noScale, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = src_.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth = CV_MAT_DEPTH(stype);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    ଯଦି ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = CV_MAKETYPE(ddepth, cn);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    ତାର୍କିକ needDouble = sdepth == CV_64F || ddepth == CV_64F;
    ତାର୍କିକ doubleCheck = ସତ୍ୟ;
    ଯଦି (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    ତାର୍କିକ halfCheck = ସତ୍ୟ;
    ତାର୍କିକ needHalf = sdepth == CV_16F || ddepth == CV_16F;
    ଯଦି (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    ଯଦି (!doubleCheck)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!halfCheck)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rowsPerWI = 4;

    ଅକ୍ଷର cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], ଆକାର(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], ଆକାର(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    ଦଶମିକ alphaf = (ଦଶମିକ)alpha, betaf = (ଦଶମିକ)beta;

    ଯଦି (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    ଅନ୍ୟଥା ଯଦି (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    ଅନ୍ୟଥା
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    ଯଦି (!k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    CV_IMPL_ADD(CV_IMPL_OCL);
    ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

ଶୂନ୍ୟ Mat::convertTo(OutputArray dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type_, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି (empty())
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = CV_MAT_DEPTH(type_);
        ଯଦି (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        ତାର୍କିକ allowTransposed = dims == 1 ||
            dst.kind() == ନିବେଶ_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ଫେରନ୍ତୁ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth = CV_MAT_DEPTH(stype);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = sdepth;
    ଯଦି (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    ଅନ୍ୟଥା
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    ତାର୍କିକ noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ଯଦି (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ଫେରନ୍ତୁ;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*ଏହି, dst, ddepth, noScale, alpha, beta))

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *ଏହି;
    ତାର୍କିକ allowTransposed = dims == 1 ||
        dst.kind() == ନିବେଶ_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    ଯଦି( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    ଅନ୍ୟଥା ଯଦି( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    ଦ୍ୱିମିକ_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    ଦ୍ୱିଗୁଣ scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    ଯଦି( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(it.size*cn), 1);

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

ଶୂନ୍ୟ UMat::convertTo(OutputArray dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type_, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    ଯଦି (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ଫେରନ୍ତୁ;
    }

#ifdef HAVE_OPENCL
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth = CV_MAT_DEPTH(stype);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = sdepth;
    ଯଦି (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    ଅନ୍ୟଥା
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    ତାର୍କିକ noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ଯଦି (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ଫେରନ୍ତୁ;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*ଏହି, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *ଏହି;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (ଶୂନ୍ୟ)src;
}

} // namespace cv
