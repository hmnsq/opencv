// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "convert.hpp"
#അന്തര്ഭാവ <sys/types.h>

നാമാകാശ cv {
ശൂന്യം LUT8u_dispatch( സ്ഥിരം uchar* src, സ്ഥിരം uchar* lut, uchar* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn );
ശൂന്യം LUT16u_dispatch( സ്ഥിരം uchar* src, സ്ഥിരം ushort* lut, ushort* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

നാമാകാശ cv
{

template<typename Ti, typename T> സ്ഥിതം ശൂന്യം
LUT_( സ്ഥിരം ശൂന്യം* _src, സ്ഥിരം ശൂന്യം* _lut, ശൂന്യം* _dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn )
{
    സ്ഥിരം Ti* src = (സ്ഥിരം Ti*)_src;
    സ്ഥിരം T* lut = (സ്ഥിരം T*)_lut;
    T* dst = (T*)_dst;
    എങ്കിൽ( lutcn == 1 )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < len*cn; i += cn )
            വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, ശൂന്യം (*fn)(സ്ഥിരം uchar*, സ്ഥിരം T*, T*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ)>
സ്ഥിതം ശൂന്യം lutDispatchWrap( സ്ഥിരം ശൂന്യം* src, സ്ഥിരം ശൂന്യം* lut, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn )
{
    fn((സ്ഥിരം uchar*)src, (സ്ഥിരം T*)lut, (T*)dst, len, cn, lutcn);
}

തരനിർവചനം ശൂന്യം (*LUTFunc)( സ്ഥിരം ശൂന്യം* src, സ്ഥിരം ശൂന്യം* lut, ശൂന്യം* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn );

സ്ഥിതം LUTFunc getLUTFunc(സ്ഥിരം പൂർണ്ണസംഖ്യ srcDepth, സ്ഥിരം പൂർണ്ണസംഖ്യ dstDepth)
{
    LUTFunc ret = nullptr;
    എങ്കിൽ((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        തിരഞ്ഞെടുപ്പ്(dstDepth)
        {
            അവസ്ഥ CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   നിർത്തുക;
            അവസ്ഥ CV_8S:   ret = LUT_<uint8_t, int8_t>;    നിർത്തുക;
            അവസ്ഥ CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; നിർത്തുക;
            അവസ്ഥ CV_16S:  ret = LUT_<uint8_t, int16_t>;   നിർത്തുക;
            അവസ്ഥ CV_32S:  ret = LUT_<uint8_t, int32_t>;   നിർത്തുക;
            അവസ്ഥ CV_32F:  ret = LUT_<uint8_t, int32_t>;   നിർത്തുക; // float
            അവസ്ഥ CV_64F:  ret = LUT_<uint8_t, int64_t>;   നിർത്തുക; // double
            അവസ്ഥ CV_16F:  ret = LUT_<uint8_t, int16_t>;   നിർത്തുക; // hfloat
            അവസ്ഥ CV_16BF: ret = LUT_<uint8_t, int16_t>;   നിർത്തുക; // bfloat
            അവസ്ഥ CV_Bool: ret = LUT_<uint8_t, uint8_t>;   നിർത്തുക; // bool
            അവസ്ഥ CV_64U:  ret = LUT_<uint8_t, uint64_t>;  നിർത്തുക;
            അവസ്ഥ CV_64S:  ret = LUT_<uint8_t, int64_t>;   നിർത്തുക;
            അവസ്ഥ CV_32U:  ret = LUT_<uint8_t, uint32_t>;  നിർത്തുക;
            സ്ഥിരസ്ഥിതി:      ret = nullptr;                  നിർത്തുക;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        തിരഞ്ഞെടുപ്പ്(dstDepth)
        {
            അവസ്ഥ CV_8U:   ret = LUT_<uint16_t, uint8_t>;  നിർത്തുക;
            അവസ്ഥ CV_8S:   ret = LUT_<uint16_t, int8_t>;   നിർത്തുക;
            അവസ്ഥ CV_16U:  ret = LUT_<uint16_t, uint16_t>; നിർത്തുക;
            അവസ്ഥ CV_16S:  ret = LUT_<uint16_t, int16_t>;  നിർത്തുക;
            അവസ്ഥ CV_32S:  ret = LUT_<uint16_t, int32_t>;  നിർത്തുക;
            അവസ്ഥ CV_32F:  ret = LUT_<uint16_t, int32_t>;  നിർത്തുക; // float
            അവസ്ഥ CV_64F:  ret = LUT_<uint16_t, int64_t>;  നിർത്തുക; // double
            അവസ്ഥ CV_16F:  ret = LUT_<uint16_t, int16_t>;  നിർത്തുക; // hfloat
            അവസ്ഥ CV_16BF: ret = LUT_<uint16_t, int16_t>;  നിർത്തുക; // bfloat
            അവസ്ഥ CV_Bool: ret = LUT_<uint16_t, uint8_t>;  നിർത്തുക; // bool
            അവസ്ഥ CV_64U:  ret = LUT_<uint16_t, uint64_t>; നിർത്തുക;
            അവസ്ഥ CV_64S:  ret = LUT_<uint16_t, int64_t>;  നിർത്തുക;
            അവസ്ഥ CV_32U:  ret = LUT_<uint16_t, uint32_t>; നിർത്തുക;
            സ്ഥിരസ്ഥിതി:      ret = nullptr;                  നിർത്തുക;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    തിരികെ ret;
}

#ifdef HAVE_OPENCL

സ്ഥിതം തർക്കം ocl_LUT(നിവേശം_Array _src, നിവേശം_Array _lut, OutputArray _dst)
{
    പൂർണ്ണസംഖ്യ lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    പൂർണ്ണസംഖ്യ kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    തിരികെ k.run(2, globalSize, ശൂന്യ, അസത്യം);
}

#endif

വർഗ്ഗം LUTParallelBody : പൊതു ParallelLoopBody
{
പൊതു:
    സ്ഥിരം Mat& src_;
    സ്ഥിരം Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(സ്ഥിരം Mat& src, സ്ഥിരം Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    ശൂന്യം operator()( സ്ഥിരം cv::Range& range ) സ്ഥിരം CV_OVERRIDE
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ row0 = range.start;
        സ്ഥിരം പൂർണ്ണസംഖ്യ row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        പൂർണ്ണസംഖ്യ cn = src.channels();
        പൂർണ്ണസംഖ്യ lutcn = lut_.channels();

        സ്ഥിരം Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        പൂർണ്ണസംഖ്യ len = (പൂർണ്ണസംഖ്യ)it.size;

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
സ്വകാര്യ:
    LUTParallelBody(സ്ഥിരം LUTParallelBody&);
    LUTParallelBody& operator=(സ്ഥിരം LUTParallelBody&);
};

} // cv::

ശൂന്യം cv::LUT( നിവേശം_Array _src, നിവേശം_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ cn = _src.channels(), depth = _src.depth();
    പൂർണ്ണസംഖ്യ lutcn = _lut.channels();
    സ്ഥിരം size_t lut_size = _lut.total();

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

    എങ്കിൽ(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    അല്ലെങ്കിൽ
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    സ്ഥിരം LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    എങ്കിൽ (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        എങ്കിൽ (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (ഇരട്ട)std::max((size_t)1, dst.total()>>16));
        അല്ലെങ്കിൽ
            body(all);

        തിരികെ;
    }

    സ്ഥിരം Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ len = (പൂർണ്ണസംഖ്യ)it.size;

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
