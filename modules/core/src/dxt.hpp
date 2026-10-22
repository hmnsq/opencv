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

#अगर_अघोषित OPENCV_CORE_SRC_DXT_HPP
#घोषणा OPENCV_CORE_SRC_DXT_HPP

#समावेश "opencv2/core/utility.hpp"

namespace cv {

क्रमागत DftKind
{
    DFT_KIND_C2C = 0,   // complex -> complex (forward or inverse)
    DFT_KIND_R2C,       // real -> CCS-packed (or "half complex") spectrum, forward only
    DFT_KIND_C2R,       // CCS-packed (or full complex) spectrum -> real, inverse only
    DFT_KIND_DCT,       // DCT-II, even n only
    DFT_KIND_IDCT       // DCT-III, even n only
};

क्रमागत DftStageMode
{
    DFT_STAGE_SCALAR = 0,   // scalar reference loop (span < VL, no SIMD for this depth, ...)
    DFT_STAGE_FULL = 1      // span >= VL: vector loop with back-off clamp (brief, section 5)
};

क्रमागत { DFT_MAX_STAGES = 40 };

काष्ठा DftStage
{
    पूर्णांक radix;          // 3, 4, 5 or an odd number >= 7
    पूर्णांक span;           // L: length of the sub-DFTs combined by this stage
    पूर्णांक ngroups;        // G = nc / (radix*span)
    पूर्णांक nsteps;         // number of (vector) iterations of the flat loop, precomputed by the plan
    पूर्णांक mode;           // DftStageMode
    पूर्णांक tw_stride;      // per-leg twiddle table length (== span for now)
    // Per-stage twiddles, split layout, leg-major: tw_re[(q-1)*tw_stride + j] = cos(-2*pi*q*j/(radix*span)),
    // q in [1, radix), j in [0, span). Type T of the plan.
    स्थिर व्योम* tw_re;
    स्थिर व्योम* tw_im;
    // radix-odd (>=7) only: cs[p*h + k] = cos(2*pi*(p+1)*(k+1)/radix), sn[...] = sin(...), h = (radix-1)/2
    स्थिर व्योम* cs;
    स्थिर व्योम* sn;
};

काष्ठा DftPlan
{
    पूर्णांक kind;           // DftKind
    पूर्णांक n;              // logical transform length (real length for R2C/C2R/DCT/IDCT)
    पूर्णांक nc;             // complex length actually run through the stages (n/2 for even real kinds)
    पूर्णांक depth;          // CV_32F or CV_64F
    पूर्णांक vl;             // vector lanes of the kernels this plan was built for (DFTKernels::vlanes)
    पूर्णांक first_radix;    // 1, 2, 4 or 8: the fused first stage done in preproc
    bool real_निवेश;    // R2C with odd n: gather real samples, im = 0
    bool pingpong;      // stages must alternate between two workspace pairs
    bool need_tmp;      // C2R / DCT / IDCT: a temp of 2*nc elements (interleaved complex or the Makhoul-permuted reals)
    पूर्णांक nstages;
    DftStage stages[DFT_MAX_STAGES];
    // first_radix == 1: 2*nc element indices into the (real-typed) source (re index, im index);
    // first_radix > 1: nc/first_radix output positions (g*first_radix) of the input columns j
    Auto_बफर<पूर्णांक> itab;
    Auto_बफर<द्विगुन> tw;      // storage for all the tables below (type T, 8-byte aligned)
    स्थिर व्योम* rtw_re;         // W_n^k = exp(-2*pi*i*k/n), k in [0, n/2]; R2C/C2R/DCT/IDCT with even n
    स्थिर व्योम* rtw_im;
    स्थिर व्योम* dct_re;         // s*W_4n^k, k in [0, n/2]; DCT/IDCT (s = DCT normalization)
    स्थिर व्योम* dct_im;
    माप_प्रकार ws_bytes;            // workspace size (incl. 64 bytes for alignment)
    माप_प्रकार pair1_ofs;           // byte offset of the second (re, im) pair or of the temp, from the aligned base
    माप_प्रकार scratch_ofs;         // byte offset of the radix-odd scratch area

    DftPlan() : kind(0), n(0), nc(0), depth(0), vl(0), first_radix(1), real_निवेश(false),
                pingpong(false), need_tmp(false), nstages(0), rtw_re(0), rtw_im(0),
                dct_re(0), dct_im(0), ws_bytes(0), pair1_ofs(0), scratch_ofs(0) {}

    // Builds the plan for a 1D transform of the given kind (DftKind) and length.
    //
    // IMPORTANT: `vl` (vector lanes) MUST come from the dispatched kernel table
    // (DFTKernels::vlanes), never from VTraits<> in dxt.cpp, which is compiled for the baseline
    // ISA and reports the wrong lane count on AVX2/AVX512/RVV builds. Stage modes and step counts
    // depend on it.
    व्योम build(पूर्णांक kind, पूर्णांक n, पूर्णांक depth, पूर्णांक vl);

निजी:
    // fills the twiddle tables (stage tables, W_n^k, DCT) into `tw`; ntw = total number of T entries
    ढाँचा<typename T> व्योम fillTables(माप_प्रकार ntw, bool need_rtw, bool need_dct);

    DftPlan(स्थिर DftPlan&);            // tables hold raw pointers into the AutoBuffers: no copies
    DftPlan& चालक=(स्थिर DftPlan&);
};

// (plan, src, src_step (in elements), re, im, conj)
प्रकार व्योम (*DftPreprocFunc)(स्थिर DftPlan& plan, स्थिर व्योम* src, माप_प्रकार sstep,
                               व्योम* re, व्योम* im, bool conj);
// (stage, src_re, src_im, dst_re, dst_im, scratch); src == dst allowed only when !plan.pingpong
प्रकार व्योम (*DftStageFunc)(स्थिर DftStage& st, स्थिर व्योम* sre, स्थिर व्योम* sim,
                             व्योम* dre, व्योम* dim, व्योम* scratch);

काष्ठा DFTKernels
{
    // group A
    DftPreprocFunc preprocRadix0;   // gather + deinterleave only
    DftPreprocFunc preprocRadix2;   // + fused twiddle-free radix-2
    DftPreprocFunc preprocRadix4;   // + fused twiddle-free radix-4
    DftPreprocFunc preprocRadix8;   // + fused twiddle-free radix-8
    // C2R: CCS (or full complex, complex_input) spectrum -> interleaved "2Z" temp (2*nc elements)
    व्योम (*preprocCCS)(स्थिर DftPlan& plan, स्थिर व्योम* src, व्योम* tmp, bool complex_निवेश);
    // DCT: Makhoul permutation of the (possibly strided) input into the temp
    व्योम (*preprocDCT)(स्थिर DftPlan& plan, स्थिर व्योम* src, माप_प्रकार sstep, व्योम* tmp);
    // IDCT: pre-twiddle + CCS untangle -> interleaved temp
    व्योम (*preprocIDCT)(स्थिर DftPlan& plan, स्थिर व्योम* src, माप_प्रकार sstep, व्योम* tmp);
    // group B
    DftStageFunc radix3, radix4, radix5, radixOdd;
    // group C
    व्योम (*postprocDFT)(स्थिर DftPlan& plan, स्थिर व्योम* re, स्थिर व्योम* im, व्योम* dst,
                        द्विगुन re_scale, द्विगुन im_scale);          // split -> interleaved complex
    व्योम (*postprocRealDFT)(स्थिर DftPlan& plan, स्थिर व्योम* re, स्थिर व्योम* im, व्योम* dst,
                            द्विगुन scale, bool complex_output);     // R2C untangle + CCS/complex packing
    व्योम (*postprocReal)(स्थिर DftPlan& plan, स्थिर व्योम* re, व्योम* dst, द्विगुन scale); // re only
    व्योम (*postprocDCT)(स्थिर DftPlan& plan, स्थिर व्योम* re, स्थिर व्योम* im, व्योम* dst, माप_प्रकार dstep);
    व्योम (*postprocIDCT)(स्थिर DftPlan& plan, स्थिर व्योम* re, स्थिर व्योम* im, व्योम* dst, माप_प्रकार dstep);
    पूर्णांक vlanes;     // VTraits<v_T>::vlanes() of the ISA these pointers belong to (1 = no SIMD)
};

}

#पूर्ण_अगर
