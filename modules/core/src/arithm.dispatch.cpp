// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "arithm_expr.hpp"
#അന്തര്ഭാവ "arithm.simd.hpp"
#അന്തര്ഭാവ "arithm.simd_declarations.hpp"

നാമാകാശ cv { നാമാകാശ ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R, തർക്കം chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(മുകൾ op, പൂർണ്ണസംഖ്യ T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(മുകൾ op, പൂർണ്ണസംഖ്യ esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(പൂർണ്ണസംഖ്യ esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(പൂർണ്ണസംഖ്യ T, പൂർണ്ണസംഖ്യ R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(പൂർണ്ണസംഖ്യ mdepth, പൂർണ്ണസംഖ്യ T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(പൂർണ്ണസംഖ്യ T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
സ്ഥിതം TKernel getCastFunc(പൂർണ്ണസംഖ്യ sd, പൂർണ്ണസംഖ്യ dd, തർക്കം scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(മുകൾ op, പൂർണ്ണസംഖ്യ depth0, പൂർണ്ണസംഖ്യ depth1, പൂർണ്ണസംഖ്യ depth2, പൂർണ്ണസംഖ്യ rdepth)
{
    (ശൂന്യം)depth2;

    എങ്കിൽ (op == OP_CAST)          തിരികെ getCastFunc(depth0, rdepth, അസത്യം);
    എങ്കിൽ (op == OP_CONVERT_SCALE) തിരികെ getCastFunc(depth0, rdepth, സത്യം);

    എങ്കിൽ (op == OP_ADD || op == OP_SUB)
    {
        എങ്കിൽ (depth0 != depth1) തിരികെ {};   // operands must be the same type
        തിരികെ op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    എങ്കിൽ (op == OP_ADDW)
    {
        എങ്കിൽ (depth0 != depth1) തിരികെ {};
        തിരികെ getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    എങ്കിൽ (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        എങ്കിൽ (depth0 != depth1) തിരികെ {};
        എങ്കിൽ (op == OP_MUL) തിരികെ getMulFunc(depth0, rdepth);
        എങ്കിൽ (op == OP_POW) തിരികെ getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        സ്ഥിരം തർക്കം isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        തിരികെ getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    എങ്കിൽ (op == OP_MIN || op == OP_MAX)
    {
        എങ്കിൽ (depth0 != depth1 || rdepth != depth0) തിരികെ {};
        തിരികെ op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    എങ്കിൽ (op == OP_ABSDIFF)
    {
        എങ്കിൽ (depth0 != depth1) തിരികെ {};
        തിരികെ getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    എങ്കിൽ (opCategory(op) == CAT_COMPARE)
    {
        എങ്കിൽ (depth0 != depth1 || rdepth != CV_8U) തിരികെ {};
        തിരികെ getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    എങ്കിൽ (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        എങ്കിൽ (depth0 != depth1 || rdepth != depth0) തിരികെ {};
        തിരികെ getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    എങ്കിൽ (op == OP_NOT)
    {
        എങ്കിൽ (rdepth != depth0) തിരികെ {};
        തിരികെ getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    എങ്കിൽ (op == OP_HYPOT || op == OP_ATAN2)
    {
        എങ്കിൽ (depth0 != depth1) തിരികെ {};
        തിരികെ op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    എങ്കിൽ (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        എങ്കിൽ (rdepth != depth0) തിരികെ {};
        തിരികെ getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    എങ്കിൽ (op == OP_SELECT)
    {
        എങ്കിൽ (depth1 != depth2 || rdepth != depth1) തിരികെ {};
        തിരികെ getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    എങ്കിൽ (op == OP_CLAMP)
    {
        എങ്കിൽ (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) തിരികെ {};
        തിരികെ getClampFunc(rdepth);
    }

    തിരികെ {};
}

}} // namespace cv::ew

നാമാകാശ cv { നാമാകാശ hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
ശൂന്യം mul8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
           uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ശൂന്യം* scale)
{
    സ്ഥിരം ഇരട്ട params[] = { scale ? *(സ്ഥിരം ഇരട്ട*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
സ്ഥിതം ശൂന്യം bitwise8u(ew::മുകൾ op, സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
                      uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height)
{
    സ്ഥിരം ഇരട്ട noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

ശൂന്യം and8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
           uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ശൂന്യം*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

ശൂന്യം or8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
          uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ശൂന്യം*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

ശൂന്യം xor8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* src2, size_t step2,
           uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ശൂന്യം*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

ശൂന്യം not8u(സ്ഥിരം uchar* src1, size_t step1, സ്ഥിരം uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, ശൂന്യം*)
{
    സ്ഥിരം ഇരട്ട noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
