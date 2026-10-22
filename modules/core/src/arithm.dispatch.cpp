// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#شامل "precomp.hpp"
#شامل "arithm_expr.hpp"
#شامل "arithm.simd.hpp"
#شامل "arithm.simd_declarations.hpp"

فضاینام cv { فضاینام ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(صحیح T, صحیح R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(صحیح T, صحیح R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(صحیح T, صحیح R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(صحیح T, صحیح R, منطقی chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(صحیح T, صحیح R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(صحیح T, صحیح R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(صحیح T, صحیح R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(صحیح T, صحیح R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(صحیح T, صحیح R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(بالاترین op, صحیح T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(بالاترین op, صحیح esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(صحیح esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(صحیح T, صحیح R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(صحیح mdepth, صحیح T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(صحیح T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
ایستا TKernel getCastFunc(صحیح sd, صحیح dd, منطقی scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(بالاترین op, صحیح depth0, صحیح depth1, صحیح depth2, صحیح rdepth)
{
    (تهی)depth2;

    اگر (op == OP_CAST)          بازگشت getCastFunc(depth0, rdepth, نادرست);
    اگر (op == OP_CONVERT_SCALE) بازگشت getCastFunc(depth0, rdepth, درست);

    اگر (op == OP_ADD || op == OP_SUB)
    {
        اگر (depth0 != depth1) بازگشت {};   // operands must be the same type
        بازگشت op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    اگر (op == OP_ADDW)
    {
        اگر (depth0 != depth1) بازگشت {};
        بازگشت getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    اگر (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        اگر (depth0 != depth1) بازگشت {};
        اگر (op == OP_MUL) بازگشت getMulFunc(depth0, rdepth);
        اگر (op == OP_POW) بازگشت getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        ثابت منطقی isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        بازگشت getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    اگر (op == OP_MIN || op == OP_MAX)
    {
        اگر (depth0 != depth1 || rdepth != depth0) بازگشت {};
        بازگشت op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    اگر (op == OP_ABSDIFF)
    {
        اگر (depth0 != depth1) بازگشت {};
        بازگشت getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    اگر (opCategory(op) == CAT_COMPARE)
    {
        اگر (depth0 != depth1 || rdepth != CV_8U) بازگشت {};
        بازگشت getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    اگر (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        اگر (depth0 != depth1 || rdepth != depth0) بازگشت {};
        بازگشت getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    اگر (op == OP_NOT)
    {
        اگر (rdepth != depth0) بازگشت {};
        بازگشت getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    اگر (op == OP_HYPOT || op == OP_ATAN2)
    {
        اگر (depth0 != depth1) بازگشت {};
        بازگشت op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    اگر (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        اگر (rdepth != depth0) بازگشت {};
        بازگشت getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    اگر (op == OP_SELECT)
    {
        اگر (depth1 != depth2 || rdepth != depth1) بازگشت {};
        بازگشت getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    اگر (op == OP_CLAMP)
    {
        اگر (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) بازگشت {};
        بازگشت getClampFunc(rdepth);
    }

    بازگشت {};
}

}} // namespace cv::ew

فضاینام cv { فضاینام hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
تهی mul8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
           uchar* dst, size_t step, صحیح width, صحیح height, تهی* scale)
{
    ثابت دوگانه params[] = { scale ? *(ثابت دوگانه*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
ایستا تهی bitwise8u(ew::بالاترین op, ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                      uchar* dst, size_t step, صحیح width, صحیح height)
{
    ثابت دوگانه noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

تهی and8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
           uchar* dst, size_t step, صحیح width, صحیح height, تهی*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

تهی or8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
          uchar* dst, size_t step, صحیح width, صحیح height, تهی*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

تهی xor8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
           uchar* dst, size_t step, صحیح width, صحیح height, تهی*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

تهی not8u(ثابت uchar* src1, size_t step1, ثابت uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, صحیح width, صحیح height, تهی*)
{
    ثابت دوگانه noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
