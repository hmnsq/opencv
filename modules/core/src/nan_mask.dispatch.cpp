// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "nan_mask.simd.hpp"
#شامل "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv {

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_patchNaNs( داخلہ_Output_Array _a, دوہرا value )
{
    صحیح ftype = _a.گہرائی();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقی doubleSupport = d.doubleFPConfig() > 0;
    اگر (!doubleSupport && ftype == CV_64F)
    {
        واپس جھوٹ;
    }

    صحیح صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, صفیں_فی_WI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    UMat a = _a.getUMat();
    صحیح cn = a.channels();

    // to pass float or double to args
    اگر (ftype == CV_32F)
    {
        k.دلائل(ocl::KernelArg::Read_Only_No_سائز(a), ocl::KernelArg::WriteOnly(a, cn), (اعشاری)value);
    }
    ورنہ // CV_64F
    {
        k.دلائل(ocl::KernelArg::Read_Only_No_سائز(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.ستون_تعداد * cn, ((size_t)a.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

ساکن PatchNanFunc getPatchNanFunc(منطقی isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

خالی patchNaNs( داخلہ_Output_Array _a, دوہرا _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.گہرائی() == CV_32F || _a.گہرائی() == CV_64F);

    CV_OCL_تسلسل(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    ثابت Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.گہرائی() == CV_64F);

    برائے (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_finite_نقاب(ثابت UMat img, UMat نقاب)
{
    صحیح channels = img.channels();
    صحیح گہرائی = img.گہرائی();

    ثابت ocl::Device d = ocl::Device::getDefault();
    منطقی doubleSupport = d.doubleFPConfig() > 0;
    اگر (!doubleSupport && گہرائی == CV_64F)
    {
        واپس جھوٹ;
    }

    صحیح صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         گہرائی == CV_32F ? "float" : "double", channels, صفیں_فی_WI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    k.دلائل(ocl::KernelArg::Read_Only_No_سائز(img), ocl::KernelArg::WriteOnly(نقاب));

    size_t globalsize[2] = { (size_t)img.ستون_تعداد, ((size_t)img.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

ساکن Finite_نقاب_Func get_Finite_نقاب_Func(منطقی isDouble, صحیح cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Finite_نقاب_Func, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

خالی finite_نقاب(داخلہ_Array _src, OutputArray نقاب_2)
{
    CV_INSTRUMENT_REGION();

    صحیح channels = _src.channels();
    صحیح گہرائی = _src.گہرائی();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( گہرائی == CV_32F || گہرائی == CV_64F );
    std::vector<صحیح> vsz(_src.dims());
    _src.sizend(vsz.ڈیٹا());
    نقاب_2.create(_src.dims(), vsz.ڈیٹا(), CV_8UC1);

    CV_OCL_تسلسل(_src.isUMat() && نقاب_2.isUMat() && _src.dims() <= 2,
               ocl_finite_نقاب(_src.getUMat(), نقاب_2.getUMat()));

    Mat src = _src.getMat();
    Mat نقاب = نقاب_2.getMat();

    ثابت Mat *arrays[]={&src, &نقاب, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t کل = planes[0].کل();
    size_t i, nplanes = it.nplanes;

    Finite_نقاب_Func func = get_Finite_نقاب_Func((گہرائی == CV_64F), channels);

    برائے( i = 0; i < nplanes; i++, ++it )
    {
        ثابت uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, کل);
    }
}
} //namespace cv