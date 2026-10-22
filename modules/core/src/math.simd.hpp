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

#समावेश "opencv2/core/hal/intrin.hpp"
#समावेश "convert.hpp"          // typed vx_load_pair_as / v_store_pair_as helpers (cv::)
#समावेश "arithm_expr.hpp"      // the kernel contract: TOp / TKernel / KernelFunc
#समावेश <cmath>

namespace cv {

// Everything outside cv::ew::CV_CPU_OPTIMIZATION_NAMESPACE must be skipped in the
// declarations-only re-includes (one per dispatched mode), or it gets redefined.
#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY
#अगर (CV_SIMD || CV_SIMD_SCALABLE)

// f32-pair -> f16/bf16 stores for the half-float hub (convert.hpp covers the other pairs)
अटल अंतरभूत व्योम v_store_pair_as(hfloat* p, स्थिर v_float32& a, स्थिर v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}
अटल अंतरभूत व्योम v_store_pair_as(bfloat* p, स्थिर v_float32& a, स्थिर v_float32& b)
{
    v_pack_store(p, a);
    v_pack_store(p + VTraits<v_float32>::vlanes(), b);
}

#पूर्ण_अगर
#पूर्ण_अगर // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

namespace ew {
CV_CPU_OPTIMIZATION_NAMESPACE_BEGIN

// ---- per-op kernel entry points for THIS baseline (the regular dispatchers in
//      math.dispatch.cpp reach them through CV_CPU_DISPATCH). ----
TKernel getMathFunc_(शीर्ष op, पूर्णांक T);                 // unary math, T -> T, T in {f16, bf16, f32, f64}
TKernel getPowFunc_(पूर्णांक T, पूर्णांक R);                   // OP_POW, T x T -> T (R must equal T)

#अगर_अघोषित CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

// ===========================================================================
// Op functors: vec(Wvec) over the work vector (f32 or f64), scl(WT) for the scalar path/tail.
// New unary math ops slot in here.
// ===========================================================================
काष्ठा MSqrt {
    ढाँचा<typename V> अटल V vec(स्थिर V& x) { वापस v_sqrt(x); }
    ढाँचा<typename W> अटल W scl(W x) { वापस std::वर्ग_मूल(x); }
};
काष्ठा MExp {
    ढाँचा<typename V> अटल V vec(स्थिर V& x) { वापस v_exp(x); }
    ढाँचा<typename W> अटल W scl(W x) { वापस std::exp(x); }
};
काष्ठा MLog {
    ढाँचा<typename V> अटल V vec(स्थिर V& x) { वापस v_log(x); }
    ढाँचा<typename W> अटल W scl(W x) { वापस std::log(x); }
};
काष्ठा MSin {
    ढाँचा<typename V> अटल V vec(स्थिर V& x) { वापस v_sin(x); }
    ढाँचा<typename W> अटल W scl(W x) { वापस std::sin(x); }
};
काष्ठा MCos {
    ढाँचा<typename V> अटल V vec(स्थिर V& x) { वापस v_cos(x); }
    ढाँचा<typename W> अटल W scl(W x) { वापस std::cos(x); }
};
// tanh(x) = (e^2x - 1) / (e^2x + 1), on top of v_exp (no v_tanh intrinsic). The input is clamped
// first: tanh saturates to +/-1 well inside |x| <= 10 (f32) / 20 (f64), while an unclamped large x
// would push e^2x to inf and the ratio to inf/inf = NaN. (A NaN input may map to a saturated value
// on some ISAs instead of NaN - the polynomial v_exp has relaxed NaN semantics anyway.)
काष्ठा MTanh {
    अटल v_float32 vec(स्थिर v_float32& x)
    {
        स्थिर v_float32 one = vx_setall_f32(1.f), lim = vx_setall_f32(10.f);
        v_float32 cx = v_min(v_max(x, v_sub(vx_setzero_f32(), lim)), lim);
        v_float32 e = v_exp(v_add(cx, cx));
        वापस v_div(v_sub(e, one), v_add(e, one));
    }
#अगर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    अटल v_float64 vec(स्थिर v_float64& x)
    {
        स्थिर v_float64 one = vx_setall_f64(1.), lim = vx_setall_f64(20.);
        v_float64 cx = v_min(v_max(x, v_sub(vx_setzero_f64(), lim)), lim);
        v_float64 e = v_exp(v_add(cx, cx));
        वापस v_div(v_sub(e, one), v_add(e, one));
    }
#पूर्ण_अगर
    ढाँचा<typename W> अटल W scl(W x) { वापस std::tanh(x); }
};
काष्ठा MErf {
    अटल v_float32 vec(स्थिर v_float32& x) { वापस v_erf(x); }
#अगर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    // no f64 SIMD erf primitive: apply std::erf per lane (keeps the one kernel shape; the
    // store/load round-trip is noise next to libm erf itself)
    अटल v_float64 vec(स्थिर v_float64& x)
    {
        द्विगुन buf[VTraits<v_float64>::max_nlanes];
        v_store(buf, x);
        क्रम (पूर्णांक i = 0; i < VTraits<v_float64>::vlanes(); i++) buf[i] = std::erf(buf[i]);
        वापस vx_load(buf);
    }
#पूर्ण_अगर
    ढाँचा<typename W> अटल W scl(W x) { वापस std::erf(x); }
};
काष्ठा MRelu {
    ढाँचा<typename V> अटल V vec(स्थिर V& x) { वापस v_max(x, v_setzero_<V>()); }
    ढाँचा<typename W> अटल W scl(W x) { वापस x > W(0) ? x : W(0); }
};

// ===========================================================================
// The unary kernel: dst = Op(src), T -> T. Wvec picks the work vector: v_float32 / v_float64 for
// the native depths, v_float32 for the f16/bf16 hub (vx_load_pair_as does the widening).
// ===========================================================================
ढाँचा<typename T, typename Wvec, श्रेणी Op>
अटल पूर्णांक vecUnaryKernel(स्थिर व्योम* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                          स्थिर व्योम*, माप_प्रकार, माप_प्रकार, स्थिर व्योम*, माप_प्रकार, माप_प्रकार,
                          व्योम* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                          स्थिर द्विगुन*, पूर्णांक, व्योम*)
{
    s0y /= माप(T);
    dsty /= माप(T);
    CV_Assert(s0x <= 1u);

    स्थिर T* src0 = (स्थिर T*)src0_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    अगर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width) { width *= height; height = 1; }

    // vertical broadcast (a row expanded into a matrix, s0y == 0): every output row is identical -
    // compute the first one, memcpy the rest (transcendentals cost far more than a row copy)
    स्थिर पूर्णांक urows = (s0y == 0 && height > 1) ? 1 : height;

    क्रम (पूर्णांक y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        अगर (s0x == 0)                     // broadcast-scalar source: one value covers the row
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            क्रम (पूर्णांक x = 0; x < width; x++) dst[x] = v;
            जारी;
        }
        पूर्णांक x = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
        स्थिर पूर्णांक VECSZ = VTraits<Wvec>::vlanes();
        // in-place (dst == src) forbids the right-edge backoff: it would re-read already-written
        // values and apply Op twice. Those rows finish in the scalar tail instead.
        स्थिर bool use_पुच्छ_trick = width >= VECSZ*4 && src0_ != dst_;
        क्रम (; x < width; x += VECSZ*2)
        {
            अगर (x + VECSZ*2 > width) { अगर (!use_पुच्छ_trick || x == 0) अवरोध; x = width - VECSZ*2; }
            Wvec a0, a1;
            vx_load_pair_as(src0 + x, a0, a1);
            a0 = Op::vec(a0); a1 = Op::vec(a1);
            v_store_pair_as(dst + x, a0, a1);
        }
#पूर्ण_अगर
        क्रम (; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    क्रम (पूर्णांक y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*माप(T));
    वापस 0;
}

// Scalar-only variant for (op, depth) pairs with no SIMD primitive (erf on f64; every op's f64
// when the baseline has no 64-bit float SIMD).
ढाँचा<typename T, typename WT, श्रेणी Op>
अटल पूर्णांक scalarUnaryKernel(स्थिर व्योम* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                             स्थिर व्योम*, माप_प्रकार, माप_प्रकार, स्थिर व्योम*, माप_प्रकार, माप_प्रकार,
                             व्योम* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                             स्थिर द्विगुन*, पूर्णांक, व्योम*)
{
    s0y /= माप(T);
    dsty /= माप(T);
    CV_Assert(s0x <= 1u);
    स्थिर T* src0 = (स्थिर T*)src0_;
    T* dst = (T*)dst_;
    अगर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width) { width *= height; height = 1; }
    स्थिर पूर्णांक urows = (s0y == 0 && height > 1) ? 1 : height;   // vertical broadcast: 1 row + copies
    क्रम (पूर्णांक y = 0; y < urows; y++, src0 += s0y, dst += dsty)
    {
        अगर (s0x == 0)
        {
            T v = saturate_cast<T>(Op::scl((WT)src0[0]));
            क्रम (पूर्णांक x = 0; x < width; x++) dst[x] = v;
            जारी;
        }
        क्रम (पूर्णांक x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(Op::scl((WT)src0[x]));
    }
    dst = (T*)dst_;
    क्रम (पूर्णांक y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*माप(T));
    वापस 0;
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
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
अटल अंतरभूत v_float32 vxSetallW(भग्न v,  स्थिर v_float32&) { वापस vx_setall_f32(v); }
#अगर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
अटल अंतरभूत v_float64 vxSetallW(द्विगुन v, स्थिर v_float64&) { वापस vx_setall_f64(v); }
#पूर्ण_अगर
#पूर्ण_अगर

// Plain scalar pow for baselines without the needed SIMD float width (f64 without 64-bit SIMD).
ढाँचा<typename T, typename WT>
अटल पूर्णांक scalarPowKernel(स्थिर व्योम* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                           स्थिर व्योम* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                           स्थिर व्योम*, माप_प्रकार, माप_प्रकार,
                           व्योम* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                           स्थिर द्विगुन*, पूर्णांक, व्योम*)
{
    s0y /= माप(T); s1y /= माप(T); dsty /= माप(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);
    स्थिर T* src0 = (स्थिर T*)src0_;
    स्थिर T* src1 = (स्थिर T*)src1_;
    T* dst = (T*)dst_;
    अगर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width && s1y == s1x*(माप_प्रकार)width)
    { width *= height; height = 1; }
    क्रम (पूर्णांक y = 0; y < height; y++, src0 += s0y, src1 += s1y, dst += dsty)
        क्रम (पूर्णांक x = 0; x < width; x++)
            dst[x] = saturate_cast<T>(std::घात((WT)src0[x*s0x], (WT)src1[x*s1x]));
    वापस 0;
}

ढाँचा<typename T, typename Wvec>
अटल पूर्णांक powKernel(स्थिर व्योम* src0_, माप_प्रकार s0y, माप_प्रकार s0x,
                     स्थिर व्योम* src1_, माप_प्रकार s1y, माप_प्रकार s1x,
                     स्थिर व्योम*, माप_प्रकार, माप_प्रकार,
                     व्योम* dst_, माप_प्रकार dsty, पूर्णांक width, पूर्णांक height,
                     स्थिर द्विगुन*, पूर्णांक, व्योम*)
{
    s0y /= माप(T);
    s1y /= माप(T);
    dsty /= माप(T);
    CV_Assert(s0x <= 1u && s1x <= 1u);

    स्थिर T* src0 = (स्थिर T*)src0_;
    स्थिर T* src1 = (स्थिर T*)src1_;
    T* dst = (T*)dst_;
    using WT = typename VTraits<Wvec>::lane_type;

    अगर (height > 1 && dsty == (माप_प्रकार)width && s0y == s0x*(माप_प्रकार)width && s1y == s1x*(माप_प्रकार)width)
    { width *= height; height = 1; }

    [[maybe_unused]] स्थिर bool पुच्छ_trick = src0_ != dst_ && src1_ != dst_;

    // both operands vertically broadcast: every output row is identical - compute one, copy
    स्थिर पूर्णांक urows = (s0y == 0 && s1y == 0 && height > 1) ? 1 : height;

    क्रम (पूर्णांक y = 0; y < urows; y++, src0 += s0y, src1 += s1y, dst += dsty)
    {
        पूर्णांक x = 0;
        अगर (s1x == 0)                                   // scalar exponent for this row
        {
            स्थिर WT p = (WT)src1[0];
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
            स्थिर पूर्णांक VECSZ = VTraits<Wvec>::vlanes();
            अगर (s0x == 1)
            {
                अगर (p == WT(2) || p == WT(3))
                {
                    क्रम (; x < width; x += VECSZ*2)
                    {
                        अगर (x + VECSZ*2 > width) { अगर (!पुच्छ_trick || x == 0) अवरोध; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        Wvec r0 = v_mul(a0, a0), r1 = v_mul(a1, a1);
                        अगर (p == WT(3)) { r0 = v_mul(r0, a0); r1 = v_mul(r1, a1); }
                        v_store_pair_as(dst + x, r0, r1);
                    }
                }
                अन्यथा अगर (p == WT(0.5))
                {
                    क्रम (; x < width; x += VECSZ*2)
                    {
                        अगर (x + VECSZ*2 > width) { अगर (!पुच्छ_trick || x == 0) अवरोध; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_sqrt(a0); a1 = v_sqrt(a1);
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा अगर (p == WT(1))
                {
                    अगर ((स्थिर व्योम*)src0 != (स्थिर व्योम*)dst)
                        क्रम (; x < width; x++) dst[x] = src0[x];
                    x = width;
                }
                अन्यथा अगर (p == WT(0))
                {
                    स्थिर T one = saturate_cast<T>(1);
                    क्रम (; x < width; x++) dst[x] = one;
                }
                अन्यथा अगर (p == WT(-0.5))
                {
                    स्थिर Wvec one = vxSetallW(WT(1), Wvec());
                    क्रम (; x < width; x += VECSZ*2)
                    {
                        अगर (x + VECSZ*2 > width) { अगर (!पुच्छ_trick || x == 0) अवरोध; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        a0 = v_div(one, v_sqrt(a0)); a1 = v_div(one, v_sqrt(a1));
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा अगर (p == std::rint(p) && std::असल(p) <= WT(65536))
                {
                    // any other INTEGER exponent: LSB-first binary exponentiation - the same
                    // multiply chain (and order) as the classic iPow, fully vectorized. Also more
                    // accurate than exp(p*log x) (a few ulp vs ~2e-7 rel) and semantically exact
                    // on non-positive bases: the sign falls out of the multiplies, 0^negative
                    // divides to inf - no scalar patching needed.
                    स्थिर पूर्णांक ip = (पूर्णांक)p, ap = ip < 0 ? -ip : ip;   // ap >= 1 (0..3 handled above)
                    स्थिर Wvec one = vxSetallW(WT(1), Wvec());
                    क्रम (; x < width; x += VECSZ*2)
                    {
                        अगर (x + VECSZ*2 > width) { अगर (!पुच्छ_trick || x == 0) अवरोध; x = width - VECSZ*2; }
                        Wvec b0, b1;
                        vx_load_pair_as(src0 + x, b0, b1);
                        Wvec a0 = one, a1 = one;
                        क्रम (पूर्णांक q = ap; q > 1; q >>= 1)
                        {
                            अगर (q & 1) { a0 = v_mul(a0, b0); a1 = v_mul(a1, b1); }
                            b0 = v_mul(b0, b0); b1 = v_mul(b1, b1);
                        }
                        a0 = v_mul(a0, b0); a1 = v_mul(a1, b1);
                        अगर (ip < 0) { a0 = v_div(one, a0); a1 = v_div(one, a1); }
                        v_store_pair_as(dst + x, a0, a1);
                    }
                }
                अन्यथा                                    // general scalar exponent: exp(p * log(x))
                {
                    स्थिर Wvec vp = vxSetallW(p, Wvec()), z = v_setzero_<Wvec>();
                    क्रम (; x < width; x += VECSZ*2)
                    {
                        अगर (x + VECSZ*2 > width) { अगर (!पुच्छ_trick || x == 0) अवरोध; x = width - VECSZ*2; }
                        Wvec a0, a1;
                        vx_load_pair_as(src0 + x, a0, a1);
                        अगर (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                        {                               // exact std::pow for x <= 0 lanes
                            क्रम (पूर्णांक i = 0; i < VECSZ*2; i++)
                                dst[x + i] = saturate_cast<T>(std::घात((WT)src0[x + i], p));
                            जारी;
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
#पूर्ण_अगर
            क्रम (; x < width; x++)
                dst[x] = saturate_cast<T>(std::घात((WT)src0[x*s0x], p));
            जारी;
        }
        // per-element exponent
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
        अगर (s0x == 1)
        {
            स्थिर पूर्णांक VECSZ = VTraits<Wvec>::vlanes();
            स्थिर Wvec z = v_setzero_<Wvec>();
            क्रम (; x < width; x += VECSZ*2)
            {
                अगर (x + VECSZ*2 > width) { अगर (!पुच्छ_trick || x == 0) अवरोध; x = width - VECSZ*2; }
                Wvec a0, a1, b0, b1;
                vx_load_pair_as(src0 + x, a0, a1);
                vx_load_pair_as(src1 + x, b0, b1);
                अगर (v_check_any(v_or(v_le(a0, z), v_le(a1, z))))
                {
                    क्रम (पूर्णांक i = 0; i < VECSZ*2; i++)
                        dst[x + i] = saturate_cast<T>(std::घात((WT)src0[x + i], (WT)src1[x + i]));
                    जारी;
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
#पूर्ण_अगर
        क्रम (; x < width; x++)
            dst[x] = saturate_cast<T>(std::घात((WT)src0[x*s0x], (WT)src1[x]));
    }
    dst = (T*)dst_;
    क्रम (पूर्णांक y = urows; y < height; y++)
        स_नकल(dst + (माप_प्रकार)y*dsty, dst, (माप_प्रकार)width*माप(T));
    वापस 0;
}

TKernel getPowFunc_(पूर्णांक T, पूर्णांक R)
{
    अगर (R != T)
        वापस {};
    KernelFunc fptr = nullptr;
    चयन (T)
    {
    हाल CV_16F:  fptr = powKernel<hfloat, v_float32>; अवरोध;
    हाल CV_16BF: fptr = powKernel<bfloat, v_float32>; अवरोध;
    हाल CV_32F:  fptr = powKernel<भग्न,  v_float32>; अवरोध;
#अगर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    हाल CV_64F:  fptr = powKernel<द्विगुन, v_float64>; अवरोध;
#अन्यथा
    हाल CV_64F:  fptr = scalarPowKernel<द्विगुन, द्विगुन>; अवरोध;
#पूर्ण_अगर
    शेष: ;
    }
    वापस {fptr, nullptr, 0};
}

// ===========================================================================
// getters for THIS baseline
// ===========================================================================
ढाँचा<श्रेणी Op>
अटल KernelFunc mathByDepth(पूर्णांक T)
{
    चयन (T)
    {
    हाल CV_16F:  वापस vecUnaryKernel<hfloat, v_float32, Op>;
    हाल CV_16BF: वापस vecUnaryKernel<bfloat, v_float32, Op>;
    हाल CV_32F:  वापस vecUnaryKernel<भग्न,  v_float32, Op>;
#अगर CV_SIMD_64F || CV_SIMD_SCALABLE_64F
    हाल CV_64F:  वापस vecUnaryKernel<द्विगुन, v_float64, Op>;
#अन्यथा
    हाल CV_64F:  वापस scalarUnaryKernel<द्विगुन, द्विगुन, Op>;
#पूर्ण_अगर
    शेष:      वापस nullptr;
    }
}

TKernel getMathFunc_(शीर्ष op, पूर्णांक T)
{
    KernelFunc f = nullptr;
    चयन (op)
    {
    हाल OP_SQRT: f = mathByDepth<MSqrt>(T); अवरोध;
    हाल OP_EXP:  f = mathByDepth<MExp >(T); अवरोध;
    हाल OP_LOG:  f = mathByDepth<MLog >(T); अवरोध;
    हाल OP_SIN:  f = mathByDepth<MSin >(T); अवरोध;
    हाल OP_COS:  f = mathByDepth<MCos >(T); अवरोध;
    हाल OP_TANH: f = mathByDepth<MTanh>(T); अवरोध;
    हाल OP_RELU: f = mathByDepth<MRelu>(T); अवरोध;
    हाल OP_ERF:  f = mathByDepth<MErf >(T); अवरोध;
    शेष: ;
    }
    वापस {f, nullptr, 0};
}


#पूर्ण_अगर // CV_CPU_OPTIMIZATION_DECLARATIONS_ONLY

CV_CPU_OPTIMIZATION_NAMESPACE_END
}} // namespace cv::ew
