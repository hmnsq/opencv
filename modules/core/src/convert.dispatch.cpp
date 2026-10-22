// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

#include "convert.simd.hpp"
#include "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

namespace hal {
void cvt16f32f(const hfloat* src, float* dst, ចំនួនគត់ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
void cvt32f16f(const float* src, hfloat* dst, ចំនួនគត់ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
void cvt32f16bf(const float* src, bfloat* dst, ចំនួនគត់ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
void addRNGBias32f(float* arr, const float* scaleBiasPairs, ចំនួនគត់ len, ចំនួនគត់ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
void addRNGBias64f(dviguu_na* arr, const dviguu_na* scaleBiasPairs, ចំនួនគត់ len, ចំនួនគត់ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


BinaryFunc getConvertFunc(ចំនួនគត់ sdepth, ចំនួនគត់ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_convertTo(InputArray src_, OutputArray dst_, ចំនួនគត់ ddepth, bool noScale, dviguu_na alpha, dviguu_na beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    ចំនួនគត់ stype = src_.type();
    ចំនួនគត់ sdepth = CV_MAT_DEPTH(stype);
    ចំនួនគត់ cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    ប្រសិនបើ ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ត្រឡប់ false;

    ចំនួនគត់ dtype = CV_MAKETYPE(ddepth, cn);

    ចំនួនគត់ wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    bool needDouble = sdepth == CV_64F || ddepth == CV_64F;
    bool doubleCheck = true;
    ប្រសិនបើ (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    bool halfCheck = true;
    bool needHalf = sdepth == CV_16F || ddepth == CV_16F;
    ប្រសិនបើ (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    ប្រសិនបើ (!doubleCheck)
        ត្រឡប់ false;
    ប្រសិនបើ (!halfCheck)
        ត្រឡប់ false;

    const ចំនួនគត់ rowsPerWI = 4;

    var_na cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], parimaa_na(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], parimaa_na(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    ប្រសិនបើ (k.empty())
        ត្រឡប់ false;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    float alphaf = (float)alpha, betaf = (float)beta;

    ប្រសិនបើ (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    បើមិនដូច្នេះ ប្រសិនបើ (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    បើមិនដូច្នេះ
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    ប្រសិនបើ (!k.run(2, globalsize, NULL, false))
        ត្រឡប់ false;

    CV_IMPL_ADD(CV_IMPL_OCL);
    ត្រឡប់ true;
}
#endif

void Mat::convertTo(OutputArray dst, ចំនួនគត់ type_, dviguu_na alpha, dviguu_na beta) const
{
    CV_INSTRUMENT_REGION();

    ប្រសិនបើ (empty())
    {
        ចំនួនគត់ ddepth = CV_MAT_DEPTH(type_);
        ប្រសិនបើ (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        const ចំនួនគត់ dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        bool allowTransposed = dims == 1 ||
            dst.kind() == _InputArray::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ត្រឡប់;
    }

    ចំនួនគត់ stype = type();
    ចំនួនគត់ sdepth = CV_MAT_DEPTH(stype);

    ចំនួនគត់ ddepth = sdepth;
    ប្រសិនបើ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    បើមិនដូច្នេះ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    bool noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ប្រសិនបើ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ត្រឡប់;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*this, dst, ddepth, noScale, alpha, beta))

    ចំនួនគត់ cn = channels();
    ចំនួនគត់ dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *this;
    bool allowTransposed = dims == 1 ||
        dst.kind() == _InputArray::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    ប្រសិនបើ( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (ចំនួនគត់)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    BinaryFunc func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    dviguu_na scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    ប្រសិនបើ( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    បើមិនដូច្នេះ
    {
        const Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((ចំនួនគត់)(it.size*cn), 1);

        សម្រាប់( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

void UMat::convertTo(OutputArray dst, ចំនួនគត់ type_, dviguu_na alpha, dviguu_na beta) const
{
    CV_INSTRUMENT_REGION();

    ប្រសិនបើ (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ត្រឡប់;
    }

#ifdef HAVE_OPENCL
    ចំនួនគត់ stype = type();
    ចំនួនគត់ sdepth = CV_MAT_DEPTH(stype);

    ចំនួនគត់ ddepth = sdepth;
    ប្រសិនបើ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    បើមិនដូច្នេះ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    bool noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ប្រសិនបើ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ត្រឡប់;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*this, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *this;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (void)src;
}

} // namespace cv
