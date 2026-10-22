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

#ضمن "precomp.hpp"
#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencl_kernels_core.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_core.hpp"
#ضمن "intel_gpu_gemm.inl.hpp"

#ضمن "matmul.simd.hpp"
#ضمن "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#جيڪڏهن_وصف_ٿيل HAVE_CLAMDBLAS

ساكن منطقي ocl_gemm_amdblas( داخلا_Array matA, داخلا_Array matB, مزدوج alpha,
                      داخلا_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type(), esz = CV_ELEM_سائيز(type);
    منطقي haveC = matC.قسم() != cv::داخلا_Array_2::NONE;
    سائيز سائيز_A = matA.size(), سائيز_B = matB.size(), سائيز_C = haveC ? matC.size() : سائيز(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (atrans)
        سائيز_A = سائيز(سائيز_A.اوچائي, سائيز_A.ويڪر);
    إذا (btrans)
        سائيز_B = سائيز(سائيز_B.اوچائي, سائيز_B.ويڪر);
    إذا (haveC && ctrans)
        سائيز_C = سائيز(سائيز_C.اوچائي, سائيز_C.ويڪر);

    سائيز سائيز_D(سائيز_B.ويڪر, سائيز_A.اوچائي);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( سائيز_A.ويڪر == سائيز_B.اوچائي && (!haveC || سائيز_C == سائيز_D) );

    matD.create(سائيز_D, type);
    إذا ( matA.offset() % esz != 0 || matA.قدم() % esz != 0 ||
         matB.offset() % esz != 0 || matB.قدم() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.قدم() % esz != 0)) )
        أرجع خطأ;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    إذا (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        أرجع خطأ;
    }
    إذا (haveC)
    {
        UMat C = matC.getUMat();
        إذا (!ocl::internal::isCLBuffer(C))
            أرجع خطأ;
    }
    إذا (haveC)
        ctrans ? transpose(matC, D) : matC.copy_منزل(D);
    وإلا
        D.سيٽ_منزل(Scalar::all(0));

    صحيح M = سائيز_D.اوچائي, N = سائيز_D.ويڪر, K = سائيز_A.ويڪر;
    صحيح lda = (صحيح)A.قدم / esz, ldb = (صحيح)B.قدم / esz, ldc = (صحيح)D.قدم / esz;
    صحيح offa = (صحيح)A.offset / esz, offb = (صحيح)B.offset / esz, offc = (صحيح)D.offset / esz;

    cl_command_قطار clq = (cl_command_قطار)ocl::قطار::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_ترتيب ترتيب = clblas_صف_Major;
    clblas_حالت حالت = clblasSuccess;

    إذا (type == CV_32FC1)
        حالت = clblasSgemm(ترتيب, transA, transB, M, N, K,
                             (cl_float)alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    وإلا إذا (type == CV_64FC1)
        حالت = clblasDgemm(ترتيب, transA, transB, M, N, K,
                             alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    وإلا إذا (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         حالت = clblasCgemm(ترتيب, transA, transB, M, N, K,
                              alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, عدم, عدم);
    }
    وإلا إذا (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        حالت = clblasZgemm(ترتيب, transA, transB, M, N, K,
                             alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    }
    وإلا
        CV_Error(Error::StsUnsupportedFormat, "");

    أرجع حالت == clblasSuccess;
}

#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
ساكن منطقي ocl_gemm( داخلا_Array matA, داخلا_Array matB, مزدوج alpha,
                      داخلا_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type();
    صحيح اونهائي = CV_MAT_اونهائي(type);
    صحيح cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0;

    إذا (!doubleSupport && اونهائي == CV_64F)
        أرجع خطأ;

    منطقي haveC = matC.قسم() != cv::داخلا_Array_2::NONE;
    سائيز سائيز_A = matA.size(), سائيز_B = matB.size(), سائيز_C = haveC ? matC.size() : سائيز(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    سائيز سائيز_D(((btrans) ? سائيز_B.اوچائي : سائيز_B.ويڪر),
               ((atrans) ? سائيز_A.ويڪر : سائيز_A.اوچائي));

    إذا (atrans)
        سائيز_A = سائيز(سائيز_A.اوچائي, سائيز_A.ويڪر);
    إذا (btrans)
        سائيز_B = سائيز(سائيز_B.اوچائي, سائيز_B.ويڪر);
    إذا (haveC && ctrans)
        سائيز_C = سائيز(سائيز_C.اوچائي, سائيز_C.ويڪر);

    CV_CheckEQ(سائيز_A.ويڪر, سائيز_B.اوچائي, "");
    إذا (haveC)
        CV_CheckEQ(سائيز_C, سائيز_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(سائيز_D, type);
    UMat D = matD.getUMat();

    منطقي isPropagatedC2D = خطأ;  // D content is updated with C / C.t()

    إذا (dev.intelSubgroupsSupport() && (اونهائي == CV_32F) && cn == 1)
    {
        إذا (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copy_منزل(D);
            isPropagatedC2D = سچ;
        }
        وإلا
        {
            beta = 0.0;
        }

        منطقي res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, سائيز_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        إذا (res)
            أرجع سچ;
        // fallback on generic OpenCL code
    }

    إذا (سائيز_D.ويڪر < 8 || سائيز_D.اوچائي < 8)
        أرجع خطأ;

    String opts;

    صحيح wg_سائيز = (صحيح)dev.max_Work_گروهه_سائيز();
    صحيح سائيز_Dmin = std::min(سائيز_D.ويڪر, سائيز_D.اوچائي);
    wg_سائيز = std::min(wg_سائيز, سائيز_Dmin * سائيز_Dmin);
    صحيح بلاڪ_سائيز_3 = (wg_سائيز / (32*cn) < 32) ? (wg_سائيز / (16*cn) < 16) ? (wg_سائيز / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    إذا (atrans)
        A = A.t();

    إذا (btrans)
        B = B.t();

    إذا (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_منزل(D);

    صحيح ويڪٽر_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    صحيح kercn = ocl::check_Optimal_ويڪٽر_ويڪر(ويڪٽر_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_منزل_Str(type), ocl::type_منزل_Str(اونهائي), ocl::type_منزل_Str(CV_MAKETYPE(اونهائي, kercn)),
                      cn, kercn, بلاڪ_سائيز_3,
                      (سائيز_A.ويڪر % بلاڪ_سائيز_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    إذا (اونهائي == CV_64F)
        k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(A),
               ocl::KernelArg::Read_Only_No_سائيز(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               سائيز_A.ويڪر, alpha, beta);
    وإلا
        k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(A),
               ocl::KernelArg::Read_Only_No_سائيز(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               سائيز_A.ويڪر, (عائم)alpha, (عائم)beta);

    size_t globalsize[2] = { (size_t)سائيز_D.ويڪر * cn / kercn, (size_t)سائيز_D.اوچائي};
    size_t localsize[2] = { (size_t)بلاڪ_سائيز_3, (size_t)بلاڪ_سائيز_3};

    أرجع k.لاڳيتو(2, globalsize, بلاڪ_سائيز_3 !=1 ? localsize : عدم, خطأ);
}
#جيڪڏهن_پورو


نطاق hal {

فراغ gemm32f(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
             عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#جيڪڏهن_وصف_ٿيل CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#جيڪڏهن_پورو
}

فراغ gemm64f(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#جيڪڏهن_وصف_ٿيل CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#جيڪڏهن_پورو
}

فراغ gemm32fc(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
              عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#جيڪڏهن_وصف_ٿيل CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#جيڪڏهن_پورو
}

فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#جيڪڏهن_وصف_ٿيل CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#جيڪڏهن_پورو
}

} // namespace hal

فراغ gemm(داخلا_Array matA, داخلا_Array matB, مزدوج alpha,
          داخلا_Array matC, مزدوج beta, OutputArray _matD, صحيح flags)
{
#جيڪڏهن_وصف_ٿيل HAVE_CLAMDBLAS
    CV_OCL_لاڳيتو(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.کانن_ڳڻپ() > 20 && matA.صفون() > 20 && matB.کانن_ڳڻپ() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    CV_OCL_لاڳيتو(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#جيڪڏهن_پورو

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    سائيز a_سائيز = A.size(), d_سائيز;
    صحيح len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    بدل( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالة 0:
        d_سائيز = سائيز( B.کانن_ڳڻپ, a_سائيز.اوچائي );
        len = B.صفون;
        CV_Assert( a_سائيز.ويڪر == len );
        اكسر;
    حالة 1:
        d_سائيز = سائيز( B.کانن_ڳڻپ, a_سائيز.ويڪر );
        len = B.صفون;
        CV_Assert( a_سائيز.اوچائي == len );
        اكسر;
    حالة 2:
        d_سائيز = سائيز( B.صفون, a_سائيز.اوچائي );
        len = B.کانن_ڳڻپ;
        CV_Assert( a_سائيز.ويڪر == len );
        اكسر;
    حالة 3:
        d_سائيز = سائيز( B.صفون, a_سائيز.ويڪر );
        len = B.کانن_ڳڻپ;
        CV_Assert( a_سائيز.اوچائي == len );
        اكسر;
    }

    إذا( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.صفون == d_سائيز.اوچائي && C.کانن_ڳڻپ == d_سائيز.ويڪر) ||
             ((flags&GEMM_3_T) != 0 && C.صفون == d_سائيز.ويڪر && C.کانن_ڳڻپ == d_سائيز.اوچائي)));
    }

    _matD.create( d_سائيز.اوچائي, d_سائيز.ويڪر, type );
    Mat D = _matD.getMat();
    إذا( (flags & GEMM_3_T) != 0 && C.معلومات == D.معلومات )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    إذا( D.معلومات == A.معلومات || D.معلومات == B.معلومات )
    {
        DProxy = Mat(d_سائيز.اوچائي, d_سائيز.ويڪر, D.type());
        DProxyPtr = &DProxy;
    }

    إذا( type == CV_32FC1 )
        hal::gemm32f(A.ptr<عائم>(), A.قدم, B.ptr<عائم>(), B.قدم, static_cast<عائم>(alpha),
                     C.ptr<عائم>(), C.قدم, static_cast<عائم>(beta),
                     DProxyPtr->ptr<عائم>(), DProxyPtr->قدم,
                     a_سائيز.اوچائي, a_سائيز.ويڪر, DProxyPtr->کانن_ڳڻپ, flags);
    وإلا إذا( type == CV_64FC1 )
        hal::gemm64f(A.ptr<مزدوج>(), A.قدم, B.ptr<مزدوج>(), B.قدم, alpha,
                     C.ptr<مزدوج>(), C.قدم, beta,
                     DProxyPtr->ptr<مزدوج>(), DProxyPtr->قدم,
                     a_سائيز.اوچائي, a_سائيز.ويڪر, DProxyPtr->کانن_ڳڻپ, flags);
    وإلا إذا( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<عائم>(), A.قدم, B.ptr<عائم>(), B.قدم, static_cast<عائم>(alpha),
                      C.ptr<عائم>(), C.قدم, static_cast<عائم>(beta),
                      DProxyPtr->ptr<عائم>(), DProxyPtr->قدم,
                      a_سائيز.اوچائي, a_سائيز.ويڪر, DProxyPtr->کانن_ڳڻپ, flags);
    وإلا
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<مزدوج>(), A.قدم, B.ptr<مزدوج>(), B.قدم, alpha,
                      C.ptr<مزدوج>(), C.قدم, beta,
                      DProxyPtr->ptr<مزدوج>(), DProxyPtr->قدم,
                      a_سائيز.اوچائي, a_سائيز.ويڪر, DProxyPtr->کانن_ڳڻپ, flags);
    }

    إذا(DProxyPtr != &D)
        DProxyPtr->copy_منزل(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ساكن TransformFunc getTransformFunc(صحيح اونهائي)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (اونهائي),
        CV_CPU_DISPATCH_MODES_ALL);
}

ساكن TransformFunc getDiagTransformFunc(صحيح اونهائي)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (اونهائي),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ transform(داخلا_Array _src, OutputArray _dst, داخلا_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح اونهائي = src.اونهائي(), scn = src.channels(), dcn = m.صفون;
    CV_Assert( scn == m.کانن_ڳڻپ || scn + 1 == m.کانن_ڳڻپ );
    منطقي isDiag = خطأ;

    _dst.create_Same_سائيز( src, CV_MAKETYPE(اونهائي, dcn) );
    Mat dst = _dst.getMat();

    إذا (src.معلومات == dst.معلومات)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    صحيح mtype = اونهائي == CV_32S || اونهائي == CV_64F ? CV_64F : CV_32F;
    Auto_بفر<مزدوج> _mbuf;
    مزدوج* mbuf;

    إذا( !m.isContinuous() || m.type() != mtype || m.کانن_ڳڻپ != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.معلومات();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.ڪل()*tmp.elem_سائيز());
        إذا( m.کانن_ڳڻپ == scn+1 )
            m.convert_منزل(tmp, mtype);
        وإلا
        {
            Mat tmppart = tmp.کانو_Range_2(0, m.کانن_ڳڻپ);
            m.convert_منزل(tmppart, mtype);
        }
        m = tmp;
    }
    وإلا
        mbuf = m.ptr<مزدوج>();

    إذا( scn == dcn )
    {
        صحيح i, j;
        مزدوج eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        إذا( scn == 1 )
        {
            مزدوج alpha, beta;
            إذا( mtype == CV_32F )
                alpha = m.at<عائم>(0), beta = m.at<عائم>(1);
            وإلا
                alpha = m.at<مزدوج>(0), beta = m.at<مزدوج>(1);
            src.convert_منزل(dst, dst.type(), alpha, beta);
            أرجع;
        }

        لكل( i = 0, isDiag = سچ; isDiag && i < scn; i++ )
        {
            لكل( j = 0; isDiag && j < scn; j++ )
            {
                مزدوج v = mtype == CV_32F ? m.at<عائم>(i, j) : m.at<مزدوج>(i, j);
                إذا( i != j && fabs(v) > eps )
                    isDiag = خطأ;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(اونهائي): getTransformFunc(اونهائي);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, ڪل = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)ڪل, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ساكن TransformFunc getPerspectiveTransform(صحيح اونهائي)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (اونهائي),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ perspectiveTransform(داخلا_Array _src, OutputArray _dst, داخلا_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح اونهائي = src.اونهائي(), scn = src.channels(), dcn = m.صفون-1;
    CV_Assert( scn + 1 == m.کانن_ڳڻپ );
    CV_Assert( اونهائي == CV_32F || اونهائي == CV_64F );

    _dst.create_Same_سائيز( src, CV_MAKETYPE(اونهائي, dcn) );
    Mat dst = _dst.getMat();

    ثابت صحيح mtype = CV_64F;
    Auto_بفر<مزدوج> _mbuf;
    مزدوج* mbuf = m.ptr<مزدوج>();

    إذا( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.معلومات();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convert_منزل(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(اونهائي);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, ڪل = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)ڪل, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

ساكن منطقي ocl_scale_جوڙيو( داخلا_Array _src1, مزدوج alpha, داخلا_Array _src2, OutputArray _dst, صحيح type )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    منطقي doubleSupport = d.doubleFPConfig() > 0;
    سائيز size = _src1.size();
    صحيح اونهائي = CV_MAT_اونهائي(type);
    إذا ( (!doubleSupport && اونهائي == CV_64F) || size != _src2.size() )
        أرجع خطأ;

    _dst.create(size, type);
    صحيح cn = CV_MAT_CN(type), wdepth = std::max(اونهائي, CV_32F);
    صحيح kercn = ocl::predict_Optimal_ويڪٽر_ويڪر_Max(_src1, _src2, _dst),
        صفون_في_WI = d.isIntel() ? 4 : 1;

    حرف cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(اونهائي, kercn)), اونهائي,
                         ocl::type_منزل_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(اونهائي, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, اونهائي, kercn, cvt[1], حجم(cvt[1])),
                         ocl::type_منزل_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", صفون_في_WI));
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_سائيز(src1),
            src2arg = ocl::KernelArg::Read_Only_No_سائيز(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    إذا (wdepth == CV_32F)
        k.دليلون(src1arg, src2arg, dstarg, (عائم)alpha);
    وإلا
        k.دليلون(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.کانن_ڳڻپ * cn / kercn, ((size_t)dst.صفون + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

#جيڪڏهن_پورو

ساكن Scale_جوڙيو_Func get_Scale_جوڙيو_Func(صحيح اونهائي)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_جوڙيو_Func, (اونهائي),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ scale_جوڙيو(داخلا_Array _src1, مزدوج alpha, داخلا_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src1.type(), اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_لاڳيتو(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_جوڙيو(_src1, alpha, _src2, _dst, type))

    إذا( اونهائي != CV_32F && اونهائي != CV_64F )
    {
        جوڙيو_Weighted(_src1, alpha, _src2, 1, 0, _dst, اونهائي);
        أرجع;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    عائم falpha = (عائم)alpha;
    فراغ* palpha = اونهائي == CV_32F ? (فراغ*)&falpha : (فراغ*)&alpha;

    Scale_جوڙيو_Func func = get_Scale_جوڙيو_Func(اونهائي);
    CV_Assert(func);

    إذا (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.ڪل()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (صحيح)len, palpha);
        أرجع;
    }

    ثابت Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (صحيح)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

فراغ calc_Covar_ميٽرڪس( ثابت Mat* معلومات, صحيح nsamples, Mat& covar, Mat& سراسري, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( معلومات, nsamples > 0 );
    سائيز size = معلومات[0].size();
    صحيح sz = size.ويڪر * size.اوچائي, esz = (صحيح)معلومات[0].elem_سائيز();
    صحيح type = معلومات[0].type();
    Mat سراسري_2;
    ctype = std::max(std::max(CV_MAT_اونهائي(ctype >= 0 ? ctype : type), سراسري.اونهائي()), CV_32F);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( سراسري.size() == size );
        إذا( سراسري.isContinuous() && سراسري.type() == ctype )
            سراسري_2 = سراسري.reshape(1, 1);
        وإلا
        {
            سراسري.convert_منزل(سراسري_2, ctype);
            سراسري_2 = سراسري_2.reshape(1, 1);
        }
    }

    Mat معلومات_3(nsamples, sz, type);

    لكل( صحيح i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( معلومات[i].size() == size, معلومات[i].type() == type );
        إذا( معلومات[i].isContinuous() )
            يادگيري_نقل( معلومات_3.ptr(i), معلومات[i].ptr(), sz*esz );
        وإلا
        {
            Mat معلومات_صف(size.اوچائي, size.ويڪر, type, معلومات_3.ptr(i));
            معلومات[i].copy_منزل(معلومات_صف);
        }
    }

    calc_Covar_ميٽرڪس( معلومات_3, covar, سراسري_2, (flags & ~(cv::COVAR_صفون|cv::COVAR_کانن_ڳڻپ)) | cv::COVAR_صفون, ctype );
    إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        سراسري = سراسري_2.reshape(1, size.اوچائي);
}

فراغ calc_Covar_ميٽرڪس( داخلا_Array _src, OutputArray _covar, داخلا_Output_Array سراسري, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    إذا(_src.قسم() == داخلا_Array_2::STD_ويڪٽر_MAT || _src.قسم() == داخلا_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_ويڪٽر(src);

        CV_Assert( src.size() > 0 );

        سائيز size = src[0].size();
        صحيح type = src[0].type();

        ctype = std::max(std::max(CV_MAT_اونهائي(ctype >= 0 ? ctype : type), سراسري.اونهائي()), CV_32F);

        Mat معلومات_3(static_cast<صحيح>(src.size()), size.area(), type);

        صحيح i = 0;
        لكل(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat معلومات_صف(size.اوچائي, size.ويڪر, type, معلومات_3.ptr(i));
            (*each).copy_منزل(معلومات_صف);
        }

        Mat سراسري_2;
        إذا( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( سراسري.size() == size );
            Mat mean0 = سراسري.getMat();

            إذا( mean0.isContinuous() && mean0.type() == ctype )
                سراسري_2 = mean0.reshape(1, 1);
            وإلا
            {
                mean0.convert_منزل(سراسري_2, ctype);
                سراسري_2 = سراسري_2.reshape(1, 1);
            }
        }

        calc_Covar_ميٽرڪس( معلومات_3, _covar, سراسري_2, (flags & ~(cv::COVAR_صفون|cv::COVAR_کانن_ڳڻپ)) | cv::COVAR_صفون, ctype );

        إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            سراسري_2 = سراسري_2.reshape(1, size.اوچائي);
            سراسري_2.copy_منزل(سراسري);
        }
        أرجع;
    }

    Mat معلومات = _src.getMat(), سراسري_2;
    CV_Assert( ((flags & cv::COVAR_صفون) != 0) ^ ((flags & cv::COVAR_کانن_ڳڻپ) != 0) );
    منطقي take_صفون = (flags & cv::COVAR_صفون) != 0;
    صحيح type = معلومات.type();
    صحيح nsamples = take_صفون ? معلومات.صفون : معلومات.کانن_ڳڻپ;
    CV_Assert( nsamples > 0 );
    سائيز size = take_صفون ? سائيز(معلومات.کانن_ڳڻپ, 1) : سائيز(1, معلومات.صفون);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        سراسري_2 = سراسري.getMat();
        ctype = std::max(std::max(CV_MAT_اونهائي(ctype >= 0 ? ctype : type), سراسري_2.اونهائي()), CV_32F);
        CV_Assert( سراسري_2.size() == size );
        إذا( سراسري_2.type() != ctype )
        {
            سراسري.create(سراسري_2.size(), ctype);
            Mat tmp = سراسري.getMat();
            سراسري_2.convert_منزل(tmp, ctype);
            سراسري_2 = tmp;
        }
    }
    وإلا
    {
        ctype = std::max(CV_MAT_اونهائي(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, سراسري, take_صفون ? 0 : 1, REDUCE_AVG, ctype );
        سراسري_2 = سراسري.getMat();
    }

    mulTransposed( معلومات, _covar, ((flags & cv::COVAR_عام) == 0) ^ take_صفون,
        سراسري_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ساكن MahalanobisImplFunc getMahalanobisImplFunc(صحيح اونهائي)
{
#جيڪڏهن_وصف_ٿيل CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (اونهائي));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (اونهائي),
        CV_CPU_DISPATCH_MODES_ALL);
#جيڪڏهن_پورو
}


مزدوج Mahalanobis(داخلا_Array _v1, داخلا_Array _v2, داخلا_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    صحيح type = v1.type(), اونهائي = v1.اونهائي();
    سائيز sz = v1.size();
    صحيح len = sz.ويڪر*sz.اوچائي*v1.channels();
    Auto_بفر<مزدوج> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.صفون && len == icovar.کانن_ڳڻپ );

    sz.ويڪر *= v1.channels();
    إذا( v1.isContinuous() && v2.isContinuous() )
    {
        sz.ويڪر *= sz.اوچائي;
        sz.اوچائي = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(اونهائي);
    CV_Assert(func);

    مزدوج نتيجو = func(v1, v2, icovar, buf.معلومات(), len);
    أرجع std::جذر(نتيجو);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ساكن MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata)
{
#جيڪڏهن_وصف_ٿيل CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#جيڪڏهن_پورو
}

فراغ mulTransposed(داخلا_Array _src, OutputArray _dst, منطقي ata,
                   داخلا_Array _delta, مزدوج scale, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ثابت صحيح gemm_level = 100; // boundary above which GEMM is faster.
    صحيح stype = src.type();
    dtype = std::max(std::max(CV_MAT_اونهائي(dtype >= 0 ? dtype : stype), delta.اونهائي()), CV_32F);
    CV_Assert( src.channels() == 1 );

    إذا( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.صفون == src.صفون || delta.صفون == 1),
            (delta.کانن_ڳڻپ == src.کانن_ڳڻپ || delta.کانن_ڳڻپ == 1));
        إذا( delta.type() != dtype )
            delta.convert_منزل(delta, dtype);
    }

    صحيح dsize = ata ? src.کانن_ڳڻپ : src.صفون;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    إذا( src.معلومات == dst.معلومات || (stype == dtype &&
        (dst.کانن_ڳڻپ >= gemm_level && dst.صفون >= gemm_level &&
         src.کانن_ڳڻپ >= gemm_level && src.صفون >= gemm_level)))
    {
        Mat src2;
        ثابت Mat* tsrc = &src;
        إذا( !delta.empty() )
        {
            إذا( delta.size() == src.size() )
                ڪٽيو( src, delta, src2 );
            وإلا
            {
                repeat(delta, src.صفون/delta.صفون, src.کانن_ڳڻپ/delta.کانن_ڳڻپ, src2);
                ڪٽيو( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    وإلا
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        إذا( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, خطأ );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ساكن مزدوج dotProd_8u(ثابت فراغ* src1, ثابت فراغ* src2, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ثابت uchar*)src1, (ثابت uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساكن مزدوج dotProd_8s(ثابت فراغ* src1, ثابت فراغ* src2, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ثابت schar*)src1, (ثابت schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساكن مزدوج dotProd_16u(ثابت فراغ* src1, ثابت فراغ* src2, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ثابت ushort*)src1, (ثابت ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساكن مزدوج dotProd_16s(ثابت فراغ* src1, ثابت فراغ* src2, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ثابت ننڍو*)src1, (ثابت ننڍو*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساكن مزدوج dotProd_32s(ثابت فراغ* src1, ثابت فراغ* src2, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ثابت صحيح*)src1, (ثابت صحيح*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساكن مزدوج dotProd_32f(ثابت فراغ* src1, ثابت فراغ* src2, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ثابت عائم*)src1, (ثابت عائم*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساكن مزدوج dotProd_64f(ثابت فراغ* src1, ثابت فراغ* src2, صحيح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ثابت مزدوج*)src1, (ثابت مزدوج*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

تعريفنوع مزدوج (*DotProdFunc)(ثابت فراغ* src1, ثابت فراغ* src2, صحيح len);

ساكن DotProdFunc getDotProdFunc(صحيح اونهائي)
{
    ساكن DotProdFunc dotProdTab[CV_اونهائي_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    أرجع dotProdTab[اونهائي];
}

مزدوج Mat::dot(داخلا_Array _mat) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    صحيح cn = channels();
    إذا (هذا->dims <= 2)
    {
        مزدوج حاصل_ضرب = 0;
        CALL_HAL_RET(dot_حاصل_ضرب, cv_hal_dot_حاصل_ضرب, حاصل_ضرب, هذا->معلومات, هذا->قدم, mat.معلومات, mat.قدم,
                     هذا->کانن_ڳڻپ * cn, هذا->صفون, هذا->اونهائي());
    }

    DotProdFunc func = getDotProdFunc(اونهائي());
    CV_Assert(func != 0 );

    إذا( isContinuous() && mat.isContinuous() )
    {
        size_t len = ڪل()*cn;
        إذا( len == (size_t)(صحيح)len )
            أرجع func(معلومات, mat.معلومات, (صحيح)len);
    }

    ثابت Mat* arrays[] = {هذا, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح len = (صحيح)(it.size*cn);
    مزدوج r = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    أرجع r;
}


#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

ساكن منطقي ocl_dot( داخلا_Array _src1, داخلا_Array _src2, مزدوج & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    صحيح type = src1.type(), اونهائي = CV_MAT_اونهائي(type),
            kercn = ocl::predict_Optimal_ويڪٽر_ويڪر(src1, src2);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا ( !doubleSupport && اونهائي == CV_64F )
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروهه_سائيز();
    صحيح ddepth = std::max(CV_32F, اونهائي);

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    حرف cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(اونهائي, kercn)), ocl::type_منزل_Str(اونهائي),
                         ocl::type_منزل_Str(ddepth), ocl::type_منزل_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(اونهائي, ddepth, kercn, cvt, حجم(cvt)),
                         (صحيح)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_سائيز(src1),
            src2arg = ocl::KernelArg::Read_Only_No_سائيز(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.دليلون(src1arg, src1.کانن_ڳڻپ, (صحيح)src1.ڪل(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    إذا (k.لاڳيتو(1, &globalsize, &wgs, سچ))
    {
        res = جوڙ(db.getMat(ACCESS_READ))[0];
        أرجع سچ;
    }
    أرجع خطأ;
}

#جيڪڏهن_پورو

مزدوج UMat::dot(داخلا_Array m) ثابت
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_سائيز(*هذا) && m.type() == type());

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    مزدوج r = 0;
    CV_OCL_لاڳيتو_2(dims <= 2, ocl_dot(*هذا, m, r), r)
#جيڪڏهن_پورو

    أرجع getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
