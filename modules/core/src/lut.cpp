// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "convert.hpp"
#अन्तर्भाव <sys/types.h>

नामाकाश cv {
शून्य LUT8u_dispatch( अचर uchar* src, अचर uchar* lut, uchar* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn );
शून्य LUT16u_dispatch( अचर uchar* src, अचर ushort* lut, ushort* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

नामाकाश cv
{

ढाँचा<typename Ti, typename T> स्थिर शून्य
LUT_( अचर शून्य* _src, अचर शून्य* _lut, शून्य* _dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn )
{
    अचर Ti* src = (अचर Ti*)_src;
    अचर T* lut = (अचर T*)_lut;
    T* dst = (T*)_dst;
    यदि( lutcn == 1 )
    {
        लागि( पूर्णाङ्क i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    अन्यथा
    {
        लागि( पूर्णाङ्क i = 0; i < len*cn; i += cn )
            लागि( पूर्णाङ्क k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

ढाँचा<typename T, शून्य (*fn)(अचर uchar*, अचर T*, T*, पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर शून्य lutDispatchWrap( अचर शून्य* src, अचर शून्य* lut, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn )
{
    fn((अचर uchar*)src, (अचर T*)lut, (T*)dst, len, cn, lutcn);
}

प्रकारनिर्देश शून्य (*LUTFunc)( अचर शून्य* src, अचर शून्य* lut, शून्य* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn );

स्थिर LUTFunc getLUTFunc(अचर पूर्णाङ्क srcDepth, अचर पूर्णाङ्क dstDepth)
{
    LUTFunc ret = nullptr;
    यदि((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        विकल्प(dstDepth)
        {
            पक्ष CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   रोक;
            पक्ष CV_8S:   ret = LUT_<uint8_t, int8_t>;    रोक;
            पक्ष CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; रोक;
            पक्ष CV_16S:  ret = LUT_<uint8_t, int16_t>;   रोक;
            पक्ष CV_32S:  ret = LUT_<uint8_t, int32_t>;   रोक;
            पक्ष CV_32F:  ret = LUT_<uint8_t, int32_t>;   रोक; // float
            पक्ष CV_64F:  ret = LUT_<uint8_t, int64_t>;   रोक; // double
            पक्ष CV_16F:  ret = LUT_<uint8_t, int16_t>;   रोक; // hfloat
            पक्ष CV_16BF: ret = LUT_<uint8_t, int16_t>;   रोक; // bfloat
            पक्ष CV_Bool: ret = LUT_<uint8_t, uint8_t>;   रोक; // bool
            पक्ष CV_64U:  ret = LUT_<uint8_t, uint64_t>;  रोक;
            पक्ष CV_64S:  ret = LUT_<uint8_t, int64_t>;   रोक;
            पक्ष CV_32U:  ret = LUT_<uint8_t, uint32_t>;  रोक;
            स्वभाव:      ret = nullptr;                  रोक;
        }
    }
    अन्यथा यदि((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        विकल्प(dstDepth)
        {
            पक्ष CV_8U:   ret = LUT_<uint16_t, uint8_t>;  रोक;
            पक्ष CV_8S:   ret = LUT_<uint16_t, int8_t>;   रोक;
            पक्ष CV_16U:  ret = LUT_<uint16_t, uint16_t>; रोक;
            पक्ष CV_16S:  ret = LUT_<uint16_t, int16_t>;  रोक;
            पक्ष CV_32S:  ret = LUT_<uint16_t, int32_t>;  रोक;
            पक्ष CV_32F:  ret = LUT_<uint16_t, int32_t>;  रोक; // float
            पक्ष CV_64F:  ret = LUT_<uint16_t, int64_t>;  रोक; // double
            पक्ष CV_16F:  ret = LUT_<uint16_t, int16_t>;  रोक; // hfloat
            पक्ष CV_16BF: ret = LUT_<uint16_t, int16_t>;  रोक; // bfloat
            पक्ष CV_Bool: ret = LUT_<uint16_t, uint8_t>;  रोक; // bool
            पक्ष CV_64U:  ret = LUT_<uint16_t, uint64_t>; रोक;
            पक्ष CV_64S:  ret = LUT_<uint16_t, int64_t>;  रोक;
            पक्ष CV_32U:  ret = LUT_<uint16_t, uint32_t>; रोक;
            स्वभाव:      ret = nullptr;                  रोक;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    फर्काऊ ret;
}

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_LUT(आगत_Array _src, आगत_Array _lut, OutputArray _dst)
{
    पूर्णाङ्क lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    पूर्णाङ्क kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    यदि (k.empty())
        फर्काऊ असत्य;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    माप_प्रकार globalSize[2] = { (माप_प्रकार)dst.cols * dcn / kercn, ((माप_प्रकार)dst.rows + 3) / 4 };
    फर्काऊ k.run(2, globalSize, रिक्त, असत्य);
}

#endif

वर्ग LUTParallelBody : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    अचर Mat& src_;
    अचर Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(अचर Mat& src, अचर Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    शून्य चालक()( अचर cv::Range& range ) अचर CV_OVERRIDE
    {
        अचर पूर्णाङ्क row0 = range.start;
        अचर पूर्णाङ्क row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        पूर्णाङ्क cn = src.channels();
        पूर्णाङ्क lutcn = lut_.channels();

        अचर Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णाङ्क len = (पूर्णाङ्क)it.size;

        लागि( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
गुप्त:
    LUTParallelBody(अचर LUTParallelBody&);
    LUTParallelBody& चालक=(अचर LUTParallelBody&);
};

} // cv::

शून्य cv::LUT( आगत_Array _src, आगत_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क cn = _src.channels(), depth = _src.depth();
    पूर्णाङ्क lutcn = _lut.channels();
    अचर माप_प्रकार lut_size = _lut.total();

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

    यदि(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    अन्यथा
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    अचर LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    यदि (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        यदि (dst.total() >= (माप_प्रकार)(1<<18))
            parallel_for_(all, body, (द्विगुण)std::max((माप_प्रकार)1, dst.total()>>16));
        अन्यथा
            body(all);

        फर्काऊ;
    }

    अचर Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क len = (पूर्णाङ्क)it.size;

    लागि( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
