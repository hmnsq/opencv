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

ନାମାକାଶ cv
{

ସ୍ଥିର ତାର୍କିକ intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    ଦ୍ୱିଗୁଣ alpha, ଦ୍ୱିଗୁଣ beta,
    ତାର୍କିକ atrans, ତାର୍କିକ btrans,
    ତାର୍କିକ& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    ଯଦି (M < 4 || N < 4 || K < 4)  // vload4
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    CV_LOG_VERBOSE(ଖାଲି, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lx = 8, ly = 4;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx = 4, dy = 8;

    ଯଦି(!atrans && !btrans)
    {
        ଯଦି (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        ଅନ୍ୟଥା
        {
            ଯଦି (M % 2 != 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            ଯଦି (N % (4*8) != 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    ଅନ୍ୟଥା ଯଦି(atrans && !btrans)
    {
        ଯଦି (M % 32 != 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଯଦି (N % 32 != 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    ଅନ୍ୟଥା ଯଦି(!atrans && btrans)
    {
        ଯଦି (K % 4 != 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (M % 32 != 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଯଦି (N % 32 != 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        ଯଦି (K % 16 != 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(ଖାଲି, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ଅଚଳ size_t gx = divUp((size_t)N, dx);
    ଅଚଳ size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    ଅଚଳ ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    ଯଦି(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        ଯଦି (k.empty())
        {
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (A.offset / ଆକାର(ଦଶମିକ)),
               ocl::KernelArg::PtrReadOnly(B),
               (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (B.offset / ଆକାର(ଦଶମିକ)),
               ocl::KernelArg::PtrWriteOnly(D),
               (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (D.offset / ଆକାର(ଦଶମିକ)),
               M, N, K,
               (ଦଶମିକ)alpha,
               (ଦଶମିକ)beta,
               (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(A.step / ଆକାର(ଦଶମିକ)),
               (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(B.step / ଆକାର(ଦଶମିକ)),
               (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(D.step / ଆକାର(ଦଶମିକ))
        );

        ତାର୍କିକ ret = k.run(2, global, local, ମିଥ୍ୟା, q);
        ଫେରନ୍ତୁ ret;
    }
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (A.offset / ଆକାର(ଦଶମିକ)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (B.offset / ଆକାର(ଦଶମିକ)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (D.offset / ଆକାର(ଦଶମିକ)),
                    M, N, K,
                    (ଦଶମିକ)alpha,
                    (ଦଶମିକ)beta,
                    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(A.step / ଆକାର(ଦଶମିକ)),
                    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(B.step / ଆକାର(ଦଶମିକ)),
                    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(D.step / ଆକାର(ଦଶମିକ)),
                    (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) start_index,                          // 14 start_index
                    stride);

            ତାର୍କିକ ret = k.run(2, global, local, ମିଥ୍ୟା, q);
            ଯଦି (!ret)
            {
                ଯଦି (start_index != 0)
                    isPropagatedC2D = ମିଥ୍ୟା;  // D array content is changed, need to rewrite
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
}

} // namespace cv

#endif
