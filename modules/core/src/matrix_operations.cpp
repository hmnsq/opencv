// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencv2/core/mat.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

ശൂന്യം cv::swap( Mat& a, Mat& b )
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


ശൂന്യം cv::hconcat(സ്ഥിരം Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( nsrc == 0 || !src )
    {
        _dst.release();
        തിരികെ;
    }

    പൂർണ്ണസംഖ്യ totalCols = 0, cols = 0;
    വേണ്ടി( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    വേണ്ടി( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

ശൂന്യം cv::hconcat(നിവേശം_Array src1, നിവേശം_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

ശൂന്യം cv::hconcat(നിവേശം_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

ശൂന്യം cv::vconcat(സ്ഥിരം Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    എങ്കിൽ( nsrc == 0 || !src )
    {
        _dst.release();
        തിരികെ;
    }

    പൂർണ്ണസംഖ്യ totalRows = 0, rows = 0;
    വേണ്ടി( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    വേണ്ടി( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

ശൂന്യം cv::vconcat(നിവേശം_Array src1, നിവേശം_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

ശൂന്യം cv::vconcat(നിവേശം_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

നാമാകാശ cv {

സ്ഥിതം തർക്കം ocl_setIdentity( നിവേശം_Output_Array _m, സ്ഥിരം Scalar& s )
{
    പൂർണ്ണസംഖ്യ type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    പൂർണ്ണസംഖ്യ sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    എങ്കിൽ (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        എങ്കിൽ (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            എങ്കിൽ (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, nullptr, അസത്യം);
}

}

#endif

ശൂന്യം cv::setIdentity( നിവേശം_Output_Array _m, സ്ഥിരം Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    പൂർണ്ണസംഖ്യ rows = m.rows, cols = m.cols, type = m.type();

    എങ്കിൽ( type == CV_32FC1 )
    {
        ദശാംശം* data = m.ptr<ദശാംശം>();
        ദശാംശം val = (ദശാംശം)s[0];
        size_t step = m.step/വലുപ്പം(data[0]);

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < rows; i++, data += step )
        {
            വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < cols; j++ )
                data[j] = 0;
            എങ്കിൽ( i < cols )
                data[i] = val;
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_64FC1 )
    {
        ഇരട്ട* data = m.ptr<ഇരട്ട>();
        ഇരട്ട val = s[0];
        size_t step = m.step/വലുപ്പം(data[0]);

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            എങ്കിൽ (i < cols)
                data[i] = val;
        }
    }
    അല്ലെങ്കിൽ
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


നാമാകാശ cv {

UMat UMat::eye(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    തിരികെ UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, പൂർണ്ണസംഖ്യ type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    തിരികെ m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( നിവേശം_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    പൂർണ്ണസംഖ്യ type = m.type();
    പൂർണ്ണസംഖ്യ nm = std::min(m.rows, m.cols);

    എങ്കിൽ( type == CV_32FC1 )
    {
        സ്ഥിരം ദശാംശം* ptr = m.ptr<ദശാംശം>();
        size_t step = m.step/വലുപ്പം(ptr[0]) + 1;
        ഇരട്ട _s = 0;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nm; i++ )
            _s += ptr[i*step];
        തിരികെ _s;
    }

    എങ്കിൽ( type == CV_64FC1 )
    {
        സ്ഥിരം ഇരട്ട* ptr = m.ptr<ഇരട്ട>();
        size_t step = m.step/വലുപ്പം(ptr[0]) + 1;
        ഇരട്ട _s = 0;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nm; i++ )
            _s += ptr[i*step];
        തിരികെ _s;
    }

    തിരികെ cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

ശൂന്യം cv::completeSymm( നിവേശം_Output_Array _m, തർക്കം LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    പൂർണ്ണസംഖ്യ rows = m.rows;
    പൂർണ്ണസംഖ്യ j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < rows; i++ )
    {
        എങ്കിൽ( !LtoR ) j1 = i; അല്ലെങ്കിൽ j0 = i+1;
        വേണ്ടി( പൂർണ്ണസംഖ്യ j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(നിവേശം_Array _m) സ്ഥിരം
{
    Mat m = _m.getMat();
    പൂർണ്ണസംഖ്യ tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    എങ്കിൽ( d == CV_32F )
    {
        സ്ഥിരം ദശാംശം *a = (സ്ഥിരം ദശാംശം*)data, *b = (സ്ഥിരം ദശാംശം*)m.data;
        ദശാംശം* c = (ദശാംശം*)result.data;
        size_t lda = rows > 1 ? step/വലുപ്പം(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/വലുപ്പം(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    അല്ലെങ്കിൽ എങ്കിൽ( d == CV_64F )
    {
        സ്ഥിരം ഇരട്ട *a = (സ്ഥിരം ഇരട്ട*)data, *b = (സ്ഥിരം ഇരട്ട*)m.data;
        ഇരട്ട* c = (ഇരട്ട*)result.data;
        size_t lda = rows > 1 ? step/വലുപ്പം(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/വലുപ്പം(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    തിരികെ result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

നാമാകാശ cv
{

തരനിർവചനം ശൂന്യം (*ReduceSumFunc)(സ്ഥിരം Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);
ReduceSumFunc getReduceRSumFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);
ReduceSumFunc getReduceRSum2Func(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);

template <typename T, typename WT, typename Op>
ഘടന ReduceR_SIMD
{
    പൂർണ്ണസംഖ്യ operator()(സ്ഥിരം T*, പൂർണ്ണസംഖ്യ start, പൂർണ്ണസംഖ്യ, WT*, സ്ഥിരം Op&) സ്ഥിരം
    {
        തിരികെ start;
    }
};


template<typename T, typename ST, typename WT, വർഗ്ഗം Op, വർഗ്ഗം OpInit>
വർഗ്ഗം ReduceR_Invoker : പൊതു ParallelLoopBody
{
പൊതു:
  ReduceR_Invoker(സ്ഥിരം Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  ശൂന്യം operator()(സ്ഥിരം Range& range) സ്ഥിരം CV_OVERRIDE
  {
    സ്ഥിരം T* src = srcmat.ptr<T>();
    സ്ഥിരം size_t srcstep = srcmat.step/വലുപ്പം(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    പൂർണ്ണസംഖ്യ i = 0;

    വേണ്ടി( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    പൂർണ്ണസംഖ്യ height = srcmat.size().height;
    വേണ്ടി( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #എങ്കിൽ CV_ENABLE_UNROLLED
        വേണ്ടി(; i <= range.end - 4; i += 4 )
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
        വേണ്ടി( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    വേണ്ടി( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
സ്വകാര്യ:
  സ്ഥിരം Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, വർഗ്ഗം Op, വർഗ്ഗം OpInit = OpNop<ST> > സ്ഥിതം ശൂന്യം
reduceR_( സ്ഥിരം Mat& srcmat, Mat& dstmat)
{
    തരനിർവചനം typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, വർഗ്ഗം Op, വർഗ്ഗം OpInit>
വർഗ്ഗം ReduceC_Invoker : പൊതു ParallelLoopBody
{
പൊതു:
  ReduceC_Invoker(സ്ഥിരം Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  ശൂന്യം operator()(സ്ഥിരം Range& range) സ്ഥിരം CV_OVERRIDE
  {
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = srcmat.channels();
    സ്ഥിരം പൂർണ്ണസംഖ്യ width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    വേണ്ടി( പൂർണ്ണസംഖ്യ y = range.start; y < range.end; y++ )
    {
        സ്ഥിരം T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        എങ്കിൽ( width == cn )
        {
          വേണ്ടി( പൂർണ്ണസംഖ്യ k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        അല്ലെങ്കിൽ
        {
            വേണ്ടി(പൂർണ്ണസംഖ്യ k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            വേണ്ടി(പൂർണ്ണസംഖ്യ k = cn ; k < width ; k += cn )
            {
                വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            വേണ്ടി(പൂർണ്ണസംഖ്യ k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
സ്വകാര്യ:
  സ്ഥിരം Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, വർഗ്ഗം Op, വർഗ്ഗം OpInit = OpNop<ST> > സ്ഥിതം ശൂന്യം
reduceC_( സ്ഥിരം Mat& srcmat, Mat& dstmat)
{
    തരനിർവചനം typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

തരനിർവചനം ശൂന്യം (*ReduceFunc)( സ്ഥിരം Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, പൂർണ്ണസംഖ്യ,   OpAdd<പൂർണ്ണസംഖ്യ>, OpNop<പൂർണ്ണസംഖ്യ> >
#define reduceSumR8u32f  reduceR_<uchar, ദശാംശം, OpAdd<പൂർണ്ണസംഖ്യ>, OpNop<പൂർണ്ണസംഖ്യ> >
#define reduceSumR8u64f  reduceR_<uchar, ഇരട്ട,OpAdd<പൂർണ്ണസംഖ്യ>, OpNop<പൂർണ്ണസംഖ്യ> >
#define reduceSumR16u32f reduceR_<ushort,ദശാംശം, OpAdd<ദശാംശം> >
#define reduceSumR16u64f reduceR_<ushort,ഇരട്ട,OpAdd<ഇരട്ട> >
#define reduceSumR16s32f reduceR_<short, ദശാംശം, OpAdd<ദശാംശം> >
#define reduceSumR16s64f reduceR_<short, ഇരട്ട,OpAdd<ഇരട്ട> >
#define reduceSumR32f32f reduceR_<ദശാംശം, ദശാംശം, OpAdd<ദശാംശം> >
#define reduceSumR32f64f reduceR_<ദശാംശം, ഇരട്ട,OpAdd<ഇരട്ട> >
#define reduceSumR64f64f reduceR_<ഇരട്ട,ഇരട്ട,OpAdd<ഇരട്ട> >

#define reduceSum2R8u32s  reduceR_<uchar, പൂർണ്ണസംഖ്യ,   OpAddSqr<പൂർണ്ണസംഖ്യ>,   OpSqr<പൂർണ്ണസംഖ്യ> >
#define reduceSum2R8u32f  reduceR_<uchar, ദശാംശം, OpAddSqr<പൂർണ്ണസംഖ്യ>,   OpSqr<പൂർണ്ണസംഖ്യ> >
#define reduceSum2R8u64f  reduceR_<uchar, ഇരട്ട,OpAddSqr<പൂർണ്ണസംഖ്യ>,   OpSqr<പൂർണ്ണസംഖ്യ> >
#define reduceSum2R16u32f reduceR_<ushort,ദശാംശം, OpAddSqr<ദശാംശം>, OpSqr<ദശാംശം> >
#define reduceSum2R16u64f reduceR_<ushort,ഇരട്ട,OpAddSqr<ഇരട്ട>,OpSqr<ഇരട്ട> >
#define reduceSum2R16s32f reduceR_<short, ദശാംശം, OpAddSqr<ദശാംശം>, OpSqr<ദശാംശം> >
#define reduceSum2R16s64f reduceR_<short, ഇരട്ട,OpAddSqr<ഇരട്ട>,OpSqr<ഇരട്ട> >
#define reduceSum2R32f32f reduceR_<ദശാംശം, ദശാംശം, OpAddSqr<ദശാംശം>, OpSqr<ദശാംശം> >
#define reduceSum2R32f64f reduceR_<ദശാംശം, ഇരട്ട,OpAddSqr<ഇരട്ട>,OpSqr<ഇരട്ട> >
#define reduceSum2R64f64f reduceR_<ഇരട്ട,ഇരട്ട,OpAddSqr<ഇരട്ട>,OpSqr<ഇരട്ട> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<ദശാംശം, ദശാംശം, OpMax<ദശാംശം> >
#define reduceMaxR64f reduceR_<ഇരട്ട,ഇരട്ട,OpMax<ഇരട്ട> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<ദശാംശം, ദശാംശം, OpMin<ദശാംശം> >
#define reduceMinR64f reduceR_<ഇരട്ട,ഇരട്ട,OpMin<ഇരട്ട> >

#എങ്കിൽ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
സ്ഥിതം വരിയിൽ തർക്കം ipp_reduceSumC_8u16u16s32f_64f(സ്ഥിരം cv::Mat& srcmat, cv::Mat& dstmat)
{
    പൂർണ്ണസംഖ്യ sstep = (പൂർണ്ണസംഖ്യ)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    തരനിർവചനം IppStatus (CV_STDCALL * IppiSum)(സ്ഥിരം ശൂന്യം * pSrc, പൂർണ്ണസംഖ്യ srcStep, IppiSize roiSize, Ipp64f* pSum);
    തരനിർവചനം IppStatus (CV_STDCALL * IppiSumHint)(സ്ഥിരം ശൂന്യം * pSrc, പൂർണ്ണസംഖ്യ srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    എങ്കിൽ(ddepth == CV_64F)
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

    എങ്കിൽ(ippiSum)
    {
        വേണ്ടി(പൂർണ്ണസംഖ്യ y = 0; y < srcmat.size().height; y++)
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                തിരികെ അസത്യം;
        }
        തിരികെ സത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ(ippiSumHint)
    {
        വേണ്ടി(പൂർണ്ണസംഖ്യ y = 0; y < srcmat.size().height; y++)
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                തിരികെ അസത്യം;
        }
        തിരികെ സത്യം;
    }

    തിരികെ അസത്യം;
}

സ്ഥിതം വരിയിൽ ശൂന്യം reduceSumC_8u16u16s32f_64f(സ്ഥിരം cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    എങ്കിൽ(dstmat.depth() == CV_64F)
    {
        പൂർണ്ണസംഖ്യ sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, ഇരട്ട,   cv::OpAdd<ഇരട്ട> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, ഇരട്ട,   cv::OpAdd<ഇരട്ട> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, ഇരട്ട,   cv::OpAdd<ഇരട്ട> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<ദശാംശം, ഇരട്ട,   cv::OpAdd<ഇരട്ട> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, പൂർണ്ണസംഖ്യ,   OpAdd<പൂർണ്ണസംഖ്യ>, OpNop<പൂർണ്ണസംഖ്യ> >
#define reduceSumC8u32f  reduceC_<uchar, ദശാംശം, OpAdd<പൂർണ്ണസംഖ്യ>, OpNop<പൂർണ്ണസംഖ്യ> >
#define reduceSumC16u32f reduceC_<ushort,ദശാംശം, OpAdd<ദശാംശം> >
#define reduceSumC16s32f reduceC_<short, ദശാംശം, OpAdd<ദശാംശം> >
#define reduceSumC32f32f reduceC_<ദശാംശം, ദശാംശം, OpAdd<ദശാംശം> >
#define reduceSumC64f64f reduceC_<ഇരട്ട,ഇരട്ട,OpAdd<ഇരട്ട> >

#define reduceSum2C8u32s  reduceC_<uchar, പൂർണ്ണസംഖ്യ,   OpAddSqr<പൂർണ്ണസംഖ്യ>,   OpSqr<പൂർണ്ണസംഖ്യ> >
#define reduceSum2C8u32f  reduceC_<uchar, ദശാംശം, OpAddSqr<പൂർണ്ണസംഖ്യ>,   OpSqr<പൂർണ്ണസംഖ്യ> >
#define reduceSum2C16u32f reduceC_<ushort,ദശാംശം, OpAddSqr<ദശാംശം>, OpSqr<ദശാംശം> >
#define reduceSum2C16s32f reduceC_<short, ദശാംശം, OpAddSqr<ദശാംശം>, OpSqr<ദശാംശം> >
#define reduceSum2C32f32f reduceC_<ദശാംശം, ദശാംശം, OpAddSqr<ദശാംശം>, OpSqr<ദശാംശം> >
#define reduceSum2C64f64f reduceC_<ഇരട്ട,ഇരട്ട,OpAddSqr<ഇരട്ട>,OpSqr<ഇരട്ട> >

#എങ്കിൽ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#അല്ലെങ്കിൽ
#define reduceSumC8u64f  reduceC_<uchar, ഇരട്ട,OpAdd<പൂർണ്ണസംഖ്യ>, OpNop<പൂർണ്ണസംഖ്യ> >
#define reduceSumC16u64f reduceC_<ushort,ഇരട്ട,OpAdd<ഇരട്ട> >
#define reduceSumC16s64f reduceC_<short, ഇരട്ട,OpAdd<ഇരട്ട> >
#define reduceSumC32f64f reduceC_<ദശാംശം, ഇരട്ട,OpAdd<ഇരട്ട> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, ഇരട്ട,OpAddSqr<പൂർണ്ണസംഖ്യ>,   OpSqr<പൂർണ്ണസംഖ്യ> >
#define reduceSum2C16u64f reduceC_<ushort,ഇരട്ട,OpAddSqr<ഇരട്ട>,OpSqr<ഇരട്ട> >
#define reduceSum2C16s64f reduceC_<short, ഇരട്ട,OpAddSqr<ഇരട്ട>,OpSqr<ഇരട്ട> >
#define reduceSum2C32f64f reduceC_<ദശാംശം, ഇരട്ട,OpAddSqr<ഇരട്ട>,OpSqr<ഇരട്ട> >

#എങ്കിൽ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
സ്ഥിതം വരിയിൽ തർക്കം ipp_reduce##optype##C##favor(സ്ഥിരം cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    എങ്കിൽ((srcmat.channels() == 1)) \
    { \
        പൂർണ്ണസംഖ്യ sstep = (പൂർണ്ണസംഖ്യ)srcmat.step; \
        തരനിർവചനം Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        വേണ്ടി(പൂർണ്ണസംഖ്യ y = 0; y < srcmat.size().height; y++)\
        {\
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                തിരികെ അസത്യം;\
        }\
        തിരികെ സത്യം;\
    }\
    തിരികെ അസത്യം; \
} \
സ്ഥിതം വരിയിൽ ശൂന്യം reduce##optype##C##favor(സ്ഥിരം cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#എങ്കിൽ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, ദശാംശം, ദശാംശം)
#അല്ലെങ്കിൽ
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<ദശാംശം, ദശാംശം, OpMax<ദശാംശം> >
#endif
#define reduceMaxC64f reduceC_<ഇരട്ട,ഇരട്ട,OpMax<ഇരട്ട> >

#എങ്കിൽ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, ദശാംശം, ദശാംശം)
#അല്ലെങ്കിൽ
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<ദശാംശം, ദശാംശം, OpMin<ദശാംശം> >
#endif
#define reduceMinC64f reduceC_<ഇരട്ട,ഇരട്ട,OpMin<ഇരട്ട> >

#ifdef HAVE_OPENCL

നാമാകാശ cv {

സ്ഥിതം തർക്കം ocl_reduce(നിവേശം_Array _src, OutputArray _dst,
                       പൂർണ്ണസംഖ്യ dim, പൂർണ്ണസംഖ്യ op, പൂർണ്ണസംഖ്യ op0, പൂർണ്ണസംഖ്യ stype, പൂർണ്ണസംഖ്യ dtype)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ min_opt_cols = 128, buf_cols = 32;
    പൂർണ്ണസംഖ്യ sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    സ്ഥിരം ocl::Device &defDev = ocl::Device::getDefault();
    തർക്കം doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    തർക്കം useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    എങ്കിൽ (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        തിരികെ അസത്യം;

    എങ്കിൽ (op == REDUCE_AVG)
    {
        എങ്കിൽ (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    സ്ഥിരം അക്ഷരം * സ്ഥിരം ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    പൂർണ്ണസംഖ്യ wdepth = std::max(ddepth, CV_32F);
    എങ്കിൽ (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        എങ്കിൽ (defDev.isIntel())
        {
            സ്ഥിതം സ്ഥിരം size_t max_Item_In_Group_എണ്ണം = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_എണ്ണം);
        }
        അക്ഷരം cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], വലുപ്പം(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], വലുപ്പം(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], വലുപ്പം(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        എങ്കിൽ (k.empty())
            തിരികെ അസത്യം;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        എങ്കിൽ (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        അല്ലെങ്കിൽ
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        തിരികെ k.run(2, globalSize, localSize, അസത്യം);
    }
    അല്ലെങ്കിൽ
    {
        അക്ഷരം cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], വലുപ്പം(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], വലുപ്പം(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], വലുപ്പം(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        എങ്കിൽ (k.empty())
            തിരികെ അസത്യം;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        എങ്കിൽ (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        അല്ലെങ്കിൽ
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        തിരികെ k.run(1, &globalsize, ശൂന്യ, അസത്യം);
    }
}

}

#endif

ശൂന്യം cv::reduce(നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ dim, പൂർണ്ണസംഖ്യ op, പൂർണ്ണസംഖ്യ dtype)
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, സത്യം, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        തിരികെ;
    }
    പൂർണ്ണസംഖ്യ op0 = op;
    പൂർണ്ണസംഖ്യ stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    എങ്കിൽ( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    പൂർണ്ണസംഖ്യ ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    എങ്കിൽ (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    എങ്കിൽ (src.empty()) {
        src.convertTo(_dst, dtype);
        തിരികെ;
    }
    എങ്കിൽ (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    എങ്കിൽ( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        എങ്കിൽ( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    എങ്കിൽ( dim == 0 )
    {
        എങ്കിൽ( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            എങ്കിൽ(simd_func)
                func = (ReduceFunc)simd_func;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        അല്ലെങ്കിൽ എങ്കിൽ(op == REDUCE_MAX)
        {
            എങ്കിൽ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        അല്ലെങ്കിൽ എങ്കിൽ(op == REDUCE_MIN)
        {
            എങ്കിൽ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            എങ്കിൽ(simd_func)
                func = simd_func;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            എങ്കിൽ(simd_func)
                func = (ReduceFunc)simd_func;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        അല്ലെങ്കിൽ എങ്കിൽ(op == REDUCE_MAX)
        {
            എങ്കിൽ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        അല്ലെങ്കിൽ എങ്കിൽ(op == REDUCE_MIN)
        {
            എങ്കിൽ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        അല്ലെങ്കിൽ എങ്കിൽ(op == REDUCE_SUM2)
        {
            എങ്കിൽ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            അല്ലെങ്കിൽ എങ്കിൽ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    എങ്കിൽ( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    എങ്കിൽ( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

നാമാകാശ cv
{

template<typename T> സ്ഥിതം ശൂന്യം sort_( സ്ഥിരം Mat& src, Mat& dst, പൂർണ്ണസംഖ്യ flags )
{
    പൂർണ്ണസംഖ്യ n, len;
    തർക്കം sortRows = (flags & 1) == SORT_EVERY_ROW;
    തർക്കം inplace = src.data == dst.data;
    തർക്കം sortDescending = (flags & SORT_DESCENDING) != 0;

    എങ്കിൽ( sortRows )
        n = src.rows, len = src.cols;
    അല്ലെങ്കിൽ
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](സ്ഥിരം Range& range)
    {
        AutoBuffer<T> buf;
        എങ്കിൽ( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            എങ്കിൽ( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                എങ്കിൽ( !inplace )
                {
                    സ്ഥിരം T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, വലുപ്പം(T) * len);
                }
                ptr = dptr;
            }
            അല്ലെങ്കിൽ
            {
                വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            എങ്കിൽ( sortDescending )
            {
                വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            എങ്കിൽ( !sortRows )
                വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#എങ്കിൽ defined(HAVE_IPP) && !IPP_DISABLE_SORT
തരനിർവചനം IppStatus (CV_STDCALL *IppSortFunc)(ശൂന്യം  *pSrcDst, പൂർണ്ണസംഖ്യ    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, പൂർണ്ണസംഖ്യ, Ipp8u*)>
സ്ഥിതം IppStatus CV_STDCALL ippSortWrap(ശൂന്യം* pSrcDst, പൂർണ്ണസംഖ്യ len, Ipp8u* pBuffer)
{
    തിരികെ fn((T*)pSrcDst, len, pBuffer);
}

സ്ഥിതം IppSortFunc getSortFunc(പൂർണ്ണസംഖ്യ depth, തർക്കം sortDescending)
{
    എങ്കിൽ (!sortDescending)
        തിരികെ depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    അല്ലെങ്കിൽ
        തിരികെ depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

സ്ഥിതം തർക്കം ipp_sort(സ്ഥിരം Mat& src, Mat& dst, പൂർണ്ണസംഖ്യ flags)
{
    CV_INSTRUMENT_REGION_IPP();

    തർക്കം        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    തർക്കം        sortDescending  = (flags & SORT_DESCENDING) != 0;
    തർക്കം        inplace         = (src.data == dst.data);
    പൂർണ്ണസംഖ്യ         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    എങ്കിൽ(!ippsSortRadix_I)
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ n, len;
    എങ്കിൽ( sortRows )
        n = src.rows, len = src.cols;
    അല്ലെങ്കിൽ
        n = src.cols, len = src.rows;

    പൂർണ്ണസംഖ്യ bufferSize;
    എങ്കിൽ(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        തിരികെ അസത്യം;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    അസ്ഥിര തർക്കം ok = സത്യം;
    parallel_for_(Range(0, n), [&](സ്ഥിരം Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        എങ്കിൽ( !sortRows )
            row.create(1, src.rows, src.type());

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = range.start; i < range.end; i++ )
        {
            എങ്കിൽ( !ok )
                നിർത്തുക;
            ശൂന്യം* ptr;
            എങ്കിൽ( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                എങ്കിൽ( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            അല്ലെങ്കിൽ
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            എങ്കിൽ( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = അസത്യം;
                നിർത്തുക;
            }

            എങ്കിൽ( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    തിരികെ ok;
}
#endif

template<typename _Tp> വർഗ്ഗം LessThanIdx
{
പൊതു:
    LessThanIdx( സ്ഥിരം _Tp* _arr ) : arr(_arr) {}
    തർക്കം operator()(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b) സ്ഥിരം { തിരികെ arr[a] < arr[b]; }
    സ്ഥിരം _Tp* arr;
};

template<typename T> സ്ഥിതം ശൂന്യം sortIdx_( സ്ഥിരം Mat& src, Mat& dst, പൂർണ്ണസംഖ്യ flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<പൂർണ്ണസംഖ്യ> ibuf;
    തർക്കം sortRows = (flags & 1) == SORT_EVERY_ROW;
    തർക്കം sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    പൂർണ്ണസംഖ്യ n, len;
    എങ്കിൽ( sortRows )
        n = src.rows, len = src.cols;
    അല്ലെങ്കിൽ
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    പൂർണ്ണസംഖ്യ* _iptr = ibuf.data();

    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        പൂർണ്ണസംഖ്യ* iptr = _iptr;

        എങ്കിൽ( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<പൂർണ്ണസംഖ്യ>(i);
        }
        അല്ലെങ്കിൽ
        {
            വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        എങ്കിൽ( sortDescending )
        {
            വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        എങ്കിൽ( !sortRows )
            വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < len; j++ )
                dst.ptr<പൂർണ്ണസംഖ്യ>(j)[i] = iptr[j];
    }
}

#എങ്കിൽ defined(HAVE_IPP) && !IPP_DISABLE_SORT
തരനിർവചനം IppStatus (CV_STDCALL *IppSortIndexFunc)(സ്ഥിരം ശൂന്യം*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, പൂർണ്ണസംഖ്യ len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(സ്ഥിരം T*, Ipp32s, Ipp32s*, പൂർണ്ണസംഖ്യ, Ipp8u*)>
സ്ഥിതം IppStatus CV_STDCALL ippSortIndexWrap(സ്ഥിരം ശൂന്യം* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, പൂർണ്ണസംഖ്യ len, Ipp8u *pBuffer)
{
    തിരികെ fn((സ്ഥിരം T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

സ്ഥിതം IppSortIndexFunc getSortIndexFunc(പൂർണ്ണസംഖ്യ depth, തർക്കം sortDescending)
{
    എങ്കിൽ (!sortDescending)
        തിരികെ depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    അല്ലെങ്കിൽ
        തിരികെ depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

സ്ഥിതം തർക്കം ipp_sortIdx( സ്ഥിരം Mat& src, Mat& dst, പൂർണ്ണസംഖ്യ flags )
{
    CV_INSTRUMENT_REGION_IPP();

    തർക്കം        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    തർക്കം        sortDescending  = (flags & SORT_DESCENDING) != 0;
    പൂർണ്ണസംഖ്യ         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    എങ്കിൽ(!ippsSortRadixIndex)
        തിരികെ അസത്യം;

    എങ്കിൽ(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        പൂർണ്ണസംഖ്യ               bufferSize;
        എങ്കിൽ(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            തിരികെ അസത്യം;

        buffer.allocate(bufferSize);

        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < src.rows; i++)
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (സ്ഥിരം ശൂന്യം*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                തിരികെ അസത്യം;
        }
    }
    അല്ലെങ്കിൽ
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        പൂർണ്ണസംഖ്യ               bufferSize;
        എങ്കിൽ(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            തിരികെ അസത്യം;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        വേണ്ടി(പൂർണ്ണസംഖ്യ i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (സ്ഥിരം ശൂന്യം*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                തിരികെ അസത്യം;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    തിരികെ സത്യം;
}
#endif

തരനിർവചനം ശൂന്യം (*SortFunc)(സ്ഥിരം Mat& src, Mat& dst, പൂർണ്ണസംഖ്യ flags);
}

ശൂന്യം cv::sort( നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#എങ്കിൽ !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    സ്ഥിതം SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<പൂർണ്ണസംഖ്യ>, sort_<ദശാംശം>, sort_<ഇരട്ട>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

ശൂന്യം cv::sortIdx( നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    എങ്കിൽ( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#എങ്കിൽ !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    സ്ഥിതം SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<പൂർണ്ണസംഖ്യ>, sortIdx_<ദശാംശം>, sortIdx_<ഇരട്ട>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
