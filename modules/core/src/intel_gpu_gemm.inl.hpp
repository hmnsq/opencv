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

নামাকাশ cv
{

স্থির যৌক্তিক intel_gpu_gemm(
    UMat A, আকার_2 আকার_A,
    UMat B, আকার_2 আকার_B,
    UMat D, আকার_2 আকার_D,
    দ্বিগুণ alpha, দ্বিগুণ beta,
    যৌক্তিক atrans, যৌক্তিক btrans,
    যৌক্তিক& isPropagatedC2D
)
{
    CV_UNUSED(আকার_B);

    পূর্ণসংখ্যা M = আকার_D.height, N = আকার_D.width, K = ((atrans)? আকার_A.height : আকার_A.width);

    যদি (M < 4 || N < 4 || K < 4)  // vload4
        ফেরত মিথ্যা;

    CV_LOG_VERBOSE(ফাঁকা, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned পূর্ণসংখ্যা lx = 8, ly = 4;
    unsigned পূর্ণসংখ্যা dx = 4, dy = 8;

    যদি(!atrans && !btrans)
    {
        যদি (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        অন্যথা
        {
            যদি (M % 2 != 0)
                ফেরত মিথ্যা;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            যদি (N % (4*8) != 0)
                ফেরত মিথ্যা;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    অন্যথা যদি(atrans && !btrans)
    {
        যদি (M % 32 != 0)
            ফেরত মিথ্যা;
        যদি (N % 32 != 0)
            ফেরত মিথ্যা;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    অন্যথা যদি(!atrans && btrans)
    {
        যদি (K % 4 != 0)
            ফেরত মিথ্যা;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    অন্যথা
    {
        যদি (M % 32 != 0)
            ফেরত মিথ্যা;
        যদি (N % 32 != 0)
            ফেরত মিথ্যা;
        যদি (K % 16 != 0)
            ফেরত মিথ্যা;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(ফাঁকা, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ধ্রুবক size_t gx = divUp((size_t)N, dx);
    ধ্রুবক size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    ধ্রুবক ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    যদি(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        যদি (k.empty())
        {
            ফেরত মিথ্যা;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (পূর্ণসংখ্যা) (A.offset / আকার(দশমিক)),
               ocl::KernelArg::PtrReadOnly(B),
               (পূর্ণসংখ্যা) (B.offset / আকার(দশমিক)),
               ocl::KernelArg::PtrWriteOnly(D),
               (পূর্ণসংখ্যা) (D.offset / আকার(দশমিক)),
               M, N, K,
               (দশমিক)alpha,
               (দশমিক)beta,
               (পূর্ণসংখ্যা)(A.step / আকার(দশমিক)),
               (পূর্ণসংখ্যা)(B.step / আকার(দশমিক)),
               (পূর্ণসংখ্যা)(D.step / আকার(দশমিক))
        );

        যৌক্তিক ret = k.run(2, global, local, মিথ্যা, q);
        ফেরত ret;
    }
    অন্যথা
    {
        পূর্ণসংখ্যা stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        জন্য(পূর্ণসংখ্যা start_সূচক = 0; start_সূচক < K; start_সূচক += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (পূর্ণসংখ্যা) (A.offset / আকার(দশমিক)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (পূর্ণসংখ্যা) (B.offset / আকার(দশমিক)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (পূর্ণসংখ্যা) (D.offset / আকার(দশমিক)),
                    M, N, K,
                    (দশমিক)alpha,
                    (দশমিক)beta,
                    (পূর্ণসংখ্যা)(A.step / আকার(দশমিক)),
                    (পূর্ণসংখ্যা)(B.step / আকার(দশমিক)),
                    (পূর্ণসংখ্যা)(D.step / আকার(দশমিক)),
                    (পূর্ণসংখ্যা) start_সূচক,                          // 14 start_index
                    stride);

            যৌক্তিক ret = k.run(2, global, local, মিথ্যা, q);
            যদি (!ret)
            {
                যদি (start_সূচক != 0)
                    isPropagatedC2D = মিথ্যা;  // D array content is changed, need to rewrite
                ফেরত মিথ্যা;
            }
        }
        ফেরত সত্য;
    }
}

} // namespace cv

#endif
