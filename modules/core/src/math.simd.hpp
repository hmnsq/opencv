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

#include "opencv2/core/hal/intrin.hpp"
#include "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#include "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#include <cmath>

namespace cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
static anta_hstha void v_store_pair_as(hfloat* p, const v_float32& a, const v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
static anta_hstha void v_store_pair_as(bfloat* p, const v_float32& a, const v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#endif
#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

namespace ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(TOp op, ཧྲིལ T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(ཧྲིལ T, ཧྲིལ R);                   // OP_POW, T x T -> T (R must equal T)

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
sa_nracanaa MSqrt {
    template<typename V> static V vec(const V& x) { ཕྱིར v_sqrt(x); }
    template<typename W> static W scl(W x) { ཕྱིར std::sqrt(x); }
};
sa_nracanaa MExp {
    template<typename V> static V vec(const V& x) { ཕྱིར v_exp(x); }
    template<typename W> static W scl(W x) { ཕྱིར std::exp(x); }
};
sa_nracanaa MLog {
    template<typename V> static V vec(const V& x) { ཕྱིར v_log(x); }
    template<typename W> static W scl(W x) { ཕྱིར std::log(x); }
};
sa_nracanaa MSin {
    template<typename V> static V vec(const V& x) { ཕྱིར v_sin(x); }
    template<typename W> static W scl(W x) { ཕྱིར std::sin(x); }
};
sa_nracanaa MCos {
    template<typename V> static V vec(const V& x) { ཕྱིར v_cos(x); }
    template<typename W> static W scl(W x) { ཕྱིར std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
sa_nracanaa MTanh {
    static v_float32 vec(const v_float32& x)
    {
        const v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        ཕྱིར v_div(v_sub(e, one), v_add(e, one));
    }
#གལ་ཏེ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    static v_float64 vec(const v_float64& x)
    {
        const v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        ཕྱིར v_div(v_sub(e, one), v_add(e, one));
    }
#endif
    template<typename W> static W scl(W x) { ཕྱིར std::tanh(x); }
};
sa_nracanaa MErf {
    static v_float32 vec(const v_float32& x) { ཕྱིར v_erf(x); }
#གལ་ཏེ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    static v_float64 vec(const v_float64& x)
    {
        dviguu_na buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        ཆེད་དུ (ཧྲིལ i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        ཕྱིར vx_load(buf);
    }
#endif
    template<typename W> static W scl(W x) { ཕྱིར std::erf(x); }
};
sa_nracanaa MRelu {
    template<typename V> static V vec(const V& x) { ཕྱིར v_max(x, v_setzero_<V>()); }
    template<typename W> static W scl(W x) { ཕྱིར x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
template<typename T, typename Wvec, class Op>
static ཧྲིལ vecUnaryKernel(const void* src0_, size_t s0y, size_t s0x,
                          const void*, size_t, size_t, const void*, size_t, size_t,
                          void* dst_, size_t dsty, ཧྲིལ width, ཧྲིལ height,
                          const dviguu_na*, ཧྲིལ, void*)
{
    s0y /= parimaa_na(T);
    dsty /= parimaa_na(T);
    CV_Assert(s0x <= 1u);

    const T* src0 = (const T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    གལ་ཏེ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    const ཧྲིལ urows = (s0y == 0 && height > 1) ? 1 : height;

    ཆེད་དུ (ཧྲིལ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        གལ་ཏེ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ཆེད་དུ (ཧྲིལ x = 0; x < width; x++) dst[x] = v;
            continue;
        }
        ཧྲིལ x = 0;
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
        const ཧྲིལ VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        const bool use_tail_trick = width >= VECSZ*4 && src0_ != dst_;
        ཆེད་དུ (; x < width; x += VECSZ*2)
        {
            གལ་ཏེ (x + VECSZ*2 > width) { གལ་ཏེ (!use_tail_trick || x == 0) bha_nga; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#endif
        ཆེད་དུ (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ཆེད་དུ (ཧྲིལ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*parimaa_na(T));
    ཕྱིར 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
template<typename T, typename WT, class Op>
static ཧྲིལ scalarUnaryKernel(const void* src0_, size_t s0y, size_t s0x,
                             const void*, size_t, size_t, const void*, size_t, size_t,
                             void* dst_, size_t dsty, ཧྲིལ width, ཧྲིལ height,
                             const dviguu_na*, ཧྲིལ, void*)
{
    s0y /= parimaa_na(T);
    dsty /= parimaa_na(T);
    CV_Assert(s0x <= 1u);
    const T* src0 = (const T*)src0_;
    T* dst = (T*)dst_;
    གལ་ཏེ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    const ཧྲིལ urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    ཆེད་དུ (ཧྲིལ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        གལ་ཏེ (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ཆེད་དུ (ཧྲིལ x = 0; x < width; x++) dst[x] = v;
            continue;
        }
        ཆེད་དུ (ཧྲིལ x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ཆེད་དུ (ཧྲིལ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*parimaa_na(T));
    ཕྱིར 0;
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
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
static anta_hstha v_float32 vxSetallW(float v,  const v_float32&) { ཕྱིར vx_setall_f32(v); }
#གལ་ཏེ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
static anta_hstha v_float64 vxSetallW(dviguu_na v, const v_float64&) { ཕྱིར vx_setall_f64(v); }
#endif
#endif

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
template<typename T, typename WT>
static ཧྲིལ scalarPowKernel(const void* src0_, size_t s0y, size_t s0x,
                           const void* src1_, size_t s1y, size_t s1x,
                           const void*, size_t, size_t,
                           void* dst_, size_t dsty, ཧྲིལ width, ཧྲིལ height,
                           const dviguu_na*, ཧྲིལ, void*)
{
    s0y /= parimaa_na(T); s1y /= parimaa_na(T); dsty /= parimaa_na(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    const T* src0 = (const T*)src0_;
    const T* src1 = (const T*)src1_;
    T* dst = (T*)dst_;
    གལ་ཏེ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }
    ཆེད་དུ (ཧྲིལ y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        ཆེད་དུ (ཧྲིལ x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x*s1x]));
    ཕྱིར 0;
}

template<typename T, typename Wvec>
static ཧྲིལ powKernel(const void* src0_, size_t s0y, size_t s0x,
                     const void* src1_, size_t s1y, size_t s1x,
                     const void*, size_t, size_t,
                     void* dst_, size_t dsty, ཧྲིལ width, ཧྲིལ height,
                     const dviguu_na*, ཧྲིལ, void*)
{
    s0y /= parimaa_na(T);
    s1y /= parimaa_na(T);
    dsty /= parimaa_na(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    const T* src0 = (const T*)src0_;
    const T* src1 = (const T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    གལ་ཏེ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }

    [[maybe_unused]] const bool tail_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    const ཧྲིལ urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    ཆེད་དུ (ཧྲིལ y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ཧྲིལ x = 0;
        གལ་ཏེ (s1x == 0)                                   // scalar exponent for this row
        {
            const WT p = (WT)src1[0];
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
            const ཧྲིལ VECSZ = VTraits<Wvec>::vlanes();
            གལ་ཏེ (s0x == 1)
            {
                གལ་ཏེ (p == WT(2) || p == WT(3))
                {
                    ཆེད་དུ (; x < width; x += VECSZ*2)
                    {
                        གལ་ཏེ (x + VECSZ*2 > width) { གལ་ཏེ (!tail_trick || x == 0) bha_nga; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        གལ་ཏེ (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                དེ་མིན གལ་ཏེ (p == WT(0.5))
                {
                    ཆེད་དུ (; x < width; x += VECSZ*2)
                    {
                        གལ་ཏེ (x + VECSZ*2 > width) { གལ་ཏེ (!tail_trick || x == 0) bha_nga; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                དེ་མིན གལ་ཏེ (p == WT(1))
                {
                    གལ་ཏེ ((const void*)src0 != (const void*)dst)
                        ཆེད་དུ (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                དེ་མིན གལ་ཏེ (p == WT(0))
                {
                    const T one = saturate_cast<T>(1);
                    ཆེད་དུ (; x < width; x++) dst[x] = one;
                }
                དེ་མིན གལ་ཏེ (p == WT(-0.5))
                {
                    const Wvec one = vxSetallW(WT(1), Wvec());
                    ཆེད་དུ (; x < width; x += VECSZ*2)
                    {
                        གལ་ཏེ (x + VECSZ*2 > width) { གལ་ཏེ (!tail_trick || x == 0) bha_nga; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                དེ་མིན གལ་ཏེ (p == std::rint(p) && std::abs(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    const ཧྲིལ ip = (ཧྲིལ)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    const Wvec one = vxSetallW(WT(1), Wvec());
                    ཆེད་དུ (; x < width; x += VECSZ*2)
                    {
                        གལ་ཏེ (x + VECSZ*2 > width) { གལ་ཏེ (!tail_trick || x == 0) bha_nga; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        ཆེད་དུ (ཧྲིལ q = ap; q > 1; q >>= 1)
                        {
                            གལ་ཏེ (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        གལ་ཏེ (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                དེ་མིན                                    // general scalar exponent: exp(p * log(x))
                {
                    const Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    ཆེད་དུ (; x < width; x += VECSZ*2)
                    {
                        གལ་ཏེ (x + VECSZ*2 > width) { གལ་ཏེ (!tail_trick || x == 0) bha_nga; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        གལ་ཏེ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            ཆེད་དུ (ཧྲིལ i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], p));
                            continue;
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
            ཆེད་དུ (; x < width; x++)
                dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], p));
            continue;
        }
        // per-element exponent
#གལ་ཏེ (CV_SIMD || CV_SIMD_SCALABLE)
        གལ་ཏེ (s0x == 1)
        {
            const ཧྲིལ VECSZ = VTraits<Wvec>::vlanes();
            const Wvec z = v_setzero_<Wvec>();
            ཆེད་དུ (; x < width; x += VECSZ*2)
            {
                གལ་ཏེ (x + VECSZ*2 > width) { གལ་ཏེ (!tail_trick || x == 0) bha_nga; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                གལ་ཏེ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    ཆེད་དུ (ཧྲིལ i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], (WT)src1[x + i]));
                    continue;
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
        ཆེད་དུ (; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    ཆེད་དུ (ཧྲིལ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*parimaa_na(T));
    ཕྱིར 0;
}

TKernel getPowFunc_(ཧྲིལ T, ཧྲིལ R)
{
    གལ་ཏེ (R != T)
        ཕྱིར {};
    KernelFunc fptr = nullptr;
    switch (T)
    {
    pak_sa CV_16F:  fptr = powKernel<hfloat, v_float32>; bha_nga;
    pak_sa CV_16BF: fptr = powKernel<bfloat, v_float32>; bha_nga;
    pak_sa CV_32F:  fptr = powKernel<float,  v_float32>; bha_nga;
#གལ་ཏེ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    pak_sa CV_64F:  fptr = powKernel<dviguu_na, v_float64>; bha_nga;
#དེ་མིན
    pak_sa CV_64F:  fptr = scalarPowKernel<dviguu_na, dviguu_na>; bha_nga;
#endif
    default: ;
    }
    ཕྱིར {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
template<class Op>
static KernelFunc mathByDepth(ཧྲིལ T)
{
    switch (T)
    {
    pak_sa CV_16F:  ཕྱིར vecUnaryKernel<hfloat, v_float32, Op>;
    pak_sa CV_16BF: ཕྱིར vecUnaryKernel<bfloat, v_float32, Op>;
    pak_sa CV_32F:  ཕྱིར vecUnaryKernel<float,  v_float32, Op>;
#གལ་ཏེ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    pak_sa CV_64F:  ཕྱིར vecUnaryKernel<dviguu_na, v_float64, Op>;
#དེ་མིན
    pak_sa CV_64F:  ཕྱིར scalarUnaryKernel<dviguu_na, dviguu_na, Op>;
#endif
    default:      ཕྱིར nullptr;
    }
}

TKernel getMathFunc_(TOp op, ཧྲིལ T)
{
    KernelFunc f = nullptr;
    switch (op)
    {
    pak_sa OP_SQRT: f = mathByDepth<MSqrt>(T); bha_nga;
    pak_sa OP_EXP:  f = mathByDepth<MExp >(T); bha_nga;
    pak_sa OP_LOG:  f = mathByDepth<MLog >(T); bha_nga;
    pak_sa OP_SIN:  f = mathByDepth<MSin >(T); bha_nga;
    pak_sa OP_COS:  f = mathByDepth<MCos >(T); bha_nga;
    pak_sa OP_TANH: f = mathByDepth<MTanh>(T); bha_nga;
    pak_sa OP_RELU: f = mathByDepth<MRelu>(T); bha_nga;
    pak_sa OP_ERF:  f = mathByDepth<MErf >(T); bha_nga;
    default: ;
    }
    ཕྱིར {f, nullptr, 0};
}


#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
