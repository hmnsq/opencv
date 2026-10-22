// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "nan_mask.simd.hpp"
#شامل "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

فضاینام cv {

#ifdef HAVE_OPENCL

ایستا منطقی ocl_patchNaNs( درون_داد_Output_Array _a, دوگانه value )
{
    صحیح ftype = _a.depth();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقی doubleSupport = d.doubleFPConfig() > 0;
    اگر (!doubleSupport && ftype == CV_64F)
    {
        بازگشت نادرست;
    }

    صحیح rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, rowsPerWI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    اگر (k.empty())
        بازگشت نادرست;

    UMat a = _a.getUMat();
    صحیح cn = a.channels();

    // to pass float or double to args
    اگر (ftype == CV_32F)
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), (اعشاری)value);
    }
    وگرنه // CV_64F
    {
        k.args(ocl::KernelArg::ReadOnlyNoSize(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.cols * cn, ((size_t)a.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

ایستا PatchNanFunc getPatchNanFunc(منطقی isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

تهی patchNaNs( درون_داد_Output_Array _a, دوگانه _val )
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

    برای (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#ifdef HAVE_OPENCL

ایستا منطقی ocl_finiteMask(ثابت UMat img, UMat mask)
{
    صحیح channels = img.channels();
    صحیح depth = img.depth();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقی doubleSupport = d.doubleFPConfig() > 0;
    اگر (!doubleSupport && depth == CV_64F)
    {
        بازگشت نادرست;
    }

    صحیح rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         depth == CV_32F ? "float" : "double", channels, rowsPerWI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    اگر (k.empty())
        بازگشت نادرست;

    k.args(ocl::KernelArg::ReadOnlyNoSize(img), ocl::KernelArg::WriteOnly(mask));

    size_t globalsize[2] = { (size_t)img.cols, ((size_t)img.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

ایستا FiniteMaskFunc getFiniteMaskFunc(منطقی isDouble, صحیح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getFiniteMaskFunc, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

تهی finiteMask(درون_داد_Array _src, OutputArray _mask)
{
    CV_INSTRUMENT_REGION();

    صحیح channels = _src.channels();
    صحیح depth = _src.depth();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( depth == CV_32F || depth == CV_64F );
    std::vector<صحیح> vsz(_src.dims());
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

    برای( i = 0; i < nplanes; i++, ++it )
    {
        ثابت uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, total);
    }
}
} //namespace cv