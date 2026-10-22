// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

#ଅନ୍ତର୍ଭାଵ "convert_scale.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv
{

ସ୍ଥିର ଦ୍ୱିମିକ_Func getCvtScaleAbsFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଦ୍ୱିମିକ_Func getConvertScaleFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_convertScaleAbs( ନିବେଶ_Array _src, OutputArray _dst, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta )
{
    ଅଚଳ ocl::Device & d = ocl::Device::getDefault();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;
    ଯଦି (!doubleSupport && depth == CV_64F)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    _dst.createSameSize(_src, CV_8UC(cn));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = 1;
    ଯଦି (d.isIntel())
    {
        ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    ଅନ୍ୟଥା
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rowsPerWI = d.isIntel() ? 4 : 1;
    ଅକ୍ଷର cvt[2][50];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], ଆକାର(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], ଆକାର(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ଯଦି (wdepth == CV_32F)
        k.args(srcarg, dstarg, (ଦଶମିକ)alpha, (ଦଶମିକ)beta);
    ଅନ୍ୟଥା ଯଦି (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ଶୂନ୍ୟ convertScaleAbs(ନିବେଶ_Array _src, OutputArray _dst, ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = src.channels();
    ଦ୍ୱିଗୁଣ scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    ଦ୍ୱିମିକ_Func func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    ଯଦି( src.dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size*cn, 1);

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
