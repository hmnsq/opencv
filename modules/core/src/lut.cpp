// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "convert.hpp"
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

ନାମାକାଶ cv {
ଶୂନ୍ୟ LUT8u_dispatch( ଅଚଳ uchar* src, ଅଚଳ uchar* lut, uchar* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn );
ଶୂନ୍ୟ LUT16u_dispatch( ଅଚଳ uchar* src, ଅଚଳ ushort* lut, ushort* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

ନାମାକାଶ cv
{

template<typename Ti, typename T> ସ୍ଥିର ଶୂନ୍ୟ
LUT_( ଅଚଳ ଶୂନ୍ୟ* _src, ଅଚଳ ଶୂନ୍ୟ* _lut, ଶୂନ୍ୟ* _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn )
{
    ଅଚଳ Ti* src = (ଅଚଳ Ti*)_src;
    ଅଚଳ T* lut = (ଅଚଳ T*)_lut;
    T* dst = (T*)_dst;
    ଯଦି( lutcn == 1 )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < len*cn; i += cn )
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, ଶୂନ୍ୟ (*fn)(ଅଚଳ uchar*, ଅଚଳ T*, T*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା)>
ସ୍ଥିର ଶୂନ୍ୟ lutDispatchWrap( ଅଚଳ ଶୂନ୍ୟ* src, ଅଚଳ ଶୂନ୍ୟ* lut, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn )
{
    fn((ଅଚଳ uchar*)src, (ଅଚଳ T*)lut, (T*)dst, len, cn, lutcn);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*LUTFunc)( ଅଚଳ ଶୂନ୍ୟ* src, ଅଚଳ ଶୂନ୍ୟ* lut, ଶୂନ୍ୟ* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn );

ସ୍ଥିର LUTFunc getLUTFunc(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcDepth, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstDepth)
{
    LUTFunc ret = nullptr;
    ଯଦି((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        ବାଛ(dstDepth)
        {
            ସ୍ଥିତି CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_8S:   ret = LUT_<uint8_t, int8_t>;    ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_16S:  ret = LUT_<uint8_t, int16_t>;   ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_32S:  ret = LUT_<uint8_t, int32_t>;   ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_32F:  ret = LUT_<uint8_t, int32_t>;   ଭାଙ୍ଗ; // float
            ସ୍ଥିତି CV_64F:  ret = LUT_<uint8_t, int64_t>;   ଭାଙ୍ଗ; // double
            ସ୍ଥିତି CV_16F:  ret = LUT_<uint8_t, int16_t>;   ଭାଙ୍ଗ; // hfloat
            ସ୍ଥିତି CV_16BF: ret = LUT_<uint8_t, int16_t>;   ଭାଙ୍ଗ; // bfloat
            ସ୍ଥିତି CV_Bool: ret = LUT_<uint8_t, uint8_t>;   ଭାଙ୍ଗ; // bool
            ସ୍ଥିତି CV_64U:  ret = LUT_<uint8_t, uint64_t>;  ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_64S:  ret = LUT_<uint8_t, int64_t>;   ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_32U:  ret = LUT_<uint8_t, uint32_t>;  ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:      ret = nullptr;                  ଭାଙ୍ଗ;
        }
    }
    ଅନ୍ୟଥା ଯଦି((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        ବାଛ(dstDepth)
        {
            ସ୍ଥିତି CV_8U:   ret = LUT_<uint16_t, uint8_t>;  ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_8S:   ret = LUT_<uint16_t, int8_t>;   ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_16U:  ret = LUT_<uint16_t, uint16_t>; ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_16S:  ret = LUT_<uint16_t, int16_t>;  ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_32S:  ret = LUT_<uint16_t, int32_t>;  ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_32F:  ret = LUT_<uint16_t, int32_t>;  ଭାଙ୍ଗ; // float
            ସ୍ଥିତି CV_64F:  ret = LUT_<uint16_t, int64_t>;  ଭାଙ୍ଗ; // double
            ସ୍ଥିତି CV_16F:  ret = LUT_<uint16_t, int16_t>;  ଭାଙ୍ଗ; // hfloat
            ସ୍ଥିତି CV_16BF: ret = LUT_<uint16_t, int16_t>;  ଭାଙ୍ଗ; // bfloat
            ସ୍ଥିତି CV_Bool: ret = LUT_<uint16_t, uint8_t>;  ଭାଙ୍ଗ; // bool
            ସ୍ଥିତି CV_64U:  ret = LUT_<uint16_t, uint64_t>; ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_64S:  ret = LUT_<uint16_t, int64_t>;  ଭାଙ୍ଗ;
            ସ୍ଥିତି CV_32U:  ret = LUT_<uint16_t, uint32_t>; ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:      ret = nullptr;                  ଭାଙ୍ଗ;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    ଫେରନ୍ତୁ ret;
}

#ifdef HAVE_OPENCL

ସ୍ଥିର ତାର୍କିକ ocl_LUT(ନିବେଶ_Array _src, ନିବେଶ_Array _lut, OutputArray _dst)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    ଫେରନ୍ତୁ k.run(2, globalSize, ଖାଲି, ମିଥ୍ୟା);
}

#endif

ଶ୍ରେଣୀ LUTParallelBody : ସର୍ବସାଧାରଣ ParallelLoopBody
{
ସର୍ବସାଧାରଣ:
    ଅଚଳ Mat& src_;
    ଅଚଳ Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ଅଚଳ Mat& src, ଅଚଳ Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    ଶୂନ୍ୟ operator()( ଅଚଳ cv::Range& range ) ଅଚଳ CV_OVERRIDE
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା row0 = range.start;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = src.channels();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn = lut_.channels();

        ଅଚଳ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size;

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
ବ୍ୟକ୍ତିଗତ:
    LUTParallelBody(ଅଚଳ LUTParallelBody&);
    LUTParallelBody& operator=(ଅଚଳ LUTParallelBody&);
};

} // cv::

ଶୂନ୍ୟ cv::LUT( ନିବେଶ_Array _src, ନିବେଶ_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = _src.channels(), depth = _src.depth();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn = _lut.channels();
    ଅଚଳ size_t lut_size = _lut.total();

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

    ଯଦି(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    ଅନ୍ୟଥା
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    ଅଚଳ LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    ଯଦି (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        ଯଦି (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (ଦ୍ୱିଗୁଣ)std::max((size_t)1, dst.total()>>16));
        ଅନ୍ୟଥା
            body(all);

        ଫେରନ୍ତୁ;
    }

    ଅଚଳ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size;

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
