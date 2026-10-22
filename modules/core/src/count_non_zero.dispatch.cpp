// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "count_non_zero.simd.hpp"
#ضمن "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن گنتی_Non_Zero_Func get_گنتی_Non_Zero_Tab(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_گنتی_Non_Zero_Tab, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

#جے_تعریف_شدہ HAVE_OPENCL
ساكن منطقي ocl_گنتی_Non_Zero( داخلہ_Array _src, صحيح & res )
{
    صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), kercn = ocl::predict_Optimal_سمتیہ_چوڑائی(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (ڈونگھائی == CV_64F && !doubleSupport)
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروہ_ناپ();

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, kercn)),
                         ocl::type_منزل_Str(ڈونگھائی), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    إذا (k.empty())
        أرجع خطأ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(src), src.خانے_گنتی, (صحيح)src.کل(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    إذا (k.لگاتار(1, &globalsize, &wgs, سچ))
        أرجع res = saturate_cast<صحيح>(cv::جوڑ(db.getMat(ACCESS_READ))[0]), سچ;
    أرجع خطأ;
}
#جے_انت

#إذا defined HAVE_IPP
ساكن منطقي ipp_گنتی_Non_Zero( Mat &src, صحيح &res )
{
    CV_INSTRUMENT_REGION_IPP();

#إذا IPP_نسخہ_X100 < 201801
    // Poor performance of SSE42
    إذا(cv::ipp::get_Ipp_چوٹی_Features() == ippCPUID_SSE42)
        أرجع خطأ;
#جے_انت

    Ipp32s  گنتی_2 = 0;
    صحيح     ڈونگھائی = src.ڈونگھائی();

    إذا(src.dims <= 2)
    {
        Ipp_حالت حالت;
        Ippi_ناپ  size = {src.خانے_گنتی*src.channels(), src.صفاں};

        إذا(ڈونگھائی == CV_8U)
            حالت = CV_INSTRUMENT_FUN_IPP(ippi_گنتی_وچ_Range_8u_C1_R, (ثابت Ipp8u *)src.ptr(), (صحيح)src.قدم, size, &گنتی_2, 0, 0);
        وإلا إذا(ڈونگھائی == CV_32F)
            حالت = CV_INSTRUMENT_FUN_IPP(ippi_گنتی_وچ_Range_32f_C1_R, (ثابت Ipp32f *)src.ptr(), (صحيح)src.قدم, size, &گنتی_2, 0, 0);
        وإلا
            أرجع خطأ;

        إذا(حالت < 0)
            أرجع خطأ;

        res = size.چوڑائی*size.اچائی - گنتی_2;
    }
    وإلا
    {
        Ipp_حالت       حالت;
        ثابت Mat      *arrays[] = {&src, عدم};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_ناپ        size  = {(صحيح)it.size*src.channels(), 1};
        res = 0;
        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(ڈونگھائی == CV_8U)
                حالت = CV_INSTRUMENT_FUN_IPP(ippi_گنتی_وچ_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (صحيح)it.planes->قدم, size, &گنتی_2, 0, 0);
            وإلا إذا(ڈونگھائی == CV_32F)
                حالت = CV_INSTRUMENT_FUN_IPP(ippi_گنتی_وچ_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (صحيح)it.planes->قدم, size, &گنتی_2, 0, 0);
            وإلا
                أرجع خطأ;

            إذا(حالت < 0 || (صحيح)it.planes->کل()*src.channels() < گنتی_2)
                أرجع خطأ;

            res += (صحيح)it.planes->کل()*src.channels() - گنتی_2;
        }
    }

    أرجع سچ;
}
#جے_انت

صحيح گنتی_Non_Zero(داخلہ_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#إذا defined HAVE_OPENCL || defined HAVE_IPP
    صحيح res = -1;
#جے_انت

#جے_تعریف_شدہ HAVE_OPENCL
    CV_OCL_لگاتار_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_گنتی_Non_Zero(_src, res),
                res)
#جے_انت

    Mat src = _src.getMat();
    CV_IPP_لگاتار_FAST(ipp_گنتی_Non_Zero(src, res), res);

    گنتی_Non_Zero_Func func = get_گنتی_Non_Zero_Tab(src.ڈونگھائی());
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح کل = (صحيح)it.size, nz = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], کل );

    أرجع nz;
}

فراغ لبھو_Non_Zero(داخلہ_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    صحيح ڈونگھائی = src.ڈونگھائی();
    std::vector<نقطہ> idxvec;
    صحيح صفاں = src.صفاں, خانے_گنتی = src.خانے_گنتی;
    Auto_بفر<صحيح> buf_(خانے_گنتی + 1);
    صحيح* buf = buf_.انکڑے();

    لكل( صحيح i = 0; i < صفاں; i++ )
    {
        صحيح j, k = 0;
        ثابت uchar* ptr8 = src.ptr(i);
        إذا( ڈونگھائی == CV_8U || ڈونگھائی == CV_8S || ڈونگھائی == CV_Bool || ڈونگھائی == CV_8F_E4M3FNUZ )
        {
            j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 vzero = vx_setzero_u8();
            لكل( ; j <= خانے_گنتی - vlanes; j += vlanes )
            {
                إذا( !v_check_any(v_ne(vx_لدو(ptr8 + j), vzero)) )
                    استمر;
                لكل( صحيح t = j; t < j + vlanes; t++ )
                    إذا( ptr8[t] != 0 ) buf[k++] = t;
            }
#جے_انت
            لكل( ; j < خانے_گنتی; j++ )
                إذا( ptr8[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( ڈونگھائی == CV_16U || ڈونگھائی == CV_16S )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < خانے_گنتی; j++ )
                إذا( ptr16[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( ڈونگھائی == CV_32S || ڈونگھائی == CV_32U )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < خانے_گنتی; j++ )
                إذا( ptr32s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( ڈونگھائی == CV_64S || ڈونگھائی == CV_64U )
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < خانے_گنتی; j++ )
                إذا( ptr64s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( ڈونگھائی == CV_32F )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < خانے_گنتی; j++ )
                إذا( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        وإلا إذا( ڈونگھائی == CV_16F || ڈونگھائی == CV_16BF )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < خانے_گنتی; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                إذا( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        وإلا إذا( ڈونگھائی == CV_8F_E4M3FN )
        {
            لكل( j = 0; j < خانے_گنتی; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                إذا( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        وإلا
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < خانے_گنتی; j++ )
                إذا( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        إذا( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            لكل( j = 0; j < k; j++ )
                idxvec[sz + j] = نقطہ(buf[j], i);
        }
    }

    إذا( idxvec.empty() || (_idx.قسم() == داخلہ_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    إذا( !idxvec.empty() )
        Mat(idxvec).copy_منزل(_idx);
}

} // namespace
