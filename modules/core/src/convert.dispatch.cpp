// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "convert.simd.hpp"
#अन्तर्भाव "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

नामाकाश hal {
शून्य cvt16f32f(ध्रुव hfloat* src, भिन्न* dst, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य cvt32f16f(ध्रुव भिन्न* src, hfloat* dst, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य cvt32f16bf(ध्रुव भिन्न* src, bfloat* dst, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य addRNGBias32f(भिन्न* arr, ध्रुव भिन्न* scaleBiasPairs, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य addRNGBias64f(द्विगुण* arr, ध्रुव द्विगुण* scaleBiasPairs, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


द्विमान_Func getConvertFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_convertTo(निवेशः_Array src_, OutputArray dst_, पूर्णाङ्क ddepth, तार्किक noScale, द्विगुण alpha, द्विगुण beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    पूर्णाङ्क stype = src_.type();
    पूर्णाङ्क sdepth = CV_MAT_DEPTH(stype);
    पूर्णाङ्क cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    सचे ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        पटिनिवत्त मिच्छा;

    पूर्णाङ्क dtype = CV_MAKETYPE(ddepth, cn);

    पूर्णाङ्क wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    तार्किक needDouble = sdepth == CV_64F || ddepth == CV_64F;
    तार्किक doubleCheck = सच्च;
    सचे (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    तार्किक halfCheck = सच्च;
    तार्किक needHalf = sdepth == CV_16F || ddepth == CV_16F;
    सचे (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    सचे (!doubleCheck)
        पटिनिवत्त मिच्छा;
    सचे (!halfCheck)
        पटिनिवत्त मिच्छा;

    ध्रुव पूर्णाङ्क rowsPerWI = 4;

    वर्ण cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], परिमाण(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], परिमाण(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    सचे (k.empty())
        पटिनिवत्त मिच्छा;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    भिन्न alphaf = (भिन्न)alpha, betaf = (भिन्न)beta;

    सचे (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    अञ्ञथा सचे (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    अञ्ञथा
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    सचे (!k.run(2, globalsize, रिक्त, मिच्छा))
        पटिनिवत्त मिच्छा;

    CV_IMPL_ADD(CV_IMPL_OCL);
    पटिनिवत्त सच्च;
}
#endif

शून्य Mat::convertTo(OutputArray dst, पूर्णाङ्क type_, द्विगुण alpha, द्विगुण beta) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे (empty())
    {
        पूर्णाङ्क ddepth = CV_MAT_DEPTH(type_);
        सचे (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        ध्रुव पूर्णाङ्क dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        तार्किक allowTransposed = dims == 1 ||
            dst.kind() == निवेशः_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        पटिनिवत्त;
    }

    पूर्णाङ्क stype = type();
    पूर्णाङ्क sdepth = CV_MAT_DEPTH(stype);

    पूर्णाङ्क ddepth = sdepth;
    सचे (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    अञ्ञथा
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    तार्किक noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    सचे (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        पटिनिवत्त;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*एतत्, dst, ddepth, noScale, alpha, beta))

    पूर्णाङ्क cn = channels();
    पूर्णाङ्क dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *एतत्;
    तार्किक allowTransposed = dims == 1 ||
        dst.kind() == निवेशः_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    सचे( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    अञ्ञथा सचे( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (पूर्णाङ्क)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    द्विमान_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    द्विगुण scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    सचे( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    अञ्ञथा
    {
        ध्रुव Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((पूर्णाङ्क)(it.size*cn), 1);

        अत्थाय( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

शून्य UMat::convertTo(OutputArray dst, पूर्णाङ्क type_, द्विगुण alpha, द्विगुण beta) ध्रुव
{
    CV_INSTRUMENT_REGION();

    सचे (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        पटिनिवत्त;
    }

#ifdef HAVE_OPENCL
    पूर्णाङ्क stype = type();
    पूर्णाङ्क sdepth = CV_MAT_DEPTH(stype);

    पूर्णाङ्क ddepth = sdepth;
    सचे (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    अञ्ञथा
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    तार्किक noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    सचे (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        पटिनिवत्त;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*एतत्, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *एतत्;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (शून्य)src;
}

} // namespace cv
