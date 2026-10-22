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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_core.hpp"
#ಅನ್ತರ್ಭಾವ "intel_gpu_gemm.inl.hpp"

#ಅನ್ತರ್ಭಾವ "matmul.simd.hpp"
#ಅನ್ತರ್ಭಾವ "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CLAMDBLAS

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_gemm_amdblas( ಒಳಹರಿವು_Array matA, ಒಳಹರಿವು_Array matB, ದ್ವಿಗುಣ alpha,
                      ಒಳಹರಿವು_Array matC, ದ್ವಿಗುಣ beta, OutputArray matD, ಪೂರ್ಣಾಂಕ flags )
{
    ಪೂರ್ಣಾಂಕ type = matA.type(), esz = CV_ELEM_ಗಾತ್ರ(type);
    ತಾರ್ಕಿಕ haveC = matC.ಬಗೆ() != cv::ಒಳಹರಿವು_Array_2::NONE;
    ಗಾತ್ರ_2 ಗಾತ್ರ_A = matA.size(), ಗಾತ್ರ_B = matB.size(), ಗಾತ್ರ_C = haveC ? matC.size() : ಗಾತ್ರ_2(0, 0);
    ತಾರ್ಕಿಕ atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ಆದರೆ (atrans)
        ಗಾತ್ರ_A = ಗಾತ್ರ_2(ಗಾತ್ರ_A.ಎತ್ತರ, ಗಾತ್ರ_A.ಅಗಲ);
    ಆದರೆ (btrans)
        ಗಾತ್ರ_B = ಗಾತ್ರ_2(ಗಾತ್ರ_B.ಎತ್ತರ, ಗಾತ್ರ_B.ಅಗಲ);
    ಆದರೆ (haveC && ctrans)
        ಗಾತ್ರ_C = ಗಾತ್ರ_2(ಗಾತ್ರ_C.ಎತ್ತರ, ಗಾತ್ರ_C.ಅಗಲ);

    ಗಾತ್ರ_2 ಗಾತ್ರ_D(ಗಾತ್ರ_B.ಅಗಲ, ಗಾತ್ರ_A.ಎತ್ತರ);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( ಗಾತ್ರ_A.ಅಗಲ == ಗಾತ್ರ_B.ಎತ್ತರ && (!haveC || ಗಾತ್ರ_C == ಗಾತ್ರ_D) );

    matD.create(ಗಾತ್ರ_D, type);
    ಆದರೆ ( matA.offset() % esz != 0 || matA.ಹೆಜ್ಜೆ() % esz != 0 ||
         matB.offset() % esz != 0 || matB.ಹೆಜ್ಜೆ() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.ಹೆಜ್ಜೆ() % esz != 0)) )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    ಆದರೆ (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಆದರೆ (haveC)
    {
        UMat C = matC.getUMat();
        ಆದರೆ (!ocl::internal::isCLBuffer(C))
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಆದರೆ (haveC)
        ctrans ? transpose(matC, D) : matC.copy_ಗಮ್ಯ(D);
    ಇಲ್ಲದಿದ್ದರೆ
        D.ಗಣ_ಗಮ್ಯ(Scalar::all(0));

    ಪೂರ್ಣಾಂಕ M = ಗಾತ್ರ_D.ಎತ್ತರ, N = ಗಾತ್ರ_D.ಅಗಲ, K = ಗಾತ್ರ_A.ಅಗಲ;
    ಪೂರ್ಣಾಂಕ lda = (ಪೂರ್ಣಾಂಕ)A.ಹೆಜ್ಜೆ / esz, ldb = (ಪೂರ್ಣಾಂಕ)B.ಹೆಜ್ಜೆ / esz, ldc = (ಪೂರ್ಣಾಂಕ)D.ಹೆಜ್ಜೆ / esz;
    ಪೂರ್ಣಾಂಕ offa = (ಪೂರ್ಣಾಂಕ)A.offset / esz, offb = (ಪೂರ್ಣಾಂಕ)B.offset / esz, offc = (ಪೂರ್ಣಾಂಕ)D.offset / esz;

    cl_command_ಸರತಿ clq = (cl_command_ಸರತಿ)ocl::ಸರತಿ::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_ಕ್ರಮ ಕ್ರಮ = clblas_ಅಡ್ಡಸಾಲು_Major;
    clblas_ಸ್ಥಿತಿ ಸ್ಥಿತಿ_2 = clblasSuccess;

    ಆದರೆ (type == CV_32FC1)
        ಸ್ಥಿತಿ_2 = clblasSgemm(ಕ್ರಮ, transA, transB, M, N, K,
                             (cl_float)alpha, (ಸ್ಥಿರಾಂಕ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ಸ್ಥಿರಾಂಕ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == CV_64FC1)
        ಸ್ಥಿತಿ_2 = clblasDgemm(ಕ್ರಮ, transA, transB, M, N, K,
                             alpha, (ಸ್ಥಿರಾಂಕ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ಸ್ಥಿರಾಂಕ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         ಸ್ಥಿತಿ_2 = clblasCgemm(ಕ್ರಮ, transA, transB, M, N, K,
                              alpha_2, (ಸ್ಥಿರಾಂಕ cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ಸ್ಥಿರಾಂಕ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, ಖಾಲಿ, ಖಾಲಿ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        ಸ್ಥಿತಿ_2 = clblasZgemm(ಕ್ರಮ, transA, transB, M, N, K,
                             alpha_2, (ಸ್ಥಿರಾಂಕ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ಸ್ಥಿರಾಂಕ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ಖಾಲಿ, ಖಾಲಿ);
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsUnsupportedFormat, "");

    ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2 == clblasSuccess;
}

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_gemm( ಒಳಹರಿವು_Array matA, ಒಳಹರಿವು_Array matB, ದ್ವಿಗುಣ alpha,
                      ಒಳಹರಿವು_Array matC, ದ್ವಿಗುಣ beta, OutputArray matD, ಪೂರ್ಣಾಂಕ flags )
{
    ಪೂರ್ಣಾಂಕ type = matA.type();
    ಪೂರ್ಣಾಂಕ ಆಳ = CV_MAT_ಆಳ(type);
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ಸ್ಥಿರಾಂಕ ocl::Device & dev = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = dev.doubleFPConfig() > 0;

    ಆದರೆ (!doubleSupport && ಆಳ == CV_64F)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ತಾರ್ಕಿಕ haveC = matC.ಬಗೆ() != cv::ಒಳಹರಿವು_Array_2::NONE;
    ಗಾತ್ರ_2 ಗಾತ್ರ_A = matA.size(), ಗಾತ್ರ_B = matB.size(), ಗಾತ್ರ_C = haveC ? matC.size() : ಗಾತ್ರ_2(0, 0);
    ತಾರ್ಕಿಕ atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ಆದರೆ (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    ಗಾತ್ರ_2 ಗಾತ್ರ_D(((btrans) ? ಗಾತ್ರ_B.ಎತ್ತರ : ಗಾತ್ರ_B.ಅಗಲ),
               ((atrans) ? ಗಾತ್ರ_A.ಅಗಲ : ಗಾತ್ರ_A.ಎತ್ತರ));

    ಆದರೆ (atrans)
        ಗಾತ್ರ_A = ಗಾತ್ರ_2(ಗಾತ್ರ_A.ಎತ್ತರ, ಗಾತ್ರ_A.ಅಗಲ);
    ಆದರೆ (btrans)
        ಗಾತ್ರ_B = ಗಾತ್ರ_2(ಗಾತ್ರ_B.ಎತ್ತರ, ಗಾತ್ರ_B.ಅಗಲ);
    ಆದರೆ (haveC && ctrans)
        ಗಾತ್ರ_C = ಗಾತ್ರ_2(ಗಾತ್ರ_C.ಎತ್ತರ, ಗಾತ್ರ_C.ಅಗಲ);

    CV_CheckEQ(ಗಾತ್ರ_A.ಅಗಲ, ಗಾತ್ರ_B.ಎತ್ತರ, "");
    ಆದರೆ (haveC)
        CV_CheckEQ(ಗಾತ್ರ_C, ಗಾತ್ರ_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(ಗಾತ್ರ_D, type);
    UMat D = matD.getUMat();

    ತಾರ್ಕಿಕ isPropagatedC2D = ಸುಳ್ಳು;  // D content is updated with C / C.t()

    ಆದರೆ (dev.intelSubgroupsSupport() && (ಆಳ == CV_32F) && cn == 1)
    {
        ಆದರೆ (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copy_ಗಮ್ಯ(D);
            isPropagatedC2D = ನಿಜ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            beta = 0.0;
        }

        ತಾರ್ಕಿಕ res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, ಗಾತ್ರ_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        ಆದರೆ (res)
            ಹಿಂತಿರುಗಿ ನಿಜ;
        // fallback on generic OpenCL code
    }

    ಆದರೆ (ಗಾತ್ರ_D.ಅಗಲ < 8 || ಗಾತ್ರ_D.ಎತ್ತರ < 8)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    String opts;

    ಪೂರ್ಣಾಂಕ wg_ಗಾತ್ರ = (ಪೂರ್ಣಾಂಕ)dev.max_Work_ಗುಂಪು_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_Dmin = std::min(ಗಾತ್ರ_D.ಅಗಲ, ಗಾತ್ರ_D.ಎತ್ತರ);
    wg_ಗಾತ್ರ = std::min(wg_ಗಾತ್ರ, ಗಾತ್ರ_Dmin * ಗಾತ್ರ_Dmin);
    ಪೂರ್ಣಾಂಕ ಖಂಡ_ಗಾತ್ರ_3 = (wg_ಗಾತ್ರ / (32*cn) < 32) ? (wg_ಗಾತ್ರ / (16*cn) < 16) ? (wg_ಗಾತ್ರ / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    ಆದರೆ (atrans)
        A = A.t();

    ಆದರೆ (btrans)
        B = B.t();

    ಆದರೆ (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_ಗಮ್ಯ(D);

    ಪೂರ್ಣಾಂಕ ಸದಿಶ_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    ಪೂರ್ಣಾಂಕ kercn = ocl::check_Optimal_ಸದಿಶ_ಅಗಲ(ಸದಿಶ_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_ಗಮ್ಯ_Str(type), ocl::type_ಗಮ್ಯ_Str(ಆಳ), ocl::type_ಗಮ್ಯ_Str(CV_MAKETYPE(ಆಳ, kercn)),
                      cn, kercn, ಖಂಡ_ಗಾತ್ರ_3,
                      (ಗಾತ್ರ_A.ಅಗಲ % ಖಂಡ_ಗಾತ್ರ_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ (ಆಳ == CV_64F)
        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(A),
               ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               ಗಾತ್ರ_A.ಅಗಲ, alpha, beta);
    ಇಲ್ಲದಿದ್ದರೆ
        k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(A),
               ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               ಗಾತ್ರ_A.ಅಗಲ, (ದಶಮಾಂಶ)alpha, (ದಶಮಾಂಶ)beta);

    size_t globalsize[2] = { (size_t)ಗಾತ್ರ_D.ಅಗಲ * cn / kercn, (size_t)ಗಾತ್ರ_D.ಎತ್ತರ};
    size_t localsize[2] = { (size_t)ಖಂಡ_ಗಾತ್ರ_3, (size_t)ಖಂಡ_ಗಾತ್ರ_3};

    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಂಡ_ಗಾತ್ರ_3 !=1 ? localsize : ಖಾಲಿ, ಸುಳ್ಳು);
}
#ಆದರೆ_ಅಂತ್ಯ


ನಾಮಾಕಾಶ hal {

ಶೂನ್ಯ gemm32f(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
             ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
             ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags));
#ಇಲ್ಲದಿದ್ದರೆ
    CV_CPU_DISPATCH(gemm32f, (src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ gemm64f(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
             ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
             ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags));
#ಇಲ್ಲದಿದ್ದರೆ
    CV_CPU_DISPATCH(gemm64f, (src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ gemm32fc(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2, size_t src2_ಹೆಜ್ಜೆ,
              ದಶಮಾಂಶ alpha, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src3, size_t src3_ಹೆಜ್ಜೆ, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ* dst, size_t dst_ಹೆಜ್ಜೆ,
              ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags));
#ಇಲ್ಲದಿದ್ದರೆ
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ gemm64fc(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src1, size_t src1_ಹೆಜ್ಜೆ, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src2, size_t src2_ಹೆಜ್ಜೆ,
              ದ್ವಿಗುಣ alpha, ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ* src3, size_t src3_ಹೆಜ್ಜೆ, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_ಹೆಜ್ಜೆ,
              ಪೂರ್ಣಾಂಕ m_a, ಪೂರ್ಣಾಂಕ n_a, ಪೂರ್ಣಾಂಕ n_d, ಪೂರ್ಣಾಂಕ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags));
#ಇಲ್ಲದಿದ್ದರೆ
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_ಹೆಜ್ಜೆ, src2, src2_ಹೆಜ್ಜೆ, alpha, src3, src3_ಹೆಜ್ಜೆ, beta, dst, dst_ಹೆಜ್ಜೆ, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#ಆದರೆ_ಅಂತ್ಯ
}

} // namespace hal

ಶೂನ್ಯ gemm(ಒಳಹರಿವು_Array matA, ಒಳಹರಿವು_Array matB, ದ್ವಿಗುಣ alpha,
          ಒಳಹರಿವು_Array matC, ದ್ವಿಗುಣ beta, OutputArray _matD, ಪೂರ್ಣಾಂಕ flags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CLAMDBLAS
    CV_OCL_ಸತತ(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ() > 20 && matA.ಅಡ್ಡಸಾಲುಗಳು() > 20 && matB.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    CV_OCL_ಸತತ(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#ಆದರೆ_ಅಂತ್ಯ

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    ಗಾತ್ರ_2 a_ಗಾತ್ರ = A.size(), d_ಗಾತ್ರ;
    ಪೂರ್ಣಾಂಕ len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    ಆಯ್ಕೆ( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ಸ್ಥಿತಿ 0:
        d_ಗಾತ್ರ = ಗಾತ್ರ_2( B.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, a_ಗಾತ್ರ.ಎತ್ತರ );
        len = B.ಅಡ್ಡಸಾಲುಗಳು;
        CV_Assert( a_ಗಾತ್ರ.ಅಗಲ == len );
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 1:
        d_ಗಾತ್ರ = ಗಾತ್ರ_2( B.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, a_ಗಾತ್ರ.ಅಗಲ );
        len = B.ಅಡ್ಡಸಾಲುಗಳು;
        CV_Assert( a_ಗಾತ್ರ.ಎತ್ತರ == len );
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 2:
        d_ಗಾತ್ರ = ಗಾತ್ರ_2( B.ಅಡ್ಡಸಾಲುಗಳು, a_ಗಾತ್ರ.ಎತ್ತರ );
        len = B.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        CV_Assert( a_ಗಾತ್ರ.ಅಗಲ == len );
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 3:
        d_ಗಾತ್ರ = ಗಾತ್ರ_2( B.ಅಡ್ಡಸಾಲುಗಳು, a_ಗಾತ್ರ.ಅಗಲ );
        len = B.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        CV_Assert( a_ಗಾತ್ರ.ಎತ್ತರ == len );
        ನಿಲ್ಲಿಸು;
    }

    ಆದರೆ( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.ಅಡ್ಡಸಾಲುಗಳು == d_ಗಾತ್ರ.ಎತ್ತರ && C.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == d_ಗಾತ್ರ.ಅಗಲ) ||
             ((flags&GEMM_3_T) != 0 && C.ಅಡ್ಡಸಾಲುಗಳು == d_ಗಾತ್ರ.ಅಗಲ && C.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == d_ಗಾತ್ರ.ಎತ್ತರ)));
    }

    _matD.create( d_ಗಾತ್ರ.ಎತ್ತರ, d_ಗಾತ್ರ.ಅಗಲ, type );
    Mat D = _matD.getMat();
    ಆದರೆ( (flags & GEMM_3_T) != 0 && C.ದತ್ತಾಂಶ == D.ದತ್ತಾಂಶ )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    ಆದರೆ( D.ದತ್ತಾಂಶ == A.ದತ್ತಾಂಶ || D.ದತ್ತಾಂಶ == B.ದತ್ತಾಂಶ )
    {
        DProxy = Mat(d_ಗಾತ್ರ.ಎತ್ತರ, d_ಗಾತ್ರ.ಅಗಲ, D.type());
        DProxyPtr = &DProxy;
    }

    ಆದರೆ( type == CV_32FC1 )
        hal::gemm32f(A.ptr<ದಶಮಾಂಶ>(), A.ಹೆಜ್ಜೆ, B.ptr<ದಶಮಾಂಶ>(), B.ಹೆಜ್ಜೆ, static_cast<ದಶಮಾಂಶ>(alpha),
                     C.ptr<ದಶಮಾಂಶ>(), C.ಹೆಜ್ಜೆ, static_cast<ದಶಮಾಂಶ>(beta),
                     DProxyPtr->ptr<ದಶಮಾಂಶ>(), DProxyPtr->ಹೆಜ್ಜೆ,
                     a_ಗಾತ್ರ.ಎತ್ತರ, a_ಗಾತ್ರ.ಅಗಲ, DProxyPtr->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, flags);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_64FC1 )
        hal::gemm64f(A.ptr<ದ್ವಿಗುಣ>(), A.ಹೆಜ್ಜೆ, B.ptr<ದ್ವಿಗುಣ>(), B.ಹೆಜ್ಜೆ, alpha,
                     C.ptr<ದ್ವಿಗುಣ>(), C.ಹೆಜ್ಜೆ, beta,
                     DProxyPtr->ptr<ದ್ವಿಗುಣ>(), DProxyPtr->ಹೆಜ್ಜೆ,
                     a_ಗಾತ್ರ.ಎತ್ತರ, a_ಗಾತ್ರ.ಅಗಲ, DProxyPtr->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, flags);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<ದಶಮಾಂಶ>(), A.ಹೆಜ್ಜೆ, B.ptr<ದಶಮಾಂಶ>(), B.ಹೆಜ್ಜೆ, static_cast<ದಶಮಾಂಶ>(alpha),
                      C.ptr<ದಶಮಾಂಶ>(), C.ಹೆಜ್ಜೆ, static_cast<ದಶಮಾಂಶ>(beta),
                      DProxyPtr->ptr<ದಶಮಾಂಶ>(), DProxyPtr->ಹೆಜ್ಜೆ,
                      a_ಗಾತ್ರ.ಎತ್ತರ, a_ಗಾತ್ರ.ಅಗಲ, DProxyPtr->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, flags);
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<ದ್ವಿಗುಣ>(), A.ಹೆಜ್ಜೆ, B.ptr<ದ್ವಿಗುಣ>(), B.ಹೆಜ್ಜೆ, alpha,
                      C.ptr<ದ್ವಿಗುಣ>(), C.ಹೆಜ್ಜೆ, beta,
                      DProxyPtr->ptr<ದ್ವಿಗುಣ>(), DProxyPtr->ಹೆಜ್ಜೆ,
                      a_ಗಾತ್ರ.ಎತ್ತರ, a_ಗಾತ್ರ.ಅಗಲ, DProxyPtr->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, flags);
    }

    ಆದರೆ(DProxyPtr != &D)
        DProxyPtr->copy_ಗಮ್ಯ(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ಸ್ಥಿರ TransformFunc getTransformFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಸ್ಥಿರ TransformFunc getDiagTransformFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ transform(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಒಳಹರಿವು_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ಪೂರ್ಣಾಂಕ ಆಳ = src.ಆಳ(), scn = src.channels(), dcn = m.ಅಡ್ಡಸಾಲುಗಳು;
    CV_Assert( scn == m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ || scn + 1 == m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );
    ತಾರ್ಕಿಕ isDiag = ಸುಳ್ಳು;

    _dst.create_Same_ಗಾತ್ರ( src, CV_MAKETYPE(ಆಳ, dcn) );
    Mat dst = _dst.getMat();

    ಆದರೆ (src.ದತ್ತಾಂಶ == dst.ದತ್ತಾಂಶ)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    ಪೂರ್ಣಾಂಕ mtype = ಆಳ == CV_32S || ಆಳ == CV_64F ? CV_64F : CV_32F;
    Auto_ಬಫರ್<ದ್ವಿಗುಣ> _mbuf;
    ದ್ವಿಗುಣ* mbuf;

    ಆದರೆ( !m.isContinuous() || m.type() != mtype || m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.ದತ್ತಾಂಶ();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.ಒಟ್ಟು()*tmp.elem_ಗಾತ್ರ());
        ಆದರೆ( m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == scn+1 )
            m.convert_ಗಮ್ಯ(tmp, mtype);
        ಇಲ್ಲದಿದ್ದರೆ
        {
            Mat tmppart = tmp.ಲಂಬಸಾಲು_Range_2(0, m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
            m.convert_ಗಮ್ಯ(tmppart, mtype);
        }
        m = tmp;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        mbuf = m.ptr<ದ್ವಿಗುಣ>();

    ಆದರೆ( scn == dcn )
    {
        ಪೂರ್ಣಾಂಕ i, j;
        ದ್ವಿಗುಣ eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        ಆದರೆ( scn == 1 )
        {
            ದ್ವಿಗುಣ alpha, beta;
            ಆದರೆ( mtype == CV_32F )
                alpha = m.at<ದಶಮಾಂಶ>(0), beta = m.at<ದಶಮಾಂಶ>(1);
            ಇಲ್ಲದಿದ್ದರೆ
                alpha = m.at<ದ್ವಿಗುಣ>(0), beta = m.at<ದ್ವಿಗುಣ>(1);
            src.convert_ಗಮ್ಯ(dst, dst.type(), alpha, beta);
            ಹಿಂತಿರುಗಿ;
        }

        ಗಾಗಿ( i = 0, isDiag = ನಿಜ; isDiag && i < scn; i++ )
        {
            ಗಾಗಿ( j = 0; isDiag && j < scn; j++ )
            {
                ದ್ವಿಗುಣ v = mtype == CV_32F ? m.at<ದಶಮಾಂಶ>(i, j) : m.at<ದ್ವಿಗುಣ>(i, j);
                ಆದರೆ( i != j && fabs(v) > eps )
                    isDiag = ಸುಳ್ಳು;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(ಆಳ): getTransformFunc(ಆಳ);
    CV_Assert( func != 0 );

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, ಒಟ್ಟು = it.size;

    ಗಾಗಿ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ಸ್ಥಿರ TransformFunc getPerspectiveTransform(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ perspectiveTransform(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಒಳಹರಿವು_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ಪೂರ್ಣಾಂಕ ಆಳ = src.ಆಳ(), scn = src.channels(), dcn = m.ಅಡ್ಡಸಾಲುಗಳು-1;
    CV_Assert( scn + 1 == m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );
    CV_Assert( ಆಳ == CV_32F || ಆಳ == CV_64F );

    _dst.create_Same_ಗಾತ್ರ( src, CV_MAKETYPE(ಆಳ, dcn) );
    Mat dst = _dst.getMat();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ mtype = CV_64F;
    Auto_ಬಫರ್<ದ್ವಿಗುಣ> _mbuf;
    ದ್ವಿಗುಣ* mbuf = m.ptr<ದ್ವಿಗುಣ>();

    ಆದರೆ( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.ದತ್ತಾಂಶ();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convert_ಗಮ್ಯ(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(ಆಳ);
    CV_Assert( func != 0 );

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, ಒಟ್ಟು = it.size;

    ಗಾಗಿ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_scale_ಕೂಡಿಸು( ಒಳಹರಿವು_Array _src1, ದ್ವಿಗುಣ alpha, ಒಳಹರಿವು_Array _src2, OutputArray _dst, ಪೂರ್ಣಾಂಕ type )
{
    ಸ್ಥಿರಾಂಕ ocl::Device & d = ocl::Device::getDefault();

    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    ಗಾತ್ರ_2 size = _src1.size();
    ಪೂರ್ಣಾಂಕ ಆಳ = CV_MAT_ಆಳ(type);
    ಆದರೆ ( (!doubleSupport && ಆಳ == CV_64F) || size != _src2.size() )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    _dst.create(size, type);
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(type), wdepth = std::max(ಆಳ, CV_32F);
    ಪೂರ್ಣಾಂಕ kercn = ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ_Max(_src1, _src2, _dst),
        ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = d.isIntel() ? 4 : 1;

    ಅಕ್ಷರ cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)), ಆಳ,
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(ಆಳ, wdepth, kercn, cvt[0], ಗಾತ್ರ(cvt[0])),
                         ocl::convertTypeStr(wdepth, ಆಳ, kercn, cvt[1], ಗಾತ್ರ(cvt[1])),
                         ocl::type_ಗಮ್ಯ_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src1),
            src2arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ಆದರೆ (wdepth == CV_32F)
        k.ವಾದಗಳು(src1arg, src2arg, dstarg, (ದಶಮಾಂಶ)alpha);
    ಇಲ್ಲದಿದ್ದರೆ
        k.ವಾದಗಳು(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn / kercn, ((size_t)dst.ಅಡ್ಡಸಾಲುಗಳು + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ Scale_ಕೂಡಿಸು_Func get_Scale_ಕೂಡಿಸು_Func(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_ಕೂಡಿಸು_Func, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ scale_ಕೂಡಿಸು(ಒಳಹರಿವು_Array _src1, ದ್ವಿಗುಣ alpha, ಒಳಹರಿವು_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ type = _src1.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_ಸತತ(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_ಕೂಡಿಸು(_src1, alpha, _src2, _dst, type))

    ಆದರೆ( ಆಳ != CV_32F && ಆಳ != CV_64F )
    {
        ಕೂಡಿಸು_Weighted(_src1, alpha, _src2, 1, 0, _dst, ಆಳ);
        ಹಿಂತಿರುಗಿ;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    ದಶಮಾಂಶ falpha = (ದಶಮಾಂಶ)alpha;
    ಶೂನ್ಯ* palpha = ಆಳ == CV_32F ? (ಶೂನ್ಯ*)&falpha : (ಶೂನ್ಯ*)&alpha;

    Scale_ಕೂಡಿಸು_Func func = get_Scale_ಕೂಡಿಸು_Func(ಆಳ);
    CV_Assert(func);

    ಆದರೆ (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.ಒಟ್ಟು()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (ಪೂರ್ಣಾಂಕ)len, palpha);
        ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    ಗಾಗಿ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (ಪೂರ್ಣಾಂಕ)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

ಶೂನ್ಯ calc_Covar_ಮಾತೃಕೆ( ಸ್ಥಿರಾಂಕ Mat* ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ nsamples, Mat& covar, Mat& ಸರಾಸರಿ, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( ದತ್ತಾಂಶ, nsamples > 0 );
    ಗಾತ್ರ_2 size = ದತ್ತಾಂಶ[0].size();
    ಪೂರ್ಣಾಂಕ sz = size.ಅಗಲ * size.ಎತ್ತರ, esz = (ಪೂರ್ಣಾಂಕ)ದತ್ತಾಂಶ[0].elem_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ type = ದತ್ತಾಂಶ[0].type();
    Mat ಸರಾಸರಿ_2;
    ctype = std::max(std::max(CV_MAT_ಆಳ(ctype >= 0 ? ctype : type), ಸರಾಸರಿ.ಆಳ()), CV_32F);

    ಆದರೆ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( ಸರಾಸರಿ.size() == size );
        ಆದರೆ( ಸರಾಸರಿ.isContinuous() && ಸರಾಸರಿ.type() == ctype )
            ಸರಾಸರಿ_2 = ಸರಾಸರಿ.reshape(1, 1);
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಸರಾಸರಿ.convert_ಗಮ್ಯ(ಸರಾಸರಿ_2, ctype);
            ಸರಾಸರಿ_2 = ಸರಾಸರಿ_2.reshape(1, 1);
        }
    }

    Mat ದತ್ತಾಂಶ_3(nsamples, sz, type);

    ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( ದತ್ತಾಂಶ[i].size() == size, ದತ್ತಾಂಶ[i].type() == type );
        ಆದರೆ( ದತ್ತಾಂಶ[i].isContinuous() )
            ಸ್ಮರಣೆ_ನಕಲು( ದತ್ತಾಂಶ_3.ptr(i), ದತ್ತಾಂಶ[i].ptr(), sz*esz );
        ಇಲ್ಲದಿದ್ದರೆ
        {
            Mat ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲು(size.ಎತ್ತರ, size.ಅಗಲ, type, ದತ್ತಾಂಶ_3.ptr(i));
            ದತ್ತಾಂಶ[i].copy_ಗಮ್ಯ(ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲು);
        }
    }

    calc_Covar_ಮಾತೃಕೆ( ದತ್ತಾಂಶ_3, covar, ಸರಾಸರಿ_2, (flags & ~(cv::COVAR_ಅಡ್ಡಸಾಲುಗಳು|cv::COVAR_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)) | cv::COVAR_ಅಡ್ಡಸಾಲುಗಳು, ctype );
    ಆದರೆ( (flags & cv::COVAR_USE_AVG) == 0 )
        ಸರಾಸರಿ = ಸರಾಸರಿ_2.reshape(1, size.ಎತ್ತರ);
}

ಶೂನ್ಯ calc_Covar_ಮಾತೃಕೆ( ಒಳಹರಿವು_Array _src, OutputArray _covar, ಒಳಹರಿವು_Output_Array ಸರಾಸರಿ, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ctype )
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ(_src.ಬಗೆ() == ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT || _src.ಬಗೆ() == ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_ಸದಿಶ(src);

        CV_Assert( src.size() > 0 );

        ಗಾತ್ರ_2 size = src[0].size();
        ಪೂರ್ಣಾಂಕ type = src[0].type();

        ctype = std::max(std::max(CV_MAT_ಆಳ(ctype >= 0 ? ctype : type), ಸರಾಸರಿ.ಆಳ()), CV_32F);

        Mat ದತ್ತಾಂಶ_3(static_cast<ಪೂರ್ಣಾಂಕ>(src.size()), size.area(), type);

        ಪೂರ್ಣಾಂಕ i = 0;
        ಗಾಗಿ(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲು(size.ಎತ್ತರ, size.ಅಗಲ, type, ದತ್ತಾಂಶ_3.ptr(i));
            (*each).copy_ಗಮ್ಯ(ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲು);
        }

        Mat ಸರಾಸರಿ_2;
        ಆದರೆ( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( ಸರಾಸರಿ.size() == size );
            Mat mean0 = ಸರಾಸರಿ.getMat();

            ಆದರೆ( mean0.isContinuous() && mean0.type() == ctype )
                ಸರಾಸರಿ_2 = mean0.reshape(1, 1);
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mean0.convert_ಗಮ್ಯ(ಸರಾಸರಿ_2, ctype);
                ಸರಾಸರಿ_2 = ಸರಾಸರಿ_2.reshape(1, 1);
            }
        }

        calc_Covar_ಮಾತೃಕೆ( ದತ್ತಾಂಶ_3, _covar, ಸರಾಸರಿ_2, (flags & ~(cv::COVAR_ಅಡ್ಡಸಾಲುಗಳು|cv::COVAR_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)) | cv::COVAR_ಅಡ್ಡಸಾಲುಗಳು, ctype );

        ಆದರೆ( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            ಸರಾಸರಿ_2 = ಸರಾಸರಿ_2.reshape(1, size.ಎತ್ತರ);
            ಸರಾಸರಿ_2.copy_ಗಮ್ಯ(ಸರಾಸರಿ);
        }
        ಹಿಂತಿರುಗಿ;
    }

    Mat ದತ್ತಾಂಶ = _src.getMat(), ಸರಾಸರಿ_2;
    CV_Assert( ((flags & cv::COVAR_ಅಡ್ಡಸಾಲುಗಳು) != 0) ^ ((flags & cv::COVAR_ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ) != 0) );
    ತಾರ್ಕಿಕ take_ಅಡ್ಡಸಾಲುಗಳು = (flags & cv::COVAR_ಅಡ್ಡಸಾಲುಗಳು) != 0;
    ಪೂರ್ಣಾಂಕ type = ದತ್ತಾಂಶ.type();
    ಪೂರ್ಣಾಂಕ nsamples = take_ಅಡ್ಡಸಾಲುಗಳು ? ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು : ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    CV_Assert( nsamples > 0 );
    ಗಾತ್ರ_2 size = take_ಅಡ್ಡಸಾಲುಗಳು ? ಗಾತ್ರ_2(ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, 1) : ಗಾತ್ರ_2(1, ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು);

    ಆದರೆ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        ಸರಾಸರಿ_2 = ಸರಾಸರಿ.getMat();
        ctype = std::max(std::max(CV_MAT_ಆಳ(ctype >= 0 ? ctype : type), ಸರಾಸರಿ_2.ಆಳ()), CV_32F);
        CV_Assert( ಸರಾಸರಿ_2.size() == size );
        ಆದರೆ( ಸರಾಸರಿ_2.type() != ctype )
        {
            ಸರಾಸರಿ.create(ಸರಾಸರಿ_2.size(), ctype);
            Mat tmp = ಸರಾಸರಿ.getMat();
            ಸರಾಸರಿ_2.convert_ಗಮ್ಯ(tmp, ctype);
            ಸರಾಸರಿ_2 = tmp;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ctype = std::max(CV_MAT_ಆಳ(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, ಸರಾಸರಿ, take_ಅಡ್ಡಸಾಲುಗಳು ? 0 : 1, REDUCE_AVG, ctype );
        ಸರಾಸರಿ_2 = ಸರಾಸರಿ.getMat();
    }

    mulTransposed( ದತ್ತಾಂಶ, _covar, ((flags & cv::COVAR_ಸಾಮಾನ್ಯ) == 0) ^ take_ಅಡ್ಡಸಾಲುಗಳು,
        ಸರಾಸರಿ_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ಸ್ಥಿರ MahalanobisImplFunc getMahalanobisImplFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (ಆಳ));
#ಇಲ್ಲದಿದ್ದರೆ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (ಆಳ),
        CV_CPU_DISPATCH_MODES_ALL);
#ಆದರೆ_ಅಂತ್ಯ
}


ದ್ವಿಗುಣ Mahalanobis(ಒಳಹರಿವು_Array _v1, ಒಳಹರಿವು_Array _v2, ಒಳಹರಿವು_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    ಪೂರ್ಣಾಂಕ type = v1.type(), ಆಳ = v1.ಆಳ();
    ಗಾತ್ರ_2 sz = v1.size();
    ಪೂರ್ಣಾಂಕ len = sz.ಅಗಲ*sz.ಎತ್ತರ*v1.channels();
    Auto_ಬಫರ್<ದ್ವಿಗುಣ> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.ಅಡ್ಡಸಾಲುಗಳು && len == icovar.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ );

    sz.ಅಗಲ *= v1.channels();
    ಆದರೆ( v1.isContinuous() && v2.isContinuous() )
    {
        sz.ಅಗಲ *= sz.ಎತ್ತರ;
        sz.ಎತ್ತರ = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(ಆಳ);
    CV_Assert(func);

    ದ್ವಿಗುಣ ಫಲಿತಾಂಶ = func(v1, v2, icovar, buf.ದತ್ತಾಂಶ(), len);
    ಹಿಂತಿರುಗಿ std::ವರ್ಗಮೂಲ(ಫಲಿತಾಂಶ);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ಸ್ಥಿರ MulTransposedFunc getMulTransposedFunc(ಪೂರ್ಣಾಂಕ stype, ಪೂರ್ಣಾಂಕ dtype, ತಾರ್ಕಿಕ ata)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#ಇಲ್ಲದಿದ್ದರೆ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ mulTransposed(ಒಳಹರಿವು_Array _src, OutputArray _dst, ತಾರ್ಕಿಕ ata,
                   ಒಳಹರಿವು_Array _delta, ದ್ವಿಗುಣ scale, ಪೂರ್ಣಾಂಕ dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ gemm_level = 100; // boundary above which GEMM is faster.
    ಪೂರ್ಣಾಂಕ stype = src.type();
    dtype = std::max(std::max(CV_MAT_ಆಳ(dtype >= 0 ? dtype : stype), delta.ಆಳ()), CV_32F);
    CV_Assert( src.channels() == 1 );

    ಆದರೆ( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.ಅಡ್ಡಸಾಲುಗಳು == src.ಅಡ್ಡಸಾಲುಗಳು || delta.ಅಡ್ಡಸಾಲುಗಳು == 1),
            (delta.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ || delta.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1));
        ಆದರೆ( delta.type() != dtype )
            delta.convert_ಗಮ್ಯ(delta, dtype);
    }

    ಪೂರ್ಣಾಂಕ dsize = ata ? src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ : src.ಅಡ್ಡಸಾಲುಗಳು;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    ಆದರೆ( src.ದತ್ತಾಂಶ == dst.ದತ್ತಾಂಶ || (stype == dtype &&
        (dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ >= gemm_level && dst.ಅಡ್ಡಸಾಲುಗಳು >= gemm_level &&
         src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ >= gemm_level && src.ಅಡ್ಡಸಾಲುಗಳು >= gemm_level)))
    {
        Mat src2;
        ಸ್ಥಿರಾಂಕ Mat* tsrc = &src;
        ಆದರೆ( !delta.empty() )
        {
            ಆದರೆ( delta.size() == src.size() )
                ಕಳೆ( src, delta, src2 );
            ಇಲ್ಲದಿದ್ದರೆ
            {
                repeat(delta, src.ಅಡ್ಡಸಾಲುಗಳು/delta.ಅಡ್ಡಸಾಲುಗಳು, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/delta.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src2);
                ಕಳೆ( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        ಆದರೆ( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, ಸುಳ್ಳು );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_8u(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ಸ್ಥಿರಾಂಕ uchar*)src1, (ಸ್ಥಿರಾಂಕ uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_8s(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ಸ್ಥಿರಾಂಕ schar*)src1, (ಸ್ಥಿರಾಂಕ schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_16u(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ಸ್ಥಿರಾಂಕ ushort*)src1, (ಸ್ಥಿರಾಂಕ ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_16s(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ*)src1, (ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_32s(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)src1, (ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_32f(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)src1, (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_64f(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಂಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)src1, (ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ (*DotProdFunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಂಕ len);

ಸ್ಥಿರ DotProdFunc getDotProdFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ DotProdFunc dotProdTab[CV_ಆಳ_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    ಹಿಂತಿರುಗಿ dotProdTab[ಆಳ];
}

ದ್ವಿಗುಣ Mat::dot(ಒಳಹರಿವು_Array _mat) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    ಪೂರ್ಣಾಂಕ cn = channels();
    ಆದರೆ (ಇದು->dims <= 2)
    {
        ದ್ವಿಗುಣ ಗುಣಲಬ್ಧ = 0;
        CALL_HAL_RET(dot_ಗುಣಲಬ್ಧ, cv_hal_dot_ಗುಣಲಬ್ಧ, ಗುಣಲಬ್ಧ, ಇದು->ದತ್ತಾಂಶ, ಇದು->ಹೆಜ್ಜೆ, mat.ದತ್ತಾಂಶ, mat.ಹೆಜ್ಜೆ,
                     ಇದು->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn, ಇದು->ಅಡ್ಡಸಾಲುಗಳು, ಇದು->ಆಳ());
    }

    DotProdFunc func = getDotProdFunc(ಆಳ());
    CV_Assert(func != 0 );

    ಆದರೆ( isContinuous() && mat.isContinuous() )
    {
        size_t len = ಒಟ್ಟು()*cn;
        ಆದರೆ( len == (size_t)(ಪೂರ್ಣಾಂಕ)len )
            ಹಿಂತಿರುಗಿ func(ದತ್ತಾಂಶ, mat.ದತ್ತಾಂಶ, (ಪೂರ್ಣಾಂಕ)len);
    }

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {ಇದು, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ len = (ಪೂರ್ಣಾಂಕ)(it.size*cn);
    ದ್ವಿಗುಣ r = 0;

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    ಹಿಂತಿರುಗಿ r;
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_dot( ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, ದ್ವಿಗುಣ & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    ಪೂರ್ಣಾಂಕ type = src1.type(), ಆಳ = CV_MAT_ಆಳ(type),
            kercn = ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(src1, src2);
    ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ಆದರೆ ( !doubleSupport && ಆಳ == CV_64F )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ಗುಂಪು_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ ddepth = std::max(CV_32F, ಆಳ);

    ಪೂರ್ಣಾಂಕ wgs2_aligned = 1;
    ಆಗ (wgs2_aligned < (ಪೂರ್ಣಾಂಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ಅಕ್ಷರ cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ಆಳ, kercn)), ocl::type_ಗಮ್ಯ_Str(ಆಳ),
                         ocl::type_ಗಮ್ಯ_Str(ddepth), ocl::type_ಗಮ್ಯ_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(ಆಳ, ddepth, kercn, cvt, ಗಾತ್ರ(cvt)),
                         (ಪೂರ್ಣಾಂಕ)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src1),
            src2arg = ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.ವಾದಗಳು(src1arg, src1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (ಪೂರ್ಣಾಂಕ)src1.ಒಟ್ಟು(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    ಆದರೆ (k.ಸತತ(1, &globalsize, &wgs, ನಿಜ))
    {
        res = ಮೊತ್ತ(db.getMat(ACCESS_READ))[0];
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

#ಆದರೆ_ಅಂತ್ಯ

ದ್ವಿಗುಣ UMat::dot(ಒಳಹರಿವು_Array m) ಸ್ಥಿರಾಂಕ
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_ಗಾತ್ರ(*ಇದು) && m.type() == type());

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ದ್ವಿಗುಣ r = 0;
    CV_OCL_ಸತತ_2(dims <= 2, ocl_dot(*ಇದು, m, r), r)
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
