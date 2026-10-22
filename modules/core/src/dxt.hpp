// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Shared, ISA-independent types of the 1D DFT/DCT engine (dxt.cpp + dxt.simd.hpp).
//
// The engine runs a 1D transform in three groups of passes:
//   A (preproc):  table-driven gather from the user input (or from an interleaved temp) into
//                 split re[]/im[] workspace, fused with the first twiddle-free radix-r0 stage,
//                 r0 in {1,2,4,8}. Scalar.
//   B (stages):   mixed-radix decimation-in-time stages with growing span, direction-free
//                 (forward twiddles only), out-of-place (src pair -> dst pair, ping-pong) or
//                 in-place when the plan says it is legal. Vectorized with universal intrinsics.
//   C (postproc): interleave / CCS packing / DCT ordering with fused scaling into the user output.
//
// Inverse transforms use the conjugation trick: conj() on the gather, im_scale = -scale on output.
// Buffers keep fixed semantics (re is always re).
//
// Plans are immutable after construction and shared read-only by all rows of a batch; the
// workspace is a separate, per-executor, mutable buffer allocated ONCE by the owner (never per
// call).

#ifndef OPENCV_CORE_SRC_DXT_HPP
#define OPENCV_CORE_SRC_DXT_HPP

#അന്തര്ഭാവ "opencv2/core/utility.hpp"

നാമാകാശ cv {

എണ്ണൽ DftKind
{
    DFT_KIND_C2C = 0,   // complex -> complex (forward or inverse)
    DFT_KIND_R2C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_KIND_C2R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_KIND_DCT,       // DCT-II, even n only
    DFT_KIND_IDCT       // DCT-III, even n only
};

എണ്ണൽ DftStageMode
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_FULL = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

എണ്ണൽ { DFT_MAX_STAGES = 40 };

ഘടന DftStage
{
    പൂർണ്ണസംഖ്യ radix;          // 3, 4, 5 or an odd number >= 7
    പൂർണ്ണസംഖ്യ span;           // L: length of the sub-DFTs combined by this stage
    പൂർണ്ണസംഖ്യ ngroups;        // G = nc / (radix*span)
    പൂർണ്ണസംഖ്യ nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    പൂർണ്ണസംഖ്യ mode;           // DftStageMode
    പൂർണ്ണസംഖ്യ tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    സ്ഥിരം ശൂന്യം* tw_re;
    സ്ഥിരം ശൂന്യം* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    സ്ഥിരം ശൂന്യം* cs;
    സ്ഥിരം ശൂന്യം* sn;
};

ഘടന DftPlan
{
    പൂർണ്ണസംഖ്യ kind;           // DftKind
    പൂർണ്ണസംഖ്യ n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    പൂർണ്ണസംഖ്യ nc;             // complex length actually run through the stages (n/2 for even real kinds)
    പൂർണ്ണസംഖ്യ depth;          // CV_32F or CV_64F
    പൂർണ്ണസംഖ്യ vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    പൂർണ്ണസംഖ്യ first_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    തർക്കം real_നിവേശം;    // R2C with odd n: gather real samples, im = 0
    തർക്കം pingpong;      // stages must alternate between two workspace pairs
    തർക്കം need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    പൂർണ്ണസംഖ്യ nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    AutoBuffer<പൂർണ്ണസംഖ്യ> itab;
    AutoBuffer<ഇരട്ട> tw;      // storage for all the tables below (type T, 8-byte aligned)
    സ്ഥിരം ശൂന്യം* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    സ്ഥിരം ശൂന്യം* rtw_im;
    സ്ഥിരം ശൂന്യം* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    സ്ഥിരം ശൂന്യം* dct_im;
    size_t ws_bytes;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : kind(0), n(0), nc(0), depth(0), vl(0), first_radix(1), real_നിവേശം(അസത്യം),
                pingpong(അസത്യം), need_tmp(അസത്യം), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_bytes(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    ശൂന്യം build(പൂർണ്ണസംഖ്യ kind, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ vl);

സ്വകാര്യ:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    template<typename T> ശൂന്യം fillTables(size_t ntw, തർക്കം need_rtw, തർക്കം need_dct);

    DftPlan(സ്ഥിരം DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& operator=(സ്ഥിരം DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
തരനിർവചനം ശൂന്യം (*DftPreprocFunc)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* src, size_t sstep,
                               ശൂന്യം* re, ശൂന്യം* im, തർക്കം conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
തരനിർവചനം ശൂന്യം (*DftStageFunc)(സ്ഥിരം DftStage& st, സ്ഥിരം ശൂന്യം* sre, സ്ഥിരം ശൂന്യം* sim,
                             ശൂന്യം* dre, ശൂന്യം* dim, ശൂന്യം* scratch);

ഘടന DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    ശൂന്യം (*preprocCCS)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* src, ശൂന്യം* tmp, തർക്കം complex_നിവേശം);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    ശൂന്യം (*preprocDCT)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* src, size_t sstep, ശൂന്യം* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    ശൂന്യം (*preprocIDCT)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* src, size_t sstep, ശൂന്യം* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    ശൂന്യം (*postprocDFT)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* re, സ്ഥിരം ശൂന്യം* im, ശൂന്യം* dst,
                        ഇരട്ട re_scale, ഇരട്ട im_scale);          // split -> interleaved complex
    ശൂന്യം (*postprocRealDFT)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* re, സ്ഥിരം ശൂന്യം* im, ശൂന്യം* dst,
                            ഇരട്ട scale, തർക്കം complex_output);     // R2C untangle + CCS/complex packing
    ശൂന്യം (*postprocReal)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* re, ശൂന്യം* dst, ഇരട്ട scale); // re only
    ശൂന്യം (*postprocDCT)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* re, സ്ഥിരം ശൂന്യം* im, ശൂന്യം* dst, size_t dstep);
    ശൂന്യം (*postprocIDCT)(സ്ഥിരം DftPlan& plan, സ്ഥിരം ശൂന്യം* re, സ്ഥിരം ശൂന്യം* im, ശൂന്യം* dst, size_t dstep);
    പൂർണ്ണസംഖ്യ vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#endif
