// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "stat_parallel.hpp"
#ضمن "sum.simd.hpp"
#ضمن "sum.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv
{

مجموعه_Func get_مجموعه_Func(صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_مجموعه_Func, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#که_تعریف_شوی HAVE_OPENCL

منطقي ocl_مجموعه( ننوت_Array _src, Scalar & res, صحيح مجموعه_op, ننوت_Array ماسک_2,
                     ننوت_Array _src2, منطقي calc2, ثابت Scalar & res2 )
{
    CV_Assert(مجموعه_op == OCL_OP_مجموعه || مجموعه_op == OCL_OP_مجموعه_ABS || مجموعه_op == OCL_OP_مجموعه_SQR);

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0,
        have_ماسک = ماسک_2.ډول() != ننوت_Array_2::NONE,
        haveSrc2 = _src2.ډول() != ننوت_Array_2::NONE;
    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !have_ماسک ? ocl::predict_Optimal_ویکټور_پلنوالی(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    صحيح convert_cn = haveSrc2 ? mcn : cn;

    إذا ( (!doubleSupport && ژوروالی == CV_64F) || cn > 4 )
        أرجع خطأ;

    إذا (ژوروالی >= CV_16F)
        أرجع خطأ;

    صحيح ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_ډله_اندازه();

    صحيح ddepth = std::max(مجموعه_op == OCL_OP_مجموعه_SQR ? CV_32F : CV_32S, ژوروالی),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!have_ماسک || ماسک_2.type() == CV_8U || ماسک_2.type() == CV_8S || ماسک_2.type() == CV_Bool);

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ساكن ثابت حرف * ثابت opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    حرف cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, mcn)), ocl::type_منزل_Str(ژوروالی),
                         ocl::type_منزل_Str(dtype), ocl::type_منزل_Str(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::type_منزل_Str(ddepth), ddepth, cn,
                         ocl::convertTypeStr(ژوروالی, ddepth, mcn, cvt[0], حجم(cvt[0])),
                         opMap[مجموعه_op], (صحيح)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         have_ماسک ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         have_ماسک && ماسک_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         ژوروالی <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], حجم(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), ماسک = ماسک_2.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_اندازه(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_اندازه(ماسک),
            src2arg = ocl::KernelArg::Read_Only_No_اندازه(src2);

    إذا (have_ماسک)
    {
        إذا (haveSrc2)
            k.دلیلونه(srcarg, src.ستون_شمېر, (صحيح)src.ټول(), ngroups, dbarg, maskarg, src2arg);
        وإلا
            k.دلیلونه(srcarg, src.ستون_شمېر, (صحيح)src.ټول(), ngroups, dbarg, maskarg);
    }
    وإلا
    {
        إذا (haveSrc2)
            k.دلیلونه(srcarg, src.ستون_شمېر, (صحيح)src.ټول(), ngroups, dbarg, src2arg);
        وإلا
            k.دلیلونه(srcarg, src.ستون_شمېر, (صحيح)src.ټول(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    إذا (k.پرلپسې(1, &globalsize, &wgs, ریښتیا))
    {
        تعريفنوع Scalar (*part_مجموعه)(Mat m);
        part_مجموعه funcs[3] = { ocl_part_مجموعه<صحيح>, ocl_part_مجموعه<عائم>, ocl_part_مجموعه<مزدوج> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        إذا (calc2)
            const_cast<Scalar &>(res2) = func(mres.ستون_Range_2(ngroups, dbsize));

        res = func(mres.ستون_Range_2(0, ngroups));
        أرجع ریښتیا;
    }
    أرجع خطأ;
}

#د_که_پای

// serial implementation, also used for the pieces of the parallel version
ساكن Scalar مجموعه_2(ثابت Mat& src)
{
    Scalar _res;
    صحيح cn = src.channels();

    إذا (src.dims <= 2)
    {
        CALL_HAL_RET2(مجموعه, cv_hal_مجموعه, _res, src.معلومات, src.ګام, src.type(), src.ستون_شمېر, src.لیکې, &_res[0]);
    }
    وإلا إذا (src.isContinuous())
    {
        CALL_HAL_RET2(مجموعه, cv_hal_مجموعه, _res, src.معلومات, 0, src.type(), (صحيح)src.ټول(), 1, &_res[0]);
    }

    صحيح k, ژوروالی = src.ژوروالی();
    مجموعه_Func func = get_مجموعه_Func(ژوروالی);
    إذا (func == تش_اشاره) {
        إذا (ژوروالی == CV_Bool && cn == 1)
            أرجع Scalar((مزدوج)شمېر_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            type_منزل_String(src.type()).c_str()));
    }

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح ټول = (صحيح)it.size, بلاک_اندازه_2 = ټول, partial_بلاک_اندازه = 0;
    صحيح j, شمېر_2 = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&_res[0];
    size_t esz = 0;
    منطقي partial_مجموعه_Is_Int = ژوروالی < CV_32S;
    منطقي بلاک_مجموعه = partial_مجموعه_Is_Int || ژوروالی == CV_16F || ژوروالی == CV_16BF;

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
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            شمېر_2 += bsz;
            إذا( بلاک_مجموعه && (شمېر_2 + بلاک_اندازه_2 >= partial_بلاک_اندازه || (i+1 >= it.nplanes && j+bsz >= ټول)) )
            {
                إذا (partial_مجموعه_Is_Int) {
                    لكل( k = 0; k < cn; k++ )
                    {
                        _res[k] += buf[k];
                        buf[k] = 0;
                    }
                } وإلا {
                    لكل( k = 0; k < cn; k++ )
                    {
                        _res[k] += ((عائم*)buf)[k];
                        buf[k] = 0;
                    }
                }
                شمېر_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    أرجع _res;
}

Scalar مجموعه(ننوت_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#که_تعریف_شوی HAVE_OPENCL
    CV_OCL_پرلپسې_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_مجموعه(_src, _res, OCL_OP_مجموعه),
                _res);
#د_که_پای

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    إذا (!وېشل_For_Parallel_Stat(src, Mat(), chunks))
        أرجع مجموعه_2(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (صحيح)chunks.size()), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.پیل; i < r.end; i++)
            part[i] = مجموعه_2(chunks[i].src);
    });
    لكل (ثابت Scalar& v : part)
        _res += v;
    أرجع _res;
}

} // namespace
