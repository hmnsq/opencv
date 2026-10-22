// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "split.simd.hpp"
#ಅನ್ತರ್ಭಾವ "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

ಶೂನ್ಯ split8u(ಸ್ಥಿರಾಂಕ uchar* src, uchar** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ split16u(ಸ್ಥಿರಾಂಕ ushort* src, ushort** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ split32s(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* src, ಪೂರ್ಣಾಂಕ** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ split64s(ಸ್ಥಿರಾಂಕ int64* src, int64** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn )
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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ವಿಭಜಿಸು_Func)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn);

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ T*, T**, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ ವಿಭಜಿಸು_Wrap(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ** dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn)
{
    fn((ಸ್ಥಿರಾಂಕ T*)src, (T**)dst, len, cn);
}

ಸ್ಥಿರ ವಿಭಜಿಸು_Func get_ವಿಭಜಿಸು_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ ವಿಭಜಿಸು_Func ವಿಭಜಿಸು_Tab[CV_ಆಳ_MAX] =
    {
        ವಿಭಜಿಸು_Wrap<uchar, cv::hal::split8u>, ವಿಭಜಿಸು_Wrap<uchar, cv::hal::split8u>,
        ವಿಭಜಿಸು_Wrap<ushort, cv::hal::split16u>, ವಿಭಜಿಸು_Wrap<ushort, cv::hal::split16u>,
        ವಿಭಜಿಸು_Wrap<ಪೂರ್ಣಾಂಕ, cv::hal::split32s>, ವಿಭಜಿಸು_Wrap<ಪೂರ್ಣಾಂಕ, cv::hal::split32s>,
        ವಿಭಜಿಸು_Wrap<int64_t, cv::hal::split64s>, ವಿಭಜಿಸು_Wrap<ushort, cv::hal::split16u>,
        ವಿಭಜಿಸು_Wrap<ushort, cv::hal::split16u>, ವಿಭಜಿಸು_Wrap<uchar, cv::hal::split8u>,
        ವಿಭಜಿಸು_Wrap<int64_t, cv::hal::split64s>, ವಿಭಜಿಸು_Wrap<int64_t, cv::hal::split64s>,
        ವಿಭಜಿಸು_Wrap<ಪೂರ್ಣಾಂಕ, cv::hal::split32s>, ವಿಭಜಿಸು_Wrap<uchar, cv::hal::split8u>,
        ವಿಭಜಿಸು_Wrap<uchar, cv::hal::split8u>, 0,
    };

    ಹಿಂತಿರುಗಿ ವಿಭಜಿಸು_Tab[ಆಳ];
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ವಿಭಜಿಸು(ಸ್ಥಿರಾಂಕ Mat& src, Mat* mv, ಪೂರ್ಣಾಂಕ channels)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಆದರೆ(channels != 3 && channels != 4)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ(src.dims <= 2)
    {
        Ippi_ಗಾತ್ರ size       = ippi_ಗಾತ್ರ(src.size());
        ಶೂನ್ಯ    *dstPtrs[4] = {ಖಾಲಿ};
        size_t   dst_ಹೆಜ್ಜೆ_3    = mv[0].ಹೆಜ್ಜೆ;
        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            ಆದರೆ(dst_ಹೆಜ್ಜೆ_3 != mv[i].ಹೆಜ್ಜೆ)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }

        ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(llwi_Copy_ವಿಭಜಿಸು, src.ptr(), (ಪೂರ್ಣಾಂಕ)src.ಹೆಜ್ಜೆ, dstPtrs, (ಪೂರ್ಣಾಂಕ)dst_ಹೆಜ್ಜೆ_3, size, (ಪೂರ್ಣಾಂಕ)src.elemSize1(), channels, 0) >= 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ Mat *arrays[5] = {ಖಾಲಿ};
        uchar     *ptrs[5]   = {ಖಾಲಿ};
        arrays[0] = &src;

        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_ಗಾತ್ರ size = { (ಪೂರ್ಣಾಂಕ)it.size, 1 };

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ವಿಭಜಿಸು, ptrs[0], 0, (ಶೂನ್ಯ**)&ptrs[1], 0, size, (ಪೂರ್ಣಾಂಕ)src.elemSize1(), channels, 0) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(mv); CV_ಬಳಸದ(channels);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ವಿಭಜಿಸು(ಸ್ಥಿರಾಂಕ Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ k, ಆಳ = src.ಆಳ(), cn = src.channels();
    ಆದರೆ( cn == 1 )
    {
        src.copy_ಗಮ್ಯ(mv[0]);
        ಹಿಂತಿರುಗಿ;
    }

    ಗಾಗಿ( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, ಆಳ);
    }

    CV_IPP_ಸತತ_FAST(ipp_ವಿಭಜಿಸು(src, mv, cn));

    ವಿಭಜಿಸು_Func func = get_ವಿಭಜಿಸು_Func(ಆಳ);
    CV_Assert( func != 0 );

    size_t esz = src.elem_ಗಾತ್ರ(), esz1 = src.elemSize1();
    size_t blocksize0 = (ಖಂಡ_ಗಾತ್ರ + esz-1)/esz;
    Auto_ಬಫರ್<uchar> _buf((cn+1)*(ಗಾತ್ರ(Mat*) + ಗಾತ್ರ(uchar*)) + 16);
    ಸ್ಥಿರಾಂಕ Mat** arrays = (ಸ್ಥಿರಾಂಕ Mat**)_buf.ದತ್ತಾಂಶ();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    ಗಾಗಿ( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t ಒಟ್ಟು = it.size;
    size_t blocksize = std::min((size_t)CV_ವಿಭಜಿಸು_ವಿಲೀನ_MAX_ಖಂಡ_ಗಾತ್ರ(cn), cn <= 4 ? ಒಟ್ಟು : std::min(ಒಟ್ಟು, blocksize0));

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( size_t j = 0; j < ಒಟ್ಟು; j += blocksize )
        {
            size_t bsz = std::min(ಒಟ್ಟು - j, blocksize);
            func( ptrs[0], (ಶೂನ್ಯ**)&ptrs[1], (ಪೂರ್ಣಾಂಕ)bsz, cn );

            ಆದರೆ( j + blocksize < ಒಟ್ಟು )
            {
                ptrs[0] += bsz*esz;
                ಗಾಗಿ( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ವಿಭಜಿಸು( ಒಳಹರಿವು_Array _m, Output_Array_ದ_Arrays _mv )
{
    ಪೂರ್ಣಾಂಕ type = _m.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::ವಿಭಜಿಸು_ವಿಲೀನ_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_ಗಮ್ಯ_Str(ಆಳ), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಗಾತ್ರ_2 size = _m.size();
    _mv.create(cn, 1, ಆಳ);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < cn; ++i)
        _mv.create(size, ಆಳ, i);

    std::vector<UMat> dst;
    _mv.get_UMat_ಸದಿಶ(dst);

    ಪೂರ್ಣಾಂಕ argidx = k.ಗಣ(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < cn; ++i)
        argidx = k.ಗಣ(argidx, ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dst[i]));
    k.ಗಣ(argidx, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI);

    size_t globalsize[2] = { (size_t)size.ಅಗಲ, ((size_t)size.ಎತ್ತರ + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ವಿಭಜಿಸು(ಒಳಹರಿವು_Array _m, Output_Array_ದ_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_ಸತತ(_m.dims() <= 2 && _mv.is_UMat_ಸದಿಶ(),
               ocl_ವಿಭಜಿಸು(_m, _mv))

    Mat m = _m.getMat();
    ಆದರೆ( m.empty() )
    {
        _mv.release();
        ಹಿಂತಿರುಗಿ;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.ಆಳ() );

    ಪೂರ್ಣಾಂಕ ಆಳ = m.ಆಳ(), cn = m.channels();
    _mv.create(cn, 1, ಆಳ);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, ಆಳ, i);

    std::vector<Mat> dst;
    _mv.get_Mat_ಸದಿಶ(dst);

    ವಿಭಜಿಸು(m, &dst[0]);
}

} // namespace
