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

#جے_تعریف_شدہ HAVE_OPENCL

نطاق cv
{

ساكن منطقي intel_gpu_gemm(
    UMat A, ناپ ناپ_A,
    UMat B, ناپ ناپ_B,
    UMat D, ناپ ناپ_D,
    مزدوج alpha, مزدوج beta,
    منطقي atrans, منطقي btrans,
    منطقي& isPropagatedC2D
)
{
    CV_بن_ورتیا(ناپ_B);

    صحيح M = ناپ_D.اچائی, N = ناپ_D.چوڑائی, K = ((atrans)? ناپ_A.اچائی : ناپ_A.چوڑائی);

    إذا (M < 4 || N < 4 || K < 4)  // vload4
        أرجع خطأ;

    CV_LOG_VERBOSE(عدم, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    بنانشانی صحيح lx = 8, ly = 4;
    بنانشانی صحيح dx = 4, dy = 8;

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

    ثابت size_t gx = div_اتے((size_t)N, dx);
    ثابت size_t gy = div_اتے((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {گیڑا_اتے(gx, lx), گیڑا_اتے(gy, ly), 1};

    ocl::قطار q;
    String errmsg;
    ثابت ocl::پروگرام_2 پروگرام = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    إذا(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), پروگرام);
        إذا (k.empty())
        {
            أرجع خطأ;
        }

        k.دلیلاں(ocl::KernelArg::PtrReadOnly(A),
               (صحيح) (A.offset / حجم(عائم)),
               ocl::KernelArg::PtrReadOnly(B),
               (صحيح) (B.offset / حجم(عائم)),
               ocl::KernelArg::PtrWriteOnly(D),
               (صحيح) (D.offset / حجم(عائم)),
               M, N, K,
               (عائم)alpha,
               (عائم)beta,
               (صحيح)(A.قدم / حجم(عائم)),
               (صحيح)(B.قدم / حجم(عائم)),
               (صحيح)(D.قدم / حجم(عائم))
        );

        منطقي ret = k.لگاتار(2, global, local, خطأ, q);
        أرجع ret;
    }
    وإلا
    {
        صحيح stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        لكل(صحيح شروع_اشاریہ = 0; شروع_اشاریہ < K; شروع_اشاریہ += stride)
        {
             ocl::Kernel k(kernelName.c_str(), پروگرام);
             k.دلیلاں(ocl::KernelArg::PtrReadOnly(A),
                    (صحيح) (A.offset / حجم(عائم)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (صحيح) (B.offset / حجم(عائم)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (صحيح) (D.offset / حجم(عائم)),
                    M, N, K,
                    (عائم)alpha,
                    (عائم)beta,
                    (صحيح)(A.قدم / حجم(عائم)),
                    (صحيح)(B.قدم / حجم(عائم)),
                    (صحيح)(D.قدم / حجم(عائم)),
                    (صحيح) شروع_اشاریہ,                          // 14 start_index
                    stride);

            منطقي ret = k.لگاتار(2, global, local, خطأ, q);
            إذا (!ret)
            {
                إذا (شروع_اشاریہ != 0)
                    isPropagatedC2D = خطأ;  // D array content is changed, need to rewrite
                أرجع خطأ;
            }
        }
        أرجع سچ;
    }
}

} // namespace cv

#جے_انت
