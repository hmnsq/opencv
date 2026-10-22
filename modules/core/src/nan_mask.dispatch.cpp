// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

#అన్తర్భావ "nan_mask.simd.hpp"
#అన్తర్భావ "nan_mask.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv {

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_patchNaNs( ఆదానం_Output_Array _a, ద్విగుణ value )
{
    పూర్ణాంకం ftype = _a.లోతు();

    స్థిరరాశి ocl::Device d = ocl::Device::getDefault();
    తార్కిక doubleSupport = d.doubleFPConfig() > 0;
    ఒకవేళ (!doubleSupport && ftype == CV_64F)
    {
        తిరిగి అబద్ధం;
    }

    పూర్ణాంకం అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                     format("-D UNARY_OP -D OP_PATCH_NANS -D dstT=%s -D DEPTH_dst=%d -D rowsPerWI=%d %s",
                            ftype == CV_64F ? "double" : "float", ftype, అడ్డువరుసలు_చొప్పున_WI,
                            doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat a = _a.getUMat();
    పూర్ణాంకం cn = a.channels();

    // to pass float or double to args
    ఒకవేళ (ftype == CV_32F)
    {
        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(a), ocl::KernelArg::WriteOnly(a, cn), (దశాంశం)value);
    }
    లేకపోతే // CV_64F
    {
        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(a), ocl::KernelArg::WriteOnly(a, cn), value);
    }

    size_t globalsize[2] = { (size_t)a.నిలువుల_సంఖ్య * cn, ((size_t)a.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

స్థిర PatchNanFunc getPatchNanFunc(తార్కిక isDouble)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPatchNanFunc, (isDouble), CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య patchNaNs( ఆదానం_Output_Array _a, ద్విగుణ _val )
{
    CV_INSTRUMENT_REGION();
    CV_Assert( _a.లోతు() == CV_32F || _a.లోతు() == CV_64F);

    CV_OCL_నిరంతరం(_a.isUMat() && _a.dims() <= 2,
               ocl_patchNaNs(_a, _val))

    Mat a = _a.getMat();
    స్థిరరాశి Mat* arrays[] = {&a, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t len = it.size*a.channels();

    PatchNanFunc func = getPatchNanFunc(_a.లోతు() == CV_64F);

    కొరకు (size_t i = 0; i < it.nplanes; i++, ++it)
    {
        func(ptrs[0], len, _val);
    }
}


#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_finite_ముసుగు(స్థిరరాశి UMat img, UMat ముసుగు)
{
    పూర్ణాంకం channels = img.channels();
    పూర్ణాంకం లోతు = img.లోతు();

    స్థిరరాశి ocl::Device d = ocl::Device::getDefault();
    తార్కిక doubleSupport = d.doubleFPConfig() > 0;
    ఒకవేళ (!doubleSupport && లోతు == CV_64F)
    {
        తిరిగి అబద్ధం;
    }

    పూర్ణాంకం అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ocl::Kernel k("finiteMask", ocl::core::finitemask_oclsrc,
                  format("-D srcT=%s -D cn=%d -D rowsPerWI=%d %s",
                         లోతు == CV_32F ? "float" : "double", channels, అడ్డువరుసలు_చొప్పున_WI,
                         doubleSupport ? "-D DOUBLE_SUPPORT" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(img), ocl::KernelArg::WriteOnly(ముసుగు));

    size_t globalsize[2] = { (size_t)img.నిలువుల_సంఖ్య, ((size_t)img.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

స్థిర Finite_ముసుగు_Func get_Finite_ముసుగు_Func(తార్కిక isDouble, పూర్ణాంకం cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Finite_ముసుగు_Func, (isDouble, cn), CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య finite_ముసుగు(ఆదానం_Array _src, OutputArray ముసుగు_2)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం channels = _src.channels();
    పూర్ణాంకం లోతు = _src.లోతు();
    CV_Assert( channels > 0 && channels <= 4);
    CV_Assert( లోతు == CV_32F || లోతు == CV_64F );
    std::vector<పూర్ణాంకం> vsz(_src.dims());
    _src.sizend(vsz.దత్తాంశం());
    ముసుగు_2.create(_src.dims(), vsz.దత్తాంశం(), CV_8UC1);

    CV_OCL_నిరంతరం(_src.isUMat() && ముసుగు_2.isUMat() && _src.dims() <= 2,
               ocl_finite_ముసుగు(_src.getUMat(), ముసుగు_2.getUMat()));

    Mat src = _src.getMat();
    Mat ముసుగు = ముసుగు_2.getMat();

    స్థిరరాశి Mat *arrays[]={&src, &ముసుగు, 0};
    Mat planes[2];
    NAryMatIterator it(arrays, planes);
    size_t మొత్తం = planes[0].మొత్తం();
    size_t i, nplanes = it.nplanes;

    Finite_ముసుగు_Func func = get_Finite_ముసుగు_Func((లోతు == CV_64F), channels);

    కొరకు( i = 0; i < nplanes; i++, ++it )
    {
        స్థిరరాశి uchar* sptr = planes[0].ptr();
        uchar* dptr = planes[1].ptr();

        func(sptr, dptr, మొత్తం);
    }
}
} //namespace cv