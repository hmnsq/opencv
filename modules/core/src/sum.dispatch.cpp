// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "stat.hpp"

#અન્તર્ભાવ "stat_parallel.hpp"
#અન્તર્ભાવ "sum.simd.hpp"
#અન્તર્ભાવ "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv
{

SumFunc getSumFunc(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

તાર્કિક ocl_sum( આગત_Array _src, Scalar & res, પૂર્ણાંક sum_op, આગત_Array _mask,
                     આગત_Array _src2, તાર્કિક calc2, અચલ Scalar & res2 )
{
    CV_Assert(sum_op == OCL_OP_SUM || sum_op == OCL_OP_SUM_ABS || sum_op == OCL_OP_SUM_SQR);

    અચલ ocl::Device & dev = ocl::Device::getDefault();
    તાર્કિક doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != આગત_Array_2::NONE,
        haveSrc2 = _src2.kind() != આગત_Array_2::NONE;
    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    પૂર્ણાંક convert_cn = haveSrc2 ? mcn : cn;

    જો ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        પાછા ખોટું;

    જો (depth >= CV_16F)
        પાછા ખોટું;

    પૂર્ણાંક ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.maxWorkGroupSize();

    પૂર્ણાંક ddepth = std::max(sum_op == OCL_OP_SUM_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    પૂર્ણાંક wgs2_aligned = 1;
    જ્યારે (wgs2_aligned < (પૂર્ણાંક)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    સ્થિર અચલ અક્ષર * અચલ opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    અક્ષર cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], કદ(cvt[0])),
                         opMap[sum_op], (પૂર્ણાંક)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], કદ(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    જો (k.empty())
        પાછા ખોટું;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2);

    જો (haveMask)
    {
        જો (haveSrc2)
            k.args(srcarg, src.cols, (પૂર્ણાંક)src.total(), ngroups, dbarg, maskarg, src2arg);
        નહીંતર
            k.args(srcarg, src.cols, (પૂર્ણાંક)src.total(), ngroups, dbarg, maskarg);
    }
    નહીંતર
    {
        જો (haveSrc2)
            k.args(srcarg, src.cols, (પૂર્ણાંક)src.total(), ngroups, dbarg, src2arg);
        નહીંતર
            k.args(srcarg, src.cols, (પૂર્ણાંક)src.total(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    જો (k.run(1, &globalsize, &wgs, સાચું))
    {
        પ્રકારવ્યાખ્યા Scalar (*part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<પૂર્ણાંક>, ocl_part_sum<દશાંશ>, ocl_part_sum<દ્વિગુણ> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        જો (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        પાછા સાચું;
    }
    પાછા ખોટું;
}

#endif

// serial implementation, also used for the pieces of the parallel version
સ્થિર Scalar sum_(અચલ Mat& src)
{
    Scalar _res;
    પૂર્ણાંક cn = src.channels();

    જો (src.dims <= 2)
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    નહીંતર જો (src.isContinuous())
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, 0, src.type(), (પૂર્ણાંક)src.total(), 1, &_res[0]);
    }

    પૂર્ણાંક k, depth = src.depth();
    SumFunc func = getSumFunc(depth);
    જો (func == nullptr) {
        જો (depth == CV_Bool && cn == 1)
            પાછા Scalar((દ્વિગુણ)ગણતરી_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    અચલ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    પૂર્ણાંક total = (પૂર્ણાંક)it.size, blockSize = total, partialBlockSize = 0;
    પૂર્ણાંક j, ગણતરી_2 = 0;
    પૂર્ણાંક _buf[CV_CN_MAX];
    પૂર્ણાંક* buf = (પૂર્ણાંક*)&_res[0];
    size_t esz = 0;
    તાર્કિક partialSumIsInt = depth < CV_32S;
    તાર્કિક blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;

    જો( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        માટે( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( j = 0; j < total; j += blockSize )
        {
            પૂર્ણાંક bsz = std::min(total - j, blockSize);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            ગણતરી_2 += bsz;
            જો( blockSum && (ગણતરી_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                જો (partialSumIsInt) {
                    માટે( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } નહીંતર {
                    માટે( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((દશાંશ*)buf)[k];
                        buf[k] = 0;
                    }
                }
                ગણતરી_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    પાછા _res;
}

Scalar sum(આગત_Array _src)
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
    જો (!splitForParallelStat(src, Mat(), chunks))
        પાછા sum_(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (પૂર્ણાંક)chunks.size()), [&](અચલ Range& r)
    {
        માટે (પૂર્ણાંક i = r.start; i < r.end; i++)
            part[i] = sum_(chunks[i].src);
    });
    માટે (અચલ Scalar& v : part)
        _res += v;
    પાછા _res;
}

} // namespace
