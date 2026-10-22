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

#నిర్వచితమైతే HAVE_OPENCL

నామాకాశ cv
{

స్థిర తార్కిక intel_gpu_gemm(
    UMat A, కొలత కొలత_A,
    UMat B, కొలత కొలత_B,
    UMat D, కొలత కొలత_D,
    ద్విగుణ alpha, ద్విగుణ beta,
    తార్కిక atrans, తార్కిక btrans,
    తార్కిక& isPropagatedC2D
)
{
    CV_వాడని(కొలత_B);

    పూర్ణాంకం M = కొలత_D.ఎత్తు, N = కొలత_D.వెడల్పు, K = ((atrans)? కొలత_A.ఎత్తు : కొలత_A.వెడల్పు);

    ఒకవేళ (M < 4 || N < 4 || K < 4)  // vload4
        తిరిగి అబద్ధం;

    CV_LOG_VERBOSE(ఖాళీ, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    గుర్తులేని పూర్ణాంకం lx = 8, ly = 4;
    గుర్తులేని పూర్ణాంకం dx = 4, dy = 8;

    ఒకవేళ(!atrans && !btrans)
    {
        ఒకవేళ (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        లేకపోతే
        {
            ఒకవేళ (M % 2 != 0)
                తిరిగి అబద్ధం;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            ఒకవేళ (N % (4*8) != 0)
                తిరిగి అబద్ధం;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    లేకపోతే ఒకవేళ(atrans && !btrans)
    {
        ఒకవేళ (M % 32 != 0)
            తిరిగి అబద్ధం;
        ఒకవేళ (N % 32 != 0)
            తిరిగి అబద్ధం;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    లేకపోతే ఒకవేళ(!atrans && btrans)
    {
        ఒకవేళ (K % 4 != 0)
            తిరిగి అబద్ధం;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    లేకపోతే
    {
        ఒకవేళ (M % 32 != 0)
            తిరిగి అబద్ధం;
        ఒకవేళ (N % 32 != 0)
            తిరిగి అబద్ధం;
        ఒకవేళ (K % 16 != 0)
            తిరిగి అబద్ధం;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(ఖాళీ, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    స్థిరరాశి size_t gx = div_పైకి((size_t)N, dx);
    స్థిరరాశి size_t gy = div_పైకి((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {విడత_పైకి(gx, lx), విడత_పైకి(gy, ly), 1};

    ocl::వరుస q;
    String errmsg;
    స్థిరరాశి ocl::కార్యక్రమం_2 కార్యక్రమం = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    ఒకవేళ(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), కార్యక్రమం);
        ఒకవేళ (k.empty())
        {
            తిరిగి అబద్ధం;
        }

        k.వాదనలు(ocl::KernelArg::PtrReadOnly(A),
               (పూర్ణాంకం) (A.offset / పరిమాణం(దశాంశం)),
               ocl::KernelArg::PtrReadOnly(B),
               (పూర్ణాంకం) (B.offset / పరిమాణం(దశాంశం)),
               ocl::KernelArg::PtrWriteOnly(D),
               (పూర్ణాంకం) (D.offset / పరిమాణం(దశాంశం)),
               M, N, K,
               (దశాంశం)alpha,
               (దశాంశం)beta,
               (పూర్ణాంకం)(A.అడుగు / పరిమాణం(దశాంశం)),
               (పూర్ణాంకం)(B.అడుగు / పరిమాణం(దశాంశం)),
               (పూర్ణాంకం)(D.అడుగు / పరిమాణం(దశాంశం))
        );

        తార్కిక ret = k.నిరంతరం(2, global, local, అబద్ధం, q);
        తిరిగి ret;
    }
    లేకపోతే
    {
        పూర్ణాంకం stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        కొరకు(పూర్ణాంకం ప్రారంభం_సూచీ = 0; ప్రారంభం_సూచీ < K; ప్రారంభం_సూచీ += stride)
        {
             ocl::Kernel k(kernelName.c_str(), కార్యక్రమం);
             k.వాదనలు(ocl::KernelArg::PtrReadOnly(A),
                    (పూర్ణాంకం) (A.offset / పరిమాణం(దశాంశం)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (పూర్ణాంకం) (B.offset / పరిమాణం(దశాంశం)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (పూర్ణాంకం) (D.offset / పరిమాణం(దశాంశం)),
                    M, N, K,
                    (దశాంశం)alpha,
                    (దశాంశం)beta,
                    (పూర్ణాంకం)(A.అడుగు / పరిమాణం(దశాంశం)),
                    (పూర్ణాంకం)(B.అడుగు / పరిమాణం(దశాంశం)),
                    (పూర్ణాంకం)(D.అడుగు / పరిమాణం(దశాంశం)),
                    (పూర్ణాంకం) ప్రారంభం_సూచీ,                          // 14 start_index
                    stride);

            తార్కిక ret = k.నిరంతరం(2, global, local, అబద్ధం, q);
            ఒకవేళ (!ret)
            {
                ఒకవేళ (ప్రారంభం_సూచీ != 0)
                    isPropagatedC2D = అబద్ధం;  // D array content is changed, need to rewrite
                తిరిగి అబద్ధం;
            }
        }
        తిరిగి నిజం;
    }
}

} // namespace cv

#అయితే_ముగింపు
