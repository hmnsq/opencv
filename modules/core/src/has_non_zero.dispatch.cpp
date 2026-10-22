// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"

#شامل "has_non_zero.simd.hpp"
#شامل "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv {

ساکن HasNonZeroFunc getHasNonZeroFunc(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شدہ HAVE_OPENCL
ساکن منطقی ocl_hasNonZero( داخلہ_Array _src, منطقی & res )
{
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), kercn = ocl::predict_Optimal_سمتیہ_چوڑائی(_src);
    منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    اگر (گہرائی == CV_64F && !doubleSupport)
        واپس جھوٹ;

    صحیح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروہ_سائز();

    صحیح wgs2_aligned = 1;
    جب (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)),
                         ocl::type_مقصد_Str(گہرائی), (صحیح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), src.ستون_تعداد, (صحیح)src.کل(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    اگر (k.تسلسل(1, &globalsize, &wgs, سچ))
        واپس res = (saturate_cast<صحیح>(cv::مجموع(db.getMat(ACCESS_READ))[0])>0), سچ;
    واپس جھوٹ;
}
#اگر_ختم

منطقی hasNonZero(داخلہ_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    منطقی res = جھوٹ;

#اگر_تعریف_شدہ HAVE_OPENCL
    CV_OCL_تسلسل_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#اگر_ختم

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.گہرائی());
    CV_Assert( func != 0 );

    اگر (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        اگر (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.کل());
        ورنہ
            برائے(صحیح صف = 0, صفیں_گنتی = src.صفیں ; !res && (صف<صفیں_گنتی) ; ++صف)
                res |= func(src.ptr<uchar>(صف), src.ستون_تعداد);
    }
    ورنہ//if (src.dims != 2)
    {
        ثابت Mat* arrays[] = {&src, خالی_اشاریہ};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        برائے(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ثابت Mat& plane = itNAry.planes[0];
            اگر (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.کل());
            ورنہ
              برائے(صحیح صف = 0, صفیں_گنتی = plane.صفیں ; !res && (صف<صفیں_گنتی) ; ++صف)
                  res |= func(plane.ptr<uchar>(صف), plane.ستون_تعداد);
        }
    }

    واپس res;
}

} // namespace
