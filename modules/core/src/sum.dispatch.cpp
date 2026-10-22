// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "stat_parallel.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "sum.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv
{

ਜੋੜ_Func get_ਜੋੜ_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ਜੋੜ_Func, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਤਾਰਕਿਕ ocl_ਜੋੜ( ਨਿਵੇਸ਼_Array _src, Scalar & res, ਪੂਰਨ ਜੋੜ_op, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2,
                     ਨਿਵੇਸ਼_Array _src2, ਤਾਰਕਿਕ calc2, ਅਚਲ Scalar & res2 )
{
    CV_Assert(ਜੋੜ_op == OCL_OP_ਜੋੜ || ਜੋੜ_op == OCL_OP_ਜੋੜ_ABS || ਜੋੜ_op == OCL_OP_ਜੋੜ_SQR);

    ਅਚਲ ocl::Device & dev = ocl::Device::getDefault();
    ਤਾਰਕਿਕ doubleSupport = dev.doubleFPConfig() > 0,
        have_ਨਕਾਬ = ਨਕਾਬ_2.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::NONE,
        haveSrc2 = _src2.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::NONE;
    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !have_ਨਕਾਬ ? ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    ਪੂਰਨ convert_cn = haveSrc2 ? mcn : cn;

    ਜੇ ( (!doubleSupport && ਡੂੰਘਾਈ == CV_64F) || cn > 4 )
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (ਡੂੰਘਾਈ >= CV_16F)
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_ਸਮੂਹ_ਮਾਪ();

    ਪੂਰਨ ddepth = std::max(ਜੋੜ_op == OCL_OP_ਜੋੜ_SQR ? CV_32F : CV_32S, ਡੂੰਘਾਈ),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!have_ਨਕਾਬ || ਨਕਾਬ_2.type() == CV_8U || ਨਕਾਬ_2.type() == CV_8S || ਨਕਾਬ_2.type() == CV_Bool);

    ਪੂਰਨ wgs2_aligned = 1;
    ਜਦੋਂ (wgs2_aligned < (ਪੂਰਨ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    ਅੱਖਰ cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, mcn)), ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(dtype), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth), ddepth, cn,
                         ocl::convertTypeStr(ਡੂੰਘਾਈ, ddepth, mcn, cvt[0], ਆਕਾਰ(cvt[0])),
                         opMap[ਜੋੜ_op], (ਪੂਰਨ)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         have_ਨਕਾਬ ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         have_ਨਕਾਬ && ਨਕਾਬ_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         ਡੂੰਘਾਈ <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], ਆਕਾਰ(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), ਨਕਾਬ = ਨਕਾਬ_2.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_ਮਾਪ(ਨਕਾਬ),
            src2arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src2);

    ਜੇ (have_ਨਕਾਬ)
    {
        ਜੇ (haveSrc2)
            k.ਦਲੀਲਾਂ(srcarg, src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(), ngroups, dbarg, maskarg, src2arg);
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(srcarg, src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(), ngroups, dbarg, maskarg);
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (haveSrc2)
            k.ਦਲੀਲਾਂ(srcarg, src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(), ngroups, dbarg, src2arg);
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(srcarg, src.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src.ਕੁੱਲ(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    ਜੇ (k.ਲੜੀਵਾਰ(1, &globalsize, &wgs, ਸੱਚ))
    {
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ Scalar (*part_ਜੋੜ)(Mat m);
        part_ਜੋੜ funcs[3] = { ocl_part_ਜੋੜ<ਪੂਰਨ>, ocl_part_ਜੋੜ<ਦਸ਼ਮਲਵ>, ocl_part_ਜੋੜ<ਦੁੱਗਣਾ> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        ਜੇ (calc2)
            const_cast<Scalar &>(res2) = func(mres.ਖਾਨਾ_Range_2(ngroups, dbsize));

        res = func(mres.ਖਾਨਾ_Range_2(0, ngroups));
        ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਅੰਤ

// serial implementation, also used for the pieces of the parallel version
ਸਥਿਰ Scalar ਜੋੜ_2(ਅਚਲ Mat& src)
{
    Scalar _res;
    ਪੂਰਨ cn = src.channels();

    ਜੇ (src.dims <= 2)
    {
        CALL_HAL_RET2(ਜੋੜ, cv_hal_ਜੋੜ, _res, src.ਅੰਕੜੇ, src.ਕਦਮ, src.type(), src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, &_res[0]);
    }
    ਨਹੀਂਤਾਂ ਜੇ (src.isContinuous())
    {
        CALL_HAL_RET2(ਜੋੜ, cv_hal_ਜੋੜ, _res, src.ਅੰਕੜੇ, 0, src.type(), (ਪੂਰਨ)src.ਕੁੱਲ(), 1, &_res[0]);
    }

    ਪੂਰਨ k, ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ();
    ਜੋੜ_Func func = get_ਜੋੜ_Func(ਡੂੰਘਾਈ);
    ਜੇ (func == ਖਾਲੀ_ਸੂਚਕ) {
        ਜੇ (ਡੂੰਘਾਈ == CV_Bool && cn == 1)
            ਵਾਪਸ Scalar((ਦੁੱਗਣਾ)ਗਿਣਤੀ_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            type_ਮੰਜ਼ਿਲ_String(src.type()).c_str()));
    }

    ਅਚਲ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size, ਖੰਡ_ਮਾਪ_2 = ਕੁੱਲ, partial_ਖੰਡ_ਮਾਪ = 0;
    ਪੂਰਨ j, ਗਿਣਤੀ_3 = 0;
    ਪੂਰਨ _buf[CV_CN_MAX];
    ਪੂਰਨ* buf = (ਪੂਰਨ*)&_res[0];
    size_t esz = 0;
    ਤਾਰਕਿਕ partial_ਜੋੜ_Is_Int = ਡੂੰਘਾਈ < CV_32S;
    ਤਾਰਕਿਕ ਖੰਡ_ਜੋੜ = partial_ਜੋੜ_Is_Int || ਡੂੰਘਾਈ == CV_16F || ਡੂੰਘਾਈ == CV_16BF;

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
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            ਗਿਣਤੀ_3 += bsz;
            ਜੇ( ਖੰਡ_ਜੋੜ && (ਗਿਣਤੀ_3 + ਖੰਡ_ਮਾਪ_2 >= partial_ਖੰਡ_ਮਾਪ || (i+1 >= it.nplanes && j+bsz >= ਕੁੱਲ)) )
            {
                ਜੇ (partial_ਜੋੜ_Is_Int) {
                    ਲਈ( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } ਨਹੀਂਤਾਂ {
                    ਲਈ( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((ਦਸ਼ਮਲਵ*)buf)[k];
                        buf[k] = 0;
                    }
                }
                ਗਿਣਤੀ_3 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    ਵਾਪਸ _res;
}

Scalar ਜੋੜ(ਨਿਵੇਸ਼_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    CV_OCL_ਲੜੀਵਾਰ_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_ਜੋੜ(_src, _res, OCL_OP_ਜੋੜ),
                _res);
#ਜੇ_ਅੰਤ

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    ਜੇ (!ਵੰਡੋ_For_Parallel_Stat(src, Mat(), chunks))
        ਵਾਪਸ ਜੋੜ_2(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (ਪੂਰਨ)chunks.size()), [&](ਅਚਲ Range& r)
    {
        ਲਈ (ਪੂਰਨ i = r.ਸ਼ੁਰੂ; i < r.end; i++)
            part[i] = ਜੋੜ_2(chunks[i].src);
    });
    ਲਈ (ਅਚਲ Scalar& v : part)
        _res += v;
    ਵਾਪਸ _res;
}

} // namespace
