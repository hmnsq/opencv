// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

#ಅನ್ತರ್ಭಾವ "count_non_zero.simd.hpp"
#ಅನ್ತರ್ಭಾವ "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ಎಣಿಕೆ_Non_Zero_Func get_ಎಣಿಕೆ_Non_Zero_Tab(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ಎಣಿಕೆ_Non_Zero_Tab, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ಎಣಿಕೆ_Non_Zero( ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಂಕ & res )
{
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), kercn = ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src);
    ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ಆದರೆ (ಆಳ == CV_64F && !doubleSupport)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ಗುಂಪು_ಗಾತ್ರ();

    ಪೂರ್ಣಾಂಕ wgs2_aligned = 1;
    ಆಗ (wgs2_aligned < (ಪೂರ್ಣಾಂಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)),
                         ocl::type_ಗಮ್ಯ_Str(ಆಳ), (ಪೂರ್ಣಾಂಕ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ಆದರೆ (k.ಸತತ(1, &globalsize, &wgs, ನಿಜ))
        ಹಿಂತಿರುಗಿ res = saturate_cast<ಪೂರ್ಣಾಂಕ>(cv::ಮೊತ್ತ(db.getMat(ACCESS_READ))[0]), ನಿಜ;
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined HAVE_IPP
ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ಎಣಿಕೆ_Non_Zero( Mat &src, ಪೂರ್ಣಾಂಕ &res )
{
    CV_INSTRUMENT_REGION_IPP();

#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 < 201801
    // Poor performance of SSE42
    ಆದರೆ(cv::ipp::get_Ipp_ತುದಿ_Features() == ippCPUID_SSE42)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ

    Ipp32s  ಎಣಿಕೆ_3 = 0;
    ಪೂರ್ಣಾಂಕ     ಆಳ = src.ಆಳ();

    ಆದರೆ(src.dims <= 2)
    {
        Ipp_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2;
        Ippi_ಗಾತ್ರ  size = {src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*src.channels(), src.ಅಡ್ಡಸಾಲುಗಳು};

        ಆದರೆ(ಆಳ == CV_8U)
            ಸ್ಥಿತಿ_2 = CV_INSTRUMENT_FUN_IPP(ippi_ಎಣಿಕೆ_ಒಳ_Range_8u_C1_R, (ಸ್ಥಿರಾಂಕ Ipp8u *)src.ptr(), (ಪೂರ್ಣಾಂಕ)src.ಹೆಜ್ಜೆ, size, &ಎಣಿಕೆ_3, 0, 0);
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಆಳ == CV_32F)
            ಸ್ಥಿತಿ_2 = CV_INSTRUMENT_FUN_IPP(ippi_ಎಣಿಕೆ_ಒಳ_Range_32f_C1_R, (ಸ್ಥಿರಾಂಕ Ipp32f *)src.ptr(), (ಪೂರ್ಣಾಂಕ)src.ಹೆಜ್ಜೆ, size, &ಎಣಿಕೆ_3, 0, 0);
        ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        ಆದರೆ(ಸ್ಥಿತಿ_2 < 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        res = size.ಅಗಲ*size.ಎತ್ತರ - ಎಣಿಕೆ_3;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Ipp_ಸ್ಥಿತಿ       ಸ್ಥಿತಿ_2;
        ಸ್ಥಿರಾಂಕ Mat      *arrays[] = {&src, ಖಾಲಿ};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_ಗಾತ್ರ        size  = {(ಪೂರ್ಣಾಂಕ)it.size*src.channels(), 1};
        res = 0;
        ಗಾಗಿ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ಆದರೆ(ಆಳ == CV_8U)
                ಸ್ಥಿತಿ_2 = CV_INSTRUMENT_FUN_IPP(ippi_ಎಣಿಕೆ_ಒಳ_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (ಪೂರ್ಣಾಂಕ)it.planes->ಹೆಜ್ಜೆ, size, &ಎಣಿಕೆ_3, 0, 0);
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಆಳ == CV_32F)
                ಸ್ಥಿತಿ_2 = CV_INSTRUMENT_FUN_IPP(ippi_ಎಣಿಕೆ_ಒಳ_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (ಪೂರ್ಣಾಂಕ)it.planes->ಹೆಜ್ಜೆ, size, &ಎಣಿಕೆ_3, 0, 0);
            ಇಲ್ಲದಿದ್ದರೆ
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

            ಆದರೆ(ಸ್ಥಿತಿ_2 < 0 || (ಪೂರ್ಣಾಂಕ)it.planes->ಒಟ್ಟು()*src.channels() < ಎಣಿಕೆ_3)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

            res += (ಪೂರ್ಣಾಂಕ)it.planes->ಒಟ್ಟು()*src.channels() - ಎಣಿಕೆ_3;
        }
    }

    ಹಿಂತಿರುಗಿ ನಿಜ;
}
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_Non_Zero(ಒಳಹರಿವು_Array _src)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#ಆದರೆ defined HAVE_OPENCL || defined HAVE_IPP
    ಪೂರ್ಣಾಂಕ res = -1;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    CV_OCL_ಸತತ_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_ಎಣಿಕೆ_Non_Zero(_src, res),
                res)
#ಆದರೆ_ಅಂತ್ಯ

    Mat src = _src.getMat();
    CV_IPP_ಸತತ_FAST(ipp_ಎಣಿಕೆ_Non_Zero(src, res), res);

    ಎಣಿಕೆ_Non_Zero_Func func = get_ಎಣಿಕೆ_Non_Zero_Tab(src.ಆಳ());
    CV_Assert( func != 0 );

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size, nz = 0;

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], ಒಟ್ಟು );

    ಹಿಂತಿರುಗಿ nz;
}

ಶೂನ್ಯ ಕಂಡುಹಿಡಿ_Non_Zero(ಒಳಹರಿವು_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    ಪೂರ್ಣಾಂಕ ಆಳ = src.ಆಳ();
    std::vector<ಬಿಂದು> idxvec;
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು = src.ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> buf_(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ + 1);
    ಪೂರ್ಣಾಂಕ* buf = buf_.ದತ್ತಾಂಶ();

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < ಅಡ್ಡಸಾಲುಗಳು; i++ )
    {
        ಪೂರ್ಣಾಂಕ j, k = 0;
        ಸ್ಥಿರಾಂಕ uchar* ptr8 = src.ptr(i);
        ಆದರೆ( ಆಳ == CV_8U || ಆಳ == CV_8S || ಆಳ == CV_Bool || ಆಳ == CV_8F_E4M3FNUZ )
        {
            j = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ vlanes = VTraits<v_uint8>::vlanes();
            ಸ್ಥಿರಾಂಕ v_uint8 vzero = vx_setzero_u8();
            ಗಾಗಿ( ; j <= ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ - vlanes; j += vlanes )
            {
                ಆದರೆ( !v_check_any(v_ne(vx_ತುಂಬು(ptr8 + j), vzero)) )
                    ಮುಂದುವರಿ;
                ಗಾಗಿ( ಪೂರ್ಣಾಂಕ t = j; t < j + vlanes; t++ )
                    ಆದರೆ( ptr8[t] != 0 ) buf[k++] = t;
            }
#ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ( ; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                ಆದರೆ( ptr8[j] != 0 ) buf[k++] = j;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_16U || ಆಳ == CV_16S )
        {
            ಸ್ಥಿರಾಂಕ ushort* ptr16 = (ಸ್ಥಿರಾಂಕ ushort*)ptr8;
            ಗಾಗಿ( j = 0; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                ಆದರೆ( ptr16[j] != 0 ) buf[k++] = j;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_32S || ಆಳ == CV_32U )
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ptr32s = (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)ptr8;
            ಗಾಗಿ( j = 0; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                ಆದರೆ( ptr32s[j] != 0 ) buf[k++] = j;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_64S || ಆಳ == CV_64U )
        {
            ಸ್ಥಿರಾಂಕ int64* ptr64s = (ಸ್ಥಿರಾಂಕ int64*)ptr8;
            ಗಾಗಿ( j = 0; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                ಆದರೆ( ptr64s[j] != 0 ) buf[k++] = j;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_32F )
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ptr32s = (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)ptr8;
            ಗಾಗಿ( j = 0; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                ಆದರೆ( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_16F || ಆಳ == CV_16BF )
        {
            ಸ್ಥಿರಾಂಕ ushort* ptr16 = (ಸ್ಥಿರಾಂಕ ushort*)ptr8;
            ಗಾಗಿ( j = 0; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                ಆದರೆ( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ಆಳ == CV_8F_E4M3FN )
        {
            ಗಾಗಿ( j = 0; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                ಆದರೆ( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಸ್ಥಿರಾಂಕ int64* ptr64s = (ಸ್ಥಿರಾಂಕ int64*)ptr8;
            ಗಾಗಿ( j = 0; j < ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; j++ )
                ಆದರೆ( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        ಆದರೆ( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            ಗಾಗಿ( j = 0; j < k; j++ )
                idxvec[sz + j] = ಬಿಂದು(buf[j], i);
        }
    }

    ಆದರೆ( idxvec.empty() || (_idx.ಬಗೆ() == ಒಳಹರಿವು_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    ಆದರೆ( !idxvec.empty() )
        Mat(idxvec).copy_ಗಮ್ಯ(_idx);
}

} // namespace
