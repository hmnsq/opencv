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

नामाकाश cv
{

स्थिर तार्किक intel_gpu_gemm(
    UMat A, Size sizeA,
    UMat B, Size sizeB,
    UMat D, Size sizeD,
    द्विगुण alpha, द्विगुण beta,
    तार्किक atrans, तार्किक btrans,
    तार्किक& isPropagatedC2D
)
{
    CV_UNUSED(sizeB);

    पूर्णांक M = sizeD.height, N = sizeD.width, K = ((atrans)? sizeA.height : sizeA.width);

    जर (M < 4 || N < 4 || K < 4)  // vload4
        परत असत्य;

    CV_LOG_VERBOSE(रिक्त, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    अचिन्हित पूर्णांक lx = 8, ly = 4;
    अचिन्हित पूर्णांक dx = 4, dy = 8;

    जर(!atrans && !btrans)
    {
        जर (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        अन्यथा
        {
            जर (M % 2 != 0)
                परत असत्य;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            जर (N % (4*8) != 0)
                परत असत्य;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    अन्यथा जर(atrans && !btrans)
    {
        जर (M % 32 != 0)
            परत असत्य;
        जर (N % 32 != 0)
            परत असत्य;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    अन्यथा जर(!atrans && btrans)
    {
        जर (K % 4 != 0)
            परत असत्य;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    अन्यथा
    {
        जर (M % 32 != 0)
            परत असत्य;
        जर (N % 32 != 0)
            परत असत्य;
        जर (K % 16 != 0)
            परत असत्य;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(रिक्त, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    स्थिरांक माप_प्रकार gx = divUp((माप_प्रकार)N, dx);
    स्थिरांक माप_प्रकार gy = divUp((माप_प्रकार)M, dy);

    माप_प्रकार local[] = {lx, ly, 1};
    माप_प्रकार global[] = {roundUp(gx, lx), roundUp(gy, ly), 1};

    ocl::Queue q;
    String errmsg;
    स्थिरांक ocl::Program program = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    जर(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), program);
        जर (k.empty())
        {
            परत असत्य;
        }

        k.args(ocl::KernelArg::PtrReadOnly(A),
               (पूर्णांक) (A.offset / आकार(दशांश)),
               ocl::KernelArg::PtrReadOnly(B),
               (पूर्णांक) (B.offset / आकार(दशांश)),
               ocl::KernelArg::PtrWriteOnly(D),
               (पूर्णांक) (D.offset / आकार(दशांश)),
               M, N, K,
               (दशांश)alpha,
               (दशांश)beta,
               (पूर्णांक)(A.step / आकार(दशांश)),
               (पूर्णांक)(B.step / आकार(दशांश)),
               (पूर्णांक)(D.step / आकार(दशांश))
        );

        तार्किक ret = k.run(2, global, local, असत्य, q);
        परत ret;
    }
    अन्यथा
    {
        पूर्णांक stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        साठी(पूर्णांक start_index = 0; start_index < K; start_index += stride)
        {
             ocl::Kernel k(kernelName.c_str(), program);
             k.args(ocl::KernelArg::PtrReadOnly(A),
                    (पूर्णांक) (A.offset / आकार(दशांश)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (पूर्णांक) (B.offset / आकार(दशांश)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (पूर्णांक) (D.offset / आकार(दशांश)),
                    M, N, K,
                    (दशांश)alpha,
                    (दशांश)beta,
                    (पूर्णांक)(A.step / आकार(दशांश)),
                    (पूर्णांक)(B.step / आकार(दशांश)),
                    (पूर्णांक)(D.step / आकार(दशांश)),
                    (पूर्णांक) start_index,                          // 14 start_index
                    stride);

            तार्किक ret = k.run(2, global, local, असत्य, q);
            जर (!ret)
            {
                जर (start_index != 0)
                    isPropagatedC2D = असत्य;  // D array content is changed, need to rewrite
                परत असत्य;
            }
        }
        परत सत्य;
    }
}

} // namespace cv

#endif
