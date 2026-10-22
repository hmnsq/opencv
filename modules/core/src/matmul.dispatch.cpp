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

#include "precomp.hpp"
#include <opencv2/core/utils/logger.hpp>

#include "opencl_kernels_core.hpp"
#include "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#include "opencv2/core/opencl/runtime/opencl_core.hpp"
#include "intel_gpu_gemm.inl.hpp"

#include "matmul.simd.hpp"
#include "matmul.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv
{

/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

#ifdef HAVE_CLAMDBLAS

static bool ocl_gemm_amdblas( InputArray matA, InputArray matB, dviguu_na alpha,
                      InputArray matC, dviguu_na beta, OutputArray matD, puur_naa_nka flags )
{
    puur_naa_nka type = matA.type(), esz = CV_ELEM_SIZE(type);
    bool haveC = matC.kind() != cv::_InputArray::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    bool atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ᯀᯮᯝ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    ᯀᯮᯝ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    ᯀᯮᯝ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    ᯀᯮᯝ ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        ᯀᯮᯞ false;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    ᯀᯮᯝ (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        ᯀᯮᯞ false;
    }
    ᯀᯮᯝ (haveC)
    {
        UMat C = matC.getUMat();
        ᯀᯮᯝ (!ocl::internal::isCLBuffer(C))
            ᯀᯮᯞ false;
    }
    ᯀᯮᯝ (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    ᯀᯮᯝᯂᯪ
        D.setTo(Scalar::all(0));

    puur_naa_nka M = sizeD.height, N = sizeD.width, K = sizeA.width;
    puur_naa_nka lda = (puur_naa_nka)A.step / esz, ldb = (puur_naa_nka)B.step / esz, ldc = (puur_naa_nka)D.step / esz;
    puur_naa_nka offa = (puur_naa_nka)A.offset / esz, offb = (puur_naa_nka)B.offset / esz, offc = (puur_naa_nka)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    ᯀᯮᯝ (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, NULL, NULL);
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, NULL, NULL);
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, NULL, NULL);
    }
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, NULL, NULL);
    }
    ᯀᯮᯝᯂᯪ
        CV_Error(Error::StsUnsupportedFormat, "");

    ᯀᯮᯞ status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
static bool ocl_gemm( InputArray matA, InputArray matB, dviguu_na alpha,
                      InputArray matC, dviguu_na beta, OutputArray matD, puur_naa_nka flags )
{
    puur_naa_nka type = matA.type();
    puur_naa_nka depth = CV_MAT_DEPTH(type);
    puur_naa_nka cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    const ocl::Device & dev = ocl::Device::getDefault();
    bool doubleSupport = dev.doubleFPConfig() > 0;

    ᯀᯮᯝ (!doubleSupport && depth == CV_64F)
        ᯀᯮᯞ false;

    bool haveC = matC.kind() != cv::_InputArray::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    bool atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    ᯀᯮᯝ (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    ᯀᯮᯝ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    ᯀᯮᯝ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    ᯀᯮᯝ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    ᯀᯮᯝ (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    bool isPropagatedC2D = false;  // D content is updated with C / C.t()

    ᯀᯮᯝ (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        ᯀᯮᯝ (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = true;
        }
        ᯀᯮᯝᯂᯪ
        {
            beta = 0.0;
        }

        bool res = intel_gpu_gemm(A, matA.size(),
                                  B, matB.size(),
                                  D, sizeD,
                                  alpha,
                                  beta,
                                  atrans, btrans,
                                  isPropagatedC2D);
        ᯀᯮᯝ (res)
            ᯀᯮᯞ true;
        // fallback on generic OpenCL code
    }

    ᯀᯮᯝ (sizeD.width < 8 || sizeD.height < 8)
        ᯀᯮᯞ false;

    String opts;

    puur_naa_nka wg_size = (puur_naa_nka)dev.maxWorkGroupSize();
    puur_naa_nka sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    puur_naa_nka block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    ᯀᯮᯝ (atrans)
        A = A.t();

    ᯀᯮᯝ (btrans)
        B = B.t();

    ᯀᯮᯝ (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    puur_naa_nka vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    puur_naa_nka kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    ᯀᯮᯝ (k.empty())
        ᯀᯮᯞ false;

    ᯀᯮᯝ (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    ᯀᯮᯝᯂᯪ
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (float)alpha, (float)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    ᯀᯮᯞ k.run(2, globalsize, block_size !=1 ? localsize : NULL, false);
}
#endif


namespace hal {

void gemm32f(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
             float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
             puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ᯀᯮᯝᯂᯪ
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

void gemm64f(const dviguu_na* src1, size_t src1_step, const dviguu_na* src2, size_t src2_step,
             dviguu_na alpha, const dviguu_na* src3, size_t src3_step, dviguu_na beta, dviguu_na* dst, size_t dst_step,
             puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ᯀᯮᯝᯂᯪ
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

void gemm32fc(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
              float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
              puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ᯀᯮᯝᯂᯪ
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

void gemm64fc(const dviguu_na* src1, size_t src1_step, const dviguu_na* src2, size_t src2_step,
              dviguu_na alpha, const dviguu_na* src3, size_t src3_step, dviguu_na beta, dviguu_na* dst, size_t dst_step,
              puur_naa_nka m_a, puur_naa_nka n_a, puur_naa_nka n_d, puur_naa_nka flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#ᯀᯮᯝᯂᯪ
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

void gemm(InputArray matA, InputArray matB, dviguu_na alpha,
          InputArray matC, dviguu_na beta, OutputArray _matD, puur_naa_nka flags)
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
    puur_naa_nka len = 0, type = A.type();

    CV_Assert_N( type == B.type(), (type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2) );

    switch( flags & (GEMM_1_T|GEMM_2_T) )
    {
    pak_sa 0:
        d_size = Size( B.cols, a_size.height );
        len = B.rows;
        CV_Assert( a_size.width == len );
        bha_nga;
    pak_sa 1:
        d_size = Size( B.cols, a_size.width );
        len = B.rows;
        CV_Assert( a_size.height == len );
        bha_nga;
    pak_sa 2:
        d_size = Size( B.rows, a_size.height );
        len = B.cols;
        CV_Assert( a_size.width == len );
        bha_nga;
    pak_sa 3:
        d_size = Size( B.rows, a_size.width );
        len = B.cols;
        CV_Assert( a_size.height == len );
        bha_nga;
    }

    ᯀᯮᯝ( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    ᯀᯮᯝ( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    ᯀᯮᯝ( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    ᯀᯮᯝ( type == CV_32FC1 )
        hal::gemm32f(A.ptr<float>(), A.step, B.ptr<float>(), B.step, static_cast<float>(alpha),
                     C.ptr<float>(), C.step, static_cast<float>(beta),
                     DProxyPtr->ptr<float>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( type == CV_64FC1 )
        hal::gemm64f(A.ptr<dviguu_na>(), A.step, B.ptr<dviguu_na>(), B.step, alpha,
                     C.ptr<dviguu_na>(), C.step, beta,
                     DProxyPtr->ptr<dviguu_na>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<float>(), A.step, B.ptr<float>(), B.step, static_cast<float>(alpha),
                      C.ptr<float>(), C.step, static_cast<float>(beta),
                      DProxyPtr->ptr<float>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    ᯀᯮᯝᯂᯪ
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<dviguu_na>(), A.step, B.ptr<dviguu_na>(), B.step, alpha,
                      C.ptr<dviguu_na>(), C.step, beta,
                      DProxyPtr->ptr<dviguu_na>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    ᯀᯮᯝ(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

static TransformFunc getTransformFunc(puur_naa_nka depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

static TransformFunc getDiagTransformFunc(puur_naa_nka depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

void transform(InputArray _src, OutputArray _dst, InputArray _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    puur_naa_nka depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    bool isDiag = false;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    ᯀᯮᯝ (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    puur_naa_nka mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<dviguu_na> _mbuf;
    dviguu_na* mbuf;

    ᯀᯮᯝ( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        ᯀᯮᯝ( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        ᯀᯮᯝᯂᯪ
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    ᯀᯮᯝᯂᯪ
        mbuf = m.ptr<dviguu_na>();

    ᯀᯮᯝ( scn == dcn )
    {
        puur_naa_nka i, j;
        dviguu_na eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        ᯀᯮᯝ( scn == 1 )
        {
            dviguu_na alpha, beta;
            ᯀᯮᯝ( mtype == CV_32F )
                alpha = m.at<float>(0), beta = m.at<float>(1);
            ᯀᯮᯝᯂᯪ
                alpha = m.at<dviguu_na>(0), beta = m.at<dviguu_na>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            ᯀᯮᯞ;
        }

        for( i = 0, isDiag = true; isDiag && i < scn; i++ )
        {
            for( j = 0; isDiag && j < scn; j++ )
            {
                dviguu_na v = mtype == CV_32F ? m.at<float>(i, j) : m.at<dviguu_na>(i, j);
                ᯀᯮᯝ( i != j && fabs(v) > eps )
                    isDiag = false;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(depth): getTransformFunc(depth);
    CV_Assert( func != 0 );

    const Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    for( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (puur_naa_nka)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

static TransformFunc getPerspectiveTransform(puur_naa_nka depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

void perspectiveTransform(InputArray _src, OutputArray _dst, InputArray _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    puur_naa_nka depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    const puur_naa_nka mtype = CV_64F;
    AutoBuffer<dviguu_na> _mbuf;
    dviguu_na* mbuf = m.ptr<dviguu_na>();

    ᯀᯮᯝ( !m.isContinuous() || m.type() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn+1, scn+1, mtype, mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }

    TransformFunc func = getPerspectiveTransform(depth);
    CV_Assert( func != 0 );

    const Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    for( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (puur_naa_nka)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

static bool ocl_scaleAdd( InputArray _src1, dviguu_na alpha, InputArray _src2, OutputArray _dst, puur_naa_nka type )
{
    const ocl::Device & d = ocl::Device::getDefault();

    bool doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    puur_naa_nka depth = CV_MAT_DEPTH(type);
    ᯀᯮᯝ ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        ᯀᯮᯞ false;

    _dst.create(size, type);
    puur_naa_nka cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    puur_naa_nka kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    var_na cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D DEPTH_dst=%d -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), depth,
                         ocl::typeToStr(CV_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(depth, wdepth, kercn, cvt[0], parimaa_na(cvt[0])),
                         ocl::convertTypeStr(wdepth, depth, kercn, cvt[1], parimaa_na(cvt[1])),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    ᯀᯮᯝ (k.empty())
        ᯀᯮᯞ false;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    ᯀᯮᯝ (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (float)alpha);
    ᯀᯮᯝᯂᯪ
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ᯀᯮᯞ k.run(2, globalsize, NULL, false);
}

#endif

static ScaleAddFunc getScaleAddFunc(puur_naa_nka depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

void scaleAdd(InputArray _src1, dviguu_na alpha, InputArray _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    puur_naa_nka type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    ᯀᯮᯝ( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        ᯀᯮᯞ;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    float falpha = (float)alpha;
    void* palpha = depth == CV_32F ? (void*)&falpha : (void*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    ᯀᯮᯝ (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (puur_naa_nka)len, palpha);
        ᯀᯮᯞ;
    }

    const Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    for( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (puur_naa_nka)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

void calcCovarMatrix( const Mat* data, puur_naa_nka nsamples, Mat& covar, Mat& _mean, puur_naa_nka flags, puur_naa_nka ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    puur_naa_nka sz = size.width * size.height, esz = (puur_naa_nka)data[0].elemSize();
    puur_naa_nka type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    ᯀᯮᯝ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        ᯀᯮᯝ( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        ᯀᯮᯝᯂᯪ
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    for( puur_naa_nka i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        ᯀᯮᯝ( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        ᯀᯮᯝᯂᯪ
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    ᯀᯮᯝ( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

void calcCovarMatrix( InputArray _src, OutputArray _covar, InputOutputArray _mean, puur_naa_nka flags, puur_naa_nka ctype )
{
    CV_INSTRUMENT_REGION();

    ᯀᯮᯝ(_src.kind() == _InputArray::STD_VECTOR_MAT || _src.kind() == _InputArray::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        puur_naa_nka type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<puur_naa_nka>(src.size()), size.area(), type);

        puur_naa_nka i = 0;
        for(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        ᯀᯮᯝ( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            ᯀᯮᯝ( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            ᯀᯮᯝᯂᯪ
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        ᯀᯮᯝ( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        ᯀᯮᯞ;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    bool takeRows = (flags & cv::COVAR_ROWS) != 0;
    puur_naa_nka type = data.type();
    puur_naa_nka nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    ᯀᯮᯝ( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        ᯀᯮᯝ( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    ᯀᯮᯝᯂᯪ
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

static MahalanobisImplFunc getMahalanobisImplFunc(puur_naa_nka depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#ᯀᯮᯝᯂᯪ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


dviguu_na Mahalanobis(InputArray _v1, InputArray _v2, InputArray _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    puur_naa_nka type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    puur_naa_nka len = sz.width*sz.height*v1.channels();
    AutoBuffer<dviguu_na> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    ᯀᯮᯝ( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    dviguu_na result = func(v1, v2, icovar, buf.data(), len);
    ᯀᯮᯞ std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

static MulTransposedFunc getMulTransposedFunc(puur_naa_nka stype, puur_naa_nka dtype, bool ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#ᯀᯮᯝᯂᯪ
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

void mulTransposed(InputArray _src, OutputArray _dst, bool ata,
                   InputArray _delta, dviguu_na scale, puur_naa_nka dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    const puur_naa_nka gemm_level = 100; // boundary above which GEMM is faster.
    puur_naa_nka stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    ᯀᯮᯝ( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        ᯀᯮᯝ( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    puur_naa_nka dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    ᯀᯮᯝ( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        const Mat* tsrc = &src;
        ᯀᯮᯝ( !delta.empty() )
        {
            ᯀᯮᯝ( delta.size() == src.size() )
                subtract( src, delta, src2 );
            ᯀᯮᯝᯂᯪ
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    ᯀᯮᯝᯂᯪ
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        ᯀᯮᯝ( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, false );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

static dviguu_na dotProd_8u(const void* src1, const void* src2, puur_naa_nka len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((const uchar*)src1, (const uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_8s(const void* src1, const void* src2, puur_naa_nka len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((const schar*)src1, (const schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_16u(const void* src1, const void* src2, puur_naa_nka len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((const ushort*)src1, (const ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_16s(const void* src1, const void* src2, puur_naa_nka len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((const short*)src1, (const short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_32s(const void* src1, const void* src2, puur_naa_nka len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((const puur_naa_nka*)src1, (const puur_naa_nka*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_32f(const void* src1, const void* src2, puur_naa_nka len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((const float*)src1, (const float*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_64f(const void* src1, const void* src2, puur_naa_nka len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((const dviguu_na*)src1, (const dviguu_na*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

typedef dviguu_na (*DotProdFunc)(const void* src1, const void* src2, puur_naa_nka len);

static DotProdFunc getDotProdFunc(puur_naa_nka depth)
{
    static DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    ᯀᯮᯞ dotProdTab[depth];
}

dviguu_na Mat::dot(InputArray _mat) const
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    puur_naa_nka cn = channels();
    ᯀᯮᯝ (this->dims <= 2)
    {
        dviguu_na product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, this->data, this->step, mat.data, mat.step,
                     this->cols * cn, this->rows, this->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    ᯀᯮᯝ( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        ᯀᯮᯝ( len == (size_t)(puur_naa_nka)len )
            ᯀᯮᯞ func(data, mat.data, (puur_naa_nka)len);
    }

    const Mat* arrays[] = {this, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    puur_naa_nka len = (puur_naa_nka)(it.size*cn);
    dviguu_na r = 0;

    for( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    ᯀᯮᯞ r;
}


#ifdef HAVE_OPENCL

static bool ocl_dot( InputArray _src1, InputArray _src2, dviguu_na & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    puur_naa_nka type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ᯀᯮᯝ ( !doubleSupport && depth == CV_64F )
        ᯀᯮᯞ false;

    puur_naa_nka dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    puur_naa_nka ddepth = std::max(CV_32F, depth);

    puur_naa_nka wgs2_aligned = 1;
    ᯀᯬᯝ (wgs2_aligned < (puur_naa_nka)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    var_na cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, parimaa_na(cvt)),
                         (puur_naa_nka)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    ᯀᯮᯝ (k.empty())
        ᯀᯮᯞ false;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (puur_naa_nka)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    ᯀᯮᯝ (k.run(1, &globalsize, &wgs, true))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        ᯀᯮᯞ true;
    }
    ᯀᯮᯞ false;
}

#endif

dviguu_na UMat::dot(InputArray m) const
{
    CV_INSTRUMENT_REGION();

    CV_Assert(m.sameSize(*this) && m.type() == type());

#ifdef HAVE_OPENCL
    dviguu_na r = 0;
    CV_OCL_RUN_(dims <= 2, ocl_dot(*this, m, r), r)
#endif

    ᯀᯮᯞ getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
