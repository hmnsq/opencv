// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#ضمن "precomp.hpp"
#ضمن "arithm_expr.hpp"
#ضمن "arithm.simd.hpp"
#ضمن "arithm.simd_declarations.hpp"

نطاق cv { نطاق ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(صحيح T, صحيح R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(صحيح T, صحيح R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(صحيح T, صحيح R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(صحيح T, صحيح R, منطقي chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(صحيح T, صحيح R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(صحيح T, صحيح R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(صحيح T, صحيح R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(صحيح T, صحيح R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(صحيح T, صحيح R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(قمة op, صحيح T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(قمة op, صحيح esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(صحيح esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(صحيح T, صحيح R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(صحيح mdepth, صحيح T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(صحيح T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
ساكن TKernel getCastFunc(صحيح sd, صحيح dd, منطقي scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(قمة op, صحيح depth0, صحيح depth1, صحيح depth2, صحيح rdepth)
{
    (فراغ)depth2;

    إذا (op == OP_CAST)          إرجاع getCastFunc(depth0, rdepth, خطأ);
    إذا (op == OP_CONVERT_SCALE) إرجاع getCastFunc(depth0, rdepth, true);

    إذا (op == OP_ADD || op == OP_SUB)
    {
        إذا (depth0 != depth1) إرجاع {};   // operands must be the same type
        إرجاع op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    إذا (op == OP_ADDW)
    {
        إذا (depth0 != depth1) إرجاع {};
        إرجاع getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    إذا (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        إذا (depth0 != depth1) إرجاع {};
        إذا (op == OP_MUL) إرجاع getMulFunc(depth0, rdepth);
        إذا (op == OP_POW) إرجاع getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        ثابت منطقي isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        إرجاع getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    إذا (op == OP_MIN || op == OP_MAX)
    {
        إذا (depth0 != depth1 || rdepth != depth0) إرجاع {};
        إرجاع op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    إذا (op == OP_ABSDIFF)
    {
        إذا (depth0 != depth1) إرجاع {};
        إرجاع getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    إذا (opCategory(op) == CAT_COMPARE)
    {
        إذا (depth0 != depth1 || rdepth != CV_8U) إرجاع {};
        إرجاع getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    إذا (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        إذا (depth0 != depth1 || rdepth != depth0) إرجاع {};
        إرجاع getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    إذا (op == OP_NOT)
    {
        إذا (rdepth != depth0) إرجاع {};
        إرجاع getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    إذا (op == OP_HYPOT || op == OP_ATAN2)
    {
        إذا (depth0 != depth1) إرجاع {};
        إرجاع op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    إذا (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        إذا (rdepth != depth0) إرجاع {};
        إرجاع getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    إذا (op == OP_SELECT)
    {
        إذا (depth1 != depth2 || rdepth != depth1) إرجاع {};
        إرجاع getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    إذا (op == OP_CLAMP)
    {
        إذا (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) إرجاع {};
        إرجاع getClampFunc(rdepth);
    }

    إرجاع {};
}

}} // namespace cv::ew

نطاق cv { نطاق hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
فراغ mul8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
           uchar* dst, size_t step, صحيح width, صحيح height, فراغ* scale)
{
    ثابت مزدوج params[] = { scale ? *(ثابت مزدوج*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
ساكن فراغ bitwise8u(ew::قمة op, ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
                      uchar* dst, size_t step, صحيح width, صحيح height)
{
    ثابت مزدوج noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

فراغ and8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
           uchar* dst, size_t step, صحيح width, صحيح height, فراغ*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

فراغ or8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
          uchar* dst, size_t step, صحيح width, صحيح height, فراغ*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

فراغ xor8u(ثابت uchar* src1, size_t step1, ثابت uchar* src2, size_t step2,
           uchar* dst, size_t step, صحيح width, صحيح height, فراغ*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

فراغ not8u(ثابت uchar* src1, size_t step1, ثابت uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, صحيح width, صحيح height, فراغ*)
{
    ثابت مزدوج noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
