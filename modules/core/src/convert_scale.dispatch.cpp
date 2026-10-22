// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "convert_scale.simd.hpp"
#ضمن "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv
{

ساكن دودویی_Func getCvtScaleAbsFunc(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

دودویی_Func getConvertScaleFunc(صحيح sdepth, صحيح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_convertScaleAbs( درون_داد_Array _src, OutputArray _dst, مزدوج alpha, مزدوج beta )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    صحيح type = _src.type(), عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type);
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا (!doubleSupport && عمق == CV_64F)
        أرجع خطأ;

    _dst.create_Same_اندازه(_src, CV_8UC(cn));
    صحيح kercn = 1;
    إذا (d.isIntel())
    {
        ساكن ثابت صحيح وکتور_Widths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::check_Optimal_وکتور_عرض( وکتور_Widths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_وکتور_MAX);
    }
    وإلا
        kercn = ocl::predict_Optimal_وکتور_عرض_Max(_src, _dst);

    صحيح ردیفها_در_هر_WI = d.isIntel() ? 4 : 1;
    حرف cvt[2][50];
    صحيح wdepth = std::max(عمق, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::type_مقصد_Str(CV_8UC(kercn)), CV_8U,
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(عمق, kercn)),
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(عمق, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], حجم(cvt[1])),
                         ocl::type_مقصد_Str(wdepth), ردیفها_در_هر_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_اندازه(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    إذا (wdepth == CV_32F)
        k.آرگومانها(srcarg, dstarg, (عائم)alpha, (عائم)beta);
    وإلا إذا (wdepth == CV_64F)
        k.آرگومانها(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.تعداد_ستون * cn / kercn, ((size_t)src.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

فراغ convertScaleAbs(درون_داد_Array _src, OutputArray _dst, مزدوج alpha, مزدوج beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پیاپی(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    صحيح cn = src.channels();
    مزدوج scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    دودویی_Func func = getCvtScaleAbsFunc(src.عمق());
    CV_Assert( func != 0 );

    إذا( src.dims <= 2 )
    {
        اندازه sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.گام, 0, 0, dst.ptr(), dst.گام, sz, scale );
    }
    وإلا
    {
        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        اندازه sz((صحيح)it.size*cn, 1);

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
