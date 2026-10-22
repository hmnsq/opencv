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

#অন্তর্ভাৱ "opencv2/core/utility.hpp"

নামাকাশ cv {

গণনা DftKind
{
    DFT_KIND_C2C = 0,   // complex -> complex (forward or inverse)
    DFT_KIND_R2C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_KIND_C2R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_KIND_DCT,       // DCT-II, even n only
    DFT_KIND_IDCT       // DCT-III, even n only
};

গণনা DftStageMode
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_FULL = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

গণনা { DFT_MAX_STAGES = 40 };

সংরচনা DftStage
{
    পূর্ণাঙ্ক radix;          // 3, 4, 5 or an odd number >= 7
    পূর্ণাঙ্ক span;           // L: length of the sub-DFTs combined by this stage
    পূর্ণাঙ্ক ngroups;        // G = nc / (radix*span)
    পূর্ণাঙ্ক nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    পূর্ণাঙ্ক mode;           // DftStageMode
    পূর্ণাঙ্ক tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    ধ্রুৱ শূন্য* tw_re;
    ধ্রুৱ শূন্য* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    ধ্রুৱ শূন্য* cs;
    ধ্রুৱ শূন্য* sn;
};

সংরচনা DftPlan
{
    পূর্ণাঙ্ক kind;           // DftKind
    পূর্ণাঙ্ক n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    পূর্ণাঙ্ক nc;             // complex length actually run through the stages (n/2 for even real kinds)
    পূর্ণাঙ্ক depth;          // CV_32F or CV_64F
    পূর্ণাঙ্ক vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    পূর্ণাঙ্ক first_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    তার্কিক real_প্ৰবেশ;    // R2C with odd n: gather real samples, im = 0
    তার্কিক pingpong;      // stages must alternate between two workspace pairs
    তার্কিক need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    পূর্ণাঙ্ক nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    AutoBuffer<পূর্ণাঙ্ক> itab;
    AutoBuffer<দ্ৱিগুণ> tw;      // storage for all the tables below (type T, 8-byte aligned)
    ধ্রুৱ শূন্য* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    ধ্রুৱ শূন্য* rtw_im;
    ধ্রুৱ শূন্য* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    ধ্রুৱ শূন্য* dct_im;
    size_t ws_bytes;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : kind(0), n(0), nc(0), depth(0), vl(0), first_radix(1), real_প্ৰবেশ(মিছা),
                pingpong(মিছা), need_tmp(মিছা), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_bytes(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    শূন্য build(পূর্ণাঙ্ক kind, পূর্ণাঙ্ক n, পূর্ণাঙ্ক depth, পূর্ণাঙ্ক vl);

গুপ্ত:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    template<typename T> শূন্য fillTables(size_t ntw, তার্কিক need_rtw, তার্কিক need_dct);

    DftPlan(ধ্রুৱ DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& operator=(ধ্রুৱ DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
প্রকারনির্দেশ শূন্য (*DftPreprocFunc)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* src, size_t sstep,
                               শূন্য* re, শূন্য* im, তার্কিক conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
প্রকারনির্দেশ শূন্য (*DftStageFunc)(ধ্রুৱ DftStage& st, ধ্রুৱ শূন্য* sre, ধ্রুৱ শূন্য* sim,
                             শূন্য* dre, শূন্য* dim, শূন্য* scratch);

সংরচনা DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    শূন্য (*preprocCCS)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* src, শূন্য* tmp, তার্কিক complex_প্ৰবেশ);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    শূন্য (*preprocDCT)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* src, size_t sstep, শূন্য* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    শূন্য (*preprocIDCT)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* src, size_t sstep, শূন্য* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    শূন্য (*postprocDFT)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* re, ধ্রুৱ শূন্য* im, শূন্য* dst,
                        দ্ৱিগুণ re_scale, দ্ৱিগুণ im_scale);          // split -> interleaved complex
    শূন্য (*postprocRealDFT)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* re, ধ্রুৱ শূন্য* im, শূন্য* dst,
                            দ্ৱিগুণ scale, তার্কিক complex_output);     // R2C untangle + CCS/complex packing
    শূন্য (*postprocReal)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* re, শূন্য* dst, দ্ৱিগুণ scale); // re only
    শূন্য (*postprocDCT)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* re, ধ্রুৱ শূন্য* im, শূন্য* dst, size_t dstep);
    শূন্য (*postprocIDCT)(ধ্রুৱ DftPlan& plan, ধ্রুৱ শূন্য* re, ধ্রুৱ শূন্য* im, শূন্য* dst, size_t dstep);
    পূর্ণাঙ্ক vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#endif
