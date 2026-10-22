// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "split.simd.hpp"
#অন্তর্ভাব "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv { নামাকাশ hal {

শূন্য split8u(ধ্রুবক uchar* src, uchar** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য split16u(ধ্রুবক ushort* src, ushort** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য split32s(ধ্রুবক পূর্ণসংখ্যা* src, পূর্ণসংখ্যা** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য split64s(ধ্রুবক int64* src, int64** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn )
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

প্রকারলেখ শূন্য (*SplitFunc)(ধ্রুবক শূন্য* src, শূন্য** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn);

template<typename T, শূন্য (*fn)(ধ্রুবক T*, T**, পূর্ণসংখ্যা, পূর্ণসংখ্যা)>
স্থির শূন্য splitWrap(ধ্রুবক শূন্য* src, শূন্য** dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn)
{
    fn((ধ্রুবক T*)src, (T**)dst, len, cn);
}

স্থির SplitFunc getSplitFunc(পূর্ণসংখ্যা depth)
{
    স্থির SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<পূর্ণসংখ্যা, cv::hal::split32s>, splitWrap<পূর্ণসংখ্যা, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<পূর্ণসংখ্যা, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    ফেরত splitTab[depth];
}

#ifdef HAVE_IPP

স্থির যৌক্তিক ipp_split(ধ্রুবক Mat& src, Mat* mv, পূর্ণসংখ্যা channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    যদি(channels != 3 && channels != 4)
        ফেরত মিথ্যা;

    যদি(src.dims <= 2)
    {
        Ippi_আকার size       = ippi_আকার(src.size());
        শূন্য    *dstPtrs[4] = {ফাঁকা};
        size_t   dstStep    = mv[0].step;
        জন্য(পূর্ণসংখ্যা i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            যদি(dstStep != mv[i].step)
                ফেরত মিথ্যা;
        }

        ফেরত CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (পূর্ণসংখ্যা)src.step, dstPtrs, (পূর্ণসংখ্যা)dstStep, size, (পূর্ণসংখ্যা)src.elemSize1(), channels, 0) >= 0;
    }
    অন্যথা
    {
        ধ্রুবক Mat *arrays[5] = {ফাঁকা};
        uchar     *ptrs[5]   = {ফাঁকা};
        arrays[0] = &src;

        জন্য(পূর্ণসংখ্যা i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_আকার size = { (পূর্ণসংখ্যা)it.size, 1 };

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (শূন্য**)&ptrs[1], 0, size, (পূর্ণসংখ্যা)src.elemSize1(), channels, 0) < 0)
                ফেরত মিথ্যা;
        }
        ফেরত সত্য;
    }
#অন্যথা
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    ফেরত মিথ্যা;
#endif
}
#endif

শূন্য split(ধ্রুবক Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা k, depth = src.depth(), cn = src.channels();
    যদি( cn == 1 )
    {
        src.copyTo(mv[0]);
        ফেরত;
    }

    জন্য( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elem_আকার(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_আকার + esz-1)/esz;
    Auto_বাফার<uchar> _buf((cn+1)*(আকার(Mat*) + আকার(uchar*)) + 16);
    ধ্রুবক Mat** arrays = (ধ্রুবক Mat**)_buf.তথ্য();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    জন্য( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t যোগ = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_আকার(cn), cn <= 4 ? যোগ : std::min(যোগ, blocksize0));

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( size_t j = 0; j < যোগ; j += blocksize )
        {
            size_t bsz = std::min(যোগ - j, blocksize);
            func( ptrs[0], (শূন্য**)&ptrs[1], (পূর্ণসংখ্যা)bsz, cn );

            যদি( j + blocksize < যোগ )
            {
                ptrs[0] += bsz*esz;
                জন্য( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_split( প্রবেশ_Array _m, OutputArrayOfArrays _mv )
{
    পূর্ণসংখ্যা type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    জন্য (পূর্ণসংখ্যা i = 0; i < cn; ++i)
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
    যদি (k.empty())
        ফেরত মিথ্যা;

    আকার_2 size = _m.size();
    _mv.create(cn, 1, depth);
    জন্য (পূর্ণসংখ্যা i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    পূর্ণসংখ্যা argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    জন্য (পূর্ণসংখ্যা i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::Write_Only_No_আকার(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

শূন্য split(প্রবেশ_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    যদি( m.empty() )
    {
        _mv.release();
        ফেরত;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    পূর্ণসংখ্যা depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    জন্য (পূর্ণসংখ্যা i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
