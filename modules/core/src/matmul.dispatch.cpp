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

#جے_تعریف_شدہ HAVE_CLAMDBLAS

ساكن منطقي ocl_gemm_amdblas( داخلہ_Array matA, داخلہ_Array matB, مزدوج alpha,
                      داخلہ_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type(), esz = CV_ELEM_ناپ(type);
    منطقي haveC = matC.قسم() != cv::داخلہ_Array_2::NONE;
    ناپ ناپ_A = matA.size(), ناپ_B = matB.size(), ناپ_C = haveC ? matC.size() : ناپ(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (atrans)
        ناپ_A = ناپ(ناپ_A.اچائی, ناپ_A.چوڑائی);
    إذا (btrans)
        ناپ_B = ناپ(ناپ_B.اچائی, ناپ_B.چوڑائی);
    إذا (haveC && ctrans)
        ناپ_C = ناپ(ناپ_C.اچائی, ناپ_C.چوڑائی);

    ناپ ناپ_D(ناپ_B.چوڑائی, ناپ_A.اچائی);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( ناپ_A.چوڑائی == ناپ_B.اچائی && (!haveC || ناپ_C == ناپ_D) );

    matD.create(ناپ_D, type);
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
        D.سیٹ_منزل(Scalar::all(0));

    صحيح M = ناپ_D.اچائی, N = ناپ_D.چوڑائی, K = ناپ_A.چوڑائی;
    صحيح lda = (صحيح)A.قدم / esz, ldb = (صحيح)B.قدم / esz, ldc = (صحيح)D.قدم / esz;
    صحيح offa = (صحيح)A.offset / esz, offb = (صحيح)B.offset / esz, offc = (صحيح)D.offset / esz;

    cl_command_قطار clq = (cl_command_قطار)ocl::قطار::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_ترتیب ترتیب = clblas_صف_Major;
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

#جے_انت

#جے_تعریف_شدہ HAVE_OPENCL
ساكن منطقي ocl_gemm( داخلہ_Array matA, داخلہ_Array matB, مزدوج alpha,
                      داخلہ_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type();
    صحيح ڈونگھائی = CV_MAT_ڈونگھائی(type);
    صحيح cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0;

    إذا (!doubleSupport && ڈونگھائی == CV_64F)
        أرجع خطأ;

    منطقي haveC = matC.قسم() != cv::داخلہ_Array_2::NONE;
    ناپ ناپ_A = matA.size(), ناپ_B = matB.size(), ناپ_C = haveC ? matC.size() : ناپ(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    ناپ ناپ_D(((btrans) ? ناپ_B.اچائی : ناپ_B.چوڑائی),
               ((atrans) ? ناپ_A.چوڑائی : ناپ_A.اچائی));

    إذا (atrans)
        ناپ_A = ناپ(ناپ_A.اچائی, ناپ_A.چوڑائی);
    إذا (btrans)
        ناپ_B = ناپ(ناپ_B.اچائی, ناپ_B.چوڑائی);
    إذا (haveC && ctrans)
        ناپ_C = ناپ(ناپ_C.اچائی, ناپ_C.چوڑائی);

    CV_CheckEQ(ناپ_A.چوڑائی, ناپ_B.اچائی, "");
    إذا (haveC)
        CV_CheckEQ(ناپ_C, ناپ_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(ناپ_D, type);
    UMat D = matD.getUMat();

    منطقي isPropagatedC2D = خطأ;  // D content is updated with C / C.t()

    إذا (dev.intelSubgroupsSupport() && (ڈونگھائی == CV_32F) && cn == 1)
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
                                  D, ناپ_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        إذا (res)
            أرجع سچ;
        // fallback on generic OpenCL code
    }

    إذا (ناپ_D.چوڑائی < 8 || ناپ_D.اچائی < 8)
        أرجع خطأ;

    String opts;

    صحيح wg_ناپ = (صحيح)dev.max_Work_گروہ_ناپ();
    صحيح ناپ_Dmin = std::min(ناپ_D.چوڑائی, ناپ_D.اچائی);
    wg_ناپ = std::min(wg_ناپ, ناپ_Dmin * ناپ_Dmin);
    صحيح بلاک_ناپ_3 = (wg_ناپ / (32*cn) < 32) ? (wg_ناپ / (16*cn) < 16) ? (wg_ناپ / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    إذا (atrans)
        A = A.t();

    إذا (btrans)
        B = B.t();

    إذا (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_منزل(D);

    صحيح سمتیہ_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    صحيح kercn = ocl::check_Optimal_سمتیہ_چوڑائی(سمتیہ_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_منزل_Str(type), ocl::type_منزل_Str(ڈونگھائی), ocl::type_منزل_Str(CV_MAKETYPE(ڈونگھائی, kercn)),
                      cn, kercn, بلاک_ناپ_3,
                      (ناپ_A.چوڑائی % بلاک_ناپ_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    إذا (ڈونگھائی == CV_64F)
        k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(A),
               ocl::KernelArg::Read_Only_No_ناپ(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               ناپ_A.چوڑائی, alpha, beta);
    وإلا
        k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(A),
               ocl::KernelArg::Read_Only_No_ناپ(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               ناپ_A.چوڑائی, (عائم)alpha, (عائم)beta);

    size_t globalsize[2] = { (size_t)ناپ_D.چوڑائی * cn / kercn, (size_t)ناپ_D.اچائی};
    size_t localsize[2] = { (size_t)بلاک_ناپ_3, (size_t)بلاک_ناپ_3};

    أرجع k.لگاتار(2, globalsize, بلاک_ناپ_3 !=1 ? localsize : عدم, خطأ);
}
#جے_انت


نطاق hal {

فراغ gemm32f(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
             عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#جے_تعریف_شدہ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#جے_انت
}

فراغ gemm64f(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#جے_تعریف_شدہ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#جے_انت
}

فراغ gemm32fc(ثابت عائم* src1, size_t src1_قدم, ثابت عائم* src2, size_t src2_قدم,
              عائم alpha, ثابت عائم* src3, size_t src3_قدم, عائم beta, عائم* dst, size_t dst_قدم,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#جے_تعریف_شدہ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#جے_انت
}

فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_قدم, ثابت مزدوج* src2, size_t src2_قدم,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_قدم, مزدوج beta, مزدوج* dst, size_t dst_قدم,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#جے_تعریف_شدہ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#جے_انت
}

} // namespace hal

فراغ gemm(داخلہ_Array matA, داخلہ_Array matB, مزدوج alpha,
          داخلہ_Array matC, مزدوج beta, OutputArray _matD, صحيح flags)
{
#جے_تعریف_شدہ HAVE_CLAMDBLAS
    CV_OCL_لگاتار(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.خانے_گنتی() > 20 && matA.صفاں() > 20 && matB.خانے_گنتی() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#جے_انت

#جے_تعریف_شدہ HAVE_OPENCL
    CV_OCL_لگاتار(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#جے_انت

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    ناپ a_ناپ = A.size(), d_ناپ;
    صحيح len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    بدل( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالة 0:
        d_ناپ = ناپ( B.خانے_گنتی, a_ناپ.اچائی );
        len = B.صفاں;
        CV_Assert( a_ناپ.چوڑائی == len );
        اكسر;
    حالة 1:
        d_ناپ = ناپ( B.خانے_گنتی, a_ناپ.چوڑائی );
        len = B.صفاں;
        CV_Assert( a_ناپ.اچائی == len );
        اكسر;
    حالة 2:
        d_ناپ = ناپ( B.صفاں, a_ناپ.اچائی );
        len = B.خانے_گنتی;
        CV_Assert( a_ناپ.چوڑائی == len );
        اكسر;
    حالة 3:
        d_ناپ = ناپ( B.صفاں, a_ناپ.چوڑائی );
        len = B.خانے_گنتی;
        CV_Assert( a_ناپ.اچائی == len );
        اكسر;
    }

    إذا( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.صفاں == d_ناپ.اچائی && C.خانے_گنتی == d_ناپ.چوڑائی) ||
             ((flags&GEMM_3_T) != 0 && C.صفاں == d_ناپ.چوڑائی && C.خانے_گنتی == d_ناپ.اچائی)));
    }

    _matD.create( d_ناپ.اچائی, d_ناپ.چوڑائی, type );
    Mat D = _matD.getMat();
    إذا( (flags & GEMM_3_T) != 0 && C.انکڑے == D.انکڑے )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    إذا( D.انکڑے == A.انکڑے || D.انکڑے == B.انکڑے )
    {
        DProxy = Mat(d_ناپ.اچائی, d_ناپ.چوڑائی, D.type());
        DProxyPtr = &DProxy;
    }

    إذا( type == CV_32FC1 )
        hal::gemm32f(A.ptr<عائم>(), A.قدم, B.ptr<عائم>(), B.قدم, static_cast<عائم>(alpha),
                     C.ptr<عائم>(), C.قدم, static_cast<عائم>(beta),
                     DProxyPtr->ptr<عائم>(), DProxyPtr->قدم,
                     a_ناپ.اچائی, a_ناپ.چوڑائی, DProxyPtr->خانے_گنتی, flags);
    وإلا إذا( type == CV_64FC1 )
        hal::gemm64f(A.ptr<مزدوج>(), A.قدم, B.ptr<مزدوج>(), B.قدم, alpha,
                     C.ptr<مزدوج>(), C.قدم, beta,
                     DProxyPtr->ptr<مزدوج>(), DProxyPtr->قدم,
                     a_ناپ.اچائی, a_ناپ.چوڑائی, DProxyPtr->خانے_گنتی, flags);
    وإلا إذا( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<عائم>(), A.قدم, B.ptr<عائم>(), B.قدم, static_cast<عائم>(alpha),
                      C.ptr<عائم>(), C.قدم, static_cast<عائم>(beta),
                      DProxyPtr->ptr<عائم>(), DProxyPtr->قدم,
                      a_ناپ.اچائی, a_ناپ.چوڑائی, DProxyPtr->خانے_گنتی, flags);
    وإلا
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<مزدوج>(), A.قدم, B.ptr<مزدوج>(), B.قدم, alpha,
                      C.ptr<مزدوج>(), C.قدم, beta,
                      DProxyPtr->ptr<مزدوج>(), DProxyPtr->قدم,
                      a_ناپ.اچائی, a_ناپ.چوڑائی, DProxyPtr->خانے_گنتی, flags);
    }

    إذا(DProxyPtr != &D)
        DProxyPtr->copy_منزل(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ساكن TransformFunc getTransformFunc(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

ساكن TransformFunc getDiagTransformFunc(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ transform(داخلہ_Array _src, OutputArray _dst, داخلہ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح ڈونگھائی = src.ڈونگھائی(), scn = src.channels(), dcn = m.صفاں;
    CV_Assert( scn == m.خانے_گنتی || scn + 1 == m.خانے_گنتی );
    منطقي isDiag = خطأ;

    _dst.create_Same_ناپ( src, CV_MAKETYPE(ڈونگھائی, dcn) );
    Mat dst = _dst.getMat();

    إذا (src.انکڑے == dst.انکڑے)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    صحيح mtype = ڈونگھائی == CV_32S || ڈونگھائی == CV_64F ? CV_64F : CV_32F;
    Auto_بفر<مزدوج> _mbuf;
    مزدوج* mbuf;

    إذا( !m.isContinuous() || m.type() != mtype || m.خانے_گنتی != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.انکڑے();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.کل()*tmp.elem_ناپ());
        إذا( m.خانے_گنتی == scn+1 )
            m.convert_منزل(tmp, mtype);
        وإلا
        {
            Mat tmppart = tmp.خانہ_Range_2(0, m.خانے_گنتی);
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

    TransformFunc func = isDiag ? getDiagTransformFunc(ڈونگھائی): getTransformFunc(ڈونگھائی);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, کل = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)کل, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ساكن TransformFunc getPerspectiveTransform(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ perspectiveTransform(داخلہ_Array _src, OutputArray _dst, داخلہ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح ڈونگھائی = src.ڈونگھائی(), scn = src.channels(), dcn = m.صفاں-1;
    CV_Assert( scn + 1 == m.خانے_گنتی );
    CV_Assert( ڈونگھائی == CV_32F || ڈونگھائی == CV_64F );

    _dst.create_Same_ناپ( src, CV_MAKETYPE(ڈونگھائی, dcn) );
    Mat dst = _dst.getMat();

    ثابت صحيح mtype = CV_64F;
    Auto_بفر<مزدوج> _mbuf;
    مزدوج* mbuf = m.ptr<مزدوج>();

    إذا( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.انکڑے();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convert_منزل(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(ڈونگھائی);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, کل = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)کل, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#جے_تعریف_شدہ HAVE_OPENCL

ساكن منطقي ocl_scale_جوڑو( داخلہ_Array _src1, مزدوج alpha, داخلہ_Array _src2, OutputArray _dst, صحيح type )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    منطقي doubleSupport = d.doubleFPConfig() > 0;
    ناپ size = _src1.size();
    صحيح ڈونگھائی = CV_MAT_ڈونگھائی(type);
    إذا ( (!doubleSupport && ڈونگھائی == CV_64F) || size != _src2.size() )
        أرجع خطأ;

    _dst.create(size, type);
    صحيح cn = CV_MAT_CN(type), wdepth = std::max(ڈونگھائی, CV_32F);
    صحيح kercn = ocl::predict_Optimal_سمتیہ_چوڑائی_Max(_src1, _src2, _dst),
        صفاں_فی_WI = d.isIntel() ? 4 : 1;

    حرف cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, kercn)), ڈونگھائی,
                         ocl::type_منزل_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(ڈونگھائی, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, ڈونگھائی, kercn, cvt[1], حجم(cvt[1])),
                         ocl::type_منزل_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", صفاں_فی_WI));
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ناپ(src1),
            src2arg = ocl::KernelArg::Read_Only_No_ناپ(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    إذا (wdepth == CV_32F)
        k.دلیلاں(src1arg, src2arg, dstarg, (عائم)alpha);
    وإلا
        k.دلیلاں(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.خانے_گنتی * cn / kercn, ((size_t)dst.صفاں + صفاں_فی_WI - 1) / صفاں_فی_WI };
    أرجع k.لگاتار(2, globalsize, عدم, خطأ);
}

#جے_انت

ساكن Scale_جوڑو_Func get_Scale_جوڑو_Func(صحيح ڈونگھائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_جوڑو_Func, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ scale_جوڑو(داخلہ_Array _src1, مزدوج alpha, داخلہ_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src1.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_لگاتار(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_جوڑو(_src1, alpha, _src2, _dst, type))

    إذا( ڈونگھائی != CV_32F && ڈونگھائی != CV_64F )
    {
        جوڑو_Weighted(_src1, alpha, _src2, 1, 0, _dst, ڈونگھائی);
        أرجع;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    عائم falpha = (عائم)alpha;
    فراغ* palpha = ڈونگھائی == CV_32F ? (فراغ*)&falpha : (فراغ*)&alpha;

    Scale_جوڑو_Func func = get_Scale_جوڑو_Func(ڈونگھائی);
    CV_Assert(func);

    إذا (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.کل()*cn;
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

فراغ calc_Covar_میٹرکس( ثابت Mat* انکڑے, صحيح nsamples, Mat& covar, Mat& اوسط, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( انکڑے, nsamples > 0 );
    ناپ size = انکڑے[0].size();
    صحيح sz = size.چوڑائی * size.اچائی, esz = (صحيح)انکڑے[0].elem_ناپ();
    صحيح type = انکڑے[0].type();
    Mat اوسط_2;
    ctype = std::max(std::max(CV_MAT_ڈونگھائی(ctype >= 0 ? ctype : type), اوسط.ڈونگھائی()), CV_32F);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( اوسط.size() == size );
        إذا( اوسط.isContinuous() && اوسط.type() == ctype )
            اوسط_2 = اوسط.reshape(1, 1);
        وإلا
        {
            اوسط.convert_منزل(اوسط_2, ctype);
            اوسط_2 = اوسط_2.reshape(1, 1);
        }
    }

    Mat انکڑے_3(nsamples, sz, type);

    لكل( صحيح i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( انکڑے[i].size() == size, انکڑے[i].type() == type );
        إذا( انکڑے[i].isContinuous() )
            حافظہ_نقل( انکڑے_3.ptr(i), انکڑے[i].ptr(), sz*esz );
        وإلا
        {
            Mat انکڑے_صف(size.اچائی, size.چوڑائی, type, انکڑے_3.ptr(i));
            انکڑے[i].copy_منزل(انکڑے_صف);
        }
    }

    calc_Covar_میٹرکس( انکڑے_3, covar, اوسط_2, (flags & ~(cv::COVAR_صفاں|cv::COVAR_خانے_گنتی)) | cv::COVAR_صفاں, ctype );
    إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        اوسط = اوسط_2.reshape(1, size.اچائی);
}

فراغ calc_Covar_میٹرکس( داخلہ_Array _src, OutputArray _covar, داخلہ_Output_Array اوسط, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    إذا(_src.قسم() == داخلہ_Array_2::STD_سمتیہ_MAT || _src.قسم() == داخلہ_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_سمتیہ(src);

        CV_Assert( src.size() > 0 );

        ناپ size = src[0].size();
        صحيح type = src[0].type();

        ctype = std::max(std::max(CV_MAT_ڈونگھائی(ctype >= 0 ? ctype : type), اوسط.ڈونگھائی()), CV_32F);

        Mat انکڑے_3(static_cast<صحيح>(src.size()), size.area(), type);

        صحيح i = 0;
        لكل(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat انکڑے_صف(size.اچائی, size.چوڑائی, type, انکڑے_3.ptr(i));
            (*each).copy_منزل(انکڑے_صف);
        }

        Mat اوسط_2;
        إذا( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( اوسط.size() == size );
            Mat mean0 = اوسط.getMat();

            إذا( mean0.isContinuous() && mean0.type() == ctype )
                اوسط_2 = mean0.reshape(1, 1);
            وإلا
            {
                mean0.convert_منزل(اوسط_2, ctype);
                اوسط_2 = اوسط_2.reshape(1, 1);
            }
        }

        calc_Covar_میٹرکس( انکڑے_3, _covar, اوسط_2, (flags & ~(cv::COVAR_صفاں|cv::COVAR_خانے_گنتی)) | cv::COVAR_صفاں, ctype );

        إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            اوسط_2 = اوسط_2.reshape(1, size.اچائی);
            اوسط_2.copy_منزل(اوسط);
        }
        أرجع;
    }

    Mat انکڑے = _src.getMat(), اوسط_2;
    CV_Assert( ((flags & cv::COVAR_صفاں) != 0) ^ ((flags & cv::COVAR_خانے_گنتی) != 0) );
    منطقي take_صفاں = (flags & cv::COVAR_صفاں) != 0;
    صحيح type = انکڑے.type();
    صحيح nsamples = take_صفاں ? انکڑے.صفاں : انکڑے.خانے_گنتی;
    CV_Assert( nsamples > 0 );
    ناپ size = take_صفاں ? ناپ(انکڑے.خانے_گنتی, 1) : ناپ(1, انکڑے.صفاں);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        اوسط_2 = اوسط.getMat();
        ctype = std::max(std::max(CV_MAT_ڈونگھائی(ctype >= 0 ? ctype : type), اوسط_2.ڈونگھائی()), CV_32F);
        CV_Assert( اوسط_2.size() == size );
        إذا( اوسط_2.type() != ctype )
        {
            اوسط.create(اوسط_2.size(), ctype);
            Mat tmp = اوسط.getMat();
            اوسط_2.convert_منزل(tmp, ctype);
            اوسط_2 = tmp;
        }
    }
    وإلا
    {
        ctype = std::max(CV_MAT_ڈونگھائی(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, اوسط, take_صفاں ? 0 : 1, REDUCE_AVG, ctype );
        اوسط_2 = اوسط.getMat();
    }

    mulTransposed( انکڑے, _covar, ((flags & cv::COVAR_عام) == 0) ^ take_صفاں,
        اوسط_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ساكن MahalanobisImplFunc getMahalanobisImplFunc(صحيح ڈونگھائی)
{
#جے_تعریف_شدہ CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (ڈونگھائی));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (ڈونگھائی),
        CV_CPU_DISPATCH_MODES_ALL);
#جے_انت
}


مزدوج Mahalanobis(داخلہ_Array _v1, داخلہ_Array _v2, داخلہ_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    صحيح type = v1.type(), ڈونگھائی = v1.ڈونگھائی();
    ناپ sz = v1.size();
    صحيح len = sz.چوڑائی*sz.اچائی*v1.channels();
    Auto_بفر<مزدوج> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.صفاں && len == icovar.خانے_گنتی );

    sz.چوڑائی *= v1.channels();
    إذا( v1.isContinuous() && v2.isContinuous() )
    {
        sz.چوڑائی *= sz.اچائی;
        sz.اچائی = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(ڈونگھائی);
    CV_Assert(func);

    مزدوج نتیجہ = func(v1, v2, icovar, buf.انکڑے(), len);
    أرجع std::جذر(نتیجہ);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ساكن MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata)
{
#جے_تعریف_شدہ CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#جے_انت
}

فراغ mulTransposed(داخلہ_Array _src, OutputArray _dst, منطقي ata,
                   داخلہ_Array _delta, مزدوج scale, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ثابت صحيح gemm_level = 100; // boundary above which GEMM is faster.
    صحيح stype = src.type();
    dtype = std::max(std::max(CV_MAT_ڈونگھائی(dtype >= 0 ? dtype : stype), delta.ڈونگھائی()), CV_32F);
    CV_Assert( src.channels() == 1 );

    إذا( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.صفاں == src.صفاں || delta.صفاں == 1),
            (delta.خانے_گنتی == src.خانے_گنتی || delta.خانے_گنتی == 1));
        إذا( delta.type() != dtype )
            delta.convert_منزل(delta, dtype);
    }

    صحيح dsize = ata ? src.خانے_گنتی : src.صفاں;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    إذا( src.انکڑے == dst.انکڑے || (stype == dtype &&
        (dst.خانے_گنتی >= gemm_level && dst.صفاں >= gemm_level &&
         src.خانے_گنتی >= gemm_level && src.صفاں >= gemm_level)))
    {
        Mat src2;
        ثابت Mat* tsrc = &src;
        إذا( !delta.empty() )
        {
            إذا( delta.size() == src.size() )
                گھٹاؤ( src, delta, src2 );
            وإلا
            {
                repeat(delta, src.صفاں/delta.صفاں, src.خانے_گنتی/delta.خانے_گنتی, src2);
                گھٹاؤ( src, src2, src2 );
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
    CV_CPU_DISPATCH(dotProd_16s, ((ثابت چھوٹا*)src1, (ثابت چھوٹا*)src2, len),
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

ساكن DotProdFunc getDotProdFunc(صحيح ڈونگھائی)
{
    ساكن DotProdFunc dotProdTab[CV_ڈونگھائی_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    أرجع dotProdTab[ڈونگھائی];
}

مزدوج Mat::dot(داخلہ_Array _mat) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    صحيح cn = channels();
    إذا (هذا->dims <= 2)
    {
        مزدوج ضرب_حاصل = 0;
        CALL_HAL_RET(dot_ضرب_حاصل, cv_hal_dot_ضرب_حاصل, ضرب_حاصل, هذا->انکڑے, هذا->قدم, mat.انکڑے, mat.قدم,
                     هذا->خانے_گنتی * cn, هذا->صفاں, هذا->ڈونگھائی());
    }

    DotProdFunc func = getDotProdFunc(ڈونگھائی());
    CV_Assert(func != 0 );

    إذا( isContinuous() && mat.isContinuous() )
    {
        size_t len = کل()*cn;
        إذا( len == (size_t)(صحيح)len )
            أرجع func(انکڑے, mat.انکڑے, (صحيح)len);
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


#جے_تعریف_شدہ HAVE_OPENCL

ساكن منطقي ocl_dot( داخلہ_Array _src1, داخلہ_Array _src2, مزدوج & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    صحيح type = src1.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type),
            kercn = ocl::predict_Optimal_سمتیہ_چوڑائی(src1, src2);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا ( !doubleSupport && ڈونگھائی == CV_64F )
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروہ_ناپ();
    صحيح ddepth = std::max(CV_32F, ڈونگھائی);

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    حرف cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, kercn)), ocl::type_منزل_Str(ڈونگھائی),
                         ocl::type_منزل_Str(ddepth), ocl::type_منزل_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(ڈونگھائی, ddepth, kercn, cvt, حجم(cvt)),
                         (صحيح)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_ناپ(src1),
            src2arg = ocl::KernelArg::Read_Only_No_ناپ(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.دلیلاں(src1arg, src1.خانے_گنتی, (صحيح)src1.کل(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    إذا (k.لگاتار(1, &globalsize, &wgs, سچ))
    {
        res = جوڑ(db.getMat(ACCESS_READ))[0];
        أرجع سچ;
    }
    أرجع خطأ;
}

#جے_انت

مزدوج UMat::dot(داخلہ_Array m) ثابت
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_ناپ(*هذا) && m.type() == type());

#جے_تعریف_شدہ HAVE_OPENCL
    مزدوج r = 0;
    CV_OCL_لگاتار_2(dims <= 2, ocl_dot(*هذا, m, r), r)
#جے_انت

    أرجع getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
