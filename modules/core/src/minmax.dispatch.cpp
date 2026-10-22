// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "stat.hpp"
#समावेश "stat_parallel.hpp"
#समावेश "opencv2/core/detail/dispatch_helper.impl.hpp"
#समावेश <algorithm>

#समावेश "minmax.simd.hpp"
#समावेश "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

अटल MinMaxIdxFunc getMinMaxIdxFunc(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (depth),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
अटल व्योम ofs2idx(स्थिर Mat& a, माप_प्रकार ofs, पूर्णांक* idx)
{
    पूर्णांक i, d = a.dims;
    अगर( ofs > 0 )
    {
        ofs--;
        क्रम( i = d-1; i >= 0; i-- )
        {
            पूर्णांक sz = a.size[i];
            idx[i] = (पूर्णांक)(ofs % sz);
            ofs /= sz;
        }
    }
    अन्यथा
    {
        क्रम( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#अगर_घोषित HAVE_OPENCL

#घोषणा MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

ढाँचा <typename T>
व्योम getMinMaxRes(स्थिर Mat & db, द्विगुन * minVal, द्विगुन * maxVal,
                  पूर्णांक* minLoc, पूर्णांक* maxLoc,
                  पूर्णांक groupnum, पूर्णांक cols, द्विगुन * maxVal2)
{
    uint सूचकांक_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = सूचकांक_max, maxloc = सूचकांक_max;

    माप_प्रकार सूचकांक = 0;
    स्थिर T * minptr = शून्य, * maxptr = शून्य, * maxptr2 = शून्य;
    स्थिर uint * minlocptr = शून्य, * maxlocptr = शून्य;
    अगर (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        सूचकांक += माप(T) * groupnum;
        सूचकांक = align_आकार(सूचकांक, MINMAX_STRUCT_ALIGNMENT);
    }
    अगर (maxVal || maxLoc)
    {
        maxptr = (स्थिर T *)(db.ptr() + सूचकांक);
        सूचकांक += माप(T) * groupnum;
        सूचकांक = align_आकार(सूचकांक, MINMAX_STRUCT_ALIGNMENT);
    }
    अगर (minLoc)
    {
        minlocptr = (स्थिर uint *)(db.ptr() + सूचकांक);
        सूचकांक += माप(uint) * groupnum;
        सूचकांक = align_आकार(सूचकांक, MINMAX_STRUCT_ALIGNMENT);
    }
    अगर (maxLoc)
    {
        maxlocptr = (स्थिर uint *)(db.ptr() + सूचकांक);
        सूचकांक += माप(uint) * groupnum;
        सूचकांक = align_आकार(सूचकांक, MINMAX_STRUCT_ALIGNMENT);
    }
    अगर (maxVal2)
        maxptr2 = (स्थिर T *)(db.ptr() + सूचकांक);

    क्रम (पूर्णांक i = 0; i < groupnum; i++)
    {
        अगर (minptr && minptr[i] <= minval)
        {
            अगर (minptr[i] == minval)
            {
                अगर (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            अन्यथा
            {
                अगर (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        अगर (maxptr && maxptr[i] >= maxval)
        {
            अगर (maxptr[i] == maxval)
            {
                अगर (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            अन्यथा
            {
                अगर (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        अगर (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    bool zero_mask = (minLoc && minloc == सूचकांक_max) ||
    (maxLoc && maxloc == सूचकांक_max);

    अगर (minVal)
        *minVal = zero_mask ? 0 : (द्विगुन)minval;
    अगर (maxVal)
        *maxVal = zero_mask ? 0 : (द्विगुन)maxval;
    अगर (maxVal2)
        *maxVal2 = zero_mask ? 0 : (द्विगुन)maxval2;

    अगर (minLoc)
    {
        minLoc[0] = zero_mask ? -1 : minloc / cols;
        minLoc[1] = zero_mask ? -1 : minloc % cols;
    }
    अगर (maxLoc)
    {
        maxLoc[0] = zero_mask ? -1 : maxloc / cols;
        maxLoc[1] = zero_mask ? -1 : maxloc % cols;
    }
}

प्रकार व्योम (*getMinMaxResFunc)(स्थिर Mat & db, द्विगुन * minVal, द्विगुन * maxVal,
                                 पूर्णांक * minLoc, पूर्णांक *maxLoc, पूर्णांक gropunum, पूर्णांक cols, द्विगुन * maxVal2);

bool ocl_minMaxIdx( निवेश_Array _src, द्विगुन* minVal, द्विगुन* maxVal, पूर्णांक* minLoc, पूर्णांक* maxLoc, निवेश_Array _mask,
                   पूर्णांक ddepth, bool abs_मूल्यावली, निवेश_Array _src2, द्विगुन * maxVal2)
{
    स्थिर ocl::Device & dev = ocl::Device::getDefault();

#अगर_घोषित __ANDROID__
    अगर (dev.isNVidia())
        वापस false;
#पूर्ण_अगर

    अगर (dev.deviceVersionMajor() == 1 && dev.deviceVersionMinor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        वापस false;
    }

    bool doubleSupport = dev.doubleFPConfig() > 0, haveMask = !_mask.empty(),
    haveSrc2 = _src2.kind() != निवेश_Array_2::NONE;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
    kercn = haveMask ? cn : std::min(4, ocl::predictOptimalVectorWidth(_src, _src2));

    अगर (depth >= CV_16F)
        वापस false;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    अगर ((haveMask || type == CV_32FC1) && dev.isAMD())
        वापस false;

    CV_Assert( (cn == 1 && (!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    अगर (ddepth < 0)
        ddepth = depth;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    अगर (depth == CV_32S || depth == CV_8S || depth == CV_32U || depth == CV_64U ||
        depth == CV_64S || depth == CV_16F || depth == CV_16BF)
        वापस false;

    अगर ((depth == CV_64F || ddepth == CV_64F) && !doubleSupport)
        वापस false;

    पूर्णांक groupnum = dev.maxComputeUnits();
    माप_प्रकार wgs = dev.max_Work_Group_आकार();

    पूर्णांक wgs2_aligned = 1;
    जबतक (wgs2_aligned < (पूर्णांक)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    bool needMinVal = minVal || minLoc, needMinLoc = minLoc != शून्य,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != शून्य;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    अगर (!(needMaxLoc || needMinLoc) && haveMask)
    {
        अगर (needMinVal)
            needMinLoc = true;
        अन्यथा
            needMaxLoc = true;
    }

    अक्षर cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         depth, ocl::typeToStr(depth), haveMask ? " -D HAVE_MASK" : "", (पूर्णांक)wgs,
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(depth, ddepth, kercn, cvt[0], माप(cvt[0])),
                         abs_मूल्यावली ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], माप(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    अगर (k.empty())
        वापस false;

    पूर्णांक esz = CV_ELEM_आकार(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

    अगर (cn > 1 && !haveMask)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    अगर (haveSrc2)
    {
        अगर (!haveMask)
            k.args(ocl::KernelArg::Read_Only_No_आकार(src), src.cols, (पूर्णांक)src.योग(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_आकार(src2));
        अन्यथा
            k.args(ocl::KernelArg::Read_Only_No_आकार(src), src.cols, (पूर्णांक)src.योग(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_आकार(mask),
                   ocl::KernelArg::Read_Only_No_आकार(src2));
    }
    अन्यथा
    {
        अगर (!haveMask)
            k.args(ocl::KernelArg::Read_Only_No_आकार(src), src.cols, (पूर्णांक)src.योग(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        अन्यथा
            k.args(ocl::KernelArg::Read_Only_No_आकार(src), src.cols, (पूर्णांक)src.योग(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_आकार(mask));
    }

    माप_प्रकार globalsize = groupnum * wgs;
    अगर (!k.run(1, &globalsize, &wgs, true))
        वापस false;

    अटल स्थिर getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<अक्षर>,
        getMinMaxRes<ushort>,
        getMinMaxRes<लघु>,
        getMinMaxRes<पूर्णांक>,
        getMinMaxRes<भग्न>,
        getMinMaxRes<द्विगुन>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    पूर्णांक loc_अस्थायी[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_अस्थायी : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_अस्थायी : maxLoc,
         groupnum, src.cols, maxVal2);

    वापस true;
}

#पूर्ण_अगर

}

// serial implementation, also used for the pieces of the parallel version
अटल व्योम minMaxIdx_(cv::निवेश_Array _src, द्विगुन* minVal,
                       द्विगुन* maxVal, पूर्णांक* minIdx, पूर्णांक* maxIdx,
                       cv::निवेश_Array _mask)
{
    using namespace cv;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (_mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool)) ||
               (cn > 1 && _mask.empty() && !minIdx && !maxIdx) );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (_mask.empty() || _src.size() == _mask.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    अगर (src.dims <= 2)
    {
        अगर ((माप_प्रकार)src.step == (माप_प्रकार)mask.step || mask.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.तथ्य, src.step, src.cols*cn, src.rows,
                     src.depth(), minVal, maxVal, minIdx, maxIdx, mask.तथ्य);
        }
        CALL_HAL(minMaxIdxMaskStep, cv_hal_minMaxIdxMaskStep, src.तथ्य, src.step, src.cols*cn, src.rows,
                 src.depth(), minVal, maxVal, minIdx, maxIdx, mask.तथ्य, mask.step);
    }
    अन्यथा अगर (src.isContinuous() && (mask.isContinuous() || mask.empty()))
    {
        पूर्णांक res = cv_hal_minMaxIdx(src.तथ्य, 0, (पूर्णांक)src.योग()*cn, 1, src.depth(),
                                   minVal, maxVal, minIdx, maxIdx, mask.तथ्य);

        अगर (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            अगर (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            अगर (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            वापस;
        }
        अन्यथा अगर (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::StsInternal,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(depth);
    CV_Assert( func != 0 );

    स्थिर Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    माप_प्रकार minidx = 0, maxidx = 0;
    माप_प्रकार startidx = 1;
    जोड़ {
        पूर्णांक i;
        भग्न f;
        द्विगुन d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    पूर्णांक plane_आकार = (पूर्णांक)it.size*cn;
    minval.L = maxval.L = 0;

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it, startidx += plane_आकार )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_आकार, startidx );

    द्विगुन dminval, dmaxval;
    अगर( depth <= CV_32S || depth == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    अन्यथा अगर( depth == CV_32F || depth == CV_16F || depth == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    अन्यथा अगर( depth == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    अन्यथा अगर( depth == CV_64S || depth == CV_32U )
        dminval = (द्विगुन)minval.L, dmaxval = (द्विगुन)maxval.L;
    अन्यथा {
        CV_Assert(depth == CV_64U);
        dminval = (द्विगुन)minval.UL, dmaxval = (द्विगुन)maxval.UL;
    }

    अगर( minVal )
        *minVal = dminval;
    अगर( maxVal )
        *maxVal = dmaxval;

    अगर( minIdx )
        ofs2idx(src, minidx, minIdx);
    अगर( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

व्योम cv::minMaxIdx(निवेश_Array _src, द्विगुन* minVal,
                   द्विगुन* maxVal, पूर्णांक* minIdx, पूर्णांक* maxIdx,
                   निवेश_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    अगर (!_src.isUMat())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        स्थिर पूर्णांक cn = src.channels();
        स्थिर bool argsOk = (cn == 1 && (mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S ||
                             mask.type() == CV_Bool) && src.size == mask.size))) ||
                            (cn > 1 && mask.empty() && !minIdx && !maxIdx);
        अगर (argsOk && splitForParallelStat(src, mask, chunks))
        {
            स्थिर पूर्णांक n = (पूर्णांक)chunks.size();
            std::vector<द्विगुन> mn(n), mx(n);
            std::vector<माप_प्रकार> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](स्थिर Range& r)
            {
                क्रम (पूर्णांक i = r.start; i < r.end; i++)
                {
                    पूर्णांक a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : nullptr, cn == 1 ? b : nullptr,
                               chunks[i].mask);
                    अगर (cn > 1)
                        imn[i] = imx[i] = 1;
                    अन्यथा अगर (a[0] >= 0)
                    {
                        imn[i] = chunk_सूचकांक_To_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_सूचकांक_To_Elem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            पूर्णांक bmin = -1, bmax = -1;
            क्रम (पूर्णांक i = 0; i < n; i++)
            {
                अगर (imn[i] == 0)
                    जारी;
                अगर (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                अगर (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            अगर (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            अगर (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            अगर (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            अगर (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            वापस;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, _mask);
}

व्योम cv::minMaxLoc( निवेश_Array _img, द्विगुन* minVal, द्विगुन* maxVal,
                    Point* minLoc, Point* maxLoc, निवेश_Array mask )
{
    CV_INSTRUMENT_REGION();

    पूर्णांक dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (पूर्णांक*)minLoc, (पूर्णांक*)maxLoc, mask);
    अगर( minLoc) {
        अगर (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        अन्यथा {
            minLoc->y = 0;
        }
    }
    अगर( maxLoc) {
        अगर (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        अन्यथा {
            maxLoc->y = 0;
        }
    }
}

क्रमागत श्रेणी ReduceMode
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    LAST_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    LAST_MAX  = 3, //!< get index of last max occurrence
};

ढाँचा <typename T>
काष्ठा reduceMinMaxImpl
{
    व्योम चालक()(स्थिर cv::Mat& src, cv::Mat& dst, ReduceMode mode, स्थिर पूर्णांक axis) स्थिर
    {
        चयन(mode)
        {
        हाल ReduceMode::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            अवरोध;
        हाल ReduceMode::LAST_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            अवरोध;
        हाल ReduceMode::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            अवरोध;
        हाल ReduceMode::LAST_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            अवरोध;
        }
    }

    ढाँचा <ढाँचा<श्रेणी> श्रेणी Cmp>
    अटल व्योम reduceMinMaxApply(स्थिर cv::Mat& src, cv::Mat& dst, स्थिर पूर्णांक axis)
    {
        Cmp<T> cmp;

        स्थिर स्वतः *src_ptr = src.ptr<T>();
        स्वतः *dst_ptr = dst.ptr<int32_t>();

        स्थिर माप_प्रकार outer_आकार = src.योग(0, axis);
        स्थिर स्वतः mid_आकार = static_cast<माप_प्रकार>(src.size[axis]);

        स्थिर माप_प्रकार outer_step = src.योग(axis);
        स्थिर माप_प्रकार dst_step = dst.योग(axis);

        स्थिर माप_प्रकार mid_step = src.योग(axis + 1);

        क्रम (माप_प्रकार outer = 0; outer < outer_आकार; ++outer)
        {
            स्थिर माप_प्रकार outer_offset = outer * outer_step;
            स्थिर माप_प्रकार dst_offset = outer * dst_step;
            क्रम (माप_प्रकार mid = 0; mid != mid_आकार; ++mid)
            {
                स्थिर माप_प्रकार src_offset = outer_offset + mid * mid_step;
                क्रम (माप_प्रकार inner = 0; inner < mid_step; inner++)
                {
                    int32_t& सूचकांक = dst_ptr[dst_offset + inner];

                    स्थिर माप_प्रकार पूर्व = outer_offset + सूचकांक * mid_step + inner;
                    स्थिर माप_प्रकार curr = src_offset + inner;

                    अगर (cmp(src_ptr[curr], src_ptr[पूर्व]))
                    {
                        सूचकांक = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

अटल व्योम reduceMinMax(cv::निवेश_Array src, cv::OutputArray dst, ReduceMode mode, पूर्णांक axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    पूर्णांक dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<पूर्णांक> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    अगर(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.तथ्य(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.setTo(cv::Scalar::all(0));

    अगर (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    bool needs_copy = !dstMat.isContinuous();
    अगर (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::depthDispatch<reduceMinMaxImpl>(srcMat.depth(), srcMat, dstMat, mode, axis);

    अगर (needs_copy)
    {
        dstMat.copyTo(dst);
    }
}

व्योम cv::reduceArgMin(निवेश_Array src, OutputArray dst, पूर्णांक axis, bool last_सूचकांक)
{
    reduceMinMax(src, dst, last_सूचकांक ? ReduceMode::LAST_MIN : ReduceMode::FIRST_MIN, axis);
}

व्योम cv::reduceArgMax(निवेश_Array src, OutputArray dst, पूर्णांक axis, bool last_सूचकांक)
{
    reduceMinMax(src, dst, last_सूचकांक ? ReduceMode::LAST_MAX : ReduceMode::FIRST_MAX, axis);
}
