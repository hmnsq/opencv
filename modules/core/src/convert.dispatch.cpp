// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "convert.simd.hpp"
#ಅನ್ತರ್ಭಾವ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ನಾಮಾಕಾಶ hal {
ಶೂನ್ಯ cvt16f32f(ಸ್ಥಿರಾಂಕ hfloat* src, ದಶಮಾಂಶ* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಶೂನ್ಯ cvt32f16f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, hfloat* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಶೂನ್ಯ cvt32f16bf(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src, bfloat* dst, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಶೂನ್ಯ ಕೂಡಿಸು_RNGBias32f(ದಶಮಾಂಶ* arr, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* scale_Bias_ಜೋಡಿಗಳು, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(ಕೂಡಿಸು_RNGBias32f, (arr, scale_Bias_ಜೋಡಿಗಳು, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಶೂನ್ಯ ಕೂಡಿಸು_RNGBias64f(ದ್ವಿಗುಣ* arr, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* scale_Bias_ಜೋಡಿಗಳು, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(ಕೂಡಿಸು_RNGBias64f, (arr, scale_Bias_ಜೋಡಿಗಳು, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


ದ್ವಿಮಾನ_Func getConvertFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_convert_ಗಮ್ಯ(ಒಳಹರಿವು_Array src_, OutputArray dst_, ಪೂರ್ಣಾಂಕ ddepth, ತಾರ್ಕಿಕ noScale, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    ಪೂರ್ಣಾಂಕ stype = src_.type();
    ಪೂರ್ಣಾಂಕ sdepth = CV_MAT_ಆಳ(stype);
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    ಆದರೆ ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ dtype = CV_MAKETYPE(ddepth, cn);

    ಪೂರ್ಣಾಂಕ wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    ತಾರ್ಕಿಕ needDouble = sdepth == CV_64F || ddepth == CV_64F;
    ತಾರ್ಕಿಕ doubleCheck = ನಿಜ;
    ಆದರೆ (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    ತಾರ್ಕಿಕ halfCheck = ನಿಜ;
    ತಾರ್ಕಿಕ needHalf = sdepth == CV_16F || ddepth == CV_16F;
    ಆದರೆ (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    ಆದರೆ (!doubleCheck)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಆದರೆ (!halfCheck)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = 4;

    ಅಕ್ಷರ cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::type_ಗಮ್ಯ_Str(sdepth), ocl::type_ಗಮ್ಯ_Str(wdepth), ocl::type_ಗಮ್ಯ_Str(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], ಗಾತ್ರ(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], ಗಾತ್ರ(cvt[1])),
                         ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = src_.getUMat();
    dst_.create_Same_ಗಾತ್ರ(src_, dtype);
    UMat dst = dst_.getUMat();

    ದಶಮಾಂಶ alphaf = (ದಶಮಾಂಶ)alpha, betaf = (ದಶಮಾಂಶ)beta;

    ಆದರೆ (noScale)
        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), ocl::KernelArg::WriteOnly(dst, cn));
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (wdepth == CV_32F)
        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    ಇಲ್ಲದಿದ್ದರೆ
        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn,
        div_ಮೇಲೆ((size_t)dst.ಅಡ್ಡಸಾಲುಗಳು, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI)
    };
    ಆದರೆ (!k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    CV_IMPL_ಕೂಡಿಸು(CV_IMPL_OCL);
    ಹಿಂತಿರುಗಿ ನಿಜ;
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ Mat::convert_ಗಮ್ಯ(OutputArray dst, ಪೂರ್ಣಾಂಕ type_, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (empty())
    {
        ಪೂರ್ಣಾಂಕ ddepth = CV_MAT_ಆಳ(type_);
        ಆದರೆ (ddepth < 0)
            ddepth = dst.fixedType() ? dst.ಆಳ() : ಆಳ();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        ತಾರ್ಕಿಕ allowTransposed = dims == 1 ||
            dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ ||
            (dst.fixed_ಗಾತ್ರ() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ಹಿಂತಿರುಗಿ;
    }

    ಪೂರ್ಣಾಂಕ stype = type();
    ಪೂರ್ಣಾಂಕ sdepth = CV_MAT_ಆಳ(stype);

    ಪೂರ್ಣಾಂಕ ddepth = sdepth;
    ಆದರೆ (type_ >= 0)
        ddepth = CV_MAT_ಆಳ(type_);
    ಇಲ್ಲದಿದ್ದರೆ
        ddepth = dst.fixedType() ? dst.ಆಳ() : sdepth;

    ತಾರ್ಕಿಕ noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ಆದರೆ (sdepth == ddepth && noScale)
    {
        copy_ಗಮ್ಯ(dst);
        ಹಿಂತಿರುಗಿ;
    }

    CV_OCL_ಸತತ(dims <= 2 && dst.isUMat(),
               ocl_convert_ಗಮ್ಯ(*ಇದು, dst, ddepth, noScale, alpha, beta))

    ಪೂರ್ಣಾಂಕ cn = channels();
    ಪೂರ್ಣಾಂಕ dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *ಇದು;
    ತಾರ್ಕಿಕ allowTransposed = dims == 1 ||
        dst.ಬಗೆ() == ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ ||
        (dst.fixed_ಗಾತ್ರ() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    ಆದರೆ( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, dstMat.ದತ್ತಾಂಶ, dstMat.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn, src.ಅಡ್ಡಸಾಲುಗಳು, sdepth, ddepth, alpha, beta);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.ದತ್ತಾಂಶ, 0, dstMat.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    ದ್ವಿಮಾನ_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    ದ್ವಿಗುಣ scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    ಆದರೆ( dims <= 2 )
    {
        ಗಾತ್ರ_2 sz = getContinuousSize2D(src, dstMat, cn);
        func(src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, 0, 0, dstMat.ದತ್ತಾಂಶ, dstMat.ಹೆಜ್ಜೆ, sz, scale);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ಗಾತ್ರ_2 sz((ಪೂರ್ಣಾಂಕ)(it.size*cn), 1);

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

ಶೂನ್ಯ UMat::convert_ಗಮ್ಯ(OutputArray dst, ಪೂರ್ಣಾಂಕ type_, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ಹಿಂತಿರುಗಿ;
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ಪೂರ್ಣಾಂಕ stype = type();
    ಪೂರ್ಣಾಂಕ sdepth = CV_MAT_ಆಳ(stype);

    ಪೂರ್ಣಾಂಕ ddepth = sdepth;
    ಆದರೆ (type_ >= 0)
        ddepth = CV_MAT_ಆಳ(type_);
    ಇಲ್ಲದಿದ್ದರೆ
        ddepth = dst.fixedType() ? dst.ಆಳ() : sdepth;

    ತಾರ್ಕಿಕ noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ಆದರೆ (sdepth == ddepth && noScale)
    {
        copy_ಗಮ್ಯ(dst);
        ಹಿಂತಿರುಗಿ;
    }

    CV_OCL_ಸತತ(dims <= 2,
               ocl_convert_ಗಮ್ಯ(*ಇದು, dst, ddepth, noScale, alpha, beta))
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENCL

    UMat src = *ಇದು;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convert_ಗಮ್ಯ(dst, type_, alpha, beta);
    (ಶೂನ್ಯ)src;
}

} // namespace cv
