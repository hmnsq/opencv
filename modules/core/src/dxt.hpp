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

#اگر_غیر_تعریف_شدہ OPENCV_CORE_SRC_DXT_HPP
#تعریف_کرو OPENCV_CORE_SRC_DXT_HPP

#شامل "opencv2/core/utility.hpp"

نامخلا cv {

شمار Dft_قسم
{
    DFT_قسم_C2_C = 0,   // complex -> complex (forward or inverse)
    DFT_قسم_R2_C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_قسم_C2_R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_قسم_DCT,       // DCT-II, even n only
    DFT_قسم_IDCT       // DCT-III, even n only
};

شمار Dft_Stage_منوال
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_بھرا = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

شمار { DFT_MAX_STAGES = 40 };

ساخت DftStage
{
    صحیح radix;          // 3, 4, 5 or an odd number >= 7
    صحیح وسعت;           // L: length of the sub-DFTs combined by this stage
    صحیح ngroups;        // G = nc / (radix*span)
    صحیح nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    صحیح منوال;           // DftStageMode
    صحیح tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    ثابت خالی* tw_re;
    ثابت خالی* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    ثابت خالی* cs;
    ثابت خالی* sn;
};

ساخت DftPlan
{
    صحیح قسم;           // DftKind
    صحیح n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    صحیح nc;             // complex length actually run through the stages (n/2 for even real kinds)
    صحیح گہرائی;          // CV_32F or CV_64F
    صحیح vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    صحیح پہلا_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    منطقی real_داخلہ;    // R2C with odd n: gather real samples, im = 0
    منطقی pingpong;      // stages must alternate between two workspace pairs
    منطقی need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    صحیح nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    Auto_بفر<صحیح> itab;
    Auto_بفر<دوہرا> tw;      // storage for all the tables below (type T, 8-byte aligned)
    ثابت خالی* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    ثابت خالی* rtw_im;
    ثابت خالی* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    ثابت خالی* dct_im;
    size_t ws_بائٹیں;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : قسم(0), n(0), nc(0), گہرائی(0), vl(0), پہلا_radix(1), real_داخلہ(جھوٹ),
                pingpong(جھوٹ), need_tmp(جھوٹ), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_بائٹیں(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    خالی build(صحیح قسم, صحیح n, صحیح گہرائی, صحیح vl);

نجی:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    سانچہ<قسم_نام T> خالی fillTables(size_t ntw, منطقی need_rtw, منطقی need_dct);

    DftPlan(ثابت DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& عامل=(ثابت DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
قسمتعریف خالی (*DftPreprocFunc)(ثابت DftPlan& plan, ثابت خالی* src, size_t sstep,
                               خالی* re, خالی* im, منطقی conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
قسمتعریف خالی (*DftStageFunc)(ثابت DftStage& st, ثابت خالی* sre, ثابت خالی* sim,
                             خالی* dre, خالی* dim, خالی* scratch);

ساخت DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    خالی (*preprocCCS)(ثابت DftPlan& plan, ثابت خالی* src, خالی* tmp, منطقی complex_داخلہ);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    خالی (*preprocDCT)(ثابت DftPlan& plan, ثابت خالی* src, size_t sstep, خالی* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    خالی (*preprocIDCT)(ثابت DftPlan& plan, ثابت خالی* src, size_t sstep, خالی* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    خالی (*postprocDFT)(ثابت DftPlan& plan, ثابت خالی* re, ثابت خالی* im, خالی* dst,
                        دوہرا re_scale, دوہرا im_scale);          // split -> interleaved complex
    خالی (*postprocRealDFT)(ثابت DftPlan& plan, ثابت خالی* re, ثابت خالی* im, خالی* dst,
                            دوہرا scale, منطقی complex_output);     // R2C untangle + CCS/complex packing
    خالی (*postprocReal)(ثابت DftPlan& plan, ثابت خالی* re, خالی* dst, دوہرا scale); // re only
    خالی (*postprocDCT)(ثابت DftPlan& plan, ثابت خالی* re, ثابت خالی* im, خالی* dst, size_t dstep);
    خالی (*postprocIDCT)(ثابت DftPlan& plan, ثابت خالی* re, ثابت خالی* im, خالی* dst, size_t dstep);
    صحیح vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#اگر_ختم
