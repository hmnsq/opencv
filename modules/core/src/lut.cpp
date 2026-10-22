// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "convert.hpp"
#அந்தர்பாவ <sys/types.h>

நாமாகாஶ cv {
வெறுமை LUT8u_dispatch( மாறிலி uchar* src, மாறிலி uchar* lut, uchar* dst, முழு len, முழு cn, முழு lutcn );
வெறுமை LUT16u_dispatch( மாறிலி uchar* src, மாறிலி ushort* lut, ushort* dst, முழு len, முழு cn, முழு lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

நாமாகாஶ cv
{

template<typename Ti, typename T> நிலையான வெறுமை
LUT_( மாறிலி வெறுமை* _src, மாறிலி வெறுமை* _lut, வெறுமை* _dst, முழு len, முழு cn, முழு lutcn )
{
    மாறிலி Ti* src = (மாறிலி Ti*)_src;
    மாறிலி T* lut = (மாறிலி T*)_lut;
    T* dst = (T*)_dst;
    எனில்( lutcn == 1 )
    {
        க்கு( முழு i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    இல்லையெனில்
    {
        க்கு( முழு i = 0; i < len*cn; i += cn )
            க்கு( முழு k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, வெறுமை (*fn)(மாறிலி uchar*, மாறிலி T*, T*, முழு, முழு, முழு)>
நிலையான வெறுமை lutDispatchWrap( மாறிலி வெறுமை* src, மாறிலி வெறுமை* lut, வெறுமை* dst, முழு len, முழு cn, முழு lutcn )
{
    fn((மாறிலி uchar*)src, (மாறிலி T*)lut, (T*)dst, len, cn, lutcn);
}

வகைவரை வெறுமை (*LUTFunc)( மாறிலி வெறுமை* src, மாறிலி வெறுமை* lut, வெறுமை* dst, முழு len, முழு cn, முழு lutcn );

நிலையான LUTFunc getLUTFunc(மாறிலி முழு srcDepth, மாறிலி முழு dstDepth)
{
    LUTFunc ret = nullptr;
    எனில்((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        தேர்வு(dstDepth)
        {
            நிலை CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   நிறுத்து;
            நிலை CV_8S:   ret = LUT_<uint8_t, int8_t>;    நிறுத்து;
            நிலை CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; நிறுத்து;
            நிலை CV_16S:  ret = LUT_<uint8_t, int16_t>;   நிறுத்து;
            நிலை CV_32S:  ret = LUT_<uint8_t, int32_t>;   நிறுத்து;
            நிலை CV_32F:  ret = LUT_<uint8_t, int32_t>;   நிறுத்து; // float
            நிலை CV_64F:  ret = LUT_<uint8_t, int64_t>;   நிறுத்து; // double
            நிலை CV_16F:  ret = LUT_<uint8_t, int16_t>;   நிறுத்து; // hfloat
            நிலை CV_16BF: ret = LUT_<uint8_t, int16_t>;   நிறுத்து; // bfloat
            நிலை CV_Bool: ret = LUT_<uint8_t, uint8_t>;   நிறுத்து; // bool
            நிலை CV_64U:  ret = LUT_<uint8_t, uint64_t>;  நிறுத்து;
            நிலை CV_64S:  ret = LUT_<uint8_t, int64_t>;   நிறுத்து;
            நிலை CV_32U:  ret = LUT_<uint8_t, uint32_t>;  நிறுத்து;
            இயல்பு:      ret = nullptr;                  நிறுத்து;
        }
    }
    இல்லையெனில் எனில்((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        தேர்வு(dstDepth)
        {
            நிலை CV_8U:   ret = LUT_<uint16_t, uint8_t>;  நிறுத்து;
            நிலை CV_8S:   ret = LUT_<uint16_t, int8_t>;   நிறுத்து;
            நிலை CV_16U:  ret = LUT_<uint16_t, uint16_t>; நிறுத்து;
            நிலை CV_16S:  ret = LUT_<uint16_t, int16_t>;  நிறுத்து;
            நிலை CV_32S:  ret = LUT_<uint16_t, int32_t>;  நிறுத்து;
            நிலை CV_32F:  ret = LUT_<uint16_t, int32_t>;  நிறுத்து; // float
            நிலை CV_64F:  ret = LUT_<uint16_t, int64_t>;  நிறுத்து; // double
            நிலை CV_16F:  ret = LUT_<uint16_t, int16_t>;  நிறுத்து; // hfloat
            நிலை CV_16BF: ret = LUT_<uint16_t, int16_t>;  நிறுத்து; // bfloat
            நிலை CV_Bool: ret = LUT_<uint16_t, uint8_t>;  நிறுத்து; // bool
            நிலை CV_64U:  ret = LUT_<uint16_t, uint64_t>; நிறுத்து;
            நிலை CV_64S:  ret = LUT_<uint16_t, int64_t>;  நிறுத்து;
            நிலை CV_32U:  ret = LUT_<uint16_t, uint32_t>; நிறுத்து;
            இயல்பு:      ret = nullptr;                  நிறுத்து;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    திரும்பு ret;
}

#ifdef HAVE_OPENCL

நிலையான தருக்க ocl_LUT(உள்ளீடு_Array _src, உள்ளீடு_Array _lut, OutputArray _dst)
{
    முழு lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    முழு kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    எனில் (k.empty())
        திரும்பு பொய்;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    திரும்பு k.run(2, globalSize, வெற்று, பொய்);
}

#endif

வகுப்பு LUTParallelBody : பொது ParallelLoopBody
{
பொது:
    மாறிலி Mat& src_;
    மாறிலி Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(மாறிலி Mat& src, மாறிலி Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    வெறுமை operator()( மாறிலி cv::Range& range ) மாறிலி CV_OVERRIDE
    {
        மாறிலி முழு row0 = range.start;
        மாறிலி முழு row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        முழு cn = src.channels();
        முழு lutcn = lut_.channels();

        மாறிலி Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        முழு len = (முழு)it.size;

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
தனிப்பட்ட:
    LUTParallelBody(மாறிலி LUTParallelBody&);
    LUTParallelBody& operator=(மாறிலி LUTParallelBody&);
};

} // cv::

வெறுமை cv::LUT( உள்ளீடு_Array _src, உள்ளீடு_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    முழு cn = _src.channels(), depth = _src.depth();
    முழு lutcn = _lut.channels();
    மாறிலி size_t lut_size = _lut.total();

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

    எனில்(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    இல்லையெனில்
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    மாறிலி LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    எனில் (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        எனில் (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (இரட்டை)std::max((size_t)1, dst.total()>>16));
        இல்லையெனில்
            body(all);

        திரும்பு;
    }

    மாறிலி Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு len = (முழு)it.size;

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
