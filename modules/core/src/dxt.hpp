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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_SRC_DXT_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_SRC_DXT_HPP

#ಅನ್ತರ್ಭಾವ "opencv2/core/utility.hpp"

ನಾಮಾಕಾಶ cv {

ಎಣಿಕೆ Dft_ಬಗೆ
{
    DFT_ಬಗೆ_C2_C = 0,   // complex -> complex (forward or inverse)
    DFT_ಬಗೆ_R2_C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_ಬಗೆ_C2_R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_ಬಗೆ_DCT,       // DCT-II, even n only
    DFT_ಬಗೆ_IDCT       // DCT-III, even n only
};

ಎಣಿಕೆ Dft_Stage_ಬಹುಲಕ
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_ತುಂಬಿದ = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

ಎಣಿಕೆ { DFT_MAX_STAGES = 40 };

ರಚನೆ DftStage
{
    ಪೂರ್ಣಾಂಕ radix;          // 3, 4, 5 or an odd number >= 7
    ಪೂರ್ಣಾಂಕ ಹರವು;           // L: length of the sub-DFTs combined by this stage
    ಪೂರ್ಣಾಂಕ ngroups;        // G = nc / (radix*span)
    ಪೂರ್ಣಾಂಕ nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;           // DftStageMode
    ಪೂರ್ಣಾಂಕ tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* tw_re;
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* cs;
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* sn;
};

ರಚನೆ DftPlan
{
    ಪೂರ್ಣಾಂಕ ಬಗೆ;           // DftKind
    ಪೂರ್ಣಾಂಕ n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    ಪೂರ್ಣಾಂಕ nc;             // complex length actually run through the stages (n/2 for even real kinds)
    ಪೂರ್ಣಾಂಕ ಆಳ;          // CV_32F or CV_64F
    ಪೂರ್ಣಾಂಕ vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    ಪೂರ್ಣಾಂಕ ಮೊದಲ_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    ತಾರ್ಕಿಕ real_ಒಳಹರಿವು;    // R2C with odd n: gather real samples, im = 0
    ತಾರ್ಕಿಕ pingpong;      // stages must alternate between two workspace pairs
    ತಾರ್ಕಿಕ need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    ಪೂರ್ಣಾಂಕ nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> itab;
    Auto_ಬಫರ್<ದ್ವಿಗುಣ> tw;      // storage for all the tables below (type T, 8-byte aligned)
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* rtw_im;
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* dct_im;
    size_t ws_ಬೈಟುಗಳು;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : ಬಗೆ(0), n(0), nc(0), ಆಳ(0), vl(0), ಮೊದಲ_radix(1), real_ಒಳಹರಿವು(ಸುಳ್ಳು),
                pingpong(ಸುಳ್ಳು), need_tmp(ಸುಳ್ಳು), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_ಬೈಟುಗಳು(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    ಶೂನ್ಯ build(ಪೂರ್ಣಾಂಕ ಬಗೆ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಆಳ, ಪೂರ್ಣಾಂಕ vl);

ಖಾಸಗಿ:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಶೂನ್ಯ fillTables(size_t ntw, ತಾರ್ಕಿಕ need_rtw, ತಾರ್ಕಿಕ need_dct);

    DftPlan(ಸ್ಥಿರಾಂಕ DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*DftPreprocFunc)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t sstep,
                               ಶೂನ್ಯ* re, ಶೂನ್ಯ* im, ತಾರ್ಕಿಕ conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*DftStageFunc)(ಸ್ಥಿರಾಂಕ DftStage& st, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* sre, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* sim,
                             ಶೂನ್ಯ* dre, ಶೂನ್ಯ* dim, ಶೂನ್ಯ* scratch);

ರಚನೆ DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    ಶೂನ್ಯ (*preprocCCS)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಶೂನ್ಯ* tmp, ತಾರ್ಕಿಕ complex_ಒಳಹರಿವು);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    ಶೂನ್ಯ (*preprocDCT)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t sstep, ಶೂನ್ಯ* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    ಶೂನ್ಯ (*preprocIDCT)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t sstep, ಶೂನ್ಯ* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    ಶೂನ್ಯ (*postprocDFT)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* re, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* im, ಶೂನ್ಯ* dst,
                        ದ್ವಿಗುಣ re_scale, ದ್ವಿಗುಣ im_scale);          // split -> interleaved complex
    ಶೂನ್ಯ (*postprocRealDFT)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* re, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* im, ಶೂನ್ಯ* dst,
                            ದ್ವಿಗುಣ scale, ತಾರ್ಕಿಕ complex_output);     // R2C untangle + CCS/complex packing
    ಶೂನ್ಯ (*postprocReal)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* re, ಶೂನ್ಯ* dst, ದ್ವಿಗುಣ scale); // re only
    ಶೂನ್ಯ (*postprocDCT)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* re, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* im, ಶೂನ್ಯ* dst, size_t dstep);
    ಶೂನ್ಯ (*postprocIDCT)(ಸ್ಥಿರಾಂಕ DftPlan& plan, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* re, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* im, ಶೂನ್ಯ* dst, size_t dstep);
    ಪೂರ್ಣಾಂಕ vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#ಆದರೆ_ಅಂತ್ಯ
