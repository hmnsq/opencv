// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

#ଅନ୍ତର୍ଭାଵ "split.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv { ନାମାକାଶ hal {

ଶୂନ୍ୟ split8u(ଅଚଳ uchar* src, uchar** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ split16u(ଅଚଳ ushort* src, ushort** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ split32s(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ split64s(ଅଚଳ int64* src, int64** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split64s, cv_hal_split64s, src,dst, len, cn)
    CV_CPU_DISPATCH(split64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::

/****************************************************************************************\
*                                       split & merge                                    *
\****************************************************************************************/

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*SplitFunc)(ଅଚଳ ଶୂନ୍ୟ* src, ଶୂନ୍ୟ** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn);

template<typename T, ଶୂନ୍ୟ (*fn)(ଅଚଳ T*, T**, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ଶୂନ୍ୟ splitWrap(ଅଚଳ ଶୂନ୍ୟ* src, ଶୂନ୍ୟ** dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    fn((ଅଚଳ T*)src, (T**)dst, len, cn);
}

ସ୍ଥିର SplitFunc getSplitFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, cv::hal::split32s>, splitWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    ଫେରନ୍ତୁ splitTab[depth];
}

#ifdef HAVE_IPP

ସ୍ଥିର ତାର୍କିକ ipp_split(ଅଚଳ Mat& src, Mat* mv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ଯଦି(channels != 3 && channels != 4)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        ଶୂନ୍ୟ    *dstPtrs[4] = {ଖାଲି};
        size_t   dstStep    = mv[0].step;
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            ଯଦି(dstStep != mv[i].step)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }

        ଫେରନ୍ତୁ CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.step, dstPtrs, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dstStep, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize1(), channels, 0) >= 0;
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ Mat *arrays[5] = {ଖାଲି};
        uchar     *ptrs[5]   = {ଖାଲି};
        arrays[0] = &src;

        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, 1 };

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (ଶୂନ୍ୟ**)&ptrs[1], 0, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize1(), channels, 0) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#ଅନ୍ୟଥା
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}
#endif

ଶୂନ୍ୟ split(ଅଚଳ Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, depth = src.depth(), cn = src.channels();
    ଯଦି( cn == 1 )
    {
        src.copyTo(mv[0]);
        ଫେରନ୍ତୁ;
    }

    ପାଇଁ( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(ଆକାର(Mat*) + ଆକାର(uchar*)) + 16);
    ଅଚଳ Mat** arrays = (ଅଚଳ Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    ପାଇଁ( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (ଶୂନ୍ୟ**)&ptrs[1], (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)bsz, cn );

            ଯଦି( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                ପାଇଁ( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_split( ନିବେଶ_Array _m, OutputArrayOfArrays _mv )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::split_merge_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memopTypeToStr(depth), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ଶୂନ୍ୟ split(ନିବେଶ_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    ଯଦି( m.empty() )
    {
        _mv.release();
        ଫେରନ୍ତୁ;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
