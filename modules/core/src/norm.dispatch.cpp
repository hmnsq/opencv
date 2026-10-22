// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"

#شامل "norm.simd.hpp"
#شامل "norm.simd_declarations.hpp"

/****************************************************************************************\
*                                         norm                                           *
\****************************************************************************************/

نامخلا cv { نامخلا hal {

خارجی ثابت uchar نکالو_گنتی_جدول[256] =
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

ساکن ثابت uchar نکالو_گنتی_Table2[] =
{
    0, 1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    1, 2, 2, 2, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4,
    2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 2, 3, 3, 3, 3, 4, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4
};

ساکن ثابت uchar نکالو_گنتی_Table4[] =
{
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};


صحیح normHamming(ثابت uchar* a, صحیح n, صحیح cell_سائز)
{
    صحیح output;
    CALL_HAL_RET(normHamming8u, cv_hal_normHamming8u, output, a, n, cell_سائز);

    اگر( cell_سائز == 1 )
        واپس normHamming(a, n);
    ثابت uchar* tab = 0;
    اگر( cell_سائز == 2 )
        tab = نکالو_گنتی_Table2;
    ورنہ اگر( cell_سائز == 4 )
        tab = نکالو_گنتی_Table4;
    ورنہ
        واپس -1;
    صحیح i = 0;
    صحیح نتیجہ = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    اگر ( cell_سائز == 2)
    {
        v_uint16 نقاب = v_reinterpret_as_u16(vx_setall_u8(0x55));
        برائے(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_بھرو(a + i));
            t = v_جوڑو(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a0, v_shr<1>(a0)), نقاب))));
        }
    }
    ورنہ    // cellSize == 4
    {
        v_uint16 نقاب = v_reinterpret_as_u16(vx_setall_u8(0x11));
        برائے(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 a0 = v_reinterpret_as_u16(vx_بھرو(a + i));
            v_uint16 a1 = v_or(a0, v_shr<2>(a0));
            t = v_جوڑو(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(a1, v_shr<1>(a1)), نقاب))));

        }
    }
    نتیجہ += (صحیح)v_reduce_مجموع(t);
    vx_cleanup();
#وگرنہ_اگر CV_ENABLE_UNROLLED
    برائے( ; i <= n - 4; i += 4 )
        نتیجہ += tab[a[i]] + tab[a[i+1]] + tab[a[i+2]] + tab[a[i+3]];
#اگر_ختم
    برائے( ; i < n; i++ )
        نتیجہ += tab[a[i]];
    واپس نتیجہ;
}

صحیح normHamming(ثابت uchar* a, ثابت uchar* b, صحیح n, صحیح cell_سائز)
{
    صحیح output;
    CALL_HAL_RET(normHammingDiff8u, cv_hal_normHammingDiff8u, output, a, b, n, cell_سائز);

    اگر( cell_سائز == 1 )
        واپس normHamming(a, b, n);
    ثابت uchar* tab = 0;
    اگر( cell_سائز == 2 )
        tab = نکالو_گنتی_Table2;
    ورنہ اگر( cell_سائز == 4 )
        tab = نکالو_گنتی_Table4;
    ورنہ
        واپس -1;
    صحیح i = 0;
    صحیح نتیجہ = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_uint64 t = vx_setzero_u64();
    اگر ( cell_سائز == 2)
    {
        v_uint16 نقاب = v_reinterpret_as_u16(vx_setall_u8(0x55));
        برائے(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_بھرو(a + i), vx_بھرو(b + i)));
            t = v_جوڑو(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab0, v_shr<1>(ab0)), نقاب))));
        }
    }
    ورنہ    // cellSize == 4
    {
        v_uint16 نقاب = v_reinterpret_as_u16(vx_setall_u8(0x11));
        برائے(; i <= n - VTraits<v_uint8>::vlanes(); i += VTraits<v_uint8>::vlanes())
        {
            v_uint16 ab0 = v_reinterpret_as_u16(v_xor(vx_بھرو(a + i), vx_بھرو(b + i)));
            v_uint16 ab1 = v_or(ab0, v_shr<2>(ab0));
            t = v_جوڑو(t, v_popcount(v_reinterpret_as_u64(v_and(v_or(ab1, v_shr<1>(ab1)), نقاب))));
        }
    }
    نتیجہ += (صحیح)v_reduce_مجموع(t);
    vx_cleanup();
#وگرنہ_اگر CV_ENABLE_UNROLLED
    برائے( ; i <= n - 4; i += 4 )
        نتیجہ += tab[a[i] ^ b[i]] + tab[a[i+1] ^ b[i+1]] +
                tab[a[i+2] ^ b[i+2]] + tab[a[i+3] ^ b[i+3]];
#اگر_ختم
    برائے( ; i < n; i++ )
        نتیجہ += tab[a[i] ^ b[i]];
    واپس نتیجہ;
}

اعشاری normL2Sqr_(ثابت اعشاری* a, ثابت اعشاری* b, صحیح n)
{
    صحیح j = 0; اعشاری d = 0.f;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    برائے (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_float32 t0 = v_sub(vx_بھرو(a + j), vx_بھرو(b + j));
        v_float32 t1 = v_sub(vx_بھرو(a + j + vl), vx_بھرو(b + j + vl));
        v_d0 = v_muladd(t0, t0, v_d0);
        v_float32 t2 = v_sub(vx_بھرو(a + j + 2 * vl), vx_بھرو(b + j + 2 * vl));
        v_d1 = v_muladd(t1, t1, v_d1);
        v_float32 t3 = v_sub(vx_بھرو(a + j + 3 * vl), vx_بھرو(b + j + 3 * vl));
        v_d2 = v_muladd(t2, t2, v_d2);
        v_d3 = v_muladd(t3, t3, v_d3);
    }
    v_d0 = v_جوڑو(v_جوڑو(v_d0, v_d1), v_جوڑو(v_d2, v_d3));
    برائے (; j <= n - vl; j += vl)
    {
        v_float32 t0 = v_sub(vx_بھرو(a + j), vx_بھرو(b + j));
        v_d0 = v_muladd(t0, t0, v_d0);
    }
    d = v_reduce_مجموع(v_d0);
#اگر_ختم
    برائے( ; j < n; j++ )
    {
        اعشاری t = a[j] - b[j];
        d += t*t;
    }
    واپس d;
}


اعشاری normL1_(ثابت اعشاری* a, ثابت اعشاری* b, صحیح n)
{
    صحیح j = 0; اعشاری d = 0.f;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح vl = VTraits<v_float32>::vlanes();
    v_float32 v_d0 = vx_setzero_f32(), v_d1 = vx_setzero_f32();
    v_float32 v_d2 = vx_setzero_f32(), v_d3 = vx_setzero_f32();
    برائے (; j <= n - 4 * vl; j += 4 * vl)
    {
        v_d0 = v_جوڑو(v_d0, v_absdiff(vx_بھرو(a + j), vx_بھرو(b + j)));
        v_d1 = v_جوڑو(v_d1, v_absdiff(vx_بھرو(a + j + vl), vx_بھرو(b + j + vl)));
        v_d2 = v_جوڑو(v_d2, v_absdiff(vx_بھرو(a + j + 2 * vl), vx_بھرو(b + j + 2 * vl)));
        v_d3 = v_جوڑو(v_d3, v_absdiff(vx_بھرو(a + j + 3 * vl), vx_بھرو(b + j + 3 * vl)));
    }
    v_d0 = v_جوڑو(v_جوڑو(v_d0, v_d1), v_جوڑو(v_d2, v_d3));
    برائے (; j <= n - vl; j += vl)
        v_d0 = v_جوڑو(v_d0, v_absdiff(vx_بھرو(a + j), vx_بھرو(b + j)));
    d = v_reduce_مجموع(v_d0);
#اگر_ختم
    برائے( ; j < n; j++ )
        d += std::مطلق_قدر(a[j] - b[j]);
    واپس d;
}

صحیح normL1_(ثابت uchar* a, ثابت uchar* b, صحیح n)
{
    صحیح j = 0, d = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    ثابت صحیح vl = VTraits<v_uint8>::vlanes();
    برائے (; j <= n - 4 * vl; j += 4 * vl)
        d += v_reduce_sad(vx_بھرو(a + j), vx_بھرو(b + j)) +
             v_reduce_sad(vx_بھرو(a + j + vl), vx_بھرو(b + j + vl)) +
             v_reduce_sad(vx_بھرو(a + j + 2 * vl), vx_بھرو(b + j + 2 * vl)) +
             v_reduce_sad(vx_بھرو(a + j + 3 * vl), vx_بھرو(b + j + 3 * vl));
    برائے (; j <= n - vl; j += vl)
        d += v_reduce_sad(vx_بھرو(a + j), vx_بھرو(b + j));
#اگر_ختم
    برائے( ; j < n; j++ )
        d += std::مطلق_قدر(a[j] - b[j]);
    واپس d;
}

} //cv::hal

//==================================================================================================

قسمتعریف صحیح (*NormFunc)(ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح);
قسمتعریف صحیح (*NormDiffFunc)(ثابت uchar*, ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح);

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_norm( داخلہ_Array _src, صحیح normType, داخلہ_Array نقاب_2, دوہرا & نتیجہ )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

#اگر_تعریف_شدہ __ANDROID__
    اگر (d.isNVidia())
        واپس جھوٹ;
#اگر_ختم
    ثابت صحیح cn = _src.channels();
    اگر (cn > 4)
        واپس جھوٹ;
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type);
    منطقی doubleSupport = d.doubleFPConfig() > 0,
            have_نقاب = نقاب_2.قسم() != داخلہ_Array_2::NONE;

    اگر (گہرائی >= CV_16F)
        واپس جھوٹ;  // TODO: support FP16

    اگر ( !(normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR) ||
         (!doubleSupport && گہرائی == CV_64F))
        واپس جھوٹ;

    UMat src = _src.getUMat();

    اگر (normType == NORM_INF)
    {
        اگر (!ocl_minMaxIdx(_src, تہی, &نتیجہ, تہی, تہی, نقاب_2,
                           std::max(گہرائی, CV_32S), گہرائی != CV_8U && گہرائی != CV_16U))
            واپس جھوٹ;
    }
    ورنہ اگر (normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR)
    {
        Scalar sc;
        منطقی unstype = گہرائی == CV_8U || گہرائی == CV_16U;

        اگر ( !ocl_مجموع(have_نقاب ? src : src.reshape(1), sc, normType == NORM_L2 || normType == NORM_L2SQR ?
                    OCL_OP_مجموع_SQR : (unstype ? OCL_OP_مجموع : OCL_OP_مجموع_ABS), نقاب_2) )
            واپس جھوٹ;

        دوہرا s = 0.0;
        برائے (صحیح i = 0; i < (have_نقاب ? cn : 1); ++i)
            s += sc[i];

        نتیجہ = normType == NORM_L1 || normType == NORM_L2SQR ? s : std::جذر(s);
    }

    واپس سچ;
}

#اگر_ختم

ساکن NormFunc getNormFunc(صحیح normType, صحیح گہرائی) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormFunc, (normType, گہرائی), CV_CPU_DISPATCH_MODES_ALL);
}
ساکن NormDiffFunc getNormDiffFunc(صحیح normType, صحیح گہرائی) {
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getNormDiffFunc, (normType, گہرائی), CV_CPU_DISPATCH_MODES_ALL);
}

دوہرا norm( داخلہ_Array _src, صحیح normType, داخلہ_Array نقاب_2 )
{
    CV_INSTRUMENT_REGION();

    normType &= NORM_TYPE_نقاب;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
               ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && _src.type() == CV_8U) );

#اگر defined HAVE_OPENCL
    دوہرا نتیجہ_2 = 0;
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENCL
    CV_OCL_تسلسل_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_norm(_src, normType, نقاب_2, نتیجہ_2),
                نتیجہ_2)
#اگر_ختم

    Mat src = _src.getMat(), نقاب = نقاب_2.getMat();
    CV_Assert( نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src.size == نقاب.size) );
    اگر( src.empty() )
        واپس 0.0;
    صحیح گہرائی = src.گہرائی(), cn = src.channels();
    اگر( src.dims <= 2 )
    {
        دوہرا نتیجہ;
        CALL_HAL_RET(norm, cv_hal_norm, نتیجہ, src.ڈیٹا, src.قدم, نقاب.ڈیٹا, نقاب.قدم, src.ستون_تعداد, src.صفیں, src.type(), normType);
    }
    ورنہ اگر( src.isContinuous() && نقاب.isContinuous() )
    {
        دوہرا نتیجہ;
        CALL_HAL_RET(norm, cv_hal_norm, نتیجہ, src.ڈیٹا, 0, نقاب.ڈیٹا, 0, (صحیح)src.کل(), 1, src.type(), normType);
    }

    NormFunc func = getNormFunc(normType >> 1, گہرائی);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    اگر( src.isContinuous() && نقاب.empty() )
    {
        size_t len = src.کل()*cn;
        اگر( len == (size_t)(صحیح)len )
        {
            اگر( گہرائی == CV_32F )
            {
                ثابت uchar* ڈیٹا = src.ptr<ثابت uchar>();

                اگر( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    دوہرا نتیجہ = 0;
                    func(ڈیٹا, 0, (uchar*)&نتیجہ, (صحیح)len, 1);
                    واپس normType == NORM_L2 ? std::جذر(نتیجہ) : نتیجہ;
                }
                اگر( normType == NORM_INF )
                {
                    اعشاری نتیجہ = 0;
                    func(ڈیٹا, 0, (uchar*)&نتیجہ, (صحیح)len, 1);
                    واپس نتیجہ;
                }
            }
            اگر( گہرائی == CV_8U )
            {
                ثابت uchar* ڈیٹا = src.ptr<uchar>();

                اگر( normType == NORM_HAMMING )
                {
                    واپس hal::normHamming(ڈیٹا, (صحیح)len, 1);
                }

                اگر( normType == NORM_HAMMING2 )
                {
                    واپس hal::normHamming(ڈیٹا, (صحیح)len, 2);
                }
            }
        }
    }

    CV_Assert( نقاب.empty() || نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool );

    اگر( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        اگر( !نقاب.empty() )
        {
            Mat عارضی_2;
            bitwise_and(src, نقاب, عارضی_2);
            واپس norm(عارضی_2, normType);
        }
        صحیح cell_سائز = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src, 0};
        uchar* ptrs[1] = {};
        NAryMatIterator it(arrays, ptrs);
        صحیح کل = (صحیح)it.size;
        صحیح نتیجہ = 0;

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            نتیجہ += hal::normHamming(ptrs[0], کل, cell_سائز);
        }

        واپس نتیجہ;
    }

    ثابت Mat* arrays[] = {&src, &نقاب, 0};
    uchar* ptrs[2] = {};
    اتحاد
    {
        دوہرا d;
        غیرعلامتی u;
        uint64 UL;
        اعشاری f;
    }
    نتیجہ;
    نتیجہ.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)زیادہ_سے_زیادہ_عدد, "");
    منطقی is_fp16 = گہرائی == CV_16F || گہرائی == CV_16BF;

    اگر ((normType == NORM_L1 && (گہرائی <= CV_16S || گہرائی == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (گہرائی <= CV_8S || گہرائی == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src.elem_سائز();
        ثابت صحیح کل = (صحیح)it.size;
        ثابت صحیح بلاک_Size0 = (is_fp16 ? (1 << 10) :
            گہرائی == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && گہرائی <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحیح بلاک_سائز_2 = std::min(کل, بلاک_Size0);
        اتحاد {
            صحیح i;
            اعشاری f;
        } blocksum;
        blocksum.i = 0;
        صحیح گنتی_2 = 0;

        برائے (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            برائے (صحیح j = 0; j < کل; j += بلاک_سائز_2)
            {
                صحیح bsz = std::min(کل - j, بلاک_سائز_2);
                func(ptrs[0], ptrs[1], (uchar*)&blocksum, bsz, cn);
                گنتی_2 += bsz;
                اگر (گنتی_2 + بلاک_سائز_2 >= بلاک_Size0 || (i+1 >= it.nplanes && j+bsz >= کل))
                {
                    نتیجہ.d += is_fp16 ? (دوہرا)blocksum.f : (دوہرا)blocksum.i;
                    blocksum.i = 0;
                    گنتی_2 = 0;
                }
                ptrs[0] += bsz*esz;
                اگر (ptrs[1])
                    ptrs[1] += bsz;
            }
        }
    }
    ورنہ
    {
        // generic implementation
        برائے (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], (uchar*)&نتیجہ, (صحیح)it.size, cn);
        }
    }

    اگر( normType == NORM_INF )
    {
        اگر(گہرائی <= CV_32S || گہرائی == CV_32U || گہرائی == CV_Bool)
            واپس نتیجہ.u;
        اگر (گہرائی == CV_32F || is_fp16)
            واپس نتیجہ.f;
        اگر (گہرائی == CV_64U || گہرائی == CV_64S)
            واپس (دوہرا)نتیجہ.UL;
    }
    اگر( normType == NORM_L2 )
        واپس std::جذر(نتیجہ.d);

    واپس نتیجہ.d;
}

//==================================================================================================

#اگر_تعریف_شدہ HAVE_OPENCL
ساکن منطقی ocl_norm( داخلہ_Array _src1, داخلہ_Array _src2, صحیح normType, داخلہ_Array نقاب_2, دوہرا & نتیجہ )
{
#اگر_تعریف_شدہ __ANDROID__
    اگر (ocl::Device::getDefault().isNVidia())
        واپس جھوٹ;
#اگر_ختم

    Scalar sc1, sc2;
    صحیح cn = _src1.channels();
    اگر (cn > 4)
        واپس جھوٹ;
    صحیح type = _src1.type(), گہرائی = CV_MAT_گہرائی(type);
    منطقی relative = (normType & NORM_RELATIVE) != 0;
    normType &= ~NORM_RELATIVE;
    منطقی normsum = normType == NORM_L1 || normType == NORM_L2 || normType == NORM_L2SQR;

#اگر_تعریف_شدہ __APPLE__
    اگر(normType == NORM_L1 && type == CV_16UC3 && !نقاب_2.empty())
        واپس جھوٹ;
#اگر_ختم

    اگر (normsum)
    {
        اگر (!ocl_مجموع(_src1, sc1, normType == NORM_L2 || normType == NORM_L2SQR ?
                     OCL_OP_مجموع_SQR : OCL_OP_مجموع, نقاب_2, _src2, relative, sc2))
            واپس جھوٹ;
    }
    ورنہ
    {
        اگر (!ocl_minMaxIdx(_src1, تہی, &sc1[0], تہی, تہی, نقاب_2, std::max(CV_32S, گہرائی),
                           جھوٹ, _src2, relative ? &sc2[0] : تہی))
            واپس جھوٹ;
        cn = 1;
    }

    دوہرا s2 = 0;
    برائے (صحیح i = 0; i < cn; ++i)
    {
        نتیجہ += sc1[i];
        اگر (relative)
            s2 += sc2[i];
    }

    اگر (normType == NORM_L2)
    {
        نتیجہ = std::جذر(نتیجہ);
        اگر (relative)
            s2 = std::جذر(s2);
    }

    اگر (relative)
        نتیجہ /= (s2 + DBL_EPSILON);

    واپس سچ;
}  // ocl_norm()
#اگر_ختم  // HAVE_OPENCL

دوہرا norm( داخلہ_Array _src1, داخلہ_Array _src2, صحیح normType, داخلہ_Array نقاب_2 )
{
    CV_INSTRUMENT_REGION();

    CV_CheckTypeEQ(_src1.type(), _src2.type(), "Input type mismatch");
    CV_Assert(_src1.same_سائز(_src2));

#اگر defined HAVE_OPENCL
    دوہرا نتیجہ_2 = 0;
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENCL
    CV_OCL_تسلسل_2(OCL_PERFORMANCE_CHECK(_src1.isUMat()),
                ocl_norm(_src1, _src2, normType, نقاب_2, نتیجہ_2),
                نتیجہ_2)
#اگر_ختم

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), نقاب = نقاب_2.getMat();
    CV_Assert( نقاب.empty() || ((نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool) && src1.size == نقاب.size) );
    اگر( src1.empty() || src2.empty() )
        واپس 0.0;
    صحیح گہرائی = src1.گہرائی(), cn = src1.channels();
    اگر( src1.dims <= 2 )
    {
        دوہرا نتیجہ;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, نتیجہ, src1.ڈیٹا, src1.قدم, src2.ڈیٹا, src2.قدم, نقاب.ڈیٹا, نقاب.قدم, src1.ستون_تعداد, src1.صفیں, src1.type(), normType);
    }
    ورنہ اگر( src1.isContinuous() && src2.isContinuous() && نقاب.isContinuous() )
    {
        دوہرا نتیجہ;
        CALL_HAL_RET(normDiff, cv_hal_normDiff, نتیجہ, src1.ڈیٹا, 0, src2.ڈیٹا, 0, نقاب.ڈیٹا, 0, (صحیح)src1.کل(), 1, src1.type(), normType);
    }

    اگر( normType & NORM_RELATIVE )
    {
        واپس norm(_src1, _src2, normType & ~NORM_RELATIVE, نقاب_2)/(norm(_src2, normType, نقاب_2) + DBL_EPSILON);
    }

    normType &= 7;
    CV_Assert( normType == NORM_INF || normType == NORM_L1 ||
               normType == NORM_L2 || normType == NORM_L2SQR ||
              ((normType == NORM_HAMMING || normType == NORM_HAMMING2) && src1.type() == CV_8U) );

    NormDiffFunc func = getNormDiffFunc(normType >> 1, گہرائی);
    CV_Assert( (normType >> 1) >= 3 || func != 0 );

    اگر( src1.isContinuous() && src2.isContinuous() && نقاب.empty() )
    {
        size_t len = src1.کل()*src1.channels();
        اگر( len == (size_t)(صحیح)len )
        {
            اگر( src1.گہرائی() == CV_32F )
            {
                ثابت uchar* data1 = src1.ptr<ثابت uchar>();
                ثابت uchar* data2 = src2.ptr<ثابت uchar>();

                اگر( normType == NORM_L2 || normType == NORM_L2SQR || normType == NORM_L1 )
                {
                    دوہرا نتیجہ = 0;
                    func(data1, data2, 0, (uchar*)&نتیجہ, (صحیح)len, 1);
                    واپس normType == NORM_L2 ? std::جذر(نتیجہ) : نتیجہ;
                }
                اگر( normType == NORM_INF )
                {
                    اعشاری نتیجہ = 0;
                    func(data1, data2, 0, (uchar*)&نتیجہ, (صحیح)len, 1);
                    واپس نتیجہ;
                }
            }
        }
    }

    CV_Assert( نقاب.empty() || نقاب.type() == CV_8U || نقاب.type() == CV_8S || نقاب.type() == CV_Bool );

    اگر( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
    {
        اگر( !نقاب.empty() )
        {
            Mat عارضی_2;
            bitwise_xor(src1, src2, عارضی_2);
            bitwise_and(عارضی_2, نقاب, عارضی_2);
            واپس norm(عارضی_2, normType);
        }
        صحیح cell_سائز = normType == NORM_HAMMING ? 1 : 2;

        ثابت Mat* arrays[] = {&src1, &src2, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحیح کل = (صحیح)it.size;
        صحیح نتیجہ = 0;

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            نتیجہ += hal::normHamming(ptrs[0], ptrs[1], کل, cell_سائز);
        }

        واپس نتیجہ;
    }

    ثابت Mat* arrays[] = {&src1, &src2, &نقاب, 0};
    uchar* ptrs[3] = {};
    اتحاد
    {
        دوہرا d;
        اعشاری f;
        غیرعلامتی u;
        uint64 UL;
    }
    نتیجہ;
    نتیجہ.d = 0;
    NAryMatIterator it(arrays, ptrs);
    CV_CheckLT((size_t)it.size, (size_t)زیادہ_سے_زیادہ_عدد, "");

    منطقی is_fp16 = گہرائی == CV_16F || گہرائی == CV_16BF;

    اگر ((normType == NORM_L1 && (گہرائی <= CV_16S || گہرائی == CV_Bool || is_fp16)) ||
        ((normType == NORM_L2 || normType == NORM_L2SQR) && (گہرائی <= CV_8S || گہرائی == CV_Bool || is_fp16)))
    {
        // special case to handle "integer" overflow in accumulator
        ثابت size_t esz = src1.elem_سائز();
        ثابت صحیح کل = (صحیح)it.size;
        ثابت صحیح بلاک_Size0 = (is_fp16 ? (1 << 10) :
            گہرائی == CV_Bool ? (1 << 30) :
            normType == NORM_L1 && گہرائی <= CV_8S ? (1 << 23) : (1 << 15))/cn;
        ثابت صحیح بلاک_سائز_2 = std::min(کل, بلاک_Size0);
        اتحاد {
            صحیح i;
            اعشاری f;
        } blocksum;
        blocksum.i = 0;
        صحیح گنتی_2 = 0;

        برائے (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            برائے (صحیح j = 0; j < کل; j += بلاک_سائز_2)
            {
                صحیح bsz = std::min(کل - j, بلاک_سائز_2);
                func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&blocksum, bsz, cn);
                گنتی_2 += bsz;
                اگر (گنتی_2 + بلاک_سائز_2 >= بلاک_Size0 || (i+1 >= it.nplanes && j+bsz >= کل))
                {
                    نتیجہ.d += is_fp16 ? (دوہرا)blocksum.f : (دوہرا)blocksum.i;
                    blocksum.i = 0;
                    گنتی_2 = 0;
                }
                ptrs[0] += bsz*esz;
                ptrs[1] += bsz*esz;
                اگر (ptrs[2])
                    ptrs[2] += bsz;
            }
        }
    }
    ورنہ
    {
        // generic implementation
        برائے (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            func(ptrs[0], ptrs[1], ptrs[2], (uchar*)&نتیجہ, (صحیح)it.size, cn);
        }
    }

    اگر( normType == NORM_INF )
    {
        اگر (گہرائی <= CV_32S || گہرائی == CV_32U || گہرائی == CV_Bool)
            واپس نتیجہ.u;
        اگر (گہرائی == CV_32F || is_fp16)
            واپس نتیجہ.f;
        اگر (گہرائی == CV_64U || گہرائی == CV_64S)
            واپس (دوہرا)نتیجہ.UL;
    }
    اگر( normType == NORM_L2 )
        واپس std::جذر(نتیجہ.d);

    واپس نتیجہ.d;
}

cv::Hamming::نتیجہ_Type Hamming::عامل()( ثابت غیرعلامتی حرف* a, ثابت غیرعلامتی حرف* b, صحیح size ) ثابت
{
    واپس cv::hal::normHamming(a, b, size);
}

دوہرا PSNR(داخلہ_Array _src1, داخلہ_Array _src2, دوہرا R)
{
    CV_INSTRUMENT_REGION();

    //Input arrays must have depth CV_8U
    CV_Assert( _src1.type() == _src2.type() );

    دوہرا diff = std::جذر(norm(_src1, _src2, NORM_L2SQR)/(_src1.کل()*_src1.channels()));
    واپس 20*log10(R/(diff+DBL_EPSILON));
}


#اگر_تعریف_شدہ HAVE_OPENCL
ساکن منطقی ocl_normalize( داخلہ_Array _src, داخلہ_Output_Array _dst, داخلہ_Array نقاب_2, صحیح dtype,
                           دوہرا scale, دوہرا delta )
{
    UMat src = _src.getUMat();

    اگر( نقاب_2.empty() )
        src.convert_مقصد( _dst, dtype, scale, delta );
    ورنہ اگر (src.channels() <= 4)
    {
        ثابت ocl::Device & dev = ocl::Device::getDefault();

        صحیح stype = _src.type(), sdepth = CV_MAT_گہرائی(stype), cn = CV_MAT_CN(stype),
                ddepth = CV_MAT_گہرائی(dtype), wdepth = std::max(CV_32F, std::max(sdepth, ddepth)),
                صفیں_فی_WI = dev.isIntel() ? 4 : 1;

        اعشاری fscale = static_cast<اعشاری>(scale), fdelta = static_cast<اعشاری>(delta);
        منطقی haveScale = std::fabs(scale - 1) > DBL_EPSILON,
                haveZeroScale = !(std::fabs(scale) > DBL_EPSILON),
                haveDelta = std::fabs(delta) > DBL_EPSILON,
                doubleSupport = dev.doubleFPConfig() > 0;

        اگر (!haveScale && !haveDelta && stype == dtype)
        {
            _src.copy_مقصد(_dst, نقاب_2);
            واپس سچ;
        }
        اگر (haveZeroScale)
        {
            _dst.سیٹ_مقصد(Scalar(delta), نقاب_2);
            واپس سچ;
        }

        اگر ((sdepth == CV_64F || ddepth == CV_64F) && !doubleSupport)
            واپس جھوٹ;

        حرف cvt[2][50];
        String opts = format("-D srcT=%s -D dstT=%s -D convertToWT=%s -D cn=%d -D rowsPerWI=%d"
                             " -D convertToDT=%s -D workT=%s%s%s%s -D srcT1=%s -D dstT1=%s",
                             ocl::type_مقصد_Str(stype), ocl::type_مقصد_Str(dtype),
                             ocl::convertTypeStr(sdepth, wdepth, cn, cvt[0], حجم(cvt[0])), cn,
                             صفیں_فی_WI, ocl::convertTypeStr(wdepth, ddepth, cn, cvt[1], حجم(cvt[1])),
                             ocl::type_مقصد_Str(CV_MAKE_TYPE(wdepth, cn)),
                             doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                             haveScale ? " -D HAVE_SCALE" : "",
                             haveDelta ? " -D HAVE_DELTA" : "",
                             ocl::type_مقصد_Str(sdepth), ocl::type_مقصد_Str(ddepth));

        ocl::Kernel k("normalizek", ocl::core::normalize_oclsrc, opts);
        اگر (k.empty())
            واپس جھوٹ;

        UMat نقاب = نقاب_2.getUMat(), dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::Read_Only_No_سائز(src),
                maskarg = ocl::KernelArg::Read_Only_No_سائز(نقاب),
                dstarg = ocl::KernelArg::ReadWrite(dst);

        اگر (haveScale)
        {
            اگر (haveDelta)
                k.دلائل(srcarg, maskarg, dstarg, fscale, fdelta);
            ورنہ
                k.دلائل(srcarg, maskarg, dstarg, fscale);
        }
        ورنہ
        {
            اگر (haveDelta)
                k.دلائل(srcarg, maskarg, dstarg, fdelta);
            ورنہ
                k.دلائل(srcarg, maskarg, dstarg);
        }

        size_t globalsize[2] = { (size_t)src.ستون_تعداد, ((size_t)src.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
        واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
    }
    ورنہ
    {
        UMat عارضی_2;
        src.convert_مقصد( عارضی_2, dtype, scale, delta );
        عارضی_2.copy_مقصد( _dst, نقاب_2 );
    }

    واپس سچ;
}  // ocl_normalize
#اگر_ختم  // HAVE_OPENCL

خالی normalize(داخلہ_Array _src, داخلہ_Output_Array _dst, دوہرا a, دوہرا b,
               صحیح norm_type, صحیح rtype, داخلہ_Array نقاب_2)
{
    CV_INSTRUMENT_REGION();

    دوہرا scale = 1, سرکاؤ = 0;
    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type);

    اگر( rtype < 0 )
        rtype = _dst.fixedType() ? _dst.گہرائی() : گہرائی;

    اگر( norm_type == NORM_MINMAX )
    {
        دوہرا smin = 0, smax = 0;
        دوہرا dmin = MIN( a, b ), dmax = MAX( a, b );
        minMaxIdx( _src, &smin, &smax, 0, 0, نقاب_2 );
        scale = (dmax - dmin)*(smax - smin > DBL_EPSILON ? 1./(smax - smin) : 0);
        اگر( rtype == CV_32F )
        {
            scale = (اعشاری)scale;
            سرکاؤ = (اعشاری)dmin - (اعشاری)(smin*scale);
        }
        ورنہ
            سرکاؤ = dmin - smin*scale;
    }
    ورنہ اگر( norm_type == NORM_L2 || norm_type == NORM_L1 || norm_type == NORM_INF )
    {
        scale = norm( _src, norm_type, نقاب_2 );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
        سرکاؤ = 0;
    }
    ورنہ
        CV_Error( cv::Error::StsBadArg, "Unknown/unsupported norm type" );

    CV_OCL_تسلسل(_dst.isUMat(),
               ocl_normalize(_src, _dst, نقاب_2, rtype, scale, سرکاؤ))

    Mat src = _src.getMat();
    اگر( نقاب_2.empty() )
        src.convert_مقصد( _dst, rtype, scale, سرکاؤ );
    ورنہ
    {
        Mat عارضی_2;
        src.convert_مقصد( عارضی_2, rtype, scale, سرکاؤ );
        عارضی_2.copy_مقصد( _dst, نقاب_2 );
    }
}

}  // namespace
