// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "arithm_expr.hpp"
#ଅନ୍ତର୍ଭାଵ "arithm.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "arithm.simd_declarations.hpp"

ନାମାକାଶ cv { ନାମାକାଶ ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R, ତାର୍କିକ chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T, ପୂର୍ଣ୍ଣସଂଖ୍ୟା R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା mdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
ସ୍ଥିର TKernel getCastFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dd, ତାର୍କିକ scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(ଶୀର୍ଷ op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rdepth)
{
    (ଶୂନ୍ୟ)depth2;

    ଯଦି (op == OP_CAST)          ଫେରନ୍ତୁ getCastFunc(depth0, rdepth, ମିଥ୍ୟା);
    ଯଦି (op == OP_CONVERT_SCALE) ଫେରନ୍ତୁ getCastFunc(depth0, rdepth, ସତ୍ୟ);

    ଯଦି (op == OP_ADD || op == OP_SUB)
    {
        ଯଦି (depth0 != depth1) ଫେରନ୍ତୁ {};   // operands must be the same type
        ଫେରନ୍ତୁ op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    ଯଦି (op == OP_ADDW)
    {
        ଯଦି (depth0 != depth1) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    ଯଦି (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        ଯଦି (depth0 != depth1) ଫେରନ୍ତୁ {};
        ଯଦି (op == OP_MUL) ଫେରନ୍ତୁ getMulFunc(depth0, rdepth);
        ଯଦି (op == OP_POW) ଫେରନ୍ତୁ getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        ଅଚଳ ତାର୍କିକ isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        ଫେରନ୍ତୁ getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    ଯଦି (op == OP_MIN || op == OP_MAX)
    {
        ଯଦି (depth0 != depth1 || rdepth != depth0) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    ଯଦି (op == OP_ABSDIFF)
    {
        ଯଦି (depth0 != depth1) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    ଯଦି (opCategory(op) == CAT_COMPARE)
    {
        ଯଦି (depth0 != depth1 || rdepth != CV_8U) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    ଯଦି (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        ଯଦି (depth0 != depth1 || rdepth != depth0) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    ଯଦି (op == OP_NOT)
    {
        ଯଦି (rdepth != depth0) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    ଯଦି (op == OP_HYPOT || op == OP_ATAN2)
    {
        ଯଦି (depth0 != depth1) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    ଯଦି (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        ଯଦି (rdepth != depth0) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    ଯଦି (op == OP_SELECT)
    {
        ଯଦି (depth1 != depth2 || rdepth != depth1) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    ଯଦି (op == OP_CLAMP)
    {
        ଯଦି (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) ଫେରନ୍ତୁ {};
        ଫେରନ୍ତୁ getClampFunc(rdepth);
    }

    ଫେରନ୍ତୁ {};
}

}} // namespace cv::ew

ନାମାକାଶ cv { ନାମାକାଶ hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
ଶୂନ୍ୟ mul8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
           uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଶୂନ୍ୟ* scale)
{
    ଅଚଳ ଦ୍ୱିଗୁଣ params[] = { scale ? *(ଅଚଳ ଦ୍ୱିଗୁଣ*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
ସ୍ଥିର ଶୂନ୍ୟ bitwise8u(ew::ଶୀର୍ଷ op, ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
                      uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height)
{
    ଅଚଳ ଦ୍ୱିଗୁଣ noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

ଶୂନ୍ୟ and8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
           uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଶୂନ୍ୟ*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

ଶୂନ୍ୟ or8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
          uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଶୂନ୍ୟ*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

ଶୂନ୍ୟ xor8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* src2, size_t step2,
           uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଶୂନ୍ୟ*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

ଶୂନ୍ୟ not8u(ଅଚଳ uchar* src1, size_t step1, ଅଚଳ uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, ଶୂନ୍ୟ*)
{
    ଅଚଳ ଦ୍ୱିଗୁଣ noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
