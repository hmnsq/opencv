// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

#ಅನ್ತರ್ಭಾವ "has_non_zero.simd.hpp"
#ಅನ್ತರ್ಭಾವ "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ HasNonZeroFunc getHasNonZeroFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_hasNonZero( ಒಳಹರಿವು_Array _src, ತಾರ್ಕಿಕ & res )
{
    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ಯದಿ (depth == CV_64F && !doubleSupport)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    ಪೂರ್ಣಾಙ್ಕ wgs2_aligned = 1;
    ಯಾವತ್ (wgs2_aligned < (ಪೂರ್ಣಾಙ್ಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (ಪೂರ್ಣಾಙ್ಕ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ಪೂರ್ಣಾಙ್ಕ)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ಯದಿ (k.run(1, &globalsize, &wgs, ಸತ್ಯ))
        ಪ್ರತ್ಯಾಗಮ res = (saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(cv::sum(db.getMat(ACCESS_READ))[0])>0), ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}
#endif

ತಾರ್ಕಿಕ hasNonZero(ಒಳಹರಿವು_Array _src)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    ತಾರ್ಕಿಕ res = ಅಸತ್ಯ;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    ಯದಿ (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        ಯದಿ (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        ಅನ್ಯಥಾ
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ row = 0, rows_ಎಣಿಕೆ = src.rows ; !res && (row<rows_ಎಣಿಕೆ) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    ಅನ್ಯಥಾ//if (src.dims != 2)
    {
        ಧ್ರುವ Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        ಪ್ರತಿ(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ಧ್ರುವ Mat& plane = itNAry.planes[0];
            ಯದಿ (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            ಅನ್ಯಥಾ
              ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ row = 0, rows_ಎಣಿಕೆ = plane.rows ; !res && (row<rows_ಎಣಿಕೆ) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    ಪ್ರತ್ಯಾಗಮ res;
}

} // namespace
