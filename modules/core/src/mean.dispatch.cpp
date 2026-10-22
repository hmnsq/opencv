// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "stat.hpp"
#அந்தர்பாவ "stat_parallel.hpp"

#அந்தர்பாவ "mean.simd.hpp"
#அந்தர்பாவ "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


நாமாகாஶ cv {

// serial implementation, also used for the pieces of the parallel version
நிலையான Scalar mean_(மாறிலி Mat& src, மாறிலி Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    எனில் (src.empty())
        திரும்பு Scalar::all(0.0);

    முழு k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    எனில் (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (முழு)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    இல்லையெனில்
    {
        எனில் (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    மாறிலி Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு total = (முழு)it.size, blockSize = total, partialBlockSize = 0;
    முழு j, மொத்தம்_2 = 0;
    முழு _buf[CV_CN_MAX];
    முழு* buf = (முழு*)&s[0];
    தருக்க partialSumIsInt = depth < CV_32S;
    தருக்க blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    எனில்( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        க்கு( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( j = 0; j < total; j += blockSize )
        {
            முழு bsz = std::min(total - j, blockSize);
            முழு nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            மொத்தம்_2 += nz;
            nz0 += nz;
            எனில்( blockSum && (மொத்தம்_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                எனில் (partialSumIsInt) {
                    க்கு( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } இல்லையெனில் {
                    க்கு( k = 0; k < cn; k++ )
                    {
                        s[k] += ((தசம*)buf)[k];
                        buf[k] = 0;
                    }
                }
                மொத்தம்_2 = 0;
            }
            ptrs[0] += bsz*esz;
            எனில்( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    திரும்பு s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
நிலையான இரட்டை stat_மொத்தம்(மாறிலி StatChunk& c)
{
    திரும்பு c.mask.empty() ? (இரட்டை)c.src.total() : (இரட்டை)மொத்தம்_Non_Zero(c.mask);
}

Scalar mean(உள்ளீடு_Array _src, உள்ளீடு_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    எனில் (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        திரும்பு mean_(src, mask);

    மாறிலி முழு n = (முழு)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<இரட்டை> cnt(n);
    parallel_for_(Range(0, n), [&](மாறிலி Range& r)
    {
        க்கு (முழு i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_மொத்தம்(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    இரட்டை total = 0;
    க்கு (முழு i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    திரும்பு s*(total > 0 ? 1./total : 0);
}

நிலையான SumSqrFunc getSumSqrFunc(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
நிலையான தருக்க ocl_meanStdDev( உள்ளீடு_Array _src, OutputArray _mean, OutputArray _sdv, உள்ளீடு_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    தருக்க haveMask = _mask.kind() != உள்ளீடு_Array_2::NONE;
    முழு nz = haveMask ? -1 : (முழு)_src.total();
    Scalar mean(0), stddev(0);
    மாறிலி முழு cn = _src.channels();
    எனில் (cn > 4)
        திரும்பு பொய்;

    {
        முழு type = _src.type(), depth = CV_MAT_DEPTH(type);
        தருக்க doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        மாறிலி ocl::Device &defDev = ocl::Device::getDefault();
        முழு groups = defDev.maxComputeUnits();
        எனில் (defDev.isIntel())
        {
            நிலையான மாறிலி முழு subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        முழு ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        முழு wgs2_aligned = 1;
        போது (wgs2_aligned < (முழு)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        எனில் ( (!doubleSupport && depth == CV_64F) )
            திரும்பு பொய்;

        எழுத்து cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], அளவு(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], அளவு(cvt[1])),
                             (முழு)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        எனில் (k.empty())
            திரும்பு பொய்;

        முழு dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        எனில் (haveMask)
            k.args(srcarg, src.cols, (முழு)src.total(), groups, dbarg, maskarg);
        இல்லையெனில்
            k.args(srcarg, src.cols, (முழு)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        எனில்(!k.run(1, &globalsize, &wgs, பொய்))
            திரும்பு பொய்;

        வகைவரை Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<முழு>, ocl_part_sum<தசம>, ocl_part_sum<இரட்டை> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        எனில் (haveMask)
            nz = saturate_cast<முழு>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    இரட்டை total = nz != 0 ? 1.0 / nz : 0;
    முழு k, j;
    க்கு (முழு i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    க்கு( j = 0; j < 2; j++ )
    {
        மாறிலி இரட்டை * மாறிலி sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        எனில்( !_dst.needed() )
            தொடர்;

        எனில்( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, உண்மை);
        Mat dst = _dst.getMat();
        முழு dcn = (முழு)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        இரட்டை* dptr = dst.ptr<இரட்டை>();
        க்கு( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        க்கு( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    திரும்பு உண்மை;
}
#endif

// serial implementation, also used for the pieces of the parallel version
நிலையான வெறுமை meanStdDev_(உள்ளீடு_Array _src, OutputArray _mean, OutputArray _sdv, உள்ளீடு_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    முழு k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    எனில்(_mean.needed())
    {
        எனில்( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, உண்மை);

        mean_mat = _mean.getMat();
        முழு dcn = (முழு)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        இரட்டை* dptr = mean_mat.ptr<இரட்டை>();
        க்கு(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    எனில் (_sdv.needed())
    {
        எனில்( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, உண்மை);

        stddev_mat = _sdv.getMat();
        முழு dcn = (முழு)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        இரட்டை* dptr = stddev_mat.ptr<இரட்டை>();
        க்கு(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    எனில் (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (முழு)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<இரட்டை>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<இரட்டை>() : nullptr,
                 mask.data, 0);
    }
    இல்லையெனில்
    {
        எனில் (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<இரட்டை>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<இரட்டை>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    மாறிலி Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு total = (முழு)it.size, blockSize = total, partialBlockSize = 0;
    முழு j;
    int64_t மொத்தம்_2 = 0, nz0 = 0;
    இரட்டை _buf[CV_CN_MAX*4];
    இரட்டை *s = _buf, *sq = s + cn;
    முழு *sbuf = (முழு*)s, *sqbuf = (முழு*)sq;
    தருக்க partialSumIsInt = depth < CV_32S;
    தருக்க blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    தருக்க blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    க்கு( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    எனில்( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (முழு*)(sq + cn);
        எனில்( blockSqSum )
            sqbuf = sbuf + cn;
        க்கு( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( j = 0; j < total; j += blockSize )
        {
            முழு bsz = std::min(total - j, blockSize);
            முழு nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            மொத்தம்_2 += nz;
            nz0 += nz;
            எனில்( blockSum && (மொத்தம்_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                எனில் (partialSumIsInt) {
                    க்கு( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } இல்லையெனில் {
                    க்கு( k = 0; k < cn; k++ )
                    {
                        s[k] += ((தசம*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                எனில்( blockSqSum )
                {
                    க்கு( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                மொத்தம்_2 = 0;
            }
            ptrs[0] += bsz*esz;
            எனில்( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    இரட்டை scale = nz0 ? 1./nz0 : 0.;
    க்கு( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    எனில் (_mean.needed())
    {
        மாறிலி இரட்டை* sptr = s;
        இரட்டை* dptr = mean_mat.ptr<இரட்டை>();
        க்கு( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    எனில் (_sdv.needed())
    {
        மாறிலி இரட்டை* sptr = sq;
        இரட்டை* dptr = stddev_mat.ptr<இரட்டை>();
        க்கு( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

நிலையான வெறுமை writeStatResult(OutputArray _dst, முழு cn, மாறிலி Scalar& v)
{
    எனில் (!_dst.needed())
        திரும்பு;
    எனில் (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, உண்மை);
    Mat d = _dst.getMat();
    முழு dcn = (முழு)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    இரட்டை* p = d.ptr<இரட்டை>();
    க்கு (முழு k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

வெறுமை meanStdDev(உள்ளீடு_Array _src, OutputArray _mean, OutputArray _sdv, உள்ளீடு_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    எனில் (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        மாறிலி முழு cn = src.channels();
        மாறிலி தருக்க maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        எனில் (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            மாறிலி முழு n = (முழு)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<இரட்டை> cnt(n);
            parallel_for_(Range(0, n), [&](மாறிலி Range& r)
            {
                க்கு (முழு i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    க்கு (முழு k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<இரட்டை>(k);
                        sd[i][k] = si.at<இரட்டை>(k);
                    }
                    cnt[i] = stat_மொத்தம்(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            இரட்டை total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            க்கு (முழு i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            எனில் (total > 0)
                M *= 1./total;
            க்கு (முழு i = 0; i < n; i++)
                க்கு (முழு k = 0; k < cn; k++)
                {
                    இரட்டை dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            க்கு (முழு k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            திரும்பு;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
