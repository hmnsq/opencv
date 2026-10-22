// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "stat.hpp"

#समावेश "stat_parallel.hpp"
#समावेश "sum.simd.hpp"
#समावेश "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv
{

योग_Func get_योग_Func(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_योग_Func, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#अगर_घोषित HAVE_OPENCL

bool ocl_योग( निवेश_Array _src, Scalar & res, पूर्णांक योग_op, निवेश_Array _mask,
                     निवेश_Array _src2, bool calc2, स्थिर Scalar & res2 )
{
    CV_Assert(योग_op == OCL_OP_योग || योग_op == OCL_OP_योग_ABS || योग_op == OCL_OP_योग_SQR);

    स्थिर ocl::Device & dev = ocl::Device::getDefault();
    bool doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != निवेश_Array_2::NONE,
        haveSrc2 = _src2.kind() != निवेश_Array_2::NONE;
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    पूर्णांक convert_cn = haveSrc2 ? mcn : cn;

    अगर ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        वापस false;

    अगर (depth >= CV_16F)
        वापस false;

    पूर्णांक ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    माप_प्रकार wgs = dev.max_Work_Group_आकार();

    पूर्णांक ddepth = std::max(योग_op == OCL_OP_योग_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    पूर्णांक wgs2_aligned = 1;
    जबतक (wgs2_aligned < (पूर्णांक)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    अटल स्थिर अक्षर * स्थिर opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    अक्षर cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], माप(cvt[0])),
                         opMap[योग_op], (पूर्णांक)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], माप(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    अगर (k.empty())
        वापस false;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_आकार(mask),
            src2arg = ocl::KernelArg::Read_Only_No_आकार(src2);

    अगर (haveMask)
    {
        अगर (haveSrc2)
            k.args(srcarg, src.cols, (पूर्णांक)src.योग(), ngroups, dbarg, maskarg, src2arg);
        अन्यथा
            k.args(srcarg, src.cols, (पूर्णांक)src.योग(), ngroups, dbarg, maskarg);
    }
    अन्यथा
    {
        अगर (haveSrc2)
            k.args(srcarg, src.cols, (पूर्णांक)src.योग(), ngroups, dbarg, src2arg);
        अन्यथा
            k.args(srcarg, src.cols, (पूर्णांक)src.योग(), ngroups, dbarg);
    }

    माप_प्रकार globalsize = ngroups * wgs;
    अगर (k.run(1, &globalsize, &wgs, true))
    {
        प्रकार Scalar (*part_योग)(Mat m);
        part_योग funcs[3] = { ocl_part_योग<पूर्णांक>, ocl_part_योग<भग्न>, ocl_part_योग<द्विगुन> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        अगर (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        वापस true;
    }
    वापस false;
}

#पूर्ण_अगर

// serial implementation, also used for the pieces of the parallel version
अटल Scalar योग_4(स्थिर Mat& src)
{
    Scalar _res;
    पूर्णांक cn = src.channels();

    अगर (src.dims <= 2)
    {
        CALL_HAL_RET2(योग_2, cv_hal_योग, _res, src.तथ्य, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    अन्यथा अगर (src.isContinuous())
    {
        CALL_HAL_RET2(योग_2, cv_hal_योग, _res, src.तथ्य, 0, src.type(), (पूर्णांक)src.योग(), 1, &_res[0]);
    }

    पूर्णांक k, depth = src.depth();
    योग_Func func = get_योग_Func(depth);
    अगर (func == nullptr) {
        अगर (depth == CV_Bool && cn == 1)
            वापस Scalar((द्विगुन)गिनती_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    स्थिर Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक योग = (पूर्णांक)it.size, block_आकार = योग, partial_Block_आकार = 0;
    पूर्णांक j, गिनती_2 = 0;
    पूर्णांक _buf[CV_CN_MAX];
    पूर्णांक* buf = (पूर्णांक*)&_res[0];
    माप_प्रकार esz = 0;
    bool partial_योग_Is_Int = depth < CV_32S;
    bool block_योग = partial_योग_Is_Int || depth == CV_16F || depth == CV_16BF;

    अगर( block_योग )
    {
        partial_Block_आकार = depth <= CV_8S ? (1 << 23) : (1 << 15);
        block_आकार = std::min(block_आकार, partial_Block_आकार);
        buf = _buf;
        क्रम( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_आकार();
    }

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( j = 0; j < योग; j += block_आकार )
        {
            पूर्णांक bsz = std::min(योग - j, block_आकार);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            गिनती_2 += bsz;
            अगर( block_योग && (गिनती_2 + block_आकार >= partial_Block_आकार || (i+1 >= it.nplanes && j+bsz >= योग)) )
            {
                अगर (partial_योग_Is_Int) {
                    क्रम( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } अन्यथा {
                    क्रम( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((भग्न*)buf)[k];
                        buf[k] = 0;
                    }
                }
                गिनती_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    वापस _res;
}

Scalar योग_2(निवेश_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#अगर_घोषित HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_योग(_src, _res, OCL_OP_योग),
                _res);
#पूर्ण_अगर

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    अगर (!splitForParallelStat(src, Mat(), chunks))
        वापस योग_4(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (पूर्णांक)chunks.size()), [&](स्थिर Range& r)
    {
        क्रम (पूर्णांक i = r.start; i < r.end; i++)
            part[i] = योग_4(chunks[i].src);
    });
    क्रम (स्थिर Scalar& v : part)
        _res += v;
    वापस _res;
}

} // namespace
