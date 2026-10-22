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

سانچو<قسم_نالو Ti, قسم_نالو T> ساكن فراغ
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

سانچو<قسم_نالو T, فراغ (*fn)(ثابت uchar*, ثابت T*, T*, صحيح, صحيح, صحيح)>
ساكن فراغ lutDispatchWrap( ثابت فراغ* src, ثابت فراغ* lut, فراغ* dst, صحيح len, صحيح cn, صحيح lutcn )
{
    fn((ثابت uchar*)src, (ثابت T*)lut, (T*)dst, len, cn, lutcn);
}

تعريفنوع فراغ (*LUTFunc)( ثابت فراغ* src, ثابت فراغ* lut, فراغ* dst, صحيح len, صحيح cn, صحيح lutcn );

ساكن LUTFunc getLUTFunc(ثابت صحيح src_اونهائي, ثابت صحيح dst_اونهائي)
{
    LUTFunc ret = خالي_اشارو;
    إذا((src_اونهائي == CV_8U) || (src_اونهائي == CV_8S))
    {
        بدل(dst_اونهائي)
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
            افتراضي:      ret = خالي_اشارو;                  اكسر;
        }
    }
    وإلا إذا((src_اونهائي == CV_16U) || (src_اونهائي == CV_16S))
    {
        بدل(dst_اونهائي)
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
            افتراضي:      ret = خالي_اشارو;                  اكسر;
        }
    }

    CV_CheckTrue(ret != خالي_اشارو, "An unexpected type combination was specified.");
    أرجع ret;
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

ساكن منطقي ocl_LUT(داخلا_Array _src, داخلا_Array _lut, OutputArray _dst)
{
    صحيح lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.اونهائي();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_سائيز(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    صحيح kercn = lcn == 1 ? std::min(4, ocl::predict_Optimal_ويڪٽر_ويڪر(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::type_منزل_Str(src.اونهائي()), ocl::memop_Type_منزل_Str(ddepth)));
    إذا (k.empty())
        أرجع خطأ;

    k.دليلون(ocl::KernelArg::Read_Only_No_سائيز(src), ocl::KernelArg::Read_Only_No_سائيز(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t global_سائيز[2] = { (size_t)dst.کانن_ڳڻپ * dcn / kercn, ((size_t)dst.صفون + 3) / 4 };
    أرجع k.لاڳيتو(2, global_سائيز, عدم, خطأ);
}

#جيڪڏهن_پورو

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
        ثابت صحيح row0 = range.شروعات;
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

فراغ cv::LUT( داخلا_Array _src, داخلا_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحيح cn = _src.channels(), اونهائي = _src.اونهائي();
    صحيح lutcn = _lut.channels();
    ثابت size_t lut_سائيز = _lut.ڪل();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_سائيز == 256) && ((اونهائي == CV_8U)||(اونهائي == CV_8S))) ||
            ((lut_سائيز == 65536) && ((اونهائي == CV_16U)||(اونهائي == CV_16S)))
        )
    );

    CV_OCL_لاڳيتو(_dst.isUMat() && _src.dims() <= 2 && (lut_سائيز == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_سائيز(_src, CV_MAKETYPE(_lut.اونهائي(), cn));
    Mat dst = _dst.getMat();

    إذا(lut_سائيز == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.معلومات, src.قدم, src.type(), lut.معلومات,
                 lut.elemSize1(), lutcn, dst.معلومات, dst.قدم, src.کانن_ڳڻپ, src.صفون);
    }
    وإلا
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.قدم, src.type(), lut.معلومات,
                 lut.elemSize1(), lutcn, dst.معلومات, dst.قدم, src.کانن_ڳڻپ, src.صفون);
    }

    ثابت LUTFunc func = getLUTFunc(src.اونهائي(), dst.اونهائي());
    CV_Assert( func != خالي_اشارو );

    إذا (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.صفون);
        إذا (dst.ڪل() >= (size_t)(1<<18))
            parallel_for_(all, body, (مزدوج)std::max((size_t)1, dst.ڪل()>>16));
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
