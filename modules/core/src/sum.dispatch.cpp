// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "stat.hpp"

#அந்தர்பாவ "stat_parallel.hpp"
#அந்தர்பாவ "sum.simd.hpp"
#அந்தர்பாவ "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv
{

SumFunc getSumFunc(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

தருக்க ocl_sum( உள்ளீடு_Array _src, Scalar & res, முழு sum_op, உள்ளீடு_Array _mask,
                     உள்ளீடு_Array _src2, தருக்க calc2, மாறிலி Scalar & res2 )
{
    CV_Assert(sum_op == OCL_OP_SUM || sum_op == OCL_OP_SUM_ABS || sum_op == OCL_OP_SUM_SQR);

    மாறிலி ocl::Device & dev = ocl::Device::getDefault();
    தருக்க doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != உள்ளீடு_Array_2::NONE,
        haveSrc2 = _src2.kind() != உள்ளீடு_Array_2::NONE;
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    முழு convert_cn = haveSrc2 ? mcn : cn;

    எனில் ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        திரும்பு பொய்;

    எனில் (depth >= CV_16F)
        திரும்பு பொய்;

    முழு ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.maxWorkGroupSize();

    முழு ddepth = std::max(sum_op == OCL_OP_SUM_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    முழு wgs2_aligned = 1;
    போது (wgs2_aligned < (முழு)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    நிலையான மாறிலி எழுத்து * மாறிலி opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    எழுத்து cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], அளவு(cvt[0])),
                         opMap[sum_op], (முழு)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], அளவு(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2);

    எனில் (haveMask)
    {
        எனில் (haveSrc2)
            k.args(srcarg, src.cols, (முழு)src.total(), ngroups, dbarg, maskarg, src2arg);
        இல்லையெனில்
            k.args(srcarg, src.cols, (முழு)src.total(), ngroups, dbarg, maskarg);
    }
    இல்லையெனில்
    {
        எனில் (haveSrc2)
            k.args(srcarg, src.cols, (முழு)src.total(), ngroups, dbarg, src2arg);
        இல்லையெனில்
            k.args(srcarg, src.cols, (முழு)src.total(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    எனில் (k.run(1, &globalsize, &wgs, உண்மை))
    {
        வகைவரை Scalar (*part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<முழு>, ocl_part_sum<தசம>, ocl_part_sum<இரட்டை> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        எனில் (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        திரும்பு உண்மை;
    }
    திரும்பு பொய்;
}

#endif

// serial implementation, also used for the pieces of the parallel version
நிலையான Scalar sum_(மாறிலி Mat& src)
{
    Scalar _res;
    முழு cn = src.channels();

    எனில் (src.dims <= 2)
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    இல்லையெனில் எனில் (src.isContinuous())
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, 0, src.type(), (முழு)src.total(), 1, &_res[0]);
    }

    முழு k, depth = src.depth();
    SumFunc func = getSumFunc(depth);
    எனில் (func == nullptr) {
        எனில் (depth == CV_Bool && cn == 1)
            திரும்பு Scalar((இரட்டை)மொத்தம்_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    மாறிலி Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு total = (முழு)it.size, blockSize = total, partialBlockSize = 0;
    முழு j, மொத்தம்_2 = 0;
    முழு _buf[CV_CN_MAX];
    முழு* buf = (முழு*)&_res[0];
    size_t esz = 0;
    தருக்க partialSumIsInt = depth < CV_32S;
    தருக்க blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;

    எனில்( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        க்கு( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( j = 0; j < total; j += blockSize )
        {
            முழு bsz = std::min(total - j, blockSize);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            மொத்தம்_2 += bsz;
            எனில்( blockSum && (மொத்தம்_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                எனில் (partialSumIsInt) {
                    க்கு( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } இல்லையெனில் {
                    க்கு( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((தசம*)buf)[k];
                        buf[k] = 0;
                    }
                }
                மொத்தம்_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    திரும்பு _res;
}

Scalar sum(உள்ளீடு_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_sum(_src, _res, OCL_OP_SUM),
                _res);
#endif

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    எனில் (!splitForParallelStat(src, Mat(), chunks))
        திரும்பு sum_(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (முழு)chunks.size()), [&](மாறிலி Range& r)
    {
        க்கு (முழு i = r.start; i < r.end; i++)
            part[i] = sum_(chunks[i].src);
    });
    க்கு (மாறிலி Scalar& v : part)
        _res += v;
    திரும்பு _res;
}

} // namespace
