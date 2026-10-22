// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "split.simd.hpp"
#شامل "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

فضاینام cv { فضاینام hal {

تهی split8u(ثابت uchar* src, uchar** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی split16u(ثابت ushort* src, ushort** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی split32s(ثابت صحیح* src, صحیح** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی split64s(ثابت int64* src, int64** dst, صحیح len, صحیح cn )
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

تعریفنوع تهی (*SplitFunc)(ثابت تهی* src, تهی** dst, صحیح len, صحیح cn);

template<typename T, تهی (*fn)(ثابت T*, T**, صحیح, صحیح)>
ایستا تهی splitWrap(ثابت تهی* src, تهی** dst, صحیح len, صحیح cn)
{
    fn((ثابت T*)src, (T**)dst, len, cn);
}

ایستا SplitFunc getSplitFunc(صحیح depth)
{
    ایستا SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<صحیح, cv::hal::split32s>, splitWrap<صحیح, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<صحیح, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    بازگشت splitTab[depth];
}

#ifdef HAVE_IPP

ایستا منطقی ipp_split(ثابت Mat& src, Mat* mv, صحیح channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    اگر(channels != 3 && channels != 4)
        بازگشت نادرست;

    اگر(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        تهی    *dstPtrs[4] = {پوچ};
        size_t   dstStep    = mv[0].step;
        برای(صحیح i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            اگر(dstStep != mv[i].step)
                بازگشت نادرست;
        }

        بازگشت CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (صحیح)src.step, dstPtrs, (صحیح)dstStep, size, (صحیح)src.elemSize1(), channels, 0) >= 0;
    }
    وگرنه
    {
        ثابت Mat *arrays[5] = {پوچ};
        uchar     *ptrs[5]   = {پوچ};
        arrays[0] = &src;

        برای(صحیح i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (صحیح)it.size, 1 };

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (تهی**)&ptrs[1], 0, size, (صحیح)src.elemSize1(), channels, 0) < 0)
                بازگشت نادرست;
        }
        بازگشت درست;
    }
#وگرنه
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    بازگشت نادرست;
#endif
}
#endif

تهی split(ثابت Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    صحیح k, depth = src.depth(), cn = src.channels();
    اگر( cn == 1 )
    {
        src.copyTo(mv[0]);
        بازگشت;
    }

    برای( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(اندازه(Mat*) + اندازه(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    برای( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (تهی**)&ptrs[1], (صحیح)bsz, cn );

            اگر( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                برای( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

ایستا منطقی ocl_split( درون_داد_Array _m, OutputArrayOfArrays _mv )
{
    صحیح type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    برای (صحیح i = 0; i < cn; ++i)
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
    اگر (k.empty())
        بازگشت نادرست;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    برای (صحیح i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    صحیح argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    برای (صحیح i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

تهی split(درون_داد_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    اگر( m.empty() )
    {
        _mv.release();
        بازگشت;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    صحیح depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    برای (صحیح i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
