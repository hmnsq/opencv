// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "split.simd.hpp"
#অন্তর্ভাব "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv { নামাকাশ hal {

শূন্য split8u(ধ্রুব uchar* src, uchar** dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য split16u(ধ্রুব ushort* src, ushort** dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য split32s(ধ্রুব পূর্ণাঙ্ক* src, পূর্ণাঙ্ক** dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য split64s(ধ্রুব int64* src, int64** dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn )
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

প্রকারনির্দেশ শূন্য (*SplitFunc)(ধ্রুব শূন্য* src, শূন্য** dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn);

template<typename T, শূন্য (*fn)(ধ্রুব T*, T**, পূর্ণাঙ্ক, পূর্ণাঙ্ক)>
স্থির শূন্য splitWrap(ধ্রুব শূন্য* src, শূন্য** dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn)
{
    fn((ধ্রুব T*)src, (T**)dst, len, cn);
}

স্থির SplitFunc getSplitFunc(পূর্ণাঙ্ক depth)
{
    স্থির SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<পূর্ণাঙ্ক, cv::hal::split32s>, splitWrap<পূর্ণাঙ্ক, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<পূর্ণাঙ্ক, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    প্রত্যাগম splitTab[depth];
}

#ifdef HAVE_IPP

স্থির তার্কিক ipp_split(ধ্রুব Mat& src, Mat* mv, পূর্ণাঙ্ক channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    যদি(channels != 3 && channels != 4)
        প্রত্যাগম অসত্য;

    যদি(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        শূন্য    *dstPtrs[4] = {রিক্ত};
        size_t   dstStep    = mv[0].step;
        প্রতি(পূর্ণাঙ্ক i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            যদি(dstStep != mv[i].step)
                প্রত্যাগম অসত্য;
        }

        প্রত্যাগম CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (পূর্ণাঙ্ক)src.step, dstPtrs, (পূর্ণাঙ্ক)dstStep, size, (পূর্ণাঙ্ক)src.elemSize1(), channels, 0) >= 0;
    }
    অন্যথা
    {
        ধ্রুব Mat *arrays[5] = {রিক্ত};
        uchar     *ptrs[5]   = {রিক্ত};
        arrays[0] = &src;

        প্রতি(পূর্ণাঙ্ক i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (পূর্ণাঙ্ক)it.size, 1 };

        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (শূন্য**)&ptrs[1], 0, size, (পূর্ণাঙ্ক)src.elemSize1(), channels, 0) < 0)
                প্রত্যাগম অসত্য;
        }
        প্রত্যাগম সত্য;
    }
#অন্যথা
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    প্রত্যাগম অসত্য;
#endif
}
#endif

শূন্য split(ধ্রুব Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক k, depth = src.depth(), cn = src.channels();
    যদি( cn == 1 )
    {
        src.copyTo(mv[0]);
        প্রত্যাগম;
    }

    প্রতি( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(পরিমাণ(Mat*) + পরিমাণ(uchar*)) + 16);
    ধ্রুব Mat** arrays = (ধ্রুব Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    প্রতি( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        প্রতি( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (শূন্য**)&ptrs[1], (পূর্ণাঙ্ক)bsz, cn );

            যদি( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                প্রতি( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_split( প্রবেশ_Array _m, OutputArrayOfArrays _mv )
{
    পূর্ণাঙ্ক type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    প্রতি (পূর্ণাঙ্ক i = 0; i < cn; ++i)
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
        প্রত্যাগম অসত্য;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    প্রতি (পূর্ণাঙ্ক i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    পূর্ণাঙ্ক argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    প্রতি (পূর্ণাঙ্ক i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    প্রত্যাগম k.run(2, globalsize, রিক্ত, অসত্য);
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
        প্রত্যাগম;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    পূর্ণাঙ্ক depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    প্রতি (পূর্ণাঙ্ক i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
