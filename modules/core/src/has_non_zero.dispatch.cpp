// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "stat.hpp"

#অন্তর্ভাব "has_non_zero.simd.hpp"
#অন্তর্ভাব "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

স্থির HasNonZeroFunc getHasNonZeroFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
স্থির তার্কিক ocl_hasNonZero( প্রবেশ_Array _src, তার্কিক & res )
{
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    তার্কিক doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    যদি (depth == CV_64F && !doubleSupport)
        প্রত্যাগম অসত্য;

    পূর্ণাঙ্ক dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    পূর্ণাঙ্ক wgs2_aligned = 1;
    যাবত্ (wgs2_aligned < (পূর্ণাঙ্ক)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (পূর্ণাঙ্ক)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (পূর্ণাঙ্ক)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    যদি (k.run(1, &globalsize, &wgs, সত্য))
        প্রত্যাগম res = (saturate_cast<পূর্ণাঙ্ক>(cv::sum(db.getMat(ACCESS_READ))[0])>0), সত্য;
    প্রত্যাগম অসত্য;
}
#endif

তার্কিক hasNonZero(প্রবেশ_Array _src)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    তার্কিক res = অসত্য;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    যদি (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        যদি (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        অন্যথা
            প্রতি(পূর্ণাঙ্ক row = 0, rows_গুনতি = src.rows ; !res && (row<rows_গুনতি) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    অন্যথা//if (src.dims != 2)
    {
        ধ্রুব Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        প্রতি(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ধ্রুব Mat& plane = itNAry.planes[0];
            যদি (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            অন্যথা
              প্রতি(পূর্ণাঙ্ক row = 0, rows_গুনতি = plane.rows ; !res && (row<rows_গুনতি) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    প্রত্যাগম res;
}

} // namespace
