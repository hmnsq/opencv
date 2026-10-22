// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"

#അന്തര്ഭാവ "convert_scale.simd.hpp"
#അന്തര്ഭാവ "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv
{

സ്ഥിതം ദ്വിമാന_Func getCvtScaleAbsFunc(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ദ്വിമാന_Func getConvertScaleFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_convertScaleAbs( നിവേശം_Array _src, OutputArray _dst, ഇരട്ട alpha, ഇരട്ട beta )
{
    സ്ഥിരം ocl::Device & d = ocl::Device::getDefault();

    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    തർക്കം doubleSupport = d.doubleFPConfig() > 0;
    എങ്കിൽ (!doubleSupport && depth == CV_64F)
        തിരികെ അസത്യം;

    _dst.createSameSize(_src, CV_8UC(cn));
    പൂർണ്ണസംഖ്യ kercn = 1;
    എങ്കിൽ (d.isIntel())
    {
        സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    അല്ലെങ്കിൽ
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    പൂർണ്ണസംഖ്യ rowsPerWI = d.isIntel() ? 4 : 1;
    അക്ഷരം cvt[2][50];
    പൂർണ്ണസംഖ്യ wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], വലുപ്പം(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], വലുപ്പം(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    എങ്കിൽ (wdepth == CV_32F)
        k.args(srcarg, dstarg, (ദശാംശം)alpha, (ദശാംശം)beta);
    അല്ലെങ്കിൽ എങ്കിൽ (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

ശൂന്യം convertScaleAbs(നിവേശം_Array _src, OutputArray _dst, ഇരട്ട alpha, ഇരട്ട beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    പൂർണ്ണസംഖ്യ cn = src.channels();
    ഇരട്ട scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    ദ്വിമാന_Func func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    എങ്കിൽ( src.dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((പൂർണ്ണസംഖ്യ)it.size*cn, 1);

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
