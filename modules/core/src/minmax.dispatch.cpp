// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"
#شامل "stat_parallel.hpp"
#شامل "opencv2/core/detail/dispatch_helper.impl.hpp"
#شامل <algorithm>

#شامل "minmax.simd.hpp"
#شامل "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv {

ساکن MinMaxIdxFunc getMinMaxIdxFunc(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (گہرائی),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
ساکن خالی ofs2idx(ثابت Mat& a, size_t ofs, صحیح* idx)
{
    صحیح i, d = a.dims;
    اگر( ofs > 0 )
    {
        ofs--;
        برائے( i = d-1; i >= 0; i-- )
        {
            صحیح sz = a.size[i];
            idx[i] = (صحیح)(ofs % sz);
            ofs /= sz;
        }
    }
    ورنہ
    {
        برائے( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#اگر_تعریف_شدہ HAVE_OPENCL

#تعریف_کرو MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

سانچہ <قسم_نام T>
خالی getMinMaxRes(ثابت Mat & db, دوہرا * minVal, دوہرا * maxVal,
                  صحیح* minLoc, صحیح* maxLoc,
                  صحیح groupnum, صحیح ستون_تعداد, دوہرا * maxVal2)
{
    uint اشاریہ_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = اشاریہ_max, maxloc = اشاریہ_max;

    size_t اشاریہ = 0;
    ثابت T * minptr = تہی, * maxptr = تہی, * maxptr2 = تہی;
    ثابت uint * minlocptr = تہی, * maxlocptr = تہی;
    اگر (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        اشاریہ += حجم(T) * groupnum;
        اشاریہ = align_سائز(اشاریہ, MINMAX_STRUCT_ALIGNMENT);
    }
    اگر (maxVal || maxLoc)
    {
        maxptr = (ثابت T *)(db.ptr() + اشاریہ);
        اشاریہ += حجم(T) * groupnum;
        اشاریہ = align_سائز(اشاریہ, MINMAX_STRUCT_ALIGNMENT);
    }
    اگر (minLoc)
    {
        minlocptr = (ثابت uint *)(db.ptr() + اشاریہ);
        اشاریہ += حجم(uint) * groupnum;
        اشاریہ = align_سائز(اشاریہ, MINMAX_STRUCT_ALIGNMENT);
    }
    اگر (maxLoc)
    {
        maxlocptr = (ثابت uint *)(db.ptr() + اشاریہ);
        اشاریہ += حجم(uint) * groupnum;
        اشاریہ = align_سائز(اشاریہ, MINMAX_STRUCT_ALIGNMENT);
    }
    اگر (maxVal2)
        maxptr2 = (ثابت T *)(db.ptr() + اشاریہ);

    برائے (صحیح i = 0; i < groupnum; i++)
    {
        اگر (minptr && minptr[i] <= minval)
        {
            اگر (minptr[i] == minval)
            {
                اگر (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            ورنہ
            {
                اگر (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        اگر (maxptr && maxptr[i] >= maxval)
        {
            اگر (maxptr[i] == maxval)
            {
                اگر (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            ورنہ
            {
                اگر (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        اگر (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    منطقی zero_نقاب = (minLoc && minloc == اشاریہ_max) ||
    (maxLoc && maxloc == اشاریہ_max);

    اگر (minVal)
        *minVal = zero_نقاب ? 0 : (دوہرا)minval;
    اگر (maxVal)
        *maxVal = zero_نقاب ? 0 : (دوہرا)maxval;
    اگر (maxVal2)
        *maxVal2 = zero_نقاب ? 0 : (دوہرا)maxval2;

    اگر (minLoc)
    {
        minLoc[0] = zero_نقاب ? -1 : minloc / ستون_تعداد;
        minLoc[1] = zero_نقاب ? -1 : minloc % ستون_تعداد;
    }
    اگر (maxLoc)
    {
        maxLoc[0] = zero_نقاب ? -1 : maxloc / ستون_تعداد;
        maxLoc[1] = zero_نقاب ? -1 : maxloc % ستون_تعداد;
    }
}

قسمتعریف خالی (*getMinMaxResFunc)(ثابت Mat & db, دوہرا * minVal, دوہرا * maxVal,
                                 صحیح * minLoc, صحیح *maxLoc, صحیح gropunum, صحیح ستون_تعداد, دوہرا * maxVal2);

منطقی ocl_minMaxIdx( داخلہ_Array _src, دوہرا* minVal, دوہرا* maxVal, صحیح* minLoc, صحیح* maxLoc, داخلہ_Array نقاب_2,
                   صحیح ddepth, منطقی abs_اقدار, داخلہ_Array _src2, دوہرا * maxVal2)
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();

#اگر_تعریف_شدہ __ANDROID__
    اگر (dev.isNVidia())
        واپس جھوٹ;
#اگر_ختم

    اگر (dev.device_نسخہ_Major() == 1 && dev.device_نسخہ_Minor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        واپس جھوٹ;
    }

    منطقی doubleSupport = dev.doubleFPConfig() > 0, have_نقاب = !نقاب_2.empty(),
    haveSrc2 = _src2.قسم() != داخلہ_Array_2::NONE;
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type),
    kercn = have_نقاب ? cn : std::min(4, ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _src2));

    اگر (گہرائی >= CV_16F)
        واپس جھوٹ;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    اگر ((have_نقاب || type == CV_32FC1) && dev.isAMD())
        واپس جھوٹ;

    CV_Assert( (cn == 1 && (!have_نقاب || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    اگر (ddepth < 0)
        ddepth = گہرائی;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    اگر (گہرائی == CV_32S || گہرائی == CV_8S || گہرائی == CV_32U || گہرائی == CV_64U ||
        گہرائی == CV_64S || گہرائی == CV_16F || گہرائی == CV_16BF)
        واپس جھوٹ;

    اگر ((گہرائی == CV_64F || ddepth == CV_64F) && !doubleSupport)
        واپس جھوٹ;

    صحیح groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_گروہ_سائز();

    صحیح wgs2_aligned = 1;
    جب (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    منطقی needMinVal = minVal || minLoc, needMinLoc = minLoc != تہی,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != تہی;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    اگر (!(needMaxLoc || needMinLoc) && have_نقاب)
    {
        اگر (needMinVal)
            needMinLoc = سچ;
        ورنہ
            needMaxLoc = سچ;
    }

    حرف cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         گہرائی, ocl::type_مقصد_Str(گہرائی), have_نقاب ? " -D HAVE_MASK" : "", (صحیح)wgs,
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         نقاب_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::type_مقصد_Str(ddepth), ocl::type_مقصد_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(گہرائی, ddepth, kercn, cvt[0], حجم(cvt[0])),
                         abs_اقدار ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         گہرائی <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], حجم(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    اگر (k.empty())
        واپس جھوٹ;

    صحیح esz = CV_ELEM_سائز(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), نقاب = نقاب_2.getUMat();

    اگر (cn > 1 && !have_نقاب)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    اگر (haveSrc2)
    {
        اگر (!have_نقاب)
            k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), src.ستون_تعداد, (صحیح)src.کل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_سائز(src2));
        ورنہ
            k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), src.ستون_تعداد, (صحیح)src.کل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_سائز(نقاب),
                   ocl::KernelArg::Read_Only_No_سائز(src2));
    }
    ورنہ
    {
        اگر (!have_نقاب)
            k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), src.ستون_تعداد, (صحیح)src.کل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        ورنہ
            k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), src.ستون_تعداد, (صحیح)src.کل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_سائز(نقاب));
    }

    size_t globalsize = groupnum * wgs;
    اگر (!k.تسلسل(1, &globalsize, &wgs, سچ))
        واپس جھوٹ;

    ساکن ثابت getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<حرف>,
        getMinMaxRes<ushort>,
        getMinMaxRes<مختصر>,
        getMinMaxRes<صحیح>,
        getMinMaxRes<اعشاری>,
        getMinMaxRes<دوہرا>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    صحیح loc_عارضی[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_عارضی : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_عارضی : maxLoc,
         groupnum, src.ستون_تعداد, maxVal2);

    واپس سچ;
}

#اگر_ختم

}

// serial implementation, also used for the pieces of the parallel version
ساکن خالی minMaxIdx_(cv::داخلہ_Array _src, دوہرا* minVal,
                       دوہرا* maxVal, صحیح* minIdx, صحیح* maxIdx,
                       cv::داخلہ_Array نقاب_2)
{
    استعمال نامخلا cv;
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (نقاب_2.empty() || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool)) ||
               (cn > 1 && نقاب_2.empty() && !minIdx && !maxIdx) );

    CV_OCL_تسلسل(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (نقاب_2.empty() || _src.size() == نقاب_2.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, نقاب_2))

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();

    اگر (src.dims <= 2)
    {
        اگر ((size_t)src.قدم == (size_t)نقاب.قدم || نقاب.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.ڈیٹا, src.قدم, src.ستون_تعداد*cn, src.صفیں,
                     src.گہرائی(), minVal, maxVal, minIdx, maxIdx, نقاب.ڈیٹا);
        }
        CALL_HAL(min_Max_Idx_نقاب_قدم, cv_hal_min_Max_Idx_نقاب_قدم, src.ڈیٹا, src.قدم, src.ستون_تعداد*cn, src.صفیں,
                 src.گہرائی(), minVal, maxVal, minIdx, maxIdx, نقاب.ڈیٹا, نقاب.قدم);
    }
    ورنہ اگر (src.isContinuous() && (نقاب.isContinuous() || نقاب.empty()))
    {
        صحیح res = cv_hal_minMaxIdx(src.ڈیٹا, 0, (صحیح)src.کل()*cn, 1, src.گہرائی(),
                                   minVal, maxVal, minIdx, maxIdx, نقاب.ڈیٹا);

        اگر (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            اگر (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            اگر (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            واپس;
        }
        ورنہ اگر (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::Sts_اندرونی,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(گہرائی);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    اتحاد {
        صحیح i;
        اعشاری f;
        دوہرا d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    صحیح plane_سائز = (صحیح)it.size*cn;
    minval.L = maxval.L = 0;

    برائے( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_سائز )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_سائز, startidx );

    دوہرا dminval, dmaxval;
    اگر( گہرائی <= CV_32S || گہرائی == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    ورنہ اگر( گہرائی == CV_32F || گہرائی == CV_16F || گہرائی == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    ورنہ اگر( گہرائی == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    ورنہ اگر( گہرائی == CV_64S || گہرائی == CV_32U )
        dminval = (دوہرا)minval.L, dmaxval = (دوہرا)maxval.L;
    ورنہ {
        CV_Assert(گہرائی == CV_64U);
        dminval = (دوہرا)minval.UL, dmaxval = (دوہرا)maxval.UL;
    }

    اگر( minVal )
        *minVal = dminval;
    اگر( maxVal )
        *maxVal = dmaxval;

    اگر( minIdx )
        ofs2idx(src, minidx, minIdx);
    اگر( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

خالی cv::minMaxIdx(داخلہ_Array _src, دوہرا* minVal,
                   دوہرا* maxVal, صحیح* minIdx, صحیح* maxIdx,
                   داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    اگر (!_src.isUMat())
    {
        Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
        ثابت صحیح cn = src.channels();
        ثابت منطقی دلائل_Ok = (cn == 1 && (نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S ||
                             نقاب.type() == CV_Bool) && src.size == نقاب.size))) ||
                            (cn > 1 && نقاب.empty() && !minIdx && !maxIdx);
        اگر (دلائل_Ok && بانٹو_For_Parallel_Stat(src, نقاب, chunks))
        {
            ثابت صحیح n = (صحیح)chunks.size();
            std::vector<دوہرا> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                برائے (صحیح i = r.آغاز; i < r.end; i++)
                {
                    صحیح a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : خالی_اشاریہ, cn == 1 ? b : خالی_اشاریہ,
                               chunks[i].نقاب);
                    اگر (cn > 1)
                        imn[i] = imx[i] = 1;
                    ورنہ اگر (a[0] >= 0)
                    {
                        imn[i] = chunk_اشاریہ_مقصد_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_اشاریہ_مقصد_Elem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            صحیح bmin = -1, bmax = -1;
            برائے (صحیح i = 0; i < n; i++)
            {
                اگر (imn[i] == 0)
                    جاری;
                اگر (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                اگر (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            اگر (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            اگر (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            اگر (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            اگر (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            واپس;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, نقاب_2);
}

خالی cv::minMaxLoc( داخلہ_Array _img, دوہرا* minVal, دوہرا* maxVal,
                    نقطہ* minLoc, نقطہ* maxLoc, داخلہ_Array نقاب )
{
    CV_INSTRUMENT_REGION();

    صحیح dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (صحیح*)minLoc, (صحیح*)maxLoc, نقاب);
    اگر( minLoc) {
        اگر (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        ورنہ {
            minLoc->y = 0;
        }
    }
    اگر( maxLoc) {
        اگر (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        ورنہ {
            maxLoc->y = 0;
        }
    }
}

شمار جماعت Reduce_منوال
{
    پہلا_MIN = 0, //!< get index of first min occurrence
    آخری_MIN  = 1, //!< get index of last min occurrence
    پہلا_MAX = 2, //!< get index of first max occurrence
    آخری_MAX  = 3, //!< get index of last max occurrence
};

سانچہ <قسم_نام T>
ساخت reduceMinMaxImpl
{
    خالی عامل()(ثابت cv::Mat& src, cv::Mat& dst, Reduce_منوال منوال, ثابت صحیح axis) ثابت
    {
        انتخاب(منوال)
        {
        حالت Reduce_منوال::پہلا_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            توڑو;
        حالت Reduce_منوال::آخری_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            توڑو;
        حالت Reduce_منوال::پہلا_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            توڑو;
        حالت Reduce_منوال::آخری_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            توڑو;
        }
    }

    سانچہ <سانچہ<جماعت> جماعت Cmp>
    ساکن خالی reduceMinMaxApply(ثابت cv::Mat& src, cv::Mat& dst, ثابت صحیح axis)
    {
        Cmp<T> cmp;

        ثابت خودکار *src_ptr = src.ptr<T>();
        خودکار *dst_ptr = dst.ptr<int32_t>();

        ثابت size_t outer_سائز = src.کل(0, axis);
        ثابت خودکار mid_سائز = static_cast<size_t>(src.size[axis]);

        ثابت size_t outer_قدم = src.کل(axis);
        ثابت size_t dst_قدم = dst.کل(axis);

        ثابت size_t mid_قدم = src.کل(axis + 1);

        برائے (size_t outer = 0; outer < outer_سائز; ++outer)
        {
            ثابت size_t outer_انحراف = outer * outer_قدم;
            ثابت size_t dst_انحراف = outer * dst_قدم;
            برائے (size_t mid = 0; mid != mid_سائز; ++mid)
            {
                ثابت size_t src_انحراف = outer_انحراف + mid * mid_قدم;
                برائے (size_t inner = 0; inner < mid_قدم; inner++)
                {
                    int32_t& اشاریہ = dst_ptr[dst_انحراف + inner];

                    ثابت size_t پچھلا = outer_انحراف + اشاریہ * mid_قدم + inner;
                    ثابت size_t curr = src_انحراف + inner;

                    اگر (cmp(src_ptr[curr], src_ptr[پچھلا]))
                    {
                        اشاریہ = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ساکن خالی reduceMinMax(cv::داخلہ_Array src, cv::OutputArray dst, Reduce_منوال منوال, صحیح axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    صحیح dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<صحیح> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    اگر(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.ڈیٹا(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.سیٹ_مقصد(cv::Scalar::all(0));

    اگر (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    منطقی needs_copy = !dstMat.isContinuous();
    اگر (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::گہرائی_Dispatch<reduceMinMaxImpl>(srcMat.گہرائی(), srcMat, dstMat, منوال, axis);

    اگر (needs_copy)
    {
        dstMat.copy_مقصد(dst);
    }
}

خالی cv::reduceArgMin(داخلہ_Array src, OutputArray dst, صحیح axis, منطقی آخری_اشاریہ)
{
    reduceMinMax(src, dst, آخری_اشاریہ ? Reduce_منوال::آخری_MIN : Reduce_منوال::پہلا_MIN, axis);
}

خالی cv::reduceArgMax(داخلہ_Array src, OutputArray dst, صحیح axis, منطقی آخری_اشاریہ)
{
    reduceMinMax(src, dst, آخری_اشاریہ ? Reduce_منوال::آخری_MAX : Reduce_منوال::پہلا_MAX, axis);
}
