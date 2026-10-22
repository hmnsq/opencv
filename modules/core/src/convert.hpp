// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SRC_CONVERT_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು SRC_CONVERT_HPP

#ಅನ್ತರ್ಭಾವ "opencv2/core/types.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"

ನಾಮಾಕಾಶ cv
{

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ uchar* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_ತುಂಬು_expand_q(ptr))); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ schar* ptr, v_float32& a)
{ a = v_cvt_f32(vx_ತುಂಬು_expand_q(ptr)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ ushort* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_ತುಂಬು_expand(ptr))); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_ತುಂಬು_expand(ptr))); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ptr, v_float32& a)
{ a = v_cvt_f32(vx_ತುಂಬು(ptr)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* ptr, v_float32& a)
{
    v_uint32 delta = vx_setall_u32(0x80000000U);
    v_uint32 ua = vx_ತುಂಬು(ptr);
    v_uint32 ಮುಖವಾಡ_a = v_and(v_ge(ua, delta), delta);
    v_float32 fmask_a = v_cvt_f32(v_reinterpret_as_s32(ಮುಖವಾಡ_a)); // 0.f or (float)(-(1 << 31))
    a = v_cvt_f32(v_reinterpret_as_s32(v_sub(ua, ಮುಖವಾಡ_a)));
    // restore the original values
    a = v_sub(a, fmask_a); // subtract 0 or a large negative number
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ptr, v_float32& a)
{ a = vx_ತುಂಬು(ptr); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ hfloat* ptr, v_float32& a)
{ a = vx_ತುಂಬು_expand(ptr); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ bfloat* ptr, v_float32& a)
{ a = vx_ತುಂಬು_expand(ptr); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(ushort* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{ v_pack_u_store(ptr, v_ಸುತ್ತು(a)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(ಗಿಡ್ಡ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{ v_pack_store(ptr, v_ಸುತ್ತು(a)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(ಪೂರ್ಣಾಂಕ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{ v_store(ptr, v_ಸುತ್ತು(a)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(ಚಿಹ್ನೆರಹಿತ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{
    v_float32 z = vx_setzero_f32();
    v_store(ptr, v_reinterpret_as_u32(v_ಸುತ್ತು(v_max(a, z))));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(ದಶಮಾಂಶ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{ v_store(ptr, a); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(hfloat* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{ v_pack_store(ptr, a); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(bfloat* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{ v_pack_store(ptr, a); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(int64_t* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{
    v_int32 ia = v_ಸುತ್ತು(a);
    v_int64 ia_0, ia_1;
    v_expand(ia, ia_0, ia_1);
    v_store(ptr, ia_0);
    v_store(ptr + VTraits<v_uint64>::vlanes(), ia_1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(uint64_t* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{
    v_int32 ia = v_ಸುತ್ತು(a);
    v_uint64 ia_0, ia_1;
    ia = v_max(ia, vx_setzero_s32());
    v_expand(v_reinterpret_as_u32(ia), ia_0, ia_1);
    v_store(ptr, ia_0);
    v_store(ptr + VTraits<v_uint64>::vlanes(), ia_1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uchar* ptr, v_uint16& a, v_uint16& b)
{ v_expand(vx_ತುಂಬು(ptr), a, b); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ schar* ptr, v_uint16& a, v_uint16& b)
{
    ಸ್ಥಿರಾಂಕ v_int8 z = vx_setzero_s8();
    v_int16 sa, sb;
    v_expand(v_max(vx_ತುಂಬು(ptr), z), sa, sb);
    a = v_reinterpret_as_u16(sa);
    b = v_reinterpret_as_u16(sb);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ushort* ptr, v_uint16& a, v_uint16& b)
{ a = vx_ತುಂಬು(ptr); b = vx_ತುಂಬು(ptr + VTraits<v_uint16>::vlanes()); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uchar* ptr, v_int16& a, v_int16& b)
{
    v_uint16 ua, ub;
    v_expand(vx_ತುಂಬು(ptr), ua, ub);
    a = v_reinterpret_as_s16(ua);
    b = v_reinterpret_as_s16(ub);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ schar* ptr, v_int16& a, v_int16& b)
{ v_expand(vx_ತುಂಬು(ptr), a, b); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* ptr, v_int16& a, v_int16& b)
{ a = vx_ತುಂಬು(ptr); b = vx_ತುಂಬು(ptr + VTraits<v_uint16>::vlanes()); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uchar* ptr, v_int32& a, v_int32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_ತುಂಬು_expand(ptr), ua, ub);
    a = v_reinterpret_as_s32(ua);
    b = v_reinterpret_as_s32(ub);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ schar* ptr, v_int32& a, v_int32& b)
{ v_expand(vx_ತುಂಬು_expand(ptr), a, b); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ushort* ptr, v_int32& a, v_int32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_ತುಂಬು(ptr), ua, ub);
    a = v_reinterpret_as_s32(ua);
    b = v_reinterpret_as_s32(ub);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ushort* ptr, v_uint32& a, v_uint32& b)
{
    v_expand(vx_ತುಂಬು(ptr), a, b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* ptr, v_int32& a, v_int32& b)
{
    v_expand(vx_ತುಂಬು(ptr), a, b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ptr, v_int32& a, v_int32& b)
{
    a = vx_ತುಂಬು(ptr);
    b = vx_ತುಂಬು(ptr + VTraits<v_int32>::vlanes());
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uchar* ptr, v_float32& a, v_float32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_ತುಂಬು_expand(ptr), ua, ub);
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
    b = v_cvt_f32(v_reinterpret_as_s32(ub));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ schar* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia, ib;
    v_expand(vx_ತುಂಬು_expand(ptr), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ushort* ptr, v_float32& a, v_float32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_ತುಂಬು(ptr), ua, ub);
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
    b = v_cvt_f32(v_reinterpret_as_s32(ub));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia, ib;
    v_expand(vx_ತುಂಬು(ptr), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia = vx_ತುಂಬು(ptr), ib = vx_ತುಂಬು(ptr + VTraits<v_int32>::vlanes());
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

// v_pack()/v_pack_u() saturate at every width, 64->32 included, so a 64-bit source needs no
// clamping here any more - see the HAL headers for the per-backend implementations.

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ int64_t* ptr, v_int32& a, v_int32& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack(vx_ತುಂಬು(ptr), vx_ತುಂಬು(ptr + int64_nlanes));
    b = v_pack(vx_ತುಂಬು(ptr + int64_nlanes*2), vx_ತುಂಬು(ptr + int64_nlanes*3));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ int64_t* ptr, v_uint64& a, v_uint64& b)
{
    // clamp the negatives to zero through the sign bit: v_gt on 64-bit lanes is sign(b - a) at the
    // SSE and NEON baselines, which overflows for an operand at the bottom of the range
    v_int64 ia = vx_ತುಂಬು(ptr), ib = vx_ತುಂಬು(ptr + VTraits<v_uint64>::vlanes());
    ia = v_and(ia, v_not(v_shr<63>(ia)));
    ib = v_and(ib, v_not(v_shr<63>(ib)));
    a = v_reinterpret_as_u64(ia);
    b = v_reinterpret_as_u64(ib);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ int64_t* ptr, v_uint32& a, v_uint32& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack_u(vx_ತುಂಬು(ptr), vx_ತುಂಬು(ptr + nlanes));
    b = v_pack_u(vx_ತುಂಬು(ptr + nlanes*2), vx_ತುಂಬು(ptr + nlanes*3));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uint64_t* ptr, v_float32& a, v_float32& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes = VTraits<v_uint64>::vlanes();
    ದಶಮಾಂಶ buf[VTraits<v_uint64>::max_nlanes*4];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nlanes*4; i++) {
        buf[i] = (ದಶಮಾಂಶ)ptr[i];
    }
    a = vx_ತುಂಬು(buf);
    b = vx_ತುಂಬು(buf + nlanes*2);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ int64_t* ptr, v_float32& a, v_float32& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes = VTraits<v_uint64>::vlanes();
    ದಶಮಾಂಶ buf[VTraits<v_uint64>::max_nlanes*4];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nlanes*4; i++) {
        buf[i] = (ದಶಮಾಂಶ)ptr[i];
    }
    a = vx_ತುಂಬು(buf);
    b = vx_ತುಂಬು(buf + nlanes*2);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ* ptr, v_float32& a, v_float32& b)
{
    v_uint16 z = vx_setzero_u16();
    v_uint16 uab = vx_ತುಂಬು_expand((ಸ್ಥಿರಾಂಕ uchar*)ptr);
    uab = v_shr<15>(v_gt(uab, z));
    v_int32 ia, ib;
    v_expand(v_reinterpret_as_s16(uab), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ* ptr, v_float32& a)
{
    v_uint32 z = vx_setzero_u32();
    v_uint32 ua = vx_ತುಂಬು_expand_q((ಸ್ಥಿರಾಂಕ uchar*)ptr);
    ua = v_shr<31>(v_gt(ua, z));
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ schar* ptr, v_uint32& a, v_uint32& b)
{
    v_int16 ab = v_max(vx_ತುಂಬು_expand(ptr), vx_setzero_s16());
    v_expand(v_reinterpret_as_u16(ab), a, b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* ptr, v_uint32& a, v_uint32& b)
{
    v_int16 ab = v_max(vx_ತುಂಬು(ptr), vx_setzero_s16());
    v_expand(v_reinterpret_as_u16(ab), a, b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ptr, v_uint32& a, v_uint32& b)
{
    v_int32 z = vx_setzero_s32();
    v_int32 ia = v_max(vx_ತುಂಬು(ptr), z);
    v_int32 ib = v_max(vx_ತುಂಬು(ptr + VTraits<v_int32>::vlanes()), z);
    a = v_reinterpret_as_u32(ia);
    b = v_reinterpret_as_u32(ib);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uint64_t* ptr, v_uint32& a, v_uint32& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack(vx_ತುಂಬು(ptr), vx_ತುಂಬು(ptr + int64_nlanes));
    b = v_pack(vx_ತುಂಬು(ptr + int64_nlanes*2), vx_ತುಂಬು(ptr + int64_nlanes*3));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uint64_t* ptr, v_int32& a, v_int32& b)
{
    // There is no uint64 -> int32 narrow in the HAL, so narrow as unsigned first - v_pack() clamps
    // anything above UINT32_MAX to UINT32_MAX - and bring that down to INT32_MAX with the same
    // 32-bit v_min() the uint32 -> int32 direction below uses.
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ int64_nlanes = VTraits<v_uint64>::vlanes();
    ಸ್ಥಿರಾಂಕ v_uint32 hi = vx_setall_u32((ಚಿಹ್ನೆರಹಿತ)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
    a = v_reinterpret_as_s32(v_min(v_pack(vx_ತುಂಬು(ptr), vx_ತುಂಬು(ptr + int64_nlanes)), hi));
    b = v_reinterpret_as_s32(v_min(v_pack(vx_ತುಂಬು(ptr + int64_nlanes*2),
                                          vx_ತುಂಬು(ptr + int64_nlanes*3)), hi));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ptr, v_float32& a, v_float32& b)
{ a = vx_ತುಂಬು(ptr); b = vx_ತುಂಬು(ptr + VTraits<v_float32>::vlanes()); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ hfloat* ptr, v_float32& a, v_float32& b)
{
    a = vx_ತುಂಬು_expand(ptr);
    b = vx_ತುಂಬು_expand(ptr + VTraits<v_float32>::vlanes());
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ bfloat* ptr, v_float32& a, v_float32& b)
{
    a = vx_ತುಂಬು_expand(ptr);
    b = vx_ತುಂಬು_expand(ptr + VTraits<v_float32>::vlanes());
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* ptr, v_uint32& a, v_uint32& b)
{
    a = vx_ತುಂಬು(ptr);
    b = vx_ತುಂಬು(ptr + VTraits<v_uint32>::vlanes());
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* ptr, v_int32& a, v_int32& b)
{
    // Reinterpreting alone would turn everything above INT32_MAX negative, and the saturating
    // narrow that follows would then clamp it to the WRONG end of the destination's range
    // (4294967295 -> -1 -> -128 for schar, rather than 127). Clamp first, like the int -> unsigned
    // direction above does for negatives.
    ಸ್ಥಿರಾಂಕ v_uint32 hi = vx_setall_u32((ಚಿಹ್ನೆರಹಿತ)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
    a = v_reinterpret_as_s32(v_min(vx_ತುಂಬು(ptr), hi));
    b = v_reinterpret_as_s32(v_min(vx_ತುಂಬು(ptr + VTraits<v_uint32>::vlanes()), hi));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* ptr, v_float32& a, v_float32& b)
{
    v_uint32 delta = vx_setall_u32(0x80000000U);
    v_uint32 ua = vx_ತುಂಬು(ptr);
    v_uint32 ub = vx_ತುಂಬು(ptr + VTraits<v_uint32>::vlanes());
    v_uint32 ಮುಖವಾಡ_a = v_and(v_ge(ua, delta), delta), ಮುಖವಾಡ_b = v_and(v_ge(ub, delta), delta);
    v_float32 fmask_a = v_cvt_f32(v_reinterpret_as_s32(ಮುಖವಾಡ_a)); // 0.f or (float)(-(1 << 31))
    v_float32 fmask_b = v_cvt_f32(v_reinterpret_as_s32(ಮುಖವಾಡ_b)); // 0.f or (float)(-(1 << 31))
    a = v_cvt_f32(v_reinterpret_as_s32(v_sub(ua, ಮುಖವಾಡ_a)));
    b = v_cvt_f32(v_reinterpret_as_s32(v_sub(ub, ಮುಖವಾಡ_b)));
    // restore the original values
    a = v_sub(a, fmask_a); // subtract 0 or a large negative number
    b = v_sub(b, fmask_b); // subtract 0 or a large negative number
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uchar* ptr, ಸ್ಥಿರಾಂಕ v_uint16& a, ಸ್ಥಿರಾಂಕ v_uint16& b)
{
    v_store(ptr, v_pack(a, b));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(schar* ptr, ಸ್ಥಿರಾಂಕ v_uint16& a, ಸ್ಥಿರಾಂಕ v_uint16& b)
{
    ಸ್ಥಿರಾಂಕ v_uint8 maxval = vx_setall_u8((uchar)std::numeric_limits<schar>::max());
    v_uint8 v = v_pack(a, b);
    v_store(ptr, v_reinterpret_as_s8(v_min(v, maxval)));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ushort* ptr, ಸ್ಥಿರಾಂಕ v_uint16& a, ಸ್ಥಿರಾಂಕ v_uint16& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_uint16>::vlanes(), b); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uchar* ptr, ಸ್ಥಿರಾಂಕ v_int16& a, ಸ್ಥಿರಾಂಕ v_int16& b)
{ v_store(ptr, v_pack_u(a, b)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(schar* ptr, ಸ್ಥಿರಾಂಕ v_int16& a, ಸ್ಥಿರಾಂಕ v_int16& b)
{ v_store(ptr, v_pack(a, b)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಗಿಡ್ಡ* ptr, ಸ್ಥಿರಾಂಕ v_int16& a, ಸ್ಥಿರಾಂಕ v_int16& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_int16>::vlanes(), b); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uchar* ptr, ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{ v_pack_u_store(ptr, v_pack(a, b)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(schar* ptr, ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{ v_pack_store(ptr, v_pack(a, b)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ushort* ptr, ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{ v_store(ptr, v_pack_u(a, b)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಗಿಡ್ಡ* ptr, ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{ v_store(ptr, v_pack(a, b)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಪೂರ್ಣಾಂಕ* ptr, ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_int32>::vlanes(), b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(int64_t* ptr, ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{
    v_int64 q0, q1, q2, q3;
    v_expand(a, q0, q1);
    v_expand(b, q2, q3);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes = VTraits<v_uint64>::vlanes();
    v_store(ptr, q0);
    v_store(ptr + nlanes, q1);
    v_store(ptr + nlanes*2, q2);
    v_store(ptr + nlanes*3, q3);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uchar* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{ v_pack_u_store(ptr, v_pack(v_ಸುತ್ತು(a), v_ಸುತ್ತು(b))); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(schar* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{ v_pack_store(ptr, v_pack(v_ಸುತ್ತು(a), v_ಸುತ್ತು(b))); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ತಾರ್ಕಿಕ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{
    v_float32 z = vx_setzero_f32();
    v_uint32 ma = v_shr<31>(v_reinterpret_as_u32(v_ne(a, z)));
    v_uint32 mb = v_shr<31>(v_reinterpret_as_u32(v_ne(b, z)));
    v_uint16 mab = v_pack(ma, mb);
    v_pack_store((uchar*)ptr, mab);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ushort* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{ v_store(ptr, v_pack_u(v_ಸುತ್ತು(a), v_ಸುತ್ತು(b))); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಗಿಡ್ಡ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{ v_store(ptr, v_pack(v_ಸುತ್ತು(a), v_ಸುತ್ತು(b))); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಪೂರ್ಣಾಂಕ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{
    v_int32 ia = v_ಸುತ್ತು(a), ib = v_ಸುತ್ತು(b);
    v_store(ptr, ia);
    v_store(ptr + VTraits<v_int32>::vlanes(), ib);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ದಶಮಾಂಶ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_float32>::vlanes(), b); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಚಿಹ್ನೆರಹಿತ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{
    // v_round(f32) narrows to v_int32, so values in [2^31, 2^32) saturate to INT32_MAX. Scalar for now
    // (same class as the f64->{u64,s64,u32} stores above); a proper f32->u32 intrinsic can replace it.
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = VTraits<v_float32>::vlanes();
    ದಶಮಾಂಶ buf[VTraits<v_float32>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2*n; i++) ptr[i] = saturate_cast<ಚಿಹ್ನೆರಹಿತ>(buf[i]);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uchar* ptr, ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b)
{
    v_pack_store(ptr, v_pack(a, b));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ushort* ptr, ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b)
{
    v_store(ptr, v_pack(a, b));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಚಿಹ್ನೆರಹಿತ* ptr, ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_uint32>::vlanes(), b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uint64_t* ptr, ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b)
{
    v_uint64 q0, q1, q2, q3;
    v_expand(a, q0, q1);
    v_expand(b, q2, q3);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes = VTraits<v_uint64>::vlanes();
    v_store(ptr, q0);
    v_store(ptr + nlanes, q1);
    v_store(ptr + nlanes*2, q2);
    v_store(ptr + nlanes*3, q3);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uint64_t* ptr, ಸ್ಥಿರಾಂಕ v_uint64& a, ಸ್ಥಿರಾಂಕ v_uint64& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_uint64>::vlanes(), b);
}

// float32 -> FP8 encode, shared by both scale paths. Fallback: no portable per-lane variable shift.

// fallback test plus the pieces encodeFp8Finish needs, to skip costly math when unused
ಮಾದರಿ<ಪೂರ್ಣಾಂಕ bias> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
fp8Prepare(ಸ್ಥಿರಾಂಕ v_float32& vf, v_uint32& ತುಂಬಿದ, v_uint32& sbit, v_int32& newexpRaw, v_int32& fallback_ಮುಖವಾಡ)
{
    v_uint32 u = v_reinterpret_as_u32(vf);
    v_uint32 e = v_and(v_shr<23>(u), vx_setall_u32(0xFFu));
    v_uint32 m = v_and(u, vx_setall_u32(0x7FFFFFu));
    sbit = v_and(v_shr<24>(u), vx_setall_u32(0x80u));
    ತುಂಬಿದ = v_or(m, vx_setall_u32(1u << 23));
    newexpRaw = v_sub(v_reinterpret_as_s32(e), vx_setall_s32(127 - bias));
    v_int32 isNan = v_reinterpret_as_s32(v_eq(e, vx_setall_u32(0xFFu)));
    fallback_ಮುಖವಾಡ = v_or(isNan, v_le(newexpRaw, vx_setall_s32(0)));
}

// Valid only when fp8Prepare's fallbackMask is false; must stay bit-identical to encodeE4M3.
ಮಾದರಿ<ಪೂರ್ಣಾಂಕ bias, ತಾರ್ಕಿಕ fnuz> ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int32
encode_Fp8_ಮುಕ್ತಾಯ(v_uint32 ತುಂಬಿದ, v_uint32 sbit, v_int32 newexpRaw)
{
    v_uint32 one     = vx_setall_u32(1u);
    v_uint32 half    = vx_setall_u32(1u << 19);
    v_uint32 q       = v_shr<20>(ತುಂಬಿದ);
    v_uint32 rem     = v_and(ತುಂಬಿದ, vx_setall_u32((1u << 20) - 1));
    v_uint32 tie     = v_and(v_eq(rem, half), v_ne(v_and(q, one), vx_setzero_u32()));
    v_uint32 inc     = v_and(v_or(v_gt(rem, half), tie), one);
    v_uint32 rounded = v_ಕೂಡಿಸು(q, inc);

    v_uint32 ವಾಹಕ = v_ne(v_and(rounded, vx_setall_u32(16u)), vx_setall_u32(0u));
    rounded = v_select(ವಾಹಕ, v_shr<1>(rounded), rounded);
    v_int32 newexp = v_ಕೂಡಿಸು(newexpRaw, v_and(v_reinterpret_as_s32(ವಾಹಕ), vx_setall_s32(1)));

    v_uint32 mant = v_and(rounded, vx_setall_u32(7u));

    v_int32 gt15 = v_gt(newexp, vx_setall_s32(15));
    v_int32 overflow;
    ಆದರೆ constexpr (fnuz)
        overflow = gt15;
    ಇಲ್ಲದಿದ್ದರೆ
        overflow = v_or(gt15, v_and(v_eq(newexp, vx_setall_s32(15)),
                                     v_eq(v_reinterpret_as_s32(mant), vx_setall_s32(7))));

    v_uint32 maxFin = v_or(sbit, vx_setall_u32(fnuz ? 0x7Fu : 0x7Eu));

    v_uint32 ಸಾಮಾನ್ಯ = v_or(v_or(sbit, v_shl<3>(v_reinterpret_as_u32(newexp))), mant);
    v_uint32 ಫಲಿತಾಂಶ = v_select(v_reinterpret_as_u32(overflow), maxFin, ಸಾಮಾನ್ಯ);
    ಹಿಂತಿರುಗಿ v_reinterpret_as_s32(ಫಲಿತಾಂಶ);
}

// encodes vf0..vf3 to FP8 bytes; scalarSrc must hold the same values, contiguous, for fallback
ಮಾದರಿ<ಪೂರ್ಣಾಂಕ bias, ತಾರ್ಕಿಕ fnuz> ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
encodeFp8Vec4(ಸ್ಥಿರಾಂಕ v_float32& vf0, ಸ್ಥಿರಾಂಕ v_float32& vf1, ಸ್ಥಿರಾಂಕ v_float32& vf2, ಸ್ಥಿರಾಂಕ v_float32& vf3,
              ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* scalarSrc, ಪೂರ್ಣಾಂಕ vecsz, uchar* dst)
{
    v_uint32 full0, full1, full2, full3, sbit0, sbit1, sbit2, sbit3;
    v_int32 newexpRaw0, newexpRaw1, newexpRaw2, newexpRaw3;
    v_int32 pre0, pre1, pre2, pre3;
    fp8Prepare<bias>(vf0, full0, sbit0, newexpRaw0, pre0);
    fp8Prepare<bias>(vf1, full1, sbit1, newexpRaw1, pre1);
    fp8Prepare<bias>(vf2, full2, sbit2, newexpRaw2, pre2);
    fp8Prepare<bias>(vf3, full3, sbit3, newexpRaw3, pre3);

    ಆದರೆ (v_check_all(v_and(v_and(pre0, pre1), v_and(pre2, pre3))))
    {
        // whole group is subnormal/zero/NaN — skip the costlier fast-path math
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < vecsz; k++)
            dst[k] = fp8_detail::encodeE4M3(scalarSrc[k], bias, fnuz);
        ಹಿಂತಿರುಗಿ;
    }

    v_int32 byte0 = encode_Fp8_ಮುಕ್ತಾಯ<bias, fnuz>(full0, sbit0, newexpRaw0);
    v_int32 byte1 = encode_Fp8_ಮುಕ್ತಾಯ<bias, fnuz>(full1, sbit1, newexpRaw1);
    v_int32 byte2 = encode_Fp8_ಮುಕ್ತಾಯ<bias, fnuz>(full2, sbit2, newexpRaw2);
    v_int32 byte3 = encode_Fp8_ಮುಕ್ತಾಯ<bias, fnuz>(full3, sbit3, newexpRaw3);
    v_store(dst, v_pack_u(v_pack(byte0, byte1), v_pack(byte2, byte3)));

    v_int32 one = vx_setall_s32(1);
    uchar fallback[VTraits<v_uint8>::max_nlanes];
    v_store(fallback, v_pack_u(v_pack(v_and(pre0, one), v_and(pre1, one)),
                                v_pack(v_and(pre2, one), v_and(pre3, one))));
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < vecsz; k++)
        ಆದರೆ (fallback[k])
            dst[k] = fp8_detail::encodeE4M3(scalarSrc[k], bias, fnuz);
}

#ಆದರೆ (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// v_cvt_f64() converts SIGNED 64-bit lanes, so it cannot be used on a uint64 directly. Split into
// two halves below 2^32 instead: each converts exactly, scaling the high half by 2^32 is exact, and
// the single add at the end is the only rounding - so the result matches (double)x.
//
// Converting the bit pattern as signed and adding 2^64 back would be simpler, but it rounds twice:
// for 2^63 + 1025 it gives 2^63 where (double)x is 2^63 + 2048.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_float64 v_cvt_f64_u64(ಸ್ಥಿರಾಂಕ v_uint64& x)
{
    ಸ್ಥಿರಾಂಕ v_int64 hi = v_reinterpret_as_s64(v_shr<32>(x));
    ಸ್ಥಿರಾಂಕ v_int64 lo = v_reinterpret_as_s64(v_and(x, vx_setall_u64(0xFFFFFFFFull)));
    ಹಿಂತಿರುಗಿ v_ಕೂಡಿಸು(v_mul(v_cvt_f64(hi), vx_setall_f64(4294967296.0)), v_cvt_f64(lo));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ uint64_t* ptr, v_float32& a)
{
    v_float64 a_0 = v_cvt_f64_u64(vx_ತುಂಬು(ptr));
    v_float64 a_1 = v_cvt_f64_u64(vx_ತುಂಬು(ptr + VTraits<v_uint64>::vlanes()));
    a = v_cvt_f32(a_0, a_1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ int64_t* ptr, v_float32& a)
{
    v_float64 a_0 = v_cvt_f64(vx_ತುಂಬು(ptr));
    v_float64 a_1 = v_cvt_f64(vx_ತುಂಬು(ptr + VTraits<v_uint64>::vlanes()));
    a = v_cvt_f32(a_0, a_1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ptr, v_float32& a)
{
    v_float64 v0 = vx_ತುಂಬು(ptr), v1 = vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes());
    a = v_cvt_f32(v0, v1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ* ptr, v_float64& a, v_float64& b)
{
    v_uint32 z = vx_setzero_u32();
    v_uint32 uab = vx_ತುಂಬು_expand_q((ಸ್ಥಿರಾಂಕ uchar*)ptr);
    uab = v_shr<31>(v_gt(uab, z));
    v_float32 fab = v_cvt_f32(v_reinterpret_as_s32(uab));
    a = v_cvt_f64(fab);
    b = v_cvt_f64_ಮೇಲಿನ(fab);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ hfloat* ptr, v_float64& a, v_float64& b)
{
    v_float32 fab = vx_ತುಂಬು_expand(ptr);
    a = v_cvt_f64(fab);
    b = v_cvt_f64_ಮೇಲಿನ(fab);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ bfloat* ptr, v_float64& a, v_float64& b)
{
    v_float32 fab = vx_ತುಂಬು_expand(ptr);
    a = v_cvt_f64(fab);
    b = v_cvt_f64_ಮೇಲಿನ(fab);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ptr, v_int32& a, v_int32& b)
{
    v_float64 v0 = vx_ತುಂಬು(ptr), v1 = vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes());
    v_float64 v2 = vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes()*2), v3 = vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes()*3);
    v_int32 iv0 = v_ಸುತ್ತು(v0), iv1 = v_ಸುತ್ತು(v1);
    v_int32 iv2 = v_ಸುತ್ತು(v2), iv3 = v_ಸುತ್ತು(v3);
    a = v_combine_ಕೆಳಗಿನ(iv0, iv1);
    b = v_combine_ಕೆಳಗಿನ(iv2, iv3);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uint64_t* ptr, v_float64& a, v_float64& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_cvt_f64_u64(vx_ತುಂಬು(ptr));
    b = v_cvt_f64_u64(vx_ತುಂಬು(ptr + int64_nlanes));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ptr, v_float32& a, v_float32& b)
{
    v_float64 v0 = vx_ತುಂಬು(ptr), v1 = vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes());
    v_float64 v2 = vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes()*2), v3 = vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes()*3);
    a = v_cvt_f32(v0, v1);
    b = v_cvt_f32(v2, v3);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uchar* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = v_reinterpret_as_s32(vx_ತುಂಬು_expand_q(ptr));
    a = v_cvt_f64(v0);
    b = v_cvt_f64_ಮೇಲಿನ(v0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ schar* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_ತುಂಬು_expand_q(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_ಮೇಲಿನ(v0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ushort* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = v_reinterpret_as_s32(vx_ತುಂಬು_expand(ptr));
    a = v_cvt_f64(v0);
    b = v_cvt_f64_ಮೇಲಿನ(v0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_ತುಂಬು_expand(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_ಮೇಲಿನ(v0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_ತುಂಬು(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_ಮೇಲಿನ(v0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* ptr, v_float64& a, v_float64& b)
{
    v_float32 v0 = vx_ತುಂಬು(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_ಮೇಲಿನ(v0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ptr, v_float64& a, v_float64& b)
{
    a = vx_ತುಂಬು(ptr);
    b = vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes());
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ int64_t* ptr, v_float64& a, v_float64& b)
{
    a = v_cvt_f64(vx_ತುಂಬು(ptr));
    b = v_cvt_f64(vx_ತುಂಬು(ptr + VTraits<v_float64>::vlanes()));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* ptr, v_float64& a, v_float64& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes = VTraits<v_uint64>::vlanes();
    ದ್ವಿಗುಣ buf[VTraits<v_uint64>::max_nlanes*2];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nlanes*2; i++)
        buf[i] = (ದ್ವಿಗುಣ)ptr[i];
    a = vx_ತುಂಬು(buf);
    b = vx_ತುಂಬು(buf + nlanes);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(ದ್ವಿಗುಣ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_ಮೇಲಿನ(a);
    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ದ್ವಿಗುಣ* ptr, ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_ಮೇಲಿನ(a);
    v_float64 fb0 = v_cvt_f64(b), fb1 = v_cvt_f64_ಮೇಲಿನ(b);

    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
    v_store(ptr + VTraits<v_float64>::vlanes()*2, fb0);
    v_store(ptr + VTraits<v_float64>::vlanes()*3, fb1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ದ್ವಿಗುಣ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_ಮೇಲಿನ(a);
    v_float64 fb0 = v_cvt_f64(b), fb1 = v_cvt_f64_ಮೇಲಿನ(b);

    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
    v_store(ptr + VTraits<v_float64>::vlanes()*2, fb0);
    v_store(ptr + VTraits<v_float64>::vlanes()*3, fb1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ದ್ವಿಗುಣ* ptr, ಸ್ಥಿರಾಂಕ v_float64& a, ಸ್ಥಿರಾಂಕ v_float64& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_float64>::vlanes(), b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಪೂರ್ಣಾಂಕ* ptr, ಸ್ಥಿರಾಂಕ v_float64& a, ಸ್ಥಿರಾಂಕ v_float64& b)
{
    v_int32 ia = v_ಸುತ್ತು(a), ib = v_ಸುತ್ತು(b);
    v_store(ptr, v_combine_ಕೆಳಗಿನ(ia, ib));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ದಶಮಾಂಶ* ptr, ಸ್ಥಿರಾಂಕ v_float64& a, ಸ್ಥಿರಾಂಕ v_float64& b)
{
    v_float32 v = v_cvt_f32(a, b);
    v_store(ptr, v);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(hfloat* ptr, ಸ್ಥಿರಾಂಕ v_float64& a, ಸ್ಥಿರಾಂಕ v_float64& b)
{
    v_float32 v = v_cvt_f32(a, b);
    v_pack_store(ptr, v);
}

// f64 -> {u64, s64, u32}: no correct vector path yet - v_round(f64,f64) narrows to v_int32, which
// truncates the 64-bit range (u64/s64) and the upper half of u32. Scalar for now (matches the scalar
// tail these functions already fall back to); a proper f64->s64/u64 intrinsic can replace it later.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uint64_t* ptr, ಸ್ಥಿರಾಂಕ v_float64& a, ಸ್ಥಿರಾಂಕ v_float64& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = VTraits<v_float64>::vlanes();
    ದ್ವಿಗುಣ buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2*n; i++) ptr[i] = saturate_cast<uint64_t>(buf[i]);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(int64_t* ptr, ಸ್ಥಿರಾಂಕ v_float64& a, ಸ್ಥಿರಾಂಕ v_float64& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = VTraits<v_float64>::vlanes();
    ದ್ವಿಗುಣ buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2*n; i++) ptr[i] = saturate_cast<int64_t>(buf[i]);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಚಿಹ್ನೆರಹಿತ* ptr, ಸ್ಥಿರಾಂಕ v_float64& a, ಸ್ಥಿರಾಂಕ v_float64& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ n = VTraits<v_float64>::vlanes();
    ದ್ವಿಗುಣ buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2*n; i++) ptr[i] = saturate_cast<ಚಿಹ್ನೆರಹಿತ>(buf[i]);
}

#ಇಲ್ಲದಿದ್ದರೆ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ptr, v_float32& a)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ದಶಮಾಂಶ buf[VTraits<v_float32>::max_nlanes*2];

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<ದಶಮಾಂಶ>(ptr[i]);
    a = vx_ತುಂಬು(buf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ uint64_t* ptr, v_float32& a)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ದಶಮಾಂಶ buf[VTraits<v_float32>::max_nlanes*2];

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<ದಶಮಾಂಶ>(ptr[i]);
    a = vx_ತುಂಬು(buf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_as(ಸ್ಥಿರಾಂಕ int64_t* ptr, v_float32& a)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ದಶಮಾಂಶ buf[VTraits<v_float32>::max_nlanes*2];

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<ದಶಮಾಂಶ>(ptr[i]);
    a = vx_ತುಂಬು(buf);
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tdvec>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* ptr, _Tdvec& a, _Tdvec& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<_Tdvec>::vlanes();
    ಬಗೆ_ಹೆಸರು VTraits<_Tdvec>::lane_type buf[VTraits<_Tdvec>::max_nlanes*2];

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < VECSZ*2; i++ )
        buf[i] = saturate_cast<ಬಗೆ_ಹೆಸರು VTraits<_Tdvec>::lane_type>(ptr[i]);
    a = vx_ತುಂಬು(buf);
    b = vx_ತುಂಬು(buf + VECSZ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_as(ದ್ವಿಗುಣ* ptr, ಸ್ಥಿರಾಂಕ v_float32& a)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<v_float32>::vlanes();
    ದಶಮಾಂಶ buf[VTraits<v_float32>::max_nlanes];

    v_store(buf, a);
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < VECSZ; i++ )
        ptr[i] = (ದ್ವಿಗುಣ)buf[i];
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tsvec>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ದ್ವಿಗುಣ* ptr, ಸ್ಥಿರಾಂಕ _Tsvec& a, ಸ್ಥಿರಾಂಕ _Tsvec& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<_Tsvec>::vlanes();
    ಬಗೆ_ಹೆಸರು VTraits<_Tsvec>::lane_type buf[VTraits<_Tsvec>::max_nlanes*2];

    v_store(buf, a); v_store(buf + VECSZ, b);
    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < VECSZ*2; i++ )
        ptr[i] = (ದ್ವಿಗುಣ)buf[i];
}

#ಆದರೆ_ಅಂತ್ಯ /////////// CV_SIMD_64F || CV_SIMD_SCALABLE_64F

#ಆದರೆ_ಅಂತ್ಯ /////////// CV_SIMD || CV_SIMD_SCALABLE

}

#ಆದರೆ_ಅಂತ್ಯ // SRC_CONVERT_HPP
