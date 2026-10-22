// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "stat.hpp"

#అన్తర్భావ "stat_parallel.hpp"
#అన్తర్భావ "sum.simd.hpp"
#అన్తర్భావ "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv
{

కూడిక_Func get_కూడిక_Func(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_కూడిక_Func, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

#నిర్వచితమైతే HAVE_OPENCL

తార్కిక ocl_కూడిక( ఆదానం_Array _src, Scalar & res, పూర్ణాంకం కూడిక_op, ఆదానం_Array ముసుగు_2,
                     ఆదానం_Array _src2, తార్కిక calc2, స్థిరరాశి Scalar & res2 )
{
    CV_Assert(కూడిక_op == OCL_OP_కూడిక || కూడిక_op == OCL_OP_కూడిక_ABS || కూడిక_op == OCL_OP_కూడిక_SQR);

    స్థిరరాశి ocl::Device & dev = ocl::Device::getDefault();
    తార్కిక doubleSupport = dev.doubleFPConfig() > 0,
        have_ముసుగు = ముసుగు_2.రకం() != ఆదానం_Array_2::NONE,
        haveSrc2 = _src2.రకం() != ఆదానం_Array_2::NONE;
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !have_ముసుగు ? ocl::predict_Optimal_సదిశ_వెడల్పు(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    పూర్ణాంకం convert_cn = haveSrc2 ? mcn : cn;

    ఒకవేళ ( (!doubleSupport && లోతు == CV_64F) || cn > 4 )
        తిరిగి అబద్ధం;

    ఒకవేళ (లోతు >= CV_16F)
        తిరిగి అబద్ధం;

    పూర్ణాంకం ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_సమూహం_కొలత();

    పూర్ణాంకం ddepth = std::max(కూడిక_op == OCL_OP_కూడిక_SQR ? CV_32F : CV_32S, లోతు),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!have_ముసుగు || ముసుగు_2.type() == CV_8U || ముసుగు_2.type() == CV_8S || ముసుగు_2.type() == CV_Bool);

    పూర్ణాంకం wgs2_aligned = 1;
    అయితే (wgs2_aligned < (పూర్ణాంకం)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    అక్షరం cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, mcn)), ocl::type_గమ్యం_Str(లోతు),
                         ocl::type_గమ్యం_Str(dtype), ocl::type_గమ్యం_Str(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::type_గమ్యం_Str(ddepth), ddepth, cn,
                         ocl::convertTypeStr(లోతు, ddepth, mcn, cvt[0], పరిమాణం(cvt[0])),
                         opMap[కూడిక_op], (పూర్ణాంకం)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         have_ముసుగు ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         have_ముసుగు && ముసుగు_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         లోతు <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], పరిమాణం(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), ముసుగు = ముసుగు_2.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_కొలత(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_కొలత(ముసుగు),
            src2arg = ocl::KernelArg::Read_Only_No_కొలత(src2);

    ఒకవేళ (have_ముసుగు)
    {
        ఒకవేళ (haveSrc2)
            k.వాదనలు(srcarg, src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(), ngroups, dbarg, maskarg, src2arg);
        లేకపోతే
            k.వాదనలు(srcarg, src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(), ngroups, dbarg, maskarg);
    }
    లేకపోతే
    {
        ఒకవేళ (haveSrc2)
            k.వాదనలు(srcarg, src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(), ngroups, dbarg, src2arg);
        లేకపోతే
            k.వాదనలు(srcarg, src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    ఒకవేళ (k.నిరంతరం(1, &globalsize, &wgs, నిజం))
    {
        రకనిర్వచనం Scalar (*part_కూడిక)(Mat m);
        part_కూడిక funcs[3] = { ocl_part_కూడిక<పూర్ణాంకం>, ocl_part_కూడిక<దశాంశం>, ocl_part_కూడిక<ద్విగుణ> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        ఒకవేళ (calc2)
            const_cast<Scalar &>(res2) = func(mres.నిలువు_Range_2(ngroups, dbsize));

        res = func(mres.నిలువు_Range_2(0, ngroups));
        తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
}

#అయితే_ముగింపు

// serial implementation, also used for the pieces of the parallel version
స్థిర Scalar కూడిక_2(స్థిరరాశి Mat& src)
{
    Scalar _res;
    పూర్ణాంకం cn = src.channels();

    ఒకవేళ (src.dims <= 2)
    {
        CALL_HAL_RET2(కూడిక, cv_hal_కూడిక, _res, src.దత్తాంశం, src.అడుగు, src.type(), src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, &_res[0]);
    }
    లేకపోతే ఒకవేళ (src.isContinuous())
    {
        CALL_HAL_RET2(కూడిక, cv_hal_కూడిక, _res, src.దత్తాంశం, 0, src.type(), (పూర్ణాంకం)src.మొత్తం(), 1, &_res[0]);
    }

    పూర్ణాంకం k, లోతు = src.లోతు();
    కూడిక_Func func = get_కూడిక_Func(లోతు);
    ఒకవేళ (func == శూన్యసూచిక) {
        ఒకవేళ (లోతు == CV_Bool && cn == 1)
            తిరిగి Scalar((ద్విగుణ)లెక్క_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            type_గమ్యం_String(src.type()).c_str()));
    }

    స్థిరరాశి Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size, ఖండం_కొలత_2 = మొత్తం, partial_ఖండం_కొలత = 0;
    పూర్ణాంకం j, లెక్క_2 = 0;
    పూర్ణాంకం _buf[CV_CN_MAX];
    పూర్ణాంకం* buf = (పూర్ణాంకం*)&_res[0];
    size_t esz = 0;
    తార్కిక partial_కూడిక_Is_Int = లోతు < CV_32S;
    తార్కిక ఖండం_కూడిక = partial_కూడిక_Is_Int || లోతు == CV_16F || లోతు == CV_16BF;

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
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            లెక్క_2 += bsz;
            ఒకవేళ( ఖండం_కూడిక && (లెక్క_2 + ఖండం_కొలత_2 >= partial_ఖండం_కొలత || (i+1 >= it.nplanes && j+bsz >= మొత్తం)) )
            {
                ఒకవేళ (partial_కూడిక_Is_Int) {
                    కొరకు( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } లేకపోతే {
                    కొరకు( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((దశాంశం*)buf)[k];
                        buf[k] = 0;
                    }
                }
                లెక్క_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    తిరిగి _res;
}

Scalar కూడిక(ఆదానం_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#నిర్వచితమైతే HAVE_OPENCL
    CV_OCL_నిరంతరం_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_కూడిక(_src, _res, OCL_OP_కూడిక),
                _res);
#అయితే_ముగింపు

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    ఒకవేళ (!విభజించు_For_Parallel_Stat(src, Mat(), chunks))
        తిరిగి కూడిక_2(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (పూర్ణాంకం)chunks.size()), [&](స్థిరరాశి Range& r)
    {
        కొరకు (పూర్ణాంకం i = r.ప్రారంభం; i < r.end; i++)
            part[i] = కూడిక_2(chunks[i].src);
    });
    కొరకు (స్థిరరాశి Scalar& v : part)
        _res += v;
    తిరిగి _res;
}

} // namespace
