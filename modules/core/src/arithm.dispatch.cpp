// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#समावेश "precomp.hpp"
#समावेश "arithm_expr.hpp"
#समावेश "arithm.simd.hpp"
#समावेश "arithm.simd_declarations.hpp"

namespace cv { namespace ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(पूर्णांक T, पूर्णांक R, bool chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(पूर्णांक T, पूर्णांक R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(पूर्णांक T, पूर्णांक R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(पूर्णांक T, पूर्णांक R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(शीर्ष op, पूर्णांक T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(शीर्ष op, पूर्णांक esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(पूर्णांक esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(पूर्णांक T, पूर्णांक R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(पूर्णांक mdepth, पूर्णांक T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(पूर्णांक T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
अटल TKernel getCastFunc(पूर्णांक sd, पूर्णांक dd, bool scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(शीर्ष op, पूर्णांक depth0, पूर्णांक depth1, पूर्णांक depth2, पूर्णांक rdepth)
{
    (व्योम)depth2;

    अगर (op == OP_CAST)          वापस getCastFunc(depth0, rdepth, false);
    अगर (op == OP_CONVERT_SCALE) वापस getCastFunc(depth0, rdepth, true);

    अगर (op == OP_ADD || op == OP_SUB)
    {
        अगर (depth0 != depth1) वापस {};   // operands must be the same type
        वापस op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    अगर (op == OP_ADDW)
    {
        अगर (depth0 != depth1) वापस {};
        वापस getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    अगर (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        अगर (depth0 != depth1) वापस {};
        अगर (op == OP_MUL) वापस getMulFunc(depth0, rdepth);
        अगर (op == OP_POW) वापस getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        स्थिर bool isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        वापस getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    अगर (op == OP_MIN || op == OP_MAX)
    {
        अगर (depth0 != depth1 || rdepth != depth0) वापस {};
        वापस op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    अगर (op == OP_ABSDIFF)
    {
        अगर (depth0 != depth1) वापस {};
        वापस getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    अगर (opCategory(op) == CAT_COMPARE)
    {
        अगर (depth0 != depth1 || rdepth != CV_8U) वापस {};
        वापस getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    अगर (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        अगर (depth0 != depth1 || rdepth != depth0) वापस {};
        वापस getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    अगर (op == OP_NOT)
    {
        अगर (rdepth != depth0) वापस {};
        वापस getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    अगर (op == OP_HYPOT || op == OP_ATAN2)
    {
        अगर (depth0 != depth1) वापस {};
        वापस op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    अगर (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        अगर (rdepth != depth0) वापस {};
        वापस getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    अगर (op == OP_SELECT)
    {
        अगर (depth1 != depth2 || rdepth != depth1) वापस {};
        वापस getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    अगर (op == OP_CLAMP)
    {
        अगर (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) वापस {};
        वापस getClampFunc(rdepth);
    }

    वापस {};
}

}} // namespace cv::ew

namespace cv { namespace hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
व्योम mul8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, व्योम* scale)
{
    स्थिर द्विगुन params[] = { scale ? *(स्थिर द्विगुन*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
अटल व्योम bitwise8u(ew::शीर्ष op, स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
                      uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    स्थिर द्विगुन noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

व्योम and8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, व्योम*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

व्योम or8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
          uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, व्योम*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

व्योम xor8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, व्योम*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

व्योम not8u(स्थिर uchar* src1, माप_प्रकार step1, स्थिर uchar* /*src2*/, माप_प्रकार /*step2*/,
           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, व्योम*)
{
    स्थिर द्विगुन noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
