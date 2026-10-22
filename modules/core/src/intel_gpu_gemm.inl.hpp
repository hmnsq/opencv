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

#اگر_تعریف_شدہ HAVE_OPENCL

نامخلا cv
{

ساکن منطقی intel_gpu_gemm(
    UMat A, سائز سائز_A,
    UMat B, سائز سائز_B,
    UMat D, سائز سائز_D,
    دوہرا alpha, دوہرا beta,
    منطقی atrans, منطقی btrans,
    منطقی& isPropagatedC2D
)
{
    CV_غیر_مستعمل(سائز_B);

    صحیح M = سائز_D.اونچائی, N = سائز_D.چوڑائی, K = ((atrans)? سائز_A.اونچائی : سائز_A.چوڑائی);

    اگر (M < 4 || N < 4 || K < 4)  // vload4
        واپس جھوٹ;

    CV_LOG_VERBOSE(تہی, 0, "M=" << M << " N=" << N << " K=" << K);

    std::string kernelName;

    غیرعلامتی صحیح lx = 8, ly = 4;
    غیرعلامتی صحیح dx = 4, dy = 8;

    اگر(!atrans && !btrans)
    {
        اگر (M % 32 == 0 && N % 32 == 0 && K % 16 == 0)
        {
            kernelName = "intelblas_gemm_buffer_NN_sp";
        }
        ورنہ
        {
            اگر (M % 2 != 0)
                واپس جھوٹ;
            // vload4(0, dst_write0) - 4 cols
            // multiply by lx: 8
            اگر (N % (4*8) != 0)
                واپس جھوٹ;
            kernelName = "intelblas_gemm_buffer_NN";
        }
    }
    ورنہ اگر(atrans && !btrans)
    {
        اگر (M % 32 != 0)
            واپس جھوٹ;
        اگر (N % 32 != 0)
            واپس جھوٹ;
        kernelName = "intelblas_gemm_buffer_TN";
    }
    ورنہ اگر(!atrans && btrans)
    {
        اگر (K % 4 != 0)
            واپس جھوٹ;
        kernelName = "intelblas_gemm_buffer_NT";
        ly = 16;
        dx = 1;
    }
    ورنہ
    {
        اگر (M % 32 != 0)
            واپس جھوٹ;
        اگر (N % 32 != 0)
            واپس جھوٹ;
        اگر (K % 16 != 0)
            واپس جھوٹ;
        kernelName = "intelblas_gemm_buffer_TT";
    }

    CV_LOG_DEBUG(تہی, "kernel: " << kernelName << "  (M=" << M << " N=" << N << " K=" << K << ")");

    ثابت size_t gx = div_اوپر((size_t)N, dx);
    ثابت size_t gy = div_اوپر((size_t)M, dy);

    size_t local[] = {lx, ly, 1};
    size_t global[] = {باری_اوپر(gx, lx), باری_اوپر(gy, ly), 1};

    ocl::قطار q;
    String errmsg;
    ثابت ocl::پروگرام_2 پروگرام = ocl::Context::getDefault().getProg(ocl::core::intel_gemm_oclsrc, "", errmsg);

    اگر(!atrans && btrans)
    {
        ocl::Kernel k(kernelName.c_str(), پروگرام);
        اگر (k.empty())
        {
            واپس جھوٹ;
        }

        k.دلائل(ocl::KernelArg::PtrReadOnly(A),
               (صحیح) (A.offset / حجم(اعشاری)),
               ocl::KernelArg::PtrReadOnly(B),
               (صحیح) (B.offset / حجم(اعشاری)),
               ocl::KernelArg::PtrWriteOnly(D),
               (صحیح) (D.offset / حجم(اعشاری)),
               M, N, K,
               (اعشاری)alpha,
               (اعشاری)beta,
               (صحیح)(A.قدم / حجم(اعشاری)),
               (صحیح)(B.قدم / حجم(اعشاری)),
               (صحیح)(D.قدم / حجم(اعشاری))
        );

        منطقی ret = k.تسلسل(2, global, local, جھوٹ, q);
        واپس ret;
    }
    ورنہ
    {
        صحیح stride = (M * N < 1024 * 1024) ? 10000000 : 256;

        برائے(صحیح آغاز_اشاریہ = 0; آغاز_اشاریہ < K; آغاز_اشاریہ += stride)
        {
             ocl::Kernel k(kernelName.c_str(), پروگرام);
             k.دلائل(ocl::KernelArg::PtrReadOnly(A),
                    (صحیح) (A.offset / حجم(اعشاری)),
                    ocl::KernelArg::PtrReadOnly(B),
                    (صحیح) (B.offset / حجم(اعشاری)),
                    ocl::KernelArg::PtrWriteOnly(D),
                    (صحیح) (D.offset / حجم(اعشاری)),
                    M, N, K,
                    (اعشاری)alpha,
                    (اعشاری)beta,
                    (صحیح)(A.قدم / حجم(اعشاری)),
                    (صحیح)(B.قدم / حجم(اعشاری)),
                    (صحیح)(D.قدم / حجم(اعشاری)),
                    (صحیح) آغاز_اشاریہ,                          // 14 start_index
                    stride);

            منطقی ret = k.تسلسل(2, global, local, جھوٹ, q);
            اگر (!ret)
            {
                اگر (آغاز_اشاریہ != 0)
                    isPropagatedC2D = جھوٹ;  // D array content is changed, need to rewrite
                واپس جھوٹ;
            }
        }
        واپس سچ;
    }
}

} // namespace cv

#اگر_ختم
