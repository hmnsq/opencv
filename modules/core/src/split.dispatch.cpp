// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"

#அந்தர்பாவ "split.simd.hpp"
#அந்தர்பாவ "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv { நாமாகாஶ hal {

வெறுமை split8u(மாறிலி uchar* src, uchar** dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை split16u(மாறிலி ushort* src, ushort** dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை split32s(மாறிலி முழு* src, முழு** dst, முழு len, முழு cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை split64s(மாறிலி int64* src, int64** dst, முழு len, முழு cn )
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

வகைவரை வெறுமை (*SplitFunc)(மாறிலி வெறுமை* src, வெறுமை** dst, முழு len, முழு cn);

template<typename T, வெறுமை (*fn)(மாறிலி T*, T**, முழு, முழு)>
நிலையான வெறுமை splitWrap(மாறிலி வெறுமை* src, வெறுமை** dst, முழு len, முழு cn)
{
    fn((மாறிலி T*)src, (T**)dst, len, cn);
}

நிலையான SplitFunc getSplitFunc(முழு depth)
{
    நிலையான SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<முழு, cv::hal::split32s>, splitWrap<முழு, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<முழு, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    திரும்பு splitTab[depth];
}

#ifdef HAVE_IPP

நிலையான தருக்க ipp_split(மாறிலி Mat& src, Mat* mv, முழு channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    எனில்(channels != 3 && channels != 4)
        திரும்பு பொய்;

    எனில்(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        வெறுமை    *dstPtrs[4] = {வெற்று};
        size_t   dstStep    = mv[0].step;
        க்கு(முழு i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            எனில்(dstStep != mv[i].step)
                திரும்பு பொய்;
        }

        திரும்பு CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (முழு)src.step, dstPtrs, (முழு)dstStep, size, (முழு)src.elemSize1(), channels, 0) >= 0;
    }
    இல்லையெனில்
    {
        மாறிலி Mat *arrays[5] = {வெற்று};
        uchar     *ptrs[5]   = {வெற்று};
        arrays[0] = &src;

        க்கு(முழு i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (முழு)it.size, 1 };

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (வெறுமை**)&ptrs[1], 0, size, (முழு)src.elemSize1(), channels, 0) < 0)
                திரும்பு பொய்;
        }
        திரும்பு உண்மை;
    }
#இல்லையெனில்
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    திரும்பு பொய்;
#endif
}
#endif

வெறுமை split(மாறிலி Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    முழு k, depth = src.depth(), cn = src.channels();
    எனில்( cn == 1 )
    {
        src.copyTo(mv[0]);
        திரும்பு;
    }

    க்கு( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(அளவு(Mat*) + அளவு(uchar*)) + 16);
    மாறிலி Mat** arrays = (மாறிலி Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    க்கு( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (வெறுமை**)&ptrs[1], (முழு)bsz, cn );

            எனில்( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                க்கு( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_split( உள்ளீடு_Array _m, OutputArrayOfArrays _mv )
{
    முழு type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    க்கு (முழு i = 0; i < cn; ++i)
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
    எனில் (k.empty())
        திரும்பு பொய்;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    க்கு (முழு i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    முழு argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    க்கு (முழு i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

வெறுமை split(உள்ளீடு_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    எனில்( m.empty() )
    {
        _mv.release();
        திரும்பு;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    முழு depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    க்கு (முழு i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
