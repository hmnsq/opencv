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

#شامل "opencv2/core/hal/intrin.hpp"
#شامل "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#شامل "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#شامل <cmath>

نامخلا cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#اگر (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
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

#اگر_ختم
#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

نامخلا ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(چوٹی op, صحیح T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(صحیح T, صحیح R);                   // OP_POW, T x T -> T (R must equal T)

#اگر_غیر_تعریف_شدہ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
ساخت MSqrt {
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& x) { واپس v_sqrt(x); }
    سانچہ<قسم_نام W> ساکن W scl(W x) { واپس std::جذر(x); }
};
ساخت MExp {
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& x) { واپس v_exp(x); }
    سانچہ<قسم_نام W> ساکن W scl(W x) { واپس std::قوت_نما(x); }
};
ساخت MLog {
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& x) { واپس v_log(x); }
    سانچہ<قسم_نام W> ساکن W scl(W x) { واپس std::لوگارتھم(x); }
};
ساخت MSin {
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& x) { واپس v_sin(x); }
    سانچہ<قسم_نام W> ساکن W scl(W x) { واپس std::جیب(x); }
};
ساخت MCos {
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& x) { واپس v_cos(x); }
    سانچہ<قسم_نام W> ساکن W scl(W x) { واپس std::جیب_تمام(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
ساخت MTanh {
    ساکن v_float32 vec(ثابت v_float32& x)
    {
        ثابت v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_جوڑو(cx, cx));
        واپس v_div(v_sub(e, one), v_جوڑو(e, one));
    }
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ساکن v_float64 vec(ثابت v_float64& x)
    {
        ثابت v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_جوڑو(cx, cx));
        واپس v_div(v_sub(e, one), v_جوڑو(e, one));
    }
#اگر_ختم
    سانچہ<قسم_نام W> ساکن W scl(W x) { واپس std::tanh(x); }
};
ساخت MErf {
    ساکن v_float32 vec(ثابت v_float32& x) { واپس v_erf(x); }
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    ساکن v_float64 vec(ثابت v_float64& x)
    {
        دوہرا buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        برائے (صحیح i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        واپس vx_بھرو(buf);
    }
#اگر_ختم
    سانچہ<قسم_نام W> ساکن W scl(W x) { واپس std::erf(x); }
};
ساخت MRelu {
    سانچہ<قسم_نام V> ساکن V vec(ثابت V& x) { واپس v_max(x, v_setzero_<V>()); }
    سانچہ<قسم_نام W> ساکن W scl(W x) { واپس x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
سانچہ<قسم_نام T, قسم_نام Wvec, جماعت Op>
ساکن صحیح vecUnaryKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                          ثابت خالی*, size_t, size_t, ثابت خالی*, size_t, size_t,
                          خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                          ثابت دوہرا*, صحیح, خالی*)
{
    s0y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x <= 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    T* dst = (T*)dst_;
    استعمال WT = قسم_نام VTraits<Wvec>::lane_type;

    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && s0y == s0x*(size_t)چوڑائی) { چوڑائی *= اونچائی; اونچائی = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ثابت صحیح urows = (s0y == 0 && اونچائی > 1) ? 1 : اونچائی;

    برائے (صحیح y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        اگر (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            برائے (صحیح x = 0; x < چوڑائی; x++) dst[x] = v;
            جاری;
        }
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        ثابت صحیح VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ثابت منطقی use_دم_trick = چوڑائی >= VECSZ*4 && src0_ != dst_;
        برائے (; x < چوڑائی; x += VECSZ*2)
        {
            اگر (x + VECSZ*2 > چوڑائی) { اگر (!use_دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ*2; }
            Wvec a0, a1;
            vx_بھرو_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#اگر_ختم
        برائے (; x < چوڑائی; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    برائے (صحیح y = urows; y < اونچائی; y++)
        حافظہ_نقل(dst + (size_t)y*dsty, dst, (size_t)چوڑائی*حجم(T));
    واپس 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
سانچہ<قسم_نام T, قسم_نام WT, جماعت Op>
ساکن صحیح scalarUnaryKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                             ثابت خالی*, size_t, size_t, ثابت خالی*, size_t, size_t,
                             خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                             ثابت دوہرا*, صحیح, خالی*)
{
    s0y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x <= 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    T* dst = (T*)dst_;
    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && s0y == s0x*(size_t)چوڑائی) { چوڑائی *= اونچائی; اونچائی = 1; }
    ثابت صحیح urows = (s0y == 0 && اونچائی > 1) ? 1 : اونچائی;   // vertical broadcast: 1 row + copies
    برائے (صحیح y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        اگر (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            برائے (صحیح x = 0; x < چوڑائی; x++) dst[x] = v;
            جاری;
        }
        برائے (صحیح x = 0; x < چوڑائی; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    برائے (صحیح y = urows; y < اونچائی; y++)
        حافظہ_نقل(dst + (size_t)y*dsty, dst, (size_t)چوڑائی*حجم(T));
    واپس 0;
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
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
ساکن اندرونی v_float32 vxSetallW(اعشاری v,  ثابت v_float32&) { واپس vx_setall_f32(v); }
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ساکن اندرونی v_float64 vxSetallW(دوہرا v, ثابت v_float64&) { واپس vx_setall_f64(v); }
#اگر_ختم
#اگر_ختم

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
سانچہ<قسم_نام T, قسم_نام WT>
ساکن صحیح scalarPowKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                           ثابت خالی* src1_, size_t s1y, size_t s1x,
                           ثابت خالی*, size_t, size_t,
                           خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                           ثابت دوہرا*, صحیح, خالی*)
{
    s0y /= حجم(T); s1y /= حجم(T); dsty /= حجم(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    T* dst = (T*)dst_;
    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && s0y == s0x*(size_t)چوڑائی && s1y == s1x*(size_t)چوڑائی)
    { چوڑائی *= اونچائی; اونچائی = 1; }
    برائے (صحیح y = 0; y < اونچائی; y++, src0 += s0y, src1 += s1y, dst += dsty)
        برائے (صحیح x = 0; x < چوڑائی; x++)
            dst[x] = saturate_cast<T>(std::قوت((WT)src0[x*s0x], (WT)src1[x*s1x]));
    واپس 0;
}

سانچہ<قسم_نام T, قسم_نام Wvec>
ساکن صحیح powKernel(ثابت خالی* src0_, size_t s0y, size_t s0x,
                     ثابت خالی* src1_, size_t s1y, size_t s1x,
                     ثابت خالی*, size_t, size_t,
                     خالی* dst_, size_t dsty, صحیح چوڑائی, صحیح اونچائی,
                     ثابت دوہرا*, صحیح, خالی*)
{
    s0y /= حجم(T);
    s1y /= حجم(T);
    dsty /= حجم(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    T* dst = (T*)dst_;
    استعمال WT = قسم_نام VTraits<Wvec>::lane_type;

    اگر (اونچائی > 1 && dsty == (size_t)چوڑائی && s0y == s0x*(size_t)چوڑائی && s1y == s1x*(size_t)چوڑائی)
    { چوڑائی *= اونچائی; اونچائی = 1; }

    [[maybe_unused]] ثابت منطقی دم_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ثابت صحیح urows = (s0y == 0 && s1y == 0 && اونچائی > 1) ? 1 : اونچائی;

    برائے (صحیح y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        صحیح x = 0;
        اگر (s1x == 0)                                   // scalar exponent for this row
        {
            ثابت WT p = (WT)src1[0];
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
            ثابت صحیح VECSZ = VTraits<Wvec>::vlanes();
            اگر (s0x == 1)
            {
                اگر (p == WT(2) || p == WT(3))
                {
                    برائے (; x < چوڑائی; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > چوڑائی) { اگر (!دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ*2; }
                        Wvec a0, a1;
                        vx_بھرو_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        اگر (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                ورنہ اگر (p == WT(0.5))
                {
                    برائے (; x < چوڑائی; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > چوڑائی) { اگر (!دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ*2; }
                        Wvec a0, a1;
                        vx_بھرو_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ورنہ اگر (p == WT(1))
                {
                    اگر ((ثابت خالی*)src0 != (ثابت خالی*)dst)
                        برائے (; x < چوڑائی; x++) dst[x] = src0[x];
                    x = چوڑائی;
                }
                ورنہ اگر (p == WT(0))
                {
                    ثابت T one = saturate_cast<T>(1);
                    برائے (; x < چوڑائی; x++) dst[x] = one;
                }
                ورنہ اگر (p == WT(-0.5))
                {
                    ثابت Wvec one = vxSetallW(WT(1), Wvec());
                    برائے (; x < چوڑائی; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > چوڑائی) { اگر (!دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ*2; }
                        Wvec a0, a1;
                        vx_بھرو_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ورنہ اگر (p == std::rint(p) && std::مطلق_قدر(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ثابت صحیح ip = (صحیح)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ثابت Wvec one = vxSetallW(WT(1), Wvec());
                    برائے (; x < چوڑائی; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > چوڑائی) { اگر (!دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ*2; }
                        Wvec b0, b1;
                        vx_بھرو_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        برائے (صحیح q = ap; q > 1; q >>= 1)
                        {
                            اگر (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        اگر (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ورنہ                                    // general scalar exponent: exp(p * log(x))
                {
                    ثابت Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    برائے (; x < چوڑائی; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > چوڑائی) { اگر (!دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ*2; }
                        Wvec a0, a1;
                        vx_بھرو_pair_as(src0 + x, a0, a1);
                        اگر (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            برائے (صحیح i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::قوت((WT)src0[x + i], p));
                            جاری;
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
#اگر_ختم
            برائے (; x < چوڑائی; x++)
                dst[x] = saturate_cast<T>(std::قوت((WT)src0[x*s0x], p));
            جاری;
        }
        // per-element exponent
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        اگر (s0x == 1)
        {
            ثابت صحیح VECSZ = VTraits<Wvec>::vlanes();
            ثابت Wvec z = v_setzero_<Wvec>();
            برائے (; x < چوڑائی; x += VECSZ*2)
            {
                اگر (x + VECSZ*2 > چوڑائی) { اگر (!دم_trick || x == 0) توڑو; x = چوڑائی - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_بھرو_pair_as(src0 + x, a0, a1);
                vx_بھرو_pair_as(src1 + x, b0, b1);
                اگر (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    برائے (صحیح i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::قوت((WT)src0[x + i], (WT)src1[x + i]));
                    جاری;
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
#اگر_ختم
        برائے (; x < چوڑائی; x++)
            dst[x] = saturate_cast<T>(std::قوت((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    برائے (صحیح y = urows; y < اونچائی; y++)
        حافظہ_نقل(dst + (size_t)y*dsty, dst, (size_t)چوڑائی*حجم(T));
    واپس 0;
}

TKernel getPowFunc_(صحیح T, صحیح R)
{
    اگر (R != T)
        واپس {};
    KernelFunc fptr = خالی_اشاریہ;
    انتخاب (T)
    {
    حالت CV_16F:  fptr = powKernel<hfloat, v_float32>; توڑو;
    حالت CV_16BF: fptr = powKernel<bfloat, v_float32>; توڑو;
    حالت CV_32F:  fptr = powKernel<اعشاری,  v_float32>; توڑو;
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  fptr = powKernel<دوہرا, v_float64>; توڑو;
#ورنہ
    حالت CV_64F:  fptr = scalarPowKernel<دوہرا, دوہرا>; توڑو;
#اگر_ختم
    طےشدہ: ;
    }
    واپس {fptr, خالی_اشاریہ, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
سانچہ<جماعت Op>
ساکن KernelFunc math_By_گہرائی(صحیح T)
{
    انتخاب (T)
    {
    حالت CV_16F:  واپس vecUnaryKernel<hfloat, v_float32, Op>;
    حالت CV_16BF: واپس vecUnaryKernel<bfloat, v_float32, Op>;
    حالت CV_32F:  واپس vecUnaryKernel<اعشاری,  v_float32, Op>;
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  واپس vecUnaryKernel<دوہرا, v_float64, Op>;
#ورنہ
    حالت CV_64F:  واپس scalarUnaryKernel<دوہرا, دوہرا, Op>;
#اگر_ختم
    طےشدہ:      واپس خالی_اشاریہ;
    }
}

TKernel getMathFunc_(چوٹی op, صحیح T)
{
    KernelFunc f = خالی_اشاریہ;
    انتخاب (op)
    {
    حالت OP_SQRT: f = math_By_گہرائی<MSqrt>(T); توڑو;
    حالت OP_EXP:  f = math_By_گہرائی<MExp >(T); توڑو;
    حالت OP_LOG:  f = math_By_گہرائی<MLog >(T); توڑو;
    حالت OP_SIN:  f = math_By_گہرائی<MSin >(T); توڑو;
    حالت OP_COS:  f = math_By_گہرائی<MCos >(T); توڑو;
    حالت OP_TANH: f = math_By_گہرائی<MTanh>(T); توڑو;
    حالت OP_RELU: f = math_By_گہرائی<MRelu>(T); توڑو;
    حالت OP_ERF:  f = math_By_گہرائی<MErf >(T); توڑو;
    طےشدہ: ;
    }
    واپس {f, خالی_اشاریہ, 0};
}


#اگر_ختم // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_انتہا
}} // namespace cv::ew
