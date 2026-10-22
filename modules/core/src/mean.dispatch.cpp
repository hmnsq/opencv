// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "stat.hpp"
#ଅନ୍ତର୍ଭାଵ "stat_parallel.hpp"

#ଅନ୍ତର୍ଭାଵ "mean.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


ନାମାକାଶ cv {

// serial implementation, also used for the pieces of the parallel version
ସ୍ଥିର Scalar mean_(ଅଚଳ Mat& src, ଅଚଳ Mat& mask)
{
    CV_Assert( mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size) );
    ଯଦି (src.empty())
        ଫେରନ୍ତୁ Scalar::all(0.0);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, cn = src.channels(), depth = src.depth();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    ଯଦି (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(), 1, src.type(),
                      &s[0], nullptr /*stddev*/, mask.data, 0);
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (src.dims <= 2)
        {
            CALL_HAL_RET2(meanStdDev, cv_hal_meanStdDev, s, src.data, src.step, src.cols, src.rows, src.type(),
                          &s[0], nullptr, mask.data, mask.step);
        }
    }

    SumFunc func = getSumFunc(depth);

    CV_Assert( func != 0 );

    ଅଚଳ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, blockSize = total, partialBlockSize = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, ଗଣନା_3 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା _buf[CV_CN_MAX];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* buf = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)&s[0];
    ତାର୍କିକ partialSumIsInt = depth < CV_32S;
    ତାର୍କିକ blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    size_t esz = 0, nz0 = 0;

    ଯଦି( blockSum )
    {
        partialBlockSize = depth <= CV_8S ? (1 << 23) : (1 << 15);
        blockSize = std::min(blockSize, partialBlockSize);
        buf = _buf;
        ପାଇଁ( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elemSize();
    }

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( j = 0; j < total; j += blockSize )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା bsz = std::min(total - j, blockSize);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            ଗଣନା_3 += nz;
            nz0 += nz;
            ଯଦି( blockSum && (ଗଣନା_3 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                ଯଦି (partialSumIsInt) {
                    ପାଇଁ( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } ଅନ୍ୟଥା {
                    ପାଇଁ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ଦଶମିକ*)buf)[k];
                        buf[k] = 0;
                    }
                }
                ଗଣନା_3 = 0;
            }
            ptrs[0] += bsz*esz;
            ଯଦି( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    ଫେରନ୍ତୁ s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ସ୍ଥିର ଦ୍ୱିଗୁଣ stat_ଗଣନା(ଅଚଳ StatChunk& c)
{
    ଫେରନ୍ତୁ c.mask.empty() ? (ଦ୍ୱିଗୁଣ)c.src.total() : (ଦ୍ୱିଗୁଣ)ଗଣନା_Non_Zero(c.mask);
}

Scalar mean(ନିବେଶ_Array _src, ନିବେଶ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), mask = _mask.getMat();
    CV_Assert( mask.empty() || mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    ଯଦି (src.channels() > 4 || !splitForParallelStat(src, mask, chunks))
        ଫେରନ୍ତୁ mean_(src, mask);

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<ଦ୍ୱିଗୁଣ> cnt(n);
    parallel_for_(Range(0, n), [&](ଅଚଳ Range& r)
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = r.start; i < r.end; i++)
        {
            part[i] = mean_(chunks[i].src, chunks[i].mask);
            cnt[i] = stat_ଗଣନା(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    ଦ୍ୱିଗୁଣ total = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        total += cnt[i];
    }
    ଫେରନ୍ତୁ s*(total > 0 ? 1./total : 0);
}

ସ୍ଥିର SumSqrFunc getSumSqrFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getSumSqrFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ ocl_meanStdDev( ନିବେଶ_Array _src, OutputArray _mean, OutputArray _sdv, ନିବେଶ_Array _mask )
{
    CV_INSTRUMENT_REGION_OPENCL();

    ତାର୍କିକ haveMask = _mask.kind() != ନିବେଶ_Array_2::NONE;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nz = haveMask ? -1 : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_src.total();
    Scalar mean(0), stddev(0);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = _src.channels();
    ଯଦି (cn > 4)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type);
        ତାର୍କିକ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                isMaskContinuous = _mask.isContinuous();
        ଅଚଳ ocl::Device &defDev = ocl::Device::getDefault();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups = defDev.maxComputeUnits();
        ଯଦି (defDev.isIntel())
        {
            ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା subSliceEUCount = 10;
            groups = (groups / subSliceEUCount) * 2;
        }
        size_t wgs = defDev.maxWorkGroupSize();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = std::max(CV_32S, depth), sqddepth = std::max(CV_32F, depth),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!haveMask || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା wgs2_aligned = 1;
        ଯେତେବେଳେ (wgs2_aligned < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        ଯଦି ( (!doubleSupport && depth == CV_64F) )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଅକ୍ଷର cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::typeToStr(type), ocl::typeToStr(depth),
                             ocl::typeToStr(dtype), ocl::typeToStr(ddepth), sqddepth,
                             ocl::typeToStr(sqdtype), ocl::typeToStr(sqddepth),
                             ocl::convertTypeStr(depth, sqddepth, cn, cvt[0], ଆକାର(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             isMaskContinuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(depth, ddepth, cn, cvt[1], ଆକାର(cvt[1])),
                             (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs, wgs2_aligned, haveMask ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        ଯଦି (k.empty())
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbsize = groups * ((haveMask ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_SIZE(sqdtype) + CV_ELEM_SIZE(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), mask = _mask.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnlyNoSize(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::ReadOnlyNoSize(mask);

        ଯଦି (haveMask)
            k.args(srcarg, src.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(), groups, dbarg, maskarg);
        ଅନ୍ୟଥା
            k.args(srcarg, src.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(), groups, dbarg);

        size_t globalsize = groups * wgs;

        ଯଦି(!k.run(1, &globalsize, &wgs, ମିଥ୍ୟା))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ପ୍ରକାରବ୍ୟାଖ୍ୟା Scalar (* part_sum)(Mat m);
        part_sum funcs[3] = { ocl_part_sum<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, ocl_part_sum<ଦଶମିକ>, ocl_part_sum<ଦ୍ୱିଗୁଣ> };
        Mat dbm = db.getMat(ACCESS_READ);

        mean = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_SIZE(dtype)));

        ଯଦି (haveMask)
            nz = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_SIZE(dtype) +
                                                           CV_ELEM_SIZE(sqdtype))))[0]);
    }

    ଦ୍ୱିଗୁଣ total = nz != 0 ? 1.0 / nz : 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, j;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cn; ++i)
    {
        mean[i] *= total;
        stddev[i] = std::sqrt(std::max(stddev[i] * total - mean[i] * mean[i] , 0.));
    }

    ପାଇଁ( j = 0; j < 2; j++ )
    {
        ଅଚଳ ଦ୍ୱିଗୁଣ * ଅଚଳ sptr = j == 0 ? &mean[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? _mean : _sdv;
        ଯଦି( !_dst.needed() )
            ଜାରି;

        ଯଦି( !_dst.fixedSize() )
            _dst.create(cn, 1, CV_64F, -1, ସତ୍ୟ);
        Mat dst = _dst.getMat();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.total();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.cols == 1 || dst.rows == 1) && dcn >= cn );
        ଦ୍ୱିଗୁଣ* dptr = dst.ptr<ଦ୍ୱିଗୁଣ>();
        ପାଇଁ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        ପାଇଁ( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

// serial implementation, also used for the pieces of the parallel version
ସ୍ଥିର ଶୂନ୍ୟ meanStdDev_(ନିବେଶ_Array _src, OutputArray _mean, OutputArray _sdv, ନିବେଶ_Array _mask)
{
    CV_Assert(!_src.empty());
    CV_Assert( _mask.empty() || _mask.type() == CV_8U || _mask.type() == CV_8S || _mask.type() == CV_Bool );

    CV_OCL_RUN(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_meanStdDev(_src, _mean, _sdv, _mask))

    Mat src = _src.getMat(), mask = _mask.getMat();

    CV_Assert(mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) && src.size == mask.size));

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, cn = src.channels(), depth = src.depth();
    Mat mean_mat, stddev_mat;

    ଯଦି(_mean.needed())
    {
        ଯଦି( !_mean.fixedSize() )
            _mean.create(cn, 1, CV_64F, -1, ସତ୍ୟ);

        mean_mat = _mean.getMat();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mean_mat.total();
        CV_Assert( mean_mat.type() == CV_64F && mean_mat.isContinuous() &&
                   (mean_mat.cols == 1 || mean_mat.rows == 1) && dcn >= cn );

        ଦ୍ୱିଗୁଣ* dptr = mean_mat.ptr<ଦ୍ୱିଗୁଣ>();
        ପାଇଁ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ଯଦି (_sdv.needed())
    {
        ଯଦି( !_sdv.fixedSize() )
            _sdv.create(cn, 1, CV_64F, -1, ସତ୍ୟ);

        stddev_mat = _sdv.getMat();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)stddev_mat.total();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.cols == 1 || stddev_mat.rows == 1) && dcn >= cn );

        ଦ୍ୱିଗୁଣ* dptr = stddev_mat.ptr<ଦ୍ୱିଗୁଣ>();
        ପାଇଁ(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    ଯଦି (src.isContinuous() && mask.isContinuous())
    {
        CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, 0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(), 1, src.type(),
                 _mean.needed() ? mean_mat.ptr<ଦ୍ୱିଗୁଣ>() : nullptr,
                 _sdv.needed() ? stddev_mat.ptr<ଦ୍ୱିଗୁଣ>() : nullptr,
                 mask.data, 0);
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (src.dims <= 2)
        {
            CALL_HAL(meanStdDev, cv_hal_meanStdDev, src.data, src.step, src.cols, src.rows, src.type(),
                     _mean.needed() ? mean_mat.ptr<ଦ୍ୱିଗୁଣ>() : nullptr,
                     _sdv.needed() ? stddev_mat.ptr<ଦ୍ୱିଗୁଣ>() : nullptr,
                     mask.data, mask.step);
        }
    }

    SumSqrFunc func = getSumSqrFunc(depth);

    CV_Assert( func != 0 );

    ଅଚଳ Mat* arrays[] = {&src, &mask, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, blockSize = total, partialBlockSize = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    int64_t ଗଣନା_3 = 0, nz0 = 0;
    ଦ୍ୱିଗୁଣ _buf[CV_CN_MAX*4];
    ଦ୍ୱିଗୁଣ *s = _buf, *sq = s + cn;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sbuf = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)s, *sqbuf = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)sq;
    ତାର୍କିକ partialSumIsInt = depth < CV_32S;
    ତାର୍କିକ blockSum = partialSumIsInt || depth == CV_16F || depth == CV_16BF;
    ତାର୍କିକ blockSqSum = depth <= CV_8S;
    size_t esz = 0;

    ପାଇଁ( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    ଯଦି( blockSum )
    {
        partialBlockSize = 1 << 15;
        blockSize = std::min(blockSize, partialBlockSize);
        sbuf = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)(sq + cn);
        ଯଦି( blockSqSum )
            sqbuf = sbuf + cn;
        ପାଇଁ( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elemSize();
    }

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( j = 0; j < total; j += blockSize )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା bsz = std::min(total - j, blockSize);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            ଗଣନା_3 += nz;
            nz0 += nz;
            ଯଦି( blockSum && (ଗଣନା_3 + blockSize >= partialBlockSize || (i+1 >= it.nplanes && j+bsz >= total)) )
            {
                ଯଦି (partialSumIsInt) {
                    ପାଇଁ( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } ଅନ୍ୟଥା {
                    ପାଇଁ( k = 0; k < cn; k++ )
                    {
                        s[k] += ((ଦଶମିକ*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                ଯଦି( blockSqSum )
                {
                    ପାଇଁ( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                ଗଣନା_3 = 0;
            }
            ptrs[0] += bsz*esz;
            ଯଦି( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    ଦ୍ୱିଗୁଣ scale = nz0 ? 1./nz0 : 0.;
    ପାଇଁ( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::sqrt(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    ଯଦି (_mean.needed())
    {
        ଅଚଳ ଦ୍ୱିଗୁଣ* sptr = s;
        ଦ୍ୱିଗୁଣ* dptr = mean_mat.ptr<ଦ୍ୱିଗୁଣ>();
        ପାଇଁ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    ଯଦି (_sdv.needed())
    {
        ଅଚଳ ଦ୍ୱିଗୁଣ* sptr = sq;
        ଦ୍ୱିଗୁଣ* dptr = stddev_mat.ptr<ଦ୍ୱିଗୁଣ>();
        ପାଇଁ( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

ସ୍ଥିର ଶୂନ୍ୟ writeStatResult(OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ଅଚଳ Scalar& v)
{
    ଯଦି (!_dst.needed())
        ଫେରନ୍ତୁ;
    ଯଦି (!_dst.fixedSize())
        _dst.create(cn, 1, CV_64F, -1, ସତ୍ୟ);
    Mat d = _dst.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dcn = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)d.total();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.cols == 1 || d.rows == 1) && dcn >= cn );
    ଦ୍ୱିଗୁଣ* p = d.ptr<ଦ୍ୱିଗୁଣ>();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

ଶୂନ୍ୟ meanStdDev(ନିବେଶ_Array _src, OutputArray _mean, OutputArray _sdv, ନିବେଶ_Array _mask)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ଯଦି (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), mask = _mask.getMat();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = src.channels();
        ଅଚଳ ତାର୍କିକ maskOk = mask.empty() || ((mask.type() == CV_8U || mask.type() == CV_8S || mask.type() == CV_Bool) &&
                                             src.size == mask.size);
        ଯଦି (cn <= 4 && maskOk && splitForParallelStat(src, mask, chunks))
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<ଦ୍ୱିଗୁଣ> cnt(n);
            parallel_for_(Range(0, n), [&](ଅଚଳ Range& r)
            {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    meanStdDev_(chunks[i].src, mi, si, chunks[i].mask);
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<ଦ୍ୱିଗୁଣ>(k);
                        sd[i][k] = si.at<ଦ୍ୱିଗୁଣ>(k);
                    }
                    cnt[i] = stat_ଗଣନା(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            ଦ୍ୱିଗୁଣ total = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                total += cnt[i];
            }
            ଯଦି (total > 0)
                M *= 1./total;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                {
                    ଦ୍ୱିଗୁଣ dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++)
                SD[k] = total > 0 ? std::sqrt(std::max(V[k]/total, 0.)) : 0;
            writeStatResult(_mean, cn, M);
            writeStatResult(_sdv, cn, SD);
            ଫେରନ୍ତୁ;
        }
    }
    meanStdDev_(_src, _mean, _sdv, _mask);
}

} // namespace
