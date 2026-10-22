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

فضاینام cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ifdef HAVE_CLAMDBLAS

ایستا منطقی ocl_gemm_amdblas( درون_داد_Array matA, درون_داد_Array matB, دوگانه alpha,
                      درون_داد_Array matC, دوگانه beta, OutputArray matD, صحیح flags )
{
    صحیح type = matA.type(), esz = CV_ELEM_SIZE(type);
    منطقی haveC = matC.kind() != cv::درون_داد_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    منطقی atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    اگر (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    اگر (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    اگر (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    اگر ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        بازگشت نادرست;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    اگر (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        بازگشت نادرست;
    }
    اگر (haveC)
    {
        UMat C = matC.getUMat();
        اگر (!ocl::internal::isCLBuffer(C))
            بازگشت نادرست;
    }
    اگر (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    وگرنه
        D.setTo(Scalar::all(0));

    صحیح M = sizeD.height, N = sizeD.width, K = sizeA.width;
    صحیح lda = (صحیح)A.step / esz, ldb = (صحیح)B.step / esz, ldc = (صحیح)D.step / esz;
    صحیح offa = (صحیح)A.offset / esz, offb = (صحیح)B.offset / esz, offc = (صحیح)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    اگر (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, پوچ, پوچ);
    وگرنه اگر (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, پوچ, پوچ);
    وگرنه اگر (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, پوچ, پوچ);
    }
    وگرنه اگر (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, پوچ, پوچ);
    }
    وگرنه
        CV_Error(Error::StsUnsupportedFormat, "");

    بازگشت status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
ایستا منطقی ocl_gemm( درون_داد_Array matA, درون_داد_Array matB, دوگانه alpha,
                      درون_داد_Array matC, دوگانه beta, OutputArray matD, صحیح flags )
{
    صحیح type = matA.type();
    صحیح depth = CV_MAT_DEPTH(type);
    صحیح cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقی doubleSupport = dev.doubleFPConfig() > 0;

    اگر (!doubleSupport && depth == CV_64F)
        بازگشت نادرست;

    منطقی haveC = matC.kind() != cv::درون_داد_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    منطقی atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    اگر (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    اگر (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    اگر (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    اگر (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    اگر (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    منطقی isPropagatedC2D = نادرست;  // D content is updated with C / C.t()

    اگر (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        اگر (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = درست;
        }
        وگرنه
        {
            beta = 0.0;
        }

        منطقی res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        اگر (res)
            بازگشت درست;
        // fallback on generic OpenCL code
    }

    اگر (sizeD.width < 8 || sizeD.height < 8)
        بازگشت نادرست;

    String opts;

    صحیح wg_size = (صحیح)dev.maxWorkGroupSize();
    صحیح sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    صحیح block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    اگر (atrans)
        A = A.t();

    اگر (btrans)
        B = B.t();

    اگر (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    صحیح vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    صحیح kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    اگر (k.empty())
        بازگشت نادرست;

    اگر (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    وگرنه
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (اعشاری)alpha, (اعشاری)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    بازگشت k.run(2, globalsize, block_size !=1 ? localsize : پوچ, نادرست);
}
#endif


فضاینام hal {

تهی gemm32f(ثابت اعشاری* src1, size_t src1_step, ثابت اعشاری* src2, size_t src2_step,
             اعشاری alpha, ثابت اعشاری* src3, size_t src3_step, اعشاری beta, اعشاری* dst, size_t dst_step,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#وگرنه
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

تهی gemm64f(ثابت دوگانه* src1, size_t src1_step, ثابت دوگانه* src2, size_t src2_step,
             دوگانه alpha, ثابت دوگانه* src3, size_t src3_step, دوگانه beta, دوگانه* dst, size_t dst_step,
             صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#وگرنه
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

تهی gemm32fc(ثابت اعشاری* src1, size_t src1_step, ثابت اعشاری* src2, size_t src2_step,
              اعشاری alpha, ثابت اعشاری* src3, size_t src3_step, اعشاری beta, اعشاری* dst, size_t dst_step,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#وگرنه
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

تهی gemm64fc(ثابت دوگانه* src1, size_t src1_step, ثابت دوگانه* src2, size_t src2_step,
              دوگانه alpha, ثابت دوگانه* src3, size_t src3_step, دوگانه beta, دوگانه* dst, size_t dst_step,
              صحیح m_a, صحیح n_a, صحیح n_d, صحیح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#وگرنه
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

تهی gemm(درون_داد_Array matA, درون_داد_Array matB, دوگانه alpha,
          درون_داد_Array matC, دوگانه beta, OutputArray _matD, صحیح flags)
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
    صحیح len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    انتخاب( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالت 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        بشکن;
    حالت 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        بشکن;
    حالت 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        بشکن;
    حالت 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        بشکن;
    }

    اگر( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    اگر( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    اگر( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    اگر( type == CV_32FC1 )
        hal::gemm32f(A.ptr<اعشاری>(), A.step, B.ptr<اعشاری>(), B.step, static_cast<اعشاری>(alpha),
                     C.ptr<اعشاری>(), C.step, static_cast<اعشاری>(beta),
                     DProxyPtr->ptr<اعشاری>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    وگرنه اگر( type == CV_64FC1 )
        hal::gemm64f(A.ptr<دوگانه>(), A.step, B.ptr<دوگانه>(), B.step, alpha,
                     C.ptr<دوگانه>(), C.step, beta,
                     DProxyPtr->ptr<دوگانه>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    وگرنه اگر( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<اعشاری>(), A.step, B.ptr<اعشاری>(), B.step, static_cast<اعشاری>(alpha),
                      C.ptr<اعشاری>(), C.step, static_cast<اعشاری>(beta),
                      DProxyPtr->ptr<اعشاری>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    وگرنه
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<دوگانه>(), A.step, B.ptr<دوگانه>(), B.step, alpha,
                      C.ptr<دوگانه>(), C.step, beta,
                      DProxyPtr->ptr<دوگانه>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    اگر(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ایستا TransformFunc getTransformFunc(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ایستا TransformFunc getDiagTransformFunc(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی transform(درون_داد_Array _src, OutputArray _dst, درون_داد_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحیح depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    منطقی isDiag = نادرست;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    اگر (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    صحیح mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<دوگانه> _mbuf;
    دوگانه* mbuf;

    اگر( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        اگر( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        وگرنه
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    وگرنه
        mbuf = m.ptr<دوگانه>();

    اگر( scn == dcn )
    {
        صحیح i, j;
        دوگانه eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        اگر( scn == 1 )
        {
            دوگانه alpha, beta;
            اگر( mtype == CV_32F )
                alpha = m.at<اعشاری>(0), beta = m.at<اعشاری>(1);
            وگرنه
                alpha = m.at<دوگانه>(0), beta = m.at<دوگانه>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            بازگشت;
        }

        برای( i = 0, isDiag = درست; isDiag && i < scn; i++ )
        {
            برای( j = 0; isDiag && j < scn; j++ )
            {
                دوگانه v = mtype == CV_32F ? m.at<اعشاری>(i, j) : m.at<دوگانه>(i, j);
                اگر( i != j && fabs(v) > eps )
                    isDiag = نادرست;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    برای( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحیح)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ایستا TransformFunc getPerspectiveTransform(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی perspectiveTransform(درون_داد_Array _src, OutputArray _dst, درون_داد_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحیح depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ثابت صحیح mtype = CV_64F;
    AutoBuffer<دوگانه> _mbuf;
    دوگانه* mbuf = m.ptr<دوگانه>();

    اگر( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    برای( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحیح)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

ایستا منطقی ocl_scaleAdd( درون_داد_Array _src1, دوگانه alpha, درون_داد_Array _src2, OutputArray _dst, صحیح type )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    منطقی doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    صحیح depth = CV_MAT_DEPTH(type);
    اگر ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        بازگشت نادرست;

    _dst.create(size, type);
    صحیح cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    صحیح kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    نویسه cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], اندازه(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], اندازه(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    اگر (k.empty())
        بازگشت نادرست;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    اگر (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (اعشاری)alpha);
    وگرنه
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

#endif

ایستا ScaleAddFunc getScaleAddFunc(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

تهی scaleAdd(درون_داد_Array _src1, دوگانه alpha, درون_داد_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    اگر( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        بازگشت;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    اعشاری falpha = (اعشاری)alpha;
    تهی* palpha = depth == CV_32F ? (تهی*)&falpha : (تهی*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    اگر (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (صحیح)len, palpha);
        بازگشت;
    }

    ثابت Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    برای( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (صحیح)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

تهی calcCovarMatrix( ثابت Mat* data, صحیح nsamples, Mat& covar, Mat& _mean, صحیح flags, صحیح ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    صحیح sz = size.width * size.height, esz = (صحیح)data[0].elemSize();
    صحیح type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    اگر( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        اگر( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        وگرنه
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    برای( صحیح i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        اگر( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        وگرنه
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    اگر( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

تهی calcCovarMatrix( درون_داد_Array _src, OutputArray _covar, درون_داد_Output_Array _mean, صحیح flags, صحیح ctype )
{
    CV_INSTRUMENT_REGION();

    اگر(_src.kind() == درون_داد_Array_2::STD_VECTOR_MAT || _src.kind() == درون_داد_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        صحیح type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<صحیح>(src.size()), size.area(), type);

        صحیح i = 0;
        برای(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        اگر( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            اگر( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            وگرنه
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        اگر( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        بازگشت;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    منطقی takeRows = (flags & cv::COVAR_ROWS) != 0;
    صحیح type = data.type();
    صحیح nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    اگر( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        اگر( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    وگرنه
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

ایستا MahalanobisImplFunc getMahalanobisImplFunc(صحیح depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#وگرنه
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


دوگانه Mahalanobis(درون_داد_Array _v1, درون_داد_Array _v2, درون_داد_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    صحیح type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    صحیح len = sz.width*sz.height*v1.channels();
    AutoBuffer<دوگانه> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    اگر( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    دوگانه result = func(v1, v2, icovar, buf.data(), len);
    بازگشت std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ایستا MulTransposedFunc getMulTransposedFunc(صحیح stype, صحیح dtype, منطقی ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#وگرنه
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

تهی mulTransposed(درون_داد_Array _src, OutputArray _dst, منطقی ata,
                   درون_داد_Array _delta, دوگانه scale, صحیح dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ثابت صحیح gemm_level = 100; // boundary above which GEMM is faster.
    صحیح stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    اگر( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        اگر( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    صحیح dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    اگر( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        ثابت Mat* tsrc = &src;
        اگر( !delta.empty() )
        {
            اگر( delta.size() == src.size() )
                subtract( src, delta, src2 );
            وگرنه
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    وگرنه
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        اگر( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, نادرست );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

ایستا دوگانه dotProd_8u(ثابت تهی* src1, ثابت تهی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((ثابت uchar*)src1, (ثابت uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ایستا دوگانه dotProd_8s(ثابت تهی* src1, ثابت تهی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((ثابت schar*)src1, (ثابت schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ایستا دوگانه dotProd_16u(ثابت تهی* src1, ثابت تهی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((ثابت ushort*)src1, (ثابت ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ایستا دوگانه dotProd_16s(ثابت تهی* src1, ثابت تهی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((ثابت short*)src1, (ثابت short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ایستا دوگانه dotProd_32s(ثابت تهی* src1, ثابت تهی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((ثابت صحیح*)src1, (ثابت صحیح*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ایستا دوگانه dotProd_32f(ثابت تهی* src1, ثابت تهی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((ثابت اعشاری*)src1, (ثابت اعشاری*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
ایستا دوگانه dotProd_64f(ثابت تهی* src1, ثابت تهی* src2, صحیح len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((ثابت دوگانه*)src1, (ثابت دوگانه*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

تعریفنوع دوگانه (*DotProdFunc)(ثابت تهی* src1, ثابت تهی* src2, صحیح len);

ایستا DotProdFunc getDotProdFunc(صحیح depth)
{
    ایستا DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    بازگشت dotProdTab[depth];
}

دوگانه Mat::dot(درون_داد_Array _mat) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    صحیح cn = channels();
    اگر (این->dims <= 2)
    {
        دوگانه product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, این->data, این->step, mat.data, mat.step,
                     این->cols * cn, این->rows, این->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    اگر( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        اگر( len == (size_t)(صحیح)len )
            بازگشت func(data, mat.data, (صحیح)len);
    }

    ثابت Mat* arrays[] = {این, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح len = (صحیح)(it.size*cn);
    دوگانه r = 0;

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    بازگشت r;
}


#ifdef HAVE_OPENCL

ایستا منطقی ocl_dot( درون_داد_Array _src1, درون_داد_Array _src2, دوگانه & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    صحیح type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    اگر ( !doubleSupport && depth == CV_64F )
        بازگشت نادرست;

    صحیح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    صحیح ddepth = std::max(CV_32F, depth);

    صحیح wgs2_aligned = 1;
    وقتی (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    نویسه cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, اندازه(cvt)),
                         (صحیح)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    اگر (k.empty())
        بازگشت نادرست;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (صحیح)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    اگر (k.run(1, &globalsize, &wgs, درست))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        بازگشت درست;
    }
    بازگشت نادرست;
}

#endif

دوگانه UMat::dot(درون_داد_Array m) ثابت
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*این) && m.type() == type());

#ifdef HAVE_OPENCL
    دوگانه r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*این, m, r), r)
#endif

    بازگشت getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
