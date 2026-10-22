// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "count_non_zero.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਗਿਣਤੀ_Non_Zero_Func get_ਗਿਣਤੀ_Non_Zero_Tab(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ਗਿਣਤੀ_Non_Zero_Tab, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ ocl_ਗਿਣਤੀ_Non_Zero( ਨਿਵੇਸ਼_Array _src, ਪੂਰਨ & res )
{
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), kercn = ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src);
    ਤਾਰਕਿਕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ਜੇ (ਡੂੰਘਾਈ == CV_64F && !doubleSupport)
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ਸਮੂਹ_ਮਾਪ();

    ਪੂਰਨ wgs2_aligned = 1;
    ਜਦੋਂ (wgs2_aligned < (ਪੂਰਨ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), (ਪੂਰਨ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ਜੇ (k.ਲੜੀਵਾਰ(1, &globalsize, &wgs, ਸੱਚ))
        ਵਾਪਸ res = saturate_cast<ਪੂਰਨ>(cv::ਜੋੜ(db.getMat(ACCESS_READ))[0]), ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
}
#ਜੇ_ਅੰਤ

#ਜੇ defined HAVE_IPP
ਸਥਿਰ ਤਾਰਕਿਕ ipp_ਗਿਣਤੀ_Non_Zero( Mat &src, ਪੂਰਨ &res )
{
    CV_INSTRUMENT_REGION_IPP();

#ਜੇ IPP_ਸੰਸਕਰਣ_X100 < 201801
    // Poor performance of SSE42
    ਜੇ(cv::ipp::get_Ipp_ਸਿਖਰ_Features() == ippCPUID_SSE42)
        ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

    Ipp32s  ਗਿਣਤੀ_3 = 0;
    ਪੂਰਨ     ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();

    ਜੇ(src.dims <= 2)
    {
        Ipp_ਸਥਿਤੀ ਸਥਿਤੀ;
        Ippi_ਮਾਪ  size = {src.ਖਾਨੇ_ਗਿਣਤੀ*src.channels(), src.ਪੰਕਤੀਆਂ};

        ਜੇ(ਡੂੰਘਾਈ == CV_8U)
            ਸਥਿਤੀ = CV_INSTRUMENT_FUN_IPP(ippi_ਗਿਣਤੀ_ਵਿੱਚ_Range_8u_C1_R, (ਅਚਲ Ipp8u *)src.ptr(), (ਪੂਰਨ)src.ਕਦਮ, size, &ਗਿਣਤੀ_3, 0, 0);
        ਨਹੀਂਤਾਂ ਜੇ(ਡੂੰਘਾਈ == CV_32F)
            ਸਥਿਤੀ = CV_INSTRUMENT_FUN_IPP(ippi_ਗਿਣਤੀ_ਵਿੱਚ_Range_32f_C1_R, (ਅਚਲ Ipp32f *)src.ptr(), (ਪੂਰਨ)src.ਕਦਮ, size, &ਗਿਣਤੀ_3, 0, 0);
        ਨਹੀਂਤਾਂ
            ਵਾਪਸ ਝੂਠ;

        ਜੇ(ਸਥਿਤੀ < 0)
            ਵਾਪਸ ਝੂਠ;

        res = size.ਚੌੜਾਈ*size.ਉਚਾਈ - ਗਿਣਤੀ_3;
    }
    ਨਹੀਂਤਾਂ
    {
        Ipp_ਸਥਿਤੀ       ਸਥਿਤੀ;
        ਅਚਲ Mat      *arrays[] = {&src, ਨਿਰਾ};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_ਮਾਪ        size  = {(ਪੂਰਨ)it.size*src.channels(), 1};
        res = 0;
        ਲਈ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ਜੇ(ਡੂੰਘਾਈ == CV_8U)
                ਸਥਿਤੀ = CV_INSTRUMENT_FUN_IPP(ippi_ਗਿਣਤੀ_ਵਿੱਚ_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (ਪੂਰਨ)it.planes->ਕਦਮ, size, &ਗਿਣਤੀ_3, 0, 0);
            ਨਹੀਂਤਾਂ ਜੇ(ਡੂੰਘਾਈ == CV_32F)
                ਸਥਿਤੀ = CV_INSTRUMENT_FUN_IPP(ippi_ਗਿਣਤੀ_ਵਿੱਚ_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (ਪੂਰਨ)it.planes->ਕਦਮ, size, &ਗਿਣਤੀ_3, 0, 0);
            ਨਹੀਂਤਾਂ
                ਵਾਪਸ ਝੂਠ;

            ਜੇ(ਸਥਿਤੀ < 0 || (ਪੂਰਨ)it.planes->ਕੁੱਲ()*src.channels() < ਗਿਣਤੀ_3)
                ਵਾਪਸ ਝੂਠ;

            res += (ਪੂਰਨ)it.planes->ਕੁੱਲ()*src.channels() - ਗਿਣਤੀ_3;
        }
    }

    ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

ਪੂਰਨ ਗਿਣਤੀ_Non_Zero(ਨਿਵੇਸ਼_Array _src)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#ਜੇ defined HAVE_OPENCL || defined HAVE_IPP
    ਪੂਰਨ res = -1;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    CV_OCL_ਲੜੀਵਾਰ_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_ਗਿਣਤੀ_Non_Zero(_src, res),
                res)
#ਜੇ_ਅੰਤ

    Mat src = _src.getMat();
    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_ਗਿਣਤੀ_Non_Zero(src, res), res);

    ਗਿਣਤੀ_Non_Zero_Func func = get_ਗਿਣਤੀ_Non_Zero_Tab(src.ਡੂੰਘਾਈ());
    CV_Assert( func != 0 );

    ਅਚਲ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size, nz = 0;

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], ਕੁੱਲ );

    ਵਾਪਸ nz;
}

ਖਾਲੀ ਲੱਭੋ_Non_Zero(ਨਿਵੇਸ਼_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    ਪੂਰਨ ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();
    std::vector<ਬਿੰਦੂ> idxvec;
    ਪੂਰਨ ਪੰਕਤੀਆਂ = src.ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ = src.ਖਾਨੇ_ਗਿਣਤੀ;
    Auto_ਬਫਰ<ਪੂਰਨ> buf_(ਖਾਨੇ_ਗਿਣਤੀ + 1);
    ਪੂਰਨ* buf = buf_.ਅੰਕੜੇ();

    ਲਈ( ਪੂਰਨ i = 0; i < ਪੰਕਤੀਆਂ; i++ )
    {
        ਪੂਰਨ j, k = 0;
        ਅਚਲ uchar* ptr8 = src.ptr(i);
        ਜੇ( ਡੂੰਘਾਈ == CV_8U || ਡੂੰਘਾਈ == CV_8S || ਡੂੰਘਾਈ == CV_Bool || ਡੂੰਘਾਈ == CV_8F_E4M3FNUZ )
        {
            j = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ਅਚਲ ਪੂਰਨ vlanes = VTraits<v_uint8>::vlanes();
            ਅਚਲ v_uint8 vzero = vx_setzero_u8();
            ਲਈ( ; j <= ਖਾਨੇ_ਗਿਣਤੀ - vlanes; j += vlanes )
            {
                ਜੇ( !v_check_any(v_ne(vx_ਲੱਦੋ(ptr8 + j), vzero)) )
                    ਜਾਰੀ;
                ਲਈ( ਪੂਰਨ t = j; t < j + vlanes; t++ )
                    ਜੇ( ptr8[t] != 0 ) buf[k++] = t;
            }
#ਜੇ_ਅੰਤ
            ਲਈ( ; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                ਜੇ( ptr8[j] != 0 ) buf[k++] = j;
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_16U || ਡੂੰਘਾਈ == CV_16S )
        {
            ਅਚਲ ushort* ptr16 = (ਅਚਲ ushort*)ptr8;
            ਲਈ( j = 0; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                ਜੇ( ptr16[j] != 0 ) buf[k++] = j;
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_32S || ਡੂੰਘਾਈ == CV_32U )
        {
            ਅਚਲ ਪੂਰਨ* ptr32s = (ਅਚਲ ਪੂਰਨ*)ptr8;
            ਲਈ( j = 0; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                ਜੇ( ptr32s[j] != 0 ) buf[k++] = j;
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_64S || ਡੂੰਘਾਈ == CV_64U )
        {
            ਅਚਲ int64* ptr64s = (ਅਚਲ int64*)ptr8;
            ਲਈ( j = 0; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                ਜੇ( ptr64s[j] != 0 ) buf[k++] = j;
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_32F )
        {
            ਅਚਲ ਪੂਰਨ* ptr32s = (ਅਚਲ ਪੂਰਨ*)ptr8;
            ਲਈ( j = 0; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                ਜੇ( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF )
        {
            ਅਚਲ ushort* ptr16 = (ਅਚਲ ushort*)ptr8;
            ਲਈ( j = 0; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                ਜੇ( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        ਨਹੀਂਤਾਂ ਜੇ( ਡੂੰਘਾਈ == CV_8F_E4M3FN )
        {
            ਲਈ( j = 0; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                ਜੇ( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        ਨਹੀਂਤਾਂ
        {
            ਅਚਲ int64* ptr64s = (ਅਚਲ int64*)ptr8;
            ਲਈ( j = 0; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                ਜੇ( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        ਜੇ( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            ਲਈ( j = 0; j < k; j++ )
                idxvec[sz + j] = ਬਿੰਦੂ(buf[j], i);
        }
    }

    ਜੇ( idxvec.empty() || (_idx.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    ਜੇ( !idxvec.empty() )
        Mat(idxvec).copy_ਮੰਜ਼ਿਲ(_idx);
}

} // namespace
