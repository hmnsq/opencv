// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "convert.hpp"
#include <sys/types.h>

namespace cv {
void LUT8u_dispatch( const uchar* src, const uchar* lut, uchar* dst, ཧྲིལ len, ཧྲིལ cn, ཧྲིལ lutcn );
void LUT16u_dispatch( const uchar* src, const ushort* lut, ushort* dst, ཧྲིལ len, ཧྲིལ cn, ཧྲིལ lutcn );
} // namespace cv

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

namespace cv
{

template<typename Ti, typename T> static void
LUT_( const void* _src, const void* _lut, void* _dst, ཧྲིལ len, ཧྲིལ cn, ཧྲིལ lutcn )
{
    const Ti* src = (const Ti*)_src;
    const T* lut = (const T*)_lut;
    T* dst = (T*)_dst;
    གལ་སྲིད( lutcn == 1 )
    {
        དོན་ལུ( ཧྲིལ i = 0; i < len*cn; i++ )
            dst[i] = lut[src[i]];
    }
    དེ་མིན
    {
        དོན་ལུ( ཧྲིལ i = 0; i < len*cn; i += cn )
            དོན་ལུ( ཧྲིལ k = 0; k < cn; k++ )
                dst[i+k] = lut[src[i+k]*cn+k];
    }
}

template<typename T, void (*fn)(const uchar*, const T*, T*, ཧྲིལ, ཧྲིལ, ཧྲིལ)>
static void lutDispatchWrap( const void* src, const void* lut, void* dst, ཧྲིལ len, ཧྲིལ cn, ཧྲིལ lutcn )
{
    fn((const uchar*)src, (const T*)lut, (T*)dst, len, cn, lutcn);
}

typedef void (*LUTFunc)( const void* src, const void* lut, void* dst, ཧྲིལ len, ཧྲིལ cn, ཧྲིལ lutcn );

static LUTFunc getLUTFunc(const ཧྲིལ srcDepth, const ཧྲིལ dstDepth)
{
    LUTFunc ret = nullptr;
    གལ་སྲིད((srcDepth == CV_8U) || (srcDepth == CV_8S))
    {
        switch(dstDepth)
        {
            pak_sa CV_8U:   ret = lutDispatchWrap<uchar, LUT8u_dispatch>;   bha_nga;
            pak_sa CV_8S:   ret = LUT_<uint8_t, int8_t>;    bha_nga;
            pak_sa CV_16U:  ret = lutDispatchWrap<ushort, LUT16u_dispatch>; bha_nga;
            pak_sa CV_16S:  ret = LUT_<uint8_t, int16_t>;   bha_nga;
            pak_sa CV_32S:  ret = LUT_<uint8_t, int32_t>;   bha_nga;
            pak_sa CV_32F:  ret = LUT_<uint8_t, int32_t>;   bha_nga; // float
            pak_sa CV_64F:  ret = LUT_<uint8_t, int64_t>;   bha_nga; // double
            pak_sa CV_16F:  ret = LUT_<uint8_t, int16_t>;   bha_nga; // hfloat
            pak_sa CV_16BF: ret = LUT_<uint8_t, int16_t>;   bha_nga; // bfloat
            pak_sa CV_Bool: ret = LUT_<uint8_t, uint8_t>;   bha_nga; // bool
            pak_sa CV_64U:  ret = LUT_<uint8_t, uint64_t>;  bha_nga;
            pak_sa CV_64S:  ret = LUT_<uint8_t, int64_t>;   bha_nga;
            pak_sa CV_32U:  ret = LUT_<uint8_t, uint32_t>;  bha_nga;
            default:      ret = nullptr;                  bha_nga;
        }
    }
    དེ་མིན གལ་སྲིད((srcDepth == CV_16U) || (srcDepth == CV_16S))
    {
        switch(dstDepth)
        {
            pak_sa CV_8U:   ret = LUT_<uint16_t, uint8_t>;  bha_nga;
            pak_sa CV_8S:   ret = LUT_<uint16_t, int8_t>;   bha_nga;
            pak_sa CV_16U:  ret = LUT_<uint16_t, uint16_t>; bha_nga;
            pak_sa CV_16S:  ret = LUT_<uint16_t, int16_t>;  bha_nga;
            pak_sa CV_32S:  ret = LUT_<uint16_t, int32_t>;  bha_nga;
            pak_sa CV_32F:  ret = LUT_<uint16_t, int32_t>;  bha_nga; // float
            pak_sa CV_64F:  ret = LUT_<uint16_t, int64_t>;  bha_nga; // double
            pak_sa CV_16F:  ret = LUT_<uint16_t, int16_t>;  bha_nga; // hfloat
            pak_sa CV_16BF: ret = LUT_<uint16_t, int16_t>;  bha_nga; // bfloat
            pak_sa CV_Bool: ret = LUT_<uint16_t, uint8_t>;  bha_nga; // bool
            pak_sa CV_64U:  ret = LUT_<uint16_t, uint64_t>; bha_nga;
            pak_sa CV_64S:  ret = LUT_<uint16_t, int64_t>;  bha_nga;
            pak_sa CV_32U:  ret = LUT_<uint16_t, uint32_t>; bha_nga;
            default:      ret = nullptr;                  bha_nga;
        }
    }

    CV_CheckTrue(ret != nullptr, "An unexpected type combination was specified.");
    ལོག ret;
}

#ifdef HAVE_OPENCL

static bool ocl_LUT(InputArray _src, InputArray _lut, OutputArray _dst)
{
    ཧྲིལ lcn = _lut.channels(), dcn = _src.channels(), ddepth = _lut.depth();

    UMat src = _src.getUMat(), lut = _lut.getUMat();
    _dst.createSameSize(src, CV_MAKETYPE(ddepth, dcn));
    UMat dst = _dst.getUMat();
    ཧྲིལ kercn = lcn == 1 ? std::min(4, ocl::predictOptimalVectorWidth(_src, _dst)) : dcn;

    ocl::Kernel k("LUT", ocl::core::lut_oclsrc,
                  format("-D dcn=%d -D lcn=%d -D srcT=%s -D dstT=%s", kercn, lcn,
                         ocl::typeToStr(src.depth()), ocl::memopTypeToStr(ddepth)));
    གལ་སྲིད (k.empty())
        ལོག false;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::ReadOnlyNoSize(lut),
        ocl::KernelArg::WriteOnly(dst, dcn, kercn));

    size_t globalSize[2] = { (size_t)dst.cols * dcn / kercn, ((size_t)dst.rows + 3) / 4 };
    ལོག k.run(2, globalSize, NULL, false);
}

#endif

class LUTParallelBody : public ParallelLoopBody
{
public:
    const Mat& src_;
    const Mat& lut_;
    Mat& dst_;

    LUTFunc func_;

    LUTParallelBody(const Mat& src, const Mat& lut, Mat& dst, LUTFunc func)
        : src_(src), lut_(lut), dst_(dst), func_(func)
    {
    }

    void operator()( const cv::Range& range ) const CV_OVERRIDE
    {
        const ཧྲིལ row0 = range.start;
        const ཧྲིལ row1 = range.end;

        Mat src = src_.rowRange(row0, row1);
        Mat dst = dst_.rowRange(row0, row1);

        ཧྲིལ cn = src.channels();
        ཧྲིལ lutcn = lut_.channels();

        const Mat* arrays[] = {&src, &dst, 0};
        uchar* ptrs[2] = {};
        NAryMatIterator it(arrays, ptrs);
        ཧྲིལ len = (ཧྲིལ)it.size;

        དོན་ལུ( size_t i = 0; i < it.nplanes; i++, ++it )
            func_(ptrs[0], lut_.ptr(), ptrs[1], len, cn, lutcn);
    }
private:
    LUTParallelBody(const LUTParallelBody&);
    LUTParallelBody& operator=(const LUTParallelBody&);
};

} // cv::

void cv::LUT( InputArray _src, InputArray _lut, OutputArray _dst )
{
    CV_INSTRUMENT_REGION();

    ཧྲིལ cn = _src.channels(), depth = _src.depth();
    ཧྲིལ lutcn = _lut.channels();
    const size_t lut_size = _lut.total();

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

    གལ་སྲིད(lut_size == 256)
    {
        CALL_HAL(LUT, cv_hal_lut, src.data, src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }
    དེ་མིན
    {
        CALL_HAL(LUT16, cv_hal_lut16, src.ptr<ushort>(), src.step, src.type(), lut.data,
                 lut.elemSize1(), lutcn, dst.data, dst.step, src.cols, src.rows);
    }

    const LUTFunc func = getLUTFunc(src.depth(), dst.depth());
    CV_Assert( func != nullptr );

    གལ་སྲིད (_src.dims() <= 2)
    {
        LUTParallelBody body(src, lut, dst, func);
        Range all(0, dst.rows);
        གལ་སྲིད (dst.total() >= (size_t)(1<<18))
            parallel_for_(all, body, (dviguu_na)std::max((size_t)1, dst.total()>>16));
        དེ་མིན
            body(all);

        ལོག;
    }

    const Mat* arrays[] = {&src, &dst, 0};
    uchar* ptrs[2] = {};
    NAryMatIterator it(arrays, ptrs);
    ཧྲིལ len = (ཧྲིལ)it.size;

    དོན་ལུ( size_t i = 0; i < it.nplanes; i++, ++it )
        func(ptrs[0], lut.ptr(), ptrs[1], len, cn, lutcn);
}
