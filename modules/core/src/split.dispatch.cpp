// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "split.simd.hpp"
#अन्तर्भाव "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv { नामाकाश hal {

शून्य split8u(ध्रुव uchar* src, uchar** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य split16u(ध्रुव ushort* src, ushort** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य split32s(ध्रुव पूर्णाङ्क* src, पूर्णाङ्क** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य split64s(ध्रुव int64* src, int64** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
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

प्रकारनिर्देश शून्य (*विभज_Func)(ध्रुव शून्य* src, शून्य** dst, पूर्णाङ्क len, पूर्णाङ्क cn);

प्रतिरूपम्<प्रकारनाम T, शून्य (*fn)(ध्रुव T*, T**, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर शून्य विभज_Wrap(ध्रुव शून्य* src, शून्य** dst, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    fn((ध्रुव T*)src, (T**)dst, len, cn);
}

स्थिर विभज_Func get_विभज_Func(पूर्णाङ्क गभीरता)
{
    स्थिर विभज_Func विभज_Tab[CV_गभीरता_MAX] =
    {
        विभज_Wrap<uchar, cv::hal::split8u>, विभज_Wrap<uchar, cv::hal::split8u>,
        विभज_Wrap<ushort, cv::hal::split16u>, विभज_Wrap<ushort, cv::hal::split16u>,
        विभज_Wrap<पूर्णाङ्क, cv::hal::split32s>, विभज_Wrap<पूर्णाङ्क, cv::hal::split32s>,
        विभज_Wrap<int64_t, cv::hal::split64s>, विभज_Wrap<ushort, cv::hal::split16u>,
        विभज_Wrap<ushort, cv::hal::split16u>, विभज_Wrap<uchar, cv::hal::split8u>,
        विभज_Wrap<int64_t, cv::hal::split64s>, विभज_Wrap<int64_t, cv::hal::split64s>,
        विभज_Wrap<पूर्णाङ्क, cv::hal::split32s>, विभज_Wrap<uchar, cv::hal::split8u>,
        विभज_Wrap<uchar, cv::hal::split8u>, 0,
    };

    प्रत्यागम विभज_Tab[गभीरता];
}

#यदि_परिभाषितम् HAVE_IPP

स्थिर तार्किक ipp_विभज(ध्रुव Mat& src, Mat* mv, पूर्णाङ्क channels)
{
#यदि_परिभाषितम् HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    यदि(channels != 3 && channels != 4)
        प्रत्यागम असत्य;

    यदि(src.dims <= 2)
    {
        Ippi_आकार size       = ippi_आकार(src.size());
        शून्य    *dstPtrs[4] = {रिक्त};
        size_t   dst_चरणः_3    = mv[0].चरणः;
        प्रति(पूर्णाङ्क i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            यदि(dst_चरणः_3 != mv[i].चरणः)
                प्रत्यागम असत्य;
        }

        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwi_Copy_विभज, src.ptr(), (पूर्णाङ्क)src.चरणः, dstPtrs, (पूर्णाङ्क)dst_चरणः_3, size, (पूर्णाङ्क)src.elemSize1(), channels, 0) >= 0;
    }
    अन्यथा
    {
        ध्रुव Mat *arrays[5] = {रिक्त};
        uchar     *ptrs[5]   = {रिक्त};
        arrays[0] = &src;

        प्रति(पूर्णाङ्क i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_आकार size = { (पूर्णाङ्क)it.size, 1 };

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwi_Copy_विभज, ptrs[0], 0, (शून्य**)&ptrs[1], 0, size, (पूर्णाङ्क)src.elemSize1(), channels, 0) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }
#अन्यथा
    CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(mv); CV_अप्रयुक्तम्(channels);
    प्रत्यागम असत्य;
#यदि_अन्तः
}
#यदि_अन्तः

शून्य विभज(ध्रुव Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क k, गभीरता = src.गभीरता(), cn = src.channels();
    यदि( cn == 1 )
    {
        src.copy_गन्तव्यम्(mv[0]);
        प्रत्यागम;
    }

    प्रति( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, गभीरता);
    }

    CV_IPP_सततम्_FAST(ipp_विभज(src, mv, cn));

    विभज_Func func = get_विभज_Func(गभीरता);
    CV_Assert( func != 0 );

    size_t esz = src.elem_आकार(), esz1 = src.elemSize1();
    size_t blocksize0 = (खण्डम्_आकार + esz-1)/esz;
    Auto_बफर<uchar> _buf((cn+1)*(परिमाण(Mat*) + परिमाण(uchar*)) + 16);
    ध्रुव Mat** arrays = (ध्रुव Mat**)_buf.दत्त();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    प्रति( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t योग = it.size;
    size_t blocksize = std::min((size_t)CV_विभज_MERGE_MAX_खण्डम्_आकार(cn), cn <= 4 ? योग : std::min(योग, blocksize0));

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( size_t j = 0; j < योग; j += blocksize )
        {
            size_t bsz = std::min(योग - j, blocksize);
            func( ptrs[0], (शून्य**)&ptrs[1], (पूर्णाङ्क)bsz, cn );

            यदि( j + blocksize < योग )
            {
                ptrs[0] += bsz*esz;
                प्रति( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#यदि_परिभाषितम् HAVE_OPENCL

स्थिर तार्किक ocl_विभज( निवेशः_Array _m, Output_Array_सम्बन्धि_Arrays _mv )
{
    पूर्णाङ्क type = _m.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type),
            पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::विभज_merge_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_गन्तव्यम्_Str(गभीरता), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    यदि (k.empty())
        प्रत्यागम असत्य;

    आकार size = _m.size();
    _mv.create(cn, 1, गभीरता);
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
        _mv.create(size, गभीरता, i);

    std::vector<UMat> dst;
    _mv.get_UMat_सदिशम्(dst);

    पूर्णाङ्क argidx = k.समुच्चयः(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
        argidx = k.समुच्चयः(argidx, ocl::KernelArg::Write_Only_No_आकार(dst[i]));
    k.समुच्चयः(argidx, पङ्क्तिकाः_प्रत्येकम्_WI);

    size_t globalsize[2] = { (size_t)size.विस्तारः, ((size_t)size.height + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

#यदि_अन्तः

शून्य विभज(निवेशः_Array _m, Output_Array_सम्बन्धि_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_सततम्(_m.dims() <= 2 && _mv.is_UMat_सदिशम्(),
               ocl_विभज(_m, _mv))

    Mat m = _m.getMat();
    यदि( m.empty() )
    {
        _mv.release();
        प्रत्यागम;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.गभीरता() );

    पूर्णाङ्क गभीरता = m.गभीरता(), cn = m.channels();
    _mv.create(cn, 1, गभीरता);
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, गभीरता, i);

    std::vector<Mat> dst;
    _mv.get_Mat_सदिशम्(dst);

    विभज(m, &dst[0]);
}

} // namespace
