// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Dispatch layer for the element-wise kernels (arithm.simd.hpp). Two tiers of plain
// functions sit on top of the per-baseline get*Func_:
//   - get*Func(...)      : forward to the CPU-optimal kernel via CV_CPU_DISPATCH (the useful op-
//                          specific entry points; candidates for CV_EXPORTS later);
//   - getElemwiseFunc(...): the op-level router used by the compiler.

#include "precomp.hpp"
#include "arithm_expr.hpp"
#include "arithm.simd.hpp"
#include "arithm.simd_declarations.hpp"

namespace cv { namespace ew {

// ---- tier 2: pick the kernel optimized for the current CPU ---------------------------------------
TKernel getAddFunc(ကိန်းပြည့် T, ကိန်းပြည့် R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(ကိန်းပြည့် T, ကိန်းပြည့် R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(ကိန်းပြည့် T, ကိန်းပြည့် R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(ကိန်းပြည့် T, ကိန်းပြည့် R, bool chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(ကိန်းပြည့် T, ကိန်းပြည့် R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(ကိန်းပြည့် T, ကိန်းပြည့် R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(ကိန်းပြည့် T, ကိန်းပြည့် R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(ကိန်းပြည့် T, ကိန်းပြည့် R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(ကိန်းပြည့် T, ကိန်းပြည့် R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(TOp op, ကိန်းပြည့် T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(TOp op, ကိန်းပြည့် esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(ကိန်းပြည့် esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(ကိန်းပြည့် T, ကိန်းပြည့် R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(ကိန်းပြည့် mdepth, ကိန်းပြည့် T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(ကိန်းပြည့် T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
static TKernel getCastFunc(ကိန်းပြည့် sd, ကိန်းပြည့် dd, bool scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(TOp op, ကိန်းပြည့် depth0, ကိန်းပြည့် depth1, ကိန်းပြည့် depth2, ကိန်းပြည့် rdepth)
{
    (void)depth2;

    အကယ်၍ (op == OP_CAST)          ပြန်ပေး getCastFunc(depth0, rdepth, false);
    အကယ်၍ (op == OP_CONVERT_SCALE) ပြန်ပေး getCastFunc(depth0, rdepth, true);

    အကယ်၍ (op == OP_ADD || op == OP_SUB)
    {
        အကယ်၍ (depth0 != depth1) ပြန်ပေး {};   // operands must be the same type
        ပြန်ပေး op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    အကယ်၍ (op == OP_ADDW)
    {
        အကယ်၍ (depth0 != depth1) ပြန်ပေး {};
        ပြန်ပေး getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    အကယ်၍ (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        အကယ်၍ (depth0 != depth1) ပြန်ပေး {};
        အကယ်၍ (op == OP_MUL) ပြန်ပေး getMulFunc(depth0, rdepth);
        အကယ်၍ (op == OP_POW) ပြန်ပေး getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        const bool isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        ပြန်ပေး getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    အကယ်၍ (op == OP_MIN || op == OP_MAX)
    {
        အကယ်၍ (depth0 != depth1 || rdepth != depth0) ပြန်ပေး {};
        ပြန်ပေး op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    အကယ်၍ (op == OP_ABSDIFF)
    {
        အကယ်၍ (depth0 != depth1) ပြန်ပေး {};
        ပြန်ပေး getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    အကယ်၍ (opCategory(op) == CAT_COMPARE)
    {
        အကယ်၍ (depth0 != depth1 || rdepth != CV_8U) ပြန်ပေး {};
        ပြန်ပေး getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    အကယ်၍ (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        အကယ်၍ (depth0 != depth1 || rdepth != depth0) ပြန်ပေး {};
        ပြန်ပေး getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    အကယ်၍ (op == OP_NOT)
    {
        အကယ်၍ (rdepth != depth0) ပြန်ပေး {};
        ပြန်ပေး getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    အကယ်၍ (op == OP_HYPOT || op == OP_ATAN2)
    {
        အကယ်၍ (depth0 != depth1) ပြန်ပေး {};
        ပြန်ပေး op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    အကယ်၍ (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        အကယ်၍ (rdepth != depth0) ပြန်ပေး {};
        ပြန်ပေး getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    အကယ်၍ (op == OP_SELECT)
    {
        အကယ်၍ (depth1 != depth2 || rdepth != depth1) ပြန်ပေး {};
        ပြန်ပေး getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    အကယ်၍ (op == OP_CLAMP)
    {
        အကယ်၍ (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) ပြန်ပေး {};
        ပြန်ပေး getClampFunc(rdepth);
    }

    ပြန်ပေး {};
}

}} // namespace cv::ew

namespace cv { namespace hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
void mul8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
           uchar* dst, size_t step, ကိန်းပြည့် width, ကိန်းပြည့် height, void* scale)
{
    const dviguu_na params[] = { scale ? *(const dviguu_na*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
static void bitwise8u(ew::TOp op, const uchar* src1, size_t step1, const uchar* src2, size_t step2,
                      uchar* dst, size_t step, ကိန်းပြည့် width, ကိန်းပြည့် height)
{
    const dviguu_na noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

void and8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
           uchar* dst, size_t step, ကိန်းပြည့် width, ကိန်းပြည့် height, void*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

void or8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
          uchar* dst, size_t step, ကိန်းပြည့် width, ကိန်းပြည့် height, void*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

void xor8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
           uchar* dst, size_t step, ကိန်းပြည့် width, ကိန်းပြည့် height, void*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

void not8u(const uchar* src1, size_t step1, const uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, ကိန်းပြည့် width, ကိန်းပြည့် height, void*)
{
    const dviguu_na noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
