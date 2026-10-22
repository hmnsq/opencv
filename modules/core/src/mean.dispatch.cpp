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
ساكن Scalar سراسري_3(ثابت Mat& src, ثابت Mat& نقاب)
{
    CV_Assert( نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src.size == نقاب.size) );
    إذا (src.empty())
        أرجع Scalar::all(0.0);

    صحيح k, cn = src.channels(), اونهائي = src.اونهائي();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    إذا (src.isContinuous() && نقاب.isContinuous())
    {
        CALL_HAL_RET2(سراسري_Std_Dev, cv_hal_سراسري_Std_Dev, s, src.معلومات, 0, (صحيح)src.ڪل(), 1, src.type(),
                      &s[0], خالي_اشارو /*stddev*/, نقاب.معلومات, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL_RET2(سراسري_Std_Dev, cv_hal_سراسري_Std_Dev, s, src.معلومات, src.قدم, src.کانن_ڳڻپ, src.صفون, src.type(),
                          &s[0], خالي_اشارو, نقاب.معلومات, نقاب.قدم);
        }
    }

    جوڙ_Func func = get_جوڙ_Func(اونهائي);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح ڪل = (صحيح)it.size, بلاڪ_سائيز_2 = ڪل, partial_بلاڪ_سائيز = 0;
    صحيح j, ڳڻپ_2 = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&s[0];
    منطقي partial_جوڙ_Is_Int = اونهائي < CV_32S;
    منطقي بلاڪ_جوڙ = partial_جوڙ_Is_Int || اونهائي == CV_16F || اونهائي == CV_16BF;
    size_t esz = 0, nz0 = 0;

    إذا( بلاڪ_جوڙ )
    {
        partial_بلاڪ_سائيز = اونهائي <= CV_8S ? (1 << 23) : (1 << 15);
        بلاڪ_سائيز_2 = std::min(بلاڪ_سائيز_2, partial_بلاڪ_سائيز);
        buf = _buf;
        لكل( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_سائيز();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < ڪل; j += بلاڪ_سائيز_2 )
        {
            صحيح bsz = std::min(ڪل - j, بلاڪ_سائيز_2);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            ڳڻپ_2 += nz;
            nz0 += nz;
            إذا( بلاڪ_جوڙ && (ڳڻپ_2 + بلاڪ_سائيز_2 >= partial_بلاڪ_سائيز || (i+1 >= it.nplanes && j+bsz >= ڪل)) )
            {
                إذا (partial_جوڙ_Is_Int) {
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
                ڳڻپ_2 = 0;
            }
            ptrs[0] += bsz*esz;
            إذا( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    أرجع s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ساكن مزدوج stat_ڳڻپ(ثابت StatChunk& c)
{
    أرجع c.نقاب.empty() ? (مزدوج)c.src.ڪل() : (مزدوج)ڳڻپ_Non_Zero(c.نقاب);
}

Scalar سراسري_2(داخلا_Array _src, داخلا_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
    CV_Assert( نقاب.empty() || نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    إذا (src.channels() > 4 || !ورهايو_For_Parallel_Stat(src, نقاب, chunks))
        أرجع سراسري_3(src, نقاب);

    ثابت صحيح n = (صحيح)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<مزدوج> cnt(n);
    parallel_for_(Range(0, n), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.شروعات; i < r.end; i++)
        {
            part[i] = سراسري_3(chunks[i].src, chunks[i].نقاب);
            cnt[i] = stat_ڳڻپ(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    مزدوج ڪل = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        ڪل += cnt[i];
    }
    أرجع s*(ڪل > 0 ? 1./ڪل : 0);
}

ساكن جوڙ_Sqr_Func get_جوڙ_Sqr_Func(صحيح اونهائي)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_جوڙ_Sqr_Func, (اونهائي),
        CV_CPU_DISPATCH_MODES_ALL);
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
ساكن منطقي ocl_سراسري_Std_Dev( داخلا_Array _src, OutputArray سراسري, OutputArray _sdv, داخلا_Array نقاب_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    منطقي have_نقاب = نقاب_2.قسم() != داخلا_Array_2::NONE;
    صحيح nz = have_نقاب ? -1 : (صحيح)_src.ڪل();
    Scalar سراسري_2(0), stddev(0);
    ثابت صحيح cn = _src.channels();
    إذا (cn > 4)
        أرجع خطأ;

    {
        صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type);
        منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_نقاب_Continuous = نقاب_2.isContinuous();
        ثابت ocl::Device &defDev = ocl::Device::getDefault();
        صحيح groups = defDev.maxComputeUnits();
        إذا (defDev.isIntel())
        {
            ساكن ثابت صحيح sub_ٽڪرو_EUCount = 10;
            groups = (groups / sub_ٽڪرو_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_گروهه_سائيز();

        صحيح ddepth = std::max(CV_32S, اونهائي), sqddepth = std::max(CV_32F, اونهائي),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_نقاب || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool);

        صحيح wgs2_aligned = 1;
        طالما (wgs2_aligned < (صحيح)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        إذا ( (!doubleSupport && اونهائي == CV_64F) )
            أرجع خطأ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_منزل_Str(type), ocl::type_منزل_Str(اونهائي),
                             ocl::type_منزل_Str(dtype), ocl::type_منزل_Str(ddepth), sqddepth,
                             ocl::type_منزل_Str(sqdtype), ocl::type_منزل_Str(sqddepth),
                             ocl::convertTypeStr(اونهائي, sqddepth, cn, cvt[0], حجم(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_نقاب_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(اونهائي, ddepth, cn, cvt[1], حجم(cvt[1])),
                             (صحيح)wgs, wgs2_aligned, have_نقاب ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        إذا (k.empty())
            أرجع خطأ;

        صحيح dbsize = groups * ((have_نقاب ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_سائيز(sqdtype) + CV_ELEM_سائيز(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), نقاب = نقاب_2.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_سائيز(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_سائيز(نقاب);

        إذا (have_نقاب)
            k.دليلون(srcarg, src.کانن_ڳڻپ, (صحيح)src.ڪل(), groups, dbarg, maskarg);
        وإلا
            k.دليلون(srcarg, src.کانن_ڳڻپ, (صحيح)src.ڪل(), groups, dbarg);

        size_t globalsize = groups * wgs;

        إذا(!k.لاڳيتو(1, &globalsize, &wgs, خطأ))
            أرجع خطأ;

        تعريفنوع Scalar (* part_جوڙ)(Mat m);
        part_جوڙ funcs[3] = { ocl_part_جوڙ<صحيح>, ocl_part_جوڙ<عائم>, ocl_part_جوڙ<مزدوج> };
        Mat dbm = db.getMat(ACCESS_READ);

        سراسري_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_سائيز(dtype)));

        إذا (have_نقاب)
            nz = saturate_cast<صحيح>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_سائيز(dtype) +
                                                           CV_ELEM_سائيز(sqdtype))))[0]);
    }

    مزدوج ڪل = nz != 0 ? 1.0 / nz : 0;
    صحيح k, j;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        سراسري_2[i] *= ڪل;
        stddev[i] = std::جذر(std::max(stddev[i] * ڪل - سراسري_2[i] * سراسري_2[i] , 0.));
    }

    لكل( j = 0; j < 2; j++ )
    {
        ثابت مزدوج * ثابت sptr = j == 0 ? &سراسري_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? سراسري : _sdv;
        إذا( !_dst.needed() )
            استمر;

        إذا( !_dst.fixed_سائيز() )
            _dst.create(cn, 1, CV_64F, -1, سچ);
        Mat dst = _dst.getMat();
        صحيح dcn = (صحيح)dst.ڪل();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.کانن_ڳڻپ == 1 || dst.صفون == 1) && dcn >= cn );
        مزدوج* dptr = dst.ptr<مزدوج>();
        لكل( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        لكل( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    أرجع سچ;
}
#جيڪڏهن_پورو

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ سراسري_Std_Dev_2(داخلا_Array _src, OutputArray سراسري, OutputArray _sdv, داخلا_Array نقاب_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( نقاب_2.empty() || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool );

    CV_OCL_لاڳيتو(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_سراسري_Std_Dev(_src, سراسري, _sdv, نقاب_2))

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();

    CV_Assert(نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src.size == نقاب.size));

    صحيح k, cn = src.channels(), اونهائي = src.اونهائي();
    Mat سراسري_mat, stddev_mat;

    إذا(سراسري.needed())
    {
        إذا( !سراسري.fixed_سائيز() )
            سراسري.create(cn, 1, CV_64F, -1, سچ);

        سراسري_mat = سراسري.getMat();
        صحيح dcn = (صحيح)سراسري_mat.ڪل();
        CV_Assert( سراسري_mat.type() == CV_64F && سراسري_mat.isContinuous() &&
                   (سراسري_mat.کانن_ڳڻپ == 1 || سراسري_mat.صفون == 1) && dcn >= cn );

        مزدوج* dptr = سراسري_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    إذا (_sdv.needed())
    {
        إذا( !_sdv.fixed_سائيز() )
            _sdv.create(cn, 1, CV_64F, -1, سچ);

        stddev_mat = _sdv.getMat();
        صحيح dcn = (صحيح)stddev_mat.ڪل();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.کانن_ڳڻپ == 1 || stddev_mat.صفون == 1) && dcn >= cn );

        مزدوج* dptr = stddev_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    إذا (src.isContinuous() && نقاب.isContinuous())
    {
        CALL_HAL(سراسري_Std_Dev, cv_hal_سراسري_Std_Dev, src.معلومات, 0, (صحيح)src.ڪل(), 1, src.type(),
                 سراسري.needed() ? سراسري_mat.ptr<مزدوج>() : خالي_اشارو,
                 _sdv.needed() ? stddev_mat.ptr<مزدوج>() : خالي_اشارو,
                 نقاب.معلومات, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL(سراسري_Std_Dev, cv_hal_سراسري_Std_Dev, src.معلومات, src.قدم, src.کانن_ڳڻپ, src.صفون, src.type(),
                     سراسري.needed() ? سراسري_mat.ptr<مزدوج>() : خالي_اشارو,
                     _sdv.needed() ? stddev_mat.ptr<مزدوج>() : خالي_اشارو,
                     نقاب.معلومات, نقاب.قدم);
        }
    }

    جوڙ_Sqr_Func func = get_جوڙ_Sqr_Func(اونهائي);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح ڪل = (صحيح)it.size, بلاڪ_سائيز_2 = ڪل, partial_بلاڪ_سائيز = 0;
    صحيح j;
    int64_t ڳڻپ_2 = 0, nz0 = 0;
    مزدوج _buf[CV_CN_MAX*4];
    مزدوج *s = _buf, *sq = s + cn;
    صحيح *sbuf = (صحيح*)s, *sqbuf = (صحيح*)sq;
    منطقي partial_جوڙ_Is_Int = اونهائي < CV_32S;
    منطقي بلاڪ_جوڙ = partial_جوڙ_Is_Int || اونهائي == CV_16F || اونهائي == CV_16BF;
    منطقي بلاڪ_Sq_جوڙ = اونهائي <= CV_8S;
    size_t esz = 0;

    لكل( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    إذا( بلاڪ_جوڙ )
    {
        partial_بلاڪ_سائيز = 1 << 15;
        بلاڪ_سائيز_2 = std::min(بلاڪ_سائيز_2, partial_بلاڪ_سائيز);
        sbuf = (صحيح*)(sq + cn);
        إذا( بلاڪ_Sq_جوڙ )
            sqbuf = sbuf + cn;
        لكل( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_سائيز();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < ڪل; j += بلاڪ_سائيز_2 )
        {
            صحيح bsz = std::min(ڪل - j, بلاڪ_سائيز_2);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            ڳڻپ_2 += nz;
            nz0 += nz;
            إذا( بلاڪ_جوڙ && (ڳڻپ_2 + بلاڪ_سائيز_2 >= partial_بلاڪ_سائيز || (i+1 >= it.nplanes && j+bsz >= ڪل)) )
            {
                إذا (partial_جوڙ_Is_Int) {
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
                إذا( بلاڪ_Sq_جوڙ )
                {
                    لكل( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                ڳڻپ_2 = 0;
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

    إذا (سراسري.needed())
    {
        ثابت مزدوج* sptr = s;
        مزدوج* dptr = سراسري_mat.ptr<مزدوج>();
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

ساكن فراغ write_Stat_نتيجو(OutputArray _dst, صحيح cn, ثابت Scalar& v)
{
    إذا (!_dst.needed())
        أرجع;
    إذا (!_dst.fixed_سائيز())
        _dst.create(cn, 1, CV_64F, -1, سچ);
    Mat d = _dst.getMat();
    صحيح dcn = (صحيح)d.ڪل();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.کانن_ڳڻپ == 1 || d.صفون == 1) && dcn >= cn );
    مزدوج* p = d.ptr<مزدوج>();
    لكل (صحيح k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

فراغ سراسري_Std_Dev(داخلا_Array _src, OutputArray سراسري, OutputArray _sdv, داخلا_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي نقاب_Ok = نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) &&
                                             src.size == نقاب.size);
        إذا (cn <= 4 && نقاب_Ok && ورهايو_For_Parallel_Stat(src, نقاب, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<مزدوج> cnt(n);
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.شروعات; i < r.end; i++)
                {
                    Mat mi, si;
                    سراسري_Std_Dev_2(chunks[i].src, mi, si, chunks[i].نقاب);
                    لكل (صحيح k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<مزدوج>(k);
                        sd[i][k] = si.at<مزدوج>(k);
                    }
                    cnt[i] = stat_ڳڻپ(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            مزدوج ڪل = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            لكل (صحيح i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                ڪل += cnt[i];
            }
            إذا (ڪل > 0)
                M *= 1./ڪل;
            لكل (صحيح i = 0; i < n; i++)
                لكل (صحيح k = 0; k < cn; k++)
                {
                    مزدوج dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            لكل (صحيح k = 0; k < cn; k++)
                SD[k] = ڪل > 0 ? std::جذر(std::max(V[k]/ڪل, 0.)) : 0;
            write_Stat_نتيجو(سراسري, cn, M);
            write_Stat_نتيجو(_sdv, cn, SD);
            أرجع;
        }
    }
    سراسري_Std_Dev_2(_src, سراسري, _sdv, نقاب_2);
}

} // namespace
