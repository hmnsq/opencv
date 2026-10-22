// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "convert.hpp"
#شامل <sys/types.h>

نامخلا cv {
خالی LUT8u_dispatch( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحیح len, صحیح cn, صحیح lutcn );
خالی LUT16u_dispatch( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحیح len, صحیح cn, صحیح lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

نامخلا cv
{

سانچہ<قسم_نام Ti, قسم_نام T> ساکن خالی
LUT_( ثابت خالی* _src, ثابت خالی* _lut, خالی* _dst, صحیح len, صحیح cn, صحیح lutcn )
{
    ثابت Ti* src = (ثابت Ti*)_src;
    ثابت T* lut = (ثابت T*)_lut;
    T* dst = (T*)_dst;
    اگر( lutcn == 1 )
    {
        برائے( صحیح i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    ورنہ
    {
        برائے( صحیح i = 0; i < len*cn; i += cn )
            برائے( صحیح k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

سانچہ<قسم_نام T, خالی (*fn)(ثابت uchar*, ثابت T*, T*, صحیح, صحیح, صحیح)>
ساکن خالی lutDispatchWrap( ثابت خالی* src, ثابت خالی* lut, خالی* dst, صحیح len, صحیح cn, صحیح lutcn )
{
    fn((ثابت uchar*)src, (ثابت T*)lut, (T*)dst, len, cn, lutcn);
}

قسمتعریف خالی (*LUTFunc)( ثابت خالی* src, ثابت خالی* lut, خالی* dst, صحیح len, صحیح cn, صحیح lutcn );

ساکن LUTFunc getLUTFunc(ثابت صحیح src_گہرائی, ثابت صحیح dst_گہرائی)
{
    LUTFunc ret = خالی_اشاریہ;
    اگر((src_گہرائی == CV_8U) || (src_گہرائی == CV_8S))
    {
        انتخاب(dst_گہرائی)
        {
            حالت CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   توڑو;
            حالت CV_8S:   ret = LUT_<uint8_t, int8_t>;    توڑو;
            حالت CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; توڑو;
            حالت CV_16S:  ret = LUT_<uint8_t, int16_t>;   توڑو;
            حالت CV_32S:  ret = LUT_<uint8_t, int32_t>;   توڑو;
            حالت CV_32F:  ret = LUT_<uint8_t, int32_t>;   توڑو; // float
            حالت CV_64F:  ret = LUT_<uint8_t, int64_t>;   توڑو; // double
            حالت CV_16F:  ret = LUT_<uint8_t, int16_t>;   توڑو; // hfloat
            حالت CV_16BF: ret = LUT_<uint8_t, int16_t>;   توڑو; // bfloat
            حالت CV_Bool: ret = LUT_<uint8_t, uint8_t>;   توڑو; // bool
            حالت CV_64U:  ret = LUT_<uint8_t, uint64_t>;  توڑو;
            حالت CV_64S:  ret = LUT_<uint8_t, int64_t>;   توڑو;
            حالت CV_32U:  ret = LUT_<uint8_t, uint32_t>;  توڑو;
            طےشدہ:      ret = خالی_اشاریہ;                  توڑو;
        }
    }
    ورنہ اگر((src_گہرائی == CV_16U) || (src_گہرائی == CV_16S))
    {
        انتخاب(dst_گہرائی)
        {
            حالت CV_8U:   ret = LUT_<uint16_t, uint8_t>;  توڑو;
            حالت CV_8S:   ret = LUT_<uint16_t, int8_t>;   توڑو;
            حالت CV_16U:  ret = LUT_<uint16_t, uint16_t>; توڑو;
            حالت CV_16S:  ret = LUT_<uint16_t, int16_t>;  توڑو;
            حالت CV_32S:  ret = LUT_<uint16_t, int32_t>;  توڑو;
            حالت CV_32F:  ret = LUT_<uint16_t, int32_t>;  توڑو; // float
            حالت CV_64F:  ret = LUT_<uint16_t, int64_t>;  توڑو; // double
            حالت CV_16F:  ret = LUT_<uint16_t, int16_t>;  توڑو; // hfloat
            حالت CV_16BF: ret = LUT_<uint16_t, int16_t>;  توڑو; // bfloat
            حالت CV_Bool: ret = LUT_<uint16_t, uint8_t>;  توڑو; // bool
            حالت CV_64U:  ret = LUT_<uint16_t, uint64_t>; توڑو;
            حالت CV_64S:  ret = LUT_<uint16_t, int64_t>;  توڑو;
            حالت CV_32U:  ret = LUT_<uint16_t, uint32_t>; توڑو;
            طےشدہ:      ret = خالی_اشاریہ;                  توڑو;
        }
    }

    CV_CheckTrue(ret != خالی_اشاریہ, "An unexpected type combination was specified.");
    واپس ret;
}

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_LUT(داخلہ_Array _src, داخلہ_Array _lut, OutputArray _dst)
{
    صحیح lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.گہرائی();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_سائز(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    صحیح kercn = lcn == 1 ? std::min(4, ocl::predict_Optimal_سمتیہ_چوڑائی(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::type_مقصد_Str(src.گہرائی()), ocl::memop_Type_مقصد_Str(ddepth)));
    اگر (k.empty())
        واپس جھوٹ;

    k.دلائل(ocl::KernelArg::Read_Only_No_سائز(src), ocl::KernelArg::Read_Only_No_سائز(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t global_سائز[2] = { (size_t)dst.ستون_تعداد * dcn / kercn, ((size_t)dst.صفیں + 3) / 4 };
    واپس k.تسلسل(2, global_سائز, تہی, جھوٹ);
}

#اگر_ختم

جماعت LUTParallelBody : عوامی ParallelLoopBody
{
عوامی:
    ثابت Mat& src_;
    ثابت Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ثابت Mat& src, ثابت Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    خالی عامل()( ثابت cv::Range& range ) ثابت CV_OVERRIDE
    {
        ثابت صحیح row0 = range.آغاز;
        ثابت صحیح row1 = range.end;

        Mat src = src_.صف_Range_2(row0, row1);
        Mat dst = dst_.صف_Range_2(row0, row1);

        صحیح cn = src.channels();
        صحیح lutcn = lut_.channels();

        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحیح len = (صحیح)it.size;

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
نجی:
    LUTParallelBody(ثابت LUTParallelBody&);
    LUTParallelBody& عامل=(ثابت LUTParallelBody&);
};

} // cv::

خالی cv::LUT( داخلہ_Array _src, داخلہ_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحیح cn = _src.channels(), گہرائی = _src.گہرائی();
    صحیح lutcn = _lut.channels();
    ثابت size_t lut_سائز = _lut.کل();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_سائز == 256) && ((گہرائی == CV_8U)||(گہرائی == CV_8S))) ||
            ((lut_سائز == 65536) && ((گہرائی == CV_16U)||(گہرائی == CV_16S)))
        )
    );

    CV_OCL_تسلسل(_dst.isUMat() && _src.dims() <= 2 && (lut_سائز == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_سائز(_src, CV_MAKETYPE(_lut.گہرائی(), cn));
    Mat dst = _dst.getMat();

    اگر(lut_سائز == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.ڈیٹا, src.قدم, src.type(), lut.ڈیٹا,
                 lut.elemSize1(), lutcn, dst.ڈیٹا, dst.قدم, src.ستون_تعداد, src.صفیں);
    }
    ورنہ
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.قدم, src.type(), lut.ڈیٹا,
                 lut.elemSize1(), lutcn, dst.ڈیٹا, dst.قدم, src.ستون_تعداد, src.صفیں);
    }

    ثابت LUTFunc func = getLUTFunc(src.گہرائی(), dst.گہرائی());
    CV_Assert( func != خالی_اشاریہ );

    اگر (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.صفیں);
        اگر (dst.کل() >= (size_t)(1<<18))
            parallel_for_(all, body, (دوہرا)std::max((size_t)1, dst.کل()>>16));
        ورنہ
            body(all);

        واپس;
    }

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح len = (صحیح)it.size;

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
