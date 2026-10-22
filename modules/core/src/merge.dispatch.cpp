// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "merge.simd.hpp"
#شامل "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

فضاینام cv { فضاینام hal {

تهی merge8u(ثابت uchar** src, uchar* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی merge16u(ثابت ushort** src, ushort* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی merge32s(ثابت صحیح** src, صحیح* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی merge64s(ثابت int64** src, int64* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


تعریفنوع تهی (*MergeFunc)(ثابت تهی** src, تهی* dst, صحیح len, صحیح cn);

template<typename T, تهی (*fn)(ثابت T**, T*, صحیح, صحیح)>
ایستا تهی mergeWrap(ثابت تهی** src, تهی* dst, صحیح len, صحیح cn)
{
    fn((ثابت T**)src, (T*)dst, len, cn);
}

ایستا MergeFunc getMergeFunc(صحیح depth)
{
    ایستا MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<صحیح, cv::hal::merge32s>, mergeWrap<صحیح, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<صحیح, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    بازگشت mergeTab[depth];
}

#ifdef HAVE_IPP

ایستا منطقی ipp_merge(ثابت Mat* mv, Mat& dst, صحیح channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    اگر(channels != 3 && channels != 4)
        بازگشت نادرست;

    اگر(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        ثابت تهی *srcPtrs[4] = {پوچ};
        size_t      srcStep    = mv[0].step;
        برای(صحیح i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            اگر(srcStep != mv[i].step)
                بازگشت نادرست;
        }

        بازگشت CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (صحیح)srcStep, dst.ptr(), (صحیح)dst.step, size, (صحیح)mv[0].elemSize1(), channels, 0) >= 0;
    }
    وگرنه
    {
        ثابت Mat *arrays[5] = {پوچ};
        uchar     *ptrs[5]   = {پوچ};
        arrays[0] = &dst;

        برای(صحیح i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (صحیح)it.size, 1 };

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (ثابت تهی**)&ptrs[1], 0, ptrs[0], 0, size, (صحیح)mv[0].elemSize1(), channels, 0) < 0)
                بازگشت نادرست;
        }
        بازگشت درست;
    }
#وگرنه
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    بازگشت نادرست;
#endif
}
#endif

تهی merge(ثابت Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    صحیح depth = mv[0].depth();
    منطقی allch1 = درست;
    صحیح k, cn = 0;
    size_t i;

    برای( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    اگر( n == 1 )
    {
        mv[0].copyTo(dst);
        بازگشت;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (صحیح)n));

    اگر( !allch1 )
    {
        AutoBuffer<صحیح> pairs(cn*2);
        صحیح j, ni=0;

        برای( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            برای( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        بازگشت;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (صحیح)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(اندازه(Mat*) + اندازه(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    برای( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (صحیح)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    برای( i = 0; i < it.nplanes; i++, ++it )
    {
        برای( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (ثابت تهی**)&ptrs[1], ptrs[0], (صحیح)bsz, cn );

            اگر( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                برای( صحیح t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

ایستا منطقی ocl_merge( درون_داد_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    صحیح type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    برای (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        صحیح itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        اگر (src[i].dims > 2)
            بازگشت نادرست;

        CV_Assert(size == src[i].size() && depth == idepth);

        برای (صحیح cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    صحیح dcn = (صحیح)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    برای (صحیح i = 0; i < dcn; ++i)
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
    اگر (k.empty())
        بازگشت نادرست;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    صحیح argidx = 0;
    برای (صحیح i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

تهی merge(درون_داد_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
