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

جمع_Func get_جمع_Func(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_جمع_Func, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شده HAVE_OPENCL

منطقي ocl_جمع( درون_داد_Array _src, Scalar & res, صحيح جمع_op, درون_داد_Array ماسک_2,
                     درون_داد_Array _src2, منطقي calc2, ثابت Scalar & res2 )
{
    CV_Assert(جمع_op == OCL_OP_جمع || جمع_op == OCL_OP_جمع_ABS || جمع_op == OCL_OP_جمع_SQR);

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0,
        have_ماسک = ماسک_2.نوع() != درون_داد_Array_2::NONE,
        haveSrc2 = _src2.نوع() != درون_داد_Array_2::NONE;
    صحيح type = _src.type(), عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !have_ماسک ? ocl::predict_Optimal_وکتور_عرض(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    صحيح convert_cn = haveSrc2 ? mcn : cn;

    إذا ( (!doubleSupport && عمق == CV_64F) || cn > 4 )
        أرجع خطأ;

    إذا (عمق >= CV_16F)
        أرجع خطأ;

    صحيح ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_گروه_اندازه();

    صحيح ddepth = std::max(جمع_op == OCL_OP_جمع_SQR ? CV_32F : CV_32S, عمق),
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
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(عمق, mcn)), ocl::type_مقصد_Str(عمق),
                         ocl::type_مقصد_Str(dtype), ocl::type_مقصد_Str(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::type_مقصد_Str(ddepth), ddepth, cn,
                         ocl::convertTypeStr(عمق, ddepth, mcn, cvt[0], حجم(cvt[0])),
                         opMap[جمع_op], (صحيح)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         have_ماسک ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         have_ماسک && ماسک_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         عمق <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], حجم(cvt[1])) : "noconvert");

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
            k.آرگومانها(srcarg, src.تعداد_ستون, (صحيح)src.مجموع(), ngroups, dbarg, maskarg, src2arg);
        وإلا
            k.آرگومانها(srcarg, src.تعداد_ستون, (صحيح)src.مجموع(), ngroups, dbarg, maskarg);
    }
    وإلا
    {
        إذا (haveSrc2)
            k.آرگومانها(srcarg, src.تعداد_ستون, (صحيح)src.مجموع(), ngroups, dbarg, src2arg);
        وإلا
            k.آرگومانها(srcarg, src.تعداد_ستون, (صحيح)src.مجموع(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    إذا (k.پیاپی(1, &globalsize, &wgs, درست))
    {
        تعريفنوع Scalar (*part_جمع)(Mat m);
        part_جمع funcs[3] = { ocl_part_جمع<صحيح>, ocl_part_جمع<عائم>, ocl_part_جمع<مزدوج> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        إذا (calc2)
            const_cast<Scalar &>(res2) = func(mres.ستون_Range_2(ngroups, dbsize));

        res = func(mres.ستون_Range_2(0, ngroups));
        أرجع درست;
    }
    أرجع خطأ;
}

#پایان_اگر

// serial implementation, also used for the pieces of the parallel version
ساكن Scalar جمع_2(ثابت Mat& src)
{
    Scalar _res;
    صحيح cn = src.channels();

    إذا (src.dims <= 2)
    {
        CALL_HAL_RET2(جمع, cv_hal_جمع, _res, src.داده, src.گام, src.type(), src.تعداد_ستون, src.ردیفها, &_res[0]);
    }
    وإلا إذا (src.isContinuous())
    {
        CALL_HAL_RET2(جمع, cv_hal_جمع, _res, src.داده, 0, src.type(), (صحيح)src.مجموع(), 1, &_res[0]);
    }

    صحيح k, عمق = src.عمق();
    جمع_Func func = get_جمع_Func(عمق);
    إذا (func == اشاره_تهی) {
        إذا (عمق == CV_Bool && cn == 1)
            أرجع Scalar((مزدوج)شمار_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            type_مقصد_String(src.type()).c_str()));
    }

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح مجموع = (صحيح)it.size, بلوک_اندازه_2 = مجموع, partial_بلوک_اندازه = 0;
    صحيح j, شمار_2 = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&_res[0];
    size_t esz = 0;
    منطقي partial_جمع_Is_Int = عمق < CV_32S;
    منطقي بلوک_جمع = partial_جمع_Is_Int || عمق == CV_16F || عمق == CV_16BF;

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
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            شمار_2 += bsz;
            إذا( بلوک_جمع && (شمار_2 + بلوک_اندازه_2 >= partial_بلوک_اندازه || (i+1 >= it.nplanes && j+bsz >= مجموع)) )
            {
                إذا (partial_جمع_Is_Int) {
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
                شمار_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    أرجع _res;
}

Scalar جمع(درون_داد_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#اگر_تعریف_شده HAVE_OPENCL
    CV_OCL_پیاپی_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_جمع(_src, _res, OCL_OP_جمع),
                _res);
#پایان_اگر

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    إذا (!تقسیم_For_Parallel_Stat(src, Mat(), chunks))
        أرجع جمع_2(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (صحيح)chunks.size()), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.شروع; i < r.end; i++)
            part[i] = جمع_2(chunks[i].src);
    });
    لكل (ثابت Scalar& v : part)
        _res += v;
    أرجع _res;
}

} // namespace
