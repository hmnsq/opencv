// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "merge.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

ਖਾਲੀ merge8u(ਅਚਲ uchar** src, uchar* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ merge16u(ਅਚਲ ushort** src, ushort* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ merge32s(ਅਚਲ ਪੂਰਨ** src, ਪੂਰਨ* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ merge64s(ਅਚਲ int64** src, int64* dst, ਪੂਰਨ len, ਪੂਰਨ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ਮਿਲਾਓ_Func)(ਅਚਲ ਖਾਲੀ** src, ਖਾਲੀ* dst, ਪੂਰਨ len, ਪੂਰਨ cn);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਖਾਲੀ (*fn)(ਅਚਲ T**, T*, ਪੂਰਨ, ਪੂਰਨ)>
ਸਥਿਰ ਖਾਲੀ ਮਿਲਾਓ_Wrap(ਅਚਲ ਖਾਲੀ** src, ਖਾਲੀ* dst, ਪੂਰਨ len, ਪੂਰਨ cn)
{
    fn((ਅਚਲ T**)src, (T*)dst, len, cn);
}

ਸਥਿਰ ਮਿਲਾਓ_Func get_ਮਿਲਾਓ_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ ਮਿਲਾਓ_Func ਮਿਲਾਓ_Tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        ਮਿਲਾਓ_Wrap<uchar, cv::hal::merge8u>, ਮਿਲਾਓ_Wrap<uchar, cv::hal::merge8u>,
        ਮਿਲਾਓ_Wrap<ushort, cv::hal::merge16u>, ਮਿਲਾਓ_Wrap<ushort, cv::hal::merge16u>,
        ਮਿਲਾਓ_Wrap<ਪੂਰਨ, cv::hal::merge32s>, ਮਿਲਾਓ_Wrap<ਪੂਰਨ, cv::hal::merge32s>,
        ਮਿਲਾਓ_Wrap<int64_t, cv::hal::merge64s>, ਮਿਲਾਓ_Wrap<ushort, cv::hal::merge16u>,
        ਮਿਲਾਓ_Wrap<ushort, cv::hal::merge16u>, ਮਿਲਾਓ_Wrap<uchar, cv::hal::merge8u>,
        ਮਿਲਾਓ_Wrap<int64_t, cv::hal::merge64s>, ਮਿਲਾਓ_Wrap<int64_t, cv::hal::merge64s>,
        ਮਿਲਾਓ_Wrap<ਪੂਰਨ, cv::hal::merge32s>, ਮਿਲਾਓ_Wrap<uchar, cv::hal::merge8u>,
        ਮਿਲਾਓ_Wrap<uchar, cv::hal::merge8u>, 0,
    };

    ਵਾਪਸ ਮਿਲਾਓ_Tab[ਡੂੰਘਾਈ];
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP

ਸਥਿਰ ਤਾਰਕਿਕ ipp_ਮਿਲਾਓ(ਅਚਲ Mat* mv, Mat& dst, ਪੂਰਨ channels)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ਜੇ(channels != 3 && channels != 4)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ(mv[0].dims <= 2)
    {
        Ippi_ਮਾਪ    size       = ippi_ਮਾਪ(mv[0].size());
        ਅਚਲ ਖਾਲੀ *srcPtrs[4] = {ਨਿਰਾ};
        size_t      src_ਕਦਮ_3    = mv[0].ਕਦਮ;
        ਲਈ(ਪੂਰਨ i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            ਜੇ(src_ਕਦਮ_3 != mv[i].ਕਦਮ)
                ਵਾਪਸ ਝੂਠ;
        }

        ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(llwi_Copy_ਮਿਲਾਓ, srcPtrs, (ਪੂਰਨ)src_ਕਦਮ_3, dst.ptr(), (ਪੂਰਨ)dst.ਕਦਮ, size, (ਪੂਰਨ)mv[0].elemSize1(), channels, 0) >= 0;
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ Mat *arrays[5] = {ਨਿਰਾ};
        uchar     *ptrs[5]   = {ਨਿਰਾ};
        arrays[0] = &dst;

        ਲਈ(ਪੂਰਨ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_ਮਾਪ size = { (ਪੂਰਨ)it.size, 1 };

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ਮਿਲਾਓ, (ਅਚਲ ਖਾਲੀ**)&ptrs[1], 0, ptrs[0], 0, size, (ਪੂਰਨ)mv[0].elemSize1(), channels, 0) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
        ਵਾਪਸ ਸੱਚ;
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(dst); CV_ਅਣਵਰਤਿਆ(mv); CV_ਅਣਵਰਤਿਆ(channels);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ ਮਿਲਾਓ(ਅਚਲ Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    ਪੂਰਨ ਡੂੰਘਾਈ = mv[0].ਡੂੰਘਾਈ();
    ਤਾਰਕਿਕ allch1 = ਸੱਚ;
    ਪੂਰਨ k, cn = 0;
    size_t i;

    ਲਈ( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(ਡੂੰਘਾਈ, cn));
    Mat dst = _dst.getMat();

    ਜੇ( n == 1 )
    {
        mv[0].copy_ਮੰਜ਼ਿਲ(dst);
        ਵਾਪਸ;
    }

    CV_IPP_ਲੜੀਵਾਰ(allch1, ipp_ਮਿਲਾਓ(mv, dst, (ਪੂਰਨ)n));

    ਜੇ( !allch1 )
    {
        Auto_ਬਫਰ<ਪੂਰਨ> ਜੋੜੇ(cn*2);
        ਪੂਰਨ j, ni=0;

        ਲਈ( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            ਲਈ( k = 0; k < ni; k++ )
            {
                ਜੋੜੇ[(j+k)*2] = j + k;
                ਜੋੜੇ[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &ਜੋੜੇ[0], cn );
        ਵਾਪਸ;
    }

    ਮਿਲਾਓ_Func func = get_ਮਿਲਾਓ_Func(ਡੂੰਘਾਈ);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_ਮਾਪ(), esz1 = dst.elemSize1();
    size_t blocksize0 = (ਪੂਰਨ)((ਖੰਡ_ਮਾਪ + esz-1)/esz);
    Auto_ਬਫਰ<uchar> _buf((cn+1)*(ਆਕਾਰ(Mat*) + ਆਕਾਰ(uchar*)) + 16);
    ਅਚਲ Mat** arrays = (ਅਚਲ Mat**)_buf.ਅੰਕੜੇ();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    ਲਈ( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t ਕੁੱਲ = (ਪੂਰਨ)it.size;
    size_t blocksize = std::min((size_t)CV_ਵੰਡੋ_ਮਿਲਾਓ_MAX_ਖੰਡ_ਮਾਪ(cn), cn <= 4 ? ਕੁੱਲ : std::min(ਕੁੱਲ, blocksize0));

    ਲਈ( i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( size_t j = 0; j < ਕੁੱਲ; j += blocksize )
        {
            size_t bsz = std::min(ਕੁੱਲ - j, blocksize);
            func( (ਅਚਲ ਖਾਲੀ**)&ptrs[1], ptrs[0], (ਪੂਰਨ)bsz, cn );

            ਜੇ( j + blocksize < ਕੁੱਲ )
            {
                ptrs[0] += bsz*esz;
                ਲਈ( ਪੂਰਨ t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_ਮਿਲਾਓ( ਨਿਵੇਸ਼_Array_ਦਾ_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_ਸਦਿਸ਼(src);
    CV_Assert(!src.empty());

    ਪੂਰਨ type = src[0].type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ਮਾਪ size = src[0].size();

    ਲਈ (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        ਪੂਰਨ itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_ਡੂੰਘਾਈ(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        ਜੇ (src[i].dims > 2)
            ਵਾਪਸ ਝੂਠ;

        CV_Assert(size == src[i].size() && ਡੂੰਘਾਈ == idepth);

        ਲਈ (ਪੂਰਨ cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    ਪੂਰਨ dcn = (ਪੂਰਨ)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    ਲਈ (ਪੂਰਨ i = 0; i < dcn; ++i)
    {
        srcargs += format("DECLARE_SRC_PARAM(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        cndecl += format(" -D scn%d=%d", i, ksrc[i].channels());
    }

    ocl::Kernel k("merge", ocl::core::ਵੰਡੋ_ਮਿਲਾਓ_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    _dst.create(size, CV_MAKE_TYPE(ਡੂੰਘਾਈ, dcn));
    UMat dst = _dst.getUMat();

    ਪੂਰਨ argidx = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < dcn; ++i)
        argidx = k.ਸੈੱਟ(argidx, ocl::KernelArg::Read_Only_No_ਮਾਪ(ksrc[i]));
    argidx = k.ਸੈੱਟ(argidx, ocl::KernelArg::WriteOnly(dst));
    k.ਸੈੱਟ(argidx, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI);

    size_t globalsize[2] = { (size_t)dst.ਖਾਨੇ_ਗਿਣਤੀ, ((size_t)dst.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ ਮਿਲਾਓ(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ਲੜੀਵਾਰ(_mv.is_UMat_ਸਦਿਸ਼() && _dst.isUMat(),
               ocl_ਮਿਲਾਓ(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_ਸਦਿਸ਼(mv);
    ਮਿਲਾਓ(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
