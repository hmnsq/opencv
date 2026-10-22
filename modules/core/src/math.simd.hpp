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
#यदि (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
स्थिर अन्तःस्थ शून्य v_store_pair_as(hfloat* p, ध्रुव v_float32& a, ध्रुव v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
स्थिर अन्तःस्थ शून्य v_store_pair_as(bfloat* p, ध्रुव v_float32& a, ध्रुव v_float32& b)
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
TKernel getMathFunc_(शीर्ष op, पूर्णाङ्क T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(पूर्णाङ्क T, पूर्णाङ्क R);                   // OP_POW, T x T -> T (R must equal T)

#ifndef CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
संरचना MSqrt {
    template<typename V> स्थिर V vec(ध्रुव V& x) { प्रत्यागम v_sqrt(x); }
    template<typename W> स्थिर W scl(W x) { प्रत्यागम std::sqrt(x); }
};
संरचना MExp {
    template<typename V> स्थिर V vec(ध्रुव V& x) { प्रत्यागम v_exp(x); }
    template<typename W> स्थिर W scl(W x) { प्रत्यागम std::exp(x); }
};
संरचना MLog {
    template<typename V> स्थिर V vec(ध्रुव V& x) { प्रत्यागम v_log(x); }
    template<typename W> स्थिर W scl(W x) { प्रत्यागम std::log(x); }
};
संरचना MSin {
    template<typename V> स्थिर V vec(ध्रुव V& x) { प्रत्यागम v_sin(x); }
    template<typename W> स्थिर W scl(W x) { प्रत्यागम std::sin(x); }
};
संरचना MCos {
    template<typename V> स्थिर V vec(ध्रुव V& x) { प्रत्यागम v_cos(x); }
    template<typename W> स्थिर W scl(W x) { प्रत्यागम std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
संरचना MTanh {
    स्थिर v_float32 vec(ध्रुव v_float32& x)
    {
        ध्रुव v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        प्रत्यागम v_div(v_sub(e, one), v_add(e, one));
    }
#यदि CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    स्थिर v_float64 vec(ध्रुव v_float64& x)
    {
        ध्रुव v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        प्रत्यागम v_div(v_sub(e, one), v_add(e, one));
    }
#endif
    template<typename W> स्थिर W scl(W x) { प्रत्यागम std::tanh(x); }
};
संरचना MErf {
    स्थिर v_float32 vec(ध्रुव v_float32& x) { प्रत्यागम v_erf(x); }
#यदि CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    स्थिर v_float64 vec(ध्रुव v_float64& x)
    {
        द्विगुण buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        प्रति (पूर्णाङ्क i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        प्रत्यागम vx_load(buf);
    }
#endif
    template<typename W> स्थिर W scl(W x) { प्रत्यागम std::erf(x); }
};
संरचना MRelu {
    template<typename V> स्थिर V vec(ध्रुव V& x) { प्रत्यागम v_max(x, v_setzero_<V>()); }
    template<typename W> स्थिर W scl(W x) { प्रत्यागम x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
template<typename T, typename Wvec, वर्ग Op>
स्थिर पूर्णाङ्क vecUnaryKernel(ध्रुव शून्य* src0_, size_t s0y, size_t s0x,
                          ध्रुव शून्य*, size_t, size_t, ध्रुव शून्य*, size_t, size_t,
                          शून्य* dst_, size_t dsty, पूर्णाङ्क width, पूर्णाङ्क height,
                          ध्रुव द्विगुण*, पूर्णाङ्क, शून्य*)
{
    s0y /= परिमाण(T);
    dsty /= परिमाण(T);
    CV_Assert(s0x <= 1u);

    ध्रुव T* src0 = (ध्रुव T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    यदि (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    ध्रुव पूर्णाङ्क urows = (s0y == 0 && height > 1) ? 1 : height;

    प्रति (पूर्णाङ्क y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        यदि (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            प्रति (पूर्णाङ्क x = 0; x < width; x++) dst[x] = v;
            अनुवर्त;
        }
        पूर्णाङ्क x = 0;
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        ध्रुव पूर्णाङ्क VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        ध्रुव तार्किक use_tail_trick = width >= VECSZ*4 && src0_ != dst_;
        प्रति (; x < width; x += VECSZ*2)
        {
            यदि (x + VECSZ*2 > width) { यदि (!use_tail_trick || x == 0) भङ्ग; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#endif
        प्रति (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    प्रति (पूर्णाङ्क y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*परिमाण(T));
    प्रत्यागम 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
template<typename T, typename WT, वर्ग Op>
स्थिर पूर्णाङ्क scalarUnaryKernel(ध्रुव शून्य* src0_, size_t s0y, size_t s0x,
                             ध्रुव शून्य*, size_t, size_t, ध्रुव शून्य*, size_t, size_t,
                             शून्य* dst_, size_t dsty, पूर्णाङ्क width, पूर्णाङ्क height,
                             ध्रुव द्विगुण*, पूर्णाङ्क, शून्य*)
{
    s0y /= परिमाण(T);
    dsty /= परिमाण(T);
    CV_Assert(s0x <= 1u);
    ध्रुव T* src0 = (ध्रुव T*)src0_;
    T* dst = (T*)dst_;
    यदि (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width) { width *= height; height = 1; }
    ध्रुव पूर्णाङ्क urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    प्रति (पूर्णाङ्क y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        यदि (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            प्रति (पूर्णाङ्क x = 0; x < width; x++) dst[x] = v;
            अनुवर्त;
        }
        प्रति (पूर्णाङ्क x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    प्रति (पूर्णाङ्क y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*परिमाण(T));
    प्रत्यागम 0;
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
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
स्थिर अन्तःस्थ v_float32 vxSetallW(भिन्न v,  ध्रुव v_float32&) { प्रत्यागम vx_setall_f32(v); }
#यदि CV_SIMD_64F || CV_SIMD_SCALABLE_64F
स्थिर अन्तःस्थ v_float64 vxSetallW(द्विगुण v, ध्रुव v_float64&) { प्रत्यागम vx_setall_f64(v); }
#endif
#endif

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
template<typename T, typename WT>
स्थिर पूर्णाङ्क scalarPowKernel(ध्रुव शून्य* src0_, size_t s0y, size_t s0x,
                           ध्रुव शून्य* src1_, size_t s1y, size_t s1x,
                           ध्रुव शून्य*, size_t, size_t,
                           शून्य* dst_, size_t dsty, पूर्णाङ्क width, पूर्णाङ्क height,
                           ध्रुव द्विगुण*, पूर्णाङ्क, शून्य*)
{
    s0y /= परिमाण(T); s1y /= परिमाण(T); dsty /= परिमाण(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    ध्रुव T* src0 = (ध्रुव T*)src0_;
    ध्रुव T* src1 = (ध्रुव T*)src1_;
    T* dst = (T*)dst_;
    यदि (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }
    प्रति (पूर्णाङ्क y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        प्रति (पूर्णाङ्क x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x*s1x]));
    प्रत्यागम 0;
}

template<typename T, typename Wvec>
स्थिर पूर्णाङ्क powKernel(ध्रुव शून्य* src0_, size_t s0y, size_t s0x,
                     ध्रुव शून्य* src1_, size_t s1y, size_t s1x,
                     ध्रुव शून्य*, size_t, size_t,
                     शून्य* dst_, size_t dsty, पूर्णाङ्क width, पूर्णाङ्क height,
                     ध्रुव द्विगुण*, पूर्णाङ्क, शून्य*)
{
    s0y /= परिमाण(T);
    s1y /= परिमाण(T);
    dsty /= परिमाण(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    ध्रुव T* src0 = (ध्रुव T*)src0_;
    ध्रुव T* src1 = (ध्रुव T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    यदि (height > 1 && dsty == (size_t)width && s0y == s0x*(size_t)width && s1y == s1x*(size_t)width)
    { width *= height; height = 1; }

    [[maybe_unused]] ध्रुव तार्किक tail_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    ध्रुव पूर्णाङ्क urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    प्रति (पूर्णाङ्क y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        पूर्णाङ्क x = 0;
        यदि (s1x == 0)                                   // scalar exponent for this row
        {
            ध्रुव WT p = (WT)src1[0];
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
            ध्रुव पूर्णाङ्क VECSZ = VTraits<Wvec>::vlanes();
            यदि (s0x == 1)
            {
                यदि (p == WT(2) || p == WT(3))
                {
                    प्रति (; x < width; x += VECSZ*2)
                    {
                        यदि (x + VECSZ*2 > width) { यदि (!tail_trick || x == 0) भङ्ग; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        यदि (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                अन्यथा यदि (p == WT(0.5))
                {
                    प्रति (; x < width; x += VECSZ*2)
                    {
                        यदि (x + VECSZ*2 > width) { यदि (!tail_trick || x == 0) भङ्ग; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा यदि (p == WT(1))
                {
                    यदि ((ध्रुव शून्य*)src0 != (ध्रुव शून्य*)dst)
                        प्रति (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                अन्यथा यदि (p == WT(0))
                {
                    ध्रुव T one = saturate_cast<T>(1);
                    प्रति (; x < width; x++) dst[x] = one;
                }
                अन्यथा यदि (p == WT(-0.5))
                {
                    ध्रुव Wvec one = vxSetallW(WT(1), Wvec());
                    प्रति (; x < width; x += VECSZ*2)
                    {
                        यदि (x + VECSZ*2 > width) { यदि (!tail_trick || x == 0) भङ्ग; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा यदि (p == std::rint(p) && std::abs(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    ध्रुव पूर्णाङ्क ip = (पूर्णाङ्क)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    ध्रुव Wvec one = vxSetallW(WT(1), Wvec());
                    प्रति (; x < width; x += VECSZ*2)
                    {
                        यदि (x + VECSZ*2 > width) { यदि (!tail_trick || x == 0) भङ्ग; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        प्रति (पूर्णाङ्क q = ap; q > 1; q >>= 1)
                        {
                            यदि (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        यदि (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा                                    // general scalar exponent: exp(p * log(x))
                {
                    ध्रुव Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    प्रति (; x < width; x += VECSZ*2)
                    {
                        यदि (x + VECSZ*2 > width) { यदि (!tail_trick || x == 0) भङ्ग; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        यदि (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            प्रति (पूर्णाङ्क i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], p));
                            अनुवर्त;
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
            प्रति (; x < width; x++)
                dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], p));
            अनुवर्त;
        }
        // per-element exponent
#यदि (CV_SIMD || CV_SIMD_SCALABLE)
        यदि (s0x == 1)
        {
            ध्रुव पूर्णाङ्क VECSZ = VTraits<Wvec>::vlanes();
            ध्रुव Wvec z = v_setzero_<Wvec>();
            प्रति (; x < width; x += VECSZ*2)
            {
                यदि (x + VECSZ*2 > width) { यदि (!tail_trick || x == 0) भङ्ग; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                यदि (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    प्रति (पूर्णाङ्क i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::pow((WT)src0[x + i], (WT)src1[x + i]));
                    अनुवर्त;
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
        प्रति (; x < width; x++)
            dst[x] = saturate_cast<T>(std::pow((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    प्रति (पूर्णाङ्क y = urows; y < height; y++)
        memcpy(dst + (size_t)y*dsty, dst, (size_t)width*परिमाण(T));
    प्रत्यागम 0;
}

TKernel getPowFunc_(पूर्णाङ्क T, पूर्णाङ्क R)
{
    यदि (R != T)
        प्रत्यागम {};
    KernelFunc fptr = nullptr;
    विकल्प (T)
    {
    पक्ष CV_16F:  fptr = powKernel<hfloat, v_float32>; भङ्ग;
    पक्ष CV_16BF: fptr = powKernel<bfloat, v_float32>; भङ्ग;
    पक्ष CV_32F:  fptr = powKernel<भिन्न,  v_float32>; भङ्ग;
#यदि CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    पक्ष CV_64F:  fptr = powKernel<द्विगुण, v_float64>; भङ्ग;
#अन्यथा
    पक्ष CV_64F:  fptr = scalarPowKernel<द्विगुण, द्विगुण>; भङ्ग;
#endif
    स्वभाव: ;
    }
    प्रत्यागम {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
template<वर्ग Op>
स्थिर KernelFunc mathByDepth(पूर्णाङ्क T)
{
    विकल्प (T)
    {
    पक्ष CV_16F:  प्रत्यागम vecUnaryKernel<hfloat, v_float32, Op>;
    पक्ष CV_16BF: प्रत्यागम vecUnaryKernel<bfloat, v_float32, Op>;
    पक्ष CV_32F:  प्रत्यागम vecUnaryKernel<भिन्न,  v_float32, Op>;
#यदि CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    पक्ष CV_64F:  प्रत्यागम vecUnaryKernel<द्विगुण, v_float64, Op>;
#अन्यथा
    पक्ष CV_64F:  प्रत्यागम scalarUnaryKernel<द्विगुण, द्विगुण, Op>;
#endif
    स्वभाव:      प्रत्यागम nullptr;
    }
}

TKernel getMathFunc_(शीर्ष op, पूर्णाङ्क T)
{
    KernelFunc f = nullptr;
    विकल्प (op)
    {
    पक्ष OP_SQRT: f = mathByDepth<MSqrt>(T); भङ्ग;
    पक्ष OP_EXP:  f = mathByDepth<MExp >(T); भङ्ग;
    पक्ष OP_LOG:  f = mathByDepth<MLog >(T); भङ्ग;
    पक्ष OP_SIN:  f = mathByDepth<MSin >(T); भङ्ग;
    पक्ष OP_COS:  f = mathByDepth<MCos >(T); भङ्ग;
    पक्ष OP_TANH: f = mathByDepth<MTanh>(T); भङ्ग;
    पक्ष OP_RELU: f = mathByDepth<MRelu>(T); भङ्ग;
    पक्ष OP_ERF:  f = mathByDepth<MErf >(T); भङ्ग;
    स्वभाव: ;
    }
    प्रत्यागम {f, nullptr, 0};
}


#endif // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
