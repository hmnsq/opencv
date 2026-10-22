// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Element-wise kernels for the new arithmetic engine, SIMD-dispatched per CPU baseline.
//
// This file is compiled once per SIMD baseline (registered via ocv_add_dispatched_file). The per-op
// entry points get*Func_(...) live in cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE and return the kernel
// optimized for that baseline; the regular get*Func / getElemwiseFunc dispatchers (which forward here
// through CV_CPU_DISPATCH) live in arithm.dispatch.cpp.
//
// Kernel shape (house style of arithm.simd.hpp + convert.simd.hpp):
//  - one 2D tile; per-row outer loop with stepy; dst contiguous in x.
//  - stepx is restricted to {0,1}: 1 = contiguous (SIMD), 0 = broadcast-scalar along x.
//  - continuity collapse 2D->1D when every operand+dst is gap-free.
//  - halide right-edge backoff for the SIMD tail; scalar tail / scalar path for width<VECSZ.
//  - IN-PLACE SAFE: the right-edge backoff is suppressed (scalar tail) when dst aliases an input.
//
// One template (vecBinaryKernel) covers the whole matrix: the work-vector type selects the native
// same-type saturating path vs the widening f32-hub path. Adding a binary op = add one Op functor.

#അന്തര്ഭാവ "opencv2/core/hal/intrin.hpp"
#അന്തര്ഭാവ "convert.hpp"          // typed load/store-as helpers (cv::), and getConvert*Func
#അന്തര്ഭാവ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <cmath>
#അന്തര്ഭാവ <cstring>

നാമാകാശ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// BinaryFunc and getConvertFunc / getConvertScaleFunc come from core (precomp.hpp / convert.hpp).

#എങ്കിൽ CV_SIMD128_FP16
#undef CV_SIMD_16F
#define CV_SIMD_16F 1
#അല്ലെങ്കിലെങ്കിൽ !defined(CV_SIMD_16F)
#define CV_SIMD_16F 0       // keep `#if CV_SIMD_16F` -Wundef-clean on builds without FP16 SIMD
#endif

#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം uchar* p, v_uint8& a)
{ a = vx_setall_u8(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം schar* p, v_int8& a)
{ a = vx_setall_s8(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം uchar* p, v_int16& a)
{ a = vx_setall_s16(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം schar* p, v_int16& a)
{ a = vx_setall_s16(*p); }

സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം ushort* p, v_uint16& a)
{ a = vx_setall_u16(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം short* p, v_int16& a)
{ a = vx_setall_s16(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം ushort* p, v_int32& a)
{ a = vx_setall_s32(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം short* p, v_int32& a)
{ a = vx_setall_s32(*p); }

സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം unsigned* p, v_uint32& a)
{ a = vx_setall_u32(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം പൂർണ്ണസംഖ്യ* p, v_int32& a)
{ a = vx_setall_s32(*p); }

സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം uchar* p, v_float32& a)
{ a = vx_setall_f32(ദശാംശം(*p)); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം schar* p, v_float32& a)
{ a = vx_setall_f32(ദശാംശം(*p)); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം ushort* p, v_float32& a)
{ a = vx_setall_f32(ദശാംശം(*p)); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം short* p, v_float32& a)
{ a = vx_setall_f32(ദശാംശം(*p)); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം ദശാംശം* p, v_float32& a)
{ a = vx_setall_f32(*p); }

// ---------------------------------------------------------------------------
// Saturating integer helpers the universal intrinsics lack: v_add_sat/v_sub_sat for 32-bit lanes
// (v_add/v_sub saturate 8/16-bit but WRAP 32-bit), and v_mul_sat - the FULL-precision product
// clamped to the lane type, which is exactly cv::multiply's integer semantics at scale == 1.
// Local for now - the plan is to promote these into core/hal/intrin_*.hpp as proper universal
// intrinsics for all integer types. On NEON: single-instruction add/sub (vqadd/vqsub) and the
// widening-multiply + saturating-narrow pattern (vmull + vqmovn; the vget/vcombine forms compile
// to smull2/sqxtn2 on AArch64). Elsewhere: the Hacker's Delight (2-13) bit tricks for add/sub and
// the portable v_mul_expand + saturating v_pack composition for the 8/16-bit multiplies (RVV/LSX
// also have native ones - later, in the intrinsics). 32-bit lanes have no universal widening
// multiply (no v_mul_expand for s32), so 32-bit v_mul_sat is NEON-only (EW_HAVE_MULSAT32) and
// getMulFunc_ keeps the f64 work-vector kernels on the other backends.
#എങ്കിൽ defined(__ARM_NEON)
സ്ഥിതം വരിയിൽ v_int32  v_add_sat(സ്ഥിരം v_int32& a,  സ്ഥിരം v_int32& b)  { തിരികെ v_int32(vqaddq_s32(a.val, b.val)); }
സ്ഥിതം വരിയിൽ v_int32  v_sub_sat(സ്ഥിരം v_int32& a,  സ്ഥിരം v_int32& b)  { തിരികെ v_int32(vqsubq_s32(a.val, b.val)); }
സ്ഥിതം വരിയിൽ v_uint32 v_add_sat(സ്ഥിരം v_uint32& a, സ്ഥിരം v_uint32& b) { തിരികെ v_uint32(vqaddq_u32(a.val, b.val)); }
സ്ഥിതം വരിയിൽ v_uint32 v_sub_sat(സ്ഥിരം v_uint32& a, സ്ഥിരം v_uint32& b) { തിരികെ v_uint32(vqsubq_u32(a.val, b.val)); }
#define EW_HAVE_MULSAT32 1
സ്ഥിതം വരിയിൽ v_uint8 v_mul_sat(സ്ഥിരം v_uint8& a, സ്ഥിരം v_uint8& b)
{
    uint16x8_t p0 = vmull_u8(vget_താഴെയുള്ള_u8(a.val), vget_താഴെയുള്ള_u8(b.val));
    uint16x8_t p1 = vmull_u8(vget_മുകളിലെ_u8(a.val), vget_മുകളിലെ_u8(b.val));
    തിരികെ v_uint8(vcombine_u8(vqmovn_u16(p0), vqmovn_u16(p1)));
}
സ്ഥിതം വരിയിൽ v_int8 v_mul_sat(സ്ഥിരം v_int8& a, സ്ഥിരം v_int8& b)
{
    int16x8_t p0 = vmull_s8(vget_താഴെയുള്ള_s8(a.val), vget_താഴെയുള്ള_s8(b.val));
    int16x8_t p1 = vmull_s8(vget_മുകളിലെ_s8(a.val), vget_മുകളിലെ_s8(b.val));
    തിരികെ v_int8(vcombine_s8(vqmovn_s16(p0), vqmovn_s16(p1)));
}
സ്ഥിതം വരിയിൽ v_uint16 v_mul_sat(സ്ഥിരം v_uint16& a, സ്ഥിരം v_uint16& b)
{
    uint32x4_t p0 = vmull_u16(vget_താഴെയുള്ള_u16(a.val), vget_താഴെയുള്ള_u16(b.val));
    uint32x4_t p1 = vmull_u16(vget_മുകളിലെ_u16(a.val), vget_മുകളിലെ_u16(b.val));
    തിരികെ v_uint16(vcombine_u16(vqmovn_u32(p0), vqmovn_u32(p1)));
}
സ്ഥിതം വരിയിൽ v_int16 v_mul_sat(സ്ഥിരം v_int16& a, സ്ഥിരം v_int16& b)
{
    int32x4_t p0 = vmull_s16(vget_താഴെയുള്ള_s16(a.val), vget_താഴെയുള്ള_s16(b.val));
    int32x4_t p1 = vmull_s16(vget_മുകളിലെ_s16(a.val), vget_മുകളിലെ_s16(b.val));
    തിരികെ v_int16(vcombine_s16(vqmovn_s32(p0), vqmovn_s32(p1)));
}
സ്ഥിതം വരിയിൽ v_uint32 v_mul_sat(സ്ഥിരം v_uint32& a, സ്ഥിരം v_uint32& b)
{
    uint64x2_t p0 = vmull_u32(vget_താഴെയുള്ള_u32(a.val), vget_താഴെയുള്ള_u32(b.val));
    uint64x2_t p1 = vmull_u32(vget_മുകളിലെ_u32(a.val), vget_മുകളിലെ_u32(b.val));
    തിരികെ v_uint32(vcombine_u32(vqmovn_u64(p0), vqmovn_u64(p1)));
}
സ്ഥിതം വരിയിൽ v_int32 v_mul_sat(സ്ഥിരം v_int32& a, സ്ഥിരം v_int32& b)
{
    int64x2_t p0 = vmull_s32(vget_താഴെയുള്ള_s32(a.val), vget_താഴെയുള്ള_s32(b.val));
    int64x2_t p1 = vmull_s32(vget_മുകളിലെ_s32(a.val), vget_മുകളിലെ_s32(b.val));
    തിരികെ v_int32(vcombine_s32(vqmovn_s64(p0), vqmovn_s64(p1)));
}
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ v_int32 v_add_sat(സ്ഥിരം v_int32& a, സ്ഥിരം v_int32& b)
{
    v_int32 res = v_add(a, b);
    v_int32 ov  = v_and(v_xor(a, res), v_xor(b, res));               // sign bit set iff overflow
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(INT_MAX));       // a >= 0 ? INT_MAX : INT_MIN
    തിരികെ v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
സ്ഥിതം വരിയിൽ v_int32 v_sub_sat(സ്ഥിരം v_int32& a, സ്ഥിരം v_int32& b)
{
    v_int32 res = v_sub(a, b);
    v_int32 ov  = v_and(v_xor(a, b), v_xor(a, res));
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(INT_MAX));
    തിരികെ v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
സ്ഥിതം വരിയിൽ v_uint32 v_add_sat(സ്ഥിരം v_uint32& a, സ്ഥിരം v_uint32& b)
{
    v_uint32 res = v_add(a, b);
    തിരികെ v_or(res, v_lt(res, a));                                  // wrapped => all-ones
}
സ്ഥിതം വരിയിൽ v_uint32 v_sub_sat(സ്ഥിരം v_uint32& a, സ്ഥിരം v_uint32& b)
{
    തിരികെ v_and(v_sub(a, b), v_ge(a, b));                           // borrow => zero
}
സ്ഥിതം വരിയിൽ v_uint8 v_mul_sat(സ്ഥിരം v_uint8& a, സ്ഥിരം v_uint8& b)
{ v_uint16 p0, p1; v_mul_expand(a, b, p0, p1); തിരികെ v_pack(p0, p1); }
സ്ഥിതം വരിയിൽ v_int8 v_mul_sat(സ്ഥിരം v_int8& a, സ്ഥിരം v_int8& b)
{ v_int16 p0, p1; v_mul_expand(a, b, p0, p1); തിരികെ v_pack(p0, p1); }
സ്ഥിതം വരിയിൽ v_uint16 v_mul_sat(സ്ഥിരം v_uint16& a, സ്ഥിരം v_uint16& b)
{ v_uint32 p0, p1; v_mul_expand(a, b, p0, p1); തിരികെ v_pack(p0, p1); }
സ്ഥിതം വരിയിൽ v_int16 v_mul_sat(സ്ഥിരം v_int16& a, സ്ഥിരം v_int16& b)
{ v_int32 p0, p1; v_mul_expand(a, b, p0, p1); തിരികെ v_pack(p0, p1); }
#endif
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F   // scalable (RVV) has v_float64 without CV_SIMD_64F
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം ഇരട്ട*   p, v_float64& a) { a = vx_setall_f64(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം പൂർണ്ണസംഖ്യ*      p, v_float64& a) { a = vx_setall_f64((ഇരട്ട)*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം unsigned* p, v_float64& a) { a = vx_setall_f64((ഇരട്ട)*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം int64_t*  p, v_float64& a) { a = vx_setall_f64((ഇരട്ട)*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം uint64_t* p, v_float64& a) { a = vx_setall_f64((ഇരട്ട)*p); }
#endif
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം bfloat* p, v_float32& a)
{ a = vx_setall_f32(ദശാംശം(*p)); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം hfloat* p, v_float32& a)
{ a = vx_setall_f32(ദശാംശം(*p)); }

സ്ഥിതം വരിയിൽ ശൂന്യം vx_load_pair_as(സ്ഥിരം uchar* p, v_uint8& a, v_uint8& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_uint8>::vlanes());
}

സ്ഥിതം വരിയിൽ ശൂന്യം vx_load_pair_as(സ്ഥിരം schar* p, v_int8& a, v_int8& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_int8>::vlanes());
}

സ്ഥിതം വരിയിൽ ശൂന്യം vx_load_pair_as(സ്ഥിരം uchar* p, v_uint32& a, v_uint32& b)
{
    v_uint16 w = vx_load_expand(p);
    v_expand(w, a, b);
}

സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(uchar* p, സ്ഥിരം v_uint8& a, സ്ഥിരം v_uint8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_uint8>::vlanes(), b);
}

സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(schar* p, സ്ഥിരം v_int8& a, സ്ഥിരം v_int8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_int8>::vlanes(), b);
}


സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(പൂർണ്ണസംഖ്യ* p, സ്ഥിരം v_int16& a, സ്ഥിരം v_int16& b)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ nlanes32 = VTraits<v_int32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v0);
    v_store(p + nlanes32, v1);
    v_store(p + nlanes32*2, v2);
    v_store(p + nlanes32*3, v3);
}

സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(ദശാംശം* p, സ്ഥിരം v_int16& a, സ്ഥിരം v_int16& b)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ nlanes32 = VTraits<v_float32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v_cvt_f32(v0));
    v_store(p + nlanes32, v_cvt_f32(v1));
    v_store(p + nlanes32*2, v_cvt_f32(v2));
    v_store(p + nlanes32*3, v_cvt_f32(v3));
}

സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(ദശാംശം* p, സ്ഥിരം v_int32& a, സ്ഥിരം v_int32& b)
{
    v_store(p, v_cvt_f32(a));
    v_store(p + VTraits<v_float32>::vlanes(), v_cvt_f32(b));
}

സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(hfloat* p, സ്ഥിരം v_float32& a, സ്ഥിരം v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(bfloat* p, സ്ഥിരം v_float32& a, സ്ഥിരം v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

// Reinterpret a comparison result (whose lane type matches the operands) as the UNSIGNED integer
// vector of the same width: u8->u8, s8->u8, u16/s16->u16, u32/s32/f32->u32. The destination type
// drives the overload, so the compare kernel needs no per-width if constexpr.
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_uint8&  s, v_uint8&  d) { d = s; }
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_int8&   s, v_uint8&  d) { d = v_reinterpret_as_u8(s); }
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_uint16& s, v_uint16& d) { d = s; }
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_int16&  s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_uint32& s, v_uint32& d) { d = s; }
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_int32&  s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_float32& s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
#എങ്കിൽ CV_SIMD_16F
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_float16& s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
#endif
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
സ്ഥിതം വരിയിൽ ശൂന്യം v_reinterpret_as(സ്ഥിരം v_float64& s, v_uint64& d) { d = v_reinterpret_as_u64(s); }
#endif

// Broadcast the compare mask value (1 or 255) across the unsigned work vector.
സ്ഥിതം വരിയിൽ ശൂന്യം v_setall_mask(v_uint8&  v, uchar t) { v = vx_setall_u8(t); }
സ്ഥിതം വരിയിൽ ശൂന്യം v_setall_mask(v_uint16& v, uchar t) { v = vx_setall_u16(t); }
സ്ഥിതം വരിയിൽ ശൂന്യം v_setall_mask(v_uint32& v, uchar t) { v = vx_setall_u32(t); }

// NOTE: v_store_pair_as(uchar*, v_uint16/v_uint32, ...) - the narrowing pack-to-u8 stores we need -
// already come from core's convert.hpp (included above), so they are not redefined here.

#എങ്കിൽ CV_SIMD_16F
സ്ഥിതം വരിയിൽ ശൂന്യം vx_setall_as(സ്ഥിരം hfloat* p, v_float16& a)
{ a = vx_setall_f16(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം vx_load_pair_as(സ്ഥിരം hfloat* p, v_float16& a, v_float16& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_float16>::vlanes());
}
സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(hfloat* p, സ്ഥിരം v_float16& a, സ്ഥിരം v_float16& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_float16>::vlanes(), b);
}
#endif

#endif

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

നാമാകാശ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      arithm.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getAddFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);
TKernel getSubFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);
TKernel getMulFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);
TKernel getDivFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R, തർക്കം checked);
TKernel getMinFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);
TKernel getMaxFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);
TKernel getAbsdiffFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);
TKernel getHypotFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);         // hypot = sqrt(x^2+y^2), float depths, T x T -> T
TKernel getAtan2Func_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);         // atan2(y, x), radians (-pi, pi], float depths
TKernel getCmpFunc_(മുകൾ op, പൂർണ്ണസംഖ്യ T);
TKernel getBitwiseFunc_(മുകൾ op, പൂർണ്ണസംഖ്യ esz);                    // OP_AND / OP_OR / OP_XOR, by element size
TKernel getNotFunc_(പൂർണ്ണസംഖ്യ esz);                                // OP_NOT, by element size
TKernel getAddWeightedFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);                   // OP_ADDW, a*alpha+b*beta+gamma (T x T -> R)
TKernel getSelectFunc_(പൂർണ്ണസംഖ്യ mdepth, പൂർണ്ണസംഖ്യ T);   // OP_SELECT: 1-byte mask, a/b/dst of T (by esz)
TKernel getClampFunc_(പൂർണ്ണസംഖ്യ T);                // OP_CLAMP: min(max(x, lo), hi), all operands of T
TKernel getCastFunc_(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth, തർക്കം scaled);   // OP_CAST / OP_CONVERT_SCALE

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors (vector + scalar). New binary ops slot in here.
// ===========================================================================
ഘടന EwAdd {
    // useScalar: does the op consume the scale scalar (params[0]) in vec()/preproc()? When false,
    // vecBinaryKernel's fast 2-arg branch is taken unconditionally (the check folds at compile time);
    // when true, it is taken only if the runtime scale is exactly 1.
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_add(a, b); }
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: v_add wraps - use the saturating version to match the scalar int64+clamp tail
    സ്ഥിതം v_int32  vec(സ്ഥിരം v_int32& a,  സ്ഥിരം v_int32& b)  { തിരികെ v_add_sat(a, b); }
    സ്ഥിതം v_uint32 vec(സ്ഥിരം v_uint32& a, സ്ഥിരം v_uint32& b) { തിരികെ v_add_sat(a, b); }
#endif
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    // Accumulate in the promoted type, NOT in W: for the native saturating path W is the narrow
    // lane type (schar/short/...), and (W)(a+b) would wrap in 8/16 bits before saturate_cast<Tr>
    // could clamp. Letting a+b promote (narrow -> int) keeps saturation for 8/16-bit outputs and
    // the natural wrap for 32/64-bit (both matching cv::add). The SIMD path already saturates.
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ W(a + b); }
};

ഘടന EwSub {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_sub(a, b); }
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: see EwAdd::vec
    സ്ഥിതം v_int32  vec(സ്ഥിരം v_int32& a,  സ്ഥിരം v_int32& b)  { തിരികെ v_sub_sat(a, b); }
    സ്ഥിതം v_uint32 vec(സ്ഥിരം v_uint32& a, സ്ഥിരം v_uint32& b) { തിരികെ v_sub_sat(a, b); }
#endif
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ W(a - b); }   // see EwAdd::scl
    // 64-bit unsigned has no wider WT to hold a-b, so the generic path would wrap on underflow.
    // Saturate at 0 to match cv::subtract (8/16/32-bit already saturate via SIMD floor / wide WT).
    സ്ഥിതം uint64_t scl(uint64_t a, uint64_t b, uint64_t) { തിരികെ uint64_t(a >= b)*(a - b); }
};

// mul/div compute in a wide FLOAT work type (W = float for <=16-bit/f16/bf/f32, double for
// 32/64-bit/f64), matching cv::multiply/divide; the executor casts the work-type result down to
// rdepth. scalar (reference) path only for now - SIMD can follow. (No vec(): never instantiated.)
// vec(a, b, scale): the 3rd arg is the scale vector. COMMUTATIVE ops (mul, absdiff) ignore it - their
// scale is folded into the (cheaper) preproc of one operand. DIVISION is NOT commutative, so its scale
// MUST stick to the numerator; it takes the scale in vec (numerator*scale/denominator) and leaves
// preproc as identity. vecBinaryKernel always passes vscalar to vec, so every branch (incl. a broadcast
// denominator) divides correctly. The 2-arg vec (scale==1) is the both-contiguous fast path.
ഘടന EwMul {
    സ്ഥിതം constexpr തർക്കം useScalar = സത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_mul(a, b); }
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    // integer lanes: full product clamped to the lane type (v_mul_sat) == cv::multiply semantics
    // at scale 1. These drive the scale==1 fast path on FULL-width registers (Wvec1 = the native
    // lane vector): whole-register loads/stores, widening happens inside the multiply itself.
    // The guard matches the v_mul_sat definitions above (like EwAdd's 32-bit overloads): in
    // no-SIMD builds the vector typedefs still exist (intrin_cpp), but the helpers do not.
    സ്ഥിതം v_uint8  vec(സ്ഥിരം v_uint8& a,  സ്ഥിരം v_uint8& b)  { തിരികെ v_mul_sat(a, b); }
    സ്ഥിതം v_int8   vec(സ്ഥിരം v_int8& a,   സ്ഥിരം v_int8& b)   { തിരികെ v_mul_sat(a, b); }
    സ്ഥിതം v_uint16 vec(സ്ഥിരം v_uint16& a, സ്ഥിരം v_uint16& b) { തിരികെ v_mul_sat(a, b); }
    സ്ഥിതം v_int16  vec(സ്ഥിരം v_int16& a,  സ്ഥിരം v_int16& b)  { തിരികെ v_mul_sat(a, b); }
#ifdef EW_HAVE_MULSAT32
    സ്ഥിതം v_uint32 vec(സ്ഥിരം v_uint32& a, സ്ഥിരം v_uint32& b) { തിരികെ v_mul_sat(a, b); }
    സ്ഥിതം v_int32  vec(സ്ഥിരം v_int32& a,  സ്ഥിരം v_int32& b)  { തിരികെ v_mul_sat(a, b); }
#endif
#endif
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V& s) { തിരികെ v_mul(a, s); }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST s) { തിരികെ a * b * s; }
};
// div has two variants by the COMMON INPUT type (matching cv::'s per-type kernel choice): integer
// inputs guard divide-by-zero -> 0 (cv:: iscalar_div); float inputs do NOT guard (cv:: fscalar_div,
// a/0 -> inf), which then saturates on the cast to an integer output exactly like cv::divide.
ഘടന EwDivInt {
    സ്ഥിതം constexpr തർക്കം useScalar = സത്യം;
    // integer inputs computed in the float work type: guard b==0 -> 0. Scale rides the numerator (vec).
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) {
        സ്ഥിരം V z = v_setzero_<V>();
        തിരികെ v_select(v_eq(b, z), z, v_div(a, b));
    }
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം V& s) {
        സ്ഥിരം V z = v_setzero_<V>();
        തിരികെ v_select(v_eq(b, z), z, v_div(v_mul(a, s), b));
    }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }   // identity: scale is in vec
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST s) { തിരികെ b != W(0) ? a * s / b : W(0); }
};
ഘടന EwDivFlt {
    സ്ഥിതം constexpr തർക്കം useScalar = സത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_div(a, b); }
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം V& s) { തിരികെ v_div(v_mul(a, s), b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }   // identity: scale is in vec
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST s) { തിരികെ a * s / b; }
};

// atan2(y, x) in RADIANS over the standard C range (-pi, pi] - the fastAtan2 minimax polynomial
// (mathfuncs_core.simd.hpp v_atan_f32) with the 180/pi factor dropped and the C quadrant logic
// (fastAtan2 returns degrees in [0, 360)). Absolute accuracy ~1e-5 rad, same as cv::fastAtan2.
// Generic over the universal-intrinsic float vector type.
template<typename V>
സ്ഥിതം വരിയിൽ V v_atan2(സ്ഥിരം V& y, സ്ഥിരം V& x)
{
    using LT = typename VTraits<V>::lane_type;
    സ്ഥിരം V eps  = v_setall_<V>((LT)DBL_EPSILON);
    സ്ഥിരം V z    = v_setzero_<V>();
    സ്ഥിരം V p7   = v_setall_<V>((LT)-0.04432655554792128);
    സ്ഥിരം V p5   = v_setall_<V>((LT)0.1555786518463281);
    സ്ഥിരം V p3   = v_setall_<V>((LT)-0.3258083974640975);
    സ്ഥിരം V p1   = v_setall_<V>((LT)0.9997878412794807);
    സ്ഥിരം V vpi2 = v_setall_<V>((LT)(CV_PI/2));
    സ്ഥിരം V vpi  = v_setall_<V>((LT)CV_PI);

    V ax = v_abs(x), ay = v_abs(y);
    V c  = v_div(v_min(ax, ay), v_add(v_max(ax, ay), eps));
    V c2 = v_mul(c, c);
    V a  = v_mul(v_fma(v_fma(v_fma(p7, c2, p5), c2, p3), c2, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(vpi2, a));
    a = v_select(v_lt(x, z), v_sub(vpi, a), a);
    a = v_select(v_lt(y, z), v_sub(z, a), a);
    തിരികെ a;
}

ഘടന EwAtan2 {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_atan2(a, b); }   // a = y, b = x
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ std::atan2(a, b); }
};

// hypot(x, y) = sqrt(x^2 + y^2): NAIVE (matches cv::magnitude; overflow at |x| ~ 1e19+ for f32
// inputs is accepted), computed in the float work type; T x T -> T over the float depths.
ഘടന EwHypot {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_sqrt(v_fma(a, a, v_mul(b, b))); }
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ std::sqrt(a*a + b*b); }
};

// min / max / absdiff: T x T -> T (same depth in and out, no scale). v_min/v_max exist for every
// vector lane type (64-bit ints fall back to scalar). absdiff uses v_absdiff (defined for the
// UNSIGNED and float lane types - signed/wide depths go through the scalar path), and the scalar
// |a-b| is computed branch-wise so it never underflows an unsigned work type.
ഘടന EwMin {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_min(a, b); }
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ std::min(a, b); }
};
ഘടന EwMax {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_max(a, b); }
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ std::max(a, b); }
};
ഘടന EwAbsdiff {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    // The absdiff RESULT is the UNSIGNED type of the input width: v_absdiff(v_int8/16/32) already returns
    // v_uint8/16/32 (the true |a-b|, which can exceed the signed max), and v_absdiff on unsigned/float
    // returns the same type. vec() therefore returns THAT type (deduced), not the input V, so the kernel
    // stores it through the matching UNSIGNED v_store_pair_as (an honest same-type store) - no reinterpret,
    // no touching the saturating-narrow overloads' semantics. (This is why the kernel keeps the vec
    // result in its own variable rather than reusing the input operand.)
    template<typename V> സ്ഥിതം auto vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_absdiff(a, b); }
    template<typename V, typename S> സ്ഥിതം auto vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ v_absdiff(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ a > b ? W(a - b) : W(b - a); }
};

// Signed T -> SAME signed T (rdepth==T): |a-b| saturated into the signed range, in ONE pass. cv::absdiff
// keeps the signed depth, so the generic path computes absdiff->unsigned then casts back down (2 insns);
// this fuses it. v_absdiff yields the unsigned |a-b|; v_min clamps to the signed max (already >=0), then
// a same-width reinterpret to signed (all values now fit).
ഘടന EwAbsdiffS {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    സ്ഥിതം v_int8  vec(സ്ഥിരം v_int8&  a, സ്ഥിരം v_int8&  b) { തിരികെ v_reinterpret_as_s8 (v_min(v_absdiff(a, b), vx_setall_u8 (0x7f))); }
    സ്ഥിതം v_int16 vec(സ്ഥിരം v_int16& a, സ്ഥിരം v_int16& b) { തിരികെ v_reinterpret_as_s16(v_min(v_absdiff(a, b), vx_setall_u16(0x7fff))); }
    സ്ഥിതം v_int32 vec(സ്ഥിരം v_int32& a, സ്ഥിരം v_int32& b) { തിരികെ v_reinterpret_as_s32(v_min(v_absdiff(a, b), vx_setall_u32(0x7fffffff))); }
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ a > b ? W(a - b) : W(b - a); }
    // 64-bit signed has no wider work type to widen into, and |INT64_MIN - b| does not fit in
    // int64_t, so the difference is taken in uint64 - exact for every pair - and then clamped.
    // The narrower widths reach this overload only through their int64 work type, where |a - b|
    // is at most 2^32 and the clamp is a no-op.
    സ്ഥിതം int64_t scl(int64_t a, int64_t b, int64_t)
    {
        സ്ഥിരം uint64_t d = (a > b) ? (uint64_t)a - (uint64_t)b : (uint64_t)b - (uint64_t)a;
        തിരികെ (int64_t)std::min<uint64_t>(d, (uint64_t)INT64_MAX);
    }
};

// compare: T x T -> u8 mask. cmp(a,b) is the scalar relation over a work type W (integers compare
// directly, f16/bf16 through float); vec(a,b) is the vector relation returning an all-ones/zero mask
// in the operand's own lane type. The kernel turns either into 0 / trueVal (1 or 255 via TKernel::flags).
ഘടന EwCmpEq { template<typename W> സ്ഥിതം തർക്കം cmp(W a, W b) { തിരികെ a == b; }
                 template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_eq(a, b); } };
ഘടന EwCmpNe { template<typename W> സ്ഥിതം തർക്കം cmp(W a, W b) { തിരികെ a != b; }
                 template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_ne(a, b); } };
// LT/LE are synthesized from GT/GE via the EW_KERNEL_SWAP01 flag (a<b == b>a), so no Lt/Le kernels.
ഘടന EwCmpGt { template<typename W> സ്ഥിതം തർക്കം cmp(W a, W b) { തിരികെ a > b; }
                 template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_gt(a, b); } };
ഘടന EwCmpGe { template<typename W> സ്ഥിതം തർക്കം cmp(W a, W b) { തിരികെ a >= b; }
                 template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_ge(a, b); } };

// bitwise AND/OR/XOR: bit-pattern op, type-agnostic. Run on the UNSIGNED integer whose width matches
// the element (u8/u16/u32/u64), so one functor set covers every depth. No scale, no widening (T x T ->
// T, exactly like min/max); preproc is the identity (min/max share this shape). 64-bit uses the scalar
// path (no widening vector helpers), the rest ride vecBinaryKernel's native same-type path.
ഘടന EwAnd {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_and(a, b); }
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ W(a & b); }
};
ഘടന EwOr {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_or(a, b); }
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ W(a | b); }
};
ഘടന EwXor {
    സ്ഥിതം constexpr തർക്കം useScalar = അസത്യം;
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b) { തിരികെ v_xor(a, b); }
    template<typename V, typename S> സ്ഥിതം V vec(സ്ഥിരം V& a, സ്ഥിരം V& b, സ്ഥിരം S&) { തിരികെ vec(a, b); }
    template<typename V> സ്ഥിതം V preproc(സ്ഥിരം V& a, സ്ഥിരം V&) { തിരികെ a; }
    template<typename W, typename ST> സ്ഥിതം W scl(W a, W b, ST) { തിരികെ W(a ^ b); }
};

// Collapse a gap-free 2D tile to 1D (call with the per-operand x/y-steps).
#define EW_TRY_COLLAPSE(NSRC) \
    എങ്കിൽ (height > 1 && dsty == (size_t)width && \
        s0y == s0x*(size_t)width && (NSRC < 2 || s1y == s1x*(size_t)width)) \
    { width *= height; height = 1; }

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
template<typename T, typename Tr, typename WT, വർഗ്ഗം Op, typename ST=WT>
സ്ഥിതം പൂർണ്ണസംഖ്യ scalar_ദ്വിമാന_Kernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                              സ്ഥിരം ശൂന്യം* src1_, size_t s1y, size_t s1x,
                              സ്ഥിരം ശൂന്യം*, size_t, size_t, ശൂന്യം* dst_, size_t dsty,
                              പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, സ്ഥിരം ഇരട്ട* params, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    എങ്കിൽ (width <= 0 || height <= 0) തിരികെ 0;
    s0y /= വലുപ്പം(T);
    s1y /= വലുപ്പം(T);
    dsty /= വലുപ്പം(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);

    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* src1 = (സ്ഥിരം T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        എങ്കിൽ (s0x == s1x) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], (WT)src1[x], scalar));
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s0x == 0) {
            WT sc0 = (WT)src0[0];
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl(sc0, (WT)src1[x], scalar));
        }
        അല്ലെങ്കിൽ {
            WT sc1 = (WT)src1[0];
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], sc1, scalar));
        }
    }
    തിരികെ 0;
}

template<typename T, typename WT>
സ്ഥിതം ശൂന്യം expandScalar(സ്ഥിരം T* sc, size_t sx, പൂർണ്ണസംഖ്യ n0, WT* scbuf, പൂർണ്ണസംഖ്യ n)
{
    എങ്കിൽ (n0 <= 0 || n <= 0) തിരികെ;
    പൂർണ്ണസംഖ്യ i = 0;
    വേണ്ടി (; i < n0; i++) scbuf[i] = (WT)sc[i*sx];
    വേണ്ടി (; i < n; i++) scbuf[i] = scbuf[i - n0];
}

// Decode the per-channel patch bytes (mask + value) from the kernel flags into small arrays. Uniform
// (no EW_CMP_PATCH): mask = trueVal, value = 0 (an ordinary compare). Per-channel: from the 4-bit
// fields. The compare kernels apply  result = (rawmask & mask) | value  per channel - folding the
// former separate patch pass into the compare (one pass).
സ്ഥിതം വരിയിൽ ശൂന്യം cmpUnpackPatch(പൂർണ്ണസംഖ്യ flags, uchar trueVal, uchar mvals[4], uchar vvals[4])
{
    എങ്കിൽ (flags & EW_CMP_PATCH)
        വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < 4; c++)
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ f = (flags >> (EW_CMP_PATCH_SHIFT + c*4)) & 0xF;
            mvals[c] = (uchar)cmpPatchByte(f & 3);
            vvals[c] = (uchar)cmpPatchByte((f >> 2) & 3);
        }
    അല്ലെങ്കിൽ
        വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < 4; c++) { mvals[c] = trueVal; vvals[c] = 0; }
}

// Scalar comparison kernel: T x T -> u8 mask. Result is 0 (false) or `trueVal` (true), trueVal read
// from TKernel::flags - 255 (default, matching cv::compare) or 1 (a numpy-style 0/1 mask). Integers
// compare directly (WT == T); f16/bf16 compare through a float WT. This is the fallback for the depths
// the SIMD kernel doesn't cover (f16/bf16/64-bit). stepx in {0,1}; dst is u8, byte step == element step.
template<typename T, typename WT, വർഗ്ഗം Cmp>
സ്ഥിതം പൂർണ്ണസംഖ്യ scalarCompareKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                         സ്ഥിരം ശൂന്യം* src1_, size_t s1y, size_t s1x,
                         സ്ഥിരം ശൂന്യം*, size_t, size_t,
                         ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                         സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ flags, ശൂന്യം*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    എങ്കിൽ (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= വലുപ്പം(T);
    s1y /= വലുപ്പം(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);

    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* src1 = (സ്ഥിരം T*)src1_;
    uchar* dst = (uchar*)dst_;
    സ്ഥിരം uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    സ്ഥിരം തർക്കം perch = (flags & EW_CMP_PATCH) != 0;          // per-channel fix-up (width=cn<=4)
    CV_Assert(!perch || width <= 4);                         // a per-channel patch is a short-row tile
    #define EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                         : (uchar)((cond) ? trueVal : 0))

    EW_TRY_COLLAPSE(2);
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        എങ്കിൽ (s0x == s1x) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], (WT)src1[x]), x);
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s0x == 0) {
            WT a = (WT)src0[0];
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, (WT)src1[x]), x);
        }
        അല്ലെങ്കിൽ {
            WT b = (WT)src1[0];
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], b), x);
        }
    }
    #undef EW_CMP_APPLY
    തിരികെ 0;
}

// SIMD comparison kernel: T x T -> u8 mask, for the directly-comparable depths (u8/s8/u16/s16/u32/
// s32/f32). vec(a,b) gives an all-ones/zero mask in the operand's lane type; v_reinterpret_as turns it
// into the unsigned int of the same width; cmpFuse packs to u8 and applies (rawmask & M) | V. The
// per-row SIMD body runs when an operand is contiguous or broadcast; a per-channel patch (M/V differ
// by channel) only ever arrives as a short-row (width=cn) tile and is handled there.
template<typename T, typename Vvec, typename Uvec, വർഗ്ഗം Cmp>
സ്ഥിതം പൂർണ്ണസംഖ്യ vecCompareKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                            സ്ഥിരം ശൂന്യം* src1_, size_t s1y, size_t s1x,
                            സ്ഥിരം ശൂന്യം*, size_t, size_t,
                            ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                            സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ flags, ശൂന്യം*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    എങ്കിൽ (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= വലുപ്പം(T);
    s1y /= വലുപ്പം(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);

    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* src1 = (സ്ഥിരം T*)src1_;
    uchar* dst = (uchar*)dst_;
    സ്ഥിരം uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];                                 // per-channel fix-up: (rawmask & m) | v
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    സ്ഥിരം തർക്കം perch = (flags & EW_CMP_PATCH) != 0;
    CV_Assert(!perch || width <= 4);                          // a per-channel patch is a short-row tile

    EW_TRY_COLLAPSE(2);
    പൂർണ്ണസംഖ്യ y = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    // Short rows (2/3/4 elements): a per-channel scalar over a multi-channel image arrives as a
    // (width=cn) x (height=pixels) tile with the scalar broadcast over rows (s?y==0). The per-row SIMD
    // below never triggers at such a tiny width, so expand the width<=4 broadcast operand (threshold
    // AND the interleaved per-channel mask/value bytes) across the lanes and compare many rows at once.
    // This is also the ONLY path a per-channel patch (M/V differ by channel) reaches.
    // constexpr: the short-row loads T natively (vx_load->Vvec), so it exists ONLY at native width. The
    // widened f16/bf16->f32 path (sizeof(T) < Vvec lane) and 8-byte f64 fall through to per-row + scalar.
    // (sizeof(T) <= 4 is part of the constexpr gate: for f64 the body would compile to nothing but
    // dead stores - gcc 9 flags scbuf/bsrc/bsx as set-but-not-used there.)
    എങ്കിൽ constexpr (വലുപ്പം(T) == വലുപ്പം(typename VTraits<Vvec>::lane_type) && വലുപ്പം(T) <= 4)
    എങ്കിൽ (height > 1 && width <= 4 &&
        ((s0y == 0 && s1y == (size_t)width*s1x) || (s1y == 0 && s0y == (size_t)width*s0x)) &&
        dsty == (size_t)width)
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ  = VTraits<Vvec>::vlanes();          // sizeof(T)-type lanes
        സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ8 = VTraits<v_uint8>::vlanes();       // u8 output lanes
        constexpr പൂർണ്ണസംഖ്യ MAXV8 = VTraits<v_uint8>::max_nlanes;
        T scbuf[MAXV8 * 3] = {};                             // interleaved threshold (elements)
        uchar mbuf[MAXV8 * 3] = {}, vbuf[MAXV8 * 3] = {};    // interleaved mask / value (bytes)
        // ^ the {} inits are for -Wmaybe-uninitialized only: expandScalar/decode fill every lane that
        //   is later read, but the compiler cannot prove it with a runtime VECSZ
        സ്ഥിരം തർക്കം bc0 = (s0y == 0);                         // src0 is the broadcast (short) operand
        സ്ഥിരം T* bsrc = bc0 ? src0 : src1; സ്ഥിരം size_t bsx = bc0 ? s0x : s1x;
        സ്ഥിരം T*& src = bc0 ? src1 : src0;                   // the row-stepping operand (advanced below)
        സ്ഥിരം size_t sy = bc0 ? s1y : s0y;
        // The compare direction (scalar-first when bc0, array-first otherwise) is hoisted OUT of the row
        // loop with an explicit if(bc0) - the loop is written twice so no per-iteration branch remains.
        // Per branch: own unroll, threshold + interleaved M/V loaded ONCE. sizeof==1 keeps 12 vectors.
        #define EW_CMPR(r, a, b) v_reinterpret_as(Cmp::vec(a, b), r)   // rawmask -> sizeof(T)-byte uint
        എങ്കിൽ constexpr (വലുപ്പം(T) == 1)                        // 3 u8 masks -> 3 u8 stores
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ ewidth = VECSZ8 * 3;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, width, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, width, vbuf, ewidth);
            സ്ഥിരം പൂർണ്ണസംഖ്യ dy = ewidth / width;
            Vvec  sc0=vx_load(scbuf),  sc1=vx_load(scbuf+VECSZ),   sc2=vx_load(scbuf+VECSZ*2);
            v_uint8 M0=vx_load(mbuf),  M1=vx_load(mbuf+VECSZ8),    M2=vx_load(mbuf+VECSZ8*2);
            v_uint8 V0=vx_load(vbuf),  V1=vx_load(vbuf+VECSZ8),    V2=vx_load(vbuf+VECSZ8*2);
            #define EW_ROW1(A0,B0,A1,B1,A2,B2) \
                വേണ്ടി (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 o0=v_or(v_and(r0,M0),V0),o1=v_or(v_and(r1,M1),V1),o2=v_or(v_and(r2,M2),V2); \
                    v_store(dst, o0); v_store(dst+VECSZ8, o1); v_store(dst+VECSZ8*2, o2); }
            എങ്കിൽ (bc0) EW_ROW1(sc0,vx_load(src), sc1,vx_load(src+VECSZ), sc2,vx_load(src+VECSZ*2))
            അല്ലെങ്കിൽ     EW_ROW1(vx_load(src),sc0, vx_load(src+VECSZ),sc1, vx_load(src+VECSZ*2),sc2)
            #undef EW_ROW1
        }
        അല്ലെങ്കിൽ എങ്കിൽ constexpr (വലുപ്പം(T) == 2)                   // 3 u16 masks -> 1 full + 1 half u8 store
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ ewidth = VECSZ * 3;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, width, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, width, vbuf, ewidth);
            സ്ഥിരം പൂർണ്ണസംഖ്യ dy = ewidth / width;
            Vvec  sc0=vx_load(scbuf), sc1=vx_load(scbuf+VECSZ), sc2=vx_load(scbuf+VECSZ*2);
            v_uint8 M0=vx_load(mbuf), M1=vx_load(mbuf+VECSZ8);
            v_uint8 V0=vx_load(vbuf), V1=vx_load(vbuf+VECSZ8);
            #define EW_ROW2(A0,B0,A1,B1,A2,B2) \
                വേണ്ടി (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 b0=v_pack(r0,r1), b1=v_pack(r2,r2); \
                    v_uint8 o0=v_or(v_and(b0,M0),V0), o1=v_or(v_and(b1,M1),V1); \
                    v_store(dst, o0); v_store_താഴെയുള്ള(dst+VECSZ8, o1); }
            എങ്കിൽ (bc0) EW_ROW2(sc0,vx_load(src), sc1,vx_load(src+VECSZ), sc2,vx_load(src+VECSZ*2))
            അല്ലെങ്കിൽ     EW_ROW2(vx_load(src),sc0, vx_load(src+VECSZ),sc1, vx_load(src+VECSZ*2),sc2)
            #undef EW_ROW2
        }
        അല്ലെങ്കിൽ എങ്കിൽ constexpr (വലുപ്പം(T) == 4)                   // sizeof==4: 6 u32 -> 3 u16 -> 1 full + 1 half u8
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ ewidth = VECSZ * 6;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, width, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, width, vbuf, ewidth);
            സ്ഥിരം പൂർണ്ണസംഖ്യ dy = ewidth / width;
            Vvec sc0=vx_load(scbuf),sc1=vx_load(scbuf+VECSZ),sc2=vx_load(scbuf+VECSZ*2),
                 sc3=vx_load(scbuf+VECSZ*3),sc4=vx_load(scbuf+VECSZ*4),sc5=vx_load(scbuf+VECSZ*5);
            v_uint8 M0=vx_load(mbuf), M1=vx_load(mbuf+VECSZ8);
            v_uint8 V0=vx_load(vbuf), V1=vx_load(vbuf+VECSZ8);
            #define EW_ROW4(A0,B0,A1,B1,A2,B2,A3,B3,A4,B4,A5,B5) \
                വേണ്ടി (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2,r3,r4,r5; \
                    EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    EW_CMPR(r3,A3,B3); EW_CMPR(r4,A4,B4); EW_CMPR(r5,A5,B5); \
                    v_uint16 p0=v_pack(r0,r1), p1=v_pack(r2,r3), p2=v_pack(r4,r5); \
                    v_uint8 q0=v_pack(p0,p1), q1=v_pack(p2,p2); \
                    v_uint8 o0=v_or(v_and(q0,M0),V0), o1=v_or(v_and(q1,M1),V1); \
                    v_store(dst, o0); v_store_താഴെയുള്ള(dst+VECSZ8, o1); }
            എങ്കിൽ (bc0) EW_ROW4(sc0,vx_load(src),sc1,vx_load(src+VECSZ),sc2,vx_load(src+VECSZ*2),
                             sc3,vx_load(src+VECSZ*3),sc4,vx_load(src+VECSZ*4),sc5,vx_load(src+VECSZ*5))
            അല്ലെങ്കിൽ     EW_ROW4(vx_load(src),sc0,vx_load(src+VECSZ),sc1,vx_load(src+VECSZ*2),sc2,
                             vx_load(src+VECSZ*3),sc3,vx_load(src+VECSZ*4),sc4,vx_load(src+VECSZ*5),sc5)
            #undef EW_ROW4
        }
        #undef EW_CMPR
    }
#endif
    വേണ്ടി (; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        പൂർണ്ണസംഖ്യ x = 0;
    #എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        // SIMD for BOTH the contiguous case AND a broadcast operand (a per-channel scalar const has
        // step 0 - without this the scalar-vs-array compare, incl. every multi-channel scalar compare,
        // fell to the scalar tail below, ~5x slower). vx_setall broadcasts the step-0 operand once.
        // 4-vector unroll + halide right-edge backoff (reprocess the last 4*VECSZ when width is not a
        // multiple); dst is a separate mask buffer so the overlap is a harmless idempotent rewrite.
        സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Vvec>::vlanes();
        സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ8 = VTraits<v_uint8>::vlanes();
        v_uint8 vT; v_setall_mask(vT, trueVal);              // uniform mask (per-channel patch is short-row)
        സ്ഥിരം തർക്കം tailTrick = width >= 4*VECSZ && src0_ != dst_ && src1_ != dst_;
        // Narrow the 4 masks to u8 FIRST, then apply trueVal on the u8 result (1 and per u8 vector, not
        // per wide lane-group). One block per MASK lane width (== sizeof(T) natively, but 4 for the
        // f16/bf16->f32 widened path); a 4-vector unroll spans exactly 4*VECSZ elements.
        constexpr size_t MW = വലുപ്പം(typename VTraits<Uvec>::lane_type);
        #define PACK_STORE_CMP_RESULT(m0,m1,m2,m3, D) ചെയ്യുക { \
            എങ്കിൽ constexpr (MW == 1u) { \
                v_uint8 o0=v_and(m0,vT),o1=v_and(m1,vT),o2=v_and(m2,vT),o3=v_and(m3,vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); v_store((D)+VECSZ8*2,o2); v_store((D)+VECSZ8*3,o3); \
            } അല്ലെങ്കിൽ എങ്കിൽ constexpr (MW == 2u) { \
                v_uint8 o0=v_and(v_pack(m0,m1),vT), o1=v_and(v_pack(m2,m3),vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); \
            } അല്ലെങ്കിൽ എങ്കിൽ constexpr (MW == 4u) { \
                v_uint8 o0=v_and(v_pack(v_pack(m0,m1),v_pack(m2,m3)),vT); \
                v_store((D),o0); \
            } അല്ലെങ്കിൽ { /* sizeof==8: 4 u64 -> 2 u32 -> 1 u16 -> u8 (low half), 4*VECSZ elems -> store_low */ \
                v_uint16 g=v_pack(v_pack(m0,m1),v_pack(m2,m3)); \
                v_uint8 o0=v_and(v_pack(g,g),vT); \
                v_store_താഴെയുള്ള((D),o0); \
            } } ആകുമ്പോൾ(0)
        എങ്കിൽ (s0x == 1u && s1x == 1u)
        {
            വേണ്ടി (; x < width; x += 4*VECSZ)
            {
                എങ്കിൽ (x + 4*VECSZ > width) { എങ്കിൽ (!tailTrick) നിർത്തുക; x = width - 4*VECSZ; }
                Vvec a0, a1, a2, a3, b0, b1, b2, b3;   // vx_load_pair_as widens f16/bf16->f32, else native
                vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+2*VECSZ, a2, a3);
                vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b1), m1);
                v_reinterpret_as(Cmp::vec(a2, b2), m2); v_reinterpret_as(Cmp::vec(a3, b3), m3);
                PACK_STORE_CMP_RESULT(m0, m1, m2, m3, dst+x);
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s1x == 0u)                       // src1 (e.g. the scalar) broadcast
        {
            Vvec b0; vx_setall_as(src1, b0);
            വേണ്ടി (; x < width; x += 4*VECSZ)
            {
                എങ്കിൽ (x + 4*VECSZ > width) { എങ്കിൽ (!tailTrick) നിർത്തുക; x = width - 4*VECSZ; }
                Vvec a0, a1, a2, a3;
                vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+2*VECSZ, a2, a3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b0), m1);
                v_reinterpret_as(Cmp::vec(a2, b0), m2); v_reinterpret_as(Cmp::vec(a3, b0), m3);
                PACK_STORE_CMP_RESULT(m0, m1, m2, m3, dst+x);
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s0x == 0u)                       // src0 broadcast
        {
            Vvec a0; vx_setall_as(src0, a0);
            വേണ്ടി (; x < width; x += 4*VECSZ)
            {
                എങ്കിൽ (x + 4*VECSZ > width) { എങ്കിൽ (!tailTrick) നിർത്തുക; x = width - 4*VECSZ; }
                Vvec b0, b1, b2, b3;
                vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a0, b1), m1);
                v_reinterpret_as(Cmp::vec(a0, b2), m2); v_reinterpret_as(Cmp::vec(a0, b3), m3);
                PACK_STORE_CMP_RESULT(m0, m1, m2, m3, dst+x);
            }
        }
        #undef PACK_STORE_CMP_RESULT
    #endif
        // Fold the fix-up: raw all-ones/zero mask -> (raw & M) | V. Uniform => raw & trueVal (V=0); a
        // per-channel patch (width=cn<=4 here) indexes M/V by the channel x.
        #define EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                             : (uchar)((cond) ? trueVal : 0))
        എങ്കിൽ (s0x == s1x) {
            വേണ്ടി (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], src1[x]), x);
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s0x == 0) {
            T a = src0[0];
            വേണ്ടി (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, src1[x]), x);
        }
        അല്ലെങ്കിൽ {
            T b = src1[0];
            വേണ്ടി (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], b), x);
        }
        #undef EW_CMP_APPLY
    }
    തിരികെ 0;
}

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
template<typename T, typename Tr, typename Wvec, typename WT, വർഗ്ഗം Op, typename ST=WT, typename Wvec1=Wvec>
സ്ഥിതം പൂർണ്ണസംഖ്യ vec_ദ്വിമാന_Kernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                         സ്ഥിരം ശൂന്യം* src1_, size_t s1y, size_t s1x,
                         സ്ഥിരം ശൂന്യം*, size_t, size_t,
                         ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                         സ്ഥിരം ഇരട്ട* params, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    എങ്കിൽ (width <= 0 || height <= 0) തിരികെ 0;
    s0y /= വലുപ്പം(T);
    s1y /= വലുപ്പം(T);
    dsty /= വലുപ്പം(Tr);

    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);

    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* src1 = (സ്ഥിരം T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    പൂർണ്ണസംഖ്യ y = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    using Wlane = typename VTraits<Wvec>::lane_type;
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Wvec>::vlanes();
    സ്ഥിരം തർക്കം use_tail_trick = width >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    [[maybe_unused]] Wvec vscalar;
    vx_setall_as(&scalar, vscalar);
    എങ്കിൽ (height > 1 && width <= 4 &&
        ((s0y == 0 && s1y == width*s1x) ||
         (s1y == 0 && s0y == width*s0x)) &&
        dsty == (size_t)width) {
        constexpr പൂർണ്ണസംഖ്യ MAXVECSZ = VTraits<Wvec>::max_nlanes;
        Wlane scbuf[MAXVECSZ*6] = {};   // {} for -Wmaybe-uninitialized only (filled up to VECSZ*6)
        സ്ഥിരം പൂർണ്ണസംഖ്യ ewidth = VECSZ*6;
        expandScalar(s0y == 0 ? src0 : src1, s0y == 0 ? s0x : s1x, width, scbuf, ewidth);
        പൂർണ്ണസംഖ്യ dy = ewidth / width;
        Wvec sc0, sc1, sc2, sc3, sc4, sc5;
        sc0 = Op::preproc(vx_load(scbuf), vscalar);
        sc1 = Op::preproc(vx_load(scbuf + VECSZ), vscalar);
        sc2 = Op::preproc(vx_load(scbuf + VECSZ*2), vscalar);
        sc3 = Op::preproc(vx_load(scbuf + VECSZ*3), vscalar);
        sc4 = Op::preproc(vx_load(scbuf + VECSZ*4), vscalar);
        sc5 = Op::preproc(vx_load(scbuf + VECSZ*5), vscalar);

        എങ്കിൽ (s0y == 0) {
            വേണ്ടി (; y + dy <= height; y += dy, src1 += s1y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_load_pair_as(src1, v0, v1);
                vx_load_pair_as(src1 + VECSZ*2, v2, v3);
                vx_load_pair_as(src1 + VECSZ*4, v4, v5);
                auto w0 = Op::vec(sc0, v0, vscalar), w1 = Op::vec(sc1, v1, vscalar),
                     w2 = Op::vec(sc2, v2, vscalar), w3 = Op::vec(sc3, v3, vscalar),
                     w4 = Op::vec(sc4, v4, vscalar), w5 = Op::vec(sc5, v5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
        അല്ലെങ്കിൽ {
            വേണ്ടി (; y + dy <= height; y += dy, src0 += s0y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_load_pair_as(src0, v0, v1);
                vx_load_pair_as(src0 + VECSZ*2, v2, v3);
                vx_load_pair_as(src0 + VECSZ*4, v4, v5);
                auto w0 = Op::vec(v0, sc0, vscalar), w1 = Op::vec(v1, sc1, vscalar),
                     w2 = Op::vec(v2, sc2, vscalar), w3 = Op::vec(v3, sc3, vscalar),
                     w4 = Op::vec(v4, sc4, vscalar), w5 = Op::vec(v5, sc5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
    }
#endif
    വേണ്ടി (; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        പൂർണ്ണസംഖ്യ x = 0;
    #എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        Wvec a0, a1, a2, a3, b0, b1, b2, b3;
        എങ്കിൽ (s0x == s1x) {
            എങ്കിൽ (!Op::useScalar || scalar == ST(1)) {
                // this branch runs on Wvec1, which may be WIDER-laned than Wvec (e.g. the u8 mul
                // path: Wvec1=v_uint16 is 2x the lanes of Wvec=v_float32) - step by ITS lane count,
                // or the pairs overlap and the tail backoff writes past the row end
                സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ1 = VTraits<Wvec1>::vlanes();
                സ്ഥിരം തർക്കം tail_trick1 = width >= VECSZ1*4 && src0_ != dst_ && src1_ != dst_;
                വേണ്ടി (; x < width; x += VECSZ1*4) {
                    Wvec1 a0_, a1_, a2_, a3_, b0_, b1_, b2_, b3_;
                    എങ്കിൽ (x + VECSZ1*4 > width) { എങ്കിൽ (!tail_trick1) നിർത്തുക; x = width - VECSZ1*4; }
                    vx_load_pair_as(src0 + x, a0_, a1_);
                    vx_load_pair_as(src0 + x + VECSZ1*2, a2_, a3_);
                    vx_load_pair_as(src1 + x, b0_, b1_);
                    vx_load_pair_as(src1 + x + VECSZ1*2, b2_, b3_);
                    auto c0 = Op::vec(a0_, b0_), c1 = Op::vec(a1_, b1_),
                         c2 = Op::vec(a2_, b2_), c3 = Op::vec(a3_, b3_);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ1*2, c2, c3);
                }
            }
            അല്ലെങ്കിൽ {
                വേണ്ടി (; x < width; x += VECSZ*4) {
                    എങ്കിൽ (x + VECSZ*4 > width) { എങ്കിൽ (!use_tail_trick) നിർത്തുക; x = width - VECSZ*4; }
                    vx_load_pair_as(src0 + x, a0, a1);
                    vx_load_pair_as(src0 + x + VECSZ*2, a2, a3);
                    vx_load_pair_as(src1 + x, b0, b1);
                    vx_load_pair_as(src1 + x + VECSZ*2, b2, b3);
                    auto c0 = Op::vec(Op::preproc(a0, vscalar), b0, vscalar),
                         c1 = Op::vec(Op::preproc(a1, vscalar), b1, vscalar),
                         c2 = Op::vec(Op::preproc(a2, vscalar), b2, vscalar),
                         c3 = Op::vec(Op::preproc(a3, vscalar), b3, vscalar);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ*2, c2, c3);
                }
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (s1x == 0) {
            vx_setall_as(src1, b0);
            b0 = Op::preproc(b0, vscalar);
            വേണ്ടി (; x < width; x += VECSZ*4) {
                എങ്കിൽ (x + VECSZ*4 > width) { എങ്കിൽ (!use_tail_trick) നിർത്തുക; x = width - VECSZ*4; }
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src0 + x + VECSZ*2, a2, a3);
                auto c0 = Op::vec(a0, b0, vscalar), c1 = Op::vec(a1, b0, vscalar),
                     c2 = Op::vec(a2, b0, vscalar), c3 = Op::vec(a3, b0, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
        അല്ലെങ്കിൽ {
            vx_setall_as(src0, b0);
            b0 = Op::preproc(b0, vscalar);
            വേണ്ടി (; x < width; x += VECSZ*4) {
                എങ്കിൽ (x + VECSZ*4 > width) { എങ്കിൽ (!use_tail_trick) നിർത്തുക; x = width - VECSZ*4; }
                vx_load_pair_as(src1 + x, a0, a1);
                vx_load_pair_as(src1 + x + VECSZ*2, a2, a3);
                auto c0 = Op::vec(b0, a0, vscalar), c1 = Op::vec(b0, a1, vscalar),
                     c2 = Op::vec(b0, a2, vscalar), c3 = Op::vec(b0, a3, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
    #endif
        വേണ്ടി (; x < width; x++)
            dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x*s0x], (WT)src1[x*s1x], scalar));
    }
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    vx_cleanup();
#endif
    തിരികെ 0;
}

// ===========================================================================
// OP_ADD
// ===========================================================================

// add dispatch: T x T -> R (operands already same depth T).
//  - native saturating path (Wvec = native, use_simd=true) for T -> T on <=32-bit ints + f32
//    (32-bit lanes via the local v_add_sat/v_sub_sat - v_add/v_sub wrap there);
//  - widening f32-hub (Wvec = v_float32, use_simd=true) for the widened/half/float outputs;
//  - scalar (use_simd=false) for 64-bit results and f64 (no vector path yet).
template<വർഗ്ഗം Op>
TKernel getAddSubFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)
{
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_8U:
        fptr = R == CV_8U ? vec_ദ്വിമാന_Kernel<uchar, uchar, v_uint8, short, Op, uchar> :
               R == CV_16S ? vec_ദ്വിമാന_Kernel<uchar, short, v_int16, short, Op, short> :
               R == CV_32S ? vec_ദ്വിമാന_Kernel<uchar, പൂർണ്ണസംഖ്യ, v_int16, short, Op, short> :
               R == CV_32F ? vec_ദ്വിമാന_Kernel<uchar, ദശാംശം, v_int16, short, Op, short> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_8S:
        fptr =
            R == CV_8S ? vec_ദ്വിമാന_Kernel<schar, schar, v_int8, short, Op, schar> :
            R == CV_16S ? vec_ദ്വിമാന_Kernel<schar, short, v_int16, short, Op, short> :
            R == CV_32S ? vec_ദ്വിമാന_Kernel<schar, പൂർണ്ണസംഖ്യ, v_int16, short, Op, short> :
            R == CV_32F ? vec_ദ്വിമാന_Kernel<schar, ദശാംശം, v_int16, short, Op, short> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_16U:
        fptr = R == CV_16U ? vec_ദ്വിമാന_Kernel<ushort, ushort, v_uint16, പൂർണ്ണസംഖ്യ, Op, ushort> :
               R == CV_32S ? vec_ദ്വിമാന_Kernel<ushort, പൂർണ്ണസംഖ്യ, v_int32, പൂർണ്ണസംഖ്യ, Op, പൂർണ്ണസംഖ്യ> :
               R == CV_32F ? vec_ദ്വിമാന_Kernel<ushort, ദശാംശം, v_int32, പൂർണ്ണസംഖ്യ, Op, പൂർണ്ണസംഖ്യ> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_16S:
        fptr = R == CV_16S ? vec_ദ്വിമാന_Kernel<short, short, v_int16, പൂർണ്ണസംഖ്യ, Op, short> :
               R == CV_32S ? vec_ദ്വിമാന_Kernel<short, പൂർണ്ണസംഖ്യ,   v_int32, പൂർണ്ണസംഖ്യ, Op, പൂർണ്ണസംഖ്യ> :
               R == CV_32F ? vec_ദ്വിമാന_Kernel<short, ദശാംശം, v_int32, പൂർണ്ണസംഖ്യ, Op, പൂർണ്ണസംഖ്യ> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_32U:
        fptr = R == CV_32U ? vec_ദ്വിമാന_Kernel<unsigned, unsigned, v_uint32, int64_t, Op, unsigned> :
                R == CV_64S ? scalar_ദ്വിമാന_Kernel<unsigned, int64_t, int64_t, Op> :
                R == CV_64F ? scalar_ദ്വിമാന_Kernel<unsigned, ഇരട്ട, int64_t, Op> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_32S:
        fptr = R == CV_32S ? vec_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, v_int32, int64_t, Op, പൂർണ്ണസംഖ്യ> :
               R == CV_64S ? scalar_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, ഇരട്ട, int64_t, Op> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_64U:
        fptr = R == CV_64U ? scalar_ദ്വിമാന_Kernel<uint64_t, uint64_t, uint64_t, Op> :
               R == CV_64F ? scalar_ദ്വിമാന_Kernel<uint64_t, ഇരട്ട, ഇരട്ട, Op> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_64S:
        fptr = R == CV_64S ? scalar_ദ്വിമാന_Kernel<int64_t, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_ദ്വിമാന_Kernel<int64_t, ഇരട്ട, ഇരട്ട, Op> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_16F:
        fptr =
        #എങ്കിൽ CV_SIMD_16F
        R == CV_16F ? vec_ദ്വിമാന_Kernel<hfloat, hfloat, v_float16, ദശാംശം, Op, hfloat> :
        #അല്ലെങ്കിൽ
        R == CV_16F ? vec_ദ്വിമാന_Kernel<hfloat, hfloat, v_float32, ദശാംശം, Op, ദശാംശം> :
        #endif
        R == CV_32F ? vec_ദ്വിമാന_Kernel<hfloat, ദശാംശം, v_float32, ദശാംശം, Op, ദശാംശം> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_16BF:
        fptr =
            R == CV_16BF ? vec_ദ്വിമാന_Kernel<bfloat, bfloat, v_float32, ദശാംശം, Op, ദശാംശം> :
            R == CV_32F ? vec_ദ്വിമാന_Kernel<bfloat, ദശാംശം,  v_float32, ദശാംശം, Op, ദശാംശം> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_32F:
        fptr = R == CV_32F ? vec_ദ്വിമാന_Kernel<ദശാംശം, ദശാംശം, v_float32, ദശാംശം, Op, ദശാംശം> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_64F:
        #എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, v_float64, ഇരട്ട, Op> : nullptr;
        #അല്ലെങ്കിൽ
        fptr = R == CV_64F ? scalar_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, ഇരട്ട, Op> : nullptr;
        #endif
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        ;
    }
    തിരികെ {fptr, nullptr, 0};
}

TKernel getMulFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)
{
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_8U:
        fptr =   // scale==1 fast path: whole u8 registers, v_mul_sat widens+clamps inside; Wvec=f32 for scale
            R == CV_8U ? vec_ദ്വിമാന_Kernel<uchar, uchar, v_float32, ദശാംശം, EwMul, ദശാംശം, v_uint8> :
            R == CV_32F ? vec_ദ്വിമാന_Kernel<uchar, ദശാംശം, v_float32, ദശാംശം, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_8S:
        fptr =   // scale==1 fast path: whole s8 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_8S ? vec_ദ്വിമാന_Kernel<schar, schar, v_float32, ദശാംശം, EwMul, ദശാംശം, v_int8> :
            R == CV_32F ? vec_ദ്വിമാന_Kernel<schar, ദശാംശം, v_float32, ദശാംശം, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_16U:
        fptr =   // scale==1 fast path: whole u16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16U ? vec_ദ്വിമാന_Kernel<ushort, ushort, v_float32, ദശാംശം, EwMul, ദശാംശം, v_uint16> :
            R == CV_32F ? vec_ദ്വിമാന_Kernel<ushort, ദശാംശം,  v_float32, ദശാംശം, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_16S:
        fptr =   // scale==1 fast path: whole s16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16S ? vec_ദ്വിമാന_Kernel<short, short, v_float32, ദശാംശം, EwMul, ദശാംശം, v_int16> :
            R == CV_32F ? vec_ദ്വിമാന_Kernel<short, ദശാംശം, v_float32, ദശാംശം, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_16F:
        fptr =
            R == CV_16F ? vec_ദ്വിമാന_Kernel<hfloat, hfloat, v_float32, ദശാംശം, EwMul> :
            R == CV_32F ? vec_ദ്വിമാന_Kernel<hfloat, ദശാംശം,  v_float32, ദശാംശം, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_16BF:
        fptr =
            R == CV_16BF ? vec_ദ്വിമാന_Kernel<bfloat, bfloat, v_float32, ദശാംശം, EwMul> :
            R == CV_32F ? vec_ദ്വിമാന_Kernel<bfloat, ദശാംശം,  v_float32, ദശാംശം, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_32F:
        fptr = R == CV_32F ? vec_ദ്വിമാന_Kernel<ദശാംശം, ദശാംശം, v_float32, ദശാംശം, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_32U:
        fptr =   // scale==1 fast path (where v_mul_sat32 exists): whole u32 registers, widening
                 // multiply + saturating narrow; the f64 work vector serves the scale path
        #എങ്കിൽ defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32U ? vec_ദ്വിമാന_Kernel<unsigned, unsigned, v_float64, ഇരട്ട, EwMul, ഇരട്ട, v_uint32> :
            R == CV_64F ? vec_ദ്വിമാന_Kernel<unsigned, ഇരട്ട,   v_float64, ഇരട്ട, EwMul> : nullptr;
        #അല്ലെങ്കിലെങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32U ? vec_ദ്വിമാന_Kernel<unsigned, unsigned, v_float64, ഇരട്ട, EwMul> :
            R == CV_64F ? vec_ദ്വിമാന_Kernel<unsigned, ഇരട്ട,   v_float64, ഇരട്ട, EwMul> : nullptr;
        #അല്ലെങ്കിൽ
            R == CV_32U ? scalar_ദ്വിമാന_Kernel<unsigned, unsigned, ഇരട്ട, EwMul> :
            R == CV_64F ? scalar_ദ്വിമാന_Kernel<unsigned, ഇരട്ട,   ഇരട്ട, EwMul> : nullptr;
        #endif
        നിർത്തുക;
    അവസ്ഥ CV_32S:
        fptr =   // scale==1 fast path: see CV_32U
        #എങ്കിൽ defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32S ? vec_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ,    v_float64, ഇരട്ട, EwMul, ഇരട്ട, v_int32> :
            R == CV_64F ? vec_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, ഇരട്ട, v_float64, ഇരട്ട, EwMul> : nullptr;
        #അല്ലെങ്കിലെങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32S ? vec_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ,    v_float64, ഇരട്ട, EwMul> :
            R == CV_64F ? vec_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, ഇരട്ട, v_float64, ഇരട്ട, EwMul> : nullptr;
        #അല്ലെങ്കിൽ
            R == CV_32S ? scalar_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ,    ഇരട്ട, EwMul> :
            R == CV_64F ? scalar_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, ഇരട്ട, ഇരട്ട, EwMul> : nullptr;
        #endif
        നിർത്തുക;
    അവസ്ഥ CV_64U:
        fptr = R == CV_64F ? scalar_ദ്വിമാന_Kernel<uint64_t, ഇരട്ട, ഇരട്ട, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_64S:
        fptr = R == CV_64F ? scalar_ദ്വിമാന_Kernel<int64_t,  ഇരട്ട, ഇരട്ട, EwMul> : nullptr;
        നിർത്തുക;
    അവസ്ഥ CV_64F:
        #എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, v_float64, ഇരട്ട, EwMul> : nullptr;
        #അല്ലെങ്കിൽ
        fptr = R == CV_64F ? scalar_ദ്വിമാന_Kernel<ഇരട്ട,   ഇരട്ട, ഇരട്ട, EwMul> : nullptr;
        #endif
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        ;
    }
    തിരികെ {fptr, nullptr, 0};
}

// `checked` (decided by the CALLER from the ORIGINAL input types) selects the divide-by-zero
// policy, INDEPENDENT of the work type R: EwDivInt guards /0 -> 0 for integer-semantics division,
// EwDivFlt does not (a/0 -> inf, saturating on a later cast, as cv::divide does for float inputs).
// It must apply on EVERY row - two wide integers (e.g. 32U / 64S) promote to a 64F work type yet
// still need the integer guard, so the float-work rows can't hardcode EwDivFlt.
TKernel getDivFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R, തർക്കം checked)
{
    KernelFunc fptr = nullptr;
    // SIMD via vecBinaryKernel: div scales the numerator in vec (a*scale/b), EwDivFlt (float a/0 -> inf,
    // saturates on cast) / EwDivInt (v_select guards b==0 -> 0 post-facto). Prefer a DIRECT T->T kernel
    // (1 pass, saturate on store) - emitBinary probes result==T first; a T->f32/f64 kernel serves an
    // explicit float dtype. Work type: f32 for <=16-bit/f16/bf16/f32; f64 for 32/64-bit.
    #define DIV(T_, Tr_, Wv_, W_) (checked ? vec_ദ്വിമാന_Kernel<T_, Tr_, Wv_, W_, EwDivInt> \
                                           : vec_ദ്വിമാന_Kernel<T_, Tr_, Wv_, W_, EwDivFlt>)
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #define DIVW(T_, Tr_) DIV(T_, Tr_, v_float64, ഇരട്ട)                 // 32/64-bit, f64 SIMD
#അല്ലെങ്കിൽ
    #define DIVW(T_, Tr_) (checked ? scalar_ദ്വിമാന_Kernel<T_, Tr_, ഇരട്ട, EwDivInt> \
                                    : scalar_ദ്വിമാന_Kernel<T_, Tr_, ഇരട്ട, EwDivFlt>)
#endif
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_8U:   fptr = R==CV_8U  ? DIV(uchar,  uchar,  v_float32, ദശാംശം) : R==CV_32F ? DIV(uchar,  ദശാംശം, v_float32, ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_8S:   fptr = R==CV_8S  ? DIV(schar,  schar,  v_float32, ദശാംശം) : R==CV_32F ? DIV(schar,  ദശാംശം, v_float32, ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16U:  fptr = R==CV_16U ? DIV(ushort, ushort, v_float32, ദശാംശം) : R==CV_32F ? DIV(ushort, ദശാംശം, v_float32, ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16S:  fptr = R==CV_16S ? DIV(short,  short,  v_float32, ദശാംശം) : R==CV_32F ? DIV(short,  ദശാംശം, v_float32, ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16F:  fptr = R==CV_16F ? DIV(hfloat, hfloat, v_float32, ദശാംശം) : R==CV_32F ? DIV(hfloat, ദശാംശം, v_float32, ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16BF: fptr = R==CV_16BF? DIV(bfloat, bfloat, v_float32, ദശാംശം) : R==CV_32F ? DIV(bfloat, ദശാംശം, v_float32, ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32F:  fptr = R==CV_32F ? DIV(ദശാംശം,  ദശാംശം,  v_float32, ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32U:  fptr = R==CV_32U ? DIVW(unsigned, unsigned) : R==CV_64F ? DIVW(unsigned, ഇരട്ട) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32S:  fptr = R==CV_32S ? DIVW(പൂർണ്ണസംഖ്യ,      പൂർണ്ണസംഖ്യ)      : R==CV_64F ? DIVW(പൂർണ്ണസംഖ്യ,      ഇരട്ട) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_64U:  fptr = R==CV_64F ? DIVW(uint64_t, ഇരട്ട) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_64S:  fptr = R==CV_64F ? DIVW(int64_t,  ഇരട്ട) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_64F:  fptr = R==CV_64F ? DIVW(ഇരട്ട,   ഇരട്ട) : nullptr; നിർത്തുക;
    സ്ഥിരസ്ഥിതി:      ;
    }
    #undef DIV
    #undef DIVW
    തിരികെ {fptr, nullptr, 0};
}


// min / max: T x T -> T for every depth. Native v_min/v_max on the matching lane type (8/16/32-bit
// ints, f16/bf16/f32); 64-bit ints use the scalar path. Op = EwMin or EwMax.
template<വർഗ്ഗം Op>
സ്ഥിതം TKernel getMinMaxFunc(പൂർണ്ണസംഖ്യ T)
{
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_8U:   fptr = vec_ദ്വിമാന_Kernel<uchar,    uchar,    v_uint8,   short,   Op, uchar>; നിർത്തുക;
    അവസ്ഥ CV_8S:   fptr = vec_ദ്വിമാന_Kernel<schar,    schar,    v_int8,    short,   Op, schar>; നിർത്തുക;
    അവസ്ഥ CV_16U:  fptr = vec_ദ്വിമാന_Kernel<ushort,   ushort,   v_uint16,  പൂർണ്ണസംഖ്യ,     Op, ushort>; നിർത്തുക;
    അവസ്ഥ CV_16S:  fptr = vec_ദ്വിമാന_Kernel<short,    short,    v_int16,   പൂർണ്ണസംഖ്യ,     Op, short>; നിർത്തുക;
    അവസ്ഥ CV_32U:  fptr = vec_ദ്വിമാന_Kernel<unsigned, unsigned, v_uint32,  int64_t, Op, unsigned>; നിർത്തുക;
    അവസ്ഥ CV_32S:  fptr = vec_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ,      പൂർണ്ണസംഖ്യ,      v_int32,   int64_t, Op, പൂർണ്ണസംഖ്യ>; നിർത്തുക;
    അവസ്ഥ CV_16F:
        #എങ്കിൽ CV_SIMD_16F
        fptr = vec_ദ്വിമാന_Kernel<hfloat,   hfloat,   v_float16, ദശാംശം,   Op, hfloat>;
        #അല്ലെങ്കിൽ
        fptr = vec_ദ്വിമാന_Kernel<hfloat,   hfloat,   v_float32, ദശാംശം,   Op, ദശാംശം>;
        #endif
        നിർത്തുക;
    അവസ്ഥ CV_16BF: fptr = vec_ദ്വിമാന_Kernel<bfloat, bfloat, v_float32, ദശാംശം, Op, ദശാംശം>; നിർത്തുക;
    അവസ്ഥ CV_32F:  fptr = vec_ദ്വിമാന_Kernel<ദശാംശം, ദശാംശം, v_float32, ദശാംശം, Op, ദശാംശം>; നിർത്തുക;
    അവസ്ഥ CV_64U:  fptr = scalar_ദ്വിമാന_Kernel<uint64_t, uint64_t, uint64_t, Op>; നിർത്തുക;
    അവസ്ഥ CV_64S:  fptr = scalar_ദ്വിമാന_Kernel<int64_t,  int64_t,  int64_t,  Op>; നിർത്തുക;
    #എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    അവസ്ഥ CV_64F:  fptr = vec_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, v_float64, ഇരട്ട, Op, ഇരട്ട>; നിർത്തുക;
    #അല്ലെങ്കിൽ
    അവസ്ഥ CV_64F:  fptr = scalar_ദ്വിമാന_Kernel<ഇരട്ട,   ഇരട്ട,   ഇരട്ട,   Op>; നിർത്തുക;
    #endif
    സ്ഥിരസ്ഥിതി:      ;
    }
    തിരികെ {fptr, nullptr, 0};
}

// absdiff: |a-b|, T x T -> R.
TKernel getAbsdiffFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)
{
    // Signed T has TWO outputs: R==T -> EwAbsdiffS (saturating |a-b| kept in the signed range, native work,
    // ONE pass - the depth cv::absdiff keeps); R==(the unsigned type of the same width) -> EwAbsdiff (the
    // true |a-b| via v_absdiff, for an explicit unsigned dst). Unsigned/float T only produce R==T. Each case
    // returns nullptr for a wrong R (no separate rdepth guard).
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_8U:   fptr = R == CV_8U  ? vec_ദ്വിമാന_Kernel<uchar,  uchar,  v_uint8,  short,   EwAbsdiff, uchar>  : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16U:  fptr = R == CV_16U ? vec_ദ്വിമാന_Kernel<ushort, ushort, v_uint16, പൂർണ്ണസംഖ്യ,     EwAbsdiff, ushort> : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32U:  fptr = R == CV_32U ? vec_ദ്വിമാന_Kernel<unsigned, unsigned, v_uint32, int64_t, EwAbsdiff, unsigned> : nullptr; നിർത്തുക;
    അവസ്ഥ CV_8S:   fptr = R == CV_8S  ? vec_ദ്വിമാന_Kernel<schar, schar, v_int8,  short,   EwAbsdiffS, schar> :
                         R == CV_8U  ? vec_ദ്വിമാന_Kernel<schar, uchar, v_int8,  short,   EwAbsdiff,  schar> : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16S:  fptr = R == CV_16S ? vec_ദ്വിമാന_Kernel<short, short, v_int16, പൂർണ്ണസംഖ്യ,     EwAbsdiffS, short> :
                         R == CV_16U ? vec_ദ്വിമാന_Kernel<short, ushort, v_int16, പൂർണ്ണസംഖ്യ,     EwAbsdiff,  short> : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32S:  fptr = R == CV_32S ? vec_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ,   പൂർണ്ണസംഖ്യ,   v_int32, int64_t, EwAbsdiffS, പൂർണ്ണസംഖ്യ> :
                         R == CV_32U ? vec_ദ്വിമാന_Kernel<പൂർണ്ണസംഖ്യ, unsigned, v_int32, int64_t, EwAbsdiff,  പൂർണ്ണസംഖ്യ> : nullptr; നിർത്തുക;
    #എങ്കിൽ CV_SIMD_16F
    അവസ്ഥ CV_16F:  fptr = R == CV_16F ? vec_ദ്വിമാന_Kernel<hfloat, hfloat, v_float16, ദശാംശം, EwAbsdiff, hfloat> : nullptr; നിർത്തുക;
    #അല്ലെങ്കിൽ
    അവസ്ഥ CV_16F:  fptr = R == CV_16F ? vec_ദ്വിമാന_Kernel<hfloat, hfloat, v_float32, ദശാംശം, EwAbsdiff, ദശാംശം>  : nullptr; നിർത്തുക;
    #endif
    അവസ്ഥ CV_16BF: fptr = R == CV_16BF ? vec_ദ്വിമാന_Kernel<bfloat, bfloat, v_float32, ദശാംശം, EwAbsdiff, ദശാംശം> : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32F:  fptr = R == CV_32F  ? vec_ദ്വിമാന_Kernel<ദശാംശം,  ദശാംശം,  v_float32, ദശാംശം, EwAbsdiff, ദശാംശം> : nullptr; നിർത്തുക;
    അവസ്ഥ CV_64U:  fptr = R == CV_64U  ? scalar_ദ്വിമാന_Kernel<uint64_t, uint64_t, uint64_t, EwAbsdiff> : nullptr; നിർത്തുക;
    അവസ്ഥ CV_64S:  fptr = R == CV_64S  ? scalar_ദ്വിമാന_Kernel<int64_t,  int64_t,  int64_t,  EwAbsdiffS> :
                         R == CV_64U  ? scalar_ദ്വിമാന_Kernel<int64_t,  uint64_t, int64_t,  EwAbsdiff> : nullptr; നിർത്തുക;
    #എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    അവസ്ഥ CV_64F:  fptr = R == CV_64F  ? vec_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, v_float64, ഇരട്ട, EwAbsdiff, ഇരട്ട> : nullptr; നിർത്തുക;
    #അല്ലെങ്കിൽ
    അവസ്ഥ CV_64F:  fptr = R == CV_64F  ? scalar_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, ഇരട്ട, EwAbsdiff> : nullptr; നിർത്തുക;
    #endif
    സ്ഥിരസ്ഥിതി:      ;
    }
    തിരികെ {fptr, nullptr, 0};
}

// compare: T x T -> u8 mask. Directly-comparable depths (u8/s8/u16/s16/u32/s32/f32) take the SIMD
// vecCompareKernel; f16/bf16 and 64-bit int depths fall back to scalarCompareKernel. The returned kernel
// defaults to a 255 mask in TKernel::flags (cv::compare-compatible); kernel.flags=1 gives a 0/1 mask.
template<വർഗ്ഗം Cmp>
സ്ഥിതം KernelFunc compareByType(പൂർണ്ണസംഖ്യ T)
{
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_8U:   തിരികെ vecCompareKernel<uchar,    v_uint8,   v_uint8,  Cmp>;
    അവസ്ഥ CV_8S:   തിരികെ vecCompareKernel<schar,    v_int8,    v_uint8,  Cmp>;
    അവസ്ഥ CV_16U:  തിരികെ vecCompareKernel<ushort,   v_uint16,  v_uint16, Cmp>;
    അവസ്ഥ CV_16S:  തിരികെ vecCompareKernel<short,    v_int16,   v_uint16, Cmp>;
    അവസ്ഥ CV_32U:  തിരികെ vecCompareKernel<unsigned, v_uint32,  v_uint32, Cmp>;
    അവസ്ഥ CV_32S:  തിരികെ vecCompareKernel<പൂർണ്ണസംഖ്യ,      v_int32,   v_uint32, Cmp>;
    അവസ്ഥ CV_32F:  തിരികെ vecCompareKernel<ദശാംശം,    v_float32, v_uint32, Cmp>;
    #എങ്കിൽ CV_SIMD_16F
    അവസ്ഥ CV_16F:  തിരികെ vecCompareKernel<hfloat,   v_float16, v_uint16, Cmp>;
    #അല്ലെങ്കിൽ
    അവസ്ഥ CV_16F:  തിരികെ vecCompareKernel<hfloat,   v_float32, v_uint32, Cmp>;   // widen f16->f32
    #endif
    അവസ്ഥ CV_16BF: തിരികെ vecCompareKernel<bfloat,   v_float32, v_uint32, Cmp>;   // widen bf16->f32 (no native)
    അവസ്ഥ CV_64U:  തിരികെ scalarCompareKernel<uint64_t, uint64_t, Cmp>;
    അവസ്ഥ CV_64S:  തിരികെ scalarCompareKernel<int64_t,  int64_t,  Cmp>;
    #എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    അവസ്ഥ CV_64F:  തിരികെ vecCompareKernel<ഇരട്ട,   v_float64, v_uint64, Cmp>;
    #അല്ലെങ്കിൽ
    അവസ്ഥ CV_64F:  തിരികെ scalarCompareKernel<ഇരട്ട,   ഇരട്ട,   Cmp>;
    #endif
    സ്ഥിരസ്ഥിതി:      തിരികെ nullptr;
    }
}

TKernel getCmpFunc_(മുകൾ op, പൂർണ്ണസംഖ്യ T)
{
    // Only 4 physical kernels (eq/ne/gt/ge): LT/LE reuse GT/GE with the operands swapped
    // (a<b == b>a, a<=b == b>=a) via the EW_KERNEL_SWAP01 flag, honored by the executor.
    KernelFunc f = nullptr;
    പൂർണ്ണസംഖ്യ flags = 0;              // mask value: 0 flag bits => 0/255 (cv::compare); EW_KERNEL_MASK1 => 0/1
    തിരഞ്ഞെടുപ്പ് (op)
    {
    അവസ്ഥ OP_CMP_EQ: f = compareByType<EwCmpEq>(T); നിർത്തുക;
    അവസ്ഥ OP_CMP_NE: f = compareByType<EwCmpNe>(T); നിർത്തുക;
    അവസ്ഥ OP_CMP_GT: f = compareByType<EwCmpGt>(T); നിർത്തുക;
    അവസ്ഥ OP_CMP_GE: f = compareByType<EwCmpGe>(T); നിർത്തുക;
    അവസ്ഥ OP_CMP_LT: f = compareByType<EwCmpGt>(T); flags = EW_KERNEL_SWAP01; നിർത്തുക;
    അവസ്ഥ OP_CMP_LE: f = compareByType<EwCmpGe>(T); flags = EW_KERNEL_SWAP01; നിർത്തുക;
    സ്ഥിരസ്ഥിതി:        ;
    }
    തിരികെ {f, nullptr, flags};
}

// ===========================================================================
// OP_AND / OP_OR / OP_XOR / OP_NOT: bitwise, type-agnostic (by element size)
// ===========================================================================
// A bit-pattern op ignores the operand's semantic type, so we run it on the UNSIGNED integer whose
// width matches the element (1/2/4/8 bytes). One functor set (EwAnd/EwOr/EwXor) times four widths
// covers every depth; the dispatchers below pick by element size. AND/OR/XOR reuse vecBinaryKernel's
// native same-type path (as min/max do); 64-bit falls to the scalar kernel. NOT is unary.

// bitwise NOT: ~x. Single operand -> always a full contiguous array (no broadcast), so just a flat
// per-row complement. SIMD for 1/2/4-byte elements; 8-byte uses the scalar tail (Vvec unused there).
template<typename T, typename Vvec>
സ്ഥിതം പൂർണ്ണസംഖ്യ notKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                     സ്ഥിരം ശൂന്യം*, size_t, size_t, സ്ഥിരം ശൂന്യം*, size_t, size_t,
                     ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                     സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    എങ്കിൽ (width <= 0 || height <= 0) തിരികെ 0;
    s0y /= വലുപ്പം(T);
    dsty /= വലുപ്പം(T);
    CV_Assert(s0x == 1u || width == 1);
    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    T* dst = (T*)dst_;
    എങ്കിൽ (height > 1 && dsty == (size_t)width && s0y == (size_t)width) { width *= height; height = 1; }
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < height; y++, src0 += s0y, dst += dsty)
    {
        പൂർണ്ണസംഖ്യ x = 0;
    #എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        എങ്കിൽ constexpr (വലുപ്പം(T) <= 4)
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Vvec>::vlanes();
            വേണ്ടി (; x <= width - VECSZ; x += VECSZ)
                v_store(dst + x, v_not(vx_load(src0 + x)));
        }
    #endif
        വേണ്ടി (; x < width; x++) dst[x] = (T)~src0[x];
    }
    തിരികെ 0;
}

template<വർഗ്ഗം Op>
സ്ഥിതം KernelFunc bitwiseByEsz(പൂർണ്ണസംഖ്യ esz)
{
    തിരഞ്ഞെടുപ്പ് (esz)
    {
    അവസ്ഥ 1: തിരികെ vec_ദ്വിമാന_Kernel<uint8_t,  uint8_t,  v_uint8,  uint8_t,  Op, uint8_t>;
    അവസ്ഥ 2: തിരികെ vec_ദ്വിമാന_Kernel<uint16_t, uint16_t, v_uint16, uint16_t, Op, uint16_t>;
    അവസ്ഥ 4: തിരികെ vec_ദ്വിമാന_Kernel<uint32_t, uint32_t, v_uint32, uint32_t, Op, uint32_t>;
    അവസ്ഥ 8: തിരികെ scalar_ദ്വിമാന_Kernel<uint64_t, uint64_t, uint64_t, Op>;
    സ്ഥിരസ്ഥിതി: തിരികെ nullptr;
    }
}

TKernel getBitwiseFunc_(മുകൾ op, പൂർണ്ണസംഖ്യ esz)
{
    KernelFunc f = nullptr;
    തിരഞ്ഞെടുപ്പ് (op)
    {
    അവസ്ഥ OP_AND: f = bitwiseByEsz<EwAnd>(esz); നിർത്തുക;
    അവസ്ഥ OP_OR:  f = bitwiseByEsz<EwOr >(esz); നിർത്തുക;
    അവസ്ഥ OP_XOR: f = bitwiseByEsz<EwXor>(esz); നിർത്തുക;
    സ്ഥിരസ്ഥിതി:     ;
    }
    തിരികെ {f, nullptr, 0};
}

TKernel getNotFunc_(പൂർണ്ണസംഖ്യ esz)
{
    KernelFunc f = nullptr;
    തിരഞ്ഞെടുപ്പ് (esz)
    {
    അവസ്ഥ 1: f = notKernel<uint8_t,  v_uint8 >; നിർത്തുക;
    അവസ്ഥ 2: f = notKernel<uint16_t, v_uint16>; നിർത്തുക;
    അവസ്ഥ 4: f = notKernel<uint32_t, v_uint32>; നിർത്തുക;
    അവസ്ഥ 8: f = notKernel<uint64_t, v_uint32>; നിർത്തുക;   // SIMD path compiled out for 8-byte -> scalar ~
    സ്ഥിരസ്ഥിതി: ;
    }
    തിരികെ {f, nullptr, 0};
}

// ===========================================================================
// OP_ADDW (addWeighted): dst = a*alpha + b*beta + gamma, params[0..2] = {alpha, beta, gamma}. Two fused
// v_fma in the work type Wvec - f32 SIMD for u8/s8/u16/s16/f16/bf16/f32; the 32-bit-int/64-bit group
// works in f64 (v_float64 SIMD under CV_SIMD_64F || CV_SIMD_SCALABLE_64F, else use_simd=false scalar). Like vecBinaryKernel but
// WITHOUT its multi-channel short-row
// branch: addWeighted takes plain scalar coefficients (a multi-channel scalar is not optimized, matching
// the classic function). The broadcast branches fold the constant operand's contribution once.
// ===========================================================================
template<typename T, typename Tr, typename Wvec, typename WT, തർക്കം use_simd>
സ്ഥിതം പൂർണ്ണസംഖ്യ addWeightedKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                             സ്ഥിരം ശൂന്യം* src1_, size_t s1y, size_t s1x,
                             സ്ഥിരം ശൂന്യം*, size_t, size_t,
                             ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                             സ്ഥിരം ഇരട്ട* params, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    എങ്കിൽ (width <= 0 || height <= 0) തിരികെ 0;
    s0y /= വലുപ്പം(T); s1y /= വലുപ്പം(T); dsty /= വലുപ്പം(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);
    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* src1 = (സ്ഥിരം T*)src1_;
    Tr* dst = (Tr*)dst_;
    സ്ഥിരം WT alpha = (WT)params[0], beta = (WT)params[1], gamma = (WT)params[2];
    EW_TRY_COLLAPSE(2);
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    Wvec va{}, vb{}, vg{};
    സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Wvec>::vlanes();
    സ്ഥിരം തർക്കം tail = width >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    എങ്കിൽ constexpr (use_simd) {
        WT fa=alpha, fb=beta, fg=gamma;
        vx_setall_as(&fa, va); vx_setall_as(&fb, vb); vx_setall_as(&fg, vg);
    }
#endif
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        പൂർണ്ണസംഖ്യ x = 0;
    #എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        എങ്കിൽ constexpr (use_simd)
        {
            Wvec a0,a1,a2,a3,b0,b1,b2,b3;
            എങ്കിൽ (s0x == s1x) {                                // both arrays contiguous
                വേണ്ടി (; x < width; x += VECSZ*4) {
                    എങ്കിൽ (x+VECSZ*4 > width) { എങ്കിൽ (!tail) നിർത്തുക; x = width-VECSZ*4; }
                    vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+VECSZ*2, a2, a3);
                    vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+VECSZ*2, b2, b3);
                    a0=v_fma(a0,va,v_fma(b0,vb,vg)); a1=v_fma(a1,va,v_fma(b1,vb,vg));
                    a2=v_fma(a2,va,v_fma(b2,vb,vg)); a3=v_fma(a3,va,v_fma(b3,vb,vg));
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            അല്ലെങ്കിൽ എങ്കിൽ (s1x == 0) {                             // src1 broadcast: b*beta+gamma is constant
                Wvec bb; vx_setall_as(src1, bb); Wvec bc = v_fma(bb, vb, vg);
                വേണ്ടി (; x < width; x += VECSZ*4) {
                    എങ്കിൽ (x+VECSZ*4 > width) { എങ്കിൽ (!tail) നിർത്തുക; x = width-VECSZ*4; }
                    vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+VECSZ*2, a2, a3);
                    a0=v_fma(a0,va,bc); a1=v_fma(a1,va,bc); a2=v_fma(a2,va,bc); a3=v_fma(a3,va,bc);
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            അല്ലെങ്കിൽ {                                           // src0 broadcast: a*alpha+gamma is constant
                Wvec aa; vx_setall_as(src0, aa); Wvec acg = v_fma(aa, va, vg);
                വേണ്ടി (; x < width; x += VECSZ*4) {
                    എങ്കിൽ (x+VECSZ*4 > width) { എങ്കിൽ (!tail) നിർത്തുക; x = width-VECSZ*4; }
                    vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+VECSZ*2, b2, b3);
                    b0=v_fma(b0,vb,acg); b1=v_fma(b1,vb,acg); b2=v_fma(b2,vb,acg); b3=v_fma(b3,vb,acg);
                    v_store_pair_as(dst+x, b0, b1); v_store_pair_as(dst+x+VECSZ*2, b2, b3);
                }
            }
        }
    #endif
        എങ്കിൽ (s0x == s1x) {
            വേണ്ടി (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + (WT)src1[x]*beta + gamma);
        } അല്ലെങ്കിൽ എങ്കിൽ (s0x == 0) {
            സ്ഥിരം WT ac = (WT)src0[0]*alpha + gamma;
            വേണ്ടി (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src1[x]*beta + ac);
        } അല്ലെങ്കിൽ {
            സ്ഥിരം WT bc = (WT)src1[0]*beta + gamma;
            വേണ്ടി (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + bc);
        }
    }
    തിരികെ 0;
}

TKernel getAddWeightedFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)
{
    KernelFunc f = nullptr;
    #define AWS(Tt, Rr) addWeightedKernel<Tt, Rr, v_float32, ദശാംശം, സത്യം>
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #define AWD(Tt, Rr) addWeightedKernel<Tt, Rr, v_float64, ഇരട്ട, സത്യം>       // f64 SIMD
#അല്ലെങ്കിൽ
    #define AWD(Tt, Rr) addWeightedKernel<Tt, Rr, v_float32, ഇരട്ട, അസത്യം>      // scalar (v_float32 unused)
#endif
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_8U:  f = R==CV_8U  ? AWS(uint8_t,  uint8_t)  : R==CV_32F ? AWS(uint8_t,  ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_8S:  f = R==CV_8S  ? AWS(int8_t,   int8_t)   : R==CV_32F ? AWS(int8_t,   ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16U: f = R==CV_16U ? AWS(uint16_t, uint16_t) : R==CV_32F ? AWS(uint16_t, ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16S: f = R==CV_16S ? AWS(int16_t,  int16_t)  : R==CV_32F ? AWS(int16_t,  ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16F: f = R==CV_16F ? AWS(hfloat,   hfloat)   : R==CV_32F ? AWS(hfloat,   ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_16BF:f = R==CV_32F ? AWS(bfloat,   ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32F: f = R==CV_32F ? AWS(ദശാംശം,    ദശാംശം) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32U: f = R==CV_32U ? AWD(unsigned, unsigned) : R==CV_64F ? AWD(unsigned, ഇരട്ട) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_32S: f = R==CV_32S ? AWD(പൂർണ്ണസംഖ്യ,      പൂർണ്ണസംഖ്യ)      : R==CV_64F ? AWD(പൂർണ്ണസംഖ്യ,      ഇരട്ട) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_64U: f = R==CV_64F ? AWD(uint64_t, ഇരട്ട) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_64S: f = R==CV_64F ? AWD(int64_t,  ഇരട്ട) : nullptr; നിർത്തുക;
    അവസ്ഥ CV_64F: f = R==CV_64F ? AWD(ഇരട്ട,   ഇരട്ട) : nullptr; നിർത്തുക;
    സ്ഥിരസ്ഥിതി: ;
    }
    #undef AWS
    #undef AWD
    തിരികെ {f, nullptr, 0};
}

// ===========================================================================
// OP_SELECT: dst = (mask != 0) ? a : b
// ===========================================================================
// The one masking primitive of the engine. It serves both the public texpr select() and the
// masked-op tail: `cv::add(..., mask)` computes the full result into a temp `r`, then a final
// select(mask, r, dst) -> dst lands the masked subset in the (pre-existing) output and PRESERVES
// the rest - dst rides as both an input and the result. That aliasing is safe even under the
// right-edge tail backoff: re-running select over already-blended elements is IDEMPOTENT
// (mask!=0 lanes stay a, mask==0 lanes stay b/dst). Only dst == mask would break (the store
// rewrites the mask before the backoff re-reads it) - that combination falls to the scalar tail.
//
// The mask is one byte per element (bool/u8/s8 - never parameterized by its depth, we just test
// the byte != 0); a/b/dst share one depth, the kernel is templated by the element SIZE only.
// Channels fall out of the broadcast machinery: single-channel data arrives as a (width x 1) tile
// with a per-element mask (smx == 1); n-channel data as a tall-thin tile - channel axis = width,
// the 1-channel mask broadcasting across it (smx == 0, one mask byte per row) - handled by the
// interleaved fast path for 2..4 channels, per-row otherwise. Branches may broadcast (s1x/s2x == 0).
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
// VECSZ(T) mask bytes -> T-width lanes (u8 direct, u16/u32 via expand)
സ്ഥിതം വരിയിൽ v_uint8  loadSelectMask(സ്ഥിരം uchar* m, സ്ഥിരം v_uint8&)  { തിരികെ vx_load(m); }
സ്ഥിതം വരിയിൽ v_uint16 loadSelectMask(സ്ഥിരം uchar* m, സ്ഥിരം v_uint16&) { തിരികെ vx_load_expand(m); }
സ്ഥിതം വരിയിൽ v_uint32 loadSelectMask(സ്ഥിരം uchar* m, സ്ഥിരം v_uint32&) { തിരികെ vx_load_expand_q(m); }

സ്ഥിതം വരിയിൽ ശൂന്യം setallSelect(സ്ഥിരം uint8_t*  p, v_uint8&  a) { a = vx_setall_u8(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം setallSelect(സ്ഥിരം uint16_t* p, v_uint16& a) { a = vx_setall_u16(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം setallSelect(സ്ഥിരം uint32_t* p, v_uint32& a) { a = vx_setall_u32(*p); }
#endif

template<typename T, typename Tvec>
സ്ഥിതം പൂർണ്ണസംഖ്യ selectKernel(സ്ഥിരം ശൂന്യം* mask_, size_t smy, size_t smx,
                        സ്ഥിരം ശൂന്യം* src1_, size_t s1y, size_t s1x,
                        സ്ഥിരം ശൂന്യം* src2_, size_t s2y, size_t s2x,
                        ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                        സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    s1y /= വലുപ്പം(T);                    // the 1-byte mask's smy stays in bytes == elements
    s2y /= വലുപ്പം(T);
    dsty /= വലുപ്പം(T);
    CV_Assert(smx <= 1u && s1x <= 1u && s2x <= 1u);

    സ്ഥിരം uchar* mask = (സ്ഥിരം uchar*)mask_;
    സ്ഥിരം T* src1 = (സ്ഥിരം T*)src1_;
    സ്ഥിരം T* src2 = (സ്ഥിരം T*)src2_;
    T* dst = (T*)dst_;

    എങ്കിൽ (height > 1 && dsty == (size_t)width && smy == smx*(size_t)width &&
        s1y == s1x*(size_t)width && s2y == s2x*(size_t)width)
    { width *= height; height = 1; }

    പൂർണ്ണസംഖ്യ y = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
    എങ്കിൽ constexpr (വലുപ്പം(T) <= 4)
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Tvec>::vlanes();
        സ്ഥിരം Tvec z = v_setzero_<Tvec>();

        // n-channel data under a per-pixel mask (the masked-op shape): channel axis = width (2..4),
        // one mask byte per row. Process VECSZ rows per iteration - expand the mask once and
        // interleave it across the channel lanes.
        എങ്കിൽ (height > VECSZ && 2 <= width && width <= 4 && smx == 0u && smy == 1u &&
            s1x == 1u && s1y == (size_t)width && s2x == 1u && s2y == (size_t)width &&
            dsty == (size_t)width)
        {
            constexpr പൂർണ്ണസംഖ്യ MAXVECSZ = VTraits<Tvec>::max_nlanes;
            T maskbuf[MAXVECSZ*4] = {};      // {} for -Wmaybe-uninitialized only (fully written)
            സ്ഥിരം പൂർണ്ണസംഖ്യ dy = VECSZ;
            വേണ്ടി (; y + dy <= height; y += dy, mask += dy, src1 += width*dy, src2 += width*dy,
                                                          dst += width*dy)
            {
                Tvec m0 = v_ne(loadSelectMask(mask, z), z), m1, m2, m3;
                എങ്കിൽ (width == 2)      v_store_interleave(maskbuf, m0, m0);
                അല്ലെങ്കിൽ എങ്കിൽ (width == 3) v_store_interleave(maskbuf, m0, m0, m0);
                അല്ലെങ്കിൽ                 v_store_interleave(maskbuf, m0, m0, m0, m0);
                m0 = vx_load(maskbuf);
                m1 = vx_load(maskbuf + VECSZ);
                എങ്കിൽ (width > 2) m2 = vx_load(maskbuf + VECSZ*2);
                എങ്കിൽ (width > 3) m3 = vx_load(maskbuf + VECSZ*3);
                v_store(dst, v_select(m0, vx_load(src1), vx_load(src2)));
                v_store(dst + VECSZ, v_select(m1, vx_load(src1 + VECSZ), vx_load(src2 + VECSZ)));
                എങ്കിൽ (width > 2)
                    v_store(dst + VECSZ*2,
                            v_select(m2, vx_load(src1 + VECSZ*2), vx_load(src2 + VECSZ*2)));
                എങ്കിൽ (width > 3)
                    v_store(dst + VECSZ*3,
                            v_select(m3, vx_load(src1 + VECSZ*3), vx_load(src2 + VECSZ*3)));
            }
            // the remaining < VECSZ rows fall through to the per-row path below
        }
        അല്ലെങ്കിൽ എങ്കിൽ (smx == 1)                                   // per-element mask - the common case
        {
            // a branch aliasing dst is fine under the backoff (idempotent select); dst == mask is not
            സ്ഥിരം തർക്കം use_tail_trick = width >= VECSZ*2 && dst_ != mask_;
            Tvec a, b;
            വേണ്ടി (; y < height; y++, mask += smy, src1 += s1y, src2 += s2y, dst += dsty)
            {
                // a row-constant branch still changes per row
                എങ്കിൽ (s1x == 0) setallSelect(src1, a);
                എങ്കിൽ (s2x == 0) setallSelect(src2, b);
                പൂർണ്ണസംഖ്യ x = 0;
                വേണ്ടി (; x < width; x += VECSZ)
                {
                    എങ്കിൽ (x + VECSZ > width) { എങ്കിൽ (!use_tail_trick || x == 0) നിർത്തുക; x = width - VECSZ; }
                    Tvec m = v_ne(loadSelectMask(mask + x, z), z);
                    എങ്കിൽ (s1x) a = vx_load(src1 + x);
                    എങ്കിൽ (s2x) b = vx_load(src2 + x);
                    v_store(dst + x, v_select(m, a, b));
                }
                വേണ്ടി (; x < width; x++)
                    dst[x] = mask[x] != 0 ? src1[x*s1x] : src2[x*s2x];
            }
            തിരികെ 0;
        }
    }
#endif
    വേണ്ടി (; y < height; y++, mask += smy, src1 += s1y, src2 += s2y, dst += dsty)
    {
        എങ്കിൽ (smx == 0)                     // one mask byte per row (n-channel data / broadcast mask)
        {
            സ്ഥിരം T* s = mask[0] != 0 ? src1 : src2;
            സ്ഥിരം size_t sx = mask[0] != 0 ? s1x : s2x;
            എങ്കിൽ ((സ്ഥിരം ശൂന്യം*)s != (സ്ഥിരം ശൂന്യം*)dst)      // row select from dst itself is a no-op
                വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++) dst[x] = s[x*sx];
        }
        അല്ലെങ്കിൽ
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++)
                dst[x] = mask[x] != 0 ? src1[x*s1x] : src2[x*s2x];
    }
    തിരികെ 0;
}

TKernel getSelectFunc_(പൂർണ്ണസംഖ്യ mdepth, പൂർണ്ണസംഖ്യ T)
{
    എങ്കിൽ (CV_ELEM_SIZE1(mdepth) != 1)      // the mask must be a 1-byte type (u8/s8/bool)
        തിരികെ {};
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (CV_ELEM_SIZE1(T))
    {
    അവസ്ഥ 1: fptr = selectKernel<uint8_t,  v_uint8 >; നിർത്തുക;
    അവസ്ഥ 2: fptr = selectKernel<uint16_t, v_uint16>; നിർത്തുക;
    അവസ്ഥ 4: fptr = selectKernel<uint32_t, v_uint32>; നിർത്തുക;
    അവസ്ഥ 8: fptr = selectKernel<uint64_t, v_uint32>; നിർത്തുക;   // SIMD path compiled out -> scalar
    സ്ഥിരസ്ഥിതി: ;
    }
    തിരികെ {fptr, nullptr, 0};
}

// ===========================================================================
// OP_CLAMP: dst = min(max(x, lo), hi)
// ===========================================================================
// All four operands share one depth (emitTernary unifies them); lo/hi are usually broadcast
// scalars (clamp(img, 10, 200) - literals ride as 0-dim consts with stepx == 0) but may be full
// arrays. clamp is IDEMPOTENT, so the right-edge tail backoff stays on when dst aliases x; only
// dst aliasing lo/hi suppresses it (the store would corrupt the bounds before the re-read).
// NaN note: v_min/v_max lane behavior on NaN is ISA-specific, matching the scalar std::min/max
// unspecifiedness - clamp of NaN is not a contract either way.
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
സ്ഥിതം വരിയിൽ ശൂന്യം setallClamp(സ്ഥിരം uchar*  p, v_uint8&   a) { a = vx_setall_u8(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം setallClamp(സ്ഥിരം schar*  p, v_int8&    a) { a = vx_setall_s8(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം setallClamp(സ്ഥിരം ushort* p, v_uint16&  a) { a = vx_setall_u16(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം setallClamp(സ്ഥിരം short*  p, v_int16&   a) { a = vx_setall_s16(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം setallClamp(സ്ഥിരം unsigned* p, v_uint32& a) { a = vx_setall_u32(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം setallClamp(സ്ഥിരം പൂർണ്ണസംഖ്യ*    p, v_int32&   a) { a = vx_setall_s32(*p); }
സ്ഥിതം വരിയിൽ ശൂന്യം setallClamp(സ്ഥിരം ദശാംശം*  p, v_float32& a) { a = vx_setall_f32(*p); }
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
സ്ഥിതം വരിയിൽ ശൂന്യം setallClamp(സ്ഥിരം ഇരട്ട* p, v_float64& a) { a = vx_setall_f64(*p); }
#endif
#endif

template<typename T, typename Tvec>
സ്ഥിതം പൂർണ്ണസംഖ്യ clampKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                       സ്ഥിരം ശൂന്യം* lo_, size_t s1y, size_t s1x,
                       സ്ഥിരം ശൂന്യം* hi_, size_t s2y, size_t s2x,
                       ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                       സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    s0y /= വലുപ്പം(T);
    s1y /= വലുപ്പം(T);
    s2y /= വലുപ്പം(T);
    dsty /= വലുപ്പം(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);

    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* lo = (സ്ഥിരം T*)lo_;
    സ്ഥിരം T* hi = (സ്ഥിരം T*)hi_;
    T* dst = (T*)dst_;

    എങ്കിൽ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width &&
        s1y == s1x*(size_t)width && s2y == s2x*(size_t)width)
    { width *= height; height = 1; }

    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < height; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
    {
        പൂർണ്ണസംഖ്യ x = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        എങ്കിൽ (s0x == 1)
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Tvec>::vlanes();
            സ്ഥിരം തർക്കം use_tail_trick = width >= VECSZ*2 && lo_ != dst_ && hi_ != dst_;
            Tvec vlo, vhi;
            എങ്കിൽ (s1x == 0) setallClamp(lo, vlo);
            എങ്കിൽ (s2x == 0) setallClamp(hi, vhi);
            വേണ്ടി (; x < width; x += VECSZ)
            {
                എങ്കിൽ (x + VECSZ > width) { എങ്കിൽ (!use_tail_trick || x == 0) നിർത്തുക; x = width - VECSZ; }
                എങ്കിൽ (s1x) vlo = vx_load(lo + x);
                എങ്കിൽ (s2x) vhi = vx_load(hi + x);
                v_store(dst + x, v_min(v_max(vx_load(src0 + x), vlo), vhi));
            }
        }
#endif
        വേണ്ടി (; x < width; x++)
        {
            T v = src0[x*s0x], l = lo[x*s1x], h = hi[x*s2x];
            dst[x] = std::min(std::max(v, l), h);
        }
    }
    തിരികെ 0;
}

// Scalar-only clamp for the depths without a native SIMD lane type here (f16/bf16 - compared in
// float; 64-bit ints).
template<typename T, typename WT>
സ്ഥിതം പൂർണ്ണസംഖ്യ scalarClampKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                             സ്ഥിരം ശൂന്യം* lo_, size_t s1y, size_t s1x,
                             സ്ഥിരം ശൂന്യം* hi_, size_t s2y, size_t s2x,
                             ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                             സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    s0y /= വലുപ്പം(T); s1y /= വലുപ്പം(T); s2y /= വലുപ്പം(T); dsty /= വലുപ്പം(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);
    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* lo = (സ്ഥിരം T*)lo_;
    സ്ഥിരം T* hi = (സ്ഥിരം T*)hi_;
    T* dst = (T*)dst_;
    എങ്കിൽ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width &&
        s1y == s1x*(size_t)width && s2y == s2x*(size_t)width)
    { width *= height; height = 1; }
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < height; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
        വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++)
        {
            WT v = (WT)src0[x*s0x], l = (WT)lo[x*s1x], h = (WT)hi[x*s2x];
            dst[x] = saturate_cast<T>(std::min(std::max(v, l), h));
        }
    തിരികെ 0;
}

TKernel getClampFunc_(പൂർണ്ണസംഖ്യ T)
{
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_8U:   fptr = clampKernel<uchar,    v_uint8  >; നിർത്തുക;
    അവസ്ഥ CV_8S:   fptr = clampKernel<schar,    v_int8   >; നിർത്തുക;
    അവസ്ഥ CV_16U:  fptr = clampKernel<ushort,   v_uint16 >; നിർത്തുക;
    അവസ്ഥ CV_16S:  fptr = clampKernel<short,    v_int16  >; നിർത്തുക;
    അവസ്ഥ CV_32U:  fptr = clampKernel<unsigned, v_uint32 >; നിർത്തുക;
    അവസ്ഥ CV_32S:  fptr = clampKernel<പൂർണ്ണസംഖ്യ,      v_int32  >; നിർത്തുക;
    അവസ്ഥ CV_32F:  fptr = clampKernel<ദശാംശം,    v_float32>; നിർത്തുക;
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    അവസ്ഥ CV_64F:  fptr = clampKernel<ഇരട്ട,   v_float64>; നിർത്തുക;
#അല്ലെങ്കിൽ
    അവസ്ഥ CV_64F:  fptr = scalarClampKernel<ഇരട്ട, ഇരട്ട>; നിർത്തുക;
#endif
    അവസ്ഥ CV_16F:  fptr = scalarClampKernel<hfloat, ദശാംശം>; നിർത്തുക;
    അവസ്ഥ CV_16BF: fptr = scalarClampKernel<bfloat, ദശാംശം>; നിർത്തുക;
    അവസ്ഥ CV_64U:  fptr = scalarClampKernel<uint64_t, uint64_t>; നിർത്തുക;
    അവസ്ഥ CV_64S:  fptr = scalarClampKernel<int64_t,  int64_t >; നിർത്തുക;
    സ്ഥിരസ്ഥിതി: ;
    }
    തിരികെ {fptr, nullptr, 0};
}

സ്ഥിതം പൂർണ്ണസംഖ്യ expandKernel(size_t s0y, size_t s0x,
                        ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                        പൂർണ്ണസംഖ്യ esz1)
{
    uchar* dst = (uchar*)dst_;
    എങ്കിൽ (s0x == 0u && width > 1) {
        പൂർണ്ണസംഖ്യ h = s0y > 0u ? height : 1;
        CV_Assert(esz1 == 1 || esz1 == 2 || esz1 == 4 || esz1 == 8);
        വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < h; y++) {
            എങ്കിൽ (esz1 == 1) {
                uchar* rowptr = dst + dsty*y;
                uchar val = rowptr[0];
                വേണ്ടി (പൂർണ്ണസംഖ്യ x = 1; x < width; x++) rowptr[x] = val;
            }
            അല്ലെങ്കിൽ എങ്കിൽ (esz1 == 2) {
                ushort* rowptr = (ushort*)(dst + dsty*y);
                ushort val = rowptr[0];
                വേണ്ടി (പൂർണ്ണസംഖ്യ x = 1; x < width; x++) rowptr[x] = val;
            }
            അല്ലെങ്കിൽ എങ്കിൽ (esz1 == 4) {
                unsigned* rowptr = (unsigned*)(dst + dsty*y);
                unsigned val = rowptr[0];
                വേണ്ടി (പൂർണ്ണസംഖ്യ x = 1; x < width; x++) rowptr[x] = val;
            }
            അല്ലെങ്കിൽ {
                uint64_t* rowptr = (uint64_t*)(dst + dsty*y);
                uint64_t val = rowptr[0];
                വേണ്ടി (പൂർണ്ണസംഖ്യ x = 1; x < width; x++) rowptr[x] = val;
            }
        }
    }

    എങ്കിൽ (s0y == 0) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ y = 1; y < height; y++)
            memcpy(dst + dsty*y, dst, (size_t)esz1*width);
    }
    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ castKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                      സ്ഥിരം ശൂന്യം*, size_t, size_t,
                      സ്ഥിരം ശൂന്യം*, size_t, size_t,
                      ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                      സ്ഥിരം ഇരട്ട* params, പൂർണ്ണസംഖ്യ esz1, ശൂന്യം* userdata)
{
    ദ്വിമാന_Func castfunc = (ദ്വിമാന_Func)userdata;
    castfunc((സ്ഥിരം uchar*)src0_, s0y, nullptr, 0, (uchar*)dst_, dsty,
             Size((s0x > 0u ? width : 1), (s0y > 0u ? height : 1)), (ശൂന്യം*)params);
    തിരികെ expandKernel(s0y, s0x, dst_, dsty, width, height, esz1);
}

// ===========================================================================
// Per-op entry points (this baseline). The op-level getElemwiseFunc() dispatcher and the regular
// get*Func() forwarders live in arithm.dispatch.cpp.
// ===========================================================================

// OP_CAST / OP_CONVERT_SCALE: wrap core's convert BinaryFunc (carried in kernel.userdata) in
// castKernel, which runs it over the distinct sub-region and then expands across broadcast axes.
TKernel getCastFunc_(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth, തർക്കം scaled)
{
    ദ്വിമാന_Func cvt = scaled ? getConvertScaleFunc(sdepth, ddepth) : getConvertFunc(sdepth, ddepth);
    തിരികെ {castKernel, (ശൂന്യം*)cvt, CV_ELEM_SIZE1(ddepth)};
}

// Non-template wrappers around the templated kernel selectors (ADD/SUB share getAddSubFunc, MIN/MAX
// share getMinMaxFunc). min/max are T x T -> T, so R is ignored (the dispatcher validates R == T).
TKernel getAddFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R) { തിരികെ getAddSubFunc<EwAdd>(T, R); }
TKernel getSubFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R) { തിരികെ getAddSubFunc<EwSub>(T, R); }
TKernel getMinFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R) { (ശൂന്യം)R; തിരികെ getMinMaxFunc<EwMin>(T); }
TKernel getMaxFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R) { (ശൂന്യം)R; തിരികെ getMinMaxFunc<EwMax>(T); }

TKernel getHypotFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)
{
    എങ്കിൽ (R != T)
        തിരികെ {};
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_16F:  fptr = vec_ദ്വിമാന_Kernel<hfloat, hfloat, v_float32, ദശാംശം, EwHypot>; നിർത്തുക;
    അവസ്ഥ CV_16BF: fptr = vec_ദ്വിമാന_Kernel<bfloat, bfloat, v_float32, ദശാംശം, EwHypot>; നിർത്തുക;
    അവസ്ഥ CV_32F:  fptr = vec_ദ്വിമാന_Kernel<ദശാംശം,  ദശാംശം,  v_float32, ദശാംശം, EwHypot>; നിർത്തുക;
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    അവസ്ഥ CV_64F:  fptr = vec_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, v_float64, ഇരട്ട, EwHypot>; നിർത്തുക;
#അല്ലെങ്കിൽ
    അവസ്ഥ CV_64F:  fptr = scalar_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, ഇരട്ട, EwHypot>; നിർത്തുക;
#endif
    സ്ഥിരസ്ഥിതി: ;
    }
    തിരികെ {fptr, nullptr, 0};
}

TKernel getAtan2Func_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)
{
    എങ്കിൽ (R != T)
        തിരികെ {};
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_16F:  fptr = vec_ദ്വിമാന_Kernel<hfloat, hfloat, v_float32, ദശാംശം, EwAtan2>; നിർത്തുക;
    അവസ്ഥ CV_16BF: fptr = vec_ദ്വിമാന_Kernel<bfloat, bfloat, v_float32, ദശാംശം, EwAtan2>; നിർത്തുക;
    അവസ്ഥ CV_32F:  fptr = vec_ദ്വിമാന_Kernel<ദശാംശം,  ദശാംശം,  v_float32, ദശാംശം, EwAtan2>; നിർത്തുക;
    അവസ്ഥ CV_64F:  fptr = scalar_ദ്വിമാന_Kernel<ഇരട്ട, ഇരട്ട, ഇരട്ട, EwAtan2>; നിർത്തുക;   // exact std::atan2
    സ്ഥിരസ്ഥിതി: ;
    }
    തിരികെ {fptr, nullptr, 0};
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
