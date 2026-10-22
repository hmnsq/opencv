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

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/hal/intrin.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>

ਨਾਮਾਕਾਸ਼ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
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

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ਨਾਮਾਕਾਸ਼ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(ਸਿਖਰ op, ਪੂਰਨ T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(ਪੂਰਨ T, ਪੂਰਨ R);                   // OP_POW, T x T -> T (R must equal T)

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
ਬਣਤਰ MSqrt {
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& x) { ਵਾਪਸ v_sqrt(x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ W scl(W x) { ਵਾਪਸ std::ਵਰਗਮੂਲ(x); }
};
ਬਣਤਰ MExp {
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& x) { ਵਾਪਸ v_exp(x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ W scl(W x) { ਵਾਪਸ std::ਘਾਤਾਂਕ(x); }
};
ਬਣਤਰ MLog {
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& x) { ਵਾਪਸ v_log(x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ W scl(W x) { ਵਾਪਸ std::ਲਘੂਗਣਕ(x); }
};
ਬਣਤਰ MSin {
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& x) { ਵਾਪਸ v_sin(x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ W scl(W x) { ਵਾਪਸ std::ਜੀਵਾ(x); }
};
ਬਣਤਰ MCos {
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& x) { ਵਾਪਸ v_cos(x); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ W scl(W x) { ਵਾਪਸ std::ਕੋਜੀਵਾ(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
ਬਣਤਰ MTanh {
    ਸਥਿਰ v_float32 vec(ਅਚਲ v_float32& x)
    {
        ਅਚਲ v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_ਜੋੜੋ(cx, cx));
        ਵਾਪਸ v_div(v_sub(e, one), v_ਜੋੜੋ(e, one));
    }
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਸਥਿਰ v_float64 vec(ਅਚਲ v_float64& x)
    {
        ਅਚਲ v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_ਜੋੜੋ(cx, cx));
        ਵਾਪਸ v_div(v_sub(e, one), v_ਜੋੜੋ(e, one));
    }
#ਜੇ_ਅੰਤ
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ W scl(W x) { ਵਾਪਸ std::tanh(x); }
};
ਬਣਤਰ MErf {
    ਸਥਿਰ v_float32 vec(ਅਚਲ v_float32& x) { ਵਾਪਸ v_erf(x); }
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    ਸਥਿਰ v_float64 vec(ਅਚਲ v_float64& x)
    {
        ਦੁੱਗਣਾ buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        ਲਈ (ਪੂਰਨ i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        ਵਾਪਸ vx_ਲੱਦੋ(buf);
    }
#ਜੇ_ਅੰਤ
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ W scl(W x) { ਵਾਪਸ std::erf(x); }
};
ਬਣਤਰ MRelu {
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V> ਸਥਿਰ V vec(ਅਚਲ V& x) { ਵਾਪਸ v_max(x, v_setzero_<V>()); }
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ W> ਸਥਿਰ W scl(W x) { ਵਾਪਸ x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Wvec, ਵਰਗ Op>
ਸਥਿਰ ਪੂਰਨ vecUnaryKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                          ਅਚਲ ਖਾਲੀ*, size_t, size_t, ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                          ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                          ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ*)
{
    s0y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(T);
    CV_Assert(s0x <= 1u);

    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    T* dst = (T*)dst_;
    ਵਰਤੋਂ WT = ਕਿਸਮ_ਨਾਂ VTraits<Wvec>::lane_type;

    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && s0y == s0x*(size_t)ਚੌੜਾਈ) { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ਅਚਲ ਪੂਰਨ urows = (s0y == 0 && ਉਚਾਈ > 1) ? 1 : ਉਚਾਈ;

    ਲਈ (ਪੂਰਨ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ਜੇ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++) dst[x] = v;
            ਜਾਰੀ;
        }
        ਪੂਰਨ x = 0;
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ਅਚਲ ਤਾਰਕਿਕ use_ਪੂਛ_trick = ਚੌੜਾਈ >= VECSZ*4 && src0_ != dst_;
        ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*2)
        {
            ਜੇ (x + VECSZ*2 > ਚੌੜਾਈ) { ਜੇ (!use_ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*2; }
            Wvec a0, a1;
            vx_ਲੱਦੋ_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#ਜੇ_ਅੰਤ
        ਲਈ (; x < ਚੌੜਾਈ; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ਲਈ (ਪੂਰਨ y = urows; y < ਉਚਾਈ; y++)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + (size_t)y*dsty, dst, (size_t)ਚੌੜਾਈ*ਆਕਾਰ(T));
    ਵਾਪਸ 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT, ਵਰਗ Op>
ਸਥਿਰ ਪੂਰਨ scalarUnaryKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                             ਅਚਲ ਖਾਲੀ*, size_t, size_t, ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                             ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                             ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ*)
{
    s0y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(T);
    CV_Assert(s0x <= 1u);
    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    T* dst = (T*)dst_;
    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && s0y == s0x*(size_t)ਚੌੜਾਈ) { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }
    ਅਚਲ ਪੂਰਨ urows = (s0y == 0 && ਉਚਾਈ > 1) ? 1 : ਉਚਾਈ;   // vertical broadcast: 1 row + copies
    ਲਈ (ਪੂਰਨ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ਜੇ (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++) dst[x] = v;
            ਜਾਰੀ;
        }
        ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ਲਈ (ਪੂਰਨ y = urows; y < ਉਚਾਈ; y++)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + (size_t)y*dsty, dst, (size_t)ਚੌੜਾਈ*ਆਕਾਰ(T));
    ਵਾਪਸ 0;
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
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
ਸਥਿਰ ਸਤਰਅੰਦਰ v_float32 vxSetallW(ਦਸ਼ਮਲਵ v,  ਅਚਲ v_float32&) { ਵਾਪਸ vx_setall_f32(v); }
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ਸਥਿਰ ਸਤਰਅੰਦਰ v_float64 vxSetallW(ਦੁੱਗਣਾ v, ਅਚਲ v_float64&) { ਵਾਪਸ vx_setall_f64(v); }
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT>
ਸਥਿਰ ਪੂਰਨ scalarPowKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                           ਅਚਲ ਖਾਲੀ* src1_, size_t s1y, size_t s1x,
                           ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                           ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                           ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ*)
{
    s0y /= ਆਕਾਰ(T); s1y /= ਆਕਾਰ(T); dsty /= ਆਕਾਰ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* src1 = (ਅਚਲ T*)src1_;
    T* dst = (T*)dst_;
    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && s0y == s0x*(size_t)ਚੌੜਾਈ && s1y == s1x*(size_t)ਚੌੜਾਈ)
    { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }
    ਲਈ (ਪੂਰਨ y = 0; y < ਉਚਾਈ; y++, src0 += s0y, src1 += s1y, dst += dsty)
        ਲਈ (ਪੂਰਨ x = 0; x < ਚੌੜਾਈ; x++)
            dst[x] = saturate_cast<T>(std::ਘਾਤ((WT)src0[x*s0x], (WT)src1[x*s1x]));
    ਵਾਪਸ 0;
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Wvec>
ਸਥਿਰ ਪੂਰਨ powKernel(ਅਚਲ ਖਾਲੀ* src0_, size_t s0y, size_t s0x,
                     ਅਚਲ ਖਾਲੀ* src1_, size_t s1y, size_t s1x,
                     ਅਚਲ ਖਾਲੀ*, size_t, size_t,
                     ਖਾਲੀ* dst_, size_t dsty, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ,
                     ਅਚਲ ਦੁੱਗਣਾ*, ਪੂਰਨ, ਖਾਲੀ*)
{
    s0y /= ਆਕਾਰ(T);
    s1y /= ਆਕਾਰ(T);
    dsty /= ਆਕਾਰ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ਅਚਲ T* src0 = (ਅਚਲ T*)src0_;
    ਅਚਲ T* src1 = (ਅਚਲ T*)src1_;
    T* dst = (T*)dst_;
    ਵਰਤੋਂ WT = ਕਿਸਮ_ਨਾਂ VTraits<Wvec>::lane_type;

    ਜੇ (ਉਚਾਈ > 1 && dsty == (size_t)ਚੌੜਾਈ && s0y == s0x*(size_t)ਚੌੜਾਈ && s1y == s1x*(size_t)ਚੌੜਾਈ)
    { ਚੌੜਾਈ *= ਉਚਾਈ; ਉਚਾਈ = 1; }

    [[maybe_unused]] ਅਚਲ ਤਾਰਕਿਕ ਪੂਛ_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ਅਚਲ ਪੂਰਨ urows = (s0y == 0 && s1y == 0 && ਉਚਾਈ > 1) ? 1 : ਉਚਾਈ;

    ਲਈ (ਪੂਰਨ y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ਪੂਰਨ x = 0;
        ਜੇ (s1x == 0)                                   // scalar exponent for this row
        {
            ਅਚਲ WT p = (WT)src1[0];
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
            ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Wvec>::vlanes();
            ਜੇ (s0x == 1)
            {
                ਜੇ (p == WT(2) || p == WT(3))
                {
                    ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*2)
                    {
                        ਜੇ (x + VECSZ*2 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ਲੱਦੋ_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        ਜੇ (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ (p == WT(0.5))
                {
                    ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*2)
                    {
                        ਜੇ (x + VECSZ*2 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ਲੱਦੋ_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ (p == WT(1))
                {
                    ਜੇ ((ਅਚਲ ਖਾਲੀ*)src0 != (ਅਚਲ ਖਾਲੀ*)dst)
                        ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = src0[x];
                    x = ਚੌੜਾਈ;
                }
                ਨਹੀਂਤਾਂ ਜੇ (p == WT(0))
                {
                    ਅਚਲ T one = saturate_cast<T>(1);
                    ਲਈ (; x < ਚੌੜਾਈ; x++) dst[x] = one;
                }
                ਨਹੀਂਤਾਂ ਜੇ (p == WT(-0.5))
                {
                    ਅਚਲ Wvec one = vxSetallW(WT(1), Wvec());
                    ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*2)
                    {
                        ਜੇ (x + VECSZ*2 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ਲੱਦੋ_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ (p == std::rint(p) && std::ਨਿਰਪੇਖ(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ਅਚਲ ਪੂਰਨ ip = (ਪੂਰਨ)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ਅਚਲ Wvec one = vxSetallW(WT(1), Wvec());
                    ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*2)
                    {
                        ਜੇ (x + VECSZ*2 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*2; }
                        Wvec b0, b1;
                        vx_ਲੱਦੋ_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        ਲਈ (ਪੂਰਨ q = ap; q > 1; q >>= 1)
                        {
                            ਜੇ (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        ਜੇ (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ਨਹੀਂਤਾਂ                                    // general scalar exponent: exp(p * log(x))
                {
                    ਅਚਲ Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*2)
                    {
                        ਜੇ (x + VECSZ*2 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ਲੱਦੋ_pair_as(src0 + x, a0, a1);
                        ਜੇ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            ਲਈ (ਪੂਰਨ i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::ਘਾਤ((WT)src0[x + i], p));
                            ਜਾਰੀ;
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
#ਜੇ_ਅੰਤ
            ਲਈ (; x < ਚੌੜਾਈ; x++)
                dst[x] = saturate_cast<T>(std::ਘਾਤ((WT)src0[x*s0x], p));
            ਜਾਰੀ;
        }
        // per-element exponent
#ਜੇ (CV_SIMD || CV_SIMD_SCALABLE)
        ਜੇ (s0x == 1)
        {
            ਅਚਲ ਪੂਰਨ VECSZ = VTraits<Wvec>::vlanes();
            ਅਚਲ Wvec z = v_setzero_<Wvec>();
            ਲਈ (; x < ਚੌੜਾਈ; x += VECSZ*2)
            {
                ਜੇ (x + VECSZ*2 > ਚੌੜਾਈ) { ਜੇ (!ਪੂਛ_trick || x == 0) ਰੋਕੋ; x = ਚੌੜਾਈ - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_ਲੱਦੋ_pair_as(src0 + x, a0, a1);
                vx_ਲੱਦੋ_pair_as(src1 + x, b0, b1);
                ਜੇ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    ਲਈ (ਪੂਰਨ i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::ਘਾਤ((WT)src0[x + i], (WT)src1[x + i]));
                    ਜਾਰੀ;
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
#ਜੇ_ਅੰਤ
        ਲਈ (; x < ਚੌੜਾਈ; x++)
            dst[x] = saturate_cast<T>(std::ਘਾਤ((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    ਲਈ (ਪੂਰਨ y = urows; y < ਉਚਾਈ; y++)
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(dst + (size_t)y*dsty, dst, (size_t)ਚੌੜਾਈ*ਆਕਾਰ(T));
    ਵਾਪਸ 0;
}

TKernel getPowFunc_(ਪੂਰਨ T, ਪੂਰਨ R)
{
    ਜੇ (R != T)
        ਵਾਪਸ {};
    KernelFunc fptr = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_16F:  fptr = powKernel<hfloat, v_float32>; ਰੋਕੋ;
    ਹਾਲਤ CV_16BF: fptr = powKernel<bfloat, v_float32>; ਰੋਕੋ;
    ਹਾਲਤ CV_32F:  fptr = powKernel<ਦਸ਼ਮਲਵ,  v_float32>; ਰੋਕੋ;
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਹਾਲਤ CV_64F:  fptr = powKernel<ਦੁੱਗਣਾ, v_float64>; ਰੋਕੋ;
#ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_64F:  fptr = scalarPowKernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ>; ਰੋਕੋ;
#ਜੇ_ਅੰਤ
    ਮੂਲ: ;
    }
    ਵਾਪਸ {fptr, ਖਾਲੀ_ਸੂਚਕ, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
ਸਾਂਚਾ<ਵਰਗ Op>
ਸਥਿਰ KernelFunc math_By_ਡੂੰਘਾਈ(ਪੂਰਨ T)
{
    ਚੋਣ (T)
    {
    ਹਾਲਤ CV_16F:  ਵਾਪਸ vecUnaryKernel<hfloat, v_float32, Op>;
    ਹਾਲਤ CV_16BF: ਵਾਪਸ vecUnaryKernel<bfloat, v_float32, Op>;
    ਹਾਲਤ CV_32F:  ਵਾਪਸ vecUnaryKernel<ਦਸ਼ਮਲਵ,  v_float32, Op>;
#ਜੇ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ਹਾਲਤ CV_64F:  ਵਾਪਸ vecUnaryKernel<ਦੁੱਗਣਾ, v_float64, Op>;
#ਨਹੀਂਤਾਂ
    ਹਾਲਤ CV_64F:  ਵਾਪਸ scalarUnaryKernel<ਦੁੱਗਣਾ, ਦੁੱਗਣਾ, Op>;
#ਜੇ_ਅੰਤ
    ਮੂਲ:      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
}

TKernel getMathFunc_(ਸਿਖਰ op, ਪੂਰਨ T)
{
    KernelFunc f = ਖਾਲੀ_ਸੂਚਕ;
    ਚੋਣ (op)
    {
    ਹਾਲਤ OP_SQRT: f = math_By_ਡੂੰਘਾਈ<MSqrt>(T); ਰੋਕੋ;
    ਹਾਲਤ OP_EXP:  f = math_By_ਡੂੰਘਾਈ<MExp >(T); ਰੋਕੋ;
    ਹਾਲਤ OP_LOG:  f = math_By_ਡੂੰਘਾਈ<MLog >(T); ਰੋਕੋ;
    ਹਾਲਤ OP_SIN:  f = math_By_ਡੂੰਘਾਈ<MSin >(T); ਰੋਕੋ;
    ਹਾਲਤ OP_COS:  f = math_By_ਡੂੰਘਾਈ<MCos >(T); ਰੋਕੋ;
    ਹਾਲਤ OP_TANH: f = math_By_ਡੂੰਘਾਈ<MTanh>(T); ਰੋਕੋ;
    ਹਾਲਤ OP_RELU: f = math_By_ਡੂੰਘਾਈ<MRelu>(T); ਰੋਕੋ;
    ਹਾਲਤ OP_ERF:  f = math_By_ਡੂੰਘਾਈ<MErf >(T); ਰੋਕੋ;
    ਮੂਲ: ;
    }
    ਵਾਪਸ {f, ਖਾਲੀ_ਸੂਚਕ, 0};
}


#ਜੇ_ਅੰਤ // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_ਸਿਰਾ
}} // namespace cv::ew
