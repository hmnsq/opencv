// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"

#અન્તર્ભાવ "convert.simd.hpp"
#અન્તર્ભાવ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv {

નામાકાશ hal {
શૂન્ય cvt16f32f(અચલ hfloat* src, દશાંશ* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
શૂન્ય cvt32f16f(અચલ દશાંશ* src, hfloat* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
શૂન્ય cvt32f16bf(અચલ દશાંશ* src, bfloat* dst, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
શૂન્ય addRNGBias32f(દશાંશ* arr, અચલ દશાંશ* scaleBiasPairs, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
શૂન્ય addRNGBias64f(દ્વિગુણ* arr, અચલ દ્વિગુણ* scaleBiasPairs, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


દ્વિઅંકી_Func getConvertFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
સ્થિર તાર્કિક ocl_convertTo(આગત_Array src_, OutputArray dst_, પૂર્ણાંક ddepth, તાર્કિક noScale, દ્વિગુણ alpha, દ્વિગુણ beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    પૂર્ણાંક stype = src_.type();
    પૂર્ણાંક sdepth = CV_MAT_DEPTH(stype);
    પૂર્ણાંક cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    જો ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        પાછા ખોટું;

    પૂર્ણાંક dtype = CV_MAKETYPE(ddepth, cn);

    પૂર્ણાંક wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    તાર્કિક needDouble = sdepth == CV_64F || ddepth == CV_64F;
    તાર્કિક doubleCheck = સાચું;
    જો (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    તાર્કિક halfCheck = સાચું;
    તાર્કિક needHalf = sdepth == CV_16F || ddepth == CV_16F;
    જો (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    જો (!doubleCheck)
        પાછા ખોટું;
    જો (!halfCheck)
        પાછા ખોટું;

    અચલ પૂર્ણાંક rowsPerWI = 4;

    અક્ષર cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], કદ(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], કદ(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    જો (k.empty())
        પાછા ખોટું;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    દશાંશ alphaf = (દશાંશ)alpha, betaf = (દશાંશ)beta;

    જો (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    નહીંતર જો (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    નહીંતર
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    જો (!k.run(2, globalsize, ખાલી, ખોટું))
        પાછા ખોટું;

    CV_IMPL_ADD(CV_IMPL_OCL);
    પાછા સાચું;
}
#endif

શૂન્ય Mat::convertTo(OutputArray dst, પૂર્ણાંક type_, દ્વિગુણ alpha, દ્વિગુણ beta) અચલ
{
    CV_INSTRUMENT_REGION();

    જો (empty())
    {
        પૂર્ણાંક ddepth = CV_MAT_DEPTH(type_);
        જો (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        અચલ પૂર્ણાંક dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        તાર્કિક allowTransposed = dims == 1 ||
            dst.kind() == આગત_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        પાછા;
    }

    પૂર્ણાંક stype = type();
    પૂર્ણાંક sdepth = CV_MAT_DEPTH(stype);

    પૂર્ણાંક ddepth = sdepth;
    જો (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    નહીંતર
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    તાર્કિક noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    જો (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        પાછા;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*આ, dst, ddepth, noScale, alpha, beta))

    પૂર્ણાંક cn = channels();
    પૂર્ણાંક dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *આ;
    તાર્કિક allowTransposed = dims == 1 ||
        dst.kind() == આગત_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    જો( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    નહીંતર જો( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (પૂર્ણાંક)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    દ્વિઅંકી_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    દ્વિગુણ scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    જો( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    નહીંતર
    {
        અચલ Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((પૂર્ણાંક)(it.size*cn), 1);

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

શૂન્ય UMat::convertTo(OutputArray dst, પૂર્ણાંક type_, દ્વિગુણ alpha, દ્વિગુણ beta) અચલ
{
    CV_INSTRUMENT_REGION();

    જો (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        પાછા;
    }

#ifdef HAVE_OPENCL
    પૂર્ણાંક stype = type();
    પૂર્ણાંક sdepth = CV_MAT_DEPTH(stype);

    પૂર્ણાંક ddepth = sdepth;
    જો (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    નહીંતર
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    તાર્કિક noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    જો (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        પાછા;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*આ, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *આ;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (શૂન્ય)src;
}

} // namespace cv
