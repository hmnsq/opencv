// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "convert.hpp"
#ಅನ್ತರ್ಭಾವ <sys/types.h>

ನಾಮಾಕಾಶ cv {
ಶೂನ್ಯ LUT8u_dispatch( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* lut, uchar* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn );
ಶೂನ್ಯ LUT16u_dispatch( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ ushort* lut, ushort* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು Ti, ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
LUT_( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _lut, ಶೂನ್ಯ* _dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn )
{
    ಸ್ಥಿರಾಂಕ Ti* src = (ಸ್ಥಿರಾಂಕ Ti*)_src;
    ಸ್ಥಿರಾಂಕ T* lut = (ಸ್ಥಿರಾಂಕ T*)_lut;
    T* dst = (T*)_dst;
    ಆದರೆ( lutcn == 1 )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < len*cn; i += cn )
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T, ಶೂನ್ಯ (*fn)(ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ T*, T*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ)>
ಸ್ಥಿರ ಶೂನ್ಯ lutDispatchWrap( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* lut, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn )
{
    fn((ಸ್ಥಿರಾಂಕ uchar*)src, (ಸ್ಥಿರಾಂಕ T*)lut, (T*)dst, len, cn, lutcn);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*LUTFunc)( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* lut, ಶೂನ್ಯ* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn );

ಸ್ಥಿರ LUTFunc getLUTFunc(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ src_ಆಳ, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dst_ಆಳ)
{
    LUTFunc ret = ಶೂನ್ಯಸೂಚಕ;
    ಆದರೆ((src_ಆಳ == CV_8U) || (src_ಆಳ == CV_8S))
    {
        ಆಯ್ಕೆ(dst_ಆಳ)
        {
            ಸ್ಥಿತಿ CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_8S:   ret = LUT_<uint8_t, int8_t>;    ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_16S:  ret = LUT_<uint8_t, int16_t>;   ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_32S:  ret = LUT_<uint8_t, int32_t>;   ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_32F:  ret = LUT_<uint8_t, int32_t>;   ನಿಲ್ಲಿಸು; // float
            ಸ್ಥಿತಿ CV_64F:  ret = LUT_<uint8_t, int64_t>;   ನಿಲ್ಲಿಸು; // double
            ಸ್ಥಿತಿ CV_16F:  ret = LUT_<uint8_t, int16_t>;   ನಿಲ್ಲಿಸು; // hfloat
            ಸ್ಥಿತಿ CV_16BF: ret = LUT_<uint8_t, int16_t>;   ನಿಲ್ಲಿಸು; // bfloat
            ಸ್ಥಿತಿ CV_Bool: ret = LUT_<uint8_t, uint8_t>;   ನಿಲ್ಲಿಸು; // bool
            ಸ್ಥಿತಿ CV_64U:  ret = LUT_<uint8_t, uint64_t>;  ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_64S:  ret = LUT_<uint8_t, int64_t>;   ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_32U:  ret = LUT_<uint8_t, uint32_t>;  ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:      ret = ಶೂನ್ಯಸೂಚಕ;                  ನಿಲ್ಲಿಸು;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ((src_ಆಳ == CV_16U) || (src_ಆಳ == CV_16S))
    {
        ಆಯ್ಕೆ(dst_ಆಳ)
        {
            ಸ್ಥಿತಿ CV_8U:   ret = LUT_<uint16_t, uint8_t>;  ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_8S:   ret = LUT_<uint16_t, int8_t>;   ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_16U:  ret = LUT_<uint16_t, uint16_t>; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_16S:  ret = LUT_<uint16_t, int16_t>;  ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_32S:  ret = LUT_<uint16_t, int32_t>;  ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_32F:  ret = LUT_<uint16_t, int32_t>;  ನಿಲ್ಲಿಸು; // float
            ಸ್ಥಿತಿ CV_64F:  ret = LUT_<uint16_t, int64_t>;  ನಿಲ್ಲಿಸು; // double
            ಸ್ಥಿತಿ CV_16F:  ret = LUT_<uint16_t, int16_t>;  ನಿಲ್ಲಿಸು; // hfloat
            ಸ್ಥಿತಿ CV_16BF: ret = LUT_<uint16_t, int16_t>;  ನಿಲ್ಲಿಸು; // bfloat
            ಸ್ಥಿತಿ CV_Bool: ret = LUT_<uint16_t, uint8_t>;  ನಿಲ್ಲಿಸು; // bool
            ಸ್ಥಿತಿ CV_64U:  ret = LUT_<uint16_t, uint64_t>; ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_64S:  ret = LUT_<uint16_t, int64_t>;  ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ CV_32U:  ret = LUT_<uint16_t, uint32_t>; ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:      ret = ಶೂನ್ಯಸೂಚಕ;                  ನಿಲ್ಲಿಸು;
        }
    }

    CV_CheckTrue(ret != ಶೂನ್ಯಸೂಚಕ, "An unexpected type combination was specified.");
    ಹಿಂತಿರುಗಿ ret;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_LUT(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _lut, OutputArray _dst)
{
    ಪೂರ್ಣಾಂಕ lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.ಆಳ();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_ಗಾತ್ರ(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    ಪೂರ್ಣಾಂಕ kercn = lcn == 1 ? std::min(4, ocl::predict_Optimal_ಸದಿಶ_ಅಗಲ(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::type_ಗಮ್ಯ_Str(src.ಆಳ()), ocl::memop_Type_ಗಮ್ಯ_Str(ddepth)));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    k.ವಾದಗಳು(ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(src), ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t global_ಗಾತ್ರ[2] = { (size_t)dst.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * dcn / kercn, ((size_t)dst.ಅಡ್ಡಸಾಲುಗಳು + 3) / 4 };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, global_ಗಾತ್ರ, ಖಾಲಿ, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ

ವರ್ಗ LUTParallelBody : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರಾಂಕ Mat& src_;
    ಸ್ಥಿರಾಂಕ Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(ಸ್ಥಿರಾಂಕ Mat& src, ಸ್ಥಿರಾಂಕ Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()( ಸ್ಥಿರಾಂಕ cv::Range& range ) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ row0 = range.ಆರಂಭ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ row1 = range.end;

        Mat src = src_.ಅಡ್ಡಸಾಲು_Range_2(row0, row1);
        Mat dst = dst_.ಅಡ್ಡಸಾಲು_Range_2(row0, row1);

        ಪೂರ್ಣಾಂಕ cn = src.channels();
        ಪೂರ್ಣಾಂಕ lutcn = lut_.channels();

        ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ಪೂರ್ಣಾಂಕ len = (ಪೂರ್ಣಾಂಕ)it.size;

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
ಖಾಸಗಿ:
    LUTParallelBody(ಸ್ಥಿರಾಂಕ LUTParallelBody&);
    LUTParallelBody& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ LUTParallelBody&);
};

} // cv::

ಶೂನ್ಯ cv::LUT( ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ cn = _src.channels(), ಆಳ = _src.ಆಳ();
    ಪೂರ್ಣಾಂಕ lutcn = _lut.channels();
    ಸ್ಥಿರಾಂಕ size_t lut_ಗಾತ್ರ = _lut.ಒಟ್ಟು();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_ಗಾತ್ರ == 256) && ((ಆಳ == CV_8U)||(ಆಳ == CV_8S))) ||
            ((lut_ಗಾತ್ರ == 65536) && ((ಆಳ == CV_16U)||(ಆಳ == CV_16S)))
        )
    );

    CV_OCL_ಸತತ(_dst.isUMat() && _src.dims() <= 2 && (lut_ಗಾತ್ರ == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_ಗಾತ್ರ(_src, CV_MAKETYPE(_lut.ಆಳ(), cn));
    Mat dst = _dst.getMat();

    ಆದರೆ(lut_ಗಾತ್ರ == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.ದತ್ತಾಂಶ, src.ಹೆಜ್ಜೆ, src.type(), lut.ದತ್ತಾಂಶ,
                 lut.elemSize1(), lutcn, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.ಹೆಜ್ಜೆ, src.type(), lut.ದತ್ತಾಂಶ,
                 lut.elemSize1(), lutcn, dst.ದತ್ತಾಂಶ, dst.ಹೆಜ್ಜೆ, src.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, src.ಅಡ್ಡಸಾಲುಗಳು);
    }

    ಸ್ಥಿರಾಂಕ LUTFunc func = getLUTFunc(src.ಆಳ(), dst.ಆಳ());
    CV_Assert( func != ಶೂನ್ಯಸೂಚಕ );

    ಆದರೆ (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.ಅಡ್ಡಸಾಲುಗಳು);
        ಆದರೆ (dst.ಒಟ್ಟು() >= (size_t)(1<<18))
            parallel_for_(all, body, (ದ್ವಿಗುಣ)std::max((size_t)1, dst.ಒಟ್ಟು()>>16));
        ಇಲ್ಲದಿದ್ದರೆ
            body(all);

        ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಂಕ len = (ಪೂರ್ಣಾಂಕ)it.size;

    ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
