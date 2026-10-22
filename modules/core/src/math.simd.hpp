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

فضاینام cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#اگر (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
ایستا درونخطی تهی v_store_pair_as(hfloat* p, ثابت v_float32& a, ثابت v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
ایستا درونخطی تهی v_store_pair_as(bfloat* p, ثابت v_float32& a, ثابت v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#endif
#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

فضاینام ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(بالاترین op, صحیح T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(صحیح T, صحیح R);                   // OP_POW, T x T -> T (R must equal T)

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
ساختار MSqrt {
    template<typename V> ایستا V vec(ثابت V& x) { بازگشت v_sqrt(x); }
    template<typename W> ایستا W scl(W x) { بازگشت std::sqrt(x); }
};
ساختار MExp {
    template<typename V> ایستا V vec(ثابت V& x) { بازگشت v_exp(x); }
    template<typename W> ایستا W scl(W x) { بازگشت std::exp(x); }
};
ساختار MLog {
    template<typename V> ایستا V vec(ثابت V& x) { بازگشت v_log(x); }
    template<typename W> ایستا W scl(W x) { بازگشت std::log(x); }
};
ساختار MSin {
    template<typename V> ایستا V vec(ثابت V& x) { بازگشت v_sin(x); }
    template<typename W> ایستا W scl(W x) { بازگشت std::sin(x); }
};
ساختار MCos {
    template<typename V> ایستا V vec(ثابت V& x) { بازگشت v_cos(x); }
    template<typename W> ایستا W scl(W x) { بازگشت std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
ساختار MTanh {
    ایستا v_float32 vec(ثابت v_float32& x)
    {
        ثابت v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        بازگشت v_div(v_sub(e, one), v_add(e, one));
    }
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ایستا v_float64 vec(ثابت v_float64& x)
    {
        ثابت v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        بازگشت v_div(v_sub(e, one), v_add(e, one));
    }
#endif
    template<typename W> ایستا W scl(W x) { بازگشت std::tanh(x); }
};
ساختار MErf {
    ایستا v_float32 vec(ثابت v_float32& x) { بازگشت v_erf(x); }
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    ایستا v_float64 vec(ثابت v_float64& x)
    {
        دوگانه buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        برای (صحیح i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        بازگشت vx_load(buf);
    }
#endif
    template<typename W> ایستا W scl(W x) { بازگشت std::erf(x); }
};
ساختار MRelu {
    template<typename V> ایستا V vec(ثابت V& x) { بازگشت v_max(x, v_setzero_<V>()); }
    template<typename W> ایستا W scl(W x) { بازگشت x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
template<typename T, typename Wvec, رده Op>
ایستا صحیح vecUnaryKernel(ثابت تهی* src0_, size_t s0y, size_t s0x,
                          ثابت تهی*, size_t, size_t, ثابت تهی*, size_t, size_t,
                          تهی* dst_, size_t dsty, صحیح width, صحیح height,
                          ثابت دوگانه*, صحیح, تهی*)
{
    s0y /= اندازه(T);
    dsty /= اندازه(T);
    CV_Assert(s0x <= 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    اگر (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ثابت صحیح urows = (s0y == 0 && height > 1) ? 1 : height;

    برای (صحیح y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        اگر (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            برای (صحیح x = 0; x < width; x++) dst[x] = v;
            ادامه;
        }
        صحیح x = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        ثابت صحیح VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ثابت منطقی use_tail_trick = width >= VECSZ*4 && src0_ != dst_;
        برای (; x < width; x += VECSZ*2)
        {
            اگر (x + VECSZ*2 > width) { اگر (!use_tail_trick || x == 0) بشکن; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#endif
        برای (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    برای (صحیح y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*اندازه(T));
    بازگشت 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
template<typename T, typename WT, رده Op>
ایستا صحیح scalarUnaryKernel(ثابت تهی* src0_, size_t s0y, size_t s0x,
                             ثابت تهی*, size_t, size_t, ثابت تهی*, size_t, size_t,
                             تهی* dst_, size_t dsty, صحیح width, صحیح height,
                             ثابت دوگانه*, صحیح, تهی*)
{
    s0y /= اندازه(T);
    dsty /= اندازه(T);
    CV_Assert(s0x <= 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    T* dst = (T*)dst_;
    اگر (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ثابت صحیح urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    برای (صحیح y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        اگر (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            برای (صحیح x = 0; x < width; x++) dst[x] = v;
            ادامه;
        }
        برای (صحیح x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    برای (صحیح y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*اندازه(T));
    بازگشت 0;
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
ایستا درونخطی v_float32 vxSetallW(اعشاری v,  ثابت v_float32&) { بازگشت vx_setall_f32(v); }
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ایستا درونخطی v_float64 vxSetallW(دوگانه v, ثابت v_float64&) { بازگشت vx_setall_f64(v); }
#endif
#endif

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
template<typename T, typename WT>
ایستا صحیح scalarPowKernel(ثابت تهی* src0_, size_t s0y, size_t s0x,
                           ثابت تهی* src1_, size_t s1y, size_t s1x,
                           ثابت تهی*, size_t, size_t,
                           تهی* dst_, size_t dsty, صحیح width, صحیح height,
                           ثابت دوگانه*, صحیح, تهی*)
{
    s0y /= اندازه(T); s1y /= اندازه(T); dsty /= اندازه(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    T* dst = (T*)dst_;
    اگر (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }
    برای (صحیح y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        برای (صحیح x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x*s1x]));
    بازگشت 0;
}

template<typename T, typename Wvec>
ایستا صحیح powKernel(ثابت تهی* src0_, size_t s0y, size_t s0x,
                     ثابت تهی* src1_, size_t s1y, size_t s1x,
                     ثابت تهی*, size_t, size_t,
                     تهی* dst_, size_t dsty, صحیح width, صحیح height,
                     ثابت دوگانه*, صحیح, تهی*)
{
    s0y /= اندازه(T);
    s1y /= اندازه(T);
    dsty /= اندازه(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ثابت T* src0 = (ثابت T*)src0_;
    ثابت T* src1 = (ثابت T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    اگر (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }

    [[maybe_unused]] ثابت منطقی tail_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ثابت صحیح urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    برای (صحیح y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
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
                    برای (; x < width; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > width) { اگر (!tail_trick || x == 0) بشکن; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        اگر (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                وگرنه اگر (p == WT(0.5))
                {
                    برای (; x < width; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > width) { اگر (!tail_trick || x == 0) بشکن; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                وگرنه اگر (p == WT(1))
                {
                    اگر ((ثابت تهی*)src0 != (ثابت تهی*)dst)
                        برای (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                وگرنه اگر (p == WT(0))
                {
                    ثابت T one = saturate_cast<T>(1);
                    برای (; x < width; x++) dst[x] = one;
                }
                وگرنه اگر (p == WT(-0.5))
                {
                    ثابت Wvec one = vxSetallW(WT(1), Wvec());
                    برای (; x < width; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > width) { اگر (!tail_trick || x == 0) بشکن; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                وگرنه اگر (p == std::rint(p) && std::abs(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ثابت صحیح ip = (صحیح)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ثابت Wvec one = vxSetallW(WT(1), Wvec());
                    برای (; x < width; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > width) { اگر (!tail_trick || x == 0) بشکن; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        برای (صحیح q = ap; q > 1; q >>= 1)
                        {
                            اگر (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        اگر (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                وگرنه                                    // general scalar exponent: exp(p * log(x))
                {
                    ثابت Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    برای (; x < width; x += VECSZ*2)
                    {
                        اگر (x + VECSZ*2 > width) { اگر (!tail_trick || x == 0) بشکن; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        اگر (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            برای (صحیح i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], p));
                            ادامه;
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
#endif
            برای (; x < width; x++)
                dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], p));
            ادامه;
        }
        // per-element exponent
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
        اگر (s0x == 1)
        {
            ثابت صحیح VECSZ = VTraits<Wvec>::vlanes();
            ثابت Wvec z = v_setzero_<Wvec>();
            برای (; x < width; x += VECSZ*2)
            {
                اگر (x + VECSZ*2 > width) { اگر (!tail_trick || x == 0) بشکن; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                اگر (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    برای (صحیح i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], (WT)src1[x + i]));
                    ادامه;
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
#endif
        برای (; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    برای (صحیح y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*اندازه(T));
    بازگشت 0;
}

TKernel getPowFunc_(صحیح T, صحیح R)
{
    اگر (R != T)
        بازگشت {};
    KernelFunc fptr = nullptr;
    انتخاب (T)
    {
    حالت CV_16F:  fptr = powKernel<hfloat, v_float32>; بشکن;
    حالت CV_16BF: fptr = powKernel<bfloat, v_float32>; بشکن;
    حالت CV_32F:  fptr = powKernel<اعشاری,  v_float32>; بشکن;
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  fptr = powKernel<دوگانه, v_float64>; بشکن;
#وگرنه
    حالت CV_64F:  fptr = scalarPowKernel<دوگانه, دوگانه>; بشکن;
#endif
    پیشفرض: ;
    }
    بازگشت {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
template<رده Op>
ایستا KernelFunc mathByDepth(صحیح T)
{
    انتخاب (T)
    {
    حالت CV_16F:  بازگشت vecUnaryKernel<hfloat, v_float32, Op>;
    حالت CV_16BF: بازگشت vecUnaryKernel<bfloat, v_float32, Op>;
    حالت CV_32F:  بازگشت vecUnaryKernel<اعشاری,  v_float32, Op>;
#اگر CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    حالت CV_64F:  بازگشت vecUnaryKernel<دوگانه, v_float64, Op>;
#وگرنه
    حالت CV_64F:  بازگشت scalarUnaryKernel<دوگانه, دوگانه, Op>;
#endif
    پیشفرض:      بازگشت nullptr;
    }
}

TKernel getMathFunc_(بالاترین op, صحیح T)
{
    KernelFunc f = nullptr;
    انتخاب (op)
    {
    حالت OP_SQRT: f = mathByDepth<MSqrt>(T); بشکن;
    حالت OP_EXP:  f = mathByDepth<MExp >(T); بشکن;
    حالت OP_LOG:  f = mathByDepth<MLog >(T); بشکن;
    حالت OP_SIN:  f = mathByDepth<MSin >(T); بشکن;
    حالت OP_COS:  f = mathByDepth<MCos >(T); بشکن;
    حالت OP_TANH: f = mathByDepth<MTanh>(T); بشکن;
    حالت OP_RELU: f = mathByDepth<MRelu>(T); بشکن;
    حالت OP_ERF:  f = mathByDepth<MErf >(T); بشکن;
    پیشفرض: ;
    }
    بازگشت {f, nullptr, 0};
}


#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
