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

    ціле M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    якщо (M < 4 || N < 4 || K < 4)  // vload4
        повернути false;

    CV_LOG_VERBOSE(NULL, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned ціле lx = 8, ly = 4;
    unsigned ціле dx = 4, dy = 8;

    якщо(!atrans && !btrans)
    {
        якщо (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        інакше
        {
            якщо (M % 2 != 0)
                повернути false;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            якщо (N % (4*8) != 0)
                повернути false;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    інакше якщо(atrans && !btrans)
    {
        якщо (M % 32 != 0)
            повернути false;
        якщо (N % 32 != 0)
            повернути false;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    інакше якщо(!atrans && btrans)
    {
        якщо (K % 4 != 0)
            повернути false;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    інакше
    {
        якщо (M % 32 != 0)
            повернути false;
        якщо (N % 32 != 0)
            повернути false;
        якщо (K % 16 != 0)
            повернути false;
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

    якщо(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        якщо (k.empty())
        {
            повернути false;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (ціле) (A.offset / sizeof(float)),
               ocl::KernelArg::PtrReadOnly(B),
               (ціле) (B.offset / sizeof(float)),
               ocl::KernelArg::PtrWriteOnly(D),
               (ціле) (D.offset / sizeof(float)),
               M, N, K,
               (float)alpha,
               (float)beta,
               (ціле)(A.step / sizeof(float)),
               (ціле)(B.step / sizeof(float)),
               (ціле)(D.step / sizeof(float))
        );

        bool ret = k.run(2, global, local, false, q);
        повернути ret;
    }
    інакше
    {
        ціле stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        для(ціле start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (ціле) (A.offset / sizeof(float)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (ціле) (B.offset / sizeof(float)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (ціле) (D.offset / sizeof(float)),
                    M, N, K,
                    (float)alpha,
                    (float)beta,
                    (ціле)(A.step / sizeof(float)),
                    (ціле)(B.step / sizeof(float)),
                    (ціле)(D.step / sizeof(float)),
                    (ціле) start_index,                          // 14 start_index
                    stride);

            bool ret = k.run(2, global, local, false, q);
            якщо (!ret)
            {
                якщо (start_index != 0)
                    isPropagatedC2D = false;  // D array content is changed, need to rewrite
                повернути false;
            }
        }
        повернути true;
    }
}

} // namespace cv

#endif
