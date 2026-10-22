// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "has_non_zero.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "has_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ HasNonZeroFunc getHasNonZeroFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getHasNonZeroFunc, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ ocl_hasNonZero( ਨਿਵੇਸ਼_Array _src, ਤਾਰਕਿਕ & res )
{
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), kercn = ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src);
    ਤਾਰਕਿਕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ਜੇ (ਡੂੰਘਾਈ == CV_64F && !doubleSupport)
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ਸਮੂਹ_ਮਾਪ();

    ਪੂਰਨ wgs2_aligned = 1;
    ਜਦੋਂ (wgs2_aligned < (ਪੂਰਨ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), (ਪੂਰਨ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ਜੇ (k.ਲੜੀਵਾਰ(1, &globalsize, &wgs, ਸੱਚ))
        ਵਾਪਸ res = (saturate_cast<ਪੂਰਨ>(cv::ਜੋੜ(db.getMat(ACCESS_READ))[0])>0), ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
}
#ਜੇ_ਅੰਤ

ਤਾਰਕਿਕ hasNonZero(ਨਿਵੇਸ਼_Array _src)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

    ਤਾਰਕਿਕ res = ਝੂਠ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    CV_OCL_ਲੜੀਵਾਰ_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_hasNonZero(_src, res),
                res)
#ਜੇ_ਅੰਤ

    Mat src = _src.getMat();

    HasNonZeroFunc func = getHasNonZeroFunc(src.ਡੂੰਘਾਈ());
    CV_Assert( func != 0 );

    ਜੇ (src.dims == 2)//fast path to avoid creating planes of single rows
    {
        ਜੇ (src.isContinuous())
            res |= func(src.ptr<uchar>(0), src.ਕੁੱਲ());
        ਨਹੀਂਤਾਂ
            ਲਈ(ਪੂਰਨ ਪੰਕਤੀ = 0, ਪੰਕਤੀਆਂ_ਗਿਣਤੀ = src.ਪੰਕਤੀਆਂ ; !res && (ਪੰਕਤੀ<ਪੰਕਤੀਆਂ_ਗਿਣਤੀ) ; ++ਪੰਕਤੀ)
                res |= func(src.ptr<uchar>(ਪੰਕਤੀ), src.ਖਾਨੇ_ਗਿਣਤੀ);
    }
    ਨਹੀਂਤਾਂ//if (src.dims != 2)
    {
        ਅਚਲ Mat* arrays[] = {&src, ਖਾਲੀ_ਸੂਚਕ};
        Mat planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        ਲਈ(size_t p = 0 ; !res && (p<itNAry.nplanes) ; ++p, ++itNAry)
        {
            ਅਚਲ Mat& plane = itNAry.planes[0];
            ਜੇ (plane.isContinuous())
                res |= func(plane.ptr<uchar>(0), plane.ਕੁੱਲ());
            ਨਹੀਂਤਾਂ
              ਲਈ(ਪੂਰਨ ਪੰਕਤੀ = 0, ਪੰਕਤੀਆਂ_ਗਿਣਤੀ = plane.ਪੰਕਤੀਆਂ ; !res && (ਪੰਕਤੀ<ਪੰਕਤੀਆਂ_ਗਿਣਤੀ) ; ++ਪੰਕਤੀ)
                  res |= func(plane.ptr<uchar>(ਪੰਕਤੀ), plane.ਖਾਨੇ_ਗਿਣਤੀ);
        }
    }

    ਵਾਪਸ res;
}

} // namespace
