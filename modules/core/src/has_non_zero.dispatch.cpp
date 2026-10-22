// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "has_non_zero.simd.hpp"
#include "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static HasNonZeroFunc getHasNonZeroFunc(ចំនួនគត់ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_hasNonZero( InputArray _src, bool & res )
{
    ចំនួនគត់ type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ប្រសិនបើ (depth == CV_64F && !doubleSupport)
        ត្រឡប់ false;

    ចំនួនគត់ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    ចំនួនគត់ wgs2_aligned = 1;
    ខណៈពេល (wgs2_aligned < (ចំនួនគត់)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (ចំនួនគត់)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ប្រសិនបើ (k.empty())
        ត្រឡប់ false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ចំនួនគត់)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ប្រសិនបើ (k.run(1, &globalsize, &wgs, true))
        ត្រឡប់ res = (saturate_cast<ចំនួនគត់>(cv::sum(db.getMat(ACCESS_READ))[0])>0), true;
    ត្រឡប់ false;
}
#endif

bool hasNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    ចំនួនគត់ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    bool res = false;

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.depth());
    CV_Assert( func != 0 );

    ប្រសិនបើ (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        ប្រសិនបើ (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        បើមិនដូច្នេះ
            សម្រាប់(ចំនួនគត់ row = 0, rowsCount = src.rows ; !res && (row<rowsCount) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    បើមិនដូច្នេះ//if (src.dims != 2)
    {
        const Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        សម្រាប់(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            const Mat& plane = itNAry.planes[0];
            ប្រសិនបើ (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            បើមិនដូច្នេះ
              សម្រាប់(ចំនួនគត់ row = 0, rowsCount = plane.rows ; !res && (row<rowsCount) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    ត្រឡប់ res;
}

} // namespace
