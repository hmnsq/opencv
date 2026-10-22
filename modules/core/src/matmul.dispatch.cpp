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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_core.hpp"
#அந்தர்பாவ "intel_gpu_gemm.inl.hpp"

#அந்தர்பாவ "matmul.simd.hpp"
#அந்தர்பாவ "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ifdef HAVE_CLAMDBLAS

நிலையான தருக்க ocl_gemm_amdblas( உள்ளீடு_Array matA, உள்ளீடு_Array matB, இரட்டை alpha,
                      உள்ளீடு_Array matC, இரட்டை beta, OutputArray matD, முழு flags )
{
    முழு type = matA.type(), esz = CV_ELEM_SIZE(type);
    தருக்க haveC = matC.kind() != cv::உள்ளீடு_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    தருக்க atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    எனில் (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    எனில் (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    எனில் (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    எனில் ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        திரும்பு பொய்;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    எனில் (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        திரும்பு பொய்;
    }
    எனில் (haveC)
    {
        UMat C = matC.getUMat();
        எனில் (!ocl::internal::isCLBuffer(C))
            திரும்பு பொய்;
    }
    எனில் (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    இல்லையெனில்
        D.setTo(Scalar::all(0));

    முழு M = sizeD.height, N = sizeD.width, K = sizeA.width;
    முழு lda = (முழு)A.step / esz, ldb = (முழு)B.step / esz, ldc = (முழு)D.step / esz;
    முழு offa = (முழு)A.offset / esz, offb = (முழு)B.offset / esz, offc = (முழு)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    எனில் (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (மாறிலி cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (மாறிலி cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, வெற்று, வெற்று);
    இல்லையெனில் எனில் (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (மாறிலி cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (மாறிலி cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, வெற்று, வெற்று);
    இல்லையெனில் எனில் (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (மாறிலி cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (மாறிலி cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, வெற்று, வெற்று);
    }
    இல்லையெனில் எனில் (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (மாறிலி cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (மாறிலி cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, வெற்று, வெற்று);
    }
    இல்லையெனில்
        CV_Error(Error::StsUnsupportedFormat, "");

    திரும்பு status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
நிலையான தருக்க ocl_gemm( உள்ளீடு_Array matA, உள்ளீடு_Array matB, இரட்டை alpha,
                      உள்ளீடு_Array matC, இரட்டை beta, OutputArray matD, முழு flags )
{
    முழு type = matA.type();
    முழு depth = CV_MAT_DEPTH(type);
    முழு cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    மாறிலி ocl::Device & dev = ocl::Device::getDefault();
    தருக்க doubleSupport = dev.doubleFPConfig() > 0;

    எனில் (!doubleSupport && depth == CV_64F)
        திரும்பு பொய்;

    தருக்க haveC = matC.kind() != cv::உள்ளீடு_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    தருக்க atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    எனில் (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    எனில் (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    எனில் (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    எனில் (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    எனில் (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    தருக்க isPropagatedC2D = பொய்;  // D content is updated with C / C.t()

    எனில் (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        எனில் (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = உண்மை;
        }
        இல்லையெனில்
        {
            beta = 0.0;
        }

        தருக்க res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        எனில் (res)
            திரும்பு உண்மை;
        // fallback on generic OpenCL code
    }

    எனில் (sizeD.width < 8 || sizeD.height < 8)
        திரும்பு பொய்;

    String opts;

    முழு wg_size = (முழு)dev.maxWorkGroupSize();
    முழு sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    முழு block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    எனில் (atrans)
        A = A.t();

    எனில் (btrans)
        B = B.t();

    எனில் (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    முழு vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    முழு kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    எனில் (k.empty())
        திரும்பு பொய்;

    எனில் (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    இல்லையெனில்
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (தசம)alpha, (தசம)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    திரும்பு k.run(2, globalsize, block_size !=1 ? localsize : வெற்று, பொய்);
}
#endif


நாமாகாஶ hal {

வெறுமை gemm32f(மாறிலி தசம* src1, size_t src1_step, மாறிலி தசம* src2, size_t src2_step,
             தசம alpha, மாறிலி தசம* src3, size_t src3_step, தசம beta, தசம* dst, size_t dst_step,
             முழு m_a, முழு n_a, முழு n_d, முழு flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#இல்லையெனில்
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

வெறுமை gemm64f(மாறிலி இரட்டை* src1, size_t src1_step, மாறிலி இரட்டை* src2, size_t src2_step,
             இரட்டை alpha, மாறிலி இரட்டை* src3, size_t src3_step, இரட்டை beta, இரட்டை* dst, size_t dst_step,
             முழு m_a, முழு n_a, முழு n_d, முழு flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#இல்லையெனில்
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

வெறுமை gemm32fc(மாறிலி தசம* src1, size_t src1_step, மாறிலி தசம* src2, size_t src2_step,
              தசம alpha, மாறிலி தசம* src3, size_t src3_step, தசம beta, தசம* dst, size_t dst_step,
              முழு m_a, முழு n_a, முழு n_d, முழு flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#இல்லையெனில்
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

வெறுமை gemm64fc(மாறிலி இரட்டை* src1, size_t src1_step, மாறிலி இரட்டை* src2, size_t src2_step,
              இரட்டை alpha, மாறிலி இரட்டை* src3, size_t src3_step, இரட்டை beta, இரட்டை* dst, size_t dst_step,
              முழு m_a, முழு n_a, முழு n_d, முழு flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#இல்லையெனில்
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

வெறுமை gemm(உள்ளீடு_Array matA, உள்ளீடு_Array matB, இரட்டை alpha,
          உள்ளீடு_Array matC, இரட்டை beta, OutputArray _matD, முழு flags)
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
    முழு len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    தேர்வு( flags & (GEMM_1_T|GEMM_2_T) )
    {
    நிலை 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        நிறுத்து;
    நிலை 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        நிறுத்து;
    நிலை 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        நிறுத்து;
    நிலை 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        நிறுத்து;
    }

    எனில்( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    எனில்( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    எனில்( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    எனில்( type == CV_32FC1 )
        hal::gemm32f(A.ptr<தசம>(), A.step, B.ptr<தசம>(), B.step, static_cast<தசம>(alpha),
                     C.ptr<தசம>(), C.step, static_cast<தசம>(beta),
                     DProxyPtr->ptr<தசம>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    இல்லையெனில் எனில்( type == CV_64FC1 )
        hal::gemm64f(A.ptr<இரட்டை>(), A.step, B.ptr<இரட்டை>(), B.step, alpha,
                     C.ptr<இரட்டை>(), C.step, beta,
                     DProxyPtr->ptr<இரட்டை>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    இல்லையெனில் எனில்( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<தசம>(), A.step, B.ptr<தசம>(), B.step, static_cast<தசம>(alpha),
                      C.ptr<தசம>(), C.step, static_cast<தசம>(beta),
                      DProxyPtr->ptr<தசம>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    இல்லையெனில்
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<இரட்டை>(), A.step, B.ptr<இரட்டை>(), B.step, alpha,
                      C.ptr<இரட்டை>(), C.step, beta,
                      DProxyPtr->ptr<இரட்டை>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    எனில்(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

நிலையான TransformFunc getTransformFunc(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

நிலையான TransformFunc getDiagTransformFunc(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை transform(உள்ளீடு_Array _src, OutputArray _dst, உள்ளீடு_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    முழு depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    தருக்க isDiag = பொய்;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    எனில் (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    முழு mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<இரட்டை> _mbuf;
    இரட்டை* mbuf;

    எனில்( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        எனில்( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        இல்லையெனில்
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    இல்லையெனில்
        mbuf = m.ptr<இரட்டை>();

    எனில்( scn == dcn )
    {
        முழு i, j;
        இரட்டை eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        எனில்( scn == 1 )
        {
            இரட்டை alpha, beta;
            எனில்( mtype == CV_32F )
                alpha = m.at<தசம>(0), beta = m.at<தசம>(1);
            இல்லையெனில்
                alpha = m.at<இரட்டை>(0), beta = m.at<இரட்டை>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            திரும்பு;
        }

        க்கு( i = 0, isDiag = உண்மை; isDiag && i < scn; i++ )
        {
            க்கு( j = 0; isDiag && j < scn; j++ )
            {
                இரட்டை v = mtype == CV_32F ? m.at<தசம>(i, j) : m.at<இரட்டை>(i, j);
                எனில்( i != j && fabs(v) > eps )
                    isDiag = பொய்;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    மாறிலி Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    க்கு( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (முழு)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

நிலையான TransformFunc getPerspectiveTransform(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை perspectiveTransform(உள்ளீடு_Array _src, OutputArray _dst, உள்ளீடு_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    முழு depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    மாறிலி முழு mtype = CV_64F;
    AutoBuffer<இரட்டை> _mbuf;
    இரட்டை* mbuf = m.ptr<இரட்டை>();

    எனில்( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    மாறிலி Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    க்கு( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (முழு)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_scaleAdd( உள்ளீடு_Array _src1, இரட்டை alpha, உள்ளீடு_Array _src2, OutputArray _dst, முழு type )
{
    மாறிலி ocl::Device & d = ocl::Device::getDefault();

    தருக்க doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    முழு depth = CV_MAT_DEPTH(type);
    எனில் ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        திரும்பு பொய்;

    _dst.create(size, type);
    முழு cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    முழு kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    எழுத்து cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], அளவு(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], அளவு(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    எனில் (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (தசம)alpha);
    இல்லையெனில்
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

#endif

நிலையான ScaleAddFunc getScaleAddFunc(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

வெறுமை scaleAdd(உள்ளீடு_Array _src1, இரட்டை alpha, உள்ளீடு_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    முழு type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    எனில்( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        திரும்பு;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    தசம falpha = (தசம)alpha;
    வெறுமை* palpha = depth == CV_32F ? (வெறுமை*)&falpha : (வெறுமை*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    எனில் (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (முழு)len, palpha);
        திரும்பு;
    }

    மாறிலி Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    க்கு( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (முழு)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

வெறுமை calcCovarMatrix( மாறிலி Mat* data, முழு nsamples, Mat& covar, Mat& _mean, முழு flags, முழு ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    முழு sz = size.width * size.height, esz = (முழு)data[0].elemSize();
    முழு type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    எனில்( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        எனில்( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        இல்லையெனில்
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    க்கு( முழு i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        எனில்( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        இல்லையெனில்
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    எனில்( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

வெறுமை calcCovarMatrix( உள்ளீடு_Array _src, OutputArray _covar, உள்ளீடு_Output_Array _mean, முழு flags, முழு ctype )
{
    CV_INSTRUMENT_REGION();

    எனில்(_src.kind() == உள்ளீடு_Array_2::STD_VECTOR_MAT || _src.kind() == உள்ளீடு_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        முழு type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<முழு>(src.size()), size.area(), type);

        முழு i = 0;
        க்கு(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        எனில்( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            எனில்( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            இல்லையெனில்
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        எனில்( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        திரும்பு;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    தருக்க takeRows = (flags & cv::COVAR_ROWS) != 0;
    முழு type = data.type();
    முழு nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    எனில்( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        எனில்( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    இல்லையெனில்
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

நிலையான MahalanobisImplFunc getMahalanobisImplFunc(முழு depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#இல்லையெனில்
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


இரட்டை Mahalanobis(உள்ளீடு_Array _v1, உள்ளீடு_Array _v2, உள்ளீடு_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    முழு type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    முழு len = sz.width*sz.height*v1.channels();
    AutoBuffer<இரட்டை> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    எனில்( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    இரட்டை result = func(v1, v2, icovar, buf.data(), len);
    திரும்பு std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

நிலையான MulTransposedFunc getMulTransposedFunc(முழு stype, முழு dtype, தருக்க ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#இல்லையெனில்
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

வெறுமை mulTransposed(உள்ளீடு_Array _src, OutputArray _dst, தருக்க ata,
                   உள்ளீடு_Array _delta, இரட்டை scale, முழு dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    மாறிலி முழு gemm_level = 100; // boundary above which GEMM is faster.
    முழு stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    எனில்( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        எனில்( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    முழு dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    எனில்( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        மாறிலி Mat* tsrc = &src;
        எனில்( !delta.empty() )
        {
            எனில்( delta.size() == src.size() )
                subtract( src, delta, src2 );
            இல்லையெனில்
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    இல்லையெனில்
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        எனில்( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, பொய் );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

நிலையான இரட்டை dotProd_8u(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((மாறிலி uchar*)src1, (மாறிலி uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
நிலையான இரட்டை dotProd_8s(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((மாறிலி schar*)src1, (மாறிலி schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
நிலையான இரட்டை dotProd_16u(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((மாறிலி ushort*)src1, (மாறிலி ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
நிலையான இரட்டை dotProd_16s(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((மாறிலி short*)src1, (மாறிலி short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
நிலையான இரட்டை dotProd_32s(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((மாறிலி முழு*)src1, (மாறிலி முழு*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
நிலையான இரட்டை dotProd_32f(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((மாறிலி தசம*)src1, (மாறிலி தசம*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
நிலையான இரட்டை dotProd_64f(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, முழு len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((மாறிலி இரட்டை*)src1, (மாறிலி இரட்டை*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

வகைவரை இரட்டை (*DotProdFunc)(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, முழு len);

நிலையான DotProdFunc getDotProdFunc(முழு depth)
{
    நிலையான DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    திரும்பு dotProdTab[depth];
}

இரட்டை Mat::dot(உள்ளீடு_Array _mat) மாறிலி
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    முழு cn = channels();
    எனில் (இது->dims <= 2)
    {
        இரட்டை product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, இது->data, இது->step, mat.data, mat.step,
                     இது->cols * cn, இது->rows, இது->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    எனில்( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        எனில்( len == (size_t)(முழு)len )
            திரும்பு func(data, mat.data, (முழு)len);
    }

    மாறிலி Mat* arrays[] = {இது, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு len = (முழு)(it.size*cn);
    இரட்டை r = 0;

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    திரும்பு r;
}


#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_dot( உள்ளீடு_Array _src1, உள்ளீடு_Array _src2, இரட்டை & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    முழு type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    தருக்க doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    எனில் ( !doubleSupport && depth == CV_64F )
        திரும்பு பொய்;

    முழு dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    முழு ddepth = std::max(CV_32F, depth);

    முழு wgs2_aligned = 1;
    போது (wgs2_aligned < (முழு)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    எழுத்து cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, அளவு(cvt)),
                         (முழு)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (முழு)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    எனில் (k.run(1, &globalsize, &wgs, உண்மை))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        திரும்பு உண்மை;
    }
    திரும்பு பொய்;
}

#endif

இரட்டை UMat::dot(உள்ளீடு_Array m) மாறிலி
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*இது) && m.type() == type());

#ifdef HAVE_OPENCL
    இரட்டை r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*இது, m, r), r)
#endif

    திரும்பு getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
