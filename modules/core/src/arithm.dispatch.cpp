// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm_expr.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "arithm.simd_declarations.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel get_ਜੋੜੋ_Func(ਪੂਰਨ T, ਪੂਰਨ R)            { CV_CPU_DISPATCH(get_ਜੋੜੋ_Func_2,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(ਪੂਰਨ T, ਪੂਰਨ R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(ਪੂਰਨ T, ਪੂਰਨ R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(ਪੂਰਨ T, ਪੂਰਨ R, ਤਾਰਕਿਕ chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(ਪੂਰਨ T, ਪੂਰਨ R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(ਪੂਰਨ T, ਪੂਰਨ R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(ਪੂਰਨ T, ਪੂਰਨ R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(ਪੂਰਨ T, ਪੂਰਨ R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(ਪੂਰਨ T, ਪੂਰਨ R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(ਸਿਖਰ op, ਪੂਰਨ T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(ਸਿਖਰ op, ਪੂਰਨ esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(ਪੂਰਨ esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel get_ਜੋੜੋ_Weighted_Func(ਪੂਰਨ T, ਪੂਰਨ R)    { CV_CPU_DISPATCH(get_ਜੋੜੋ_Weighted_Func_2, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(ਪੂਰਨ mdepth, ਪੂਰਨ T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(ਪੂਰਨ T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
ਸਥਿਰ TKernel getCastFunc(ਪੂਰਨ sd, ਪੂਰਨ dd, ਤਾਰਕਿਕ scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(ਸਿਖਰ op, ਪੂਰਨ depth0, ਪੂਰਨ depth1, ਪੂਰਨ depth2, ਪੂਰਨ rdepth)
{
    (ਖਾਲੀ)depth2;

    ਜੇ (op == OP_CAST)          ਵਾਪਸ getCastFunc(depth0, rdepth, ਝੂਠ);
    ਜੇ (op == OP_CONVERT_SCALE) ਵਾਪਸ getCastFunc(depth0, rdepth, ਸੱਚ);

    ਜੇ (op == OP_ਜੋੜੋ || op == OP_SUB)
    {
        ਜੇ (depth0 != depth1) ਵਾਪਸ {};   // operands must be the same type
        ਵਾਪਸ op == OP_ਜੋੜੋ ? get_ਜੋੜੋ_Func(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    ਜੇ (op == OP_ADDW)
    {
        ਜੇ (depth0 != depth1) ਵਾਪਸ {};
        ਵਾਪਸ get_ਜੋੜੋ_Weighted_Func(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    ਜੇ (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        ਜੇ (depth0 != depth1) ਵਾਪਸ {};
        ਜੇ (op == OP_MUL) ਵਾਪਸ getMulFunc(depth0, rdepth);
        ਜੇ (op == OP_POW) ਵਾਪਸ getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        ਅਚਲ ਤਾਰਕਿਕ isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        ਵਾਪਸ getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    ਜੇ (op == OP_MIN || op == OP_MAX)
    {
        ਜੇ (depth0 != depth1 || rdepth != depth0) ਵਾਪਸ {};
        ਵਾਪਸ op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    ਜੇ (op == OP_ABSDIFF)
    {
        ਜੇ (depth0 != depth1) ਵਾਪਸ {};
        ਵਾਪਸ getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    ਜੇ (opCategory(op) == CAT_COMPARE)
    {
        ਜੇ (depth0 != depth1 || rdepth != CV_8U) ਵਾਪਸ {};
        ਵਾਪਸ getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    ਜੇ (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        ਜੇ (depth0 != depth1 || rdepth != depth0) ਵਾਪਸ {};
        ਵਾਪਸ getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    ਜੇ (op == OP_NOT)
    {
        ਜੇ (rdepth != depth0) ਵਾਪਸ {};
        ਵਾਪਸ getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    ਜੇ (op == OP_HYPOT || op == OP_ATAN2)
    {
        ਜੇ (depth0 != depth1) ਵਾਪਸ {};
        ਵਾਪਸ op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    ਜੇ (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        ਜੇ (rdepth != depth0) ਵਾਪਸ {};
        ਵਾਪਸ getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    ਜੇ (op == OP_SELECT)
    {
        ਜੇ (depth1 != depth2 || rdepth != depth1) ਵਾਪਸ {};
        ਵਾਪਸ getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    ਜੇ (op == OP_CLAMP)
    {
        ਜੇ (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) ਵਾਪਸ {};
        ਵਾਪਸ getClampFunc(rdepth);
    }

    ਵਾਪਸ {};
}

}} // namespace cv::ew

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
ਖਾਲੀ mul8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
           uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਖਾਲੀ* scale)
{
    ਅਚਲ ਦੁੱਗਣਾ params[] = { scale ? *(ਅਚਲ ਦੁੱਗਣਾ*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, ਖਾਲੀ_ਸੂਚਕ, 0, 0, dst, ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
ਸਥਿਰ ਖਾਲੀ bitwise8u(ew::ਸਿਖਰ op, ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
                      uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ)
{
    ਅਚਲ ਦੁੱਗਣਾ noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != ਖਾਲੀ_ਸੂਚਕ);
    k.fptr(src1, step1, 1, src2, step2, 1, ਖਾਲੀ_ਸੂਚਕ, 0, 0, dst, ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ,
           noparams, k.flags, k.userdata);
}

ਖਾਲੀ and8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
           uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਖਾਲੀ*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ); }

ਖਾਲੀ or8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
          uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਖਾਲੀ*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ); }

ਖਾਲੀ xor8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* src2, size_t step2,
           uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਖਾਲੀ*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ); }

ਖਾਲੀ not8u(ਅਚਲ uchar* src1, size_t step1, ਅਚਲ uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t ਕਦਮ, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, ਖਾਲੀ*)
{
    ਅਚਲ ਦੁੱਗਣਾ noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != ਖਾਲੀ_ਸੂਚਕ);
    k.fptr(src1, step1, 1, ਖਾਲੀ_ਸੂਚਕ, 0, 0, ਖਾਲੀ_ਸੂਚਕ, 0, 0, dst, ਕਦਮ, ਚੌੜਾਈ, ਉਚਾਈ,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
