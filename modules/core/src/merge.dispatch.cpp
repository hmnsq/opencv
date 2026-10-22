// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "merge.simd.hpp"
#ضمن "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv { نطاق hal {

فراغ merge8u(ثابت uchar** src, uchar* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ merge16u(ثابت ushort** src, ushort* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ merge32s(ثابت صحيح** src, صحيح* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ merge64s(ثابت int64** src, int64* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


تعريفنوع فراغ (*MergeFunc)(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn);

template<typename T, فراغ (*fn)(ثابت T**, T*, صحيح, صحيح)>
ساكن فراغ mergeWrap(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn)
{
    fn((ثابت T**)src, (T*)dst, len, cn);
}

ساكن MergeFunc getMergeFunc(صحيح depth)
{
    ساكن MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<صحيح, cv::hal::merge32s>, mergeWrap<صحيح, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<صحيح, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    أرجع mergeTab[depth];
}

#ifdef HAVE_IPP

ساكن منطقي ipp_merge(ثابت Mat* mv, Mat& dst, صحيح channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        ثابت فراغ *srcPtrs[4] = {عدم};
        size_t      srcStep    = mv[0].step;
        لكل(صحيح i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            إذا(srcStep != mv[i].step)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (صحيح)srcStep, dst.ptr(), (صحيح)dst.step, size, (صحيح)mv[0].elemSize1(), channels, 0) >= 0;
    }
    وإلا
    {
        ثابت Mat *arrays[5] = {عدم};
        uchar     *ptrs[5]   = {عدم};
        arrays[0] = &dst;

        لكل(صحيح i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (صحيح)it.size, 1 };

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (ثابت فراغ**)&ptrs[1], 0, ptrs[0], 0, size, (صحيح)mv[0].elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع true;
    }
#وإلا
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    أرجع خطأ;
#endif
}
#endif

فراغ merge(ثابت Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    صحيح depth = mv[0].depth();
    منطقي allch1 = true;
    صحيح k, cn = 0;
    size_t i;

    لكل( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    إذا( n == 1 )
    {
        mv[0].copyTo(dst);
        أرجع;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (صحيح)n));

    إذا( !allch1 )
    {
        AutoBuffer<صحيح> pairs(cn*2);
        صحيح j, ni=0;

        لكل( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            لكل( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        أرجع;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (صحيح)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    لكل( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (صحيح)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    لكل( i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (ثابت فراغ**)&ptrs[1], ptrs[0], (صحيح)bsz, cn );

            إذا( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                لكل( صحيح t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

ساكن منطقي ocl_merge( شغرو_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    صحيح type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    لكل (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        صحيح itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        إذا (src[i].dims > 2)
            أرجع خطأ;

        CV_Assert(size == src[i].size() && depth == idepth);

        لكل (صحيح cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    صحيح dcn = (صحيح)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    لكل (صحيح i = 0; i < dcn; ++i)
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
    إذا (k.empty())
        أرجع خطأ;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    صحيح argidx = 0;
    لكل (صحيح i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, عدم, خطأ);
}

#endif

فراغ merge(شغرو_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
