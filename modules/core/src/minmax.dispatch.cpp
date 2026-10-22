// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "stat.hpp"
#અન્તર્ભાવ "stat_parallel.hpp"
#અન્તર્ભાવ "opencv2/core/detail/dispatch_helper.impl.hpp"
#અન્તર્ભાવ <algorithm>

#અન્તર્ભાવ "minmax.simd.hpp"
#અન્તર્ભાવ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv {

સ્થિર MinMaxIdxFunc getMinMaxIdxFunc(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
સ્થિર શૂન્ય ofs2idx(અચલ Mat& a, size_t ofs, પૂર્ણાંક* idx)
{
    પૂર્ણાંક i, d = a.dims;
    જો( ofs > 0 )
    {
        ofs--;
        માટે( i = d-1; i >= 0; i-- )
        {
            પૂર્ણાંક sz = a.size[i];
            idx[i] = (પૂર્ણાંક)(ofs % sz);
            ofs /= sz;
        }
    }
    નહીંતર
    {
        માટે( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
શૂન્ય getMinMaxRes(અચલ Mat & db, દ્વિગુણ * minVal, દ્વિગુણ * maxVal,
                  પૂર્ણાંક* minLoc, પૂર્ણાંક* maxLoc,
                  પૂર્ણાંક groupnum, પૂર્ણાંક cols, દ્વિગુણ * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    અચલ T * minptr = ખાલી, * maxptr = ખાલી, * maxptr2 = ખાલી;
    અચલ uint * minlocptr = ખાલી, * maxlocptr = ખાલી;
    જો (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += કદ(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    જો (maxVal || maxLoc)
    {
        maxptr = (અચલ T *)(db.ptr() + index);
        index += કદ(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    જો (minLoc)
    {
        minlocptr = (અચલ uint *)(db.ptr() + index);
        index += કદ(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    જો (maxLoc)
    {
        maxlocptr = (અચલ uint *)(db.ptr() + index);
        index += કદ(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    જો (maxVal2)
        maxptr2 = (અચલ T *)(db.ptr() + index);

    માટે (પૂર્ણાંક i = 0; i < groupnum; i++)
    {
        જો (minptr && minptr[i] <= minval)
        {
            જો (minptr[i] == minval)
            {
                જો (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            નહીંતર
            {
                જો (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        જો (maxptr && maxptr[i] >= maxval)
        {
            જો (maxptr[i] == maxval)
            {
                જો (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            નહીંતર
            {
                જો (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        જો (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    તાર્કિક zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    જો (minVal)
        *minVal = zero_mask ? 0 : (દ્વિગુણ)minval;
    જો (maxVal)
        *maxVal = zero_mask ? 0 : (દ્વિગુણ)maxval;
    જો (maxVal2)
        *maxVal2 = zero_mask ? 0 : (દ્વિગુણ)maxval2;

    જો (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    જો (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

પ્રકારવ્યાખ્યા શૂન્ય (*getMinMaxResFunc)(અચલ Mat & db, દ્વિગુણ * minVal, દ્વિગુણ * maxVal,
                                 પૂર્ણાંક * minLoc, પૂર્ણાંક *maxLoc, પૂર્ણાંક gropunum, પૂર્ણાંક cols, દ્વિગુણ * maxVal2);

તાર્કિક ocl_minMaxIdx( આગત_Array _src, દ્વિગુણ* minVal, દ્વિગુણ* maxVal, પૂર્ણાંક* minLoc, પૂર્ણાંક* maxLoc, આગત_Array _mask,
                   પૂર્ણાંક ddepth, તાર્કિક abs_કિંમતો, આગત_Array _src2, દ્વિગુણ * maxVal2)
{
    અચલ ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    જો (dev.isNVidia())
        પાછા ખોટું;
#endif

    જો (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        પાછા ખોટું;
    }

    તાર્કિક doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != આગત_Array_2::NONE;
    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    જો (depth >= CV_16F)
        પાછા ખોટું;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    જો ((haveMask || type == CV_32FC1) && dev.isAMD())
        પાછા ખોટું;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    જો (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    જો (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        પાછા ખોટું;

    જો ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        પાછા ખોટું;

    પૂર્ણાંક groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    પૂર્ણાંક wgs2_aligned = 1;
    જ્યારે (wgs2_aligned < (પૂર્ણાંક)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    તાર્કિક needMinVal = minVal || minLoc, needMinLoc = minLoc != ખાલી,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != ખાલી;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    જો (!(needMaxLoc || needMinLoc) && haveMask)
    {
        જો (needMinVal)
            needMinLoc = સાચું;
        નહીંતર
            needMaxLoc = સાચું;
    }

    અક્ષર cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (પૂર્ણાંક)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], કદ(cvt[0])),
                         abs_કિંમતો ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], કદ(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    જો (k.empty())
        પાછા ખોટું;

    પૂર્ણાંક esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    જો (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    જો (haveSrc2)
    {
        જો (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (પૂર્ણાંક)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        નહીંતર
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (પૂર્ણાંક)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    નહીંતર
    {
        જો (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (પૂર્ણાંક)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        નહીંતર
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (પૂર્ણાંક)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    જો (!k.run(1, &globalsize, &wgs, સાચું))
        પાછા ખોટું;

    સ્થિર અચલ getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<અક્ષર>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<પૂર્ણાંક>,
        getMinMaxRes<દશાંશ>,
        getMinMaxRes<દ્વિગુણ>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    પૂર્ણાંક locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    પાછા સાચું;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
સ્થિર શૂન્ય minMaxIdx_(cv::આગત_Array _src, દ્વિગુણ* minVal,
                       દ્વિગુણ* maxVal, પૂર્ણાંક* minIdx, પૂર્ણાંક* maxIdx,
                       cv::આગત_Array _mask)
{
    using નામાકાશ cv;
    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    જો (src.dims <= 2)
    {
        જો ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    નહીંતર જો (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        પૂર્ણાંક res = cv_hal_minMaxIdx(src.data, 0, (પૂર્ણાંક)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        જો (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            જો (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            જો (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            પાછા;
        }
        નહીંતર જો (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    અચલ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    સંઘ {
        પૂર્ણાંક i;
        દશાંશ f;
        દ્વિગુણ d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    પૂર્ણાંક planeSize = (પૂર્ણાંક)it.size*cn;
    minval.L = maxval.L = 0;

    માટે( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    દ્વિગુણ dminval, dmaxval;
    જો( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    નહીંતર જો( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    નહીંતર જો( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    નહીંતર જો( depth == CV_64S || depth == CV_32U )
        dminval = (દ્વિગુણ)minval.L, dmaxval = (દ્વિગુણ)maxval.L;
    નહીંતર {
        CV_Assert(depth == CV_64U);
        dminval = (દ્વિગુણ)minval.UL, dmaxval = (દ્વિગુણ)maxval.UL;
    }

    જો( minVal )
        *minVal = dminval;
    જો( maxVal )
        *maxVal = dmaxval;

    જો( minIdx )
        ofs2idx(src, minidx, minIdx);
    જો( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

શૂન્ય cv::minMaxIdx(આગત_Array _src, દ્વિગુણ* minVal,
                   દ્વિગુણ* maxVal, પૂર્ણાંક* minIdx, પૂર્ણાંક* maxIdx,
                   આગત_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    જો (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        અચલ પૂર્ણાંક cn = src.channels();
        અચલ તાર્કિક argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        જો (argsOk && splitForParallelStat(src, mask, chunks))
        {
            અચલ પૂર્ણાંક n = (પૂર્ણાંક)chunks.size();
            std::vector<દ્વિગુણ> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](અચલ Range& r)
            {
                માટે (પૂર્ણાંક i = r.start; i < r.end; i++)
                {
                    પૂર્ણાંક a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    જો (cn > 1)
                        imn[i] = imx[i] = 1;
                    નહીંતર જો (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            પૂર્ણાંક bmin = -1, bmax = -1;
            માટે (પૂર્ણાંક i = 0; i < n; i++)
            {
                જો (imn[i] == 0)
                    ચાલુ;
                જો (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                જો (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            જો (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            જો (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            જો (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            જો (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            પાછા;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

શૂન્ય cv::minMaxLoc( આગત_Array _img, દ્વિગુણ* minVal, દ્વિગુણ* maxVal,
                    Point* minLoc, Point* maxLoc, આગત_Array mask )
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (પૂર્ણાંક*)minLoc, (પૂર્ણાંક*)maxLoc, mask);
    જો( minLoc) {
        જો (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        નહીંતર {
            minLoc->y = 0;
        }
    }
    જો( maxLoc) {
        જો (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        નહીંતર {
            maxLoc->y = 0;
        }
    }
}

ગણના વર્ગ ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
સંરચના reduceMinMaxImpl
{
    શૂન્ય operator()(અચલ cv::Mat& src, cv::Mat& dst, ReduceMode mode, અચલ પૂર્ણાંક axis) અચલ
    {
        પસંદગી(mode)
        {
        સ્થિતિ ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            તોડો;
        સ્થિતિ ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            તોડો;
        સ્થિતિ ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            તોડો;
        સ્થિતિ ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            તોડો;
        }
    }

    template <template<વર્ગ> વર્ગ Cmp>
    સ્થિર શૂન્ય reduceMinMaxApply(અચલ cv::Mat& src, cv::Mat& dst, અચલ પૂર્ણાંક axis)
    {
        Cmp<T> cmp;

        અચલ auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        અચલ size_t outer_size = src.total(0, axis);
        અચલ auto mid_size = static_cast<size_t>(src.size[axis]);

        અચલ size_t outer_step = src.total(axis);
        અચલ size_t dst_step = dst.total(axis);

        અચલ size_t mid_step = src.total(axis + 1);

        માટે (size_t outer = 0; outer < outer_size; ++outer)
        {
            અચલ size_t outer_offset = outer * outer_step;
            અચલ size_t dst_offset = outer * dst_step;
            માટે (size_t mid = 0; mid != mid_size; ++mid)
            {
                અચલ size_t src_offset = outer_offset + mid * mid_step;
                માટે (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    અચલ size_t prev = outer_offset + index * mid_step + inner;
                    અચલ size_t curr = src_offset + inner;

                    જો (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

સ્થિર શૂન્ય reduceMinMax(cv::આગત_Array src, cv::OutputArray dst, ReduceMode mode, પૂર્ણાંક axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    પૂર્ણાંક dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<પૂર્ણાંક> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    જો(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    જો (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    તાર્કિક needs_copy = !dstMat.isContinuous();
    જો (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    જો (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

શૂન્ય cv::reduceArgMin(આગત_Array src, OutputArray dst, પૂર્ણાંક axis, તાર્કિક lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

શૂન્ય cv::reduceArgMax(આગત_Array src, OutputArray dst, પૂર્ણાંક axis, તાર્કિક lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
