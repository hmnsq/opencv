// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "stat.hpp"
#অন্তর্ভাব "stat_parallel.hpp"

#অন্তর্ভাব "mean.simd.hpp"
#অন্তর্ভাব "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


নামাকাশ cv {

// serial implementation, also used for the pieces of the parallel version
স্থির Scalar mean_(ধ্রুব Mat& src, ধ্রুব Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    যদি (src.empty())
        প্রত্যাগম Scalar::all(0.0);

    পূর্ণাঙ্ক k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    যদি (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (পূর্ণাঙ্ক)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    অন্যথা
    {
        যদি (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    ধ্রুব Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size, blockSize = total, partialBlockSize = 0;
    পূর্ণাঙ্ক j, গুনতি_2 = 0;
    পূর্ণাঙ্ক _buf[CV_CN_MAX];
    পূর্ণাঙ্ক* buf = (পূর্ণাঙ্ক*)&s[0];
    তার্কিক partialSumIsInt = depth < CV_32S;
    তার্কিক blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    যদি( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        প্রতি( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        প্রতি( j = 0; j < total; j += blockSize )
        {
            পূর্ণাঙ্ক bsz = std::min(total - j, blockSize);
            পূর্ণাঙ্ক nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            গুনতি_2 += nz;
            nz0 += nz;
            যদি( blockSum && (গুনতি_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                যদি (partialSumIsInt) {
                    প্রতি( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } অন্যথা {
                    প্রতি( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ভিন্ন*)buf)[k];
                        buf[k] = 0;
                    }
                }
                গুনতি_2 = 0;
            }
            ptrs[0] += bsz*esz;
            যদি( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    প্রত্যাগম s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
স্থির দ্বিগুণ stat_গুনতি(ধ্রুব StatChunk& c)
{
    প্রত্যাগম c.mask.empty() ? (দ্বিগুণ)c.src.total() : (দ্বিগুণ)গুনতি_Non_Zero(c.mask);
}

Scalar mean(প্রবেশ_Array _src, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    যদি (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        প্রত্যাগম mean_(src, mask);

    ধ্রুব পূর্ণাঙ্ক n = (পূর্ণাঙ্ক)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<দ্বিগুণ> cnt(n);
    parallel_for_(Range(0, n), [&](ধ্রুব Range& r)
    {
        প্রতি (পূর্ণাঙ্ক i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_গুনতি(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    দ্বিগুণ total = 0;
    প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    প্রত্যাগম s*(total > 0 ? 1./total : 0);
}

স্থির SumSqrFunc getSumSqrFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
স্থির তার্কিক ocl_meanStdDev( প্রবেশ_Array _src, OutputArray _mean, OutputArray _sdv, প্রবেশ_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    তার্কিক haveMask = _mask.kind() != প্রবেশ_Array_2::NONE;
    পূর্ণাঙ্ক nz = haveMask ? -1 : (পূর্ণাঙ্ক)_src.total();
    Scalar mean(0), stddev(0);
    ধ্রুব পূর্ণাঙ্ক cn = _src.channels();
    যদি (cn > 4)
        প্রত্যাগম অসত্য;

    {
        পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type);
        তার্কিক doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        ধ্রুব ocl::Device &defDev = ocl::Device::getDefault();
        পূর্ণাঙ্ক groups = defDev.maxComputeUnits();
        যদি (defDev.isIntel())
        {
            স্থির ধ্রুব পূর্ণাঙ্ক subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        পূর্ণাঙ্ক ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        পূর্ণাঙ্ক wgs2_aligned = 1;
        যাবত্ (wgs2_aligned < (পূর্ণাঙ্ক)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        যদি ( (!doubleSupport && depth == CV_64F) )
            প্রত্যাগম অসত্য;

        বর্ণ cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], পরিমাণ(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], পরিমাণ(cvt[1])),
                             (পূর্ণাঙ্ক)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        যদি (k.empty())
            প্রত্যাগম অসত্য;

        পূর্ণাঙ্ক dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        যদি (haveMask)
            k.args(srcarg, src.cols, (পূর্ণাঙ্ক)src.total(), groups, dbarg, maskarg);
        অন্যথা
            k.args(srcarg, src.cols, (পূর্ণাঙ্ক)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        যদি(!k.run(1, &globalsize, &wgs, অসত্য))
            প্রত্যাগম অসত্য;

        প্রকারনির্দেশ Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<পূর্ণাঙ্ক>, ocl_part_sum<ভিন্ন>, ocl_part_sum<দ্বিগুণ> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        যদি (haveMask)
            nz = saturate_cast<পূর্ণাঙ্ক>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    দ্বিগুণ total = nz != 0 ? 1.0 / nz : 0;
    পূর্ণাঙ্ক k, j;
    প্রতি (পূর্ণাঙ্ক i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    প্রতি( j = 0; j < 2; j++ )
    {
        ধ্রুব দ্বিগুণ * ধ্রুব sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        যদি( !_dst.needed() )
            অনুবর্ত;

        যদি( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, সত্য);
        Mat dst = _dst.getMat();
        পূর্ণাঙ্ক dcn = (পূর্ণাঙ্ক)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        দ্বিগুণ* dptr = dst.ptr<দ্বিগুণ>();
        প্রতি( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        প্রতি( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    প্রত্যাগম সত্য;
}
#endif

// serial implementation, also used for the pieces of the parallel version
স্থির শূন্য meanStdDev_(প্রবেশ_Array _src, OutputArray _mean, OutputArray _sdv, প্রবেশ_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    পূর্ণাঙ্ক k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    যদি(_mean.needed())
    {
        যদি( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, সত্য);

        mean_mat = _mean.getMat();
        পূর্ণাঙ্ক dcn = (পূর্ণাঙ্ক)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        দ্বিগুণ* dptr = mean_mat.ptr<দ্বিগুণ>();
        প্রতি(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    যদি (_sdv.needed())
    {
        যদি( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, সত্য);

        stddev_mat = _sdv.getMat();
        পূর্ণাঙ্ক dcn = (পূর্ণাঙ্ক)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        দ্বিগুণ* dptr = stddev_mat.ptr<দ্বিগুণ>();
        প্রতি(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    যদি (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (পূর্ণাঙ্ক)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<দ্বিগুণ>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<দ্বিগুণ>() : nullptr,
                 mask.data, 0);
    }
    অন্যথা
    {
        যদি (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<দ্বিগুণ>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<দ্বিগুণ>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    ধ্রুব Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size, blockSize = total, partialBlockSize = 0;
    পূর্ণাঙ্ক j;
    int64_t গুনতি_2 = 0, nz0 = 0;
    দ্বিগুণ _buf[CV_CN_MAX*4];
    দ্বিগুণ *s = _buf, *sq = s + cn;
    পূর্ণাঙ্ক *sbuf = (পূর্ণাঙ্ক*)s, *sqbuf = (পূর্ণাঙ্ক*)sq;
    তার্কিক partialSumIsInt = depth < CV_32S;
    তার্কিক blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    তার্কিক blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    প্রতি( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    যদি( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (পূর্ণাঙ্ক*)(sq + cn);
        যদি( blockSqSum )
            sqbuf = sbuf + cn;
        প্রতি( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        প্রতি( j = 0; j < total; j += blockSize )
        {
            পূর্ণাঙ্ক bsz = std::min(total - j, blockSize);
            পূর্ণাঙ্ক nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            গুনতি_2 += nz;
            nz0 += nz;
            যদি( blockSum && (গুনতি_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                যদি (partialSumIsInt) {
                    প্রতি( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } অন্যথা {
                    প্রতি( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ভিন্ন*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                যদি( blockSqSum )
                {
                    প্রতি( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                গুনতি_2 = 0;
            }
            ptrs[0] += bsz*esz;
            যদি( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    দ্বিগুণ scale = nz0 ? 1./nz0 : 0.;
    প্রতি( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    যদি (_mean.needed())
    {
        ধ্রুব দ্বিগুণ* sptr = s;
        দ্বিগুণ* dptr = mean_mat.ptr<দ্বিগুণ>();
        প্রতি( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    যদি (_sdv.needed())
    {
        ধ্রুব দ্বিগুণ* sptr = sq;
        দ্বিগুণ* dptr = stddev_mat.ptr<দ্বিগুণ>();
        প্রতি( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

স্থির শূন্য writeStatResult(OutputArray _dst, পূর্ণাঙ্ক cn, ধ্রুব Scalar& v)
{
    যদি (!_dst.needed())
        প্রত্যাগম;
    যদি (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, সত্য);
    Mat d = _dst.getMat();
    পূর্ণাঙ্ক dcn = (পূর্ণাঙ্ক)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    দ্বিগুণ* p = d.ptr<দ্বিগুণ>();
    প্রতি (পূর্ণাঙ্ক k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

শূন্য meanStdDev(প্রবেশ_Array _src, OutputArray _mean, OutputArray _sdv, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    যদি (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ধ্রুব পূর্ণাঙ্ক cn = src.channels();
        ধ্রুব তার্কিক maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        যদি (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            ধ্রুব পূর্ণাঙ্ক n = (পূর্ণাঙ্ক)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<দ্বিগুণ> cnt(n);
            parallel_for_(Range(0, n), [&](ধ্রুব Range& r)
            {
                প্রতি (পূর্ণাঙ্ক i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    প্রতি (পূর্ণাঙ্ক k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<দ্বিগুণ>(k);
                        sd[i][k] = si.at<দ্বিগুণ>(k);
                    }
                    cnt[i] = stat_গুনতি(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            দ্বিগুণ total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            যদি (total > 0)
                M *= 1./total;
            প্রতি (পূর্ণাঙ্ক i = 0; i < n; i++)
                প্রতি (পূর্ণাঙ্ক k = 0; k < cn; k++)
                {
                    দ্বিগুণ dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            প্রতি (পূর্ণাঙ্ক k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            প্রত্যাগম;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
