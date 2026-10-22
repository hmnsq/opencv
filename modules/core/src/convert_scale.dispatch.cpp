// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "convert_scale.simd.hpp"
#अन्तर्भाव "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv
{

स्थिर द्विमान_Func getCvtScaleAbsFunc(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
}

द्विमान_Func getConvertScaleFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#यदि_परिभाषितम् HAVE_OPENCL

स्थिर तार्किक ocl_convertScaleAbs( निवेशः_Array _src, OutputArray _dst, द्विगुण alpha, द्विगुण beta )
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();

    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type);
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    यदि (!doubleSupport && गभीरता == CV_64F)
        प्रत्यागम असत्य;

    _dst.create_Same_आकार(_src, CV_8UC(cn));
    पूर्णाङ्क kercn = 1;
    यदि (d.isIntel())
    {
        स्थिर ध्रुव पूर्णाङ्क सदिशम्_Widths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::check_Optimal_सदिशम्_विस्तारः( सदिशम्_Widths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_सदिशम्_MAX);
    }
    अन्यथा
        kercn = ocl::predict_Optimal_सदिशम्_विस्तारः_Max(_src, _dst);

    पूर्णाङ्क पङ्क्तिकाः_प्रत्येकम्_WI = d.isIntel() ? 4 : 1;
    वर्ण cvt[2][50];
    पूर्णाङ्क wdepth = std::max(गभीरता, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::type_गन्तव्यम्_Str(CV_8UC(kercn)), CV_8U,
                         ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, kercn)),
                         ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(गभीरता, wdepth, kercn, cvt[0], परिमाण(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], परिमाण(cvt[1])),
                         ocl::type_गन्तव्यम्_Str(wdepth), पङ्क्तिकाः_प्रत्येकम्_WI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    यदि (wdepth == CV_32F)
        k.तर्काः(srcarg, dstarg, (भिन्न)alpha, (भिन्न)beta);
    अन्यथा यदि (wdepth == CV_64F)
        k.तर्काः(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.स्तम्भसङ्ख्या * cn / kercn, ((size_t)src.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

#यदि_अन्तः

शून्य convertScaleAbs(निवेशः_Array _src, OutputArray _dst, द्विगुण alpha, द्विगुण beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_सततम्(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    पूर्णाङ्क cn = src.channels();
    द्विगुण scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    द्विमान_Func func = getCvtScaleAbsFunc(src.गभीरता());
    CV_Assert( func != 0 );

    यदि( src.dims <= 2 )
    {
        आकार sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.चरणः, 0, 0, dst.ptr(), dst.चरणः, sz, scale );
    }
    अन्यथा
    {
        ध्रुव Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        आकार sz((पूर्णाङ्क)it.size*cn, 1);

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
