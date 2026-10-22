// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "stat.hpp"

#অন্তর্ভাব "stat_parallel.hpp"
#অন্তর্ভাব "sum.simd.hpp"
#অন্তর্ভাব "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv
{

যোগ_Func get_যোগ_Func(পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_যোগ_Func, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

যৌক্তিক ocl_যোগ( প্রবেশ_Array _src, Scalar & res, পূর্ণসংখ্যা যোগ_op, প্রবেশ_Array _mask,
                     প্রবেশ_Array _src2, যৌক্তিক calc2, ধ্রুবক Scalar & res2 )
{
    CV_Assert(যোগ_op == OCL_OP_যোগ || যোগ_op == OCL_OP_যোগ_ABS || যোগ_op == OCL_OP_যোগ_SQR);

    ধ্রুবক ocl::Device & dev = ocl::Device::getDefault();
    যৌক্তিক doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != প্রবেশ_Array_2::NONE,
        haveSrc2 = _src2.kind() != প্রবেশ_Array_2::NONE;
    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    পূর্ণসংখ্যা convert_cn = haveSrc2 ? mcn : cn;

    যদি ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        ফেরত মিথ্যা;

    যদি (depth >= CV_16F)
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_Group_আকার();

    পূর্ণসংখ্যা ddepth = std::max(যোগ_op == OCL_OP_যোগ_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    পূর্ণসংখ্যা wgs2_aligned = 1;
    যখন (wgs2_aligned < (পূর্ণসংখ্যা)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    স্থির ধ্রুবক অক্ষর * ধ্রুবক opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    অক্ষর cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], আকার(cvt[0])),
                         opMap[যোগ_op], (পূর্ণসংখ্যা)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], আকার(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_আকার(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_আকার(mask),
            src2arg = ocl::KernelArg::Read_Only_No_আকার(src2);

    যদি (haveMask)
    {
        যদি (haveSrc2)
            k.args(srcarg, src.cols, (পূর্ণসংখ্যা)src.যোগ(), ngroups, dbarg, maskarg, src2arg);
        অন্যথা
            k.args(srcarg, src.cols, (পূর্ণসংখ্যা)src.যোগ(), ngroups, dbarg, maskarg);
    }
    অন্যথা
    {
        যদি (haveSrc2)
            k.args(srcarg, src.cols, (পূর্ণসংখ্যা)src.যোগ(), ngroups, dbarg, src2arg);
        অন্যথা
            k.args(srcarg, src.cols, (পূর্ণসংখ্যা)src.যোগ(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    যদি (k.run(1, &globalsize, &wgs, সত্য))
    {
        প্রকারলেখ Scalar (*part_যোগ)(Mat m);
        part_যোগ funcs[3] = { ocl_part_যোগ<পূর্ণসংখ্যা>, ocl_part_যোগ<দশমিক>, ocl_part_যোগ<দ্বিগুণ> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        যদি (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        ফেরত সত্য;
    }
    ফেরত মিথ্যা;
}

#endif

// serial implementation, also used for the pieces of the parallel version
স্থির Scalar যোগ_4(ধ্রুবক Mat& src)
{
    Scalar _res;
    পূর্ণসংখ্যা cn = src.channels();

    যদি (src.dims <= 2)
    {
        CALL_HAL_RET2(যোগ_2, cv_hal_যোগ, _res, src.তথ্য, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    অন্যথা যদি (src.isContinuous())
    {
        CALL_HAL_RET2(যোগ_2, cv_hal_যোগ, _res, src.তথ্য, 0, src.type(), (পূর্ণসংখ্যা)src.যোগ(), 1, &_res[0]);
    }

    পূর্ণসংখ্যা k, depth = src.depth();
    যোগ_Func func = get_যোগ_Func(depth);
    যদি (func == nullptr) {
        যদি (depth == CV_Bool && cn == 1)
            ফেরত Scalar((দ্বিগুণ)গুনতি_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    ধ্রুবক Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)it.size, block_আকার = যোগ, partial_Block_আকার = 0;
    পূর্ণসংখ্যা j, গুনতি_2 = 0;
    পূর্ণসংখ্যা _buf[CV_CN_MAX];
    পূর্ণসংখ্যা* buf = (পূর্ণসংখ্যা*)&_res[0];
    size_t esz = 0;
    যৌক্তিক partial_যোগ_Is_Int = depth < CV_32S;
    যৌক্তিক block_যোগ = partial_যোগ_Is_Int || depth == CV_16F || depth == CV_16BF;

    যদি( block_যোগ )
    {
        partial_Block_আকার = depth <= CV_8S ? (1 << 23) : (1 << 15);
        block_আকার = std::min(block_আকার, partial_Block_আকার);
        buf = _buf;
        জন্য( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_আকার();
    }

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( j = 0; j < যোগ; j += block_আকার )
        {
            পূর্ণসংখ্যা bsz = std::min(যোগ - j, block_আকার);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            গুনতি_2 += bsz;
            যদি( block_যোগ && (গুনতি_2 + block_আকার >= partial_Block_আকার || (i+1 >= it.nplanes && j+bsz >= যোগ)) )
            {
                যদি (partial_যোগ_Is_Int) {
                    জন্য( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } অন্যথা {
                    জন্য( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((দশমিক*)buf)[k];
                        buf[k] = 0;
                    }
                }
                গুনতি_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    ফেরত _res;
}

Scalar যোগ_2(প্রবেশ_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_যোগ(_src, _res, OCL_OP_যোগ),
                _res);
#endif

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    যদি (!splitForParallelStat(src, Mat(), chunks))
        ফেরত যোগ_4(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (পূর্ণসংখ্যা)chunks.size()), [&](ধ্রুবক Range& r)
    {
        জন্য (পূর্ণসংখ্যা i = r.start; i < r.end; i++)
            part[i] = যোগ_4(chunks[i].src);
    });
    জন্য (ধ্রুবক Scalar& v : part)
        _res += v;
    ফেরত _res;
}

} // namespace
