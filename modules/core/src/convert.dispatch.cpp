// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"

#അന്തര്ഭാവ "convert.simd.hpp"
#അന്തര്ഭാവ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv {

നാമാകാശ hal {
ശൂന്യം cvt16f32f(സ്ഥിരം hfloat* src, ദശാംശം* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ശൂന്യം cvt32f16f(സ്ഥിരം ദശാംശം* src, hfloat* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ശൂന്യം cvt32f16bf(സ്ഥിരം ദശാംശം* src, bfloat* dst, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ശൂന്യം addRNGBias32f(ദശാംശം* arr, സ്ഥിരം ദശാംശം* scaleBiasPairs, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
ശൂന്യം addRNGBias64f(ഇരട്ട* arr, സ്ഥിരം ഇരട്ട* scaleBiasPairs, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


ദ്വിമാന_Func getConvertFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
സ്ഥിതം തർക്കം ocl_convertTo(നിവേശം_Array src_, OutputArray dst_, പൂർണ്ണസംഖ്യ ddepth, തർക്കം noScale, ഇരട്ട alpha, ഇരട്ട beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    പൂർണ്ണസംഖ്യ stype = src_.type();
    പൂർണ്ണസംഖ്യ sdepth = CV_MAT_DEPTH(stype);
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    എങ്കിൽ ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ dtype = CV_MAKETYPE(ddepth, cn);

    പൂർണ്ണസംഖ്യ wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    തർക്കം needDouble = sdepth == CV_64F || ddepth == CV_64F;
    തർക്കം doubleCheck = സത്യം;
    എങ്കിൽ (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    തർക്കം halfCheck = സത്യം;
    തർക്കം needHalf = sdepth == CV_16F || ddepth == CV_16F;
    എങ്കിൽ (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    എങ്കിൽ (!doubleCheck)
        തിരികെ അസത്യം;
    എങ്കിൽ (!halfCheck)
        തിരികെ അസത്യം;

    സ്ഥിരം പൂർണ്ണസംഖ്യ rowsPerWI = 4;

    അക്ഷരം cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], വലുപ്പം(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], വലുപ്പം(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    ദശാംശം alphaf = (ദശാംശം)alpha, betaf = (ദശാംശം)beta;

    എങ്കിൽ (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    അല്ലെങ്കിൽ എങ്കിൽ (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    അല്ലെങ്കിൽ
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    എങ്കിൽ (!k.run(2, globalsize, ശൂന്യ, അസത്യം))
        തിരികെ അസത്യം;

    CV_IMPL_ADD(CV_IMPL_OCL);
    തിരികെ സത്യം;
}
#endif

ശൂന്യം Mat::convertTo(OutputArray dst, പൂർണ്ണസംഖ്യ type_, ഇരട്ട alpha, ഇരട്ട beta) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ (empty())
    {
        പൂർണ്ണസംഖ്യ ddepth = CV_MAT_DEPTH(type_);
        എങ്കിൽ (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        സ്ഥിരം പൂർണ്ണസംഖ്യ dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        തർക്കം allowTransposed = dims == 1 ||
            dst.kind() == നിവേശം_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        തിരികെ;
    }

    പൂർണ്ണസംഖ്യ stype = type();
    പൂർണ്ണസംഖ്യ sdepth = CV_MAT_DEPTH(stype);

    പൂർണ്ണസംഖ്യ ddepth = sdepth;
    എങ്കിൽ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    അല്ലെങ്കിൽ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    തർക്കം noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    എങ്കിൽ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        തിരികെ;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*ഇത്, dst, ddepth, noScale, alpha, beta))

    പൂർണ്ണസംഖ്യ cn = channels();
    പൂർണ്ണസംഖ്യ dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *ഇത്;
    തർക്കം allowTransposed = dims == 1 ||
        dst.kind() == നിവേശം_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    എങ്കിൽ( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (പൂർണ്ണസംഖ്യ)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    ദ്വിമാന_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    ഇരട്ട scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    എങ്കിൽ( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((പൂർണ്ണസംഖ്യ)(it.size*cn), 1);

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

ശൂന്യം UMat::convertTo(OutputArray dst, പൂർണ്ണസംഖ്യ type_, ഇരട്ട alpha, ഇരട്ട beta) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        തിരികെ;
    }

#ifdef HAVE_OPENCL
    പൂർണ്ണസംഖ്യ stype = type();
    പൂർണ്ണസംഖ്യ sdepth = CV_MAT_DEPTH(stype);

    പൂർണ്ണസംഖ്യ ddepth = sdepth;
    എങ്കിൽ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    അല്ലെങ്കിൽ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    തർക്കം noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    എങ്കിൽ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        തിരികെ;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*ഇത്, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *ഇത്;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (ശൂന്യം)src;
}

} // namespace cv
