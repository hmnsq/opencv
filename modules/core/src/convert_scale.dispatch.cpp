// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "convert_scale.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv
{

ਸਥਿਰ ਦੁਆਧਾਰੀ_Func getCvtScaleAbsFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਦੁਆਧਾਰੀ_Func getConvertScaleFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_convertScaleAbs( ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta )
{
    ਅਚਲ ocl::Device & d = ocl::Device::getDefault();

    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type);
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;
    ਜੇ (!doubleSupport && ਡੂੰਘਾਈ == CV_64F)
        ਵਾਪਸ ਝੂਠ;

    _dst.create_Same_ਮਾਪ(_src, CV_8UC(cn));
    ਪੂਰਨ kercn = 1;
    ਜੇ (d.isIntel())
    {
        ਸਥਿਰ ਅਚਲ ਪੂਰਨ ਸਦਿਸ਼_Widths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::check_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ( ਸਦਿਸ਼_Widths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_ਸਦਿਸ਼_MAX);
    }
    ਨਹੀਂਤਾਂ
        kercn = ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ_Max(_src, _dst);

    ਪੂਰਨ ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;
    ਅੱਖਰ cvt[2][50];
    ਪੂਰਨ wdepth = std::max(ਡੂੰਘਾਈ, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_8UC(kercn)), CV_8U,
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(ਡੂੰਘਾਈ, wdepth, kercn, cvt[0], ਆਕਾਰ(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], ਆਕਾਰ(cvt[1])),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(wdepth), ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ਜੇ (wdepth == CV_32F)
        k.ਦਲੀਲਾਂ(srcarg, dstarg, (ਦਸ਼ਮਲਵ)alpha, (ਦਸ਼ਮਲਵ)beta);
    ਨਹੀਂਤਾਂ ਜੇ (wdepth == CV_64F)
        k.ਦਲੀਲਾਂ(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)src.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ convertScaleAbs(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ਲੜੀਵਾਰ(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    ਪੂਰਨ cn = src.channels();
    ਦੁੱਗਣਾ scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    ਦੁਆਧਾਰੀ_Func func = getCvtScaleAbsFunc(src.ਡੂੰਘਾਈ());
    CV_Assert( func != 0 );

    ਜੇ( src.dims <= 2 )
    {
        ਮਾਪ sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.ਕਦਮ, 0, 0, dst.ptr(), dst.ਕਦਮ, sz, scale );
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ਮਾਪ sz((ਪੂਰਨ)it.size*cn, 1);

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
