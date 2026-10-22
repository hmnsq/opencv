// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "nan_mask.simd.hpp"
#अन्तर्भाव "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_patchNaNs( निवेशः_Output_Array _a, द्विगुण value )
{
    पूर्णाङ्क ftype = _a.depth();

    ध्रुव ocl::Device d = ocl::Device::getDefault();
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    सचे (!doubleSupport && ftype == CV_64F)
    {
        पटिनिवत्त मिच्छा;
    }

    पूर्णाङ्क rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    सचे (k.empty())
        पटिनिवत्त मिच्छा;

    UMat a = _a.getUMat();
    पूर्णाङ्क cn = a.channels();

    // to pass float or double to args
    सचे (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (भिन्न)value);
    }
    अञ्ञथा // CV_64F
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    पटिनिवत्त k.run(2, globalsize, रिक्त, मिच्छा);
}

#endif

स्थिर PatchNanFunc getPatchNanFunc(तार्किक isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

शून्य patchNaNs( निवेशः_Output_Array _a, द्विगुण _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ध्रुव Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    अत्थाय (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_finiteMask(ध्रुव UMat img, UMat mask)
{
    पूर्णाङ्क channels = img.channels();
    पूर्णाङ्क depth = img.depth();

    ध्रुव ocl::Device d = ocl::Device::getDefault();
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    सचे (!doubleSupport && depth == CV_64F)
    {
        पटिनिवत्त मिच्छा;
    }

    पूर्णाङ्क rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    सचे (k.empty())
        पटिनिवत्त मिच्छा;

    k.args(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    पटिनिवत्त k.run(2, globalsize, रिक्त, मिच्छा);
}

#endif

स्थिर FiniteMaskFunc getFiniteMaskFunc(तार्किक isDouble, पूर्णाङ्क cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

शून्य finiteMask(निवेशः_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क channels = _src.channels();
    पूर्णाङ्क depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<पूर्णाङ्क> vsz(_src.dims());
    _src.sizend(vsz.data());
    _mask.create(_src.dims(), vsz.data(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    ध्रुव Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t total = planes[0].total();
    size_t i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    अत्थाय( i = 0; i < nplanes; i++, ++it )
    {
        ध्रुव uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv