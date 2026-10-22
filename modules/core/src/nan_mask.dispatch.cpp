// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "nan_mask.simd.hpp"
#অন্তর্ভাব "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_patchNaNs( প্রবেশ_Output_Array _a, দ্বিগুণ value )
{
    পূর্ণাঙ্ক ftype = _a.depth();

    ধ্রুব ocl::Device d = ocl::Device::getDefault();
    তার্কিক doubleSupport = d.doubleFPConfig() > 0;
    যদি (!doubleSupport && ftype == CV_64F)
    {
        প্রত্যাগম অসত্য;
    }

    পূর্ণাঙ্ক rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    UMat a = _a.getUMat();
    পূর্ণাঙ্ক cn = a.channels();

    // to pass float or double to args
    যদি (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (ভিন্ন)value);
    }
    অন্যথা // CV_64F
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    প্রত্যাগম k.run(2, globalsize, রিক্ত, অসত্য);
}

#endif

স্থির PatchNanFunc getPatchNanFunc(তার্কিক isDouble)
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
    ধ্রুব Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    প্রতি (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_finiteMask(ধ্রুব UMat img, UMat mask)
{
    পূর্ণাঙ্ক channels = img.channels();
    পূর্ণাঙ্ক depth = img.depth();

    ধ্রুব ocl::Device d = ocl::Device::getDefault();
    তার্কিক doubleSupport = d.doubleFPConfig() > 0;
    যদি (!doubleSupport && depth == CV_64F)
    {
        প্রত্যাগম অসত্য;
    }

    পূর্ণাঙ্ক rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    k.args(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    প্রত্যাগম k.run(2, globalsize, রিক্ত, অসত্য);
}

#endif

স্থির FiniteMaskFunc getFiniteMaskFunc(তার্কিক isDouble, পূর্ণাঙ্ক cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য finiteMask(প্রবেশ_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক channels = _src.channels();
    পূর্ণাঙ্ক depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<পূর্ণাঙ্ক> vsz(_src.dims());
    _src.sizend(vsz.data());
    _mask.create(_src.dims(), vsz.data(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    ধ্রুব Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t total = planes[0].total();
    size_t i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    প্রতি( i = 0; i < nplanes; i++, ++it )
    {
        ধ্রুব uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv