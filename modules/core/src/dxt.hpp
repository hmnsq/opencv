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

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utility.hpp"

ନାମାକାଶ cv {

ଗଣନା DftKind
{
    DFT_KIND_C2C = 0,   // complex -> complex (forward or inverse)
    DFT_KIND_R2C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_KIND_C2R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_KIND_DCT,       // DCT-II, even n only
    DFT_KIND_IDCT       // DCT-III, even n only
};

ଗଣନା DftStageMode
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_FULL = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

ଗଣନା { DFT_MAX_STAGES = 40 };

ସଂରଚନା DftStage
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା radix;          // 3, 4, 5 or an odd number >= 7
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା span;           // L: length of the sub-DFTs combined by this stage
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngroups;        // G = nc / (radix*span)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;           // DftStageMode
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    ଅଚଳ ଶୂନ୍ୟ* tw_re;
    ଅଚଳ ଶୂନ୍ୟ* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    ଅଚଳ ଶୂନ୍ୟ* cs;
    ଅଚଳ ଶୂନ୍ୟ* sn;
};

ସଂରଚନା DftPlan
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind;           // DftKind
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nc;             // complex length actually run through the stages (n/2 for even real kinds)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;          // CV_32F or CV_64F
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    ତାର୍କିକ real_ନିବେଶ;    // R2C with odd n: gather real samples, im = 0
    ତାର୍କିକ pingpong;      // stages must alternate between two workspace pairs
    ତାର୍କିକ need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> itab;
    AutoBuffer<ଦ୍ୱିଗୁଣ> tw;      // storage for all the tables below (type T, 8-byte aligned)
    ଅଚଳ ଶୂନ୍ୟ* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    ଅଚଳ ଶୂନ୍ୟ* rtw_im;
    ଅଚଳ ଶୂନ୍ୟ* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    ଅଚଳ ଶୂନ୍ୟ* dct_im;
    size_t ws_bytes;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : kind(0), n(0), nc(0), depth(0), vl(0), first_radix(1), real_ନିବେଶ(ମିଥ୍ୟା),
                pingpong(ମିଥ୍ୟା), need_tmp(ମିଥ୍ୟା), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_bytes(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    ଶୂନ୍ୟ build(ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା vl);

ବ୍ୟକ୍ତିଗତ:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    template<typename T> ଶୂନ୍ୟ fillTables(size_t ntw, ତାର୍କିକ need_rtw, ତାର୍କିକ need_dct);

    DftPlan(ଅଚଳ DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& operator=(ଅଚଳ DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*DftPreprocFunc)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* src, size_t sstep,
                               ଶୂନ୍ୟ* re, ଶୂନ୍ୟ* im, ତାର୍କିକ conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*DftStageFunc)(ଅଚଳ DftStage& st, ଅଚଳ ଶୂନ୍ୟ* sre, ଅଚଳ ଶୂନ୍ୟ* sim,
                             ଶୂନ୍ୟ* dre, ଶୂନ୍ୟ* dim, ଶୂନ୍ୟ* scratch);

ସଂରଚନା DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    ଶୂନ୍ୟ (*preprocCCS)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* src, ଶୂନ୍ୟ* tmp, ତାର୍କିକ complex_ନିବେଶ);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    ଶୂନ୍ୟ (*preprocDCT)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* src, size_t sstep, ଶୂନ୍ୟ* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    ଶୂନ୍ୟ (*preprocIDCT)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* src, size_t sstep, ଶୂନ୍ୟ* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    ଶୂନ୍ୟ (*postprocDFT)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* re, ଅଚଳ ଶୂନ୍ୟ* im, ଶୂନ୍ୟ* dst,
                        ଦ୍ୱିଗୁଣ re_scale, ଦ୍ୱିଗୁଣ im_scale);          // split -> interleaved complex
    ଶୂନ୍ୟ (*postprocRealDFT)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* re, ଅଚଳ ଶୂନ୍ୟ* im, ଶୂନ୍ୟ* dst,
                            ଦ୍ୱିଗୁଣ scale, ତାର୍କିକ complex_output);     // R2C untangle + CCS/complex packing
    ଶୂନ୍ୟ (*postprocReal)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* re, ଶୂନ୍ୟ* dst, ଦ୍ୱିଗୁଣ scale); // re only
    ଶୂନ୍ୟ (*postprocDCT)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* re, ଅଚଳ ଶୂନ୍ୟ* im, ଶୂନ୍ୟ* dst, size_t dstep);
    ଶୂନ୍ୟ (*postprocIDCT)(ଅଚଳ DftPlan& plan, ଅଚଳ ଶୂନ୍ୟ* re, ଅଚଳ ଶୂନ୍ୟ* im, ଶୂନ୍ୟ* dst, size_t dstep);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#endif
