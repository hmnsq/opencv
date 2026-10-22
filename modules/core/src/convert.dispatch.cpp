// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "convert.simd.hpp"
#شامل "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

فضاینام cv {

فضاینام hal {
تهی cvt16f32f(ثابت hfloat* src, اعشاری* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
تهی cvt32f16f(ثابت اعشاری* src, hfloat* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
تهی cvt32f16bf(ثابت اعشاری* src, bfloat* dst, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
تهی addRNGBias32f(اعشاری* arr, ثابت اعشاری* scaleBiasPairs, صحیح len, صحیح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
تهی addRNGBias64f(دوگانه* arr, ثابت دوگانه* scaleBiasPairs, صحیح len, صحیح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


دودویی_Func getConvertFunc(صحیح sdepth, صحیح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ایستا منطقی ocl_convertTo(درون_داد_Array src_, OutputArray dst_, صحیح ddepth, منطقی noScale, دوگانه alpha, دوگانه beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    صحیح stype = src_.type();
    صحیح sdepth = CV_MAT_DEPTH(stype);
    صحیح cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    اگر ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        بازگشت نادرست;

    صحیح dtype = CV_MAKETYPE(ddepth, cn);

    صحیح wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    منطقی needDouble = sdepth == CV_64F || ddepth == CV_64F;
    منطقی doubleCheck = درست;
    اگر (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    منطقی halfCheck = درست;
    منطقی needHalf = sdepth == CV_16F || ddepth == CV_16F;
    اگر (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    اگر (!doubleCheck)
        بازگشت نادرست;
    اگر (!halfCheck)
        بازگشت نادرست;

    ثابت صحیح rowsPerWI = 4;

    نویسه cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], اندازه(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], اندازه(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    اگر (k.empty())
        بازگشت نادرست;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    اعشاری alphaf = (اعشاری)alpha, betaf = (اعشاری)beta;

    اگر (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    وگرنه اگر (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    وگرنه
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    اگر (!k.run(2, globalsize, پوچ, نادرست))
        بازگشت نادرست;

    CV_IMPL_ADD(CV_IMPL_OCL);
    بازگشت درست;
}
#endif

تهی Mat::convertTo(OutputArray dst, صحیح type_, دوگانه alpha, دوگانه beta) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر (empty())
    {
        صحیح ddepth = CV_MAT_DEPTH(type_);
        اگر (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        ثابت صحیح dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        منطقی allowTransposed = dims == 1 ||
            dst.kind() == درون_داد_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        بازگشت;
    }

    صحیح stype = type();
    صحیح sdepth = CV_MAT_DEPTH(stype);

    صحیح ddepth = sdepth;
    اگر (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    وگرنه
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    منطقی noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    اگر (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        بازگشت;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*این, dst, ddepth, noScale, alpha, beta))

    صحیح cn = channels();
    صحیح dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *این;
    منطقی allowTransposed = dims == 1 ||
        dst.kind() == درون_داد_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    اگر( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    وگرنه اگر( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (صحیح)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    دودویی_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    دوگانه scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    اگر( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    وگرنه
    {
        ثابت Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((صحیح)(it.size*cn), 1);

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

تهی UMat::convertTo(OutputArray dst, صحیح type_, دوگانه alpha, دوگانه beta) ثابت
{
    CV_INSTRUMENT_REGION();

    اگر (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        بازگشت;
    }

#ifdef HAVE_OPENCL
    صحیح stype = type();
    صحیح sdepth = CV_MAT_DEPTH(stype);

    صحیح ddepth = sdepth;
    اگر (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    وگرنه
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    منطقی noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    اگر (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        بازگشت;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*این, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *این;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (تهی)src;
}

} // namespace cv
