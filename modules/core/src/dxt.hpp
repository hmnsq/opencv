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

#నిర్వచితం_కాకపోతే OPENCV_CORE_SRC_DXT_HPP
#నిర్వచించు OPENCV_CORE_SRC_DXT_HPP

#అన్తర్భావ "opencv2/core/utility.hpp"

నామాకాశ cv {

గణన Dft_రకం
{
    DFT_రకం_C2_C = 0,   // complex -> complex (forward or inverse)
    DFT_రకం_R2_C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_రకం_C2_R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_రకం_DCT,       // DCT-II, even n only
    DFT_రకం_IDCT       // DCT-III, even n only
};

గణన Dft_Stage_బహుళకం
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_నిండిన = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

గణన { DFT_MAX_STAGES = 40 };

నిర్మాణం DftStage
{
    పూర్ణాంకం radix;          // 3, 4, 5 or an odd number >= 7
    పూర్ణాంకం పరిధి;           // L: length of the sub-DFTs combined by this stage
    పూర్ణాంకం ngroups;        // G = nc / (radix*span)
    పూర్ణాంకం nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    పూర్ణాంకం బహుళకం;           // DftStageMode
    పూర్ణాంకం tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    స్థిరరాశి శూన్య* tw_re;
    స్థిరరాశి శూన్య* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    స్థిరరాశి శూన్య* cs;
    స్థిరరాశి శూన్య* sn;
};

నిర్మాణం DftPlan
{
    పూర్ణాంకం రకం;           // DftKind
    పూర్ణాంకం n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    పూర్ణాంకం nc;             // complex length actually run through the stages (n/2 for even real kinds)
    పూర్ణాంకం లోతు;          // CV_32F or CV_64F
    పూర్ణాంకం vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    పూర్ణాంకం మొదటి_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    తార్కిక real_ఆదానం;    // R2C with odd n: gather real samples, im = 0
    తార్కిక pingpong;      // stages must alternate between two workspace pairs
    తార్కిక need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    పూర్ణాంకం nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    Auto_బఫరు<పూర్ణాంకం> itab;
    Auto_బఫరు<ద్విగుణ> tw;      // storage for all the tables below (type T, 8-byte aligned)
    స్థిరరాశి శూన్య* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    స్థిరరాశి శూన్య* rtw_im;
    స్థిరరాశి శూన్య* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    స్థిరరాశి శూన్య* dct_im;
    size_t ws_బైట్లు;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : రకం(0), n(0), nc(0), లోతు(0), vl(0), మొదటి_radix(1), real_ఆదానం(అబద్ధం),
                pingpong(అబద్ధం), need_tmp(అబద్ధం), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_బైట్లు(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    శూన్య build(పూర్ణాంకం రకం, పూర్ణాంకం n, పూర్ణాంకం లోతు, పూర్ణాంకం vl);

అంతరంగ:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    మూస<రకం_పేరు T> శూన్య fillTables(size_t ntw, తార్కిక need_rtw, తార్కిక need_dct);

    DftPlan(స్థిరరాశి DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& క్రియాకారకం=(స్థిరరాశి DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
రకనిర్వచనం శూన్య (*DftPreprocFunc)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* src, size_t sstep,
                               శూన్య* re, శూన్య* im, తార్కిక conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
రకనిర్వచనం శూన్య (*DftStageFunc)(స్థిరరాశి DftStage& st, స్థిరరాశి శూన్య* sre, స్థిరరాశి శూన్య* sim,
                             శూన్య* dre, శూన్య* dim, శూన్య* scratch);

నిర్మాణం DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    శూన్య (*preprocCCS)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* src, శూన్య* tmp, తార్కిక complex_ఆదానం);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    శూన్య (*preprocDCT)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* src, size_t sstep, శూన్య* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    శూన్య (*preprocIDCT)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* src, size_t sstep, శూన్య* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    శూన్య (*postprocDFT)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* re, స్థిరరాశి శూన్య* im, శూన్య* dst,
                        ద్విగుణ re_scale, ద్విగుణ im_scale);          // split -> interleaved complex
    శూన్య (*postprocRealDFT)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* re, స్థిరరాశి శూన్య* im, శూన్య* dst,
                            ద్విగుణ scale, తార్కిక complex_output);     // R2C untangle + CCS/complex packing
    శూన్య (*postprocReal)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* re, శూన్య* dst, ద్విగుణ scale); // re only
    శూన్య (*postprocDCT)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* re, స్థిరరాశి శూన్య* im, శూన్య* dst, size_t dstep);
    శూన్య (*postprocIDCT)(స్థిరరాశి DftPlan& plan, స్థిరరాశి శూన్య* re, స్థిరరాశి శూన్య* im, శూన్య* dst, size_t dstep);
    పూర్ణాంకం vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#అయితే_ముగింపు
