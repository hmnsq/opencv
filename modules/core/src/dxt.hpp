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

#यदि_अपरिभाषितम् OPENCV_CORE_SRC_DXT_HPP
#परिभाषय OPENCV_CORE_SRC_DXT_HPP

#अन्तर्भाव "opencv2/core/utility.hpp"

नामाकाश cv {

गणना Dft_विधा
{
    DFT_विधा_C2_C = 0,   // complex -> complex (forward or inverse)
    DFT_विधा_R2_C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_विधा_C2_R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_विधा_DCT,       // DCT-II, even n only
    DFT_विधा_IDCT       // DCT-III, even n only
};

गणना Dft_Stage_बहुलकम्
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_पूर्णम् = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

गणना { DFT_MAX_STAGES = 40 };

संरचना DftStage
{
    पूर्णाङ्क radix;          // 3, 4, 5 or an odd number >= 7
    पूर्णाङ्क विस्तृतिः;           // L: length of the sub-DFTs combined by this stage
    पूर्णाङ्क ngroups;        // G = nc / (radix*span)
    पूर्णाङ्क nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    पूर्णाङ्क बहुलकम्;           // DftStageMode
    पूर्णाङ्क tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    ध्रुव शून्य* tw_re;
    ध्रुव शून्य* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    ध्रुव शून्य* cs;
    ध्रुव शून्य* sn;
};

संरचना DftPlan
{
    पूर्णाङ्क विधा;           // DftKind
    पूर्णाङ्क n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    पूर्णाङ्क nc;             // complex length actually run through the stages (n/2 for even real kinds)
    पूर्णाङ्क गभीरता;          // CV_32F or CV_64F
    पूर्णाङ्क vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    पूर्णाङ्क first_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    तार्किक real_निवेशः;    // R2C with odd n: gather real samples, im = 0
    तार्किक pingpong;      // stages must alternate between two workspace pairs
    तार्किक need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    पूर्णाङ्क nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    Auto_बफर<पूर्णाङ्क> itab;
    Auto_बफर<द्विगुण> tw;      // storage for all the tables below (type T, 8-byte aligned)
    ध्रुव शून्य* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    ध्रुव शून्य* rtw_im;
    ध्रुव शून्य* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    ध्रुव शून्य* dct_im;
    size_t ws_अष्टकानि;            // workspace size (incl. 64 bytes for alignment)
    size_t pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    size_t scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : विधा(0), n(0), nc(0), गभीरता(0), vl(0), first_radix(1), real_निवेशः(असत्य),
                pingpong(असत्य), need_tmp(असत्य), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_अष्टकानि(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    शून्य build(पूर्णाङ्क विधा, पूर्णाङ्क n, पूर्णाङ्क गभीरता, पूर्णाङ्क vl);

गुप्त:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    प्रतिरूपम्<प्रकारनाम T> शून्य fillTables(size_t ntw, तार्किक need_rtw, तार्किक need_dct);

    DftPlan(ध्रुव DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& संक्रियकः=(ध्रुव DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
प्रकारनिर्देश शून्य (*DftPreprocFunc)(ध्रुव DftPlan& plan, ध्रुव शून्य* src, size_t sstep,
                               शून्य* re, शून्य* im, तार्किक conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
प्रकारनिर्देश शून्य (*DftStageFunc)(ध्रुव DftStage& st, ध्रुव शून्य* sre, ध्रुव शून्य* sim,
                             शून्य* dre, शून्य* dim, शून्य* scratch);

संरचना DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    शून्य (*preprocCCS)(ध्रुव DftPlan& plan, ध्रुव शून्य* src, शून्य* tmp, तार्किक complex_निवेशः);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    शून्य (*preprocDCT)(ध्रुव DftPlan& plan, ध्रुव शून्य* src, size_t sstep, शून्य* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    शून्य (*preprocIDCT)(ध्रुव DftPlan& plan, ध्रुव शून्य* src, size_t sstep, शून्य* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    शून्य (*postprocDFT)(ध्रुव DftPlan& plan, ध्रुव शून्य* re, ध्रुव शून्य* im, शून्य* dst,
                        द्विगुण re_scale, द्विगुण im_scale);          // split -> interleaved complex
    शून्य (*postprocRealDFT)(ध्रुव DftPlan& plan, ध्रुव शून्य* re, ध्रुव शून्य* im, शून्य* dst,
                            द्विगुण scale, तार्किक complex_output);     // R2C untangle + CCS/complex packing
    शून्य (*postprocReal)(ध्रुव DftPlan& plan, ध्रुव शून्य* re, शून्य* dst, द्विगुण scale); // re only
    शून्य (*postprocDCT)(ध्रुव DftPlan& plan, ध्रुव शून्य* re, ध्रुव शून्य* im, शून्य* dst, size_t dstep);
    शून्य (*postprocIDCT)(ध्रुव DftPlan& plan, ध्रुव शून्य* re, ध्रुव शून्य* im, शून्य* dst, size_t dstep);
    पूर्णाङ्क vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#यदि_अन्तः
