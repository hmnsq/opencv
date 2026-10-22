// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "convert.hpp"
#ضمن <sys/types.h>

نطاق cv {
فراغ LUT8u_dispatch( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحيح len, صحيح cn, صحيح lutcn );
فراغ LUT16u_dispatch( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحيح len, صحيح cn, صحيح lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

نطاق cv
{

سانچا<قسم_ناں Ti, قسم_ناں T> ساكن فراغ
LUT_( ثابت فراغ* _src, ثابت فراغ* _lut, فراغ* _dst, صحيح len, صحيح cn, صحيح lutcn )
{
    ثابت Ti* src = (ثابت Ti*)_src;
    ثابت T* lut = (ثابت T*)_lut;
    T* dst = (T*)_dst;
    إذا( lutcn == 1 )
    {
        لكل( صحيح i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len*cn; i += cn )
            لكل( صحيح k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

سانچا<قسم_ناں T, فراغ (*fn)(ثابت uchar*, ثابت T*, T*, صحيح, صحيح, صحيح)>
ساكن فراغ lutDispatchWrap( ثابت فراغ* src, ثابت فراغ* lut, فراغ* dst, صحيح len, صحيح cn, صحيح lutcn )
{
    fn((ثابت uchar*)src, (ثابت T*)lut, (T*)dst, len, cn, lutcn);
}

تعريفنوع فراغ (*LUTFunc)( ثابت فراغ* src, ثابت فراغ* lut, فراغ* dst, صحيح len, صحيح cn, صحيح lutcn );

ساكن LUTFunc getLUTFunc(ثابت صحيح src_ڈونگھائی, ثابت صحيح dst_ڈونگھائی)
{
    LUTFunc ret = خالی_سوچک;
    إذا((src_ڈونگھائی == CV_8U) || (src_ڈونگھائی == CV_8S))
    {
        بدل(dst_ڈونگھائی)
        {
            حالة CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   اكسر;
            حالة CV_8S:   ret = LUT_<uint8_t, int8_t>;    اكسر;
            حالة CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; اكسر;
            حالة CV_16S:  ret = LUT_<uint8_t, int16_t>;   اكسر;
            حالة CV_32S:  ret = LUT_<uint8_t, int32_t>;   اكسر;
            حالة CV_32F:  ret = LUT_<uint8_t, int32_t>;   اكسر; // float
            حالة CV_64F:  ret = LUT_<uint8_t, int64_t>;   اكسر; // double
            حالة CV_16F:  ret = LUT_<uint8_t, int16_t>;   اكسر; // hfloat
            حالة CV_16BF: ret = LUT_<uint8_t, int16_t>;   اكسر; // bfloat
            حالة CV_Bool: ret = LUT_<uint8_t, uint8_t>;   اكسر; // bool
            حالة CV_64U:  ret = LUT_<uint8_t, uint64_t>;  اكسر;
            حالة CV_64S:  ret = LUT_<uint8_t, int64_t>;   اكسر;
            حالة CV_32U:  ret = LUT_<uint8_t, uint32_t>;  اكسر;
            افتراضي:      ret = خالی_سوچک;                  اكسر;
        }
    }
    وإلا إذا((src_ڈونگھائی == CV_16U) || (src_ڈونگھائی == CV_16S))
    {
        بدل(dst_ڈونگھائی)
        {
            حالة CV_8U:   ret = LUT_<uint16_t, uint8_t>;  اكسر;
            حالة CV_8S:   ret = LUT_<uint16_t, int8_t>;   اكسر;
            حالة CV_16U:  ret = LUT_<uint16_t, uint16_t>; اكسر;
            حالة CV_16S:  ret = LUT_<uint16_t, int16_t>;  اكسر;
            حالة CV_32S:  ret = LUT_<uint16_t, int32_t>;  اكسر;
            حالة CV_32F:  ret = LUT_<uint16_t, int32_t>;  اكسر; // float
            حالة CV_64F:  ret = LUT_<uint16_t, int64_t>;  اكسر; // double
            حالة CV_16F:  ret = LUT_<uint16_t, int16_t>;  اكسر; // hfloat
            حالة CV_16BF: ret = LUT_<uint16_t, int16_t>;  اكسر; // bfloat
            حالة CV_Bool: ret = LUT_<uint16_t, uint8_t>;  اكسر; // bool
            حالة CV_64U:  ret = LUT_<uint16_t, uint64_t>; اكسر;
            حالة CV_64S:  ret = LUT_<uint16_t, int64_t>;  اكسر;
            حالة CV_32U:  ret = LUT_<uint16_t, uint32_t>; اكسر;
            افتراضي:      ret = خالی_سوچک;                  اكسر;
        }
    }

    CV_CheckTrue(ret != خالی_سوچک, "An unexpected type combination was specified.");
    أرجع ret;
}

#جے_تعریف_شدہ HAVE_OPENCL

ساكن منطقي ocl_LUT(داخلہ_Array _src, داخلہ_Array _lut, OutputArray _dst)
{
    صحيح lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.ڈونگھائی();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_ناپ(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    صحيح kercn = lcn == 1 ? std::min(4, ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::type_منزل_Str(src.ڈونگھائی()), ocl::memop_Type_منزل_Str(ddepth)));
    إذا (k.empty())
        أرجع خطأ;

    k.دلیلاں(ocl::KernelArg::Read_Only_No_ناپ(src), ocl::KernelArg::Read_Only_No_ناپ(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t global_ناپ[2] = { (size_t)dst.خانے_گنتی * dcn / kercn, ((size_t)dst.صفاں + 3) / 4 };
    أرجع k.لگاتار(2, global_ناپ, عدم, خطأ);
}

#جے_انت

صنف LUTParallelBody : عام ParallelLoopBody
{
عام:
    ثابت Mat& src_;
    ثابت Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ثابت Mat& src, ثابت Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    فراغ عامل()( ثابت cv::Range& range ) ثابت CV_OVERRIDE
    {
        ثابت صحيح row0 = range.شروع;
        ثابت صحيح row1 = range.end;

        Mat src = src_.صف_Range_2(row0, row1);
        Mat dst = dst_.صف_Range_2(row0, row1);

        صحيح cn = src.channels();
        صحيح lutcn = lut_.channels();

        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح len = (صحيح)it.size;

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
خاص:
    LUTParallelBody(ثابت LUTParallelBody&);
    LUTParallelBody& عامل=(ثابت LUTParallelBody&);
};

} // cv::

فراغ cv::LUT( داخلہ_Array _src, داخلہ_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحيح cn = _src.channels(), ڈونگھائی = _src.ڈونگھائی();
    صحيح lutcn = _lut.channels();
    ثابت size_t lut_ناپ = _lut.کل();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_ناپ == 256) && ((ڈونگھائی == CV_8U)||(ڈونگھائی == CV_8S))) ||
            ((lut_ناپ == 65536) && ((ڈونگھائی == CV_16U)||(ڈونگھائی == CV_16S)))
        )
    );

    CV_OCL_لگاتار(_dst.isUMat() && _src.dims() <= 2 && (lut_ناپ == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_ناپ(_src, CV_MAKETYPE(_lut.ڈونگھائی(), cn));
    Mat dst = _dst.getMat();

    إذا(lut_ناپ == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.انکڑے, src.قدم, src.type(), lut.انکڑے,
                 lut.elemSize1(), lutcn, dst.انکڑے, dst.قدم, src.خانے_گنتی, src.صفاں);
    }
    وإلا
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.قدم, src.type(), lut.انکڑے,
                 lut.elemSize1(), lutcn, dst.انکڑے, dst.قدم, src.خانے_گنتی, src.صفاں);
    }

    ثابت LUTFunc func = getLUTFunc(src.ڈونگھائی(), dst.ڈونگھائی());
    CV_Assert( func != خالی_سوچک );

    إذا (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.صفاں);
        إذا (dst.کل() >= (size_t)(1<<18))
            parallel_for_(all, body, (مزدوج)std::max((size_t)1, dst.کل()>>16));
        وإلا
            body(all);

        أرجع;
    }

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح len = (صحيح)it.size;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
