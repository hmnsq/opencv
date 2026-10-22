// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/mat.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

ଶୂନ୍ୟ cv::swap( Mat& a, Mat& b )
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


ଶୂନ୍ୟ cv::hconcat(ଅଚଳ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ଯଦି( nsrc == 0 || !src )
    {
        _dst.release();
        ଫେରନ୍ତୁ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା totalCols = 0, cols = 0;
    ପାଇଁ( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    ପାଇଁ( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

ଶୂନ୍ୟ cv::hconcat(ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

ଶୂନ୍ୟ cv::hconcat(ନିବେଶ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

ଶୂନ୍ୟ cv::vconcat(ଅଚଳ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    ଯଦି( nsrc == 0 || !src )
    {
        _dst.release();
        ଫେରନ୍ତୁ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା totalRows = 0, rows = 0;
    ପାଇଁ( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    ପାଇଁ( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

ଶୂନ୍ୟ cv::vconcat(ନିବେଶ_Array src1, ନିବେଶ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

ଶୂନ୍ୟ cv::vconcat(ନିବେଶ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

ନାମାକାଶ cv {

ସ୍ଥିର ତାର୍କିକ ocl_setIdentity( ନିବେଶ_Output_Array _m, ଅଚଳ Scalar& s )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    ଯଦି (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        ଯଦି (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            ଯଦି (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, nullptr, ମିଥ୍ୟା);
}

}

#endif

ଶୂନ୍ୟ cv::setIdentity( ନିବେଶ_Output_Array _m, ଅଚଳ Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows = m.rows, cols = m.cols, type = m.type();

    ଯଦି( type == CV_32FC1 )
    {
        ଦଶମିକ* data = m.ptr<ଦଶମିକ>();
        ଦଶମିକ val = (ଦଶମିକ)s[0];
        size_t step = m.step/ଆକାର(data[0]);

        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < rows; i++, data += step )
        {
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < cols; j++ )
                data[j] = 0;
            ଯଦି( i < cols )
                data[i] = val;
        }
    }
    ଅନ୍ୟଥା ଯଦି( type == CV_64FC1 )
    {
        ଦ୍ୱିଗୁଣ* data = m.ptr<ଦ୍ୱିଗୁଣ>();
        ଦ୍ୱିଗୁଣ val = s[0];
        size_t step = m.step/ଆକାର(data[0]);

        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            ଯଦି (i < cols)
                data[i] = val;
        }
    }
    ଅନ୍ୟଥା
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


ନାମାକାଶ cv {

UMat UMat::eye(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    ଫେରନ୍ତୁ UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    ଫେରନ୍ତୁ m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( ନିବେଶ_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = m.type();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nm = std::min(m.rows, m.cols);

    ଯଦି( type == CV_32FC1 )
    {
        ଅଚଳ ଦଶମିକ* ptr = m.ptr<ଦଶମିକ>();
        size_t step = m.step/ଆକାର(ptr[0]) + 1;
        ଦ୍ୱିଗୁଣ _s = 0;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nm; i++ )
            _s += ptr[i*step];
        ଫେରନ୍ତୁ _s;
    }

    ଯଦି( type == CV_64FC1 )
    {
        ଅଚଳ ଦ୍ୱିଗୁଣ* ptr = m.ptr<ଦ୍ୱିଗୁଣ>();
        size_t step = m.step/ଆକାର(ptr[0]) + 1;
        ଦ୍ୱିଗୁଣ _s = 0;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nm; i++ )
            _s += ptr[i*step];
        ଫେରନ୍ତୁ _s;
    }

    ଫେରନ୍ତୁ cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

ଶୂନ୍ୟ cv::completeSymm( ନିବେଶ_Output_Array _m, ତାର୍କିକ LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows = m.rows;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < rows; i++ )
    {
        ଯଦି( !LtoR ) j1 = i; ଅନ୍ୟଥା j0 = i+1;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(ନିବେଶ_Array _m) ଅଚଳ
{
    Mat m = _m.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    ଯଦି( d == CV_32F )
    {
        ଅଚଳ ଦଶମିକ *a = (ଅଚଳ ଦଶମିକ*)data, *b = (ଅଚଳ ଦଶମିକ*)m.data;
        ଦଶମିକ* c = (ଦଶମିକ*)result.data;
        size_t lda = rows > 1 ? step/ଆକାର(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/ଆକାର(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    ଅନ୍ୟଥା ଯଦି( d == CV_64F )
    {
        ଅଚଳ ଦ୍ୱିଗୁଣ *a = (ଅଚଳ ଦ୍ୱିଗୁଣ*)data, *b = (ଅଚଳ ଦ୍ୱିଗୁଣ*)m.data;
        ଦ୍ୱିଗୁଣ* c = (ଦ୍ୱିଗୁଣ*)result.data;
        size_t lda = rows > 1 ? step/ଆକାର(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/ଆକାର(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    ଫେରନ୍ତୁ result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

ନାମାକାଶ cv
{

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ReduceSumFunc)(ଅଚଳ Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth);
ReduceSumFunc getReduceRSumFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth);
ReduceSumFunc getReduceRSum2Func(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth);

template <typename T, typename WT, typename Op>
ସଂରଚନା ReduceR_SIMD
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator()(ଅଚଳ T*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, WT*, ଅଚଳ Op&) ଅଚଳ
    {
        ଫେରନ୍ତୁ start;
    }
};


template<typename T, typename ST, typename WT, ଶ୍ରେଣୀ Op, ଶ୍ରେଣୀ OpInit>
ଶ୍ରେଣୀ ReduceR_Invoker : ସର୍ବସାଧାରଣ ParallelLoopBody
{
ସର୍ବସାଧାରଣ:
  ReduceR_Invoker(ଅଚଳ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  ଶୂନ୍ୟ operator()(ଅଚଳ Range& range) ଅଚଳ CV_OVERRIDE
  {
    ଅଚଳ T* src = srcmat.ptr<T>();
    ଅଚଳ size_t srcstep = srcmat.step/ଆକାର(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

    ପାଇଁ( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା height = srcmat.size().height;
    ପାଇଁ( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #ଯଦି CV_ENABLE_UNROLLED
        ପାଇଁ(; i <= range.end - 4; i += 4 )
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
        ପାଇଁ( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    ପାଇଁ( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, ଶ୍ରେଣୀ Op, ଶ୍ରେଣୀ OpInit = OpNop<ST> > ସ୍ଥିର ଶୂନ୍ୟ
reduceR_( ଅଚଳ Mat& srcmat, Mat& dstmat)
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, ଶ୍ରେଣୀ Op, ଶ୍ରେଣୀ OpInit>
ଶ୍ରେଣୀ ReduceC_Invoker : ସର୍ବସାଧାରଣ ParallelLoopBody
{
ସର୍ବସାଧାରଣ:
  ReduceC_Invoker(ଅଚଳ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  ଶୂନ୍ୟ operator()(ଅଚଳ Range& range) ଅଚଳ CV_OVERRIDE
  {
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = srcmat.channels();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = range.start; y < range.end; y++ )
    {
        ଅଚଳ T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        ଯଦି( width == cn )
        {
          ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        ଅନ୍ୟଥା
        {
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = cn ; k < width ; k += cn )
            {
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, ଶ୍ରେଣୀ Op, ଶ୍ରେଣୀ OpInit = OpNop<ST> > ସ୍ଥିର ଶୂନ୍ୟ
reduceC_( ଅଚଳ Mat& srcmat, Mat& dstmat)
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ReduceFunc)( ଅଚଳ Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,   OpAdd<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, OpNop<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSumR8u32f  reduceR_<uchar, ଦଶମିକ, OpAdd<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, OpNop<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSumR8u64f  reduceR_<uchar, ଦ୍ୱିଗୁଣ,OpAdd<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, OpNop<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSumR16u32f reduceR_<ushort,ଦଶମିକ, OpAdd<ଦଶମିକ> >
#define reduceSumR16u64f reduceR_<ushort,ଦ୍ୱିଗୁଣ,OpAdd<ଦ୍ୱିଗୁଣ> >
#define reduceSumR16s32f reduceR_<short, ଦଶମିକ, OpAdd<ଦଶମିକ> >
#define reduceSumR16s64f reduceR_<short, ଦ୍ୱିଗୁଣ,OpAdd<ଦ୍ୱିଗୁଣ> >
#define reduceSumR32f32f reduceR_<ଦଶମିକ, ଦଶମିକ, OpAdd<ଦଶମିକ> >
#define reduceSumR32f64f reduceR_<ଦଶମିକ, ଦ୍ୱିଗୁଣ,OpAdd<ଦ୍ୱିଗୁଣ> >
#define reduceSumR64f64f reduceR_<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ,OpAdd<ଦ୍ୱିଗୁଣ> >

#define reduceSum2R8u32s  reduceR_<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,   OpAddSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,   OpSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSum2R8u32f  reduceR_<uchar, ଦଶମିକ, OpAddSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,   OpSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSum2R8u64f  reduceR_<uchar, ଦ୍ୱିଗୁଣ,OpAddSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,   OpSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSum2R16u32f reduceR_<ushort,ଦଶମିକ, OpAddSqr<ଦଶମିକ>, OpSqr<ଦଶମିକ> >
#define reduceSum2R16u64f reduceR_<ushort,ଦ୍ୱିଗୁଣ,OpAddSqr<ଦ୍ୱିଗୁଣ>,OpSqr<ଦ୍ୱିଗୁଣ> >
#define reduceSum2R16s32f reduceR_<short, ଦଶମିକ, OpAddSqr<ଦଶମିକ>, OpSqr<ଦଶମିକ> >
#define reduceSum2R16s64f reduceR_<short, ଦ୍ୱିଗୁଣ,OpAddSqr<ଦ୍ୱିଗୁଣ>,OpSqr<ଦ୍ୱିଗୁଣ> >
#define reduceSum2R32f32f reduceR_<ଦଶମିକ, ଦଶମିକ, OpAddSqr<ଦଶମିକ>, OpSqr<ଦଶମିକ> >
#define reduceSum2R32f64f reduceR_<ଦଶମିକ, ଦ୍ୱିଗୁଣ,OpAddSqr<ଦ୍ୱିଗୁଣ>,OpSqr<ଦ୍ୱିଗୁଣ> >
#define reduceSum2R64f64f reduceR_<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ,OpAddSqr<ଦ୍ୱିଗୁଣ>,OpSqr<ଦ୍ୱିଗୁଣ> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<ଦଶମିକ, ଦଶମିକ, OpMax<ଦଶମିକ> >
#define reduceMaxR64f reduceR_<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ,OpMax<ଦ୍ୱିଗୁଣ> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<ଦଶମିକ, ଦଶମିକ, OpMin<ଦଶମିକ> >
#define reduceMinR64f reduceR_<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ,OpMin<ଦ୍ୱିଗୁଣ> >

#ଯଦି defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ipp_reduceSumC_8u16u16s32f_64f(ଅଚଳ cv::Mat& srcmat, cv::Mat& dstmat)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sstep = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    ପ୍ରକାରବ୍ୟାଖ୍ୟା IppStatus (CV_STDCALL * IppiSum)(ଅଚଳ ଶୂନ୍ୟ * pSrc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcStep, IppiSize roiSize, Ipp64f* pSum);
    ପ୍ରକାରବ୍ୟାଖ୍ୟା IppStatus (CV_STDCALL * IppiSumHint)(ଅଚଳ ଶୂନ୍ୟ * pSrc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    ଯଦି(ddepth == CV_64F)
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

    ଯଦି(ippiSum)
    {
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < srcmat.size().height; y++)
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଅନ୍ୟଥା ଯଦି(ippiSumHint)
    {
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < srcmat.size().height; y++)
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ reduceSumC_8u16u16s32f_64f(ଅଚଳ cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    ଯଦି(dstmat.depth() == CV_64F)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, ଦ୍ୱିଗୁଣ,   cv::OpAdd<ଦ୍ୱିଗୁଣ> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, ଦ୍ୱିଗୁଣ,   cv::OpAdd<ଦ୍ୱିଗୁଣ> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, ଦ୍ୱିଗୁଣ,   cv::OpAdd<ଦ୍ୱିଗୁଣ> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<ଦଶମିକ, ଦ୍ୱିଗୁଣ,   cv::OpAdd<ଦ୍ୱିଗୁଣ> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,   OpAdd<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, OpNop<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSumC8u32f  reduceC_<uchar, ଦଶମିକ, OpAdd<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, OpNop<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSumC16u32f reduceC_<ushort,ଦଶମିକ, OpAdd<ଦଶମିକ> >
#define reduceSumC16s32f reduceC_<short, ଦଶମିକ, OpAdd<ଦଶମିକ> >
#define reduceSumC32f32f reduceC_<ଦଶମିକ, ଦଶମିକ, OpAdd<ଦଶମିକ> >
#define reduceSumC64f64f reduceC_<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ,OpAdd<ଦ୍ୱିଗୁଣ> >

#define reduceSum2C8u32s  reduceC_<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା,   OpAddSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,   OpSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSum2C8u32f  reduceC_<uchar, ଦଶମିକ, OpAddSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,   OpSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSum2C16u32f reduceC_<ushort,ଦଶମିକ, OpAddSqr<ଦଶମିକ>, OpSqr<ଦଶମିକ> >
#define reduceSum2C16s32f reduceC_<short, ଦଶମିକ, OpAddSqr<ଦଶମିକ>, OpSqr<ଦଶମିକ> >
#define reduceSum2C32f32f reduceC_<ଦଶମିକ, ଦଶମିକ, OpAddSqr<ଦଶମିକ>, OpSqr<ଦଶମିକ> >
#define reduceSum2C64f64f reduceC_<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ,OpAddSqr<ଦ୍ୱିଗୁଣ>,OpSqr<ଦ୍ୱିଗୁଣ> >

#ଯଦି defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#ଅନ୍ୟଥା
#define reduceSumC8u64f  reduceC_<uchar, ଦ୍ୱିଗୁଣ,OpAdd<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, OpNop<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSumC16u64f reduceC_<ushort,ଦ୍ୱିଗୁଣ,OpAdd<ଦ୍ୱିଗୁଣ> >
#define reduceSumC16s64f reduceC_<short, ଦ୍ୱିଗୁଣ,OpAdd<ଦ୍ୱିଗୁଣ> >
#define reduceSumC32f64f reduceC_<ଦଶମିକ, ଦ୍ୱିଗୁଣ,OpAdd<ଦ୍ୱିଗୁଣ> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, ଦ୍ୱିଗୁଣ,OpAddSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>,   OpSqr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> >
#define reduceSum2C16u64f reduceC_<ushort,ଦ୍ୱିଗୁଣ,OpAddSqr<ଦ୍ୱିଗୁଣ>,OpSqr<ଦ୍ୱିଗୁଣ> >
#define reduceSum2C16s64f reduceC_<short, ଦ୍ୱିଗୁଣ,OpAddSqr<ଦ୍ୱିଗୁଣ>,OpSqr<ଦ୍ୱିଗୁଣ> >
#define reduceSum2C32f64f reduceC_<ଦଶମିକ, ଦ୍ୱିଗୁଣ,OpAddSqr<ଦ୍ୱିଗୁଣ>,OpSqr<ଦ୍ୱିଗୁଣ> >

#ଯଦି defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ ipp_reduce##optype##C##favor(ଅଚଳ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    ଯଦି((srcmat.channels() == 1)) \
    { \
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sstep = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)srcmat.step; \
        ପ୍ରକାରବ୍ୟାଖ୍ୟା Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < srcmat.size().height; y++)\
        {\
            ଯଦି(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                ଫେରନ୍ତୁ ମିଥ୍ୟା;\
        }\
        ଫେରନ୍ତୁ ସତ୍ୟ;\
    }\
    ଫେରନ୍ତୁ ମିଥ୍ୟା; \
} \
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ reduce##optype##C##favor(ଅଚଳ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#ଯଦି defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, ଦଶମିକ, ଦଶମିକ)
#ଅନ୍ୟଥା
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<ଦଶମିକ, ଦଶମିକ, OpMax<ଦଶମିକ> >
#endif
#define reduceMaxC64f reduceC_<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ,OpMax<ଦ୍ୱିଗୁଣ> >

#ଯଦି defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, ଦଶମିକ, ଦଶମିକ)
#ଅନ୍ୟଥା
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<ଦଶମିକ, ଦଶମିକ, OpMin<ଦଶମିକ> >
#endif
#define reduceMinC64f reduceC_<ଦ୍ୱିଗୁଣ,ଦ୍ୱିଗୁଣ,OpMin<ଦ୍ୱିଗୁଣ> >

#ifdef HAVE_OPENCL

ନାମାକାଶ cv {

ସ୍ଥିର ତାର୍କିକ ocl_reduce(ନିବେଶ_Array _src, OutputArray _dst,
                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା dim, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_opt_cols = 128, buf_cols = 32;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    ଅଚଳ ocl::Device &defDev = ocl::Device::getDefault();
    ତାର୍କିକ doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    ତାର୍କିକ useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    ଯଦି (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (op == REDUCE_AVG)
    {
        ଯଦି (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ଅଚଳ ଅକ୍ଷର * ଅଚଳ ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wdepth = std::max(ddepth, CV_32F);
    ଯଦି (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        ଯଦି (defDev.isIntel())
        {
            ସ୍ଥିର ଅଚଳ size_t max_Item_In_Group_ଗଣନା = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_ଗଣନା);
        }
        ଅକ୍ଷର cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], ଆକାର(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], ଆକାର(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], ଆକାର(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        ଯଦି (k.empty())
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ଯଦି (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        ଅନ୍ୟଥା
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        ଫେରନ୍ତୁ k.run(2, globalSize, localSize, ମିଥ୍ୟା);
    }
    ଅନ୍ୟଥା
    {
        ଅକ୍ଷର cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], ଆକାର(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], ଆକାର(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], ଆକାର(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        ଯଦି (k.empty())
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        ଯଦି (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        ଅନ୍ୟଥା
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        ଫେରନ୍ତୁ k.run(1, &globalsize, ଖାଲି, ମିଥ୍ୟା);
    }
}

}

#endif

ଶୂନ୍ୟ cv::reduce(ନିବେଶ_Array _src, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dim, ପୂର୍ଣ୍ଣସଂଖ୍ୟା op, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype)
{
    CV_INSTRUMENT_REGION();

    ଯଦି (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, ସତ୍ୟ, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        ଫେରନ୍ତୁ;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା op0 = op;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    ଯଦି( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    ଯଦି (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    ଯଦି (src.empty()) {
        src.convertTo(_dst, dtype);
        ଫେରନ୍ତୁ;
    }
    ଯଦି (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    ଯଦି( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        ଯଦି( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    ଯଦି( dim == 0 )
    {
        ଯଦି( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            ଯଦି(simd_func)
                func = (ReduceFunc)simd_func;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        ଅନ୍ୟଥା ଯଦି(op == REDUCE_MAX)
        {
            ଯଦି(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        ଅନ୍ୟଥା ଯଦି(op == REDUCE_MIN)
        {
            ଯଦି(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        ଅନ୍ୟଥା ଯଦି( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            ଯଦି(simd_func)
                func = simd_func;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    ଅନ୍ୟଥା
    {
        ଯଦି(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            ଯଦି(simd_func)
                func = (ReduceFunc)simd_func;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        ଅନ୍ୟଥା ଯଦି(op == REDUCE_MAX)
        {
            ଯଦି(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        ଅନ୍ୟଥା ଯଦି(op == REDUCE_MIN)
        {
            ଯଦି(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        ଅନ୍ୟଥା ଯଦି(op == REDUCE_SUM2)
        {
            ଯଦି(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            ଅନ୍ୟଥା ଯଦି(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    ଯଦି( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    ଯଦି( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

ନାମାକାଶ cv
{

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ sort_( ଅଚଳ Mat& src, Mat& dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, len;
    ତାର୍କିକ sortRows = (flags & 1) == SORT_EVERY_ROW;
    ତାର୍କିକ inplace = src.data == dst.data;
    ତାର୍କିକ sortDescending = (flags & SORT_DESCENDING) != 0;

    ଯଦି( sortRows )
        n = src.rows, len = src.cols;
    ଅନ୍ୟଥା
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](ଅଚଳ Range& range)
    {
        AutoBuffer<T> buf;
        ଯଦି( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            ଯଦି( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                ଯଦି( !inplace )
                {
                    ଅଚଳ T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, ଆକାର(T) * len);
                }
                ptr = dptr;
            }
            ଅନ୍ୟଥା
            {
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            ଯଦି( sortDescending )
            {
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            ଯଦି( !sortRows )
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#ଯଦି defined(HAVE_IPP) && !IPP_DISABLE_SORT
ପ୍ରକାରବ୍ୟାଖ୍ୟା IppStatus (CV_STDCALL *IppSortFunc)(ଶୂନ୍ୟ  *pSrcDst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, Ipp8u*)>
ସ୍ଥିର IppStatus CV_STDCALL ippSortWrap(ଶୂନ୍ୟ* pSrcDst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, Ipp8u* pBuffer)
{
    ଫେରନ୍ତୁ fn((T*)pSrcDst, len, pBuffer);
}

ସ୍ଥିର IppSortFunc getSortFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ତାର୍କିକ sortDescending)
{
    ଯଦି (!sortDescending)
        ଫେରନ୍ତୁ depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

ସ୍ଥିର ତାର୍କିକ ipp_sort(ଅଚଳ Mat& src, Mat& dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    CV_INSTRUMENT_REGION_IPP();

    ତାର୍କିକ        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    ତାର୍କିକ        sortDescending  = (flags & SORT_DESCENDING) != 0;
    ତାର୍କିକ        inplace         = (src.data == dst.data);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    ଯଦି(!ippsSortRadix_I)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, len;
    ଯଦି( sortRows )
        n = src.rows, len = src.cols;
    ଅନ୍ୟଥା
        n = src.cols, len = src.rows;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା bufferSize;
    ଯଦି(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    ଅସ୍ଥିର ତାର୍କିକ ok = ସତ୍ୟ;
    parallel_for_(Range(0, n), [&](ଅଚଳ Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        ଯଦି( !sortRows )
            row.create(1, src.rows, src.type());

        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = range.start; i < range.end; i++ )
        {
            ଯଦି( !ok )
                ଭାଙ୍ଗ;
            ଶୂନ୍ୟ* ptr;
            ଯଦି( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                ଯଦି( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            ଅନ୍ୟଥା
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            ଯଦି( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = ମିଥ୍ୟା;
                ଭାଙ୍ଗ;
            }

            ଯଦି( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    ଫେରନ୍ତୁ ok;
}
#endif

template<typename _Tp> ଶ୍ରେଣୀ LessThanIdx
{
ସର୍ବସାଧାରଣ:
    LessThanIdx( ଅଚଳ _Tp* _arr ) : arr(_arr) {}
    ତାର୍କିକ operator()(ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b) ଅଚଳ { ଫେରନ୍ତୁ arr[a] < arr[b]; }
    ଅଚଳ _Tp* arr;
};

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ sortIdx_( ଅଚଳ Mat& src, Mat& dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ibuf;
    ତାର୍କିକ sortRows = (flags & 1) == SORT_EVERY_ROW;
    ତାର୍କିକ sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, len;
    ଯଦି( sortRows )
        n = src.rows, len = src.cols;
    ଅନ୍ୟଥା
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _iptr = ibuf.data();

    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା* iptr = _iptr;

        ଯଦି( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i);
        }
        ଅନ୍ୟଥା
        {
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        ଯଦି( sortDescending )
        {
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        ଯଦି( !sortRows )
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < len; j++ )
                dst.ptr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(j)[i] = iptr[j];
    }
}

#ଯଦି defined(HAVE_IPP) && !IPP_DISABLE_SORT
ପ୍ରକାରବ୍ୟାଖ୍ୟା IppStatus (CV_STDCALL *IppSortIndexFunc)(ଅଚଳ ଶୂନ୍ୟ*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(ଅଚଳ T*, Ipp32s, Ipp32s*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, Ipp8u*)>
ସ୍ଥିର IppStatus CV_STDCALL ippSortIndexWrap(ଅଚଳ ଶୂନ୍ୟ* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, Ipp8u *pBuffer)
{
    ଫେରନ୍ତୁ fn((ଅଚଳ T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

ସ୍ଥିର IppSortIndexFunc getSortIndexFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ତାର୍କିକ sortDescending)
{
    ଯଦି (!sortDescending)
        ଫେରନ୍ତୁ depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

ସ୍ଥିର ତାର୍କିକ ipp_sortIdx( ଅଚଳ Mat& src, Mat& dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    CV_INSTRUMENT_REGION_IPP();

    ତାର୍କିକ        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    ତାର୍କିକ        sortDescending  = (flags & SORT_DESCENDING) != 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    ଯଦି(!ippsSortRadixIndex)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା               bufferSize;
        ଯଦି(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        buffer.allocate(bufferSize);

        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < src.rows; i++)
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ଅଚଳ ଶୂନ୍ୟ*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
    }
    ଅନ୍ୟଥା
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା               bufferSize;
        ଯଦି(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            ଯଦି(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ଅଚଳ ଶୂନ୍ୟ*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*SortFunc)(ଅଚଳ Mat& src, Mat& dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
}

ଶୂନ୍ୟ cv::sort( ନିବେଶ_Array _src, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#ଯଦି !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    ସ୍ଥିର SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, sort_<ଦଶମିକ>, sort_<ଦ୍ୱିଗୁଣ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

ଶୂନ୍ୟ cv::sortIdx( ନିବେଶ_Array _src, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    ଯଦି( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#ଯଦି !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    ସ୍ଥିର SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, sortIdx_<ଦଶମିକ>, sortIdx_<ଦ୍ୱିଗୁଣ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
