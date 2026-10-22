// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

#ଅନ୍ତର୍ଭାଵ "merge.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv { ନାମାକାଶ hal {

ଶୂନ୍ୟ merge8u(ଅଚଳ uchar** src, uchar* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ merge16u(ଅଚଳ ushort** src, ushort* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ merge32s(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା** src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ merge64s(ଅଚଳ int64** src, int64* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*MergeFunc)(ଅଚଳ ଶୂନ୍ୟ** src, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);

template<typename T, ଶୂନ୍ୟ (*fn)(ଅଚଳ T**, T*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ଶୂନ୍ୟ mergeWrap(ଅଚଳ ଶୂନ୍ୟ** src, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    fn((ଅଚଳ T**)src, (T*)dst, len, cn);
}

ସ୍ଥିର MergeFunc getMergeFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, cv::hal::merge32s>, mergeWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    ଫେରନ୍ତୁ mergeTab[depth];
}

#ifdef HAVE_IPP

ସ୍ଥିର ତାର୍କିକ ipp_merge(ଅଚଳ Mat* mv, Mat& dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ଯଦି(channels != 3 && channels != 4)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        ଅଚଳ ଶୂନ୍ୟ *srcPtrs[4] = {ଖାଲି};
        size_t      srcStep    = mv[0].step;
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            ଯଦି(srcStep != mv[i].step)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }

        ଫେରନ୍ତୁ CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)srcStep, dst.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.step, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mv[0].elemSize1(), channels, 0) >= 0;
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ Mat *arrays[5] = {ଖାଲି};
        uchar     *ptrs[5]   = {ଖାଲି};
        arrays[0] = &dst;

        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, 1 };

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (ଅଚଳ ଶୂନ୍ୟ**)&ptrs[1], 0, ptrs[0], 0, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mv[0].elemSize1(), channels, 0) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#ଅନ୍ୟଥା
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}
#endif

ଶୂନ୍ୟ merge(ଅଚଳ Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = mv[0].depth();
    ତାର୍କିକ allch1 = ସତ୍ୟ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, cn = 0;
    size_t i;

    ପାଇଁ( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    ଯଦି( n == 1 )
    {
        mv[0].copyTo(dst);
        ଫେରନ୍ତୁ;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)n));

    ଯଦି( !allch1 )
    {
        AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> pairs(cn*2);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, ni=0;

        ପାଇଁ( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            ପାଇଁ( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        ଫେରନ୍ତୁ;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(ଆକାର(Mat*) + ଆକାର(uchar*)) + 16);
    ଅଚଳ Mat** arrays = (ଅଚଳ Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    ପାଇଁ( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    ପାଇଁ( i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (ଅଚଳ ଶୂନ୍ୟ**)&ptrs[1], ptrs[0], (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)bsz, cn );

            ଯଦି( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_merge( ନିବେଶ_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    ପାଇଁ (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        ଯଦି (src[i].dims > 2)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        CV_Assert(size == src[i].size() && depth == idepth);

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dcn; ++i)
    {
        srcargs += format("DECLARE_SRC_PARAM(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        cndecl += format(" -D scn%d=%d", i, ksrc[i].channels());
    }

    ocl::Kernel k("merge", ocl::core::split_merge_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memopTypeToStr(depth), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା argidx = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ଶୂନ୍ୟ merge(ନିବେଶ_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
