// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencv2/core/mat.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

શૂન્ય cv::swap( Mat& a, Mat& b )
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


શૂન્ય cv::hconcat(અચલ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    જો( nsrc == 0 || !src )
    {
        _dst.release();
        પાછા;
    }

    પૂર્ણાંક totalCols = 0, cols = 0;
    માટે( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    માટે( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

શૂન્ય cv::hconcat(આગત_Array src1, આગત_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

શૂન્ય cv::hconcat(આગત_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

શૂન્ય cv::vconcat(અચલ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    જો( nsrc == 0 || !src )
    {
        _dst.release();
        પાછા;
    }

    પૂર્ણાંક totalRows = 0, rows = 0;
    માટે( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    માટે( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

શૂન્ય cv::vconcat(આગત_Array src1, આગત_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

શૂન્ય cv::vconcat(આગત_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

નામાકાશ cv {

સ્થિર તાર્કિક ocl_setIdentity( આગત_Output_Array _m, અચલ Scalar& s )
{
    પૂર્ણાંક type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    પૂર્ણાંક sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    જો (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        જો (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            જો (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    જો (k.empty())
        પાછા ખોટું;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, nullptr, ખોટું);
}

}

#endif

શૂન્ય cv::setIdentity( આગત_Output_Array _m, અચલ Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    પૂર્ણાંક rows = m.rows, cols = m.cols, type = m.type();

    જો( type == CV_32FC1 )
    {
        દશાંશ* data = m.ptr<દશાંશ>();
        દશાંશ val = (દશાંશ)s[0];
        size_t step = m.step/કદ(data[0]);

        માટે( પૂર્ણાંક i = 0; i < rows; i++, data += step )
        {
            માટે( પૂર્ણાંક j = 0; j < cols; j++ )
                data[j] = 0;
            જો( i < cols )
                data[i] = val;
        }
    }
    નહીંતર જો( type == CV_64FC1 )
    {
        દ્વિગુણ* data = m.ptr<દ્વિગુણ>();
        દ્વિગુણ val = s[0];
        size_t step = m.step/કદ(data[0]);

        માટે( પૂર્ણાંક i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            જો (i < cols)
                data[i] = val;
        }
    }
    નહીંતર
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


નામાકાશ cv {

UMat UMat::eye(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type, UMatUsageFlags usageFlags)
{
    પાછા UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, પૂર્ણાંક type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    પાછા m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( આગત_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    પૂર્ણાંક type = m.type();
    પૂર્ણાંક nm = std::min(m.rows, m.cols);

    જો( type == CV_32FC1 )
    {
        અચલ દશાંશ* ptr = m.ptr<દશાંશ>();
        size_t step = m.step/કદ(ptr[0]) + 1;
        દ્વિગુણ _s = 0;
        માટે( પૂર્ણાંક i = 0; i < nm; i++ )
            _s += ptr[i*step];
        પાછા _s;
    }

    જો( type == CV_64FC1 )
    {
        અચલ દ્વિગુણ* ptr = m.ptr<દ્વિગુણ>();
        size_t step = m.step/કદ(ptr[0]) + 1;
        દ્વિગુણ _s = 0;
        માટે( પૂર્ણાંક i = 0; i < nm; i++ )
            _s += ptr[i*step];
        પાછા _s;
    }

    પાછા cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

શૂન્ય cv::completeSymm( આગત_Output_Array _m, તાર્કિક LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    પૂર્ણાંક rows = m.rows;
    પૂર્ણાંક j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    માટે( પૂર્ણાંક i = 0; i < rows; i++ )
    {
        જો( !LtoR ) j1 = i; નહીંતર j0 = i+1;
        માટે( પૂર્ણાંક j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(આગત_Array _m) અચલ
{
    Mat m = _m.getMat();
    પૂર્ણાંક tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    જો( d == CV_32F )
    {
        અચલ દશાંશ *a = (અચલ દશાંશ*)data, *b = (અચલ દશાંશ*)m.data;
        દશાંશ* c = (દશાંશ*)result.data;
        size_t lda = rows > 1 ? step/કદ(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/કદ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    નહીંતર જો( d == CV_64F )
    {
        અચલ દ્વિગુણ *a = (અચલ દ્વિગુણ*)data, *b = (અચલ દ્વિગુણ*)m.data;
        દ્વિગુણ* c = (દ્વિગુણ*)result.data;
        size_t lda = rows > 1 ? step/કદ(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/કદ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    પાછા result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

નામાકાશ cv
{

પ્રકારવ્યાખ્યા શૂન્ય (*ReduceSumFunc)(અચલ Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);
ReduceSumFunc getReduceRSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);
ReduceSumFunc getReduceRSum2Func(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);

template <typename T, typename WT, typename Op>
સંરચના ReduceR_SIMD
{
    પૂર્ણાંક operator()(અચલ T*, પૂર્ણાંક start, પૂર્ણાંક, WT*, અચલ Op&) અચલ
    {
        પાછા start;
    }
};


template<typename T, typename ST, typename WT, વર્ગ Op, વર્ગ OpInit>
વર્ગ ReduceR_Invoker : જાહેર ParallelLoopBody
{
જાહેર:
  ReduceR_Invoker(અચલ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  શૂન્ય operator()(અચલ Range& range) અચલ CV_OVERRIDE
  {
    અચલ T* src = srcmat.ptr<T>();
    અચલ size_t srcstep = srcmat.step/કદ(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    પૂર્ણાંક i = 0;

    માટે( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    પૂર્ણાંક height = srcmat.size().height;
    માટે( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #જો CV_ENABLE_UNROLLED
        માટે(; i <= range.end - 4; i += 4 )
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
        માટે( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    માટે( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
ખાનગી:
  અચલ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, વર્ગ Op, વર્ગ OpInit = OpNop<ST> > સ્થિર શૂન્ય
reduceR_( અચલ Mat& srcmat, Mat& dstmat)
{
    પ્રકારવ્યાખ્યા typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, વર્ગ Op, વર્ગ OpInit>
વર્ગ ReduceC_Invoker : જાહેર ParallelLoopBody
{
જાહેર:
  ReduceC_Invoker(અચલ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  શૂન્ય operator()(અચલ Range& range) અચલ CV_OVERRIDE
  {
    અચલ પૂર્ણાંક cn = srcmat.channels();
    અચલ પૂર્ણાંક width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    માટે( પૂર્ણાંક y = range.start; y < range.end; y++ )
    {
        અચલ T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        જો( width == cn )
        {
          માટે( પૂર્ણાંક k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        નહીંતર
        {
            માટે(પૂર્ણાંક k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            માટે(પૂર્ણાંક k = cn ; k < width ; k += cn )
            {
                માટે (પૂર્ણાંક c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            માટે(પૂર્ણાંક k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
ખાનગી:
  અચલ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, વર્ગ Op, વર્ગ OpInit = OpNop<ST> > સ્થિર શૂન્ય
reduceC_( અચલ Mat& srcmat, Mat& dstmat)
{
    પ્રકારવ્યાખ્યા typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

પ્રકારવ્યાખ્યા શૂન્ય (*ReduceFunc)( અચલ Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, પૂર્ણાંક,   OpAdd<પૂર્ણાંક>, OpNop<પૂર્ણાંક> >
#define reduceSumR8u32f  reduceR_<uchar, દશાંશ, OpAdd<પૂર્ણાંક>, OpNop<પૂર્ણાંક> >
#define reduceSumR8u64f  reduceR_<uchar, દ્વિગુણ,OpAdd<પૂર્ણાંક>, OpNop<પૂર્ણાંક> >
#define reduceSumR16u32f reduceR_<ushort,દશાંશ, OpAdd<દશાંશ> >
#define reduceSumR16u64f reduceR_<ushort,દ્વિગુણ,OpAdd<દ્વિગુણ> >
#define reduceSumR16s32f reduceR_<short, દશાંશ, OpAdd<દશાંશ> >
#define reduceSumR16s64f reduceR_<short, દ્વિગુણ,OpAdd<દ્વિગુણ> >
#define reduceSumR32f32f reduceR_<દશાંશ, દશાંશ, OpAdd<દશાંશ> >
#define reduceSumR32f64f reduceR_<દશાંશ, દ્વિગુણ,OpAdd<દ્વિગુણ> >
#define reduceSumR64f64f reduceR_<દ્વિગુણ,દ્વિગુણ,OpAdd<દ્વિગુણ> >

#define reduceSum2R8u32s  reduceR_<uchar, પૂર્ણાંક,   OpAddSqr<પૂર્ણાંક>,   OpSqr<પૂર્ણાંક> >
#define reduceSum2R8u32f  reduceR_<uchar, દશાંશ, OpAddSqr<પૂર્ણાંક>,   OpSqr<પૂર્ણાંક> >
#define reduceSum2R8u64f  reduceR_<uchar, દ્વિગુણ,OpAddSqr<પૂર્ણાંક>,   OpSqr<પૂર્ણાંક> >
#define reduceSum2R16u32f reduceR_<ushort,દશાંશ, OpAddSqr<દશાંશ>, OpSqr<દશાંશ> >
#define reduceSum2R16u64f reduceR_<ushort,દ્વિગુણ,OpAddSqr<દ્વિગુણ>,OpSqr<દ્વિગુણ> >
#define reduceSum2R16s32f reduceR_<short, દશાંશ, OpAddSqr<દશાંશ>, OpSqr<દશાંશ> >
#define reduceSum2R16s64f reduceR_<short, દ્વિગુણ,OpAddSqr<દ્વિગુણ>,OpSqr<દ્વિગુણ> >
#define reduceSum2R32f32f reduceR_<દશાંશ, દશાંશ, OpAddSqr<દશાંશ>, OpSqr<દશાંશ> >
#define reduceSum2R32f64f reduceR_<દશાંશ, દ્વિગુણ,OpAddSqr<દ્વિગુણ>,OpSqr<દ્વિગુણ> >
#define reduceSum2R64f64f reduceR_<દ્વિગુણ,દ્વિગુણ,OpAddSqr<દ્વિગુણ>,OpSqr<દ્વિગુણ> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<દશાંશ, દશાંશ, OpMax<દશાંશ> >
#define reduceMaxR64f reduceR_<દ્વિગુણ,દ્વિગુણ,OpMax<દ્વિગુણ> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<દશાંશ, દશાંશ, OpMin<દશાંશ> >
#define reduceMinR64f reduceR_<દ્વિગુણ,દ્વિગુણ,OpMin<દ્વિગુણ> >

#જો defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
સ્થિર અંતઃરેખ તાર્કિક ipp_reduceSumC_8u16u16s32f_64f(અચલ cv::Mat& srcmat, cv::Mat& dstmat)
{
    પૂર્ણાંક sstep = (પૂર્ણાંક)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    પ્રકારવ્યાખ્યા IppStatus (CV_STDCALL * IppiSum)(અચલ શૂન્ય * pSrc, પૂર્ણાંક srcStep, IppiSize roiSize, Ipp64f* pSum);
    પ્રકારવ્યાખ્યા IppStatus (CV_STDCALL * IppiSumHint)(અચલ શૂન્ય * pSrc, પૂર્ણાંક srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    જો(ddepth == CV_64F)
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

    જો(ippiSum)
    {
        માટે(પૂર્ણાંક y = 0; y < srcmat.size().height; y++)
        {
            જો(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                પાછા ખોટું;
        }
        પાછા સાચું;
    }
    નહીંતર જો(ippiSumHint)
    {
        માટે(પૂર્ણાંક y = 0; y < srcmat.size().height; y++)
        {
            જો(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                પાછા ખોટું;
        }
        પાછા સાચું;
    }

    પાછા ખોટું;
}

સ્થિર અંતઃરેખ શૂન્ય reduceSumC_8u16u16s32f_64f(અચલ cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    જો(dstmat.depth() == CV_64F)
    {
        પૂર્ણાંક sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, દ્વિગુણ,   cv::OpAdd<દ્વિગુણ> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, દ્વિગુણ,   cv::OpAdd<દ્વિગુણ> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, દ્વિગુણ,   cv::OpAdd<દ્વિગુણ> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<દશાંશ, દ્વિગુણ,   cv::OpAdd<દ્વિગુણ> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, પૂર્ણાંક,   OpAdd<પૂર્ણાંક>, OpNop<પૂર્ણાંક> >
#define reduceSumC8u32f  reduceC_<uchar, દશાંશ, OpAdd<પૂર્ણાંક>, OpNop<પૂર્ણાંક> >
#define reduceSumC16u32f reduceC_<ushort,દશાંશ, OpAdd<દશાંશ> >
#define reduceSumC16s32f reduceC_<short, દશાંશ, OpAdd<દશાંશ> >
#define reduceSumC32f32f reduceC_<દશાંશ, દશાંશ, OpAdd<દશાંશ> >
#define reduceSumC64f64f reduceC_<દ્વિગુણ,દ્વિગુણ,OpAdd<દ્વિગુણ> >

#define reduceSum2C8u32s  reduceC_<uchar, પૂર્ણાંક,   OpAddSqr<પૂર્ણાંક>,   OpSqr<પૂર્ણાંક> >
#define reduceSum2C8u32f  reduceC_<uchar, દશાંશ, OpAddSqr<પૂર્ણાંક>,   OpSqr<પૂર્ણાંક> >
#define reduceSum2C16u32f reduceC_<ushort,દશાંશ, OpAddSqr<દશાંશ>, OpSqr<દશાંશ> >
#define reduceSum2C16s32f reduceC_<short, દશાંશ, OpAddSqr<દશાંશ>, OpSqr<દશાંશ> >
#define reduceSum2C32f32f reduceC_<દશાંશ, દશાંશ, OpAddSqr<દશાંશ>, OpSqr<દશાંશ> >
#define reduceSum2C64f64f reduceC_<દ્વિગુણ,દ્વિગુણ,OpAddSqr<દ્વિગુણ>,OpSqr<દ્વિગુણ> >

#જો defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#નહીંતર
#define reduceSumC8u64f  reduceC_<uchar, દ્વિગુણ,OpAdd<પૂર્ણાંક>, OpNop<પૂર્ણાંક> >
#define reduceSumC16u64f reduceC_<ushort,દ્વિગુણ,OpAdd<દ્વિગુણ> >
#define reduceSumC16s64f reduceC_<short, દ્વિગુણ,OpAdd<દ્વિગુણ> >
#define reduceSumC32f64f reduceC_<દશાંશ, દ્વિગુણ,OpAdd<દ્વિગુણ> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, દ્વિગુણ,OpAddSqr<પૂર્ણાંક>,   OpSqr<પૂર્ણાંક> >
#define reduceSum2C16u64f reduceC_<ushort,દ્વિગુણ,OpAddSqr<દ્વિગુણ>,OpSqr<દ્વિગુણ> >
#define reduceSum2C16s64f reduceC_<short, દ્વિગુણ,OpAddSqr<દ્વિગુણ>,OpSqr<દ્વિગુણ> >
#define reduceSum2C32f64f reduceC_<દશાંશ, દ્વિગુણ,OpAddSqr<દ્વિગુણ>,OpSqr<દ્વિગુણ> >

#જો defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
સ્થિર અંતઃરેખ તાર્કિક ipp_reduce##optype##C##favor(અચલ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    જો((srcmat.channels() == 1)) \
    { \
        પૂર્ણાંક sstep = (પૂર્ણાંક)srcmat.step; \
        પ્રકારવ્યાખ્યા Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        માટે(પૂર્ણાંક y = 0; y < srcmat.size().height; y++)\
        {\
            જો(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                પાછા ખોટું;\
        }\
        પાછા સાચું;\
    }\
    પાછા ખોટું; \
} \
સ્થિર અંતઃરેખ શૂન્ય reduce##optype##C##favor(અચલ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#જો defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, દશાંશ, દશાંશ)
#નહીંતર
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<દશાંશ, દશાંશ, OpMax<દશાંશ> >
#endif
#define reduceMaxC64f reduceC_<દ્વિગુણ,દ્વિગુણ,OpMax<દ્વિગુણ> >

#જો defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, દશાંશ, દશાંશ)
#નહીંતર
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<દશાંશ, દશાંશ, OpMin<દશાંશ> >
#endif
#define reduceMinC64f reduceC_<દ્વિગુણ,દ્વિગુણ,OpMin<દ્વિગુણ> >

#ifdef HAVE_OPENCL

નામાકાશ cv {

સ્થિર તાર્કિક ocl_reduce(આગત_Array _src, OutputArray _dst,
                       પૂર્ણાંક dim, પૂર્ણાંક op, પૂર્ણાંક op0, પૂર્ણાંક stype, પૂર્ણાંક dtype)
{
    અચલ પૂર્ણાંક min_opt_cols = 128, buf_cols = 32;
    પૂર્ણાંક sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    અચલ ocl::Device &defDev = ocl::Device::getDefault();
    તાર્કિક doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    તાર્કિક useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    જો (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        પાછા ખોટું;

    જો (op == REDUCE_AVG)
    {
        જો (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    અચલ અક્ષર * અચલ ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    પૂર્ણાંક wdepth = std::max(ddepth, CV_32F);
    જો (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        જો (defDev.isIntel())
        {
            સ્થિર અચલ size_t max_Item_In_Group_ગણતરી = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_ગણતરી);
        }
        અક્ષર cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], કદ(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], કદ(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], કદ(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        જો (k.empty())
            પાછા ખોટું;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        જો (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        નહીંતર
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        પાછા k.run(2, globalSize, localSize, ખોટું);
    }
    નહીંતર
    {
        અક્ષર cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], કદ(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], કદ(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], કદ(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        જો (k.empty())
            પાછા ખોટું;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        જો (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        નહીંતર
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        પાછા k.run(1, &globalsize, ખાલી, ખોટું);
    }
}

}

#endif

શૂન્ય cv::reduce(આગત_Array _src, OutputArray _dst, પૂર્ણાંક dim, પૂર્ણાંક op, પૂર્ણાંક dtype)
{
    CV_INSTRUMENT_REGION();

    જો (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, સાચું, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        પાછા;
    }
    પૂર્ણાંક op0 = op;
    પૂર્ણાંક stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    જો( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    પૂર્ણાંક ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    જો (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    જો (src.empty()) {
        src.convertTo(_dst, dtype);
        પાછા;
    }
    જો (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    જો( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        જો( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    જો( dim == 0 )
    {
        જો( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            જો(simd_func)
                func = (ReduceFunc)simd_func;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            નહીંતર જો(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        નહીંતર જો(op == REDUCE_MAX)
        {
            જો(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            નહીંતર જો(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        નહીંતર જો(op == REDUCE_MIN)
        {
            જો(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            નહીંતર જો(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        નહીંતર જો( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            જો(simd_func)
                func = simd_func;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            નહીંતર જો(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    નહીંતર
    {
        જો(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            જો(simd_func)
                func = (ReduceFunc)simd_func;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            નહીંતર જો(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        નહીંતર જો(op == REDUCE_MAX)
        {
            જો(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            નહીંતર જો(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        નહીંતર જો(op == REDUCE_MIN)
        {
            જો(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            નહીંતર જો(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        નહીંતર જો(op == REDUCE_SUM2)
        {
            જો(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            નહીંતર જો(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            નહીંતર જો(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            નહીંતર જો(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            નહીંતર જો(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            નહીંતર જો(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    જો( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    જો( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

નામાકાશ cv
{

template<typename T> સ્થિર શૂન્ય sort_( અચલ Mat& src, Mat& dst, પૂર્ણાંક flags )
{
    પૂર્ણાંક n, len;
    તાર્કિક sortRows = (flags & 1) == SORT_EVERY_ROW;
    તાર્કિક inplace = src.data == dst.data;
    તાર્કિક sortDescending = (flags & SORT_DESCENDING) != 0;

    જો( sortRows )
        n = src.rows, len = src.cols;
    નહીંતર
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](અચલ Range& range)
    {
        AutoBuffer<T> buf;
        જો( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        માટે( પૂર્ણાંક i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            જો( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                જો( !inplace )
                {
                    અચલ T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, કદ(T) * len);
                }
                ptr = dptr;
            }
            નહીંતર
            {
                માટે( પૂર્ણાંક j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            જો( sortDescending )
            {
                માટે( પૂર્ણાંક j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            જો( !sortRows )
                માટે( પૂર્ણાંક j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#જો defined(HAVE_IPP) && !IPP_DISABLE_SORT
પ્રકારવ્યાખ્યા IppStatus (CV_STDCALL *IppSortFunc)(શૂન્ય  *pSrcDst, પૂર્ણાંક    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, પૂર્ણાંક, Ipp8u*)>
સ્થિર IppStatus CV_STDCALL ippSortWrap(શૂન્ય* pSrcDst, પૂર્ણાંક len, Ipp8u* pBuffer)
{
    પાછા fn((T*)pSrcDst, len, pBuffer);
}

સ્થિર IppSortFunc getSortFunc(પૂર્ણાંક depth, તાર્કિક sortDescending)
{
    જો (!sortDescending)
        પાછા depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    નહીંતર
        પાછા depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

સ્થિર તાર્કિક ipp_sort(અચલ Mat& src, Mat& dst, પૂર્ણાંક flags)
{
    CV_INSTRUMENT_REGION_IPP();

    તાર્કિક        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    તાર્કિક        sortDescending  = (flags & SORT_DESCENDING) != 0;
    તાર્કિક        inplace         = (src.data == dst.data);
    પૂર્ણાંક         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    જો(!ippsSortRadix_I)
        પાછા ખોટું;

    પૂર્ણાંક n, len;
    જો( sortRows )
        n = src.rows, len = src.cols;
    નહીંતર
        n = src.cols, len = src.rows;

    પૂર્ણાંક bufferSize;
    જો(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        પાછા ખોટું;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    અસ્થિર તાર્કિક ok = સાચું;
    parallel_for_(Range(0, n), [&](અચલ Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        જો( !sortRows )
            row.create(1, src.rows, src.type());

        માટે( પૂર્ણાંક i = range.start; i < range.end; i++ )
        {
            જો( !ok )
                તોડો;
            શૂન્ય* ptr;
            જો( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                જો( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            નહીંતર
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            જો( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = ખોટું;
                તોડો;
            }

            જો( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    પાછા ok;
}
#endif

template<typename _Tp> વર્ગ LessThanIdx
{
જાહેર:
    LessThanIdx( અચલ _Tp* _arr ) : arr(_arr) {}
    તાર્કિક operator()(પૂર્ણાંક a, પૂર્ણાંક b) અચલ { પાછા arr[a] < arr[b]; }
    અચલ _Tp* arr;
};

template<typename T> સ્થિર શૂન્ય sortIdx_( અચલ Mat& src, Mat& dst, પૂર્ણાંક flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<પૂર્ણાંક> ibuf;
    તાર્કિક sortRows = (flags & 1) == SORT_EVERY_ROW;
    તાર્કિક sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    પૂર્ણાંક n, len;
    જો( sortRows )
        n = src.rows, len = src.cols;
    નહીંતર
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    પૂર્ણાંક* _iptr = ibuf.data();

    માટે( પૂર્ણાંક i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        પૂર્ણાંક* iptr = _iptr;

        જો( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<પૂર્ણાંક>(i);
        }
        નહીંતર
        {
            માટે( પૂર્ણાંક j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        માટે( પૂર્ણાંક j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        જો( sortDescending )
        {
            માટે( પૂર્ણાંક j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        જો( !sortRows )
            માટે( પૂર્ણાંક j = 0; j < len; j++ )
                dst.ptr<પૂર્ણાંક>(j)[i] = iptr[j];
    }
}

#જો defined(HAVE_IPP) && !IPP_DISABLE_SORT
પ્રકારવ્યાખ્યા IppStatus (CV_STDCALL *IppSortIndexFunc)(અચલ શૂન્ય*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, પૂર્ણાંક len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(અચલ T*, Ipp32s, Ipp32s*, પૂર્ણાંક, Ipp8u*)>
સ્થિર IppStatus CV_STDCALL ippSortIndexWrap(અચલ શૂન્ય* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, પૂર્ણાંક len, Ipp8u *pBuffer)
{
    પાછા fn((અચલ T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

સ્થિર IppSortIndexFunc getSortIndexFunc(પૂર્ણાંક depth, તાર્કિક sortDescending)
{
    જો (!sortDescending)
        પાછા depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    નહીંતર
        પાછા depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

સ્થિર તાર્કિક ipp_sortIdx( અચલ Mat& src, Mat& dst, પૂર્ણાંક flags )
{
    CV_INSTRUMENT_REGION_IPP();

    તાર્કિક        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    તાર્કિક        sortDescending  = (flags & SORT_DESCENDING) != 0;
    પૂર્ણાંક         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    જો(!ippsSortRadixIndex)
        પાછા ખોટું;

    જો(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        પૂર્ણાંક               bufferSize;
        જો(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            પાછા ખોટું;

        buffer.allocate(bufferSize);

        માટે(પૂર્ણાંક i = 0; i < src.rows; i++)
        {
            જો(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (અચલ શૂન્ય*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                પાછા ખોટું;
        }
    }
    નહીંતર
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        પૂર્ણાંક               bufferSize;
        જો(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            પાછા ખોટું;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        માટે(પૂર્ણાંક i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            જો(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (અચલ શૂન્ય*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                પાછા ખોટું;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    પાછા સાચું;
}
#endif

પ્રકારવ્યાખ્યા શૂન્ય (*SortFunc)(અચલ Mat& src, Mat& dst, પૂર્ણાંક flags);
}

શૂન્ય cv::sort( આગત_Array _src, OutputArray _dst, પૂર્ણાંક flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#જો !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    સ્થિર SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<પૂર્ણાંક>, sort_<દશાંશ>, sort_<દ્વિગુણ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

શૂન્ય cv::sortIdx( આગત_Array _src, OutputArray _dst, પૂર્ણાંક flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    જો( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#જો !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    સ્થિર SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<પૂર્ણાંક>, sortIdx_<દશાંશ>, sortIdx_<દ્વિગુણ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
