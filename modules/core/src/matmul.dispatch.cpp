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
                      InputArray matC, dviguu_na beta, OutputArray matD, ကိန်းပြည့် flags )
{
    ကိန်းပြည့် type = matA.type(), esz = CV_ELEM_SIZE(type);
    bool haveC = matC.kind() != cv::_InputArray::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    bool atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    အကယ်၍ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    အကယ်၍ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    အကယ်၍ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    Size sizeD(sizeB.width, sizeA.height);

    CV_Assert( matB.type() == type && (!haveC || matC.type() == type) );
    CV_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD.create(sizeD, type);
    အကယ်၍ ( matA.offset() % esz != 0 || matA.step() % esz != 0 ||
         matB.offset() % esz != 0 || matB.step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC.step() % esz != 0)) )
        ပြန်ပေး false;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    အကယ်၍ (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        ပြန်ပေး false;
    }
    အကယ်၍ (haveC)
    {
        UMat C = matC.getUMat();
        အကယ်၍ (!ocl::internal::isCLBuffer(C))
            ပြန်ပေး false;
    }
    အကယ်၍ (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    မဟုတ်ပါက
        D.setTo(Scalar::all(0));

    ကိန်းပြည့် M = sizeD.height, N = sizeD.width, K = sizeA.width;
    ကိန်းပြည့် lda = (ကိန်းပြည့်)A.step / esz, ldb = (ကိန်းပြည့်)B.step / esz, ldc = (ကိန်းပြည့်)D.step / esz;
    ကိန်းပြည့် offa = (ကိန်းပြည့်)A.offset / esz, offb = (ကိန်းပြည့်)B.offset / esz, offc = (ကိန်းပြည့်)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault().ptr();
    clblasTranspose transA = atrans ? clblasTrans : clblasNoTrans;
    clblasTranspose transB = btrans ? clblasTrans : clblasNoTrans;
    clblasOrder order = clblasRowMajor;
    clblasStatus status = clblasSuccess;

    အကယ်၍ (type == CV_32FC1)
        status = clblasSgemm(order, transA, transB, M, N, K,
                             (cl_float)alpha, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, NULL, NULL);
    မဟုတ်ပါက အကယ်၍ (type == CV_64FC1)
        status = clblasDgemm(order, transA, transB, M, N, K,
                             alpha, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, NULL, NULL);
    မဟုတ်ပါက အကယ်၍ (type == CV_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clblasCgemm(order, transA, transB, M, N, K,
                              alpha_2, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                              (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                              beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                              1, &clq, 0, NULL, NULL);
    }
    မဟုတ်ပါက အကယ်၍ (type == CV_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clblasZgemm(order, transA, transB, M, N, K,
                             alpha_2, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                             (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                             beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                             1, &clq, 0, NULL, NULL);
    }
    မဟုတ်ပါက
        CV_Error(Error::StsUnsupportedFormat, "");

    ပြန်ပေး status == clblasSuccess;
}

#endif

#ifdef HAVE_OPENCL
static bool ocl_gemm( InputArray matA, InputArray matB, dviguu_na alpha,
                      InputArray matC, dviguu_na beta, OutputArray matD, ကိန်းပြည့် flags )
{
    ကိန်းပြည့် type = matA.type();
    ကိန်းပြည့် depth = CV_MAT_DEPTH(type);
    ကိန်းပြည့် cn = CV_MAT_CN(type);

    CV_CheckTypeEQ(type, matB.type(), "");
    CV_CheckType(type, type == CV_32FC1 || type == CV_64FC1 || type == CV_32FC2 || type == CV_64FC2, "");

    const ocl::Device & dev = ocl::Device::getDefault();
    bool doubleSupport = dev.doubleFPConfig() > 0;

    အကယ်၍ (!doubleSupport && depth == CV_64F)
        ပြန်ပေး false;

    bool haveC = matC.kind() != cv::_InputArray::NONE;
    Size sizeA = matA.size(), sizeB = matB.size(), sizeC = haveC ? matC.size() : Size(0, 0);
    bool atrans = (flags & GEMM_1_T) != 0, btrans = (flags & GEMM_2_T) != 0, ctrans = (flags & GEMM_3_T) != 0;

    အကယ်၍ (haveC)
        CV_CheckTypeEQ(type, matC.type(), "");

    Size sizeD(((btrans) ? sizeB.height : sizeB.width),
               ((atrans) ? sizeA.width : sizeA.height));

    အကယ်၍ (atrans)
        sizeA = Size(sizeA.height, sizeA.width);
    အကယ်၍ (btrans)
        sizeB = Size(sizeB.height, sizeB.width);
    အကယ်၍ (haveC && ctrans)
        sizeC = Size(sizeC.height, sizeC.width);

    CV_CheckEQ(sizeA.width, sizeB.height, "");
    အကယ်၍ (haveC)
        CV_CheckEQ(sizeC, sizeD, "");

    UMat A = matA.getUMat();
    UMat B = matB.getUMat();

    matD.create(sizeD, type);
    UMat D = matD.getUMat();

    bool isPropagatedC2D = false;  // D content is updated with C / C.t()

    အကယ်၍ (dev.intelSubgroupsSupport() && (depth == CV_32F) && cn == 1)
    {
        အကယ်၍ (haveC && beta != 0.0)
        {
            ctrans ? transpose(matC, D) : matC.copyTo(D);
            isPropagatedC2D = true;
        }
        မဟုတ်ပါက
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
        အကယ်၍ (res)
            ပြန်ပေး true;
        // fallback on generic OpenCL code
    }

    အကယ်၍ (sizeD.width < 8 || sizeD.height < 8)
        ပြန်ပေး false;

    String opts;

    ကိန်းပြည့် wg_size = (ကိန်းပြည့်)dev.maxWorkGroupSize();
    ကိန်းပြည့် sizeDmin = std::min(sizeD.width, sizeD.height);
    wg_size = std::min(wg_size, sizeDmin * sizeDmin);
    ကိန်းပြည့် block_size = (wg_size / (32*cn) < 32) ? (wg_size / (16*cn) < 16) ? (wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    အကယ်၍ (atrans)
        A = A.t();

    အကယ်၍ (btrans)
        B = B.t();

    အကယ်၍ (haveC && !isPropagatedC2D)
        ctrans ? transpose(matC, D) : matC.copyTo(D);

    ကိန်းပြည့် vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    ကိန်းပြည့် kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);

    opts += format(" -D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d%s%s%s",
                      ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(CV_MAKETYPE(depth, kercn)),
                      cn, kercn, block_size,
                      (sizeA.width % block_size !=0) ? " -D NO_MULT" : "",
                      haveC ? " -D HAVE_C" : "",
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", cv::ocl::core::gemm_oclsrc, opts);
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    အကယ်၍ (depth == CV_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    မဟုတ်ပါက
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (float)alpha, (float)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};

    ပြန်ပေး k.run(2, globalsize, block_size !=1 ? localsize : NULL, false);
}
#endif


namespace hal {

void gemm32f(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
             float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
             ကိန်းပြည့် m_a, ကိန်းပြည့် n_a, ကိန်းပြည့် n_d, ကိန်းပြည့် flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32f, cv_hal_gemm32f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#မဟုတ်ပါက
    CV_CPU_DISPATCH(gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

void gemm64f(const dviguu_na* src1, size_t src1_step, const dviguu_na* src2, size_t src2_step,
             dviguu_na alpha, const dviguu_na* src3, size_t src3_step, dviguu_na beta, dviguu_na* dst, size_t dst_step,
             ကိန်းပြည့် m_a, ကိန်းပြည့် n_a, ကိန်းပြည့် n_d, ကိန်းပြည့် flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64f, cv_hal_gemm64f, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#မဟုတ်ပါက
    CV_CPU_DISPATCH(gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

void gemm32fc(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
              float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
              ကိန်းပြည့် m_a, ကိန်းပြည့် n_a, ကိန်းပြည့် n_d, ကိန်းပြည့် flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#မဟုတ်ပါက
    CV_CPU_DISPATCH(gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

void gemm64fc(const dviguu_na* src1, size_t src1_step, const dviguu_na* src2, size_t src2_step,
              dviguu_na alpha, const dviguu_na* src3, size_t src3_step, dviguu_na beta, dviguu_na* dst, size_t dst_step,
              ကိန်းပြည့် m_a, ကိန်းပြည့် n_a, ကိန်းပြည့် n_d, ကိန်းပြည့် flags)
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags)
#ifdef CV_GEMM_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags));
#မဟုတ်ပါက
    CV_CPU_DISPATCH(gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

} // namespace hal

void gemm(InputArray matA, InputArray matB, dviguu_na alpha,
          InputArray matC, dviguu_na beta, OutputArray _matD, ကိန်းပြည့် flags)
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
    ကိန်းပြည့် len = 0, type = A.type();

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

    အကယ်၍( !C.empty() )
    {
        CV_Assert_N( C.type() == type,
            (((flags&GEMM_3_T) == 0 && C.rows == d_size.height && C.cols == d_size.width) ||
             ((flags&GEMM_3_T) != 0 && C.rows == d_size.width && C.cols == d_size.height)));
    }

    _matD.create( d_size.height, d_size.width, type );
    Mat D = _matD.getMat();
    အကယ်၍( (flags & GEMM_3_T) != 0 && C.data == D.data )
    {
        transpose( C, C );
        flags &= ~GEMM_3_T;
    }

    Mat *DProxyPtr = &D, DProxy;
    အကယ်၍( D.data == A.data || D.data == B.data )
    {
        DProxy = Mat(d_size.height, d_size.width, D.type());
        DProxyPtr = &DProxy;
    }

    အကယ်၍( type == CV_32FC1 )
        hal::gemm32f(A.ptr<float>(), A.step, B.ptr<float>(), B.step, static_cast<float>(alpha),
                     C.ptr<float>(), C.step, static_cast<float>(beta),
                     DProxyPtr->ptr<float>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    မဟုတ်ပါက အကယ်၍( type == CV_64FC1 )
        hal::gemm64f(A.ptr<dviguu_na>(), A.step, B.ptr<dviguu_na>(), B.step, alpha,
                     C.ptr<dviguu_na>(), C.step, beta,
                     DProxyPtr->ptr<dviguu_na>(), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    မဟုတ်ပါက အကယ်၍( type == CV_32FC2 )
        hal::gemm32fc(A.ptr<float>(), A.step, B.ptr<float>(), B.step, static_cast<float>(alpha),
                      C.ptr<float>(), C.step, static_cast<float>(beta),
                      DProxyPtr->ptr<float>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    မဟုတ်ပါက
    {
        CV_Assert( type == CV_64FC2 );
        hal::gemm64fc(A.ptr<dviguu_na>(), A.step, B.ptr<dviguu_na>(), B.step, alpha,
                      C.ptr<dviguu_na>(), C.step, beta,
                      DProxyPtr->ptr<dviguu_na>(), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    အကယ်၍(DProxyPtr != &D)
        DProxyPtr->copyTo(D);
}



/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/

static TransformFunc getTransformFunc(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

static TransformFunc getDiagTransformFunc(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getDiagTransformFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

void transform(InputArray _src, OutputArray _dst, InputArray _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ကိန်းပြည့် depth = src.depth(), scn = src.channels(), dcn = m.rows;
    CV_Assert( scn == m.cols || scn + 1 == m.cols );
    bool isDiag = false;

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    အကယ်၍ (src.data == dst.data)  // inplace case
    {
        CV_Assert(scn == dcn);
        src = src.clone();  // TODO Add performance warning
    }

    ကိန်းပြည့် mtype = depth == CV_32S || depth == CV_64F ? CV_64F : CV_32F;
    AutoBuffer<dviguu_na> _mbuf;
    dviguu_na* mbuf;

    အကယ်၍( !m.isContinuous() || m.type() != mtype || m.cols != scn + 1 )
    {
        _mbuf.allocate(dcn*(scn+1));
        mbuf = _mbuf.data();
        Mat tmp(dcn, scn+1, mtype, mbuf);
        memset(tmp.ptr(), 0, tmp.total()*tmp.elemSize());
        အကယ်၍( m.cols == scn+1 )
            m.convertTo(tmp, mtype);
        မဟုတ်ပါက
        {
            Mat tmppart = tmp.colRange(0, m.cols);
            m.convertTo(tmppart, mtype);
        }
        m = tmp;
    }
    မဟုတ်ပါက
        mbuf = m.ptr<dviguu_na>();

    အကယ်၍( scn == dcn )
    {
        ကိန်းပြည့် i, j;
        dviguu_na eps = mtype == CV_32F ? FLT_EPSILON : DBL_EPSILON;

        အကယ်၍( scn == 1 )
        {
            dviguu_na alpha, beta;
            အကယ်၍( mtype == CV_32F )
                alpha = m.at<float>(0), beta = m.at<float>(1);
            မဟုတ်ပါက
                alpha = m.at<dviguu_na>(0), beta = m.at<dviguu_na>(1);
            src.convertTo(dst, dst.type(), alpha, beta);
            ပြန်ပေး;
        }

        အတွက်( i = 0, isDiag = true; isDiag && i < scn; i++ )
        {
            အတွက်( j = 0; isDiag && j < scn; j++ )
            {
                dviguu_na v = mtype == CV_32F ? m.at<float>(i, j) : m.at<dviguu_na>(i, j);
                အကယ်၍( i != j && fabs(v) > eps )
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

    အတွက်( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ကိန်းပြည့်)total, scn, dcn );
}



/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/

static TransformFunc getPerspectiveTransform(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getPerspectiveTransform, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

void perspectiveTransform(InputArray _src, OutputArray _dst, InputArray _mtx)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), m = _mtx.getMat();
    ကိန်းပြည့် depth = src.depth(), scn = src.channels(), dcn = m.rows-1;
    CV_Assert( scn + 1 == m.cols );
    CV_Assert( depth == CV_32F || depth == CV_64F );

    _dst.createSameSize( src, CV_MAKETYPE(depth, dcn) );
    Mat dst = _dst.getMat();

    const ကိန်းပြည့် mtype = CV_64F;
    AutoBuffer<dviguu_na> _mbuf;
    dviguu_na* mbuf = m.ptr<dviguu_na>();

    အကယ်၍( !m.isContinuous() || m.type() != mtype )
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

    အတွက်( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (ကိန်းပြည့်)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/

#ifdef HAVE_OPENCL

static bool ocl_scaleAdd( InputArray _src1, dviguu_na alpha, InputArray _src2, OutputArray _dst, ကိန်းပြည့် type )
{
    const ocl::Device & d = ocl::Device::getDefault();

    bool doubleSupport = d.doubleFPConfig() > 0;
    Size size = _src1.size();
    ကိန်းပြည့် depth = CV_MAT_DEPTH(type);
    အကယ်၍ ( (!doubleSupport && depth == CV_64F) || size != _src2.size() )
        ပြန်ပေး false;

    _dst.create(size, type);
    ကိန်းပြည့် cn = CV_MAT_CN(type), wdepth = std::max(depth, CV_32F);
    ကိန်းပြည့် kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
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
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    အကယ်၍ (wdepth == CV_32F)
        k.args(src1arg, src2arg, dstarg, (float)alpha);
    မဟုတ်ပါက
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst.cols * cn / kercn, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ပြန်ပေး k.run(2, globalsize, NULL, false);
}

#endif

static ScaleAddFunc getScaleAddFunc(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getScaleAddFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

void scaleAdd(InputArray _src1, dviguu_na alpha, InputArray _src2, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ကိန်းပြည့် type = _src1.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( type == _src2.type() );

    CV_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    အကယ်၍( depth != CV_32F && depth != CV_64F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, depth);
        ပြန်ပေး;
    }

    Mat src1 = _src1.getMat(), src2 = _src2.getMat();
    CV_Assert(src1.size == src2.size);

    _dst.create(src1.size, type);
    Mat dst = _dst.getMat();

    float falpha = (float)alpha;
    void* palpha = depth == CV_32F ? (void*)&falpha : (void*)&alpha;

    ScaleAddFunc func = getScaleAddFunc(depth);
    CV_Assert(func);

    အကယ်၍ (src1.isContinuous() && src2.isContinuous() && dst.isContinuous())
    {
        size_t len = src1.total()*cn;
        func(src1.ptr(), src2.ptr(), dst.ptr(), (ကိန်းပြည့်)len, palpha);
        ပြန်ပေး;
    }

    const Mat* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3] = {};
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    အတွက်( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (ကိန်းပြည့်)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

void calcCovarMatrix( const Mat* data, ကိန်းပြည့် nsamples, Mat& covar, Mat& _mean, ကိန်းပြည့် flags, ကိန်းပြည့် ctype )
{
    CV_INSTRUMENT_REGION();

    CV_Assert_N( data, nsamples > 0 );
    Size size = data[0].size();
    ကိန်းပြည့် sz = size.width * size.height, esz = (ကိန်းပြည့်)data[0].elemSize();
    ကိန်းပြည့် type = data[0].type();
    Mat mean;
    ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

    အကယ်၍( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        CV_Assert( _mean.size() == size );
        အကယ်၍( _mean.isContinuous() && _mean.type() == ctype )
            mean = _mean.reshape(1, 1);
        မဟုတ်ပါက
        {
            _mean.convertTo(mean, ctype);
            mean = mean.reshape(1, 1);
        }
    }

    Mat _data(nsamples, sz, type);

    အတွက်( ကိန်းပြည့် i = 0; i < nsamples; i++ )
    {
        CV_Assert_N( data[i].size() == size, data[i].type() == type );
        အကယ်၍( data[i].isContinuous() )
            memcpy( _data.ptr(i), data[i].ptr(), sz*esz );
        မဟုတ်ပါက
        {
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );
    အကယ်၍( (flags & cv::COVAR_USE_AVG) == 0 )
        _mean = mean.reshape(1, size.height);
}

void calcCovarMatrix( InputArray _src, OutputArray _covar, InputOutputArray _mean, ကိန်းပြည့် flags, ကိန်းပြည့် ctype )
{
    CV_INSTRUMENT_REGION();

    အကယ်၍(_src.kind() == _InputArray::STD_VECTOR_MAT || _src.kind() == _InputArray::STD_ARRAY_MAT)
    {
        std::vector<cv::Mat> src;
        _src.getMatVector(src);

        CV_Assert( src.size() > 0 );

        Size size = src[0].size();
        ကိန်းပြည့် type = src[0].type();

        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.depth()), CV_32F);

        Mat _data(static_cast<ကိန်းပြည့်>(src.size()), size.area(), type);

        ကိန်းပြည့် i = 0;
        အတွက်(std::vector<cv::Mat>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CV_Assert_N( (*each).size() == size, (*each).type() == type );
            Mat dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        Mat mean;
        အကယ်၍( (flags & cv::COVAR_USE_AVG) != 0 )
        {
            CV_Assert( _mean.size() == size );
            Mat mean0 = _mean.getMat();

            အကယ်၍( mean0.isContinuous() && mean0.type() == ctype )
                mean = mean0.reshape(1, 1);
            မဟုတ်ပါက
            {
                mean0.convertTo(mean, ctype);
                mean = mean.reshape(1, 1);
            }
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(cv::COVAR_ROWS|cv::COVAR_COLS)) | cv::COVAR_ROWS, ctype );

        အကယ်၍( (flags & cv::COVAR_USE_AVG) == 0 )
        {
            mean = mean.reshape(1, size.height);
            mean.copyTo(_mean);
        }
        ပြန်ပေး;
    }

    Mat data = _src.getMat(), mean;
    CV_Assert( ((flags & cv::COVAR_ROWS) != 0) ^ ((flags & cv::COVAR_COLS) != 0) );
    bool takeRows = (flags & cv::COVAR_ROWS) != 0;
    ကိန်းပြည့် type = data.type();
    ကိန်းပြည့် nsamples = takeRows ? data.rows : data.cols;
    CV_Assert( nsamples > 0 );
    Size size = takeRows ? Size(data.cols, 1) : Size(1, data.rows);

    အကယ်၍( (flags & cv::COVAR_USE_AVG) != 0 )
    {
        mean = _mean.getMat();
        ctype = std::max(std::max(CV_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.depth()), CV_32F);
        CV_Assert( mean.size() == size );
        အကယ်၍( mean.type() != ctype )
        {
            _mean.create(mean.size(), ctype);
            Mat tmp = _mean.getMat();
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    မဟုတ်ပါက
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

static MahalanobisImplFunc getMahalanobisImplFunc(ကိန်းပြည့် depth)
{
#ifdef CV_MAHALANOBIS_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMahalanobisImplFunc, (depth));
#မဟုတ်ပါက
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMahalanobisImplFunc, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}


dviguu_na Mahalanobis(InputArray _v1, InputArray _v2, InputArray _icovar)
{
    CV_INSTRUMENT_REGION();

    Mat v1 = _v1.getMat(), v2 = _v2.getMat(), icovar = _icovar.getMat();
    ကိန်းပြည့် type = v1.type(), depth = v1.depth();
    Size sz = v1.size();
    ကိန်းပြည့် len = sz.width*sz.height*v1.channels();
    AutoBuffer<dviguu_na> buf(len);

    CV_Assert_N( type == v2.type(), type == icovar.type(),
        sz == v2.size(), len == icovar.rows && len == icovar.cols );

    sz.width *= v1.channels();
    အကယ်၍( v1.isContinuous() && v2.isContinuous() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    MahalanobisImplFunc func = getMahalanobisImplFunc(depth);
    CV_Assert(func);

    dviguu_na result = func(v1, v2, icovar, buf.data(), len);
    ပြန်ပေး std::sqrt(result);
}



/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

static MulTransposedFunc getMulTransposedFunc(ကိန်းပြည့် stype, ကိန်းပြည့် dtype, bool ata)
{
#ifdef CV_MULTRANSPOSED_BASELINE_ONLY
    CV_CPU_CALL_BASELINE(getMulTransposedFunc, (stype, dtype, ata));
#မဟုတ်ပါက
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getMulTransposedFunc, (stype, dtype, ata),
        CV_CPU_DISPATCH_MODES_ALL);
#endif
}

void mulTransposed(InputArray _src, OutputArray _dst, bool ata,
                   InputArray _delta, dviguu_na scale, ကိန်းပြည့် dtype)
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat(), delta = _delta.getMat();
    const ကိန်းပြည့် gemm_level = 100; // boundary above which GEMM is faster.
    ကိန်းပြည့် stype = src.type();
    dtype = std::max(std::max(CV_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.depth()), CV_32F);
    CV_Assert( src.channels() == 1 );

    အကယ်၍( !delta.empty() )
    {
        CV_Assert_N( delta.channels() == 1,
            (delta.rows == src.rows || delta.rows == 1),
            (delta.cols == src.cols || delta.cols == 1));
        အကယ်၍( delta.type() != dtype )
            delta.convertTo(delta, dtype);
    }

    ကိန်းပြည့် dsize = ata ? src.cols : src.rows;
    _dst.create( dsize, dsize, dtype );
    Mat dst = _dst.getMat();

    အကယ်၍( src.data == dst.data || (stype == dtype &&
        (dst.cols >= gemm_level && dst.rows >= gemm_level &&
         src.cols >= gemm_level && src.rows >= gemm_level)))
    {
        Mat src2;
        const Mat* tsrc = &src;
        အကယ်၍( !delta.empty() )
        {
            အကယ်၍( delta.size() == src.size() )
                subtract( src, delta, src2 );
            မဟုတ်ပါက
            {
                repeat(delta, src.rows/delta.rows, src.cols/delta.cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, Mat(), 0, dst, ata ? GEMM_1_T : GEMM_2_T );
    }
    မဟုတ်ပါက
    {
        MulTransposedFunc func = getMulTransposedFunc(stype, dtype, ata);
        အကယ်၍( !func )
            CV_Error( cv::Error::StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, false );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/

static dviguu_na dotProd_8u(const void* src1, const void* src2, ကိန်းပြည့် len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8u, ((const uchar*)src1, (const uchar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_8s(const void* src1, const void* src2, ကိန်းပြည့် len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_8s, ((const schar*)src1, (const schar*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_16u(const void* src1, const void* src2, ကိန်းပြည့် len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16u, ((const ushort*)src1, (const ushort*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_16s(const void* src1, const void* src2, ကိန်းပြည့် len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_16s, ((const short*)src1, (const short*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_32s(const void* src1, const void* src2, ကိန်းပြည့် len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32s, ((const ကိန်းပြည့်*)src1, (const ကိန်းပြည့်*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_32f(const void* src1, const void* src2, ကိန်းပြည့် len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_32f, ((const float*)src1, (const float*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}
static dviguu_na dotProd_64f(const void* src1, const void* src2, ကိန်းပြည့် len)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(dotProd_64f, ((const dviguu_na*)src1, (const dviguu_na*)src2, len),
        CV_CPU_DISPATCH_MODES_ALL);
}

typedef dviguu_na (*DotProdFunc)(const void* src1, const void* src2, ကိန်းပြည့် len);

static DotProdFunc getDotProdFunc(ကိန်းပြည့် depth)
{
    static DotProdFunc dotProdTab[CV_DEPTH_MAX] =
    {
        dotProd_8u, dotProd_8s,
        dotProd_16u, dotProd_16s,
        dotProd_32s, dotProd_32f,
        dotProd_64f, 0
    };

    ပြန်ပေး dotProdTab[depth];
}

dviguu_na Mat::dot(InputArray _mat) const
{
    CV_INSTRUMENT_REGION();

    Mat mat = _mat.getMat();
    CV_Assert_N( mat.type() == type(), mat.size == size);

    ကိန်းပြည့် cn = channels();
    အကယ်၍ (this->dims <= 2)
    {
        dviguu_na product = 0;
        CALL_HAL_RET(dotProduct, cv_hal_dotProduct, product, this->data, this->step, mat.data, mat.step,
                     this->cols * cn, this->rows, this->depth());
    }

    DotProdFunc func = getDotProdFunc(depth());
    CV_Assert(func != 0 );

    အကယ်၍( isContinuous() && mat.isContinuous() )
    {
        size_t len = total()*cn;
        အကယ်၍( len == (size_t)(ကိန်းပြည့်)len )
            ပြန်ပေး func(data, mat.data, (ကိန်းပြည့်)len);
    }

    const Mat* arrays[] = {this, &mat, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ကိန်းပြည့် len = (ကိန်းပြည့်)(it.size*cn);
    dviguu_na r = 0;

    အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    ပြန်ပေး r;
}


#ifdef HAVE_OPENCL

static bool ocl_dot( InputArray _src1, InputArray _src2, dviguu_na & res )
{
    UMat src1 = _src1.getUMat().reshape(1), src2 = _src2.getUMat().reshape(1);

    ကိန်းပြည့် type = src1.type(), depth = CV_MAT_DEPTH(type),
            kercn = ocl::predictOptimalVectorWidth(src1, src2);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    အကယ်၍ ( !doubleSupport && depth == CV_64F )
        ပြန်ပေး false;

    ကိန်းပြည့် dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();
    ကိန်းပြည့် ddepth = std::max(CV_32F, depth);

    ကိန်းပြည့် wgs2_aligned = 1;
    နေစဉ် (wgs2_aligned < (ကိန်းပြည့်)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    var_na cvt[50];
    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D dstT=%s -D dstTK=%s -D ddepth=%d -D convertToDT=%s -D OP_DOT "
                         "-D WGS=%d -D WGS2_ALIGNED=%d%s%s%s -D kercn=%d",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)), ocl::typeToStr(depth),
                         ocl::typeToStr(ddepth), ocl::typeToStr(CV_MAKE_TYPE(ddepth, kercn)),
                         ddepth, ocl::convertTypeStr(depth, ddepth, kercn, cvt, parimaa_na(cvt)),
                         (ကိန်းပြည့်)wgs, wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src1.isContinuous() ? " -D HAVE_SRC_CONT" : "",
                         _src2.isContinuous() ? " -D HAVE_SRC2_CONT" : "", kercn));
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    UMat db(1, dbsize, ddepth);

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dbarg = ocl::KernelArg::PtrWriteOnly(db);

    k.args(src1arg, src1.cols, (ကိန်းပြည့်)src1.total(), dbsize, dbarg, src2arg);

    size_t globalsize = dbsize * wgs;
    အကယ်၍ (k.run(1, &globalsize, &wgs, true))
    {
        res = sum(db.getMat(ACCESS_READ))[0];
        ပြန်ပေး true;
    }
    ပြန်ပေး false;
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

    ပြန်ပေး getMat(ACCESS_READ).dot(m);
}

}  // namespace cv::
/* End of file. */
