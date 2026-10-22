// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "stat.hpp"

#అన్తర్భావ "has_non_zero.simd.hpp"
#అన్తర్భావ "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv {

స్థిర HasNonZeroFunc getHasNonZeroFunc(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక ocl_hasNonZero( ఆదానం_Array _src, తార్కిక & res )
{
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), kercn = ocl::predict_Optimal_సదిశ_వెడల్పు(_src);
    తార్కిక doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ఒకవేళ (లోతు == CV_64F && !doubleSupport)
        తిరిగి అబద్ధం;

    పూర్ణాంకం dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_సమూహం_కొలత();

    పూర్ణాంకం wgs2_aligned = 1;
    అయితే (wgs2_aligned < (పూర్ణాంకం)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)),
                         ocl::type_గమ్యం_Str(లోతు), (పూర్ణాంకం)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ఒకవేళ (k.నిరంతరం(1, &globalsize, &wgs, నిజం))
        తిరిగి res = (saturate_cast<పూర్ణాంకం>(cv::కూడిక(db.getMat(ACCESS_READ))[0])>0), నిజం;
    తిరిగి అబద్ధం;
}
#అయితే_ముగింపు

తార్కిక hasNonZero(ఆదానం_Array _src)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    తార్కిక res = అబద్ధం;

#నిర్వచితమైతే HAVE_OPENCL
    CV_OCL_నిరంతరం_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#అయితే_ముగింపు

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.లోతు());
    CV_Assert( func != 0 );

    ఒకవేళ (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        ఒకవేళ (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.మొత్తం());
        లేకపోతే
            కొరకు(పూర్ణాంకం అడ్డువరుస = 0, అడ్డువరుసలు_లెక్క = src.అడ్డువరుసలు ; !res && (అడ్డువరుస<అడ్డువరుసలు_లెక్క) ; ++అడ్డువరుస)
                res |= func(src.ptr<uchar>(అడ్డువరుస), src.నిలువుల_సంఖ్య);
    }
    లేకపోతే//if (src.dims != 2)
    {
        స్థిరరాశి Mat* arrays[] = {&src, శూన్యసూచిక};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        కొరకు(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            స్థిరరాశి Mat& plane = itNAry.planes[0];
            ఒకవేళ (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.మొత్తం());
            లేకపోతే
              కొరకు(పూర్ణాంకం అడ్డువరుస = 0, అడ్డువరుసలు_లెక్క = plane.అడ్డువరుసలు ; !res && (అడ్డువరుస<అడ్డువరుసలు_లెక్క) ; ++అడ్డువరుస)
                  res |= func(plane.ptr<uchar>(అడ్డువరుస), plane.నిలువుల_సంఖ్య);
        }
    }

    తిరిగి res;
}

} // namespace
