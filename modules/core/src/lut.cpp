// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "convert.hpp"
#अन्तर्भाव <sys/types.h>

नामाकाश cv {
शून्य LUT8u_dispatch( स्थिरांक uchar* src, स्थिरांक uchar* lut, uchar* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );
शून्य LUT16u_dispatch( स्थिरांक uchar* src, स्थिरांक ushort* lut, ushort* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

नामाकाश cv
{

ढाँचा<typename Ti, typename T> स्थिर शून्य
LUT_( स्थिरांक शून्य* _src, स्थिरांक शून्य* _lut, शून्य* _dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    स्थिरांक Ti* src = (स्थिरांक Ti*)_src;
    स्थिरांक T* lut = (स्थिरांक T*)_lut;
    T* dst = (T*)_dst;
    जर( lutcn == 1 )
    {
        साठी( पूर्णांक i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    अन्यथा
    {
        साठी( पूर्णांक i = 0; i < len*cn; i += cn )
            साठी( पूर्णांक k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

ढाँचा<typename T, शून्य (*fn)(स्थिरांक uchar*, स्थिरांक T*, T*, पूर्णांक, पूर्णांक, पूर्णांक)>
स्थिर शून्य lutDispatchWrap( स्थिरांक शून्य* src, स्थिरांक शून्य* lut, शून्य* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    fn((स्थिरांक uchar*)src, (स्थिरांक T*)lut, (T*)dst, len, cn, lutcn);
}

प्रकारव्याख्या शून्य (*LUTFunc)( स्थिरांक शून्य* src, स्थिरांक शून्य* lut, शून्य* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );

स्थिर LUTFunc getLUTFunc(स्थिरांक पूर्णांक srcDepth, स्थिरांक पूर्णांक dstDepth)
{
    LUTFunc ret = nullptr;
    जर((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        निवड(dstDepth)
        {
            स्थिती CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   थांबा;
            स्थिती CV_8S:   ret = LUT_<uint8_t, int8_t>;    थांबा;
            स्थिती CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; थांबा;
            स्थिती CV_16S:  ret = LUT_<uint8_t, int16_t>;   थांबा;
            स्थिती CV_32S:  ret = LUT_<uint8_t, int32_t>;   थांबा;
            स्थिती CV_32F:  ret = LUT_<uint8_t, int32_t>;   थांबा; // float
            स्थिती CV_64F:  ret = LUT_<uint8_t, int64_t>;   थांबा; // double
            स्थिती CV_16F:  ret = LUT_<uint8_t, int16_t>;   थांबा; // hfloat
            स्थिती CV_16BF: ret = LUT_<uint8_t, int16_t>;   थांबा; // bfloat
            स्थिती CV_Bool: ret = LUT_<uint8_t, uint8_t>;   थांबा; // bool
            स्थिती CV_64U:  ret = LUT_<uint8_t, uint64_t>;  थांबा;
            स्थिती CV_64S:  ret = LUT_<uint8_t, int64_t>;   थांबा;
            स्थिती CV_32U:  ret = LUT_<uint8_t, uint32_t>;  थांबा;
            पूर्वनिर्धारित:      ret = nullptr;                  थांबा;
        }
    }
    अन्यथा जर((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        निवड(dstDepth)
        {
            स्थिती CV_8U:   ret = LUT_<uint16_t, uint8_t>;  थांबा;
            स्थिती CV_8S:   ret = LUT_<uint16_t, int8_t>;   थांबा;
            स्थिती CV_16U:  ret = LUT_<uint16_t, uint16_t>; थांबा;
            स्थिती CV_16S:  ret = LUT_<uint16_t, int16_t>;  थांबा;
            स्थिती CV_32S:  ret = LUT_<uint16_t, int32_t>;  थांबा;
            स्थिती CV_32F:  ret = LUT_<uint16_t, int32_t>;  थांबा; // float
            स्थिती CV_64F:  ret = LUT_<uint16_t, int64_t>;  थांबा; // double
            स्थिती CV_16F:  ret = LUT_<uint16_t, int16_t>;  थांबा; // hfloat
            स्थिती CV_16BF: ret = LUT_<uint16_t, int16_t>;  थांबा; // bfloat
            स्थिती CV_Bool: ret = LUT_<uint16_t, uint8_t>;  थांबा; // bool
            स्थिती CV_64U:  ret = LUT_<uint16_t, uint64_t>; थांबा;
            स्थिती CV_64S:  ret = LUT_<uint16_t, int64_t>;  थांबा;
            स्थिती CV_32U:  ret = LUT_<uint16_t, uint32_t>; थांबा;
            पूर्वनिर्धारित:      ret = nullptr;                  थांबा;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    परत ret;
}

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_LUT(आदान_Array _src, आदान_Array _lut, OutputArray _dst)
{
    पूर्णांक lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    पूर्णांक kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    जर (k.empty())
        परत असत्य;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    माप_प्रकार globalSize[2] = { (माप_प्रकार)dst.cols * dcn / kercn, ((माप_प्रकार)dst.rows + 3) / 4 };
    परत k.run(2, globalSize, रिक्त, असत्य);
}

#endif

वर्ग LUTParallelBody : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    स्थिरांक Mat& src_;
    स्थिरांक Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(स्थिरांक Mat& src, स्थिरांक Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    शून्य चालक()( स्थिरांक cv::Range& range ) स्थिरांक CV_OVERRIDE
    {
        स्थिरांक पूर्णांक row0 = range.start;
        स्थिरांक पूर्णांक row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        पूर्णांक cn = src.channels();
        पूर्णांक lutcn = lut_.channels();

        स्थिरांक Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        पूर्णांक len = (पूर्णांक)it.size;

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
खाजगी:
    LUTParallelBody(स्थिरांक LUTParallelBody&);
    LUTParallelBody& चालक=(स्थिरांक LUTParallelBody&);
};

} // cv::

शून्य cv::LUT( आदान_Array _src, आदान_Array _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    पूर्णांक cn = _src.channels(), depth = _src.depth();
    पूर्णांक lutcn = _lut.channels();
    स्थिरांक माप_प्रकार lut_size = _lut.total();

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

    जर(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    अन्यथा
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    स्थिरांक LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    जर (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        जर (dst.total() >= (माप_प्रकार)(1<<18))
            parallel_for_(all, body, (द्विगुण)std::max((माप_प्रकार)1, dst.total()>>16));
        अन्यथा
            body(all);

        परत;
    }

    स्थिरांक Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक len = (पूर्णांक)it.size;

    साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
