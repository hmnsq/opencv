// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"

#અન્તર્ભાવ "split.simd.hpp"
#અન્તર્ભાવ "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv { નામાકાશ hal {

શૂન્ય split8u(અચલ uchar* src, uchar** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય split16u(અચલ ushort* src, ushort** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય split32s(અચલ પૂર્ણાંક* src, પૂર્ણાંક** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય split64s(અચલ int64* src, int64** dst, પૂર્ણાંક len, પૂર્ણાંક cn )
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

પ્રકારવ્યાખ્યા શૂન્ય (*SplitFunc)(અચલ શૂન્ય* src, શૂન્ય** dst, પૂર્ણાંક len, પૂર્ણાંક cn);

template<typename T, શૂન્ય (*fn)(અચલ T*, T**, પૂર્ણાંક, પૂર્ણાંક)>
સ્થિર શૂન્ય splitWrap(અચલ શૂન્ય* src, શૂન્ય** dst, પૂર્ણાંક len, પૂર્ણાંક cn)
{
    fn((અચલ T*)src, (T**)dst, len, cn);
}

સ્થિર SplitFunc getSplitFunc(પૂર્ણાંક depth)
{
    સ્થિર SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<પૂર્ણાંક, cv::hal::split32s>, splitWrap<પૂર્ણાંક, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<પૂર્ણાંક, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    પાછા splitTab[depth];
}

#ifdef HAVE_IPP

સ્થિર તાર્કિક ipp_split(અચલ Mat& src, Mat* mv, પૂર્ણાંક channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    જો(channels != 3 && channels != 4)
        પાછા ખોટું;

    જો(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        શૂન્ય    *dstPtrs[4] = {ખાલી};
        size_t   dstStep    = mv[0].step;
        માટે(પૂર્ણાંક i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            જો(dstStep != mv[i].step)
                પાછા ખોટું;
        }

        પાછા CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (પૂર્ણાંક)src.step, dstPtrs, (પૂર્ણાંક)dstStep, size, (પૂર્ણાંક)src.elemSize1(), channels, 0) >= 0;
    }
    નહીંતર
    {
        અચલ Mat *arrays[5] = {ખાલી};
        uchar     *ptrs[5]   = {ખાલી};
        arrays[0] = &src;

        માટે(પૂર્ણાંક i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (પૂર્ણાંક)it.size, 1 };

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            જો(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (શૂન્ય**)&ptrs[1], 0, size, (પૂર્ણાંક)src.elemSize1(), channels, 0) < 0)
                પાછા ખોટું;
        }
        પાછા સાચું;
    }
#નહીંતર
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    પાછા ખોટું;
#endif
}
#endif

શૂન્ય split(અચલ Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક k, depth = src.depth(), cn = src.channels();
    જો( cn == 1 )
    {
        src.copyTo(mv[0]);
        પાછા;
    }

    માટે( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(કદ(Mat*) + કદ(uchar*)) + 16);
    અચલ Mat** arrays = (અચલ Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    માટે( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (શૂન્ય**)&ptrs[1], (પૂર્ણાંક)bsz, cn );

            જો( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                માટે( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_split( આગત_Array _m, OutputArrayOfArrays _mv )
{
    પૂર્ણાંક type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    માટે (પૂર્ણાંક i = 0; i < cn; ++i)
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
    જો (k.empty())
        પાછા ખોટું;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    માટે (પૂર્ણાંક i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    પૂર્ણાંક argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    માટે (પૂર્ણાંક i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

શૂન્ય split(આગત_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    જો( m.empty() )
    {
        _mv.release();
        પાછા;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    પૂર્ણાંક depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    માટે (પૂર્ણાંક i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
