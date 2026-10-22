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

#समावेश "precomp.hpp"
#समावेश <opencv2/core/utils/logger.hpp>

#समावेश "opencl_kernels_core.hpp"
#समावेश "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#समावेश "opencv2/core/opencl/runtime/opencl_core.hpp"
#समावेश "intel_gpu_gemm.inl.hpp"

#समावेश "matmul.simd.hpp"
#समावेश "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#अगर_घोषित HAVE_CLAMDBLAS

अटल bool ocl_gemm_amdblas( निवेश_Array matA, निवेश_Array matB, द्विगुन alpha,
                      निवेश_Array matC, द्विगुन beta, OutputArray matD, पूर्णांक flags )
{
    पूर्णांक type = matA.type(), esz = CV_ELEM_आकार(type);
    bool haveC = matC.kind() != cv::निवेश_Array_2::NONE;
    आकार आकार_A = matA.size(), आकार_B = matB.size(), आकार_C = haveC ? matC.size() : आकार(0, 0);
    bool atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    अगर (atrans)
        आकार_A = आकार(आकार_A.height, आकार_A.width);
    अगर (btrans)
        आकार_B = आकार(आकार_B.height, आकार_B.width);
    अगर (haveC && ctrans)
        आकार_C = आकार(आकार_C.height, आकार_C.width);

    आकार आकार_D(आकार_B.width, आकार_A.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( आकार_A.width == आकार_B.height && (!haveC || आकार_C == आकार_D) );

    matD.create(आकार_D, type);
    अगर ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        वापस false;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    अगर (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        वापस false;
    }
    अगर (haveC)
    {
        UMat C = matC.getUMat();
        अगर (!ocl::internal::isCLBuffer(C))
            वापस false;
    }
    अगर (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    अन्यथा
        D.setTo(Scalar::all(0));

    पूर्णांक M = आकार_D.height, N = आकार_D.width, K = आकार_A.width;
    पूर्णांक lda = (पूर्णांक)A.step / esz, ldb = (पूर्णांक)B.step / esz, ldc = (पूर्णांक)D.step / esz;
    पूर्णांक offa = (पूर्णांक)A.offset / esz, offb = (पूर्णांक)B.offset / esz, offc = (पूर्णांक)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    अगर (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (स्थिर cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (स्थिर cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, शून्य, शून्य);
    अन्यथा अगर (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (स्थिर cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (स्थिर cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, शून्य, शून्य);
    अन्यथा अगर (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (स्थिर cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (स्थिर cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, शून्य, शून्य);
    }
    अन्यथा अगर (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (स्थिर cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (स्थिर cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, शून्य, शून्य);
    }
    अन्यथा
        CV_Error(Error::StsUnsupportedFormat, "");

    वापस status == clblasSuccess;
}

#पूर्ण_अगर

#अगर_घोषित HAVE_OPENCL
अटल bool ocl_gemm( निवेश_Array matA, निवेश_Array matB, द्विगुन alpha,
                      निवेश_Array matC, द्विगुन beta, OutputArray matD, पूर्णांक flags )
{
    पूर्णांक type = matA.type();
    पूर्णांक depth = CV_MAT_DEPTH(type);
    पूर्णांक cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    स्थिर ocl::Device & dev = ocl::Device::getDefault();
    bool doubleSupport = dev.doubleFPConfig() > 0;

    अगर (!doubleSupport && depth == CV_64F)
        वापस false;

    bool haveC = matC.kind() != cv::निवेश_Array_2::NONE;
    आकार आकार_A = matA.size(), आकार_B = matB.size(), आकार_C = haveC ? matC.size() : आकार(0, 0);
    bool atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    अगर (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    आकार आकार_D(((btrans) ? आकार_B.height : आकार_B.width),
               ((atrans) ? आकार_A.width : आकार_A.height));

    अगर (atrans)
        आकार_A = आकार(आकार_A.height, आकार_A.width);
    अगर (btrans)
        आकार_B = आकार(आकार_B.height, आकार_B.width);
    अगर (haveC && ctrans)
        आकार_C = आकार(आकार_C.height, आकार_C.width);

    CV_CheckEQ(आकार_A.width, आकार_B.height, "");
    अगर (haveC)
        CV_CheckEQ(आकार_C, आकार_D, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(आकार_D, type);
    UMat D = matD.getUMat();

    bool isPropagatedC2D = false;  // D content is updated with C / C.t()

    अगर (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        अगर (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = true;
        }
        अन्यथा
        {
            beta = 0.0;
        }

        bool res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, आकार_D,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        अगर (res)
            वापस true;
        // fallback on generic OpenCL code
    }

    अगर (आकार_D.width < 8 || आकार_D.height < 8)
        वापस false;

    String opts;

    पूर्णांक wg_आकार = (पूर्णांक)dev.max_Work_Group_आकार();
    पूर्णांक आकार_Dmin = std::min(आकार_D.width, आकार_D.height);
    wg_आकार = std::min(wg_आकार, आकार_Dmin * आकार_Dmin);
    पूर्णांक block_आकार_2 = (wg_आकार / (32*cn) < 32) ? (wg_आकार / (16*cn) < 16) ? (wg_आकार / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    अगर (atrans)
        A = A.t();

    अगर (btrans)
        B = B.t();

    अगर (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    पूर्णांक vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    पूर्णांक kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_आकार_2,
                      (आकार_A.width % block_आकार_2 !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    अगर (k.empty())
        वापस false;

    अगर (depth == CV_64F)
        k.args(ocl::KernelArg::Read_Only_No_आकार(A),
               ocl::KernelArg::Read_Only_No_आकार(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               आकार_A.width, alpha, beta);
    अन्यथा
        k.args(ocl::KernelArg::Read_Only_No_आकार(A),
               ocl::KernelArg::Read_Only_No_आकार(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               आकार_A.width, (भग्न)alpha, (भग्न)beta);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)आकार_D.width * cn / kercn, (माप_प्रकार)आकार_D.height};
    माप_प्रकार localsize[2] = { (माप_प्रकार)block_आकार_2, (माप_प्रकार)block_आकार_2};

    वापस k.run(2, globalsize, block_आकार_2 !=1 ? localsize : शून्य, false);
}
#पूर्ण_अगर


namespace hal {

व्योम gemm32f(स्थिर भग्न* src1, माप_प्रकार src1_step, स्थिर भग्न* src2, माप_प्रकार src2_step,
             भग्न alpha, स्थिर भग्न* src3, माप_प्रकार src3_step, भग्न beta, भग्न* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#अगर_घोषित CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#अन्यथा
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#पूर्ण_अगर
}

व्योम gemm64f(स्थिर द्विगुन* src1, माप_प्रकार src1_step, स्थिर द्विगुन* src2, माप_प्रकार src2_step,
             द्विगुन alpha, स्थिर द्विगुन* src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन* dst, माप_प्रकार dst_step,
             पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#अगर_घोषित CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#अन्यथा
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#पूर्ण_अगर
}

व्योम gemm32fc(स्थिर भग्न* src1, माप_प्रकार src1_step, स्थिर भग्न* src2, माप_प्रकार src2_step,
              भग्न alpha, स्थिर भग्न* src3, माप_प्रकार src3_step, भग्न beta, भग्न* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#अगर_घोषित CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#अन्यथा
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#पूर्ण_अगर
}

व्योम gemm64fc(स्थिर द्विगुन* src1, माप_प्रकार src1_step, स्थिर द्विगुन* src2, माप_प्रकार src2_step,
              द्विगुन alpha, स्थिर द्विगुन* src3, माप_प्रकार src3_step, द्विगुन beta, द्विगुन* dst, माप_प्रकार dst_step,
              पूर्णांक m_a, पूर्णांक n_a, पूर्णांक n_d, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#अगर_घोषित CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#अन्यथा
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#पूर्ण_अगर
}

} // namespace hal

व्योम gemm(निवेश_Array matA, निवेश_Array matB, द्विगुन alpha,
          निवेश_Array matC, द्विगुन beta, OutputArray _matD, पूर्णांक flags)
{
#अगर_घोषित HAVE_CLAMDBLAS
    CV_OCL_RUN(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA.cols() > 20 && matA.rows() > 20 && matB.cols() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENCL
    CV_OCL_RUN(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#पूर्ण_अगर

    Mat A = matA.getMat(), B = matB.getMat(), C = beta != 0.0 ? matC.getMat() : Mat();
    आकार a_आकार = A.size(), d_आकार;
    पूर्णांक len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    चयन( flags & (GEMM_1_T|GEMM_2_T) )
    {
    हाल 0:
        d_आकार = आकार( B.cols, a_आकार.height );
        len = B.rows;
        CV_Assert( a_आकार.width == len );
        अवरोध;
    हाल 1:
        d_आकार = आकार( B.cols, a_आकार.width );
        len = B.rows;
        CV_Assert( a_आकार.height == len );
        अवरोध;
    हाल 2:
        d_आकार = आकार( B.rows, a_आकार.height );
        len = B.cols;
        CV_Assert( a_आकार.width == len );
        अवरोध;
    हाल 3:
        d_आकार = आकार( B.rows, a_आकार.width );
        len = B.cols;
        CV_Assert( a_आकार.height == len );
        अवरोध;
    }

    अगर( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_आकार.height && C.cols == d_आकार.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_आकार.width && C.cols == d_आकार.height)));
    }

    _matD.create( d_आकार.height, d_आकार.width, type );
    Mat D = _matD.getMat();
    अगर( (flags & GEMM_3_T) != 0 && C.तथ्य == D.तथ्य )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    अगर( D.तथ्य == A.तथ्य || D.तथ्य == B.तथ्य )
    {
        DProxy = Mat(d_आकार.height, d_आकार.width, D.type());
        DProxyPtr = &DProxy;
    }

    अगर( type == CV_32FC1 )
        hal::gemm32f(A.ptr<भग्न>(), A.step, B.ptr<भग्न>(), B.step, static_cast<भग्न>(alpha),
                     C.ptr<भग्न>(), C.step, static_cast<भग्न>(beta),
                     DProxyPtr->ptr<भग्न>(), DProxyPtr->step,
                     a_आकार.height, a_आकार.width, DProxyPtr->cols, flags);
    अन्यथा अगर( type == CV_64FC1 )
        hal::gemm64f(A.ptr<द्विगुन>(), A.step, B.ptr<द्विगुन>(), B.step, alpha,
                     C.ptr<द्विगुन>(), C.step, beta,
                     DProxyPtr->ptr<द्विगुन>(), DProxyPtr->step,
                     a_आकार.height, a_आकार.width, DProxyPtr->cols, flags);
    अन्यथा अगर( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<भग्न>(), A.step, B.ptr<भग्न>(), B.step, static_cast<भग्न>(alpha),
                      C.ptr<भग्न>(), C.step, static_cast<भग्न>(beta),
                      DProxyPtr->ptr<भग्न>(), DProxyPtr->step,
                      a_आकार.height, a_आकार.width, DProxyPtr->cols, flags);
    अन्यथा
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<द्विगुन>(), A.step, B.ptr<द्विगुन>(), B.step, alpha,
                      C.ptr<द्विगुन>(), C.step, beta,
                      DProxyPtr->ptr<द्विगुन>(), DProxyPtr->step,
                      a_आकार.height, a_आकार.width, DProxyPtr->cols, flags);
    }

    अगर(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

अटल TransformFunc getTransformFunc(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

अटल TransformFunc getDiagTransformFunc(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम transform(निवेश_Array _src, OutputArray _dst, निवेश_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    पूर्णांक depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    bool isDiag = false;

    _dst.create_Same_आकार( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    अगर (src.तथ्य == dst.तथ्य)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    पूर्णांक mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    Auto_बफर<द्विगुन> _mbuf;
    द्विगुन* mbuf;

    अगर( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.तथ्य();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        स_रखो(tmp.ptr(), 0, tmp.योग()*tmp.elem_आकार());
        अगर( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        अन्यथा
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    अन्यथा
        mbuf = m.ptr<द्विगुन>();

    अगर( scn == dcn )
    {
        पूर्णांक i, j;
        द्विगुन eps = mtype == CV_32F ? भग्न_अंतर : द्विग_अंतर;

        अगर( scn == 1 )
        {
            द्विगुन alpha, beta;
            अगर( mtype == CV_32F )
                alpha = m.at<भग्न>(0), beta = m.at<भग्न>(1);
            अन्यथा
                alpha = m.at<द्विगुन>(0), beta = m.at<द्विगुन>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            वापस;
        }

        क्रम( i = 0, isDiag = true; isDiag && i < scn; i++ )
        {
            क्रम( j = 0; isDiag && j < scn; j++ )
            {
                द्विगुन v = mtype == CV_32F ? m.at<भग्न>(i, j) : m.at<द्विगुन>(i, j);
                अगर( i != j && भ_असल(v) > eps )
                    isDiag = false;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    स्थिर Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    माप_प्रकार i, योग = it.size;

    क्रम( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (पूर्णांक)योग, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

अटल TransformFunc getPerspectiveTransform(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम perspectiveTransform(निवेश_Array _src, OutputArray _dst, निवेश_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    पूर्णांक depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.create_Same_आकार( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    स्थिर पूर्णांक mtype = CV_64F;
    Auto_बफर<द्विगुन> _mbuf;
    द्विगुन* mbuf = m.ptr<द्विगुन>();

    अगर( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.तथ्य();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    स्थिर Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    माप_प्रकार i, योग = it.size;

    क्रम( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (पूर्णांक)योग, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_scaleAdd( निवेश_Array _src1, द्विगुन alpha, निवेश_Array _src2, OutputArray _dst, पूर्णांक type )
{
    स्थिर ocl::Device & d = ocl::Device::getDefault();

    bool doubleSupport = d.doubleFPConfig() > 0;
    आकार size = _src1.size();
    पूर्णांक depth = CV_MAT_DEPTH(type);
    अगर ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        वापस false;

    _dst.create(size, type);
    पूर्णांक cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    पूर्णांक kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    अक्षर cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], माप(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], माप(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    अगर (k.empty())
        वापस false;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_आकार(src1),
            src2arg = ocl::KernelArg::Read_Only_No_आकार(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    अगर (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (भग्न)alpha);
    अन्यथा
        k.args(src1arg, src2arg, dstarg, alpha);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)dst.cols * cn / kercn, ((माप_प्रकार)dst.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

अटल ScaleAddFunc getScaleAddFunc(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम scaleAdd(निवेश_Array _src1, द्विगुन alpha, निवेश_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    अगर( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        वापस;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    भग्न falpha = (भग्न)alpha;
    व्योम* palpha = depth == CV_32F ? (व्योम*)&falpha : (व्योम*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    अगर (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        माप_प्रकार len = src1.योग()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (पूर्णांक)len, palpha);
        वापस;
    }

    स्थिर Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    माप_प्रकार i, len = it.size*cn;

    क्रम( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (पूर्णांक)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

व्योम calcCovarMatrix( स्थिर Mat* तथ्य, पूर्णांक nsamples, Mat& covar, Mat& _mean, पूर्णांक flags, पूर्णांक ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( तथ्य, nsamples > 0 );
    आकार size = तथ्य[0].size();
    पूर्णांक sz = size.width * size.height, esz = (पूर्णांक)तथ्य[0].elem_आकार();
    पूर्णांक type = तथ्य[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    अगर( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        अगर( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        अन्यथा
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat तथ्य_3(nsamples, sz, type);

    क्रम( पूर्णांक i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( तथ्य[i].size() == size, तथ्य[i].type() == type );
        अगर( तथ्य[i].isContinuous() )
            स_नकल( तथ्य_3.ptr(i), तथ्य[i].ptr(), sz*esz );
        अन्यथा
        {
            Mat तथ्य_Row(size.height, size.width, type, तथ्य_3.ptr(i));
            तथ्य[i].copyTo(तथ्य_Row);
        }
    }

    calcCovarMatrix( तथ्य_3, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    अगर( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

व्योम calcCovarMatrix( निवेश_Array _src, OutputArray _covar, निवेश_Output_Array _mean, पूर्णांक flags, पूर्णांक ctype )
{
    CV_INSTRUMENT_REGION();

    अगर(_src.kind() == निवेश_Array_2::STD_VECTOR_MAT || _src.kind() == निवेश_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        आकार size = src[0].size();
        पूर्णांक type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat तथ्य_3(static_cast<पूर्णांक>(src.size()), size.area(), type);

        पूर्णांक i = 0;
        क्रम(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat तथ्य_Row(size.height, size.width, type, तथ्य_3.ptr(i));
            (*each).copyTo(तथ्य_Row);
        }

        Mat mean;
        अगर( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            अगर( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            अन्यथा
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( तथ्य_3, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        अगर( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        वापस;
    }

    Mat तथ्य = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    bool takeRows = (flags & cv::COVAR_ROWS) != 0;
    पूर्णांक type = तथ्य.type();
    पूर्णांक nsamples = takeRows ? तथ्य.rows : तथ्य.cols;
    CV_Assert( nsamples > 0 );
    आकार size = takeRows ? आकार(तथ्य.cols, 1) : आकार(1, तथ्य.rows);

    अगर( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        अगर( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    अन्यथा
    {
        ctype = std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), CV_32F);
        reduce( _src, _mean, takeRows ? 0 : 1, REDUCE_AVG, ctype );
        mean = _mean.getMat();
    }

    mulTransposed( तथ्य, _covar, ((flags & cv::COVAR_NORMAL) == 0) ^ takeRows,
        mean, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

अटल MahalanobisImplFunc getMahalanobisImplFunc(पूर्णांक depth)
{
#अगर_घोषित CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#अन्यथा
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#पूर्ण_अगर
}


द्विगुन Mahalanobis(निवेश_Array _v1, निवेश_Array _v2, निवेश_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    पूर्णांक type = v1.type(), depth = v1.depth();
    आकार sz = v1.size();
    पूर्णांक len = sz.width*sz.height*v1.channels();
    Auto_बफर<द्विगुन> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    अगर( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    द्विगुन परिणाम = func(v1, v2, icovar, buf.तथ्य(), len);
    वापस std::वर्ग_मूल(परिणाम);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

अटल MulTransposedFunc getMulTransposedFunc(पूर्णांक stype, पूर्णांक dtype, bool ata)
{
#अगर_घोषित CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#अन्यथा
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#पूर्ण_अगर
}

व्योम mulTransposed(निवेश_Array _src, OutputArray _dst, bool ata,
                   निवेश_Array _delta, द्विगुन scale, पूर्णांक dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    स्थिर पूर्णांक gemm_level = 100; // boundary above which GEMM is faster.
    पूर्णांक stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    अगर( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        अगर( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    पूर्णांक dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    अगर( src.तथ्य == dst.तथ्य || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        स्थिर Mat* tsrc = &src;
        अगर( !delta.empty() )
        {
            अगर( delta.size() == src.size() )
                subtract( src, delta, src2 );
            अन्यथा
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    अन्यथा
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        अगर( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, false );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

अटल द्विगुन dotProd_8u(स्थिर व्योम* src1, स्थिर व्योम* src2, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((स्थिर uchar*)src1, (स्थिर uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
अटल द्विगुन dotProd_8s(स्थिर व्योम* src1, स्थिर व्योम* src2, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((स्थिर schar*)src1, (स्थिर schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
अटल द्विगुन dotProd_16u(स्थिर व्योम* src1, स्थिर व्योम* src2, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((स्थिर ushort*)src1, (स्थिर ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
अटल द्विगुन dotProd_16s(स्थिर व्योम* src1, स्थिर व्योम* src2, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((स्थिर लघु*)src1, (स्थिर लघु*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
अटल द्विगुन dotProd_32s(स्थिर व्योम* src1, स्थिर व्योम* src2, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((स्थिर पूर्णांक*)src1, (स्थिर पूर्णांक*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
अटल द्विगुन dotProd_32f(स्थिर व्योम* src1, स्थिर व्योम* src2, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((स्थिर भग्न*)src1, (स्थिर भग्न*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
अटल द्विगुन dotProd_64f(स्थिर व्योम* src1, स्थिर व्योम* src2, पूर्णांक len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((स्थिर द्विगुन*)src1, (स्थिर द्विगुन*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

प्रकार द्विगुन (*DotProdFunc)(स्थिर व्योम* src1, स्थिर व्योम* src2, पूर्णांक len);

अटल DotProdFunc getDotProdFunc(पूर्णांक depth)
{
    अटल DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    वापस dotProdTab[depth];
}

द्विगुन Mat::dot(निवेश_Array _mat) स्थिर
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    पूर्णांक cn = channels();
    अगर (यह->dims <= 2)
    {
        द्विगुन product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, यह->तथ्य, यह->step, mat.तथ्य, mat.step,
                     यह->cols * cn, यह->rows, यह->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    अगर( isContinuous() && mat.isContinuous() )
    {
        माप_प्रकार len = योग()*cn;
        अगर( len == (माप_प्रकार)(पूर्णांक)len )
            वापस func(तथ्य, mat.तथ्य, (पूर्णांक)len);
    }

    स्थिर Mat* arrays[] = {यह, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक len = (पूर्णांक)(it.size*cn);
    द्विगुन r = 0;

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    वापस r;
}


#अगर_घोषित HAVE_OPENCL

अटल bool ocl_dot( निवेश_Array _src1, निवेश_Array _src2, द्विगुन & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    पूर्णांक type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    अगर ( !doubleSupport && depth == CV_64F )
        वापस false;

    पूर्णांक dbsize = ocl::Device::getDefault().maxComputeUnits();
    माप_प्रकार wgs = ocl::Device::getDefault().max_Work_Group_आकार();
    पूर्णांक ddepth = std::max(CV_32F, depth);

    पूर्णांक wgs2_aligned = 1;
    जबतक (wgs2_aligned < (पूर्णांक)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    अक्षर cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, माप(cvt)),
                         (पूर्णांक)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    अगर (k.empty())
        वापस false;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::Read_Only_No_आकार(src1),
            src2arg = ocl::KernelArg::Read_Only_No_आकार(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (पूर्णांक)src1.योग(), dbsize, dbarg, src2arg);

    माप_प्रकार globalsize = dbsize * wgs;
    अगर (k.run(1, &globalsize, &wgs, true))
    {
        res = योग_2(db.getMat(ACCESS_READ))[0];
        वापस true;
    }
    वापस false;
}

#पूर्ण_अगर

द्विगुन UMat::dot(निवेश_Array m) स्थिर
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.same_आकार(*यह) && m.type() == type());

#अगर_घोषित HAVE_OPENCL
    द्विगुन r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*यह, m, r), r)
#पूर्ण_अगर

    वापस getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
