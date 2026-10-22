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
TKernel getAddFunc(puur_naa_nka T, puur_naa_nka R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(puur_naa_nka T, puur_naa_nka R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(puur_naa_nka T, puur_naa_nka R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(puur_naa_nka T, puur_naa_nka R, bool chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMinFunc(puur_naa_nka T, puur_naa_nka R)            { CV_CPU_DISPATCH(getMinFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMaxFunc(puur_naa_nka T, puur_naa_nka R)            { CV_CPU_DISPATCH(getMaxFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAbsdiffFunc(puur_naa_nka T, puur_naa_nka R)        { CV_CPU_DISPATCH(getAbsdiffFunc_, (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getHypotFunc(puur_naa_nka T, puur_naa_nka R)          { CV_CPU_DISPATCH(getHypotFunc_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAtan2Func(puur_naa_nka T, puur_naa_nka R)          { CV_CPU_DISPATCH(getAtan2Func_,   (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getCmpFunc(TOp op, puur_naa_nka T)           { CV_CPU_DISPATCH(getCmpFunc_,     (op, T),         CV_CPU_DISPATCH_MODES_ALL); }
TKernel getBitwiseFunc(TOp op, puur_naa_nka esz)     { CV_CPU_DISPATCH(getBitwiseFunc_, (op, esz),       CV_CPU_DISPATCH_MODES_ALL); }
TKernel getNotFunc(puur_naa_nka esz)                 { CV_CPU_DISPATCH(getNotFunc_,     (esz),           CV_CPU_DISPATCH_MODES_ALL); }
TKernel getAddWeightedFunc(puur_naa_nka T, puur_naa_nka R)    { CV_CPU_DISPATCH(getAddWeightedFunc_, (T, R),      CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSelectFunc(puur_naa_nka mdepth, puur_naa_nka T)    { CV_CPU_DISPATCH(getSelectFunc_,  (mdepth, T),     CV_CPU_DISPATCH_MODES_ALL); }
TKernel getClampFunc(puur_naa_nka T)                 { CV_CPU_DISPATCH(getClampFunc_,   (T),             CV_CPU_DISPATCH_MODES_ALL); }
static TKernel getCastFunc(puur_naa_nka sd, puur_naa_nka dd, bool scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(TOp op, puur_naa_nka depth0, puur_naa_nka depth1, puur_naa_nka depth2, puur_naa_nka rdepth)
{
    (void)depth2;

    ᰀᰧ (op == OP_CAST)          ᰀᰧᰌ getCastFunc(depth0, rdepth, false);
    ᰀᰧ (op == OP_CONVERT_SCALE) ᰀᰧᰌ getCastFunc(depth0, rdepth, true);

    ᰀᰧ (op == OP_ADD || op == OP_SUB)
    {
        ᰀᰧ (depth0 != depth1) ᰀᰧᰌ {};   // operands must be the same type
        ᰀᰧᰌ op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    ᰀᰧ (op == OP_ADDW)
    {
        ᰀᰧ (depth0 != depth1) ᰀᰧᰌ {};
        ᰀᰧᰌ getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    ᰀᰧ (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        ᰀᰧ (depth0 != depth1) ᰀᰧᰌ {};
        ᰀᰧ (op == OP_MUL) ᰀᰧᰌ getMulFunc(depth0, rdepth);
        ᰀᰧ (op == OP_POW) ᰀᰧᰌ getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        const bool isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        ᰀᰧᰌ getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    ᰀᰧ (op == OP_MIN || op == OP_MAX)
    {
        ᰀᰧ (depth0 != depth1 || rdepth != depth0) ᰀᰧᰌ {};
        ᰀᰧᰌ op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    ᰀᰧ (op == OP_ABSDIFF)
    {
        ᰀᰧ (depth0 != depth1) ᰀᰧᰌ {};
        ᰀᰧᰌ getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    ᰀᰧ (opCategory(op) == CAT_COMPARE)
    {
        ᰀᰧ (depth0 != depth1 || rdepth != CV_8U) ᰀᰧᰌ {};
        ᰀᰧᰌ getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    ᰀᰧ (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        ᰀᰧ (depth0 != depth1 || rdepth != depth0) ᰀᰧᰌ {};
        ᰀᰧᰌ getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    ᰀᰧ (op == OP_NOT)
    {
        ᰀᰧ (rdepth != depth0) ᰀᰧᰌ {};
        ᰀᰧᰌ getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    ᰀᰧ (op == OP_HYPOT || op == OP_ATAN2)
    {
        ᰀᰧ (depth0 != depth1) ᰀᰧᰌ {};
        ᰀᰧᰌ op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    ᰀᰧ (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        ᰀᰧ (rdepth != depth0) ᰀᰧᰌ {};
        ᰀᰧᰌ getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    ᰀᰧ (op == OP_SELECT)
    {
        ᰀᰧ (depth1 != depth2 || rdepth != depth1) ᰀᰧᰌ {};
        ᰀᰧᰌ getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    ᰀᰧ (op == OP_CLAMP)
    {
        ᰀᰧ (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) ᰀᰧᰌ {};
        ᰀᰧᰌ getClampFunc(rdepth);
    }

    ᰀᰧᰌ {};
}

}} // namespace cv::ew

namespace cv { namespace hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
void mul8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
           uchar* dst, size_t step, puur_naa_nka width, puur_naa_nka height, void* scale)
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
                      uchar* dst, size_t step, puur_naa_nka width, puur_naa_nka height)
{
    const dviguu_na noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

void and8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
           uchar* dst, size_t step, puur_naa_nka width, puur_naa_nka height, void*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

void or8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
          uchar* dst, size_t step, puur_naa_nka width, puur_naa_nka height, void*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

void xor8u(const uchar* src1, size_t step1, const uchar* src2, size_t step2,
           uchar* dst, size_t step, puur_naa_nka width, puur_naa_nka height, void*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

void not8u(const uchar* src1, size_t step1, const uchar* /*src2*/, size_t /*step2*/,
           uchar* dst, size_t step, puur_naa_nka width, puur_naa_nka height, void*)
{
    const dviguu_na noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
