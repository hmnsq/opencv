// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "convert.simd.hpp"
#अन्तर्भाव "convert.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

नामाकाश hal {
शून्य cvt16f32f(स्थिरांक hfloat* src, दशांश* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt16f32f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य cvt32f16f(स्थिरांक दशांश* src, hfloat* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16f, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य cvt32f16bf(स्थिरांक दशांश* src, bfloat* dst, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(cvt32f16bf, (src, dst, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य addRNGBias32f(दशांश* arr, स्थिरांक दशांश* scaleBiasPairs, पूर्णांक len, पूर्णांक cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias32f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}
शून्य addRNGBias64f(द्विगुण* arr, स्थिरांक द्विगुण* scaleBiasPairs, पूर्णांक len, पूर्णांक cn)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(addRNGBias64f, (arr, scaleBiasPairs, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace


द्विमान_Func getConvertFunc(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getConvertFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_convertTo(आदान_Array src_, OutputArray dst_, पूर्णांक ddepth, तार्किक noScale, द्विगुण alpha, द्विगुण beta)
{
    CV_INSTRUMENT_REGION();

    CV_Assert(ddepth >= 0);

    पूर्णांक stype = src_.type();
    पूर्णांक sdepth = CV_MAT_DEPTH(stype);
    पूर्णांक cn = CV_MAT_CN(stype);

    // FP8 types are not supported by the OpenCL kernels yet; fall back to CPU.
    जर ((sdepth >= CV_8F_E4M3FN && sdepth <= CV_8F_E4M3FNUZ) ||
        (ddepth >= CV_8F_E4M3FN && ddepth <= CV_8F_E4M3FNUZ))
        परत असत्य;

    पूर्णांक dtype = CV_MAKETYPE(ddepth, cn);

    पूर्णांक wdepth = (sdepth == CV_64F) ? CV_64F : CV_32F;

    तार्किक needDouble = sdepth == CV_64F || ddepth == CV_64F;
    तार्किक doubleCheck = सत्य;
    जर (needDouble)
    {
        doubleCheck = ocl::Device::getDefault().hasFP64();
    }
    तार्किक halfCheck = सत्य;
    तार्किक needHalf = sdepth == CV_16F || ddepth == CV_16F;
    जर (needHalf)
    {
        halfCheck = ocl::Device::getDefault().hasFP16();
    }

    जर (!doubleCheck)
        परत असत्य;
    जर (!halfCheck)
        परत असत्य;

    स्थिरांक पूर्णांक rowsPerWI = 4;

    अक्षर cvt[2][50];
    ocl::Kernel k("convertTo", ocl::core::convert_oclsrc,
                  format("-D srcT=%s -D WT=%s -D dstT=%s -D convertToWT=%s -D convertToDT=%s -D rowsPerWI=%d%s%s%s",
                         ocl::typeToStr(sdepth), ocl::typeToStr(wdepth), ocl::typeToStr(ddepth),
                         ocl::convertTypeStr(sdepth, wdepth, 1, cvt[0], आकार(cvt[0])),
                         ocl::convertTypeStr(wdepth, ddepth, 1, cvt[1], आकार(cvt[1])),
                         rowsPerWI,
                         needDouble ? " -D DOUBLE_SUPPORT" : "",
                         needHalf ? " -D HALF_SUPPORT" : "",
                         noScale ? " -D NO_SCALE" : ""
                  )
    );

    जर (k.empty())
        परत असत्य;

    UMat src = src_.getUMat();
    dst_.createSameSize(src_, dtype);
    UMat dst = dst_.getUMat();

    दशांश alphaf = (दशांश)alpha, betaf = (दशांश)beta;

    जर (noScale)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn));
    अन्यथा जर (wdepth == CV_32F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alphaf, betaf);
    अन्यथा
        k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnly(dst, cn), alpha, beta);

    माप_प्रकार globalsize[2] = {
        (माप_प्रकार)dst.cols * cn,
        divUp((माप_प्रकार)dst.rows, rowsPerWI)
    };
    जर (!k.run(2, globalsize, रिक्त, असत्य))
        परत असत्य;

    CV_IMPL_ADD(CV_IMPL_OCL);
    परत सत्य;
}
#endif

शून्य Mat::convertTo(OutputArray dst, पूर्णांक type_, द्विगुण alpha, द्विगुण beta) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर (empty())
    {
        पूर्णांक ddepth = CV_MAT_DEPTH(type_);
        जर (ddepth < 0)
            ddepth = dst.fixedType() ? dst.depth() : depth();
        स्थिरांक पूर्णांक dtype = CV_MAKETYPE(ddepth, channels());

        dst.release();
        तार्किक allowTransposed = dims == 1 ||
            dst.kind() == आदान_Array_2::STD_VECTOR ||
            (dst.fixedSize() && dst.dims() == 1);
        dst.create(size, dtype, -1, allowTransposed);
        परत;
    }

    पूर्णांक stype = type();
    पूर्णांक sdepth = CV_MAT_DEPTH(stype);

    पूर्णांक ddepth = sdepth;
    जर (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    अन्यथा
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    तार्किक noScale = std::भ_असल(alpha - 1) < द्विग_अंतर && std::भ_असल(beta) < द्विग_अंतर;
    जर (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        परत;
    }

    CV_OCL_RUN(dims <= 2 && dst.isUMat(),
               ocl_convertTo(*हे, dst, ddepth, noScale, alpha, beta))

    पूर्णांक cn = channels();
    पूर्णांक dtype = CV_MAKETYPE(ddepth, cn);

    Mat src = *हे;
    तार्किक allowTransposed = dims == 1 ||
        dst.kind() == आदान_Array_2::STD_VECTOR ||
        (dst.fixedSize() && dst.dims() == 1);
    dst.create( size, dtype, -1, allowTransposed );
    Mat dstMat = dst.getMat();

    जर( dims <= 2 )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, src.step, dstMat.data, dstMat.step, src.cols * cn, src.rows, sdepth, ddepth, alpha, beta);
    }
    अन्यथा जर( src.isContinuous() && dstMat.isContinuous() )
    {
        CALL_HAL(convertScale, cv_hal_convertScale, src.data, 0, dstMat.data, 0, (पूर्णांक)src.total() * cn, 1, sdepth, ddepth, alpha, beta);
    }

    द्विमान_Func func = noScale ? getConvertFunc(sdepth, ddepth) : getConvertScaleFunc(sdepth, ddepth);
    द्विगुण scale[] = {alpha, beta};
    CV_Assert( func != 0 );

    जर( dims <= 2 )
    {
        Size sz = getContinuousSize2D(src, dstMat, cn);
        func(src.data, src.step, 0, 0, dstMat.data, dstMat.step, sz, scale);
    }
    अन्यथा
    {
        स्थिरांक Mat* arrays[] = {&src, &dstMat, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        Size sz((पूर्णांक)(it.size*cn), 1);

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            func(ptrs[0], 1, 0, 0, ptrs[1], 1, sz, scale);
    }
}

शून्य UMat::convertTo(OutputArray dst, पूर्णांक type_, द्विगुण alpha, द्विगुण beta) स्थिरांक
{
    CV_INSTRUMENT_REGION();

    जर (empty())
    {
        dst.release();
        dst.create(size(), type_ >= 0 ? type_ : type());
        परत;
    }

#ifdef HAVE_OPENCL
    पूर्णांक stype = type();
    पूर्णांक sdepth = CV_MAT_DEPTH(stype);

    पूर्णांक ddepth = sdepth;
    जर (type_ >= 0)
        ddepth = CV_MAT_DEPTH(type_);
    अन्यथा
        ddepth = dst.fixedType() ? dst.depth() : sdepth;

    तार्किक noScale = std::भ_असल(alpha - 1) < द्विग_अंतर && std::भ_असल(beta) < द्विग_अंतर;
    जर (sdepth == ddepth && noScale)
    {
        copyTo(dst);
        परत;
    }

    CV_OCL_RUN(dims <= 2,
               ocl_convertTo(*हे, dst, ddepth, noScale, alpha, beta))
#endif // HAVE_OPENCL

    UMat src = *हे;  // Fake reference to itself.
                       // Resolves issue 8693 in case of src == dst.
    Mat m = getMat(ACCESS_READ);
    m.convertTo(dst, type_, alpha, beta);
    (शून्य)src;
}

} // namespace cv
