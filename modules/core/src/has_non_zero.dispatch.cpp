// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "has_non_zero.simd.hpp"
#ضمن "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن HasNonZeroFunc getHasNonZeroFunc(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#جے_تعریف_شدہ HAVE_OPENCL
ساكن منطقي ocl_hasNonZero( داخلہ_Array _src, منطقي & res )
{
    صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), kercn = ocl::predict_Optimal_سمتیہ_چوڑائی(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (ڈونگھائی == CV_64F && !doubleSupport)
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروہ_ناپ();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, kercn)),
                         ocl::type_منزل_Str(ڈونگھائی), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(src), src.خانے_گنتی, (صحيح)src.کل(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    إذا (k.لگاتار(1, &globalsize, &wgs, سچ))
        أرجع res = (saturate_cast<صحيح>(cv::جوڑ(db.getMat(ACCESS_READ))[0])>0), سچ;
    أرجع خطأ;
}
#جے_انت

منطقي hasNonZero(داخلہ_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    منطقي res = خطأ;

#جے_تعریف_شدہ HAVE_OPENCL
    CV_OCL_لگاتار_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#جے_انت

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.ڈونگھائی());
    CV_Assert( func != 0 );

    إذا (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        إذا (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.کل());
        وإلا
            لكل(صحيح صف = 0, صفاں_گنتی = src.صفاں ; !res && (صف<صفاں_گنتی) ; ++صف)
                res |= func(src.ptr<uchar>(صف), src.خانے_گنتی);
    }
    وإلا//if (src.dims != 2)
    {
        ثابت Mat* arrays[] = {&src, خالی_سوچک};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        لكل(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ثابت Mat& plane = itNAry.planes[0];
            إذا (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.کل());
            وإلا
              لكل(صحيح صف = 0, صفاں_گنتی = plane.صفاں ; !res && (صف<صفاں_گنتی) ; ++صف)
                  res |= func(plane.ptr<uchar>(صف), plane.خانے_گنتی);
        }
    }

    أرجع res;
}

} // namespace
