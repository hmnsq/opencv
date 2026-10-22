// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"

#அந்தர்பாவ "convert_scale.simd.hpp"
#அந்தர்பாவ "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv
{

நிலையான இருமம்_Func getCvtScaleAbsFunc(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

இருமம்_Func getConvertScaleFunc(முழு sdepth, முழு ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_convertScaleAbs( உள்ளீடு_Array _src, OutputArray _dst, இரட்டை alpha, இரட்டை beta )
{
    மாறிலி ocl::Device & d = ocl::Device::getDefault();

    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    தருக்க doubleSupport = d.doubleFPConfig() > 0;
    எனில் (!doubleSupport && depth == CV_64F)
        திரும்பு பொய்;

    _dst.createSameSize(_src, CV_8UC(cn));
    முழு kercn = 1;
    எனில் (d.isIntel())
    {
        நிலையான மாறிலி முழு vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    இல்லையெனில்
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    முழு rowsPerWI = d.isIntel() ? 4 : 1;
    எழுத்து cvt[2][50];
    முழு wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], அளவு(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], அளவு(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    எனில் (wdepth == CV_32F)
        k.args(srcarg, dstarg, (தசம)alpha, (தசம)beta);
    இல்லையெனில் எனில் (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

வெறுமை convertScaleAbs(உள்ளீடு_Array _src, OutputArray _dst, இரட்டை alpha, இரட்டை beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    முழு cn = src.channels();
    இரட்டை scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    இருமம்_Func func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    எனில்( src.dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    இல்லையெனில்
    {
        மாறிலி Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((முழு)it.size*cn, 1);

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
