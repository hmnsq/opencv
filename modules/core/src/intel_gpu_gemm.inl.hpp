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

    整数 M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    如果 (M < 4 || N < 4 || K < 4)  // vload4
        返回 false;

    CV_LOG_VERBOSE(NULL, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned 整数 lx = 8, ly = 4;
    unsigned 整数 dx = 4, dy = 8;

    如果(!atrans && !btrans)
    {
        如果 (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        否则
        {
            如果 (M % 2 != 0)
                返回 false;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            如果 (N % (4*8) != 0)
                返回 false;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    否则 如果(atrans && !btrans)
    {
        如果 (M % 32 != 0)
            返回 false;
        如果 (N % 32 != 0)
            返回 false;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    否则 如果(!atrans && btrans)
    {
        如果 (K % 4 != 0)
            返回 false;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    否则
    {
        如果 (M % 32 != 0)
            返回 false;
        如果 (N % 32 != 0)
            返回 false;
        如果 (K % 16 != 0)
            返回 false;
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

    如果(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        如果 (k.empty())
        {
            返回 false;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (整数) (A.offset / sizeof(float)),
               ocl::KernelArg::PtrReadOnly(B),
               (整数) (B.offset / sizeof(float)),
               ocl::KernelArg::PtrWriteOnly(D),
               (整数) (D.offset / sizeof(float)),
               M, N, K,
               (float)alpha,
               (float)beta,
               (整数)(A.step / sizeof(float)),
               (整数)(B.step / sizeof(float)),
               (整数)(D.step / sizeof(float))
        );

        bool ret = k.run(2, global, local, false, q);
        返回 ret;
    }
    否则
    {
        整数 stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        为(整数 start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (整数) (A.offset / sizeof(float)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (整数) (B.offset / sizeof(float)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (整数) (D.offset / sizeof(float)),
                    M, N, K,
                    (float)alpha,
                    (float)beta,
                    (整数)(A.step / sizeof(float)),
                    (整数)(B.step / sizeof(float)),
                    (整数)(D.step / sizeof(float)),
                    (整数) start_index,                          // 14 start_index
                    stride);

            bool ret = k.run(2, global, local, false, q);
            如果 (!ret)
            {
                如果 (start_index != 0)
                    isPropagatedC2D = false;  // D array content is changed, need to rewrite
                返回 false;
            }
        }
        返回 true;
    }
}

} // namespace cv

#endif
