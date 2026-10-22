// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "stat.hpp"

#અન્તર્ભાવ "has_non_zero.simd.hpp"
#અન્તર્ભાવ "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv {

સ્થિર HasNonZeroFunc getHasNonZeroFunc(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
સ્થિર તાર્કિક ocl_hasNonZero( આગત_Array _src, તાર્કિક & res )
{
    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    તાર્કિક doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    જો (depth == CV_64F && !doubleSupport)
        પાછા ખોટું;

    પૂર્ણાંક dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    પૂર્ણાંક wgs2_aligned = 1;
    જ્યારે (wgs2_aligned < (પૂર્ણાંક)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (પૂર્ણાંક)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    જો (k.empty())
        પાછા ખોટું;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (પૂર્ણાંક)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    જો (k.run(1, &globalsize, &wgs, સાચું))
        પાછા res = (saturate_cast<પૂર્ણાંક>(cv::sum(db.getMat(ACCESS_READ))[0])>0), સાચું;
    પાછા ખોટું;
}
#endif

તાર્કિક hasNonZero(આગત_Array _src)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    તાર્કિક res = ખોટું;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    જો (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        જો (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        નહીંતર
            માટે(પૂર્ણાંક row = 0, rows_ગણતરી = src.rows ; !res && (row<rows_ગણતરી) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    નહીંતર//if (src.dims != 2)
    {
        અચલ Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        માટે(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            અચલ Mat& plane = itNAry.planes[0];
            જો (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            નહીંતર
              માટે(પૂર્ણાંક row = 0, rows_ગણતરી = plane.rows ; !res && (row<rows_ગણતરી) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    પાછા res;
}

} // namespace
