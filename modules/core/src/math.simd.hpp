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

#അന്തര്ഭാവ "opencv2/core/hal/intrin.hpp"
#അന്തര്ഭാവ "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#അന്തര്ഭാവ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#അന്തര്ഭാവ <cmath>

നാമാകാശ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(hfloat* p, സ്ഥിരം v_float32& a, സ്ഥിരം v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
സ്ഥിതം വരിയിൽ ശൂന്യം v_store_pair_as(bfloat* p, സ്ഥിരം v_float32& a, സ്ഥിരം v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#endif
#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

നാമാകാശ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(മുകൾ op, പൂർണ്ണസംഖ്യ T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R);                   // OP_POW, T x T -> T (R must equal T)

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
ഘടന MSqrt {
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& x) { തിരികെ v_sqrt(x); }
    template<typename W> സ്ഥിതം W scl(W x) { തിരികെ std::sqrt(x); }
};
ഘടന MExp {
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& x) { തിരികെ v_exp(x); }
    template<typename W> സ്ഥിതം W scl(W x) { തിരികെ std::exp(x); }
};
ഘടന MLog {
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& x) { തിരികെ v_log(x); }
    template<typename W> സ്ഥിതം W scl(W x) { തിരികെ std::log(x); }
};
ഘടന MSin {
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& x) { തിരികെ v_sin(x); }
    template<typename W> സ്ഥിതം W scl(W x) { തിരികെ std::sin(x); }
};
ഘടന MCos {
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& x) { തിരികെ v_cos(x); }
    template<typename W> സ്ഥിതം W scl(W x) { തിരികെ std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
ഘടന MTanh {
    സ്ഥിതം v_float32 vec(സ്ഥിരം v_float32& x)
    {
        സ്ഥിരം v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        തിരികെ v_div(v_sub(e, one), v_add(e, one));
    }
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    സ്ഥിതം v_float64 vec(സ്ഥിരം v_float64& x)
    {
        സ്ഥിരം v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        തിരികെ v_div(v_sub(e, one), v_add(e, one));
    }
#endif
    template<typename W> സ്ഥിതം W scl(W x) { തിരികെ std::tanh(x); }
};
ഘടന MErf {
    സ്ഥിതം v_float32 vec(സ്ഥിരം v_float32& x) { തിരികെ v_erf(x); }
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    സ്ഥിതം v_float64 vec(സ്ഥിരം v_float64& x)
    {
        ഇരട്ട buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        തിരികെ vx_load(buf);
    }
#endif
    template<typename W> സ്ഥിതം W scl(W x) { തിരികെ std::erf(x); }
};
ഘടന MRelu {
    template<typename V> സ്ഥിതം V vec(സ്ഥിരം V& x) { തിരികെ v_max(x, v_setzero_<V>()); }
    template<typename W> സ്ഥിതം W scl(W x) { തിരികെ x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
template<typename T, typename Wvec, വർഗ്ഗം Op>
സ്ഥിതം പൂർണ്ണസംഖ്യ vecUnaryKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                          സ്ഥിരം ശൂന്യം*, size_t, size_t, സ്ഥിരം ശൂന്യം*, size_t, size_t,
                          ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                          സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    s0y /= വലുപ്പം(T);
    dsty /= വലുപ്പം(T);
    CV_Assert(s0x <= 1u);

    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    എങ്കിൽ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    സ്ഥിരം പൂർണ്ണസംഖ്യ urows = (s0y == 0 && height > 1) ? 1 : height;

    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        എങ്കിൽ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++) dst[x] = v;
            തുടരുക;
        }
        പൂർണ്ണസംഖ്യ x = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        സ്ഥിരം തർക്കം use_tail_trick = width >= VECSZ*4 && src0_ != dst_;
        വേണ്ടി (; x < width; x += VECSZ*2)
        {
            എങ്കിൽ (x + VECSZ*2 > width) { എങ്കിൽ (!use_tail_trick || x == 0) നിർത്തുക; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#endif
        വേണ്ടി (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*വലുപ്പം(T));
    തിരികെ 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
template<typename T, typename WT, വർഗ്ഗം Op>
സ്ഥിതം പൂർണ്ണസംഖ്യ scalarUnaryKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                             സ്ഥിരം ശൂന്യം*, size_t, size_t, സ്ഥിരം ശൂന്യം*, size_t, size_t,
                             ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                             സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    s0y /= വലുപ്പം(T);
    dsty /= വലുപ്പം(T);
    CV_Assert(s0x <= 1u);
    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    T* dst = (T*)dst_;
    എങ്കിൽ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    സ്ഥിരം പൂർണ്ണസംഖ്യ urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        എങ്കിൽ (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++) dst[x] = v;
            തുടരുക;
        }
        വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*വലുപ്പം(T));
    തിരികെ 0;
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
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
സ്ഥിതം വരിയിൽ v_float32 vxSetallW(ദശാംശം v,  സ്ഥിരം v_float32&) { തിരികെ vx_setall_f32(v); }
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
സ്ഥിതം വരിയിൽ v_float64 vxSetallW(ഇരട്ട v, സ്ഥിരം v_float64&) { തിരികെ vx_setall_f64(v); }
#endif
#endif

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
template<typename T, typename WT>
സ്ഥിതം പൂർണ്ണസംഖ്യ scalarPowKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                           സ്ഥിരം ശൂന്യം* src1_, size_t s1y, size_t s1x,
                           സ്ഥിരം ശൂന്യം*, size_t, size_t,
                           ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                           സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    s0y /= വലുപ്പം(T); s1y /= വലുപ്പം(T); dsty /= വലുപ്പം(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* src1 = (സ്ഥിരം T*)src1_;
    T* dst = (T*)dst_;
    എങ്കിൽ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x*s1x]));
    തിരികെ 0;
}

template<typename T, typename Wvec>
സ്ഥിതം പൂർണ്ണസംഖ്യ powKernel(സ്ഥിരം ശൂന്യം* src0_, size_t s0y, size_t s0x,
                     സ്ഥിരം ശൂന്യം* src1_, size_t s1y, size_t s1x,
                     സ്ഥിരം ശൂന്യം*, size_t, size_t,
                     ശൂന്യം* dst_, size_t dsty, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                     സ്ഥിരം ഇരട്ട*, പൂർണ്ണസംഖ്യ, ശൂന്യം*)
{
    s0y /= വലുപ്പം(T);
    s1y /= വലുപ്പം(T);
    dsty /= വലുപ്പം(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    സ്ഥിരം T* src0 = (സ്ഥിരം T*)src0_;
    സ്ഥിരം T* src1 = (സ്ഥിരം T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    എങ്കിൽ (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }

    [[maybe_unused]] സ്ഥിരം തർക്കം tail_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    സ്ഥിരം പൂർണ്ണസംഖ്യ urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        പൂർണ്ണസംഖ്യ x = 0;
        എങ്കിൽ (s1x == 0)                                   // scalar exponent for this row
        {
            സ്ഥിരം WT p = (WT)src1[0];
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
            സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Wvec>::vlanes();
            എങ്കിൽ (s0x == 1)
            {
                എങ്കിൽ (p == WT(2) || p == WT(3))
                {
                    വേണ്ടി (; x < width; x += VECSZ*2)
                    {
                        എങ്കിൽ (x + VECSZ*2 > width) { എങ്കിൽ (!tail_trick || x == 0) നിർത്തുക; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        എങ്കിൽ (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ (p == WT(0.5))
                {
                    വേണ്ടി (; x < width; x += VECSZ*2)
                    {
                        എങ്കിൽ (x + VECSZ*2 > width) { എങ്കിൽ (!tail_trick || x == 0) നിർത്തുക; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ (p == WT(1))
                {
                    എങ്കിൽ ((സ്ഥിരം ശൂന്യം*)src0 != (സ്ഥിരം ശൂന്യം*)dst)
                        വേണ്ടി (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                അല്ലെങ്കിൽ എങ്കിൽ (p == WT(0))
                {
                    സ്ഥിരം T one = saturate_cast<T>(1);
                    വേണ്ടി (; x < width; x++) dst[x] = one;
                }
                അല്ലെങ്കിൽ എങ്കിൽ (p == WT(-0.5))
                {
                    സ്ഥിരം Wvec one = vxSetallW(WT(1), Wvec());
                    വേണ്ടി (; x < width; x += VECSZ*2)
                    {
                        എങ്കിൽ (x + VECSZ*2 > width) { എങ്കിൽ (!tail_trick || x == 0) നിർത്തുക; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ (p == std::rint(p) && std::abs(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    സ്ഥിരം പൂർണ്ണസംഖ്യ ip = (പൂർണ്ണസംഖ്യ)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    സ്ഥിരം Wvec one = vxSetallW(WT(1), Wvec());
                    വേണ്ടി (; x < width; x += VECSZ*2)
                    {
                        എങ്കിൽ (x + VECSZ*2 > width) { എങ്കിൽ (!tail_trick || x == 0) നിർത്തുക; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        വേണ്ടി (പൂർണ്ണസംഖ്യ q = ap; q > 1; q >>= 1)
                        {
                            എങ്കിൽ (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        എങ്കിൽ (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                അല്ലെങ്കിൽ                                    // general scalar exponent: exp(p * log(x))
                {
                    സ്ഥിരം Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    വേണ്ടി (; x < width; x += VECSZ*2)
                    {
                        എങ്കിൽ (x + VECSZ*2 > width) { എങ്കിൽ (!tail_trick || x == 0) നിർത്തുക; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        എങ്കിൽ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], p));
                            തുടരുക;
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
            വേണ്ടി (; x < width; x++)
                dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], p));
            തുടരുക;
        }
        // per-element exponent
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
        എങ്കിൽ (s0x == 1)
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ VECSZ = VTraits<Wvec>::vlanes();
            സ്ഥിരം Wvec z = v_setzero_<Wvec>();
            വേണ്ടി (; x < width; x += VECSZ*2)
            {
                എങ്കിൽ (x + VECSZ*2 > width) { എങ്കിൽ (!tail_trick || x == 0) നിർത്തുക; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                എങ്കിൽ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], (WT)src1[x + i]));
                    തുടരുക;
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
        വേണ്ടി (; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    വേണ്ടി (പൂർണ്ണസംഖ്യ y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*വലുപ്പം(T));
    തിരികെ 0;
}

TKernel getPowFunc_(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)
{
    എങ്കിൽ (R != T)
        തിരികെ {};
    KernelFunc fptr = nullptr;
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_16F:  fptr = powKernel<hfloat, v_float32>; നിർത്തുക;
    അവസ്ഥ CV_16BF: fptr = powKernel<bfloat, v_float32>; നിർത്തുക;
    അവസ്ഥ CV_32F:  fptr = powKernel<ദശാംശം,  v_float32>; നിർത്തുക;
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    അവസ്ഥ CV_64F:  fptr = powKernel<ഇരട്ട, v_float64>; നിർത്തുക;
#അല്ലെങ്കിൽ
    അവസ്ഥ CV_64F:  fptr = scalarPowKernel<ഇരട്ട, ഇരട്ട>; നിർത്തുക;
#endif
    സ്ഥിരസ്ഥിതി: ;
    }
    തിരികെ {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
template<വർഗ്ഗം Op>
സ്ഥിതം KernelFunc mathByDepth(പൂർണ്ണസംഖ്യ T)
{
    തിരഞ്ഞെടുപ്പ് (T)
    {
    അവസ്ഥ CV_16F:  തിരികെ vecUnaryKernel<hfloat, v_float32, Op>;
    അവസ്ഥ CV_16BF: തിരികെ vecUnaryKernel<bfloat, v_float32, Op>;
    അവസ്ഥ CV_32F:  തിരികെ vecUnaryKernel<ദശാംശം,  v_float32, Op>;
#എങ്കിൽ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    അവസ്ഥ CV_64F:  തിരികെ vecUnaryKernel<ഇരട്ട, v_float64, Op>;
#അല്ലെങ്കിൽ
    അവസ്ഥ CV_64F:  തിരികെ scalarUnaryKernel<ഇരട്ട, ഇരട്ട, Op>;
#endif
    സ്ഥിരസ്ഥിതി:      തിരികെ nullptr;
    }
}

TKernel getMathFunc_(മുകൾ op, പൂർണ്ണസംഖ്യ T)
{
    KernelFunc f = nullptr;
    തിരഞ്ഞെടുപ്പ് (op)
    {
    അവസ്ഥ OP_SQRT: f = mathByDepth<MSqrt>(T); നിർത്തുക;
    അവസ്ഥ OP_EXP:  f = mathByDepth<MExp >(T); നിർത്തുക;
    അവസ്ഥ OP_LOG:  f = mathByDepth<MLog >(T); നിർത്തുക;
    അവസ്ഥ OP_SIN:  f = mathByDepth<MSin >(T); നിർത്തുക;
    അവസ്ഥ OP_COS:  f = mathByDepth<MCos >(T); നിർത്തുക;
    അവസ്ഥ OP_TANH: f = mathByDepth<MTanh>(T); നിർത്തുക;
    അവസ്ഥ OP_RELU: f = mathByDepth<MRelu>(T); നിർത്തുക;
    അവസ്ഥ OP_ERF:  f = mathByDepth<MErf >(T); നിർത്തുക;
    സ്ഥിരസ്ഥിതി: ;
    }
    തിരികെ {f, nullptr, 0};
}


#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
