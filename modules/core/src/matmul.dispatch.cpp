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

#شامل "precomp.hpp"
#شامل <opencv2/core/utils/logger.hpp>

#شامل "opencl_kernels_core.hpp"
#شامل "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#شامل "opencv2/core/opencl/runtime/opencl_core.hpp"
#شامل "intel_gpu_gemm.inl.hpp"

#شامل "matmul.simd.hpp"
#شامل "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#اگر_تعریف_شدہ HAVE_CLAMDBLAS

ساکن منطقی ocl_gemm_amdblas( داخلہ_Array matA, داخلہ_Array matB, دوہرا alpha,
                      داخلہ_Array matC, دوہرا beta, OutputArray matD, صحیح flags )
{
    صحیح type = matA.type(), esz = CV_ELEM_سائز(type);
    منطقی haveC = matC.قسم() != cv::داخلہ_Array_2::NONE;
    سائز سائز_A = matA.size(), سائز_B = matB.size(), سائز_C = haveC ? matC.size() : سائز(0, 0);
    منطقی atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    اگر (atrans)
        سائز_A = سائز(سائز_A.اونچائی, سائز_A.چوڑائی);
    اگر (btrans)
        سائز_B = سائز(سائز_B.اونچائی, سائز_B.چوڑائی);
    اگر (haveC && ctrans)
        سائز_C = سائز(سائز_C.اونچائی, سائز_C.چوڑائی);

    سائز سائز_D(سائز_B.چوڑائی, سائز_A.اونچائی);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( سائز_A.چوڑائی == سائز_B.اونچائی && (!haveC || سائز_C == سائز_D) );

    matD.create(سائز_D, type);
    اگر ( matA.offset() % esz != 0 || matA.قدم() % esz != 0 ||
         matB.offset() % esz != 0 || matB.قدم() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.قدم() % esz != 0)) )
        واپس جھوٹ;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    اگر (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        واپس جھوٹ;
    }
    اگر (haveC)
    {
        UMat C = matC.getUMat();
        اگر (!ocl::internal::isCLBuffer(C))
            واپس جھوٹ;
    }
    اگر (haveC)
        ctrans ? transpose(matC, D) : matC.copy_مقصد(D);
    ورنہ
        D.سیٹ_مقصد(Scalar::all(0));

    صحیح M = سائز_D.اونچائی, N = سائز_D.چوڑائی, K = سائز_A.چوڑائی;
    صحیح lda = (صحیح)A.قدم / esz, ldb = (صحیح)B.قدم / esz, ldc = (صحیح)D.قدم / esz;
    صحیح offa = (صحیح)A.offset / esz, offb = (صحیح)B.offset / esz, offc = (صحیح)D.offset / esz;

    cl_command_قطار clq = (cl_command_قطار)ocl::قطار::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_ترتیب ترتیب = clblas_صف_Major;
    clblas_حالت حالت_2 = clblasSuccess;

    اگر (type == CV_32FC1)
        حالت_2 = clblasSgemm(ترتیب, transA, transB, M, N, K,
                             (cl_float)alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, تہی, تہی);
    ورنہ اگر (type == CV_64FC1)
        حالت_2 = clblasDgemm(ترتیب, transA, transB, M, N, K,
                             alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, تہی, تہی);
    ورنہ اگر (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         حالت_2 = clblasCgemm(ترتیب, transA, transB, M, N, K,
                              alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, تہی, تہی);
    }
    ورنہ اگر (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        حالت_2 = clblasZgemm(ترتیب, transA, transB, M, N, K,
                             alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, تہی, تہی);
    }
    ورنہ
        CV_Error(Error::StsUnsupportedFormat, "");

    واپس حالت_2 == clblasSuccess;
}

#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENCL
ساکن منطقی ocl_gemm( داخلہ_Array matA, داخلہ_Array matB, دوہرا alpha,
                      داخلہ_Array matC, دوہرا beta, OutputArray matD, صحیح flags )
{
    صحیح type = matA.type();
    صحیح گہرائی = CV_MAT_گہرائی(type);
    صحیح cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقی doubleSupport = dev.doubleFPConfig() > 0;

    اگر (!doubleSupport && گہرائی == CV_64F)
        واپس جھوٹ;

    منطقی haveC = matC.قسم() != cv::داخلہ_Array_2::NONE;
    سائز سائز_A = matA.size(), سائز_B = matB.size(), سائز_C = haveC ? matC.size() : سائز(0, 0);
    منطقی atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    اگر (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    سائز سائز_D(((btrans) ? سائز_B.اونچائی : سائز_B.چوڑائی),
               ((atrans) ? سائز_A.چوڑائی : سائز_A.اونچائی));

    اگر (atrans)
        سائز_A = سائز(سائز_A.اونچائی, سائز_A.چوڑائی);
    اگر (btrans)
        سائز_B = سائز(سائز_B.اونچائی, سائز_B.چوڑائی);
    اگر (haveC && ctrans)
        سائز_C = سائز(سائز_C.اونچائی, سائز_C.چوڑائی);

    CV_CheckEQ(سائز_A.چوڑائی, سائز_B.اونچائی, "");
    اگر (haveC)
        CV_CheckEQ(سائز_C, سائز_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(سائز_D, type);
    UMat D = matD.getUMat();

    منطقی isPropagatedC2D = جھوٹ;  // D content is updated with C / C.t()

    اگر (dev.intelSubgroupsSupport() && (گہرائی == CV_32F) && cn == 1)
    {
        اگر (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copy_مقصد(D);
            isPropagatedC2D = سچ;
        }
        ورنہ
        {
            beta = 0.0;
        }

        منطقی res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, سائز_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        اگر (res)
            واپس سچ;
        // fallback on generic OpenCL code
    }

    اگر (سائز_D.چوڑائی < 8 || سائز_D.اونچائی < 8)
        واپس جھوٹ;

    String opts;

    صحیح wg_سائز = (صحیح)dev.max_Work_گروہ_سائز();
    صحیح سائز_Dmin = std::min(سائز_D.چوڑائی, سائز_D.اونچائی);
    wg_سائز = std::min(wg_سائز, سائز_Dmin * سائز_Dmin);
    صحیح بلاک_سائز_3 = (wg_سائز / (32*cn) < 32) ? (wg_سائز / (16*cn) < 16) ? (wg_سائز / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    اگر (atrans)
        A = A.t();

    اگر (btrans)
        B = B.t();

    اگر (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_مقصد(D);

    صحیح سمتیہ_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    صحیح kercn = ocl::check_Optimal_سمتیہ_چوڑائی(سمتیہ_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_مقصد_Str(type), ocl::type_مقصد_Str(گہرائی), ocl::type_مقصد_Str(CV_MAKETYPE(گہرائی, kercn)),
                      cn, kercn, بلاک_سائز_3,
                      (سائز_A.چوڑائی % بلاک_سائز_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    اگر (k.empty())
        واپس جھوٹ;

    اگر (گہرائی == CV_64F)
        k.دلائل(ocl::KernelArg::Read_Only_No_سائز(A),
               ocl::KernelArg::Read_Only_No_سائز(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               سائز_A.چوڑائی, alpha, beta);
    ورنہ
        k.دلائل(ocl::KernelArg::Read_Only_No_سائز(A),
               ocl::KernelArg::Read_Only_No_سائز(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               سائز_A.چوڑائی, (اعشاری)alpha, (اعشاری)beta);

    size_t globalsize[2] = { (size_t)سائز_D.چوڑائی * cn / kercn, (size_t)سائز_D.اونچائی};
    size_t localsize[2] = { (size_t)بلاک_سائز_3, (size_t)بلاک_سائز_3};

    واپس k.تسلسل(2, globalsize, بلاک_سائز_3 !=1 ? localsize : تہی, جھوٹ);
}
#اگر_ختم


نامخلا hal {

خالی gemm32f(ثابت اعشاری* src1, size_t src1_قدم, ثابت اعشاری* src2, size_t src2_قدم,
             اعشاری alpha, ثابت اعشاری* src3, size_t src3_قدم, اعشاری beta, اعشاری* dst, size_t dst_قدم,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#اگر_تعریف_شدہ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#ورنہ
    CV_CPU_DISPATCH(gemm32f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#اگر_ختم
}

خالی gemm64f(ثابت دوہرا* src1, size_t src1_قدم, ثابت دوہرا* src2, size_t src2_قدم,
             دوہرا alpha, ثابت دوہرا* src3, size_t src3_قدم, دوہرا beta, دوہرا* dst, size_t dst_قدم,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#اگر_تعریف_شدہ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#ورنہ
    CV_CPU_DISPATCH(gemm64f, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#اگر_ختم
}

خالی gemm32fc(ثابت اعشاری* src1, size_t src1_قدم, ثابت اعشاری* src2, size_t src2_قدم,
              اعشاری alpha, ثابت اعشاری* src3, size_t src3_قدم, اعشاری beta, اعشاری* dst, size_t dst_قدم,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#اگر_تعریف_شدہ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#ورنہ
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#اگر_ختم
}

خالی gemm64fc(ثابت دوہرا* src1, size_t src1_قدم, ثابت دوہرا* src2, size_t src2_قدم,
              دوہرا alpha, ثابت دوہرا* src3, size_t src3_قدم, دوہرا beta, دوہرا* dst, size_t dst_قدم,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags)
#اگر_تعریف_شدہ CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags));
#ورنہ
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_قدم, src2, src2_قدم, alpha, src3, src3_قدم, beta, dst, dst_قدم, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#اگر_ختم
}

} // namespace hal

خالی gemm(داخلہ_Array matA, داخلہ_Array matB, دوہرا alpha,
          داخلہ_Array matC, دوہرا beta, OutputArray _matD, صحیح flags)
{
#اگر_تعریف_شدہ HAVE_CLAMDBLAS
    CV_OCL_تسلسل(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.ستون_تعداد() > 20 && matA.صفیں() > 20 && matB.ستون_تعداد() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENCL
    CV_OCL_تسلسل(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#اگر_ختم

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    سائز a_سائز = A.size(), d_سائز;
    صحیح len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    انتخاب( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالت 0:
        d_سائز = سائز( B.ستون_تعداد, a_سائز.اونچائی );
        len = B.صفیں;
        CV_Assert( a_سائز.چوڑائی == len );
        توڑو;
    حالت 1:
        d_سائز = سائز( B.ستون_تعداد, a_سائز.چوڑائی );
        len = B.صفیں;
        CV_Assert( a_سائز.اونچائی == len );
        توڑو;
    حالت 2:
        d_سائز = سائز( B.صفیں, a_سائز.اونچائی );
        len = B.ستون_تعداد;
        CV_Assert( a_سائز.چوڑائی == len );
        توڑو;
    حالت 3:
        d_سائز = سائز( B.صفیں, a_سائز.چوڑائی );
        len = B.ستون_تعداد;
        CV_Assert( a_سائز.اونچائی == len );
        توڑو;
    }

    اگر( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.صفیں == d_سائز.اونچائی && C.ستون_تعداد == d_سائز.چوڑائی) ||
             ((flags&GEMM_3_T) != 0 && C.صفیں == d_سائز.چوڑائی && C.ستون_تعداد == d_سائز.اونچائی)));
    }

    _matD.create( d_سائز.اونچائی, d_سائز.چوڑائی, type );
    Mat D = _matD.getMat();
    اگر( (flags & GEMM_3_T) != 0 && C.ڈیٹا == D.ڈیٹا )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    اگر( D.ڈیٹا == A.ڈیٹا || D.ڈیٹا == B.ڈیٹا )
    {
        DProxy = Mat(d_سائز.اونچائی, d_سائز.چوڑائی, D.type());
        DProxyPtr = &DProxy;
    }

    اگر( type == CV_32FC1 )
        hal::gemm32f(A.ptr<اعشاری>(), A.قدم, B.ptr<اعشاری>(), B.قدم, static_cast<اعشاری>(alpha),
                     C.ptr<اعشاری>(), C.قدم, static_cast<اعشاری>(beta),
                     DProxyPtr->ptr<اعشاری>(), DProxyPtr->قدم,
                     a_سائز.اونچائی, a_سائز.چوڑائی, DProxyPtr->ستون_تعداد, flags);
    ورنہ اگر( type == CV_64FC1 )
        hal::gemm64f(A.ptr<دوہرا>(), A.قدم, B.ptr<دوہرا>(), B.قدم, alpha,
                     C.ptr<دوہرا>(), C.قدم, beta,
                     DProxyPtr->ptr<دوہرا>(), DProxyPtr->قدم,
                     a_سائز.اونچائی, a_سائز.چوڑائی, DProxyPtr->ستون_تعداد, flags);
    ورنہ اگر( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<اعشاری>(), A.قدم, B.ptr<اعشاری>(), B.قدم, static_cast<اعشاری>(alpha),
                      C.ptr<اعشاری>(), C.قدم, static_cast<اعشاری>(beta),
                      DProxyPtr->ptr<اعشاری>(), DProxyPtr->قدم,
                      a_سائز.اونچائی, a_سائز.چوڑائی, DProxyPtr->ستون_تعداد, flags);
    ورنہ
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<دوہرا>(), A.قدم, B.ptr<دوہرا>(), B.قدم, alpha,
                      C.ptr<دوہرا>(), C.قدم, beta,
                      DProxyPtr->ptr<دوہرا>(), DProxyPtr->قدم,
                      a_سائز.اونچائی, a_سائز.چوڑائی, DProxyPtr->ستون_تعداد, flags);
    }

    اگر(DProxyPtr != &D)
        DProxyPtr->copy_مقصد(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ساکن TransformFunc getTransformFunc(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

ساکن TransformFunc getDiagTransformFunc(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی transform(داخلہ_Array _src, OutputArray _dst, داخلہ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحیح گہرائی = src.گہرائی(), scn = src.channels(), dcn = m.صفیں;
    CV_Assert( scn == m.ستون_تعداد || scn + 1 == m.ستون_تعداد );
    منطقی isDiag = جھوٹ;

    _dst.create_Same_سائز( src, CV_MAKETYPE(گہرائی, dcn) );
    Mat dst = _dst.getMat();

    اگر (src.ڈیٹا == dst.ڈیٹا)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    صحیح mtype = گہرائی == CV_32S || گہرائی == CV_64F ? CV_64F : CV_32F;
    Auto_بفر<دوہرا> _mbuf;
    دوہرا* mbuf;

    اگر( !m.isContinuous() || m.type() != mtype || m.ستون_تعداد != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.ڈیٹا();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.کل()*tmp.elem_سائز());
        اگر( m.ستون_تعداد == scn+1 )
            m.convert_مقصد(tmp, mtype);
        ورنہ
        {
            Mat tmppart = tmp.ستون_Range_2(0, m.ستون_تعداد);
            m.convert_مقصد(tmppart, mtype);
        }
        m = tmp;
    }
    ورنہ
        mbuf = m.ptr<دوہرا>();

    اگر( scn == dcn )
    {
        صحیح i, j;
        دوہرا eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        اگر( scn == 1 )
        {
            دوہرا alpha, beta;
            اگر( mtype == CV_32F )
                alpha = m.at<اعشاری>(0), beta = m.at<اعشاری>(1);
            ورنہ
                alpha = m.at<دوہرا>(0), beta = m.at<دوہرا>(1);
            src.convert_مقصد(dst, dst.type(), alpha, beta);
            واپس;
        }

        برائے( i = 0, isDiag = سچ; isDiag && i < scn; i++ )
        {
            برائے( j = 0; isDiag && j < scn; j++ )
            {
                دوہرا v = mtype == CV_32F ? m.at<اعشاری>(i, j) : m.at<دوہرا>(i, j);
                اگر( i != j && fabs(v) > eps )
                    isDiag = جھوٹ;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(گہرائی): getTransformFunc(گہرائی);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, کل = it.size;

    برائے( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحیح)کل, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ساکن TransformFunc getPerspectiveTransform(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی perspectiveTransform(داخلہ_Array _src, OutputArray _dst, داخلہ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحیح گہرائی = src.گہرائی(), scn = src.channels(), dcn = m.صفیں-1;
    CV_Assert( scn + 1 == m.ستون_تعداد );
    CV_Assert( گہرائی == CV_32F || گہرائی == CV_64F );

    _dst.create_Same_سائز( src, CV_MAKETYPE(گہرائی, dcn) );
    Mat dst = _dst.getMat();

    ثابت صحیح mtype = CV_64F;
    Auto_بفر<دوہرا> _mbuf;
    دوہرا* mbuf = m.ptr<دوہرا>();

    اگر( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.ڈیٹا();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convert_مقصد(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(گہرائی);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, کل = it.size;

    برائے( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحیح)کل, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_scale_جوڑو( داخلہ_Array _src1, دوہرا alpha, داخلہ_Array _src2, OutputArray _dst, صحیح type )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    منطقی doubleSupport = d.doubleFPConfig() > 0;
    سائز size = _src1.size();
    صحیح گہرائی = CV_MAT_گہرائی(type);
    اگر ( (!doubleSupport && گہرائی == CV_64F) || size != _src2.size() )
        واپس جھوٹ;

    _dst.create(size, type);
    صحیح cn = CV_MAT_CN(type), wdepth = std::max(گہرائی, CV_32F);
    صحیح kercn = ocl::predict_Optimal_سمتیہ_چوڑائی_Max(_src1, _src2, _dst),
        صفیں_فی_WI = d.isIntel() ? 4 : 1;

    حرف cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)), گہرائی,
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(گہرائی, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, گہرائی, kercn, cvt[1], حجم(cvt[1])),
                         ocl::type_مقصد_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", صفیں_فی_WI));
    اگر (k.empty())
        واپس جھوٹ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_سائز(src1),
            src2arg = ocl::KernelArg::Read_Only_No_سائز(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    اگر (wdepth == CV_32F)
        k.دلائل(src1arg, src2arg, dstarg, (اعشاری)alpha);
    ورنہ
        k.دلائل(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.ستون_تعداد * cn / kercn, ((size_t)dst.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

ساکن Scale_جوڑو_Func get_Scale_جوڑو_Func(صحیح گہرائی)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_جوڑو_Func, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی scale_جوڑو(داخلہ_Array _src1, دوہرا alpha, داخلہ_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src1.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_تسلسل(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_جوڑو(_src1, alpha, _src2, _dst, type))

    اگر( گہرائی != CV_32F && گہرائی != CV_64F )
    {
        جوڑو_Weighted(_src1, alpha, _src2, 1, 0, _dst, گہرائی);
        واپس;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    اعشاری falpha = (اعشاری)alpha;
    خالی* palpha = گہرائی == CV_32F ? (خالی*)&falpha : (خالی*)&alpha;

    Scale_جوڑو_Func func = get_Scale_جوڑو_Func(گہرائی);
    CV_Assert(func);

    اگر (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.کل()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (صحیح)len, palpha);
        واپس;
    }

    ثابت Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    برائے( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (صحیح)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

خالی calc_Covar_میٹرکس( ثابت Mat* ڈیٹا, صحیح nsamples, Mat& covar, Mat& اوسط, صحیح flags, صحیح ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( ڈیٹا, nsamples > 0 );
    سائز size = ڈیٹا[0].size();
    صحیح sz = size.چوڑائی * size.اونچائی, esz = (صحیح)ڈیٹا[0].elem_سائز();
    صحیح type = ڈیٹا[0].type();
    Mat اوسط_2;
    ctype = std::max(std::max(CV_MAT_گہرائی(ctype >= 0 ? ctype : type), اوسط.گہرائی()), CV_32F);

    اگر( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( اوسط.size() == size );
        اگر( اوسط.isContinuous() && اوسط.type() == ctype )
            اوسط_2 = اوسط.reshape(1, 1);
        ورنہ
        {
            اوسط.convert_مقصد(اوسط_2, ctype);
            اوسط_2 = اوسط_2.reshape(1, 1);
        }
    }

    Mat ڈیٹا_3(nsamples, sz, type);

    برائے( صحیح i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( ڈیٹا[i].size() == size, ڈیٹا[i].type() == type );
        اگر( ڈیٹا[i].isContinuous() )
            حافظہ_نقل( ڈیٹا_3.ptr(i), ڈیٹا[i].ptr(), sz*esz );
        ورنہ
        {
            Mat ڈیٹا_صف(size.اونچائی, size.چوڑائی, type, ڈیٹا_3.ptr(i));
            ڈیٹا[i].copy_مقصد(ڈیٹا_صف);
        }
    }

    calc_Covar_میٹرکس( ڈیٹا_3, covar, اوسط_2, (flags & ~(cv::COVAR_صفیں|cv::COVAR_ستون_تعداد)) | cv::COVAR_صفیں, ctype );
    اگر( (flags & cv::COVAR_USE_AVG) == 0 )
        اوسط = اوسط_2.reshape(1, size.اونچائی);
}

خالی calc_Covar_میٹرکس( داخلہ_Array _src, OutputArray _covar, داخلہ_Output_Array اوسط, صحیح flags, صحیح ctype )
{
    CV_INSTRUMENT_REGION();

    اگر(_src.قسم() == داخلہ_Array_2::STD_سمتیہ_MAT || _src.قسم() == داخلہ_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_سمتیہ(src);

        CV_Assert( src.size() > 0 );

        سائز size = src[0].size();
        صحیح type = src[0].type();

        ctype = std::max(std::max(CV_MAT_گہرائی(ctype >= 0 ? ctype : type), اوسط.گہرائی()), CV_32F);

        Mat ڈیٹا_3(static_cast<صحیح>(src.size()), size.area(), type);

        صحیح i = 0;
        برائے(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat ڈیٹا_صف(size.اونچائی, size.چوڑائی, type, ڈیٹا_3.ptr(i));
            (*each).copy_مقصد(ڈیٹا_صف);
        }

        Mat اوسط_2;
        اگر( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( اوسط.size() == size );
            Mat mean0 = اوسط.getMat();

            اگر( mean0.isContinuous() && mean0.type() == ctype )
                اوسط_2 = mean0.reshape(1, 1);
            ورنہ
            {
                mean0.convert_مقصد(اوسط_2, ctype);
                اوسط_2 = اوسط_2.reshape(1, 1);
            }
        }

        calc_Covar_میٹرکس( ڈیٹا_3, _covar, اوسط_2, (flags & ~(cv::COVAR_صفیں|cv::COVAR_ستون_تعداد)) | cv::COVAR_صفیں, ctype );

        اگر( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            اوسط_2 = اوسط_2.reshape(1, size.اونچائی);
            اوسط_2.copy_مقصد(اوسط);
        }
        واپس;
    }

    Mat ڈیٹا = _src.getMat(), اوسط_2;
    CV_Assert( ((flags & cv::COVAR_صفیں) != 0) ^ ((flags & cv::COVAR_ستون_تعداد) != 0) );
    منطقی take_صفیں = (flags & cv::COVAR_صفیں) != 0;
    صحیح type = ڈیٹا.type();
    صحیح nsamples = take_صفیں ? ڈیٹا.صفیں : ڈیٹا.ستون_تعداد;
    CV_Assert( nsamples > 0 );
    سائز size = take_صفیں ? سائز(ڈیٹا.ستون_تعداد, 1) : سائز(1, ڈیٹا.صفیں);

    اگر( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        اوسط_2 = اوسط.getMat();
        ctype = std::max(std::max(CV_MAT_گہرائی(ctype >= 0 ? ctype : type), اوسط_2.گہرائی()), CV_32F);
        CV_Assert( اوسط_2.size() == size );
        اگر( اوسط_2.type() != ctype )
        {
            اوسط.create(اوسط_2.size(), ctype);
            Mat tmp = اوسط.getMat();
            اوسط_2.convert_مقصد(tmp, ctype);
            اوسط_2 = tmp;
        }
    }
    ورنہ
    {
        ctype = std::max(CV_MAT_گہرائی(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, اوسط, take_صفیں ? 0 : 1, REDUCE_AVG, ctype );
        اوسط_2 = اوسط.getMat();
    }

    mulTransposed( ڈیٹا, _covar, ((flags & cv::COVAR_معمول) == 0) ^ take_صفیں,
        اوسط_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

ساکن MahalanobisImplFunc getMahalanobisImplFunc(صحیح گہرائی)
{
#اگر_تعریف_شدہ CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (گہرائی));
#ورنہ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (گہرائی),
        CV_CPU_DISPATCH_MODES_ALL);
#اگر_ختم
}


دوہرا Mahalanobis(داخلہ_Array _v1, داخلہ_Array _v2, داخلہ_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    صحیح type = v1.type(), گہرائی = v1.گہرائی();
    سائز sz = v1.size();
    صحیح len = sz.چوڑائی*sz.اونچائی*v1.channels();
    Auto_بفر<دوہرا> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.صفیں && len == icovar.ستون_تعداد );

    sz.چوڑائی *= v1.channels();
    اگر( v1.isContinuous() && v2.isContinuous() )
    {
        sz.چوڑائی *= sz.اونچائی;
        sz.اونچائی = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(گہرائی);
    CV_Assert(func);

    دوہرا نتیجہ = func(v1, v2, icovar, buf.ڈیٹا(), len);
    واپس std::جذر(نتیجہ);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ساکن MulTransposedFunc getMulTransposedFunc(صحیح stype, صحیح dtype, منطقی ata)
{
#اگر_تعریف_شدہ CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#ورنہ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#اگر_ختم
}

خالی mulTransposed(داخلہ_Array _src, OutputArray _dst, منطقی ata,
                   داخلہ_Array _delta, دوہرا scale, صحیح dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ثابت صحیح gemm_level = 100; // boundary above which GEMM is faster.
    صحیح stype = src.type();
    dtype = std::max(std::max(CV_MAT_گہرائی(dtype >= 0 ? dtype : stype), delta.گہرائی()), CV_32F);
    CV_Assert( src.channels() == 1 );

    اگر( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.صفیں == src.صفیں || delta.صفیں == 1),
            (delta.ستون_تعداد == src.ستون_تعداد || delta.ستون_تعداد == 1));
        اگر( delta.type() != dtype )
            delta.convert_مقصد(delta, dtype);
    }

    صحیح dsize = ata ? src.ستون_تعداد : src.صفیں;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    اگر( src.ڈیٹا == dst.ڈیٹا || (stype == dtype &&
        (dst.ستون_تعداد >= gemm_level && dst.صفیں >= gemm_level &&
         src.ستون_تعداد >= gemm_level && src.صفیں >= gemm_level)))
    {
        Mat src2;
        ثابت Mat* tsrc = &src;
        اگر( !delta.empty() )
        {
            اگر( delta.size() == src.size() )
                تفریق( src, delta, src2 );
            ورنہ
            {
                repeat(delta, src.صفیں/delta.صفیں, src.ستون_تعداد/delta.ستون_تعداد, src2);
                تفریق( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    ورنہ
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        اگر( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, جھوٹ );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ساکن دوہرا dotProd_8u(ثابت خالی* src1, ثابت خالی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ثابت uchar*)src1, (ثابت uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساکن دوہرا dotProd_8s(ثابت خالی* src1, ثابت خالی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ثابت schar*)src1, (ثابت schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساکن دوہرا dotProd_16u(ثابت خالی* src1, ثابت خالی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ثابت ushort*)src1, (ثابت ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساکن دوہرا dotProd_16s(ثابت خالی* src1, ثابت خالی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ثابت مختصر*)src1, (ثابت مختصر*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساکن دوہرا dotProd_32s(ثابت خالی* src1, ثابت خالی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ثابت صحیح*)src1, (ثابت صحیح*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساکن دوہرا dotProd_32f(ثابت خالی* src1, ثابت خالی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ثابت اعشاری*)src1, (ثابت اعشاری*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ساکن دوہرا dotProd_64f(ثابت خالی* src1, ثابت خالی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ثابت دوہرا*)src1, (ثابت دوہرا*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

قسمتعریف دوہرا (*DotProdFunc)(ثابت خالی* src1, ثابت خالی* src2, صحیح len);

ساکن DotProdFunc getDotProdFunc(صحیح گہرائی)
{
    ساکن DotProdFunc dotProdTab[CV_گہرائی_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    واپس dotProdTab[گہرائی];
}

دوہرا Mat::dot(داخلہ_Array _mat) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    صحیح cn = channels();
    اگر (یہ->dims <= 2)
    {
        دوہرا حاصل_ضرب = 0;
        CALL_HAL_RET(dot_حاصل_ضرب, cv_hal_dot_حاصل_ضرب, حاصل_ضرب, یہ->ڈیٹا, یہ->قدم, mat.ڈیٹا, mat.قدم,
                     یہ->ستون_تعداد * cn, یہ->صفیں, یہ->گہرائی());
    }

    DotProdFunc func = getDotProdFunc(گہرائی());
    CV_Assert(func != 0 );

    اگر( isContinuous() && mat.isContinuous() )
    {
        size_t len = کل()*cn;
        اگر( len == (size_t)(صحیح)len )
            واپس func(ڈیٹا, mat.ڈیٹا, (صحیح)len);
    }

    ثابت Mat* arrays[] = {یہ, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح len = (صحیح)(it.size*cn);
    دوہرا r = 0;

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    واپس r;
}


#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_dot( داخلہ_Array _src1, داخلہ_Array _src2, دوہرا & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    صحیح type = src1.type(), گہرائی = CV_MAT_گہرائی(type),
            kercn = ocl::predict_Optimal_سمتیہ_چوڑائی(src1, src2);
    منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    اگر ( !doubleSupport && گہرائی == CV_64F )
        واپس جھوٹ;

    صحیح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_گروہ_سائز();
    صحیح ddepth = std::max(CV_32F, گہرائی);

    صحیح wgs2_aligned = 1;
    جب (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    حرف cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)), ocl::type_مقصد_Str(گہرائی),
                         ocl::type_مقصد_Str(ddepth), ocl::type_مقصد_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(گہرائی, ddepth, kercn, cvt, حجم(cvt)),
                         (صحیح)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    اگر (k.empty())
        واپس جھوٹ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_سائز(src1),
            src2arg = ocl::KernelArg::Read_Only_No_سائز(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.دلائل(src1arg, src1.ستون_تعداد, (صحیح)src1.کل(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    اگر (k.تسلسل(1, &globalsize, &wgs, سچ))
    {
        res = مجموع(db.getMat(ACCESS_READ))[0];
        واپس سچ;
    }
    واپس جھوٹ;
}

#اگر_ختم

دوہرا UMat::dot(داخلہ_Array m) ثابت
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_سائز(*یہ) && m.type() == type());

#اگر_تعریف_شدہ HAVE_OPENCL
    دوہرا r = 0;
    CV_OCL_تسلسل_2(dims <= 2, ocl_dot(*یہ, m, r), r)
#اگر_ختم

    واپس getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
