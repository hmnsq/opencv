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
TKernel getAddFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getAddFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getSubFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getSubFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getMulFunc(पूर्णांक T, पूर्णांक R)            { CV_CPU_DISPATCH(getMulFunc_,     (T, R),          CV_CPU_DISPATCH_MODES_ALL); }
TKernel getDivFunc(पूर्णांक T, पूर्णांक R, तार्किक chk)  { CV_CPU_DISPATCH(getDivFunc_,     (T, R, chk),     CV_CPU_DISPATCH_MODES_ALL); }
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
स्थिर TKernel getCastFunc(पूर्णांक sd, पूर्णांक dd, तार्किक scaled)
                                            { CV_CPU_DISPATCH(getCastFunc_,    (sd, dd, scaled),CV_CPU_DISPATCH_MODES_ALL); }

// ---- tier 3: op-level dispatcher used by the compiler --------------------------------------------
TKernel getElemwiseFunc(शीर्ष op, पूर्णांक depth0, पूर्णांक depth1, पूर्णांक depth2, पूर्णांक rdepth)
{
    (शून्य)depth2;

    जर (op == OP_CAST)          परत getCastFunc(depth0, rdepth, असत्य);
    जर (op == OP_CONVERT_SCALE) परत getCastFunc(depth0, rdepth, सत्य);

    जर (op == OP_ADD || op == OP_SUB)
    {
        जर (depth0 != depth1) परत {};   // operands must be the same type
        परत op == OP_ADD ? getAddFunc(depth0, rdepth) : getSubFunc(depth0, rdepth);
    }

    // OP_ADDW (addWeighted): a*alpha+b*beta+gamma; operands same type T, result R (T/f32 for small ints
    // + f16/bf16/f32, f64 otherwise). alpha/beta/gamma travel in the instruction's params.
    जर (op == OP_ADDW)
    {
        जर (depth0 != depth1) परत {};
        परत getAddWeightedFunc(depth0, rdepth);
    }

    // OP_MUL / OP_DIV / OP_POW: operands same type T; compute in the float work type (rdepth).
    जर (op == OP_MUL || op == OP_DIV || op == OP_POW)
    {
        जर (depth0 != depth1) परत {};
        जर (op == OP_MUL) परत getMulFunc(depth0, rdepth);
        जर (op == OP_POW) परत getPowFunc(depth0, rdepth);
        // integer inputs guard divide-by-zero (-> 0); floats do not (a/0 -> inf, matching cv::divide).
        स्थिरांक तार्किक isflt = depth0==CV_16F || depth0==CV_16BF || depth0==CV_32F || depth0==CV_64F;
        परत getDivFunc(depth0, rdepth, !isflt);
    }

    // OP_MIN / OP_MAX: T x T -> T.
    जर (op == OP_MIN || op == OP_MAX)
    {
        जर (depth0 != depth1 || rdepth != depth0) परत {};
        परत op == OP_MIN ? getMinFunc(depth0, rdepth) : getMaxFunc(depth0, rdepth);
    }

    // OP_ABSDIFF: result is absdiffResultDepth(T) (unsigned same width for signed).
    जर (op == OP_ABSDIFF)
    {
        जर (depth0 != depth1) परत {};
        परत getAbsdiffFunc(depth0, rdepth);
    }

    // OP_CMP_*: T x T -> u8 mask (0 / 1 / 255, value via TKernel::flags).
    जर (opCategory(op) == CAT_COMPARE)
    {
        जर (depth0 != depth1 || rdepth != CV_8U) परत {};
        परत getCmpFunc(op, depth0);
    }

    // OP_AND / OP_OR / OP_XOR: bit-pattern op, T x T -> T (same depth), dispatched by element size.
    जर (op == OP_AND || op == OP_OR || op == OP_XOR)
    {
        जर (depth0 != depth1 || rdepth != depth0) परत {};
        परत getBitwiseFunc(op, CV_ELEM_SIZE1(depth0));
    }

    // OP_NOT: ~x, one input, T -> T (same depth), dispatched by element size.
    जर (op == OP_NOT)
    {
        जर (rdepth != depth0) परत {};
        परत getNotFunc(CV_ELEM_SIZE1(depth0));
    }

    // OP_HYPOT / OP_ATAN2: T x T -> T over the float depths (integer inputs are the compiler's job).
    जर (op == OP_HYPOT || op == OP_ATAN2)
    {
        जर (depth0 != depth1) परत {};
        परत op == OP_HYPOT ? getHypotFunc(depth0, rdepth) : getAtan2Func(depth0, rdepth);
    }

    // unary math: T -> T over the float depths (math.simd.hpp). Anything else - integer input,
    // widening/narrowing result - is the compiler's job (cast to a float working type first).
    जर (op == OP_SQRT || op == OP_EXP || op == OP_LOG || op == OP_SIN || op == OP_COS ||
        op == OP_TANH || op == OP_ERF || op == OP_RELU)
    {
        जर (rdepth != depth0) परत {};
        परत getMathFunc(op, depth0);
    }

    // OP_SELECT: depth0 is the (1-byte, never cast) mask; both branches and dst share one depth.
    जर (op == OP_SELECT)
    {
        जर (depth1 != depth2 || rdepth != depth1) परत {};
        परत getSelectFunc(depth0, rdepth);
    }

    // OP_CLAMP: x, lo, hi and dst all share one depth (emitTernary unifies them).
    जर (op == OP_CLAMP)
    {
        जर (depth1 != depth0 || depth2 != depth0 || rdepth != depth0) परत {};
        परत getClampFunc(rdepth);
    }

    परत {};
}

}} // namespace cv::ew

नामाकाश cv { नामाकाश hal {

// Legacy cv::hal entry point, still declared in core/hal/hal.hpp and used by external code (the
// G-API fluid backend in opencv_contrib calls it directly): forward to the element-wise engine's
// u8 multiply kernel. `scale` is a pointer to a double, as in the old contract.
शून्य mul8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, शून्य* scale)
{
    स्थिरांक द्विगुण params[] = { scale ? *(स्थिरांक द्विगुण*)scale : 1.0 };
    ew::TKernel k = ew::getMulFunc(CV_8U, CV_8U);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           params, k.flags, k.userdata);
}

// Legacy cv::hal bitwise entry points, still declared in core/hal/hal.hpp and used by other modules
// (e.g. opencv_objdetect's aruco) and external code: forward to the element-wise engine's byte-wise
// bitwise kernels. No scalar params - bitwise ops ignore them.
स्थिर शून्य bitwise8u(ew::शीर्ष op, स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
                      uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height)
{
    स्थिरांक द्विगुण noparams[4] = {};
    ew::TKernel k = ew::getBitwiseFunc(op, 1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, src2, step2, 1, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

शून्य and8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, शून्य*)
{ bitwise8u(ew::OP_AND, src1, step1, src2, step2, dst, step, width, height); }

शून्य or8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
          uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, शून्य*)
{ bitwise8u(ew::OP_OR, src1, step1, src2, step2, dst, step, width, height); }

शून्य xor8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* src2, माप_प्रकार step2,
           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, शून्य*)
{ bitwise8u(ew::OP_XOR, src1, step1, src2, step2, dst, step, width, height); }

शून्य not8u(स्थिरांक uchar* src1, माप_प्रकार step1, स्थिरांक uchar* /*src2*/, माप_प्रकार /*step2*/,
           uchar* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height, शून्य*)
{
    स्थिरांक द्विगुण noparams[4] = {};
    ew::TKernel k = ew::getNotFunc(1);
    CV_Assert(k.fptr != nullptr);
    k.fptr(src1, step1, 1, nullptr, 0, 0, nullptr, 0, 0, dst, step, width, height,
           noparams, k.flags, k.userdata);
}

}} // namespace cv::hal
