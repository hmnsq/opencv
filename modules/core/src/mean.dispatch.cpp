// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"
#include "stat_parallel.hpp"

#include "mean.simd.hpp"
#include "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


namespace cv {

// serial implementation, also used for the pieces of the parallel version
static Scalar mean_(const Mat& src, const Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    如果 (src.empty())
        返回 Scalar::all(0.0);

    整数 k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    如果 (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (整数)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    否则
    {
        如果 (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    const Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    整数 total = (整数)it.size, blockSize = total, partialBlockSize = 0;
    整数 j, count = 0;
    整数 _buf[CV_CN_MAX];
    整数* buf = (整数*)&s[0];
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    如果( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        为( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    为( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        为( j = 0; j < total; j += blockSize )
        {
            整数 bsz = std::min(total - j, blockSize);
            整数 nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            count += nz;
            nz0 += nz;
            如果( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                如果 (partialSumIsInt) {
                    为( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } 否则 {
                    为( k = 0; k < cn; k++ )
                    {
                        s[k] += ((float*)buf)[k];
                        buf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
            如果( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    返回 s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
static double statCount(const StatChunk& c)
{
    返回 c.mask.empty() ? (double)c.src.total() : (double)countNonZero(c.mask);
}

Scalar mean(InputArray _src, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    如果 (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        返回 mean_(src, mask);

    const 整数 n = (整数)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<double> cnt(n);
    parallel_for_(Range(0, n), [&](const Range& r)
    {
        为 (整数 i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = statCount(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    double total = 0;
    为 (整数 i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    返回 s*(total > 0 ? 1./total : 0);
}

static SumSqrFunc getSumSqrFunc(整数 depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_meanStdDev( InputArray _src, OutputArray _mean, OutputArray _sdv, InputArray _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    bool haveMask = _mask.kind() != _InputArray::NONE;
    整数 nz = haveMask ? -1 : (整数)_src.total();
    Scalar mean(0), stddev(0);
    const 整数 cn = _src.channels();
    如果 (cn > 4)
        返回 false;

    {
        整数 type = _src.type(), depth = CV_MAT_DEPTH(type);
        bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        const ocl::Device &defDev = ocl::Device::getDefault();
        整数 groups = defDev.maxComputeUnits();
        如果 (defDev.isIntel())
        {
            static const 整数 subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        整数 ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        整数 wgs2_aligned = 1;
        当 (wgs2_aligned < (整数)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        如果 ( (!doubleSupport && depth == CV_64F) )
            返回 false;

        char cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], sizeof(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], sizeof(cvt[1])),
                             (整数)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        如果 (k.empty())
            返回 false;

        整数 dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        如果 (haveMask)
            k.args(srcarg, src.cols, (整数)src.total(), groups, dbarg, maskarg);
        否则
            k.args(srcarg, src.cols, (整数)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        如果(!k.run(1, &globalsize, &wgs, false))
            返回 false;

        typedef Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<整数>, ocl_part_sum<float>, ocl_part_sum<double> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        如果 (haveMask)
            nz = saturate_cast<整数>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    double total = nz != 0 ? 1.0 / nz : 0;
    整数 k, j;
    为 (整数 i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    为( j = 0; j < 2; j++ )
    {
        const double * const sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        如果( !_dst.needed() )
            continue;

        如果( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, true);
        Mat dst = _dst.getMat();
        整数 dcn = (整数)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        double* dptr = dst.ptr<double>();
        为( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        为( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    返回 true;
}
#endif

// serial implementation, also used for the pieces of the parallel version
static void meanStdDev_(InputArray _src, OutputArray _mean, OutputArray _sdv, InputArray _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    整数 k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    如果(_mean.needed())
    {
        如果( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, true);

        mean_mat = _mean.getMat();
        整数 dcn = (整数)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        double* dptr = mean_mat.ptr<double>();
        为(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    如果 (_sdv.needed())
    {
        如果( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, true);

        stddev_mat = _sdv.getMat();
        整数 dcn = (整数)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        double* dptr = stddev_mat.ptr<double>();
        为(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    如果 (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (整数)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<double>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<double>() : nullptr,
                 mask.data, 0);
    }
    否则
    {
        如果 (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<double>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<double>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    const Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    整数 total = (整数)it.size, blockSize = total, partialBlockSize = 0;
    整数 j;
    int64_t count = 0, nz0 = 0;
    double _buf[CV_CN_MAX*4];
    double *s = _buf, *sq = s + cn;
    整数 *sbuf = (整数*)s, *sqbuf = (整数*)sq;
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    bool blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    为( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    如果( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (整数*)(sq + cn);
        如果( blockSqSum )
            sqbuf = sbuf + cn;
        为( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    为( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        为( j = 0; j < total; j += blockSize )
        {
            整数 bsz = std::min(total - j, blockSize);
            整数 nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            count += nz;
            nz0 += nz;
            如果( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                如果 (partialSumIsInt) {
                    为( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } 否则 {
                    为( k = 0; k < cn; k++ )
                    {
                        s[k] += ((float*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                如果( blockSqSum )
                {
                    为( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
            如果( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    double scale = nz0 ? 1./nz0 : 0.;
    为( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    如果 (_mean.needed())
    {
        const double* sptr = s;
        double* dptr = mean_mat.ptr<double>();
        为( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    如果 (_sdv.needed())
    {
        const double* sptr = sq;
        double* dptr = stddev_mat.ptr<double>();
        为( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

static void writeStatResult(OutputArray _dst, 整数 cn, const Scalar& v)
{
    如果 (!_dst.needed())
        返回;
    如果 (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, true);
    Mat d = _dst.getMat();
    整数 dcn = (整数)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    double* p = d.ptr<double>();
    为 (整数 k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

void meanStdDev(InputArray _src, OutputArray _mean, OutputArray _sdv, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    如果 (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const 整数 cn = src.channels();
        const bool maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        如果 (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            const 整数 n = (整数)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<double> cnt(n);
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                为 (整数 i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    为 (整数 k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<double>(k);
                        sd[i][k] = si.at<double>(k);
                    }
                    cnt[i] = statCount(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            double total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            为 (整数 i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            如果 (total > 0)
                M *= 1./total;
            为 (整数 i = 0; i < n; i++)
                为 (整数 k = 0; k < cn; k++)
                {
                    double dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            为 (整数 k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            返回;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
