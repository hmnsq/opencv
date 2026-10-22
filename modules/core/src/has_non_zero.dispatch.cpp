// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "has_non_zero.simd.hpp"
#ضمن "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن HasNonZeroFunc getHasNonZeroFunc(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ساكن منطقي ocl_hasNonZero( InputArray _src, منطقي & res )
{
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    אם (depth == CV_64F && !doubleSupport)
        החזר שקר;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    صحيح wgs2_aligned = 1;
    כלעוד (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    אם (k.empty())
        החזר שקר;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    אם (k.run(1, &globalsize, &wgs, אמת))
        החזר res = (saturate_cast<صحيح>(cv::sum(db.getMat(ACCESS_READ))[0])>0), אמת;
    החזר שקר;
}
#endif

منطقي hasNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    منطقي res = שקר;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    אם (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        אם (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        אחרת
            עבור(صحيح row = 0, rowsCount = src.rows ; !res && (row<rowsCount) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    אחרת//if (src.dims != 2)
    {
        ثابت Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        עבור(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ثابت Mat& plane = itNAry.planes[0];
            אם (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            אחרת
              עבור(صحيح row = 0, rowsCount = plane.rows ; !res && (row<rowsCount) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    החזר res;
}

} // namespace
