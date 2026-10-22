// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "split.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

ਖਾਲੀ split8u(ਅਚਲ uchar* src, uchar** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ split16u(ਅਚਲ ushort* src, ushort** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ split32s(ਅਚਲ ਪੂਰਨ* src, ਪੂਰਨ** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ split64s(ਅਚਲ int64* src, int64** dst, ਪੂਰਨ len, ਪੂਰਨ cn )
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

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ਵੰਡੋ_Func)(ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ** dst, ਪੂਰਨ len, ਪੂਰਨ cn);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਖਾਲੀ (*fn)(ਅਚਲ T*, T**, ਪੂਰਨ, ਪੂਰਨ)>
ਸਥਿਰ ਖਾਲੀ ਵੰਡੋ_Wrap(ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ** dst, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    fn((ਅਚਲ T*)src, (T**)dst, len, cn);
}

ਸਥਿਰ ਵੰਡੋ_Func get_ਵੰਡੋ_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਵੰਡੋ_Func ਵੰਡੋ_Tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        ਵੰਡੋ_Wrap<uchar, cv::hal::split8u>, ਵੰਡੋ_Wrap<uchar, cv::hal::split8u>,
        ਵੰਡੋ_Wrap<ushort, cv::hal::split16u>, ਵੰਡੋ_Wrap<ushort, cv::hal::split16u>,
        ਵੰਡੋ_Wrap<ਪੂਰਨ, cv::hal::split32s>, ਵੰਡੋ_Wrap<ਪੂਰਨ, cv::hal::split32s>,
        ਵੰਡੋ_Wrap<int64_t, cv::hal::split64s>, ਵੰਡੋ_Wrap<ushort, cv::hal::split16u>,
        ਵੰਡੋ_Wrap<ushort, cv::hal::split16u>, ਵੰਡੋ_Wrap<uchar, cv::hal::split8u>,
        ਵੰਡੋ_Wrap<int64_t, cv::hal::split64s>, ਵੰਡੋ_Wrap<int64_t, cv::hal::split64s>,
        ਵੰਡੋ_Wrap<ਪੂਰਨ, cv::hal::split32s>, ਵੰਡੋ_Wrap<uchar, cv::hal::split8u>,
        ਵੰਡੋ_Wrap<uchar, cv::hal::split8u>, 0,
    };

    ਵਾਪਸ ਵੰਡੋ_Tab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP

ਸਥਿਰ ਤਾਰਕਿਕ ipp_ਵੰਡੋ(ਅਚਲ Mat& src, Mat* mv, ਪੂਰਨ channels)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ਜੇ(channels != 3 && channels != 4)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ(src.dims <= 2)
    {
        Ippi_ਮਾਪ size       = ippi_ਮਾਪ(src.size());
        ਖਾਲੀ    *dstPtrs[4] = {ਨਿਰਾ};
        size_t   dst_ਕਦਮ_3    = mv[0].ਕਦਮ;
        ਲਈ(ਪੂਰਨ i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            ਜੇ(dst_ਕਦਮ_3 != mv[i].ਕਦਮ)
                ਵਾਪਸ ਝੂਠ;
        }

        ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(llwi_Copy_ਵੰਡੋ, src.ptr(), (ਪੂਰਨ)src.ਕਦਮ, dstPtrs, (ਪੂਰਨ)dst_ਕਦਮ_3, size, (ਪੂਰਨ)src.elemSize1(), channels, 0) >= 0;
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ Mat *arrays[5] = {ਨਿਰਾ};
        uchar     *ptrs[5]   = {ਨਿਰਾ};
        arrays[0] = &src;

        ਲਈ(ਪੂਰਨ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_ਮਾਪ size = { (ਪੂਰਨ)it.size, 1 };

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ਵੰਡੋ, ptrs[0], 0, (ਖਾਲੀ**)&ptrs[1], 0, size, (ਪੂਰਨ)src.elemSize1(), channels, 0) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
        ਵਾਪਸ ਸੱਚ;
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(mv); CV_ਅਣਵਰਤਿਆ(channels);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ ਵੰਡੋ(ਅਚਲ Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ k, ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ(), cn = src.channels();
    ਜੇ( cn == 1 )
    {
        src.copy_ਮੰਜ਼ਿਲ(mv[0]);
        ਵਾਪਸ;
    }

    ਲਈ( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, ਡੂੰਘਾਈ);
    }

    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_ਵੰਡੋ(src, mv, cn));

    ਵੰਡੋ_Func func = get_ਵੰਡੋ_Func(ਡੂੰਘਾਈ);
    CV_Assert( func != 0 );

    size_t esz = src.elem_ਮਾਪ(), esz1 = src.elemSize1();
    size_t blocksize0 = (ਖੰਡ_ਮਾਪ + esz-1)/esz;
    Auto_ਬਫਰ<uchar> _buf((cn+1)*(ਆਕਾਰ(Mat*) + ਆਕਾਰ(uchar*)) + 16);
    ਅਚਲ Mat** arrays = (ਅਚਲ Mat**)_buf.ਅੰਕੜੇ();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    ਲਈ( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t ਕੁੱਲ = it.size;
    size_t blocksize = std::min((size_t)CV_ਵੰਡੋ_ਮਿਲਾਓ_MAX_ਖੰਡ_ਮਾਪ(cn), cn <= 4 ? ਕੁੱਲ : std::min(ਕੁੱਲ, blocksize0));

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( size_t j = 0; j < ਕੁੱਲ; j += blocksize )
        {
            size_t bsz = std::min(ਕੁੱਲ - j, blocksize);
            func( ptrs[0], (ਖਾਲੀ**)&ptrs[1], (ਪੂਰਨ)bsz, cn );

            ਜੇ( j + blocksize < ਕੁੱਲ )
            {
                ptrs[0] += bsz*esz;
                ਲਈ( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_ਵੰਡੋ( ਨਿਵੇਸ਼_Array _m, Output_Array_ਦਾ_Arrays _mv )
{
    ਪੂਰਨ type = _m.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    ਲਈ (ਪੂਰਨ i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::ਵੰਡੋ_ਮਿਲਾਓ_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    ਮਾਪ size = _m.size();
    _mv.create(cn, 1, ਡੂੰਘਾਈ);
    ਲਈ (ਪੂਰਨ i = 0; i < cn; ++i)
        _mv.create(size, ਡੂੰਘਾਈ, i);

    std::vector<UMat> dst;
    _mv.get_UMat_ਸਦਿਸ਼(dst);

    ਪੂਰਨ argidx = k.ਸੈੱਟ(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    ਲਈ (ਪੂਰਨ i = 0; i < cn; ++i)
        argidx = k.ਸੈੱਟ(argidx, ocl::KernelArg::Write_Only_No_ਮਾਪ(dst[i]));
    k.ਸੈੱਟ(argidx, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI);

    size_t globalsize[2] = { (size_t)size.ਚੌੜਾਈ, ((size_t)size.ਉਚਾਈ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ ਵੰਡੋ(ਨਿਵੇਸ਼_Array _m, Output_Array_ਦਾ_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ਲੜੀਵਾਰ(_m.dims() <= 2 && _mv.is_UMat_ਸਦਿਸ਼(),
               ocl_ਵੰਡੋ(_m, _mv))

    Mat m = _m.getMat();
    ਜੇ( m.empty() )
    {
        _mv.release();
        ਵਾਪਸ;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.ਡੂੰਘਾਈ() );

    ਪੂਰਨ ਡੂੰਘਾਈ = m.ਡੂੰਘਾਈ(), cn = m.channels();
    _mv.create(cn, 1, ਡੂੰਘਾਈ);
    ਲਈ (ਪੂਰਨ i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, ਡੂੰਘਾਈ, i);

    std::vector<Mat> dst;
    _mv.get_Mat_ਸਦਿਸ਼(dst);

    ਵੰਡੋ(m, &dst[0]);
}

} // namespace
