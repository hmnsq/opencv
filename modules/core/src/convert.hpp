// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#اگر_غیر_تعریف_شدہ SRC_CONVERT_HPP
#تعریف_کرو SRC_CONVERT_HPP

#شامل "opencv2/core/types.hpp"
#شامل "opencv2/core/hal/intrin.hpp"

نامخلا cv
{

#اگر (CV_SIMD || CV_SIMD_SCALABLE)

ساکن اندرونی خالی vx_بھرو_as(ثابت uchar* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_بھرو_expand_q(ptr))); }

ساکن اندرونی خالی vx_بھرو_as(ثابت schar* ptr, v_float32& a)
{ a = v_cvt_f32(vx_بھرو_expand_q(ptr)); }

ساکن اندرونی خالی vx_بھرو_as(ثابت ushort* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_بھرو_expand(ptr))); }

ساکن اندرونی خالی vx_بھرو_as(ثابت مختصر* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_بھرو_expand(ptr))); }

ساکن اندرونی خالی vx_بھرو_as(ثابت صحیح* ptr, v_float32& a)
{ a = v_cvt_f32(vx_بھرو(ptr)); }

ساکن اندرونی خالی vx_بھرو_as(ثابت غیرعلامتی* ptr, v_float32& a)
{
    v_uint32 delta = vx_setall_u32(0x80000000U);
    v_uint32 ua = vx_بھرو(ptr);
    v_uint32 نقاب_a = v_and(v_ge(ua, delta), delta);
    v_float32 fmask_a = v_cvt_f32(v_reinterpret_as_s32(نقاب_a)); // 0.f or (float)(-(1 << 31))
    a = v_cvt_f32(v_reinterpret_as_s32(v_sub(ua, نقاب_a)));
    // restore the original values
    a = v_sub(a, fmask_a); // subtract 0 or a large negative number
}

ساکن اندرونی خالی vx_بھرو_as(ثابت اعشاری* ptr, v_float32& a)
{ a = vx_بھرو(ptr); }

ساکن اندرونی خالی vx_بھرو_as(ثابت hfloat* ptr, v_float32& a)
{ a = vx_بھرو_expand(ptr); }

ساکن اندرونی خالی vx_بھرو_as(ثابت bfloat* ptr, v_float32& a)
{ a = vx_بھرو_expand(ptr); }

ساکن اندرونی خالی v_store_as(ushort* ptr, ثابت v_float32& a)
{ v_pack_u_store(ptr, v_باری(a)); }

ساکن اندرونی خالی v_store_as(مختصر* ptr, ثابت v_float32& a)
{ v_pack_store(ptr, v_باری(a)); }

ساکن اندرونی خالی v_store_as(صحیح* ptr, ثابت v_float32& a)
{ v_store(ptr, v_باری(a)); }

ساکن اندرونی خالی v_store_as(غیرعلامتی* ptr, ثابت v_float32& a)
{
    v_float32 z = vx_setzero_f32();
    v_store(ptr, v_reinterpret_as_u32(v_باری(v_max(a, z))));
}

ساکن اندرونی خالی v_store_as(اعشاری* ptr, ثابت v_float32& a)
{ v_store(ptr, a); }

ساکن اندرونی خالی v_store_as(hfloat* ptr, ثابت v_float32& a)
{ v_pack_store(ptr, a); }

ساکن اندرونی خالی v_store_as(bfloat* ptr, ثابت v_float32& a)
{ v_pack_store(ptr, a); }

ساکن اندرونی خالی v_store_as(int64_t* ptr, ثابت v_float32& a)
{
    v_int32 ia = v_باری(a);
    v_int64 ia_0, ia_1;
    v_expand(ia, ia_0, ia_1);
    v_store(ptr, ia_0);
    v_store(ptr + VTraits<v_uint64>::vlanes(), ia_1);
}

ساکن اندرونی خالی v_store_as(uint64_t* ptr, ثابت v_float32& a)
{
    v_int32 ia = v_باری(a);
    v_uint64 ia_0, ia_1;
    ia = v_max(ia, vx_setzero_s32());
    v_expand(v_reinterpret_as_u32(ia), ia_0, ia_1);
    v_store(ptr, ia_0);
    v_store(ptr + VTraits<v_uint64>::vlanes(), ia_1);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uchar* ptr, v_uint16& a, v_uint16& b)
{ v_expand(vx_بھرو(ptr), a, b); }

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت schar* ptr, v_uint16& a, v_uint16& b)
{
    ثابت v_int8 z = vx_setzero_s8();
    v_int16 sa, sb;
    v_expand(v_max(vx_بھرو(ptr), z), sa, sb);
    a = v_reinterpret_as_u16(sa);
    b = v_reinterpret_as_u16(sb);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت ushort* ptr, v_uint16& a, v_uint16& b)
{ a = vx_بھرو(ptr); b = vx_بھرو(ptr + VTraits<v_uint16>::vlanes()); }

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uchar* ptr, v_int16& a, v_int16& b)
{
    v_uint16 ua, ub;
    v_expand(vx_بھرو(ptr), ua, ub);
    a = v_reinterpret_as_s16(ua);
    b = v_reinterpret_as_s16(ub);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت schar* ptr, v_int16& a, v_int16& b)
{ v_expand(vx_بھرو(ptr), a, b); }

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت مختصر* ptr, v_int16& a, v_int16& b)
{ a = vx_بھرو(ptr); b = vx_بھرو(ptr + VTraits<v_uint16>::vlanes()); }

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uchar* ptr, v_int32& a, v_int32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_بھرو_expand(ptr), ua, ub);
    a = v_reinterpret_as_s32(ua);
    b = v_reinterpret_as_s32(ub);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت schar* ptr, v_int32& a, v_int32& b)
{ v_expand(vx_بھرو_expand(ptr), a, b); }

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت ushort* ptr, v_int32& a, v_int32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_بھرو(ptr), ua, ub);
    a = v_reinterpret_as_s32(ua);
    b = v_reinterpret_as_s32(ub);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت ushort* ptr, v_uint32& a, v_uint32& b)
{
    v_expand(vx_بھرو(ptr), a, b);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت مختصر* ptr, v_int32& a, v_int32& b)
{
    v_expand(vx_بھرو(ptr), a, b);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت صحیح* ptr, v_int32& a, v_int32& b)
{
    a = vx_بھرو(ptr);
    b = vx_بھرو(ptr + VTraits<v_int32>::vlanes());
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uchar* ptr, v_float32& a, v_float32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_بھرو_expand(ptr), ua, ub);
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
    b = v_cvt_f32(v_reinterpret_as_s32(ub));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت schar* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia, ib;
    v_expand(vx_بھرو_expand(ptr), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت ushort* ptr, v_float32& a, v_float32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_بھرو(ptr), ua, ub);
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
    b = v_cvt_f32(v_reinterpret_as_s32(ub));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت مختصر* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia, ib;
    v_expand(vx_بھرو(ptr), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت صحیح* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia = vx_بھرو(ptr), ib = vx_بھرو(ptr + VTraits<v_int32>::vlanes());
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

// v_pack()/v_pack_u() saturate at every width, 64->32 included, so a 64-bit source needs no
// clamping here any more - see the HAL headers for the per-backend implementations.

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت int64_t* ptr, v_int32& a, v_int32& b)
{
    ثابت صحیح int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack(vx_بھرو(ptr), vx_بھرو(ptr + int64_nlanes));
    b = v_pack(vx_بھرو(ptr + int64_nlanes*2), vx_بھرو(ptr + int64_nlanes*3));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت int64_t* ptr, v_uint64& a, v_uint64& b)
{
    // clamp the negatives to zero through the sign bit: v_gt on 64-bit lanes is sign(b - a) at the
    // SSE and NEON baselines, which overflows for an operand at the bottom of the range
    v_int64 ia = vx_بھرو(ptr), ib = vx_بھرو(ptr + VTraits<v_uint64>::vlanes());
    ia = v_and(ia, v_not(v_shr<63>(ia)));
    ib = v_and(ib, v_not(v_shr<63>(ib)));
    a = v_reinterpret_as_u64(ia);
    b = v_reinterpret_as_u64(ib);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت int64_t* ptr, v_uint32& a, v_uint32& b)
{
    ثابت صحیح nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack_u(vx_بھرو(ptr), vx_بھرو(ptr + nlanes));
    b = v_pack_u(vx_بھرو(ptr + nlanes*2), vx_بھرو(ptr + nlanes*3));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uint64_t* ptr, v_float32& a, v_float32& b)
{
    ثابت صحیح nlanes = VTraits<v_uint64>::vlanes();
    اعشاری buf[VTraits<v_uint64>::max_nlanes*4];
    برائے (صحیح i = 0; i < nlanes*4; i++) {
        buf[i] = (اعشاری)ptr[i];
    }
    a = vx_بھرو(buf);
    b = vx_بھرو(buf + nlanes*2);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت int64_t* ptr, v_float32& a, v_float32& b)
{
    ثابت صحیح nlanes = VTraits<v_uint64>::vlanes();
    اعشاری buf[VTraits<v_uint64>::max_nlanes*4];
    برائے (صحیح i = 0; i < nlanes*4; i++) {
        buf[i] = (اعشاری)ptr[i];
    }
    a = vx_بھرو(buf);
    b = vx_بھرو(buf + nlanes*2);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت منطقی* ptr, v_float32& a, v_float32& b)
{
    v_uint16 z = vx_setzero_u16();
    v_uint16 uab = vx_بھرو_expand((ثابت uchar*)ptr);
    uab = v_shr<15>(v_gt(uab, z));
    v_int32 ia, ib;
    v_expand(v_reinterpret_as_s16(uab), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

ساکن اندرونی خالی vx_بھرو_as(ثابت منطقی* ptr, v_float32& a)
{
    v_uint32 z = vx_setzero_u32();
    v_uint32 ua = vx_بھرو_expand_q((ثابت uchar*)ptr);
    ua = v_shr<31>(v_gt(ua, z));
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت schar* ptr, v_uint32& a, v_uint32& b)
{
    v_int16 ab = v_max(vx_بھرو_expand(ptr), vx_setzero_s16());
    v_expand(v_reinterpret_as_u16(ab), a, b);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت مختصر* ptr, v_uint32& a, v_uint32& b)
{
    v_int16 ab = v_max(vx_بھرو(ptr), vx_setzero_s16());
    v_expand(v_reinterpret_as_u16(ab), a, b);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت صحیح* ptr, v_uint32& a, v_uint32& b)
{
    v_int32 z = vx_setzero_s32();
    v_int32 ia = v_max(vx_بھرو(ptr), z);
    v_int32 ib = v_max(vx_بھرو(ptr + VTraits<v_int32>::vlanes()), z);
    a = v_reinterpret_as_u32(ia);
    b = v_reinterpret_as_u32(ib);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uint64_t* ptr, v_uint32& a, v_uint32& b)
{
    ثابت صحیح int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack(vx_بھرو(ptr), vx_بھرو(ptr + int64_nlanes));
    b = v_pack(vx_بھرو(ptr + int64_nlanes*2), vx_بھرو(ptr + int64_nlanes*3));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uint64_t* ptr, v_int32& a, v_int32& b)
{
    // There is no uint64 -> int32 narrow in the HAL, so narrow as unsigned first - v_pack() clamps
    // anything above UINT32_MAX to UINT32_MAX - and bring that down to INT32_MAX with the same
    // 32-bit v_min() the uint32 -> int32 direction below uses.
    ثابت صحیح int64_nlanes = VTraits<v_uint64>::vlanes();
    ثابت v_uint32 hi = vx_setall_u32((غیرعلامتی)زیادہ_سے_زیادہ_عدد);
    a = v_reinterpret_as_s32(v_min(v_pack(vx_بھرو(ptr), vx_بھرو(ptr + int64_nlanes)), hi));
    b = v_reinterpret_as_s32(v_min(v_pack(vx_بھرو(ptr + int64_nlanes*2),
                                          vx_بھرو(ptr + int64_nlanes*3)), hi));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت اعشاری* ptr, v_float32& a, v_float32& b)
{ a = vx_بھرو(ptr); b = vx_بھرو(ptr + VTraits<v_float32>::vlanes()); }

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت hfloat* ptr, v_float32& a, v_float32& b)
{
    a = vx_بھرو_expand(ptr);
    b = vx_بھرو_expand(ptr + VTraits<v_float32>::vlanes());
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت bfloat* ptr, v_float32& a, v_float32& b)
{
    a = vx_بھرو_expand(ptr);
    b = vx_بھرو_expand(ptr + VTraits<v_float32>::vlanes());
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت غیرعلامتی* ptr, v_uint32& a, v_uint32& b)
{
    a = vx_بھرو(ptr);
    b = vx_بھرو(ptr + VTraits<v_uint32>::vlanes());
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت غیرعلامتی* ptr, v_int32& a, v_int32& b)
{
    // Reinterpreting alone would turn everything above INT32_MAX negative, and the saturating
    // narrow that follows would then clamp it to the WRONG end of the destination's range
    // (4294967295 -> -1 -> -128 for schar, rather than 127). Clamp first, like the int -> unsigned
    // direction above does for negatives.
    ثابت v_uint32 hi = vx_setall_u32((غیرعلامتی)زیادہ_سے_زیادہ_عدد);
    a = v_reinterpret_as_s32(v_min(vx_بھرو(ptr), hi));
    b = v_reinterpret_as_s32(v_min(vx_بھرو(ptr + VTraits<v_uint32>::vlanes()), hi));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت غیرعلامتی* ptr, v_float32& a, v_float32& b)
{
    v_uint32 delta = vx_setall_u32(0x80000000U);
    v_uint32 ua = vx_بھرو(ptr);
    v_uint32 ub = vx_بھرو(ptr + VTraits<v_uint32>::vlanes());
    v_uint32 نقاب_a = v_and(v_ge(ua, delta), delta), نقاب_b = v_and(v_ge(ub, delta), delta);
    v_float32 fmask_a = v_cvt_f32(v_reinterpret_as_s32(نقاب_a)); // 0.f or (float)(-(1 << 31))
    v_float32 fmask_b = v_cvt_f32(v_reinterpret_as_s32(نقاب_b)); // 0.f or (float)(-(1 << 31))
    a = v_cvt_f32(v_reinterpret_as_s32(v_sub(ua, نقاب_a)));
    b = v_cvt_f32(v_reinterpret_as_s32(v_sub(ub, نقاب_b)));
    // restore the original values
    a = v_sub(a, fmask_a); // subtract 0 or a large negative number
    b = v_sub(b, fmask_b); // subtract 0 or a large negative number
}

ساکن اندرونی خالی v_store_pair_as(uchar* ptr, ثابت v_uint16& a, ثابت v_uint16& b)
{
    v_store(ptr, v_pack(a, b));
}

ساکن اندرونی خالی v_store_pair_as(schar* ptr, ثابت v_uint16& a, ثابت v_uint16& b)
{
    ثابت v_uint8 maxval = vx_setall_u8((uchar)std::numeric_limits<schar>::max());
    v_uint8 v = v_pack(a, b);
    v_store(ptr, v_reinterpret_as_s8(v_min(v, maxval)));
}

ساکن اندرونی خالی v_store_pair_as(ushort* ptr, ثابت v_uint16& a, ثابت v_uint16& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_uint16>::vlanes(), b); }

ساکن اندرونی خالی v_store_pair_as(uchar* ptr, ثابت v_int16& a, ثابت v_int16& b)
{ v_store(ptr, v_pack_u(a, b)); }

ساکن اندرونی خالی v_store_pair_as(schar* ptr, ثابت v_int16& a, ثابت v_int16& b)
{ v_store(ptr, v_pack(a, b)); }

ساکن اندرونی خالی v_store_pair_as(مختصر* ptr, ثابت v_int16& a, ثابت v_int16& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_int16>::vlanes(), b); }

ساکن اندرونی خالی v_store_pair_as(uchar* ptr, ثابت v_int32& a, ثابت v_int32& b)
{ v_pack_u_store(ptr, v_pack(a, b)); }

ساکن اندرونی خالی v_store_pair_as(schar* ptr, ثابت v_int32& a, ثابت v_int32& b)
{ v_pack_store(ptr, v_pack(a, b)); }

ساکن اندرونی خالی v_store_pair_as(ushort* ptr, ثابت v_int32& a, ثابت v_int32& b)
{ v_store(ptr, v_pack_u(a, b)); }

ساکن اندرونی خالی v_store_pair_as(مختصر* ptr, ثابت v_int32& a, ثابت v_int32& b)
{ v_store(ptr, v_pack(a, b)); }

ساکن اندرونی خالی v_store_pair_as(صحیح* ptr, ثابت v_int32& a, ثابت v_int32& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_int32>::vlanes(), b);
}

ساکن اندرونی خالی v_store_pair_as(int64_t* ptr, ثابت v_int32& a, ثابت v_int32& b)
{
    v_int64 q0, q1, q2, q3;
    v_expand(a, q0, q1);
    v_expand(b, q2, q3);
    ثابت صحیح nlanes = VTraits<v_uint64>::vlanes();
    v_store(ptr, q0);
    v_store(ptr + nlanes, q1);
    v_store(ptr + nlanes*2, q2);
    v_store(ptr + nlanes*3, q3);
}

ساکن اندرونی خالی v_store_pair_as(uchar* ptr, ثابت v_float32& a, ثابت v_float32& b)
{ v_pack_u_store(ptr, v_pack(v_باری(a), v_باری(b))); }

ساکن اندرونی خالی v_store_pair_as(schar* ptr, ثابت v_float32& a, ثابت v_float32& b)
{ v_pack_store(ptr, v_pack(v_باری(a), v_باری(b))); }

ساکن اندرونی خالی v_store_pair_as(منطقی* ptr, ثابت v_float32& a, ثابت v_float32& b)
{
    v_float32 z = vx_setzero_f32();
    v_uint32 ma = v_shr<31>(v_reinterpret_as_u32(v_ne(a, z)));
    v_uint32 mb = v_shr<31>(v_reinterpret_as_u32(v_ne(b, z)));
    v_uint16 mab = v_pack(ma, mb);
    v_pack_store((uchar*)ptr, mab);
}

ساکن اندرونی خالی v_store_pair_as(ushort* ptr, ثابت v_float32& a, ثابت v_float32& b)
{ v_store(ptr, v_pack_u(v_باری(a), v_باری(b))); }

ساکن اندرونی خالی v_store_pair_as(مختصر* ptr, ثابت v_float32& a, ثابت v_float32& b)
{ v_store(ptr, v_pack(v_باری(a), v_باری(b))); }

ساکن اندرونی خالی v_store_pair_as(صحیح* ptr, ثابت v_float32& a, ثابت v_float32& b)
{
    v_int32 ia = v_باری(a), ib = v_باری(b);
    v_store(ptr, ia);
    v_store(ptr + VTraits<v_int32>::vlanes(), ib);
}

ساکن اندرونی خالی v_store_pair_as(اعشاری* ptr, ثابت v_float32& a, ثابت v_float32& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_float32>::vlanes(), b); }

ساکن اندرونی خالی v_store_pair_as(غیرعلامتی* ptr, ثابت v_float32& a, ثابت v_float32& b)
{
    // v_round(f32) narrows to v_int32, so values in [2^31, 2^32) saturate to INT32_MAX. Scalar for now
    // (same class as the f64->{u64,s64,u32} stores above); a proper f32->u32 intrinsic can replace it.
    ثابت صحیح n = VTraits<v_float32>::vlanes();
    اعشاری buf[VTraits<v_float32>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    برائے (صحیح i = 0; i < 2*n; i++) ptr[i] = saturate_cast<غیرعلامتی>(buf[i]);
}

ساکن اندرونی خالی v_store_pair_as(uchar* ptr, ثابت v_uint32& a, ثابت v_uint32& b)
{
    v_pack_store(ptr, v_pack(a, b));
}

ساکن اندرونی خالی v_store_pair_as(ushort* ptr, ثابت v_uint32& a, ثابت v_uint32& b)
{
    v_store(ptr, v_pack(a, b));
}

ساکن اندرونی خالی v_store_pair_as(غیرعلامتی* ptr, ثابت v_uint32& a, ثابت v_uint32& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_uint32>::vlanes(), b);
}

ساکن اندرونی خالی v_store_pair_as(uint64_t* ptr, ثابت v_uint32& a, ثابت v_uint32& b)
{
    v_uint64 q0, q1, q2, q3;
    v_expand(a, q0, q1);
    v_expand(b, q2, q3);
    ثابت صحیح nlanes = VTraits<v_uint64>::vlanes();
    v_store(ptr, q0);
    v_store(ptr + nlanes, q1);
    v_store(ptr + nlanes*2, q2);
    v_store(ptr + nlanes*3, q3);
}

ساکن اندرونی خالی v_store_pair_as(uint64_t* ptr, ثابت v_uint64& a, ثابت v_uint64& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_uint64>::vlanes(), b);
}

// float32 -> FP8 encode, shared by both scale paths. Fallback: no portable per-lane variable shift.

// fallback test plus the pieces encodeFp8Finish needs, to skip costly math when unused
سانچہ<صحیح bias> ساکن اندرونی خالی
fp8Prepare(ثابت v_float32& vf, v_uint32& بھرا, v_uint32& sbit, v_int32& newexpRaw, v_int32& fallback_نقاب)
{
    v_uint32 u = v_reinterpret_as_u32(vf);
    v_uint32 e = v_and(v_shr<23>(u), vx_setall_u32(0xFFu));
    v_uint32 m = v_and(u, vx_setall_u32(0x7FFFFFu));
    sbit = v_and(v_shr<24>(u), vx_setall_u32(0x80u));
    بھرا = v_or(m, vx_setall_u32(1u << 23));
    newexpRaw = v_sub(v_reinterpret_as_s32(e), vx_setall_s32(127 - bias));
    v_int32 isNan = v_reinterpret_as_s32(v_eq(e, vx_setall_u32(0xFFu)));
    fallback_نقاب = v_or(isNan, v_le(newexpRaw, vx_setall_s32(0)));
}

// Valid only when fp8Prepare's fallbackMask is false; must stay bit-identical to encodeE4M3.
سانچہ<صحیح bias, منطقی fnuz> ساکن اندرونی v_int32
encode_Fp8_اختتام(v_uint32 بھرا, v_uint32 sbit, v_int32 newexpRaw)
{
    v_uint32 one     = vx_setall_u32(1u);
    v_uint32 half    = vx_setall_u32(1u << 19);
    v_uint32 q       = v_shr<20>(بھرا);
    v_uint32 rem     = v_and(بھرا, vx_setall_u32((1u << 20) - 1));
    v_uint32 tie     = v_and(v_eq(rem, half), v_ne(v_and(q, one), vx_setzero_u32()));
    v_uint32 inc     = v_and(v_or(v_gt(rem, half), tie), one);
    v_uint32 rounded = v_جوڑو(q, inc);

    v_uint32 حاصل = v_ne(v_and(rounded, vx_setall_u32(16u)), vx_setall_u32(0u));
    rounded = v_select(حاصل, v_shr<1>(rounded), rounded);
    v_int32 newexp = v_جوڑو(newexpRaw, v_and(v_reinterpret_as_s32(حاصل), vx_setall_s32(1)));

    v_uint32 mant = v_and(rounded, vx_setall_u32(7u));

    v_int32 gt15 = v_gt(newexp, vx_setall_s32(15));
    v_int32 overflow;
    اگر constexpr (fnuz)
        overflow = gt15;
    ورنہ
        overflow = v_or(gt15, v_and(v_eq(newexp, vx_setall_s32(15)),
                                     v_eq(v_reinterpret_as_s32(mant), vx_setall_s32(7))));

    v_uint32 maxFin = v_or(sbit, vx_setall_u32(fnuz ? 0x7Fu : 0x7Eu));

    v_uint32 معمول = v_or(v_or(sbit, v_shl<3>(v_reinterpret_as_u32(newexp))), mant);
    v_uint32 نتیجہ = v_select(v_reinterpret_as_u32(overflow), maxFin, معمول);
    واپس v_reinterpret_as_s32(نتیجہ);
}

// encodes vf0..vf3 to FP8 bytes; scalarSrc must hold the same values, contiguous, for fallback
سانچہ<صحیح bias, منطقی fnuz> ساکن اندرونی خالی
encodeFp8Vec4(ثابت v_float32& vf0, ثابت v_float32& vf1, ثابت v_float32& vf2, ثابت v_float32& vf3,
              ثابت اعشاری* scalarSrc, صحیح vecsz, uchar* dst)
{
    v_uint32 full0, full1, full2, full3, sbit0, sbit1, sbit2, sbit3;
    v_int32 newexpRaw0, newexpRaw1, newexpRaw2, newexpRaw3;
    v_int32 pre0, pre1, pre2, pre3;
    fp8Prepare<bias>(vf0, full0, sbit0, newexpRaw0, pre0);
    fp8Prepare<bias>(vf1, full1, sbit1, newexpRaw1, pre1);
    fp8Prepare<bias>(vf2, full2, sbit2, newexpRaw2, pre2);
    fp8Prepare<bias>(vf3, full3, sbit3, newexpRaw3, pre3);

    اگر (v_check_all(v_and(v_and(pre0, pre1), v_and(pre2, pre3))))
    {
        // whole group is subnormal/zero/NaN — skip the costlier fast-path math
        برائے (صحیح k = 0; k < vecsz; k++)
            dst[k] = fp8_detail::encodeE4M3(scalarSrc[k], bias, fnuz);
        واپس;
    }

    v_int32 byte0 = encode_Fp8_اختتام<bias, fnuz>(full0, sbit0, newexpRaw0);
    v_int32 byte1 = encode_Fp8_اختتام<bias, fnuz>(full1, sbit1, newexpRaw1);
    v_int32 byte2 = encode_Fp8_اختتام<bias, fnuz>(full2, sbit2, newexpRaw2);
    v_int32 byte3 = encode_Fp8_اختتام<bias, fnuz>(full3, sbit3, newexpRaw3);
    v_store(dst, v_pack_u(v_pack(byte0, byte1), v_pack(byte2, byte3)));

    v_int32 one = vx_setall_s32(1);
    uchar fallback[VTraits<v_uint8>::max_nlanes];
    v_store(fallback, v_pack_u(v_pack(v_and(pre0, one), v_and(pre1, one)),
                                v_pack(v_and(pre2, one), v_and(pre3, one))));
    برائے (صحیح k = 0; k < vecsz; k++)
        اگر (fallback[k])
            dst[k] = fp8_detail::encodeE4M3(scalarSrc[k], bias, fnuz);
}

#اگر (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// v_cvt_f64() converts SIGNED 64-bit lanes, so it cannot be used on a uint64 directly. Split into
// two halves below 2^32 instead: each converts exactly, scaling the high half by 2^32 is exact, and
// the single add at the end is the only rounding - so the result matches (double)x.
//
// Converting the bit pattern as signed and adding 2^64 back would be simpler, but it rounds twice:
// for 2^63 + 1025 it gives 2^63 where (double)x is 2^63 + 2048.
ساکن اندرونی v_float64 v_cvt_f64_u64(ثابت v_uint64& x)
{
    ثابت v_int64 hi = v_reinterpret_as_s64(v_shr<32>(x));
    ثابت v_int64 lo = v_reinterpret_as_s64(v_and(x, vx_setall_u64(0xFFFFFFFFull)));
    واپس v_جوڑو(v_mul(v_cvt_f64(hi), vx_setall_f64(4294967296.0)), v_cvt_f64(lo));
}

ساکن اندرونی خالی vx_بھرو_as(ثابت uint64_t* ptr, v_float32& a)
{
    v_float64 a_0 = v_cvt_f64_u64(vx_بھرو(ptr));
    v_float64 a_1 = v_cvt_f64_u64(vx_بھرو(ptr + VTraits<v_uint64>::vlanes()));
    a = v_cvt_f32(a_0, a_1);
}

ساکن اندرونی خالی vx_بھرو_as(ثابت int64_t* ptr, v_float32& a)
{
    v_float64 a_0 = v_cvt_f64(vx_بھرو(ptr));
    v_float64 a_1 = v_cvt_f64(vx_بھرو(ptr + VTraits<v_uint64>::vlanes()));
    a = v_cvt_f32(a_0, a_1);
}

ساکن اندرونی خالی vx_بھرو_as(ثابت دوہرا* ptr, v_float32& a)
{
    v_float64 v0 = vx_بھرو(ptr), v1 = vx_بھرو(ptr + VTraits<v_float64>::vlanes());
    a = v_cvt_f32(v0, v1);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت منطقی* ptr, v_float64& a, v_float64& b)
{
    v_uint32 z = vx_setzero_u32();
    v_uint32 uab = vx_بھرو_expand_q((ثابت uchar*)ptr);
    uab = v_shr<31>(v_gt(uab, z));
    v_float32 fab = v_cvt_f32(v_reinterpret_as_s32(uab));
    a = v_cvt_f64(fab);
    b = v_cvt_f64_بلند(fab);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت hfloat* ptr, v_float64& a, v_float64& b)
{
    v_float32 fab = vx_بھرو_expand(ptr);
    a = v_cvt_f64(fab);
    b = v_cvt_f64_بلند(fab);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت bfloat* ptr, v_float64& a, v_float64& b)
{
    v_float32 fab = vx_بھرو_expand(ptr);
    a = v_cvt_f64(fab);
    b = v_cvt_f64_بلند(fab);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت دوہرا* ptr, v_int32& a, v_int32& b)
{
    v_float64 v0 = vx_بھرو(ptr), v1 = vx_بھرو(ptr + VTraits<v_float64>::vlanes());
    v_float64 v2 = vx_بھرو(ptr + VTraits<v_float64>::vlanes()*2), v3 = vx_بھرو(ptr + VTraits<v_float64>::vlanes()*3);
    v_int32 iv0 = v_باری(v0), iv1 = v_باری(v1);
    v_int32 iv2 = v_باری(v2), iv3 = v_باری(v3);
    a = v_combine_کم(iv0, iv1);
    b = v_combine_کم(iv2, iv3);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uint64_t* ptr, v_float64& a, v_float64& b)
{
    ثابت صحیح int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_cvt_f64_u64(vx_بھرو(ptr));
    b = v_cvt_f64_u64(vx_بھرو(ptr + int64_nlanes));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت دوہرا* ptr, v_float32& a, v_float32& b)
{
    v_float64 v0 = vx_بھرو(ptr), v1 = vx_بھرو(ptr + VTraits<v_float64>::vlanes());
    v_float64 v2 = vx_بھرو(ptr + VTraits<v_float64>::vlanes()*2), v3 = vx_بھرو(ptr + VTraits<v_float64>::vlanes()*3);
    a = v_cvt_f32(v0, v1);
    b = v_cvt_f32(v2, v3);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uchar* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = v_reinterpret_as_s32(vx_بھرو_expand_q(ptr));
    a = v_cvt_f64(v0);
    b = v_cvt_f64_بلند(v0);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت schar* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_بھرو_expand_q(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_بلند(v0);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت ushort* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = v_reinterpret_as_s32(vx_بھرو_expand(ptr));
    a = v_cvt_f64(v0);
    b = v_cvt_f64_بلند(v0);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت مختصر* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_بھرو_expand(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_بلند(v0);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت صحیح* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_بھرو(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_بلند(v0);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت اعشاری* ptr, v_float64& a, v_float64& b)
{
    v_float32 v0 = vx_بھرو(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_بلند(v0);
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت دوہرا* ptr, v_float64& a, v_float64& b)
{
    a = vx_بھرو(ptr);
    b = vx_بھرو(ptr + VTraits<v_float64>::vlanes());
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت int64_t* ptr, v_float64& a, v_float64& b)
{
    a = v_cvt_f64(vx_بھرو(ptr));
    b = v_cvt_f64(vx_بھرو(ptr + VTraits<v_float64>::vlanes()));
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت غیرعلامتی* ptr, v_float64& a, v_float64& b)
{
    ثابت صحیح nlanes = VTraits<v_uint64>::vlanes();
    دوہرا buf[VTraits<v_uint64>::max_nlanes*2];
    برائے (صحیح i = 0; i < nlanes*2; i++)
        buf[i] = (دوہرا)ptr[i];
    a = vx_بھرو(buf);
    b = vx_بھرو(buf + nlanes);
}

ساکن اندرونی خالی v_store_as(دوہرا* ptr, ثابت v_float32& a)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_بلند(a);
    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
}

ساکن اندرونی خالی v_store_pair_as(دوہرا* ptr, ثابت v_int32& a, ثابت v_int32& b)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_بلند(a);
    v_float64 fb0 = v_cvt_f64(b), fb1 = v_cvt_f64_بلند(b);

    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
    v_store(ptr + VTraits<v_float64>::vlanes()*2, fb0);
    v_store(ptr + VTraits<v_float64>::vlanes()*3, fb1);
}

ساکن اندرونی خالی v_store_pair_as(دوہرا* ptr, ثابت v_float32& a, ثابت v_float32& b)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_بلند(a);
    v_float64 fb0 = v_cvt_f64(b), fb1 = v_cvt_f64_بلند(b);

    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
    v_store(ptr + VTraits<v_float64>::vlanes()*2, fb0);
    v_store(ptr + VTraits<v_float64>::vlanes()*3, fb1);
}

ساکن اندرونی خالی v_store_pair_as(دوہرا* ptr, ثابت v_float64& a, ثابت v_float64& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_float64>::vlanes(), b);
}

ساکن اندرونی خالی v_store_pair_as(صحیح* ptr, ثابت v_float64& a, ثابت v_float64& b)
{
    v_int32 ia = v_باری(a), ib = v_باری(b);
    v_store(ptr, v_combine_کم(ia, ib));
}

ساکن اندرونی خالی v_store_pair_as(اعشاری* ptr, ثابت v_float64& a, ثابت v_float64& b)
{
    v_float32 v = v_cvt_f32(a, b);
    v_store(ptr, v);
}

ساکن اندرونی خالی v_store_pair_as(hfloat* ptr, ثابت v_float64& a, ثابت v_float64& b)
{
    v_float32 v = v_cvt_f32(a, b);
    v_pack_store(ptr, v);
}

// f64 -> {u64, s64, u32}: no correct vector path yet - v_round(f64,f64) narrows to v_int32, which
// truncates the 64-bit range (u64/s64) and the upper half of u32. Scalar for now (matches the scalar
// tail these functions already fall back to); a proper f64->s64/u64 intrinsic can replace it later.
ساکن اندرونی خالی v_store_pair_as(uint64_t* ptr, ثابت v_float64& a, ثابت v_float64& b)
{
    ثابت صحیح n = VTraits<v_float64>::vlanes();
    دوہرا buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    برائے (صحیح i = 0; i < 2*n; i++) ptr[i] = saturate_cast<uint64_t>(buf[i]);
}

ساکن اندرونی خالی v_store_pair_as(int64_t* ptr, ثابت v_float64& a, ثابت v_float64& b)
{
    ثابت صحیح n = VTraits<v_float64>::vlanes();
    دوہرا buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    برائے (صحیح i = 0; i < 2*n; i++) ptr[i] = saturate_cast<int64_t>(buf[i]);
}

ساکن اندرونی خالی v_store_pair_as(غیرعلامتی* ptr, ثابت v_float64& a, ثابت v_float64& b)
{
    ثابت صحیح n = VTraits<v_float64>::vlanes();
    دوہرا buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    برائے (صحیح i = 0; i < 2*n; i++) ptr[i] = saturate_cast<غیرعلامتی>(buf[i]);
}

#ورنہ

ساکن اندرونی خالی vx_بھرو_as(ثابت دوہرا* ptr, v_float32& a)
{
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    اعشاری buf[VTraits<v_float32>::max_nlanes*2];

    برائے( صحیح i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<اعشاری>(ptr[i]);
    a = vx_بھرو(buf);
}

ساکن اندرونی خالی vx_بھرو_as(ثابت uint64_t* ptr, v_float32& a)
{
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    اعشاری buf[VTraits<v_float32>::max_nlanes*2];

    برائے( صحیح i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<اعشاری>(ptr[i]);
    a = vx_بھرو(buf);
}

ساکن اندرونی خالی vx_بھرو_as(ثابت int64_t* ptr, v_float32& a)
{
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    اعشاری buf[VTraits<v_float32>::max_nlanes*2];

    برائے( صحیح i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<اعشاری>(ptr[i]);
    a = vx_بھرو(buf);
}

سانچہ<قسم_نام _Tdvec>
ساکن اندرونی خالی vx_بھرو_pair_as(ثابت دوہرا* ptr, _Tdvec& a, _Tdvec& b)
{
    ثابت صحیح VECSZ = VTraits<_Tdvec>::vlanes();
    قسم_نام VTraits<_Tdvec>::lane_type buf[VTraits<_Tdvec>::max_nlanes*2];

    برائے( صحیح i = 0; i < VECSZ*2; i++ )
        buf[i] = saturate_cast<قسم_نام VTraits<_Tdvec>::lane_type>(ptr[i]);
    a = vx_بھرو(buf);
    b = vx_بھرو(buf + VECSZ);
}

ساکن اندرونی خالی v_store_as(دوہرا* ptr, ثابت v_float32& a)
{
    ثابت صحیح VECSZ = VTraits<v_float32>::vlanes();
    اعشاری buf[VTraits<v_float32>::max_nlanes];

    v_store(buf, a);
    برائے( صحیح i = 0; i < VECSZ; i++ )
        ptr[i] = (دوہرا)buf[i];
}

سانچہ<قسم_نام _Tsvec>
ساکن اندرونی خالی v_store_pair_as(دوہرا* ptr, ثابت _Tsvec& a, ثابت _Tsvec& b)
{
    ثابت صحیح VECSZ = VTraits<_Tsvec>::vlanes();
    قسم_نام VTraits<_Tsvec>::lane_type buf[VTraits<_Tsvec>::max_nlanes*2];

    v_store(buf, a); v_store(buf + VECSZ, b);
    برائے( صحیح i = 0; i < VECSZ*2; i++ )
        ptr[i] = (دوہرا)buf[i];
}

#اگر_ختم /////////// CV_SIMD_64F || CV_SIMD_SCALABLE_64F

#اگر_ختم /////////// CV_SIMD || CV_SIMD_SCALABLE

}

#اگر_ختم // SRC_CONVERT_HPP
