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

#అన్తర్భావ "opencv2/core/hal/intrin.hpp"
#అన్తర్భావ "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#అన్తర్భావ "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#అన్తర్భావ <cmath>

నామాకాశ cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
స్థిర అంతఃవరుస శూన్య v_store_pair_as(hfloat* p, స్థిరరాశి v_float32& a, స్థిరరాశి v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
స్థిర అంతఃవరుస శూన్య v_store_pair_as(bfloat* p, స్థిరరాశి v_float32& a, స్థిరరాశి v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#అయితే_ముగింపు
#అయితే_ముగింపు // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

నామాకాశ ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(శిఖరం op, పూర్ణాంకం T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(పూర్ణాంకం T, పూర్ణాంకం R);                   // OP_POW, T x T -> T (R must equal T)

#నిర్వచితం_కాకపోతే CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
నిర్మాణం MSqrt {
    మూస<రకం_పేరు V> స్థిర V vec(స్థిరరాశి V& x) { తిరిగి v_sqrt(x); }
    మూస<రకం_పేరు W> స్థిర W scl(W x) { తిరిగి std::వర్గమూలం(x); }
};
నిర్మాణం MExp {
    మూస<రకం_పేరు V> స్థిర V vec(స్థిరరాశి V& x) { తిరిగి v_exp(x); }
    మూస<రకం_పేరు W> స్థిర W scl(W x) { తిరిగి std::ఘాతాంకం(x); }
};
నిర్మాణం MLog {
    మూస<రకం_పేరు V> స్థిర V vec(స్థిరరాశి V& x) { తిరిగి v_log(x); }
    మూస<రకం_పేరు W> స్థిర W scl(W x) { తిరిగి std::సంవర్గమానం(x); }
};
నిర్మాణం MSin {
    మూస<రకం_పేరు V> స్థిర V vec(స్థిరరాశి V& x) { తిరిగి v_sin(x); }
    మూస<రకం_పేరు W> స్థిర W scl(W x) { తిరిగి std::జ్యా(x); }
};
నిర్మాణం MCos {
    మూస<రకం_పేరు V> స్థిర V vec(స్థిరరాశి V& x) { తిరిగి v_cos(x); }
    మూస<రకం_పేరు W> స్థిర W scl(W x) { తిరిగి std::కోజ్యా(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
నిర్మాణం MTanh {
    స్థిర v_float32 vec(స్థిరరాశి v_float32& x)
    {
        స్థిరరాశి v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_కూడు(cx, cx));
        తిరిగి v_div(v_sub(e, one), v_కూడు(e, one));
    }
#ఒకవేళ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    స్థిర v_float64 vec(స్థిరరాశి v_float64& x)
    {
        స్థిరరాశి v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_కూడు(cx, cx));
        తిరిగి v_div(v_sub(e, one), v_కూడు(e, one));
    }
#అయితే_ముగింపు
    మూస<రకం_పేరు W> స్థిర W scl(W x) { తిరిగి std::tanh(x); }
};
నిర్మాణం MErf {
    స్థిర v_float32 vec(స్థిరరాశి v_float32& x) { తిరిగి v_erf(x); }
#ఒకవేళ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    స్థిర v_float64 vec(స్థిరరాశి v_float64& x)
    {
        ద్విగుణ buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        కొరకు (పూర్ణాంకం i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        తిరిగి vx_ఎక్కించు(buf);
    }
#అయితే_ముగింపు
    మూస<రకం_పేరు W> స్థిర W scl(W x) { తిరిగి std::erf(x); }
};
నిర్మాణం MRelu {
    మూస<రకం_పేరు V> స్థిర V vec(స్థిరరాశి V& x) { తిరిగి v_max(x, v_setzero_<V>()); }
    మూస<రకం_పేరు W> స్థిర W scl(W x) { తిరిగి x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
మూస<రకం_పేరు T, రకం_పేరు Wvec, తరగతి Op>
స్థిర పూర్ణాంకం vecUnaryKernel(స్థిరరాశి శూన్య* src0_, size_t s0y, size_t s0x,
                          స్థిరరాశి శూన్య*, size_t, size_t, స్థిరరాశి శూన్య*, size_t, size_t,
                          శూన్య* dst_, size_t dsty, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                          స్థిరరాశి ద్విగుణ*, పూర్ణాంకం, శూన్య*)
{
    s0y /= పరిమాణం(T);
    dsty /= పరిమాణం(T);
    CV_Assert(s0x <= 1u);

    స్థిరరాశి T* src0 = (స్థిరరాశి T*)src0_;
    T* dst = (T*)dst_;
    ఉపయోగించి WT = రకం_పేరు VTraits<Wvec>::lane_type;

    ఒకవేళ (ఎత్తు > 1 && dsty == (size_t)వెడల్పు && s0y == s0x*(size_t)వెడల్పు) { వెడల్పు *= ఎత్తు; ఎత్తు = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    స్థిరరాశి పూర్ణాంకం urows = (s0y == 0 && ఎత్తు > 1) ? 1 : ఎత్తు;

    కొరకు (పూర్ణాంకం y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ఒకవేళ (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            కొరకు (పూర్ణాంకం x = 0; x < వెడల్పు; x++) dst[x] = v;
            కొనసాగు;
        }
        పూర్ణాంకం x = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        స్థిరరాశి తార్కిక use_తోక_trick = వెడల్పు >= VECSZ*4 && src0_ != dst_;
        కొరకు (; x < వెడల్పు; x += VECSZ*2)
        {
            ఒకవేళ (x + VECSZ*2 > వెడల్పు) { ఒకవేళ (!use_తోక_trick || x == 0) ఆపు; x = వెడల్పు - VECSZ*2; }
            Wvec a0, a1;
            vx_ఎక్కించు_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#అయితే_ముగింపు
        కొరకు (; x < వెడల్పు; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    కొరకు (పూర్ణాంకం y = urows; y < ఎత్తు; y++)
        జ్ఞాపకం_నకలు(dst + (size_t)y*dsty, dst, (size_t)వెడల్పు*పరిమాణం(T));
    తిరిగి 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
మూస<రకం_పేరు T, రకం_పేరు WT, తరగతి Op>
స్థిర పూర్ణాంకం scalarUnaryKernel(స్థిరరాశి శూన్య* src0_, size_t s0y, size_t s0x,
                             స్థిరరాశి శూన్య*, size_t, size_t, స్థిరరాశి శూన్య*, size_t, size_t,
                             శూన్య* dst_, size_t dsty, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                             స్థిరరాశి ద్విగుణ*, పూర్ణాంకం, శూన్య*)
{
    s0y /= పరిమాణం(T);
    dsty /= పరిమాణం(T);
    CV_Assert(s0x <= 1u);
    స్థిరరాశి T* src0 = (స్థిరరాశి T*)src0_;
    T* dst = (T*)dst_;
    ఒకవేళ (ఎత్తు > 1 && dsty == (size_t)వెడల్పు && s0y == s0x*(size_t)వెడల్పు) { వెడల్పు *= ఎత్తు; ఎత్తు = 1; }
    స్థిరరాశి పూర్ణాంకం urows = (s0y == 0 && ఎత్తు > 1) ? 1 : ఎత్తు;   // vertical broadcast: 1 row + copies
    కొరకు (పూర్ణాంకం y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        ఒకవేళ (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            కొరకు (పూర్ణాంకం x = 0; x < వెడల్పు; x++) dst[x] = v;
            కొనసాగు;
        }
        కొరకు (పూర్ణాంకం x = 0; x < వెడల్పు; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    కొరకు (పూర్ణాంకం y = urows; y < ఎత్తు; y++)
        జ్ఞాపకం_నకలు(dst + (size_t)y*dsty, dst, (size_t)వెడల్పు*పరిమాణం(T));
    తిరిగి 0;
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
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
స్థిర అంతఃవరుస v_float32 vxSetallW(దశాంశం v,  స్థిరరాశి v_float32&) { తిరిగి vx_setall_f32(v); }
#ఒకవేళ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
స్థిర అంతఃవరుస v_float64 vxSetallW(ద్విగుణ v, స్థిరరాశి v_float64&) { తిరిగి vx_setall_f64(v); }
#అయితే_ముగింపు
#అయితే_ముగింపు

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
మూస<రకం_పేరు T, రకం_పేరు WT>
స్థిర పూర్ణాంకం scalarPowKernel(స్థిరరాశి శూన్య* src0_, size_t s0y, size_t s0x,
                           స్థిరరాశి శూన్య* src1_, size_t s1y, size_t s1x,
                           స్థిరరాశి శూన్య*, size_t, size_t,
                           శూన్య* dst_, size_t dsty, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                           స్థిరరాశి ద్విగుణ*, పూర్ణాంకం, శూన్య*)
{
    s0y /= పరిమాణం(T); s1y /= పరిమాణం(T); dsty /= పరిమాణం(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    స్థిరరాశి T* src0 = (స్థిరరాశి T*)src0_;
    స్థిరరాశి T* src1 = (స్థిరరాశి T*)src1_;
    T* dst = (T*)dst_;
    ఒకవేళ (ఎత్తు > 1 && dsty == (size_t)వెడల్పు && s0y == s0x*(size_t)వెడల్పు && s1y == s1x*(size_t)వెడల్పు)
    { వెడల్పు *= ఎత్తు; ఎత్తు = 1; }
    కొరకు (పూర్ణాంకం y = 0; y < ఎత్తు; y++, src0 += s0y, src1 += s1y, dst += dsty)
        కొరకు (పూర్ణాంకం x = 0; x < వెడల్పు; x++)
            dst[x] = saturate_cast<T>(std::ఘాతం((WT)src0[x*s0x], (WT)src1[x*s1x]));
    తిరిగి 0;
}

మూస<రకం_పేరు T, రకం_పేరు Wvec>
స్థిర పూర్ణాంకం powKernel(స్థిరరాశి శూన్య* src0_, size_t s0y, size_t s0x,
                     స్థిరరాశి శూన్య* src1_, size_t s1y, size_t s1x,
                     స్థిరరాశి శూన్య*, size_t, size_t,
                     శూన్య* dst_, size_t dsty, పూర్ణాంకం వెడల్పు, పూర్ణాంకం ఎత్తు,
                     స్థిరరాశి ద్విగుణ*, పూర్ణాంకం, శూన్య*)
{
    s0y /= పరిమాణం(T);
    s1y /= పరిమాణం(T);
    dsty /= పరిమాణం(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    స్థిరరాశి T* src0 = (స్థిరరాశి T*)src0_;
    స్థిరరాశి T* src1 = (స్థిరరాశి T*)src1_;
    T* dst = (T*)dst_;
    ఉపయోగించి WT = రకం_పేరు VTraits<Wvec>::lane_type;

    ఒకవేళ (ఎత్తు > 1 && dsty == (size_t)వెడల్పు && s0y == s0x*(size_t)వెడల్పు && s1y == s1x*(size_t)వెడల్పు)
    { వెడల్పు *= ఎత్తు; ఎత్తు = 1; }

    [[maybe_unused]] స్థిరరాశి తార్కిక తోక_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    స్థిరరాశి పూర్ణాంకం urows = (s0y == 0 && s1y == 0 && ఎత్తు > 1) ? 1 : ఎత్తు;

    కొరకు (పూర్ణాంకం y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        పూర్ణాంకం x = 0;
        ఒకవేళ (s1x == 0)                                   // scalar exponent for this row
        {
            స్థిరరాశి WT p = (WT)src1[0];
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
            స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<Wvec>::vlanes();
            ఒకవేళ (s0x == 1)
            {
                ఒకవేళ (p == WT(2) || p == WT(3))
                {
                    కొరకు (; x < వెడల్పు; x += VECSZ*2)
                    {
                        ఒకవేళ (x + VECSZ*2 > వెడల్పు) { ఒకవేళ (!తోక_trick || x == 0) ఆపు; x = వెడల్పు - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ఎక్కించు_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        ఒకవేళ (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                లేకపోతే ఒకవేళ (p == WT(0.5))
                {
                    కొరకు (; x < వెడల్పు; x += VECSZ*2)
                    {
                        ఒకవేళ (x + VECSZ*2 > వెడల్పు) { ఒకవేళ (!తోక_trick || x == 0) ఆపు; x = వెడల్పు - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ఎక్కించు_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                లేకపోతే ఒకవేళ (p == WT(1))
                {
                    ఒకవేళ ((స్థిరరాశి శూన్య*)src0 != (స్థిరరాశి శూన్య*)dst)
                        కొరకు (; x < వెడల్పు; x++) dst[x] = src0[x];
                    x = వెడల్పు;
                }
                లేకపోతే ఒకవేళ (p == WT(0))
                {
                    స్థిరరాశి T one = saturate_cast<T>(1);
                    కొరకు (; x < వెడల్పు; x++) dst[x] = one;
                }
                లేకపోతే ఒకవేళ (p == WT(-0.5))
                {
                    స్థిరరాశి Wvec one = vxSetallW(WT(1), Wvec());
                    కొరకు (; x < వెడల్పు; x += VECSZ*2)
                    {
                        ఒకవేళ (x + VECSZ*2 > వెడల్పు) { ఒకవేళ (!తోక_trick || x == 0) ఆపు; x = వెడల్పు - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ఎక్కించు_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                లేకపోతే ఒకవేళ (p == std::rint(p) && std::నిరపేక్ష(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    స్థిరరాశి పూర్ణాంకం ip = (పూర్ణాంకం)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    స్థిరరాశి Wvec one = vxSetallW(WT(1), Wvec());
                    కొరకు (; x < వెడల్పు; x += VECSZ*2)
                    {
                        ఒకవేళ (x + VECSZ*2 > వెడల్పు) { ఒకవేళ (!తోక_trick || x == 0) ఆపు; x = వెడల్పు - VECSZ*2; }
                        Wvec b0, b1;
                        vx_ఎక్కించు_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        కొరకు (పూర్ణాంకం q = ap; q > 1; q >>= 1)
                        {
                            ఒకవేళ (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        ఒకవేళ (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                లేకపోతే                                    // general scalar exponent: exp(p * log(x))
                {
                    స్థిరరాశి Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    కొరకు (; x < వెడల్పు; x += VECSZ*2)
                    {
                        ఒకవేళ (x + VECSZ*2 > వెడల్పు) { ఒకవేళ (!తోక_trick || x == 0) ఆపు; x = వెడల్పు - VECSZ*2; }
                        Wvec a0, a1;
                        vx_ఎక్కించు_pair_as(src0 + x, a0, a1);
                        ఒకవేళ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            కొరకు (పూర్ణాంకం i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::ఘాతం((WT)src0[x + i], p));
                            కొనసాగు;
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
#అయితే_ముగింపు
            కొరకు (; x < వెడల్పు; x++)
                dst[x] = saturate_cast<T>(std::ఘాతం((WT)src0[x*s0x], p));
            కొనసాగు;
        }
        // per-element exponent
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
        ఒకవేళ (s0x == 1)
        {
            స్థిరరాశి పూర్ణాంకం VECSZ = VTraits<Wvec>::vlanes();
            స్థిరరాశి Wvec z = v_setzero_<Wvec>();
            కొరకు (; x < వెడల్పు; x += VECSZ*2)
            {
                ఒకవేళ (x + VECSZ*2 > వెడల్పు) { ఒకవేళ (!తోక_trick || x == 0) ఆపు; x = వెడల్పు - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_ఎక్కించు_pair_as(src0 + x, a0, a1);
                vx_ఎక్కించు_pair_as(src1 + x, b0, b1);
                ఒకవేళ (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    కొరకు (పూర్ణాంకం i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::ఘాతం((WT)src0[x + i], (WT)src1[x + i]));
                    కొనసాగు;
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
#అయితే_ముగింపు
        కొరకు (; x < వెడల్పు; x++)
            dst[x] = saturate_cast<T>(std::ఘాతం((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    కొరకు (పూర్ణాంకం y = urows; y < ఎత్తు; y++)
        జ్ఞాపకం_నకలు(dst + (size_t)y*dsty, dst, (size_t)వెడల్పు*పరిమాణం(T));
    తిరిగి 0;
}

TKernel getPowFunc_(పూర్ణాంకం T, పూర్ణాంకం R)
{
    ఒకవేళ (R != T)
        తిరిగి {};
    KernelFunc fptr = శూన్యసూచిక;
    ఎంపిక (T)
    {
    స్థితి CV_16F:  fptr = powKernel<hfloat, v_float32>; ఆపు;
    స్థితి CV_16BF: fptr = powKernel<bfloat, v_float32>; ఆపు;
    స్థితి CV_32F:  fptr = powKernel<దశాంశం,  v_float32>; ఆపు;
#ఒకవేళ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    స్థితి CV_64F:  fptr = powKernel<ద్విగుణ, v_float64>; ఆపు;
#లేకపోతే
    స్థితి CV_64F:  fptr = scalarPowKernel<ద్విగుణ, ద్విగుణ>; ఆపు;
#అయితే_ముగింపు
    అప్రమేయ: ;
    }
    తిరిగి {fptr, శూన్యసూచిక, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
మూస<తరగతి Op>
స్థిర KernelFunc math_By_లోతు(పూర్ణాంకం T)
{
    ఎంపిక (T)
    {
    స్థితి CV_16F:  తిరిగి vecUnaryKernel<hfloat, v_float32, Op>;
    స్థితి CV_16BF: తిరిగి vecUnaryKernel<bfloat, v_float32, Op>;
    స్థితి CV_32F:  తిరిగి vecUnaryKernel<దశాంశం,  v_float32, Op>;
#ఒకవేళ CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    స్థితి CV_64F:  తిరిగి vecUnaryKernel<ద్విగుణ, v_float64, Op>;
#లేకపోతే
    స్థితి CV_64F:  తిరిగి scalarUnaryKernel<ద్విగుణ, ద్విగుణ, Op>;
#అయితే_ముగింపు
    అప్రమేయ:      తిరిగి శూన్యసూచిక;
    }
}

TKernel getMathFunc_(శిఖరం op, పూర్ణాంకం T)
{
    KernelFunc f = శూన్యసూచిక;
    ఎంపిక (op)
    {
    స్థితి OP_SQRT: f = math_By_లోతు<MSqrt>(T); ఆపు;
    స్థితి OP_EXP:  f = math_By_లోతు<MExp >(T); ఆపు;
    స్థితి OP_LOG:  f = math_By_లోతు<MLog >(T); ఆపు;
    స్థితి OP_SIN:  f = math_By_లోతు<MSin >(T); ఆపు;
    స్థితి OP_COS:  f = math_By_లోతు<MCos >(T); ఆపు;
    స్థితి OP_TANH: f = math_By_లోతు<MTanh>(T); ఆపు;
    స్థితి OP_RELU: f = math_By_లోతు<MRelu>(T); ఆపు;
    స్థితి OP_ERF:  f = math_By_లోతు<MErf >(T); ఆపు;
    అప్రమేయ: ;
    }
    తిరిగి {f, శూన్యసూచిక, 0};
}


#అయితే_ముగింపు // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_చివర
}} // namespace cv::ew
