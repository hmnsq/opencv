// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"

#അന്തര്ഭാവ "split.simd.hpp"
#അന്തര്ഭാവ "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv { നാമാകാശ hal {

ശൂന്യം split8u(സ്ഥിരം uchar* src, uchar** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം split16u(സ്ഥിരം ushort* src, ushort** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം split32s(സ്ഥിരം പൂർണ്ണസംഖ്യ* src, പൂർണ്ണസംഖ്യ** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം split64s(സ്ഥിരം int64* src, int64** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn )
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

തരനിർവചനം ശൂന്യം (*SplitFunc)(സ്ഥിരം ശൂന്യം* src, ശൂന്യം** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn);

template<typename T, ശൂന്യം (*fn)(സ്ഥിരം T*, T**, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം ശൂന്യം splitWrap(സ്ഥിരം ശൂന്യം* src, ശൂന്യം** dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn)
{
    fn((സ്ഥിരം T*)src, (T**)dst, len, cn);
}

സ്ഥിതം SplitFunc getSplitFunc(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<പൂർണ്ണസംഖ്യ, cv::hal::split32s>, splitWrap<പൂർണ്ണസംഖ്യ, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<പൂർണ്ണസംഖ്യ, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    തിരികെ splitTab[depth];
}

#ifdef HAVE_IPP

സ്ഥിതം തർക്കം ipp_split(സ്ഥിരം Mat& src, Mat* mv, പൂർണ്ണസംഖ്യ channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    എങ്കിൽ(channels != 3 && channels != 4)
        തിരികെ അസത്യം;

    എങ്കിൽ(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        ശൂന്യം    *dstPtrs[4] = {ശൂന്യ};
        size_t   dstStep    = mv[0].step;
        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            എങ്കിൽ(dstStep != mv[i].step)
                തിരികെ അസത്യം;
        }

        തിരികെ CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (പൂർണ്ണസംഖ്യ)src.step, dstPtrs, (പൂർണ്ണസംഖ്യ)dstStep, size, (പൂർണ്ണസംഖ്യ)src.elemSize1(), channels, 0) >= 0;
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം Mat *arrays[5] = {ശൂന്യ};
        uchar     *ptrs[5]   = {ശൂന്യ};
        arrays[0] = &src;

        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (പൂർണ്ണസംഖ്യ)it.size, 1 };

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (ശൂന്യം**)&ptrs[1], 0, size, (പൂർണ്ണസംഖ്യ)src.elemSize1(), channels, 0) < 0)
                തിരികെ അസത്യം;
        }
        തിരികെ സത്യം;
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    തിരികെ അസത്യം;
#endif
}
#endif

ശൂന്യം split(സ്ഥിരം Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ k, depth = src.depth(), cn = src.channels();
    എങ്കിൽ( cn == 1 )
    {
        src.copyTo(mv[0]);
        തിരികെ;
    }

    വേണ്ടി( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(വലുപ്പം(Mat*) + വലുപ്പം(uchar*)) + 16);
    സ്ഥിരം Mat** arrays = (സ്ഥിരം Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    വേണ്ടി( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (ശൂന്യം**)&ptrs[1], (പൂർണ്ണസംഖ്യ)bsz, cn );

            എങ്കിൽ( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                വേണ്ടി( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_split( നിവേശം_Array _m, OutputArrayOfArrays _mv )
{
    പൂർണ്ണസംഖ്യ type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < cn; ++i)
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
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    പൂർണ്ണസംഖ്യ argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

ശൂന്യം split(നിവേശം_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    എങ്കിൽ( m.empty() )
    {
        _mv.release();
        തിരികെ;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    പൂർണ്ണസംഖ്യ depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
