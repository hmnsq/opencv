// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"
#ಅನ್ತರ್ಭಾವ "stat_parallel.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/detail/dispatch_helper.impl.hpp"
#ಅನ್ತರ್ಭಾವ <algorithm>

#ಅನ್ತರ್ಭಾವ "minmax.simd.hpp"
#ಅನ್ತರ್ಭಾವ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ MinMaxIdxFunc getMinMaxIdxFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (ಆಳ),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
ಸ್ಥಿರ ಶೂನ್ಯ ofs2idx(ಸ್ಥಿರಾಂಕ Mat& a, size_t ofs, ಪೂರ್ಣಾಂಕ* idx)
{
    ಪೂರ್ಣಾಂಕ i, d = a.dims;
    ಆದರೆ( ofs > 0 )
    {
        ofs--;
        ಗಾಗಿ( i = d-1; i >= 0; i-- )
        {
            ಪೂರ್ಣಾಂಕ sz = a.size[i];
            idx[i] = (ಪೂರ್ಣಾಂಕ)(ofs % sz);
            ofs /= sz;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

#ವ್ಯಾಖ್ಯಾನಿಸು MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ಶೂನ್ಯ getMinMaxRes(ಸ್ಥಿರಾಂಕ Mat & db, ದ್ವಿಗುಣ * minVal, ದ್ವಿಗುಣ * maxVal,
                  ಪೂರ್ಣಾಂಕ* minLoc, ಪೂರ್ಣಾಂಕ* maxLoc,
                  ಪೂರ್ಣಾಂಕ groupnum, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ದ್ವಿಗುಣ * maxVal2)
{
    uint ಸೂಚ್ಯಂಕ_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = ಸೂಚ್ಯಂಕ_max, maxloc = ಸೂಚ್ಯಂಕ_max;

    size_t ಸೂಚ್ಯಂಕ = 0;
    ಸ್ಥಿರಾಂಕ T * minptr = ಖಾಲಿ, * maxptr = ಖಾಲಿ, * maxptr2 = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ uint * minlocptr = ಖಾಲಿ, * maxlocptr = ಖಾಲಿ;
    ಆದರೆ (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        ಸೂಚ್ಯಂಕ += ಗಾತ್ರ(T) * groupnum;
        ಸೂಚ್ಯಂಕ = align_ಗಾತ್ರ(ಸೂಚ್ಯಂಕ, MINMAX_STRUCT_ALIGNMENT);
    }
    ಆದರೆ (maxVal || maxLoc)
    {
        maxptr = (ಸ್ಥಿರಾಂಕ T *)(db.ptr() + ಸೂಚ್ಯಂಕ);
        ಸೂಚ್ಯಂಕ += ಗಾತ್ರ(T) * groupnum;
        ಸೂಚ್ಯಂಕ = align_ಗಾತ್ರ(ಸೂಚ್ಯಂಕ, MINMAX_STRUCT_ALIGNMENT);
    }
    ಆದರೆ (minLoc)
    {
        minlocptr = (ಸ್ಥಿರಾಂಕ uint *)(db.ptr() + ಸೂಚ್ಯಂಕ);
        ಸೂಚ್ಯಂಕ += ಗಾತ್ರ(uint) * groupnum;
        ಸೂಚ್ಯಂಕ = align_ಗಾತ್ರ(ಸೂಚ್ಯಂಕ, MINMAX_STRUCT_ALIGNMENT);
    }
    ಆದರೆ (maxLoc)
    {
        maxlocptr = (ಸ್ಥಿರಾಂಕ uint *)(db.ptr() + ಸೂಚ್ಯಂಕ);
        ಸೂಚ್ಯಂಕ += ಗಾತ್ರ(uint) * groupnum;
        ಸೂಚ್ಯಂಕ = align_ಗಾತ್ರ(ಸೂಚ್ಯಂಕ, MINMAX_STRUCT_ALIGNMENT);
    }
    ಆದರೆ (maxVal2)
        maxptr2 = (ಸ್ಥಿರಾಂಕ T *)(db.ptr() + ಸೂಚ್ಯಂಕ);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < groupnum; i++)
    {
        ಆದರೆ (minptr && minptr[i] <= minval)
        {
            ಆದರೆ (minptr[i] == minval)
            {
                ಆದರೆ (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        ಆದರೆ (maxptr && maxptr[i] >= maxval)
        {
            ಆದರೆ (maxptr[i] == maxval)
            {
                ಆದರೆ (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        ಆದರೆ (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    ತಾರ್ಕಿಕ zero_ಮುಖವಾಡ = (minLoc && minloc == ಸೂಚ್ಯಂಕ_max) ||
    (maxLoc && maxloc == ಸೂಚ್ಯಂಕ_max);

    ಆದರೆ (minVal)
        *minVal = zero_ಮುಖವಾಡ ? 0 : (ದ್ವಿಗುಣ)minval;
    ಆದರೆ (maxVal)
        *maxVal = zero_ಮುಖವಾಡ ? 0 : (ದ್ವಿಗುಣ)maxval;
    ಆದರೆ (maxVal2)
        *maxVal2 = zero_ಮುಖವಾಡ ? 0 : (ದ್ವಿಗುಣ)maxval2;

    ಆದರೆ (minLoc)
    {
        minLoc[0] = zero_ಮುಖವಾಡ ? -1 : minloc / ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        minLoc[1] = zero_ಮುಖವಾಡ ? -1 : minloc % ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    }
    ಆದರೆ (maxLoc)
    {
        maxLoc[0] = zero_ಮುಖವಾಡ ? -1 : maxloc / ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        maxLoc[1] = zero_ಮುಖವಾಡ ? -1 : maxloc % ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*getMinMaxResFunc)(ಸ್ಥಿರಾಂಕ Mat & db, ದ್ವಿಗುಣ * minVal, ದ್ವಿಗುಣ * maxVal,
                                 ಪೂರ್ಣಾಂಕ * minLoc, ಪೂರ್ಣಾಂಕ *maxLoc, ಪೂರ್ಣಾಂಕ gropunum, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ದ್ವಿಗುಣ * maxVal2);

ತಾರ್ಕಿಕ ocl_minMaxIdx( ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ* minVal, ದ್ವಿಗುಣ* maxVal, ಪೂರ್ಣಾಂಕ* minLoc, ಪೂರ್ಣಾಂಕ* maxLoc, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2,
                   ಪೂರ್ಣಾಂಕ ddepth, ತಾರ್ಕಿಕ abs_ಮೌಲ್ಯಗಳು, ಒಳಹರಿವು_Array _src2, ದ್ವಿಗುಣ * maxVal2)
{
    ಸ್ಥಿರಾಂಕ ocl::Device & dev = ocl::Device::getDefault();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
    ಆದರೆ (dev.isNVidia())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (dev.device_ಆವೃತ್ತಿ_Major() == 1 && dev.device_ಆವೃತ್ತಿ_Minor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }

    ತಾರ್ಕಿಕ doubleSupport = dev.doubleFPConfig() > 0, have_ಮುಖವಾಡ = !ಮುಖವಾಡ_2.empty(),
    haveSrc2 = _src2.ಬಗೆ() != ಒಳಹರಿವು_Array_2::NONE;
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type),
    kercn = have_ಮುಖವಾಡ ? cn : std::min(4, ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src, _src2));

    ಆದರೆ (ಆಳ >= CV_16F)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    ಆದರೆ ((have_ಮುಖವಾಡ || type == CV_32FC1) && dev.isAMD())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    CV_Assert( (cn == 1 && (!have_ಮುಖವಾಡ || ಮುಖವಾಡ_2.type() == CV_8U || ಮುಖವಾಡ_2.type() == CV_8S || ಮುಖವಾಡ_2.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    ಆದರೆ (ddepth < 0)
        ddepth = ಆಳ;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    ಆದರೆ (ಆಳ == CV_32S || ಆಳ == CV_8S || ಆಳ == CV_32U || ಆಳ == CV_64U ||
        ಆಳ == CV_64S || ಆಳ == CV_16F || ಆಳ == CV_16BF)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ ((ಆಳ == CV_64F || ddepth == CV_64F) && !doubleSupport)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_ಗುಂಪು_ಗಾತ್ರ();

    ಪೂರ್ಣಾಂಕ wgs2_aligned = 1;
    ಆಗ (wgs2_aligned < (ಪೂರ್ಣಾಂಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ತಾರ್ಕಿಕ needMinVal = minVal || minLoc, needMinLoc = minLoc != ಖಾಲಿ,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != ಖಾಲಿ;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    ಆದರೆ (!(needMaxLoc || needMinLoc) && have_ಮುಖವಾಡ)
    {
        ಆದರೆ (needMinVal)
            needMinLoc = ನಿಜ;
        ಇಲ್ಲದಿದ್ದರೆ
            needMaxLoc = ನಿಜ;
    }

    ಅಕ್ಷರ cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         ಆಳ, ocl::type_ಗಮ್ಯ_Str(ಆಳ), have_ಮುಖವಾಡ ? " -D HAVE_MASK" : "", (ಪೂರ್ಣಾಂಕ)wgs,
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         ಮುಖವಾಡ_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::type_ಗಮ್ಯ_Str(ddepth), ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(ಆಳ, ddepth, kercn, cvt[0], ಗಾತ್ರ(cvt[0])),
                         abs_ಮೌಲ್ಯಗಳು ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         ಆಳ <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], ಗಾತ್ರ(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ esz = CV_ELEM_ಗಾತ್ರ(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getUMat();

    ಆದರೆ (cn > 1 && !have_ಮುಖವಾಡ)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    ಆದರೆ (haveSrc2)
    {
        ಆದರೆ (!have_ಮುಖವಾಡ)
            k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2));
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ),
                   ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2));
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (!have_ಮುಖವಾಡ)
            k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ));
    }

    size_t globalsize = groupnum * wgs;
    ಆದರೆ (!k.ಸತತ(1, &globalsize, &wgs, ನಿಜ))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<ಅಕ್ಷರ>,
        getMinMaxRes<ushort>,
        getMinMaxRes<ಗಿಡ್ಡ>,
        getMinMaxRes<ಪೂರ್ಣಾಂಕ>,
        getMinMaxRes<ದಶಮಾಂಶ>,
        getMinMaxRes<ದ್ವಿಗುಣ>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    ಪೂರ್ಣಾಂಕ loc_ತಾತ್ಕಾಲಿಕ[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_ತಾತ್ಕಾಲಿಕ : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_ತಾತ್ಕಾಲಿಕ : maxLoc,
         groupnum, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, maxVal2);

    ಹಿಂತಿರುಗಿ ನಿಜ;
}

#ಆದರೆ_ಅಂತ್ಯ

}

// serial implementation, also used for the pieces of the parallel version
ಸ್ಥಿರ ಶೂನ್ಯ minMaxIdx_(cv::ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ* minVal,
                       ದ್ವಿಗುಣ* maxVal, ಪೂರ್ಣಾಂಕ* minIdx, ಪೂರ್ಣಾಂಕ* maxIdx,
                       cv::ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    ಬಳಸಿ ನಾಮಾಕಾಶ cv;
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (ಮುಖವಾಡ_2.empty() || ಮುಖವಾಡ_2.type() == CV_8U || ಮುಖವಾಡ_2.type() == CV_8S || ಮುಖವಾಡ_2.type() == CV_Bool)) ||
               (cn > 1 && ಮುಖವಾಡ_2.empty() && !minIdx && !maxIdx) );

    CV_OCL_ಸತತ(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (ಮುಖವಾಡ_2.empty() || _src.size() == ಮುಖವಾಡ_2.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, ಮುಖವಾಡ_2))

    Mat src = _src.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();

    ಆದರೆ (src.dims <= 2)
    {
        ಆದರೆ ((size_t)src.ಹೆಜ್ಜೆ == (size_t)ಮುಖವಾಡ.ಹೆಜ್ಜೆ || ಮುಖವಾಡ.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*cn, src.ಅಡ್ಡಸಾಲುಗಳು,
                     src.ಆಳ(), minVal, maxVal, minIdx, maxIdx, ಮುಖವಾಡ.ದತ್ತಾಂಶ);
        }
        CALL_HAL(min_Max_Idx_ಮುಖವಾಡ_ಹೆಜ್ಜೆ, cv_hal_min_Max_Idx_ಮುಖವಾಡ_ಹೆಜ್ಜೆ, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*cn, src.ಅಡ್ಡಸಾಲುಗಳು,
                 src.ಆಳ(), minVal, maxVal, minIdx, maxIdx, ಮುಖವಾಡ.ದತ್ತಾಂಶ, ಮುಖವಾಡ.ಹೆಜ್ಜೆ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (src.isContinuous() && (ಮುಖವಾಡ.isContinuous() || ಮುಖವಾಡ.empty()))
    {
        ಪೂರ್ಣಾಂಕ res = cv_hal_minMaxIdx(src.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು()*cn, 1, src.ಆಳ(),
                                   minVal, maxVal, minIdx, maxIdx, ಮುಖವಾಡ.ದತ್ತಾಂಶ);

        ಆದರೆ (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            ಆದರೆ (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            ಆದರೆ (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            ಹಿಂತಿರುಗಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::Sts_ಆಂತರಿಕ,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(ಆಳ);
    CV_Assert( func != 0 );

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &ಮುಖವಾಡ, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    ಒಕ್ಕೂಟ {
        ಪೂರ್ಣಾಂಕ i;
        ದಶಮಾಂಶ f;
        ದ್ವಿಗುಣ d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    ಪೂರ್ಣಾಂಕ plane_ಗಾತ್ರ = (ಪೂರ್ಣಾಂಕ)it.size*cn;
    minval.L = maxval.L = 0;

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_ಗಾತ್ರ )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_ಗಾತ್ರ, startidx );

    ದ್ವಿಗುಣ dminval, dmaxval;
    ಆದರೆ( ಆಳ <= CV_32S || ಆಳ == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_32F || ಆಳ == CV_16F || ಆಳ == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_64S || ಆಳ == CV_32U )
        dminval = (ದ್ವಿಗುಣ)minval.L, dmaxval = (ದ್ವಿಗುಣ)maxval.L;
    ಇಲ್ಲದಿದ್ದರೆ {
        CV_Assert(ಆಳ == CV_64U);
        dminval = (ದ್ವಿಗುಣ)minval.UL, dmaxval = (ದ್ವಿಗುಣ)maxval.UL;
    }

    ಆದರೆ( minVal )
        *minVal = dminval;
    ಆದರೆ( maxVal )
        *maxVal = dmaxval;

    ಆದರೆ( minIdx )
        ofs2idx(src, minidx, minIdx);
    ಆದರೆ( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

ಶೂನ್ಯ cv::minMaxIdx(ಒಳಹರಿವು_Array _src, ದ್ವಿಗುಣ* minVal,
                   ದ್ವಿಗುಣ* maxVal, ಪೂರ್ಣಾಂಕ* minIdx, ಪೂರ್ಣಾಂಕ* maxIdx,
                   ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ಆದರೆ (!_src.isUMat())
    {
        Mat src = _src.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = src.channels();
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ವಾದಗಳು_Ok = (cn == 1 && (ಮುಖವಾಡ.empty() || ((ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S ||
                             ಮುಖವಾಡ.type() == CV_Bool) && src.size == ಮುಖವಾಡ.size))) ||
                            (cn > 1 && ಮುಖವಾಡ.empty() && !minIdx && !maxIdx);
        ಆದರೆ (ವಾದಗಳು_Ok && ವಿಭಜಿಸು_For_Parallel_Stat(src, ಮುಖವಾಡ, chunks))
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = (ಪೂರ್ಣಾಂಕ)chunks.size();
            std::vector<ದ್ವಿಗುಣ> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ಸ್ಥಿರಾಂಕ Range& r)
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = r.ಆರಂಭ; i < r.end; i++)
                {
                    ಪೂರ್ಣಾಂಕ a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : ಶೂನ್ಯಸೂಚಕ, cn == 1 ? b : ಶೂನ್ಯಸೂಚಕ,
                               chunks[i].ಮುಖವಾಡ);
                    ಆದರೆ (cn > 1)
                        imn[i] = imx[i] = 1;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a[0] >= 0)
                    {
                        imn[i] = chunk_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Elem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            ಪೂರ್ಣಾಂಕ bmin = -1, bmax = -1;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
            {
                ಆದರೆ (imn[i] == 0)
                    ಮುಂದುವರಿ;
                ಆದರೆ (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                ಆದರೆ (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            ಆದರೆ (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            ಆದರೆ (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            ಆದರೆ (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            ಆದರೆ (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            ಹಿಂತಿರುಗಿ;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, ಮುಖವಾಡ_2);
}

ಶೂನ್ಯ cv::minMaxLoc( ಒಳಹರಿವು_Array _img, ದ್ವಿಗುಣ* minVal, ದ್ವಿಗುಣ* maxVal,
                    ಬಿಂದು* minLoc, ಬಿಂದು* maxLoc, ಒಳಹರಿವು_Array ಮುಖವಾಡ )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (ಪೂರ್ಣಾಂಕ*)minLoc, (ಪೂರ್ಣಾಂಕ*)maxLoc, ಮುಖವಾಡ);
    ಆದರೆ( minLoc) {
        ಆದರೆ (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        ಇಲ್ಲದಿದ್ದರೆ {
            minLoc->y = 0;
        }
    }
    ಆದರೆ( maxLoc) {
        ಆದರೆ (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        ಇಲ್ಲದಿದ್ದರೆ {
            maxLoc->y = 0;
        }
    }
}

ಎಣಿಕೆ ವರ್ಗ Reduce_ಬಹುಲಕ
{
    ಮೊದಲ_MIN = 0, //!< get index of first min occurrence
    ಕೊನೆಯ_MIN  = 1, //!< get index of last min occurrence
    ಮೊದಲ_MAX = 2, //!< get index of first max occurrence
    ಕೊನೆಯ_MAX  = 3, //!< get index of last max occurrence
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
ರಚನೆ reduceMinMaxImpl
{
    ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ cv::Mat& src, cv::Mat& dst, Reduce_ಬಹುಲಕ ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ axis) ಸ್ಥಿರಾಂಕ
    {
        ಆಯ್ಕೆ(ಬಹುಲಕ)
        {
        ಸ್ಥಿತಿ Reduce_ಬಹುಲಕ::ಮೊದಲ_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ Reduce_ಬಹುಲಕ::ಕೊನೆಯ_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ Reduce_ಬಹುಲಕ::ಮೊದಲ_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ Reduce_ಬಹುಲಕ::ಕೊನೆಯ_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            ನಿಲ್ಲಿಸು;
        }
    }

    ಮಾದರಿ <ಮಾದರಿ<ವರ್ಗ> ವರ್ಗ Cmp>
    ಸ್ಥಿರ ಶೂನ್ಯ reduceMinMaxApply(ಸ್ಥಿರಾಂಕ cv::Mat& src, cv::Mat& dst, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ axis)
    {
        Cmp<T> cmp;

        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ *src_ptr = src.ptr<T>();
        ಸ್ವಯಂಚಾಲಿತ *dst_ptr = dst.ptr<int32_t>();

        ಸ್ಥಿರಾಂಕ size_t outer_ಗಾತ್ರ = src.ಒಟ್ಟು(0, axis);
        ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ mid_ಗಾತ್ರ = static_cast<size_t>(src.size[axis]);

        ಸ್ಥಿರಾಂಕ size_t outer_ಹೆಜ್ಜೆ = src.ಒಟ್ಟು(axis);
        ಸ್ಥಿರಾಂಕ size_t dst_ಹೆಜ್ಜೆ = dst.ಒಟ್ಟು(axis);

        ಸ್ಥಿರಾಂಕ size_t mid_ಹೆಜ್ಜೆ = src.ಒಟ್ಟು(axis + 1);

        ಗಾಗಿ (size_t outer = 0; outer < outer_ಗಾತ್ರ; ++outer)
        {
            ಸ್ಥಿರಾಂಕ size_t outer_ಸ್ಥಾನಾಂತರ = outer * outer_ಹೆಜ್ಜೆ;
            ಸ್ಥಿರಾಂಕ size_t dst_ಸ್ಥಾನಾಂತರ = outer * dst_ಹೆಜ್ಜೆ;
            ಗಾಗಿ (size_t mid = 0; mid != mid_ಗಾತ್ರ; ++mid)
            {
                ಸ್ಥಿರಾಂಕ size_t src_ಸ್ಥಾನಾಂತರ = outer_ಸ್ಥಾನಾಂತರ + mid * mid_ಹೆಜ್ಜೆ;
                ಗಾಗಿ (size_t inner = 0; inner < mid_ಹೆಜ್ಜೆ; inner++)
                {
                    int32_t& ಸೂಚ್ಯಂಕ = dst_ptr[dst_ಸ್ಥಾನಾಂತರ + inner];

                    ಸ್ಥಿರಾಂಕ size_t prev = outer_ಸ್ಥಾನಾಂತರ + ಸೂಚ್ಯಂಕ * mid_ಹೆಜ್ಜೆ + inner;
                    ಸ್ಥಿರಾಂಕ size_t curr = src_ಸ್ಥಾನಾಂತರ + inner;

                    ಆದರೆ (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        ಸೂಚ್ಯಂಕ = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ಸ್ಥಿರ ಶೂನ್ಯ reduceMinMax(cv::ಒಳಹರಿವು_Array src, cv::OutputArray dst, Reduce_ಬಹುಲಕ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    ಪೂರ್ಣಾಂಕ dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<ಪೂರ್ಣಾಂಕ> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    ಆದರೆ(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.ದತ್ತಾಂಶ(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.ಗಣ_ಗಮ್ಯ(cv::Scalar::all(0));

    ಆದರೆ (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    ತಾರ್ಕಿಕ needs_copy = !dstMat.isContinuous();
    ಆದರೆ (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::ಆಳ_Dispatch<reduceMinMaxImpl>(srcMat.ಆಳ(), srcMat, dstMat, ಬಹುಲಕ, axis);

    ಆದರೆ (needs_copy)
    {
        dstMat.copy_ಗಮ್ಯ(dst);
    }
}

ಶೂನ್ಯ cv::reduceArgMin(ಒಳಹರಿವು_Array src, OutputArray dst, ಪೂರ್ಣಾಂಕ axis, ತಾರ್ಕಿಕ ಕೊನೆಯ_ಸೂಚ್ಯಂಕ)
{
    reduceMinMax(src, dst, ಕೊನೆಯ_ಸೂಚ್ಯಂಕ ? Reduce_ಬಹುಲಕ::ಕೊನೆಯ_MIN : Reduce_ಬಹುಲಕ::ಮೊದಲ_MIN, axis);
}

ಶೂನ್ಯ cv::reduceArgMax(ಒಳಹರಿವು_Array src, OutputArray dst, ಪೂರ್ಣಾಂಕ axis, ತಾರ್ಕಿಕ ಕೊನೆಯ_ಸೂಚ್ಯಂಕ)
{
    reduceMinMax(src, dst, ಕೊನೆಯ_ಸೂಚ್ಯಂಕ ? Reduce_ಬಹುಲಕ::ಕೊನೆಯ_MAX : Reduce_ಬಹುಲಕ::ಮೊದಲ_MAX, axis);
}
