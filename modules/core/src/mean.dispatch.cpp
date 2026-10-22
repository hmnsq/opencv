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
    если (src.empty())
        вернуть Scalar::all(0.0);

    целое k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    если (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (целое)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    иначе
    {
        если (src.dims <= 2)
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
    целое total = (целое)it.size, blockSize = total, partialBlockSize = 0;
    целое j, count = 0;
    целое _buf[CV_CN_MAX];
    целое* buf = (целое*)&s[0];
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    если( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        для( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    для( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        для( j = 0; j < total; j += blockSize )
        {
            целое bsz = std::min(total - j, blockSize);
            целое nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            count += nz;
            nz0 += nz;
            если( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                если (partialSumIsInt) {
                    для( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } иначе {
                    для( k = 0; k < cn; k++ )
                    {
                        s[k] += ((float*)buf)[k];
                        buf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
            если( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    вернуть s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
static double statCount(const StatChunk& c)
{
    вернуть c.mask.empty() ? (double)c.src.total() : (double)countNonZero(c.mask);
}

Scalar mean(InputArray _src, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    если (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        вернуть mean_(src, mask);

    const целое n = (целое)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<double> cnt(n);
    parallel_for_(Range(0, n), [&](const Range& r)
    {
        для (целое i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = statCount(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    double total = 0;
    для (целое i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    вернуть s*(total > 0 ? 1./total : 0);
}

static SumSqrFunc getSumSqrFunc(целое depth)
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
    целое nz = haveMask ? -1 : (целое)_src.total();
    Scalar mean(0), stddev(0);
    const целое cn = _src.channels();
    если (cn > 4)
        вернуть false;

    {
        целое type = _src.type(), depth = CV_MAT_DEPTH(type);
        bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        const ocl::Device &defDev = ocl::Device::getDefault();
        целое groups = defDev.maxComputeUnits();
        если (defDev.isIntel())
        {
            static const целое subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        целое ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        целое wgs2_aligned = 1;
        пока (wgs2_aligned < (целое)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        если ( (!doubleSupport && depth == CV_64F) )
            вернуть false;

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
                             (целое)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        если (k.empty())
            вернуть false;

        целое dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        если (haveMask)
            k.args(srcarg, src.cols, (целое)src.total(), groups, dbarg, maskarg);
        иначе
            k.args(srcarg, src.cols, (целое)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        если(!k.run(1, &globalsize, &wgs, false))
            вернуть false;

        typedef Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<целое>, ocl_part_sum<float>, ocl_part_sum<double> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        если (haveMask)
            nz = saturate_cast<целое>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    double total = nz != 0 ? 1.0 / nz : 0;
    целое k, j;
    для (целое i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    для( j = 0; j < 2; j++ )
    {
        const double * const sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        если( !_dst.needed() )
            continue;

        если( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, true);
        Mat dst = _dst.getMat();
        целое dcn = (целое)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        double* dptr = dst.ptr<double>();
        для( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        для( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    вернуть true;
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

    целое k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    если(_mean.needed())
    {
        если( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, true);

        mean_mat = _mean.getMat();
        целое dcn = (целое)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        double* dptr = mean_mat.ptr<double>();
        для(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    если (_sdv.needed())
    {
        если( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, true);

        stddev_mat = _sdv.getMat();
        целое dcn = (целое)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        double* dptr = stddev_mat.ptr<double>();
        для(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    если (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (целое)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<double>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<double>() : nullptr,
                 mask.data, 0);
    }
    иначе
    {
        если (src.dims <= 2)
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
    целое total = (целое)it.size, blockSize = total, partialBlockSize = 0;
    целое j;
    int64_t count = 0, nz0 = 0;
    double _buf[CV_CN_MAX*4];
    double *s = _buf, *sq = s + cn;
    целое *sbuf = (целое*)s, *sqbuf = (целое*)sq;
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    bool blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    для( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    если( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (целое*)(sq + cn);
        если( blockSqSum )
            sqbuf = sbuf + cn;
        для( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    для( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        для( j = 0; j < total; j += blockSize )
        {
            целое bsz = std::min(total - j, blockSize);
            целое nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            count += nz;
            nz0 += nz;
            если( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                если (partialSumIsInt) {
                    для( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } иначе {
                    для( k = 0; k < cn; k++ )
                    {
                        s[k] += ((float*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                если( blockSqSum )
                {
                    для( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
            если( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    double scale = nz0 ? 1./nz0 : 0.;
    для( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    если (_mean.needed())
    {
        const double* sptr = s;
        double* dptr = mean_mat.ptr<double>();
        для( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    если (_sdv.needed())
    {
        const double* sptr = sq;
        double* dptr = stddev_mat.ptr<double>();
        для( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

static void writeStatResult(OutputArray _dst, целое cn, const Scalar& v)
{
    если (!_dst.needed())
        вернуть;
    если (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, true);
    Mat d = _dst.getMat();
    целое dcn = (целое)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    double* p = d.ptr<double>();
    для (целое k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

void meanStdDev(InputArray _src, OutputArray _mean, OutputArray _sdv, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    если (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const целое cn = src.channels();
        const bool maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        если (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            const целое n = (целое)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<double> cnt(n);
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                для (целое i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    для (целое k = 0; k < cn; k++)
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
            для (целое i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            если (total > 0)
                M *= 1./total;
            для (целое i = 0; i < n; i++)
                для (целое k = 0; k < cn; k++)
                {
                    double dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            для (целое k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            вернуть;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
