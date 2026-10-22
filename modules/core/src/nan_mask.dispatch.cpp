// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "nan_mask.simd.hpp"
#ضمن "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_patchNaNs( درون_داد_Output_Array _a, مزدوج value )
{
    صحيح ftype = _a.عمق();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا (!doubleSupport && ftype == CV_64F)
    {
        أرجع خطأ;
    }

    صحيح ردیفها_در_هر_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, ردیفها_در_هر_WI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat a = _a.getUMat();
    صحيح cn = a.channels();

    // to pass float or double to args
    إذا (ftype == CV_32F)
    {
        k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(a), ocl::KernelArg::WriteOnly(a, cn), (عائم)value);
    }
    وإلا // CV_64F
    {
        k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.تعداد_ستون * cn, ((size_t)a.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

ساكن PatchNanFunc getPatchNanFunc(منطقي isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

فراغ patchNaNs( درون_داد_Output_Array _a, مزدوج _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.عمق() == CV_32F || _a.عمق() == CV_64F);

    CV_OCL_پیاپی(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ثابت Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.عمق() == CV_64F);

    لكل (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_finite_ماسک(ثابت UMat img, UMat ماسک)
{
    صحيح channels = img.channels();
    صحيح عمق = img.عمق();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقي doubleSupport = d.doubleFPConfig() > 0;
    إذا (!doubleSupport && عمق == CV_64F)
    {
        أرجع خطأ;
    }

    صحيح ردیفها_در_هر_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         عمق == CV_32F ? "float" : "double", channels, ردیفها_در_هر_WI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(img), ocl::KernelArg::WriteOnly(ماسک));

    size_t globalsize[2] = { (size_t)img.تعداد_ستون, ((size_t)img.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

ساكن Finite_ماسک_Func get_Finite_ماسک_Func(منطقي isDouble, صحيح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Finite_ماسک_Func, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

فراغ finite_ماسک(درون_داد_Array _src, OutputArray ماسک_2)
{
    CV_INSTRUMENT_REGION();

    صحيح channels = _src.channels();
    صحيح عمق = _src.عمق();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( عمق == CV_32F || عمق == CV_64F );
    std::vector<صحيح> vsz(_src.dims());
    _src.sizend(vsz.داده());
    ماسک_2.create(_src.dims(), vsz.داده(), CV_8UC1);

    CV_OCL_پیاپی(_src.isUMat() && ماسک_2.isUMat() && _src.dims() <= 2,
               ocl_finite_ماسک(_src.getUMat(), ماسک_2.getUMat()));

    Mat src = _src.getMat();
    Mat ماسک = ماسک_2.getMat();

    ثابت Mat *arrays[]={&src, &ماسک, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t مجموع = planes[0].مجموع();
    size_t i, nplanes = it.nplanes;

    Finite_ماسک_Func func = get_Finite_ماسک_Func((عمق == CV_64F), channels);

    لكل( i = 0; i < nplanes; i++, ++it )
    {
        ثابت uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, مجموع);
    }
}
} //namespace cv