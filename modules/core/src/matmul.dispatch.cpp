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

#که_تعریف_شوی HAVE_CLAMDBLAS

ساكن منطقي ocl_gemm_amdblas( ننوت_Array matA, ننوت_Array matB, مزدوج alpha,
                      ننوت_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type(), esz = CV_ELEM_اندازه(type);
    منطقي haveC = matC.ډول() != cv::ننوت_Array_2::NONE;
    اندازه اندازه_A = matA.size(), اندازه_B = matB.size(), اندازه_C = haveC ? matC.size() : اندازه(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (atrans)
        اندازه_A = اندازه(اندازه_A.لوړوالی, اندازه_A.پلنوالی);
    إذا (btrans)
        اندازه_B = اندازه(اندازه_B.لوړوالی, اندازه_B.پلنوالی);
    إذا (haveC && ctrans)
        اندازه_C = اندازه(اندازه_C.لوړوالی, اندازه_C.پلنوالی);

    اندازه اندازه_D(اندازه_B.پلنوالی, اندازه_A.لوړوالی);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( اندازه_A.پلنوالی == اندازه_B.لوړوالی && (!haveC || اندازه_C == اندازه_D) );

    matD.create(اندازه_D, type);
    إذا ( matA.offset() % esz != 0 || matA.ګام() % esz != 0 ||
         matB.offset() % esz != 0 || matB.ګام() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.ګام() % esz != 0)) )
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
        D.سېټ_منزل(Scalar::all(0));

    صحيح M = اندازه_D.لوړوالی, N = اندازه_D.پلنوالی, K = اندازه_A.پلنوالی;
    صحيح lda = (صحيح)A.ګام / esz, ldb = (صحيح)B.ګام / esz, ldc = (صحيح)D.ګام / esz;
    صحيح offa = (صحيح)A.offset / esz, offb = (صحيح)B.offset / esz, offc = (صحيح)D.offset / esz;

    cl_command_کتار clq = (cl_command_کتار)ocl::کتار::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_ترتیب ترتیب = clblas_لیکه_Major;
    clblas_حالت حالت = clblasSuccess;

    إذا (type == CV_32FC1)
        حالت = clblasSgemm(ترتیب, transA, transB, M, N, K,
                             (cl_float)alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    وإلا إذا (type == CV_64FC1)
        حالت = clblasDgemm(ترتیب, transA, transB, M, N, K,
                             alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    وإلا إذا (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         حالت = clblasCgemm(ترتیب, transA, transB, M, N, K,
                              alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, عدم, عدم);
    }
    وإلا إذا (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        حالت = clblasZgemm(ترتیب, transA, transB, M, N, K,
                             alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    }
    وإلا
        CV_Error(Error::StsUnsupportedFormat, "");

    أرجع حالت == clblasSuccess;
}

#د_که_پای

#که_تعریف_شوی HAVE_OPENCL
ساكن منطقي ocl_gemm( ننوت_Array matA, ننوت_Array matB, مزدوج alpha,
                      ننوت_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type();
    صحيح ژوروالی = CV_MAT_ژوروالی(type);
    صحيح cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0;

    إذا (!doubleSupport && ژوروالی == CV_64F)
        أرجع خطأ;

    منطقي haveC = matC.ډول() != cv::ننوت_Array_2::NONE;
    اندازه اندازه_A = matA.size(), اندازه_B = matB.size(), اندازه_C = haveC ? matC.size() : اندازه(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    اندازه اندازه_D(((btrans) ? اندازه_B.لوړوالی : اندازه_B.پلنوالی),
               ((atrans) ? اندازه_A.پلنوالی : اندازه_A.لوړوالی));

    إذا (atrans)
        اندازه_A = اندازه(اندازه_A.لوړوالی, اندازه_A.پلنوالی);
    إذا (btrans)
        اندازه_B = اندازه(اندازه_B.لوړوالی, اندازه_B.پلنوالی);
    إذا (haveC && ctrans)
        اندازه_C = اندازه(اندازه_C.لوړوالی, اندازه_C.پلنوالی);

    CV_CheckEQ(اندازه_A.پلنوالی, اندازه_B.لوړوالی, "");
    إذا (haveC)
        CV_CheckEQ(اندازه_C, اندازه_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(اندازه_D, type);
    UMat D = matD.getUMat();

    منطقي isPropagatedC2D = خطأ;  // D content is updated with C / C.t()

    إذا (dev.intelSubgroupsSupport() && (ژوروالی == CV_32F) && cn == 1)
    {
        إذا (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copy_منزل(D);
            isPropagatedC2D = ریښتیا;
        }
        وإلا
        {
            beta = 0.0;
        }

        منطقي res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, اندازه_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        إذا (res)
            أرجع ریښتیا;
        // fallback on generic OpenCL code
    }

    إذا (اندازه_D.پلنوالی < 8 || اندازه_D.لوړوالی < 8)
        أرجع خطأ;

    String opts;

    صحيح wg_اندازه = (صحيح)dev.max_Work_ډله_اندازه();
    صحيح اندازه_Dmin = std::min(اندازه_D.پلنوالی, اندازه_D.لوړوالی);
    wg_اندازه = std::min(wg_اندازه, اندازه_Dmin * اندازه_Dmin);
    صحيح بلاک_اندازه_3 = (wg_اندازه / (32*cn) < 32) ? (wg_اندازه / (16*cn) < 16) ? (wg_اندازه / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    إذا (atrans)
        A = A.t();

    إذا (btrans)
        B = B.t();

    إذا (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_منزل(D);

    صحيح ویکټور_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    صحيح kercn = ocl::check_Optimal_ویکټور_پلنوالی(ویکټور_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_منزل_Str(type), ocl::type_منزل_Str(ژوروالی), ocl::type_منزل_Str(CV_MAKETYPE(ژوروالی, kercn)),
                      cn, kercn, بلاک_اندازه_3,
                      (اندازه_A.پلنوالی % بلاک_اندازه_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    إذا (ژوروالی == CV_64F)
        k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(A),
               ocl::KernelArg::Read_Only_No_اندازه(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               اندازه_A.پلنوالی, alpha, beta);
    وإلا
        k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(A),
               ocl::KernelArg::Read_Only_No_اندازه(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               اندازه_A.پلنوالی, (عائم)alpha, (عائم)beta);

    size_t globalsize[2] = { (size_t)اندازه_D.پلنوالی * cn / kercn, (size_t)اندازه_D.لوړوالی};
    size_t localsize[2] = { (size_t)بلاک_اندازه_3, (size_t)بلاک_اندازه_3};

    أرجع k.پرلپسې(2, globalsize, بلاک_اندازه_3 !=1 ? localsize : عدم, خطأ);
}
#د_که_پای


نطاق hal {

فراغ gemm32f(ثابت عائم* src1, size_t src1_ګام, ثابت عائم* src2, size_t src2_ګام,
             عائم alpha, ثابت عائم* src3, size_t src3_ګام, عائم beta, عائم* dst, size_t dst_ګام,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags)
#که_تعریف_شوی CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32f, (src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#د_که_پای
}

فراغ gemm64f(ثابت مزدوج* src1, size_t src1_ګام, ثابت مزدوج* src2, size_t src2_ګام,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_ګام, مزدوج beta, مزدوج* dst, size_t dst_ګام,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags)
#که_تعریف_شوی CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64f, (src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#د_که_پای
}

فراغ gemm32fc(ثابت عائم* src1, size_t src1_ګام, ثابت عائم* src2, size_t src2_ګام,
              عائم alpha, ثابت عائم* src3, size_t src3_ګام, عائم beta, عائم* dst, size_t dst_ګام,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags)
#که_تعریف_شوی CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#د_که_پای
}

فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_ګام, ثابت مزدوج* src2, size_t src2_ګام,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_ګام, مزدوج beta, مزدوج* dst, size_t dst_ګام,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags)
#که_تعریف_شوی CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_ګام, src2, src2_ګام, alpha, src3, src3_ګام, beta, dst, dst_ګام, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#د_که_پای
}

} // namespace hal

فراغ gemm(ننوت_Array matA, ننوت_Array matB, مزدوج alpha,
          ننوت_Array matC, مزدوج beta, OutputArray _matD, صحيح flags)
{
#که_تعریف_شوی HAVE_CLAMDBLAS
    CV_OCL_پرلپسې(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.ستون_شمېر() > 20 && matA.لیکې() > 20 && matB.ستون_شمېر() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#د_که_پای

#که_تعریف_شوی HAVE_OPENCL
    CV_OCL_پرلپسې(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#د_که_پای

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    اندازه a_اندازه = A.size(), d_اندازه;
    صحيح len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    بدل( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالة 0:
        d_اندازه = اندازه( B.ستون_شمېر, a_اندازه.لوړوالی );
        len = B.لیکې;
        CV_Assert( a_اندازه.پلنوالی == len );
        اكسر;
    حالة 1:
        d_اندازه = اندازه( B.ستون_شمېر, a_اندازه.پلنوالی );
        len = B.لیکې;
        CV_Assert( a_اندازه.لوړوالی == len );
        اكسر;
    حالة 2:
        d_اندازه = اندازه( B.لیکې, a_اندازه.لوړوالی );
        len = B.ستون_شمېر;
        CV_Assert( a_اندازه.پلنوالی == len );
        اكسر;
    حالة 3:
        d_اندازه = اندازه( B.لیکې, a_اندازه.پلنوالی );
        len = B.ستون_شمېر;
        CV_Assert( a_اندازه.لوړوالی == len );
        اكسر;
    }

    إذا( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.لیکې == d_اندازه.لوړوالی && C.ستون_شمېر == d_اندازه.پلنوالی) ||
             ((flags&GEMM_3_T) != 0 && C.لیکې == d_اندازه.پلنوالی && C.ستون_شمېر == d_اندازه.لوړوالی)));
    }

    _matD.create( d_اندازه.لوړوالی, d_اندازه.پلنوالی, type );
    Mat D = _matD.getMat();
    إذا( (flags & GEMM_3_T) != 0 && C.معلومات == D.معلومات )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    إذا( D.معلومات == A.معلومات || D.معلومات == B.معلومات )
    {
        DProxy = Mat(d_اندازه.لوړوالی, d_اندازه.پلنوالی, D.type());
        DProxyPtr = &DProxy;
    }

    إذا( type == CV_32FC1 )
        hal::gemm32f(A.ptr<عائم>(), A.ګام, B.ptr<عائم>(), B.ګام, static_cast<عائم>(alpha),
                     C.ptr<عائم>(), C.ګام, static_cast<عائم>(beta),
                     DProxyPtr->ptr<عائم>(), DProxyPtr->ګام,
                     a_اندازه.لوړوالی, a_اندازه.پلنوالی, DProxyPtr->ستون_شمېر, flags);
    وإلا إذا( type == CV_64FC1 )
        hal::gemm64f(A.ptr<مزدوج>(), A.ګام, B.ptr<مزدوج>(), B.ګام, alpha,
                     C.ptr<مزدوج>(), C.ګام, beta,
                     DProxyPtr->ptr<مزدوج>(), DProxyPtr->ګام,
                     a_اندازه.لوړوالی, a_اندازه.پلنوالی, DProxyPtr->ستون_شمېر, flags);
    وإلا إذا( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<عائم>(), A.ګام, B.ptr<عائم>(), B.ګام, static_cast<عائم>(alpha),
                      C.ptr<عائم>(), C.ګام, static_cast<عائم>(beta),
                      DProxyPtr->ptr<عائم>(), DProxyPtr->ګام,
                      a_اندازه.لوړوالی, a_اندازه.پلنوالی, DProxyPtr->ستون_شمېر, flags);
    وإلا
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<مزدوج>(), A.ګام, B.ptr<مزدوج>(), B.ګام, alpha,
                      C.ptr<مزدوج>(), C.ګام, beta,
                      DProxyPtr->ptr<مزدوج>(), DProxyPtr->ګام,
                      a_اندازه.لوړوالی, a_اندازه.پلنوالی, DProxyPtr->ستون_شمېر, flags);
    }

    إذا(DProxyPtr != &D)
        DProxyPtr->copy_منزل(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ساكن TransformFunc getTransformFunc(صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
}

ساكن TransformFunc getDiagTransformFunc(صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ transform(ننوت_Array _src, OutputArray _dst, ننوت_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح ژوروالی = src.ژوروالی(), scn = src.channels(), dcn = m.لیکې;
    CV_Assert( scn == m.ستون_شمېر || scn + 1 == m.ستون_شمېر );
    منطقي isDiag = خطأ;

    _dst.create_Same_اندازه( src, CV_MAKETYPE(ژوروالی, dcn) );
    Mat dst = _dst.getMat();

    إذا (src.معلومات == dst.معلومات)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    صحيح mtype = ژوروالی == CV_32S || ژوروالی == CV_64F ? CV_64F : CV_32F;
    Auto_بفر<مزدوج> _mbuf;
    مزدوج* mbuf;

    إذا( !m.isContinuous() || m.type() != mtype || m.ستون_شمېر != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.معلومات();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.ټول()*tmp.elem_اندازه());
        إذا( m.ستون_شمېر == scn+1 )
            m.convert_منزل(tmp, mtype);
        وإلا
        {
            Mat tmppart = tmp.ستون_Range_2(0, m.ستون_شمېر);
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

        لكل( i = 0, isDiag = ریښتیا; isDiag && i < scn; i++ )
        {
            لكل( j = 0; isDiag && j < scn; j++ )
            {
                مزدوج v = mtype == CV_32F ? m.at<عائم>(i, j) : m.at<مزدوج>(i, j);
                إذا( i != j && fabs(v) > eps )
                    isDiag = خطأ;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(ژوروالی): getTransformFunc(ژوروالی);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, ټول = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)ټول, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ساكن TransformFunc getPerspectiveTransform(صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ perspectiveTransform(ننوت_Array _src, OutputArray _dst, ننوت_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح ژوروالی = src.ژوروالی(), scn = src.channels(), dcn = m.لیکې-1;
    CV_Assert( scn + 1 == m.ستون_شمېر );
    CV_Assert( ژوروالی == CV_32F || ژوروالی == CV_64F );

    _dst.create_Same_اندازه( src, CV_MAKETYPE(ژوروالی, dcn) );
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

    TransformFunc func = getPerspectiveTransform(ژوروالی);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, ټول = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)ټول, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_scale_جمع_کړه( ننوت_Array _src1, مزدوج alpha, ننوت_Array _src2, OutputArray _dst, صحيح type )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    منطقي doubleSupport = d.doubleFPConfig() > 0;
    اندازه size = _src1.size();
    صحيح ژوروالی = CV_MAT_ژوروالی(type);
    إذا ( (!doubleSupport && ژوروالی == CV_64F) || size != _src2.size() )
        أرجع خطأ;

    _dst.create(size, type);
    صحيح cn = CV_MAT_CN(type), wdepth = std::max(ژوروالی, CV_32F);
    صحيح kercn = ocl::predict_Optimal_ویکټور_پلنوالی_Max(_src1, _src2, _dst),
        لیکې_فی_WI = d.isIntel() ? 4 : 1;

    حرف cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, kercn)), ژوروالی,
                         ocl::type_منزل_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(ژوروالی, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, ژوروالی, kercn, cvt[1], حجم(cvt[1])),
                         ocl::type_منزل_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", لیکې_فی_WI));
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_اندازه(src1),
            src2arg = ocl::KernelArg::Read_Only_No_اندازه(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    إذا (wdepth == CV_32F)
        k.دلیلونه(src1arg, src2arg, dstarg, (عائم)alpha);
    وإلا
        k.دلیلونه(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.ستون_شمېر * cn / kercn, ((size_t)dst.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
}

#د_که_پای

ساكن Scale_جمع_کړه_Func get_Scale_جمع_کړه_Func(صحيح ژوروالی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_جمع_کړه_Func, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ scale_جمع_کړه(ننوت_Array _src1, مزدوج alpha, ننوت_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src1.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_پرلپسې(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_جمع_کړه(_src1, alpha, _src2, _dst, type))

    إذا( ژوروالی != CV_32F && ژوروالی != CV_64F )
    {
        جمع_کړه_Weighted(_src1, alpha, _src2, 1, 0, _dst, ژوروالی);
        أرجع;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    عائم falpha = (عائم)alpha;
    فراغ* palpha = ژوروالی == CV_32F ? (فراغ*)&falpha : (فراغ*)&alpha;

    Scale_جمع_کړه_Func func = get_Scale_جمع_کړه_Func(ژوروالی);
    CV_Assert(func);

    إذا (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.ټول()*cn;
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

فراغ calc_Covar_ماتریکس( ثابت Mat* معلومات, صحيح nsamples, Mat& covar, Mat& منځنۍ, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( معلومات, nsamples > 0 );
    اندازه size = معلومات[0].size();
    صحيح sz = size.پلنوالی * size.لوړوالی, esz = (صحيح)معلومات[0].elem_اندازه();
    صحيح type = معلومات[0].type();
    Mat منځنۍ_2;
    ctype = std::max(std::max(CV_MAT_ژوروالی(ctype >= 0 ? ctype : type), منځنۍ.ژوروالی()), CV_32F);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( منځنۍ.size() == size );
        إذا( منځنۍ.isContinuous() && منځنۍ.type() == ctype )
            منځنۍ_2 = منځنۍ.reshape(1, 1);
        وإلا
        {
            منځنۍ.convert_منزل(منځنۍ_2, ctype);
            منځنۍ_2 = منځنۍ_2.reshape(1, 1);
        }
    }

    Mat معلومات_3(nsamples, sz, type);

    لكل( صحيح i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( معلومات[i].size() == size, معلومات[i].type() == type );
        إذا( معلومات[i].isContinuous() )
            حافظه_نقل( معلومات_3.ptr(i), معلومات[i].ptr(), sz*esz );
        وإلا
        {
            Mat معلومات_لیکه(size.لوړوالی, size.پلنوالی, type, معلومات_3.ptr(i));
            معلومات[i].copy_منزل(معلومات_لیکه);
        }
    }

    calc_Covar_ماتریکس( معلومات_3, covar, منځنۍ_2, (flags & ~(cv::COVAR_لیکې|cv::COVAR_ستون_شمېر)) | cv::COVAR_لیکې, ctype );
    إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        منځنۍ = منځنۍ_2.reshape(1, size.لوړوالی);
}

فراغ calc_Covar_ماتریکس( ننوت_Array _src, OutputArray _covar, ننوت_Output_Array منځنۍ, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    إذا(_src.ډول() == ننوت_Array_2::STD_ویکټور_MAT || _src.ډول() == ننوت_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_ویکټور(src);

        CV_Assert( src.size() > 0 );

        اندازه size = src[0].size();
        صحيح type = src[0].type();

        ctype = std::max(std::max(CV_MAT_ژوروالی(ctype >= 0 ? ctype : type), منځنۍ.ژوروالی()), CV_32F);

        Mat معلومات_3(static_cast<صحيح>(src.size()), size.area(), type);

        صحيح i = 0;
        لكل(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat معلومات_لیکه(size.لوړوالی, size.پلنوالی, type, معلومات_3.ptr(i));
            (*each).copy_منزل(معلومات_لیکه);
        }

        Mat منځنۍ_2;
        إذا( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( منځنۍ.size() == size );
            Mat mean0 = منځنۍ.getMat();

            إذا( mean0.isContinuous() && mean0.type() == ctype )
                منځنۍ_2 = mean0.reshape(1, 1);
            وإلا
            {
                mean0.convert_منزل(منځنۍ_2, ctype);
                منځنۍ_2 = منځنۍ_2.reshape(1, 1);
            }
        }

        calc_Covar_ماتریکس( معلومات_3, _covar, منځنۍ_2, (flags & ~(cv::COVAR_لیکې|cv::COVAR_ستون_شمېر)) | cv::COVAR_لیکې, ctype );

        إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            منځنۍ_2 = منځنۍ_2.reshape(1, size.لوړوالی);
            منځنۍ_2.copy_منزل(منځنۍ);
        }
        أرجع;
    }

    Mat معلومات = _src.getMat(), منځنۍ_2;
    CV_Assert( ((flags & cv::COVAR_لیکې) != 0) ^ ((flags & cv::COVAR_ستون_شمېر) != 0) );
    منطقي take_لیکې = (flags & cv::COVAR_لیکې) != 0;
    صحيح type = معلومات.type();
    صحيح nsamples = take_لیکې ? معلومات.لیکې : معلومات.ستون_شمېر;
    CV_Assert( nsamples > 0 );
    اندازه size = take_لیکې ? اندازه(معلومات.ستون_شمېر, 1) : اندازه(1, معلومات.لیکې);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        منځنۍ_2 = منځنۍ.getMat();
        ctype = std::max(std::max(CV_MAT_ژوروالی(ctype >= 0 ? ctype : type), منځنۍ_2.ژوروالی()), CV_32F);
        CV_Assert( منځنۍ_2.size() == size );
        إذا( منځنۍ_2.type() != ctype )
        {
            منځنۍ.create(منځنۍ_2.size(), ctype);
            Mat tmp = منځنۍ.getMat();
            منځنۍ_2.convert_منزل(tmp, ctype);
            منځنۍ_2 = tmp;
        }
    }
    وإلا
    {
        ctype = std::max(CV_MAT_ژوروالی(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, منځنۍ, take_لیکې ? 0 : 1, REDUCE_AVG, ctype );
        منځنۍ_2 = منځنۍ.getMat();
    }

    mulTransposed( معلومات, _covar, ((flags & cv::COVAR_نورمال) == 0) ^ take_لیکې,
        منځنۍ_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ساكن MahalanobisImplFunc getMahalanobisImplFunc(صحيح ژوروالی)
{
#که_تعریف_شوی CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (ژوروالی));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (ژوروالی),
        CV_CPU_DISPATCH_MODES_ALL);
#د_که_پای
}


مزدوج Mahalanobis(ننوت_Array _v1, ننوت_Array _v2, ننوت_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    صحيح type = v1.type(), ژوروالی = v1.ژوروالی();
    اندازه sz = v1.size();
    صحيح len = sz.پلنوالی*sz.لوړوالی*v1.channels();
    Auto_بفر<مزدوج> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.لیکې && len == icovar.ستون_شمېر );

    sz.پلنوالی *= v1.channels();
    إذا( v1.isContinuous() && v2.isContinuous() )
    {
        sz.پلنوالی *= sz.لوړوالی;
        sz.لوړوالی = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(ژوروالی);
    CV_Assert(func);

    مزدوج پایله = func(v1, v2, icovar, buf.معلومات(), len);
    أرجع std::جذر(پایله);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ساكن MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata)
{
#که_تعریف_شوی CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#د_که_پای
}

فراغ mulTransposed(ننوت_Array _src, OutputArray _dst, منطقي ata,
                   ننوت_Array _delta, مزدوج scale, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ثابت صحيح gemm_level = 100; // boundary above which GEMM is faster.
    صحيح stype = src.type();
    dtype = std::max(std::max(CV_MAT_ژوروالی(dtype >= 0 ? dtype : stype), delta.ژوروالی()), CV_32F);
    CV_Assert( src.channels() == 1 );

    إذا( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.لیکې == src.لیکې || delta.لیکې == 1),
            (delta.ستون_شمېر == src.ستون_شمېر || delta.ستون_شمېر == 1));
        إذا( delta.type() != dtype )
            delta.convert_منزل(delta, dtype);
    }

    صحيح dsize = ata ? src.ستون_شمېر : src.لیکې;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    إذا( src.معلومات == dst.معلومات || (stype == dtype &&
        (dst.ستون_شمېر >= gemm_level && dst.لیکې >= gemm_level &&
         src.ستون_شمېر >= gemm_level && src.لیکې >= gemm_level)))
    {
        Mat src2;
        ثابت Mat* tsrc = &src;
        إذا( !delta.empty() )
        {
            إذا( delta.size() == src.size() )
                کمول( src, delta, src2 );
            وإلا
            {
                repeat(delta, src.لیکې/delta.لیکې, src.ستون_شمېر/delta.ستون_شمېر, src2);
                کمول( src, src2, src2 );
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
    CV_CPU_DISPATCH(dotProd_16s, ((ثابت لنډ*)src1, (ثابت لنډ*)src2, len),
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

ساكن DotProdFunc getDotProdFunc(صحيح ژوروالی)
{
    ساكن DotProdFunc dotProdTab[CV_ژوروالی_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    أرجع dotProdTab[ژوروالی];
}

مزدوج Mat::dot(ننوت_Array _mat) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    صحيح cn = channels();
    إذا (هذا->dims <= 2)
    {
        مزدوج حاصل_ضرب = 0;
        CALL_HAL_RET(dot_حاصل_ضرب, cv_hal_dot_حاصل_ضرب, حاصل_ضرب, هذا->معلومات, هذا->ګام, mat.معلومات, mat.ګام,
                     هذا->ستون_شمېر * cn, هذا->لیکې, هذا->ژوروالی());
    }

    DotProdFunc func = getDotProdFunc(ژوروالی());
    CV_Assert(func != 0 );

    إذا( isContinuous() && mat.isContinuous() )
    {
        size_t len = ټول()*cn;
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


#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_dot( ننوت_Array _src1, ننوت_Array _src2, مزدوج & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    صحيح type = src1.type(), ژوروالی = CV_MAT_ژوروالی(type),
            kercn = ocl::predict_Optimal_ویکټور_پلنوالی(src1, src2);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا ( !doubleSupport && ژوروالی == CV_64F )
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_ډله_اندازه();
    صحيح ddepth = std::max(CV_32F, ژوروالی);

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    حرف cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, kercn)), ocl::type_منزل_Str(ژوروالی),
                         ocl::type_منزل_Str(ddepth), ocl::type_منزل_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(ژوروالی, ddepth, kercn, cvt, حجم(cvt)),
                         (صحيح)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_اندازه(src1),
            src2arg = ocl::KernelArg::Read_Only_No_اندازه(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.دلیلونه(src1arg, src1.ستون_شمېر, (صحيح)src1.ټول(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    إذا (k.پرلپسې(1, &globalsize, &wgs, ریښتیا))
    {
        res = مجموعه(db.getMat(ACCESS_READ))[0];
        أرجع ریښتیا;
    }
    أرجع خطأ;
}

#د_که_پای

مزدوج UMat::dot(ننوت_Array m) ثابت
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_اندازه(*هذا) && m.type() == type());

#که_تعریف_شوی HAVE_OPENCL
    مزدوج r = 0;
    CV_OCL_پرلپسې_2(dims <= 2, ocl_dot(*هذا, m, r), r)
#د_که_پای

    أرجع getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
