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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "intel_gpu_gemm.inl.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "matmul.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CLAMDBLAS

ਸਥਿਰ ਤਾਰਕਿਕ ocl_gemm_amdblas( ਨਿਵੇਸ਼_Array matA, ਨਿਵੇਸ਼_Array matB, ਦੁੱਗਣਾ alpha,
                      ਨਿਵੇਸ਼_Array matC, ਦੁੱਗਣਾ beta, OutputArray matD, ਪੂਰਨ flags )
{
    ਪੂਰਨ type = matA.type(), esz = CV_ELEM_ਮਾਪ(type);
    ਤਾਰਕਿਕ haveC = matC.ਕਿਸਮ() != cv::ਨਿਵੇਸ਼_Array_2::NONE;
    ਮਾਪ ਮਾਪ_A = matA.size(), ਮਾਪ_B = matB.size(), ਮਾਪ_C = haveC ? matC.size() : ਮਾਪ(0, 0);
    ਤਾਰਕਿਕ atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ਜੇ (atrans)
        ਮਾਪ_A = ਮਾਪ(ਮਾਪ_A.ਉਚਾਈ, ਮਾਪ_A.ਚੌੜਾਈ);
    ਜੇ (btrans)
        ਮਾਪ_B = ਮਾਪ(ਮਾਪ_B.ਉਚਾਈ, ਮਾਪ_B.ਚੌੜਾਈ);
    ਜੇ (haveC && ctrans)
        ਮਾਪ_C = ਮਾਪ(ਮਾਪ_C.ਉਚਾਈ, ਮਾਪ_C.ਚੌੜਾਈ);

    ਮਾਪ ਮਾਪ_D(ਮਾਪ_B.ਚੌੜਾਈ, ਮਾਪ_A.ਉਚਾਈ);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( ਮਾਪ_A.ਚੌੜਾਈ == ਮਾਪ_B.ਉਚਾਈ && (!haveC || ਮਾਪ_C == ਮਾਪ_D) );

    matD.create(ਮਾਪ_D, type);
    ਜੇ ( matA.offset() % esz != 0 || matA.ਕਦਮ() % esz != 0 ||
         matB.offset() % esz != 0 || matB.ਕਦਮ() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.ਕਦਮ() % esz != 0)) )
        ਵਾਪਸ ਝੂਠ;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    ਜੇ (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        ਵਾਪਸ ਝੂਠ;
    }
    ਜੇ (haveC)
    {
        UMat C = matC.getUMat();
        ਜੇ (!ocl::internal::isCLBuffer(C))
            ਵਾਪਸ ਝੂਠ;
    }
    ਜੇ (haveC)
        ctrans ? transpose(matC, D) : matC.copy_ਮੰਜ਼ਿਲ(D);
    ਨਹੀਂਤਾਂ
        D.ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar::all(0));

    ਪੂਰਨ M = ਮਾਪ_D.ਉਚਾਈ, N = ਮਾਪ_D.ਚੌੜਾਈ, K = ਮਾਪ_A.ਚੌੜਾਈ;
    ਪੂਰਨ lda = (ਪੂਰਨ)A.ਕਦਮ / esz, ldb = (ਪੂਰਨ)B.ਕਦਮ / esz, ldc = (ਪੂਰਨ)D.ਕਦਮ / esz;
    ਪੂਰਨ offa = (ਪੂਰਨ)A.offset / esz, offb = (ਪੂਰਨ)B.offset / esz, offc = (ਪੂਰਨ)D.offset / esz;

    cl_command_ਕਤਾਰ clq = (cl_command_ਕਤਾਰ)ocl::ਕਤਾਰ::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_ਕ੍ਰਮ ਕ੍ਰਮ = clblas_ਪੰਕਤੀ_Major;
    clblas_ਸਥਿਤੀ ਸਥਿਤੀ = clblasSuccess;

    ਜੇ (type == CV_32FC1)
        ਸਥਿਤੀ = clblasSgemm(ਕ੍ਰਮ, transA, transB, M, N, K,
                             (cl_float)alpha, (ਅਚਲ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ਅਚਲ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਨਹੀਂਤਾਂ ਜੇ (type == CV_64FC1)
        ਸਥਿਤੀ = clblasDgemm(ਕ੍ਰਮ, transA, transB, M, N, K,
                             alpha, (ਅਚਲ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ਅਚਲ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਨਹੀਂਤਾਂ ਜੇ (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         ਸਥਿਤੀ = clblasCgemm(ਕ੍ਰਮ, transA, transB, M, N, K,
                              alpha_2, (ਅਚਲ cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ਅਚਲ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, ਨਿਰਾ, ਨਿਰਾ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        ਸਥਿਤੀ = clblasZgemm(ਕ੍ਰਮ, transA, transB, M, N, K,
                             alpha_2, (ਅਚਲ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ਅਚਲ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ਨਿਰਾ, ਨਿਰਾ);
    }
    ਨਹੀਂਤਾਂ
        CV_Error(Error::StsUnsupportedFormat, "");

    ਵਾਪਸ ਸਥਿਤੀ == clblasSuccess;
}

#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
ਸਥਿਰ ਤਾਰਕਿਕ ocl_gemm( ਨਿਵੇਸ਼_Array matA, ਨਿਵੇਸ਼_Array matB, ਦੁੱਗਣਾ alpha,
                      ਨਿਵੇਸ਼_Array matC, ਦੁੱਗਣਾ beta, OutputArray matD, ਪੂਰਨ flags )
{
    ਪੂਰਨ type = matA.type();
    ਪੂਰਨ ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਪੂਰਨ cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ਅਚਲ ocl::Device & dev = ocl::Device::getDefault();
    ਤਾਰਕਿਕ doubleSupport = dev.doubleFPConfig() > 0;

    ਜੇ (!doubleSupport && ਡੂੰਘਾਈ == CV_64F)
        ਵਾਪਸ ਝੂਠ;

    ਤਾਰਕਿਕ haveC = matC.ਕਿਸਮ() != cv::ਨਿਵੇਸ਼_Array_2::NONE;
    ਮਾਪ ਮਾਪ_A = matA.size(), ਮਾਪ_B = matB.size(), ਮਾਪ_C = haveC ? matC.size() : ਮਾਪ(0, 0);
    ਤਾਰਕਿਕ atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ਜੇ (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    ਮਾਪ ਮਾਪ_D(((btrans) ? ਮਾਪ_B.ਉਚਾਈ : ਮਾਪ_B.ਚੌੜਾਈ),
               ((atrans) ? ਮਾਪ_A.ਚੌੜਾਈ : ਮਾਪ_A.ਉਚਾਈ));

    ਜੇ (atrans)
        ਮਾਪ_A = ਮਾਪ(ਮਾਪ_A.ਉਚਾਈ, ਮਾਪ_A.ਚੌੜਾਈ);
    ਜੇ (btrans)
        ਮਾਪ_B = ਮਾਪ(ਮਾਪ_B.ਉਚਾਈ, ਮਾਪ_B.ਚੌੜਾਈ);
    ਜੇ (haveC && ctrans)
        ਮਾਪ_C = ਮਾਪ(ਮਾਪ_C.ਉਚਾਈ, ਮਾਪ_C.ਚੌੜਾਈ);

    CV_CheckEQ(ਮਾਪ_A.ਚੌੜਾਈ, ਮਾਪ_B.ਉਚਾਈ, "");
    ਜੇ (haveC)
        CV_CheckEQ(ਮਾਪ_C, ਮਾਪ_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(ਮਾਪ_D, type);
    UMat D = matD.getUMat();

    ਤਾਰਕਿਕ isPropagatedC2D = ਝੂਠ;  // D content is updated with C / C.t()

    ਜੇ (dev.intelSubgroupsSupport() && (ਡੂੰਘਾਈ == CV_32F) && cn == 1)
    {
        ਜੇ (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copy_ਮੰਜ਼ਿਲ(D);
            isPropagatedC2D = ਸੱਚ;
        }
        ਨਹੀਂਤਾਂ
        {
            beta = 0.0;
        }

        ਤਾਰਕਿਕ res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, ਮਾਪ_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        ਜੇ (res)
            ਵਾਪਸ ਸੱਚ;
        // fallback on generic OpenCL code
    }

    ਜੇ (ਮਾਪ_D.ਚੌੜਾਈ < 8 || ਮਾਪ_D.ਉਚਾਈ < 8)
        ਵਾਪਸ ਝੂਠ;

    String opts;

    ਪੂਰਨ wg_ਮਾਪ = (ਪੂਰਨ)dev.max_Work_ਸਮੂਹ_ਮਾਪ();
    ਪੂਰਨ ਮਾਪ_Dmin = std::min(ਮਾਪ_D.ਚੌੜਾਈ, ਮਾਪ_D.ਉਚਾਈ);
    wg_ਮਾਪ = std::min(wg_ਮਾਪ, ਮਾਪ_Dmin * ਮਾਪ_Dmin);
    ਪੂਰਨ ਖੰਡ_ਮਾਪ_3 = (wg_ਮਾਪ / (32*cn) < 32) ? (wg_ਮਾਪ / (16*cn) < 16) ? (wg_ਮਾਪ / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    ਜੇ (atrans)
        A = A.t();

    ਜੇ (btrans)
        B = B.t();

    ਜੇ (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_ਮੰਜ਼ਿਲ(D);

    ਪੂਰਨ ਸਦਿਸ਼_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    ਪੂਰਨ kercn = ocl::check_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(ਸਦਿਸ਼_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_ਮੰਜ਼ਿਲ_Str(type), ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKETYPE(ਡੂੰਘਾਈ, kercn)),
                      cn, kercn, ਖੰਡ_ਮਾਪ_3,
                      (ਮਾਪ_A.ਚੌੜਾਈ % ਖੰਡ_ਮਾਪ_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (ਡੂੰਘਾਈ == CV_64F)
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(A),
               ocl::KernelArg::Read_Only_No_ਮਾਪ(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               ਮਾਪ_A.ਚੌੜਾਈ, alpha, beta);
    ਨਹੀਂਤਾਂ
        k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(A),
               ocl::KernelArg::Read_Only_No_ਮਾਪ(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               ਮਾਪ_A.ਚੌੜਾਈ, (ਦਸ਼ਮਲਵ)alpha, (ਦਸ਼ਮਲਵ)beta);

    size_t globalsize[2] = { (size_t)ਮਾਪ_D.ਚੌੜਾਈ * cn / kercn, (size_t)ਮਾਪ_D.ਉਚਾਈ};
    size_t localsize[2] = { (size_t)ਖੰਡ_ਮਾਪ_3, (size_t)ਖੰਡ_ਮਾਪ_3};

    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਖੰਡ_ਮਾਪ_3 !=1 ? localsize : ਨਿਰਾ, ਝੂਠ);
}
#ਜੇ_ਅੰਤ


ਨਾਮਾਕਾਸ਼ hal {

ਖਾਲੀ gemm32f(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
             ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
             ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags));
#ਨਹੀਂਤਾਂ
    CV_CPU_DISPATCH(gemm32f, (src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ gemm64f(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
             ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
             ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags));
#ਨਹੀਂਤਾਂ
    CV_CPU_DISPATCH(gemm64f, (src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ gemm32fc(ਅਚਲ ਦਸ਼ਮਲਵ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦਸ਼ਮਲਵ* src2, size_t src2_ਕਦਮ,
              ਦਸ਼ਮਲਵ alpha, ਅਚਲ ਦਸ਼ਮਲਵ* src3, size_t src3_ਕਦਮ, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ* dst, size_t dst_ਕਦਮ,
              ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags));
#ਨਹੀਂਤਾਂ
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ gemm64fc(ਅਚਲ ਦੁੱਗਣਾ* src1, size_t src1_ਕਦਮ, ਅਚਲ ਦੁੱਗਣਾ* src2, size_t src2_ਕਦਮ,
              ਦੁੱਗਣਾ alpha, ਅਚਲ ਦੁੱਗਣਾ* src3, size_t src3_ਕਦਮ, ਦੁੱਗਣਾ beta, ਦੁੱਗਣਾ* dst, size_t dst_ਕਦਮ,
              ਪੂਰਨ m_a, ਪੂਰਨ n_a, ਪੂਰਨ n_d, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags));
#ਨਹੀਂਤਾਂ
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_ਕਦਮ, src2, src2_ਕਦਮ, alpha, src3, src3_ਕਦਮ, beta, dst, dst_ਕਦਮ, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#ਜੇ_ਅੰਤ
}

} // namespace hal

ਖਾਲੀ gemm(ਨਿਵੇਸ਼_Array matA, ਨਿਵੇਸ਼_Array matB, ਦੁੱਗਣਾ alpha,
          ਨਿਵੇਸ਼_Array matC, ਦੁੱਗਣਾ beta, OutputArray _matD, ਪੂਰਨ flags)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CLAMDBLAS
    CV_OCL_ਲੜੀਵਾਰ(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.ਖਾਨੇ_ਗਿਣਤੀ() > 20 && matA.ਪੰਕਤੀਆਂ() > 20 && matB.ਖਾਨੇ_ਗਿਣਤੀ() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    CV_OCL_ਲੜੀਵਾਰ(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#ਜੇ_ਅੰਤ

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    ਮਾਪ a_ਮਾਪ = A.size(), d_ਮਾਪ;
    ਪੂਰਨ len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    ਚੋਣ( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ਹਾਲਤ 0:
        d_ਮਾਪ = ਮਾਪ( B.ਖਾਨੇ_ਗਿਣਤੀ, a_ਮਾਪ.ਉਚਾਈ );
        len = B.ਪੰਕਤੀਆਂ;
        CV_Assert( a_ਮਾਪ.ਚੌੜਾਈ == len );
        ਰੋਕੋ;
    ਹਾਲਤ 1:
        d_ਮਾਪ = ਮਾਪ( B.ਖਾਨੇ_ਗਿਣਤੀ, a_ਮਾਪ.ਚੌੜਾਈ );
        len = B.ਪੰਕਤੀਆਂ;
        CV_Assert( a_ਮਾਪ.ਉਚਾਈ == len );
        ਰੋਕੋ;
    ਹਾਲਤ 2:
        d_ਮਾਪ = ਮਾਪ( B.ਪੰਕਤੀਆਂ, a_ਮਾਪ.ਉਚਾਈ );
        len = B.ਖਾਨੇ_ਗਿਣਤੀ;
        CV_Assert( a_ਮਾਪ.ਚੌੜਾਈ == len );
        ਰੋਕੋ;
    ਹਾਲਤ 3:
        d_ਮਾਪ = ਮਾਪ( B.ਪੰਕਤੀਆਂ, a_ਮਾਪ.ਚੌੜਾਈ );
        len = B.ਖਾਨੇ_ਗਿਣਤੀ;
        CV_Assert( a_ਮਾਪ.ਉਚਾਈ == len );
        ਰੋਕੋ;
    }

    ਜੇ( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.ਪੰਕਤੀਆਂ == d_ਮਾਪ.ਉਚਾਈ && C.ਖਾਨੇ_ਗਿਣਤੀ == d_ਮਾਪ.ਚੌੜਾਈ) ||
             ((flags&GEMM_3_T) != 0 && C.ਪੰਕਤੀਆਂ == d_ਮਾਪ.ਚੌੜਾਈ && C.ਖਾਨੇ_ਗਿਣਤੀ == d_ਮਾਪ.ਉਚਾਈ)));
    }

    _matD.create( d_ਮਾਪ.ਉਚਾਈ, d_ਮਾਪ.ਚੌੜਾਈ, type );
    Mat D = _matD.getMat();
    ਜੇ( (flags & GEMM_3_T) != 0 && C.ਅੰਕੜੇ == D.ਅੰਕੜੇ )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    ਜੇ( D.ਅੰਕੜੇ == A.ਅੰਕੜੇ || D.ਅੰਕੜੇ == B.ਅੰਕੜੇ )
    {
        DProxy = Mat(d_ਮਾਪ.ਉਚਾਈ, d_ਮਾਪ.ਚੌੜਾਈ, D.type());
        DProxyPtr = &DProxy;
    }

    ਜੇ( type == CV_32FC1 )
        hal::gemm32f(A.ptr<ਦਸ਼ਮਲਵ>(), A.ਕਦਮ, B.ptr<ਦਸ਼ਮਲਵ>(), B.ਕਦਮ, static_cast<ਦਸ਼ਮਲਵ>(alpha),
                     C.ptr<ਦਸ਼ਮਲਵ>(), C.ਕਦਮ, static_cast<ਦਸ਼ਮਲਵ>(beta),
                     DProxyPtr->ptr<ਦਸ਼ਮਲਵ>(), DProxyPtr->ਕਦਮ,
                     a_ਮਾਪ.ਉਚਾਈ, a_ਮਾਪ.ਚੌੜਾਈ, DProxyPtr->ਖਾਨੇ_ਗਿਣਤੀ, flags);
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_64FC1 )
        hal::gemm64f(A.ptr<ਦੁੱਗਣਾ>(), A.ਕਦਮ, B.ptr<ਦੁੱਗਣਾ>(), B.ਕਦਮ, alpha,
                     C.ptr<ਦੁੱਗਣਾ>(), C.ਕਦਮ, beta,
                     DProxyPtr->ptr<ਦੁੱਗਣਾ>(), DProxyPtr->ਕਦਮ,
                     a_ਮਾਪ.ਉਚਾਈ, a_ਮਾਪ.ਚੌੜਾਈ, DProxyPtr->ਖਾਨੇ_ਗਿਣਤੀ, flags);
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<ਦਸ਼ਮਲਵ>(), A.ਕਦਮ, B.ptr<ਦਸ਼ਮਲਵ>(), B.ਕਦਮ, static_cast<ਦਸ਼ਮਲਵ>(alpha),
                      C.ptr<ਦਸ਼ਮਲਵ>(), C.ਕਦਮ, static_cast<ਦਸ਼ਮਲਵ>(beta),
                      DProxyPtr->ptr<ਦਸ਼ਮਲਵ>(), DProxyPtr->ਕਦਮ,
                      a_ਮਾਪ.ਉਚਾਈ, a_ਮਾਪ.ਚੌੜਾਈ, DProxyPtr->ਖਾਨੇ_ਗਿਣਤੀ, flags);
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<ਦੁੱਗਣਾ>(), A.ਕਦਮ, B.ptr<ਦੁੱਗਣਾ>(), B.ਕਦਮ, alpha,
                      C.ptr<ਦੁੱਗਣਾ>(), C.ਕਦਮ, beta,
                      DProxyPtr->ptr<ਦੁੱਗਣਾ>(), DProxyPtr->ਕਦਮ,
                      a_ਮਾਪ.ਉਚਾਈ, a_ਮਾਪ.ਚੌੜਾਈ, DProxyPtr->ਖਾਨੇ_ਗਿਣਤੀ, flags);
    }

    ਜੇ(DProxyPtr != &D)
        DProxyPtr->copy_ਮੰਜ਼ਿਲ(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ਸਥਿਰ TransformFunc getTransformFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਸਥਿਰ TransformFunc getDiagTransformFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ transform(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਨਿਵੇਸ਼_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ਪੂਰਨ ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ(), scn = src.channels(), dcn = m.ਪੰਕਤੀਆਂ;
    CV_Assert( scn == m.ਖਾਨੇ_ਗਿਣਤੀ || scn + 1 == m.ਖਾਨੇ_ਗਿਣਤੀ );
    ਤਾਰਕਿਕ isDiag = ਝੂਠ;

    _dst.create_Same_ਮਾਪ( src, CV_MAKETYPE(ਡੂੰਘਾਈ, dcn) );
    Mat dst = _dst.getMat();

    ਜੇ (src.ਅੰਕੜੇ == dst.ਅੰਕੜੇ)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    ਪੂਰਨ mtype = ਡੂੰਘਾਈ == CV_32S || ਡੂੰਘਾਈ == CV_64F ? CV_64F : CV_32F;
    Auto_ਬਫਰ<ਦੁੱਗਣਾ> _mbuf;
    ਦੁੱਗਣਾ* mbuf;

    ਜੇ( !m.isContinuous() || m.type() != mtype || m.ਖਾਨੇ_ਗਿਣਤੀ != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.ਅੰਕੜੇ();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.ਕੁੱਲ()*tmp.elem_ਮਾਪ());
        ਜੇ( m.ਖਾਨੇ_ਗਿਣਤੀ == scn+1 )
            m.convert_ਮੰਜ਼ਿਲ(tmp, mtype);
        ਨਹੀਂਤਾਂ
        {
            Mat tmppart = tmp.ਖਾਨਾ_Range_2(0, m.ਖਾਨੇ_ਗਿਣਤੀ);
            m.convert_ਮੰਜ਼ਿਲ(tmppart, mtype);
        }
        m = tmp;
    }
    ਨਹੀਂਤਾਂ
        mbuf = m.ptr<ਦੁੱਗਣਾ>();

    ਜੇ( scn == dcn )
    {
        ਪੂਰਨ i, j;
        ਦੁੱਗਣਾ eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        ਜੇ( scn == 1 )
        {
            ਦੁੱਗਣਾ alpha, beta;
            ਜੇ( mtype == CV_32F )
                alpha = m.at<ਦਸ਼ਮਲਵ>(0), beta = m.at<ਦਸ਼ਮਲਵ>(1);
            ਨਹੀਂਤਾਂ
                alpha = m.at<ਦੁੱਗਣਾ>(0), beta = m.at<ਦੁੱਗਣਾ>(1);
            src.convert_ਮੰਜ਼ਿਲ(dst, dst.type(), alpha, beta);
            ਵਾਪਸ;
        }

        ਲਈ( i = 0, isDiag = ਸੱਚ; isDiag && i < scn; i++ )
        {
            ਲਈ( j = 0; isDiag && j < scn; j++ )
            {
                ਦੁੱਗਣਾ v = mtype == CV_32F ? m.at<ਦਸ਼ਮਲਵ>(i, j) : m.at<ਦੁੱਗਣਾ>(i, j);
                ਜੇ( i != j && fabs(v) > eps )
                    isDiag = ਝੂਠ;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(ਡੂੰਘਾਈ): getTransformFunc(ਡੂੰਘਾਈ);
    CV_Assert( func != 0 );

    ਅਚਲ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, ਕੁੱਲ = it.size;

    ਲਈ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ਪੂਰਨ)ਕੁੱਲ, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ਸਥਿਰ TransformFunc getPerspectiveTransform(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ perspectiveTransform(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਨਿਵੇਸ਼_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ਪੂਰਨ ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ(), scn = src.channels(), dcn = m.ਪੰਕਤੀਆਂ-1;
    CV_Assert( scn + 1 == m.ਖਾਨੇ_ਗਿਣਤੀ );
    CV_Assert( ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F );

    _dst.create_Same_ਮਾਪ( src, CV_MAKETYPE(ਡੂੰਘਾਈ, dcn) );
    Mat dst = _dst.getMat();

    ਅਚਲ ਪੂਰਨ mtype = CV_64F;
    Auto_ਬਫਰ<ਦੁੱਗਣਾ> _mbuf;
    ਦੁੱਗਣਾ* mbuf = m.ptr<ਦੁੱਗਣਾ>();

    ਜੇ( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.ਅੰਕੜੇ();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convert_ਮੰਜ਼ਿਲ(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(ਡੂੰਘਾਈ);
    CV_Assert( func != 0 );

    ਅਚਲ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, ਕੁੱਲ = it.size;

    ਲਈ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ਪੂਰਨ)ਕੁੱਲ, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_scale_ਜੋੜੋ( ਨਿਵੇਸ਼_Array _src1, ਦੁੱਗਣਾ alpha, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst, ਪੂਰਨ type )
{
    ਅਚਲ ocl::Device & d = ocl::Device::getDefault();

    ਤਾਰਕਿਕ doubleSupport = d.doubleFPConfig() > 0;
    ਮਾਪ size = _src1.size();
    ਪੂਰਨ ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਜੇ ( (!doubleSupport && ਡੂੰਘਾਈ == CV_64F) || size != _src2.size() )
        ਵਾਪਸ ਝੂਠ;

    _dst.create(size, type);
    ਪੂਰਨ cn = CV_MAT_CN(type), wdepth = std::max(ਡੂੰਘਾਈ, CV_32F);
    ਪੂਰਨ kercn = ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ_Max(_src1, _src2, _dst),
        ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = d.isIntel() ? 4 : 1;

    ਅੱਖਰ cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)), ਡੂੰਘਾਈ,
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(ਡੂੰਘਾਈ, wdepth, kercn, cvt[0], ਆਕਾਰ(cvt[0])),
                         ocl::convertTypeStr(wdepth, ਡੂੰਘਾਈ, kercn, cvt[1], ਆਕਾਰ(cvt[1])),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src1),
            src2arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ਜੇ (wdepth == CV_32F)
        k.ਦਲੀਲਾਂ(src1arg, src2arg, dstarg, (ਦਸ਼ਮਲਵ)alpha);
    ਨਹੀਂਤਾਂ
        k.ਦਲੀਲਾਂ(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)dst.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ Scale_ਜੋੜੋ_Func get_Scale_ਜੋੜੋ_Func(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_ਜੋੜੋ_Func, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਖਾਲੀ scale_ਜੋੜੋ(ਨਿਵੇਸ਼_Array _src1, ਦੁੱਗਣਾ alpha, ਨਿਵੇਸ਼_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ type = _src1.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_ਲੜੀਵਾਰ(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_ਜੋੜੋ(_src1, alpha, _src2, _dst, type))

    ਜੇ( ਡੂੰਘਾਈ != CV_32F && ਡੂੰਘਾਈ != CV_64F )
    {
        ਜੋੜੋ_Weighted(_src1, alpha, _src2, 1, 0, _dst, ਡੂੰਘਾਈ);
        ਵਾਪਸ;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    ਦਸ਼ਮਲਵ falpha = (ਦਸ਼ਮਲਵ)alpha;
    ਖਾਲੀ* palpha = ਡੂੰਘਾਈ == CV_32F ? (ਖਾਲੀ*)&falpha : (ਖਾਲੀ*)&alpha;

    Scale_ਜੋੜੋ_Func func = get_Scale_ਜੋੜੋ_Func(ਡੂੰਘਾਈ);
    CV_Assert(func);

    ਜੇ (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.ਕੁੱਲ()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (ਪੂਰਨ)len, palpha);
        ਵਾਪਸ;
    }

    ਅਚਲ Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    ਲਈ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (ਪੂਰਨ)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

ਖਾਲੀ calc_Covar_ਮੈਟ੍ਰਿਕਸ( ਅਚਲ Mat* ਅੰਕੜੇ, ਪੂਰਨ nsamples, Mat& covar, Mat& ਔਸਤ, ਪੂਰਨ flags, ਪੂਰਨ ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( ਅੰਕੜੇ, nsamples > 0 );
    ਮਾਪ size = ਅੰਕੜੇ[0].size();
    ਪੂਰਨ sz = size.ਚੌੜਾਈ * size.ਉਚਾਈ, esz = (ਪੂਰਨ)ਅੰਕੜੇ[0].elem_ਮਾਪ();
    ਪੂਰਨ type = ਅੰਕੜੇ[0].type();
    Mat ਔਸਤ_2;
    ctype = std::max(std::max(CV_MAT_ਡੂੰਘਾਈ(ctype >= 0 ? ctype : type), ਔਸਤ.ਡੂੰਘਾਈ()), CV_32F);

    ਜੇ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( ਔਸਤ.size() == size );
        ਜੇ( ਔਸਤ.isContinuous() && ਔਸਤ.type() == ctype )
            ਔਸਤ_2 = ਔਸਤ.reshape(1, 1);
        ਨਹੀਂਤਾਂ
        {
            ਔਸਤ.convert_ਮੰਜ਼ਿਲ(ਔਸਤ_2, ctype);
            ਔਸਤ_2 = ਔਸਤ_2.reshape(1, 1);
        }
    }

    Mat ਅੰਕੜੇ_3(nsamples, sz, type);

    ਲਈ( ਪੂਰਨ i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( ਅੰਕੜੇ[i].size() == size, ਅੰਕੜੇ[i].type() == type );
        ਜੇ( ਅੰਕੜੇ[i].isContinuous() )
            ਸਮ੍ਰਿਤੀ_ਨਕਲ( ਅੰਕੜੇ_3.ptr(i), ਅੰਕੜੇ[i].ptr(), sz*esz );
        ਨਹੀਂਤਾਂ
        {
            Mat ਅੰਕੜੇ_ਪੰਕਤੀ(size.ਉਚਾਈ, size.ਚੌੜਾਈ, type, ਅੰਕੜੇ_3.ptr(i));
            ਅੰਕੜੇ[i].copy_ਮੰਜ਼ਿਲ(ਅੰਕੜੇ_ਪੰਕਤੀ);
        }
    }

    calc_Covar_ਮੈਟ੍ਰਿਕਸ( ਅੰਕੜੇ_3, covar, ਔਸਤ_2, (flags & ~(cv::COVAR_ਪੰਕਤੀਆਂ|cv::COVAR_ਖਾਨੇ_ਗਿਣਤੀ)) | cv::COVAR_ਪੰਕਤੀਆਂ, ctype );
    ਜੇ( (flags & cv::COVAR_USE_AVG) == 0 )
        ਔਸਤ = ਔਸਤ_2.reshape(1, size.ਉਚਾਈ);
}

ਖਾਲੀ calc_Covar_ਮੈਟ੍ਰਿਕਸ( ਨਿਵੇਸ਼_Array _src, OutputArray _covar, ਨਿਵੇਸ਼_Output_Array ਔਸਤ, ਪੂਰਨ flags, ਪੂਰਨ ctype )
{
    CV_INSTRUMENT_REGION();

    ਜੇ(_src.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT || _src.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_ਸਦਿਸ਼(src);

        CV_Assert( src.size() > 0 );

        ਮਾਪ size = src[0].size();
        ਪੂਰਨ type = src[0].type();

        ctype = std::max(std::max(CV_MAT_ਡੂੰਘਾਈ(ctype >= 0 ? ctype : type), ਔਸਤ.ਡੂੰਘਾਈ()), CV_32F);

        Mat ਅੰਕੜੇ_3(static_cast<ਪੂਰਨ>(src.size()), size.area(), type);

        ਪੂਰਨ i = 0;
        ਲਈ(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat ਅੰਕੜੇ_ਪੰਕਤੀ(size.ਉਚਾਈ, size.ਚੌੜਾਈ, type, ਅੰਕੜੇ_3.ptr(i));
            (*each).copy_ਮੰਜ਼ਿਲ(ਅੰਕੜੇ_ਪੰਕਤੀ);
        }

        Mat ਔਸਤ_2;
        ਜੇ( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( ਔਸਤ.size() == size );
            Mat mean0 = ਔਸਤ.getMat();

            ਜੇ( mean0.isContinuous() && mean0.type() == ctype )
                ਔਸਤ_2 = mean0.reshape(1, 1);
            ਨਹੀਂਤਾਂ
            {
                mean0.convert_ਮੰਜ਼ਿਲ(ਔਸਤ_2, ctype);
                ਔਸਤ_2 = ਔਸਤ_2.reshape(1, 1);
            }
        }

        calc_Covar_ਮੈਟ੍ਰਿਕਸ( ਅੰਕੜੇ_3, _covar, ਔਸਤ_2, (flags & ~(cv::COVAR_ਪੰਕਤੀਆਂ|cv::COVAR_ਖਾਨੇ_ਗਿਣਤੀ)) | cv::COVAR_ਪੰਕਤੀਆਂ, ctype );

        ਜੇ( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            ਔਸਤ_2 = ਔਸਤ_2.reshape(1, size.ਉਚਾਈ);
            ਔਸਤ_2.copy_ਮੰਜ਼ਿਲ(ਔਸਤ);
        }
        ਵਾਪਸ;
    }

    Mat ਅੰਕੜੇ = _src.getMat(), ਔਸਤ_2;
    CV_Assert( ((flags & cv::COVAR_ਪੰਕਤੀਆਂ) != 0) ^ ((flags & cv::COVAR_ਖਾਨੇ_ਗਿਣਤੀ) != 0) );
    ਤਾਰਕਿਕ take_ਪੰਕਤੀਆਂ = (flags & cv::COVAR_ਪੰਕਤੀਆਂ) != 0;
    ਪੂਰਨ type = ਅੰਕੜੇ.type();
    ਪੂਰਨ nsamples = take_ਪੰਕਤੀਆਂ ? ਅੰਕੜੇ.ਪੰਕਤੀਆਂ : ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ;
    CV_Assert( nsamples > 0 );
    ਮਾਪ size = take_ਪੰਕਤੀਆਂ ? ਮਾਪ(ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ, 1) : ਮਾਪ(1, ਅੰਕੜੇ.ਪੰਕਤੀਆਂ);

    ਜੇ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        ਔਸਤ_2 = ਔਸਤ.getMat();
        ctype = std::max(std::max(CV_MAT_ਡੂੰਘਾਈ(ctype >= 0 ? ctype : type), ਔਸਤ_2.ਡੂੰਘਾਈ()), CV_32F);
        CV_Assert( ਔਸਤ_2.size() == size );
        ਜੇ( ਔਸਤ_2.type() != ctype )
        {
            ਔਸਤ.create(ਔਸਤ_2.size(), ctype);
            Mat tmp = ਔਸਤ.getMat();
            ਔਸਤ_2.convert_ਮੰਜ਼ਿਲ(tmp, ctype);
            ਔਸਤ_2 = tmp;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ctype = std::max(CV_MAT_ਡੂੰਘਾਈ(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, ਔਸਤ, take_ਪੰਕਤੀਆਂ ? 0 : 1, REDUCE_AVG, ctype );
        ਔਸਤ_2 = ਔਸਤ.getMat();
    }

    mulTransposed( ਅੰਕੜੇ, _covar, ((flags & cv::COVAR_ਸਾਧਾਰਨ) == 0) ^ take_ਪੰਕਤੀਆਂ,
        ਔਸਤ_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ਸਥਿਰ MahalanobisImplFunc getMahalanobisImplFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (ਡੂੰਘਾਈ));
#ਨਹੀਂਤਾਂ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (ਡੂੰਘਾਈ),
        CV_CPU_DISPATCH_MODES_ALL);
#ਜੇ_ਅੰਤ
}


ਦੁੱਗਣਾ Mahalanobis(ਨਿਵੇਸ਼_Array _v1, ਨਿਵੇਸ਼_Array _v2, ਨਿਵੇਸ਼_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    ਪੂਰਨ type = v1.type(), ਡੂੰਘਾਈ = v1.ਡੂੰਘਾਈ();
    ਮਾਪ sz = v1.size();
    ਪੂਰਨ len = sz.ਚੌੜਾਈ*sz.ਉਚਾਈ*v1.channels();
    Auto_ਬਫਰ<ਦੁੱਗਣਾ> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.ਪੰਕਤੀਆਂ && len == icovar.ਖਾਨੇ_ਗਿਣਤੀ );

    sz.ਚੌੜਾਈ *= v1.channels();
    ਜੇ( v1.isContinuous() && v2.isContinuous() )
    {
        sz.ਚੌੜਾਈ *= sz.ਉਚਾਈ;
        sz.ਉਚਾਈ = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(ਡੂੰਘਾਈ);
    CV_Assert(func);

    ਦੁੱਗਣਾ ਨਤੀਜਾ = func(v1, v2, icovar, buf.ਅੰਕੜੇ(), len);
    ਵਾਪਸ std::ਵਰਗਮੂਲ(ਨਤੀਜਾ);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ਸਥਿਰ MulTransposedFunc getMulTransposedFunc(ਪੂਰਨ stype, ਪੂਰਨ dtype, ਤਾਰਕਿਕ ata)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#ਨਹੀਂਤਾਂ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ mulTransposed(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਤਾਰਕਿਕ ata,
                   ਨਿਵੇਸ਼_Array _delta, ਦੁੱਗਣਾ scale, ਪੂਰਨ dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ਅਚਲ ਪੂਰਨ gemm_level = 100; // boundary above which GEMM is faster.
    ਪੂਰਨ stype = src.type();
    dtype = std::max(std::max(CV_MAT_ਡੂੰਘਾਈ(dtype >= 0 ? dtype : stype), delta.ਡੂੰਘਾਈ()), CV_32F);
    CV_Assert( src.channels() == 1 );

    ਜੇ( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.ਪੰਕਤੀਆਂ == src.ਪੰਕਤੀਆਂ || delta.ਪੰਕਤੀਆਂ == 1),
            (delta.ਖਾਨੇ_ਗਿਣਤੀ == src.ਖਾਨੇ_ਗਿਣਤੀ || delta.ਖਾਨੇ_ਗਿਣਤੀ == 1));
        ਜੇ( delta.type() != dtype )
            delta.convert_ਮੰਜ਼ਿਲ(delta, dtype);
    }

    ਪੂਰਨ dsize = ata ? src.ਖਾਨੇ_ਗਿਣਤੀ : src.ਪੰਕਤੀਆਂ;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    ਜੇ( src.ਅੰਕੜੇ == dst.ਅੰਕੜੇ || (stype == dtype &&
        (dst.ਖਾਨੇ_ਗਿਣਤੀ >= gemm_level && dst.ਪੰਕਤੀਆਂ >= gemm_level &&
         src.ਖਾਨੇ_ਗਿਣਤੀ >= gemm_level && src.ਪੰਕਤੀਆਂ >= gemm_level)))
    {
        Mat src2;
        ਅਚਲ Mat* tsrc = &src;
        ਜੇ( !delta.empty() )
        {
            ਜੇ( delta.size() == src.size() )
                ਘਟਾਓ( src, delta, src2 );
            ਨਹੀਂਤਾਂ
            {
                repeat(delta, src.ਪੰਕਤੀਆਂ/delta.ਪੰਕਤੀਆਂ, src.ਖਾਨੇ_ਗਿਣਤੀ/delta.ਖਾਨੇ_ਗਿਣਤੀ, src2);
                ਘਟਾਓ( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    ਨਹੀਂਤਾਂ
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        ਜੇ( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, ਝੂਠ );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ਸਥਿਰ ਦੁੱਗਣਾ dotProd_8u(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ਅਚਲ uchar*)src1, (ਅਚਲ uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਸਥਿਰ ਦੁੱਗਣਾ dotProd_8s(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ਅਚਲ schar*)src1, (ਅਚਲ schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਸਥਿਰ ਦੁੱਗਣਾ dotProd_16u(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ਅਚਲ ushort*)src1, (ਅਚਲ ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਸਥਿਰ ਦੁੱਗਣਾ dotProd_16s(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ਅਚਲ ਛੋਟਾ*)src1, (ਅਚਲ ਛੋਟਾ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਸਥਿਰ ਦੁੱਗਣਾ dotProd_32s(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ਅਚਲ ਪੂਰਨ*)src1, (ਅਚਲ ਪੂਰਨ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਸਥਿਰ ਦੁੱਗਣਾ dotProd_32f(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ਅਚਲ ਦਸ਼ਮਲਵ*)src1, (ਅਚਲ ਦਸ਼ਮਲਵ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ਸਥਿਰ ਦੁੱਗਣਾ dotProd_64f(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਪੂਰਨ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ਅਚਲ ਦੁੱਗਣਾ*)src1, (ਅਚਲ ਦੁੱਗਣਾ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦੁੱਗਣਾ (*DotProdFunc)(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, ਪੂਰਨ len);

ਸਥਿਰ DotProdFunc getDotProdFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ DotProdFunc dotProdTab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    ਵਾਪਸ dotProdTab[ਡੂੰਘਾਈ];
}

ਦੁੱਗਣਾ Mat::dot(ਨਿਵੇਸ਼_Array _mat) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    ਪੂਰਨ cn = channels();
    ਜੇ (ਇਹ->dims <= 2)
    {
        ਦੁੱਗਣਾ ਗੁਣਨਫਲ = 0;
        CALL_HAL_RET(dot_ਗੁਣਨਫਲ, cv_hal_dot_ਗੁਣਨਫਲ, ਗੁਣਨਫਲ, ਇਹ->ਅੰਕੜੇ, ਇਹ->ਕਦਮ, mat.ਅੰਕੜੇ, mat.ਕਦਮ,
                     ਇਹ->ਖਾਨੇ_ਗਿਣਤੀ * cn, ਇਹ->ਪੰਕਤੀਆਂ, ਇਹ->ਡੂੰਘਾਈ());
    }

    DotProdFunc func = getDotProdFunc(ਡੂੰਘਾਈ());
    CV_Assert(func != 0 );

    ਜੇ( isContinuous() && mat.isContinuous() )
    {
        size_t len = ਕੁੱਲ()*cn;
        ਜੇ( len == (size_t)(ਪੂਰਨ)len )
            ਵਾਪਸ func(ਅੰਕੜੇ, mat.ਅੰਕੜੇ, (ਪੂਰਨ)len);
    }

    ਅਚਲ Mat* arrays[] = {ਇਹ, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ len = (ਪੂਰਨ)(it.size*cn);
    ਦੁੱਗਣਾ r = 0;

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    ਵਾਪਸ r;
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_dot( ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2, ਦੁੱਗਣਾ & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    ਪੂਰਨ type = src1.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type),
            kercn = ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(src1, src2);
    ਤਾਰਕਿਕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ਜੇ ( !doubleSupport && ਡੂੰਘਾਈ == CV_64F )
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ਸਮੂਹ_ਮਾਪ();
    ਪੂਰਨ ddepth = std::max(CV_32F, ਡੂੰਘਾਈ);

    ਪੂਰਨ wgs2_aligned = 1;
    ਜਦੋਂ (wgs2_aligned < (ਪੂਰਨ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ਅੱਖਰ cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)), ocl::type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ),
                         ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(ਡੂੰਘਾਈ, ddepth, kercn, cvt, ਆਕਾਰ(cvt)),
                         (ਪੂਰਨ)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src1),
            src2arg = ocl::KernelArg::Read_Only_No_ਮਾਪ(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.ਦਲੀਲਾਂ(src1arg, src1.ਖਾਨੇ_ਗਿਣਤੀ, (ਪੂਰਨ)src1.ਕੁੱਲ(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    ਜੇ (k.ਲੜੀਵਾਰ(1, &globalsize, &wgs, ਸੱਚ))
    {
        res = ਜੋੜ(db.getMat(ACCESS_READ))[0];
        ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਅੰਤ

ਦੁੱਗਣਾ UMat::dot(ਨਿਵੇਸ਼_Array m) ਅਚਲ
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_ਮਾਪ(*ਇਹ) && m.type() == type());

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਦੁੱਗਣਾ r = 0;
    CV_OCL_ਲੜੀਵਾਰ_2(dims <= 2, ocl_dot(*ਇਹ, m, r), r)
#ਜੇ_ਅੰਤ

    ਵਾਪਸ getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
