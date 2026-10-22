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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਨਾਮਾਕਾਸ਼ cv
{

ਸਥਿਰ ਤਾਰਕਿਕ intel_gpu_gemm(
    UMat A, ਮਾਪ ਮਾਪ_A,
    UMat B, ਮਾਪ ਮਾਪ_B,
    UMat D, ਮਾਪ ਮਾਪ_D,
    ਦੁੱਗਣਾ alpha, ਦੁੱਗਣਾ beta,
    ਤਾਰਕਿਕ atrans, ਤਾਰਕਿਕ btrans,
    ਤਾਰਕਿਕ& isPropagatedC2D
)
{
    CV_ਅਣਵਰਤਿਆ(ਮਾਪ_B);

    ਪੂਰਨ M = ਮਾਪ_D.ਉਚਾਈ, N = ਮਾਪ_D.ਚੌੜਾਈ, K = ((atrans)? ਮਾਪ_A.ਉਚਾਈ : ਮਾਪ_A.ਚੌੜਾਈ);

    ਜੇ (M < 4 || N < 4 || K < 4)  // vload4
        ਵਾਪਸ ਝੂਠ;

    CV_LOG_VERBOSE(ਨਿਰਾ, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lx = 8, ly = 4;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dx = 4, dy = 8;

    ਜੇ(!atrans && !btrans)
    {
        ਜੇ (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (M % 2 != 0)
                ਵਾਪਸ ਝੂਠ;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            ਜੇ (N % (4*8) != 0)
                ਵਾਪਸ ਝੂਠ;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ(atrans && !btrans)
    {
        ਜੇ (M % 32 != 0)
            ਵਾਪਸ ਝੂਠ;
        ਜੇ (N % 32 != 0)
            ਵਾਪਸ ਝੂਠ;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    ਨਹੀਂਤਾਂ ਜੇ(!atrans && btrans)
    {
        ਜੇ (K % 4 != 0)
            ਵਾਪਸ ਝੂਠ;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (M % 32 != 0)
            ਵਾਪਸ ਝੂਠ;
        ਜੇ (N % 32 != 0)
            ਵਾਪਸ ਝੂਠ;
        ਜੇ (K % 16 != 0)
            ਵਾਪਸ ਝੂਠ;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(ਨਿਰਾ, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ਅਚਲ size_t gx = div_ਉੱਪਰ((size_t)N, dx);
    ਅਚਲ size_t gy = div_ਉੱਪਰ((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {ਗੇੜਾ_ਉੱਪਰ(gx, lx), ਗੇੜਾ_ਉੱਪਰ(gy, ly), 1};

    ocl::ਕਤਾਰ q;
    String errmsg;
    ਅਚਲ ocl::ਪ੍ਰੋਗਰਾਮ_2 ਪ੍ਰੋਗਰਾਮ = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    ਜੇ(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), ਪ੍ਰੋਗਰਾਮ);
        ਜੇ (k.empty())
        {
            ਵਾਪਸ ਝੂਠ;
        }

        k.ਦਲੀਲਾਂ(ocl::KernelArg::PtrReadOnly(A),
               (ਪੂਰਨ) (A.offset / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
               ocl::KernelArg::PtrReadOnly(B),
               (ਪੂਰਨ) (B.offset / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
               ocl::KernelArg::PtrWriteOnly(D),
               (ਪੂਰਨ) (D.offset / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
               M, N, K,
               (ਦਸ਼ਮਲਵ)alpha,
               (ਦਸ਼ਮਲਵ)beta,
               (ਪੂਰਨ)(A.ਕਦਮ / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
               (ਪੂਰਨ)(B.ਕਦਮ / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
               (ਪੂਰਨ)(D.ਕਦਮ / ਆਕਾਰ(ਦਸ਼ਮਲਵ))
        );

        ਤਾਰਕਿਕ ret = k.ਲੜੀਵਾਰ(2, global, local, ਝੂਠ, q);
        ਵਾਪਸ ret;
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        ਲਈ(ਪੂਰਨ ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ = 0; ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ < K; ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ += stride)
        {
             ocl::Kernel k(kernelName.c_str(), ਪ੍ਰੋਗਰਾਮ);
             k.ਦਲੀਲਾਂ(ocl::KernelArg::PtrReadOnly(A),
                    (ਪੂਰਨ) (A.offset / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (ਪੂਰਨ) (B.offset / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (ਪੂਰਨ) (D.offset / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
                    M, N, K,
                    (ਦਸ਼ਮਲਵ)alpha,
                    (ਦਸ਼ਮਲਵ)beta,
                    (ਪੂਰਨ)(A.ਕਦਮ / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
                    (ਪੂਰਨ)(B.ਕਦਮ / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
                    (ਪੂਰਨ)(D.ਕਦਮ / ਆਕਾਰ(ਦਸ਼ਮਲਵ)),
                    (ਪੂਰਨ) ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ,                          // 14 start_index
                    stride);

            ਤਾਰਕਿਕ ret = k.ਲੜੀਵਾਰ(2, global, local, ਝੂਠ, q);
            ਜੇ (!ret)
            {
                ਜੇ (ਸ਼ੁਰੂ_ਸੂਚਕਾਂਕ != 0)
                    isPropagatedC2D = ਝੂਠ;  // D array content is changed, need to rewrite
                ਵਾਪਸ ਝੂਠ;
            }
        }
        ਵਾਪਸ ਸੱਚ;
    }
}

} // namespace cv

#ਜੇ_ਅੰਤ
