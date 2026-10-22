// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "nan_mask.simd.hpp"
#अन्तर्भाव "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

#यदि_परिभाषितम् HAVE_OPENCL

स्थिर तार्किक ocl_patchNaNs( निवेशः_Output_Array _a, द्विगुण value )
{
    पूर्णाङ्क ftype = _a.गभीरता();

    ध्रुव ocl::Device d = ocl::Device::getDefault();
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    यदि (!doubleSupport && ftype == CV_64F)
    {
        प्रत्यागम असत्य;
    }

    पूर्णाङ्क पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, पङ्क्तिकाः_प्रत्येकम्_WI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat a = _a.getUMat();
    पूर्णाङ्क cn = a.channels();

    // to pass float or double to args
    यदि (ftype == CV_32F)
    {
        k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(a), ocl::KernelArg::WriteOnly(a, cn), (भिन्न)value);
    }
    अन्यथा // CV_64F
    {
        k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.स्तम्भसङ्ख्या * cn, ((size_t)a.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

#यदि_अन्तः

स्थिर PatchNanFunc getPatchNanFunc(तार्किक isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

शून्य patchNaNs( निवेशः_Output_Array _a, द्विगुण _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.गभीरता() == CV_32F || _a.गभीरता() == CV_64F);

    CV_OCL_सततम्(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ध्रुव Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.गभीरता() == CV_64F);

    प्रति (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#यदि_परिभाषितम् HAVE_OPENCL

स्थिर तार्किक ocl_finite_आवरणम्(ध्रुव UMat img, UMat आवरणम्)
{
    पूर्णाङ्क channels = img.channels();
    पूर्णाङ्क गभीरता = img.गभीरता();

    ध्रुव ocl::Device d = ocl::Device::getDefault();
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    यदि (!doubleSupport && गभीरता == CV_64F)
    {
        प्रत्यागम असत्य;
    }

    पूर्णाङ्क पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         गभीरता == CV_32F ? "float" : "double", channels, पङ्क्तिकाः_प्रत्येकम्_WI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    यदि (k.empty())
        प्रत्यागम असत्य;

    k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(img), ocl::KernelArg::WriteOnly(आवरणम्));

    size_t globalsize[2] = { (size_t)img.स्तम्भसङ्ख्या, ((size_t)img.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

#यदि_अन्तः

स्थिर Finite_आवरणम्_Func get_Finite_आवरणम्_Func(तार्किक isDouble, पूर्णाङ्क cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Finite_आवरणम्_Func, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

शून्य finite_आवरणम्(निवेशः_Array _src, OutputArray आवरणम्_2)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क channels = _src.channels();
    पूर्णाङ्क गभीरता = _src.गभीरता();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( गभीरता == CV_32F || गभीरता == CV_64F );
    std::vector<पूर्णाङ्क> vsz(_src.dims());
    _src.sizend(vsz.दत्त());
    आवरणम्_2.create(_src.dims(), vsz.दत्त(), CV_8UC1);

    CV_OCL_सततम्(_src.isUMat() && आवरणम्_2.isUMat() && _src.dims() <= 2,
               ocl_finite_आवरणम्(_src.getUMat(), आवरणम्_2.getUMat()));

    Mat src = _src.getMat();
    Mat आवरणम् = आवरणम्_2.getMat();

    ध्रुव Mat *arrays[]={&src, &आवरणम्, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t योग = planes[0].योग();
    size_t i, nplanes = it.nplanes;

    Finite_आवरणम्_Func func = get_Finite_आवरणम्_Func((गभीरता == CV_64F), channels);

    प्रति( i = 0; i < nplanes; i++, ++it )
    {
        ध्रुव uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, योग);
    }
}
} //namespace cv