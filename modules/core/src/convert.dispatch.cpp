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
शून्य सङ्कलय_RNGBias32f(भिन्न* arr, ध्रुव भिन्न* scale_Bias_युग्मानि, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(सङ्कलय_RNGBias32f, (arr, scale_Bias_युग्मानि, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य सङ्कलय_RNGBias64f(द्विगुण* arr, ध्रुव द्विगुण* scale_Bias_युग्मानि, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(सङ्कलय_RNGBias64f, (arr, scale_Bias_युग्मानि, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


द्विमान_Func getConvertFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#यदि_परिभाषितम् HAVE_OPENCL
स्थिर तार्किक ocl_convert_गन्तव्यम्(निवेशः_Array src_, OutputArray dst_, पूर्णाङ्क ddepth, तार्किक noScale, द्विगुण alpha, द्विगुण beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    पूर्णाङ्क stype = src_.type();
    पूर्णाङ्क sdepth = CV_MAT_गभीरता(stype);
    पूर्णाङ्क cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    यदि ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        प्रत्यागम असत्य;

    पूर्णाङ्क dtype = CV_MAKETYPE(ddepth, cn);

    पूर्णाङ्क wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    तार्किक needDouble = sdepth == CV_64F || ddepth == CV_64F;
    तार्किक doubleCheck = सत्य;
    यदि (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    तार्किक halfCheck = सत्य;
    तार्किक needHalf = sdepth == CV_16F || ddepth == CV_16F;
    यदि (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    यदि (!doubleCheck)
        प्रत्यागम असत्य;
    यदि (!halfCheck)
        प्रत्यागम असत्य;

    ध्रुव पूर्णाङ्क पङ्क्तिकाः_प्रत्येकम्_WI = 4;

    वर्ण cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::type_गन्तव्यम्_Str(sdepth), ocl::type_गन्तव्यम्_Str(wdepth), ocl::type_गन्तव्यम्_Str(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], परिमाण(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], परिमाण(cvt[1])),
                         पङ्क्तिकाः_प्रत्येकम्_WI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src = src_.getUMat();
    dst_.create_Same_आकार(src_, dtype);
    UMat dst = dst_.getUMat();

    भिन्न alphaf = (भिन्न)alpha, betaf = (भिन्न)beta;

    यदि (noScale)
        k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(src), ocl::KernelArg::WriteOnly(dst, cn));
    अन्यथा यदि (wdepth == CV_32F)
        k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    अन्यथा
        k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.स्तम्भसङ्ख्या * cn,
        div_ऊर्ध्वम्((size_t)dst.पङ्क्तिकाः, पङ्क्तिकाः_प्रत्येकम्_WI)
    };
    यदि (!k.सततम्(2, globalsize, रिक्त, असत्य))
        प्रत्यागम असत्य;

    CV_IMPL_सङ्कलय(CV_IMPL_OCL);
    प्रत्यागम सत्य;
}
#यदि_अन्तः

शून्य Mat::convert_गन्तव्यम्(OutputArray dst, पूर्णाङ्क type_, द्विगुण alpha, द्विगुण beta) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि (empty())
    {
        पूर्णाङ्क ddepth = CV_MAT_गभीरता(type_);
        यदि (ddepth < 0)
            ddepth = dst.fixedType() ? dst.गभीरता() : गभीरता();
        ध्रुव पूर्णाङ्क dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        तार्किक allowTransposed = dims == 1 ||
            dst.विधा() == निवेशः_Array_2::STD_सदिशम् ||
            (dst.fixed_आकार() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        प्रत्यागम;
    }

    पूर्णाङ्क stype = type();
    पूर्णाङ्क sdepth = CV_MAT_गभीरता(stype);

    पूर्णाङ्क ddepth = sdepth;
    यदि (type_ >= 0)
        ddepth = CV_MAT_गभीरता(type_);
    अन्यथा
        ddepth = dst.fixedType() ? dst.गभीरता() : sdepth;

    तार्किक noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    यदि (sdepth == ddepth && noScale)
    {
        copy_गन्तव्यम्(dst);
        प्रत्यागम;
    }

    CV_OCL_सततम्(dims <= 2 && dst.isUMat(),
               ocl_convert_गन्तव्यम्(*एतत्, dst, ddepth, noScale, alpha, beta))

    पूर्णाङ्क cn = channels();
    पूर्णाङ्क dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *एतत्;
    तार्किक allowTransposed = dims == 1 ||
        dst.विधा() == निवेशः_Array_2::STD_सदिशम् ||
        (dst.fixed_आकार() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    यदि( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.दत्त, src.चरणः, dstMat.दत्त, dstMat.चरणः, src.स्तम्भसङ्ख्या * cn, src.पङ्क्तिकाः, sdepth, ddepth, alpha, beta);
    }
    अन्यथा यदि( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.दत्त, 0, dstMat.दत्त, 0, (पूर्णाङ्क)src.योग() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    द्विमान_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    द्विगुण scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    यदि( dims <= 2 )
    {
        आकार sz = getContinuousSize2D(src, dstMat, cn);
        func(src.दत्त, src.चरणः, 0, 0, dstMat.दत्त, dstMat.चरणः, sz, scale);
    }
    अन्यथा
    {
        ध्रुव Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        आकार sz((पूर्णाङ्क)(it.size*cn), 1);

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

शून्य UMat::convert_गन्तव्यम्(OutputArray dst, पूर्णाङ्क type_, द्विगुण alpha, द्विगुण beta) ध्रुव
{
    CV_INSTRUMENT_REGION();

    यदि (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        प्रत्यागम;
    }

#यदि_परिभाषितम् HAVE_OPENCL
    पूर्णाङ्क stype = type();
    पूर्णाङ्क sdepth = CV_MAT_गभीरता(stype);

    पूर्णाङ्क ddepth = sdepth;
    यदि (type_ >= 0)
        ddepth = CV_MAT_गभीरता(type_);
    अन्यथा
        ddepth = dst.fixedType() ? dst.गभीरता() : sdepth;

    तार्किक noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    यदि (sdepth == ddepth && noScale)
    {
        copy_गन्तव्यम्(dst);
        प्रत्यागम;
    }

    CV_OCL_सततम्(dims <= 2,
               ocl_convert_गन्तव्यम्(*एतत्, dst, ddepth, noScale, alpha, beta))
#यदि_अन्तः // HAVE_OPENCL

    UMat src = *एतत्;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convert_गन्तव्यम्(dst, type_, alpha, beta);
    (शून्य)src;
}

} // namespace cv
