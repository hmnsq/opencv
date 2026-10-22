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

#यदि_परिभाषितम् HAVE_OPENCL

नामाकाश cv
{

स्थिर तार्किक intel_gpu_gemm(
    UMat A, आकार आकार_A,
    UMat B, आकार आकार_B,
    UMat D, आकार आकार_D,
    द्विगुण alpha, द्विगुण beta,
    तार्किक atrans, तार्किक btrans,
    तार्किक& isPropagatedC2D
)
{
    CV_अप्रयुक्तम्(आकार_B);

    पूर्णाङ्क M = आकार_D.height, N = आकार_D.विस्तारः, K = ((atrans)? आकार_A.height : आकार_A.विस्तारः);

    यदि (M < 4 || N < 4 || K < 4)  // vload4
        प्रत्यागम असत्य;

    CV_LOG_VERBOSE(रिक्त, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    अचिह्नित पूर्णाङ्क lx = 8, ly = 4;
    अचिह्नित पूर्णाङ्क dx = 4, dy = 8;

    यदि(!atrans && !btrans)
    {
        यदि (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        अन्यथा
        {
            यदि (M % 2 != 0)
                प्रत्यागम असत्य;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            यदि (N % (4*8) != 0)
                प्रत्यागम असत्य;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    अन्यथा यदि(atrans && !btrans)
    {
        यदि (M % 32 != 0)
            प्रत्यागम असत्य;
        यदि (N % 32 != 0)
            प्रत्यागम असत्य;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    अन्यथा यदि(!atrans && btrans)
    {
        यदि (K % 4 != 0)
            प्रत्यागम असत्य;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    अन्यथा
    {
        यदि (M % 32 != 0)
            प्रत्यागम असत्य;
        यदि (N % 32 != 0)
            प्रत्यागम असत्य;
        यदि (K % 16 != 0)
            प्रत्यागम असत्य;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(रिक्त, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ध्रुव size_t gx = div_ऊर्ध्वम्((size_t)N, dx);
    ध्रुव size_t gy = div_ऊर्ध्वम्((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {आवर्तनम्_ऊर्ध्वम्(gx, lx), आवर्तनम्_ऊर्ध्वम्(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    ध्रुव ocl::क्रमादेशः_2 क्रमादेशः = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    यदि(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), क्रमादेशः);
        यदि (k.empty())
        {
            प्रत्यागम असत्य;
        }

        k.तर्काः(ocl::KernelArg::PtrReadOnly(A),
               (पूर्णाङ्क) (A.offset / परिमाण(भिन्न)),
               ocl::KernelArg::PtrReadOnly(B),
               (पूर्णाङ्क) (B.offset / परिमाण(भिन्न)),
               ocl::KernelArg::PtrWriteOnly(D),
               (पूर्णाङ्क) (D.offset / परिमाण(भिन्न)),
               M, N, K,
               (भिन्न)alpha,
               (भिन्न)beta,
               (पूर्णाङ्क)(A.चरणः / परिमाण(भिन्न)),
               (पूर्णाङ्क)(B.चरणः / परिमाण(भिन्न)),
               (पूर्णाङ्क)(D.चरणः / परिमाण(भिन्न))
        );

        तार्किक ret = k.सततम्(2, global, local, असत्य, q);
        प्रत्यागम ret;
    }
    अन्यथा
    {
        पूर्णाङ्क stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        प्रति(पूर्णाङ्क start_सूचकांक = 0; start_सूचकांक < K; start_सूचकांक += stride)
        {
             ocl::Kernel k(kernelName.c_str(), क्रमादेशः);
             k.तर्काः(ocl::KernelArg::PtrReadOnly(A),
                    (पूर्णाङ्क) (A.offset / परिमाण(भिन्न)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (पूर्णाङ्क) (B.offset / परिमाण(भिन्न)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (पूर्णाङ्क) (D.offset / परिमाण(भिन्न)),
                    M, N, K,
                    (भिन्न)alpha,
                    (भिन्न)beta,
                    (पूर्णाङ्क)(A.चरणः / परिमाण(भिन्न)),
                    (पूर्णाङ्क)(B.चरणः / परिमाण(भिन्न)),
                    (पूर्णाङ्क)(D.चरणः / परिमाण(भिन्न)),
                    (पूर्णाङ्क) start_सूचकांक,                          // 14 start_index
                    stride);

            तार्किक ret = k.सततम्(2, global, local, असत्य, q);
            यदि (!ret)
            {
                यदि (start_सूचकांक != 0)
                    isPropagatedC2D = असत्य;  // D array content is changed, need to rewrite
                प्रत्यागम असत्य;
            }
        }
        प्रत्यागम सत्य;
    }
}

} // namespace cv

#यदि_अन्तः
