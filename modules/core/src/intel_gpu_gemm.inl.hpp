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

நாமாகாஶ cv
{

நிலையான தருக்க intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    இரட்டை alpha, இரட்டை beta,
    தருக்க atrans, தருக்க btrans,
    தருக்க& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    முழு M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    எனில் (M < 4 || N < 4 || K < 4)  // vload4
        திரும்பு பொய்;

    CV_LOG_VERBOSE(வெற்று, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned முழு lx = 8, ly = 4;
    unsigned முழு dx = 4, dy = 8;

    எனில்(!atrans && !btrans)
    {
        எனில் (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        இல்லையெனில்
        {
            எனில் (M % 2 != 0)
                திரும்பு பொய்;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            எனில் (N % (4*8) != 0)
                திரும்பு பொய்;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    இல்லையெனில் எனில்(atrans && !btrans)
    {
        எனில் (M % 32 != 0)
            திரும்பு பொய்;
        எனில் (N % 32 != 0)
            திரும்பு பொய்;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    இல்லையெனில் எனில்(!atrans && btrans)
    {
        எனில் (K % 4 != 0)
            திரும்பு பொய்;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    இல்லையெனில்
    {
        எனில் (M % 32 != 0)
            திரும்பு பொய்;
        எனில் (N % 32 != 0)
            திரும்பு பொய்;
        எனில் (K % 16 != 0)
            திரும்பு பொய்;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(வெற்று, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    மாறிலி size_t gx = divUp((size_t)N, dx);
    மாறிலி size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    மாறிலி ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    எனில்(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        எனில் (k.empty())
        {
            திரும்பு பொய்;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (முழு) (A.offset / அளவு(தசம)),
               ocl::KernelArg::PtrReadOnly(B),
               (முழு) (B.offset / அளவு(தசம)),
               ocl::KernelArg::PtrWriteOnly(D),
               (முழு) (D.offset / அளவு(தசம)),
               M, N, K,
               (தசம)alpha,
               (தசம)beta,
               (முழு)(A.step / அளவு(தசம)),
               (முழு)(B.step / அளவு(தசம)),
               (முழு)(D.step / அளவு(தசம))
        );

        தருக்க ret = k.run(2, global, local, பொய், q);
        திரும்பு ret;
    }
    இல்லையெனில்
    {
        முழு stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        க்கு(முழு start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (முழு) (A.offset / அளவு(தசம)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (முழு) (B.offset / அளவு(தசம)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (முழு) (D.offset / அளவு(தசம)),
                    M, N, K,
                    (தசம)alpha,
                    (தசம)beta,
                    (முழு)(A.step / அளவு(தசம)),
                    (முழு)(B.step / அளவு(தசம)),
                    (முழு)(D.step / அளவு(தசம)),
                    (முழு) start_index,                          // 14 start_index
                    stride);

            தருக்க ret = k.run(2, global, local, பொய், q);
            எனில் (!ret)
            {
                எனில் (start_index != 0)
                    isPropagatedC2D = பொய்;  // D array content is changed, need to rewrite
                திரும்பு பொய்;
            }
        }
        திரும்பு உண்மை;
    }
}

} // namespace cv

#endif
