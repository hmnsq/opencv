// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "nan_mask.simd.hpp"
#ضمن "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

ساكن منطقي ocl_patchNaNs( داخلا_Output_Array _a, مزدوج value )
{
    صحيح ftype = _a.اونهائي();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا (!doubleSupport && ftype == CV_64F)
    {
        أرجع خطأ;
    }

    صحيح صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, صفون_في_WI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat a = _a.getUMat();
    صحيح cn = a.channels();

    // to pass float or double to args
    إذا (ftype == CV_32F)
    {
        k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(a), ocl::KernelArg::WriteOnly(a, cn), (عائم)value);
    }
    وإلا // CV_64F
    {
        k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.کانن_ڳڻپ * cn, ((size_t)a.صفون + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

#جيڪڏهن_پورو

ساكن PatchNanFunc getPatchNanFunc(منطقي isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

فراغ patchNaNs( داخلا_Output_Array _a, مزدوج _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.اونهائي() == CV_32F || _a.اونهائي() == CV_64F);

    CV_OCL_لاڳيتو(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ثابت Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.اونهائي() == CV_64F);

    لكل (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

ساكن منطقي ocl_finite_نقاب(ثابت UMat img, UMat نقاب)
{
    صحيح channels = img.channels();
    صحيح اونهائي = img.اونهائي();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا (!doubleSupport && اونهائي == CV_64F)
    {
        أرجع خطأ;
    }

    صحيح صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         اونهائي == CV_32F ? "float" : "double", channels, صفون_في_WI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(img), ocl::KernelArg::WriteOnly(نقاب));

    size_t globalsize[2] = { (size_t)img.کانن_ڳڻپ, ((size_t)img.صفون + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

#جيڪڏهن_پورو

ساكن Finite_نقاب_Func get_Finite_نقاب_Func(منطقي isDouble, صحيح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Finite_نقاب_Func, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

فراغ finite_نقاب(داخلا_Array _src, OutputArray نقاب_2)
{
    CV_INSTRUMENT_REGION();

    صحيح channels = _src.channels();
    صحيح اونهائي = _src.اونهائي();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( اونهائي == CV_32F || اونهائي == CV_64F );
    std::vector<صحيح> vsz(_src.dims());
    _src.sizend(vsz.معلومات());
    نقاب_2.create(_src.dims(), vsz.معلومات(), CV_8UC1);

    CV_OCL_لاڳيتو(_src.isUMat() && نقاب_2.isUMat() && _src.dims() <= 2,
               ocl_finite_نقاب(_src.getUMat(), نقاب_2.getUMat()));

    Mat src = _src.getMat();
    Mat نقاب = نقاب_2.getMat();

    ثابت Mat *arrays[]={&src, &نقاب, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t ڪل = planes[0].ڪل();
    size_t i, nplanes = it.nplanes;

    Finite_نقاب_Func func = get_Finite_نقاب_Func((اونهائي == CV_64F), channels);

    لكل( i = 0; i < nplanes; i++, ++it )
    {
        ثابت uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, ڪل);
    }
}
} //namespace cv