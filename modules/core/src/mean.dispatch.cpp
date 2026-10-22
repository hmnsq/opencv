// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"
#अन्तर्भाव "stat_parallel.hpp"

#अन्तर्भाव "mean.simd.hpp"
#अन्तर्भाव "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


नामाकाश cv {

// serial implementation, also used for the pieces of the parallel version
स्थिर Scalar mean_(अचर Mat& src, अचर Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    यदि (src.empty())
        फर्काऊ Scalar::all(0.0);

    पूर्णाङ्क k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    यदि (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (पूर्णाङ्क)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    अन्यथा
    {
        यदि (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    अचर Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क total = (पूर्णाङ्क)it.size, blockSize = total, partialBlockSize = 0;
    पूर्णाङ्क j, गन्ती_2 = 0;
    पूर्णाङ्क _buf[CV_CN_MAX];
    पूर्णाङ्क* buf = (पूर्णाङ्क*)&s[0];
    तार्किक partialSumIsInt = depth < CV_32S;
    तार्किक blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    माप_प्रकार esz = 0, nz0 = 0;

    यदि( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        लागि( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    लागि( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        लागि( j = 0; j < total; j += blockSize )
        {
            पूर्णाङ्क bsz = std::min(total - j, blockSize);
            पूर्णाङ्क nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            गन्ती_2 += nz;
            nz0 += nz;
            यदि( blockSum && (गन्ती_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                यदि (partialSumIsInt) {
                    लागि( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } अन्यथा {
                    लागि( k = 0; k < cn; k++ )
                    {
                        s[k] += ((भिन्न*)buf)[k];
                        buf[k] = 0;
                    }
                }
                गन्ती_2 = 0;
            }
            ptrs[0] += bsz*esz;
            यदि( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    फर्काऊ s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
स्थिर द्विगुण stat_गन्ती(अचर StatChunk& c)
{
    फर्काऊ c.mask.empty() ? (द्विगुण)c.src.total() : (द्विगुण)गन्ती_Non_Zero(c.mask);
}

Scalar mean(आगत_Array _src, आगत_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    यदि (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        फर्काऊ mean_(src, mask);

    अचर पूर्णाङ्क n = (पूर्णाङ्क)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<द्विगुण> cnt(n);
    parallel_for_(Range(0, n), [&](अचर Range& r)
    {
        लागि (पूर्णाङ्क i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_गन्ती(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    द्विगुण total = 0;
    लागि (पूर्णाङ्क i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    फर्काऊ s*(total > 0 ? 1./total : 0);
}

स्थिर SumSqrFunc getSumSqrFunc(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_meanStdDev( आगत_Array _src, OutputArray _mean, OutputArray _sdv, आगत_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    तार्किक haveMask = _mask.kind() != आगत_Array_2::NONE;
    पूर्णाङ्क nz = haveMask ? -1 : (पूर्णाङ्क)_src.total();
    Scalar mean(0), stddev(0);
    अचर पूर्णाङ्क cn = _src.channels();
    यदि (cn > 4)
        फर्काऊ असत्य;

    {
        पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type);
        तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        अचर ocl::Device &defDev = ocl::Device::getDefault();
        पूर्णाङ्क groups = defDev.maxComputeUnits();
        यदि (defDev.isIntel())
        {
            स्थिर अचर पूर्णाङ्क subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        माप_प्रकार wgs = defDev.maxWorkGroupSize();

        पूर्णाङ्क ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        पूर्णाङ्क wgs2_aligned = 1;
        जबसम्म (wgs2_aligned < (पूर्णाङ्क)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        यदि ( (!doubleSupport && depth == CV_64F) )
            फर्काऊ असत्य;

        वर्ण cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], परिमाण(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], परिमाण(cvt[1])),
                             (पूर्णाङ्क)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        यदि (k.empty())
            फर्काऊ असत्य;

        पूर्णाङ्क dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        यदि (haveMask)
            k.args(srcarg, src.cols, (पूर्णाङ्क)src.total(), groups, dbarg, maskarg);
        अन्यथा
            k.args(srcarg, src.cols, (पूर्णाङ्क)src.total(), groups, dbarg);

        माप_प्रकार globalsize = groups * wgs;

        यदि(!k.run(1, &globalsize, &wgs, असत्य))
            फर्काऊ असत्य;

        प्रकारनिर्देश Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<पूर्णाङ्क>, ocl_part_sum<भिन्न>, ocl_part_sum<द्विगुण> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        यदि (haveMask)
            nz = saturate_cast<पूर्णाङ्क>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    द्विगुण total = nz != 0 ? 1.0 / nz : 0;
    पूर्णाङ्क k, j;
    लागि (पूर्णाङ्क i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::वर्ग_मूल(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    लागि( j = 0; j < 2; j++ )
    {
        अचर द्विगुण * अचर sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        यदि( !_dst.needed() )
            जारी;

        यदि( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, सत्य);
        Mat dst = _dst.getMat();
        पूर्णाङ्क dcn = (पूर्णाङ्क)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        द्विगुण* dptr = dst.ptr<द्विगुण>();
        लागि( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        लागि( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    फर्काऊ सत्य;
}
#endif

// serial implementation, also used for the pieces of the parallel version
स्थिर शून्य meanStdDev_(आगत_Array _src, OutputArray _mean, OutputArray _sdv, आगत_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    पूर्णाङ्क k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    यदि(_mean.needed())
    {
        यदि( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, सत्य);

        mean_mat = _mean.getMat();
        पूर्णाङ्क dcn = (पूर्णाङ्क)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        द्विगुण* dptr = mean_mat.ptr<द्विगुण>();
        लागि(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    यदि (_sdv.needed())
    {
        यदि( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, सत्य);

        stddev_mat = _sdv.getMat();
        पूर्णाङ्क dcn = (पूर्णाङ्क)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        द्विगुण* dptr = stddev_mat.ptr<द्विगुण>();
        लागि(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    यदि (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (पूर्णाङ्क)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<द्विगुण>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<द्विगुण>() : nullptr,
                 mask.data, 0);
    }
    अन्यथा
    {
        यदि (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<द्विगुण>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<द्विगुण>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    अचर Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क total = (पूर्णाङ्क)it.size, blockSize = total, partialBlockSize = 0;
    पूर्णाङ्क j;
    int64_t गन्ती_2 = 0, nz0 = 0;
    द्विगुण _buf[CV_CN_MAX*4];
    द्विगुण *s = _buf, *sq = s + cn;
    पूर्णाङ्क *sbuf = (पूर्णाङ्क*)s, *sqbuf = (पूर्णाङ्क*)sq;
    तार्किक partialSumIsInt = depth < CV_32S;
    तार्किक blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    तार्किक blockSqSum = depth <= CV_8S;
    माप_प्रकार esz = 0;

    लागि( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    यदि( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (पूर्णाङ्क*)(sq + cn);
        यदि( blockSqSum )
            sqbuf = sbuf + cn;
        लागि( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    लागि( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        लागि( j = 0; j < total; j += blockSize )
        {
            पूर्णाङ्क bsz = std::min(total - j, blockSize);
            पूर्णाङ्क nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            गन्ती_2 += nz;
            nz0 += nz;
            यदि( blockSum && (गन्ती_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                यदि (partialSumIsInt) {
                    लागि( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } अन्यथा {
                    लागि( k = 0; k < cn; k++ )
                    {
                        s[k] += ((भिन्न*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                यदि( blockSqSum )
                {
                    लागि( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                गन्ती_2 = 0;
            }
            ptrs[0] += bsz*esz;
            यदि( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    द्विगुण scale = nz0 ? 1./nz0 : 0.;
    लागि( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::वर्ग_मूल(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    यदि (_mean.needed())
    {
        अचर द्विगुण* sptr = s;
        द्विगुण* dptr = mean_mat.ptr<द्विगुण>();
        लागि( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    यदि (_sdv.needed())
    {
        अचर द्विगुण* sptr = sq;
        द्विगुण* dptr = stddev_mat.ptr<द्विगुण>();
        लागि( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

स्थिर शून्य writeStatResult(OutputArray _dst, पूर्णाङ्क cn, अचर Scalar& v)
{
    यदि (!_dst.needed())
        फर्काऊ;
    यदि (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, सत्य);
    Mat d = _dst.getMat();
    पूर्णाङ्क dcn = (पूर्णाङ्क)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    द्विगुण* p = d.ptr<द्विगुण>();
    लागि (पूर्णाङ्क k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

शून्य meanStdDev(आगत_Array _src, OutputArray _mean, OutputArray _sdv, आगत_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    यदि (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        अचर पूर्णाङ्क cn = src.channels();
        अचर तार्किक maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        यदि (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            अचर पूर्णाङ्क n = (पूर्णाङ्क)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<द्विगुण> cnt(n);
            parallel_for_(Range(0, n), [&](अचर Range& r)
            {
                लागि (पूर्णाङ्क i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    लागि (पूर्णाङ्क k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<द्विगुण>(k);
                        sd[i][k] = si.at<द्विगुण>(k);
                    }
                    cnt[i] = stat_गन्ती(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            द्विगुण total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            लागि (पूर्णाङ्क i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            यदि (total > 0)
                M *= 1./total;
            लागि (पूर्णाङ्क i = 0; i < n; i++)
                लागि (पूर्णाङ्क k = 0; k < cn; k++)
                {
                    द्विगुण dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            लागि (पूर्णाङ्क k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::वर्ग_मूल(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            फर्काऊ;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
