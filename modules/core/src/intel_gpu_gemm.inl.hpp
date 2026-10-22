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

नामाकाश cv
{

स्थिर तार्किक intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    द्विगुण alpha, द्विगुण beta,
    तार्किक atrans, तार्किक btrans,
    तार्किक& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    पूर्णाङ्क M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    जइ (M < 4 || N < 4 || K < 4)  // vload4
        प्रत्यागम मिच्छ;

    CV_LOG_VERBOSE(रिक्त, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned पूर्णाङ्क lx = 8, ly = 4;
    unsigned पूर्णाङ्क dx = 4, dy = 8;

    जइ(!atrans && !btrans)
    {
        जइ (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        अण्णहा
        {
            जइ (M % 2 != 0)
                प्रत्यागम मिच्छ;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            जइ (N % (4*8) != 0)
                प्रत्यागम मिच्छ;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    अण्णहा जइ(atrans && !btrans)
    {
        जइ (M % 32 != 0)
            प्रत्यागम मिच्छ;
        जइ (N % 32 != 0)
            प्रत्यागम मिच्छ;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    अण्णहा जइ(!atrans && btrans)
    {
        जइ (K % 4 != 0)
            प्रत्यागम मिच्छ;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    अण्णहा
    {
        जइ (M % 32 != 0)
            प्रत्यागम मिच्छ;
        जइ (N % 32 != 0)
            प्रत्यागम मिच्छ;
        जइ (K % 16 != 0)
            प्रत्यागम मिच्छ;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(रिक्त, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ध्रुव size_t gx = divUp((size_t)N, dx);
    ध्रुव size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    ध्रुव ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    जइ(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        जइ (k.empty())
        {
            प्रत्यागम मिच्छ;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (पूर्णाङ्क) (A.offset / परिमाण(भिन्न)),
               ocl::KernelArg::PtrReadOnly(B),
               (पूर्णाङ्क) (B.offset / परिमाण(भिन्न)),
               ocl::KernelArg::PtrWriteOnly(D),
               (पूर्णाङ्क) (D.offset / परिमाण(भिन्न)),
               M, N, K,
               (भिन्न)alpha,
               (भिन्न)beta,
               (पूर्णाङ्क)(A.step / परिमाण(भिन्न)),
               (पूर्णाङ्क)(B.step / परिमाण(भिन्न)),
               (पूर्णाङ्क)(D.step / परिमाण(भिन्न))
        );

        तार्किक ret = k.run(2, global, local, मिच्छ, q);
        प्रत्यागम ret;
    }
    अण्णहा
    {
        पूर्णाङ्क stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        प्रति(पूर्णाङ्क start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (पूर्णाङ्क) (A.offset / परिमाण(भिन्न)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (पूर्णाङ्क) (B.offset / परिमाण(भिन्न)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (पूर्णाङ्क) (D.offset / परिमाण(भिन्न)),
                    M, N, K,
                    (भिन्न)alpha,
                    (भिन्न)beta,
                    (पूर्णाङ्क)(A.step / परिमाण(भिन्न)),
                    (पूर्णाङ्क)(B.step / परिमाण(भिन्न)),
                    (पूर्णाङ्क)(D.step / परिमाण(भिन्न)),
                    (पूर्णाङ्क) start_index,                          // 14 start_index
                    stride);

            तार्किक ret = k.run(2, global, local, मिच्छ, q);
            जइ (!ret)
            {
                जइ (start_index != 0)
                    isPropagatedC2D = मिच्छ;  // D array content is changed, need to rewrite
                प्रत्यागम मिच्छ;
            }
        }
        प्रत्यागम सच्च;
    }
}

} // namespace cv

#endif
