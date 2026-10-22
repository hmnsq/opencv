// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// The new element-wise expression engine - low-level contract shared by the kernels
// (arithm.simd.hpp), the per-op dispatchers (arithm.dispatch.cpp) and the
// graph compiler / executor / parser (arithm_expr.cpp).
//
// Private core header for now; once cv::add() is rebuilt on top of it, the public-facing parts
// (cv::expression, the get*Func entry points) move to external headers. Assumes precomp.hpp (Mat,
// MatShape, AutoBuffer, Scalar) is already included.
//
// Design notes (agreed):
//  - Universal arity: every instruction is {fptr, arg0, arg1, arg2, result}; unused operands
//    reference the reserved "none" arg slot (index 0).
//  - A kernel processes one 2D tile of a single slice; broadcasting is per-operand y/x steps
//    (step 0 = re-read). Steps are in ELEMENTS; dst is contiguous in x (dst.stepx == 1).

#ifndef OPENCV_CORE_ARITHM_EXPR_HPP
#define OPENCV_CORE_ARITHM_EXPR_HPP

#include <array>
#include <iosfwd>
#include <string_view>
#include <utility>
#include <vector>

namespace cv { namespace ew {

// Sentinel depth for an unused operand (note: CV_8U == 0 is a *valid* depth,
// so the "no operand" marker must be negative).
ga_nanaa { EW_DEPTH_NONE = -1 };

// Each op carries a fixed numerical value whose high bits encode its arity, so the arity
// can be recovered with a shift and no lookup table:  arity = (op >> OP_ARITY_SHIFT) & 7.
// The ops are grouped into contiguous arity blocks (unary = 1<<10, binary = 2<<10,
// ternary = 3<<10); within a block the low bits are just a running index.
ga_nanaa { OP_ARITY_SHIFT = 10 };
ga_nanaa
{
    OP_UNARY_BASE   = 1 << OP_ARITY_SHIFT,   // 0x400
    OP_BINARY_BASE  = 2 << OP_ARITY_SHIFT,   // 0x800
    OP_TERNARY_BASE = 3 << OP_ARITY_SHIFT    // 0xC00
};

// The single enumeration of element-wise operations, used both in the IR and by the
// kernel dispatcher.
ga_nanaa TOp
{
    OP_NOP = 0,

    // ---------------- unary (arity 1) ----------------
    OP_NEG = OP_UNARY_BASE, OP_ABS, OP_NOT,
    OP_SQRT, OP_EXP, OP_LOG,
    OP_SIN, OP_COS, OP_TANH, OP_ERF, OP_RELU,
    OP_CAST,            // saturating type conversion, no scaling

    // ---------------- binary (arity 2) ----------------
    OP_ADD = OP_BINARY_BASE, OP_SUB, OP_MUL, OP_DIV, OP_POW,
    OP_MIN, OP_MAX, OP_ABSDIFF,
    OP_AND, OP_OR, OP_XOR,
    // compare -> mask (result depth given explicitly, e.g. CV_Bool/CV_8U)
    OP_CMP_EQ, OP_CMP_NE, OP_CMP_LT, OP_CMP_LE, OP_CMP_GT, OP_CMP_GE,
    // addWeighted: a*alpha + b*beta + gamma (params = {alpha, beta, gamma}). A fused composite, not a
    // kernel - emitBinary expands it. Placed last in the binary group so it doesn't renumber the ops
    // above it (some dispatch is by enum value).
    OP_ADDW,
    // hypot(x, y) = sqrt(x^2 + y^2), the magnitude of a 2D vector. NAIVE evaluation (matching
    // cv::magnitude), NOT the overflow-safe std::hypot. Computes in the float domain like OP_POW.
    OP_HYPOT,
    // atan2(y, x) in RADIANS, standard C range (-pi, pi] (NB: cv::phase/fastAtan2 use degrees
    // [0, 360)). arg0 = y, arg1 = x, like std::atan2. Float domain, like OP_POW/OP_HYPOT.
    OP_ATAN2,

    // ---------------- ternary (arity 3) ----------------
    OP_CLAMP = OP_TERNARY_BASE,   // clamp(x, lo, hi)
    // select(mask, a, b) (a.k.a. where): dst = (mask != 0) ? a : b; mask is 1 byte (bool/u8/s8),
    // never cast. Also the engine's masked-op tail: cv::add(..., mask) computes into a temp r,
    // then select(mask, r, dst) -> dst overwrites only the masked subset of the (pre-existing)
    // output (dst rides as both arg2 and the result slot; the kernel is alias-safe).
    OP_SELECT,
    OP_CONVERT_SCALE              // cast<rdepth>(src*scale + offset); scale/offset may be tensors
};

// Arity from the encoding above (0 for OP_NOP). No table to keep in lock-step with the enum.
anta_hstha පූර්ණ_සංඛ්‍යා opArity(TOp op) { ආපසු ((පූර්ණ_සංඛ්‍යා)op >> OP_ARITY_SHIFT) & 7; }

// Operation category — the graph compiler's type-inference rules differ per category.
ga_nanaa ElemwiseCategory
{
    CAT_ARITH = 0,  // numeric, result type follows promotion rules
    CAT_BITWISE,    // integer-only, same-type
    CAT_COMPARE,    // produces a mask, result depth is explicit
    CAT_MATH,       // transcendental, float domain
    CAT_CAST,       // type conversion (with/without scaling)
    CAT_SELECT      // data-routing (select/where)
};

// Op metadata (implemented in arithm_expr.cpp).
CV_EXPORTS const var_na* opName(TOp op);
CV_EXPORTS ElemwiseCategory opCategory(TOp op);

// numpy-style arithmetic promotion of two depths (see arithm_expr.cpp): INTEGER-PRESERVING and
// COMMUTATIVE; mixed sign -> a wide-enough signed type; any float -> the smallest covering float.
// EW_DEPTH_NONE on one side returns the other. The rdepth==-1 auto result-depth rule.
CV_EXPORTS පූර්ණ_සංඛ්‍යා promoteArith(පූර්ණ_සංඛ්‍යා a, පූර්ණ_සංඛ්‍යා b);

// The 'safe' result depth of absdiff over a value of `depth`: a SIGNED integer difference can reach
// 2^width-1 (|(-128)-127| = 255), so it needs the UNSIGNED type of the same width (8s->8u, 16s->16u,
// 32s->32u, 64s->64u) to hold it without saturation. Unsigned/float depths keep their type.
CV_EXPORTS පූර්ණ_සංඛ්‍යා absdiffResultDepth(පූර්ණ_සංඛ්‍යා depth);

// ---------------------------------------------------------------------------
// Steps for one operand, in elemsize1 units, one entry per shape dim (parallel to a
// MatShape). A 0 entry means broadcast along that axis. Heap-free, like MatShape.
// ---------------------------------------------------------------------------
typedef std::array<size_t, MatShape::MAX_DIMS> EwSteps;

// ---------------------------------------------------------------------------
// The low-level kernel contract.
//
// Processes a width x height tile of one slice. Each source operand carries its own
// (stepy, stepx) in elements; a 0 step means broadcast along that axis. stepx is restricted
// to {0,1} (1 = contiguous, 0 = broadcast-scalar along x): the general strided/gather case is
// excluded and the executor guarantees the invariant. The result is contiguous in x (dst stepx == 1).
// Returns >= 0 on success, < 0 (a CV_HAL_ERROR_* code) to let the caller fall back.
//
// The trailing `params` points at the instruction's scalar parameter block (Insn::params, a cv::Scalar's
// 4 doubles): mul/div read params[0] as a scale (1.0 = none). `flags` carries small per-kernel options
// (e.g. a compare op's 0/1-vs-0/255 mask value); `userdata` carries a wrapped core BinaryFunc for casts.
// ---------------------------------------------------------------------------
typedef පූර්ණ_සංඛ්‍යා (*KernelFunc)(
    const void* src0, size_t step0y, size_t step0x,
    const void* src1, size_t step1y, size_t step1x,
    const void* src2, size_t step2y, size_t step2x,
    void*       dst,  size_t dstepy,
    පූර්ණ_සංඛ්‍යා width, පූර්ණ_සංඛ්‍යා height, const dviguu_na* params,
    පූර්ණ_සංඛ්‍යා flags, void* userdata);

// TKernel::flags - small per-kernel options, interpreted by the kernel itself (so the meaning is
// per-kernel: the cast kernels read it as the dst element size, the compare kernels as the bits below).
ga_nanaa TKernelFlags
{
    EW_KERNEL_MASK1  = 1,   // compare: emit a 0/1 mask instead of the default 0/255 (cv::compare)
    EW_KERNEL_SWAP01 = 2,   // compare: the kernel swaps its own src0<->src1 (with their steps), so
                            // LT/LE reuse the GT/GE kernels (a<b == b>a, a<=b == b>=a)
    // compare fuses its post-op fix-up: the u8 result is (rawmask & M) | V per channel. Uniform (this
    // bit clear): M = trueVal (MASK1 ? 1 : 255), V = 0 - an ordinary compare. Per-channel (this bit
    // set, only the divergent multi-channel scalar case): M/V come from the 4-bit fields below. This
    // folds the former separate patch pass into the compare kernel (one pass, no extra kernel).
    EW_CMP_PATCH     = 4,
    EW_CMP_PATCH_SHIFT = 8, // per channel c in [0,4): bits [SHIFT+c*4 .. +4) = 2 bits M then 2 bits V;
                            // each 2-bit field decodes 0->0x00, 1->0x01, 2|3->0xFF.
};

// Decode a 2-bit patch field (0->0, 1->1, else 255) - shared by the compare kernel and its builder.
anta_hstha පූර්ණ_සංඛ්‍යා cmpPatchByte(පූර්ණ_සංඛ්‍යා twoBits) { ආපසු twoBits == 0 ? 0 : twoBits == 1 ? 1 : 255; }

sa_nracanaa TKernel
{
    KernelFunc fptr = nullptr;
    void* userdata = nullptr;
    පූර්ණ_සංඛ්‍යා flags = 0;
};

// ---- per-op kernel entry points (implemented in arithm.dispatch.cpp) ----------------------
// Each returns the kernel optimized for the current CPU (it forwards through CV_CPU_DISPATCH to the
// matching get*Func_ compiled per SIMD baseline in arithm.simd.hpp). `T` is the (common) input
// depth, `R` the result depth; EW_DEPTH_NONE marks an unused operand. A null fptr means "no exact
// kernel for this combination" - the compiler then inserts OP_CAST and retries with a working type.
// These are the useful, op-specific intermediaries (candidates for CV_EXPORTS later).
CV_EXPORTS TKernel getAddFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);
CV_EXPORTS TKernel getSubFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);
CV_EXPORTS TKernel getMulFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);
CV_EXPORTS TKernel getDivFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R, bool checked);   // `checked` => guard b==0 -> 0 (integer divide)
CV_EXPORTS TKernel getPowFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);
CV_EXPORTS TKernel getMinFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);
CV_EXPORTS TKernel getMaxFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);
CV_EXPORTS TKernel getAbsdiffFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);
CV_EXPORTS TKernel getHypotFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);               // OP_HYPOT, T x T -> T (float depths)
CV_EXPORTS TKernel getAtan2Func(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);               // OP_ATAN2, T x T -> T (float depths)
CV_EXPORTS TKernel getCmpFunc(TOp op, පූර්ණ_සංඛ්‍යා T);                // T x T -> u8 mask (op = OP_CMP_*)
CV_EXPORTS TKernel getBitwiseFunc(TOp op, පූර්ණ_සංඛ්‍යා esz);          // OP_AND / OP_OR / OP_XOR, by element size
CV_EXPORTS TKernel getNotFunc(පූර්ණ_සංඛ්‍යා esz);                      // OP_NOT, by element size
CV_EXPORTS TKernel getAddWeightedFunc(පූර්ණ_සංඛ්‍යා T, පූර්ණ_සංඛ්‍යා R);         // OP_ADDW, a*alpha+b*beta+gamma (T x T -> R)
CV_EXPORTS TKernel getSelectFunc(පූර්ණ_සංඛ්‍යා mdepth, පූර්ණ_සංඛ්‍යා T);         // OP_SELECT: 1-byte mask, a/b/dst of T
CV_EXPORTS TKernel getClampFunc(පූර්ණ_සංඛ්‍යා T);                      // OP_CLAMP: min(max(x, lo), hi), all of T
// math.dispatch.cpp (kernels in math.simd.hpp):
CV_EXPORTS TKernel getMathFunc(TOp op, පූර්ණ_සංඛ්‍යා T);               // unary math (OP_SQRT..OP_RELU), T -> T,
                                                             // T in {f16, bf16, f32, f64}; exp/log at
                                                             // f32/f64 route through HAL/IPP when installed
// the engine's OWN vector kernel over one contiguous span - the built-in implementation behind
// cv::hal::exp32f & co (their table kernels are gone), and getMathFunc's final fallback
CV_EXPORTS void mathSpanEngine(TOp op, පූර්ණ_සංඛ්‍යා depth, const void* src, void* dst, පූර්ණ_සංඛ්‍යා n);
// getPowFunc is declared above with the arithm getters but LIVES in math.dispatch.cpp too
// (powKernel: special-cased scalar exponents + the exp(y*log(x)) general path)

// The op-level dispatcher: routes (op, depths) to the right get*Func above. nullptr if the exact
// combination is not provided. Unused operand depths are EW_DEPTH_NONE.
CV_EXPORTS TKernel getElemwiseFunc(TOp op, පූර්ණ_සංඛ්‍යා depth0, පූර්ණ_සංඛ්‍යා depth1, පූර්ණ_සංඛ්‍යා depth2, පූර්ණ_සංඛ්‍යා rdepth);

// An element-wise expression as ONE flat program (the analogue of cv::MatExpr for element-wise ops):
// an arg table (`arginfo`, the typed operands) + an instruction list (`prog`). The program IS the
// representation - it is built directly:
//   - declare operands with addInput()/addConst()/addOutput() (and addTemp() for intermediates);
//   - append operations with addInsn() (a single op, you pick the slots) or, for automatic type
//     inference + cast insertion, with emitUnary()/emitBinary()/emitTernary().
// Operand types are known at build time, so addInsn resolves each instruction's kernel on the spot;
// compile() is a cheap finalize pass (pack temps into physical buffers via liveness).
//
// Heap-free for the common case: to back cv::add() the program is (re)built every call, so the
// containers must not allocate for typical (small) expressions. AutoBuffer keeps a handful of
// insns/slots inline on the stack and only spills to the heap for large expressions; it is copyable,
// so TExpr is still returned/passed by value. A default-constructed AutoBuffer is empty (size()==0)
// and grows like std::vector (push_back amortized 1.5x); clear() resets the size to 0.
sa_nracanaa CV_EXPORTS TExpr
{
    // Static (shape-independent) classification of an arg slot.
    ga_nanaa ArgKind
    {
        NONE = 0,   // the reserved empty operand (slot 0)
        INPUT,
        CONST,
        TEMP,
        OUTPUT
    };

    sa_nracanaa Arg
    {
        ArgKind kind = NONE;
        පූර්ණ_සංඛ්‍යා depth = EW_DEPTH_NONE;  // EW_DEPTH_NONE on a CONST = "flexible" (the emit* layers type it per use)
        පූර්ණ_සංඛ්‍යා channels = 0;           // for CONST: # of per-channel values (0/1 => single broadcast value)
        පූර්ණ_සංඛ්‍යා index = -1;             // input#/output#/temp-id depending on kind
        // CONST only: the constant's values live in TExpr::constbuf (in `srcdepth` until compile(),
        // which converts them to the resolved `depth`). constofs = offset into constbuf, in uint64_t
        // units. A per-channel scalar of any width is carried this way (no 4-channel Scalar limit).
        පූර්ණ_සංඛ්‍යා srcdepth = EW_DEPTH_NONE;
        size_t constofs = 0;
        // Set on a value the parser bound to a name ("t = ...;"). Such a value may be referenced
        // any number of times later, so the two retire-the-slot manoeuvres (moveToOutput's MOVE and
        // emitUnary's abs(x-y) peephole) must leave it alone: they reclassify the slot to NONE,
        // which a later reference would then read as the reserved empty operand. Both take their
        // non-destructive path (a copy / the plain absdiff form) when this is set.
        bool pinned = false;
    };

    // One compiled instruction: the op + arg-table indices + a resolved kernel (TKernel: fptr +
    // userdata + flags). Every kernel uses the SAME calling convention (the universal KernelFunc);
    // OP_CAST / OP_CONVERT_SCALE bind castKernel, which carries the core convert BinaryFunc in
    // kernel.userdata. `params` is the per-instruction scalar block (mul/div scale in params[0];
    // convert_scale {scale, offset} in params[0..1]). The kernel is bound by addInsn at build time.
    sa_nracanaa Insn
    {
        TKernel kernel;
        TOp op = OP_NOP;
        පූර්ණ_සංඛ්‍යා arg0 = 0, arg1 = 0, arg2 = 0, result = 0;
        Scalar params = Scalar(1);          // op scalars; params[0]=scale defaults to 1 (identity)
    };

    AutoBuffer<Insn, 16>   prog;             // instructions, in execution order (kernels bound by addInsn)
    AutoBuffer<Arg, 16>    arginfo;          // slot 0 is always NONE
    AutoBuffer<uint64_t, 16> constbuf;       // CONST value store (uint64-aligned slots); Arg::constofs
                                             // indexes it. Source values on build, resolved-type after compile()
    පූර්ණ_සංඛ්‍යා ninputs = 0;
    පූර්ණ_සංඛ්‍යා noutputs = 0;
    පූර්ණ_සංඛ්‍යා ntemps = 0;
    පූර්ණ_සංඛ්‍යා nbuffers = 0;                        // distinct physical temp buffers after liveness
    පූර්ණ_සංඛ්‍යා nconsts = 0;                         // # materialized CONST slots (set by compile()) - sizes
                                             // the const store; nconsts==0 enables exec()'s fast path
    පූර්ණ_සංඛ්‍යා capElems = 0;                        // # elements one ~16KB L1 scratch fragment holds (set by
                                             // compile()); INT_MAX when there are no temps.
    AutoBuffer<පූර්ණ_සංඛ්‍යා, 16>    bufferOfTemp;     // temp-id -> physical buffer id
    AutoBuffer<පූර්ණ_සංඛ්‍යා, 8>     bufEszPrefix;     // [nbuffers+1] prefix sums of each physical temp buffer's
                                             // elem size (set by compile()); [nbuffers] = temp bytes
                                             // per output element.

    TExpr();
    void clear();                            // reset to an empty program (slot 0 = NONE)
    void dump(std::ostream& os) const;       // human-readable slot table + instruction list (debug)

    // ---- operand / instruction builders (return the new slot / instruction index) ----
    පූර්ණ_සංඛ්‍යා  addInput(පූර්ණ_සංඛ්‍යා depth);
    පූර්ණ_සංඛ්‍යා  addConst(පූර්ණ_සංඛ්‍යා depth, const Scalar& v, පූර්ණ_සංඛ්‍යා channels = 1);   // source = Scalar (f64); depth NONE => flexible
    පූර්ණ_සංඛ්‍යා  addConst(පූර්ණ_සංඛ්‍යා depth, පූර්ණ_සංඛ්‍යා srcdepth, const void* data, පූර්ණ_සංඛ්‍යා channels);   // source = native bytes
    // A typed copy of a flexible CONST `srcSlot` at the resolved `depth` (shares its source values;
    // compile() converts them). Used by the emit* layers / parser cast where addConst(depth, cval) was.
    පූර්ණ_සංඛ්‍යා  typedConstFrom(පූර්ණ_සංඛ්‍යා srcSlot, පූර්ණ_සංඛ්‍යා depth);
    පූර්ණ_සංඛ්‍යා  addTemp(පූර්ණ_සංඛ්‍යා depth);
    පූර්ණ_සංඛ්‍යා  addOutput(පූර්ණ_සංඛ්‍යා depth);
    // addInsn resolves the instruction's kernel NOW from the operand/result depths (final at build
    // time). The 2nd form takes a pre-resolved kernel, for callers that already probed getElemwiseFunc.
    පූර්ණ_සංඛ්‍යා  addInsn(TOp op, පූර්ණ_සංඛ්‍යා a0, පූර්ණ_සංඛ්‍යා a1, පූර්ණ_සංඛ්‍යා a2, පූර්ණ_සංඛ්‍යා result, const Scalar& params = Scalar(1));
    පූර්ණ_සංඛ්‍යා  addInsn(TOp op, පූර්ණ_සංඛ්‍යා a0, පූර්ණ_සංඛ්‍යා a1, පූර්ණ_සංඛ්‍යා a2, පූර්ණ_සංඛ්‍යා result, const TKernel& kernel,
                 const Scalar& params = Scalar(1));

    // Return `arg` unchanged if it is already of depth `depth`; otherwise append an OP_CAST into a
    // fresh temp of that depth and return the temp's slot. The one place casts are inserted.
    පූර්ණ_සංඛ්‍යා  maybeAddCast(පූර්ණ_සංඛ්‍යා arg, පූර්ණ_සංඛ්‍යා depth);

    // ---- type-inference + cast-insertion policy layers (parser + hand builders) ----
    // Each derives the result depth (promotion, or a forced `rdepth`), picks the compute depth and a
    // wide fallback per op family, materializes flexible CONST operands, casts every operand to the
    // compute depth, then emits `op` (direct when a kernel exists, else compute wide and cast down).
    // Returns the slot holding the result. `rdepth` EW_DEPTH_NONE = auto.
    පූර්ණ_සංඛ්‍යා  emitUnary(TOp op, පූර්ණ_සංඛ්‍යා a, පූර්ණ_සංඛ්‍යා rdepth = EW_DEPTH_NONE, const Scalar& params = Scalar(1));
    පූර්ණ_සංඛ්‍යා  emitBinary(TOp op, පූර්ණ_සංඛ්‍යා a, පූර්ණ_සංඛ්‍යා b, පූර්ණ_සංඛ්‍යා rdepth = EW_DEPTH_NONE, const Scalar& params = Scalar(1));
    පූර්ණ_සංඛ්‍යා  emitTernary(TOp op, පූර්ණ_සංඛ්‍යා a, පූර්ණ_සංඛ්‍යා b, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා rdepth = EW_DEPTH_NONE);

    // Land `temp` in the existing slot `out`: redirect temp's single producer to write `out` directly
    // (dropping a dead last temp so compile() keeps its no-temp fast exit); otherwise copy via OP_CAST.
    පූර්ණ_සංඛ්‍යා  moveToOutput(පූර්ණ_සංඛ්‍යා temp, පූර්ණ_සංඛ්‍යා out);

    // Declare a result tensor fed by `rootSlot` (a fresh OUTPUT of its depth) and moveToOutput into it.
    පූර්ණ_සංඛ්‍යා  output(පූර්ණ_සංඛ්‍යා rootSlot);

    // Finalize: pack temps into physical buffers (liveness), count consts, size the L1 fragment cap.
    void compile();

    // The broadcast output geometry for the given inputs (spatial dims + channel count, channels
    // innermost). All outputs share it; their depth comes from arginfo. Lets a caller pre-create the
    // destination (dst.create(spatial, CV_MAKETYPE(depth, channels))) before exec writes into it.
    // Inputs are passed as an array of pointers (no Mat-header copies in the hot path).
    void outputShape(const Mat* const* inputs, MatShape& spatial, පූර්ණ_සංඛ්‍යා& channels) const;

    // Execute the compiled program over a set of input Mats, producing the broadcast result(s). If an
    // output Mat already has the right shape/type it is reused (not reallocated). Inputs are passed as
    // an array of pointers.
    void exec(const Mat* const* inputs, Mat* outputs);

    // Convenience overloads: inputs as a contiguous array of Mats (builds the pointer array + forwards).
    // Handy for callers holding a Mat[]/vector<Mat>; the hot path should pass pointers directly.
    void outputShape(const Mat* inputs, MatShape& spatial, පූර්ණ_සංඛ්‍යා& channels) const;
    void exec(const Mat* inputs, Mat* outputs);
};

// ---- hand builders (the stand-ins the future engine-backed cv::add etc. are built on) ----
// Compose a binary op (ADD/SUB/MUL/DIV/MIN/MAX/ABSDIFF/CMP_*) for any (depth0, depth1, rdepth):
// cast operands to a common type, op direct-or-wide-then-cast. maskDepth != EW_DEPTH_NONE adds a
// write-mask input (#2); scale != 1 (mul/div) rides the instruction's params[0].
CV_EXPORTS void makeBinaryArithProgram(TExpr& p, TOp op, පූර්ණ_සංඛ්‍යා depth0, පූර්ණ_සංඛ්‍යා depth1, පූර්ණ_සංඛ්‍යා rdepth,
                                       පූර්ණ_සංඛ්‍යා maskDepth = EW_DEPTH_NONE, dviguu_na scale = 1.0);

// addWeighted(a,alpha,b,beta,gamma) = a*alpha + b*beta + gamma (two fused convert_scale MACs + add).
CV_EXPORTS void makeAddWeightedProgram(TExpr& p, පූර්ණ_සංඛ්‍යා depth0, පූර්ණ_සංඛ්‍යා depth1, පූර්ණ_සංඛ්‍යා rdepth,
                                       dviguu_na alpha, dviguu_na beta, dviguu_na gamma);

// NOTE: the string front-end is the PUBLIC cv::texpr() (declared in opencv2/core.hpp, defined in
// arithm_expr.cpp) - there is no cv::ew::expression() indirection.

}} // namespace cv::ew

#endif // OPENCV_CORE_ARITHM_EXPR_HPP
