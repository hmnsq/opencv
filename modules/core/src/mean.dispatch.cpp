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
ساكن Scalar اوسط_3(ثابت Mat& src, ثابت Mat& نقاب)
{
    CV_Assert( نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src.size == نقاب.size) );
    إذا (src.empty())
        أرجع Scalar::all(0.0);

    صحيح k, cn = src.channels(), ڈونگھائی = src.ڈونگھائی();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    إذا (src.isContinuous() && نقاب.isContinuous())
    {
        CALL_HAL_RET2(اوسط_Std_Dev, cv_hal_اوسط_Std_Dev, s, src.انکڑے, 0, (صحيح)src.کل(), 1, src.type(),
                      &s[0], خالی_سوچک /*stddev*/, نقاب.انکڑے, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL_RET2(اوسط_Std_Dev, cv_hal_اوسط_Std_Dev, s, src.انکڑے, src.قدم, src.خانے_گنتی, src.صفاں, src.type(),
                          &s[0], خالی_سوچک, نقاب.انکڑے, نقاب.قدم);
        }
    }

    جوڑ_Func func = get_جوڑ_Func(ڈونگھائی);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح کل = (صحيح)it.size, بلاک_ناپ_2 = کل, partial_بلاک_ناپ = 0;
    صحيح j, گنتی_2 = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&s[0];
    منطقي partial_جوڑ_Is_Int = ڈونگھائی < CV_32S;
    منطقي بلاک_جوڑ = partial_جوڑ_Is_Int || ڈونگھائی == CV_16F || ڈونگھائی == CV_16BF;
    size_t esz = 0, nz0 = 0;

    إذا( بلاک_جوڑ )
    {
        partial_بلاک_ناپ = ڈونگھائی <= CV_8S ? (1 << 23) : (1 << 15);
        بلاک_ناپ_2 = std::min(بلاک_ناپ_2, partial_بلاک_ناپ);
        buf = _buf;
        لكل( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_ناپ();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < کل; j += بلاک_ناپ_2 )
        {
            صحيح bsz = std::min(کل - j, بلاک_ناپ_2);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            گنتی_2 += nz;
            nz0 += nz;
            إذا( بلاک_جوڑ && (گنتی_2 + بلاک_ناپ_2 >= partial_بلاک_ناپ || (i+1 >= it.nplanes && j+bsz >= کل)) )
            {
                إذا (partial_جوڑ_Is_Int) {
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
                گنتی_2 = 0;
            }
            ptrs[0] += bsz*esz;
            إذا( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    أرجع s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ساكن مزدوج stat_گنتی(ثابت StatChunk& c)
{
    أرجع c.نقاب.empty() ? (مزدوج)c.src.کل() : (مزدوج)گنتی_Non_Zero(c.نقاب);
}

Scalar اوسط_2(داخلہ_Array _src, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
    CV_Assert( نقاب.empty() || نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    إذا (src.channels() > 4 || !ونڈو_For_Parallel_Stat(src, نقاب, chunks))
        أرجع اوسط_3(src, نقاب);

    ثابت صحيح n = (صحيح)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<مزدوج> cnt(n);
    parallel_for_(Range(0, n), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.شروع; i < r.end; i++)
        {
            part[i] = اوسط_3(chunks[i].src, chunks[i].نقاب);
            cnt[i] = stat_گنتی(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    مزدوج کل = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        کل += cnt[i];
    }
    أرجع s*(کل > 0 ? 1./کل : 0);
}

ساكن جوڑ_Sqr_Func get_جوڑ_Sqr_Func(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_جوڑ_Sqr_Func, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#جے_تعریف_شدہ HAVE_OPENCL
ساكن منطقي ocl_اوسط_Std_Dev( داخلہ_Array _src, OutputArray اوسط, OutputArray _sdv, داخلہ_Array نقاب_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    منطقي have_نقاب = نقاب_2.قسم() != داخلہ_Array_2::NONE;
    صحيح nz = have_نقاب ? -1 : (صحيح)_src.کل();
    Scalar اوسط_2(0), stddev(0);
    ثابت صحيح cn = _src.channels();
    إذا (cn > 4)
        أرجع خطأ;

    {
        صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type);
        منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_نقاب_Continuous = نقاب_2.isContinuous();
        ثابت ocl::Device &defDev = ocl::Device::getDefault();
        صحيح groups = defDev.maxComputeUnits();
        إذا (defDev.isIntel())
        {
            ساكن ثابت صحيح sub_ٹکڑا_EUCount = 10;
            groups = (groups / sub_ٹکڑا_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_گروہ_ناپ();

        صحيح ddepth = std::max(CV_32S, ڈونگھائی), sqddepth = std::max(CV_32F, ڈونگھائی),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_نقاب || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool);

        صحيح wgs2_aligned = 1;
        طالما (wgs2_aligned < (صحيح)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        إذا ( (!doubleSupport && ڈونگھائی == CV_64F) )
            أرجع خطأ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_منزل_Str(type), ocl::type_منزل_Str(ڈونگھائی),
                             ocl::type_منزل_Str(dtype), ocl::type_منزل_Str(ddepth), sqddepth,
                             ocl::type_منزل_Str(sqdtype), ocl::type_منزل_Str(sqddepth),
                             ocl::convertTypeStr(ڈونگھائی, sqddepth, cn, cvt[0], حجم(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_نقاب_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(ڈونگھائی, ddepth, cn, cvt[1], حجم(cvt[1])),
                             (صحيح)wgs, wgs2_aligned, have_نقاب ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        إذا (k.empty())
            أرجع خطأ;

        صحيح dbsize = groups * ((have_نقاب ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_ناپ(sqdtype) + CV_ELEM_ناپ(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), نقاب = نقاب_2.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ناپ(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_ناپ(نقاب);

        إذا (have_نقاب)
            k.دلیلاں(srcarg, src.خانے_گنتی, (صحيح)src.کل(), groups, dbarg, maskarg);
        وإلا
            k.دلیلاں(srcarg, src.خانے_گنتی, (صحيح)src.کل(), groups, dbarg);

        size_t globalsize = groups * wgs;

        إذا(!k.لگاتار(1, &globalsize, &wgs, خطأ))
            أرجع خطأ;

        تعريفنوع Scalar (* part_جوڑ)(Mat m);
        part_جوڑ funcs[3] = { ocl_part_جوڑ<صحيح>, ocl_part_جوڑ<عائم>, ocl_part_جوڑ<مزدوج> };
        Mat dbm = db.getMat(ACCESS_READ);

        اوسط_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_ناپ(dtype)));

        إذا (have_نقاب)
            nz = saturate_cast<صحيح>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_ناپ(dtype) +
                                                           CV_ELEM_ناپ(sqdtype))))[0]);
    }

    مزدوج کل = nz != 0 ? 1.0 / nz : 0;
    صحيح k, j;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        اوسط_2[i] *= کل;
        stddev[i] = std::جذر(std::max(stddev[i] * کل - اوسط_2[i] * اوسط_2[i] , 0.));
    }

    لكل( j = 0; j < 2; j++ )
    {
        ثابت مزدوج * ثابت sptr = j == 0 ? &اوسط_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? اوسط : _sdv;
        إذا( !_dst.needed() )
            استمر;

        إذا( !_dst.fixed_ناپ() )
            _dst.create(cn, 1, CV_64F, -1, سچ);
        Mat dst = _dst.getMat();
        صحيح dcn = (صحيح)dst.کل();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.خانے_گنتی == 1 || dst.صفاں == 1) && dcn >= cn );
        مزدوج* dptr = dst.ptr<مزدوج>();
        لكل( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        لكل( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    أرجع سچ;
}
#جے_انت

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ اوسط_Std_Dev_2(داخلہ_Array _src, OutputArray اوسط, OutputArray _sdv, داخلہ_Array نقاب_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( نقاب_2.empty() || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool );

    CV_OCL_لگاتار(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_اوسط_Std_Dev(_src, اوسط, _sdv, نقاب_2))

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();

    CV_Assert(نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src.size == نقاب.size));

    صحيح k, cn = src.channels(), ڈونگھائی = src.ڈونگھائی();
    Mat اوسط_mat, stddev_mat;

    إذا(اوسط.needed())
    {
        إذا( !اوسط.fixed_ناپ() )
            اوسط.create(cn, 1, CV_64F, -1, سچ);

        اوسط_mat = اوسط.getMat();
        صحيح dcn = (صحيح)اوسط_mat.کل();
        CV_Assert( اوسط_mat.type() == CV_64F && اوسط_mat.isContinuous() &&
                   (اوسط_mat.خانے_گنتی == 1 || اوسط_mat.صفاں == 1) && dcn >= cn );

        مزدوج* dptr = اوسط_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    إذا (_sdv.needed())
    {
        إذا( !_sdv.fixed_ناپ() )
            _sdv.create(cn, 1, CV_64F, -1, سچ);

        stddev_mat = _sdv.getMat();
        صحيح dcn = (صحيح)stddev_mat.کل();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.خانے_گنتی == 1 || stddev_mat.صفاں == 1) && dcn >= cn );

        مزدوج* dptr = stddev_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    إذا (src.isContinuous() && نقاب.isContinuous())
    {
        CALL_HAL(اوسط_Std_Dev, cv_hal_اوسط_Std_Dev, src.انکڑے, 0, (صحيح)src.کل(), 1, src.type(),
                 اوسط.needed() ? اوسط_mat.ptr<مزدوج>() : خالی_سوچک,
                 _sdv.needed() ? stddev_mat.ptr<مزدوج>() : خالی_سوچک,
                 نقاب.انکڑے, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL(اوسط_Std_Dev, cv_hal_اوسط_Std_Dev, src.انکڑے, src.قدم, src.خانے_گنتی, src.صفاں, src.type(),
                     اوسط.needed() ? اوسط_mat.ptr<مزدوج>() : خالی_سوچک,
                     _sdv.needed() ? stddev_mat.ptr<مزدوج>() : خالی_سوچک,
                     نقاب.انکڑے, نقاب.قدم);
        }
    }

    جوڑ_Sqr_Func func = get_جوڑ_Sqr_Func(ڈونگھائی);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح کل = (صحيح)it.size, بلاک_ناپ_2 = کل, partial_بلاک_ناپ = 0;
    صحيح j;
    int64_t گنتی_2 = 0, nz0 = 0;
    مزدوج _buf[CV_CN_MAX*4];
    مزدوج *s = _buf, *sq = s + cn;
    صحيح *sbuf = (صحيح*)s, *sqbuf = (صحيح*)sq;
    منطقي partial_جوڑ_Is_Int = ڈونگھائی < CV_32S;
    منطقي بلاک_جوڑ = partial_جوڑ_Is_Int || ڈونگھائی == CV_16F || ڈونگھائی == CV_16BF;
    منطقي بلاک_Sq_جوڑ = ڈونگھائی <= CV_8S;
    size_t esz = 0;

    لكل( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    إذا( بلاک_جوڑ )
    {
        partial_بلاک_ناپ = 1 << 15;
        بلاک_ناپ_2 = std::min(بلاک_ناپ_2, partial_بلاک_ناپ);
        sbuf = (صحيح*)(sq + cn);
        إذا( بلاک_Sq_جوڑ )
            sqbuf = sbuf + cn;
        لكل( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_ناپ();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < کل; j += بلاک_ناپ_2 )
        {
            صحيح bsz = std::min(کل - j, بلاک_ناپ_2);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            گنتی_2 += nz;
            nz0 += nz;
            إذا( بلاک_جوڑ && (گنتی_2 + بلاک_ناپ_2 >= partial_بلاک_ناپ || (i+1 >= it.nplanes && j+bsz >= کل)) )
            {
                إذا (partial_جوڑ_Is_Int) {
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
                إذا( بلاک_Sq_جوڑ )
                {
                    لكل( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                گنتی_2 = 0;
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
        sq[k] = std::جذر(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    إذا (اوسط.needed())
    {
        ثابت مزدوج* sptr = s;
        مزدوج* dptr = اوسط_mat.ptr<مزدوج>();
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

ساكن فراغ write_Stat_نتیجہ(OutputArray _dst, صحيح cn, ثابت Scalar& v)
{
    إذا (!_dst.needed())
        أرجع;
    إذا (!_dst.fixed_ناپ())
        _dst.create(cn, 1, CV_64F, -1, سچ);
    Mat d = _dst.getMat();
    صحيح dcn = (صحيح)d.کل();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.خانے_گنتی == 1 || d.صفاں == 1) && dcn >= cn );
    مزدوج* p = d.ptr<مزدوج>();
    لكل (صحيح k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

فراغ اوسط_Std_Dev(داخلہ_Array _src, OutputArray اوسط, OutputArray _sdv, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي نقاب_Ok = نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) &&
                                             src.size == نقاب.size);
        إذا (cn <= 4 && نقاب_Ok && ونڈو_For_Parallel_Stat(src, نقاب, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<مزدوج> cnt(n);
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.شروع; i < r.end; i++)
                {
                    Mat mi, si;
                    اوسط_Std_Dev_2(chunks[i].src, mi, si, chunks[i].نقاب);
                    لكل (صحيح k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<مزدوج>(k);
                        sd[i][k] = si.at<مزدوج>(k);
                    }
                    cnt[i] = stat_گنتی(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            مزدوج کل = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            لكل (صحيح i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                کل += cnt[i];
            }
            إذا (کل > 0)
                M *= 1./کل;
            لكل (صحيح i = 0; i < n; i++)
                لكل (صحيح k = 0; k < cn; k++)
                {
                    مزدوج dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            لكل (صحيح k = 0; k < cn; k++)
                SD[k] = کل > 0 ? std::جذر(std::max(V[k]/کل, 0.)) : 0;
            write_Stat_نتیجہ(اوسط, cn, M);
            write_Stat_نتیجہ(_sdv, cn, SD);
            أرجع;
        }
    }
    اوسط_Std_Dev_2(_src, اوسط, _sdv, نقاب_2);
}

} // namespace
