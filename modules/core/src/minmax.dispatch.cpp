// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "stat.hpp"
#అన్తర్భావ "stat_parallel.hpp"
#అన్తర్భావ "opencv2/core/detail/dispatch_helper.impl.hpp"
#అన్తర్భావ <algorithm>

#అన్తర్భావ "minmax.simd.hpp"
#అన్తర్భావ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv {

స్థిర MinMaxIdxFunc getMinMaxIdxFunc(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (లోతు),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
స్థిర శూన్య ofs2idx(స్థిరరాశి Mat& a, size_t ofs, పూర్ణాంకం* idx)
{
    పూర్ణాంకం i, d = a.dims;
    ఒకవేళ( ofs > 0 )
    {
        ofs--;
        కొరకు( i = d-1; i >= 0; i-- )
        {
            పూర్ణాంకం sz = a.size[i];
            idx[i] = (పూర్ణాంకం)(ofs % sz);
            ofs /= sz;
        }
    }
    లేకపోతే
    {
        కొరకు( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#నిర్వచితమైతే HAVE_OPENCL

#నిర్వచించు MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

మూస <రకం_పేరు T>
శూన్య getMinMaxRes(స్థిరరాశి Mat & db, ద్విగుణ * minVal, ద్విగుణ * maxVal,
                  పూర్ణాంకం* minLoc, పూర్ణాంకం* maxLoc,
                  పూర్ణాంకం groupnum, పూర్ణాంకం నిలువుల_సంఖ్య, ద్విగుణ * maxVal2)
{
    uint సూచీ_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = సూచీ_max, maxloc = సూచీ_max;

    size_t సూచీ = 0;
    స్థిరరాశి T * minptr = ఖాళీ, * maxptr = ఖాళీ, * maxptr2 = ఖాళీ;
    స్థిరరాశి uint * minlocptr = ఖాళీ, * maxlocptr = ఖాళీ;
    ఒకవేళ (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        సూచీ += పరిమాణం(T) * groupnum;
        సూచీ = align_కొలత(సూచీ, MINMAX_STRUCT_ALIGNMENT);
    }
    ఒకవేళ (maxVal || maxLoc)
    {
        maxptr = (స్థిరరాశి T *)(db.ptr() + సూచీ);
        సూచీ += పరిమాణం(T) * groupnum;
        సూచీ = align_కొలత(సూచీ, MINMAX_STRUCT_ALIGNMENT);
    }
    ఒకవేళ (minLoc)
    {
        minlocptr = (స్థిరరాశి uint *)(db.ptr() + సూచీ);
        సూచీ += పరిమాణం(uint) * groupnum;
        సూచీ = align_కొలత(సూచీ, MINMAX_STRUCT_ALIGNMENT);
    }
    ఒకవేళ (maxLoc)
    {
        maxlocptr = (స్థిరరాశి uint *)(db.ptr() + సూచీ);
        సూచీ += పరిమాణం(uint) * groupnum;
        సూచీ = align_కొలత(సూచీ, MINMAX_STRUCT_ALIGNMENT);
    }
    ఒకవేళ (maxVal2)
        maxptr2 = (స్థిరరాశి T *)(db.ptr() + సూచీ);

    కొరకు (పూర్ణాంకం i = 0; i < groupnum; i++)
    {
        ఒకవేళ (minptr && minptr[i] <= minval)
        {
            ఒకవేళ (minptr[i] == minval)
            {
                ఒకవేళ (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            లేకపోతే
            {
                ఒకవేళ (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        ఒకవేళ (maxptr && maxptr[i] >= maxval)
        {
            ఒకవేళ (maxptr[i] == maxval)
            {
                ఒకవేళ (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            లేకపోతే
            {
                ఒకవేళ (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        ఒకవేళ (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    తార్కిక zero_ముసుగు = (minLoc && minloc == సూచీ_max) ||
    (maxLoc && maxloc == సూచీ_max);

    ఒకవేళ (minVal)
        *minVal = zero_ముసుగు ? 0 : (ద్విగుణ)minval;
    ఒకవేళ (maxVal)
        *maxVal = zero_ముసుగు ? 0 : (ద్విగుణ)maxval;
    ఒకవేళ (maxVal2)
        *maxVal2 = zero_ముసుగు ? 0 : (ద్విగుణ)maxval2;

    ఒకవేళ (minLoc)
    {
        minLoc[0] = zero_ముసుగు ? -1 : minloc / నిలువుల_సంఖ్య;
        minLoc[1] = zero_ముసుగు ? -1 : minloc % నిలువుల_సంఖ్య;
    }
    ఒకవేళ (maxLoc)
    {
        maxLoc[0] = zero_ముసుగు ? -1 : maxloc / నిలువుల_సంఖ్య;
        maxLoc[1] = zero_ముసుగు ? -1 : maxloc % నిలువుల_సంఖ్య;
    }
}

రకనిర్వచనం శూన్య (*getMinMaxResFunc)(స్థిరరాశి Mat & db, ద్విగుణ * minVal, ద్విగుణ * maxVal,
                                 పూర్ణాంకం * minLoc, పూర్ణాంకం *maxLoc, పూర్ణాంకం gropunum, పూర్ణాంకం నిలువుల_సంఖ్య, ద్విగుణ * maxVal2);

తార్కిక ocl_minMaxIdx( ఆదానం_Array _src, ద్విగుణ* minVal, ద్విగుణ* maxVal, పూర్ణాంకం* minLoc, పూర్ణాంకం* maxLoc, ఆదానం_Array ముసుగు_2,
                   పూర్ణాంకం ddepth, తార్కిక abs_విలువలు, ఆదానం_Array _src2, ద్విగుణ * maxVal2)
{
    స్థిరరాశి ocl::Device & dev = ocl::Device::getDefault();

#నిర్వచితమైతే __ANDROID__
    ఒకవేళ (dev.isNVidia())
        తిరిగి అబద్ధం;
#అయితే_ముగింపు

    ఒకవేళ (dev.device_సంస్కరణ_Major() == 1 && dev.device_సంస్కరణ_Minor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        తిరిగి అబద్ధం;
    }

    తార్కిక doubleSupport = dev.doubleFPConfig() > 0, have_ముసుగు = !ముసుగు_2.empty(),
    haveSrc2 = _src2.రకం() != ఆదానం_Array_2::NONE;
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type),
    kercn = have_ముసుగు ? cn : std::min(4, ocl::predict_Optimal_సదిశ_వెడల్పు(_src, _src2));

    ఒకవేళ (లోతు >= CV_16F)
        తిరిగి అబద్ధం;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    ఒకవేళ ((have_ముసుగు || type == CV_32FC1) && dev.isAMD())
        తిరిగి అబద్ధం;

    CV_Assert( (cn == 1 && (!have_ముసుగు || ముసుగు_2.type() == CV_8U || ముసుగు_2.type() == CV_8S || ముసుగు_2.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    ఒకవేళ (ddepth < 0)
        ddepth = లోతు;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    ఒకవేళ (లోతు == CV_32S || లోతు == CV_8S || లోతు == CV_32U || లోతు == CV_64U ||
        లోతు == CV_64S || లోతు == CV_16F || లోతు == CV_16BF)
        తిరిగి అబద్ధం;

    ఒకవేళ ((లోతు == CV_64F || ddepth == CV_64F) && !doubleSupport)
        తిరిగి అబద్ధం;

    పూర్ణాంకం groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_సమూహం_కొలత();

    పూర్ణాంకం wgs2_aligned = 1;
    అయితే (wgs2_aligned < (పూర్ణాంకం)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    తార్కిక needMinVal = minVal || minLoc, needMinLoc = minLoc != ఖాళీ,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != ఖాళీ;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    ఒకవేళ (!(needMaxLoc || needMinLoc) && have_ముసుగు)
    {
        ఒకవేళ (needMinVal)
            needMinLoc = నిజం;
        లేకపోతే
            needMaxLoc = నిజం;
    }

    అక్షరం cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         లోతు, ocl::type_గమ్యం_Str(లోతు), have_ముసుగు ? " -D HAVE_MASK" : "", (పూర్ణాంకం)wgs,
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         ముసుగు_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::type_గమ్యం_Str(ddepth), ocl::type_గమ్యం_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(లోతు, ddepth, kercn, cvt[0], పరిమాణం(cvt[0])),
                         abs_విలువలు ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         లోతు <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], పరిమాణం(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    పూర్ణాంకం esz = CV_ELEM_కొలత(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), ముసుగు = ముసుగు_2.getUMat();

    ఒకవేళ (cn > 1 && !have_ముసుగు)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    ఒకవేళ (haveSrc2)
    {
        ఒకవేళ (!have_ముసుగు)
            k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_కొలత(src2));
        లేకపోతే
            k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_కొలత(ముసుగు),
                   ocl::KernelArg::Read_Only_No_కొలత(src2));
    }
    లేకపోతే
    {
        ఒకవేళ (!have_ముసుగు)
            k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        లేకపోతే
            k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_కొలత(ముసుగు));
    }

    size_t globalsize = groupnum * wgs;
    ఒకవేళ (!k.నిరంతరం(1, &globalsize, &wgs, నిజం))
        తిరిగి అబద్ధం;

    స్థిర స్థిరరాశి getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<అక్షరం>,
        getMinMaxRes<ushort>,
        getMinMaxRes<పొట్టి>,
        getMinMaxRes<పూర్ణాంకం>,
        getMinMaxRes<దశాంశం>,
        getMinMaxRes<ద్విగుణ>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    పూర్ణాంకం loc_తాత్కాలిక[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_తాత్కాలిక : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_తాత్కాలిక : maxLoc,
         groupnum, src.నిలువుల_సంఖ్య, maxVal2);

    తిరిగి నిజం;
}

#అయితే_ముగింపు

}

// serial implementation, also used for the pieces of the parallel version
స్థిర శూన్య minMaxIdx_(cv::ఆదానం_Array _src, ద్విగుణ* minVal,
                       ద్విగుణ* maxVal, పూర్ణాంకం* minIdx, పూర్ణాంకం* maxIdx,
                       cv::ఆదానం_Array ముసుగు_2)
{
    ఉపయోగించి నామాకాశ cv;
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (ముసుగు_2.empty() || ముసుగు_2.type() == CV_8U || ముసుగు_2.type() == CV_8S || ముసుగు_2.type() == CV_Bool)) ||
               (cn > 1 && ముసుగు_2.empty() && !minIdx && !maxIdx) );

    CV_OCL_నిరంతరం(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (ముసుగు_2.empty() || _src.size() == ముసుగు_2.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, ముసుగు_2))

    Mat src = _src.getMat(), ముసుగు = ముసుగు_2.getMat();

    ఒకవేళ (src.dims <= 2)
    {
        ఒకవేళ ((size_t)src.అడుగు == (size_t)ముసుగు.అడుగు || ముసుగు.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.దత్తాంశం, src.అడుగు, src.నిలువుల_సంఖ్య*cn, src.అడ్డువరుసలు,
                     src.లోతు(), minVal, maxVal, minIdx, maxIdx, ముసుగు.దత్తాంశం);
        }
        CALL_HAL(min_Max_Idx_ముసుగు_అడుగు, cv_hal_min_Max_Idx_ముసుగు_అడుగు, src.దత్తాంశం, src.అడుగు, src.నిలువుల_సంఖ్య*cn, src.అడ్డువరుసలు,
                 src.లోతు(), minVal, maxVal, minIdx, maxIdx, ముసుగు.దత్తాంశం, ముసుగు.అడుగు);
    }
    లేకపోతే ఒకవేళ (src.isContinuous() && (ముసుగు.isContinuous() || ముసుగు.empty()))
    {
        పూర్ణాంకం res = cv_hal_minMaxIdx(src.దత్తాంశం, 0, (పూర్ణాంకం)src.మొత్తం()*cn, 1, src.లోతు(),
                                   minVal, maxVal, minIdx, maxIdx, ముసుగు.దత్తాంశం);

        ఒకవేళ (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            ఒకవేళ (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            ఒకవేళ (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            తిరిగి;
        }
        లేకపోతే ఒకవేళ (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::Sts_అంతర్గత,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(లోతు);
    CV_Assert( func != 0 );

    స్థిరరాశి Mat* arrays[] = {&src, &ముసుగు, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    సంఘం {
        పూర్ణాంకం i;
        దశాంశం f;
        ద్విగుణ d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    పూర్ణాంకం plane_కొలత = (పూర్ణాంకం)it.size*cn;
    minval.L = maxval.L = 0;

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_కొలత )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_కొలత, startidx );

    ద్విగుణ dminval, dmaxval;
    ఒకవేళ( లోతు <= CV_32S || లోతు == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    లేకపోతే ఒకవేళ( లోతు == CV_32F || లోతు == CV_16F || లోతు == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    లేకపోతే ఒకవేళ( లోతు == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    లేకపోతే ఒకవేళ( లోతు == CV_64S || లోతు == CV_32U )
        dminval = (ద్విగుణ)minval.L, dmaxval = (ద్విగుణ)maxval.L;
    లేకపోతే {
        CV_Assert(లోతు == CV_64U);
        dminval = (ద్విగుణ)minval.UL, dmaxval = (ద్విగుణ)maxval.UL;
    }

    ఒకవేళ( minVal )
        *minVal = dminval;
    ఒకవేళ( maxVal )
        *maxVal = dmaxval;

    ఒకవేళ( minIdx )
        ofs2idx(src, minidx, minIdx);
    ఒకవేళ( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

శూన్య cv::minMaxIdx(ఆదానం_Array _src, ద్విగుణ* minVal,
                   ద్విగుణ* maxVal, పూర్ణాంకం* minIdx, పూర్ణాంకం* maxIdx,
                   ఆదానం_Array ముసుగు_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ఒకవేళ (!_src.isUMat())
    {
        Mat src = _src.getMat(), ముసుగు = ముసుగు_2.getMat();
        స్థిరరాశి పూర్ణాంకం cn = src.channels();
        స్థిరరాశి తార్కిక వాదనలు_Ok = (cn == 1 && (ముసుగు.empty() || ((ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S ||
                             ముసుగు.type() == CV_Bool) && src.size == ముసుగు.size))) ||
                            (cn > 1 && ముసుగు.empty() && !minIdx && !maxIdx);
        ఒకవేళ (వాదనలు_Ok && విభజించు_For_Parallel_Stat(src, ముసుగు, chunks))
        {
            స్థిరరాశి పూర్ణాంకం n = (పూర్ణాంకం)chunks.size();
            std::vector<ద్విగుణ> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](స్థిరరాశి Range& r)
            {
                కొరకు (పూర్ణాంకం i = r.ప్రారంభం; i < r.end; i++)
                {
                    పూర్ణాంకం a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : శూన్యసూచిక, cn == 1 ? b : శూన్యసూచిక,
                               chunks[i].ముసుగు);
                    ఒకవేళ (cn > 1)
                        imn[i] = imx[i] = 1;
                    లేకపోతే ఒకవేళ (a[0] >= 0)
                    {
                        imn[i] = chunk_సూచీ_గమ్యం_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_సూచీ_గమ్యం_Elem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            పూర్ణాంకం bmin = -1, bmax = -1;
            కొరకు (పూర్ణాంకం i = 0; i < n; i++)
            {
                ఒకవేళ (imn[i] == 0)
                    కొనసాగు;
                ఒకవేళ (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                ఒకవేళ (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            ఒకవేళ (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            ఒకవేళ (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            ఒకవేళ (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            ఒకవేళ (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            తిరిగి;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, ముసుగు_2);
}

శూన్య cv::minMaxLoc( ఆదానం_Array _img, ద్విగుణ* minVal, ద్విగుణ* maxVal,
                    బిందువు* minLoc, బిందువు* maxLoc, ఆదానం_Array ముసుగు )
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (పూర్ణాంకం*)minLoc, (పూర్ణాంకం*)maxLoc, ముసుగు);
    ఒకవేళ( minLoc) {
        ఒకవేళ (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        లేకపోతే {
            minLoc->y = 0;
        }
    }
    ఒకవేళ( maxLoc) {
        ఒకవేళ (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        లేకపోతే {
            maxLoc->y = 0;
        }
    }
}

గణన తరగతి Reduce_బహుళకం
{
    మొదటి_MIN = 0, //!< get index of first min occurrence
    చివరి_MIN  = 1, //!< get index of last min occurrence
    మొదటి_MAX = 2, //!< get index of first max occurrence
    చివరి_MAX  = 3, //!< get index of last max occurrence
};

మూస <రకం_పేరు T>
నిర్మాణం reduceMinMaxImpl
{
    శూన్య క్రియాకారకం()(స్థిరరాశి cv::Mat& src, cv::Mat& dst, Reduce_బహుళకం బహుళకం, స్థిరరాశి పూర్ణాంకం axis) స్థిరరాశి
    {
        ఎంపిక(బహుళకం)
        {
        స్థితి Reduce_బహుళకం::మొదటి_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            ఆపు;
        స్థితి Reduce_బహుళకం::చివరి_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            ఆపు;
        స్థితి Reduce_బహుళకం::మొదటి_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            ఆపు;
        స్థితి Reduce_బహుళకం::చివరి_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            ఆపు;
        }
    }

    మూస <మూస<తరగతి> తరగతి Cmp>
    స్థిర శూన్య reduceMinMaxApply(స్థిరరాశి cv::Mat& src, cv::Mat& dst, స్థిరరాశి పూర్ణాంకం axis)
    {
        Cmp<T> cmp;

        స్థిరరాశి స్వయంచాలక *src_ptr = src.ptr<T>();
        స్వయంచాలక *dst_ptr = dst.ptr<int32_t>();

        స్థిరరాశి size_t outer_కొలత = src.మొత్తం(0, axis);
        స్థిరరాశి స్వయంచాలక mid_కొలత = static_cast<size_t>(src.size[axis]);

        స్థిరరాశి size_t outer_అడుగు = src.మొత్తం(axis);
        స్థిరరాశి size_t dst_అడుగు = dst.మొత్తం(axis);

        స్థిరరాశి size_t mid_అడుగు = src.మొత్తం(axis + 1);

        కొరకు (size_t outer = 0; outer < outer_కొలత; ++outer)
        {
            స్థిరరాశి size_t outer_స్థానభ్రంశం = outer * outer_అడుగు;
            స్థిరరాశి size_t dst_స్థానభ్రంశం = outer * dst_అడుగు;
            కొరకు (size_t mid = 0; mid != mid_కొలత; ++mid)
            {
                స్థిరరాశి size_t src_స్థానభ్రంశం = outer_స్థానభ్రంశం + mid * mid_అడుగు;
                కొరకు (size_t inner = 0; inner < mid_అడుగు; inner++)
                {
                    int32_t& సూచీ = dst_ptr[dst_స్థానభ్రంశం + inner];

                    స్థిరరాశి size_t prev = outer_స్థానభ్రంశం + సూచీ * mid_అడుగు + inner;
                    స్థిరరాశి size_t curr = src_స్థానభ్రంశం + inner;

                    ఒకవేళ (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        సూచీ = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

స్థిర శూన్య reduceMinMax(cv::ఆదానం_Array src, cv::OutputArray dst, Reduce_బహుళకం బహుళకం, పూర్ణాంకం axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    పూర్ణాంకం dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<పూర్ణాంకం> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    ఒకవేళ(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.దత్తాంశం(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.సమితి_గమ్యం(cv::Scalar::all(0));

    ఒకవేళ (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    తార్కిక needs_copy = !dstMat.isContinuous();
    ఒకవేళ (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::లోతు_Dispatch<reduceMinMaxImpl>(srcMat.లోతు(), srcMat, dstMat, బహుళకం, axis);

    ఒకవేళ (needs_copy)
    {
        dstMat.copy_గమ్యం(dst);
    }
}

శూన్య cv::reduceArgMin(ఆదానం_Array src, OutputArray dst, పూర్ణాంకం axis, తార్కిక చివరి_సూచీ)
{
    reduceMinMax(src, dst, చివరి_సూచీ ? Reduce_బహుళకం::చివరి_MIN : Reduce_బహుళకం::మొదటి_MIN, axis);
}

శూన్య cv::reduceArgMax(ఆదానం_Array src, OutputArray dst, పూర్ణాంకం axis, తార్కిక చివరి_సూచీ)
{
    reduceMinMax(src, dst, చివరి_సూచీ ? Reduce_బహుళకం::చివరి_MAX : Reduce_బహుళకం::మొదటి_MAX, axis);
}
