// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "merge.simd.hpp"
#अन्तर्भाव "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv { नामाकाश hal {

शून्य merge8u(स्थिरांक uchar** src, uchar* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य merge16u(स्थिरांक ushort** src, ushort* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य merge32s(स्थिरांक पूर्णांक** src, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य merge64s(स्थिरांक int64** src, int64* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


प्रकारव्याख्या शून्य (*MergeFunc)(स्थिरांक शून्य** src, शून्य* dst, पूर्णांक len, पूर्णांक cn);

ढाँचा<typename T, शून्य (*fn)(स्थिरांक T**, T*, पूर्णांक, पूर्णांक)>
स्थिर शून्य mergeWrap(स्थिरांक शून्य** src, शून्य* dst, पूर्णांक len, पूर्णांक cn)
{
    fn((स्थिरांक T**)src, (T*)dst, len, cn);
}

स्थिर MergeFunc getMergeFunc(पूर्णांक depth)
{
    स्थिर MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<पूर्णांक, cv::hal::merge32s>, mergeWrap<पूर्णांक, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<पूर्णांक, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    परत mergeTab[depth];
}

#ifdef HAVE_IPP

स्थिर तार्किक ipp_merge(स्थिरांक Mat* mv, Mat& dst, पूर्णांक channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    जर(channels != 3 && channels != 4)
        परत असत्य;

    जर(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        स्थिरांक शून्य *srcPtrs[4] = {रिक्त};
        माप_प्रकार      srcStep    = mv[0].step;
        साठी(पूर्णांक i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            जर(srcStep != mv[i].step)
                परत असत्य;
        }

        परत CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (पूर्णांक)srcStep, dst.ptr(), (पूर्णांक)dst.step, size, (पूर्णांक)mv[0].elemSize1(), channels, 0) >= 0;
    }
    अन्यथा
    {
        स्थिरांक Mat *arrays[5] = {रिक्त};
        uchar     *ptrs[5]   = {रिक्त};
        arrays[0] = &dst;

        साठी(पूर्णांक i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (पूर्णांक)it.size, 1 };

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            जर(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (स्थिरांक शून्य**)&ptrs[1], 0, ptrs[0], 0, size, (पूर्णांक)mv[0].elemSize1(), channels, 0) < 0)
                परत असत्य;
        }
        परत सत्य;
    }
#अन्यथा
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    परत असत्य;
#endif
}
#endif

शून्य merge(स्थिरांक Mat* mv, माप_प्रकार n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    पूर्णांक depth = mv[0].depth();
    तार्किक allch1 = सत्य;
    पूर्णांक k, cn = 0;
    माप_प्रकार i;

    साठी( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    जर( n == 1 )
    {
        mv[0].copyTo(dst);
        परत;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (पूर्णांक)n));

    जर( !allch1 )
    {
        AutoBuffer<पूर्णांक> pairs(cn*2);
        पूर्णांक j, ni=0;

        साठी( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            साठी( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        परत;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    माप_प्रकार esz = dst.elemSize(), esz1 = dst.elemSize1();
    माप_प्रकार blocksize0 = (पूर्णांक)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(आकार(Mat*) + आकार(uchar*)) + 16);
    स्थिरांक Mat** arrays = (स्थिरांक Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    साठी( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    माप_प्रकार total = (पूर्णांक)it.size;
    माप_प्रकार blocksize = std::min((माप_प्रकार)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    साठी( i = 0; i < it.nplanes; i++, ++it )
    {
        साठी( माप_प्रकार j = 0; j < total; j += blocksize )
        {
            माप_प्रकार bsz = std::min(total - j, blocksize);
            func( (स्थिरांक शून्य**)&ptrs[1], ptrs[0], (पूर्णांक)bsz, cn );

            जर( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                साठी( पूर्णांक t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_merge( आदान_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    पूर्णांक type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    साठी (माप_प्रकार i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        पूर्णांक itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        जर (src[i].dims > 2)
            परत असत्य;

        CV_Assert(size == src[i].size() && depth == idepth);

        साठी (पूर्णांक cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    पूर्णांक dcn = (पूर्णांक)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    साठी (पूर्णांक i = 0; i < dcn; ++i)
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
    जर (k.empty())
        परत असत्य;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    पूर्णांक argidx = 0;
    साठी (पूर्णांक i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst.cols, ((माप_प्रकार)dst.rows + rowsPerWI - 1) / rowsPerWI };
    परत k.run(2, globalsize, रिक्त, असत्य);
}

#endif

शून्य merge(आदान_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
