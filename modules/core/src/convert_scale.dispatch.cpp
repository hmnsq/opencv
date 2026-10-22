// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "convert_scale.simd.hpp"
#অন্তর্ভাব "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv
{

স্থির দ্বিমিক_Func getCvtScaleAbsFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

দ্বিমিক_Func getConvertScaleFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_convertScaleAbs( প্রবেশ_Array _src, OutputArray _dst, দ্বিগুণ alpha, দ্বিগুণ beta )
{
    ধ্রুব ocl::Device & d = ocl::Device::getDefault();

    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    তার্কিক doubleSupport = d.doubleFPConfig() > 0;
    যদি (!doubleSupport && depth == CV_64F)
        প্রত্যাগম অসত্য;

    _dst.createSameSize(_src, CV_8UC(cn));
    পূর্ণাঙ্ক kercn = 1;
    যদি (d.isIntel())
    {
        স্থির ধ্রুব পূর্ণাঙ্ক vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    অন্যথা
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    পূর্ণাঙ্ক rowsPerWI = d.isIntel() ? 4 : 1;
    বর্ণ cvt[2][50];
    পূর্ণাঙ্ক wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], পরিমাণ(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], পরিমাণ(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    যদি (wdepth == CV_32F)
        k.args(srcarg, dstarg, (ভিন্ন)alpha, (ভিন্ন)beta);
    অন্যথা যদি (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    প্রত্যাগম k.run(2, globalsize, রিক্ত, অসত্য);
}

#endif

শূন্য convertScaleAbs(প্রবেশ_Array _src, OutputArray _dst, দ্বিগুণ alpha, দ্বিগুণ beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    পূর্ণাঙ্ক cn = src.channels();
    দ্বিগুণ scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    দ্বিমিক_Func func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    যদি( src.dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    অন্যথা
    {
        ধ্রুব Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((পূর্ণাঙ্ক)it.size*cn, 1);

        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
