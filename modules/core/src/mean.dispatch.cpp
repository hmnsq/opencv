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
ساكن Scalar منځنۍ_3(ثابت Mat& src, ثابت Mat& ماسک)
{
    CV_Assert( ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) && src.size == ماسک.size) );
    إذا (src.empty())
        أرجع Scalar::all(0.0);

    صحيح k, cn = src.channels(), ژوروالی = src.ژوروالی();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    إذا (src.isContinuous() && ماسک.isContinuous())
    {
        CALL_HAL_RET2(منځنۍ_Std_Dev, cv_hal_منځنۍ_Std_Dev, s, src.معلومات, 0, (صحيح)src.ټول(), 1, src.type(),
                      &s[0], تش_اشاره /*stddev*/, ماسک.معلومات, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL_RET2(منځنۍ_Std_Dev, cv_hal_منځنۍ_Std_Dev, s, src.معلومات, src.ګام, src.ستون_شمېر, src.لیکې, src.type(),
                          &s[0], تش_اشاره, ماسک.معلومات, ماسک.ګام);
        }
    }

    مجموعه_Func func = get_مجموعه_Func(ژوروالی);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &ماسک, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح ټول = (صحيح)it.size, بلاک_اندازه_2 = ټول, partial_بلاک_اندازه = 0;
    صحيح j, شمېر_2 = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&s[0];
    منطقي partial_مجموعه_Is_Int = ژوروالی < CV_32S;
    منطقي بلاک_مجموعه = partial_مجموعه_Is_Int || ژوروالی == CV_16F || ژوروالی == CV_16BF;
    size_t esz = 0, nz0 = 0;

    إذا( بلاک_مجموعه )
    {
        partial_بلاک_اندازه = ژوروالی <= CV_8S ? (1 << 23) : (1 << 15);
        بلاک_اندازه_2 = std::min(بلاک_اندازه_2, partial_بلاک_اندازه);
        buf = _buf;
        لكل( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_اندازه();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < ټول; j += بلاک_اندازه_2 )
        {
            صحيح bsz = std::min(ټول - j, بلاک_اندازه_2);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            شمېر_2 += nz;
            nz0 += nz;
            إذا( بلاک_مجموعه && (شمېر_2 + بلاک_اندازه_2 >= partial_بلاک_اندازه || (i+1 >= it.nplanes && j+bsz >= ټول)) )
            {
                إذا (partial_مجموعه_Is_Int) {
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
                شمېر_2 = 0;
            }
            ptrs[0] += bsz*esz;
            إذا( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    أرجع s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ساكن مزدوج stat_شمېر(ثابت StatChunk& c)
{
    أرجع c.ماسک.empty() ? (مزدوج)c.src.ټول() : (مزدوج)شمېر_Non_Zero(c.ماسک);
}

Scalar منځنۍ_2(ننوت_Array _src, ننوت_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), ماسک = ماسک_2.getMat();
    CV_Assert( ماسک.empty() || ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    إذا (src.channels() > 4 || !وېشل_For_Parallel_Stat(src, ماسک, chunks))
        أرجع منځنۍ_3(src, ماسک);

    ثابت صحيح n = (صحيح)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<مزدوج> cnt(n);
    parallel_for_(Range(0, n), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.پیل; i < r.end; i++)
        {
            part[i] = منځنۍ_3(chunks[i].src, chunks[i].ماسک);
            cnt[i] = stat_شمېر(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    مزدوج ټول = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        ټول += cnt[i];
    }
    أرجع s*(ټول > 0 ? 1./ټول : 0);
}

ساكن مجموعه_Sqr_Func get_مجموعه_Sqr_Func(صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_مجموعه_Sqr_Func, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#که_تعریف_شوی HAVE_OPENCL
ساكن منطقي ocl_منځنۍ_Std_Dev( ننوت_Array _src, OutputArray منځنۍ, OutputArray _sdv, ننوت_Array ماسک_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    منطقي have_ماسک = ماسک_2.ډول() != ننوت_Array_2::NONE;
    صحيح nz = have_ماسک ? -1 : (صحيح)_src.ټول();
    Scalar منځنۍ_2(0), stddev(0);
    ثابت صحيح cn = _src.channels();
    إذا (cn > 4)
        أرجع خطأ;

    {
        صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type);
        منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_ماسک_Continuous = ماسک_2.isContinuous();
        ثابت ocl::Device &defDev = ocl::Device::getDefault();
        صحيح groups = defDev.maxComputeUnits();
        إذا (defDev.isIntel())
        {
            ساكن ثابت صحيح sub_ټوټه_EUCount = 10;
            groups = (groups / sub_ټوټه_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_ډله_اندازه();

        صحيح ddepth = std::max(CV_32S, ژوروالی), sqddepth = std::max(CV_32F, ژوروالی),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_ماسک || ماسک_2.type() == CV_8U || ماسک_2.type() == CV_8S || ماسک_2.type() == CV_Bool);

        صحيح wgs2_aligned = 1;
        طالما (wgs2_aligned < (صحيح)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        إذا ( (!doubleSupport && ژوروالی == CV_64F) )
            أرجع خطأ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_منزل_Str(type), ocl::type_منزل_Str(ژوروالی),
                             ocl::type_منزل_Str(dtype), ocl::type_منزل_Str(ddepth), sqddepth,
                             ocl::type_منزل_Str(sqdtype), ocl::type_منزل_Str(sqddepth),
                             ocl::convertTypeStr(ژوروالی, sqddepth, cn, cvt[0], حجم(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_ماسک_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(ژوروالی, ddepth, cn, cvt[1], حجم(cvt[1])),
                             (صحيح)wgs, wgs2_aligned, have_ماسک ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        إذا (k.empty())
            أرجع خطأ;

        صحيح dbsize = groups * ((have_ماسک ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_اندازه(sqdtype) + CV_ELEM_اندازه(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), ماسک = ماسک_2.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_اندازه(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_اندازه(ماسک);

        إذا (have_ماسک)
            k.دلیلونه(srcarg, src.ستون_شمېر, (صحيح)src.ټول(), groups, dbarg, maskarg);
        وإلا
            k.دلیلونه(srcarg, src.ستون_شمېر, (صحيح)src.ټول(), groups, dbarg);

        size_t globalsize = groups * wgs;

        إذا(!k.پرلپسې(1, &globalsize, &wgs, خطأ))
            أرجع خطأ;

        تعريفنوع Scalar (* part_مجموعه)(Mat m);
        part_مجموعه funcs[3] = { ocl_part_مجموعه<صحيح>, ocl_part_مجموعه<عائم>, ocl_part_مجموعه<مزدوج> };
        Mat dbm = db.getMat(ACCESS_READ);

        منځنۍ_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_اندازه(dtype)));

        إذا (have_ماسک)
            nz = saturate_cast<صحيح>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_اندازه(dtype) +
                                                           CV_ELEM_اندازه(sqdtype))))[0]);
    }

    مزدوج ټول = nz != 0 ? 1.0 / nz : 0;
    صحيح k, j;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        منځنۍ_2[i] *= ټول;
        stddev[i] = std::جذر(std::max(stddev[i] * ټول - منځنۍ_2[i] * منځنۍ_2[i] , 0.));
    }

    لكل( j = 0; j < 2; j++ )
    {
        ثابت مزدوج * ثابت sptr = j == 0 ? &منځنۍ_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? منځنۍ : _sdv;
        إذا( !_dst.needed() )
            استمر;

        إذا( !_dst.fixed_اندازه() )
            _dst.create(cn, 1, CV_64F, -1, ریښتیا);
        Mat dst = _dst.getMat();
        صحيح dcn = (صحيح)dst.ټول();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.ستون_شمېر == 1 || dst.لیکې == 1) && dcn >= cn );
        مزدوج* dptr = dst.ptr<مزدوج>();
        لكل( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        لكل( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    أرجع ریښتیا;
}
#د_که_پای

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ منځنۍ_Std_Dev_2(ننوت_Array _src, OutputArray منځنۍ, OutputArray _sdv, ننوت_Array ماسک_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( ماسک_2.empty() || ماسک_2.type() == CV_8U || ماسک_2.type() == CV_8S || ماسک_2.type() == CV_Bool );

    CV_OCL_پرلپسې(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_منځنۍ_Std_Dev(_src, منځنۍ, _sdv, ماسک_2))

    Mat src = _src.getMat(), ماسک = ماسک_2.getMat();

    CV_Assert(ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) && src.size == ماسک.size));

    صحيح k, cn = src.channels(), ژوروالی = src.ژوروالی();
    Mat منځنۍ_mat, stddev_mat;

    إذا(منځنۍ.needed())
    {
        إذا( !منځنۍ.fixed_اندازه() )
            منځنۍ.create(cn, 1, CV_64F, -1, ریښتیا);

        منځنۍ_mat = منځنۍ.getMat();
        صحيح dcn = (صحيح)منځنۍ_mat.ټول();
        CV_Assert( منځنۍ_mat.type() == CV_64F && منځنۍ_mat.isContinuous() &&
                   (منځنۍ_mat.ستون_شمېر == 1 || منځنۍ_mat.لیکې == 1) && dcn >= cn );

        مزدوج* dptr = منځنۍ_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    إذا (_sdv.needed())
    {
        إذا( !_sdv.fixed_اندازه() )
            _sdv.create(cn, 1, CV_64F, -1, ریښتیا);

        stddev_mat = _sdv.getMat();
        صحيح dcn = (صحيح)stddev_mat.ټول();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.ستون_شمېر == 1 || stddev_mat.لیکې == 1) && dcn >= cn );

        مزدوج* dptr = stddev_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    إذا (src.isContinuous() && ماسک.isContinuous())
    {
        CALL_HAL(منځنۍ_Std_Dev, cv_hal_منځنۍ_Std_Dev, src.معلومات, 0, (صحيح)src.ټول(), 1, src.type(),
                 منځنۍ.needed() ? منځنۍ_mat.ptr<مزدوج>() : تش_اشاره,
                 _sdv.needed() ? stddev_mat.ptr<مزدوج>() : تش_اشاره,
                 ماسک.معلومات, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL(منځنۍ_Std_Dev, cv_hal_منځنۍ_Std_Dev, src.معلومات, src.ګام, src.ستون_شمېر, src.لیکې, src.type(),
                     منځنۍ.needed() ? منځنۍ_mat.ptr<مزدوج>() : تش_اشاره,
                     _sdv.needed() ? stddev_mat.ptr<مزدوج>() : تش_اشاره,
                     ماسک.معلومات, ماسک.ګام);
        }
    }

    مجموعه_Sqr_Func func = get_مجموعه_Sqr_Func(ژوروالی);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &ماسک, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح ټول = (صحيح)it.size, بلاک_اندازه_2 = ټول, partial_بلاک_اندازه = 0;
    صحيح j;
    int64_t شمېر_2 = 0, nz0 = 0;
    مزدوج _buf[CV_CN_MAX*4];
    مزدوج *s = _buf, *sq = s + cn;
    صحيح *sbuf = (صحيح*)s, *sqbuf = (صحيح*)sq;
    منطقي partial_مجموعه_Is_Int = ژوروالی < CV_32S;
    منطقي بلاک_مجموعه = partial_مجموعه_Is_Int || ژوروالی == CV_16F || ژوروالی == CV_16BF;
    منطقي بلاک_Sq_مجموعه = ژوروالی <= CV_8S;
    size_t esz = 0;

    لكل( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    إذا( بلاک_مجموعه )
    {
        partial_بلاک_اندازه = 1 << 15;
        بلاک_اندازه_2 = std::min(بلاک_اندازه_2, partial_بلاک_اندازه);
        sbuf = (صحيح*)(sq + cn);
        إذا( بلاک_Sq_مجموعه )
            sqbuf = sbuf + cn;
        لكل( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_اندازه();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < ټول; j += بلاک_اندازه_2 )
        {
            صحيح bsz = std::min(ټول - j, بلاک_اندازه_2);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            شمېر_2 += nz;
            nz0 += nz;
            إذا( بلاک_مجموعه && (شمېر_2 + بلاک_اندازه_2 >= partial_بلاک_اندازه || (i+1 >= it.nplanes && j+bsz >= ټول)) )
            {
                إذا (partial_مجموعه_Is_Int) {
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
                إذا( بلاک_Sq_مجموعه )
                {
                    لكل( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                شمېر_2 = 0;
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

    إذا (منځنۍ.needed())
    {
        ثابت مزدوج* sptr = s;
        مزدوج* dptr = منځنۍ_mat.ptr<مزدوج>();
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

ساكن فراغ write_Stat_پایله(OutputArray _dst, صحيح cn, ثابت Scalar& v)
{
    إذا (!_dst.needed())
        أرجع;
    إذا (!_dst.fixed_اندازه())
        _dst.create(cn, 1, CV_64F, -1, ریښتیا);
    Mat d = _dst.getMat();
    صحيح dcn = (صحيح)d.ټول();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.ستون_شمېر == 1 || d.لیکې == 1) && dcn >= cn );
    مزدوج* p = d.ptr<مزدوج>();
    لكل (صحيح k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

فراغ منځنۍ_Std_Dev(ننوت_Array _src, OutputArray منځنۍ, OutputArray _sdv, ننوت_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), ماسک = ماسک_2.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي ماسک_Ok = ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) &&
                                             src.size == ماسک.size);
        إذا (cn <= 4 && ماسک_Ok && وېشل_For_Parallel_Stat(src, ماسک, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<مزدوج> cnt(n);
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.پیل; i < r.end; i++)
                {
                    Mat mi, si;
                    منځنۍ_Std_Dev_2(chunks[i].src, mi, si, chunks[i].ماسک);
                    لكل (صحيح k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<مزدوج>(k);
                        sd[i][k] = si.at<مزدوج>(k);
                    }
                    cnt[i] = stat_شمېر(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            مزدوج ټول = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            لكل (صحيح i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                ټول += cnt[i];
            }
            إذا (ټول > 0)
                M *= 1./ټول;
            لكل (صحيح i = 0; i < n; i++)
                لكل (صحيح k = 0; k < cn; k++)
                {
                    مزدوج dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            لكل (صحيح k = 0; k < cn; k++)
                SD[k] = ټول > 0 ? std::جذر(std::max(V[k]/ټول, 0.)) : 0;
            write_Stat_پایله(منځنۍ, cn, M);
            write_Stat_پایله(_sdv, cn, SD);
            أرجع;
        }
    }
    منځنۍ_Std_Dev_2(_src, منځنۍ, _sdv, ماسک_2);
}

} // namespace
