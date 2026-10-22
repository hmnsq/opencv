// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "convert.hpp"
#અન્તર્ભાવ <sys/types.h>

નામાકાશ cv {
શૂન્ય LUT8u_dispatch( અચલ uchar* src, અચલ uchar* lut, uchar* dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn );
શૂન્ય LUT16u_dispatch( અચલ uchar* src, અચલ ushort* lut, ushort* dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

નામાકાશ cv
{

template<typename Ti, typename T> સ્થિર શૂન્ય
LUT_( અચલ શૂન્ય* _src, અચલ શૂન્ય* _lut, શૂન્ય* _dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn )
{
    અચલ Ti* src = (અચલ Ti*)_src;
    અચલ T* lut = (અચલ T*)_lut;
    T* dst = (T*)_dst;
    જો( lutcn == 1 )
    {
        માટે( પૂર્ણાંક i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    નહીંતર
    {
        માટે( પૂર્ણાંક i = 0; i < len*cn; i += cn )
            માટે( પૂર્ણાંક k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, શૂન્ય (*fn)(અચલ uchar*, અચલ T*, T*, પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક)>
સ્થિર શૂન્ય lutDispatchWrap( અચલ શૂન્ય* src, અચલ શૂન્ય* lut, શૂન્ય* dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn )
{
    fn((અચલ uchar*)src, (અચલ T*)lut, (T*)dst, len, cn, lutcn);
}

પ્રકારવ્યાખ્યા શૂન્ય (*LUTFunc)( અચલ શૂન્ય* src, અચલ શૂન્ય* lut, શૂન્ય* dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn );

સ્થિર LUTFunc getLUTFunc(અચલ પૂર્ણાંક srcDepth, અચલ પૂર્ણાંક dstDepth)
{
    LUTFunc ret = nullptr;
    જો((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        પસંદગી(dstDepth)
        {
            સ્થિતિ CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   તોડો;
            સ્થિતિ CV_8S:   ret = LUT_<uint8_t, int8_t>;    તોડો;
            સ્થિતિ CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; તોડો;
            સ્થિતિ CV_16S:  ret = LUT_<uint8_t, int16_t>;   તોડો;
            સ્થિતિ CV_32S:  ret = LUT_<uint8_t, int32_t>;   તોડો;
            સ્થિતિ CV_32F:  ret = LUT_<uint8_t, int32_t>;   તોડો; // float
            સ્થિતિ CV_64F:  ret = LUT_<uint8_t, int64_t>;   તોડો; // double
            સ્થિતિ CV_16F:  ret = LUT_<uint8_t, int16_t>;   તોડો; // hfloat
            સ્થિતિ CV_16BF: ret = LUT_<uint8_t, int16_t>;   તોડો; // bfloat
            સ્થિતિ CV_Bool: ret = LUT_<uint8_t, uint8_t>;   તોડો; // bool
            સ્થિતિ CV_64U:  ret = LUT_<uint8_t, uint64_t>;  તોડો;
            સ્થિતિ CV_64S:  ret = LUT_<uint8_t, int64_t>;   તોડો;
            સ્થિતિ CV_32U:  ret = LUT_<uint8_t, uint32_t>;  તોડો;
            મૂળભૂત:      ret = nullptr;                  તોડો;
        }
    }
    નહીંતર જો((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        પસંદગી(dstDepth)
        {
            સ્થિતિ CV_8U:   ret = LUT_<uint16_t, uint8_t>;  તોડો;
            સ્થિતિ CV_8S:   ret = LUT_<uint16_t, int8_t>;   તોડો;
            સ્થિતિ CV_16U:  ret = LUT_<uint16_t, uint16_t>; તોડો;
            સ્થિતિ CV_16S:  ret = LUT_<uint16_t, int16_t>;  તોડો;
            સ્થિતિ CV_32S:  ret = LUT_<uint16_t, int32_t>;  તોડો;
            સ્થિતિ CV_32F:  ret = LUT_<uint16_t, int32_t>;  તોડો; // float
            સ્થિતિ CV_64F:  ret = LUT_<uint16_t, int64_t>;  તોડો; // double
            સ્થિતિ CV_16F:  ret = LUT_<uint16_t, int16_t>;  તોડો; // hfloat
            સ્થિતિ CV_16BF: ret = LUT_<uint16_t, int16_t>;  તોડો; // bfloat
            સ્થિતિ CV_Bool: ret = LUT_<uint16_t, uint8_t>;  તોડો; // bool
            સ્થિતિ CV_64U:  ret = LUT_<uint16_t, uint64_t>; તોડો;
            સ્થિતિ CV_64S:  ret = LUT_<uint16_t, int64_t>;  તોડો;
            સ્થિતિ CV_32U:  ret = LUT_<uint16_t, uint32_t>; તોડો;
            મૂળભૂત:      ret = nullptr;                  તોડો;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    પાછા ret;
}

#ifdef HAVE_OPENCL

સ્થિર તાર્કિક ocl_LUT(આગત_Array _src, આગત_Array _lut, OutputArray _dst)
{
    પૂર્ણાંક lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    પૂર્ણાંક kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    જો (k.empty())
        પાછા ખોટું;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    પાછા k.run(2, globalSize, ખાલી, ખોટું);
}

#endif

વર્ગ LUTParallelBody : જાહેર ParallelLoopBody
{
જાહેર:
    અચલ Mat& src_;
    અચલ Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(અચલ Mat& src, અચલ Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    શૂન્ય operator()( અચલ cv::Range& range ) અચલ CV_OVERRIDE
    {
        અચલ પૂર્ણાંક row0 = range.start;
        અચલ પૂર્ણાંક row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        પૂર્ણાંક cn = src.channels();
        પૂર્ણાંક lutcn = lut_.channels();

        અચલ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        પૂર્ણાંક len = (પૂર્ણાંક)it.size;

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
ખાનગી:
    LUTParallelBody(અચલ LUTParallelBody&);
    LUTParallelBody& operator=(અચલ LUTParallelBody&);
};

} // cv::

શૂન્ય cv::LUT( આગત_Array _src, આગત_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક cn = _src.channels(), depth = _src.depth();
    પૂર્ણાંક lutcn = _lut.channels();
    અચલ size_t lut_size = _lut.total();

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

    જો(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    નહીંતર
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    અચલ LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    જો (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        જો (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (દ્વિગુણ)std::max((size_t)1, dst.total()>>16));
        નહીંતર
            body(all);

        પાછા;
    }

    અચલ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    પૂર્ણાંક len = (પૂર્ણાંક)it.size;

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
