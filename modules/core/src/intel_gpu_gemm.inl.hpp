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

નામાકાશ cv
{

સ્થિર તાર્કિક intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    દ્વિગુણ alpha, દ્વિગુણ beta,
    તાર્કિક atrans, તાર્કિક btrans,
    તાર્કિક& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    પૂર્ણાંક M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    જો (M < 4 || N < 4 || K < 4)  // vload4
        પાછા ખોટું;

    CV_LOG_VERBOSE(ખાલી, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    unsigned પૂર્ણાંક lx = 8, ly = 4;
    unsigned પૂર્ણાંક dx = 4, dy = 8;

    જો(!atrans && !btrans)
    {
        જો (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        નહીંતર
        {
            જો (M % 2 != 0)
                પાછા ખોટું;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            જો (N % (4*8) != 0)
                પાછા ખોટું;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    નહીંતર જો(atrans && !btrans)
    {
        જો (M % 32 != 0)
            પાછા ખોટું;
        જો (N % 32 != 0)
            પાછા ખોટું;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    નહીંતર જો(!atrans && btrans)
    {
        જો (K % 4 != 0)
            પાછા ખોટું;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    નહીંતર
    {
        જો (M % 32 != 0)
            પાછા ખોટું;
        જો (N % 32 != 0)
            પાછા ખોટું;
        જો (K % 16 != 0)
            પાછા ખોટું;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(ખાલી, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    અચલ size_t gx = divUp((size_t)N, dx);
    અચલ size_t gy = divUp((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    અચલ ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    જો(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        જો (k.empty())
        {
            પાછા ખોટું;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (પૂર્ણાંક) (A.offset / કદ(દશાંશ)),
               ocl::KernelArg::PtrReadOnly(B),
               (પૂર્ણાંક) (B.offset / કદ(દશાંશ)),
               ocl::KernelArg::PtrWriteOnly(D),
               (પૂર્ણાંક) (D.offset / કદ(દશાંશ)),
               M, N, K,
               (દશાંશ)alpha,
               (દશાંશ)beta,
               (પૂર્ણાંક)(A.step / કદ(દશાંશ)),
               (પૂર્ણાંક)(B.step / કદ(દશાંશ)),
               (પૂર્ણાંક)(D.step / કદ(દશાંશ))
        );

        તાર્કિક ret = k.run(2, global, local, ખોટું, q);
        પાછા ret;
    }
    નહીંતર
    {
        પૂર્ણાંક stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        માટે(પૂર્ણાંક start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (પૂર્ણાંક) (A.offset / કદ(દશાંશ)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (પૂર્ણાંક) (B.offset / કદ(દશાંશ)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (પૂર્ણાંક) (D.offset / કદ(દશાંશ)),
                    M, N, K,
                    (દશાંશ)alpha,
                    (દશાંશ)beta,
                    (પૂર્ણાંક)(A.step / કદ(દશાંશ)),
                    (પૂર્ણાંક)(B.step / કદ(દશાંશ)),
                    (પૂર્ણાંક)(D.step / કદ(દશાંશ)),
                    (પૂર્ણાંક) start_index,                          // 14 start_index
                    stride);

            તાર્કિક ret = k.run(2, global, local, ખોટું, q);
            જો (!ret)
            {
                જો (start_index != 0)
                    isPropagatedC2D = ખોટું;  // D array content is changed, need to rewrite
                પાછા ખોટું;
            }
        }
        પાછા સાચું;
    }
}

} // namespace cv

#endif
