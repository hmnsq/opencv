// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "stat_parallel.hpp"
#include "sum.simd.hpp"
#include "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv
{

SumFunc getSumFunc(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL

bool ocl_sum( InputArray _src, Scalar & res, ကိန်းပြည့် sum_op, InputArray _mask,
                     InputArray _src2, bool calc2, const Scalar & res2 )
{
    CV_Assert(sum_op == OCL_OP_SUM || sum_op == OCL_OP_SUM_ABS || sum_op == OCL_OP_SUM_SQR);

    const ocl::Device & dev = ocl::Device::getDefault();
    bool doubleSupport = dev.doubleFPConfig() > 0,
        haveMask = _mask.kind() != _InputArray::NONE,
        haveSrc2 = _src2.kind() != _InputArray::NONE;
    ကိန်းပြည့် type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !haveMask ? ocl::predictOptimalVectorWidth(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    ကိန်းပြည့် convert_cn = haveSrc2 ? mcn : cn;

    အကယ်၍ ( (!doubleSupport && depth == CV_64F) || cn > 4 )
        ပြန်ပေး false;

    အကယ်၍ (depth >= CV_16F)
        ပြန်ပေး false;

    ကိန်းပြည့် ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.maxWorkGroupSize();

    ကိန်းပြည့် ddepth = std::max(sum_op == OCL_OP_SUM_SQR ? CV_32F : CV_32S, depth),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

    ကိန်းပြည့် wgs2_aligned = 1;
    နေစဉ် (wgs2_aligned < (ကိန်းပြည့်)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    static const var_na * const opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    var_na cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, mcn)), ocl::typeToStr(depth),
                         ocl::typeToStr(dtype), ocl::typeToStr(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::typeToStr(ddepth), ddepth, cn,
                         ocl::convertTypeStr(depth, ddepth, mcn, cvt[0], parimaa_na(cvt[0])),
                         opMap[sum_op], (ကိန်းပြည့်)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         haveMask ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         haveMask && _mask.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         depth <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], parimaa_na(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), mask = _mask.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::ReadOnlyNoSize(mask),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2);

    အကယ်၍ (haveMask)
    {
        အကယ်၍ (haveSrc2)
            k.args(srcarg, src.cols, (ကိန်းပြည့်)src.total(), ngroups, dbarg, maskarg, src2arg);
        မဟုတ်ပါက
            k.args(srcarg, src.cols, (ကိန်းပြည့်)src.total(), ngroups, dbarg, maskarg);
    }
    မဟုတ်ပါက
    {
        အကယ်၍ (haveSrc2)
            k.args(srcarg, src.cols, (ကိန်းပြည့်)src.total(), ngroups, dbarg, src2arg);
        မဟုတ်ပါက
            k.args(srcarg, src.cols, (ကိန်းပြည့်)src.total(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    အကယ်၍ (k.run(1, &globalsize, &wgs, true))
    {
        typedef Scalar (*part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<ကိန်းပြည့်>, ocl_part_sum<float>, ocl_part_sum<dviguu_na> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        အကယ်၍ (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        ပြန်ပေး true;
    }
    ပြန်ပေး false;
}

#endif

// serial implementation, also used for the pieces of the parallel version
static Scalar sum_(const Mat& src)
{
    Scalar _res;
    ကိန်းပြည့် cn = src.channels();

    အကယ်၍ (src.dims <= 2)
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, src.step, src.type(), src.cols, src.rows, &_res[0]);
    }
    မဟုတ်ပါက အကယ်၍ (src.isContinuous())
    {
        CALL_HAL_RET2(sum, cv_hal_sum, _res, src.data, 0, src.type(), (ကိန်းပြည့်)src.total(), 1, &_res[0]);
    }

    ကိန်းပြည့် k, depth = src.depth();
    SumFunc func = getSumFunc(depth);
    အကယ်၍ (func == nullptr) {
        အကယ်၍ (depth == CV_Bool && cn == 1)
            ပြန်ပေး Scalar((dviguu_na)countNonZero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            typeToString(src.type()).c_str()));
    }

    const Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    ကိန်းပြည့် total = (ကိန်းပြည့်)it.size, blockSize = total, partialBlockSize = 0;
    ကိန်းပြည့် j, count = 0;
    ကိန်းပြည့် _buf[CV_CN_MAX];
    ကိန်းပြည့်* buf = (ကိန်းပြည့်*)&_res[0];
    size_t esz = 0;
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;

    အကယ်၍( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        အတွက်( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        အတွက်( j = 0; j < total; j += blockSize )
        {
            ကိန်းပြည့် bsz = std::min(total - j, blockSize);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            count += bsz;
            အကယ်၍( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                အကယ်၍ (partialSumIsInt) {
                    အတွက်( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } မဟုတ်ပါက {
                    အတွက်( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((float*)buf)[k];
                        buf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    ပြန်ပေး _res;
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
    အကယ်၍ (!splitForParallelStat(src, Mat(), chunks))
        ပြန်ပေး sum_(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (ကိန်းပြည့်)chunks.size()), [&](const Range& r)
    {
        အတွက် (ကိန်းပြည့် i = r.start; i < r.end; i++)
            part[i] = sum_(chunks[i].src);
    });
    အတွက် (const Scalar& v : part)
        _res += v;
    ပြန်ပေး _res;
}

} // namespace
