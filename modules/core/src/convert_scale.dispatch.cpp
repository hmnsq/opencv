// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "convert_scale.simd.hpp"
#شامل "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv
{

ساکن ثنائی_Func getCvtScaleAbsFunc(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

ثنائی_Func getConvertScaleFunc(صحیح sdepth, صحیح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_convertScaleAbs( داخلہ_Array _src, OutputArray _dst, دوہرا alpha, دوہرا beta )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type);
    منطقی doubleSupport = d.doubleFPConfig() > 0;
    اگر (!doubleSupport && گہرائی == CV_64F)
        واپس جھوٹ;

    _dst.create_Same_سائز(_src, CV_8UC(cn));
    صحیح kercn = 1;
    اگر (d.isIntel())
    {
        ساکن ثابت صحیح سمتیہ_Widths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::check_Optimal_سمتیہ_چوڑائی( سمتیہ_Widths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_سمتیہ_MAX);
    }
    ورنہ
        kercn = ocl::predict_Optimal_سمتیہ_چوڑائی_Max(_src, _dst);

    صحیح صفیں_فی_WI = d.isIntel() ? 4 : 1;
    حرف cvt[2][50];
    صحیح wdepth = std::max(گہرائی, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::type_مقصد_Str(CV_8UC(kercn)), CV_8U,
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)),
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(گہرائی, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], حجم(cvt[1])),
                         ocl::type_مقصد_Str(wdepth), صفیں_فی_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    اگر (k.empty())
        واپس جھوٹ;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_سائز(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    اگر (wdepth == CV_32F)
        k.دلائل(srcarg, dstarg, (اعشاری)alpha, (اعشاری)beta);
    ورنہ اگر (wdepth == CV_64F)
        k.دلائل(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.ستون_تعداد * cn / kercn, ((size_t)src.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

خالی convertScaleAbs(داخلہ_Array _src, OutputArray _dst, دوہرا alpha, دوہرا beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_تسلسل(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    صحیح cn = src.channels();
    دوہرا scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    ثنائی_Func func = getCvtScaleAbsFunc(src.گہرائی());
    CV_Assert( func != 0 );

    اگر( src.dims <= 2 )
    {
        سائز sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.قدم, 0, 0, dst.ptr(), dst.قدم, sz, scale );
    }
    ورنہ
    {
        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        سائز sz((صحیح)it.size*cn, 1);

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
