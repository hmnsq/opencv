// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "convert.hpp"
#ضمن <sys/types.h>

نطاق cv {
فراغ LUT8u_dispatch( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحيح len, صحيح cn, صحيح lutcn );
فراغ LUT16u_dispatch( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحيح len, صحيح cn, صحيح lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

نطاق cv
{

template<typename Ti, typename T> ساكن فراغ
LUT_( ثابت فراغ* _src, ثابت فراغ* _lut, فراغ* _dst, صحيح len, صحيح cn, صحيح lutcn )
{
    ثابت Ti* src = (ثابت Ti*)_src;
    ثابت T* lut = (ثابت T*)_lut;
    T* dst = (T*)_dst;
    𐤀𐤌( lutcn == 1 )
    {
        لكل( صحيح i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    وإلا
    {
        لكل( صحيح i = 0; i < len*cn; i += cn )
            لكل( صحيح k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, فراغ (*fn)(ثابت uchar*, ثابت T*, T*, صحيح, صحيح, صحيح)>
ساكن فراغ lutDispatchWrap( ثابت فراغ* src, ثابت فراغ* lut, فراغ* dst, صحيح len, صحيح cn, صحيح lutcn )
{
    fn((ثابت uchar*)src, (ثابت T*)lut, (T*)dst, len, cn, lutcn);
}

تعريفنوع فراغ (*LUTFunc)( ثابت فراغ* src, ثابت فراغ* lut, فراغ* dst, صحيح len, صحيح cn, صحيح lutcn );

ساكن LUTFunc getLUTFunc(ثابت صحيح srcDepth, ثابت صحيح dstDepth)
{
    LUTFunc ret = nullptr;
    𐤀𐤌((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        بدل(dstDepth)
        {
            حالة CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   اكسر;
            حالة CV_8S:   ret = LUT_<uint8_t, int8_t>;    اكسر;
            حالة CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; اكسر;
            حالة CV_16S:  ret = LUT_<uint8_t, int16_t>;   اكسر;
            حالة CV_32S:  ret = LUT_<uint8_t, int32_t>;   اكسر;
            حالة CV_32F:  ret = LUT_<uint8_t, int32_t>;   اكسر; // float
            حالة CV_64F:  ret = LUT_<uint8_t, int64_t>;   اكسر; // double
            حالة CV_16F:  ret = LUT_<uint8_t, int16_t>;   اكسر; // hfloat
            حالة CV_16BF: ret = LUT_<uint8_t, int16_t>;   اكسر; // bfloat
            حالة CV_Bool: ret = LUT_<uint8_t, uint8_t>;   اكسر; // bool
            حالة CV_64U:  ret = LUT_<uint8_t, uint64_t>;  اكسر;
            حالة CV_64S:  ret = LUT_<uint8_t, int64_t>;   اكسر;
            حالة CV_32U:  ret = LUT_<uint8_t, uint32_t>;  اكسر;
            افتراضي:      ret = nullptr;                  اكسر;
        }
    }
    وإلا 𐤀𐤌((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        بدل(dstDepth)
        {
            حالة CV_8U:   ret = LUT_<uint16_t, uint8_t>;  اكسر;
            حالة CV_8S:   ret = LUT_<uint16_t, int8_t>;   اكسر;
            حالة CV_16U:  ret = LUT_<uint16_t, uint16_t>; اكسر;
            حالة CV_16S:  ret = LUT_<uint16_t, int16_t>;  اكسر;
            حالة CV_32S:  ret = LUT_<uint16_t, int32_t>;  اكسر;
            حالة CV_32F:  ret = LUT_<uint16_t, int32_t>;  اكسر; // float
            حالة CV_64F:  ret = LUT_<uint16_t, int64_t>;  اكسر; // double
            حالة CV_16F:  ret = LUT_<uint16_t, int16_t>;  اكسر; // hfloat
            حالة CV_16BF: ret = LUT_<uint16_t, int16_t>;  اكسر; // bfloat
            حالة CV_Bool: ret = LUT_<uint16_t, uint8_t>;  اكسر; // bool
            حالة CV_64U:  ret = LUT_<uint16_t, uint64_t>; اكسر;
            حالة CV_64S:  ret = LUT_<uint16_t, int64_t>;  اكسر;
            حالة CV_32U:  ret = LUT_<uint16_t, uint32_t>; اكسر;
            افتراضي:      ret = nullptr;                  اكسر;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    أرجع ret;
}

#ifdef HAVE_OPENCL

ساكن منطقي ocl_LUT(InputArray _src, InputArray _lut, OutputArray _dst)
{
    صحيح lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    صحيح kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    𐤀𐤌 (k.empty())
        أرجع خطأ;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    أرجع k.run(2, globalSize, عدم, خطأ);
}

#endif

صنف LUTParallelBody : عام ParallelLoopBody
{
عام:
    ثابت Mat& src_;
    ثابت Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ثابت Mat& src, ثابت Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    فراغ operator()( ثابت cv::Range& range ) ثابت CV_OVERRIDE
    {
        ثابت صحيح row0 = range.start;
        ثابت صحيح row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        صحيح cn = src.channels();
        صحيح lutcn = lut_.channels();

        ثابت Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        صحيح len = (صحيح)it.size;

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
خاص:
    LUTParallelBody(ثابت LUTParallelBody&);
    LUTParallelBody& operator=(ثابت LUTParallelBody&);
};

} // cv::

فراغ cv::LUT( InputArray _src, InputArray _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    صحيح cn = _src.channels(), depth = _src.depth();
    صحيح lutcn = _lut.channels();
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

    𐤀𐤌(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    وإلا
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    ثابت LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    𐤀𐤌 (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        𐤀𐤌 (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (مزدوج)std::max((size_t)1, dst.total()>>16));
        وإلا
            body(all);

        أرجع;
    }

    ثابت Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح len = (صحيح)it.size;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
