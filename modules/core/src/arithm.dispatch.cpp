// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "arithm_expr.hpp"
#অন্তর্ভাব "arithm.simd.hpp"
#অন্তর্ভাব "arithm.simd_declarations.hpp"

নামাকাশ cv { নামাকাশ ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R, তার্কিক chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(শীর্ষ op, পূর্ণাঙ্ক T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(শীর্ষ op, পূর্ণাঙ্ক esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(পূর্ণাঙ্ক esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(পূর্ণাঙ্ক T, পূর্ণাঙ্ক R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(পূর্ণাঙ্ক mdepth, পূর্ণাঙ্ক T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(পূর্ণাঙ্ক T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
স্থির TKernel getCastFunc(পূর্ণাঙ্ক sd, পূর্ণাঙ্ক dd, তার্কিক scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(শীর্ষ op, পূর্ণাঙ্ক depth0, পূর্ণাঙ্ক depth1, পূর্ণাঙ্ক depth2, পূর্ণাঙ্ক rdepth)
{
    (শূন্য)depth2;

    যদি (op == OP_CAST)          প্রত্যাগম getCastFunc(depth0, rdepth, অসত্য);
    যদি (op == OP_CONVERT_SCALE) প্রত্যাগম getCastFunc(depth0, rdepth, সত্য);

    যদি (op == OP_ADD || op == OP_SUB)
    {
        যদি (depth0 != depth1) প্রত্যাগম {};   // operands must be the same type
        প্রত্যাগম op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    যদি (op == OP_ADDW)
    {
        যদি (depth0 != depth1) প্রত্যাগম {};
        প্রত্যাগম getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    যদি (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        যদি (depth0 != depth1) প্রত্যাগম {};
        যদি (op == OP_MUL) প্রত্যাগম getMulFunc(depth0, rdepth);
        যদি (op == OP_POW) প্রত্যাগম getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        ধ্রুব তার্কিক isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        প্রত্যাগম getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    যদি (op == OP_MIN || op == OP_MAX)
    {
        যদি (depth0 != depth1 || rdepth != depth0) প্রত্যাগম {};
        প্রত্যাগম op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    যদি (op == OP_ABSDIFF)
    {
        যদি (depth0 != depth1) প্রত্যাগম {};
        প্রত্যাগম getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    যদি (opCategory(op) == CAT_COMPARE)
    {
        যদি (depth0 != depth1 || rdepth != CV_8U) প্রত্যাগম {};
        প্রত্যাগম getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    যদি (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        যদি (depth0 != depth1 || rdepth != depth0) প্রত্যাগম {};
        প্রত্যাগম getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    যদি (op == OP_NOT)
    {
        যদি (rdepth != depth0) প্রত্যাগম {};
        প্রত্যাগম getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    যদি (op == OP_HYPOT || op == OP_ATAN2)
    {
        যদি (depth0 != depth1) প্রত্যাগম {};
        প্রত্যাগম op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    যদি (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        যদি (rdepth != depth0) প্রত্যাগম {};
        প্রত্যাগম getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    যদি (op == OP_SELECT)
    {
        যদি (depth1 != depth2 || rdepth != depth1) প্রত্যাগম {};
        প্রত্যাগম getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    যদি (op == OP_CLAMP)
    {
        যদি (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) প্রত্যাগম {};
        প্রত্যাগম getClampFunc(rdepth);
    }

    প্রত্যাগম {};
}

}} // namespace cv::ew

নামাকাশ cv { নামাকাশ hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
শূন্য mul8u(ধ্রুব uchar* src1, size_t step1, ধ্রুব uchar* src2, size_t step2,
           uchar* dst, size_t step, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height, শূন্য* scale)
{
    ধ্রুব দ্বিগুণ params[] = { scale ? *(ধ্রুব দ্বিগুণ*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
স্থির শূন্য bitwise8u(ew::শীর্ষ op, ধ্রুব uchar* src1, size_t step1, ধ্রুব uchar* src2, size_t step2,
                      uchar* dst, size_t step, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height)
{
    ধ্রুব দ্বিগুণ noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

শূন্য and8u(ধ্রুব uchar* src1, size_t step1, ধ্রুব uchar* src2, size_t step2,
           uchar* dst, size_t step, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height, শূন্য*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

শূন্য or8u(ধ্রুব uchar* src1, size_t step1, ধ্রুব uchar* src2, size_t step2,
          uchar* dst, size_t step, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height, শূন্য*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

শূন্য xor8u(ধ্রুব uchar* src1, size_t step1, ধ্রুব uchar* src2, size_t step2,
           uchar* dst, size_t step, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height, শূন্য*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

শূন্য not8u(ধ্রুব uchar* src1, size_t step1, ধ্রুব uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height, শূন্য*)
{
    ধ্রুব দ্বিগুণ noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
