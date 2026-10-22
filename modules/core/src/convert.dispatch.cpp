// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencl_kernels_core.hpp"

#অন্তর্ভাৱ "convert.simd.hpp"
#অন্তর্ভাৱ "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

নামাকাশ hal {
শূন্য cvt16f32f(ধ্রুৱ hfloat* src, ভিন্ন* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
শূন্য cvt32f16f(ধ্রুৱ ভিন্ন* src, hfloat* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
শূন্য cvt32f16bf(ধ্রুৱ ভিন্ন* src, bfloat* dst, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
শূন্য addRNGBias32f(ভিন্ন* arr, ধ্রুৱ ভিন্ন* scaleBiasPairs, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
শূন্য addRNGBias64f(দ্ৱিগুণ* arr, ধ্রুৱ দ্ৱিগুণ* scaleBiasPairs, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


দ্বিমিক_Func getConvertFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
স্থির তার্কিক ocl_convertTo(প্ৰবেশ_Array src_, OutputArray dst_, পূর্ণাঙ্ক ddepth, তার্কিক noScale, দ্ৱিগুণ alpha, দ্ৱিগুণ beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    পূর্ণাঙ্ক stype = src_.type();
    পূর্ণাঙ্ক sdepth = CV_MAT_DEPTH(stype);
    পূর্ণাঙ্ক cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    যদি ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ঘূৰাই মিছা;

    পূর্ণাঙ্ক dtype = CV_MAKETYPE(ddepth, cn);

    পূর্ণাঙ্ক wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    তার্কিক needDouble = sdepth == CV_64F || ddepth == CV_64F;
    তার্কিক doubleCheck = সঁচা;
    যদি (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    তার্কিক halfCheck = সঁচা;
    তার্কিক needHalf = sdepth == CV_16F || ddepth == CV_16F;
    যদি (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    যদি (!doubleCheck)
        ঘূৰাই মিছা;
    যদি (!halfCheck)
        ঘূৰাই মিছা;

    ধ্রুৱ পূর্ণাঙ্ক rowsPerWI = 4;

    ৱর্ণ cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], পরিমাণ(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], পরিমাণ(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    যদি (k.empty())
        ঘূৰাই মিছা;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    ভিন্ন alphaf = (ভিন্ন)alpha, betaf = (ভিন্ন)beta;

    যদি (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    অন্যথা যদি (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    অন্যথা
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    যদি (!k.run(2, globalsize, রিক্ত, মিছা))
        ঘূৰাই মিছা;

    CV_IMPL_ADD(CV_IMPL_OCL);
    ঘূৰাই সঁচা;
}
#endif

শূন্য Mat::convertTo(OutputArray dst, পূর্ণাঙ্ক type_, দ্ৱিগুণ alpha, দ্ৱিগুণ beta) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি (empty())
    {
        পূর্ণাঙ্ক ddepth = CV_MAT_DEPTH(type_);
        যদি (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        ধ্রুৱ পূর্ণাঙ্ক dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        তার্কিক allowTransposed = dims == 1 ||
            dst.kind() == প্ৰবেশ_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ঘূৰাই;
    }

    পূর্ণাঙ্ক stype = type();
    পূর্ণাঙ্ক sdepth = CV_MAT_DEPTH(stype);

    পূর্ণাঙ্ক ddepth = sdepth;
    যদি (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    অন্যথা
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    তার্কিক noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    যদি (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ঘূৰাই;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*এতত্, dst, ddepth, noScale, alpha, beta))

    পূর্ণাঙ্ক cn = channels();
    পূর্ণাঙ্ক dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *এতত্;
    তার্কিক allowTransposed = dims == 1 ||
        dst.kind() == প্ৰবেশ_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    যদি( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    অন্যথা যদি( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (পূর্ণাঙ্ক)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    দ্বিমিক_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    দ্ৱিগুণ scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    যদি( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    অন্যথা
    {
        ধ্রুৱ Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((পূর্ণাঙ্ক)(it.size*cn), 1);

        বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

শূন্য UMat::convertTo(OutputArray dst, পূর্ণাঙ্ক type_, দ্ৱিগুণ alpha, দ্ৱিগুণ beta) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    যদি (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ঘূৰাই;
    }

#ifdef HAVE_OPENCL
    পূর্ণাঙ্ক stype = type();
    পূর্ণাঙ্ক sdepth = CV_MAT_DEPTH(stype);

    পূর্ণাঙ্ক ddepth = sdepth;
    যদি (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    অন্যথা
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    তার্কিক noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    যদি (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ঘূৰাই;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*এতত্, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *এতত্;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (শূন্য)src;
}

} // namespace cv
