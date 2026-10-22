// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "has_non_zero.simd.hpp"
#ضمن "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن HasNonZeroFunc getHasNonZeroFunc(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شده HAVE_OPENCL
ساكن منطقي ocl_hasNonZero( درون_داد_Array _src, منطقي & res )
{
    صحيح type = _src.type(), عمق = CV_MAT_عمق(type), kercn = ocl::predict_Optimal_وکتور_عرض(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (عمق == CV_64F && !doubleSupport)
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروه_اندازه();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(عمق, kercn)),
                         ocl::type_مقصد_Str(عمق), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(src), src.تعداد_ستون, (صحيح)src.مجموع(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    إذا (k.پیاپی(1, &globalsize, &wgs, درست))
        أرجع res = (saturate_cast<صحيح>(cv::جمع(db.getMat(ACCESS_READ))[0])>0), درست;
    أرجع خطأ;
}
#پایان_اگر

منطقي hasNonZero(درون_داد_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    منطقي res = خطأ;

#اگر_تعریف_شده HAVE_OPENCL
    CV_OCL_پیاپی_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#پایان_اگر

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.عمق());
    CV_Assert( func != 0 );

    إذا (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        إذا (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.مجموع());
        وإلا
            لكل(صحيح ردیف = 0, ردیفها_شمار = src.ردیفها ; !res && (ردیف<ردیفها_شمار) ; ++ردیف)
                res |= func(src.ptr<uchar>(ردیف), src.تعداد_ستون);
    }
    وإلا//if (src.dims != 2)
    {
        ثابت Mat* arrays[] = {&src, اشاره_تهی};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        لكل(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ثابت Mat& plane = itNAry.planes[0];
            إذا (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.مجموع());
            وإلا
              لكل(صحيح ردیف = 0, ردیفها_شمار = plane.ردیفها ; !res && (ردیف<ردیفها_شمار) ; ++ردیف)
                  res |= func(plane.ptr<uchar>(ردیف), plane.تعداد_ستون);
        }
    }

    أرجع res;
}

} // namespace
