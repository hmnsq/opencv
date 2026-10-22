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

#ifdef HAVE_OPENCL

ನಾಮಾಕಾಶ cv
{

ಸ್ಥಿರ ತಾರ್ಕಿಕ intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    ದ್ವಿಗುಣ alpha, ದ್ವಿಗುಣ beta,
    ತಾರ್ಕಿಕ atrans, ತಾರ್ಕಿಕ btrans,
    ತಾರ್ಕಿಕ& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    ಪೂರ್ಣಾಙ್ಕ M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    ಯದಿ (M < 4 || N < 4 || K < 4)  // vload4
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned ಪೂರ್ಣಾಙ್ಕ lx = 8, ly = 4;
    unsigned ಪೂರ್ಣಾಙ್ಕ dx = 4, dy = 8;

    ಯದಿ(!atrans && !btrans)
    {
        ಯದಿ (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ (M % 2 != 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            ಯದಿ (N % (4*8) != 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    ಅನ್ಯಥಾ ಯದಿ(atrans && !btrans)
    {
        ಯದಿ (M % 32 != 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಯದಿ (N % 32 != 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    ಅನ್ಯಥಾ ಯದಿ(!atrans && btrans)
    {
        ಯದಿ (K % 4 != 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (M % 32 != 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಯದಿ (N % 32 != 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        ಯದಿ (K % 16 != 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(ರಿಕ್ತ, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ಧ್ರುವ size_t gx = divUp((size_t)N, dx);
    ಧ್ರುವ size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    ಧ್ರುವ ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    ಯದಿ(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        ಯದಿ (k.empty())
        {
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (ಪೂರ್ಣಾಙ್ಕ) (A.offset / ಪರಿಮಾಣ(ಭಿನ್ನ)),
               ocl::KernelArg::PtrReadOnly(B),
               (ಪೂರ್ಣಾಙ್ಕ) (B.offset / ಪರಿಮಾಣ(ಭಿನ್ನ)),
               ocl::KernelArg::PtrWriteOnly(D),
               (ಪೂರ್ಣಾಙ್ಕ) (D.offset / ಪರಿಮಾಣ(ಭಿನ್ನ)),
               M, N, K,
               (ಭಿನ್ನ)alpha,
               (ಭಿನ್ನ)beta,
               (ಪೂರ್ಣಾಙ್ಕ)(A.step / ಪರಿಮಾಣ(ಭಿನ್ನ)),
               (ಪೂರ್ಣಾಙ್ಕ)(B.step / ಪರಿಮಾಣ(ಭಿನ್ನ)),
               (ಪೂರ್ಣಾಙ್ಕ)(D.step / ಪರಿಮಾಣ(ಭಿನ್ನ))
        );

        ತಾರ್ಕಿಕ ret = k.run(2, global, local, ಅಸತ್ಯ, q);
        ಪ್ರತ್ಯಾಗಮ ret;
    }
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (ಪೂರ್ಣಾಙ್ಕ) (A.offset / ಪರಿಮಾಣ(ಭಿನ್ನ)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (ಪೂರ್ಣಾಙ್ಕ) (B.offset / ಪರಿಮಾಣ(ಭಿನ್ನ)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (ಪೂರ್ಣಾಙ್ಕ) (D.offset / ಪರಿಮಾಣ(ಭಿನ್ನ)),
                    M, N, K,
                    (ಭಿನ್ನ)alpha,
                    (ಭಿನ್ನ)beta,
                    (ಪೂರ್ಣಾಙ್ಕ)(A.step / ಪರಿಮಾಣ(ಭಿನ್ನ)),
                    (ಪೂರ್ಣಾಙ್ಕ)(B.step / ಪರಿಮಾಣ(ಭಿನ್ನ)),
                    (ಪೂರ್ಣಾಙ್ಕ)(D.step / ಪರಿಮಾಣ(ಭಿನ್ನ)),
                    (ಪೂರ್ಣಾಙ್ಕ) start_index,                          // 14 start_index
                    stride);

            ತಾರ್ಕಿಕ ret = k.run(2, global, local, ಅಸತ್ಯ, q);
            ಯದಿ (!ret)
            {
                ಯದಿ (start_index != 0)
                    isPropagatedC2D = ಅಸತ್ಯ;  // D array content is changed, need to rewrite
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
}

} // namespace cv

#endif
