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

جوڑ_Func get_جوڑ_Func(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_جوڑ_Func, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#جے_تعریف_شدہ HAVE_OPENCL

منطقي ocl_جوڑ( داخلہ_Array _src, Scalar & res, صحيح جوڑ_op, داخلہ_Array نقاب_2,
                     داخلہ_Array _src2, منطقي calc2, ثابت Scalar & res2 )
{
    CV_Assert(جوڑ_op == OCL_OP_جوڑ || جوڑ_op == OCL_OP_جوڑ_ABS || جوڑ_op == OCL_OP_جوڑ_SQR);

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0,
        have_نقاب = نقاب_2.قسم() != داخلہ_Array_2::NONE,
        haveSrc2 = _src2.قسم() != داخلہ_Array_2::NONE;
    صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type),
            kercn = cn == 1 && !have_نقاب ? ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _src2) : 1,
            mcn = std::max(cn, kercn);
    CV_Assert(!haveSrc2 || _src2.type() == type);
    صحيح convert_cn = haveSrc2 ? mcn : cn;

    إذا ( (!doubleSupport && ڈونگھائی == CV_64F) || cn > 4 )
        أرجع خطأ;

    إذا (ڈونگھائی >= CV_16F)
        أرجع خطأ;

    صحيح ngroups = dev.maxComputeUnits(), dbsize = ngroups * (calc2 ? 2 : 1);
    size_t wgs = dev.max_Work_گروہ_ناپ();

    صحيح ddepth = std::max(جوڑ_op == OCL_OP_جوڑ_SQR ? CV_32F : CV_32S, ڈونگھائی),
            dtype = CV_MAKE_TYPE(ddepth, cn);
    CV_Assert(!have_نقاب || نقاب_2.type() == CV_8U || نقاب_2.type() == CV_8S || نقاب_2.type() == CV_Bool);

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ساكن ثابت حرف * ثابت opMap[3] = { "OP_SUM", "OP_SUM_ABS", "OP_SUM_SQR" };
    حرف cvt[2][50];
    String opts = format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D dstT1=%s -D ddepth=%d -D cn=%d"
                         " -D convertToDT=%s -D %s -D WGS=%d -D WGS2_ALIGNED=%d%s%s%s%s -D kercn=%d%s%s%s -D convertFromU=%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, mcn)), ocl::type_منزل_Str(ڈونگھائی),
                         ocl::type_منزل_Str(dtype), ocl::type_منزل_Str(CV_MAKE_TYPE(ddepth, mcn)),
                         ocl::type_منزل_Str(ddepth), ddepth, cn,
                         ocl::convertTypeStr(ڈونگھائی, ddepth, mcn, cvt[0], حجم(cvt[0])),
                         opMap[جوڑ_op], (صحيح)wgs, wgs2_aligned,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         have_نقاب ? " -D HAVE_MASK" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         have_نقاب && نقاب_2.isContinuous() ? " -D HAVE_MASK_CONT" : "", kercn,
                         haveSrc2 ? " -D HAVE_SRC2" : "", calc2 ? " -D OP_CALC2" : "",
                         haveSrc2 && _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "",
                         ڈونگھائی <= CV_32S && ddepth == CV_32S ? ocl::convertTypeStr(CV_8U, ddepth, convert_cn, cvt[1], حجم(cvt[1])) : "noconvert");

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), src2 = _src2.getUMat(),
        db(1, dbsize, dtype), نقاب = نقاب_2.getUMat();

    ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_ناپ(src),
            dbarg = ocl::KernelArg::PtrWriteOnly(db),
            maskarg = ocl::KernelArg::Read_Only_No_ناپ(نقاب),
            src2arg = ocl::KernelArg::Read_Only_No_ناپ(src2);

    إذا (have_نقاب)
    {
        إذا (haveSrc2)
            k.دلیلاں(srcarg, src.خانے_گنتی, (صحيح)src.کل(), ngroups, dbarg, maskarg, src2arg);
        وإلا
            k.دلیلاں(srcarg, src.خانے_گنتی, (صحيح)src.کل(), ngroups, dbarg, maskarg);
    }
    وإلا
    {
        إذا (haveSrc2)
            k.دلیلاں(srcarg, src.خانے_گنتی, (صحيح)src.کل(), ngroups, dbarg, src2arg);
        وإلا
            k.دلیلاں(srcarg, src.خانے_گنتی, (صحيح)src.کل(), ngroups, dbarg);
    }

    size_t globalsize = ngroups * wgs;
    إذا (k.لگاتار(1, &globalsize, &wgs, سچ))
    {
        تعريفنوع Scalar (*part_جوڑ)(Mat m);
        part_جوڑ funcs[3] = { ocl_part_جوڑ<صحيح>, ocl_part_جوڑ<عائم>, ocl_part_جوڑ<مزدوج> },
                func = funcs[ddepth - CV_32S];

        Mat mres = db.getMat(ACCESS_READ);
        إذا (calc2)
            const_cast<Scalar &>(res2) = func(mres.خانہ_Range_2(ngroups, dbsize));

        res = func(mres.خانہ_Range_2(0, ngroups));
        أرجع سچ;
    }
    أرجع خطأ;
}

#جے_انت

// serial implementation, also used for the pieces of the parallel version
ساكن Scalar جوڑ_2(ثابت Mat& src)
{
    Scalar _res;
    صحيح cn = src.channels();

    إذا (src.dims <= 2)
    {
        CALL_HAL_RET2(جوڑ, cv_hal_جوڑ, _res, src.انکڑے, src.قدم, src.type(), src.خانے_گنتی, src.صفاں, &_res[0]);
    }
    وإلا إذا (src.isContinuous())
    {
        CALL_HAL_RET2(جوڑ, cv_hal_جوڑ, _res, src.انکڑے, 0, src.type(), (صحيح)src.کل(), 1, &_res[0]);
    }

    صحيح k, ڈونگھائی = src.ڈونگھائی();
    جوڑ_Func func = get_جوڑ_Func(ڈونگھائی);
    إذا (func == خالی_سوچک) {
        إذا (ڈونگھائی == CV_Bool && cn == 1)
            أرجع Scalar((مزدوج)گنتی_Non_Zero(src));
        CV_Error_(Error::StsNotImplemented, ("sum: %s input is not supported, convert the input to another type (e.g. CV_32F) first",
                                            type_منزل_String(src.type()).c_str()));
    }

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح کل = (صحيح)it.size, بلاک_ناپ_2 = کل, partial_بلاک_ناپ = 0;
    صحيح j, گنتی_2 = 0;
    صحيح _buf[CV_CN_MAX];
    صحيح* buf = (صحيح*)&_res[0];
    size_t esz = 0;
    منطقي partial_جوڑ_Is_Int = ڈونگھائی < CV_32S;
    منطقي بلاک_جوڑ = partial_جوڑ_Is_Int || ڈونگھائی == CV_16F || ڈونگھائی == CV_16BF;

    إذا( بلاک_جوڑ )
    {
        partial_بلاک_ناپ = ڈونگھائی <= CV_8S ? (1 << 23) : (1 << 15);
        بلاک_ناپ_2 = std::min(بلاک_ناپ_2, partial_بلاک_ناپ);
        buf = _buf;
        لكل( k = 0; k < cn; k++ )
            buf[k] = 0;
        esz = src.elem_ناپ();
    }

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( j = 0; j < کل; j += بلاک_ناپ_2 )
        {
            صحيح bsz = std::min(کل - j, بلاک_ناپ_2);
            func( ptrs[0], 0, (uchar*)buf, bsz, cn );
            گنتی_2 += bsz;
            إذا( بلاک_جوڑ && (گنتی_2 + بلاک_ناپ_2 >= partial_بلاک_ناپ || (i+1 >= it.nplanes && j+bsz >= کل)) )
            {
                إذا (partial_جوڑ_Is_Int) {
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
                گنتی_2 = 0;
            }
            ptrs[0] += bsz*esz;
        }
    }
    أرجع _res;
}

Scalar جوڑ(داخلہ_Array _src)
{
    CV_INSTRUMENT_REGION();

    Scalar _res;
#جے_تعریف_شدہ HAVE_OPENCL
    CV_OCL_لگاتار_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_جوڑ(_src, _res, OCL_OP_جوڑ),
                _res);
#جے_انت

    Mat src = _src.getMat();
    CV_CheckLE( src.channels(), 4, "cv::sum does not support more than 4 channels" );

    std::vector<StatChunk> chunks;
    إذا (!ونڈو_For_Parallel_Stat(src, Mat(), chunks))
        أرجع جوڑ_2(src);

    std::vector<Scalar> part(chunks.size());
    parallel_for_(Range(0, (صحيح)chunks.size()), [&](ثابت Range& r)
    {
        لكل (صحيح i = r.شروع; i < r.end; i++)
            part[i] = جوڑ_2(chunks[i].src);
    });
    لكل (ثابت Scalar& v : part)
        _res += v;
    أرجع _res;
}

} // namespace
