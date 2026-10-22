// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "convert.simd.hpp"
#অন্তর্ভাব "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

নামাকাশ hal {
শূন্য cvt16f32f(ধ্রুবক hfloat* src, দশমিক* dst, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
শূন্য cvt32f16f(ধ্রুবক দশমিক* src, hfloat* dst, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
শূন্য cvt32f16bf(ধ্রুবক দশমিক* src, bfloat* dst, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
শূন্য addRNGBias32f(দশমিক* arr, ধ্রুবক দশমিক* scaleBiasPairs, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
শূন্য addRNGBias64f(দ্বিগুণ* arr, ধ্রুবক দ্বিগুণ* scaleBiasPairs, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


দ্বিমিক_Func getConvertFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
স্থির যৌক্তিক ocl_convertTo(প্রবেশ_Array src_, OutputArray dst_, পূর্ণসংখ্যা ddepth, যৌক্তিক noScale, দ্বিগুণ alpha, দ্বিগুণ beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    পূর্ণসংখ্যা stype = src_.type();
    পূর্ণসংখ্যা sdepth = CV_MAT_DEPTH(stype);
    পূর্ণসংখ্যা cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    যদি ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা dtype = CV_MAKETYPE(ddepth, cn);

    পূর্ণসংখ্যা wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    যৌক্তিক needDouble = sdepth == CV_64F || ddepth == CV_64F;
    যৌক্তিক doubleCheck = সত্য;
    যদি (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    যৌক্তিক halfCheck = সত্য;
    যৌক্তিক needHalf = sdepth == CV_16F || ddepth == CV_16F;
    যদি (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    যদি (!doubleCheck)
        ফেরত মিথ্যা;
    যদি (!halfCheck)
        ফেরত মিথ্যা;

    ধ্রুবক পূর্ণসংখ্যা rowsPerWI = 4;

    অক্ষর cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], আকার(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], আকার(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src = src_.getUMat();
    dst_.create_Same_আকার(src_, dtype);
    UMat dst = dst_.getUMat();

    দশমিক alphaf = (দশমিক)alpha, betaf = (দশমিক)beta;

    যদি (noScale)
        k.args(ocl::KernelArg::Read_Only_No_আকার(src), ocl::KernelArg::WriteOnly(dst, cn));
    অন্যথা যদি (wdepth == CV_32F)
        k.args(ocl::KernelArg::Read_Only_No_আকার(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    অন্যথা
        k.args(ocl::KernelArg::Read_Only_No_আকার(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    যদি (!k.run(2, globalsize, ফাঁকা, মিথ্যা))
        ফেরত মিথ্যা;

    CV_IMPL_ADD(CV_IMPL_OCL);
    ফেরত সত্য;
}
#endif

শূন্য Mat::convertTo(OutputArray dst, পূর্ণসংখ্যা type_, দ্বিগুণ alpha, দ্বিগুণ beta) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি (empty())
    {
        পূর্ণসংখ্যা ddepth = CV_MAT_DEPTH(type_);
        যদি (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        ধ্রুবক পূর্ণসংখ্যা dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        যৌক্তিক allowTransposed = dims == 1 ||
            dst.kind() == প্রবেশ_Array_2::STD_VECTOR ||
            (dst.fixed_আকার() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ফেরত;
    }

    পূর্ণসংখ্যা stype = type();
    পূর্ণসংখ্যা sdepth = CV_MAT_DEPTH(stype);

    পূর্ণসংখ্যা ddepth = sdepth;
    যদি (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    অন্যথা
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    যৌক্তিক noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    যদি (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ফেরত;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*এই, dst, ddepth, noScale, alpha, beta))

    পূর্ণসংখ্যা cn = channels();
    পূর্ণসংখ্যা dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *এই;
    যৌক্তিক allowTransposed = dims == 1 ||
        dst.kind() == প্রবেশ_Array_2::STD_VECTOR ||
        (dst.fixed_আকার() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    যদি( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.তথ্য, src.step, dstMat.তথ্য, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    অন্যথা যদি( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.তথ্য, 0, dstMat.তথ্য, 0, (পূর্ণসংখ্যা)src.যোগ() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    দ্বিমিক_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    দ্বিগুণ scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    যদি( dims <= 2 )
    {
        আকার_2 sz = getContinuousSize2D(src, dstMat, cn);
        func(src.তথ্য, src.step, 0, 0, dstMat.তথ্য, dstMat.step, sz, scale);
    }
    অন্যথা
    {
        ধ্রুবক Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        আকার_2 sz((পূর্ণসংখ্যা)(it.size*cn), 1);

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

শূন্য UMat::convertTo(OutputArray dst, পূর্ণসংখ্যা type_, দ্বিগুণ alpha, দ্বিগুণ beta) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    যদি (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ফেরত;
    }

#ifdef HAVE_OPENCL
    পূর্ণসংখ্যা stype = type();
    পূর্ণসংখ্যা sdepth = CV_MAT_DEPTH(stype);

    পূর্ণসংখ্যা ddepth = sdepth;
    যদি (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    অন্যথা
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    যৌক্তিক noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    যদি (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ফেরত;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*এই, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *এই;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (শূন্য)src;
}

} // namespace cv
