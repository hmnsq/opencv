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

#অন্তর্ভাৱ "opencv2/core/hal/intrin.hpp"
#অন্তর্ভাৱ "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#অন্তর্ভাৱ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#অন্তর্ভাৱ <cmath>

নামাকাশ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#যদি (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
স্থির অন্তঃস্থ শূন্য v_store_pair_as(hfloat* p, ধ্রুৱ v_float32& a, ধ্রুৱ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
স্থির অন্তঃস্থ শূন্য v_store_pair_as(bfloat* p, ধ্রুৱ v_float32& a, ধ্রুৱ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#endif
#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

নামাকাশ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(শীৰ্ষ op, পূর্ণাঙ্ক T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R);                   // OP_POW, T x T -> T (R must equal T)

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
সংরচনা MSqrt {
    template<typename V> স্থির V vec(ধ্রুৱ V& x) { ঘূৰাই v_sqrt(x); }
    template<typename W> স্থির W scl(W x) { ঘূৰাই std::sqrt(x); }
};
সংরচনা MExp {
    template<typename V> স্থির V vec(ধ্রুৱ V& x) { ঘূৰাই v_exp(x); }
    template<typename W> স্থির W scl(W x) { ঘূৰাই std::exp(x); }
};
সংরচনা MLog {
    template<typename V> স্থির V vec(ধ্রুৱ V& x) { ঘূৰাই v_log(x); }
    template<typename W> স্থির W scl(W x) { ঘূৰাই std::log(x); }
};
সংরচনা MSin {
    template<typename V> স্থির V vec(ধ্রুৱ V& x) { ঘূৰাই v_sin(x); }
    template<typename W> স্থির W scl(W x) { ঘূৰাই std::sin(x); }
};
সংরচনা MCos {
    template<typename V> স্থির V vec(ধ্রুৱ V& x) { ঘূৰাই v_cos(x); }
    template<typename W> স্থির W scl(W x) { ঘূৰাই std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
সংরচনা MTanh {
    স্থির v_float32 vec(ধ্রুৱ v_float32& x)
    {
        ধ্রুৱ v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        ঘূৰাই v_div(v_sub(e, one), v_add(e, one));
    }
#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    স্থির v_float64 vec(ধ্রুৱ v_float64& x)
    {
        ধ্রুৱ v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        ঘূৰাই v_div(v_sub(e, one), v_add(e, one));
    }
#endif
    template<typename W> স্থির W scl(W x) { ঘূৰাই std::tanh(x); }
};
সংরচনা MErf {
    স্থির v_float32 vec(ধ্রুৱ v_float32& x) { ঘূৰাই v_erf(x); }
#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    স্থির v_float64 vec(ধ্রুৱ v_float64& x)
    {
        দ্ৱিগুণ buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        বাবে (পূর্ণাঙ্ক i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        ঘূৰাই vx_load(buf);
    }
#endif
    template<typename W> স্থির W scl(W x) { ঘূৰাই std::erf(x); }
};
সংরচনা MRelu {
    template<typename V> স্থির V vec(ধ্রুৱ V& x) { ঘূৰাই v_max(x, v_setzero_<V>()); }
    template<typename W> স্থির W scl(W x) { ঘূৰাই x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
template<typename T, typename Wvec, শ্ৰেণী Op>
স্থির পূর্ণাঙ্ক vecUnaryKernel(ধ্রুৱ শূন্য* src0_, size_t s0y, size_t s0x,
                          ধ্রুৱ শূন্য*, size_t, size_t, ধ্রুৱ শূন্য*, size_t, size_t,
                          শূন্য* dst_, size_t dsty, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height,
                          ধ্রুৱ দ্ৱিগুণ*, পূর্ণাঙ্ক, শূন্য*)
{
    s0y /= পরিমাণ(T);
    dsty /= পরিমাণ(T);
    CV_Assert(s0x <= 1u);

    ধ্রুৱ T* src0 = (ধ্রুৱ T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    যদি (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ধ্রুৱ পূর্ণাঙ্ক urows = (s0y == 0 && height > 1) ? 1 : height;

    বাবে (পূর্ণাঙ্ক y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        যদি (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            বাবে (পূর্ণাঙ্ক x = 0; x < width; x++) dst[x] = v;
            অনুৱর্ত;
        }
        পূর্ণাঙ্ক x = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
        ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ধ্রুৱ তার্কিক use_tail_trick = width >= VECSZ*4 && src0_ != dst_;
        বাবে (; x < width; x += VECSZ*2)
        {
            যদি (x + VECSZ*2 > width) { যদি (!use_tail_trick || x == 0) ভঙ্গ; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#endif
        বাবে (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    বাবে (পূর্ণাঙ্ক y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*পরিমাণ(T));
    ঘূৰাই 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
template<typename T, typename WT, শ্ৰেণী Op>
স্থির পূর্ণাঙ্ক scalarUnaryKernel(ধ্রুৱ শূন্য* src0_, size_t s0y, size_t s0x,
                             ধ্রুৱ শূন্য*, size_t, size_t, ধ্রুৱ শূন্য*, size_t, size_t,
                             শূন্য* dst_, size_t dsty, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height,
                             ধ্রুৱ দ্ৱিগুণ*, পূর্ণাঙ্ক, শূন্য*)
{
    s0y /= পরিমাণ(T);
    dsty /= পরিমাণ(T);
    CV_Assert(s0x <= 1u);
    ধ্রুৱ T* src0 = (ধ্রুৱ T*)src0_;
    T* dst = (T*)dst_;
    যদি (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ধ্রুৱ পূর্ণাঙ্ক urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    বাবে (পূর্ণাঙ্ক y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        যদি (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            বাবে (পূর্ণাঙ্ক x = 0; x < width; x++) dst[x] = v;
            অনুৱর্ত;
        }
        বাবে (পূর্ণাঙ্ক x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    বাবে (পূর্ণাঙ্ক y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*পরিমাণ(T));
    ঘূৰাই 0;
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
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
স্থির অন্তঃস্থ v_float32 vxSetallW(ভিন্ন v,  ধ্রুৱ v_float32&) { ঘূৰাই vx_setall_f32(v); }
#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F
স্থির অন্তঃস্থ v_float64 vxSetallW(দ্ৱিগুণ v, ধ্রুৱ v_float64&) { ঘূৰাই vx_setall_f64(v); }
#endif
#endif

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
template<typename T, typename WT>
স্থির পূর্ণাঙ্ক scalarPowKernel(ধ্রুৱ শূন্য* src0_, size_t s0y, size_t s0x,
                           ধ্রুৱ শূন্য* src1_, size_t s1y, size_t s1x,
                           ধ্রুৱ শূন্য*, size_t, size_t,
                           শূন্য* dst_, size_t dsty, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height,
                           ধ্রুৱ দ্ৱিগুণ*, পূর্ণাঙ্ক, শূন্য*)
{
    s0y /= পরিমাণ(T); s1y /= পরিমাণ(T); dsty /= পরিমাণ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ধ্রুৱ T* src0 = (ধ্রুৱ T*)src0_;
    ধ্রুৱ T* src1 = (ধ্রুৱ T*)src1_;
    T* dst = (T*)dst_;
    যদি (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }
    বাবে (পূর্ণাঙ্ক y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        বাবে (পূর্ণাঙ্ক x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x*s1x]));
    ঘূৰাই 0;
}

template<typename T, typename Wvec>
স্থির পূর্ণাঙ্ক powKernel(ধ্রুৱ শূন্য* src0_, size_t s0y, size_t s0x,
                     ধ্রুৱ শূন্য* src1_, size_t s1y, size_t s1x,
                     ধ্রুৱ শূন্য*, size_t, size_t,
                     শূন্য* dst_, size_t dsty, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height,
                     ধ্রুৱ দ্ৱিগুণ*, পূর্ণাঙ্ক, শূন্য*)
{
    s0y /= পরিমাণ(T);
    s1y /= পরিমাণ(T);
    dsty /= পরিমাণ(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ধ্রুৱ T* src0 = (ধ্রুৱ T*)src0_;
    ধ্রুৱ T* src1 = (ধ্রুৱ T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    যদি (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }

    [[maybe_unused]] ধ্রুৱ তার্কিক tail_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ধ্রুৱ পূর্ণাঙ্ক urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    বাবে (পূর্ণাঙ্ক y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        পূর্ণাঙ্ক x = 0;
        যদি (s1x == 0)                                   // scalar exponent for this row
        {
            ধ্রুৱ WT p = (WT)src1[0];
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
            ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<Wvec>::vlanes();
            যদি (s0x == 1)
            {
                যদি (p == WT(2) || p == WT(3))
                {
                    বাবে (; x < width; x += VECSZ*2)
                    {
                        যদি (x + VECSZ*2 > width) { যদি (!tail_trick || x == 0) ভঙ্গ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        যদি (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                অন্যথা যদি (p == WT(0.5))
                {
                    বাবে (; x < width; x += VECSZ*2)
                    {
                        যদি (x + VECSZ*2 > width) { যদি (!tail_trick || x == 0) ভঙ্গ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                অন্যথা যদি (p == WT(1))
                {
                    যদি ((ধ্রুৱ শূন্য*)src0 != (ধ্রুৱ শূন্য*)dst)
                        বাবে (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                অন্যথা যদি (p == WT(0))
                {
                    ধ্রুৱ T one = saturate_cast<T>(1);
                    বাবে (; x < width; x++) dst[x] = one;
                }
                অন্যথা যদি (p == WT(-0.5))
                {
                    ধ্রুৱ Wvec one = vxSetallW(WT(1), Wvec());
                    বাবে (; x < width; x += VECSZ*2)
                    {
                        যদি (x + VECSZ*2 > width) { যদি (!tail_trick || x == 0) ভঙ্গ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                অন্যথা যদি (p == std::rint(p) && std::abs(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ধ্রুৱ পূর্ণাঙ্ক ip = (পূর্ণাঙ্ক)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ধ্রুৱ Wvec one = vxSetallW(WT(1), Wvec());
                    বাবে (; x < width; x += VECSZ*2)
                    {
                        যদি (x + VECSZ*2 > width) { যদি (!tail_trick || x == 0) ভঙ্গ; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        বাবে (পূর্ণাঙ্ক q = ap; q > 1; q >>= 1)
                        {
                            যদি (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        যদি (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                অন্যথা                                    // general scalar exponent: exp(p * log(x))
                {
                    ধ্রুৱ Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    বাবে (; x < width; x += VECSZ*2)
                    {
                        যদি (x + VECSZ*2 > width) { যদি (!tail_trick || x == 0) ভঙ্গ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        যদি (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            বাবে (পূর্ণাঙ্ক i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], p));
                            অনুৱর্ত;
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
            বাবে (; x < width; x++)
                dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], p));
            অনুৱর্ত;
        }
        // per-element exponent
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
        যদি (s0x == 1)
        {
            ধ্রুৱ পূর্ণাঙ্ক VECSZ = VTraits<Wvec>::vlanes();
            ধ্রুৱ Wvec z = v_setzero_<Wvec>();
            বাবে (; x < width; x += VECSZ*2)
            {
                যদি (x + VECSZ*2 > width) { যদি (!tail_trick || x == 0) ভঙ্গ; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                যদি (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    বাবে (পূর্ণাঙ্ক i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], (WT)src1[x + i]));
                    অনুৱর্ত;
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
        বাবে (; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    বাবে (পূর্ণাঙ্ক y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*পরিমাণ(T));
    ঘূৰাই 0;
}

TKernel getPowFunc_(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)
{
    যদি (R != T)
        ঘূৰাই {};
    KernelFunc fptr = nullptr;
    ৱিকল্প (T)
    {
    পক্ষ CV_16F:  fptr = powKernel<hfloat, v_float32>; ভঙ্গ;
    পক্ষ CV_16BF: fptr = powKernel<bfloat, v_float32>; ভঙ্গ;
    পক্ষ CV_32F:  fptr = powKernel<ভিন্ন,  v_float32>; ভঙ্গ;
#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    পক্ষ CV_64F:  fptr = powKernel<দ্ৱিগুণ, v_float64>; ভঙ্গ;
#অন্যথা
    পক্ষ CV_64F:  fptr = scalarPowKernel<দ্ৱিগুণ, দ্ৱিগুণ>; ভঙ্গ;
#endif
    স্ৱভাৱ: ;
    }
    ঘূৰাই {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
template<শ্ৰেণী Op>
স্থির KernelFunc mathByDepth(পূর্ণাঙ্ক T)
{
    ৱিকল্প (T)
    {
    পক্ষ CV_16F:  ঘূৰাই vecUnaryKernel<hfloat, v_float32, Op>;
    পক্ষ CV_16BF: ঘূৰাই vecUnaryKernel<bfloat, v_float32, Op>;
    পক্ষ CV_32F:  ঘূৰাই vecUnaryKernel<ভিন্ন,  v_float32, Op>;
#যদি CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    পক্ষ CV_64F:  ঘূৰাই vecUnaryKernel<দ্ৱিগুণ, v_float64, Op>;
#অন্যথা
    পক্ষ CV_64F:  ঘূৰাই scalarUnaryKernel<দ্ৱিগুণ, দ্ৱিগুণ, Op>;
#endif
    স্ৱভাৱ:      ঘূৰাই nullptr;
    }
}

TKernel getMathFunc_(শীৰ্ষ op, পূর্ণাঙ্ক T)
{
    KernelFunc f = nullptr;
    ৱিকল্প (op)
    {
    পক্ষ OP_SQRT: f = mathByDepth<MSqrt>(T); ভঙ্গ;
    পক্ষ OP_EXP:  f = mathByDepth<MExp >(T); ভঙ্গ;
    পক্ষ OP_LOG:  f = mathByDepth<MLog >(T); ভঙ্গ;
    পক্ষ OP_SIN:  f = mathByDepth<MSin >(T); ভঙ্গ;
    পক্ষ OP_COS:  f = mathByDepth<MCos >(T); ভঙ্গ;
    পক্ষ OP_TANH: f = mathByDepth<MTanh>(T); ভঙ্গ;
    পক্ষ OP_RELU: f = mathByDepth<MRelu>(T); ভঙ্গ;
    পক্ষ OP_ERF:  f = mathByDepth<MErf >(T); ভঙ্গ;
    স্ৱভাৱ: ;
    }
    ঘূৰাই {f, nullptr, 0};
}


#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
