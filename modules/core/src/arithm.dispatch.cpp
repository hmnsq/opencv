// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "arithm_expr.hpp"
#ಅನ್ತರ್ಭಾವ "arithm.simd.hpp"
#ಅನ್ತರ್ಭಾವ "arithm.simd_declarations.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R, ತಾರ್ಕಿಕ chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(ಪೂರ್ಣಾಙ್ಕ esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(ಪೂರ್ಣಾಙ್ಕ T, ಪೂರ್ಣಾಙ್ಕ R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(ಪೂರ್ಣಾಙ್ಕ mdepth, ಪೂರ್ಣಾಙ್ಕ T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(ಪೂರ್ಣಾಙ್ಕ T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
ಸ್ಥಿರ TKernel getCastFunc(ಪೂರ್ಣಾಙ್ಕ sd, ಪೂರ್ಣಾಙ್ಕ dd, ತಾರ್ಕಿಕ scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(ತುದಿ op, ಪೂರ್ಣಾಙ್ಕ depth0, ಪೂರ್ಣಾಙ್ಕ depth1, ಪೂರ್ಣಾಙ್ಕ depth2, ಪೂರ್ಣಾಙ್ಕ rdepth)
{
    (ಶೂನ್ಯ)depth2;

    ಯದಿ (op == OP_CAST)          ಪ್ರತ್ಯಾಗಮ getCastFunc(depth0, rdepth, ಅಸತ್ಯ);
    ಯದಿ (op == OP_CONVERT_SCALE) ಪ್ರತ್ಯಾಗಮ getCastFunc(depth0, rdepth, ಸತ್ಯ);

    ಯದಿ (op == OP_ADD || op == OP_SUB)
    {
        ಯದಿ (depth0 != depth1) ಪ್ರತ್ಯಾಗಮ {};   // operands must be the same type
        ಪ್ರತ್ಯಾಗಮ op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    ಯದಿ (op == OP_ADDW)
    {
        ಯದಿ (depth0 != depth1) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    ಯದಿ (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        ಯದಿ (depth0 != depth1) ಪ್ರತ್ಯಾಗಮ {};
        ಯದಿ (op == OP_MUL) ಪ್ರತ್ಯಾಗಮ getMulFunc(depth0, rdepth);
        ಯದಿ (op == OP_POW) ಪ್ರತ್ಯಾಗಮ getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        ಧ್ರುವ ತಾರ್ಕಿಕ isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        ಪ್ರತ್ಯಾಗಮ getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    ಯದಿ (op == OP_MIN || op == OP_MAX)
    {
        ಯದಿ (depth0 != depth1 || rdepth != depth0) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    ಯದಿ (op == OP_ABSDIFF)
    {
        ಯದಿ (depth0 != depth1) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    ಯದಿ (opCategory(op) == CAT_COMPARE)
    {
        ಯದಿ (depth0 != depth1 || rdepth != CV_8U) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    ಯದಿ (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        ಯದಿ (depth0 != depth1 || rdepth != depth0) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    ಯದಿ (op == OP_NOT)
    {
        ಯದಿ (rdepth != depth0) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    ಯದಿ (op == OP_HYPOT || op == OP_ATAN2)
    {
        ಯದಿ (depth0 != depth1) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    ಯದಿ (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        ಯದಿ (rdepth != depth0) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    ಯದಿ (op == OP_SELECT)
    {
        ಯದಿ (depth1 != depth2 || rdepth != depth1) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    ಯದಿ (op == OP_CLAMP)
    {
        ಯದಿ (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) ಪ್ರತ್ಯಾಗಮ {};
        ಪ್ರತ್ಯಾಗಮ getClampFunc(rdepth);
    }

    ಪ್ರತ್ಯಾಗಮ {};
}

}} // namespace cv::ew

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
ಶೂನ್ಯ mul8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
           uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಶೂನ್ಯ* scale)
{
    ಧ್ರುವ ದ್ವಿಗುಣ params[] = { scale ? *(ಧ್ರುವ ದ್ವಿಗುಣ*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
ಸ್ಥಿರ ಶೂನ್ಯ bitwise8u(ew::ತುದಿ op, ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
                      uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height)
{
    ಧ್ರುವ ದ್ವಿಗುಣ noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

ಶೂನ್ಯ and8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
           uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಶೂನ್ಯ*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

ಶೂನ್ಯ or8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
          uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಶೂನ್ಯ*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

ಶೂನ್ಯ xor8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* src2, size_t step2,
           uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಶೂನ್ಯ*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

ಶೂನ್ಯ not8u(ಧ್ರುವ uchar* src1, size_t step1, ಧ್ರುವ uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, ಶೂನ್ಯ*)
{
    ಧ್ರುವ ದ್ವಿಗುಣ noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
