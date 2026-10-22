// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "count_non_zero.simd.hpp"
#ضمن "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن ڳڻپ_Non_Zero_Func get_ڳڻپ_Non_Zero_Tab(صحيح اونهائي)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ڳڻپ_Non_Zero_Tab, (اونهائي),
        CV_CPU_DISPATCH_MODES_ALL);
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
ساكن منطقي ocl_ڳڻپ_Non_Zero( داخلا_Array _src, صحيح & res )
{
    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type), kercn = ocl::predict_Optimal_ويڪٽر_ويڪر(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (اونهائي == CV_64F && !doubleSupport)
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروهه_سائيز();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(اونهائي, kercn)),
                         ocl::type_منزل_Str(اونهائي), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(src), src.کانن_ڳڻپ, (صحيح)src.ڪل(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    إذا (k.لاڳيتو(1, &globalsize, &wgs, سچ))
        أرجع res = saturate_cast<صحيح>(cv::جوڙ(db.getMat(ACCESS_READ))[0]), سچ;
    أرجع خطأ;
}
#جيڪڏهن_پورو

#إذا defined HAVE_IPP
ساكن منطقي ipp_ڳڻپ_Non_Zero( Mat &src, صحيح &res )
{
    CV_INSTRUMENT_REGION_IPP();

#إذا IPP_نسخو_X100 < 201801
    // Poor performance of SSE42
    إذا(cv::ipp::get_Ipp_چوٽي_Features() == ippCPUID_SSE42)
        أرجع خطأ;
#جيڪڏهن_پورو

    Ipp32s  ڳڻپ_2 = 0;
    صحيح     اونهائي = src.اونهائي();

    إذا(src.dims <= 2)
    {
        Ipp_حالت حالت;
        Ippi_سائيز  size = {src.کانن_ڳڻپ*src.channels(), src.صفون};

        إذا(اونهائي == CV_8U)
            حالت = CV_INSTRUMENT_FUN_IPP(ippi_ڳڻپ_اندريون_Range_8u_C1_R, (ثابت Ipp8u *)src.ptr(), (صحيح)src.قدم, size, &ڳڻپ_2, 0, 0);
        وإلا إذا(اونهائي == CV_32F)
            حالت = CV_INSTRUMENT_FUN_IPP(ippi_ڳڻپ_اندريون_Range_32f_C1_R, (ثابت Ipp32f *)src.ptr(), (صحيح)src.قدم, size, &ڳڻپ_2, 0, 0);
        وإلا
            أرجع خطأ;

        إذا(حالت < 0)
            أرجع خطأ;

        res = size.ويڪر*size.اوچائي - ڳڻپ_2;
    }
    وإلا
    {
        Ipp_حالت       حالت;
        ثابت Mat      *arrays[] = {&src, عدم};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_سائيز        size  = {(صحيح)it.size*src.channels(), 1};
        res = 0;
        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(اونهائي == CV_8U)
                حالت = CV_INSTRUMENT_FUN_IPP(ippi_ڳڻپ_اندريون_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (صحيح)it.planes->قدم, size, &ڳڻپ_2, 0, 0);
            وإلا إذا(اونهائي == CV_32F)
                حالت = CV_INSTRUMENT_FUN_IPP(ippi_ڳڻپ_اندريون_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (صحيح)it.planes->قدم, size, &ڳڻپ_2, 0, 0);
            وإلا
                أرجع خطأ;

            إذا(حالت < 0 || (صحيح)it.planes->ڪل()*src.channels() < ڳڻپ_2)
                أرجع خطأ;

            res += (صحيح)it.planes->ڪل()*src.channels() - ڳڻپ_2;
        }
    }

    أرجع سچ;
}
#جيڪڏهن_پورو

صحيح ڳڻپ_Non_Zero(داخلا_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#إذا defined HAVE_OPENCL || defined HAVE_IPP
    صحيح res = -1;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    CV_OCL_لاڳيتو_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_ڳڻپ_Non_Zero(_src, res),
                res)
#جيڪڏهن_پورو

    Mat src = _src.getMat();
    CV_IPP_لاڳيتو_FAST(ipp_ڳڻپ_Non_Zero(src, res), res);

    ڳڻپ_Non_Zero_Func func = get_ڳڻپ_Non_Zero_Tab(src.اونهائي());
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح ڪل = (صحيح)it.size, nz = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], ڪل );

    أرجع nz;
}

فراغ ڳوليو_Non_Zero(داخلا_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    صحيح اونهائي = src.اونهائي();
    std::vector<نقطو> idxvec;
    صحيح صفون = src.صفون, کانن_ڳڻپ = src.کانن_ڳڻپ;
    Auto_بفر<صحيح> buf_(کانن_ڳڻپ + 1);
    صحيح* buf = buf_.معلومات();

    لكل( صحيح i = 0; i < صفون; i++ )
    {
        صحيح j, k = 0;
        ثابت uchar* ptr8 = src.ptr(i);
        إذا( اونهائي == CV_8U || اونهائي == CV_8S || اونهائي == CV_Bool || اونهائي == CV_8F_E4M3FNUZ )
        {
            j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 vzero = vx_setzero_u8();
            لكل( ; j <= کانن_ڳڻپ - vlanes; j += vlanes )
            {
                إذا( !v_check_any(v_ne(vx_ڀريو(ptr8 + j), vzero)) )
                    استمر;
                لكل( صحيح t = j; t < j + vlanes; t++ )
                    إذا( ptr8[t] != 0 ) buf[k++] = t;
            }
#جيڪڏهن_پورو
            لكل( ; j < کانن_ڳڻپ; j++ )
                إذا( ptr8[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( اونهائي == CV_16U || اونهائي == CV_16S )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < کانن_ڳڻپ; j++ )
                إذا( ptr16[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( اونهائي == CV_32S || اونهائي == CV_32U )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < کانن_ڳڻپ; j++ )
                إذا( ptr32s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( اونهائي == CV_64S || اونهائي == CV_64U )
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < کانن_ڳڻپ; j++ )
                إذا( ptr64s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( اونهائي == CV_32F )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < کانن_ڳڻپ; j++ )
                إذا( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        وإلا إذا( اونهائي == CV_16F || اونهائي == CV_16BF )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < کانن_ڳڻپ; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                إذا( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        وإلا إذا( اونهائي == CV_8F_E4M3FN )
        {
            لكل( j = 0; j < کانن_ڳڻپ; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                إذا( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        وإلا
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < کانن_ڳڻپ; j++ )
                إذا( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        إذا( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            لكل( j = 0; j < k; j++ )
                idxvec[sz + j] = نقطو(buf[j], i);
        }
    }

    إذا( idxvec.empty() || (_idx.قسم() == داخلا_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    إذا( !idxvec.empty() )
        Mat(idxvec).copy_منزل(_idx);
}

} // namespace
