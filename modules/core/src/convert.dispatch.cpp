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
فراغ addRNGBias32f(عائم* arr, ثابت عائم* scaleBiasPairs, صحيح len, صحيح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
فراغ addRNGBias64f(مزدوج* arr, ثابت مزدوج* scaleBiasPairs, صحيح len, صحيح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


BinaryFunc getConvertFunc(صحيح sdepth, صحيح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ساكن منطقي ocl_convertTo(InputArray src_, OutputArray dst_, صحيح ddepth, منطقي noScale, مزدوج alpha, مزدوج beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    صحيح stype = src_.type();
    صحيح sdepth = CV_MAT_DEPTH(stype);
    صحيح cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    ܐܢ ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ܦܢܐ ܕܓܠܐ;

    صحيح dtype = CV_MAKETYPE(ddepth, cn);

    صحيح wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    منطقي needDouble = sdepth == CV_64F || ddepth == CV_64F;
    منطقي doubleCheck = ܫܪܪܐ;
    ܐܢ (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    منطقي halfCheck = ܫܪܪܐ;
    منطقي needHalf = sdepth == CV_16F || ddepth == CV_16F;
    ܐܢ (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    ܐܢ (!doubleCheck)
        ܦܢܐ ܕܓܠܐ;
    ܐܢ (!halfCheck)
        ܦܢܐ ܕܓܠܐ;

    ثابت صحيح rowsPerWI = 4;

    حرف cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], حجم(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    ܐܢ (k.empty())
        ܦܢܐ ܕܓܠܐ;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    عائم alphaf = (عائم)alpha, betaf = (عائم)beta;

    ܐܢ (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    ܐܚܪܢܐ ܐܢ (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    ܐܚܪܢܐ
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    ܐܢ (!k.run(2, globalsize, عدم, ܕܓܠܐ))
        ܦܢܐ ܕܓܠܐ;

    CV_IMPL_ADD(CV_IMPL_OCL);
    ܦܢܐ ܫܪܪܐ;
}
#endif

فراغ Mat::convertTo(OutputArray dst, صحيح type_, مزدوج alpha, مزدوج beta) ثابت
{
    CV_INSTRUMENT_REGION();

    ܐܢ (empty())
    {
        صحيح ddepth = CV_MAT_DEPTH(type_);
        ܐܢ (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        ثابت صحيح dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        منطقي allowTransposed = dims == 1 ||
            dst.kind() == _InputArray::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ܦܢܐ;
    }

    صحيح stype = type();
    صحيح sdepth = CV_MAT_DEPTH(stype);

    صحيح ddepth = sdepth;
    ܐܢ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    ܐܚܪܢܐ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    منطقي noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ܐܢ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ܦܢܐ;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*هذا, dst, ddepth, noScale, alpha, beta))

    صحيح cn = channels();
    صحيح dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *هذا;
    منطقي allowTransposed = dims == 1 ||
        dst.kind() == _InputArray::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    ܐܢ( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    ܐܚܪܢܐ ܐܢ( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (صحيح)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    BinaryFunc func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    مزدوج scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    ܐܢ( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    ܐܚܪܢܐ
    {
        ثابت Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((صحيح)(it.size*cn), 1);

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

فراغ UMat::convertTo(OutputArray dst, صحيح type_, مزدوج alpha, مزدوج beta) ثابت
{
    CV_INSTRUMENT_REGION();

    ܐܢ (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ܦܢܐ;
    }

#ifdef HAVE_OPENCL
    صحيح stype = type();
    صحيح sdepth = CV_MAT_DEPTH(stype);

    صحيح ddepth = sdepth;
    ܐܢ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    ܐܚܪܢܐ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    منطقي noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ܐܢ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ܦܢܐ;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*هذا, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *هذا;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (فراغ)src;
}

} // namespace cv
