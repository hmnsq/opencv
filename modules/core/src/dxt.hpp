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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_SRC_DXT_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_SRC_DXT_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utility.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਗਿਣਤੀ Dft_ਕਿਸਮ
{
    DFT_ਕਿਸਮ_C2_C = 0,   // complex -> complex (forward or inverse)
    DFT_ਕਿਸਮ_R2_C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_ਕਿਸਮ_C2_R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_ਕਿਸਮ_DCT,       // DCT-II, even n only
    DFT_ਕਿਸਮ_IDCT       // DCT-III, even n only
};

ਗਿਣਤੀ Dft_Stage_ਬਹੁਲਕ
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_ਭਰਿਆ = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

ਗਿਣਤੀ { DFT_MAX_STAGES = 40 };

ਬਣਤਰ DftStage
{
    ਪੂਰਨ radix;          // 3, 4, 5 or an odd number >= 7
    ਪੂਰਨ ਵਿਸਤਾਰ;           // L: length of the sub-DFTs combined by this stage
    ਪੂਰਨ ngroups;        // G = nc / (radix*span)
    ਪੂਰਨ nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    ਪੂਰਨ ਬਹੁਲਕ;           // DftStageMode
    ਪੂਰਨ tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    ਅਚਲ ਖਾਲੀ* tw_re;
    ਅਚਲ ਖਾਲੀ* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    ਅਚਲ ਖਾਲੀ* cs;
    ਅਚਲ ਖਾਲੀ* sn;
};

ਬਣਤਰ DftPlan
{
    ਪੂਰਨ ਕਿਸਮ;           // DftKind
    ਪੂਰਨ n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    ਪੂਰਨ nc;             // complex length actually run through the stages (n/2 for even real kinds)
    ਪੂਰਨ ਡੂੰਘਾਈ;          // CV_32F or CV_64F
    ਪੂਰਨ vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    ਪੂਰਨ ਪਹਿਲਾ_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    ਤਾਰਕਿਕ real_ਨਿਵੇਸ਼;    // R2C with odd n: gather real samples, im = 0
    ਤਾਰਕਿਕ pingpong;      // stages must alternate between two workspace pairs
    ਤਾਰਕਿਕ need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    ਪੂਰਨ nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    Auto_ਬਫਰ<ਪੂਰਨ> itab;
    Auto_ਬਫਰ<ਦੁੱਗਣਾ> tw;      // storage for all the tables below (type T, 8-byte aligned)
    ਅਚਲ ਖਾਲੀ* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    ਅਚਲ ਖਾਲੀ* rtw_im;
    ਅਚਲ ਖਾਲੀ* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    ਅਚਲ ਖਾਲੀ* dct_im;
    size_t ws_ਬਾਈਟਾਂ;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : ਕਿਸਮ(0), n(0), nc(0), ਡੂੰਘਾਈ(0), vl(0), ਪਹਿਲਾ_radix(1), real_ਨਿਵੇਸ਼(ਝੂਠ),
                pingpong(ਝੂਠ), need_tmp(ਝੂਠ), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_ਬਾਈਟਾਂ(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    ਖਾਲੀ build(ਪੂਰਨ ਕਿਸਮ, ਪੂਰਨ n, ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ vl);

ਨਿਜੀ:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਖਾਲੀ fillTables(size_t ntw, ਤਾਰਕਿਕ need_rtw, ਤਾਰਕਿਕ need_dct);

    DftPlan(ਅਚਲ DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& ਸੰਚਾਲਕ=(ਅਚਲ DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*DftPreprocFunc)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* src, size_t sstep,
                               ਖਾਲੀ* re, ਖਾਲੀ* im, ਤਾਰਕਿਕ conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*DftStageFunc)(ਅਚਲ DftStage& st, ਅਚਲ ਖਾਲੀ* sre, ਅਚਲ ਖਾਲੀ* sim,
                             ਖਾਲੀ* dre, ਖਾਲੀ* dim, ਖਾਲੀ* scratch);

ਬਣਤਰ DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    ਖਾਲੀ (*preprocCCS)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* src, ਖਾਲੀ* tmp, ਤਾਰਕਿਕ complex_ਨਿਵੇਸ਼);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    ਖਾਲੀ (*preprocDCT)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* src, size_t sstep, ਖਾਲੀ* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    ਖਾਲੀ (*preprocIDCT)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* src, size_t sstep, ਖਾਲੀ* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    ਖਾਲੀ (*postprocDFT)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* re, ਅਚਲ ਖਾਲੀ* im, ਖਾਲੀ* dst,
                        ਦੁੱਗਣਾ re_scale, ਦੁੱਗਣਾ im_scale);          // split -> interleaved complex
    ਖਾਲੀ (*postprocRealDFT)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* re, ਅਚਲ ਖਾਲੀ* im, ਖਾਲੀ* dst,
                            ਦੁੱਗਣਾ scale, ਤਾਰਕਿਕ complex_output);     // R2C untangle + CCS/complex packing
    ਖਾਲੀ (*postprocReal)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* re, ਖਾਲੀ* dst, ਦੁੱਗਣਾ scale); // re only
    ਖਾਲੀ (*postprocDCT)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* re, ਅਚਲ ਖਾਲੀ* im, ਖਾਲੀ* dst, size_t dstep);
    ਖਾਲੀ (*postprocIDCT)(ਅਚਲ DftPlan& plan, ਅਚਲ ਖਾਲੀ* re, ਅਚਲ ਖਾਲੀ* im, ਖਾਲੀ* dst, size_t dstep);
    ਪੂਰਨ vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#ਜੇ_ਅੰਤ
