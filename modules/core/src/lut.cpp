// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "convert.hpp"
#समावेश <sys/types.h>

namespace cv {
व्योम LUT8u_dispatch( स्थिर uchar* src, स्थिर uchar* lut, uchar* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );
व्योम LUT16u_dispatch( स्थिर uchar* src, स्थिर ushort* lut, ushort* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

namespace cv
{

ढाँचा<typename Ti, typename T> अटल व्योम
LUT_( स्थिर व्योम* _src, स्थिर व्योम* _lut, व्योम* _dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    स्थिर Ti* src = (स्थिर Ti*)_src;
    स्थिर T* lut = (स्थिर T*)_lut;
    T* dst = (T*)_dst;
    अगर( lutcn == 1 )
    {
        क्रम( पूर्णांक i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    अन्यथा
    {
        क्रम( पूर्णांक i = 0; i < len*cn; i += cn )
            क्रम( पूर्णांक k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

ढाँचा<typename T, व्योम (*fn)(स्थिर uchar*, स्थिर T*, T*, पूर्णांक, पूर्णांक, पूर्णांक)>
अटल व्योम lutDispatchWrap( स्थिर व्योम* src, स्थिर व्योम* lut, व्योम* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    fn((स्थिर uchar*)src, (स्थिर T*)lut, (T*)dst, len, cn, lutcn);
}

प्रकार व्योम (*LUTFunc)( स्थिर व्योम* src, स्थिर व्योम* lut, व्योम* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );

अटल LUTFunc getLUTFunc(स्थिर पूर्णांक srcDepth, स्थिर पूर्णांक dstDepth)
{
    LUTFunc ret = nullptr;
    अगर((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        चयन(dstDepth)
        {
            हाल CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   अवरोध;
            हाल CV_8S:   ret = LUT_<uint8_t, int8_t>;    अवरोध;
            हाल CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; अवरोध;
            हाल CV_16S:  ret = LUT_<uint8_t, int16_t>;   अवरोध;
            हाल CV_32S:  ret = LUT_<uint8_t, int32_t>;   अवरोध;
            हाल CV_32F:  ret = LUT_<uint8_t, int32_t>;   अवरोध; // float
            हाल CV_64F:  ret = LUT_<uint8_t, int64_t>;   अवरोध; // double
            हाल CV_16F:  ret = LUT_<uint8_t, int16_t>;   अवरोध; // hfloat
            हाल CV_16BF: ret = LUT_<uint8_t, int16_t>;   अवरोध; // bfloat
            हाल CV_Bool: ret = LUT_<uint8_t, uint8_t>;   अवरोध; // bool
            हाल CV_64U:  ret = LUT_<uint8_t, uint64_t>;  अवरोध;
            हाल CV_64S:  ret = LUT_<uint8_t, int64_t>;   अवरोध;
            हाल CV_32U:  ret = LUT_<uint8_t, uint32_t>;  अवरोध;
            शेष:      ret = nullptr;                  अवरोध;
        }
    }
    अन्यथा अगर((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        चयन(dstDepth)
        {
            हाल CV_8U:   ret = LUT_<uint16_t, uint8_t>;  अवरोध;
            हाल CV_8S:   ret = LUT_<uint16_t, int8_t>;   अवरोध;
            हाल CV_16U:  ret = LUT_<uint16_t, uint16_t>; अवरोध;
            हाल CV_16S:  ret = LUT_<uint16_t, int16_t>;  अवरोध;
            हाल CV_32S:  ret = LUT_<uint16_t, int32_t>;  अवरोध;
            हाल CV_32F:  ret = LUT_<uint16_t, int32_t>;  अवरोध; // float
            हाल CV_64F:  ret = LUT_<uint16_t, int64_t>;  अवरोध; // double
            हाल CV_16F:  ret = LUT_<uint16_t, int16_t>;  अवरोध; // hfloat
            हाल CV_16BF: ret = LUT_<uint16_t, int16_t>;  अवरोध; // bfloat
            हाल CV_Bool: ret = LUT_<uint16_t, uint8_t>;  अवरोध; // bool
            हाल CV_64U:  ret = LUT_<uint16_t, uint64_t>; अवरोध;
            हाल CV_64S:  ret = LUT_<uint16_t, int64_t>;  अवरोध;
            हाल CV_32U:  ret = LUT_<uint16_t, uint32_t>; अवरोध;
            शेष:      ret = nullptr;                  अवरोध;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    वापस ret;
}

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_LUT(निवेश_Array _src, निवेश_Array _lut, OutputArray _dst)
{
    पूर्णांक lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.create_Same_आकार(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    पूर्णांक kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    अगर (k.empty())
        वापस false;

    k.args(ocl::KernelArg::Read_Only_No_आकार(src), ocl::KernelArg::Read_Only_No_आकार(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    माप_प्रकार global_आकार[2] = { (माप_प्रकार)dst.cols * dcn / kercn, ((माप_प्रकार)dst.rows + 3) / 4 };
    वापस k.run(2, global_आकार, शून्य, false);
}

#पूर्ण_अगर

श्रेणी LUTParallelBody : खुला ParallelLoopBody
{
खुला:
    स्थिर Mat& src_;
    स्थिर Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(स्थिर Mat& src, स्थिर Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    व्योम चालक()( स्थिर cv::Range& range ) स्थिर CV_OVERRIDE
    {
        स्थिर पूर्णांक row0 = range.start;
        स्थिर पूर्णांक row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        पूर्णांक cn = src.channels();
        पूर्णांक lutcn = lut_.channels();

        स्थिर Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णांक len = (पूर्णांक)it.size;

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
निजी:
    LUTParallelBody(स्थिर LUTParallelBody&);
    LUTParallelBody& चालक=(स्थिर LUTParallelBody&);
};

} // cv::

व्योम cv::LUT( निवेश_Array _src, निवेश_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णांक cn = _src.channels(), depth = _src.depth();
    पूर्णांक lutcn = _lut.channels();
    स्थिर माप_प्रकार lut_आकार = _lut.योग();

    CV_Assert( (lutcn == cn || lutcn == 1) && _lut.isContinuous() &&
        (
            ((lut_आकार == 256) && ((depth == CV_8U)||(depth == CV_8S))) ||
            ((lut_आकार == 65536) && ((depth == CV_16U)||(depth == CV_16S)))
        )
    );

    CV_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && (lut_आकार == 256),
               ocl_LUT(_src, _lut, _dst))

    Mat src = _src.getMat(), lut = _lut.getMat();
    _dst.create_Same_आकार(_src, CV_MAKETYPE(_lut.depth(), cn));
    Mat dst = _dst.getMat();

    अगर(lut_आकार == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.तथ्य, src.step, src.type(), lut.तथ्य,
                 lut.elemSize1(), lutcn, dst.तथ्य, dst.step, src.cols, src.rows);
    }
    अन्यथा
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.तथ्य,
                 lut.elemSize1(), lutcn, dst.तथ्य, dst.step, src.cols, src.rows);
    }

    स्थिर LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    अगर (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        अगर (dst.योग() >= (माप_प्रकार)(1<<18))
            parallel_for_(all, body, (द्विगुन)std::max((माप_प्रकार)1, dst.योग()>>16));
        अन्यथा
            body(all);

        वापस;
    }

    स्थिर Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक len = (पूर्णांक)it.size;

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
