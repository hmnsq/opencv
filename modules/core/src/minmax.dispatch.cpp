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

ساكن MinMaxIdxFunc getMinMaxIdxFunc(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (عمق),
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

#اگر_تعریف_شده HAVE_OPENCL

#تعریف_کن MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

قالب <نام_نوع T>
فراغ getMinMaxRes(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                  صحيح* minLoc, صحيح* maxLoc,
                  صحيح groupnum, صحيح تعداد_ستون, مزدوج * maxVal2)
{
    uint شاخص_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = شاخص_max, maxloc = شاخص_max;

    size_t شاخص = 0;
    ثابت T * minptr = عدم, * maxptr = عدم, * maxptr2 = عدم;
    ثابت uint * minlocptr = عدم, * maxlocptr = عدم;
    إذا (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        شاخص += حجم(T) * groupnum;
        شاخص = align_اندازه(شاخص, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxVal || maxLoc)
    {
        maxptr = (ثابت T *)(db.ptr() + شاخص);
        شاخص += حجم(T) * groupnum;
        شاخص = align_اندازه(شاخص, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (minLoc)
    {
        minlocptr = (ثابت uint *)(db.ptr() + شاخص);
        شاخص += حجم(uint) * groupnum;
        شاخص = align_اندازه(شاخص, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxLoc)
    {
        maxlocptr = (ثابت uint *)(db.ptr() + شاخص);
        شاخص += حجم(uint) * groupnum;
        شاخص = align_اندازه(شاخص, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxVal2)
        maxptr2 = (ثابت T *)(db.ptr() + شاخص);

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
    منطقي zero_ماسک = (minLoc && minloc == شاخص_max) ||
    (maxLoc && maxloc == شاخص_max);

    إذا (minVal)
        *minVal = zero_ماسک ? 0 : (مزدوج)minval;
    إذا (maxVal)
        *maxVal = zero_ماسک ? 0 : (مزدوج)maxval;
    إذا (maxVal2)
        *maxVal2 = zero_ماسک ? 0 : (مزدوج)maxval2;

    إذا (minLoc)
    {
        minLoc[0] = zero_ماسک ? -1 : minloc / تعداد_ستون;
        minLoc[1] = zero_ماسک ? -1 : minloc % تعداد_ستون;
    }
    إذا (maxLoc)
    {
        maxLoc[0] = zero_ماسک ? -1 : maxloc / تعداد_ستون;
        maxLoc[1] = zero_ماسک ? -1 : maxloc % تعداد_ستون;
    }
}

تعريفنوع فراغ (*getMinMaxResFunc)(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                                 صحيح * minLoc, صحيح *maxLoc, صحيح gropunum, صحيح تعداد_ستون, مزدوج * maxVal2);

منطقي ocl_minMaxIdx( درون_داد_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, درون_داد_Array ماسک_2,
                   صحيح ddepth, منطقي abs_مقادیر, درون_داد_Array _src2, مزدوج * maxVal2)
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();

#اگر_تعریف_شده __ANDROID__
    إذا (dev.isNVidia())
        أرجع خطأ;
#پایان_اگر

    إذا (dev.device_نسخه_Major() == 1 && dev.device_نسخه_Minor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        أرجع خطأ;
    }

    منطقي doubleSupport = dev.doubleFPConfig() > 0, have_ماسک = !ماسک_2.empty(),
    haveSrc2 = _src2.نوع() != درون_داد_Array_2::NONE;
    صحيح type = _src.type(), عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type),
    kercn = have_ماسک ? cn : std::min(4, ocl::predict_Optimal_وکتور_عرض(_src, _src2));

    إذا (عمق >= CV_16F)
        أرجع خطأ;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    إذا ((have_ماسک || type == CV_32FC1) && dev.isAMD())
        أرجع خطأ;

    CV_Assert( (cn == 1 && (!have_ماسک || ماسک_2.type() == CV_8U || ماسک_2.type() == CV_8S || ماسک_2.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    إذا (ddepth < 0)
        ddepth = عمق;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    إذا (عمق == CV_32S || عمق == CV_8S || عمق == CV_32U || عمق == CV_64U ||
        عمق == CV_64S || عمق == CV_16F || عمق == CV_16BF)
        أرجع خطأ;

    إذا ((عمق == CV_64F || ddepth == CV_64F) && !doubleSupport)
        أرجع خطأ;

    صحيح groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_گروه_اندازه();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    منطقي needMinVal = minVal || minLoc, needMinLoc = minLoc != عدم,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != عدم;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    إذا (!(needMaxLoc || needMinLoc) && have_ماسک)
    {
        إذا (needMinVal)
            needMinLoc = درست;
        وإلا
            needMaxLoc = درست;
    }

    حرف cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         عمق, ocl::type_مقصد_Str(عمق), have_ماسک ? " -D HAVE_MASK" : "", (صحيح)wgs,
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(عمق, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         ماسک_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::type_مقصد_Str(ddepth), ocl::type_مقصد_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(عمق, ddepth, kercn, cvt[0], حجم(cvt[0])),
                         abs_مقادیر ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         عمق <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], حجم(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    صحيح esz = CV_ELEM_اندازه(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), ماسک = ماسک_2.getUMat();

    إذا (cn > 1 && !have_ماسک)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    إذا (haveSrc2)
    {
        إذا (!have_ماسک)
            k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(src), src.تعداد_ستون, (صحيح)src.مجموع(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_اندازه(src2));
        وإلا
            k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(src), src.تعداد_ستون, (صحيح)src.مجموع(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_اندازه(ماسک),
                   ocl::KernelArg::Read_Only_No_اندازه(src2));
    }
    وإلا
    {
        إذا (!have_ماسک)
            k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(src), src.تعداد_ستون, (صحيح)src.مجموع(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        وإلا
            k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(src), src.تعداد_ستون, (صحيح)src.مجموع(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_اندازه(ماسک));
    }

    size_t globalsize = groupnum * wgs;
    إذا (!k.پیاپی(1, &globalsize, &wgs, درست))
        أرجع خطأ;

    ساكن ثابت getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<حرف>,
        getMinMaxRes<ushort>,
        getMinMaxRes<کوتاه>,
        getMinMaxRes<صحيح>,
        getMinMaxRes<عائم>,
        getMinMaxRes<مزدوج>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    صحيح loc_موقت[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_موقت : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_موقت : maxLoc,
         groupnum, src.تعداد_ستون, maxVal2);

    أرجع درست;
}

#پایان_اگر

}

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ minMaxIdx_(cv::درون_داد_Array _src, مزدوج* minVal,
                       مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                       cv::درون_داد_Array ماسک_2)
{
    استفاده نطاق cv;
    صحيح type = _src.type(), عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (ماسک_2.empty() || ماسک_2.type() == CV_8U || ماسک_2.type() == CV_8S || ماسک_2.type() == CV_Bool)) ||
               (cn > 1 && ماسک_2.empty() && !minIdx && !maxIdx) );

    CV_OCL_پیاپی(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (ماسک_2.empty() || _src.size() == ماسک_2.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, ماسک_2))

    Mat src = _src.getMat(), ماسک = ماسک_2.getMat();

    إذا (src.dims <= 2)
    {
        إذا ((size_t)src.گام == (size_t)ماسک.گام || ماسک.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.داده, src.گام, src.تعداد_ستون*cn, src.ردیفها,
                     src.عمق(), minVal, maxVal, minIdx, maxIdx, ماسک.داده);
        }
        CALL_HAL(min_Max_Idx_ماسک_گام, cv_hal_min_Max_Idx_ماسک_گام, src.داده, src.گام, src.تعداد_ستون*cn, src.ردیفها,
                 src.عمق(), minVal, maxVal, minIdx, maxIdx, ماسک.داده, ماسک.گام);
    }
    وإلا إذا (src.isContinuous() && (ماسک.isContinuous() || ماسک.empty()))
    {
        صحيح res = cv_hal_minMaxIdx(src.داده, 0, (صحيح)src.مجموع()*cn, 1, src.عمق(),
                                   minVal, maxVal, minIdx, maxIdx, ماسک.داده);

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
            CV_Error_(cv::Error::Sts_داخلی,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(عمق);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &ماسک, 0};
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
    صحيح plane_اندازه = (صحيح)it.size*cn;
    minval.L = maxval.L = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_اندازه )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_اندازه, startidx );

    مزدوج dminval, dmaxval;
    إذا( عمق <= CV_32S || عمق == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    وإلا إذا( عمق == CV_32F || عمق == CV_16F || عمق == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    وإلا إذا( عمق == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    وإلا إذا( عمق == CV_64S || عمق == CV_32U )
        dminval = (مزدوج)minval.L, dmaxval = (مزدوج)maxval.L;
    وإلا {
        CV_Assert(عمق == CV_64U);
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

فراغ cv::minMaxIdx(درون_داد_Array _src, مزدوج* minVal,
                   مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                   درون_داد_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat())
    {
        Mat src = _src.getMat(), ماسک = ماسک_2.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي آرگومانها_Ok = (cn == 1 && (ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S ||
                             ماسک.type() == CV_Bool) && src.size == ماسک.size))) ||
                            (cn > 1 && ماسک.empty() && !minIdx && !maxIdx);
        إذا (آرگومانها_Ok && تقسیم_For_Parallel_Stat(src, ماسک, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<مزدوج> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.شروع; i < r.end; i++)
                {
                    صحيح a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : اشاره_تهی, cn == 1 ? b : اشاره_تهی,
                               chunks[i].ماسک);
                    إذا (cn > 1)
                        imn[i] = imx[i] = 1;
                    وإلا إذا (a[0] >= 0)
                    {
                        imn[i] = chunk_شاخص_مقصد_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_شاخص_مقصد_Elem(chunks[i], b) + 1;
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
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, ماسک_2);
}

فراغ cv::minMaxLoc( درون_داد_Array _img, مزدوج* minVal, مزدوج* maxVal,
                    نقطه* minLoc, نقطه* maxLoc, درون_داد_Array ماسک )
{
    CV_INSTRUMENT_REGION();

    صحيح dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (صحيح*)minLoc, (صحيح*)maxLoc, ماسک);
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

تعداد صنف Reduce_نما
{
    اولی_MIN = 0, //!< get index of first min occurrence
    آخرین_MIN  = 1, //!< get index of last min occurrence
    اولی_MAX = 2, //!< get index of first max occurrence
    آخرین_MAX  = 3, //!< get index of last max occurrence
};

قالب <نام_نوع T>
بنية reduceMinMaxImpl
{
    فراغ عملگر()(ثابت cv::Mat& src, cv::Mat& dst, Reduce_نما نما, ثابت صحيح axis) ثابت
    {
        بدل(نما)
        {
        حالة Reduce_نما::اولی_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            اكسر;
        حالة Reduce_نما::آخرین_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            اكسر;
        حالة Reduce_نما::اولی_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            اكسر;
        حالة Reduce_نما::آخرین_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            اكسر;
        }
    }

    قالب <قالب<صنف> صنف Cmp>
    ساكن فراغ reduceMinMaxApply(ثابت cv::Mat& src, cv::Mat& dst, ثابت صحيح axis)
    {
        Cmp<T> cmp;

        ثابت خودکار *src_ptr = src.ptr<T>();
        خودکار *dst_ptr = dst.ptr<int32_t>();

        ثابت size_t outer_اندازه = src.مجموع(0, axis);
        ثابت خودکار mid_اندازه = static_cast<size_t>(src.size[axis]);

        ثابت size_t outer_گام = src.مجموع(axis);
        ثابت size_t dst_گام = dst.مجموع(axis);

        ثابت size_t mid_گام = src.مجموع(axis + 1);

        لكل (size_t outer = 0; outer < outer_اندازه; ++outer)
        {
            ثابت size_t outer_جابه_جایی = outer * outer_گام;
            ثابت size_t dst_جابه_جایی = outer * dst_گام;
            لكل (size_t mid = 0; mid != mid_اندازه; ++mid)
            {
                ثابت size_t src_جابه_جایی = outer_جابه_جایی + mid * mid_گام;
                لكل (size_t inner = 0; inner < mid_گام; inner++)
                {
                    int32_t& شاخص = dst_ptr[dst_جابه_جایی + inner];

                    ثابت size_t prev = outer_جابه_جایی + شاخص * mid_گام + inner;
                    ثابت size_t curr = src_جابه_جایی + inner;

                    إذا (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        شاخص = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ساكن فراغ reduceMinMax(cv::درون_داد_Array src, cv::OutputArray dst, Reduce_نما نما, صحيح axis)
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

    dst.create(srcMat.dims, sizes.داده(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.مجموعه_مقصد(cv::Scalar::all(0));

    إذا (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    منطقي needs_copy = !dstMat.isContinuous();
    إذا (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::عمق_Dispatch<reduceMinMaxImpl>(srcMat.عمق(), srcMat, dstMat, نما, axis);

    إذا (needs_copy)
    {
        dstMat.copy_مقصد(dst);
    }
}

فراغ cv::reduceArgMin(درون_داد_Array src, OutputArray dst, صحيح axis, منطقي آخرین_شاخص)
{
    reduceMinMax(src, dst, آخرین_شاخص ? Reduce_نما::آخرین_MIN : Reduce_نما::اولی_MIN, axis);
}

فراغ cv::reduceArgMax(درون_داد_Array src, OutputArray dst, صحيح axis, منطقي آخرین_شاخص)
{
    reduceMinMax(src, dst, آخرین_شاخص ? Reduce_نما::آخرین_MAX : Reduce_نما::اولی_MAX, axis);
}
