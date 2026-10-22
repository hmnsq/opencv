// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"

#अन्तर्भाव "stat_parallel.hpp"
#अन्तर्भाव "sum.simd.hpp"
#अन्तर्भाव "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv
{

योग_Func get_योग_Func(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_योग_Func, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
}

#यदि_परिभाषितम् HAVE_OPENCL

तार्किक ocl_योग( निवेशः_Array _src, Scalar & res, पूर्णाङ्क योग_op, निवेशः_Array आवरणम्_2,
                     निवेशः_Array _src2, तार्किक calc2, ध्रुव Scalar & res2 )
{
    CV_Assert(योग_op == OCL_OP_योग || योग_op == OCL_OP_योग_ABS || योग_op == OCL_OP_योग_SQR);

    ध्रुव ocl::Device & dev = ocl::Device::getDefault();
    तार्किक doubleSupport = dev.doubleFPConfig() > 0,
        have_आवरणम् = आवरणम्_2.विधा() != निवेशः_Array_2::NONE,
        haveSrc2 = _src2.विधा() != निवेशः_Array_2::NONE;
    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !have_आवरणम् ? ocl::predict_Optimal_सदिशम्_विस्तारः(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    पूर्णाङ्क convert_cn = haveSrc2 ? mcn : cn;

    यदि ( (!doubleSupport && गभीरता == CV_64F) || cn > 4 )
        प्रत्यागम असत्य;

    यदि (गभीरता >= CV_16F)
        प्रत्यागम असत्य;

    पूर्णाङ्क ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_गणः_आकार();

    पूर्णाङ्क ddepth = std::max(योग_op == OCL_OP_योग_SQR ? CV_32F : CV_32S, गभीरता),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!have_आवरणम् || आवरणम्_2.type() == CV_8U || आवरणम्_2.type() == CV_8S || आवरणम्_2.type() == CV_Bool);

    पूर्णाङ्क wgs2_aligned = 1;
    यावत् (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    स्थिर ध्रुव वर्ण * ध्रुव opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    वर्ण cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, mcn)), ocl::type_गन्तव्यम्_Str(गभीरता),
                         ocl::type_गन्तव्यम्_Str(dtype), ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::type_गन्तव्यम्_Str(ddepth), ddepth, cn,
                         ocl::convertTypeStr(गभीरता, ddepth, mcn, cvt[0], परिमाण(cvt[0])),
                         opMap[योग_op], (पूर्णाङ्क)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         have_आवरणम् ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         have_आवरणम् && आवरणम्_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         गभीरता <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], परिमाण(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), आवरणम् = आवरणम्_2.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_आकार(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_आकार(आवरणम्),
            src2arg = ocl::KernelArg::Read_Only_No_आकार(src2);

    यदि (have_आवरणम्)
    {
        यदि (haveSrc2)
            k.तर्काः(srcarg, src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(), ngroups, dbarg, maskarg, src2arg);
        अन्यथा
            k.तर्काः(srcarg, src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(), ngroups, dbarg, maskarg);
    }
    अन्यथा
    {
        यदि (haveSrc2)
            k.तर्काः(srcarg, src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(), ngroups, dbarg, src2arg);
        अन्यथा
            k.तर्काः(srcarg, src.स्तम्भसङ्ख्या, (पूर्णाङ्क)src.योग(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    यदि (k.सततम्(1, &globalsize, &wgs, सत्य))
    {
        प्रकारनिर्देश Scalar (*part_योग)(Mat m);
        part_योग funcs[3] = { ocl_part_योग<पूर्णाङ्क>, ocl_part_योग<भिन्न>, ocl_part_योग<द्विगुण> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        यदि (calc2)
            const_cast<Scalar &>(res2) = func(mres.colRange(ngroups, dbsize));

        res = func(mres.colRange(0, ngroups));
        प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

#यदि_अन्तः

// serial implementation, also used for the pieces of the parallel version
स्थिर Scalar योग_4(ध्रुव Mat& src)
{
    Scalar _res;
    पूर्णाङ्क cn = src.channels();

    यदि (src.dims <= 2)
    {
        CALL_HAL_RET2(योग_2, cv_hal_योग, _res, src.दत्त, src.चरणः, src.type(), src.स्तम्भसङ्ख्या, src.पङ्क्तिकाः, &_res[0]);
    }
    अन्यथा यदि (src.isContinuous())
    {
        CALL_HAL_RET2(योग_2, cv_hal_योग, _res, src.दत्त, 0, src.type(), (पूर्णाङ्क)src.योग(), 1, &_res[0]);
    }

    पूर्णाङ्क k, गभीरता = src.गभीरता();
    योग_Func func = get_योग_Func(गभीरता);
    यदि (func == शून्यसूचकः) {
        यदि (गभीरता == CV_Bool && cn == 1)
            प्रत्यागम Scalar((द्विगुण)गणनम्_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            type_गन्तव्यम्_String(src.type()).c_str()));
    }

    ध्रुव Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क योग = (पूर्णाङ्क)it.size, खण्डम्_आकार_2 = योग, partial_खण्डम्_आकार = 0;
    पूर्णाङ्क j, गणनम्_2 = 0;
    पूर्णाङ्क _buf[CV_CN_MAX];
    पूर्णाङ्क* buf = (पूर्णाङ्क*)&_res[0];
    size_t esz = 0;
    तार्किक partial_योग_Is_Int = गभीरता < CV_32S;
    तार्किक खण्डम्_योग = partial_योग_Is_Int || गभीरता == CV_16F || गभीरता == CV_16BF;

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
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            गणनम्_2 += bsz;
            यदि( खण्डम्_योग && (गणनम्_2 + खण्डम्_आकार_2 >= partial_खण्डम्_आकार || (i+1 >= it.nplanes && j+bsz >= योग)) )
            {
                यदि (partial_योग_Is_Int) {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } अन्यथा {
                    प्रति( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((भिन्न*)buf)[k];
                        buf[k] = 0;
                    }
                }
                गणनम्_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    प्रत्यागम _res;
}

Scalar योग_2(निवेशः_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#यदि_परिभाषितम् HAVE_OPENCL
    CV_OCL_सततम्_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_योग(_src, _res, OCL_OP_योग),
                _res);
#यदि_अन्तः

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    यदि (!विभज_For_Parallel_Stat(src, Mat(), chunks))
        प्रत्यागम योग_4(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (पूर्णाङ्क)chunks.size()), [&](ध्रुव Range& r)
    {
        प्रति (पूर्णाङ्क i = r.start; i < r.end; i++)
            part[i] = योग_4(chunks[i].src);
    });
    प्रति (ध्रुव Scalar& v : part)
        _res += v;
    प्रत्यागम _res;
}

} // namespace
