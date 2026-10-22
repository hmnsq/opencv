// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencl_kernels_core.hpp"
#অন্তর্ভাৱ "stat.hpp"

#অন্তর্ভাৱ "stat_parallel.hpp"
#অন্তর্ভাৱ "sum.simd.hpp"
#অন্তর্ভাৱ "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv
{

SumFunc getSumFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

তার্কিক ocl_sum( প্ৰবেশ_Array _src, Scalar & res, পূর্ণাঙ্ক sum_op, প্ৰবেশ_Array _mask,
                     প্ৰবেশ_Array _src2, তার্কিক calc2, ধ্রুৱ Scalar & res2 )
{
    CV_Assert(sum_op == OCL_OP_SUM || sum_op == OCL_OP_SUM_ABS || sum_op == OCL_OP_SUM_SQR);

    ধ্রুৱ ocl::Device & dev = ocl::Device::getDefault();
    তার্কিক doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != প্ৰবেশ_Array_2::NONE,
        haveSrc2 = _src2.kind() != প্ৰবেশ_Array_2::NONE;
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    পূর্ণাঙ্ক convert_cn = haveSrc2 ? mcn : cn;

    যদি ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        ঘূৰাই মিছা;

    যদি (depth >= CV_16F)
        ঘূৰাই মিছা;

    পূর্ণাঙ্ক ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.maxWorkGroupSize();

    পূর্ণাঙ্ক ddepth = std::max(sum_op == OCL_OP_SUM_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    পূর্ণাঙ্ক wgs2_aligned = 1;
    যেতিয়ালৈ (wgs2_aligned < (পূর্ণাঙ্ক)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    ৱর্ণ cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], পরিমাণ(cvt[0])),
                         opMap[sum_op], (পূর্ণাঙ্ক)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], পরিমাণ(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    যদি (k.empty())
        ঘূৰাই মিছা;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2);

    যদি (haveMask)
    {
        যদি (haveSrc2)
            k.args(srcarg, src.cols, (পূর্ণাঙ্ক)src.total(), ngroups, dbarg, maskarg, src2arg);
        অন্যথা
            k.args(srcarg, src.cols, (পূর্ণাঙ্ক)src.total(), ngroups, dbarg, maskarg);
    }
    অন্যথা
    {
        যদি (haveSrc2)
            k.args(srcarg, src.cols, (পূর্ণাঙ্ক)src.total(), ngroups, dbarg, src2arg);
        অন্যথা
            k.args(srcarg, src.cols, (পূর্ণাঙ্ক)src.total(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    যদি (k.run(1, &globalsize, &wgs, সঁচা))
    {
        প্রকারনির্দেশ Scalar (*part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<পূর্ণাঙ্ক>, ocl_part_sum<ভিন্ন>, ocl_part_sum<দ্ৱিগুণ> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        যদি (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
}

#endif

// serial implementation, also used for the pieces of the parallel version
স্থির Scalar sum_(ধ্রুৱ Mat& src)
{
    Scalar _res;
    পূর্ণাঙ্ক cn = src.channels();

    যদি (src.dims <= 2)
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    অন্যথা যদি (src.isContinuous())
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, 0, src.type(), (পূর্ণাঙ্ক)src.total(), 1, &_res[0]);
    }

    পূর্ণাঙ্ক k, depth = src.depth();
    SumFunc func = getSumFunc(depth);
    যদি (func == nullptr) {
        যদি (depth == CV_Bool && cn == 1)
            ঘূৰাই Scalar((দ্ৱিগুণ)গুনতি_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    ধ্রুৱ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size, blockSize = total, partialBlockSize = 0;
    পূর্ণাঙ্ক j, গুনতি_2 = 0;
    পূর্ণাঙ্ক _buf[CV_CN_MAX];
    পূর্ণাঙ্ক* buf = (পূর্ণাঙ্ক*)&_res[0];
    size_t esz = 0;
    তার্কিক partialSumIsInt = depth < CV_32S;
    তার্কিক blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;

    যদি( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        বাবে( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        বাবে( j = 0; j < total; j += blockSize )
        {
            পূর্ণাঙ্ক bsz = std::min(total - j, blockSize);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            গুনতি_2 += bsz;
            যদি( blockSum && (গুনতি_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                যদি (partialSumIsInt) {
                    বাবে( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } অন্যথা {
                    বাবে( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((ভিন্ন*)buf)[k];
                        buf[k] = 0;
                    }
                }
                গুনতি_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    ঘূৰাই _res;
}

Scalar sum(প্ৰবেশ_Array _src)
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
    যদি (!splitForParallelStat(src, Mat(), chunks))
        ঘূৰাই sum_(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (পূর্ণাঙ্ক)chunks.size()), [&](ধ্রুৱ Range& r)
    {
        বাবে (পূর্ণাঙ্ক i = r.start; i < r.end; i++)
            part[i] = sum_(chunks[i].src);
    });
    বাবে (ধ্রুৱ Scalar& v : part)
        _res += v;
    ঘূৰাই _res;
}

} // namespace
