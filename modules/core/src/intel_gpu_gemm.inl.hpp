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
    dviguu_na alpha, dviguu_na beta,
    bool atrans, bool btrans,
    bool& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    จำนวนเต็ม M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    ถ้า (M < 4 || N < 4 || K < 4)  // vload4
        คืน false;

    CV_LOG_VERBOSE(NULL, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned จำนวนเต็ม lx = 8, ly = 4;
    unsigned จำนวนเต็ม dx = 4, dy = 8;

    ถ้า(!atrans && !btrans)
    {
        ถ้า (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        มิฉะนั้น
        {
            ถ้า (M % 2 != 0)
                คืน false;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            ถ้า (N % (4*8) != 0)
                คืน false;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    มิฉะนั้น ถ้า(atrans && !btrans)
    {
        ถ้า (M % 32 != 0)
            คืน false;
        ถ้า (N % 32 != 0)
            คืน false;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    มิฉะนั้น ถ้า(!atrans && btrans)
    {
        ถ้า (K % 4 != 0)
            คืน false;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    มิฉะนั้น
    {
        ถ้า (M % 32 != 0)
            คืน false;
        ถ้า (N % 32 != 0)
            คืน false;
        ถ้า (K % 16 != 0)
            คืน false;
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

    ถ้า(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        ถ้า (k.empty())
        {
            คืน false;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (จำนวนเต็ม) (A.offset / parimaa_na(float)),
               ocl::KernelArg::PtrReadOnly(B),
               (จำนวนเต็ม) (B.offset / parimaa_na(float)),
               ocl::KernelArg::PtrWriteOnly(D),
               (จำนวนเต็ม) (D.offset / parimaa_na(float)),
               M, N, K,
               (float)alpha,
               (float)beta,
               (จำนวนเต็ม)(A.step / parimaa_na(float)),
               (จำนวนเต็ม)(B.step / parimaa_na(float)),
               (จำนวนเต็ม)(D.step / parimaa_na(float))
        );

        bool ret = k.run(2, global, local, false, q);
        คืน ret;
    }
    มิฉะนั้น
    {
        จำนวนเต็ม stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        สำหรับ(จำนวนเต็ม start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (จำนวนเต็ม) (A.offset / parimaa_na(float)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (จำนวนเต็ม) (B.offset / parimaa_na(float)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (จำนวนเต็ม) (D.offset / parimaa_na(float)),
                    M, N, K,
                    (float)alpha,
                    (float)beta,
                    (จำนวนเต็ม)(A.step / parimaa_na(float)),
                    (จำนวนเต็ม)(B.step / parimaa_na(float)),
                    (จำนวนเต็ม)(D.step / parimaa_na(float)),
                    (จำนวนเต็ม) start_index,                          // 14 start_index
                    stride);

            bool ret = k.run(2, global, local, false, q);
            ถ้า (!ret)
            {
                ถ้า (start_index != 0)
                    isPropagatedC2D = false;  // D array content is changed, need to rewrite
                คืน false;
            }
        }
        คืน true;
    }
}

} // namespace cv

#endif
