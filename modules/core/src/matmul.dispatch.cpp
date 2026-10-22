/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
// Copyright (C) 2014-2015, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/opencl/runtime/opencl_core.hpp"
#ଅନ୍ତର୍ଭାଵ "intel_gpu_gemm.inl.hpp"

#ଅନ୍ତର୍ଭାଵ "matmul.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ifdef HAVE_CLAMDBLAS

ସ୍ଥିର ତାର୍କିକ ocl_gemm_amdblas( ନିବେଶ_Array matA, ନିବେଶ_Array matB, ଦ୍ୱିଗୁଣ alpha,
                      ନିବେଶ_Array matC, ଦ୍ୱିଗୁଣ beta, OutputArray matD, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = matA.type(), esz = CV_ELEM_SIZE(type);
    ତାର୍କିକ haveC = matC.kind() != cv::ନିବେଶ_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    ତାର୍କିକ atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ଯଦି (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    ଯଦି (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    ଯଦି (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    ଯଦି ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    ଯଦି (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଯଦି (haveC)
    {
        UMat C = matC.getUMat();
        ଯଦି (!ocl::internal::isCLBuffer(C))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଯଦି (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    ଅନ୍ୟଥା
        D.setTo(Scalar::all(0));

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା M = sizeD.height, N = sizeD.width, K = sizeA.width;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)A.step / esz, ldb = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)B.step / esz, ldc = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)D.step / esz;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offa = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)A.offset / esz, offb = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)B.offset / esz, offc = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    ଯଦି (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (ଅଚଳ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ଅଚଳ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ଖାଲି, ଖାଲି);
    ଅନ୍ୟଥା ଯଦି (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (ଅଚଳ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ଅଚଳ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ଖାଲି, ଖାଲି);
    ଅନ୍ୟଥା ଯଦି (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (ଅଚଳ cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ଅଚଳ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, ଖାଲି, ଖାଲି);
    }
    ଅନ୍ୟଥା ଯଦି (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (ଅଚଳ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ଅଚଳ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ଖାଲି, ଖାଲି);
    }
    ଅନ୍ୟଥା
        CV_Error(Error::StsUnsupportedFormat, "");

    ଫେରନ୍ତୁ status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ ocl_gemm( ନିବେଶ_Array matA, ନିବେଶ_Array matB, ଦ୍ୱିଗୁଣ alpha,
                      ନିବେଶ_Array matC, ଦ୍ୱିଗୁଣ beta, OutputArray matD, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = matA.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_MAT_DEPTH(type);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ଅଚଳ ocl::Device & dev = ocl::Device::getDefault();
    ତାର୍କିକ doubleSupport = dev.doubleFPConfig() > 0;

    ଯଦି (!doubleSupport && depth == CV_64F)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ତାର୍କିକ haveC = matC.kind() != cv::ନିବେଶ_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    ତାର୍କିକ atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ଯଦି (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    ଯଦି (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    ଯଦି (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    ଯଦି (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    ଯଦି (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    ତାର୍କିକ isPropagatedC2D = ମିଥ୍ୟା;  // D content is updated with C / C.t()

    ଯଦି (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        ଯଦି (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = ସତ୍ୟ;
        }
        ଅନ୍ୟଥା
        {
            beta = 0.0;
        }

        ତାର୍କିକ res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        ଯଦି (res)
            ଫେରନ୍ତୁ ସତ୍ୟ;
        // fallback on generic OpenCL code
    }

    ଯଦି (sizeD.width < 8 || sizeD.height < 8)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    String opts;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wg_size = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dev.maxWorkGroupSize();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    ଯଦି (atrans)
        A = A.t();

    ଯଦି (btrans)
        B = B.t();

    ଯଦି (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    ଅନ୍ୟଥା
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (ଦଶମିକ)alpha, (ଦଶମିକ)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    ଫେରନ୍ତୁ k.run(2, globalsize, block_size !=1 ? localsize : ଖାଲି, ମିଥ୍ୟା);
}
#endif


ନାମାକାଶ hal {

ଶୂନ୍ୟ gemm32f(ଅଚଳ ଦଶମିକ* src1, size_t src1_step, ଅଚଳ ଦଶମିକ* src2, size_t src2_step,
             ଦଶମିକ alpha, ଅଚଳ ଦଶମିକ* src3, size_t src3_step, ଦଶମିକ beta, ଦଶମିକ* dst, size_t dst_step,
             ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ଅନ୍ୟଥା
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ଶୂନ୍ୟ gemm64f(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, size_t src1_step, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, size_t src2_step,
             ଦ୍ୱିଗୁଣ alpha, ଅଚଳ ଦ୍ୱିଗୁଣ* src3, size_t src3_step, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ* dst, size_t dst_step,
             ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ଅନ୍ୟଥା
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ଶୂନ୍ୟ gemm32fc(ଅଚଳ ଦଶମିକ* src1, size_t src1_step, ଅଚଳ ଦଶମିକ* src2, size_t src2_step,
              ଦଶମିକ alpha, ଅଚଳ ଦଶମିକ* src3, size_t src3_step, ଦଶମିକ beta, ଦଶମିକ* dst, size_t dst_step,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ଅନ୍ୟଥା
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ଶୂନ୍ୟ gemm64fc(ଅଚଳ ଦ୍ୱିଗୁଣ* src1, size_t src1_step, ଅଚଳ ଦ୍ୱିଗୁଣ* src2, size_t src2_step,
              ଦ୍ୱିଗୁଣ alpha, ଅଚଳ ଦ୍ୱିଗୁଣ* src3, size_t src3_step, ଦ୍ୱିଗୁଣ beta, ଦ୍ୱିଗୁଣ* dst, size_t dst_step,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା m_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ଅନ୍ୟଥା
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

ଶୂନ୍ୟ gemm(ନିବେଶ_Array matA, ନିବେଶ_Array matB, ଦ୍ୱିଗୁଣ alpha,
          ନିବେଶ_Array matC, ଦ୍ୱିଗୁଣ beta, OutputArray _matD, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ifdef HAVE_CLAMDBLAS
    CV_OCL_RUN(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.cols() > 20 && matA.rows() > 20 && matB.cols() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#endif

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    Size a_size = A.size(), d_size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    ବାଛ( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ସ୍ଥିତି 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        ଭାଙ୍ଗ;
    ସ୍ଥିତି 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        ଭାଙ୍ଗ;
    ସ୍ଥିତି 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        ଭାଙ୍ଗ;
    ସ୍ଥିତି 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        ଭାଙ୍ଗ;
    }

    ଯଦି( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    ଯଦି( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    ଯଦି( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    ଯଦି( type == CV_32FC1 )
        hal::gemm32f(A.ptr<ଦଶମିକ>(), A.step, B.ptr<ଦଶମିକ>(), B.step, static_cast<ଦଶମିକ>(alpha),
                     C.ptr<ଦଶମିକ>(), C.step, static_cast<ଦଶମିକ>(beta),
                     DProxyPtr->ptr<ଦଶମିକ>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    ଅନ୍ୟଥା ଯଦି( type == CV_64FC1 )
        hal::gemm64f(A.ptr<ଦ୍ୱିଗୁଣ>(), A.step, B.ptr<ଦ୍ୱିଗୁଣ>(), B.step, alpha,
                     C.ptr<ଦ୍ୱିଗୁଣ>(), C.step, beta,
                     DProxyPtr->ptr<ଦ୍ୱିଗୁଣ>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    ଅନ୍ୟଥା ଯଦି( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<ଦଶମିକ>(), A.step, B.ptr<ଦଶମିକ>(), B.step, static_cast<ଦଶମିକ>(alpha),
                      C.ptr<ଦଶମିକ>(), C.step, static_cast<ଦଶମିକ>(beta),
                      DProxyPtr->ptr<ଦଶମିକ>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    ଅନ୍ୟଥା
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<ଦ୍ୱିଗୁଣ>(), A.step, B.ptr<ଦ୍ୱିଗୁଣ>(), B.step, alpha,
                      C.ptr<ଦ୍ୱିଗୁଣ>(), C.step, beta,
                      DProxyPtr->ptr<ଦ୍ୱିଗୁଣ>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    ଯଦି(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ସ୍ଥିର TransformFunc getTransformFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ସ୍ଥିର TransformFunc getDiagTransformFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ transform(ନିବେଶ_Array _src, OutputArray _dst, ନିବେଶ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    ତାର୍କିକ isDiag = ମିଥ୍ୟା;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ଯଦି (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<ଦ୍ୱିଗୁଣ> _mbuf;
    ଦ୍ୱିଗୁଣ* mbuf;

    ଯଦି( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        ଯଦି( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        ଅନ୍ୟଥା
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    ଅନ୍ୟଥା
        mbuf = m.ptr<ଦ୍ୱିଗୁଣ>();

    ଯଦି( scn == dcn )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
        ଦ୍ୱିଗୁଣ eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        ଯଦି( scn == 1 )
        {
            ଦ୍ୱିଗୁଣ alpha, beta;
            ଯଦି( mtype == CV_32F )
                alpha = m.at<ଦଶମିକ>(0), beta = m.at<ଦଶମିକ>(1);
            ଅନ୍ୟଥା
                alpha = m.at<ଦ୍ୱିଗୁଣ>(0), beta = m.at<ଦ୍ୱିଗୁଣ>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            ଫେରନ୍ତୁ;
        }

        ପାଇଁ( i = 0, isDiag = ସତ୍ୟ; isDiag && i < scn; i++ )
        {
            ପାଇଁ( j = 0; isDiag && j < scn; j++ )
            {
                ଦ୍ୱିଗୁଣ v = mtype == CV_32F ? m.at<ଦଶମିକ>(i, j) : m.at<ଦ୍ୱିଗୁଣ>(i, j);
                ଯଦି( i != j && fabs(v) > eps )
                    isDiag = ମିଥ୍ୟା;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    ଅଚଳ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    ପାଇଁ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ସ୍ଥିର TransformFunc getPerspectiveTransform(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ perspectiveTransform(ନିବେଶ_Array _src, OutputArray _dst, ନିବେଶ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype = CV_64F;
    AutoBuffer<ଦ୍ୱିଗୁଣ> _mbuf;
    ଦ୍ୱିଗୁଣ* mbuf = m.ptr<ଦ୍ୱିଗୁଣ>();

    ଯଦି( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    ଅଚଳ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    ପାଇଁ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_scaleAdd( ନିବେଶ_Array _src1, ଦ୍ୱିଗୁଣ alpha, ନିବେଶ_Array _src2, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type )
{
    ଅଚଳ ocl::Device & d = ocl::Device::getDefault();

    ତାର୍କିକ doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_MAT_DEPTH(type);
    ଯଦି ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    _dst.create(size, type);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    ଅକ୍ଷର cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], ଆକାର(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], ଆକାର(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ଯଦି (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (ଦଶମିକ)alpha);
    ଅନ୍ୟଥା
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ସ୍ଥିର ScaleAddFunc getScaleAddFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ଶୂନ୍ୟ scaleAdd(ନିବେଶ_Array _src1, ଦ୍ୱିଗୁଣ alpha, ନିବେଶ_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    ଯଦି( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        ଫେରନ୍ତୁ;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    ଦଶମିକ falpha = (ଦଶମିକ)alpha;
    ଶୂନ୍ୟ* palpha = depth == CV_32F ? (ଶୂନ୍ୟ*)&falpha : (ଶୂନ୍ୟ*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    ଯଦି (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, palpha);
        ଫେରନ୍ତୁ;
    }

    ଅଚଳ Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    ପାଇଁ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

ଶୂନ୍ୟ calcCovarMatrix( ଅଚଳ Mat* data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsamples, Mat& covar, Mat& _mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz = size.width * size.height, esz = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)data[0].elemSize();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    ଯଦି( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        ଯଦି( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        ଅନ୍ୟଥା
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        ଯଦି( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        ଅନ୍ୟଥା
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    ଯଦି( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

ଶୂନ୍ୟ calcCovarMatrix( ନିବେଶ_Array _src, OutputArray _covar, ନିବେଶ_Output_Array _mean, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ctype )
{
    CV_INSTRUMENT_REGION();

    ଯଦି(_src.kind() == ନିବେଶ_Array_2::STD_VECTOR_MAT || _src.kind() == ନିବେଶ_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(src.size()), size.area(), type);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        ପାଇଁ(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        ଯଦି( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            ଯଦି( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            ଅନ୍ୟଥା
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        ଯଦି( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        ଫେରନ୍ତୁ;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    ତାର୍କିକ takeRows = (flags & cv::COVAR_ROWS) != 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = data.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    ଯଦି( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        ଯଦି( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    ଅନ୍ୟଥା
    {
        ctype = std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, _mean, takeRows ? 0 : 1, REDUCE_AVG, ctype );
        mean = _mean.getMat();
    }

    mulTransposed( data, _covar, ((flags & cv::COVAR_NORMAL) == 0) ^ takeRows,
        mean, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ସ୍ଥିର MahalanobisImplFunc getMahalanobisImplFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#ଅନ୍ୟଥା
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


ଦ୍ୱିଗୁଣ Mahalanobis(ନିବେଶ_Array _v1, ନିବେଶ_Array _v2, ନିବେଶ_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = sz.width*sz.height*v1.channels();
    AutoBuffer<ଦ୍ୱିଗୁଣ> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    ଯଦି( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    ଦ୍ୱିଗୁଣ result = func(v1, v2, icovar, buf.data(), len);
    ଫେରନ୍ତୁ std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ସ୍ଥିର MulTransposedFunc getMulTransposedFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, ତାର୍କିକ ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#ଅନ୍ୟଥା
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ଶୂନ୍ୟ mulTransposed(ନିବେଶ_Array _src, OutputArray _dst, ତାର୍କିକ ata,
                   ନିବେଶ_Array _delta, ଦ୍ୱିଗୁଣ scale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା gemm_level = 100; // boundary above which GEMM is faster.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    ଯଦି( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        ଯଦି( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    ଯଦି( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        ଅଚଳ Mat* tsrc = &src;
        ଯଦି( !delta.empty() )
        {
            ଯଦି( delta.size() == src.size() )
                subtract( src, delta, src2 );
            ଅନ୍ୟଥା
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    ଅନ୍ୟଥା
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        ଯଦି( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, ମିଥ୍ୟା );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ସ୍ଥିର ଦ୍ୱିଗୁଣ dotProd_8u(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ଅଚଳ uchar*)src1, (ଅଚଳ uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ସ୍ଥିର ଦ୍ୱିଗୁଣ dotProd_8s(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ଅଚଳ schar*)src1, (ଅଚଳ schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ସ୍ଥିର ଦ୍ୱିଗୁଣ dotProd_16u(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ଅଚଳ ushort*)src1, (ଅଚଳ ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ସ୍ଥିର ଦ୍ୱିଗୁଣ dotProd_16s(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ଅଚଳ short*)src1, (ଅଚଳ short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ସ୍ଥିର ଦ୍ୱିଗୁଣ dotProd_32s(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)src1, (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ସ୍ଥିର ଦ୍ୱିଗୁଣ dotProd_32f(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ଅଚଳ ଦଶମିକ*)src1, (ଅଚଳ ଦଶମିକ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ସ୍ଥିର ଦ୍ୱିଗୁଣ dotProd_64f(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ଅଚଳ ଦ୍ୱିଗୁଣ*)src1, (ଅଚଳ ଦ୍ୱିଗୁଣ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦ୍ୱିଗୁଣ (*DotProdFunc)(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len);

ସ୍ଥିର DotProdFunc getDotProdFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    ଫେରନ୍ତୁ dotProdTab[depth];
}

ଦ୍ୱିଗୁଣ Mat::dot(ନିବେଶ_Array _mat) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = channels();
    ଯଦି (ଏହି->dims <= 2)
    {
        ଦ୍ୱିଗୁଣ product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, ଏହି->data, ଏହି->step, mat.data, mat.step,
                     ଏହି->cols * cn, ଏହି->rows, ଏହି->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    ଯଦି( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        ଯଦି( len == (size_t)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len )
            ଫେରନ୍ତୁ func(data, mat.data, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len);
    }

    ଅଚଳ Mat* arrays[] = {ଏହି, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(it.size*cn);
    ଦ୍ୱିଗୁଣ r = 0;

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    ଫେରନ୍ତୁ r;
}


#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_dot( ନିବେଶ_Array _src1, ନିବେଶ_Array _src2, ଦ୍ୱିଗୁଣ & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    ତାର୍କିକ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ଯଦି ( !doubleSupport && depth == CV_64F )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = std::max(CV_32F, depth);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wgs2_aligned = 1;
    ଯେତେବେଳେ (wgs2_aligned < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ଅକ୍ଷର cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, ଆକାର(cvt)),
                         (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    ଯଦି (k.run(1, &globalsize, &wgs, ସତ୍ୟ))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

#endif

ଦ୍ୱିଗୁଣ UMat::dot(ନିବେଶ_Array m) ଅଚଳ
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*ଏହି) && m.type() == type());

#ifdef HAVE_OPENCL
    ଦ୍ୱିଗୁଣ r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*ଏହି, m, r), r)
#endif

    ଫେରନ୍ତୁ getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
