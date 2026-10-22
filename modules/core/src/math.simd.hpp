// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Element-wise MATH kernels (sqrt/exp/log/sin/cos/tanh/erf/relu) and SELECT for the new arithmetic
// engine, SIMD-dispatched per CPU baseline - the unary/ternary sibling of arithm.simd.hpp.
//
// This file is compiled once per SIMD baseline (registered via ocv_add_dispatched_file). The per-op
// entry points get*Func_(...) live in cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE and return the kernel
// optimized for that baseline; the regular get*Func dispatchers live in math.dispatch.cpp.
//
// Kernel shape (house style of arithm.simd.hpp):
//  - one 2D tile; per-row outer loop with stepy (bytes); dst contiguous in x; stepx in {0,1}.
//  - continuity collapse 2D->1D when every operand+dst is gap-free.
//  - halide right-edge backoff for the SIMD tail, SUPPRESSED when dst aliases an input (in-place
//    unary math would re-apply Op to already-written values).
//
// Math is T -> T over the four float depths: f32/f64 compute natively (v_exp & co exist for both);
// f16/bf16 ride the f32 hub (vx_load_pair_as widens one native vector into two f32 vectors, the
// saturating v_store_pair_as packs them back) - more accurate than a native f16 polynomial and
// works on every baseline. Integer inputs never reach these kernels: emitUnary computes integer
// math in the float domain and casts.

#ضمن "opencv2/core/hal/intrin.hpp"
#ضمن "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#ضمن "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#ضمن <cmath>

نطاق cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#جيڪڏهن_وصف_نه_ٿيل CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#إذا (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
ساكن مضمن فراغ v_store_pair_as(hfloat* p, ثابت v_float32& a, ثابت v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
ساكن مضمن فراغ v_store_pair_as(bfloat* p, ثابت v_float32& a, ثابت v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#جيڪڏهن_پورو
#جيڪڏهن_پورو // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

نطاق ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(چوٽي op, صحيح T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(صحيح T, صحيح R);                   // OP_POW, T x T -> T (R must equal T)

#جيڪڏهن_وصف_نه_ٿيل CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
بنية MSqrt {
    سانچو<قسم_نالو V> ساكن V vec(ثابت V& x) { أرجع v_sqrt(x); }
    سانچو<قسم_نالو W> ساكن W scl(W x) { أرجع std::جذر(x); }
};
بنية MExp {
    سانچو<قسم_نالو V> ساكن V vec(ثابت V& x) { أرجع v_exp(x); }
    سانچو<قسم_نالو W> ساكن W scl(W x) { أرجع std::قوت_نما(x); }
};
بنية MLog {
    سانچو<قسم_نالو V> ساكن V vec(ثابت V& x) { أرجع v_log(x); }
    سانچو<قسم_نالو W> ساكن W scl(W x) { أرجع std::لاگرٿم(x); }
};
بنية MSin {
    سانچو<قسم_نالو V> ساكن V vec(ثابت V& x) { أرجع v_sin(x); }
    سانچو<قسم_نالو W> ساكن W scl(W x) { أرجع std::جيب(x); }
};
بنية MCos {
    سانچو<قسم_نالو V> ساكن V vec(ثابت V& x) { أرجع v_cos(x); }
    سانچو<قسم_نالو W> ساكن W scl(W x) { أرجع std::جيب_تمام(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
بنية MTanh {
    ساكن v_float32 vec(ثابت v_float32& x)
    {
        ثابت v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_جوڙيو(cx, cx));
        أرجع v_div(v_sub(e, one), v_جوڙيو(e, one));
    }
#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ساكن v_float64 vec(ثابت v_float64& x)
    {
        ثابت v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_جوڙيو(cx, cx));
        أرجع v_div(v_sub(e, one), v_جوڙيو(e, one));
    }
#جيڪڏهن_پورو
    سانچو<قسم_نالو W> ساكن W scl(W x) { أرجع std::tanh(x); }
};
بنية MErf {
    ساكن v_float32 vec(ثابت v_float32& x) { أرجع v_erf(x); }
#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    ساكن v_float64 vec(ثابت v_float64& x)
    {
        مزدوج buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        لكل (صحيح i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        أرجع vx_ڀريو(buf);
    }
#جيڪڏهن_پورو
    سانچو<قسم_نالو W> ساكن W scl(W x) { أرجع std::erf(x); }
};
بنية MRelu {
    سانچو<قسم_نالو V> ساكن V vec(ثابت V& x) { أرجع v_max(x, v_setzero_<V>()); }
    سانچو<قسم_نالو W> ساكن W scl(W x) { أرجع x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
سانچو<قسم_نالو T, قسم_نالو Wvec, صنف Op>
ساكن صحيح vecUnaryKernel(ثابت فراغ* src0_, size_t s0y, size_t s0x,
                          ثابت فراغ*, size_t, size_t, ثابت فراغ*, size_t, size_t,
                          فراغ* dst_, size_t dsty, صحيح ويڪر, صحيح اوچائي,
                          ثابت مزدوج*, صحيح, فراغ*)
{
    s0y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x <= 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    T* dst = (T*)dst_;
    استعمال WT = قسم_نالو VTraits<Wvec>::lane_type;

    إذا (اوچائي > 1 && dsty == (size_t)ويڪر && s0y == s0x*(size_t)ويڪر) { ويڪر *= اوچائي; اوچائي = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ثابت صحيح urows = (s0y == 0 && اوچائي > 1) ? 1 : اوچائي;

    لكل (صحيح y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        إذا (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            لكل (صحيح x = 0; x < ويڪر; x++) dst[x] = v;
            استمر;
        }
        صحيح x = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        ثابت صحيح VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ثابت منطقي use_پڇ_trick = ويڪر >= VECSZ*4 && src0_ != dst_;
        لكل (; x < ويڪر; x += VECSZ*2)
        {
            إذا (x + VECSZ*2 > ويڪر) { إذا (!use_پڇ_trick || x == 0) اكسر; x = ويڪر - VECSZ*2; }
            Wvec a0, a1;
            vx_ڀريو_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#جيڪڏهن_پورو
        لكل (; x < ويڪر; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    لكل (صحيح y = urows; y < اوچائي; y++)
        يادگيري_نقل(dst + (size_t)y*dsty, dst, (size_t)ويڪر*حجم(T));
    أرجع 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
سانچو<قسم_نالو T, قسم_نالو WT, صنف Op>
ساكن صحيح scalarUnaryKernel(ثابت فراغ* src0_, size_t s0y, size_t s0x,
                             ثابت فراغ*, size_t, size_t, ثابت فراغ*, size_t, size_t,
                             فراغ* dst_, size_t dsty, صحيح ويڪر, صحيح اوچائي,
                             ثابت مزدوج*, صحيح, فراغ*)
{
    s0y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x <= 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    T* dst = (T*)dst_;
    إذا (اوچائي > 1 && dsty == (size_t)ويڪر && s0y == s0x*(size_t)ويڪر) { ويڪر *= اوچائي; اوچائي = 1; }
    ثابت صحيح urows = (s0y == 0 && اوچائي > 1) ? 1 : اوچائي;   // vertical broadcast: 1 row + copies
    لكل (صحيح y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        إذا (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            لكل (صحيح x = 0; x < ويڪر; x++) dst[x] = v;
            استمر;
        }
        لكل (صحيح x = 0; x < ويڪر; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    لكل (صحيح y = urows; y < اوچائي; y++)
        يادگيري_نقل(dst + (size_t)y*dsty, dst, (size_t)ويڪر*حجم(T));
    أرجع 0;
}

// ===========================================================================
// OP_POW: dst = pow(x, y), T x T -> T over the float depths. Exact std::pow semantics.
//
// The exponent is USUALLY a broadcast scalar (pow(x, 2), texpr literals ride as 0-dim consts with
// stepx == 0) - dispatched PER ROW to the important special cases: y==2 -> x*x, y==3 -> x*x*x,
// y==0.5 -> v_sqrt, y==1 -> copy, y==0 -> fill 1 (std::pow(anything, 0) == 1, NaN included).
// Everything else - and the per-element exponent - runs the general vectorized path
// exp(y * log(x)), which is only valid for x > 0: any lane with x <= 0 falls back to scalar
// std::pow for the whole vector pair (v_check_any per pair; negative/zero bases are rare, and the
// scalar path preserves every std::pow subtlety - signed results for integer y, NaN for
// fractional y, the x == 0 family). One knowing deviation: y==0.5 uses v_sqrt, so pow(-0., .5)
// returns -0. instead of std::pow's +0.
//
// The halide right-edge tail backoff is used in every SIMD loop, SUPPRESSED when dst aliases an
// input: pow is not idempotent, so an in-place backoff would re-read already-written values (the
// overlap region is otherwise just recomputed from the untouched source). Suppressed rows finish
// in the scalar tail.
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
ساكن مضمن v_float32 vxSetallW(عائم v,  ثابت v_float32&) { أرجع vx_setall_f32(v); }
#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ساكن مضمن v_float64 vxSetallW(مزدوج v, ثابت v_float64&) { أرجع vx_setall_f64(v); }
#جيڪڏهن_پورو
#جيڪڏهن_پورو

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
سانچو<قسم_نالو T, قسم_نالو WT>
ساكن صحيح scalarPowKernel(ثابت فراغ* src0_, size_t s0y, size_t s0x,
                           ثابت فراغ* src1_, size_t s1y, size_t s1x,
                           ثابت فراغ*, size_t, size_t,
                           فراغ* dst_, size_t dsty, صحيح ويڪر, صحيح اوچائي,
                           ثابت مزدوج*, صحيح, فراغ*)
{
    s0y /= حجم(T); s1y /= حجم(T); dsty /= حجم(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    T* dst = (T*)dst_;
    إذا (اوچائي > 1 && dsty == (size_t)ويڪر && s0y == s0x*(size_t)ويڪر && s1y == s1x*(size_t)ويڪر)
    { ويڪر *= اوچائي; اوچائي = 1; }
    لكل (صحيح y = 0; y < اوچائي; y++, src0 += s0y, src1 += s1y, dst += dsty)
        لكل (صحيح x = 0; x < ويڪر; x++)
            dst[x] = saturate_cast<T>(std::قوت((WT)src0[x*s0x], (WT)src1[x*s1x]));
    أرجع 0;
}

سانچو<قسم_نالو T, قسم_نالو Wvec>
ساكن صحيح powKernel(ثابت فراغ* src0_, size_t s0y, size_t s0x,
                     ثابت فراغ* src1_, size_t s1y, size_t s1x,
                     ثابت فراغ*, size_t, size_t,
                     فراغ* dst_, size_t dsty, صحيح ويڪر, صحيح اوچائي,
                     ثابت مزدوج*, صحيح, فراغ*)
{
    s0y /= حجم(T);
    s1y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    T* dst = (T*)dst_;
    استعمال WT = قسم_نالو VTraits<Wvec>::lane_type;

    إذا (اوچائي > 1 && dsty == (size_t)ويڪر && s0y == s0x*(size_t)ويڪر && s1y == s1x*(size_t)ويڪر)
    { ويڪر *= اوچائي; اوچائي = 1; }

    [[maybe_unused]] ثابت منطقي پڇ_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ثابت صحيح urows = (s0y == 0 && s1y == 0 && اوچائي > 1) ? 1 : اوچائي;

    لكل (صحيح y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        صحيح x = 0;
        إذا (s1x == 0)                                   // scalar exponent for this row
        {
            ثابت WT p = (WT)src1[0];
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
            ثابت صحيح VECSZ = VTraits<Wvec>::vlanes();
            إذا (s0x == 1)
            {
                إذا (p == WT(2) || p == WT(3))
                {
                    لكل (; x < ويڪر; x += VECSZ*2)
                    {
                        إذا (x + VECSZ*2 > ويڪر) { إذا (!پڇ_trick || x == 0) اكسر; x = ويڪر - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ڀريو_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        إذا (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                وإلا إذا (p == WT(0.5))
                {
                    لكل (; x < ويڪر; x += VECSZ*2)
                    {
                        إذا (x + VECSZ*2 > ويڪر) { إذا (!پڇ_trick || x == 0) اكسر; x = ويڪر - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ڀريو_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                وإلا إذا (p == WT(1))
                {
                    إذا ((ثابت فراغ*)src0 != (ثابت فراغ*)dst)
                        لكل (; x < ويڪر; x++) dst[x] = src0[x];
                    x = ويڪر;
                }
                وإلا إذا (p == WT(0))
                {
                    ثابت T one = saturate_cast<T>(1);
                    لكل (; x < ويڪر; x++) dst[x] = one;
                }
                وإلا إذا (p == WT(-0.5))
                {
                    ثابت Wvec one = vxSetallW(WT(1), Wvec());
                    لكل (; x < ويڪر; x += VECSZ*2)
                    {
                        إذا (x + VECSZ*2 > ويڪر) { إذا (!پڇ_trick || x == 0) اكسر; x = ويڪر - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ڀريو_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                وإلا إذا (p == std::rint(p) && std::مطلق_قدر(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ثابت صحيح ip = (صحيح)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ثابت Wvec one = vxSetallW(WT(1), Wvec());
                    لكل (; x < ويڪر; x += VECSZ*2)
                    {
                        إذا (x + VECSZ*2 > ويڪر) { إذا (!پڇ_trick || x == 0) اكسر; x = ويڪر - VECSZ*2; }
                        Wvec b0, b1;
                        vx_ڀريو_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        لكل (صحيح q = ap; q > 1; q >>= 1)
                        {
                            إذا (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        إذا (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                وإلا                                    // general scalar exponent: exp(p * log(x))
                {
                    ثابت Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    لكل (; x < ويڪر; x += VECSZ*2)
                    {
                        إذا (x + VECSZ*2 > ويڪر) { إذا (!پڇ_trick || x == 0) اكسر; x = ويڪر - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ڀريو_pair_as(src0 + x, a0, a1);
                        إذا (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            لكل (صحيح i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::قوت((WT)src0[x + i], p));
                            استمر;
                        }
                        a0 = v_log(a0);
                        a1 = v_log(a1);
                        a0 = v_mul(vp, a0);
                        a1 = v_mul(vp, a1);
                        a0 = v_exp(a0);
                        a1 = v_exp(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
            }
#جيڪڏهن_پورو
            لكل (; x < ويڪر; x++)
                dst[x] = saturate_cast<T>(std::قوت((WT)src0[x*s0x], p));
            استمر;
        }
        // per-element exponent
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
        إذا (s0x == 1)
        {
            ثابت صحيح VECSZ = VTraits<Wvec>::vlanes();
            ثابت Wvec z = v_setzero_<Wvec>();
            لكل (; x < ويڪر; x += VECSZ*2)
            {
                إذا (x + VECSZ*2 > ويڪر) { إذا (!پڇ_trick || x == 0) اكسر; x = ويڪر - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_ڀريو_pair_as(src0 + x, a0, a1);
                vx_ڀريو_pair_as(src1 + x, b0, b1);
                إذا (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    لكل (صحيح i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::قوت((WT)src0[x + i], (WT)src1[x + i]));
                    استمر;
                }
                a0 = v_log(a0);
                a1 = v_log(a1);
                a0 = v_mul(b0, a0);
                a1 = v_mul(b1, a1);
                a0 = v_exp(a0);
                a1 = v_exp(a1);
                v_store_pair_as(dst + x, a0, a1);
            }
        }
#جيڪڏهن_پورو
        لكل (; x < ويڪر; x++)
            dst[x] = saturate_cast<T>(std::قوت((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    لكل (صحيح y = urows; y < اوچائي; y++)
        يادگيري_نقل(dst + (size_t)y*dsty, dst, (size_t)ويڪر*حجم(T));
    أرجع 0;
}

TKernel getPowFunc_(صحيح T, صحيح R)
{
    إذا (R != T)
        أرجع {};
    KernelFunc fptr = خالي_اشارو;
    بدل (T)
    {
    حالة CV_16F:  fptr = powKernel<hfloat, v_float32>; اكسر;
    حالة CV_16BF: fptr = powKernel<bfloat, v_float32>; اكسر;
    حالة CV_32F:  fptr = powKernel<عائم,  v_float32>; اكسر;
#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالة CV_64F:  fptr = powKernel<مزدوج, v_float64>; اكسر;
#وإلا
    حالة CV_64F:  fptr = scalarPowKernel<مزدوج, مزدوج>; اكسر;
#جيڪڏهن_پورو
    افتراضي: ;
    }
    أرجع {fptr, خالي_اشارو, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
سانچو<صنف Op>
ساكن KernelFunc math_By_اونهائي(صحيح T)
{
    بدل (T)
    {
    حالة CV_16F:  أرجع vecUnaryKernel<hfloat, v_float32, Op>;
    حالة CV_16BF: أرجع vecUnaryKernel<bfloat, v_float32, Op>;
    حالة CV_32F:  أرجع vecUnaryKernel<عائم,  v_float32, Op>;
#إذا CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالة CV_64F:  أرجع vecUnaryKernel<مزدوج, v_float64, Op>;
#وإلا
    حالة CV_64F:  أرجع scalarUnaryKernel<مزدوج, مزدوج, Op>;
#جيڪڏهن_پورو
    افتراضي:      أرجع خالي_اشارو;
    }
}

TKernel getMathFunc_(چوٽي op, صحيح T)
{
    KernelFunc f = خالي_اشارو;
    بدل (op)
    {
    حالة OP_SQRT: f = math_By_اونهائي<MSqrt>(T); اكسر;
    حالة OP_EXP:  f = math_By_اونهائي<MExp >(T); اكسر;
    حالة OP_LOG:  f = math_By_اونهائي<MLog >(T); اكسر;
    حالة OP_SIN:  f = math_By_اونهائي<MSin >(T); اكسر;
    حالة OP_COS:  f = math_By_اونهائي<MCos >(T); اكسر;
    حالة OP_TANH: f = math_By_اونهائي<MTanh>(T); اكسر;
    حالة OP_RELU: f = math_By_اونهائي<MRelu>(T); اكسر;
    حالة OP_ERF:  f = math_By_اونهائي<MErf >(T); اكسر;
    افتراضي: ;
    }
    أرجع {f, خالي_اشارو, 0};
}


#جيڪڏهن_پورو // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_پڇاڙي
}} // namespace cv::ew
