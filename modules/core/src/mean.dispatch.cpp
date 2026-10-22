// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"
#شامل "stat_parallel.hpp"

#شامل "mean.simd.hpp"
#شامل "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


نامخلا cv {

// serial implementation, also used for the pieces of the parallel version
ساکن Scalar اوسط_3(ثابت Mat& src, ثابت Mat& نقاب)
{
    CV_Assert( نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src.size == نقاب.size) );
    اگر (src.empty())
        واپس Scalar::all(0.0);

    صحیح k, cn = src.channels(), گہرائی = src.گہرائی();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    اگر (src.isContinuous() && نقاب.isContinuous())
    {
        CALL_HAL_RET2(اوسط_Std_Dev, cv_hal_اوسط_Std_Dev, s, src.ڈیٹا, 0, (صحیح)src.کل(), 1, src.type(),
                      &s[0], خالی_اشاریہ /*stddev*/, نقاب.ڈیٹا, 0);
    }
    ورنہ
    {
        اگر (src.dims <= 2)
        {
            CALL_HAL_RET2(اوسط_Std_Dev, cv_hal_اوسط_Std_Dev, s, src.ڈیٹا, src.قدم, src.ستون_تعداد, src.صفیں, src.type(),
                          &s[0], خالی_اشاریہ, نقاب.ڈیٹا, نقاب.قدم);
        }
    }

    مجموع_Func func = get_مجموع_Func(گہرائی);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح کل = (صحیح)it.size, بلاک_سائز_2 = کل, partial_بلاک_سائز = 0;
    صحیح j, گنتی_2 = 0;
    صحیح _buf[CV_CN_MAX];
    صحیح* buf = (صحیح*)&s[0];
    منطقی partial_مجموع_Is_Int = گہرائی < CV_32S;
    منطقی بلاک_مجموع = partial_مجموع_Is_Int || گہرائی == CV_16F || گہرائی == CV_16BF;
    size_t esz = 0, nz0 = 0;

    اگر( بلاک_مجموع )
    {
        partial_بلاک_سائز = گہرائی <= CV_8S ? (1 << 23) : (1 << 15);
        بلاک_سائز_2 = std::min(بلاک_سائز_2, partial_بلاک_سائز);
        buf = _buf;
        برائے( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_سائز();
    }

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( j = 0; j < کل; j += بلاک_سائز_2 )
        {
            صحیح bsz = std::min(کل - j, بلاک_سائز_2);
            صحیح nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            گنتی_2 += nz;
            nz0 += nz;
            اگر( بلاک_مجموع && (گنتی_2 + بلاک_سائز_2 >= partial_بلاک_سائز || (i+1 >= it.nplanes && j+bsz >= کل)) )
            {
                اگر (partial_مجموع_Is_Int) {
                    برائے( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } ورنہ {
                    برائے( k = 0; k < cn; k++ )
                    {
                        s[k] += ((اعشاری*)buf)[k];
                        buf[k] = 0;
                    }
                }
                گنتی_2 = 0;
            }
            ptrs[0] += bsz*esz;
            اگر( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    واپس s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
ساکن دوہرا stat_گنتی(ثابت StatChunk& c)
{
    واپس c.نقاب.empty() ? (دوہرا)c.src.کل() : (دوہرا)گنتی_Non_Zero(c.نقاب);
}

Scalar اوسط_2(داخلہ_Array _src, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
    CV_Assert( نقاب.empty() || نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    اگر (src.channels() > 4 || !بانٹو_For_Parallel_Stat(src, نقاب, chunks))
        واپس اوسط_3(src, نقاب);

    ثابت صحیح n = (صحیح)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<دوہرا> cnt(n);
    parallel_for_(Range(0, n), [&](ثابت Range& r)
    {
        برائے (صحیح i = r.آغاز; i < r.end; i++)
        {
            part[i] = اوسط_3(chunks[i].src, chunks[i].نقاب);
            cnt[i] = stat_گنتی(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    دوہرا کل = 0;
    برائے (صحیح i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        کل += cnt[i];
    }
    واپس s*(کل > 0 ? 1./کل : 0);
}

ساکن مجموع_Sqr_Func get_مجموع_Sqr_Func(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_مجموع_Sqr_Func, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شدہ HAVE_OPENCL
ساکن منطقی ocl_اوسط_Std_Dev( داخلہ_Array _src, OutputArray اوسط, OutputArray _sdv, داخلہ_Array نقاب_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    منطقی have_نقاب = نقاب_2.قسم() != داخلہ_Array_2::NONE;
    صحیح nz = have_نقاب ? -1 : (صحیح)_src.کل();
    Scalar اوسط_2(0), stddev(0);
    ثابت صحیح cn = _src.channels();
    اگر (cn > 4)
        واپس جھوٹ;

    {
        صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type);
        منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_نقاب_Continuous = نقاب_2.isContinuous();
        ثابت ocl::Device &defDev = ocl::Device::getDefault();
        صحیح groups = defDev.maxComputeUnits();
        اگر (defDev.isIntel())
        {
            ساکن ثابت صحیح sub_قاش_EUCount = 10;
            groups = (groups / sub_قاش_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_گروہ_سائز();

        صحیح ddepth = std::max(CV_32S, گہرائی), sqddepth = std::max(CV_32F, گہرائی),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_نقاب || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool);

        صحیح wgs2_aligned = 1;
        جب (wgs2_aligned < (صحیح)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        اگر ( (!doubleSupport && گہرائی == CV_64F) )
            واپس جھوٹ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_مقصد_Str(type), ocl::type_مقصد_Str(گہرائی),
                             ocl::type_مقصد_Str(dtype), ocl::type_مقصد_Str(ddepth), sqddepth,
                             ocl::type_مقصد_Str(sqdtype), ocl::type_مقصد_Str(sqddepth),
                             ocl::convertTypeStr(گہرائی, sqddepth, cn, cvt[0], حجم(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_نقاب_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(گہرائی, ddepth, cn, cvt[1], حجم(cvt[1])),
                             (صحیح)wgs, wgs2_aligned, have_نقاب ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        اگر (k.empty())
            واپس جھوٹ;

        صحیح dbsize = groups * ((have_نقاب ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_سائز(sqdtype) + CV_ELEM_سائز(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), نقاب = نقاب_2.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_سائز(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_سائز(نقاب);

        اگر (have_نقاب)
            k.دلائل(srcarg, src.ستون_تعداد, (صحیح)src.کل(), groups, dbarg, maskarg);
        ورنہ
            k.دلائل(srcarg, src.ستون_تعداد, (صحیح)src.کل(), groups, dbarg);

        size_t globalsize = groups * wgs;

        اگر(!k.تسلسل(1, &globalsize, &wgs, جھوٹ))
            واپس جھوٹ;

        قسمتعریف Scalar (* part_مجموع)(Mat m);
        part_مجموع funcs[3] = { ocl_part_مجموع<صحیح>, ocl_part_مجموع<اعشاری>, ocl_part_مجموع<دوہرا> };
        Mat dbm = db.getMat(ACCESS_READ);

        اوسط_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_سائز(dtype)));

        اگر (have_نقاب)
            nz = saturate_cast<صحیح>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_سائز(dtype) +
                                                           CV_ELEM_سائز(sqdtype))))[0]);
    }

    دوہرا کل = nz != 0 ? 1.0 / nz : 0;
    صحیح k, j;
    برائے (صحیح i = 0; i < cn; ++i)
    {
        اوسط_2[i] *= کل;
        stddev[i] = std::جذر(std::max(stddev[i] * کل - اوسط_2[i] * اوسط_2[i] , 0.));
    }

    برائے( j = 0; j < 2; j++ )
    {
        ثابت دوہرا * ثابت sptr = j == 0 ? &اوسط_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? اوسط : _sdv;
        اگر( !_dst.needed() )
            جاری;

        اگر( !_dst.fixed_سائز() )
            _dst.create(cn, 1, CV_64F, -1, سچ);
        Mat dst = _dst.getMat();
        صحیح dcn = (صحیح)dst.کل();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.ستون_تعداد == 1 || dst.صفیں == 1) && dcn >= cn );
        دوہرا* dptr = dst.ptr<دوہرا>();
        برائے( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        برائے( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    واپس سچ;
}
#اگر_ختم

// serial implementation, also used for the pieces of the parallel version
ساکن خالی اوسط_Std_Dev_2(داخلہ_Array _src, OutputArray اوسط, OutputArray _sdv, داخلہ_Array نقاب_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( نقاب_2.empty() || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool );

    CV_OCL_تسلسل(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_اوسط_Std_Dev(_src, اوسط, _sdv, نقاب_2))

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();

    CV_Assert(نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src.size == نقاب.size));

    صحیح k, cn = src.channels(), گہرائی = src.گہرائی();
    Mat اوسط_mat, stddev_mat;

    اگر(اوسط.needed())
    {
        اگر( !اوسط.fixed_سائز() )
            اوسط.create(cn, 1, CV_64F, -1, سچ);

        اوسط_mat = اوسط.getMat();
        صحیح dcn = (صحیح)اوسط_mat.کل();
        CV_Assert( اوسط_mat.type() == CV_64F && اوسط_mat.isContinuous() &&
                   (اوسط_mat.ستون_تعداد == 1 || اوسط_mat.صفیں == 1) && dcn >= cn );

        دوہرا* dptr = اوسط_mat.ptr<دوہرا>();
        برائے(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    اگر (_sdv.needed())
    {
        اگر( !_sdv.fixed_سائز() )
            _sdv.create(cn, 1, CV_64F, -1, سچ);

        stddev_mat = _sdv.getMat();
        صحیح dcn = (صحیح)stddev_mat.کل();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.ستون_تعداد == 1 || stddev_mat.صفیں == 1) && dcn >= cn );

        دوہرا* dptr = stddev_mat.ptr<دوہرا>();
        برائے(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    اگر (src.isContinuous() && نقاب.isContinuous())
    {
        CALL_HAL(اوسط_Std_Dev, cv_hal_اوسط_Std_Dev, src.ڈیٹا, 0, (صحیح)src.کل(), 1, src.type(),
                 اوسط.needed() ? اوسط_mat.ptr<دوہرا>() : خالی_اشاریہ,
                 _sdv.needed() ? stddev_mat.ptr<دوہرا>() : خالی_اشاریہ,
                 نقاب.ڈیٹا, 0);
    }
    ورنہ
    {
        اگر (src.dims <= 2)
        {
            CALL_HAL(اوسط_Std_Dev, cv_hal_اوسط_Std_Dev, src.ڈیٹا, src.قدم, src.ستون_تعداد, src.صفیں, src.type(),
                     اوسط.needed() ? اوسط_mat.ptr<دوہرا>() : خالی_اشاریہ,
                     _sdv.needed() ? stddev_mat.ptr<دوہرا>() : خالی_اشاریہ,
                     نقاب.ڈیٹا, نقاب.قدم);
        }
    }

    مجموع_Sqr_Func func = get_مجموع_Sqr_Func(گہرائی);

    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح کل = (صحیح)it.size, بلاک_سائز_2 = کل, partial_بلاک_سائز = 0;
    صحیح j;
    int64_t گنتی_2 = 0, nz0 = 0;
    دوہرا _buf[CV_CN_MAX*4];
    دوہرا *s = _buf, *sq = s + cn;
    صحیح *sbuf = (صحیح*)s, *sqbuf = (صحیح*)sq;
    منطقی partial_مجموع_Is_Int = گہرائی < CV_32S;
    منطقی بلاک_مجموع = partial_مجموع_Is_Int || گہرائی == CV_16F || گہرائی == CV_16BF;
    منطقی بلاک_Sq_مجموع = گہرائی <= CV_8S;
    size_t esz = 0;

    برائے( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    اگر( بلاک_مجموع )
    {
        partial_بلاک_سائز = 1 << 15;
        بلاک_سائز_2 = std::min(بلاک_سائز_2, partial_بلاک_سائز);
        sbuf = (صحیح*)(sq + cn);
        اگر( بلاک_Sq_مجموع )
            sqbuf = sbuf + cn;
        برائے( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_سائز();
    }

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( j = 0; j < کل; j += بلاک_سائز_2 )
        {
            صحیح bsz = std::min(کل - j, بلاک_سائز_2);
            صحیح nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            گنتی_2 += nz;
            nz0 += nz;
            اگر( بلاک_مجموع && (گنتی_2 + بلاک_سائز_2 >= partial_بلاک_سائز || (i+1 >= it.nplanes && j+bsz >= کل)) )
            {
                اگر (partial_مجموع_Is_Int) {
                    برائے( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } ورنہ {
                    برائے( k = 0; k < cn; k++ )
                    {
                        s[k] += ((اعشاری*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                اگر( بلاک_Sq_مجموع )
                {
                    برائے( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                گنتی_2 = 0;
            }
            ptrs[0] += bsz*esz;
            اگر( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    دوہرا scale = nz0 ? 1./nz0 : 0.;
    برائے( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::جذر(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    اگر (اوسط.needed())
    {
        ثابت دوہرا* sptr = s;
        دوہرا* dptr = اوسط_mat.ptr<دوہرا>();
        برائے( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    اگر (_sdv.needed())
    {
        ثابت دوہرا* sptr = sq;
        دوہرا* dptr = stddev_mat.ptr<دوہرا>();
        برائے( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

ساکن خالی write_Stat_نتیجہ(OutputArray _dst, صحیح cn, ثابت Scalar& v)
{
    اگر (!_dst.needed())
        واپس;
    اگر (!_dst.fixed_سائز())
        _dst.create(cn, 1, CV_64F, -1, سچ);
    Mat d = _dst.getMat();
    صحیح dcn = (صحیح)d.کل();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.ستون_تعداد == 1 || d.صفیں == 1) && dcn >= cn );
    دوہرا* p = d.ptr<دوہرا>();
    برائے (صحیح k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

خالی اوسط_Std_Dev(داخلہ_Array _src, OutputArray اوسط, OutputArray _sdv, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    اگر (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
        ثابت صحیح cn = src.channels();
        ثابت منطقی نقاب_Ok = نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) &&
                                             src.size == نقاب.size);
        اگر (cn <= 4 && نقاب_Ok && بانٹو_For_Parallel_Stat(src, نقاب, chunks))
        {
            ثابت صحیح n = (صحیح)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<دوہرا> cnt(n);
            parallel_for_(Range(0, n), [&](ثابت Range& r)
            {
                برائے (صحیح i = r.آغاز; i < r.end; i++)
                {
                    Mat mi, si;
                    اوسط_Std_Dev_2(chunks[i].src, mi, si, chunks[i].نقاب);
                    برائے (صحیح k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<دوہرا>(k);
                        sd[i][k] = si.at<دوہرا>(k);
                    }
                    cnt[i] = stat_گنتی(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            دوہرا کل = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            برائے (صحیح i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                کل += cnt[i];
            }
            اگر (کل > 0)
                M *= 1./کل;
            برائے (صحیح i = 0; i < n; i++)
                برائے (صحیح k = 0; k < cn; k++)
                {
                    دوہرا dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            برائے (صحیح k = 0; k < cn; k++)
                SD[k] = کل > 0 ? std::جذر(std::max(V[k]/کل, 0.)) : 0;
            write_Stat_نتیجہ(اوسط, cn, M);
            write_Stat_نتیجہ(_sdv, cn, SD);
            واپس;
        }
    }
    اوسط_Std_Dev_2(_src, اوسط, _sdv, نقاب_2);
}

} // namespace
