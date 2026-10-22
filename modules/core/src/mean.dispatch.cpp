// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"
#ضمن "stat_parallel.hpp"

#ضمن "mean.simd.hpp"
#ضمن "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


نطاق cv {

// serial implementation, also used for the pieces of the parallel version
ساكن Scalar mean_(ثابت Mat& src, ثابت Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    إذا (src.empty())
        أرجع Scalar::all(0.0);

    صحيح k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    إذا (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (صحيح)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
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
    صحيح total = (صحيح)it.size, blockSize = total, partialBlockSize = 0;
    صحيح j, کيراءن_2 = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&s[0];
    منطقي partialSumIsInt = depth < CV_32S;
    منطقي blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    إذا( blockSum )
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
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            کيراءن_2 += nz;
            nz0 += nz;
            إذا( blockSum && (کيراءن_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                إذا (partialSumIsInt) {
                    لكل( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } وإلا {
                    لكل( k = 0; k < cn; k++ )
                    {
                        s[k] += ((عائم*)buf)[k];
                        buf[k] = 0;
                    }
                }
                کيراءن_2 = 0;
            }
            ptrs[0] += bsz*esz;
            إذا( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    أرجع s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ساكن مزدوج stat_کيراءن(ثابت StatChunk& c)
{
    أرجع c.mask.empty() ? (مزدوج)c.src.total() : (مزدوج)کيراءن_Non_Zero(c.mask);
}

Scalar mean(ماسوقن_Array _src, ماسوقن_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    إذا (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        أرجع mean_(src, mask);

    ثابت صحيح n = (صحيح)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<مزدوج> cnt(n);
    parallel_for_(Range(0, n), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_کيراءن(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    مزدوج total = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    أرجع s*(total > 0 ? 1./total : 0);
}

ساكن SumSqrFunc getSumSqrFunc(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ساكن منطقي ocl_meanStdDev( ماسوقن_Array _src, OutputArray _mean, OutputArray _sdv, ماسوقن_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    منطقي haveMask = _mask.kind() != ماسوقن_Array_2::NONE;
    صحيح nz = haveMask ? -1 : (صحيح)_src.total();
    Scalar mean(0), stddev(0);
    ثابت صحيح cn = _src.channels();
    إذا (cn > 4)
        أرجع خطأ;

    {
        صحيح type = _src.type(), depth = CV_MAT_DEPTH(type);
        منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        ثابت ocl::Device &defDev = ocl::Device::getDefault();
        صحيح groups = defDev.maxComputeUnits();
        إذا (defDev.isIntel())
        {
            ساكن ثابت صحيح subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        صحيح ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        صحيح wgs2_aligned = 1;
        طالما (wgs2_aligned < (صحيح)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        إذا ( (!doubleSupport && depth == CV_64F) )
            أرجع خطأ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], حجم(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], حجم(cvt[1])),
                             (صحيح)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        إذا (k.empty())
            أرجع خطأ;

        صحيح dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        إذا (haveMask)
            k.args(srcarg, src.cols, (صحيح)src.total(), groups, dbarg, maskarg);
        وإلا
            k.args(srcarg, src.cols, (صحيح)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        إذا(!k.run(1, &globalsize, &wgs, خطأ))
            أرجع خطأ;

        تعريفنوع Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<صحيح>, ocl_part_sum<عائم>, ocl_part_sum<مزدوج> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        إذا (haveMask)
            nz = saturate_cast<صحيح>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    مزدوج total = nz != 0 ? 1.0 / nz : 0;
    صحيح k, j;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    لكل( j = 0; j < 2; j++ )
    {
        ثابت مزدوج * ثابت sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        إذا( !_dst.needed() )
            استمر;

        إذا( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, true);
        Mat dst = _dst.getMat();
        صحيح dcn = (صحيح)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        مزدوج* dptr = dst.ptr<مزدوج>();
        لكل( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        لكل( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    أرجع true;
}
#endif

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ meanStdDev_(ماسوقن_Array _src, OutputArray _mean, OutputArray _sdv, ماسوقن_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    صحيح k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    إذا(_mean.needed())
    {
        إذا( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, true);

        mean_mat = _mean.getMat();
        صحيح dcn = (صحيح)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        مزدوج* dptr = mean_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    إذا (_sdv.needed())
    {
        إذا( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, true);

        stddev_mat = _sdv.getMat();
        صحيح dcn = (صحيح)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        مزدوج* dptr = stddev_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    إذا (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (صحيح)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<مزدوج>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<مزدوج>() : nullptr,
                 mask.data, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<مزدوج>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<مزدوج>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح total = (صحيح)it.size, blockSize = total, partialBlockSize = 0;
    صحيح j;
    int64_t کيراءن_2 = 0, nz0 = 0;
    مزدوج _buf[CV_CN_MAX*4];
    مزدوج *s = _buf, *sq = s + cn;
    صحيح *sbuf = (صحيح*)s, *sqbuf = (صحيح*)sq;
    منطقي partialSumIsInt = depth < CV_32S;
    منطقي blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    منطقي blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    لكل( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    إذا( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (صحيح*)(sq + cn);
        إذا( blockSqSum )
            sqbuf = sbuf + cn;
        لكل( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < total; j += blockSize )
        {
            صحيح bsz = std::min(total - j, blockSize);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            کيراءن_2 += nz;
            nz0 += nz;
            إذا( blockSum && (کيراءن_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                إذا (partialSumIsInt) {
                    لكل( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } وإلا {
                    لكل( k = 0; k < cn; k++ )
                    {
                        s[k] += ((عائم*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                إذا( blockSqSum )
                {
                    لكل( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                کيراءن_2 = 0;
            }
            ptrs[0] += bsz*esz;
            إذا( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    مزدوج scale = nz0 ? 1./nz0 : 0.;
    لكل( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    إذا (_mean.needed())
    {
        ثابت مزدوج* sptr = s;
        مزدوج* dptr = mean_mat.ptr<مزدوج>();
        لكل( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    إذا (_sdv.needed())
    {
        ثابت مزدوج* sptr = sq;
        مزدوج* dptr = stddev_mat.ptr<مزدوج>();
        لكل( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

ساكن فراغ writeStatResult(OutputArray _dst, صحيح cn, ثابت Scalar& v)
{
    إذا (!_dst.needed())
        أرجع;
    إذا (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, true);
    Mat d = _dst.getMat();
    صحيح dcn = (صحيح)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    مزدوج* p = d.ptr<مزدوج>();
    لكل (صحيح k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

فراغ meanStdDev(ماسوقن_Array _src, OutputArray _mean, OutputArray _sdv, ماسوقن_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        إذا (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<مزدوج> cnt(n);
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    لكل (صحيح k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<مزدوج>(k);
                        sd[i][k] = si.at<مزدوج>(k);
                    }
                    cnt[i] = stat_کيراءن(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            مزدوج total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            لكل (صحيح i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            إذا (total > 0)
                M *= 1./total;
            لكل (صحيح i = 0; i < n; i++)
                لكل (صحيح k = 0; k < cn; k++)
                {
                    مزدوج dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            لكل (صحيح k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            أرجع;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
