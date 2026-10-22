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
    གལ་སྲིད (src.empty())
        ལོག Scalar::all(0.0);

    ཧྲིལ k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    གལ་སྲིད (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (ཧྲིལ)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    དེ་མིན
    {
        གལ་སྲིད (src.dims <= 2)
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
    ཧྲིལ total = (ཧྲིལ)it.size, blockSize = total, partialBlockSize = 0;
    ཧྲིལ j, count = 0;
    ཧྲིལ _buf[CV_CN_MAX];
    ཧྲིལ* buf = (ཧྲིལ*)&s[0];
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    གལ་སྲིད( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        དོན་ལུ( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    དོན་ལུ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        དོན་ལུ( j = 0; j < total; j += blockSize )
        {
            ཧྲིལ bsz = std::min(total - j, blockSize);
            ཧྲིལ nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            count += nz;
            nz0 += nz;
            གལ་སྲིད( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                གལ་སྲིད (partialSumIsInt) {
                    དོན་ལུ( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } དེ་མིན {
                    དོན་ལུ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((float*)buf)[k];
                        buf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
            གལ་སྲིད( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    ལོག s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
static dviguu_na statCount(const StatChunk& c)
{
    ལོག c.mask.empty() ? (dviguu_na)c.src.total() : (dviguu_na)countNonZero(c.mask);
}

Scalar mean(InputArray _src, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    གལ་སྲིད (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        ལོག mean_(src, mask);

    const ཧྲིལ n = (ཧྲིལ)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<dviguu_na> cnt(n);
    parallel_for_(Range(0, n), [&](const Range& r)
    {
        དོན་ལུ (ཧྲིལ i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = statCount(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    dviguu_na total = 0;
    དོན་ལུ (ཧྲིལ i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    ལོག s*(total > 0 ? 1./total : 0);
}

static SumSqrFunc getSumSqrFunc(ཧྲིལ depth)
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
    ཧྲིལ nz = haveMask ? -1 : (ཧྲིལ)_src.total();
    Scalar mean(0), stddev(0);
    const ཧྲིལ cn = _src.channels();
    གལ་སྲིད (cn > 4)
        ལོག false;

    {
        ཧྲིལ type = _src.type(), depth = CV_MAT_DEPTH(type);
        bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        const ocl::Device &defDev = ocl::Device::getDefault();
        ཧྲིལ groups = defDev.maxComputeUnits();
        གལ་སྲིད (defDev.isIntel())
        {
            static const ཧྲིལ subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        ཧྲིལ ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        ཧྲིལ wgs2_aligned = 1;
        སྐབས (wgs2_aligned < (ཧྲིལ)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        གལ་སྲིད ( (!doubleSupport && depth == CV_64F) )
            ལོག false;

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
                             (ཧྲིལ)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        གལ་སྲིད (k.empty())
            ལོག false;

        ཧྲིལ dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        གལ་སྲིད (haveMask)
            k.args(srcarg, src.cols, (ཧྲིལ)src.total(), groups, dbarg, maskarg);
        དེ་མིན
            k.args(srcarg, src.cols, (ཧྲིལ)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        གལ་སྲིད(!k.run(1, &globalsize, &wgs, false))
            ལོག false;

        typedef Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<ཧྲིལ>, ocl_part_sum<float>, ocl_part_sum<dviguu_na> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        གལ་སྲིད (haveMask)
            nz = saturate_cast<ཧྲིལ>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    dviguu_na total = nz != 0 ? 1.0 / nz : 0;
    ཧྲིལ k, j;
    དོན་ལུ (ཧྲིལ i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    དོན་ལུ( j = 0; j < 2; j++ )
    {
        const dviguu_na * const sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        གལ་སྲིད( !_dst.needed() )
            continue;

        གལ་སྲིད( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, true);
        Mat dst = _dst.getMat();
        ཧྲིལ dcn = (ཧྲིལ)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        dviguu_na* dptr = dst.ptr<dviguu_na>();
        དོན་ལུ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        དོན་ལུ( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ལོག true;
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

    ཧྲིལ k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    གལ་སྲིད(_mean.needed())
    {
        གལ་སྲིད( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, true);

        mean_mat = _mean.getMat();
        ཧྲིལ dcn = (ཧྲིལ)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        dviguu_na* dptr = mean_mat.ptr<dviguu_na>();
        དོན་ལུ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    གལ་སྲིད (_sdv.needed())
    {
        གལ་སྲིད( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, true);

        stddev_mat = _sdv.getMat();
        ཧྲིལ dcn = (ཧྲིལ)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        dviguu_na* dptr = stddev_mat.ptr<dviguu_na>();
        དོན་ལུ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    གལ་སྲིད (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (ཧྲིལ)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<dviguu_na>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<dviguu_na>() : nullptr,
                 mask.data, 0);
    }
    དེ་མིན
    {
        གལ་སྲིད (src.dims <= 2)
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
    ཧྲིལ total = (ཧྲིལ)it.size, blockSize = total, partialBlockSize = 0;
    ཧྲིལ j;
    int64_t count = 0, nz0 = 0;
    dviguu_na _buf[CV_CN_MAX*4];
    dviguu_na *s = _buf, *sq = s + cn;
    ཧྲིལ *sbuf = (ཧྲིལ*)s, *sqbuf = (ཧྲིལ*)sq;
    bool partialSumIsInt = depth < CV_32S;
    bool blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    bool blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    དོན་ལུ( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    གལ་སྲིད( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (ཧྲིལ*)(sq + cn);
        གལ་སྲིད( blockSqSum )
            sqbuf = sbuf + cn;
        དོན་ལུ( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    དོན་ལུ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        དོན་ལུ( j = 0; j < total; j += blockSize )
        {
            ཧྲིལ bsz = std::min(total - j, blockSize);
            ཧྲིལ nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            count += nz;
            nz0 += nz;
            གལ་སྲིད( blockSum && (count + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                གལ་སྲིད (partialSumIsInt) {
                    དོན་ལུ( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } དེ་མིན {
                    དོན་ལུ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((float*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                གལ་སྲིད( blockSqSum )
                {
                    དོན་ལུ( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                count = 0;
            }
            ptrs[0] += bsz*esz;
            གལ་སྲིད( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    dviguu_na scale = nz0 ? 1./nz0 : 0.;
    དོན་ལུ( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    གལ་སྲིད (_mean.needed())
    {
        const dviguu_na* sptr = s;
        dviguu_na* dptr = mean_mat.ptr<dviguu_na>();
        དོན་ལུ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    གལ་སྲིད (_sdv.needed())
    {
        const dviguu_na* sptr = sq;
        dviguu_na* dptr = stddev_mat.ptr<dviguu_na>();
        དོན་ལུ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

static void writeStatResult(OutputArray _dst, ཧྲིལ cn, const Scalar& v)
{
    གལ་སྲིད (!_dst.needed())
        ལོག;
    གལ་སྲིད (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, true);
    Mat d = _dst.getMat();
    ཧྲིལ dcn = (ཧྲིལ)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    dviguu_na* p = d.ptr<dviguu_na>();
    དོན་ལུ (ཧྲིལ k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

void meanStdDev(InputArray _src, OutputArray _mean, OutputArray _sdv, InputArray _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    གལ་སྲིད (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        const ཧྲིལ cn = src.channels();
        const bool maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        གལ་སྲིད (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            const ཧྲིལ n = (ཧྲིལ)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<dviguu_na> cnt(n);
            parallel_for_(Range(0, n), [&](const Range& r)
            {
                དོན་ལུ (ཧྲིལ i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    དོན་ལུ (ཧྲིལ k = 0; k < cn; k++)
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
            དོན་ལུ (ཧྲིལ i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            གལ་སྲིད (total > 0)
                M *= 1./total;
            དོན་ལུ (ཧྲིལ i = 0; i < n; i++)
                དོན་ལུ (ཧྲིལ k = 0; k < cn; k++)
                {
                    dviguu_na dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            དོན་ལུ (ཧྲིལ k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            ལོག;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
