// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"

#અન્તર્ભાવ "merge.simd.hpp"
#અન્તર્ભાવ "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv { નામાકાશ hal {

શૂન્ય merge8u(અચલ uchar** src, uchar* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય merge16u(અચલ ushort** src, ushort* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય merge32s(અચલ પૂર્ણાંક** src, પૂર્ણાંક* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય merge64s(અચલ int64** src, int64* dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


પ્રકારવ્યાખ્યા શૂન્ય (*MergeFunc)(અચલ શૂન્ય** src, શૂન્ય* dst, પૂર્ણાંક len, પૂર્ણાંક cn);

template<typename T, શૂન્ય (*fn)(અચલ T**, T*, પૂર્ણાંક, પૂર્ણાંક)>
સ્થિર શૂન્ય mergeWrap(અચલ શૂન્ય** src, શૂન્ય* dst, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    fn((અચલ T**)src, (T*)dst, len, cn);
}

સ્થિર MergeFunc getMergeFunc(પૂર્ણાંક depth)
{
    સ્થિર MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<પૂર્ણાંક, cv::hal::merge32s>, mergeWrap<પૂર્ણાંક, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<પૂર્ણાંક, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    પાછા mergeTab[depth];
}

#ifdef HAVE_IPP

સ્થિર તાર્કિક ipp_merge(અચલ Mat* mv, Mat& dst, પૂર્ણાંક channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    જો(channels != 3 && channels != 4)
        પાછા ખોટું;

    જો(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        અચલ શૂન્ય *srcPtrs[4] = {ખાલી};
        size_t      srcStep    = mv[0].step;
        માટે(પૂર્ણાંક i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            જો(srcStep != mv[i].step)
                પાછા ખોટું;
        }

        પાછા CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (પૂર્ણાંક)srcStep, dst.ptr(), (પૂર્ણાંક)dst.step, size, (પૂર્ણાંક)mv[0].elemSize1(), channels, 0) >= 0;
    }
    નહીંતર
    {
        અચલ Mat *arrays[5] = {ખાલી};
        uchar     *ptrs[5]   = {ખાલી};
        arrays[0] = &dst;

        માટે(પૂર્ણાંક i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (પૂર્ણાંક)it.size, 1 };

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            જો(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (અચલ શૂન્ય**)&ptrs[1], 0, ptrs[0], 0, size, (પૂર્ણાંક)mv[0].elemSize1(), channels, 0) < 0)
                પાછા ખોટું;
        }
        પાછા સાચું;
    }
#નહીંતર
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    પાછા ખોટું;
#endif
}
#endif

શૂન્ય merge(અચલ Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    પૂર્ણાંક depth = mv[0].depth();
    તાર્કિક allch1 = સાચું;
    પૂર્ણાંક k, cn = 0;
    size_t i;

    માટે( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    જો( n == 1 )
    {
        mv[0].copyTo(dst);
        પાછા;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (પૂર્ણાંક)n));

    જો( !allch1 )
    {
        AutoBuffer<પૂર્ણાંક> pairs(cn*2);
        પૂર્ણાંક j, ni=0;

        માટે( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            માટે( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        પાછા;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (પૂર્ણાંક)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(કદ(Mat*) + કદ(uchar*)) + 16);
    અચલ Mat** arrays = (અચલ Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    માટે( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (પૂર્ણાંક)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    માટે( i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (અચલ શૂન્ય**)&ptrs[1], ptrs[0], (પૂર્ણાંક)bsz, cn );

            જો( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                માટે( પૂર્ણાંક t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_merge( આગત_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    પૂર્ણાંક type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    માટે (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        પૂર્ણાંક itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        જો (src[i].dims > 2)
            પાછા ખોટું;

        CV_Assert(size == src[i].size() && depth == idepth);

        માટે (પૂર્ણાંક cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    પૂર્ણાંક dcn = (પૂર્ણાંક)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    માટે (પૂર્ણાંક i = 0; i < dcn; ++i)
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
    જો (k.empty())
        પાછા ખોટું;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    પૂર્ણાંક argidx = 0;
    માટે (પૂર્ણાંક i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

શૂન્ય merge(આગત_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
