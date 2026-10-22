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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ <opencv2/core/utils/logger.hpp>

#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_core.hpp"
#అన్తర్భావ "intel_gpu_gemm.inl.hpp"

#అన్తర్భావ "matmul.simd.hpp"
#అన్తర్భావ "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#నిర్వచితమైతే HAVE_CLAMDBLAS

స్థిర తార్కిక ocl_gemm_amdblas( ఆదానం_Array matA, ఆదానం_Array matB, ద్విగుణ alpha,
                      ఆదానం_Array matC, ద్విగుణ beta, OutputArray matD, పూర్ణాంకం flags )
{
    పూర్ణాంకం type = matA.type(), esz = CV_ELEM_కొలత(type);
    తార్కిక haveC = matC.రకం() != cv::ఆదానం_Array_2::NONE;
    కొలత కొలత_A = matA.size(), కొలత_B = matB.size(), కొలత_C = haveC ? matC.size() : కొలత(0, 0);
    తార్కిక atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ఒకవేళ (atrans)
        కొలత_A = కొలత(కొలత_A.ఎత్తు, కొలత_A.వెడల్పు);
    ఒకవేళ (btrans)
        కొలత_B = కొలత(కొలత_B.ఎత్తు, కొలత_B.వెడల్పు);
    ఒకవేళ (haveC && ctrans)
        కొలత_C = కొలత(కొలత_C.ఎత్తు, కొలత_C.వెడల్పు);

    కొలత కొలత_D(కొలత_B.వెడల్పు, కొలత_A.ఎత్తు);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( కొలత_A.వెడల్పు == కొలత_B.ఎత్తు && (!haveC || కొలత_C == కొలత_D) );

    matD.create(కొలత_D, type);
    ఒకవేళ ( matA.offset() % esz != 0 || matA.అడుగు() % esz != 0 ||
         matB.offset() % esz != 0 || matB.అడుగు() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.అడుగు() % esz != 0)) )
        తిరిగి అబద్ధం;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    ఒకవేళ (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        తిరిగి అబద్ధం;
    }
    ఒకవేళ (haveC)
    {
        UMat C = matC.getUMat();
        ఒకవేళ (!ocl::internal::isCLBuffer(C))
            తిరిగి అబద్ధం;
    }
    ఒకవేళ (haveC)
        ctrans ? transpose(matC, D) : matC.copy_గమ్యం(D);
    లేకపోతే
        D.సమితి_గమ్యం(Scalar::all(0));

    పూర్ణాంకం M = కొలత_D.ఎత్తు, N = కొలత_D.వెడల్పు, K = కొలత_A.వెడల్పు;
    పూర్ణాంకం lda = (పూర్ణాంకం)A.అడుగు / esz, ldb = (పూర్ణాంకం)B.అడుగు / esz, ldc = (పూర్ణాంకం)D.అడుగు / esz;
    పూర్ణాంకం offa = (పూర్ణాంకం)A.offset / esz, offb = (పూర్ణాంకం)B.offset / esz, offc = (పూర్ణాంకం)D.offset / esz;

    cl_command_వరుస clq = (cl_command_వరుస)ocl::వరుస::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_క్రమం క్రమం = clblas_అడ్డువరుస_Major;
    clblas_స్థితి స్థితి_2 = clblasSuccess;

    ఒకవేళ (type == CV_32FC1)
        స్థితి_2 = clblasSgemm(క్రమం, transA, transB, M, N, K,
                             (cl_float)alpha, (స్థిరరాశి cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (స్థిరరాశి cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ఖాళీ, ఖాళీ);
    లేకపోతే ఒకవేళ (type == CV_64FC1)
        స్థితి_2 = clblasDgemm(క్రమం, transA, transB, M, N, K,
                             alpha, (స్థిరరాశి cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (స్థిరరాశి cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ఖాళీ, ఖాళీ);
    లేకపోతే ఒకవేళ (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         స్థితి_2 = clblasCgemm(క్రమం, transA, transB, M, N, K,
                              alpha_2, (స్థిరరాశి cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (స్థిరరాశి cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, ఖాళీ, ఖాళీ);
    }
    లేకపోతే ఒకవేళ (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        స్థితి_2 = clblasZgemm(క్రమం, transA, transB, M, N, K,
                             alpha_2, (స్థిరరాశి cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (స్థిరరాశి cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ఖాళీ, ఖాళీ);
    }
    లేకపోతే
        CV_Error(Error::StsUnsupportedFormat, "");

    తిరిగి స్థితి_2 == clblasSuccess;
}

#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక ocl_gemm( ఆదానం_Array matA, ఆదానం_Array matB, ద్విగుణ alpha,
                      ఆదానం_Array matC, ద్విగుణ beta, OutputArray matD, పూర్ణాంకం flags )
{
    పూర్ణాంకం type = matA.type();
    పూర్ణాంకం లోతు = CV_MAT_లోతు(type);
    పూర్ణాంకం cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    స్థిరరాశి ocl::Device & dev = ocl::Device::getDefault();
    తార్కిక doubleSupport = dev.doubleFPConfig() > 0;

    ఒకవేళ (!doubleSupport && లోతు == CV_64F)
        తిరిగి అబద్ధం;

    తార్కిక haveC = matC.రకం() != cv::ఆదానం_Array_2::NONE;
    కొలత కొలత_A = matA.size(), కొలత_B = matB.size(), కొలత_C = haveC ? matC.size() : కొలత(0, 0);
    తార్కిక atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ఒకవేళ (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    కొలత కొలత_D(((btrans) ? కొలత_B.ఎత్తు : కొలత_B.వెడల్పు),
               ((atrans) ? కొలత_A.వెడల్పు : కొలత_A.ఎత్తు));

    ఒకవేళ (atrans)
        కొలత_A = కొలత(కొలత_A.ఎత్తు, కొలత_A.వెడల్పు);
    ఒకవేళ (btrans)
        కొలత_B = కొలత(కొలత_B.ఎత్తు, కొలత_B.వెడల్పు);
    ఒకవేళ (haveC && ctrans)
        కొలత_C = కొలత(కొలత_C.ఎత్తు, కొలత_C.వెడల్పు);

    CV_CheckEQ(కొలత_A.వెడల్పు, కొలత_B.ఎత్తు, "");
    ఒకవేళ (haveC)
        CV_CheckEQ(కొలత_C, కొలత_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(కొలత_D, type);
    UMat D = matD.getUMat();

    తార్కిక isPropagatedC2D = అబద్ధం;  // D content is updated with C / C.t()

    ఒకవేళ (dev.intelSubgroupsSupport() && (లోతు == CV_32F) && cn == 1)
    {
        ఒకవేళ (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copy_గమ్యం(D);
            isPropagatedC2D = నిజం;
        }
        లేకపోతే
        {
            beta = 0.0;
        }

        తార్కిక res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, కొలత_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        ఒకవేళ (res)
            తిరిగి నిజం;
        // fallback on generic OpenCL code
    }

    ఒకవేళ (కొలత_D.వెడల్పు < 8 || కొలత_D.ఎత్తు < 8)
        తిరిగి అబద్ధం;

    String opts;

    పూర్ణాంకం wg_కొలత = (పూర్ణాంకం)dev.max_Work_సమూహం_కొలత();
    పూర్ణాంకం కొలత_Dmin = std::min(కొలత_D.వెడల్పు, కొలత_D.ఎత్తు);
    wg_కొలత = std::min(wg_కొలత, కొలత_Dmin * కొలత_Dmin);
    పూర్ణాంకం ఖండం_కొలత_3 = (wg_కొలత / (32*cn) < 32) ? (wg_కొలత / (16*cn) < 16) ? (wg_కొలత / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    ఒకవేళ (atrans)
        A = A.t();

    ఒకవేళ (btrans)
        B = B.t();

    ఒకవేళ (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_గమ్యం(D);

    పూర్ణాంకం సదిశ_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    పూర్ణాంకం kercn = ocl::check_Optimal_సదిశ_వెడల్పు(సదిశ_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_గమ్యం_Str(type), ocl::type_గమ్యం_Str(లోతు), ocl::type_గమ్యం_Str(CV_MAKETYPE(లోతు, kercn)),
                      cn, kercn, ఖండం_కొలత_3,
                      (కొలత_A.వెడల్పు % ఖండం_కొలత_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    ఒకవేళ (లోతు == CV_64F)
        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(A),
               ocl::KernelArg::Read_Only_No_కొలత(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               కొలత_A.వెడల్పు, alpha, beta);
    లేకపోతే
        k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(A),
               ocl::KernelArg::Read_Only_No_కొలత(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               కొలత_A.వెడల్పు, (దశాంశం)alpha, (దశాంశం)beta);

    size_t globalsize[2] = { (size_t)కొలత_D.వెడల్పు * cn / kercn, (size_t)కొలత_D.ఎత్తు};
    size_t localsize[2] = { (size_t)ఖండం_కొలత_3, (size_t)ఖండం_కొలత_3};

    తిరిగి k.నిరంతరం(2, globalsize, ఖండం_కొలత_3 !=1 ? localsize : ఖాళీ, అబద్ధం);
}
#అయితే_ముగింపు


నామాకాశ hal {

శూన్య gemm32f(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
             దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
             పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags)
#నిర్వచితమైతే CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags));
#లేకపోతే
    CV_CPU_DISPATCH(gemm32f, (src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#అయితే_ముగింపు
}

శూన్య gemm64f(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
             ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
             పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags)
#నిర్వచితమైతే CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags));
#లేకపోతే
    CV_CPU_DISPATCH(gemm64f, (src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#అయితే_ముగింపు
}

శూన్య gemm32fc(స్థిరరాశి దశాంశం* src1, size_t src1_అడుగు, స్థిరరాశి దశాంశం* src2, size_t src2_అడుగు,
              దశాంశం alpha, స్థిరరాశి దశాంశం* src3, size_t src3_అడుగు, దశాంశం beta, దశాంశం* dst, size_t dst_అడుగు,
              పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags)
#నిర్వచితమైతే CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags));
#లేకపోతే
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#అయితే_ముగింపు
}

శూన్య gemm64fc(స్థిరరాశి ద్విగుణ* src1, size_t src1_అడుగు, స్థిరరాశి ద్విగుణ* src2, size_t src2_అడుగు,
              ద్విగుణ alpha, స్థిరరాశి ద్విగుణ* src3, size_t src3_అడుగు, ద్విగుణ beta, ద్విగుణ* dst, size_t dst_అడుగు,
              పూర్ణాంకం m_a, పూర్ణాంకం n_a, పూర్ణాంకం n_d, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags)
#నిర్వచితమైతే CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags));
#లేకపోతే
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_అడుగు, src2, src2_అడుగు, alpha, src3, src3_అడుగు, beta, dst, dst_అడుగు, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#అయితే_ముగింపు
}

} // namespace hal

శూన్య gemm(ఆదానం_Array matA, ఆదానం_Array matB, ద్విగుణ alpha,
          ఆదానం_Array matC, ద్విగుణ beta, OutputArray _matD, పూర్ణాంకం flags)
{
#నిర్వచితమైతే HAVE_CLAMDBLAS
    CV_OCL_నిరంతరం(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.నిలువుల_సంఖ్య() > 20 && matA.అడ్డువరుసలు() > 20 && matB.నిలువుల_సంఖ్య() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENCL
    CV_OCL_నిరంతరం(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#అయితే_ముగింపు

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    కొలత a_కొలత = A.size(), d_కొలత;
    పూర్ణాంకం len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    ఎంపిక( flags & (GEMM_1_T|GEMM_2_T) )
    {
    స్థితి 0:
        d_కొలత = కొలత( B.నిలువుల_సంఖ్య, a_కొలత.ఎత్తు );
        len = B.అడ్డువరుసలు;
        CV_Assert( a_కొలత.వెడల్పు == len );
        ఆపు;
    స్థితి 1:
        d_కొలత = కొలత( B.నిలువుల_సంఖ్య, a_కొలత.వెడల్పు );
        len = B.అడ్డువరుసలు;
        CV_Assert( a_కొలత.ఎత్తు == len );
        ఆపు;
    స్థితి 2:
        d_కొలత = కొలత( B.అడ్డువరుసలు, a_కొలత.ఎత్తు );
        len = B.నిలువుల_సంఖ్య;
        CV_Assert( a_కొలత.వెడల్పు == len );
        ఆపు;
    స్థితి 3:
        d_కొలత = కొలత( B.అడ్డువరుసలు, a_కొలత.వెడల్పు );
        len = B.నిలువుల_సంఖ్య;
        CV_Assert( a_కొలత.ఎత్తు == len );
        ఆపు;
    }

    ఒకవేళ( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.అడ్డువరుసలు == d_కొలత.ఎత్తు && C.నిలువుల_సంఖ్య == d_కొలత.వెడల్పు) ||
             ((flags&GEMM_3_T) != 0 && C.అడ్డువరుసలు == d_కొలత.వెడల్పు && C.నిలువుల_సంఖ్య == d_కొలత.ఎత్తు)));
    }

    _matD.create( d_కొలత.ఎత్తు, d_కొలత.వెడల్పు, type );
    Mat D = _matD.getMat();
    ఒకవేళ( (flags & GEMM_3_T) != 0 && C.దత్తాంశం == D.దత్తాంశం )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    ఒకవేళ( D.దత్తాంశం == A.దత్తాంశం || D.దత్తాంశం == B.దత్తాంశం )
    {
        DProxy = Mat(d_కొలత.ఎత్తు, d_కొలత.వెడల్పు, D.type());
        DProxyPtr = &DProxy;
    }

    ఒకవేళ( type == CV_32FC1 )
        hal::gemm32f(A.ptr<దశాంశం>(), A.అడుగు, B.ptr<దశాంశం>(), B.అడుగు, static_cast<దశాంశం>(alpha),
                     C.ptr<దశాంశం>(), C.అడుగు, static_cast<దశాంశం>(beta),
                     DProxyPtr->ptr<దశాంశం>(), DProxyPtr->అడుగు,
                     a_కొలత.ఎత్తు, a_కొలత.వెడల్పు, DProxyPtr->నిలువుల_సంఖ్య, flags);
    లేకపోతే ఒకవేళ( type == CV_64FC1 )
        hal::gemm64f(A.ptr<ద్విగుణ>(), A.అడుగు, B.ptr<ద్విగుణ>(), B.అడుగు, alpha,
                     C.ptr<ద్విగుణ>(), C.అడుగు, beta,
                     DProxyPtr->ptr<ద్విగుణ>(), DProxyPtr->అడుగు,
                     a_కొలత.ఎత్తు, a_కొలత.వెడల్పు, DProxyPtr->నిలువుల_సంఖ్య, flags);
    లేకపోతే ఒకవేళ( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<దశాంశం>(), A.అడుగు, B.ptr<దశాంశం>(), B.అడుగు, static_cast<దశాంశం>(alpha),
                      C.ptr<దశాంశం>(), C.అడుగు, static_cast<దశాంశం>(beta),
                      DProxyPtr->ptr<దశాంశం>(), DProxyPtr->అడుగు,
                      a_కొలత.ఎత్తు, a_కొలత.వెడల్పు, DProxyPtr->నిలువుల_సంఖ్య, flags);
    లేకపోతే
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<ద్విగుణ>(), A.అడుగు, B.ptr<ద్విగుణ>(), B.అడుగు, alpha,
                      C.ptr<ద్విగుణ>(), C.అడుగు, beta,
                      DProxyPtr->ptr<ద్విగుణ>(), DProxyPtr->అడుగు,
                      a_కొలత.ఎత్తు, a_కొలత.వెడల్పు, DProxyPtr->నిలువుల_సంఖ్య, flags);
    }

    ఒకవేళ(DProxyPtr != &D)
        DProxyPtr->copy_గమ్యం(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

స్థిర TransformFunc getTransformFunc(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

స్థిర TransformFunc getDiagTransformFunc(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య transform(ఆదానం_Array _src, OutputArray _dst, ఆదానం_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    పూర్ణాంకం లోతు = src.లోతు(), scn = src.channels(), dcn = m.అడ్డువరుసలు;
    CV_Assert( scn == m.నిలువుల_సంఖ్య || scn + 1 == m.నిలువుల_సంఖ్య );
    తార్కిక isDiag = అబద్ధం;

    _dst.create_Same_కొలత( src, CV_MAKETYPE(లోతు, dcn) );
    Mat dst = _dst.getMat();

    ఒకవేళ (src.దత్తాంశం == dst.దత్తాంశం)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    పూర్ణాంకం mtype = లోతు == CV_32S || లోతు == CV_64F ? CV_64F : CV_32F;
    Auto_బఫరు<ద్విగుణ> _mbuf;
    ద్విగుణ* mbuf;

    ఒకవేళ( !m.isContinuous() || m.type() != mtype || m.నిలువుల_సంఖ్య != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.దత్తాంశం();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.మొత్తం()*tmp.elem_కొలత());
        ఒకవేళ( m.నిలువుల_సంఖ్య == scn+1 )
            m.convert_గమ్యం(tmp, mtype);
        లేకపోతే
        {
            Mat tmppart = tmp.నిలువు_Range_2(0, m.నిలువుల_సంఖ్య);
            m.convert_గమ్యం(tmppart, mtype);
        }
        m = tmp;
    }
    లేకపోతే
        mbuf = m.ptr<ద్విగుణ>();

    ఒకవేళ( scn == dcn )
    {
        పూర్ణాంకం i, j;
        ద్విగుణ eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        ఒకవేళ( scn == 1 )
        {
            ద్విగుణ alpha, beta;
            ఒకవేళ( mtype == CV_32F )
                alpha = m.at<దశాంశం>(0), beta = m.at<దశాంశం>(1);
            లేకపోతే
                alpha = m.at<ద్విగుణ>(0), beta = m.at<ద్విగుణ>(1);
            src.convert_గమ్యం(dst, dst.type(), alpha, beta);
            తిరిగి;
        }

        కొరకు( i = 0, isDiag = నిజం; isDiag && i < scn; i++ )
        {
            కొరకు( j = 0; isDiag && j < scn; j++ )
            {
                ద్విగుణ v = mtype == CV_32F ? m.at<దశాంశం>(i, j) : m.at<ద్విగుణ>(i, j);
                ఒకవేళ( i != j && fabs(v) > eps )
                    isDiag = అబద్ధం;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(లోతు): getTransformFunc(లోతు);
    CV_Assert( func != 0 );

    స్థిరరాశి Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, మొత్తం = it.size;

    కొరకు( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (పూర్ణాంకం)మొత్తం, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

స్థిర TransformFunc getPerspectiveTransform(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య perspectiveTransform(ఆదానం_Array _src, OutputArray _dst, ఆదానం_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    పూర్ణాంకం లోతు = src.లోతు(), scn = src.channels(), dcn = m.అడ్డువరుసలు-1;
    CV_Assert( scn + 1 == m.నిలువుల_సంఖ్య );
    CV_Assert( లోతు == CV_32F || లోతు == CV_64F );

    _dst.create_Same_కొలత( src, CV_MAKETYPE(లోతు, dcn) );
    Mat dst = _dst.getMat();

    స్థిరరాశి పూర్ణాంకం mtype = CV_64F;
    Auto_బఫరు<ద్విగుణ> _mbuf;
    ద్విగుణ* mbuf = m.ptr<ద్విగుణ>();

    ఒకవేళ( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.దత్తాంశం();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convert_గమ్యం(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(లోతు);
    CV_Assert( func != 0 );

    స్థిరరాశి Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, మొత్తం = it.size;

    కొరకు( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (పూర్ణాంకం)మొత్తం, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_scale_కూడు( ఆదానం_Array _src1, ద్విగుణ alpha, ఆదానం_Array _src2, OutputArray _dst, పూర్ణాంకం type )
{
    స్థిరరాశి ocl::Device & d = ocl::Device::getDefault();

    తార్కిక doubleSupport = d.doubleFPConfig() > 0;
    కొలత size = _src1.size();
    పూర్ణాంకం లోతు = CV_MAT_లోతు(type);
    ఒకవేళ ( (!doubleSupport && లోతు == CV_64F) || size != _src2.size() )
        తిరిగి అబద్ధం;

    _dst.create(size, type);
    పూర్ణాంకం cn = CV_MAT_CN(type), wdepth = std::max(లోతు, CV_32F);
    పూర్ణాంకం kercn = ocl::predict_Optimal_సదిశ_వెడల్పు_Max(_src1, _src2, _dst),
        అడ్డువరుసలు_చొప్పున_WI = d.isIntel() ? 4 : 1;

    అక్షరం cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)), లోతు,
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(లోతు, wdepth, kercn, cvt[0], పరిమాణం(cvt[0])),
                         ocl::convertTypeStr(wdepth, లోతు, kercn, cvt[1], పరిమాణం(cvt[1])),
                         ocl::type_గమ్యం_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", అడ్డువరుసలు_చొప్పున_WI));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_కొలత(src1),
            src2arg = ocl::KernelArg::Read_Only_No_కొలత(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ఒకవేళ (wdepth == CV_32F)
        k.వాదనలు(src1arg, src2arg, dstarg, (దశాంశం)alpha);
    లేకపోతే
        k.వాదనలు(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.నిలువుల_సంఖ్య * cn / kercn, ((size_t)dst.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

స్థిర Scale_కూడు_Func get_Scale_కూడు_Func(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_కూడు_Func, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

శూన్య scale_కూడు(ఆదానం_Array _src1, ద్విగుణ alpha, ఆదానం_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం type = _src1.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_నిరంతరం(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_కూడు(_src1, alpha, _src2, _dst, type))

    ఒకవేళ( లోతు != CV_32F && లోతు != CV_64F )
    {
        కూడు_Weighted(_src1, alpha, _src2, 1, 0, _dst, లోతు);
        తిరిగి;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    దశాంశం falpha = (దశాంశం)alpha;
    శూన్య* palpha = లోతు == CV_32F ? (శూన్య*)&falpha : (శూన్య*)&alpha;

    Scale_కూడు_Func func = get_Scale_కూడు_Func(లోతు);
    CV_Assert(func);

    ఒకవేళ (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.మొత్తం()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (పూర్ణాంకం)len, palpha);
        తిరిగి;
    }

    స్థిరరాశి Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    కొరకు( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (పూర్ణాంకం)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

శూన్య calc_Covar_మాతృక( స్థిరరాశి Mat* దత్తాంశం, పూర్ణాంకం nsamples, Mat& covar, Mat& సగటు, పూర్ణాంకం flags, పూర్ణాంకం ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( దత్తాంశం, nsamples > 0 );
    కొలత size = దత్తాంశం[0].size();
    పూర్ణాంకం sz = size.వెడల్పు * size.ఎత్తు, esz = (పూర్ణాంకం)దత్తాంశం[0].elem_కొలత();
    పూర్ణాంకం type = దత్తాంశం[0].type();
    Mat సగటు_2;
    ctype = std::max(std::max(CV_MAT_లోతు(ctype >= 0 ? ctype : type), సగటు.లోతు()), CV_32F);

    ఒకవేళ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( సగటు.size() == size );
        ఒకవేళ( సగటు.isContinuous() && సగటు.type() == ctype )
            సగటు_2 = సగటు.reshape(1, 1);
        లేకపోతే
        {
            సగటు.convert_గమ్యం(సగటు_2, ctype);
            సగటు_2 = సగటు_2.reshape(1, 1);
        }
    }

    Mat దత్తాంశం_3(nsamples, sz, type);

    కొరకు( పూర్ణాంకం i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( దత్తాంశం[i].size() == size, దత్తాంశం[i].type() == type );
        ఒకవేళ( దత్తాంశం[i].isContinuous() )
            జ్ఞాపకం_నకలు( దత్తాంశం_3.ptr(i), దత్తాంశం[i].ptr(), sz*esz );
        లేకపోతే
        {
            Mat దత్తాంశం_అడ్డువరుస(size.ఎత్తు, size.వెడల్పు, type, దత్తాంశం_3.ptr(i));
            దత్తాంశం[i].copy_గమ్యం(దత్తాంశం_అడ్డువరుస);
        }
    }

    calc_Covar_మాతృక( దత్తాంశం_3, covar, సగటు_2, (flags & ~(cv::COVAR_అడ్డువరుసలు|cv::COVAR_నిలువుల_సంఖ్య)) | cv::COVAR_అడ్డువరుసలు, ctype );
    ఒకవేళ( (flags & cv::COVAR_USE_AVG) == 0 )
        సగటు = సగటు_2.reshape(1, size.ఎత్తు);
}

శూన్య calc_Covar_మాతృక( ఆదానం_Array _src, OutputArray _covar, ఆదానం_Output_Array సగటు, పూర్ణాంకం flags, పూర్ణాంకం ctype )
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ(_src.రకం() == ఆదానం_Array_2::STD_సదిశ_MAT || _src.రకం() == ఆదానం_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_సదిశ(src);

        CV_Assert( src.size() > 0 );

        కొలత size = src[0].size();
        పూర్ణాంకం type = src[0].type();

        ctype = std::max(std::max(CV_MAT_లోతు(ctype >= 0 ? ctype : type), సగటు.లోతు()), CV_32F);

        Mat దత్తాంశం_3(static_cast<పూర్ణాంకం>(src.size()), size.area(), type);

        పూర్ణాంకం i = 0;
        కొరకు(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat దత్తాంశం_అడ్డువరుస(size.ఎత్తు, size.వెడల్పు, type, దత్తాంశం_3.ptr(i));
            (*each).copy_గమ్యం(దత్తాంశం_అడ్డువరుస);
        }

        Mat సగటు_2;
        ఒకవేళ( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( సగటు.size() == size );
            Mat mean0 = సగటు.getMat();

            ఒకవేళ( mean0.isContinuous() && mean0.type() == ctype )
                సగటు_2 = mean0.reshape(1, 1);
            లేకపోతే
            {
                mean0.convert_గమ్యం(సగటు_2, ctype);
                సగటు_2 = సగటు_2.reshape(1, 1);
            }
        }

        calc_Covar_మాతృక( దత్తాంశం_3, _covar, సగటు_2, (flags & ~(cv::COVAR_అడ్డువరుసలు|cv::COVAR_నిలువుల_సంఖ్య)) | cv::COVAR_అడ్డువరుసలు, ctype );

        ఒకవేళ( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            సగటు_2 = సగటు_2.reshape(1, size.ఎత్తు);
            సగటు_2.copy_గమ్యం(సగటు);
        }
        తిరిగి;
    }

    Mat దత్తాంశం = _src.getMat(), సగటు_2;
    CV_Assert( ((flags & cv::COVAR_అడ్డువరుసలు) != 0) ^ ((flags & cv::COVAR_నిలువుల_సంఖ్య) != 0) );
    తార్కిక take_అడ్డువరుసలు = (flags & cv::COVAR_అడ్డువరుసలు) != 0;
    పూర్ణాంకం type = దత్తాంశం.type();
    పూర్ణాంకం nsamples = take_అడ్డువరుసలు ? దత్తాంశం.అడ్డువరుసలు : దత్తాంశం.నిలువుల_సంఖ్య;
    CV_Assert( nsamples > 0 );
    కొలత size = take_అడ్డువరుసలు ? కొలత(దత్తాంశం.నిలువుల_సంఖ్య, 1) : కొలత(1, దత్తాంశం.అడ్డువరుసలు);

    ఒకవేళ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        సగటు_2 = సగటు.getMat();
        ctype = std::max(std::max(CV_MAT_లోతు(ctype >= 0 ? ctype : type), సగటు_2.లోతు()), CV_32F);
        CV_Assert( సగటు_2.size() == size );
        ఒకవేళ( సగటు_2.type() != ctype )
        {
            సగటు.create(సగటు_2.size(), ctype);
            Mat tmp = సగటు.getMat();
            సగటు_2.convert_గమ్యం(tmp, ctype);
            సగటు_2 = tmp;
        }
    }
    లేకపోతే
    {
        ctype = std::max(CV_MAT_లోతు(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, సగటు, take_అడ్డువరుసలు ? 0 : 1, REDUCE_AVG, ctype );
        సగటు_2 = సగటు.getMat();
    }

    mulTransposed( దత్తాంశం, _covar, ((flags & cv::COVAR_సాధారణ) == 0) ^ take_అడ్డువరుసలు,
        సగటు_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

స్థిర MahalanobisImplFunc getMahalanobisImplFunc(పూర్ణాంకం లోతు)
{
#నిర్వచితమైతే CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (లోతు));
#లేకపోతే
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
#అయితే_ముగింపు
}


ద్విగుణ Mahalanobis(ఆదానం_Array _v1, ఆదానం_Array _v2, ఆదానం_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    పూర్ణాంకం type = v1.type(), లోతు = v1.లోతు();
    కొలత sz = v1.size();
    పూర్ణాంకం len = sz.వెడల్పు*sz.ఎత్తు*v1.channels();
    Auto_బఫరు<ద్విగుణ> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.అడ్డువరుసలు && len == icovar.నిలువుల_సంఖ్య );

    sz.వెడల్పు *= v1.channels();
    ఒకవేళ( v1.isContinuous() && v2.isContinuous() )
    {
        sz.వెడల్పు *= sz.ఎత్తు;
        sz.ఎత్తు = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(లోతు);
    CV_Assert(func);

    ద్విగుణ ఫలితం = func(v1, v2, icovar, buf.దత్తాంశం(), len);
    తిరిగి std::వర్గమూలం(ఫలితం);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

స్థిర MulTransposedFunc getMulTransposedFunc(పూర్ణాంకం stype, పూర్ణాంకం dtype, తార్కిక ata)
{
#నిర్వచితమైతే CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#లేకపోతే
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#అయితే_ముగింపు
}

శూన్య mulTransposed(ఆదానం_Array _src, OutputArray _dst, తార్కిక ata,
                   ఆదానం_Array _delta, ద్విగుణ scale, పూర్ణాంకం dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    స్థిరరాశి పూర్ణాంకం gemm_level = 100; // boundary above which GEMM is faster.
    పూర్ణాంకం stype = src.type();
    dtype = std::max(std::max(CV_MAT_లోతు(dtype >= 0 ? dtype : stype), delta.లోతు()), CV_32F);
    CV_Assert( src.channels() == 1 );

    ఒకవేళ( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.అడ్డువరుసలు == src.అడ్డువరుసలు || delta.అడ్డువరుసలు == 1),
            (delta.నిలువుల_సంఖ్య == src.నిలువుల_సంఖ్య || delta.నిలువుల_సంఖ్య == 1));
        ఒకవేళ( delta.type() != dtype )
            delta.convert_గమ్యం(delta, dtype);
    }

    పూర్ణాంకం dsize = ata ? src.నిలువుల_సంఖ్య : src.అడ్డువరుసలు;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    ఒకవేళ( src.దత్తాంశం == dst.దత్తాంశం || (stype == dtype &&
        (dst.నిలువుల_సంఖ్య >= gemm_level && dst.అడ్డువరుసలు >= gemm_level &&
         src.నిలువుల_సంఖ్య >= gemm_level && src.అడ్డువరుసలు >= gemm_level)))
    {
        Mat src2;
        స్థిరరాశి Mat* tsrc = &src;
        ఒకవేళ( !delta.empty() )
        {
            ఒకవేళ( delta.size() == src.size() )
                వ్యవకలనం( src, delta, src2 );
            లేకపోతే
            {
                repeat(delta, src.అడ్డువరుసలు/delta.అడ్డువరుసలు, src.నిలువుల_సంఖ్య/delta.నిలువుల_సంఖ్య, src2);
                వ్యవకలనం( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    లేకపోతే
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        ఒకవేళ( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, అబద్ధం );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

స్థిర ద్విగుణ dotProd_8u(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((స్థిరరాశి uchar*)src1, (స్థిరరాశి uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
స్థిర ద్విగుణ dotProd_8s(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((స్థిరరాశి schar*)src1, (స్థిరరాశి schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
స్థిర ద్విగుణ dotProd_16u(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((స్థిరరాశి ushort*)src1, (స్థిరరాశి ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
స్థిర ద్విగుణ dotProd_16s(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((స్థిరరాశి పొట్టి*)src1, (స్థిరరాశి పొట్టి*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
స్థిర ద్విగుణ dotProd_32s(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((స్థిరరాశి పూర్ణాంకం*)src1, (స్థిరరాశి పూర్ణాంకం*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
స్థిర ద్విగుణ dotProd_32f(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((స్థిరరాశి దశాంశం*)src1, (స్థిరరాశి దశాంశం*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
స్థిర ద్విగుణ dotProd_64f(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, పూర్ణాంకం len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((స్థిరరాశి ద్విగుణ*)src1, (స్థిరరాశి ద్విగుణ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

రకనిర్వచనం ద్విగుణ (*DotProdFunc)(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, పూర్ణాంకం len);

స్థిర DotProdFunc getDotProdFunc(పూర్ణాంకం లోతు)
{
    స్థిర DotProdFunc dotProdTab[CV_లోతు_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    తిరిగి dotProdTab[లోతు];
}

ద్విగుణ Mat::dot(ఆదానం_Array _mat) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    పూర్ణాంకం cn = channels();
    ఒకవేళ (ఇది->dims <= 2)
    {
        ద్విగుణ లబ్ధం = 0;
        CALL_HAL_RET(dot_లబ్ధం, cv_hal_dot_లబ్ధం, లబ్ధం, ఇది->దత్తాంశం, ఇది->అడుగు, mat.దత్తాంశం, mat.అడుగు,
                     ఇది->నిలువుల_సంఖ్య * cn, ఇది->అడ్డువరుసలు, ఇది->లోతు());
    }

    DotProdFunc func = getDotProdFunc(లోతు());
    CV_Assert(func != 0 );

    ఒకవేళ( isContinuous() && mat.isContinuous() )
    {
        size_t len = మొత్తం()*cn;
        ఒకవేళ( len == (size_t)(పూర్ణాంకం)len )
            తిరిగి func(దత్తాంశం, mat.దత్తాంశం, (పూర్ణాంకం)len);
    }

    స్థిరరాశి Mat* arrays[] = {ఇది, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం len = (పూర్ణాంకం)(it.size*cn);
    ద్విగుణ r = 0;

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    తిరిగి r;
}


#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_dot( ఆదానం_Array _src1, ఆదానం_Array _src2, ద్విగుణ & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    పూర్ణాంకం type = src1.type(), లోతు = CV_MAT_లోతు(type),
            kercn = ocl::predict_Optimal_సదిశ_వెడల్పు(src1, src2);
    తార్కిక doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ఒకవేళ ( !doubleSupport && లోతు == CV_64F )
        తిరిగి అబద్ధం;

    పూర్ణాంకం dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_సమూహం_కొలత();
    పూర్ణాంకం ddepth = std::max(CV_32F, లోతు);

    పూర్ణాంకం wgs2_aligned = 1;
    అయితే (wgs2_aligned < (పూర్ణాంకం)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    అక్షరం cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)), ocl::type_గమ్యం_Str(లోతు),
                         ocl::type_గమ్యం_Str(ddepth), ocl::type_గమ్యం_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(లోతు, ddepth, kercn, cvt, పరిమాణం(cvt)),
                         (పూర్ణాంకం)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_కొలత(src1),
            src2arg = ocl::KernelArg::Read_Only_No_కొలత(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.వాదనలు(src1arg, src1.నిలువుల_సంఖ్య, (పూర్ణాంకం)src1.మొత్తం(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    ఒకవేళ (k.నిరంతరం(1, &globalsize, &wgs, నిజం))
    {
        res = కూడిక(db.getMat(ACCESS_READ))[0];
        తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
}

#అయితే_ముగింపు

ద్విగుణ UMat::dot(ఆదానం_Array m) స్థిరరాశి
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_కొలత(*ఇది) && m.type() == type());

#నిర్వచితమైతే HAVE_OPENCL
    ద్విగుణ r = 0;
    CV_OCL_నిరంతరం_2(dims <= 2, ocl_dot(*ఇది, m, r), r)
#అయితే_ముగింపు

    తిరిగి getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
