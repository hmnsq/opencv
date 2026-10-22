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

স্থির তার্কিক intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    দ্ৱিগুণ alpha, দ্ৱিগুণ beta,
    তার্কিক atrans, তার্কিক btrans,
    তার্কিক& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    পূর্ণাঙ্ক M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    যদি (M < 4 || N < 4 || K < 4)  // vload4
        ঘূৰাই মিছা;

    CV_LOG_VERBOSE(রিক্ত, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned পূর্ণাঙ্ক lx = 8, ly = 4;
    unsigned পূর্ণাঙ্ক dx = 4, dy = 8;

    যদি(!atrans && !btrans)
    {
        যদি (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        অন্যথা
        {
            যদি (M % 2 != 0)
                ঘূৰাই মিছা;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            যদি (N % (4*8) != 0)
                ঘূৰাই মিছা;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    অন্যথা যদি(atrans && !btrans)
    {
        যদি (M % 32 != 0)
            ঘূৰাই মিছা;
        যদি (N % 32 != 0)
            ঘূৰাই মিছা;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    অন্যথা যদি(!atrans && btrans)
    {
        যদি (K % 4 != 0)
            ঘূৰাই মিছা;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    অন্যথা
    {
        যদি (M % 32 != 0)
            ঘূৰাই মিছা;
        যদি (N % 32 != 0)
            ঘূৰাই মিছা;
        যদি (K % 16 != 0)
            ঘূৰাই মিছা;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(রিক্ত, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ধ্রুৱ size_t gx = divUp((size_t)N, dx);
    ধ্রুৱ size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    ধ্রুৱ ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    যদি(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        যদি (k.empty())
        {
            ঘূৰাই মিছা;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (পূর্ণাঙ্ক) (A.offset / পরিমাণ(ভিন্ন)),
               ocl::KernelArg::PtrReadOnly(B),
               (পূর্ণাঙ্ক) (B.offset / পরিমাণ(ভিন্ন)),
               ocl::KernelArg::PtrWriteOnly(D),
               (পূর্ণাঙ্ক) (D.offset / পরিমাণ(ভিন্ন)),
               M, N, K,
               (ভিন্ন)alpha,
               (ভিন্ন)beta,
               (পূর্ণাঙ্ক)(A.step / পরিমাণ(ভিন্ন)),
               (পূর্ণাঙ্ক)(B.step / পরিমাণ(ভিন্ন)),
               (পূর্ণাঙ্ক)(D.step / পরিমাণ(ভিন্ন))
        );

        তার্কিক ret = k.run(2, global, local, মিছা, q);
        ঘূৰাই ret;
    }
    অন্যথা
    {
        পূর্ণাঙ্ক stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        বাবে(পূর্ণাঙ্ক start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (পূর্ণাঙ্ক) (A.offset / পরিমাণ(ভিন্ন)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (পূর্ণাঙ্ক) (B.offset / পরিমাণ(ভিন্ন)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (পূর্ণাঙ্ক) (D.offset / পরিমাণ(ভিন্ন)),
                    M, N, K,
                    (ভিন্ন)alpha,
                    (ভিন্ন)beta,
                    (পূর্ণাঙ্ক)(A.step / পরিমাণ(ভিন্ন)),
                    (পূর্ণাঙ্ক)(B.step / পরিমাণ(ভিন্ন)),
                    (পূর্ণাঙ্ক)(D.step / পরিমাণ(ভিন্ন)),
                    (পূর্ণাঙ্ক) start_index,                          // 14 start_index
                    stride);

            তার্কিক ret = k.run(2, global, local, মিছা, q);
            যদি (!ret)
            {
                যদি (start_index != 0)
                    isPropagatedC2D = মিছা;  // D array content is changed, need to rewrite
                ঘূৰাই মিছা;
            }
        }
        ঘূৰাই সঁচা;
    }
}

} // namespace cv

#endif
