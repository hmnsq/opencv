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

#அந்தர்பாவ "opencv2/core/hal/intrin.hpp"
#அந்தர்பாவ "convert.hpp"          // typed load/store-as helpers (cv::), and getConvert*Func
#அந்தர்பாவ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <cmath>
#அந்தர்பாவ <cstring>

நாமாகாஶ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// BinaryFunc and getConvertFunc / getConvertScaleFunc come from core (precomp.hpp / convert.hpp).

#எனில் CV_SIMD128_FP16
#undef CV_SIMD_16F
#define CV_SIMD_16F 1
#அல்லதெனில் !defined(CV_SIMD_16F)
#define CV_SIMD_16F 0       // keep `#if CV_SIMD_16F` -Wundef-clean on builds without FP16 SIMD
#endif

#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி uchar* p, v_uint8& a)
{ a = vx_setall_u8(*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி schar* p, v_int8& a)
{ a = vx_setall_s8(*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி uchar* p, v_int16& a)
{ a = vx_setall_s16(*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி schar* p, v_int16& a)
{ a = vx_setall_s16(*p); }

நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி ushort* p, v_uint16& a)
{ a = vx_setall_u16(*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி short* p, v_int16& a)
{ a = vx_setall_s16(*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி ushort* p, v_int32& a)
{ a = vx_setall_s32(*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி short* p, v_int32& a)
{ a = vx_setall_s32(*p); }

நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி unsigned* p, v_uint32& a)
{ a = vx_setall_u32(*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி முழு* p, v_int32& a)
{ a = vx_setall_s32(*p); }

நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி uchar* p, v_float32& a)
{ a = vx_setall_f32(தசம(*p)); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி schar* p, v_float32& a)
{ a = vx_setall_f32(தசம(*p)); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி ushort* p, v_float32& a)
{ a = vx_setall_f32(தசம(*p)); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி short* p, v_float32& a)
{ a = vx_setall_f32(தசம(*p)); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி தசம* p, v_float32& a)
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
#எனில் defined(__ARM_NEON)
நிலையான உள்வரி v_int32  v_add_sat(மாறிலி v_int32& a,  மாறிலி v_int32& b)  { திரும்பு v_int32(vqaddq_s32(a.val, b.val)); }
நிலையான உள்வரி v_int32  v_sub_sat(மாறிலி v_int32& a,  மாறிலி v_int32& b)  { திரும்பு v_int32(vqsubq_s32(a.val, b.val)); }
நிலையான உள்வரி v_uint32 v_add_sat(மாறிலி v_uint32& a, மாறிலி v_uint32& b) { திரும்பு v_uint32(vqaddq_u32(a.val, b.val)); }
நிலையான உள்வரி v_uint32 v_sub_sat(மாறிலி v_uint32& a, மாறிலி v_uint32& b) { திரும்பு v_uint32(vqsubq_u32(a.val, b.val)); }
#define EW_HAVE_MULSAT32 1
நிலையான உள்வரி v_uint8 v_mul_sat(மாறிலி v_uint8& a, மாறிலி v_uint8& b)
{
    uint16x8_t p0 = vmull_u8(vget_கீழ்_u8(a.val), vget_கீழ்_u8(b.val));
    uint16x8_t p1 = vmull_u8(vget_உயர்வு_u8(a.val), vget_உயர்வு_u8(b.val));
    திரும்பு v_uint8(vcombine_u8(vqmovn_u16(p0), vqmovn_u16(p1)));
}
நிலையான உள்வரி v_int8 v_mul_sat(மாறிலி v_int8& a, மாறிலி v_int8& b)
{
    int16x8_t p0 = vmull_s8(vget_கீழ்_s8(a.val), vget_கீழ்_s8(b.val));
    int16x8_t p1 = vmull_s8(vget_உயர்வு_s8(a.val), vget_உயர்வு_s8(b.val));
    திரும்பு v_int8(vcombine_s8(vqmovn_s16(p0), vqmovn_s16(p1)));
}
நிலையான உள்வரி v_uint16 v_mul_sat(மாறிலி v_uint16& a, மாறிலி v_uint16& b)
{
    uint32x4_t p0 = vmull_u16(vget_கீழ்_u16(a.val), vget_கீழ்_u16(b.val));
    uint32x4_t p1 = vmull_u16(vget_உயர்வு_u16(a.val), vget_உயர்வு_u16(b.val));
    திரும்பு v_uint16(vcombine_u16(vqmovn_u32(p0), vqmovn_u32(p1)));
}
நிலையான உள்வரி v_int16 v_mul_sat(மாறிலி v_int16& a, மாறிலி v_int16& b)
{
    int32x4_t p0 = vmull_s16(vget_கீழ்_s16(a.val), vget_கீழ்_s16(b.val));
    int32x4_t p1 = vmull_s16(vget_உயர்வு_s16(a.val), vget_உயர்வு_s16(b.val));
    திரும்பு v_int16(vcombine_s16(vqmovn_s32(p0), vqmovn_s32(p1)));
}
நிலையான உள்வரி v_uint32 v_mul_sat(மாறிலி v_uint32& a, மாறிலி v_uint32& b)
{
    uint64x2_t p0 = vmull_u32(vget_கீழ்_u32(a.val), vget_கீழ்_u32(b.val));
    uint64x2_t p1 = vmull_u32(vget_உயர்வு_u32(a.val), vget_உயர்வு_u32(b.val));
    திரும்பு v_uint32(vcombine_u32(vqmovn_u64(p0), vqmovn_u64(p1)));
}
நிலையான உள்வரி v_int32 v_mul_sat(மாறிலி v_int32& a, மாறிலி v_int32& b)
{
    int64x2_t p0 = vmull_s32(vget_கீழ்_s32(a.val), vget_கீழ்_s32(b.val));
    int64x2_t p1 = vmull_s32(vget_உயர்வு_s32(a.val), vget_உயர்வு_s32(b.val));
    திரும்பு v_int32(vcombine_s32(vqmovn_s64(p0), vqmovn_s64(p1)));
}
#இல்லையெனில்
நிலையான உள்வரி v_int32 v_add_sat(மாறிலி v_int32& a, மாறிலி v_int32& b)
{
    v_int32 res = v_add(a, b);
    v_int32 ov  = v_and(v_xor(a, res), v_xor(b, res));               // sign bit set iff overflow
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(INT_MAX));       // a >= 0 ? INT_MAX : INT_MIN
    திரும்பு v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
நிலையான உள்வரி v_int32 v_sub_sat(மாறிலி v_int32& a, மாறிலி v_int32& b)
{
    v_int32 res = v_sub(a, b);
    v_int32 ov  = v_and(v_xor(a, b), v_xor(a, res));
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(INT_MAX));
    திரும்பு v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
நிலையான உள்வரி v_uint32 v_add_sat(மாறிலி v_uint32& a, மாறிலி v_uint32& b)
{
    v_uint32 res = v_add(a, b);
    திரும்பு v_or(res, v_lt(res, a));                                  // wrapped => all-ones
}
நிலையான உள்வரி v_uint32 v_sub_sat(மாறிலி v_uint32& a, மாறிலி v_uint32& b)
{
    திரும்பு v_and(v_sub(a, b), v_ge(a, b));                           // borrow => zero
}
நிலையான உள்வரி v_uint8 v_mul_sat(மாறிலி v_uint8& a, மாறிலி v_uint8& b)
{ v_uint16 p0, p1; v_mul_expand(a, b, p0, p1); திரும்பு v_pack(p0, p1); }
நிலையான உள்வரி v_int8 v_mul_sat(மாறிலி v_int8& a, மாறிலி v_int8& b)
{ v_int16 p0, p1; v_mul_expand(a, b, p0, p1); திரும்பு v_pack(p0, p1); }
நிலையான உள்வரி v_uint16 v_mul_sat(மாறிலி v_uint16& a, மாறிலி v_uint16& b)
{ v_uint32 p0, p1; v_mul_expand(a, b, p0, p1); திரும்பு v_pack(p0, p1); }
நிலையான உள்வரி v_int16 v_mul_sat(மாறிலி v_int16& a, மாறிலி v_int16& b)
{ v_int32 p0, p1; v_mul_expand(a, b, p0, p1); திரும்பு v_pack(p0, p1); }
#endif
#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F   // scalable (RVV) has v_float64 without CV_SIMD_64F
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி இரட்டை*   p, v_float64& a) { a = vx_setall_f64(*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி முழு*      p, v_float64& a) { a = vx_setall_f64((இரட்டை)*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி unsigned* p, v_float64& a) { a = vx_setall_f64((இரட்டை)*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி int64_t*  p, v_float64& a) { a = vx_setall_f64((இரட்டை)*p); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி uint64_t* p, v_float64& a) { a = vx_setall_f64((இரட்டை)*p); }
#endif
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி bfloat* p, v_float32& a)
{ a = vx_setall_f32(தசம(*p)); }
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி hfloat* p, v_float32& a)
{ a = vx_setall_f32(தசம(*p)); }

நிலையான உள்வரி வெறுமை vx_load_pair_as(மாறிலி uchar* p, v_uint8& a, v_uint8& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_uint8>::vlanes());
}

நிலையான உள்வரி வெறுமை vx_load_pair_as(மாறிலி schar* p, v_int8& a, v_int8& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_int8>::vlanes());
}

நிலையான உள்வரி வெறுமை vx_load_pair_as(மாறிலி uchar* p, v_uint32& a, v_uint32& b)
{
    v_uint16 w = vx_load_expand(p);
    v_expand(w, a, b);
}

நிலையான உள்வரி வெறுமை v_store_pair_as(uchar* p, மாறிலி v_uint8& a, மாறிலி v_uint8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_uint8>::vlanes(), b);
}

நிலையான உள்வரி வெறுமை v_store_pair_as(schar* p, மாறிலி v_int8& a, மாறிலி v_int8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_int8>::vlanes(), b);
}


நிலையான உள்வரி வெறுமை v_store_pair_as(முழு* p, மாறிலி v_int16& a, மாறிலி v_int16& b)
{
    மாறிலி முழு nlanes32 = VTraits<v_int32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v0);
    v_store(p + nlanes32, v1);
    v_store(p + nlanes32*2, v2);
    v_store(p + nlanes32*3, v3);
}

நிலையான உள்வரி வெறுமை v_store_pair_as(தசம* p, மாறிலி v_int16& a, மாறிலி v_int16& b)
{
    மாறிலி முழு nlanes32 = VTraits<v_float32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v_cvt_f32(v0));
    v_store(p + nlanes32, v_cvt_f32(v1));
    v_store(p + nlanes32*2, v_cvt_f32(v2));
    v_store(p + nlanes32*3, v_cvt_f32(v3));
}

நிலையான உள்வரி வெறுமை v_store_pair_as(தசம* p, மாறிலி v_int32& a, மாறிலி v_int32& b)
{
    v_store(p, v_cvt_f32(a));
    v_store(p + VTraits<v_float32>::vlanes(), v_cvt_f32(b));
}

நிலையான உள்வரி வெறுமை v_store_pair_as(hfloat* p, மாறிலி v_float32& a, மாறிலி v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

நிலையான உள்வரி வெறுமை v_store_pair_as(bfloat* p, மாறிலி v_float32& a, மாறிலி v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

// Reinterpret a comparison result (whose lane type matches the operands) as the UNSIGNED integer
// vector of the same width: u8->u8, s8->u8, u16/s16->u16, u32/s32/f32->u32. The destination type
// drives the overload, so the compare kernel needs no per-width if constexpr.
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_uint8&  s, v_uint8&  d) { d = s; }
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_int8&   s, v_uint8&  d) { d = v_reinterpret_as_u8(s); }
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_uint16& s, v_uint16& d) { d = s; }
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_int16&  s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_uint32& s, v_uint32& d) { d = s; }
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_int32&  s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_float32& s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
#எனில் CV_SIMD_16F
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_float16& s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
#endif
#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
நிலையான உள்வரி வெறுமை v_reinterpret_as(மாறிலி v_float64& s, v_uint64& d) { d = v_reinterpret_as_u64(s); }
#endif

// Broadcast the compare mask value (1 or 255) across the unsigned work vector.
நிலையான உள்வரி வெறுமை v_setall_mask(v_uint8&  v, uchar t) { v = vx_setall_u8(t); }
நிலையான உள்வரி வெறுமை v_setall_mask(v_uint16& v, uchar t) { v = vx_setall_u16(t); }
நிலையான உள்வரி வெறுமை v_setall_mask(v_uint32& v, uchar t) { v = vx_setall_u32(t); }

// NOTE: v_store_pair_as(uchar*, v_uint16/v_uint32, ...) - the narrowing pack-to-u8 stores we need -
// already come from core's convert.hpp (included above), so they are not redefined here.

#எனில் CV_SIMD_16F
நிலையான உள்வரி வெறுமை vx_setall_as(மாறிலி hfloat* p, v_float16& a)
{ a = vx_setall_f16(*p); }
நிலையான உள்வரி வெறுமை vx_load_pair_as(மாறிலி hfloat* p, v_float16& a, v_float16& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_float16>::vlanes());
}
நிலையான உள்வரி வெறுமை v_store_pair_as(hfloat* p, மாறிலி v_float16& a, மாறிலி v_float16& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_float16>::vlanes(), b);
}
#endif

#endif

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

நாமாகாஶ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      arithm.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getAddFunc_(முழு T, முழு R);
TKernel getSubFunc_(முழு T, முழு R);
TKernel getMulFunc_(முழு T, முழு R);
TKernel getDivFunc_(முழு T, முழு R, தருக்க checked);
TKernel getMinFunc_(முழு T, முழு R);
TKernel getMaxFunc_(முழு T, முழு R);
TKernel getAbsdiffFunc_(முழு T, முழு R);
TKernel getHypotFunc_(முழு T, முழு R);         // hypot = sqrt(x^2+y^2), float depths, T x T -> T
TKernel getAtan2Func_(முழு T, முழு R);         // atan2(y, x), radians (-pi, pi], float depths
TKernel getCmpFunc_(உச்சி op, முழு T);
TKernel getBitwiseFunc_(உச்சி op, முழு esz);                    // OP_AND / OP_OR / OP_XOR, by element size
TKernel getNotFunc_(முழு esz);                                // OP_NOT, by element size
TKernel getAddWeightedFunc_(முழு T, முழு R);                   // OP_ADDW, a*alpha+b*beta+gamma (T x T -> R)
TKernel getSelectFunc_(முழு mdepth, முழு T);   // OP_SELECT: 1-byte mask, a/b/dst of T (by esz)
TKernel getClampFunc_(முழு T);                // OP_CLAMP: min(max(x, lo), hi), all operands of T
TKernel getCastFunc_(முழு sdepth, முழு ddepth, தருக்க scaled);   // OP_CAST / OP_CONVERT_SCALE

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors (vector + scalar). New binary ops slot in here.
// ===========================================================================
கட்டமைப்பு EwAdd {
    // useScalar: does the op consume the scale scalar (params[0]) in vec()/preproc()? When false,
    // vecBinaryKernel's fast 2-arg branch is taken unconditionally (the check folds at compile time);
    // when true, it is taken only if the runtime scale is exactly 1.
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_add(a, b); }
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: v_add wraps - use the saturating version to match the scalar int64+clamp tail
    நிலையான v_int32  vec(மாறிலி v_int32& a,  மாறிலி v_int32& b)  { திரும்பு v_add_sat(a, b); }
    நிலையான v_uint32 vec(மாறிலி v_uint32& a, மாறிலி v_uint32& b) { திரும்பு v_add_sat(a, b); }
#endif
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    // Accumulate in the promoted type, NOT in W: for the native saturating path W is the narrow
    // lane type (schar/short/...), and (W)(a+b) would wrap in 8/16 bits before saturate_cast<Tr>
    // could clamp. Letting a+b promote (narrow -> int) keeps saturation for 8/16-bit outputs and
    // the natural wrap for 32/64-bit (both matching cv::add). The SIMD path already saturates.
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு W(a + b); }
};

கட்டமைப்பு EwSub {
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_sub(a, b); }
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: see EwAdd::vec
    நிலையான v_int32  vec(மாறிலி v_int32& a,  மாறிலி v_int32& b)  { திரும்பு v_sub_sat(a, b); }
    நிலையான v_uint32 vec(மாறிலி v_uint32& a, மாறிலி v_uint32& b) { திரும்பு v_sub_sat(a, b); }
#endif
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு W(a - b); }   // see EwAdd::scl
    // 64-bit unsigned has no wider WT to hold a-b, so the generic path would wrap on underflow.
    // Saturate at 0 to match cv::subtract (8/16/32-bit already saturate via SIMD floor / wide WT).
    நிலையான uint64_t scl(uint64_t a, uint64_t b, uint64_t) { திரும்பு uint64_t(a >= b)*(a - b); }
};

// mul/div compute in a wide FLOAT work type (W = float for <=16-bit/f16/bf/f32, double for
// 32/64-bit/f64), matching cv::multiply/divide; the executor casts the work-type result down to
// rdepth. scalar (reference) path only for now - SIMD can follow. (No vec(): never instantiated.)
// vec(a, b, scale): the 3rd arg is the scale vector. COMMUTATIVE ops (mul, absdiff) ignore it - their
// scale is folded into the (cheaper) preproc of one operand. DIVISION is NOT commutative, so its scale
// MUST stick to the numerator; it takes the scale in vec (numerator*scale/denominator) and leaves
// preproc as identity. vecBinaryKernel always passes vscalar to vec, so every branch (incl. a broadcast
// denominator) divides correctly. The 2-arg vec (scale==1) is the both-contiguous fast path.
கட்டமைப்பு EwMul {
    நிலையான constexpr தருக்க useScalar = உண்மை;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_mul(a, b); }
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    // integer lanes: full product clamped to the lane type (v_mul_sat) == cv::multiply semantics
    // at scale 1. These drive the scale==1 fast path on FULL-width registers (Wvec1 = the native
    // lane vector): whole-register loads/stores, widening happens inside the multiply itself.
    // The guard matches the v_mul_sat definitions above (like EwAdd's 32-bit overloads): in
    // no-SIMD builds the vector typedefs still exist (intrin_cpp), but the helpers do not.
    நிலையான v_uint8  vec(மாறிலி v_uint8& a,  மாறிலி v_uint8& b)  { திரும்பு v_mul_sat(a, b); }
    நிலையான v_int8   vec(மாறிலி v_int8& a,   மாறிலி v_int8& b)   { திரும்பு v_mul_sat(a, b); }
    நிலையான v_uint16 vec(மாறிலி v_uint16& a, மாறிலி v_uint16& b) { திரும்பு v_mul_sat(a, b); }
    நிலையான v_int16  vec(மாறிலி v_int16& a,  மாறிலி v_int16& b)  { திரும்பு v_mul_sat(a, b); }
#ifdef EW_HAVE_MULSAT32
    நிலையான v_uint32 vec(மாறிலி v_uint32& a, மாறிலி v_uint32& b) { திரும்பு v_mul_sat(a, b); }
    நிலையான v_int32  vec(மாறிலி v_int32& a,  மாறிலி v_int32& b)  { திரும்பு v_mul_sat(a, b); }
#endif
#endif
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V& s) { திரும்பு v_mul(a, s); }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST s) { திரும்பு a * b * s; }
};
// div has two variants by the COMMON INPUT type (matching cv::'s per-type kernel choice): integer
// inputs guard divide-by-zero -> 0 (cv:: iscalar_div); float inputs do NOT guard (cv:: fscalar_div,
// a/0 -> inf), which then saturates on the cast to an integer output exactly like cv::divide.
கட்டமைப்பு EwDivInt {
    நிலையான constexpr தருக்க useScalar = உண்மை;
    // integer inputs computed in the float work type: guard b==0 -> 0. Scale rides the numerator (vec).
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) {
        மாறிலி V z = v_setzero_<V>();
        திரும்பு v_select(v_eq(b, z), z, v_div(a, b));
    }
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி V& s) {
        மாறிலி V z = v_setzero_<V>();
        திரும்பு v_select(v_eq(b, z), z, v_div(v_mul(a, s), b));
    }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }   // identity: scale is in vec
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST s) { திரும்பு b != W(0) ? a * s / b : W(0); }
};
கட்டமைப்பு EwDivFlt {
    நிலையான constexpr தருக்க useScalar = உண்மை;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_div(a, b); }
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி V& s) { திரும்பு v_div(v_mul(a, s), b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }   // identity: scale is in vec
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST s) { திரும்பு a * s / b; }
};

// atan2(y, x) in RADIANS over the standard C range (-pi, pi] - the fastAtan2 minimax polynomial
// (mathfuncs_core.simd.hpp v_atan_f32) with the 180/pi factor dropped and the C quadrant logic
// (fastAtan2 returns degrees in [0, 360)). Absolute accuracy ~1e-5 rad, same as cv::fastAtan2.
// Generic over the universal-intrinsic float vector type.
template<typename V>
நிலையான உள்வரி V v_atan2(மாறிலி V& y, மாறிலி V& x)
{
    using LT = typename VTraits<V>::lane_type;
    மாறிலி V eps  = v_setall_<V>((LT)DBL_EPSILON);
    மாறிலி V z    = v_setzero_<V>();
    மாறிலி V p7   = v_setall_<V>((LT)-0.04432655554792128);
    மாறிலி V p5   = v_setall_<V>((LT)0.1555786518463281);
    மாறிலி V p3   = v_setall_<V>((LT)-0.3258083974640975);
    மாறிலி V p1   = v_setall_<V>((LT)0.9997878412794807);
    மாறிலி V vpi2 = v_setall_<V>((LT)(CV_PI/2));
    மாறிலி V vpi  = v_setall_<V>((LT)CV_PI);

    V ax = v_abs(x), ay = v_abs(y);
    V c  = v_div(v_min(ax, ay), v_add(v_max(ax, ay), eps));
    V c2 = v_mul(c, c);
    V a  = v_mul(v_fma(v_fma(v_fma(p7, c2, p5), c2, p3), c2, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(vpi2, a));
    a = v_select(v_lt(x, z), v_sub(vpi, a), a);
    a = v_select(v_lt(y, z), v_sub(z, a), a);
    திரும்பு a;
}

கட்டமைப்பு EwAtan2 {
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_atan2(a, b); }   // a = y, b = x
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு std::atan2(a, b); }
};

// hypot(x, y) = sqrt(x^2 + y^2): NAIVE (matches cv::magnitude; overflow at |x| ~ 1e19+ for f32
// inputs is accepted), computed in the float work type; T x T -> T over the float depths.
கட்டமைப்பு EwHypot {
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_sqrt(v_fma(a, a, v_mul(b, b))); }
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு std::sqrt(a*a + b*b); }
};

// min / max / absdiff: T x T -> T (same depth in and out, no scale). v_min/v_max exist for every
// vector lane type (64-bit ints fall back to scalar). absdiff uses v_absdiff (defined for the
// UNSIGNED and float lane types - signed/wide depths go through the scalar path), and the scalar
// |a-b| is computed branch-wise so it never underflows an unsigned work type.
கட்டமைப்பு EwMin {
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_min(a, b); }
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு std::min(a, b); }
};
கட்டமைப்பு EwMax {
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_max(a, b); }
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு std::max(a, b); }
};
கட்டமைப்பு EwAbsdiff {
    நிலையான constexpr தருக்க useScalar = பொய்;
    // The absdiff RESULT is the UNSIGNED type of the input width: v_absdiff(v_int8/16/32) already returns
    // v_uint8/16/32 (the true |a-b|, which can exceed the signed max), and v_absdiff on unsigned/float
    // returns the same type. vec() therefore returns THAT type (deduced), not the input V, so the kernel
    // stores it through the matching UNSIGNED v_store_pair_as (an honest same-type store) - no reinterpret,
    // no touching the saturating-narrow overloads' semantics. (This is why the kernel keeps the vec
    // result in its own variable rather than reusing the input operand.)
    template<typename V> நிலையான auto vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_absdiff(a, b); }
    template<typename V, typename S> நிலையான auto vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு v_absdiff(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு a > b ? W(a - b) : W(b - a); }
};

// Signed T -> SAME signed T (rdepth==T): |a-b| saturated into the signed range, in ONE pass. cv::absdiff
// keeps the signed depth, so the generic path computes absdiff->unsigned then casts back down (2 insns);
// this fuses it. v_absdiff yields the unsigned |a-b|; v_min clamps to the signed max (already >=0), then
// a same-width reinterpret to signed (all values now fit).
கட்டமைப்பு EwAbsdiffS {
    நிலையான constexpr தருக்க useScalar = பொய்;
    நிலையான v_int8  vec(மாறிலி v_int8&  a, மாறிலி v_int8&  b) { திரும்பு v_reinterpret_as_s8 (v_min(v_absdiff(a, b), vx_setall_u8 (0x7f))); }
    நிலையான v_int16 vec(மாறிலி v_int16& a, மாறிலி v_int16& b) { திரும்பு v_reinterpret_as_s16(v_min(v_absdiff(a, b), vx_setall_u16(0x7fff))); }
    நிலையான v_int32 vec(மாறிலி v_int32& a, மாறிலி v_int32& b) { திரும்பு v_reinterpret_as_s32(v_min(v_absdiff(a, b), vx_setall_u32(0x7fffffff))); }
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு a > b ? W(a - b) : W(b - a); }
    // 64-bit signed has no wider work type to widen into, and |INT64_MIN - b| does not fit in
    // int64_t, so the difference is taken in uint64 - exact for every pair - and then clamped.
    // The narrower widths reach this overload only through their int64 work type, where |a - b|
    // is at most 2^32 and the clamp is a no-op.
    நிலையான int64_t scl(int64_t a, int64_t b, int64_t)
    {
        மாறிலி uint64_t d = (a > b) ? (uint64_t)a - (uint64_t)b : (uint64_t)b - (uint64_t)a;
        திரும்பு (int64_t)std::min<uint64_t>(d, (uint64_t)INT64_MAX);
    }
};

// compare: T x T -> u8 mask. cmp(a,b) is the scalar relation over a work type W (integers compare
// directly, f16/bf16 through float); vec(a,b) is the vector relation returning an all-ones/zero mask
// in the operand's own lane type. The kernel turns either into 0 / trueVal (1 or 255 via TKernel::flags).
கட்டமைப்பு EwCmpEq { template<typename W> நிலையான தருக்க cmp(W a, W b) { திரும்பு a == b; }
                 template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_eq(a, b); } };
கட்டமைப்பு EwCmpNe { template<typename W> நிலையான தருக்க cmp(W a, W b) { திரும்பு a != b; }
                 template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_ne(a, b); } };
// LT/LE are synthesized from GT/GE via the EW_KERNEL_SWAP01 flag (a<b == b>a), so no Lt/Le kernels.
கட்டமைப்பு EwCmpGt { template<typename W> நிலையான தருக்க cmp(W a, W b) { திரும்பு a > b; }
                 template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_gt(a, b); } };
கட்டமைப்பு EwCmpGe { template<typename W> நிலையான தருக்க cmp(W a, W b) { திரும்பு a >= b; }
                 template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_ge(a, b); } };

// bitwise AND/OR/XOR: bit-pattern op, type-agnostic. Run on the UNSIGNED integer whose width matches
// the element (u8/u16/u32/u64), so one functor set covers every depth. No scale, no widening (T x T ->
// T, exactly like min/max); preproc is the identity (min/max share this shape). 64-bit uses the scalar
// path (no widening vector helpers), the rest ride vecBinaryKernel's native same-type path.
கட்டமைப்பு EwAnd {
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_and(a, b); }
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு W(a & b); }
};
கட்டமைப்பு EwOr {
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_or(a, b); }
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு W(a | b); }
};
கட்டமைப்பு EwXor {
    நிலையான constexpr தருக்க useScalar = பொய்;
    template<typename V> நிலையான V vec(மாறிலி V& a, மாறிலி V& b) { திரும்பு v_xor(a, b); }
    template<typename V, typename S> நிலையான V vec(மாறிலி V& a, மாறிலி V& b, மாறிலி S&) { திரும்பு vec(a, b); }
    template<typename V> நிலையான V preproc(மாறிலி V& a, மாறிலி V&) { திரும்பு a; }
    template<typename W, typename ST> நிலையான W scl(W a, W b, ST) { திரும்பு W(a ^ b); }
};

// Collapse a gap-free 2D tile to 1D (call with the per-operand x/y-steps).
#define EW_TRY_COLLAPSE(NSRC) \
    எனில் (height > 1 && dsty == (size_t)width && \
        s0y == s0x*(size_t)width && (NSRC < 2 || s1y == s1x*(size_t)width)) \
    { width *= height; height = 1; }

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
template<typename T, typename Tr, typename WT, வகுப்பு Op, typename ST=WT>
நிலையான முழு scalar_இருமம்_Kernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                              மாறிலி வெறுமை* src1_, size_t s1y, size_t s1x,
                              மாறிலி வெறுமை*, size_t, size_t, வெறுமை* dst_, size_t dsty,
                              முழு width, முழு height, மாறிலி இரட்டை* params, முழு, வெறுமை*)
{
    எனில் (width <= 0 || height <= 0) திரும்பு 0;
    s0y /= அளவு(T);
    s1y /= அளவு(T);
    dsty /= அளவு(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);

    மாறிலி T* src0 = (மாறிலி T*)src0_;
    மாறிலி T* src1 = (மாறிலி T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    க்கு (முழு y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        எனில் (s0x == s1x) {
            க்கு (முழு x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], (WT)src1[x], scalar));
        }
        இல்லையெனில் எனில் (s0x == 0) {
            WT sc0 = (WT)src0[0];
            க்கு (முழு x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl(sc0, (WT)src1[x], scalar));
        }
        இல்லையெனில் {
            WT sc1 = (WT)src1[0];
            க்கு (முழு x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], sc1, scalar));
        }
    }
    திரும்பு 0;
}

template<typename T, typename WT>
நிலையான வெறுமை expandScalar(மாறிலி T* sc, size_t sx, முழு n0, WT* scbuf, முழு n)
{
    எனில் (n0 <= 0 || n <= 0) திரும்பு;
    முழு i = 0;
    க்கு (; i < n0; i++) scbuf[i] = (WT)sc[i*sx];
    க்கு (; i < n; i++) scbuf[i] = scbuf[i - n0];
}

// Decode the per-channel patch bytes (mask + value) from the kernel flags into small arrays. Uniform
// (no EW_CMP_PATCH): mask = trueVal, value = 0 (an ordinary compare). Per-channel: from the 4-bit
// fields. The compare kernels apply  result = (rawmask & mask) | value  per channel - folding the
// former separate patch pass into the compare (one pass).
நிலையான உள்வரி வெறுமை cmpUnpackPatch(முழு flags, uchar trueVal, uchar mvals[4], uchar vvals[4])
{
    எனில் (flags & EW_CMP_PATCH)
        க்கு (முழு c = 0; c < 4; c++)
        {
            மாறிலி முழு f = (flags >> (EW_CMP_PATCH_SHIFT + c*4)) & 0xF;
            mvals[c] = (uchar)cmpPatchByte(f & 3);
            vvals[c] = (uchar)cmpPatchByte((f >> 2) & 3);
        }
    இல்லையெனில்
        க்கு (முழு c = 0; c < 4; c++) { mvals[c] = trueVal; vvals[c] = 0; }
}

// Scalar comparison kernel: T x T -> u8 mask. Result is 0 (false) or `trueVal` (true), trueVal read
// from TKernel::flags - 255 (default, matching cv::compare) or 1 (a numpy-style 0/1 mask). Integers
// compare directly (WT == T); f16/bf16 compare through a float WT. This is the fallback for the depths
// the SIMD kernel doesn't cover (f16/bf16/64-bit). stepx in {0,1}; dst is u8, byte step == element step.
template<typename T, typename WT, வகுப்பு Cmp>
நிலையான முழு scalarCompareKernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                         மாறிலி வெறுமை* src1_, size_t s1y, size_t s1x,
                         மாறிலி வெறுமை*, size_t, size_t,
                         வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                         மாறிலி இரட்டை*, முழு flags, வெறுமை*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    எனில் (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= அளவு(T);
    s1y /= அளவு(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);

    மாறிலி T* src0 = (மாறிலி T*)src0_;
    மாறிலி T* src1 = (மாறிலி T*)src1_;
    uchar* dst = (uchar*)dst_;
    மாறிலி uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    மாறிலி தருக்க perch = (flags & EW_CMP_PATCH) != 0;          // per-channel fix-up (width=cn<=4)
    CV_Assert(!perch || width <= 4);                         // a per-channel patch is a short-row tile
    #define EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                         : (uchar)((cond) ? trueVal : 0))

    EW_TRY_COLLAPSE(2);
    க்கு (முழு y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        எனில் (s0x == s1x) {
            க்கு (முழு x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], (WT)src1[x]), x);
        }
        இல்லையெனில் எனில் (s0x == 0) {
            WT a = (WT)src0[0];
            க்கு (முழு x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, (WT)src1[x]), x);
        }
        இல்லையெனில் {
            WT b = (WT)src1[0];
            க்கு (முழு x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], b), x);
        }
    }
    #undef EW_CMP_APPLY
    திரும்பு 0;
}

// SIMD comparison kernel: T x T -> u8 mask, for the directly-comparable depths (u8/s8/u16/s16/u32/
// s32/f32). vec(a,b) gives an all-ones/zero mask in the operand's lane type; v_reinterpret_as turns it
// into the unsigned int of the same width; cmpFuse packs to u8 and applies (rawmask & M) | V. The
// per-row SIMD body runs when an operand is contiguous or broadcast; a per-channel patch (M/V differ
// by channel) only ever arrives as a short-row (width=cn) tile and is handled there.
template<typename T, typename Vvec, typename Uvec, வகுப்பு Cmp>
நிலையான முழு vecCompareKernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                            மாறிலி வெறுமை* src1_, size_t s1y, size_t s1x,
                            மாறிலி வெறுமை*, size_t, size_t,
                            வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                            மாறிலி இரட்டை*, முழு flags, வெறுமை*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    எனில் (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= அளவு(T);
    s1y /= அளவு(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);

    மாறிலி T* src0 = (மாறிலி T*)src0_;
    மாறிலி T* src1 = (மாறிலி T*)src1_;
    uchar* dst = (uchar*)dst_;
    மாறிலி uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];                                 // per-channel fix-up: (rawmask & m) | v
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    மாறிலி தருக்க perch = (flags & EW_CMP_PATCH) != 0;
    CV_Assert(!perch || width <= 4);                          // a per-channel patch is a short-row tile

    EW_TRY_COLLAPSE(2);
    முழு y = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    // Short rows (2/3/4 elements): a per-channel scalar over a multi-channel image arrives as a
    // (width=cn) x (height=pixels) tile with the scalar broadcast over rows (s?y==0). The per-row SIMD
    // below never triggers at such a tiny width, so expand the width<=4 broadcast operand (threshold
    // AND the interleaved per-channel mask/value bytes) across the lanes and compare many rows at once.
    // This is also the ONLY path a per-channel patch (M/V differ by channel) reaches.
    // constexpr: the short-row loads T natively (vx_load->Vvec), so it exists ONLY at native width. The
    // widened f16/bf16->f32 path (sizeof(T) < Vvec lane) and 8-byte f64 fall through to per-row + scalar.
    // (sizeof(T) <= 4 is part of the constexpr gate: for f64 the body would compile to nothing but
    // dead stores - gcc 9 flags scbuf/bsrc/bsx as set-but-not-used there.)
    எனில் constexpr (அளவு(T) == அளவு(typename VTraits<Vvec>::lane_type) && அளவு(T) <= 4)
    எனில் (height > 1 && width <= 4 &&
        ((s0y == 0 && s1y == (size_t)width*s1x) || (s1y == 0 && s0y == (size_t)width*s0x)) &&
        dsty == (size_t)width)
    {
        மாறிலி முழு VECSZ  = VTraits<Vvec>::vlanes();          // sizeof(T)-type lanes
        மாறிலி முழு VECSZ8 = VTraits<v_uint8>::vlanes();       // u8 output lanes
        constexpr முழு MAXV8 = VTraits<v_uint8>::max_nlanes;
        T scbuf[MAXV8 * 3] = {};                             // interleaved threshold (elements)
        uchar mbuf[MAXV8 * 3] = {}, vbuf[MAXV8 * 3] = {};    // interleaved mask / value (bytes)
        // ^ the {} inits are for -Wmaybe-uninitialized only: expandScalar/decode fill every lane that
        //   is later read, but the compiler cannot prove it with a runtime VECSZ
        மாறிலி தருக்க bc0 = (s0y == 0);                         // src0 is the broadcast (short) operand
        மாறிலி T* bsrc = bc0 ? src0 : src1; மாறிலி size_t bsx = bc0 ? s0x : s1x;
        மாறிலி T*& src = bc0 ? src1 : src0;                   // the row-stepping operand (advanced below)
        மாறிலி size_t sy = bc0 ? s1y : s0y;
        // The compare direction (scalar-first when bc0, array-first otherwise) is hoisted OUT of the row
        // loop with an explicit if(bc0) - the loop is written twice so no per-iteration branch remains.
        // Per branch: own unroll, threshold + interleaved M/V loaded ONCE. sizeof==1 keeps 12 vectors.
        #define EW_CMPR(r, a, b) v_reinterpret_as(Cmp::vec(a, b), r)   // rawmask -> sizeof(T)-byte uint
        எனில் constexpr (அளவு(T) == 1)                        // 3 u8 masks -> 3 u8 stores
        {
            மாறிலி முழு ewidth = VECSZ8 * 3;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, width, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, width, vbuf, ewidth);
            மாறிலி முழு dy = ewidth / width;
            Vvec  sc0=vx_load(scbuf),  sc1=vx_load(scbuf+VECSZ),   sc2=vx_load(scbuf+VECSZ*2);
            v_uint8 M0=vx_load(mbuf),  M1=vx_load(mbuf+VECSZ8),    M2=vx_load(mbuf+VECSZ8*2);
            v_uint8 V0=vx_load(vbuf),  V1=vx_load(vbuf+VECSZ8),    V2=vx_load(vbuf+VECSZ8*2);
            #define EW_ROW1(A0,B0,A1,B1,A2,B2) \
                க்கு (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 o0=v_or(v_and(r0,M0),V0),o1=v_or(v_and(r1,M1),V1),o2=v_or(v_and(r2,M2),V2); \
                    v_store(dst, o0); v_store(dst+VECSZ8, o1); v_store(dst+VECSZ8*2, o2); }
            எனில் (bc0) EW_ROW1(sc0,vx_load(src), sc1,vx_load(src+VECSZ), sc2,vx_load(src+VECSZ*2))
            இல்லையெனில்     EW_ROW1(vx_load(src),sc0, vx_load(src+VECSZ),sc1, vx_load(src+VECSZ*2),sc2)
            #undef EW_ROW1
        }
        இல்லையெனில் எனில் constexpr (அளவு(T) == 2)                   // 3 u16 masks -> 1 full + 1 half u8 store
        {
            மாறிலி முழு ewidth = VECSZ * 3;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, width, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, width, vbuf, ewidth);
            மாறிலி முழு dy = ewidth / width;
            Vvec  sc0=vx_load(scbuf), sc1=vx_load(scbuf+VECSZ), sc2=vx_load(scbuf+VECSZ*2);
            v_uint8 M0=vx_load(mbuf), M1=vx_load(mbuf+VECSZ8);
            v_uint8 V0=vx_load(vbuf), V1=vx_load(vbuf+VECSZ8);
            #define EW_ROW2(A0,B0,A1,B1,A2,B2) \
                க்கு (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 b0=v_pack(r0,r1), b1=v_pack(r2,r2); \
                    v_uint8 o0=v_or(v_and(b0,M0),V0), o1=v_or(v_and(b1,M1),V1); \
                    v_store(dst, o0); v_store_கீழ்(dst+VECSZ8, o1); }
            எனில் (bc0) EW_ROW2(sc0,vx_load(src), sc1,vx_load(src+VECSZ), sc2,vx_load(src+VECSZ*2))
            இல்லையெனில்     EW_ROW2(vx_load(src),sc0, vx_load(src+VECSZ),sc1, vx_load(src+VECSZ*2),sc2)
            #undef EW_ROW2
        }
        இல்லையெனில் எனில் constexpr (அளவு(T) == 4)                   // sizeof==4: 6 u32 -> 3 u16 -> 1 full + 1 half u8
        {
            மாறிலி முழு ewidth = VECSZ * 6;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, width, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, width, vbuf, ewidth);
            மாறிலி முழு dy = ewidth / width;
            Vvec sc0=vx_load(scbuf),sc1=vx_load(scbuf+VECSZ),sc2=vx_load(scbuf+VECSZ*2),
                 sc3=vx_load(scbuf+VECSZ*3),sc4=vx_load(scbuf+VECSZ*4),sc5=vx_load(scbuf+VECSZ*5);
            v_uint8 M0=vx_load(mbuf), M1=vx_load(mbuf+VECSZ8);
            v_uint8 V0=vx_load(vbuf), V1=vx_load(vbuf+VECSZ8);
            #define EW_ROW4(A0,B0,A1,B1,A2,B2,A3,B3,A4,B4,A5,B5) \
                க்கு (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2,r3,r4,r5; \
                    EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    EW_CMPR(r3,A3,B3); EW_CMPR(r4,A4,B4); EW_CMPR(r5,A5,B5); \
                    v_uint16 p0=v_pack(r0,r1), p1=v_pack(r2,r3), p2=v_pack(r4,r5); \
                    v_uint8 q0=v_pack(p0,p1), q1=v_pack(p2,p2); \
                    v_uint8 o0=v_or(v_and(q0,M0),V0), o1=v_or(v_and(q1,M1),V1); \
                    v_store(dst, o0); v_store_கீழ்(dst+VECSZ8, o1); }
            எனில் (bc0) EW_ROW4(sc0,vx_load(src),sc1,vx_load(src+VECSZ),sc2,vx_load(src+VECSZ*2),
                             sc3,vx_load(src+VECSZ*3),sc4,vx_load(src+VECSZ*4),sc5,vx_load(src+VECSZ*5))
            இல்லையெனில்     EW_ROW4(vx_load(src),sc0,vx_load(src+VECSZ),sc1,vx_load(src+VECSZ*2),sc2,
                             vx_load(src+VECSZ*3),sc3,vx_load(src+VECSZ*4),sc4,vx_load(src+VECSZ*5),sc5)
            #undef EW_ROW4
        }
        #undef EW_CMPR
    }
#endif
    க்கு (; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        முழு x = 0;
    #எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        // SIMD for BOTH the contiguous case AND a broadcast operand (a per-channel scalar const has
        // step 0 - without this the scalar-vs-array compare, incl. every multi-channel scalar compare,
        // fell to the scalar tail below, ~5x slower). vx_setall broadcasts the step-0 operand once.
        // 4-vector unroll + halide right-edge backoff (reprocess the last 4*VECSZ when width is not a
        // multiple); dst is a separate mask buffer so the overlap is a harmless idempotent rewrite.
        மாறிலி முழு VECSZ = VTraits<Vvec>::vlanes();
        மாறிலி முழு VECSZ8 = VTraits<v_uint8>::vlanes();
        v_uint8 vT; v_setall_mask(vT, trueVal);              // uniform mask (per-channel patch is short-row)
        மாறிலி தருக்க tailTrick = width >= 4*VECSZ && src0_ != dst_ && src1_ != dst_;
        // Narrow the 4 masks to u8 FIRST, then apply trueVal on the u8 result (1 and per u8 vector, not
        // per wide lane-group). One block per MASK lane width (== sizeof(T) natively, but 4 for the
        // f16/bf16->f32 widened path); a 4-vector unroll spans exactly 4*VECSZ elements.
        constexpr size_t MW = அளவு(typename VTraits<Uvec>::lane_type);
        #define PACK_STORE_CMP_RESULT(m0,m1,m2,m3, D) செய் { \
            எனில் constexpr (MW == 1u) { \
                v_uint8 o0=v_and(m0,vT),o1=v_and(m1,vT),o2=v_and(m2,vT),o3=v_and(m3,vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); v_store((D)+VECSZ8*2,o2); v_store((D)+VECSZ8*3,o3); \
            } இல்லையெனில் எனில் constexpr (MW == 2u) { \
                v_uint8 o0=v_and(v_pack(m0,m1),vT), o1=v_and(v_pack(m2,m3),vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); \
            } இல்லையெனில் எனில் constexpr (MW == 4u) { \
                v_uint8 o0=v_and(v_pack(v_pack(m0,m1),v_pack(m2,m3)),vT); \
                v_store((D),o0); \
            } இல்லையெனில் { /* sizeof==8: 4 u64 -> 2 u32 -> 1 u16 -> u8 (low half), 4*VECSZ elems -> store_low */ \
                v_uint16 g=v_pack(v_pack(m0,m1),v_pack(m2,m3)); \
                v_uint8 o0=v_and(v_pack(g,g),vT); \
                v_store_கீழ்((D),o0); \
            } } போது(0)
        எனில் (s0x == 1u && s1x == 1u)
        {
            க்கு (; x < width; x += 4*VECSZ)
            {
                எனில் (x + 4*VECSZ > width) { எனில் (!tailTrick) நிறுத்து; x = width - 4*VECSZ; }
                Vvec a0, a1, a2, a3, b0, b1, b2, b3;   // vx_load_pair_as widens f16/bf16->f32, else native
                vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+2*VECSZ, a2, a3);
                vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b1), m1);
                v_reinterpret_as(Cmp::vec(a2, b2), m2); v_reinterpret_as(Cmp::vec(a3, b3), m3);
                PACK_STORE_CMP_RESULT(m0, m1, m2, m3, dst+x);
            }
        }
        இல்லையெனில் எனில் (s1x == 0u)                       // src1 (e.g. the scalar) broadcast
        {
            Vvec b0; vx_setall_as(src1, b0);
            க்கு (; x < width; x += 4*VECSZ)
            {
                எனில் (x + 4*VECSZ > width) { எனில் (!tailTrick) நிறுத்து; x = width - 4*VECSZ; }
                Vvec a0, a1, a2, a3;
                vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+2*VECSZ, a2, a3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b0), m1);
                v_reinterpret_as(Cmp::vec(a2, b0), m2); v_reinterpret_as(Cmp::vec(a3, b0), m3);
                PACK_STORE_CMP_RESULT(m0, m1, m2, m3, dst+x);
            }
        }
        இல்லையெனில் எனில் (s0x == 0u)                       // src0 broadcast
        {
            Vvec a0; vx_setall_as(src0, a0);
            க்கு (; x < width; x += 4*VECSZ)
            {
                எனில் (x + 4*VECSZ > width) { எனில் (!tailTrick) நிறுத்து; x = width - 4*VECSZ; }
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
        எனில் (s0x == s1x) {
            க்கு (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], src1[x]), x);
        }
        இல்லையெனில் எனில் (s0x == 0) {
            T a = src0[0];
            க்கு (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, src1[x]), x);
        }
        இல்லையெனில் {
            T b = src1[0];
            க்கு (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], b), x);
        }
        #undef EW_CMP_APPLY
    }
    திரும்பு 0;
}

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
template<typename T, typename Tr, typename Wvec, typename WT, வகுப்பு Op, typename ST=WT, typename Wvec1=Wvec>
நிலையான முழு vec_இருமம்_Kernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                         மாறிலி வெறுமை* src1_, size_t s1y, size_t s1x,
                         மாறிலி வெறுமை*, size_t, size_t,
                         வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                         மாறிலி இரட்டை* params, முழு, வெறுமை*)
{
    எனில் (width <= 0 || height <= 0) திரும்பு 0;
    s0y /= அளவு(T);
    s1y /= அளவு(T);
    dsty /= அளவு(Tr);

    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);

    மாறிலி T* src0 = (மாறிலி T*)src0_;
    மாறிலி T* src1 = (மாறிலி T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    முழு y = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    using Wlane = typename VTraits<Wvec>::lane_type;
    மாறிலி முழு VECSZ = VTraits<Wvec>::vlanes();
    மாறிலி தருக்க use_tail_trick = width >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    [[maybe_unused]] Wvec vscalar;
    vx_setall_as(&scalar, vscalar);
    எனில் (height > 1 && width <= 4 &&
        ((s0y == 0 && s1y == width*s1x) ||
         (s1y == 0 && s0y == width*s0x)) &&
        dsty == (size_t)width) {
        constexpr முழு MAXVECSZ = VTraits<Wvec>::max_nlanes;
        Wlane scbuf[MAXVECSZ*6] = {};   // {} for -Wmaybe-uninitialized only (filled up to VECSZ*6)
        மாறிலி முழு ewidth = VECSZ*6;
        expandScalar(s0y == 0 ? src0 : src1, s0y == 0 ? s0x : s1x, width, scbuf, ewidth);
        முழு dy = ewidth / width;
        Wvec sc0, sc1, sc2, sc3, sc4, sc5;
        sc0 = Op::preproc(vx_load(scbuf), vscalar);
        sc1 = Op::preproc(vx_load(scbuf + VECSZ), vscalar);
        sc2 = Op::preproc(vx_load(scbuf + VECSZ*2), vscalar);
        sc3 = Op::preproc(vx_load(scbuf + VECSZ*3), vscalar);
        sc4 = Op::preproc(vx_load(scbuf + VECSZ*4), vscalar);
        sc5 = Op::preproc(vx_load(scbuf + VECSZ*5), vscalar);

        எனில் (s0y == 0) {
            க்கு (; y + dy <= height; y += dy, src1 += s1y*dy, dst += dsty*dy) {
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
        இல்லையெனில் {
            க்கு (; y + dy <= height; y += dy, src0 += s0y*dy, dst += dsty*dy) {
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
    க்கு (; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        முழு x = 0;
    #எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        Wvec a0, a1, a2, a3, b0, b1, b2, b3;
        எனில் (s0x == s1x) {
            எனில் (!Op::useScalar || scalar == ST(1)) {
                // this branch runs on Wvec1, which may be WIDER-laned than Wvec (e.g. the u8 mul
                // path: Wvec1=v_uint16 is 2x the lanes of Wvec=v_float32) - step by ITS lane count,
                // or the pairs overlap and the tail backoff writes past the row end
                மாறிலி முழு VECSZ1 = VTraits<Wvec1>::vlanes();
                மாறிலி தருக்க tail_trick1 = width >= VECSZ1*4 && src0_ != dst_ && src1_ != dst_;
                க்கு (; x < width; x += VECSZ1*4) {
                    Wvec1 a0_, a1_, a2_, a3_, b0_, b1_, b2_, b3_;
                    எனில் (x + VECSZ1*4 > width) { எனில் (!tail_trick1) நிறுத்து; x = width - VECSZ1*4; }
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
            இல்லையெனில் {
                க்கு (; x < width; x += VECSZ*4) {
                    எனில் (x + VECSZ*4 > width) { எனில் (!use_tail_trick) நிறுத்து; x = width - VECSZ*4; }
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
        இல்லையெனில் எனில் (s1x == 0) {
            vx_setall_as(src1, b0);
            b0 = Op::preproc(b0, vscalar);
            க்கு (; x < width; x += VECSZ*4) {
                எனில் (x + VECSZ*4 > width) { எனில் (!use_tail_trick) நிறுத்து; x = width - VECSZ*4; }
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src0 + x + VECSZ*2, a2, a3);
                auto c0 = Op::vec(a0, b0, vscalar), c1 = Op::vec(a1, b0, vscalar),
                     c2 = Op::vec(a2, b0, vscalar), c3 = Op::vec(a3, b0, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
        இல்லையெனில் {
            vx_setall_as(src0, b0);
            b0 = Op::preproc(b0, vscalar);
            க்கு (; x < width; x += VECSZ*4) {
                எனில் (x + VECSZ*4 > width) { எனில் (!use_tail_trick) நிறுத்து; x = width - VECSZ*4; }
                vx_load_pair_as(src1 + x, a0, a1);
                vx_load_pair_as(src1 + x + VECSZ*2, a2, a3);
                auto c0 = Op::vec(b0, a0, vscalar), c1 = Op::vec(b0, a1, vscalar),
                     c2 = Op::vec(b0, a2, vscalar), c3 = Op::vec(b0, a3, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
    #endif
        க்கு (; x < width; x++)
            dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x*s0x], (WT)src1[x*s1x], scalar));
    }
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    vx_cleanup();
#endif
    திரும்பு 0;
}

// ===========================================================================
// OP_ADD
// ===========================================================================

// add dispatch: T x T -> R (operands already same depth T).
//  - native saturating path (Wvec = native, use_simd=true) for T -> T on <=32-bit ints + f32
//    (32-bit lanes via the local v_add_sat/v_sub_sat - v_add/v_sub wrap there);
//  - widening f32-hub (Wvec = v_float32, use_simd=true) for the widened/half/float outputs;
//  - scalar (use_simd=false) for 64-bit results and f64 (no vector path yet).
template<வகுப்பு Op>
TKernel getAddSubFunc(முழு T, முழு R)
{
    KernelFunc fptr = nullptr;
    தேர்வு (T)
    {
    நிலை CV_8U:
        fptr = R == CV_8U ? vec_இருமம்_Kernel<uchar, uchar, v_uint8, short, Op, uchar> :
               R == CV_16S ? vec_இருமம்_Kernel<uchar, short, v_int16, short, Op, short> :
               R == CV_32S ? vec_இருமம்_Kernel<uchar, முழு, v_int16, short, Op, short> :
               R == CV_32F ? vec_இருமம்_Kernel<uchar, தசம, v_int16, short, Op, short> : nullptr;
        நிறுத்து;
    நிலை CV_8S:
        fptr =
            R == CV_8S ? vec_இருமம்_Kernel<schar, schar, v_int8, short, Op, schar> :
            R == CV_16S ? vec_இருமம்_Kernel<schar, short, v_int16, short, Op, short> :
            R == CV_32S ? vec_இருமம்_Kernel<schar, முழு, v_int16, short, Op, short> :
            R == CV_32F ? vec_இருமம்_Kernel<schar, தசம, v_int16, short, Op, short> : nullptr;
        நிறுத்து;
    நிலை CV_16U:
        fptr = R == CV_16U ? vec_இருமம்_Kernel<ushort, ushort, v_uint16, முழு, Op, ushort> :
               R == CV_32S ? vec_இருமம்_Kernel<ushort, முழு, v_int32, முழு, Op, முழு> :
               R == CV_32F ? vec_இருமம்_Kernel<ushort, தசம, v_int32, முழு, Op, முழு> : nullptr;
        நிறுத்து;
    நிலை CV_16S:
        fptr = R == CV_16S ? vec_இருமம்_Kernel<short, short, v_int16, முழு, Op, short> :
               R == CV_32S ? vec_இருமம்_Kernel<short, முழு,   v_int32, முழு, Op, முழு> :
               R == CV_32F ? vec_இருமம்_Kernel<short, தசம, v_int32, முழு, Op, முழு> : nullptr;
        நிறுத்து;
    நிலை CV_32U:
        fptr = R == CV_32U ? vec_இருமம்_Kernel<unsigned, unsigned, v_uint32, int64_t, Op, unsigned> :
                R == CV_64S ? scalar_இருமம்_Kernel<unsigned, int64_t, int64_t, Op> :
                R == CV_64F ? scalar_இருமம்_Kernel<unsigned, இரட்டை, int64_t, Op> : nullptr;
        நிறுத்து;
    நிலை CV_32S:
        fptr = R == CV_32S ? vec_இருமம்_Kernel<முழு, முழு, v_int32, int64_t, Op, முழு> :
               R == CV_64S ? scalar_இருமம்_Kernel<முழு, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_இருமம்_Kernel<முழு, இரட்டை, int64_t, Op> : nullptr;
        நிறுத்து;
    நிலை CV_64U:
        fptr = R == CV_64U ? scalar_இருமம்_Kernel<uint64_t, uint64_t, uint64_t, Op> :
               R == CV_64F ? scalar_இருமம்_Kernel<uint64_t, இரட்டை, இரட்டை, Op> : nullptr;
        நிறுத்து;
    நிலை CV_64S:
        fptr = R == CV_64S ? scalar_இருமம்_Kernel<int64_t, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_இருமம்_Kernel<int64_t, இரட்டை, இரட்டை, Op> : nullptr;
        நிறுத்து;
    நிலை CV_16F:
        fptr =
        #எனில் CV_SIMD_16F
        R == CV_16F ? vec_இருமம்_Kernel<hfloat, hfloat, v_float16, தசம, Op, hfloat> :
        #இல்லையெனில்
        R == CV_16F ? vec_இருமம்_Kernel<hfloat, hfloat, v_float32, தசம, Op, தசம> :
        #endif
        R == CV_32F ? vec_இருமம்_Kernel<hfloat, தசம, v_float32, தசம, Op, தசம> : nullptr;
        நிறுத்து;
    நிலை CV_16BF:
        fptr =
            R == CV_16BF ? vec_இருமம்_Kernel<bfloat, bfloat, v_float32, தசம, Op, தசம> :
            R == CV_32F ? vec_இருமம்_Kernel<bfloat, தசம,  v_float32, தசம, Op, தசம> : nullptr;
        நிறுத்து;
    நிலை CV_32F:
        fptr = R == CV_32F ? vec_இருமம்_Kernel<தசம, தசம, v_float32, தசம, Op, தசம> : nullptr;
        நிறுத்து;
    நிலை CV_64F:
        #எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_இருமம்_Kernel<இரட்டை, இரட்டை, v_float64, இரட்டை, Op> : nullptr;
        #இல்லையெனில்
        fptr = R == CV_64F ? scalar_இருமம்_Kernel<இரட்டை, இரட்டை, இரட்டை, Op> : nullptr;
        #endif
        நிறுத்து;
    இயல்பு:
        ;
    }
    திரும்பு {fptr, nullptr, 0};
}

TKernel getMulFunc_(முழு T, முழு R)
{
    KernelFunc fptr = nullptr;
    தேர்வு (T)
    {
    நிலை CV_8U:
        fptr =   // scale==1 fast path: whole u8 registers, v_mul_sat widens+clamps inside; Wvec=f32 for scale
            R == CV_8U ? vec_இருமம்_Kernel<uchar, uchar, v_float32, தசம, EwMul, தசம, v_uint8> :
            R == CV_32F ? vec_இருமம்_Kernel<uchar, தசம, v_float32, தசம, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_8S:
        fptr =   // scale==1 fast path: whole s8 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_8S ? vec_இருமம்_Kernel<schar, schar, v_float32, தசம, EwMul, தசம, v_int8> :
            R == CV_32F ? vec_இருமம்_Kernel<schar, தசம, v_float32, தசம, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_16U:
        fptr =   // scale==1 fast path: whole u16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16U ? vec_இருமம்_Kernel<ushort, ushort, v_float32, தசம, EwMul, தசம, v_uint16> :
            R == CV_32F ? vec_இருமம்_Kernel<ushort, தசம,  v_float32, தசம, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_16S:
        fptr =   // scale==1 fast path: whole s16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16S ? vec_இருமம்_Kernel<short, short, v_float32, தசம, EwMul, தசம, v_int16> :
            R == CV_32F ? vec_இருமம்_Kernel<short, தசம, v_float32, தசம, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_16F:
        fptr =
            R == CV_16F ? vec_இருமம்_Kernel<hfloat, hfloat, v_float32, தசம, EwMul> :
            R == CV_32F ? vec_இருமம்_Kernel<hfloat, தசம,  v_float32, தசம, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_16BF:
        fptr =
            R == CV_16BF ? vec_இருமம்_Kernel<bfloat, bfloat, v_float32, தசம, EwMul> :
            R == CV_32F ? vec_இருமம்_Kernel<bfloat, தசம,  v_float32, தசம, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_32F:
        fptr = R == CV_32F ? vec_இருமம்_Kernel<தசம, தசம, v_float32, தசம, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_32U:
        fptr =   // scale==1 fast path (where v_mul_sat32 exists): whole u32 registers, widening
                 // multiply + saturating narrow; the f64 work vector serves the scale path
        #எனில் defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32U ? vec_இருமம்_Kernel<unsigned, unsigned, v_float64, இரட்டை, EwMul, இரட்டை, v_uint32> :
            R == CV_64F ? vec_இருமம்_Kernel<unsigned, இரட்டை,   v_float64, இரட்டை, EwMul> : nullptr;
        #அல்லதெனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32U ? vec_இருமம்_Kernel<unsigned, unsigned, v_float64, இரட்டை, EwMul> :
            R == CV_64F ? vec_இருமம்_Kernel<unsigned, இரட்டை,   v_float64, இரட்டை, EwMul> : nullptr;
        #இல்லையெனில்
            R == CV_32U ? scalar_இருமம்_Kernel<unsigned, unsigned, இரட்டை, EwMul> :
            R == CV_64F ? scalar_இருமம்_Kernel<unsigned, இரட்டை,   இரட்டை, EwMul> : nullptr;
        #endif
        நிறுத்து;
    நிலை CV_32S:
        fptr =   // scale==1 fast path: see CV_32U
        #எனில் defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32S ? vec_இருமம்_Kernel<முழு, முழு,    v_float64, இரட்டை, EwMul, இரட்டை, v_int32> :
            R == CV_64F ? vec_இருமம்_Kernel<முழு, இரட்டை, v_float64, இரட்டை, EwMul> : nullptr;
        #அல்லதெனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32S ? vec_இருமம்_Kernel<முழு, முழு,    v_float64, இரட்டை, EwMul> :
            R == CV_64F ? vec_இருமம்_Kernel<முழு, இரட்டை, v_float64, இரட்டை, EwMul> : nullptr;
        #இல்லையெனில்
            R == CV_32S ? scalar_இருமம்_Kernel<முழு, முழு,    இரட்டை, EwMul> :
            R == CV_64F ? scalar_இருமம்_Kernel<முழு, இரட்டை, இரட்டை, EwMul> : nullptr;
        #endif
        நிறுத்து;
    நிலை CV_64U:
        fptr = R == CV_64F ? scalar_இருமம்_Kernel<uint64_t, இரட்டை, இரட்டை, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_64S:
        fptr = R == CV_64F ? scalar_இருமம்_Kernel<int64_t,  இரட்டை, இரட்டை, EwMul> : nullptr;
        நிறுத்து;
    நிலை CV_64F:
        #எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_இருமம்_Kernel<இரட்டை, இரட்டை, v_float64, இரட்டை, EwMul> : nullptr;
        #இல்லையெனில்
        fptr = R == CV_64F ? scalar_இருமம்_Kernel<இரட்டை,   இரட்டை, இரட்டை, EwMul> : nullptr;
        #endif
        நிறுத்து;
    இயல்பு:
        ;
    }
    திரும்பு {fptr, nullptr, 0};
}

// `checked` (decided by the CALLER from the ORIGINAL input types) selects the divide-by-zero
// policy, INDEPENDENT of the work type R: EwDivInt guards /0 -> 0 for integer-semantics division,
// EwDivFlt does not (a/0 -> inf, saturating on a later cast, as cv::divide does for float inputs).
// It must apply on EVERY row - two wide integers (e.g. 32U / 64S) promote to a 64F work type yet
// still need the integer guard, so the float-work rows can't hardcode EwDivFlt.
TKernel getDivFunc_(முழு T, முழு R, தருக்க checked)
{
    KernelFunc fptr = nullptr;
    // SIMD via vecBinaryKernel: div scales the numerator in vec (a*scale/b), EwDivFlt (float a/0 -> inf,
    // saturates on cast) / EwDivInt (v_select guards b==0 -> 0 post-facto). Prefer a DIRECT T->T kernel
    // (1 pass, saturate on store) - emitBinary probes result==T first; a T->f32/f64 kernel serves an
    // explicit float dtype. Work type: f32 for <=16-bit/f16/bf16/f32; f64 for 32/64-bit.
    #define DIV(T_, Tr_, Wv_, W_) (checked ? vec_இருமம்_Kernel<T_, Tr_, Wv_, W_, EwDivInt> \
                                           : vec_இருமம்_Kernel<T_, Tr_, Wv_, W_, EwDivFlt>)
#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #define DIVW(T_, Tr_) DIV(T_, Tr_, v_float64, இரட்டை)                 // 32/64-bit, f64 SIMD
#இல்லையெனில்
    #define DIVW(T_, Tr_) (checked ? scalar_இருமம்_Kernel<T_, Tr_, இரட்டை, EwDivInt> \
                                    : scalar_இருமம்_Kernel<T_, Tr_, இரட்டை, EwDivFlt>)
#endif
    தேர்வு (T)
    {
    நிலை CV_8U:   fptr = R==CV_8U  ? DIV(uchar,  uchar,  v_float32, தசம) : R==CV_32F ? DIV(uchar,  தசம, v_float32, தசம) : nullptr; நிறுத்து;
    நிலை CV_8S:   fptr = R==CV_8S  ? DIV(schar,  schar,  v_float32, தசம) : R==CV_32F ? DIV(schar,  தசம, v_float32, தசம) : nullptr; நிறுத்து;
    நிலை CV_16U:  fptr = R==CV_16U ? DIV(ushort, ushort, v_float32, தசம) : R==CV_32F ? DIV(ushort, தசம, v_float32, தசம) : nullptr; நிறுத்து;
    நிலை CV_16S:  fptr = R==CV_16S ? DIV(short,  short,  v_float32, தசம) : R==CV_32F ? DIV(short,  தசம, v_float32, தசம) : nullptr; நிறுத்து;
    நிலை CV_16F:  fptr = R==CV_16F ? DIV(hfloat, hfloat, v_float32, தசம) : R==CV_32F ? DIV(hfloat, தசம, v_float32, தசம) : nullptr; நிறுத்து;
    நிலை CV_16BF: fptr = R==CV_16BF? DIV(bfloat, bfloat, v_float32, தசம) : R==CV_32F ? DIV(bfloat, தசம, v_float32, தசம) : nullptr; நிறுத்து;
    நிலை CV_32F:  fptr = R==CV_32F ? DIV(தசம,  தசம,  v_float32, தசம) : nullptr; நிறுத்து;
    நிலை CV_32U:  fptr = R==CV_32U ? DIVW(unsigned, unsigned) : R==CV_64F ? DIVW(unsigned, இரட்டை) : nullptr; நிறுத்து;
    நிலை CV_32S:  fptr = R==CV_32S ? DIVW(முழு,      முழு)      : R==CV_64F ? DIVW(முழு,      இரட்டை) : nullptr; நிறுத்து;
    நிலை CV_64U:  fptr = R==CV_64F ? DIVW(uint64_t, இரட்டை) : nullptr; நிறுத்து;
    நிலை CV_64S:  fptr = R==CV_64F ? DIVW(int64_t,  இரட்டை) : nullptr; நிறுத்து;
    நிலை CV_64F:  fptr = R==CV_64F ? DIVW(இரட்டை,   இரட்டை) : nullptr; நிறுத்து;
    இயல்பு:      ;
    }
    #undef DIV
    #undef DIVW
    திரும்பு {fptr, nullptr, 0};
}


// min / max: T x T -> T for every depth. Native v_min/v_max on the matching lane type (8/16/32-bit
// ints, f16/bf16/f32); 64-bit ints use the scalar path. Op = EwMin or EwMax.
template<வகுப்பு Op>
நிலையான TKernel getMinMaxFunc(முழு T)
{
    KernelFunc fptr = nullptr;
    தேர்வு (T)
    {
    நிலை CV_8U:   fptr = vec_இருமம்_Kernel<uchar,    uchar,    v_uint8,   short,   Op, uchar>; நிறுத்து;
    நிலை CV_8S:   fptr = vec_இருமம்_Kernel<schar,    schar,    v_int8,    short,   Op, schar>; நிறுத்து;
    நிலை CV_16U:  fptr = vec_இருமம்_Kernel<ushort,   ushort,   v_uint16,  முழு,     Op, ushort>; நிறுத்து;
    நிலை CV_16S:  fptr = vec_இருமம்_Kernel<short,    short,    v_int16,   முழு,     Op, short>; நிறுத்து;
    நிலை CV_32U:  fptr = vec_இருமம்_Kernel<unsigned, unsigned, v_uint32,  int64_t, Op, unsigned>; நிறுத்து;
    நிலை CV_32S:  fptr = vec_இருமம்_Kernel<முழு,      முழு,      v_int32,   int64_t, Op, முழு>; நிறுத்து;
    நிலை CV_16F:
        #எனில் CV_SIMD_16F
        fptr = vec_இருமம்_Kernel<hfloat,   hfloat,   v_float16, தசம,   Op, hfloat>;
        #இல்லையெனில்
        fptr = vec_இருமம்_Kernel<hfloat,   hfloat,   v_float32, தசம,   Op, தசம>;
        #endif
        நிறுத்து;
    நிலை CV_16BF: fptr = vec_இருமம்_Kernel<bfloat, bfloat, v_float32, தசம, Op, தசம>; நிறுத்து;
    நிலை CV_32F:  fptr = vec_இருமம்_Kernel<தசம, தசம, v_float32, தசம, Op, தசம>; நிறுத்து;
    நிலை CV_64U:  fptr = scalar_இருமம்_Kernel<uint64_t, uint64_t, uint64_t, Op>; நிறுத்து;
    நிலை CV_64S:  fptr = scalar_இருமம்_Kernel<int64_t,  int64_t,  int64_t,  Op>; நிறுத்து;
    #எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    நிலை CV_64F:  fptr = vec_இருமம்_Kernel<இரட்டை, இரட்டை, v_float64, இரட்டை, Op, இரட்டை>; நிறுத்து;
    #இல்லையெனில்
    நிலை CV_64F:  fptr = scalar_இருமம்_Kernel<இரட்டை,   இரட்டை,   இரட்டை,   Op>; நிறுத்து;
    #endif
    இயல்பு:      ;
    }
    திரும்பு {fptr, nullptr, 0};
}

// absdiff: |a-b|, T x T -> R.
TKernel getAbsdiffFunc_(முழு T, முழு R)
{
    // Signed T has TWO outputs: R==T -> EwAbsdiffS (saturating |a-b| kept in the signed range, native work,
    // ONE pass - the depth cv::absdiff keeps); R==(the unsigned type of the same width) -> EwAbsdiff (the
    // true |a-b| via v_absdiff, for an explicit unsigned dst). Unsigned/float T only produce R==T. Each case
    // returns nullptr for a wrong R (no separate rdepth guard).
    KernelFunc fptr = nullptr;
    தேர்வு (T)
    {
    நிலை CV_8U:   fptr = R == CV_8U  ? vec_இருமம்_Kernel<uchar,  uchar,  v_uint8,  short,   EwAbsdiff, uchar>  : nullptr; நிறுத்து;
    நிலை CV_16U:  fptr = R == CV_16U ? vec_இருமம்_Kernel<ushort, ushort, v_uint16, முழு,     EwAbsdiff, ushort> : nullptr; நிறுத்து;
    நிலை CV_32U:  fptr = R == CV_32U ? vec_இருமம்_Kernel<unsigned, unsigned, v_uint32, int64_t, EwAbsdiff, unsigned> : nullptr; நிறுத்து;
    நிலை CV_8S:   fptr = R == CV_8S  ? vec_இருமம்_Kernel<schar, schar, v_int8,  short,   EwAbsdiffS, schar> :
                         R == CV_8U  ? vec_இருமம்_Kernel<schar, uchar, v_int8,  short,   EwAbsdiff,  schar> : nullptr; நிறுத்து;
    நிலை CV_16S:  fptr = R == CV_16S ? vec_இருமம்_Kernel<short, short, v_int16, முழு,     EwAbsdiffS, short> :
                         R == CV_16U ? vec_இருமம்_Kernel<short, ushort, v_int16, முழு,     EwAbsdiff,  short> : nullptr; நிறுத்து;
    நிலை CV_32S:  fptr = R == CV_32S ? vec_இருமம்_Kernel<முழு,   முழு,   v_int32, int64_t, EwAbsdiffS, முழு> :
                         R == CV_32U ? vec_இருமம்_Kernel<முழு, unsigned, v_int32, int64_t, EwAbsdiff,  முழு> : nullptr; நிறுத்து;
    #எனில் CV_SIMD_16F
    நிலை CV_16F:  fptr = R == CV_16F ? vec_இருமம்_Kernel<hfloat, hfloat, v_float16, தசம, EwAbsdiff, hfloat> : nullptr; நிறுத்து;
    #இல்லையெனில்
    நிலை CV_16F:  fptr = R == CV_16F ? vec_இருமம்_Kernel<hfloat, hfloat, v_float32, தசம, EwAbsdiff, தசம>  : nullptr; நிறுத்து;
    #endif
    நிலை CV_16BF: fptr = R == CV_16BF ? vec_இருமம்_Kernel<bfloat, bfloat, v_float32, தசம, EwAbsdiff, தசம> : nullptr; நிறுத்து;
    நிலை CV_32F:  fptr = R == CV_32F  ? vec_இருமம்_Kernel<தசம,  தசம,  v_float32, தசம, EwAbsdiff, தசம> : nullptr; நிறுத்து;
    நிலை CV_64U:  fptr = R == CV_64U  ? scalar_இருமம்_Kernel<uint64_t, uint64_t, uint64_t, EwAbsdiff> : nullptr; நிறுத்து;
    நிலை CV_64S:  fptr = R == CV_64S  ? scalar_இருமம்_Kernel<int64_t,  int64_t,  int64_t,  EwAbsdiffS> :
                         R == CV_64U  ? scalar_இருமம்_Kernel<int64_t,  uint64_t, int64_t,  EwAbsdiff> : nullptr; நிறுத்து;
    #எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    நிலை CV_64F:  fptr = R == CV_64F  ? vec_இருமம்_Kernel<இரட்டை, இரட்டை, v_float64, இரட்டை, EwAbsdiff, இரட்டை> : nullptr; நிறுத்து;
    #இல்லையெனில்
    நிலை CV_64F:  fptr = R == CV_64F  ? scalar_இருமம்_Kernel<இரட்டை, இரட்டை, இரட்டை, EwAbsdiff> : nullptr; நிறுத்து;
    #endif
    இயல்பு:      ;
    }
    திரும்பு {fptr, nullptr, 0};
}

// compare: T x T -> u8 mask. Directly-comparable depths (u8/s8/u16/s16/u32/s32/f32) take the SIMD
// vecCompareKernel; f16/bf16 and 64-bit int depths fall back to scalarCompareKernel. The returned kernel
// defaults to a 255 mask in TKernel::flags (cv::compare-compatible); kernel.flags=1 gives a 0/1 mask.
template<வகுப்பு Cmp>
நிலையான KernelFunc compareByType(முழு T)
{
    தேர்வு (T)
    {
    நிலை CV_8U:   திரும்பு vecCompareKernel<uchar,    v_uint8,   v_uint8,  Cmp>;
    நிலை CV_8S:   திரும்பு vecCompareKernel<schar,    v_int8,    v_uint8,  Cmp>;
    நிலை CV_16U:  திரும்பு vecCompareKernel<ushort,   v_uint16,  v_uint16, Cmp>;
    நிலை CV_16S:  திரும்பு vecCompareKernel<short,    v_int16,   v_uint16, Cmp>;
    நிலை CV_32U:  திரும்பு vecCompareKernel<unsigned, v_uint32,  v_uint32, Cmp>;
    நிலை CV_32S:  திரும்பு vecCompareKernel<முழு,      v_int32,   v_uint32, Cmp>;
    நிலை CV_32F:  திரும்பு vecCompareKernel<தசம,    v_float32, v_uint32, Cmp>;
    #எனில் CV_SIMD_16F
    நிலை CV_16F:  திரும்பு vecCompareKernel<hfloat,   v_float16, v_uint16, Cmp>;
    #இல்லையெனில்
    நிலை CV_16F:  திரும்பு vecCompareKernel<hfloat,   v_float32, v_uint32, Cmp>;   // widen f16->f32
    #endif
    நிலை CV_16BF: திரும்பு vecCompareKernel<bfloat,   v_float32, v_uint32, Cmp>;   // widen bf16->f32 (no native)
    நிலை CV_64U:  திரும்பு scalarCompareKernel<uint64_t, uint64_t, Cmp>;
    நிலை CV_64S:  திரும்பு scalarCompareKernel<int64_t,  int64_t,  Cmp>;
    #எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    நிலை CV_64F:  திரும்பு vecCompareKernel<இரட்டை,   v_float64, v_uint64, Cmp>;
    #இல்லையெனில்
    நிலை CV_64F:  திரும்பு scalarCompareKernel<இரட்டை,   இரட்டை,   Cmp>;
    #endif
    இயல்பு:      திரும்பு nullptr;
    }
}

TKernel getCmpFunc_(உச்சி op, முழு T)
{
    // Only 4 physical kernels (eq/ne/gt/ge): LT/LE reuse GT/GE with the operands swapped
    // (a<b == b>a, a<=b == b>=a) via the EW_KERNEL_SWAP01 flag, honored by the executor.
    KernelFunc f = nullptr;
    முழு flags = 0;              // mask value: 0 flag bits => 0/255 (cv::compare); EW_KERNEL_MASK1 => 0/1
    தேர்வு (op)
    {
    நிலை OP_CMP_EQ: f = compareByType<EwCmpEq>(T); நிறுத்து;
    நிலை OP_CMP_NE: f = compareByType<EwCmpNe>(T); நிறுத்து;
    நிலை OP_CMP_GT: f = compareByType<EwCmpGt>(T); நிறுத்து;
    நிலை OP_CMP_GE: f = compareByType<EwCmpGe>(T); நிறுத்து;
    நிலை OP_CMP_LT: f = compareByType<EwCmpGt>(T); flags = EW_KERNEL_SWAP01; நிறுத்து;
    நிலை OP_CMP_LE: f = compareByType<EwCmpGe>(T); flags = EW_KERNEL_SWAP01; நிறுத்து;
    இயல்பு:        ;
    }
    திரும்பு {f, nullptr, flags};
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
நிலையான முழு notKernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                     மாறிலி வெறுமை*, size_t, size_t, மாறிலி வெறுமை*, size_t, size_t,
                     வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                     மாறிலி இரட்டை*, முழு, வெறுமை*)
{
    எனில் (width <= 0 || height <= 0) திரும்பு 0;
    s0y /= அளவு(T);
    dsty /= அளவு(T);
    CV_Assert(s0x == 1u || width == 1);
    மாறிலி T* src0 = (மாறிலி T*)src0_;
    T* dst = (T*)dst_;
    எனில் (height > 1 && dsty == (size_t)width && s0y == (size_t)width) { width *= height; height = 1; }
    க்கு (முழு y = 0; y < height; y++, src0 += s0y, dst += dsty)
    {
        முழு x = 0;
    #எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        எனில் constexpr (அளவு(T) <= 4)
        {
            மாறிலி முழு VECSZ = VTraits<Vvec>::vlanes();
            க்கு (; x <= width - VECSZ; x += VECSZ)
                v_store(dst + x, v_not(vx_load(src0 + x)));
        }
    #endif
        க்கு (; x < width; x++) dst[x] = (T)~src0[x];
    }
    திரும்பு 0;
}

template<வகுப்பு Op>
நிலையான KernelFunc bitwiseByEsz(முழு esz)
{
    தேர்வு (esz)
    {
    நிலை 1: திரும்பு vec_இருமம்_Kernel<uint8_t,  uint8_t,  v_uint8,  uint8_t,  Op, uint8_t>;
    நிலை 2: திரும்பு vec_இருமம்_Kernel<uint16_t, uint16_t, v_uint16, uint16_t, Op, uint16_t>;
    நிலை 4: திரும்பு vec_இருமம்_Kernel<uint32_t, uint32_t, v_uint32, uint32_t, Op, uint32_t>;
    நிலை 8: திரும்பு scalar_இருமம்_Kernel<uint64_t, uint64_t, uint64_t, Op>;
    இயல்பு: திரும்பு nullptr;
    }
}

TKernel getBitwiseFunc_(உச்சி op, முழு esz)
{
    KernelFunc f = nullptr;
    தேர்வு (op)
    {
    நிலை OP_AND: f = bitwiseByEsz<EwAnd>(esz); நிறுத்து;
    நிலை OP_OR:  f = bitwiseByEsz<EwOr >(esz); நிறுத்து;
    நிலை OP_XOR: f = bitwiseByEsz<EwXor>(esz); நிறுத்து;
    இயல்பு:     ;
    }
    திரும்பு {f, nullptr, 0};
}

TKernel getNotFunc_(முழு esz)
{
    KernelFunc f = nullptr;
    தேர்வு (esz)
    {
    நிலை 1: f = notKernel<uint8_t,  v_uint8 >; நிறுத்து;
    நிலை 2: f = notKernel<uint16_t, v_uint16>; நிறுத்து;
    நிலை 4: f = notKernel<uint32_t, v_uint32>; நிறுத்து;
    நிலை 8: f = notKernel<uint64_t, v_uint32>; நிறுத்து;   // SIMD path compiled out for 8-byte -> scalar ~
    இயல்பு: ;
    }
    திரும்பு {f, nullptr, 0};
}

// ===========================================================================
// OP_ADDW (addWeighted): dst = a*alpha + b*beta + gamma, params[0..2] = {alpha, beta, gamma}. Two fused
// v_fma in the work type Wvec - f32 SIMD for u8/s8/u16/s16/f16/bf16/f32; the 32-bit-int/64-bit group
// works in f64 (v_float64 SIMD under CV_SIMD_64F || CV_SIMD_SCALABLE_64F, else use_simd=false scalar). Like vecBinaryKernel but
// WITHOUT its multi-channel short-row
// branch: addWeighted takes plain scalar coefficients (a multi-channel scalar is not optimized, matching
// the classic function). The broadcast branches fold the constant operand's contribution once.
// ===========================================================================
template<typename T, typename Tr, typename Wvec, typename WT, தருக்க use_simd>
நிலையான முழு addWeightedKernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                             மாறிலி வெறுமை* src1_, size_t s1y, size_t s1x,
                             மாறிலி வெறுமை*, size_t, size_t,
                             வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                             மாறிலி இரட்டை* params, முழு, வெறுமை*)
{
    எனில் (width <= 0 || height <= 0) திரும்பு 0;
    s0y /= அளவு(T); s1y /= அளவு(T); dsty /= அளவு(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)width == 1u);
    மாறிலி T* src0 = (மாறிலி T*)src0_;
    மாறிலி T* src1 = (மாறிலி T*)src1_;
    Tr* dst = (Tr*)dst_;
    மாறிலி WT alpha = (WT)params[0], beta = (WT)params[1], gamma = (WT)params[2];
    EW_TRY_COLLAPSE(2);
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    Wvec va{}, vb{}, vg{};
    மாறிலி முழு VECSZ = VTraits<Wvec>::vlanes();
    மாறிலி தருக்க tail = width >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    எனில் constexpr (use_simd) {
        WT fa=alpha, fb=beta, fg=gamma;
        vx_setall_as(&fa, va); vx_setall_as(&fb, vb); vx_setall_as(&fg, vg);
    }
#endif
    க்கு (முழு y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        முழு x = 0;
    #எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        எனில் constexpr (use_simd)
        {
            Wvec a0,a1,a2,a3,b0,b1,b2,b3;
            எனில் (s0x == s1x) {                                // both arrays contiguous
                க்கு (; x < width; x += VECSZ*4) {
                    எனில் (x+VECSZ*4 > width) { எனில் (!tail) நிறுத்து; x = width-VECSZ*4; }
                    vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+VECSZ*2, a2, a3);
                    vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+VECSZ*2, b2, b3);
                    a0=v_fma(a0,va,v_fma(b0,vb,vg)); a1=v_fma(a1,va,v_fma(b1,vb,vg));
                    a2=v_fma(a2,va,v_fma(b2,vb,vg)); a3=v_fma(a3,va,v_fma(b3,vb,vg));
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            இல்லையெனில் எனில் (s1x == 0) {                             // src1 broadcast: b*beta+gamma is constant
                Wvec bb; vx_setall_as(src1, bb); Wvec bc = v_fma(bb, vb, vg);
                க்கு (; x < width; x += VECSZ*4) {
                    எனில் (x+VECSZ*4 > width) { எனில் (!tail) நிறுத்து; x = width-VECSZ*4; }
                    vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+VECSZ*2, a2, a3);
                    a0=v_fma(a0,va,bc); a1=v_fma(a1,va,bc); a2=v_fma(a2,va,bc); a3=v_fma(a3,va,bc);
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            இல்லையெனில் {                                           // src0 broadcast: a*alpha+gamma is constant
                Wvec aa; vx_setall_as(src0, aa); Wvec acg = v_fma(aa, va, vg);
                க்கு (; x < width; x += VECSZ*4) {
                    எனில் (x+VECSZ*4 > width) { எனில் (!tail) நிறுத்து; x = width-VECSZ*4; }
                    vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+VECSZ*2, b2, b3);
                    b0=v_fma(b0,vb,acg); b1=v_fma(b1,vb,acg); b2=v_fma(b2,vb,acg); b3=v_fma(b3,vb,acg);
                    v_store_pair_as(dst+x, b0, b1); v_store_pair_as(dst+x+VECSZ*2, b2, b3);
                }
            }
        }
    #endif
        எனில் (s0x == s1x) {
            க்கு (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + (WT)src1[x]*beta + gamma);
        } இல்லையெனில் எனில் (s0x == 0) {
            மாறிலி WT ac = (WT)src0[0]*alpha + gamma;
            க்கு (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src1[x]*beta + ac);
        } இல்லையெனில் {
            மாறிலி WT bc = (WT)src1[0]*beta + gamma;
            க்கு (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + bc);
        }
    }
    திரும்பு 0;
}

TKernel getAddWeightedFunc_(முழு T, முழு R)
{
    KernelFunc f = nullptr;
    #define AWS(Tt, Rr) addWeightedKernel<Tt, Rr, v_float32, தசம, உண்மை>
#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #define AWD(Tt, Rr) addWeightedKernel<Tt, Rr, v_float64, இரட்டை, உண்மை>       // f64 SIMD
#இல்லையெனில்
    #define AWD(Tt, Rr) addWeightedKernel<Tt, Rr, v_float32, இரட்டை, பொய்>      // scalar (v_float32 unused)
#endif
    தேர்வு (T)
    {
    நிலை CV_8U:  f = R==CV_8U  ? AWS(uint8_t,  uint8_t)  : R==CV_32F ? AWS(uint8_t,  தசம) : nullptr; நிறுத்து;
    நிலை CV_8S:  f = R==CV_8S  ? AWS(int8_t,   int8_t)   : R==CV_32F ? AWS(int8_t,   தசம) : nullptr; நிறுத்து;
    நிலை CV_16U: f = R==CV_16U ? AWS(uint16_t, uint16_t) : R==CV_32F ? AWS(uint16_t, தசம) : nullptr; நிறுத்து;
    நிலை CV_16S: f = R==CV_16S ? AWS(int16_t,  int16_t)  : R==CV_32F ? AWS(int16_t,  தசம) : nullptr; நிறுத்து;
    நிலை CV_16F: f = R==CV_16F ? AWS(hfloat,   hfloat)   : R==CV_32F ? AWS(hfloat,   தசம) : nullptr; நிறுத்து;
    நிலை CV_16BF:f = R==CV_32F ? AWS(bfloat,   தசம) : nullptr; நிறுத்து;
    நிலை CV_32F: f = R==CV_32F ? AWS(தசம,    தசம) : nullptr; நிறுத்து;
    நிலை CV_32U: f = R==CV_32U ? AWD(unsigned, unsigned) : R==CV_64F ? AWD(unsigned, இரட்டை) : nullptr; நிறுத்து;
    நிலை CV_32S: f = R==CV_32S ? AWD(முழு,      முழு)      : R==CV_64F ? AWD(முழு,      இரட்டை) : nullptr; நிறுத்து;
    நிலை CV_64U: f = R==CV_64F ? AWD(uint64_t, இரட்டை) : nullptr; நிறுத்து;
    நிலை CV_64S: f = R==CV_64F ? AWD(int64_t,  இரட்டை) : nullptr; நிறுத்து;
    நிலை CV_64F: f = R==CV_64F ? AWD(இரட்டை,   இரட்டை) : nullptr; நிறுத்து;
    இயல்பு: ;
    }
    #undef AWS
    #undef AWD
    திரும்பு {f, nullptr, 0};
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
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
// VECSZ(T) mask bytes -> T-width lanes (u8 direct, u16/u32 via expand)
நிலையான உள்வரி v_uint8  loadSelectMask(மாறிலி uchar* m, மாறிலி v_uint8&)  { திரும்பு vx_load(m); }
நிலையான உள்வரி v_uint16 loadSelectMask(மாறிலி uchar* m, மாறிலி v_uint16&) { திரும்பு vx_load_expand(m); }
நிலையான உள்வரி v_uint32 loadSelectMask(மாறிலி uchar* m, மாறிலி v_uint32&) { திரும்பு vx_load_expand_q(m); }

நிலையான உள்வரி வெறுமை setallSelect(மாறிலி uint8_t*  p, v_uint8&  a) { a = vx_setall_u8(*p); }
நிலையான உள்வரி வெறுமை setallSelect(மாறிலி uint16_t* p, v_uint16& a) { a = vx_setall_u16(*p); }
நிலையான உள்வரி வெறுமை setallSelect(மாறிலி uint32_t* p, v_uint32& a) { a = vx_setall_u32(*p); }
#endif

template<typename T, typename Tvec>
நிலையான முழு selectKernel(மாறிலி வெறுமை* mask_, size_t smy, size_t smx,
                        மாறிலி வெறுமை* src1_, size_t s1y, size_t s1x,
                        மாறிலி வெறுமை* src2_, size_t s2y, size_t s2x,
                        வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                        மாறிலி இரட்டை*, முழு, வெறுமை*)
{
    s1y /= அளவு(T);                    // the 1-byte mask's smy stays in bytes == elements
    s2y /= அளவு(T);
    dsty /= அளவு(T);
    CV_Assert(smx <= 1u && s1x <= 1u && s2x <= 1u);

    மாறிலி uchar* mask = (மாறிலி uchar*)mask_;
    மாறிலி T* src1 = (மாறிலி T*)src1_;
    மாறிலி T* src2 = (மாறிலி T*)src2_;
    T* dst = (T*)dst_;

    எனில் (height > 1 && dsty == (size_t)width && smy == smx*(size_t)width &&
        s1y == s1x*(size_t)width && s2y == s2x*(size_t)width)
    { width *= height; height = 1; }

    முழு y = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
    எனில் constexpr (அளவு(T) <= 4)
    {
        மாறிலி முழு VECSZ = VTraits<Tvec>::vlanes();
        மாறிலி Tvec z = v_setzero_<Tvec>();

        // n-channel data under a per-pixel mask (the masked-op shape): channel axis = width (2..4),
        // one mask byte per row. Process VECSZ rows per iteration - expand the mask once and
        // interleave it across the channel lanes.
        எனில் (height > VECSZ && 2 <= width && width <= 4 && smx == 0u && smy == 1u &&
            s1x == 1u && s1y == (size_t)width && s2x == 1u && s2y == (size_t)width &&
            dsty == (size_t)width)
        {
            constexpr முழு MAXVECSZ = VTraits<Tvec>::max_nlanes;
            T maskbuf[MAXVECSZ*4] = {};      // {} for -Wmaybe-uninitialized only (fully written)
            மாறிலி முழு dy = VECSZ;
            க்கு (; y + dy <= height; y += dy, mask += dy, src1 += width*dy, src2 += width*dy,
                                                          dst += width*dy)
            {
                Tvec m0 = v_ne(loadSelectMask(mask, z), z), m1, m2, m3;
                எனில் (width == 2)      v_store_interleave(maskbuf, m0, m0);
                இல்லையெனில் எனில் (width == 3) v_store_interleave(maskbuf, m0, m0, m0);
                இல்லையெனில்                 v_store_interleave(maskbuf, m0, m0, m0, m0);
                m0 = vx_load(maskbuf);
                m1 = vx_load(maskbuf + VECSZ);
                எனில் (width > 2) m2 = vx_load(maskbuf + VECSZ*2);
                எனில் (width > 3) m3 = vx_load(maskbuf + VECSZ*3);
                v_store(dst, v_select(m0, vx_load(src1), vx_load(src2)));
                v_store(dst + VECSZ, v_select(m1, vx_load(src1 + VECSZ), vx_load(src2 + VECSZ)));
                எனில் (width > 2)
                    v_store(dst + VECSZ*2,
                            v_select(m2, vx_load(src1 + VECSZ*2), vx_load(src2 + VECSZ*2)));
                எனில் (width > 3)
                    v_store(dst + VECSZ*3,
                            v_select(m3, vx_load(src1 + VECSZ*3), vx_load(src2 + VECSZ*3)));
            }
            // the remaining < VECSZ rows fall through to the per-row path below
        }
        இல்லையெனில் எனில் (smx == 1)                                   // per-element mask - the common case
        {
            // a branch aliasing dst is fine under the backoff (idempotent select); dst == mask is not
            மாறிலி தருக்க use_tail_trick = width >= VECSZ*2 && dst_ != mask_;
            Tvec a, b;
            க்கு (; y < height; y++, mask += smy, src1 += s1y, src2 += s2y, dst += dsty)
            {
                // a row-constant branch still changes per row
                எனில் (s1x == 0) setallSelect(src1, a);
                எனில் (s2x == 0) setallSelect(src2, b);
                முழு x = 0;
                க்கு (; x < width; x += VECSZ)
                {
                    எனில் (x + VECSZ > width) { எனில் (!use_tail_trick || x == 0) நிறுத்து; x = width - VECSZ; }
                    Tvec m = v_ne(loadSelectMask(mask + x, z), z);
                    எனில் (s1x) a = vx_load(src1 + x);
                    எனில் (s2x) b = vx_load(src2 + x);
                    v_store(dst + x, v_select(m, a, b));
                }
                க்கு (; x < width; x++)
                    dst[x] = mask[x] != 0 ? src1[x*s1x] : src2[x*s2x];
            }
            திரும்பு 0;
        }
    }
#endif
    க்கு (; y < height; y++, mask += smy, src1 += s1y, src2 += s2y, dst += dsty)
    {
        எனில் (smx == 0)                     // one mask byte per row (n-channel data / broadcast mask)
        {
            மாறிலி T* s = mask[0] != 0 ? src1 : src2;
            மாறிலி size_t sx = mask[0] != 0 ? s1x : s2x;
            எனில் ((மாறிலி வெறுமை*)s != (மாறிலி வெறுமை*)dst)      // row select from dst itself is a no-op
                க்கு (முழு x = 0; x < width; x++) dst[x] = s[x*sx];
        }
        இல்லையெனில்
            க்கு (முழு x = 0; x < width; x++)
                dst[x] = mask[x] != 0 ? src1[x*s1x] : src2[x*s2x];
    }
    திரும்பு 0;
}

TKernel getSelectFunc_(முழு mdepth, முழு T)
{
    எனில் (CV_ELEM_SIZE1(mdepth) != 1)      // the mask must be a 1-byte type (u8/s8/bool)
        திரும்பு {};
    KernelFunc fptr = nullptr;
    தேர்வு (CV_ELEM_SIZE1(T))
    {
    நிலை 1: fptr = selectKernel<uint8_t,  v_uint8 >; நிறுத்து;
    நிலை 2: fptr = selectKernel<uint16_t, v_uint16>; நிறுத்து;
    நிலை 4: fptr = selectKernel<uint32_t, v_uint32>; நிறுத்து;
    நிலை 8: fptr = selectKernel<uint64_t, v_uint32>; நிறுத்து;   // SIMD path compiled out -> scalar
    இயல்பு: ;
    }
    திரும்பு {fptr, nullptr, 0};
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
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
நிலையான உள்வரி வெறுமை setallClamp(மாறிலி uchar*  p, v_uint8&   a) { a = vx_setall_u8(*p); }
நிலையான உள்வரி வெறுமை setallClamp(மாறிலி schar*  p, v_int8&    a) { a = vx_setall_s8(*p); }
நிலையான உள்வரி வெறுமை setallClamp(மாறிலி ushort* p, v_uint16&  a) { a = vx_setall_u16(*p); }
நிலையான உள்வரி வெறுமை setallClamp(மாறிலி short*  p, v_int16&   a) { a = vx_setall_s16(*p); }
நிலையான உள்வரி வெறுமை setallClamp(மாறிலி unsigned* p, v_uint32& a) { a = vx_setall_u32(*p); }
நிலையான உள்வரி வெறுமை setallClamp(மாறிலி முழு*    p, v_int32&   a) { a = vx_setall_s32(*p); }
நிலையான உள்வரி வெறுமை setallClamp(மாறிலி தசம*  p, v_float32& a) { a = vx_setall_f32(*p); }
#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
நிலையான உள்வரி வெறுமை setallClamp(மாறிலி இரட்டை* p, v_float64& a) { a = vx_setall_f64(*p); }
#endif
#endif

template<typename T, typename Tvec>
நிலையான முழு clampKernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                       மாறிலி வெறுமை* lo_, size_t s1y, size_t s1x,
                       மாறிலி வெறுமை* hi_, size_t s2y, size_t s2x,
                       வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                       மாறிலி இரட்டை*, முழு, வெறுமை*)
{
    s0y /= அளவு(T);
    s1y /= அளவு(T);
    s2y /= அளவு(T);
    dsty /= அளவு(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);

    மாறிலி T* src0 = (மாறிலி T*)src0_;
    மாறிலி T* lo = (மாறிலி T*)lo_;
    மாறிலி T* hi = (மாறிலி T*)hi_;
    T* dst = (T*)dst_;

    எனில் (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width &&
        s1y == s1x*(size_t)width && s2y == s2x*(size_t)width)
    { width *= height; height = 1; }

    க்கு (முழு y = 0; y < height; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
    {
        முழு x = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
        எனில் (s0x == 1)
        {
            மாறிலி முழு VECSZ = VTraits<Tvec>::vlanes();
            மாறிலி தருக்க use_tail_trick = width >= VECSZ*2 && lo_ != dst_ && hi_ != dst_;
            Tvec vlo, vhi;
            எனில் (s1x == 0) setallClamp(lo, vlo);
            எனில் (s2x == 0) setallClamp(hi, vhi);
            க்கு (; x < width; x += VECSZ)
            {
                எனில் (x + VECSZ > width) { எனில் (!use_tail_trick || x == 0) நிறுத்து; x = width - VECSZ; }
                எனில் (s1x) vlo = vx_load(lo + x);
                எனில் (s2x) vhi = vx_load(hi + x);
                v_store(dst + x, v_min(v_max(vx_load(src0 + x), vlo), vhi));
            }
        }
#endif
        க்கு (; x < width; x++)
        {
            T v = src0[x*s0x], l = lo[x*s1x], h = hi[x*s2x];
            dst[x] = std::min(std::max(v, l), h);
        }
    }
    திரும்பு 0;
}

// Scalar-only clamp for the depths without a native SIMD lane type here (f16/bf16 - compared in
// float; 64-bit ints).
template<typename T, typename WT>
நிலையான முழு scalarClampKernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                             மாறிலி வெறுமை* lo_, size_t s1y, size_t s1x,
                             மாறிலி வெறுமை* hi_, size_t s2y, size_t s2x,
                             வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                             மாறிலி இரட்டை*, முழு, வெறுமை*)
{
    s0y /= அளவு(T); s1y /= அளவு(T); s2y /= அளவு(T); dsty /= அளவு(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);
    மாறிலி T* src0 = (மாறிலி T*)src0_;
    மாறிலி T* lo = (மாறிலி T*)lo_;
    மாறிலி T* hi = (மாறிலி T*)hi_;
    T* dst = (T*)dst_;
    எனில் (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width &&
        s1y == s1x*(size_t)width && s2y == s2x*(size_t)width)
    { width *= height; height = 1; }
    க்கு (முழு y = 0; y < height; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
        க்கு (முழு x = 0; x < width; x++)
        {
            WT v = (WT)src0[x*s0x], l = (WT)lo[x*s1x], h = (WT)hi[x*s2x];
            dst[x] = saturate_cast<T>(std::min(std::max(v, l), h));
        }
    திரும்பு 0;
}

TKernel getClampFunc_(முழு T)
{
    KernelFunc fptr = nullptr;
    தேர்வு (T)
    {
    நிலை CV_8U:   fptr = clampKernel<uchar,    v_uint8  >; நிறுத்து;
    நிலை CV_8S:   fptr = clampKernel<schar,    v_int8   >; நிறுத்து;
    நிலை CV_16U:  fptr = clampKernel<ushort,   v_uint16 >; நிறுத்து;
    நிலை CV_16S:  fptr = clampKernel<short,    v_int16  >; நிறுத்து;
    நிலை CV_32U:  fptr = clampKernel<unsigned, v_uint32 >; நிறுத்து;
    நிலை CV_32S:  fptr = clampKernel<முழு,      v_int32  >; நிறுத்து;
    நிலை CV_32F:  fptr = clampKernel<தசம,    v_float32>; நிறுத்து;
#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    நிலை CV_64F:  fptr = clampKernel<இரட்டை,   v_float64>; நிறுத்து;
#இல்லையெனில்
    நிலை CV_64F:  fptr = scalarClampKernel<இரட்டை, இரட்டை>; நிறுத்து;
#endif
    நிலை CV_16F:  fptr = scalarClampKernel<hfloat, தசம>; நிறுத்து;
    நிலை CV_16BF: fptr = scalarClampKernel<bfloat, தசம>; நிறுத்து;
    நிலை CV_64U:  fptr = scalarClampKernel<uint64_t, uint64_t>; நிறுத்து;
    நிலை CV_64S:  fptr = scalarClampKernel<int64_t,  int64_t >; நிறுத்து;
    இயல்பு: ;
    }
    திரும்பு {fptr, nullptr, 0};
}

நிலையான முழு expandKernel(size_t s0y, size_t s0x,
                        வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                        முழு esz1)
{
    uchar* dst = (uchar*)dst_;
    எனில் (s0x == 0u && width > 1) {
        முழு h = s0y > 0u ? height : 1;
        CV_Assert(esz1 == 1 || esz1 == 2 || esz1 == 4 || esz1 == 8);
        க்கு (முழு y = 0; y < h; y++) {
            எனில் (esz1 == 1) {
                uchar* rowptr = dst + dsty*y;
                uchar val = rowptr[0];
                க்கு (முழு x = 1; x < width; x++) rowptr[x] = val;
            }
            இல்லையெனில் எனில் (esz1 == 2) {
                ushort* rowptr = (ushort*)(dst + dsty*y);
                ushort val = rowptr[0];
                க்கு (முழு x = 1; x < width; x++) rowptr[x] = val;
            }
            இல்லையெனில் எனில் (esz1 == 4) {
                unsigned* rowptr = (unsigned*)(dst + dsty*y);
                unsigned val = rowptr[0];
                க்கு (முழு x = 1; x < width; x++) rowptr[x] = val;
            }
            இல்லையெனில் {
                uint64_t* rowptr = (uint64_t*)(dst + dsty*y);
                uint64_t val = rowptr[0];
                க்கு (முழு x = 1; x < width; x++) rowptr[x] = val;
            }
        }
    }

    எனில் (s0y == 0) {
        க்கு (முழு y = 1; y < height; y++)
            memcpy(dst + dsty*y, dst, (size_t)esz1*width);
    }
    திரும்பு 0;
}

நிலையான முழு castKernel(மாறிலி வெறுமை* src0_, size_t s0y, size_t s0x,
                      மாறிலி வெறுமை*, size_t, size_t,
                      மாறிலி வெறுமை*, size_t, size_t,
                      வெறுமை* dst_, size_t dsty, முழு width, முழு height,
                      மாறிலி இரட்டை* params, முழு esz1, வெறுமை* userdata)
{
    இருமம்_Func castfunc = (இருமம்_Func)userdata;
    castfunc((மாறிலி uchar*)src0_, s0y, nullptr, 0, (uchar*)dst_, dsty,
             Size((s0x > 0u ? width : 1), (s0y > 0u ? height : 1)), (வெறுமை*)params);
    திரும்பு expandKernel(s0y, s0x, dst_, dsty, width, height, esz1);
}

// ===========================================================================
// Per-op entry points (this baseline). The op-level getElemwiseFunc() dispatcher and the regular
// get*Func() forwarders live in arithm.dispatch.cpp.
// ===========================================================================

// OP_CAST / OP_CONVERT_SCALE: wrap core's convert BinaryFunc (carried in kernel.userdata) in
// castKernel, which runs it over the distinct sub-region and then expands across broadcast axes.
TKernel getCastFunc_(முழு sdepth, முழு ddepth, தருக்க scaled)
{
    இருமம்_Func cvt = scaled ? getConvertScaleFunc(sdepth, ddepth) : getConvertFunc(sdepth, ddepth);
    திரும்பு {castKernel, (வெறுமை*)cvt, CV_ELEM_SIZE1(ddepth)};
}

// Non-template wrappers around the templated kernel selectors (ADD/SUB share getAddSubFunc, MIN/MAX
// share getMinMaxFunc). min/max are T x T -> T, so R is ignored (the dispatcher validates R == T).
TKernel getAddFunc_(முழு T, முழு R) { திரும்பு getAddSubFunc<EwAdd>(T, R); }
TKernel getSubFunc_(முழு T, முழு R) { திரும்பு getAddSubFunc<EwSub>(T, R); }
TKernel getMinFunc_(முழு T, முழு R) { (வெறுமை)R; திரும்பு getMinMaxFunc<EwMin>(T); }
TKernel getMaxFunc_(முழு T, முழு R) { (வெறுமை)R; திரும்பு getMinMaxFunc<EwMax>(T); }

TKernel getHypotFunc_(முழு T, முழு R)
{
    எனில் (R != T)
        திரும்பு {};
    KernelFunc fptr = nullptr;
    தேர்வு (T)
    {
    நிலை CV_16F:  fptr = vec_இருமம்_Kernel<hfloat, hfloat, v_float32, தசம, EwHypot>; நிறுத்து;
    நிலை CV_16BF: fptr = vec_இருமம்_Kernel<bfloat, bfloat, v_float32, தசம, EwHypot>; நிறுத்து;
    நிலை CV_32F:  fptr = vec_இருமம்_Kernel<தசம,  தசம,  v_float32, தசம, EwHypot>; நிறுத்து;
#எனில் CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    நிலை CV_64F:  fptr = vec_இருமம்_Kernel<இரட்டை, இரட்டை, v_float64, இரட்டை, EwHypot>; நிறுத்து;
#இல்லையெனில்
    நிலை CV_64F:  fptr = scalar_இருமம்_Kernel<இரட்டை, இரட்டை, இரட்டை, EwHypot>; நிறுத்து;
#endif
    இயல்பு: ;
    }
    திரும்பு {fptr, nullptr, 0};
}

TKernel getAtan2Func_(முழு T, முழு R)
{
    எனில் (R != T)
        திரும்பு {};
    KernelFunc fptr = nullptr;
    தேர்வு (T)
    {
    நிலை CV_16F:  fptr = vec_இருமம்_Kernel<hfloat, hfloat, v_float32, தசம, EwAtan2>; நிறுத்து;
    நிலை CV_16BF: fptr = vec_இருமம்_Kernel<bfloat, bfloat, v_float32, தசம, EwAtan2>; நிறுத்து;
    நிலை CV_32F:  fptr = vec_இருமம்_Kernel<தசம,  தசம,  v_float32, தசம, EwAtan2>; நிறுத்து;
    நிலை CV_64F:  fptr = scalar_இருமம்_Kernel<இரட்டை, இரட்டை, இரட்டை, EwAtan2>; நிறுத்து;   // exact std::atan2
    இயல்பு: ;
    }
    திரும்பு {fptr, nullptr, 0};
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
