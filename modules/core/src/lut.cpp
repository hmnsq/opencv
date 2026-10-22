// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "convert.hpp"
#अन्तर्भाव <sys/types.h>

नामाकाश cv {
शून्य LUT8u_dispatch( ध्रुव uchar* src, ध्रुव uchar* lut, uchar* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn );
शून्य LUT16u_dispatch( ध्रुव uchar* src, ध्रुव ushort* lut, ushort* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

नामाकाश cv
{

template<typename Ti, typename T> स्थिर शून्य
LUT_( ध्रुव शून्य* _src, ध्रुव शून्य* _lut, शून्य* _dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn )
{
    ध्रुव Ti* src = (ध्रुव Ti*)_src;
    ध्रुव T* lut = (ध्रुव T*)_lut;
    T* dst = (T*)_dst;
    सचे( lutcn == 1 )
    {
        अत्थाय( पूर्णाङ्क i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    अञ्ञथा
    {
        अत्थाय( पूर्णाङ्क i = 0; i < len*cn; i += cn )
            अत्थाय( पूर्णाङ्क k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, शून्य (*fn)(ध्रुव uchar*, ध्रुव T*, T*, पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर शून्य lutDispatchWrap( ध्रुव शून्य* src, ध्रुव शून्य* lut, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn )
{
    fn((ध्रुव uchar*)src, (ध्रुव T*)lut, (T*)dst, len, cn, lutcn);
}

प्रकारनिर्देश शून्य (*LUTFunc)( ध्रुव शून्य* src, ध्रुव शून्य* lut, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn );

स्थिर LUTFunc getLUTFunc(ध्रुव पूर्णाङ्क srcDepth, ध्रुव पूर्णाङ्क dstDepth)
{
    LUTFunc ret = nullptr;
    सचे((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        विकल्प(dstDepth)
        {
            पक्ष CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   भङ्ग;
            पक्ष CV_8S:   ret = LUT_<uint8_t, int8_t>;    भङ्ग;
            पक्ष CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; भङ्ग;
            पक्ष CV_16S:  ret = LUT_<uint8_t, int16_t>;   भङ्ग;
            पक्ष CV_32S:  ret = LUT_<uint8_t, int32_t>;   भङ्ग;
            पक्ष CV_32F:  ret = LUT_<uint8_t, int32_t>;   भङ्ग; // float
            पक्ष CV_64F:  ret = LUT_<uint8_t, int64_t>;   भङ्ग; // double
            पक्ष CV_16F:  ret = LUT_<uint8_t, int16_t>;   भङ्ग; // hfloat
            पक्ष CV_16BF: ret = LUT_<uint8_t, int16_t>;   भङ्ग; // bfloat
            पक्ष CV_Bool: ret = LUT_<uint8_t, uint8_t>;   भङ्ग; // bool
            पक्ष CV_64U:  ret = LUT_<uint8_t, uint64_t>;  भङ्ग;
            पक्ष CV_64S:  ret = LUT_<uint8_t, int64_t>;   भङ्ग;
            पक्ष CV_32U:  ret = LUT_<uint8_t, uint32_t>;  भङ्ग;
            स्वभाव:      ret = nullptr;                  भङ्ग;
        }
    }
    अञ्ञथा सचे((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        विकल्प(dstDepth)
        {
            पक्ष CV_8U:   ret = LUT_<uint16_t, uint8_t>;  भङ्ग;
            पक्ष CV_8S:   ret = LUT_<uint16_t, int8_t>;   भङ्ग;
            पक्ष CV_16U:  ret = LUT_<uint16_t, uint16_t>; भङ्ग;
            पक्ष CV_16S:  ret = LUT_<uint16_t, int16_t>;  भङ्ग;
            पक्ष CV_32S:  ret = LUT_<uint16_t, int32_t>;  भङ्ग;
            पक्ष CV_32F:  ret = LUT_<uint16_t, int32_t>;  भङ्ग; // float
            पक्ष CV_64F:  ret = LUT_<uint16_t, int64_t>;  भङ्ग; // double
            पक्ष CV_16F:  ret = LUT_<uint16_t, int16_t>;  भङ्ग; // hfloat
            पक्ष CV_16BF: ret = LUT_<uint16_t, int16_t>;  भङ्ग; // bfloat
            पक्ष CV_Bool: ret = LUT_<uint16_t, uint8_t>;  भङ्ग; // bool
            पक्ष CV_64U:  ret = LUT_<uint16_t, uint64_t>; भङ्ग;
            पक्ष CV_64S:  ret = LUT_<uint16_t, int64_t>;  भङ्ग;
            पक्ष CV_32U:  ret = LUT_<uint16_t, uint32_t>; भङ्ग;
            स्वभाव:      ret = nullptr;                  भङ्ग;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    पटिनिवत्त ret;
}

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_LUT(निवेशः_Array _src, निवेशः_Array _lut, OutputArray _dst)
{
    पूर्णाङ्क lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    पूर्णाङ्क kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    सचे (k.empty())
        पटिनिवत्त मिच्छा;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    पटिनिवत्त k.run(2, globalSize, रिक्त, मिच्छा);
}

#endif

वग्ग LUTParallelBody : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    ध्रुव Mat& src_;
    ध्रुव Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ध्रुव Mat& src, ध्रुव Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    शून्य operator()( ध्रुव cv::Range& range ) ध्रुव CV_OVERRIDE
    {
        ध्रुव पूर्णाङ्क row0 = range.start;
        ध्रुव पूर्णाङ्क row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        पूर्णाङ्क cn = src.channels();
        पूर्णाङ्क lutcn = lut_.channels();

        ध्रुव Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णाङ्क len = (पूर्णाङ्क)it.size;

        अत्थाय( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
गुप्त:
    LUTParallelBody(ध्रुव LUTParallelBody&);
    LUTParallelBody& operator=(ध्रुव LUTParallelBody&);
};

} // cv::

शून्य cv::LUT( निवेशः_Array _src, निवेशः_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क cn = _src.channels(), depth = _src.depth();
    पूर्णाङ्क lutcn = _lut.channels();
    ध्रुव size_t lut_size = _lut.total();

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

    सचे(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    अञ्ञथा
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    ध्रुव LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    सचे (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        सचे (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (द्विगुण)std::max((size_t)1, dst.total()>>16));
        अञ्ञथा
            body(all);

        पटिनिवत्त;
    }

    ध्रुव Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क len = (पूर्णाङ्क)it.size;

    अत्थाय( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
