// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

#include "convert.simd.hpp"
#include "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

namespace hal {
void cvt16f32f(const hfloat* src, float* dst, ຈຳນວນເຕັມ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
void cvt32f16f(const float* src, hfloat* dst, ຈຳນວນເຕັມ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
void cvt32f16bf(const float* src, bfloat* dst, ຈຳນວນເຕັມ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
void addRNGBias32f(float* arr, const float* scaleBiasPairs, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
void addRNGBias64f(dviguu_na* arr, const dviguu_na* scaleBiasPairs, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


BinaryFunc getConvertFunc(ຈຳນວນເຕັມ sdepth, ຈຳນວນເຕັມ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_convertTo(InputArray src_, OutputArray dst_, ຈຳນວນເຕັມ ddepth, bool noScale, dviguu_na alpha, dviguu_na beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    ຈຳນວນເຕັມ stype = src_.type();
    ຈຳນວນເຕັມ sdepth = CV_MAT_DEPTH(stype);
    ຈຳນວນເຕັມ cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    ຖ້າ ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        ສົ່ງຄືນ false;

    ຈຳນວນເຕັມ dtype = CV_MAKETYPE(ddepth, cn);

    ຈຳນວນເຕັມ wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    bool needDouble = sdepth == CV_64F || ddepth == CV_64F;
    bool doubleCheck = true;
    ຖ້າ (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    bool halfCheck = true;
    bool needHalf = sdepth == CV_16F || ddepth == CV_16F;
    ຖ້າ (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    ຖ້າ (!doubleCheck)
        ສົ່ງຄືນ false;
    ຖ້າ (!halfCheck)
        ສົ່ງຄືນ false;

    const ຈຳນວນເຕັມ rowsPerWI = 4;

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

    ຖ້າ (k.empty())
        ສົ່ງຄືນ false;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    float alphaf = (float)alpha, betaf = (float)beta;

    ຖ້າ (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    ບໍ່ດັ່ງນັ້ນ
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    size_t globalsize[2] = {
        (size_t)dst.cols * cn,
        divUp((size_t)dst.rows, rowsPerWI)
    };
    ຖ້າ (!k.run(2, globalsize, NULL, false))
        ສົ່ງຄືນ false;

    CV_IMPL_ADD(CV_IMPL_OCL);
    ສົ່ງຄືນ true;
}
#endif

void Mat::convertTo(OutputArray dst, ຈຳນວນເຕັມ type_, dviguu_na alpha, dviguu_na beta) const
{
    CV_INSTRUMENT_REGION();

    ຖ້າ (empty())
    {
        ຈຳນວນເຕັມ ddepth = CV_MAT_DEPTH(type_);
        ຖ້າ (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        const ຈຳນວນເຕັມ dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        bool allowTransposed = dims == 1 ||
            dst.kind() == _InputArray::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        ສົ່ງຄືນ;
    }

    ຈຳນວນເຕັມ stype = type();
    ຈຳນວນເຕັມ sdepth = CV_MAT_DEPTH(stype);

    ຈຳນວນເຕັມ ddepth = sdepth;
    ຖ້າ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    ບໍ່ດັ່ງນັ້ນ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    bool noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ຖ້າ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ສົ່ງຄືນ;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*this, dst, ddepth, noScale, alpha, beta))

    ຈຳນວນເຕັມ cn = channels();
    ຈຳນວນເຕັມ dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *this;
    bool allowTransposed = dims == 1 ||
        dst.kind() == _InputArray::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    ຖ້າ( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (ຈຳນວນເຕັມ)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    BinaryFunc func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    dviguu_na scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    ຖ້າ( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        const Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((ຈຳນວນເຕັມ)(it.size*cn), 1);

        ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

void UMat::convertTo(OutputArray dst, ຈຳນວນເຕັມ type_, dviguu_na alpha, dviguu_na beta) const
{
    CV_INSTRUMENT_REGION();

    ຖ້າ (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        ສົ່ງຄືນ;
    }

#ifdef HAVE_OPENCL
    ຈຳນວນເຕັມ stype = type();
    ຈຳນວນເຕັມ sdepth = CV_MAT_DEPTH(stype);

    ຈຳນວນເຕັມ ddepth = sdepth;
    ຖ້າ (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    ບໍ່ດັ່ງນັ້ນ
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    bool noScale = std::fabs(alpha - 1) < DBL_EPSILON && std::fabs(beta) < DBL_EPSILON;
    ຖ້າ (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        ສົ່ງຄືນ;
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
