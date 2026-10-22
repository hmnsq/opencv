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
स्थिर Scalar माध्यम्_3(ध्रुव Mat& src, ध्रुव Mat& आवरणम्)
{
    CV_Assert( आवरणम्.empty() || ((आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool) && src.size == आवरणम्.size) );
    यदि (src.empty())
        प्रत्यागम Scalar::all(0.0);

    पूर्णाङ्क k, cn = src.channels(), गभीरता = src.गभीरता();
    Scalar s = Scalar::all(0.0);

    CV_Assert( cn <= 4 );

    यदि (src.isContinuous() && आवरणम्.isContinuous())
    {
        CALL_HAL_RET2(माध्यम्_Std_Dev, cv_hal_माध्यम्_Std_Dev, s, src.दत्त, 0, (पूर्णाङ्क)src.योग(), 1, src.type(),
                      &s[0], शून्यसूचकः /*stddev*/, आवरणम्.दत्त, 0);
    }
    अन्यथा
    {
        यदि (src.dims <= 2)
        {
            CALL_HAL_RET2(माध्यम्_Std_Dev, cv_hal_माध्यम्_Std_Dev, s, src.दत्त, src.चरणः, src.स्तम्भसङ्ख्या, src.पङ्क्तिकाः, src.type(),
                          &s[0], शून्यसूचकः, आवरणम्.दत्त, आवरणम्.चरणः);
        }
    }

    योग_Func func = get_योग_Func(गभीरता);

    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, &आवरणम्, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क योग = (पूर्णाङ्क)it.size, खण्डम्_आकार_2 = योग, partial_खण्डम्_आकार = 0;
    पूर्णाङ्क j, गणनम्_2 = 0;
    पूर्णाङ्क _buf[CV_CN_MAX];
    पूर्णाङ्क* buf = (पूर्णाङ्क*)&s[0];
    तार्किक partial_योग_Is_Int = गभीरता < CV_32S;
    तार्किक खण्डम्_योग = partial_योग_Is_Int || गभीरता == CV_16F || गभीरता == CV_16BF;
    size_t esz = 0, nz0 = 0;

    यदि( खण्डम्_योग )
    {
        partial_खण्डम्_आकार = गभीरता <= CV_8S ? (1 << 23) : (1 << 15);
        खण्डम्_आकार_2 = std::min(खण्डम्_आकार_2, partial_खण्डम्_आकार);
        buf = _buf;
        प्रति( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_आकार();
    }

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( j = 0; j < योग; j += खण्डम्_आकार_2 )
        {
            पूर्णाङ्क bsz = std::min(योग - j, खण्डम्_आकार_2);
            पूर्णाङ्क nz = func( ptrs[0], ptrs[1], (uchar*)buf, bsz, cn );
            गणनम्_2 += nz;
            nz0 += nz;
            यदि( खण्डम्_योग && (गणनम्_2 + खण्डम्_आकार_2 >= partial_खण्डम्_आकार || (i+1 >= it.nplanes && j+bsz >= योग)) )
            {
                यदि (partial_योग_Is_Int) {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        s[k] += buf[k];
                        buf[k] = 0;
                    }
                } अन्यथा {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        s[k] += ((भिन्न*)buf)[k];
                        buf[k] = 0;
                    }
                }
                गणनम्_2 = 0;
            }
            ptrs[0] += bsz*esz;
            यदि( ptrs[1] )
                ptrs[1] += bsz;
        }
    }
    प्रत्यागम s*(nz0 ? 1./nz0 : 0);
}

// number of elements a piece contributes to the mean
स्थिर द्विगुण stat_गणनम्(ध्रुव StatChunk& c)
{
    प्रत्यागम c.आवरणम्.empty() ? (द्विगुण)c.src.योग() : (द्विगुण)गणनम्_Non_Zero(c.आवरणम्);
}

Scalar माध्यम्_2(निवेशः_Array _src, निवेशः_Array आवरणम्_2)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), आवरणम् = आवरणम्_2.getMat();
    CV_Assert( आवरणम्.empty() || आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool);

    std::vector<StatChunk> chunks;
    यदि (src.channels() > 4 || !विभज_For_Parallel_Stat(src, आवरणम्, chunks))
        प्रत्यागम माध्यम्_3(src, आवरणम्);

    ध्रुव पूर्णाङ्क n = (पूर्णाङ्क)chunks.size();
    std::vector<Scalar> part(n);
    std::vector<द्विगुण> cnt(n);
    parallel_for_(Range(0, n), [&](ध्रुव Range& r)
    {
        प्रति (पूर्णाङ्क i = r.start; i < r.end; i++)
        {
            part[i] = माध्यम्_3(chunks[i].src, chunks[i].आवरणम्);
            cnt[i] = stat_गणनम्(chunks[i]);
        }
    });
    Scalar s = Scalar::all(0.0);
    द्विगुण योग = 0;
    प्रति (पूर्णाङ्क i = 0; i < n; i++)
    {
        s += part[i]*cnt[i];
        योग += cnt[i];
    }
    प्रत्यागम s*(योग > 0 ? 1./योग : 0);
}

स्थिर योग_Sqr_Func get_योग_Sqr_Func(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_योग_Sqr_Func, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
}

#यदि_परिभाषितम् HAVE_OPENCL
स्थिर तार्किक ocl_माध्यम्_Std_Dev( निवेशः_Array _src, OutputArray माध्यम्, OutputArray _sdv, निवेशः_Array आवरणम्_2 )
{
    CV_INSTRUMENT_REGION_OPENCL();

    तार्किक have_आवरणम् = आवरणम्_2.विधा() != निवेशः_Array_2::NONE;
    पूर्णाङ्क nz = have_आवरणम् ? -1 : (पूर्णाङ्क)_src.योग();
    Scalar माध्यम्_2(0), stddev(0);
    ध्रुव पूर्णाङ्क cn = _src.channels();
    यदि (cn > 4)
        प्रत्यागम असत्य;

    {
        पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type);
        तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0,
                isContinuous = _src.isContinuous(),
                is_आवरणम्_Continuous = आवरणम्_2.isContinuous();
        ध्रुव ocl::Device &defDev = ocl::Device::getDefault();
        पूर्णाङ्क groups = defDev.maxComputeUnits();
        यदि (defDev.isIntel())
        {
            स्थिर ध्रुव पूर्णाङ्क sub_खण्डिका_EUCount = 10;
            groups = (groups / sub_खण्डिका_EUCount) * 2;
        }
        size_t wgs = defDev.max_Work_गणः_आकार();

        पूर्णाङ्क ddepth = std::max(CV_32S, गभीरता), sqddepth = std::max(CV_32F, गभीरता),
                dtype = CV_MAKE_TYPE(ddepth, cn),
                sqdtype = CV_MAKETYPE(sqddepth, cn);
        CV_Assert(!have_आवरणम् || आवरणम्_2.type() == CV_8U || आवरणम्_2.type() == CV_8S || आवरणम्_2.type() == CV_Bool);

        पूर्णाङ्क wgs2_aligned = 1;
        यावत् (wgs2_aligned < (पूर्णाङ्क)wgs)
            wgs2_aligned <<= 1;
        wgs2_aligned >>= 1;

        यदि ( (!doubleSupport && गभीरता == CV_64F) )
            प्रत्यागम असत्य;

        वर्ण cvt[2][50];
        String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstT1=%s -D sqddepth=%d"
                             " -D sqdstT=%s -D sqdstT1=%s -D convertToSDT=%s -D cn=%d%s%s"
                             " -D convertToDT=%s -D WGS=%d -D WGS2_ALIGNED=%d%s%s",
                             ocl::type_गन्तव्यम्_Str(type), ocl::type_गन्तव्यम्_Str(गभीरता),
                             ocl::type_गन्तव्यम्_Str(dtype), ocl::type_गन्तव्यम्_Str(ddepth), sqddepth,
                             ocl::type_गन्तव्यम्_Str(sqdtype), ocl::type_गन्तव्यम्_Str(sqddepth),
                             ocl::convertTypeStr(गभीरता, sqddepth, cn, cvt[0], परिमाण(cvt[0])),
                             cn, isContinuous ? " -D HAVE_SRC_CONT" : "",
                             is_आवरणम्_Continuous ? " -D HAVE_MASK_CONT" : "",
                             ocl::convertTypeStr(गभीरता, ddepth, cn, cvt[1], परिमाण(cvt[1])),
                             (पूर्णाङ्क)wgs, wgs2_aligned, have_आवरणम् ? " -D HAVE_MASK" : "",
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("meanStdDev", ocl::core::meanstddev_oclsrc, opts);
        यदि (k.empty())
            प्रत्यागम असत्य;

        पूर्णाङ्क dbsize = groups * ((have_आवरणम् ? CV_ELEM_SIZE1(CV_32S) : 0) +
                               CV_ELEM_आकार(sqdtype) + CV_ELEM_आकार(dtype));
        UMat src = _src.getUMat(), db(1, dbsize, CV_8UC1), आवरणम् = आवरणम्_2.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
                dbarg = ocl::KernelArg::PtrWriteOnly(db),
                maskarg = ocl::KernelArg::Read_Only_No_आकार(आवरणम्);

        यदि (have_आवरणम्)
            k.तर्काः(srcarg, src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(), groups, dbarg, maskarg);
        अन्यथा
            k.तर्काः(srcarg, src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(), groups, dbarg);

        size_t globalsize = groups * wgs;

        यदि(!k.सततम्(1, &globalsize, &wgs, असत्य))
            प्रत्यागम असत्य;

        प्रकारनिर्देश Scalar (* part_योग)(Mat m);
        part_योग funcs[3] = { ocl_part_योग<पूर्णाङ्क>, ocl_part_योग<भिन्न>, ocl_part_योग<द्विगुण> };
        Mat dbm = db.getMat(ACCESS_READ);

        माध्यम्_2 = funcs[ddepth - CV_32S](Mat(1, groups, dtype, dbm.ptr()));
        stddev = funcs[sqddepth - CV_32S](Mat(1, groups, sqdtype, dbm.ptr() + groups * CV_ELEM_आकार(dtype)));

        यदि (have_आवरणम्)
            nz = saturate_cast<पूर्णाङ्क>(funcs[0](Mat(1, groups, CV_32SC1, dbm.ptr() +
                                                 groups * (CV_ELEM_आकार(dtype) +
                                                           CV_ELEM_आकार(sqdtype))))[0]);
    }

    द्विगुण योग = nz != 0 ? 1.0 / nz : 0;
    पूर्णाङ्क k, j;
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
    {
        माध्यम्_2[i] *= योग;
        stddev[i] = std::वर्गमूलम्(std::max(stddev[i] * योग - माध्यम्_2[i] * माध्यम्_2[i] , 0.));
    }

    प्रति( j = 0; j < 2; j++ )
    {
        ध्रुव द्विगुण * ध्रुव sptr = j == 0 ? &माध्यम्_2[0] : &stddev[0];
        _OutputArray _dst = j == 0 ? माध्यम् : _sdv;
        यदि( !_dst.needed() )
            अनुवर्त;

        यदि( !_dst.fixed_आकार() )
            _dst.create(cn, 1, CV_64F, -1, सत्य);
        Mat dst = _dst.getMat();
        पूर्णाङ्क dcn = (पूर्णाङ्क)dst.योग();
        CV_Assert( dst.type() == CV_64F && dst.isContinuous() &&
                   (dst.स्तम्भसङ्ख्या == 1 || dst.पङ्क्तिकाः == 1) && dcn >= cn );
        द्विगुण* dptr = dst.ptr<द्विगुण>();
        प्रति( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
        प्रति( ; k < dcn; k++ )
            dptr[k] = 0;
    }

    प्रत्यागम सत्य;
}
#यदि_अन्तः

// serial implementation, also used for the pieces of the parallel version
स्थिर शून्य माध्यम्_Std_Dev_2(निवेशः_Array _src, OutputArray माध्यम्, OutputArray _sdv, निवेशः_Array आवरणम्_2)
{
    CV_Assert(!_src.empty());
    CV_Assert( आवरणम्_2.empty() || आवरणम्_2.type() == CV_8U || आवरणम्_2.type() == CV_8S || आवरणम्_2.type() == CV_Bool );

    CV_OCL_सततम्(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
               ocl_माध्यम्_Std_Dev(_src, माध्यम्, _sdv, आवरणम्_2))

    Mat src = _src.getMat(), आवरणम् = आवरणम्_2.getMat();

    CV_Assert(आवरणम्.empty() || ((आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool) && src.size == आवरणम्.size));

    पूर्णाङ्क k, cn = src.channels(), गभीरता = src.गभीरता();
    Mat माध्यम्_mat, stddev_mat;

    यदि(माध्यम्.needed())
    {
        यदि( !माध्यम्.fixed_आकार() )
            माध्यम्.create(cn, 1, CV_64F, -1, सत्य);

        माध्यम्_mat = माध्यम्.getMat();
        पूर्णाङ्क dcn = (पूर्णाङ्क)माध्यम्_mat.योग();
        CV_Assert( माध्यम्_mat.type() == CV_64F && माध्यम्_mat.isContinuous() &&
                   (माध्यम्_mat.स्तम्भसङ्ख्या == 1 || माध्यम्_mat.पङ्क्तिकाः == 1) && dcn >= cn );

        द्विगुण* dptr = माध्यम्_mat.ptr<द्विगुण>();
        प्रति(k = cn ; k < dcn; k++ )
            dptr[k] = 0;
    }

    यदि (_sdv.needed())
    {
        यदि( !_sdv.fixed_आकार() )
            _sdv.create(cn, 1, CV_64F, -1, सत्य);

        stddev_mat = _sdv.getMat();
        पूर्णाङ्क dcn = (पूर्णाङ्क)stddev_mat.योग();
        CV_Assert( stddev_mat.type() == CV_64F && stddev_mat.isContinuous() &&
                   (stddev_mat.स्तम्भसङ्ख्या == 1 || stddev_mat.पङ्क्तिकाः == 1) && dcn >= cn );

        द्विगुण* dptr = stddev_mat.ptr<द्विगुण>();
        प्रति(k = cn ; k < dcn; k++ )
            dptr[k] = 0;

    }

    यदि (src.isContinuous() && आवरणम्.isContinuous())
    {
        CALL_HAL(माध्यम्_Std_Dev, cv_hal_माध्यम्_Std_Dev, src.दत्त, 0, (पूर्णाङ्क)src.योग(), 1, src.type(),
                 माध्यम्.needed() ? माध्यम्_mat.ptr<द्विगुण>() : शून्यसूचकः,
                 _sdv.needed() ? stddev_mat.ptr<द्विगुण>() : शून्यसूचकः,
                 आवरणम्.दत्त, 0);
    }
    अन्यथा
    {
        यदि (src.dims <= 2)
        {
            CALL_HAL(माध्यम्_Std_Dev, cv_hal_माध्यम्_Std_Dev, src.दत्त, src.चरणः, src.स्तम्भसङ्ख्या, src.पङ्क्तिकाः, src.type(),
                     माध्यम्.needed() ? माध्यम्_mat.ptr<द्विगुण>() : शून्यसूचकः,
                     _sdv.needed() ? stddev_mat.ptr<द्विगुण>() : शून्यसूचकः,
                     आवरणम्.दत्त, आवरणम्.चरणः);
        }
    }

    योग_Sqr_Func func = get_योग_Sqr_Func(गभीरता);

    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, &आवरणम्, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क योग = (पूर्णाङ्क)it.size, खण्डम्_आकार_2 = योग, partial_खण्डम्_आकार = 0;
    पूर्णाङ्क j;
    int64_t गणनम्_2 = 0, nz0 = 0;
    द्विगुण _buf[CV_CN_MAX*4];
    द्विगुण *s = _buf, *sq = s + cn;
    पूर्णाङ्क *sbuf = (पूर्णाङ्क*)s, *sqbuf = (पूर्णाङ्क*)sq;
    तार्किक partial_योग_Is_Int = गभीरता < CV_32S;
    तार्किक खण्डम्_योग = partial_योग_Is_Int || गभीरता == CV_16F || गभीरता == CV_16BF;
    तार्किक खण्डम्_Sq_योग = गभीरता <= CV_8S;
    size_t esz = 0;

    प्रति( k = 0; k < cn; k++ )
        s[k] = sq[k] = 0;

    यदि( खण्डम्_योग )
    {
        partial_खण्डम्_आकार = 1 << 15;
        खण्डम्_आकार_2 = std::min(खण्डम्_आकार_2, partial_खण्डम्_आकार);
        sbuf = (पूर्णाङ्क*)(sq + cn);
        यदि( खण्डम्_Sq_योग )
            sqbuf = sbuf + cn;
        प्रति( k = 0; k < cn; k++ )
            sbuf[k] = sqbuf[k] = 0;
        esz = src.elem_आकार();
    }

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( j = 0; j < योग; j += खण्डम्_आकार_2 )
        {
            पूर्णाङ्क bsz = std::min(योग - j, खण्डम्_आकार_2);
            पूर्णाङ्क nz = func( ptrs[0], ptrs[1], (uchar*)sbuf, (uchar*)sqbuf, bsz, cn );
            गणनम्_2 += nz;
            nz0 += nz;
            यदि( खण्डम्_योग && (गणनम्_2 + खण्डम्_आकार_2 >= partial_खण्डम्_आकार || (i+1 >= it.nplanes && j+bsz >= योग)) )
            {
                यदि (partial_योग_Is_Int) {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        s[k] += sbuf[k];
                        sbuf[k] = 0;
                    }
                } अन्यथा {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        s[k] += ((भिन्न*)sbuf)[k];
                        sbuf[k] = 0;
                    }
                }
                यदि( खण्डम्_Sq_योग )
                {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        sq[k] += sqbuf[k];
                        sqbuf[k] = 0;
                    }
                }
                गणनम्_2 = 0;
            }
            ptrs[0] += bsz*esz;
            यदि( ptrs[1] )
                ptrs[1] += bsz;
        }
    }

    द्विगुण scale = nz0 ? 1./nz0 : 0.;
    प्रति( k = 0; k < cn; k++ )
    {
        s[k] *= scale;
        sq[k] = std::वर्गमूलम्(std::max(sq[k]*scale - s[k]*s[k], 0.));
    }

    यदि (माध्यम्.needed())
    {
        ध्रुव द्विगुण* sptr = s;
        द्विगुण* dptr = माध्यम्_mat.ptr<द्विगुण>();
        प्रति( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }

    यदि (_sdv.needed())
    {
        ध्रुव द्विगुण* sptr = sq;
        द्विगुण* dptr = stddev_mat.ptr<द्विगुण>();
        प्रति( k = 0; k < cn; k++ )
            dptr[k] = sptr[k];
    }
}

स्थिर शून्य write_Stat_परिणाम(OutputArray _dst, पूर्णाङ्क cn, ध्रुव Scalar& v)
{
    यदि (!_dst.needed())
        प्रत्यागम;
    यदि (!_dst.fixed_आकार())
        _dst.create(cn, 1, CV_64F, -1, सत्य);
    Mat d = _dst.getMat();
    पूर्णाङ्क dcn = (पूर्णाङ्क)d.योग();
    CV_Assert( d.type() == CV_64F && d.isContinuous() && (d.स्तम्भसङ्ख्या == 1 || d.पङ्क्तिकाः == 1) && dcn >= cn );
    द्विगुण* p = d.ptr<द्विगुण>();
    प्रति (पूर्णाङ्क k = 0; k < dcn; k++)
        p[k] = k < cn ? v[k] : 0;
}

शून्य माध्यम्_Std_Dev(निवेशः_Array _src, OutputArray माध्यम्, OutputArray _sdv, निवेशः_Array आवरणम्_2)
{
    CV_INSTRUMENT_REGION();

    std::vector<StatChunk> chunks;
    यदि (!_src.isUMat() && !_src.empty())
    {
        Mat src = _src.getMat(), आवरणम् = आवरणम्_2.getMat();
        ध्रुव पूर्णाङ्क cn = src.channels();
        ध्रुव तार्किक आवरणम्_Ok = आवरणम्.empty() || ((आवरणम्.type() == CV_8U || आवरणम्.type() == CV_8S || आवरणम्.type() == CV_Bool) &&
                                             src.size == आवरणम्.size);
        यदि (cn <= 4 && आवरणम्_Ok && विभज_For_Parallel_Stat(src, आवरणम्, chunks))
        {
            ध्रुव पूर्णाङ्क n = (पूर्णाङ्क)chunks.size();
            std::vector<Scalar> m(n), sd(n);
            std::vector<द्विगुण> cnt(n);
            parallel_for_(Range(0, n), [&](ध्रुव Range& r)
            {
                प्रति (पूर्णाङ्क i = r.start; i < r.end; i++)
                {
                    Mat mi, si;
                    माध्यम्_Std_Dev_2(chunks[i].src, mi, si, chunks[i].आवरणम्);
                    प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                    {
                        m[i][k] = mi.at<द्विगुण>(k);
                        sd[i][k] = si.at<द्विगुण>(k);
                    }
                    cnt[i] = stat_गणनम्(chunks[i]);
                }
            });
            // combine the pieces: the variance is the mean of the piece variances plus the
            // variance of the piece means, both weighted by the piece sizes
            द्विगुण योग = 0;
            Scalar M = Scalar::all(0.0), V = Scalar::all(0.0);
            प्रति (पूर्णाङ्क i = 0; i < n; i++)
            {
                M += m[i]*cnt[i];
                योग += cnt[i];
            }
            यदि (योग > 0)
                M *= 1./योग;
            प्रति (पूर्णाङ्क i = 0; i < n; i++)
                प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                {
                    द्विगुण dm = m[i][k] - M[k];
                    V[k] += cnt[i]*(sd[i][k]*sd[i][k] + dm*dm);
                }
            Scalar SD;
            प्रति (पूर्णाङ्क k = 0; k < cn; k++)
                SD[k] = योग > 0 ? std::वर्गमूलम्(std::max(V[k]/योग, 0.)) : 0;
            write_Stat_परिणाम(माध्यम्, cn, M);
            write_Stat_परिणाम(_sdv, cn, SD);
            प्रत्यागम;
        }
    }
    माध्यम्_Std_Dev_2(_src, माध्यम्, _sdv, आवरणम्_2);
}

} // namespace
