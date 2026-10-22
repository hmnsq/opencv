// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"

#समावेश "convert.simd.hpp"
#समावेश "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

namespace hal {
व्योम cvt16f32f(स्थिर hfloat* src, भग्न* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
व्योम cvt32f16f(स्थिर भग्न* src, hfloat* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
व्योम cvt32f16bf(स्थिर भग्न* src, bfloat* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
व्योम addRNGBias32f(भग्न* arr, स्थिर भग्न* scaleBiasPairs, पूर्णांक len, पूर्णांक cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
व्योम addRNGBias64f(द्विगुन* arr, स्थिर द्विगुन* scaleBiasPairs, पूर्णांक len, पूर्णांक cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


द्विआधारी_Func getConvertFunc(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#अगर_घोषित HAVE_OPENCL
अटल bool ocl_convertTo(निवेश_Array src_, OutputArray dst_, पूर्णांक ddepth, bool noScale, द्विगुन alpha, द्विगुन beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    पूर्णांक stype = src_.type();
    पूर्णांक sdepth = CV_MAT_DEPTH(stype);
    पूर्णांक cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    अगर ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        वापस false;

    पूर्णांक dtype = CV_MAKETYPE(ddepth, cn);

    पूर्णांक wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    bool needDouble = sdepth == CV_64F || ddepth == CV_64F;
    bool doubleCheck = true;
    अगर (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    bool halfCheck = true;
    bool needHalf = sdepth == CV_16F || ddepth == CV_16F;
    अगर (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    अगर (!doubleCheck)
        वापस false;
    अगर (!halfCheck)
        वापस false;

    स्थिर पूर्णांक rowsPerWI = 4;

    अक्षर cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], माप(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], माप(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    अगर (k.empty())
        वापस false;

    UMat src = src_.getUMat();
    dst_.create_Same_आकार(src_, dtype);
    UMat dst = dst_.getUMat();

    भग्न alphaf = (भग्न)alpha, betaf = (भग्न)beta;

    अगर (noScale)
        k.args(ocl::KernelArg::Read_Only_No_आकार(src), ocl::KernelArg::WriteOnly(dst, cn));
    अन्यथा अगर (wdepth == CV_32F)
        k.args(ocl::KernelArg::Read_Only_No_आकार(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    अन्यथा
        k.args(ocl::KernelArg::Read_Only_No_आकार(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    माप_प्रकार globalsize[2] = {
        (माप_प्रकार)dst.cols * cn,
        divUp((माप_प्रकार)dst.rows, rowsPerWI)
    };
    अगर (!k.run(2, globalsize, शून्य, false))
        वापस false;

    CV_IMPL_ADD(CV_IMPL_OCL);
    वापस true;
}
#पूर्ण_अगर

व्योम Mat::convertTo(OutputArray dst, पूर्णांक type_, द्विगुन alpha, द्विगुन beta) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर (empty())
    {
        पूर्णांक ddepth = CV_MAT_DEPTH(type_);
        अगर (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        स्थिर पूर्णांक dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        bool allowTransposed = dims == 1 ||
            dst.kind() == निवेश_Array_2::STD_VECTOR ||
            (dst.fixed_आकार() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        वापस;
    }

    पूर्णांक stype = type();
    पूर्णांक sdepth = CV_MAT_DEPTH(stype);

    पूर्णांक ddepth = sdepth;
    अगर (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    अन्यथा
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    bool noScale = std::भ_असल(alpha - 1) < द्विग_अंतर && std::भ_असल(beta) < द्विग_अंतर;
    अगर (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        वापस;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*यह, dst, ddepth, noScale, alpha, beta))

    पूर्णांक cn = channels();
    पूर्णांक dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *यह;
    bool allowTransposed = dims == 1 ||
        dst.kind() == निवेश_Array_2::STD_VECTOR ||
        (dst.fixed_आकार() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    अगर( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.तथ्य, src.step, dstMat.तथ्य, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    अन्यथा अगर( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.तथ्य, 0, dstMat.तथ्य, 0, (पूर्णांक)src.योग() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    द्विआधारी_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    द्विगुन scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    अगर( dims <= 2 )
    {
        आकार sz = getContinuousSize2D(src, dstMat, cn);
        func(src.तथ्य, src.step, 0, 0, dstMat.तथ्य, dstMat.step, sz, scale);
    }
    अन्यथा
    {
        स्थिर Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        आकार sz((पूर्णांक)(it.size*cn), 1);

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

व्योम UMat::convertTo(OutputArray dst, पूर्णांक type_, द्विगुन alpha, द्विगुन beta) स्थिर
{
    CV_INSTRUMENT_REGION();

    अगर (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        वापस;
    }

#अगर_घोषित HAVE_OPENCL
    पूर्णांक stype = type();
    पूर्णांक sdepth = CV_MAT_DEPTH(stype);

    पूर्णांक ddepth = sdepth;
    अगर (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    अन्यथा
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    bool noScale = std::भ_असल(alpha - 1) < द्विग_अंतर && std::भ_असल(beta) < द्विग_अंतर;
    अगर (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        वापस;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*यह, dst, ddepth, noScale, alpha, beta))
#पूर्ण_अगर // HAVE_OPENCL

    UMat src = *यह;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (व्योम)src;
}

} // namespace cv
