// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

#అన్తర్భావ "split.simd.hpp"
#అన్తర్భావ "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv { నామాకాశ hal {

శూన్య split8u(స్థిరరాశి uchar* src, uchar** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య split16u(స్థిరరాశి ushort* src, ushort** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య split32s(స్థిరరాశి పూర్ణాంకం* src, పూర్ణాంకం** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య split64s(స్థిరరాశి int64* src, int64** dst, పూర్ణాంకం len, పూర్ణాంకం cn )
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

రకనిర్వచనం శూన్య (*విభజించు_Func)(స్థిరరాశి శూన్య* src, శూన్య** dst, పూర్ణాంకం len, పూర్ణాంకం cn);

మూస<రకం_పేరు T, శూన్య (*fn)(స్థిరరాశి T*, T**, పూర్ణాంకం, పూర్ణాంకం)>
స్థిర శూన్య విభజించు_Wrap(స్థిరరాశి శూన్య* src, శూన్య** dst, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    fn((స్థిరరాశి T*)src, (T**)dst, len, cn);
}

స్థిర విభజించు_Func get_విభజించు_Func(పూర్ణాంకం లోతు)
{
    స్థిర విభజించు_Func విభజించు_Tab[CV_లోతు_MAX] =
    {
        విభజించు_Wrap<uchar, cv::hal::split8u>, విభజించు_Wrap<uchar, cv::hal::split8u>,
        విభజించు_Wrap<ushort, cv::hal::split16u>, విభజించు_Wrap<ushort, cv::hal::split16u>,
        విభజించు_Wrap<పూర్ణాంకం, cv::hal::split32s>, విభజించు_Wrap<పూర్ణాంకం, cv::hal::split32s>,
        విభజించు_Wrap<int64_t, cv::hal::split64s>, విభజించు_Wrap<ushort, cv::hal::split16u>,
        విభజించు_Wrap<ushort, cv::hal::split16u>, విభజించు_Wrap<uchar, cv::hal::split8u>,
        విభజించు_Wrap<int64_t, cv::hal::split64s>, విభజించు_Wrap<int64_t, cv::hal::split64s>,
        విభజించు_Wrap<పూర్ణాంకం, cv::hal::split32s>, విభజించు_Wrap<uchar, cv::hal::split8u>,
        విభజించు_Wrap<uchar, cv::hal::split8u>, 0,
    };

    తిరిగి విభజించు_Tab[లోతు];
}

#నిర్వచితమైతే HAVE_IPP

స్థిర తార్కిక ipp_విభజించు(స్థిరరాశి Mat& src, Mat* mv, పూర్ణాంకం channels)
{
#నిర్వచితమైతే HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ఒకవేళ(channels != 3 && channels != 4)
        తిరిగి అబద్ధం;

    ఒకవేళ(src.dims <= 2)
    {
        Ippi_కొలత size       = ippi_కొలత(src.size());
        శూన్య    *dstPtrs[4] = {ఖాళీ};
        size_t   dst_అడుగు_3    = mv[0].అడుగు;
        కొరకు(పూర్ణాంకం i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            ఒకవేళ(dst_అడుగు_3 != mv[i].అడుగు)
                తిరిగి అబద్ధం;
        }

        తిరిగి CV_INSTRUMENT_FUN_IPP(llwi_Copy_విభజించు, src.ptr(), (పూర్ణాంకం)src.అడుగు, dstPtrs, (పూర్ణాంకం)dst_అడుగు_3, size, (పూర్ణాంకం)src.elemSize1(), channels, 0) >= 0;
    }
    లేకపోతే
    {
        స్థిరరాశి Mat *arrays[5] = {ఖాళీ};
        uchar     *ptrs[5]   = {ఖాళీ};
        arrays[0] = &src;

        కొరకు(పూర్ణాంకం i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_కొలత size = { (పూర్ణాంకం)it.size, 1 };

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_విభజించు, ptrs[0], 0, (శూన్య**)&ptrs[1], 0, size, (పూర్ణాంకం)src.elemSize1(), channels, 0) < 0)
                తిరిగి అబద్ధం;
        }
        తిరిగి నిజం;
    }
#లేకపోతే
    CV_వాడని(src); CV_వాడని(mv); CV_వాడని(channels);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}
#అయితే_ముగింపు

శూన్య విభజించు(స్థిరరాశి Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం k, లోతు = src.లోతు(), cn = src.channels();
    ఒకవేళ( cn == 1 )
    {
        src.copy_గమ్యం(mv[0]);
        తిరిగి;
    }

    కొరకు( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, లోతు);
    }

    CV_IPP_నిరంతరం_FAST(ipp_విభజించు(src, mv, cn));

    విభజించు_Func func = get_విభజించు_Func(లోతు);
    CV_Assert( func != 0 );

    size_t esz = src.elem_కొలత(), esz1 = src.elemSize1();
    size_t blocksize0 = (ఖండం_కొలత + esz-1)/esz;
    Auto_బఫరు<uchar> _buf((cn+1)*(పరిమాణం(Mat*) + పరిమాణం(uchar*)) + 16);
    స్థిరరాశి Mat** arrays = (స్థిరరాశి Mat**)_buf.దత్తాంశం();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    కొరకు( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t మొత్తం = it.size;
    size_t blocksize = std::min((size_t)CV_విభజించు_విలీనం_MAX_ఖండం_కొలత(cn), cn <= 4 ? మొత్తం : std::min(మొత్తం, blocksize0));

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( size_t j = 0; j < మొత్తం; j += blocksize )
        {
            size_t bsz = std::min(మొత్తం - j, blocksize);
            func( ptrs[0], (శూన్య**)&ptrs[1], (పూర్ణాంకం)bsz, cn );

            ఒకవేళ( j + blocksize < మొత్తం )
            {
                ptrs[0] += bsz*esz;
                కొరకు( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_విభజించు( ఆదానం_Array _m, Output_Array_యొక్క_Arrays _mv )
{
    పూర్ణాంకం type = _m.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type),
            అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    కొరకు (పూర్ణాంకం i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::విభజించు_విలీనం_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_గమ్యం_Str(లోతు), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    కొలత size = _m.size();
    _mv.create(cn, 1, లోతు);
    కొరకు (పూర్ణాంకం i = 0; i < cn; ++i)
        _mv.create(size, లోతు, i);

    std::vector<UMat> dst;
    _mv.get_UMat_సదిశ(dst);

    పూర్ణాంకం argidx = k.సమితి(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    కొరకు (పూర్ణాంకం i = 0; i < cn; ++i)
        argidx = k.సమితి(argidx, ocl::KernelArg::Write_Only_No_కొలత(dst[i]));
    k.సమితి(argidx, అడ్డువరుసలు_చొప్పున_WI);

    size_t globalsize[2] = { (size_t)size.వెడల్పు, ((size_t)size.ఎత్తు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

శూన్య విభజించు(ఆదానం_Array _m, Output_Array_యొక్క_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_నిరంతరం(_m.dims() <= 2 && _mv.is_UMat_సదిశ(),
               ocl_విభజించు(_m, _mv))

    Mat m = _m.getMat();
    ఒకవేళ( m.empty() )
    {
        _mv.release();
        తిరిగి;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.లోతు() );

    పూర్ణాంకం లోతు = m.లోతు(), cn = m.channels();
    _mv.create(cn, 1, లోతు);
    కొరకు (పూర్ణాంకం i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, లోతు, i);

    std::vector<Mat> dst;
    _mv.get_Mat_సదిశ(dst);

    విభజించు(m, &dst[0]);
}

} // namespace
