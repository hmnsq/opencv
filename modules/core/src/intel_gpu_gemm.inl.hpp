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

#अगर_घोषित HAVE_OPENCL

namespace cv
{

अटल bool intel_gpu_gemm(
    UMat A, आकार आकार_A,
    UMat B, आकार आकार_B,
    UMat D, आकार आकार_D,
    द्विगुन alpha, द्विगुन beta,
    bool atrans, bool btrans,
    bool& isPropagatedC2D
)
{
    CV_UNUSED(आकार_B);

    पूर्णांक M = आकार_D.height, N = आकार_D.width, K = ((atrans)? आकार_A.height : आकार_A.width);

    अगर (M < 4 || N < 4 || K < 4)  // vload4
        वापस false;

    CV_LOG_VERBOSE(शून्य, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    अचिन्हित पूर्णांक lx = 8, ly = 4;
    अचिन्हित पूर्णांक dx = 4, dy = 8;

    अगर(!atrans && !btrans)
    {
        अगर (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        अन्यथा
        {
            अगर (M % 2 != 0)
                वापस false;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            अगर (N % (4*8) != 0)
                वापस false;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    अन्यथा अगर(atrans && !btrans)
    {
        अगर (M % 32 != 0)
            वापस false;
        अगर (N % 32 != 0)
            वापस false;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    अन्यथा अगर(!atrans && btrans)
    {
        अगर (K % 4 != 0)
            वापस false;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    अन्यथा
    {
        अगर (M % 32 != 0)
            वापस false;
        अगर (N % 32 != 0)
            वापस false;
        अगर (K % 16 != 0)
            वापस false;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(शून्य, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    स्थिर माप_प्रकार gx = divUp((माप_प्रकार)N, dx);
    स्थिर माप_प्रकार gy = divUp((माप_प्रकार)M, dy);

    माप_प्रकार local[] = {lx, ly, 1};
    माप_प्रकार global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    स्थिर ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    अगर(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        अगर (k.empty())
        {
            वापस false;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (पूर्णांक) (A.offset / माप(भग्न)),
               ocl::KernelArg::PtrReadOnly(B),
               (पूर्णांक) (B.offset / माप(भग्न)),
               ocl::KernelArg::PtrWriteOnly(D),
               (पूर्णांक) (D.offset / माप(भग्न)),
               M, N, K,
               (भग्न)alpha,
               (भग्न)beta,
               (पूर्णांक)(A.step / माप(भग्न)),
               (पूर्णांक)(B.step / माप(भग्न)),
               (पूर्णांक)(D.step / माप(भग्न))
        );

        bool ret = k.run(2, global, local, false, q);
        वापस ret;
    }
    अन्यथा
    {
        पूर्णांक stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        क्रम(पूर्णांक start_सूचकांक = 0; start_सूचकांक < K; start_सूचकांक += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (पूर्णांक) (A.offset / माप(भग्न)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (पूर्णांक) (B.offset / माप(भग्न)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (पूर्णांक) (D.offset / माप(भग्न)),
                    M, N, K,
                    (भग्न)alpha,
                    (भग्न)beta,
                    (पूर्णांक)(A.step / माप(भग्न)),
                    (पूर्णांक)(B.step / माप(भग्न)),
                    (पूर्णांक)(D.step / माप(भग्न)),
                    (पूर्णांक) start_सूचकांक,                          // 14 start_index
                    stride);

            bool ret = k.run(2, global, local, false, q);
            अगर (!ret)
            {
                अगर (start_सूचकांक != 0)
                    isPropagatedC2D = false;  // D array content is changed, need to rewrite
                वापस false;
            }
        }
        वापस true;
    }
}

} // namespace cv

#पूर्ण_अगर
