// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "stat.hpp"
#समावेश "stat_parallel.hpp"

#समावेश "mean.simd.hpp"
#समावेश "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


namespace cv {

// serial implementation, also used for the pieces of the parallel version
अटल Scalar mean_(स्थिर Mat& src, स्थिर Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    अगर (src.empty())
        वापस Scalar::all(0.0);

    पूर्णांक k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    अगर (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.तथ्य, 0, (पूर्णांक)src.योग(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.तथ्य, 0);
    }
    अन्यथा
    {
        अगर (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.तथ्य, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.तथ्य, mask.step);
        }
    }

    योग_Func func = get_योग_Func(depth);

    CV_Assert( func != 0 );

    स्थिर Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक योग = (पूर्णांक)it.size, block_आकार = योग, partial_Block_आकार = 0;
    पूर्णांक j, गिनती_2 = 0;
    पूर्णांक _buf[CV_CN_MAX];
    पूर्णांक* buf = (पूर्णांक*)&s[0];
    bool partial_योग_Is_Int = depth < CV_32S;
    bool block_योग = partial_योग_Is_Int || depth == CV_16F || depth == CV_16BF;
    माप_प्रकार esz = 0, nz0 = 0;

    अगर( block_योग )
    {
        partial_Block_आकार = depth <= CV_8S ? (1 << 23) : (1 << 15);
        block_आकार = std::min(block_आकार, partial_Block_आकार);
        buf = _buf;
        क्रम( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_आकार();
    }

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( j = 0; j < योग; j += block_आकार )
        {
            पूर्णांक bsz = std::min(योग - j, block_आकार);
            पूर्णांक nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            गिनती_2 += nz;
            nz0 += nz;
            अगर( block_योग && (गिनती_2 + block_आकार >= partial_Block_आकार || (i+1 >= it.nplanes && j+bsz >= योग)) )
            {
                अगर (partial_योग_Is_Int) {
                    क्रम( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } अन्यथा {
                    क्रम( k = 0; k < cn; k++ )
                    {
                        s[k] += ((भग्न*)buf)[k];
                        buf[k] = 0;
                    }
                }
                गिनती_2 = 0;
            }
            ptrs[0] += bsz*esz;
            अगर( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    वापस s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
अटल द्विगुन stat_गिनती(स्थिर StatChunk& c)
{
    वापस c.mask.empty() ? (द्विगुन)c.src.योग() : (द्विगुन)गिनती_Non_Zero(c.mask);
}

Scalar mean(निवेश_Array _src, निवेश_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    अगर (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        वापस mean_(src, mask);

    स्थिर पूर्णांक n = (पूर्णांक)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<द्विगुन> cnt(n);
    parallel_for_(Range(0, n), [&](स्थिर Range& r)
    {
        क्रम (पूर्णांक i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_गिनती(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    द्विगुन योग = 0;
    क्रम (पूर्णांक i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        योग += cnt[i];
    }
    वापस s*(योग > 0 ? 1./योग : 0);
}

अटल योग_Sqr_Func get_योग_Sqr_Func(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_योग_Sqr_Func, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#अगर_घोषित HAVE_OPENCL
अटल bool ocl_meanStdDev( निवेश_Array _src, OutputArray _mean, OutputArray _sdv, निवेश_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    bool haveMask = _mask.kind() != निवेश_Array_2::NONE;
    पूर्णांक nz = haveMask ? -1 : (पूर्णांक)_src.योग();
    Scalar mean(0), stddev(0);
    स्थिर पूर्णांक cn = _src.channels();
    अगर (cn > 4)
        वापस false;

    {
        पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type);
        bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        स्थिर ocl::Device &defDev = ocl::Device::getDefault();
        पूर्णांक groups = defDev.maxComputeUnits();
        अगर (defDev.isIntel())
        {
            अटल स्थिर पूर्णांक subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        माप_प्रकार wgs = defDev.max_Work_Group_आकार();

        पूर्णांक ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        पूर्णांक wgs2_aligned = 1;
        जबतक (wgs2_aligned < (पूर्णांक)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        अगर ( (!doubleSupport && depth == CV_64F) )
            वापस false;

        अक्षर cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], माप(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], माप(cvt[1])),
                             (पूर्णांक)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        अगर (k.empty())
            वापस false;

        पूर्णांक dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_आकार(sqdtype) + CV_ELEM_आकार(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_आकार(mask);

        अगर (haveMask)
            k.args(srcarg, src.cols, (पूर्णांक)src.योग(), groups, dbarg, maskarg);
        अन्यथा
            k.args(srcarg, src.cols, (पूर्णांक)src.योग(), groups, dbarg);

        माप_प्रकार globalsize = groups * wgs;

        अगर(!k.run(1, &globalsize, &wgs, false))
            वापस false;

        प्रकार Scalar (* part_योग)(Mat m);
        part_योग funcs[3] = { ocl_part_योग<पूर्णांक>, ocl_part_योग<भग्न>, ocl_part_योग<द्विगुन> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_आकार(dtype)));

        अगर (haveMask)
            nz = saturate_cast<पूर्णांक>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_आकार(dtype) +
                                                           CV_ELEM_आकार(sqdtype))))[0]);
    }

    द्विगुन योग = nz != 0 ? 1.0 / nz : 0;
    पूर्णांक k, j;
    क्रम (पूर्णांक i = 0; i < cn; ++i)
    {
        mean[i] *= योग;
        stddev[i] = std::वर्ग_मूल(std::max(stddev[i] * योग - mean[i] * mean[i] , 0.));
    }

    क्रम( j = 0; j < 2; j++ )
    {
        स्थिर द्विगुन * स्थिर sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        अगर( !_dst.needed() )
            जारी;

        अगर( !_dst.fixed_आकार() )
            _dst.create(cn, 1, CV_64F, -1, true);
        Mat dst = _dst.getMat();
        पूर्णांक dcn = (पूर्णांक)dst.योग();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        द्विगुन* dptr = dst.ptr<द्विगुन>();
        क्रम( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        क्रम( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    वापस true;
}
#पूर्ण_अगर

// serial implementation, also used for the pieces of the parallel version
अटल व्योम meanStdDev_(निवेश_Array _src, OutputArray _mean, OutputArray _sdv, निवेश_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    पूर्णांक k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    अगर(_mean.needed())
    {
        अगर( !_mean.fixed_आकार() )
            _mean.create(cn, 1, CV_64F, -1, true);

        mean_mat = _mean.getMat();
        पूर्णांक dcn = (पूर्णांक)mean_mat.योग();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        द्विगुन* dptr = mean_mat.ptr<द्विगुन>();
        क्रम(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    अगर (_sdv.needed())
    {
        अगर( !_sdv.fixed_आकार() )
            _sdv.create(cn, 1, CV_64F, -1, true);

        stddev_mat = _sdv.getMat();
        पूर्णांक dcn = (पूर्णांक)stddev_mat.योग();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        द्विगुन* dptr = stddev_mat.ptr<द्विगुन>();
        क्रम(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    अगर (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.तथ्य, 0, (पूर्णांक)src.योग(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<द्विगुन>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<द्विगुन>() : nullptr,
                 mask.तथ्य, 0);
    }
    अन्यथा
    {
        अगर (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.तथ्य, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<द्विगुन>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<द्विगुन>() : nullptr,
                     mask.तथ्य, mask.step);
        }
    }

    योग_Sqr_Func func = get_योग_Sqr_Func(depth);

    CV_Assert( func != 0 );

    स्थिर Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक योग = (पूर्णांक)it.size, block_आकार = योग, partial_Block_आकार = 0;
    पूर्णांक j;
    int64_t गिनती_2 = 0, nz0 = 0;
    द्विगुन _buf[CV_CN_MAX*4];
    द्विगुन *s = _buf, *sq = s + cn;
    पूर्णांक *sbuf = (पूर्णांक*)s, *sqbuf = (पूर्णांक*)sq;
    bool partial_योग_Is_Int = depth < CV_32S;
    bool block_योग = partial_योग_Is_Int || depth == CV_16F || depth == CV_16BF;
    bool block_Sq_योग = depth <= CV_8S;
    माप_प्रकार esz = 0;

    क्रम( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    अगर( block_योग )
    {
        partial_Block_आकार = 1 << 15;
        block_आकार = std::min(block_आकार, partial_Block_आकार);
        sbuf = (पूर्णांक*)(sq + cn);
        अगर( block_Sq_योग )
            sqbuf = sbuf + cn;
        क्रम( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_आकार();
    }

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( j = 0; j < योग; j += block_आकार )
        {
            पूर्णांक bsz = std::min(योग - j, block_आकार);
            पूर्णांक nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            गिनती_2 += nz;
            nz0 += nz;
            अगर( block_योग && (गिनती_2 + block_आकार >= partial_Block_आकार || (i+1 >= it.nplanes && j+bsz >= योग)) )
            {
                अगर (partial_योग_Is_Int) {
                    क्रम( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } अन्यथा {
                    क्रम( k = 0; k < cn; k++ )
                    {
                        s[k] += ((भग्न*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                अगर( block_Sq_योग )
                {
                    क्रम( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                गिनती_2 = 0;
            }
            ptrs[0] += bsz*esz;
            अगर( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    द्विगुन scale = nz0 ? 1./nz0 : 0.;
    क्रम( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::वर्ग_मूल(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    अगर (_mean.needed())
    {
        स्थिर द्विगुन* sptr = s;
        द्विगुन* dptr = mean_mat.ptr<द्विगुन>();
        क्रम( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    अगर (_sdv.needed())
    {
        स्थिर द्विगुन* sptr = sq;
        द्विगुन* dptr = stddev_mat.ptr<द्विगुन>();
        क्रम( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

अटल व्योम write_Stat_परिणाम(OutputArray _dst, पूर्णांक cn, स्थिर Scalar& v)
{
    अगर (!_dst.needed())
        वापस;
    अगर (!_dst.fixed_आकार())
        _dst.create(cn, 1, CV_64F, -1, true);
    Mat d = _dst.getMat();
    पूर्णांक dcn = (पूर्णांक)d.योग();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    द्विगुन* p = d.ptr<द्विगुन>();
    क्रम (पूर्णांक k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

व्योम meanStdDev(निवेश_Array _src, OutputArray _mean, OutputArray _sdv, निवेश_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    अगर (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        स्थिर पूर्णांक cn = src.channels();
        स्थिर bool maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        अगर (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            स्थिर पूर्णांक n = (पूर्णांक)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<द्विगुन> cnt(n);
            parallel_for_(Range(0, n), [&](स्थिर Range& r)
            {
                क्रम (पूर्णांक i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    क्रम (पूर्णांक k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<द्विगुन>(k);
                        sd[i][k] = si.at<द्विगुन>(k);
                    }
                    cnt[i] = stat_गिनती(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            द्विगुन योग = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            क्रम (पूर्णांक i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                योग += cnt[i];
            }
            अगर (योग > 0)
                M *= 1./योग;
            क्रम (पूर्णांक i = 0; i < n; i++)
                क्रम (पूर्णांक k = 0; k < cn; k++)
                {
                    द्विगुन dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            क्रम (पूर्णांक k = 0; k < cn; k++)
                SD[k] = योग > 0 ? std::वर्ग_मूल(std::max(V[k]/योग, 0.)) : 0;
            write_Stat_परिणाम(_mean, cn, M);
            write_Stat_परिणाम(_sdv, cn, SD);
            वापस;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
