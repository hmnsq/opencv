// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencv2/core/mat.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

வெறுமை cv::swap( Mat& a, Mat& b )
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


வெறுமை cv::hconcat(மாறிலி Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    எனில்( nsrc == 0 || !src )
    {
        _dst.release();
        திரும்பு;
    }

    முழு totalCols = 0, cols = 0;
    க்கு( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    க்கு( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

வெறுமை cv::hconcat(உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

வெறுமை cv::hconcat(உள்ளீடு_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

வெறுமை cv::vconcat(மாறிலி Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    எனில்( nsrc == 0 || !src )
    {
        _dst.release();
        திரும்பு;
    }

    முழு totalRows = 0, rows = 0;
    க்கு( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    க்கு( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

வெறுமை cv::vconcat(உள்ளீடு_Array src1, உள்ளீடு_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

வெறுமை cv::vconcat(உள்ளீடு_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

நாமாகாஶ cv {

நிலையான தருக்க ocl_setIdentity( உள்ளீடு_Output_Array _m, மாறிலி Scalar& s )
{
    முழு type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    முழு sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    எனில் (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        எனில் (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            எனில் (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, nullptr, பொய்);
}

}

#endif

வெறுமை cv::setIdentity( உள்ளீடு_Output_Array _m, மாறிலி Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    முழு rows = m.rows, cols = m.cols, type = m.type();

    எனில்( type == CV_32FC1 )
    {
        தசம* data = m.ptr<தசம>();
        தசம val = (தசம)s[0];
        size_t step = m.step/அளவு(data[0]);

        க்கு( முழு i = 0; i < rows; i++, data += step )
        {
            க்கு( முழு j = 0; j < cols; j++ )
                data[j] = 0;
            எனில்( i < cols )
                data[i] = val;
        }
    }
    இல்லையெனில் எனில்( type == CV_64FC1 )
    {
        இரட்டை* data = m.ptr<இரட்டை>();
        இரட்டை val = s[0];
        size_t step = m.step/அளவு(data[0]);

        க்கு( முழு i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            எனில் (i < cols)
                data[i] = val;
        }
    }
    இல்லையெனில்
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


நாமாகாஶ cv {

UMat UMat::eye(முழு rows, முழு cols, முழு type, UMatUsageFlags usageFlags)
{
    திரும்பு UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, முழு type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    திரும்பு m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( உள்ளீடு_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    முழு type = m.type();
    முழு nm = std::min(m.rows, m.cols);

    எனில்( type == CV_32FC1 )
    {
        மாறிலி தசம* ptr = m.ptr<தசம>();
        size_t step = m.step/அளவு(ptr[0]) + 1;
        இரட்டை _s = 0;
        க்கு( முழு i = 0; i < nm; i++ )
            _s += ptr[i*step];
        திரும்பு _s;
    }

    எனில்( type == CV_64FC1 )
    {
        மாறிலி இரட்டை* ptr = m.ptr<இரட்டை>();
        size_t step = m.step/அளவு(ptr[0]) + 1;
        இரட்டை _s = 0;
        க்கு( முழு i = 0; i < nm; i++ )
            _s += ptr[i*step];
        திரும்பு _s;
    }

    திரும்பு cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

வெறுமை cv::completeSymm( உள்ளீடு_Output_Array _m, தருக்க LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    முழு rows = m.rows;
    முழு j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    க்கு( முழு i = 0; i < rows; i++ )
    {
        எனில்( !LtoR ) j1 = i; இல்லையெனில் j0 = i+1;
        க்கு( முழு j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(உள்ளீடு_Array _m) மாறிலி
{
    Mat m = _m.getMat();
    முழு tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    எனில்( d == CV_32F )
    {
        மாறிலி தசம *a = (மாறிலி தசம*)data, *b = (மாறிலி தசம*)m.data;
        தசம* c = (தசம*)result.data;
        size_t lda = rows > 1 ? step/அளவு(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/அளவு(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    இல்லையெனில் எனில்( d == CV_64F )
    {
        மாறிலி இரட்டை *a = (மாறிலி இரட்டை*)data, *b = (மாறிலி இரட்டை*)m.data;
        இரட்டை* c = (இரட்டை*)result.data;
        size_t lda = rows > 1 ? step/அளவு(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/அளவு(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    திரும்பு result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

நாமாகாஶ cv
{

வகைவரை வெறுமை (*ReduceSumFunc)(மாறிலி Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(முழு sdepth, முழு ddepth);
ReduceSumFunc getReduceRSumFunc(முழு sdepth, முழு ddepth);
ReduceSumFunc getReduceRSum2Func(முழு sdepth, முழு ddepth);

template <typename T, typename WT, typename Op>
கட்டமைப்பு ReduceR_SIMD
{
    முழு operator()(மாறிலி T*, முழு start, முழு, WT*, மாறிலி Op&) மாறிலி
    {
        திரும்பு start;
    }
};


template<typename T, typename ST, typename WT, வகுப்பு Op, வகுப்பு OpInit>
வகுப்பு ReduceR_Invoker : பொது ParallelLoopBody
{
பொது:
  ReduceR_Invoker(மாறிலி Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  வெறுமை operator()(மாறிலி Range& range) மாறிலி CV_OVERRIDE
  {
    மாறிலி T* src = srcmat.ptr<T>();
    மாறிலி size_t srcstep = srcmat.step/அளவு(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    முழு i = 0;

    க்கு( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    முழு height = srcmat.size().height;
    க்கு( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #எனில் CV_ENABLE_UNROLLED
        க்கு(; i <= range.end - 4; i += 4 )
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
        க்கு( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    க்கு( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
தனிப்பட்ட:
  மாறிலி Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, வகுப்பு Op, வகுப்பு OpInit = OpNop<ST> > நிலையான வெறுமை
reduceR_( மாறிலி Mat& srcmat, Mat& dstmat)
{
    வகைவரை typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, வகுப்பு Op, வகுப்பு OpInit>
வகுப்பு ReduceC_Invoker : பொது ParallelLoopBody
{
பொது:
  ReduceC_Invoker(மாறிலி Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  வெறுமை operator()(மாறிலி Range& range) மாறிலி CV_OVERRIDE
  {
    மாறிலி முழு cn = srcmat.channels();
    மாறிலி முழு width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    க்கு( முழு y = range.start; y < range.end; y++ )
    {
        மாறிலி T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        எனில்( width == cn )
        {
          க்கு( முழு k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        இல்லையெனில்
        {
            க்கு(முழு k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            க்கு(முழு k = cn ; k < width ; k += cn )
            {
                க்கு (முழு c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            க்கு(முழு k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
தனிப்பட்ட:
  மாறிலி Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, வகுப்பு Op, வகுப்பு OpInit = OpNop<ST> > நிலையான வெறுமை
reduceC_( மாறிலி Mat& srcmat, Mat& dstmat)
{
    வகைவரை typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

வகைவரை வெறுமை (*ReduceFunc)( மாறிலி Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, முழு,   OpAdd<முழு>, OpNop<முழு> >
#define reduceSumR8u32f  reduceR_<uchar, தசம, OpAdd<முழு>, OpNop<முழு> >
#define reduceSumR8u64f  reduceR_<uchar, இரட்டை,OpAdd<முழு>, OpNop<முழு> >
#define reduceSumR16u32f reduceR_<ushort,தசம, OpAdd<தசம> >
#define reduceSumR16u64f reduceR_<ushort,இரட்டை,OpAdd<இரட்டை> >
#define reduceSumR16s32f reduceR_<short, தசம, OpAdd<தசம> >
#define reduceSumR16s64f reduceR_<short, இரட்டை,OpAdd<இரட்டை> >
#define reduceSumR32f32f reduceR_<தசம, தசம, OpAdd<தசம> >
#define reduceSumR32f64f reduceR_<தசம, இரட்டை,OpAdd<இரட்டை> >
#define reduceSumR64f64f reduceR_<இரட்டை,இரட்டை,OpAdd<இரட்டை> >

#define reduceSum2R8u32s  reduceR_<uchar, முழு,   OpAddSqr<முழு>,   OpSqr<முழு> >
#define reduceSum2R8u32f  reduceR_<uchar, தசம, OpAddSqr<முழு>,   OpSqr<முழு> >
#define reduceSum2R8u64f  reduceR_<uchar, இரட்டை,OpAddSqr<முழு>,   OpSqr<முழு> >
#define reduceSum2R16u32f reduceR_<ushort,தசம, OpAddSqr<தசம>, OpSqr<தசம> >
#define reduceSum2R16u64f reduceR_<ushort,இரட்டை,OpAddSqr<இரட்டை>,OpSqr<இரட்டை> >
#define reduceSum2R16s32f reduceR_<short, தசம, OpAddSqr<தசம>, OpSqr<தசம> >
#define reduceSum2R16s64f reduceR_<short, இரட்டை,OpAddSqr<இரட்டை>,OpSqr<இரட்டை> >
#define reduceSum2R32f32f reduceR_<தசம, தசம, OpAddSqr<தசம>, OpSqr<தசம> >
#define reduceSum2R32f64f reduceR_<தசம, இரட்டை,OpAddSqr<இரட்டை>,OpSqr<இரட்டை> >
#define reduceSum2R64f64f reduceR_<இரட்டை,இரட்டை,OpAddSqr<இரட்டை>,OpSqr<இரட்டை> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<தசம, தசம, OpMax<தசம> >
#define reduceMaxR64f reduceR_<இரட்டை,இரட்டை,OpMax<இரட்டை> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<தசம, தசம, OpMin<தசம> >
#define reduceMinR64f reduceR_<இரட்டை,இரட்டை,OpMin<இரட்டை> >

#எனில் defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
நிலையான உள்வரி தருக்க ipp_reduceSumC_8u16u16s32f_64f(மாறிலி cv::Mat& srcmat, cv::Mat& dstmat)
{
    முழு sstep = (முழு)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    வகைவரை IppStatus (CV_STDCALL * IppiSum)(மாறிலி வெறுமை * pSrc, முழு srcStep, IppiSize roiSize, Ipp64f* pSum);
    வகைவரை IppStatus (CV_STDCALL * IppiSumHint)(மாறிலி வெறுமை * pSrc, முழு srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    எனில்(ddepth == CV_64F)
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

    எனில்(ippiSum)
    {
        க்கு(முழு y = 0; y < srcmat.size().height; y++)
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                திரும்பு பொய்;
        }
        திரும்பு உண்மை;
    }
    இல்லையெனில் எனில்(ippiSumHint)
    {
        க்கு(முழு y = 0; y < srcmat.size().height; y++)
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                திரும்பு பொய்;
        }
        திரும்பு உண்மை;
    }

    திரும்பு பொய்;
}

நிலையான உள்வரி வெறுமை reduceSumC_8u16u16s32f_64f(மாறிலி cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    எனில்(dstmat.depth() == CV_64F)
    {
        முழு sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, இரட்டை,   cv::OpAdd<இரட்டை> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, இரட்டை,   cv::OpAdd<இரட்டை> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, இரட்டை,   cv::OpAdd<இரட்டை> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<தசம, இரட்டை,   cv::OpAdd<இரட்டை> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, முழு,   OpAdd<முழு>, OpNop<முழு> >
#define reduceSumC8u32f  reduceC_<uchar, தசம, OpAdd<முழு>, OpNop<முழு> >
#define reduceSumC16u32f reduceC_<ushort,தசம, OpAdd<தசம> >
#define reduceSumC16s32f reduceC_<short, தசம, OpAdd<தசம> >
#define reduceSumC32f32f reduceC_<தசம, தசம, OpAdd<தசம> >
#define reduceSumC64f64f reduceC_<இரட்டை,இரட்டை,OpAdd<இரட்டை> >

#define reduceSum2C8u32s  reduceC_<uchar, முழு,   OpAddSqr<முழு>,   OpSqr<முழு> >
#define reduceSum2C8u32f  reduceC_<uchar, தசம, OpAddSqr<முழு>,   OpSqr<முழு> >
#define reduceSum2C16u32f reduceC_<ushort,தசம, OpAddSqr<தசம>, OpSqr<தசம> >
#define reduceSum2C16s32f reduceC_<short, தசம, OpAddSqr<தசம>, OpSqr<தசம> >
#define reduceSum2C32f32f reduceC_<தசம, தசம, OpAddSqr<தசம>, OpSqr<தசம> >
#define reduceSum2C64f64f reduceC_<இரட்டை,இரட்டை,OpAddSqr<இரட்டை>,OpSqr<இரட்டை> >

#எனில் defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#இல்லையெனில்
#define reduceSumC8u64f  reduceC_<uchar, இரட்டை,OpAdd<முழு>, OpNop<முழு> >
#define reduceSumC16u64f reduceC_<ushort,இரட்டை,OpAdd<இரட்டை> >
#define reduceSumC16s64f reduceC_<short, இரட்டை,OpAdd<இரட்டை> >
#define reduceSumC32f64f reduceC_<தசம, இரட்டை,OpAdd<இரட்டை> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, இரட்டை,OpAddSqr<முழு>,   OpSqr<முழு> >
#define reduceSum2C16u64f reduceC_<ushort,இரட்டை,OpAddSqr<இரட்டை>,OpSqr<இரட்டை> >
#define reduceSum2C16s64f reduceC_<short, இரட்டை,OpAddSqr<இரட்டை>,OpSqr<இரட்டை> >
#define reduceSum2C32f64f reduceC_<தசம, இரட்டை,OpAddSqr<இரட்டை>,OpSqr<இரட்டை> >

#எனில் defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
நிலையான உள்வரி தருக்க ipp_reduce##optype##C##favor(மாறிலி cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    எனில்((srcmat.channels() == 1)) \
    { \
        முழு sstep = (முழு)srcmat.step; \
        வகைவரை Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        க்கு(முழு y = 0; y < srcmat.size().height; y++)\
        {\
            எனில்(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                திரும்பு பொய்;\
        }\
        திரும்பு உண்மை;\
    }\
    திரும்பு பொய்; \
} \
நிலையான உள்வரி வெறுமை reduce##optype##C##favor(மாறிலி cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#எனில் defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, தசம, தசம)
#இல்லையெனில்
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<தசம, தசம, OpMax<தசம> >
#endif
#define reduceMaxC64f reduceC_<இரட்டை,இரட்டை,OpMax<இரட்டை> >

#எனில் defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, தசம, தசம)
#இல்லையெனில்
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<தசம, தசம, OpMin<தசம> >
#endif
#define reduceMinC64f reduceC_<இரட்டை,இரட்டை,OpMin<இரட்டை> >

#ifdef HAVE_OPENCL

நாமாகாஶ cv {

நிலையான தருக்க ocl_reduce(உள்ளீடு_Array _src, OutputArray _dst,
                       முழு dim, முழு op, முழு op0, முழு stype, முழு dtype)
{
    மாறிலி முழு min_opt_cols = 128, buf_cols = 32;
    முழு sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    மாறிலி ocl::Device &defDev = ocl::Device::getDefault();
    தருக்க doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    தருக்க useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    எனில் (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        திரும்பு பொய்;

    எனில் (op == REDUCE_AVG)
    {
        எனில் (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    மாறிலி எழுத்து * மாறிலி ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    முழு wdepth = std::max(ddepth, CV_32F);
    எனில் (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        எனில் (defDev.isIntel())
        {
            நிலையான மாறிலி size_t max_Item_In_Group_மொத்தம் = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_மொத்தம்);
        }
        எழுத்து cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], அளவு(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], அளவு(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], அளவு(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        எனில் (k.empty())
            திரும்பு பொய்;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        எனில் (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        இல்லையெனில்
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        திரும்பு k.run(2, globalSize, localSize, பொய்);
    }
    இல்லையெனில்
    {
        எழுத்து cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], அளவு(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], அளவு(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], அளவு(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        எனில் (k.empty())
            திரும்பு பொய்;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        எனில் (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        இல்லையெனில்
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        திரும்பு k.run(1, &globalsize, வெற்று, பொய்);
    }
}

}

#endif

வெறுமை cv::reduce(உள்ளீடு_Array _src, OutputArray _dst, முழு dim, முழு op, முழு dtype)
{
    CV_INSTRUMENT_REGION();

    எனில் (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, உண்மை, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        திரும்பு;
    }
    முழு op0 = op;
    முழு stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    எனில்( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    முழு ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    எனில் (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    எனில் (src.empty()) {
        src.convertTo(_dst, dtype);
        திரும்பு;
    }
    எனில் (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    எனில்( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        எனில்( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    எனில்( dim == 0 )
    {
        எனில்( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            எனில்(simd_func)
                func = (ReduceFunc)simd_func;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            இல்லையெனில் எனில்(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        இல்லையெனில் எனில்(op == REDUCE_MAX)
        {
            எனில்(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            இல்லையெனில் எனில்(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        இல்லையெனில் எனில்(op == REDUCE_MIN)
        {
            எனில்(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            இல்லையெனில் எனில்(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        இல்லையெனில் எனில்( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            எனில்(simd_func)
                func = simd_func;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            இல்லையெனில் எனில்(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    இல்லையெனில்
    {
        எனில்(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            எனில்(simd_func)
                func = (ReduceFunc)simd_func;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            இல்லையெனில் எனில்(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        இல்லையெனில் எனில்(op == REDUCE_MAX)
        {
            எனில்(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            இல்லையெனில் எனில்(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        இல்லையெனில் எனில்(op == REDUCE_MIN)
        {
            எனில்(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            இல்லையெனில் எனில்(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        இல்லையெனில் எனில்(op == REDUCE_SUM2)
        {
            எனில்(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            இல்லையெனில் எனில்(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            இல்லையெனில் எனில்(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            இல்லையெனில் எனில்(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            இல்லையெனில் எனில்(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            இல்லையெனில் எனில்(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    எனில்( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    எனில்( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

நாமாகாஶ cv
{

template<typename T> நிலையான வெறுமை sort_( மாறிலி Mat& src, Mat& dst, முழு flags )
{
    முழு n, len;
    தருக்க sortRows = (flags & 1) == SORT_EVERY_ROW;
    தருக்க inplace = src.data == dst.data;
    தருக்க sortDescending = (flags & SORT_DESCENDING) != 0;

    எனில்( sortRows )
        n = src.rows, len = src.cols;
    இல்லையெனில்
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](மாறிலி Range& range)
    {
        AutoBuffer<T> buf;
        எனில்( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        க்கு( முழு i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            எனில்( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                எனில்( !inplace )
                {
                    மாறிலி T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, அளவு(T) * len);
                }
                ptr = dptr;
            }
            இல்லையெனில்
            {
                க்கு( முழு j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            எனில்( sortDescending )
            {
                க்கு( முழு j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            எனில்( !sortRows )
                க்கு( முழு j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#எனில் defined(HAVE_IPP) && !IPP_DISABLE_SORT
வகைவரை IppStatus (CV_STDCALL *IppSortFunc)(வெறுமை  *pSrcDst, முழு    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, முழு, Ipp8u*)>
நிலையான IppStatus CV_STDCALL ippSortWrap(வெறுமை* pSrcDst, முழு len, Ipp8u* pBuffer)
{
    திரும்பு fn((T*)pSrcDst, len, pBuffer);
}

நிலையான IppSortFunc getSortFunc(முழு depth, தருக்க sortDescending)
{
    எனில் (!sortDescending)
        திரும்பு depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    இல்லையெனில்
        திரும்பு depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

நிலையான தருக்க ipp_sort(மாறிலி Mat& src, Mat& dst, முழு flags)
{
    CV_INSTRUMENT_REGION_IPP();

    தருக்க        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    தருக்க        sortDescending  = (flags & SORT_DESCENDING) != 0;
    தருக்க        inplace         = (src.data == dst.data);
    முழு         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    எனில்(!ippsSortRadix_I)
        திரும்பு பொய்;

    முழு n, len;
    எனில்( sortRows )
        n = src.rows, len = src.cols;
    இல்லையெனில்
        n = src.cols, len = src.rows;

    முழு bufferSize;
    எனில்(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        திரும்பு பொய்;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    நிலையற்ற தருக்க ok = உண்மை;
    parallel_for_(Range(0, n), [&](மாறிலி Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        எனில்( !sortRows )
            row.create(1, src.rows, src.type());

        க்கு( முழு i = range.start; i < range.end; i++ )
        {
            எனில்( !ok )
                நிறுத்து;
            வெறுமை* ptr;
            எனில்( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                எனில்( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            இல்லையெனில்
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            எனில்( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = பொய்;
                நிறுத்து;
            }

            எனில்( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    திரும்பு ok;
}
#endif

template<typename _Tp> வகுப்பு LessThanIdx
{
பொது:
    LessThanIdx( மாறிலி _Tp* _arr ) : arr(_arr) {}
    தருக்க operator()(முழு a, முழு b) மாறிலி { திரும்பு arr[a] < arr[b]; }
    மாறிலி _Tp* arr;
};

template<typename T> நிலையான வெறுமை sortIdx_( மாறிலி Mat& src, Mat& dst, முழு flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<முழு> ibuf;
    தருக்க sortRows = (flags & 1) == SORT_EVERY_ROW;
    தருக்க sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    முழு n, len;
    எனில்( sortRows )
        n = src.rows, len = src.cols;
    இல்லையெனில்
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    முழு* _iptr = ibuf.data();

    க்கு( முழு i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        முழு* iptr = _iptr;

        எனில்( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<முழு>(i);
        }
        இல்லையெனில்
        {
            க்கு( முழு j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        க்கு( முழு j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        எனில்( sortDescending )
        {
            க்கு( முழு j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        எனில்( !sortRows )
            க்கு( முழு j = 0; j < len; j++ )
                dst.ptr<முழு>(j)[i] = iptr[j];
    }
}

#எனில் defined(HAVE_IPP) && !IPP_DISABLE_SORT
வகைவரை IppStatus (CV_STDCALL *IppSortIndexFunc)(மாறிலி வெறுமை*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, முழு len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(மாறிலி T*, Ipp32s, Ipp32s*, முழு, Ipp8u*)>
நிலையான IppStatus CV_STDCALL ippSortIndexWrap(மாறிலி வெறுமை* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, முழு len, Ipp8u *pBuffer)
{
    திரும்பு fn((மாறிலி T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

நிலையான IppSortIndexFunc getSortIndexFunc(முழு depth, தருக்க sortDescending)
{
    எனில் (!sortDescending)
        திரும்பு depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    இல்லையெனில்
        திரும்பு depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

நிலையான தருக்க ipp_sortIdx( மாறிலி Mat& src, Mat& dst, முழு flags )
{
    CV_INSTRUMENT_REGION_IPP();

    தருக்க        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    தருக்க        sortDescending  = (flags & SORT_DESCENDING) != 0;
    முழு         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    எனில்(!ippsSortRadixIndex)
        திரும்பு பொய்;

    எனில்(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        முழு               bufferSize;
        எனில்(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            திரும்பு பொய்;

        buffer.allocate(bufferSize);

        க்கு(முழு i = 0; i < src.rows; i++)
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (மாறிலி வெறுமை*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                திரும்பு பொய்;
        }
    }
    இல்லையெனில்
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        முழு               bufferSize;
        எனில்(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            திரும்பு பொய்;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        க்கு(முழு i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            எனில்(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (மாறிலி வெறுமை*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                திரும்பு பொய்;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    திரும்பு உண்மை;
}
#endif

வகைவரை வெறுமை (*SortFunc)(மாறிலி Mat& src, Mat& dst, முழு flags);
}

வெறுமை cv::sort( உள்ளீடு_Array _src, OutputArray _dst, முழு flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#எனில் !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    நிலையான SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<முழு>, sort_<தசம>, sort_<இரட்டை>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

வெறுமை cv::sortIdx( உள்ளீடு_Array _src, OutputArray _dst, முழு flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    எனில்( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#எனில் !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    நிலையான SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<முழு>, sortIdx_<தசம>, sortIdx_<இரட்டை>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
