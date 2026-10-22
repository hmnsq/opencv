// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"

#अन्तर्भाव "stat_parallel.hpp"
#अन्तर्भाव "sum.simd.hpp"
#अन्तर्भाव "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv
{

SumFunc getSumFunc(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

तार्किक ocl_sum( आदान_Array _src, Scalar & res, पूर्णाङ्क sum_op, आदान_Array _mask,
                     आदान_Array _src2, तार्किक calc2, ध्रुव Scalar & res2 )
{
    CV_Assert(sum_op == OCL_OP_SUM || sum_op == OCL_OP_SUM_ABS || sum_op == OCL_OP_SUM_SQR);

    ध्रुव ocl::Device & dev = ocl::Device::getDefault();
    तार्किक doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != आदान_Array_2::NONE,
        haveSrc2 = _src2.kind() != आदान_Array_2::NONE;
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    पूर्णाङ्क convert_cn = haveSrc2 ? mcn : cn;

    यदि ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        प्रत्यागम असत्य;

    यदि (depth >= CV_16F)
        प्रत्यागम असत्य;

    पूर्णाङ्क ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.maxWorkGroupSize();

    पूर्णाङ्क ddepth = std::max(sum_op == OCL_OP_SUM_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    पूर्णाङ्क wgs2_aligned = 1;
    यावत् (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    स्थिर ध्रुव वर्ण * ध्रुव opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    वर्ण cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], परिमाण(cvt[0])),
                         opMap[sum_op], (पूर्णाङ्क)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], परिमाण(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2);

    यदि (haveMask)
    {
        यदि (haveSrc2)
            k.args(srcarg, src.cols, (पूर्णाङ्क)src.total(), ngroups, dbarg, maskarg, src2arg);
        अन्यथा
            k.args(srcarg, src.cols, (पूर्णाङ्क)src.total(), ngroups, dbarg, maskarg);
    }
    अन्यथा
    {
        यदि (haveSrc2)
            k.args(srcarg, src.cols, (पूर्णाङ्क)src.total(), ngroups, dbarg, src2arg);
        अन्यथा
            k.args(srcarg, src.cols, (पूर्णाङ्क)src.total(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    यदि (k.run(1, &globalsize, &wgs, सत्य))
    {
        प्रकारनिर्देश Scalar (*part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<पूर्णाङ्क>, ocl_part_sum<भिन्न>, ocl_part_sum<द्विगुण> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        यदि (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

#endif

// serial implementation, also used for the pieces of the parallel version
स्थिर Scalar sum_(ध्रुव Mat& src)
{
    Scalar _res;
    पूर्णाङ्क cn = src.channels();

    यदि (src.dims <= 2)
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    अन्यथा यदि (src.isContinuous())
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, 0, src.type(), (पूर्णाङ्क)src.total(), 1, &_res[0]);
    }

    पूर्णाङ्क k, depth = src.depth();
    SumFunc func = getSumFunc(depth);
    यदि (func == nullptr) {
        यदि (depth == CV_Bool && cn == 1)
            प्रत्यागम Scalar((द्विगुण)मोजणी_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    ध्रुव Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क total = (पूर्णाङ्क)it.size, blockSize = total, partialBlockSize = 0;
    पूर्णाङ्क j, मोजणी_2 = 0;
    पूर्णाङ्क _buf[CV_CN_MAX];
    पूर्णाङ्क* buf = (पूर्णाङ्क*)&_res[0];
    size_t esz = 0;
    तार्किक partialSumIsInt = depth < CV_32S;
    तार्किक blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;

    यदि( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        प्रति( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( j = 0; j < total; j += blockSize )
        {
            पूर्णाङ्क bsz = std::min(total - j, blockSize);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            मोजणी_2 += bsz;
            यदि( blockSum && (मोजणी_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                यदि (partialSumIsInt) {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } अन्यथा {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((भिन्न*)buf)[k];
                        buf[k] = 0;
                    }
                }
                मोजणी_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    प्रत्यागम _res;
}

Scalar sum(आदान_Array _src)
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
    यदि (!splitForParallelStat(src, Mat(), chunks))
        प्रत्यागम sum_(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (पूर्णाङ्क)chunks.size()), [&](ध्रुव Range& r)
    {
        प्रति (पूर्णाङ्क i = r.start; i < r.end; i++)
            part[i] = sum_(chunks[i].src);
    });
    प्रति (ध्रुव Scalar& v : part)
        _res += v;
    प्रत्यागम _res;
}

} // namespace
