// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "convert.hpp"
#అన్తర్భావ <sys/types.h>

నామాకాశ cv {
శూన్య LUT8u_dispatch( స్థిరరాశి uchar* src, స్థిరరాశి uchar* lut, uchar* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn );
శూన్య LUT16u_dispatch( స్థిరరాశి uchar* src, స్థిరరాశి ushort* lut, ushort* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

నామాకాశ cv
{

మూస<రకం_పేరు Ti, రకం_పేరు T> స్థిర శూన్య
LUT_( స్థిరరాశి శూన్య* _src, స్థిరరాశి శూన్య* _lut, శూన్య* _dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn )
{
    స్థిరరాశి Ti* src = (స్థిరరాశి Ti*)_src;
    స్థిరరాశి T* lut = (స్థిరరాశి T*)_lut;
    T* dst = (T*)_dst;
    ఒకవేళ( lutcn == 1 )
    {
        కొరకు( పూర్ణాంకం i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    లేకపోతే
    {
        కొరకు( పూర్ణాంకం i = 0; i < len*cn; i += cn )
            కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

మూస<రకం_పేరు T, శూన్య (*fn)(స్థిరరాశి uchar*, స్థిరరాశి T*, T*, పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం)>
స్థిర శూన్య lutDispatchWrap( స్థిరరాశి శూన్య* src, స్థిరరాశి శూన్య* lut, శూన్య* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn )
{
    fn((స్థిరరాశి uchar*)src, (స్థిరరాశి T*)lut, (T*)dst, len, cn, lutcn);
}

రకనిర్వచనం శూన్య (*LUTFunc)( స్థిరరాశి శూన్య* src, స్థిరరాశి శూన్య* lut, శూన్య* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn );

స్థిర LUTFunc getLUTFunc(స్థిరరాశి పూర్ణాంకం src_లోతు, స్థిరరాశి పూర్ణాంకం dst_లోతు)
{
    LUTFunc ret = శూన్యసూచిక;
    ఒకవేళ((src_లోతు == CV_8U) || (src_లోతు == CV_8S))
    {
        ఎంపిక(dst_లోతు)
        {
            స్థితి CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   ఆపు;
            స్థితి CV_8S:   ret = LUT_<uint8_t, int8_t>;    ఆపు;
            స్థితి CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; ఆపు;
            స్థితి CV_16S:  ret = LUT_<uint8_t, int16_t>;   ఆపు;
            స్థితి CV_32S:  ret = LUT_<uint8_t, int32_t>;   ఆపు;
            స్థితి CV_32F:  ret = LUT_<uint8_t, int32_t>;   ఆపు; // float
            స్థితి CV_64F:  ret = LUT_<uint8_t, int64_t>;   ఆపు; // double
            స్థితి CV_16F:  ret = LUT_<uint8_t, int16_t>;   ఆపు; // hfloat
            స్థితి CV_16BF: ret = LUT_<uint8_t, int16_t>;   ఆపు; // bfloat
            స్థితి CV_Bool: ret = LUT_<uint8_t, uint8_t>;   ఆపు; // bool
            స్థితి CV_64U:  ret = LUT_<uint8_t, uint64_t>;  ఆపు;
            స్థితి CV_64S:  ret = LUT_<uint8_t, int64_t>;   ఆపు;
            స్థితి CV_32U:  ret = LUT_<uint8_t, uint32_t>;  ఆపు;
            అప్రమేయ:      ret = శూన్యసూచిక;                  ఆపు;
        }
    }
    లేకపోతే ఒకవేళ((src_లోతు == CV_16U) || (src_లోతు == CV_16S))
    {
        ఎంపిక(dst_లోతు)
        {
            స్థితి CV_8U:   ret = LUT_<uint16_t, uint8_t>;  ఆపు;
            స్థితి CV_8S:   ret = LUT_<uint16_t, int8_t>;   ఆపు;
            స్థితి CV_16U:  ret = LUT_<uint16_t, uint16_t>; ఆపు;
            స్థితి CV_16S:  ret = LUT_<uint16_t, int16_t>;  ఆపు;
            స్థితి CV_32S:  ret = LUT_<uint16_t, int32_t>;  ఆపు;
            స్థితి CV_32F:  ret = LUT_<uint16_t, int32_t>;  ఆపు; // float
            స్థితి CV_64F:  ret = LUT_<uint16_t, int64_t>;  ఆపు; // double
            స్థితి CV_16F:  ret = LUT_<uint16_t, int16_t>;  ఆపు; // hfloat
            స్థితి CV_16BF: ret = LUT_<uint16_t, int16_t>;  ఆపు; // bfloat
            స్థితి CV_Bool: ret = LUT_<uint16_t, uint8_t>;  ఆపు; // bool
            స్థితి CV_64U:  ret = LUT_<uint16_t, uint64_t>; ఆపు;
            స్థితి CV_64S:  ret = LUT_<uint16_t, int64_t>;  ఆపు;
            స్థితి CV_32U:  ret = LUT_<uint16_t, uint32_t>; ఆపు;
            అప్రమేయ:      ret = శూన్యసూచిక;                  ఆపు;
        }
    }

    CV_CheckTrue(ret != శూన్యసూచిక, "An unexpected type combination was specified.");
    తిరిగి ret;
}

#నిర్వచితమైతే HAVE_OPENCL

స్థిర తార్కిక ocl_LUT(ఆదానం_Array _src, ఆదానం_Array _lut, OutputArray _dst)
{
    పూర్ణాంకం lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.లోతు();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_కొలత(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    పూర్ణాంకం kercn = lcn == 1 ? std::min(4, ocl::predict_Optimal_సదిశ_వెడల్పు(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::type_గమ్యం_Str(src.లోతు()), ocl::memop_Type_గమ్యం_Str(ddepth)));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), ocl::KernelArg::Read_Only_No_కొలత(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t global_కొలత[2] = { (size_t)dst.నిలువుల_సంఖ్య * dcn / kercn, ((size_t)dst.అడ్డువరుసలు + 3) / 4 };
    తిరిగి k.నిరంతరం(2, global_కొలత, ఖాళీ, అబద్ధం);
}

#అయితే_ముగింపు

తరగతి LUTParallelBody : బహిరంగ ParallelLoopBody
{
బహిరంగ:
    స్థిరరాశి Mat& src_;
    స్థిరరాశి Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(స్థిరరాశి Mat& src, స్థిరరాశి Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    శూన్య క్రియాకారకం()( స్థిరరాశి cv::Range& range ) స్థిరరాశి CV_OVERRIDE
    {
        స్థిరరాశి పూర్ణాంకం row0 = range.ప్రారంభం;
        స్థిరరాశి పూర్ణాంకం row1 = range.end;

        Mat src = src_.అడ్డువరుస_Range_2(row0, row1);
        Mat dst = dst_.అడ్డువరుస_Range_2(row0, row1);

        పూర్ణాంకం cn = src.channels();
        పూర్ణాంకం lutcn = lut_.channels();

        స్థిరరాశి Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        పూర్ణాంకం len = (పూర్ణాంకం)it.size;

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
అంతరంగ:
    LUTParallelBody(స్థిరరాశి LUTParallelBody&);
    LUTParallelBody& క్రియాకారకం=(స్థిరరాశి LUTParallelBody&);
};

} // cv::

శూన్య cv::LUT( ఆదానం_Array _src, ఆదానం_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం cn = _src.channels(), లోతు = _src.లోతు();
    పూర్ణాంకం lutcn = _lut.channels();
    స్థిరరాశి size_t lut_కొలత = _lut.మొత్తం();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_కొలత == 256) && ((లోతు == CV_8U)||(లోతు == CV_8S))) ||
            ((lut_కొలత == 65536) && ((లోతు == CV_16U)||(లోతు == CV_16S)))
        )
    );

    CV_OCL_నిరంతరం(_dst.isUMat() && _src.dims() <= 2 && (lut_కొలత == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_కొలత(_src, CV_MAKETYPE(_lut.లోతు(), cn));
    Mat dst = _dst.getMat();

    ఒకవేళ(lut_కొలత == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.దత్తాంశం, src.అడుగు, src.type(), lut.దత్తాంశం,
                 lut.elemSize1(), lutcn, dst.దత్తాంశం, dst.అడుగు, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు);
    }
    లేకపోతే
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.అడుగు, src.type(), lut.దత్తాంశం,
                 lut.elemSize1(), lutcn, dst.దత్తాంశం, dst.అడుగు, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు);
    }

    స్థిరరాశి LUTFunc func = getLUTFunc(src.లోతు(), dst.లోతు());
    CV_Assert( func != శూన్యసూచిక );

    ఒకవేళ (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.అడ్డువరుసలు);
        ఒకవేళ (dst.మొత్తం() >= (size_t)(1<<18))
            parallel_for_(all, body, (ద్విగుణ)std::max((size_t)1, dst.మొత్తం()>>16));
        లేకపోతే
            body(all);

        తిరిగి;
    }

    స్థిరరాశి Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం len = (పూర్ణాంకం)it.size;

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
