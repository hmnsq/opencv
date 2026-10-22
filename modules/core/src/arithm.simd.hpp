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

#अन्तर्भाव "opencv2/core/hal/intrin.hpp"
#अन्तर्भाव "convert.hpp"          // typed load/store-as helpers (cv::), and getConvert*Func
#अन्तर्भाव "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#अन्तर्भाव <algorithm>
#अन्तर्भाव <cmath>
#अन्तर्भाव <cstring>

नामाकाश cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// BinaryFunc and getConvertFunc / getConvertScaleFunc come from core (precomp.hpp / convert.hpp).

#जर CV_SIMD128_FP16
#undef CV_SIMD_16F
#define CV_SIMD_16F 1
#किंवाजर !defined(CV_SIMD_16F)
#define CV_SIMD_16F 0       // keep `#if CV_SIMD_16F` -Wundef-clean on builds without FP16 SIMD
#endif

#जर (CV_SIMD || CV_SIMD_SCALABLE)
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक uchar* p, v_uint8& a)
{ a = vx_setall_u8(*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक schar* p, v_int8& a)
{ a = vx_setall_s8(*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक uchar* p, v_int16& a)
{ a = vx_setall_s16(*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक schar* p, v_int16& a)
{ a = vx_setall_s16(*p); }

स्थिर ओळीत शून्य vx_setall_as(स्थिरांक ushort* p, v_uint16& a)
{ a = vx_setall_u16(*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक लघु* p, v_int16& a)
{ a = vx_setall_s16(*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक ushort* p, v_int32& a)
{ a = vx_setall_s32(*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक लघु* p, v_int32& a)
{ a = vx_setall_s32(*p); }

स्थिर ओळीत शून्य vx_setall_as(स्थिरांक अचिन्हित* p, v_uint32& a)
{ a = vx_setall_u32(*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक पूर्णांक* p, v_int32& a)
{ a = vx_setall_s32(*p); }

स्थिर ओळीत शून्य vx_setall_as(स्थिरांक uchar* p, v_float32& a)
{ a = vx_setall_f32(दशांश(*p)); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक schar* p, v_float32& a)
{ a = vx_setall_f32(दशांश(*p)); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक ushort* p, v_float32& a)
{ a = vx_setall_f32(दशांश(*p)); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक लघु* p, v_float32& a)
{ a = vx_setall_f32(दशांश(*p)); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक दशांश* p, v_float32& a)
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
#जर defined(__ARM_NEON)
स्थिर ओळीत v_int32  v_add_sat(स्थिरांक v_int32& a,  स्थिरांक v_int32& b)  { परत v_int32(vqaddq_s32(a.val, b.val)); }
स्थिर ओळीत v_int32  v_sub_sat(स्थिरांक v_int32& a,  स्थिरांक v_int32& b)  { परत v_int32(vqsubq_s32(a.val, b.val)); }
स्थिर ओळीत v_uint32 v_add_sat(स्थिरांक v_uint32& a, स्थिरांक v_uint32& b) { परत v_uint32(vqaddq_u32(a.val, b.val)); }
स्थिर ओळीत v_uint32 v_sub_sat(स्थिरांक v_uint32& a, स्थिरांक v_uint32& b) { परत v_uint32(vqsubq_u32(a.val, b.val)); }
#define EW_HAVE_MULSAT32 1
स्थिर ओळीत v_uint8 v_mul_sat(स्थिरांक v_uint8& a, स्थिरांक v_uint8& b)
{
    uint16x8_t p0 = vmull_u8(vget_खालचा_u8(a.val), vget_खालचा_u8(b.val));
    uint16x8_t p1 = vmull_u8(vget_वरचा_u8(a.val), vget_वरचा_u8(b.val));
    परत v_uint8(vcombine_u8(vqmovn_u16(p0), vqmovn_u16(p1)));
}
स्थिर ओळीत v_int8 v_mul_sat(स्थिरांक v_int8& a, स्थिरांक v_int8& b)
{
    int16x8_t p0 = vmull_s8(vget_खालचा_s8(a.val), vget_खालचा_s8(b.val));
    int16x8_t p1 = vmull_s8(vget_वरचा_s8(a.val), vget_वरचा_s8(b.val));
    परत v_int8(vcombine_s8(vqmovn_s16(p0), vqmovn_s16(p1)));
}
स्थिर ओळीत v_uint16 v_mul_sat(स्थिरांक v_uint16& a, स्थिरांक v_uint16& b)
{
    uint32x4_t p0 = vmull_u16(vget_खालचा_u16(a.val), vget_खालचा_u16(b.val));
    uint32x4_t p1 = vmull_u16(vget_वरचा_u16(a.val), vget_वरचा_u16(b.val));
    परत v_uint16(vcombine_u16(vqmovn_u32(p0), vqmovn_u32(p1)));
}
स्थिर ओळीत v_int16 v_mul_sat(स्थिरांक v_int16& a, स्थिरांक v_int16& b)
{
    int32x4_t p0 = vmull_s16(vget_खालचा_s16(a.val), vget_खालचा_s16(b.val));
    int32x4_t p1 = vmull_s16(vget_वरचा_s16(a.val), vget_वरचा_s16(b.val));
    परत v_int16(vcombine_s16(vqmovn_s32(p0), vqmovn_s32(p1)));
}
स्थिर ओळीत v_uint32 v_mul_sat(स्थिरांक v_uint32& a, स्थिरांक v_uint32& b)
{
    uint64x2_t p0 = vmull_u32(vget_खालचा_u32(a.val), vget_खालचा_u32(b.val));
    uint64x2_t p1 = vmull_u32(vget_वरचा_u32(a.val), vget_वरचा_u32(b.val));
    परत v_uint32(vcombine_u32(vqmovn_u64(p0), vqmovn_u64(p1)));
}
स्थिर ओळीत v_int32 v_mul_sat(स्थिरांक v_int32& a, स्थिरांक v_int32& b)
{
    int64x2_t p0 = vmull_s32(vget_खालचा_s32(a.val), vget_खालचा_s32(b.val));
    int64x2_t p1 = vmull_s32(vget_वरचा_s32(a.val), vget_वरचा_s32(b.val));
    परत v_int32(vcombine_s32(vqmovn_s64(p0), vqmovn_s64(p1)));
}
#अन्यथा
स्थिर ओळीत v_int32 v_add_sat(स्थिरांक v_int32& a, स्थिरांक v_int32& b)
{
    v_int32 res = v_add(a, b);
    v_int32 ov  = v_and(v_xor(a, res), v_xor(b, res));               // sign bit set iff overflow
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(पूर्णांक_उच्च));       // a >= 0 ? INT_MAX : INT_MIN
    परत v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
स्थिर ओळीत v_int32 v_sub_sat(स्थिरांक v_int32& a, स्थिरांक v_int32& b)
{
    v_int32 res = v_sub(a, b);
    v_int32 ov  = v_and(v_xor(a, b), v_xor(a, res));
    v_int32 sat = v_xor(v_shr<31>(a), vx_setall_s32(पूर्णांक_उच्च));
    परत v_select(v_lt(ov, vx_setzero_s32()), sat, res);
}
स्थिर ओळीत v_uint32 v_add_sat(स्थिरांक v_uint32& a, स्थिरांक v_uint32& b)
{
    v_uint32 res = v_add(a, b);
    परत v_or(res, v_lt(res, a));                                  // wrapped => all-ones
}
स्थिर ओळीत v_uint32 v_sub_sat(स्थिरांक v_uint32& a, स्थिरांक v_uint32& b)
{
    परत v_and(v_sub(a, b), v_ge(a, b));                           // borrow => zero
}
स्थिर ओळीत v_uint8 v_mul_sat(स्थिरांक v_uint8& a, स्थिरांक v_uint8& b)
{ v_uint16 p0, p1; v_mul_expand(a, b, p0, p1); परत v_pack(p0, p1); }
स्थिर ओळीत v_int8 v_mul_sat(स्थिरांक v_int8& a, स्थिरांक v_int8& b)
{ v_int16 p0, p1; v_mul_expand(a, b, p0, p1); परत v_pack(p0, p1); }
स्थिर ओळीत v_uint16 v_mul_sat(स्थिरांक v_uint16& a, स्थिरांक v_uint16& b)
{ v_uint32 p0, p1; v_mul_expand(a, b, p0, p1); परत v_pack(p0, p1); }
स्थिर ओळीत v_int16 v_mul_sat(स्थिरांक v_int16& a, स्थिरांक v_int16& b)
{ v_int32 p0, p1; v_mul_expand(a, b, p0, p1); परत v_pack(p0, p1); }
#endif
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F   // scalable (RVV) has v_float64 without CV_SIMD_64F
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक द्विगुण*   p, v_float64& a) { a = vx_setall_f64(*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक पूर्णांक*      p, v_float64& a) { a = vx_setall_f64((द्विगुण)*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक अचिन्हित* p, v_float64& a) { a = vx_setall_f64((द्विगुण)*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक int64_t*  p, v_float64& a) { a = vx_setall_f64((द्विगुण)*p); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक uint64_t* p, v_float64& a) { a = vx_setall_f64((द्विगुण)*p); }
#endif
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक bfloat* p, v_float32& a)
{ a = vx_setall_f32(दशांश(*p)); }
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक hfloat* p, v_float32& a)
{ a = vx_setall_f32(दशांश(*p)); }

स्थिर ओळीत शून्य vx_load_pair_as(स्थिरांक uchar* p, v_uint8& a, v_uint8& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_uint8>::vlanes());
}

स्थिर ओळीत शून्य vx_load_pair_as(स्थिरांक schar* p, v_int8& a, v_int8& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_int8>::vlanes());
}

स्थिर ओळीत शून्य vx_load_pair_as(स्थिरांक uchar* p, v_uint32& a, v_uint32& b)
{
    v_uint16 w = vx_load_expand(p);
    v_expand(w, a, b);
}

स्थिर ओळीत शून्य v_store_pair_as(uchar* p, स्थिरांक v_uint8& a, स्थिरांक v_uint8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_uint8>::vlanes(), b);
}

स्थिर ओळीत शून्य v_store_pair_as(schar* p, स्थिरांक v_int8& a, स्थिरांक v_int8& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_int8>::vlanes(), b);
}


स्थिर ओळीत शून्य v_store_pair_as(पूर्णांक* p, स्थिरांक v_int16& a, स्थिरांक v_int16& b)
{
    स्थिरांक पूर्णांक nlanes32 = VTraits<v_int32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v0);
    v_store(p + nlanes32, v1);
    v_store(p + nlanes32*2, v2);
    v_store(p + nlanes32*3, v3);
}

स्थिर ओळीत शून्य v_store_pair_as(दशांश* p, स्थिरांक v_int16& a, स्थिरांक v_int16& b)
{
    स्थिरांक पूर्णांक nlanes32 = VTraits<v_float32>::vlanes();
    v_int32 v0, v1, v2, v3;
    v_expand(a, v0, v1);
    v_expand(b, v2, v3);
    v_store(p, v_cvt_f32(v0));
    v_store(p + nlanes32, v_cvt_f32(v1));
    v_store(p + nlanes32*2, v_cvt_f32(v2));
    v_store(p + nlanes32*3, v_cvt_f32(v3));
}

स्थिर ओळीत शून्य v_store_pair_as(दशांश* p, स्थिरांक v_int32& a, स्थिरांक v_int32& b)
{
    v_store(p, v_cvt_f32(a));
    v_store(p + VTraits<v_float32>::vlanes(), v_cvt_f32(b));
}

स्थिर ओळीत शून्य v_store_pair_as(hfloat* p, स्थिरांक v_float32& a, स्थिरांक v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

स्थिर ओळीत शून्य v_store_pair_as(bfloat* p, स्थिरांक v_float32& a, स्थिरांक v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

// Reinterpret a comparison result (whose lane type matches the operands) as the UNSIGNED integer
// vector of the same width: u8->u8, s8->u8, u16/s16->u16, u32/s32/f32->u32. The destination type
// drives the overload, so the compare kernel needs no per-width if constexpr.
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_uint8&  s, v_uint8&  d) { d = s; }
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_int8&   s, v_uint8&  d) { d = v_reinterpret_as_u8(s); }
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_uint16& s, v_uint16& d) { d = s; }
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_int16&  s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_uint32& s, v_uint32& d) { d = s; }
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_int32&  s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_float32& s, v_uint32& d) { d = v_reinterpret_as_u32(s); }
#जर CV_SIMD_16F
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_float16& s, v_uint16& d) { d = v_reinterpret_as_u16(s); }
#endif
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
स्थिर ओळीत शून्य v_reinterpret_as(स्थिरांक v_float64& s, v_uint64& d) { d = v_reinterpret_as_u64(s); }
#endif

// Broadcast the compare mask value (1 or 255) across the unsigned work vector.
स्थिर ओळीत शून्य v_setall_mask(v_uint8&  v, uchar t) { v = vx_setall_u8(t); }
स्थिर ओळीत शून्य v_setall_mask(v_uint16& v, uchar t) { v = vx_setall_u16(t); }
स्थिर ओळीत शून्य v_setall_mask(v_uint32& v, uchar t) { v = vx_setall_u32(t); }

// NOTE: v_store_pair_as(uchar*, v_uint16/v_uint32, ...) - the narrowing pack-to-u8 stores we need -
// already come from core's convert.hpp (included above), so they are not redefined here.

#जर CV_SIMD_16F
स्थिर ओळीत शून्य vx_setall_as(स्थिरांक hfloat* p, v_float16& a)
{ a = vx_setall_f16(*p); }
स्थिर ओळीत शून्य vx_load_pair_as(स्थिरांक hfloat* p, v_float16& a, v_float16& b)
{
    a = vx_load(p);
    b = vx_load(p + VTraits<v_float16>::vlanes());
}
स्थिर ओळीत शून्य v_store_pair_as(hfloat* p, स्थिरांक v_float16& a, स्थिरांक v_float16& b)
{
    v_store(p, a);
    v_store(p + VTraits<v_float16>::vlanes(), b);
}
#endif

#endif

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

नामाकाश ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      arithm.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getAddFunc_(पूर्णांक T, पूर्णांक R);
TKernel getSubFunc_(पूर्णांक T, पूर्णांक R);
TKernel getMulFunc_(पूर्णांक T, पूर्णांक R);
TKernel getDivFunc_(पूर्णांक T, पूर्णांक R, तार्किक checked);
TKernel getMinFunc_(पूर्णांक T, पूर्णांक R);
TKernel getMaxFunc_(पूर्णांक T, पूर्णांक R);
TKernel getAbsdiffFunc_(पूर्णांक T, पूर्णांक R);
TKernel getHypotFunc_(पूर्णांक T, पूर्णांक R);         // hypot = sqrt(x^2+y^2), float depths, T x T -> T
TKernel getAtan2Func_(पूर्णांक T, पूर्णांक R);         // atan2(y, x), radians (-pi, pi], float depths
TKernel getCmpFunc_(शीर्ष op, पूर्णांक T);
TKernel getBitwiseFunc_(शीर्ष op, पूर्णांक esz);                    // OP_AND / OP_OR / OP_XOR, by element size
TKernel getNotFunc_(पूर्णांक esz);                                // OP_NOT, by element size
TKernel getAddWeightedFunc_(पूर्णांक T, पूर्णांक R);                   // OP_ADDW, a*alpha+b*beta+gamma (T x T -> R)
TKernel getSelectFunc_(पूर्णांक mdepth, पूर्णांक T);   // OP_SELECT: 1-byte mask, a/b/dst of T (by esz)
TKernel getClampFunc_(पूर्णांक T);                // OP_CLAMP: min(max(x, lo), hi), all operands of T
TKernel getCastFunc_(पूर्णांक sdepth, पूर्णांक ddepth, तार्किक scaled);   // OP_CAST / OP_CONVERT_SCALE

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors (vector + scalar). New binary ops slot in here.
// ===========================================================================
संरचना EwAdd {
    // useScalar: does the op consume the scale scalar (params[0]) in vec()/preproc()? When false,
    // vecBinaryKernel's fast 2-arg branch is taken unconditionally (the check folds at compile time);
    // when true, it is taken only if the runtime scale is exactly 1.
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_add(a, b); }
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: v_add wraps - use the saturating version to match the scalar int64+clamp tail
    स्थिर v_int32  vec(स्थिरांक v_int32& a,  स्थिरांक v_int32& b)  { परत v_add_sat(a, b); }
    स्थिर v_uint32 vec(स्थिरांक v_uint32& a, स्थिरांक v_uint32& b) { परत v_add_sat(a, b); }
#endif
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    // Accumulate in the promoted type, NOT in W: for the native saturating path W is the narrow
    // lane type (schar/short/...), and (W)(a+b) would wrap in 8/16 bits before saturate_cast<Tr>
    // could clamp. Letting a+b promote (narrow -> int) keeps saturation for 8/16-bit outputs and
    // the natural wrap for 32/64-bit (both matching cv::add). The SIMD path already saturates.
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत W(a + b); }
};

संरचना EwSub {
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_sub(a, b); }
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    // 32-bit lanes: see EwAdd::vec
    स्थिर v_int32  vec(स्थिरांक v_int32& a,  स्थिरांक v_int32& b)  { परत v_sub_sat(a, b); }
    स्थिर v_uint32 vec(स्थिरांक v_uint32& a, स्थिरांक v_uint32& b) { परत v_sub_sat(a, b); }
#endif
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत W(a - b); }   // see EwAdd::scl
    // 64-bit unsigned has no wider WT to hold a-b, so the generic path would wrap on underflow.
    // Saturate at 0 to match cv::subtract (8/16/32-bit already saturate via SIMD floor / wide WT).
    स्थिर uint64_t scl(uint64_t a, uint64_t b, uint64_t) { परत uint64_t(a >= b)*(a - b); }
};

// mul/div compute in a wide FLOAT work type (W = float for <=16-bit/f16/bf/f32, double for
// 32/64-bit/f64), matching cv::multiply/divide; the executor casts the work-type result down to
// rdepth. scalar (reference) path only for now - SIMD can follow. (No vec(): never instantiated.)
// vec(a, b, scale): the 3rd arg is the scale vector. COMMUTATIVE ops (mul, absdiff) ignore it - their
// scale is folded into the (cheaper) preproc of one operand. DIVISION is NOT commutative, so its scale
// MUST stick to the numerator; it takes the scale in vec (numerator*scale/denominator) and leaves
// preproc as identity. vecBinaryKernel always passes vscalar to vec, so every branch (incl. a broadcast
// denominator) divides correctly. The 2-arg vec (scale==1) is the both-contiguous fast path.
संरचना EwMul {
    स्थिर constexpr तार्किक useScalar = सत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_mul(a, b); }
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    // integer lanes: full product clamped to the lane type (v_mul_sat) == cv::multiply semantics
    // at scale 1. These drive the scale==1 fast path on FULL-width registers (Wvec1 = the native
    // lane vector): whole-register loads/stores, widening happens inside the multiply itself.
    // The guard matches the v_mul_sat definitions above (like EwAdd's 32-bit overloads): in
    // no-SIMD builds the vector typedefs still exist (intrin_cpp), but the helpers do not.
    स्थिर v_uint8  vec(स्थिरांक v_uint8& a,  स्थिरांक v_uint8& b)  { परत v_mul_sat(a, b); }
    स्थिर v_int8   vec(स्थिरांक v_int8& a,   स्थिरांक v_int8& b)   { परत v_mul_sat(a, b); }
    स्थिर v_uint16 vec(स्थिरांक v_uint16& a, स्थिरांक v_uint16& b) { परत v_mul_sat(a, b); }
    स्थिर v_int16  vec(स्थिरांक v_int16& a,  स्थिरांक v_int16& b)  { परत v_mul_sat(a, b); }
#ifdef EW_HAVE_MULSAT32
    स्थिर v_uint32 vec(स्थिरांक v_uint32& a, स्थिरांक v_uint32& b) { परत v_mul_sat(a, b); }
    स्थिर v_int32  vec(स्थिरांक v_int32& a,  स्थिरांक v_int32& b)  { परत v_mul_sat(a, b); }
#endif
#endif
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V& s) { परत v_mul(a, s); }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST s) { परत a * b * s; }
};
// div has two variants by the COMMON INPUT type (matching cv::'s per-type kernel choice): integer
// inputs guard divide-by-zero -> 0 (cv:: iscalar_div); float inputs do NOT guard (cv:: fscalar_div,
// a/0 -> inf), which then saturates on the cast to an integer output exactly like cv::divide.
संरचना EwDivInt {
    स्थिर constexpr तार्किक useScalar = सत्य;
    // integer inputs computed in the float work type: guard b==0 -> 0. Scale rides the numerator (vec).
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) {
        स्थिरांक V z = v_setzero_<V>();
        परत v_select(v_eq(b, z), z, v_div(a, b));
    }
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक V& s) {
        स्थिरांक V z = v_setzero_<V>();
        परत v_select(v_eq(b, z), z, v_div(v_mul(a, s), b));
    }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }   // identity: scale is in vec
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST s) { परत b != W(0) ? a * s / b : W(0); }
};
संरचना EwDivFlt {
    स्थिर constexpr तार्किक useScalar = सत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_div(a, b); }
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक V& s) { परत v_div(v_mul(a, s), b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }   // identity: scale is in vec
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST s) { परत a * s / b; }
};

// atan2(y, x) in RADIANS over the standard C range (-pi, pi] - the fastAtan2 minimax polynomial
// (mathfuncs_core.simd.hpp v_atan_f32) with the 180/pi factor dropped and the C quadrant logic
// (fastAtan2 returns degrees in [0, 360)). Absolute accuracy ~1e-5 rad, same as cv::fastAtan2.
// Generic over the universal-intrinsic float vector type.
ढाँचा<typename V>
स्थिर ओळीत V v_atan2(स्थिरांक V& y, स्थिरांक V& x)
{
    using LT = typename VTraits<V>::lane_type;
    स्थिरांक V eps  = v_setall_<V>((LT)द्विग_अंतर);
    स्थिरांक V z    = v_setzero_<V>();
    स्थिरांक V p7   = v_setall_<V>((LT)-0.04432655554792128);
    स्थिरांक V p5   = v_setall_<V>((LT)0.1555786518463281);
    स्थिरांक V p3   = v_setall_<V>((LT)-0.3258083974640975);
    स्थिरांक V p1   = v_setall_<V>((LT)0.9997878412794807);
    स्थिरांक V vpi2 = v_setall_<V>((LT)(CV_PI/2));
    स्थिरांक V vpi  = v_setall_<V>((LT)CV_PI);

    V ax = v_abs(x), ay = v_abs(y);
    V c  = v_div(v_min(ax, ay), v_add(v_max(ax, ay), eps));
    V c2 = v_mul(c, c);
    V a  = v_mul(v_fma(v_fma(v_fma(p7, c2, p5), c2, p3), c2, p1), c);
    a = v_select(v_ge(ax, ay), a, v_sub(vpi2, a));
    a = v_select(v_lt(x, z), v_sub(vpi, a), a);
    a = v_select(v_lt(y, z), v_sub(z, a), a);
    परत a;
}

संरचना EwAtan2 {
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_atan2(a, b); }   // a = y, b = x
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत std::atan2(a, b); }
};

// hypot(x, y) = sqrt(x^2 + y^2): NAIVE (matches cv::magnitude; overflow at |x| ~ 1e19+ for f32
// inputs is accepted), computed in the float work type; T x T -> T over the float depths.
संरचना EwHypot {
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_sqrt(v_fma(a, a, v_mul(b, b))); }
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत std::वर्ग_मूल(a*a + b*b); }
};

// min / max / absdiff: T x T -> T (same depth in and out, no scale). v_min/v_max exist for every
// vector lane type (64-bit ints fall back to scalar). absdiff uses v_absdiff (defined for the
// UNSIGNED and float lane types - signed/wide depths go through the scalar path), and the scalar
// |a-b| is computed branch-wise so it never underflows an unsigned work type.
संरचना EwMin {
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_min(a, b); }
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत std::min(a, b); }
};
संरचना EwMax {
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_max(a, b); }
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत std::max(a, b); }
};
संरचना EwAbsdiff {
    स्थिर constexpr तार्किक useScalar = असत्य;
    // The absdiff RESULT is the UNSIGNED type of the input width: v_absdiff(v_int8/16/32) already returns
    // v_uint8/16/32 (the true |a-b|, which can exceed the signed max), and v_absdiff on unsigned/float
    // returns the same type. vec() therefore returns THAT type (deduced), not the input V, so the kernel
    // stores it through the matching UNSIGNED v_store_pair_as (an honest same-type store) - no reinterpret,
    // no touching the saturating-narrow overloads' semantics. (This is why the kernel keeps the vec
    // result in its own variable rather than reusing the input operand.)
    ढाँचा<typename V> स्थिर स्वतः vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_absdiff(a, b); }
    ढाँचा<typename V, typename S> स्थिर स्वतः vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत v_absdiff(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत a > b ? W(a - b) : W(b - a); }
};

// Signed T -> SAME signed T (rdepth==T): |a-b| saturated into the signed range, in ONE pass. cv::absdiff
// keeps the signed depth, so the generic path computes absdiff->unsigned then casts back down (2 insns);
// this fuses it. v_absdiff yields the unsigned |a-b|; v_min clamps to the signed max (already >=0), then
// a same-width reinterpret to signed (all values now fit).
संरचना EwAbsdiffS {
    स्थिर constexpr तार्किक useScalar = असत्य;
    स्थिर v_int8  vec(स्थिरांक v_int8&  a, स्थिरांक v_int8&  b) { परत v_reinterpret_as_s8 (v_min(v_absdiff(a, b), vx_setall_u8 (0x7f))); }
    स्थिर v_int16 vec(स्थिरांक v_int16& a, स्थिरांक v_int16& b) { परत v_reinterpret_as_s16(v_min(v_absdiff(a, b), vx_setall_u16(0x7fff))); }
    स्थिर v_int32 vec(स्थिरांक v_int32& a, स्थिरांक v_int32& b) { परत v_reinterpret_as_s32(v_min(v_absdiff(a, b), vx_setall_u32(0x7fffffff))); }
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत a > b ? W(a - b) : W(b - a); }
    // 64-bit signed has no wider work type to widen into, and |INT64_MIN - b| does not fit in
    // int64_t, so the difference is taken in uint64 - exact for every pair - and then clamped.
    // The narrower widths reach this overload only through their int64 work type, where |a - b|
    // is at most 2^32 and the clamp is a no-op.
    स्थिर int64_t scl(int64_t a, int64_t b, int64_t)
    {
        स्थिरांक uint64_t d = (a > b) ? (uint64_t)a - (uint64_t)b : (uint64_t)b - (uint64_t)a;
        परत (int64_t)std::min<uint64_t>(d, (uint64_t)INT64_MAX);
    }
};

// compare: T x T -> u8 mask. cmp(a,b) is the scalar relation over a work type W (integers compare
// directly, f16/bf16 through float); vec(a,b) is the vector relation returning an all-ones/zero mask
// in the operand's own lane type. The kernel turns either into 0 / trueVal (1 or 255 via TKernel::flags).
संरचना EwCmpEq { ढाँचा<typename W> स्थिर तार्किक cmp(W a, W b) { परत a == b; }
                 ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_eq(a, b); } };
संरचना EwCmpNe { ढाँचा<typename W> स्थिर तार्किक cmp(W a, W b) { परत a != b; }
                 ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_ne(a, b); } };
// LT/LE are synthesized from GT/GE via the EW_KERNEL_SWAP01 flag (a<b == b>a), so no Lt/Le kernels.
संरचना EwCmpGt { ढाँचा<typename W> स्थिर तार्किक cmp(W a, W b) { परत a > b; }
                 ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_gt(a, b); } };
संरचना EwCmpGe { ढाँचा<typename W> स्थिर तार्किक cmp(W a, W b) { परत a >= b; }
                 ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_ge(a, b); } };

// bitwise AND/OR/XOR: bit-pattern op, type-agnostic. Run on the UNSIGNED integer whose width matches
// the element (u8/u16/u32/u64), so one functor set covers every depth. No scale, no widening (T x T ->
// T, exactly like min/max); preproc is the identity (min/max share this shape). 64-bit uses the scalar
// path (no widening vector helpers), the rest ride vecBinaryKernel's native same-type path.
संरचना EwAnd {
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_and(a, b); }
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत W(a & b); }
};
संरचना EwOr {
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_or(a, b); }
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत W(a | b); }
};
संरचना EwXor {
    स्थिर constexpr तार्किक useScalar = असत्य;
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b) { परत v_xor(a, b); }
    ढाँचा<typename V, typename S> स्थिर V vec(स्थिरांक V& a, स्थिरांक V& b, स्थिरांक S&) { परत vec(a, b); }
    ढाँचा<typename V> स्थिर V preproc(स्थिरांक V& a, स्थिरांक V&) { परत a; }
    ढाँचा<typename W, typename ST> स्थिर W scl(W a, W b, ST) { परत W(a ^ b); }
};

// Collapse a gap-free 2D tile to 1D (call with the per-operand x/y-steps).
#define EW_TRY_COLLAPSE(NSRC) \
    जर (height > 1 && dsty == (माप_प्रकार)width && \
        s0y == s0x*(माप_प्रकार)width && (NSRC < 2 || s1y == s1x*(माप_प्रकार)width)) \
    { width *= height; height = 1; }

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
ढाँचा<typename T, typename Tr, typename WT, वर्ग Op, typename ST=WT>
स्थिर पूर्णांक scalar_द्विमान_Kernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                              स्थिरांक शून्य* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                              स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार, शून्य* dst_, माप_प्रकार dsty,
                              पूर्णांक width, पूर्णांक height, स्थिरांक द्विगुण* params, पूर्णांक, शून्य*)
{
    जर (width <= 0 || height <= 0) परत 0;
    s0y /= आकार(T);
    s1y /= आकार(T);
    dsty /= आकार(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (माप_प्रकार)width == 1u);

    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* src1 = (स्थिरांक T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    साठी (पूर्णांक y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        जर (s0x == s1x) {
            साठी (पूर्णांक x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], (WT)src1[x], scalar));
        }
        अन्यथा जर (s0x == 0) {
            WT sc0 = (WT)src0[0];
            साठी (पूर्णांक x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl(sc0, (WT)src1[x], scalar));
        }
        अन्यथा {
            WT sc1 = (WT)src1[0];
            साठी (पूर्णांक x = 0; x < width; x++)
                dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x], sc1, scalar));
        }
    }
    परत 0;
}

ढाँचा<typename T, typename WT>
स्थिर शून्य expandScalar(स्थिरांक T* sc, माप_प्रकार sx, पूर्णांक n0, WT* scbuf, पूर्णांक n)
{
    जर (n0 <= 0 || n <= 0) परत;
    पूर्णांक i = 0;
    साठी (; i < n0; i++) scbuf[i] = (WT)sc[i*sx];
    साठी (; i < n; i++) scbuf[i] = scbuf[i - n0];
}

// Decode the per-channel patch bytes (mask + value) from the kernel flags into small arrays. Uniform
// (no EW_CMP_PATCH): mask = trueVal, value = 0 (an ordinary compare). Per-channel: from the 4-bit
// fields. The compare kernels apply  result = (rawmask & mask) | value  per channel - folding the
// former separate patch pass into the compare (one pass).
स्थिर ओळीत शून्य cmpUnpackPatch(पूर्णांक flags, uchar trueVal, uchar mvals[4], uchar vvals[4])
{
    जर (flags & EW_CMP_PATCH)
        साठी (पूर्णांक c = 0; c < 4; c++)
        {
            स्थिरांक पूर्णांक f = (flags >> (EW_CMP_PATCH_SHIFT + c*4)) & 0xF;
            mvals[c] = (uchar)cmpPatchByte(f & 3);
            vvals[c] = (uchar)cmpPatchByte((f >> 2) & 3);
        }
    अन्यथा
        साठी (पूर्णांक c = 0; c < 4; c++) { mvals[c] = trueVal; vvals[c] = 0; }
}

// Scalar comparison kernel: T x T -> u8 mask. Result is 0 (false) or `trueVal` (true), trueVal read
// from TKernel::flags - 255 (default, matching cv::compare) or 1 (a numpy-style 0/1 mask). Integers
// compare directly (WT == T); f16/bf16 compare through a float WT. This is the fallback for the depths
// the SIMD kernel doesn't cover (f16/bf16/64-bit). stepx in {0,1}; dst is u8, byte step == element step.
ढाँचा<typename T, typename WT, वर्ग Cmp>
स्थिर पूर्णांक scalarCompareKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                         स्थिरांक शून्य* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                         स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                         शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                         स्थिरांक द्विगुण*, पूर्णांक flags, शून्य*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    जर (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= आकार(T);
    s1y /= आकार(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (माप_प्रकार)width == 1u);

    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* src1 = (स्थिरांक T*)src1_;
    uchar* dst = (uchar*)dst_;
    स्थिरांक uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    स्थिरांक तार्किक perch = (flags & EW_CMP_PATCH) != 0;          // per-channel fix-up (width=cn<=4)
    CV_Assert(!perch || width <= 4);                         // a per-channel patch is a short-row tile
    #define EW_CMP_APPLY(cond, x) (perch ? (uchar)(((cond) ? mvals[x] : 0) | vvals[x]) \
                                         : (uchar)((cond) ? trueVal : 0))

    EW_TRY_COLLAPSE(2);
    साठी (पूर्णांक y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        जर (s0x == s1x) {
            साठी (पूर्णांक x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], (WT)src1[x]), x);
        }
        अन्यथा जर (s0x == 0) {
            WT a = (WT)src0[0];
            साठी (पूर्णांक x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, (WT)src1[x]), x);
        }
        अन्यथा {
            WT b = (WT)src1[0];
            साठी (पूर्णांक x = 0; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp((WT)src0[x], b), x);
        }
    }
    #undef EW_CMP_APPLY
    परत 0;
}

// SIMD comparison kernel: T x T -> u8 mask, for the directly-comparable depths (u8/s8/u16/s16/u32/
// s32/f32). vec(a,b) gives an all-ones/zero mask in the operand's lane type; v_reinterpret_as turns it
// into the unsigned int of the same width; cmpFuse packs to u8 and applies (rawmask & M) | V. The
// per-row SIMD body runs when an operand is contiguous or broadcast; a per-channel patch (M/V differ
// by channel) only ever arrives as a short-row (width=cn) tile and is handled there.
ढाँचा<typename T, typename Vvec, typename Uvec, वर्ग Cmp>
स्थिर पूर्णांक vecCompareKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                            स्थिरांक शून्य* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                            स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                            शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                            स्थिरांक द्विगुण*, पूर्णांक flags, शून्य*)
{
    // LT/LE reuse the GT/GE kernel with the operands swapped (a<b == b>a, a<=b == b>=a).
    जर (flags & EW_KERNEL_SWAP01) { std::swap(src0_, src1_); std::swap(s0y, s1y); std::swap(s0x, s1x); }
    s0y /= आकार(T);
    s1y /= आकार(T);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (माप_प्रकार)width == 1u);

    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* src1 = (स्थिरांक T*)src1_;
    uchar* dst = (uchar*)dst_;
    स्थिरांक uchar trueVal = (flags & EW_KERNEL_MASK1) ? 1 : 255;
    uchar mvals[4], vvals[4];                                 // per-channel fix-up: (rawmask & m) | v
    cmpUnpackPatch(flags, trueVal, mvals, vvals);
    स्थिरांक तार्किक perch = (flags & EW_CMP_PATCH) != 0;
    CV_Assert(!perch || width <= 4);                          // a per-channel patch is a short-row tile

    EW_TRY_COLLAPSE(2);
    पूर्णांक y = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    // Short rows (2/3/4 elements): a per-channel scalar over a multi-channel image arrives as a
    // (width=cn) x (height=pixels) tile with the scalar broadcast over rows (s?y==0). The per-row SIMD
    // below never triggers at such a tiny width, so expand the width<=4 broadcast operand (threshold
    // AND the interleaved per-channel mask/value bytes) across the lanes and compare many rows at once.
    // This is also the ONLY path a per-channel patch (M/V differ by channel) reaches.
    // constexpr: the short-row loads T natively (vx_load->Vvec), so it exists ONLY at native width. The
    // widened f16/bf16->f32 path (sizeof(T) < Vvec lane) and 8-byte f64 fall through to per-row + scalar.
    // (sizeof(T) <= 4 is part of the constexpr gate: for f64 the body would compile to nothing but
    // dead stores - gcc 9 flags scbuf/bsrc/bsx as set-but-not-used there.)
    जर constexpr (आकार(T) == आकार(typename VTraits<Vvec>::lane_type) && आकार(T) <= 4)
    जर (height > 1 && width <= 4 &&
        ((s0y == 0 && s1y == (माप_प्रकार)width*s1x) || (s1y == 0 && s0y == (माप_प्रकार)width*s0x)) &&
        dsty == (माप_प्रकार)width)
    {
        स्थिरांक पूर्णांक VECSZ  = VTraits<Vvec>::vlanes();          // sizeof(T)-type lanes
        स्थिरांक पूर्णांक VECSZ8 = VTraits<v_uint8>::vlanes();       // u8 output lanes
        constexpr पूर्णांक MAXV8 = VTraits<v_uint8>::max_nlanes;
        T scbuf[MAXV8 * 3] = {};                             // interleaved threshold (elements)
        uchar mbuf[MAXV8 * 3] = {}, vbuf[MAXV8 * 3] = {};    // interleaved mask / value (bytes)
        // ^ the {} inits are for -Wmaybe-uninitialized only: expandScalar/decode fill every lane that
        //   is later read, but the compiler cannot prove it with a runtime VECSZ
        स्थिरांक तार्किक bc0 = (s0y == 0);                         // src0 is the broadcast (short) operand
        स्थिरांक T* bsrc = bc0 ? src0 : src1; स्थिरांक माप_प्रकार bsx = bc0 ? s0x : s1x;
        स्थिरांक T*& src = bc0 ? src1 : src0;                   // the row-stepping operand (advanced below)
        स्थिरांक माप_प्रकार sy = bc0 ? s1y : s0y;
        // The compare direction (scalar-first when bc0, array-first otherwise) is hoisted OUT of the row
        // loop with an explicit if(bc0) - the loop is written twice so no per-iteration branch remains.
        // Per branch: own unroll, threshold + interleaved M/V loaded ONCE. sizeof==1 keeps 12 vectors.
        #define EW_CMPR(r, a, b) v_reinterpret_as(Cmp::vec(a, b), r)   // rawmask -> sizeof(T)-byte uint
        जर constexpr (आकार(T) == 1)                        // 3 u8 masks -> 3 u8 stores
        {
            स्थिरांक पूर्णांक ewidth = VECSZ8 * 3;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (माप_प्रकार)1, width, mbuf, ewidth);
            expandScalar(vvals, (माप_प्रकार)1, width, vbuf, ewidth);
            स्थिरांक पूर्णांक dy = ewidth / width;
            Vvec  sc0=vx_load(scbuf),  sc1=vx_load(scbuf+VECSZ),   sc2=vx_load(scbuf+VECSZ*2);
            v_uint8 M0=vx_load(mbuf),  M1=vx_load(mbuf+VECSZ8),    M2=vx_load(mbuf+VECSZ8*2);
            v_uint8 V0=vx_load(vbuf),  V1=vx_load(vbuf+VECSZ8),    V2=vx_load(vbuf+VECSZ8*2);
            #define EW_ROW1(A0,B0,A1,B1,A2,B2) \
                साठी (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 o0=v_or(v_and(r0,M0),V0),o1=v_or(v_and(r1,M1),V1),o2=v_or(v_and(r2,M2),V2); \
                    v_store(dst, o0); v_store(dst+VECSZ8, o1); v_store(dst+VECSZ8*2, o2); }
            जर (bc0) EW_ROW1(sc0,vx_load(src), sc1,vx_load(src+VECSZ), sc2,vx_load(src+VECSZ*2))
            अन्यथा     EW_ROW1(vx_load(src),sc0, vx_load(src+VECSZ),sc1, vx_load(src+VECSZ*2),sc2)
            #undef EW_ROW1
        }
        अन्यथा जर constexpr (आकार(T) == 2)                   // 3 u16 masks -> 1 full + 1 half u8 store
        {
            स्थिरांक पूर्णांक ewidth = VECSZ * 3;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (माप_प्रकार)1, width, mbuf, ewidth);
            expandScalar(vvals, (माप_प्रकार)1, width, vbuf, ewidth);
            स्थिरांक पूर्णांक dy = ewidth / width;
            Vvec  sc0=vx_load(scbuf), sc1=vx_load(scbuf+VECSZ), sc2=vx_load(scbuf+VECSZ*2);
            v_uint8 M0=vx_load(mbuf), M1=vx_load(mbuf+VECSZ8);
            v_uint8 V0=vx_load(vbuf), V1=vx_load(vbuf+VECSZ8);
            #define EW_ROW2(A0,B0,A1,B1,A2,B2) \
                साठी (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2; EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    v_uint8 b0=v_pack(r0,r1), b1=v_pack(r2,r2); \
                    v_uint8 o0=v_or(v_and(b0,M0),V0), o1=v_or(v_and(b1,M1),V1); \
                    v_store(dst, o0); v_store_खालचा(dst+VECSZ8, o1); }
            जर (bc0) EW_ROW2(sc0,vx_load(src), sc1,vx_load(src+VECSZ), sc2,vx_load(src+VECSZ*2))
            अन्यथा     EW_ROW2(vx_load(src),sc0, vx_load(src+VECSZ),sc1, vx_load(src+VECSZ*2),sc2)
            #undef EW_ROW2
        }
        अन्यथा जर constexpr (आकार(T) == 4)                   // sizeof==4: 6 u32 -> 3 u16 -> 1 full + 1 half u8
        {
            स्थिरांक पूर्णांक ewidth = VECSZ * 6;
            expandScalar(bsrc, bsx, width, scbuf, ewidth);
            expandScalar(mvals, (माप_प्रकार)1, width, mbuf, ewidth);
            expandScalar(vvals, (माप_प्रकार)1, width, vbuf, ewidth);
            स्थिरांक पूर्णांक dy = ewidth / width;
            Vvec sc0=vx_load(scbuf),sc1=vx_load(scbuf+VECSZ),sc2=vx_load(scbuf+VECSZ*2),
                 sc3=vx_load(scbuf+VECSZ*3),sc4=vx_load(scbuf+VECSZ*4),sc5=vx_load(scbuf+VECSZ*5);
            v_uint8 M0=vx_load(mbuf), M1=vx_load(mbuf+VECSZ8);
            v_uint8 V0=vx_load(vbuf), V1=vx_load(vbuf+VECSZ8);
            #define EW_ROW4(A0,B0,A1,B1,A2,B2,A3,B3,A4,B4,A5,B5) \
                साठी (; y + dy <= height; y += dy, src += sy*dy, dst += dsty*dy) { \
                    Uvec r0,r1,r2,r3,r4,r5; \
                    EW_CMPR(r0,A0,B0); EW_CMPR(r1,A1,B1); EW_CMPR(r2,A2,B2); \
                    EW_CMPR(r3,A3,B3); EW_CMPR(r4,A4,B4); EW_CMPR(r5,A5,B5); \
                    v_uint16 p0=v_pack(r0,r1), p1=v_pack(r2,r3), p2=v_pack(r4,r5); \
                    v_uint8 q0=v_pack(p0,p1), q1=v_pack(p2,p2); \
                    v_uint8 o0=v_or(v_and(q0,M0),V0), o1=v_or(v_and(q1,M1),V1); \
                    v_store(dst, o0); v_store_खालचा(dst+VECSZ8, o1); }
            जर (bc0) EW_ROW4(sc0,vx_load(src),sc1,vx_load(src+VECSZ),sc2,vx_load(src+VECSZ*2),
                             sc3,vx_load(src+VECSZ*3),sc4,vx_load(src+VECSZ*4),sc5,vx_load(src+VECSZ*5))
            अन्यथा     EW_ROW4(vx_load(src),sc0,vx_load(src+VECSZ),sc1,vx_load(src+VECSZ*2),sc2,
                             vx_load(src+VECSZ*3),sc3,vx_load(src+VECSZ*4),sc4,vx_load(src+VECSZ*5),sc5)
            #undef EW_ROW4
        }
        #undef EW_CMPR
    }
#endif
    साठी (; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        पूर्णांक x = 0;
    #जर (CV_SIMD || CV_SIMD_SCALABLE)
        // SIMD for BOTH the contiguous case AND a broadcast operand (a per-channel scalar const has
        // step 0 - without this the scalar-vs-array compare, incl. every multi-channel scalar compare,
        // fell to the scalar tail below, ~5x slower). vx_setall broadcasts the step-0 operand once.
        // 4-vector unroll + halide right-edge backoff (reprocess the last 4*VECSZ when width is not a
        // multiple); dst is a separate mask buffer so the overlap is a harmless idempotent rewrite.
        स्थिरांक पूर्णांक VECSZ = VTraits<Vvec>::vlanes();
        स्थिरांक पूर्णांक VECSZ8 = VTraits<v_uint8>::vlanes();
        v_uint8 vT; v_setall_mask(vT, trueVal);              // uniform mask (per-channel patch is short-row)
        स्थिरांक तार्किक tailTrick = width >= 4*VECSZ && src0_ != dst_ && src1_ != dst_;
        // Narrow the 4 masks to u8 FIRST, then apply trueVal on the u8 result (1 and per u8 vector, not
        // per wide lane-group). One block per MASK lane width (== sizeof(T) natively, but 4 for the
        // f16/bf16->f32 widened path); a 4-vector unroll spans exactly 4*VECSZ elements.
        constexpr माप_प्रकार MW = आकार(typename VTraits<Uvec>::lane_type);
        #define PACK_STORE_CMP_RESULT(m0,m1,m2,m3, D) करा { \
            जर constexpr (MW == 1u) { \
                v_uint8 o0=v_and(m0,vT),o1=v_and(m1,vT),o2=v_and(m2,vT),o3=v_and(m3,vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); v_store((D)+VECSZ8*2,o2); v_store((D)+VECSZ8*3,o3); \
            } अन्यथा जर constexpr (MW == 2u) { \
                v_uint8 o0=v_and(v_pack(m0,m1),vT), o1=v_and(v_pack(m2,m3),vT); \
                v_store((D),o0); v_store((D)+VECSZ8,o1); \
            } अन्यथा जर constexpr (MW == 4u) { \
                v_uint8 o0=v_and(v_pack(v_pack(m0,m1),v_pack(m2,m3)),vT); \
                v_store((D),o0); \
            } अन्यथा { /* sizeof==8: 4 u64 -> 2 u32 -> 1 u16 -> u8 (low half), 4*VECSZ elems -> store_low */ \
                v_uint16 g=v_pack(v_pack(m0,m1),v_pack(m2,m3)); \
                v_uint8 o0=v_and(v_pack(g,g),vT); \
                v_store_खालचा((D),o0); \
            } } पर्यंत(0)
        जर (s0x == 1u && s1x == 1u)
        {
            साठी (; x < width; x += 4*VECSZ)
            {
                जर (x + 4*VECSZ > width) { जर (!tailTrick) थांबा; x = width - 4*VECSZ; }
                Vvec a0, a1, a2, a3, b0, b1, b2, b3;   // vx_load_pair_as widens f16/bf16->f32, else native
                vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+2*VECSZ, a2, a3);
                vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+2*VECSZ, b2, b3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b1), m1);
                v_reinterpret_as(Cmp::vec(a2, b2), m2); v_reinterpret_as(Cmp::vec(a3, b3), m3);
                PACK_STORE_CMP_RESULT(m0, m1, m2, m3, dst+x);
            }
        }
        अन्यथा जर (s1x == 0u)                       // src1 (e.g. the scalar) broadcast
        {
            Vvec b0; vx_setall_as(src1, b0);
            साठी (; x < width; x += 4*VECSZ)
            {
                जर (x + 4*VECSZ > width) { जर (!tailTrick) थांबा; x = width - 4*VECSZ; }
                Vvec a0, a1, a2, a3;
                vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+2*VECSZ, a2, a3);
                Uvec m0, m1, m2, m3;
                v_reinterpret_as(Cmp::vec(a0, b0), m0); v_reinterpret_as(Cmp::vec(a1, b0), m1);
                v_reinterpret_as(Cmp::vec(a2, b0), m2); v_reinterpret_as(Cmp::vec(a3, b0), m3);
                PACK_STORE_CMP_RESULT(m0, m1, m2, m3, dst+x);
            }
        }
        अन्यथा जर (s0x == 0u)                       // src0 broadcast
        {
            Vvec a0; vx_setall_as(src0, a0);
            साठी (; x < width; x += 4*VECSZ)
            {
                जर (x + 4*VECSZ > width) { जर (!tailTrick) थांबा; x = width - 4*VECSZ; }
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
        जर (s0x == s1x) {
            साठी (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], src1[x]), x);
        }
        अन्यथा जर (s0x == 0) {
            T a = src0[0];
            साठी (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(a, src1[x]), x);
        }
        अन्यथा {
            T b = src1[0];
            साठी (; x < width; x++) dst[x] = EW_CMP_APPLY(Cmp::cmp(src0[x], b), x);
        }
        #undef EW_CMP_APPLY
    }
    परत 0;
}

// Unified binary kernel: T0 x T1 -> Tr (operands same depth for arithmetic; cast is separate).
//   Wvec     = work vector. Native (v_uint8/...) drives the same-type saturating path
//              (v_add saturates 8/16-bit, wraps 32-bit); v_float32 drives the widening hub.
//   WT       = scalar work type for the tail.
//   Op       = operation functor (vec()/scl()).
//   use_simd = compile-time switch; false => pure scalar (32/64-bit widened outputs, f64).
// stepx in {0,1}; dst contiguous. In-place safe (see file header).
ढाँचा<typename T, typename Tr, typename Wvec, typename WT, वर्ग Op, typename ST=WT, typename Wvec1=Wvec>
स्थिर पूर्णांक vec_द्विमान_Kernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                         स्थिरांक शून्य* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                         स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                         शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                         स्थिरांक द्विगुण* params, पूर्णांक, शून्य*)
{
    जर (width <= 0 || height <= 0) परत 0;
    s0y /= आकार(T);
    s1y /= आकार(T);
    dsty /= आकार(Tr);

    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (माप_प्रकार)width == 1u);

    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* src1 = (स्थिरांक T*)src1_;
    Tr* dst = (Tr*)dst_;
    [[maybe_unused]] ST scalar = saturate_cast<ST>(params[0]);   // mul/div scale; ignored by add/sub

    EW_TRY_COLLAPSE(2);
    पूर्णांक y = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    using Wlane = typename VTraits<Wvec>::lane_type;
    स्थिरांक पूर्णांक VECSZ = VTraits<Wvec>::vlanes();
    स्थिरांक तार्किक use_tail_trick = width >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    [[maybe_unused]] Wvec vscalar;
    vx_setall_as(&scalar, vscalar);
    जर (height > 1 && width <= 4 &&
        ((s0y == 0 && s1y == width*s1x) ||
         (s1y == 0 && s0y == width*s0x)) &&
        dsty == (माप_प्रकार)width) {
        constexpr पूर्णांक MAXVECSZ = VTraits<Wvec>::max_nlanes;
        Wlane scbuf[MAXVECSZ*6] = {};   // {} for -Wmaybe-uninitialized only (filled up to VECSZ*6)
        स्थिरांक पूर्णांक ewidth = VECSZ*6;
        expandScalar(s0y == 0 ? src0 : src1, s0y == 0 ? s0x : s1x, width, scbuf, ewidth);
        पूर्णांक dy = ewidth / width;
        Wvec sc0, sc1, sc2, sc3, sc4, sc5;
        sc0 = Op::preproc(vx_load(scbuf), vscalar);
        sc1 = Op::preproc(vx_load(scbuf + VECSZ), vscalar);
        sc2 = Op::preproc(vx_load(scbuf + VECSZ*2), vscalar);
        sc3 = Op::preproc(vx_load(scbuf + VECSZ*3), vscalar);
        sc4 = Op::preproc(vx_load(scbuf + VECSZ*4), vscalar);
        sc5 = Op::preproc(vx_load(scbuf + VECSZ*5), vscalar);

        जर (s0y == 0) {
            साठी (; y + dy <= height; y += dy, src1 += s1y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_load_pair_as(src1, v0, v1);
                vx_load_pair_as(src1 + VECSZ*2, v2, v3);
                vx_load_pair_as(src1 + VECSZ*4, v4, v5);
                स्वतः w0 = Op::vec(sc0, v0, vscalar), w1 = Op::vec(sc1, v1, vscalar),
                     w2 = Op::vec(sc2, v2, vscalar), w3 = Op::vec(sc3, v3, vscalar),
                     w4 = Op::vec(sc4, v4, vscalar), w5 = Op::vec(sc5, v5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
        अन्यथा {
            साठी (; y + dy <= height; y += dy, src0 += s0y*dy, dst += dsty*dy) {
                Wvec v0, v1, v2, v3, v4, v5;
                vx_load_pair_as(src0, v0, v1);
                vx_load_pair_as(src0 + VECSZ*2, v2, v3);
                vx_load_pair_as(src0 + VECSZ*4, v4, v5);
                स्वतः w0 = Op::vec(v0, sc0, vscalar), w1 = Op::vec(v1, sc1, vscalar),
                     w2 = Op::vec(v2, sc2, vscalar), w3 = Op::vec(v3, sc3, vscalar),
                     w4 = Op::vec(v4, sc4, vscalar), w5 = Op::vec(v5, sc5, vscalar);
                v_store_pair_as(dst, w0, w1);
                v_store_pair_as(dst + VECSZ*2, w2, w3);
                v_store_pair_as(dst + VECSZ*4, w4, w5);
            }
        }
    }
#endif
    साठी (; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        पूर्णांक x = 0;
    #जर (CV_SIMD || CV_SIMD_SCALABLE)
        Wvec a0, a1, a2, a3, b0, b1, b2, b3;
        जर (s0x == s1x) {
            जर (!Op::useScalar || scalar == ST(1)) {
                // this branch runs on Wvec1, which may be WIDER-laned than Wvec (e.g. the u8 mul
                // path: Wvec1=v_uint16 is 2x the lanes of Wvec=v_float32) - step by ITS lane count,
                // or the pairs overlap and the tail backoff writes past the row end
                स्थिरांक पूर्णांक VECSZ1 = VTraits<Wvec1>::vlanes();
                स्थिरांक तार्किक tail_trick1 = width >= VECSZ1*4 && src0_ != dst_ && src1_ != dst_;
                साठी (; x < width; x += VECSZ1*4) {
                    Wvec1 a0_, a1_, a2_, a3_, b0_, b1_, b2_, b3_;
                    जर (x + VECSZ1*4 > width) { जर (!tail_trick1) थांबा; x = width - VECSZ1*4; }
                    vx_load_pair_as(src0 + x, a0_, a1_);
                    vx_load_pair_as(src0 + x + VECSZ1*2, a2_, a3_);
                    vx_load_pair_as(src1 + x, b0_, b1_);
                    vx_load_pair_as(src1 + x + VECSZ1*2, b2_, b3_);
                    स्वतः c0 = Op::vec(a0_, b0_), c1 = Op::vec(a1_, b1_),
                         c2 = Op::vec(a2_, b2_), c3 = Op::vec(a3_, b3_);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ1*2, c2, c3);
                }
            }
            अन्यथा {
                साठी (; x < width; x += VECSZ*4) {
                    जर (x + VECSZ*4 > width) { जर (!use_tail_trick) थांबा; x = width - VECSZ*4; }
                    vx_load_pair_as(src0 + x, a0, a1);
                    vx_load_pair_as(src0 + x + VECSZ*2, a2, a3);
                    vx_load_pair_as(src1 + x, b0, b1);
                    vx_load_pair_as(src1 + x + VECSZ*2, b2, b3);
                    स्वतः c0 = Op::vec(Op::preproc(a0, vscalar), b0, vscalar),
                         c1 = Op::vec(Op::preproc(a1, vscalar), b1, vscalar),
                         c2 = Op::vec(Op::preproc(a2, vscalar), b2, vscalar),
                         c3 = Op::vec(Op::preproc(a3, vscalar), b3, vscalar);
                    v_store_pair_as(dst + x, c0, c1);
                    v_store_pair_as(dst + x + VECSZ*2, c2, c3);
                }
            }
        }
        अन्यथा जर (s1x == 0) {
            vx_setall_as(src1, b0);
            b0 = Op::preproc(b0, vscalar);
            साठी (; x < width; x += VECSZ*4) {
                जर (x + VECSZ*4 > width) { जर (!use_tail_trick) थांबा; x = width - VECSZ*4; }
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src0 + x + VECSZ*2, a2, a3);
                स्वतः c0 = Op::vec(a0, b0, vscalar), c1 = Op::vec(a1, b0, vscalar),
                     c2 = Op::vec(a2, b0, vscalar), c3 = Op::vec(a3, b0, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
        अन्यथा {
            vx_setall_as(src0, b0);
            b0 = Op::preproc(b0, vscalar);
            साठी (; x < width; x += VECSZ*4) {
                जर (x + VECSZ*4 > width) { जर (!use_tail_trick) थांबा; x = width - VECSZ*4; }
                vx_load_pair_as(src1 + x, a0, a1);
                vx_load_pair_as(src1 + x + VECSZ*2, a2, a3);
                स्वतः c0 = Op::vec(b0, a0, vscalar), c1 = Op::vec(b0, a1, vscalar),
                     c2 = Op::vec(b0, a2, vscalar), c3 = Op::vec(b0, a3, vscalar);
                v_store_pair_as(dst + x, c0, c1);
                v_store_pair_as(dst + x + VECSZ*2, c2, c3);
            }
        }
    #endif
        साठी (; x < width; x++)
            dst[x] = saturate_cast<Tr>(Op::scl((WT)src0[x*s0x], (WT)src1[x*s1x], scalar));
    }
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    vx_cleanup();
#endif
    परत 0;
}

// ===========================================================================
// OP_ADD
// ===========================================================================

// add dispatch: T x T -> R (operands already same depth T).
//  - native saturating path (Wvec = native, use_simd=true) for T -> T on <=32-bit ints + f32
//    (32-bit lanes via the local v_add_sat/v_sub_sat - v_add/v_sub wrap there);
//  - widening f32-hub (Wvec = v_float32, use_simd=true) for the widened/half/float outputs;
//  - scalar (use_simd=false) for 64-bit results and f64 (no vector path yet).
ढाँचा<वर्ग Op>
TKernel getAddSubFunc(पूर्णांक T, पूर्णांक R)
{
    KernelFunc fptr = nullptr;
    निवड (T)
    {
    स्थिती CV_8U:
        fptr = R == CV_8U ? vec_द्विमान_Kernel<uchar, uchar, v_uint8, लघु, Op, uchar> :
               R == CV_16S ? vec_द्विमान_Kernel<uchar, लघु, v_int16, लघु, Op, लघु> :
               R == CV_32S ? vec_द्विमान_Kernel<uchar, पूर्णांक, v_int16, लघु, Op, लघु> :
               R == CV_32F ? vec_द्विमान_Kernel<uchar, दशांश, v_int16, लघु, Op, लघु> : nullptr;
        थांबा;
    स्थिती CV_8S:
        fptr =
            R == CV_8S ? vec_द्विमान_Kernel<schar, schar, v_int8, लघु, Op, schar> :
            R == CV_16S ? vec_द्विमान_Kernel<schar, लघु, v_int16, लघु, Op, लघु> :
            R == CV_32S ? vec_द्विमान_Kernel<schar, पूर्णांक, v_int16, लघु, Op, लघु> :
            R == CV_32F ? vec_द्विमान_Kernel<schar, दशांश, v_int16, लघु, Op, लघु> : nullptr;
        थांबा;
    स्थिती CV_16U:
        fptr = R == CV_16U ? vec_द्विमान_Kernel<ushort, ushort, v_uint16, पूर्णांक, Op, ushort> :
               R == CV_32S ? vec_द्विमान_Kernel<ushort, पूर्णांक, v_int32, पूर्णांक, Op, पूर्णांक> :
               R == CV_32F ? vec_द्विमान_Kernel<ushort, दशांश, v_int32, पूर्णांक, Op, पूर्णांक> : nullptr;
        थांबा;
    स्थिती CV_16S:
        fptr = R == CV_16S ? vec_द्विमान_Kernel<लघु, लघु, v_int16, पूर्णांक, Op, लघु> :
               R == CV_32S ? vec_द्विमान_Kernel<लघु, पूर्णांक,   v_int32, पूर्णांक, Op, पूर्णांक> :
               R == CV_32F ? vec_द्विमान_Kernel<लघु, दशांश, v_int32, पूर्णांक, Op, पूर्णांक> : nullptr;
        थांबा;
    स्थिती CV_32U:
        fptr = R == CV_32U ? vec_द्विमान_Kernel<अचिन्हित, अचिन्हित, v_uint32, int64_t, Op, अचिन्हित> :
                R == CV_64S ? scalar_द्विमान_Kernel<अचिन्हित, int64_t, int64_t, Op> :
                R == CV_64F ? scalar_द्विमान_Kernel<अचिन्हित, द्विगुण, int64_t, Op> : nullptr;
        थांबा;
    स्थिती CV_32S:
        fptr = R == CV_32S ? vec_द्विमान_Kernel<पूर्णांक, पूर्णांक, v_int32, int64_t, Op, पूर्णांक> :
               R == CV_64S ? scalar_द्विमान_Kernel<पूर्णांक, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_द्विमान_Kernel<पूर्णांक, द्विगुण, int64_t, Op> : nullptr;
        थांबा;
    स्थिती CV_64U:
        fptr = R == CV_64U ? scalar_द्विमान_Kernel<uint64_t, uint64_t, uint64_t, Op> :
               R == CV_64F ? scalar_द्विमान_Kernel<uint64_t, द्विगुण, द्विगुण, Op> : nullptr;
        थांबा;
    स्थिती CV_64S:
        fptr = R == CV_64S ? scalar_द्विमान_Kernel<int64_t, int64_t, int64_t, Op> :
               R == CV_64F ? scalar_द्विमान_Kernel<int64_t, द्विगुण, द्विगुण, Op> : nullptr;
        थांबा;
    स्थिती CV_16F:
        fptr =
        #जर CV_SIMD_16F
        R == CV_16F ? vec_द्विमान_Kernel<hfloat, hfloat, v_float16, दशांश, Op, hfloat> :
        #अन्यथा
        R == CV_16F ? vec_द्विमान_Kernel<hfloat, hfloat, v_float32, दशांश, Op, दशांश> :
        #endif
        R == CV_32F ? vec_द्विमान_Kernel<hfloat, दशांश, v_float32, दशांश, Op, दशांश> : nullptr;
        थांबा;
    स्थिती CV_16BF:
        fptr =
            R == CV_16BF ? vec_द्विमान_Kernel<bfloat, bfloat, v_float32, दशांश, Op, दशांश> :
            R == CV_32F ? vec_द्विमान_Kernel<bfloat, दशांश,  v_float32, दशांश, Op, दशांश> : nullptr;
        थांबा;
    स्थिती CV_32F:
        fptr = R == CV_32F ? vec_द्विमान_Kernel<दशांश, दशांश, v_float32, दशांश, Op, दशांश> : nullptr;
        थांबा;
    स्थिती CV_64F:
        #जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_द्विमान_Kernel<द्विगुण, द्विगुण, v_float64, द्विगुण, Op> : nullptr;
        #अन्यथा
        fptr = R == CV_64F ? scalar_द्विमान_Kernel<द्विगुण, द्विगुण, द्विगुण, Op> : nullptr;
        #endif
        थांबा;
    पूर्वनिर्धारित:
        ;
    }
    परत {fptr, nullptr, 0};
}

TKernel getMulFunc_(पूर्णांक T, पूर्णांक R)
{
    KernelFunc fptr = nullptr;
    निवड (T)
    {
    स्थिती CV_8U:
        fptr =   // scale==1 fast path: whole u8 registers, v_mul_sat widens+clamps inside; Wvec=f32 for scale
            R == CV_8U ? vec_द्विमान_Kernel<uchar, uchar, v_float32, दशांश, EwMul, दशांश, v_uint8> :
            R == CV_32F ? vec_द्विमान_Kernel<uchar, दशांश, v_float32, दशांश, EwMul> : nullptr;
        थांबा;
    स्थिती CV_8S:
        fptr =   // scale==1 fast path: whole s8 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_8S ? vec_द्विमान_Kernel<schar, schar, v_float32, दशांश, EwMul, दशांश, v_int8> :
            R == CV_32F ? vec_द्विमान_Kernel<schar, दशांश, v_float32, दशांश, EwMul> : nullptr;
        थांबा;
    स्थिती CV_16U:
        fptr =   // scale==1 fast path: whole u16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16U ? vec_द्विमान_Kernel<ushort, ushort, v_float32, दशांश, EwMul, दशांश, v_uint16> :
            R == CV_32F ? vec_द्विमान_Kernel<ushort, दशांश,  v_float32, दशांश, EwMul> : nullptr;
        थांबा;
    स्थिती CV_16S:
        fptr =   // scale==1 fast path: whole s16 registers via v_mul_sat; Wvec=f32 for scale
            R == CV_16S ? vec_द्विमान_Kernel<लघु, लघु, v_float32, दशांश, EwMul, दशांश, v_int16> :
            R == CV_32F ? vec_द्विमान_Kernel<लघु, दशांश, v_float32, दशांश, EwMul> : nullptr;
        थांबा;
    स्थिती CV_16F:
        fptr =
            R == CV_16F ? vec_द्विमान_Kernel<hfloat, hfloat, v_float32, दशांश, EwMul> :
            R == CV_32F ? vec_द्विमान_Kernel<hfloat, दशांश,  v_float32, दशांश, EwMul> : nullptr;
        थांबा;
    स्थिती CV_16BF:
        fptr =
            R == CV_16BF ? vec_द्विमान_Kernel<bfloat, bfloat, v_float32, दशांश, EwMul> :
            R == CV_32F ? vec_द्विमान_Kernel<bfloat, दशांश,  v_float32, दशांश, EwMul> : nullptr;
        थांबा;
    स्थिती CV_32F:
        fptr = R == CV_32F ? vec_द्विमान_Kernel<दशांश, दशांश, v_float32, दशांश, EwMul> : nullptr;
        थांबा;
    स्थिती CV_32U:
        fptr =   // scale==1 fast path (where v_mul_sat32 exists): whole u32 registers, widening
                 // multiply + saturating narrow; the f64 work vector serves the scale path
        #जर defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32U ? vec_द्विमान_Kernel<अचिन्हित, अचिन्हित, v_float64, द्विगुण, EwMul, द्विगुण, v_uint32> :
            R == CV_64F ? vec_द्विमान_Kernel<अचिन्हित, द्विगुण,   v_float64, द्विगुण, EwMul> : nullptr;
        #किंवाजर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32U ? vec_द्विमान_Kernel<अचिन्हित, अचिन्हित, v_float64, द्विगुण, EwMul> :
            R == CV_64F ? vec_द्विमान_Kernel<अचिन्हित, द्विगुण,   v_float64, द्विगुण, EwMul> : nullptr;
        #अन्यथा
            R == CV_32U ? scalar_द्विमान_Kernel<अचिन्हित, अचिन्हित, द्विगुण, EwMul> :
            R == CV_64F ? scalar_द्विमान_Kernel<अचिन्हित, द्विगुण,   द्विगुण, EwMul> : nullptr;
        #endif
        थांबा;
    स्थिती CV_32S:
        fptr =   // scale==1 fast path: see CV_32U
        #जर defined(EW_HAVE_MULSAT32) && (CV_SIMD_64F || CV_SIMD_SCALABLE_64F)
            R == CV_32S ? vec_द्विमान_Kernel<पूर्णांक, पूर्णांक,    v_float64, द्विगुण, EwMul, द्विगुण, v_int32> :
            R == CV_64F ? vec_द्विमान_Kernel<पूर्णांक, द्विगुण, v_float64, द्विगुण, EwMul> : nullptr;
        #किंवाजर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
            R == CV_32S ? vec_द्विमान_Kernel<पूर्णांक, पूर्णांक,    v_float64, द्विगुण, EwMul> :
            R == CV_64F ? vec_द्विमान_Kernel<पूर्णांक, द्विगुण, v_float64, द्विगुण, EwMul> : nullptr;
        #अन्यथा
            R == CV_32S ? scalar_द्विमान_Kernel<पूर्णांक, पूर्णांक,    द्विगुण, EwMul> :
            R == CV_64F ? scalar_द्विमान_Kernel<पूर्णांक, द्विगुण, द्विगुण, EwMul> : nullptr;
        #endif
        थांबा;
    स्थिती CV_64U:
        fptr = R == CV_64F ? scalar_द्विमान_Kernel<uint64_t, द्विगुण, द्विगुण, EwMul> : nullptr;
        थांबा;
    स्थिती CV_64S:
        fptr = R == CV_64F ? scalar_द्विमान_Kernel<int64_t,  द्विगुण, द्विगुण, EwMul> : nullptr;
        थांबा;
    स्थिती CV_64F:
        #जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
        fptr = R == CV_64F ? vec_द्विमान_Kernel<द्विगुण, द्विगुण, v_float64, द्विगुण, EwMul> : nullptr;
        #अन्यथा
        fptr = R == CV_64F ? scalar_द्विमान_Kernel<द्विगुण,   द्विगुण, द्विगुण, EwMul> : nullptr;
        #endif
        थांबा;
    पूर्वनिर्धारित:
        ;
    }
    परत {fptr, nullptr, 0};
}

// `checked` (decided by the CALLER from the ORIGINAL input types) selects the divide-by-zero
// policy, INDEPENDENT of the work type R: EwDivInt guards /0 -> 0 for integer-semantics division,
// EwDivFlt does not (a/0 -> inf, saturating on a later cast, as cv::divide does for float inputs).
// It must apply on EVERY row - two wide integers (e.g. 32U / 64S) promote to a 64F work type yet
// still need the integer guard, so the float-work rows can't hardcode EwDivFlt.
TKernel getDivFunc_(पूर्णांक T, पूर्णांक R, तार्किक checked)
{
    KernelFunc fptr = nullptr;
    // SIMD via vecBinaryKernel: div scales the numerator in vec (a*scale/b), EwDivFlt (float a/0 -> inf,
    // saturates on cast) / EwDivInt (v_select guards b==0 -> 0 post-facto). Prefer a DIRECT T->T kernel
    // (1 pass, saturate on store) - emitBinary probes result==T first; a T->f32/f64 kernel serves an
    // explicit float dtype. Work type: f32 for <=16-bit/f16/bf16/f32; f64 for 32/64-bit.
    #define DIV(T_, Tr_, Wv_, W_) (checked ? vec_द्विमान_Kernel<T_, Tr_, Wv_, W_, EwDivInt> \
                                           : vec_द्विमान_Kernel<T_, Tr_, Wv_, W_, EwDivFlt>)
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #define DIVW(T_, Tr_) DIV(T_, Tr_, v_float64, द्विगुण)                 // 32/64-bit, f64 SIMD
#अन्यथा
    #define DIVW(T_, Tr_) (checked ? scalar_द्विमान_Kernel<T_, Tr_, द्विगुण, EwDivInt> \
                                    : scalar_द्विमान_Kernel<T_, Tr_, द्विगुण, EwDivFlt>)
#endif
    निवड (T)
    {
    स्थिती CV_8U:   fptr = R==CV_8U  ? DIV(uchar,  uchar,  v_float32, दशांश) : R==CV_32F ? DIV(uchar,  दशांश, v_float32, दशांश) : nullptr; थांबा;
    स्थिती CV_8S:   fptr = R==CV_8S  ? DIV(schar,  schar,  v_float32, दशांश) : R==CV_32F ? DIV(schar,  दशांश, v_float32, दशांश) : nullptr; थांबा;
    स्थिती CV_16U:  fptr = R==CV_16U ? DIV(ushort, ushort, v_float32, दशांश) : R==CV_32F ? DIV(ushort, दशांश, v_float32, दशांश) : nullptr; थांबा;
    स्थिती CV_16S:  fptr = R==CV_16S ? DIV(लघु,  लघु,  v_float32, दशांश) : R==CV_32F ? DIV(लघु,  दशांश, v_float32, दशांश) : nullptr; थांबा;
    स्थिती CV_16F:  fptr = R==CV_16F ? DIV(hfloat, hfloat, v_float32, दशांश) : R==CV_32F ? DIV(hfloat, दशांश, v_float32, दशांश) : nullptr; थांबा;
    स्थिती CV_16BF: fptr = R==CV_16BF? DIV(bfloat, bfloat, v_float32, दशांश) : R==CV_32F ? DIV(bfloat, दशांश, v_float32, दशांश) : nullptr; थांबा;
    स्थिती CV_32F:  fptr = R==CV_32F ? DIV(दशांश,  दशांश,  v_float32, दशांश) : nullptr; थांबा;
    स्थिती CV_32U:  fptr = R==CV_32U ? DIVW(अचिन्हित, अचिन्हित) : R==CV_64F ? DIVW(अचिन्हित, द्विगुण) : nullptr; थांबा;
    स्थिती CV_32S:  fptr = R==CV_32S ? DIVW(पूर्णांक,      पूर्णांक)      : R==CV_64F ? DIVW(पूर्णांक,      द्विगुण) : nullptr; थांबा;
    स्थिती CV_64U:  fptr = R==CV_64F ? DIVW(uint64_t, द्विगुण) : nullptr; थांबा;
    स्थिती CV_64S:  fptr = R==CV_64F ? DIVW(int64_t,  द्विगुण) : nullptr; थांबा;
    स्थिती CV_64F:  fptr = R==CV_64F ? DIVW(द्विगुण,   द्विगुण) : nullptr; थांबा;
    पूर्वनिर्धारित:      ;
    }
    #undef DIV
    #undef DIVW
    परत {fptr, nullptr, 0};
}


// min / max: T x T -> T for every depth. Native v_min/v_max on the matching lane type (8/16/32-bit
// ints, f16/bf16/f32); 64-bit ints use the scalar path. Op = EwMin or EwMax.
ढाँचा<वर्ग Op>
स्थिर TKernel getMinMaxFunc(पूर्णांक T)
{
    KernelFunc fptr = nullptr;
    निवड (T)
    {
    स्थिती CV_8U:   fptr = vec_द्विमान_Kernel<uchar,    uchar,    v_uint8,   लघु,   Op, uchar>; थांबा;
    स्थिती CV_8S:   fptr = vec_द्विमान_Kernel<schar,    schar,    v_int8,    लघु,   Op, schar>; थांबा;
    स्थिती CV_16U:  fptr = vec_द्विमान_Kernel<ushort,   ushort,   v_uint16,  पूर्णांक,     Op, ushort>; थांबा;
    स्थिती CV_16S:  fptr = vec_द्विमान_Kernel<लघु,    लघु,    v_int16,   पूर्णांक,     Op, लघु>; थांबा;
    स्थिती CV_32U:  fptr = vec_द्विमान_Kernel<अचिन्हित, अचिन्हित, v_uint32,  int64_t, Op, अचिन्हित>; थांबा;
    स्थिती CV_32S:  fptr = vec_द्विमान_Kernel<पूर्णांक,      पूर्णांक,      v_int32,   int64_t, Op, पूर्णांक>; थांबा;
    स्थिती CV_16F:
        #जर CV_SIMD_16F
        fptr = vec_द्विमान_Kernel<hfloat,   hfloat,   v_float16, दशांश,   Op, hfloat>;
        #अन्यथा
        fptr = vec_द्विमान_Kernel<hfloat,   hfloat,   v_float32, दशांश,   Op, दशांश>;
        #endif
        थांबा;
    स्थिती CV_16BF: fptr = vec_द्विमान_Kernel<bfloat, bfloat, v_float32, दशांश, Op, दशांश>; थांबा;
    स्थिती CV_32F:  fptr = vec_द्विमान_Kernel<दशांश, दशांश, v_float32, दशांश, Op, दशांश>; थांबा;
    स्थिती CV_64U:  fptr = scalar_द्विमान_Kernel<uint64_t, uint64_t, uint64_t, Op>; थांबा;
    स्थिती CV_64S:  fptr = scalar_द्विमान_Kernel<int64_t,  int64_t,  int64_t,  Op>; थांबा;
    #जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिती CV_64F:  fptr = vec_द्विमान_Kernel<द्विगुण, द्विगुण, v_float64, द्विगुण, Op, द्विगुण>; थांबा;
    #अन्यथा
    स्थिती CV_64F:  fptr = scalar_द्विमान_Kernel<द्विगुण,   द्विगुण,   द्विगुण,   Op>; थांबा;
    #endif
    पूर्वनिर्धारित:      ;
    }
    परत {fptr, nullptr, 0};
}

// absdiff: |a-b|, T x T -> R.
TKernel getAbsdiffFunc_(पूर्णांक T, पूर्णांक R)
{
    // Signed T has TWO outputs: R==T -> EwAbsdiffS (saturating |a-b| kept in the signed range, native work,
    // ONE pass - the depth cv::absdiff keeps); R==(the unsigned type of the same width) -> EwAbsdiff (the
    // true |a-b| via v_absdiff, for an explicit unsigned dst). Unsigned/float T only produce R==T. Each case
    // returns nullptr for a wrong R (no separate rdepth guard).
    KernelFunc fptr = nullptr;
    निवड (T)
    {
    स्थिती CV_8U:   fptr = R == CV_8U  ? vec_द्विमान_Kernel<uchar,  uchar,  v_uint8,  लघु,   EwAbsdiff, uchar>  : nullptr; थांबा;
    स्थिती CV_16U:  fptr = R == CV_16U ? vec_द्विमान_Kernel<ushort, ushort, v_uint16, पूर्णांक,     EwAbsdiff, ushort> : nullptr; थांबा;
    स्थिती CV_32U:  fptr = R == CV_32U ? vec_द्विमान_Kernel<अचिन्हित, अचिन्हित, v_uint32, int64_t, EwAbsdiff, अचिन्हित> : nullptr; थांबा;
    स्थिती CV_8S:   fptr = R == CV_8S  ? vec_द्विमान_Kernel<schar, schar, v_int8,  लघु,   EwAbsdiffS, schar> :
                         R == CV_8U  ? vec_द्विमान_Kernel<schar, uchar, v_int8,  लघु,   EwAbsdiff,  schar> : nullptr; थांबा;
    स्थिती CV_16S:  fptr = R == CV_16S ? vec_द्विमान_Kernel<लघु, लघु, v_int16, पूर्णांक,     EwAbsdiffS, लघु> :
                         R == CV_16U ? vec_द्विमान_Kernel<लघु, ushort, v_int16, पूर्णांक,     EwAbsdiff,  लघु> : nullptr; थांबा;
    स्थिती CV_32S:  fptr = R == CV_32S ? vec_द्विमान_Kernel<पूर्णांक,   पूर्णांक,   v_int32, int64_t, EwAbsdiffS, पूर्णांक> :
                         R == CV_32U ? vec_द्विमान_Kernel<पूर्णांक, अचिन्हित, v_int32, int64_t, EwAbsdiff,  पूर्णांक> : nullptr; थांबा;
    #जर CV_SIMD_16F
    स्थिती CV_16F:  fptr = R == CV_16F ? vec_द्विमान_Kernel<hfloat, hfloat, v_float16, दशांश, EwAbsdiff, hfloat> : nullptr; थांबा;
    #अन्यथा
    स्थिती CV_16F:  fptr = R == CV_16F ? vec_द्विमान_Kernel<hfloat, hfloat, v_float32, दशांश, EwAbsdiff, दशांश>  : nullptr; थांबा;
    #endif
    स्थिती CV_16BF: fptr = R == CV_16BF ? vec_द्विमान_Kernel<bfloat, bfloat, v_float32, दशांश, EwAbsdiff, दशांश> : nullptr; थांबा;
    स्थिती CV_32F:  fptr = R == CV_32F  ? vec_द्विमान_Kernel<दशांश,  दशांश,  v_float32, दशांश, EwAbsdiff, दशांश> : nullptr; थांबा;
    स्थिती CV_64U:  fptr = R == CV_64U  ? scalar_द्विमान_Kernel<uint64_t, uint64_t, uint64_t, EwAbsdiff> : nullptr; थांबा;
    स्थिती CV_64S:  fptr = R == CV_64S  ? scalar_द्विमान_Kernel<int64_t,  int64_t,  int64_t,  EwAbsdiffS> :
                         R == CV_64U  ? scalar_द्विमान_Kernel<int64_t,  uint64_t, int64_t,  EwAbsdiff> : nullptr; थांबा;
    #जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिती CV_64F:  fptr = R == CV_64F  ? vec_द्विमान_Kernel<द्विगुण, द्विगुण, v_float64, द्विगुण, EwAbsdiff, द्विगुण> : nullptr; थांबा;
    #अन्यथा
    स्थिती CV_64F:  fptr = R == CV_64F  ? scalar_द्विमान_Kernel<द्विगुण, द्विगुण, द्विगुण, EwAbsdiff> : nullptr; थांबा;
    #endif
    पूर्वनिर्धारित:      ;
    }
    परत {fptr, nullptr, 0};
}

// compare: T x T -> u8 mask. Directly-comparable depths (u8/s8/u16/s16/u32/s32/f32) take the SIMD
// vecCompareKernel; f16/bf16 and 64-bit int depths fall back to scalarCompareKernel. The returned kernel
// defaults to a 255 mask in TKernel::flags (cv::compare-compatible); kernel.flags=1 gives a 0/1 mask.
ढाँचा<वर्ग Cmp>
स्थिर KernelFunc compareByType(पूर्णांक T)
{
    निवड (T)
    {
    स्थिती CV_8U:   परत vecCompareKernel<uchar,    v_uint8,   v_uint8,  Cmp>;
    स्थिती CV_8S:   परत vecCompareKernel<schar,    v_int8,    v_uint8,  Cmp>;
    स्थिती CV_16U:  परत vecCompareKernel<ushort,   v_uint16,  v_uint16, Cmp>;
    स्थिती CV_16S:  परत vecCompareKernel<लघु,    v_int16,   v_uint16, Cmp>;
    स्थिती CV_32U:  परत vecCompareKernel<अचिन्हित, v_uint32,  v_uint32, Cmp>;
    स्थिती CV_32S:  परत vecCompareKernel<पूर्णांक,      v_int32,   v_uint32, Cmp>;
    स्थिती CV_32F:  परत vecCompareKernel<दशांश,    v_float32, v_uint32, Cmp>;
    #जर CV_SIMD_16F
    स्थिती CV_16F:  परत vecCompareKernel<hfloat,   v_float16, v_uint16, Cmp>;
    #अन्यथा
    स्थिती CV_16F:  परत vecCompareKernel<hfloat,   v_float32, v_uint32, Cmp>;   // widen f16->f32
    #endif
    स्थिती CV_16BF: परत vecCompareKernel<bfloat,   v_float32, v_uint32, Cmp>;   // widen bf16->f32 (no native)
    स्थिती CV_64U:  परत scalarCompareKernel<uint64_t, uint64_t, Cmp>;
    स्थिती CV_64S:  परत scalarCompareKernel<int64_t,  int64_t,  Cmp>;
    #जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिती CV_64F:  परत vecCompareKernel<द्विगुण,   v_float64, v_uint64, Cmp>;
    #अन्यथा
    स्थिती CV_64F:  परत scalarCompareKernel<द्विगुण,   द्विगुण,   Cmp>;
    #endif
    पूर्वनिर्धारित:      परत nullptr;
    }
}

TKernel getCmpFunc_(शीर्ष op, पूर्णांक T)
{
    // Only 4 physical kernels (eq/ne/gt/ge): LT/LE reuse GT/GE with the operands swapped
    // (a<b == b>a, a<=b == b>=a) via the EW_KERNEL_SWAP01 flag, honored by the executor.
    KernelFunc f = nullptr;
    पूर्णांक flags = 0;              // mask value: 0 flag bits => 0/255 (cv::compare); EW_KERNEL_MASK1 => 0/1
    निवड (op)
    {
    स्थिती OP_CMP_EQ: f = compareByType<EwCmpEq>(T); थांबा;
    स्थिती OP_CMP_NE: f = compareByType<EwCmpNe>(T); थांबा;
    स्थिती OP_CMP_GT: f = compareByType<EwCmpGt>(T); थांबा;
    स्थिती OP_CMP_GE: f = compareByType<EwCmpGe>(T); थांबा;
    स्थिती OP_CMP_LT: f = compareByType<EwCmpGt>(T); flags = EW_KERNEL_SWAP01; थांबा;
    स्थिती OP_CMP_LE: f = compareByType<EwCmpGe>(T); flags = EW_KERNEL_SWAP01; थांबा;
    पूर्वनिर्धारित:        ;
    }
    परत {f, nullptr, flags};
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
ढाँचा<typename T, typename Vvec>
स्थिर पूर्णांक notKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                     स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार, स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                     शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                     स्थिरांक द्विगुण*, पूर्णांक, शून्य*)
{
    जर (width <= 0 || height <= 0) परत 0;
    s0y /= आकार(T);
    dsty /= आकार(T);
    CV_Assert(s0x == 1u || width == 1);
    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    T* dst = (T*)dst_;
    जर (height > 1 && dsty == (माप_प्रकार)width && s0y == (माप_प्रकार)width) { width *= height; height = 1; }
    साठी (पूर्णांक y = 0; y < height; y++, src0 += s0y, dst += dsty)
    {
        पूर्णांक x = 0;
    #जर (CV_SIMD || CV_SIMD_SCALABLE)
        जर constexpr (आकार(T) <= 4)
        {
            स्थिरांक पूर्णांक VECSZ = VTraits<Vvec>::vlanes();
            साठी (; x <= width - VECSZ; x += VECSZ)
                v_store(dst + x, v_not(vx_load(src0 + x)));
        }
    #endif
        साठी (; x < width; x++) dst[x] = (T)~src0[x];
    }
    परत 0;
}

ढाँचा<वर्ग Op>
स्थिर KernelFunc bitwiseByEsz(पूर्णांक esz)
{
    निवड (esz)
    {
    स्थिती 1: परत vec_द्विमान_Kernel<uint8_t,  uint8_t,  v_uint8,  uint8_t,  Op, uint8_t>;
    स्थिती 2: परत vec_द्विमान_Kernel<uint16_t, uint16_t, v_uint16, uint16_t, Op, uint16_t>;
    स्थिती 4: परत vec_द्विमान_Kernel<uint32_t, uint32_t, v_uint32, uint32_t, Op, uint32_t>;
    स्थिती 8: परत scalar_द्विमान_Kernel<uint64_t, uint64_t, uint64_t, Op>;
    पूर्वनिर्धारित: परत nullptr;
    }
}

TKernel getBitwiseFunc_(शीर्ष op, पूर्णांक esz)
{
    KernelFunc f = nullptr;
    निवड (op)
    {
    स्थिती OP_AND: f = bitwiseByEsz<EwAnd>(esz); थांबा;
    स्थिती OP_OR:  f = bitwiseByEsz<EwOr >(esz); थांबा;
    स्थिती OP_XOR: f = bitwiseByEsz<EwXor>(esz); थांबा;
    पूर्वनिर्धारित:     ;
    }
    परत {f, nullptr, 0};
}

TKernel getNotFunc_(पूर्णांक esz)
{
    KernelFunc f = nullptr;
    निवड (esz)
    {
    स्थिती 1: f = notKernel<uint8_t,  v_uint8 >; थांबा;
    स्थिती 2: f = notKernel<uint16_t, v_uint16>; थांबा;
    स्थिती 4: f = notKernel<uint32_t, v_uint32>; थांबा;
    स्थिती 8: f = notKernel<uint64_t, v_uint32>; थांबा;   // SIMD path compiled out for 8-byte -> scalar ~
    पूर्वनिर्धारित: ;
    }
    परत {f, nullptr, 0};
}

// ===========================================================================
// OP_ADDW (addWeighted): dst = a*alpha + b*beta + gamma, params[0..2] = {alpha, beta, gamma}. Two fused
// v_fma in the work type Wvec - f32 SIMD for u8/s8/u16/s16/f16/bf16/f32; the 32-bit-int/64-bit group
// works in f64 (v_float64 SIMD under CV_SIMD_64F || CV_SIMD_SCALABLE_64F, else use_simd=false scalar). Like vecBinaryKernel but
// WITHOUT its multi-channel short-row
// branch: addWeighted takes plain scalar coefficients (a multi-channel scalar is not optimized, matching
// the classic function). The broadcast branches fold the constant operand's contribution once.
// ===========================================================================
ढाँचा<typename T, typename Tr, typename Wvec, typename WT, तार्किक use_simd>
स्थिर पूर्णांक addWeightedKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                             स्थिरांक शून्य* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                             स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                             शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                             स्थिरांक द्विगुण* params, पूर्णांक, शून्य*)
{
    जर (width <= 0 || height <= 0) परत 0;
    s0y /= आकार(T); s1y /= आकार(T); dsty /= आकार(Tr);
    CV_Assert((s0x|s1x) == 1u || (s0x|s1x) + (माप_प्रकार)width == 1u);
    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* src1 = (स्थिरांक T*)src1_;
    Tr* dst = (Tr*)dst_;
    स्थिरांक WT alpha = (WT)params[0], beta = (WT)params[1], gamma = (WT)params[2];
    EW_TRY_COLLAPSE(2);
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    Wvec va{}, vb{}, vg{};
    स्थिरांक पूर्णांक VECSZ = VTraits<Wvec>::vlanes();
    स्थिरांक तार्किक tail = width >= VECSZ*4 && src0_ != dst_ && src1_ != dst_;
    जर constexpr (use_simd) {
        WT fa=alpha, fb=beta, fg=gamma;
        vx_setall_as(&fa, va); vx_setall_as(&fb, vb); vx_setall_as(&fg, vg);
    }
#endif
    साठी (पूर्णांक y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        पूर्णांक x = 0;
    #जर (CV_SIMD || CV_SIMD_SCALABLE)
        जर constexpr (use_simd)
        {
            Wvec a0,a1,a2,a3,b0,b1,b2,b3;
            जर (s0x == s1x) {                                // both arrays contiguous
                साठी (; x < width; x += VECSZ*4) {
                    जर (x+VECSZ*4 > width) { जर (!tail) थांबा; x = width-VECSZ*4; }
                    vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+VECSZ*2, a2, a3);
                    vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+VECSZ*2, b2, b3);
                    a0=v_fma(a0,va,v_fma(b0,vb,vg)); a1=v_fma(a1,va,v_fma(b1,vb,vg));
                    a2=v_fma(a2,va,v_fma(b2,vb,vg)); a3=v_fma(a3,va,v_fma(b3,vb,vg));
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            अन्यथा जर (s1x == 0) {                             // src1 broadcast: b*beta+gamma is constant
                Wvec bb; vx_setall_as(src1, bb); Wvec bc = v_fma(bb, vb, vg);
                साठी (; x < width; x += VECSZ*4) {
                    जर (x+VECSZ*4 > width) { जर (!tail) थांबा; x = width-VECSZ*4; }
                    vx_load_pair_as(src0+x, a0, a1); vx_load_pair_as(src0+x+VECSZ*2, a2, a3);
                    a0=v_fma(a0,va,bc); a1=v_fma(a1,va,bc); a2=v_fma(a2,va,bc); a3=v_fma(a3,va,bc);
                    v_store_pair_as(dst+x, a0, a1); v_store_pair_as(dst+x+VECSZ*2, a2, a3);
                }
            }
            अन्यथा {                                           // src0 broadcast: a*alpha+gamma is constant
                Wvec aa; vx_setall_as(src0, aa); Wvec acg = v_fma(aa, va, vg);
                साठी (; x < width; x += VECSZ*4) {
                    जर (x+VECSZ*4 > width) { जर (!tail) थांबा; x = width-VECSZ*4; }
                    vx_load_pair_as(src1+x, b0, b1); vx_load_pair_as(src1+x+VECSZ*2, b2, b3);
                    b0=v_fma(b0,vb,acg); b1=v_fma(b1,vb,acg); b2=v_fma(b2,vb,acg); b3=v_fma(b3,vb,acg);
                    v_store_pair_as(dst+x, b0, b1); v_store_pair_as(dst+x+VECSZ*2, b2, b3);
                }
            }
        }
    #endif
        जर (s0x == s1x) {
            साठी (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + (WT)src1[x]*beta + gamma);
        } अन्यथा जर (s0x == 0) {
            स्थिरांक WT ac = (WT)src0[0]*alpha + gamma;
            साठी (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src1[x]*beta + ac);
        } अन्यथा {
            स्थिरांक WT bc = (WT)src1[0]*beta + gamma;
            साठी (; x < width; x++) dst[x] = saturate_cast<Tr>((WT)src0[x]*alpha + bc);
        }
    }
    परत 0;
}

TKernel getAddWeightedFunc_(पूर्णांक T, पूर्णांक R)
{
    KernelFunc f = nullptr;
    #define AWS(Tt, Rr) addWeightedKernel<Tt, Rr, v_float32, दशांश, सत्य>
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    #define AWD(Tt, Rr) addWeightedKernel<Tt, Rr, v_float64, द्विगुण, सत्य>       // f64 SIMD
#अन्यथा
    #define AWD(Tt, Rr) addWeightedKernel<Tt, Rr, v_float32, द्विगुण, असत्य>      // scalar (v_float32 unused)
#endif
    निवड (T)
    {
    स्थिती CV_8U:  f = R==CV_8U  ? AWS(uint8_t,  uint8_t)  : R==CV_32F ? AWS(uint8_t,  दशांश) : nullptr; थांबा;
    स्थिती CV_8S:  f = R==CV_8S  ? AWS(int8_t,   int8_t)   : R==CV_32F ? AWS(int8_t,   दशांश) : nullptr; थांबा;
    स्थिती CV_16U: f = R==CV_16U ? AWS(uint16_t, uint16_t) : R==CV_32F ? AWS(uint16_t, दशांश) : nullptr; थांबा;
    स्थिती CV_16S: f = R==CV_16S ? AWS(int16_t,  int16_t)  : R==CV_32F ? AWS(int16_t,  दशांश) : nullptr; थांबा;
    स्थिती CV_16F: f = R==CV_16F ? AWS(hfloat,   hfloat)   : R==CV_32F ? AWS(hfloat,   दशांश) : nullptr; थांबा;
    स्थिती CV_16BF:f = R==CV_32F ? AWS(bfloat,   दशांश) : nullptr; थांबा;
    स्थिती CV_32F: f = R==CV_32F ? AWS(दशांश,    दशांश) : nullptr; थांबा;
    स्थिती CV_32U: f = R==CV_32U ? AWD(अचिन्हित, अचिन्हित) : R==CV_64F ? AWD(अचिन्हित, द्विगुण) : nullptr; थांबा;
    स्थिती CV_32S: f = R==CV_32S ? AWD(पूर्णांक,      पूर्णांक)      : R==CV_64F ? AWD(पूर्णांक,      द्विगुण) : nullptr; थांबा;
    स्थिती CV_64U: f = R==CV_64F ? AWD(uint64_t, द्विगुण) : nullptr; थांबा;
    स्थिती CV_64S: f = R==CV_64F ? AWD(int64_t,  द्विगुण) : nullptr; थांबा;
    स्थिती CV_64F: f = R==CV_64F ? AWD(द्विगुण,   द्विगुण) : nullptr; थांबा;
    पूर्वनिर्धारित: ;
    }
    #undef AWS
    #undef AWD
    परत {f, nullptr, 0};
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
#जर (CV_SIMD || CV_SIMD_SCALABLE)
// VECSZ(T) mask bytes -> T-width lanes (u8 direct, u16/u32 via expand)
स्थिर ओळीत v_uint8  loadSelectMask(स्थिरांक uchar* m, स्थिरांक v_uint8&)  { परत vx_load(m); }
स्थिर ओळीत v_uint16 loadSelectMask(स्थिरांक uchar* m, स्थिरांक v_uint16&) { परत vx_load_expand(m); }
स्थिर ओळीत v_uint32 loadSelectMask(स्थिरांक uchar* m, स्थिरांक v_uint32&) { परत vx_load_expand_q(m); }

स्थिर ओळीत शून्य setallSelect(स्थिरांक uint8_t*  p, v_uint8&  a) { a = vx_setall_u8(*p); }
स्थिर ओळीत शून्य setallSelect(स्थिरांक uint16_t* p, v_uint16& a) { a = vx_setall_u16(*p); }
स्थिर ओळीत शून्य setallSelect(स्थिरांक uint32_t* p, v_uint32& a) { a = vx_setall_u32(*p); }
#endif

ढाँचा<typename T, typename Tvec>
स्थिर पूर्णांक selectKernel(स्थिरांक शून्य* mask_, माप_प्रकार smy, माप_प्रकार smx,
                        स्थिरांक शून्य* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                        स्थिरांक शून्य* src2_, माप_प्रकार s2y, माप_प्रकार s2x,
                        शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                        स्थिरांक द्विगुण*, पूर्णांक, शून्य*)
{
    s1y /= आकार(T);                    // the 1-byte mask's smy stays in bytes == elements
    s2y /= आकार(T);
    dsty /= आकार(T);
    CV_Assert(smx <= 1u && s1x <= 1u && s2x <= 1u);

    स्थिरांक uchar* mask = (स्थिरांक uchar*)mask_;
    स्थिरांक T* src1 = (स्थिरांक T*)src1_;
    स्थिरांक T* src2 = (स्थिरांक T*)src2_;
    T* dst = (T*)dst_;

    जर (height > 1 && dsty == (माप_प्रकार)width && smy == smx*(माप_प्रकार)width &&
        s1y == s1x*(माप_प्रकार)width && s2y == s2x*(माप_प्रकार)width)
    { width *= height; height = 1; }

    पूर्णांक y = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
    जर constexpr (आकार(T) <= 4)
    {
        स्थिरांक पूर्णांक VECSZ = VTraits<Tvec>::vlanes();
        स्थिरांक Tvec z = v_setzero_<Tvec>();

        // n-channel data under a per-pixel mask (the masked-op shape): channel axis = width (2..4),
        // one mask byte per row. Process VECSZ rows per iteration - expand the mask once and
        // interleave it across the channel lanes.
        जर (height > VECSZ && 2 <= width && width <= 4 && smx == 0u && smy == 1u &&
            s1x == 1u && s1y == (माप_प्रकार)width && s2x == 1u && s2y == (माप_प्रकार)width &&
            dsty == (माप_प्रकार)width)
        {
            constexpr पूर्णांक MAXVECSZ = VTraits<Tvec>::max_nlanes;
            T maskbuf[MAXVECSZ*4] = {};      // {} for -Wmaybe-uninitialized only (fully written)
            स्थिरांक पूर्णांक dy = VECSZ;
            साठी (; y + dy <= height; y += dy, mask += dy, src1 += width*dy, src2 += width*dy,
                                                          dst += width*dy)
            {
                Tvec m0 = v_ne(loadSelectMask(mask, z), z), m1, m2, m3;
                जर (width == 2)      v_store_interleave(maskbuf, m0, m0);
                अन्यथा जर (width == 3) v_store_interleave(maskbuf, m0, m0, m0);
                अन्यथा                 v_store_interleave(maskbuf, m0, m0, m0, m0);
                m0 = vx_load(maskbuf);
                m1 = vx_load(maskbuf + VECSZ);
                जर (width > 2) m2 = vx_load(maskbuf + VECSZ*2);
                जर (width > 3) m3 = vx_load(maskbuf + VECSZ*3);
                v_store(dst, v_select(m0, vx_load(src1), vx_load(src2)));
                v_store(dst + VECSZ, v_select(m1, vx_load(src1 + VECSZ), vx_load(src2 + VECSZ)));
                जर (width > 2)
                    v_store(dst + VECSZ*2,
                            v_select(m2, vx_load(src1 + VECSZ*2), vx_load(src2 + VECSZ*2)));
                जर (width > 3)
                    v_store(dst + VECSZ*3,
                            v_select(m3, vx_load(src1 + VECSZ*3), vx_load(src2 + VECSZ*3)));
            }
            // the remaining < VECSZ rows fall through to the per-row path below
        }
        अन्यथा जर (smx == 1)                                   // per-element mask - the common case
        {
            // a branch aliasing dst is fine under the backoff (idempotent select); dst == mask is not
            स्थिरांक तार्किक use_tail_trick = width >= VECSZ*2 && dst_ != mask_;
            Tvec a, b;
            साठी (; y < height; y++, mask += smy, src1 += s1y, src2 += s2y, dst += dsty)
            {
                // a row-constant branch still changes per row
                जर (s1x == 0) setallSelect(src1, a);
                जर (s2x == 0) setallSelect(src2, b);
                पूर्णांक x = 0;
                साठी (; x < width; x += VECSZ)
                {
                    जर (x + VECSZ > width) { जर (!use_tail_trick || x == 0) थांबा; x = width - VECSZ; }
                    Tvec m = v_ne(loadSelectMask(mask + x, z), z);
                    जर (s1x) a = vx_load(src1 + x);
                    जर (s2x) b = vx_load(src2 + x);
                    v_store(dst + x, v_select(m, a, b));
                }
                साठी (; x < width; x++)
                    dst[x] = mask[x] != 0 ? src1[x*s1x] : src2[x*s2x];
            }
            परत 0;
        }
    }
#endif
    साठी (; y < height; y++, mask += smy, src1 += s1y, src2 += s2y, dst += dsty)
    {
        जर (smx == 0)                     // one mask byte per row (n-channel data / broadcast mask)
        {
            स्थिरांक T* s = mask[0] != 0 ? src1 : src2;
            स्थिरांक माप_प्रकार sx = mask[0] != 0 ? s1x : s2x;
            जर ((स्थिरांक शून्य*)s != (स्थिरांक शून्य*)dst)      // row select from dst itself is a no-op
                साठी (पूर्णांक x = 0; x < width; x++) dst[x] = s[x*sx];
        }
        अन्यथा
            साठी (पूर्णांक x = 0; x < width; x++)
                dst[x] = mask[x] != 0 ? src1[x*s1x] : src2[x*s2x];
    }
    परत 0;
}

TKernel getSelectFunc_(पूर्णांक mdepth, पूर्णांक T)
{
    जर (CV_ELEM_SIZE1(mdepth) != 1)      // the mask must be a 1-byte type (u8/s8/bool)
        परत {};
    KernelFunc fptr = nullptr;
    निवड (CV_ELEM_SIZE1(T))
    {
    स्थिती 1: fptr = selectKernel<uint8_t,  v_uint8 >; थांबा;
    स्थिती 2: fptr = selectKernel<uint16_t, v_uint16>; थांबा;
    स्थिती 4: fptr = selectKernel<uint32_t, v_uint32>; थांबा;
    स्थिती 8: fptr = selectKernel<uint64_t, v_uint32>; थांबा;   // SIMD path compiled out -> scalar
    पूर्वनिर्धारित: ;
    }
    परत {fptr, nullptr, 0};
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
#जर (CV_SIMD || CV_SIMD_SCALABLE)
स्थिर ओळीत शून्य setallClamp(स्थिरांक uchar*  p, v_uint8&   a) { a = vx_setall_u8(*p); }
स्थिर ओळीत शून्य setallClamp(स्थिरांक schar*  p, v_int8&    a) { a = vx_setall_s8(*p); }
स्थिर ओळीत शून्य setallClamp(स्थिरांक ushort* p, v_uint16&  a) { a = vx_setall_u16(*p); }
स्थिर ओळीत शून्य setallClamp(स्थिरांक लघु*  p, v_int16&   a) { a = vx_setall_s16(*p); }
स्थिर ओळीत शून्य setallClamp(स्थिरांक अचिन्हित* p, v_uint32& a) { a = vx_setall_u32(*p); }
स्थिर ओळीत शून्य setallClamp(स्थिरांक पूर्णांक*    p, v_int32&   a) { a = vx_setall_s32(*p); }
स्थिर ओळीत शून्य setallClamp(स्थिरांक दशांश*  p, v_float32& a) { a = vx_setall_f32(*p); }
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
स्थिर ओळीत शून्य setallClamp(स्थिरांक द्विगुण* p, v_float64& a) { a = vx_setall_f64(*p); }
#endif
#endif

ढाँचा<typename T, typename Tvec>
स्थिर पूर्णांक clampKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                       स्थिरांक शून्य* lo_, माप_प्रकार s1y, माप_प्रकार s1x,
                       स्थिरांक शून्य* hi_, माप_प्रकार s2y, माप_प्रकार s2x,
                       शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                       स्थिरांक द्विगुण*, पूर्णांक, शून्य*)
{
    s0y /= आकार(T);
    s1y /= आकार(T);
    s2y /= आकार(T);
    dsty /= आकार(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);

    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* lo = (स्थिरांक T*)lo_;
    स्थिरांक T* hi = (स्थिरांक T*)hi_;
    T* dst = (T*)dst_;

    जर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width &&
        s1y == s1x*(माप_प्रकार)width && s2y == s2x*(माप_प्रकार)width)
    { width *= height; height = 1; }

    साठी (पूर्णांक y = 0; y < height; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
    {
        पूर्णांक x = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
        जर (s0x == 1)
        {
            स्थिरांक पूर्णांक VECSZ = VTraits<Tvec>::vlanes();
            स्थिरांक तार्किक use_tail_trick = width >= VECSZ*2 && lo_ != dst_ && hi_ != dst_;
            Tvec vlo, vhi;
            जर (s1x == 0) setallClamp(lo, vlo);
            जर (s2x == 0) setallClamp(hi, vhi);
            साठी (; x < width; x += VECSZ)
            {
                जर (x + VECSZ > width) { जर (!use_tail_trick || x == 0) थांबा; x = width - VECSZ; }
                जर (s1x) vlo = vx_load(lo + x);
                जर (s2x) vhi = vx_load(hi + x);
                v_store(dst + x, v_min(v_max(vx_load(src0 + x), vlo), vhi));
            }
        }
#endif
        साठी (; x < width; x++)
        {
            T v = src0[x*s0x], l = lo[x*s1x], h = hi[x*s2x];
            dst[x] = std::min(std::max(v, l), h);
        }
    }
    परत 0;
}

// Scalar-only clamp for the depths without a native SIMD lane type here (f16/bf16 - compared in
// float; 64-bit ints).
ढाँचा<typename T, typename WT>
स्थिर पूर्णांक scalarClampKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                             स्थिरांक शून्य* lo_, माप_प्रकार s1y, माप_प्रकार s1x,
                             स्थिरांक शून्य* hi_, माप_प्रकार s2y, माप_प्रकार s2x,
                             शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                             स्थिरांक द्विगुण*, पूर्णांक, शून्य*)
{
    s0y /= आकार(T); s1y /= आकार(T); s2y /= आकार(T); dsty /= आकार(T);
    CV_Assert(s0x <= 1u && s1x <= 1u && s2x <= 1u);
    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* lo = (स्थिरांक T*)lo_;
    स्थिरांक T* hi = (स्थिरांक T*)hi_;
    T* dst = (T*)dst_;
    जर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width &&
        s1y == s1x*(माप_प्रकार)width && s2y == s2x*(माप_प्रकार)width)
    { width *= height; height = 1; }
    साठी (पूर्णांक y = 0; y < height; y++, src0 += s0y, lo += s1y, hi += s2y, dst += dsty)
        साठी (पूर्णांक x = 0; x < width; x++)
        {
            WT v = (WT)src0[x*s0x], l = (WT)lo[x*s1x], h = (WT)hi[x*s2x];
            dst[x] = saturate_cast<T>(std::min(std::max(v, l), h));
        }
    परत 0;
}

TKernel getClampFunc_(पूर्णांक T)
{
    KernelFunc fptr = nullptr;
    निवड (T)
    {
    स्थिती CV_8U:   fptr = clampKernel<uchar,    v_uint8  >; थांबा;
    स्थिती CV_8S:   fptr = clampKernel<schar,    v_int8   >; थांबा;
    स्थिती CV_16U:  fptr = clampKernel<ushort,   v_uint16 >; थांबा;
    स्थिती CV_16S:  fptr = clampKernel<लघु,    v_int16  >; थांबा;
    स्थिती CV_32U:  fptr = clampKernel<अचिन्हित, v_uint32 >; थांबा;
    स्थिती CV_32S:  fptr = clampKernel<पूर्णांक,      v_int32  >; थांबा;
    स्थिती CV_32F:  fptr = clampKernel<दशांश,    v_float32>; थांबा;
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिती CV_64F:  fptr = clampKernel<द्विगुण,   v_float64>; थांबा;
#अन्यथा
    स्थिती CV_64F:  fptr = scalarClampKernel<द्विगुण, द्विगुण>; थांबा;
#endif
    स्थिती CV_16F:  fptr = scalarClampKernel<hfloat, दशांश>; थांबा;
    स्थिती CV_16BF: fptr = scalarClampKernel<bfloat, दशांश>; थांबा;
    स्थिती CV_64U:  fptr = scalarClampKernel<uint64_t, uint64_t>; थांबा;
    स्थिती CV_64S:  fptr = scalarClampKernel<int64_t,  int64_t >; थांबा;
    पूर्वनिर्धारित: ;
    }
    परत {fptr, nullptr, 0};
}

स्थिर पूर्णांक expandKernel(माप_प्रकार s0y, माप_प्रकार s0x,
                        शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                        पूर्णांक esz1)
{
    uchar* dst = (uchar*)dst_;
    जर (s0x == 0u && width > 1) {
        पूर्णांक h = s0y > 0u ? height : 1;
        CV_Assert(esz1 == 1 || esz1 == 2 || esz1 == 4 || esz1 == 8);
        साठी (पूर्णांक y = 0; y < h; y++) {
            जर (esz1 == 1) {
                uchar* rowptr = dst + dsty*y;
                uchar val = rowptr[0];
                साठी (पूर्णांक x = 1; x < width; x++) rowptr[x] = val;
            }
            अन्यथा जर (esz1 == 2) {
                ushort* rowptr = (ushort*)(dst + dsty*y);
                ushort val = rowptr[0];
                साठी (पूर्णांक x = 1; x < width; x++) rowptr[x] = val;
            }
            अन्यथा जर (esz1 == 4) {
                अचिन्हित* rowptr = (अचिन्हित*)(dst + dsty*y);
                अचिन्हित val = rowptr[0];
                साठी (पूर्णांक x = 1; x < width; x++) rowptr[x] = val;
            }
            अन्यथा {
                uint64_t* rowptr = (uint64_t*)(dst + dsty*y);
                uint64_t val = rowptr[0];
                साठी (पूर्णांक x = 1; x < width; x++) rowptr[x] = val;
            }
        }
    }

    जर (s0y == 0) {
        साठी (पूर्णांक y = 1; y < height; y++)
            स_नकल(dst + dsty*y, dst, (माप_प्रकार)esz1*width);
    }
    परत 0;
}

स्थिर पूर्णांक castKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                      स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                      स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                      शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                      स्थिरांक द्विगुण* params, पूर्णांक esz1, शून्य* userdata)
{
    द्विमान_Func castfunc = (द्विमान_Func)userdata;
    castfunc((स्थिरांक uchar*)src0_, s0y, nullptr, 0, (uchar*)dst_, dsty,
             Size((s0x > 0u ? width : 1), (s0y > 0u ? height : 1)), (शून्य*)params);
    परत expandKernel(s0y, s0x, dst_, dsty, width, height, esz1);
}

// ===========================================================================
// Per-op entry points (this baseline). The op-level getElemwiseFunc() dispatcher and the regular
// get*Func() forwarders live in arithm.dispatch.cpp.
// ===========================================================================

// OP_CAST / OP_CONVERT_SCALE: wrap core's convert BinaryFunc (carried in kernel.userdata) in
// castKernel, which runs it over the distinct sub-region and then expands across broadcast axes.
TKernel getCastFunc_(पूर्णांक sdepth, पूर्णांक ddepth, तार्किक scaled)
{
    द्विमान_Func cvt = scaled ? getConvertScaleFunc(sdepth, ddepth) : getConvertFunc(sdepth, ddepth);
    परत {castKernel, (शून्य*)cvt, CV_ELEM_SIZE1(ddepth)};
}

// Non-template wrappers around the templated kernel selectors (ADD/SUB share getAddSubFunc, MIN/MAX
// share getMinMaxFunc). min/max are T x T -> T, so R is ignored (the dispatcher validates R == T).
TKernel getAddFunc_(पूर्णांक T, पूर्णांक R) { परत getAddSubFunc<EwAdd>(T, R); }
TKernel getSubFunc_(पूर्णांक T, पूर्णांक R) { परत getAddSubFunc<EwSub>(T, R); }
TKernel getMinFunc_(पूर्णांक T, पूर्णांक R) { (शून्य)R; परत getMinMaxFunc<EwMin>(T); }
TKernel getMaxFunc_(पूर्णांक T, पूर्णांक R) { (शून्य)R; परत getMinMaxFunc<EwMax>(T); }

TKernel getHypotFunc_(पूर्णांक T, पूर्णांक R)
{
    जर (R != T)
        परत {};
    KernelFunc fptr = nullptr;
    निवड (T)
    {
    स्थिती CV_16F:  fptr = vec_द्विमान_Kernel<hfloat, hfloat, v_float32, दशांश, EwHypot>; थांबा;
    स्थिती CV_16BF: fptr = vec_द्विमान_Kernel<bfloat, bfloat, v_float32, दशांश, EwHypot>; थांबा;
    स्थिती CV_32F:  fptr = vec_द्विमान_Kernel<दशांश,  दशांश,  v_float32, दशांश, EwHypot>; थांबा;
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिती CV_64F:  fptr = vec_द्विमान_Kernel<द्विगुण, द्विगुण, v_float64, द्विगुण, EwHypot>; थांबा;
#अन्यथा
    स्थिती CV_64F:  fptr = scalar_द्विमान_Kernel<द्विगुण, द्विगुण, द्विगुण, EwHypot>; थांबा;
#endif
    पूर्वनिर्धारित: ;
    }
    परत {fptr, nullptr, 0};
}

TKernel getAtan2Func_(पूर्णांक T, पूर्णांक R)
{
    जर (R != T)
        परत {};
    KernelFunc fptr = nullptr;
    निवड (T)
    {
    स्थिती CV_16F:  fptr = vec_द्विमान_Kernel<hfloat, hfloat, v_float32, दशांश, EwAtan2>; थांबा;
    स्थिती CV_16BF: fptr = vec_द्विमान_Kernel<bfloat, bfloat, v_float32, दशांश, EwAtan2>; थांबा;
    स्थिती CV_32F:  fptr = vec_द्विमान_Kernel<दशांश,  दशांश,  v_float32, दशांश, EwAtan2>; थांबा;
    स्थिती CV_64F:  fptr = scalar_द्विमान_Kernel<द्विगुण, द्विगुण, द्विगुण, EwAtan2>; थांबा;   // exact std::atan2
    पूर्वनिर्धारित: ;
    }
    परत {fptr, nullptr, 0};
}

#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
