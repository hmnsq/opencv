// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"
#ಅನ್ತರ್ಭಾವ "stat_parallel.hpp"

#ಅನ್ತರ್ಭಾವ "mean.simd.hpp"
#ಅನ್ತರ್ಭಾವ "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


ನಾಮಾಕಾಶ cv {

// serial implementation, also used for the pieces of the parallel version
ಸ್ಥಿರ Scalar mean_(ಧ್ರುವ Mat& src, ಧ್ರುವ Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    ಯದಿ (src.empty())
        ಪ್ರತ್ಯಾಗಮ Scalar::all(0.0);

    ಪೂರ್ಣಾಙ್ಕ k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    ಯದಿ (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (ಪೂರ್ಣಾಙ್ಕ)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    ಧ್ರುವ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ total = (ಪೂರ್ಣಾಙ್ಕ)it.size, blockSize = total, partialBlockSize = 0;
    ಪೂರ್ಣಾಙ್ಕ j, ಎಣಿಕೆ_2 = 0;
    ಪೂರ್ಣಾಙ್ಕ _buf[CV_CN_MAX];
    ಪೂರ್ಣಾಙ್ಕ* buf = (ಪೂರ್ಣಾಙ್ಕ*)&s[0];
    ತಾರ್ಕಿಕ partialSumIsInt = depth < CV_32S;
    ತಾರ್ಕಿಕ blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    ಯದಿ( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        ಪ್ರತಿ( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( j = 0; j < total; j += blockSize )
        {
            ಪೂರ್ಣಾಙ್ಕ bsz = std::min(total - j, blockSize);
            ಪೂರ್ಣಾಙ್ಕ nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            ಎಣಿಕೆ_2 += nz;
            nz0 += nz;
            ಯದಿ( blockSum && (ಎಣಿಕೆ_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                ಯದಿ (partialSumIsInt) {
                    ಪ್ರತಿ( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } ಅನ್ಯಥಾ {
                    ಪ್ರತಿ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ಭಿನ್ನ*)buf)[k];
                        buf[k] = 0;
                    }
                }
                ಎಣಿಕೆ_2 = 0;
            }
            ptrs[0] += bsz*esz;
            ಯದಿ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    ಪ್ರತ್ಯಾಗಮ s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ಸ್ಥಿರ ದ್ವಿಗುಣ stat_ಎಣಿಕೆ(ಧ್ರುವ StatChunk& c)
{
    ಪ್ರತ್ಯಾಗಮ c.mask.empty() ? (ದ್ವಿಗುಣ)c.src.total() : (ದ್ವಿಗುಣ)ಎಣಿಕೆ_Non_Zero(c.mask);
}

Scalar mean(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    ಯದಿ (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        ಪ್ರತ್ಯಾಗಮ mean_(src, mask);

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n = (ಪೂರ್ಣಾಙ್ಕ)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<ದ್ವಿಗುಣ> cnt(n);
    parallel_for_(Range(0, n), [&](ಧ್ರುವ Range& r)
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_ಎಣಿಕೆ(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    ದ್ವಿಗುಣ total = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    ಪ್ರತ್ಯಾಗಮ s*(total > 0 ? 1./total : 0);
}

ಸ್ಥಿರ SumSqrFunc getSumSqrFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_meanStdDev( ಒಳಹರಿವು_Array _src, OutputArray _mean, OutputArray _sdv, ಒಳಹರಿವು_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    ತಾರ್ಕಿಕ haveMask = _mask.kind() != ಒಳಹರಿವು_Array_2::NONE;
    ಪೂರ್ಣಾಙ್ಕ nz = haveMask ? -1 : (ಪೂರ್ಣಾಙ್ಕ)_src.total();
    Scalar mean(0), stddev(0);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = _src.channels();
    ಯದಿ (cn > 4)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    {
        ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type);
        ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        ಧ್ರುವ ocl::Device &defDev = ocl::Device::getDefault();
        ಪೂರ್ಣಾಙ್ಕ groups = defDev.maxComputeUnits();
        ಯದಿ (defDev.isIntel())
        {
            ಸ್ಥಿರ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        ಪೂರ್ಣಾಙ್ಕ ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        ಪೂರ್ಣಾಙ್ಕ wgs2_aligned = 1;
        ಯಾವತ್ (wgs2_aligned < (ಪೂರ್ಣಾಙ್ಕ)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        ಯದಿ ( (!doubleSupport && depth == CV_64F) )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ವರ್ಣ cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], ಪರಿಮಾಣ(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], ಪರಿಮಾಣ(cvt[1])),
                             (ಪೂರ್ಣಾಙ್ಕ)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        ಯದಿ (k.empty())
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಪೂರ್ಣಾಙ್ಕ dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        ಯದಿ (haveMask)
            k.args(srcarg, src.cols, (ಪೂರ್ಣಾಙ್ಕ)src.total(), groups, dbarg, maskarg);
        ಅನ್ಯಥಾ
            k.args(srcarg, src.cols, (ಪೂರ್ಣಾಙ್ಕ)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        ಯದಿ(!k.run(1, &globalsize, &wgs, ಅಸತ್ಯ))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಪ್ರಕಾರನಿರ್ದೇಶ Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<ಪೂರ್ಣಾಙ್ಕ>, ocl_part_sum<ಭಿನ್ನ>, ocl_part_sum<ದ್ವಿಗುಣ> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        ಯದಿ (haveMask)
            nz = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    ದ್ವಿಗುಣ total = nz != 0 ? 1.0 / nz : 0;
    ಪೂರ್ಣಾಙ್ಕ k, j;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    ಪ್ರತಿ( j = 0; j < 2; j++ )
    {
        ಧ್ರುವ ದ್ವಿಗುಣ * ಧ್ರುವ sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        ಯದಿ( !_dst.needed() )
            ಅನುವರ್ತ;

        ಯದಿ( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, ಸತ್ಯ);
        Mat dst = _dst.getMat();
        ಪೂರ್ಣಾಙ್ಕ dcn = (ಪೂರ್ಣಾಙ್ಕ)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        ದ್ವಿಗುಣ* dptr = dst.ptr<ದ್ವಿಗುಣ>();
        ಪ್ರತಿ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        ಪ್ರತಿ( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif

// serial implementation, also used for the pieces of the parallel version
ಸ್ಥಿರ ಶೂನ್ಯ meanStdDev_(ಒಳಹರಿವು_Array _src, OutputArray _mean, OutputArray _sdv, ಒಳಹರಿವು_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    ಪೂರ್ಣಾಙ್ಕ k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    ಯದಿ(_mean.needed())
    {
        ಯದಿ( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, ಸತ್ಯ);

        mean_mat = _mean.getMat();
        ಪೂರ್ಣಾಙ್ಕ dcn = (ಪೂರ್ಣಾಙ್ಕ)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        ದ್ವಿಗುಣ* dptr = mean_mat.ptr<ದ್ವಿಗುಣ>();
        ಪ್ರತಿ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ಯದಿ (_sdv.needed())
    {
        ಯದಿ( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, ಸತ್ಯ);

        stddev_mat = _sdv.getMat();
        ಪೂರ್ಣಾಙ್ಕ dcn = (ಪೂರ್ಣಾಙ್ಕ)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        ದ್ವಿಗುಣ* dptr = stddev_mat.ptr<ದ್ವಿಗುಣ>();
        ಪ್ರತಿ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    ಯದಿ (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (ಪೂರ್ಣಾಙ್ಕ)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<ದ್ವಿಗುಣ>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<ದ್ವಿಗುಣ>() : nullptr,
                 mask.data, 0);
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<ದ್ವಿಗುಣ>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<ದ್ವಿಗುಣ>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    ಧ್ರುವ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ total = (ಪೂರ್ಣಾಙ್ಕ)it.size, blockSize = total, partialBlockSize = 0;
    ಪೂರ್ಣಾಙ್ಕ j;
    int64_t ಎಣಿಕೆ_2 = 0, nz0 = 0;
    ದ್ವಿಗುಣ _buf[CV_CN_MAX*4];
    ದ್ವಿಗುಣ *s = _buf, *sq = s + cn;
    ಪೂರ್ಣಾಙ್ಕ *sbuf = (ಪೂರ್ಣಾಙ್ಕ*)s, *sqbuf = (ಪೂರ್ಣಾಙ್ಕ*)sq;
    ತಾರ್ಕಿಕ partialSumIsInt = depth < CV_32S;
    ತಾರ್ಕಿಕ blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    ತಾರ್ಕಿಕ blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    ಪ್ರತಿ( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    ಯದಿ( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (ಪೂರ್ಣಾಙ್ಕ*)(sq + cn);
        ಯದಿ( blockSqSum )
            sqbuf = sbuf + cn;
        ಪ್ರತಿ( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( j = 0; j < total; j += blockSize )
        {
            ಪೂರ್ಣಾಙ್ಕ bsz = std::min(total - j, blockSize);
            ಪೂರ್ಣಾಙ್ಕ nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            ಎಣಿಕೆ_2 += nz;
            nz0 += nz;
            ಯದಿ( blockSum && (ಎಣಿಕೆ_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                ಯದಿ (partialSumIsInt) {
                    ಪ್ರತಿ( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } ಅನ್ಯಥಾ {
                    ಪ್ರತಿ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ಭಿನ್ನ*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                ಯದಿ( blockSqSum )
                {
                    ಪ್ರತಿ( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                ಎಣಿಕೆ_2 = 0;
            }
            ptrs[0] += bsz*esz;
            ಯದಿ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    ದ್ವಿಗುಣ scale = nz0 ? 1./nz0 : 0.;
    ಪ್ರತಿ( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    ಯದಿ (_mean.needed())
    {
        ಧ್ರುವ ದ್ವಿಗುಣ* sptr = s;
        ದ್ವಿಗುಣ* dptr = mean_mat.ptr<ದ್ವಿಗುಣ>();
        ಪ್ರತಿ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    ಯದಿ (_sdv.needed())
    {
        ಧ್ರುವ ದ್ವಿಗುಣ* sptr = sq;
        ದ್ವಿಗುಣ* dptr = stddev_mat.ptr<ದ್ವಿಗುಣ>();
        ಪ್ರತಿ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ writeStatResult(OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ cn, ಧ್ರುವ Scalar& v)
{
    ಯದಿ (!_dst.needed())
        ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, ಸತ್ಯ);
    Mat d = _dst.getMat();
    ಪೂರ್ಣಾಙ್ಕ dcn = (ಪೂರ್ಣಾಙ್ಕ)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    ದ್ವಿಗುಣ* p = d.ptr<ದ್ವಿಗುಣ>();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

ಶೂನ್ಯ meanStdDev(ಒಳಹರಿವು_Array _src, OutputArray _mean, OutputArray _sdv, ಒಳಹರಿವು_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ಯದಿ (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = src.channels();
        ಧ್ರುವ ತಾರ್ಕಿಕ maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        ಯದಿ (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ n = (ಪೂರ್ಣಾಙ್ಕ)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<ದ್ವಿಗುಣ> cnt(n);
            parallel_for_(Range(0, n), [&](ಧ್ರುವ Range& r)
            {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<ದ್ವಿಗುಣ>(k);
                        sd[i][k] = si.at<ದ್ವಿಗುಣ>(k);
                    }
                    cnt[i] = stat_ಎಣಿಕೆ(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            ದ್ವಿಗುಣ total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            ಯದಿ (total > 0)
                M *= 1./total;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                {
                    ದ್ವಿಗುಣ dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            ಪ್ರತ್ಯಾಗಮ;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
