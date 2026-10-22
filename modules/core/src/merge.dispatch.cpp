// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"

#അന്തര്ഭാവ "merge.simd.hpp"
#അന്തര്ഭാവ "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv { നാമാകാശ hal {

ശൂന്യം merge8u(സ്ഥിരം uchar** src, uchar* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം merge16u(സ്ഥിരം ushort** src, ushort* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം merge32s(സ്ഥിരം പൂർണ്ണസംഖ്യ** src, പൂർണ്ണസംഖ്യ* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം merge64s(സ്ഥിരം int64** src, int64* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


തരനിർവചനം ശൂന്യം (*MergeFunc)(സ്ഥിരം ശൂന്യം** src, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);

template<typename T, ശൂന്യം (*fn)(സ്ഥിരം T**, T*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം ശൂന്യം mergeWrap(സ്ഥിരം ശൂന്യം** src, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    fn((സ്ഥിരം T**)src, (T*)dst, len, cn);
}

സ്ഥിതം MergeFunc getMergeFunc(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<പൂർണ്ണസംഖ്യ, cv::hal::merge32s>, mergeWrap<പൂർണ്ണസംഖ്യ, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<പൂർണ്ണസംഖ്യ, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    തിരികെ mergeTab[depth];
}

#ifdef HAVE_IPP

സ്ഥിതം തർക്കം ipp_merge(സ്ഥിരം Mat* mv, Mat& dst, പൂർണ്ണസംഖ്യ channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    എങ്കിൽ(channels != 3 && channels != 4)
        തിരികെ അസത്യം;

    എങ്കിൽ(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        സ്ഥിരം ശൂന്യം *srcPtrs[4] = {ശൂന്യ};
        size_t      srcStep    = mv[0].step;
        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            എങ്കിൽ(srcStep != mv[i].step)
                തിരികെ അസത്യം;
        }

        തിരികെ CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (പൂർണ്ണസംഖ്യ)srcStep, dst.ptr(), (പൂർണ്ണസംഖ്യ)dst.step, size, (പൂർണ്ണസംഖ്യ)mv[0].elemSize1(), channels, 0) >= 0;
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം Mat *arrays[5] = {ശൂന്യ};
        uchar     *ptrs[5]   = {ശൂന്യ};
        arrays[0] = &dst;

        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (പൂർണ്ണസംഖ്യ)it.size, 1 };

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (സ്ഥിരം ശൂന്യം**)&ptrs[1], 0, ptrs[0], 0, size, (പൂർണ്ണസംഖ്യ)mv[0].elemSize1(), channels, 0) < 0)
                തിരികെ അസത്യം;
        }
        തിരികെ സത്യം;
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    തിരികെ അസത്യം;
#endif
}
#endif

ശൂന്യം merge(സ്ഥിരം Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    പൂർണ്ണസംഖ്യ depth = mv[0].depth();
    തർക്കം allch1 = സത്യം;
    പൂർണ്ണസംഖ്യ k, cn = 0;
    size_t i;

    വേണ്ടി( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    എങ്കിൽ( n == 1 )
    {
        mv[0].copyTo(dst);
        തിരികെ;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (പൂർണ്ണസംഖ്യ)n));

    എങ്കിൽ( !allch1 )
    {
        AutoBuffer<പൂർണ്ണസംഖ്യ> pairs(cn*2);
        പൂർണ്ണസംഖ്യ j, ni=0;

        വേണ്ടി( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            വേണ്ടി( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        തിരികെ;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (പൂർണ്ണസംഖ്യ)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(വലുപ്പം(Mat*) + വലുപ്പം(uchar*)) + 16);
    സ്ഥിരം Mat** arrays = (സ്ഥിരം Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    വേണ്ടി( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (പൂർണ്ണസംഖ്യ)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    വേണ്ടി( i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (സ്ഥിരം ശൂന്യം**)&ptrs[1], ptrs[0], (പൂർണ്ണസംഖ്യ)bsz, cn );

            എങ്കിൽ( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                വേണ്ടി( പൂർണ്ണസംഖ്യ t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_merge( നിവേശം_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    പൂർണ്ണസംഖ്യ type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    വേണ്ടി (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        പൂർണ്ണസംഖ്യ itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        എങ്കിൽ (src[i].dims > 2)
            തിരികെ അസത്യം;

        CV_Assert(size == src[i].size() && depth == idepth);

        വേണ്ടി (പൂർണ്ണസംഖ്യ cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    പൂർണ്ണസംഖ്യ dcn = (പൂർണ്ണസംഖ്യ)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dcn; ++i)
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
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    പൂർണ്ണസംഖ്യ argidx = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

ശൂന്യം merge(നിവേശം_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
