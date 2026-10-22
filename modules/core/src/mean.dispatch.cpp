// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat_parallel.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "mean.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


ਨਾਮਾਕਾਸ਼ cv {

// serial implementation, also used for the pieces of the parallel version
ਸਥਿਰ Scalar ਔਸਤ_3(ਅਚਲ Mat& src, ਅਚਲ Mat& ਨਕਾਬ)
{
    CV_Assert( ਨਕਾਬ.empty() || ((ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool) && src.size == ਨਕਾਬ.size) );
    ਜੇ (src.empty())
        ਵਾਪਸ Scalar::all(0.0);

    ਪੂਰਨ k, cn = src.channels(), ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    ਜੇ (src.isContinuous() && ਨਕਾਬ.isContinuous())
    {
        CALL_HAL_RET2(ਔਸਤ_Std_Dev, cv_hal_ਔਸਤ_Std_Dev, s, src.ਅੰਕੜੇ, 0, (ਪੂਰਨ)src.ਕੁੱਲ(), 1, src.type(),
                      &s[0], ਖਾਲੀ_ਸੂਚਕ /*stddev*/, ਨਕਾਬ.ਅੰਕੜੇ, 0);
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (src.dims <= 2)
        {
            CALL_HAL_RET2(ਔਸਤ_Std_Dev, cv_hal_ਔਸਤ_Std_Dev, s, src.ਅੰਕੜੇ, src.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, src.type(),
                          &s[0], ਖਾਲੀ_ਸੂਚਕ, ਨਕਾਬ.ਅੰਕੜੇ, ਨਕਾਬ.ਕਦਮ);
        }
    }

    ਜੋੜ_Func func = get_ਜੋੜ_Func(ਡੂੰਘਾਈ);

    CV_Assert( func != 0 );

    ਅਚਲ Mat* arrays[] = {&src, &ਨਕਾਬ, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size, ਖੰਡ_ਮਾਪ_2 = ਕੁੱਲ, partial_ਖੰਡ_ਮਾਪ = 0;
    ਪੂਰਨ j, ਗਿਣਤੀ_3 = 0;
    ਪੂਰਨ _buf[CV_CN_MAX];
    ਪੂਰਨ* buf = (ਪੂਰਨ*)&s[0];
    ਤਾਰਕਿਕ partial_ਜੋੜ_Is_Int = ਡੂੰਘਾਈ < CV_32S;
    ਤਾਰਕਿਕ ਖੰਡ_ਜੋੜ = partial_ਜੋੜ_Is_Int || ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF;
    size_t esz = 0, nz0 = 0;

    ਜੇ( ਖੰਡ_ਜੋੜ )
    {
        partial_ਖੰਡ_ਮਾਪ = ਡੂੰਘਾਈ <= CV_8S ? (1 << 23) : (1 << 15);
        ਖੰਡ_ਮਾਪ_2 = std::min(ਖੰਡ_ਮਾਪ_2, partial_ਖੰਡ_ਮਾਪ);
        buf = _buf;
        ਲਈ( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_ਮਾਪ();
    }

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( j = 0; j < ਕੁੱਲ; j += ਖੰਡ_ਮਾਪ_2 )
        {
            ਪੂਰਨ bsz = std::min(ਕੁੱਲ - j, ਖੰਡ_ਮਾਪ_2);
            ਪੂਰਨ nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            ਗਿਣਤੀ_3 += nz;
            nz0 += nz;
            ਜੇ( ਖੰਡ_ਜੋੜ && (ਗਿਣਤੀ_3 + ਖੰਡ_ਮਾਪ_2 >= partial_ਖੰਡ_ਮਾਪ || (i+1 >= it.nplanes && j+bsz >= ਕੁੱਲ)) )
            {
                ਜੇ (partial_ਜੋੜ_Is_Int) {
                    ਲਈ( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } ਨਹੀਂਤਾਂ {
                    ਲਈ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ਦਸ਼ਮਲਵ*)buf)[k];
                        buf[k] = 0;
                    }
                }
                ਗਿਣਤੀ_3 = 0;
            }
            ptrs[0] += bsz*esz;
            ਜੇ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    ਵਾਪਸ s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ਸਥਿਰ ਦੁੱਗਣਾ stat_ਗਿਣਤੀ(ਅਚਲ StatChunk& c)
{
    ਵਾਪਸ c.ਨਕਾਬ.empty() ? (ਦੁੱਗਣਾ)c.src.ਕੁੱਲ() : (ਦੁੱਗਣਾ)ਗਿਣਤੀ_Non_Zero(c.ਨਕਾਬ);
}

Scalar ਔਸਤ_2(ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();
    CV_Assert( ਨਕਾਬ.empty() || ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    ਜੇ (src.channels() > 4 || !ਵੰਡੋ_For_Parallel_Stat(src, ਨਕਾਬ, chunks))
        ਵਾਪਸ ਔਸਤ_3(src, ਨਕਾਬ);

    ਅਚਲ ਪੂਰਨ n = (ਪੂਰਨ)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<ਦੁੱਗਣਾ> cnt(n);
    parallel_for_(Range(0, n), [&](ਅਚਲ Range& r)
    {
        ਲਈ (ਪੂਰਨ i = r.ਸ਼ੁਰੂ; i < r.end; i++)
        {
            part[i] = ਔਸਤ_3(chunks[i].src, chunks[i].ਨਕਾਬ);
            cnt[i] = stat_ਗਿਣਤੀ(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    ਦੁੱਗਣਾ ਕੁੱਲ = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        ਕੁੱਲ += cnt[i];
    }
    ਵਾਪਸ s*(ਕੁੱਲ > 0 ? 1./ਕੁੱਲ : 0);
}

ਸਥਿਰ ਜੋੜ_Sqr_Func get_ਜੋੜ_Sqr_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ਜੋੜ_Sqr_Func, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ ocl_ਔਸਤ_Std_Dev( ਨਿਵੇਸ਼_Array _src, OutputArray ਔਸਤ, OutputArray _sdv, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    ਤਾਰਕਿਕ have_ਨਕਾਬ = ਨਕਾਬ_2.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::NONE;
    ਪੂਰਨ nz = have_ਨਕਾਬ ? -1 : (ਪੂਰਨ)_src.ਕੁੱਲ();
    Scalar ਔਸਤ_2(0), stddev(0);
    ਅਚਲ ਪੂਰਨ cn = _src.channels();
    ਜੇ (cn > 4)
        ਵਾਪਸ ਝੂਠ;

    {
        ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
        ਤਾਰਕਿਕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_ਨਕਾਬ_Continuous = ਨਕਾਬ_2.isContinuous();
        ਅਚਲ ocl::Device &defDev = ocl::Device::getDefault();
        ਪੂਰਨ groups = defDev.maxComputeUnits();
        ਜੇ (defDev.isIntel())
        {
            ਸਥਿਰ ਅਚਲ ਪੂਰਨ sub_ਟੁਕੜਾ_EUCount = 10;
            groups = (groups / sub_ਟੁਕੜਾ_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_ਸਮੂਹ_ਮਾਪ();

        ਪੂਰਨ ddepth = std::max(CV_32S, ਡੂੰਘਾਈ), sqddepth = std::max(CV_32F, ਡੂੰਘਾਈ),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_ਨਕਾਬ || ਨਕਾਬ_2.type() == CV_8U || ਨਕਾਬ_2.type() == CV_8S || ਨਕਾਬ_2.type() == CV_Bool);

        ਪੂਰਨ wgs2_aligned = 1;
        ਜਦੋਂ (wgs2_aligned < (ਪੂਰਨ)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        ਜੇ ( (!doubleSupport && ਡੂੰਘਾਈ == CV_64F) )
            ਵਾਪਸ ਝੂਠ;

        ਅੱਖਰ cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_ਮੰਜ਼ਿਲ_Str(type), ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ),
                             ocl::type_ਮੰਜ਼ਿਲ_Str(dtype), ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth), sqddepth,
                             ocl::type_ਮੰਜ਼ਿਲ_Str(sqdtype), ocl::type_ਮੰਜ਼ਿਲ_Str(sqddepth),
                             ocl::convertTypeStr(ਡੂੰਘਾਈ, sqddepth, cn, cvt[0], ਆਕਾਰ(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_ਨਕਾਬ_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(ਡੂੰਘਾਈ, ddepth, cn, cvt[1], ਆਕਾਰ(cvt[1])),
                             (ਪੂਰਨ)wgs, wgs2_aligned, have_ਨਕਾਬ ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        ਜੇ (k.empty())
            ਵਾਪਸ ਝੂਠ;

        ਪੂਰਨ dbsize = groups * ((have_ਨਕਾਬ ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_ਮਾਪ(sqdtype) + CV_ELEM_ਮਾਪ(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), ਨਕਾਬ = ਨਕਾਬ_2.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ);

        ਜੇ (have_ਨਕਾਬ)
            k.ਦਲੀਲਾਂ(srcarg, src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(), groups, dbarg, maskarg);
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(srcarg, src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(), groups, dbarg);

        size_t globalsize = groups * wgs;

        ਜੇ(!k.ਲੜੀਵਾਰ(1, &globalsize, &wgs, ਝੂਠ))
            ਵਾਪਸ ਝੂਠ;

        ਕਿਸਮਪਰਿਭਾਸ਼ਾ Scalar (* part_ਜੋੜ)(Mat m);
        part_ਜੋੜ funcs[3] = { ocl_part_ਜੋੜ<ਪੂਰਨ>, ocl_part_ਜੋੜ<ਦਸ਼ਮਲਵ>, ocl_part_ਜੋੜ<ਦੁੱਗਣਾ> };
        Mat dbm = db.getMat(ACCESS_READ);

        ਔਸਤ_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_ਮਾਪ(dtype)));

        ਜੇ (have_ਨਕਾਬ)
            nz = saturate_cast<ਪੂਰਨ>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_ਮਾਪ(dtype) +
                                                           CV_ELEM_ਮਾਪ(sqdtype))))[0]);
    }

    ਦੁੱਗਣਾ ਕੁੱਲ = nz != 0 ? 1.0 / nz : 0;
    ਪੂਰਨ k, j;
    ਲਈ (ਪੂਰਨ i = 0; i < cn; ++i)
    {
        ਔਸਤ_2[i] *= ਕੁੱਲ;
        stddev[i] = std::ਵਰਗਮੂਲ(std::max(stddev[i] * ਕੁੱਲ - ਔਸਤ_2[i] * ਔਸਤ_2[i] , 0.));
    }

    ਲਈ( j = 0; j < 2; j++ )
    {
        ਅਚਲ ਦੁੱਗਣਾ * ਅਚਲ sptr = j == 0 ? &ਔਸਤ_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? ਔਸਤ : _sdv;
        ਜੇ( !_dst.needed() )
            ਜਾਰੀ;

        ਜੇ( !_dst.fixed_ਮਾਪ() )
            _dst.create(cn, 1, CV_64F, -1, ਸੱਚ);
        Mat dst = _dst.getMat();
        ਪੂਰਨ dcn = (ਪੂਰਨ)dst.ਕੁੱਲ();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.ਖਾਨੇ_ਗਿਣਤੀ == 1 || dst.ਪੰਕਤੀਆਂ == 1) && dcn >= cn );
        ਦੁੱਗਣਾ* dptr = dst.ptr<ਦੁੱਗਣਾ>();
        ਲਈ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        ਲਈ( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

// serial implementation, also used for the pieces of the parallel version
ਸਥਿਰ ਖਾਲੀ ਔਸਤ_Std_Dev_2(ਨਿਵੇਸ਼_Array _src, OutputArray ਔਸਤ, OutputArray _sdv, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( ਨਕਾਬ_2.empty() || ਨਕਾਬ_2.type() == CV_8U || ਨਕਾਬ_2.type() == CV_8S || ਨਕਾਬ_2.type() == CV_Bool );

    CV_OCL_ਲੜੀਵਾਰ(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_ਔਸਤ_Std_Dev(_src, ਔਸਤ, _sdv, ਨਕਾਬ_2))

    Mat src = _src.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();

    CV_Assert(ਨਕਾਬ.empty() || ((ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool) && src.size == ਨਕਾਬ.size));

    ਪੂਰਨ k, cn = src.channels(), ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();
    Mat ਔਸਤ_mat, stddev_mat;

    ਜੇ(ਔਸਤ.needed())
    {
        ਜੇ( !ਔਸਤ.fixed_ਮਾਪ() )
            ਔਸਤ.create(cn, 1, CV_64F, -1, ਸੱਚ);

        ਔਸਤ_mat = ਔਸਤ.getMat();
        ਪੂਰਨ dcn = (ਪੂਰਨ)ਔਸਤ_mat.ਕੁੱਲ();
        CV_Assert( ਔਸਤ_mat.type() == CV_64F && ਔਸਤ_mat.isContinuous() &&
                   (ਔਸਤ_mat.ਖਾਨੇ_ਗਿਣਤੀ == 1 || ਔਸਤ_mat.ਪੰਕਤੀਆਂ == 1) && dcn >= cn );

        ਦੁੱਗਣਾ* dptr = ਔਸਤ_mat.ptr<ਦੁੱਗਣਾ>();
        ਲਈ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ਜੇ (_sdv.needed())
    {
        ਜੇ( !_sdv.fixed_ਮਾਪ() )
            _sdv.create(cn, 1, CV_64F, -1, ਸੱਚ);

        stddev_mat = _sdv.getMat();
        ਪੂਰਨ dcn = (ਪੂਰਨ)stddev_mat.ਕੁੱਲ();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.ਖਾਨੇ_ਗਿਣਤੀ == 1 || stddev_mat.ਪੰਕਤੀਆਂ == 1) && dcn >= cn );

        ਦੁੱਗਣਾ* dptr = stddev_mat.ptr<ਦੁੱਗਣਾ>();
        ਲਈ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    ਜੇ (src.isContinuous() && ਨਕਾਬ.isContinuous())
    {
        CALL_HAL(ਔਸਤ_Std_Dev, cv_hal_ਔਸਤ_Std_Dev, src.ਅੰਕੜੇ, 0, (ਪੂਰਨ)src.ਕੁੱਲ(), 1, src.type(),
                 ਔਸਤ.needed() ? ਔਸਤ_mat.ptr<ਦੁੱਗਣਾ>() : ਖਾਲੀ_ਸੂਚਕ,
                 _sdv.needed() ? stddev_mat.ptr<ਦੁੱਗਣਾ>() : ਖਾਲੀ_ਸੂਚਕ,
                 ਨਕਾਬ.ਅੰਕੜੇ, 0);
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (src.dims <= 2)
        {
            CALL_HAL(ਔਸਤ_Std_Dev, cv_hal_ਔਸਤ_Std_Dev, src.ਅੰਕੜੇ, src.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, src.type(),
                     ਔਸਤ.needed() ? ਔਸਤ_mat.ptr<ਦੁੱਗਣਾ>() : ਖਾਲੀ_ਸੂਚਕ,
                     _sdv.needed() ? stddev_mat.ptr<ਦੁੱਗਣਾ>() : ਖਾਲੀ_ਸੂਚਕ,
                     ਨਕਾਬ.ਅੰਕੜੇ, ਨਕਾਬ.ਕਦਮ);
        }
    }

    ਜੋੜ_Sqr_Func func = get_ਜੋੜ_Sqr_Func(ਡੂੰਘਾਈ);

    CV_Assert( func != 0 );

    ਅਚਲ Mat* arrays[] = {&src, &ਨਕਾਬ, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size, ਖੰਡ_ਮਾਪ_2 = ਕੁੱਲ, partial_ਖੰਡ_ਮਾਪ = 0;
    ਪੂਰਨ j;
    int64_t ਗਿਣਤੀ_3 = 0, nz0 = 0;
    ਦੁੱਗਣਾ _buf[CV_CN_MAX*4];
    ਦੁੱਗਣਾ *s = _buf, *sq = s + cn;
    ਪੂਰਨ *sbuf = (ਪੂਰਨ*)s, *sqbuf = (ਪੂਰਨ*)sq;
    ਤਾਰਕਿਕ partial_ਜੋੜ_Is_Int = ਡੂੰਘਾਈ < CV_32S;
    ਤਾਰਕਿਕ ਖੰਡ_ਜੋੜ = partial_ਜੋੜ_Is_Int || ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF;
    ਤਾਰਕਿਕ ਖੰਡ_Sq_ਜੋੜ = ਡੂੰਘਾਈ <= CV_8S;
    size_t esz = 0;

    ਲਈ( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    ਜੇ( ਖੰਡ_ਜੋੜ )
    {
        partial_ਖੰਡ_ਮਾਪ = 1 << 15;
        ਖੰਡ_ਮਾਪ_2 = std::min(ਖੰਡ_ਮਾਪ_2, partial_ਖੰਡ_ਮਾਪ);
        sbuf = (ਪੂਰਨ*)(sq + cn);
        ਜੇ( ਖੰਡ_Sq_ਜੋੜ )
            sqbuf = sbuf + cn;
        ਲਈ( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_ਮਾਪ();
    }

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( j = 0; j < ਕੁੱਲ; j += ਖੰਡ_ਮਾਪ_2 )
        {
            ਪੂਰਨ bsz = std::min(ਕੁੱਲ - j, ਖੰਡ_ਮਾਪ_2);
            ਪੂਰਨ nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            ਗਿਣਤੀ_3 += nz;
            nz0 += nz;
            ਜੇ( ਖੰਡ_ਜੋੜ && (ਗਿਣਤੀ_3 + ਖੰਡ_ਮਾਪ_2 >= partial_ਖੰਡ_ਮਾਪ || (i+1 >= it.nplanes && j+bsz >= ਕੁੱਲ)) )
            {
                ਜੇ (partial_ਜੋੜ_Is_Int) {
                    ਲਈ( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } ਨਹੀਂਤਾਂ {
                    ਲਈ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ਦਸ਼ਮਲਵ*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                ਜੇ( ਖੰਡ_Sq_ਜੋੜ )
                {
                    ਲਈ( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                ਗਿਣਤੀ_3 = 0;
            }
            ptrs[0] += bsz*esz;
            ਜੇ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    ਦੁੱਗਣਾ scale = nz0 ? 1./nz0 : 0.;
    ਲਈ( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::ਵਰਗਮੂਲ(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    ਜੇ (ਔਸਤ.needed())
    {
        ਅਚਲ ਦੁੱਗਣਾ* sptr = s;
        ਦੁੱਗਣਾ* dptr = ਔਸਤ_mat.ptr<ਦੁੱਗਣਾ>();
        ਲਈ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    ਜੇ (_sdv.needed())
    {
        ਅਚਲ ਦੁੱਗਣਾ* sptr = sq;
        ਦੁੱਗਣਾ* dptr = stddev_mat.ptr<ਦੁੱਗਣਾ>();
        ਲਈ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

ਸਥਿਰ ਖਾਲੀ write_Stat_ਨਤੀਜਾ(OutputArray _dst, ਪੂਰਨ cn, ਅਚਲ Scalar& v)
{
    ਜੇ (!_dst.needed())
        ਵਾਪਸ;
    ਜੇ (!_dst.fixed_ਮਾਪ())
        _dst.create(cn, 1, CV_64F, -1, ਸੱਚ);
    Mat d = _dst.getMat();
    ਪੂਰਨ dcn = (ਪੂਰਨ)d.ਕੁੱਲ();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.ਖਾਨੇ_ਗਿਣਤੀ == 1 || d.ਪੰਕਤੀਆਂ == 1) && dcn >= cn );
    ਦੁੱਗਣਾ* p = d.ptr<ਦੁੱਗਣਾ>();
    ਲਈ (ਪੂਰਨ k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

ਖਾਲੀ ਔਸਤ_Std_Dev(ਨਿਵੇਸ਼_Array _src, OutputArray ਔਸਤ, OutputArray _sdv, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ਜੇ (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();
        ਅਚਲ ਪੂਰਨ cn = src.channels();
        ਅਚਲ ਤਾਰਕਿਕ ਨਕਾਬ_Ok = ਨਕਾਬ.empty() || ((ਨਕਾਬ.type() == CV_8U || ਨਕਾਬ.type() == CV_8S || ਨਕਾਬ.type() == CV_Bool) &&
                                             src.size == ਨਕਾਬ.size);
        ਜੇ (cn <= 4 && ਨਕਾਬ_Ok && ਵੰਡੋ_For_Parallel_Stat(src, ਨਕਾਬ, chunks))
        {
            ਅਚਲ ਪੂਰਨ n = (ਪੂਰਨ)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<ਦੁੱਗਣਾ> cnt(n);
            parallel_for_(Range(0, n), [&](ਅਚਲ Range& r)
            {
                ਲਈ (ਪੂਰਨ i = r.ਸ਼ੁਰੂ; i < r.end; i++)
                {
                    Mat mi, si;
                    ਔਸਤ_Std_Dev_2(chunks[i].src, mi, si, chunks[i].ਨਕਾਬ);
                    ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<ਦੁੱਗਣਾ>(k);
                        sd[i][k] = si.at<ਦੁੱਗਣਾ>(k);
                    }
                    cnt[i] = stat_ਗਿਣਤੀ(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            ਦੁੱਗਣਾ ਕੁੱਲ = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                ਕੁੱਲ += cnt[i];
            }
            ਜੇ (ਕੁੱਲ > 0)
                M *= 1./ਕੁੱਲ;
            ਲਈ (ਪੂਰਨ i = 0; i < n; i++)
                ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                {
                    ਦੁੱਗਣਾ dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            ਲਈ (ਪੂਰਨ k = 0; k < cn; k++)
                SD[k] = ਕੁੱਲ > 0 ? std::ਵਰਗਮੂਲ(std::max(V[k]/ਕੁੱਲ, 0.)) : 0;
            write_Stat_ਨਤੀਜਾ(ਔਸਤ, cn, M);
            write_Stat_ਨਤੀਜਾ(_sdv, cn, SD);
            ਵਾਪਸ;
        }
    }
    ਔਸਤ_Std_Dev_2(_src, ਔਸਤ, _sdv, ਨਕਾਬ_2);
}

} // namespace
