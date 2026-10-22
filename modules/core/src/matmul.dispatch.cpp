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

#ifdef HAVE_CLAMDBLAS

स्थिर तार्किक ocl_gemm_amdblas( निवेशः_Array matA, निवेशः_Array matB, द्विगुण alpha,
                      निवेशः_Array matC, द्विगुण beta, OutputArray matD, पूर्णाङ्क flags )
{
    पूर्णाङ्क type = matA.type(), esz = CV_ELEM_SIZE(type);
    तार्किक haveC = matC.kind() != cv::निवेशः_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    तार्किक atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    जइ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    जइ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    जइ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    जइ ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        प्रत्यागम मिच्छ;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    जइ (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        प्रत्यागम मिच्छ;
    }
    जइ (haveC)
    {
        UMat C = matC.getUMat();
        जइ (!ocl::internal::isCLBuffer(C))
            प्रत्यागम मिच्छ;
    }
    जइ (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    अण्णहा
        D.setTo(Scalar::all(0));

    पूर्णाङ्क M = sizeD.height, N = sizeD.width, K = sizeA.width;
    पूर्णाङ्क lda = (पूर्णाङ्क)A.step / esz, ldb = (पूर्णाङ्क)B.step / esz, ldc = (पूर्णाङ्क)D.step / esz;
    पूर्णाङ्क offa = (पूर्णाङ्क)A.offset / esz, offb = (पूर्णाङ्क)B.offset / esz, offc = (पूर्णाङ्क)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    जइ (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (ध्रुव cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ध्रुव cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, रिक्त, रिक्त);
    अण्णहा जइ (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (ध्रुव cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ध्रुव cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, रिक्त, रिक्त);
    अण्णहा जइ (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (ध्रुव cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ध्रुव cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, रिक्त, रिक्त);
    }
    अण्णहा जइ (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (ध्रुव cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ध्रुव cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, रिक्त, रिक्त);
    }
    अण्णहा
        CV_Error(Error::StsUnsupportedFormat, "");

    प्रत्यागम status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_gemm( निवेशः_Array matA, निवेशः_Array matB, द्विगुण alpha,
                      निवेशः_Array matC, द्विगुण beta, OutputArray matD, पूर्णाङ्क flags )
{
    पूर्णाङ्क type = matA.type();
    पूर्णाङ्क depth = CV_MAT_DEPTH(type);
    पूर्णाङ्क cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ध्रुव ocl::Device & dev = ocl::Device::getDefault();
    तार्किक doubleSupport = dev.doubleFPConfig() > 0;

    जइ (!doubleSupport && depth == CV_64F)
        प्रत्यागम मिच्छ;

    तार्किक haveC = matC.kind() != cv::निवेशः_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    तार्किक atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    जइ (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    जइ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    जइ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    जइ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    जइ (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    तार्किक isPropagatedC2D = मिच्छ;  // D content is updated with C / C.t()

    जइ (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        जइ (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = सच्च;
        }
        अण्णहा
        {
            beta = 0.0;
        }

        तार्किक res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        जइ (res)
            प्रत्यागम सच्च;
        // fallback on generic OpenCL code
    }

    जइ (sizeD.width < 8 || sizeD.height < 8)
        प्रत्यागम मिच्छ;

    String opts;

    पूर्णाङ्क wg_size = (पूर्णाङ्क)dev.maxWorkGroupSize();
    पूर्णाङ्क sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    पूर्णाङ्क block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    जइ (atrans)
        A = A.t();

    जइ (btrans)
        B = B.t();

    जइ (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    पूर्णाङ्क vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    पूर्णाङ्क kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    जइ (k.empty())
        प्रत्यागम मिच्छ;

    जइ (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    अण्णहा
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (भिन्न)alpha, (भिन्न)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    प्रत्यागम k.run(2, globalsize, block_size !=1 ? localsize : रिक्त, मिच्छ);
}
#endif


नामाकाश hal {

शून्य gemm32f(ध्रुव भिन्न* src1, size_t src1_step, ध्रुव भिन्न* src2, size_t src2_step,
             भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_step, भिन्न beta, भिन्न* dst, size_t dst_step,
             पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#अण्णहा
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

शून्य gemm64f(ध्रुव द्विगुण* src1, size_t src1_step, ध्रुव द्विगुण* src2, size_t src2_step,
             द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_step, द्विगुण beta, द्विगुण* dst, size_t dst_step,
             पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#अण्णहा
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

शून्य gemm32fc(ध्रुव भिन्न* src1, size_t src1_step, ध्रुव भिन्न* src2, size_t src2_step,
              भिन्न alpha, ध्रुव भिन्न* src3, size_t src3_step, भिन्न beta, भिन्न* dst, size_t dst_step,
              पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#अण्णहा
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

शून्य gemm64fc(ध्रुव द्विगुण* src1, size_t src1_step, ध्रुव द्विगुण* src2, size_t src2_step,
              द्विगुण alpha, ध्रुव द्विगुण* src3, size_t src3_step, द्विगुण beta, द्विगुण* dst, size_t dst_step,
              पूर्णाङ्क m_a, पूर्णाङ्क n_a, पूर्णाङ्क n_d, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#अण्णहा
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

शून्य gemm(निवेशः_Array matA, निवेशः_Array matB, द्विगुण alpha,
          निवेशः_Array matC, द्विगुण beta, OutputArray _matD, पूर्णाङ्क flags)
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
    पूर्णाङ्क len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    विकल्प( flags & (GEMM_1_T|GEMM_2_T) )
    {
    पक्ष 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        भङ्ग;
    पक्ष 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        भङ्ग;
    पक्ष 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        भङ्ग;
    पक्ष 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        भङ्ग;
    }

    जइ( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    जइ( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    जइ( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    जइ( type == CV_32FC1 )
        hal::gemm32f(A.ptr<भिन्न>(), A.step, B.ptr<भिन्न>(), B.step, static_cast<भिन्न>(alpha),
                     C.ptr<भिन्न>(), C.step, static_cast<भिन्न>(beta),
                     DProxyPtr->ptr<भिन्न>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    अण्णहा जइ( type == CV_64FC1 )
        hal::gemm64f(A.ptr<द्विगुण>(), A.step, B.ptr<द्विगुण>(), B.step, alpha,
                     C.ptr<द्विगुण>(), C.step, beta,
                     DProxyPtr->ptr<द्विगुण>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    अण्णहा जइ( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<भिन्न>(), A.step, B.ptr<भिन्न>(), B.step, static_cast<भिन्न>(alpha),
                      C.ptr<भिन्न>(), C.step, static_cast<भिन्न>(beta),
                      DProxyPtr->ptr<भिन्न>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    अण्णहा
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<द्विगुण>(), A.step, B.ptr<द्विगुण>(), B.step, alpha,
                      C.ptr<द्विगुण>(), C.step, beta,
                      DProxyPtr->ptr<द्विगुण>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    जइ(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

स्थिर TransformFunc getTransformFunc(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

स्थिर TransformFunc getDiagTransformFunc(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य transform(निवेशः_Array _src, OutputArray _dst, निवेशः_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    पूर्णाङ्क depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    तार्किक isDiag = मिच्छ;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    जइ (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    पूर्णाङ्क mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<द्विगुण> _mbuf;
    द्विगुण* mbuf;

    जइ( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        जइ( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        अण्णहा
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    अण्णहा
        mbuf = m.ptr<द्विगुण>();

    जइ( scn == dcn )
    {
        पूर्णाङ्क i, j;
        द्विगुण eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        जइ( scn == 1 )
        {
            द्विगुण alpha, beta;
            जइ( mtype == CV_32F )
                alpha = m.at<भिन्न>(0), beta = m.at<भिन्न>(1);
            अण्णहा
                alpha = m.at<द्विगुण>(0), beta = m.at<द्विगुण>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            प्रत्यागम;
        }

        प्रति( i = 0, isDiag = सच्च; isDiag && i < scn; i++ )
        {
            प्रति( j = 0; isDiag && j < scn; j++ )
            {
                द्विगुण v = mtype == CV_32F ? m.at<भिन्न>(i, j) : m.at<द्विगुण>(i, j);
                जइ( i != j && fabs(v) > eps )
                    isDiag = मिच्छ;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    प्रति( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (पूर्णाङ्क)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

स्थिर TransformFunc getPerspectiveTransform(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य perspectiveTransform(निवेशः_Array _src, OutputArray _dst, निवेशः_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    पूर्णाङ्क depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ध्रुव पूर्णाङ्क mtype = CV_64F;
    AutoBuffer<द्विगुण> _mbuf;
    द्विगुण* mbuf = m.ptr<द्विगुण>();

    जइ( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    प्रति( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (पूर्णाङ्क)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_scaleAdd( निवेशः_Array _src1, द्विगुण alpha, निवेशः_Array _src2, OutputArray _dst, पूर्णाङ्क type )
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();

    तार्किक doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    पूर्णाङ्क depth = CV_MAT_DEPTH(type);
    जइ ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        प्रत्यागम मिच्छ;

    _dst.create(size, type);
    पूर्णाङ्क cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    पूर्णाङ्क kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    वर्ण cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], परिमाण(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], परिमाण(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    जइ (k.empty())
        प्रत्यागम मिच्छ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    जइ (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (भिन्न)alpha);
    अण्णहा
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, रिक्त, मिच्छ);
}

#endif

स्थिर ScaleAddFunc getScaleAddFunc(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य scaleAdd(निवेशः_Array _src1, द्विगुण alpha, निवेशः_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    जइ( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        प्रत्यागम;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    भिन्न falpha = (भिन्न)alpha;
    शून्य* palpha = depth == CV_32F ? (शून्य*)&falpha : (शून्य*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    जइ (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
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

शून्य calcCovarMatrix( ध्रुव Mat* data, पूर्णाङ्क nsamples, Mat& covar, Mat& _mean, पूर्णाङ्क flags, पूर्णाङ्क ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    पूर्णाङ्क sz = size.width * size.height, esz = (पूर्णाङ्क)data[0].elemSize();
    पूर्णाङ्क type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    जइ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        जइ( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        अण्णहा
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    प्रति( पूर्णाङ्क i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        जइ( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        अण्णहा
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    जइ( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

शून्य calcCovarMatrix( निवेशः_Array _src, OutputArray _covar, निवेशः_Output_Array _mean, पूर्णाङ्क flags, पूर्णाङ्क ctype )
{
    CV_INSTRUMENT_REGION();

    जइ(_src.kind() == निवेशः_Array_2::STD_VECTOR_MAT || _src.kind() == निवेशः_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        पूर्णाङ्क type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<पूर्णाङ्क>(src.size()), size.area(), type);

        पूर्णाङ्क i = 0;
        प्रति(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        जइ( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            जइ( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            अण्णहा
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        जइ( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        प्रत्यागम;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    तार्किक takeRows = (flags & cv::COVAR_ROWS) != 0;
    पूर्णाङ्क type = data.type();
    पूर्णाङ्क nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    जइ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        जइ( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    अण्णहा
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

स्थिर MahalanobisImplFunc getMahalanobisImplFunc(पूर्णाङ्क depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#अण्णहा
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


द्विगुण Mahalanobis(निवेशः_Array _v1, निवेशः_Array _v2, निवेशः_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    पूर्णाङ्क type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    पूर्णाङ्क len = sz.width*sz.height*v1.channels();
    AutoBuffer<द्विगुण> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    जइ( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    द्विगुण result = func(v1, v2, icovar, buf.data(), len);
    प्रत्यागम std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

स्थिर MulTransposedFunc getMulTransposedFunc(पूर्णाङ्क stype, पूर्णाङ्क dtype, तार्किक ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#अण्णहा
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

शून्य mulTransposed(निवेशः_Array _src, OutputArray _dst, तार्किक ata,
                   निवेशः_Array _delta, द्विगुण scale, पूर्णाङ्क dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ध्रुव पूर्णाङ्क gemm_level = 100; // boundary above which GEMM is faster.
    पूर्णाङ्क stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    जइ( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        जइ( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    पूर्णाङ्क dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    जइ( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        ध्रुव Mat* tsrc = &src;
        जइ( !delta.empty() )
        {
            जइ( delta.size() == src.size() )
                subtract( src, delta, src2 );
            अण्णहा
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    अण्णहा
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        जइ( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, मिच्छ );
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
    CV_CPU_DISPATCH(dotProd_16s, ((ध्रुव short*)src1, (ध्रुव short*)src2, len),
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

स्थिर DotProdFunc getDotProdFunc(पूर्णाङ्क depth)
{
    स्थिर DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    प्रत्यागम dotProdTab[depth];
}

द्विगुण Mat::dot(निवेशः_Array _mat) ध्रुव
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    पूर्णाङ्क cn = channels();
    जइ (एतत्->dims <= 2)
    {
        द्विगुण product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, एतत्->data, एतत्->step, mat.data, mat.step,
                     एतत्->cols * cn, एतत्->rows, एतत्->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    जइ( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        जइ( len == (size_t)(पूर्णाङ्क)len )
            प्रत्यागम func(data, mat.data, (पूर्णाङ्क)len);
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


#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_dot( निवेशः_Array _src1, निवेशः_Array _src2, द्विगुण & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    पूर्णाङ्क type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    जइ ( !doubleSupport && depth == CV_64F )
        प्रत्यागम मिच्छ;

    पूर्णाङ्क dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    पूर्णाङ्क ddepth = std::max(CV_32F, depth);

    पूर्णाङ्क wgs2_aligned = 1;
    जाव (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    वर्ण cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, परिमाण(cvt)),
                         (पूर्णाङ्क)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    जइ (k.empty())
        प्रत्यागम मिच्छ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (पूर्णाङ्क)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    जइ (k.run(1, &globalsize, &wgs, सच्च))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        प्रत्यागम सच्च;
    }
    प्रत्यागम मिच्छ;
}

#endif

द्विगुण UMat::dot(निवेशः_Array m) ध्रुव
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*एतत्) && m.type() == type());

#ifdef HAVE_OPENCL
    द्विगुण r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*एतत्, m, r), r)
#endif

    प्रत्यागम getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
