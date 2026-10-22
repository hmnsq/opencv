// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "arithm_expr.hpp"
#அந்தர்பாவ "arithm.simd.hpp"
#அந்தர்பாவ "arithm.simd_declarations.hpp"

நாமாகாஶ cv { நாமாகாஶ ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(முழு T, முழு R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(முழு T, முழு R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(முழு T, முழு R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(முழு T, முழு R, தருக்க chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(முழு T, முழு R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(முழு T, முழு R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(முழு T, முழு R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(முழு T, முழு R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(முழு T, முழு R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(உச்சி op, முழு T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(உச்சி op, முழு esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(முழு esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(முழு T, முழு R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(முழு mdepth, முழு T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(முழு T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
நிலையான TKernel getCastFunc(முழு sd, முழு dd, தருக்க scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(உச்சி op, முழு depth0, முழு depth1, முழு depth2, முழு rdepth)
{
    (வெறுமை)depth2;

    எனில் (op == OP_CAST)          திரும்பு getCastFunc(depth0, rdepth, பொய்);
    எனில் (op == OP_CONVERT_SCALE) திரும்பு getCastFunc(depth0, rdepth, உண்மை);

    எனில் (op == OP_ADD || op == OP_SUB)
    {
        எனில் (depth0 != depth1) திரும்பு {};   // operands must be the same type
        திரும்பு op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    எனில் (op == OP_ADDW)
    {
        எனில் (depth0 != depth1) திரும்பு {};
        திரும்பு getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    எனில் (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        எனில் (depth0 != depth1) திரும்பு {};
        எனில் (op == OP_MUL) திரும்பு getMulFunc(depth0, rdepth);
        எனில் (op == OP_POW) திரும்பு getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        மாறிலி தருக்க isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        திரும்பு getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    எனில் (op == OP_MIN || op == OP_MAX)
    {
        எனில் (depth0 != depth1 || rdepth != depth0) திரும்பு {};
        திரும்பு op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    எனில் (op == OP_ABSDIFF)
    {
        எனில் (depth0 != depth1) திரும்பு {};
        திரும்பு getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    எனில் (opCategory(op) == CAT_COMPARE)
    {
        எனில் (depth0 != depth1 || rdepth != CV_8U) திரும்பு {};
        திரும்பு getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    எனில் (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        எனில் (depth0 != depth1 || rdepth != depth0) திரும்பு {};
        திரும்பு getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    எனில் (op == OP_NOT)
    {
        எனில் (rdepth != depth0) திரும்பு {};
        திரும்பு getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    எனில் (op == OP_HYPOT || op == OP_ATAN2)
    {
        எனில் (depth0 != depth1) திரும்பு {};
        திரும்பு op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    எனில் (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        எனில் (rdepth != depth0) திரும்பு {};
        திரும்பு getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    எனில் (op == OP_SELECT)
    {
        எனில் (depth1 != depth2 || rdepth != depth1) திரும்பு {};
        திரும்பு getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    எனில் (op == OP_CLAMP)
    {
        எனில் (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) திரும்பு {};
        திரும்பு getClampFunc(rdepth);
    }

    திரும்பு {};
}

}} // namespace cv::ew

நாமாகாஶ cv { நாமாகாஶ hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
வெறுமை mul8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
           uchar* dst, size_t step, முழு width, முழு height, வெறுமை* scale)
{
    மாறிலி இரட்டை params[] = { scale ? *(மாறிலி இரட்டை*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
நிலையான வெறுமை bitwise8u(ew::உச்சி op, மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
                      uchar* dst, size_t step, முழு width, முழு height)
{
    மாறிலி இரட்டை noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

வெறுமை and8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
           uchar* dst, size_t step, முழு width, முழு height, வெறுமை*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

வெறுமை or8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
          uchar* dst, size_t step, முழு width, முழு height, வெறுமை*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

வெறுமை xor8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* src2, size_t step2,
           uchar* dst, size_t step, முழு width, முழு height, வெறுமை*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

வெறுமை not8u(மாறிலி uchar* src1, size_t step1, மாறிலி uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, முழு width, முழு height, வெறுமை*)
{
    மாறிலி இரட்டை noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
