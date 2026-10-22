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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_core.hpp"
#অন্তর্ভাব "intel_gpu_gemm.inl.hpp"

#অন্তর্ভাব "matmul.simd.hpp"
#অন্তর্ভাব "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ifdef HAVE_CLAMDBLAS

স্থির যৌক্তিক ocl_gemm_amdblas( প্রবেশ_Array matA, প্রবেশ_Array matB, দ্বিগুণ alpha,
                      প্রবেশ_Array matC, দ্বিগুণ beta, OutputArray matD, পূর্ণসংখ্যা flags )
{
    পূর্ণসংখ্যা type = matA.type(), esz = CV_ELEM_আকার(type);
    যৌক্তিক haveC = matC.kind() != cv::প্রবেশ_Array_2::NONE;
    আকার_2 আকার_A = matA.size(), আকার_B = matB.size(), আকার_C = haveC ? matC.size() : আকার_2(0, 0);
    যৌক্তিক atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    যদি (atrans)
        আকার_A = আকার_2(আকার_A.height, আকার_A.width);
    যদি (btrans)
        আকার_B = আকার_2(আকার_B.height, আকার_B.width);
    যদি (haveC && ctrans)
        আকার_C = আকার_2(আকার_C.height, আকার_C.width);

    আকার_2 আকার_D(আকার_B.width, আকার_A.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( আকার_A.width == আকার_B.height && (!haveC || আকার_C == আকার_D) );

    matD.create(আকার_D, type);
    যদি ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        ফেরত মিথ্যা;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    যদি (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        ফেরত মিথ্যা;
    }
    যদি (haveC)
    {
        UMat C = matC.getUMat();
        যদি (!ocl::internal::isCLBuffer(C))
            ফেরত মিথ্যা;
    }
    যদি (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    অন্যথা
        D.setTo(Scalar::all(0));

    পূর্ণসংখ্যা M = আকার_D.height, N = আকার_D.width, K = আকার_A.width;
    পূর্ণসংখ্যা lda = (পূর্ণসংখ্যা)A.step / esz, ldb = (পূর্ণসংখ্যা)B.step / esz, ldc = (পূর্ণসংখ্যা)D.step / esz;
    পূর্ণসংখ্যা offa = (পূর্ণসংখ্যা)A.offset / esz, offb = (পূর্ণসংখ্যা)B.offset / esz, offc = (পূর্ণসংখ্যা)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    যদি (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (ধ্রুবক cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ধ্রুবক cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ফাঁকা, ফাঁকা);
    অন্যথা যদি (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (ধ্রুবক cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ধ্রুবক cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ফাঁকা, ফাঁকা);
    অন্যথা যদি (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (ধ্রুবক cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ধ্রুবক cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, ফাঁকা, ফাঁকা);
    }
    অন্যথা যদি (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (ধ্রুবক cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ধ্রুবক cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ফাঁকা, ফাঁকা);
    }
    অন্যথা
        CV_Error(Error::StsUnsupportedFormat, "");

    ফেরত status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
স্থির যৌক্তিক ocl_gemm( প্রবেশ_Array matA, প্রবেশ_Array matB, দ্বিগুণ alpha,
                      প্রবেশ_Array matC, দ্বিগুণ beta, OutputArray matD, পূর্ণসংখ্যা flags )
{
    পূর্ণসংখ্যা type = matA.type();
    পূর্ণসংখ্যা depth = CV_MAT_DEPTH(type);
    পূর্ণসংখ্যা cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ধ্রুবক ocl::Device & dev = ocl::Device::getDefault();
    যৌক্তিক doubleSupport = dev.doubleFPConfig() > 0;

    যদি (!doubleSupport && depth == CV_64F)
        ফেরত মিথ্যা;

    যৌক্তিক haveC = matC.kind() != cv::প্রবেশ_Array_2::NONE;
    আকার_2 আকার_A = matA.size(), আকার_B = matB.size(), আকার_C = haveC ? matC.size() : আকার_2(0, 0);
    যৌক্তিক atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    যদি (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    আকার_2 আকার_D(((btrans) ? আকার_B.height : আকার_B.width),
               ((atrans) ? আকার_A.width : আকার_A.height));

    যদি (atrans)
        আকার_A = আকার_2(আকার_A.height, আকার_A.width);
    যদি (btrans)
        আকার_B = আকার_2(আকার_B.height, আকার_B.width);
    যদি (haveC && ctrans)
        আকার_C = আকার_2(আকার_C.height, আকার_C.width);

    CV_CheckEQ(আকার_A.width, আকার_B.height, "");
    যদি (haveC)
        CV_CheckEQ(আকার_C, আকার_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(আকার_D, type);
    UMat D = matD.getUMat();

    যৌক্তিক isPropagatedC2D = মিথ্যা;  // D content is updated with C / C.t()

    যদি (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        যদি (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = সত্য;
        }
        অন্যথা
        {
            beta = 0.0;
        }

        যৌক্তিক res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, আকার_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        যদি (res)
            ফেরত সত্য;
        // fallback on generic OpenCL code
    }

    যদি (আকার_D.width < 8 || আকার_D.height < 8)
        ফেরত মিথ্যা;

    String opts;

    পূর্ণসংখ্যা wg_আকার = (পূর্ণসংখ্যা)dev.max_Work_Group_আকার();
    পূর্ণসংখ্যা আকার_Dmin = std::min(আকার_D.width, আকার_D.height);
    wg_আকার = std::min(wg_আকার, আকার_Dmin * আকার_Dmin);
    পূর্ণসংখ্যা block_আকার_2 = (wg_আকার / (32*cn) < 32) ? (wg_আকার / (16*cn) < 16) ? (wg_আকার / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    যদি (atrans)
        A = A.t();

    যদি (btrans)
        B = B.t();

    যদি (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    পূর্ণসংখ্যা vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    পূর্ণসংখ্যা kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_আকার_2,
                      (আকার_A.width % block_আকার_2 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    যদি (k.empty())
        ফেরত মিথ্যা;

    যদি (depth == CV_64F)
        k.args(ocl::KernelArg::Read_Only_No_আকার(A),
               ocl::KernelArg::Read_Only_No_আকার(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               আকার_A.width, alpha, beta);
    অন্যথা
        k.args(ocl::KernelArg::Read_Only_No_আকার(A),
               ocl::KernelArg::Read_Only_No_আকার(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               আকার_A.width, (দশমিক)alpha, (দশমিক)beta);

    size_t globalsize[2] = { (size_t)আকার_D.width * cn / kercn, (size_t)আকার_D.height};
    size_t localsize[2] = { (size_t)block_আকার_2, (size_t)block_আকার_2};

    ফেরত k.run(2, globalsize, block_আকার_2 !=1 ? localsize : ফাঁকা, মিথ্যা);
}
#endif


নামাকাশ hal {

শূন্য gemm32f(ধ্রুবক দশমিক* src1, size_t src1_step, ধ্রুবক দশমিক* src2, size_t src2_step,
             দশমিক alpha, ধ্রুবক দশমিক* src3, size_t src3_step, দশমিক beta, দশমিক* dst, size_t dst_step,
             পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#অন্যথা
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

শূন্য gemm64f(ধ্রুবক দ্বিগুণ* src1, size_t src1_step, ধ্রুবক দ্বিগুণ* src2, size_t src2_step,
             দ্বিগুণ alpha, ধ্রুবক দ্বিগুণ* src3, size_t src3_step, দ্বিগুণ beta, দ্বিগুণ* dst, size_t dst_step,
             পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#অন্যথা
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

শূন্য gemm32fc(ধ্রুবক দশমিক* src1, size_t src1_step, ধ্রুবক দশমিক* src2, size_t src2_step,
              দশমিক alpha, ধ্রুবক দশমিক* src3, size_t src3_step, দশমিক beta, দশমিক* dst, size_t dst_step,
              পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#অন্যথা
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

শূন্য gemm64fc(ধ্রুবক দ্বিগুণ* src1, size_t src1_step, ধ্রুবক দ্বিগুণ* src2, size_t src2_step,
              দ্বিগুণ alpha, ধ্রুবক দ্বিগুণ* src3, size_t src3_step, দ্বিগুণ beta, দ্বিগুণ* dst, size_t dst_step,
              পূর্ণসংখ্যা m_a, পূর্ণসংখ্যা n_a, পূর্ণসংখ্যা n_d, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#অন্যথা
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

শূন্য gemm(প্রবেশ_Array matA, প্রবেশ_Array matB, দ্বিগুণ alpha,
          প্রবেশ_Array matC, দ্বিগুণ beta, OutputArray _matD, পূর্ণসংখ্যা flags)
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
    আকার_2 a_আকার = A.size(), d_আকার;
    পূর্ণসংখ্যা len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    নির্বাচন( flags & (GEMM_1_T|GEMM_2_T) )
    {
    ক্ষেত্র 0:
        d_আকার = আকার_2( B.cols, a_আকার.height );
        len = B.rows;
        CV_Assert( a_আকার.width == len );
        বিরতি;
    ক্ষেত্র 1:
        d_আকার = আকার_2( B.cols, a_আকার.width );
        len = B.rows;
        CV_Assert( a_আকার.height == len );
        বিরতি;
    ক্ষেত্র 2:
        d_আকার = আকার_2( B.rows, a_আকার.height );
        len = B.cols;
        CV_Assert( a_আকার.width == len );
        বিরতি;
    ক্ষেত্র 3:
        d_আকার = আকার_2( B.rows, a_আকার.width );
        len = B.cols;
        CV_Assert( a_আকার.height == len );
        বিরতি;
    }

    যদি( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_আকার.height && C.cols == d_আকার.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_আকার.width && C.cols == d_আকার.height)));
    }

    _matD.create( d_আকার.height, d_আকার.width, type );
    Mat D = _matD.getMat();
    যদি( (flags & GEMM_3_T) != 0 && C.তথ্য == D.তথ্য )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    যদি( D.তথ্য == A.তথ্য || D.তথ্য == B.তথ্য )
    {
        DProxy = Mat(d_আকার.height, d_আকার.width, D.type());
        DProxyPtr = &DProxy;
    }

    যদি( type == CV_32FC1 )
        hal::gemm32f(A.ptr<দশমিক>(), A.step, B.ptr<দশমিক>(), B.step, static_cast<দশমিক>(alpha),
                     C.ptr<দশমিক>(), C.step, static_cast<দশমিক>(beta),
                     DProxyPtr->ptr<দশমিক>(), DProxyPtr->step,
                     a_আকার.height, a_আকার.width, DProxyPtr->cols, flags);
    অন্যথা যদি( type == CV_64FC1 )
        hal::gemm64f(A.ptr<দ্বিগুণ>(), A.step, B.ptr<দ্বিগুণ>(), B.step, alpha,
                     C.ptr<দ্বিগুণ>(), C.step, beta,
                     DProxyPtr->ptr<দ্বিগুণ>(), DProxyPtr->step,
                     a_আকার.height, a_আকার.width, DProxyPtr->cols, flags);
    অন্যথা যদি( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<দশমিক>(), A.step, B.ptr<দশমিক>(), B.step, static_cast<দশমিক>(alpha),
                      C.ptr<দশমিক>(), C.step, static_cast<দশমিক>(beta),
                      DProxyPtr->ptr<দশমিক>(), DProxyPtr->step,
                      a_আকার.height, a_আকার.width, DProxyPtr->cols, flags);
    অন্যথা
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<দ্বিগুণ>(), A.step, B.ptr<দ্বিগুণ>(), B.step, alpha,
                      C.ptr<দ্বিগুণ>(), C.step, beta,
                      DProxyPtr->ptr<দ্বিগুণ>(), DProxyPtr->step,
                      a_আকার.height, a_আকার.width, DProxyPtr->cols, flags);
    }

    যদি(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

স্থির TransformFunc getTransformFunc(পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

স্থির TransformFunc getDiagTransformFunc(পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য transform(প্রবেশ_Array _src, OutputArray _dst, প্রবেশ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    পূর্ণসংখ্যা depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    যৌক্তিক isDiag = মিথ্যা;

    _dst.create_Same_আকার( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    যদি (src.তথ্য == dst.তথ্য)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    পূর্ণসংখ্যা mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    Auto_বাফার<দ্বিগুণ> _mbuf;
    দ্বিগুণ* mbuf;

    যদি( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.তথ্য();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.যোগ()*tmp.elem_আকার());
        যদি( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        অন্যথা
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    অন্যথা
        mbuf = m.ptr<দ্বিগুণ>();

    যদি( scn == dcn )
    {
        পূর্ণসংখ্যা i, j;
        দ্বিগুণ eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        যদি( scn == 1 )
        {
            দ্বিগুণ alpha, beta;
            যদি( mtype == CV_32F )
                alpha = m.at<দশমিক>(0), beta = m.at<দশমিক>(1);
            অন্যথা
                alpha = m.at<দ্বিগুণ>(0), beta = m.at<দ্বিগুণ>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            ফেরত;
        }

        জন্য( i = 0, isDiag = সত্য; isDiag && i < scn; i++ )
        {
            জন্য( j = 0; isDiag && j < scn; j++ )
            {
                দ্বিগুণ v = mtype == CV_32F ? m.at<দশমিক>(i, j) : m.at<দ্বিগুণ>(i, j);
                যদি( i != j && fabs(v) > eps )
                    isDiag = মিথ্যা;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    ধ্রুবক Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, যোগ = it.size;

    জন্য( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (পূর্ণসংখ্যা)যোগ, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

স্থির TransformFunc getPerspectiveTransform(পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য perspectiveTransform(প্রবেশ_Array _src, OutputArray _dst, প্রবেশ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    পূর্ণসংখ্যা depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.create_Same_আকার( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ধ্রুবক পূর্ণসংখ্যা mtype = CV_64F;
    Auto_বাফার<দ্বিগুণ> _mbuf;
    দ্বিগুণ* mbuf = m.ptr<দ্বিগুণ>();

    যদি( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.তথ্য();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    ধ্রুবক Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, যোগ = it.size;

    জন্য( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (পূর্ণসংখ্যা)যোগ, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_scaleAdd( প্রবেশ_Array _src1, দ্বিগুণ alpha, প্রবেশ_Array _src2, OutputArray _dst, পূর্ণসংখ্যা type )
{
    ধ্রুবক ocl::Device & d = ocl::Device::getDefault();

    যৌক্তিক doubleSupport = d.doubleFPConfig() > 0;
    আকার_2 size = _src1.size();
    পূর্ণসংখ্যা depth = CV_MAT_DEPTH(type);
    যদি ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        ফেরত মিথ্যা;

    _dst.create(size, type);
    পূর্ণসংখ্যা cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    পূর্ণসংখ্যা kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    অক্ষর cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], আকার(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], আকার(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_আকার(src1),
            src2arg = ocl::KernelArg::Read_Only_No_আকার(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    যদি (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (দশমিক)alpha);
    অন্যথা
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

#endif

স্থির ScaleAddFunc getScaleAddFunc(পূর্ণসংখ্যা depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য scaleAdd(প্রবেশ_Array _src1, দ্বিগুণ alpha, প্রবেশ_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    যদি( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        ফেরত;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    দশমিক falpha = (দশমিক)alpha;
    শূন্য* palpha = depth == CV_32F ? (শূন্য*)&falpha : (শূন্য*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    যদি (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.যোগ()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (পূর্ণসংখ্যা)len, palpha);
        ফেরত;
    }

    ধ্রুবক Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    জন্য( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (পূর্ণসংখ্যা)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

শূন্য calcCovarMatrix( ধ্রুবক Mat* তথ্য, পূর্ণসংখ্যা nsamples, Mat& covar, Mat& _mean, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( তথ্য, nsamples > 0 );
    আকার_2 size = তথ্য[0].size();
    পূর্ণসংখ্যা sz = size.width * size.height, esz = (পূর্ণসংখ্যা)তথ্য[0].elem_আকার();
    পূর্ণসংখ্যা type = তথ্য[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    যদি( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        যদি( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        অন্যথা
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat তথ্য_3(nsamples, sz, type);

    জন্য( পূর্ণসংখ্যা i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( তথ্য[i].size() == size, তথ্য[i].type() == type );
        যদি( তথ্য[i].isContinuous() )
            memcpy( তথ্য_3.ptr(i), তথ্য[i].ptr(), sz*esz );
        অন্যথা
        {
            Mat তথ্য_Row(size.height, size.width, type, তথ্য_3.ptr(i));
            তথ্য[i].copyTo(তথ্য_Row);
        }
    }

    calcCovarMatrix( তথ্য_3, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    যদি( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

শূন্য calcCovarMatrix( প্রবেশ_Array _src, OutputArray _covar, প্রবেশ_Output_Array _mean, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা ctype )
{
    CV_INSTRUMENT_REGION();

    যদি(_src.kind() == প্রবেশ_Array_2::STD_VECTOR_MAT || _src.kind() == প্রবেশ_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        আকার_2 size = src[0].size();
        পূর্ণসংখ্যা type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat তথ্য_3(static_cast<পূর্ণসংখ্যা>(src.size()), size.area(), type);

        পূর্ণসংখ্যা i = 0;
        জন্য(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat তথ্য_Row(size.height, size.width, type, তথ্য_3.ptr(i));
            (*each).copyTo(তথ্য_Row);
        }

        Mat mean;
        যদি( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            যদি( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            অন্যথা
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( তথ্য_3, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        যদি( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        ফেরত;
    }

    Mat তথ্য = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    যৌক্তিক takeRows = (flags & cv::COVAR_ROWS) != 0;
    পূর্ণসংখ্যা type = তথ্য.type();
    পূর্ণসংখ্যা nsamples = takeRows ? তথ্য.rows : তথ্য.cols;
    CV_Assert( nsamples > 0 );
    আকার_2 size = takeRows ? আকার_2(তথ্য.cols, 1) : আকার_2(1, তথ্য.rows);

    যদি( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        যদি( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    অন্যথা
    {
        ctype = std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, _mean, takeRows ? 0 : 1, REDUCE_AVG, ctype );
        mean = _mean.getMat();
    }

    mulTransposed( তথ্য, _covar, ((flags & cv::COVAR_NORMAL) == 0) ^ takeRows,
        mean, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

স্থির MahalanobisImplFunc getMahalanobisImplFunc(পূর্ণসংখ্যা depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#অন্যথা
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


দ্বিগুণ Mahalanobis(প্রবেশ_Array _v1, প্রবেশ_Array _v2, প্রবেশ_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    পূর্ণসংখ্যা type = v1.type(), depth = v1.depth();
    আকার_2 sz = v1.size();
    পূর্ণসংখ্যা len = sz.width*sz.height*v1.channels();
    Auto_বাফার<দ্বিগুণ> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    যদি( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    দ্বিগুণ ফলাফল = func(v1, v2, icovar, buf.তথ্য(), len);
    ফেরত std::sqrt(ফলাফল);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

স্থির MulTransposedFunc getMulTransposedFunc(পূর্ণসংখ্যা stype, পূর্ণসংখ্যা dtype, যৌক্তিক ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#অন্যথা
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

শূন্য mulTransposed(প্রবেশ_Array _src, OutputArray _dst, যৌক্তিক ata,
                   প্রবেশ_Array _delta, দ্বিগুণ scale, পূর্ণসংখ্যা dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ধ্রুবক পূর্ণসংখ্যা gemm_level = 100; // boundary above which GEMM is faster.
    পূর্ণসংখ্যা stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    যদি( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        যদি( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    পূর্ণসংখ্যা dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    যদি( src.তথ্য == dst.তথ্য || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        ধ্রুবক Mat* tsrc = &src;
        যদি( !delta.empty() )
        {
            যদি( delta.size() == src.size() )
                subtract( src, delta, src2 );
            অন্যথা
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    অন্যথা
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        যদি( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, মিথ্যা );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

স্থির দ্বিগুণ dotProd_8u(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ধ্রুবক uchar*)src1, (ধ্রুবক uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্বিগুণ dotProd_8s(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ধ্রুবক schar*)src1, (ধ্রুবক schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্বিগুণ dotProd_16u(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ধ্রুবক ushort*)src1, (ধ্রুবক ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্বিগুণ dotProd_16s(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ধ্রুবক short*)src1, (ধ্রুবক short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্বিগুণ dotProd_32s(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ধ্রুবক পূর্ণসংখ্যা*)src1, (ধ্রুবক পূর্ণসংখ্যা*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্বিগুণ dotProd_32f(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ধ্রুবক দশমিক*)src1, (ধ্রুবক দশমিক*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্বিগুণ dotProd_64f(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, পূর্ণসংখ্যা len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ধ্রুবক দ্বিগুণ*)src1, (ধ্রুবক দ্বিগুণ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

প্রকারলেখ দ্বিগুণ (*DotProdFunc)(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, পূর্ণসংখ্যা len);

স্থির DotProdFunc getDotProdFunc(পূর্ণসংখ্যা depth)
{
    স্থির DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    ফেরত dotProdTab[depth];
}

দ্বিগুণ Mat::dot(প্রবেশ_Array _mat) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    পূর্ণসংখ্যা cn = channels();
    যদি (এই->dims <= 2)
    {
        দ্বিগুণ product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, এই->তথ্য, এই->step, mat.তথ্য, mat.step,
                     এই->cols * cn, এই->rows, এই->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    যদি( isContinuous() && mat.isContinuous() )
    {
        size_t len = যোগ()*cn;
        যদি( len == (size_t)(পূর্ণসংখ্যা)len )
            ফেরত func(তথ্য, mat.তথ্য, (পূর্ণসংখ্যা)len);
    }

    ধ্রুবক Mat* arrays[] = {এই, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা len = (পূর্ণসংখ্যা)(it.size*cn);
    দ্বিগুণ r = 0;

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    ফেরত r;
}


#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_dot( প্রবেশ_Array _src1, প্রবেশ_Array _src2, দ্বিগুণ & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    পূর্ণসংখ্যা type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    যৌক্তিক doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    যদি ( !doubleSupport && depth == CV_64F )
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_Group_আকার();
    পূর্ণসংখ্যা ddepth = std::max(CV_32F, depth);

    পূর্ণসংখ্যা wgs2_aligned = 1;
    যখন (wgs2_aligned < (পূর্ণসংখ্যা)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    অক্ষর cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, আকার(cvt)),
                         (পূর্ণসংখ্যা)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    যদি (k.empty())
        ফেরত মিথ্যা;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_আকার(src1),
            src2arg = ocl::KernelArg::Read_Only_No_আকার(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (পূর্ণসংখ্যা)src1.যোগ(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    যদি (k.run(1, &globalsize, &wgs, সত্য))
    {
        res = যোগ_2(db.getMat(ACCESS_READ))[0];
        ফেরত সত্য;
    }
    ফেরত মিথ্যা;
}

#endif

দ্বিগুণ UMat::dot(প্রবেশ_Array m) ধ্রুবক
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_আকার(*এই) && m.type() == type());

#ifdef HAVE_OPENCL
    দ্বিগুণ r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*এই, m, r), r)
#endif

    ফেরত getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
