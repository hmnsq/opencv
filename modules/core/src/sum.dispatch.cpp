// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

#ಅನ್ತರ್ಭಾವ "stat_parallel.hpp"
#ಅನ್ತರ್ಭಾವ "sum.simd.hpp"
#ಅನ್ತರ್ಭಾವ "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv
{

ಮೊತ್ತ_Func get_ಮೊತ್ತ_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ಮೊತ್ತ_Func, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ತಾರ್ಕಿಕ ocl_ಮೊತ್ತ( ಒಳಹರಿವು_Array _src, Scalar & res, ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_op, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2,
                     ಒಳಹರಿವು_Array _src2, ತಾರ್ಕಿಕ calc2, ಸ್ಥಿರಾಂಕ Scalar & res2 )
{
    CV_Assert(ಮೊತ್ತ_op == OCL_OP_ಮೊತ್ತ || ಮೊತ್ತ_op == OCL_OP_ಮೊತ್ತ_ABS || ಮೊತ್ತ_op == OCL_OP_ಮೊತ್ತ_SQR);

    ಸ್ಥಿರಾಂಕ ocl::Device & dev = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = dev.doubleFPConfig() > 0,
        have_ಮುಖವಾಡ = ಮುಖವಾಡ_2.ಬಗೆ() != ಒಳಹರಿವು_Array_2::NONE,
        haveSrc2 = _src2.ಬಗೆ() != ಒಳಹರಿವು_Array_2::NONE;
    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !have_ಮುಖವಾಡ ? ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    ಪೂರ್ಣಾಂಕ convert_cn = haveSrc2 ? mcn : cn;

    ಆದರೆ ( (!doubleSupport && ಆಳ == CV_64F) || cn > 4 )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (ಆಳ >= CV_16F)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_ಗುಂಪು_ಗಾತ್ರ();

    ಪೂರ್ಣಾಂಕ ddepth = std::max(ಮೊತ್ತ_op == OCL_OP_ಮೊತ್ತ_SQR ? CV_32F : CV_32S, ಆಳ),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!have_ಮುಖವಾಡ || ಮುಖವಾಡ_2.type() == CV_8U || ಮುಖವಾಡ_2.type() == CV_8S || ಮುಖವಾಡ_2.type() == CV_Bool);

    ಪೂರ್ಣಾಂಕ wgs2_aligned = 1;
    ಆಗ (wgs2_aligned < (ಪೂರ್ಣಾಂಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    ಅಕ್ಷರ cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, mcn)), ocl::type_ಗಮ್ಯ_Str(ಆಳ),
                         ocl::type_ಗಮ್ಯ_Str(dtype), ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::type_ಗಮ್ಯ_Str(ddepth), ddepth, cn,
                         ocl::convertTypeStr(ಆಳ, ddepth, mcn, cvt[0], ಗಾತ್ರ(cvt[0])),
                         opMap[ಮೊತ್ತ_op], (ಪೂರ್ಣಾಂಕ)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         have_ಮುಖವಾಡ ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         have_ಮುಖವಾಡ && ಮುಖವಾಡ_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         ಆಳ <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], ಗಾತ್ರ(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(ಮುಖವಾಡ),
            src2arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2);

    ಆದರೆ (have_ಮುಖವಾಡ)
    {
        ಆದರೆ (haveSrc2)
            k.ವಾದಗಳು(srcarg, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), ngroups, dbarg, maskarg, src2arg);
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(srcarg, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), ngroups, dbarg, maskarg);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (haveSrc2)
            k.ವಾದಗಳು(srcarg, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), ngroups, dbarg, src2arg);
        ಇಲ್ಲದಿದ್ದರೆ
            k.ವಾದಗಳು(srcarg, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    ಆದರೆ (k.ಸತತ(1, &globalsize, &wgs, ನಿಜ))
    {
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Scalar (*part_ಮೊತ್ತ)(Mat m);
        part_ಮೊತ್ತ funcs[3] = { ocl_part_ಮೊತ್ತ<ಪೂರ್ಣಾಂಕ>, ocl_part_ಮೊತ್ತ<ದಶಮಾಂಶ>, ocl_part_ಮೊತ್ತ<ದ್ವಿಗುಣ> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        ಆದರೆ (calc2)
            const_cast<Scalar &>(res2) = func(mres.ಲಂಬಸಾಲು_Range_2(ngroups, dbsize));

        res = func(mres.ಲಂಬಸಾಲು_Range_2(0, ngroups));
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

#ಆದರೆ_ಅಂತ್ಯ

// serial implementation, also used for the pieces of the parallel version
ಸ್ಥಿರ Scalar ಮೊತ್ತ_2(ಸ್ಥಿರಾಂಕ Mat& src)
{
    Scalar _res;
    ಪೂರ್ಣಾಂಕ cn = src.channels();

    ಆದರೆ (src.dims <= 2)
    {
        CALL_HAL_RET2(ಮೊತ್ತ, cv_hal_ಮೊತ್ತ, _res, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, src.type(), src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು, &_res[0]);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (src.isContinuous())
    {
        CALL_HAL_RET2(ಮೊತ್ತ, cv_hal_ಮೊತ್ತ, _res, src.ದತ್ತಾಂಶ, 0, src.type(), (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು(), 1, &_res[0]);
    }

    ಪೂರ್ಣಾಂಕ k, ಆಳ = src.ಆಳ();
    ಮೊತ್ತ_Func func = get_ಮೊತ್ತ_Func(ಆಳ);
    ಆದರೆ (func == ಶೂನ್ಯಸೂಚಕ) {
        ಆದರೆ (ಆಳ == CV_Bool && cn == 1)
            ಹಿಂತಿರುಗಿ Scalar((ದ್ವಿಗುಣ)ಎಣಿಕೆ_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            type_ಗಮ್ಯ_String(src.type()).c_str()));
    }

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size, ಖಂಡ_ಗಾತ್ರ_2 = ಒಟ್ಟು, partial_ಖಂಡ_ಗಾತ್ರ = 0;
    ಪೂರ್ಣಾಂಕ j, ಎಣಿಕೆ_3 = 0;
    ಪೂರ್ಣಾಂಕ _buf[CV_CN_MAX];
    ಪೂರ್ಣಾಂಕ* buf = (ಪೂರ್ಣಾಂಕ*)&_res[0];
    size_t esz = 0;
    ತಾರ್ಕಿಕ partial_ಮೊತ್ತ_Is_Int = ಆಳ < CV_32S;
    ತಾರ್ಕಿಕ ಖಂಡ_ಮೊತ್ತ = partial_ಮೊತ್ತ_Is_Int || ಆಳ == CV_16F || ಆಳ == CV_16BF;

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
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            ಎಣಿಕೆ_3 += bsz;
            ಆದರೆ( ಖಂಡ_ಮೊತ್ತ && (ಎಣಿಕೆ_3 + ಖಂಡ_ಗಾತ್ರ_2 >= partial_ಖಂಡ_ಗಾತ್ರ || (i+1 >= it.nplanes && j+bsz >= ಒಟ್ಟು)) )
            {
                ಆದರೆ (partial_ಮೊತ್ತ_Is_Int) {
                    ಗಾಗಿ( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಗಾಗಿ( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((ದಶಮಾಂಶ*)buf)[k];
                        buf[k] = 0;
                    }
                }
                ಎಣಿಕೆ_3 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    ಹಿಂತಿರುಗಿ _res;
}

Scalar ಮೊತ್ತ(ಒಳಹರಿವು_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    CV_OCL_ಸತತ_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_ಮೊತ್ತ(_src, _res, OCL_OP_ಮೊತ್ತ),
                _res);
#ಆದರೆ_ಅಂತ್ಯ

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    ಆದರೆ (!ವಿಭಜಿಸು_For_Parallel_Stat(src, Mat(), chunks))
        ಹಿಂತಿರುಗಿ ಮೊತ್ತ_2(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (ಪೂರ್ಣಾಂಕ)chunks.size()), [&](ಸ್ಥಿರಾಂಕ Range& r)
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = r.ಆರಂಭ; i < r.end; i++)
            part[i] = ಮೊತ್ತ_2(chunks[i].src);
    });
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ Scalar& v : part)
        _res += v;
    ಹಿಂತಿರುಗಿ _res;
}

} // namespace
