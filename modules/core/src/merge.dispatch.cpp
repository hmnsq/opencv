// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "merge.simd.hpp"
#অন্তর্ভাব "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv { নামাকাশ hal {

শূন্য merge8u(ধ্রুবক uchar** src, uchar* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য merge16u(ধ্রুবক ushort** src, ushort* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য merge32s(ধ্রুবক পূর্ণসংখ্যা** src, পূর্ণসংখ্যা* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য merge64s(ধ্রুবক int64** src, int64* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


প্রকারলেখ শূন্য (*MergeFunc)(ধ্রুবক শূন্য** src, শূন্য* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn);

template<typename T, শূন্য (*fn)(ধ্রুবক T**, T*, পূর্ণসংখ্যা, পূর্ণসংখ্যা)>
স্থির শূন্য mergeWrap(ধ্রুবক শূন্য** src, শূন্য* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    fn((ধ্রুবক T**)src, (T*)dst, len, cn);
}

স্থির MergeFunc getMergeFunc(পূর্ণসংখ্যা depth)
{
    স্থির MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<পূর্ণসংখ্যা, cv::hal::merge32s>, mergeWrap<পূর্ণসংখ্যা, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<পূর্ণসংখ্যা, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    ফেরত mergeTab[depth];
}

#ifdef HAVE_IPP

স্থির যৌক্তিক ipp_merge(ধ্রুবক Mat* mv, Mat& dst, পূর্ণসংখ্যা channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    যদি(channels != 3 && channels != 4)
        ফেরত মিথ্যা;

    যদি(mv[0].dims <= 2)
    {
        Ippi_আকার    size       = ippi_আকার(mv[0].size());
        ধ্রুবক শূন্য *srcPtrs[4] = {ফাঁকা};
        size_t      srcStep    = mv[0].step;
        জন্য(পূর্ণসংখ্যা i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            যদি(srcStep != mv[i].step)
                ফেরত মিথ্যা;
        }

        ফেরত CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (পূর্ণসংখ্যা)srcStep, dst.ptr(), (পূর্ণসংখ্যা)dst.step, size, (পূর্ণসংখ্যা)mv[0].elemSize1(), channels, 0) >= 0;
    }
    অন্যথা
    {
        ধ্রুবক Mat *arrays[5] = {ফাঁকা};
        uchar     *ptrs[5]   = {ফাঁকা};
        arrays[0] = &dst;

        জন্য(পূর্ণসংখ্যা i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_আকার size = { (পূর্ণসংখ্যা)it.size, 1 };

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (ধ্রুবক শূন্য**)&ptrs[1], 0, ptrs[0], 0, size, (পূর্ণসংখ্যা)mv[0].elemSize1(), channels, 0) < 0)
                ফেরত মিথ্যা;
        }
        ফেরত সত্য;
    }
#অন্যথা
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    ফেরত মিথ্যা;
#endif
}
#endif

শূন্য merge(ধ্রুবক Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    পূর্ণসংখ্যা depth = mv[0].depth();
    যৌক্তিক allch1 = সত্য;
    পূর্ণসংখ্যা k, cn = 0;
    size_t i;

    জন্য( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    যদি( n == 1 )
    {
        mv[0].copyTo(dst);
        ফেরত;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (পূর্ণসংখ্যা)n));

    যদি( !allch1 )
    {
        Auto_বাফার<পূর্ণসংখ্যা> pairs(cn*2);
        পূর্ণসংখ্যা j, ni=0;

        জন্য( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            জন্য( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        ফেরত;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_আকার(), esz1 = dst.elemSize1();
    size_t blocksize0 = (পূর্ণসংখ্যা)((BLOCK_আকার + esz-1)/esz);
    Auto_বাফার<uchar> _buf((cn+1)*(আকার(Mat*) + আকার(uchar*)) + 16);
    ধ্রুবক Mat** arrays = (ধ্রুবক Mat**)_buf.তথ্য();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    জন্য( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t যোগ = (পূর্ণসংখ্যা)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_আকার(cn), cn <= 4 ? যোগ : std::min(যোগ, blocksize0));

    জন্য( i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( size_t j = 0; j < যোগ; j += blocksize )
        {
            size_t bsz = std::min(যোগ - j, blocksize);
            func( (ধ্রুবক শূন্য**)&ptrs[1], ptrs[0], (পূর্ণসংখ্যা)bsz, cn );

            যদি( j + blocksize < যোগ )
            {
                ptrs[0] += bsz*esz;
                জন্য( পূর্ণসংখ্যা t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_merge( প্রবেশ_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    পূর্ণসংখ্যা type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    আকার_2 size = src[0].size();

    জন্য (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        পূর্ণসংখ্যা itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        যদি (src[i].dims > 2)
            ফেরত মিথ্যা;

        CV_Assert(size == src[i].size() && depth == idepth);

        জন্য (পূর্ণসংখ্যা cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    পূর্ণসংখ্যা dcn = (পূর্ণসংখ্যা)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    জন্য (পূর্ণসংখ্যা i = 0; i < dcn; ++i)
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
    যদি (k.empty())
        ফেরত মিথ্যা;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    পূর্ণসংখ্যা argidx = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::Read_Only_No_আকার(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

শূন্য merge(প্রবেশ_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
