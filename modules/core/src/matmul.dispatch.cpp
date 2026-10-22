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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#અન્તર્ભાવ "opencv2/core/opencl/runtime/opencl_core.hpp"
#અન્તર્ભાવ "intel_gpu_gemm.inl.hpp"

#અન્તર્ભાવ "matmul.simd.hpp"
#અન્તર્ભાવ "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ifdef HAVE_CLAMDBLAS

સ્થિર તાર્કિક ocl_gemm_amdblas( આગત_Array matA, આગત_Array matB, દ્વિગુણ alpha,
                      આગત_Array matC, દ્વિગુણ beta, OutputArray matD, પૂર્ણાંક flags )
{
    પૂર્ણાંક type = matA.type(), esz = CV_ELEM_SIZE(type);
    તાર્કિક haveC = matC.kind() != cv::આગત_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    તાર્કિક atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    જો (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    જો (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    જો (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    જો ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        પાછા ખોટું;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    જો (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        પાછા ખોટું;
    }
    જો (haveC)
    {
        UMat C = matC.getUMat();
        જો (!ocl::internal::isCLBuffer(C))
            પાછા ખોટું;
    }
    જો (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    નહીંતર
        D.setTo(Scalar::all(0));

    પૂર્ણાંક M = sizeD.height, N = sizeD.width, K = sizeA.width;
    પૂર્ણાંક lda = (પૂર્ણાંક)A.step / esz, ldb = (પૂર્ણાંક)B.step / esz, ldc = (પૂર્ણાંક)D.step / esz;
    પૂર્ણાંક offa = (પૂર્ણાંક)A.offset / esz, offb = (પૂર્ણાંક)B.offset / esz, offc = (પૂર્ણાંક)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    જો (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (અચલ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (અચલ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ખાલી, ખાલી);
    નહીંતર જો (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (અચલ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (અચલ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ખાલી, ખાલી);
    નહીંતર જો (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (અચલ cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (અચલ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, ખાલી, ખાલી);
    }
    નહીંતર જો (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (અચલ cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (અચલ cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, ખાલી, ખાલી);
    }
    નહીંતર
        CV_Error(Error::StsUnsupportedFormat, "");

    પાછા status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
સ્થિર તાર્કિક ocl_gemm( આગત_Array matA, આગત_Array matB, દ્વિગુણ alpha,
                      આગત_Array matC, દ્વિગુણ beta, OutputArray matD, પૂર્ણાંક flags )
{
    પૂર્ણાંક type = matA.type();
    પૂર્ણાંક depth = CV_MAT_DEPTH(type);
    પૂર્ણાંક cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    અચલ ocl::Device & dev = ocl::Device::getDefault();
    તાર્કિક doubleSupport = dev.doubleFPConfig() > 0;

    જો (!doubleSupport && depth == CV_64F)
        પાછા ખોટું;

    તાર્કિક haveC = matC.kind() != cv::આગત_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    તાર્કિક atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    જો (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    જો (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    જો (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    જો (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    જો (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    તાર્કિક isPropagatedC2D = ખોટું;  // D content is updated with C / C.t()

    જો (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        જો (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = સાચું;
        }
        નહીંતર
        {
            beta = 0.0;
        }

        તાર્કિક res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        જો (res)
            પાછા સાચું;
        // fallback on generic OpenCL code
    }

    જો (sizeD.width < 8 || sizeD.height < 8)
        પાછા ખોટું;

    String opts;

    પૂર્ણાંક wg_size = (પૂર્ણાંક)dev.maxWorkGroupSize();
    પૂર્ણાંક sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    પૂર્ણાંક block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    જો (atrans)
        A = A.t();

    જો (btrans)
        B = B.t();

    જો (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    પૂર્ણાંક vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    પૂર્ણાંક kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    જો (k.empty())
        પાછા ખોટું;

    જો (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    નહીંતર
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (દશાંશ)alpha, (દશાંશ)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    પાછા k.run(2, globalsize, block_size !=1 ? localsize : ખાલી, ખોટું);
}
#endif


નામાકાશ hal {

શૂન્ય gemm32f(અચલ દશાંશ* src1, size_t src1_step, અચલ દશાંશ* src2, size_t src2_step,
             દશાંશ alpha, અચલ દશાંશ* src3, size_t src3_step, દશાંશ beta, દશાંશ* dst, size_t dst_step,
             પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#નહીંતર
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

શૂન્ય gemm64f(અચલ દ્વિગુણ* src1, size_t src1_step, અચલ દ્વિગુણ* src2, size_t src2_step,
             દ્વિગુણ alpha, અચલ દ્વિગુણ* src3, size_t src3_step, દ્વિગુણ beta, દ્વિગુણ* dst, size_t dst_step,
             પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#નહીંતર
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

શૂન્ય gemm32fc(અચલ દશાંશ* src1, size_t src1_step, અચલ દશાંશ* src2, size_t src2_step,
              દશાંશ alpha, અચલ દશાંશ* src3, size_t src3_step, દશાંશ beta, દશાંશ* dst, size_t dst_step,
              પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#નહીંતર
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

શૂન્ય gemm64fc(અચલ દ્વિગુણ* src1, size_t src1_step, અચલ દ્વિગુણ* src2, size_t src2_step,
              દ્વિગુણ alpha, અચલ દ્વિગુણ* src3, size_t src3_step, દ્વિગુણ beta, દ્વિગુણ* dst, size_t dst_step,
              પૂર્ણાંક m_a, પૂર્ણાંક n_a, પૂર્ણાંક n_d, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#નહીંતર
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

શૂન્ય gemm(આગત_Array matA, આગત_Array matB, દ્વિગુણ alpha,
          આગત_Array matC, દ્વિગુણ beta, OutputArray _matD, પૂર્ણાંક flags)
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
    પૂર્ણાંક len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    પસંદગી( flags & (GEMM_1_T|GEMM_2_T) )
    {
    સ્થિતિ 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        તોડો;
    સ્થિતિ 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        તોડો;
    સ્થિતિ 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        તોડો;
    સ્થિતિ 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        તોડો;
    }

    જો( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    જો( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    જો( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    જો( type == CV_32FC1 )
        hal::gemm32f(A.ptr<દશાંશ>(), A.step, B.ptr<દશાંશ>(), B.step, static_cast<દશાંશ>(alpha),
                     C.ptr<દશાંશ>(), C.step, static_cast<દશાંશ>(beta),
                     DProxyPtr->ptr<દશાંશ>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    નહીંતર જો( type == CV_64FC1 )
        hal::gemm64f(A.ptr<દ્વિગુણ>(), A.step, B.ptr<દ્વિગુણ>(), B.step, alpha,
                     C.ptr<દ્વિગુણ>(), C.step, beta,
                     DProxyPtr->ptr<દ્વિગુણ>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    નહીંતર જો( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<દશાંશ>(), A.step, B.ptr<દશાંશ>(), B.step, static_cast<દશાંશ>(alpha),
                      C.ptr<દશાંશ>(), C.step, static_cast<દશાંશ>(beta),
                      DProxyPtr->ptr<દશાંશ>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    નહીંતર
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<દ્વિગુણ>(), A.step, B.ptr<દ્વિગુણ>(), B.step, alpha,
                      C.ptr<દ્વિગુણ>(), C.step, beta,
                      DProxyPtr->ptr<દ્વિગુણ>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    જો(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

સ્થિર TransformFunc getTransformFunc(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

સ્થિર TransformFunc getDiagTransformFunc(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય transform(આગત_Array _src, OutputArray _dst, આગત_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    પૂર્ણાંક depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    તાર્કિક isDiag = ખોટું;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    જો (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    પૂર્ણાંક mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<દ્વિગુણ> _mbuf;
    દ્વિગુણ* mbuf;

    જો( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        જો( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        નહીંતર
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    નહીંતર
        mbuf = m.ptr<દ્વિગુણ>();

    જો( scn == dcn )
    {
        પૂર્ણાંક i, j;
        દ્વિગુણ eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        જો( scn == 1 )
        {
            દ્વિગુણ alpha, beta;
            જો( mtype == CV_32F )
                alpha = m.at<દશાંશ>(0), beta = m.at<દશાંશ>(1);
            નહીંતર
                alpha = m.at<દ્વિગુણ>(0), beta = m.at<દ્વિગુણ>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            પાછા;
        }

        માટે( i = 0, isDiag = સાચું; isDiag && i < scn; i++ )
        {
            માટે( j = 0; isDiag && j < scn; j++ )
            {
                દ્વિગુણ v = mtype == CV_32F ? m.at<દશાંશ>(i, j) : m.at<દ્વિગુણ>(i, j);
                જો( i != j && fabs(v) > eps )
                    isDiag = ખોટું;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    અચલ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    માટે( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (પૂર્ણાંક)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

સ્થિર TransformFunc getPerspectiveTransform(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય perspectiveTransform(આગત_Array _src, OutputArray _dst, આગત_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    પૂર્ણાંક depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    અચલ પૂર્ણાંક mtype = CV_64F;
    AutoBuffer<દ્વિગુણ> _mbuf;
    દ્વિગુણ* mbuf = m.ptr<દ્વિગુણ>();

    જો( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    અચલ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    માટે( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (પૂર્ણાંક)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_scaleAdd( આગત_Array _src1, દ્વિગુણ alpha, આગત_Array _src2, OutputArray _dst, પૂર્ણાંક type )
{
    અચલ ocl::Device & d = ocl::Device::getDefault();

    તાર્કિક doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    પૂર્ણાંક depth = CV_MAT_DEPTH(type);
    જો ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        પાછા ખોટું;

    _dst.create(size, type);
    પૂર્ણાંક cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    પૂર્ણાંક kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    અક્ષર cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], કદ(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], કદ(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    જો (k.empty())
        પાછા ખોટું;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    જો (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (દશાંશ)alpha);
    નહીંતર
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

#endif

સ્થિર ScaleAddFunc getScaleAddFunc(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

શૂન્ય scaleAdd(આગત_Array _src1, દ્વિગુણ alpha, આગત_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    જો( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        પાછા;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    દશાંશ falpha = (દશાંશ)alpha;
    શૂન્ય* palpha = depth == CV_32F ? (શૂન્ય*)&falpha : (શૂન્ય*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    જો (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (પૂર્ણાંક)len, palpha);
        પાછા;
    }

    અચલ Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    માટે( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (પૂર્ણાંક)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

શૂન્ય calcCovarMatrix( અચલ Mat* data, પૂર્ણાંક nsamples, Mat& covar, Mat& _mean, પૂર્ણાંક flags, પૂર્ણાંક ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    પૂર્ણાંક sz = size.width * size.height, esz = (પૂર્ણાંક)data[0].elemSize();
    પૂર્ણાંક type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    જો( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        જો( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        નહીંતર
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    માટે( પૂર્ણાંક i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        જો( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        નહીંતર
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    જો( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

શૂન્ય calcCovarMatrix( આગત_Array _src, OutputArray _covar, આગત_Output_Array _mean, પૂર્ણાંક flags, પૂર્ણાંક ctype )
{
    CV_INSTRUMENT_REGION();

    જો(_src.kind() == આગત_Array_2::STD_VECTOR_MAT || _src.kind() == આગત_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        પૂર્ણાંક type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<પૂર્ણાંક>(src.size()), size.area(), type);

        પૂર્ણાંક i = 0;
        માટે(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        જો( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            જો( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            નહીંતર
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        જો( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        પાછા;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    તાર્કિક takeRows = (flags & cv::COVAR_ROWS) != 0;
    પૂર્ણાંક type = data.type();
    પૂર્ણાંક nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    જો( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        જો( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    નહીંતર
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

સ્થિર MahalanobisImplFunc getMahalanobisImplFunc(પૂર્ણાંક depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#નહીંતર
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


દ્વિગુણ Mahalanobis(આગત_Array _v1, આગત_Array _v2, આગત_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    પૂર્ણાંક type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    પૂર્ણાંક len = sz.width*sz.height*v1.channels();
    AutoBuffer<દ્વિગુણ> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    જો( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    દ્વિગુણ result = func(v1, v2, icovar, buf.data(), len);
    પાછા std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

સ્થિર MulTransposedFunc getMulTransposedFunc(પૂર્ણાંક stype, પૂર્ણાંક dtype, તાર્કિક ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#નહીંતર
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

શૂન્ય mulTransposed(આગત_Array _src, OutputArray _dst, તાર્કિક ata,
                   આગત_Array _delta, દ્વિગુણ scale, પૂર્ણાંક dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    અચલ પૂર્ણાંક gemm_level = 100; // boundary above which GEMM is faster.
    પૂર્ણાંક stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    જો( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        જો( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    પૂર્ણાંક dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    જો( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        અચલ Mat* tsrc = &src;
        જો( !delta.empty() )
        {
            જો( delta.size() == src.size() )
                subtract( src, delta, src2 );
            નહીંતર
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    નહીંતર
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        જો( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, ખોટું );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

સ્થિર દ્વિગુણ dotProd_8u(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((અચલ uchar*)src1, (અચલ uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
સ્થિર દ્વિગુણ dotProd_8s(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((અચલ schar*)src1, (અચલ schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
સ્થિર દ્વિગુણ dotProd_16u(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((અચલ ushort*)src1, (અચલ ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
સ્થિર દ્વિગુણ dotProd_16s(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((અચલ short*)src1, (અચલ short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
સ્થિર દ્વિગુણ dotProd_32s(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((અચલ પૂર્ણાંક*)src1, (અચલ પૂર્ણાંક*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
સ્થિર દ્વિગુણ dotProd_32f(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((અચલ દશાંશ*)src1, (અચલ દશાંશ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
સ્થિર દ્વિગુણ dotProd_64f(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, પૂર્ણાંક len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((અચલ દ્વિગુણ*)src1, (અચલ દ્વિગુણ*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

પ્રકારવ્યાખ્યા દ્વિગુણ (*DotProdFunc)(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, પૂર્ણાંક len);

સ્થિર DotProdFunc getDotProdFunc(પૂર્ણાંક depth)
{
    સ્થિર DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    પાછા dotProdTab[depth];
}

દ્વિગુણ Mat::dot(આગત_Array _mat) અચલ
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    પૂર્ણાંક cn = channels();
    જો (આ->dims <= 2)
    {
        દ્વિગુણ product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, આ->data, આ->step, mat.data, mat.step,
                     આ->cols * cn, આ->rows, આ->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    જો( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        જો( len == (size_t)(પૂર્ણાંક)len )
            પાછા func(data, mat.data, (પૂર્ણાંક)len);
    }

    અચલ Mat* arrays[] = {આ, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    પૂર્ણાંક len = (પૂર્ણાંક)(it.size*cn);
    દ્વિગુણ r = 0;

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    પાછા r;
}


#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_dot( આગત_Array _src1, આગત_Array _src2, દ્વિગુણ & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    પૂર્ણાંક type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    તાર્કિક doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    જો ( !doubleSupport && depth == CV_64F )
        પાછા ખોટું;

    પૂર્ણાંક dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    પૂર્ણાંક ddepth = std::max(CV_32F, depth);

    પૂર્ણાંક wgs2_aligned = 1;
    જ્યારે (wgs2_aligned < (પૂર્ણાંક)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    અક્ષર cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, કદ(cvt)),
                         (પૂર્ણાંક)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    જો (k.empty())
        પાછા ખોટું;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (પૂર્ણાંક)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    જો (k.run(1, &globalsize, &wgs, સાચું))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        પાછા સાચું;
    }
    પાછા ખોટું;
}

#endif

દ્વિગુણ UMat::dot(આગત_Array m) અચલ
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*આ) && m.type() == type());

#ifdef HAVE_OPENCL
    દ્વિગુણ r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*આ, m, r), r)
#endif

    પાછા getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
