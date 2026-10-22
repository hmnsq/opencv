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

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"
#ಅನ್ತರ್ಭಾವ "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#ಅನ್ತರ್ಭಾವ <cmath>

ನಾಮಾಕಾಶ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
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

#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ನಾಮಾಕಾಶ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(ತುದಿ op, ಪೂರ್ಣಾಂಕ T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R);                   // OP_POW, T x T -> T (R must equal T)

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
ರಚನೆ MSqrt {
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_sqrt(x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ W scl(W x) { ಹಿಂತಿರುಗಿ std::ವರ್ಗಮೂಲ(x); }
};
ರಚನೆ MExp {
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_exp(x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ W scl(W x) { ಹಿಂತಿರುಗಿ std::ಘಾತಾಂಕ(x); }
};
ರಚನೆ MLog {
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_log(x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ W scl(W x) { ಹಿಂತಿರುಗಿ std::ಲಘುಗಣಕ(x); }
};
ರಚನೆ MSin {
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_sin(x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ W scl(W x) { ಹಿಂತಿರುಗಿ std::ಜ್ಯಾ(x); }
};
ರಚನೆ MCos {
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_cos(x); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ W scl(W x) { ಹಿಂತಿರುಗಿ std::ಕೋಜ್ಯಾ(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
ರಚನೆ MTanh {
    ಸ್ಥಿರ v_float32 vec(ಸ್ಥಿರಾಂಕ v_float32& x)
    {
        ಸ್ಥಿರಾಂಕ v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_ಕೂಡಿಸು(cx, cx));
        ಹಿಂತಿರುಗಿ v_div(v_sub(e, one), v_ಕೂಡಿಸು(e, one));
    }
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿರ v_float64 vec(ಸ್ಥಿರಾಂಕ v_float64& x)
    {
        ಸ್ಥಿರಾಂಕ v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_ಕೂಡಿಸು(cx, cx));
        ಹಿಂತಿರುಗಿ v_div(v_sub(e, one), v_ಕೂಡಿಸು(e, one));
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ W scl(W x) { ಹಿಂತಿರುಗಿ std::tanh(x); }
};
ರಚನೆ MErf {
    ಸ್ಥಿರ v_float32 vec(ಸ್ಥಿರಾಂಕ v_float32& x) { ಹಿಂತಿರುಗಿ v_erf(x); }
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    ಸ್ಥಿರ v_float64 vec(ಸ್ಥಿರಾಂಕ v_float64& x)
    {
        ದ್ವಿಗುಣ buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        ಹಿಂತಿರುಗಿ vx_ತುಂಬು(buf);
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ W scl(W x) { ಹಿಂತಿರುಗಿ std::erf(x); }
};
ರಚನೆ MRelu {
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು V> ಸ್ಥಿರ V vec(ಸ್ಥಿರಾಂಕ V& x) { ಹಿಂತಿರುಗಿ v_max(x, v_setzero_<V>()); }
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು W> ಸ್ಥಿರ W scl(W x) { ಹಿಂತಿರುಗಿ x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Wvec, ವರ್ಗ Op>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ vecUnaryKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                          ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                          ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                          ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    s0y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(T);
    CV_Assert(s0x <= 1u);

    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    T* dst = (T*)dst_;
    ಬಳಸಿ WT = ಬಗೆ_ಹೆಸರು VTraits<Wvec>::lane_type;

    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && s0y == s0x*(size_t)ಅಗಲ) { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ urows = (s0y == 0 && ಎತ್ತರ > 1) ? 1 : ಎತ್ತರ;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ಆದರೆ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++) dst[x] = v;
            ಮುಂದುವರಿ;
        }
        ಪೂರ್ಣಾಂಕ x = 0;
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ use_ಬಾಲ_trick = ಅಗಲ >= VECSZ*4 && src0_ != dst_;
        ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*2)
        {
            ಆದರೆ (x + VECSZ*2 > ಅಗಲ) { ಆದರೆ (!use_ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*2; }
            Wvec a0, a1;
            vx_ತುಂಬು_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; x < ಅಗಲ; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = urows; y < ಎತ್ತರ; y++)
        ಸ್ಮರಣೆ_ನಕಲು(dst + (size_t)y*dsty, dst, (size_t)ಅಗಲ*ಗಾತ್ರ(T));
    ಹಿಂತಿರುಗಿ 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT, ವರ್ಗ Op>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scalarUnaryKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                             ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                             ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    s0y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(T);
    CV_Assert(s0x <= 1u);
    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    T* dst = (T*)dst_;
    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && s0y == s0x*(size_t)ಅಗಲ) { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ urows = (s0y == 0 && ಎತ್ತರ > 1) ? 1 : ಎತ್ತರ;   // vertical broadcast: 1 row + copies
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ಆದರೆ (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++) dst[x] = v;
            ಮುಂದುವರಿ;
        }
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = urows; y < ಎತ್ತರ; y++)
        ಸ್ಮರಣೆ_ನಕಲು(dst + (size_t)y*dsty, dst, (size_t)ಅಗಲ*ಗಾತ್ರ(T));
    ಹಿಂತಿರುಗಿ 0;
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
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_float32 vxSetallW(ದಶಮಾಂಶ v,  ಸ್ಥಿರಾಂಕ v_float32&) { ಹಿಂತಿರುಗಿ vx_setall_f32(v); }
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ಸ್ಥಿರ ಸಾಲೊಳಗೆ v_float64 vxSetallW(ದ್ವಿಗುಣ v, ಸ್ಥಿರಾಂಕ v_float64&) { ಹಿಂತಿರುಗಿ vx_setall_f64(v); }
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು WT>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scalarPowKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                           ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                           ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                           ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                           ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    s0y /= ಗಾತ್ರ(T); s1y /= ಗಾತ್ರ(T); dsty /= ಗಾತ್ರ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)src1_;
    T* dst = (T*)dst_;
    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && s0y == s0x*(size_t)ಅಗಲ && s1y == s1x*(size_t)ಅಗಲ)
    { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < ಎತ್ತರ; y++, src0 += s0y, src1 += s1y, dst += dsty)
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < ಅಗಲ; x++)
            dst[x] = saturate_cast<T>(std::ಘಾತ((WT)src0[x*s0x], (WT)src1[x*s1x]));
    ಹಿಂತಿರುಗಿ 0;
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಬಗೆ_ಹೆಸರು Wvec>
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ powKernel(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, size_t, size_t,
                     ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                     ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ*)
{
    s0y /= ಗಾತ್ರ(T);
    s1y /= ಗಾತ್ರ(T);
    dsty /= ಗಾತ್ರ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ಸ್ಥಿರಾಂಕ T* src0 = (ಸ್ಥಿರಾಂಕ T*)src0_;
    ಸ್ಥಿರಾಂಕ T* src1 = (ಸ್ಥಿರಾಂಕ T*)src1_;
    T* dst = (T*)dst_;
    ಬಳಸಿ WT = ಬಗೆ_ಹೆಸರು VTraits<Wvec>::lane_type;

    ಆದರೆ (ಎತ್ತರ > 1 && dsty == (size_t)ಅಗಲ && s0y == s0x*(size_t)ಅಗಲ && s1y == s1x*(size_t)ಅಗಲ)
    { ಅಗಲ *= ಎತ್ತರ; ಎತ್ತರ = 1; }

    [[maybe_unused]] ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ ಬಾಲ_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ urows = (s0y == 0 && s1y == 0 && ಎತ್ತರ > 1) ? 1 : ಎತ್ತರ;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ಪೂರ್ಣಾಂಕ x = 0;
        ಆದರೆ (s1x == 0)                                   // scalar exponent for this row
        {
            ಸ್ಥಿರಾಂಕ WT p = (WT)src1[0];
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Wvec>::vlanes();
            ಆದರೆ (s0x == 1)
            {
                ಆದರೆ (p == WT(2) || p == WT(3))
                {
                    ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*2)
                    {
                        ಆದರೆ (x + VECSZ*2 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ತುಂಬು_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        ಆದರೆ (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p == WT(0.5))
                {
                    ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*2)
                    {
                        ಆದರೆ (x + VECSZ*2 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ತುಂಬು_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p == WT(1))
                {
                    ಆದರೆ ((ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*)src0 != (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*)dst)
                        ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = src0[x];
                    x = ಅಗಲ;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p == WT(0))
                {
                    ಸ್ಥಿರಾಂಕ T one = saturate_cast<T>(1);
                    ಗಾಗಿ (; x < ಅಗಲ; x++) dst[x] = one;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p == WT(-0.5))
                {
                    ಸ್ಥಿರಾಂಕ Wvec one = vxSetallW(WT(1), Wvec());
                    ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*2)
                    {
                        ಆದರೆ (x + VECSZ*2 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ತುಂಬು_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p == std::rint(p) && std::ನಿರಪೇಕ್ಷ(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ip = (ಪೂರ್ಣಾಂಕ)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ಸ್ಥಿರಾಂಕ Wvec one = vxSetallW(WT(1), Wvec());
                    ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*2)
                    {
                        ಆದರೆ (x + VECSZ*2 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*2; }
                        Wvec b0, b1;
                        vx_ತುಂಬು_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ q = ap; q > 1; q >>= 1)
                        {
                            ಆದರೆ (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        ಆದರೆ (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ                                    // general scalar exponent: exp(p * log(x))
                {
                    ಸ್ಥಿರಾಂಕ Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*2)
                    {
                        ಆದರೆ (x + VECSZ*2 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ತುಂಬು_pair_as(src0 + x, a0, a1);
                        ಆದರೆ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::ಘಾತ((WT)src0[x + i], p));
                            ಮುಂದುವರಿ;
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
#ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ (; x < ಅಗಲ; x++)
                dst[x] = saturate_cast<T>(std::ಘಾತ((WT)src0[x*s0x], p));
            ಮುಂದುವರಿ;
        }
        // per-element exponent
#ಆದರೆ (CV_SIMD || CV_SIMD_SCALABLE)
        ಆದರೆ (s0x == 1)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ VECSZ = VTraits<Wvec>::vlanes();
            ಸ್ಥಿರಾಂಕ Wvec z = v_setzero_<Wvec>();
            ಗಾಗಿ (; x < ಅಗಲ; x += VECSZ*2)
            {
                ಆದರೆ (x + VECSZ*2 > ಅಗಲ) { ಆದರೆ (!ಬಾಲ_trick || x == 0) ನಿಲ್ಲಿಸು; x = ಅಗಲ - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_ತುಂಬು_pair_as(src0 + x, a0, a1);
                vx_ತುಂಬು_pair_as(src1 + x, b0, b1);
                ಆದರೆ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::ಘಾತ((WT)src0[x + i], (WT)src1[x + i]));
                    ಮುಂದುವರಿ;
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
#ಆದರೆ_ಅಂತ್ಯ
        ಗಾಗಿ (; x < ಅಗಲ; x++)
            dst[x] = saturate_cast<T>(std::ಘಾತ((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = urows; y < ಎತ್ತರ; y++)
        ಸ್ಮರಣೆ_ನಕಲು(dst + (size_t)y*dsty, dst, (size_t)ಅಗಲ*ಗಾತ್ರ(T));
    ಹಿಂತಿರುಗಿ 0;
}

TKernel getPowFunc_(ಪೂರ್ಣಾಂಕ T, ಪೂರ್ಣಾಂಕ R)
{
    ಆದರೆ (R != T)
        ಹಿಂತಿರುಗಿ {};
    KernelFunc fptr = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_16F:  fptr = powKernel<hfloat, v_float32>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_16BF: fptr = powKernel<bfloat, v_float32>; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ CV_32F:  fptr = powKernel<ದಶಮಾಂಶ,  v_float32>; ನಿಲ್ಲಿಸು;
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿತಿ CV_64F:  fptr = powKernel<ದ್ವಿಗುಣ, v_float64>; ನಿಲ್ಲಿಸು;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_64F:  fptr = scalarPowKernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>; ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ವನಿಯೋಜಿತ: ;
    }
    ಹಿಂತಿರುಗಿ {fptr, ಶೂನ್ಯಸೂಚಕ, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
ಮಾದರಿ<ವರ್ಗ Op>
ಸ್ಥಿರ KernelFunc math_By_ಆಳ(ಪೂರ್ಣಾಂಕ T)
{
    ಆಯ್ಕೆ (T)
    {
    ಸ್ಥಿತಿ CV_16F:  ಹಿಂತಿರುಗಿ vecUnaryKernel<hfloat, v_float32, Op>;
    ಸ್ಥಿತಿ CV_16BF: ಹಿಂತಿರುಗಿ vecUnaryKernel<bfloat, v_float32, Op>;
    ಸ್ಥಿತಿ CV_32F:  ಹಿಂತಿರುಗಿ vecUnaryKernel<ದಶಮಾಂಶ,  v_float32, Op>;
#ಆದರೆ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿತಿ CV_64F:  ಹಿಂತಿರುಗಿ vecUnaryKernel<ದ್ವಿಗುಣ, v_float64, Op>;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿತಿ CV_64F:  ಹಿಂತಿರುಗಿ scalarUnaryKernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, Op>;
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ವನಿಯೋಜಿತ:      ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
    }
}

TKernel getMathFunc_(ತುದಿ op, ಪೂರ್ಣಾಂಕ T)
{
    KernelFunc f = ಶೂನ್ಯಸೂಚಕ;
    ಆಯ್ಕೆ (op)
    {
    ಸ್ಥಿತಿ OP_SQRT: f = math_By_ಆಳ<MSqrt>(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_EXP:  f = math_By_ಆಳ<MExp >(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_LOG:  f = math_By_ಆಳ<MLog >(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_SIN:  f = math_By_ಆಳ<MSin >(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_COS:  f = math_By_ಆಳ<MCos >(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_TANH: f = math_By_ಆಳ<MTanh>(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_RELU: f = math_By_ಆಳ<MRelu>(T); ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ OP_ERF:  f = math_By_ಆಳ<MErf >(T); ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ: ;
    }
    ಹಿಂತಿರುಗಿ {f, ಶೂನ್ಯಸೂಚಕ, 0};
}


#ಆದರೆ_ಅಂತ್ಯ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ಕೊನೆ
}} // namespace cv::ew
