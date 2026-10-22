// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "split.simd.hpp"
#ضمن "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv { نطاق hal {

فراغ split8u(ثابت uchar* src, uchar** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ split16u(ثابت ushort* src, ushort** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ split32s(ثابت صحيح* src, صحيح** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ split64s(ثابت int64* src, int64** dst, صحيح len, صحيح cn )
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

تعريفنوع فراغ (*SplitFunc)(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn);

template<typename T, فراغ (*fn)(ثابت T*, T**, صحيح, صحيح)>
ساكن فراغ splitWrap(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn)
{
    fn((ثابت T*)src, (T**)dst, len, cn);
}

ساكن SplitFunc getSplitFunc(صحيح depth)
{
    ساكن SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<صحيح, cv::hal::split32s>, splitWrap<صحيح, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<صحيح, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    أرجع splitTab[depth];
}

#ifdef HAVE_IPP

ساكن منطقي ipp_split(ثابت Mat& src, Mat* mv, صحيح channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    𐡄𐡍(channels != 3 && channels != 4)
        أرجع خطأ;

    𐡄𐡍(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        فراغ    *dstPtrs[4] = {عدم};
        size_t   dstStep    = mv[0].step;
        لكل(صحيح i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            𐡄𐡍(dstStep != mv[i].step)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (صحيح)src.step, dstPtrs, (صحيح)dstStep, size, (صحيح)src.elemSize1(), channels, 0) >= 0;
    }
    وإلا
    {
        ثابت Mat *arrays[5] = {عدم};
        uchar     *ptrs[5]   = {عدم};
        arrays[0] = &src;

        لكل(صحيح i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (صحيح)it.size, 1 };

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            𐡄𐡍(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (فراغ**)&ptrs[1], 0, size, (صحيح)src.elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع 𐡒𐡔𐡈;
    }
#وإلا
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    أرجع خطأ;
#endif
}
#endif

فراغ split(ثابت Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    صحيح k, depth = src.depth(), cn = src.channels();
    𐡄𐡍( cn == 1 )
    {
        src.copyTo(mv[0]);
        أرجع;
    }

    لكل( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    لكل( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (فراغ**)&ptrs[1], (صحيح)bsz, cn );

            𐡄𐡍( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                لكل( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

ساكن منطقي ocl_split( InputArray _m, OutputArrayOfArrays _mv )
{
    صحيح type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    لكل (صحيح i = 0; i < cn; ++i)
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
    𐡄𐡍 (k.empty())
        أرجع خطأ;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    صحيح argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    لكل (صحيح i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, عدم, خطأ);
}

#endif

فراغ split(InputArray _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    𐡄𐡍( m.empty() )
    {
        _mv.release();
        أرجع;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    صحيح depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
