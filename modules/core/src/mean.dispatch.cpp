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
ساكن Scalar میانگین_3(ثابت Mat& src, ثابت Mat& ماسک)
{
    CV_Assert( ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) && src.size == ماسک.size) );
    إذا (src.empty())
        أرجع Scalar::all(0.0);

    صحيح k, cn = src.channels(), عمق = src.عمق();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    إذا (src.isContinuous() && ماسک.isContinuous())
    {
        CALL_HAL_RET2(میانگین_Std_Dev, cv_hal_میانگین_Std_Dev, s, src.داده, 0, (صحيح)src.مجموع(), 1, src.type(),
                      &s[0], اشاره_تهی /*stddev*/, ماسک.داده, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL_RET2(میانگین_Std_Dev, cv_hal_میانگین_Std_Dev, s, src.داده, src.گام, src.تعداد_ستون, src.ردیفها, src.type(),
                          &s[0], اشاره_تهی, ماسک.داده, ماسک.گام);
        }
    }

    جمع_Func func = get_جمع_Func(عمق);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &ماسک, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح مجموع = (صحيح)it.size, بلوک_اندازه_2 = مجموع, partial_بلوک_اندازه = 0;
    صحيح j, شمار_2 = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&s[0];
    منطقي partial_جمع_Is_Int = عمق < CV_32S;
    منطقي بلوک_جمع = partial_جمع_Is_Int || عمق == CV_16F || عمق == CV_16BF;
    size_t esz = 0, nz0 = 0;

    إذا( بلوک_جمع )
    {
        partial_بلوک_اندازه = عمق <= CV_8S ? (1 << 23) : (1 << 15);
        بلوک_اندازه_2 = std::min(بلوک_اندازه_2, partial_بلوک_اندازه);
        buf = _buf;
        لكل( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_اندازه();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < مجموع; j += بلوک_اندازه_2 )
        {
            صحيح bsz = std::min(مجموع - j, بلوک_اندازه_2);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            شمار_2 += nz;
            nz0 += nz;
            إذا( بلوک_جمع && (شمار_2 + بلوک_اندازه_2 >= partial_بلوک_اندازه || (i+1 >= it.nplanes && j+bsz >= مجموع)) )
            {
                إذا (partial_جمع_Is_Int) {
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
                شمار_2 = 0;
            }
            ptrs[0] += bsz*esz;
            إذا( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    أرجع s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ساكن مزدوج stat_شمار(ثابت StatChunk& c)
{
    أرجع c.ماسک.empty() ? (مزدوج)c.src.مجموع() : (مزدوج)شمار_Non_Zero(c.ماسک);
}

Scalar میانگین_2(درون_داد_Array _src, درون_داد_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), ماسک = ماسک_2.getMat();
    CV_Assert( ماسک.empty() || ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    إذا (src.channels() > 4 || !تقسیم_For_Parallel_Stat(src, ماسک, chunks))
        أرجع میانگین_3(src, ماسک);

    ثابت صحيح n = (صحيح)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<مزدوج> cnt(n);
    parallel_for_(Range(0, n), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.شروع; i < r.end; i++)
        {
            part[i] = میانگین_3(chunks[i].src, chunks[i].ماسک);
            cnt[i] = stat_شمار(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    مزدوج مجموع = 0;
    لكل (صحيح i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        مجموع += cnt[i];
    }
    أرجع s*(مجموع > 0 ? 1./مجموع : 0);
}

ساكن جمع_Sqr_Func get_جمع_Sqr_Func(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_جمع_Sqr_Func, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شده HAVE_OPENCL
ساكن منطقي ocl_میانگین_Std_Dev( درون_داد_Array _src, OutputArray میانگین, OutputArray _sdv, درون_داد_Array ماسک_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    منطقي have_ماسک = ماسک_2.نوع() != درون_داد_Array_2::NONE;
    صحيح nz = have_ماسک ? -1 : (صحيح)_src.مجموع();
    Scalar میانگین_2(0), stddev(0);
    ثابت صحيح cn = _src.channels();
    إذا (cn > 4)
        أرجع خطأ;

    {
        صحيح type = _src.type(), عمق = CV_MAT_عمق(type);
        منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_ماسک_Continuous = ماسک_2.isContinuous();
        ثابت ocl::Device &defDev = ocl::Device::getDefault();
        صحيح groups = defDev.maxComputeUnits();
        إذا (defDev.isIntel())
        {
            ساكن ثابت صحيح sub_برش_EUCount = 10;
            groups = (groups / sub_برش_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_گروه_اندازه();

        صحيح ddepth = std::max(CV_32S, عمق), sqddepth = std::max(CV_32F, عمق),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_ماسک || ماسک_2.type() == CV_8U || ماسک_2.type() == CV_8S || ماسک_2.type() == CV_Bool);

        صحيح wgs2_aligned = 1;
        طالما (wgs2_aligned < (صحيح)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        إذا ( (!doubleSupport && عمق == CV_64F) )
            أرجع خطأ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_مقصد_Str(type), ocl::type_مقصد_Str(عمق),
                             ocl::type_مقصد_Str(dtype), ocl::type_مقصد_Str(ddepth), sqddepth,
                             ocl::type_مقصد_Str(sqdtype), ocl::type_مقصد_Str(sqddepth),
                             ocl::convertTypeStr(عمق, sqddepth, cn, cvt[0], حجم(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_ماسک_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(عمق, ddepth, cn, cvt[1], حجم(cvt[1])),
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
            k.آرگومانها(srcarg, src.تعداد_ستون, (صحيح)src.مجموع(), groups, dbarg, maskarg);
        وإلا
            k.آرگومانها(srcarg, src.تعداد_ستون, (صحيح)src.مجموع(), groups, dbarg);

        size_t globalsize = groups * wgs;

        إذا(!k.پیاپی(1, &globalsize, &wgs, خطأ))
            أرجع خطأ;

        تعريفنوع Scalar (* part_جمع)(Mat m);
        part_جمع funcs[3] = { ocl_part_جمع<صحيح>, ocl_part_جمع<عائم>, ocl_part_جمع<مزدوج> };
        Mat dbm = db.getMat(ACCESS_READ);

        میانگین_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_اندازه(dtype)));

        إذا (have_ماسک)
            nz = saturate_cast<صحيح>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_اندازه(dtype) +
                                                           CV_ELEM_اندازه(sqdtype))))[0]);
    }

    مزدوج مجموع = nz != 0 ? 1.0 / nz : 0;
    صحيح k, j;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        میانگین_2[i] *= مجموع;
        stddev[i] = std::جذر(std::max(stddev[i] * مجموع - میانگین_2[i] * میانگین_2[i] , 0.));
    }

    لكل( j = 0; j < 2; j++ )
    {
        ثابت مزدوج * ثابت sptr = j == 0 ? &میانگین_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? میانگین : _sdv;
        إذا( !_dst.needed() )
            استمر;

        إذا( !_dst.fixed_اندازه() )
            _dst.create(cn, 1, CV_64F, -1, درست);
        Mat dst = _dst.getMat();
        صحيح dcn = (صحيح)dst.مجموع();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.تعداد_ستون == 1 || dst.ردیفها == 1) && dcn >= cn );
        مزدوج* dptr = dst.ptr<مزدوج>();
        لكل( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        لكل( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    أرجع درست;
}
#پایان_اگر

// serial implementation, also used for the pieces of the parallel version
ساكن فراغ میانگین_Std_Dev_2(درون_داد_Array _src, OutputArray میانگین, OutputArray _sdv, درون_داد_Array ماسک_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( ماسک_2.empty() || ماسک_2.type() == CV_8U || ماسک_2.type() == CV_8S || ماسک_2.type() == CV_Bool );

    CV_OCL_پیاپی(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_میانگین_Std_Dev(_src, میانگین, _sdv, ماسک_2))

    Mat src = _src.getMat(), ماسک = ماسک_2.getMat();

    CV_Assert(ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) && src.size == ماسک.size));

    صحيح k, cn = src.channels(), عمق = src.عمق();
    Mat میانگین_mat, stddev_mat;

    إذا(میانگین.needed())
    {
        إذا( !میانگین.fixed_اندازه() )
            میانگین.create(cn, 1, CV_64F, -1, درست);

        میانگین_mat = میانگین.getMat();
        صحيح dcn = (صحيح)میانگین_mat.مجموع();
        CV_Assert( میانگین_mat.type() == CV_64F && میانگین_mat.isContinuous() &&
                   (میانگین_mat.تعداد_ستون == 1 || میانگین_mat.ردیفها == 1) && dcn >= cn );

        مزدوج* dptr = میانگین_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    إذا (_sdv.needed())
    {
        إذا( !_sdv.fixed_اندازه() )
            _sdv.create(cn, 1, CV_64F, -1, درست);

        stddev_mat = _sdv.getMat();
        صحيح dcn = (صحيح)stddev_mat.مجموع();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.تعداد_ستون == 1 || stddev_mat.ردیفها == 1) && dcn >= cn );

        مزدوج* dptr = stddev_mat.ptr<مزدوج>();
        لكل(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    إذا (src.isContinuous() && ماسک.isContinuous())
    {
        CALL_HAL(میانگین_Std_Dev, cv_hal_میانگین_Std_Dev, src.داده, 0, (صحيح)src.مجموع(), 1, src.type(),
                 میانگین.needed() ? میانگین_mat.ptr<مزدوج>() : اشاره_تهی,
                 _sdv.needed() ? stddev_mat.ptr<مزدوج>() : اشاره_تهی,
                 ماسک.داده, 0);
    }
    وإلا
    {
        إذا (src.dims <= 2)
        {
            CALL_HAL(میانگین_Std_Dev, cv_hal_میانگین_Std_Dev, src.داده, src.گام, src.تعداد_ستون, src.ردیفها, src.type(),
                     میانگین.needed() ? میانگین_mat.ptr<مزدوج>() : اشاره_تهی,
                     _sdv.needed() ? stddev_mat.ptr<مزدوج>() : اشاره_تهی,
                     ماسک.داده, ماسک.گام);
        }
    }

    جمع_Sqr_Func func = get_جمع_Sqr_Func(عمق);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &ماسک, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح مجموع = (صحيح)it.size, بلوک_اندازه_2 = مجموع, partial_بلوک_اندازه = 0;
    صحيح j;
    int64_t شمار_2 = 0, nz0 = 0;
    مزدوج _buf[CV_CN_MAX*4];
    مزدوج *s = _buf, *sq = s + cn;
    صحيح *sbuf = (صحيح*)s, *sqbuf = (صحيح*)sq;
    منطقي partial_جمع_Is_Int = عمق < CV_32S;
    منطقي بلوک_جمع = partial_جمع_Is_Int || عمق == CV_16F || عمق == CV_16BF;
    منطقي بلوک_Sq_جمع = عمق <= CV_8S;
    size_t esz = 0;

    لكل( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    إذا( بلوک_جمع )
    {
        partial_بلوک_اندازه = 1 << 15;
        بلوک_اندازه_2 = std::min(بلوک_اندازه_2, partial_بلوک_اندازه);
        sbuf = (صحيح*)(sq + cn);
        إذا( بلوک_Sq_جمع )
            sqbuf = sbuf + cn;
        لكل( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_اندازه();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < مجموع; j += بلوک_اندازه_2 )
        {
            صحيح bsz = std::min(مجموع - j, بلوک_اندازه_2);
            صحيح nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            شمار_2 += nz;
            nz0 += nz;
            إذا( بلوک_جمع && (شمار_2 + بلوک_اندازه_2 >= partial_بلوک_اندازه || (i+1 >= it.nplanes && j+bsz >= مجموع)) )
            {
                إذا (partial_جمع_Is_Int) {
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
                إذا( بلوک_Sq_جمع )
                {
                    لكل( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                شمار_2 = 0;
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

    إذا (میانگین.needed())
    {
        ثابت مزدوج* sptr = s;
        مزدوج* dptr = میانگین_mat.ptr<مزدوج>();
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

ساكن فراغ write_Stat_نتیجه(OutputArray _dst, صحيح cn, ثابت Scalar& v)
{
    إذا (!_dst.needed())
        أرجع;
    إذا (!_dst.fixed_اندازه())
        _dst.create(cn, 1, CV_64F, -1, درست);
    Mat d = _dst.getMat();
    صحيح dcn = (صحيح)d.مجموع();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.تعداد_ستون == 1 || d.ردیفها == 1) && dcn >= cn );
    مزدوج* p = d.ptr<مزدوج>();
    لكل (صحيح k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

فراغ میانگین_Std_Dev(درون_داد_Array _src, OutputArray میانگین, OutputArray _sdv, درون_داد_Array ماسک_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    إذا (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), ماسک = ماسک_2.getMat();
        ثابت صحيح cn = src.channels();
        ثابت منطقي ماسک_Ok = ماسک.empty() || ((ماسک.type() == CV_8U || ماسک.type() == CV_8S || ماسک.type() == CV_Bool) &&
                                             src.size == ماسک.size);
        إذا (cn <= 4 && ماسک_Ok && تقسیم_For_Parallel_Stat(src, ماسک, chunks))
        {
            ثابت صحيح n = (صحيح)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<مزدوج> cnt(n);
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                لكل (صحيح i = r.شروع; i < r.end; i++)
                {
                    Mat mi, si;
                    میانگین_Std_Dev_2(chunks[i].src, mi, si, chunks[i].ماسک);
                    لكل (صحيح k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<مزدوج>(k);
                        sd[i][k] = si.at<مزدوج>(k);
                    }
                    cnt[i] = stat_شمار(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            مزدوج مجموع = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            لكل (صحيح i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                مجموع += cnt[i];
            }
            إذا (مجموع > 0)
                M *= 1./مجموع;
            لكل (صحيح i = 0; i < n; i++)
                لكل (صحيح k = 0; k < cn; k++)
                {
                    مزدوج dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            لكل (صحيح k = 0; k < cn; k++)
                SD[k] = مجموع > 0 ? std::جذر(std::max(V[k]/مجموع, 0.)) : 0;
            write_Stat_نتیجه(میانگین, cn, M);
            write_Stat_نتیجه(_sdv, cn, SD);
            أرجع;
        }
    }
    میانگین_Std_Dev_2(_src, میانگین, _sdv, ماسک_2);
}

} // namespace
