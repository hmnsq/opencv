// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "convert_scale.simd.hpp"
#ಅನ್ತರ್ಭಾವ "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv
{

ಸ್ಥಿರ ದ್ವಿಮಾನ_Func getCvtScaleAbsFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ದ್ವಿಮಾನ_Func getConvertScaleFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_convertScaleAbs( ಒಳಹರಿವು_Array _src, OutputArray _dst, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta )
{
    ಸ್ಥಿರಾಂಕ ocl::Device & d = ocl::Device::getDefault();

    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type);
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಆದರೆ (!doubleSupport && ಆಳ == CV_64F)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    _dst.create_Same_ಗಾತ್ರ(_src, CV_8UC(cn));
    ಪೂರ್ಣಾಂಕ kercn = 1;
    ಆದರೆ (d.isIntel())
    {
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಸದಿಶ_Widths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::check_Optimal_ಸದಿಶ_ಅಗಲ( ಸದಿಶ_Widths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_ಸದಿಶ_MAX);
    }
    ಇಲ್ಲದಿದ್ದರೆ
        kercn = ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ_Max(_src, _dst);

    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;
    ಅಕ್ಷರ cvt[2][50];
    ಪೂರ್ಣಾಂಕ wdepth = std::max(ಆಳ, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::type_ಗಮ್ಯ_Str(CV_8UC(kercn)), CV_8U,
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)),
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(ಆಳ, wdepth, kercn, cvt[0], ಗಾತ್ರ(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], ಗಾತ್ರ(cvt[1])),
                         ocl::type_ಗಮ್ಯ_Str(wdepth), ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ಆದರೆ (wdepth == CV_32F)
        k.ವಾದಗಳು(srcarg, dstarg, (ದಶಮಾಂಶ)alpha, (ದಶಮಾಂಶ)beta);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (wdepth == CV_64F)
        k.ವಾದಗಳು(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)src.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ convertScaleAbs(ಒಳಹರಿವು_Array _src, OutputArray _dst, ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ಸತತ(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    ಪೂರ್ಣಾಂಕ cn = src.channels();
    ದ್ವಿಗುಣ scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    ದ್ವಿಮಾನ_Func func = getCvtScaleAbsFunc(src.ಆಳ());
    CV_Assert( func != 0 );

    ಆದರೆ( src.dims <= 2 )
    {
        ಗಾತ್ರ_2 sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.ಹೆಜ್ಜೆ, 0, 0, dst.ptr(), dst.ಹೆಜ್ಜೆ, sz, scale );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ಗಾತ್ರ_2 sz((ಪೂರ್ಣಾಂಕ)it.size*cn, 1);

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
