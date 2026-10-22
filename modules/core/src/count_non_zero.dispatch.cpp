// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "count_non_zero.simd.hpp"
#ضمن "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن شمار_Non_Zero_Func get_شمار_Non_Zero_Tab(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_شمار_Non_Zero_Tab, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شده HAVE_OPENCL
ساكن منطقي ocl_شمار_Non_Zero( درون_داد_Array _src, صحيح & res )
{
    صحيح type = _src.type(), عمق = CV_MAT_عمق(type), kercn = ocl::predict_Optimal_وکتور_عرض(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (عمق == CV_64F && !doubleSupport)
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروه_اندازه();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(عمق, kercn)),
                         ocl::type_مقصد_Str(عمق), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(src), src.تعداد_ستون, (صحيح)src.مجموع(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    إذا (k.پیاپی(1, &globalsize, &wgs, درست))
        أرجع res = saturate_cast<صحيح>(cv::جمع(db.getMat(ACCESS_READ))[0]), درست;
    أرجع خطأ;
}
#پایان_اگر

#إذا defined HAVE_IPP
ساكن منطقي ipp_شمار_Non_Zero( Mat &src, صحيح &res )
{
    CV_INSTRUMENT_REGION_IPP();

#إذا IPP_نسخه_X100 < 201801
    // Poor performance of SSE42
    إذا(cv::ipp::get_Ipp_بالاترین_Features() == ippCPUID_SSE42)
        أرجع خطأ;
#پایان_اگر

    Ipp32s  شمار_2 = 0;
    صحيح     عمق = src.عمق();

    إذا(src.dims <= 2)
    {
        Ipp_وضعیت وضعیت;
        Ippi_اندازه  size = {src.تعداد_ستون*src.channels(), src.ردیفها};

        إذا(عمق == CV_8U)
            وضعیت = CV_INSTRUMENT_FUN_IPP(ippi_شمار_درون_Range_8u_C1_R, (ثابت Ipp8u *)src.ptr(), (صحيح)src.گام, size, &شمار_2, 0, 0);
        وإلا إذا(عمق == CV_32F)
            وضعیت = CV_INSTRUMENT_FUN_IPP(ippi_شمار_درون_Range_32f_C1_R, (ثابت Ipp32f *)src.ptr(), (صحيح)src.گام, size, &شمار_2, 0, 0);
        وإلا
            أرجع خطأ;

        إذا(وضعیت < 0)
            أرجع خطأ;

        res = size.عرض*size.ارتفاع - شمار_2;
    }
    وإلا
    {
        Ipp_وضعیت       وضعیت;
        ثابت Mat      *arrays[] = {&src, عدم};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_اندازه        size  = {(صحيح)it.size*src.channels(), 1};
        res = 0;
        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(عمق == CV_8U)
                وضعیت = CV_INSTRUMENT_FUN_IPP(ippi_شمار_درون_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (صحيح)it.planes->گام, size, &شمار_2, 0, 0);
            وإلا إذا(عمق == CV_32F)
                وضعیت = CV_INSTRUMENT_FUN_IPP(ippi_شمار_درون_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (صحيح)it.planes->گام, size, &شمار_2, 0, 0);
            وإلا
                أرجع خطأ;

            إذا(وضعیت < 0 || (صحيح)it.planes->مجموع()*src.channels() < شمار_2)
                أرجع خطأ;

            res += (صحيح)it.planes->مجموع()*src.channels() - شمار_2;
        }
    }

    أرجع درست;
}
#پایان_اگر

صحيح شمار_Non_Zero(درون_داد_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#إذا defined HAVE_OPENCL || defined HAVE_IPP
    صحيح res = -1;
#پایان_اگر

#اگر_تعریف_شده HAVE_OPENCL
    CV_OCL_پیاپی_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_شمار_Non_Zero(_src, res),
                res)
#پایان_اگر

    Mat src = _src.getMat();
    CV_IPP_پیاپی_FAST(ipp_شمار_Non_Zero(src, res), res);

    شمار_Non_Zero_Func func = get_شمار_Non_Zero_Tab(src.عمق());
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح مجموع = (صحيح)it.size, nz = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], مجموع );

    أرجع nz;
}

فراغ پیدا_کن_Non_Zero(درون_داد_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    صحيح عمق = src.عمق();
    std::vector<نقطه> idxvec;
    صحيح ردیفها = src.ردیفها, تعداد_ستون = src.تعداد_ستون;
    Auto_میانگیر<صحيح> buf_(تعداد_ستون + 1);
    صحيح* buf = buf_.داده();

    لكل( صحيح i = 0; i < ردیفها; i++ )
    {
        صحيح j, k = 0;
        ثابت uchar* ptr8 = src.ptr(i);
        إذا( عمق == CV_8U || عمق == CV_8S || عمق == CV_Bool || عمق == CV_8F_E4M3FNUZ )
        {
            j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 vzero = vx_setzero_u8();
            لكل( ; j <= تعداد_ستون - vlanes; j += vlanes )
            {
                إذا( !v_check_any(v_ne(vx_بارگیری(ptr8 + j), vzero)) )
                    استمر;
                لكل( صحيح t = j; t < j + vlanes; t++ )
                    إذا( ptr8[t] != 0 ) buf[k++] = t;
            }
#پایان_اگر
            لكل( ; j < تعداد_ستون; j++ )
                إذا( ptr8[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( عمق == CV_16U || عمق == CV_16S )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < تعداد_ستون; j++ )
                إذا( ptr16[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( عمق == CV_32S || عمق == CV_32U )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < تعداد_ستون; j++ )
                إذا( ptr32s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( عمق == CV_64S || عمق == CV_64U )
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < تعداد_ستون; j++ )
                إذا( ptr64s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( عمق == CV_32F )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < تعداد_ستون; j++ )
                إذا( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        وإلا إذا( عمق == CV_16F || عمق == CV_16BF )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < تعداد_ستون; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                إذا( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        وإلا إذا( عمق == CV_8F_E4M3FN )
        {
            لكل( j = 0; j < تعداد_ستون; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                إذا( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        وإلا
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < تعداد_ستون; j++ )
                إذا( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        إذا( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            لكل( j = 0; j < k; j++ )
                idxvec[sz + j] = نقطه(buf[j], i);
        }
    }

    إذا( idxvec.empty() || (_idx.نوع() == درون_داد_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    إذا( !idxvec.empty() )
        Mat(idxvec).copy_مقصد(_idx);
}

} // namespace
