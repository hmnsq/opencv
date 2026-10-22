// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"

#समावेश "merge.simd.hpp"
#समावेश "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

व्योम merge8u(स्थिर uchar** src, uchar* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम merge16u(स्थिर ushort** src, ushort* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम merge32s(स्थिर पूर्णांक** src, पूर्णांक* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम merge64s(स्थिर int64** src, int64* dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


प्रकार व्योम (*MergeFunc)(स्थिर व्योम** src, व्योम* dst, पूर्णांक len, पूर्णांक cn);

ढाँचा<typename T, व्योम (*fn)(स्थिर T**, T*, पूर्णांक, पूर्णांक)>
अटल व्योम mergeWrap(स्थिर व्योम** src, व्योम* dst, पूर्णांक len, पूर्णांक cn)
{
    fn((स्थिर T**)src, (T*)dst, len, cn);
}

अटल MergeFunc getMergeFunc(पूर्णांक depth)
{
    अटल MergeFunc mergeTab[CV_DEPTH_MAX] =
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

    वापस mergeTab[depth];
}

#अगर_घोषित HAVE_IPP

अटल bool ipp_merge(स्थिर Mat* mv, Mat& dst, पूर्णांक channels)
{
#अगर_घोषित HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    अगर(channels != 3 && channels != 4)
        वापस false;

    अगर(mv[0].dims <= 2)
    {
        Ippi_आकार    size       = ippi_आकार(mv[0].size());
        स्थिर व्योम *srcPtrs[4] = {शून्य};
        माप_प्रकार      srcStep    = mv[0].step;
        क्रम(पूर्णांक i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            अगर(srcStep != mv[i].step)
                वापस false;
        }

        वापस CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (पूर्णांक)srcStep, dst.ptr(), (पूर्णांक)dst.step, size, (पूर्णांक)mv[0].elemSize1(), channels, 0) >= 0;
    }
    अन्यथा
    {
        स्थिर Mat *arrays[5] = {शून्य};
        uchar     *ptrs[5]   = {शून्य};
        arrays[0] = &dst;

        क्रम(पूर्णांक i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_आकार size = { (पूर्णांक)it.size, 1 };

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            अगर(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (स्थिर व्योम**)&ptrs[1], 0, ptrs[0], 0, size, (पूर्णांक)mv[0].elemSize1(), channels, 0) < 0)
                वापस false;
        }
        वापस true;
    }
#अन्यथा
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    वापस false;
#पूर्ण_अगर
}
#पूर्ण_अगर

व्योम merge(स्थिर Mat* mv, माप_प्रकार n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    पूर्णांक depth = mv[0].depth();
    bool allch1 = true;
    पूर्णांक k, cn = 0;
    माप_प्रकार i;

    क्रम( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    अगर( n == 1 )
    {
        mv[0].copyTo(dst);
        वापस;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (पूर्णांक)n));

    अगर( !allch1 )
    {
        Auto_बफर<पूर्णांक> pairs(cn*2);
        पूर्णांक j, ni=0;

        क्रम( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            क्रम( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        वापस;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    माप_प्रकार esz = dst.elem_आकार(), esz1 = dst.elemSize1();
    माप_प्रकार blocksize0 = (पूर्णांक)((BLOCK_आकार + esz-1)/esz);
    Auto_बफर<uchar> _buf((cn+1)*(माप(Mat*) + माप(uchar*)) + 16);
    स्थिर Mat** arrays = (स्थिर Mat**)_buf.तथ्य();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    क्रम( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    माप_प्रकार योग = (पूर्णांक)it.size;
    माप_प्रकार blocksize = std::min((माप_प्रकार)CV_SPLIT_MERGE_MAX_BLOCK_आकार(cn), cn <= 4 ? योग : std::min(योग, blocksize0));

    क्रम( i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( माप_प्रकार j = 0; j < योग; j += blocksize )
        {
            माप_प्रकार bsz = std::min(योग - j, blocksize);
            func( (स्थिर व्योम**)&ptrs[1], ptrs[0], (पूर्णांक)bsz, cn );

            अगर( j + blocksize < योग )
            {
                ptrs[0] += bsz*esz;
                क्रम( पूर्णांक t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_merge( निवेश_Array_Of_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    पूर्णांक type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    आकार size = src[0].size();

    क्रम (माप_प्रकार i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        पूर्णांक itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        अगर (src[i].dims > 2)
            वापस false;

        CV_Assert(size == src[i].size() && depth == idepth);

        क्रम (पूर्णांक cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    पूर्णांक dcn = (पूर्णांक)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    क्रम (पूर्णांक i = 0; i < dcn; ++i)
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
    अगर (k.empty())
        वापस false;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    पूर्णांक argidx = 0;
    क्रम (पूर्णांक i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::Read_Only_No_आकार(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst.cols, ((माप_प्रकार)dst.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

व्योम merge(निवेश_Array_Of_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
