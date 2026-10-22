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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ನಾಮಾಕಾಶ cv
{

ಸ್ಥಿರ ತಾರ್ಕಿಕ intel_gpu_gemm(
    UMat A, ಗಾತ್ರ_2 ಗಾತ್ರ_A,
    UMat B, ಗಾತ್ರ_2 ಗಾತ್ರ_B,
    UMat D, ಗಾತ್ರ_2 ಗಾತ್ರ_D,
    ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta,
    ತಾರ್ಕಿಕ atrans, ತಾರ್ಕಿಕ btrans,
    ತಾರ್ಕಿಕ& isPropagatedC2D
)
{
    CV_ಬಳಸದ(ಗಾತ್ರ_B);

    ಪೂರ್ಣಾಂಕ M = ಗಾತ್ರ_D.ಎತ್ತರ, N = ಗಾತ್ರ_D.ಅಗಲ, K = ((atrans)? ಗಾತ್ರ_A.ಎತ್ತರ : ಗಾತ್ರ_A.ಅಗಲ);

    ಆದರೆ (M < 4 || N < 4 || K < 4)  // vload4
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    CV_LOG_VERBOSE(ಖಾಲಿ, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ lx = 8, ly = 4;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ dx = 4, dy = 8;

    ಆದರೆ(!atrans && !btrans)
    {
        ಆದರೆ (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (M % 2 != 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            ಆದರೆ (N % (4*8) != 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(atrans && !btrans)
    {
        ಆದರೆ (M % 32 != 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಆದರೆ (N % 32 != 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(!atrans && btrans)
    {
        ಆದರೆ (K % 4 != 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (M % 32 != 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಆದರೆ (N % 32 != 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಆದರೆ (K % 16 != 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(ಖಾಲಿ, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ಸ್ಥಿರಾಂಕ size_t gx = div_ಮೇಲೆ((size_t)N, dx);
    ಸ್ಥಿರಾಂಕ size_t gy = div_ಮೇಲೆ((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {ಸುತ್ತು_ಮೇಲೆ(gx, lx), ಸುತ್ತು_ಮೇಲೆ(gy, ly), 1};

    ocl::ಸರತಿ q;
    String errmsg;
    ಸ್ಥಿರಾಂಕ ocl::ಕಾರ್ಯಕ್ರಮ_2 ಕಾರ್ಯಕ್ರಮ = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    ಆದರೆ(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), ಕಾರ್ಯಕ್ರಮ);
        ಆದರೆ (k.empty())
        {
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }

        k.ವಾದಗಳು(ocl::KernelArg::PtrReadOnly(A),
               (ಪೂರ್ಣಾಂಕ) (A.offset / ಗಾತ್ರ(ದಶಮಾಂಶ)),
               ocl::KernelArg::PtrReadOnly(B),
               (ಪೂರ್ಣಾಂಕ) (B.offset / ಗಾತ್ರ(ದಶಮಾಂಶ)),
               ocl::KernelArg::PtrWriteOnly(D),
               (ಪೂರ್ಣಾಂಕ) (D.offset / ಗಾತ್ರ(ದಶಮಾಂಶ)),
               M, N, K,
               (ದಶಮಾಂಶ)alpha,
               (ದಶಮಾಂಶ)beta,
               (ಪೂರ್ಣಾಂಕ)(A.ಹೆಜ್ಜೆ / ಗಾತ್ರ(ದಶಮಾಂಶ)),
               (ಪೂರ್ಣಾಂಕ)(B.ಹೆಜ್ಜೆ / ಗಾತ್ರ(ದಶಮಾಂಶ)),
               (ಪೂರ್ಣಾಂಕ)(D.ಹೆಜ್ಜೆ / ಗಾತ್ರ(ದಶಮಾಂಶ))
        );

        ತಾರ್ಕಿಕ ret = k.ಸತತ(2, global, local, ಸುಳ್ಳು, q);
        ಹಿಂತಿರುಗಿ ret;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        ಗಾಗಿ(ಪೂರ್ಣಾಂಕ ಆರಂಭ_ಸೂಚ್ಯಂಕ = 0; ಆರಂಭ_ಸೂಚ್ಯಂಕ < K; ಆರಂಭ_ಸೂಚ್ಯಂಕ += stride)
        {
             ocl::Kernel k(kernelName.c_str(), ಕಾರ್ಯಕ್ರಮ);
             k.ವಾದಗಳು(ocl::KernelArg::PtrReadOnly(A),
                    (ಪೂರ್ಣಾಂಕ) (A.offset / ಗಾತ್ರ(ದಶಮಾಂಶ)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (ಪೂರ್ಣಾಂಕ) (B.offset / ಗಾತ್ರ(ದಶಮಾಂಶ)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (ಪೂರ್ಣಾಂಕ) (D.offset / ಗಾತ್ರ(ದಶಮಾಂಶ)),
                    M, N, K,
                    (ದಶಮಾಂಶ)alpha,
                    (ದಶಮಾಂಶ)beta,
                    (ಪೂರ್ಣಾಂಕ)(A.ಹೆಜ್ಜೆ / ಗಾತ್ರ(ದಶಮಾಂಶ)),
                    (ಪೂರ್ಣಾಂಕ)(B.ಹೆಜ್ಜೆ / ಗಾತ್ರ(ದಶಮಾಂಶ)),
                    (ಪೂರ್ಣಾಂಕ)(D.ಹೆಜ್ಜೆ / ಗಾತ್ರ(ದಶಮಾಂಶ)),
                    (ಪೂರ್ಣಾಂಕ) ಆರಂಭ_ಸೂಚ್ಯಂಕ,                          // 14 start_index
                    stride);

            ತಾರ್ಕಿಕ ret = k.ಸತತ(2, global, local, ಸುಳ್ಳು, q);
            ಆದರೆ (!ret)
            {
                ಆದರೆ (ಆರಂಭ_ಸೂಚ್ಯಂಕ != 0)
                    isPropagatedC2D = ಸುಳ್ಳು;  // D array content is changed, need to rewrite
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
            }
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
}

} // namespace cv

#ಆದರೆ_ಅಂತ್ಯ
