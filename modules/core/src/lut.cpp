// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

ਨਾਮਾਕਾਸ਼ cv {
ਖਾਲੀ LUT8u_dispatch( ਅਚਲ uchar* src, ਅਚਲ uchar* lut, uchar* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn );
ਖਾਲੀ LUT16u_dispatch( ਅਚਲ uchar* src, ਅਚਲ ushort* lut, ushort* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv
{

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Ti, ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
LUT_( ਅਚਲ ਖਾਲੀ* _src, ਅਚਲ ਖਾਲੀ* _lut, ਖਾਲੀ* _dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn )
{
    ਅਚਲ Ti* src = (ਅਚਲ Ti*)_src;
    ਅਚਲ T* lut = (ਅਚਲ T*)_lut;
    T* dst = (T*)_dst;
    ਜੇ( lutcn == 1 )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( ਪੂਰਨ i = 0; i < len*cn; i += cn )
            ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਖਾਲੀ (*fn)(ਅਚਲ uchar*, ਅਚਲ T*, T*, ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ)>
ਸਥਿਰ ਖਾਲੀ lutDispatchWrap( ਅਚਲ ਖਾਲੀ* src, ਅਚਲ ਖਾਲੀ* lut, ਖਾਲੀ* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn )
{
    fn((ਅਚਲ uchar*)src, (ਅਚਲ T*)lut, (T*)dst, len, cn, lutcn);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*LUTFunc)( ਅਚਲ ਖਾਲੀ* src, ਅਚਲ ਖਾਲੀ* lut, ਖਾਲੀ* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn );

ਸਥਿਰ LUTFunc getLUTFunc(ਅਚਲ ਪੂਰਨ src_ਡੂੰਘਾਈ, ਅਚਲ ਪੂਰਨ dst_ਡੂੰਘਾਈ)
{
    LUTFunc ret = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ((src_ਡੂੰਘਾਈ == CV_8U) || (src_ਡੂੰਘਾਈ == CV_8S))
    {
        ਚੋਣ(dst_ਡੂੰਘਾਈ)
        {
            ਹਾਲਤ CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   ਰੋਕੋ;
            ਹਾਲਤ CV_8S:   ret = LUT_<uint8_t, int8_t>;    ਰੋਕੋ;
            ਹਾਲਤ CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; ਰੋਕੋ;
            ਹਾਲਤ CV_16S:  ret = LUT_<uint8_t, int16_t>;   ਰੋਕੋ;
            ਹਾਲਤ CV_32S:  ret = LUT_<uint8_t, int32_t>;   ਰੋਕੋ;
            ਹਾਲਤ CV_32F:  ret = LUT_<uint8_t, int32_t>;   ਰੋਕੋ; // float
            ਹਾਲਤ CV_64F:  ret = LUT_<uint8_t, int64_t>;   ਰੋਕੋ; // double
            ਹਾਲਤ CV_16F:  ret = LUT_<uint8_t, int16_t>;   ਰੋਕੋ; // hfloat
            ਹਾਲਤ CV_16BF: ret = LUT_<uint8_t, int16_t>;   ਰੋਕੋ; // bfloat
            ਹਾਲਤ CV_Bool: ret = LUT_<uint8_t, uint8_t>;   ਰੋਕੋ; // bool
            ਹਾਲਤ CV_64U:  ret = LUT_<uint8_t, uint64_t>;  ਰੋਕੋ;
            ਹਾਲਤ CV_64S:  ret = LUT_<uint8_t, int64_t>;   ਰੋਕੋ;
            ਹਾਲਤ CV_32U:  ret = LUT_<uint8_t, uint32_t>;  ਰੋਕੋ;
            ਮੂਲ:      ret = ਖਾਲੀ_ਸੂਚਕ;                  ਰੋਕੋ;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ((src_ਡੂੰਘਾਈ == CV_16U) || (src_ਡੂੰਘਾਈ == CV_16S))
    {
        ਚੋਣ(dst_ਡੂੰਘਾਈ)
        {
            ਹਾਲਤ CV_8U:   ret = LUT_<uint16_t, uint8_t>;  ਰੋਕੋ;
            ਹਾਲਤ CV_8S:   ret = LUT_<uint16_t, int8_t>;   ਰੋਕੋ;
            ਹਾਲਤ CV_16U:  ret = LUT_<uint16_t, uint16_t>; ਰੋਕੋ;
            ਹਾਲਤ CV_16S:  ret = LUT_<uint16_t, int16_t>;  ਰੋਕੋ;
            ਹਾਲਤ CV_32S:  ret = LUT_<uint16_t, int32_t>;  ਰੋਕੋ;
            ਹਾਲਤ CV_32F:  ret = LUT_<uint16_t, int32_t>;  ਰੋਕੋ; // float
            ਹਾਲਤ CV_64F:  ret = LUT_<uint16_t, int64_t>;  ਰੋਕੋ; // double
            ਹਾਲਤ CV_16F:  ret = LUT_<uint16_t, int16_t>;  ਰੋਕੋ; // hfloat
            ਹਾਲਤ CV_16BF: ret = LUT_<uint16_t, int16_t>;  ਰੋਕੋ; // bfloat
            ਹਾਲਤ CV_Bool: ret = LUT_<uint16_t, uint8_t>;  ਰੋਕੋ; // bool
            ਹਾਲਤ CV_64U:  ret = LUT_<uint16_t, uint64_t>; ਰੋਕੋ;
            ਹਾਲਤ CV_64S:  ret = LUT_<uint16_t, int64_t>;  ਰੋਕੋ;
            ਹਾਲਤ CV_32U:  ret = LUT_<uint16_t, uint32_t>; ਰੋਕੋ;
            ਮੂਲ:      ret = ਖਾਲੀ_ਸੂਚਕ;                  ਰੋਕੋ;
        }
    }

    CV_CheckTrue(ret != ਖਾਲੀ_ਸੂਚਕ, "An unexpected type combination was specified.");
    ਵਾਪਸ ret;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਸਥਿਰ ਤਾਰਕਿਕ ocl_LUT(ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Array _lut, OutputArray _dst)
{
    ਪੂਰਨ lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.ਡੂੰਘਾਈ();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_ਮਾਪ(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    ਪੂਰਨ kercn = lcn == 1 ? std::min(4, ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::type_ਮੰਜ਼ਿਲ_Str(src.ਡੂੰਘਾਈ()), ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ddepth)));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    k.ਦਲੀਲਾਂ(ocl::KernelArg::Read_Only_No_ਮਾਪ(src), ocl::KernelArg::Read_Only_No_ਮਾਪ(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t global_ਮਾਪ[2] = { (size_t)dst.ਖਾਨੇ_ਗਿਣਤੀ * dcn / kercn, ((size_t)dst.ਪੰਕਤੀਆਂ + 3) / 4 };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, global_ਮਾਪ, ਨਿਰਾ, ਝੂਠ);
}

#ਜੇ_ਅੰਤ

ਵਰਗ LUTParallelBody : ਜਨਤਕ ParallelLoopBody
{
ਜਨਤਕ:
    ਅਚਲ Mat& src_;
    ਅਚਲ Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ਅਚਲ Mat& src, ਅਚਲ Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    ਖਾਲੀ ਸੰਚਾਲਕ()( ਅਚਲ cv::Range& range ) ਅਚਲ CV_OVERRIDE
    {
        ਅਚਲ ਪੂਰਨ row0 = range.ਸ਼ੁਰੂ;
        ਅਚਲ ਪੂਰਨ row1 = range.end;

        Mat src = src_.ਪੰਕਤੀ_Range_2(row0, row1);
        Mat dst = dst_.ਪੰਕਤੀ_Range_2(row0, row1);

        ਪੂਰਨ cn = src.channels();
        ਪੂਰਨ lutcn = lut_.channels();

        ਅਚਲ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ਪੂਰਨ len = (ਪੂਰਨ)it.size;

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
ਨਿਜੀ:
    LUTParallelBody(ਅਚਲ LUTParallelBody&);
    LUTParallelBody& ਸੰਚਾਲਕ=(ਅਚਲ LUTParallelBody&);
};

} // cv::

ਖਾਲੀ cv::LUT( ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ cn = _src.channels(), ਡੂੰਘਾਈ = _src.ਡੂੰਘਾਈ();
    ਪੂਰਨ lutcn = _lut.channels();
    ਅਚਲ size_t lut_ਮਾਪ = _lut.ਕੁੱਲ();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_ਮਾਪ == 256) && ((ਡੂੰਘਾਈ == CV_8U)||(ਡੂੰਘਾਈ == CV_8S))) ||
            ((lut_ਮਾਪ == 65536) && ((ਡੂੰਘਾਈ == CV_16U)||(ਡੂੰਘਾਈ == CV_16S)))
        )
    );

    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat() && _src.dims() <= 2 && (lut_ਮਾਪ == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_ਮਾਪ(_src, CV_MAKETYPE(_lut.ਡੂੰਘਾਈ(), cn));
    Mat dst = _dst.getMat();

    ਜੇ(lut_ਮਾਪ == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.ਅੰਕੜੇ, src.ਕਦਮ, src.type(), lut.ਅੰਕੜੇ,
                 lut.elemSize1(), lutcn, dst.ਅੰਕੜੇ, dst.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ);
    }
    ਨਹੀਂਤਾਂ
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.ਕਦਮ, src.type(), lut.ਅੰਕੜੇ,
                 lut.elemSize1(), lutcn, dst.ਅੰਕੜੇ, dst.ਕਦਮ, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ);
    }

    ਅਚਲ LUTFunc func = getLUTFunc(src.ਡੂੰਘਾਈ(), dst.ਡੂੰਘਾਈ());
    CV_Assert( func != ਖਾਲੀ_ਸੂਚਕ );

    ਜੇ (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.ਪੰਕਤੀਆਂ);
        ਜੇ (dst.ਕੁੱਲ() >= (size_t)(1<<18))
            parallel_for_(all, body, (ਦੁੱਗਣਾ)std::max((size_t)1, dst.ਕੁੱਲ()>>16));
        ਨਹੀਂਤਾਂ
            body(all);

        ਵਾਪਸ;
    }

    ਅਚਲ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ਪੂਰਨ len = (ਪੂਰਨ)it.size;

    ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
