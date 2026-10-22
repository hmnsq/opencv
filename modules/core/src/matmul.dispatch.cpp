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

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ <opencv2/core/utils/logger.hpp>

#অন্তর্ভাৱ "opencl_kernels_core.hpp"
#অন্তর্ভাৱ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#অন্তর্ভাৱ "opencv2/core/opencl/runtime/opencl_core.hpp"
#অন্তর্ভাৱ "intel_gpu_gemm.inl.hpp"

#অন্তর্ভাৱ "matmul.simd.hpp"
#অন্তর্ভাৱ "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ifdef HAVE_CLAMDBLAS

স্থির তার্কিক ocl_gemm_amdblas( প্ৰবেশ_Array matA, প্ৰবেশ_Array matB, দ্ৱিগুণ alpha,
                      প্ৰবেশ_Array matC, দ্ৱিগুণ beta, OutputArray matD, পূর্ণাঙ্ক flags )
{
    পূর্ণাঙ্ক type = matA.type(), esz = CV_ELEM_SIZE(type);
    তার্কিক haveC = matC.kind() != cv::প্ৰবেশ_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    তার্কিক atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    যদি (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    যদি (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    যদি (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    যদি ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        ঘূৰাই মিছা;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    যদি (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        ঘূৰাই মিছা;
    }
    যদি (haveC)
    {
        UMat C = matC.getUMat();
        যদি (!ocl::internal::isCLBuffer(C))
            ঘূৰাই মিছা;
    }
    যদি (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    অন্যথা
        D.setTo(Scalar::all(0));

    পূর্ণাঙ্ক M = sizeD.height, N = sizeD.width, K = sizeA.width;
    পূর্ণাঙ্ক lda = (পূর্ণাঙ্ক)A.step / esz, ldb = (পূর্ণাঙ্ক)B.step / esz, ldc = (পূর্ণাঙ্ক)D.step / esz;
    পূর্ণাঙ্ক offa = (পূর্ণাঙ্ক)A.offset / esz, offb = (পূর্ণাঙ্ক)B.offset / esz, offc = (পূর্ণাঙ্ক)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    যদি (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (ধ্রুৱ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ধ্রুৱ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, রিক্ত, রিক্ত);
    অন্যথা যদি (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (ধ্রুৱ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ধ্রুৱ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, রিক্ত, রিক্ত);
    অন্যথা যদি (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (ধ্রুৱ cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ধ্রুৱ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, রিক্ত, রিক্ত);
    }
    অন্যথা যদি (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (ধ্রুৱ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ধ্রুৱ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, রিক্ত, রিক্ত);
    }
    অন্যথা
        CV_Error(Error::StsUnsupportedFormat, "");

    ঘূৰাই status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
স্থির তার্কিক ocl_gemm( প্ৰবেশ_Array matA, প্ৰবেশ_Array matB, দ্ৱিগুণ alpha,
                      প্ৰবেশ_Array matC, দ্ৱিগুণ beta, OutputArray matD, পূর্ণাঙ্ক flags )
{
    পূর্ণাঙ্ক type = matA.type();
    পূর্ণাঙ্ক depth = CV_MAT_DEPTH(type);
    পূর্ণাঙ্ক cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ধ্রুৱ ocl::Device & dev = ocl::Device::getDefault();
    তার্কিক doubleSupport = dev.doubleFPConfig() > 0;

    যদি (!doubleSupport && depth == CV_64F)
        ঘূৰাই মিছা;

    তার্কিক haveC = matC.kind() != cv::প্ৰবেশ_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    তার্কিক atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    যদি (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    যদি (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    যদি (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    যদি (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    যদি (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    তার্কিক isPropagatedC2D = মিছা;  // D content is updated with C / C.t()

    যদি (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        যদি (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = সঁচা;
        }
        অন্যথা
        {
            beta = 0.0;
        }

        তার্কিক res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        যদি (res)
            ঘূৰাই সঁচা;
        // fallback on generic OpenCL code
    }

    যদি (sizeD.width < 8 || sizeD.height < 8)
        ঘূৰাই মিছা;

    String opts;

    পূর্ণাঙ্ক wg_size = (পূর্ণাঙ্ক)dev.maxWorkGroupSize();
    পূর্ণাঙ্ক sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    পূর্ণাঙ্ক block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    যদি (atrans)
        A = A.t();

    যদি (btrans)
        B = B.t();

    যদি (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    পূর্ণাঙ্ক vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    পূর্ণাঙ্ক kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    যদি (k.empty())
        ঘূৰাই মিছা;

    যদি (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    অন্যথা
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (ভিন্ন)alpha, (ভিন্ন)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    ঘূৰাই k.run(2, globalsize, block_size !=1 ? localsize : রিক্ত, মিছা);
}
#endif


নামাকাশ hal {

শূন্য gemm32f(ধ্রুৱ ভিন্ন* src1, size_t src1_step, ধ্রুৱ ভিন্ন* src2, size_t src2_step,
             ভিন্ন alpha, ধ্রুৱ ভিন্ন* src3, size_t src3_step, ভিন্ন beta, ভিন্ন* dst, size_t dst_step,
             পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags)
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

শূন্য gemm64f(ধ্রুৱ দ্ৱিগুণ* src1, size_t src1_step, ধ্রুৱ দ্ৱিগুণ* src2, size_t src2_step,
             দ্ৱিগুণ alpha, ধ্রুৱ দ্ৱিগুণ* src3, size_t src3_step, দ্ৱিগুণ beta, দ্ৱিগুণ* dst, size_t dst_step,
             পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags)
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

শূন্য gemm32fc(ধ্রুৱ ভিন্ন* src1, size_t src1_step, ধ্রুৱ ভিন্ন* src2, size_t src2_step,
              ভিন্ন alpha, ধ্রুৱ ভিন্ন* src3, size_t src3_step, ভিন্ন beta, ভিন্ন* dst, size_t dst_step,
              পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags)
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

শূন্য gemm64fc(ধ্রুৱ দ্ৱিগুণ* src1, size_t src1_step, ধ্রুৱ দ্ৱিগুণ* src2, size_t src2_step,
              দ্ৱিগুণ alpha, ধ্রুৱ দ্ৱিগুণ* src3, size_t src3_step, দ্ৱিগুণ beta, দ্ৱিগুণ* dst, size_t dst_step,
              পূর্ণাঙ্ক m_a, পূর্ণাঙ্ক n_a, পূর্ণাঙ্ক n_d, পূর্ণাঙ্ক flags)
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

শূন্য gemm(প্ৰবেশ_Array matA, প্ৰবেশ_Array matB, দ্ৱিগুণ alpha,
          প্ৰবেশ_Array matC, দ্ৱিগুণ beta, OutputArray _matD, পূর্ণাঙ্ক flags)
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
    পূর্ণাঙ্ক len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    ৱিকল্প( flags & (GEMM_1_T|GEMM_2_T) )
    {
    পক্ষ 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        ভঙ্গ;
    পক্ষ 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        ভঙ্গ;
    পক্ষ 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        ভঙ্গ;
    পক্ষ 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        ভঙ্গ;
    }

    যদি( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    যদি( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    যদি( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    যদি( type == CV_32FC1 )
        hal::gemm32f(A.ptr<ভিন্ন>(), A.step, B.ptr<ভিন্ন>(), B.step, static_cast<ভিন্ন>(alpha),
                     C.ptr<ভিন্ন>(), C.step, static_cast<ভিন্ন>(beta),
                     DProxyPtr->ptr<ভিন্ন>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    অন্যথা যদি( type == CV_64FC1 )
        hal::gemm64f(A.ptr<দ্ৱিগুণ>(), A.step, B.ptr<দ্ৱিগুণ>(), B.step, alpha,
                     C.ptr<দ্ৱিগুণ>(), C.step, beta,
                     DProxyPtr->ptr<দ্ৱিগুণ>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    অন্যথা যদি( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<ভিন্ন>(), A.step, B.ptr<ভিন্ন>(), B.step, static_cast<ভিন্ন>(alpha),
                      C.ptr<ভিন্ন>(), C.step, static_cast<ভিন্ন>(beta),
                      DProxyPtr->ptr<ভিন্ন>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    অন্যথা
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<দ্ৱিগুণ>(), A.step, B.ptr<দ্ৱিগুণ>(), B.step, alpha,
                      C.ptr<দ্ৱিগুণ>(), C.step, beta,
                      DProxyPtr->ptr<দ্ৱিগুণ>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    যদি(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

স্থির TransformFunc getTransformFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

স্থির TransformFunc getDiagTransformFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য transform(প্ৰবেশ_Array _src, OutputArray _dst, প্ৰবেশ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    পূর্ণাঙ্ক depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    তার্কিক isDiag = মিছা;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    যদি (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    পূর্ণাঙ্ক mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<দ্ৱিগুণ> _mbuf;
    দ্ৱিগুণ* mbuf;

    যদি( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
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
        mbuf = m.ptr<দ্ৱিগুণ>();

    যদি( scn == dcn )
    {
        পূর্ণাঙ্ক i, j;
        দ্ৱিগুণ eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        যদি( scn == 1 )
        {
            দ্ৱিগুণ alpha, beta;
            যদি( mtype == CV_32F )
                alpha = m.at<ভিন্ন>(0), beta = m.at<ভিন্ন>(1);
            অন্যথা
                alpha = m.at<দ্ৱিগুণ>(0), beta = m.at<দ্ৱিগুণ>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            ঘূৰাই;
        }

        বাবে( i = 0, isDiag = সঁচা; isDiag && i < scn; i++ )
        {
            বাবে( j = 0; isDiag && j < scn; j++ )
            {
                দ্ৱিগুণ v = mtype == CV_32F ? m.at<ভিন্ন>(i, j) : m.at<দ্ৱিগুণ>(i, j);
                যদি( i != j && fabs(v) > eps )
                    isDiag = মিছা;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    ধ্রুৱ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    বাবে( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (পূর্ণাঙ্ক)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

স্থির TransformFunc getPerspectiveTransform(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য perspectiveTransform(প্ৰবেশ_Array _src, OutputArray _dst, প্ৰবেশ_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    পূর্ণাঙ্ক depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ধ্রুৱ পূর্ণাঙ্ক mtype = CV_64F;
    AutoBuffer<দ্ৱিগুণ> _mbuf;
    দ্ৱিগুণ* mbuf = m.ptr<দ্ৱিগুণ>();

    যদি( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    ধ্রুৱ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    বাবে( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (পূর্ণাঙ্ক)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_scaleAdd( প্ৰবেশ_Array _src1, দ্ৱিগুণ alpha, প্ৰবেশ_Array _src2, OutputArray _dst, পূর্ণাঙ্ক type )
{
    ধ্রুৱ ocl::Device & d = ocl::Device::getDefault();

    তার্কিক doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    পূর্ণাঙ্ক depth = CV_MAT_DEPTH(type);
    যদি ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        ঘূৰাই মিছা;

    _dst.create(size, type);
    পূর্ণাঙ্ক cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    পূর্ণাঙ্ক kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    ৱর্ণ cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], পরিমাণ(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], পরিমাণ(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    যদি (k.empty())
        ঘূৰাই মিছা;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    যদি (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (ভিন্ন)alpha);
    অন্যথা
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ঘূৰাই k.run(2, globalsize, রিক্ত, মিছা);
}

#endif

স্থির ScaleAddFunc getScaleAddFunc(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

শূন্য scaleAdd(প্ৰবেশ_Array _src1, দ্ৱিগুণ alpha, প্ৰবেশ_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    যদি( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        ঘূৰাই;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    ভিন্ন falpha = (ভিন্ন)alpha;
    শূন্য* palpha = depth == CV_32F ? (শূন্য*)&falpha : (শূন্য*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    যদি (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (পূর্ণাঙ্ক)len, palpha);
        ঘূৰাই;
    }

    ধ্রুৱ Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    বাবে( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (পূর্ণাঙ্ক)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

শূন্য calcCovarMatrix( ধ্রুৱ Mat* data, পূর্ণাঙ্ক nsamples, Mat& covar, Mat& _mean, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    পূর্ণাঙ্ক sz = size.width * size.height, esz = (পূর্ণাঙ্ক)data[0].elemSize();
    পূর্ণাঙ্ক type = data[0].type();
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

    Mat _data(nsamples, sz, type);

    বাবে( পূর্ণাঙ্ক i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        যদি( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        অন্যথা
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    যদি( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

শূন্য calcCovarMatrix( প্ৰবেশ_Array _src, OutputArray _covar, প্ৰবেশ_Output_Array _mean, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক ctype )
{
    CV_INSTRUMENT_REGION();

    যদি(_src.kind() == প্ৰবেশ_Array_2::STD_VECTOR_MAT || _src.kind() == প্ৰবেশ_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        পূর্ণাঙ্ক type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<পূর্ণাঙ্ক>(src.size()), size.area(), type);

        পূর্ণাঙ্ক i = 0;
        বাবে(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
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

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        যদি( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        ঘূৰাই;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    তার্কিক takeRows = (flags & cv::COVAR_ROWS) != 0;
    পূর্ণাঙ্ক type = data.type();
    পূর্ণাঙ্ক nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

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

    mulTransposed( data, _covar, ((flags & cv::COVAR_NORMAL) == 0) ^ takeRows,
        mean, (flags & cv::COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}



/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

স্থির MahalanobisImplFunc getMahalanobisImplFunc(পূর্ণাঙ্ক depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#অন্যথা
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


দ্ৱিগুণ Mahalanobis(প্ৰবেশ_Array _v1, প্ৰবেশ_Array _v2, প্ৰবেশ_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    পূর্ণাঙ্ক type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    পূর্ণাঙ্ক len = sz.width*sz.height*v1.channels();
    AutoBuffer<দ্ৱিগুণ> buf(len);

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

    দ্ৱিগুণ result = func(v1, v2, icovar, buf.data(), len);
    ঘূৰাই std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

স্থির MulTransposedFunc getMulTransposedFunc(পূর্ণাঙ্ক stype, পূর্ণাঙ্ক dtype, তার্কিক ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#অন্যথা
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

শূন্য mulTransposed(প্ৰবেশ_Array _src, OutputArray _dst, তার্কিক ata,
                   প্ৰবেশ_Array _delta, দ্ৱিগুণ scale, পূর্ণাঙ্ক dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ধ্রুৱ পূর্ণাঙ্ক gemm_level = 100; // boundary above which GEMM is faster.
    পূর্ণাঙ্ক stype = src.type();
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

    পূর্ণাঙ্ক dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    যদি( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        ধ্রুৱ Mat* tsrc = &src;
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
        completeSymm( dst, মিছা );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

স্থির দ্ৱিগুণ dotProd_8u(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ধ্রুৱ uchar*)src1, (ধ্রুৱ uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্ৱিগুণ dotProd_8s(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ধ্রুৱ schar*)src1, (ধ্রুৱ schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্ৱিগুণ dotProd_16u(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ধ্রুৱ ushort*)src1, (ধ্রুৱ ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্ৱিগুণ dotProd_16s(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ধ্রুৱ short*)src1, (ধ্রুৱ short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্ৱিগুণ dotProd_32s(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ধ্রুৱ পূর্ণাঙ্ক*)src1, (ধ্রুৱ পূর্ণাঙ্ক*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্ৱিগুণ dotProd_32f(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ধ্রুৱ ভিন্ন*)src1, (ধ্রুৱ ভিন্ন*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
স্থির দ্ৱিগুণ dotProd_64f(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, পূর্ণাঙ্ক len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ধ্রুৱ দ্ৱিগুণ*)src1, (ধ্রুৱ দ্ৱিগুণ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

প্রকারনির্দেশ দ্ৱিগুণ (*DotProdFunc)(ধ্রুৱ শূন্য* src1, ধ্রুৱ শূন্য* src2, পূর্ণাঙ্ক len);

স্থির DotProdFunc getDotProdFunc(পূর্ণাঙ্ক depth)
{
    স্থির DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    ঘূৰাই dotProdTab[depth];
}

দ্ৱিগুণ Mat::dot(প্ৰবেশ_Array _mat) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    পূর্ণাঙ্ক cn = channels();
    যদি (এতত্->dims <= 2)
    {
        দ্ৱিগুণ product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, এতত্->data, এতত্->step, mat.data, mat.step,
                     এতত্->cols * cn, এতত্->rows, এতত্->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    যদি( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        যদি( len == (size_t)(পূর্ণাঙ্ক)len )
            ঘূৰাই func(data, mat.data, (পূর্ণাঙ্ক)len);
    }

    ধ্রুৱ Mat* arrays[] = {এতত্, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক len = (পূর্ণাঙ্ক)(it.size*cn);
    দ্ৱিগুণ r = 0;

    বাবে( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    ঘূৰাই r;
}


#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_dot( প্ৰবেশ_Array _src1, প্ৰবেশ_Array _src2, দ্ৱিগুণ & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    পূর্ণাঙ্ক type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    তার্কিক doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    যদি ( !doubleSupport && depth == CV_64F )
        ঘূৰাই মিছা;

    পূর্ণাঙ্ক dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    পূর্ণাঙ্ক ddepth = std::max(CV_32F, depth);

    পূর্ণাঙ্ক wgs2_aligned = 1;
    যেতিয়ালৈ (wgs2_aligned < (পূর্ণাঙ্ক)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ৱর্ণ cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, পরিমাণ(cvt)),
                         (পূর্ণাঙ্ক)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    যদি (k.empty())
        ঘূৰাই মিছা;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (পূর্ণাঙ্ক)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    যদি (k.run(1, &globalsize, &wgs, সঁচা))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
}

#endif

দ্ৱিগুণ UMat::dot(প্ৰবেশ_Array m) ধ্রুৱ
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*এতত্) && m.type() == type());

#ifdef HAVE_OPENCL
    দ্ৱিগুণ r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*এতত্, m, r), r)
#endif

    ঘূৰাই getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
