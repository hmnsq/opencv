// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"
#ضمن "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

فراغ cv::swap( Mat& a, Mat& b )
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


فراغ cv::hconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح totalCols = 0, cols = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

فراغ cv::hconcat(شغرو_Array src1, شغرو_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

فراغ cv::hconcat(شغرو_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

فراغ cv::vconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح totalRows = 0, rows = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

فراغ cv::vconcat(شغرو_Array src1, شغرو_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

فراغ cv::vconcat(شغرو_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_setIdentity( شغرو_Output_Array _m, ثابت Scalar& s )
{
    صحيح type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    صحيح sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    إذا (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        إذا (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            إذا (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    إذا (k.empty())
        أرجع خطأ;

    UMat m = _m.getUMat();
    k.حجوجي(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    أرجع k.run(2, globalsize, nullptr, خطأ);
}

}

#endif

فراغ cv::setIdentity( شغرو_Output_Array _m, ثابت Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    صحيح rows = m.rows, cols = m.cols, type = m.type();

    إذا( type == CV_32FC1 )
    {
        عائم* data = m.ptr<عائم>();
        عائم val = (عائم)s[0];
        size_t step = m.step/حجم(data[0]);

        لكل( صحيح i = 0; i < rows; i++, data += step )
        {
            لكل( صحيح j = 0; j < cols; j++ )
                data[j] = 0;
            إذا( i < cols )
                data[i] = val;
        }
    }
    وإلا إذا( type == CV_64FC1 )
    {
        مزدوج* data = m.ptr<مزدوج>();
        مزدوج val = s[0];
        size_t step = m.step/حجم(data[0]);

        لكل( صحيح i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            إذا (i < cols)
                data[i] = val;
        }
    }
    وإلا
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


نطاق cv {

UMat UMat::eye(صحيح rows, صحيح cols, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, صحيح type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    أرجع m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( شغرو_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    صحيح type = m.type();
    صحيح nm = std::min(m.rows, m.cols);

    إذا( type == CV_32FC1 )
    {
        ثابت عائم* ptr = m.ptr<عائم>();
        size_t step = m.step/حجم(ptr[0]) + 1;
        مزدوج _s = 0;
        لكل( صحيح i = 0; i < nm; i++ )
            _s += ptr[i*step];
        أرجع _s;
    }

    إذا( type == CV_64FC1 )
    {
        ثابت مزدوج* ptr = m.ptr<مزدوج>();
        size_t step = m.step/حجم(ptr[0]) + 1;
        مزدوج _s = 0;
        لكل( صحيح i = 0; i < nm; i++ )
            _s += ptr[i*step];
        أرجع _s;
    }

    أرجع cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

فراغ cv::completeSymm( شغرو_Output_Array _m, منطقي LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    صحيح rows = m.rows;
    صحيح j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    لكل( صحيح i = 0; i < rows; i++ )
    {
        إذا( !LtoR ) j1 = i; وإلا j0 = i+1;
        لكل( صحيح j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(شغرو_Array _m) ثابت
{
    Mat m = _m.getMat();
    صحيح tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    إذا( d == CV_32F )
    {
        ثابت عائم *a = (ثابت عائم*)data, *b = (ثابت عائم*)m.data;
        عائم* c = (عائم*)result.data;
        size_t lda = rows > 1 ? step/حجم(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    وإلا إذا( d == CV_64F )
    {
        ثابت مزدوج *a = (ثابت مزدوج*)data, *b = (ثابت مزدوج*)m.data;
        مزدوج* c = (مزدوج*)result.data;
        size_t lda = rows > 1 ? step/حجم(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    أرجع result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

نطاق cv
{

تعريفنوع فراغ (*ReduceSumFunc)(ثابت Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(صحيح sdepth, صحيح ddepth);
ReduceSumFunc getReduceRSumFunc(صحيح sdepth, صحيح ddepth);
ReduceSumFunc getReduceRSum2Func(صحيح sdepth, صحيح ddepth);

template <typename T, typename WT, typename Op>
بنية ReduceR_SIMD
{
    صحيح operator()(ثابت T*, صحيح start, صحيح, WT*, ثابت Op&) ثابت
    {
        أرجع start;
    }
};


template<typename T, typename ST, typename WT, صنف Op, صنف OpInit>
صنف ReduceR_Invoker : عام ParallelLoopBody
{
عام:
  ReduceR_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  فراغ operator()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت T* src = srcmat.ptr<T>();
    ثابت size_t srcstep = srcmat.step/حجم(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    صحيح i = 0;

    لكل( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    صحيح height = srcmat.size().height;
    لكل( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #إذا CV_ENABLE_UNROLLED
        لكل(; i <= range.end - 4; i += 4 )
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
        لكل( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    لكل( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
خاص:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceR_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, صنف Op, صنف OpInit>
صنف ReduceC_Invoker : عام ParallelLoopBody
{
عام:
  ReduceC_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  فراغ operator()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    لكل( صحيح y = range.start; y < range.end; y++ )
    {
        ثابت T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        إذا( width == cn )
        {
          لكل( صحيح k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        وإلا
        {
            لكل(صحيح k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            لكل(صحيح k = cn ; k < width ; k += cn )
            {
                لكل (صحيح c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            لكل(صحيح k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
خاص:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceC_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

تعريفنوع فراغ (*ReduceFunc)( ثابت Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, صحيح,   OpAdd<صحيح>, OpNop<صحيح> >
#define reduceSumR8u32f  reduceR_<uchar, عائم, OpAdd<صحيح>, OpNop<صحيح> >
#define reduceSumR8u64f  reduceR_<uchar, مزدوج,OpAdd<صحيح>, OpNop<صحيح> >
#define reduceSumR16u32f reduceR_<ushort,عائم, OpAdd<عائم> >
#define reduceSumR16u64f reduceR_<ushort,مزدوج,OpAdd<مزدوج> >
#define reduceSumR16s32f reduceR_<short, عائم, OpAdd<عائم> >
#define reduceSumR16s64f reduceR_<short, مزدوج,OpAdd<مزدوج> >
#define reduceSumR32f32f reduceR_<عائم, عائم, OpAdd<عائم> >
#define reduceSumR32f64f reduceR_<عائم, مزدوج,OpAdd<مزدوج> >
#define reduceSumR64f64f reduceR_<مزدوج,مزدوج,OpAdd<مزدوج> >

#define reduceSum2R8u32s  reduceR_<uchar, صحيح,   OpAddSqr<صحيح>,   OpSqr<صحيح> >
#define reduceSum2R8u32f  reduceR_<uchar, عائم, OpAddSqr<صحيح>,   OpSqr<صحيح> >
#define reduceSum2R8u64f  reduceR_<uchar, مزدوج,OpAddSqr<صحيح>,   OpSqr<صحيح> >
#define reduceSum2R16u32f reduceR_<ushort,عائم, OpAddSqr<عائم>, OpSqr<عائم> >
#define reduceSum2R16u64f reduceR_<ushort,مزدوج,OpAddSqr<مزدوج>,OpSqr<مزدوج> >
#define reduceSum2R16s32f reduceR_<short, عائم, OpAddSqr<عائم>, OpSqr<عائم> >
#define reduceSum2R16s64f reduceR_<short, مزدوج,OpAddSqr<مزدوج>,OpSqr<مزدوج> >
#define reduceSum2R32f32f reduceR_<عائم, عائم, OpAddSqr<عائم>, OpSqr<عائم> >
#define reduceSum2R32f64f reduceR_<عائم, مزدوج,OpAddSqr<مزدوج>,OpSqr<مزدوج> >
#define reduceSum2R64f64f reduceR_<مزدوج,مزدوج,OpAddSqr<مزدوج>,OpSqr<مزدوج> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<عائم, عائم, OpMax<عائم> >
#define reduceMaxR64f reduceR_<مزدوج,مزدوج,OpMax<مزدوج> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<عائم, عائم, OpMin<عائم> >
#define reduceMinR64f reduceR_<مزدوج,مزدوج,OpMin<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ساكن مضمن منطقي ipp_reduceSumC_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    صحيح sstep = (صحيح)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    تعريفنوع IppStatus (CV_STDCALL * IppiSum)(ثابت فراغ * pSrc, صحيح srcStep, IppiSize roiSize, Ipp64f* pSum);
    تعريفنوع IppStatus (CV_STDCALL * IppiSumHint)(ثابت فراغ * pSrc, صحيح srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    إذا(ddepth == CV_64F)
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

    إذا(ippiSum)
    {
        لكل(صحيح y = 0; y < srcmat.size().height; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                أرجع خطأ;
        }
        أرجع true;
    }
    وإلا إذا(ippiSumHint)
    {
        لكل(صحيح y = 0; y < srcmat.size().height; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                أرجع خطأ;
        }
        أرجع true;
    }

    أرجع خطأ;
}

ساكن مضمن فراغ reduceSumC_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    إذا(dstmat.depth() == CV_64F)
    {
        صحيح sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, مزدوج,   cv::OpAdd<مزدوج> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, مزدوج,   cv::OpAdd<مزدوج> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, مزدوج,   cv::OpAdd<مزدوج> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<عائم, مزدوج,   cv::OpAdd<مزدوج> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, صحيح,   OpAdd<صحيح>, OpNop<صحيح> >
#define reduceSumC8u32f  reduceC_<uchar, عائم, OpAdd<صحيح>, OpNop<صحيح> >
#define reduceSumC16u32f reduceC_<ushort,عائم, OpAdd<عائم> >
#define reduceSumC16s32f reduceC_<short, عائم, OpAdd<عائم> >
#define reduceSumC32f32f reduceC_<عائم, عائم, OpAdd<عائم> >
#define reduceSumC64f64f reduceC_<مزدوج,مزدوج,OpAdd<مزدوج> >

#define reduceSum2C8u32s  reduceC_<uchar, صحيح,   OpAddSqr<صحيح>,   OpSqr<صحيح> >
#define reduceSum2C8u32f  reduceC_<uchar, عائم, OpAddSqr<صحيح>,   OpSqr<صحيح> >
#define reduceSum2C16u32f reduceC_<ushort,عائم, OpAddSqr<عائم>, OpSqr<عائم> >
#define reduceSum2C16s32f reduceC_<short, عائم, OpAddSqr<عائم>, OpSqr<عائم> >
#define reduceSum2C32f32f reduceC_<عائم, عائم, OpAddSqr<عائم>, OpSqr<عائم> >
#define reduceSum2C64f64f reduceC_<مزدوج,مزدوج,OpAddSqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#وإلا
#define reduceSumC8u64f  reduceC_<uchar, مزدوج,OpAdd<صحيح>, OpNop<صحيح> >
#define reduceSumC16u64f reduceC_<ushort,مزدوج,OpAdd<مزدوج> >
#define reduceSumC16s64f reduceC_<short, مزدوج,OpAdd<مزدوج> >
#define reduceSumC32f64f reduceC_<عائم, مزدوج,OpAdd<مزدوج> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, مزدوج,OpAddSqr<صحيح>,   OpSqr<صحيح> >
#define reduceSum2C16u64f reduceC_<ushort,مزدوج,OpAddSqr<مزدوج>,OpSqr<مزدوج> >
#define reduceSum2C16s64f reduceC_<short, مزدوج,OpAddSqr<مزدوج>,OpSqr<مزدوج> >
#define reduceSum2C32f64f reduceC_<عائم, مزدوج,OpAddSqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
ساكن مضمن منطقي ipp_reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    إذا((srcmat.channels() == 1)) \
    { \
        صحيح sstep = (صحيح)srcmat.step; \
        تعريفنوع Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        لكل(صحيح y = 0; y < srcmat.size().height; y++)\
        {\
            إذا(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                أرجع خطأ;\
        }\
        أرجع true;\
    }\
    أرجع خطأ; \
} \
ساكن مضمن فراغ reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, عائم, عائم)
#وإلا
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<عائم, عائم, OpMax<عائم> >
#endif
#define reduceMaxC64f reduceC_<مزدوج,مزدوج,OpMax<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, عائم, عائم)
#وإلا
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<عائم, عائم, OpMin<عائم> >
#endif
#define reduceMinC64f reduceC_<مزدوج,مزدوج,OpMin<مزدوج> >

#ifdef HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_reduce(شغرو_Array _src, OutputArray _dst,
                       صحيح dim, صحيح op, صحيح op0, صحيح stype, صحيح dtype)
{
    ثابت صحيح min_opt_cols = 128, buf_cols = 32;
    صحيح sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    ثابت ocl::Device &defDev = ocl::Device::getDefault();
    منطقي doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    منطقي useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    إذا (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        أرجع خطأ;

    إذا (op == REDUCE_AVG)
    {
        إذا (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ثابت حرف * ثابت ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    صحيح wdepth = std::max(ddepth, CV_32F);
    إذا (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        إذا (defDev.isIntel())
        {
            ساكن ثابت size_t max_Item_In_Group_كدي = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_كدي);
        }
        حرف cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], حجم(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], حجم(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        إذا (k.empty())
            أرجع خطأ;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        إذا (op0 == REDUCE_AVG)
            k.حجوجي(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        وإلا
            k.حجوجي(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        أرجع k.run(2, globalSize, localSize, خطأ);
    }
    وإلا
    {
        حرف cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], حجم(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], حجم(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        إذا (k.empty())
            أرجع خطأ;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        إذا (op0 == REDUCE_AVG)
            k.حجوجي(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        وإلا
            k.حجوجي(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        أرجع k.run(1, &globalsize, عدم, خطأ);
    }
}

}

#endif

فراغ cv::reduce(شغرو_Array _src, OutputArray _dst, صحيح dim, صحيح op, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    إذا (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, true, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        أرجع;
    }
    صحيح op0 = op;
    صحيح stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    إذا( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    صحيح ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    إذا (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    إذا (src.empty()) {
        src.convertTo(_dst, dtype);
        أرجع;
    }
    إذا (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    إذا( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        إذا( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    إذا( dim == 0 )
    {
        إذا( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        وإلا إذا(op == REDUCE_MAX)
        {
            إذا(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        وإلا إذا(op == REDUCE_MIN)
        {
            إذا(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        وإلا إذا( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            إذا(simd_func)
                func = simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    وإلا
    {
        إذا(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        وإلا إذا(op == REDUCE_MAX)
        {
            إذا(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        وإلا إذا(op == REDUCE_MIN)
        {
            إذا(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        وإلا إذا(op == REDUCE_SUM2)
        {
            إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    إذا( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    إذا( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

نطاق cv
{

template<typename T> ساكن فراغ sort_( ثابت Mat& src, Mat& dst, صحيح flags )
{
    صحيح n, len;
    منطقي sortRows = (flags & 1) == SORT_EVERY_ROW;
    منطقي inplace = src.data == dst.data;
    منطقي sortDescending = (flags & SORT_DESCENDING) != 0;

    إذا( sortRows )
        n = src.rows, len = src.cols;
    وإلا
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        AutoBuffer<T> buf;
        إذا( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        لكل( صحيح i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            إذا( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                إذا( !inplace )
                {
                    ثابت T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, حجم(T) * len);
                }
                ptr = dptr;
            }
            وإلا
            {
                لكل( صحيح j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            إذا( sortDescending )
            {
                لكل( صحيح j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            إذا( !sortRows )
                لكل( صحيح j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#إذا defined(HAVE_IPP) && !IPP_DISABLE_SORT
تعريفنوع IppStatus (CV_STDCALL *IppSortFunc)(فراغ  *pSrcDst, صحيح    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, صحيح, Ipp8u*)>
ساكن IppStatus CV_STDCALL ippSortWrap(فراغ* pSrcDst, صحيح len, Ipp8u* pBuffer)
{
    أرجع fn((T*)pSrcDst, len, pBuffer);
}

ساكن IppSortFunc getSortFunc(صحيح depth, منطقي sortDescending)
{
    إذا (!sortDescending)
        أرجع depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    وإلا
        أرجع depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

ساكن منطقي ipp_sort(ثابت Mat& src, Mat& dst, صحيح flags)
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    منطقي        sortDescending  = (flags & SORT_DESCENDING) != 0;
    منطقي        inplace         = (src.data == dst.data);
    صحيح         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    إذا(!ippsSortRadix_I)
        أرجع خطأ;

    صحيح n, len;
    إذا( sortRows )
        n = src.rows, len = src.cols;
    وإلا
        n = src.cols, len = src.rows;

    صحيح bufferSize;
    إذا(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        أرجع خطأ;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    متطاير منطقي ok = true;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        إذا( !sortRows )
            row.create(1, src.rows, src.type());

        لكل( صحيح i = range.start; i < range.end; i++ )
        {
            إذا( !ok )
                اكسر;
            فراغ* ptr;
            إذا( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                إذا( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            وإلا
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            إذا( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = خطأ;
                اكسر;
            }

            إذا( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    أرجع ok;
}
#endif

template<typename _Tp> صنف LessThanIdx
{
عام:
    LessThanIdx( ثابت _Tp* _arr ) : arr(_arr) {}
    منطقي operator()(صحيح a, صحيح b) ثابت { أرجع arr[a] < arr[b]; }
    ثابت _Tp* arr;
};

template<typename T> ساكن فراغ sortIdx_( ثابت Mat& src, Mat& dst, صحيح flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<صحيح> ibuf;
    منطقي sortRows = (flags & 1) == SORT_EVERY_ROW;
    منطقي sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    صحيح n, len;
    إذا( sortRows )
        n = src.rows, len = src.cols;
    وإلا
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    صحيح* _iptr = ibuf.data();

    لكل( صحيح i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        صحيح* iptr = _iptr;

        إذا( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<صحيح>(i);
        }
        وإلا
        {
            لكل( صحيح j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        لكل( صحيح j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        إذا( sortDescending )
        {
            لكل( صحيح j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        إذا( !sortRows )
            لكل( صحيح j = 0; j < len; j++ )
                dst.ptr<صحيح>(j)[i] = iptr[j];
    }
}

#إذا defined(HAVE_IPP) && !IPP_DISABLE_SORT
تعريفنوع IppStatus (CV_STDCALL *IppSortIndexFunc)(ثابت فراغ*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, صحيح len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(ثابت T*, Ipp32s, Ipp32s*, صحيح, Ipp8u*)>
ساكن IppStatus CV_STDCALL ippSortIndexWrap(ثابت فراغ* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, صحيح len, Ipp8u *pBuffer)
{
    أرجع fn((ثابت T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

ساكن IppSortIndexFunc getSortIndexFunc(صحيح depth, منطقي sortDescending)
{
    إذا (!sortDescending)
        أرجع depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    وإلا
        أرجع depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

ساكن منطقي ipp_sortIdx( ثابت Mat& src, Mat& dst, صحيح flags )
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    منطقي        sortDescending  = (flags & SORT_DESCENDING) != 0;
    صحيح         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    إذا(!ippsSortRadixIndex)
        أرجع خطأ;

    إذا(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        صحيح               bufferSize;
        إذا(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            أرجع خطأ;

        buffer.allocate(bufferSize);

        لكل(صحيح i = 0; i < src.rows; i++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ثابت فراغ*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                أرجع خطأ;
        }
    }
    وإلا
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        صحيح               bufferSize;
        إذا(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            أرجع خطأ;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        لكل(صحيح i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            إذا(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ثابت فراغ*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                أرجع خطأ;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    أرجع true;
}
#endif

تعريفنوع فراغ (*SortFunc)(ثابت Mat& src, Mat& dst, صحيح flags);
}

فراغ cv::sort( شغرو_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#إذا !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    ساكن SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<صحيح>, sort_<عائم>, sort_<مزدوج>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

فراغ cv::sortIdx( شغرو_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    إذا( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#إذا !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    ساكن SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<صحيح>, sortIdx_<عائم>, sortIdx_<مزدوج>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
