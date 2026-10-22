// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "nan_mask.simd.hpp"
#अन्तर्भाव "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_patchNaNs( आगत_Output_Array _a, द्विगुण value )
{
    पूर्णाङ्क ftype = _a.depth();

    अचर ocl::Device d = ocl::Device::getDefault();
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    यदि (!doubleSupport && ftype == CV_64F)
    {
        फर्काऊ असत्य;
    }

    पूर्णाङ्क rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    यदि (k.empty())
        फर्काऊ असत्य;

    UMat a = _a.getUMat();
    पूर्णाङ्क cn = a.channels();

    // to pass float or double to args
    यदि (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (भिन्न)value);
    }
    अन्यथा // CV_64F
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    माप_प्रकार globalsize[2] = { (माप_प्रकार)a.cols * cn, ((माप_प्रकार)a.rows + rowsPerWI - 1) / rowsPerWI };
    फर्काऊ k.run(2, globalsize, रिक्त, असत्य);
}

#endif

स्थिर PatchNanFunc getPatchNanFunc(तार्किक isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

शून्य patchNaNs( आगत_Output_Array _a, द्विगुण _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    अचर Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    माप_प्रकार len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    लागि (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_finiteMask(अचर UMat img, UMat mask)
{
    पूर्णाङ्क channels = img.channels();
    पूर्णाङ्क depth = img.depth();

    अचर ocl::Device d = ocl::Device::getDefault();
    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    यदि (!doubleSupport && depth == CV_64F)
    {
        फर्काऊ असत्य;
    }

    पूर्णाङ्क rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    यदि (k.empty())
        फर्काऊ असत्य;

    k.args(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    माप_प्रकार globalsize[2] = { (माप_प्रकार)img.cols, ((माप_प्रकार)img.rows + rowsPerWI - 1) / rowsPerWI };
    फर्काऊ k.run(2, globalsize, रिक्त, असत्य);
}

#endif

स्थिर FiniteMaskFunc getFiniteMaskFunc(तार्किक isDouble, पूर्णाङ्क cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

शून्य finiteMask(आगत_Array _src, OutputArray _mask)
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

    अचर Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    माप_प्रकार total = planes[0].total();
    माप_प्रकार i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    लागि( i = 0; i < nplanes; i++, ++it )
    {
        अचर uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv