// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "stat.hpp"

#அந்தர்பாவ "has_non_zero.simd.hpp"
#அந்தர்பாவ "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv {

நிலையான HasNonZeroFunc getHasNonZeroFunc(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
நிலையான தருக்க ocl_hasNonZero( உள்ளீடு_Array _src, தருக்க & res )
{
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    தருக்க doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    எனில் (depth == CV_64F && !doubleSupport)
        திரும்பு பொய்;

    முழு dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    முழு wgs2_aligned = 1;
    போது (wgs2_aligned < (முழு)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (முழு)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (முழு)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    எனில் (k.run(1, &globalsize, &wgs, உண்மை))
        திரும்பு res = (saturate_cast<முழு>(cv::sum(db.getMat(ACCESS_READ))[0])>0), உண்மை;
    திரும்பு பொய்;
}
#endif

தருக்க hasNonZero(உள்ளீடு_Array _src)
{
    CV_INSTRUMENT_REGION();

    முழு type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    தருக்க res = பொய்;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    எனில் (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        எனில் (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        இல்லையெனில்
            க்கு(முழு row = 0, rows_மொத்தம் = src.rows ; !res && (row<rows_மொத்தம்) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    இல்லையெனில்//if (src.dims != 2)
    {
        மாறிலி Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        க்கு(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            மாறிலி Mat& plane = itNAry.planes[0];
            எனில் (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            இல்லையெனில்
              க்கு(முழு row = 0, rows_மொத்தம் = plane.rows ; !res && (row<rows_மொத்தம்) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    திரும்பு res;
}

} // namespace
