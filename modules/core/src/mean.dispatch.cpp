// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "stat.hpp"
#అన్తర్భావ "stat_parallel.hpp"

#అన్తర్భావ "mean.simd.hpp"
#అన్తర్భావ "mean.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content


నామాకాశ cv {

// serial implementation, also used for the pieces of the parallel version
స్థిర Scalar సగటు_3(స్థిరరాశి Mat& src, స్థిరరాశి Mat& ముసుగు)
{
    CV_Assert( ముసుగు.empty() || ((ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool) && src.size == ముసుగు.size) );
    ఒకవేళ (src.empty())
        తిరిగి Scalar::all(0.0);

    పూర్ణాంకం k, cn = src.channels(), లోతు = src.లోతు();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    ఒకవేళ (src.isContinuous() && ముసుగు.isContinuous())
    {
        CALL_HAL_RET2(సగటు_Std_Dev, cv_hal_సగటు_Std_Dev, s, src.దత్తాంశం, 0, (పూర్ణాంకం)src.మొత్తం(), 1, src.type(),
                      &s[0], శూన్యసూచిక /*stddev*/, ముసుగు.దత్తాంశం, 0);
    }
    లేకపోతే
    {
        ఒకవేళ (src.dims <= 2)
        {
            CALL_HAL_RET2(సగటు_Std_Dev, cv_hal_సగటు_Std_Dev, s, src.దత్తాంశం, src.అడుగు, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, src.type(),
                          &s[0], శూన్యసూచిక, ముసుగు.దత్తాంశం, ముసుగు.అడుగు);
        }
    }

    కూడిక_Func func = get_కూడిక_Func(లోతు);

    CV_Assert( func != 0 );

    స్థిరరాశి Mat* arrays[] = {&src, &ముసుగు, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size, ఖండం_కొలత_2 = మొత్తం, partial_ఖండం_కొలత = 0;
    పూర్ణాంకం j, లెక్క_2 = 0;
    పూర్ణాంకం _buf[CV_CN_MAX];
    పూర్ణాంకం* buf = (పూర్ణాంకం*)&s[0];
    తార్కిక partial_కూడిక_Is_Int = లోతు < CV_32S;
    తార్కిక ఖండం_కూడిక = partial_కూడిక_Is_Int || లోతు == CV_16F || లోతు == CV_16BF;
    size_t esz = 0, nz0 = 0;

    ఒకవేళ( ఖండం_కూడిక )
    {
        partial_ఖండం_కొలత = లోతు <= CV_8S ? (1 << 23) : (1 << 15);
        ఖండం_కొలత_2 = std::min(ఖండం_కొలత_2, partial_ఖండం_కొలత);
        buf = _buf;
        కొరకు( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_కొలత();
    }

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( j = 0; j < మొత్తం; j += ఖండం_కొలత_2 )
        {
            పూర్ణాంకం bsz = std::min(మొత్తం - j, ఖండం_కొలత_2);
            పూర్ణాంకం nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            లెక్క_2 += nz;
            nz0 += nz;
            ఒకవేళ( ఖండం_కూడిక && (లెక్క_2 + ఖండం_కొలత_2 >= partial_ఖండం_కొలత || (i+1 >= it.nplanes && j+bsz >= మొత్తం)) )
            {
                ఒకవేళ (partial_కూడిక_Is_Int) {
                    కొరకు( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } లేకపోతే {
                    కొరకు( k = 0; k < cn; k++ )
                    {
                        s[k] += ((దశాంశం*)buf)[k];
                        buf[k] = 0;
                    }
                }
                లెక్క_2 = 0;
            }
            ptrs[0] += bsz*esz;
            ఒకవేళ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    తిరిగి s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
స్థిర ద్విగుణ stat_లెక్క(స్థిరరాశి StatChunk& c)
{
    తిరిగి c.ముసుగు.empty() ? (ద్విగుణ)c.src.మొత్తం() : (ద్విగుణ)లెక్క_Non_Zero(c.ముసుగు);
}

Scalar సగటు_2(ఆదానం_Array _src, ఆదానం_Array ముసుగు_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), ముసుగు = ముసుగు_2.getMat();
    CV_Assert( ముసుగు.empty() || ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    ఒకవేళ (src.channels() > 4 || !విభజించు_For_Parallel_Stat(src, ముసుగు, chunks))
        తిరిగి సగటు_3(src, ముసుగు);

    స్థిరరాశి పూర్ణాంకం n = (పూర్ణాంకం)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<ద్విగుణ> cnt(n);
    parallel_for_(Range(0, n), [&](స్థిరరాశి Range& r)
    {
        కొరకు (పూర్ణాంకం i = r.ప్రారంభం; i < r.end; i++)
        {
            part[i] = సగటు_3(chunks[i].src, chunks[i].ముసుగు);
            cnt[i] = stat_లెక్క(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    ద్విగుణ మొత్తం = 0;
    కొరకు (పూర్ణాంకం i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        మొత్తం += cnt[i];
    }
    తిరిగి s*(మొత్తం > 0 ? 1./మొత్తం : 0);
}

స్థిర కూడిక_Sqr_Func get_కూడిక_Sqr_Func(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_కూడిక_Sqr_Func, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక ocl_సగటు_Std_Dev( ఆదానం_Array _src, OutputArray సగటు, OutputArray _sdv, ఆదానం_Array ముసుగు_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    తార్కిక have_ముసుగు = ముసుగు_2.రకం() != ఆదానం_Array_2::NONE;
    పూర్ణాంకం nz = have_ముసుగు ? -1 : (పూర్ణాంకం)_src.మొత్తం();
    Scalar సగటు_2(0), stddev(0);
    స్థిరరాశి పూర్ణాంకం cn = _src.channels();
    ఒకవేళ (cn > 4)
        తిరిగి అబద్ధం;

    {
        పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type);
        తార్కిక doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_ముసుగు_Continuous = ముసుగు_2.isContinuous();
        స్థిరరాశి ocl::Device &defDev = ocl::Device::getDefault();
        పూర్ణాంకం groups = defDev.maxComputeUnits();
        ఒకవేళ (defDev.isIntel())
        {
            స్థిర స్థిరరాశి పూర్ణాంకం sub_ముక్క_EUCount = 10;
            groups = (groups / sub_ముక్క_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_సమూహం_కొలత();

        పూర్ణాంకం ddepth = std::max(CV_32S, లోతు), sqddepth = std::max(CV_32F, లోతు),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_ముసుగు || ముసుగు_2.type() == CV_8U || ముసుగు_2.type() == CV_8S || ముసుగు_2.type() == CV_Bool);

        పూర్ణాంకం wgs2_aligned = 1;
        అయితే (wgs2_aligned < (పూర్ణాంకం)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        ఒకవేళ ( (!doubleSupport && లోతు == CV_64F) )
            తిరిగి అబద్ధం;

        అక్షరం cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_గమ్యం_Str(type), ocl::type_గమ్యం_Str(లోతు),
                             ocl::type_గమ్యం_Str(dtype), ocl::type_గమ్యం_Str(ddepth), sqddepth,
                             ocl::type_గమ్యం_Str(sqdtype), ocl::type_గమ్యం_Str(sqddepth),
                             ocl::convertTypeStr(లోతు, sqddepth, cn, cvt[0], పరిమాణం(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_ముసుగు_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(లోతు, ddepth, cn, cvt[1], పరిమాణం(cvt[1])),
                             (పూర్ణాంకం)wgs, wgs2_aligned, have_ముసుగు ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        ఒకవేళ (k.empty())
            తిరిగి అబద్ధం;

        పూర్ణాంకం dbsize = groups * ((have_ముసుగు ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_కొలత(sqdtype) + CV_ELEM_కొలత(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), ముసుగు = ముసుగు_2.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_కొలత(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_కొలత(ముసుగు);

        ఒకవేళ (have_ముసుగు)
            k.వాదనలు(srcarg, src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(), groups, dbarg, maskarg);
        లేకపోతే
            k.వాదనలు(srcarg, src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(), groups, dbarg);

        size_t globalsize = groups * wgs;

        ఒకవేళ(!k.నిరంతరం(1, &globalsize, &wgs, అబద్ధం))
            తిరిగి అబద్ధం;

        రకనిర్వచనం Scalar (* part_కూడిక)(Mat m);
        part_కూడిక funcs[3] = { ocl_part_కూడిక<పూర్ణాంకం>, ocl_part_కూడిక<దశాంశం>, ocl_part_కూడిక<ద్విగుణ> };
        Mat dbm = db.getMat(ACCESS_READ);

        సగటు_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_కొలత(dtype)));

        ఒకవేళ (have_ముసుగు)
            nz = saturate_cast<పూర్ణాంకం>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_కొలత(dtype) +
                                                           CV_ELEM_కొలత(sqdtype))))[0]);
    }

    ద్విగుణ మొత్తం = nz != 0 ? 1.0 / nz : 0;
    పూర్ణాంకం k, j;
    కొరకు (పూర్ణాంకం i = 0; i < cn; ++i)
    {
        సగటు_2[i] *= మొత్తం;
        stddev[i] = std::వర్గమూలం(std::max(stddev[i] * మొత్తం - సగటు_2[i] * సగటు_2[i] , 0.));
    }

    కొరకు( j = 0; j < 2; j++ )
    {
        స్థిరరాశి ద్విగుణ * స్థిరరాశి sptr = j == 0 ? &సగటు_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? సగటు : _sdv;
        ఒకవేళ( !_dst.needed() )
            కొనసాగు;

        ఒకవేళ( !_dst.fixed_కొలత() )
            _dst.create(cn, 1, CV_64F, -1, నిజం);
        Mat dst = _dst.getMat();
        పూర్ణాంకం dcn = (పూర్ణాంకం)dst.మొత్తం();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.నిలువుల_సంఖ్య == 1 || dst.అడ్డువరుసలు == 1) && dcn >= cn );
        ద్విగుణ* dptr = dst.ptr<ద్విగుణ>();
        కొరకు( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        కొరకు( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    తిరిగి నిజం;
}
#అయితే_ముగింపు

// serial implementation, also used for the pieces of the parallel version
స్థిర శూన్య సగటు_Std_Dev_2(ఆదానం_Array _src, OutputArray సగటు, OutputArray _sdv, ఆదానం_Array ముసుగు_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( ముసుగు_2.empty() || ముసుగు_2.type() == CV_8U || ముసుగు_2.type() == CV_8S || ముసుగు_2.type() == CV_Bool );

    CV_OCL_నిరంతరం(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_సగటు_Std_Dev(_src, సగటు, _sdv, ముసుగు_2))

    Mat src = _src.getMat(), ముసుగు = ముసుగు_2.getMat();

    CV_Assert(ముసుగు.empty() || ((ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool) && src.size == ముసుగు.size));

    పూర్ణాంకం k, cn = src.channels(), లోతు = src.లోతు();
    Mat సగటు_mat, stddev_mat;

    ఒకవేళ(సగటు.needed())
    {
        ఒకవేళ( !సగటు.fixed_కొలత() )
            సగటు.create(cn, 1, CV_64F, -1, నిజం);

        సగటు_mat = సగటు.getMat();
        పూర్ణాంకం dcn = (పూర్ణాంకం)సగటు_mat.మొత్తం();
        CV_Assert( సగటు_mat.type() == CV_64F && సగటు_mat.isContinuous() &&
                   (సగటు_mat.నిలువుల_సంఖ్య == 1 || సగటు_mat.అడ్డువరుసలు == 1) && dcn >= cn );

        ద్విగుణ* dptr = సగటు_mat.ptr<ద్విగుణ>();
        కొరకు(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    ఒకవేళ (_sdv.needed())
    {
        ఒకవేళ( !_sdv.fixed_కొలత() )
            _sdv.create(cn, 1, CV_64F, -1, నిజం);

        stddev_mat = _sdv.getMat();
        పూర్ణాంకం dcn = (పూర్ణాంకం)stddev_mat.మొత్తం();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.నిలువుల_సంఖ్య == 1 || stddev_mat.అడ్డువరుసలు == 1) && dcn >= cn );

        ద్విగుణ* dptr = stddev_mat.ptr<ద్విగుణ>();
        కొరకు(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    ఒకవేళ (src.isContinuous() && ముసుగు.isContinuous())
    {
        CALL_HAL(సగటు_Std_Dev, cv_hal_సగటు_Std_Dev, src.దత్తాంశం, 0, (పూర్ణాంకం)src.మొత్తం(), 1, src.type(),
                 సగటు.needed() ? సగటు_mat.ptr<ద్విగుణ>() : శూన్యసూచిక,
                 _sdv.needed() ? stddev_mat.ptr<ద్విగుణ>() : శూన్యసూచిక,
                 ముసుగు.దత్తాంశం, 0);
    }
    లేకపోతే
    {
        ఒకవేళ (src.dims <= 2)
        {
            CALL_HAL(సగటు_Std_Dev, cv_hal_సగటు_Std_Dev, src.దత్తాంశం, src.అడుగు, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, src.type(),
                     సగటు.needed() ? సగటు_mat.ptr<ద్విగుణ>() : శూన్యసూచిక,
                     _sdv.needed() ? stddev_mat.ptr<ద్విగుణ>() : శూన్యసూచిక,
                     ముసుగు.దత్తాంశం, ముసుగు.అడుగు);
        }
    }

    కూడిక_Sqr_Func func = get_కూడిక_Sqr_Func(లోతు);

    CV_Assert( func != 0 );

    స్థిరరాశి Mat* arrays[] = {&src, &ముసుగు, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size, ఖండం_కొలత_2 = మొత్తం, partial_ఖండం_కొలత = 0;
    పూర్ణాంకం j;
    int64_t లెక్క_2 = 0, nz0 = 0;
    ద్విగుణ _buf[CV_CN_MAX*4];
    ద్విగుణ *s = _buf, *sq = s + cn;
    పూర్ణాంకం *sbuf = (పూర్ణాంకం*)s, *sqbuf = (పూర్ణాంకం*)sq;
    తార్కిక partial_కూడిక_Is_Int = లోతు < CV_32S;
    తార్కిక ఖండం_కూడిక = partial_కూడిక_Is_Int || లోతు == CV_16F || లోతు == CV_16BF;
    తార్కిక ఖండం_Sq_కూడిక = లోతు <= CV_8S;
    size_t esz = 0;

    కొరకు( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    ఒకవేళ( ఖండం_కూడిక )
    {
        partial_ఖండం_కొలత = 1 << 15;
        ఖండం_కొలత_2 = std::min(ఖండం_కొలత_2, partial_ఖండం_కొలత);
        sbuf = (పూర్ణాంకం*)(sq + cn);
        ఒకవేళ( ఖండం_Sq_కూడిక )
            sqbuf = sbuf + cn;
        కొరకు( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_కొలత();
    }

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( j = 0; j < మొత్తం; j += ఖండం_కొలత_2 )
        {
            పూర్ణాంకం bsz = std::min(మొత్తం - j, ఖండం_కొలత_2);
            పూర్ణాంకం nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            లెక్క_2 += nz;
            nz0 += nz;
            ఒకవేళ( ఖండం_కూడిక && (లెక్క_2 + ఖండం_కొలత_2 >= partial_ఖండం_కొలత || (i+1 >= it.nplanes && j+bsz >= మొత్తం)) )
            {
                ఒకవేళ (partial_కూడిక_Is_Int) {
                    కొరకు( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } లేకపోతే {
                    కొరకు( k = 0; k < cn; k++ )
                    {
                        s[k] += ((దశాంశం*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                ఒకవేళ( ఖండం_Sq_కూడిక )
                {
                    కొరకు( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                లెక్క_2 = 0;
            }
            ptrs[0] += bsz*esz;
            ఒకవేళ( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    ద్విగుణ scale = nz0 ? 1./nz0 : 0.;
    కొరకు( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::వర్గమూలం(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    ఒకవేళ (సగటు.needed())
    {
        స్థిరరాశి ద్విగుణ* sptr = s;
        ద్విగుణ* dptr = సగటు_mat.ptr<ద్విగుణ>();
        కొరకు( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    ఒకవేళ (_sdv.needed())
    {
        స్థిరరాశి ద్విగుణ* sptr = sq;
        ద్విగుణ* dptr = stddev_mat.ptr<ద్విగుణ>();
        కొరకు( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

స్థిర శూన్య write_Stat_ఫలితం(OutputArray _dst, పూర్ణాంకం cn, స్థిరరాశి Scalar& v)
{
    ఒకవేళ (!_dst.needed())
        తిరిగి;
    ఒకవేళ (!_dst.fixed_కొలత())
        _dst.create(cn, 1, CV_64F, -1, నిజం);
    Mat d = _dst.getMat();
    పూర్ణాంకం dcn = (పూర్ణాంకం)d.మొత్తం();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.నిలువుల_సంఖ్య == 1 || d.అడ్డువరుసలు == 1) && dcn >= cn );
    ద్విగుణ* p = d.ptr<ద్విగుణ>();
    కొరకు (పూర్ణాంకం k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

శూన్య సగటు_Std_Dev(ఆదానం_Array _src, OutputArray సగటు, OutputArray _sdv, ఆదానం_Array ముసుగు_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    ఒకవేళ (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), ముసుగు = ముసుగు_2.getMat();
        స్థిరరాశి పూర్ణాంకం cn = src.channels();
        స్థిరరాశి తార్కిక ముసుగు_Ok = ముసుగు.empty() || ((ముసుగు.type() == CV_8U || ముసుగు.type() == CV_8S || ముసుగు.type() == CV_Bool) &&
                                             src.size == ముసుగు.size);
        ఒకవేళ (cn <= 4 && ముసుగు_Ok && విభజించు_For_Parallel_Stat(src, ముసుగు, chunks))
        {
            స్థిరరాశి పూర్ణాంకం n = (పూర్ణాంకం)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<ద్విగుణ> cnt(n);
            parallel_for_(Range(0, n), [&](స్థిరరాశి Range& r)
            {
                కొరకు (పూర్ణాంకం i = r.ప్రారంభం; i < r.end; i++)
                {
                    Mat mi, si;
                    సగటు_Std_Dev_2(chunks[i].src, mi, si, chunks[i].ముసుగు);
                    కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<ద్విగుణ>(k);
                        sd[i][k] = si.at<ద్విగుణ>(k);
                    }
                    cnt[i] = stat_లెక్క(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            ద్విగుణ మొత్తం = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            కొరకు (పూర్ణాంకం i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                మొత్తం += cnt[i];
            }
            ఒకవేళ (మొత్తం > 0)
                M *= 1./మొత్తం;
            కొరకు (పూర్ణాంకం i = 0; i < n; i++)
                కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                {
                    ద్విగుణ dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            కొరకు (పూర్ణాంకం k = 0; k < cn; k++)
                SD[k] = మొత్తం > 0 ? std::వర్గమూలం(std::max(V[k]/మొత్తం, 0.)) : 0;
            write_Stat_ఫలితం(సగటు, cn, M);
            write_Stat_ఫలితం(_sdv, cn, SD);
            తిరిగి;
        }
    }
    సగటు_Std_Dev_2(_src, సగటు, _sdv, ముసుగు_2);
}

} // namespace
