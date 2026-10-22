// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

#అన్తర్భావ "convert_scale.simd.hpp"
#అన్తర్భావ "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv
{

స్థిర ద్విమాన_Func getCvtScaleAbsFunc(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

ద్విమాన_Func getConvertScaleFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_convertScaleAbs( ఆదానం_Array _src, OutputArray _dst, ద్విగుణ alpha, ద్విగుణ beta )
{
    స్థిరరాశి ocl::Device & d = ocl::Device::getDefault();

    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type);
    తార్కిక doubleSupport = d.doubleFPConfig() > 0;
    ఒకవేళ (!doubleSupport && లోతు == CV_64F)
        తిరిగి అబద్ధం;

    _dst.create_Same_కొలత(_src, CV_8UC(cn));
    పూర్ణాంకం kercn = 1;
    ఒకవేళ (d.isIntel())
    {
        స్థిర స్థిరరాశి పూర్ణాంకం సదిశ_Widths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::check_Optimal_సదిశ_వెడల్పు( సదిశ_Widths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_సదిశ_MAX);
    }
    లేకపోతే
        kercn = ocl::predict_Optimal_సదిశ_వెడల్పు_Max(_src, _dst);

    పూర్ణాంకం అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;
    అక్షరం cvt[2][50];
    పూర్ణాంకం wdepth = std::max(లోతు, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::type_గమ్యం_Str(CV_8UC(kercn)), CV_8U,
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)),
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(లోతు, wdepth, kercn, cvt[0], పరిమాణం(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], పరిమాణం(cvt[1])),
                         ocl::type_గమ్యం_Str(wdepth), అడ్డువరుసలు_చొప్పున_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_కొలత(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ఒకవేళ (wdepth == CV_32F)
        k.వాదనలు(srcarg, dstarg, (దశాంశం)alpha, (దశాంశం)beta);
    లేకపోతే ఒకవేళ (wdepth == CV_64F)
        k.వాదనలు(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.నిలువుల_సంఖ్య * cn / kercn, ((size_t)src.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

శూన్య convertScaleAbs(ఆదానం_Array _src, OutputArray _dst, ద్విగుణ alpha, ద్విగుణ beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_నిరంతరం(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    పూర్ణాంకం cn = src.channels();
    ద్విగుణ scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    ద్విమాన_Func func = getCvtScaleAbsFunc(src.లోతు());
    CV_Assert( func != 0 );

    ఒకవేళ( src.dims <= 2 )
    {
        కొలత sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.అడుగు, 0, 0, dst.ptr(), dst.అడుగు, sz, scale );
    }
    లేకపోతే
    {
        స్థిరరాశి Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        కొలత sz((పూర్ణాంకం)it.size*cn, 1);

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
