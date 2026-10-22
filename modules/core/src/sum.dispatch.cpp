// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "stat_parallel.hpp"
#ضمن "sum.simd.hpp"
#ضمن "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv
{

SumFunc getSumFunc(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

منطقي ocl_sum( InputArray _src, Scalar & res, صحيح sum_op, InputArray _mask,
                     InputArray _src2, منطقي calc2, ثابت Scalar & res2 )
{
    CV_Assert(sum_op == OCL_OP_SUM || sum_op == OCL_OP_SUM_ABS || sum_op == OCL_OP_SUM_SQR);

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != _InputArray::NONE,
        haveSrc2 = _src2.kind() != _InputArray::NONE;
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    صحيح convert_cn = haveSrc2 ? mcn : cn;

    𐤀𐤌 ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        أرجع خطأ;

    𐤀𐤌 (depth >= CV_16F)
        أرجع خطأ;

    صحيح ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.maxWorkGroupSize();

    صحيح ddepth = std::max(sum_op == OCL_OP_SUM_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ساكن ثابت حرف * ثابت opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    حرف cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], حجم(cvt[0])),
                         opMap[sum_op], (صحيح)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], حجم(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    𐤀𐤌 (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2);

    𐤀𐤌 (haveMask)
    {
        𐤀𐤌 (haveSrc2)
            k.args(srcarg, src.cols, (صحيح)src.total(), ngroups, dbarg, maskarg, src2arg);
        وإلا
            k.args(srcarg, src.cols, (صحيح)src.total(), ngroups, dbarg, maskarg);
    }
    وإلا
    {
        𐤀𐤌 (haveSrc2)
            k.args(srcarg, src.cols, (صحيح)src.total(), ngroups, dbarg, src2arg);
        وإلا
            k.args(srcarg, src.cols, (صحيح)src.total(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    𐤀𐤌 (k.run(1, &globalsize, &wgs, true))
    {
        تعريفنوع Scalar (*part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<صحيح>, ocl_part_sum<عائم>, ocl_part_sum<مزدوج> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        𐤀𐤌 (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        أرجع true;
    }
    أرجع خطأ;
}

#endif

// serial implementation, also used for the pieces of the parallel version
ساكن Scalar sum_(ثابت Mat& src)
{
    Scalar _res;
    صحيح cn = src.channels();

    𐤀𐤌 (src.dims <= 2)
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    وإلا 𐤀𐤌 (src.isContinuous())
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, 0, src.type(), (صحيح)src.total(), 1, &_res[0]);
    }

    صحيح k, depth = src.depth();
    SumFunc func = getSumFunc(depth);
    𐤀𐤌 (func == nullptr) {
        𐤀𐤌 (depth == CV_Bool && cn == 1)
            أرجع Scalar((مزدوج)countNonZero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح total = (صحيح)it.size, blockSize = total, partialBlockSize = 0;
    صحيح j, count = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&_res[0];
    size_t esz = 0;
    منطقي partialSumIsInt = depth < CV_32S;
    منطقي blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;

    𐤀𐤌( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        لكل( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < total; j += blockSize )
        {
            صحيح bsz = std::min(total - j, blockSize);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            count += bsz;
            𐤀𐤌( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                𐤀𐤌 (partialSumIsInt) {
                    لكل( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } وإلا {
                    لكل( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((عائم*)buf)[k];
                        buf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    أرجع _res;
}

Scalar sum(InputArray _src)
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
    𐤀𐤌 (!splitForParallelStat(src, Mat(), chunks))
        أرجع sum_(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (صحيح)chunks.size()), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.start; i < r.end; i++)
            part[i] = sum_(chunks[i].src);
    });
    لكل (ثابت Scalar& v : part)
        _res += v;
    أرجع _res;
}

} // namespace
