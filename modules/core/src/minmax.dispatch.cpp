// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "stat.hpp"
#അന്തര്ഭാവ "stat_parallel.hpp"
#അന്തര്ഭാവ "opencv2/core/detail/dispatch_helper.impl.hpp"
#അന്തര്ഭാവ <algorithm>

#അന്തര്ഭാവ "minmax.simd.hpp"
#അന്തര്ഭാവ "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv {

സ്ഥിതം MinMaxIdxFunc getMinMaxIdxFunc(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
സ്ഥിതം ശൂന്യം ofs2idx(സ്ഥിരം Mat& a, size_t ofs, പൂർണ്ണസംഖ്യ* idx)
{
    പൂർണ്ണസംഖ്യ i, d = a.dims;
    എങ്കിൽ( ofs > 0 )
    {
        ofs--;
        വേണ്ടി( i = d-1; i >= 0; i-- )
        {
            പൂർണ്ണസംഖ്യ sz = a.size[i];
            idx[i] = (പൂർണ്ണസംഖ്യ)(ofs % sz);
            ofs /= sz;
        }
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#ifdef HAVE_OPENCL

#define MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

template <typename T>
ശൂന്യം getMinMaxRes(സ്ഥിരം Mat & db, ഇരട്ട * minVal, ഇരട്ട * maxVal,
                  പൂർണ്ണസംഖ്യ* minLoc, പൂർണ്ണസംഖ്യ* maxLoc,
                  പൂർണ്ണസംഖ്യ groupnum, പൂർണ്ണസംഖ്യ cols, ഇരട്ട * maxVal2)
{
    uint index_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = index_max, maxloc = index_max;

    size_t index = 0;
    സ്ഥിരം T * minptr = ശൂന്യ, * maxptr = ശൂന്യ, * maxptr2 = ശൂന്യ;
    സ്ഥിരം uint * minlocptr = ശൂന്യ, * maxlocptr = ശൂന്യ;
    എങ്കിൽ (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        index += വലുപ്പം(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    എങ്കിൽ (maxVal || maxLoc)
    {
        maxptr = (സ്ഥിരം T *)(db.ptr() + index);
        index += വലുപ്പം(T) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    എങ്കിൽ (minLoc)
    {
        minlocptr = (സ്ഥിരം uint *)(db.ptr() + index);
        index += വലുപ്പം(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    എങ്കിൽ (maxLoc)
    {
        maxlocptr = (സ്ഥിരം uint *)(db.ptr() + index);
        index += വലുപ്പം(uint) * groupnum;
        index = alignSize(index, MINMAX_STRUCT_ALIGNMENT);
    }
    എങ്കിൽ (maxVal2)
        maxptr2 = (സ്ഥിരം T *)(db.ptr() + index);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < groupnum; i++)
    {
        എങ്കിൽ (minptr && minptr[i] <= minval)
        {
            എങ്കിൽ (minptr[i] == minval)
            {
                എങ്കിൽ (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            അല്ലെങ്കിൽ
            {
                എങ്കിൽ (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        എങ്കിൽ (maxptr && maxptr[i] >= maxval)
        {
            എങ്കിൽ (maxptr[i] == maxval)
            {
                എങ്കിൽ (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            അല്ലെങ്കിൽ
            {
                എങ്കിൽ (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        എങ്കിൽ (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    തർക്കം zero_mask = (minLoc && minloc == index_max) ||
    (maxLoc && maxloc == index_max);

    എങ്കിൽ (minVal)
        *minVal = zero_mask ? 0 : (ഇരട്ട)minval;
    എങ്കിൽ (maxVal)
        *maxVal = zero_mask ? 0 : (ഇരട്ട)maxval;
    എങ്കിൽ (maxVal2)
        *maxVal2 = zero_mask ? 0 : (ഇരട്ട)maxval2;

    എങ്കിൽ (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    എങ്കിൽ (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

തരനിർവചനം ശൂന്യം (*getMinMaxResFunc)(സ്ഥിരം Mat & db, ഇരട്ട * minVal, ഇരട്ട * maxVal,
                                 പൂർണ്ണസംഖ്യ * minLoc, പൂർണ്ണസംഖ്യ *maxLoc, പൂർണ്ണസംഖ്യ gropunum, പൂർണ്ണസംഖ്യ cols, ഇരട്ട * maxVal2);

തർക്കം ocl_minMaxIdx( നിവേശം_Array _src, ഇരട്ട* minVal, ഇരട്ട* maxVal, പൂർണ്ണസംഖ്യ* minLoc, പൂർണ്ണസംഖ്യ* maxLoc, നിവേശം_Array _mask,
                   പൂർണ്ണസംഖ്യ ddepth, തർക്കം abs_മൂല്യങ്ങൾ, നിവേശം_Array _src2, ഇരട്ട * maxVal2)
{
    സ്ഥിരം ocl::Device & dev = ocl::Device::getDefault();

#ifdef __ANDROID__
    എങ്കിൽ (dev.isNVidia())
        തിരികെ അസത്യം;
#endif

    എങ്കിൽ (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        തിരികെ അസത്യം;
    }

    തർക്കം doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != നിവേശം_Array_2::NONE;
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    എങ്കിൽ (depth >= CV_16F)
        തിരികെ അസത്യം;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    എങ്കിൽ ((haveMask || type == CV_32FC1) && dev.isAMD())
        തിരികെ അസത്യം;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    എങ്കിൽ (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    എങ്കിൽ (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        തിരികെ അസത്യം;

    എങ്കിൽ ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ groupnum = dev.maxComputeUnits();
    size_t wgs = dev.maxWorkGroupSize();

    പൂർണ്ണസംഖ്യ wgs2_aligned = 1;
    ആകുമ്പോൾ (wgs2_aligned < (പൂർണ്ണസംഖ്യ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    തർക്കം needMinVal = minVal || minLoc, needMinLoc = minLoc != ശൂന്യ,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != ശൂന്യ;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    എങ്കിൽ (!(needMaxLoc || needMinLoc) && haveMask)
    {
        എങ്കിൽ (needMinVal)
            needMinLoc = സത്യം;
        അല്ലെങ്കിൽ
            needMaxLoc = സത്യം;
    }

    അക്ഷരം cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (പൂർണ്ണസംഖ്യ)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], വലുപ്പം(cvt[0])),
                         abs_മൂല്യങ്ങൾ ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], വലുപ്പം(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ esz = CV_ELEM_SIZE(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    എങ്കിൽ (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    എങ്കിൽ (haveSrc2)
    {
        എങ്കിൽ (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (പൂർണ്ണസംഖ്യ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(src2));
        അല്ലെങ്കിൽ
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (പൂർണ്ണസംഖ്യ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask),
                   ocl::KernelArg::ReadOnlyNoSize(src2));
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (!haveMask)
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (പൂർണ്ണസംഖ്യ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        അല്ലെങ്കിൽ
            k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (പൂർണ്ണസംഖ്യ)src.total(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::ReadOnlyNoSize(mask));
    }

    size_t globalsize = groupnum * wgs;
    എങ്കിൽ (!k.run(1, &globalsize, &wgs, സത്യം))
        തിരികെ അസത്യം;

    സ്ഥിതം സ്ഥിരം getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<അക്ഷരം>,
        getMinMaxRes<ushort>,
        getMinMaxRes<short>,
        getMinMaxRes<പൂർണ്ണസംഖ്യ>,
        getMinMaxRes<ദശാംശം>,
        getMinMaxRes<ഇരട്ട>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    പൂർണ്ണസംഖ്യ locTemp[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : locTemp : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : locTemp : maxLoc,
         groupnum, src.cols, maxVal2);

    തിരികെ സത്യം;
}

#endif

}

// serial implementation, also used for the pieces of the parallel version
സ്ഥിതം ശൂന്യം minMaxIdx_(cv::നിവേശം_Array _src, ഇരട്ട* minVal,
                       ഇരട്ട* maxVal, പൂർണ്ണസംഖ്യ* minIdx, പൂർണ്ണസംഖ്യ* maxIdx,
                       cv::നിവേശം_Array _mask)
{
    using നാമാകാശ cv;
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    എങ്കിൽ (src.dims <= 2)
    {
        എങ്കിൽ ((size_t)src.step == (size_t)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.data, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.data, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.data, mask.step);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        പൂർണ്ണസംഖ്യ res = cv_hal_minMaxIdx(src.data, 0, (പൂർണ്ണസംഖ്യ)src.total()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.data);

        എങ്കിൽ (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            എങ്കിൽ (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            എങ്കിൽ (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            തിരികെ;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    സ്ഥിരം Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    സംഘം {
        പൂർണ്ണസംഖ്യ i;
        ദശാംശം f;
        ഇരട്ട d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    പൂർണ്ണസംഖ്യ planeSize = (പൂർണ്ണസംഖ്യ)it.size*cn;
    minval.L = maxval.L = 0;

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it, startidx += planeSize )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, planeSize, startidx );

    ഇരട്ട dminval, dmaxval;
    എങ്കിൽ( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_64S || depth == CV_32U )
        dminval = (ഇരട്ട)minval.L, dmaxval = (ഇരട്ട)maxval.L;
    അല്ലെങ്കിൽ {
        CV_Assert(depth == CV_64U);
        dminval = (ഇരട്ട)minval.UL, dmaxval = (ഇരട്ട)maxval.UL;
    }

    എങ്കിൽ( minVal )
        *minVal = dminval;
    എങ്കിൽ( maxVal )
        *maxVal = dmaxval;

    എങ്കിൽ( minIdx )
        ofs2idx(src, minidx, minIdx);
    എങ്കിൽ( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

ശൂന്യം cv::minMaxIdx(നിവേശം_Array _src, ഇരട്ട* minVal,
                   ഇരട്ട* maxVal, പൂർണ്ണസംഖ്യ* minIdx, പൂർണ്ണസംഖ്യ* maxIdx,
                   നിവേശം_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    എങ്കിൽ (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        സ്ഥിരം പൂർണ്ണസംഖ്യ cn = src.channels();
        സ്ഥിരം തർക്കം argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        എങ്കിൽ (argsOk && splitForParallelStat(src, mask, chunks))
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ n = (പൂർണ്ണസംഖ്യ)chunks.size();
            std::vector<ഇരട്ട> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](സ്ഥിരം Range& r)
            {
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = r.start; i < r.end; i++)
                {
                    പൂർണ്ണസംഖ്യ a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    എങ്കിൽ (cn > 1)
                        imn[i] = imx[i] = 1;
                    അല്ലെങ്കിൽ എങ്കിൽ (a[0] >= 0)
                    {
                        imn[i] = chunkIndexToElem(chunks[i], a) + 1;
                        imx[i] = chunkIndexToElem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            പൂർണ്ണസംഖ്യ bmin = -1, bmax = -1;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
            {
                എങ്കിൽ (imn[i] == 0)
                    തുടരുക;
                എങ്കിൽ (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                എങ്കിൽ (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            എങ്കിൽ (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            എങ്കിൽ (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            എങ്കിൽ (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            എങ്കിൽ (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            തിരികെ;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

ശൂന്യം cv::minMaxLoc( നിവേശം_Array _img, ഇരട്ട* minVal, ഇരട്ട* maxVal,
                    Point* minLoc, Point* maxLoc, നിവേശം_Array mask )
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (പൂർണ്ണസംഖ്യ*)minLoc, (പൂർണ്ണസംഖ്യ*)maxLoc, mask);
    എങ്കിൽ( minLoc) {
        എങ്കിൽ (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        അല്ലെങ്കിൽ {
            minLoc->y = 0;
        }
    }
    എങ്കിൽ( maxLoc) {
        എങ്കിൽ (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        അല്ലെങ്കിൽ {
            maxLoc->y = 0;
        }
    }
}

എണ്ണൽ വർഗ്ഗം ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

template <typename T>
ഘടന reduceMinMaxImpl
{
    ശൂന്യം operator()(സ്ഥിരം cv::Mat& src, cv::Mat& dst, ReduceMode mode, സ്ഥിരം പൂർണ്ണസംഖ്യ axis) സ്ഥിരം
    {
        തിരഞ്ഞെടുപ്പ്(mode)
        {
        അവസ്ഥ ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            നിർത്തുക;
        അവസ്ഥ ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            നിർത്തുക;
        അവസ്ഥ ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            നിർത്തുക;
        അവസ്ഥ ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            നിർത്തുക;
        }
    }

    template <template<വർഗ്ഗം> വർഗ്ഗം Cmp>
    സ്ഥിതം ശൂന്യം reduceMinMaxApply(സ്ഥിരം cv::Mat& src, cv::Mat& dst, സ്ഥിരം പൂർണ്ണസംഖ്യ axis)
    {
        Cmp<T> cmp;

        സ്ഥിരം auto *src_ptr = src.ptr<T>();
        auto *dst_ptr = dst.ptr<int32_t>();

        സ്ഥിരം size_t outer_size = src.total(0, axis);
        സ്ഥിരം auto mid_size = static_cast<size_t>(src.size[axis]);

        സ്ഥിരം size_t outer_step = src.total(axis);
        സ്ഥിരം size_t dst_step = dst.total(axis);

        സ്ഥിരം size_t mid_step = src.total(axis + 1);

        വേണ്ടി (size_t outer = 0; outer < outer_size; ++outer)
        {
            സ്ഥിരം size_t outer_offset = outer * outer_step;
            സ്ഥിരം size_t dst_offset = outer * dst_step;
            വേണ്ടി (size_t mid = 0; mid != mid_size; ++mid)
            {
                സ്ഥിരം size_t src_offset = outer_offset + mid * mid_step;
                വേണ്ടി (size_t inner = 0; inner < mid_step; inner++)
                {
                    int32_t& index = dst_ptr[dst_offset + inner];

                    സ്ഥിരം size_t prev = outer_offset + index * mid_step + inner;
                    സ്ഥിരം size_t curr = src_offset + inner;

                    എങ്കിൽ (cmp(src_ptr[curr], src_ptr[prev]))
                    {
                        index = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

സ്ഥിതം ശൂന്യം reduceMinMax(cv::നിവേശം_Array src, cv::OutputArray dst, ReduceMode mode, പൂർണ്ണസംഖ്യ axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    പൂർണ്ണസംഖ്യ dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<പൂർണ്ണസംഖ്യ> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    എങ്കിൽ(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.data(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    എങ്കിൽ (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    തർക്കം needs_copy = !dstMat.isContinuous();
    എങ്കിൽ (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    എങ്കിൽ (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

ശൂന്യം cv::reduceArgMin(നിവേശം_Array src, OutputArray dst, പൂർണ്ണസംഖ്യ axis, തർക്കം lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

ശൂന്യം cv::reduceArgMax(നിവേശം_Array src, OutputArray dst, പൂർണ്ണസംഖ്യ axis, തർക്കം lastIndex)
{
    reduceMinMax(src, dst, lastIndex ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
