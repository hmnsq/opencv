// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

#include "convert_scale.simd.hpp"
#include "convert_scale.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv
{

static BinaryFunc getCvtScaleAbsFunc(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCvtScaleAbsFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

BinaryFunc getConvertScaleFunc(ကိန်းပြည့် sdepth, ကိန်းပြည့် ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertScaleFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

static bool ocl_convertScaleAbs( InputArray _src, OutputArray _dst, dviguu_na alpha, dviguu_na beta )
{
    const ocl::Device & d = ocl::Device::getDefault();

    ကိန်းပြည့် type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    bool doubleSupport = d.doubleFPConfig() > 0;
    အကယ်၍ (!doubleSupport && depth == CV_64F)
        ပြန်ပေး false;

    _dst.createSameSize(_src, CV_8UC(cn));
    ကိန်းပြည့် kercn = 1;
    အကယ်၍ (d.isIntel())
    {
        static const ကိန်းပြည့် vectorWidths[] = {4, 4, 4, 4, 4, 4, 4, -1};
        kercn = ocl::checkOptimalVectorWidth( vectorWidths, _src, _dst,
                                              noArray(), noArray(), noArray(),
                                              noArray(), noArray(), noArray(),
                                              noArray(), ocl::OCL_VECTOR_MAX);
    }
    မဟုတ်ပါက
        kercn = ocl::predictOptimalVectorWidthMax(_src, _dst);

    ကိန်းပြည့် rowsPerWI = d.isIntel() ? 4 : 1;
    var_na cvt[2][50];
    ကိန်းပြည့် wdepth = std::max(depth, CV_32F);
    String build_opt = format("-D OP_CONVERT_SCALE_ABS -D UNARY_OP -D dstT=%s -D DEPTH_dst=%d -D srcT1=%s"
                         " -D workT=%s -D wdepth=%d -D convertToWT1=%s -D convertToDT=%s"
                         " -D workT1=%s -D rowsPerWI=%d%s",
                         ocl::typeToStr(CV_8UC(kercn)), CV_8U,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)), wdepth,
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], parimaa_na(cvt[0])),
                         ocl::convertTypeStr(wdepth, CV_8U, kercn, cvt[1], parimaa_na(cvt[1])),
                         ocl::typeToStr(wdepth), rowsPerWI,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "");
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc, build_opt);
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    UMat src = _src.getUMat();
    UMat dst = _dst.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    အကယ်၍ (wdepth == CV_32F)
        k.args(srcarg, dstarg, (float)alpha, (float)beta);
    မဟုတ်ပါက အကယ်၍ (wdepth == CV_64F)
        k.args(srcarg, dstarg, alpha, beta);

    size_t globalsize[2] = { (size_t)src.cols * cn / kercn, ((size_t)src.rows + rowsPerWI - 1) / rowsPerWI };
    ပြန်ပေး k.run(2, globalsize, NULL, false);
}

#endif

void convertScaleAbs(InputArray _src, OutputArray _dst, dviguu_na alpha, dviguu_na beta)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat(),
               ocl_convertScaleAbs(_src, _dst, alpha, beta))

    Mat src = _src.getMat();
    ကိန်းပြည့် cn = src.channels();
    dviguu_na scale[] = {alpha, beta};
    _dst.create( src.size, CV_8UC(cn) );
    Mat dst = _dst.getMat();
    BinaryFunc func = getCvtScaleAbsFunc(src.depth());
    CV_Assert( func != 0 );

    အကယ်၍( src.dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dst, cn);
        func( src.ptr(), src.step, 0, 0, dst.ptr(), dst.step, sz, scale );
    }
    မဟုတ်ပါက
    {
        const Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((ကိန်းပြည့်)it.size*cn, 1);

        အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
            func( ptrs[0], 0, 0, 0, ptrs[1], 0, sz, scale );
    }
}

} // namespace
