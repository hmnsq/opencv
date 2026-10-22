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

#اگر_تعریف_شده HAVE_CLAMDBLAS

ساكن منطقي ocl_gemm_amdblas( درون_داد_Array matA, درون_داد_Array matB, مزدوج alpha,
                      درون_داد_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type(), esz = CV_ELEM_اندازه(type);
    منطقي haveC = matC.نوع() != cv::درون_داد_Array_2::NONE;
    اندازه اندازه_A = matA.size(), اندازه_B = matB.size(), اندازه_C = haveC ? matC.size() : اندازه(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (atrans)
        اندازه_A = اندازه(اندازه_A.ارتفاع, اندازه_A.عرض);
    إذا (btrans)
        اندازه_B = اندازه(اندازه_B.ارتفاع, اندازه_B.عرض);
    إذا (haveC && ctrans)
        اندازه_C = اندازه(اندازه_C.ارتفاع, اندازه_C.عرض);

    اندازه اندازه_D(اندازه_B.عرض, اندازه_A.ارتفاع);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( اندازه_A.عرض == اندازه_B.ارتفاع && (!haveC || اندازه_C == اندازه_D) );

    matD.create(اندازه_D, type);
    إذا ( matA.offset() % esz != 0 || matA.گام() % esz != 0 ||
         matB.offset() % esz != 0 || matB.گام() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.گام() % esz != 0)) )
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
        ctrans ? transpose(matC, D) : matC.copy_مقصد(D);
    وإلا
        D.مجموعه_مقصد(Scalar::all(0));

    صحيح M = اندازه_D.ارتفاع, N = اندازه_D.عرض, K = اندازه_A.عرض;
    صحيح lda = (صحيح)A.گام / esz, ldb = (صحيح)B.گام / esz, ldc = (صحيح)D.گام / esz;
    صحيح offa = (صحيح)A.offset / esz, offb = (صحيح)B.offset / esz, offc = (صحيح)D.offset / esz;

    cl_command_صف clq = (cl_command_صف)ocl::صف::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_ترتیب ترتیب = clblas_ردیف_Major;
    clblas_وضعیت وضعیت = clblasSuccess;

    إذا (type == CV_32FC1)
        وضعیت = clblasSgemm(ترتیب, transA, transB, M, N, K,
                             (cl_float)alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    وإلا إذا (type == CV_64FC1)
        وضعیت = clblasDgemm(ترتیب, transA, transB, M, N, K,
                             alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    وإلا إذا (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         وضعیت = clblasCgemm(ترتیب, transA, transB, M, N, K,
                              alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, عدم, عدم);
    }
    وإلا إذا (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        وضعیت = clblasZgemm(ترتیب, transA, transB, M, N, K,
                             alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    }
    وإلا
        CV_Error(Error::StsUnsupportedFormat, "");

    أرجع وضعیت == clblasSuccess;
}

#پایان_اگر

#اگر_تعریف_شده HAVE_OPENCL
ساكن منطقي ocl_gemm( درون_داد_Array matA, درون_داد_Array matB, مزدوج alpha,
                      درون_داد_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type();
    صحيح عمق = CV_MAT_عمق(type);
    صحيح cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0;

    إذا (!doubleSupport && عمق == CV_64F)
        أرجع خطأ;

    منطقي haveC = matC.نوع() != cv::درون_داد_Array_2::NONE;
    اندازه اندازه_A = matA.size(), اندازه_B = matB.size(), اندازه_C = haveC ? matC.size() : اندازه(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    اندازه اندازه_D(((btrans) ? اندازه_B.ارتفاع : اندازه_B.عرض),
               ((atrans) ? اندازه_A.عرض : اندازه_A.ارتفاع));

    إذا (atrans)
        اندازه_A = اندازه(اندازه_A.ارتفاع, اندازه_A.عرض);
    إذا (btrans)
        اندازه_B = اندازه(اندازه_B.ارتفاع, اندازه_B.عرض);
    إذا (haveC && ctrans)
        اندازه_C = اندازه(اندازه_C.ارتفاع, اندازه_C.عرض);

    CV_CheckEQ(اندازه_A.عرض, اندازه_B.ارتفاع, "");
    إذا (haveC)
        CV_CheckEQ(اندازه_C, اندازه_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(اندازه_D, type);
    UMat D = matD.getUMat();

    منطقي isPropagatedC2D = خطأ;  // D content is updated with C / C.t()

    إذا (dev.intelSubgroupsSupport() && (عمق == CV_32F) && cn == 1)
    {
        إذا (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copy_مقصد(D);
            isPropagatedC2D = درست;
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
            أرجع درست;
        // fallback on generic OpenCL code
    }

    إذا (اندازه_D.عرض < 8 || اندازه_D.ارتفاع < 8)
        أرجع خطأ;

    String opts;

    صحيح wg_اندازه = (صحيح)dev.max_Work_گروه_اندازه();
    صحيح اندازه_Dmin = std::min(اندازه_D.عرض, اندازه_D.ارتفاع);
    wg_اندازه = std::min(wg_اندازه, اندازه_Dmin * اندازه_Dmin);
    صحيح بلوک_اندازه_3 = (wg_اندازه / (32*cn) < 32) ? (wg_اندازه / (16*cn) < 16) ? (wg_اندازه / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    إذا (atrans)
        A = A.t();

    إذا (btrans)
        B = B.t();

    إذا (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_مقصد(D);

    صحيح وکتور_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    صحيح kercn = ocl::check_Optimal_وکتور_عرض(وکتور_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_مقصد_Str(type), ocl::type_مقصد_Str(عمق), ocl::type_مقصد_Str(CV_MAKETYPE(عمق, kercn)),
                      cn, kercn, بلوک_اندازه_3,
                      (اندازه_A.عرض % بلوک_اندازه_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    إذا (عمق == CV_64F)
        k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(A),
               ocl::KernelArg::Read_Only_No_اندازه(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               اندازه_A.عرض, alpha, beta);
    وإلا
        k.آرگومانها(ocl::KernelArg::Read_Only_No_اندازه(A),
               ocl::KernelArg::Read_Only_No_اندازه(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               اندازه_A.عرض, (عائم)alpha, (عائم)beta);

    size_t globalsize[2] = { (size_t)اندازه_D.عرض * cn / kercn, (size_t)اندازه_D.ارتفاع};
    size_t localsize[2] = { (size_t)بلوک_اندازه_3, (size_t)بلوک_اندازه_3};

    أرجع k.پیاپی(2, globalsize, بلوک_اندازه_3 !=1 ? localsize : عدم, خطأ);
}
#پایان_اگر


نطاق hal {

فراغ gemm32f(ثابت عائم* src1, size_t src1_گام, ثابت عائم* src2, size_t src2_گام,
             عائم alpha, ثابت عائم* src3, size_t src3_گام, عائم beta, عائم* dst, size_t dst_گام,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags)
#اگر_تعریف_شده CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32f, (src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#پایان_اگر
}

فراغ gemm64f(ثابت مزدوج* src1, size_t src1_گام, ثابت مزدوج* src2, size_t src2_گام,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_گام, مزدوج beta, مزدوج* dst, size_t dst_گام,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags)
#اگر_تعریف_شده CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64f, (src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#پایان_اگر
}

فراغ gemm32fc(ثابت عائم* src1, size_t src1_گام, ثابت عائم* src2, size_t src2_گام,
              عائم alpha, ثابت عائم* src3, size_t src3_گام, عائم beta, عائم* dst, size_t dst_گام,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags)
#اگر_تعریف_شده CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#پایان_اگر
}

فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_گام, ثابت مزدوج* src2, size_t src2_گام,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_گام, مزدوج beta, مزدوج* dst, size_t dst_گام,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags)
#اگر_تعریف_شده CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_گام, src2, src2_گام, alpha, src3, src3_گام, beta, dst, dst_گام, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#پایان_اگر
}

} // namespace hal

فراغ gemm(درون_داد_Array matA, درون_داد_Array matB, مزدوج alpha,
          درون_داد_Array matC, مزدوج beta, OutputArray _matD, صحيح flags)
{
#اگر_تعریف_شده HAVE_CLAMDBLAS
    CV_OCL_پیاپی(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.تعداد_ستون() > 20 && matA.ردیفها() > 20 && matB.تعداد_ستون() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#پایان_اگر

#اگر_تعریف_شده HAVE_OPENCL
    CV_OCL_پیاپی(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#پایان_اگر

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    اندازه a_اندازه = A.size(), d_اندازه;
    صحيح len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    بدل( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالة 0:
        d_اندازه = اندازه( B.تعداد_ستون, a_اندازه.ارتفاع );
        len = B.ردیفها;
        CV_Assert( a_اندازه.عرض == len );
        اكسر;
    حالة 1:
        d_اندازه = اندازه( B.تعداد_ستون, a_اندازه.عرض );
        len = B.ردیفها;
        CV_Assert( a_اندازه.ارتفاع == len );
        اكسر;
    حالة 2:
        d_اندازه = اندازه( B.ردیفها, a_اندازه.ارتفاع );
        len = B.تعداد_ستون;
        CV_Assert( a_اندازه.عرض == len );
        اكسر;
    حالة 3:
        d_اندازه = اندازه( B.ردیفها, a_اندازه.عرض );
        len = B.تعداد_ستون;
        CV_Assert( a_اندازه.ارتفاع == len );
        اكسر;
    }

    إذا( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.ردیفها == d_اندازه.ارتفاع && C.تعداد_ستون == d_اندازه.عرض) ||
             ((flags&GEMM_3_T) != 0 && C.ردیفها == d_اندازه.عرض && C.تعداد_ستون == d_اندازه.ارتفاع)));
    }

    _matD.create( d_اندازه.ارتفاع, d_اندازه.عرض, type );
    Mat D = _matD.getMat();
    إذا( (flags & GEMM_3_T) != 0 && C.داده == D.داده )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    إذا( D.داده == A.داده || D.داده == B.داده )
    {
        DProxy = Mat(d_اندازه.ارتفاع, d_اندازه.عرض, D.type());
        DProxyPtr = &DProxy;
    }

    إذا( type == CV_32FC1 )
        hal::gemm32f(A.ptr<عائم>(), A.گام, B.ptr<عائم>(), B.گام, static_cast<عائم>(alpha),
                     C.ptr<عائم>(), C.گام, static_cast<عائم>(beta),
                     DProxyPtr->ptr<عائم>(), DProxyPtr->گام,
                     a_اندازه.ارتفاع, a_اندازه.عرض, DProxyPtr->تعداد_ستون, flags);
    وإلا إذا( type == CV_64FC1 )
        hal::gemm64f(A.ptr<مزدوج>(), A.گام, B.ptr<مزدوج>(), B.گام, alpha,
                     C.ptr<مزدوج>(), C.گام, beta,
                     DProxyPtr->ptr<مزدوج>(), DProxyPtr->گام,
                     a_اندازه.ارتفاع, a_اندازه.عرض, DProxyPtr->تعداد_ستون, flags);
    وإلا إذا( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<عائم>(), A.گام, B.ptr<عائم>(), B.گام, static_cast<عائم>(alpha),
                      C.ptr<عائم>(), C.گام, static_cast<عائم>(beta),
                      DProxyPtr->ptr<عائم>(), DProxyPtr->گام,
                      a_اندازه.ارتفاع, a_اندازه.عرض, DProxyPtr->تعداد_ستون, flags);
    وإلا
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<مزدوج>(), A.گام, B.ptr<مزدوج>(), B.گام, alpha,
                      C.ptr<مزدوج>(), C.گام, beta,
                      DProxyPtr->ptr<مزدوج>(), DProxyPtr->گام,
                      a_اندازه.ارتفاع, a_اندازه.عرض, DProxyPtr->تعداد_ستون, flags);
    }

    إذا(DProxyPtr != &D)
        DProxyPtr->copy_مقصد(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ساكن TransformFunc getTransformFunc(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

ساكن TransformFunc getDiagTransformFunc(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ transform(درون_داد_Array _src, OutputArray _dst, درون_داد_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح عمق = src.عمق(), scn = src.channels(), dcn = m.ردیفها;
    CV_Assert( scn == m.تعداد_ستون || scn + 1 == m.تعداد_ستون );
    منطقي isDiag = خطأ;

    _dst.create_Same_اندازه( src, CV_MAKETYPE(عمق, dcn) );
    Mat dst = _dst.getMat();

    إذا (src.داده == dst.داده)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    صحيح mtype = عمق == CV_32S || عمق == CV_64F ? CV_64F : CV_32F;
    Auto_میانگیر<مزدوج> _mbuf;
    مزدوج* mbuf;

    إذا( !m.isContinuous() || m.type() != mtype || m.تعداد_ستون != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.داده();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.مجموع()*tmp.elem_اندازه());
        إذا( m.تعداد_ستون == scn+1 )
            m.convert_مقصد(tmp, mtype);
        وإلا
        {
            Mat tmppart = tmp.ستون_Range_2(0, m.تعداد_ستون);
            m.convert_مقصد(tmppart, mtype);
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
            src.convert_مقصد(dst, dst.type(), alpha, beta);
            أرجع;
        }

        لكل( i = 0, isDiag = درست; isDiag && i < scn; i++ )
        {
            لكل( j = 0; isDiag && j < scn; j++ )
            {
                مزدوج v = mtype == CV_32F ? m.at<عائم>(i, j) : m.at<مزدوج>(i, j);
                إذا( i != j && fabs(v) > eps )
                    isDiag = خطأ;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(عمق): getTransformFunc(عمق);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, مجموع = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)مجموع, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ساكن TransformFunc getPerspectiveTransform(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ perspectiveTransform(درون_داد_Array _src, OutputArray _dst, درون_داد_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح عمق = src.عمق(), scn = src.channels(), dcn = m.ردیفها-1;
    CV_Assert( scn + 1 == m.تعداد_ستون );
    CV_Assert( عمق == CV_32F || عمق == CV_64F );

    _dst.create_Same_اندازه( src, CV_MAKETYPE(عمق, dcn) );
    Mat dst = _dst.getMat();

    ثابت صحيح mtype = CV_64F;
    Auto_میانگیر<مزدوج> _mbuf;
    مزدوج* mbuf = m.ptr<مزدوج>();

    إذا( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.داده();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convert_مقصد(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(عمق);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, مجموع = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)مجموع, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_scale_جمع_کن( درون_داد_Array _src1, مزدوج alpha, درون_داد_Array _src2, OutputArray _dst, صحيح type )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    منطقي doubleSupport = d.doubleFPConfig() > 0;
    اندازه size = _src1.size();
    صحيح عمق = CV_MAT_عمق(type);
    إذا ( (!doubleSupport && عمق == CV_64F) || size != _src2.size() )
        أرجع خطأ;

    _dst.create(size, type);
    صحيح cn = CV_MAT_CN(type), wdepth = std::max(عمق, CV_32F);
    صحيح kercn = ocl::predict_Optimal_وکتور_عرض_Max(_src1, _src2, _dst),
        ردیفها_در_هر_WI = d.isIntel() ? 4 : 1;

    حرف cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(عمق, kercn)), عمق,
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(عمق, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, عمق, kercn, cvt[1], حجم(cvt[1])),
                         ocl::type_مقصد_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", ردیفها_در_هر_WI));
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_اندازه(src1),
            src2arg = ocl::KernelArg::Read_Only_No_اندازه(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    إذا (wdepth == CV_32F)
        k.آرگومانها(src1arg, src2arg, dstarg, (عائم)alpha);
    وإلا
        k.آرگومانها(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.تعداد_ستون * cn / kercn, ((size_t)dst.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

ساكن Scale_جمع_کن_Func get_Scale_جمع_کن_Func(صحيح عمق)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_جمع_کن_Func, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ scale_جمع_کن(درون_داد_Array _src1, مزدوج alpha, درون_داد_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src1.type(), عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_پیاپی(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_جمع_کن(_src1, alpha, _src2, _dst, type))

    إذا( عمق != CV_32F && عمق != CV_64F )
    {
        جمع_کن_Weighted(_src1, alpha, _src2, 1, 0, _dst, عمق);
        أرجع;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    عائم falpha = (عائم)alpha;
    فراغ* palpha = عمق == CV_32F ? (فراغ*)&falpha : (فراغ*)&alpha;

    Scale_جمع_کن_Func func = get_Scale_جمع_کن_Func(عمق);
    CV_Assert(func);

    إذا (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.مجموع()*cn;
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

فراغ calc_Covar_ماتریس( ثابت Mat* داده, صحيح nsamples, Mat& covar, Mat& میانگین, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( داده, nsamples > 0 );
    اندازه size = داده[0].size();
    صحيح sz = size.عرض * size.ارتفاع, esz = (صحيح)داده[0].elem_اندازه();
    صحيح type = داده[0].type();
    Mat میانگین_2;
    ctype = std::max(std::max(CV_MAT_عمق(ctype >= 0 ? ctype : type), میانگین.عمق()), CV_32F);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( میانگین.size() == size );
        إذا( میانگین.isContinuous() && میانگین.type() == ctype )
            میانگین_2 = میانگین.reshape(1, 1);
        وإلا
        {
            میانگین.convert_مقصد(میانگین_2, ctype);
            میانگین_2 = میانگین_2.reshape(1, 1);
        }
    }

    Mat داده_3(nsamples, sz, type);

    لكل( صحيح i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( داده[i].size() == size, داده[i].type() == type );
        إذا( داده[i].isContinuous() )
            کپی_حافظه( داده_3.ptr(i), داده[i].ptr(), sz*esz );
        وإلا
        {
            Mat داده_ردیف(size.ارتفاع, size.عرض, type, داده_3.ptr(i));
            داده[i].copy_مقصد(داده_ردیف);
        }
    }

    calc_Covar_ماتریس( داده_3, covar, میانگین_2, (flags & ~(cv::COVAR_ردیفها|cv::COVAR_تعداد_ستون)) | cv::COVAR_ردیفها, ctype );
    إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        میانگین = میانگین_2.reshape(1, size.ارتفاع);
}

فراغ calc_Covar_ماتریس( درون_داد_Array _src, OutputArray _covar, درون_داد_Output_Array میانگین, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    إذا(_src.نوع() == درون_داد_Array_2::STD_وکتور_MAT || _src.نوع() == درون_داد_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_وکتور(src);

        CV_Assert( src.size() > 0 );

        اندازه size = src[0].size();
        صحيح type = src[0].type();

        ctype = std::max(std::max(CV_MAT_عمق(ctype >= 0 ? ctype : type), میانگین.عمق()), CV_32F);

        Mat داده_3(static_cast<صحيح>(src.size()), size.area(), type);

        صحيح i = 0;
        لكل(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat داده_ردیف(size.ارتفاع, size.عرض, type, داده_3.ptr(i));
            (*each).copy_مقصد(داده_ردیف);
        }

        Mat میانگین_2;
        إذا( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( میانگین.size() == size );
            Mat mean0 = میانگین.getMat();

            إذا( mean0.isContinuous() && mean0.type() == ctype )
                میانگین_2 = mean0.reshape(1, 1);
            وإلا
            {
                mean0.convert_مقصد(میانگین_2, ctype);
                میانگین_2 = میانگین_2.reshape(1, 1);
            }
        }

        calc_Covar_ماتریس( داده_3, _covar, میانگین_2, (flags & ~(cv::COVAR_ردیفها|cv::COVAR_تعداد_ستون)) | cv::COVAR_ردیفها, ctype );

        إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            میانگین_2 = میانگین_2.reshape(1, size.ارتفاع);
            میانگین_2.copy_مقصد(میانگین);
        }
        أرجع;
    }

    Mat داده = _src.getMat(), میانگین_2;
    CV_Assert( ((flags & cv::COVAR_ردیفها) != 0) ^ ((flags & cv::COVAR_تعداد_ستون) != 0) );
    منطقي take_ردیفها = (flags & cv::COVAR_ردیفها) != 0;
    صحيح type = داده.type();
    صحيح nsamples = take_ردیفها ? داده.ردیفها : داده.تعداد_ستون;
    CV_Assert( nsamples > 0 );
    اندازه size = take_ردیفها ? اندازه(داده.تعداد_ستون, 1) : اندازه(1, داده.ردیفها);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        میانگین_2 = میانگین.getMat();
        ctype = std::max(std::max(CV_MAT_عمق(ctype >= 0 ? ctype : type), میانگین_2.عمق()), CV_32F);
        CV_Assert( میانگین_2.size() == size );
        إذا( میانگین_2.type() != ctype )
        {
            میانگین.create(میانگین_2.size(), ctype);
            Mat tmp = میانگین.getMat();
            میانگین_2.convert_مقصد(tmp, ctype);
            میانگین_2 = tmp;
        }
    }
    وإلا
    {
        ctype = std::max(CV_MAT_عمق(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, میانگین, take_ردیفها ? 0 : 1, REDUCE_AVG, ctype );
        میانگین_2 = میانگین.getMat();
    }

    mulTransposed( داده, _covar, ((flags & cv::COVAR_نرمال) == 0) ^ take_ردیفها,
        میانگین_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ساكن MahalanobisImplFunc getMahalanobisImplFunc(صحيح عمق)
{
#اگر_تعریف_شده CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (عمق));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (عمق),
        CV_CPU_DISPATCH_MODES_ALL);
#پایان_اگر
}


مزدوج Mahalanobis(درون_داد_Array _v1, درون_داد_Array _v2, درون_داد_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    صحيح type = v1.type(), عمق = v1.عمق();
    اندازه sz = v1.size();
    صحيح len = sz.عرض*sz.ارتفاع*v1.channels();
    Auto_میانگیر<مزدوج> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.ردیفها && len == icovar.تعداد_ستون );

    sz.عرض *= v1.channels();
    إذا( v1.isContinuous() && v2.isContinuous() )
    {
        sz.عرض *= sz.ارتفاع;
        sz.ارتفاع = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(عمق);
    CV_Assert(func);

    مزدوج نتیجه = func(v1, v2, icovar, buf.داده(), len);
    أرجع std::جذر(نتیجه);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ساكن MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata)
{
#اگر_تعریف_شده CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#پایان_اگر
}

فراغ mulTransposed(درون_داد_Array _src, OutputArray _dst, منطقي ata,
                   درون_داد_Array _delta, مزدوج scale, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ثابت صحيح gemm_level = 100; // boundary above which GEMM is faster.
    صحيح stype = src.type();
    dtype = std::max(std::max(CV_MAT_عمق(dtype >= 0 ? dtype : stype), delta.عمق()), CV_32F);
    CV_Assert( src.channels() == 1 );

    إذا( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.ردیفها == src.ردیفها || delta.ردیفها == 1),
            (delta.تعداد_ستون == src.تعداد_ستون || delta.تعداد_ستون == 1));
        إذا( delta.type() != dtype )
            delta.convert_مقصد(delta, dtype);
    }

    صحيح dsize = ata ? src.تعداد_ستون : src.ردیفها;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    إذا( src.داده == dst.داده || (stype == dtype &&
        (dst.تعداد_ستون >= gemm_level && dst.ردیفها >= gemm_level &&
         src.تعداد_ستون >= gemm_level && src.ردیفها >= gemm_level)))
    {
        Mat src2;
        ثابت Mat* tsrc = &src;
        إذا( !delta.empty() )
        {
            إذا( delta.size() == src.size() )
                تفریق( src, delta, src2 );
            وإلا
            {
                repeat(delta, src.ردیفها/delta.ردیفها, src.تعداد_ستون/delta.تعداد_ستون, src2);
                تفریق( src, src2, src2 );
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
    CV_CPU_DISPATCH(dotProd_16s, ((ثابت کوتاه*)src1, (ثابت کوتاه*)src2, len),
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

ساكن DotProdFunc getDotProdFunc(صحيح عمق)
{
    ساكن DotProdFunc dotProdTab[CV_عمق_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    أرجع dotProdTab[عمق];
}

مزدوج Mat::dot(درون_داد_Array _mat) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    صحيح cn = channels();
    إذا (هذا->dims <= 2)
    {
        مزدوج حاصلضرب = 0;
        CALL_HAL_RET(dot_حاصلضرب, cv_hal_dot_حاصلضرب, حاصلضرب, هذا->داده, هذا->گام, mat.داده, mat.گام,
                     هذا->تعداد_ستون * cn, هذا->ردیفها, هذا->عمق());
    }

    DotProdFunc func = getDotProdFunc(عمق());
    CV_Assert(func != 0 );

    إذا( isContinuous() && mat.isContinuous() )
    {
        size_t len = مجموع()*cn;
        إذا( len == (size_t)(صحيح)len )
            أرجع func(داده, mat.داده, (صحيح)len);
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


#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_dot( درون_داد_Array _src1, درون_داد_Array _src2, مزدوج & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    صحيح type = src1.type(), عمق = CV_MAT_عمق(type),
            kercn = ocl::predict_Optimal_وکتور_عرض(src1, src2);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا ( !doubleSupport && عمق == CV_64F )
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروه_اندازه();
    صحيح ddepth = std::max(CV_32F, عمق);

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    حرف cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(عمق, kercn)), ocl::type_مقصد_Str(عمق),
                         ocl::type_مقصد_Str(ddepth), ocl::type_مقصد_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(عمق, ddepth, kercn, cvt, حجم(cvt)),
                         (صحيح)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_اندازه(src1),
            src2arg = ocl::KernelArg::Read_Only_No_اندازه(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.آرگومانها(src1arg, src1.تعداد_ستون, (صحيح)src1.مجموع(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    إذا (k.پیاپی(1, &globalsize, &wgs, درست))
    {
        res = جمع(db.getMat(ACCESS_READ))[0];
        أرجع درست;
    }
    أرجع خطأ;
}

#پایان_اگر

مزدوج UMat::dot(درون_داد_Array m) ثابت
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_اندازه(*هذا) && m.type() == type());

#اگر_تعریف_شده HAVE_OPENCL
    مزدوج r = 0;
    CV_OCL_پیاپی_2(dims <= 2, ocl_dot(*هذا, m, r), r)
#پایان_اگر

    أرجع getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
