// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "nan_mask.simd.hpp"
#অন্তর্ভাব "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_patchNaNs( প্রবেশ_Output_Array _a, দ্বিগুণ value )
{
    পূর্ণসংখ্যা ftype = _a.depth();

    ধ্রুবক ocl::Device d = ocl::Device::getDefault();
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;
    যদি (!doubleSupport && ftype == CV_64F)
    {
        ফেরত মিথ্যা;
    }

    পূর্ণসংখ্যা rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat a = _a.getUMat();
    পূর্ণসংখ্যা cn = a.channels();

    // to pass float or double to args
    যদি (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::Read_Only_No_আকার(a), ocl::KernelArg::WriteOnly(a, cn), (দশমিক)value);
    }
    অন্যথা // CV_64F
    {
        k.args(ocl::KernelArg::Read_Only_No_আকার(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

স্থির PatchNanFunc getPatchNanFunc(যৌক্তিক isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য patchNaNs( প্রবেশ_Output_Array _a, দ্বিগুণ _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ধ্রুবক Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    জন্য (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_finiteMask(ধ্রুবক UMat img, UMat mask)
{
    পূর্ণসংখ্যা channels = img.channels();
    পূর্ণসংখ্যা depth = img.depth();

    ধ্রুবক ocl::Device d = ocl::Device::getDefault();
    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;
    যদি (!doubleSupport && depth == CV_64F)
    {
        ফেরত মিথ্যা;
    }

    পূর্ণসংখ্যা rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    যদি (k.empty())
        ফেরত মিথ্যা;

    k.args(ocl::KernelArg::Read_Only_No_আকার(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

স্থির FiniteMaskFunc getFiniteMaskFunc(যৌক্তিক isDouble, পূর্ণসংখ্যা cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য finiteMask(প্রবেশ_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা channels = _src.channels();
    পূর্ণসংখ্যা depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<পূর্ণসংখ্যা> vsz(_src.dims());
    _src.sizend(vsz.তথ্য());
    _mask.create(_src.dims(), vsz.তথ্য(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    ধ্রুবক Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t যোগ = planes[0].যোগ();
    size_t i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    জন্য( i = 0; i < nplanes; i++, ++it )
    {
        ধ্রুবক uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, যোগ);
    }
}
} //namespace cv