// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"

#અન્તર્ભાવ "convert_scale.simd.hpp"
#અન્તર્ભાવ "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv
{

સ્થિર દ્વિઅંકી_Func getCvtScaleAbsFunc(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

દ્વિઅંકી_Func getConvertScaleFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_convertScaleAbs( આગત_Array _src, OutputArray _dst, દ્વિગુણ alpha, દ્વિગુણ beta )
{
    અચલ ocl::Device & d = ocl::Device::getDefault();

    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    તાર્કિક doubleSupport = d.doubleFPConfig() > 0;
    જો (!doubleSupport && depth == CV_64F)
        પાછા ખોટું;

    _dst.createSameSize(_src, CV_8UC(cn));
    પૂર્ણાંક kercn = 1;
    જો (d.isIntel())
    {
        સ્થિર અચલ પૂર્ણાંક vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    નહીંતર
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    પૂર્ણાંક rowsPerWI = d.isIntel() ? 4 : 1;
    અક્ષર cvt[2][50];
    પૂર્ણાંક wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], કદ(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], કદ(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    જો (k.empty())
        પાછા ખોટું;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    જો (wdepth == CV_32F)
        k.args(srcarg, dstarg, (દશાંશ)alpha, (દશાંશ)beta);
    નહીંતર જો (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

શૂન્ય convertScaleAbs(આગત_Array _src, OutputArray _dst, દ્વિગુણ alpha, દ્વિગુણ beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    પૂર્ણાંક cn = src.channels();
    દ્વિગુણ scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    દ્વિઅંકી_Func func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    જો( src.dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    નહીંતર
    {
        અચલ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((પૂર્ણાંક)it.size*cn, 1);

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
