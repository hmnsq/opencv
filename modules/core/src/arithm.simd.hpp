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

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"
#ಅನ್ತರ್ಭಾವ "convert.hpp"          // typed load/store-as helpers (cv::), and getConvert*Func
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <cmath>
#ಅನ್ತರ್ಭಾವ <cstring>

ನಾಮಾಕಾಶ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// BinaryFunc and getConvertFunc / getConvertScaleFunc come from core (precomp.hpp / convert.hpp).

#ಆದರೆ CV_SIMD128_FP16
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_SIMD_16F
#ವ್ಯಾಖ್ಯಾನಿಸು CV_SIMD_16F 1
#ಅಥವಾಆದರೆ !defined(CV_SIMD_16F)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_SIMD_16F 0       // keep `#if CV_SIMD_16F` -Wundef-clean on builds without FP16 SIMD
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ uchar* p, v_uint8& a)
{ a = vx_setall_u8(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ schar* p, v_int8& a)
{ a = vx_setall_s8(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ uchar* p, v_int16& a)
{ a = vx_setall_s16(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ schar* p, v_int16& a)
{ a = vx_setall_s16(*p); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ushort* p, v_uint16& a)
{ a = vx_setall_u16(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* p, v_int16& a)
{ a = vx_setall_s16(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ushort* p, v_int32& a)
{ a = vx_setall_s32(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* p, v_int32& a)
{ a = vx_setall_s32(*p); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* p, v_uint32& a)
{ a = vx_setall_u32(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* p, v_int32& a)
{ a = vx_setall_s32(*p); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ uchar* p, v_float32& a)
{ a = vx_setall_f32(ದಶಮಾಂಶ(*p)); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ schar* p, v_float32& a)
{ a = vx_setall_f32(ದಶಮಾಂಶ(*p)); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ushort* p, v_float32& a)
{ a = vx_setall_f32(ದಶಮಾಂಶ(*p)); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ* p, v_float32& a)
{ a = vx_setall_f32(ದಶಮಾಂಶ(*p)); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* p, v_float32& a)
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
#ಆದರೆ defined(__ARM_NEON)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int32  v_ಕೂಡಿಸು_sat(ಸ್ಥಿರಾಂಕ v_int32& a,  ಸ್ಥಿರಾಂಕ v_int32& b)  { ಹಿಂತಿರುಗಿ v_int32(vqaddq_s32(a.val, b.val)); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int32  v_sub_sat(ಸ್ಥಿರಾಂಕ v_int32& a,  ಸ್ಥಿರಾಂಕ v_int32& b)  { ಹಿಂತಿರುಗಿ v_int32(vqsubq_s32(a.val, b.val)); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint32 v_ಕೂಡಿಸು_sat(ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b) { ಹಿಂತಿರುಗಿ v_uint32(vqaddq_u32(a.val, b.val)); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint32 v_sub_sat(ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b) { ಹಿಂತಿರುಗಿ v_uint32(vqsubq_u32(a.val, b.val)); }
#ವ್ಯಾಖ್ಯಾನಿಸು EW_HAVE_MULSAT32 1
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint8 v_mul_sat(ಸ್ಥಿರಾಂಕ v_uint8& a, ಸ್ಥಿರಾಂಕ v_uint8& b)
{
    uint16x8_t p0 = vmull_u8(vget_ಕೆಳಗಿನ_u8(a.val), vget_ಕೆಳಗಿನ_u8(b.val));
    uint16x8_t p1 = vmull_u8(vget_ಮೇಲಿನ_u8(a.val), vget_ಮೇಲಿನ_u8(b.val));
    ಹಿಂತಿರುಗಿ v_uint8(vcombine_u8(vqmovn_u16(p0), vqmovn_u16(p1)));
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int8 v_mul_sat(ಸ್ಥಿರಾಂಕ v_int8& a, ಸ್ಥಿರಾಂಕ v_int8& b)
{
    int16x8_t p0 = vmull_s8(vget_ಕೆಳಗಿನ_s8(a.val), vget_ಕೆಳಗಿನ_s8(b.val));
    int16x8_t p1 = vmull_s8(vget_ಮೇಲಿನ_s8(a.val), vget_ಮೇಲಿನ_s8(b.val));
    ಹಿಂತಿರುಗಿ v_int8(vcombine_s8(vqmovn_s16(p0), vqmovn_s16(p1)));
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint16 v_mul_sat(ಸ್ಥಿರಾಂಕ v_uint16& a, ಸ್ಥಿರಾಂಕ v_uint16& b)
{
    uint32x4_t p0 = vmull_u16(vget_ಕೆಳಗಿನ_u16(a.val), vget_ಕೆಳಗಿನ_u16(b.val));
    uint32x4_t p1 = vmull_u16(vget_ಮೇಲಿನ_u16(a.val), vget_ಮೇಲಿನ_u16(b.val));
    ಹಿಂತಿರುಗಿ v_uint16(vcombine_u16(vqmovn_u32(p0), vqmovn_u32(p1)));
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int16 v_mul_sat(ಸ್ಥಿರಾಂಕ v_int16& a, ಸ್ಥಿರಾಂಕ v_int16& b)
{
    int32x4_t p0 = vmull_s16(vget_ಕೆಳಗಿನ_s16(a.val), vget_ಕೆಳಗಿನ_s16(b.val));
    int32x4_t p1 = vmull_s16(vget_ಮೇಲಿನ_s16(a.val), vget_ಮೇಲಿನ_s16(b.val));
    ಹಿಂತಿರುಗಿ v_int16(vcombine_s16(vqmovn_s32(p0), vqmovn_s32(p1)));
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint32 v_mul_sat(ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b)
{
    uint64x2_t p0 = vmull_u32(vget_ಕೆಳಗಿನ_u32(a.val), vget_ಕೆಳಗಿನ_u32(b.val));
    uint64x2_t p1 = vmull_u32(vget_ಮೇಲಿನ_u32(a.val), vget_ಮೇಲಿನ_u32(b.val));
    ಹಿಂತಿರುಗಿ v_uint32(vcombine_u32(vqmovn_u64(p0), vqmovn_u64(p1)));
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int32 v_mul_sat(ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{
    int64x2_t p0 = vmull_s32(vget_ಕೆಳಗಿನ_s32(a.val), vget_ಕೆಳಗಿನ_s32(b.val));
    int64x2_t p1 = vmull_s32(vget_ಮೇಲಿನ_s32(a.val), vget_ಮೇಲಿನ_s32(b.val));
    ಹಿಂತಿರುಗಿ v_int32(vcombine_s32(vqmovn_s64(p0), vqmovn_s64(p1)));
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int32 v_ಕೂಡಿಸು_sat(ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{
    v_int32 res = v_ಕೂಡಿಸು(a, b);
    v_int32 ov  = v_and(v_xor(a, res), v_xor(b, res));               // sign bit set iff overflow
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ));       // a >= 0 ? INT_MAX : INT_MIN
    ಹಿಂತಿರುಗಿ v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int32 v_sub_sat(ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{
    v_int32 res = v_sub(a, b);
    v_int32 ov  = v_and(v_xor(a, b), v_xor(a, res));
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ));
    ಹಿಂತಿರುಗಿ v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint32 v_ಕೂಡಿಸು_sat(ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b)
{
    v_uint32 res = v_ಕೂಡಿಸು(a, b);
    ಹಿಂತಿರುಗಿ v_or(res, v_lt(res, a));                                  // wrapped => all-ones
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint32 v_sub_sat(ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b)
{
    ಹಿಂತಿರುಗಿ v_and(v_sub(a, b), v_ge(a, b));                           // borrow => zero
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint8 v_mul_sat(ಸ್ಥಿರಾಂಕ v_uint8& a, ಸ್ಥಿರಾಂಕ v_uint8& b)
{ v_uint16 p0, p1; v_mul_expand(a, b, p0, p1); ಹಿಂತಿರುಗಿ v_pack(p0, p1); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int8 v_mul_sat(ಸ್ಥಿರಾಂಕ v_int8& a, ಸ್ಥಿರಾಂಕ v_int8& b)
{ v_int16 p0, p1; v_mul_expand(a, b, p0, p1); ಹಿಂತಿರುಗಿ v_pack(p0, p1); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint16 v_mul_sat(ಸ್ಥಿರಾಂಕ v_uint16& a, ಸ್ಥಿರಾಂಕ v_uint16& b)
{ v_uint32 p0, p1; v_mul_expand(a, b, p0, p1); ಹಿಂತಿರುಗಿ v_pack(p0, p1); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_int16 v_mul_sat(ಸ್ಥಿರಾಂಕ v_int16& a, ಸ್ಥಿರಾಂಕ v_int16& b)
{ v_int32 p0, p1; v_mul_expand(a, b, p0, p1); ಹಿಂತಿರುಗಿ v_pack(p0, p1); }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F   // scalable (RVV) has v_float64 without CV_SIMD_64F
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*   p, v_float64& a) { a = vx_setall_f64(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*      p, v_float64& a) { a = vx_setall_f64((ದ್ವಿಗುಣ)*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* p, v_float64& a) { a = vx_setall_f64((ದ್ವಿಗುಣ)*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ int64_t*  p, v_float64& a) { a = vx_setall_f64((ದ್ವಿಗುಣ)*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ uint64_t* p, v_float64& a) { a = vx_setall_f64((ದ್ವಿಗುಣ)*p); }
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ bfloat* p, v_float32& a)
{ a = vx_setall_f32(ದಶಮಾಂಶ(*p)); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ hfloat* p, v_float32& a)
{ a = vx_setall_f32(ದಶಮಾಂಶ(*p)); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uchar* p, v_uint8& a, v_uint8& b)
{
    a = vx_ತುಂಬು(p);
    b = vx_ತುಂಬು(p + VTraits<v_uint8>::vlanes());
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ schar* p, v_int8& a, v_int8& b)
{
    a = vx_ತುಂಬು(p);
    b = vx_ತುಂಬು(p + VTraits<v_int8>::vlanes());
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ uchar* p, v_uint32& a, v_uint32& b)
{
    v_uint16 w = vx_ತುಂಬು_expand(p);
    v_expand(w, a, b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(uchar* p, ಸ್ಥಿರಾಂಕ v_uint8& a, ಸ್ಥಿರಾಂಕ v_uint8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_uint8>::vlanes(), b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(schar* p, ಸ್ಥಿರಾಂಕ v_int8& a, ಸ್ಥಿರಾಂಕ v_int8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_int8>::vlanes(), b);
}


ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ಪೂರ್ಣಾಂಕ* p, ಸ್ಥಿರಾಂಕ v_int16& a, ಸ್ಥಿರಾಂಕ v_int16& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes32 = VTraits<v_int32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v0);
    v_store(p + nlanes32, v1);
    v_store(p + nlanes32*2, v2);
    v_store(p + nlanes32*3, v3);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ದಶಮಾಂಶ* p, ಸ್ಥಿರಾಂಕ v_int16& a, ಸ್ಥಿರಾಂಕ v_int16& b)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nlanes32 = VTraits<v_float32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v_cvt_f32(v0));
    v_store(p + nlanes32, v_cvt_f32(v1));
    v_store(p + nlanes32*2, v_cvt_f32(v2));
    v_store(p + nlanes32*3, v_cvt_f32(v3));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(ದಶಮಾಂಶ* p, ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b)
{
    v_store(p, v_cvt_f32(a));
    v_store(p + VTraits<v_float32>::vlanes(), v_cvt_f32(b));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(hfloat* p, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(bfloat* p, ಸ್ಥಿರಾಂಕ v_float32& a, ಸ್ಥಿರಾಂಕ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

// Reinterpret a comparison result (whose lane type matches the operands) as the UNSIGNED integer
// vector of the same width: u8->u8, s8->u8, u16/s16->u16, u32/s32/f32->u32. The destination type
// drives the overload, so the compare kernel needs no per-width if constexpr.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_uint8&  s, v_uint8&  d) { d = s; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_int8&   s, v_uint8&  d) { d = v_reinterpret_as_u8(s); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_uint16& s, v_uint16& d) { d = s; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_int16&  s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_uint32& s, v_uint32& d) { d = s; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_int32&  s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_float32& s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
#ಆದರೆ CV_SIMD_16F
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_float16& s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_reinterpret_as(ಸ್ಥಿರಾಂಕ v_float64& s, v_uint64& d) { d = v_reinterpret_as_u64(s); }
#ಆದರೆ_ಅಂತ್ಯ

// Broadcast the compare mask value (1 or 255) across the unsigned work vector.
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_setall_ಮುಖವಾಡ(v_uint8&  v, uchar t) { v = vx_setall_u8(t); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_setall_ಮುಖವಾಡ(v_uint16& v, uchar t) { v = vx_setall_u16(t); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_setall_ಮುಖವಾಡ(v_uint32& v, uchar t) { v = vx_setall_u32(t); }

// NOTE: v_store_pair_as(uchar*, v_uint16/v_uint32, ...) - the narrowing pack-to-u8 stores we need -
// already come from core's convert.hpp (included above), so they are not redefined here.

#ಆದರೆ CV_SIMD_16F
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_setall_as(ಸ್ಥಿರಾಂಕ hfloat* p, v_float16& a)
{ a = vx_setall_f16(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ vx_ತುಂಬು_pair_as(ಸ್ಥಿರಾಂಕ hfloat* p, v_float16& a, v_float16& b)
{
    a = vx_ತುಂಬು(p);
    b = vx_ತುಂಬು(p + VTraits<v_float16>::vlanes());
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_store_pair_as(hfloat* p, ಸ್ಥಿರಾಂಕ v_float16& a, ಸ್ಥಿರಾಂಕ v_float16& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_float16>::vlanes(), b);
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ನಾಮಾಕಾಶ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      arithm.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel get_ಕೂಡಿಸು_Func_2(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);
TKernel getSubFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);
TKernel getMulFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);
TKernel getDivFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R, ತಾರ್ಕಿಕ checked);
TKernel getMinFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);
TKernel getMaxFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);
TKernel getAbsdiffFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);
TKernel getHypotFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);         // hypot = sqrt(x^2+y^2), float depths, T x T -> T
TKernel getAtan2Func_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);         // atan2(y, x), radians (-pi, pi], float depths
TKernel getCmpFunc_(ತುದಿ op, ಪೂರ್ಣಾಂಕ T);
TKernel getBitwiseFunc_(ತುದಿ op, ಪೂರ್ಣಾಂಕ esz);                    // OP_AND / OP_OR / OP_XOR, by element size
TKernel getNotFunc_(ಪೂರ್ಣಾಂಕ esz);                                // OP_NOT, by element size
TKernel get_ಕೂಡಿಸು_Weighted_Func_2(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);                   // OP_ADDW, a*alpha+b*beta+gamma (T x T -> R)
TKernel getSelectFunc_(ಪೂರ್ಣಾಂಕ mdepth, ಪೂರ್ಣಾಂಕ T);   // OP_SELECT: 1-byte mask, a/b/dst of T (by esz)
TKernel getClampFunc_(ಪೂರ್ಣಾಂಕ T);                // OP_CLAMP: min(max(x, lo), hi), all operands of T
TKernel getCastFunc_(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth, ತಾರ್ಕಿಕ scaled);   // OP_CAST / OP_CONVERT_SCALE

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors (vector + scalar). New binary ops slot in here.
// ===========================================================================
ರಚನೆ Ew_ಕೂಡಿಸು {
    // useScalar: does the op consume the scale scalar (params[0]) in vec()/preproc()? When false,
    // vecBinaryKernel's fast 2-arg branch is taken unconditionally (the check folds at compile time);
    // when true, it is taken only if the runtime scale is exactly 1.
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_ಕೂಡಿಸು(a, b); }
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: v_add wraps - use the saturating version to match the scalar int64+clamp tail
    ಸ್ಥಿರ v_int32  vec(ಸ್ಥಿರಾಂಕ v_int32& a,  ಸ್ಥಿರಾಂಕ v_int32& b)  { ಹಿಂತಿರುಗಿ v_ಕೂಡಿಸು_sat(a, b); }
    ಸ್ಥಿರ v_uint32 vec(ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b) { ಹಿಂತಿರುಗಿ v_ಕೂಡಿಸು_sat(a, b); }
#ಆದರೆ_ಅಂತ್ಯ
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    // Accumulate in the promoted type, NOT in W: for the native saturating path W is the narrow
    // lane type (schar/short/...), and (W)(a+b) would wrap in 8/16 bits before saturate_cast<Tr>
    // could clamp. Letting a+b promote (narrow -> int) keeps saturation for 8/16-bit outputs and
    // the natural wrap for 32/64-bit (both matching cv::add). The SIMD path already saturates.
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ W(a + b); }
};

ರಚನೆ EwSub {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_sub(a, b); }
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: see EwAdd::vec
    ಸ್ಥಿರ v_int32  vec(ಸ್ಥಿರಾಂಕ v_int32& a,  ಸ್ಥಿರಾಂಕ v_int32& b)  { ಹಿಂತಿರುಗಿ v_sub_sat(a, b); }
    ಸ್ಥಿರ v_uint32 vec(ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b) { ಹಿಂತಿರುಗಿ v_sub_sat(a, b); }
#ಆದರೆ_ಅಂತ್ಯ
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ W(a - b); }   // see EwAdd::scl
    // 64-bit unsigned has no wider WT to hold a-b, so the generic path would wrap on underflow.
    // Saturate at 0 to match cv::subtract (8/16/32-bit already saturate via SIMD floor / wide WT).
    ಸ್ಥಿರ uint64_t scl(uint64_t a, uint64_t b, uint64_t) { ಹಿಂತಿರುಗಿ uint64_t(a >= b)*(a - b); }
};

// mul/div compute in a wide FLOAT work type (W = float for <=16-bit/f16/bf/f32, double for
// 32/64-bit/f64), matching cv::multiply/divide; the executor casts the work-type result down to
// rdepth. scalar (reference) path only for now - SIMD can follow. (No vec(): never instantiated.)
// vec(a, b, scale): the 3rd arg is the scale vector. COMMUTATIVE ops (mul, absdiff) ignore it - their
// scale is folded into the (cheaper) preproc of one operand. DIVISION is NOT commutative, so its scale
// MUST stick to the numerator; it takes the scale in vec (numerator*scale/denominator) and leaves
// preproc as identity. vecBinaryKernel always passes vscalar to vec, so every branch (incl. a broadcast
// denominator) divides correctly. The 2-arg vec (scale==1) is the both-contiguous fast path.
ರಚನೆ EwMul {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ನಿಜ;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_mul(a, b); }
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    // integer lanes: full product clamped to the lane type (v_mul_sat) == cv::multiply semantics
    // at scale 1. These drive the scale==1 fast path on FULL-width registers (Wvec1 = the native
    // lane vector): whole-register loads/stores, widening happens inside the multiply itself.
    // The guard matches the v_mul_sat definitions above (like EwAdd's 32-bit overloads): in
    // no-SIMD builds the vector typedefs still exist (intrin_cpp), but the helpers do not.
    ಸ್ಥಿರ v_uint8  vec(ಸ್ಥಿರಾಂಕ v_uint8& a,  ಸ್ಥಿರಾಂಕ v_uint8& b)  { ಹಿಂತಿರುಗಿ v_mul_sat(a, b); }
    ಸ್ಥಿರ v_int8   vec(ಸ್ಥಿರಾಂಕ v_int8& a,   ಸ್ಥಿರಾಂಕ v_int8& b)   { ಹಿಂತಿರುಗಿ v_mul_sat(a, b); }
    ಸ್ಥಿರ v_uint16 vec(ಸ್ಥಿರಾಂಕ v_uint16& a, ಸ್ಥಿರಾಂಕ v_uint16& b) { ಹಿಂತಿರುಗಿ v_mul_sat(a, b); }
    ಸ್ಥಿರ v_int16  vec(ಸ್ಥಿರಾಂಕ v_int16& a,  ಸ್ಥಿರಾಂಕ v_int16& b)  { ಹಿಂತಿರುಗಿ v_mul_sat(a, b); }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ EW_HAVE_MULSAT32
    ಸ್ಥಿರ v_uint32 vec(ಸ್ಥಿರಾಂಕ v_uint32& a, ಸ್ಥಿರಾಂಕ v_uint32& b) { ಹಿಂತಿರುಗಿ v_mul_sat(a, b); }
    ಸ್ಥಿರ v_int32  vec(ಸ್ಥಿರಾಂಕ v_int32& a,  ಸ್ಥಿರಾಂಕ v_int32& b)  { ಹಿಂತಿರುಗಿ v_mul_sat(a, b); }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& s) { ಹಿಂತಿರುಗಿ v_mul(a, s); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST s) { ಹಿಂತಿರುಗಿ a * b * s; }
};
// div has two variants by the COMMON INPUT type (matching cv::'s per-type kernel choice): integer
// inputs guard divide-by-zero -> 0 (cv:: iscalar_div); float inputs do NOT guard (cv:: fscalar_div,
// a/0 -> inf), which then saturates on the cast to an integer output exactly like cv::divide.
ರಚನೆ EwDivInt {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ನಿಜ;
    // integer inputs computed in the float work type: guard b==0 -> 0. Scale rides the numerator (vec).
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) {
        ಸ್ಥಿರಾಂಕ V z = v_setzero_<V>();
        ಹಿಂತಿರುಗಿ v_select(v_eq(b, z), z, v_div(a, b));
    }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ V& s) {
        ಸ್ಥಿರಾಂಕ V z = v_setzero_<V>();
        ಹಿಂತಿರುಗಿ v_select(v_eq(b, z), z, v_div(v_mul(a, s), b));
    }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }   // identity: scale is in vec
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST s) { ಹಿಂತಿರುಗಿ b != W(0) ? a * s / b : W(0); }
};
ರಚನೆ EwDivFlt {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ನಿಜ;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_div(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ V& s) { ಹಿಂತಿರುಗಿ v_div(v_mul(a, s), b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }   // identity: scale is in vec
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST s) { ಹಿಂತಿರುಗಿ a * s / b; }
};

// atan2(y, x) in RADIANS over the standard C range (-pi, pi] - the fastAtan2 minimax polynomial
// (mathfuncs_core.simd.hpp v_atan_f32) with the 180/pi factor dropped and the C quadrant logic
// (fastAtan2 returns degrees in [0, 360)). Absolute accuracy ~1e-5 rad, same as cv::fastAtan2.
// Generic over the universal-intrinsic float vector type.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V>
ಸ್ಥಿರ ಸಾಲೊಳಗೆ V v_atan2(ಸ್ಥಿರಾಂಕ V& y, ಸ್ಥಿರಾಂಕ V& x)
{
    ಬಳಸಿ LT = ಬಗೆ_ಹೆಸರು VTraits<V>::lane_type;
    ಸ್ಥಿರಾಂಕ V eps  = v_setall_<V>((LT)DBL_EPSILON);
    ಸ್ಥಿರಾಂಕ V z    = v_setzero_<V>();
    ಸ್ಥಿರಾಂಕ V p7   = v_setall_<V>((LT)-0.04432655554792128);
    ಸ್ಥಿರಾಂಕ V p5   = v_setall_<V>((LT)0.1555786518463281);
    ಸ್ಥಿರಾಂಕ V p3   = v_setall_<V>((LT)-0.3258083974640975);
    ಸ್ಥಿರಾಂಕ V p1   = v_setall_<V>((LT)0.9997878412794807);
    ಸ್ಥಿರಾಂಕ V vpi2 = v_setall_<V>((LT)(CV_PI/2));
    ಸ್ಥಿರಾಂಕ V vpi  = v_setall_<V>((LT)CV_PI);

    V ax = v_abs(x), ay = v_abs(y);
    V c  = v_div(v_min(ax, ay), v_ಕೂಡಿಸು(v_max(ax, ay), eps));
    V c2 = v_mul(c, c);
    V a  = v_mul(v_fma(v_fma(v_fma(p7, c2, p5), c2, p3), c2, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(vpi2, a));
    a = v_select(v_lt(x, z), v_sub(vpi, a), a);
    a = v_select(v_lt(y, z), v_sub(z, a), a);
    ಹಿಂತಿರುಗಿ a;
}

ರಚನೆ EwAtan2 {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_atan2(a, b); }   // a = y, b = x
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ std::atan2(a, b); }
};

// hypot(x, y) = sqrt(x^2 + y^2): NAIVE (matches cv::magnitude; overflow at |x| ~ 1e19+ for f32
// inputs is accepted), computed in the float work type; T x T -> T over the float depths.
ರಚನೆ EwHypot {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_sqrt(v_fma(a, a, v_mul(b, b))); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ std::ವರ್ಗಮೂಲ(a*a + b*b); }
};

// min / max / absdiff: T x T -> T (same depth in and out, no scale). v_min/v_max exist for every
// vector lane type (64-bit ints fall back to scalar). absdiff uses v_absdiff (defined for the
// UNSIGNED and float lane types - signed/wide depths go through the scalar path), and the scalar
// |a-b| is computed branch-wise so it never underflows an unsigned work type.
ರಚನೆ EwMin {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_min(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ std::min(a, b); }
};
ರಚನೆ EwMax {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_max(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ std::max(a, b); }
};
ರಚನೆ EwAbsdiff {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    // The absdiff RESULT is the UNSIGNED type of the input width: v_absdiff(v_int8/16/32) already returns
    // v_uint8/16/32 (the true |a-b|, which can exceed the signed max), and v_absdiff on unsigned/float
    // returns the same type. vec() therefore returns THAT type (deduced), not the input V, so the kernel
    // stores it through the matching UNSIGNED v_store_pair_as (an honest same-type store) - no reinterpret,
    // no touching the saturating-narrow overloads' semantics. (This is why the kernel keeps the vec
    // result in its own variable rather than reusing the input operand.)
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ ಸ್ವಯಂಚಾಲಿತ vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_absdiff(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ ಸ್ವಯಂಚಾಲಿತ vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ v_absdiff(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ a > b ? W(a - b) : W(b - a); }
};

// Signed T -> SAME signed T (rdepth==T): |a-b| saturated into the signed range, in ONE pass. cv::absdiff
// keeps the signed depth, so the generic path computes absdiff->unsigned then casts back down (2 insns);
// this fuses it. v_absdiff yields the unsigned |a-b|; v_min clamps to the signed max (already >=0), then
// a same-width reinterpret to signed (all values now fit).
ರಚನೆ EwAbsdiffS {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಸ್ಥಿರ v_int8  vec(ಸ್ಥಿರಾಂಕ v_int8&  a, ಸ್ಥಿರಾಂಕ v_int8&  b) { ಹಿಂತಿರುಗಿ v_reinterpret_as_s8 (v_min(v_absdiff(a, b), vx_setall_u8 (0x7f))); }
    ಸ್ಥಿರ v_int16 vec(ಸ್ಥಿರಾಂಕ v_int16& a, ಸ್ಥಿರಾಂಕ v_int16& b) { ಹಿಂತಿರುಗಿ v_reinterpret_as_s16(v_min(v_absdiff(a, b), vx_setall_u16(0x7fff))); }
    ಸ್ಥಿರ v_int32 vec(ಸ್ಥಿರಾಂಕ v_int32& a, ಸ್ಥಿರಾಂಕ v_int32& b) { ಹಿಂತಿರುಗಿ v_reinterpret_as_s32(v_min(v_absdiff(a, b), vx_setall_u32(0x7fffffff))); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ a > b ? W(a - b) : W(b - a); }
    // 64-bit signed has no wider work type to widen into, and |INT64_MIN - b| does not fit in
    // int64_t, so the difference is taken in uint64 - exact for every pair - and then clamped.
    // The narrower widths reach this overload only through their int64 work type, where |a - b|
    // is at most 2^32 and the clamp is a no-op.
    ಸ್ಥಿರ int64_t scl(int64_t a, int64_t b, int64_t)
    {
        ಸ್ಥಿರಾಂಕ uint64_t d = (a > b) ? (uint64_t)a - (uint64_t)b : (uint64_t)b - (uint64_t)a;
        ಹಿಂತಿರುಗಿ (int64_t)std::min<uint64_t>(d, (uint64_t)INT64_MAX);
    }
};

// compare: T x T -> u8 mask. cmp(a,b) is the scalar relation over a work type W (integers compare
// directly, f16/bf16 through float); vec(a,b) is the vector relation returning an all-ones/zero mask
// in the operand's own lane type. The kernel turns either into 0 / trueVal (1 or 255 via TKernel::flags).
ರಚನೆ EwCmpEq { ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ ತಾರ್ಕಿಕ cmp(W a, W b) { ಹಿಂತಿರುಗಿ a == b; }
                 ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_eq(a, b); } };
ರಚನೆ EwCmpNe { ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ ತಾರ್ಕಿಕ cmp(W a, W b) { ಹಿಂತಿರುಗಿ a != b; }
                 ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_ne(a, b); } };
// LT/LE are synthesized from GT/GE via the EW_KERNEL_SWAP01 flag (a<b == b>a), so no Lt/Le kernels.
ರಚನೆ EwCmpGt { ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ ತಾರ್ಕಿಕ cmp(W a, W b) { ಹಿಂತಿರುಗಿ a > b; }
                 ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_gt(a, b); } };
ರಚನೆ EwCmpGe { ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ ತಾರ್ಕಿಕ cmp(W a, W b) { ಹಿಂತಿರುಗಿ a >= b; }
                 ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_ge(a, b); } };

// bitwise AND/OR/XOR: bit-pattern op, type-agnostic. Run on the UNSIGNED integer whose width matches
// the element (u8/u16/u32/u64), so one functor set covers every depth. No scale, no widening (T x T ->
// T, exactly like min/max); preproc is the identity (min/max share this shape). 64-bit uses the scalar
// path (no widening vector helpers), the rest ride vecBinaryKernel's native same-type path.
ರಚನೆ EwAnd {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_and(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ W(a & b); }
};
ರಚನೆ EwOr {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_or(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ W(a | b); }
};
ರಚನೆ EwXor {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ useScalar = ಸುಳ್ಳು;
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b) { ಹಿಂತಿರುಗಿ v_xor(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V, ಬಗೆ_ಹೆಸರು S> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V& b, ಸ್ಥಿರಾಂಕ S&) { ಹಿಂತಿರುಗಿ vec(a, b); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V preproc(ಸ್ಥಿರಾಂಕ V& a, ಸ್ಥಿರಾಂಕ V&) { ಹಿಂತಿರುಗಿ a; }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W, ಬಗೆ_ಹೆಸರು ST> ಸ್ಥಿರ W scl(W a, W b, ST) { ಹಿಂತಿರುಗಿ W(a ^ b); }
};

// Collapse a gap-free 2D tile to 1D (call with the per-operand x/y-steps).
#ವ್ಯಾಖ್ಯಾನಿಸು EW_TRY_COLLAPSE(NSRC) \
    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && \
        s0y == s0x*(size_t)ಅಗಲ && (NSRC < 2 || s1y == s1x*(size_t)ಅಗಲ)) \
    { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Tr, ಬಗೆ_ಹೆಸರು WT, ವರ್ಗ Op, ಬಗೆ_ಹೆಸರು ST=WT>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scalar_ದ್ವಿಮಾನ_Kernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                              ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                              ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t, ಶೂನ್ಯ* dst_, size_t dsty,
                              ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* params, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    ಆದರೆ (ಅಗಲ <= 0 || ಎತ್ತರ <= 0) ಹಿಂತಿರುಗಿ 0;
    s0y /= ಗಾತ್ರ(T);
    s1y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ಅಗಲ == 1u);

    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < ಎತ್ತರ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ಆದರೆ (s0x == s1x) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], (WT)src1[x], scalar));
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s0x == 0) {
            WT sc0 = (WT)src0[0];
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++)
                dst[x] = saturate_cast<Tr>(Op::scl(sc0, (WT)src1[x], scalar));
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            WT sc1 = (WT)src1[0];
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], sc1, scalar));
        }
    }
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT>
ಸ್ಥಿರ ಶೂನ್ಯ expandScalar(ಸ್ಥಿರಾಂಕ T* sc, size_t sx, ಪೂರ್ಣಾಂಕ n0, WT* scbuf, ಪೂರ್ಣಾಂಕ n)
{
    ಆದರೆ (n0 <= 0 || n <= 0) ಹಿಂತಿರುಗಿ;
    ಪೂರ್ಣಾಂಕ i = 0;
    ಗಾಗಿ (; i < n0; i++) scbuf[i] = (WT)sc[i*sx];
    ಗಾಗಿ (; i < n; i++) scbuf[i] = scbuf[i - n0];
}

// Decode the per-channel patch bytes (mask + value) from the kernel flags into small arrays. Uniform
// (no EW_CMP_PATCH): mask = trueVal, value = 0 (an ordinary compare). Per-channel: from the 4-bit
// fields. The compare kernels apply  result = (rawmask & mask) | value  per channel - folding the
// former separate patch pass into the compare (one pass).
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cmpUnpackPatch(ಪೂರ್ಣಾಂಕ flags, uchar trueVal, uchar mvals[4], uchar vvals[4])
{
    ಆದರೆ (flags & EW_CMP_PATCH)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < 4; c++)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ f = (flags >> (EW_CMP_PATCH_ಸರಿಸು + c*4)) & 0xF;
            mvals[c] = (uchar)cmpPatchByte(f & 3);
            vvals[c] = (uchar)cmpPatchByte((f >> 2) & 3);
        }
    ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < 4; c++) { mvals[c] = trueVal; vvals[c] = 0; }
}

// Scalar comparison kernel: T x T -> u8 mask. Result is 0 (false) or `trueVal` (true), trueVal read
// from TKernel::flags - 255 (default, matching cv::compare) or 1 (a numpy-style 0/1 mask). Integers
// compare directly (WT == T); f16/bf16 compare through a float WT. This is the fallback for the depths
// the SIMD kernel doesn't cover (f16/bf16/64-bit). stepx in {0,1}; dst is u8, byte step == element step.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT, ವರ್ಗ Cmp>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scalarCompareKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                         ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                         ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                         ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                         ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ flags, ಶೂನ್ಯ*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    ಆದರೆ (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= ಗಾತ್ರ(T);
    s1y /= ಗಾತ್ರ(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ಅಗಲ == 1u);

    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)src1_;
    uchar* dst = (uchar*)dst_;
    ಸ್ಥಿರಾಂಕ uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ perch = (flags & EW_CMP_PATCH) != 0;          // per-channel fix-up (width=cn<=4)
    CV_Assert(!perch || ಅಗಲ <= 4);                         // a per-channel patch is a short-row tile
    #ವ್ಯಾಖ್ಯಾನಿಸು EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                         : (uchar)((cond) ? trueVal : 0))

    EW_TRY_COLLAPSE(2);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < ಎತ್ತರ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ಆದರೆ (s0x == s1x) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], (WT)src1[x]), x);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s0x == 0) {
            WT a = (WT)src0[0];
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, (WT)src1[x]), x);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            WT b = (WT)src1[0];
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], b), x);
        }
    }
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು EW_CMP_APPLY
    ಹಿಂತಿರುಗಿ 0;
}

// SIMD comparison kernel: T x T -> u8 mask, for the directly-comparable depths (u8/s8/u16/s16/u32/
// s32/f32). vec(a,b) gives an all-ones/zero mask in the operand's lane type; v_reinterpret_as turns it
// into the unsigned int of the same width; cmpFuse packs to u8 and applies (rawmask & M) | V. The
// per-row SIMD body runs when an operand is contiguous or broadcast; a per-channel patch (M/V differ
// by channel) only ever arrives as a short-row (width=cn) tile and is handled there.
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Vvec, ಬಗೆ_ಹೆಸರು Uvec, ವರ್ಗ Cmp>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ vecCompareKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                            ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                            ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                            ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                            ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ flags, ಶೂನ್ಯ*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    ಆದರೆ (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= ಗಾತ್ರ(T);
    s1y /= ಗಾತ್ರ(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ಅಗಲ == 1u);

    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)src1_;
    uchar* dst = (uchar*)dst_;
    ಸ್ಥಿರಾಂಕ uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];                                 // per-channel fix-up: (rawmask & m) | v
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ perch = (flags & EW_CMP_PATCH) != 0;
    CV_Assert(!perch || ಅಗಲ <= 4);                          // a per-channel patch is a short-row tile

    EW_TRY_COLLAPSE(2);
    ಪೂರ್ಣಾಂಕ y = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    // Short rows (2/3/4 elements): a per-channel scalar over a multi-channel image arrives as a
    // (width=cn) x (height=pixels) tile with the scalar broadcast over rows (s?y==0). The per-row SIMD
    // below never triggers at such a tiny width, so expand the width<=4 broadcast operand (threshold
    // AND the interleaved per-channel mask/value bytes) across the lanes and compare many rows at once.
    // This is also the ONLY path a per-channel patch (M/V differ by channel) reaches.
    // constexpr: the short-row loads T natively (vx_load->Vvec), so it exists ONLY at native width. The
    // widened f16/bf16->f32 path (sizeof(T) < Vvec lane) and 8-byte f64 fall through to per-row + scalar.
    // (sizeof(T) <= 4 is part of the constexpr gate: for f64 the body would compile to nothing but
    // dead stores - gcc 9 flags scbuf/bsrc/bsx as set-but-not-used there.)
    ಆದರೆ constexpr (ಗಾತ್ರ(T) == ಗಾತ್ರ(ಬಗೆ_ಹೆಸರು VTraits<Vvec>::lane_type) && ಗಾತ್ರ(T) <= 4)
    ಆದರೆ (ಎತ್ತರ > 1 && ಅಗಲ <= 4 &&
        ((s0y == 0 && s1y == (size_t)ಅಗಲ*s1x) || (s1y == 0 && s0y == (size_t)ಅಗಲ*s0x)) &&
        dsty == (size_t)ಅಗಲ)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ  = VTraits<Vvec>::vlanes();          // sizeof(T)-type lanes
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ8 = VTraits<v_uint8>::vlanes();       // u8 output lanes
        constexpr ಪೂರ್ಣಾಂಕ MAXV8 = VTraits<v_uint8>::max_nlanes;
        T scbuf[MAXV8 * 3] = {};                             // interleaved threshold (elements)
        uchar mbuf[MAXV8 * 3] = {}, vbuf[MAXV8 * 3] = {};    // interleaved mask / value (bytes)
        // ^ the {} inits are for -Wmaybe-uninitialized only: expandScalar/decode fill every lane that
        //   is later read, but the compiler cannot prove it with a runtime VECSZ
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ bc0 = (s0y == 0);                         // src0 is the broadcast (short) operand
        ಸ್ಥಿರಾಂಕ T* bsrc = bc0 ? src0 : src1; ಸ್ಥಿರಾಂಕ size_t bsx = bc0 ? s0x : s1x;
        ಸ್ಥಿರಾಂಕ T*& src = bc0 ? src1 : src0;                   // the row-stepping operand (advanced below)
        ಸ್ಥಿರಾಂಕ size_t sy = bc0 ? s1y : s0y;
        // The compare direction (scalar-first when bc0, array-first otherwise) is hoisted OUT of the row
        // loop with an explicit if(bc0) - the loop is written twice so no per-iteration branch remains.
        // Per branch: own unroll, threshold + interleaved M/V loaded ONCE. sizeof==1 keeps 12 vectors.
        #ವ್ಯಾಖ್ಯಾನಿಸು EW_CMPR(r, a, b) v_reinterpret_as(Cmp::vec(a, b), r)   // rawmask -> sizeof(T)-byte uint
        ಆದರೆ constexpr (ಗಾತ್ರ(T) == 1)                        // 3 u8 masks -> 3 u8 stores
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ewidth = VECSZ8 * 3;
            expandScalar(bsrc, bsx, ಅಗಲ, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, ಅಗಲ, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, ಅಗಲ, vbuf, ewidth);
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dy = ewidth / ಅಗಲ;
            Vvec  sc0=vx_ತುಂಬು(scbuf),  sc1=vx_ತುಂಬು(scbuf+VECSZ),   sc2=vx_ತುಂಬು(scbuf+VECSZ*2);
            v_uint8 M0=vx_ತುಂಬು(mbuf),  M1=vx_ತುಂಬು(mbuf+VECSZ8),    M2=vx_ತುಂಬು(mbuf+VECSZ8*2);
            v_uint8 V0=vx_ತುಂಬು(vbuf),  V1=vx_ತುಂಬು(vbuf+VECSZ8),    V2=vx_ತುಂಬು(vbuf+VECSZ8*2);
            #ವ್ಯಾಖ್ಯಾನಿಸು EW_ROW1(A0,B0,A1,B1,A2,B2) \
                ಗಾಗಿ (; y + dy <= ಎತ್ತರ; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 o0=v_or(v_and(r0,M0),V0),o1=v_or(v_and(r1,M1),V1),o2=v_or(v_and(r2,M2),V2); \
                    v_store(dst, o0); v_store(dst+VECSZ8, o1); v_store(dst+VECSZ8*2, o2); }
            ಆದರೆ (bc0) EW_ROW1(sc0,vx_ತುಂಬು(src), sc1,vx_ತುಂಬು(src+VECSZ), sc2,vx_ತುಂಬು(src+VECSZ*2))
            ಇಲ್ಲದಿದ್ದರೆ     EW_ROW1(vx_ತುಂಬು(src),sc0, vx_ತುಂಬು(src+VECSZ),sc1, vx_ತುಂಬು(src+VECSZ*2),sc2)
            #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು EW_ROW1
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ constexpr (ಗಾತ್ರ(T) == 2)                   // 3 u16 masks -> 1 full + 1 half u8 store
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ewidth = VECSZ * 3;
            expandScalar(bsrc, bsx, ಅಗಲ, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, ಅಗಲ, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, ಅಗಲ, vbuf, ewidth);
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dy = ewidth / ಅಗಲ;
            Vvec  sc0=vx_ತುಂಬು(scbuf), sc1=vx_ತುಂಬು(scbuf+VECSZ), sc2=vx_ತುಂಬು(scbuf+VECSZ*2);
            v_uint8 M0=vx_ತುಂಬು(mbuf), M1=vx_ತುಂಬು(mbuf+VECSZ8);
            v_uint8 V0=vx_ತುಂಬು(vbuf), V1=vx_ತುಂಬು(vbuf+VECSZ8);
            #ವ್ಯಾಖ್ಯಾನಿಸು EW_ROW2(A0,B0,A1,B1,A2,B2) \
                ಗಾಗಿ (; y + dy <= ಎತ್ತರ; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 b0=v_pack(r0,r1), b1=v_pack(r2,r2); \
                    v_uint8 o0=v_or(v_and(b0,M0),V0), o1=v_or(v_and(b1,M1),V1); \
                    v_store(dst, o0); v_store_ಕೆಳಗಿನ(dst+VECSZ8, o1); }
            ಆದರೆ (bc0) EW_ROW2(sc0,vx_ತುಂಬು(src), sc1,vx_ತುಂಬು(src+VECSZ), sc2,vx_ತುಂಬು(src+VECSZ*2))
            ಇಲ್ಲದಿದ್ದರೆ     EW_ROW2(vx_ತುಂಬು(src),sc0, vx_ತುಂಬು(src+VECSZ),sc1, vx_ತುಂಬು(src+VECSZ*2),sc2)
            #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು EW_ROW2
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ constexpr (ಗಾತ್ರ(T) == 4)                   // sizeof==4: 6 u32 -> 3 u16 -> 1 full + 1 half u8
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ewidth = VECSZ * 6;
            expandScalar(bsrc, bsx, ಅಗಲ, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, ಅಗಲ, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, ಅಗಲ, vbuf, ewidth);
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dy = ewidth / ಅಗಲ;
            Vvec sc0=vx_ತುಂಬು(scbuf),sc1=vx_ತುಂಬು(scbuf+VECSZ),sc2=vx_ತುಂಬು(scbuf+VECSZ*2),
                 sc3=vx_ತುಂಬು(scbuf+VECSZ*3),sc4=vx_ತುಂಬು(scbuf+VECSZ*4),sc5=vx_ತುಂಬು(scbuf+VECSZ*5);
            v_uint8 M0=vx_ತುಂಬು(mbuf), M1=vx_ತುಂಬು(mbuf+VECSZ8);
            v_uint8 V0=vx_ತುಂಬು(vbuf), V1=vx_ತುಂಬು(vbuf+VECSZ8);
            #ವ್ಯಾಖ್ಯಾನಿಸು EW_ROW4(A0,B0,A1,B1,A2,B2,A3,B3,A4,B4,A5,B5) \
                ಗಾಗಿ (; y + dy <= ಎತ್ತರ; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2,r3,r4,r5; \
                    EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    EW_CMPR(r3,A3,B3); EW_CMPR(r4,A4,B4); EW_CMPR(r5,A5,B5); \
                    v_uint16 p0=v_pack(r0,r1), p1=v_pack(r2,r3), p2=v_pack(r4,r5); \
                    v_uint8 q0=v_pack(p0,p1), q1=v_pack(p2,p2); \
                    v_uint8 o0=v_or(v_and(q0,M0),V0), o1=v_or(v_and(q1,M1),V1); \
                    v_store(dst, o0); v_store_ಕೆಳಗಿನ(dst+VECSZ8, o1); }
            ಆದರೆ (bc0) EW_ROW4(sc0,vx_ತುಂಬು(src),sc1,vx_ತುಂಬು(src+VECSZ),sc2,vx_ತುಂಬು(src+VECSZ*2),
                             sc3,vx_ತುಂಬು(src+VECSZ*3),sc4,vx_ತುಂಬು(src+VECSZ*4),sc5,vx_ತುಂಬು(src+VECSZ*5))
            ಇಲ್ಲದಿದ್ದರೆ     EW_ROW4(vx_ತುಂಬು(src),sc0,vx_ತುಂಬು(src+VECSZ),sc1,vx_ತುಂಬು(src+VECSZ*2),sc2,
                             vx_ತುಂಬು(src+VECSZ*3),sc3,vx_ತುಂಬು(src+VECSZ*4),sc4,vx_ತುಂಬು(src+VECSZ*5),sc5)
            #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು EW_ROW4
        }
        #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು EW_CMPR
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ (; y < ಎತ್ತರ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ಪೂರ್ಣಾಂಕ x = 0;
    #ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        // SIMD for BOTH the contiguous case AND a broadcast operand (a per-channel scalar const has
        // step 0 - without this the scalar-vs-array compare, incl. every multi-channel scalar compare,
        // fell to the scalar tail below, ~5x slower). vx_setall broadcasts the step-0 operand once.
        // 4-vector unroll + halide right-edge backoff (reprocess the last 4*VECSZ when width is not a
        // multiple); dst is a separate mask buffer so the overlap is a harmless idempotent rewrite.
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Vvec>::vlanes();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ8 = VTraits<v_uint8>::vlanes();
        v_uint8 vT; v_setall_ಮುಖವಾಡ(vT, trueVal);              // uniform mask (per-channel patch is short-row)
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ಬಾಲ_Trick = ಅಗಲ >= 4*VECSZ && src0_ != dst_ && src1_ != dst_;
        // Narrow the 4 masks to u8 FIRST, then apply trueVal on the u8 result (1 and per u8 vector, not
        // per wide lane-group). One block per MASK lane width (== sizeof(T) natively, but 4 for the
        // f16/bf16->f32 widened path); a 4-vector unroll spans exactly 4*VECSZ elements.
        constexpr size_t MW = ಗಾತ್ರ(ಬಗೆ_ಹೆಸರು VTraits<Uvec>::lane_type);
        #ವ್ಯಾಖ್ಯಾನಿಸು PACK_STORE_CMP_ಫಲಿತಾಂಶ(m0,m1,m2,m3, D) ಮಾಡು { \
            ಆದರೆ constexpr (MW == 1u) { \
                v_uint8 o0=v_and(m0,vT),o1=v_and(m1,vT),o2=v_and(m2,vT),o3=v_and(m3,vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); v_store((D)+VECSZ8*2,o2); v_store((D)+VECSZ8*3,o3); \
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ constexpr (MW == 2u) { \
                v_uint8 o0=v_and(v_pack(m0,m1),vT), o1=v_and(v_pack(m2,m3),vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); \
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ constexpr (MW == 4u) { \
                v_uint8 o0=v_and(v_pack(v_pack(m0,m1),v_pack(m2,m3)),vT); \
                v_store((D),o0); \
            } ಇಲ್ಲದಿದ್ದರೆ { /* sizeof==8: 4 u64 -> 2 u32 -> 1 u16 -> u8 (low half), 4*VECSZ elems -> store_low */ \
                v_uint16 g=v_pack(v_pack(m0,m1),v_pack(m2,m3)); \
                v_uint8 o0=v_and(v_pack(g,g),vT); \
                v_store_ಕೆಳಗಿನ((D),o0); \
            } } ಆಗ(0)
        ಆದರೆ (s0x == 1u && s1x == 1u)
        {
            ಗಾಗಿ (; x < ಅಗಲ; x += 4*VECSZ)
            {
                ಆದರೆ (x + 4*VECSZ > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_Trick) ನಿಲ್ಲಿಸು; x = ಅಗಲ - 4*VECSZ; }
                Vvec a0, a1, a2, a3, b0, b1, b2, b3;   // vx_load_pair_as widens f16/bf16->f32, else native
                vx_ತುಂಬು_pair_as(src0+x, a0, a1); vx_ತುಂಬು_pair_as(src0+x+2*VECSZ, a2, a3);
                vx_ತುಂಬು_pair_as(src1+x, b0, b1); vx_ತುಂಬು_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b1), m1);
                v_reinterpret_as(Cmp::vec(a2, b2), m2); v_reinterpret_as(Cmp::vec(a3, b3), m3);
                PACK_STORE_CMP_ಫಲಿತಾಂಶ(m0, m1, m2, m3, dst+x);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s1x == 0u)                       // src1 (e.g. the scalar) broadcast
        {
            Vvec b0; vx_setall_as(src1, b0);
            ಗಾಗಿ (; x < ಅಗಲ; x += 4*VECSZ)
            {
                ಆದರೆ (x + 4*VECSZ > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_Trick) ನಿಲ್ಲಿಸು; x = ಅಗಲ - 4*VECSZ; }
                Vvec a0, a1, a2, a3;
                vx_ತುಂಬು_pair_as(src0+x, a0, a1); vx_ತುಂಬು_pair_as(src0+x+2*VECSZ, a2, a3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b0), m1);
                v_reinterpret_as(Cmp::vec(a2, b0), m2); v_reinterpret_as(Cmp::vec(a3, b0), m3);
                PACK_STORE_CMP_ಫಲಿತಾಂಶ(m0, m1, m2, m3, dst+x);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s0x == 0u)                       // src0 broadcast
        {
            Vvec a0; vx_setall_as(src0, a0);
            ಗಾಗಿ (; x < ಅಗಲ; x += 4*VECSZ)
            {
                ಆದರೆ (x + 4*VECSZ > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_Trick) ನಿಲ್ಲಿಸು; x = ಅಗಲ - 4*VECSZ; }
                Vvec b0, b1, b2, b3;
                vx_ತುಂಬು_pair_as(src1+x, b0, b1); vx_ತುಂಬು_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a0, b1), m1);
                v_reinterpret_as(Cmp::vec(a0, b2), m2); v_reinterpret_as(Cmp::vec(a0, b3), m3);
                PACK_STORE_CMP_ಫಲಿತಾಂಶ(m0, m1, m2, m3, dst+x);
            }
        }
        #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು PACK_STORE_CMP_ಫಲಿತಾಂಶ
    #ಆದರೆ_ಅಂತ್ಯ
        // Fold the fix-up: raw all-ones/zero mask -> (raw & M) | V. Uniform => raw & trueVal (V=0); a
        // per-channel patch (width=cn<=4 here) indexes M/V by the channel x.
        #ವ್ಯಾಖ್ಯಾನಿಸು EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                             : (uchar)((cond) ? trueVal : 0))
        ಆದರೆ (s0x == s1x) {
            ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], src1[x]), x);
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s0x == 0) {
            T a = src0[0];
            ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, src1[x]), x);
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            T b = src1[0];
            ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], b), x);
        }
        #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು EW_CMP_APPLY
    }
    ಹಿಂತಿರುಗಿ 0;
}

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Tr, ಬಗೆ_ಹೆಸರು Wvec, ಬಗೆ_ಹೆಸರು WT, ವರ್ಗ Op, ಬಗೆ_ಹೆಸರು ST=WT, ಬಗೆ_ಹೆಸರು Wvec1=Wvec>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ vec_ದ್ವಿಮಾನ_Kernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                         ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                         ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                         ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                         ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* params, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    ಆದರೆ (ಅಗಲ <= 0 || ಎತ್ತರ <= 0) ಹಿಂತಿರುಗಿ 0;
    s0y /= ಗಾತ್ರ(T);
    s1y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(Tr);

    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ಅಗಲ == 1u);

    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    ಪೂರ್ಣಾಂಕ y = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಬಳಸಿ Wlane = ಬಗೆ_ಹೆಸರು VTraits<Wvec>::lane_type;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Wvec>::vlanes();
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ use_ಬಾಲ_trick = ಅಗಲ >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    [[maybe_unused]] Wvec vscalar;
    vx_setall_as(&scalar, vscalar);
    ಆದರೆ (ಎತ್ತರ > 1 && ಅಗಲ <= 4 &&
        ((s0y == 0 && s1y == ಅಗಲ*s1x) ||
         (s1y == 0 && s0y == ಅಗಲ*s0x)) &&
        dsty == (size_t)ಅಗಲ) {
        constexpr ಪೂರ್ಣಾಂಕ MAXVECSZ = VTraits<Wvec>::max_nlanes;
        Wlane scbuf[MAXVECSZ*6] = {};   // {} for -Wmaybe-uninitialized only (filled up to VECSZ*6)
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ewidth = VECSZ*6;
        expandScalar(s0y == 0 ? src0 : src1, s0y == 0 ? s0x : s1x, ಅಗಲ, scbuf, ewidth);
        ಪೂರ್ಣಾಂಕ dy = ewidth / ಅಗಲ;
        Wvec sc0, sc1, sc2, sc3, sc4, sc5;
        sc0 = Op::preproc(vx_ತುಂಬು(scbuf), vscalar);
        sc1 = Op::preproc(vx_ತುಂಬು(scbuf + VECSZ), vscalar);
        sc2 = Op::preproc(vx_ತುಂಬು(scbuf + VECSZ*2), vscalar);
        sc3 = Op::preproc(vx_ತುಂಬು(scbuf + VECSZ*3), vscalar);
        sc4 = Op::preproc(vx_ತುಂಬು(scbuf + VECSZ*4), vscalar);
        sc5 = Op::preproc(vx_ತುಂಬು(scbuf + VECSZ*5), vscalar);

        ಆದರೆ (s0y == 0) {
            ಗಾಗಿ (; y + dy <= ಎತ್ತರ; y += dy, src1 += s1y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_ತುಂಬು_pair_as(src1, v0, v1);
                vx_ತುಂಬು_pair_as(src1 + VECSZ*2, v2, v3);
                vx_ತುಂಬು_pair_as(src1 + VECSZ*4, v4, v5);
                ಸ್ವಯಂಚಾಲಿತ w0 = Op::vec(sc0, v0, vscalar), w1 = Op::vec(sc1, v1, vscalar),
                     w2 = Op::vec(sc2, v2, vscalar), w3 = Op::vec(sc3, v3, vscalar),
                     w4 = Op::vec(sc4, v4, vscalar), w5 = Op::vec(sc5, v5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಗಾಗಿ (; y + dy <= ಎತ್ತರ; y += dy, src0 += s0y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_ತುಂಬು_pair_as(src0, v0, v1);
                vx_ತುಂಬು_pair_as(src0 + VECSZ*2, v2, v3);
                vx_ತುಂಬು_pair_as(src0 + VECSZ*4, v4, v5);
                ಸ್ವಯಂಚಾಲಿತ w0 = Op::vec(v0, sc0, vscalar), w1 = Op::vec(v1, sc1, vscalar),
                     w2 = Op::vec(v2, sc2, vscalar), w3 = Op::vec(v3, sc3, vscalar),
                     w4 = Op::vec(v4, sc4, vscalar), w5 = Op::vec(v5, sc5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ (; y < ಎತ್ತರ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ಪೂರ್ಣಾಂಕ x = 0;
    #ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        Wvec a0, a1, a2, a3, b0, b1, b2, b3;
        ಆದರೆ (s0x == s1x) {
            ಆದರೆ (!Op::useScalar || scalar == ST(1)) {
                // this branch runs on Wvec1, which may be WIDER-laned than Wvec (e.g. the u8 mul
                // path: Wvec1=v_uint16 is 2x the lanes of Wvec=v_float32) - step by ITS lane count,
                // or the pairs overlap and the tail backoff writes past the row end
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ1 = VTraits<Wvec1>::vlanes();
                ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ಬಾಲ_trick1 = ಅಗಲ >= VECSZ1*4 && src0_ != dst_ && src1_ != dst_;
                ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ1*4) {
                    Wvec1 a0_, a1_, a2_, a3_, b0_, b1_, b2_, b3_;
                    ಆದರೆ (x + VECSZ1*4 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_trick1) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ1*4; }
                    vx_ತುಂಬು_pair_as(src0 + x, a0_, a1_);
                    vx_ತುಂಬು_pair_as(src0 + x + VECSZ1*2, a2_, a3_);
                    vx_ತುಂಬು_pair_as(src1 + x, b0_, b1_);
                    vx_ತುಂಬು_pair_as(src1 + x + VECSZ1*2, b2_, b3_);
                    ಸ್ವಯಂಚಾಲಿತ c0 = Op::vec(a0_, b0_), c1 = Op::vec(a1_, b1_),
                         c2 = Op::vec(a2_, b2_), c3 = Op::vec(a3_, b3_);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ1*2, c2, c3);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*4) {
                    ಆದರೆ (x + VECSZ*4 > ಅಗಲ) { ಆದರೆ (!use_ಬಾಲ_trick) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*4; }
                    vx_ತುಂಬು_pair_as(src0 + x, a0, a1);
                    vx_ತುಂಬು_pair_as(src0 + x + VECSZ*2, a2, a3);
                    vx_ತುಂಬು_pair_as(src1 + x, b0, b1);
                    vx_ತುಂಬು_pair_as(src1 + x + VECSZ*2, b2, b3);
                    ಸ್ವಯಂಚಾಲಿತ c0 = Op::vec(Op::preproc(a0, vscalar), b0, vscalar),
                         c1 = Op::vec(Op::preproc(a1, vscalar), b1, vscalar),
                         c2 = Op::vec(Op::preproc(a2, vscalar), b2, vscalar),
                         c3 = Op::vec(Op::preproc(a3, vscalar), b3, vscalar);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ*2, c2, c3);
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s1x == 0) {
            vx_setall_as(src1, b0);
            b0 = Op::preproc(b0, vscalar);
            ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*4) {
                ಆದರೆ (x + VECSZ*4 > ಅಗಲ) { ಆದರೆ (!use_ಬಾಲ_trick) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*4; }
                vx_ತುಂಬು_pair_as(src0 + x, a0, a1);
                vx_ತುಂಬು_pair_as(src0 + x + VECSZ*2, a2, a3);
                ಸ್ವಯಂಚಾಲಿತ c0 = Op::vec(a0, b0, vscalar), c1 = Op::vec(a1, b0, vscalar),
                     c2 = Op::vec(a2, b0, vscalar), c3 = Op::vec(a3, b0, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            vx_setall_as(src0, b0);
            b0 = Op::preproc(b0, vscalar);
            ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*4) {
                ಆದರೆ (x + VECSZ*4 > ಅಗಲ) { ಆದರೆ (!use_ಬಾಲ_trick) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*4; }
                vx_ತುಂಬು_pair_as(src1 + x, a0, a1);
                vx_ತುಂಬು_pair_as(src1 + x + VECSZ*2, a2, a3);
                ಸ್ವಯಂಚಾಲಿತ c0 = Op::vec(b0, a0, vscalar), c1 = Op::vec(b0, a1, vscalar),
                     c2 = Op::vec(b0, a2, vscalar), c3 = Op::vec(b0, a3, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
    #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; x < ಅಗಲ; x++)
            dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x*s0x], (WT)src1[x*s1x], scalar));
    }
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    vx_cleanup();
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ 0;
}

// ===========================================================================
// OP_ADD
// ===========================================================================

// add dispatch: T x T -> R (operands already same depth T).
//  - native saturating path (Wvec = native, use_simd=true) for T -> T on <=32-bit ints + f32
//    (32-bit lanes via the local v_add_sat/v_sub_sat - v_add/v_sub wrap there);
//  - widening f32-hub (Wvec = v_float32, use_simd=true) for the widened/half/float outputs;
//  - scalar (use_simd=false) for 64-bit results and f64 (no vector path yet).
ಮಾದರಿ<ವರ್ಗ Op>
TKernel get_ಕೂಡಿಸು_Sub_Func(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R)
{
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_8U:
        fptr = R == CV_8U ? vec_ದ್ವಿಮಾನ_Kernel<uchar, uchar, v_uint8, ಗಿಡ್ಡ, Op, uchar> :
               R == CV_16S ? vec_ದ್ವಿಮಾನ_Kernel<uchar, ಗಿಡ್ಡ, v_int16, ಗಿಡ್ಡ, Op, ಗಿಡ್ಡ> :
               R == CV_32S ? vec_ದ್ವಿಮಾನ_Kernel<uchar, ಪೂರ್ಣಾಂಕ, v_int16, ಗಿಡ್ಡ, Op, ಗಿಡ್ಡ> :
               R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<uchar, ದಶಮಾಂಶ, v_int16, ಗಿಡ್ಡ, Op, ಗಿಡ್ಡ> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:
        fptr =
            R == CV_8S ? vec_ದ್ವಿಮಾನ_Kernel<schar, schar, v_int8, ಗಿಡ್ಡ, Op, schar> :
            R == CV_16S ? vec_ದ್ವಿಮಾನ_Kernel<schar, ಗಿಡ್ಡ, v_int16, ಗಿಡ್ಡ, Op, ಗಿಡ್ಡ> :
            R == CV_32S ? vec_ದ್ವಿಮಾನ_Kernel<schar, ಪೂರ್ಣಾಂಕ, v_int16, ಗಿಡ್ಡ, Op, ಗಿಡ್ಡ> :
            R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<schar, ದಶಮಾಂಶ, v_int16, ಗಿಡ್ಡ, Op, ಗಿಡ್ಡ> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U:
        fptr = R == CV_16U ? vec_ದ್ವಿಮಾನ_Kernel<ushort, ushort, v_uint16, ಪೂರ್ಣಾಂಕ, Op, ushort> :
               R == CV_32S ? vec_ದ್ವಿಮಾನ_Kernel<ushort, ಪೂರ್ಣಾಂಕ, v_int32, ಪೂರ್ಣಾಂಕ, Op, ಪೂರ್ಣಾಂಕ> :
               R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<ushort, ದಶಮಾಂಶ, v_int32, ಪೂರ್ಣಾಂಕ, Op, ಪೂರ್ಣಾಂಕ> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S:
        fptr = R == CV_16S ? vec_ದ್ವಿಮಾನ_Kernel<ಗಿಡ್ಡ, ಗಿಡ್ಡ, v_int16, ಪೂರ್ಣಾಂಕ, Op, ಗಿಡ್ಡ> :
               R == CV_32S ? vec_ದ್ವಿಮಾನ_Kernel<ಗಿಡ್ಡ, ಪೂರ್ಣಾಂಕ,   v_int32, ಪೂರ್ಣಾಂಕ, Op, ಪೂರ್ಣಾಂಕ> :
               R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<ಗಿಡ್ಡ, ದಶಮಾಂಶ, v_int32, ಪೂರ್ಣಾಂಕ, Op, ಪೂರ್ಣಾಂಕ> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U:
        fptr = R == CV_32U ? vec_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, v_uint32, int64_t, Op, ಚಿಹ್ನೆರಹಿತ> :
                R == CV_64S ? scalar_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, int64_t, int64_t, Op> :
                R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, int64_t, Op> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S:
        fptr = R == CV_32S ? vec_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, v_int32, int64_t, Op, ಪೂರ್ಣಾಂಕ> :
               R == CV_64S ? scalar_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, int64_t, Op> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U:
        fptr = R == CV_64U ? scalar_ದ್ವಿಮಾನ_Kernel<uint64_t, uint64_t, uint64_t, Op> :
               R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<uint64_t, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, Op> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S:
        fptr = R == CV_64S ? scalar_ದ್ವಿಮಾನ_Kernel<int64_t, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<int64_t, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, Op> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16F:
        fptr =
        #ಆದರೆ CV_SIMD_16F
        R == CV_16F ? vec_ದ್ವಿಮಾನ_Kernel<hfloat, hfloat, v_float16, ದಶಮಾಂಶ, Op, hfloat> :
        #ಇಲ್ಲದಿದ್ದರೆ
        R == CV_16F ? vec_ದ್ವಿಮಾನ_Kernel<hfloat, hfloat, v_float32, ದಶಮಾಂಶ, Op, ದಶಮಾಂಶ> :
        #ಆದರೆ_ಅಂತ್ಯ
        R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<hfloat, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ, Op, ದಶಮಾಂಶ> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF:
        fptr =
            R == CV_16BF ? vec_ದ್ವಿಮಾನ_Kernel<bfloat, bfloat, v_float32, ದಶಮಾಂಶ, Op, ದಶಮಾಂಶ> :
            R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<bfloat, ದಶಮಾಂಶ,  v_float32, ದಶಮಾಂಶ, Op, ದಶಮಾಂಶ> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:
        fptr = R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<ದಶಮಾಂಶ, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ, Op, ದಶಮಾಂಶ> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64F:
        #ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, v_float64, ದ್ವಿಗುಣ, Op> : ಶೂನ್ಯಸೂಚಕ;
        #ಇಲ್ಲದಿದ್ದರೆ
        fptr = R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, Op> : ಶೂನ್ಯಸೂಚಕ;
        #ಆದರೆ_ಅಂತ್ಯ
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}

TKernel getMulFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R)
{
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_8U:
        fptr =   // scale==1 fast path: whole u8 registers, v_mul_sat widens+clamps inside; Wvec=f32 for scale
            R == CV_8U ? vec_ದ್ವಿಮಾನ_Kernel<uchar, uchar, v_float32, ದಶಮಾಂಶ, EwMul, ದಶಮಾಂಶ, v_uint8> :
            R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<uchar, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:
        fptr =   // scale==1 fast path: whole s8 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_8S ? vec_ದ್ವಿಮಾನ_Kernel<schar, schar, v_float32, ದಶಮಾಂಶ, EwMul, ದಶಮಾಂಶ, v_int8> :
            R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<schar, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U:
        fptr =   // scale==1 fast path: whole u16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16U ? vec_ದ್ವಿಮಾನ_Kernel<ushort, ushort, v_float32, ದಶಮಾಂಶ, EwMul, ದಶಮಾಂಶ, v_uint16> :
            R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<ushort, ದಶಮಾಂಶ,  v_float32, ದಶಮಾಂಶ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S:
        fptr =   // scale==1 fast path: whole s16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16S ? vec_ದ್ವಿಮಾನ_Kernel<ಗಿಡ್ಡ, ಗಿಡ್ಡ, v_float32, ದಶಮಾಂಶ, EwMul, ದಶಮಾಂಶ, v_int16> :
            R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<ಗಿಡ್ಡ, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16F:
        fptr =
            R == CV_16F ? vec_ದ್ವಿಮಾನ_Kernel<hfloat, hfloat, v_float32, ದಶಮಾಂಶ, EwMul> :
            R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<hfloat, ದಶಮಾಂಶ,  v_float32, ದಶಮಾಂಶ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF:
        fptr =
            R == CV_16BF ? vec_ದ್ವಿಮಾನ_Kernel<bfloat, bfloat, v_float32, ದಶಮಾಂಶ, EwMul> :
            R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<bfloat, ದಶಮಾಂಶ,  v_float32, ದಶಮಾಂಶ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:
        fptr = R == CV_32F ? vec_ದ್ವಿಮಾನ_Kernel<ದಶಮಾಂಶ, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U:
        fptr =   // scale==1 fast path (where v_mul_sat32 exists): whole u32 registers, widening
                 // multiply + saturating narrow; the f64 work vector serves the scale path
        #ಆದರೆ defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32U ? vec_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, v_float64, ದ್ವಿಗುಣ, EwMul, ದ್ವಿಗುಣ, v_uint32> :
            R == CV_64F ? vec_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ,   v_float64, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        #ಅಥವಾಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32U ? vec_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, v_float64, ದ್ವಿಗುಣ, EwMul> :
            R == CV_64F ? vec_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ,   v_float64, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        #ಇಲ್ಲದಿದ್ದರೆ
            R == CV_32U ? scalar_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ, EwMul> :
            R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ,   ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        #ಆದರೆ_ಅಂತ್ಯ
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S:
        fptr =   // scale==1 fast path: see CV_32U
        #ಆದರೆ defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32S ? vec_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ,    v_float64, ದ್ವಿಗುಣ, EwMul, ದ್ವಿಗುಣ, v_int32> :
            R == CV_64F ? vec_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, v_float64, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        #ಅಥವಾಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32S ? vec_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ,    v_float64, ದ್ವಿಗುಣ, EwMul> :
            R == CV_64F ? vec_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, v_float64, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        #ಇಲ್ಲದಿದ್ದರೆ
            R == CV_32S ? scalar_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ,    ದ್ವಿಗುಣ, EwMul> :
            R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        #ಆದರೆ_ಅಂತ್ಯ
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U:
        fptr = R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<uint64_t, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S:
        fptr = R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<int64_t,  ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64F:
        #ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, v_float64, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        #ಇಲ್ಲದಿದ್ದರೆ
        fptr = R == CV_64F ? scalar_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ,   ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, EwMul> : ಶೂನ್ಯಸೂಚಕ;
        #ಆದರೆ_ಅಂತ್ಯ
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}

// `checked` (decided by the CALLER from the ORIGINAL input types) selects the divide-by-zero
// policy, INDEPENDENT of the work type R: EwDivInt guards /0 -> 0 for integer-semantics division,
// EwDivFlt does not (a/0 -> inf, saturating on a later cast, as cv::divide does for float inputs).
// It must apply on EVERY row - two wide integers (e.g. 32U / 64S) promote to a 64F work type yet
// still need the integer guard, so the float-work rows can't hardcode EwDivFlt.
TKernel getDivFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R, ತಾರ್ಕಿಕ checked)
{
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    // SIMD via vecBinaryKernel: div scales the numerator in vec (a*scale/b), EwDivFlt (float a/0 -> inf,
    // saturates on cast) / EwDivInt (v_select guards b==0 -> 0 post-facto). Prefer a DIRECT T->T kernel
    // (1 pass, saturate on store) - emitBinary probes result==T first; a T->f32/f64 kernel serves an
    // explicit float dtype. Work type: f32 for <=16-bit/f16/bf16/f32; f64 for 32/64-bit.
    #ವ್ಯಾಖ್ಯಾನಿಸು DIV(T_, Tr_, Wv_, W_) (checked ? vec_ದ್ವಿಮಾನ_Kernel<T_, Tr_, Wv_, W_, EwDivInt> \
                                           : vec_ದ್ವಿಮಾನ_Kernel<T_, Tr_, Wv_, W_, EwDivFlt>)
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #ವ್ಯಾಖ್ಯಾನಿಸು DIVW(T_, Tr_) DIV(T_, Tr_, v_float64, ದ್ವಿಗುಣ)                 // 32/64-bit, f64 SIMD
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು DIVW(T_, Tr_) (checked ? scalar_ದ್ವಿಮಾನ_Kernel<T_, Tr_, ದ್ವಿಗುಣ, EwDivInt> \
                                    : scalar_ದ್ವಿಮಾನ_Kernel<T_, Tr_, ದ್ವಿಗುಣ, EwDivFlt>)
#ಆದರೆ_ಅಂತ್ಯ
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_8U:   fptr = R==CV_8U  ? DIV(uchar,  uchar,  v_float32, ದಶಮಾಂಶ) : R==CV_32F ? DIV(uchar,  ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:   fptr = R==CV_8S  ? DIV(schar,  schar,  v_float32, ದಶಮಾಂಶ) : R==CV_32F ? DIV(schar,  ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U:  fptr = R==CV_16U ? DIV(ushort, ushort, v_float32, ದಶಮಾಂಶ) : R==CV_32F ? DIV(ushort, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S:  fptr = R==CV_16S ? DIV(ಗಿಡ್ಡ,  ಗಿಡ್ಡ,  v_float32, ದಶಮಾಂಶ) : R==CV_32F ? DIV(ಗಿಡ್ಡ,  ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16F:  fptr = R==CV_16F ? DIV(hfloat, hfloat, v_float32, ದಶಮಾಂಶ) : R==CV_32F ? DIV(hfloat, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF: fptr = R==CV_16BF? DIV(bfloat, bfloat, v_float32, ದಶಮಾಂಶ) : R==CV_32F ? DIV(bfloat, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:  fptr = R==CV_32F ? DIV(ದಶಮಾಂಶ,  ದಶಮಾಂಶ,  v_float32, ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U:  fptr = R==CV_32U ? DIVW(ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ) : R==CV_64F ? DIVW(ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S:  fptr = R==CV_32S ? DIVW(ಪೂರ್ಣಾಂಕ,      ಪೂರ್ಣಾಂಕ)      : R==CV_64F ? DIVW(ಪೂರ್ಣಾಂಕ,      ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U:  fptr = R==CV_64F ? DIVW(uint64_t, ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S:  fptr = R==CV_64F ? DIVW(int64_t,  ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64F:  fptr = R==CV_64F ? DIVW(ದ್ವಿಗುಣ,   ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:      ;
    }
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DIV
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು DIVW
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}


// min / max: T x T -> T for every depth. Native v_min/v_max on the matching lane type (8/16/32-bit
// ints, f16/bf16/f32); 64-bit ints use the scalar path. Op = EwMin or EwMax.
ಮಾದರಿ<ವರ್ಗ Op>
ಸ್ಥಿರ TKernel getMinMaxFunc(ಪೂರ್ಣಾಂಕ T)
{
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_8U:   fptr = vec_ದ್ವಿಮಾನ_Kernel<uchar,    uchar,    v_uint8,   ಗಿಡ್ಡ,   Op, uchar>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:   fptr = vec_ದ್ವಿಮಾನ_Kernel<schar,    schar,    v_int8,    ಗಿಡ್ಡ,   Op, schar>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ushort,   ushort,   v_uint16,  ಪೂರ್ಣಾಂಕ,     Op, ushort>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ಗಿಡ್ಡ,    ಗಿಡ್ಡ,    v_int16,   ಪೂರ್ಣಾಂಕ,     Op, ಗಿಡ್ಡ>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, v_uint32,  int64_t, Op, ಚಿಹ್ನೆರಹಿತ>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ,      ಪೂರ್ಣಾಂಕ,      v_int32,   int64_t, Op, ಪೂರ್ಣಾಂಕ>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16F:
        #ಆದರೆ CV_SIMD_16F
        fptr = vec_ದ್ವಿಮಾನ_Kernel<hfloat,   hfloat,   v_float16, ದಶಮಾಂಶ,   Op, hfloat>;
        #ಇಲ್ಲದಿದ್ದರೆ
        fptr = vec_ದ್ವಿಮಾನ_Kernel<hfloat,   hfloat,   v_float32, ದಶಮಾಂಶ,   Op, ದಶಮಾಂಶ>;
        #ಆದರೆ_ಅಂತ್ಯ
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF: fptr = vec_ದ್ವಿಮಾನ_Kernel<bfloat, bfloat, v_float32, ದಶಮಾಂಶ, Op, ದಶಮಾಂಶ>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ದಶಮಾಂಶ, ದಶಮಾಂಶ, v_float32, ದಶಮಾಂಶ, Op, ದಶಮಾಂಶ>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U:  fptr = scalar_ದ್ವಿಮಾನ_Kernel<uint64_t, uint64_t, uint64_t, Op>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S:  fptr = scalar_ದ್ವಿಮಾನ_Kernel<int64_t,  int64_t,  int64_t,  Op>; ನಿಲ್ಲಿಸು;
    #ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿತಿ CV_64F:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, v_float64, ದ್ವಿಗುಣ, Op, ದ್ವಿಗುಣ>; ನಿಲ್ಲಿಸು;
    #ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_64F:  fptr = scalar_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ,   ದ್ವಿಗುಣ,   ದ್ವಿಗುಣ,   Op>; ನಿಲ್ಲಿಸು;
    #ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ವನಿಯೋಜಿತ:      ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}

// absdiff: |a-b|, T x T -> R.
TKernel getAbsdiffFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R)
{
    // Signed T has TWO outputs: R==T -> EwAbsdiffS (saturating |a-b| kept in the signed range, native work,
    // ONE pass - the depth cv::absdiff keeps); R==(the unsigned type of the same width) -> EwAbsdiff (the
    // true |a-b| via v_absdiff, for an explicit unsigned dst). Unsigned/float T only produce R==T. Each case
    // returns nullptr for a wrong R (no separate rdepth guard).
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_8U:   fptr = R == CV_8U  ? vec_ದ್ವಿಮಾನ_Kernel<uchar,  uchar,  v_uint8,  ಗಿಡ್ಡ,   EwAbsdiff, uchar>  : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U:  fptr = R == CV_16U ? vec_ದ್ವಿಮಾನ_Kernel<ushort, ushort, v_uint16, ಪೂರ್ಣಾಂಕ,     EwAbsdiff, ushort> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U:  fptr = R == CV_32U ? vec_ದ್ವಿಮಾನ_Kernel<ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ, v_uint32, int64_t, EwAbsdiff, ಚಿಹ್ನೆರಹಿತ> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:   fptr = R == CV_8S  ? vec_ದ್ವಿಮಾನ_Kernel<schar, schar, v_int8,  ಗಿಡ್ಡ,   EwAbsdiffS, schar> :
                         R == CV_8U  ? vec_ದ್ವಿಮಾನ_Kernel<schar, uchar, v_int8,  ಗಿಡ್ಡ,   EwAbsdiff,  schar> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S:  fptr = R == CV_16S ? vec_ದ್ವಿಮಾನ_Kernel<ಗಿಡ್ಡ, ಗಿಡ್ಡ, v_int16, ಪೂರ್ಣಾಂಕ,     EwAbsdiffS, ಗಿಡ್ಡ> :
                         R == CV_16U ? vec_ದ್ವಿಮಾನ_Kernel<ಗಿಡ್ಡ, ushort, v_int16, ಪೂರ್ಣಾಂಕ,     EwAbsdiff,  ಗಿಡ್ಡ> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S:  fptr = R == CV_32S ? vec_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ,   ಪೂರ್ಣಾಂಕ,   v_int32, int64_t, EwAbsdiffS, ಪೂರ್ಣಾಂಕ> :
                         R == CV_32U ? vec_ದ್ವಿಮಾನ_Kernel<ಪೂರ್ಣಾಂಕ, ಚಿಹ್ನೆರಹಿತ, v_int32, int64_t, EwAbsdiff,  ಪೂರ್ಣಾಂಕ> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    #ಆದರೆ CV_SIMD_16F
    ಸ್ಥಿತಿ CV_16F:  fptr = R == CV_16F ? vec_ದ್ವಿಮಾನ_Kernel<hfloat, hfloat, v_float16, ದಶಮಾಂಶ, EwAbsdiff, hfloat> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    #ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_16F:  fptr = R == CV_16F ? vec_ದ್ವಿಮಾನ_Kernel<hfloat, hfloat, v_float32, ದಶಮಾಂಶ, EwAbsdiff, ದಶಮಾಂಶ>  : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    #ಆದರೆ_ಅಂತ್ಯ
    ಸ್ಥಿತಿ CV_16BF: fptr = R == CV_16BF ? vec_ದ್ವಿಮಾನ_Kernel<bfloat, bfloat, v_float32, ದಶಮಾಂಶ, EwAbsdiff, ದಶಮಾಂಶ> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:  fptr = R == CV_32F  ? vec_ದ್ವಿಮಾನ_Kernel<ದಶಮಾಂಶ,  ದಶಮಾಂಶ,  v_float32, ದಶಮಾಂಶ, EwAbsdiff, ದಶಮಾಂಶ> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U:  fptr = R == CV_64U  ? scalar_ದ್ವಿಮಾನ_Kernel<uint64_t, uint64_t, uint64_t, EwAbsdiff> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S:  fptr = R == CV_64S  ? scalar_ದ್ವಿಮಾನ_Kernel<int64_t,  int64_t,  int64_t,  EwAbsdiffS> :
                         R == CV_64U  ? scalar_ದ್ವಿಮಾನ_Kernel<int64_t,  uint64_t, int64_t,  EwAbsdiff> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    #ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿತಿ CV_64F:  fptr = R == CV_64F  ? vec_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, v_float64, ದ್ವಿಗುಣ, EwAbsdiff, ದ್ವಿಗುಣ> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    #ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_64F:  fptr = R == CV_64F  ? scalar_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, EwAbsdiff> : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    #ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ವನಿಯೋಜಿತ:      ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}

// compare: T x T -> u8 mask. Directly-comparable depths (u8/s8/u16/s16/u32/s32/f32) take the SIMD
// vecCompareKernel; f16/bf16 and 64-bit int depths fall back to scalarCompareKernel. The returned kernel
// defaults to a 255 mask in TKernel::flags (cv::compare-compatible); kernel.flags=1 gives a 0/1 mask.
ಮಾದರಿ<ವರ್ಗ Cmp>
ಸ್ಥಿರ KernelFunc compareByType(ಪೂರ್ಣಾಂಕ T)
{
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_8U:   ಹಿಂತಿರುಗಿ vecCompareKernel<uchar,    v_uint8,   v_uint8,  Cmp>;
    ಸ್ಥಿತಿ CV_8S:   ಹಿಂತಿರುಗಿ vecCompareKernel<schar,    v_int8,    v_uint8,  Cmp>;
    ಸ್ಥಿತಿ CV_16U:  ಹಿಂತಿರುಗಿ vecCompareKernel<ushort,   v_uint16,  v_uint16, Cmp>;
    ಸ್ಥಿತಿ CV_16S:  ಹಿಂತಿರುಗಿ vecCompareKernel<ಗಿಡ್ಡ,    v_int16,   v_uint16, Cmp>;
    ಸ್ಥಿತಿ CV_32U:  ಹಿಂತಿರುಗಿ vecCompareKernel<ಚಿಹ್ನೆರಹಿತ, v_uint32,  v_uint32, Cmp>;
    ಸ್ಥಿತಿ CV_32S:  ಹಿಂತಿರುಗಿ vecCompareKernel<ಪೂರ್ಣಾಂಕ,      v_int32,   v_uint32, Cmp>;
    ಸ್ಥಿತಿ CV_32F:  ಹಿಂತಿರುಗಿ vecCompareKernel<ದಶಮಾಂಶ,    v_float32, v_uint32, Cmp>;
    #ಆದರೆ CV_SIMD_16F
    ಸ್ಥಿತಿ CV_16F:  ಹಿಂತಿರುಗಿ vecCompareKernel<hfloat,   v_float16, v_uint16, Cmp>;
    #ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_16F:  ಹಿಂತಿರುಗಿ vecCompareKernel<hfloat,   v_float32, v_uint32, Cmp>;   // widen f16->f32
    #ಆದರೆ_ಅಂತ್ಯ
    ಸ್ಥಿತಿ CV_16BF: ಹಿಂತಿರುಗಿ vecCompareKernel<bfloat,   v_float32, v_uint32, Cmp>;   // widen bf16->f32 (no native)
    ಸ್ಥಿತಿ CV_64U:  ಹಿಂತಿರುಗಿ scalarCompareKernel<uint64_t, uint64_t, Cmp>;
    ಸ್ಥಿತಿ CV_64S:  ಹಿಂತಿರುಗಿ scalarCompareKernel<int64_t,  int64_t,  Cmp>;
    #ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿತಿ CV_64F:  ಹಿಂತಿರುಗಿ vecCompareKernel<ದ್ವಿಗುಣ,   v_float64, v_uint64, Cmp>;
    #ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_64F:  ಹಿಂತಿರುಗಿ scalarCompareKernel<ದ್ವಿಗುಣ,   ದ್ವಿಗುಣ,   Cmp>;
    #ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ವನಿಯೋಜಿತ:      ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
    }
}

TKernel getCmpFunc_(ತುದಿ op, ಪೂರ್ಣಾಂಕ T)
{
    // Only 4 physical kernels (eq/ne/gt/ge): LT/LE reuse GT/GE with the operands swapped
    // (a<b == b>a, a<=b == b>=a) via the EW_KERNEL_SWAP01 flag, honored by the executor.
    KernelFunc f = ಶೂನ್ಯಸೂಚಕ;
    ಪೂರ್ಣಾಂಕ flags = 0;              // mask value: 0 flag bits => 0/255 (cv::compare); EW_KERNEL_MASK1 => 0/1
    ಆಯ್ಕೆ (op)
    {
    ಸ್ಥಿತಿ OP_CMP_EQ: f = compareByType<EwCmpEq>(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_CMP_NE: f = compareByType<EwCmpNe>(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_CMP_GT: f = compareByType<EwCmpGt>(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_CMP_GE: f = compareByType<EwCmpGe>(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_CMP_LT: f = compareByType<EwCmpGt>(T); flags = EW_KERNEL_SWAP01; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_CMP_LE: f = compareByType<EwCmpGe>(T); flags = EW_KERNEL_SWAP01; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:        ;
    }
    ಹಿಂತಿರುಗಿ {f, ಶೂನ್ಯಸೂಚಕ, flags};
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
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Vvec>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ notKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                     ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                     ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    ಆದರೆ (ಅಗಲ <= 0 || ಎತ್ತರ <= 0) ಹಿಂತಿರುಗಿ 0;
    s0y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(T);
    CV_Assert(s0x == 1u || ಅಗಲ == 1);
    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    T* dst = (T*)dst_;
    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && s0y == (size_t)ಅಗಲ) { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < ಎತ್ತರ; y++, src0 += s0y, dst += dsty)
    {
        ಪೂರ್ಣಾಂಕ x = 0;
    #ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಆದರೆ constexpr (ಗಾತ್ರ(T) <= 4)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Vvec>::vlanes();
            ಗಾಗಿ (; x <= ಅಗಲ - VECSZ; x += VECSZ)
                v_store(dst + x, v_not(vx_ತುಂಬು(src0 + x)));
        }
    #ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = (T)~src0[x];
    }
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ<ವರ್ಗ Op>
ಸ್ಥಿರ KernelFunc bitwiseByEsz(ಪೂರ್ಣಾಂಕ esz)
{
    ಆಯ್ಕೆ (esz)
    {
    ಸ್ಥಿತಿ 1: ಹಿಂತಿರುಗಿ vec_ದ್ವಿಮಾನ_Kernel<uint8_t,  uint8_t,  v_uint8,  uint8_t,  Op, uint8_t>;
    ಸ್ಥಿತಿ 2: ಹಿಂತಿರುಗಿ vec_ದ್ವಿಮಾನ_Kernel<uint16_t, uint16_t, v_uint16, uint16_t, Op, uint16_t>;
    ಸ್ಥಿತಿ 4: ಹಿಂತಿರುಗಿ vec_ದ್ವಿಮಾನ_Kernel<uint32_t, uint32_t, v_uint32, uint32_t, Op, uint32_t>;
    ಸ್ಥಿತಿ 8: ಹಿಂತಿರುಗಿ scalar_ದ್ವಿಮಾನ_Kernel<uint64_t, uint64_t, uint64_t, Op>;
    ಪೂರ್ವನಿಯೋಜಿತ: ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
    }
}

TKernel getBitwiseFunc_(ತುದಿ op, ಪೂರ್ಣಾಂಕ esz)
{
    KernelFunc f = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (op)
    {
    ಸ್ಥಿತಿ OP_AND: f = bitwiseByEsz<EwAnd>(esz); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_OR:  f = bitwiseByEsz<EwOr >(esz); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_XOR: f = bitwiseByEsz<EwXor>(esz); ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:     ;
    }
    ಹಿಂತಿರುಗಿ {f, ಶೂನ್ಯಸೂಚಕ, 0};
}

TKernel getNotFunc_(ಪೂರ್ಣಾಂಕ esz)
{
    KernelFunc f = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (esz)
    {
    ಸ್ಥಿತಿ 1: f = notKernel<uint8_t,  v_uint8 >; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 2: f = notKernel<uint16_t, v_uint16>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 4: f = notKernel<uint32_t, v_uint32>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 8: f = notKernel<uint64_t, v_uint32>; ನಿಲ್ಲಿಸು;   // SIMD path compiled out for 8-byte -> scalar ~
    ಪೂರ್ವನಿಯೋಜಿತ: ;
    }
    ಹಿಂತಿರುಗಿ {f, ಶೂನ್ಯಸೂಚಕ, 0};
}

// ===========================================================================
// OP_ADDW (addWeighted): dst = a*alpha + b*beta + gamma, params[0..2] = {alpha, beta, gamma}. Two fused
// v_fma in the work type Wvec - f32 SIMD for u8/s8/u16/s16/f16/bf16/f32; the 32-bit-int/64-bit group
// works in f64 (v_float64 SIMD under CV_SIMD_64F || CV_SIMD_SCALABLE_64F, else use_simd=false scalar). Like vecBinaryKernel but
// WITHOUT its multi-channel short-row
// branch: addWeighted takes plain scalar coefficients (a multi-channel scalar is not optimized, matching
// the classic function). The broadcast branches fold the constant operand's contribution once.
// ===========================================================================
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Tr, ಬಗೆ_ಹೆಸರು Wvec, ಬಗೆ_ಹೆಸರು WT, ತಾರ್ಕಿಕ use_simd>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_Weighted_Kernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                             ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                             ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* params, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    ಆದರೆ (ಅಗಲ <= 0 || ಎತ್ತರ <= 0) ಹಿಂತಿರುಗಿ 0;
    s0y /= ಗಾತ್ರ(T); s1y /= ಗಾತ್ರ(T); dsty /= ಗಾತ್ರ(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ಅಗಲ == 1u);
    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)src1_;
    Tr* dst = (Tr*)dst_;
    ಸ್ಥಿರಾಂಕ WT alpha = (WT)params[0], beta = (WT)params[1], gamma = (WT)params[2];
    EW_TRY_COLLAPSE(2);
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    Wvec va{}, vb{}, vg{};
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Wvec>::vlanes();
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ಬಾಲ = ಅಗಲ >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    ಆದರೆ constexpr (use_simd) {
        WT fa=alpha, fb=beta, fg=gamma;
        vx_setall_as(&fa, va); vx_setall_as(&fb, vb); vx_setall_as(&fg, vg);
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < ಎತ್ತರ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ಪೂರ್ಣಾಂಕ x = 0;
    #ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಆದರೆ constexpr (use_simd)
        {
            Wvec a0,a1,a2,a3,b0,b1,b2,b3;
            ಆದರೆ (s0x == s1x) {                                // both arrays contiguous
                ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*4) {
                    ಆದರೆ (x+VECSZ*4 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ) ನಿಲ್ಲಿಸು; x = ಅಗಲ-VECSZ*4; }
                    vx_ತುಂಬು_pair_as(src0+x, a0, a1); vx_ತುಂಬು_pair_as(src0+x+VECSZ*2, a2, a3);
                    vx_ತುಂಬು_pair_as(src1+x, b0, b1); vx_ತುಂಬು_pair_as(src1+x+VECSZ*2, b2, b3);
                    a0=v_fma(a0,va,v_fma(b0,vb,vg)); a1=v_fma(a1,va,v_fma(b1,vb,vg));
                    a2=v_fma(a2,va,v_fma(b2,vb,vg)); a3=v_fma(a3,va,v_fma(b3,vb,vg));
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s1x == 0) {                             // src1 broadcast: b*beta+gamma is constant
                Wvec bb; vx_setall_as(src1, bb); Wvec bc = v_fma(bb, vb, vg);
                ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*4) {
                    ಆದರೆ (x+VECSZ*4 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ) ನಿಲ್ಲಿಸು; x = ಅಗಲ-VECSZ*4; }
                    vx_ತುಂಬು_pair_as(src0+x, a0, a1); vx_ತುಂಬು_pair_as(src0+x+VECSZ*2, a2, a3);
                    a0=v_fma(a0,va,bc); a1=v_fma(a1,va,bc); a2=v_fma(a2,va,bc); a3=v_fma(a3,va,bc);
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ {                                           // src0 broadcast: a*alpha+gamma is constant
                Wvec aa; vx_setall_as(src0, aa); Wvec acg = v_fma(aa, va, vg);
                ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*4) {
                    ಆದರೆ (x+VECSZ*4 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ) ನಿಲ್ಲಿಸು; x = ಅಗಲ-VECSZ*4; }
                    vx_ತುಂಬು_pair_as(src1+x, b0, b1); vx_ತುಂಬು_pair_as(src1+x+VECSZ*2, b2, b3);
                    b0=v_fma(b0,vb,acg); b1=v_fma(b1,vb,acg); b2=v_fma(b2,vb,acg); b3=v_fma(b3,vb,acg);
                    v_store_pair_as(dst+x, b0, b1); v_store_pair_as(dst+x+VECSZ*2, b2, b3);
                }
            }
        }
    #ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ (s0x == s1x) {
            ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + (WT)src1[x]*beta + gamma);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s0x == 0) {
            ಸ್ಥಿರಾಂಕ WT ac = (WT)src0[0]*alpha + gamma;
            ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = saturate_cast<Tr>((WT)src1[x]*beta + ac);
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಸ್ಥಿರಾಂಕ WT bc = (WT)src1[0]*beta + gamma;
            ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + bc);
        }
    }
    ಹಿಂತಿರುಗಿ 0;
}

TKernel get_ಕೂಡಿಸು_Weighted_Func_2(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R)
{
    KernelFunc f = ಶೂನ್ಯಸೂಚಕ;
    #ವ್ಯಾಖ್ಯಾನಿಸು AWS(Tt, Rr) ಕೂಡಿಸು_Weighted_Kernel<Tt, Rr, v_float32, ದಶಮಾಂಶ, ನಿಜ>
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #ವ್ಯಾಖ್ಯಾನಿಸು AWD(Tt, Rr) ಕೂಡಿಸು_Weighted_Kernel<Tt, Rr, v_float64, ದ್ವಿಗುಣ, ನಿಜ>       // f64 SIMD
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು AWD(Tt, Rr) ಕೂಡಿಸು_Weighted_Kernel<Tt, Rr, v_float32, ದ್ವಿಗುಣ, ಸುಳ್ಳು>      // scalar (v_float32 unused)
#ಆದರೆ_ಅಂತ್ಯ
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_8U:  f = R==CV_8U  ? AWS(uint8_t,  uint8_t)  : R==CV_32F ? AWS(uint8_t,  ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:  f = R==CV_8S  ? AWS(int8_t,   int8_t)   : R==CV_32F ? AWS(int8_t,   ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U: f = R==CV_16U ? AWS(uint16_t, uint16_t) : R==CV_32F ? AWS(uint16_t, ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S: f = R==CV_16S ? AWS(int16_t,  int16_t)  : R==CV_32F ? AWS(int16_t,  ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16F: f = R==CV_16F ? AWS(hfloat,   hfloat)   : R==CV_32F ? AWS(hfloat,   ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF:f = R==CV_32F ? AWS(bfloat,   ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F: f = R==CV_32F ? AWS(ದಶಮಾಂಶ,    ದಶಮಾಂಶ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U: f = R==CV_32U ? AWD(ಚಿಹ್ನೆರಹಿತ, ಚಿಹ್ನೆರಹಿತ) : R==CV_64F ? AWD(ಚಿಹ್ನೆರಹಿತ, ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S: f = R==CV_32S ? AWD(ಪೂರ್ಣಾಂಕ,      ಪೂರ್ಣಾಂಕ)      : R==CV_64F ? AWD(ಪೂರ್ಣಾಂಕ,      ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U: f = R==CV_64F ? AWD(uint64_t, ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S: f = R==CV_64F ? AWD(int64_t,  ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64F: f = R==CV_64F ? AWD(ದ್ವಿಗುಣ,   ದ್ವಿಗುಣ) : ಶೂನ್ಯಸೂಚಕ; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ: ;
    }
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು AWS
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು AWD
    ಹಿಂತಿರುಗಿ {f, ಶೂನ್ಯಸೂಚಕ, 0};
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
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
// VECSZ(T) mask bytes -> T-width lanes (u8 direct, u16/u32 via expand)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint8  ತುಂಬು_Select_ಮುಖವಾಡ(ಸ್ಥಿರಾಂಕ uchar* m, ಸ್ಥಿರಾಂಕ v_uint8&)  { ಹಿಂತಿರುಗಿ vx_ತುಂಬು(m); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint16 ತುಂಬು_Select_ಮುಖವಾಡ(ಸ್ಥಿರಾಂಕ uchar* m, ಸ್ಥಿರಾಂಕ v_uint16&) { ಹಿಂತಿರುಗಿ vx_ತುಂಬು_expand(m); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_uint32 ತುಂಬು_Select_ಮುಖವಾಡ(ಸ್ಥಿರಾಂಕ uchar* m, ಸ್ಥಿರಾಂಕ v_uint32&) { ಹಿಂತಿರುಗಿ vx_ತುಂಬು_expand_q(m); }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallSelect(ಸ್ಥಿರಾಂಕ uint8_t*  p, v_uint8&  a) { a = vx_setall_u8(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallSelect(ಸ್ಥಿರಾಂಕ uint16_t* p, v_uint16& a) { a = vx_setall_u16(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallSelect(ಸ್ಥಿರಾಂಕ uint32_t* p, v_uint32& a) { a = vx_setall_u32(*p); }
#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Tvec>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ selectKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* ಮುಖವಾಡ_3, size_t smy, size_t smx,
                        ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                        ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2_, size_t s2y, size_t s2x,
                        ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    s1y /= ಗಾತ್ರ(T);                    // the 1-byte mask's smy stays in bytes == elements
    s2y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(T);
    CV_Assert(smx <= 1u && s1x <= 1u && s2x <= 1u);

    ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ = (ಸ್ಥಿರಾಂಕ uchar*)ಮುಖವಾಡ_3;
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)src1_;
    ಸ್ಥಿರಾಂಕ T* src2 = (ಸ್ಥಿರಾಂಕ T*)src2_;
    T* dst = (T*)dst_;

    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && smy == smx*(size_t)ಅಗಲ &&
        s1y == s1x*(size_t)ಅಗಲ && s2y == s2x*(size_t)ಅಗಲ)
    { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }

    ಪೂರ್ಣಾಂಕ y = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
    ಆದರೆ constexpr (ಗಾತ್ರ(T) <= 4)
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Tvec>::vlanes();
        ಸ್ಥಿರಾಂಕ Tvec z = v_setzero_<Tvec>();

        // n-channel data under a per-pixel mask (the masked-op shape): channel axis = width (2..4),
        // one mask byte per row. Process VECSZ rows per iteration - expand the mask once and
        // interleave it across the channel lanes.
        ಆದರೆ (ಎತ್ತರ > VECSZ && 2 <= ಅಗಲ && ಅಗಲ <= 4 && smx == 0u && smy == 1u &&
            s1x == 1u && s1y == (size_t)ಅಗಲ && s2x == 1u && s2y == (size_t)ಅಗಲ &&
            dsty == (size_t)ಅಗಲ)
        {
            constexpr ಪೂರ್ಣಾಂಕ MAXVECSZ = VTraits<Tvec>::max_nlanes;
            T maskbuf[MAXVECSZ*4] = {};      // {} for -Wmaybe-uninitialized only (fully written)
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dy = VECSZ;
            ಗಾಗಿ (; y + dy <= ಎತ್ತರ; y += dy, ಮುಖವಾಡ += dy, src1 += ಅಗಲ*dy, src2 += ಅಗಲ*dy,
                                                          dst += ಅಗಲ*dy)
            {
                Tvec m0 = v_ne(ತುಂಬು_Select_ಮುಖವಾಡ(ಮುಖವಾಡ, z), z), m1, m2, m3;
                ಆದರೆ (ಅಗಲ == 2)      v_store_interleave(maskbuf, m0, m0);
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಗಲ == 3) v_store_interleave(maskbuf, m0, m0, m0);
                ಇಲ್ಲದಿದ್ದರೆ                 v_store_interleave(maskbuf, m0, m0, m0, m0);
                m0 = vx_ತುಂಬು(maskbuf);
                m1 = vx_ತುಂಬು(maskbuf + VECSZ);
                ಆದರೆ (ಅಗಲ > 2) m2 = vx_ತುಂಬು(maskbuf + VECSZ*2);
                ಆದರೆ (ಅಗಲ > 3) m3 = vx_ತುಂಬು(maskbuf + VECSZ*3);
                v_store(dst, v_select(m0, vx_ತುಂಬು(src1), vx_ತುಂಬು(src2)));
                v_store(dst + VECSZ, v_select(m1, vx_ತುಂಬು(src1 + VECSZ), vx_ತುಂಬು(src2 + VECSZ)));
                ಆದರೆ (ಅಗಲ > 2)
                    v_store(dst + VECSZ*2,
                            v_select(m2, vx_ತುಂಬು(src1 + VECSZ*2), vx_ತುಂಬು(src2 + VECSZ*2)));
                ಆದರೆ (ಅಗಲ > 3)
                    v_store(dst + VECSZ*3,
                            v_select(m3, vx_ತುಂಬು(src1 + VECSZ*3), vx_ತುಂಬು(src2 + VECSZ*3)));
            }
            // the remaining < VECSZ rows fall through to the per-row path below
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (smx == 1)                                   // per-element mask - the common case
        {
            // a branch aliasing dst is fine under the backoff (idempotent select); dst == mask is not
            ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ use_ಬಾಲ_trick = ಅಗಲ >= VECSZ*2 && dst_ != ಮುಖವಾಡ_3;
            Tvec a, b;
            ಗಾಗಿ (; y < ಎತ್ತರ; y++, ಮುಖವಾಡ += smy, src1 += s1y, src2 += s2y, dst += dsty)
            {
                // a row-constant branch still changes per row
                ಆದರೆ (s1x == 0) setallSelect(src1, a);
                ಆದರೆ (s2x == 0) setallSelect(src2, b);
                ಪೂರ್ಣಾಂಕ x = 0;
                ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ)
                {
                    ಆದರೆ (x + VECSZ > ಅಗಲ) { ಆದರೆ (!use_ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ; }
                    Tvec m = v_ne(ತುಂಬು_Select_ಮುಖವಾಡ(ಮುಖವಾಡ + x, z), z);
                    ಆದರೆ (s1x) a = vx_ತುಂಬು(src1 + x);
                    ಆದರೆ (s2x) b = vx_ತುಂಬು(src2 + x);
                    v_store(dst + x, v_select(m, a, b));
                }
                ಗಾಗಿ (; x < ಅಗಲ; x++)
                    dst[x] = ಮುಖವಾಡ[x] != 0 ? src1[x*s1x] : src2[x*s2x];
            }
            ಹಿಂತಿರುಗಿ 0;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಗಾಗಿ (; y < ಎತ್ತರ; y++, ಮುಖವಾಡ += smy, src1 += s1y, src2 += s2y, dst += dsty)
    {
        ಆದರೆ (smx == 0)                     // one mask byte per row (n-channel data / broadcast mask)
        {
            ಸ್ಥಿರಾಂಕ T* s = ಮುಖವಾಡ[0] != 0 ? src1 : src2;
            ಸ್ಥಿರಾಂಕ size_t sx = ಮುಖವಾಡ[0] != 0 ? s1x : s2x;
            ಆದರೆ ((ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*)s != (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*)dst)      // row select from dst itself is a no-op
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++) dst[x] = s[x*sx];
        }
        ಇಲ್ಲದಿದ್ದರೆ
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++)
                dst[x] = ಮುಖವಾಡ[x] != 0 ? src1[x*s1x] : src2[x*s2x];
    }
    ಹಿಂತಿರುಗಿ 0;
}

TKernel getSelectFunc_(ಪೂರ್ಣಾಂಕ mdepth, ಪೂರ್ಣಾಂಕ T)
{
    ಆದರೆ (CV_ELEM_SIZE1(mdepth) != 1)      // the mask must be a 1-byte type (u8/s8/bool)
        ಹಿಂತಿರುಗಿ {};
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (CV_ELEM_SIZE1(T))
    {
    ಸ್ಥಿತಿ 1: fptr = selectKernel<uint8_t,  v_uint8 >; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 2: fptr = selectKernel<uint16_t, v_uint16>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 4: fptr = selectKernel<uint32_t, v_uint32>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 8: fptr = selectKernel<uint64_t, v_uint32>; ನಿಲ್ಲಿಸು;   // SIMD path compiled out -> scalar
    ಪೂರ್ವನಿಯೋಜಿತ: ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
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
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallClamp(ಸ್ಥಿರಾಂಕ uchar*  p, v_uint8&   a) { a = vx_setall_u8(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallClamp(ಸ್ಥಿರಾಂಕ schar*  p, v_int8&    a) { a = vx_setall_s8(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallClamp(ಸ್ಥಿರಾಂಕ ushort* p, v_uint16&  a) { a = vx_setall_u16(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallClamp(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ*  p, v_int16&   a) { a = vx_setall_s16(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallClamp(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ* p, v_uint32& a) { a = vx_setall_u32(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallClamp(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*    p, v_int32&   a) { a = vx_setall_s32(*p); }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallClamp(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*  p, v_float32& a) { a = vx_setall_f32(*p); }
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ setallClamp(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* p, v_float64& a) { a = vx_setall_f64(*p); }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Tvec>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ clampKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                       ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* lo_, size_t s1y, size_t s1x,
                       ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* hi_, size_t s2y, size_t s2x,
                       ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                       ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    s0y /= ಗಾತ್ರ(T);
    s1y /= ಗಾತ್ರ(T);
    s2y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);

    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* lo = (ಸ್ಥಿರಾಂಕ T*)lo_;
    ಸ್ಥಿರಾಂಕ T* hi = (ಸ್ಥಿರಾಂಕ T*)hi_;
    T* dst = (T*)dst_;

    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && s0y == s0x*(size_t)ಅಗಲ &&
        s1y == s1x*(size_t)ಅಗಲ && s2y == s2x*(size_t)ಅಗಲ)
    { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < ಎತ್ತರ; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
    {
        ಪೂರ್ಣಾಂಕ x = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಆದರೆ (s0x == 1)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Tvec>::vlanes();
            ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ use_ಬಾಲ_trick = ಅಗಲ >= VECSZ*2 && lo_ != dst_ && hi_ != dst_;
            Tvec vlo, vhi;
            ಆದರೆ (s1x == 0) setallClamp(lo, vlo);
            ಆದರೆ (s2x == 0) setallClamp(hi, vhi);
            ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ)
            {
                ಆದರೆ (x + VECSZ > ಅಗಲ) { ಆದರೆ (!use_ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ; }
                ಆದರೆ (s1x) vlo = vx_ತುಂಬು(lo + x);
                ಆದರೆ (s2x) vhi = vx_ತುಂಬು(hi + x);
                v_store(dst + x, v_min(v_max(vx_ತುಂಬು(src0 + x), vlo), vhi));
            }
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; x < ಅಗಲ; x++)
        {
            T v = src0[x*s0x], l = lo[x*s1x], h = hi[x*s2x];
            dst[x] = std::min(std::max(v, l), h);
        }
    }
    ಹಿಂತಿರುಗಿ 0;
}

// Scalar-only clamp for the depths without a native SIMD lane type here (f16/bf16 - compared in
// float; 64-bit ints).
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scalarClampKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* lo_, size_t s1y, size_t s1x,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* hi_, size_t s2y, size_t s2x,
                             ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                             ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    s0y /= ಗಾತ್ರ(T); s1y /= ಗಾತ್ರ(T); s2y /= ಗಾತ್ರ(T); dsty /= ಗಾತ್ರ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);
    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* lo = (ಸ್ಥಿರಾಂಕ T*)lo_;
    ಸ್ಥಿರಾಂಕ T* hi = (ಸ್ಥಿರಾಂಕ T*)hi_;
    T* dst = (T*)dst_;
    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && s0y == s0x*(size_t)ಅಗಲ &&
        s1y == s1x*(size_t)ಅಗಲ && s2y == s2x*(size_t)ಅಗಲ)
    { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < ಎತ್ತರ; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++)
        {
            WT v = (WT)src0[x*s0x], l = (WT)lo[x*s1x], h = (WT)hi[x*s2x];
            dst[x] = saturate_cast<T>(std::min(std::max(v, l), h));
        }
    ಹಿಂತಿರುಗಿ 0;
}

TKernel getClampFunc_(ಪೂರ್ಣಾಂಕ T)
{
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_8U:   fptr = clampKernel<uchar,    v_uint8  >; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_8S:   fptr = clampKernel<schar,    v_int8   >; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16U:  fptr = clampKernel<ushort,   v_uint16 >; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16S:  fptr = clampKernel<ಗಿಡ್ಡ,    v_int16  >; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32U:  fptr = clampKernel<ಚಿಹ್ನೆರಹಿತ, v_uint32 >; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32S:  fptr = clampKernel<ಪೂರ್ಣಾಂಕ,      v_int32  >; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:  fptr = clampKernel<ದಶಮಾಂಶ,    v_float32>; ನಿಲ್ಲಿಸು;
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿತಿ CV_64F:  fptr = clampKernel<ದ್ವಿಗುಣ,   v_float64>; ನಿಲ್ಲಿಸು;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_64F:  fptr = scalarClampKernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>; ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
    ಸ್ಥಿತಿ CV_16F:  fptr = scalarClampKernel<hfloat, ದಶಮಾಂಶ>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF: fptr = scalarClampKernel<bfloat, ದಶಮಾಂಶ>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64U:  fptr = scalarClampKernel<uint64_t, uint64_t>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64S:  fptr = scalarClampKernel<int64_t,  int64_t >; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ: ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ expandKernel(size_t s0y, size_t s0x,
                        ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                        ಪೂರ್ಣಾಂಕ esz1)
{
    uchar* dst = (uchar*)dst_;
    ಆದರೆ (s0x == 0u && ಅಗಲ > 1) {
        ಪೂರ್ಣಾಂಕ h = s0y > 0u ? ಎತ್ತರ : 1;
        CV_Assert(esz1 == 1 || esz1 == 2 || esz1 == 4 || esz1 == 8);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < h; y++) {
            ಆದರೆ (esz1 == 1) {
                uchar* rowptr = dst + dsty*y;
                uchar val = rowptr[0];
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 1; x < ಅಗಲ; x++) rowptr[x] = val;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz1 == 2) {
                ushort* rowptr = (ushort*)(dst + dsty*y);
                ushort val = rowptr[0];
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 1; x < ಅಗಲ; x++) rowptr[x] = val;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (esz1 == 4) {
                ಚಿಹ್ನೆರಹಿತ* rowptr = (ಚಿಹ್ನೆರಹಿತ*)(dst + dsty*y);
                ಚಿಹ್ನೆರಹಿತ val = rowptr[0];
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 1; x < ಅಗಲ; x++) rowptr[x] = val;
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                uint64_t* rowptr = (uint64_t*)(dst + dsty*y);
                uint64_t val = rowptr[0];
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 1; x < ಅಗಲ; x++) rowptr[x] = val;
            }
        }
    }

    ಆದರೆ (s0y == 0) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 1; y < ಎತ್ತರ; y++)
            ಸ್ಮರಣೆ_ನಕಲು(dst + dsty*y, dst, (size_t)esz1*ಅಗಲ);
    }
    ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ castKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                      ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                      ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                      ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                      ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* params, ಪೂರ್ಣಾಂಕ esz1, ಶೂನ್ಯ* userdata)
{
    ದ್ವಿಮಾನ_Func castfunc = (ದ್ವಿಮಾನ_Func)userdata;
    castfunc((ಸ್ಥಿರಾಂಕ uchar*)src0_, s0y, ಶೂನ್ಯಸೂಚಕ, 0, (uchar*)dst_, dsty,
             ಗಾತ್ರ_2((s0x > 0u ? ಅಗಲ : 1), (s0y > 0u ? ಎತ್ತರ : 1)), (ಶೂನ್ಯ*)params);
    ಹಿಂತಿರುಗಿ expandKernel(s0y, s0x, dst_, dsty, ಅಗಲ, ಎತ್ತರ, esz1);
}

// ===========================================================================
// Per-op entry points (this baseline). The op-level getElemwiseFunc() dispatcher and the regular
// get*Func() forwarders live in arithm.dispatch.cpp.
// ===========================================================================

// OP_CAST / OP_CONVERT_SCALE: wrap core's convert BinaryFunc (carried in kernel.userdata) in
// castKernel, which runs it over the distinct sub-region and then expands across broadcast axes.
TKernel getCastFunc_(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth, ತಾರ್ಕಿಕ scaled)
{
    ದ್ವಿಮಾನ_Func cvt = scaled ? getConvertScaleFunc(sdepth, ddepth) : getConvertFunc(sdepth, ddepth);
    ಹಿಂತಿರುಗಿ {castKernel, (ಶೂನ್ಯ*)cvt, CV_ELEM_SIZE1(ddepth)};
}

// Non-template wrappers around the templated kernel selectors (ADD/SUB share getAddSubFunc, MIN/MAX
// share getMinMaxFunc). min/max are T x T -> T, so R is ignored (the dispatcher validates R == T).
TKernel get_ಕೂಡಿಸು_Func_2(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R) { ಹಿಂತಿರುಗಿ get_ಕೂಡಿಸು_Sub_Func<Ew_ಕೂಡಿಸು>(T, R); }
TKernel getSubFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R) { ಹಿಂತಿರುಗಿ get_ಕೂಡಿಸು_Sub_Func<EwSub>(T, R); }
TKernel getMinFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R) { (ಶೂನ್ಯ)R; ಹಿಂತಿರುಗಿ getMinMaxFunc<EwMin>(T); }
TKernel getMaxFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R) { (ಶೂನ್ಯ)R; ಹಿಂತಿರುಗಿ getMinMaxFunc<EwMax>(T); }

TKernel getHypotFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R)
{
    ಆದರೆ (R != T)
        ಹಿಂತಿರುಗಿ {};
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_16F:  fptr = vec_ದ್ವಿಮಾನ_Kernel<hfloat, hfloat, v_float32, ದಶಮಾಂಶ, EwHypot>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF: fptr = vec_ದ್ವಿಮಾನ_Kernel<bfloat, bfloat, v_float32, ದಶಮಾಂಶ, EwHypot>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ದಶಮಾಂಶ,  ದಶಮಾಂಶ,  v_float32, ದಶಮಾಂಶ, EwHypot>; ನಿಲ್ಲಿಸು;
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿತಿ CV_64F:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, v_float64, ದ್ವಿಗುಣ, EwHypot>; ನಿಲ್ಲಿಸು;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_64F:  fptr = scalar_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, EwHypot>; ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ವನಿಯೋಜಿತ: ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}

TKernel getAtan2Func_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R)
{
    ಆದರೆ (R != T)
        ಹಿಂತಿರುಗಿ {};
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_16F:  fptr = vec_ದ್ವಿಮಾನ_Kernel<hfloat, hfloat, v_float32, ದಶಮಾಂಶ, EwAtan2>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF: fptr = vec_ದ್ವಿಮಾನ_Kernel<bfloat, bfloat, v_float32, ದಶಮಾಂಶ, EwAtan2>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:  fptr = vec_ದ್ವಿಮಾನ_Kernel<ದಶಮಾಂಶ,  ದಶಮಾಂಶ,  v_float32, ದಶಮಾಂಶ, EwAtan2>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_64F:  fptr = scalar_ದ್ವಿಮಾನ_Kernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, EwAtan2>; ನಿಲ್ಲಿಸು;   // exact std::atan2
    ಪೂರ್ವನಿಯೋಜಿತ: ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}

#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
}} // namespace cv::ew
