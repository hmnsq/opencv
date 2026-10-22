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

#அந்தர்பாவ "opencv2/core/utility.hpp"

நாமாகாஶ cv {

எண்ணிக்கை DftKind
{
    DFT_KIND_C2C = 0,   // complex -> complex (forward or inverse)
    DFT_KIND_R2C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_KIND_C2R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_KIND_DCT,       // DCT-II, even n only
    DFT_KIND_IDCT       // DCT-III, even n only
};

எண்ணிக்கை DftStageMode
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_FULL = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

எண்ணிக்கை { DFT_MAX_STAGES = 40 };

கட்டமைப்பு DftStage
{
    முழு radix;          // 3, 4, 5 or an odd number >= 7
    முழு span;           // L: length of the sub-DFTs combined by this stage
    முழு ngroups;        // G = nc / (radix*span)
    முழு nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    முழு mode;           // DftStageMode
    முழு tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    மாறிலி வெறுமை* tw_re;
    மாறிலி வெறுமை* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    மாறிலி வெறுமை* cs;
    மாறிலி வெறுமை* sn;
};

கட்டமைப்பு DftPlan
{
    முழு kind;           // DftKind
    முழு n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    முழு nc;             // complex length actually run through the stages (n/2 for even real kinds)
    முழு depth;          // CV_32F or CV_64F
    முழு vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    முழு first_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    தருக்க real_உள்ளீடு;    // R2C with odd n: gather real samples, im = 0
    தருக்க pingpong;      // stages must alternate between two workspace pairs
    தருக்க need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    முழு nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    AutoBuffer<முழு> itab;
    AutoBuffer<இரட்டை> tw;      // storage for all the tables below (type T, 8-byte aligned)
    மாறிலி வெறுமை* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    மாறிலி வெறுமை* rtw_im;
    மாறிலி வெறுமை* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    மாறிலி வெறுமை* dct_im;
    size_t ws_bytes;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : kind(0), n(0), nc(0), depth(0), vl(0), first_radix(1), real_உள்ளீடு(பொய்),
                pingpong(பொய்), need_tmp(பொய்), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_bytes(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    வெறுமை build(முழு kind, முழு n, முழு depth, முழு vl);

தனிப்பட்ட:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    template<typename T> வெறுமை fillTables(size_t ntw, தருக்க need_rtw, தருக்க need_dct);

    DftPlan(மாறிலி DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& operator=(மாறிலி DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
வகைவரை வெறுமை (*DftPreprocFunc)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* src, size_t sstep,
                               வெறுமை* re, வெறுமை* im, தருக்க conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
வகைவரை வெறுமை (*DftStageFunc)(மாறிலி DftStage& st, மாறிலி வெறுமை* sre, மாறிலி வெறுமை* sim,
                             வெறுமை* dre, வெறுமை* dim, வெறுமை* scratch);

கட்டமைப்பு DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    வெறுமை (*preprocCCS)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* src, வெறுமை* tmp, தருக்க complex_உள்ளீடு);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    வெறுமை (*preprocDCT)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* src, size_t sstep, வெறுமை* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    வெறுமை (*preprocIDCT)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* src, size_t sstep, வெறுமை* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    வெறுமை (*postprocDFT)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* re, மாறிலி வெறுமை* im, வெறுமை* dst,
                        இரட்டை re_scale, இரட்டை im_scale);          // split -> interleaved complex
    வெறுமை (*postprocRealDFT)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* re, மாறிலி வெறுமை* im, வெறுமை* dst,
                            இரட்டை scale, தருக்க complex_output);     // R2C untangle + CCS/complex packing
    வெறுமை (*postprocReal)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* re, வெறுமை* dst, இரட்டை scale); // re only
    வெறுமை (*postprocDCT)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* re, மாறிலி வெறுமை* im, வெறுமை* dst, size_t dstep);
    வெறுமை (*postprocIDCT)(மாறிலி DftPlan& plan, மாறிலி வெறுமை* re, மாறிலி வெறுமை* im, வெறுமை* dst, size_t dstep);
    முழு vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#endif
