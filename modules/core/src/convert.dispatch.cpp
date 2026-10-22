// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "convert.simd.hpp"
#ಅನ್ತರ್ಭಾವ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ನಾಮಾಕಾಶ hal {
ಶೂನ್ಯ cvt16f32f(ಧ್ರುವ hfloat* src, ಭಿನ್ನ* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಶೂನ್ಯ cvt32f16f(ಧ್ರುವ ಭಿನ್ನ* src, hfloat* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಶೂನ್ಯ cvt32f16bf(ಧ್ರುವ ಭಿನ್ನ* src, bfloat* dst, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಶೂನ್ಯ addRNGBias32f(ಭಿನ್ನ* arr, ಧ್ರುವ ಭಿನ್ನ* scaleBiasPairs, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಶೂನ್ಯ addRNGBias64f(ದ್ವಿಗುಣ* arr, ಧ್ರುವ ದ್ವಿಗುಣ* scaleBiasPairs, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


ದ್ವಿಮಾನ_Func getConvertFunc(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_convertTo(ಒಳಹರಿವು_Array src_, OutputArray dst_, ಪೂರ್ಣಾಙ್ಕ ddepth, ತಾರ್ಕಿಕ noScale, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    ಪೂರ್ಣಾಙ್ಕ stype = src_.type();
    ಪೂರ್ಣಾಙ್ಕ sdepth = CV_MAT_DEPTH(stype);
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    ಯದಿ ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ dtype = CV_MAKETYPE(ddepth, cn);

    ಪೂರ್ಣಾಙ್ಕ wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    ತಾರ್ಕಿಕ needDouble = sdepth == CV_64F || ddepth == CV_64F;
    ತಾರ್ಕಿಕ doubleCheck = ಸತ್ಯ;
    ಯದಿ (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    ತಾರ್ಕಿಕ halfCheck = ಸತ್ಯ;
    ತಾರ್ಕಿಕ needHalf = sdepth == CV_16F || ddepth == CV_16F;
    ಯದಿ (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    ಯದಿ (!doubleCheck)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!halfCheck)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ rowsPerWI = 4;

    ವರ್ಣ cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], ಪರಿಮಾಣ(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], ಪರಿಮಾಣ(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    ಭಿನ್ನ alphaf = (ಭಿನ್ನ)alpha, betaf = (ಭಿನ್ನ)beta;

    ಯದಿ (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    ಅನ್ಯಥಾ ಯದಿ (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    ಅನ್ಯಥಾ
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    ಯದಿ (!k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    CV_IMPL_ADD(CV_IMPL_OCL);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif

ಶೂನ್ಯ Mat::convertTo(OutputArray dst, ಪೂರ್ಣಾಙ್ಕ type_, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ (empty())
    {
        ಪೂರ್ಣಾಙ್ಕ ddepth = CV_MAT_DEPTH(type_);
        ಯದಿ (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        ತಾರ್ಕಿಕ allowTransposed = dims == 1 ||
            dst.kind() == ಒಳಹರಿವು_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪೂರ್ಣಾಙ್ಕ stype = type();
    ಪೂರ್ಣಾಙ್ಕ sdepth = CV_MAT_DEPTH(stype);

    ಪೂರ್ಣಾಙ್ಕ ddepth = sdepth;
    ಯದಿ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    ಅನ್ಯಥಾ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    ತಾರ್ಕಿಕ noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ಯದಿ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ಪ್ರತ್ಯಾಗಮ;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*ಏತತ್, dst, ddepth, noScale, alpha, beta))

    ಪೂರ್ಣಾಙ್ಕ cn = channels();
    ಪೂರ್ಣಾಙ್ಕ dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *ಏತತ್;
    ತಾರ್ಕಿಕ allowTransposed = dims == 1 ||
        dst.kind() == ಒಳಹರಿವು_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    ಯದಿ( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    ಅನ್ಯಥಾ ಯದಿ( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (ಪೂರ್ಣಾಙ್ಕ)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    ದ್ವಿಮಾನ_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    ದ್ವಿಗುಣ scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    ಯದಿ( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((ಪೂರ್ಣಾಙ್ಕ)(it.size*cn), 1);

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

ಶೂನ್ಯ UMat::convertTo(OutputArray dst, ಪೂರ್ಣಾಙ್ಕ type_, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    ಯದಿ (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ಪ್ರತ್ಯಾಗಮ;
    }

#ifdef HAVE_OPENCL
    ಪೂರ್ಣಾಙ್ಕ stype = type();
    ಪೂರ್ಣಾಙ್ಕ sdepth = CV_MAT_DEPTH(stype);

    ಪೂರ್ಣಾಙ್ಕ ddepth = sdepth;
    ಯದಿ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    ಅನ್ಯಥಾ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    ತಾರ್ಕಿಕ noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ಯದಿ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ಪ್ರತ್ಯಾಗಮ;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*ಏತತ್, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *ಏತತ್;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (ಶೂನ್ಯ)src;
}

} // namespace cv
