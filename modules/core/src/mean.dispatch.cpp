// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "stat.hpp"
#അന്തര്ഭാവ "stat_parallel.hpp"

#അന്തര്ഭാവ "mean.simd.hpp"
#അന്തര്ഭാവ "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


നാമാകാശ cv {

// serial implementation, also used for the pieces of the parallel version
സ്ഥിതം Scalar mean_(സ്ഥിരം Mat& src, സ്ഥിരം Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    എങ്കിൽ (src.empty())
        തിരികെ Scalar::all(0.0);

    പൂർണ്ണസംഖ്യ k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    എങ്കിൽ (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (പൂർണ്ണസംഖ്യ)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    സ്ഥിരം Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size, blockSize = total, partialBlockSize = 0;
    പൂർണ്ണസംഖ്യ j, എണ്ണം_2 = 0;
    പൂർണ്ണസംഖ്യ _buf[CV_CN_MAX];
    പൂർണ്ണസംഖ്യ* buf = (പൂർണ്ണസംഖ്യ*)&s[0];
    തർക്കം partialSumIsInt = depth < CV_32S;
    തർക്കം blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    എങ്കിൽ( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        വേണ്ടി( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( j = 0; j < total; j += blockSize )
        {
            പൂർണ്ണസംഖ്യ bsz = std::min(total - j, blockSize);
            പൂർണ്ണസംഖ്യ nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            എണ്ണം_2 += nz;
            nz0 += nz;
            എങ്കിൽ( blockSum && (എണ്ണം_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                എങ്കിൽ (partialSumIsInt) {
                    വേണ്ടി( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } അല്ലെങ്കിൽ {
                    വേണ്ടി( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ദശാംശം*)buf)[k];
                        buf[k] = 0;
                    }
                }
                എണ്ണം_2 = 0;
            }
            ptrs[0] += bsz*esz;
            എങ്കിൽ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    തിരികെ s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
സ്ഥിതം ഇരട്ട stat_എണ്ണം(സ്ഥിരം StatChunk& c)
{
    തിരികെ c.mask.empty() ? (ഇരട്ട)c.src.total() : (ഇരട്ട)എണ്ണം_Non_Zero(c.mask);
}

Scalar mean(നിവേശം_Array _src, നിവേശം_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    എങ്കിൽ (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        തിരികെ mean_(src, mask);

    സ്ഥിരം പൂർണ്ണസംഖ്യ n = (പൂർണ്ണസംഖ്യ)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<ഇരട്ട> cnt(n);
    parallel_for_(Range(0, n), [&](സ്ഥിരം Range& r)
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_എണ്ണം(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    ഇരട്ട total = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    തിരികെ s*(total > 0 ? 1./total : 0);
}

സ്ഥിതം SumSqrFunc getSumSqrFunc(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
സ്ഥിതം തർക്കം ocl_meanStdDev( നിവേശം_Array _src, OutputArray _mean, OutputArray _sdv, നിവേശം_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    തർക്കം haveMask = _mask.kind() != നിവേശം_Array_2::NONE;
    പൂർണ്ണസംഖ്യ nz = haveMask ? -1 : (പൂർണ്ണസംഖ്യ)_src.total();
    Scalar mean(0), stddev(0);
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = _src.channels();
    എങ്കിൽ (cn > 4)
        തിരികെ അസത്യം;

    {
        പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type);
        തർക്കം doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        സ്ഥിരം ocl::Device &defDev = ocl::Device::getDefault();
        പൂർണ്ണസംഖ്യ groups = defDev.maxComputeUnits();
        എങ്കിൽ (defDev.isIntel())
        {
            സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        പൂർണ്ണസംഖ്യ ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        പൂർണ്ണസംഖ്യ wgs2_aligned = 1;
        ആകുമ്പോൾ (wgs2_aligned < (പൂർണ്ണസംഖ്യ)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        എങ്കിൽ ( (!doubleSupport && depth == CV_64F) )
            തിരികെ അസത്യം;

        അക്ഷരം cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], വലുപ്പം(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], വലുപ്പം(cvt[1])),
                             (പൂർണ്ണസംഖ്യ)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        എങ്കിൽ (k.empty())
            തിരികെ അസത്യം;

        പൂർണ്ണസംഖ്യ dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        എങ്കിൽ (haveMask)
            k.args(srcarg, src.cols, (പൂർണ്ണസംഖ്യ)src.total(), groups, dbarg, maskarg);
        അല്ലെങ്കിൽ
            k.args(srcarg, src.cols, (പൂർണ്ണസംഖ്യ)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        എങ്കിൽ(!k.run(1, &globalsize, &wgs, അസത്യം))
            തിരികെ അസത്യം;

        തരനിർവചനം Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<പൂർണ്ണസംഖ്യ>, ocl_part_sum<ദശാംശം>, ocl_part_sum<ഇരട്ട> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        എങ്കിൽ (haveMask)
            nz = saturate_cast<പൂർണ്ണസംഖ്യ>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    ഇരട്ട total = nz != 0 ? 1.0 / nz : 0;
    പൂർണ്ണസംഖ്യ k, j;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    വേണ്ടി( j = 0; j < 2; j++ )
    {
        സ്ഥിരം ഇരട്ട * സ്ഥിരം sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        എങ്കിൽ( !_dst.needed() )
            തുടരുക;

        എങ്കിൽ( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, സത്യം);
        Mat dst = _dst.getMat();
        പൂർണ്ണസംഖ്യ dcn = (പൂർണ്ണസംഖ്യ)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        ഇരട്ട* dptr = dst.ptr<ഇരട്ട>();
        വേണ്ടി( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        വേണ്ടി( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    തിരികെ സത്യം;
}
#endif

// serial implementation, also used for the pieces of the parallel version
സ്ഥിതം ശൂന്യം meanStdDev_(നിവേശം_Array _src, OutputArray _mean, OutputArray _sdv, നിവേശം_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    പൂർണ്ണസംഖ്യ k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    എങ്കിൽ(_mean.needed())
    {
        എങ്കിൽ( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, സത്യം);

        mean_mat = _mean.getMat();
        പൂർണ്ണസംഖ്യ dcn = (പൂർണ്ണസംഖ്യ)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        ഇരട്ട* dptr = mean_mat.ptr<ഇരട്ട>();
        വേണ്ടി(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    എങ്കിൽ (_sdv.needed())
    {
        എങ്കിൽ( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, സത്യം);

        stddev_mat = _sdv.getMat();
        പൂർണ്ണസംഖ്യ dcn = (പൂർണ്ണസംഖ്യ)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        ഇരട്ട* dptr = stddev_mat.ptr<ഇരട്ട>();
        വേണ്ടി(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    എങ്കിൽ (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (പൂർണ്ണസംഖ്യ)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<ഇരട്ട>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<ഇരട്ട>() : nullptr,
                 mask.data, 0);
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<ഇരട്ട>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<ഇരട്ട>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    സ്ഥിരം Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size, blockSize = total, partialBlockSize = 0;
    പൂർണ്ണസംഖ്യ j;
    int64_t എണ്ണം_2 = 0, nz0 = 0;
    ഇരട്ട _buf[CV_CN_MAX*4];
    ഇരട്ട *s = _buf, *sq = s + cn;
    പൂർണ്ണസംഖ്യ *sbuf = (പൂർണ്ണസംഖ്യ*)s, *sqbuf = (പൂർണ്ണസംഖ്യ*)sq;
    തർക്കം partialSumIsInt = depth < CV_32S;
    തർക്കം blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    തർക്കം blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    വേണ്ടി( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    എങ്കിൽ( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (പൂർണ്ണസംഖ്യ*)(sq + cn);
        എങ്കിൽ( blockSqSum )
            sqbuf = sbuf + cn;
        വേണ്ടി( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( j = 0; j < total; j += blockSize )
        {
            പൂർണ്ണസംഖ്യ bsz = std::min(total - j, blockSize);
            പൂർണ്ണസംഖ്യ nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            എണ്ണം_2 += nz;
            nz0 += nz;
            എങ്കിൽ( blockSum && (എണ്ണം_2 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                എങ്കിൽ (partialSumIsInt) {
                    വേണ്ടി( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } അല്ലെങ്കിൽ {
                    വേണ്ടി( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ദശാംശം*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                എങ്കിൽ( blockSqSum )
                {
                    വേണ്ടി( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                എണ്ണം_2 = 0;
            }
            ptrs[0] += bsz*esz;
            എങ്കിൽ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    ഇരട്ട scale = nz0 ? 1./nz0 : 0.;
    വേണ്ടി( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    എങ്കിൽ (_mean.needed())
    {
        സ്ഥിരം ഇരട്ട* sptr = s;
        ഇരട്ട* dptr = mean_mat.ptr<ഇരട്ട>();
        വേണ്ടി( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    എങ്കിൽ (_sdv.needed())
    {
        സ്ഥിരം ഇരട്ട* sptr = sq;
        ഇരട്ട* dptr = stddev_mat.ptr<ഇരട്ട>();
        വേണ്ടി( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

സ്ഥിതം ശൂന്യം writeStatResult(OutputArray _dst, പൂർണ്ണസംഖ്യ cn, സ്ഥിരം Scalar& v)
{
    എങ്കിൽ (!_dst.needed())
        തിരികെ;
    എങ്കിൽ (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, സത്യം);
    Mat d = _dst.getMat();
    പൂർണ്ണസംഖ്യ dcn = (പൂർണ്ണസംഖ്യ)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    ഇരട്ട* p = d.ptr<ഇരട്ട>();
    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

ശൂന്യം meanStdDev(നിവേശം_Array _src, OutputArray _mean, OutputArray _sdv, നിവേശം_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    എങ്കിൽ (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        സ്ഥിരം പൂർണ്ണസംഖ്യ cn = src.channels();
        സ്ഥിരം തർക്കം maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        എങ്കിൽ (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ n = (പൂർണ്ണസംഖ്യ)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<ഇരട്ട> cnt(n);
            parallel_for_(Range(0, n), [&](സ്ഥിരം Range& r)
            {
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<ഇരട്ട>(k);
                        sd[i][k] = si.at<ഇരട്ട>(k);
                    }
                    cnt[i] = stat_എണ്ണം(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            ഇരട്ട total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            എങ്കിൽ (total > 0)
                M *= 1./total;
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                {
                    ഇരട്ട dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            തിരികെ;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
