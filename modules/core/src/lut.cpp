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

کينډۍ<ډول_نوم Ti, ډول_نوم T> ساكن فراغ
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

کينډۍ<ډول_نوم T, فراغ (*fn)(ثابت uchar*, ثابت T*, T*, صحيح, صحيح, صحيح)>
ساكن فراغ lutDispatchWrap( ثابت فراغ* src, ثابت فراغ* lut, فراغ* dst, صحيح len, صحيح cn, صحيح lutcn )
{
    fn((ثابت uchar*)src, (ثابت T*)lut, (T*)dst, len, cn, lutcn);
}

تعريفنوع فراغ (*LUTFunc)( ثابت فراغ* src, ثابت فراغ* lut, فراغ* dst, صحيح len, صحيح cn, صحيح lutcn );

ساكن LUTFunc getLUTFunc(ثابت صحيح src_ژوروالی, ثابت صحيح dst_ژوروالی)
{
    LUTFunc ret = تش_اشاره;
    إذا((src_ژوروالی == CV_8U) || (src_ژوروالی == CV_8S))
    {
        بدل(dst_ژوروالی)
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
            افتراضي:      ret = تش_اشاره;                  اكسر;
        }
    }
    وإلا إذا((src_ژوروالی == CV_16U) || (src_ژوروالی == CV_16S))
    {
        بدل(dst_ژوروالی)
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
            افتراضي:      ret = تش_اشاره;                  اكسر;
        }
    }

    CV_CheckTrue(ret != تش_اشاره, "An unexpected type combination was specified.");
    أرجع ret;
}

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_LUT(ننوت_Array _src, ننوت_Array _lut, OutputArray _dst)
{
    صحيح lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.ژوروالی();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_اندازه(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    صحيح kercn = lcn == 1 ? std::min(4, ocl::predict_Optimal_ویکټور_پلنوالی(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::type_منزل_Str(src.ژوروالی()), ocl::memop_Type_منزل_Str(ddepth)));
    إذا (k.empty())
        أرجع خطأ;

    k.دلیلونه(ocl::KernelArg::Read_Only_No_اندازه(src), ocl::KernelArg::Read_Only_No_اندازه(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t global_اندازه[2] = { (size_t)dst.ستون_شمېر * dcn / kercn, ((size_t)dst.لیکې + 3) / 4 };
    أرجع k.پرلپسې(2, global_اندازه, عدم, خطأ);
}

#د_که_پای

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

    فراغ چلوونکی()( ثابت cv::Range& range ) ثابت CV_OVERRIDE
    {
        ثابت صحيح row0 = range.پیل;
        ثابت صحيح row1 = range.end;

        Mat src = src_.لیکه_Range_2(row0, row1);
        Mat dst = dst_.لیکه_Range_2(row0, row1);

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
    LUTParallelBody& چلوونکی=(ثابت LUTParallelBody&);
};

} // cv::

فراغ cv::LUT( ننوت_Array _src, ننوت_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحيح cn = _src.channels(), ژوروالی = _src.ژوروالی();
    صحيح lutcn = _lut.channels();
    ثابت size_t lut_اندازه = _lut.ټول();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_اندازه == 256) && ((ژوروالی == CV_8U)||(ژوروالی == CV_8S))) ||
            ((lut_اندازه == 65536) && ((ژوروالی == CV_16U)||(ژوروالی == CV_16S)))
        )
    );

    CV_OCL_پرلپسې(_dst.isUMat() && _src.dims() <= 2 && (lut_اندازه == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_اندازه(_src, CV_MAKETYPE(_lut.ژوروالی(), cn));
    Mat dst = _dst.getMat();

    إذا(lut_اندازه == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.معلومات, src.ګام, src.type(), lut.معلومات,
                 lut.elemSize1(), lutcn, dst.معلومات, dst.ګام, src.ستون_شمېر, src.لیکې);
    }
    وإلا
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.ګام, src.type(), lut.معلومات,
                 lut.elemSize1(), lutcn, dst.معلومات, dst.ګام, src.ستون_شمېر, src.لیکې);
    }

    ثابت LUTFunc func = getLUTFunc(src.ژوروالی(), dst.ژوروالی());
    CV_Assert( func != تش_اشاره );

    إذا (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.لیکې);
        إذا (dst.ټول() >= (size_t)(1<<18))
            parallel_for_(all, body, (مزدوج)std::max((size_t)1, dst.ټول()>>16));
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
