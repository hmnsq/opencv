// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "has_non_zero.simd.hpp"
#include "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static HasNonZeroFunc getHasNonZeroFunc(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_hasNonZero( InputArray _src, bool & res )
{
    ကိန်းပြည့် type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    အကယ်၍ (depth == CV_64F && !doubleSupport)
        ပြန်ပေး false;

    ကိန်းပြည့် dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    ကိန်းပြည့် wgs2_aligned = 1;
    နေစဉ် (wgs2_aligned < (ကိန်းပြည့်)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (ကိန်းပြည့်)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ကိန်းပြည့်)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    အကယ်၍ (k.run(1, &globalsize, &wgs, true))
        ပြန်ပေး res = (saturate_cast<ကိန်းပြည့်>(cv::sum(db.getMat(ACCESS_READ))[0])>0), true;
    ပြန်ပေး false;
}
#endif

bool hasNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    ကိန်းပြည့် type = _src.type(), cn = CV_MAT_CN(type);
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

    အကယ်၍ (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        အကယ်၍ (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.total());
        မဟုတ်ပါက
            အတွက်(ကိန်းပြည့် row = 0, rowsCount = src.rows ; !res && (row<rowsCount) ; ++row)
                res |= func(src.ptr<uchar>(row), src.cols);
    }
    မဟုတ်ပါက//if (src.dims != 2)
    {
        const Mat* arrays[] = {&src, nullptr};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        အတွက်(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            const Mat& plane = itNAry.planes[0];
            အကယ်၍ (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.total());
            မဟုတ်ပါက
              အတွက်(ကိန်းပြည့် row = 0, rowsCount = plane.rows ; !res && (row<rowsCount) ; ++row)
                  res |= func(plane.ptr<uchar>(row), plane.cols);
        }
    }

    ပြန်ပေး res;
}

} // namespace
