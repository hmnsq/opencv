// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"

#അന്തര്ഭാവ "nan_mask.simd.hpp"
#അന്തര്ഭാവ "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv {

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_patchNaNs( നിവേശം_Output_Array _a, ഇരട്ട value )
{
    പൂർണ്ണസംഖ്യ ftype = _a.depth();

    സ്ഥിരം ocl::Device d = ocl::Device::getDefault();
    തർക്കം doubleSupport = d.doubleFPConfig() > 0;
    എങ്കിൽ (!doubleSupport && ftype == CV_64F)
    {
        തിരികെ അസത്യം;
    }

    പൂർണ്ണസംഖ്യ rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat a = _a.getUMat();
    പൂർണ്ണസംഖ്യ cn = a.channels();

    // to pass float or double to args
    എങ്കിൽ (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (ദശാംശം)value);
    }
    അല്ലെങ്കിൽ // CV_64F
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

സ്ഥിതം PatchNanFunc getPatchNanFunc(തർക്കം isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം patchNaNs( നിവേശം_Output_Array _a, ഇരട്ട _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    സ്ഥിരം Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    വേണ്ടി (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_finiteMask(സ്ഥിരം UMat img, UMat mask)
{
    പൂർണ്ണസംഖ്യ channels = img.channels();
    പൂർണ്ണസംഖ്യ depth = img.depth();

    സ്ഥിരം ocl::Device d = ocl::Device::getDefault();
    തർക്കം doubleSupport = d.doubleFPConfig() > 0;
    എങ്കിൽ (!doubleSupport && depth == CV_64F)
    {
        തിരികെ അസത്യം;
    }

    പൂർണ്ണസംഖ്യ rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    k.args(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

സ്ഥിതം FiniteMaskFunc getFiniteMaskFunc(തർക്കം isDouble, പൂർണ്ണസംഖ്യ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം finiteMask(നിവേശം_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ channels = _src.channels();
    പൂർണ്ണസംഖ്യ depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<പൂർണ്ണസംഖ്യ> vsz(_src.dims());
    _src.sizend(vsz.data());
    _mask.create(_src.dims(), vsz.data(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    സ്ഥിരം Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t total = planes[0].total();
    size_t i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    വേണ്ടി( i = 0; i < nplanes; i++, ++it )
    {
        സ്ഥിരം uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv