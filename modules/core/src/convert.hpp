// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#यदि_अपरिभाषितम् SRC_CONVERT_HPP
#परिभाषय SRC_CONVERT_HPP

#अन्तर्भाव "opencv2/core/types.hpp"
#अन्तर्भाव "opencv2/core/hal/intrin.hpp"

नामाकाश cv
{

#यदि (CV_SIMD || CV_SIMD_SCALABLE)

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव uchar* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_आरोपय_expand_q(ptr))); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव schar* ptr, v_float32& a)
{ a = v_cvt_f32(vx_आरोपय_expand_q(ptr)); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव ushort* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_आरोपय_expand(ptr))); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव ह्रस्व* ptr, v_float32& a)
{ a = v_cvt_f32(v_reinterpret_as_s32(vx_आरोपय_expand(ptr))); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव पूर्णाङ्क* ptr, v_float32& a)
{ a = v_cvt_f32(vx_आरोपय(ptr)); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव अचिह्नित* ptr, v_float32& a)
{
    v_uint32 delta = vx_setall_u32(0x80000000U);
    v_uint32 ua = vx_आरोपय(ptr);
    v_uint32 आवरणम्_a = v_and(v_ge(ua, delta), delta);
    v_float32 fmask_a = v_cvt_f32(v_reinterpret_as_s32(आवरणम्_a)); // 0.f or (float)(-(1 << 31))
    a = v_cvt_f32(v_reinterpret_as_s32(v_sub(ua, आवरणम्_a)));
    // restore the original values
    a = v_sub(a, fmask_a); // subtract 0 or a large negative number
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव भिन्न* ptr, v_float32& a)
{ a = vx_आरोपय(ptr); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव hfloat* ptr, v_float32& a)
{ a = vx_आरोपय_expand(ptr); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव bfloat* ptr, v_float32& a)
{ a = vx_आरोपय_expand(ptr); }

स्थिर अन्तःस्थ शून्य v_store_as(ushort* ptr, ध्रुव v_float32& a)
{ v_pack_u_store(ptr, v_आवर्तनम्(a)); }

स्थिर अन्तःस्थ शून्य v_store_as(ह्रस्व* ptr, ध्रुव v_float32& a)
{ v_pack_store(ptr, v_आवर्तनम्(a)); }

स्थिर अन्तःस्थ शून्य v_store_as(पूर्णाङ्क* ptr, ध्रुव v_float32& a)
{ v_store(ptr, v_आवर्तनम्(a)); }

स्थिर अन्तःस्थ शून्य v_store_as(अचिह्नित* ptr, ध्रुव v_float32& a)
{
    v_float32 z = vx_setzero_f32();
    v_store(ptr, v_reinterpret_as_u32(v_आवर्तनम्(v_max(a, z))));
}

स्थिर अन्तःस्थ शून्य v_store_as(भिन्न* ptr, ध्रुव v_float32& a)
{ v_store(ptr, a); }

स्थिर अन्तःस्थ शून्य v_store_as(hfloat* ptr, ध्रुव v_float32& a)
{ v_pack_store(ptr, a); }

स्थिर अन्तःस्थ शून्य v_store_as(bfloat* ptr, ध्रुव v_float32& a)
{ v_pack_store(ptr, a); }

स्थिर अन्तःस्थ शून्य v_store_as(int64_t* ptr, ध्रुव v_float32& a)
{
    v_int32 ia = v_आवर्तनम्(a);
    v_int64 ia_0, ia_1;
    v_expand(ia, ia_0, ia_1);
    v_store(ptr, ia_0);
    v_store(ptr + VTraits<v_uint64>::vlanes(), ia_1);
}

स्थिर अन्तःस्थ शून्य v_store_as(uint64_t* ptr, ध्रुव v_float32& a)
{
    v_int32 ia = v_आवर्तनम्(a);
    v_uint64 ia_0, ia_1;
    ia = v_max(ia, vx_setzero_s32());
    v_expand(v_reinterpret_as_u32(ia), ia_0, ia_1);
    v_store(ptr, ia_0);
    v_store(ptr + VTraits<v_uint64>::vlanes(), ia_1);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uchar* ptr, v_uint16& a, v_uint16& b)
{ v_expand(vx_आरोपय(ptr), a, b); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव schar* ptr, v_uint16& a, v_uint16& b)
{
    ध्रुव v_int8 z = vx_setzero_s8();
    v_int16 sa, sb;
    v_expand(v_max(vx_आरोपय(ptr), z), sa, sb);
    a = v_reinterpret_as_u16(sa);
    b = v_reinterpret_as_u16(sb);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ushort* ptr, v_uint16& a, v_uint16& b)
{ a = vx_आरोपय(ptr); b = vx_आरोपय(ptr + VTraits<v_uint16>::vlanes()); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uchar* ptr, v_int16& a, v_int16& b)
{
    v_uint16 ua, ub;
    v_expand(vx_आरोपय(ptr), ua, ub);
    a = v_reinterpret_as_s16(ua);
    b = v_reinterpret_as_s16(ub);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव schar* ptr, v_int16& a, v_int16& b)
{ v_expand(vx_आरोपय(ptr), a, b); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ह्रस्व* ptr, v_int16& a, v_int16& b)
{ a = vx_आरोपय(ptr); b = vx_आरोपय(ptr + VTraits<v_uint16>::vlanes()); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uchar* ptr, v_int32& a, v_int32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_आरोपय_expand(ptr), ua, ub);
    a = v_reinterpret_as_s32(ua);
    b = v_reinterpret_as_s32(ub);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव schar* ptr, v_int32& a, v_int32& b)
{ v_expand(vx_आरोपय_expand(ptr), a, b); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ushort* ptr, v_int32& a, v_int32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_आरोपय(ptr), ua, ub);
    a = v_reinterpret_as_s32(ua);
    b = v_reinterpret_as_s32(ub);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ushort* ptr, v_uint32& a, v_uint32& b)
{
    v_expand(vx_आरोपय(ptr), a, b);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ह्रस्व* ptr, v_int32& a, v_int32& b)
{
    v_expand(vx_आरोपय(ptr), a, b);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव पूर्णाङ्क* ptr, v_int32& a, v_int32& b)
{
    a = vx_आरोपय(ptr);
    b = vx_आरोपय(ptr + VTraits<v_int32>::vlanes());
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uchar* ptr, v_float32& a, v_float32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_आरोपय_expand(ptr), ua, ub);
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
    b = v_cvt_f32(v_reinterpret_as_s32(ub));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव schar* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia, ib;
    v_expand(vx_आरोपय_expand(ptr), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ushort* ptr, v_float32& a, v_float32& b)
{
    v_uint32 ua, ub;
    v_expand(vx_आरोपय(ptr), ua, ub);
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
    b = v_cvt_f32(v_reinterpret_as_s32(ub));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ह्रस्व* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia, ib;
    v_expand(vx_आरोपय(ptr), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव पूर्णाङ्क* ptr, v_float32& a, v_float32& b)
{
    v_int32 ia = vx_आरोपय(ptr), ib = vx_आरोपय(ptr + VTraits<v_int32>::vlanes());
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

// v_pack()/v_pack_u() saturate at every width, 64->32 included, so a 64-bit source needs no
// clamping here any more - see the HAL headers for the per-backend implementations.

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव int64_t* ptr, v_int32& a, v_int32& b)
{
    ध्रुव पूर्णाङ्क int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack(vx_आरोपय(ptr), vx_आरोपय(ptr + int64_nlanes));
    b = v_pack(vx_आरोपय(ptr + int64_nlanes*2), vx_आरोपय(ptr + int64_nlanes*3));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव int64_t* ptr, v_uint64& a, v_uint64& b)
{
    // clamp the negatives to zero through the sign bit: v_gt on 64-bit lanes is sign(b - a) at the
    // SSE and NEON baselines, which overflows for an operand at the bottom of the range
    v_int64 ia = vx_आरोपय(ptr), ib = vx_आरोपय(ptr + VTraits<v_uint64>::vlanes());
    ia = v_and(ia, v_not(v_shr<63>(ia)));
    ib = v_and(ib, v_not(v_shr<63>(ib)));
    a = v_reinterpret_as_u64(ia);
    b = v_reinterpret_as_u64(ib);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव int64_t* ptr, v_uint32& a, v_uint32& b)
{
    ध्रुव पूर्णाङ्क nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack_u(vx_आरोपय(ptr), vx_आरोपय(ptr + nlanes));
    b = v_pack_u(vx_आरोपय(ptr + nlanes*2), vx_आरोपय(ptr + nlanes*3));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uint64_t* ptr, v_float32& a, v_float32& b)
{
    ध्रुव पूर्णाङ्क nlanes = VTraits<v_uint64>::vlanes();
    भिन्न buf[VTraits<v_uint64>::max_nlanes*4];
    प्रति (पूर्णाङ्क i = 0; i < nlanes*4; i++) {
        buf[i] = (भिन्न)ptr[i];
    }
    a = vx_आरोपय(buf);
    b = vx_आरोपय(buf + nlanes*2);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव int64_t* ptr, v_float32& a, v_float32& b)
{
    ध्रुव पूर्णाङ्क nlanes = VTraits<v_uint64>::vlanes();
    भिन्न buf[VTraits<v_uint64>::max_nlanes*4];
    प्रति (पूर्णाङ्क i = 0; i < nlanes*4; i++) {
        buf[i] = (भिन्न)ptr[i];
    }
    a = vx_आरोपय(buf);
    b = vx_आरोपय(buf + nlanes*2);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव तार्किक* ptr, v_float32& a, v_float32& b)
{
    v_uint16 z = vx_setzero_u16();
    v_uint16 uab = vx_आरोपय_expand((ध्रुव uchar*)ptr);
    uab = v_shr<15>(v_gt(uab, z));
    v_int32 ia, ib;
    v_expand(v_reinterpret_as_s16(uab), ia, ib);
    a = v_cvt_f32(ia);
    b = v_cvt_f32(ib);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव तार्किक* ptr, v_float32& a)
{
    v_uint32 z = vx_setzero_u32();
    v_uint32 ua = vx_आरोपय_expand_q((ध्रुव uchar*)ptr);
    ua = v_shr<31>(v_gt(ua, z));
    a = v_cvt_f32(v_reinterpret_as_s32(ua));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव schar* ptr, v_uint32& a, v_uint32& b)
{
    v_int16 ab = v_max(vx_आरोपय_expand(ptr), vx_setzero_s16());
    v_expand(v_reinterpret_as_u16(ab), a, b);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ह्रस्व* ptr, v_uint32& a, v_uint32& b)
{
    v_int16 ab = v_max(vx_आरोपय(ptr), vx_setzero_s16());
    v_expand(v_reinterpret_as_u16(ab), a, b);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव पूर्णाङ्क* ptr, v_uint32& a, v_uint32& b)
{
    v_int32 z = vx_setzero_s32();
    v_int32 ia = v_max(vx_आरोपय(ptr), z);
    v_int32 ib = v_max(vx_आरोपय(ptr + VTraits<v_int32>::vlanes()), z);
    a = v_reinterpret_as_u32(ia);
    b = v_reinterpret_as_u32(ib);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uint64_t* ptr, v_uint32& a, v_uint32& b)
{
    ध्रुव पूर्णाङ्क int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_pack(vx_आरोपय(ptr), vx_आरोपय(ptr + int64_nlanes));
    b = v_pack(vx_आरोपय(ptr + int64_nlanes*2), vx_आरोपय(ptr + int64_nlanes*3));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uint64_t* ptr, v_int32& a, v_int32& b)
{
    // There is no uint64 -> int32 narrow in the HAL, so narrow as unsigned first - v_pack() clamps
    // anything above UINT32_MAX to UINT32_MAX - and bring that down to INT32_MAX with the same
    // 32-bit v_min() the uint32 -> int32 direction below uses.
    ध्रुव पूर्णाङ्क int64_nlanes = VTraits<v_uint64>::vlanes();
    ध्रुव v_uint32 hi = vx_setall_u32((अचिह्नित)अधिकतम_पूर्णाङ्कः);
    a = v_reinterpret_as_s32(v_min(v_pack(vx_आरोपय(ptr), vx_आरोपय(ptr + int64_nlanes)), hi));
    b = v_reinterpret_as_s32(v_min(v_pack(vx_आरोपय(ptr + int64_nlanes*2),
                                          vx_आरोपय(ptr + int64_nlanes*3)), hi));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव भिन्न* ptr, v_float32& a, v_float32& b)
{ a = vx_आरोपय(ptr); b = vx_आरोपय(ptr + VTraits<v_float32>::vlanes()); }

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव hfloat* ptr, v_float32& a, v_float32& b)
{
    a = vx_आरोपय_expand(ptr);
    b = vx_आरोपय_expand(ptr + VTraits<v_float32>::vlanes());
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव bfloat* ptr, v_float32& a, v_float32& b)
{
    a = vx_आरोपय_expand(ptr);
    b = vx_आरोपय_expand(ptr + VTraits<v_float32>::vlanes());
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव अचिह्नित* ptr, v_uint32& a, v_uint32& b)
{
    a = vx_आरोपय(ptr);
    b = vx_आरोपय(ptr + VTraits<v_uint32>::vlanes());
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव अचिह्नित* ptr, v_int32& a, v_int32& b)
{
    // Reinterpreting alone would turn everything above INT32_MAX negative, and the saturating
    // narrow that follows would then clamp it to the WRONG end of the destination's range
    // (4294967295 -> -1 -> -128 for schar, rather than 127). Clamp first, like the int -> unsigned
    // direction above does for negatives.
    ध्रुव v_uint32 hi = vx_setall_u32((अचिह्नित)अधिकतम_पूर्णाङ्कः);
    a = v_reinterpret_as_s32(v_min(vx_आरोपय(ptr), hi));
    b = v_reinterpret_as_s32(v_min(vx_आरोपय(ptr + VTraits<v_uint32>::vlanes()), hi));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव अचिह्नित* ptr, v_float32& a, v_float32& b)
{
    v_uint32 delta = vx_setall_u32(0x80000000U);
    v_uint32 ua = vx_आरोपय(ptr);
    v_uint32 ub = vx_आरोपय(ptr + VTraits<v_uint32>::vlanes());
    v_uint32 आवरणम्_a = v_and(v_ge(ua, delta), delta), आवरणम्_b = v_and(v_ge(ub, delta), delta);
    v_float32 fmask_a = v_cvt_f32(v_reinterpret_as_s32(आवरणम्_a)); // 0.f or (float)(-(1 << 31))
    v_float32 fmask_b = v_cvt_f32(v_reinterpret_as_s32(आवरणम्_b)); // 0.f or (float)(-(1 << 31))
    a = v_cvt_f32(v_reinterpret_as_s32(v_sub(ua, आवरणम्_a)));
    b = v_cvt_f32(v_reinterpret_as_s32(v_sub(ub, आवरणम्_b)));
    // restore the original values
    a = v_sub(a, fmask_a); // subtract 0 or a large negative number
    b = v_sub(b, fmask_b); // subtract 0 or a large negative number
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(uchar* ptr, ध्रुव v_uint16& a, ध्रुव v_uint16& b)
{
    v_store(ptr, v_pack(a, b));
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(schar* ptr, ध्रुव v_uint16& a, ध्रुव v_uint16& b)
{
    ध्रुव v_uint8 maxval = vx_setall_u8((uchar)std::numeric_limits<schar>::max());
    v_uint8 v = v_pack(a, b);
    v_store(ptr, v_reinterpret_as_s8(v_min(v, maxval)));
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(ushort* ptr, ध्रुव v_uint16& a, ध्रुव v_uint16& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_uint16>::vlanes(), b); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(uchar* ptr, ध्रुव v_int16& a, ध्रुव v_int16& b)
{ v_store(ptr, v_pack_u(a, b)); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(schar* ptr, ध्रुव v_int16& a, ध्रुव v_int16& b)
{ v_store(ptr, v_pack(a, b)); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(ह्रस्व* ptr, ध्रुव v_int16& a, ध्रुव v_int16& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_int16>::vlanes(), b); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(uchar* ptr, ध्रुव v_int32& a, ध्रुव v_int32& b)
{ v_pack_u_store(ptr, v_pack(a, b)); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(schar* ptr, ध्रुव v_int32& a, ध्रुव v_int32& b)
{ v_pack_store(ptr, v_pack(a, b)); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(ushort* ptr, ध्रुव v_int32& a, ध्रुव v_int32& b)
{ v_store(ptr, v_pack_u(a, b)); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(ह्रस्व* ptr, ध्रुव v_int32& a, ध्रुव v_int32& b)
{ v_store(ptr, v_pack(a, b)); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(पूर्णाङ्क* ptr, ध्रुव v_int32& a, ध्रुव v_int32& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_int32>::vlanes(), b);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(int64_t* ptr, ध्रुव v_int32& a, ध्रुव v_int32& b)
{
    v_int64 q0, q1, q2, q3;
    v_expand(a, q0, q1);
    v_expand(b, q2, q3);
    ध्रुव पूर्णाङ्क nlanes = VTraits<v_uint64>::vlanes();
    v_store(ptr, q0);
    v_store(ptr + nlanes, q1);
    v_store(ptr + nlanes*2, q2);
    v_store(ptr + nlanes*3, q3);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(uchar* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{ v_pack_u_store(ptr, v_pack(v_आवर्तनम्(a), v_आवर्तनम्(b))); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(schar* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{ v_pack_store(ptr, v_pack(v_आवर्तनम्(a), v_आवर्तनम्(b))); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(तार्किक* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{
    v_float32 z = vx_setzero_f32();
    v_uint32 ma = v_shr<31>(v_reinterpret_as_u32(v_ne(a, z)));
    v_uint32 mb = v_shr<31>(v_reinterpret_as_u32(v_ne(b, z)));
    v_uint16 mab = v_pack(ma, mb);
    v_pack_store((uchar*)ptr, mab);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(ushort* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{ v_store(ptr, v_pack_u(v_आवर्तनम्(a), v_आवर्तनम्(b))); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(ह्रस्व* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{ v_store(ptr, v_pack(v_आवर्तनम्(a), v_आवर्तनम्(b))); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(पूर्णाङ्क* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{
    v_int32 ia = v_आवर्तनम्(a), ib = v_आवर्तनम्(b);
    v_store(ptr, ia);
    v_store(ptr + VTraits<v_int32>::vlanes(), ib);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(भिन्न* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{ v_store(ptr, a); v_store(ptr + VTraits<v_float32>::vlanes(), b); }

स्थिर अन्तःस्थ शून्य v_store_pair_as(अचिह्नित* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{
    // v_round(f32) narrows to v_int32, so values in [2^31, 2^32) saturate to INT32_MAX. Scalar for now
    // (same class as the f64->{u64,s64,u32} stores above); a proper f32->u32 intrinsic can replace it.
    ध्रुव पूर्णाङ्क n = VTraits<v_float32>::vlanes();
    भिन्न buf[VTraits<v_float32>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    प्रति (पूर्णाङ्क i = 0; i < 2*n; i++) ptr[i] = saturate_cast<अचिह्नित>(buf[i]);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(uchar* ptr, ध्रुव v_uint32& a, ध्रुव v_uint32& b)
{
    v_pack_store(ptr, v_pack(a, b));
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(ushort* ptr, ध्रुव v_uint32& a, ध्रुव v_uint32& b)
{
    v_store(ptr, v_pack(a, b));
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(अचिह्नित* ptr, ध्रुव v_uint32& a, ध्रुव v_uint32& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_uint32>::vlanes(), b);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(uint64_t* ptr, ध्रुव v_uint32& a, ध्रुव v_uint32& b)
{
    v_uint64 q0, q1, q2, q3;
    v_expand(a, q0, q1);
    v_expand(b, q2, q3);
    ध्रुव पूर्णाङ्क nlanes = VTraits<v_uint64>::vlanes();
    v_store(ptr, q0);
    v_store(ptr + nlanes, q1);
    v_store(ptr + nlanes*2, q2);
    v_store(ptr + nlanes*3, q3);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(uint64_t* ptr, ध्रुव v_uint64& a, ध्रुव v_uint64& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_uint64>::vlanes(), b);
}

// float32 -> FP8 encode, shared by both scale paths. Fallback: no portable per-lane variable shift.

// fallback test plus the pieces encodeFp8Finish needs, to skip costly math when unused
प्रतिरूपम्<पूर्णाङ्क bias> स्थिर अन्तःस्थ शून्य
fp8Prepare(ध्रुव v_float32& vf, v_uint32& पूर्णम्, v_uint32& sbit, v_int32& newexpRaw, v_int32& fallback_आवरणम्)
{
    v_uint32 u = v_reinterpret_as_u32(vf);
    v_uint32 e = v_and(v_shr<23>(u), vx_setall_u32(0xFFu));
    v_uint32 m = v_and(u, vx_setall_u32(0x7FFFFFu));
    sbit = v_and(v_shr<24>(u), vx_setall_u32(0x80u));
    पूर्णम् = v_or(m, vx_setall_u32(1u << 23));
    newexpRaw = v_sub(v_reinterpret_as_s32(e), vx_setall_s32(127 - bias));
    v_int32 isNan = v_reinterpret_as_s32(v_eq(e, vx_setall_u32(0xFFu)));
    fallback_आवरणम् = v_or(isNan, v_le(newexpRaw, vx_setall_s32(0)));
}

// Valid only when fp8Prepare's fallbackMask is false; must stay bit-identical to encodeE4M3.
प्रतिरूपम्<पूर्णाङ्क bias, तार्किक fnuz> स्थिर अन्तःस्थ v_int32
encodeFp8Finish(v_uint32 पूर्णम्, v_uint32 sbit, v_int32 newexpRaw)
{
    v_uint32 one     = vx_setall_u32(1u);
    v_uint32 half    = vx_setall_u32(1u << 19);
    v_uint32 q       = v_shr<20>(पूर्णम्);
    v_uint32 rem     = v_and(पूर्णम्, vx_setall_u32((1u << 20) - 1));
    v_uint32 tie     = v_and(v_eq(rem, half), v_ne(v_and(q, one), vx_setzero_u32()));
    v_uint32 inc     = v_and(v_or(v_gt(rem, half), tie), one);
    v_uint32 rounded = v_सङ्कलय(q, inc);

    v_uint32 carry = v_ne(v_and(rounded, vx_setall_u32(16u)), vx_setall_u32(0u));
    rounded = v_select(carry, v_shr<1>(rounded), rounded);
    v_int32 newexp = v_सङ्कलय(newexpRaw, v_and(v_reinterpret_as_s32(carry), vx_setall_s32(1)));

    v_uint32 mant = v_and(rounded, vx_setall_u32(7u));

    v_int32 gt15 = v_gt(newexp, vx_setall_s32(15));
    v_int32 overflow;
    यदि constexpr (fnuz)
        overflow = gt15;
    अन्यथा
        overflow = v_or(gt15, v_and(v_eq(newexp, vx_setall_s32(15)),
                                     v_eq(v_reinterpret_as_s32(mant), vx_setall_s32(7))));

    v_uint32 maxFin = v_or(sbit, vx_setall_u32(fnuz ? 0x7Fu : 0x7Eu));

    v_uint32 प्रसामान्यम् = v_or(v_or(sbit, v_shl<3>(v_reinterpret_as_u32(newexp))), mant);
    v_uint32 परिणाम = v_select(v_reinterpret_as_u32(overflow), maxFin, प्रसामान्यम्);
    प्रत्यागम v_reinterpret_as_s32(परिणाम);
}

// encodes vf0..vf3 to FP8 bytes; scalarSrc must hold the same values, contiguous, for fallback
प्रतिरूपम्<पूर्णाङ्क bias, तार्किक fnuz> स्थिर अन्तःस्थ शून्य
encodeFp8Vec4(ध्रुव v_float32& vf0, ध्रुव v_float32& vf1, ध्रुव v_float32& vf2, ध्रुव v_float32& vf3,
              ध्रुव भिन्न* scalarSrc, पूर्णाङ्क vecsz, uchar* dst)
{
    v_uint32 full0, full1, full2, full3, sbit0, sbit1, sbit2, sbit3;
    v_int32 newexpRaw0, newexpRaw1, newexpRaw2, newexpRaw3;
    v_int32 pre0, pre1, pre2, pre3;
    fp8Prepare<bias>(vf0, full0, sbit0, newexpRaw0, pre0);
    fp8Prepare<bias>(vf1, full1, sbit1, newexpRaw1, pre1);
    fp8Prepare<bias>(vf2, full2, sbit2, newexpRaw2, pre2);
    fp8Prepare<bias>(vf3, full3, sbit3, newexpRaw3, pre3);

    यदि (v_check_all(v_and(v_and(pre0, pre1), v_and(pre2, pre3))))
    {
        // whole group is subnormal/zero/NaN — skip the costlier fast-path math
        प्रति (पूर्णाङ्क k = 0; k < vecsz; k++)
            dst[k] = fp8_detail::encodeE4M3(scalarSrc[k], bias, fnuz);
        प्रत्यागम;
    }

    v_int32 byte0 = encodeFp8Finish<bias, fnuz>(full0, sbit0, newexpRaw0);
    v_int32 byte1 = encodeFp8Finish<bias, fnuz>(full1, sbit1, newexpRaw1);
    v_int32 byte2 = encodeFp8Finish<bias, fnuz>(full2, sbit2, newexpRaw2);
    v_int32 byte3 = encodeFp8Finish<bias, fnuz>(full3, sbit3, newexpRaw3);
    v_store(dst, v_pack_u(v_pack(byte0, byte1), v_pack(byte2, byte3)));

    v_int32 one = vx_setall_s32(1);
    uchar fallback[VTraits<v_uint8>::max_nlanes];
    v_store(fallback, v_pack_u(v_pack(v_and(pre0, one), v_and(pre1, one)),
                                v_pack(v_and(pre2, one), v_and(pre3, one))));
    प्रति (पूर्णाङ्क k = 0; k < vecsz; k++)
        यदि (fallback[k])
            dst[k] = fp8_detail::encodeE4M3(scalarSrc[k], bias, fnuz);
}

#यदि (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)

// v_cvt_f64() converts SIGNED 64-bit lanes, so it cannot be used on a uint64 directly. Split into
// two halves below 2^32 instead: each converts exactly, scaling the high half by 2^32 is exact, and
// the single add at the end is the only rounding - so the result matches (double)x.
//
// Converting the bit pattern as signed and adding 2^64 back would be simpler, but it rounds twice:
// for 2^63 + 1025 it gives 2^63 where (double)x is 2^63 + 2048.
स्थिर अन्तःस्थ v_float64 v_cvt_f64_u64(ध्रुव v_uint64& x)
{
    ध्रुव v_int64 hi = v_reinterpret_as_s64(v_shr<32>(x));
    ध्रुव v_int64 lo = v_reinterpret_as_s64(v_and(x, vx_setall_u64(0xFFFFFFFFull)));
    प्रत्यागम v_सङ्कलय(v_mul(v_cvt_f64(hi), vx_setall_f64(4294967296.0)), v_cvt_f64(lo));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव uint64_t* ptr, v_float32& a)
{
    v_float64 a_0 = v_cvt_f64_u64(vx_आरोपय(ptr));
    v_float64 a_1 = v_cvt_f64_u64(vx_आरोपय(ptr + VTraits<v_uint64>::vlanes()));
    a = v_cvt_f32(a_0, a_1);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव int64_t* ptr, v_float32& a)
{
    v_float64 a_0 = v_cvt_f64(vx_आरोपय(ptr));
    v_float64 a_1 = v_cvt_f64(vx_आरोपय(ptr + VTraits<v_uint64>::vlanes()));
    a = v_cvt_f32(a_0, a_1);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव द्विगुण* ptr, v_float32& a)
{
    v_float64 v0 = vx_आरोपय(ptr), v1 = vx_आरोपय(ptr + VTraits<v_float64>::vlanes());
    a = v_cvt_f32(v0, v1);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव तार्किक* ptr, v_float64& a, v_float64& b)
{
    v_uint32 z = vx_setzero_u32();
    v_uint32 uab = vx_आरोपय_expand_q((ध्रुव uchar*)ptr);
    uab = v_shr<31>(v_gt(uab, z));
    v_float32 fab = v_cvt_f32(v_reinterpret_as_s32(uab));
    a = v_cvt_f64(fab);
    b = v_cvt_f64_उच्चः(fab);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव hfloat* ptr, v_float64& a, v_float64& b)
{
    v_float32 fab = vx_आरोपय_expand(ptr);
    a = v_cvt_f64(fab);
    b = v_cvt_f64_उच्चः(fab);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव bfloat* ptr, v_float64& a, v_float64& b)
{
    v_float32 fab = vx_आरोपय_expand(ptr);
    a = v_cvt_f64(fab);
    b = v_cvt_f64_उच्चः(fab);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव द्विगुण* ptr, v_int32& a, v_int32& b)
{
    v_float64 v0 = vx_आरोपय(ptr), v1 = vx_आरोपय(ptr + VTraits<v_float64>::vlanes());
    v_float64 v2 = vx_आरोपय(ptr + VTraits<v_float64>::vlanes()*2), v3 = vx_आरोपय(ptr + VTraits<v_float64>::vlanes()*3);
    v_int32 iv0 = v_आवर्तनम्(v0), iv1 = v_आवर्तनम्(v1);
    v_int32 iv2 = v_आवर्तनम्(v2), iv3 = v_आवर्तनम्(v3);
    a = v_combine_नीचः(iv0, iv1);
    b = v_combine_नीचः(iv2, iv3);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uint64_t* ptr, v_float64& a, v_float64& b)
{
    ध्रुव पूर्णाङ्क int64_nlanes = VTraits<v_uint64>::vlanes();
    a = v_cvt_f64_u64(vx_आरोपय(ptr));
    b = v_cvt_f64_u64(vx_आरोपय(ptr + int64_nlanes));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव द्विगुण* ptr, v_float32& a, v_float32& b)
{
    v_float64 v0 = vx_आरोपय(ptr), v1 = vx_आरोपय(ptr + VTraits<v_float64>::vlanes());
    v_float64 v2 = vx_आरोपय(ptr + VTraits<v_float64>::vlanes()*2), v3 = vx_आरोपय(ptr + VTraits<v_float64>::vlanes()*3);
    a = v_cvt_f32(v0, v1);
    b = v_cvt_f32(v2, v3);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव uchar* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = v_reinterpret_as_s32(vx_आरोपय_expand_q(ptr));
    a = v_cvt_f64(v0);
    b = v_cvt_f64_उच्चः(v0);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव schar* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_आरोपय_expand_q(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_उच्चः(v0);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ushort* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = v_reinterpret_as_s32(vx_आरोपय_expand(ptr));
    a = v_cvt_f64(v0);
    b = v_cvt_f64_उच्चः(v0);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव ह्रस्व* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_आरोपय_expand(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_उच्चः(v0);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव पूर्णाङ्क* ptr, v_float64& a, v_float64& b)
{
    v_int32 v0 = vx_आरोपय(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_उच्चः(v0);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव भिन्न* ptr, v_float64& a, v_float64& b)
{
    v_float32 v0 = vx_आरोपय(ptr);
    a = v_cvt_f64(v0);
    b = v_cvt_f64_उच्चः(v0);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव द्विगुण* ptr, v_float64& a, v_float64& b)
{
    a = vx_आरोपय(ptr);
    b = vx_आरोपय(ptr + VTraits<v_float64>::vlanes());
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव int64_t* ptr, v_float64& a, v_float64& b)
{
    a = v_cvt_f64(vx_आरोपय(ptr));
    b = v_cvt_f64(vx_आरोपय(ptr + VTraits<v_float64>::vlanes()));
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव अचिह्नित* ptr, v_float64& a, v_float64& b)
{
    ध्रुव पूर्णाङ्क nlanes = VTraits<v_uint64>::vlanes();
    द्विगुण buf[VTraits<v_uint64>::max_nlanes*2];
    प्रति (पूर्णाङ्क i = 0; i < nlanes*2; i++)
        buf[i] = (द्विगुण)ptr[i];
    a = vx_आरोपय(buf);
    b = vx_आरोपय(buf + nlanes);
}

स्थिर अन्तःस्थ शून्य v_store_as(द्विगुण* ptr, ध्रुव v_float32& a)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_उच्चः(a);
    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(द्विगुण* ptr, ध्रुव v_int32& a, ध्रुव v_int32& b)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_उच्चः(a);
    v_float64 fb0 = v_cvt_f64(b), fb1 = v_cvt_f64_उच्चः(b);

    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
    v_store(ptr + VTraits<v_float64>::vlanes()*2, fb0);
    v_store(ptr + VTraits<v_float64>::vlanes()*3, fb1);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(द्विगुण* ptr, ध्रुव v_float32& a, ध्रुव v_float32& b)
{
    v_float64 fa0 = v_cvt_f64(a), fa1 = v_cvt_f64_उच्चः(a);
    v_float64 fb0 = v_cvt_f64(b), fb1 = v_cvt_f64_उच्चः(b);

    v_store(ptr, fa0);
    v_store(ptr + VTraits<v_float64>::vlanes(), fa1);
    v_store(ptr + VTraits<v_float64>::vlanes()*2, fb0);
    v_store(ptr + VTraits<v_float64>::vlanes()*3, fb1);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(द्विगुण* ptr, ध्रुव v_float64& a, ध्रुव v_float64& b)
{
    v_store(ptr, a);
    v_store(ptr + VTraits<v_float64>::vlanes(), b);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(पूर्णाङ्क* ptr, ध्रुव v_float64& a, ध्रुव v_float64& b)
{
    v_int32 ia = v_आवर्तनम्(a), ib = v_आवर्तनम्(b);
    v_store(ptr, v_combine_नीचः(ia, ib));
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(भिन्न* ptr, ध्रुव v_float64& a, ध्रुव v_float64& b)
{
    v_float32 v = v_cvt_f32(a, b);
    v_store(ptr, v);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(hfloat* ptr, ध्रुव v_float64& a, ध्रुव v_float64& b)
{
    v_float32 v = v_cvt_f32(a, b);
    v_pack_store(ptr, v);
}

// f64 -> {u64, s64, u32}: no correct vector path yet - v_round(f64,f64) narrows to v_int32, which
// truncates the 64-bit range (u64/s64) and the upper half of u32. Scalar for now (matches the scalar
// tail these functions already fall back to); a proper f64->s64/u64 intrinsic can replace it later.
स्थिर अन्तःस्थ शून्य v_store_pair_as(uint64_t* ptr, ध्रुव v_float64& a, ध्रुव v_float64& b)
{
    ध्रुव पूर्णाङ्क n = VTraits<v_float64>::vlanes();
    द्विगुण buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    प्रति (पूर्णाङ्क i = 0; i < 2*n; i++) ptr[i] = saturate_cast<uint64_t>(buf[i]);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(int64_t* ptr, ध्रुव v_float64& a, ध्रुव v_float64& b)
{
    ध्रुव पूर्णाङ्क n = VTraits<v_float64>::vlanes();
    द्विगुण buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    प्रति (पूर्णाङ्क i = 0; i < 2*n; i++) ptr[i] = saturate_cast<int64_t>(buf[i]);
}

स्थिर अन्तःस्थ शून्य v_store_pair_as(अचिह्नित* ptr, ध्रुव v_float64& a, ध्रुव v_float64& b)
{
    ध्रुव पूर्णाङ्क n = VTraits<v_float64>::vlanes();
    द्विगुण buf[VTraits<v_float64>::max_nlanes*2];
    v_store(buf, a); v_store(buf + n, b);
    प्रति (पूर्णाङ्क i = 0; i < 2*n; i++) ptr[i] = saturate_cast<अचिह्नित>(buf[i]);
}

#अन्यथा

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव द्विगुण* ptr, v_float32& a)
{
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
    भिन्न buf[VTraits<v_float32>::max_nlanes*2];

    प्रति( पूर्णाङ्क i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<भिन्न>(ptr[i]);
    a = vx_आरोपय(buf);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव uint64_t* ptr, v_float32& a)
{
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
    भिन्न buf[VTraits<v_float32>::max_nlanes*2];

    प्रति( पूर्णाङ्क i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<भिन्न>(ptr[i]);
    a = vx_आरोपय(buf);
}

स्थिर अन्तःस्थ शून्य vx_आरोपय_as(ध्रुव int64_t* ptr, v_float32& a)
{
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
    भिन्न buf[VTraits<v_float32>::max_nlanes*2];

    प्रति( पूर्णाङ्क i = 0; i < VECSZ; i++ )
        buf[i] = saturate_cast<भिन्न>(ptr[i]);
    a = vx_आरोपय(buf);
}

प्रतिरूपम्<प्रकारनाम _Tdvec>
स्थिर अन्तःस्थ शून्य vx_आरोपय_pair_as(ध्रुव द्विगुण* ptr, _Tdvec& a, _Tdvec& b)
{
    ध्रुव पूर्णाङ्क VECSZ = VTraits<_Tdvec>::vlanes();
    प्रकारनाम VTraits<_Tdvec>::lane_type buf[VTraits<_Tdvec>::max_nlanes*2];

    प्रति( पूर्णाङ्क i = 0; i < VECSZ*2; i++ )
        buf[i] = saturate_cast<प्रकारनाम VTraits<_Tdvec>::lane_type>(ptr[i]);
    a = vx_आरोपय(buf);
    b = vx_आरोपय(buf + VECSZ);
}

स्थिर अन्तःस्थ शून्य v_store_as(द्विगुण* ptr, ध्रुव v_float32& a)
{
    ध्रुव पूर्णाङ्क VECSZ = VTraits<v_float32>::vlanes();
    भिन्न buf[VTraits<v_float32>::max_nlanes];

    v_store(buf, a);
    प्रति( पूर्णाङ्क i = 0; i < VECSZ; i++ )
        ptr[i] = (द्विगुण)buf[i];
}

प्रतिरूपम्<प्रकारनाम _Tsvec>
स्थिर अन्तःस्थ शून्य v_store_pair_as(द्विगुण* ptr, ध्रुव _Tsvec& a, ध्रुव _Tsvec& b)
{
    ध्रुव पूर्णाङ्क VECSZ = VTraits<_Tsvec>::vlanes();
    प्रकारनाम VTraits<_Tsvec>::lane_type buf[VTraits<_Tsvec>::max_nlanes*2];

    v_store(buf, a); v_store(buf + VECSZ, b);
    प्रति( पूर्णाङ्क i = 0; i < VECSZ*2; i++ )
        ptr[i] = (द्विगुण)buf[i];
}

#यदि_अन्तः /////////// CV_SIMD_64F || CV_SIMD_SCALABLE_64F

#यदि_अन्तः /////////// CV_SIMD || CV_SIMD_SCALABLE

}

#यदि_अन्तः // SRC_CONVERT_HPP
