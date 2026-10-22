// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "opencv2/core/mat.hpp"
#समावेश "opencl_kernels_core.hpp"

#घोषणा IPP_DISABLE_REDUCE 1
#घोषणा IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

व्योम cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.rows, b.rows);
    std::swap(a.cols, b.cols);
    std::swap(a.तथ्य, b.तथ्य);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.step, b.step);
}


व्योम cv::hconcat(स्थिर Mat* src, माप_प्रकार nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    अगर( nsrc == 0 || !src )
    {
        _dst.release();
        वापस;
    }

    पूर्णांक योग_Cols = 0, cols = 0;
    क्रम( माप_प्रकार i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        योग_Cols += src[i].cols;
    }
    _dst.create( src[0].rows, योग_Cols, src[0].type());
    Mat dst = _dst.getMat();
    क्रम( माप_प्रकार i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

व्योम cv::hconcat(निवेश_Array src1, निवेश_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

व्योम cv::hconcat(निवेश_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

व्योम cv::vconcat(स्थिर Mat* src, माप_प्रकार nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    अगर( nsrc == 0 || !src )
    {
        _dst.release();
        वापस;
    }

    पूर्णांक योग_Rows = 0, rows = 0;
    क्रम( माप_प्रकार i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        योग_Rows += src[i].rows;
    }
    _dst.create( योग_Rows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    क्रम( माप_प्रकार i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

व्योम cv::vconcat(निवेश_Array src1, निवेश_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

व्योम cv::vconcat(निवेश_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#अगर_घोषित HAVE_OPENCL

namespace cv {

अटल bool ocl_setIdentity( निवेश_Output_Array _m, स्थिर Scalar& s )
{
    पूर्णांक type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    पूर्णांक sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    अगर (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        अगर (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            अगर (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    अगर (k.empty())
        वापस false;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    माप_प्रकार globalsize[2] = { (माप_प्रकार)m.cols * cn / kercn, ((माप_प्रकार)m.rows + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, nullptr, false);
}

}

#पूर्ण_अगर

व्योम cv::setIdentity( निवेश_Output_Array _m, स्थिर Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    पूर्णांक rows = m.rows, cols = m.cols, type = m.type();

    अगर( type == CV_32FC1 )
    {
        भग्न* तथ्य = m.ptr<भग्न>();
        भग्न val = (भग्न)s[0];
        माप_प्रकार step = m.step/माप(तथ्य[0]);

        क्रम( पूर्णांक i = 0; i < rows; i++, तथ्य += step )
        {
            क्रम( पूर्णांक j = 0; j < cols; j++ )
                तथ्य[j] = 0;
            अगर( i < cols )
                तथ्य[i] = val;
        }
    }
    अन्यथा अगर( type == CV_64FC1 )
    {
        द्विगुन* तथ्य = m.ptr<द्विगुन>();
        द्विगुन val = s[0];
        माप_प्रकार step = m.step/माप(तथ्य[0]);

        क्रम( पूर्णांक i = 0; i < rows; i++, तथ्य += step )
        {
            std::fill(तथ्य, तथ्य + cols, 0.0);
            अगर (i < cols)
                तथ्य[i] = val;
        }
    }
    अन्यथा
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


namespace cv {

UMat UMat::eye(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, UMatUsageFlags usageFlags)
{
    वापस UMat::eye(आकार(cols, rows), type, usageFlags);
}

UMat UMat::eye(आकार size, पूर्णांक type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    वापस m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( निवेश_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    पूर्णांक type = m.type();
    पूर्णांक nm = std::min(m.rows, m.cols);

    अगर( type == CV_32FC1 )
    {
        स्थिर भग्न* ptr = m.ptr<भग्न>();
        माप_प्रकार step = m.step/माप(ptr[0]) + 1;
        द्विगुन _s = 0;
        क्रम( पूर्णांक i = 0; i < nm; i++ )
            _s += ptr[i*step];
        वापस _s;
    }

    अगर( type == CV_64FC1 )
    {
        स्थिर द्विगुन* ptr = m.ptr<द्विगुन>();
        माप_प्रकार step = m.step/माप(ptr[0]) + 1;
        द्विगुन _s = 0;
        क्रम( पूर्णांक i = 0; i < nm; i++ )
            _s += ptr[i*step];
        वापस _s;
    }

    वापस cv::योग_2(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

व्योम cv::completeSymm( निवेश_Output_Array _m, bool LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    माप_प्रकार step = m.step, esz = m.elem_आकार();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    पूर्णांक rows = m.rows;
    पूर्णांक j0 = 0, j1 = rows;

    uchar* तथ्य = m.ptr();
    क्रम( पूर्णांक i = 0; i < rows; i++ )
    {
        अगर( !LtoR ) j1 = i; अन्यथा j0 = i+1;
        क्रम( पूर्णांक j = j0; j < j1; j++ )
            स_नकल(तथ्य + (i*step + j*esz), तथ्य + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(निवेश_Array _m) स्थिर
{
    Mat m = _m.getMat();
    पूर्णांक tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat परिणाम(rows, cols, tp);

    अगर( d == CV_32F )
    {
        स्थिर भग्न *a = (स्थिर भग्न*)तथ्य, *b = (स्थिर भग्न*)m.तथ्य;
        भग्न* c = (भग्न*)परिणाम.तथ्य;
        माप_प्रकार lda = rows > 1 ? step/माप(a[0]) : 1;
        माप_प्रकार ldb = rows > 1 ? m.step/माप(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    अन्यथा अगर( d == CV_64F )
    {
        स्थिर द्विगुन *a = (स्थिर द्विगुन*)तथ्य, *b = (स्थिर द्विगुन*)m.तथ्य;
        द्विगुन* c = (द्विगुन*)परिणाम.तथ्य;
        माप_प्रकार lda = rows > 1 ? step/माप(a[0]) : 1;
        माप_प्रकार ldb = rows > 1 ? m.step/माप(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    वापस परिणाम;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

namespace cv
{

प्रकार व्योम (*Reduce_योग_Func)(स्थिर Mat& src, Mat& dst);
Reduce_योग_Func getReduceCSumFunc(पूर्णांक sdepth, पूर्णांक ddepth);
Reduce_योग_Func getReduceRSumFunc(पूर्णांक sdepth, पूर्णांक ddepth);
Reduce_योग_Func getReduceRSum2Func(पूर्णांक sdepth, पूर्णांक ddepth);

ढाँचा <typename T, typename WT, typename Op>
काष्ठा ReduceR_SIMD
{
    पूर्णांक चालक()(स्थिर T*, पूर्णांक start, पूर्णांक, WT*, स्थिर Op&) स्थिर
    {
        वापस start;
    }
};


ढाँचा<typename T, typename ST, typename WT, श्रेणी Op, श्रेणी OpInit>
श्रेणी ReduceR_Invoker : खुला ParallelLoopBody
{
खुला:
  ReduceR_Invoker(स्थिर Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),बफर(srcmat.size().width*srcmat.channels())
  {
  }
  व्योम चालक()(स्थिर Range& range) स्थिर CV_OVERRIDE
  {
    स्थिर T* src = srcmat.ptr<T>();
    स्थिर माप_प्रकार srcstep = srcmat.step/माप(src[0]);
    WT* buf = बफर.तथ्य();
    ST* dst = dstmat.ptr<ST>();
    पूर्णांक i = 0;

    क्रम( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    पूर्णांक height = srcmat.size().height;
    क्रम( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #अगर CV_ENABLE_UNROLLED
        क्रम(; i <= range.end - 4; i += 4 )
        {
            WT s0, s1;
            s0 = op(buf[i], (WT)src[i]);
            s1 = op(buf[i+1], (WT)src[i+1]);
            buf[i] = s0; buf[i+1] = s1;

            s0 = op(buf[i+2], (WT)src[i+2]);
            s1 = op(buf[i+3], (WT)src[i+3]);
            buf[i+2] = s0; buf[i+3] = s1;
        }
        #पूर्ण_अगर
        क्रम( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    क्रम( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
निजी:
  स्थिर Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable Auto_बफर<WT> बफर;
};

ढाँचा<typename T, typename ST, श्रेणी Op, श्रेणी OpInit = OpNop<ST> > अटल व्योम
reduceR_( स्थिर Mat& srcmat, Mat& dstmat)
{
    प्रकार typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_आकार(srcmat.depth())/64);
}

ढाँचा<typename T, typename ST, typename WT, श्रेणी Op, श्रेणी OpInit>
श्रेणी ReduceC_Invoker : खुला ParallelLoopBody
{
खुला:
  ReduceC_Invoker(स्थिर Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  व्योम चालक()(स्थिर Range& range) स्थिर CV_OVERRIDE
  {
    स्थिर पूर्णांक cn = srcmat.channels();
    स्थिर पूर्णांक width = srcmat.size().width*cn;
    Auto_बफर<WT> cumul(cn);
    क्रम( पूर्णांक y = range.start; y < range.end; y++ )
    {
        स्थिर T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        अगर( width == cn )
        {
          क्रम( पूर्णांक k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        अन्यथा
        {
            क्रम(पूर्णांक k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            क्रम(पूर्णांक k = cn ; k < width ; k += cn )
            {
                क्रम (पूर्णांक c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            क्रम(पूर्णांक k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
निजी:
  स्थिर Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

ढाँचा<typename T, typename ST, श्रेणी Op, श्रेणी OpInit = OpNop<ST> > अटल व्योम
reduceC_( स्थिर Mat& srcmat, Mat& dstmat)
{
    प्रकार typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

प्रकार व्योम (*ReduceFunc)( स्थिर Mat& src, Mat& dst );

}

#घोषणा reduce_योग_R8u32s  reduceR_<uchar, पूर्णांक,   OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#घोषणा reduce_योग_R8u32f  reduceR_<uchar, भग्न, OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#घोषणा reduce_योग_R8u64f  reduceR_<uchar, द्विगुन,OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#घोषणा reduce_योग_R16u32f reduceR_<ushort,भग्न, OpAdd<भग्न> >
#घोषणा reduce_योग_R16u64f reduceR_<ushort,द्विगुन,OpAdd<द्विगुन> >
#घोषणा reduce_योग_R16s32f reduceR_<लघु, भग्न, OpAdd<भग्न> >
#घोषणा reduce_योग_R16s64f reduceR_<लघु, द्विगुन,OpAdd<द्विगुन> >
#घोषणा reduce_योग_R32f32f reduceR_<भग्न, भग्न, OpAdd<भग्न> >
#घोषणा reduce_योग_R32f64f reduceR_<भग्न, द्विगुन,OpAdd<द्विगुन> >
#घोषणा reduce_योग_R64f64f reduceR_<द्विगुन,द्विगुन,OpAdd<द्विगुन> >

#घोषणा reduceSum2R8u32s  reduceR_<uchar, पूर्णांक,   OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#घोषणा reduceSum2R8u32f  reduceR_<uchar, भग्न, OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#घोषणा reduceSum2R8u64f  reduceR_<uchar, द्विगुन,OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#घोषणा reduceSum2R16u32f reduceR_<ushort,भग्न, OpAddSqr<भग्न>, OpSqr<भग्न> >
#घोषणा reduceSum2R16u64f reduceR_<ushort,द्विगुन,OpAddSqr<द्विगुन>,OpSqr<द्विगुन> >
#घोषणा reduceSum2R16s32f reduceR_<लघु, भग्न, OpAddSqr<भग्न>, OpSqr<भग्न> >
#घोषणा reduceSum2R16s64f reduceR_<लघु, द्विगुन,OpAddSqr<द्विगुन>,OpSqr<द्विगुन> >
#घोषणा reduceSum2R32f32f reduceR_<भग्न, भग्न, OpAddSqr<भग्न>, OpSqr<भग्न> >
#घोषणा reduceSum2R32f64f reduceR_<भग्न, द्विगुन,OpAddSqr<द्विगुन>,OpSqr<द्विगुन> >
#घोषणा reduceSum2R64f64f reduceR_<द्विगुन,द्विगुन,OpAddSqr<द्विगुन>,OpSqr<द्विगुन> >

#घोषणा reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#घोषणा reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#घोषणा reduceMaxR16s reduceR_<लघु, लघु, OpMax<लघु> >
#घोषणा reduceMaxR32f reduceR_<भग्न, भग्न, OpMax<भग्न> >
#घोषणा reduceMaxR64f reduceR_<द्विगुन,द्विगुन,OpMax<द्विगुन> >

#घोषणा reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#घोषणा reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#घोषणा reduceMinR16s reduceR_<लघु, लघु, OpMin<लघु> >
#घोषणा reduceMinR32f reduceR_<भग्न, भग्न, OpMin<भग्न> >
#घोषणा reduceMinR64f reduceR_<द्विगुन,द्विगुन,OpMin<द्विगुन> >

#अगर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
अटल अंतरभूत bool ipp_reduce_योग_C_8u16u16s32f_64f(स्थिर cv::Mat& srcmat, cv::Mat& dstmat)
{
    पूर्णांक sstep = (पूर्णांक)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    Ippi_आकार roisize = { srcmat.size().width, 1 };

    प्रकार IppStatus (CV_STDCALL * Ippi_योग)(स्थिर व्योम * pSrc, पूर्णांक srcStep, Ippi_आकार roi_आकार, Ipp64f* p_योग);
    प्रकार IppStatus (CV_STDCALL * Ippi_योग_Hint)(स्थिर व्योम * pSrc, पूर्णांक srcStep, Ippi_आकार roi_आकार, Ipp64f* p_योग, IppHintAlgorithm hint);
    Ippi_योग ippi_योग = 0;
    Ippi_योग_Hint ippi_योग_Hint = 0;

    अगर(ddepth == CV_64F)
    {
        ippi_योग =
            stype == CV_8UC1 ? (Ippi_योग)ippi_योग_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_योग)ippi_योग_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_योग)ippi_योग_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_योग)ippi_योग_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_योग)ippi_योग_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_योग)ippi_योग_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_योग)ippi_योग_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_योग)ippi_योग_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_योग)ippi_योग_16s_C4_R : 0;
        ippi_योग_Hint =
            stype == CV_32FC1 ? (Ippi_योग_Hint)ippi_योग_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_योग_Hint)ippi_योग_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_योग_Hint)ippi_योग_32f_C4_R : 0;
    }

    अगर(ippi_योग)
    {
        क्रम(पूर्णांक y = 0; y < srcmat.size().height; y++)
        {
            अगर(CV_INSTRUMENT_FUN_IPP(ippi_योग, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                वापस false;
        }
        वापस true;
    }
    अन्यथा अगर(ippi_योग_Hint)
    {
        क्रम(पूर्णांक y = 0; y < srcmat.size().height; y++)
        {
            अगर(CV_INSTRUMENT_FUN_IPP(ippi_योग_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                वापस false;
        }
        वापस true;
    }

    वापस false;
}

अटल अंतरभूत व्योम reduce_योग_C_8u16u16s32f_64f(स्थिर cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduce_योग_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    अगर(dstmat.depth() == CV_64F)
    {
        पूर्णांक sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, द्विगुन,   cv::OpAdd<द्विगुन> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, द्विगुन,   cv::OpAdd<द्विगुन> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<लघु, द्विगुन,   cv::OpAdd<द्विगुन> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<भग्न, द्विगुन,   cv::OpAdd<द्विगुन> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#पूर्ण_अगर

#घोषणा reduce_योग_C8u32s  reduceC_<uchar, पूर्णांक,   OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#घोषणा reduce_योग_C8u32f  reduceC_<uchar, भग्न, OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#घोषणा reduce_योग_C16u32f reduceC_<ushort,भग्न, OpAdd<भग्न> >
#घोषणा reduce_योग_C16s32f reduceC_<लघु, भग्न, OpAdd<भग्न> >
#घोषणा reduce_योग_C32f32f reduceC_<भग्न, भग्न, OpAdd<भग्न> >
#घोषणा reduce_योग_C64f64f reduceC_<द्विगुन,द्विगुन,OpAdd<द्विगुन> >

#घोषणा reduceSum2C8u32s  reduceC_<uchar, पूर्णांक,   OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#घोषणा reduceSum2C8u32f  reduceC_<uchar, भग्न, OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#घोषणा reduceSum2C16u32f reduceC_<ushort,भग्न, OpAddSqr<भग्न>, OpSqr<भग्न> >
#घोषणा reduceSum2C16s32f reduceC_<लघु, भग्न, OpAddSqr<भग्न>, OpSqr<भग्न> >
#घोषणा reduceSum2C32f32f reduceC_<भग्न, भग्न, OpAddSqr<भग्न>, OpSqr<भग्न> >
#घोषणा reduceSum2C64f64f reduceC_<द्विगुन,द्विगुन,OpAddSqr<द्विगुन>,OpSqr<द्विगुन> >

#अगर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#घोषणा reduce_योग_C8u64f  reduce_योग_C_8u16u16s32f_64f
#घोषणा reduce_योग_C16u64f reduce_योग_C_8u16u16s32f_64f
#घोषणा reduce_योग_C16s64f reduce_योग_C_8u16u16s32f_64f
#घोषणा reduce_योग_C32f64f reduce_योग_C_8u16u16s32f_64f
#अन्यथा
#घोषणा reduce_योग_C8u64f  reduceC_<uchar, द्विगुन,OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#घोषणा reduce_योग_C16u64f reduceC_<ushort,द्विगुन,OpAdd<द्विगुन> >
#घोषणा reduce_योग_C16s64f reduceC_<लघु, द्विगुन,OpAdd<द्विगुन> >
#घोषणा reduce_योग_C32f64f reduceC_<भग्न, द्विगुन,OpAdd<द्विगुन> >
#पूर्ण_अगर

#घोषणा reduceSum2C8u64f  reduceC_<uchar, द्विगुन,OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#घोषणा reduceSum2C16u64f reduceC_<ushort,द्विगुन,OpAddSqr<द्विगुन>,OpSqr<द्विगुन> >
#घोषणा reduceSum2C16s64f reduceC_<लघु, द्विगुन,OpAddSqr<द्विगुन>,OpSqr<द्विगुन> >
#घोषणा reduceSum2C32f64f reduceC_<भग्न, द्विगुन,OpAddSqr<द्विगुन>,OpSqr<द्विगुन> >

#अगर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#घोषणा REDUCE_OP(favor, optype, type1, type2) \
अटल अंतरभूत bool ipp_reduce##optype##C##favor(स्थिर cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    अगर((srcmat.channels() == 1)) \
    { \
        पूर्णांक sstep = (पूर्णांक)srcmat.step; \
        प्रकार Ipp##favor IppType; \
        Ippi_आकार roisize = ippi_आकार(srcmat.size().width, 1);\
        क्रम(पूर्णांक y = 0; y < srcmat.size().height; y++)\
        {\
            अगर(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                वापस false;\
        }\
        वापस true;\
    }\
    वापस false; \
} \
अटल अंतरभूत व्योम reduce##optype##C##favor(स्थिर cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#पूर्ण_अगर

#अगर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, लघु, लघु)
REDUCE_OP(32f, Max, भग्न, भग्न)
#अन्यथा
#घोषणा reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#घोषणा reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#घोषणा reduceMaxC16s reduceC_<लघु, लघु, OpMax<लघु> >
#घोषणा reduceMaxC32f reduceC_<भग्न, भग्न, OpMax<भग्न> >
#पूर्ण_अगर
#घोषणा reduceMaxC64f reduceC_<द्विगुन,द्विगुन,OpMax<द्विगुन> >

#अगर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, लघु, लघु)
REDUCE_OP(32f, Min, भग्न, भग्न)
#अन्यथा
#घोषणा reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#घोषणा reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#घोषणा reduceMinC16s reduceC_<लघु, लघु, OpMin<लघु> >
#घोषणा reduceMinC32f reduceC_<भग्न, भग्न, OpMin<भग्न> >
#पूर्ण_अगर
#घोषणा reduceMinC64f reduceC_<द्विगुन,द्विगुन,OpMin<द्विगुन> >

#अगर_घोषित HAVE_OPENCL

namespace cv {

अटल bool ocl_reduce(निवेश_Array _src, OutputArray _dst,
                       पूर्णांक dim, पूर्णांक op, पूर्णांक op0, पूर्णांक stype, पूर्णांक dtype)
{
    स्थिर पूर्णांक min_opt_cols = 128, buf_cols = 32;
    पूर्णांक sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    स्थिर ocl::Device &defDev = ocl::Device::getDefault();
    bool doubleSupport = defDev.doubleFPConfig() > 0;

    माप_प्रकार wgs = defDev.max_Work_Group_आकार();
    bool useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    अगर (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        वापस false;

    अगर (op == REDUCE_AVG)
    {
        अगर (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    स्थिर अक्षर * स्थिर ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    पूर्णांक wdepth = std::max(ddepth, CV_32F);
    अगर (useOptimized)
    {
        माप_प्रकार tileHeight = (माप_प्रकार)(wgs / buf_cols);
        अगर (defDev.isIntel())
        {
            अटल स्थिर माप_प्रकार max_Item_In_Group_गिनती = 16;
            tileHeight = min(tileHeight, defDev.local_Mem_आकार() / buf_cols / CV_ELEM_आकार(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_गिनती);
        }
        अक्षर cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], माप(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], माप(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], माप(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        अगर (k.empty())
            वापस false;
        UMat src = _src.getUMat();
        आकार dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        अगर (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_आकार(dst), 1.0f / src.cols);
        अन्यथा
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_आकार(dst));

        माप_प्रकार local_आकार[2] = { (माप_प्रकार)buf_cols, (माप_प्रकार)tileHeight};
        माप_प्रकार global_आकार[2] = { (माप_प्रकार)buf_cols, (माप_प्रकार)src.rows };
        वापस k.run(2, global_आकार, local_आकार, false);
    }
    अन्यथा
    {
        अक्षर cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], माप(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], माप(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], माप(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        अगर (k.empty())
            वापस false;

        UMat src = _src.getUMat();
        आकार dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_आकार(dst);

        अगर (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        अन्यथा
            k.args(srcarg, temparg);

        माप_प्रकार globalsize = std::max(dsize.width, dsize.height);
        वापस k.run(1, &globalsize, शून्य, false);
    }
}

}

#पूर्ण_अगर

व्योम cv::reduce(निवेश_Array _src, OutputArray _dst, पूर्णांक dim, पूर्णांक op, पूर्णांक dtype)
{
    CV_INSTRUMENT_REGION();

    अगर (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, true, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        वापस;
    }
    पूर्णांक op0 = op;
    पूर्णांक stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    अगर( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    पूर्णांक ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_योग || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    अगर (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    अगर (src.empty()) {
        src.convertTo(_dst, dtype);
        वापस;
    }
    अगर (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), अस्थायी_2 = dst;

    अगर( op == REDUCE_AVG )
    {
        op = REDUCE_योग;
        अगर( sdepth < CV_32S && ddepth < CV_32S )
        {
            अस्थायी_2.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    अगर( dim == 0 )
    {
        अगर( op == REDUCE_योग )
        {
            Reduce_योग_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            अगर(simd_func)
                func = (ReduceFunc)simd_func;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_योग_R8u32s;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_योग_R8u32f;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_योग_R8u64f;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_योग_R16u32f;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_योग_R16u64f;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_योग_R16s32f;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_योग_R16s64f;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_योग_R32f32f;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_योग_R32f64f;
            अन्यथा अगर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_योग_R64f64f;
        }
        अन्यथा अगर(op == REDUCE_MAX)
        {
            अगर(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            अन्यथा अगर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        अन्यथा अगर(op == REDUCE_MIN)
        {
            अगर(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            अन्यथा अगर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        अन्यथा अगर( op == REDUCE_SUM2 )
        {
            Reduce_योग_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
            अगर(simd_func)
                func = simd_func;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            अन्यथा अगर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    अन्यथा
    {
        अगर(op == REDUCE_योग)
        {
            Reduce_योग_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            अगर(simd_func)
                func = (ReduceFunc)simd_func;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_योग_C8u32s;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_योग_C8u32f;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_योग_C8u64f;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_योग_C16u32f;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_योग_C16u64f;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_योग_C16s32f;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_योग_C16s64f;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_योग_C32f32f;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_योग_C32f64f;
            अन्यथा अगर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_योग_C64f64f;
        }
        अन्यथा अगर(op == REDUCE_MAX)
        {
            अगर(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            अन्यथा अगर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        अन्यथा अगर(op == REDUCE_MIN)
        {
            अगर(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            अन्यथा अगर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        अन्यथा अगर(op == REDUCE_SUM2)
        {
            अगर(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            अन्यथा अगर(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            अन्यथा अगर(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            अन्यथा अगर(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            अन्यथा अगर(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            अन्यथा अगर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    अगर( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, अस्थायी_2 );

    अगर( op0 == REDUCE_AVG )
        अस्थायी_2.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

namespace cv
{

ढाँचा<typename T> अटल व्योम sort_( स्थिर Mat& src, Mat& dst, पूर्णांक flags )
{
    पूर्णांक n, len;
    bool sortRows = (flags & 1) == SORT_EVERY_ROW;
    bool inplace = src.तथ्य == dst.तथ्य;
    bool sortDescending = (flags & SORT_DESCENDING) != 0;

    अगर( sortRows )
        n = src.rows, len = src.cols;
    अन्यथा
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](स्थिर Range& range)
    {
        Auto_बफर<T> buf;
        अगर( !sortRows )
            buf.allocate(len);
        T* bptr = buf.तथ्य();

        क्रम( पूर्णांक i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            अगर( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                अगर( !inplace )
                {
                    स्थिर T* sptr = src.ptr<T>(i);
                    स_नकल(dptr, sptr, माप(T) * len);
                }
                ptr = dptr;
            }
            अन्यथा
            {
                क्रम( पूर्णांक j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            अगर( sortDescending )
            {
                क्रम( पूर्णांक j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            अगर( !sortRows )
                क्रम( पूर्णांक j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#अगर defined(HAVE_IPP) && !IPP_DISABLE_SORT
प्रकार IppStatus (CV_STDCALL *IppSortFunc)(व्योम  *pSrcDst, पूर्णांक    len, Ipp8u *p_बफर);

ढाँचा<typename T, IppStatus (CV_STDCALL *fn)(T*, पूर्णांक, Ipp8u*)>
अटल IppStatus CV_STDCALL ippSortWrap(व्योम* pSrcDst, पूर्णांक len, Ipp8u* p_बफर)
{
    वापस fn((T*)pSrcDst, len, p_बफर);
}

अटल IppSortFunc getSortFunc(पूर्णांक depth, bool sortDescending)
{
    अगर (!sortDescending)
        वापस depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    अन्यथा
        वापस depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

अटल bool ipp_sort(स्थिर Mat& src, Mat& dst, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION_IPP();

    bool        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    bool        sortDescending  = (flags & SORT_DESCENDING) != 0;
    bool        inplace         = (src.तथ्य == dst.तथ्य);
    पूर्णांक         depth           = src.depth();
    Ipp_तथ्य_Type type            = ippi_Get_तथ्य_Type(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    अगर(!ippsSortRadix_I)
        वापस false;

    पूर्णांक n, len;
    अगर( sortRows )
        n = src.rows, len = src.cols;
    अन्यथा
        n = src.cols, len = src.rows;

    पूर्णांक बफर_आकार;
    अगर(ipps_Sort_Radix_Get_बफर_आकार(len, type, &बफर_आकार) < 0)
        वापस false;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    अस्थिर bool ok = true;
    parallel_for_(Range(0, n), [&](स्थिर Range& range)
    {
        Auto_बफर<Ipp8u> बफर(बफर_आकार);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        अगर( !sortRows )
            row.create(1, src.rows, src.type());

        क्रम( पूर्णांक i = range.start; i < range.end; i++ )
        {
            अगर( !ok )
                अवरोध;
            व्योम* ptr;
            अगर( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                अगर( !inplace )
                    स_नकल(dptr, src.ptr(i), src.cols * src.elem_आकार());
                ptr = dptr;
            }
            अन्यथा
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            अगर( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, बफर.तथ्य()) < 0 )
            {
                ok = false;
                अवरोध;
            }

            अगर( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    वापस ok;
}
#पूर्ण_अगर

ढाँचा<typename _Tp> श्रेणी LessThanIdx
{
खुला:
    LessThanIdx( स्थिर _Tp* _arr ) : arr(_arr) {}
    bool चालक()(पूर्णांक a, पूर्णांक b) स्थिर { वापस arr[a] < arr[b]; }
    स्थिर _Tp* arr;
};

ढाँचा<typename T> अटल व्योम sortIdx_( स्थिर Mat& src, Mat& dst, पूर्णांक flags )
{
    Auto_बफर<T> buf;
    Auto_बफर<पूर्णांक> ibuf;
    bool sortRows = (flags & 1) == SORT_EVERY_ROW;
    bool sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.तथ्य != dst.तथ्य );

    पूर्णांक n, len;
    अगर( sortRows )
        n = src.rows, len = src.cols;
    अन्यथा
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.तथ्य();
    पूर्णांक* _iptr = ibuf.तथ्य();

    क्रम( पूर्णांक i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        पूर्णांक* iptr = _iptr;

        अगर( sortRows )
        {
            ptr = (T*)(src.तथ्य + src.step*i);
            iptr = dst.ptr<पूर्णांक>(i);
        }
        अन्यथा
        {
            क्रम( पूर्णांक j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        क्रम( पूर्णांक j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        अगर( sortDescending )
        {
            क्रम( पूर्णांक j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        अगर( !sortRows )
            क्रम( पूर्णांक j = 0; j < len; j++ )
                dst.ptr<पूर्णांक>(j)[i] = iptr[j];
    }
}

#अगर defined(HAVE_IPP) && !IPP_DISABLE_SORT
प्रकार IppStatus (CV_STDCALL *Ipp_Sort_सूचकांक_Func)(स्थिर व्योम*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, पूर्णांक len, Ipp8u *p_बफर);

ढाँचा<typename T, IppStatus (CV_STDCALL *fn)(स्थिर T*, Ipp32s, Ipp32s*, पूर्णांक, Ipp8u*)>
अटल IppStatus CV_STDCALL ipp_Sort_सूचकांक_Wrap(स्थिर व्योम* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, पूर्णांक len, Ipp8u *p_बफर)
{
    वापस fn((स्थिर T*)pSrc, srcStrideBytes, pDstIndx, len, p_बफर);
}

अटल Ipp_Sort_सूचकांक_Func get_Sort_सूचकांक_Func(पूर्णांक depth, bool sortDescending)
{
    अगर (!sortDescending)
        वापस depth == CV_8U ? ipp_Sort_सूचकांक_Wrap<Ipp8u, ipps_Sort_Radix_सूचकांक_Ascend_8u> :
            depth == CV_16U ? ipp_Sort_सूचकांक_Wrap<Ipp16u, ipps_Sort_Radix_सूचकांक_Ascend_16u> :
            depth == CV_16S ? ipp_Sort_सूचकांक_Wrap<Ipp16s, ipps_Sort_Radix_सूचकांक_Ascend_16s> :
            depth == CV_32S ? ipp_Sort_सूचकांक_Wrap<Ipp32s, ipps_Sort_Radix_सूचकांक_Ascend_32s> :
            depth == CV_32F ? ipp_Sort_सूचकांक_Wrap<Ipp32f, ipps_Sort_Radix_सूचकांक_Ascend_32f> :
            0;
    अन्यथा
        वापस depth == CV_8U ? ipp_Sort_सूचकांक_Wrap<Ipp8u, ipps_Sort_Radix_सूचकांक_Descend_8u> :
            depth == CV_16U ? ipp_Sort_सूचकांक_Wrap<Ipp16u, ipps_Sort_Radix_सूचकांक_Descend_16u> :
            depth == CV_16S ? ipp_Sort_सूचकांक_Wrap<Ipp16s, ipps_Sort_Radix_सूचकांक_Descend_16s> :
            depth == CV_32S ? ipp_Sort_सूचकांक_Wrap<Ipp32s, ipps_Sort_Radix_सूचकांक_Descend_32s> :
            depth == CV_32F ? ipp_Sort_सूचकांक_Wrap<Ipp32f, ipps_Sort_Radix_सूचकांक_Descend_32f> :
            0;
}

अटल bool ipp_sortIdx( स्थिर Mat& src, Mat& dst, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION_IPP();

    bool        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    bool        sortDescending  = (flags & SORT_DESCENDING) != 0;
    पूर्णांक         depth           = src.depth();
    Ipp_तथ्य_Type type            = ippi_Get_तथ्य_Type(depth);

    Ipp_Sort_सूचकांक_Func ipps_Sort_Radix_सूचकांक = get_Sort_सूचकांक_Func(depth, sortDescending);
    अगर(!ipps_Sort_Radix_सूचकांक)
        वापस false;

    अगर(sortRows)
    {
        Auto_बफर<Ipp8u> बफर;
        पूर्णांक               बफर_आकार;
        अगर(ipps_Sort_Radix_सूचकांक_Get_बफर_आकार(src.cols, type, &बफर_आकार) < 0)
            वापस false;

        बफर.allocate(बफर_आकार);

        क्रम(पूर्णांक i = 0; i < src.rows; i++)
        {
            अगर(CV_INSTRUMENT_FUN_IPP(ipps_Sort_Radix_सूचकांक, (स्थिर व्योम*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, बफर.तथ्य()) < 0)
                वापस false;
        }
    }
    अन्यथा
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        Auto_बफर<Ipp8u> बफर;
        पूर्णांक               बफर_आकार;
        अगर(ipps_Sort_Radix_सूचकांक_Get_बफर_आकार(src.rows, type, &बफर_आकार) < 0)
            वापस false;

        बफर.allocate(बफर_आकार);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        क्रम(पूर्णांक i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            अगर(CV_INSTRUMENT_FUN_IPP(ipps_Sort_Radix_सूचकांक, (स्थिर व्योम*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, बफर.तथ्य()) < 0)
                वापस false;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    वापस true;
}
#पूर्ण_अगर

प्रकार व्योम (*SortFunc)(स्थिर Mat& src, Mat& dst, पूर्णांक flags);
}

व्योम cv::sort( निवेश_Array _src, OutputArray _dst, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_आकार( src, src.type() );
    Mat dst = _dst.getMat();
#अगर !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#पूर्ण_अगर

    अटल SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<लघु>,
        sort_<पूर्णांक>, sort_<भग्न>, sort_<द्विगुन>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

व्योम cv::sortIdx( निवेश_Array _src, OutputArray _dst, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    अगर( dst.तथ्य == src.तथ्य )
        _dst.release();
    _dst.create_Same_आकार( src, CV_32S );
    dst = _dst.getMat();

#अगर !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#पूर्ण_अगर

    अटल SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<लघु>,
        sortIdx_<पूर्णांक>, sortIdx_<भग्न>, sortIdx_<द्विगुन>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
