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

#અન્તર્ભાવ "opencv2/core/utility.hpp"

નામાકાશ cv {

ગણના DftKind
{
    DFT_KIND_C2C = 0,   // complex -> complex (forward or inverse)
    DFT_KIND_R2C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_KIND_C2R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_KIND_DCT,       // DCT-II, even n only
    DFT_KIND_IDCT       // DCT-III, even n only
};

ગણના DftStageMode
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_FULL = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

ગણના { DFT_MAX_STAGES = 40 };

સંરચના DftStage
{
    પૂર્ણાંક radix;          // 3, 4, 5 or an odd number >= 7
    પૂર્ણાંક span;           // L: length of the sub-DFTs combined by this stage
    પૂર્ણાંક ngroups;        // G = nc / (radix*span)
    પૂર્ણાંક nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    પૂર્ણાંક mode;           // DftStageMode
    પૂર્ણાંક tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    અચલ શૂન્ય* tw_re;
    અચલ શૂન્ય* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    અચલ શૂન્ય* cs;
    અચલ શૂન્ય* sn;
};

સંરચના DftPlan
{
    પૂર્ણાંક kind;           // DftKind
    પૂર્ણાંક n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    પૂર્ણાંક nc;             // complex length actually run through the stages (n/2 for even real kinds)
    પૂર્ણાંક depth;          // CV_32F or CV_64F
    પૂર્ણાંક vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    પૂર્ણાંક first_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    તાર્કિક real_આગત;    // R2C with odd n: gather real samples, im = 0
    તાર્કિક pingpong;      // stages must alternate between two workspace pairs
    તાર્કિક need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    પૂર્ણાંક nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    AutoBuffer<પૂર્ણાંક> itab;
    AutoBuffer<દ્વિગુણ> tw;      // storage for all the tables below (type T, 8-byte aligned)
    અચલ શૂન્ય* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    અચલ શૂન્ય* rtw_im;
    અચલ શૂન્ય* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    અચલ શૂન્ય* dct_im;
    size_t ws_bytes;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : kind(0), n(0), nc(0), depth(0), vl(0), first_radix(1), real_આગત(ખોટું),
                pingpong(ખોટું), need_tmp(ખોટું), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_bytes(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    શૂન્ય build(પૂર્ણાંક kind, પૂર્ણાંક n, પૂર્ણાંક depth, પૂર્ણાંક vl);

ખાનગી:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    template<typename T> શૂન્ય fillTables(size_t ntw, તાર્કિક need_rtw, તાર્કિક need_dct);

    DftPlan(અચલ DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& operator=(અચલ DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
પ્રકારવ્યાખ્યા શૂન્ય (*DftPreprocFunc)(અચલ DftPlan& plan, અચલ શૂન્ય* src, size_t sstep,
                               શૂન્ય* re, શૂન્ય* im, તાર્કિક conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
પ્રકારવ્યાખ્યા શૂન્ય (*DftStageFunc)(અચલ DftStage& st, અચલ શૂન્ય* sre, અચલ શૂન્ય* sim,
                             શૂન્ય* dre, શૂન્ય* dim, શૂન્ય* scratch);

સંરચના DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    શૂન્ય (*preprocCCS)(અચલ DftPlan& plan, અચલ શૂન્ય* src, શૂન્ય* tmp, તાર્કિક complex_આગત);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    શૂન્ય (*preprocDCT)(અચલ DftPlan& plan, અચલ શૂન્ય* src, size_t sstep, શૂન્ય* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    શૂન્ય (*preprocIDCT)(અચલ DftPlan& plan, અચલ શૂન્ય* src, size_t sstep, શૂન્ય* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    શૂન્ય (*postprocDFT)(અચલ DftPlan& plan, અચલ શૂન્ય* re, અચલ શૂન્ય* im, શૂન્ય* dst,
                        દ્વિગુણ re_scale, દ્વિગુણ im_scale);          // split -> interleaved complex
    શૂન્ય (*postprocRealDFT)(અચલ DftPlan& plan, અચલ શૂન્ય* re, અચલ શૂન્ય* im, શૂન્ય* dst,
                            દ્વિગુણ scale, તાર્કિક complex_output);     // R2C untangle + CCS/complex packing
    શૂન્ય (*postprocReal)(અચલ DftPlan& plan, અચલ શૂન્ય* re, શૂન્ય* dst, દ્વિગુણ scale); // re only
    શૂન્ય (*postprocDCT)(અચલ DftPlan& plan, અચલ શૂન્ય* re, અચલ શૂન્ય* im, શૂન્ય* dst, size_t dstep);
    શૂન્ય (*postprocIDCT)(અચલ DftPlan& plan, અચલ શૂન્ય* re, અચલ શૂન્ય* im, શૂન્ય* dst, size_t dstep);
    પૂર્ણાંક vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#endif
