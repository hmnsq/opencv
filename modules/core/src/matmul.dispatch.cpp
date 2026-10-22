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

#ifdef HAVE_CLAMDBLAS

ساكن منطقي ocl_gemm_amdblas( شغرو_Array matA, شغرو_Array matB, مزدوج alpha,
                      شغرو_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type(), esz = CV_ELEM_SIZE(type);
    منطقي haveC = matC.kind() != cv::شغرو_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    إذا (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    إذا (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    إذا ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
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
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    وإلا
        D.setTo(Scalar::all(0));

    صحيح M = sizeD.height, N = sizeD.width, K = sizeA.width;
    صحيح lda = (صحيح)A.step / esz, ldb = (صحيح)B.step / esz, ldc = (صحيح)D.step / esz;
    صحيح offa = (صحيح)A.offset / esz, offb = (صحيح)B.offset / esz, offc = (صحيح)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    إذا (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    وإلا إذا (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    وإلا إذا (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, عدم, عدم);
    }
    وإلا إذا (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (ثابت cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (ثابت cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, عدم, عدم);
    }
    وإلا
        CV_Error(Error::StsUnsupportedFormat, "");

    أرجع status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
ساكن منطقي ocl_gemm( شغرو_Array matA, شغرو_Array matB, مزدوج alpha,
                      شغرو_Array matC, مزدوج beta, OutputArray matD, صحيح flags )
{
    صحيح type = matA.type();
    صحيح depth = CV_MAT_DEPTH(type);
    صحيح cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    ثابت ocl::Device & dev = ocl::Device::getDefault();
    منطقي doubleSupport = dev.doubleFPConfig() > 0;

    إذا (!doubleSupport && depth == CV_64F)
        أرجع خطأ;

    منطقي haveC = matC.kind() != cv::شغرو_Array_2::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    منطقي atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    إذا (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    إذا (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    إذا (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    إذا (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    إذا (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    منطقي isPropagatedC2D = خطأ;  // D content is updated with C / C.t()

    إذا (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        إذا (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = true;
        }
        وإلا
        {
            beta = 0.0;
        }

        منطقي res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        إذا (res)
            أرجع true;
        // fallback on generic OpenCL code
    }

    إذا (sizeD.width < 8 || sizeD.height < 8)
        أرجع خطأ;

    String opts;

    صحيح wg_size = (صحيح)dev.maxWorkGroupSize();
    صحيح sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    صحيح block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    إذا (atrans)
        A = A.t();

    إذا (btrans)
        B = B.t();

    إذا (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    صحيح vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    صحيح kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    إذا (k.empty())
        أرجع خطأ;

    إذا (depth == CV_64F)
        k.حجوجي(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    وإلا
        k.حجوجي(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (عائم)alpha, (عائم)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    أرجع k.run(2, globalsize, block_size !=1 ? localsize : عدم, خطأ);
}
#endif


نطاق hal {

فراغ gemm32f(ثابت عائم* src1, size_t src1_step, ثابت عائم* src2, size_t src2_step,
             عائم alpha, ثابت عائم* src3, size_t src3_step, عائم beta, عائم* dst, size_t dst_step,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

فراغ gemm64f(ثابت مزدوج* src1, size_t src1_step, ثابت مزدوج* src2, size_t src2_step,
             مزدوج alpha, ثابت مزدوج* src3, size_t src3_step, مزدوج beta, مزدوج* dst, size_t dst_step,
             صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

فراغ gemm32fc(ثابت عائم* src1, size_t src1_step, ثابت عائم* src2, size_t src2_step,
              عائم alpha, ثابت عائم* src3, size_t src3_step, عائم beta, عائم* dst, size_t dst_step,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

فراغ gemm64fc(ثابت مزدوج* src1, size_t src1_step, ثابت مزدوج* src2, size_t src2_step,
              مزدوج alpha, ثابت مزدوج* src3, size_t src3_step, مزدوج beta, مزدوج* dst, size_t dst_step,
              صحيح m_a, صحيح n_a, صحيح n_d, صحيح flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#وإلا
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

فراغ gemm(شغرو_Array matA, شغرو_Array matB, مزدوج alpha,
          شغرو_Array matC, مزدوج beta, OutputArray _matD, صحيح flags)
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
    صحيح len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    بدل( flags & (GEMM_1_T|GEMM_2_T) )
    {
    حالة 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        اكسر;
    حالة 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        اكسر;
    حالة 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        اكسر;
    حالة 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        اكسر;
    }

    إذا( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    إذا( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    إذا( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    إذا( type == CV_32FC1 )
        hal::gemm32f(A.ptr<عائم>(), A.step, B.ptr<عائم>(), B.step, static_cast<عائم>(alpha),
                     C.ptr<عائم>(), C.step, static_cast<عائم>(beta),
                     DProxyPtr->ptr<عائم>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    وإلا إذا( type == CV_64FC1 )
        hal::gemm64f(A.ptr<مزدوج>(), A.step, B.ptr<مزدوج>(), B.step, alpha,
                     C.ptr<مزدوج>(), C.step, beta,
                     DProxyPtr->ptr<مزدوج>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    وإلا إذا( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<عائم>(), A.step, B.ptr<عائم>(), B.step, static_cast<عائم>(alpha),
                      C.ptr<عائم>(), C.step, static_cast<عائم>(beta),
                      DProxyPtr->ptr<عائم>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    وإلا
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<مزدوج>(), A.step, B.ptr<مزدوج>(), B.step, alpha,
                      C.ptr<مزدوج>(), C.step, beta,
                      DProxyPtr->ptr<مزدوج>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    إذا(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

ساكن TransformFunc getTransformFunc(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ساكن TransformFunc getDiagTransformFunc(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ transform(شغرو_Array _src, OutputArray _dst, شغرو_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    منطقي isDiag = خطأ;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    إذا (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    صحيح mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<مزدوج> _mbuf;
    مزدوج* mbuf;

    إذا( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        إذا( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        وإلا
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
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
            src.convertTo(dst, dst.type(), alpha, beta);
            أرجع;
        }

        لكل( i = 0, isDiag = true; isDiag && i < scn; i++ )
        {
            لكل( j = 0; isDiag && j < scn; j++ )
            {
                مزدوج v = mtype == CV_32F ? m.at<عائم>(i, j) : m.at<مزدوج>(i, j);
                إذا( i != j && fabs(v) > eps )
                    isDiag = خطأ;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

ساكن TransformFunc getPerspectiveTransform(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ perspectiveTransform(شغرو_Array _src, OutputArray _dst, شغرو_Array _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    صحيح depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ثابت صحيح mtype = CV_64F;
    AutoBuffer<مزدوج> _mbuf;
    مزدوج* mbuf = m.ptr<مزدوج>();

    إذا( !m.isContinuous() || m.type() != mtype )
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

    لكل( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (صحيح)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

ساكن منطقي ocl_scaleAdd( شغرو_Array _src1, مزدوج alpha, شغرو_Array _src2, OutputArray _dst, صحيح type )
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    منطقي doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    صحيح depth = CV_MAT_DEPTH(type);
    إذا ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        أرجع خطأ;

    _dst.create(size, type);
    صحيح cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    صحيح kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    حرف cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], حجم(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], حجم(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    إذا (k.empty())
        أرجع خطأ;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    إذا (wdepth == CV_32F)
        k.حجوجي(src1arg, src2arg, dstarg, (عائم)alpha);
    وإلا
        k.حجوجي(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, عدم, خطأ);
}

#endif

ساكن ScaleAddFunc getScaleAddFunc(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ scaleAdd(شغرو_Array _src1, مزدوج alpha, شغرو_Array _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    إذا( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        أرجع;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    عائم falpha = (عائم)alpha;
    فراغ* palpha = depth == CV_32F ? (فراغ*)&falpha : (فراغ*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    إذا (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
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

فراغ calcCovarMatrix( ثابت Mat* data, صحيح nsamples, Mat& covar, Mat& _mean, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    صحيح sz = size.width * size.height, esz = (صحيح)data[0].elemSize();
    صحيح type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        إذا( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        وإلا
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    لكل( صحيح i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        إذا( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        وإلا
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

فراغ calcCovarMatrix( شغرو_Array _src, OutputArray _covar, شغرو_Output_Array _mean, صحيح flags, صحيح ctype )
{
    CV_INSTRUMENT_REGION();

    إذا(_src.kind() == شغرو_Array_2::STD_VECTOR_MAT || _src.kind() == شغرو_Array_2::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        صحيح type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<صحيح>(src.size()), size.area(), type);

        صحيح i = 0;
        لكل(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        إذا( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            إذا( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            وإلا
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        إذا( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        أرجع;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    منطقي takeRows = (flags & cv::COVAR_ROWS) != 0;
    صحيح type = data.type();
    صحيح nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    إذا( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        إذا( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    وإلا
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

ساكن MahalanobisImplFunc getMahalanobisImplFunc(صحيح depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


مزدوج Mahalanobis(شغرو_Array _v1, شغرو_Array _v2, شغرو_Array _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    صحيح type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    صحيح len = sz.width*sz.height*v1.channels();
    AutoBuffer<مزدوج> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    إذا( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    مزدوج result = func(v1, v2, icovar, buf.data(), len);
    أرجع std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

ساكن MulTransposedFunc getMulTransposedFunc(صحيح stype, صحيح dtype, منطقي ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#وإلا
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

فراغ mulTransposed(شغرو_Array _src, OutputArray _dst, منطقي ata,
                   شغرو_Array _delta, مزدوج scale, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    ثابت صحيح gemm_level = 100; // boundary above which GEMM is faster.
    صحيح stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    إذا( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        إذا( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    صحيح dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    إذا( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        ثابت Mat* tsrc = &src;
        إذا( !delta.empty() )
        {
            إذا( delta.size() == src.size() )
                subtract( src, delta, src2 );
            وإلا
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
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
    CV_CPU_DISPATCH(dotProd_16s, ((ثابت short*)src1, (ثابت short*)src2, len),
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

ساكن DotProdFunc getDotProdFunc(صحيح depth)
{
    ساكن DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    أرجع dotProdTab[depth];
}

مزدوج Mat::dot(شغرو_Array _mat) ثابت
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    صحيح cn = channels();
    إذا (هذا->dims <= 2)
    {
        مزدوج product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, هذا->data, هذا->step, mat.data, mat.step,
                     هذا->cols * cn, هذا->rows, هذا->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    إذا( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        إذا( len == (size_t)(صحيح)len )
            أرجع func(data, mat.data, (صحيح)len);
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


#ifdef HAVE_OPENCL

ساكن منطقي ocl_dot( شغرو_Array _src1, شغرو_Array _src2, مزدوج & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    صحيح type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا ( !doubleSupport && depth == CV_64F )
        أرجع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    صحيح ddepth = std::max(CV_32F, depth);

    صحيح wgs2_aligned = 1;
    طالما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    حرف cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, حجم(cvt)),
                         (صحيح)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    إذا (k.empty())
        أرجع خطأ;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.حجوجي(src1arg, src1.cols, (صحيح)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    إذا (k.run(1, &globalsize, &wgs, true))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        أرجع true;
    }
    أرجع خطأ;
}

#endif

مزدوج UMat::dot(شغرو_Array m) ثابت
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*هذا) && m.type() == type());

#ifdef HAVE_OPENCL
    مزدوج r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*هذا, m, r), r)
#endif

    أرجع getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
