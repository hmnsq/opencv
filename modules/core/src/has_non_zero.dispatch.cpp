// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"

#अन्तर्भाव "has_non_zero.simd.hpp"
#अन्तर्भाव "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

स्थिर HasNonZeroFunc getHasNonZeroFunc(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_hasNonZero( निवेशः_Array _src, तार्किक & res )
{
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    जइ (depth == CV_64F && !doubleSupport)
        प्रत्यागम मिच्छ;

    पूर्णाङ्क dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    पूर्णाङ्क wgs2_aligned = 1;
    जाव (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (पूर्णाङ्क)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    जइ (k.empty())
        प्रत्यागम मिच्छ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णाङ्क)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    जइ (k.run(1, &globalsize, &wgs, सच्च))
        प्रत्यागम res = (saturate_cast<पूर्णाङ्क>(cv::sum(db.getMat(ACCESS_READ))[0])>0), सच्च;
    प्रत्यागम मिच्छ;
}
#endif

तार्किक hasNonZero(निवेशः_Array _src)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    तार्किक res = मिच्छ;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    जइ (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        जइ (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        अण्णहा
            प्रति(पूर्णाङ्क row = 0, rows_गणनम् = src.rows ; !res && (row<rows_गणनम्) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    अण्णहा//if (src.dims != 2)
    {
        ध्रुव Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        प्रति(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ध्रुव Mat& plane = itNAry.planes[0];
            जइ (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            अण्णहा
              प्रति(पूर्णाङ्क row = 0, rows_गणनम् = plane.rows ; !res && (row<rows_गणनम्) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    प्रत्यागम res;
}

} // namespace
