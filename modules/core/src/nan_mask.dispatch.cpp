// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "nan_mask.simd.hpp"
#ಅನ್ತರ್ಭಾವ "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_patchNaNs( ಒಳಹರಿವು_Output_Array _a, ದ್ವಿಗುಣ value )
{
    ಪೂರ್ಣಾಙ್ಕ ftype = _a.depth();

    ಧ್ರುವ ocl::Device d = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಯದಿ (!doubleSupport && ftype == CV_64F)
    {
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪೂರ್ಣಾಙ್ಕ rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat a = _a.getUMat();
    ಪೂರ್ಣಾಙ್ಕ cn = a.channels();

    // to pass float or double to args
    ಯದಿ (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (ಭಿನ್ನ)value);
    }
    ಅನ್ಯಥಾ // CV_64F
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಸ್ಥಿರ PatchNanFunc getPatchNanFunc(ತಾರ್ಕಿಕ isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ patchNaNs( ಒಳಹರಿವು_Output_Array _a, ದ್ವಿಗುಣ _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ಧ್ರುವ Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    ಪ್ರತಿ (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_finiteMask(ಧ್ರುವ UMat img, UMat mask)
{
    ಪೂರ್ಣಾಙ್ಕ channels = img.channels();
    ಪೂರ್ಣಾಙ್ಕ depth = img.depth();

    ಧ್ರುವ ocl::Device d = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಯದಿ (!doubleSupport && depth == CV_64F)
    {
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪೂರ್ಣಾಙ್ಕ rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    k.args(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಸ್ಥಿರ FiniteMaskFunc getFiniteMaskFunc(ತಾರ್ಕಿಕ isDouble, ಪೂರ್ಣಾಙ್ಕ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ finiteMask(ಒಳಹರಿವು_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ channels = _src.channels();
    ಪೂರ್ಣಾಙ್ಕ depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<ಪೂರ್ಣಾಙ್ಕ> vsz(_src.dims());
    _src.sizend(vsz.data());
    _mask.create(_src.dims(), vsz.data(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    ಧ್ರುವ Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t total = planes[0].total();
    size_t i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    ಪ್ರತಿ( i = 0; i < nplanes; i++, ++it )
    {
        ಧ್ರುವ uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv