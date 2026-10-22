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

നാമാകാശ cv
{

സ്ഥിതം തർക്കം intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    ഇരട്ട alpha, ഇരട്ട beta,
    തർക്കം atrans, തർക്കം btrans,
    തർക്കം& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    പൂർണ്ണസംഖ്യ M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    എങ്കിൽ (M < 4 || N < 4 || K < 4)  // vload4
        തിരികെ അസത്യം;

    CV_LOG_VERBOSE(ശൂന്യ, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned പൂർണ്ണസംഖ്യ lx = 8, ly = 4;
    unsigned പൂർണ്ണസംഖ്യ dx = 4, dy = 8;

    എങ്കിൽ(!atrans && !btrans)
    {
        എങ്കിൽ (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        അല്ലെങ്കിൽ
        {
            എങ്കിൽ (M % 2 != 0)
                തിരികെ അസത്യം;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            എങ്കിൽ (N % (4*8) != 0)
                തിരികെ അസത്യം;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ(atrans && !btrans)
    {
        എങ്കിൽ (M % 32 != 0)
            തിരികെ അസത്യം;
        എങ്കിൽ (N % 32 != 0)
            തിരികെ അസത്യം;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    അല്ലെങ്കിൽ എങ്കിൽ(!atrans && btrans)
    {
        എങ്കിൽ (K % 4 != 0)
            തിരികെ അസത്യം;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (M % 32 != 0)
            തിരികെ അസത്യം;
        എങ്കിൽ (N % 32 != 0)
            തിരികെ അസത്യം;
        എങ്കിൽ (K % 16 != 0)
            തിരികെ അസത്യം;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(ശൂന്യ, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    സ്ഥിരം size_t gx = divUp((size_t)N, dx);
    സ്ഥിരം size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    സ്ഥിരം ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    എങ്കിൽ(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        എങ്കിൽ (k.empty())
        {
            തിരികെ അസത്യം;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (പൂർണ്ണസംഖ്യ) (A.offset / വലുപ്പം(ദശാംശം)),
               ocl::KernelArg::PtrReadOnly(B),
               (പൂർണ്ണസംഖ്യ) (B.offset / വലുപ്പം(ദശാംശം)),
               ocl::KernelArg::PtrWriteOnly(D),
               (പൂർണ്ണസംഖ്യ) (D.offset / വലുപ്പം(ദശാംശം)),
               M, N, K,
               (ദശാംശം)alpha,
               (ദശാംശം)beta,
               (പൂർണ്ണസംഖ്യ)(A.step / വലുപ്പം(ദശാംശം)),
               (പൂർണ്ണസംഖ്യ)(B.step / വലുപ്പം(ദശാംശം)),
               (പൂർണ്ണസംഖ്യ)(D.step / വലുപ്പം(ദശാംശം))
        );

        തർക്കം ret = k.run(2, global, local, അസത്യം, q);
        തിരികെ ret;
    }
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        വേണ്ടി(പൂർണ്ണസംഖ്യ start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (പൂർണ്ണസംഖ്യ) (A.offset / വലുപ്പം(ദശാംശം)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (പൂർണ്ണസംഖ്യ) (B.offset / വലുപ്പം(ദശാംശം)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (പൂർണ്ണസംഖ്യ) (D.offset / വലുപ്പം(ദശാംശം)),
                    M, N, K,
                    (ദശാംശം)alpha,
                    (ദശാംശം)beta,
                    (പൂർണ്ണസംഖ്യ)(A.step / വലുപ്പം(ദശാംശം)),
                    (പൂർണ്ണസംഖ്യ)(B.step / വലുപ്പം(ദശാംശം)),
                    (പൂർണ്ണസംഖ്യ)(D.step / വലുപ്പം(ദശാംശം)),
                    (പൂർണ്ണസംഖ്യ) start_index,                          // 14 start_index
                    stride);

            തർക്കം ret = k.run(2, global, local, അസത്യം, q);
            എങ്കിൽ (!ret)
            {
                എങ്കിൽ (start_index != 0)
                    isPropagatedC2D = അസത്യം;  // D array content is changed, need to rewrite
                തിരികെ അസത്യം;
            }
        }
        തിരികെ സത്യം;
    }
}

} // namespace cv

#endif
