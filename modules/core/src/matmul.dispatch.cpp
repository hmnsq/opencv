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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_core.hpp"
#अन्तर्भाव "intel_gpu_gemm.inl.hpp"

#अन्तर्भाव "matmul.simd.hpp"
#अन्तर्भाव "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#यदि_परिभाषितम् HAVE_CLAMDBLAS

स्थिर तार्किक ocl_gemm_amdblas( निवेशः_Array matA, निवेशः_Array matB, द्विगुण alpha,
                      निवेशः_Array matC, द्विगुण beta, OutputArray matD, पूर्णाङ्क flags )
{
    पूर्णाङ्क type = matA.type(), esz = CV_ELEM_आकार(type);
    तार्किक haveC = matC.विधा() != cv::निवेशः_Array_2::NONE;
    आकार आकार_A = matA.size(), आकार_B = matB.size(), आकार_C = haveC ? matC.size() : आकार(0, 0);
    तार्किक atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    यदि (atrans)
        आकार_A = आकार(आकार_A.height, आकार_A.विस्तारः);
    यदि (btrans)
        आकार_B = आकार(आकार_B.height, आकार_B.विस्तारः);
    यदि (haveC && ctrans)
        आकार_C = आकार(आकार_C.height, आकार_C.विस्तारः);

    आकार आकार_D(आकार_B.विस्तारः, आकार_A.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( आकार_A.विस्तारः == आकार_B.height && (!haveC || आकार_C == आकार_D) );

    matD.create(आकार_D, type);
    यदि ( matA.offset() % esz != 0 || matA.चरणः() % esz != 0 ||
         matB.offset() % esz != 0 || matB.चरणः() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.चरणः() % esz != 0)) )
        प्रत्यागम असत्य;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    यदि (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        प्रत्यागम असत्य;
    }
    यदि (haveC)
    {
        UMat C = matC.getUMat();
        यदि (!ocl::internal::isCLBuffer(C))
            प्रत्यागम असत्य;
    }
    यदि (haveC)
        ctrans ? transpose(matC, D) : matC.copy_गन्तव्यम्(D);
    अन्यथा
        D.समुच्चयः_गन्तव्यम्(Scalar::all(0));

    पूर्णाङ्क M = आकार_D.height, N = आकार_D.विस्तारः, K = आकार_A.विस्तारः;
    पूर्णाङ्क lda = (पूर्णाङ्क)A.चरणः / esz, ldb = (पूर्णाङ्क)B.चरणः / esz, ldc = (पूर्णाङ्क)D.चरणः / esz;
    पूर्णाङ्क offa = (पूर्णाङ्क)A.offset / esz, offb = (पूर्णाङ्क)B.offset / esz, offc = (पूर्णाङ्क)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblas_क्रमः क्रमः = clblasRowMajor;
    clblas_स्थितिः स्थितिः = clblasSuccess;

    यदि (type == CV_32FC1)
        स्थितिः = clblasSgemm(क्रमः, transA, transB, M, N, K,
                             (cl_float)alpha, (ध्रुव cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ध्रुव cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, रिक्त, रिक्त);
    अन्यथा यदि (type == CV_64FC1)
        स्थितिः = clblasDgemm(क्रमः, transA, transB, M, N, K,
                             alpha, (ध्रुव cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ध्रुव cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, रिक्त, रिक्त);
    अन्यथा यदि (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         स्थितिः = clblasCgemm(क्रमः, transA, transB, M, N, K,
                              alpha_2, (ध्रुव cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ध्रुव cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, रिक्त, रिक्त);
    }
    अन्यथा यदि (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        स्थितिः = clblasZgemm(क्रमः, transA, transB, M, N, K,
                             alpha_2, (ध्रुव cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ध्रुव cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, रिक्त, रिक्त);
    }
    अन्यथा
        CV_Error(Error::StsUnsupportedFormat, "");

    प्रत्यागम स्थितिः == clblasSuccess;
}

#यदि_अन्तः

#यदि_परिभाषितम् HAVE_OPENCL
स्थिर तार्किक ocl_gemm( निवेशः_Array matA, निवेशः_Array matB, द्विगुण alpha,
                      निवेशः_Array matC, द्विगुण beta, OutputArray matD, पूर्णाङ्क flags )
{
    पूर्णाङ्क type = matA.type();
    पूर्णाङ्क गभीरता = CV_MAT_गभीरता(type);
    पूर्णाङ्क cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ध्रुव ocl::Device & dev = ocl::Device::getDefault();
    तार्किक doubleSupport = dev.doubleFPConfig() > 0;

    यदि (!doubleSupport && गभीरता == CV_64F)
        प्रत्यागम असत्य;

    तार्किक haveC = matC.विधा() != cv::निवेशः_Array_2::NONE;
    आकार आकार_A = matA.size(), आकार_B = matB.size(), आकार_C = haveC ? matC.size() : आकार(0, 0);
    तार्किक atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    यदि (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    आकार आकार_D(((btrans) ? आकार_B.height : आकार_B.विस्तारः),
               ((atrans) ? आकार_A.विस्तारः : आकार_A.height));

    यदि (atrans)
        आकार_A = आकार(आकार_A.height, आकार_A.विस्तारः);
    यदि (btrans)
        आकार_B = आकार(आकार_B.height, आकार_B.विस्तारः);
    यदि (haveC && ctrans)
        आकार_C = आकार(आकार_C.height, आकार_C.विस्तारः);

    CV_CheckEQ(आकार_A.विस्तारः, आकार_B.height, "");
    यदि (haveC)
        CV_CheckEQ(आकार_C, आकार_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(आकार_D, type);
    UMat D = matD.getUMat();

    तार्किक isPropagatedC2D = असत्य;  // D content is updated with C / C.t()

    यदि (dev.intelSubgroupsSupport() && (गभीरता == CV_32F) && cn == 1)
    {
        यदि (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copy_गन्तव्यम्(D);
            isPropagatedC2D = सत्य;
        }
        अन्यथा
        {
            beta = 0.0;
        }

        तार्किक res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, आकार_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        यदि (res)
            प्रत्यागम सत्य;
        // fallback on generic OpenCL code
    }

    यदि (आकार_D.विस्तारः < 8 || आकार_D.height < 8)
        प्रत्यागम असत्य;

    String opts;

    पूर्णाङ्क wg_आकार = (पूर्णाङ्क)dev.max_Work_गणः_आकार();
    पूर्णाङ्क आकार_Dmin = std::min(आकार_D.विस्तारः, आकार_D.height);
    wg_आकार = std::min(wg_आकार, आकार_Dmin * आकार_Dmin);
    पूर्णाङ्क खण्डम्_आकार_3 = (wg_आकार / (32*cn) < 32) ? (wg_आकार / (16*cn) < 16) ? (wg_आकार / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    यदि (atrans)
        A = A.t();

    यदि (btrans)
        B = B.t();

    यदि (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copy_गन्तव्यम्(D);

    पूर्णाङ्क सदिशम्_Widths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    पूर्णाङ्क kercn = ocl::check_Optimal_सदिशम्_विस्तारः(सदिशम्_Widths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::type_गन्तव्यम्_Str(type), ocl::type_गन्तव्यम्_Str(गभीरता), ocl::type_गन्तव्यम्_Str(CV_MAKETYPE(गभीरता, kercn)),
                      cn, kercn, खण्डम्_आकार_3,
                      (आकार_A.विस्तारः % खण्डम्_आकार_3 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    यदि (k.empty())
        प्रत्यागम असत्य;

    यदि (गभीरता == CV_64F)
        k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(A),
               ocl::KernelArg::Read_Only_No_आकार(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               आकार_A.विस्तारः, alpha, beta);
    अन्यथा
        k.तर्काः(ocl::KernelArg::Read_Only_No_आकार(A),
               ocl::KernelArg::Read_Only_No_आकार(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               आकार_A.विस्तारः, (भिन्न)alpha, (भिन्न)beta);

    size_t globalsize[2] = { (size_t)आकार_D.विस्तारः * cn / kercn, (size_t)आकार_D.height};
    size_t localsize[2] = { (size_t)खण्डम्_आकार_3, (size_t)खण्डम्_आकार_3};

    प्रत्यागम k.सततम्(2, globalsize, खण्डम्_आकार_3 !=1 ? localsize : रिक्त, असत्य);
}
#यदि_अन्तः


नामाकाश hal {

शून्य gemm32f(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
             भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
             पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags)
#यदि_परिभाषितम् CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags));
#अन्यथा
    CV_CPU_DISPATCH(gemm32f, (src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#यदि_अन्तः
}

शून्य gemm64f(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
             द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
             पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags)
#यदि_परिभाषितम् CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags));
#अन्यथा
    CV_CPU_DISPATCH(gemm64f, (src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#यदि_अन्तः
}

शून्य gemm32fc(ध्रुव भिन्न* src1, size_t src1_चरणः, ध्रुव भिन्न* src2, size_t src2_चरणः,
              भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_चरणः, भिन्न beta, भिन्न* dst, size_t dst_चरणः,
              पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags)
#यदि_परिभाषितम् CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags));
#अन्यथा
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#यदि_अन्तः
}

शून्य gemm64fc(ध्रुव द्विगुण* src1, size_t src1_चरणः, ध्रुव द्विगुण* src2, size_t src2_चरणः,
              द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_चरणः, द्विगुण beta, द्विगुण* dst, size_t dst_चरणः,
              पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags)
#यदि_परिभाषितम् CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags));
#अन्यथा
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_चरणः, src2, src2_चरणः, alpha, src3, src3_चरणः, beta, dst, dst_चरणः, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#यदि_अन्तः
}

} // namespace hal

शून्य gemm(निवेशः_Array matA, निवेशः_Array matB, द्विगुण alpha,
          निवेशः_Array matC, द्विगुण beta, OutputArray _matD, पूर्णाङ्क flags)
{
#यदि_परिभाषितम् HAVE_CLAMDBLAS
    CV_OCL_सततम्(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.स्तम्भसङ्ख्या() > 20 && matA.पङ्क्तिकाः() > 20 && matB.स्तम्भसङ्ख्या() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_OPENCL
    CV_OCL_सततम्(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#यदि_अन्तः

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    आकार a_आकार = A.size(), d_आकार;
    पूर्णाङ्क len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    विकल्प( flags & (GEMM_1_T|GEMM_2_T) )
    {
    पक्ष 0:
        d_आकार = आकार( B.स्तम्भसङ्ख्या, a_आकार.height );
        len = B.पङ्क्तिकाः;
        CV_Assert( a_आकार.विस्तारः == len );
        भङ्ग;
    पक्ष 1:
        d_आकार = आकार( B.स्तम्भसङ्ख्या, a_आकार.विस्तारः );
        len = B.पङ्क्तिकाः;
        CV_Assert( a_आकार.height == len );
        भङ्ग;
    पक्ष 2:
        d_आकार = आकार( B.पङ्क्तिकाः, a_आकार.height );
        len = B.स्तम्भसङ्ख्या;
        CV_Assert( a_आकार.विस्तारः == len );
        भङ्ग;
    पक्ष 3:
        d_आकार = आकार( B.पङ्क्तिकाः, a_आकार.विस्तारः );
        len = B.स्तम्भसङ्ख्या;
        CV_Assert( a_आकार.height == len );
        भङ्ग;
    }

    यदि( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.पङ्क्तिकाः == d_आकार.height && C.स्तम्भसङ्ख्या == d_आकार.विस्तारः) ||
             ((flags&GEMM_3_T) != 0 && C.पङ्क्तिकाः == d_आकार.विस्तारः && C.स्तम्भसङ्ख्या == d_आकार.height)));
    }

    _matD.create( d_आकार.height, d_आकार.विस्तारः, type );
    Mat D = _matD.getMat();
    यदि( (flags & GEMM_3_T) != 0 && C.दत्त == D.दत्त )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    यदि( D.दत्त == A.दत्त || D.दत्त == B.दत्त )
    {
        DProxy = Mat(d_आकार.height, d_आकार.विस्तारः, D.type());
        DProxyPtr = &DProxy;
    }

    यदि( type == CV_32FC1 )
        hal::gemm32f(A.ptr<भिन्न>(), A.चरणः, B.ptr<भिन्न>(), B.चरणः, static_cast<भिन्न>(alpha),
                     C.ptr<भिन्न>(), C.चरणः, static_cast<भिन्न>(beta),
                     DProxyPtr->ptr<भिन्न>(), DProxyPtr->चरणः,
                     a_आकार.height, a_आकार.विस्तारः, DProxyPtr->स्तम्भसङ्ख्या, flags);
    अन्यथा यदि( type == CV_64FC1 )
        hal::gemm64f(A.ptr<द्विगुण>(), A.चरणः, B.ptr<द्विगुण>(), B.चरणः, alpha,
                     C.ptr<द्विगुण>(), C.चरणः, beta,
                     DProxyPtr->ptr<द्विगुण>(), DProxyPtr->चरणः,
                     a_आकार.height, a_आकार.विस्तारः, DProxyPtr->स्तम्भसङ्ख्या, flags);
    अन्यथा यदि( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<भिन्न>(), A.चरणः, B.ptr<भिन्न>(), B.चरणः, static_cast<भिन्न>(alpha),
                      C.ptr<भिन्न>(), C.चरणः, static_cast<भिन्न>(beta),
                      DProxyPtr->ptr<भिन्न>(), DProxyPtr->चरणः,
                      a_आकार.height, a_आकार.विस्तारः, DProxyPtr->स्तम्भसङ्ख्या, flags);
    अन्यथा
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<द्विगुण>(), A.चरणः, B.ptr<द्विगुण>(), B.चरणः, alpha,
                      C.ptr<द्विगुण>(), C.चरणः, beta,
                      DProxyPtr->ptr<द्विगुण>(), DProxyPtr->चरणः,
                      a_आकार.height, a_आकार.विस्तारः, DProxyPtr->स्तम्भसङ्ख्या, flags);
    }

    यदि(DProxyPtr != &D)
        DProxyPtr->copy_गन्तव्यम्(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

स्थिर TransformFunc getTransformFunc(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
}

स्थिर TransformFunc getDiagTransformFunc(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य transform(निवेशः_Array _src, OutputArray _dst, निवेशः_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    पूर्णाङ्क गभीरता = src.गभीरता(), scn = src.channels(), dcn = m.पङ्क्तिकाः;
    CV_Assert( scn == m.स्तम्भसङ्ख्या || scn + 1 == m.स्तम्भसङ्ख्या );
    तार्किक isDiag = असत्य;

    _dst.create_Same_आकार( src, CV_MAKETYPE(गभीरता, dcn) );
    Mat dst = _dst.getMat();

    यदि (src.दत्त == dst.दत्त)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    पूर्णाङ्क mtype = गभीरता == CV_32S || गभीरता == CV_64F ? CV_64F : CV_32F;
    Auto_बफर<द्विगुण> _mbuf;
    द्विगुण* mbuf;

    यदि( !m.isContinuous() || m.type() != mtype || m.स्तम्भसङ्ख्या != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.दत्त();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        स्मृतिं_पूरय(tmp.ptr(), 0, tmp.योग()*tmp.elem_आकार());
        यदि( m.स्तम्भसङ्ख्या == scn+1 )
            m.convert_गन्तव्यम्(tmp, mtype);
        अन्यथा
        {
            Mat tmppart = tmp.colRange(0, m.स्तम्भसङ्ख्या);
            m.convert_गन्तव्यम्(tmppart, mtype);
        }
        m = tmp;
    }
    अन्यथा
        mbuf = m.ptr<द्विगुण>();

    यदि( scn == dcn )
    {
        पूर्णाङ्क i, j;
        द्विगुण eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        यदि( scn == 1 )
        {
            द्विगुण alpha, beta;
            यदि( mtype == CV_32F )
                alpha = m.at<भिन्न>(0), beta = m.at<भिन्न>(1);
            अन्यथा
                alpha = m.at<द्विगुण>(0), beta = m.at<द्विगुण>(1);
            src.convert_गन्तव्यम्(dst, dst.type(), alpha, beta);
            प्रत्यागम;
        }

        प्रति( i = 0, isDiag = सत्य; isDiag && i < scn; i++ )
        {
            प्रति( j = 0; isDiag && j < scn; j++ )
            {
                द्विगुण v = mtype == CV_32F ? m.at<भिन्न>(i, j) : m.at<द्विगुण>(i, j);
                यदि( i != j && fabs(v) > eps )
                    isDiag = असत्य;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(गभीरता): getTransformFunc(गभीरता);
    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, योग = it.size;

    प्रति( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (पूर्णाङ्क)योग, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

स्थिर TransformFunc getPerspectiveTransform(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य perspectiveTransform(निवेशः_Array _src, OutputArray _dst, निवेशः_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    पूर्णाङ्क गभीरता = src.गभीरता(), scn = src.channels(), dcn = m.पङ्क्तिकाः-1;
    CV_Assert( scn + 1 == m.स्तम्भसङ्ख्या );
    CV_Assert( गभीरता == CV_32F || गभीरता == CV_64F );

    _dst.create_Same_आकार( src, CV_MAKETYPE(गभीरता, dcn) );
    Mat dst = _dst.getMat();

    ध्रुव पूर्णाङ्क mtype = CV_64F;
    Auto_बफर<द्विगुण> _mbuf;
    द्विगुण* mbuf = m.ptr<द्विगुण>();

    यदि( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.दत्त();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convert_गन्तव्यम्(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(गभीरता);
    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, योग = it.size;

    प्रति( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (पूर्णाङ्क)योग, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#यदि_परिभाषितम् HAVE_OPENCL

स्थिर तार्किक ocl_scale_सङ्कलय( निवेशः_Array _src1, द्विगुण alpha, निवेशः_Array _src2, OutputArray _dst, पूर्णाङ्क type )
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();

    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    आकार size = _src1.size();
    पूर्णाङ्क गभीरता = CV_MAT_गभीरता(type);
    यदि ( (!doubleSupport && गभीरता == CV_64F) || size != _src2.size() )
        प्रत्यागम असत्य;

    _dst.create(size, type);
    पूर्णाङ्क cn = CV_MAT_CN(type), wdepth = std::max(गभीरता, CV_32F);
    पूर्णाङ्क kercn = ocl::predict_Optimal_सदिशम्_विस्तारः_Max(_src1, _src2, _dst),
        पङ्क्तिकाः_प्रत्येकम्_WI = d.isIntel() ? 4 : 1;

    वर्ण cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, kercn)), गभीरता,
                         ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(गभीरता, wdepth, kercn, cvt[0], परिमाण(cvt[0])),
                         ocl::convertTypeStr(wdepth, गभीरता, kercn, cvt[1], परिमाण(cvt[1])),
                         ocl::type_गन्तव्यम्_Str(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", पङ्क्तिकाः_प्रत्येकम्_WI));
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_आकार(src1),
            src2arg = ocl::KernelArg::Read_Only_No_आकार(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    यदि (wdepth == CV_32F)
        k.तर्काः(src1arg, src2arg, dstarg, (भिन्न)alpha);
    अन्यथा
        k.तर्काः(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.स्तम्भसङ्ख्या * cn / kercn, ((size_t)dst.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

#यदि_अन्तः

स्थिर Scale_सङ्कलय_Func get_Scale_सङ्कलय_Func(पूर्णाङ्क गभीरता)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_Scale_सङ्कलय_Func, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य scale_सङ्कलय(निवेशः_Array _src1, द्विगुण alpha, निवेशः_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = _src1.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_सततम्(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scale_सङ्कलय(_src1, alpha, _src2, _dst, type))

    यदि( गभीरता != CV_32F && गभीरता != CV_64F )
    {
        सङ्कलय_Weighted(_src1, alpha, _src2, 1, 0, _dst, गभीरता);
        प्रत्यागम;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    भिन्न falpha = (भिन्न)alpha;
    शून्य* palpha = गभीरता == CV_32F ? (शून्य*)&falpha : (शून्य*)&alpha;

    Scale_सङ्कलय_Func func = get_Scale_सङ्कलय_Func(गभीरता);
    CV_Assert(func);

    यदि (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.योग()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (पूर्णाङ्क)len, palpha);
        प्रत्यागम;
    }

    ध्रुव Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    प्रति( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (पूर्णाङ्क)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

शून्य calcCovarMatrix( ध्रुव Mat* दत्त, पूर्णाङ्क nsamples, Mat& covar, Mat& माध्यम्, पूर्णाङ्क flags, पूर्णाङ्क ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( दत्त, nsamples > 0 );
    आकार size = दत्त[0].size();
    पूर्णाङ्क sz = size.विस्तारः * size.height, esz = (पूर्णाङ्क)दत्त[0].elem_आकार();
    पूर्णाङ्क type = दत्त[0].type();
    Mat माध्यम्_2;
    ctype = std::max(std::max(CV_MAT_गभीरता(ctype >= 0 ? ctype : type), माध्यम्.गभीरता()), CV_32F);

    यदि( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( माध्यम्.size() == size );
        यदि( माध्यम्.isContinuous() && माध्यम्.type() == ctype )
            माध्यम्_2 = माध्यम्.reshape(1, 1);
        अन्यथा
        {
            माध्यम्.convert_गन्तव्यम्(माध्यम्_2, ctype);
            माध्यम्_2 = माध्यम्_2.reshape(1, 1);
        }
    }

    Mat दत्त_3(nsamples, sz, type);

    प्रति( पूर्णाङ्क i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( दत्त[i].size() == size, दत्त[i].type() == type );
        यदि( दत्त[i].isContinuous() )
            स्मृतिं_प्रतिलिख( दत्त_3.ptr(i), दत्त[i].ptr(), sz*esz );
        अन्यथा
        {
            Mat दत्त_Row(size.height, size.विस्तारः, type, दत्त_3.ptr(i));
            दत्त[i].copy_गन्तव्यम्(दत्त_Row);
        }
    }

    calcCovarMatrix( दत्त_3, covar, माध्यम्_2, (flags & ~(cv::COVAR_पङ्क्तिकाः|cv::COVAR_स्तम्भसङ्ख्या)) | cv::COVAR_पङ्क्तिकाः, ctype );
    यदि( (flags & cv::COVAR_USE_AVG) == 0 )
        माध्यम् = माध्यम्_2.reshape(1, size.height);
}

शून्य calcCovarMatrix( निवेशः_Array _src, OutputArray _covar, निवेशः_Output_Array माध्यम्, पूर्णाङ्क flags, पूर्णाङ्क ctype )
{
    CV_INSTRUMENT_REGION();

    यदि(_src.विधा() == निवेशः_Array_2::STD_सदिशम्_MAT || _src.विधा() == निवेशः_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.get_Mat_सदिशम्(src);

        CV_Assert( src.size() > 0 );

        आकार size = src[0].size();
        पूर्णाङ्क type = src[0].type();

        ctype = std::max(std::max(CV_MAT_गभीरता(ctype >= 0 ? ctype : type), माध्यम्.गभीरता()), CV_32F);

        Mat दत्त_3(static_cast<पूर्णाङ्क>(src.size()), size.area(), type);

        पूर्णाङ्क i = 0;
        प्रति(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat दत्त_Row(size.height, size.विस्तारः, type, दत्त_3.ptr(i));
            (*each).copy_गन्तव्यम्(दत्त_Row);
        }

        Mat माध्यम्_2;
        यदि( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( माध्यम्.size() == size );
            Mat mean0 = माध्यम्.getMat();

            यदि( mean0.isContinuous() && mean0.type() == ctype )
                माध्यम्_2 = mean0.reshape(1, 1);
            अन्यथा
            {
                mean0.convert_गन्तव्यम्(माध्यम्_2, ctype);
                माध्यम्_2 = माध्यम्_2.reshape(1, 1);
            }
        }

        calcCovarMatrix( दत्त_3, _covar, माध्यम्_2, (flags & ~(cv::COVAR_पङ्क्तिकाः|cv::COVAR_स्तम्भसङ्ख्या)) | cv::COVAR_पङ्क्तिकाः, ctype );

        यदि( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            माध्यम्_2 = माध्यम्_2.reshape(1, size.height);
            माध्यम्_2.copy_गन्तव्यम्(माध्यम्);
        }
        प्रत्यागम;
    }

    Mat दत्त = _src.getMat(), माध्यम्_2;
    CV_Assert( ((flags & cv::COVAR_पङ्क्तिकाः) != 0) ^ ((flags & cv::COVAR_स्तम्भसङ्ख्या) != 0) );
    तार्किक take_पङ्क्तिकाः = (flags & cv::COVAR_पङ्क्तिकाः) != 0;
    पूर्णाङ्क type = दत्त.type();
    पूर्णाङ्क nsamples = take_पङ्क्तिकाः ? दत्त.पङ्क्तिकाः : दत्त.स्तम्भसङ्ख्या;
    CV_Assert( nsamples > 0 );
    आकार size = take_पङ्क्तिकाः ? आकार(दत्त.स्तम्भसङ्ख्या, 1) : आकार(1, दत्त.पङ्क्तिकाः);

    यदि( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        माध्यम्_2 = माध्यम्.getMat();
        ctype = std::max(std::max(CV_MAT_गभीरता(ctype >= 0 ? ctype : type), माध्यम्_2.गभीरता()), CV_32F);
        CV_Assert( माध्यम्_2.size() == size );
        यदि( माध्यम्_2.type() != ctype )
        {
            माध्यम्.create(माध्यम्_2.size(), ctype);
            Mat tmp = माध्यम्.getMat();
            माध्यम्_2.convert_गन्तव्यम्(tmp, ctype);
            माध्यम्_2 = tmp;
        }
    }
    अन्यथा
    {
        ctype = std::max(CV_MAT_गभीरता(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, माध्यम्, take_पङ्क्तिकाः ? 0 : 1, REDUCE_AVG, ctype );
        माध्यम्_2 = माध्यम्.getMat();
    }

    mulTransposed( दत्त, _covar, ((flags & cv::COVAR_प्रसामान्यम्) == 0) ^ take_पङ्क्तिकाः,
        माध्यम्_2, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

स्थिर MahalanobisImplFunc getMahalanobisImplFunc(पूर्णाङ्क गभीरता)
{
#यदि_परिभाषितम् CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (गभीरता));
#अन्यथा
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (गभीरता),
        CV_CPU_DISPATCH_MODES_ALL);
#यदि_अन्तः
}


द्विगुण Mahalanobis(निवेशः_Array _v1, निवेशः_Array _v2, निवेशः_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    पूर्णाङ्क type = v1.type(), गभीरता = v1.गभीरता();
    आकार sz = v1.size();
    पूर्णाङ्क len = sz.विस्तारः*sz.height*v1.channels();
    Auto_बफर<द्विगुण> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.पङ्क्तिकाः && len == icovar.स्तम्भसङ्ख्या );

    sz.विस्तारः *= v1.channels();
    यदि( v1.isContinuous() && v2.isContinuous() )
    {
        sz.विस्तारः *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(गभीरता);
    CV_Assert(func);

    द्विगुण परिणाम = func(v1, v2, icovar, buf.दत्त(), len);
    प्रत्यागम std::वर्गमूलम्(परिणाम);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

स्थिर MulTransposedFunc getMulTransposedFunc(पूर्णाङ्क stype, पूर्णाङ्क dtype, तार्किक ata)
{
#यदि_परिभाषितम् CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#अन्यथा
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#यदि_अन्तः
}

शून्य mulTransposed(निवेशः_Array _src, OutputArray _dst, तार्किक ata,
                   निवेशः_Array _delta, द्विगुण scale, पूर्णाङ्क dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ध्रुव पूर्णाङ्क gemm_level = 100; // boundary above which GEMM is faster.
    पूर्णाङ्क stype = src.type();
    dtype = std::max(std::max(CV_MAT_गभीरता(dtype >= 0 ? dtype : stype), delta.गभीरता()), CV_32F);
    CV_Assert( src.channels() == 1 );

    यदि( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.पङ्क्तिकाः == src.पङ्क्तिकाः || delta.पङ्क्तिकाः == 1),
            (delta.स्तम्भसङ्ख्या == src.स्तम्भसङ्ख्या || delta.स्तम्भसङ्ख्या == 1));
        यदि( delta.type() != dtype )
            delta.convert_गन्तव्यम्(delta, dtype);
    }

    पूर्णाङ्क dsize = ata ? src.स्तम्भसङ्ख्या : src.पङ्क्तिकाः;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    यदि( src.दत्त == dst.दत्त || (stype == dtype &&
        (dst.स्तम्भसङ्ख्या >= gemm_level && dst.पङ्क्तिकाः >= gemm_level &&
         src.स्तम्भसङ्ख्या >= gemm_level && src.पङ्क्तिकाः >= gemm_level)))
    {
        Mat src2;
        ध्रुव Mat* tsrc = &src;
        यदि( !delta.empty() )
        {
            यदि( delta.size() == src.size() )
                व्यवकलय( src, delta, src2 );
            अन्यथा
            {
                repeat(delta, src.पङ्क्तिकाः/delta.पङ्क्तिकाः, src.स्तम्भसङ्ख्या/delta.स्तम्भसङ्ख्या, src2);
                व्यवकलय( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    अन्यथा
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        यदि( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, असत्य );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

स्थिर द्विगुण dotProd_8u(ध्रुव शून्य* src1, ध्रुव शून्य* src2, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ध्रुव uchar*)src1, (ध्रुव uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर द्विगुण dotProd_8s(ध्रुव शून्य* src1, ध्रुव शून्य* src2, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ध्रुव schar*)src1, (ध्रुव schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर द्विगुण dotProd_16u(ध्रुव शून्य* src1, ध्रुव शून्य* src2, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ध्रुव ushort*)src1, (ध्रुव ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर द्विगुण dotProd_16s(ध्रुव शून्य* src1, ध्रुव शून्य* src2, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ध्रुव ह्रस्व*)src1, (ध्रुव ह्रस्व*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर द्विगुण dotProd_32s(ध्रुव शून्य* src1, ध्रुव शून्य* src2, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ध्रुव पूर्णाङ्क*)src1, (ध्रुव पूर्णाङ्क*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर द्विगुण dotProd_32f(ध्रुव शून्य* src1, ध्रुव शून्य* src2, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ध्रुव भिन्न*)src1, (ध्रुव भिन्न*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर द्विगुण dotProd_64f(ध्रुव शून्य* src1, ध्रुव शून्य* src2, पूर्णाङ्क len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ध्रुव द्विगुण*)src1, (ध्रुव द्विगुण*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

प्रकारनिर्देश द्विगुण (*DotProdFunc)(ध्रुव शून्य* src1, ध्रुव शून्य* src2, पूर्णाङ्क len);

स्थिर DotProdFunc getDotProdFunc(पूर्णाङ्क गभीरता)
{
    स्थिर DotProdFunc dotProdTab[CV_गभीरता_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    प्रत्यागम dotProdTab[गभीरता];
}

द्विगुण Mat::dot(निवेशः_Array _mat) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    पूर्णाङ्क cn = channels();
    यदि (एतत्->dims <= 2)
    {
        द्विगुण गुणनफलम् = 0;
        CALL_HAL_RET(dot_गुणनफलम्, cv_hal_dot_गुणनफलम्, गुणनफलम्, एतत्->दत्त, एतत्->चरणः, mat.दत्त, mat.चरणः,
                     एतत्->स्तम्भसङ्ख्या * cn, एतत्->पङ्क्तिकाः, एतत्->गभीरता());
    }

    DotProdFunc func = getDotProdFunc(गभीरता());
    CV_Assert(func != 0 );

    यदि( isContinuous() && mat.isContinuous() )
    {
        size_t len = योग()*cn;
        यदि( len == (size_t)(पूर्णाङ्क)len )
            प्रत्यागम func(दत्त, mat.दत्त, (पूर्णाङ्क)len);
    }

    ध्रुव Mat* arrays[] = {एतत्, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क len = (पूर्णाङ्क)(it.size*cn);
    द्विगुण r = 0;

    प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    प्रत्यागम r;
}


#यदि_परिभाषितम् HAVE_OPENCL

स्थिर तार्किक ocl_dot( निवेशः_Array _src1, निवेशः_Array _src2, द्विगुण & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    पूर्णाङ्क type = src1.type(), गभीरता = CV_MAT_गभीरता(type),
            kercn = ocl::predict_Optimal_सदिशम्_विस्तारः(src1, src2);
    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    यदि ( !doubleSupport && गभीरता == CV_64F )
        प्रत्यागम असत्य;

    पूर्णाङ्क dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_गणः_आकार();
    पूर्णाङ्क ddepth = std::max(CV_32F, गभीरता);

    पूर्णाङ्क wgs2_aligned = 1;
    यावत् (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    वर्ण cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, kercn)), ocl::type_गन्तव्यम्_Str(गभीरता),
                         ocl::type_गन्तव्यम्_Str(ddepth), ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(गभीरता, ddepth, kercn, cvt, परिमाण(cvt)),
                         (पूर्णाङ्क)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_आकार(src1),
            src2arg = ocl::KernelArg::Read_Only_No_आकार(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.तर्काः(src1arg, src1.स्तम्भसङ्ख्या, (पूर्णाङ्क)src1.योग(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    यदि (k.सततम्(1, &globalsize, &wgs, सत्य))
    {
        res = योग_2(db.getMat(ACCESS_READ))[0];
        प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

#यदि_अन्तः

द्विगुण UMat::dot(निवेशः_Array m) ध्रुव
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_आकार(*एतत्) && m.type() == type());

#यदि_परिभाषितम् HAVE_OPENCL
    द्विगुण r = 0;
    CV_OCL_सततम्_2(dims <= 2, ocl_dot(*एतत्, m, r), r)
#यदि_अन्तः

    प्रत्यागम getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
