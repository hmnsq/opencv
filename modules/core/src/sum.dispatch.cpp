// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"

#شامل "stat_parallel.hpp"
#شامل "sum.simd.hpp"
#شامل "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

فضاینام cv
{

SumFunc getSumFunc(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

منطقی ocl_sum( درون_داد_Array _src, Scalar & res, صحیح sum_op, درون_داد_Array _mask,
                     درون_داد_Array _src2, منطقی calc2, ثابت Scalar & res2 )
{
    CV_Assert(sum_op == OCL_OP_SUM || sum_op == OCL_OP_SUM_ABS || sum_op == OCL_OP_SUM_SQR);

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقی doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != درون_داد_Array_2::NONE,
        haveSrc2 = _src2.kind() != درون_داد_Array_2::NONE;
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    صحیح convert_cn = haveSrc2 ? mcn : cn;

    اگر ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        بازگشت نادرست;

    اگر (depth >= CV_16F)
        بازگشت نادرست;

    صحیح ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.maxWorkGroupSize();

    صحیح ddepth = std::max(sum_op == OCL_OP_SUM_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    صحیح wgs2_aligned = 1;
    وقتی (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ایستا ثابت نویسه * ثابت opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    نویسه cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], اندازه(cvt[0])),
                         opMap[sum_op], (صحیح)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], اندازه(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    اگر (k.empty())
        بازگشت نادرست;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2);

    اگر (haveMask)
    {
        اگر (haveSrc2)
            k.args(srcarg, src.cols, (صحیح)src.total(), ngroups, dbarg, maskarg, src2arg);
        وگرنه
            k.args(srcarg, src.cols, (صحیح)src.total(), ngroups, dbarg, maskarg);
    }
    وگرنه
    {
        اگر (haveSrc2)
            k.args(srcarg, src.cols, (صحیح)src.total(), ngroups, dbarg, src2arg);
        وگرنه
            k.args(srcarg, src.cols, (صحیح)src.total(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    اگر (k.run(1, &globalsize, &wgs, درست))
    {
        تعریفنوع Scalar (*part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<صحیح>, ocl_part_sum<اعشاری>, ocl_part_sum<دوگانه> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        اگر (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        بازگشت درست;
    }
    بازگشت نادرست;
}

#endif

// serial implementation, also used for the pieces of the parallel version
ایستا Scalar sum_(ثابت Mat& src)
{
    Scalar _res;
    صحیح cn = src.channels();

    اگر (src.dims <= 2)
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    وگرنه اگر (src.isContinuous())
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, 0, src.type(), (صحیح)src.total(), 1, &_res[0]);
    }

    صحیح k, depth = src.depth();
    SumFunc func = getSumFunc(depth);
    اگر (func == nullptr) {
        اگر (depth == CV_Bool && cn == 1)
            بازگشت Scalar((دوگانه)شمار_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح total = (صحیح)it.size, blockSize = total, partialBlockSize = 0;
    صحیح j, شمار_2 = 0;
    صحیح _buf[CV_CN_MAX];
    صحیح* buf = (صحیح*)&_res[0];
    size_t esz = 0;
    منطقی partialSumIsInt = depth < CV_32S;
    منطقی blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;

    اگر( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        برای( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( j = 0; j < total; j += blockSize )
        {
            صحیح bsz = std::min(total - j, blockSize);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            شمار_2 += bsz;
            اگر( blockSum && (شمار_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                اگر (partialSumIsInt) {
                    برای( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } وگرنه {
                    برای( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((اعشاری*)buf)[k];
                        buf[k] = 0;
                    }
                }
                شمار_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    بازگشت _res;
}

Scalar sum(درون_داد_Array _src)
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
    اگر (!splitForParallelStat(src, Mat(), chunks))
        بازگشت sum_(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (صحیح)chunks.size()), [&](ثابت Range& r)
    {
        برای (صحیح i = r.start; i < r.end; i++)
            part[i] = sum_(chunks[i].src);
    });
    برای (ثابت Scalar& v : part)
        _res += v;
    بازگشت _res;
}

} // namespace
