// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"
#include "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

void cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.rows, b.rows);
    std::swap(a.cols, b.cols);
    std::swap(a.data, b.data);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.step, b.step);
}


void cv::hconcat(const Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    якщо( nsrc == 0 || !src )
    {
        _dst.release();
        повернути;
    }

    ціле totalCols = 0, cols = 0;
    для( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    для( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

void cv::hconcat(InputArray src1, InputArray src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

void cv::hconcat(InputArray _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

void cv::vconcat(const Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    якщо( nsrc == 0 || !src )
    {
        _dst.release();
        повернути;
    }

    ціле totalRows = 0, rows = 0;
    для( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    для( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

void cv::vconcat(InputArray src1, InputArray src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

void cv::vconcat(InputArray _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

namespace cv {

static bool ocl_setIdentity( InputOutputArray _m, const Scalar& s )
{
    ціле type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    ціле sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    якщо (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        якщо (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            якщо (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    якщо (k.empty())
        повернути false;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    повернути k.run(2, globalsize, nullptr, false);
}

}

#endif

void cv::setIdentity( InputOutputArray _m, const Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    ціле rows = m.rows, cols = m.cols, type = m.type();

    якщо( type == CV_32FC1 )
    {
        float* data = m.ptr<float>();
        float val = (float)s[0];
        size_t step = m.step/sizeof(data[0]);

        для( ціле i = 0; i < rows; i++, data += step )
        {
            для( ціле j = 0; j < cols; j++ )
                data[j] = 0;
            якщо( i < cols )
                data[i] = val;
        }
    }
    інакше якщо( type == CV_64FC1 )
    {
        double* data = m.ptr<double>();
        double val = s[0];
        size_t step = m.step/sizeof(data[0]);

        для( ціле i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            якщо (i < cols)
                data[i] = val;
        }
    }
    інакше
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


namespace cv {

UMat UMat::eye(ціле rows, ціле cols, ціле type, UMatUsageFlags usageFlags)
{
    повернути UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, ціле type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    повернути m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( InputArray _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    ціле type = m.type();
    ціле nm = std::min(m.rows, m.cols);

    якщо( type == CV_32FC1 )
    {
        const float* ptr = m.ptr<float>();
        size_t step = m.step/sizeof(ptr[0]) + 1;
        double _s = 0;
        для( ціле i = 0; i < nm; i++ )
            _s += ptr[i*step];
        повернути _s;
    }

    якщо( type == CV_64FC1 )
    {
        const double* ptr = m.ptr<double>();
        size_t step = m.step/sizeof(ptr[0]) + 1;
        double _s = 0;
        для( ціле i = 0; i < nm; i++ )
            _s += ptr[i*step];
        повернути _s;
    }

    повернути cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

void cv::completeSymm( InputOutputArray _m, bool LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    ціле rows = m.rows;
    ціле j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    для( ціле i = 0; i < rows; i++ )
    {
        якщо( !LtoR ) j1 = i; інакше j0 = i+1;
        для( ціле j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(InputArray _m) const
{
    Mat m = _m.getMat();
    ціле tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    якщо( d == CV_32F )
    {
        const float *a = (const float*)data, *b = (const float*)m.data;
        float* c = (float*)result.data;
        size_t lda = rows > 1 ? step/sizeof(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/sizeof(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    інакше якщо( d == CV_64F )
    {
        const double *a = (const double*)data, *b = (const double*)m.data;
        double* c = (double*)result.data;
        size_t lda = rows > 1 ? step/sizeof(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/sizeof(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    повернути result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

namespace cv
{

typedef void (*ReduceSumFunc)(const Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(ціле sdepth, ціле ddepth);
ReduceSumFunc getReduceRSumFunc(ціле sdepth, ціле ddepth);
ReduceSumFunc getReduceRSum2Func(ціле sdepth, ціле ddepth);

template <typename T, typename WT, typename Op>
struct ReduceR_SIMD
{
    ціле operator()(const T*, ціле start, ціле, WT*, const Op&) const
    {
        повернути start;
    }
};


template<typename T, typename ST, typename WT, class Op, class OpInit>
class ReduceR_Invoker : public ParallelLoopBody
{
public:
  ReduceR_Invoker(const Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  void operator()(const Range& range) const CV_OVERRIDE
  {
    const T* src = srcmat.ptr<T>();
    const size_t srcstep = srcmat.step/sizeof(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    ціле i = 0;

    для( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    ціле height = srcmat.size().height;
    для( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #якщо CV_ENABLE_UNROLLED
        для(; i <= range.end - 4; i += 4 )
        {
            WT s0, s1;
            s0 = op(buf[i], (WT)src[i]);
            s1 = op(buf[i+1], (WT)src[i+1]);
            buf[i] = s0; buf[i+1] = s1;

            s0 = op(buf[i+2], (WT)src[i+2]);
            s1 = op(buf[i+3], (WT)src[i+3]);
            buf[i+2] = s0; buf[i+3] = s1;
        }
        #endif
        для( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    для( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
private:
  const Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, class Op, class OpInit = OpNop<ST> > static void
reduceR_( const Mat& srcmat, Mat& dstmat)
{
    typedef typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, class Op, class OpInit>
class ReduceC_Invoker : public ParallelLoopBody
{
public:
  ReduceC_Invoker(const Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  void operator()(const Range& range) const CV_OVERRIDE
  {
    const ціле cn = srcmat.channels();
    const ціле width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    для( ціле y = range.start; y < range.end; y++ )
    {
        const T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        якщо( width == cn )
        {
          для( ціле k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        інакше
        {
            для(ціле k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            для(ціле k = cn ; k < width ; k += cn )
            {
                для (ціле c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            для(ціле k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
private:
  const Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, class Op, class OpInit = OpNop<ST> > static void
reduceC_( const Mat& srcmat, Mat& dstmat)
{
    typedef typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

typedef void (*ReduceFunc)( const Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, ціле,   OpAdd<ціле>, OpNop<ціле> >
#define reduceSumR8u32f  reduceR_<uchar, float, OpAdd<ціле>, OpNop<ціле> >
#define reduceSumR8u64f  reduceR_<uchar, double,OpAdd<ціле>, OpNop<ціле> >
#define reduceSumR16u32f reduceR_<ushort,float, OpAdd<float> >
#define reduceSumR16u64f reduceR_<ushort,double,OpAdd<double> >
#define reduceSumR16s32f reduceR_<short, float, OpAdd<float> >
#define reduceSumR16s64f reduceR_<short, double,OpAdd<double> >
#define reduceSumR32f32f reduceR_<float, float, OpAdd<float> >
#define reduceSumR32f64f reduceR_<float, double,OpAdd<double> >
#define reduceSumR64f64f reduceR_<double,double,OpAdd<double> >

#define reduceSum2R8u32s  reduceR_<uchar, ціле,   OpAddSqr<ціле>,   OpSqr<ціле> >
#define reduceSum2R8u32f  reduceR_<uchar, float, OpAddSqr<ціле>,   OpSqr<ціле> >
#define reduceSum2R8u64f  reduceR_<uchar, double,OpAddSqr<ціле>,   OpSqr<ціле> >
#define reduceSum2R16u32f reduceR_<ushort,float, OpAddSqr<float>, OpSqr<float> >
#define reduceSum2R16u64f reduceR_<ushort,double,OpAddSqr<double>,OpSqr<double> >
#define reduceSum2R16s32f reduceR_<short, float, OpAddSqr<float>, OpSqr<float> >
#define reduceSum2R16s64f reduceR_<short, double,OpAddSqr<double>,OpSqr<double> >
#define reduceSum2R32f32f reduceR_<float, float, OpAddSqr<float>, OpSqr<float> >
#define reduceSum2R32f64f reduceR_<float, double,OpAddSqr<double>,OpSqr<double> >
#define reduceSum2R64f64f reduceR_<double,double,OpAddSqr<double>,OpSqr<double> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<float, float, OpMax<float> >
#define reduceMaxR64f reduceR_<double,double,OpMax<double> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<float, float, OpMin<float> >
#define reduceMinR64f reduceR_<double,double,OpMin<double> >

#якщо defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
static inline bool ipp_reduceSumC_8u16u16s32f_64f(const cv::Mat& srcmat, cv::Mat& dstmat)
{
    ціле sstep = (ціле)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    typedef IppStatus (CV_STDCALL * IppiSum)(const void * pSrc, ціле srcStep, IppiSize roiSize, Ipp64f* pSum);
    typedef IppStatus (CV_STDCALL * IppiSumHint)(const void * pSrc, ціле srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    якщо(ddepth == CV_64F)
    {
        ippiSum =
            stype == CV_8UC1 ? (IppiSum)ippiSum_8u_C1R :
            stype == CV_8UC3 ? (IppiSum)ippiSum_8u_C3R :
            stype == CV_8UC4 ? (IppiSum)ippiSum_8u_C4R :
            stype == CV_16UC1 ? (IppiSum)ippiSum_16u_C1R :
            stype == CV_16UC3 ? (IppiSum)ippiSum_16u_C3R :
            stype == CV_16UC4 ? (IppiSum)ippiSum_16u_C4R :
            stype == CV_16SC1 ? (IppiSum)ippiSum_16s_C1R :
            stype == CV_16SC3 ? (IppiSum)ippiSum_16s_C3R :
            stype == CV_16SC4 ? (IppiSum)ippiSum_16s_C4R : 0;
        ippiSumHint =
            stype == CV_32FC1 ? (IppiSumHint)ippiSum_32f_C1R :
            stype == CV_32FC3 ? (IppiSumHint)ippiSum_32f_C3R :
            stype == CV_32FC4 ? (IppiSumHint)ippiSum_32f_C4R : 0;
    }

    якщо(ippiSum)
    {
        для(ціле y = 0; y < srcmat.size().height; y++)
        {
            якщо(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                повернути false;
        }
        повернути true;
    }
    інакше якщо(ippiSumHint)
    {
        для(ціле y = 0; y < srcmat.size().height; y++)
        {
            якщо(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                повернути false;
        }
        повернути true;
    }

    повернути false;
}

static inline void reduceSumC_8u16u16s32f_64f(const cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    якщо(dstmat.depth() == CV_64F)
    {
        ціле sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, double,   cv::OpAdd<double> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, double,   cv::OpAdd<double> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, double,   cv::OpAdd<double> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<float, double,   cv::OpAdd<double> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, ціле,   OpAdd<ціле>, OpNop<ціле> >
#define reduceSumC8u32f  reduceC_<uchar, float, OpAdd<ціле>, OpNop<ціле> >
#define reduceSumC16u32f reduceC_<ushort,float, OpAdd<float> >
#define reduceSumC16s32f reduceC_<short, float, OpAdd<float> >
#define reduceSumC32f32f reduceC_<float, float, OpAdd<float> >
#define reduceSumC64f64f reduceC_<double,double,OpAdd<double> >

#define reduceSum2C8u32s  reduceC_<uchar, ціле,   OpAddSqr<ціле>,   OpSqr<ціле> >
#define reduceSum2C8u32f  reduceC_<uchar, float, OpAddSqr<ціле>,   OpSqr<ціле> >
#define reduceSum2C16u32f reduceC_<ushort,float, OpAddSqr<float>, OpSqr<float> >
#define reduceSum2C16s32f reduceC_<short, float, OpAddSqr<float>, OpSqr<float> >
#define reduceSum2C32f32f reduceC_<float, float, OpAddSqr<float>, OpSqr<float> >
#define reduceSum2C64f64f reduceC_<double,double,OpAddSqr<double>,OpSqr<double> >

#якщо defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#інакше
#define reduceSumC8u64f  reduceC_<uchar, double,OpAdd<ціле>, OpNop<ціле> >
#define reduceSumC16u64f reduceC_<ushort,double,OpAdd<double> >
#define reduceSumC16s64f reduceC_<short, double,OpAdd<double> >
#define reduceSumC32f64f reduceC_<float, double,OpAdd<double> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, double,OpAddSqr<ціле>,   OpSqr<ціле> >
#define reduceSum2C16u64f reduceC_<ushort,double,OpAddSqr<double>,OpSqr<double> >
#define reduceSum2C16s64f reduceC_<short, double,OpAddSqr<double>,OpSqr<double> >
#define reduceSum2C32f64f reduceC_<float, double,OpAddSqr<double>,OpSqr<double> >

#якщо defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
static inline bool ipp_reduce##optype##C##favor(const cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    якщо((srcmat.channels() == 1)) \
    { \
        ціле sstep = (ціле)srcmat.step; \
        typedef Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        для(ціле y = 0; y < srcmat.size().height; y++)\
        {\
            якщо(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                повернути false;\
        }\
        повернути true;\
    }\
    повернути false; \
} \
static inline void reduce##optype##C##favor(const cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#якщо defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, float, float)
#інакше
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<float, float, OpMax<float> >
#endif
#define reduceMaxC64f reduceC_<double,double,OpMax<double> >

#якщо defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, float, float)
#інакше
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<float, float, OpMin<float> >
#endif
#define reduceMinC64f reduceC_<double,double,OpMin<double> >

#ifdef HAVE_OPENCL

namespace cv {

static bool ocl_reduce(InputArray _src, OutputArray _dst,
                       ціле dim, ціле op, ціле op0, ціле stype, ціле dtype)
{
    const ціле min_opt_cols = 128, buf_cols = 32;
    ціле sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    const ocl::Device &defDev = ocl::Device::getDefault();
    bool doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    bool useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    якщо (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        повернути false;

    якщо (op == REDUCE_AVG)
    {
        якщо (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    const char * const ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    ціле wdepth = std::max(ddepth, CV_32F);
    якщо (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        якщо (defDev.isIntel())
        {
            static const size_t maxItemInGroupCount = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / maxItemInGroupCount);
        }
        char cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], sizeof(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], sizeof(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], sizeof(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        якщо (k.empty())
            повернути false;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        якщо (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        інакше
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        повернути k.run(2, globalSize, localSize, false);
    }
    інакше
    {
        char cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], sizeof(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], sizeof(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], sizeof(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        якщо (k.empty())
            повернути false;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        якщо (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        інакше
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        повернути k.run(1, &globalsize, NULL, false);
    }
}

}

#endif

void cv::reduce(InputArray _src, OutputArray _dst, ціле dim, ціле op, ціле dtype)
{
    CV_INSTRUMENT_REGION();

    якщо (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, true, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        повернути;
    }
    ціле op0 = op;
    ціле stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    якщо( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    ціле ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    якщо (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    якщо (src.empty()) {
        src.convertTo(_dst, dtype);
        повернути;
    }
    якщо (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    якщо( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        якщо( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    якщо( dim == 0 )
    {
        якщо( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            якщо(simd_func)
                func = (ReduceFunc)simd_func;
            інакше якщо(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            інакше якщо(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            інакше якщо(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            інакше якщо(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            інакше якщо(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            інакше якщо(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            інакше якщо(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            інакше якщо(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            інакше якщо(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            інакше якщо(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        інакше якщо(op == REDUCE_MAX)
        {
            якщо(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            інакше якщо(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            інакше якщо(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            інакше якщо(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            інакше якщо(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        інакше якщо(op == REDUCE_MIN)
        {
            якщо(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            інакше якщо(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            інакше якщо(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            інакше якщо(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            інакше якщо(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        інакше якщо( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            якщо(simd_func)
                func = simd_func;
            інакше якщо(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            інакше якщо(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            інакше якщо(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            інакше якщо(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            інакше якщо(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            інакше якщо(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            інакше якщо(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            інакше якщо(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            інакше якщо(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            інакше якщо(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    інакше
    {
        якщо(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            якщо(simd_func)
                func = (ReduceFunc)simd_func;
            інакше якщо(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            інакше якщо(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            інакше якщо(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            інакше якщо(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            інакше якщо(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            інакше якщо(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            інакше якщо(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            інакше якщо(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            інакше якщо(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            інакше якщо(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        інакше якщо(op == REDUCE_MAX)
        {
            якщо(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            інакше якщо(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            інакше якщо(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            інакше якщо(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            інакше якщо(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        інакше якщо(op == REDUCE_MIN)
        {
            якщо(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            інакше якщо(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            інакше якщо(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            інакше якщо(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            інакше якщо(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        інакше якщо(op == REDUCE_SUM2)
        {
            якщо(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            інакше якщо(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            інакше якщо(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            інакше якщо(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            інакше якщо(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            інакше якщо(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            інакше якщо(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            інакше якщо(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            інакше якщо(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            інакше якщо(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    якщо( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    якщо( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

namespace cv
{

template<typename T> static void sort_( const Mat& src, Mat& dst, ціле flags )
{
    ціле n, len;
    bool sortRows = (flags & 1) == SORT_EVERY_ROW;
    bool inplace = src.data == dst.data;
    bool sortDescending = (flags & SORT_DESCENDING) != 0;

    якщо( sortRows )
        n = src.rows, len = src.cols;
    інакше
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](const Range& range)
    {
        AutoBuffer<T> buf;
        якщо( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        для( ціле i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            якщо( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                якщо( !inplace )
                {
                    const T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, sizeof(T) * len);
                }
                ptr = dptr;
            }
            інакше
            {
                для( ціле j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            якщо( sortDescending )
            {
                для( ціле j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            якщо( !sortRows )
                для( ціле j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#якщо defined(HAVE_IPP) && !IPP_DISABLE_SORT
typedef IppStatus (CV_STDCALL *IppSortFunc)(void  *pSrcDst, ціле    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, ціле, Ipp8u*)>
static IppStatus CV_STDCALL ippSortWrap(void* pSrcDst, ціле len, Ipp8u* pBuffer)
{
    повернути fn((T*)pSrcDst, len, pBuffer);
}

static IppSortFunc getSortFunc(ціле depth, bool sortDescending)
{
    якщо (!sortDescending)
        повернути depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    інакше
        повернути depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

static bool ipp_sort(const Mat& src, Mat& dst, ціле flags)
{
    CV_INSTRUMENT_REGION_IPP();

    bool        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    bool        sortDescending  = (flags & SORT_DESCENDING) != 0;
    bool        inplace         = (src.data == dst.data);
    ціле         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    якщо(!ippsSortRadix_I)
        повернути false;

    ціле n, len;
    якщо( sortRows )
        n = src.rows, len = src.cols;
    інакше
        n = src.cols, len = src.rows;

    ціле bufferSize;
    якщо(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        повернути false;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    volatile bool ok = true;
    parallel_for_(Range(0, n), [&](const Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        якщо( !sortRows )
            row.create(1, src.rows, src.type());

        для( ціле i = range.start; i < range.end; i++ )
        {
            якщо( !ok )
                break;
            void* ptr;
            якщо( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                якщо( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            інакше
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            якщо( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = false;
                break;
            }

            якщо( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    повернути ok;
}
#endif

template<typename _Tp> class LessThanIdx
{
public:
    LessThanIdx( const _Tp* _arr ) : arr(_arr) {}
    bool operator()(ціле a, ціле b) const { повернути arr[a] < arr[b]; }
    const _Tp* arr;
};

template<typename T> static void sortIdx_( const Mat& src, Mat& dst, ціле flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<ціле> ibuf;
    bool sortRows = (flags & 1) == SORT_EVERY_ROW;
    bool sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    ціле n, len;
    якщо( sortRows )
        n = src.rows, len = src.cols;
    інакше
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    ціле* _iptr = ibuf.data();

    для( ціле i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        ціле* iptr = _iptr;

        якщо( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<ціле>(i);
        }
        інакше
        {
            для( ціле j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        для( ціле j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        якщо( sortDescending )
        {
            для( ціле j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        якщо( !sortRows )
            для( ціле j = 0; j < len; j++ )
                dst.ptr<ціле>(j)[i] = iptr[j];
    }
}

#якщо defined(HAVE_IPP) && !IPP_DISABLE_SORT
typedef IppStatus (CV_STDCALL *IppSortIndexFunc)(const void*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, ціле len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(const T*, Ipp32s, Ipp32s*, ціле, Ipp8u*)>
static IppStatus CV_STDCALL ippSortIndexWrap(const void* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, ціле len, Ipp8u *pBuffer)
{
    повернути fn((const T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

static IppSortIndexFunc getSortIndexFunc(ціле depth, bool sortDescending)
{
    якщо (!sortDescending)
        повернути depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    інакше
        повернути depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

static bool ipp_sortIdx( const Mat& src, Mat& dst, ціле flags )
{
    CV_INSTRUMENT_REGION_IPP();

    bool        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    bool        sortDescending  = (flags & SORT_DESCENDING) != 0;
    ціле         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    якщо(!ippsSortRadixIndex)
        повернути false;

    якщо(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        ціле               bufferSize;
        якщо(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            повернути false;

        buffer.allocate(bufferSize);

        для(ціле i = 0; i < src.rows; i++)
        {
            якщо(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (const void*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                повернути false;
        }
    }
    інакше
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        ціле               bufferSize;
        якщо(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            повернути false;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        для(ціле i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            якщо(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (const void*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                повернути false;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    повернути true;
}
#endif

typedef void (*SortFunc)(const Mat& src, Mat& dst, ціле flags);
}

void cv::sort( InputArray _src, OutputArray _dst, ціле flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#якщо !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    static SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<ціле>, sort_<float>, sort_<double>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

void cv::sortIdx( InputArray _src, OutputArray _dst, ціле flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    якщо( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#якщо !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    static SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<ціле>, sortIdx_<float>, sortIdx_<double>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
