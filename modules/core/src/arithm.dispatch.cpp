// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "arithm_expr.hpp"
#अन्तर्भाव "arithm.simd.hpp"
#अन्तर्भाव "arithm.simd_declarations.hpp"

नामाकाश cv { नामाकाश ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(पूर्णाङ्क T, पूर्णाङ्क R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(पूर्णाङ्क T, पूर्णाङ्क R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(पूर्णाङ्क T, पूर्णाङ्क R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(पूर्णाङ्क T, पूर्णाङ्क R, तार्किक chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(पूर्णाङ्क T, पूर्णाङ्क R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(पूर्णाङ्क T, पूर्णाङ्क R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(पूर्णाङ्क T, पूर्णाङ्क R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(पूर्णाङ्क T, पूर्णाङ्क R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(पूर्णाङ्क T, पूर्णाङ्क R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(शीर्ष op, पूर्णाङ्क T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(शीर्ष op, पूर्णाङ्क esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(पूर्णाङ्क esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(पूर्णाङ्क T, पूर्णाङ्क R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(पूर्णाङ्क mdepth, पूर्णाङ्क T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(पूर्णाङ्क T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
स्थिर TKernel getCastFunc(पूर्णाङ्क sd, पूर्णाङ्क dd, तार्किक scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(शीर्ष op, पूर्णाङ्क depth0, पूर्णाङ्क depth1, पूर्णाङ्क depth2, पूर्णाङ्क rdepth)
{
    (शून्य)depth2;

    यदि (op == OP_CAST)          फर्काऊ getCastFunc(depth0, rdepth, असत्य);
    यदि (op == OP_CONVERT_SCALE) फर्काऊ getCastFunc(depth0, rdepth, सत्य);

    यदि (op == OP_ADD || op == OP_SUB)
    {
        यदि (depth0 != depth1) फर्काऊ {};   // operands must be the same type
        फर्काऊ op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    यदि (op == OP_ADDW)
    {
        यदि (depth0 != depth1) फर्काऊ {};
        फर्काऊ getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    यदि (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        यदि (depth0 != depth1) फर्काऊ {};
        यदि (op == OP_MUL) फर्काऊ getMulFunc(depth0, rdepth);
        यदि (op == OP_POW) फर्काऊ getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        अचर तार्किक isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        फर्काऊ getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    यदि (op == OP_MIN || op == OP_MAX)
    {
        यदि (depth0 != depth1 || rdepth != depth0) फर्काऊ {};
        फर्काऊ op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    यदि (op == OP_ABSDIFF)
    {
        यदि (depth0 != depth1) फर्काऊ {};
        फर्काऊ getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    यदि (opCategory(op) == CAT_COMPARE)
    {
        यदि (depth0 != depth1 || rdepth != CV_8U) फर्काऊ {};
        फर्काऊ getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    यदि (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        यदि (depth0 != depth1 || rdepth != depth0) फर्काऊ {};
        फर्काऊ getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    यदि (op == OP_NOT)
    {
        यदि (rdepth != depth0) फर्काऊ {};
        फर्काऊ getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    यदि (op == OP_HYPOT || op == OP_ATAN2)
    {
        यदि (depth0 != depth1) फर्काऊ {};
        फर्काऊ op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    यदि (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        यदि (rdepth != depth0) फर्काऊ {};
        फर्काऊ getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    यदि (op == OP_SELECT)
    {
        यदि (depth1 != depth2 || rdepth != depth1) फर्काऊ {};
        फर्काऊ getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    यदि (op == OP_CLAMP)
    {
        यदि (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) फर्काऊ {};
        फर्काऊ getClampFunc(rdepth);
    }

    फर्काऊ {};
}

}} // namespace cv::ew

नामाकाश cv { नामाकाश hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
शून्य mul8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, शून्य* scale)
{
    अचर द्विगुण params[] = { scale ? *(अचर द्विगुण*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
स्थिर शून्य bitwise8u(ew::शीर्ष op, अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
                      uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height)
{
    अचर द्विगुण noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

शून्य and8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, शून्य*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

शून्य or8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
          uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, शून्य*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

शून्य xor8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, शून्य*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

शून्य not8u(अचर uchar* src1, माप_प्रकार step1, अचर uchar* /*src2*/, माप_प्रकार /*step2*/,
           uchar* dst, माप_प्रकार step, पूर्णाङ्क width, पूर्णाङ्क height, शून्य*)
{
    अचर द्विगुण noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
