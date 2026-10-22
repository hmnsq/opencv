// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"
#شامل "stat_parallel.hpp"

#شامل "mean.simd.hpp"
#شامل "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


فضاینام cv {

// serial implementation, also used for the pieces of the parallel version
ایستا Scalar mean_(ثابت Mat& src, ثابت Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    اگر (src.empty())
        بازگشت Scalar::all(0.0);

    صحیح k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    اگر (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (صحیح)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    وگرنه
    {
        اگر (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح total = (صحیح)it.size, blockSize = total, partialBlockSize = 0;
    صحیح j, شمار_2 = 0;
    صحیح _buf[CV_CN_MAX];
    صحیح* buf = (صحیح*)&s[0];
    منطقی partialSumIsInt = depth < CV_32S;
    منطقی blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    اگر( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        برای( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( j = 0; j < total; j += blockSize )
        {
            صحیح bsz = std::min(total - j, blockSize);
            صحیح nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            شمار_2 += nz;
            nz0 += nz;
            اگر( blockSum && (شمار_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                اگر (partialSumIsInt) {
                    برای( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } وگرنه {
                    برای( k = 0; k < cn; k++ )
                    {
                        s[k] += ((اعشاری*)buf)[k];
                        buf[k] = 0;
                    }
                }
                شمار_2 = 0;
            }
            ptrs[0] += bsz*esz;
            اگر( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    بازگشت s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ایستا دوگانه stat_شمار(ثابت StatChunk& c)
{
    بازگشت c.mask.empty() ? (دوگانه)c.src.total() : (دوگانه)شمار_Non_Zero(c.mask);
}

Scalar mean(درون_داد_Array _src, درون_داد_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    اگر (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        بازگشت mean_(src, mask);

    ثابت صحیح n = (صحیح)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<دوگانه> cnt(n);
    parallel_for_(Range(0, n), [&](ثابت Range& r)
    {
        برای (صحیح i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_شمار(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    دوگانه total = 0;
    برای (صحیح i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    بازگشت s*(total > 0 ? 1./total : 0);
}

ایستا SumSqrFunc getSumSqrFunc(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ایستا منطقی ocl_meanStdDev( درون_داد_Array _src, OutputArray _mean, OutputArray _sdv, درون_داد_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    منطقی haveMask = _mask.kind() != درون_داد_Array_2::NONE;
    صحیح nz = haveMask ? -1 : (صحیح)_src.total();
    Scalar mean(0), stddev(0);
    ثابت صحیح cn = _src.channels();
    اگر (cn > 4)
        بازگشت نادرست;

    {
        صحیح type = _src.type(), depth = CV_MAT_DEPTH(type);
        منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        ثابت ocl::Device &defDev = ocl::Device::getDefault();
        صحیح groups = defDev.maxComputeUnits();
        اگر (defDev.isIntel())
        {
            ایستا ثابت صحیح subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        صحیح ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        صحیح wgs2_aligned = 1;
        وقتی (wgs2_aligned < (صحیح)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        اگر ( (!doubleSupport && depth == CV_64F) )
            بازگشت نادرست;

        نویسه cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], اندازه(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], اندازه(cvt[1])),
                             (صحیح)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        اگر (k.empty())
            بازگشت نادرست;

        صحیح dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        اگر (haveMask)
            k.args(srcarg, src.cols, (صحیح)src.total(), groups, dbarg, maskarg);
        وگرنه
            k.args(srcarg, src.cols, (صحیح)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        اگر(!k.run(1, &globalsize, &wgs, نادرست))
            بازگشت نادرست;

        تعریفنوع Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<صحیح>, ocl_part_sum<اعشاری>, ocl_part_sum<دوگانه> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        اگر (haveMask)
            nz = saturate_cast<صحیح>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    دوگانه total = nz != 0 ? 1.0 / nz : 0;
    صحیح k, j;
    برای (صحیح i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    برای( j = 0; j < 2; j++ )
    {
        ثابت دوگانه * ثابت sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        اگر( !_dst.needed() )
            ادامه;

        اگر( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, درست);
        Mat dst = _dst.getMat();
        صحیح dcn = (صحیح)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        دوگانه* dptr = dst.ptr<دوگانه>();
        برای( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        برای( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    بازگشت درست;
}
#endif

// serial implementation, also used for the pieces of the parallel version
ایستا تهی meanStdDev_(درون_داد_Array _src, OutputArray _mean, OutputArray _sdv, درون_داد_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    صحیح k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    اگر(_mean.needed())
    {
        اگر( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, درست);

        mean_mat = _mean.getMat();
        صحیح dcn = (صحیح)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        دوگانه* dptr = mean_mat.ptr<دوگانه>();
        برای(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    اگر (_sdv.needed())
    {
        اگر( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, درست);

        stddev_mat = _sdv.getMat();
        صحیح dcn = (صحیح)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        دوگانه* dptr = stddev_mat.ptr<دوگانه>();
        برای(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    اگر (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (صحیح)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<دوگانه>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<دوگانه>() : nullptr,
                 mask.data, 0);
    }
    وگرنه
    {
        اگر (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<دوگانه>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<دوگانه>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح total = (صحیح)it.size, blockSize = total, partialBlockSize = 0;
    صحیح j;
    int64_t شمار_2 = 0, nz0 = 0;
    دوگانه _buf[CV_CN_MAX*4];
    دوگانه *s = _buf, *sq = s + cn;
    صحیح *sbuf = (صحیح*)s, *sqbuf = (صحیح*)sq;
    منطقی partialSumIsInt = depth < CV_32S;
    منطقی blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    منطقی blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    برای( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    اگر( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (صحیح*)(sq + cn);
        اگر( blockSqSum )
            sqbuf = sbuf + cn;
        برای( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برای( j = 0; j < total; j += blockSize )
        {
            صحیح bsz = std::min(total - j, blockSize);
            صحیح nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            شمار_2 += nz;
            nz0 += nz;
            اگر( blockSum && (شمار_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                اگر (partialSumIsInt) {
                    برای( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } وگرنه {
                    برای( k = 0; k < cn; k++ )
                    {
                        s[k] += ((اعشاری*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                اگر( blockSqSum )
                {
                    برای( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                شمار_2 = 0;
            }
            ptrs[0] += bsz*esz;
            اگر( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    دوگانه scale = nz0 ? 1./nz0 : 0.;
    برای( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    اگر (_mean.needed())
    {
        ثابت دوگانه* sptr = s;
        دوگانه* dptr = mean_mat.ptr<دوگانه>();
        برای( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    اگر (_sdv.needed())
    {
        ثابت دوگانه* sptr = sq;
        دوگانه* dptr = stddev_mat.ptr<دوگانه>();
        برای( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

ایستا تهی writeStatResult(OutputArray _dst, صحیح cn, ثابت Scalar& v)
{
    اگر (!_dst.needed())
        بازگشت;
    اگر (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, درست);
    Mat d = _dst.getMat();
    صحیح dcn = (صحیح)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    دوگانه* p = d.ptr<دوگانه>();
    برای (صحیح k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

تهی meanStdDev(درون_داد_Array _src, OutputArray _mean, OutputArray _sdv, درون_داد_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    اگر (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ثابت صحیح cn = src.channels();
        ثابت منطقی maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        اگر (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            ثابت صحیح n = (صحیح)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<دوگانه> cnt(n);
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                برای (صحیح i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    برای (صحیح k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<دوگانه>(k);
                        sd[i][k] = si.at<دوگانه>(k);
                    }
                    cnt[i] = stat_شمار(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            دوگانه total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            برای (صحیح i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            اگر (total > 0)
                M *= 1./total;
            برای (صحیح i = 0; i < n; i++)
                برای (صحیح k = 0; k < cn; k++)
                {
                    دوگانه dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            برای (صحیح k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            بازگشت;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
