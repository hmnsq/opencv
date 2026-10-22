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

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/hal/intrin.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.hpp"          // typed load/store-as helpers (cv::), and getConvert*Func
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstring>

ਨਾਮਾਕਾਸ਼ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// BinaryFunc and getConvertFunc / getConvertScaleFunc come from core (precomp.hpp / convert.hpp).

#ਜੇ CV_SIMD128_FP16
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_SIMD_16F
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_SIMD_16F 1
#ਜਾਂਜੇ !defined(CV_SIMD_16F)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_SIMD_16F 0       // keep `#if CV_SIMD_16F` -Wundef-clean on builds without FP16 SIMD
#ਜੇ_ਅੰਤ

#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ uchar* p, v_uint8& a)
{ a = vx_setall_u8(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ schar* p, v_int8& a)
{ a = vx_setall_s8(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ uchar* p, v_int16& a)
{ a = vx_setall_s16(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ schar* p, v_int16& a)
{ a = vx_setall_s16(*p); }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ushort* p, v_uint16& a)
{ a = vx_setall_u16(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਛੋਟਾ* p, v_int16& a)
{ a = vx_setall_s16(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ushort* p, v_int32& a)
{ a = vx_setall_s32(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਛੋਟਾ* p, v_int32& a)
{ a = vx_setall_s32(*p); }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ* p, v_uint32& a)
{ a = vx_setall_u32(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਪੂਰਨ* p, v_int32& a)
{ a = vx_setall_s32(*p); }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ uchar* p, v_float32& a)
{ a = vx_setall_f32(ਦਸ਼ਮਲਵ(*p)); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ schar* p, v_float32& a)
{ a = vx_setall_f32(ਦਸ਼ਮਲਵ(*p)); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ushort* p, v_float32& a)
{ a = vx_setall_f32(ਦਸ਼ਮਲਵ(*p)); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਛੋਟਾ* p, v_float32& a)
{ a = vx_setall_f32(ਦਸ਼ਮਲਵ(*p)); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਦਸ਼ਮਲਵ* p, v_float32& a)
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
#ਜੇ defined(__ARM_NEON)
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int32  v_ਜੋੜੋ_sat(ਅਚਲ v_int32& a,  ਅਚਲ v_int32& b)  { ਵਾਪਸ v_int32(vqaddq_s32(a.val, b.val)); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int32  v_sub_sat(ਅਚਲ v_int32& a,  ਅਚਲ v_int32& b)  { ਵਾਪਸ v_int32(vqsubq_s32(a.val, b.val)); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint32 v_ਜੋੜੋ_sat(ਅਚਲ v_uint32& a, ਅਚਲ v_uint32& b) { ਵਾਪਸ v_uint32(vqaddq_u32(a.val, b.val)); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint32 v_sub_sat(ਅਚਲ v_uint32& a, ਅਚਲ v_uint32& b) { ਵਾਪਸ v_uint32(vqsubq_u32(a.val, b.val)); }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EW_HAVE_MULSAT32 1
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint8 v_mul_sat(ਅਚਲ v_uint8& a, ਅਚਲ v_uint8& b)
{
    uint16x8_t p0 = vmull_u8(vget_ਨੀਵਾਂ_u8(a.val), vget_ਨੀਵਾਂ_u8(b.val));
    uint16x8_t p1 = vmull_u8(vget_ਉੱਚਾ_u8(a.val), vget_ਉੱਚਾ_u8(b.val));
    ਵਾਪਸ v_uint8(vcombine_u8(vqmovn_u16(p0), vqmovn_u16(p1)));
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int8 v_mul_sat(ਅਚਲ v_int8& a, ਅਚਲ v_int8& b)
{
    int16x8_t p0 = vmull_s8(vget_ਨੀਵਾਂ_s8(a.val), vget_ਨੀਵਾਂ_s8(b.val));
    int16x8_t p1 = vmull_s8(vget_ਉੱਚਾ_s8(a.val), vget_ਉੱਚਾ_s8(b.val));
    ਵਾਪਸ v_int8(vcombine_s8(vqmovn_s16(p0), vqmovn_s16(p1)));
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint16 v_mul_sat(ਅਚਲ v_uint16& a, ਅਚਲ v_uint16& b)
{
    uint32x4_t p0 = vmull_u16(vget_ਨੀਵਾਂ_u16(a.val), vget_ਨੀਵਾਂ_u16(b.val));
    uint32x4_t p1 = vmull_u16(vget_ਉੱਚਾ_u16(a.val), vget_ਉੱਚਾ_u16(b.val));
    ਵਾਪਸ v_uint16(vcombine_u16(vqmovn_u32(p0), vqmovn_u32(p1)));
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int16 v_mul_sat(ਅਚਲ v_int16& a, ਅਚਲ v_int16& b)
{
    int32x4_t p0 = vmull_s16(vget_ਨੀਵਾਂ_s16(a.val), vget_ਨੀਵਾਂ_s16(b.val));
    int32x4_t p1 = vmull_s16(vget_ਉੱਚਾ_s16(a.val), vget_ਉੱਚਾ_s16(b.val));
    ਵਾਪਸ v_int16(vcombine_s16(vqmovn_s32(p0), vqmovn_s32(p1)));
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint32 v_mul_sat(ਅਚਲ v_uint32& a, ਅਚਲ v_uint32& b)
{
    uint64x2_t p0 = vmull_u32(vget_ਨੀਵਾਂ_u32(a.val), vget_ਨੀਵਾਂ_u32(b.val));
    uint64x2_t p1 = vmull_u32(vget_ਉੱਚਾ_u32(a.val), vget_ਉੱਚਾ_u32(b.val));
    ਵਾਪਸ v_uint32(vcombine_u32(vqmovn_u64(p0), vqmovn_u64(p1)));
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int32 v_mul_sat(ਅਚਲ v_int32& a, ਅਚਲ v_int32& b)
{
    int64x2_t p0 = vmull_s32(vget_ਨੀਵਾਂ_s32(a.val), vget_ਨੀਵਾਂ_s32(b.val));
    int64x2_t p1 = vmull_s32(vget_ਉੱਚਾ_s32(a.val), vget_ਉੱਚਾ_s32(b.val));
    ਵਾਪਸ v_int32(vcombine_s32(vqmovn_s64(p0), vqmovn_s64(p1)));
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int32 v_ਜੋੜੋ_sat(ਅਚਲ v_int32& a, ਅਚਲ v_int32& b)
{
    v_int32 res = v_ਜੋੜੋ(a, b);
    v_int32 ov  = v_and(v_xor(a, res), v_xor(b, res));               // sign bit set iff overflow
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ));       // a >= 0 ? INT_MAX : INT_MIN
    ਵਾਪਸ v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int32 v_sub_sat(ਅਚਲ v_int32& a, ਅਚਲ v_int32& b)
{
    v_int32 res = v_sub(a, b);
    v_int32 ov  = v_and(v_xor(a, b), v_xor(a, res));
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ));
    ਵਾਪਸ v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint32 v_ਜੋੜੋ_sat(ਅਚਲ v_uint32& a, ਅਚਲ v_uint32& b)
{
    v_uint32 res = v_ਜੋੜੋ(a, b);
    ਵਾਪਸ v_or(res, v_lt(res, a));                                  // wrapped => all-ones
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint32 v_sub_sat(ਅਚਲ v_uint32& a, ਅਚਲ v_uint32& b)
{
    ਵਾਪਸ v_and(v_sub(a, b), v_ge(a, b));                           // borrow => zero
}
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint8 v_mul_sat(ਅਚਲ v_uint8& a, ਅਚਲ v_uint8& b)
{ v_uint16 p0, p1; v_mul_expand(a, b, p0, p1); ਵਾਪਸ v_pack(p0, p1); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int8 v_mul_sat(ਅਚਲ v_int8& a, ਅਚਲ v_int8& b)
{ v_int16 p0, p1; v_mul_expand(a, b, p0, p1); ਵਾਪਸ v_pack(p0, p1); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint16 v_mul_sat(ਅਚਲ v_uint16& a, ਅਚਲ v_uint16& b)
{ v_uint32 p0, p1; v_mul_expand(a, b, p0, p1); ਵਾਪਸ v_pack(p0, p1); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_int16 v_mul_sat(ਅਚਲ v_int16& a, ਅਚਲ v_int16& b)
{ v_int32 p0, p1; v_mul_expand(a, b, p0, p1); ਵਾਪਸ v_pack(p0, p1); }
#ਜੇ_ਅੰਤ
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F   // scalable (RVV) has v_float64 without CV_SIMD_64F
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਦੁੱਗਣਾ*   p, v_float64& a) { a = vx_setall_f64(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਪੂਰਨ*      p, v_float64& a) { a = vx_setall_f64((ਦੁੱਗਣਾ)*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ* p, v_float64& a) { a = vx_setall_f64((ਦੁੱਗਣਾ)*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ int64_t*  p, v_float64& a) { a = vx_setall_f64((ਦੁੱਗਣਾ)*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ uint64_t* p, v_float64& a) { a = vx_setall_f64((ਦੁੱਗਣਾ)*p); }
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ bfloat* p, v_float32& a)
{ a = vx_setall_f32(ਦਸ਼ਮਲਵ(*p)); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ hfloat* p, v_float32& a)
{ a = vx_setall_f32(ਦਸ਼ਮਲਵ(*p)); }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_ਲੱਦੋ_pair_as(ਅਚਲ uchar* p, v_uint8& a, v_uint8& b)
{
    a = vx_ਲੱਦੋ(p);
    b = vx_ਲੱਦੋ(p + VTraits<v_uint8>::vlanes());
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_ਲੱਦੋ_pair_as(ਅਚਲ schar* p, v_int8& a, v_int8& b)
{
    a = vx_ਲੱਦੋ(p);
    b = vx_ਲੱਦੋ(p + VTraits<v_int8>::vlanes());
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_ਲੱਦੋ_pair_as(ਅਚਲ uchar* p, v_uint32& a, v_uint32& b)
{
    v_uint16 w = vx_ਲੱਦੋ_expand(p);
    v_expand(w, a, b);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_store_pair_as(uchar* p, ਅਚਲ v_uint8& a, ਅਚਲ v_uint8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_uint8>::vlanes(), b);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_store_pair_as(schar* p, ਅਚਲ v_int8& a, ਅਚਲ v_int8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_int8>::vlanes(), b);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_store_pair_as(ਪੂਰਨ* p, ਅਚਲ v_int16& a, ਅਚਲ v_int16& b)
{
    ਅਚਲ ਪੂਰਨ nlanes32 = VTraits<v_int32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v0);
    v_store(p + nlanes32, v1);
    v_store(p + nlanes32*2, v2);
    v_store(p + nlanes32*3, v3);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_store_pair_as(ਦਸ਼ਮਲਵ* p, ਅਚਲ v_int16& a, ਅਚਲ v_int16& b)
{
    ਅਚਲ ਪੂਰਨ nlanes32 = VTraits<v_float32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v_cvt_f32(v0));
    v_store(p + nlanes32, v_cvt_f32(v1));
    v_store(p + nlanes32*2, v_cvt_f32(v2));
    v_store(p + nlanes32*3, v_cvt_f32(v3));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_store_pair_as(ਦਸ਼ਮਲਵ* p, ਅਚਲ v_int32& a, ਅਚਲ v_int32& b)
{
    v_store(p, v_cvt_f32(a));
    v_store(p + VTraits<v_float32>::vlanes(), v_cvt_f32(b));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_store_pair_as(hfloat* p, ਅਚਲ v_float32& a, ਅਚਲ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_store_pair_as(bfloat* p, ਅਚਲ v_float32& a, ਅਚਲ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

// Reinterpret a comparison result (whose lane type matches the operands) as the UNSIGNED integer
// vector of the same width: u8->u8, s8->u8, u16/s16->u16, u32/s32/f32->u32. The destination type
// drives the overload, so the compare kernel needs no per-width if constexpr.
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_uint8&  s, v_uint8&  d) { d = s; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_int8&   s, v_uint8&  d) { d = v_reinterpret_as_u8(s); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_uint16& s, v_uint16& d) { d = s; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_int16&  s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_uint32& s, v_uint32& d) { d = s; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_int32&  s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_float32& s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
#ਜੇ CV_SIMD_16F
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_float16& s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
#ਜੇ_ਅੰਤ
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_reinterpret_as(ਅਚਲ v_float64& s, v_uint64& d) { d = v_reinterpret_as_u64(s); }
#ਜੇ_ਅੰਤ

// Broadcast the compare mask value (1 or 255) across the unsigned work vector.
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_setall_ਨਕਾਬ(v_uint8&  v, uchar t) { v = vx_setall_u8(t); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_setall_ਨਕਾਬ(v_uint16& v, uchar t) { v = vx_setall_u16(t); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_setall_ਨਕਾਬ(v_uint32& v, uchar t) { v = vx_setall_u32(t); }

// NOTE: v_store_pair_as(uchar*, v_uint16/v_uint32, ...) - the narrowing pack-to-u8 stores we need -
// already come from core's convert.hpp (included above), so they are not redefined here.

#ਜੇ CV_SIMD_16F
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_setall_as(ਅਚਲ hfloat* p, v_float16& a)
{ a = vx_setall_f16(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ vx_ਲੱਦੋ_pair_as(ਅਚਲ hfloat* p, v_float16& a, v_float16& b)
{
    a = vx_ਲੱਦੋ(p);
    b = vx_ਲੱਦੋ(p + VTraits<v_float16>::vlanes());
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_store_pair_as(hfloat* p, ਅਚਲ v_float16& a, ਅਚਲ v_float16& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_float16>::vlanes(), b);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਨਾਮਾਕਾਸ਼ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      arithm.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel get_ਜੋੜੋ_Func_2(ਪੂਰਨ T, ਪੂਰਨ R);
TKernel getSubFunc_(ਪੂਰਨ T, ਪੂਰਨ R);
TKernel getMulFunc_(ਪੂਰਨ T, ਪੂਰਨ R);
TKernel getDivFunc_(ਪੂਰਨ T, ਪੂਰਨ R, ਤਾਰਕਿਕ checked);
TKernel getMinFunc_(ਪੂਰਨ T, ਪੂਰਨ R);
TKernel getMaxFunc_(ਪੂਰਨ T, ਪੂਰਨ R);
TKernel getAbsdiffFunc_(ਪੂਰਨ T, ਪੂਰਨ R);
TKernel getHypotFunc_(ਪੂਰਨ T, ਪੂਰਨ R);         // hypot = sqrt(x^2+y^2), float depths, T x T -> T
TKernel getAtan2Func_(ਪੂਰਨ T, ਪੂਰਨ R);         // atan2(y, x), radians (-pi, pi], float depths
TKernel getCmpFunc_(ਸਿਖਰ op, ਪੂਰਨ T);
TKernel getBitwiseFunc_(ਸਿਖਰ op, ਪੂਰਨ esz);                    // OP_AND / OP_OR / OP_XOR, by element size
TKernel getNotFunc_(ਪੂਰਨ esz);                                // OP_NOT, by element size
TKernel get_ਜੋੜੋ_Weighted_Func_2(ਪੂਰਨ T, ਪੂਰਨ R);                   // OP_ADDW, a*alpha+b*beta+gamma (T x T -> R)
TKernel getSelectFunc_(ਪੂਰਨ mdepth, ਪੂਰਨ T);   // OP_SELECT: 1-byte mask, a/b/dst of T (by esz)
TKernel getClampFunc_(ਪੂਰਨ T);                // OP_CLAMP: min(max(x, lo), hi), all operands of T
TKernel getCastFunc_(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth, ਤਾਰਕਿਕ scaled);   // OP_CAST / OP_CONVERT_SCALE

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors (vector + scalar). New binary ops slot in here.
// ===========================================================================
ਬਣਤਰ Ew_ਜੋੜੋ {
    // useScalar: does the op consume the scale scalar (params[0]) in vec()/preproc()? When false,
    // vecBinaryKernel's fast 2-arg branch is taken unconditionally (the check folds at compile time);
    // when true, it is taken only if the runtime scale is exactly 1.
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_ਜੋੜੋ(a, b); }
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: v_add wraps - use the saturating version to match the scalar int64+clamp tail
    ਸਥਿਰ v_int32  vec(ਅਚਲ v_int32& a,  ਅਚਲ v_int32& b)  { ਵਾਪਸ v_ਜੋੜੋ_sat(a, b); }
    ਸਥਿਰ v_uint32 vec(ਅਚਲ v_uint32& a, ਅਚਲ v_uint32& b) { ਵਾਪਸ v_ਜੋੜੋ_sat(a, b); }
#ਜੇ_ਅੰਤ
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    // Accumulate in the promoted type, NOT in W: for the native saturating path W is the narrow
    // lane type (schar/short/...), and (W)(a+b) would wrap in 8/16 bits before saturate_cast<Tr>
    // could clamp. Letting a+b promote (narrow -> int) keeps saturation for 8/16-bit outputs and
    // the natural wrap for 32/64-bit (both matching cv::add). The SIMD path already saturates.
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ W(a + b); }
};

ਬਣਤਰ EwSub {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_sub(a, b); }
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: see EwAdd::vec
    ਸਥਿਰ v_int32  vec(ਅਚਲ v_int32& a,  ਅਚਲ v_int32& b)  { ਵਾਪਸ v_sub_sat(a, b); }
    ਸਥਿਰ v_uint32 vec(ਅਚਲ v_uint32& a, ਅਚਲ v_uint32& b) { ਵਾਪਸ v_sub_sat(a, b); }
#ਜੇ_ਅੰਤ
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ W(a - b); }   // see EwAdd::scl
    // 64-bit unsigned has no wider WT to hold a-b, so the generic path would wrap on underflow.
    // Saturate at 0 to match cv::subtract (8/16/32-bit already saturate via SIMD floor / wide WT).
    ਸਥਿਰ uint64_t scl(uint64_t a, uint64_t b, uint64_t) { ਵਾਪਸ uint64_t(a >= b)*(a - b); }
};

// mul/div compute in a wide FLOAT work type (W = float for <=16-bit/f16/bf/f32, double for
// 32/64-bit/f64), matching cv::multiply/divide; the executor casts the work-type result down to
// rdepth. scalar (reference) path only for now - SIMD can follow. (No vec(): never instantiated.)
// vec(a, b, scale): the 3rd arg is the scale vector. COMMUTATIVE ops (mul, absdiff) ignore it - their
// scale is folded into the (cheaper) preproc of one operand. DIVISION is NOT commutative, so its scale
// MUST stick to the numerator; it takes the scale in vec (numerator*scale/denominator) and leaves
// preproc as identity. vecBinaryKernel always passes vscalar to vec, so every branch (incl. a broadcast
// denominator) divides correctly. The 2-arg vec (scale==1) is the both-contiguous fast path.
ਬਣਤਰ EwMul {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਸੱਚ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_mul(a, b); }
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    // integer lanes: full product clamped to the lane type (v_mul_sat) == cv::multiply semantics
    // at scale 1. These drive the scale==1 fast path on FULL-width registers (Wvec1 = the native
    // lane vector): whole-register loads/stores, widening happens inside the multiply itself.
    // The guard matches the v_mul_sat definitions above (like EwAdd's 32-bit overloads): in
    // no-SIMD builds the vector typedefs still exist (intrin_cpp), but the helpers do not.
    ਸਥਿਰ v_uint8  vec(ਅਚਲ v_uint8& a,  ਅਚਲ v_uint8& b)  { ਵਾਪਸ v_mul_sat(a, b); }
    ਸਥਿਰ v_int8   vec(ਅਚਲ v_int8& a,   ਅਚਲ v_int8& b)   { ਵਾਪਸ v_mul_sat(a, b); }
    ਸਥਿਰ v_uint16 vec(ਅਚਲ v_uint16& a, ਅਚਲ v_uint16& b) { ਵਾਪਸ v_mul_sat(a, b); }
    ਸਥਿਰ v_int16  vec(ਅਚਲ v_int16& a,  ਅਚਲ v_int16& b)  { ਵਾਪਸ v_mul_sat(a, b); }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ EW_HAVE_MULSAT32
    ਸਥਿਰ v_uint32 vec(ਅਚਲ v_uint32& a, ਅਚਲ v_uint32& b) { ਵਾਪਸ v_mul_sat(a, b); }
    ਸਥਿਰ v_int32  vec(ਅਚਲ v_int32& a,  ਅਚਲ v_int32& b)  { ਵਾਪਸ v_mul_sat(a, b); }
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V& s) { ਵਾਪਸ v_mul(a, s); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST s) { ਵਾਪਸ a * b * s; }
};
// div has two variants by the COMMON INPUT type (matching cv::'s per-type kernel choice): integer
// inputs guard divide-by-zero -> 0 (cv:: iscalar_div); float inputs do NOT guard (cv:: fscalar_div,
// a/0 -> inf), which then saturates on the cast to an integer output exactly like cv::divide.
ਬਣਤਰ EwDivInt {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਸੱਚ;
    // integer inputs computed in the float work type: guard b==0 -> 0. Scale rides the numerator (vec).
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) {
        ਅਚਲ V z = v_setzero_<V>();
        ਵਾਪਸ v_select(v_eq(b, z), z, v_div(a, b));
    }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ V& s) {
        ਅਚਲ V z = v_setzero_<V>();
        ਵਾਪਸ v_select(v_eq(b, z), z, v_div(v_mul(a, s), b));
    }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }   // identity: scale is in vec
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST s) { ਵਾਪਸ b != W(0) ? a * s / b : W(0); }
};
ਬਣਤਰ EwDivFlt {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਸੱਚ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_div(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ V& s) { ਵਾਪਸ v_div(v_mul(a, s), b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }   // identity: scale is in vec
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST s) { ਵਾਪਸ a * s / b; }
};

// atan2(y, x) in RADIANS over the standard C range (-pi, pi] - the fastAtan2 minimax polynomial
// (mathfuncs_core.simd.hpp v_atan_f32) with the 180/pi factor dropped and the C quadrant logic
// (fastAtan2 returns degrees in [0, 360)). Absolute accuracy ~1e-5 rad, same as cv::fastAtan2.
// Generic over the universal-intrinsic float vector type.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V>
ਸਥਿਰ ਸਤਰਅੰਦਰ V v_atan2(ਅਚਲ V& y, ਅਚਲ V& x)
{
    ਵਰਤੋਂ LT = ਕਿਸਮ_ਨਾਂ VTraits<V>::lane_type;
    ਅਚਲ V eps  = v_setall_<V>((LT)DBL_EPSILON);
    ਅਚਲ V z    = v_setzero_<V>();
    ਅਚਲ V p7   = v_setall_<V>((LT)-0.04432655554792128);
    ਅਚਲ V p5   = v_setall_<V>((LT)0.1555786518463281);
    ਅਚਲ V p3   = v_setall_<V>((LT)-0.3258083974640975);
    ਅਚਲ V p1   = v_setall_<V>((LT)0.9997878412794807);
    ਅਚਲ V vpi2 = v_setall_<V>((LT)(CV_PI/2));
    ਅਚਲ V vpi  = v_setall_<V>((LT)CV_PI);

    V ax = v_abs(x), ay = v_abs(y);
    V c  = v_div(v_min(ax, ay), v_ਜੋੜੋ(v_max(ax, ay), eps));
    V c2 = v_mul(c, c);
    V a  = v_mul(v_fma(v_fma(v_fma(p7, c2, p5), c2, p3), c2, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(vpi2, a));
    a = v_select(v_lt(x, z), v_sub(vpi, a), a);
    a = v_select(v_lt(y, z), v_sub(z, a), a);
    ਵਾਪਸ a;
}

ਬਣਤਰ EwAtan2 {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_atan2(a, b); }   // a = y, b = x
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ std::atan2(a, b); }
};

// hypot(x, y) = sqrt(x^2 + y^2): NAIVE (matches cv::magnitude; overflow at |x| ~ 1e19+ for f32
// inputs is accepted), computed in the float work type; T x T -> T over the float depths.
ਬਣਤਰ EwHypot {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_sqrt(v_fma(a, a, v_mul(b, b))); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ std::ਵਰਗਮੂਲ(a*a + b*b); }
};

// min / max / absdiff: T x T -> T (same depth in and out, no scale). v_min/v_max exist for every
// vector lane type (64-bit ints fall back to scalar). absdiff uses v_absdiff (defined for the
// UNSIGNED and float lane types - signed/wide depths go through the scalar path), and the scalar
// |a-b| is computed branch-wise so it never underflows an unsigned work type.
ਬਣਤਰ EwMin {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_min(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ std::min(a, b); }
};
ਬਣਤਰ EwMax {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_max(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ std::max(a, b); }
};
ਬਣਤਰ EwAbsdiff {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    // The absdiff RESULT is the UNSIGNED type of the input width: v_absdiff(v_int8/16/32) already returns
    // v_uint8/16/32 (the true |a-b|, which can exceed the signed max), and v_absdiff on unsigned/float
    // returns the same type. vec() therefore returns THAT type (deduced), not the input V, so the kernel
    // stores it through the matching UNSIGNED v_store_pair_as (an honest same-type store) - no reinterpret,
    // no touching the saturating-narrow overloads' semantics. (This is why the kernel keeps the vec
    // result in its own variable rather than reusing the input operand.)
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ ਸਵੈਚਾਲਿਤ vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_absdiff(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ ਸਵੈਚਾਲਿਤ vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ v_absdiff(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ a > b ? W(a - b) : W(b - a); }
};

// Signed T -> SAME signed T (rdepth==T): |a-b| saturated into the signed range, in ONE pass. cv::absdiff
// keeps the signed depth, so the generic path computes absdiff->unsigned then casts back down (2 insns);
// this fuses it. v_absdiff yields the unsigned |a-b|; v_min clamps to the signed max (already >=0), then
// a same-width reinterpret to signed (all values now fit).
ਬਣਤਰ EwAbsdiffS {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਥਿਰ v_int8  vec(ਅਚਲ v_int8&  a, ਅਚਲ v_int8&  b) { ਵਾਪਸ v_reinterpret_as_s8 (v_min(v_absdiff(a, b), vx_setall_u8 (0x7f))); }
    ਸਥਿਰ v_int16 vec(ਅਚਲ v_int16& a, ਅਚਲ v_int16& b) { ਵਾਪਸ v_reinterpret_as_s16(v_min(v_absdiff(a, b), vx_setall_u16(0x7fff))); }
    ਸਥਿਰ v_int32 vec(ਅਚਲ v_int32& a, ਅਚਲ v_int32& b) { ਵਾਪਸ v_reinterpret_as_s32(v_min(v_absdiff(a, b), vx_setall_u32(0x7fffffff))); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ a > b ? W(a - b) : W(b - a); }
    // 64-bit signed has no wider work type to widen into, and |INT64_MIN - b| does not fit in
    // int64_t, so the difference is taken in uint64 - exact for every pair - and then clamped.
    // The narrower widths reach this overload only through their int64 work type, where |a - b|
    // is at most 2^32 and the clamp is a no-op.
    ਸਥਿਰ int64_t scl(int64_t a, int64_t b, int64_t)
    {
        ਅਚਲ uint64_t d = (a > b) ? (uint64_t)a - (uint64_t)b : (uint64_t)b - (uint64_t)a;
        ਵਾਪਸ (int64_t)std::min<uint64_t>(d, (uint64_t)INT64_MAX);
    }
};

// compare: T x T -> u8 mask. cmp(a,b) is the scalar relation over a work type W (integers compare
// directly, f16/bf16 through float); vec(a,b) is the vector relation returning an all-ones/zero mask
// in the operand's own lane type. The kernel turns either into 0 / trueVal (1 or 255 via TKernel::flags).
ਬਣਤਰ EwCmpEq { ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ ਤਾਰਕਿਕ cmp(W a, W b) { ਵਾਪਸ a == b; }
                 ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_eq(a, b); } };
ਬਣਤਰ EwCmpNe { ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ ਤਾਰਕਿਕ cmp(W a, W b) { ਵਾਪਸ a != b; }
                 ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_ne(a, b); } };
// LT/LE are synthesized from GT/GE via the EW_KERNEL_SWAP01 flag (a<b == b>a), so no Lt/Le kernels.
ਬਣਤਰ EwCmpGt { ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ ਤਾਰਕਿਕ cmp(W a, W b) { ਵਾਪਸ a > b; }
                 ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_gt(a, b); } };
ਬਣਤਰ EwCmpGe { ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ ਤਾਰਕਿਕ cmp(W a, W b) { ਵਾਪਸ a >= b; }
                 ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_ge(a, b); } };

// bitwise AND/OR/XOR: bit-pattern op, type-agnostic. Run on the UNSIGNED integer whose width matches
// the element (u8/u16/u32/u64), so one functor set covers every depth. No scale, no widening (T x T ->
// T, exactly like min/max); preproc is the identity (min/max share this shape). 64-bit uses the scalar
// path (no widening vector helpers), the rest ride vecBinaryKernel's native same-type path.
ਬਣਤਰ EwAnd {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_and(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ W(a & b); }
};
ਬਣਤਰ EwOr {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_or(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ W(a | b); }
};
ਬਣਤਰ EwXor {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ useScalar = ਝੂਠ;
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b) { ਵਾਪਸ v_xor(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V, ਕਿਸਮ_ਨਾਂ S> ਸਥਿਰ V vec(ਅਚਲ V& a, ਅਚਲ V& b, ਅਚਲ S&) { ਵਾਪਸ vec(a, b); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V preproc(ਅਚਲ V& a, ਅਚਲ V&) { ਵਾਪਸ a; }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W, ਕਿਸਮ_ਨਾਂ ST> ਸਥਿਰ W scl(W a, W b, ST) { ਵਾਪਸ W(a ^ b); }
};

// Collapse a gap-free 2D tile to 1D (call with the per-operand x/y-steps).
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EW_TRY_COLLAPSE(NSRC) \
    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && \
        s0y == s0x*(size_t)ਚੌੜਾਈ && (NSRC < 2 || s1y == s1x*(size_t)ਚੌੜਾਈ)) \
    { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Tr, ਕਿਸਮ_ਨਾਂ WT, ਵਰਗ Op, ਕਿਸਮ_ਨਾਂ ST=WT>
ਸਥਿਰ ਪੂਰਨ scalar_ਦੁਆਧਾਰੀ_Kernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                              ਅਚਲ ਖਾਲੀ* src1_, size_t s1y, size_t s1x,
                              ਅਚਲ ਖਾਲੀ*, size_t, size_t, ਖਾਲੀ* dst_, size_t dsty,
                              ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਅਚਲ ਦੁੱਗਣਾ* params, ਪੂਰਨ, ਖਾਲੀ*)
{
    ਜੇ (ਚੌੜਾਈ <= 0 || ਉਚਾਈ <= 0) ਵਾਪਸ 0;
    s0y /= ਆਕਾਰ(T);
    s1y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ਚੌੜਾਈ == 1u);

    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* src1 = (ਅਚਲ T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    ਲਈ (ਪੂਰਨ y = 0; y < ਉਚਾਈ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ਜੇ (s0x == s1x) {
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], (WT)src1[x], scalar));
        }
        ਨਹੀਂਤਾਂ ਜੇ (s0x == 0) {
            WT sc0 = (WT)src0[0];
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++)
                dst[x] = saturate_cast<Tr>(Op::scl(sc0, (WT)src1[x], scalar));
        }
        ਨਹੀਂਤਾਂ {
            WT sc1 = (WT)src1[0];
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], sc1, scalar));
        }
    }
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT>
ਸਥਿਰ ਖਾਲੀ expandScalar(ਅਚਲ T* sc, size_t sx, ਪੂਰਨ n0, WT* scbuf, ਪੂਰਨ n)
{
    ਜੇ (n0 <= 0 || n <= 0) ਵਾਪਸ;
    ਪੂਰਨ i = 0;
    ਲਈ (; i < n0; i++) scbuf[i] = (WT)sc[i*sx];
    ਲਈ (; i < n; i++) scbuf[i] = scbuf[i - n0];
}

// Decode the per-channel patch bytes (mask + value) from the kernel flags into small arrays. Uniform
// (no EW_CMP_PATCH): mask = trueVal, value = 0 (an ordinary compare). Per-channel: from the 4-bit
// fields. The compare kernels apply  result = (rawmask & mask) | value  per channel - folding the
// former separate patch pass into the compare (one pass).
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cmpUnpackPatch(ਪੂਰਨ flags, uchar trueVal, uchar mvals[4], uchar vvals[4])
{
    ਜੇ (flags & EW_CMP_PATCH)
        ਲਈ (ਪੂਰਨ c = 0; c < 4; c++)
        {
            ਅਚਲ ਪੂਰਨ f = (flags >> (EW_CMP_PATCH_ਖਿਸਕਾਓ + c*4)) & 0xF;
            mvals[c] = (uchar)cmpPatchByte(f & 3);
            vvals[c] = (uchar)cmpPatchByte((f >> 2) & 3);
        }
    ਨਹੀਂਤਾਂ
        ਲਈ (ਪੂਰਨ c = 0; c < 4; c++) { mvals[c] = trueVal; vvals[c] = 0; }
}

// Scalar comparison kernel: T x T -> u8 mask. Result is 0 (false) or `trueVal` (true), trueVal read
// from TKernel::flags - 255 (default, matching cv::compare) or 1 (a numpy-style 0/1 mask). Integers
// compare directly (WT == T); f16/bf16 compare through a float WT. This is the fallback for the depths
// the SIMD kernel doesn't cover (f16/bf16/64-bit). stepx in {0,1}; dst is u8, byte step == element step.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT, ਵਰਗ Cmp>
ਸਥਿਰ ਪੂਰਨ scalarCompareKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                         ਅਚਲ ਖਾਲੀ* src1_, size_t s1y, size_t s1x,
                         ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                         ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                         ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ flags, ਖਾਲੀ*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    ਜੇ (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= ਆਕਾਰ(T);
    s1y /= ਆਕਾਰ(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ਚੌੜਾਈ == 1u);

    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* src1 = (ਅਚਲ T*)src1_;
    uchar* dst = (uchar*)dst_;
    ਅਚਲ uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    ਅਚਲ ਤਾਰਕਿਕ perch = (flags & EW_CMP_PATCH) != 0;          // per-channel fix-up (width=cn<=4)
    CV_Assert(!perch || ਚੌੜਾਈ <= 4);                         // a per-channel patch is a short-row tile
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                         : (uchar)((cond) ? trueVal : 0))

    EW_TRY_COLLAPSE(2);
    ਲਈ (ਪੂਰਨ y = 0; y < ਉਚਾਈ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ਜੇ (s0x == s1x) {
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], (WT)src1[x]), x);
        }
        ਨਹੀਂਤਾਂ ਜੇ (s0x == 0) {
            WT a = (WT)src0[0];
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, (WT)src1[x]), x);
        }
        ਨਹੀਂਤਾਂ {
            WT b = (WT)src1[0];
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], b), x);
        }
    }
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ EW_CMP_APPLY
    ਵਾਪਸ 0;
}

// SIMD comparison kernel: T x T -> u8 mask, for the directly-comparable depths (u8/s8/u16/s16/u32/
// s32/f32). vec(a,b) gives an all-ones/zero mask in the operand's lane type; v_reinterpret_as turns it
// into the unsigned int of the same width; cmpFuse packs to u8 and applies (rawmask & M) | V. The
// per-row SIMD body runs when an operand is contiguous or broadcast; a per-channel patch (M/V differ
// by channel) only ever arrives as a short-row (width=cn) tile and is handled there.
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Vvec, ਕਿਸਮ_ਨਾਂ Uvec, ਵਰਗ Cmp>
ਸਥਿਰ ਪੂਰਨ vecCompareKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                            ਅਚਲ ਖਾਲੀ* src1_, size_t s1y, size_t s1x,
                            ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                            ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                            ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ flags, ਖਾਲੀ*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    ਜੇ (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= ਆਕਾਰ(T);
    s1y /= ਆਕਾਰ(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ਚੌੜਾਈ == 1u);

    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* src1 = (ਅਚਲ T*)src1_;
    uchar* dst = (uchar*)dst_;
    ਅਚਲ uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];                                 // per-channel fix-up: (rawmask & m) | v
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    ਅਚਲ ਤਾਰਕਿਕ perch = (flags & EW_CMP_PATCH) != 0;
    CV_Assert(!perch || ਚੌੜਾਈ <= 4);                          // a per-channel patch is a short-row tile

    EW_TRY_COLLAPSE(2);
    ਪੂਰਨ y = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    // Short rows (2/3/4 elements): a per-channel scalar over a multi-channel image arrives as a
    // (width=cn) x (height=pixels) tile with the scalar broadcast over rows (s?y==0). The per-row SIMD
    // below never triggers at such a tiny width, so expand the width<=4 broadcast operand (threshold
    // AND the interleaved per-channel mask/value bytes) across the lanes and compare many rows at once.
    // This is also the ONLY path a per-channel patch (M/V differ by channel) reaches.
    // constexpr: the short-row loads T natively (vx_load->Vvec), so it exists ONLY at native width. The
    // widened f16/bf16->f32 path (sizeof(T) < Vvec lane) and 8-byte f64 fall through to per-row + scalar.
    // (sizeof(T) <= 4 is part of the constexpr gate: for f64 the body would compile to nothing but
    // dead stores - gcc 9 flags scbuf/bsrc/bsx as set-but-not-used there.)
    ਜੇ constexpr (ਆਕਾਰ(T) == ਆਕਾਰ(ਕਿਸਮ_ਨਾਂ VTraits<Vvec>::lane_type) && ਆਕਾਰ(T) <= 4)
    ਜੇ (ਉਚਾਈ > 1 && ਚੌੜਾਈ <= 4 &&
        ((s0y == 0 && s1y == (size_t)ਚੌੜਾਈ*s1x) || (s1y == 0 && s0y == (size_t)ਚੌੜਾਈ*s0x)) &&
        dsty == (size_t)ਚੌੜਾਈ)
    {
        ਅਚਲ ਪੂਰਨ VECSZ  = VTraits<Vvec>::vlanes();          // sizeof(T)-type lanes
        ਅਚਲ ਪੂਰਨ VECSZ8 = VTraits<v_uint8>::vlanes();       // u8 output lanes
        constexpr ਪੂਰਨ MAXV8 = VTraits<v_uint8>::max_nlanes;
        T scbuf[MAXV8 * 3] = {};                             // interleaved threshold (elements)
        uchar mbuf[MAXV8 * 3] = {}, vbuf[MAXV8 * 3] = {};    // interleaved mask / value (bytes)
        // ^ the {} inits are for -Wmaybe-uninitialized only: expandScalar/decode fill every lane that
        //   is later read, but the compiler cannot prove it with a runtime VECSZ
        ਅਚਲ ਤਾਰਕਿਕ bc0 = (s0y == 0);                         // src0 is the broadcast (short) operand
        ਅਚਲ T* bsrc = bc0 ? src0 : src1; ਅਚਲ size_t bsx = bc0 ? s0x : s1x;
        ਅਚਲ T*& src = bc0 ? src1 : src0;                   // the row-stepping operand (advanced below)
        ਅਚਲ size_t sy = bc0 ? s1y : s0y;
        // The compare direction (scalar-first when bc0, array-first otherwise) is hoisted OUT of the row
        // loop with an explicit if(bc0) - the loop is written twice so no per-iteration branch remains.
        // Per branch: own unroll, threshold + interleaved M/V loaded ONCE. sizeof==1 keeps 12 vectors.
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EW_CMPR(r, a, b) v_reinterpret_as(Cmp::vec(a, b), r)   // rawmask -> sizeof(T)-byte uint
        ਜੇ constexpr (ਆਕਾਰ(T) == 1)                        // 3 u8 masks -> 3 u8 stores
        {
            ਅਚਲ ਪੂਰਨ ewidth = VECSZ8 * 3;
            expandScalar(bsrc, bsx, ਚੌੜਾਈ, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, ਚੌੜਾਈ, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, ਚੌੜਾਈ, vbuf, ewidth);
            ਅਚਲ ਪੂਰਨ dy = ewidth / ਚੌੜਾਈ;
            Vvec  sc0=vx_ਲੱਦੋ(scbuf),  sc1=vx_ਲੱਦੋ(scbuf+VECSZ),   sc2=vx_ਲੱਦੋ(scbuf+VECSZ*2);
            v_uint8 M0=vx_ਲੱਦੋ(mbuf),  M1=vx_ਲੱਦੋ(mbuf+VECSZ8),    M2=vx_ਲੱਦੋ(mbuf+VECSZ8*2);
            v_uint8 V0=vx_ਲੱਦੋ(vbuf),  V1=vx_ਲੱਦੋ(vbuf+VECSZ8),    V2=vx_ਲੱਦੋ(vbuf+VECSZ8*2);
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EW_ROW1(A0,B0,A1,B1,A2,B2) \
                ਲਈ (; y + dy <= ਉਚਾਈ; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 o0=v_or(v_and(r0,M0),V0),o1=v_or(v_and(r1,M1),V1),o2=v_or(v_and(r2,M2),V2); \
                    v_store(dst, o0); v_store(dst+VECSZ8, o1); v_store(dst+VECSZ8*2, o2); }
            ਜੇ (bc0) EW_ROW1(sc0,vx_ਲੱਦੋ(src), sc1,vx_ਲੱਦੋ(src+VECSZ), sc2,vx_ਲੱਦੋ(src+VECSZ*2))
            ਨਹੀਂਤਾਂ     EW_ROW1(vx_ਲੱਦੋ(src),sc0, vx_ਲੱਦੋ(src+VECSZ),sc1, vx_ਲੱਦੋ(src+VECSZ*2),sc2)
            #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ EW_ROW1
        }
        ਨਹੀਂਤਾਂ ਜੇ constexpr (ਆਕਾਰ(T) == 2)                   // 3 u16 masks -> 1 full + 1 half u8 store
        {
            ਅਚਲ ਪੂਰਨ ewidth = VECSZ * 3;
            expandScalar(bsrc, bsx, ਚੌੜਾਈ, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, ਚੌੜਾਈ, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, ਚੌੜਾਈ, vbuf, ewidth);
            ਅਚਲ ਪੂਰਨ dy = ewidth / ਚੌੜਾਈ;
            Vvec  sc0=vx_ਲੱਦੋ(scbuf), sc1=vx_ਲੱਦੋ(scbuf+VECSZ), sc2=vx_ਲੱਦੋ(scbuf+VECSZ*2);
            v_uint8 M0=vx_ਲੱਦੋ(mbuf), M1=vx_ਲੱਦੋ(mbuf+VECSZ8);
            v_uint8 V0=vx_ਲੱਦੋ(vbuf), V1=vx_ਲੱਦੋ(vbuf+VECSZ8);
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EW_ROW2(A0,B0,A1,B1,A2,B2) \
                ਲਈ (; y + dy <= ਉਚਾਈ; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 b0=v_pack(r0,r1), b1=v_pack(r2,r2); \
                    v_uint8 o0=v_or(v_and(b0,M0),V0), o1=v_or(v_and(b1,M1),V1); \
                    v_store(dst, o0); v_store_ਨੀਵਾਂ(dst+VECSZ8, o1); }
            ਜੇ (bc0) EW_ROW2(sc0,vx_ਲੱਦੋ(src), sc1,vx_ਲੱਦੋ(src+VECSZ), sc2,vx_ਲੱਦੋ(src+VECSZ*2))
            ਨਹੀਂਤਾਂ     EW_ROW2(vx_ਲੱਦੋ(src),sc0, vx_ਲੱਦੋ(src+VECSZ),sc1, vx_ਲੱਦੋ(src+VECSZ*2),sc2)
            #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ EW_ROW2
        }
        ਨਹੀਂਤਾਂ ਜੇ constexpr (ਆਕਾਰ(T) == 4)                   // sizeof==4: 6 u32 -> 3 u16 -> 1 full + 1 half u8
        {
            ਅਚਲ ਪੂਰਨ ewidth = VECSZ * 6;
            expandScalar(bsrc, bsx, ਚੌੜਾਈ, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, ਚੌੜਾਈ, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, ਚੌੜਾਈ, vbuf, ewidth);
            ਅਚਲ ਪੂਰਨ dy = ewidth / ਚੌੜਾਈ;
            Vvec sc0=vx_ਲੱਦੋ(scbuf),sc1=vx_ਲੱਦੋ(scbuf+VECSZ),sc2=vx_ਲੱਦੋ(scbuf+VECSZ*2),
                 sc3=vx_ਲੱਦੋ(scbuf+VECSZ*3),sc4=vx_ਲੱਦੋ(scbuf+VECSZ*4),sc5=vx_ਲੱਦੋ(scbuf+VECSZ*5);
            v_uint8 M0=vx_ਲੱਦੋ(mbuf), M1=vx_ਲੱਦੋ(mbuf+VECSZ8);
            v_uint8 V0=vx_ਲੱਦੋ(vbuf), V1=vx_ਲੱਦੋ(vbuf+VECSZ8);
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EW_ROW4(A0,B0,A1,B1,A2,B2,A3,B3,A4,B4,A5,B5) \
                ਲਈ (; y + dy <= ਉਚਾਈ; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2,r3,r4,r5; \
                    EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    EW_CMPR(r3,A3,B3); EW_CMPR(r4,A4,B4); EW_CMPR(r5,A5,B5); \
                    v_uint16 p0=v_pack(r0,r1), p1=v_pack(r2,r3), p2=v_pack(r4,r5); \
                    v_uint8 q0=v_pack(p0,p1), q1=v_pack(p2,p2); \
                    v_uint8 o0=v_or(v_and(q0,M0),V0), o1=v_or(v_and(q1,M1),V1); \
                    v_store(dst, o0); v_store_ਨੀਵਾਂ(dst+VECSZ8, o1); }
            ਜੇ (bc0) EW_ROW4(sc0,vx_ਲੱਦੋ(src),sc1,vx_ਲੱਦੋ(src+VECSZ),sc2,vx_ਲੱਦੋ(src+VECSZ*2),
                             sc3,vx_ਲੱਦੋ(src+VECSZ*3),sc4,vx_ਲੱਦੋ(src+VECSZ*4),sc5,vx_ਲੱਦੋ(src+VECSZ*5))
            ਨਹੀਂਤਾਂ     EW_ROW4(vx_ਲੱਦੋ(src),sc0,vx_ਲੱਦੋ(src+VECSZ),sc1,vx_ਲੱਦੋ(src+VECSZ*2),sc2,
                             vx_ਲੱਦੋ(src+VECSZ*3),sc3,vx_ਲੱਦੋ(src+VECSZ*4),sc4,vx_ਲੱਦੋ(src+VECSZ*5),sc5)
            #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ EW_ROW4
        }
        #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ EW_CMPR
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; y < ਉਚਾਈ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ਪੂਰਨ x = 0;
    #ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        // SIMD for BOTH the contiguous case AND a broadcast operand (a per-channel scalar const has
        // step 0 - without this the scalar-vs-array compare, incl. every multi-channel scalar compare,
        // fell to the scalar tail below, ~5x slower). vx_setall broadcasts the step-0 operand once.
        // 4-vector unroll + halide right-edge backoff (reprocess the last 4*VECSZ when width is not a
        // multiple); dst is a separate mask buffer so the overlap is a harmless idempotent rewrite.
        ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Vvec>::vlanes();
        ਅਚਲ ਪੂਰਨ VECSZ8 = VTraits<v_uint8>::vlanes();
        v_uint8 vT; v_setall_ਨਕਾਬ(vT, trueVal);              // uniform mask (per-channel patch is short-row)
        ਅਚਲ ਤਾਰਕਿਕ ਪੂਛ_Trick = ਚੌੜਾਈ >= 4*VECSZ && src0_ != dst_ && src1_ != dst_;
        // Narrow the 4 masks to u8 FIRST, then apply trueVal on the u8 result (1 and per u8 vector, not
        // per wide lane-group). One block per MASK lane width (== sizeof(T) natively, but 4 for the
        // f16/bf16->f32 widened path); a 4-vector unroll spans exactly 4*VECSZ elements.
        constexpr size_t MW = ਆਕਾਰ(ਕਿਸਮ_ਨਾਂ VTraits<Uvec>::lane_type);
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PACK_STORE_CMP_ਨਤੀਜਾ(m0,m1,m2,m3, D) ਕਰੋ { \
            ਜੇ constexpr (MW == 1u) { \
                v_uint8 o0=v_and(m0,vT),o1=v_and(m1,vT),o2=v_and(m2,vT),o3=v_and(m3,vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); v_store((D)+VECSZ8*2,o2); v_store((D)+VECSZ8*3,o3); \
            } ਨਹੀਂਤਾਂ ਜੇ constexpr (MW == 2u) { \
                v_uint8 o0=v_and(v_pack(m0,m1),vT), o1=v_and(v_pack(m2,m3),vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); \
            } ਨਹੀਂਤਾਂ ਜੇ constexpr (MW == 4u) { \
                v_uint8 o0=v_and(v_pack(v_pack(m0,m1),v_pack(m2,m3)),vT); \
                v_store((D),o0); \
            } ਨਹੀਂਤਾਂ { /* sizeof==8: 4 u64 -> 2 u32 -> 1 u16 -> u8 (low half), 4*VECSZ elems -> store_low */ \
                v_uint16 g=v_pack(v_pack(m0,m1),v_pack(m2,m3)); \
                v_uint8 o0=v_and(v_pack(g,g),vT); \
                v_store_ਨੀਵਾਂ((D),o0); \
            } } ਜਦੋਂ(0)
        ਜੇ (s0x == 1u && s1x == 1u)
        {
            ਲਈ (; x < ਚੌੜਾਈ; x += 4*VECSZ)
            {
                ਜੇ (x + 4*VECSZ > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_Trick) ਰੋਕੋ; x = ਚੌੜਾਈ - 4*VECSZ; }
                Vvec a0, a1, a2, a3, b0, b1, b2, b3;   // vx_load_pair_as widens f16/bf16->f32, else native
                vx_ਲੱਦੋ_pair_as(src0+x, a0, a1); vx_ਲੱਦੋ_pair_as(src0+x+2*VECSZ, a2, a3);
                vx_ਲੱਦੋ_pair_as(src1+x, b0, b1); vx_ਲੱਦੋ_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b1), m1);
                v_reinterpret_as(Cmp::vec(a2, b2), m2); v_reinterpret_as(Cmp::vec(a3, b3), m3);
                PACK_STORE_CMP_ਨਤੀਜਾ(m0, m1, m2, m3, dst+x);
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (s1x == 0u)                       // src1 (e.g. the scalar) broadcast
        {
            Vvec b0; vx_setall_as(src1, b0);
            ਲਈ (; x < ਚੌੜਾਈ; x += 4*VECSZ)
            {
                ਜੇ (x + 4*VECSZ > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_Trick) ਰੋਕੋ; x = ਚੌੜਾਈ - 4*VECSZ; }
                Vvec a0, a1, a2, a3;
                vx_ਲੱਦੋ_pair_as(src0+x, a0, a1); vx_ਲੱਦੋ_pair_as(src0+x+2*VECSZ, a2, a3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b0), m1);
                v_reinterpret_as(Cmp::vec(a2, b0), m2); v_reinterpret_as(Cmp::vec(a3, b0), m3);
                PACK_STORE_CMP_ਨਤੀਜਾ(m0, m1, m2, m3, dst+x);
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (s0x == 0u)                       // src0 broadcast
        {
            Vvec a0; vx_setall_as(src0, a0);
            ਲਈ (; x < ਚੌੜਾਈ; x += 4*VECSZ)
            {
                ਜੇ (x + 4*VECSZ > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_Trick) ਰੋਕੋ; x = ਚੌੜਾਈ - 4*VECSZ; }
                Vvec b0, b1, b2, b3;
                vx_ਲੱਦੋ_pair_as(src1+x, b0, b1); vx_ਲੱਦੋ_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a0, b1), m1);
                v_reinterpret_as(Cmp::vec(a0, b2), m2); v_reinterpret_as(Cmp::vec(a0, b3), m3);
                PACK_STORE_CMP_ਨਤੀਜਾ(m0, m1, m2, m3, dst+x);
            }
        }
        #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PACK_STORE_CMP_ਨਤੀਜਾ
    #ਜੇ_ਅੰਤ
        // Fold the fix-up: raw all-ones/zero mask -> (raw & M) | V. Uniform => raw & trueVal (V=0); a
        // per-channel patch (width=cn<=4 here) indexes M/V by the channel x.
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                             : (uchar)((cond) ? trueVal : 0))
        ਜੇ (s0x == s1x) {
            ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], src1[x]), x);
        }
        ਨਹੀਂਤਾਂ ਜੇ (s0x == 0) {
            T a = src0[0];
            ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, src1[x]), x);
        }
        ਨਹੀਂਤਾਂ {
            T b = src1[0];
            ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], b), x);
        }
        #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ EW_CMP_APPLY
    }
    ਵਾਪਸ 0;
}

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Tr, ਕਿਸਮ_ਨਾਂ Wvec, ਕਿਸਮ_ਨਾਂ WT, ਵਰਗ Op, ਕਿਸਮ_ਨਾਂ ST=WT, ਕਿਸਮ_ਨਾਂ Wvec1=Wvec>
ਸਥਿਰ ਪੂਰਨ vec_ਦੁਆਧਾਰੀ_Kernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                         ਅਚਲ ਖਾਲੀ* src1_, size_t s1y, size_t s1x,
                         ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                         ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                         ਅਚਲ ਦੁੱਗਣਾ* params, ਪੂਰਨ, ਖਾਲੀ*)
{
    ਜੇ (ਚੌੜਾਈ <= 0 || ਉਚਾਈ <= 0) ਵਾਪਸ 0;
    s0y /= ਆਕਾਰ(T);
    s1y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(Tr);

    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ਚੌੜਾਈ == 1u);

    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* src1 = (ਅਚਲ T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    ਪੂਰਨ y = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਵਰਤੋਂ Wlane = ਕਿਸਮ_ਨਾਂ VTraits<Wvec>::lane_type;
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Wvec>::vlanes();
    ਅਚਲ ਤਾਰਕਿਕ use_ਪੂਛ_trick = ਚੌੜਾਈ >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    [[maybe_unused]] Wvec vscalar;
    vx_setall_as(&scalar, vscalar);
    ਜੇ (ਉਚਾਈ > 1 && ਚੌੜਾਈ <= 4 &&
        ((s0y == 0 && s1y == ਚੌੜਾਈ*s1x) ||
         (s1y == 0 && s0y == ਚੌੜਾਈ*s0x)) &&
        dsty == (size_t)ਚੌੜਾਈ) {
        constexpr ਪੂਰਨ MAXVECSZ = VTraits<Wvec>::max_nlanes;
        Wlane scbuf[MAXVECSZ*6] = {};   // {} for -Wmaybe-uninitialized only (filled up to VECSZ*6)
        ਅਚਲ ਪੂਰਨ ewidth = VECSZ*6;
        expandScalar(s0y == 0 ? src0 : src1, s0y == 0 ? s0x : s1x, ਚੌੜਾਈ, scbuf, ewidth);
        ਪੂਰਨ dy = ewidth / ਚੌੜਾਈ;
        Wvec sc0, sc1, sc2, sc3, sc4, sc5;
        sc0 = Op::preproc(vx_ਲੱਦੋ(scbuf), vscalar);
        sc1 = Op::preproc(vx_ਲੱਦੋ(scbuf + VECSZ), vscalar);
        sc2 = Op::preproc(vx_ਲੱਦੋ(scbuf + VECSZ*2), vscalar);
        sc3 = Op::preproc(vx_ਲੱਦੋ(scbuf + VECSZ*3), vscalar);
        sc4 = Op::preproc(vx_ਲੱਦੋ(scbuf + VECSZ*4), vscalar);
        sc5 = Op::preproc(vx_ਲੱਦੋ(scbuf + VECSZ*5), vscalar);

        ਜੇ (s0y == 0) {
            ਲਈ (; y + dy <= ਉਚਾਈ; y += dy, src1 += s1y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_ਲੱਦੋ_pair_as(src1, v0, v1);
                vx_ਲੱਦੋ_pair_as(src1 + VECSZ*2, v2, v3);
                vx_ਲੱਦੋ_pair_as(src1 + VECSZ*4, v4, v5);
                ਸਵੈਚਾਲਿਤ w0 = Op::vec(sc0, v0, vscalar), w1 = Op::vec(sc1, v1, vscalar),
                     w2 = Op::vec(sc2, v2, vscalar), w3 = Op::vec(sc3, v3, vscalar),
                     w4 = Op::vec(sc4, v4, vscalar), w5 = Op::vec(sc5, v5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
        ਨਹੀਂਤਾਂ {
            ਲਈ (; y + dy <= ਉਚਾਈ; y += dy, src0 += s0y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_ਲੱਦੋ_pair_as(src0, v0, v1);
                vx_ਲੱਦੋ_pair_as(src0 + VECSZ*2, v2, v3);
                vx_ਲੱਦੋ_pair_as(src0 + VECSZ*4, v4, v5);
                ਸਵੈਚਾਲਿਤ w0 = Op::vec(v0, sc0, vscalar), w1 = Op::vec(v1, sc1, vscalar),
                     w2 = Op::vec(v2, sc2, vscalar), w3 = Op::vec(v3, sc3, vscalar),
                     w4 = Op::vec(v4, sc4, vscalar), w5 = Op::vec(v5, sc5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; y < ਉਚਾਈ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ਪੂਰਨ x = 0;
    #ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        Wvec a0, a1, a2, a3, b0, b1, b2, b3;
        ਜੇ (s0x == s1x) {
            ਜੇ (!Op::useScalar || scalar == ST(1)) {
                // this branch runs on Wvec1, which may be WIDER-laned than Wvec (e.g. the u8 mul
                // path: Wvec1=v_uint16 is 2x the lanes of Wvec=v_float32) - step by ITS lane count,
                // or the pairs overlap and the tail backoff writes past the row end
                ਅਚਲ ਪੂਰਨ VECSZ1 = VTraits<Wvec1>::vlanes();
                ਅਚਲ ਤਾਰਕਿਕ ਪੂਛ_trick1 = ਚੌੜਾਈ >= VECSZ1*4 && src0_ != dst_ && src1_ != dst_;
                ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ1*4) {
                    Wvec1 a0_, a1_, a2_, a3_, b0_, b1_, b2_, b3_;
                    ਜੇ (x + VECSZ1*4 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_trick1) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ1*4; }
                    vx_ਲੱਦੋ_pair_as(src0 + x, a0_, a1_);
                    vx_ਲੱਦੋ_pair_as(src0 + x + VECSZ1*2, a2_, a3_);
                    vx_ਲੱਦੋ_pair_as(src1 + x, b0_, b1_);
                    vx_ਲੱਦੋ_pair_as(src1 + x + VECSZ1*2, b2_, b3_);
                    ਸਵੈਚਾਲਿਤ c0 = Op::vec(a0_, b0_), c1 = Op::vec(a1_, b1_),
                         c2 = Op::vec(a2_, b2_), c3 = Op::vec(a3_, b3_);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ1*2, c2, c3);
                }
            }
            ਨਹੀਂਤਾਂ {
                ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*4) {
                    ਜੇ (x + VECSZ*4 > ਚੌੜਾਈ) { ਜੇ (!use_ਪੂਛ_trick) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*4; }
                    vx_ਲੱਦੋ_pair_as(src0 + x, a0, a1);
                    vx_ਲੱਦੋ_pair_as(src0 + x + VECSZ*2, a2, a3);
                    vx_ਲੱਦੋ_pair_as(src1 + x, b0, b1);
                    vx_ਲੱਦੋ_pair_as(src1 + x + VECSZ*2, b2, b3);
                    ਸਵੈਚਾਲਿਤ c0 = Op::vec(Op::preproc(a0, vscalar), b0, vscalar),
                         c1 = Op::vec(Op::preproc(a1, vscalar), b1, vscalar),
                         c2 = Op::vec(Op::preproc(a2, vscalar), b2, vscalar),
                         c3 = Op::vec(Op::preproc(a3, vscalar), b3, vscalar);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ*2, c2, c3);
                }
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (s1x == 0) {
            vx_setall_as(src1, b0);
            b0 = Op::preproc(b0, vscalar);
            ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*4) {
                ਜੇ (x + VECSZ*4 > ਚੌੜਾਈ) { ਜੇ (!use_ਪੂਛ_trick) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*4; }
                vx_ਲੱਦੋ_pair_as(src0 + x, a0, a1);
                vx_ਲੱਦੋ_pair_as(src0 + x + VECSZ*2, a2, a3);
                ਸਵੈਚਾਲਿਤ c0 = Op::vec(a0, b0, vscalar), c1 = Op::vec(a1, b0, vscalar),
                     c2 = Op::vec(a2, b0, vscalar), c3 = Op::vec(a3, b0, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
        ਨਹੀਂਤਾਂ {
            vx_setall_as(src0, b0);
            b0 = Op::preproc(b0, vscalar);
            ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*4) {
                ਜੇ (x + VECSZ*4 > ਚੌੜਾਈ) { ਜੇ (!use_ਪੂਛ_trick) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*4; }
                vx_ਲੱਦੋ_pair_as(src1 + x, a0, a1);
                vx_ਲੱਦੋ_pair_as(src1 + x + VECSZ*2, a2, a3);
                ਸਵੈਚਾਲਿਤ c0 = Op::vec(b0, a0, vscalar), c1 = Op::vec(b0, a1, vscalar),
                     c2 = Op::vec(b0, a2, vscalar), c3 = Op::vec(b0, a3, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
    #ਜੇ_ਅੰਤ
        ਲਈ (; x < ਚੌੜਾਈ; x++)
            dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x*s0x], (WT)src1[x*s1x], scalar));
    }
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    vx_cleanup();
#ਜੇ_ਅੰਤ
    ਵਾਪਸ 0;
}

// ===========================================================================
// OP_ADD
// ===========================================================================

// add dispatch: T x T -> R (operands already same depth T).
//  - native saturating path (Wvec = native, use_simd=true) for T -> T on <=32-bit ints + f32
//    (32-bit lanes via the local v_add_sat/v_sub_sat - v_add/v_sub wrap there);
//  - widening f32-hub (Wvec = v_float32, use_simd=true) for the widened/half/float outputs;
//  - scalar (use_simd=false) for 64-bit results and f64 (no vector path yet).
ਸਾਂਚਾ<ਵਰਗ Op>
TKernel get_ਜੋੜੋ_Sub_Func(ਪੂਰਨ T, ਪੂਰਨ R)
{
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_8U:
        fptr = R == CV_8U ? vec_ਦੁਆਧਾਰੀ_Kernel<uchar, uchar, v_uint8, ਛੋਟਾ, Op, uchar> :
               R == CV_16S ? vec_ਦੁਆਧਾਰੀ_Kernel<uchar, ਛੋਟਾ, v_int16, ਛੋਟਾ, Op, ਛੋਟਾ> :
               R == CV_32S ? vec_ਦੁਆਧਾਰੀ_Kernel<uchar, ਪੂਰਨ, v_int16, ਛੋਟਾ, Op, ਛੋਟਾ> :
               R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<uchar, ਦਸ਼ਮਲਵ, v_int16, ਛੋਟਾ, Op, ਛੋਟਾ> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_8S:
        fptr =
            R == CV_8S ? vec_ਦੁਆਧਾਰੀ_Kernel<schar, schar, v_int8, ਛੋਟਾ, Op, schar> :
            R == CV_16S ? vec_ਦੁਆਧਾਰੀ_Kernel<schar, ਛੋਟਾ, v_int16, ਛੋਟਾ, Op, ਛੋਟਾ> :
            R == CV_32S ? vec_ਦੁਆਧਾਰੀ_Kernel<schar, ਪੂਰਨ, v_int16, ਛੋਟਾ, Op, ਛੋਟਾ> :
            R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<schar, ਦਸ਼ਮਲਵ, v_int16, ਛੋਟਾ, Op, ਛੋਟਾ> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_16U:
        fptr = R == CV_16U ? vec_ਦੁਆਧਾਰੀ_Kernel<ushort, ushort, v_uint16, ਪੂਰਨ, Op, ushort> :
               R == CV_32S ? vec_ਦੁਆਧਾਰੀ_Kernel<ushort, ਪੂਰਨ, v_int32, ਪੂਰਨ, Op, ਪੂਰਨ> :
               R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<ushort, ਦਸ਼ਮਲਵ, v_int32, ਪੂਰਨ, Op, ਪੂਰਨ> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_16S:
        fptr = R == CV_16S ? vec_ਦੁਆਧਾਰੀ_Kernel<ਛੋਟਾ, ਛੋਟਾ, v_int16, ਪੂਰਨ, Op, ਛੋਟਾ> :
               R == CV_32S ? vec_ਦੁਆਧਾਰੀ_Kernel<ਛੋਟਾ, ਪੂਰਨ,   v_int32, ਪੂਰਨ, Op, ਪੂਰਨ> :
               R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਛੋਟਾ, ਦਸ਼ਮਲਵ, v_int32, ਪੂਰਨ, Op, ਪੂਰਨ> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_32U:
        fptr = R == CV_32U ? vec_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32, int64_t, Op, ਬਿਨਾਨਿਸ਼ਾਨੀ> :
                R == CV_64S ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, int64_t, int64_t, Op> :
                R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, int64_t, Op> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_32S:
        fptr = R == CV_32S ? vec_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਪੂਰਨ, v_int32, int64_t, Op, ਪੂਰਨ> :
               R == CV_64S ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਦੁੱਗਣਾ, int64_t, Op> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_64U:
        fptr = R == CV_64U ? scalar_ਦੁਆਧਾਰੀ_Kernel<uint64_t, uint64_t, uint64_t, Op> :
               R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<uint64_t, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, Op> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_64S:
        fptr = R == CV_64S ? scalar_ਦੁਆਧਾਰੀ_Kernel<int64_t, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<int64_t, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, Op> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_16F:
        fptr =
        #ਜੇ CV_SIMD_16F
        R == CV_16F ? vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, hfloat, v_float16, ਦਸ਼ਮਲਵ, Op, hfloat> :
        #ਨਹੀਂਤਾਂ
        R == CV_16F ? vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, hfloat, v_float32, ਦਸ਼ਮਲਵ, Op, ਦਸ਼ਮਲਵ> :
        #ਜੇ_ਅੰਤ
        R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ, Op, ਦਸ਼ਮਲਵ> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_16BF:
        fptr =
            R == CV_16BF ? vec_ਦੁਆਧਾਰੀ_Kernel<bfloat, bfloat, v_float32, ਦਸ਼ਮਲਵ, Op, ਦਸ਼ਮਲਵ> :
            R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<bfloat, ਦਸ਼ਮਲਵ,  v_float32, ਦਸ਼ਮਲਵ, Op, ਦਸ਼ਮਲਵ> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_32F:
        fptr = R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ, Op, ਦਸ਼ਮਲਵ> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_64F:
        #ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, v_float64, ਦੁੱਗਣਾ, Op> : ਖਾਲੀ_ਸੂਚਕ;
        #ਨਹੀਂਤਾਂ
        fptr = R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, Op> : ਖਾਲੀ_ਸੂਚਕ;
        #ਜੇ_ਅੰਤ
        ਰੋਕੋ;
    ਮੂਲ:
        ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}

TKernel getMulFunc_(ਪੂਰਨ T, ਪੂਰਨ R)
{
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_8U:
        fptr =   // scale==1 fast path: whole u8 registers, v_mul_sat widens+clamps inside; Wvec=f32 for scale
            R == CV_8U ? vec_ਦੁਆਧਾਰੀ_Kernel<uchar, uchar, v_float32, ਦਸ਼ਮਲਵ, EwMul, ਦਸ਼ਮਲਵ, v_uint8> :
            R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<uchar, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_8S:
        fptr =   // scale==1 fast path: whole s8 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_8S ? vec_ਦੁਆਧਾਰੀ_Kernel<schar, schar, v_float32, ਦਸ਼ਮਲਵ, EwMul, ਦਸ਼ਮਲਵ, v_int8> :
            R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<schar, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_16U:
        fptr =   // scale==1 fast path: whole u16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16U ? vec_ਦੁਆਧਾਰੀ_Kernel<ushort, ushort, v_float32, ਦਸ਼ਮਲਵ, EwMul, ਦਸ਼ਮਲਵ, v_uint16> :
            R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<ushort, ਦਸ਼ਮਲਵ,  v_float32, ਦਸ਼ਮਲਵ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_16S:
        fptr =   // scale==1 fast path: whole s16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16S ? vec_ਦੁਆਧਾਰੀ_Kernel<ਛੋਟਾ, ਛੋਟਾ, v_float32, ਦਸ਼ਮਲਵ, EwMul, ਦਸ਼ਮਲਵ, v_int16> :
            R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਛੋਟਾ, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_16F:
        fptr =
            R == CV_16F ? vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, hfloat, v_float32, ਦਸ਼ਮਲਵ, EwMul> :
            R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, ਦਸ਼ਮਲਵ,  v_float32, ਦਸ਼ਮਲਵ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_16BF:
        fptr =
            R == CV_16BF ? vec_ਦੁਆਧਾਰੀ_Kernel<bfloat, bfloat, v_float32, ਦਸ਼ਮਲਵ, EwMul> :
            R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<bfloat, ਦਸ਼ਮਲਵ,  v_float32, ਦਸ਼ਮਲਵ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_32F:
        fptr = R == CV_32F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_32U:
        fptr =   // scale==1 fast path (where v_mul_sat32 exists): whole u32 registers, widening
                 // multiply + saturating narrow; the f64 work vector serves the scale path
        #ਜੇ defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32U ? vec_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float64, ਦੁੱਗਣਾ, EwMul, ਦੁੱਗਣਾ, v_uint32> :
            R == CV_64F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ,   v_float64, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        #ਜਾਂਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32U ? vec_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_float64, ਦੁੱਗਣਾ, EwMul> :
            R == CV_64F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ,   v_float64, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        #ਨਹੀਂਤਾਂ
            R == CV_32U ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ, EwMul> :
            R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ,   ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        #ਜੇ_ਅੰਤ
        ਰੋਕੋ;
    ਹਾਲਤ CV_32S:
        fptr =   // scale==1 fast path: see CV_32U
        #ਜੇ defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32S ? vec_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਪੂਰਨ,    v_float64, ਦੁੱਗਣਾ, EwMul, ਦੁੱਗਣਾ, v_int32> :
            R == CV_64F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਦੁੱਗਣਾ, v_float64, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        #ਜਾਂਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32S ? vec_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਪੂਰਨ,    v_float64, ਦੁੱਗਣਾ, EwMul> :
            R == CV_64F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਦੁੱਗਣਾ, v_float64, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        #ਨਹੀਂਤਾਂ
            R == CV_32S ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਪੂਰਨ,    ਦੁੱਗਣਾ, EwMul> :
            R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        #ਜੇ_ਅੰਤ
        ਰੋਕੋ;
    ਹਾਲਤ CV_64U:
        fptr = R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<uint64_t, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_64S:
        fptr = R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<int64_t,  ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        ਰੋਕੋ;
    ਹਾਲਤ CV_64F:
        #ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, v_float64, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        #ਨਹੀਂਤਾਂ
        fptr = R == CV_64F ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ,   ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, EwMul> : ਖਾਲੀ_ਸੂਚਕ;
        #ਜੇ_ਅੰਤ
        ਰੋਕੋ;
    ਮੂਲ:
        ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}

// `checked` (decided by the CALLER from the ORIGINAL input types) selects the divide-by-zero
// policy, INDEPENDENT of the work type R: EwDivInt guards /0 -> 0 for integer-semantics division,
// EwDivFlt does not (a/0 -> inf, saturating on a later cast, as cv::divide does for float inputs).
// It must apply on EVERY row - two wide integers (e.g. 32U / 64S) promote to a 64F work type yet
// still need the integer guard, so the float-work rows can't hardcode EwDivFlt.
TKernel getDivFunc_(ਪੂਰਨ T, ਪੂਰਨ R, ਤਾਰਕਿਕ checked)
{
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    // SIMD via vecBinaryKernel: div scales the numerator in vec (a*scale/b), EwDivFlt (float a/0 -> inf,
    // saturates on cast) / EwDivInt (v_select guards b==0 -> 0 post-facto). Prefer a DIRECT T->T kernel
    // (1 pass, saturate on store) - emitBinary probes result==T first; a T->f32/f64 kernel serves an
    // explicit float dtype. Work type: f32 for <=16-bit/f16/bf16/f32; f64 for 32/64-bit.
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIV(T_, Tr_, Wv_, W_) (checked ? vec_ਦੁਆਧਾਰੀ_Kernel<T_, Tr_, Wv_, W_, EwDivInt> \
                                           : vec_ਦੁਆਧਾਰੀ_Kernel<T_, Tr_, Wv_, W_, EwDivFlt>)
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIVW(T_, Tr_) DIV(T_, Tr_, v_float64, ਦੁੱਗਣਾ)                 // 32/64-bit, f64 SIMD
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIVW(T_, Tr_) (checked ? scalar_ਦੁਆਧਾਰੀ_Kernel<T_, Tr_, ਦੁੱਗਣਾ, EwDivInt> \
                                    : scalar_ਦੁਆਧਾਰੀ_Kernel<T_, Tr_, ਦੁੱਗਣਾ, EwDivFlt>)
#ਜੇ_ਅੰਤ
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_8U:   fptr = R==CV_8U  ? DIV(uchar,  uchar,  v_float32, ਦਸ਼ਮਲਵ) : R==CV_32F ? DIV(uchar,  ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_8S:   fptr = R==CV_8S  ? DIV(schar,  schar,  v_float32, ਦਸ਼ਮਲਵ) : R==CV_32F ? DIV(schar,  ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16U:  fptr = R==CV_16U ? DIV(ushort, ushort, v_float32, ਦਸ਼ਮਲਵ) : R==CV_32F ? DIV(ushort, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16S:  fptr = R==CV_16S ? DIV(ਛੋਟਾ,  ਛੋਟਾ,  v_float32, ਦਸ਼ਮਲਵ) : R==CV_32F ? DIV(ਛੋਟਾ,  ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16F:  fptr = R==CV_16F ? DIV(hfloat, hfloat, v_float32, ਦਸ਼ਮਲਵ) : R==CV_32F ? DIV(hfloat, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16BF: fptr = R==CV_16BF? DIV(bfloat, bfloat, v_float32, ਦਸ਼ਮਲਵ) : R==CV_32F ? DIV(bfloat, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32F:  fptr = R==CV_32F ? DIV(ਦਸ਼ਮਲਵ,  ਦਸ਼ਮਲਵ,  v_float32, ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32U:  fptr = R==CV_32U ? DIVW(ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ) : R==CV_64F ? DIVW(ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32S:  fptr = R==CV_32S ? DIVW(ਪੂਰਨ,      ਪੂਰਨ)      : R==CV_64F ? DIVW(ਪੂਰਨ,      ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_64U:  fptr = R==CV_64F ? DIVW(uint64_t, ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_64S:  fptr = R==CV_64F ? DIVW(int64_t,  ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_64F:  fptr = R==CV_64F ? DIVW(ਦੁੱਗਣਾ,   ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਮੂਲ:      ;
    }
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DIV
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ DIVW
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}


// min / max: T x T -> T for every depth. Native v_min/v_max on the matching lane type (8/16/32-bit
// ints, f16/bf16/f32); 64-bit ints use the scalar path. Op = EwMin or EwMax.
ਸਾਂਚਾ<ਵਰਗ Op>
ਸਥਿਰ TKernel getMinMaxFunc(ਪੂਰਨ T)
{
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_8U:   fptr = vec_ਦੁਆਧਾਰੀ_Kernel<uchar,    uchar,    v_uint8,   ਛੋਟਾ,   Op, uchar>; ਰੋਕੋ;
    ਹਾਲਤ CV_8S:   fptr = vec_ਦੁਆਧਾਰੀ_Kernel<schar,    schar,    v_int8,    ਛੋਟਾ,   Op, schar>; ਰੋਕੋ;
    ਹਾਲਤ CV_16U:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ushort,   ushort,   v_uint16,  ਪੂਰਨ,     Op, ushort>; ਰੋਕੋ;
    ਹਾਲਤ CV_16S:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ਛੋਟਾ,    ਛੋਟਾ,    v_int16,   ਪੂਰਨ,     Op, ਛੋਟਾ>; ਰੋਕੋ;
    ਹਾਲਤ CV_32U:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32,  int64_t, Op, ਬਿਨਾਨਿਸ਼ਾਨੀ>; ਰੋਕੋ;
    ਹਾਲਤ CV_32S:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ,      ਪੂਰਨ,      v_int32,   int64_t, Op, ਪੂਰਨ>; ਰੋਕੋ;
    ਹਾਲਤ CV_16F:
        #ਜੇ CV_SIMD_16F
        fptr = vec_ਦੁਆਧਾਰੀ_Kernel<hfloat,   hfloat,   v_float16, ਦਸ਼ਮਲਵ,   Op, hfloat>;
        #ਨਹੀਂਤਾਂ
        fptr = vec_ਦੁਆਧਾਰੀ_Kernel<hfloat,   hfloat,   v_float32, ਦਸ਼ਮਲਵ,   Op, ਦਸ਼ਮਲਵ>;
        #ਜੇ_ਅੰਤ
        ਰੋਕੋ;
    ਹਾਲਤ CV_16BF: fptr = vec_ਦੁਆਧਾਰੀ_Kernel<bfloat, bfloat, v_float32, ਦਸ਼ਮਲਵ, Op, ਦਸ਼ਮਲਵ>; ਰੋਕੋ;
    ਹਾਲਤ CV_32F:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, v_float32, ਦਸ਼ਮਲਵ, Op, ਦਸ਼ਮਲਵ>; ਰੋਕੋ;
    ਹਾਲਤ CV_64U:  fptr = scalar_ਦੁਆਧਾਰੀ_Kernel<uint64_t, uint64_t, uint64_t, Op>; ਰੋਕੋ;
    ਹਾਲਤ CV_64S:  fptr = scalar_ਦੁਆਧਾਰੀ_Kernel<int64_t,  int64_t,  int64_t,  Op>; ਰੋਕੋ;
    #ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਹਾਲਤ CV_64F:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, v_float64, ਦੁੱਗਣਾ, Op, ਦੁੱਗਣਾ>; ਰੋਕੋ;
    #ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_64F:  fptr = scalar_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ,   ਦੁੱਗਣਾ,   ਦੁੱਗਣਾ,   Op>; ਰੋਕੋ;
    #ਜੇ_ਅੰਤ
    ਮੂਲ:      ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}

// absdiff: |a-b|, T x T -> R.
TKernel getAbsdiffFunc_(ਪੂਰਨ T, ਪੂਰਨ R)
{
    // Signed T has TWO outputs: R==T -> EwAbsdiffS (saturating |a-b| kept in the signed range, native work,
    // ONE pass - the depth cv::absdiff keeps); R==(the unsigned type of the same width) -> EwAbsdiff (the
    // true |a-b| via v_absdiff, for an explicit unsigned dst). Unsigned/float T only produce R==T. Each case
    // returns nullptr for a wrong R (no separate rdepth guard).
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_8U:   fptr = R == CV_8U  ? vec_ਦੁਆਧਾਰੀ_Kernel<uchar,  uchar,  v_uint8,  ਛੋਟਾ,   EwAbsdiff, uchar>  : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16U:  fptr = R == CV_16U ? vec_ਦੁਆਧਾਰੀ_Kernel<ushort, ushort, v_uint16, ਪੂਰਨ,     EwAbsdiff, ushort> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32U:  fptr = R == CV_32U ? vec_ਦੁਆਧਾਰੀ_Kernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32, int64_t, EwAbsdiff, ਬਿਨਾਨਿਸ਼ਾਨੀ> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_8S:   fptr = R == CV_8S  ? vec_ਦੁਆਧਾਰੀ_Kernel<schar, schar, v_int8,  ਛੋਟਾ,   EwAbsdiffS, schar> :
                         R == CV_8U  ? vec_ਦੁਆਧਾਰੀ_Kernel<schar, uchar, v_int8,  ਛੋਟਾ,   EwAbsdiff,  schar> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16S:  fptr = R == CV_16S ? vec_ਦੁਆਧਾਰੀ_Kernel<ਛੋਟਾ, ਛੋਟਾ, v_int16, ਪੂਰਨ,     EwAbsdiffS, ਛੋਟਾ> :
                         R == CV_16U ? vec_ਦੁਆਧਾਰੀ_Kernel<ਛੋਟਾ, ushort, v_int16, ਪੂਰਨ,     EwAbsdiff,  ਛੋਟਾ> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32S:  fptr = R == CV_32S ? vec_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ,   ਪੂਰਨ,   v_int32, int64_t, EwAbsdiffS, ਪੂਰਨ> :
                         R == CV_32U ? vec_ਦੁਆਧਾਰੀ_Kernel<ਪੂਰਨ, ਬਿਨਾਨਿਸ਼ਾਨੀ, v_int32, int64_t, EwAbsdiff,  ਪੂਰਨ> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    #ਜੇ CV_SIMD_16F
    ਹਾਲਤ CV_16F:  fptr = R == CV_16F ? vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, hfloat, v_float16, ਦਸ਼ਮਲਵ, EwAbsdiff, hfloat> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    #ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_16F:  fptr = R == CV_16F ? vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, hfloat, v_float32, ਦਸ਼ਮਲਵ, EwAbsdiff, ਦਸ਼ਮਲਵ>  : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    #ਜੇ_ਅੰਤ
    ਹਾਲਤ CV_16BF: fptr = R == CV_16BF ? vec_ਦੁਆਧਾਰੀ_Kernel<bfloat, bfloat, v_float32, ਦਸ਼ਮਲਵ, EwAbsdiff, ਦਸ਼ਮਲਵ> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32F:  fptr = R == CV_32F  ? vec_ਦੁਆਧਾਰੀ_Kernel<ਦਸ਼ਮਲਵ,  ਦਸ਼ਮਲਵ,  v_float32, ਦਸ਼ਮਲਵ, EwAbsdiff, ਦਸ਼ਮਲਵ> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_64U:  fptr = R == CV_64U  ? scalar_ਦੁਆਧਾਰੀ_Kernel<uint64_t, uint64_t, uint64_t, EwAbsdiff> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_64S:  fptr = R == CV_64S  ? scalar_ਦੁਆਧਾਰੀ_Kernel<int64_t,  int64_t,  int64_t,  EwAbsdiffS> :
                         R == CV_64U  ? scalar_ਦੁਆਧਾਰੀ_Kernel<int64_t,  uint64_t, int64_t,  EwAbsdiff> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    #ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਹਾਲਤ CV_64F:  fptr = R == CV_64F  ? vec_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, v_float64, ਦੁੱਗਣਾ, EwAbsdiff, ਦੁੱਗਣਾ> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    #ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_64F:  fptr = R == CV_64F  ? scalar_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, EwAbsdiff> : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    #ਜੇ_ਅੰਤ
    ਮੂਲ:      ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}

// compare: T x T -> u8 mask. Directly-comparable depths (u8/s8/u16/s16/u32/s32/f32) take the SIMD
// vecCompareKernel; f16/bf16 and 64-bit int depths fall back to scalarCompareKernel. The returned kernel
// defaults to a 255 mask in TKernel::flags (cv::compare-compatible); kernel.flags=1 gives a 0/1 mask.
ਸਾਂਚਾ<ਵਰਗ Cmp>
ਸਥਿਰ KernelFunc compareByType(ਪੂਰਨ T)
{
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_8U:   ਵਾਪਸ vecCompareKernel<uchar,    v_uint8,   v_uint8,  Cmp>;
    ਹਾਲਤ CV_8S:   ਵਾਪਸ vecCompareKernel<schar,    v_int8,    v_uint8,  Cmp>;
    ਹਾਲਤ CV_16U:  ਵਾਪਸ vecCompareKernel<ushort,   v_uint16,  v_uint16, Cmp>;
    ਹਾਲਤ CV_16S:  ਵਾਪਸ vecCompareKernel<ਛੋਟਾ,    v_int16,   v_uint16, Cmp>;
    ਹਾਲਤ CV_32U:  ਵਾਪਸ vecCompareKernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32,  v_uint32, Cmp>;
    ਹਾਲਤ CV_32S:  ਵਾਪਸ vecCompareKernel<ਪੂਰਨ,      v_int32,   v_uint32, Cmp>;
    ਹਾਲਤ CV_32F:  ਵਾਪਸ vecCompareKernel<ਦਸ਼ਮਲਵ,    v_float32, v_uint32, Cmp>;
    #ਜੇ CV_SIMD_16F
    ਹਾਲਤ CV_16F:  ਵਾਪਸ vecCompareKernel<hfloat,   v_float16, v_uint16, Cmp>;
    #ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_16F:  ਵਾਪਸ vecCompareKernel<hfloat,   v_float32, v_uint32, Cmp>;   // widen f16->f32
    #ਜੇ_ਅੰਤ
    ਹਾਲਤ CV_16BF: ਵਾਪਸ vecCompareKernel<bfloat,   v_float32, v_uint32, Cmp>;   // widen bf16->f32 (no native)
    ਹਾਲਤ CV_64U:  ਵਾਪਸ scalarCompareKernel<uint64_t, uint64_t, Cmp>;
    ਹਾਲਤ CV_64S:  ਵਾਪਸ scalarCompareKernel<int64_t,  int64_t,  Cmp>;
    #ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਹਾਲਤ CV_64F:  ਵਾਪਸ vecCompareKernel<ਦੁੱਗਣਾ,   v_float64, v_uint64, Cmp>;
    #ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_64F:  ਵਾਪਸ scalarCompareKernel<ਦੁੱਗਣਾ,   ਦੁੱਗਣਾ,   Cmp>;
    #ਜੇ_ਅੰਤ
    ਮੂਲ:      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
}

TKernel getCmpFunc_(ਸਿਖਰ op, ਪੂਰਨ T)
{
    // Only 4 physical kernels (eq/ne/gt/ge): LT/LE reuse GT/GE with the operands swapped
    // (a<b == b>a, a<=b == b>=a) via the EW_KERNEL_SWAP01 flag, honored by the executor.
    KernelFunc f = ਖਾਲੀ_ਸੂਚਕ;
    ਪੂਰਨ flags = 0;              // mask value: 0 flag bits => 0/255 (cv::compare); EW_KERNEL_MASK1 => 0/1
    ਚੋਣ (op)
    {
    ਹਾਲਤ OP_CMP_EQ: f = compareByType<EwCmpEq>(T); ਰੋਕੋ;
    ਹਾਲਤ OP_CMP_NE: f = compareByType<EwCmpNe>(T); ਰੋਕੋ;
    ਹਾਲਤ OP_CMP_GT: f = compareByType<EwCmpGt>(T); ਰੋਕੋ;
    ਹਾਲਤ OP_CMP_GE: f = compareByType<EwCmpGe>(T); ਰੋਕੋ;
    ਹਾਲਤ OP_CMP_LT: f = compareByType<EwCmpGt>(T); flags = EW_KERNEL_SWAP01; ਰੋਕੋ;
    ਹਾਲਤ OP_CMP_LE: f = compareByType<EwCmpGe>(T); flags = EW_KERNEL_SWAP01; ਰੋਕੋ;
    ਮੂਲ:        ;
    }
    ਵਾਪਸ {f, ਖਾਲੀ_ਸੂਚਕ, flags};
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
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Vvec>
ਸਥਿਰ ਪੂਰਨ notKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                     ਅਚਲ ਖਾਲੀ*, size_t, size_t, ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                     ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                     ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ*)
{
    ਜੇ (ਚੌੜਾਈ <= 0 || ਉਚਾਈ <= 0) ਵਾਪਸ 0;
    s0y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(T);
    CV_Assert(s0x == 1u || ਚੌੜਾਈ == 1);
    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    T* dst = (T*)dst_;
    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && s0y == (size_t)ਚੌੜਾਈ) { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }
    ਲਈ (ਪੂਰਨ y = 0; y < ਉਚਾਈ; y++, src0 += s0y, dst += dsty)
    {
        ਪੂਰਨ x = 0;
    #ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਜੇ constexpr (ਆਕਾਰ(T) <= 4)
        {
            ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Vvec>::vlanes();
            ਲਈ (; x <= ਚੌੜਾਈ - VECSZ; x += VECSZ)
                v_store(dst + x, v_not(vx_ਲੱਦੋ(src0 + x)));
        }
    #ਜੇ_ਅੰਤ
        ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = (T)~src0[x];
    }
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ<ਵਰਗ Op>
ਸਥਿਰ KernelFunc bitwiseByEsz(ਪੂਰਨ esz)
{
    ਚੋਣ (esz)
    {
    ਹਾਲਤ 1: ਵਾਪਸ vec_ਦੁਆਧਾਰੀ_Kernel<uint8_t,  uint8_t,  v_uint8,  uint8_t,  Op, uint8_t>;
    ਹਾਲਤ 2: ਵਾਪਸ vec_ਦੁਆਧਾਰੀ_Kernel<uint16_t, uint16_t, v_uint16, uint16_t, Op, uint16_t>;
    ਹਾਲਤ 4: ਵਾਪਸ vec_ਦੁਆਧਾਰੀ_Kernel<uint32_t, uint32_t, v_uint32, uint32_t, Op, uint32_t>;
    ਹਾਲਤ 8: ਵਾਪਸ scalar_ਦੁਆਧਾਰੀ_Kernel<uint64_t, uint64_t, uint64_t, Op>;
    ਮੂਲ: ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
}

TKernel getBitwiseFunc_(ਸਿਖਰ op, ਪੂਰਨ esz)
{
    KernelFunc f = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (op)
    {
    ਹਾਲਤ OP_AND: f = bitwiseByEsz<EwAnd>(esz); ਰੋਕੋ;
    ਹਾਲਤ OP_OR:  f = bitwiseByEsz<EwOr >(esz); ਰੋਕੋ;
    ਹਾਲਤ OP_XOR: f = bitwiseByEsz<EwXor>(esz); ਰੋਕੋ;
    ਮੂਲ:     ;
    }
    ਵਾਪਸ {f, ਖਾਲੀ_ਸੂਚਕ, 0};
}

TKernel getNotFunc_(ਪੂਰਨ esz)
{
    KernelFunc f = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (esz)
    {
    ਹਾਲਤ 1: f = notKernel<uint8_t,  v_uint8 >; ਰੋਕੋ;
    ਹਾਲਤ 2: f = notKernel<uint16_t, v_uint16>; ਰੋਕੋ;
    ਹਾਲਤ 4: f = notKernel<uint32_t, v_uint32>; ਰੋਕੋ;
    ਹਾਲਤ 8: f = notKernel<uint64_t, v_uint32>; ਰੋਕੋ;   // SIMD path compiled out for 8-byte -> scalar ~
    ਮੂਲ: ;
    }
    ਵਾਪਸ {f, ਖਾਲੀ_ਸੂਚਕ, 0};
}

// ===========================================================================
// OP_ADDW (addWeighted): dst = a*alpha + b*beta + gamma, params[0..2] = {alpha, beta, gamma}. Two fused
// v_fma in the work type Wvec - f32 SIMD for u8/s8/u16/s16/f16/bf16/f32; the 32-bit-int/64-bit group
// works in f64 (v_float64 SIMD under CV_SIMD_64F || CV_SIMD_SCALABLE_64F, else use_simd=false scalar). Like vecBinaryKernel but
// WITHOUT its multi-channel short-row
// branch: addWeighted takes plain scalar coefficients (a multi-channel scalar is not optimized, matching
// the classic function). The broadcast branches fold the constant operand's contribution once.
// ===========================================================================
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Tr, ਕਿਸਮ_ਨਾਂ Wvec, ਕਿਸਮ_ਨਾਂ WT, ਤਾਰਕਿਕ use_simd>
ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ_Weighted_Kernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                             ਅਚਲ ਖਾਲੀ* src1_, size_t s1y, size_t s1x,
                             ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                             ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                             ਅਚਲ ਦੁੱਗਣਾ* params, ਪੂਰਨ, ਖਾਲੀ*)
{
    ਜੇ (ਚੌੜਾਈ <= 0 || ਉਚਾਈ <= 0) ਵਾਪਸ 0;
    s0y /= ਆਕਾਰ(T); s1y /= ਆਕਾਰ(T); dsty /= ਆਕਾਰ(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)ਚੌੜਾਈ == 1u);
    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* src1 = (ਅਚਲ T*)src1_;
    Tr* dst = (Tr*)dst_;
    ਅਚਲ WT alpha = (WT)params[0], beta = (WT)params[1], gamma = (WT)params[2];
    EW_TRY_COLLAPSE(2);
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    Wvec va{}, vb{}, vg{};
    ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Wvec>::vlanes();
    ਅਚਲ ਤਾਰਕਿਕ ਪੂਛ = ਚੌੜਾਈ >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    ਜੇ constexpr (use_simd) {
        WT fa=alpha, fb=beta, fg=gamma;
        vx_setall_as(&fa, va); vx_setall_as(&fb, vb); vx_setall_as(&fg, vg);
    }
#ਜੇ_ਅੰਤ
    ਲਈ (ਪੂਰਨ y = 0; y < ਉਚਾਈ; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ਪੂਰਨ x = 0;
    #ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਜੇ constexpr (use_simd)
        {
            Wvec a0,a1,a2,a3,b0,b1,b2,b3;
            ਜੇ (s0x == s1x) {                                // both arrays contiguous
                ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*4) {
                    ਜੇ (x+VECSZ*4 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ) ਰੋਕੋ; x = ਚੌੜਾਈ-VECSZ*4; }
                    vx_ਲੱਦੋ_pair_as(src0+x, a0, a1); vx_ਲੱਦੋ_pair_as(src0+x+VECSZ*2, a2, a3);
                    vx_ਲੱਦੋ_pair_as(src1+x, b0, b1); vx_ਲੱਦੋ_pair_as(src1+x+VECSZ*2, b2, b3);
                    a0=v_fma(a0,va,v_fma(b0,vb,vg)); a1=v_fma(a1,va,v_fma(b1,vb,vg));
                    a2=v_fma(a2,va,v_fma(b2,vb,vg)); a3=v_fma(a3,va,v_fma(b3,vb,vg));
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            ਨਹੀਂਤਾਂ ਜੇ (s1x == 0) {                             // src1 broadcast: b*beta+gamma is constant
                Wvec bb; vx_setall_as(src1, bb); Wvec bc = v_fma(bb, vb, vg);
                ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*4) {
                    ਜੇ (x+VECSZ*4 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ) ਰੋਕੋ; x = ਚੌੜਾਈ-VECSZ*4; }
                    vx_ਲੱਦੋ_pair_as(src0+x, a0, a1); vx_ਲੱਦੋ_pair_as(src0+x+VECSZ*2, a2, a3);
                    a0=v_fma(a0,va,bc); a1=v_fma(a1,va,bc); a2=v_fma(a2,va,bc); a3=v_fma(a3,va,bc);
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            ਨਹੀਂਤਾਂ {                                           // src0 broadcast: a*alpha+gamma is constant
                Wvec aa; vx_setall_as(src0, aa); Wvec acg = v_fma(aa, va, vg);
                ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*4) {
                    ਜੇ (x+VECSZ*4 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ) ਰੋਕੋ; x = ਚੌੜਾਈ-VECSZ*4; }
                    vx_ਲੱਦੋ_pair_as(src1+x, b0, b1); vx_ਲੱਦੋ_pair_as(src1+x+VECSZ*2, b2, b3);
                    b0=v_fma(b0,vb,acg); b1=v_fma(b1,vb,acg); b2=v_fma(b2,vb,acg); b3=v_fma(b3,vb,acg);
                    v_store_pair_as(dst+x, b0, b1); v_store_pair_as(dst+x+VECSZ*2, b2, b3);
                }
            }
        }
    #ਜੇ_ਅੰਤ
        ਜੇ (s0x == s1x) {
            ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + (WT)src1[x]*beta + gamma);
        } ਨਹੀਂਤਾਂ ਜੇ (s0x == 0) {
            ਅਚਲ WT ac = (WT)src0[0]*alpha + gamma;
            ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = saturate_cast<Tr>((WT)src1[x]*beta + ac);
        } ਨਹੀਂਤਾਂ {
            ਅਚਲ WT bc = (WT)src1[0]*beta + gamma;
            ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + bc);
        }
    }
    ਵਾਪਸ 0;
}

TKernel get_ਜੋੜੋ_Weighted_Func_2(ਪੂਰਨ T, ਪੂਰਨ R)
{
    KernelFunc f = ਖਾਲੀ_ਸੂਚਕ;
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AWS(Tt, Rr) ਜੋੜੋ_Weighted_Kernel<Tt, Rr, v_float32, ਦਸ਼ਮਲਵ, ਸੱਚ>
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AWD(Tt, Rr) ਜੋੜੋ_Weighted_Kernel<Tt, Rr, v_float64, ਦੁੱਗਣਾ, ਸੱਚ>       // f64 SIMD
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AWD(Tt, Rr) ਜੋੜੋ_Weighted_Kernel<Tt, Rr, v_float32, ਦੁੱਗਣਾ, ਝੂਠ>      // scalar (v_float32 unused)
#ਜੇ_ਅੰਤ
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_8U:  f = R==CV_8U  ? AWS(uint8_t,  uint8_t)  : R==CV_32F ? AWS(uint8_t,  ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_8S:  f = R==CV_8S  ? AWS(int8_t,   int8_t)   : R==CV_32F ? AWS(int8_t,   ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16U: f = R==CV_16U ? AWS(uint16_t, uint16_t) : R==CV_32F ? AWS(uint16_t, ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16S: f = R==CV_16S ? AWS(int16_t,  int16_t)  : R==CV_32F ? AWS(int16_t,  ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16F: f = R==CV_16F ? AWS(hfloat,   hfloat)   : R==CV_32F ? AWS(hfloat,   ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_16BF:f = R==CV_32F ? AWS(bfloat,   ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32F: f = R==CV_32F ? AWS(ਦਸ਼ਮਲਵ,    ਦਸ਼ਮਲਵ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32U: f = R==CV_32U ? AWD(ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ) : R==CV_64F ? AWD(ਬਿਨਾਨਿਸ਼ਾਨੀ, ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_32S: f = R==CV_32S ? AWD(ਪੂਰਨ,      ਪੂਰਨ)      : R==CV_64F ? AWD(ਪੂਰਨ,      ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_64U: f = R==CV_64F ? AWD(uint64_t, ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_64S: f = R==CV_64F ? AWD(int64_t,  ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਹਾਲਤ CV_64F: f = R==CV_64F ? AWD(ਦੁੱਗਣਾ,   ਦੁੱਗਣਾ) : ਖਾਲੀ_ਸੂਚਕ; ਰੋਕੋ;
    ਮੂਲ: ;
    }
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ AWS
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ AWD
    ਵਾਪਸ {f, ਖਾਲੀ_ਸੂਚਕ, 0};
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
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
// VECSZ(T) mask bytes -> T-width lanes (u8 direct, u16/u32 via expand)
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint8  ਲੱਦੋ_Select_ਨਕਾਬ(ਅਚਲ uchar* m, ਅਚਲ v_uint8&)  { ਵਾਪਸ vx_ਲੱਦੋ(m); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint16 ਲੱਦੋ_Select_ਨਕਾਬ(ਅਚਲ uchar* m, ਅਚਲ v_uint16&) { ਵਾਪਸ vx_ਲੱਦੋ_expand(m); }
ਸਥਿਰ ਸਤਰਅੰਦਰ v_uint32 ਲੱਦੋ_Select_ਨਕਾਬ(ਅਚਲ uchar* m, ਅਚਲ v_uint32&) { ਵਾਪਸ vx_ਲੱਦੋ_expand_q(m); }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallSelect(ਅਚਲ uint8_t*  p, v_uint8&  a) { a = vx_setall_u8(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallSelect(ਅਚਲ uint16_t* p, v_uint16& a) { a = vx_setall_u16(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallSelect(ਅਚਲ uint32_t* p, v_uint32& a) { a = vx_setall_u32(*p); }
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Tvec>
ਸਥਿਰ ਪੂਰਨ selectKernel(ਅਚਲ ਖਾਲੀ* ਨਕਾਬ_3, size_t smy, size_t smx,
                        ਅਚਲ ਖਾਲੀ* src1_, size_t s1y, size_t s1x,
                        ਅਚਲ ਖਾਲੀ* src2_, size_t s2y, size_t s2x,
                        ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                        ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ*)
{
    s1y /= ਆਕਾਰ(T);                    // the 1-byte mask's smy stays in bytes == elements
    s2y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(T);
    CV_Assert(smx <= 1u && s1x <= 1u && s2x <= 1u);

    ਅਚਲ uchar* ਨਕਾਬ = (ਅਚਲ uchar*)ਨਕਾਬ_3;
    ਅਚਲ T* src1 = (ਅਚਲ T*)src1_;
    ਅਚਲ T* src2 = (ਅਚਲ T*)src2_;
    T* dst = (T*)dst_;

    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && smy == smx*(size_t)ਚੌੜਾਈ &&
        s1y == s1x*(size_t)ਚੌੜਾਈ && s2y == s2x*(size_t)ਚੌੜਾਈ)
    { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }

    ਪੂਰਨ y = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
    ਜੇ constexpr (ਆਕਾਰ(T) <= 4)
    {
        ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Tvec>::vlanes();
        ਅਚਲ Tvec z = v_setzero_<Tvec>();

        // n-channel data under a per-pixel mask (the masked-op shape): channel axis = width (2..4),
        // one mask byte per row. Process VECSZ rows per iteration - expand the mask once and
        // interleave it across the channel lanes.
        ਜੇ (ਉਚਾਈ > VECSZ && 2 <= ਚੌੜਾਈ && ਚੌੜਾਈ <= 4 && smx == 0u && smy == 1u &&
            s1x == 1u && s1y == (size_t)ਚੌੜਾਈ && s2x == 1u && s2y == (size_t)ਚੌੜਾਈ &&
            dsty == (size_t)ਚੌੜਾਈ)
        {
            constexpr ਪੂਰਨ MAXVECSZ = VTraits<Tvec>::max_nlanes;
            T maskbuf[MAXVECSZ*4] = {};      // {} for -Wmaybe-uninitialized only (fully written)
            ਅਚਲ ਪੂਰਨ dy = VECSZ;
            ਲਈ (; y + dy <= ਉਚਾਈ; y += dy, ਨਕਾਬ += dy, src1 += ਚੌੜਾਈ*dy, src2 += ਚੌੜਾਈ*dy,
                                                          dst += ਚੌੜਾਈ*dy)
            {
                Tvec m0 = v_ne(ਲੱਦੋ_Select_ਨਕਾਬ(ਨਕਾਬ, z), z), m1, m2, m3;
                ਜੇ (ਚੌੜਾਈ == 2)      v_store_interleave(maskbuf, m0, m0);
                ਨਹੀਂਤਾਂ ਜੇ (ਚੌੜਾਈ == 3) v_store_interleave(maskbuf, m0, m0, m0);
                ਨਹੀਂਤਾਂ                 v_store_interleave(maskbuf, m0, m0, m0, m0);
                m0 = vx_ਲੱਦੋ(maskbuf);
                m1 = vx_ਲੱਦੋ(maskbuf + VECSZ);
                ਜੇ (ਚੌੜਾਈ > 2) m2 = vx_ਲੱਦੋ(maskbuf + VECSZ*2);
                ਜੇ (ਚੌੜਾਈ > 3) m3 = vx_ਲੱਦੋ(maskbuf + VECSZ*3);
                v_store(dst, v_select(m0, vx_ਲੱਦੋ(src1), vx_ਲੱਦੋ(src2)));
                v_store(dst + VECSZ, v_select(m1, vx_ਲੱਦੋ(src1 + VECSZ), vx_ਲੱਦੋ(src2 + VECSZ)));
                ਜੇ (ਚੌੜਾਈ > 2)
                    v_store(dst + VECSZ*2,
                            v_select(m2, vx_ਲੱਦੋ(src1 + VECSZ*2), vx_ਲੱਦੋ(src2 + VECSZ*2)));
                ਜੇ (ਚੌੜਾਈ > 3)
                    v_store(dst + VECSZ*3,
                            v_select(m3, vx_ਲੱਦੋ(src1 + VECSZ*3), vx_ਲੱਦੋ(src2 + VECSZ*3)));
            }
            // the remaining < VECSZ rows fall through to the per-row path below
        }
        ਨਹੀਂਤਾਂ ਜੇ (smx == 1)                                   // per-element mask - the common case
        {
            // a branch aliasing dst is fine under the backoff (idempotent select); dst == mask is not
            ਅਚਲ ਤਾਰਕਿਕ use_ਪੂਛ_trick = ਚੌੜਾਈ >= VECSZ*2 && dst_ != ਨਕਾਬ_3;
            Tvec a, b;
            ਲਈ (; y < ਉਚਾਈ; y++, ਨਕਾਬ += smy, src1 += s1y, src2 += s2y, dst += dsty)
            {
                // a row-constant branch still changes per row
                ਜੇ (s1x == 0) setallSelect(src1, a);
                ਜੇ (s2x == 0) setallSelect(src2, b);
                ਪੂਰਨ x = 0;
                ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ)
                {
                    ਜੇ (x + VECSZ > ਚੌੜਾਈ) { ਜੇ (!use_ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ; }
                    Tvec m = v_ne(ਲੱਦੋ_Select_ਨਕਾਬ(ਨਕਾਬ + x, z), z);
                    ਜੇ (s1x) a = vx_ਲੱਦੋ(src1 + x);
                    ਜੇ (s2x) b = vx_ਲੱਦੋ(src2 + x);
                    v_store(dst + x, v_select(m, a, b));
                }
                ਲਈ (; x < ਚੌੜਾਈ; x++)
                    dst[x] = ਨਕਾਬ[x] != 0 ? src1[x*s1x] : src2[x*s2x];
            }
            ਵਾਪਸ 0;
        }
    }
#ਜੇ_ਅੰਤ
    ਲਈ (; y < ਉਚਾਈ; y++, ਨਕਾਬ += smy, src1 += s1y, src2 += s2y, dst += dsty)
    {
        ਜੇ (smx == 0)                     // one mask byte per row (n-channel data / broadcast mask)
        {
            ਅਚਲ T* s = ਨਕਾਬ[0] != 0 ? src1 : src2;
            ਅਚਲ size_t sx = ਨਕਾਬ[0] != 0 ? s1x : s2x;
            ਜੇ ((ਅਚਲ ਖਾਲੀ*)s != (ਅਚਲ ਖਾਲੀ*)dst)      // row select from dst itself is a no-op
                ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++) dst[x] = s[x*sx];
        }
        ਨਹੀਂਤਾਂ
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++)
                dst[x] = ਨਕਾਬ[x] != 0 ? src1[x*s1x] : src2[x*s2x];
    }
    ਵਾਪਸ 0;
}

TKernel getSelectFunc_(ਪੂਰਨ mdepth, ਪੂਰਨ T)
{
    ਜੇ (CV_ELEM_SIZE1(mdepth) != 1)      // the mask must be a 1-byte type (u8/s8/bool)
        ਵਾਪਸ {};
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (CV_ELEM_SIZE1(T))
    {
    ਹਾਲਤ 1: fptr = selectKernel<uint8_t,  v_uint8 >; ਰੋਕੋ;
    ਹਾਲਤ 2: fptr = selectKernel<uint16_t, v_uint16>; ਰੋਕੋ;
    ਹਾਲਤ 4: fptr = selectKernel<uint32_t, v_uint32>; ਰੋਕੋ;
    ਹਾਲਤ 8: fptr = selectKernel<uint64_t, v_uint32>; ਰੋਕੋ;   // SIMD path compiled out -> scalar
    ਮੂਲ: ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
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
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallClamp(ਅਚਲ uchar*  p, v_uint8&   a) { a = vx_setall_u8(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallClamp(ਅਚਲ schar*  p, v_int8&    a) { a = vx_setall_s8(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallClamp(ਅਚਲ ushort* p, v_uint16&  a) { a = vx_setall_u16(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallClamp(ਅਚਲ ਛੋਟਾ*  p, v_int16&   a) { a = vx_setall_s16(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallClamp(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ* p, v_uint32& a) { a = vx_setall_u32(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallClamp(ਅਚਲ ਪੂਰਨ*    p, v_int32&   a) { a = vx_setall_s32(*p); }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallClamp(ਅਚਲ ਦਸ਼ਮਲਵ*  p, v_float32& a) { a = vx_setall_f32(*p); }
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ setallClamp(ਅਚਲ ਦੁੱਗਣਾ* p, v_float64& a) { a = vx_setall_f64(*p); }
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Tvec>
ਸਥਿਰ ਪੂਰਨ clampKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                       ਅਚਲ ਖਾਲੀ* lo_, size_t s1y, size_t s1x,
                       ਅਚਲ ਖਾਲੀ* hi_, size_t s2y, size_t s2x,
                       ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                       ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ*)
{
    s0y /= ਆਕਾਰ(T);
    s1y /= ਆਕਾਰ(T);
    s2y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);

    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* lo = (ਅਚਲ T*)lo_;
    ਅਚਲ T* hi = (ਅਚਲ T*)hi_;
    T* dst = (T*)dst_;

    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && s0y == s0x*(size_t)ਚੌੜਾਈ &&
        s1y == s1x*(size_t)ਚੌੜਾਈ && s2y == s2x*(size_t)ਚੌੜਾਈ)
    { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }

    ਲਈ (ਪੂਰਨ y = 0; y < ਉਚਾਈ; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
    {
        ਪੂਰਨ x = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਜੇ (s0x == 1)
        {
            ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Tvec>::vlanes();
            ਅਚਲ ਤਾਰਕਿਕ use_ਪੂਛ_trick = ਚੌੜਾਈ >= VECSZ*2 && lo_ != dst_ && hi_ != dst_;
            Tvec vlo, vhi;
            ਜੇ (s1x == 0) setallClamp(lo, vlo);
            ਜੇ (s2x == 0) setallClamp(hi, vhi);
            ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ)
            {
                ਜੇ (x + VECSZ > ਚੌੜਾਈ) { ਜੇ (!use_ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ; }
                ਜੇ (s1x) vlo = vx_ਲੱਦੋ(lo + x);
                ਜੇ (s2x) vhi = vx_ਲੱਦੋ(hi + x);
                v_store(dst + x, v_min(v_max(vx_ਲੱਦੋ(src0 + x), vlo), vhi));
            }
        }
#ਜੇ_ਅੰਤ
        ਲਈ (; x < ਚੌੜਾਈ; x++)
        {
            T v = src0[x*s0x], l = lo[x*s1x], h = hi[x*s2x];
            dst[x] = std::min(std::max(v, l), h);
        }
    }
    ਵਾਪਸ 0;
}

// Scalar-only clamp for the depths without a native SIMD lane type here (f16/bf16 - compared in
// float; 64-bit ints).
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT>
ਸਥਿਰ ਪੂਰਨ scalarClampKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                             ਅਚਲ ਖਾਲੀ* lo_, size_t s1y, size_t s1x,
                             ਅਚਲ ਖਾਲੀ* hi_, size_t s2y, size_t s2x,
                             ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                             ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ*)
{
    s0y /= ਆਕਾਰ(T); s1y /= ਆਕਾਰ(T); s2y /= ਆਕਾਰ(T); dsty /= ਆਕਾਰ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);
    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* lo = (ਅਚਲ T*)lo_;
    ਅਚਲ T* hi = (ਅਚਲ T*)hi_;
    T* dst = (T*)dst_;
    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && s0y == s0x*(size_t)ਚੌੜਾਈ &&
        s1y == s1x*(size_t)ਚੌੜਾਈ && s2y == s2x*(size_t)ਚੌੜਾਈ)
    { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }
    ਲਈ (ਪੂਰਨ y = 0; y < ਉਚਾਈ; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
        ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++)
        {
            WT v = (WT)src0[x*s0x], l = (WT)lo[x*s1x], h = (WT)hi[x*s2x];
            dst[x] = saturate_cast<T>(std::min(std::max(v, l), h));
        }
    ਵਾਪਸ 0;
}

TKernel getClampFunc_(ਪੂਰਨ T)
{
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_8U:   fptr = clampKernel<uchar,    v_uint8  >; ਰੋਕੋ;
    ਹਾਲਤ CV_8S:   fptr = clampKernel<schar,    v_int8   >; ਰੋਕੋ;
    ਹਾਲਤ CV_16U:  fptr = clampKernel<ushort,   v_uint16 >; ਰੋਕੋ;
    ਹਾਲਤ CV_16S:  fptr = clampKernel<ਛੋਟਾ,    v_int16  >; ਰੋਕੋ;
    ਹਾਲਤ CV_32U:  fptr = clampKernel<ਬਿਨਾਨਿਸ਼ਾਨੀ, v_uint32 >; ਰੋਕੋ;
    ਹਾਲਤ CV_32S:  fptr = clampKernel<ਪੂਰਨ,      v_int32  >; ਰੋਕੋ;
    ਹਾਲਤ CV_32F:  fptr = clampKernel<ਦਸ਼ਮਲਵ,    v_float32>; ਰੋਕੋ;
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਹਾਲਤ CV_64F:  fptr = clampKernel<ਦੁੱਗਣਾ,   v_float64>; ਰੋਕੋ;
#ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_64F:  fptr = scalarClampKernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>; ਰੋਕੋ;
#ਜੇ_ਅੰਤ
    ਹਾਲਤ CV_16F:  fptr = scalarClampKernel<hfloat, ਦਸ਼ਮਲਵ>; ਰੋਕੋ;
    ਹਾਲਤ CV_16BF: fptr = scalarClampKernel<bfloat, ਦਸ਼ਮਲਵ>; ਰੋਕੋ;
    ਹਾਲਤ CV_64U:  fptr = scalarClampKernel<uint64_t, uint64_t>; ਰੋਕੋ;
    ਹਾਲਤ CV_64S:  fptr = scalarClampKernel<int64_t,  int64_t >; ਰੋਕੋ;
    ਮੂਲ: ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}

ਸਥਿਰ ਪੂਰਨ expandKernel(size_t s0y, size_t s0x,
                        ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                        ਪੂਰਨ esz1)
{
    uchar* dst = (uchar*)dst_;
    ਜੇ (s0x == 0u && ਚੌੜਾਈ > 1) {
        ਪੂਰਨ h = s0y > 0u ? ਉਚਾਈ : 1;
        CV_Assert(esz1 == 1 || esz1 == 2 || esz1 == 4 || esz1 == 8);
        ਲਈ (ਪੂਰਨ y = 0; y < h; y++) {
            ਜੇ (esz1 == 1) {
                uchar* rowptr = dst + dsty*y;
                uchar val = rowptr[0];
                ਲਈ (ਪੂਰਨ x = 1; x < ਚੌੜਾਈ; x++) rowptr[x] = val;
            }
            ਨਹੀਂਤਾਂ ਜੇ (esz1 == 2) {
                ushort* rowptr = (ushort*)(dst + dsty*y);
                ushort val = rowptr[0];
                ਲਈ (ਪੂਰਨ x = 1; x < ਚੌੜਾਈ; x++) rowptr[x] = val;
            }
            ਨਹੀਂਤਾਂ ਜੇ (esz1 == 4) {
                ਬਿਨਾਨਿਸ਼ਾਨੀ* rowptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ*)(dst + dsty*y);
                ਬਿਨਾਨਿਸ਼ਾਨੀ val = rowptr[0];
                ਲਈ (ਪੂਰਨ x = 1; x < ਚੌੜਾਈ; x++) rowptr[x] = val;
            }
            ਨਹੀਂਤਾਂ {
                uint64_t* rowptr = (uint64_t*)(dst + dsty*y);
                uint64_t val = rowptr[0];
                ਲਈ (ਪੂਰਨ x = 1; x < ਚੌੜਾਈ; x++) rowptr[x] = val;
            }
        }
    }

    ਜੇ (s0y == 0) {
        ਲਈ (ਪੂਰਨ y = 1; y < ਉਚਾਈ; y++)
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + dsty*y, dst, (size_t)esz1*ਚੌੜਾਈ);
    }
    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ castKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                      ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                      ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                      ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                      ਅਚਲ ਦੁੱਗਣਾ* params, ਪੂਰਨ esz1, ਖਾਲੀ* userdata)
{
    ਦੁਆਧਾਰੀ_Func castfunc = (ਦੁਆਧਾਰੀ_Func)userdata;
    castfunc((ਅਚਲ uchar*)src0_, s0y, ਖਾਲੀ_ਸੂਚਕ, 0, (uchar*)dst_, dsty,
             ਮਾਪ((s0x > 0u ? ਚੌੜਾਈ : 1), (s0y > 0u ? ਉਚਾਈ : 1)), (ਖਾਲੀ*)params);
    ਵਾਪਸ expandKernel(s0y, s0x, dst_, dsty, ਚੌੜਾਈ, ਉਚਾਈ, esz1);
}

// ===========================================================================
// Per-op entry points (this baseline). The op-level getElemwiseFunc() dispatcher and the regular
// get*Func() forwarders live in arithm.dispatch.cpp.
// ===========================================================================

// OP_CAST / OP_CONVERT_SCALE: wrap core's convert BinaryFunc (carried in kernel.userdata) in
// castKernel, which runs it over the distinct sub-region and then expands across broadcast axes.
TKernel getCastFunc_(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth, ਤਾਰਕਿਕ scaled)
{
    ਦੁਆਧਾਰੀ_Func cvt = scaled ? getConvertScaleFunc(sdepth, ddepth) : getConvertFunc(sdepth, ddepth);
    ਵਾਪਸ {castKernel, (ਖਾਲੀ*)cvt, CV_ELEM_SIZE1(ddepth)};
}

// Non-template wrappers around the templated kernel selectors (ADD/SUB share getAddSubFunc, MIN/MAX
// share getMinMaxFunc). min/max are T x T -> T, so R is ignored (the dispatcher validates R == T).
TKernel get_ਜੋੜੋ_Func_2(ਪੂਰਨ T, ਪੂਰਨ R) { ਵਾਪਸ get_ਜੋੜੋ_Sub_Func<Ew_ਜੋੜੋ>(T, R); }
TKernel getSubFunc_(ਪੂਰਨ T, ਪੂਰਨ R) { ਵਾਪਸ get_ਜੋੜੋ_Sub_Func<EwSub>(T, R); }
TKernel getMinFunc_(ਪੂਰਨ T, ਪੂਰਨ R) { (ਖਾਲੀ)R; ਵਾਪਸ getMinMaxFunc<EwMin>(T); }
TKernel getMaxFunc_(ਪੂਰਨ T, ਪੂਰਨ R) { (ਖਾਲੀ)R; ਵਾਪਸ getMinMaxFunc<EwMax>(T); }

TKernel getHypotFunc_(ਪੂਰਨ T, ਪੂਰਨ R)
{
    ਜੇ (R != T)
        ਵਾਪਸ {};
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_16F:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, hfloat, v_float32, ਦਸ਼ਮਲਵ, EwHypot>; ਰੋਕੋ;
    ਹਾਲਤ CV_16BF: fptr = vec_ਦੁਆਧਾਰੀ_Kernel<bfloat, bfloat, v_float32, ਦਸ਼ਮਲਵ, EwHypot>; ਰੋਕੋ;
    ਹਾਲਤ CV_32F:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ਦਸ਼ਮਲਵ,  ਦਸ਼ਮਲਵ,  v_float32, ਦਸ਼ਮਲਵ, EwHypot>; ਰੋਕੋ;
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਹਾਲਤ CV_64F:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, v_float64, ਦੁੱਗਣਾ, EwHypot>; ਰੋਕੋ;
#ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_64F:  fptr = scalar_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, EwHypot>; ਰੋਕੋ;
#ਜੇ_ਅੰਤ
    ਮੂਲ: ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}

TKernel getAtan2Func_(ਪੂਰਨ T, ਪੂਰਨ R)
{
    ਜੇ (R != T)
        ਵਾਪਸ {};
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_16F:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<hfloat, hfloat, v_float32, ਦਸ਼ਮਲਵ, EwAtan2>; ਰੋਕੋ;
    ਹਾਲਤ CV_16BF: fptr = vec_ਦੁਆਧਾਰੀ_Kernel<bfloat, bfloat, v_float32, ਦਸ਼ਮਲਵ, EwAtan2>; ਰੋਕੋ;
    ਹਾਲਤ CV_32F:  fptr = vec_ਦੁਆਧਾਰੀ_Kernel<ਦਸ਼ਮਲਵ,  ਦਸ਼ਮਲਵ,  v_float32, ਦਸ਼ਮਲਵ, EwAtan2>; ਰੋਕੋ;
    ਹਾਲਤ CV_64F:  fptr = scalar_ਦੁਆਧਾਰੀ_Kernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, EwAtan2>; ਰੋਕੋ;   // exact std::atan2
    ਮੂਲ: ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}

#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
}} // namespace cv::ew
