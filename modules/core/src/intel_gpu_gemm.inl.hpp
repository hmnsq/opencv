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

فضاینام cv
{

ایستا منطقی intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    دوگانه alpha, دوگانه beta,
    منطقی atrans, منطقی btrans,
    منطقی& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    صحیح M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    اگر (M < 4 || N < 4 || K < 4)  // vload4
        بازگشت نادرست;

    CV_LOG_VERBOSE(پوچ, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned صحیح lx = 8, ly = 4;
    unsigned صحیح dx = 4, dy = 8;

    اگر(!atrans && !btrans)
    {
        اگر (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        وگرنه
        {
            اگر (M % 2 != 0)
                بازگشت نادرست;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            اگر (N % (4*8) != 0)
                بازگشت نادرست;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    وگرنه اگر(atrans && !btrans)
    {
        اگر (M % 32 != 0)
            بازگشت نادرست;
        اگر (N % 32 != 0)
            بازگشت نادرست;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    وگرنه اگر(!atrans && btrans)
    {
        اگر (K % 4 != 0)
            بازگشت نادرست;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    وگرنه
    {
        اگر (M % 32 != 0)
            بازگشت نادرست;
        اگر (N % 32 != 0)
            بازگشت نادرست;
        اگر (K % 16 != 0)
            بازگشت نادرست;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(پوچ, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ثابت size_t gx = divUp((size_t)N, dx);
    ثابت size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    ثابت ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    اگر(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        اگر (k.empty())
        {
            بازگشت نادرست;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (صحیح) (A.offset / اندازه(اعشاری)),
               ocl::KernelArg::PtrReadOnly(B),
               (صحیح) (B.offset / اندازه(اعشاری)),
               ocl::KernelArg::PtrWriteOnly(D),
               (صحیح) (D.offset / اندازه(اعشاری)),
               M, N, K,
               (اعشاری)alpha,
               (اعشاری)beta,
               (صحیح)(A.step / اندازه(اعشاری)),
               (صحیح)(B.step / اندازه(اعشاری)),
               (صحیح)(D.step / اندازه(اعشاری))
        );

        منطقی ret = k.run(2, global, local, نادرست, q);
        بازگشت ret;
    }
    وگرنه
    {
        صحیح stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        برای(صحیح start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (صحیح) (A.offset / اندازه(اعشاری)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (صحیح) (B.offset / اندازه(اعشاری)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (صحیح) (D.offset / اندازه(اعشاری)),
                    M, N, K,
                    (اعشاری)alpha,
                    (اعشاری)beta,
                    (صحیح)(A.step / اندازه(اعشاری)),
                    (صحیح)(B.step / اندازه(اعشاری)),
                    (صحیح)(D.step / اندازه(اعشاری)),
                    (صحیح) start_index,                          // 14 start_index
                    stride);

            منطقی ret = k.run(2, global, local, نادرست, q);
            اگر (!ret)
            {
                اگر (start_index != 0)
                    isPropagatedC2D = نادرست;  // D array content is changed, need to rewrite
                بازگشت نادرست;
            }
        }
        بازگشت درست;
    }
}

} // namespace cv

#endif
