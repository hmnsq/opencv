// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"
#अन्तर्भाव "stat_parallel.hpp"
#अन्तर्भाव "opencv2/core/detail/dispatch_helper.impl.hpp"
#अन्तर्भाव <algorithm>

#अन्तर्भाव "minmax.simd.hpp"
#अन्तर्भाव "minmax.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

स्थिर MinMaxIdxFunc getMinMaxIdxFunc(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMinMaxIdxFunc, (गभीरता),
                    CV_CPU_DISPATCH_MODES_ALL);
}

// The function expects 1-based indexing for ofs
// Zero is treated as invalid offset (not found)
स्थिर शून्य ofs2idx(ध्रुव Mat& a, size_t ofs, पूर्णाङ्क* idx)
{
    पूर्णाङ्क i, d = a.dims;
    यदि( ofs > 0 )
    {
        ofs--;
        प्रति( i = d-1; i >= 0; i-- )
        {
            पूर्णाङ्क sz = a.size[i];
            idx[i] = (पूर्णाङ्क)(ofs % sz);
            ofs /= sz;
        }
    }
    अन्यथा
    {
        प्रति( i = d-1; i >= 0; i-- )
            idx[i] = -1;
    }
}

#यदि_परिभाषितम् HAVE_OPENCL

#परिभाषय MINMAX_STRUCT_ALIGNMENT 8 // sizeof double

प्रतिरूपम् <प्रकारनाम T>
शून्य getMinMaxRes(ध्रुव Mat & db, द्विगुण * minVal, द्विगुण * maxVal,
                  पूर्णाङ्क* minLoc, पूर्णाङ्क* maxLoc,
                  पूर्णाङ्क groupnum, पूर्णाङ्क स्तम्भसङ्ख्या, द्विगुण * maxVal2)
{
    uint सूचकांक_max = std::numeric_limits<uint>::max();
    T minval = std::numeric_limits<T>::max();
    T maxval = std::numeric_limits<T>::min() > 0 ? (T)-std::numeric_limits<T>::max() : std::numeric_limits<T>::min(), maxval2 = maxval;
    uint minloc = सूचकांक_max, maxloc = सूचकांक_max;

    size_t सूचकांक = 0;
    ध्रुव T * minptr = रिक्त, * maxptr = रिक्त, * maxptr2 = रिक्त;
    ध्रुव uint * minlocptr = रिक्त, * maxlocptr = रिक्त;
    यदि (minVal || minLoc)
    {
        minptr = db.ptr<T>();
        सूचकांक += परिमाण(T) * groupnum;
        सूचकांक = align_आकार(सूचकांक, MINMAX_STRUCT_ALIGNMENT);
    }
    यदि (maxVal || maxLoc)
    {
        maxptr = (ध्रुव T *)(db.ptr() + सूचकांक);
        सूचकांक += परिमाण(T) * groupnum;
        सूचकांक = align_आकार(सूचकांक, MINMAX_STRUCT_ALIGNMENT);
    }
    यदि (minLoc)
    {
        minlocptr = (ध्रुव uint *)(db.ptr() + सूचकांक);
        सूचकांक += परिमाण(uint) * groupnum;
        सूचकांक = align_आकार(सूचकांक, MINMAX_STRUCT_ALIGNMENT);
    }
    यदि (maxLoc)
    {
        maxlocptr = (ध्रुव uint *)(db.ptr() + सूचकांक);
        सूचकांक += परिमाण(uint) * groupnum;
        सूचकांक = align_आकार(सूचकांक, MINMAX_STRUCT_ALIGNMENT);
    }
    यदि (maxVal2)
        maxptr2 = (ध्रुव T *)(db.ptr() + सूचकांक);

    प्रति (पूर्णाङ्क i = 0; i < groupnum; i++)
    {
        यदि (minptr && minptr[i] <= minval)
        {
            यदि (minptr[i] == minval)
            {
                यदि (minlocptr)
                    minloc = std::min(minlocptr[i], minloc);
            }
            अन्यथा
            {
                यदि (minlocptr)
                    minloc = minlocptr[i];
                minval = minptr[i];
            }
        }
        यदि (maxptr && maxptr[i] >= maxval)
        {
            यदि (maxptr[i] == maxval)
            {
                यदि (maxlocptr)
                    maxloc = std::min(maxlocptr[i], maxloc);
            }
            अन्यथा
            {
                यदि (maxlocptr)
                    maxloc = maxlocptr[i];
                maxval = maxptr[i];
            }
        }
        यदि (maxptr2 && maxptr2[i] > maxval2)
            maxval2 = maxptr2[i];
    }
    तार्किक zero_आवरणम् = (minLoc && minloc == सूचकांक_max) ||
    (maxLoc && maxloc == सूचकांक_max);

    यदि (minVal)
        *minVal = zero_आवरणम् ? 0 : (द्विगुण)minval;
    यदि (maxVal)
        *maxVal = zero_आवरणम् ? 0 : (द्विगुण)maxval;
    यदि (maxVal2)
        *maxVal2 = zero_आवरणम् ? 0 : (द्विगुण)maxval2;

    यदि (minLoc)
    {
        minLoc[0] = zero_आवरणम् ? -1 : minloc / स्तम्भसङ्ख्या;
        minLoc[1] = zero_आवरणम् ? -1 : minloc % स्तम्भसङ्ख्या;
    }
    यदि (maxLoc)
    {
        maxLoc[0] = zero_आवरणम् ? -1 : maxloc / स्तम्भसङ्ख्या;
        maxLoc[1] = zero_आवरणम् ? -1 : maxloc % स्तम्भसङ्ख्या;
    }
}

प्रकारनिर्देश शून्य (*getMinMaxResFunc)(ध्रुव Mat & db, द्विगुण * minVal, द्विगुण * maxVal,
                                 पूर्णाङ्क * minLoc, पूर्णाङ्क *maxLoc, पूर्णाङ्क gropunum, पूर्णाङ्क स्तम्भसङ्ख्या, द्विगुण * maxVal2);

तार्किक ocl_minMaxIdx( निवेशः_Array _src, द्विगुण* minVal, द्विगुण* maxVal, पूर्णाङ्क* minLoc, पूर्णाङ्क* maxLoc, निवेशः_Array आवरणम्_2,
                   पूर्णाङ्क ddepth, तार्किक abs_मानानि, निवेशः_Array _src2, द्विगुण * maxVal2)
{
    ध्रुव ocl::Device & dev = ocl::Device::getDefault();

#यदि_परिभाषितम् __ANDROID__
    यदि (dev.isNVidia())
        प्रत्यागम असत्य;
#यदि_अन्तः

    यदि (dev.device_संस्करणम्_Major() == 1 && dev.device_संस्करणम्_Minor() < 2)
    {
        // 'static' storage class specifier used by "minmaxloc" is available from OpenCL 1.2+ only
        प्रत्यागम असत्य;
    }

    तार्किक doubleSupport = dev.doubleFPConfig() > 0, have_आवरणम् = !आवरणम्_2.empty(),
    haveSrc2 = _src2.विधा() != निवेशः_Array_2::NONE;
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type),
    kercn = have_आवरणम् ? cn : std::min(4, ocl::predict_Optimal_सदिशम्_विस्तारः(_src, _src2));

    यदि (गभीरता >= CV_16F)
        प्रत्यागम असत्य;

    // disabled following modes since it occasionally fails on AMD devices (e.g. A10-6800K, sep. 2014)
    यदि ((have_आवरणम् || type == CV_32FC1) && dev.isAMD())
        प्रत्यागम असत्य;

    CV_Assert( (cn == 1 && (!have_आवरणम् || आवरणम्_2.type() == CV_8U || आवरणम्_2.type() == CV_8S || आवरणम्_2.type() == CV_Bool)) ||
              (cn >= 1 && !minLoc && !maxLoc) );

    यदि (ddepth < 0)
        ddepth = गभीरता;

    CV_Assert(!haveSrc2 || _src2.type() == type);

    यदि (गभीरता == CV_32S || गभीरता == CV_8S || गभीरता == CV_32U || गभीरता == CV_64U ||
        गभीरता == CV_64S || गभीरता == CV_16F || गभीरता == CV_16BF)
        प्रत्यागम असत्य;

    यदि ((गभीरता == CV_64F || ddepth == CV_64F) && !doubleSupport)
        प्रत्यागम असत्य;

    पूर्णाङ्क groupnum = dev.maxComputeUnits();
    size_t wgs = dev.max_Work_गणः_आकार();

    पूर्णाङ्क wgs2_aligned = 1;
    यावत् (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    तार्किक needMinVal = minVal || minLoc, needMinLoc = minLoc != रिक्त,
    needMaxVal = maxVal || maxLoc, needMaxLoc = maxLoc != रिक्त;

    // in case of mask we must know whether mask is filled with zeros or not
    // so let's calculate min or max location, if it's undefined, so mask is zeros
    यदि (!(needMaxLoc || needMinLoc) && have_आवरणम्)
    {
        यदि (needMinVal)
            needMinLoc = सत्य;
        अन्यथा
            needMaxLoc = सत्य;
    }

    वर्ण cvt[2][50];
    String opts = format("-D DEPTH_%d -D srcT1=%s%s -D WGS=%d -D srcT=%s"
                         " -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d%s%s%s%s"
                         " -D dstT1=%s -D dstT=%s -D convertToDT=%s%s%s%s%s -D wdepth=%d -D convertFromU=%s"
                         " -D MINMAX_STRUCT_ALIGNMENT=%d",
                         गभीरता, ocl::type_गन्तव्यम्_Str(गभीरता), have_आवरणम् ? " -D HAVE_MASK" : "", (पूर्णाङ्क)wgs,
                         ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, kercn)), wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         आवरणम्_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         needMinVal ? " -D NEED_MINVAL" : "", needMaxVal ? " -D NEED_MAXVAL" : "",
                         needMinLoc ? " -D NEED_MINLOC" : "", needMaxLoc ? " -D NEED_MAXLOC" : "",
                         ocl::type_गन्तव्यम्_Str(ddepth), ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ocl::convertTypeStr(गभीरता, ddepth, kercn, cvt[0], परिमाण(cvt[0])),
                         abs_मानानि ? " -D OP_ABS" : "",
                         haveSrc2 ? " -D HAVE_SRC2" : "", maxVal2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", ddepth,
                         गभीरता <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, kercn, cvt[1], परिमाण(cvt[1])) : "noconvert",
                         MINMAX_STRUCT_ALIGNMENT);

    ocl::Kernel k("minmaxloc", ocl::core::minmaxloc_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    पूर्णाङ्क esz = CV_ELEM_आकार(ddepth), esz32s = CV_ELEM_SIZE1(CV_32S),
    dbsize = groupnum * ((needMinVal ? esz : 0) + (needMaxVal ? esz : 0) +
                         (needMinLoc ? esz32s : 0) + (needMaxLoc ? esz32s : 0) +
                         (maxVal2 ? esz : 0))
    + 5 * MINMAX_STRUCT_ALIGNMENT;
    UMat src = _src.getUMat(), src2 = _src2.getUMat(), db(1, dbsize, CV_8UC1), आवरणम् = आवरणम्_2.getUMat();

    यदि (cn > 1 && !have_आवरणम्)
    {
        src = src.reshape(1);
        src2 = src2.reshape(1);
    }

    यदि (haveSrc2)
    {
        यदि (!have_आवरणम्)
            k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(src), src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_आकार(src2));
        अन्यथा
            k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(src), src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_आकार(आवरणम्),
                   ocl::KernelArg::Read_Only_No_आकार(src2));
    }
    अन्यथा
    {
        यदि (!have_आवरणम्)
            k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(src), src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db));
        अन्यथा
            k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(src), src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(),
                   groupnum, ocl::KernelArg::PtrWriteOnly(db), ocl::KernelArg::Read_Only_No_आकार(आवरणम्));
    }

    size_t globalsize = groupnum * wgs;
    यदि (!k.सततम्(1, &globalsize, &wgs, सत्य))
        प्रत्यागम असत्य;

    स्थिर ध्रुव getMinMaxResFunc functab[7] =
    {
        getMinMaxRes<uchar>,
        getMinMaxRes<वर्ण>,
        getMinMaxRes<ushort>,
        getMinMaxRes<ह्रस्व>,
        getMinMaxRes<पूर्णाङ्क>,
        getMinMaxRes<भिन्न>,
        getMinMaxRes<द्विगुण>
    };

    CV_Assert(ddepth <= CV_64F);
    getMinMaxResFunc func = functab[ddepth];

    पूर्णाङ्क loc_अस्थायी[2];
    func(db.getMat(ACCESS_READ), minVal, maxVal,
         needMinLoc ? minLoc ? minLoc : loc_अस्थायी : minLoc,
         needMaxLoc ? maxLoc ? maxLoc : loc_अस्थायी : maxLoc,
         groupnum, src.स्तम्भसङ्ख्या, maxVal2);

    प्रत्यागम सत्य;
}

#यदि_अन्तः

}

// serial implementation, also used for the pieces of the parallel version
स्थिर शून्य minMaxIdx_(cv::निवेशः_Array _src, द्विगुण* minVal,
                       द्विगुण* maxVal, पूर्णाङ्क* minIdx, पूर्णाङ्क* maxIdx,
                       cv::निवेशः_Array आवरणम्_2)
{
    उपयुज्य नामाकाश cv;
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type);
    CV_Assert( (cn == 1 && (आवरणम्_2.empty() || आवरणम्_2.type() == CV_8U || आवरणम्_2.type() == CV_8S || आवरणम्_2.type() == CV_Bool)) ||
               (cn > 1 && आवरणम्_2.empty() && !minIdx && !maxIdx) );

    CV_OCL_सततम्(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2  && (आवरणम्_2.empty() || _src.size() == आवरणम्_2.size()),
               ocl_minMaxIdx(_src, minVal, maxVal, minIdx, maxIdx, आवरणम्_2))

    Mat src = _src.getMat(), आवरणम् = आवरणम्_2.getMat();

    यदि (src.dims <= 2)
    {
        यदि ((size_t)src.चरणः == (size_t)आवरणम्.चरणः || आवरणम्.empty())
        {
            CALL_HAL(minMaxIdx, cv_hal_minMaxIdx, src.दत्त, src.चरणः, src.स्तम्भसङ्ख्या*cn, src.पङ्क्तिकाः,
                     src.गभीरता(), minVal, maxVal, minIdx, maxIdx, आवरणम्.दत्त);
        }
        CALL_HAL(min_Max_Idx_आवरणम्_चरणः, cv_hal_min_Max_Idx_आवरणम्_चरणः, src.दत्त, src.चरणः, src.स्तम्भसङ्ख्या*cn, src.पङ्क्तिकाः,
                 src.गभीरता(), minVal, maxVal, minIdx, maxIdx, आवरणम्.दत्त, आवरणम्.चरणः);
    }
    अन्यथा यदि (src.isContinuous() && (आवरणम्.isContinuous() || आवरणम्.empty()))
    {
        पूर्णाङ्क res = cv_hal_minMaxIdx(src.दत्त, 0, (पूर्णाङ्क)src.योग()*cn, 1, src.गभीरता(),
                                   minVal, maxVal, minIdx, maxIdx, आवरणम्.दत्त);

        यदि (res == CV_HAL_ERROR_OK)
        {
            // minIdx[0] and minIdx[0] are always 0 for "flatten" version
            यदि (minIdx)
                ofs2idx(src, minIdx[1]+1, minIdx);
            यदि (maxIdx)
                ofs2idx(src, maxIdx[1]+1, maxIdx);
            प्रत्यागम;
        }
        अन्यथा यदि (res != CV_HAL_ERROR_NOT_IMPLEMENTED)
        {
            CV_Error_(cv::Error::Sts_आन्तरिकम्,
            ("HAL implementation minMaxIdx ==> " CVAUX_STR(cv_hal_minMaxIdx) " returned %d (0x%08x)", res, res));
        }
    }

    MinMaxIdxFunc func = getMinMaxIdxFunc(गभीरता);
    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, &आवरणम्, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);

    size_t minidx = 0, maxidx = 0;
    size_t startidx = 1;
    सङ्घ {
        पूर्णाङ्क i;
        भिन्न f;
        द्विगुण d;
        int64 L;
        uint64 UL;
    } minval, maxval;
    पूर्णाङ्क plane_आकार = (पूर्णाङ्क)it.size*cn;
    minval.L = maxval.L = 0;

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it, startidx += plane_आकार )
        func( ptrs[0], ptrs[1], &minval.L, &maxval.L, &minidx, &maxidx, plane_आकार, startidx );

    द्विगुण dminval, dmaxval;
    यदि( गभीरता <= CV_32S || गभीरता == CV_Bool )
        dminval = minval.i, dmaxval = maxval.i;
    अन्यथा यदि( गभीरता == CV_32F || गभीरता == CV_16F || गभीरता == CV_16BF )
        dminval = minval.f, dmaxval = maxval.f;
    अन्यथा यदि( गभीरता == CV_64F )
        dminval = minval.d, dmaxval = maxval.d;
    अन्यथा यदि( गभीरता == CV_64S || गभीरता == CV_32U )
        dminval = (द्विगुण)minval.L, dmaxval = (द्विगुण)maxval.L;
    अन्यथा {
        CV_Assert(गभीरता == CV_64U);
        dminval = (द्विगुण)minval.UL, dmaxval = (द्विगुण)maxval.UL;
    }

    यदि( minVal )
        *minVal = dminval;
    यदि( maxVal )
        *maxVal = dmaxval;

    यदि( minIdx )
        ofs2idx(src, minidx, minIdx);
    यदि( maxIdx )
        ofs2idx(src, maxidx, maxIdx);
}

शून्य cv::minMaxIdx(निवेशः_Array _src, द्विगुण* minVal,
                   द्विगुण* maxVal, पूर्णाङ्क* minIdx, पूर्णाङ्क* maxIdx,
                   निवेशः_Array आवरणम्_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    यदि (!_src.isUMat())
    {
        Mat src = _src.getMat(), आवरणम् = आवरणम्_2.getMat();
        ध्रुव पूर्णाङ्क cn = src.channels();
        ध्रुव तार्किक तर्काः_Ok = (cn == 1 && (आवरणम्.empty() || ((आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S ||
                             आवरणम्.type() == CV_Bool) && src.size == आवरणम्.size))) ||
                            (cn > 1 && आवरणम्.empty() && !minIdx && !maxIdx);
        यदि (तर्काः_Ok && विभज_For_Parallel_Stat(src, आवरणम्, chunks))
        {
            ध्रुव पूर्णाङ्क n = (पूर्णाङ्क)chunks.size();
            std::vector<द्विगुण> mn(n), mx(n);
            std::vector<size_t> imn(n, 0), imx(n, 0);   // 1-based row-major indices, 0 = no element
            parallel_for_(Range(0, n), [&](ध्रुव Range& r)
            {
                प्रति (पूर्णाङ्क i = r.start; i < r.end; i++)
                {
                    पूर्णाङ्क a[2] = {-1, -1}, b[2] = {-1, -1};
                    minMaxIdx_(chunks[i].src, &mn[i], &mx[i], cn == 1 ? a : शून्यसूचकः, cn == 1 ? b : शून्यसूचकः,
                               chunks[i].आवरणम्);
                    यदि (cn > 1)
                        imn[i] = imx[i] = 1;
                    अन्यथा यदि (a[0] >= 0)
                    {
                        imn[i] = chunk_सूचकांक_गन्तव्यम्_Elem(chunks[i], a) + 1;
                        imx[i] = chunk_सूचकांक_गन्तव्यम्_Elem(chunks[i], b) + 1;
                    }
                }
            });
            // pieces are in memory order, so taking the first strict improvement keeps the
            // first occurrence, as in the serial version
            पूर्णाङ्क bmin = -1, bmax = -1;
            प्रति (पूर्णाङ्क i = 0; i < n; i++)
            {
                यदि (imn[i] == 0)
                    अनुवर्त;
                यदि (bmin < 0 || mn[i] < mn[bmin]) bmin = i;
                यदि (bmax < 0 || mx[i] > mx[bmax]) bmax = i;
            }
            यदि (minVal) *minVal = bmin >= 0 ? mn[bmin] : 0;
            यदि (maxVal) *maxVal = bmax >= 0 ? mx[bmax] : 0;
            यदि (minIdx) ofs2idx(src, bmin >= 0 ? imn[bmin] : 0, minIdx);
            यदि (maxIdx) ofs2idx(src, bmax >= 0 ? imx[bmax] : 0, maxIdx);
            प्रत्यागम;
        }
    }
    minMaxIdx_(_src, minVal, maxVal, minIdx, maxIdx, आवरणम्_2);
}

शून्य cv::minMaxLoc( निवेशः_Array _img, द्विगुण* minVal, द्विगुण* maxVal,
                    Point* minLoc, Point* maxLoc, निवेशः_Array आवरणम् )
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क dims = _img.dims();
    CV_CheckLE(dims, 2, "");

    minMaxIdx(_img, minVal, maxVal, (पूर्णाङ्क*)minLoc, (पूर्णाङ्क*)maxLoc, आवरणम्);
    यदि( minLoc) {
        यदि (dims == 2)
            std::swap(minLoc->x, minLoc->y);
        अन्यथा {
            minLoc->y = 0;
        }
    }
    यदि( maxLoc) {
        यदि (dims == 2)
            std::swap(maxLoc->x, maxLoc->y);
        अन्यथा {
            maxLoc->y = 0;
        }
    }
}

गणना वर्ग Reduce_बहुलकम्
{
    FIRST_MIN = 0, //!< get index of first min occurrence
    चरमः_MIN  = 1, //!< get index of last min occurrence
    FIRST_MAX = 2, //!< get index of first max occurrence
    चरमः_MAX  = 3, //!< get index of last max occurrence
};

प्रतिरूपम् <प्रकारनाम T>
संरचना reduceMinMaxImpl
{
    शून्य संक्रियकः()(ध्रुव cv::Mat& src, cv::Mat& dst, Reduce_बहुलकम् बहुलकम्, ध्रुव पूर्णाङ्क axis) ध्रुव
    {
        विकल्प(बहुलकम्)
        {
        पक्ष Reduce_बहुलकम्::FIRST_MIN:
            reduceMinMaxApply<std::less>(src, dst, axis);
            भङ्ग;
        पक्ष Reduce_बहुलकम्::चरमः_MIN:
            reduceMinMaxApply<std::less_equal>(src, dst, axis);
            भङ्ग;
        पक्ष Reduce_बहुलकम्::FIRST_MAX:
            reduceMinMaxApply<std::greater>(src, dst, axis);
            भङ्ग;
        पक्ष Reduce_बहुलकम्::चरमः_MAX:
            reduceMinMaxApply<std::greater_equal>(src, dst, axis);
            भङ्ग;
        }
    }

    प्रतिरूपम् <प्रतिरूपम्<वर्ग> वर्ग Cmp>
    स्थिर शून्य reduceMinMaxApply(ध्रुव cv::Mat& src, cv::Mat& dst, ध्रुव पूर्णाङ्क axis)
    {
        Cmp<T> cmp;

        ध्रुव स्वचालित *src_ptr = src.ptr<T>();
        स्वचालित *dst_ptr = dst.ptr<int32_t>();

        ध्रुव size_t outer_आकार = src.योग(0, axis);
        ध्रुव स्वचालित mid_आकार = static_cast<size_t>(src.size[axis]);

        ध्रुव size_t outer_चरणः = src.योग(axis);
        ध्रुव size_t dst_चरणः = dst.योग(axis);

        ध्रुव size_t mid_चरणः = src.योग(axis + 1);

        प्रति (size_t outer = 0; outer < outer_आकार; ++outer)
        {
            ध्रुव size_t outer_विस्थापनम् = outer * outer_चरणः;
            ध्रुव size_t dst_विस्थापनम् = outer * dst_चरणः;
            प्रति (size_t mid = 0; mid != mid_आकार; ++mid)
            {
                ध्रुव size_t src_विस्थापनम् = outer_विस्थापनम् + mid * mid_चरणः;
                प्रति (size_t inner = 0; inner < mid_चरणः; inner++)
                {
                    int32_t& सूचकांक = dst_ptr[dst_विस्थापनम् + inner];

                    ध्रुव size_t पूर्व = outer_विस्थापनम् + सूचकांक * mid_चरणः + inner;
                    ध्रुव size_t curr = src_विस्थापनम् + inner;

                    यदि (cmp(src_ptr[curr], src_ptr[पूर्व]))
                    {
                        सूचकांक = static_cast<int32_t>(mid);
                    }
                }
            }
        }
    }
};

स्थिर शून्य reduceMinMax(cv::निवेशः_Array src, cv::OutputArray dst, Reduce_बहुलकम् बहुलकम्, पूर्णाङ्क axis)
{
    CV_INSTRUMENT_REGION();

    cv::Mat srcMat = src.getMat();
    पूर्णाङ्क dims = std::max(1, srcMat.dims);
    axis = (axis + dims) % dims;
    CV_Assert(srcMat.channels() == 1 && axis >= 0 && axis <= srcMat.dims);

    std::vector<पूर्णाङ्क> sizes(srcMat.dims);
    std::copy(srcMat.size.p, srcMat.size.p + srcMat.dims, sizes.begin());
    यदि(!sizes.empty())
        sizes[axis] = 1;

    dst.create(srcMat.dims, sizes.दत्त(), CV_32SC1); // indices
    cv::Mat dstMat = dst.getMat();
    dstMat.समुच्चयः_गन्तव्यम्(cv::Scalar::all(0));

    यदि (!srcMat.isContinuous())
    {
        srcMat = srcMat.clone();
    }

    तार्किक needs_copy = !dstMat.isContinuous();
    यदि (needs_copy)
    {
        dstMat = dstMat.clone();
    }

    cv::detail::गभीरता_Dispatch<reduceMinMaxImpl>(srcMat.गभीरता(), srcMat, dstMat, बहुलकम्, axis);

    यदि (needs_copy)
    {
        dstMat.copy_गन्तव्यम्(dst);
    }
}

शून्य cv::reduceArgMin(निवेशः_Array src, OutputArray dst, पूर्णाङ्क axis, तार्किक चरमः_सूचकांक)
{
    reduceMinMax(src, dst, चरमः_सूचकांक ? Reduce_बहुलकम्::चरमः_MIN : Reduce_बहुलकम्::FIRST_MIN, axis);
}

शून्य cv::reduceArgMax(निवेशः_Array src, OutputArray dst, पूर्णाङ्क axis, तार्किक चरमः_सूचकांक)
{
    reduceMinMax(src, dst, चरमः_सूचकांक ? Reduce_बहुलकम्::चरमः_MAX : Reduce_बहुलकम्::FIRST_MAX, axis);
}
