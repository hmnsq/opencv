// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

ಶೂನ್ಯ cv::swap( Mat& a, Mat& b )
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


ಶೂನ್ಯ cv::hconcat(ಧ್ರುವ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( nsrc == 0 || !src )
    {
        _dst.release();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪೂರ್ಣಾಙ್ಕ totalCols = 0, cols = 0;
    ಪ್ರತಿ( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    ಪ್ರತಿ( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

ಶೂನ್ಯ cv::hconcat(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

ಶೂನ್ಯ cv::hconcat(ಒಳಹರಿವು_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

ಶೂನ್ಯ cv::vconcat(ಧ್ರುವ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    ಯದಿ( nsrc == 0 || !src )
    {
        _dst.release();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಪೂರ್ಣಾಙ್ಕ totalRows = 0, rows = 0;
    ಪ್ರತಿ( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    ಪ್ರತಿ( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

ಶೂನ್ಯ cv::vconcat(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

ಶೂನ್ಯ cv::vconcat(ಒಳಹರಿವು_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_setIdentity( ಒಳಹರಿವು_Output_Array _m, ಧ್ರುವ Scalar& s )
{
    ಪೂರ್ಣಾಙ್ಕ type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    ಪೂರ್ಣಾಙ್ಕ sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    ಯದಿ (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        ಯದಿ (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            ಯದಿ (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, nullptr, ಅಸತ್ಯ);
}

}

#endif

ಶೂನ್ಯ cv::setIdentity( ಒಳಹರಿವು_Output_Array _m, ಧ್ರುವ Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    ಪೂರ್ಣಾಙ್ಕ rows = m.rows, cols = m.cols, type = m.type();

    ಯದಿ( type == CV_32FC1 )
    {
        ಭಿನ್ನ* data = m.ptr<ಭಿನ್ನ>();
        ಭಿನ್ನ val = (ಭಿನ್ನ)s[0];
        size_t step = m.step/ಪರಿಮಾಣ(data[0]);

        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < rows; i++, data += step )
        {
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < cols; j++ )
                data[j] = 0;
            ಯದಿ( i < cols )
                data[i] = val;
        }
    }
    ಅನ್ಯಥಾ ಯದಿ( type == CV_64FC1 )
    {
        ದ್ವಿಗುಣ* data = m.ptr<ದ್ವಿಗುಣ>();
        ದ್ವಿಗುಣ val = s[0];
        size_t step = m.step/ಪರಿಮಾಣ(data[0]);

        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            ಯದಿ (i < cols)
                data[i] = val;
        }
    }
    ಅನ್ಯಥಾ
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


ನಾಮಾಕಾಶ cv {

UMat UMat::eye(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    ಪ್ರತ್ಯಾಗಮ UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, ಪೂರ್ಣಾಙ್ಕ type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    ಪ್ರತ್ಯಾಗಮ m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( ಒಳಹರಿವು_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    ಪೂರ್ಣಾಙ್ಕ type = m.type();
    ಪೂರ್ಣಾಙ್ಕ nm = std::min(m.rows, m.cols);

    ಯದಿ( type == CV_32FC1 )
    {
        ಧ್ರುವ ಭಿನ್ನ* ptr = m.ptr<ಭಿನ್ನ>();
        size_t step = m.step/ಪರಿಮಾಣ(ptr[0]) + 1;
        ದ್ವಿಗುಣ _s = 0;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nm; i++ )
            _s += ptr[i*step];
        ಪ್ರತ್ಯಾಗಮ _s;
    }

    ಯದಿ( type == CV_64FC1 )
    {
        ಧ್ರುವ ದ್ವಿಗುಣ* ptr = m.ptr<ದ್ವಿಗುಣ>();
        size_t step = m.step/ಪರಿಮಾಣ(ptr[0]) + 1;
        ದ್ವಿಗುಣ _s = 0;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nm; i++ )
            _s += ptr[i*step];
        ಪ್ರತ್ಯಾಗಮ _s;
    }

    ಪ್ರತ್ಯಾಗಮ cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

ಶೂನ್ಯ cv::completeSymm( ಒಳಹರಿವು_Output_Array _m, ತಾರ್ಕಿಕ LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    ಪೂರ್ಣಾಙ್ಕ rows = m.rows;
    ಪೂರ್ಣಾಙ್ಕ j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < rows; i++ )
    {
        ಯದಿ( !LtoR ) j1 = i; ಅನ್ಯಥಾ j0 = i+1;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(ಒಳಹರಿವು_Array _m) ಧ್ರುವ
{
    Mat m = _m.getMat();
    ಪೂರ್ಣಾಙ್ಕ tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    ಯದಿ( d == CV_32F )
    {
        ಧ್ರುವ ಭಿನ್ನ *a = (ಧ್ರುವ ಭಿನ್ನ*)data, *b = (ಧ್ರುವ ಭಿನ್ನ*)m.data;
        ಭಿನ್ನ* c = (ಭಿನ್ನ*)result.data;
        size_t lda = rows > 1 ? step/ಪರಿಮಾಣ(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/ಪರಿಮಾಣ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    ಅನ್ಯಥಾ ಯದಿ( d == CV_64F )
    {
        ಧ್ರುವ ದ್ವಿಗುಣ *a = (ಧ್ರುವ ದ್ವಿಗುಣ*)data, *b = (ಧ್ರುವ ದ್ವಿಗುಣ*)m.data;
        ದ್ವಿಗುಣ* c = (ದ್ವಿಗುಣ*)result.data;
        size_t lda = rows > 1 ? step/ಪರಿಮಾಣ(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/ಪರಿಮಾಣ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    ಪ್ರತ್ಯಾಗಮ result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

ನಾಮಾಕಾಶ cv
{

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ReduceSumFunc)(ಧ್ರುವ Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth);
ReduceSumFunc getReduceRSumFunc(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth);
ReduceSumFunc getReduceRSum2Func(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth);

template <typename T, typename WT, typename Op>
ಸಂರಚನಾ ReduceR_SIMD
{
    ಪೂರ್ಣಾಙ್ಕ operator()(ಧ್ರುವ T*, ಪೂರ್ಣಾಙ್ಕ start, ಪೂರ್ಣಾಙ್ಕ, WT*, ಧ್ರುವ Op&) ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ start;
    }
};


template<typename T, typename ST, typename WT, ವರ್ಗ Op, ವರ್ಗ OpInit>
ವರ್ಗ ReduceR_Invoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
  ReduceR_Invoker(ಧ್ರುವ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  ಶೂನ್ಯ operator()(ಧ್ರುವ Range& range) ಧ್ರುವ CV_OVERRIDE
  {
    ಧ್ರುವ T* src = srcmat.ptr<T>();
    ಧ್ರುವ size_t srcstep = srcmat.step/ಪರಿಮಾಣ(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    ಪೂರ್ಣಾಙ್ಕ i = 0;

    ಪ್ರತಿ( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    ಪೂರ್ಣಾಙ್ಕ height = srcmat.size().height;
    ಪ್ರತಿ( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #ಯದಿ CV_ENABLE_UNROLLED
        ಪ್ರತಿ(; i <= range.end - 4; i += 4 )
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
        ಪ್ರತಿ( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    ಪ್ರತಿ( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
ಗುಪ್ತ:
  ಧ್ರುವ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, ವರ್ಗ Op, ವರ್ಗ OpInit = OpNop<ST> > ಸ್ಥಿರ ಶೂನ್ಯ
reduceR_( ಧ್ರುವ Mat& srcmat, Mat& dstmat)
{
    ಪ್ರಕಾರನಿರ್ದೇಶ typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, ವರ್ಗ Op, ವರ್ಗ OpInit>
ವರ್ಗ ReduceC_Invoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
  ReduceC_Invoker(ಧ್ರುವ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  ಶೂನ್ಯ operator()(ಧ್ರುವ Range& range) ಧ್ರುವ CV_OVERRIDE
  {
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = srcmat.channels();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ y = range.start; y < range.end; y++ )
    {
        ಧ್ರುವ T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        ಯದಿ( width == cn )
        {
          ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        ಅನ್ಯಥಾ
        {
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ k = cn ; k < width ; k += cn )
            {
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
ಗುಪ್ತ:
  ಧ್ರುವ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, ವರ್ಗ Op, ವರ್ಗ OpInit = OpNop<ST> > ಸ್ಥಿರ ಶೂನ್ಯ
reduceC_( ಧ್ರುವ Mat& srcmat, Mat& dstmat)
{
    ಪ್ರಕಾರನಿರ್ದೇಶ typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ReduceFunc)( ಧ್ರುವ Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, ಪೂರ್ಣಾಙ್ಕ,   OpAdd<ಪೂರ್ಣಾಙ್ಕ>, OpNop<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSumR8u32f  reduceR_<uchar, ಭಿನ್ನ, OpAdd<ಪೂರ್ಣಾಙ್ಕ>, OpNop<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSumR8u64f  reduceR_<uchar, ದ್ವಿಗುಣ,OpAdd<ಪೂರ್ಣಾಙ್ಕ>, OpNop<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSumR16u32f reduceR_<ushort,ಭಿನ್ನ, OpAdd<ಭಿನ್ನ> >
#define reduceSumR16u64f reduceR_<ushort,ದ್ವಿಗುಣ,OpAdd<ದ್ವಿಗುಣ> >
#define reduceSumR16s32f reduceR_<short, ಭಿನ್ನ, OpAdd<ಭಿನ್ನ> >
#define reduceSumR16s64f reduceR_<short, ದ್ವಿಗುಣ,OpAdd<ದ್ವಿಗುಣ> >
#define reduceSumR32f32f reduceR_<ಭಿನ್ನ, ಭಿನ್ನ, OpAdd<ಭಿನ್ನ> >
#define reduceSumR32f64f reduceR_<ಭಿನ್ನ, ದ್ವಿಗುಣ,OpAdd<ದ್ವಿಗುಣ> >
#define reduceSumR64f64f reduceR_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpAdd<ದ್ವಿಗುಣ> >

#define reduceSum2R8u32s  reduceR_<uchar, ಪೂರ್ಣಾಙ್ಕ,   OpAddSqr<ಪೂರ್ಣಾಙ್ಕ>,   OpSqr<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSum2R8u32f  reduceR_<uchar, ಭಿನ್ನ, OpAddSqr<ಪೂರ್ಣಾಙ್ಕ>,   OpSqr<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSum2R8u64f  reduceR_<uchar, ದ್ವಿಗುಣ,OpAddSqr<ಪೂರ್ಣಾಙ್ಕ>,   OpSqr<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSum2R16u32f reduceR_<ushort,ಭಿನ್ನ, OpAddSqr<ಭಿನ್ನ>, OpSqr<ಭಿನ್ನ> >
#define reduceSum2R16u64f reduceR_<ushort,ದ್ವಿಗುಣ,OpAddSqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#define reduceSum2R16s32f reduceR_<short, ಭಿನ್ನ, OpAddSqr<ಭಿನ್ನ>, OpSqr<ಭಿನ್ನ> >
#define reduceSum2R16s64f reduceR_<short, ದ್ವಿಗುಣ,OpAddSqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#define reduceSum2R32f32f reduceR_<ಭಿನ್ನ, ಭಿನ್ನ, OpAddSqr<ಭಿನ್ನ>, OpSqr<ಭಿನ್ನ> >
#define reduceSum2R32f64f reduceR_<ಭಿನ್ನ, ದ್ವಿಗುಣ,OpAddSqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#define reduceSum2R64f64f reduceR_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpAddSqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<ಭಿನ್ನ, ಭಿನ್ನ, OpMax<ಭಿನ್ನ> >
#define reduceMaxR64f reduceR_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpMax<ದ್ವಿಗುಣ> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<ಭಿನ್ನ, ಭಿನ್ನ, OpMin<ಭಿನ್ನ> >
#define reduceMinR64f reduceR_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpMin<ದ್ವಿಗುಣ> >

#ಯದಿ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ipp_reduceSumC_8u16u16s32f_64f(ಧ್ರುವ cv::Mat& srcmat, cv::Mat& dstmat)
{
    ಪೂರ್ಣಾಙ್ಕ sstep = (ಪೂರ್ಣಾಙ್ಕ)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    ಪ್ರಕಾರನಿರ್ದೇಶ IppStatus (CV_STDCALL * IppiSum)(ಧ್ರುವ ಶೂನ್ಯ * pSrc, ಪೂರ್ಣಾಙ್ಕ srcStep, IppiSize roiSize, Ipp64f* pSum);
    ಪ್ರಕಾರನಿರ್ದೇಶ IppStatus (CV_STDCALL * IppiSumHint)(ಧ್ರುವ ಶೂನ್ಯ * pSrc, ಪೂರ್ಣಾಙ್ಕ srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    ಯದಿ(ddepth == CV_64F)
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

    ಯದಿ(ippiSum)
    {
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ y = 0; y < srcmat.size().height; y++)
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಅನ್ಯಥಾ ಯದಿ(ippiSumHint)
    {
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ y = 0; y < srcmat.size().height; y++)
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ reduceSumC_8u16u16s32f_64f(ಧ್ರುವ cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    ಯದಿ(dstmat.depth() == CV_64F)
    {
        ಪೂರ್ಣಾಙ್ಕ sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, ದ್ವಿಗುಣ,   cv::OpAdd<ದ್ವಿಗುಣ> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, ದ್ವಿಗುಣ,   cv::OpAdd<ದ್ವಿಗುಣ> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, ದ್ವಿಗುಣ,   cv::OpAdd<ದ್ವಿಗುಣ> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<ಭಿನ್ನ, ದ್ವಿಗುಣ,   cv::OpAdd<ದ್ವಿಗುಣ> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, ಪೂರ್ಣಾಙ್ಕ,   OpAdd<ಪೂರ್ಣಾಙ್ಕ>, OpNop<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSumC8u32f  reduceC_<uchar, ಭಿನ್ನ, OpAdd<ಪೂರ್ಣಾಙ್ಕ>, OpNop<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSumC16u32f reduceC_<ushort,ಭಿನ್ನ, OpAdd<ಭಿನ್ನ> >
#define reduceSumC16s32f reduceC_<short, ಭಿನ್ನ, OpAdd<ಭಿನ್ನ> >
#define reduceSumC32f32f reduceC_<ಭಿನ್ನ, ಭಿನ್ನ, OpAdd<ಭಿನ್ನ> >
#define reduceSumC64f64f reduceC_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpAdd<ದ್ವಿಗುಣ> >

#define reduceSum2C8u32s  reduceC_<uchar, ಪೂರ್ಣಾಙ್ಕ,   OpAddSqr<ಪೂರ್ಣಾಙ್ಕ>,   OpSqr<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSum2C8u32f  reduceC_<uchar, ಭಿನ್ನ, OpAddSqr<ಪೂರ್ಣಾಙ್ಕ>,   OpSqr<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSum2C16u32f reduceC_<ushort,ಭಿನ್ನ, OpAddSqr<ಭಿನ್ನ>, OpSqr<ಭಿನ್ನ> >
#define reduceSum2C16s32f reduceC_<short, ಭಿನ್ನ, OpAddSqr<ಭಿನ್ನ>, OpSqr<ಭಿನ್ನ> >
#define reduceSum2C32f32f reduceC_<ಭಿನ್ನ, ಭಿನ್ನ, OpAddSqr<ಭಿನ್ನ>, OpSqr<ಭಿನ್ನ> >
#define reduceSum2C64f64f reduceC_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpAddSqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >

#ಯದಿ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#ಅನ್ಯಥಾ
#define reduceSumC8u64f  reduceC_<uchar, ದ್ವಿಗುಣ,OpAdd<ಪೂರ್ಣಾಙ್ಕ>, OpNop<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSumC16u64f reduceC_<ushort,ದ್ವಿಗುಣ,OpAdd<ದ್ವಿಗುಣ> >
#define reduceSumC16s64f reduceC_<short, ದ್ವಿಗುಣ,OpAdd<ದ್ವಿಗುಣ> >
#define reduceSumC32f64f reduceC_<ಭಿನ್ನ, ದ್ವಿಗುಣ,OpAdd<ದ್ವಿಗುಣ> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, ದ್ವಿಗುಣ,OpAddSqr<ಪೂರ್ಣಾಙ್ಕ>,   OpSqr<ಪೂರ್ಣಾಙ್ಕ> >
#define reduceSum2C16u64f reduceC_<ushort,ದ್ವಿಗುಣ,OpAddSqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#define reduceSum2C16s64f reduceC_<short, ದ್ವಿಗುಣ,OpAddSqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >
#define reduceSum2C32f64f reduceC_<ಭಿನ್ನ, ದ್ವಿಗುಣ,OpAddSqr<ದ್ವಿಗುಣ>,OpSqr<ದ್ವಿಗುಣ> >

#ಯದಿ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ipp_reduce##optype##C##favor(ಧ್ರುವ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    ಯದಿ((srcmat.channels() == 1)) \
    { \
        ಪೂರ್ಣಾಙ್ಕ sstep = (ಪೂರ್ಣಾಙ್ಕ)srcmat.step; \
        ಪ್ರಕಾರನಿರ್ದೇಶ Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ y = 0; y < srcmat.size().height; y++)\
        {\
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;\
        }\
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;\
    }\
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; \
} \
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ reduce##optype##C##favor(ಧ್ರುವ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#ಯದಿ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, ಭಿನ್ನ, ಭಿನ್ನ)
#ಅನ್ಯಥಾ
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<ಭಿನ್ನ, ಭಿನ್ನ, OpMax<ಭಿನ್ನ> >
#endif
#define reduceMaxC64f reduceC_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpMax<ದ್ವಿಗುಣ> >

#ಯದಿ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, ಭಿನ್ನ, ಭಿನ್ನ)
#ಅನ್ಯಥಾ
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<ಭಿನ್ನ, ಭಿನ್ನ, OpMin<ಭಿನ್ನ> >
#endif
#define reduceMinC64f reduceC_<ದ್ವಿಗುಣ,ದ್ವಿಗುಣ,OpMin<ದ್ವಿಗುಣ> >

#ifdef HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_reduce(ಒಳಹರಿವು_Array _src, OutputArray _dst,
                       ಪೂರ್ಣಾಙ್ಕ dim, ಪೂರ್ಣಾಙ್ಕ op, ಪೂರ್ಣಾಙ್ಕ op0, ಪೂರ್ಣಾಙ್ಕ stype, ಪೂರ್ಣಾಙ್ಕ dtype)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ min_opt_cols = 128, buf_cols = 32;
    ಪೂರ್ಣಾಙ್ಕ sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    ಧ್ರುವ ocl::Device &defDev = ocl::Device::getDefault();
    ತಾರ್ಕಿಕ doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    ತಾರ್ಕಿಕ useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    ಯದಿ (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (op == REDUCE_AVG)
    {
        ಯದಿ (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    ಪೂರ್ಣಾಙ್ಕ wdepth = std::max(ddepth, CV_32F);
    ಯದಿ (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        ಯದಿ (defDev.isIntel())
        {
            ಸ್ಥಿರ ಧ್ರುವ size_t max_Item_In_Group_ಎಣಿಕೆ = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_ಎಣಿಕೆ);
        }
        ವರ್ಣ cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], ಪರಿಮಾಣ(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], ಪರಿಮಾಣ(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], ಪರಿಮಾಣ(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        ಯದಿ (k.empty())
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ಯದಿ (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        ಅನ್ಯಥಾ
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        ಪ್ರತ್ಯಾಗಮ k.run(2, globalSize, localSize, ಅಸತ್ಯ);
    }
    ಅನ್ಯಥಾ
    {
        ವರ್ಣ cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], ಪರಿಮಾಣ(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], ಪರಿಮಾಣ(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], ಪರಿಮಾಣ(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        ಯದಿ (k.empty())
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        ಯದಿ (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        ಅನ್ಯಥಾ
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        ಪ್ರತ್ಯಾಗಮ k.run(1, &globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
    }
}

}

#endif

ಶೂನ್ಯ cv::reduce(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ dim, ಪೂರ್ಣಾಙ್ಕ op, ಪೂರ್ಣಾಙ್ಕ dtype)
{
    CV_INSTRUMENT_REGION();

    ಯದಿ (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, ಸತ್ಯ, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಪೂರ್ಣಾಙ್ಕ op0 = op;
    ಪೂರ್ಣಾಙ್ಕ stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    ಯದಿ( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    ಪೂರ್ಣಾಙ್ಕ ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    ಯದಿ (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    ಯದಿ (src.empty()) {
        src.convertTo(_dst, dtype);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    ಯದಿ( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        ಯದಿ( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    ಯದಿ( dim == 0 )
    {
        ಯದಿ( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            ಯದಿ(simd_func)
                func = (ReduceFunc)simd_func;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        ಅನ್ಯಥಾ ಯದಿ(op == REDUCE_MAX)
        {
            ಯದಿ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        ಅನ್ಯಥಾ ಯದಿ(op == REDUCE_MIN)
        {
            ಯದಿ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        ಅನ್ಯಥಾ ಯದಿ( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            ಯದಿ(simd_func)
                func = simd_func;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            ಯದಿ(simd_func)
                func = (ReduceFunc)simd_func;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        ಅನ್ಯಥಾ ಯದಿ(op == REDUCE_MAX)
        {
            ಯದಿ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        ಅನ್ಯಥಾ ಯದಿ(op == REDUCE_MIN)
        {
            ಯದಿ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        ಅನ್ಯಥಾ ಯದಿ(op == REDUCE_SUM2)
        {
            ಯದಿ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            ಅನ್ಯಥಾ ಯದಿ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    ಯದಿ( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    ಯದಿ( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

ನಾಮಾಕಾಶ cv
{

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ sort_( ಧ್ರುವ Mat& src, Mat& dst, ಪೂರ್ಣಾಙ್ಕ flags )
{
    ಪೂರ್ಣಾಙ್ಕ n, len;
    ತಾರ್ಕಿಕ sortRows = (flags & 1) == SORT_EVERY_ROW;
    ತಾರ್ಕಿಕ inplace = src.data == dst.data;
    ತಾರ್ಕಿಕ sortDescending = (flags & SORT_DESCENDING) != 0;

    ಯದಿ( sortRows )
        n = src.rows, len = src.cols;
    ಅನ್ಯಥಾ
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](ಧ್ರುವ Range& range)
    {
        AutoBuffer<T> buf;
        ಯದಿ( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            ಯದಿ( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                ಯದಿ( !inplace )
                {
                    ಧ್ರುವ T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, ಪರಿಮಾಣ(T) * len);
                }
                ptr = dptr;
            }
            ಅನ್ಯಥಾ
            {
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            ಯದಿ( sortDescending )
            {
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            ಯದಿ( !sortRows )
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#ಯದಿ defined(HAVE_IPP) && !IPP_DISABLE_SORT
ಪ್ರಕಾರನಿರ್ದೇಶ IppStatus (CV_STDCALL *IppSortFunc)(ಶೂನ್ಯ  *pSrcDst, ಪೂರ್ಣಾಙ್ಕ    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, ಪೂರ್ಣಾಙ್ಕ, Ipp8u*)>
ಸ್ಥಿರ IppStatus CV_STDCALL ippSortWrap(ಶೂನ್ಯ* pSrcDst, ಪೂರ್ಣಾಙ್ಕ len, Ipp8u* pBuffer)
{
    ಪ್ರತ್ಯಾಗಮ fn((T*)pSrcDst, len, pBuffer);
}

ಸ್ಥಿರ IppSortFunc getSortFunc(ಪೂರ್ಣಾಙ್ಕ depth, ತಾರ್ಕಿಕ sortDescending)
{
    ಯದಿ (!sortDescending)
        ಪ್ರತ್ಯಾಗಮ depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_sort(ಧ್ರುವ Mat& src, Mat& dst, ಪೂರ್ಣಾಙ್ಕ flags)
{
    CV_INSTRUMENT_REGION_IPP();

    ತಾರ್ಕಿಕ        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    ತಾರ್ಕಿಕ        sortDescending  = (flags & SORT_DESCENDING) != 0;
    ತಾರ್ಕಿಕ        inplace         = (src.data == dst.data);
    ಪೂರ್ಣಾಙ್ಕ         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    ಯದಿ(!ippsSortRadix_I)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ n, len;
    ಯದಿ( sortRows )
        n = src.rows, len = src.cols;
    ಅನ್ಯಥಾ
        n = src.cols, len = src.rows;

    ಪೂರ್ಣಾಙ್ಕ bufferSize;
    ಯದಿ(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    ಚಞ್ಚಲ ತಾರ್ಕಿಕ ok = ಸತ್ಯ;
    parallel_for_(Range(0, n), [&](ಧ್ರುವ Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        ಯದಿ( !sortRows )
            row.create(1, src.rows, src.type());

        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = range.start; i < range.end; i++ )
        {
            ಯದಿ( !ok )
                ಭಙ್ಗ;
            ಶೂನ್ಯ* ptr;
            ಯದಿ( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                ಯದಿ( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            ಅನ್ಯಥಾ
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            ಯದಿ( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = ಅಸತ್ಯ;
                ಭಙ್ಗ;
            }

            ಯದಿ( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    ಪ್ರತ್ಯಾಗಮ ok;
}
#endif

template<typename _Tp> ವರ್ಗ LessThanIdx
{
ಸಾರ್ವಜನಿಕ:
    LessThanIdx( ಧ್ರುವ _Tp* _arr ) : arr(_arr) {}
    ತಾರ್ಕಿಕ operator()(ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ arr[a] < arr[b]; }
    ಧ್ರುವ _Tp* arr;
};

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ sortIdx_( ಧ್ರುವ Mat& src, Mat& dst, ಪೂರ್ಣಾಙ್ಕ flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<ಪೂರ್ಣಾಙ್ಕ> ibuf;
    ತಾರ್ಕಿಕ sortRows = (flags & 1) == SORT_EVERY_ROW;
    ತಾರ್ಕಿಕ sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    ಪೂರ್ಣಾಙ್ಕ n, len;
    ಯದಿ( sortRows )
        n = src.rows, len = src.cols;
    ಅನ್ಯಥಾ
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    ಪೂರ್ಣಾಙ್ಕ* _iptr = ibuf.data();

    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        ಪೂರ್ಣಾಙ್ಕ* iptr = _iptr;

        ಯದಿ( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<ಪೂರ್ಣಾಙ್ಕ>(i);
        }
        ಅನ್ಯಥಾ
        {
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        ಯದಿ( sortDescending )
        {
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        ಯದಿ( !sortRows )
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < len; j++ )
                dst.ptr<ಪೂರ್ಣಾಙ್ಕ>(j)[i] = iptr[j];
    }
}

#ಯದಿ defined(HAVE_IPP) && !IPP_DISABLE_SORT
ಪ್ರಕಾರನಿರ್ದೇಶ IppStatus (CV_STDCALL *IppSortIndexFunc)(ಧ್ರುವ ಶೂನ್ಯ*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, ಪೂರ್ಣಾಙ್ಕ len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(ಧ್ರುವ T*, Ipp32s, Ipp32s*, ಪೂರ್ಣಾಙ್ಕ, Ipp8u*)>
ಸ್ಥಿರ IppStatus CV_STDCALL ippSortIndexWrap(ಧ್ರುವ ಶೂನ್ಯ* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, ಪೂರ್ಣಾಙ್ಕ len, Ipp8u *pBuffer)
{
    ಪ್ರತ್ಯಾಗಮ fn((ಧ್ರುವ T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

ಸ್ಥಿರ IppSortIndexFunc getSortIndexFunc(ಪೂರ್ಣಾಙ್ಕ depth, ತಾರ್ಕಿಕ sortDescending)
{
    ಯದಿ (!sortDescending)
        ಪ್ರತ್ಯಾಗಮ depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_sortIdx( ಧ್ರುವ Mat& src, Mat& dst, ಪೂರ್ಣಾಙ್ಕ flags )
{
    CV_INSTRUMENT_REGION_IPP();

    ತಾರ್ಕಿಕ        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    ತಾರ್ಕಿಕ        sortDescending  = (flags & SORT_DESCENDING) != 0;
    ಪೂರ್ಣಾಙ್ಕ         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    ಯದಿ(!ippsSortRadixIndex)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        ಪೂರ್ಣಾಙ್ಕ               bufferSize;
        ಯದಿ(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        buffer.allocate(bufferSize);

        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < src.rows; i++)
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ಧ್ರುವ ಶೂನ್ಯ*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
    }
    ಅನ್ಯಥಾ
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        ಪೂರ್ಣಾಙ್ಕ               bufferSize;
        ಯದಿ(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            ಯದಿ(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ಧ್ರುವ ಶೂನ್ಯ*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*SortFunc)(ಧ್ರುವ Mat& src, Mat& dst, ಪೂರ್ಣಾಙ್ಕ flags);
}

ಶೂನ್ಯ cv::sort( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#ಯದಿ !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    ಸ್ಥಿರ SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<ಪೂರ್ಣಾಙ್ಕ>, sort_<ಭಿನ್ನ>, sort_<ದ್ವಿಗುಣ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

ಶೂನ್ಯ cv::sortIdx( ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    ಯದಿ( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#ಯದಿ !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    ಸ್ಥಿರ SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<ಪೂರ್ಣಾಙ್ಕ>, sortIdx_<ಭಿನ್ನ>, sortIdx_<ದ್ವಿಗುಣ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
