// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"

#شامل "count_non_zero.simd.hpp"
#شامل "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv {

ساکن گنتی_Non_Zero_Func get_گنتی_Non_Zero_Tab(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_گنتی_Non_Zero_Tab, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#اگر_تعریف_شدہ HAVE_OPENCL
ساکن منطقی ocl_گنتی_Non_Zero( داخلہ_Array _src, صحیح & res )
{
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), kercn = ocl::predict_Optimal_سمتیہ_چوڑائی(_src);
    منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    اگر (گہرائی == CV_64F && !doubleSupport)
        واپس جھوٹ;

    صحیح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروہ_سائز();

    صحیح wgs2_aligned = 1;
    جب (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)),
                         ocl::type_مقصد_Str(گہرائی), (صحیح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    اگر (k.empty())
        واپس جھوٹ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), src.ستون_تعداد, (صحیح)src.کل(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    اگر (k.تسلسل(1, &globalsize, &wgs, سچ))
        واپس res = saturate_cast<صحیح>(cv::مجموع(db.getMat(ACCESS_READ))[0]), سچ;
    واپس جھوٹ;
}
#اگر_ختم

#اگر defined HAVE_IPP
ساکن منطقی ipp_گنتی_Non_Zero( Mat &src, صحیح &res )
{
    CV_INSTRUMENT_REGION_IPP();

#اگر IPP_نسخہ_X100 < 201801
    // Poor performance of SSE42
    اگر(cv::ipp::get_Ipp_چوٹی_Features() == ippCPUID_SSE42)
        واپس جھوٹ;
#اگر_ختم

    Ipp32s  گنتی_2 = 0;
    صحیح     گہرائی = src.گہرائی();

    اگر(src.dims <= 2)
    {
        Ipp_حالت حالت_2;
        Ippi_سائز  size = {src.ستون_تعداد*src.channels(), src.صفیں};

        اگر(گہرائی == CV_8U)
            حالت_2 = CV_INSTRUMENT_FUN_IPP(ippi_گنتی_اندر_Range_8u_C1_R, (ثابت Ipp8u *)src.ptr(), (صحیح)src.قدم, size, &گنتی_2, 0, 0);
        ورنہ اگر(گہرائی == CV_32F)
            حالت_2 = CV_INSTRUMENT_FUN_IPP(ippi_گنتی_اندر_Range_32f_C1_R, (ثابت Ipp32f *)src.ptr(), (صحیح)src.قدم, size, &گنتی_2, 0, 0);
        ورنہ
            واپس جھوٹ;

        اگر(حالت_2 < 0)
            واپس جھوٹ;

        res = size.چوڑائی*size.اونچائی - گنتی_2;
    }
    ورنہ
    {
        Ipp_حالت       حالت_2;
        ثابت Mat      *arrays[] = {&src, تہی};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_سائز        size  = {(صحیح)it.size*src.channels(), 1};
        res = 0;
        برائے (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            اگر(گہرائی == CV_8U)
                حالت_2 = CV_INSTRUMENT_FUN_IPP(ippi_گنتی_اندر_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (صحیح)it.planes->قدم, size, &گنتی_2, 0, 0);
            ورنہ اگر(گہرائی == CV_32F)
                حالت_2 = CV_INSTRUMENT_FUN_IPP(ippi_گنتی_اندر_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (صحیح)it.planes->قدم, size, &گنتی_2, 0, 0);
            ورنہ
                واپس جھوٹ;

            اگر(حالت_2 < 0 || (صحیح)it.planes->کل()*src.channels() < گنتی_2)
                واپس جھوٹ;

            res += (صحیح)it.planes->کل()*src.channels() - گنتی_2;
        }
    }

    واپس سچ;
}
#اگر_ختم

صحیح گنتی_Non_Zero(داخلہ_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#اگر defined HAVE_OPENCL || defined HAVE_IPP
    صحیح res = -1;
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENCL
    CV_OCL_تسلسل_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_گنتی_Non_Zero(_src, res),
                res)
#اگر_ختم

    Mat src = _src.getMat();
    CV_IPP_تسلسل_FAST(ipp_گنتی_Non_Zero(src, res), res);

    گنتی_Non_Zero_Func func = get_گنتی_Non_Zero_Tab(src.گہرائی());
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح کل = (صحیح)it.size, nz = 0;

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], کل );

    واپس nz;
}

خالی ڈھونڈو_Non_Zero(داخلہ_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    صحیح گہرائی = src.گہرائی();
    std::vector<نقطہ> idxvec;
    صحیح صفیں = src.صفیں, ستون_تعداد = src.ستون_تعداد;
    Auto_بفر<صحیح> buf_(ستون_تعداد + 1);
    صحیح* buf = buf_.ڈیٹا();

    برائے( صحیح i = 0; i < صفیں; i++ )
    {
        صحیح j, k = 0;
        ثابت uchar* ptr8 = src.ptr(i);
        اگر( گہرائی == CV_8U || گہرائی == CV_8S || گہرائی == CV_Bool || گہرائی == CV_8F_E4M3FNUZ )
        {
            j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ثابت صحیح vlanes = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 vzero = vx_setzero_u8();
            برائے( ; j <= ستون_تعداد - vlanes; j += vlanes )
            {
                اگر( !v_check_any(v_ne(vx_بھرو(ptr8 + j), vzero)) )
                    جاری;
                برائے( صحیح t = j; t < j + vlanes; t++ )
                    اگر( ptr8[t] != 0 ) buf[k++] = t;
            }
#اگر_ختم
            برائے( ; j < ستون_تعداد; j++ )
                اگر( ptr8[j] != 0 ) buf[k++] = j;
        }
        ورنہ اگر( گہرائی == CV_16U || گہرائی == CV_16S )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            برائے( j = 0; j < ستون_تعداد; j++ )
                اگر( ptr16[j] != 0 ) buf[k++] = j;
        }
        ورنہ اگر( گہرائی == CV_32S || گہرائی == CV_32U )
        {
            ثابت صحیح* ptr32s = (ثابت صحیح*)ptr8;
            برائے( j = 0; j < ستون_تعداد; j++ )
                اگر( ptr32s[j] != 0 ) buf[k++] = j;
        }
        ورنہ اگر( گہرائی == CV_64S || گہرائی == CV_64U )
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            برائے( j = 0; j < ستون_تعداد; j++ )
                اگر( ptr64s[j] != 0 ) buf[k++] = j;
        }
        ورنہ اگر( گہرائی == CV_32F )
        {
            ثابت صحیح* ptr32s = (ثابت صحیح*)ptr8;
            برائے( j = 0; j < ستون_تعداد; j++ )
                اگر( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        ورنہ اگر( گہرائی == CV_16F || گہرائی == CV_16BF )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            برائے( j = 0; j < ستون_تعداد; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                اگر( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        ورنہ اگر( گہرائی == CV_8F_E4M3FN )
        {
            برائے( j = 0; j < ستون_تعداد; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                اگر( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        ورنہ
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            برائے( j = 0; j < ستون_تعداد; j++ )
                اگر( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        اگر( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            برائے( j = 0; j < k; j++ )
                idxvec[sz + j] = نقطہ(buf[j], i);
        }
    }

    اگر( idxvec.empty() || (_idx.قسم() == داخلہ_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    اگر( !idxvec.empty() )
        Mat(idxvec).copy_مقصد(_idx);
}

} // namespace
