// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat_parallel.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/detail/dispatch_helper.impl.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>

#ਅਨ੍ਤਰ੍ਭਾਵ "minmax.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ MinMaxIdxFunc getMinMaxIdxFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (ਡੂੰਘਾਈ),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
ਸਥਿਰ ਖਾਲੀ ofs2idx(ਅਚਲ Mat& a, size_t ofs, ਪੂਰਨ* idx)
{
    ਪੂਰਨ i, d = a.dims;
    ਜੇ( ofs > 0 )
    {
        ofs--;
        ਲਈ( i = d-1; i >= 0; i-- )
        {
            ਪੂਰਨ sz = a.size[i];
            idx[i] = (ਪੂਰਨ)(ofs % sz);
            ofs /= sz;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਖਾਲੀ getMinMaxRes(ਅਚਲ Mat & db, ਦੁੱਗਣਾ * minVal, ਦੁੱਗਣਾ * maxVal,
                  ਪੂਰਨ* minLoc, ਪੂਰਨ* maxLoc,
                  ਪੂਰਨ groupnum, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਦੁੱਗਣਾ * maxVal2)
{
    uint ਸੂਚਕਾਂਕ_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = ਸੂਚਕਾਂਕ_max, maxloc = ਸੂਚਕਾਂਕ_max;

    size_t ਸੂਚਕਾਂਕ = 0;
    ਅਚਲ T * minptr = ਨਿਰਾ, * maxptr = ਨਿਰਾ, * maxptr2 = ਨਿਰਾ;
    ਅਚਲ uint * minlocptr = ਨਿਰਾ, * maxlocptr = ਨਿਰਾ;
    ਜੇ (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        ਸੂਚਕਾਂਕ += ਆਕਾਰ(T) * groupnum;
        ਸੂਚਕਾਂਕ = align_ਮਾਪ(ਸੂਚਕਾਂਕ, MINMAX_STRUCT_ALIGNMENT);
    }
    ਜੇ (maxVal || maxLoc)
    {
        maxptr = (ਅਚਲ T *)(db.ptr() + ਸੂਚਕਾਂਕ);
        ਸੂਚਕਾਂਕ += ਆਕਾਰ(T) * groupnum;
        ਸੂਚਕਾਂਕ = align_ਮਾਪ(ਸੂਚਕਾਂਕ, MINMAX_STRUCT_ALIGNMENT);
    }
    ਜੇ (minLoc)
    {
        minlocptr = (ਅਚਲ uint *)(db.ptr() + ਸੂਚਕਾਂਕ);
        ਸੂਚਕਾਂਕ += ਆਕਾਰ(uint) * groupnum;
        ਸੂਚਕਾਂਕ = align_ਮਾਪ(ਸੂਚਕਾਂਕ, MINMAX_STRUCT_ALIGNMENT);
    }
    ਜੇ (maxLoc)
    {
        maxlocptr = (ਅਚਲ uint *)(db.ptr() + ਸੂਚਕਾਂਕ);
        ਸੂਚਕਾਂਕ += ਆਕਾਰ(uint) * groupnum;
        ਸੂਚਕਾਂਕ = align_ਮਾਪ(ਸੂਚਕਾਂਕ, MINMAX_STRUCT_ALIGNMENT);
    }
    ਜੇ (maxVal2)
        maxptr2 = (ਅਚਲ T *)(db.ptr() + ਸੂਚਕਾਂਕ);

    ਲਈ (ਪੂਰਨ i = 0; i < groupnum; i++)
    {
        ਜੇ (minptr && minptr[i] <= minval)
        {
            ਜੇ (minptr[i] == minval)
            {
                ਜੇ (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        ਜੇ (maxptr && maxptr[i] >= maxval)
        {
            ਜੇ (maxptr[i] == maxval)
            {
                ਜੇ (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        ਜੇ (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    ਤਾਰਕਿਕ zero_ਨਕਾਬ = (minLoc && minloc == ਸੂਚਕਾਂਕ_max) ||
    (maxLoc && maxloc == ਸੂਚਕਾਂਕ_max);

    ਜੇ (minVal)
        *minVal = zero_ਨਕਾਬ ? 0 : (ਦੁੱਗਣਾ)minval;
    ਜੇ (maxVal)
        *maxVal = zero_ਨਕਾਬ ? 0 : (ਦੁੱਗਣਾ)maxval;
    ਜੇ (maxVal2)
        *maxVal2 = zero_ਨਕਾਬ ? 0 : (ਦੁੱਗਣਾ)maxval2;

    ਜੇ (minLoc)
    {
        minLoc[0] = zero_ਨਕਾਬ ? -1 : minloc / ਖਾਨੇ_ਗਿਣਤੀ;
        minLoc[1] = zero_ਨਕਾਬ ? -1 : minloc % ਖਾਨੇ_ਗਿਣਤੀ;
    }
    ਜੇ (maxLoc)
    {
        maxLoc[0] = zero_ਨਕਾਬ ? -1 : maxloc / ਖਾਨੇ_ਗਿਣਤੀ;
        maxLoc[1] = zero_ਨਕਾਬ ? -1 : maxloc % ਖਾਨੇ_ਗਿਣਤੀ;
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*getMinMaxResFunc)(ਅਚਲ Mat & db, ਦੁੱਗਣਾ * minVal, ਦੁੱਗਣਾ * maxVal,
                                 ਪੂਰਨ * minLoc, ਪੂਰਨ *maxLoc, ਪੂਰਨ gropunum, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਦੁੱਗਣਾ * maxVal2);

ਤਾਰਕਿਕ ocl_minMaxIdx( ਨਿਵੇਸ਼_Array _src, ਦੁੱਗਣਾ* minVal, ਦੁੱਗਣਾ* maxVal, ਪੂਰਨ* minLoc, ਪੂਰਨ* maxLoc, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2,
                   ਪੂਰਨ ddepth, ਤਾਰਕਿਕ abs_ਮੁੱਲਾਂ, ਨਿਵੇਸ਼_Array _src2, ਦੁੱਗਣਾ * maxVal2)
{
    ਅਚਲ ocl::Device & dev = ocl::Device::getDefault();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
    ਜੇ (dev.isNVidia())
        ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

    ਜੇ (dev.device_ਸੰਸਕਰਣ_Major() == 1 && dev.device_ਸੰਸਕਰਣ_Minor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ਵਾਪਸ ਝੂਠ;
    }

    ਤਾਰਕਿਕ doubleSupport = dev.doubleFPConfig() > 0, have_ਨਕਾਬ = !ਨਕਾਬ_2.empty(),
    haveSrc2 = _src2.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::NONE;
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type),
    kercn = have_ਨਕਾਬ ? cn : std::min(4, ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src, _src2));

    ਜੇ (ਡੂੰਘਾਈ >= CV_16F)
        ਵਾਪਸ ਝੂਠ;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    ਜੇ ((have_ਨਕਾਬ || type == CV_32FC1) && dev.isAMD())
        ਵਾਪਸ ਝੂਠ;

    CV_Assert( (cn == 1 && (!have_ਨਕਾਬ || ਨਕਾਬ_2.type() == CV_8U || ਨਕਾਬ_2.type() == CV_8S || ਨਕਾਬ_2.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    ਜੇ (ddepth < 0)
        ddepth = ਡੂੰਘਾਈ;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    ਜੇ (ਡੂੰਘਾਈ == CV_32S || ਡੂੰਘਾਈ == CV_8S || ਡੂੰਘਾਈ == CV_32U || ਡੂੰਘਾਈ == CV_64U ||
        ਡੂੰਘਾਈ == CV_64S || ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ ((ਡੂੰਘਾਈ == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_ਸਮੂਹ_ਮਾਪ();

    ਪੂਰਨ wgs2_aligned = 1;
    ਜਦੋਂ (wgs2_aligned < (ਪੂਰਨ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ਤਾਰਕਿਕ needMinVal = minVal || minLoc, needMinLoc = minLoc != ਨਿਰਾ,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != ਨਿਰਾ;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    ਜੇ (!(needMaxLoc || needMinLoc) && have_ਨਕਾਬ)
    {
        ਜੇ (needMinVal)
            needMinLoc = ਸੱਚ;
        ਨਹੀਂਤਾਂ
            needMaxLoc = ਸੱਚ;
    }

    ਅੱਖਰ cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         ਡੂੰਘਾਈ, ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), have_ਨਕਾਬ ? " -D HAVE_MASK" : "", (ਪੂਰਨ)wgs,
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         ਨਕਾਬ_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(ਡੂੰਘਾਈ, ddepth, kercn, cvt[0], ਆਕਾਰ(cvt[0])),
                         abs_ਮੁੱਲਾਂ ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         ਡੂੰਘਾਈ <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], ਆਕਾਰ(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ esz = CV_ELEM_ਮਾਪ(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), ਨਕਾਬ = ਨਕਾਬ_2.getUMat();

    ਜੇ (cn > 1 && !have_ਨਕਾਬ)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    ਜੇ (haveSrc2)
    {
        ਜੇ (!have_ਨਕਾਬ)
            k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ਮਾਪ(src2));
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ),
                   ocl::KernelArg::Read_Only_No_ਮਾਪ(src2));
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (!have_ਨਕਾਬ)
            k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ));
    }

    size_t globalsize = groupnum * wgs;
    ਜੇ (!k.ਲੜੀਵਾਰ(1, &globalsize, &wgs, ਸੱਚ))
        ਵਾਪਸ ਝੂਠ;

    ਸਥਿਰ ਅਚਲ getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<ਅੱਖਰ>,
        getMinMaxRes<ushort>,
        getMinMaxRes<ਛੋਟਾ>,
        getMinMaxRes<ਪੂਰਨ>,
        getMinMaxRes<ਦਸ਼ਮਲਵ>,
        getMinMaxRes<ਦੁੱਗਣਾ>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    ਪੂਰਨ loc_ਆਰਜ਼ੀ[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_ਆਰਜ਼ੀ : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_ਆਰਜ਼ੀ : maxLoc,
         groupnum, src.ਖਾਨੇ_ਗਿਣਤੀ, maxVal2);

    ਵਾਪਸ ਸੱਚ;
}

#ਜੇ_ਅੰਤ

}

// serial implementation, also used for the pieces of the parallel version
ਸਥਿਰ ਖਾਲੀ minMaxIdx_(cv::ਨਿਵੇਸ਼_Array _src, ਦੁੱਗਣਾ* minVal,
                       ਦੁੱਗਣਾ* maxVal, ਪੂਰਨ* minIdx, ਪੂਰਨ* maxIdx,
                       cv::ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (ਨਕਾਬ_2.empty() || ਨਕਾਬ_2.type() == CV_8U || ਨਕਾਬ_2.type() == CV_8S || ਨਕਾਬ_2.type() == CV_Bool)) ||
               (cn > 1 && ਨਕਾਬ_2.empty() && !minIdx && !maxIdx) );

    CV_OCL_ਲੜੀਵਾਰ(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (ਨਕਾਬ_2.empty() || _src.size() == ਨਕਾਬ_2.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, ਨਕਾਬ_2))

    Mat src = _src.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();

    ਜੇ (src.dims <= 2)
    {
        ਜੇ ((size_t)src.ਕਦਮ == (size_t)ਨਕਾਬ.ਕਦਮ || ਨਕਾਬ.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.ਅੰਕੜੇ, src.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ*cn, src.ਪੰਕਤੀਆਂ,
                     src.ਡੂੰਘਾਈ(), minVal, maxVal, minIdx, maxIdx, ਨਕਾਬ.ਅੰਕੜੇ);
        }
        CALL_HAL(min_Max_Idx_ਨਕਾਬ_ਕਦਮ, cv_hal_min_Max_Idx_ਨਕਾਬ_ਕਦਮ, src.ਅੰਕੜੇ, src.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ*cn, src.ਪੰਕਤੀਆਂ,
                 src.ਡੂੰਘਾਈ(), minVal, maxVal, minIdx, maxIdx, ਨਕਾਬ.ਅੰਕੜੇ, ਨਕਾਬ.ਕਦਮ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (src.isContinuous() && (ਨਕਾਬ.isContinuous() || ਨਕਾਬ.empty()))
    {
        ਪੂਰਨ res = cv_hal_minMaxIdx(src.ਅੰਕੜੇ, 0, (ਪੂਰਨ)src.ਕੁੱਲ()*cn, 1, src.ਡੂੰਘਾਈ(),
                                   minVal, maxVal, minIdx, maxIdx, ਨਕਾਬ.ਅੰਕੜੇ);

        ਜੇ (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            ਜੇ (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            ਜੇ (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ਵਾਪਸ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::Sts_ਅੰਦਰੂਨੀ,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(ਡੂੰਘਾਈ);
    CV_Assert( func != 0 );

    ਅਚਲ Mat* arrays[] = {&src, &ਨਕਾਬ, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    ਸੰਘ {
        ਪੂਰਨ i;
        ਦਸ਼ਮਲਵ f;
        ਦੁੱਗਣਾ d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    ਪੂਰਨ plane_ਮਾਪ = (ਪੂਰਨ)it.size*cn;
    minval.L = maxval.L = 0;

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_ਮਾਪ )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_ਮਾਪ, startidx );

    ਦੁੱਗਣਾ dminval, dmaxval;
    ਜੇ( ਡੂੰਘਾਈ <= CV_32S || ਡੂੰਘਾਈ == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_64S || ਡੂੰਘਾਈ == CV_32U )
        dminval = (ਦੁੱਗਣਾ)minval.L, dmaxval = (ਦੁੱਗਣਾ)maxval.L;
    ਨਹੀਂਤਾਂ {
        CV_Assert(ਡੂੰਘਾਈ == CV_64U);
        dminval = (ਦੁੱਗਣਾ)minval.UL, dmaxval = (ਦੁੱਗਣਾ)maxval.UL;
    }

    ਜੇ( minVal )
        *minVal = dminval;
    ਜੇ( maxVal )
        *maxVal = dmaxval;

    ਜੇ( minIdx )
        ofs2idx(src, minidx, minIdx);
    ਜੇ( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

ਖਾਲੀ cv::minMaxIdx(ਨਿਵੇਸ਼_Array _src, ਦੁੱਗਣਾ* minVal,
                   ਦੁੱਗਣਾ* maxVal, ਪੂਰਨ* minIdx, ਪੂਰਨ* maxIdx,
                   ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ਜੇ (!_src.isUMat())
    {
        Mat src = _src.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();
        ਅਚਲ ਪੂਰਨ cn = src.channels();
        ਅਚਲ ਤਾਰਕਿਕ ਦਲੀਲਾਂ_Ok = (cn == 1 && (ਨਕਾਬ.empty() || ((ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S ||
                             ਨਕਾਬ.type() == CV_Bool) && src.size == ਨਕਾਬ.size))) ||
                            (cn > 1 && ਨਕਾਬ.empty() && !minIdx && !maxIdx);
        ਜੇ (ਦਲੀਲਾਂ_Ok && ਵੰਡੋ_For_Parallel_Stat(src, ਨਕਾਬ, chunks))
        {
            ਅਚਲ ਪੂਰਨ n = (ਪੂਰਨ)chunks.size();
            std::vector<ਦੁੱਗਣਾ> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ਅਚਲ Range& r)
            {
                ਲਈ (ਪੂਰਨ i = r.ਸ਼ੁਰੂ; i < r.end; i++)
                {
                    ਪੂਰਨ a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : ਖਾਲੀ_ਸੂਚਕ, cn == 1 ? b : ਖਾਲੀ_ਸੂਚਕ,
                               chunks[i].ਨਕਾਬ);
                    ਜੇ (cn > 1)
                        imn[i] = imx[i] = 1;
                    ਨਹੀਂਤਾਂ ਜੇ (a[0] >= 0)
                    {
                        imn[i] = chunk_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Elem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            ਪੂਰਨ bmin = -1, bmax = -1;
            ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
            {
                ਜੇ (imn[i] == 0)
                    ਜਾਰੀ;
                ਜੇ (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                ਜੇ (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            ਜੇ (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            ਜੇ (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            ਜੇ (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            ਜੇ (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ਵਾਪਸ;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, ਨਕਾਬ_2);
}

ਖਾਲੀ cv::minMaxLoc( ਨਿਵੇਸ਼_Array _img, ਦੁੱਗਣਾ* minVal, ਦੁੱਗਣਾ* maxVal,
                    ਬਿੰਦੂ* minLoc, ਬਿੰਦੂ* maxLoc, ਨਿਵੇਸ਼_Array ਨਕਾਬ )
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (ਪੂਰਨ*)minLoc, (ਪੂਰਨ*)maxLoc, ਨਕਾਬ);
    ਜੇ( minLoc) {
        ਜੇ (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        ਨਹੀਂਤਾਂ {
            minLoc->y = 0;
        }
    }
    ਜੇ( maxLoc) {
        ਜੇ (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        ਨਹੀਂਤਾਂ {
            maxLoc->y = 0;
        }
    }
}

ਗਿਣਤੀ ਵਰਗ Reduce_ਬਹੁਲਕ
{
    ਪਹਿਲਾ_MIN = 0, //!< get index of first min occurrence
    ਆਖਰੀ_MIN  = 1, //!< get index of last min occurrence
    ਪਹਿਲਾ_MAX = 2, //!< get index of first max occurrence
    ਆਖਰੀ_MAX  = 3, //!< get index of last max occurrence
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ reduceMinMaxImpl
{
    ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ cv::Mat& src, cv::Mat& dst, Reduce_ਬਹੁਲਕ ਬਹੁਲਕ, ਅਚਲ ਪੂਰਨ axis) ਅਚਲ
    {
        ਚੋਣ(ਬਹੁਲਕ)
        {
        ਹਾਲਤ Reduce_ਬਹੁਲਕ::ਪਹਿਲਾ_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            ਰੋਕੋ;
        ਹਾਲਤ Reduce_ਬਹੁਲਕ::ਆਖਰੀ_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            ਰੋਕੋ;
        ਹਾਲਤ Reduce_ਬਹੁਲਕ::ਪਹਿਲਾ_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            ਰੋਕੋ;
        ਹਾਲਤ Reduce_ਬਹੁਲਕ::ਆਖਰੀ_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            ਰੋਕੋ;
        }
    }

    ਸਾਂਚਾ <ਸਾਂਚਾ<ਵਰਗ> ਵਰਗ Cmp>
    ਸਥਿਰ ਖਾਲੀ reduceMinMaxApply(ਅਚਲ cv::Mat& src, cv::Mat& dst, ਅਚਲ ਪੂਰਨ axis)
    {
        Cmp<T> cmp;

        ਅਚਲ ਸਵੈਚਾਲਿਤ *src_ptr = src.ptr<T>();
        ਸਵੈਚਾਲਿਤ *dst_ptr = dst.ptr<int32_t>();

        ਅਚਲ size_t outer_ਮਾਪ = src.ਕੁੱਲ(0, axis);
        ਅਚਲ ਸਵੈਚਾਲਿਤ mid_ਮਾਪ = static_cast<size_t>(src.size[axis]);

        ਅਚਲ size_t outer_ਕਦਮ = src.ਕੁੱਲ(axis);
        ਅਚਲ size_t dst_ਕਦਮ = dst.ਕੁੱਲ(axis);

        ਅਚਲ size_t mid_ਕਦਮ = src.ਕੁੱਲ(axis + 1);

        ਲਈ (size_t outer = 0; outer < outer_ਮਾਪ; ++outer)
        {
            ਅਚਲ size_t outer_ਵਿੱਥ = outer * outer_ਕਦਮ;
            ਅਚਲ size_t dst_ਵਿੱਥ = outer * dst_ਕਦਮ;
            ਲਈ (size_t mid = 0; mid != mid_ਮਾਪ; ++mid)
            {
                ਅਚਲ size_t src_ਵਿੱਥ = outer_ਵਿੱਥ + mid * mid_ਕਦਮ;
                ਲਈ (size_t inner = 0; inner < mid_ਕਦਮ; inner++)
                {
                    int32_t& ਸੂਚਕਾਂਕ = dst_ptr[dst_ਵਿੱਥ + inner];

                    ਅਚਲ size_t prev = outer_ਵਿੱਥ + ਸੂਚਕਾਂਕ * mid_ਕਦਮ + inner;
                    ਅਚਲ size_t curr = src_ਵਿੱਥ + inner;

                    ਜੇ (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        ਸੂਚਕਾਂਕ = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ਸਥਿਰ ਖਾਲੀ reduceMinMax(cv::ਨਿਵੇਸ਼_Array src, cv::OutputArray dst, Reduce_ਬਹੁਲਕ ਬਹੁਲਕ, ਪੂਰਨ axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    ਪੂਰਨ dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<ਪੂਰਨ> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    ਜੇ(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.ਅੰਕੜੇ(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.ਸੈੱਟ_ਮੰਜ਼ਿਲ(cv::Scalar::all(0));

    ਜੇ (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    ਤਾਰਕਿਕ needs_copy = !dstMat.isContinuous();
    ਜੇ (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::ਡੂੰਘਾਈ_Dispatch<reduceMinMaxImpl>(srcMat.ਡੂੰਘਾਈ(), srcMat, dstMat, ਬਹੁਲਕ, axis);

    ਜੇ (needs_copy)
    {
        dstMat.copy_ਮੰਜ਼ਿਲ(dst);
    }
}

ਖਾਲੀ cv::reduceArgMin(ਨਿਵੇਸ਼_Array src, OutputArray dst, ਪੂਰਨ axis, ਤਾਰਕਿਕ ਆਖਰੀ_ਸੂਚਕਾਂਕ)
{
    reduceMinMax(src, dst, ਆਖਰੀ_ਸੂਚਕਾਂਕ ? Reduce_ਬਹੁਲਕ::ਆਖਰੀ_MIN : Reduce_ਬਹੁਲਕ::ਪਹਿਲਾ_MIN, axis);
}

ਖਾਲੀ cv::reduceArgMax(ਨਿਵੇਸ਼_Array src, OutputArray dst, ਪੂਰਨ axis, ਤਾਰਕਿਕ ਆਖਰੀ_ਸੂਚਕਾਂਕ)
{
    reduceMinMax(src, dst, ਆਖਰੀ_ਸੂਚਕਾਂਕ ? Reduce_ਬਹੁਲਕ::ਆਖਰੀ_MAX : Reduce_ਬਹੁਲਕ::ਪਹਿਲਾ_MAX, axis);
}
