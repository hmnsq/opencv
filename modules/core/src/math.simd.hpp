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

#अन्तर्भाव "opencv2/core/hal/intrin.hpp"
#अन्तर्भाव "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#अन्तर्भाव "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#अन्तर्भाव <cmath>

नामाकाश cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#जर (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
स्थिर ओळीत शून्य v_store_pair_as(hfloat* p, स्थिरांक v_float32& a, स्थिरांक v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
स्थिर ओळीत शून्य v_store_pair_as(bfloat* p, स्थिरांक v_float32& a, स्थिरांक v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#endif
#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

नामाकाश ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(शीर्ष op, पूर्णांक T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(पूर्णांक T, पूर्णांक R);                   // OP_POW, T x T -> T (R must equal T)

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
संरचना MSqrt {
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& x) { परत v_sqrt(x); }
    ढाँचा<typename W> स्थिर W scl(W x) { परत std::वर्ग_मूल(x); }
};
संरचना MExp {
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& x) { परत v_exp(x); }
    ढाँचा<typename W> स्थिर W scl(W x) { परत std::exp(x); }
};
संरचना MLog {
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& x) { परत v_log(x); }
    ढाँचा<typename W> स्थिर W scl(W x) { परत std::log(x); }
};
संरचना MSin {
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& x) { परत v_sin(x); }
    ढाँचा<typename W> स्थिर W scl(W x) { परत std::sin(x); }
};
संरचना MCos {
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& x) { परत v_cos(x); }
    ढाँचा<typename W> स्थिर W scl(W x) { परत std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
संरचना MTanh {
    स्थिर v_float32 vec(स्थिरांक v_float32& x)
    {
        स्थिरांक v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        परत v_div(v_sub(e, one), v_add(e, one));
    }
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिर v_float64 vec(स्थिरांक v_float64& x)
    {
        स्थिरांक v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        परत v_div(v_sub(e, one), v_add(e, one));
    }
#endif
    ढाँचा<typename W> स्थिर W scl(W x) { परत std::tanh(x); }
};
संरचना MErf {
    स्थिर v_float32 vec(स्थिरांक v_float32& x) { परत v_erf(x); }
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    स्थिर v_float64 vec(स्थिरांक v_float64& x)
    {
        द्विगुण buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        साठी (पूर्णांक i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        परत vx_load(buf);
    }
#endif
    ढाँचा<typename W> स्थिर W scl(W x) { परत std::erf(x); }
};
संरचना MRelu {
    ढाँचा<typename V> स्थिर V vec(स्थिरांक V& x) { परत v_max(x, v_setzero_<V>()); }
    ढाँचा<typename W> स्थिर W scl(W x) { परत x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
ढाँचा<typename T, typename Wvec, वर्ग Op>
स्थिर पूर्णांक vecUnaryKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                          स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार, स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                          शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                          स्थिरांक द्विगुण*, पूर्णांक, शून्य*)
{
    s0y /= आकार(T);
    dsty /= आकार(T);
    CV_Assert(s0x <= 1u);

    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    जर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    स्थिरांक पूर्णांक urows = (s0y == 0 && height > 1) ? 1 : height;

    साठी (पूर्णांक y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        जर (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            साठी (पूर्णांक x = 0; x < width; x++) dst[x] = v;
            सुरूठेवा;
        }
        पूर्णांक x = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
        स्थिरांक पूर्णांक VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        स्थिरांक तार्किक use_tail_trick = width >= VECSZ*4 && src0_ != dst_;
        साठी (; x < width; x += VECSZ*2)
        {
            जर (x + VECSZ*2 > width) { जर (!use_tail_trick || x == 0) थांबा; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#endif
        साठी (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    साठी (पूर्णांक y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*आकार(T));
    परत 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
ढाँचा<typename T, typename WT, वर्ग Op>
स्थिर पूर्णांक scalarUnaryKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                             स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार, स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                             शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                             स्थिरांक द्विगुण*, पूर्णांक, शून्य*)
{
    s0y /= आकार(T);
    dsty /= आकार(T);
    CV_Assert(s0x <= 1u);
    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    T* dst = (T*)dst_;
    जर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width) { width *= height; height = 1; }
    स्थिरांक पूर्णांक urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    साठी (पूर्णांक y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        जर (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            साठी (पूर्णांक x = 0; x < width; x++) dst[x] = v;
            सुरूठेवा;
        }
        साठी (पूर्णांक x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    साठी (पूर्णांक y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*आकार(T));
    परत 0;
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
#जर (CV_SIMD || CV_SIMD_SCALABLE)
स्थिर ओळीत v_float32 vxSetallW(दशांश v,  स्थिरांक v_float32&) { परत vx_setall_f32(v); }
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
स्थिर ओळीत v_float64 vxSetallW(द्विगुण v, स्थिरांक v_float64&) { परत vx_setall_f64(v); }
#endif
#endif

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
ढाँचा<typename T, typename WT>
स्थिर पूर्णांक scalarPowKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                           स्थिरांक शून्य* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                           स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                           शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                           स्थिरांक द्विगुण*, पूर्णांक, शून्य*)
{
    s0y /= आकार(T); s1y /= आकार(T); dsty /= आकार(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* src1 = (स्थिरांक T*)src1_;
    T* dst = (T*)dst_;
    जर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width && s1y == s1x*(माप_प्रकार)width)
    { width *= height; height = 1; }
    साठी (पूर्णांक y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        साठी (पूर्णांक x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::घात((WT)src0[x*s0x], (WT)src1[x*s1x]));
    परत 0;
}

ढाँचा<typename T, typename Wvec>
स्थिर पूर्णांक powKernel(स्थिरांक शून्य* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                     स्थिरांक शून्य* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                     स्थिरांक शून्य*, माप_प्रकार, माप_प्रकार,
                     शून्य* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                     स्थिरांक द्विगुण*, पूर्णांक, शून्य*)
{
    s0y /= आकार(T);
    s1y /= आकार(T);
    dsty /= आकार(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    स्थिरांक T* src0 = (स्थिरांक T*)src0_;
    स्थिरांक T* src1 = (स्थिरांक T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    जर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width && s1y == s1x*(माप_प्रकार)width)
    { width *= height; height = 1; }

    [[maybe_unused]] स्थिरांक तार्किक tail_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    स्थिरांक पूर्णांक urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    साठी (पूर्णांक y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        पूर्णांक x = 0;
        जर (s1x == 0)                                   // scalar exponent for this row
        {
            स्थिरांक WT p = (WT)src1[0];
#जर (CV_SIMD || CV_SIMD_SCALABLE)
            स्थिरांक पूर्णांक VECSZ = VTraits<Wvec>::vlanes();
            जर (s0x == 1)
            {
                जर (p == WT(2) || p == WT(3))
                {
                    साठी (; x < width; x += VECSZ*2)
                    {
                        जर (x + VECSZ*2 > width) { जर (!tail_trick || x == 0) थांबा; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        जर (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                अन्यथा जर (p == WT(0.5))
                {
                    साठी (; x < width; x += VECSZ*2)
                    {
                        जर (x + VECSZ*2 > width) { जर (!tail_trick || x == 0) थांबा; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा जर (p == WT(1))
                {
                    जर ((स्थिरांक शून्य*)src0 != (स्थिरांक शून्य*)dst)
                        साठी (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                अन्यथा जर (p == WT(0))
                {
                    स्थिरांक T one = saturate_cast<T>(1);
                    साठी (; x < width; x++) dst[x] = one;
                }
                अन्यथा जर (p == WT(-0.5))
                {
                    स्थिरांक Wvec one = vxSetallW(WT(1), Wvec());
                    साठी (; x < width; x += VECSZ*2)
                    {
                        जर (x + VECSZ*2 > width) { जर (!tail_trick || x == 0) थांबा; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा जर (p == std::rint(p) && std::असल(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    स्थिरांक पूर्णांक ip = (पूर्णांक)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    स्थिरांक Wvec one = vxSetallW(WT(1), Wvec());
                    साठी (; x < width; x += VECSZ*2)
                    {
                        जर (x + VECSZ*2 > width) { जर (!tail_trick || x == 0) थांबा; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        साठी (पूर्णांक q = ap; q > 1; q >>= 1)
                        {
                            जर (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        जर (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा                                    // general scalar exponent: exp(p * log(x))
                {
                    स्थिरांक Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    साठी (; x < width; x += VECSZ*2)
                    {
                        जर (x + VECSZ*2 > width) { जर (!tail_trick || x == 0) थांबा; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        जर (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            साठी (पूर्णांक i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::घात((WT)src0[x + i], p));
                            सुरूठेवा;
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
            साठी (; x < width; x++)
                dst[x] = saturate_cast<T>(std::घात((WT)src0[x*s0x], p));
            सुरूठेवा;
        }
        // per-element exponent
#जर (CV_SIMD || CV_SIMD_SCALABLE)
        जर (s0x == 1)
        {
            स्थिरांक पूर्णांक VECSZ = VTraits<Wvec>::vlanes();
            स्थिरांक Wvec z = v_setzero_<Wvec>();
            साठी (; x < width; x += VECSZ*2)
            {
                जर (x + VECSZ*2 > width) { जर (!tail_trick || x == 0) थांबा; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                जर (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    साठी (पूर्णांक i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::घात((WT)src0[x + i], (WT)src1[x + i]));
                    सुरूठेवा;
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
        साठी (; x < width; x++)
            dst[x] = saturate_cast<T>(std::घात((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    साठी (पूर्णांक y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*आकार(T));
    परत 0;
}

TKernel getPowFunc_(पूर्णांक T, पूर्णांक R)
{
    जर (R != T)
        परत {};
    KernelFunc fptr = nullptr;
    निवड (T)
    {
    स्थिती CV_16F:  fptr = powKernel<hfloat, v_float32>; थांबा;
    स्थिती CV_16BF: fptr = powKernel<bfloat, v_float32>; थांबा;
    स्थिती CV_32F:  fptr = powKernel<दशांश,  v_float32>; थांबा;
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिती CV_64F:  fptr = powKernel<द्विगुण, v_float64>; थांबा;
#अन्यथा
    स्थिती CV_64F:  fptr = scalarPowKernel<द्विगुण, द्विगुण>; थांबा;
#endif
    पूर्वनिर्धारित: ;
    }
    परत {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
ढाँचा<वर्ग Op>
स्थिर KernelFunc mathByDepth(पूर्णांक T)
{
    निवड (T)
    {
    स्थिती CV_16F:  परत vecUnaryKernel<hfloat, v_float32, Op>;
    स्थिती CV_16BF: परत vecUnaryKernel<bfloat, v_float32, Op>;
    स्थिती CV_32F:  परत vecUnaryKernel<दशांश,  v_float32, Op>;
#जर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिती CV_64F:  परत vecUnaryKernel<द्विगुण, v_float64, Op>;
#अन्यथा
    स्थिती CV_64F:  परत scalarUnaryKernel<द्विगुण, द्विगुण, Op>;
#endif
    पूर्वनिर्धारित:      परत nullptr;
    }
}

TKernel getMathFunc_(शीर्ष op, पूर्णांक T)
{
    KernelFunc f = nullptr;
    निवड (op)
    {
    स्थिती OP_SQRT: f = mathByDepth<MSqrt>(T); थांबा;
    स्थिती OP_EXP:  f = mathByDepth<MExp >(T); थांबा;
    स्थिती OP_LOG:  f = mathByDepth<MLog >(T); थांबा;
    स्थिती OP_SIN:  f = mathByDepth<MSin >(T); थांबा;
    स्थिती OP_COS:  f = mathByDepth<MCos >(T); थांबा;
    स्थिती OP_TANH: f = mathByDepth<MTanh>(T); थांबा;
    स्थिती OP_RELU: f = mathByDepth<MRelu>(T); थांबा;
    स्थिती OP_ERF:  f = mathByDepth<MErf >(T); थांबा;
    पूर्वनिर्धारित: ;
    }
    परत {f, nullptr, 0};
}


#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
