// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"

#அந்தர்பாவ "merge.simd.hpp"
#அந்தர்பாவ "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv { நாமாகாஶ hal {

வெறுமை merge8u(மாறிலி uchar** src, uchar* dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை merge16u(மாறிலி ushort** src, ushort* dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை merge32s(மாறிலி முழு** src, முழு* dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை merge64s(மாறிலி int64** src, int64* dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


வகைவரை வெறுமை (*MergeFunc)(மாறிலி வெறுமை** src, வெறுமை* dst, முழு len, முழு cn);

template<typename T, வெறுமை (*fn)(மாறிலி T**, T*, முழு, முழு)>
நிலையான வெறுமை mergeWrap(மாறிலி வெறுமை** src, வெறுமை* dst, முழு len, முழு cn)
{
    fn((மாறிலி T**)src, (T*)dst, len, cn);
}

நிலையான MergeFunc getMergeFunc(முழு depth)
{
    நிலையான MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<முழு, cv::hal::merge32s>, mergeWrap<முழு, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<முழு, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    திரும்பு mergeTab[depth];
}

#ifdef HAVE_IPP

நிலையான தருக்க ipp_merge(மாறிலி Mat* mv, Mat& dst, முழு channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    எனில்(channels != 3 && channels != 4)
        திரும்பு பொய்;

    எனில்(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        மாறிலி வெறுமை *srcPtrs[4] = {வெற்று};
        size_t      srcStep    = mv[0].step;
        க்கு(முழு i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            எனில்(srcStep != mv[i].step)
                திரும்பு பொய்;
        }

        திரும்பு CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (முழு)srcStep, dst.ptr(), (முழு)dst.step, size, (முழு)mv[0].elemSize1(), channels, 0) >= 0;
    }
    இல்லையெனில்
    {
        மாறிலி Mat *arrays[5] = {வெற்று};
        uchar     *ptrs[5]   = {வெற்று};
        arrays[0] = &dst;

        க்கு(முழு i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (முழு)it.size, 1 };

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (மாறிலி வெறுமை**)&ptrs[1], 0, ptrs[0], 0, size, (முழு)mv[0].elemSize1(), channels, 0) < 0)
                திரும்பு பொய்;
        }
        திரும்பு உண்மை;
    }
#இல்லையெனில்
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    திரும்பு பொய்;
#endif
}
#endif

வெறுமை merge(மாறிலி Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    முழு depth = mv[0].depth();
    தருக்க allch1 = உண்மை;
    முழு k, cn = 0;
    size_t i;

    க்கு( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    எனில்( n == 1 )
    {
        mv[0].copyTo(dst);
        திரும்பு;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (முழு)n));

    எனில்( !allch1 )
    {
        AutoBuffer<முழு> pairs(cn*2);
        முழு j, ni=0;

        க்கு( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            க்கு( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        திரும்பு;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (முழு)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(அளவு(Mat*) + அளவு(uchar*)) + 16);
    மாறிலி Mat** arrays = (மாறிலி Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    க்கு( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (முழு)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    க்கு( i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (மாறிலி வெறுமை**)&ptrs[1], ptrs[0], (முழு)bsz, cn );

            எனில்( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                க்கு( முழு t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_merge( உள்ளீடு_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    முழு type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    க்கு (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        முழு itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        எனில் (src[i].dims > 2)
            திரும்பு பொய்;

        CV_Assert(size == src[i].size() && depth == idepth);

        க்கு (முழு cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    முழு dcn = (முழு)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    க்கு (முழு i = 0; i < dcn; ++i)
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
    எனில் (k.empty())
        திரும்பு பொய்;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    முழு argidx = 0;
    க்கு (முழு i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

வெறுமை merge(உள்ளீடு_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
