// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"
#ضمن "stat_parallel.hpp"
#ضمن "opencv2/core/detail/dispatch_helper.impl.hpp"
#ضمن <algorithm>

#ضمن "minmax.simd.hpp"
#ضمن "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن MinMaxIdxFunc getMinMaxIdxFunc(صحيح اونهائي)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (اونهائي),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
ساكن فراغ ofs2idx(ثابت Mat& a, size_t ofs, صحيح* idx)
{
    صحيح i, d = a.dims;
    إذا( ofs > 0 )
    {
        ofs--;
        لكل( i = d-1; i >= 0; i-- )
        {
            صحيح sz = a.size[i];
            idx[i] = (صحيح)(ofs % sz);
            ofs /= sz;
        }
    }
    وإلا
    {
        لكل( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

#وصف_ڪريو MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

سانچو <قسم_نالو T>
فراغ getMinMaxRes(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                  صحيح* minLoc, صحيح* maxLoc,
                  صحيح groupnum, صحيح کانن_ڳڻپ, مزدوج * maxVal2)
{
    uint اشاريو_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = اشاريو_max, maxloc = اشاريو_max;

    size_t اشاريو = 0;
    ثابت T * minptr = عدم, * maxptr = عدم, * maxptr2 = عدم;
    ثابت uint * minlocptr = عدم, * maxlocptr = عدم;
    إذا (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        اشاريو += حجم(T) * groupnum;
        اشاريو = align_سائيز(اشاريو, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxVal || maxLoc)
    {
        maxptr = (ثابت T *)(db.ptr() + اشاريو);
        اشاريو += حجم(T) * groupnum;
        اشاريو = align_سائيز(اشاريو, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (minLoc)
    {
        minlocptr = (ثابت uint *)(db.ptr() + اشاريو);
        اشاريو += حجم(uint) * groupnum;
        اشاريو = align_سائيز(اشاريو, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxLoc)
    {
        maxlocptr = (ثابت uint *)(db.ptr() + اشاريو);
        اشاريو += حجم(uint) * groupnum;
        اشاريو = align_سائيز(اشاريو, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxVal2)
        maxptr2 = (ثابت T *)(db.ptr() + اشاريو);

    لكل (صحيح i = 0; i < groupnum; i++)
    {
        إذا (minptr && minptr[i] <= minval)
        {
            إذا (minptr[i] == minval)
            {
                إذا (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            وإلا
            {
                إذا (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        إذا (maxptr && maxptr[i] >= maxval)
        {
            إذا (maxptr[i] == maxval)
            {
                إذا (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            وإلا
            {
                إذا (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        إذا (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    منطقي zero_نقاب = (minLoc && minloc == اشاريو_max) ||
    (maxLoc && maxloc == اشاريو_max);

    إذا (minVal)
        *minVal = zero_نقاب ? 0 : (مزدوج)minval;
    إذا (maxVal)
        *maxVal = zero_نقاب ? 0 : (مزدوج)maxval;
    إذا (maxVal2)
        *maxVal2 = zero_نقاب ? 0 : (مزدوج)maxval2;

    إذا (minLoc)
    {
        minLoc[0] = zero_نقاب ? -1 : minloc / کانن_ڳڻپ;
        minLoc[1] = zero_نقاب ? -1 : minloc % کانن_ڳڻپ;
    }
    إذا (maxLoc)
    {
        maxLoc[0] = zero_نقاب ? -1 : maxloc / کانن_ڳڻپ;
        maxLoc[1] = zero_نقاب ? -1 : maxloc % کانن_ڳڻپ;
    }
}

تعريفنوع فراغ (*getMinMaxResFunc)(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                                 صحيح * minLoc, صحيح *maxLoc, صحيح gropunum, صحيح کانن_ڳڻپ, مزدوج * maxVal2);

منطقي ocl_minMaxIdx( داخلا_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, داخلا_Array نقاب_2,
                   صحيح ddepth, منطقي abs_قدرون, داخلا_Array _src2, مزدوج * maxVal2)
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();

#جيڪڏهن_وصف_ٿيل __ANDROID__
    إذا (dev.isNVidia())
        أرجع خطأ;
#جيڪڏهن_پورو

    إذا (dev.device_نسخو_Major() == 1 && dev.device_نسخو_Minor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        أرجع خطأ;
    }

    منطقي doubleSupport = dev.doubleFPConfig() > 0, have_نقاب = !نقاب_2.empty(),
    haveSrc2 = _src2.قسم() != داخلا_Array_2::NONE;
    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type),
    kercn = have_نقاب ? cn : std::min(4, ocl::predict_Optimal_ويڪٽر_ويڪر(_src, _src2));

    إذا (اونهائي >= CV_16F)
        أرجع خطأ;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    إذا ((have_نقاب || type == CV_32FC1) && dev.isAMD())
        أرجع خطأ;

    CV_Assert( (cn == 1 && (!have_نقاب || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    إذا (ddepth < 0)
        ddepth = اونهائي;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    إذا (اونهائي == CV_32S || اونهائي == CV_8S || اونهائي == CV_32U || اونهائي == CV_64U ||
        اونهائي == CV_64S || اونهائي == CV_16F || اونهائي == CV_16BF)
        أرجع خطأ;

    إذا ((اونهائي == CV_64F || ddepth == CV_64F) && !doubleSupport)
        أرجع خطأ;

    صحيح groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_گروهه_سائيز();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    منطقي needMinVal = minVal || minLoc, needMinLoc = minLoc != عدم,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != عدم;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    إذا (!(needMaxLoc || needMinLoc) && have_نقاب)
    {
        إذا (needMinVal)
            needMinLoc = سچ;
        وإلا
            needMaxLoc = سچ;
    }

    حرف cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         اونهائي, ocl::type_منزل_Str(اونهائي), have_نقاب ? " -D HAVE_MASK" : "", (صحيح)wgs,
                         ocl::type_منزل_Str(CV_MAKE_TYPE(اونهائي, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         نقاب_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::type_منزل_Str(ddepth), ocl::type_منزل_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(اونهائي, ddepth, kercn, cvt[0], حجم(cvt[0])),
                         abs_قدرون ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         اونهائي <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], حجم(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    صحيح esz = CV_ELEM_سائيز(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), نقاب = نقاب_2.getUMat();

    إذا (cn > 1 && !have_نقاب)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    إذا (haveSrc2)
    {
        إذا (!have_نقاب)
            k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(src), src.کانن_ڳڻپ, (صحيح)src.ڪل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_سائيز(src2));
        وإلا
            k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(src), src.کانن_ڳڻپ, (صحيح)src.ڪل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_سائيز(نقاب),
                   ocl::KernelArg::Read_Only_No_سائيز(src2));
    }
    وإلا
    {
        إذا (!have_نقاب)
            k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(src), src.کانن_ڳڻپ, (صحيح)src.ڪل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        وإلا
            k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(src), src.کانن_ڳڻپ, (صحيح)src.ڪل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_سائيز(نقاب));
    }

    size_t globalsize = groupnum * wgs;
    إذا (!k.لاڳيتو(1, &globalsize, &wgs, سچ))
        أرجع خطأ;

    ساكن ثابت getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<حرف>,
        getMinMaxRes<ushort>,
        getMinMaxRes<ننڍو>,
        getMinMaxRes<صحيح>,
        getMinMaxRes<عائم>,
        getMinMaxRes<مزدوج>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    صحيح loc_عارضي[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_عارضي : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_عارضي : maxLoc,
         groupnum, src.کانن_ڳڻپ, maxVal2);

    أرجع سچ;
}

#جيڪڏهن_پورو

}

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ minMaxIdx_(cv::داخلا_Array _src, مزدوج* minVal,
                       مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                       cv::داخلا_Array نقاب_2)
{
    استعمال نطاق cv;
    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (نقاب_2.empty() || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool)) ||
               (cn > 1 && نقاب_2.empty() && !minIdx && !maxIdx) );

    CV_OCL_لاڳيتو(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (نقاب_2.empty() || _src.size() == نقاب_2.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, نقاب_2))

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();

    إذا (src.dims <= 2)
    {
        إذا ((size_t)src.قدم == (size_t)نقاب.قدم || نقاب.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.معلومات, src.قدم, src.کانن_ڳڻپ*cn, src.صفون,
                     src.اونهائي(), minVal, maxVal, minIdx, maxIdx, نقاب.معلومات);
        }
        CALL_HAL(min_Max_Idx_نقاب_قدم, cv_hal_min_Max_Idx_نقاب_قدم, src.معلومات, src.قدم, src.کانن_ڳڻپ*cn, src.صفون,
                 src.اونهائي(), minVal, maxVal, minIdx, maxIdx, نقاب.معلومات, نقاب.قدم);
    }
    وإلا إذا (src.isContinuous() && (نقاب.isContinuous() || نقاب.empty()))
    {
        صحيح res = cv_hal_minMaxIdx(src.معلومات, 0, (صحيح)src.ڪل()*cn, 1, src.اونهائي(),
                                   minVal, maxVal, minIdx, maxIdx, نقاب.معلومات);

        إذا (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            إذا (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            إذا (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            أرجع;
        }
        وإلا إذا (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::Sts_اندروني,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(اونهائي);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    اتحاد {
        صحيح i;
        عائم f;
        مزدوج d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    صحيح plane_سائيز = (صحيح)it.size*cn;
    minval.L = maxval.L = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_سائيز )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_سائيز, startidx );

    مزدوج dminval, dmaxval;
    إذا( اونهائي <= CV_32S || اونهائي == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    وإلا إذا( اونهائي == CV_32F || اونهائي == CV_16F || اونهائي == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    وإلا إذا( اونهائي == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    وإلا إذا( اونهائي == CV_64S || اونهائي == CV_32U )
        dminval = (مزدوج)minval.L, dmaxval = (مزدوج)maxval.L;
    وإلا {
        CV_Assert(اونهائي == CV_64U);
        dminval = (مزدوج)minval.UL, dmaxval = (مزدوج)maxval.UL;
    }

    إذا( minVal )
        *minVal = dminval;
    إذا( maxVal )
        *maxVal = dmaxval;

    إذا( minIdx )
        ofs2idx(src, minidx, minIdx);
    إذا( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

فراغ cv::minMaxIdx(داخلا_Array _src, مزدوج* minVal,
                   مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                   داخلا_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat())
    {
        Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي دليلون_Ok = (cn == 1 && (نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S ||
                             نقاب.type() == CV_Bool) && src.size == نقاب.size))) ||
                            (cn > 1 && نقاب.empty() && !minIdx && !maxIdx);
        إذا (دليلون_Ok && ورهايو_For_Parallel_Stat(src, نقاب, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<مزدوج> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.شروعات; i < r.end; i++)
                {
                    صحيح a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : خالي_اشارو, cn == 1 ? b : خالي_اشارو,
                               chunks[i].نقاب);
                    إذا (cn > 1)
                        imn[i] = imx[i] = 1;
                    وإلا إذا (a[0] >= 0)
                    {
                        imn[i] = chunk_اشاريو_منزل_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_اشاريو_منزل_Elem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            صحيح bmin = -1, bmax = -1;
            لكل (صحيح i = 0; i < n; i++)
            {
                إذا (imn[i] == 0)
                    استمر;
                إذا (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                إذا (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            إذا (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            إذا (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            إذا (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            إذا (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            أرجع;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, نقاب_2);
}

فراغ cv::minMaxLoc( داخلا_Array _img, مزدوج* minVal, مزدوج* maxVal,
                    نقطو* minLoc, نقطو* maxLoc, داخلا_Array نقاب )
{
    CV_INSTRUMENT_REGION();

    صحيح dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (صحيح*)minLoc, (صحيح*)maxLoc, نقاب);
    إذا( minLoc) {
        إذا (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        وإلا {
            minLoc->y = 0;
        }
    }
    إذا( maxLoc) {
        إذا (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        وإلا {
            maxLoc->y = 0;
        }
    }
}

تعداد صنف Reduce_گهڻائي
{
    پهريون_MIN = 0, //!< get index of first min occurrence
    آخري_MIN  = 1, //!< get index of last min occurrence
    پهريون_MAX = 2, //!< get index of first max occurrence
    آخري_MAX  = 3, //!< get index of last max occurrence
};

سانچو <قسم_نالو T>
بنية reduceMinMaxImpl
{
    فراغ عامل()(ثابت cv::Mat& src, cv::Mat& dst, Reduce_گهڻائي گهڻائي, ثابت صحيح axis) ثابت
    {
        بدل(گهڻائي)
        {
        حالة Reduce_گهڻائي::پهريون_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            اكسر;
        حالة Reduce_گهڻائي::آخري_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            اكسر;
        حالة Reduce_گهڻائي::پهريون_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            اكسر;
        حالة Reduce_گهڻائي::آخري_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            اكسر;
        }
    }

    سانچو <سانچو<صنف> صنف Cmp>
    ساكن فراغ reduceMinMaxApply(ثابت cv::Mat& src, cv::Mat& dst, ثابت صحيح axis)
    {
        Cmp<T> cmp;

        ثابت خودڪار *src_ptr = src.ptr<T>();
        خودڪار *dst_ptr = dst.ptr<int32_t>();

        ثابت size_t outer_سائيز = src.ڪل(0, axis);
        ثابت خودڪار mid_سائيز = static_cast<size_t>(src.size[axis]);

        ثابت size_t outer_قدم = src.ڪل(axis);
        ثابت size_t dst_قدم = dst.ڪل(axis);

        ثابت size_t mid_قدم = src.ڪل(axis + 1);

        لكل (size_t outer = 0; outer < outer_سائيز; ++outer)
        {
            ثابت size_t outer_وٿي = outer * outer_قدم;
            ثابت size_t dst_وٿي = outer * dst_قدم;
            لكل (size_t mid = 0; mid != mid_سائيز; ++mid)
            {
                ثابت size_t src_وٿي = outer_وٿي + mid * mid_قدم;
                لكل (size_t inner = 0; inner < mid_قدم; inner++)
                {
                    int32_t& اشاريو = dst_ptr[dst_وٿي + inner];

                    ثابت size_t prev = outer_وٿي + اشاريو * mid_قدم + inner;
                    ثابت size_t curr = src_وٿي + inner;

                    إذا (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        اشاريو = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ساكن فراغ reduceMinMax(cv::داخلا_Array src, cv::OutputArray dst, Reduce_گهڻائي گهڻائي, صحيح axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    صحيح dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<صحيح> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    إذا(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.معلومات(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.سيٽ_منزل(cv::Scalar::all(0));

    إذا (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    منطقي needs_copy = !dstMat.isContinuous();
    إذا (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::اونهائي_Dispatch<reduceMinMaxImpl>(srcMat.اونهائي(), srcMat, dstMat, گهڻائي, axis);

    إذا (needs_copy)
    {
        dstMat.copy_منزل(dst);
    }
}

فراغ cv::reduceArgMin(داخلا_Array src, OutputArray dst, صحيح axis, منطقي آخري_اشاريو)
{
    reduceMinMax(src, dst, آخري_اشاريو ? Reduce_گهڻائي::آخري_MIN : Reduce_گهڻائي::پهريون_MIN, axis);
}

فراغ cv::reduceArgMax(داخلا_Array src, OutputArray dst, صحيح axis, منطقي آخري_اشاريو)
{
    reduceMinMax(src, dst, آخري_اشاريو ? Reduce_گهڻائي::آخري_MAX : Reduce_گهڻائي::پهريون_MAX, axis);
}
