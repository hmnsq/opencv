// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "nan_mask.simd.hpp"
#ضمن "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

#ifdef HAVE_OPENCL

ساكن منطقي ocl_patchNaNs( شغرو_Output_Array _a, مزدوج value )
{
    صحيح ftype = _a.depth();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا (!doubleSupport && ftype == CV_64F)
    {
        أرجع خطأ;
    }

    صحيح rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat a = _a.getUMat();
    صحيح cn = a.channels();

    // to pass float or double to args
    إذا (ftype == CV_32F)
    {
        k.حجوجي(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (عائم)value);
    }
    وإلا // CV_64F
    {
        k.حجوجي(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, عدم, خطأ);
}

#endif

ساكن PatchNanFunc getPatchNanFunc(منطقي isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

فراغ patchNaNs( شغرو_Output_Array _a, مزدوج _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.depth() == CV_32F || _a.depth() == CV_64F);

    CV_OCL_RUN(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ثابت Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.depth() == CV_64F);

    لكل (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

ساكن منطقي ocl_finiteMask(ثابت UMat img, UMat mask)
{
    صحيح channels = img.channels();
    صحيح depth = img.depth();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا (!doubleSupport && depth == CV_64F)
    {
        أرجع خطأ;
    }

    صحيح rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    k.حجوجي(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, عدم, خطأ);
}

#endif

ساكن FiniteMaskFunc getFiniteMaskFunc(منطقي isDouble, صحيح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

فراغ finiteMask(شغرو_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    صحيح channels = _src.channels();
    صحيح depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<صحيح> vsz(_src.dims());
    _src.sizend(vsz.data());
    _mask.create(_src.dims(), vsz.data(), CV_8UC1);

    CV_OCL_RUN(_src.isUMat() && _mask.isUMat() && _src.dims() <= 2,
               ocl_finiteMask(_src.getUMat(), _mask.getUMat()));

    Mat src = _src.getMat();
    Mat mask = _mask.getMat();

    ثابت Mat *arrays[]={&src, &mask, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t total = planes[0].total();
    size_t i, nplanes = it.nplanes;

    FiniteMaskFunc func = getFiniteMaskFunc((depth == CV_64F), channels);

    لكل( i = 0; i < nplanes; i++, ++it )
    {
        ثابت uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv