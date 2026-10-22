// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "stat.hpp"
#અન્તર્ભાવ "stat_parallel.hpp"

#અન્તર્ભાવ "mean.simd.hpp"
#અન્તર્ભાવ "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


નામાકાશ cv {

// serial implementation, also used for the pieces of the parallel version
સ્થિર Scalar mean_(અચલ Mat& src, અચલ Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    જો (src.empty())
        પાછા Scalar::all(0.0);

    પૂર્ણાંક k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    જો (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (પૂર્ણાંક)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    નહીંતર
    {
        જો (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    અચલ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    પૂર્ણાંક total = (પૂર્ણાંક)it.size, blockSize = total, partialBlockSize = 0;
    પૂર્ણાંક j, ગણતરી_2 = 0;
    પૂર્ણાંક _buf[CV_CN_MAX];
    પૂર્ણાંક* buf = (પૂર્ણાંક*)&s[0];
    તાર્કિક partialSumIsInt = depth < CV_32S;
    તાર્કિક blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    જો( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        માટે( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( j = 0; j < total; j += blockSize )
        {
            પૂર્ણાંક bsz = std::min(total - j, blockSize);
            પૂર્ણાંક nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            ગણતરી_2 += nz;
            nz0 += nz;
            જો( blockSum && (ગણતરી_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                જો (partialSumIsInt) {
                    માટે( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } નહીંતર {
                    માટે( k = 0; k < cn; k++ )
                    {
                        s[k] += ((દશાંશ*)buf)[k];
                        buf[k] = 0;
                    }
                }
                ગણતરી_2 = 0;
            }
            ptrs[0] += bsz*esz;
            જો( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    પાછા s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
સ્થિર દ્વિગુણ stat_ગણતરી(અચલ StatChunk& c)
{
    પાછા c.mask.empty() ? (દ્વિગુણ)c.src.total() : (દ્વિગુણ)ગણતરી_Non_Zero(c.mask);
}

Scalar mean(આગત_Array _src, આગત_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    જો (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        પાછા mean_(src, mask);

    અચલ પૂર્ણાંક n = (પૂર્ણાંક)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<દ્વિગુણ> cnt(n);
    parallel_for_(Range(0, n), [&](અચલ Range& r)
    {
        માટે (પૂર્ણાંક i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_ગણતરી(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    દ્વિગુણ total = 0;
    માટે (પૂર્ણાંક i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    પાછા s*(total > 0 ? 1./total : 0);
}

સ્થિર SumSqrFunc getSumSqrFunc(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
સ્થિર તાર્કિક ocl_meanStdDev( આગત_Array _src, OutputArray _mean, OutputArray _sdv, આગત_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    તાર્કિક haveMask = _mask.kind() != આગત_Array_2::NONE;
    પૂર્ણાંક nz = haveMask ? -1 : (પૂર્ણાંક)_src.total();
    Scalar mean(0), stddev(0);
    અચલ પૂર્ણાંક cn = _src.channels();
    જો (cn > 4)
        પાછા ખોટું;

    {
        પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type);
        તાર્કિક doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        અચલ ocl::Device &defDev = ocl::Device::getDefault();
        પૂર્ણાંક groups = defDev.maxComputeUnits();
        જો (defDev.isIntel())
        {
            સ્થિર અચલ પૂર્ણાંક subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        પૂર્ણાંક ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        પૂર્ણાંક wgs2_aligned = 1;
        જ્યારે (wgs2_aligned < (પૂર્ણાંક)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        જો ( (!doubleSupport && depth == CV_64F) )
            પાછા ખોટું;

        અક્ષર cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], કદ(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], કદ(cvt[1])),
                             (પૂર્ણાંક)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        જો (k.empty())
            પાછા ખોટું;

        પૂર્ણાંક dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        જો (haveMask)
            k.args(srcarg, src.cols, (પૂર્ણાંક)src.total(), groups, dbarg, maskarg);
        નહીંતર
            k.args(srcarg, src.cols, (પૂર્ણાંક)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        જો(!k.run(1, &globalsize, &wgs, ખોટું))
            પાછા ખોટું;

        પ્રકારવ્યાખ્યા Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<પૂર્ણાંક>, ocl_part_sum<દશાંશ>, ocl_part_sum<દ્વિગુણ> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        જો (haveMask)
            nz = saturate_cast<પૂર્ણાંક>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    દ્વિગુણ total = nz != 0 ? 1.0 / nz : 0;
    પૂર્ણાંક k, j;
    માટે (પૂર્ણાંક i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    માટે( j = 0; j < 2; j++ )
    {
        અચલ દ્વિગુણ * અચલ sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        જો( !_dst.needed() )
            ચાલુ;

        જો( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, સાચું);
        Mat dst = _dst.getMat();
        પૂર્ણાંક dcn = (પૂર્ણાંક)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        દ્વિગુણ* dptr = dst.ptr<દ્વિગુણ>();
        માટે( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        માટે( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    પાછા સાચું;
}
#endif

// serial implementation, also used for the pieces of the parallel version
સ્થિર શૂન્ય meanStdDev_(આગત_Array _src, OutputArray _mean, OutputArray _sdv, આગત_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    પૂર્ણાંક k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    જો(_mean.needed())
    {
        જો( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, સાચું);

        mean_mat = _mean.getMat();
        પૂર્ણાંક dcn = (પૂર્ણાંક)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        દ્વિગુણ* dptr = mean_mat.ptr<દ્વિગુણ>();
        માટે(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    જો (_sdv.needed())
    {
        જો( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, સાચું);

        stddev_mat = _sdv.getMat();
        પૂર્ણાંક dcn = (પૂર્ણાંક)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        દ્વિગુણ* dptr = stddev_mat.ptr<દ્વિગુણ>();
        માટે(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    જો (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (પૂર્ણાંક)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<દ્વિગુણ>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<દ્વિગુણ>() : nullptr,
                 mask.data, 0);
    }
    નહીંતર
    {
        જો (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<દ્વિગુણ>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<દ્વિગુણ>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    અચલ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    પૂર્ણાંક total = (પૂર્ણાંક)it.size, blockSize = total, partialBlockSize = 0;
    પૂર્ણાંક j;
    int64_t ગણતરી_2 = 0, nz0 = 0;
    દ્વિગુણ _buf[CV_CN_MAX*4];
    દ્વિગુણ *s = _buf, *sq = s + cn;
    પૂર્ણાંક *sbuf = (પૂર્ણાંક*)s, *sqbuf = (પૂર્ણાંક*)sq;
    તાર્કિક partialSumIsInt = depth < CV_32S;
    તાર્કિક blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    તાર્કિક blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    માટે( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    જો( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (પૂર્ણાંક*)(sq + cn);
        જો( blockSqSum )
            sqbuf = sbuf + cn;
        માટે( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( j = 0; j < total; j += blockSize )
        {
            પૂર્ણાંક bsz = std::min(total - j, blockSize);
            પૂર્ણાંક nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            ગણતરી_2 += nz;
            nz0 += nz;
            જો( blockSum && (ગણતરી_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                જો (partialSumIsInt) {
                    માટે( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } નહીંતર {
                    માટે( k = 0; k < cn; k++ )
                    {
                        s[k] += ((દશાંશ*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                જો( blockSqSum )
                {
                    માટે( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                ગણતરી_2 = 0;
            }
            ptrs[0] += bsz*esz;
            જો( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    દ્વિગુણ scale = nz0 ? 1./nz0 : 0.;
    માટે( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    જો (_mean.needed())
    {
        અચલ દ્વિગુણ* sptr = s;
        દ્વિગુણ* dptr = mean_mat.ptr<દ્વિગુણ>();
        માટે( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    જો (_sdv.needed())
    {
        અચલ દ્વિગુણ* sptr = sq;
        દ્વિગુણ* dptr = stddev_mat.ptr<દ્વિગુણ>();
        માટે( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

સ્થિર શૂન્ય writeStatResult(OutputArray _dst, પૂર્ણાંક cn, અચલ Scalar& v)
{
    જો (!_dst.needed())
        પાછા;
    જો (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, સાચું);
    Mat d = _dst.getMat();
    પૂર્ણાંક dcn = (પૂર્ણાંક)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    દ્વિગુણ* p = d.ptr<દ્વિગુણ>();
    માટે (પૂર્ણાંક k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

શૂન્ય meanStdDev(આગત_Array _src, OutputArray _mean, OutputArray _sdv, આગત_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    જો (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        અચલ પૂર્ણાંક cn = src.channels();
        અચલ તાર્કિક maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        જો (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            અચલ પૂર્ણાંક n = (પૂર્ણાંક)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<દ્વિગુણ> cnt(n);
            parallel_for_(Range(0, n), [&](અચલ Range& r)
            {
                માટે (પૂર્ણાંક i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    માટે (પૂર્ણાંક k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<દ્વિગુણ>(k);
                        sd[i][k] = si.at<દ્વિગુણ>(k);
                    }
                    cnt[i] = stat_ગણતરી(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            દ્વિગુણ total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            માટે (પૂર્ણાંક i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            જો (total > 0)
                M *= 1./total;
            માટે (પૂર્ણાંક i = 0; i < n; i++)
                માટે (પૂર્ણાંક k = 0; k < cn; k++)
                {
                    દ્વિગુણ dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            માટે (પૂર્ણાંક k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            પાછા;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
