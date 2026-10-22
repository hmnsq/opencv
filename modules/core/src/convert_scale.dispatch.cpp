// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "convert_scale.simd.hpp"
#ضمن "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv
{

ساكن ثنائی_Func getCvtScaleAbsFunc(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

ثنائی_Func getConvertScaleFunc(صحيح sdepth, صحيح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#جے_تعریف_شدہ HAVE_OPENCL

ساكن منطقي ocl_convertScaleAbs( داخلہ_Array _src, OutputArray _dst, مزدوج alpha, مزدوج beta )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type);
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا (!doubleSupport && ڈونگھائی == CV_64F)
        أرجع خطأ;

    _dst.create_Same_ناپ(_src, CV_8UC(cn));
    صحيح kercn = 1;
    إذا (d.isIntel())
    {
        ساكن ثابت صحيح سمتیہ_Widths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::check_Optimal_سمتیہ_چوڑائی( سمتیہ_Widths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_سمتیہ_MAX);
    }
    وإلا
        kercn = ocl::predict_Optimal_سمتیہ_چوڑائی_Max(_src, _dst);

    صحيح صفاں_فی_WI = d.isIntel() ? 4 : 1;
    حرف cvt[2][50];
    صحيح wdepth = std::max(ڈونگھائی, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::type_منزل_Str(CV_8UC(kercn)), CV_8U,
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, kercn)),
                         ocl::type_منزل_Str(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(ڈونگھائی, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], حجم(cvt[1])),
                         ocl::type_منزل_Str(wdepth), صفاں_فی_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ناپ(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    إذا (wdepth == CV_32F)
        k.دلیلاں(srcarg, dstarg, (عائم)alpha, (عائم)beta);
    وإلا إذا (wdepth == CV_64F)
        k.دلیلاں(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.خانے_گنتی * cn / kercn, ((size_t)src.صفاں + صفاں_فی_WI - 1) / صفاں_فی_WI };
    أرجع k.لگاتار(2, globalsize, عدم, خطأ);
}

#جے_انت

فراغ convertScaleAbs(داخلہ_Array _src, OutputArray _dst, مزدوج alpha, مزدوج beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_لگاتار(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    صحيح cn = src.channels();
    مزدوج scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    ثنائی_Func func = getCvtScaleAbsFunc(src.ڈونگھائی());
    CV_Assert( func != 0 );

    إذا( src.dims <= 2 )
    {
        ناپ sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.قدم, 0, 0, dst.ptr(), dst.قدم, sz, scale );
    }
    وإلا
    {
        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ناپ sz((صحيح)it.size*cn, 1);

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
