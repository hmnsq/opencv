// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "has_non_zero.simd.hpp"
#ضمن "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن HasNonZeroFunc getHasNonZeroFunc(صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#که_تعریف_شوی HAVE_OPENCL
ساكن منطقي ocl_hasNonZero( ننوت_Array _src, منطقي & res )
{
    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type), kercn = ocl::predict_Optimal_ویکټور_پلنوالی(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (ژوروالی == CV_64F && !doubleSupport)
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ډله_اندازه();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, kercn)),
                         ocl::type_منزل_Str(ژوروالی), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(src), src.ستون_شمېر, (صحيح)src.ټول(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    إذا (k.پرلپسې(1, &globalsize, &wgs, ریښتیا))
        أرجع res = (saturate_cast<صحيح>(cv::مجموعه(db.getMat(ACCESS_READ))[0])>0), ریښتیا;
    أرجع خطأ;
}
#د_که_پای

منطقي hasNonZero(ننوت_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    منطقي res = خطأ;

#که_تعریف_شوی HAVE_OPENCL
    CV_OCL_پرلپسې_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#د_که_پای

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.ژوروالی());
    CV_Assert( func != 0 );

    إذا (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        إذا (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.ټول());
        وإلا
            لكل(صحيح لیکه = 0, لیکې_شمېر = src.لیکې ; !res && (لیکه<لیکې_شمېر) ; ++لیکه)
                res |= func(src.ptr<uchar>(لیکه), src.ستون_شمېر);
    }
    وإلا//if (src.dims != 2)
    {
        ثابت Mat* arrays[] = {&src, تش_اشاره};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        لكل(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ثابت Mat& plane = itNAry.planes[0];
            إذا (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.ټول());
            وإلا
              لكل(صحيح لیکه = 0, لیکې_شمېر = plane.لیکې ; !res && (لیکه<لیکې_شمېر) ; ++لیکه)
                  res |= func(plane.ptr<uchar>(لیکه), plane.ستون_شمېر);
        }
    }

    أرجع res;
}

} // namespace
