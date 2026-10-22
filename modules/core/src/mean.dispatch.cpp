// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"
#ಅನ್ತರ್ಭಾವ "stat_parallel.hpp"

#ಅನ್ತರ್ಭಾವ "mean.simd.hpp"
#ಅನ್ತರ್ಭಾವ "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


ನಾಮಾಕಾಶ cv {

// serial implementation, also used for the pieces of the parallel version
ಸ್ಥಿರ Scalar ಸರಾಸರಿ_3(ಸ್ಥಿರಾಂಕ Mat& src, ಸ್ಥಿರಾಂಕ Mat& ಮುಖವಾಡ)
{
    CV_Assert( ಮುಖವಾಡ.empty() || ((ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool) && src.size == ಮುಖವಾಡ.size) );
    ಆದರೆ (src.empty())
        ಹಿಂತಿರುಗಿ Scalar::all(0.0);

    ಪೂರ್ಣಾಂಕ k, cn = src.channels(), ಆಳ = src.ಆಳ();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    ಆದರೆ (src.isContinuous() && ಮುಖವಾಡ.isContinuous())
    {
        CALL_HAL_RET2(ಸರಾಸರಿ_Std_Dev, cv_hal_ಸರಾಸರಿ_Std_Dev, s, src.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), 1, src.type(),
                      &s[0], ಶೂನ್ಯಸೂಚಕ /*stddev*/, ಮುಖವಾಡ.ದತ್ತಾಂಶ, 0);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (src.dims <= 2)
        {
            CALL_HAL_RET2(ಸರಾಸರಿ_Std_Dev, cv_hal_ಸರಾಸರಿ_Std_Dev, s, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, src.type(),
                          &s[0], ಶೂನ್ಯಸೂಚಕ, ಮುಖವಾಡ.ದತ್ತಾಂಶ, ಮುಖವಾಡ.ಹೆಜ್ಜೆ);
        }
    }

    ಮೊತ್ತ_Func func = get_ಮೊತ್ತ_Func(ಆಳ);

    CV_Assert( func != 0 );

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &ಮುಖವಾಡ, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size, ಖಂಡ_ಗಾತ್ರ_2 = ಒಟ್ಟು, partial_ಖಂಡ_ಗಾತ್ರ = 0;
    ಪೂರ್ಣಾಂಕ j, ಎಣಿಕೆ_3 = 0;
    ಪೂರ್ಣಾಂಕ _buf[CV_CN_MAX];
    ಪೂರ್ಣಾಂಕ* buf = (ಪೂರ್ಣಾಂಕ*)&s[0];
    ತಾರ್ಕಿಕ partial_ಮೊತ್ತ_Is_Int = ಆಳ < CV_32S;
    ತಾರ್ಕಿಕ ಖಂಡ_ಮೊತ್ತ = partial_ಮೊತ್ತ_Is_Int || ಆಳ == CV_16F || ಆಳ == CV_16BF;
    size_t esz = 0, nz0 = 0;

    ಆದರೆ( ಖಂಡ_ಮೊತ್ತ )
    {
        partial_ಖಂಡ_ಗಾತ್ರ = ಆಳ <= CV_8S ? (1 << 23) : (1 << 15);
        ಖಂಡ_ಗಾತ್ರ_2 = std::min(ಖಂಡ_ಗಾತ್ರ_2, partial_ಖಂಡ_ಗಾತ್ರ);
        buf = _buf;
        ಗಾಗಿ( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_ಗಾತ್ರ();
    }

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( j = 0; j < ಒಟ್ಟು; j += ಖಂಡ_ಗಾತ್ರ_2 )
        {
            ಪೂರ್ಣಾಂಕ bsz = std::min(ಒಟ್ಟು - j, ಖಂಡ_ಗಾತ್ರ_2);
            ಪೂರ್ಣಾಂಕ nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            ಎಣಿಕೆ_3 += nz;
            nz0 += nz;
            ಆದರೆ( ಖಂಡ_ಮೊತ್ತ && (ಎಣಿಕೆ_3 + ಖಂಡ_ಗಾತ್ರ_2 >= partial_ಖಂಡ_ಗಾತ್ರ || (i+1 >= it.nplanes && j+bsz >= ಒಟ್ಟು)) )
            {
                ಆದರೆ (partial_ಮೊತ್ತ_Is_Int) {
                    ಗಾಗಿ( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಗಾಗಿ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ದಶಮಾಂಶ*)buf)[k];
                        buf[k] = 0;
                    }
                }
                ಎಣಿಕೆ_3 = 0;
            }
            ptrs[0] += bsz*esz;
            ಆದರೆ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    ಹಿಂತಿರುಗಿ s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ಸ್ಥಿರ ದ್ವಿಗುಣ stat_ಎಣಿಕೆ(ಸ್ಥಿರಾಂಕ StatChunk& c)
{
    ಹಿಂತಿರುಗಿ c.ಮುಖವಾಡ.empty() ? (ದ್ವಿಗುಣ)c.src.ಒಟ್ಟು() : (ದ್ವಿಗುಣ)ಎಣಿಕೆ_Non_Zero(c.ಮುಖವಾಡ);
}

Scalar ಸರಾಸರಿ_2(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();
    CV_Assert( ಮುಖವಾಡ.empty() || ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    ಆದರೆ (src.channels() > 4 || !ವಿಭಜಿಸು_For_Parallel_Stat(src, ಮುಖವಾಡ, chunks))
        ಹಿಂತಿರುಗಿ ಸರಾಸರಿ_3(src, ಮುಖವಾಡ);

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = (ಪೂರ್ಣಾಂಕ)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<ದ್ವಿಗುಣ> cnt(n);
    parallel_for_(Range(0, n), [&](ಸ್ಥಿರಾಂಕ Range& r)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = r.ಆರಂಭ; i < r.end; i++)
        {
            part[i] = ಸರಾಸರಿ_3(chunks[i].src, chunks[i].ಮುಖವಾಡ);
            cnt[i] = stat_ಎಣಿಕೆ(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    ದ್ವಿಗುಣ ಒಟ್ಟು = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        ಒಟ್ಟು += cnt[i];
    }
    ಹಿಂತಿರುಗಿ s*(ಒಟ್ಟು > 0 ? 1./ಒಟ್ಟು : 0);
}

ಸ್ಥಿರ ಮೊತ್ತ_Sqr_Func get_ಮೊತ್ತ_Sqr_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ಮೊತ್ತ_Sqr_Func, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ಸರಾಸರಿ_Std_Dev( ಒಳಹರಿವು_Array _src, OutputArray ಸರಾಸರಿ, OutputArray _sdv, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    ತಾರ್ಕಿಕ have_ಮುಖವಾಡ = ಮುಖವಾಡ_2.ಬಗೆ() != ಒಳಹರಿವು_Array_2::NONE;
    ಪೂರ್ಣಾಂಕ nz = have_ಮುಖವಾಡ ? -1 : (ಪೂರ್ಣಾಂಕ)_src.ಒಟ್ಟು();
    Scalar ಸರಾಸರಿ_2(0), stddev(0);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = _src.channels();
    ಆದರೆ (cn > 4)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    {
        ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type);
        ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_ಮುಖವಾಡ_Continuous = ಮುಖವಾಡ_2.isContinuous();
        ಸ್ಥಿರಾಂಕ ocl::Device &defDev = ocl::Device::getDefault();
        ಪೂರ್ಣಾಂಕ groups = defDev.maxComputeUnits();
        ಆದರೆ (defDev.isIntel())
        {
            ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ sub_ತುಂಡು_EUCount = 10;
            groups = (groups / sub_ತುಂಡು_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_ಗುಂಪು_ಗಾತ್ರ();

        ಪೂರ್ಣಾಂಕ ddepth = std::max(CV_32S, ಆಳ), sqddepth = std::max(CV_32F, ಆಳ),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_ಮುಖವಾಡ || ಮುಖವಾಡ_2.type() == CV_8U || ಮುಖವಾಡ_2.type() == CV_8S || ಮುಖವಾಡ_2.type() == CV_Bool);

        ಪೂರ್ಣಾಂಕ wgs2_aligned = 1;
        ಆಗ (wgs2_aligned < (ಪೂರ್ಣಾಂಕ)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        ಆದರೆ ( (!doubleSupport && ಆಳ == CV_64F) )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        ಅಕ್ಷರ cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_ಗಮ್ಯ_Str(type), ocl::type_ಗಮ್ಯ_Str(ಆಳ),
                             ocl::type_ಗಮ್ಯ_Str(dtype), ocl::type_ಗಮ್ಯ_Str(ddepth), sqddepth,
                             ocl::type_ಗಮ್ಯ_Str(sqdtype), ocl::type_ಗಮ್ಯ_Str(sqddepth),
                             ocl::convertTypeStr(ಆಳ, sqddepth, cn, cvt[0], ಗಾತ್ರ(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_ಮುಖವಾಡ_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(ಆಳ, ddepth, cn, cvt[1], ಗಾತ್ರ(cvt[1])),
                             (ಪೂರ್ಣಾಂಕ)wgs, wgs2_aligned, have_ಮುಖವಾಡ ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        ಆದರೆ (k.empty())
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        ಪೂರ್ಣಾಂಕ dbsize = groups * ((have_ಮುಖವಾಡ ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_ಗಾತ್ರ(sqdtype) + CV_ELEM_ಗಾತ್ರ(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ);

        ಆದರೆ (have_ಮುಖವಾಡ)
            k.ವಾದಗಳು(srcarg, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), groups, dbarg, maskarg);
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(srcarg, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), groups, dbarg);

        size_t globalsize = groups * wgs;

        ಆದರೆ(!k.ಸತತ(1, &globalsize, &wgs, ಸುಳ್ಳು))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Scalar (* part_ಮೊತ್ತ)(Mat m);
        part_ಮೊತ್ತ funcs[3] = { ocl_part_ಮೊತ್ತ<ಪೂರ್ಣಾಂಕ>, ocl_part_ಮೊತ್ತ<ದಶಮಾಂಶ>, ocl_part_ಮೊತ್ತ<ದ್ವಿಗುಣ> };
        Mat dbm = db.getMat(ACCESS_READ);

        ಸರಾಸರಿ_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_ಗಾತ್ರ(dtype)));

        ಆದರೆ (have_ಮುಖವಾಡ)
            nz = saturate_cast<ಪೂರ್ಣಾಂಕ>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_ಗಾತ್ರ(dtype) +
                                                           CV_ELEM_ಗಾತ್ರ(sqdtype))))[0]);
    }

    ದ್ವಿಗುಣ ಒಟ್ಟು = nz != 0 ? 1.0 / nz : 0;
    ಪೂರ್ಣಾಂಕ k, j;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < cn; ++i)
    {
        ಸರಾಸರಿ_2[i] *= ಒಟ್ಟು;
        stddev[i] = std::ವರ್ಗಮೂಲ(std::max(stddev[i] * ಒಟ್ಟು - ಸರಾಸರಿ_2[i] * ಸರಾಸರಿ_2[i] , 0.));
    }

    ಗಾಗಿ( j = 0; j < 2; j++ )
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ * ಸ್ಥಿರಾಂಕ sptr = j == 0 ? &ಸರಾಸರಿ_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? ಸರಾಸರಿ : _sdv;
        ಆದರೆ( !_dst.needed() )
            ಮುಂದುವರಿ;

        ಆದರೆ( !_dst.fixed_ಗಾತ್ರ() )
            _dst.create(cn, 1, CV_64F, -1, ನಿಜ);
        Mat dst = _dst.getMat();
        ಪೂರ್ಣಾಂಕ dcn = (ಪೂರ್ಣಾಂಕ)dst.ಒಟ್ಟು();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || dst.ಅಡ್ಡಸಾಲುಗಳು == 1) && dcn >= cn );
        ದ್ವಿಗುಣ* dptr = dst.ptr<ದ್ವಿಗುಣ>();
        ಗಾಗಿ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        ಗಾಗಿ( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ಹಿಂತಿರುಗಿ ನಿಜ;
}
#ಆದರೆ_ಅಂತ್ಯ

// serial implementation, also used for the pieces of the parallel version
ಸ್ಥಿರ ಶೂನ್ಯ ಸರಾಸರಿ_Std_Dev_2(ಒಳಹರಿವು_Array _src, OutputArray ಸರಾಸರಿ, OutputArray _sdv, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( ಮುಖವಾಡ_2.empty() || ಮುಖವಾಡ_2.type() == CV_8U || ಮುಖವಾಡ_2.type() == CV_8S || ಮುಖವಾಡ_2.type() == CV_Bool );

    CV_OCL_ಸತತ(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_ಸರಾಸರಿ_Std_Dev(_src, ಸರಾಸರಿ, _sdv, ಮುಖವಾಡ_2))

    Mat src = _src.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();

    CV_Assert(ಮುಖವಾಡ.empty() || ((ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool) && src.size == ಮುಖವಾಡ.size));

    ಪೂರ್ಣಾಂಕ k, cn = src.channels(), ಆಳ = src.ಆಳ();
    Mat ಸರಾಸರಿ_mat, stddev_mat;

    ಆದರೆ(ಸರಾಸರಿ.needed())
    {
        ಆದರೆ( !ಸರಾಸರಿ.fixed_ಗಾತ್ರ() )
            ಸರಾಸರಿ.create(cn, 1, CV_64F, -1, ನಿಜ);

        ಸರಾಸರಿ_mat = ಸರಾಸರಿ.getMat();
        ಪೂರ್ಣಾಂಕ dcn = (ಪೂರ್ಣಾಂಕ)ಸರಾಸರಿ_mat.ಒಟ್ಟು();
        CV_Assert( ಸರಾಸರಿ_mat.type() == CV_64F && ಸರಾಸರಿ_mat.isContinuous() &&
                   (ಸರಾಸರಿ_mat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || ಸರಾಸರಿ_mat.ಅಡ್ಡಸಾಲುಗಳು == 1) && dcn >= cn );

        ದ್ವಿಗುಣ* dptr = ಸರಾಸರಿ_mat.ptr<ದ್ವಿಗುಣ>();
        ಗಾಗಿ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ಆದರೆ (_sdv.needed())
    {
        ಆದರೆ( !_sdv.fixed_ಗಾತ್ರ() )
            _sdv.create(cn, 1, CV_64F, -1, ನಿಜ);

        stddev_mat = _sdv.getMat();
        ಪೂರ್ಣಾಂಕ dcn = (ಪೂರ್ಣಾಂಕ)stddev_mat.ಒಟ್ಟು();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || stddev_mat.ಅಡ್ಡಸಾಲುಗಳು == 1) && dcn >= cn );

        ದ್ವಿಗುಣ* dptr = stddev_mat.ptr<ದ್ವಿಗುಣ>();
        ಗಾಗಿ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    ಆದರೆ (src.isContinuous() && ಮುಖವಾಡ.isContinuous())
    {
        CALL_HAL(ಸರಾಸರಿ_Std_Dev, cv_hal_ಸರಾಸರಿ_Std_Dev, src.ದತ್ತಾಂಶ, 0, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), 1, src.type(),
                 ಸರಾಸರಿ.needed() ? ಸರಾಸರಿ_mat.ptr<ದ್ವಿಗುಣ>() : ಶೂನ್ಯಸೂಚಕ,
                 _sdv.needed() ? stddev_mat.ptr<ದ್ವಿಗುಣ>() : ಶೂನ್ಯಸೂಚಕ,
                 ಮುಖವಾಡ.ದತ್ತಾಂಶ, 0);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (src.dims <= 2)
        {
            CALL_HAL(ಸರಾಸರಿ_Std_Dev, cv_hal_ಸರಾಸರಿ_Std_Dev, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, src.type(),
                     ಸರಾಸರಿ.needed() ? ಸರಾಸರಿ_mat.ptr<ದ್ವಿಗುಣ>() : ಶೂನ್ಯಸೂಚಕ,
                     _sdv.needed() ? stddev_mat.ptr<ದ್ವಿಗುಣ>() : ಶೂನ್ಯಸೂಚಕ,
                     ಮುಖವಾಡ.ದತ್ತಾಂಶ, ಮುಖವಾಡ.ಹೆಜ್ಜೆ);
        }
    }

    ಮೊತ್ತ_Sqr_Func func = get_ಮೊತ್ತ_Sqr_Func(ಆಳ);

    CV_Assert( func != 0 );

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &ಮುಖವಾಡ, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size, ಖಂಡ_ಗಾತ್ರ_2 = ಒಟ್ಟು, partial_ಖಂಡ_ಗಾತ್ರ = 0;
    ಪೂರ್ಣಾಂಕ j;
    int64_t ಎಣಿಕೆ_3 = 0, nz0 = 0;
    ದ್ವಿಗುಣ _buf[CV_CN_MAX*4];
    ದ್ವಿಗುಣ *s = _buf, *sq = s + cn;
    ಪೂರ್ಣಾಂಕ *sbuf = (ಪೂರ್ಣಾಂಕ*)s, *sqbuf = (ಪೂರ್ಣಾಂಕ*)sq;
    ತಾರ್ಕಿಕ partial_ಮೊತ್ತ_Is_Int = ಆಳ < CV_32S;
    ತಾರ್ಕಿಕ ಖಂಡ_ಮೊತ್ತ = partial_ಮೊತ್ತ_Is_Int || ಆಳ == CV_16F || ಆಳ == CV_16BF;
    ತಾರ್ಕಿಕ ಖಂಡ_Sq_ಮೊತ್ತ = ಆಳ <= CV_8S;
    size_t esz = 0;

    ಗಾಗಿ( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    ಆದರೆ( ಖಂಡ_ಮೊತ್ತ )
    {
        partial_ಖಂಡ_ಗಾತ್ರ = 1 << 15;
        ಖಂಡ_ಗಾತ್ರ_2 = std::min(ಖಂಡ_ಗಾತ್ರ_2, partial_ಖಂಡ_ಗಾತ್ರ);
        sbuf = (ಪೂರ್ಣಾಂಕ*)(sq + cn);
        ಆದರೆ( ಖಂಡ_Sq_ಮೊತ್ತ )
            sqbuf = sbuf + cn;
        ಗಾಗಿ( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_ಗಾತ್ರ();
    }

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( j = 0; j < ಒಟ್ಟು; j += ಖಂಡ_ಗಾತ್ರ_2 )
        {
            ಪೂರ್ಣಾಂಕ bsz = std::min(ಒಟ್ಟು - j, ಖಂಡ_ಗಾತ್ರ_2);
            ಪೂರ್ಣಾಂಕ nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            ಎಣಿಕೆ_3 += nz;
            nz0 += nz;
            ಆದರೆ( ಖಂಡ_ಮೊತ್ತ && (ಎಣಿಕೆ_3 + ಖಂಡ_ಗಾತ್ರ_2 >= partial_ಖಂಡ_ಗಾತ್ರ || (i+1 >= it.nplanes && j+bsz >= ಒಟ್ಟು)) )
            {
                ಆದರೆ (partial_ಮೊತ್ತ_Is_Int) {
                    ಗಾಗಿ( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಗಾಗಿ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ದಶಮಾಂಶ*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                ಆದರೆ( ಖಂಡ_Sq_ಮೊತ್ತ )
                {
                    ಗಾಗಿ( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                ಎಣಿಕೆ_3 = 0;
            }
            ptrs[0] += bsz*esz;
            ಆದರೆ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    ದ್ವಿಗುಣ scale = nz0 ? 1./nz0 : 0.;
    ಗಾಗಿ( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::ವರ್ಗಮೂಲ(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    ಆದರೆ (ಸರಾಸರಿ.needed())
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* sptr = s;
        ದ್ವಿಗುಣ* dptr = ಸರಾಸರಿ_mat.ptr<ದ್ವಿಗುಣ>();
        ಗಾಗಿ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    ಆದರೆ (_sdv.needed())
    {
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* sptr = sq;
        ದ್ವಿಗುಣ* dptr = stddev_mat.ptr<ದ್ವಿಗುಣ>();
        ಗಾಗಿ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ write_Stat_ಫಲಿತಾಂಶ(OutputArray _dst, ಪೂರ್ಣಾಂಕ cn, ಸ್ಥಿರಾಂಕ Scalar& v)
{
    ಆದರೆ (!_dst.needed())
        ಹಿಂತಿರುಗಿ;
    ಆದರೆ (!_dst.fixed_ಗಾತ್ರ())
        _dst.create(cn, 1, CV_64F, -1, ನಿಜ);
    Mat d = _dst.getMat();
    ಪೂರ್ಣಾಂಕ dcn = (ಪೂರ್ಣಾಂಕ)d.ಒಟ್ಟು();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || d.ಅಡ್ಡಸಾಲುಗಳು == 1) && dcn >= cn );
    ದ್ವಿಗುಣ* p = d.ptr<ದ್ವಿಗುಣ>();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

ಶೂನ್ಯ ಸರಾಸರಿ_Std_Dev(ಒಳಹರಿವು_Array _src, OutputArray ಸರಾಸರಿ, OutputArray _sdv, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ಆದರೆ (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = src.channels();
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ಮುಖವಾಡ_Ok = ಮುಖವಾಡ.empty() || ((ಮುಖವಾಡ.type() == CV_8U || ಮುಖವಾಡ.type() == CV_8S || ಮುಖವಾಡ.type() == CV_Bool) &&
                                             src.size == ಮುಖವಾಡ.size);
        ಆದರೆ (cn <= 4 && ಮುಖವಾಡ_Ok && ವಿಭಜಿಸು_For_Parallel_Stat(src, ಮುಖವಾಡ, chunks))
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = (ಪೂರ್ಣಾಂಕ)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<ದ್ವಿಗುಣ> cnt(n);
            parallel_for_(Range(0, n), [&](ಸ್ಥಿರಾಂಕ Range& r)
            {
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = r.ಆರಂಭ; i < r.end; i++)
                {
                    Mat mi, si;
                    ಸರಾಸರಿ_Std_Dev_2(chunks[i].src, mi, si, chunks[i].ಮುಖವಾಡ);
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<ದ್ವಿಗುಣ>(k);
                        sd[i][k] = si.at<ದ್ವಿಗುಣ>(k);
                    }
                    cnt[i] = stat_ಎಣಿಕೆ(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            ದ್ವಿಗುಣ ಒಟ್ಟು = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                ಒಟ್ಟು += cnt[i];
            }
            ಆದರೆ (ಒಟ್ಟು > 0)
                M *= 1./ಒಟ್ಟು;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                {
                    ದ್ವಿಗುಣ dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++)
                SD[k] = ಒಟ್ಟು > 0 ? std::ವರ್ಗಮೂಲ(std::max(V[k]/ಒಟ್ಟು, 0.)) : 0;
            write_Stat_ಫಲಿತಾಂಶ(ಸರಾಸರಿ, cn, M);
            write_Stat_ಫಲಿತಾಂಶ(_sdv, cn, SD);
            ಹಿಂತಿರುಗಿ;
        }
    }
    ಸರಾಸರಿ_Std_Dev_2(_src, ಸರಾಸರಿ, _sdv, ಮುಖವಾಡ_2);
}

} // namespace
