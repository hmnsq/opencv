// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "convert.hpp"
#ಅನ್ತರ್ಭಾವ <sys/types.h>

ನಾಮಾಕಾಶ cv {
ಶೂನ್ಯ LUT8u_dispatch( ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* lut, uchar* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn );
ಶೂನ್ಯ LUT16u_dispatch( ಧ್ರುವ uchar* src, ಧ್ರುವ ushort* lut, ushort* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{

template<typename Ti, typename T> ಸ್ಥಿರ ಶೂನ್ಯ
LUT_( ಧ್ರುವ ಶೂನ್ಯ* _src, ಧ್ರುವ ಶೂನ್ಯ* _lut, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn )
{
    ಧ್ರುವ Ti* src = (ಧ್ರುವ Ti*)_src;
    ಧ್ರುವ T* lut = (ಧ್ರುವ T*)_lut;
    T* dst = (T*)_dst;
    ಯದಿ( lutcn == 1 )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < len*cn; i += cn )
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, ಶೂನ್ಯ (*fn)(ಧ್ರುವ uchar*, ಧ್ರುವ T*, T*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ lutDispatchWrap( ಧ್ರುವ ಶೂನ್ಯ* src, ಧ್ರುವ ಶೂನ್ಯ* lut, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn )
{
    fn((ಧ್ರುವ uchar*)src, (ಧ್ರುವ T*)lut, (T*)dst, len, cn, lutcn);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*LUTFunc)( ಧ್ರುವ ಶೂನ್ಯ* src, ಧ್ರುವ ಶೂನ್ಯ* lut, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn );

ಸ್ಥಿರ LUTFunc getLUTFunc(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ srcDepth, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dstDepth)
{
    LUTFunc ret = nullptr;
    ಯದಿ((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        ವಿಕಲ್ಪ(dstDepth)
        {
            ಪಕ್ಷ CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   ಭಙ್ಗ;
            ಪಕ್ಷ CV_8S:   ret = LUT_<uint8_t, int8_t>;    ಭಙ್ಗ;
            ಪಕ್ಷ CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; ಭಙ್ಗ;
            ಪಕ್ಷ CV_16S:  ret = LUT_<uint8_t, int16_t>;   ಭಙ್ಗ;
            ಪಕ್ಷ CV_32S:  ret = LUT_<uint8_t, int32_t>;   ಭಙ್ಗ;
            ಪಕ್ಷ CV_32F:  ret = LUT_<uint8_t, int32_t>;   ಭಙ್ಗ; // float
            ಪಕ್ಷ CV_64F:  ret = LUT_<uint8_t, int64_t>;   ಭಙ್ಗ; // double
            ಪಕ್ಷ CV_16F:  ret = LUT_<uint8_t, int16_t>;   ಭಙ್ಗ; // hfloat
            ಪಕ್ಷ CV_16BF: ret = LUT_<uint8_t, int16_t>;   ಭಙ್ಗ; // bfloat
            ಪಕ್ಷ CV_Bool: ret = LUT_<uint8_t, uint8_t>;   ಭಙ್ಗ; // bool
            ಪಕ್ಷ CV_64U:  ret = LUT_<uint8_t, uint64_t>;  ಭಙ್ಗ;
            ಪಕ್ಷ CV_64S:  ret = LUT_<uint8_t, int64_t>;   ಭಙ್ಗ;
            ಪಕ್ಷ CV_32U:  ret = LUT_<uint8_t, uint32_t>;  ಭಙ್ಗ;
            ಸ್ವಭಾವ:      ret = nullptr;                  ಭಙ್ಗ;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        ವಿಕಲ್ಪ(dstDepth)
        {
            ಪಕ್ಷ CV_8U:   ret = LUT_<uint16_t, uint8_t>;  ಭಙ್ಗ;
            ಪಕ್ಷ CV_8S:   ret = LUT_<uint16_t, int8_t>;   ಭಙ್ಗ;
            ಪಕ್ಷ CV_16U:  ret = LUT_<uint16_t, uint16_t>; ಭಙ್ಗ;
            ಪಕ್ಷ CV_16S:  ret = LUT_<uint16_t, int16_t>;  ಭಙ್ಗ;
            ಪಕ್ಷ CV_32S:  ret = LUT_<uint16_t, int32_t>;  ಭಙ್ಗ;
            ಪಕ್ಷ CV_32F:  ret = LUT_<uint16_t, int32_t>;  ಭಙ್ಗ; // float
            ಪಕ್ಷ CV_64F:  ret = LUT_<uint16_t, int64_t>;  ಭಙ್ಗ; // double
            ಪಕ್ಷ CV_16F:  ret = LUT_<uint16_t, int16_t>;  ಭಙ್ಗ; // hfloat
            ಪಕ್ಷ CV_16BF: ret = LUT_<uint16_t, int16_t>;  ಭಙ್ಗ; // bfloat
            ಪಕ್ಷ CV_Bool: ret = LUT_<uint16_t, uint8_t>;  ಭಙ್ಗ; // bool
            ಪಕ್ಷ CV_64U:  ret = LUT_<uint16_t, uint64_t>; ಭಙ್ಗ;
            ಪಕ್ಷ CV_64S:  ret = LUT_<uint16_t, int64_t>;  ಭಙ್ಗ;
            ಪಕ್ಷ CV_32U:  ret = LUT_<uint16_t, uint32_t>; ಭಙ್ಗ;
            ಸ್ವಭಾವ:      ret = nullptr;                  ಭಙ್ಗ;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    ಪ್ರತ್ಯಾಗಮ ret;
}

#ifdef HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_LUT(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _lut, OutputArray _dst)
{
    ಪೂರ್ಣಾಙ್ಕ lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    ಪೂರ್ಣಾಙ್ಕ kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalSize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

#endif

ವರ್ಗ LUTParallelBody : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
    ಧ್ರುವ Mat& src_;
    ಧ್ರುವ Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ಧ್ರುವ Mat& src, ಧ್ರುವ Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    ಶೂನ್ಯ operator()( ಧ್ರುವ cv::Range& range ) ಧ್ರುವ CV_OVERRIDE
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ row0 = range.start;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        ಪೂರ್ಣಾಙ್ಕ cn = src.channels();
        ಪೂರ್ಣಾಙ್ಕ lutcn = lut_.channels();

        ಧ್ರುವ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ಪೂರ್ಣಾಙ್ಕ len = (ಪೂರ್ಣಾಙ್ಕ)it.size;

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
ಗುಪ್ತ:
    LUTParallelBody(ಧ್ರುವ LUTParallelBody&);
    LUTParallelBody& operator=(ಧ್ರುವ LUTParallelBody&);
};

} // cv::

ಶೂನ್ಯ cv::LUT( ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ cn = _src.channels(), depth = _src.depth();
    ಪೂರ್ಣಾಙ್ಕ lutcn = _lut.channels();
    ಧ್ರುವ size_t lut_size = _lut.total();

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

    ಯದಿ(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    ಅನ್ಯಥಾ
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    ಧ್ರುವ LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    ಯದಿ (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        ಯದಿ (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (ದ್ವಿಗುಣ)std::max((size_t)1, dst.total()>>16));
        ಅನ್ಯಥಾ
            body(all);

        ಪ್ರತ್ಯಾಗಮ;
    }

    ಧ್ರುವ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ len = (ಪೂರ್ಣಾಙ್ಕ)it.size;

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
