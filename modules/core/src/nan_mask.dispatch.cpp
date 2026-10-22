// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "nan_mask.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_patchNaNs( ਨਿਵੇਸ਼_Output_Array _a, ਦੁੱਗਣਾ value )
{
    ਪੂਰਨ ftype = _a.ਡੂੰਘਾਈ();

    ਅਚਲ ocl::Device d = ocl::Device::getDefault();
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;
    ਜੇ (!doubleSupport && ftype == CV_64F)
    {
        ਵਾਪਸ ਝੂਠ;
    }

    ਪੂਰਨ ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat a = _a.getUMat();
    ਪੂਰਨ cn = a.channels();

    // to pass float or double to args
    ਜੇ (ftype == CV_32F)
    {
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(a), ocl::KernelArg::WriteOnly(a, cn), (ਦਸ਼ਮਲਵ)value);
    }
    ਨਹੀਂਤਾਂ // CV_64F
    {
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.ਖਾਨੇ_ਗਿਣਤੀ * cn, ((size_t)a.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ PatchNanFunc getPatchNanFunc(ਤਾਰਕਿਕ isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ patchNaNs( ਨਿਵੇਸ਼_Output_Array _a, ਦੁੱਗਣਾ _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.ਡੂੰਘਾਈ() == CV_32F || _a.ਡੂੰਘਾਈ() == CV_64F);

    CV_OCL_ਲੜੀਵਾਰ(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ਅਚਲ Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.ਡੂੰਘਾਈ() == CV_64F);

    ਲਈ (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_finite_ਨਕਾਬ(ਅਚਲ UMat img, UMat ਨਕਾਬ)
{
    ਪੂਰਨ channels = img.channels();
    ਪੂਰਨ ਡੂੰਘਾਈ = img.ਡੂੰਘਾਈ();

    ਅਚਲ ocl::Device d = ocl::Device::getDefault();
    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;
    ਜੇ (!doubleSupport && ਡੂੰਘਾਈ == CV_64F)
    {
        ਵਾਪਸ ਝੂਠ;
    }

    ਪੂਰਨ ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         ਡੂੰਘਾਈ == CV_32F ? "float" : "double", channels, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(img), ocl::KernelArg::WriteOnly(ਨਕਾਬ));

    size_t globalsize[2] = { (size_t)img.ਖਾਨੇ_ਗਿਣਤੀ, ((size_t)img.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ Finite_ਨਕਾਬ_Func get_Finite_ਨਕਾਬ_Func(ਤਾਰਕਿਕ isDouble, ਪੂਰਨ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Finite_ਨਕਾਬ_Func, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ finite_ਨਕਾਬ(ਨਿਵੇਸ਼_Array _src, OutputArray ਨਕਾਬ_2)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ channels = _src.channels();
    ਪੂਰਨ ਡੂੰਘਾਈ = _src.ਡੂੰਘਾਈ();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F );
    std::vector<ਪੂਰਨ> vsz(_src.dims());
    _src.sizend(vsz.ਅੰਕੜੇ());
    ਨਕਾਬ_2.create(_src.dims(), vsz.ਅੰਕੜੇ(), CV_8UC1);

    CV_OCL_ਲੜੀਵਾਰ(_src.isUMat() && ਨਕਾਬ_2.isUMat() && _src.dims() <= 2,
               ocl_finite_ਨਕਾਬ(_src.getUMat(), ਨਕਾਬ_2.getUMat()));

    Mat src = _src.getMat();
    Mat ਨਕਾਬ = ਨਕਾਬ_2.getMat();

    ਅਚਲ Mat *arrays[]={&src, &ਨਕਾਬ, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t ਕੁੱਲ = planes[0].ਕੁੱਲ();
    size_t i, nplanes = it.nplanes;

    Finite_ਨਕਾਬ_Func func = get_Finite_ਨਕਾਬ_Func((ਡੂੰਘਾਈ == CV_64F), channels);

    ਲਈ( i = 0; i < nplanes; i++, ++it )
    {
        ਅਚਲ uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, ਕੁੱਲ);
    }
}
} //namespace cv