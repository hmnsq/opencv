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

نطاق cv
{

ساكن منطقي intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    مزدوج alpha, مزدوج beta,
    منطقي atrans, منطقي btrans,
    منطقي& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    صحيح M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    ܐܢ (M < 4 || N < 4 || K < 4)  // vload4
        ܦܢܐ ܕܓܠܐ;

    CV_LOG_VERBOSE(عدم, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned صحيح lx = 8, ly = 4;
    unsigned صحيح dx = 4, dy = 8;

    ܐܢ(!atrans && !btrans)
    {
        ܐܢ (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        ܐܚܪܢܐ
        {
            ܐܢ (M % 2 != 0)
                ܦܢܐ ܕܓܠܐ;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            ܐܢ (N % (4*8) != 0)
                ܦܢܐ ܕܓܠܐ;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    ܐܚܪܢܐ ܐܢ(atrans && !btrans)
    {
        ܐܢ (M % 32 != 0)
            ܦܢܐ ܕܓܠܐ;
        ܐܢ (N % 32 != 0)
            ܦܢܐ ܕܓܠܐ;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    ܐܚܪܢܐ ܐܢ(!atrans && btrans)
    {
        ܐܢ (K % 4 != 0)
            ܦܢܐ ܕܓܠܐ;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    ܐܚܪܢܐ
    {
        ܐܢ (M % 32 != 0)
            ܦܢܐ ܕܓܠܐ;
        ܐܢ (N % 32 != 0)
            ܦܢܐ ܕܓܠܐ;
        ܐܢ (K % 16 != 0)
            ܦܢܐ ܕܓܠܐ;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(عدم, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ثابت size_t gx = divUp((size_t)N, dx);
    ثابت size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    ثابت ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    ܐܢ(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        ܐܢ (k.empty())
        {
            ܦܢܐ ܕܓܠܐ;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (صحيح) (A.offset / حجم(عائم)),
               ocl::KernelArg::PtrReadOnly(B),
               (صحيح) (B.offset / حجم(عائم)),
               ocl::KernelArg::PtrWriteOnly(D),
               (صحيح) (D.offset / حجم(عائم)),
               M, N, K,
               (عائم)alpha,
               (عائم)beta,
               (صحيح)(A.step / حجم(عائم)),
               (صحيح)(B.step / حجم(عائم)),
               (صحيح)(D.step / حجم(عائم))
        );

        منطقي ret = k.run(2, global, local, ܕܓܠܐ, q);
        ܦܢܐ ret;
    }
    ܐܚܪܢܐ
    {
        صحيح stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        لكل(صحيح start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (صحيح) (A.offset / حجم(عائم)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (صحيح) (B.offset / حجم(عائم)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (صحيح) (D.offset / حجم(عائم)),
                    M, N, K,
                    (عائم)alpha,
                    (عائم)beta,
                    (صحيح)(A.step / حجم(عائم)),
                    (صحيح)(B.step / حجم(عائم)),
                    (صحيح)(D.step / حجم(عائم)),
                    (صحيح) start_index,                          // 14 start_index
                    stride);

            منطقي ret = k.run(2, global, local, ܕܓܠܐ, q);
            ܐܢ (!ret)
            {
                ܐܢ (start_index != 0)
                    isPropagatedC2D = ܕܓܠܐ;  // D array content is changed, need to rewrite
                ܦܢܐ ܕܓܠܐ;
            }
        }
        ܦܢܐ ܫܪܪܐ;
    }
}

} // namespace cv

#endif
