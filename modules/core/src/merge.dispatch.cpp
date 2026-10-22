// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "merge.simd.hpp"
#अन्तर्भाव "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv { नामाकाश hal {

शून्य merge8u(ध्रुव uchar** src, uchar* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य merge16u(ध्रुव ushort** src, ushort* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य merge32s(ध्रुव पूर्णाङ्क** src, पूर्णाङ्क* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य merge64s(ध्रुव int64** src, int64* dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


प्रकारनिर्देश शून्य (*MergeFunc)(ध्रुव शून्य** src, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क cn);

प्रतिरूपम्<प्रकारनाम T, शून्य (*fn)(ध्रुव T**, T*, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर शून्य mergeWrap(ध्रुव शून्य** src, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    fn((ध्रुव T**)src, (T*)dst, len, cn);
}

स्थिर MergeFunc getMergeFunc(पूर्णाङ्क गभीरता)
{
    स्थिर MergeFunc mergeTab[CV_गभीरता_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<पूर्णाङ्क, cv::hal::merge32s>, mergeWrap<पूर्णाङ्क, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<पूर्णाङ्क, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    प्रत्यागम mergeTab[गभीरता];
}

#यदि_परिभाषितम् HAVE_IPP

स्थिर तार्किक ipp_merge(ध्रुव Mat* mv, Mat& dst, पूर्णाङ्क channels)
{
#यदि_परिभाषितम् HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    यदि(channels != 3 && channels != 4)
        प्रत्यागम असत्य;

    यदि(mv[0].dims <= 2)
    {
        Ippi_आकार    size       = ippi_आकार(mv[0].size());
        ध्रुव शून्य *srcPtrs[4] = {रिक्त};
        size_t      src_चरणः_3    = mv[0].चरणः;
        प्रति(पूर्णाङ्क i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            यदि(src_चरणः_3 != mv[i].चरणः)
                प्रत्यागम असत्य;
        }

        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (पूर्णाङ्क)src_चरणः_3, dst.ptr(), (पूर्णाङ्क)dst.चरणः, size, (पूर्णाङ्क)mv[0].elemSize1(), channels, 0) >= 0;
    }
    अन्यथा
    {
        ध्रुव Mat *arrays[5] = {रिक्त};
        uchar     *ptrs[5]   = {रिक्त};
        arrays[0] = &dst;

        प्रति(पूर्णाङ्क i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_आकार size = { (पूर्णाङ्क)it.size, 1 };

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (ध्रुव शून्य**)&ptrs[1], 0, ptrs[0], 0, size, (पूर्णाङ्क)mv[0].elemSize1(), channels, 0) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }
#अन्यथा
    CV_अप्रयुक्तम्(dst); CV_अप्रयुक्तम्(mv); CV_अप्रयुक्तम्(channels);
    प्रत्यागम असत्य;
#यदि_अन्तः
}
#यदि_अन्तः

शून्य merge(ध्रुव Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    पूर्णाङ्क गभीरता = mv[0].गभीरता();
    तार्किक allch1 = सत्य;
    पूर्णाङ्क k, cn = 0;
    size_t i;

    प्रति( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].गभीरता() == गभीरता);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(गभीरता, cn));
    Mat dst = _dst.getMat();

    यदि( n == 1 )
    {
        mv[0].copy_गन्तव्यम्(dst);
        प्रत्यागम;
    }

    CV_IPP_सततम्(allch1, ipp_merge(mv, dst, (पूर्णाङ्क)n));

    यदि( !allch1 )
    {
        Auto_बफर<पूर्णाङ्क> युग्मानि(cn*2);
        पूर्णाङ्क j, ni=0;

        प्रति( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            प्रति( k = 0; k < ni; k++ )
            {
                युग्मानि[(j+k)*2] = j + k;
                युग्मानि[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &युग्मानि[0], cn );
        प्रत्यागम;
    }

    MergeFunc func = getMergeFunc(गभीरता);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_आकार(), esz1 = dst.elemSize1();
    size_t blocksize0 = (पूर्णाङ्क)((खण्डम्_आकार + esz-1)/esz);
    Auto_बफर<uchar> _buf((cn+1)*(परिमाण(Mat*) + परिमाण(uchar*)) + 16);
    ध्रुव Mat** arrays = (ध्रुव Mat**)_buf.दत्त();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    प्रति( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t योग = (पूर्णाङ्क)it.size;
    size_t blocksize = std::min((size_t)CV_विभज_MERGE_MAX_खण्डम्_आकार(cn), cn <= 4 ? योग : std::min(योग, blocksize0));

    प्रति( i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( size_t j = 0; j < योग; j += blocksize )
        {
            size_t bsz = std::min(योग - j, blocksize);
            func( (ध्रुव शून्य**)&ptrs[1], ptrs[0], (पूर्णाङ्क)bsz, cn );

            यदि( j + blocksize < योग )
            {
                ptrs[0] += bsz*esz;
                प्रति( पूर्णाङ्क t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#यदि_परिभाषितम् HAVE_OPENCL

स्थिर तार्किक ocl_merge( निवेशः_Array_सम्बन्धि_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_सदिशम्(src);
    CV_Assert(!src.empty());

    पूर्णाङ्क type = src[0].type(), गभीरता = CV_MAT_गभीरता(type),
            पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    आकार size = src[0].size();

    प्रति (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        पूर्णाङ्क itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_गभीरता(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        यदि (src[i].dims > 2)
            प्रत्यागम असत्य;

        CV_Assert(size == src[i].size() && गभीरता == idepth);

        प्रति (पूर्णाङ्क cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    पूर्णाङ्क dcn = (पूर्णाङ्क)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    प्रति (पूर्णाङ्क i = 0; i < dcn; ++i)
    {
        srcargs += format("DECLARE_SRC_PARAM(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        cndecl += format(" -D scn%d=%d", i, ksrc[i].channels());
    }

    ocl::Kernel k("merge", ocl::core::विभज_merge_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_गन्तव्यम्_Str(गभीरता), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    यदि (k.empty())
        प्रत्यागम असत्य;

    _dst.create(size, CV_MAKE_TYPE(गभीरता, dcn));
    UMat dst = _dst.getUMat();

    पूर्णाङ्क argidx = 0;
    प्रति (पूर्णाङ्क i = 0; i < dcn; ++i)
        argidx = k.समुच्चयः(argidx, ocl::KernelArg::Read_Only_No_आकार(ksrc[i]));
    argidx = k.समुच्चयः(argidx, ocl::KernelArg::WriteOnly(dst));
    k.समुच्चयः(argidx, पङ्क्तिकाः_प्रत्येकम्_WI);

    size_t globalsize[2] = { (size_t)dst.स्तम्भसङ्ख्या, ((size_t)dst.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

#यदि_अन्तः

शून्य merge(निवेशः_Array_सम्बन्धि_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_सततम्(_mv.is_UMat_सदिशम्() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_सदिशम्(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
