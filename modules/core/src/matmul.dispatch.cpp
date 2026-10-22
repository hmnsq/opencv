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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_core.hpp"
#അന്തര്ഭാവ "intel_gpu_gemm.inl.hpp"

#അന്തര്ഭാവ "matmul.simd.hpp"
#അന്തര്ഭാവ "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ifdef HAVE_CLAMDBLAS

സ്ഥിതം തർക്കം ocl_gemm_amdblas( നിവേശം_Array matA, നിവേശം_Array matB, ഇരട്ട alpha,
                      നിവേശം_Array matC, ഇരട്ട beta, OutputArray matD, പൂർണ്ണസംഖ്യ flags )
{
    പൂർണ്ണസംഖ്യ type = matA.type(), esz = CV_ELEM_SIZE(type);
    തർക്കം haveC = matC.kind() != cv::നിവേശം_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    തർക്കം atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    എങ്കിൽ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    എങ്കിൽ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    എങ്കിൽ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    എങ്കിൽ ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        തിരികെ അസത്യം;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    എങ്കിൽ (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        തിരികെ അസത്യം;
    }
    എങ്കിൽ (haveC)
    {
        UMat C = matC.getUMat();
        എങ്കിൽ (!ocl::internal::isCLBuffer(C))
            തിരികെ അസത്യം;
    }
    എങ്കിൽ (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    അല്ലെങ്കിൽ
        D.setTo(Scalar::all(0));

    പൂർണ്ണസംഖ്യ M = sizeD.height, N = sizeD.width, K = sizeA.width;
    പൂർണ്ണസംഖ്യ lda = (പൂർണ്ണസംഖ്യ)A.step / esz, ldb = (പൂർണ്ണസംഖ്യ)B.step / esz, ldc = (പൂർണ്ണസംഖ്യ)D.step / esz;
    പൂർണ്ണസംഖ്യ offa = (പൂർണ്ണസംഖ്യ)A.offset / esz, offb = (പൂർണ്ണസംഖ്യ)B.offset / esz, offc = (പൂർണ്ണസംഖ്യ)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    എങ്കിൽ (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (സ്ഥിരം cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (സ്ഥിരം cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ശൂന്യ, ശൂന്യ);
    അല്ലെങ്കിൽ എങ്കിൽ (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (സ്ഥിരം cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (സ്ഥിരം cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ശൂന്യ, ശൂന്യ);
    അല്ലെങ്കിൽ എങ്കിൽ (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (സ്ഥിരം cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (സ്ഥിരം cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, ശൂന്യ, ശൂന്യ);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (സ്ഥിരം cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (സ്ഥിരം cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ശൂന്യ, ശൂന്യ);
    }
    അല്ലെങ്കിൽ
        CV_Error(Error::StsUnsupportedFormat, "");

    തിരികെ status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
സ്ഥിതം തർക്കം ocl_gemm( നിവേശം_Array matA, നിവേശം_Array matB, ഇരട്ട alpha,
                      നിവേശം_Array matC, ഇരട്ട beta, OutputArray matD, പൂർണ്ണസംഖ്യ flags )
{
    പൂർണ്ണസംഖ്യ type = matA.type();
    പൂർണ്ണസംഖ്യ depth = CV_MAT_DEPTH(type);
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    സ്ഥിരം ocl::Device & dev = ocl::Device::getDefault();
    തർക്കം doubleSupport = dev.doubleFPConfig() > 0;

    എങ്കിൽ (!doubleSupport && depth == CV_64F)
        തിരികെ അസത്യം;

    തർക്കം haveC = matC.kind() != cv::നിവേശം_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    തർക്കം atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    എങ്കിൽ (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    എങ്കിൽ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    എങ്കിൽ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    എങ്കിൽ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    എങ്കിൽ (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    തർക്കം isPropagatedC2D = അസത്യം;  // D content is updated with C / C.t()

    എങ്കിൽ (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        എങ്കിൽ (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = സത്യം;
        }
        അല്ലെങ്കിൽ
        {
            beta = 0.0;
        }

        തർക്കം res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        എങ്കിൽ (res)
            തിരികെ സത്യം;
        // fallback on generic OpenCL code
    }

    എങ്കിൽ (sizeD.width < 8 || sizeD.height < 8)
        തിരികെ അസത്യം;

    String opts;

    പൂർണ്ണസംഖ്യ wg_size = (പൂർണ്ണസംഖ്യ)dev.maxWorkGroupSize();
    പൂർണ്ണസംഖ്യ sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    പൂർണ്ണസംഖ്യ block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    എങ്കിൽ (atrans)
        A = A.t();

    എങ്കിൽ (btrans)
        B = B.t();

    എങ്കിൽ (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    പൂർണ്ണസംഖ്യ vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    പൂർണ്ണസംഖ്യ kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    എങ്കിൽ (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    അല്ലെങ്കിൽ
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (ദശാംശം)alpha, (ദശാംശം)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    തിരികെ k.run(2, globalsize, block_size !=1 ? localsize : ശൂന്യ, അസത്യം);
}
#endif


നാമാകാശ hal {

ശൂന്യം gemm32f(സ്ഥിരം ദശാംശം* src1, size_t src1_step, സ്ഥിരം ദശാംശം* src2, size_t src2_step,
             ദശാംശം alpha, സ്ഥിരം ദശാംശം* src3, size_t src3_step, ദശാംശം beta, ദശാംശം* dst, size_t dst_step,
             പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#അല്ലെങ്കിൽ
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ശൂന്യം gemm64f(സ്ഥിരം ഇരട്ട* src1, size_t src1_step, സ്ഥിരം ഇരട്ട* src2, size_t src2_step,
             ഇരട്ട alpha, സ്ഥിരം ഇരട്ട* src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട* dst, size_t dst_step,
             പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#അല്ലെങ്കിൽ
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ശൂന്യം gemm32fc(സ്ഥിരം ദശാംശം* src1, size_t src1_step, സ്ഥിരം ദശാംശം* src2, size_t src2_step,
              ദശാംശം alpha, സ്ഥിരം ദശാംശം* src3, size_t src3_step, ദശാംശം beta, ദശാംശം* dst, size_t dst_step,
              പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#അല്ലെങ്കിൽ
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ശൂന്യം gemm64fc(സ്ഥിരം ഇരട്ട* src1, size_t src1_step, സ്ഥിരം ഇരട്ട* src2, size_t src2_step,
              ഇരട്ട alpha, സ്ഥിരം ഇരട്ട* src3, size_t src3_step, ഇരട്ട beta, ഇരട്ട* dst, size_t dst_step,
              പൂർണ്ണസംഖ്യ m_a, പൂർണ്ണസംഖ്യ n_a, പൂർണ്ണസംഖ്യ n_d, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#അല്ലെങ്കിൽ
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

ശൂന്യം gemm(നിവേശം_Array matA, നിവേശം_Array matB, ഇരട്ട alpha,
          നിവേശം_Array matC, ഇരട്ട beta, OutputArray _matD, പൂർണ്ണസംഖ്യ flags)
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
    പൂർണ്ണസംഖ്യ len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    തിരഞ്ഞെടുപ്പ്( flags & (GEMM_1_T|GEMM_2_T) )
    {
    അവസ്ഥ 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        നിർത്തുക;
    അവസ്ഥ 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        നിർത്തുക;
    അവസ്ഥ 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        നിർത്തുക;
    അവസ്ഥ 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        നിർത്തുക;
    }

    എങ്കിൽ( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    എങ്കിൽ( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    എങ്കിൽ( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    എങ്കിൽ( type == CV_32FC1 )
        hal::gemm32f(A.ptr<ദശാംശം>(), A.step, B.ptr<ദശാംശം>(), B.step, static_cast<ദശാംശം>(alpha),
                     C.ptr<ദശാംശം>(), C.step, static_cast<ദശാംശം>(beta),
                     DProxyPtr->ptr<ദശാംശം>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_64FC1 )
        hal::gemm64f(A.ptr<ഇരട്ട>(), A.step, B.ptr<ഇരട്ട>(), B.step, alpha,
                     C.ptr<ഇരട്ട>(), C.step, beta,
                     DProxyPtr->ptr<ഇരട്ട>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<ദശാംശം>(), A.step, B.ptr<ദശാംശം>(), B.step, static_cast<ദശാംശം>(alpha),
                      C.ptr<ദശാംശം>(), C.step, static_cast<ദശാംശം>(beta),
                      DProxyPtr->ptr<ദശാംശം>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    അല്ലെങ്കിൽ
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<ഇരട്ട>(), A.step, B.ptr<ഇരട്ട>(), B.step, alpha,
                      C.ptr<ഇരട്ട>(), C.step, beta,
                      DProxyPtr->ptr<ഇരട്ട>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    എങ്കിൽ(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

സ്ഥിതം TransformFunc getTransformFunc(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

സ്ഥിതം TransformFunc getDiagTransformFunc(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം transform(നിവേശം_Array _src, OutputArray _dst, നിവേശം_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    പൂർണ്ണസംഖ്യ depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    തർക്കം isDiag = അസത്യം;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    എങ്കിൽ (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    പൂർണ്ണസംഖ്യ mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<ഇരട്ട> _mbuf;
    ഇരട്ട* mbuf;

    എങ്കിൽ( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        എങ്കിൽ( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        അല്ലെങ്കിൽ
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    അല്ലെങ്കിൽ
        mbuf = m.ptr<ഇരട്ട>();

    എങ്കിൽ( scn == dcn )
    {
        പൂർണ്ണസംഖ്യ i, j;
        ഇരട്ട eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        എങ്കിൽ( scn == 1 )
        {
            ഇരട്ട alpha, beta;
            എങ്കിൽ( mtype == CV_32F )
                alpha = m.at<ദശാംശം>(0), beta = m.at<ദശാംശം>(1);
            അല്ലെങ്കിൽ
                alpha = m.at<ഇരട്ട>(0), beta = m.at<ഇരട്ട>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            തിരികെ;
        }

        വേണ്ടി( i = 0, isDiag = സത്യം; isDiag && i < scn; i++ )
        {
            വേണ്ടി( j = 0; isDiag && j < scn; j++ )
            {
                ഇരട്ട v = mtype == CV_32F ? m.at<ദശാംശം>(i, j) : m.at<ഇരട്ട>(i, j);
                എങ്കിൽ( i != j && fabs(v) > eps )
                    isDiag = അസത്യം;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    സ്ഥിരം Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    വേണ്ടി( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (പൂർണ്ണസംഖ്യ)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

സ്ഥിതം TransformFunc getPerspectiveTransform(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം perspectiveTransform(നിവേശം_Array _src, OutputArray _dst, നിവേശം_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    പൂർണ്ണസംഖ്യ depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    സ്ഥിരം പൂർണ്ണസംഖ്യ mtype = CV_64F;
    AutoBuffer<ഇരട്ട> _mbuf;
    ഇരട്ട* mbuf = m.ptr<ഇരട്ട>();

    എങ്കിൽ( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    സ്ഥിരം Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    വേണ്ടി( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (പൂർണ്ണസംഖ്യ)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_scaleAdd( നിവേശം_Array _src1, ഇരട്ട alpha, നിവേശം_Array _src2, OutputArray _dst, പൂർണ്ണസംഖ്യ type )
{
    സ്ഥിരം ocl::Device & d = ocl::Device::getDefault();

    തർക്കം doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    പൂർണ്ണസംഖ്യ depth = CV_MAT_DEPTH(type);
    എങ്കിൽ ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        തിരികെ അസത്യം;

    _dst.create(size, type);
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    പൂർണ്ണസംഖ്യ kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    അക്ഷരം cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], വലുപ്പം(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], വലുപ്പം(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    എങ്കിൽ (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (ദശാംശം)alpha);
    അല്ലെങ്കിൽ
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

#endif

സ്ഥിതം ScaleAddFunc getScaleAddFunc(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ശൂന്യം scaleAdd(നിവേശം_Array _src1, ഇരട്ട alpha, നിവേശം_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    എങ്കിൽ( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        തിരികെ;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    ദശാംശം falpha = (ദശാംശം)alpha;
    ശൂന്യം* palpha = depth == CV_32F ? (ശൂന്യം*)&falpha : (ശൂന്യം*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    എങ്കിൽ (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (പൂർണ്ണസംഖ്യ)len, palpha);
        തിരികെ;
    }

    സ്ഥിരം Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    വേണ്ടി( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (പൂർണ്ണസംഖ്യ)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

ശൂന്യം calcCovarMatrix( സ്ഥിരം Mat* data, പൂർണ്ണസംഖ്യ nsamples, Mat& covar, Mat& _mean, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    പൂർണ്ണസംഖ്യ sz = size.width * size.height, esz = (പൂർണ്ണസംഖ്യ)data[0].elemSize();
    പൂർണ്ണസംഖ്യ type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    എങ്കിൽ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        എങ്കിൽ( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        അല്ലെങ്കിൽ
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        എങ്കിൽ( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        അല്ലെങ്കിൽ
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    എങ്കിൽ( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

ശൂന്യം calcCovarMatrix( നിവേശം_Array _src, OutputArray _covar, നിവേശം_Output_Array _mean, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ ctype )
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ(_src.kind() == നിവേശം_Array_2::STD_VECTOR_MAT || _src.kind() == നിവേശം_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        പൂർണ്ണസംഖ്യ type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<പൂർണ്ണസംഖ്യ>(src.size()), size.area(), type);

        പൂർണ്ണസംഖ്യ i = 0;
        വേണ്ടി(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        എങ്കിൽ( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            എങ്കിൽ( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            അല്ലെങ്കിൽ
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        എങ്കിൽ( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        തിരികെ;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    തർക്കം takeRows = (flags & cv::COVAR_ROWS) != 0;
    പൂർണ്ണസംഖ്യ type = data.type();
    പൂർണ്ണസംഖ്യ nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    എങ്കിൽ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        എങ്കിൽ( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    അല്ലെങ്കിൽ
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

സ്ഥിതം MahalanobisImplFunc getMahalanobisImplFunc(പൂർണ്ണസംഖ്യ depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#അല്ലെങ്കിൽ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


ഇരട്ട Mahalanobis(നിവേശം_Array _v1, നിവേശം_Array _v2, നിവേശം_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    പൂർണ്ണസംഖ്യ type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    പൂർണ്ണസംഖ്യ len = sz.width*sz.height*v1.channels();
    AutoBuffer<ഇരട്ട> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    എങ്കിൽ( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    ഇരട്ട result = func(v1, v2, icovar, buf.data(), len);
    തിരികെ std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

സ്ഥിതം MulTransposedFunc getMulTransposedFunc(പൂർണ്ണസംഖ്യ stype, പൂർണ്ണസംഖ്യ dtype, തർക്കം ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#അല്ലെങ്കിൽ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

ശൂന്യം mulTransposed(നിവേശം_Array _src, OutputArray _dst, തർക്കം ata,
                   നിവേശം_Array _delta, ഇരട്ട scale, പൂർണ്ണസംഖ്യ dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    സ്ഥിരം പൂർണ്ണസംഖ്യ gemm_level = 100; // boundary above which GEMM is faster.
    പൂർണ്ണസംഖ്യ stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    എങ്കിൽ( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        എങ്കിൽ( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    പൂർണ്ണസംഖ്യ dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    എങ്കിൽ( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        സ്ഥിരം Mat* tsrc = &src;
        എങ്കിൽ( !delta.empty() )
        {
            എങ്കിൽ( delta.size() == src.size() )
                subtract( src, delta, src2 );
            അല്ലെങ്കിൽ
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    അല്ലെങ്കിൽ
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        എങ്കിൽ( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, അസത്യം );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

സ്ഥിതം ഇരട്ട dotProd_8u(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((സ്ഥിരം uchar*)src1, (സ്ഥിരം uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
സ്ഥിതം ഇരട്ട dotProd_8s(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((സ്ഥിരം schar*)src1, (സ്ഥിരം schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
സ്ഥിതം ഇരട്ട dotProd_16u(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((സ്ഥിരം ushort*)src1, (സ്ഥിരം ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
സ്ഥിതം ഇരട്ട dotProd_16s(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((സ്ഥിരം short*)src1, (സ്ഥിരം short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
സ്ഥിതം ഇരട്ട dotProd_32s(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((സ്ഥിരം പൂർണ്ണസംഖ്യ*)src1, (സ്ഥിരം പൂർണ്ണസംഖ്യ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
സ്ഥിതം ഇരട്ട dotProd_32f(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((സ്ഥിരം ദശാംശം*)src1, (സ്ഥിരം ദശാംശം*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
സ്ഥിതം ഇരട്ട dotProd_64f(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, പൂർണ്ണസംഖ്യ len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((സ്ഥിരം ഇരട്ട*)src1, (സ്ഥിരം ഇരട്ട*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

തരനിർവചനം ഇരട്ട (*DotProdFunc)(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, പൂർണ്ണസംഖ്യ len);

സ്ഥിതം DotProdFunc getDotProdFunc(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    തിരികെ dotProdTab[depth];
}

ഇരട്ട Mat::dot(നിവേശം_Array _mat) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    പൂർണ്ണസംഖ്യ cn = channels();
    എങ്കിൽ (ഇത്->dims <= 2)
    {
        ഇരട്ട product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, ഇത്->data, ഇത്->step, mat.data, mat.step,
                     ഇത്->cols * cn, ഇത്->rows, ഇത്->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    എങ്കിൽ( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        എങ്കിൽ( len == (size_t)(പൂർണ്ണസംഖ്യ)len )
            തിരികെ func(data, mat.data, (പൂർണ്ണസംഖ്യ)len);
    }

    സ്ഥിരം Mat* arrays[] = {ഇത്, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ len = (പൂർണ്ണസംഖ്യ)(it.size*cn);
    ഇരട്ട r = 0;

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    തിരികെ r;
}


#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_dot( നിവേശം_Array _src1, നിവേശം_Array _src2, ഇരട്ട & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    പൂർണ്ണസംഖ്യ type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    തർക്കം doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    എങ്കിൽ ( !doubleSupport && depth == CV_64F )
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    പൂർണ്ണസംഖ്യ ddepth = std::max(CV_32F, depth);

    പൂർണ്ണസംഖ്യ wgs2_aligned = 1;
    ആകുമ്പോൾ (wgs2_aligned < (പൂർണ്ണസംഖ്യ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    അക്ഷരം cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, വലുപ്പം(cvt)),
                         (പൂർണ്ണസംഖ്യ)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (പൂർണ്ണസംഖ്യ)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    എങ്കിൽ (k.run(1, &globalsize, &wgs, സത്യം))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
}

#endif

ഇരട്ട UMat::dot(നിവേശം_Array m) സ്ഥിരം
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*ഇത്) && m.type() == type());

#ifdef HAVE_OPENCL
    ഇരട്ട r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*ഇത്, m, r), r)
#endif

    തിരികെ getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
