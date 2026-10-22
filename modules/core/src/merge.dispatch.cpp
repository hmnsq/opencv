// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "merge.simd.hpp"
#অন্তর্ভাব "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv { নামাকাশ hal {

শূন্য merge8u(ধ্রুব uchar** src, uchar* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য merge16u(ধ্রুব ushort** src, ushort* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য merge32s(ধ্রুব পূর্ণাঙ্ক** src, পূর্ণাঙ্ক* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য merge64s(ধ্রুব int64** src, int64* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


প্রকারনির্দেশ শূন্য (*MergeFunc)(ধ্রুব শূন্য** src, শূন্য* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn);

template<typename T, শূন্য (*fn)(ধ্রুব T**, T*, পূর্ণাঙ্ক, পূর্ণাঙ্ক)>
স্থির শূন্য mergeWrap(ধ্রুব শূন্য** src, শূন্য* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn)
{
    fn((ধ্রুব T**)src, (T*)dst, len, cn);
}

স্থির MergeFunc getMergeFunc(পূর্ণাঙ্ক depth)
{
    স্থির MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<পূর্ণাঙ্ক, cv::hal::merge32s>, mergeWrap<পূর্ণাঙ্ক, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<পূর্ণাঙ্ক, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    প্রত্যাগম mergeTab[depth];
}

#ifdef HAVE_IPP

স্থির তার্কিক ipp_merge(ধ্রুব Mat* mv, Mat& dst, পূর্ণাঙ্ক channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    যদি(channels != 3 && channels != 4)
        প্রত্যাগম অসত্য;

    যদি(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        ধ্রুব শূন্য *srcPtrs[4] = {রিক্ত};
        size_t      srcStep    = mv[0].step;
        প্রতি(পূর্ণাঙ্ক i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            যদি(srcStep != mv[i].step)
                প্রত্যাগম অসত্য;
        }

        প্রত্যাগম CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (পূর্ণাঙ্ক)srcStep, dst.ptr(), (পূর্ণাঙ্ক)dst.step, size, (পূর্ণাঙ্ক)mv[0].elemSize1(), channels, 0) >= 0;
    }
    অন্যথা
    {
        ধ্রুব Mat *arrays[5] = {রিক্ত};
        uchar     *ptrs[5]   = {রিক্ত};
        arrays[0] = &dst;

        প্রতি(পূর্ণাঙ্ক i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (পূর্ণাঙ্ক)it.size, 1 };

        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (ধ্রুব শূন্য**)&ptrs[1], 0, ptrs[0], 0, size, (পূর্ণাঙ্ক)mv[0].elemSize1(), channels, 0) < 0)
                প্রত্যাগম অসত্য;
        }
        প্রত্যাগম সত্য;
    }
#অন্যথা
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    প্রত্যাগম অসত্য;
#endif
}
#endif

শূন্য merge(ধ্রুব Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    পূর্ণাঙ্ক depth = mv[0].depth();
    তার্কিক allch1 = সত্য;
    পূর্ণাঙ্ক k, cn = 0;
    size_t i;

    প্রতি( i = 0; i < n; i++ )
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
        প্রত্যাগম;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (পূর্ণাঙ্ক)n));

    যদি( !allch1 )
    {
        AutoBuffer<পূর্ণাঙ্ক> pairs(cn*2);
        পূর্ণাঙ্ক j, ni=0;

        প্রতি( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            প্রতি( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        প্রত্যাগম;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (পূর্ণাঙ্ক)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(পরিমাণ(Mat*) + পরিমাণ(uchar*)) + 16);
    ধ্রুব Mat** arrays = (ধ্রুব Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    প্রতি( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (পূর্ণাঙ্ক)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    প্রতি( i = 0; i < it.nplanes; i++, ++it )
    {
        প্রতি( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (ধ্রুব শূন্য**)&ptrs[1], ptrs[0], (পূর্ণাঙ্ক)bsz, cn );

            যদি( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                প্রতি( পূর্ণাঙ্ক t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_merge( প্রবেশ_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    পূর্ণাঙ্ক type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    প্রতি (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        পূর্ণাঙ্ক itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        যদি (src[i].dims > 2)
            প্রত্যাগম অসত্য;

        CV_Assert(size == src[i].size() && depth == idepth);

        প্রতি (পূর্ণাঙ্ক cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    পূর্ণাঙ্ক dcn = (পূর্ণাঙ্ক)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    প্রতি (পূর্ণাঙ্ক i = 0; i < dcn; ++i)
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
        প্রত্যাগম অসত্য;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    পূর্ণাঙ্ক argidx = 0;
    প্রতি (পূর্ণাঙ্ক i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    প্রত্যাগম k.run(2, globalsize, রিক্ত, অসত্য);
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
