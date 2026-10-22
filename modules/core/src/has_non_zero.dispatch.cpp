// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "stat.hpp"

#ଅନ୍ତର୍ଭାଵ "has_non_zero.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv {

ସ୍ଥିର HasNonZeroFunc getHasNonZeroFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ ocl_hasNonZero( ନିବେଶ_Array _src, ତାର୍କିକ & res )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    ତାର୍କିକ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ଯଦି (depth == CV_64F && !doubleSupport)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wgs2_aligned = 1;
    ଯେତେବେଳେ (wgs2_aligned < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ଯଦି (k.run(1, &globalsize, &wgs, ସତ୍ୟ))
        ଫେରନ୍ତୁ res = (saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(cv::sum(db.getMat(ACCESS_READ))[0])>0), ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
#endif

ତାର୍କିକ hasNonZero(ନିବେଶ_Array _src)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    ତାର୍କିକ res = ମିଥ୍ୟା;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    ଯଦି (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        ଯଦି (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        ଅନ୍ୟଥା
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = 0, rows_ଗଣନା = src.rows ; !res && (row<rows_ଗଣନା) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    ଅନ୍ୟଥା//if (src.dims != 2)
    {
        ଅଚଳ Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        ପାଇଁ(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ଅଚଳ Mat& plane = itNAry.planes[0];
            ଯଦି (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            ଅନ୍ୟଥା
              ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = 0, rows_ଗଣନା = plane.rows ; !res && (row<rows_ଗଣନା) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    ଫେରନ୍ତୁ res;
}

} // namespace
