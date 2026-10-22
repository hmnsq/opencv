// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"

نامخلا cv {

قسمتعریف خالی (*PatchNanFunc)(uchar* tptr, size_t len, دوہرا newVal);
قسمتعریف خالی (*Finite_نقاب_Func)(ثابت uchar *src, uchar *dst, size_t کل);

CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

PatchNanFunc getPatchNanFunc(منطقی isDouble);
Finite_نقاب_Func get_Finite_نقاب_Func(منطقی isDouble, صحیح cn);

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ساکن خالی patchNaNs_32f(uchar* ptr, size_t ulen, دوہرا newVal)
{
    CV_INSTRUMENT_REGION();

    int32_t* tptr = (int32_t*)ptr;
    Cv32suf val;
    val.f = (اعشاری)newVal;

    صحیح j = 0;
    صحیح len = (صحیح)ulen;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_int32 v_pos_نقاب = vx_setall_s32(0x7fffffff), v_exp_نقاب = vx_setall_s32(0x7f800000);
    v_int32 v_val = vx_setall_s32(val.i);

    صحیح c_چوڑائی = VTraits<v_int32>::vlanes();
    برائے (; j < len - c_چوڑائی*2 + 1; j += c_چوڑائی*2)
    {
        v_int32 v_src0 = vx_بھرو(tptr + j);
        v_int32 v_src1 = vx_بھرو(tptr + j + c_چوڑائی);

        v_int32 v_cmp_mask0 = v_lt(v_exp_نقاب, v_and(v_src0, v_pos_نقاب));
        v_int32 v_cmp_mask1 = v_lt(v_exp_نقاب, v_and(v_src1, v_pos_نقاب));

        اگر (v_check_any(v_or(v_cmp_mask0, v_cmp_mask1)))
        {
            v_int32 v_dst0 = v_select(v_cmp_mask0, v_val, v_src0);
            v_int32 v_dst1 = v_select(v_cmp_mask1, v_val, v_src1);

            v_store(tptr + j, v_dst0);
            v_store(tptr + j + c_چوڑائی, v_dst1);
        }
    }
#اگر_ختم

    برائے (; j < len; j++)
    {
        اگر ((tptr[j] & 0x7fffffff) > 0x7f800000)
        {
            tptr[j] = val.i;
        }
    }
}


ساکن خالی patchNaNs_64f(uchar* ptr, size_t ulen, دوہرا newVal)
{
    CV_INSTRUMENT_REGION();

    int64_t* tptr = (int64_t*)ptr;
    Cv64suf val;
    val.f = newVal;

    صحیح j = 0;
    صحیح len = (صحیح)ulen;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    v_int64 v_exp_نقاب = vx_setall_s64(0x7FF0000000000000);
    v_int64 v_pos_نقاب = vx_setall_s64(0x7FFFFFFFFFFFFFFF);
    v_int64 v_val = vx_setall_s64(val.i);

    صحیح c_چوڑائی = VTraits<v_int64>::vlanes();
    برائے (; j < len - c_چوڑائی * 2 + 1; j += c_چوڑائی*2)
    {
        v_int64 v_src0 = vx_بھرو(tptr + j);
        v_int64 v_src1 = vx_بھرو(tptr + j + c_چوڑائی);

        v_int64 v_cmp_mask0 = v_lt(v_exp_نقاب, v_and(v_src0, v_pos_نقاب));
        v_int64 v_cmp_mask1 = v_lt(v_exp_نقاب, v_and(v_src1, v_pos_نقاب));

        اگر (v_check_any(v_cmp_mask0) || v_check_any(v_cmp_mask1))
        {
            // v_select is not available for v_int64, emulating it
            v_int32 val32 = v_reinterpret_as_s32(v_val);
            v_int64 v_dst0 = v_reinterpret_as_s64(v_select(v_reinterpret_as_s32(v_cmp_mask0), val32, v_reinterpret_as_s32(v_src0)));
            v_int64 v_dst1 = v_reinterpret_as_s64(v_select(v_reinterpret_as_s32(v_cmp_mask1), val32, v_reinterpret_as_s32(v_src1)));

            v_store(tptr + j, v_dst0);
            v_store(tptr + j + c_چوڑائی, v_dst1);
        }
    }
#اگر_ختم

    برائے (; j < len; j++)
        اگر ((tptr[j] & 0x7FFFFFFFFFFFFFFF) > 0x7FF0000000000000)
            tptr[j] = val.i;
}

PatchNanFunc getPatchNanFunc(منطقی isDouble)
{
    واپس isDouble ? patchNaNs_64f
                    : patchNaNs_32f;
}

////// finiteMask //////

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

سانچہ <قسم_نام _Tp, صحیح cn>
صحیح finite_نقاب_SIMD(ثابت _Tp *src, uchar *dst, size_t کل);

سانچہ <>
صحیح finite_نقاب_SIMD<اعشاری, 1>(ثابت اعشاری *fsrc, uchar *dst, size_t utotal)
{
    ثابت uint32_t* src = (ثابت uint32_t*)fsrc;
    ثابت صحیح osize = VTraits<v_uint8>::vlanes();
    v_uint32 vmaskExp = vx_setall_u32(0x7f800000);

    صحیح i = 0;
    صحیح کل = (صحیح)utotal;
    برائے(; i < کل - osize + 1; i += osize )
    {
        v_uint32 vv0, vv1, vv2, vv3;
        vv0 = v_ne(v_and(vx_بھرو(src + i              ), vmaskExp), vmaskExp);
        vv1 = v_ne(v_and(vx_بھرو(src + i +   (osize/4)), vmaskExp), vmaskExp);
        vv2 = v_ne(v_and(vx_بھرو(src + i + 2*(osize/4)), vmaskExp), vmaskExp);
        vv3 = v_ne(v_and(vx_بھرو(src + i + 3*(osize/4)), vmaskExp), vmaskExp);

        v_store(dst + i, v_pack_b(vv0, vv1, vv2, vv3));
    }

    واپس i;
}


سانچہ <>
صحیح finite_نقاب_SIMD<اعشاری, 2>(ثابت اعشاری *fsrc, uchar *dst, size_t utotal)
{
    ثابت uint32_t* src = (ثابت uint32_t*)fsrc;
    ثابت صحیح size8 = VTraits<v_uint8>::vlanes();
    v_uint32 vmaskExp = vx_setall_u32(0x7f800000);
    v_uint16 vmaskBoth = vx_setall_u16(0xffff);

    صحیح i = 0;
    صحیح کل = (صحیح)utotal;
    برائے(; i < کل - (size8 / 2) + 1; i += (size8 / 2) )
    {
        v_uint32 vv0, vv1, vv2, vv3;
        vv0 = v_ne(v_and(vx_بھرو(src + i*2                ), vmaskExp), vmaskExp);
        vv1 = v_ne(v_and(vx_بھرو(src + i*2 +   (size8 / 4)), vmaskExp), vmaskExp);
        vv2 = v_ne(v_and(vx_بھرو(src + i*2 + 2*(size8 / 4)), vmaskExp), vmaskExp);
        vv3 = v_ne(v_and(vx_بھرو(src + i*2 + 3*(size8 / 4)), vmaskExp), vmaskExp);
        v_uint8 velems = v_pack_b(vv0, vv1, vv2, vv3);
        v_uint16 vfinite = v_eq(v_reinterpret_as_u16(velems), vmaskBoth);

        // 2nd argument in vfinite is useless
        v_store_کم(dst + i, v_pack(vfinite, vfinite));
    }

    واپس i;
}


سانچہ <>
صحیح finite_نقاب_SIMD<اعشاری, 3>(ثابت اعشاری *fsrc, uchar *dst, size_t utotal)
{
    ثابت uint32_t* src = (ثابت uint32_t*)fsrc;
    ثابت صحیح npixels = VTraits<v_float32>::vlanes();
    v_uint32 vmaskExp = vx_setall_u32(0x7f800000);
    v_uint32 z = vx_setzero_u32();

    صحیح i = 0;
    صحیح کل = (صحیح)utotal;
    برائے (; i < کل - npixels + 1; i += npixels)
    {
        v_uint32 vv0, vv1, vv2;
        vv0 = v_ne(v_and(vx_بھرو(src + i*3            ), vmaskExp), vmaskExp);
        vv1 = v_ne(v_and(vx_بھرو(src + i*3 +   npixels), vmaskExp), vmaskExp);
        vv2 = v_ne(v_and(vx_بھرو(src + i*3 + 2*npixels), vmaskExp), vmaskExp);

        v_uint8 velems = v_pack_b(vv0, vv1, vv2, z);

        // 2nd arg is useless
        v_uint8 vsh1 = v_نکالو<1>(velems, velems);
        v_uint8 vsh2 = v_نکالو<2>(velems, velems);

        v_uint8 vres3 = v_and(v_and(velems, vsh1), vsh2);
        برائے (صحیح j = 0; j < npixels; j++)
        {
            dst[i + j] = v_get0(vres3);
            // 2nd arg is useless
            vres3 = v_نکالو<3>(vres3, vres3);
        }
    }

    واپس i;
}


سانچہ <>
صحیح finite_نقاب_SIMD<اعشاری, 4>(ثابت اعشاری *fsrc, uchar *dst, size_t utotal)
{
    ثابت uint32_t* src = (ثابت uint32_t*)fsrc;
    ثابت صحیح npixels = VTraits<v_uint8>::vlanes() / 2;
    ثابت صحیح nfloats = VTraits<v_uint32>::vlanes();
    ثابت v_uint32 v_نقاب_Exp = vx_setall_u32(0x7f800000);
    v_uint32 vmaskAll4 = vx_setall_u32(0xFFFFFFFF);

    صحیح i = 0;
    صحیح کل = (صحیح)utotal;
    برائے(; i < کل - npixels + 1; i += npixels )
    {
        v_uint32 v0 = vx_بھرو(src + i * 4 + 0*nfloats);
        v_uint32 v1 = vx_بھرو(src + i * 4 + 1*nfloats);
        v_uint32 v2 = vx_بھرو(src + i * 4 + 2*nfloats);
        v_uint32 v3 = vx_بھرو(src + i * 4 + 3*nfloats);
        v_uint32 v4 = vx_بھرو(src + i * 4 + 4*nfloats);
        v_uint32 v5 = vx_بھرو(src + i * 4 + 5*nfloats);
        v_uint32 v6 = vx_بھرو(src + i * 4 + 6*nfloats);
        v_uint32 v7 = vx_بھرو(src + i * 4 + 7*nfloats);

        v_uint32 vmask0 = v_ne(v_and(v0, v_نقاب_Exp), v_نقاب_Exp);
        v_uint32 vmask1 = v_ne(v_and(v1, v_نقاب_Exp), v_نقاب_Exp);
        v_uint32 vmask2 = v_ne(v_and(v2, v_نقاب_Exp), v_نقاب_Exp);
        v_uint32 vmask3 = v_ne(v_and(v3, v_نقاب_Exp), v_نقاب_Exp);
        v_uint32 vmask4 = v_ne(v_and(v4, v_نقاب_Exp), v_نقاب_Exp);
        v_uint32 vmask5 = v_ne(v_and(v5, v_نقاب_Exp), v_نقاب_Exp);
        v_uint32 vmask6 = v_ne(v_and(v6, v_نقاب_Exp), v_نقاب_Exp);
        v_uint32 vmask7 = v_ne(v_and(v7, v_نقاب_Exp), v_نقاب_Exp);

        v_uint8 velems0 = v_pack_b(vmask0, vmask1, vmask2, vmask3);
        v_uint8 velems1 = v_pack_b(vmask4, vmask5, vmask6, vmask7);

        v_uint32 vresWide0 = v_eq(v_reinterpret_as_u32(velems0), vmaskAll4);
        v_uint32 vresWide1 = v_eq(v_reinterpret_as_u32(velems1), vmaskAll4);

        // last 2 args are useless
        v_uint8 vres = v_pack_b(vresWide0, vresWide1, vresWide0, vresWide1);

        v_store_کم(dst + i, vres);
    }

    واپس i;
}


سانچہ <>
صحیح finite_نقاب_SIMD<دوہرا, 1>(ثابت دوہرا *dsrc, uchar *dst, size_t utotal)
{
    ثابت uint64_t* src = (ثابت uint64_t*)dsrc;
    ثابت صحیح size8 = VTraits<v_uint8>::vlanes();
    صحیح i = 0;
    صحیح کل = (صحیح)utotal;

    v_uint64 vmaskExp = vx_setall_u64(0x7ff0000000000000);
    v_uint64 z = vx_setzero_u64();

    برائے(; i < کل - (size8 / 2) + 1; i += (size8 / 2) )
    {
        v_uint64 vv0, vv1, vv2, vv3;
        vv0 = v_ne(v_and(vx_بھرو(src + i                ), vmaskExp), vmaskExp);
        vv1 = v_ne(v_and(vx_بھرو(src + i +   (size8 / 8)), vmaskExp), vmaskExp);
        vv2 = v_ne(v_and(vx_بھرو(src + i + 2*(size8 / 8)), vmaskExp), vmaskExp);
        vv3 = v_ne(v_and(vx_بھرو(src + i + 3*(size8 / 8)), vmaskExp), vmaskExp);

        v_uint8 v = v_pack_b(vv0, vv1, vv2, vv3, z, z, z, z);

        v_store_کم(dst + i, v);
    }

    واپس i;
}

سانچہ <>
صحیح finite_نقاب_SIMD<دوہرا, 2>(ثابت دوہرا *dsrc, uchar *dst, size_t utotal)
{
    ثابت uint64_t* src = (ثابت uint64_t*)dsrc;
    ثابت صحیح npixels = VTraits<v_uint8>::vlanes() / 2;
    ثابت صحیح ndoubles = VTraits<v_uint64>::vlanes();
    v_uint64 vmaskExp = vx_setall_u64(0x7ff0000000000000);
    v_uint16 vmaskBoth = vx_setall_u16(0xffff);

    صحیح i = 0;
    صحیح کل = (صحیح)utotal;
    برائے(; i < کل - npixels + 1; i += npixels )
    {
        v_uint64 vv0 = v_ne(v_and(vx_بھرو(src + i*2 + 0*ndoubles), vmaskExp), vmaskExp);
        v_uint64 vv1 = v_ne(v_and(vx_بھرو(src + i*2 + 1*ndoubles), vmaskExp), vmaskExp);
        v_uint64 vv2 = v_ne(v_and(vx_بھرو(src + i*2 + 2*ndoubles), vmaskExp), vmaskExp);
        v_uint64 vv3 = v_ne(v_and(vx_بھرو(src + i*2 + 3*ndoubles), vmaskExp), vmaskExp);
        v_uint64 vv4 = v_ne(v_and(vx_بھرو(src + i*2 + 4*ndoubles), vmaskExp), vmaskExp);
        v_uint64 vv5 = v_ne(v_and(vx_بھرو(src + i*2 + 5*ndoubles), vmaskExp), vmaskExp);
        v_uint64 vv6 = v_ne(v_and(vx_بھرو(src + i*2 + 6*ndoubles), vmaskExp), vmaskExp);
        v_uint64 vv7 = v_ne(v_and(vx_بھرو(src + i*2 + 7*ndoubles), vmaskExp), vmaskExp);

        v_uint8 velems0 = v_pack_b(vv0, vv1, vv2, vv3, vv4, vv5, vv6, vv7);

        v_uint16 vfinite0 = v_eq(v_reinterpret_as_u16(velems0), vmaskBoth);

        // 2nd arg is useless
        v_uint8 vres = v_pack(vfinite0, vfinite0);
        v_store_کم(dst + i, vres);
    }

    واپس i;
}


سانچہ <>
صحیح finite_نقاب_SIMD<دوہرا, 3>(ثابت دوہرا *dsrc, uchar *dst, size_t utotal)
{
    //TODO: vectorize it properly

    ثابت uint64_t* src = (ثابت uint64_t*)dsrc;
    ثابت صحیح npixels = VTraits<v_uint8>::vlanes() / 2;
    uint64_t نقاب_Exp = 0x7ff0000000000000;

    صحیح i = 0;
    صحیح کل = (صحیح)utotal;
    برائے(; i < کل - npixels + 1; i += npixels )
    {
        برائے (صحیح j = 0; j < npixels; j++)
        {
            uint64_t val0 = src[i * 3 + j * 3 + 0];
            uint64_t val1 = src[i * 3 + j * 3 + 1];
            uint64_t val2 = src[i * 3 + j * 3 + 2];

            منطقی finite = ((val0 & نقاب_Exp) != نقاب_Exp) &&
                          ((val1 & نقاب_Exp) != نقاب_Exp) &&
                          ((val2 & نقاب_Exp) != نقاب_Exp);

            dst[i + j] = finite ? 255 : 0;
        }
    }

    واپس i;
}

سانچہ <>
صحیح finite_نقاب_SIMD<دوہرا, 4>(ثابت دوہرا *dsrc, uchar *dst, size_t utotal)
{
    //TODO: vectorize it properly

    uint64_t* src = (uint64_t*)dsrc;
    ثابت صحیح npixels = VTraits<v_uint8>::vlanes() / 2;
    ثابت صحیح ndoubles = VTraits<v_uint64>::vlanes();
    v_uint16 vmaskExp16 = vx_setall_u16(0x7ff0);
    v_uint32 z = vx_setzero_u32();

    صحیح i = 0;
    صحیح کل = (صحیح)utotal;
    برائے(; i < کل - npixels + 1; i += npixels )
    {
        v_uint16 vexpb0, vexpb1, vexpb2, vexpb3;
        v_uint16 dummy;
        v_بھرو_deinterleave((uint16_t*)(src + 0*4*ndoubles), dummy, dummy, dummy, vexpb0);
        v_بھرو_deinterleave((uint16_t*)(src + 1*4*ndoubles), dummy, dummy, dummy, vexpb1);
        v_بھرو_deinterleave((uint16_t*)(src + 2*4*ndoubles), dummy, dummy, dummy, vexpb2);
        v_بھرو_deinterleave((uint16_t*)(src + 3*4*ndoubles), dummy, dummy, dummy, vexpb3);

        v_uint16 vcmp0 = v_eq(v_and(vexpb0, vmaskExp16), vmaskExp16);
        v_uint16 vcmp1 = v_eq(v_and(vexpb1, vmaskExp16), vmaskExp16);
        v_uint16 vcmp2 = v_eq(v_and(vexpb2, vmaskExp16), vmaskExp16);
        v_uint16 vcmp3 = v_eq(v_and(vexpb3, vmaskExp16), vmaskExp16);

        v_uint8 velems0 = v_pack(vcmp0, vcmp1);
        v_uint8 velems1 = v_pack(vcmp2, vcmp3);

        v_uint32 vResWide0 = v_eq(v_reinterpret_as_u32(velems0), z);
        v_uint32 vResWide1 = v_eq(v_reinterpret_as_u32(velems1), z);

        v_uint16 vp16 = v_pack(vResWide0, vResWide1);

        // 2nd arg is useless
        v_uint8 vres = v_pack(vp16, vp16);
        v_store_کم(dst, vres);

        src += npixels * 4;
        dst += npixels;
    }

    واپس i;
}

#اگر_ختم


سانچہ <قسم_نام _Tp, صحیح cn>
خالی finite_نقاب_2(ثابت uchar *src, uchar *dst, size_t کل)
{
    CV_INSTRUMENT_REGION();
    size_t i = 0;
    ثابت _Tp* tsrc = (ثابت _Tp*) src;

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    i = finite_نقاب_SIMD<_Tp, cn>(tsrc, dst, کل);
#اگر_ختم

    برائے(; i < کل; i++ )
    {
        منطقی finite = سچ;
        برائے (صحیح c = 0; c < cn; c++)
        {
            _Tp val = tsrc[i * cn + c];
            finite = finite && !cvIsNaN(val) && !cvIsInf(val);
        }
        dst[i] = finite ? 255 : 0;
    }
}

Finite_نقاب_Func get_Finite_نقاب_Func(منطقی isDouble, صحیح cn)
{
    ساکن Finite_نقاب_Func tab[CV_گہرائی_MAX] =
    {
        finite_نقاب_2<اعشاری,  1>,
        finite_نقاب_2<اعشاری,  2>,
        finite_نقاب_2<اعشاری,  3>,
        finite_نقاب_2<اعشاری,  4>,
        finite_نقاب_2<دوہرا, 1>,
        finite_نقاب_2<دوہرا, 2>,
        finite_نقاب_2<دوہرا, 3>,
        finite_نقاب_2<دوہرا, 4>,
    };

    صحیح idx = (isDouble ? 4 : 0) + cn - 1;
    واپس tab[idx];
}

#اگر_ختم
CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
} // namespace cv
