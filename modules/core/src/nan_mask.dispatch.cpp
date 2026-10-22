// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "nan_mask.simd.hpp"
#ಅನ್ತರ್ಭಾವ "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_patchNaNs( ಒಳಹರಿವು_Output_Array _a, ದ್ವಿಗುಣ value )
{
    ಪೂರ್ಣಾಂಕ ftype = _a.ಆಳ();

    ಸ್ಥಿರಾಂಕ ocl::Device d = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಆದರೆ (!doubleSupport && ftype == CV_64F)
    {
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat a = _a.getUMat();
    ಪೂರ್ಣಾಂಕ cn = a.channels();

    // to pass float or double to args
    ಆದರೆ (ftype == CV_32F)
    {
        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(a), ocl::KernelArg::WriteOnly(a, cn), (ದಶಮಾಂಶ)value);
    }
    ಇಲ್ಲದಿದ್ದರೆ // CV_64F
    {
        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn, ((size_t)a.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ PatchNanFunc getPatchNanFunc(ತಾರ್ಕಿಕ isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ patchNaNs( ಒಳಹರಿವು_Output_Array _a, ದ್ವಿಗುಣ _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.ಆಳ() == CV_32F || _a.ಆಳ() == CV_64F);

    CV_OCL_ಸತತ(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.ಆಳ() == CV_64F);

    ಗಾಗಿ (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_finite_ಮುಖವಾಡ(ಸ್ಥಿರಾಂಕ UMat img, UMat ಮುಖವಾಡ)
{
    ಪೂರ್ಣಾಂಕ channels = img.channels();
    ಪೂರ್ಣಾಂಕ ಆಳ = img.ಆಳ();

    ಸ್ಥಿರಾಂಕ ocl::Device d = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಆದರೆ (!doubleSupport && ಆಳ == CV_64F)
    {
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         ಆಳ == CV_32F ? "float" : "double", channels, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(img), ocl::KernelArg::WriteOnly(ಮುಖವಾಡ));

    size_t globalsize[2] = { (size_t)img.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ((size_t)img.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ Finite_ಮುಖವಾಡ_Func get_Finite_ಮುಖವಾಡ_Func(ತಾರ್ಕಿಕ isDouble, ಪೂರ್ಣಾಂಕ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Finite_ಮುಖವಾಡ_Func, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ finite_ಮುಖವಾಡ(ಒಳಹರಿವು_Array _src, OutputArray ಮುಖವಾಡ_2)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ channels = _src.channels();
    ಪೂರ್ಣಾಂಕ ಆಳ = _src.ಆಳ();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( ಆಳ == CV_32F || ಆಳ == CV_64F );
    std::vector<ಪೂರ್ಣಾಂಕ> vsz(_src.dims());
    _src.sizend(vsz.ದತ್ತಾಂಶ());
    ಮುಖವಾಡ_2.create(_src.dims(), vsz.ದತ್ತಾಂಶ(), CV_8UC1);

    CV_OCL_ಸತತ(_src.isUMat() && ಮುಖವಾಡ_2.isUMat() && _src.dims() <= 2,
               ocl_finite_ಮುಖವಾಡ(_src.getUMat(), ಮುಖವಾಡ_2.getUMat()));

    Mat src = _src.getMat();
    Mat ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();

    ಸ್ಥಿರಾಂಕ Mat *arrays[]={&src, &ಮುಖವಾಡ, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t ಒಟ್ಟು = planes[0].ಒಟ್ಟು();
    size_t i, nplanes = it.nplanes;

    Finite_ಮುಖವಾಡ_Func func = get_Finite_ಮುಖವಾಡ_Func((ಆಳ == CV_64F), channels);

    ಗಾಗಿ( i = 0; i < nplanes; i++, ++it )
    {
        ಸ್ಥಿರಾಂಕ uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, ಒಟ್ಟು);
    }
}
} //namespace cv