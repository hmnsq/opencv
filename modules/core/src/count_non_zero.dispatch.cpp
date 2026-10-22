// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "count_non_zero.simd.hpp"
#ضمن "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن شمېر_Non_Zero_Func get_شمېر_Non_Zero_Tab(صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_شمېر_Non_Zero_Tab, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#که_تعریف_شوی HAVE_OPENCL
ساكن منطقي ocl_شمېر_Non_Zero( ننوت_Array _src, صحيح & res )
{
    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type), kercn = ocl::predict_Optimal_ویکټور_پلنوالی(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (ژوروالی == CV_64F && !doubleSupport)
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ډله_اندازه();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, kercn)),
                         ocl::type_منزل_Str(ژوروالی), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(src), src.ستون_شمېر, (صحيح)src.ټول(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    إذا (k.پرلپسې(1, &globalsize, &wgs, ریښتیا))
        أرجع res = saturate_cast<صحيح>(cv::مجموعه(db.getMat(ACCESS_READ))[0]), ریښتیا;
    أرجع خطأ;
}
#د_که_پای

#إذا defined HAVE_IPP
ساكن منطقي ipp_شمېر_Non_Zero( Mat &src, صحيح &res )
{
    CV_INSTRUMENT_REGION_IPP();

#إذا IPP_نسخه_X100 < 201801
    // Poor performance of SSE42
    إذا(cv::ipp::get_Ipp_سر_Features() == ippCPUID_SSE42)
        أرجع خطأ;
#د_که_پای

    Ipp32s  شمېر_2 = 0;
    صحيح     ژوروالی = src.ژوروالی();

    إذا(src.dims <= 2)
    {
        Ipp_حالت حالت;
        Ippi_اندازه  size = {src.ستون_شمېر*src.channels(), src.لیکې};

        إذا(ژوروالی == CV_8U)
            حالت = CV_INSTRUMENT_FUN_IPP(ippi_شمېر_دننه_Range_8u_C1_R, (ثابت Ipp8u *)src.ptr(), (صحيح)src.ګام, size, &شمېر_2, 0, 0);
        وإلا إذا(ژوروالی == CV_32F)
            حالت = CV_INSTRUMENT_FUN_IPP(ippi_شمېر_دننه_Range_32f_C1_R, (ثابت Ipp32f *)src.ptr(), (صحيح)src.ګام, size, &شمېر_2, 0, 0);
        وإلا
            أرجع خطأ;

        إذا(حالت < 0)
            أرجع خطأ;

        res = size.پلنوالی*size.لوړوالی - شمېر_2;
    }
    وإلا
    {
        Ipp_حالت       حالت;
        ثابت Mat      *arrays[] = {&src, عدم};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_اندازه        size  = {(صحيح)it.size*src.channels(), 1};
        res = 0;
        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(ژوروالی == CV_8U)
                حالت = CV_INSTRUMENT_FUN_IPP(ippi_شمېر_دننه_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (صحيح)it.planes->ګام, size, &شمېر_2, 0, 0);
            وإلا إذا(ژوروالی == CV_32F)
                حالت = CV_INSTRUMENT_FUN_IPP(ippi_شمېر_دننه_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (صحيح)it.planes->ګام, size, &شمېر_2, 0, 0);
            وإلا
                أرجع خطأ;

            إذا(حالت < 0 || (صحيح)it.planes->ټول()*src.channels() < شمېر_2)
                أرجع خطأ;

            res += (صحيح)it.planes->ټول()*src.channels() - شمېر_2;
        }
    }

    أرجع ریښتیا;
}
#د_که_پای

صحيح شمېر_Non_Zero(ننوت_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#إذا defined HAVE_OPENCL || defined HAVE_IPP
    صحيح res = -1;
#د_که_پای

#که_تعریف_شوی HAVE_OPENCL
    CV_OCL_پرلپسې_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_شمېر_Non_Zero(_src, res),
                res)
#د_که_پای

    Mat src = _src.getMat();
    CV_IPP_پرلپسې_FAST(ipp_شمېر_Non_Zero(src, res), res);

    شمېر_Non_Zero_Func func = get_شمېر_Non_Zero_Tab(src.ژوروالی());
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح ټول = (صحيح)it.size, nz = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], ټول );

    أرجع nz;
}

فراغ پیدا_کړه_Non_Zero(ننوت_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    صحيح ژوروالی = src.ژوروالی();
    std::vector<ټکی> idxvec;
    صحيح لیکې = src.لیکې, ستون_شمېر = src.ستون_شمېر;
    Auto_بفر<صحيح> buf_(ستون_شمېر + 1);
    صحيح* buf = buf_.معلومات();

    لكل( صحيح i = 0; i < لیکې; i++ )
    {
        صحيح j, k = 0;
        ثابت uchar* ptr8 = src.ptr(i);
        إذا( ژوروالی == CV_8U || ژوروالی == CV_8S || ژوروالی == CV_Bool || ژوروالی == CV_8F_E4M3FNUZ )
        {
            j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 vzero = vx_setzero_u8();
            لكل( ; j <= ستون_شمېر - vlanes; j += vlanes )
            {
                إذا( !v_check_any(v_ne(vx_بارول(ptr8 + j), vzero)) )
                    استمر;
                لكل( صحيح t = j; t < j + vlanes; t++ )
                    إذا( ptr8[t] != 0 ) buf[k++] = t;
            }
#د_که_پای
            لكل( ; j < ستون_شمېر; j++ )
                إذا( ptr8[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( ژوروالی == CV_16U || ژوروالی == CV_16S )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < ستون_شمېر; j++ )
                إذا( ptr16[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( ژوروالی == CV_32S || ژوروالی == CV_32U )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < ستون_شمېر; j++ )
                إذا( ptr32s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( ژوروالی == CV_64S || ژوروالی == CV_64U )
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < ستون_شمېر; j++ )
                إذا( ptr64s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( ژوروالی == CV_32F )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < ستون_شمېر; j++ )
                إذا( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        وإلا إذا( ژوروالی == CV_16F || ژوروالی == CV_16BF )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < ستون_شمېر; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                إذا( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        وإلا إذا( ژوروالی == CV_8F_E4M3FN )
        {
            لكل( j = 0; j < ستون_شمېر; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                إذا( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        وإلا
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < ستون_شمېر; j++ )
                إذا( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        إذا( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            لكل( j = 0; j < k; j++ )
                idxvec[sz + j] = ټکی(buf[j], i);
        }
    }

    إذا( idxvec.empty() || (_idx.ډول() == ننوت_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    إذا( !idxvec.empty() )
        Mat(idxvec).copy_منزل(_idx);
}

} // namespace
