// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

#అన్తర్భావ "merge.simd.hpp"
#అన్తర్భావ "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv { నామాకాశ hal {

శూన్య merge8u(స్థిరరాశి uchar** src, uchar* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య merge16u(స్థిరరాశి ushort** src, ushort* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య merge32s(స్థిరరాశి పూర్ణాంకం** src, పూర్ణాంకం* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య merge64s(స్థిరరాశి int64** src, int64* dst, పూర్ణాంకం len, పూర్ణాంకం cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


రకనిర్వచనం శూన్య (*విలీనం_Func)(స్థిరరాశి శూన్య** src, శూన్య* dst, పూర్ణాంకం len, పూర్ణాంకం cn);

మూస<రకం_పేరు T, శూన్య (*fn)(స్థిరరాశి T**, T*, పూర్ణాంకం, పూర్ణాంకం)>
స్థిర శూన్య విలీనం_Wrap(స్థిరరాశి శూన్య** src, శూన్య* dst, పూర్ణాంకం len, పూర్ణాంకం cn)
{
    fn((స్థిరరాశి T**)src, (T*)dst, len, cn);
}

స్థిర విలీనం_Func get_విలీనం_Func(పూర్ణాంకం లోతు)
{
    స్థిర విలీనం_Func విలీనం_Tab[CV_లోతు_MAX] =
    {
        విలీనం_Wrap<uchar, cv::hal::merge8u>, విలీనం_Wrap<uchar, cv::hal::merge8u>,
        విలీనం_Wrap<ushort, cv::hal::merge16u>, విలీనం_Wrap<ushort, cv::hal::merge16u>,
        విలీనం_Wrap<పూర్ణాంకం, cv::hal::merge32s>, విలీనం_Wrap<పూర్ణాంకం, cv::hal::merge32s>,
        విలీనం_Wrap<int64_t, cv::hal::merge64s>, విలీనం_Wrap<ushort, cv::hal::merge16u>,
        విలీనం_Wrap<ushort, cv::hal::merge16u>, విలీనం_Wrap<uchar, cv::hal::merge8u>,
        విలీనం_Wrap<int64_t, cv::hal::merge64s>, విలీనం_Wrap<int64_t, cv::hal::merge64s>,
        విలీనం_Wrap<పూర్ణాంకం, cv::hal::merge32s>, విలీనం_Wrap<uchar, cv::hal::merge8u>,
        విలీనం_Wrap<uchar, cv::hal::merge8u>, 0,
    };

    తిరిగి విలీనం_Tab[లోతు];
}

#నిర్వచితమైతే HAVE_IPP

స్థిర తార్కిక ipp_విలీనం(స్థిరరాశి Mat* mv, Mat& dst, పూర్ణాంకం channels)
{
#నిర్వచితమైతే HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ఒకవేళ(channels != 3 && channels != 4)
        తిరిగి అబద్ధం;

    ఒకవేళ(mv[0].dims <= 2)
    {
        Ippi_కొలత    size       = ippi_కొలత(mv[0].size());
        స్థిరరాశి శూన్య *srcPtrs[4] = {ఖాళీ};
        size_t      src_అడుగు_3    = mv[0].అడుగు;
        కొరకు(పూర్ణాంకం i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            ఒకవేళ(src_అడుగు_3 != mv[i].అడుగు)
                తిరిగి అబద్ధం;
        }

        తిరిగి CV_INSTRUMENT_FUN_IPP(llwi_Copy_విలీనం, srcPtrs, (పూర్ణాంకం)src_అడుగు_3, dst.ptr(), (పూర్ణాంకం)dst.అడుగు, size, (పూర్ణాంకం)mv[0].elemSize1(), channels, 0) >= 0;
    }
    లేకపోతే
    {
        స్థిరరాశి Mat *arrays[5] = {ఖాళీ};
        uchar     *ptrs[5]   = {ఖాళీ};
        arrays[0] = &dst;

        కొరకు(పూర్ణాంకం i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_కొలత size = { (పూర్ణాంకం)it.size, 1 };

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_విలీనం, (స్థిరరాశి శూన్య**)&ptrs[1], 0, ptrs[0], 0, size, (పూర్ణాంకం)mv[0].elemSize1(), channels, 0) < 0)
                తిరిగి అబద్ధం;
        }
        తిరిగి నిజం;
    }
#లేకపోతే
    CV_వాడని(dst); CV_వాడని(mv); CV_వాడని(channels);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}
#అయితే_ముగింపు

శూన్య విలీనం(స్థిరరాశి Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    పూర్ణాంకం లోతు = mv[0].లోతు();
    తార్కిక allch1 = నిజం;
    పూర్ణాంకం k, cn = 0;
    size_t i;

    కొరకు( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].లోతు() == లోతు);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(లోతు, cn));
    Mat dst = _dst.getMat();

    ఒకవేళ( n == 1 )
    {
        mv[0].copy_గమ్యం(dst);
        తిరిగి;
    }

    CV_IPP_నిరంతరం(allch1, ipp_విలీనం(mv, dst, (పూర్ణాంకం)n));

    ఒకవేళ( !allch1 )
    {
        Auto_బఫరు<పూర్ణాంకం> జతలు(cn*2);
        పూర్ణాంకం j, ni=0;

        కొరకు( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            కొరకు( k = 0; k < ni; k++ )
            {
                జతలు[(j+k)*2] = j + k;
                జతలు[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &జతలు[0], cn );
        తిరిగి;
    }

    విలీనం_Func func = get_విలీనం_Func(లోతు);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_కొలత(), esz1 = dst.elemSize1();
    size_t blocksize0 = (పూర్ణాంకం)((ఖండం_కొలత + esz-1)/esz);
    Auto_బఫరు<uchar> _buf((cn+1)*(పరిమాణం(Mat*) + పరిమాణం(uchar*)) + 16);
    స్థిరరాశి Mat** arrays = (స్థిరరాశి Mat**)_buf.దత్తాంశం();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    కొరకు( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t మొత్తం = (పూర్ణాంకం)it.size;
    size_t blocksize = std::min((size_t)CV_విభజించు_విలీనం_MAX_ఖండం_కొలత(cn), cn <= 4 ? మొత్తం : std::min(మొత్తం, blocksize0));

    కొరకు( i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( size_t j = 0; j < మొత్తం; j += blocksize )
        {
            size_t bsz = std::min(మొత్తం - j, blocksize);
            func( (స్థిరరాశి శూన్య**)&ptrs[1], ptrs[0], (పూర్ణాంకం)bsz, cn );

            ఒకవేళ( j + blocksize < మొత్తం )
            {
                ptrs[0] += bsz*esz;
                కొరకు( పూర్ణాంకం t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_విలీనం( ఆదానం_Array_యొక్క_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_సదిశ(src);
    CV_Assert(!src.empty());

    పూర్ణాంకం type = src[0].type(), లోతు = CV_MAT_లోతు(type),
            అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    కొలత size = src[0].size();

    కొరకు (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        పూర్ణాంకం itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_లోతు(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        ఒకవేళ (src[i].dims > 2)
            తిరిగి అబద్ధం;

        CV_Assert(size == src[i].size() && లోతు == idepth);

        కొరకు (పూర్ణాంకం cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    పూర్ణాంకం dcn = (పూర్ణాంకం)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    కొరకు (పూర్ణాంకం i = 0; i < dcn; ++i)
    {
        srcargs += format("DECLARE_SRC_PARAM(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        cndecl += format(" -D scn%d=%d", i, ksrc[i].channels());
    }

    ocl::Kernel k("merge", ocl::core::విభజించు_విలీనం_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_గమ్యం_Str(లోతు), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    _dst.create(size, CV_MAKE_TYPE(లోతు, dcn));
    UMat dst = _dst.getUMat();

    పూర్ణాంకం argidx = 0;
    కొరకు (పూర్ణాంకం i = 0; i < dcn; ++i)
        argidx = k.సమితి(argidx, ocl::KernelArg::Read_Only_No_కొలత(ksrc[i]));
    argidx = k.సమితి(argidx, ocl::KernelArg::WriteOnly(dst));
    k.సమితి(argidx, అడ్డువరుసలు_చొప్పున_WI);

    size_t globalsize[2] = { (size_t)dst.నిలువుల_సంఖ్య, ((size_t)dst.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

శూన్య విలీనం(ఆదానం_Array_యొక్క_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_నిరంతరం(_mv.is_UMat_సదిశ() && _dst.isUMat(),
               ocl_విలీనం(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_సదిశ(mv);
    విలీనం(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
