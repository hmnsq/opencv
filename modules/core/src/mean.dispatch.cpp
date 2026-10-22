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
স্থির Scalar mean_(ধ্রুবক Mat& src, ধ্রুবক Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    যদি (src.empty())
        ফেরত Scalar::all(0.0);

    পূর্ণসংখ্যা k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    যদি (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.তথ্য, 0, (পূর্ণসংখ্যা)src.যোগ(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.তথ্য, 0);
    }
    অন্যথা
    {
        যদি (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.তথ্য, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.তথ্য, mask.step);
        }
    }

    যোগ_Func func = get_যোগ_Func(depth);

    CV_Assert( func != 0 );

    ধ্রুবক Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)it.size, block_আকার = যোগ, partial_Block_আকার = 0;
    পূর্ণসংখ্যা j, গুনতি_2 = 0;
    পূর্ণসংখ্যা _buf[CV_CN_MAX];
    পূর্ণসংখ্যা* buf = (পূর্ণসংখ্যা*)&s[0];
    যৌক্তিক partial_যোগ_Is_Int = depth < CV_32S;
    যৌক্তিক block_যোগ = partial_যোগ_Is_Int || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    যদি( block_যোগ )
    {
        partial_Block_আকার = depth <= CV_8S ? (1 << 23) : (1 << 15);
        block_আকার = std::min(block_আকার, partial_Block_আকার);
        buf = _buf;
        জন্য( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_আকার();
    }

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( j = 0; j < যোগ; j += block_আকার )
        {
            পূর্ণসংখ্যা bsz = std::min(যোগ - j, block_আকার);
            পূর্ণসংখ্যা nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            গুনতি_2 += nz;
            nz0 += nz;
            যদি( block_যোগ && (গুনতি_2 + block_আকার >= partial_Block_আকার || (i+1 >= it.nplanes && j+bsz >= যোগ)) )
            {
                যদি (partial_যোগ_Is_Int) {
                    জন্য( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } অন্যথা {
                    জন্য( k = 0; k < cn; k++ )
                    {
                        s[k] += ((দশমিক*)buf)[k];
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
    ফেরত s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
স্থির দ্বিগুণ stat_গুনতি(ধ্রুবক StatChunk& c)
{
    ফেরত c.mask.empty() ? (দ্বিগুণ)c.src.যোগ() : (দ্বিগুণ)গুনতি_Non_Zero(c.mask);
}

Scalar mean(প্রবেশ_Array _src, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    যদি (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        ফেরত mean_(src, mask);

    ধ্রুবক পূর্ণসংখ্যা n = (পূর্ণসংখ্যা)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<দ্বিগুণ> cnt(n);
    parallel_for_(Range(0, n), [&](ধ্রুবক Range& r)
    {
        জন্য (পূর্ণসংখ্যা i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_গুনতি(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    দ্বিগুণ যোগ = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        যোগ += cnt[i];
    }
    ফেরত s*(যোগ > 0 ? 1./যোগ : 0);
}

স্থির যোগ_Sqr_Func get_যোগ_Sqr_Func(পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_যোগ_Sqr_Func, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
স্থির যৌক্তিক ocl_meanStdDev( প্রবেশ_Array _src, OutputArray _mean, OutputArray _sdv, প্রবেশ_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    যৌক্তিক haveMask = _mask.kind() != প্রবেশ_Array_2::NONE;
    পূর্ণসংখ্যা nz = haveMask ? -1 : (পূর্ণসংখ্যা)_src.যোগ();
    Scalar mean(0), stddev(0);
    ধ্রুবক পূর্ণসংখ্যা cn = _src.channels();
    যদি (cn > 4)
        ফেরত মিথ্যা;

    {
        পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type);
        যৌক্তিক doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        ধ্রুবক ocl::Device &defDev = ocl::Device::getDefault();
        পূর্ণসংখ্যা groups = defDev.maxComputeUnits();
        যদি (defDev.isIntel())
        {
            স্থির ধ্রুবক পূর্ণসংখ্যা subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.max_Work_Group_আকার();

        পূর্ণসংখ্যা ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        পূর্ণসংখ্যা wgs2_aligned = 1;
        যখন (wgs2_aligned < (পূর্ণসংখ্যা)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        যদি ( (!doubleSupport && depth == CV_64F) )
            ফেরত মিথ্যা;

        অক্ষর cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], আকার(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], আকার(cvt[1])),
                             (পূর্ণসংখ্যা)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        যদি (k.empty())
            ফেরত মিথ্যা;

        পূর্ণসংখ্যা dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_আকার(sqdtype) + CV_ELEM_আকার(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_আকার(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_আকার(mask);

        যদি (haveMask)
            k.args(srcarg, src.cols, (পূর্ণসংখ্যা)src.যোগ(), groups, dbarg, maskarg);
        অন্যথা
            k.args(srcarg, src.cols, (পূর্ণসংখ্যা)src.যোগ(), groups, dbarg);

        size_t globalsize = groups * wgs;

        যদি(!k.run(1, &globalsize, &wgs, মিথ্যা))
            ফেরত মিথ্যা;

        প্রকারলেখ Scalar (* part_যোগ)(Mat m);
        part_যোগ funcs[3] = { ocl_part_যোগ<পূর্ণসংখ্যা>, ocl_part_যোগ<দশমিক>, ocl_part_যোগ<দ্বিগুণ> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_আকার(dtype)));

        যদি (haveMask)
            nz = saturate_cast<পূর্ণসংখ্যা>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_আকার(dtype) +
                                                           CV_ELEM_আকার(sqdtype))))[0]);
    }

    দ্বিগুণ যোগ = nz != 0 ? 1.0 / nz : 0;
    পূর্ণসংখ্যা k, j;
    জন্য (পূর্ণসংখ্যা i = 0; i < cn; ++i)
    {
        mean[i] *= যোগ;
        stddev[i] = std::sqrt(std::max(stddev[i] * যোগ - mean[i] * mean[i] , 0.));
    }

    জন্য( j = 0; j < 2; j++ )
    {
        ধ্রুবক দ্বিগুণ * ধ্রুবক sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        যদি( !_dst.needed() )
            চালু;

        যদি( !_dst.fixed_আকার() )
            _dst.create(cn, 1, CV_64F, -1, সত্য);
        Mat dst = _dst.getMat();
        পূর্ণসংখ্যা dcn = (পূর্ণসংখ্যা)dst.যোগ();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        দ্বিগুণ* dptr = dst.ptr<দ্বিগুণ>();
        জন্য( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        জন্য( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ফেরত সত্য;
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

    পূর্ণসংখ্যা k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    যদি(_mean.needed())
    {
        যদি( !_mean.fixed_আকার() )
            _mean.create(cn, 1, CV_64F, -1, সত্য);

        mean_mat = _mean.getMat();
        পূর্ণসংখ্যা dcn = (পূর্ণসংখ্যা)mean_mat.যোগ();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        দ্বিগুণ* dptr = mean_mat.ptr<দ্বিগুণ>();
        জন্য(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    যদি (_sdv.needed())
    {
        যদি( !_sdv.fixed_আকার() )
            _sdv.create(cn, 1, CV_64F, -1, সত্য);

        stddev_mat = _sdv.getMat();
        পূর্ণসংখ্যা dcn = (পূর্ণসংখ্যা)stddev_mat.যোগ();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        দ্বিগুণ* dptr = stddev_mat.ptr<দ্বিগুণ>();
        জন্য(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    যদি (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.তথ্য, 0, (পূর্ণসংখ্যা)src.যোগ(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<দ্বিগুণ>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<দ্বিগুণ>() : nullptr,
                 mask.তথ্য, 0);
    }
    অন্যথা
    {
        যদি (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.তথ্য, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<দ্বিগুণ>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<দ্বিগুণ>() : nullptr,
                     mask.তথ্য, mask.step);
        }
    }

    যোগ_Sqr_Func func = get_যোগ_Sqr_Func(depth);

    CV_Assert( func != 0 );

    ধ্রুবক Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)it.size, block_আকার = যোগ, partial_Block_আকার = 0;
    পূর্ণসংখ্যা j;
    int64_t গুনতি_2 = 0, nz0 = 0;
    দ্বিগুণ _buf[CV_CN_MAX*4];
    দ্বিগুণ *s = _buf, *sq = s + cn;
    পূর্ণসংখ্যা *sbuf = (পূর্ণসংখ্যা*)s, *sqbuf = (পূর্ণসংখ্যা*)sq;
    যৌক্তিক partial_যোগ_Is_Int = depth < CV_32S;
    যৌক্তিক block_যোগ = partial_যোগ_Is_Int || depth == CV_16F || depth == CV_16BF;
    যৌক্তিক block_Sq_যোগ = depth <= CV_8S;
    size_t esz = 0;

    জন্য( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    যদি( block_যোগ )
    {
        partial_Block_আকার = 1 << 15;
        block_আকার = std::min(block_আকার, partial_Block_আকার);
        sbuf = (পূর্ণসংখ্যা*)(sq + cn);
        যদি( block_Sq_যোগ )
            sqbuf = sbuf + cn;
        জন্য( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_আকার();
    }

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( j = 0; j < যোগ; j += block_আকার )
        {
            পূর্ণসংখ্যা bsz = std::min(যোগ - j, block_আকার);
            পূর্ণসংখ্যা nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            গুনতি_2 += nz;
            nz0 += nz;
            যদি( block_যোগ && (গুনতি_2 + block_আকার >= partial_Block_আকার || (i+1 >= it.nplanes && j+bsz >= যোগ)) )
            {
                যদি (partial_যোগ_Is_Int) {
                    জন্য( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } অন্যথা {
                    জন্য( k = 0; k < cn; k++ )
                    {
                        s[k] += ((দশমিক*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                যদি( block_Sq_যোগ )
                {
                    জন্য( k = 0; k < cn; k++ )
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
    জন্য( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    যদি (_mean.needed())
    {
        ধ্রুবক দ্বিগুণ* sptr = s;
        দ্বিগুণ* dptr = mean_mat.ptr<দ্বিগুণ>();
        জন্য( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    যদি (_sdv.needed())
    {
        ধ্রুবক দ্বিগুণ* sptr = sq;
        দ্বিগুণ* dptr = stddev_mat.ptr<দ্বিগুণ>();
        জন্য( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

স্থির শূন্য write_Stat_ফলাফল(OutputArray _dst, পূর্ণসংখ্যা cn, ধ্রুবক Scalar& v)
{
    যদি (!_dst.needed())
        ফেরত;
    যদি (!_dst.fixed_আকার())
        _dst.create(cn, 1, CV_64F, -1, সত্য);
    Mat d = _dst.getMat();
    পূর্ণসংখ্যা dcn = (পূর্ণসংখ্যা)d.যোগ();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    দ্বিগুণ* p = d.ptr<দ্বিগুণ>();
    জন্য (পূর্ণসংখ্যা k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

শূন্য meanStdDev(প্রবেশ_Array _src, OutputArray _mean, OutputArray _sdv, প্রবেশ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    যদি (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ধ্রুবক পূর্ণসংখ্যা cn = src.channels();
        ধ্রুবক যৌক্তিক maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        যদি (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            ধ্রুবক পূর্ণসংখ্যা n = (পূর্ণসংখ্যা)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<দ্বিগুণ> cnt(n);
            parallel_for_(Range(0, n), [&](ধ্রুবক Range& r)
            {
                জন্য (পূর্ণসংখ্যা i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<দ্বিগুণ>(k);
                        sd[i][k] = si.at<দ্বিগুণ>(k);
                    }
                    cnt[i] = stat_গুনতি(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            দ্বিগুণ যোগ = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            জন্য (পূর্ণসংখ্যা i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                যোগ += cnt[i];
            }
            যদি (যোগ > 0)
                M *= 1./যোগ;
            জন্য (পূর্ণসংখ্যা i = 0; i < n; i++)
                জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                {
                    দ্বিগুণ dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            জন্য (পূর্ণসংখ্যা k = 0; k < cn; k++)
                SD[k] = যোগ > 0 ? std::sqrt(std::max(V[k]/যোগ, 0.)) : 0;
            write_Stat_ফলাফল(_mean, cn, M);
            write_Stat_ফলাফল(_sdv, cn, SD);
            ফেরত;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
