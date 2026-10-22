// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "convert.hpp"
#شامل <sys/types.h>

فضاینام cv {
تهی LUT8u_dispatch( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحیح len, صحیح cn, صحیح lutcn );
تهی LUT16u_dispatch( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحیح len, صحیح cn, صحیح lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

فضاینام cv
{

template<typename Ti, typename T> ایستا تهی
LUT_( ثابت تهی* _src, ثابت تهی* _lut, تهی* _dst, صحیح len, صحیح cn, صحیح lutcn )
{
    ثابت Ti* src = (ثابت Ti*)_src;
    ثابت T* lut = (ثابت T*)_lut;
    T* dst = (T*)_dst;
    اگر( lutcn == 1 )
    {
        برای( صحیح i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    وگرنه
    {
        برای( صحیح i = 0; i < len*cn; i += cn )
            برای( صحیح k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, تهی (*fn)(ثابت uchar*, ثابت T*, T*, صحیح, صحیح, صحیح)>
ایستا تهی lutDispatchWrap( ثابت تهی* src, ثابت تهی* lut, تهی* dst, صحیح len, صحیح cn, صحیح lutcn )
{
    fn((ثابت uchar*)src, (ثابت T*)lut, (T*)dst, len, cn, lutcn);
}

تعریفنوع تهی (*LUTFunc)( ثابت تهی* src, ثابت تهی* lut, تهی* dst, صحیح len, صحیح cn, صحیح lutcn );

ایستا LUTFunc getLUTFunc(ثابت صحیح srcDepth, ثابت صحیح dstDepth)
{
    LUTFunc ret = nullptr;
    اگر((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        انتخاب(dstDepth)
        {
            حالت CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   بشکن;
            حالت CV_8S:   ret = LUT_<uint8_t, int8_t>;    بشکن;
            حالت CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; بشکن;
            حالت CV_16S:  ret = LUT_<uint8_t, int16_t>;   بشکن;
            حالت CV_32S:  ret = LUT_<uint8_t, int32_t>;   بشکن;
            حالت CV_32F:  ret = LUT_<uint8_t, int32_t>;   بشکن; // float
            حالت CV_64F:  ret = LUT_<uint8_t, int64_t>;   بشکن; // double
            حالت CV_16F:  ret = LUT_<uint8_t, int16_t>;   بشکن; // hfloat
            حالت CV_16BF: ret = LUT_<uint8_t, int16_t>;   بشکن; // bfloat
            حالت CV_Bool: ret = LUT_<uint8_t, uint8_t>;   بشکن; // bool
            حالت CV_64U:  ret = LUT_<uint8_t, uint64_t>;  بشکن;
            حالت CV_64S:  ret = LUT_<uint8_t, int64_t>;   بشکن;
            حالت CV_32U:  ret = LUT_<uint8_t, uint32_t>;  بشکن;
            پیشفرض:      ret = nullptr;                  بشکن;
        }
    }
    وگرنه اگر((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        انتخاب(dstDepth)
        {
            حالت CV_8U:   ret = LUT_<uint16_t, uint8_t>;  بشکن;
            حالت CV_8S:   ret = LUT_<uint16_t, int8_t>;   بشکن;
            حالت CV_16U:  ret = LUT_<uint16_t, uint16_t>; بشکن;
            حالت CV_16S:  ret = LUT_<uint16_t, int16_t>;  بشکن;
            حالت CV_32S:  ret = LUT_<uint16_t, int32_t>;  بشکن;
            حالت CV_32F:  ret = LUT_<uint16_t, int32_t>;  بشکن; // float
            حالت CV_64F:  ret = LUT_<uint16_t, int64_t>;  بشکن; // double
            حالت CV_16F:  ret = LUT_<uint16_t, int16_t>;  بشکن; // hfloat
            حالت CV_16BF: ret = LUT_<uint16_t, int16_t>;  بشکن; // bfloat
            حالت CV_Bool: ret = LUT_<uint16_t, uint8_t>;  بشکن; // bool
            حالت CV_64U:  ret = LUT_<uint16_t, uint64_t>; بشکن;
            حالت CV_64S:  ret = LUT_<uint16_t, int64_t>;  بشکن;
            حالت CV_32U:  ret = LUT_<uint16_t, uint32_t>; بشکن;
            پیشفرض:      ret = nullptr;                  بشکن;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    بازگشت ret;
}

#ifdef HAVE_OPENCL

ایستا منطقی ocl_LUT(درون_داد_Array _src, درون_داد_Array _lut, OutputArray _dst)
{
    صحیح lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    صحیح kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    اگر (k.empty())
        بازگشت نادرست;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    بازگشت k.run(2, globalSize, پوچ, نادرست);
}

#endif

رده LUTParallelBody : عمومی ParallelLoopBody
{
عمومی:
    ثابت Mat& src_;
    ثابت Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ثابت Mat& src, ثابت Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    تهی operator()( ثابت cv::Range& range ) ثابت CV_OVERRIDE
    {
        ثابت صحیح row0 = range.start;
        ثابت صحیح row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        صحیح cn = src.channels();
        صحیح lutcn = lut_.channels();

        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحیح len = (صحیح)it.size;

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
خصوصی:
    LUTParallelBody(ثابت LUTParallelBody&);
    LUTParallelBody& operator=(ثابت LUTParallelBody&);
};

} // cv::

تهی cv::LUT( درون_داد_Array _src, درون_داد_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحیح cn = _src.channels(), depth = _src.depth();
    صحیح lutcn = _lut.channels();
    ثابت size_t lut_size = _lut.total();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_size == 256) && ((depth == CV_8U)||(depth == CV_8S))) ||
            ((lut_size == 65536) && ((depth == CV_16U)||(depth == CV_16S)))
        )
    );

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (lut_size == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.createSameSize(_src, CV_MAKETYPE(_lut.depth(), cn));
    Mat dst = _dst.getMat();

    اگر(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    وگرنه
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    ثابت LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    اگر (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        اگر (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (دوگانه)std::max((size_t)1, dst.total()>>16));
        وگرنه
            body(all);

        بازگشت;
    }

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح len = (صحیح)it.size;

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
