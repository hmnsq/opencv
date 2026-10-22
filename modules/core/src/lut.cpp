// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "convert.hpp"
#অন্তর্ভাব <sys/types.h>

নামাকাশ cv {
শূন্য LUT8u_dispatch( ধ্রুব uchar* src, ধ্রুব uchar* lut, uchar* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn );
শূন্য LUT16u_dispatch( ধ্রুব uchar* src, ধ্রুব ushort* lut, ushort* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

নামাকাশ cv
{

template<typename Ti, typename T> স্থির শূন্য
LUT_( ধ্রুব শূন্য* _src, ধ্রুব শূন্য* _lut, শূন্য* _dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn )
{
    ধ্রুব Ti* src = (ধ্রুব Ti*)_src;
    ধ্রুব T* lut = (ধ্রুব T*)_lut;
    T* dst = (T*)_dst;
    যদি( lutcn == 1 )
    {
        প্রতি( পূর্ণাঙ্ক i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    অন্যথা
    {
        প্রতি( পূর্ণাঙ্ক i = 0; i < len*cn; i += cn )
            প্রতি( পূর্ণাঙ্ক k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, শূন্য (*fn)(ধ্রুব uchar*, ধ্রুব T*, T*, পূর্ণাঙ্ক, পূর্ণাঙ্ক, পূর্ণাঙ্ক)>
স্থির শূন্য lutDispatchWrap( ধ্রুব শূন্য* src, ধ্রুব শূন্য* lut, শূন্য* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn )
{
    fn((ধ্রুব uchar*)src, (ধ্রুব T*)lut, (T*)dst, len, cn, lutcn);
}

প্রকারনির্দেশ শূন্য (*LUTFunc)( ধ্রুব শূন্য* src, ধ্রুব শূন্য* lut, শূন্য* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn );

স্থির LUTFunc getLUTFunc(ধ্রুব পূর্ণাঙ্ক srcDepth, ধ্রুব পূর্ণাঙ্ক dstDepth)
{
    LUTFunc ret = nullptr;
    যদি((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        বিকল্প(dstDepth)
        {
            পক্ষ CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   ভঙ্গ;
            পক্ষ CV_8S:   ret = LUT_<uint8_t, int8_t>;    ভঙ্গ;
            পক্ষ CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; ভঙ্গ;
            পক্ষ CV_16S:  ret = LUT_<uint8_t, int16_t>;   ভঙ্গ;
            পক্ষ CV_32S:  ret = LUT_<uint8_t, int32_t>;   ভঙ্গ;
            পক্ষ CV_32F:  ret = LUT_<uint8_t, int32_t>;   ভঙ্গ; // float
            পক্ষ CV_64F:  ret = LUT_<uint8_t, int64_t>;   ভঙ্গ; // double
            পক্ষ CV_16F:  ret = LUT_<uint8_t, int16_t>;   ভঙ্গ; // hfloat
            পক্ষ CV_16BF: ret = LUT_<uint8_t, int16_t>;   ভঙ্গ; // bfloat
            পক্ষ CV_Bool: ret = LUT_<uint8_t, uint8_t>;   ভঙ্গ; // bool
            পক্ষ CV_64U:  ret = LUT_<uint8_t, uint64_t>;  ভঙ্গ;
            পক্ষ CV_64S:  ret = LUT_<uint8_t, int64_t>;   ভঙ্গ;
            পক্ষ CV_32U:  ret = LUT_<uint8_t, uint32_t>;  ভঙ্গ;
            স্বভাব:      ret = nullptr;                  ভঙ্গ;
        }
    }
    অন্যথা যদি((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        বিকল্প(dstDepth)
        {
            পক্ষ CV_8U:   ret = LUT_<uint16_t, uint8_t>;  ভঙ্গ;
            পক্ষ CV_8S:   ret = LUT_<uint16_t, int8_t>;   ভঙ্গ;
            পক্ষ CV_16U:  ret = LUT_<uint16_t, uint16_t>; ভঙ্গ;
            পক্ষ CV_16S:  ret = LUT_<uint16_t, int16_t>;  ভঙ্গ;
            পক্ষ CV_32S:  ret = LUT_<uint16_t, int32_t>;  ভঙ্গ;
            পক্ষ CV_32F:  ret = LUT_<uint16_t, int32_t>;  ভঙ্গ; // float
            পক্ষ CV_64F:  ret = LUT_<uint16_t, int64_t>;  ভঙ্গ; // double
            পক্ষ CV_16F:  ret = LUT_<uint16_t, int16_t>;  ভঙ্গ; // hfloat
            পক্ষ CV_16BF: ret = LUT_<uint16_t, int16_t>;  ভঙ্গ; // bfloat
            পক্ষ CV_Bool: ret = LUT_<uint16_t, uint8_t>;  ভঙ্গ; // bool
            পক্ষ CV_64U:  ret = LUT_<uint16_t, uint64_t>; ভঙ্গ;
            পক্ষ CV_64S:  ret = LUT_<uint16_t, int64_t>;  ভঙ্গ;
            পক্ষ CV_32U:  ret = LUT_<uint16_t, uint32_t>; ভঙ্গ;
            স্বভাব:      ret = nullptr;                  ভঙ্গ;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    প্রত্যাগম ret;
}

#ifdef HAVE_OPENCL

স্থির তার্কিক ocl_LUT(প্রবেশ_Array _src, প্রবেশ_Array _lut, OutputArray _dst)
{
    পূর্ণাঙ্ক lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    পূর্ণাঙ্ক kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    প্রত্যাগম k.run(2, globalSize, রিক্ত, অসত্য);
}

#endif

বর্গ LUTParallelBody : সার্বজনিক ParallelLoopBody
{
সার্বজনিক:
    ধ্রুব Mat& src_;
    ধ্রুব Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ধ্রুব Mat& src, ধ্রুব Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    শূন্য operator()( ধ্রুব cv::Range& range ) ধ্রুব CV_OVERRIDE
    {
        ধ্রুব পূর্ণাঙ্ক row0 = range.start;
        ধ্রুব পূর্ণাঙ্ক row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        পূর্ণাঙ্ক cn = src.channels();
        পূর্ণাঙ্ক lutcn = lut_.channels();

        ধ্রুব Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        পূর্ণাঙ্ক len = (পূর্ণাঙ্ক)it.size;

        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
গুপ্ত:
    LUTParallelBody(ধ্রুব LUTParallelBody&);
    LUTParallelBody& operator=(ধ্রুব LUTParallelBody&);
};

} // cv::

শূন্য cv::LUT( প্রবেশ_Array _src, প্রবেশ_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক cn = _src.channels(), depth = _src.depth();
    পূর্ণাঙ্ক lutcn = _lut.channels();
    ধ্রুব size_t lut_size = _lut.total();

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

    যদি(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    অন্যথা
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    ধ্রুব LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    যদি (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        যদি (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (দ্বিগুণ)std::max((size_t)1, dst.total()>>16));
        অন্যথা
            body(all);

        প্রত্যাগম;
    }

    ধ্রুব Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক len = (পূর্ণাঙ্ক)it.size;

    প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
