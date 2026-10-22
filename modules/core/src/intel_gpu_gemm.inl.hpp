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

namespace cv
{

static bool intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    double alpha, double beta,
    bool atrans, bool btrans,
    bool& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    số_nguyên M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    nếu (M < 4 || N < 4 || K < 4)  // vload4
        trả_về false;

    CV_LOG_VERBOSE(NULL, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned số_nguyên lx = 8, ly = 4;
    unsigned số_nguyên dx = 4, dy = 8;

    nếu(!atrans && !btrans)
    {
        nếu (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        không_thì
        {
            nếu (M % 2 != 0)
                trả_về false;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            nếu (N % (4*8) != 0)
                trả_về false;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    không_thì nếu(atrans && !btrans)
    {
        nếu (M % 32 != 0)
            trả_về false;
        nếu (N % 32 != 0)
            trả_về false;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    không_thì nếu(!atrans && btrans)
    {
        nếu (K % 4 != 0)
            trả_về false;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    không_thì
    {
        nếu (M % 32 != 0)
            trả_về false;
        nếu (N % 32 != 0)
            trả_về false;
        nếu (K % 16 != 0)
            trả_về false;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(NULL, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    const size_t gx = divUp((size_t)N, dx);
    const size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    const ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    nếu(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        nếu (k.empty())
        {
            trả_về false;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (số_nguyên) (A.offset / sizeof(float)),
               ocl::KernelArg::PtrReadOnly(B),
               (số_nguyên) (B.offset / sizeof(float)),
               ocl::KernelArg::PtrWriteOnly(D),
               (số_nguyên) (D.offset / sizeof(float)),
               M, N, K,
               (float)alpha,
               (float)beta,
               (số_nguyên)(A.step / sizeof(float)),
               (số_nguyên)(B.step / sizeof(float)),
               (số_nguyên)(D.step / sizeof(float))
        );

        bool ret = k.run(2, global, local, false, q);
        trả_về ret;
    }
    không_thì
    {
        số_nguyên stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        cho(số_nguyên start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (số_nguyên) (A.offset / sizeof(float)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (số_nguyên) (B.offset / sizeof(float)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (số_nguyên) (D.offset / sizeof(float)),
                    M, N, K,
                    (float)alpha,
                    (float)beta,
                    (số_nguyên)(A.step / sizeof(float)),
                    (số_nguyên)(B.step / sizeof(float)),
                    (số_nguyên)(D.step / sizeof(float)),
                    (số_nguyên) start_index,                          // 14 start_index
                    stride);

            bool ret = k.run(2, global, local, false, q);
            nếu (!ret)
            {
                nếu (start_index != 0)
                    isPropagatedC2D = false;  // D array content is changed, need to rewrite
                trả_về false;
            }
        }
        trả_về true;
    }
}

} // namespace cv

#endif
