// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "merge.simd.hpp"
#ಅನ್ತರ್ಭಾವ "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಶೂನ್ಯ merge8u(ಧ್ರುವ uchar** src, uchar* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ merge16u(ಧ್ರುವ ushort** src, ushort* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ merge32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ** src, ಪೂರ್ಣಾಙ್ಕ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ merge64s(ಧ್ರುವ int64** src, int64* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*MergeFunc)(ಧ್ರುವ ಶೂನ್ಯ** src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);

template<typename T, ಶೂನ್ಯ (*fn)(ಧ್ರುವ T**, T*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ mergeWrap(ಧ್ರುವ ಶೂನ್ಯ** src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    fn((ಧ್ರುವ T**)src, (T*)dst, len, cn);
}

ಸ್ಥಿರ MergeFunc getMergeFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ಪೂರ್ಣಾಙ್ಕ, cv::hal::merge32s>, mergeWrap<ಪೂರ್ಣಾಙ್ಕ, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<ಪೂರ್ಣಾಙ್ಕ, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    ಪ್ರತ್ಯಾಗಮ mergeTab[depth];
}

#ifdef HAVE_IPP

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_merge(ಧ್ರುವ Mat* mv, Mat& dst, ಪೂರ್ಣಾಙ್ಕ channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಯದಿ(channels != 3 && channels != 4)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        ಧ್ರುವ ಶೂನ್ಯ *srcPtrs[4] = {ರಿಕ್ತ};
        size_t      srcStep    = mv[0].step;
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            ಯದಿ(srcStep != mv[i].step)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }

        ಪ್ರತ್ಯಾಗಮ CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (ಪೂರ್ಣಾಙ್ಕ)srcStep, dst.ptr(), (ಪೂರ್ಣಾಙ್ಕ)dst.step, size, (ಪೂರ್ಣಾಙ್ಕ)mv[0].elemSize1(), channels, 0) >= 0;
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ Mat *arrays[5] = {ರಿಕ್ತ};
        uchar     *ptrs[5]   = {ರಿಕ್ತ};
        arrays[0] = &dst;

        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (ಪೂರ್ಣಾಙ್ಕ)it.size, 1 };

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (ಧ್ರುವ ಶೂನ್ಯ**)&ptrs[1], 0, ptrs[0], 0, size, (ಪೂರ್ಣಾಙ್ಕ)mv[0].elemSize1(), channels, 0) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}
#endif

ಶೂನ್ಯ merge(ಧ್ರುವ Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    ಪೂರ್ಣಾಙ್ಕ depth = mv[0].depth();
    ತಾರ್ಕಿಕ allch1 = ಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ k, cn = 0;
    size_t i;

    ಪ್ರತಿ( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    ಯದಿ( n == 1 )
    {
        mv[0].copyTo(dst);
        ಪ್ರತ್ಯಾಗಮ;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (ಪೂರ್ಣಾಙ್ಕ)n));

    ಯದಿ( !allch1 )
    {
        AutoBuffer<ಪೂರ್ಣಾಙ್ಕ> pairs(cn*2);
        ಪೂರ್ಣಾಙ್ಕ j, ni=0;

        ಪ್ರತಿ( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            ಪ್ರತಿ( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        ಪ್ರತ್ಯಾಗಮ;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (ಪೂರ್ಣಾಙ್ಕ)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(ಪರಿಮಾಣ(Mat*) + ಪರಿಮಾಣ(uchar*)) + 16);
    ಧ್ರುವ Mat** arrays = (ಧ್ರುವ Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    ಪ್ರತಿ( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (ಪೂರ್ಣಾಙ್ಕ)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    ಪ್ರತಿ( i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (ಧ್ರುವ ಶೂನ್ಯ**)&ptrs[1], ptrs[0], (ಪೂರ್ಣಾಙ್ಕ)bsz, cn );

            ಯದಿ( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_merge( ಒಳಹರಿವು_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    ಪೂರ್ಣಾಙ್ಕ type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    ಪ್ರತಿ (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        ಪೂರ್ಣಾಙ್ಕ itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        ಯದಿ (src[i].dims > 2)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        CV_Assert(size == src[i].size() && depth == idepth);

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    ಪೂರ್ಣಾಙ್ಕ dcn = (ಪೂರ್ಣಾಙ್ಕ)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dcn; ++i)
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
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    ಪೂರ್ಣಾಙ್ಕ argidx = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಶೂನ್ಯ merge(ಒಳಹರಿವು_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
