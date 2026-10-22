// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"

#شامل "has_non_zero.simd.hpp"
#شامل "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

فضاینام cv {

ایستا HasNonZeroFunc getHasNonZeroFunc(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ایستا منطقی ocl_hasNonZero( درون_داد_Array _src, منطقی & res )
{
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    اگر (depth == CV_64F && !doubleSupport)
        بازگشت نادرست;

    صحیح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    صحیح wgs2_aligned = 1;
    وقتی (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (صحیح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    اگر (k.empty())
        بازگشت نادرست;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحیح)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    اگر (k.run(1, &globalsize, &wgs, درست))
        بازگشت res = (saturate_cast<صحیح>(cv::sum(db.getMat(ACCESS_READ))[0])>0), درست;
    بازگشت نادرست;
}
#endif

منطقی hasNonZero(درون_داد_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    منطقی res = نادرست;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    اگر (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        اگر (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        وگرنه
            برای(صحیح row = 0, rows_شمار = src.rows ; !res && (row<rows_شمار) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    وگرنه//if (src.dims != 2)
    {
        ثابت Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        برای(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ثابت Mat& plane = itNAry.planes[0];
            اگر (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            وگرنه
              برای(صحیح row = 0, rows_شمار = plane.rows ; !res && (row<rows_شمار) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    بازگشت res;
}

} // namespace
