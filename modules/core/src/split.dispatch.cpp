// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "split.simd.hpp"
#ಅನ್ತರ್ಭಾವ "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಶೂನ್ಯ split8u(ಧ್ರುವ uchar* src, uchar** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ split16u(ಧ್ರುವ ushort* src, ushort** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ split32s(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* src, ಪೂರ್ಣಾಙ್ಕ** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ split64s(ಧ್ರುವ int64* src, int64** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn )
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

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*SplitFunc)(ಧ್ರುವ ಶೂನ್ಯ* src, ಶೂನ್ಯ** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn);

template<typename T, ಶೂನ್ಯ (*fn)(ಧ್ರುವ T*, T**, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ splitWrap(ಧ್ರುವ ಶೂನ್ಯ* src, ಶೂನ್ಯ** dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn)
{
    fn((ಧ್ರುವ T*)src, (T**)dst, len, cn);
}

ಸ್ಥಿರ SplitFunc getSplitFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ಪೂರ್ಣಾಙ್ಕ, cv::hal::split32s>, splitWrap<ಪೂರ್ಣಾಙ್ಕ, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<ಪೂರ್ಣಾಙ್ಕ, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    ಪ್ರತ್ಯಾಗಮ splitTab[depth];
}

#ifdef HAVE_IPP

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_split(ಧ್ರುವ Mat& src, Mat* mv, ಪೂರ್ಣಾಙ್ಕ channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಯದಿ(channels != 3 && channels != 4)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        ಶೂನ್ಯ    *dstPtrs[4] = {ರಿಕ್ತ};
        size_t   dstStep    = mv[0].step;
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            ಯದಿ(dstStep != mv[i].step)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }

        ಪ್ರತ್ಯಾಗಮ CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (ಪೂರ್ಣಾಙ್ಕ)src.step, dstPtrs, (ಪೂರ್ಣಾಙ್ಕ)dstStep, size, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize1(), channels, 0) >= 0;
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ Mat *arrays[5] = {ರಿಕ್ತ};
        uchar     *ptrs[5]   = {ರಿಕ್ತ};
        arrays[0] = &src;

        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (ಪೂರ್ಣಾಙ್ಕ)it.size, 1 };

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (ಶೂನ್ಯ**)&ptrs[1], 0, size, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize1(), channels, 0) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}
#endif

ಶೂನ್ಯ split(ಧ್ರುವ Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ k, depth = src.depth(), cn = src.channels();
    ಯದಿ( cn == 1 )
    {
        src.copyTo(mv[0]);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿ( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(ಪರಿಮಾಣ(Mat*) + ಪರಿಮಾಣ(uchar*)) + 16);
    ಧ್ರುವ Mat** arrays = (ಧ್ರುವ Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    ಪ್ರತಿ( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (ಶೂನ್ಯ**)&ptrs[1], (ಪೂರ್ಣಾಙ್ಕ)bsz, cn );

            ಯದಿ( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                ಪ್ರತಿ( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_split( ಒಳಹರಿವು_Array _m, OutputArrayOfArrays _mv )
{
    ಪೂರ್ಣಾಙ್ಕ type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < cn; ++i)
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
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    ಪೂರ್ಣಾಙ್ಕ argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಶೂನ್ಯ split(ಒಳಹರಿವು_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    ಯದಿ( m.empty() )
    {
        _mv.release();
        ಪ್ರತ್ಯಾಗಮ;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    ಪೂರ್ಣಾಙ್ಕ depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
