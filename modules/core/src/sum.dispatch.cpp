// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "stat.hpp"

#അന്തര്ഭാവ "stat_parallel.hpp"
#അന്തര്ഭാവ "sum.simd.hpp"
#അന്തര്ഭാവ "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv
{

SumFunc getSumFunc(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

തർക്കം ocl_sum( നിവേശം_Array _src, Scalar & res, പൂർണ്ണസംഖ്യ sum_op, നിവേശം_Array _mask,
                     നിവേശം_Array _src2, തർക്കം calc2, സ്ഥിരം Scalar & res2 )
{
    CV_Assert(sum_op == OCL_OP_SUM || sum_op == OCL_OP_SUM_ABS || sum_op == OCL_OP_SUM_SQR);

    സ്ഥിരം ocl::Device & dev = ocl::Device::getDefault();
    തർക്കം doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != നിവേശം_Array_2::NONE,
        haveSrc2 = _src2.kind() != നിവേശം_Array_2::NONE;
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    പൂർണ്ണസംഖ്യ convert_cn = haveSrc2 ? mcn : cn;

    എങ്കിൽ ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        തിരികെ അസത്യം;

    എങ്കിൽ (depth >= CV_16F)
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.maxWorkGroupSize();

    പൂർണ്ണസംഖ്യ ddepth = std::max(sum_op == OCL_OP_SUM_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    പൂർണ്ണസംഖ്യ wgs2_aligned = 1;
    ആകുമ്പോൾ (wgs2_aligned < (പൂർണ്ണസംഖ്യ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    അക്ഷരം cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], വലുപ്പം(cvt[0])),
                         opMap[sum_op], (പൂർണ്ണസംഖ്യ)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], വലുപ്പം(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2);

    എങ്കിൽ (haveMask)
    {
        എങ്കിൽ (haveSrc2)
            k.args(srcarg, src.cols, (പൂർണ്ണസംഖ്യ)src.total(), ngroups, dbarg, maskarg, src2arg);
        അല്ലെങ്കിൽ
            k.args(srcarg, src.cols, (പൂർണ്ണസംഖ്യ)src.total(), ngroups, dbarg, maskarg);
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (haveSrc2)
            k.args(srcarg, src.cols, (പൂർണ്ണസംഖ്യ)src.total(), ngroups, dbarg, src2arg);
        അല്ലെങ്കിൽ
            k.args(srcarg, src.cols, (പൂർണ്ണസംഖ്യ)src.total(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    എങ്കിൽ (k.run(1, &globalsize, &wgs, സത്യം))
    {
        തരനിർവചനം Scalar (*part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<പൂർണ്ണസംഖ്യ>, ocl_part_sum<ദശാംശം>, ocl_part_sum<ഇരട്ട> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        എങ്കിൽ (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
}

#endif

// serial implementation, also used for the pieces of the parallel version
സ്ഥിതം Scalar sum_(സ്ഥിരം Mat& src)
{
    Scalar _res;
    പൂർണ്ണസംഖ്യ cn = src.channels();

    എങ്കിൽ (src.dims <= 2)
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (src.isContinuous())
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, 0, src.type(), (പൂർണ്ണസംഖ്യ)src.total(), 1, &_res[0]);
    }

    പൂർണ്ണസംഖ്യ k, depth = src.depth();
    SumFunc func = getSumFunc(depth);
    എങ്കിൽ (func == nullptr) {
        എങ്കിൽ (depth == CV_Bool && cn == 1)
            തിരികെ Scalar((ഇരട്ട)എണ്ണം_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    സ്ഥിരം Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size, blockSize = total, partialBlockSize = 0;
    പൂർണ്ണസംഖ്യ j, എണ്ണം_2 = 0;
    പൂർണ്ണസംഖ്യ _buf[CV_CN_MAX];
    പൂർണ്ണസംഖ്യ* buf = (പൂർണ്ണസംഖ്യ*)&_res[0];
    size_t esz = 0;
    തർക്കം partialSumIsInt = depth < CV_32S;
    തർക്കം blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;

    എങ്കിൽ( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        വേണ്ടി( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( j = 0; j < total; j += blockSize )
        {
            പൂർണ്ണസംഖ്യ bsz = std::min(total - j, blockSize);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            എണ്ണം_2 += bsz;
            എങ്കിൽ( blockSum && (എണ്ണം_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                എങ്കിൽ (partialSumIsInt) {
                    വേണ്ടി( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } അല്ലെങ്കിൽ {
                    വേണ്ടി( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((ദശാംശം*)buf)[k];
                        buf[k] = 0;
                    }
                }
                എണ്ണം_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    തിരികെ _res;
}

Scalar sum(നിവേശം_Array _src)
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
    എങ്കിൽ (!splitForParallelStat(src, Mat(), chunks))
        തിരികെ sum_(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (പൂർണ്ണസംഖ്യ)chunks.size()), [&](സ്ഥിരം Range& r)
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = r.start; i < r.end; i++)
            part[i] = sum_(chunks[i].src);
    });
    വേണ്ടി (സ്ഥിരം Scalar& v : part)
        _res += v;
    തിരികെ _res;
}

} // namespace
