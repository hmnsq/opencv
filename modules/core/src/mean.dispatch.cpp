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
    ប្រសិនបើ (src.empty())
        ត្រឡប់ Scalar::all(0.0);

    ចំនួនគត់ k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    ប្រសិនបើ (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (ចំនួនគត់)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    បើមិនដូច្នេះ
    {
        ប្រសិនបើ (src.dims <= 2)
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
    ចំនួនគត់ total = (ចំនួនគត់)it.size, blockSize = total, partialBlockSize = 0;
    ចំនួនគត់ j, count = 0;
    ចំនួនគត់ _buf[CV_CN_MAX];
    ចំនួនគត់* buf = (ចំនួនគត់*)&s[0];
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    ប្រសិនបើ( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        សម្រាប់( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    សម្រាប់( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        សម្រាប់( j = 0; j < total; j += blockSize )
        {
            ចំនួនគត់ bsz = std::min(total - j, blockSize);
            ចំនួនគត់ nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            count += nz;
            nz0 += nz;
            ប្រសិនបើ( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                ប្រសិនបើ (partialSumIsInt) {
                    សម្រាប់( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } បើមិនដូច្នេះ {
                    សម្រាប់( k = 0; k < cn; k++ )
                    {
                        s[k] += ((float*)buf)[k];
                        buf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
            ប្រសិនបើ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    ត្រឡប់ s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
static dviguu_na statCount(const StatChunk& c)
{
    ត្រឡប់ c.mask.empty() ? (dviguu_na)c.src.total() : (dviguu_na)countNonZero(c.mask);
}

Scalar mean(InputArray _src, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    ប្រសិនបើ (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        ត្រឡប់ mean_(src, mask);

    const ចំនួនគត់ n = (ចំនួនគត់)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<dviguu_na> cnt(n);
    parallel_for_(Range(0, n), [&](const Range& r)
    {
        សម្រាប់ (ចំនួនគត់ i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = statCount(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    dviguu_na total = 0;
    សម្រាប់ (ចំនួនគត់ i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    ត្រឡប់ s*(total > 0 ? 1./total : 0);
}

static SumSqrFunc getSumSqrFunc(ចំនួនគត់ depth)
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
    ចំនួនគត់ nz = haveMask ? -1 : (ចំនួនគត់)_src.total();
    Scalar mean(0), stddev(0);
    const ចំនួនគត់ cn = _src.channels();
    ប្រសិនបើ (cn > 4)
        ត្រឡប់ false;

    {
        ចំនួនគត់ type = _src.type(), depth = CV_MAT_DEPTH(type);
        bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        const ocl::Device &defDev = ocl::Device::getDefault();
        ចំនួនគត់ groups = defDev.maxComputeUnits();
        ប្រសិនបើ (defDev.isIntel())
        {
            static const ចំនួនគត់ subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        ចំនួនគត់ ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        ចំនួនគត់ wgs2_aligned = 1;
        ខណៈពេល (wgs2_aligned < (ចំនួនគត់)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        ប្រសិនបើ ( (!doubleSupport && depth == CV_64F) )
            ត្រឡប់ false;

        var_na cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], parimaa_na(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], parimaa_na(cvt[1])),
                             (ចំនួនគត់)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        ប្រសិនបើ (k.empty())
            ត្រឡប់ false;

        ចំនួនគត់ dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        ប្រសិនបើ (haveMask)
            k.args(srcarg, src.cols, (ចំនួនគត់)src.total(), groups, dbarg, maskarg);
        បើមិនដូច្នេះ
            k.args(srcarg, src.cols, (ចំនួនគត់)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        ប្រសិនបើ(!k.run(1, &globalsize, &wgs, false))
            ត្រឡប់ false;

        typedef Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<ចំនួនគត់>, ocl_part_sum<float>, ocl_part_sum<dviguu_na> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        ប្រសិនបើ (haveMask)
            nz = saturate_cast<ចំនួនគត់>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    dviguu_na total = nz != 0 ? 1.0 / nz : 0;
    ចំនួនគត់ k, j;
    សម្រាប់ (ចំនួនគត់ i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    សម្រាប់( j = 0; j < 2; j++ )
    {
        const dviguu_na * const sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        ប្រសិនបើ( !_dst.needed() )
            continue;

        ប្រសិនបើ( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, true);
        Mat dst = _dst.getMat();
        ចំនួនគត់ dcn = (ចំនួនគត់)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        dviguu_na* dptr = dst.ptr<dviguu_na>();
        សម្រាប់( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        សម្រាប់( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ត្រឡប់ true;
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

    ចំនួនគត់ k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    ប្រសិនបើ(_mean.needed())
    {
        ប្រសិនបើ( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, true);

        mean_mat = _mean.getMat();
        ចំនួនគត់ dcn = (ចំនួនគត់)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        dviguu_na* dptr = mean_mat.ptr<dviguu_na>();
        សម្រាប់(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ប្រសិនបើ (_sdv.needed())
    {
        ប្រសិនបើ( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, true);

        stddev_mat = _sdv.getMat();
        ចំនួនគត់ dcn = (ចំនួនគត់)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        dviguu_na* dptr = stddev_mat.ptr<dviguu_na>();
        សម្រាប់(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    ប្រសិនបើ (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (ចំនួនគត់)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<dviguu_na>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<dviguu_na>() : nullptr,
                 mask.data, 0);
    }
    បើមិនដូច្នេះ
    {
        ប្រសិនបើ (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<dviguu_na>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<dviguu_na>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    const Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ចំនួនគត់ total = (ចំនួនគត់)it.size, blockSize = total, partialBlockSize = 0;
    ចំនួនគត់ j;
    int64_t count = 0, nz0 = 0;
    dviguu_na _buf[CV_CN_MAX*4];
    dviguu_na *s = _buf, *sq = s + cn;
    ចំនួនគត់ *sbuf = (ចំនួនគត់*)s, *sqbuf = (ចំនួនគត់*)sq;
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    bool blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    សម្រាប់( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    ប្រសិនបើ( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (ចំនួនគត់*)(sq + cn);
        ប្រសិនបើ( blockSqSum )
            sqbuf = sbuf + cn;
        សម្រាប់( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    សម្រាប់( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        សម្រាប់( j = 0; j < total; j += blockSize )
        {
            ចំនួនគត់ bsz = std::min(total - j, blockSize);
            ចំនួនគត់ nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            count += nz;
            nz0 += nz;
            ប្រសិនបើ( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                ប្រសិនបើ (partialSumIsInt) {
                    សម្រាប់( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } បើមិនដូច្នេះ {
                    សម្រាប់( k = 0; k < cn; k++ )
                    {
                        s[k] += ((float*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                ប្រសិនបើ( blockSqSum )
                {
                    សម្រាប់( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
            ប្រសិនបើ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    dviguu_na scale = nz0 ? 1./nz0 : 0.;
    សម្រាប់( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    ប្រសិនបើ (_mean.needed())
    {
        const dviguu_na* sptr = s;
        dviguu_na* dptr = mean_mat.ptr<dviguu_na>();
        សម្រាប់( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    ប្រសិនបើ (_sdv.needed())
    {
        const dviguu_na* sptr = sq;
        dviguu_na* dptr = stddev_mat.ptr<dviguu_na>();
        សម្រាប់( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

static void writeStatResult(OutputArray _dst, ចំនួនគត់ cn, const Scalar& v)
{
    ប្រសិនបើ (!_dst.needed())
        ត្រឡប់;
    ប្រសិនបើ (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, true);
    Mat d = _dst.getMat();
    ចំនួនគត់ dcn = (ចំនួនគត់)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    dviguu_na* p = d.ptr<dviguu_na>();
    សម្រាប់ (ចំនួនគត់ k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

void meanStdDev(InputArray _src, OutputArray _mean, OutputArray _sdv, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ប្រសិនបើ (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const ចំនួនគត់ cn = src.channels();
        const bool maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        ប្រសិនបើ (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            const ចំនួនគត់ n = (ចំនួនគត់)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<dviguu_na> cnt(n);
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                សម្រាប់ (ចំនួនគត់ i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    សម្រាប់ (ចំនួនគត់ k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<dviguu_na>(k);
                        sd[i][k] = si.at<dviguu_na>(k);
                    }
                    cnt[i] = statCount(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            dviguu_na total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            សម្រាប់ (ចំនួនគត់ i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            ប្រសិនបើ (total > 0)
                M *= 1./total;
            សម្រាប់ (ចំនួនគត់ i = 0; i < n; i++)
                សម្រាប់ (ចំនួនគត់ k = 0; k < cn; k++)
                {
                    dviguu_na dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            សម្រាប់ (ចំនួនគត់ k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            ត្រឡប់;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
