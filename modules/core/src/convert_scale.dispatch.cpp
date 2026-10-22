// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "convert_scale.simd.hpp"
#ಅನ್ತರ್ಭಾವ "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv
{

ಸ್ಥಿರ ದ್ವಿಮಾನ_Func getCvtScaleAbsFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ದ್ವಿಮಾನ_Func getConvertScaleFunc(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_convertScaleAbs( ಒಳಹರಿವು_Array _src, OutputArray _dst, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta )
{
    ಧ್ರುವ ocl::Device & d = ocl::Device::getDefault();

    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಯದಿ (!doubleSupport && depth == CV_64F)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    _dst.createSameSize(_src, CV_8UC(cn));
    ಪೂರ್ಣಾಙ್ಕ kercn = 1;
    ಯದಿ (d.isIntel())
    {
        ಸ್ಥಿರ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    ಅನ್ಯಥಾ
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    ಪೂರ್ಣಾಙ್ಕ rowsPerWI = d.isIntel() ? 4 : 1;
    ವರ್ಣ cvt[2][50];
    ಪೂರ್ಣಾಙ್ಕ wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], ಪರಿಮಾಣ(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], ಪರಿಮಾಣ(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ಯದಿ (wdepth == CV_32F)
        k.args(srcarg, dstarg, (ಭಿನ್ನ)alpha, (ಭಿನ್ನ)beta);
    ಅನ್ಯಥಾ ಯದಿ (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಶೂನ್ಯ convertScaleAbs(ಒಳಹರಿವು_Array _src, OutputArray _dst, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    ಪೂರ್ಣಾಙ್ಕ cn = src.channels();
    ದ್ವಿಗುಣ scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    ದ್ವಿಮಾನ_Func func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    ಯದಿ( src.dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((ಪೂರ್ಣಾಙ್ಕ)it.size*cn, 1);

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
