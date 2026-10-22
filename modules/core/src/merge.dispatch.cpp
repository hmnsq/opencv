// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "merge.simd.hpp"
#ಅನ್ತರ್ಭಾವ "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಶೂನ್ಯ merge8u(ಸ್ಥಿರಾಂಕ uchar** src, uchar* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ merge16u(ಸ್ಥಿರಾಂಕ ushort** src, ushort* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ merge32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ** src, ಪೂರ್ಣಾಂಕ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ merge64s(ಸ್ಥಿರಾಂಕ int64** src, int64* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ವಿಲೀನ_Func)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ** src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ T**, T*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ ವಿಲೀನ_Wrap(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ** src, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    fn((ಸ್ಥಿರಾಂಕ T**)src, (T*)dst, len, cn);
}

ಸ್ಥಿರ ವಿಲೀನ_Func get_ವಿಲೀನ_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ವಿಲೀನ_Func ವಿಲೀನ_Tab[CV_ಆಳ_MAX] =
    {
        ವಿಲೀನ_Wrap<uchar, cv::hal::merge8u>, ವಿಲೀನ_Wrap<uchar, cv::hal::merge8u>,
        ವಿಲೀನ_Wrap<ushort, cv::hal::merge16u>, ವಿಲೀನ_Wrap<ushort, cv::hal::merge16u>,
        ವಿಲೀನ_Wrap<ಪೂರ್ಣಾಂಕ, cv::hal::merge32s>, ವಿಲೀನ_Wrap<ಪೂರ್ಣಾಂಕ, cv::hal::merge32s>,
        ವಿಲೀನ_Wrap<int64_t, cv::hal::merge64s>, ವಿಲೀನ_Wrap<ushort, cv::hal::merge16u>,
        ವಿಲೀನ_Wrap<ushort, cv::hal::merge16u>, ವಿಲೀನ_Wrap<uchar, cv::hal::merge8u>,
        ವಿಲೀನ_Wrap<int64_t, cv::hal::merge64s>, ವಿಲೀನ_Wrap<int64_t, cv::hal::merge64s>,
        ವಿಲೀನ_Wrap<ಪೂರ್ಣಾಂಕ, cv::hal::merge32s>, ವಿಲೀನ_Wrap<uchar, cv::hal::merge8u>,
        ವಿಲೀನ_Wrap<uchar, cv::hal::merge8u>, 0,
    };

    ಹಿಂತಿರುಗಿ ವಿಲೀನ_Tab[ಆಳ];
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ವಿಲೀನ(ಸ್ಥಿರಾಂಕ Mat* mv, Mat& dst, ಪೂರ್ಣಾಂಕ channels)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಆದರೆ(channels != 3 && channels != 4)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ(mv[0].dims <= 2)
    {
        Ippi_ಗಾತ್ರ    size       = ippi_ಗಾತ್ರ(mv[0].size());
        ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *srcPtrs[4] = {ಖಾಲಿ};
        size_t      src_ಹೆಜ್ಜೆ_3    = mv[0].ಹೆಜ್ಜೆ;
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            ಆದರೆ(src_ಹೆಜ್ಜೆ_3 != mv[i].ಹೆಜ್ಜೆ)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }

        ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(llwi_Copy_ವಿಲೀನ, srcPtrs, (ಪೂರ್ಣಾಂಕ)src_ಹೆಜ್ಜೆ_3, dst.ptr(), (ಪೂರ್ಣಾಂಕ)dst.ಹೆಜ್ಜೆ, size, (ಪೂರ್ಣಾಂಕ)mv[0].elemSize1(), channels, 0) >= 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ Mat *arrays[5] = {ಖಾಲಿ};
        uchar     *ptrs[5]   = {ಖಾಲಿ};
        arrays[0] = &dst;

        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_ಗಾತ್ರ size = { (ಪೂರ್ಣಾಂಕ)it.size, 1 };

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ವಿಲೀನ, (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ**)&ptrs[1], 0, ptrs[0], 0, size, (ಪೂರ್ಣಾಂಕ)mv[0].elemSize1(), channels, 0) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(dst); CV_ಬಳಸದ(mv); CV_ಬಳಸದ(channels);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ವಿಲೀನ(ಸ್ಥಿರಾಂಕ Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    ಪೂರ್ಣಾಂಕ ಆಳ = mv[0].ಆಳ();
    ತಾರ್ಕಿಕ allch1 = ನಿಜ;
    ಪೂರ್ಣಾಂಕ k, cn = 0;
    size_t i;

    ಗಾಗಿ( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].ಆಳ() == ಆಳ);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(ಆಳ, cn));
    Mat dst = _dst.getMat();

    ಆದರೆ( n == 1 )
    {
        mv[0].copy_ಗಮ್ಯ(dst);
        ಹಿಂತಿರುಗಿ;
    }

    CV_IPP_ಸತತ(allch1, ipp_ವಿಲೀನ(mv, dst, (ಪೂರ್ಣಾಂಕ)n));

    ಆದರೆ( !allch1 )
    {
        Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> ಜೋಡಿಗಳು(cn*2);
        ಪೂರ್ಣಾಂಕ j, ni=0;

        ಗಾಗಿ( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            ಗಾಗಿ( k = 0; k < ni; k++ )
            {
                ಜೋಡಿಗಳು[(j+k)*2] = j + k;
                ಜೋಡಿಗಳು[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &ಜೋಡಿಗಳು[0], cn );
        ಹಿಂತಿರುಗಿ;
    }

    ವಿಲೀನ_Func func = get_ವಿಲೀನ_Func(ಆಳ);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_ಗಾತ್ರ(), esz1 = dst.elemSize1();
    size_t blocksize0 = (ಪೂರ್ಣಾಂಕ)((ಖಂಡ_ಗಾತ್ರ + esz-1)/esz);
    Auto_ಬಫರ್<uchar> _buf((cn+1)*(ಗಾತ್ರ(Mat*) + ಗಾತ್ರ(uchar*)) + 16);
    ಸ್ಥಿರಾಂಕ Mat** arrays = (ಸ್ಥಿರಾಂಕ Mat**)_buf.ದತ್ತಾಂಶ();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    ಗಾಗಿ( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size;
    size_t blocksize = std::min((size_t)CV_ವಿಭಜಿಸು_ವಿಲೀನ_MAX_ಖಂಡ_ಗಾತ್ರ(cn), cn <= 4 ? ಒಟ್ಟು : std::min(ಒಟ್ಟು, blocksize0));

    ಗಾಗಿ( i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( size_t j = 0; j < ಒಟ್ಟು; j += blocksize )
        {
            size_t bsz = std::min(ಒಟ್ಟು - j, blocksize);
            func( (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ**)&ptrs[1], ptrs[0], (ಪೂರ್ಣಾಂಕ)bsz, cn );

            ಆದರೆ( j + blocksize < ಒಟ್ಟು )
            {
                ptrs[0] += bsz*esz;
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ವಿಲೀನ( ಒಳಹರಿವು_Array_ದ_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_ಸದಿಶ(src);
    CV_Assert(!src.empty());

    ಪೂರ್ಣಾಂಕ type = src[0].type(), ಆಳ = CV_MAT_ಆಳ(type),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ಗಾತ್ರ_2 size = src[0].size();

    ಗಾಗಿ (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        ಪೂರ್ಣಾಂಕ itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_ಆಳ(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        ಆದರೆ (src[i].dims > 2)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        CV_Assert(size == src[i].size() && ಆಳ == idepth);

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    ಪೂರ್ಣಾಂಕ dcn = (ಪೂರ್ಣಾಂಕ)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dcn; ++i)
    {
        srcargs += format("DECLARE_SRC_PARAM(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        cndecl += format(" -D scn%d=%d", i, ksrc[i].channels());
    }

    ocl::Kernel k("merge", ocl::core::ವಿಭಜಿಸು_ವಿಲೀನ_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_ಗಮ್ಯ_Str(ಆಳ), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    _dst.create(size, CV_MAKE_TYPE(ಆಳ, dcn));
    UMat dst = _dst.getUMat();

    ಪೂರ್ಣಾಂಕ argidx = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < dcn; ++i)
        argidx = k.ಗಣ(argidx, ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ksrc[i]));
    argidx = k.ಗಣ(argidx, ocl::KernelArg::WriteOnly(dst));
    k.ಗಣ(argidx, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI);

    size_t globalsize[2] = { (size_t)dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ((size_t)dst.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ವಿಲೀನ(ಒಳಹರಿವು_Array_ದ_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ಸತತ(_mv.is_UMat_ಸದಿಶ() && _dst.isUMat(),
               ocl_ವಿಲೀನ(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_ಸದಿಶ(mv);
    ವಿಲೀನ(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
