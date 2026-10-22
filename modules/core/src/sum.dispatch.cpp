// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"

#شامل "stat_parallel.hpp"
#شامل "sum.simd.hpp"
#شامل "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv
{

مجموع_Func get_مجموع_Func(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_مجموع_Func, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شدہ HAVE_OPENCL

منطقی ocl_مجموع( داخلہ_Array _src, Scalar & res, صحیح مجموع_op, داخلہ_Array نقاب_2,
                     داخلہ_Array _src2, منطقی calc2, ثابت Scalar & res2 )
{
    CV_Assert(مجموع_op == OCL_OP_مجموع || مجموع_op == OCL_OP_مجموع_ABS || مجموع_op == OCL_OP_مجموع_SQR);

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقی doubleSupport = dev.doubleFPConfig() > 0,
        have_نقاب = نقاب_2.قسم() != داخلہ_Array_2::NONE,
        haveSrc2 = _src2.قسم() != داخلہ_Array_2::NONE;
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !have_نقاب ? ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    صحیح convert_cn = haveSrc2 ? mcn : cn;

    اگر ( (!doubleSupport && گہرائی == CV_64F) || cn > 4 )
        واپس جھوٹ;

    اگر (گہرائی >= CV_16F)
        واپس جھوٹ;

    صحیح ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_گروہ_سائز();

    صحیح ddepth = std::max(مجموع_op == OCL_OP_مجموع_SQR ? CV_32F : CV_32S, گہرائی),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!have_نقاب || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool);

    صحیح wgs2_aligned = 1;
    جب (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ساکن ثابت حرف * ثابت opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    حرف cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, mcn)), ocl::type_مقصد_Str(گہرائی),
                         ocl::type_مقصد_Str(dtype), ocl::type_مقصد_Str(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::type_مقصد_Str(ddepth), ddepth, cn,
                         ocl::convertTypeStr(گہرائی, ddepth, mcn, cvt[0], حجم(cvt[0])),
                         opMap[مجموع_op], (صحیح)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         have_نقاب ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         have_نقاب && نقاب_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         گہرائی <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], حجم(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    اگر (k.empty())
        واپس جھوٹ;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), نقاب = نقاب_2.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_سائز(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_سائز(نقاب),
            src2arg = ocl::KernelArg::Read_Only_No_سائز(src2);

    اگر (have_نقاب)
    {
        اگر (haveSrc2)
            k.دلائل(srcarg, src.ستون_تعداد, (صحیح)src.کل(), ngroups, dbarg, maskarg, src2arg);
        ورنہ
            k.دلائل(srcarg, src.ستون_تعداد, (صحیح)src.کل(), ngroups, dbarg, maskarg);
    }
    ورنہ
    {
        اگر (haveSrc2)
            k.دلائل(srcarg, src.ستون_تعداد, (صحیح)src.کل(), ngroups, dbarg, src2arg);
        ورنہ
            k.دلائل(srcarg, src.ستون_تعداد, (صحیح)src.کل(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    اگر (k.تسلسل(1, &globalsize, &wgs, سچ))
    {
        قسمتعریف Scalar (*part_مجموع)(Mat m);
        part_مجموع funcs[3] = { ocl_part_مجموع<صحیح>, ocl_part_مجموع<اعشاری>, ocl_part_مجموع<دوہرا> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        اگر (calc2)
            const_cast<Scalar &>(res2) = func(mres.ستون_Range_2(ngroups, dbsize));

        res = func(mres.ستون_Range_2(0, ngroups));
        واپس سچ;
    }
    واپس جھوٹ;
}

#اگر_ختم

// serial implementation, also used for the pieces of the parallel version
ساکن Scalar مجموع_2(ثابت Mat& src)
{
    Scalar _res;
    صحیح cn = src.channels();

    اگر (src.dims <= 2)
    {
        CALL_HAL_RET2(مجموع, cv_hal_مجموع, _res, src.ڈیٹا, src.قدم, src.type(), src.ستون_تعداد, src.صفیں, &_res[0]);
    }
    ورنہ اگر (src.isContinuous())
    {
        CALL_HAL_RET2(مجموع, cv_hal_مجموع, _res, src.ڈیٹا, 0, src.type(), (صحیح)src.کل(), 1, &_res[0]);
    }

    صحیح k, گہرائی = src.گہرائی();
    مجموع_Func func = get_مجموع_Func(گہرائی);
    اگر (func == خالی_اشاریہ) {
        اگر (گہرائی == CV_Bool && cn == 1)
            واپس Scalar((دوہرا)گنتی_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            type_مقصد_String(src.type()).c_str()));
    }

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح کل = (صحیح)it.size, بلاک_سائز_2 = کل, partial_بلاک_سائز = 0;
    صحیح j, گنتی_2 = 0;
    صحیح _buf[CV_CN_MAX];
    صحیح* buf = (صحیح*)&_res[0];
    size_t esz = 0;
    منطقی partial_مجموع_Is_Int = گہرائی < CV_32S;
    منطقی بلاک_مجموع = partial_مجموع_Is_Int || گہرائی == CV_16F || گہرائی == CV_16BF;

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
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            گنتی_2 += bsz;
            اگر( بلاک_مجموع && (گنتی_2 + بلاک_سائز_2 >= partial_بلاک_سائز || (i+1 >= it.nplanes && j+bsz >= کل)) )
            {
                اگر (partial_مجموع_Is_Int) {
                    برائے( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } ورنہ {
                    برائے( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((اعشاری*)buf)[k];
                        buf[k] = 0;
                    }
                }
                گنتی_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    واپس _res;
}

Scalar مجموع(داخلہ_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#اگر_تعریف_شدہ HAVE_OPENCL
    CV_OCL_تسلسل_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_مجموع(_src, _res, OCL_OP_مجموع),
                _res);
#اگر_ختم

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    اگر (!بانٹو_For_Parallel_Stat(src, Mat(), chunks))
        واپس مجموع_2(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (صحیح)chunks.size()), [&](ثابت Range& r)
    {
        برائے (صحیح i = r.آغاز; i < r.end; i++)
            part[i] = مجموع_2(chunks[i].src);
    });
    برائے (ثابت Scalar& v : part)
        _res += v;
    واپس _res;
}

} // namespace
