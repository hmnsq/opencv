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

#ଅନ୍ତର୍ଭାଵ "opencv2/core/hal/intrin.hpp"
#ଅନ୍ତର୍ଭାଵ "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#ଅନ୍ତର୍ଭାଵ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#ଅନ୍ତର୍ଭାଵ <cmath>

ନାମାକାଶ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ v_store_pair_as(hfloat* p, ଅଚଳ v_float32& a, ଅଚଳ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ v_store_pair_as(bfloat* p, ଅଚଳ v_float32& a, ଅଚଳ v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#endif
#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

ନାମାକାଶ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R);                   // OP_POW, T x T -> T (R must equal T)

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
ସଂରଚନା MSqrt {
    template<typename V> ସ୍ଥିର V vec(ଅଚଳ V& x) { ଫେରନ୍ତୁ v_sqrt(x); }
    template<typename W> ସ୍ଥିର W scl(W x) { ଫେରନ୍ତୁ std::sqrt(x); }
};
ସଂରଚନା MExp {
    template<typename V> ସ୍ଥିର V vec(ଅଚଳ V& x) { ଫେରନ୍ତୁ v_exp(x); }
    template<typename W> ସ୍ଥିର W scl(W x) { ଫେରନ୍ତୁ std::exp(x); }
};
ସଂରଚନା MLog {
    template<typename V> ସ୍ଥିର V vec(ଅଚଳ V& x) { ଫେରନ୍ତୁ v_log(x); }
    template<typename W> ସ୍ଥିର W scl(W x) { ଫେରନ୍ତୁ std::log(x); }
};
ସଂରଚନା MSin {
    template<typename V> ସ୍ଥିର V vec(ଅଚଳ V& x) { ଫେରନ୍ତୁ v_sin(x); }
    template<typename W> ସ୍ଥିର W scl(W x) { ଫେରନ୍ତୁ std::sin(x); }
};
ସଂରଚନା MCos {
    template<typename V> ସ୍ଥିର V vec(ଅଚଳ V& x) { ଫେରନ୍ତୁ v_cos(x); }
    template<typename W> ସ୍ଥିର W scl(W x) { ଫେରନ୍ତୁ std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
ସଂରଚନା MTanh {
    ସ୍ଥିର v_float32 vec(ଅଚଳ v_float32& x)
    {
        ଅଚଳ v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        ଫେରନ୍ତୁ v_div(v_sub(e, one), v_add(e, one));
    }
#ଯଦି CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ସ୍ଥିର v_float64 vec(ଅଚଳ v_float64& x)
    {
        ଅଚଳ v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        ଫେରନ୍ତୁ v_div(v_sub(e, one), v_add(e, one));
    }
#endif
    template<typename W> ସ୍ଥିର W scl(W x) { ଫେରନ୍ତୁ std::tanh(x); }
};
ସଂରଚନା MErf {
    ସ୍ଥିର v_float32 vec(ଅଚଳ v_float32& x) { ଫେରନ୍ତୁ v_erf(x); }
#ଯଦି CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    ସ୍ଥିର v_float64 vec(ଅଚଳ v_float64& x)
    {
        ଦ୍ୱିଗୁଣ buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        ଫେରନ୍ତୁ vx_load(buf);
    }
#endif
    template<typename W> ସ୍ଥିର W scl(W x) { ଫେରନ୍ତୁ std::erf(x); }
};
ସଂରଚନା MRelu {
    template<typename V> ସ୍ଥିର V vec(ଅଚଳ V& x) { ଫେରନ୍ତୁ v_max(x, v_setzero_<V>()); }
    template<typename W> ସ୍ଥିର W scl(W x) { ଫେରନ୍ତୁ x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
template<typename T, typename Wvec, ଶ୍ରେଣୀ Op>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା vecUnaryKernel(ଅଚଳ ଶୂନ୍ୟ* src0_, size_t s0y, size_t s0x,
                          ଅଚଳ ଶୂନ୍ୟ*, size_t, size_t, ଅଚଳ ଶୂନ୍ୟ*, size_t, size_t,
                          ଶୂନ୍ୟ* dst_, size_t dsty, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height,
                          ଅଚଳ ଦ୍ୱିଗୁଣ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ*)
{
    s0y /= ଆକାର(T);
    dsty /= ଆକାର(T);
    CV_Assert(s0x <= 1u);

    ଅଚଳ T* src0 = (ଅଚଳ T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    ଯଦି (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା urows = (s0y == 0 && height > 1) ? 1 : height;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ଯଦି (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < width; x++) dst[x] = v;
            ଜାରି;
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ଅଚଳ ତାର୍କିକ use_tail_trick = width >= VECSZ*4 && src0_ != dst_;
        ପାଇଁ (; x < width; x += VECSZ*2)
        {
            ଯଦି (x + VECSZ*2 > width) { ଯଦି (!use_tail_trick || x == 0) ଭାଙ୍ଗ; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#endif
        ପାଇଁ (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*ଆକାର(T));
    ଫେରନ୍ତୁ 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
template<typename T, typename WT, ଶ୍ରେଣୀ Op>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarUnaryKernel(ଅଚଳ ଶୂନ୍ୟ* src0_, size_t s0y, size_t s0x,
                             ଅଚଳ ଶୂନ୍ୟ*, size_t, size_t, ଅଚଳ ଶୂନ୍ୟ*, size_t, size_t,
                             ଶୂନ୍ୟ* dst_, size_t dsty, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height,
                             ଅଚଳ ଦ୍ୱିଗୁଣ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ*)
{
    s0y /= ଆକାର(T);
    dsty /= ଆକାର(T);
    CV_Assert(s0x <= 1u);
    ଅଚଳ T* src0 = (ଅଚଳ T*)src0_;
    T* dst = (T*)dst_;
    ଯଦି (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ଯଦି (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < width; x++) dst[x] = v;
            ଜାରି;
        }
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*ଆକାର(T));
    ଫେରନ୍ତୁ 0;
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
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
ସ୍ଥିର ଧାଡ଼ିଭିତରେ v_float32 vxSetallW(ଦଶମିକ v,  ଅଚଳ v_float32&) { ଫେରନ୍ତୁ vx_setall_f32(v); }
#ଯଦି CV_SIMD_64F || CV_SIMD_SCALABLE_64F
ସ୍ଥିର ଧାଡ଼ିଭିତରେ v_float64 vxSetallW(ଦ୍ୱିଗୁଣ v, ଅଚଳ v_float64&) { ଫେରନ୍ତୁ vx_setall_f64(v); }
#endif
#endif

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
template<typename T, typename WT>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarPowKernel(ଅଚଳ ଶୂନ୍ୟ* src0_, size_t s0y, size_t s0x,
                           ଅଚଳ ଶୂନ୍ୟ* src1_, size_t s1y, size_t s1x,
                           ଅଚଳ ଶୂନ୍ୟ*, size_t, size_t,
                           ଶୂନ୍ୟ* dst_, size_t dsty, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height,
                           ଅଚଳ ଦ୍ୱିଗୁଣ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ*)
{
    s0y /= ଆକାର(T); s1y /= ଆକାର(T); dsty /= ଆକାର(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ଅଚଳ T* src0 = (ଅଚଳ T*)src0_;
    ଅଚଳ T* src1 = (ଅଚଳ T*)src1_;
    T* dst = (T*)dst_;
    ଯଦି (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x*s1x]));
    ଫେରନ୍ତୁ 0;
}

template<typename T, typename Wvec>
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା powKernel(ଅଚଳ ଶୂନ୍ୟ* src0_, size_t s0y, size_t s0x,
                     ଅଚଳ ଶୂନ୍ୟ* src1_, size_t s1y, size_t s1x,
                     ଅଚଳ ଶୂନ୍ୟ*, size_t, size_t,
                     ଶୂନ୍ୟ* dst_, size_t dsty, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height,
                     ଅଚଳ ଦ୍ୱିଗୁଣ*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଶୂନ୍ୟ*)
{
    s0y /= ଆକାର(T);
    s1y /= ଆକାର(T);
    dsty /= ଆକାର(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ଅଚଳ T* src0 = (ଅଚଳ T*)src0_;
    ଅଚଳ T* src1 = (ଅଚଳ T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    ଯଦି (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }

    [[maybe_unused]] ଅଚଳ ତାର୍କିକ tail_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0;
        ଯଦି (s1x == 0)                                   // scalar exponent for this row
        {
            ଅଚଳ WT p = (WT)src1[0];
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<Wvec>::vlanes();
            ଯଦି (s0x == 1)
            {
                ଯଦି (p == WT(2) || p == WT(3))
                {
                    ପାଇଁ (; x < width; x += VECSZ*2)
                    {
                        ଯଦି (x + VECSZ*2 > width) { ଯଦି (!tail_trick || x == 0) ଭାଙ୍ଗ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        ଯଦି (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                ଅନ୍ୟଥା ଯଦି (p == WT(0.5))
                {
                    ପାଇଁ (; x < width; x += VECSZ*2)
                    {
                        ଯଦି (x + VECSZ*2 > width) { ଯଦି (!tail_trick || x == 0) ଭାଙ୍ଗ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ଅନ୍ୟଥା ଯଦି (p == WT(1))
                {
                    ଯଦି ((ଅଚଳ ଶୂନ୍ୟ*)src0 != (ଅଚଳ ଶୂନ୍ୟ*)dst)
                        ପାଇଁ (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                ଅନ୍ୟଥା ଯଦି (p == WT(0))
                {
                    ଅଚଳ T one = saturate_cast<T>(1);
                    ପାଇଁ (; x < width; x++) dst[x] = one;
                }
                ଅନ୍ୟଥା ଯଦି (p == WT(-0.5))
                {
                    ଅଚଳ Wvec one = vxSetallW(WT(1), Wvec());
                    ପାଇଁ (; x < width; x += VECSZ*2)
                    {
                        ଯଦି (x + VECSZ*2 > width) { ଯଦି (!tail_trick || x == 0) ଭାଙ୍ଗ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ଅନ୍ୟଥା ଯଦି (p == std::rint(p) && std::abs(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ip = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ଅଚଳ Wvec one = vxSetallW(WT(1), Wvec());
                    ପାଇଁ (; x < width; x += VECSZ*2)
                    {
                        ଯଦି (x + VECSZ*2 > width) { ଯଦି (!tail_trick || x == 0) ଭାଙ୍ଗ; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା q = ap; q > 1; q >>= 1)
                        {
                            ଯଦି (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        ଯଦି (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                ଅନ୍ୟଥା                                    // general scalar exponent: exp(p * log(x))
                {
                    ଅଚଳ Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    ପାଇଁ (; x < width; x += VECSZ*2)
                    {
                        ଯଦି (x + VECSZ*2 > width) { ଯଦି (!tail_trick || x == 0) ଭାଙ୍ଗ; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        ଯଦି (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], p));
                            ଜାରି;
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
            ପାଇଁ (; x < width; x++)
                dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], p));
            ଜାରି;
        }
        // per-element exponent
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
        ଯଦି (s0x == 1)
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା VECSZ = VTraits<Wvec>::vlanes();
            ଅଚଳ Wvec z = v_setzero_<Wvec>();
            ପାଇଁ (; x < width; x += VECSZ*2)
            {
                ଯଦି (x + VECSZ*2 > width) { ଯଦି (!tail_trick || x == 0) ଭାଙ୍ଗ; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                ଯଦି (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], (WT)src1[x + i]));
                    ଜାରି;
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
        ପାଇଁ (; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*ଆକାର(T));
    ଫେରନ୍ତୁ 0;
}

TKernel getPowFunc_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)
{
    ଯଦି (R != T)
        ଫେରନ୍ତୁ {};
    KernelFunc fptr = nullptr;
    ବାଛ (T)
    {
    ସ୍ଥିତି CV_16F:  fptr = powKernel<hfloat, v_float32>; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_16BF: fptr = powKernel<bfloat, v_float32>; ଭାଙ୍ଗ;
    ସ୍ଥିତି CV_32F:  fptr = powKernel<ଦଶମିକ,  v_float32>; ଭାଙ୍ଗ;
#ଯଦି CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ସ୍ଥିତି CV_64F:  fptr = powKernel<ଦ୍ୱିଗୁଣ, v_float64>; ଭାଙ୍ଗ;
#ଅନ୍ୟଥା
    ସ୍ଥିତି CV_64F:  fptr = scalarPowKernel<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ>; ଭାଙ୍ଗ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ;
    }
    ଫେରନ୍ତୁ {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
template<ଶ୍ରେଣୀ Op>
ସ୍ଥିର KernelFunc mathByDepth(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T)
{
    ବାଛ (T)
    {
    ସ୍ଥିତି CV_16F:  ଫେରନ୍ତୁ vecUnaryKernel<hfloat, v_float32, Op>;
    ସ୍ଥିତି CV_16BF: ଫେରନ୍ତୁ vecUnaryKernel<bfloat, v_float32, Op>;
    ସ୍ଥିତି CV_32F:  ଫେରନ୍ତୁ vecUnaryKernel<ଦଶମିକ,  v_float32, Op>;
#ଯଦି CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    ସ୍ଥିତି CV_64F:  ଫେରନ୍ତୁ vecUnaryKernel<ଦ୍ୱିଗୁଣ, v_float64, Op>;
#ଅନ୍ୟଥା
    ସ୍ଥିତି CV_64F:  ଫେରନ୍ତୁ scalarUnaryKernel<ଦ୍ୱିଗୁଣ, ଦ୍ୱିଗୁଣ, Op>;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:      ଫେରନ୍ତୁ nullptr;
    }
}

TKernel getMathFunc_(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା T)
{
    KernelFunc f = nullptr;
    ବାଛ (op)
    {
    ସ୍ଥିତି OP_SQRT: f = mathByDepth<MSqrt>(T); ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_EXP:  f = mathByDepth<MExp >(T); ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_LOG:  f = mathByDepth<MLog >(T); ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_SIN:  f = mathByDepth<MSin >(T); ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_COS:  f = mathByDepth<MCos >(T); ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_TANH: f = mathByDepth<MTanh>(T); ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_RELU: f = mathByDepth<MRelu>(T); ଭାଙ୍ଗ;
    ସ୍ଥିତି OP_ERF:  f = mathByDepth<MErf >(T); ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ;
    }
    ଫେରନ୍ତୁ {f, nullptr, 0};
}


#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
