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

#ifdef HAVE_CLAMDBLAS

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_gemm_amdblas( ಒಳಹರಿವು_Array matA, ಒಳಹರಿವು_Array matB, ದ್ವಿಗುಣ alpha,
                      ಒಳಹರಿವು_Array matC, ದ್ವಿಗುಣ beta, OutputArray matD, ಪೂರ್ಣಾಙ್ಕ flags )
{
    ಪೂರ್ಣಾಙ್ಕ type = matA.type(), esz = CV_ELEM_SIZE(type);
    ತಾರ್ಕಿಕ haveC = matC.kind() != cv::ಒಳಹರಿವು_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    ತಾರ್ಕಿಕ atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ಯದಿ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    ಯದಿ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    ಯದಿ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    ಯದಿ ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    ಯದಿ (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಯದಿ (haveC)
    {
        UMat C = matC.getUMat();
        ಯದಿ (!ocl::internal::isCLBuffer(C))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಯದಿ (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    ಅನ್ಯಥಾ
        D.setTo(Scalar::all(0));

    ಪೂರ್ಣಾಙ್ಕ M = sizeD.height, N = sizeD.width, K = sizeA.width;
    ಪೂರ್ಣಾಙ್ಕ lda = (ಪೂರ್ಣಾಙ್ಕ)A.step / esz, ldb = (ಪೂರ್ಣಾಙ್ಕ)B.step / esz, ldc = (ಪೂರ್ಣಾಙ್ಕ)D.step / esz;
    ಪೂರ್ಣಾಙ್ಕ offa = (ಪೂರ್ಣಾಙ್ಕ)A.offset / esz, offb = (ಪೂರ್ಣಾಙ್ಕ)B.offset / esz, offc = (ಪೂರ್ಣಾಙ್ಕ)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    ಯದಿ (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (ಧ್ರುವ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ಧ್ರುವ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಅನ್ಯಥಾ ಯದಿ (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (ಧ್ರುವ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ಧ್ರುವ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಅನ್ಯಥಾ ಯದಿ (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (ಧ್ರುವ cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ಧ್ರುವ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    }
    ಅನ್ಯಥಾ ಯದಿ (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (ಧ್ರುವ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ಧ್ರುವ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    }
    ಅನ್ಯಥಾ
        CV_Error(Error::StsUnsupportedFormat, "");

    ಪ್ರತ್ಯಾಗಮ status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_gemm( ಒಳಹರಿವು_Array matA, ಒಳಹರಿವು_Array matB, ದ್ವಿಗುಣ alpha,
                      ಒಳಹರಿವು_Array matC, ದ್ವಿಗುಣ beta, OutputArray matD, ಪೂರ್ಣಾಙ್ಕ flags )
{
    ಪೂರ್ಣಾಙ್ಕ type = matA.type();
    ಪೂರ್ಣಾಙ್ಕ depth = CV_MAT_DEPTH(type);
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ಧ್ರುವ ocl::Device & dev = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = dev.doubleFPConfig() > 0;

    ಯದಿ (!doubleSupport && depth == CV_64F)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ತಾರ್ಕಿಕ haveC = matC.kind() != cv::ಒಳಹರಿವು_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    ತಾರ್ಕಿಕ atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ಯದಿ (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    ಯದಿ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    ಯದಿ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    ಯದಿ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    ಯದಿ (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    ತಾರ್ಕಿಕ isPropagatedC2D = ಅಸತ್ಯ;  // D content is updated with C / C.t()

    ಯದಿ (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        ಯದಿ (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = ಸತ್ಯ;
        }
        ಅನ್ಯಥಾ
        {
            beta = 0.0;
        }

        ತಾರ್ಕಿಕ res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        ಯದಿ (res)
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        // fallback on generic OpenCL code
    }

    ಯದಿ (sizeD.width < 8 || sizeD.height < 8)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    String opts;

    ಪೂರ್ಣಾಙ್ಕ wg_size = (ಪೂರ್ಣಾಙ್ಕ)dev.maxWorkGroupSize();
    ಪೂರ್ಣಾಙ್ಕ sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    ಪೂರ್ಣಾಙ್ಕ block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    ಯದಿ (atrans)
        A = A.t();

    ಯದಿ (btrans)
        B = B.t();

    ಯದಿ (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    ಪೂರ್ಣಾಙ್ಕ vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    ಪೂರ್ಣಾಙ್ಕ kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    ಅನ್ಯಥಾ
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (ಭಿನ್ನ)alpha, (ಭಿನ್ನ)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, block_size !=1 ? localsize : ರಿಕ್ತ, ಅಸತ್ಯ);
}
#endif


ನಾಮಾಕಾಶ hal {

ಶೂನ್ಯ gemm32f(ಧ್ರುವ ಭಿನ್ನ* src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ* src2, size_t src2_step,
             ಭಿನ್ನ alpha, ಧ್ರುವ ಭಿನ್ನ* src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ* dst, size_t dst_step,
             ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ಅನ್ಯಥಾ
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ಶೂನ್ಯ gemm64f(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t src2_step,
             ದ್ವಿಗುಣ alpha, ಧ್ರುವ ದ್ವಿಗುಣ* src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_step,
             ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ಅನ್ಯಥಾ
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ಶೂನ್ಯ gemm32fc(ಧ್ರುವ ಭಿನ್ನ* src1, size_t src1_step, ಧ್ರುವ ಭಿನ್ನ* src2, size_t src2_step,
              ಭಿನ್ನ alpha, ಧ್ರುವ ಭಿನ್ನ* src3, size_t src3_step, ಭಿನ್ನ beta, ಭಿನ್ನ* dst, size_t dst_step,
              ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ಅನ್ಯಥಾ
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ಶೂನ್ಯ gemm64fc(ಧ್ರುವ ದ್ವಿಗುಣ* src1, size_t src1_step, ಧ್ರುವ ದ್ವಿಗುಣ* src2, size_t src2_step,
              ದ್ವಿಗುಣ alpha, ಧ್ರುವ ದ್ವಿಗುಣ* src3, size_t src3_step, ದ್ವಿಗುಣ beta, ದ್ವಿಗುಣ* dst, size_t dst_step,
              ಪೂರ್ಣಾಙ್ಕ m_a, ಪೂರ್ಣಾಙ್ಕ n_a, ಪೂರ್ಣಾಙ್ಕ n_d, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ಅನ್ಯಥಾ
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

ಶೂನ್ಯ gemm(ಒಳಹರಿವು_Array matA, ಒಳಹರಿವು_Array matB, ದ್ವಿಗುಣ alpha,
          ಒಳಹರಿವು_Array matC, ದ್ವಿಗುಣ beta, OutputArray _matD, ಪೂರ್ಣಾಙ್ಕ flags)
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
    ಪೂರ್ಣಾಙ್ಕ len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    ವಿಕಲ್ಪ( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ಪಕ್ಷ 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        ಭಙ್ಗ;
    ಪಕ್ಷ 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        ಭಙ್ಗ;
    ಪಕ್ಷ 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        ಭಙ್ಗ;
    ಪಕ್ಷ 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        ಭಙ್ಗ;
    }

    ಯದಿ( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    ಯದಿ( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    ಯದಿ( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    ಯದಿ( type == CV_32FC1 )
        hal::gemm32f(A.ptr<ಭಿನ್ನ>(), A.step, B.ptr<ಭಿನ್ನ>(), B.step, static_cast<ಭಿನ್ನ>(alpha),
                     C.ptr<ಭಿನ್ನ>(), C.step, static_cast<ಭಿನ್ನ>(beta),
                     DProxyPtr->ptr<ಭಿನ್ನ>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    ಅನ್ಯಥಾ ಯದಿ( type == CV_64FC1 )
        hal::gemm64f(A.ptr<ದ್ವಿಗುಣ>(), A.step, B.ptr<ದ್ವಿಗುಣ>(), B.step, alpha,
                     C.ptr<ದ್ವಿಗುಣ>(), C.step, beta,
                     DProxyPtr->ptr<ದ್ವಿಗುಣ>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    ಅನ್ಯಥಾ ಯದಿ( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<ಭಿನ್ನ>(), A.step, B.ptr<ಭಿನ್ನ>(), B.step, static_cast<ಭಿನ್ನ>(alpha),
                      C.ptr<ಭಿನ್ನ>(), C.step, static_cast<ಭಿನ್ನ>(beta),
                      DProxyPtr->ptr<ಭಿನ್ನ>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    ಅನ್ಯಥಾ
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<ದ್ವಿಗುಣ>(), A.step, B.ptr<ದ್ವಿಗುಣ>(), B.step, alpha,
                      C.ptr<ದ್ವಿಗುಣ>(), C.step, beta,
                      DProxyPtr->ptr<ದ್ವಿಗುಣ>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    ಯದಿ(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ಸ್ಥಿರ TransformFunc getTransformFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಸ್ಥಿರ TransformFunc getDiagTransformFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ transform(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಒಳಹರಿವು_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ಪೂರ್ಣಾಙ್ಕ depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    ತಾರ್ಕಿಕ isDiag = ಅಸತ್ಯ;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ಯದಿ (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    ಪೂರ್ಣಾಙ್ಕ mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<ದ್ವಿಗುಣ> _mbuf;
    ದ್ವಿಗುಣ* mbuf;

    ಯದಿ( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        ಯದಿ( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        ಅನ್ಯಥಾ
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    ಅನ್ಯಥಾ
        mbuf = m.ptr<ದ್ವಿಗುಣ>();

    ಯದಿ( scn == dcn )
    {
        ಪೂರ್ಣಾಙ್ಕ i, j;
        ದ್ವಿಗುಣ eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        ಯದಿ( scn == 1 )
        {
            ದ್ವಿಗುಣ alpha, beta;
            ಯದಿ( mtype == CV_32F )
                alpha = m.at<ಭಿನ್ನ>(0), beta = m.at<ಭಿನ್ನ>(1);
            ಅನ್ಯಥಾ
                alpha = m.at<ದ್ವಿಗುಣ>(0), beta = m.at<ದ್ವಿಗುಣ>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಪ್ರತಿ( i = 0, isDiag = ಸತ್ಯ; isDiag && i < scn; i++ )
        {
            ಪ್ರತಿ( j = 0; isDiag && j < scn; j++ )
            {
                ದ್ವಿಗುಣ v = mtype == CV_32F ? m.at<ಭಿನ್ನ>(i, j) : m.at<ದ್ವಿಗುಣ>(i, j);
                ಯದಿ( i != j && fabs(v) > eps )
                    isDiag = ಅಸತ್ಯ;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    ಧ್ರುವ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    ಪ್ರತಿ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ಪೂರ್ಣಾಙ್ಕ)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ಸ್ಥಿರ TransformFunc getPerspectiveTransform(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ perspectiveTransform(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಒಳಹರಿವು_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ಪೂರ್ಣಾಙ್ಕ depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ mtype = CV_64F;
    AutoBuffer<ದ್ವಿಗುಣ> _mbuf;
    ದ್ವಿಗುಣ* mbuf = m.ptr<ದ್ವಿಗುಣ>();

    ಯದಿ( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    ಧ್ರುವ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    ಪ್ರತಿ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ಪೂರ್ಣಾಙ್ಕ)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_scaleAdd( ಒಳಹರಿವು_Array _src1, ದ್ವಿಗುಣ alpha, ಒಳಹರಿವು_Array _src2, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ type )
{
    ಧ್ರುವ ocl::Device & d = ocl::Device::getDefault();

    ತಾರ್ಕಿಕ doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    ಪೂರ್ಣಾಙ್ಕ depth = CV_MAT_DEPTH(type);
    ಯದಿ ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    _dst.create(size, type);
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    ಪೂರ್ಣಾಙ್ಕ kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    ವರ್ಣ cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], ಪರಿಮಾಣ(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], ಪರಿಮಾಣ(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ಯದಿ (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (ಭಿನ್ನ)alpha);
    ಅನ್ಯಥಾ
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ಸ್ಥಿರ ScaleAddFunc getScaleAddFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಶೂನ್ಯ scaleAdd(ಒಳಹರಿವು_Array _src1, ದ್ವಿಗುಣ alpha, ಒಳಹರಿವು_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    ಯದಿ( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        ಪ್ರತ್ಯಾಗಮ;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    ಭಿನ್ನ falpha = (ಭಿನ್ನ)alpha;
    ಶೂನ್ಯ* palpha = depth == CV_32F ? (ಶೂನ್ಯ*)&falpha : (ಶೂನ್ಯ*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    ಯದಿ (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (ಪೂರ್ಣಾಙ್ಕ)len, palpha);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಧ್ರುವ Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    ಪ್ರತಿ( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (ಪೂರ್ಣಾಙ್ಕ)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

ಶೂನ್ಯ calcCovarMatrix( ಧ್ರುವ Mat* data, ಪೂರ್ಣಾಙ್ಕ nsamples, Mat& covar, Mat& _mean, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    ಪೂರ್ಣಾಙ್ಕ sz = size.width * size.height, esz = (ಪೂರ್ಣಾಙ್ಕ)data[0].elemSize();
    ಪೂರ್ಣಾಙ್ಕ type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    ಯದಿ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        ಯದಿ( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        ಅನ್ಯಥಾ
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        ಯದಿ( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        ಅನ್ಯಥಾ
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    ಯದಿ( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

ಶೂನ್ಯ calcCovarMatrix( ಒಳಹರಿವು_Array _src, OutputArray _covar, ಒಳಹರಿವು_Output_Array _mean, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ ctype )
{
    CV_INSTRUMENT_REGION();

    ಯದಿ(_src.kind() == ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT || _src.kind() == ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        ಪೂರ್ಣಾಙ್ಕ type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<ಪೂರ್ಣಾಙ್ಕ>(src.size()), size.area(), type);

        ಪೂರ್ಣಾಙ್ಕ i = 0;
        ಪ್ರತಿ(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        ಯದಿ( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            ಯದಿ( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            ಅನ್ಯಥಾ
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        ಯದಿ( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        ಪ್ರತ್ಯಾಗಮ;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    ತಾರ್ಕಿಕ takeRows = (flags & cv::COVAR_ROWS) != 0;
    ಪೂರ್ಣಾಙ್ಕ type = data.type();
    ಪೂರ್ಣಾಙ್ಕ nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    ಯದಿ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        ಯದಿ( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    ಅನ್ಯಥಾ
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

ಸ್ಥಿರ MahalanobisImplFunc getMahalanobisImplFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#ಅನ್ಯಥಾ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


ದ್ವಿಗುಣ Mahalanobis(ಒಳಹರಿವು_Array _v1, ಒಳಹರಿವು_Array _v2, ಒಳಹರಿವು_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    ಪೂರ್ಣಾಙ್ಕ type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    ಪೂರ್ಣಾಙ್ಕ len = sz.width*sz.height*v1.channels();
    AutoBuffer<ದ್ವಿಗುಣ> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    ಯದಿ( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    ದ್ವಿಗುಣ result = func(v1, v2, icovar, buf.data(), len);
    ಪ್ರತ್ಯಾಗಮ std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ಸ್ಥಿರ MulTransposedFunc getMulTransposedFunc(ಪೂರ್ಣಾಙ್ಕ stype, ಪೂರ್ಣಾಙ್ಕ dtype, ತಾರ್ಕಿಕ ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#ಅನ್ಯಥಾ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ಶೂನ್ಯ mulTransposed(ಒಳಹರಿವು_Array _src, OutputArray _dst, ತಾರ್ಕಿಕ ata,
                   ಒಳಹರಿವು_Array _delta, ದ್ವಿಗುಣ scale, ಪೂರ್ಣಾಙ್ಕ dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ gemm_level = 100; // boundary above which GEMM is faster.
    ಪೂರ್ಣಾಙ್ಕ stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    ಯದಿ( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        ಯದಿ( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    ಪೂರ್ಣಾಙ್ಕ dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    ಯದಿ( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        ಧ್ರುವ Mat* tsrc = &src;
        ಯದಿ( !delta.empty() )
        {
            ಯದಿ( delta.size() == src.size() )
                subtract( src, delta, src2 );
            ಅನ್ಯಥಾ
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    ಅನ್ಯಥಾ
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        ಯದಿ( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, ಅಸತ್ಯ );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_8u(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ಧ್ರುವ uchar*)src1, (ಧ್ರುವ uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_8s(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ಧ್ರುವ schar*)src1, (ಧ್ರುವ schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_16u(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ಧ್ರುವ ushort*)src1, (ಧ್ರುವ ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_16s(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ಧ್ರುವ short*)src1, (ಧ್ರುವ short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_32s(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)src1, (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_32f(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ಧ್ರುವ ಭಿನ್ನ*)src1, (ಧ್ರುವ ಭಿನ್ನ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ ದ್ವಿಗುಣ dotProd_64f(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಙ್ಕ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ಧ್ರುವ ದ್ವಿಗುಣ*)src1, (ಧ್ರುವ ದ್ವಿಗುಣ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ದ್ವಿಗುಣ (*DotProdFunc)(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, ಪೂರ್ಣಾಙ್ಕ len);

ಸ್ಥಿರ DotProdFunc getDotProdFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    ಪ್ರತ್ಯಾಗಮ dotProdTab[depth];
}

ದ್ವಿಗುಣ Mat::dot(ಒಳಹರಿವು_Array _mat) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    ಪೂರ್ಣಾಙ್ಕ cn = channels();
    ಯದಿ (ಏತತ್->dims <= 2)
    {
        ದ್ವಿಗುಣ product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, ಏತತ್->data, ಏತತ್->step, mat.data, mat.step,
                     ಏತತ್->cols * cn, ಏತತ್->rows, ಏತತ್->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    ಯದಿ( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        ಯದಿ( len == (size_t)(ಪೂರ್ಣಾಙ್ಕ)len )
            ಪ್ರತ್ಯಾಗಮ func(data, mat.data, (ಪೂರ್ಣಾಙ್ಕ)len);
    }

    ಧ್ರುವ Mat* arrays[] = {ಏತತ್, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ len = (ಪೂರ್ಣಾಙ್ಕ)(it.size*cn);
    ದ್ವಿಗುಣ r = 0;

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    ಪ್ರತ್ಯಾಗಮ r;
}


#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_dot( ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2, ದ್ವಿಗುಣ & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    ಪೂರ್ಣಾಙ್ಕ type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ಯದಿ ( !doubleSupport && depth == CV_64F )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    ಪೂರ್ಣಾಙ್ಕ ddepth = std::max(CV_32F, depth);

    ಪೂರ್ಣಾಙ್ಕ wgs2_aligned = 1;
    ಯಾವತ್ (wgs2_aligned < (ಪೂರ್ಣಾಙ್ಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ವರ್ಣ cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, ಪರಿಮಾಣ(cvt)),
                         (ಪೂರ್ಣಾಙ್ಕ)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (ಪೂರ್ಣಾಙ್ಕ)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    ಯದಿ (k.run(1, &globalsize, &wgs, ಸತ್ಯ))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

#endif

ದ್ವಿಗುಣ UMat::dot(ಒಳಹರಿವು_Array m) ಧ್ರುವ
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*ಏತತ್) && m.type() == type());

#ifdef HAVE_OPENCL
    ದ್ವಿಗುಣ r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*ಏತತ್, m, r), r)
#endif

    ಪ್ರತ್ಯಾಗಮ getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
