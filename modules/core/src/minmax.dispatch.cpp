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

ساكن MinMaxIdxFunc getMinMaxIdxFunc(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (ڈونگھائی),
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

#جے_تعریف_شدہ HAVE_OPENCL

#تعریف_کرو MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

سانچا <قسم_ناں T>
فراغ getMinMaxRes(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                  صحيح* minLoc, صحيح* maxLoc,
                  صحيح groupnum, صحيح خانے_گنتی, مزدوج * maxVal2)
{
    uint اشاریہ_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = اشاریہ_max, maxloc = اشاریہ_max;

    size_t اشاریہ = 0;
    ثابت T * minptr = عدم, * maxptr = عدم, * maxptr2 = عدم;
    ثابت uint * minlocptr = عدم, * maxlocptr = عدم;
    إذا (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        اشاریہ += حجم(T) * groupnum;
        اشاریہ = align_ناپ(اشاریہ, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxVal || maxLoc)
    {
        maxptr = (ثابت T *)(db.ptr() + اشاریہ);
        اشاریہ += حجم(T) * groupnum;
        اشاریہ = align_ناپ(اشاریہ, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (minLoc)
    {
        minlocptr = (ثابت uint *)(db.ptr() + اشاریہ);
        اشاریہ += حجم(uint) * groupnum;
        اشاریہ = align_ناپ(اشاریہ, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxLoc)
    {
        maxlocptr = (ثابت uint *)(db.ptr() + اشاریہ);
        اشاریہ += حجم(uint) * groupnum;
        اشاریہ = align_ناپ(اشاریہ, MINMAX_STRUCT_ALIGNMENT);
    }
    إذا (maxVal2)
        maxptr2 = (ثابت T *)(db.ptr() + اشاریہ);

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
    منطقي zero_نقاب = (minLoc && minloc == اشاریہ_max) ||
    (maxLoc && maxloc == اشاریہ_max);

    إذا (minVal)
        *minVal = zero_نقاب ? 0 : (مزدوج)minval;
    إذا (maxVal)
        *maxVal = zero_نقاب ? 0 : (مزدوج)maxval;
    إذا (maxVal2)
        *maxVal2 = zero_نقاب ? 0 : (مزدوج)maxval2;

    إذا (minLoc)
    {
        minLoc[0] = zero_نقاب ? -1 : minloc / خانے_گنتی;
        minLoc[1] = zero_نقاب ? -1 : minloc % خانے_گنتی;
    }
    إذا (maxLoc)
    {
        maxLoc[0] = zero_نقاب ? -1 : maxloc / خانے_گنتی;
        maxLoc[1] = zero_نقاب ? -1 : maxloc % خانے_گنتی;
    }
}

تعريفنوع فراغ (*getMinMaxResFunc)(ثابت Mat & db, مزدوج * minVal, مزدوج * maxVal,
                                 صحيح * minLoc, صحيح *maxLoc, صحيح gropunum, صحيح خانے_گنتی, مزدوج * maxVal2);

منطقي ocl_minMaxIdx( داخلہ_Array _src, مزدوج* minVal, مزدوج* maxVal, صحيح* minLoc, صحيح* maxLoc, داخلہ_Array نقاب_2,
                   صحيح ddepth, منطقي abs_اقدار, داخلہ_Array _src2, مزدوج * maxVal2)
{
    ثابت ocl::Device & dev = ocl::Device::getDefault();

#جے_تعریف_شدہ __ANDROID__
    إذا (dev.isNVidia())
        أرجع خطأ;
#جے_انت

    إذا (dev.device_نسخہ_Major() == 1 && dev.device_نسخہ_Minor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        أرجع خطأ;
    }

    منطقي doubleSupport = dev.doubleFPConfig() > 0, have_نقاب = !نقاب_2.empty(),
    haveSrc2 = _src2.قسم() != داخلہ_Array_2::NONE;
    صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type),
    kercn = have_نقاب ? cn : std::min(4, ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _src2));

    إذا (ڈونگھائی >= CV_16F)
        أرجع خطأ;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    إذا ((have_نقاب || type == CV_32FC1) && dev.isAMD())
        أرجع خطأ;

    CV_Assert( (cn == 1 && (!have_نقاب || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    إذا (ddepth < 0)
        ddepth = ڈونگھائی;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    إذا (ڈونگھائی == CV_32S || ڈونگھائی == CV_8S || ڈونگھائی == CV_32U || ڈونگھائی == CV_64U ||
        ڈونگھائی == CV_64S || ڈونگھائی == CV_16F || ڈونگھائی == CV_16BF)
        أرجع خطأ;

    إذا ((ڈونگھائی == CV_64F || ddepth == CV_64F) && !doubleSupport)
        أرجع خطأ;

    صحيح groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_گروہ_ناپ();

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
                         ڈونگھائی, ocl::type_منزل_Str(ڈونگھائی), have_نقاب ? " -D HAVE_MASK" : "", (صحيح)wgs,
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         نقاب_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::type_منزل_Str(ddepth), ocl::type_منزل_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(ڈونگھائی, ddepth, kercn, cvt[0], حجم(cvt[0])),
                         abs_اقدار ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         ڈونگھائی <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], حجم(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    صحيح esz = CV_ELEM_ناپ(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
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
            k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(src), src.خانے_گنتی, (صحيح)src.کل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ناپ(src2));
        وإلا
            k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(src), src.خانے_گنتی, (صحيح)src.کل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ناپ(نقاب),
                   ocl::KernelArg::Read_Only_No_ناپ(src2));
    }
    وإلا
    {
        إذا (!have_نقاب)
            k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(src), src.خانے_گنتی, (صحيح)src.کل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        وإلا
            k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(src), src.خانے_گنتی, (صحيح)src.کل(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_ناپ(نقاب));
    }

    size_t globalsize = groupnum * wgs;
    إذا (!k.لگاتار(1, &globalsize, &wgs, سچ))
        أرجع خطأ;

    ساكن ثابت getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<حرف>,
        getMinMaxRes<ushort>,
        getMinMaxRes<چھوٹا>,
        getMinMaxRes<صحيح>,
        getMinMaxRes<عائم>,
        getMinMaxRes<مزدوج>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    صحيح loc_عارضی[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_عارضی : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_عارضی : maxLoc,
         groupnum, src.خانے_گنتی, maxVal2);

    أرجع سچ;
}

#جے_انت

}

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ minMaxIdx_(cv::داخلہ_Array _src, مزدوج* minVal,
                       مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                       cv::داخلہ_Array نقاب_2)
{
    ورتوں نطاق cv;
    صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (نقاب_2.empty() || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool)) ||
               (cn > 1 && نقاب_2.empty() && !minIdx && !maxIdx) );

    CV_OCL_لگاتار(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (نقاب_2.empty() || _src.size() == نقاب_2.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, نقاب_2))

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();

    إذا (src.dims <= 2)
    {
        إذا ((size_t)src.قدم == (size_t)نقاب.قدم || نقاب.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.انکڑے, src.قدم, src.خانے_گنتی*cn, src.صفاں,
                     src.ڈونگھائی(), minVal, maxVal, minIdx, maxIdx, نقاب.انکڑے);
        }
        CALL_HAL(min_Max_Idx_نقاب_قدم, cv_hal_min_Max_Idx_نقاب_قدم, src.انکڑے, src.قدم, src.خانے_گنتی*cn, src.صفاں,
                 src.ڈونگھائی(), minVal, maxVal, minIdx, maxIdx, نقاب.انکڑے, نقاب.قدم);
    }
    وإلا إذا (src.isContinuous() && (نقاب.isContinuous() || نقاب.empty()))
    {
        صحيح res = cv_hal_minMaxIdx(src.انکڑے, 0, (صحيح)src.کل()*cn, 1, src.ڈونگھائی(),
                                   minVal, maxVal, minIdx, maxIdx, نقاب.انکڑے);

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
            CV_Error_(cv::Error::Sts_اندرونی,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(ڈونگھائی);
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
    صحيح plane_ناپ = (صحيح)it.size*cn;
    minval.L = maxval.L = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_ناپ )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_ناپ, startidx );

    مزدوج dminval, dmaxval;
    إذا( ڈونگھائی <= CV_32S || ڈونگھائی == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    وإلا إذا( ڈونگھائی == CV_32F || ڈونگھائی == CV_16F || ڈونگھائی == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    وإلا إذا( ڈونگھائی == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    وإلا إذا( ڈونگھائی == CV_64S || ڈونگھائی == CV_32U )
        dminval = (مزدوج)minval.L, dmaxval = (مزدوج)maxval.L;
    وإلا {
        CV_Assert(ڈونگھائی == CV_64U);
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

فراغ cv::minMaxIdx(داخلہ_Array _src, مزدوج* minVal,
                   مزدوج* maxVal, صحيح* minIdx, صحيح* maxIdx,
                   داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat())
    {
        Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي دلیلاں_Ok = (cn == 1 && (نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S ||
                             نقاب.type() == CV_Bool) && src.size == نقاب.size))) ||
                            (cn > 1 && نقاب.empty() && !minIdx && !maxIdx);
        إذا (دلیلاں_Ok && ونڈو_For_Parallel_Stat(src, نقاب, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<مزدوج> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.شروع; i < r.end; i++)
                {
                    صحيح a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : خالی_سوچک, cn == 1 ? b : خالی_سوچک,
                               chunks[i].نقاب);
                    إذا (cn > 1)
                        imn[i] = imx[i] = 1;
                    وإلا إذا (a[0] >= 0)
                    {
                        imn[i] = chunk_اشاریہ_منزل_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_اشاریہ_منزل_Elem(chunks[i], b) + 1;
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

فراغ cv::minMaxLoc( داخلہ_Array _img, مزدوج* minVal, مزدوج* maxVal,
                    نقطہ* minLoc, نقطہ* maxLoc, داخلہ_Array نقاب )
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

تعداد صنف Reduce_منوال
{
    پہلا_MIN = 0, //!< get index of first min occurrence
    آخری_MIN  = 1, //!< get index of last min occurrence
    پہلا_MAX = 2, //!< get index of first max occurrence
    آخری_MAX  = 3, //!< get index of last max occurrence
};

سانچا <قسم_ناں T>
بنية reduceMinMaxImpl
{
    فراغ عامل()(ثابت cv::Mat& src, cv::Mat& dst, Reduce_منوال منوال, ثابت صحيح axis) ثابت
    {
        بدل(منوال)
        {
        حالة Reduce_منوال::پہلا_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            اكسر;
        حالة Reduce_منوال::آخری_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            اكسر;
        حالة Reduce_منوال::پہلا_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            اكسر;
        حالة Reduce_منوال::آخری_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            اكسر;
        }
    }

    سانچا <سانچا<صنف> صنف Cmp>
    ساكن فراغ reduceMinMaxApply(ثابت cv::Mat& src, cv::Mat& dst, ثابت صحيح axis)
    {
        Cmp<T> cmp;

        ثابت خودکار *src_ptr = src.ptr<T>();
        خودکار *dst_ptr = dst.ptr<int32_t>();

        ثابت size_t outer_ناپ = src.کل(0, axis);
        ثابت خودکار mid_ناپ = static_cast<size_t>(src.size[axis]);

        ثابت size_t outer_قدم = src.کل(axis);
        ثابت size_t dst_قدم = dst.کل(axis);

        ثابت size_t mid_قدم = src.کل(axis + 1);

        لكل (size_t outer = 0; outer < outer_ناپ; ++outer)
        {
            ثابت size_t outer_وتھ = outer * outer_قدم;
            ثابت size_t dst_وتھ = outer * dst_قدم;
            لكل (size_t mid = 0; mid != mid_ناپ; ++mid)
            {
                ثابت size_t src_وتھ = outer_وتھ + mid * mid_قدم;
                لكل (size_t inner = 0; inner < mid_قدم; inner++)
                {
                    int32_t& اشاریہ = dst_ptr[dst_وتھ + inner];

                    ثابت size_t prev = outer_وتھ + اشاریہ * mid_قدم + inner;
                    ثابت size_t curr = src_وتھ + inner;

                    إذا (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        اشاریہ = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

ساكن فراغ reduceMinMax(cv::داخلہ_Array src, cv::OutputArray dst, Reduce_منوال منوال, صحيح axis)
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

    dst.create(srcMat.dims, sizes.انکڑے(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.سیٹ_منزل(cv::Scalar::all(0));

    إذا (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    منطقي needs_copy = !dstMat.isContinuous();
    إذا (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::ڈونگھائی_Dispatch<reduceMinMaxImpl>(srcMat.ڈونگھائی(), srcMat, dstMat, منوال, axis);

    إذا (needs_copy)
    {
        dstMat.copy_منزل(dst);
    }
}

فراغ cv::reduceArgMin(داخلہ_Array src, OutputArray dst, صحيح axis, منطقي آخری_اشاریہ)
{
    reduceMinMax(src, dst, آخری_اشاریہ ? Reduce_منوال::آخری_MIN : Reduce_منوال::پہلا_MIN, axis);
}

فراغ cv::reduceArgMax(داخلہ_Array src, OutputArray dst, صحيح axis, منطقي آخری_اشاریہ)
{
    reduceMinMax(src, dst, آخری_اشاریہ ? Reduce_منوال::آخری_MAX : Reduce_منوال::پہلا_MAX, axis);
}
