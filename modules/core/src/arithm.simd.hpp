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

#شامل "opencv2/core/hal/intrin.hpp"
#شامل "convert.hpp"          // typed load/store-as helpers (cv::), and getConvert*Func
#شامل "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#شامل <algorithm>
#شامل <cmath>
#شامل <cstring>

نامخلا cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// BinaryFunc and getConvertFunc / getConvertScaleFunc come from core (precomp.hpp / convert.hpp).

#اگر CV_SIMD128_FP16
#تعریف_منسوخ CV_SIMD_16F
#تعریف_کرو CV_SIMD_16F 1
#وگرنہ_اگر !defined(CV_SIMD_16F)
#تعریف_کرو CV_SIMD_16F 0       // keep `#if CV_SIMD_16F` -Wundef-clean on builds without FP16 SIMD
#اگر_ختم

#اگر (CV_SIMD || CV_SIMD_SCALABLE)
ساکن اندرونی خالی vx_setall_as(ثابت uchar* p, v_uint8& a)
{ a = vx_setall_u8(*p); }
ساکن اندرونی خالی vx_setall_as(ثابت schar* p, v_int8& a)
{ a = vx_setall_s8(*p); }
ساکن اندرونی خالی vx_setall_as(ثابت uchar* p, v_int16& a)
{ a = vx_setall_s16(*p); }
ساکن اندرونی خالی vx_setall_as(ثابت schar* p, v_int16& a)
{ a = vx_setall_s16(*p); }

ساکن اندرونی خالی vx_setall_as(ثابت ushort* p, v_uint16& a)
{ a = vx_setall_u16(*p); }
ساکن اندرونی خالی vx_setall_as(ثابت مختصر* p, v_int16& a)
{ a = vx_setall_s16(*p); }
ساکن اندرونی خالی vx_setall_as(ثابت ushort* p, v_int32& a)
{ a = vx_setall_s32(*p); }
ساکن اندرونی خالی vx_setall_as(ثابت مختصر* p, v_int32& a)
{ a = vx_setall_s32(*p); }

ساکن اندرونی خالی vx_setall_as(ثابت غیرعلامتی* p, v_uint32& a)
{ a = vx_setall_u32(*p); }
ساکن اندرونی خالی vx_setall_as(ثابت صحیح* p, v_int32& a)
{ a = vx_setall_s32(*p); }

ساکن اندرونی خالی vx_setall_as(ثابت uchar* p, v_float32& a)
{ a = vx_setall_f32(اعشاری(*p)); }
ساکن اندرونی خالی vx_setall_as(ثابت schar* p, v_float32& a)
{ a = vx_setall_f32(اعشاری(*p)); }
ساکن اندرونی خالی vx_setall_as(ثابت ushort* p, v_float32& a)
{ a = vx_setall_f32(اعشاری(*p)); }
ساکن اندرونی خالی vx_setall_as(ثابت مختصر* p, v_float32& a)
{ a = vx_setall_f32(اعشاری(*p)); }
ساکن اندرونی خالی vx_setall_as(ثابت اعشاری* p, v_float32& a)
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
#اگر defined(__ARM_NEON)
ساکن اندرونی v_int32  v_جوڑو_sat(ثابت v_int32& a,  ثابت v_int32& b)  { واپس v_int32(vqaddq_s32(a.val, b.val)); }
ساکن اندرونی v_int32  v_sub_sat(ثابت v_int32& a,  ثابت v_int32& b)  { واپس v_int32(vqsubq_s32(a.val, b.val)); }
ساکن اندرونی v_uint32 v_جوڑو_sat(ثابت v_uint32& a, ثابت v_uint32& b) { واپس v_uint32(vqaddq_u32(a.val, b.val)); }
ساکن اندرونی v_uint32 v_sub_sat(ثابت v_uint32& a, ثابت v_uint32& b) { واپس v_uint32(vqsubq_u32(a.val, b.val)); }
#تعریف_کرو EW_HAVE_MULSAT32 1
ساکن اندرونی v_uint8 v_mul_sat(ثابت v_uint8& a, ثابت v_uint8& b)
{
    uint16x8_t p0 = vmull_u8(vget_کم_u8(a.val), vget_کم_u8(b.val));
    uint16x8_t p1 = vmull_u8(vget_بلند_u8(a.val), vget_بلند_u8(b.val));
    واپس v_uint8(vcombine_u8(vqmovn_u16(p0), vqmovn_u16(p1)));
}
ساکن اندرونی v_int8 v_mul_sat(ثابت v_int8& a, ثابت v_int8& b)
{
    int16x8_t p0 = vmull_s8(vget_کم_s8(a.val), vget_کم_s8(b.val));
    int16x8_t p1 = vmull_s8(vget_بلند_s8(a.val), vget_بلند_s8(b.val));
    واپس v_int8(vcombine_s8(vqmovn_s16(p0), vqmovn_s16(p1)));
}
ساکن اندرونی v_uint16 v_mul_sat(ثابت v_uint16& a, ثابت v_uint16& b)
{
    uint32x4_t p0 = vmull_u16(vget_کم_u16(a.val), vget_کم_u16(b.val));
    uint32x4_t p1 = vmull_u16(vget_بلند_u16(a.val), vget_بلند_u16(b.val));
    واپس v_uint16(vcombine_u16(vqmovn_u32(p0), vqmovn_u32(p1)));
}
ساکن اندرونی v_int16 v_mul_sat(ثابت v_int16& a, ثابت v_int16& b)
{
    int32x4_t p0 = vmull_s16(vget_کم_s16(a.val), vget_کم_s16(b.val));
    int32x4_t p1 = vmull_s16(vget_بلند_s16(a.val), vget_بلند_s16(b.val));
    واپس v_int16(vcombine_s16(vqmovn_s32(p0), vqmovn_s32(p1)));
}
ساکن اندرونی v_uint32 v_mul_sat(ثابت v_uint32& a, ثابت v_uint32& b)
{
    uint64x2_t p0 = vmull_u32(vget_کم_u32(a.val), vget_کم_u32(b.val));
    uint64x2_t p1 = vmull_u32(vget_بلند_u32(a.val), vget_بلند_u32(b.val));
    واپس v_uint32(vcombine_u32(vqmovn_u64(p0), vqmovn_u64(p1)));
}
ساکن اندرونی v_int32 v_mul_sat(ثابت v_int32& a, ثابت v_int32& b)
{
    int64x2_t p0 = vmull_s32(vget_کم_s32(a.val), vget_کم_s32(b.val));
    int64x2_t p1 = vmull_s32(vget_بلند_s32(a.val), vget_بلند_s32(b.val));
    واپس v_int32(vcombine_s32(vqmovn_s64(p0), vqmovn_s64(p1)));
}
#ورنہ
ساکن اندرونی v_int32 v_جوڑو_sat(ثابت v_int32& a, ثابت v_int32& b)
{
    v_int32 res = v_جوڑو(a, b);
    v_int32 ov  = v_and(v_xor(a, res), v_xor(b, res));               // sign bit set iff overflow
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(زیادہ_سے_زیادہ_عدد));       // a >= 0 ? INT_MAX : INT_MIN
    واپس v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
ساکن اندرونی v_int32 v_sub_sat(ثابت v_int32& a, ثابت v_int32& b)
{
    v_int32 res = v_sub(a, b);
    v_int32 ov  = v_and(v_xor(a, b), v_xor(a, res));
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(زیادہ_سے_زیادہ_عدد));
    واپس v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
ساکن اندرونی v_uint32 v_جوڑو_sat(ثابت v_uint32& a, ثابت v_uint32& b)
{
    v_uint32 res = v_جوڑو(a, b);
    واپس v_or(res, v_lt(res, a));                                  // wrapped => all-ones
}
ساکن اندرونی v_uint32 v_sub_sat(ثابت v_uint32& a, ثابت v_uint32& b)
{
    واپس v_and(v_sub(a, b), v_ge(a, b));                           // borrow => zero
}
ساکن اندرونی v_uint8 v_mul_sat(ثابت v_uint8& a, ثابت v_uint8& b)
{ v_uint16 p0, p1; v_mul_expand(a, b, p0, p1); واپس v_pack(p0, p1); }
ساکن اندرونی v_int8 v_mul_sat(ثابت v_int8& a, ثابت v_int8& b)
{ v_int16 p0, p1; v_mul_expand(a, b, p0, p1); واپس v_pack(p0, p1); }
ساکن اندرونی v_uint16 v_mul_sat(ثابت v_uint16& a, ثابت v_uint16& b)
{ v_uint32 p0, p1; v_mul_expand(a, b, p0, p1); واپس v_pack(p0, p1); }
ساکن اندرونی v_int16 v_mul_sat(ثابت v_int16& a, ثابت v_int16& b)
{ v_int32 p0, p1; v_mul_expand(a, b, p0, p1); واپس v_pack(p0, p1); }
#اگر_ختم
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F   // scalable (RVV) has v_float64 without CV_SIMD_64F
ساکن اندرونی خالی vx_setall_as(ثابت دوہرا*   p, v_float64& a) { a = vx_setall_f64(*p); }
ساکن اندرونی خالی vx_setall_as(ثابت صحیح*      p, v_float64& a) { a = vx_setall_f64((دوہرا)*p); }
ساکن اندرونی خالی vx_setall_as(ثابت غیرعلامتی* p, v_float64& a) { a = vx_setall_f64((دوہرا)*p); }
ساکن اندرونی خالی vx_setall_as(ثابت int64_t*  p, v_float64& a) { a = vx_setall_f64((دوہرا)*p); }
ساکن اندرونی خالی vx_setall_as(ثابت uint64_t* p, v_float64& a) { a = vx_setall_f64((دوہرا)*p); }
#اگر_ختم
ساکن اندرونی خالی vx_setall_as(ثابت bfloat* p, v_float32& a)
{ a = vx_setall_f32(اعشاری(*p)); }
ساکن اندرونی خالی vx_setall_as(ثابت hfloat* p, v_float32& a)
{ a = vx_setall_f32(اعشاری(*p)); }

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uchar* p, v_uint8& a, v_uint8& b)
{
    a = vx_بھرو(p);
    b = vx_بھرو(p + VTraits<v_uint8>::vlanes());
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت schar* p, v_int8& a, v_int8& b)
{
    a = vx_بھرو(p);
    b = vx_بھرو(p + VTraits<v_int8>::vlanes());
}

ساکن اندرونی خالی vx_بھرو_pair_as(ثابت uchar* p, v_uint32& a, v_uint32& b)
{
    v_uint16 w = vx_بھرو_expand(p);
    v_expand(w, a, b);
}

ساکن اندرونی خالی v_store_pair_as(uchar* p, ثابت v_uint8& a, ثابت v_uint8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_uint8>::vlanes(), b);
}

ساکن اندرونی خالی v_store_pair_as(schar* p, ثابت v_int8& a, ثابت v_int8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_int8>::vlanes(), b);
}


ساکن اندرونی خالی v_store_pair_as(صحیح* p, ثابت v_int16& a, ثابت v_int16& b)
{
    ثابت صحیح nlanes32 = VTraits<v_int32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v0);
    v_store(p + nlanes32, v1);
    v_store(p + nlanes32*2, v2);
    v_store(p + nlanes32*3, v3);
}

ساکن اندرونی خالی v_store_pair_as(اعشاری* p, ثابت v_int16& a, ثابت v_int16& b)
{
    ثابت صحیح nlanes32 = VTraits<v_float32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v_cvt_f32(v0));
    v_store(p + nlanes32, v_cvt_f32(v1));
    v_store(p + nlanes32*2, v_cvt_f32(v2));
    v_store(p + nlanes32*3, v_cvt_f32(v3));
}

ساکن اندرونی خالی v_store_pair_as(اعشاری* p, ثابت v_int32& a, ثابت v_int32& b)
{
    v_store(p, v_cvt_f32(a));
    v_store(p + VTraits<v_float32>::vlanes(), v_cvt_f32(b));
}

ساکن اندرونی خالی v_store_pair_as(hfloat* p, ثابت v_float32& a, ثابت v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

ساکن اندرونی خالی v_store_pair_as(bfloat* p, ثابت v_float32& a, ثابت v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

// Reinterpret a comparison result (whose lane type matches the operands) as the UNSIGNED integer
// vector of the same width: u8->u8, s8->u8, u16/s16->u16, u32/s32/f32->u32. The destination type
// drives the overload, so the compare kernel needs no per-width if constexpr.
ساکن اندرونی خالی v_reinterpret_as(ثابت v_uint8&  s, v_uint8&  d) { d = s; }
ساکن اندرونی خالی v_reinterpret_as(ثابت v_int8&   s, v_uint8&  d) { d = v_reinterpret_as_u8(s); }
ساکن اندرونی خالی v_reinterpret_as(ثابت v_uint16& s, v_uint16& d) { d = s; }
ساکن اندرونی خالی v_reinterpret_as(ثابت v_int16&  s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
ساکن اندرونی خالی v_reinterpret_as(ثابت v_uint32& s, v_uint32& d) { d = s; }
ساکن اندرونی خالی v_reinterpret_as(ثابت v_int32&  s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
ساکن اندرونی خالی v_reinterpret_as(ثابت v_float32& s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
#اگر CV_SIMD_16F
ساکن اندرونی خالی v_reinterpret_as(ثابت v_float16& s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
#اگر_ختم
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ساکن اندرونی خالی v_reinterpret_as(ثابت v_float64& s, v_uint64& d) { d = v_reinterpret_as_u64(s); }
#اگر_ختم

// Broadcast the compare mask value (1 or 255) across the unsigned work vector.
ساکن اندرونی خالی v_setall_نقاب(v_uint8&  v, uchar t) { v = vx_setall_u8(t); }
ساکن اندرونی خالی v_setall_نقاب(v_uint16& v, uchar t) { v = vx_setall_u16(t); }
ساکن اندرونی خالی v_setall_نقاب(v_uint32& v, uchar t) { v = vx_setall_u32(t); }

// NOTE: v_store_pair_as(uchar*, v_uint16/v_uint32, ...) - the narrowing pack-to-u8 stores we need -
// already come from core's convert.hpp (included above), so they are not redefined here.

#اگر CV_SIMD_16F
ساکن اندرونی خالی vx_setall_as(ثابت hfloat* p, v_float16& a)
{ a = vx_setall_f16(*p); }
ساکن اندرونی خالی vx_بھرو_pair_as(ثابت hfloat* p, v_float16& a, v_float16& b)
{
    a = vx_بھرو(p);
    b = vx_بھرو(p + VTraits<v_float16>::vlanes());
}
ساکن اندرونی خالی v_store_pair_as(hfloat* p, ثابت v_float16& a, ثابت v_float16& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_float16>::vlanes(), b);
}
#اگر_ختم

#اگر_ختم

#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

نامخلا ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      arithm.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel get_جوڑو_Func_2(صحیح T, صحیح R);
TKernel getSubFunc_(صحیح T, صحیح R);
TKernel getMulFunc_(صحیح T, صحیح R);
TKernel getDivFunc_(صحیح T, صحیح R, منطقی checked);
TKernel getMinFunc_(صحیح T, صحیح R);
TKernel getMaxFunc_(صحیح T, صحیح R);
TKernel getAbsdiffFunc_(صحیح T, صحیح R);
TKernel getHypotFunc_(صحیح T, صحیح R);         // hypot = sqrt(x^2+y^2), float depths, T x T -> T
TKernel getAtan2Func_(صحیح T, صحیح R);         // atan2(y, x), radians (-pi, pi], float depths
TKernel getCmpFunc_(چوٹی op, صحیح T);
TKernel getBitwiseFunc_(چوٹی op, صحیح esz);                    // OP_AND / OP_OR / OP_XOR, by element size
TKernel getNotFunc_(صحیح esz);                                // OP_NOT, by element size
TKernel get_جوڑو_Weighted_Func_2(صحیح T, صحیح R);                   // OP_ADDW, a*alpha+b*beta+gamma (T x T -> R)
TKernel getSelectFunc_(صحیح mdepth, صحیح T);   // OP_SELECT: 1-byte mask, a/b/dst of T (by esz)
TKernel getClampFunc_(صحیح T);                // OP_CLAMP: min(max(x, lo), hi), all operands of T
TKernel getCastFunc_(صحیح sdepth, صحیح ddepth, منطقی scaled);   // OP_CAST / OP_CONVERT_SCALE

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors (vector + scalar). New binary ops slot in here.
// ===========================================================================
ساخت Ew_جوڑو {
    // useScalar: does the op consume the scale scalar (params[0]) in vec()/preproc()? When false,
    // vecBinaryKernel's fast 2-arg branch is taken unconditionally (the check folds at compile time);
    // when true, it is taken only if the runtime scale is exactly 1.
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_جوڑو(a, b); }
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: v_add wraps - use the saturating version to match the scalar int64+clamp tail
    ساکن v_int32  vec(ثابت v_int32& a,  ثابت v_int32& b)  { واپس v_جوڑو_sat(a, b); }
    ساکن v_uint32 vec(ثابت v_uint32& a, ثابت v_uint32& b) { واپس v_جوڑو_sat(a, b); }
#اگر_ختم
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    // Accumulate in the promoted type, NOT in W: for the native saturating path W is the narrow
    // lane type (schar/short/...), and (W)(a+b) would wrap in 8/16 bits before saturate_cast<Tr>
    // could clamp. Letting a+b promote (narrow -> int) keeps saturation for 8/16-bit outputs and
    // the natural wrap for 32/64-bit (both matching cv::add). The SIMD path already saturates.
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس W(a + b); }
};

ساخت EwSub {
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_sub(a, b); }
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: see EwAdd::vec
    ساکن v_int32  vec(ثابت v_int32& a,  ثابت v_int32& b)  { واپس v_sub_sat(a, b); }
    ساکن v_uint32 vec(ثابت v_uint32& a, ثابت v_uint32& b) { واپس v_sub_sat(a, b); }
#اگر_ختم
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس W(a - b); }   // see EwAdd::scl
    // 64-bit unsigned has no wider WT to hold a-b, so the generic path would wrap on underflow.
    // Saturate at 0 to match cv::subtract (8/16/32-bit already saturate via SIMD floor / wide WT).
    ساکن uint64_t scl(uint64_t a, uint64_t b, uint64_t) { واپس uint64_t(a >= b)*(a - b); }
};

// mul/div compute in a wide FLOAT work type (W = float for <=16-bit/f16/bf/f32, double for
// 32/64-bit/f64), matching cv::multiply/divide; the executor casts the work-type result down to
// rdepth. scalar (reference) path only for now - SIMD can follow. (No vec(): never instantiated.)
// vec(a, b, scale): the 3rd arg is the scale vector. COMMUTATIVE ops (mul, absdiff) ignore it - their
// scale is folded into the (cheaper) preproc of one operand. DIVISION is NOT commutative, so its scale
// MUST stick to the numerator; it takes the scale in vec (numerator*scale/denominator) and leaves
// preproc as identity. vecBinaryKernel always passes vscalar to vec, so every branch (incl. a broadcast
// denominator) divides correctly. The 2-arg vec (scale==1) is the both-contiguous fast path.
ساخت EwMul {
    ساکن constexpr منطقی useScalar = سچ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_mul(a, b); }
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    // integer lanes: full product clamped to the lane type (v_mul_sat) == cv::multiply semantics
    // at scale 1. These drive the scale==1 fast path on FULL-width registers (Wvec1 = the native
    // lane vector): whole-register loads/stores, widening happens inside the multiply itself.
    // The guard matches the v_mul_sat definitions above (like EwAdd's 32-bit overloads): in
    // no-SIMD builds the vector typedefs still exist (intrin_cpp), but the helpers do not.
    ساکن v_uint8  vec(ثابت v_uint8& a,  ثابت v_uint8& b)  { واپس v_mul_sat(a, b); }
    ساکن v_int8   vec(ثابت v_int8& a,   ثابت v_int8& b)   { واپس v_mul_sat(a, b); }
    ساکن v_uint16 vec(ثابت v_uint16& a, ثابت v_uint16& b) { واپس v_mul_sat(a, b); }
    ساکن v_int16  vec(ثابت v_int16& a,  ثابت v_int16& b)  { واپس v_mul_sat(a, b); }
#اگر_تعریف_شدہ EW_HAVE_MULSAT32
    ساکن v_uint32 vec(ثابت v_uint32& a, ثابت v_uint32& b) { واپس v_mul_sat(a, b); }
    ساکن v_int32  vec(ثابت v_int32& a,  ثابت v_int32& b)  { واپس v_mul_sat(a, b); }
#اگر_ختم
#اگر_ختم
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V& s) { واپس v_mul(a, s); }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST s) { واپس a * b * s; }
};
// div has two variants by the COMMON INPUT type (matching cv::'s per-type kernel choice): integer
// inputs guard divide-by-zero -> 0 (cv:: iscalar_div); float inputs do NOT guard (cv:: fscalar_div,
// a/0 -> inf), which then saturates on the cast to an integer output exactly like cv::divide.
ساخت EwDivInt {
    ساکن constexpr منطقی useScalar = سچ;
    // integer inputs computed in the float work type: guard b==0 -> 0. Scale rides the numerator (vec).
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) {
        ثابت V z = v_setzero_<V>();
        واپس v_select(v_eq(b, z), z, v_div(a, b));
    }
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b, ثابت V& s) {
        ثابت V z = v_setzero_<V>();
        واپس v_select(v_eq(b, z), z, v_div(v_mul(a, s), b));
    }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }   // identity: scale is in vec
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST s) { واپس b != W(0) ? a * s / b : W(0); }
};
ساخت EwDivFlt {
    ساکن constexpr منطقی useScalar = سچ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_div(a, b); }
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b, ثابت V& s) { واپس v_div(v_mul(a, s), b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }   // identity: scale is in vec
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST s) { واپس a * s / b; }
};

// atan2(y, x) in RADIANS over the standard C range (-pi, pi] - the fastAtan2 minimax polynomial
// (mathfuncs_core.simd.hpp v_atan_f32) with the 180/pi factor dropped and the C quadrant logic
// (fastAtan2 returns degrees in [0, 360)). Absolute accuracy ~1e-5 rad, same as cv::fastAtan2.
// Generic over the universal-intrinsic float vector type.
سانچہ<قسم_نام V>
ساکن اندرونی V v_atan2(ثابت V& y, ثابت V& x)
{
    استعمال LT = قسم_نام VTraits<V>::lane_type;
    ثابت V eps  = v_setall_<V>((LT)DBL_EPSILON);
    ثابت V z    = v_setzero_<V>();
    ثابت V p7   = v_setall_<V>((LT)-0.04432655554792128);
    ثابت V p5   = v_setall_<V>((LT)0.1555786518463281);
    ثابت V p3   = v_setall_<V>((LT)-0.3258083974640975);
    ثابت V p1   = v_setall_<V>((LT)0.9997878412794807);
    ثابت V vpi2 = v_setall_<V>((LT)(CV_PI/2));
    ثابت V vpi  = v_setall_<V>((LT)CV_PI);

    V ax = v_abs(x), ay = v_abs(y);
    V c  = v_div(v_min(ax, ay), v_جوڑو(v_max(ax, ay), eps));
    V c2 = v_mul(c, c);
    V a  = v_mul(v_fma(v_fma(v_fma(p7, c2, p5), c2, p3), c2, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(vpi2, a));
    a = v_select(v_lt(x, z), v_sub(vpi, a), a);
    a = v_select(v_lt(y, z), v_sub(z, a), a);
    واپس a;
}

ساخت EwAtan2 {
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_atan2(a, b); }   // a = y, b = x
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس std::atan2(a, b); }
};

// hypot(x, y) = sqrt(x^2 + y^2): NAIVE (matches cv::magnitude; overflow at |x| ~ 1e19+ for f32
// inputs is accepted), computed in the float work type; T x T -> T over the float depths.
ساخت EwHypot {
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_sqrt(v_fma(a, a, v_mul(b, b))); }
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس std::جذر(a*a + b*b); }
};

// min / max / absdiff: T x T -> T (same depth in and out, no scale). v_min/v_max exist for every
// vector lane type (64-bit ints fall back to scalar). absdiff uses v_absdiff (defined for the
// UNSIGNED and float lane types - signed/wide depths go through the scalar path), and the scalar
// |a-b| is computed branch-wise so it never underflows an unsigned work type.
ساخت EwMin {
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_min(a, b); }
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس std::min(a, b); }
};
ساخت EwMax {
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_max(a, b); }
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس std::max(a, b); }
};
ساخت EwAbsdiff {
    ساکن constexpr منطقی useScalar = جھوٹ;
    // The absdiff RESULT is the UNSIGNED type of the input width: v_absdiff(v_int8/16/32) already returns
    // v_uint8/16/32 (the true |a-b|, which can exceed the signed max), and v_absdiff on unsigned/float
    // returns the same type. vec() therefore returns THAT type (deduced), not the input V, so the kernel
    // stores it through the matching UNSIGNED v_store_pair_as (an honest same-type store) - no reinterpret,
    // no touching the saturating-narrow overloads' semantics. (This is why the kernel keeps the vec
    // result in its own variable rather than reusing the input operand.)
    سانچہ<قسم_نام V> ساکن خودکار vec(ثابت V& a, ثابت V& b) { واپس v_absdiff(a, b); }
    سانچہ<قسم_نام V, قسم_نام S> ساکن خودکار vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس v_absdiff(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس a > b ? W(a - b) : W(b - a); }
};

// Signed T -> SAME signed T (rdepth==T): |a-b| saturated into the signed range, in ONE pass. cv::absdiff
// keeps the signed depth, so the generic path computes absdiff->unsigned then casts back down (2 insns);
// this fuses it. v_absdiff yields the unsigned |a-b|; v_min clamps to the signed max (already >=0), then
// a same-width reinterpret to signed (all values now fit).
ساخت EwAbsdiffS {
    ساکن constexpr منطقی useScalar = جھوٹ;
    ساکن v_int8  vec(ثابت v_int8&  a, ثابت v_int8&  b) { واپس v_reinterpret_as_s8 (v_min(v_absdiff(a, b), vx_setall_u8 (0x7f))); }
    ساکن v_int16 vec(ثابت v_int16& a, ثابت v_int16& b) { واپس v_reinterpret_as_s16(v_min(v_absdiff(a, b), vx_setall_u16(0x7fff))); }
    ساکن v_int32 vec(ثابت v_int32& a, ثابت v_int32& b) { واپس v_reinterpret_as_s32(v_min(v_absdiff(a, b), vx_setall_u32(0x7fffffff))); }
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس a > b ? W(a - b) : W(b - a); }
    // 64-bit signed has no wider work type to widen into, and |INT64_MIN - b| does not fit in
    // int64_t, so the difference is taken in uint64 - exact for every pair - and then clamped.
    // The narrower widths reach this overload only through their int64 work type, where |a - b|
    // is at most 2^32 and the clamp is a no-op.
    ساکن int64_t scl(int64_t a, int64_t b, int64_t)
    {
        ثابت uint64_t d = (a > b) ? (uint64_t)a - (uint64_t)b : (uint64_t)b - (uint64_t)a;
        واپس (int64_t)std::min<uint64_t>(d, (uint64_t)INT64_MAX);
    }
};

// compare: T x T -> u8 mask. cmp(a,b) is the scalar relation over a work type W (integers compare
// directly, f16/bf16 through float); vec(a,b) is the vector relation returning an all-ones/zero mask
// in the operand's own lane type. The kernel turns either into 0 / trueVal (1 or 255 via TKernel::flags).
ساخت EwCmpEq { سانچہ<قسم_نام W> ساکن منطقی cmp(W a, W b) { واپس a == b; }
                 سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_eq(a, b); } };
ساخت EwCmpNe { سانچہ<قسم_نام W> ساکن منطقی cmp(W a, W b) { واپس a != b; }
                 سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_ne(a, b); } };
// LT/LE are synthesized from GT/GE via the EW_KERNEL_SWAP01 flag (a<b == b>a), so no Lt/Le kernels.
ساخت EwCmpGt { سانچہ<قسم_نام W> ساکن منطقی cmp(W a, W b) { واپس a > b; }
                 سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_gt(a, b); } };
ساخت EwCmpGe { سانچہ<قسم_نام W> ساکن منطقی cmp(W a, W b) { واپس a >= b; }
                 سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_ge(a, b); } };

// bitwise AND/OR/XOR: bit-pattern op, type-agnostic. Run on the UNSIGNED integer whose width matches
// the element (u8/u16/u32/u64), so one functor set covers every depth. No scale, no widening (T x T ->
// T, exactly like min/max); preproc is the identity (min/max share this shape). 64-bit uses the scalar
// path (no widening vector helpers), the rest ride vecBinaryKernel's native same-type path.
ساخت EwAnd {
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_and(a, b); }
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس W(a & b); }
};
ساخت EwOr {
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_or(a, b); }
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس W(a | b); }
};
ساخت EwXor {
    ساکن constexpr منطقی useScalar = جھوٹ;
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& a, ثابت V& b) { واپس v_xor(a, b); }
    سانچہ<قسم_نام V, قسم_نام S> ساکن V vec(ثابت V& a, ثابت V& b, ثابت S&) { واپس vec(a, b); }
    سانچہ<قسم_نام V> ساکن V preproc(ثابت V& a, ثابت V&) { واپس a; }
    سانچہ<قسم_نام W, قسم_نام ST> ساکن W scl(W a, W b, ST) { واپس W(a ^ b); }
};

// Collapse a gap-free 2D tile to 1D (call with the per-operand x/y-steps).
#تعریف_کرو EW_TRY_COLLAPSE(NSRC) \
    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && \
        s0y == s0x*(size_t)چوڑائی && (NSRC < 2 || s1y == s1x*(size_t)چوڑائی)) \
    { چوڑائی *= اونچائی; اونچائی = 1; }

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
سانچہ<قسم_نام T, قسم_نام Tr, قسم_نام WT, جماعت Op, قسم_نام ST=WT>
ساکن صحیح scalar_ثنائی_Kernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                              ثابت خالی* src1_, size_t s1y, size_t s1x,
                              ثابت خالی*, size_t, size_t, خالی* dst_, size_t dsty,
                              صحیح چوڑائی, صحیح اونچائی, ثابت دوہرا* params, صحیح, خالی*)
{
    اگر (چوڑائی <= 0 || اونچائی <= 0) واپس 0;
    s0y /= حجم(T);
    s1y /= حجم(T);
    dsty /= حجم(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)چوڑائی == 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    برائے (صحیح y = 0; y < اونچائی; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        اگر (s0x == s1x) {
            برائے (صحیح x = 0; x < چوڑائی; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], (WT)src1[x], scalar));
        }
        ورنہ اگر (s0x == 0) {
            WT sc0 = (WT)src0[0];
            برائے (صحیح x = 0; x < چوڑائی; x++)
                dst[x] = saturate_cast<Tr>(Op::scl(sc0, (WT)src1[x], scalar));
        }
        ورنہ {
            WT sc1 = (WT)src1[0];
            برائے (صحیح x = 0; x < چوڑائی; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], sc1, scalar));
        }
    }
    واپس 0;
}

سانچہ<قسم_نام T, قسم_نام WT>
ساکن خالی expandScalar(ثابت T* sc, size_t sx, صحیح n0, WT* scbuf, صحیح n)
{
    اگر (n0 <= 0 || n <= 0) واپس;
    صحیح i = 0;
    برائے (; i < n0; i++) scbuf[i] = (WT)sc[i*sx];
    برائے (; i < n; i++) scbuf[i] = scbuf[i - n0];
}

// Decode the per-channel patch bytes (mask + value) from the kernel flags into small arrays. Uniform
// (no EW_CMP_PATCH): mask = trueVal, value = 0 (an ordinary compare). Per-channel: from the 4-bit
// fields. The compare kernels apply  result = (rawmask & mask) | value  per channel - folding the
// former separate patch pass into the compare (one pass).
ساکن اندرونی خالی cmpUnpackPatch(صحیح flags, uchar trueVal, uchar mvals[4], uchar vvals[4])
{
    اگر (flags & EW_CMP_PATCH)
        برائے (صحیح c = 0; c < 4; c++)
        {
            ثابت صحیح f = (flags >> (EW_CMP_PATCH_سرکاؤ + c*4)) & 0xF;
            mvals[c] = (uchar)cmpPatchByte(f & 3);
            vvals[c] = (uchar)cmpPatchByte((f >> 2) & 3);
        }
    ورنہ
        برائے (صحیح c = 0; c < 4; c++) { mvals[c] = trueVal; vvals[c] = 0; }
}

// Scalar comparison kernel: T x T -> u8 mask. Result is 0 (false) or `trueVal` (true), trueVal read
// from TKernel::flags - 255 (default, matching cv::compare) or 1 (a numpy-style 0/1 mask). Integers
// compare directly (WT == T); f16/bf16 compare through a float WT. This is the fallback for the depths
// the SIMD kernel doesn't cover (f16/bf16/64-bit). stepx in {0,1}; dst is u8, byte step == element step.
سانچہ<قسم_نام T, قسم_نام WT, جماعت Cmp>
ساکن صحیح scalarCompareKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                         ثابت خالی* src1_, size_t s1y, size_t s1x,
                         ثابت خالی*, size_t, size_t,
                         خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                         ثابت دوہرا*, صحیح flags, خالی*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    اگر (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= حجم(T);
    s1y /= حجم(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)چوڑائی == 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    uchar* dst = (uchar*)dst_;
    ثابت uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    ثابت منطقی perch = (flags & EW_CMP_PATCH) != 0;          // per-channel fix-up (width=cn<=4)
    CV_Assert(!perch || چوڑائی <= 4);                         // a per-channel patch is a short-row tile
    #تعریف_کرو EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                         : (uchar)((cond) ? trueVal : 0))

    EW_TRY_COLLAPSE(2);
    برائے (صحیح y = 0; y < اونچائی; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        اگر (s0x == s1x) {
            برائے (صحیح x = 0; x < چوڑائی; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], (WT)src1[x]), x);
        }
        ورنہ اگر (s0x == 0) {
            WT a = (WT)src0[0];
            برائے (صحیح x = 0; x < چوڑائی; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, (WT)src1[x]), x);
        }
        ورنہ {
            WT b = (WT)src1[0];
            برائے (صحیح x = 0; x < چوڑائی; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], b), x);
        }
    }
    #تعریف_منسوخ EW_CMP_APPLY
    واپس 0;
}

// SIMD comparison kernel: T x T -> u8 mask, for the directly-comparable depths (u8/s8/u16/s16/u32/
// s32/f32). vec(a,b) gives an all-ones/zero mask in the operand's lane type; v_reinterpret_as turns it
// into the unsigned int of the same width; cmpFuse packs to u8 and applies (rawmask & M) | V. The
// per-row SIMD body runs when an operand is contiguous or broadcast; a per-channel patch (M/V differ
// by channel) only ever arrives as a short-row (width=cn) tile and is handled there.
سانچہ<قسم_نام T, قسم_نام Vvec, قسم_نام Uvec, جماعت Cmp>
ساکن صحیح vecCompareKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                            ثابت خالی* src1_, size_t s1y, size_t s1x,
                            ثابت خالی*, size_t, size_t,
                            خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                            ثابت دوہرا*, صحیح flags, خالی*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    اگر (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= حجم(T);
    s1y /= حجم(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)چوڑائی == 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    uchar* dst = (uchar*)dst_;
    ثابت uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];                                 // per-channel fix-up: (rawmask & m) | v
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    ثابت منطقی perch = (flags & EW_CMP_PATCH) != 0;
    CV_Assert(!perch || چوڑائی <= 4);                          // a per-channel patch is a short-row tile

    EW_TRY_COLLAPSE(2);
    صحیح y = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    // Short rows (2/3/4 elements): a per-channel scalar over a multi-channel image arrives as a
    // (width=cn) x (height=pixels) tile with the scalar broadcast over rows (s?y==0). The per-row SIMD
    // below never triggers at such a tiny width, so expand the width<=4 broadcast operand (threshold
    // AND the interleaved per-channel mask/value bytes) across the lanes and compare many rows at once.
    // This is also the ONLY path a per-channel patch (M/V differ by channel) reaches.
    // constexpr: the short-row loads T natively (vx_load->Vvec), so it exists ONLY at native width. The
    // widened f16/bf16->f32 path (sizeof(T) < Vvec lane) and 8-byte f64 fall through to per-row + scalar.
    // (sizeof(T) <= 4 is part of the constexpr gate: for f64 the body would compile to nothing but
    // dead stores - gcc 9 flags scbuf/bsrc/bsx as set-but-not-used there.)
    اگر constexpr (حجم(T) == حجم(قسم_نام VTraits<Vvec>::lane_type) && حجم(T) <= 4)
    اگر (اونچائی > 1 && چوڑائی <= 4 &&
        ((s0y == 0 && s1y == (size_t)چوڑائی*s1x) || (s1y == 0 && s0y == (size_t)چوڑائی*s0x)) &&
        dsty == (size_t)چوڑائی)
    {
        ثابت صحیح VECSZ  = VTraits<Vvec>::vlanes();          // sizeof(T)-type lanes
        ثابت صحیح VECSZ8 = VTraits<v_uint8>::vlanes();       // u8 output lanes
        constexpr صحیح MAXV8 = VTraits<v_uint8>::max_nlanes;
        T scbuf[MAXV8 * 3] = {};                             // interleaved threshold (elements)
        uchar mbuf[MAXV8 * 3] = {}, vbuf[MAXV8 * 3] = {};    // interleaved mask / value (bytes)
        // ^ the {} inits are for -Wmaybe-uninitialized only: expandScalar/decode fill every lane that
        //   is later read, but the compiler cannot prove it with a runtime VECSZ
        ثابت منطقی bc0 = (s0y == 0);                         // src0 is the broadcast (short) operand
        ثابت T* bsrc = bc0 ? src0 : src1; ثابت size_t bsx = bc0 ? s0x : s1x;
        ثابت T*& src = bc0 ? src1 : src0;                   // the row-stepping operand (advanced below)
        ثابت size_t sy = bc0 ? s1y : s0y;
        // The compare direction (scalar-first when bc0, array-first otherwise) is hoisted OUT of the row
        // loop with an explicit if(bc0) - the loop is written twice so no per-iteration branch remains.
        // Per branch: own unroll, threshold + interleaved M/V loaded ONCE. sizeof==1 keeps 12 vectors.
        #تعریف_کرو EW_CMPR(r, a, b) v_reinterpret_as(Cmp::vec(a, b), r)   // rawmask -> sizeof(T)-byte uint
        اگر constexpr (حجم(T) == 1)                        // 3 u8 masks -> 3 u8 stores
        {
            ثابت صحیح ewidth = VECSZ8 * 3;
            expandScalar(bsrc, bsx, چوڑائی, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, چوڑائی, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, چوڑائی, vbuf, ewidth);
            ثابت صحیح dy = ewidth / چوڑائی;
            Vvec  sc0=vx_بھرو(scbuf),  sc1=vx_بھرو(scbuf+VECSZ),   sc2=vx_بھرو(scbuf+VECSZ*2);
            v_uint8 M0=vx_بھرو(mbuf),  M1=vx_بھرو(mbuf+VECSZ8),    M2=vx_بھرو(mbuf+VECSZ8*2);
            v_uint8 V0=vx_بھرو(vbuf),  V1=vx_بھرو(vbuf+VECSZ8),    V2=vx_بھرو(vbuf+VECSZ8*2);
            #تعریف_کرو EW_ROW1(A0,B0,A1,B1,A2,B2) \
                برائے (; y + dy <= اونچائی; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 o0=v_or(v_and(r0,M0),V0),o1=v_or(v_and(r1,M1),V1),o2=v_or(v_and(r2,M2),V2); \
                    v_store(dst, o0); v_store(dst+VECSZ8, o1); v_store(dst+VECSZ8*2, o2); }
            اگر (bc0) EW_ROW1(sc0,vx_بھرو(src), sc1,vx_بھرو(src+VECSZ), sc2,vx_بھرو(src+VECSZ*2))
            ورنہ     EW_ROW1(vx_بھرو(src),sc0, vx_بھرو(src+VECSZ),sc1, vx_بھرو(src+VECSZ*2),sc2)
            #تعریف_منسوخ EW_ROW1
        }
        ورنہ اگر constexpr (حجم(T) == 2)                   // 3 u16 masks -> 1 full + 1 half u8 store
        {
            ثابت صحیح ewidth = VECSZ * 3;
            expandScalar(bsrc, bsx, چوڑائی, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, چوڑائی, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, چوڑائی, vbuf, ewidth);
            ثابت صحیح dy = ewidth / چوڑائی;
            Vvec  sc0=vx_بھرو(scbuf), sc1=vx_بھرو(scbuf+VECSZ), sc2=vx_بھرو(scbuf+VECSZ*2);
            v_uint8 M0=vx_بھرو(mbuf), M1=vx_بھرو(mbuf+VECSZ8);
            v_uint8 V0=vx_بھرو(vbuf), V1=vx_بھرو(vbuf+VECSZ8);
            #تعریف_کرو EW_ROW2(A0,B0,A1,B1,A2,B2) \
                برائے (; y + dy <= اونچائی; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 b0=v_pack(r0,r1), b1=v_pack(r2,r2); \
                    v_uint8 o0=v_or(v_and(b0,M0),V0), o1=v_or(v_and(b1,M1),V1); \
                    v_store(dst, o0); v_store_کم(dst+VECSZ8, o1); }
            اگر (bc0) EW_ROW2(sc0,vx_بھرو(src), sc1,vx_بھرو(src+VECSZ), sc2,vx_بھرو(src+VECSZ*2))
            ورنہ     EW_ROW2(vx_بھرو(src),sc0, vx_بھرو(src+VECSZ),sc1, vx_بھرو(src+VECSZ*2),sc2)
            #تعریف_منسوخ EW_ROW2
        }
        ورنہ اگر constexpr (حجم(T) == 4)                   // sizeof==4: 6 u32 -> 3 u16 -> 1 full + 1 half u8
        {
            ثابت صحیح ewidth = VECSZ * 6;
            expandScalar(bsrc, bsx, چوڑائی, scbuf, ewidth);
            expandScalar(mvals, (size_t)1, چوڑائی, mbuf, ewidth);
            expandScalar(vvals, (size_t)1, چوڑائی, vbuf, ewidth);
            ثابت صحیح dy = ewidth / چوڑائی;
            Vvec sc0=vx_بھرو(scbuf),sc1=vx_بھرو(scbuf+VECSZ),sc2=vx_بھرو(scbuf+VECSZ*2),
                 sc3=vx_بھرو(scbuf+VECSZ*3),sc4=vx_بھرو(scbuf+VECSZ*4),sc5=vx_بھرو(scbuf+VECSZ*5);
            v_uint8 M0=vx_بھرو(mbuf), M1=vx_بھرو(mbuf+VECSZ8);
            v_uint8 V0=vx_بھرو(vbuf), V1=vx_بھرو(vbuf+VECSZ8);
            #تعریف_کرو EW_ROW4(A0,B0,A1,B1,A2,B2,A3,B3,A4,B4,A5,B5) \
                برائے (; y + dy <= اونچائی; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2,r3,r4,r5; \
                    EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    EW_CMPR(r3,A3,B3); EW_CMPR(r4,A4,B4); EW_CMPR(r5,A5,B5); \
                    v_uint16 p0=v_pack(r0,r1), p1=v_pack(r2,r3), p2=v_pack(r4,r5); \
                    v_uint8 q0=v_pack(p0,p1), q1=v_pack(p2,p2); \
                    v_uint8 o0=v_or(v_and(q0,M0),V0), o1=v_or(v_and(q1,M1),V1); \
                    v_store(dst, o0); v_store_کم(dst+VECSZ8, o1); }
            اگر (bc0) EW_ROW4(sc0,vx_بھرو(src),sc1,vx_بھرو(src+VECSZ),sc2,vx_بھرو(src+VECSZ*2),
                             sc3,vx_بھرو(src+VECSZ*3),sc4,vx_بھرو(src+VECSZ*4),sc5,vx_بھرو(src+VECSZ*5))
            ورنہ     EW_ROW4(vx_بھرو(src),sc0,vx_بھرو(src+VECSZ),sc1,vx_بھرو(src+VECSZ*2),sc2,
                             vx_بھرو(src+VECSZ*3),sc3,vx_بھرو(src+VECSZ*4),sc4,vx_بھرو(src+VECSZ*5),sc5)
            #تعریف_منسوخ EW_ROW4
        }
        #تعریف_منسوخ EW_CMPR
    }
#اگر_ختم
    برائے (; y < اونچائی; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        صحیح x = 0;
    #اگر (CV_SIMD || CV_SIMD_SCALABLE)
        // SIMD for BOTH the contiguous case AND a broadcast operand (a per-channel scalar const has
        // step 0 - without this the scalar-vs-array compare, incl. every multi-channel scalar compare,
        // fell to the scalar tail below, ~5x slower). vx_setall broadcasts the step-0 operand once.
        // 4-vector unroll + halide right-edge backoff (reprocess the last 4*VECSZ when width is not a
        // multiple); dst is a separate mask buffer so the overlap is a harmless idempotent rewrite.
        ثابت صحیح VECSZ = VTraits<Vvec>::vlanes();
        ثابت صحیح VECSZ8 = VTraits<v_uint8>::vlanes();
        v_uint8 vT; v_setall_نقاب(vT, trueVal);              // uniform mask (per-channel patch is short-row)
        ثابت منطقی دم_Trick = چوڑائی >= 4*VECSZ && src0_ != dst_ && src1_ != dst_;
        // Narrow the 4 masks to u8 FIRST, then apply trueVal on the u8 result (1 and per u8 vector, not
        // per wide lane-group). One block per MASK lane width (== sizeof(T) natively, but 4 for the
        // f16/bf16->f32 widened path); a 4-vector unroll spans exactly 4*VECSZ elements.
        constexpr size_t MW = حجم(قسم_نام VTraits<Uvec>::lane_type);
        #تعریف_کرو PACK_STORE_CMP_نتیجہ(m0,m1,m2,m3, D) کرو { \
            اگر constexpr (MW == 1u) { \
                v_uint8 o0=v_and(m0,vT),o1=v_and(m1,vT),o2=v_and(m2,vT),o3=v_and(m3,vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); v_store((D)+VECSZ8*2,o2); v_store((D)+VECSZ8*3,o3); \
            } ورنہ اگر constexpr (MW == 2u) { \
                v_uint8 o0=v_and(v_pack(m0,m1),vT), o1=v_and(v_pack(m2,m3),vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); \
            } ورنہ اگر constexpr (MW == 4u) { \
                v_uint8 o0=v_and(v_pack(v_pack(m0,m1),v_pack(m2,m3)),vT); \
                v_store((D),o0); \
            } ورنہ { /* sizeof==8: 4 u64 -> 2 u32 -> 1 u16 -> u8 (low half), 4*VECSZ elems -> store_low */ \
                v_uint16 g=v_pack(v_pack(m0,m1),v_pack(m2,m3)); \
                v_uint8 o0=v_and(v_pack(g,g),vT); \
                v_store_کم((D),o0); \
            } } جب(0)
        اگر (s0x == 1u && s1x == 1u)
        {
            برائے (; x < چوڑائی; x += 4*VECSZ)
            {
                اگر (x + 4*VECSZ > چوڑائی) { اگر (!دم_Trick) توڑو; x = چوڑائی - 4*VECSZ; }
                Vvec a0, a1, a2, a3, b0, b1, b2, b3;   // vx_load_pair_as widens f16/bf16->f32, else native
                vx_بھرو_pair_as(src0+x, a0, a1); vx_بھرو_pair_as(src0+x+2*VECSZ, a2, a3);
                vx_بھرو_pair_as(src1+x, b0, b1); vx_بھرو_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b1), m1);
                v_reinterpret_as(Cmp::vec(a2, b2), m2); v_reinterpret_as(Cmp::vec(a3, b3), m3);
                PACK_STORE_CMP_نتیجہ(m0, m1, m2, m3, dst+x);
            }
        }
        ورنہ اگر (s1x == 0u)                       // src1 (e.g. the scalar) broadcast
        {
            Vvec b0; vx_setall_as(src1, b0);
            برائے (; x < چوڑائی; x += 4*VECSZ)
            {
                اگر (x + 4*VECSZ > چوڑائی) { اگر (!دم_Trick) توڑو; x = چوڑائی - 4*VECSZ; }
                Vvec a0, a1, a2, a3;
                vx_بھرو_pair_as(src0+x, a0, a1); vx_بھرو_pair_as(src0+x+2*VECSZ, a2, a3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b0), m1);
                v_reinterpret_as(Cmp::vec(a2, b0), m2); v_reinterpret_as(Cmp::vec(a3, b0), m3);
                PACK_STORE_CMP_نتیجہ(m0, m1, m2, m3, dst+x);
            }
        }
        ورنہ اگر (s0x == 0u)                       // src0 broadcast
        {
            Vvec a0; vx_setall_as(src0, a0);
            برائے (; x < چوڑائی; x += 4*VECSZ)
            {
                اگر (x + 4*VECSZ > چوڑائی) { اگر (!دم_Trick) توڑو; x = چوڑائی - 4*VECSZ; }
                Vvec b0, b1, b2, b3;
                vx_بھرو_pair_as(src1+x, b0, b1); vx_بھرو_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a0, b1), m1);
                v_reinterpret_as(Cmp::vec(a0, b2), m2); v_reinterpret_as(Cmp::vec(a0, b3), m3);
                PACK_STORE_CMP_نتیجہ(m0, m1, m2, m3, dst+x);
            }
        }
        #تعریف_منسوخ PACK_STORE_CMP_نتیجہ
    #اگر_ختم
        // Fold the fix-up: raw all-ones/zero mask -> (raw & M) | V. Uniform => raw & trueVal (V=0); a
        // per-channel patch (width=cn<=4 here) indexes M/V by the channel x.
        #تعریف_کرو EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                             : (uchar)((cond) ? trueVal : 0))
        اگر (s0x == s1x) {
            برائے (; x < چوڑائی; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], src1[x]), x);
        }
        ورنہ اگر (s0x == 0) {
            T a = src0[0];
            برائے (; x < چوڑائی; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, src1[x]), x);
        }
        ورنہ {
            T b = src1[0];
            برائے (; x < چوڑائی; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], b), x);
        }
        #تعریف_منسوخ EW_CMP_APPLY
    }
    واپس 0;
}

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
سانچہ<قسم_نام T, قسم_نام Tr, قسم_نام Wvec, قسم_نام WT, جماعت Op, قسم_نام ST=WT, قسم_نام Wvec1=Wvec>
ساکن صحیح vec_ثنائی_Kernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                         ثابت خالی* src1_, size_t s1y, size_t s1x,
                         ثابت خالی*, size_t, size_t,
                         خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                         ثابت دوہرا* params, صحیح, خالی*)
{
    اگر (چوڑائی <= 0 || اونچائی <= 0) واپس 0;
    s0y /= حجم(T);
    s1y /= حجم(T);
    dsty /= حجم(Tr);

    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)چوڑائی == 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    صحیح y = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    استعمال Wlane = قسم_نام VTraits<Wvec>::lane_type;
    ثابت صحیح VECSZ = VTraits<Wvec>::vlanes();
    ثابت منطقی use_دم_trick = چوڑائی >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    [[maybe_unused]] Wvec vscalar;
    vx_setall_as(&scalar, vscalar);
    اگر (اونچائی > 1 && چوڑائی <= 4 &&
        ((s0y == 0 && s1y == چوڑائی*s1x) ||
         (s1y == 0 && s0y == چوڑائی*s0x)) &&
        dsty == (size_t)چوڑائی) {
        constexpr صحیح MAXVECSZ = VTraits<Wvec>::max_nlanes;
        Wlane scbuf[MAXVECSZ*6] = {};   // {} for -Wmaybe-uninitialized only (filled up to VECSZ*6)
        ثابت صحیح ewidth = VECSZ*6;
        expandScalar(s0y == 0 ? src0 : src1, s0y == 0 ? s0x : s1x, چوڑائی, scbuf, ewidth);
        صحیح dy = ewidth / چوڑائی;
        Wvec sc0, sc1, sc2, sc3, sc4, sc5;
        sc0 = Op::preproc(vx_بھرو(scbuf), vscalar);
        sc1 = Op::preproc(vx_بھرو(scbuf + VECSZ), vscalar);
        sc2 = Op::preproc(vx_بھرو(scbuf + VECSZ*2), vscalar);
        sc3 = Op::preproc(vx_بھرو(scbuf + VECSZ*3), vscalar);
        sc4 = Op::preproc(vx_بھرو(scbuf + VECSZ*4), vscalar);
        sc5 = Op::preproc(vx_بھرو(scbuf + VECSZ*5), vscalar);

        اگر (s0y == 0) {
            برائے (; y + dy <= اونچائی; y += dy, src1 += s1y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_بھرو_pair_as(src1, v0, v1);
                vx_بھرو_pair_as(src1 + VECSZ*2, v2, v3);
                vx_بھرو_pair_as(src1 + VECSZ*4, v4, v5);
                خودکار w0 = Op::vec(sc0, v0, vscalar), w1 = Op::vec(sc1, v1, vscalar),
                     w2 = Op::vec(sc2, v2, vscalar), w3 = Op::vec(sc3, v3, vscalar),
                     w4 = Op::vec(sc4, v4, vscalar), w5 = Op::vec(sc5, v5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
        ورنہ {
            برائے (; y + dy <= اونچائی; y += dy, src0 += s0y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_بھرو_pair_as(src0, v0, v1);
                vx_بھرو_pair_as(src0 + VECSZ*2, v2, v3);
                vx_بھرو_pair_as(src0 + VECSZ*4, v4, v5);
                خودکار w0 = Op::vec(v0, sc0, vscalar), w1 = Op::vec(v1, sc1, vscalar),
                     w2 = Op::vec(v2, sc2, vscalar), w3 = Op::vec(v3, sc3, vscalar),
                     w4 = Op::vec(v4, sc4, vscalar), w5 = Op::vec(v5, sc5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
    }
#اگر_ختم
    برائے (; y < اونچائی; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        صحیح x = 0;
    #اگر (CV_SIMD || CV_SIMD_SCALABLE)
        Wvec a0, a1, a2, a3, b0, b1, b2, b3;
        اگر (s0x == s1x) {
            اگر (!Op::useScalar || scalar == ST(1)) {
                // this branch runs on Wvec1, which may be WIDER-laned than Wvec (e.g. the u8 mul
                // path: Wvec1=v_uint16 is 2x the lanes of Wvec=v_float32) - step by ITS lane count,
                // or the pairs overlap and the tail backoff writes past the row end
                ثابت صحیح VECSZ1 = VTraits<Wvec1>::vlanes();
                ثابت منطقی دم_trick1 = چوڑائی >= VECSZ1*4 && src0_ != dst_ && src1_ != dst_;
                برائے (; x < چوڑائی; x += VECSZ1*4) {
                    Wvec1 a0_, a1_, a2_, a3_, b0_, b1_, b2_, b3_;
                    اگر (x + VECSZ1*4 > چوڑائی) { اگر (!دم_trick1) توڑو; x = چوڑائی - VECSZ1*4; }
                    vx_بھرو_pair_as(src0 + x, a0_, a1_);
                    vx_بھرو_pair_as(src0 + x + VECSZ1*2, a2_, a3_);
                    vx_بھرو_pair_as(src1 + x, b0_, b1_);
                    vx_بھرو_pair_as(src1 + x + VECSZ1*2, b2_, b3_);
                    خودکار c0 = Op::vec(a0_, b0_), c1 = Op::vec(a1_, b1_),
                         c2 = Op::vec(a2_, b2_), c3 = Op::vec(a3_, b3_);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ1*2, c2, c3);
                }
            }
            ورنہ {
                برائے (; x < چوڑائی; x += VECSZ*4) {
                    اگر (x + VECSZ*4 > چوڑائی) { اگر (!use_دم_trick) توڑو; x = چوڑائی - VECSZ*4; }
                    vx_بھرو_pair_as(src0 + x, a0, a1);
                    vx_بھرو_pair_as(src0 + x + VECSZ*2, a2, a3);
                    vx_بھرو_pair_as(src1 + x, b0, b1);
                    vx_بھرو_pair_as(src1 + x + VECSZ*2, b2, b3);
                    خودکار c0 = Op::vec(Op::preproc(a0, vscalar), b0, vscalar),
                         c1 = Op::vec(Op::preproc(a1, vscalar), b1, vscalar),
                         c2 = Op::vec(Op::preproc(a2, vscalar), b2, vscalar),
                         c3 = Op::vec(Op::preproc(a3, vscalar), b3, vscalar);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ*2, c2, c3);
                }
            }
        }
        ورنہ اگر (s1x == 0) {
            vx_setall_as(src1, b0);
            b0 = Op::preproc(b0, vscalar);
            برائے (; x < چوڑائی; x += VECSZ*4) {
                اگر (x + VECSZ*4 > چوڑائی) { اگر (!use_دم_trick) توڑو; x = چوڑائی - VECSZ*4; }
                vx_بھرو_pair_as(src0 + x, a0, a1);
                vx_بھرو_pair_as(src0 + x + VECSZ*2, a2, a3);
                خودکار c0 = Op::vec(a0, b0, vscalar), c1 = Op::vec(a1, b0, vscalar),
                     c2 = Op::vec(a2, b0, vscalar), c3 = Op::vec(a3, b0, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
        ورنہ {
            vx_setall_as(src0, b0);
            b0 = Op::preproc(b0, vscalar);
            برائے (; x < چوڑائی; x += VECSZ*4) {
                اگر (x + VECSZ*4 > چوڑائی) { اگر (!use_دم_trick) توڑو; x = چوڑائی - VECSZ*4; }
                vx_بھرو_pair_as(src1 + x, a0, a1);
                vx_بھرو_pair_as(src1 + x + VECSZ*2, a2, a3);
                خودکار c0 = Op::vec(b0, a0, vscalar), c1 = Op::vec(b0, a1, vscalar),
                     c2 = Op::vec(b0, a2, vscalar), c3 = Op::vec(b0, a3, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
    #اگر_ختم
        برائے (; x < چوڑائی; x++)
            dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x*s0x], (WT)src1[x*s1x], scalar));
    }
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    vx_cleanup();
#اگر_ختم
    واپس 0;
}

// ===========================================================================
// OP_ADD
// ===========================================================================

// add dispatch: T x T -> R (operands already same depth T).
//  - native saturating path (Wvec = native, use_simd=true) for T -> T on <=32-bit ints + f32
//    (32-bit lanes via the local v_add_sat/v_sub_sat - v_add/v_sub wrap there);
//  - widening f32-hub (Wvec = v_float32, use_simd=true) for the widened/half/float outputs;
//  - scalar (use_simd=false) for 64-bit results and f64 (no vector path yet).
سانچہ<جماعت Op>
TKernel get_جوڑو_Sub_Func(صحیح T, صحیح R)
{
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (T)
    {
    حالت CV_8U:
        fptr = R == CV_8U ? vec_ثنائی_Kernel<uchar, uchar, v_uint8, مختصر, Op, uchar> :
               R == CV_16S ? vec_ثنائی_Kernel<uchar, مختصر, v_int16, مختصر, Op, مختصر> :
               R == CV_32S ? vec_ثنائی_Kernel<uchar, صحیح, v_int16, مختصر, Op, مختصر> :
               R == CV_32F ? vec_ثنائی_Kernel<uchar, اعشاری, v_int16, مختصر, Op, مختصر> : خالی_اشاریہ;
        توڑو;
    حالت CV_8S:
        fptr =
            R == CV_8S ? vec_ثنائی_Kernel<schar, schar, v_int8, مختصر, Op, schar> :
            R == CV_16S ? vec_ثنائی_Kernel<schar, مختصر, v_int16, مختصر, Op, مختصر> :
            R == CV_32S ? vec_ثنائی_Kernel<schar, صحیح, v_int16, مختصر, Op, مختصر> :
            R == CV_32F ? vec_ثنائی_Kernel<schar, اعشاری, v_int16, مختصر, Op, مختصر> : خالی_اشاریہ;
        توڑو;
    حالت CV_16U:
        fptr = R == CV_16U ? vec_ثنائی_Kernel<ushort, ushort, v_uint16, صحیح, Op, ushort> :
               R == CV_32S ? vec_ثنائی_Kernel<ushort, صحیح, v_int32, صحیح, Op, صحیح> :
               R == CV_32F ? vec_ثنائی_Kernel<ushort, اعشاری, v_int32, صحیح, Op, صحیح> : خالی_اشاریہ;
        توڑو;
    حالت CV_16S:
        fptr = R == CV_16S ? vec_ثنائی_Kernel<مختصر, مختصر, v_int16, صحیح, Op, مختصر> :
               R == CV_32S ? vec_ثنائی_Kernel<مختصر, صحیح,   v_int32, صحیح, Op, صحیح> :
               R == CV_32F ? vec_ثنائی_Kernel<مختصر, اعشاری, v_int32, صحیح, Op, صحیح> : خالی_اشاریہ;
        توڑو;
    حالت CV_32U:
        fptr = R == CV_32U ? vec_ثنائی_Kernel<غیرعلامتی, غیرعلامتی, v_uint32, int64_t, Op, غیرعلامتی> :
                R == CV_64S ? scalar_ثنائی_Kernel<غیرعلامتی, int64_t, int64_t, Op> :
                R == CV_64F ? scalar_ثنائی_Kernel<غیرعلامتی, دوہرا, int64_t, Op> : خالی_اشاریہ;
        توڑو;
    حالت CV_32S:
        fptr = R == CV_32S ? vec_ثنائی_Kernel<صحیح, صحیح, v_int32, int64_t, Op, صحیح> :
               R == CV_64S ? scalar_ثنائی_Kernel<صحیح, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_ثنائی_Kernel<صحیح, دوہرا, int64_t, Op> : خالی_اشاریہ;
        توڑو;
    حالت CV_64U:
        fptr = R == CV_64U ? scalar_ثنائی_Kernel<uint64_t, uint64_t, uint64_t, Op> :
               R == CV_64F ? scalar_ثنائی_Kernel<uint64_t, دوہرا, دوہرا, Op> : خالی_اشاریہ;
        توڑو;
    حالت CV_64S:
        fptr = R == CV_64S ? scalar_ثنائی_Kernel<int64_t, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_ثنائی_Kernel<int64_t, دوہرا, دوہرا, Op> : خالی_اشاریہ;
        توڑو;
    حالت CV_16F:
        fptr =
        #اگر CV_SIMD_16F
        R == CV_16F ? vec_ثنائی_Kernel<hfloat, hfloat, v_float16, اعشاری, Op, hfloat> :
        #ورنہ
        R == CV_16F ? vec_ثنائی_Kernel<hfloat, hfloat, v_float32, اعشاری, Op, اعشاری> :
        #اگر_ختم
        R == CV_32F ? vec_ثنائی_Kernel<hfloat, اعشاری, v_float32, اعشاری, Op, اعشاری> : خالی_اشاریہ;
        توڑو;
    حالت CV_16BF:
        fptr =
            R == CV_16BF ? vec_ثنائی_Kernel<bfloat, bfloat, v_float32, اعشاری, Op, اعشاری> :
            R == CV_32F ? vec_ثنائی_Kernel<bfloat, اعشاری,  v_float32, اعشاری, Op, اعشاری> : خالی_اشاریہ;
        توڑو;
    حالت CV_32F:
        fptr = R == CV_32F ? vec_ثنائی_Kernel<اعشاری, اعشاری, v_float32, اعشاری, Op, اعشاری> : خالی_اشاریہ;
        توڑو;
    حالت CV_64F:
        #اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_ثنائی_Kernel<دوہرا, دوہرا, v_float64, دوہرا, Op> : خالی_اشاریہ;
        #ورنہ
        fptr = R == CV_64F ? scalar_ثنائی_Kernel<دوہرا, دوہرا, دوہرا, Op> : خالی_اشاریہ;
        #اگر_ختم
        توڑو;
    طےشدہ:
        ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
}

TKernel getMulFunc_(صحیح T, صحیح R)
{
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (T)
    {
    حالت CV_8U:
        fptr =   // scale==1 fast path: whole u8 registers, v_mul_sat widens+clamps inside; Wvec=f32 for scale
            R == CV_8U ? vec_ثنائی_Kernel<uchar, uchar, v_float32, اعشاری, EwMul, اعشاری, v_uint8> :
            R == CV_32F ? vec_ثنائی_Kernel<uchar, اعشاری, v_float32, اعشاری, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_8S:
        fptr =   // scale==1 fast path: whole s8 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_8S ? vec_ثنائی_Kernel<schar, schar, v_float32, اعشاری, EwMul, اعشاری, v_int8> :
            R == CV_32F ? vec_ثنائی_Kernel<schar, اعشاری, v_float32, اعشاری, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_16U:
        fptr =   // scale==1 fast path: whole u16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16U ? vec_ثنائی_Kernel<ushort, ushort, v_float32, اعشاری, EwMul, اعشاری, v_uint16> :
            R == CV_32F ? vec_ثنائی_Kernel<ushort, اعشاری,  v_float32, اعشاری, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_16S:
        fptr =   // scale==1 fast path: whole s16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16S ? vec_ثنائی_Kernel<مختصر, مختصر, v_float32, اعشاری, EwMul, اعشاری, v_int16> :
            R == CV_32F ? vec_ثنائی_Kernel<مختصر, اعشاری, v_float32, اعشاری, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_16F:
        fptr =
            R == CV_16F ? vec_ثنائی_Kernel<hfloat, hfloat, v_float32, اعشاری, EwMul> :
            R == CV_32F ? vec_ثنائی_Kernel<hfloat, اعشاری,  v_float32, اعشاری, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_16BF:
        fptr =
            R == CV_16BF ? vec_ثنائی_Kernel<bfloat, bfloat, v_float32, اعشاری, EwMul> :
            R == CV_32F ? vec_ثنائی_Kernel<bfloat, اعشاری,  v_float32, اعشاری, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_32F:
        fptr = R == CV_32F ? vec_ثنائی_Kernel<اعشاری, اعشاری, v_float32, اعشاری, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_32U:
        fptr =   // scale==1 fast path (where v_mul_sat32 exists): whole u32 registers, widening
                 // multiply + saturating narrow; the f64 work vector serves the scale path
        #اگر defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32U ? vec_ثنائی_Kernel<غیرعلامتی, غیرعلامتی, v_float64, دوہرا, EwMul, دوہرا, v_uint32> :
            R == CV_64F ? vec_ثنائی_Kernel<غیرعلامتی, دوہرا,   v_float64, دوہرا, EwMul> : خالی_اشاریہ;
        #وگرنہ_اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32U ? vec_ثنائی_Kernel<غیرعلامتی, غیرعلامتی, v_float64, دوہرا, EwMul> :
            R == CV_64F ? vec_ثنائی_Kernel<غیرعلامتی, دوہرا,   v_float64, دوہرا, EwMul> : خالی_اشاریہ;
        #ورنہ
            R == CV_32U ? scalar_ثنائی_Kernel<غیرعلامتی, غیرعلامتی, دوہرا, EwMul> :
            R == CV_64F ? scalar_ثنائی_Kernel<غیرعلامتی, دوہرا,   دوہرا, EwMul> : خالی_اشاریہ;
        #اگر_ختم
        توڑو;
    حالت CV_32S:
        fptr =   // scale==1 fast path: see CV_32U
        #اگر defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32S ? vec_ثنائی_Kernel<صحیح, صحیح,    v_float64, دوہرا, EwMul, دوہرا, v_int32> :
            R == CV_64F ? vec_ثنائی_Kernel<صحیح, دوہرا, v_float64, دوہرا, EwMul> : خالی_اشاریہ;
        #وگرنہ_اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32S ? vec_ثنائی_Kernel<صحیح, صحیح,    v_float64, دوہرا, EwMul> :
            R == CV_64F ? vec_ثنائی_Kernel<صحیح, دوہرا, v_float64, دوہرا, EwMul> : خالی_اشاریہ;
        #ورنہ
            R == CV_32S ? scalar_ثنائی_Kernel<صحیح, صحیح,    دوہرا, EwMul> :
            R == CV_64F ? scalar_ثنائی_Kernel<صحیح, دوہرا, دوہرا, EwMul> : خالی_اشاریہ;
        #اگر_ختم
        توڑو;
    حالت CV_64U:
        fptr = R == CV_64F ? scalar_ثنائی_Kernel<uint64_t, دوہرا, دوہرا, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_64S:
        fptr = R == CV_64F ? scalar_ثنائی_Kernel<int64_t,  دوہرا, دوہرا, EwMul> : خالی_اشاریہ;
        توڑو;
    حالت CV_64F:
        #اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_ثنائی_Kernel<دوہرا, دوہرا, v_float64, دوہرا, EwMul> : خالی_اشاریہ;
        #ورنہ
        fptr = R == CV_64F ? scalar_ثنائی_Kernel<دوہرا,   دوہرا, دوہرا, EwMul> : خالی_اشاریہ;
        #اگر_ختم
        توڑو;
    طےشدہ:
        ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
}

// `checked` (decided by the CALLER from the ORIGINAL input types) selects the divide-by-zero
// policy, INDEPENDENT of the work type R: EwDivInt guards /0 -> 0 for integer-semantics division,
// EwDivFlt does not (a/0 -> inf, saturating on a later cast, as cv::divide does for float inputs).
// It must apply on EVERY row - two wide integers (e.g. 32U / 64S) promote to a 64F work type yet
// still need the integer guard, so the float-work rows can't hardcode EwDivFlt.
TKernel getDivFunc_(صحیح T, صحیح R, منطقی checked)
{
    KernelFunc fptr = خالی_اشاریہ;
    // SIMD via vecBinaryKernel: div scales the numerator in vec (a*scale/b), EwDivFlt (float a/0 -> inf,
    // saturates on cast) / EwDivInt (v_select guards b==0 -> 0 post-facto). Prefer a DIRECT T->T kernel
    // (1 pass, saturate on store) - emitBinary probes result==T first; a T->f32/f64 kernel serves an
    // explicit float dtype. Work type: f32 for <=16-bit/f16/bf16/f32; f64 for 32/64-bit.
    #تعریف_کرو DIV(T_, Tr_, Wv_, W_) (checked ? vec_ثنائی_Kernel<T_, Tr_, Wv_, W_, EwDivInt> \
                                           : vec_ثنائی_Kernel<T_, Tr_, Wv_, W_, EwDivFlt>)
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #تعریف_کرو DIVW(T_, Tr_) DIV(T_, Tr_, v_float64, دوہرا)                 // 32/64-bit, f64 SIMD
#ورنہ
    #تعریف_کرو DIVW(T_, Tr_) (checked ? scalar_ثنائی_Kernel<T_, Tr_, دوہرا, EwDivInt> \
                                    : scalar_ثنائی_Kernel<T_, Tr_, دوہرا, EwDivFlt>)
#اگر_ختم
    انتخاب (T)
    {
    حالت CV_8U:   fptr = R==CV_8U  ? DIV(uchar,  uchar,  v_float32, اعشاری) : R==CV_32F ? DIV(uchar,  اعشاری, v_float32, اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_8S:   fptr = R==CV_8S  ? DIV(schar,  schar,  v_float32, اعشاری) : R==CV_32F ? DIV(schar,  اعشاری, v_float32, اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_16U:  fptr = R==CV_16U ? DIV(ushort, ushort, v_float32, اعشاری) : R==CV_32F ? DIV(ushort, اعشاری, v_float32, اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_16S:  fptr = R==CV_16S ? DIV(مختصر,  مختصر,  v_float32, اعشاری) : R==CV_32F ? DIV(مختصر,  اعشاری, v_float32, اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_16F:  fptr = R==CV_16F ? DIV(hfloat, hfloat, v_float32, اعشاری) : R==CV_32F ? DIV(hfloat, اعشاری, v_float32, اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_16BF: fptr = R==CV_16BF? DIV(bfloat, bfloat, v_float32, اعشاری) : R==CV_32F ? DIV(bfloat, اعشاری, v_float32, اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_32F:  fptr = R==CV_32F ? DIV(اعشاری,  اعشاری,  v_float32, اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_32U:  fptr = R==CV_32U ? DIVW(غیرعلامتی, غیرعلامتی) : R==CV_64F ? DIVW(غیرعلامتی, دوہرا) : خالی_اشاریہ; توڑو;
    حالت CV_32S:  fptr = R==CV_32S ? DIVW(صحیح,      صحیح)      : R==CV_64F ? DIVW(صحیح,      دوہرا) : خالی_اشاریہ; توڑو;
    حالت CV_64U:  fptr = R==CV_64F ? DIVW(uint64_t, دوہرا) : خالی_اشاریہ; توڑو;
    حالت CV_64S:  fptr = R==CV_64F ? DIVW(int64_t,  دوہرا) : خالی_اشاریہ; توڑو;
    حالت CV_64F:  fptr = R==CV_64F ? DIVW(دوہرا,   دوہرا) : خالی_اشاریہ; توڑو;
    طےشدہ:      ;
    }
    #تعریف_منسوخ DIV
    #تعریف_منسوخ DIVW
    واپس {fptr, خالی_اشاریہ, 0};
}


// min / max: T x T -> T for every depth. Native v_min/v_max on the matching lane type (8/16/32-bit
// ints, f16/bf16/f32); 64-bit ints use the scalar path. Op = EwMin or EwMax.
سانچہ<جماعت Op>
ساکن TKernel getMinMaxFunc(صحیح T)
{
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (T)
    {
    حالت CV_8U:   fptr = vec_ثنائی_Kernel<uchar,    uchar,    v_uint8,   مختصر,   Op, uchar>; توڑو;
    حالت CV_8S:   fptr = vec_ثنائی_Kernel<schar,    schar,    v_int8,    مختصر,   Op, schar>; توڑو;
    حالت CV_16U:  fptr = vec_ثنائی_Kernel<ushort,   ushort,   v_uint16,  صحیح,     Op, ushort>; توڑو;
    حالت CV_16S:  fptr = vec_ثنائی_Kernel<مختصر,    مختصر,    v_int16,   صحیح,     Op, مختصر>; توڑو;
    حالت CV_32U:  fptr = vec_ثنائی_Kernel<غیرعلامتی, غیرعلامتی, v_uint32,  int64_t, Op, غیرعلامتی>; توڑو;
    حالت CV_32S:  fptr = vec_ثنائی_Kernel<صحیح,      صحیح,      v_int32,   int64_t, Op, صحیح>; توڑو;
    حالت CV_16F:
        #اگر CV_SIMD_16F
        fptr = vec_ثنائی_Kernel<hfloat,   hfloat,   v_float16, اعشاری,   Op, hfloat>;
        #ورنہ
        fptr = vec_ثنائی_Kernel<hfloat,   hfloat,   v_float32, اعشاری,   Op, اعشاری>;
        #اگر_ختم
        توڑو;
    حالت CV_16BF: fptr = vec_ثنائی_Kernel<bfloat, bfloat, v_float32, اعشاری, Op, اعشاری>; توڑو;
    حالت CV_32F:  fptr = vec_ثنائی_Kernel<اعشاری, اعشاری, v_float32, اعشاری, Op, اعشاری>; توڑو;
    حالت CV_64U:  fptr = scalar_ثنائی_Kernel<uint64_t, uint64_t, uint64_t, Op>; توڑو;
    حالت CV_64S:  fptr = scalar_ثنائی_Kernel<int64_t,  int64_t,  int64_t,  Op>; توڑو;
    #اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  fptr = vec_ثنائی_Kernel<دوہرا, دوہرا, v_float64, دوہرا, Op, دوہرا>; توڑو;
    #ورنہ
    حالت CV_64F:  fptr = scalar_ثنائی_Kernel<دوہرا,   دوہرا,   دوہرا,   Op>; توڑو;
    #اگر_ختم
    طےشدہ:      ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
}

// absdiff: |a-b|, T x T -> R.
TKernel getAbsdiffFunc_(صحیح T, صحیح R)
{
    // Signed T has TWO outputs: R==T -> EwAbsdiffS (saturating |a-b| kept in the signed range, native work,
    // ONE pass - the depth cv::absdiff keeps); R==(the unsigned type of the same width) -> EwAbsdiff (the
    // true |a-b| via v_absdiff, for an explicit unsigned dst). Unsigned/float T only produce R==T. Each case
    // returns nullptr for a wrong R (no separate rdepth guard).
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (T)
    {
    حالت CV_8U:   fptr = R == CV_8U  ? vec_ثنائی_Kernel<uchar,  uchar,  v_uint8,  مختصر,   EwAbsdiff, uchar>  : خالی_اشاریہ; توڑو;
    حالت CV_16U:  fptr = R == CV_16U ? vec_ثنائی_Kernel<ushort, ushort, v_uint16, صحیح,     EwAbsdiff, ushort> : خالی_اشاریہ; توڑو;
    حالت CV_32U:  fptr = R == CV_32U ? vec_ثنائی_Kernel<غیرعلامتی, غیرعلامتی, v_uint32, int64_t, EwAbsdiff, غیرعلامتی> : خالی_اشاریہ; توڑو;
    حالت CV_8S:   fptr = R == CV_8S  ? vec_ثنائی_Kernel<schar, schar, v_int8,  مختصر,   EwAbsdiffS, schar> :
                         R == CV_8U  ? vec_ثنائی_Kernel<schar, uchar, v_int8,  مختصر,   EwAbsdiff,  schar> : خالی_اشاریہ; توڑو;
    حالت CV_16S:  fptr = R == CV_16S ? vec_ثنائی_Kernel<مختصر, مختصر, v_int16, صحیح,     EwAbsdiffS, مختصر> :
                         R == CV_16U ? vec_ثنائی_Kernel<مختصر, ushort, v_int16, صحیح,     EwAbsdiff,  مختصر> : خالی_اشاریہ; توڑو;
    حالت CV_32S:  fptr = R == CV_32S ? vec_ثنائی_Kernel<صحیح,   صحیح,   v_int32, int64_t, EwAbsdiffS, صحیح> :
                         R == CV_32U ? vec_ثنائی_Kernel<صحیح, غیرعلامتی, v_int32, int64_t, EwAbsdiff,  صحیح> : خالی_اشاریہ; توڑو;
    #اگر CV_SIMD_16F
    حالت CV_16F:  fptr = R == CV_16F ? vec_ثنائی_Kernel<hfloat, hfloat, v_float16, اعشاری, EwAbsdiff, hfloat> : خالی_اشاریہ; توڑو;
    #ورنہ
    حالت CV_16F:  fptr = R == CV_16F ? vec_ثنائی_Kernel<hfloat, hfloat, v_float32, اعشاری, EwAbsdiff, اعشاری>  : خالی_اشاریہ; توڑو;
    #اگر_ختم
    حالت CV_16BF: fptr = R == CV_16BF ? vec_ثنائی_Kernel<bfloat, bfloat, v_float32, اعشاری, EwAbsdiff, اعشاری> : خالی_اشاریہ; توڑو;
    حالت CV_32F:  fptr = R == CV_32F  ? vec_ثنائی_Kernel<اعشاری,  اعشاری,  v_float32, اعشاری, EwAbsdiff, اعشاری> : خالی_اشاریہ; توڑو;
    حالت CV_64U:  fptr = R == CV_64U  ? scalar_ثنائی_Kernel<uint64_t, uint64_t, uint64_t, EwAbsdiff> : خالی_اشاریہ; توڑو;
    حالت CV_64S:  fptr = R == CV_64S  ? scalar_ثنائی_Kernel<int64_t,  int64_t,  int64_t,  EwAbsdiffS> :
                         R == CV_64U  ? scalar_ثنائی_Kernel<int64_t,  uint64_t, int64_t,  EwAbsdiff> : خالی_اشاریہ; توڑو;
    #اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  fptr = R == CV_64F  ? vec_ثنائی_Kernel<دوہرا, دوہرا, v_float64, دوہرا, EwAbsdiff, دوہرا> : خالی_اشاریہ; توڑو;
    #ورنہ
    حالت CV_64F:  fptr = R == CV_64F  ? scalar_ثنائی_Kernel<دوہرا, دوہرا, دوہرا, EwAbsdiff> : خالی_اشاریہ; توڑو;
    #اگر_ختم
    طےشدہ:      ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
}

// compare: T x T -> u8 mask. Directly-comparable depths (u8/s8/u16/s16/u32/s32/f32) take the SIMD
// vecCompareKernel; f16/bf16 and 64-bit int depths fall back to scalarCompareKernel. The returned kernel
// defaults to a 255 mask in TKernel::flags (cv::compare-compatible); kernel.flags=1 gives a 0/1 mask.
سانچہ<جماعت Cmp>
ساکن KernelFunc compareByType(صحیح T)
{
    انتخاب (T)
    {
    حالت CV_8U:   واپس vecCompareKernel<uchar,    v_uint8,   v_uint8,  Cmp>;
    حالت CV_8S:   واپس vecCompareKernel<schar,    v_int8,    v_uint8,  Cmp>;
    حالت CV_16U:  واپس vecCompareKernel<ushort,   v_uint16,  v_uint16, Cmp>;
    حالت CV_16S:  واپس vecCompareKernel<مختصر,    v_int16,   v_uint16, Cmp>;
    حالت CV_32U:  واپس vecCompareKernel<غیرعلامتی, v_uint32,  v_uint32, Cmp>;
    حالت CV_32S:  واپس vecCompareKernel<صحیح,      v_int32,   v_uint32, Cmp>;
    حالت CV_32F:  واپس vecCompareKernel<اعشاری,    v_float32, v_uint32, Cmp>;
    #اگر CV_SIMD_16F
    حالت CV_16F:  واپس vecCompareKernel<hfloat,   v_float16, v_uint16, Cmp>;
    #ورنہ
    حالت CV_16F:  واپس vecCompareKernel<hfloat,   v_float32, v_uint32, Cmp>;   // widen f16->f32
    #اگر_ختم
    حالت CV_16BF: واپس vecCompareKernel<bfloat,   v_float32, v_uint32, Cmp>;   // widen bf16->f32 (no native)
    حالت CV_64U:  واپس scalarCompareKernel<uint64_t, uint64_t, Cmp>;
    حالت CV_64S:  واپس scalarCompareKernel<int64_t,  int64_t,  Cmp>;
    #اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  واپس vecCompareKernel<دوہرا,   v_float64, v_uint64, Cmp>;
    #ورنہ
    حالت CV_64F:  واپس scalarCompareKernel<دوہرا,   دوہرا,   Cmp>;
    #اگر_ختم
    طےشدہ:      واپس خالی_اشاریہ;
    }
}

TKernel getCmpFunc_(چوٹی op, صحیح T)
{
    // Only 4 physical kernels (eq/ne/gt/ge): LT/LE reuse GT/GE with the operands swapped
    // (a<b == b>a, a<=b == b>=a) via the EW_KERNEL_SWAP01 flag, honored by the executor.
    KernelFunc f = خالی_اشاریہ;
    صحیح flags = 0;              // mask value: 0 flag bits => 0/255 (cv::compare); EW_KERNEL_MASK1 => 0/1
    انتخاب (op)
    {
    حالت OP_CMP_EQ: f = compareByType<EwCmpEq>(T); توڑو;
    حالت OP_CMP_NE: f = compareByType<EwCmpNe>(T); توڑو;
    حالت OP_CMP_GT: f = compareByType<EwCmpGt>(T); توڑو;
    حالت OP_CMP_GE: f = compareByType<EwCmpGe>(T); توڑو;
    حالت OP_CMP_LT: f = compareByType<EwCmpGt>(T); flags = EW_KERNEL_SWAP01; توڑو;
    حالت OP_CMP_LE: f = compareByType<EwCmpGe>(T); flags = EW_KERNEL_SWAP01; توڑو;
    طےشدہ:        ;
    }
    واپس {f, خالی_اشاریہ, flags};
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
سانچہ<قسم_نام T, قسم_نام Vvec>
ساکن صحیح notKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                     ثابت خالی*, size_t, size_t, ثابت خالی*, size_t, size_t,
                     خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                     ثابت دوہرا*, صحیح, خالی*)
{
    اگر (چوڑائی <= 0 || اونچائی <= 0) واپس 0;
    s0y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x == 1u || چوڑائی == 1);
    ثابت T* src0 = (ثابت T*)src0_;
    T* dst = (T*)dst_;
    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && s0y == (size_t)چوڑائی) { چوڑائی *= اونچائی; اونچائی = 1; }
    برائے (صحیح y = 0; y < اونچائی; y++, src0 += s0y, dst += dsty)
    {
        صحیح x = 0;
    #اگر (CV_SIMD || CV_SIMD_SCALABLE)
        اگر constexpr (حجم(T) <= 4)
        {
            ثابت صحیح VECSZ = VTraits<Vvec>::vlanes();
            برائے (; x <= چوڑائی - VECSZ; x += VECSZ)
                v_store(dst + x, v_not(vx_بھرو(src0 + x)));
        }
    #اگر_ختم
        برائے (; x < چوڑائی; x++) dst[x] = (T)~src0[x];
    }
    واپس 0;
}

سانچہ<جماعت Op>
ساکن KernelFunc bitwiseByEsz(صحیح esz)
{
    انتخاب (esz)
    {
    حالت 1: واپس vec_ثنائی_Kernel<uint8_t,  uint8_t,  v_uint8,  uint8_t,  Op, uint8_t>;
    حالت 2: واپس vec_ثنائی_Kernel<uint16_t, uint16_t, v_uint16, uint16_t, Op, uint16_t>;
    حالت 4: واپس vec_ثنائی_Kernel<uint32_t, uint32_t, v_uint32, uint32_t, Op, uint32_t>;
    حالت 8: واپس scalar_ثنائی_Kernel<uint64_t, uint64_t, uint64_t, Op>;
    طےشدہ: واپس خالی_اشاریہ;
    }
}

TKernel getBitwiseFunc_(چوٹی op, صحیح esz)
{
    KernelFunc f = خالی_اشاریہ;
    انتخاب (op)
    {
    حالت OP_AND: f = bitwiseByEsz<EwAnd>(esz); توڑو;
    حالت OP_OR:  f = bitwiseByEsz<EwOr >(esz); توڑو;
    حالت OP_XOR: f = bitwiseByEsz<EwXor>(esz); توڑو;
    طےشدہ:     ;
    }
    واپس {f, خالی_اشاریہ, 0};
}

TKernel getNotFunc_(صحیح esz)
{
    KernelFunc f = خالی_اشاریہ;
    انتخاب (esz)
    {
    حالت 1: f = notKernel<uint8_t,  v_uint8 >; توڑو;
    حالت 2: f = notKernel<uint16_t, v_uint16>; توڑو;
    حالت 4: f = notKernel<uint32_t, v_uint32>; توڑو;
    حالت 8: f = notKernel<uint64_t, v_uint32>; توڑو;   // SIMD path compiled out for 8-byte -> scalar ~
    طےشدہ: ;
    }
    واپس {f, خالی_اشاریہ, 0};
}

// ===========================================================================
// OP_ADDW (addWeighted): dst = a*alpha + b*beta + gamma, params[0..2] = {alpha, beta, gamma}. Two fused
// v_fma in the work type Wvec - f32 SIMD for u8/s8/u16/s16/f16/bf16/f32; the 32-bit-int/64-bit group
// works in f64 (v_float64 SIMD under CV_SIMD_64F || CV_SIMD_SCALABLE_64F, else use_simd=false scalar). Like vecBinaryKernel but
// WITHOUT its multi-channel short-row
// branch: addWeighted takes plain scalar coefficients (a multi-channel scalar is not optimized, matching
// the classic function). The broadcast branches fold the constant operand's contribution once.
// ===========================================================================
سانچہ<قسم_نام T, قسم_نام Tr, قسم_نام Wvec, قسم_نام WT, منطقی use_simd>
ساکن صحیح جوڑو_Weighted_Kernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                             ثابت خالی* src1_, size_t s1y, size_t s1x,
                             ثابت خالی*, size_t, size_t,
                             خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                             ثابت دوہرا* params, صحیح, خالی*)
{
    اگر (چوڑائی <= 0 || اونچائی <= 0) واپس 0;
    s0y /= حجم(T); s1y /= حجم(T); dsty /= حجم(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (size_t)چوڑائی == 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    Tr* dst = (Tr*)dst_;
    ثابت WT alpha = (WT)params[0], beta = (WT)params[1], gamma = (WT)params[2];
    EW_TRY_COLLAPSE(2);
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    Wvec va{}, vb{}, vg{};
    ثابت صحیح VECSZ = VTraits<Wvec>::vlanes();
    ثابت منطقی دم = چوڑائی >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    اگر constexpr (use_simd) {
        WT fa=alpha, fb=beta, fg=gamma;
        vx_setall_as(&fa, va); vx_setall_as(&fb, vb); vx_setall_as(&fg, vg);
    }
#اگر_ختم
    برائے (صحیح y = 0; y < اونچائی; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        صحیح x = 0;
    #اگر (CV_SIMD || CV_SIMD_SCALABLE)
        اگر constexpr (use_simd)
        {
            Wvec a0,a1,a2,a3,b0,b1,b2,b3;
            اگر (s0x == s1x) {                                // both arrays contiguous
                برائے (; x < چوڑائی; x += VECSZ*4) {
                    اگر (x+VECSZ*4 > چوڑائی) { اگر (!دم) توڑو; x = چوڑائی-VECSZ*4; }
                    vx_بھرو_pair_as(src0+x, a0, a1); vx_بھرو_pair_as(src0+x+VECSZ*2, a2, a3);
                    vx_بھرو_pair_as(src1+x, b0, b1); vx_بھرو_pair_as(src1+x+VECSZ*2, b2, b3);
                    a0=v_fma(a0,va,v_fma(b0,vb,vg)); a1=v_fma(a1,va,v_fma(b1,vb,vg));
                    a2=v_fma(a2,va,v_fma(b2,vb,vg)); a3=v_fma(a3,va,v_fma(b3,vb,vg));
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            ورنہ اگر (s1x == 0) {                             // src1 broadcast: b*beta+gamma is constant
                Wvec bb; vx_setall_as(src1, bb); Wvec bc = v_fma(bb, vb, vg);
                برائے (; x < چوڑائی; x += VECSZ*4) {
                    اگر (x+VECSZ*4 > چوڑائی) { اگر (!دم) توڑو; x = چوڑائی-VECSZ*4; }
                    vx_بھرو_pair_as(src0+x, a0, a1); vx_بھرو_pair_as(src0+x+VECSZ*2, a2, a3);
                    a0=v_fma(a0,va,bc); a1=v_fma(a1,va,bc); a2=v_fma(a2,va,bc); a3=v_fma(a3,va,bc);
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            ورنہ {                                           // src0 broadcast: a*alpha+gamma is constant
                Wvec aa; vx_setall_as(src0, aa); Wvec acg = v_fma(aa, va, vg);
                برائے (; x < چوڑائی; x += VECSZ*4) {
                    اگر (x+VECSZ*4 > چوڑائی) { اگر (!دم) توڑو; x = چوڑائی-VECSZ*4; }
                    vx_بھرو_pair_as(src1+x, b0, b1); vx_بھرو_pair_as(src1+x+VECSZ*2, b2, b3);
                    b0=v_fma(b0,vb,acg); b1=v_fma(b1,vb,acg); b2=v_fma(b2,vb,acg); b3=v_fma(b3,vb,acg);
                    v_store_pair_as(dst+x, b0, b1); v_store_pair_as(dst+x+VECSZ*2, b2, b3);
                }
            }
        }
    #اگر_ختم
        اگر (s0x == s1x) {
            برائے (; x < چوڑائی; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + (WT)src1[x]*beta + gamma);
        } ورنہ اگر (s0x == 0) {
            ثابت WT ac = (WT)src0[0]*alpha + gamma;
            برائے (; x < چوڑائی; x++) dst[x] = saturate_cast<Tr>((WT)src1[x]*beta + ac);
        } ورنہ {
            ثابت WT bc = (WT)src1[0]*beta + gamma;
            برائے (; x < چوڑائی; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + bc);
        }
    }
    واپس 0;
}

TKernel get_جوڑو_Weighted_Func_2(صحیح T, صحیح R)
{
    KernelFunc f = خالی_اشاریہ;
    #تعریف_کرو AWS(Tt, Rr) جوڑو_Weighted_Kernel<Tt, Rr, v_float32, اعشاری, سچ>
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #تعریف_کرو AWD(Tt, Rr) جوڑو_Weighted_Kernel<Tt, Rr, v_float64, دوہرا, سچ>       // f64 SIMD
#ورنہ
    #تعریف_کرو AWD(Tt, Rr) جوڑو_Weighted_Kernel<Tt, Rr, v_float32, دوہرا, جھوٹ>      // scalar (v_float32 unused)
#اگر_ختم
    انتخاب (T)
    {
    حالت CV_8U:  f = R==CV_8U  ? AWS(uint8_t,  uint8_t)  : R==CV_32F ? AWS(uint8_t,  اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_8S:  f = R==CV_8S  ? AWS(int8_t,   int8_t)   : R==CV_32F ? AWS(int8_t,   اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_16U: f = R==CV_16U ? AWS(uint16_t, uint16_t) : R==CV_32F ? AWS(uint16_t, اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_16S: f = R==CV_16S ? AWS(int16_t,  int16_t)  : R==CV_32F ? AWS(int16_t,  اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_16F: f = R==CV_16F ? AWS(hfloat,   hfloat)   : R==CV_32F ? AWS(hfloat,   اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_16BF:f = R==CV_32F ? AWS(bfloat,   اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_32F: f = R==CV_32F ? AWS(اعشاری,    اعشاری) : خالی_اشاریہ; توڑو;
    حالت CV_32U: f = R==CV_32U ? AWD(غیرعلامتی, غیرعلامتی) : R==CV_64F ? AWD(غیرعلامتی, دوہرا) : خالی_اشاریہ; توڑو;
    حالت CV_32S: f = R==CV_32S ? AWD(صحیح,      صحیح)      : R==CV_64F ? AWD(صحیح,      دوہرا) : خالی_اشاریہ; توڑو;
    حالت CV_64U: f = R==CV_64F ? AWD(uint64_t, دوہرا) : خالی_اشاریہ; توڑو;
    حالت CV_64S: f = R==CV_64F ? AWD(int64_t,  دوہرا) : خالی_اشاریہ; توڑو;
    حالت CV_64F: f = R==CV_64F ? AWD(دوہرا,   دوہرا) : خالی_اشاریہ; توڑو;
    طےشدہ: ;
    }
    #تعریف_منسوخ AWS
    #تعریف_منسوخ AWD
    واپس {f, خالی_اشاریہ, 0};
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
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
// VECSZ(T) mask bytes -> T-width lanes (u8 direct, u16/u32 via expand)
ساکن اندرونی v_uint8  بھرو_Select_نقاب(ثابت uchar* m, ثابت v_uint8&)  { واپس vx_بھرو(m); }
ساکن اندرونی v_uint16 بھرو_Select_نقاب(ثابت uchar* m, ثابت v_uint16&) { واپس vx_بھرو_expand(m); }
ساکن اندرونی v_uint32 بھرو_Select_نقاب(ثابت uchar* m, ثابت v_uint32&) { واپس vx_بھرو_expand_q(m); }

ساکن اندرونی خالی setallSelect(ثابت uint8_t*  p, v_uint8&  a) { a = vx_setall_u8(*p); }
ساکن اندرونی خالی setallSelect(ثابت uint16_t* p, v_uint16& a) { a = vx_setall_u16(*p); }
ساکن اندرونی خالی setallSelect(ثابت uint32_t* p, v_uint32& a) { a = vx_setall_u32(*p); }
#اگر_ختم

سانچہ<قسم_نام T, قسم_نام Tvec>
ساکن صحیح selectKernel(ثابت خالی* نقاب_3, size_t smy, size_t smx,
                        ثابت خالی* src1_, size_t s1y, size_t s1x,
                        ثابت خالی* src2_, size_t s2y, size_t s2x,
                        خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                        ثابت دوہرا*, صحیح, خالی*)
{
    s1y /= حجم(T);                    // the 1-byte mask's smy stays in bytes == elements
    s2y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(smx <= 1u && s1x <= 1u && s2x <= 1u);

    ثابت uchar* نقاب = (ثابت uchar*)نقاب_3;
    ثابت T* src1 = (ثابت T*)src1_;
    ثابت T* src2 = (ثابت T*)src2_;
    T* dst = (T*)dst_;

    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && smy == smx*(size_t)چوڑائی &&
        s1y == s1x*(size_t)چوڑائی && s2y == s2x*(size_t)چوڑائی)
    { چوڑائی *= اونچائی; اونچائی = 1; }

    صحیح y = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
    اگر constexpr (حجم(T) <= 4)
    {
        ثابت صحیح VECSZ = VTraits<Tvec>::vlanes();
        ثابت Tvec z = v_setzero_<Tvec>();

        // n-channel data under a per-pixel mask (the masked-op shape): channel axis = width (2..4),
        // one mask byte per row. Process VECSZ rows per iteration - expand the mask once and
        // interleave it across the channel lanes.
        اگر (اونچائی > VECSZ && 2 <= چوڑائی && چوڑائی <= 4 && smx == 0u && smy == 1u &&
            s1x == 1u && s1y == (size_t)چوڑائی && s2x == 1u && s2y == (size_t)چوڑائی &&
            dsty == (size_t)چوڑائی)
        {
            constexpr صحیح MAXVECSZ = VTraits<Tvec>::max_nlanes;
            T maskbuf[MAXVECSZ*4] = {};      // {} for -Wmaybe-uninitialized only (fully written)
            ثابت صحیح dy = VECSZ;
            برائے (; y + dy <= اونچائی; y += dy, نقاب += dy, src1 += چوڑائی*dy, src2 += چوڑائی*dy,
                                                          dst += چوڑائی*dy)
            {
                Tvec m0 = v_ne(بھرو_Select_نقاب(نقاب, z), z), m1, m2, m3;
                اگر (چوڑائی == 2)      v_store_interleave(maskbuf, m0, m0);
                ورنہ اگر (چوڑائی == 3) v_store_interleave(maskbuf, m0, m0, m0);
                ورنہ                 v_store_interleave(maskbuf, m0, m0, m0, m0);
                m0 = vx_بھرو(maskbuf);
                m1 = vx_بھرو(maskbuf + VECSZ);
                اگر (چوڑائی > 2) m2 = vx_بھرو(maskbuf + VECSZ*2);
                اگر (چوڑائی > 3) m3 = vx_بھرو(maskbuf + VECSZ*3);
                v_store(dst, v_select(m0, vx_بھرو(src1), vx_بھرو(src2)));
                v_store(dst + VECSZ, v_select(m1, vx_بھرو(src1 + VECSZ), vx_بھرو(src2 + VECSZ)));
                اگر (چوڑائی > 2)
                    v_store(dst + VECSZ*2,
                            v_select(m2, vx_بھرو(src1 + VECSZ*2), vx_بھرو(src2 + VECSZ*2)));
                اگر (چوڑائی > 3)
                    v_store(dst + VECSZ*3,
                            v_select(m3, vx_بھرو(src1 + VECSZ*3), vx_بھرو(src2 + VECSZ*3)));
            }
            // the remaining < VECSZ rows fall through to the per-row path below
        }
        ورنہ اگر (smx == 1)                                   // per-element mask - the common case
        {
            // a branch aliasing dst is fine under the backoff (idempotent select); dst == mask is not
            ثابت منطقی use_دم_trick = چوڑائی >= VECSZ*2 && dst_ != نقاب_3;
            Tvec a, b;
            برائے (; y < اونچائی; y++, نقاب += smy, src1 += s1y, src2 += s2y, dst += dsty)
            {
                // a row-constant branch still changes per row
                اگر (s1x == 0) setallSelect(src1, a);
                اگر (s2x == 0) setallSelect(src2, b);
                صحیح x = 0;
                برائے (; x < چوڑائی; x += VECSZ)
                {
                    اگر (x + VECSZ > چوڑائی) { اگر (!use_دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ; }
                    Tvec m = v_ne(بھرو_Select_نقاب(نقاب + x, z), z);
                    اگر (s1x) a = vx_بھرو(src1 + x);
                    اگر (s2x) b = vx_بھرو(src2 + x);
                    v_store(dst + x, v_select(m, a, b));
                }
                برائے (; x < چوڑائی; x++)
                    dst[x] = نقاب[x] != 0 ? src1[x*s1x] : src2[x*s2x];
            }
            واپس 0;
        }
    }
#اگر_ختم
    برائے (; y < اونچائی; y++, نقاب += smy, src1 += s1y, src2 += s2y, dst += dsty)
    {
        اگر (smx == 0)                     // one mask byte per row (n-channel data / broadcast mask)
        {
            ثابت T* s = نقاب[0] != 0 ? src1 : src2;
            ثابت size_t sx = نقاب[0] != 0 ? s1x : s2x;
            اگر ((ثابت خالی*)s != (ثابت خالی*)dst)      // row select from dst itself is a no-op
                برائے (صحیح x = 0; x < چوڑائی; x++) dst[x] = s[x*sx];
        }
        ورنہ
            برائے (صحیح x = 0; x < چوڑائی; x++)
                dst[x] = نقاب[x] != 0 ? src1[x*s1x] : src2[x*s2x];
    }
    واپس 0;
}

TKernel getSelectFunc_(صحیح mdepth, صحیح T)
{
    اگر (CV_ELEM_SIZE1(mdepth) != 1)      // the mask must be a 1-byte type (u8/s8/bool)
        واپس {};
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (CV_ELEM_SIZE1(T))
    {
    حالت 1: fptr = selectKernel<uint8_t,  v_uint8 >; توڑو;
    حالت 2: fptr = selectKernel<uint16_t, v_uint16>; توڑو;
    حالت 4: fptr = selectKernel<uint32_t, v_uint32>; توڑو;
    حالت 8: fptr = selectKernel<uint64_t, v_uint32>; توڑو;   // SIMD path compiled out -> scalar
    طےشدہ: ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
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
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
ساکن اندرونی خالی setallClamp(ثابت uchar*  p, v_uint8&   a) { a = vx_setall_u8(*p); }
ساکن اندرونی خالی setallClamp(ثابت schar*  p, v_int8&    a) { a = vx_setall_s8(*p); }
ساکن اندرونی خالی setallClamp(ثابت ushort* p, v_uint16&  a) { a = vx_setall_u16(*p); }
ساکن اندرونی خالی setallClamp(ثابت مختصر*  p, v_int16&   a) { a = vx_setall_s16(*p); }
ساکن اندرونی خالی setallClamp(ثابت غیرعلامتی* p, v_uint32& a) { a = vx_setall_u32(*p); }
ساکن اندرونی خالی setallClamp(ثابت صحیح*    p, v_int32&   a) { a = vx_setall_s32(*p); }
ساکن اندرونی خالی setallClamp(ثابت اعشاری*  p, v_float32& a) { a = vx_setall_f32(*p); }
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ساکن اندرونی خالی setallClamp(ثابت دوہرا* p, v_float64& a) { a = vx_setall_f64(*p); }
#اگر_ختم
#اگر_ختم

سانچہ<قسم_نام T, قسم_نام Tvec>
ساکن صحیح clampKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                       ثابت خالی* lo_, size_t s1y, size_t s1x,
                       ثابت خالی* hi_, size_t s2y, size_t s2x,
                       خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                       ثابت دوہرا*, صحیح, خالی*)
{
    s0y /= حجم(T);
    s1y /= حجم(T);
    s2y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* lo = (ثابت T*)lo_;
    ثابت T* hi = (ثابت T*)hi_;
    T* dst = (T*)dst_;

    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && s0y == s0x*(size_t)چوڑائی &&
        s1y == s1x*(size_t)چوڑائی && s2y == s2x*(size_t)چوڑائی)
    { چوڑائی *= اونچائی; اونچائی = 1; }

    برائے (صحیح y = 0; y < اونچائی; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
    {
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        اگر (s0x == 1)
        {
            ثابت صحیح VECSZ = VTraits<Tvec>::vlanes();
            ثابت منطقی use_دم_trick = چوڑائی >= VECSZ*2 && lo_ != dst_ && hi_ != dst_;
            Tvec vlo, vhi;
            اگر (s1x == 0) setallClamp(lo, vlo);
            اگر (s2x == 0) setallClamp(hi, vhi);
            برائے (; x < چوڑائی; x += VECSZ)
            {
                اگر (x + VECSZ > چوڑائی) { اگر (!use_دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ; }
                اگر (s1x) vlo = vx_بھرو(lo + x);
                اگر (s2x) vhi = vx_بھرو(hi + x);
                v_store(dst + x, v_min(v_max(vx_بھرو(src0 + x), vlo), vhi));
            }
        }
#اگر_ختم
        برائے (; x < چوڑائی; x++)
        {
            T v = src0[x*s0x], l = lo[x*s1x], h = hi[x*s2x];
            dst[x] = std::min(std::max(v, l), h);
        }
    }
    واپس 0;
}

// Scalar-only clamp for the depths without a native SIMD lane type here (f16/bf16 - compared in
// float; 64-bit ints).
سانچہ<قسم_نام T, قسم_نام WT>
ساکن صحیح scalarClampKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                             ثابت خالی* lo_, size_t s1y, size_t s1x,
                             ثابت خالی* hi_, size_t s2y, size_t s2x,
                             خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                             ثابت دوہرا*, صحیح, خالی*)
{
    s0y /= حجم(T); s1y /= حجم(T); s2y /= حجم(T); dsty /= حجم(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* lo = (ثابت T*)lo_;
    ثابت T* hi = (ثابت T*)hi_;
    T* dst = (T*)dst_;
    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && s0y == s0x*(size_t)چوڑائی &&
        s1y == s1x*(size_t)چوڑائی && s2y == s2x*(size_t)چوڑائی)
    { چوڑائی *= اونچائی; اونچائی = 1; }
    برائے (صحیح y = 0; y < اونچائی; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
        برائے (صحیح x = 0; x < چوڑائی; x++)
        {
            WT v = (WT)src0[x*s0x], l = (WT)lo[x*s1x], h = (WT)hi[x*s2x];
            dst[x] = saturate_cast<T>(std::min(std::max(v, l), h));
        }
    واپس 0;
}

TKernel getClampFunc_(صحیح T)
{
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (T)
    {
    حالت CV_8U:   fptr = clampKernel<uchar,    v_uint8  >; توڑو;
    حالت CV_8S:   fptr = clampKernel<schar,    v_int8   >; توڑو;
    حالت CV_16U:  fptr = clampKernel<ushort,   v_uint16 >; توڑو;
    حالت CV_16S:  fptr = clampKernel<مختصر,    v_int16  >; توڑو;
    حالت CV_32U:  fptr = clampKernel<غیرعلامتی, v_uint32 >; توڑو;
    حالت CV_32S:  fptr = clampKernel<صحیح,      v_int32  >; توڑو;
    حالت CV_32F:  fptr = clampKernel<اعشاری,    v_float32>; توڑو;
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  fptr = clampKernel<دوہرا,   v_float64>; توڑو;
#ورنہ
    حالت CV_64F:  fptr = scalarClampKernel<دوہرا, دوہرا>; توڑو;
#اگر_ختم
    حالت CV_16F:  fptr = scalarClampKernel<hfloat, اعشاری>; توڑو;
    حالت CV_16BF: fptr = scalarClampKernel<bfloat, اعشاری>; توڑو;
    حالت CV_64U:  fptr = scalarClampKernel<uint64_t, uint64_t>; توڑو;
    حالت CV_64S:  fptr = scalarClampKernel<int64_t,  int64_t >; توڑو;
    طےشدہ: ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
}

ساکن صحیح expandKernel(size_t s0y, size_t s0x,
                        خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                        صحیح esz1)
{
    uchar* dst = (uchar*)dst_;
    اگر (s0x == 0u && چوڑائی > 1) {
        صحیح h = s0y > 0u ? اونچائی : 1;
        CV_Assert(esz1 == 1 || esz1 == 2 || esz1 == 4 || esz1 == 8);
        برائے (صحیح y = 0; y < h; y++) {
            اگر (esz1 == 1) {
                uchar* rowptr = dst + dsty*y;
                uchar val = rowptr[0];
                برائے (صحیح x = 1; x < چوڑائی; x++) rowptr[x] = val;
            }
            ورنہ اگر (esz1 == 2) {
                ushort* rowptr = (ushort*)(dst + dsty*y);
                ushort val = rowptr[0];
                برائے (صحیح x = 1; x < چوڑائی; x++) rowptr[x] = val;
            }
            ورنہ اگر (esz1 == 4) {
                غیرعلامتی* rowptr = (غیرعلامتی*)(dst + dsty*y);
                غیرعلامتی val = rowptr[0];
                برائے (صحیح x = 1; x < چوڑائی; x++) rowptr[x] = val;
            }
            ورنہ {
                uint64_t* rowptr = (uint64_t*)(dst + dsty*y);
                uint64_t val = rowptr[0];
                برائے (صحیح x = 1; x < چوڑائی; x++) rowptr[x] = val;
            }
        }
    }

    اگر (s0y == 0) {
        برائے (صحیح y = 1; y < اونچائی; y++)
            حافظہ_نقل(dst + dsty*y, dst, (size_t)esz1*چوڑائی);
    }
    واپس 0;
}

ساکن صحیح castKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                      ثابت خالی*, size_t, size_t,
                      ثابت خالی*, size_t, size_t,
                      خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                      ثابت دوہرا* params, صحیح esz1, خالی* userdata)
{
    ثنائی_Func castfunc = (ثنائی_Func)userdata;
    castfunc((ثابت uchar*)src0_, s0y, خالی_اشاریہ, 0, (uchar*)dst_, dsty,
             سائز((s0x > 0u ? چوڑائی : 1), (s0y > 0u ? اونچائی : 1)), (خالی*)params);
    واپس expandKernel(s0y, s0x, dst_, dsty, چوڑائی, اونچائی, esz1);
}

// ===========================================================================
// Per-op entry points (this baseline). The op-level getElemwiseFunc() dispatcher and the regular
// get*Func() forwarders live in arithm.dispatch.cpp.
// ===========================================================================

// OP_CAST / OP_CONVERT_SCALE: wrap core's convert BinaryFunc (carried in kernel.userdata) in
// castKernel, which runs it over the distinct sub-region and then expands across broadcast axes.
TKernel getCastFunc_(صحیح sdepth, صحیح ddepth, منطقی scaled)
{
    ثنائی_Func cvt = scaled ? getConvertScaleFunc(sdepth, ddepth) : getConvertFunc(sdepth, ddepth);
    واپس {castKernel, (خالی*)cvt, CV_ELEM_SIZE1(ddepth)};
}

// Non-template wrappers around the templated kernel selectors (ADD/SUB share getAddSubFunc, MIN/MAX
// share getMinMaxFunc). min/max are T x T -> T, so R is ignored (the dispatcher validates R == T).
TKernel get_جوڑو_Func_2(صحیح T, صحیح R) { واپس get_جوڑو_Sub_Func<Ew_جوڑو>(T, R); }
TKernel getSubFunc_(صحیح T, صحیح R) { واپس get_جوڑو_Sub_Func<EwSub>(T, R); }
TKernel getMinFunc_(صحیح T, صحیح R) { (خالی)R; واپس getMinMaxFunc<EwMin>(T); }
TKernel getMaxFunc_(صحیح T, صحیح R) { (خالی)R; واپس getMinMaxFunc<EwMax>(T); }

TKernel getHypotFunc_(صحیح T, صحیح R)
{
    اگر (R != T)
        واپس {};
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (T)
    {
    حالت CV_16F:  fptr = vec_ثنائی_Kernel<hfloat, hfloat, v_float32, اعشاری, EwHypot>; توڑو;
    حالت CV_16BF: fptr = vec_ثنائی_Kernel<bfloat, bfloat, v_float32, اعشاری, EwHypot>; توڑو;
    حالت CV_32F:  fptr = vec_ثنائی_Kernel<اعشاری,  اعشاری,  v_float32, اعشاری, EwHypot>; توڑو;
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  fptr = vec_ثنائی_Kernel<دوہرا, دوہرا, v_float64, دوہرا, EwHypot>; توڑو;
#ورنہ
    حالت CV_64F:  fptr = scalar_ثنائی_Kernel<دوہرا, دوہرا, دوہرا, EwHypot>; توڑو;
#اگر_ختم
    طےشدہ: ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
}

TKernel getAtan2Func_(صحیح T, صحیح R)
{
    اگر (R != T)
        واپس {};
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (T)
    {
    حالت CV_16F:  fptr = vec_ثنائی_Kernel<hfloat, hfloat, v_float32, اعشاری, EwAtan2>; توڑو;
    حالت CV_16BF: fptr = vec_ثنائی_Kernel<bfloat, bfloat, v_float32, اعشاری, EwAtan2>; توڑو;
    حالت CV_32F:  fptr = vec_ثنائی_Kernel<اعشاری,  اعشاری,  v_float32, اعشاری, EwAtan2>; توڑو;
    حالت CV_64F:  fptr = scalar_ثنائی_Kernel<دوہرا, دوہرا, دوہرا, EwAtan2>; توڑو;   // exact std::atan2
    طےشدہ: ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
}

#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
}} // namespace cv::ew
