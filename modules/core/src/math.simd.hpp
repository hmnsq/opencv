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
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ v_store_pair_as(hfloat* p, ಧ್ರುವ v_float32& a, ಧ್ರುವ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ v_store_pair_as(bfloat* p, ಧ್ರುವ v_float32& a, ಧ್ರುವ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#endif
#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ನಾಮಾಕಾಶ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R);                   // OP_POW, T x T -> T (R must equal T)

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
ಸಂರಚನಾ MSqrt {
    template<typename V> ಸ್ಥಿರ V vec(ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_sqrt(x); }
    template<typename W> ಸ್ಥಿರ W scl(W x) { ಪ್ರತ್ಯಾಗಮ std::sqrt(x); }
};
ಸಂರಚನಾ MExp {
    template<typename V> ಸ್ಥಿರ V vec(ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_exp(x); }
    template<typename W> ಸ್ಥಿರ W scl(W x) { ಪ್ರತ್ಯಾಗಮ std::exp(x); }
};
ಸಂರಚನಾ MLog {
    template<typename V> ಸ್ಥಿರ V vec(ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_log(x); }
    template<typename W> ಸ್ಥಿರ W scl(W x) { ಪ್ರತ್ಯಾಗಮ std::log(x); }
};
ಸಂರಚನಾ MSin {
    template<typename V> ಸ್ಥಿರ V vec(ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_sin(x); }
    template<typename W> ಸ್ಥಿರ W scl(W x) { ಪ್ರತ್ಯಾಗಮ std::sin(x); }
};
ಸಂರಚನಾ MCos {
    template<typename V> ಸ್ಥಿರ V vec(ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_cos(x); }
    template<typename W> ಸ್ಥಿರ W scl(W x) { ಪ್ರತ್ಯಾಗಮ std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
ಸಂರಚನಾ MTanh {
    ಸ್ಥಿರ v_float32 vec(ಧ್ರುವ v_float32& x)
    {
        ಧ್ರುವ v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        ಪ್ರತ್ಯಾಗಮ v_div(v_sub(e, one), v_add(e, one));
    }
#ಯದಿ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಸ್ಥಿರ v_float64 vec(ಧ್ರುವ v_float64& x)
    {
        ಧ್ರುವ v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        ಪ್ರತ್ಯಾಗಮ v_div(v_sub(e, one), v_add(e, one));
    }
#endif
    template<typename W> ಸ್ಥಿರ W scl(W x) { ಪ್ರತ್ಯಾಗಮ std::tanh(x); }
};
ಸಂರಚನಾ MErf {
    ಸ್ಥಿರ v_float32 vec(ಧ್ರುವ v_float32& x) { ಪ್ರತ್ಯಾಗಮ v_erf(x); }
#ಯದಿ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    ಸ್ಥಿರ v_float64 vec(ಧ್ರುವ v_float64& x)
    {
        ದ್ವಿಗುಣ buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        ಪ್ರತ್ಯಾಗಮ vx_load(buf);
    }
#endif
    template<typename W> ಸ್ಥಿರ W scl(W x) { ಪ್ರತ್ಯಾಗಮ std::erf(x); }
};
ಸಂರಚನಾ MRelu {
    template<typename V> ಸ್ಥಿರ V vec(ಧ್ರುವ V& x) { ಪ್ರತ್ಯಾಗಮ v_max(x, v_setzero_<V>()); }
    template<typename W> ಸ್ಥಿರ W scl(W x) { ಪ್ರತ್ಯಾಗಮ x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
template<typename T, typename Wvec, ವರ್ಗ Op>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ vecUnaryKernel(ಧ್ರುವ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                          ಧ್ರುವ ಶೂನ್ಯ*, size_t, size_t, ಧ್ರುವ ಶೂನ್ಯ*, size_t, size_t,
                          ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height,
                          ಧ್ರುವ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಙ್ಕ, ಶೂನ್ಯ*)
{
    s0y /= ಪರಿಮಾಣ(T);
    dsty /= ಪರಿಮಾಣ(T);
    CV_Assert(s0x <= 1u);

    ಧ್ರುವ T* src0 = (ಧ್ರುವ T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    ಯದಿ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ urows = (s0y == 0 && height > 1) ? 1 : height;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ಯದಿ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < width; x++) dst[x] = v;
            ಅನುವರ್ತ;
        }
        ಪೂರ್ಣಾಙ್ಕ x = 0;
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ಧ್ರುವ ತಾರ್ಕಿಕ use_tail_trick = width >= VECSZ*4 && src0_ != dst_;
        ಪ್ರತಿ (; x < width; x += VECSZ*2)
        {
            ಯದಿ (x + VECSZ*2 > width) { ಯದಿ (!use_tail_trick || x == 0) ಭಙ್ಗ; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#endif
        ಪ್ರತಿ (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*ಪರಿಮಾಣ(T));
    ಪ್ರತ್ಯಾಗಮ 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
template<typename T, typename WT, ವರ್ಗ Op>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ scalarUnaryKernel(ಧ್ರುವ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                             ಧ್ರುವ ಶೂನ್ಯ*, size_t, size_t, ಧ್ರುವ ಶೂನ್ಯ*, size_t, size_t,
                             ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height,
                             ಧ್ರುವ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಙ್ಕ, ಶೂನ್ಯ*)
{
    s0y /= ಪರಿಮಾಣ(T);
    dsty /= ಪರಿಮಾಣ(T);
    CV_Assert(s0x <= 1u);
    ಧ್ರುವ T* src0 = (ಧ್ರುವ T*)src0_;
    T* dst = (T*)dst_;
    ಯದಿ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ಯದಿ (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < width; x++) dst[x] = v;
            ಅನುವರ್ತ;
        }
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*ಪರಿಮಾಣ(T));
    ಪ್ರತ್ಯಾಗಮ 0;
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
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ v_float32 vxSetallW(ಭಿನ್ನ v,  ಧ್ರುವ v_float32&) { ಪ್ರತ್ಯಾಗಮ vx_setall_f32(v); }
#ಯದಿ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ v_float64 vxSetallW(ದ್ವಿಗುಣ v, ಧ್ರುವ v_float64&) { ಪ್ರತ್ಯಾಗಮ vx_setall_f64(v); }
#endif
#endif

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
template<typename T, typename WT>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ scalarPowKernel(ಧ್ರುವ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                           ಧ್ರುವ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                           ಧ್ರುವ ಶೂನ್ಯ*, size_t, size_t,
                           ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height,
                           ಧ್ರುವ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಙ್ಕ, ಶೂನ್ಯ*)
{
    s0y /= ಪರಿಮಾಣ(T); s1y /= ಪರಿಮಾಣ(T); dsty /= ಪರಿಮಾಣ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ಧ್ರುವ T* src0 = (ಧ್ರುವ T*)src0_;
    ಧ್ರುವ T* src1 = (ಧ್ರುವ T*)src1_;
    T* dst = (T*)dst_;
    ಯದಿ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x*s1x]));
    ಪ್ರತ್ಯಾಗಮ 0;
}

template<typename T, typename Wvec>
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ powKernel(ಧ್ರುವ ಶೂನ್ಯ* src0_, size_t s0y, size_t s0x,
                     ಧ್ರುವ ಶೂನ್ಯ* src1_, size_t s1y, size_t s1x,
                     ಧ್ರುವ ಶೂನ್ಯ*, size_t, size_t,
                     ಶೂನ್ಯ* dst_, size_t dsty, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height,
                     ಧ್ರುವ ದ್ವಿಗುಣ*, ಪೂರ್ಣಾಙ್ಕ, ಶೂನ್ಯ*)
{
    s0y /= ಪರಿಮಾಣ(T);
    s1y /= ಪರಿಮಾಣ(T);
    dsty /= ಪರಿಮಾಣ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ಧ್ರುವ T* src0 = (ಧ್ರುವ T*)src0_;
    ಧ್ರುವ T* src1 = (ಧ್ರುವ T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    ಯದಿ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }

    [[maybe_unused]] ಧ್ರುವ ತಾರ್ಕಿಕ tail_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ಪೂರ್ಣಾಙ್ಕ x = 0;
        ಯದಿ (s1x == 0)                                   // scalar exponent for this row
        {
            ಧ್ರುವ WT p = (WT)src1[0];
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<Wvec>::vlanes();
            ಯದಿ (s0x == 1)
            {
                ಯದಿ (p == WT(2) || p == WT(3))
                {
                    ಪ್ರತಿ (; x < width; x += VECSZ*2)
                    {
                        ಯದಿ (x + VECSZ*2 > width) { ಯದಿ (!tail_trick || x == 0) ಭಙ್ಗ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        ಯದಿ (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ (p == WT(0.5))
                {
                    ಪ್ರತಿ (; x < width; x += VECSZ*2)
                    {
                        ಯದಿ (x + VECSZ*2 > width) { ಯದಿ (!tail_trick || x == 0) ಭಙ್ಗ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ (p == WT(1))
                {
                    ಯದಿ ((ಧ್ರುವ ಶೂನ್ಯ*)src0 != (ಧ್ರುವ ಶೂನ್ಯ*)dst)
                        ಪ್ರತಿ (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                ಅನ್ಯಥಾ ಯದಿ (p == WT(0))
                {
                    ಧ್ರುವ T one = saturate_cast<T>(1);
                    ಪ್ರತಿ (; x < width; x++) dst[x] = one;
                }
                ಅನ್ಯಥಾ ಯದಿ (p == WT(-0.5))
                {
                    ಧ್ರುವ Wvec one = vxSetallW(WT(1), Wvec());
                    ಪ್ರತಿ (; x < width; x += VECSZ*2)
                    {
                        ಯದಿ (x + VECSZ*2 > width) { ಯದಿ (!tail_trick || x == 0) ಭಙ್ಗ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ (p == std::rint(p) && std::abs(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ip = (ಪೂರ್ಣಾಙ್ಕ)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ಧ್ರುವ Wvec one = vxSetallW(WT(1), Wvec());
                    ಪ್ರತಿ (; x < width; x += VECSZ*2)
                    {
                        ಯದಿ (x + VECSZ*2 > width) { ಯದಿ (!tail_trick || x == 0) ಭಙ್ಗ; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ q = ap; q > 1; q >>= 1)
                        {
                            ಯದಿ (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        ಯದಿ (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ಅನ್ಯಥಾ                                    // general scalar exponent: exp(p * log(x))
                {
                    ಧ್ರುವ Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    ಪ್ರತಿ (; x < width; x += VECSZ*2)
                    {
                        ಯದಿ (x + VECSZ*2 > width) { ಯದಿ (!tail_trick || x == 0) ಭಙ್ಗ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        ಯದಿ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], p));
                            ಅನುವರ್ತ;
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
            ಪ್ರತಿ (; x < width; x++)
                dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], p));
            ಅನುವರ್ತ;
        }
        // per-element exponent
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
        ಯದಿ (s0x == 1)
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ VECSZ = VTraits<Wvec>::vlanes();
            ಧ್ರುವ Wvec z = v_setzero_<Wvec>();
            ಪ್ರತಿ (; x < width; x += VECSZ*2)
            {
                ಯದಿ (x + VECSZ*2 > width) { ಯದಿ (!tail_trick || x == 0) ಭಙ್ಗ; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                ಯದಿ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], (WT)src1[x + i]));
                    ಅನುವರ್ತ;
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
        ಪ್ರತಿ (; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*ಪರಿಮಾಣ(T));
    ಪ್ರತ್ಯಾಗಮ 0;
}

TKernel getPowFunc_(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)
{
    ಯದಿ (R != T)
        ಪ್ರತ್ಯಾಗಮ {};
    KernelFunc fptr = nullptr;
    ವಿಕಲ್ಪ (T)
    {
    ಪಕ್ಷ CV_16F:  fptr = powKernel<hfloat, v_float32>; ಭಙ್ಗ;
    ಪಕ್ಷ CV_16BF: fptr = powKernel<bfloat, v_float32>; ಭಙ್ಗ;
    ಪಕ್ಷ CV_32F:  fptr = powKernel<ಭಿನ್ನ,  v_float32>; ಭಙ್ಗ;
#ಯದಿ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಪಕ್ಷ CV_64F:  fptr = powKernel<ದ್ವಿಗುಣ, v_float64>; ಭಙ್ಗ;
#ಅನ್ಯಥಾ
    ಪಕ್ಷ CV_64F:  fptr = scalarPowKernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ>; ಭಙ್ಗ;
#endif
    ಸ್ವಭಾವ: ;
    }
    ಪ್ರತ್ಯಾಗಮ {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
template<ವರ್ಗ Op>
ಸ್ಥಿರ KernelFunc mathByDepth(ಪೂರ್ಣಾಙ್ಕ T)
{
    ವಿಕಲ್ಪ (T)
    {
    ಪಕ್ಷ CV_16F:  ಪ್ರತ್ಯಾಗಮ vecUnaryKernel<hfloat, v_float32, Op>;
    ಪಕ್ಷ CV_16BF: ಪ್ರತ್ಯಾಗಮ vecUnaryKernel<bfloat, v_float32, Op>;
    ಪಕ್ಷ CV_32F:  ಪ್ರತ್ಯಾಗಮ vecUnaryKernel<ಭಿನ್ನ,  v_float32, Op>;
#ಯದಿ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ಪಕ್ಷ CV_64F:  ಪ್ರತ್ಯಾಗಮ vecUnaryKernel<ದ್ವಿಗುಣ, v_float64, Op>;
#ಅನ್ಯಥಾ
    ಪಕ್ಷ CV_64F:  ಪ್ರತ್ಯಾಗಮ scalarUnaryKernel<ದ್ವಿಗುಣ, ದ್ವಿಗುಣ, Op>;
#endif
    ಸ್ವಭಾವ:      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
}

TKernel getMathFunc_(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ T)
{
    KernelFunc f = nullptr;
    ವಿಕಲ್ಪ (op)
    {
    ಪಕ್ಷ OP_SQRT: f = mathByDepth<MSqrt>(T); ಭಙ್ಗ;
    ಪಕ್ಷ OP_EXP:  f = mathByDepth<MExp >(T); ಭಙ್ಗ;
    ಪಕ್ಷ OP_LOG:  f = mathByDepth<MLog >(T); ಭಙ್ಗ;
    ಪಕ್ಷ OP_SIN:  f = mathByDepth<MSin >(T); ಭಙ್ಗ;
    ಪಕ್ಷ OP_COS:  f = mathByDepth<MCos >(T); ಭಙ್ಗ;
    ಪಕ್ಷ OP_TANH: f = mathByDepth<MTanh>(T); ಭಙ್ಗ;
    ಪಕ್ಷ OP_RELU: f = mathByDepth<MRelu>(T); ಭಙ್ಗ;
    ಪಕ್ಷ OP_ERF:  f = mathByDepth<MErf >(T); ಭಙ್ಗ;
    ಸ್ವಭಾವ: ;
    }
    ಪ್ರತ್ಯಾಗಮ {f, nullptr, 0};
}


#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
