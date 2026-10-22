// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "convert.hpp"
#অন্তর্ভাব <sys/types.h>

নামাকাশ cv {
শূন্য LUT8u_dispatch( ধ্রুবক uchar* src, ধ্রুবক uchar* lut, uchar* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn );
শূন্য LUT16u_dispatch( ধ্রুবক uchar* src, ধ্রুবক ushort* lut, ushort* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

নামাকাশ cv
{

template<typename Ti, typename T> স্থির শূন্য
LUT_( ধ্রুবক শূন্য* _src, ধ্রুবক শূন্য* _lut, শূন্য* _dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn )
{
    ধ্রুবক Ti* src = (ধ্রুবক Ti*)_src;
    ধ্রুবক T* lut = (ধ্রুবক T*)_lut;
    T* dst = (T*)_dst;
    যদি( lutcn == 1 )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    অন্যথা
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < len*cn; i += cn )
            জন্য( পূর্ণসংখ্যা k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, শূন্য (*fn)(ধ্রুবক uchar*, ধ্রুবক T*, T*, পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা)>
স্থির শূন্য lutDispatchWrap( ধ্রুবক শূন্য* src, ধ্রুবক শূন্য* lut, শূন্য* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn )
{
    fn((ধ্রুবক uchar*)src, (ধ্রুবক T*)lut, (T*)dst, len, cn, lutcn);
}

প্রকারলেখ শূন্য (*LUTFunc)( ধ্রুবক শূন্য* src, ধ্রুবক শূন্য* lut, শূন্য* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn );

স্থির LUTFunc getLUTFunc(ধ্রুবক পূর্ণসংখ্যা srcDepth, ধ্রুবক পূর্ণসংখ্যা dstDepth)
{
    LUTFunc ret = nullptr;
    যদি((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        নির্বাচন(dstDepth)
        {
            ক্ষেত্র CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   বিরতি;
            ক্ষেত্র CV_8S:   ret = LUT_<uint8_t, int8_t>;    বিরতি;
            ক্ষেত্র CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; বিরতি;
            ক্ষেত্র CV_16S:  ret = LUT_<uint8_t, int16_t>;   বিরতি;
            ক্ষেত্র CV_32S:  ret = LUT_<uint8_t, int32_t>;   বিরতি;
            ক্ষেত্র CV_32F:  ret = LUT_<uint8_t, int32_t>;   বিরতি; // float
            ক্ষেত্র CV_64F:  ret = LUT_<uint8_t, int64_t>;   বিরতি; // double
            ক্ষেত্র CV_16F:  ret = LUT_<uint8_t, int16_t>;   বিরতি; // hfloat
            ক্ষেত্র CV_16BF: ret = LUT_<uint8_t, int16_t>;   বিরতি; // bfloat
            ক্ষেত্র CV_Bool: ret = LUT_<uint8_t, uint8_t>;   বিরতি; // bool
            ক্ষেত্র CV_64U:  ret = LUT_<uint8_t, uint64_t>;  বিরতি;
            ক্ষেত্র CV_64S:  ret = LUT_<uint8_t, int64_t>;   বিরতি;
            ক্ষেত্র CV_32U:  ret = LUT_<uint8_t, uint32_t>;  বিরতি;
            পূর্বনির্ধারিত:      ret = nullptr;                  বিরতি;
        }
    }
    অন্যথা যদি((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        নির্বাচন(dstDepth)
        {
            ক্ষেত্র CV_8U:   ret = LUT_<uint16_t, uint8_t>;  বিরতি;
            ক্ষেত্র CV_8S:   ret = LUT_<uint16_t, int8_t>;   বিরতি;
            ক্ষেত্র CV_16U:  ret = LUT_<uint16_t, uint16_t>; বিরতি;
            ক্ষেত্র CV_16S:  ret = LUT_<uint16_t, int16_t>;  বিরতি;
            ক্ষেত্র CV_32S:  ret = LUT_<uint16_t, int32_t>;  বিরতি;
            ক্ষেত্র CV_32F:  ret = LUT_<uint16_t, int32_t>;  বিরতি; // float
            ক্ষেত্র CV_64F:  ret = LUT_<uint16_t, int64_t>;  বিরতি; // double
            ক্ষেত্র CV_16F:  ret = LUT_<uint16_t, int16_t>;  বিরতি; // hfloat
            ক্ষেত্র CV_16BF: ret = LUT_<uint16_t, int16_t>;  বিরতি; // bfloat
            ক্ষেত্র CV_Bool: ret = LUT_<uint16_t, uint8_t>;  বিরতি; // bool
            ক্ষেত্র CV_64U:  ret = LUT_<uint16_t, uint64_t>; বিরতি;
            ক্ষেত্র CV_64S:  ret = LUT_<uint16_t, int64_t>;  বিরতি;
            ক্ষেত্র CV_32U:  ret = LUT_<uint16_t, uint32_t>; বিরতি;
            পূর্বনির্ধারিত:      ret = nullptr;                  বিরতি;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    ফেরত ret;
}

#ifdef HAVE_OPENCL

স্থির যৌক্তিক ocl_LUT(প্রবেশ_Array _src, প্রবেশ_Array _lut, OutputArray _dst)
{
    পূর্ণসংখ্যা lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_আকার(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    পূর্ণসংখ্যা kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    যদি (k.empty())
        ফেরত মিথ্যা;

    k.args(ocl::KernelArg::Read_Only_No_আকার(src), ocl::KernelArg::Read_Only_No_আকার(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t global_আকার[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    ফেরত k.run(2, global_আকার, ফাঁকা, মিথ্যা);
}

#endif

শ্রেণী LUTParallelBody : সর্বজনীন ParallelLoopBody
{
সর্বজনীন:
    ধ্রুবক Mat& src_;
    ধ্রুবক Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ধ্রুবক Mat& src, ধ্রুবক Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    শূন্য operator()( ধ্রুবক cv::Range& range ) ধ্রুবক CV_OVERRIDE
    {
        ধ্রুবক পূর্ণসংখ্যা row0 = range.start;
        ধ্রুবক পূর্ণসংখ্যা row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        পূর্ণসংখ্যা cn = src.channels();
        পূর্ণসংখ্যা lutcn = lut_.channels();

        ধ্রুবক Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        পূর্ণসংখ্যা len = (পূর্ণসংখ্যা)it.size;

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
ব্যক্তিগত:
    LUTParallelBody(ধ্রুবক LUTParallelBody&);
    LUTParallelBody& operator=(ধ্রুবক LUTParallelBody&);
};

} // cv::

শূন্য cv::LUT( প্রবেশ_Array _src, প্রবেশ_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা cn = _src.channels(), depth = _src.depth();
    পূর্ণসংখ্যা lutcn = _lut.channels();
    ধ্রুবক size_t lut_আকার = _lut.যোগ();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_আকার == 256) && ((depth == CV_8U)||(depth == CV_8S))) ||
            ((lut_আকার == 65536) && ((depth == CV_16U)||(depth == CV_16S)))
        )
    );

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (lut_আকার == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_আকার(_src, CV_MAKETYPE(_lut.depth(), cn));
    Mat dst = _dst.getMat();

    যদি(lut_আকার == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.তথ্য, src.step, src.type(), lut.তথ্য,
                 lut.elemSize1(), lutcn, dst.তথ্য, dst.step, src.cols, src.rows);
    }
    অন্যথা
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.তথ্য,
                 lut.elemSize1(), lutcn, dst.তথ্য, dst.step, src.cols, src.rows);
    }

    ধ্রুবক LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    যদি (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        যদি (dst.যোগ() >= (size_t)(1<<18))
            parallel_for_(all, body, (দ্বিগুণ)std::max((size_t)1, dst.যোগ()>>16));
        অন্যথা
            body(all);

        ফেরত;
    }

    ধ্রুবক Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণসংখ্যা len = (পূর্ণসংখ্যা)it.size;

    জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
