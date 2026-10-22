/*
* Copyright 2015-2017 Philippe Tillet
* Copyright (c) 2017, Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files
* (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#اگر_تعریف_شده HAVE_OPENCL

نطاق cv
{

ساكن منطقي intel_gpu_gemm(
    UMat A, اندازه اندازه_A,
    UMat B, اندازه اندازه_B,
    UMat D, اندازه اندازه_D,
    مزدوج alpha, مزدوج beta,
    منطقي atrans, منطقي btrans,
    منطقي& isPropagatedC2D
)
{
    CV_استفادهنشده(اندازه_B);

    صحيح M = اندازه_D.ارتفاع, N = اندازه_D.عرض, K = ((atrans)? اندازه_A.ارتفاع : اندازه_A.عرض);

    إذا (M < 4 || N < 4 || K < 4)  // vload4
        أرجع خطأ;

    CV_LOG_VERBOSE(عدم, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    بیعلامت صحيح lx = 8, ly = 4;
    بیعلامت صحيح dx = 4, dy = 8;

    إذا(!atrans && !btrans)
    {
        إذا (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        وإلا
        {
            إذا (M % 2 != 0)
                أرجع خطأ;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            إذا (N % (4*8) != 0)
                أرجع خطأ;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    وإلا إذا(atrans && !btrans)
    {
        إذا (M % 32 != 0)
            أرجع خطأ;
        إذا (N % 32 != 0)
            أرجع خطأ;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    وإلا إذا(!atrans && btrans)
    {
        إذا (K % 4 != 0)
            أرجع خطأ;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    وإلا
    {
        إذا (M % 32 != 0)
            أرجع خطأ;
        إذا (N % 32 != 0)
            أرجع خطأ;
        إذا (K % 16 != 0)
            أرجع خطأ;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(عدم, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ثابت size_t gx = div_به_بالا((size_t)N, dx);
    ثابت size_t gy = div_به_بالا((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {دور_به_بالا(gx, lx), دور_به_بالا(gy, ly), 1};

    ocl::صف q;
    String errmsg;
    ثابت ocl::برنامه_2 برنامه = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    إذا(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), برنامه);
        إذا (k.empty())
        {
            أرجع خطأ;
        }

        k.آرگومانها(ocl::KernelArg::PtrReadOnly(A),
               (صحيح) (A.offset / حجم(عائم)),
               ocl::KernelArg::PtrReadOnly(B),
               (صحيح) (B.offset / حجم(عائم)),
               ocl::KernelArg::PtrWriteOnly(D),
               (صحيح) (D.offset / حجم(عائم)),
               M, N, K,
               (عائم)alpha,
               (عائم)beta,
               (صحيح)(A.گام / حجم(عائم)),
               (صحيح)(B.گام / حجم(عائم)),
               (صحيح)(D.گام / حجم(عائم))
        );

        منطقي ret = k.پیاپی(2, global, local, خطأ, q);
        أرجع ret;
    }
    وإلا
    {
        صحيح stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        لكل(صحيح شروع_شاخص = 0; شروع_شاخص < K; شروع_شاخص += stride)
        {
             ocl::Kernel k(kernelName.c_str(), برنامه);
             k.آرگومانها(ocl::KernelArg::PtrReadOnly(A),
                    (صحيح) (A.offset / حجم(عائم)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (صحيح) (B.offset / حجم(عائم)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (صحيح) (D.offset / حجم(عائم)),
                    M, N, K,
                    (عائم)alpha,
                    (عائم)beta,
                    (صحيح)(A.گام / حجم(عائم)),
                    (صحيح)(B.گام / حجم(عائم)),
                    (صحيح)(D.گام / حجم(عائم)),
                    (صحيح) شروع_شاخص,                          // 14 start_index
                    stride);

            منطقي ret = k.پیاپی(2, global, local, خطأ, q);
            إذا (!ret)
            {
                إذا (شروع_شاخص != 0)
                    isPropagatedC2D = خطأ;  // D array content is changed, need to rewrite
                أرجع خطأ;
            }
        }
        أرجع درست;
    }
}

} // namespace cv

#پایان_اگر
