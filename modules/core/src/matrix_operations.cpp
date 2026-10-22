// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencv2/core/mat.hpp"
#شامل "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

تهی cv::swap( Mat& a, Mat& b )
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


تهی cv::hconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    اگر( nsrc == 0 || !src )
    {
        _dst.release();
        بازگشت;
    }

    صحیح totalCols = 0, cols = 0;
    برای( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    برای( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

تهی cv::hconcat(درون_داد_Array src1, درون_داد_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

تهی cv::hconcat(درون_داد_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

تهی cv::vconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    اگر( nsrc == 0 || !src )
    {
        _dst.release();
        بازگشت;
    }

    صحیح totalRows = 0, rows = 0;
    برای( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    برای( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

تهی cv::vconcat(درون_داد_Array src1, درون_داد_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

تهی cv::vconcat(درون_داد_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

فضاینام cv {

ایستا منطقی ocl_setIdentity( درون_داد_Output_Array _m, ثابت Scalar& s )
{
    صحیح type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    صحیح sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    اگر (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        اگر (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            اگر (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    اگر (k.empty())
        بازگشت نادرست;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, nullptr, نادرست);
}

}

#endif

تهی cv::setIdentity( درون_داد_Output_Array _m, ثابت Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    صحیح rows = m.rows, cols = m.cols, type = m.type();

    اگر( type == CV_32FC1 )
    {
        اعشاری* data = m.ptr<اعشاری>();
        اعشاری val = (اعشاری)s[0];
        size_t step = m.step/اندازه(data[0]);

        برای( صحیح i = 0; i < rows; i++, data += step )
        {
            برای( صحیح j = 0; j < cols; j++ )
                data[j] = 0;
            اگر( i < cols )
                data[i] = val;
        }
    }
    وگرنه اگر( type == CV_64FC1 )
    {
        دوگانه* data = m.ptr<دوگانه>();
        دوگانه val = s[0];
        size_t step = m.step/اندازه(data[0]);

        برای( صحیح i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            اگر (i < cols)
                data[i] = val;
        }
    }
    وگرنه
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


فضاینام cv {

UMat UMat::eye(صحیح rows, صحیح cols, صحیح type, UMatUsageFlags usageFlags)
{
    بازگشت UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, صحیح type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    بازگشت m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( درون_داد_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    صحیح type = m.type();
    صحیح nm = std::min(m.rows, m.cols);

    اگر( type == CV_32FC1 )
    {
        ثابت اعشاری* ptr = m.ptr<اعشاری>();
        size_t step = m.step/اندازه(ptr[0]) + 1;
        دوگانه _s = 0;
        برای( صحیح i = 0; i < nm; i++ )
            _s += ptr[i*step];
        بازگشت _s;
    }

    اگر( type == CV_64FC1 )
    {
        ثابت دوگانه* ptr = m.ptr<دوگانه>();
        size_t step = m.step/اندازه(ptr[0]) + 1;
        دوگانه _s = 0;
        برای( صحیح i = 0; i < nm; i++ )
            _s += ptr[i*step];
        بازگشت _s;
    }

    بازگشت cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

تهی cv::completeSymm( درون_داد_Output_Array _m, منطقی LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    صحیح rows = m.rows;
    صحیح j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    برای( صحیح i = 0; i < rows; i++ )
    {
        اگر( !LtoR ) j1 = i; وگرنه j0 = i+1;
        برای( صحیح j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(درون_داد_Array _m) ثابت
{
    Mat m = _m.getMat();
    صحیح tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    اگر( d == CV_32F )
    {
        ثابت اعشاری *a = (ثابت اعشاری*)data, *b = (ثابت اعشاری*)m.data;
        اعشاری* c = (اعشاری*)result.data;
        size_t lda = rows > 1 ? step/اندازه(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/اندازه(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    وگرنه اگر( d == CV_64F )
    {
        ثابت دوگانه *a = (ثابت دوگانه*)data, *b = (ثابت دوگانه*)m.data;
        دوگانه* c = (دوگانه*)result.data;
        size_t lda = rows > 1 ? step/اندازه(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/اندازه(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    بازگشت result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

فضاینام cv
{

تعریفنوع تهی (*ReduceSumFunc)(ثابت Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(صحیح sdepth, صحیح ddepth);
ReduceSumFunc getReduceRSumFunc(صحیح sdepth, صحیح ddepth);
ReduceSumFunc getReduceRSum2Func(صحیح sdepth, صحیح ddepth);

template <typename T, typename WT, typename Op>
ساختار ReduceR_SIMD
{
    صحیح operator()(ثابت T*, صحیح start, صحیح, WT*, ثابت Op&) ثابت
    {
        بازگشت start;
    }
};


template<typename T, typename ST, typename WT, رده Op, رده OpInit>
رده ReduceR_Invoker : عمومی ParallelLoopBody
{
عمومی:
  ReduceR_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  تهی operator()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت T* src = srcmat.ptr<T>();
    ثابت size_t srcstep = srcmat.step/اندازه(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    صحیح i = 0;

    برای( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    صحیح height = srcmat.size().height;
    برای( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #اگر CV_ENABLE_UNROLLED
        برای(; i <= range.end - 4; i += 4 )
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
        برای( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    برای( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
خصوصی:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, رده Op, رده OpInit = OpNop<ST> > ایستا تهی
reduceR_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعریفنوع typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, رده Op, رده OpInit>
رده ReduceC_Invoker : عمومی ParallelLoopBody
{
عمومی:
  ReduceC_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  تهی operator()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت صحیح cn = srcmat.channels();
    ثابت صحیح width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    برای( صحیح y = range.start; y < range.end; y++ )
    {
        ثابت T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        اگر( width == cn )
        {
          برای( صحیح k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        وگرنه
        {
            برای(صحیح k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            برای(صحیح k = cn ; k < width ; k += cn )
            {
                برای (صحیح c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            برای(صحیح k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
خصوصی:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, رده Op, رده OpInit = OpNop<ST> > ایستا تهی
reduceC_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعریفنوع typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

تعریفنوع تهی (*ReduceFunc)( ثابت Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, صحیح,   OpAdd<صحیح>, OpNop<صحیح> >
#define reduceSumR8u32f  reduceR_<uchar, اعشاری, OpAdd<صحیح>, OpNop<صحیح> >
#define reduceSumR8u64f  reduceR_<uchar, دوگانه,OpAdd<صحیح>, OpNop<صحیح> >
#define reduceSumR16u32f reduceR_<ushort,اعشاری, OpAdd<اعشاری> >
#define reduceSumR16u64f reduceR_<ushort,دوگانه,OpAdd<دوگانه> >
#define reduceSumR16s32f reduceR_<short, اعشاری, OpAdd<اعشاری> >
#define reduceSumR16s64f reduceR_<short, دوگانه,OpAdd<دوگانه> >
#define reduceSumR32f32f reduceR_<اعشاری, اعشاری, OpAdd<اعشاری> >
#define reduceSumR32f64f reduceR_<اعشاری, دوگانه,OpAdd<دوگانه> >
#define reduceSumR64f64f reduceR_<دوگانه,دوگانه,OpAdd<دوگانه> >

#define reduceSum2R8u32s  reduceR_<uchar, صحیح,   OpAddSqr<صحیح>,   OpSqr<صحیح> >
#define reduceSum2R8u32f  reduceR_<uchar, اعشاری, OpAddSqr<صحیح>,   OpSqr<صحیح> >
#define reduceSum2R8u64f  reduceR_<uchar, دوگانه,OpAddSqr<صحیح>,   OpSqr<صحیح> >
#define reduceSum2R16u32f reduceR_<ushort,اعشاری, OpAddSqr<اعشاری>, OpSqr<اعشاری> >
#define reduceSum2R16u64f reduceR_<ushort,دوگانه,OpAddSqr<دوگانه>,OpSqr<دوگانه> >
#define reduceSum2R16s32f reduceR_<short, اعشاری, OpAddSqr<اعشاری>, OpSqr<اعشاری> >
#define reduceSum2R16s64f reduceR_<short, دوگانه,OpAddSqr<دوگانه>,OpSqr<دوگانه> >
#define reduceSum2R32f32f reduceR_<اعشاری, اعشاری, OpAddSqr<اعشاری>, OpSqr<اعشاری> >
#define reduceSum2R32f64f reduceR_<اعشاری, دوگانه,OpAddSqr<دوگانه>,OpSqr<دوگانه> >
#define reduceSum2R64f64f reduceR_<دوگانه,دوگانه,OpAddSqr<دوگانه>,OpSqr<دوگانه> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<اعشاری, اعشاری, OpMax<اعشاری> >
#define reduceMaxR64f reduceR_<دوگانه,دوگانه,OpMax<دوگانه> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<اعشاری, اعشاری, OpMin<اعشاری> >
#define reduceMinR64f reduceR_<دوگانه,دوگانه,OpMin<دوگانه> >

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ایستا درونخطی منطقی ipp_reduceSumC_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    صحیح sstep = (صحیح)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    تعریفنوع IppStatus (CV_STDCALL * IppiSum)(ثابت تهی * pSrc, صحیح srcStep, IppiSize roiSize, Ipp64f* pSum);
    تعریفنوع IppStatus (CV_STDCALL * IppiSumHint)(ثابت تهی * pSrc, صحیح srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    اگر(ddepth == CV_64F)
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

    اگر(ippiSum)
    {
        برای(صحیح y = 0; y < srcmat.size().height; y++)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                بازگشت نادرست;
        }
        بازگشت درست;
    }
    وگرنه اگر(ippiSumHint)
    {
        برای(صحیح y = 0; y < srcmat.size().height; y++)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                بازگشت نادرست;
        }
        بازگشت درست;
    }

    بازگشت نادرست;
}

ایستا درونخطی تهی reduceSumC_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    اگر(dstmat.depth() == CV_64F)
    {
        صحیح sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, دوگانه,   cv::OpAdd<دوگانه> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, دوگانه,   cv::OpAdd<دوگانه> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, دوگانه,   cv::OpAdd<دوگانه> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<اعشاری, دوگانه,   cv::OpAdd<دوگانه> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, صحیح,   OpAdd<صحیح>, OpNop<صحیح> >
#define reduceSumC8u32f  reduceC_<uchar, اعشاری, OpAdd<صحیح>, OpNop<صحیح> >
#define reduceSumC16u32f reduceC_<ushort,اعشاری, OpAdd<اعشاری> >
#define reduceSumC16s32f reduceC_<short, اعشاری, OpAdd<اعشاری> >
#define reduceSumC32f32f reduceC_<اعشاری, اعشاری, OpAdd<اعشاری> >
#define reduceSumC64f64f reduceC_<دوگانه,دوگانه,OpAdd<دوگانه> >

#define reduceSum2C8u32s  reduceC_<uchar, صحیح,   OpAddSqr<صحیح>,   OpSqr<صحیح> >
#define reduceSum2C8u32f  reduceC_<uchar, اعشاری, OpAddSqr<صحیح>,   OpSqr<صحیح> >
#define reduceSum2C16u32f reduceC_<ushort,اعشاری, OpAddSqr<اعشاری>, OpSqr<اعشاری> >
#define reduceSum2C16s32f reduceC_<short, اعشاری, OpAddSqr<اعشاری>, OpSqr<اعشاری> >
#define reduceSum2C32f32f reduceC_<اعشاری, اعشاری, OpAddSqr<اعشاری>, OpSqr<اعشاری> >
#define reduceSum2C64f64f reduceC_<دوگانه,دوگانه,OpAddSqr<دوگانه>,OpSqr<دوگانه> >

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#وگرنه
#define reduceSumC8u64f  reduceC_<uchar, دوگانه,OpAdd<صحیح>, OpNop<صحیح> >
#define reduceSumC16u64f reduceC_<ushort,دوگانه,OpAdd<دوگانه> >
#define reduceSumC16s64f reduceC_<short, دوگانه,OpAdd<دوگانه> >
#define reduceSumC32f64f reduceC_<اعشاری, دوگانه,OpAdd<دوگانه> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, دوگانه,OpAddSqr<صحیح>,   OpSqr<صحیح> >
#define reduceSum2C16u64f reduceC_<ushort,دوگانه,OpAddSqr<دوگانه>,OpSqr<دوگانه> >
#define reduceSum2C16s64f reduceC_<short, دوگانه,OpAddSqr<دوگانه>,OpSqr<دوگانه> >
#define reduceSum2C32f64f reduceC_<اعشاری, دوگانه,OpAddSqr<دوگانه>,OpSqr<دوگانه> >

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
ایستا درونخطی منطقی ipp_reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    اگر((srcmat.channels() == 1)) \
    { \
        صحیح sstep = (صحیح)srcmat.step; \
        تعریفنوع Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        برای(صحیح y = 0; y < srcmat.size().height; y++)\
        {\
            اگر(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                بازگشت نادرست;\
        }\
        بازگشت درست;\
    }\
    بازگشت نادرست; \
} \
ایستا درونخطی تهی reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, اعشاری, اعشاری)
#وگرنه
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<اعشاری, اعشاری, OpMax<اعشاری> >
#endif
#define reduceMaxC64f reduceC_<دوگانه,دوگانه,OpMax<دوگانه> >

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, اعشاری, اعشاری)
#وگرنه
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<اعشاری, اعشاری, OpMin<اعشاری> >
#endif
#define reduceMinC64f reduceC_<دوگانه,دوگانه,OpMin<دوگانه> >

#ifdef HAVE_OPENCL

فضاینام cv {

ایستا منطقی ocl_reduce(درون_داد_Array _src, OutputArray _dst,
                       صحیح dim, صحیح op, صحیح op0, صحیح stype, صحیح dtype)
{
    ثابت صحیح min_opt_cols = 128, buf_cols = 32;
    صحیح sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    ثابت ocl::Device &defDev = ocl::Device::getDefault();
    منطقی doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    منطقی useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    اگر (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        بازگشت نادرست;

    اگر (op == REDUCE_AVG)
    {
        اگر (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ثابت نویسه * ثابت ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    صحیح wdepth = std::max(ddepth, CV_32F);
    اگر (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        اگر (defDev.isIntel())
        {
            ایستا ثابت size_t max_Item_In_Group_شمار = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_شمار);
        }
        نویسه cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], اندازه(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], اندازه(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], اندازه(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        اگر (k.empty())
            بازگشت نادرست;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        اگر (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        وگرنه
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        بازگشت k.run(2, globalSize, localSize, نادرست);
    }
    وگرنه
    {
        نویسه cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], اندازه(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], اندازه(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], اندازه(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        اگر (k.empty())
            بازگشت نادرست;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        اگر (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        وگرنه
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        بازگشت k.run(1, &globalsize, پوچ, نادرست);
    }
}

}

#endif

تهی cv::reduce(درون_داد_Array _src, OutputArray _dst, صحیح dim, صحیح op, صحیح dtype)
{
    CV_INSTRUMENT_REGION();

    اگر (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, درست, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        بازگشت;
    }
    صحیح op0 = op;
    صحیح stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    اگر( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    صحیح ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    اگر (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    اگر (src.empty()) {
        src.convertTo(_dst, dtype);
        بازگشت;
    }
    اگر (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    اگر( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        اگر( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    اگر( dim == 0 )
    {
        اگر( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            اگر(simd_func)
                func = (ReduceFunc)simd_func;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            وگرنه اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        وگرنه اگر(op == REDUCE_MAX)
        {
            اگر(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            وگرنه اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        وگرنه اگر(op == REDUCE_MIN)
        {
            اگر(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            وگرنه اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        وگرنه اگر( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            اگر(simd_func)
                func = simd_func;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            وگرنه اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    وگرنه
    {
        اگر(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            اگر(simd_func)
                func = (ReduceFunc)simd_func;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            وگرنه اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        وگرنه اگر(op == REDUCE_MAX)
        {
            اگر(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            وگرنه اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        وگرنه اگر(op == REDUCE_MIN)
        {
            اگر(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            وگرنه اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        وگرنه اگر(op == REDUCE_SUM2)
        {
            اگر(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            وگرنه اگر(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            وگرنه اگر(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            وگرنه اگر(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            وگرنه اگر(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            وگرنه اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    اگر( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    اگر( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

فضاینام cv
{

template<typename T> ایستا تهی sort_( ثابت Mat& src, Mat& dst, صحیح flags )
{
    صحیح n, len;
    منطقی sortRows = (flags & 1) == SORT_EVERY_ROW;
    منطقی inplace = src.data == dst.data;
    منطقی sortDescending = (flags & SORT_DESCENDING) != 0;

    اگر( sortRows )
        n = src.rows, len = src.cols;
    وگرنه
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        AutoBuffer<T> buf;
        اگر( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        برای( صحیح i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            اگر( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                اگر( !inplace )
                {
                    ثابت T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, اندازه(T) * len);
                }
                ptr = dptr;
            }
            وگرنه
            {
                برای( صحیح j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            اگر( sortDescending )
            {
                برای( صحیح j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            اگر( !sortRows )
                برای( صحیح j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#اگر defined(HAVE_IPP) && !IPP_DISABLE_SORT
تعریفنوع IppStatus (CV_STDCALL *IppSortFunc)(تهی  *pSrcDst, صحیح    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, صحیح, Ipp8u*)>
ایستا IppStatus CV_STDCALL ippSortWrap(تهی* pSrcDst, صحیح len, Ipp8u* pBuffer)
{
    بازگشت fn((T*)pSrcDst, len, pBuffer);
}

ایستا IppSortFunc getSortFunc(صحیح depth, منطقی sortDescending)
{
    اگر (!sortDescending)
        بازگشت depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    وگرنه
        بازگشت depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

ایستا منطقی ipp_sort(ثابت Mat& src, Mat& dst, صحیح flags)
{
    CV_INSTRUMENT_REGION_IPP();

    منطقی        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    منطقی        sortDescending  = (flags & SORT_DESCENDING) != 0;
    منطقی        inplace         = (src.data == dst.data);
    صحیح         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    اگر(!ippsSortRadix_I)
        بازگشت نادرست;

    صحیح n, len;
    اگر( sortRows )
        n = src.rows, len = src.cols;
    وگرنه
        n = src.cols, len = src.rows;

    صحیح bufferSize;
    اگر(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        بازگشت نادرست;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    فرار منطقی ok = درست;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        اگر( !sortRows )
            row.create(1, src.rows, src.type());

        برای( صحیح i = range.start; i < range.end; i++ )
        {
            اگر( !ok )
                بشکن;
            تهی* ptr;
            اگر( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                اگر( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            وگرنه
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            اگر( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = نادرست;
                بشکن;
            }

            اگر( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    بازگشت ok;
}
#endif

template<typename _Tp> رده LessThanIdx
{
عمومی:
    LessThanIdx( ثابت _Tp* _arr ) : arr(_arr) {}
    منطقی operator()(صحیح a, صحیح b) ثابت { بازگشت arr[a] < arr[b]; }
    ثابت _Tp* arr;
};

template<typename T> ایستا تهی sortIdx_( ثابت Mat& src, Mat& dst, صحیح flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<صحیح> ibuf;
    منطقی sortRows = (flags & 1) == SORT_EVERY_ROW;
    منطقی sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    صحیح n, len;
    اگر( sortRows )
        n = src.rows, len = src.cols;
    وگرنه
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    صحیح* _iptr = ibuf.data();

    برای( صحیح i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        صحیح* iptr = _iptr;

        اگر( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<صحیح>(i);
        }
        وگرنه
        {
            برای( صحیح j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        برای( صحیح j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        اگر( sortDescending )
        {
            برای( صحیح j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        اگر( !sortRows )
            برای( صحیح j = 0; j < len; j++ )
                dst.ptr<صحیح>(j)[i] = iptr[j];
    }
}

#اگر defined(HAVE_IPP) && !IPP_DISABLE_SORT
تعریفنوع IppStatus (CV_STDCALL *IppSortIndexFunc)(ثابت تهی*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, صحیح len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(ثابت T*, Ipp32s, Ipp32s*, صحیح, Ipp8u*)>
ایستا IppStatus CV_STDCALL ippSortIndexWrap(ثابت تهی* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, صحیح len, Ipp8u *pBuffer)
{
    بازگشت fn((ثابت T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

ایستا IppSortIndexFunc getSortIndexFunc(صحیح depth, منطقی sortDescending)
{
    اگر (!sortDescending)
        بازگشت depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    وگرنه
        بازگشت depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

ایستا منطقی ipp_sortIdx( ثابت Mat& src, Mat& dst, صحیح flags )
{
    CV_INSTRUMENT_REGION_IPP();

    منطقی        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    منطقی        sortDescending  = (flags & SORT_DESCENDING) != 0;
    صحیح         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    اگر(!ippsSortRadixIndex)
        بازگشت نادرست;

    اگر(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        صحیح               bufferSize;
        اگر(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            بازگشت نادرست;

        buffer.allocate(bufferSize);

        برای(صحیح i = 0; i < src.rows; i++)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ثابت تهی*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                بازگشت نادرست;
        }
    }
    وگرنه
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        صحیح               bufferSize;
        اگر(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            بازگشت نادرست;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        برای(صحیح i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            اگر(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ثابت تهی*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                بازگشت نادرست;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    بازگشت درست;
}
#endif

تعریفنوع تهی (*SortFunc)(ثابت Mat& src, Mat& dst, صحیح flags);
}

تهی cv::sort( درون_داد_Array _src, OutputArray _dst, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#اگر !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    ایستا SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<صحیح>, sort_<اعشاری>, sort_<دوگانه>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

تهی cv::sortIdx( درون_داد_Array _src, OutputArray _dst, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    اگر( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#اگر !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    ایستا SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<صحیح>, sortIdx_<اعشاری>, sortIdx_<دوگانه>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
