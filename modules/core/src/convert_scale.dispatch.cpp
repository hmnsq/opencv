// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "convert_scale.simd.hpp"
#অন্তর্ভাব "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv
{

স্থির দ্বিমিক_Func getCvtScaleAbsFunc(পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

দ্বিমিক_Func getConvertScaleFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_convertScaleAbs( প্রবেশ_Array _src, OutputArray _dst, দ্বিগুণ alpha, দ্বিগুণ beta )
{
    ধ্রুবক ocl::Device & d = ocl::Device::getDefault();

    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;
    যদি (!doubleSupport && depth == CV_64F)
        ফেরত মিথ্যা;

    _dst.create_Same_আকার(_src, CV_8UC(cn));
    পূর্ণসংখ্যা kercn = 1;
    যদি (d.isIntel())
    {
        স্থির ধ্রুবক পূর্ণসংখ্যা vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    অন্যথা
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    পূর্ণসংখ্যা rowsPerWI = d.isIntel() ? 4 : 1;
    অক্ষর cvt[2][50];
    পূর্ণসংখ্যা wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], আকার(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], আকার(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_আকার(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    যদি (wdepth == CV_32F)
        k.args(srcarg, dstarg, (দশমিক)alpha, (দশমিক)beta);
    অন্যথা যদি (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

শূন্য convertScaleAbs(প্রবেশ_Array _src, OutputArray _dst, দ্বিগুণ alpha, দ্বিগুণ beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    পূর্ণসংখ্যা cn = src.channels();
    দ্বিগুণ scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    দ্বিমিক_Func func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    যদি( src.dims <= 2 )
    {
        আকার_2 sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    অন্যথা
    {
        ধ্রুবক Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        আকার_2 sz((পূর্ণসংখ্যা)it.size*cn, 1);

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
