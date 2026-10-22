// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

शून्य cv::swap( Mat& a, Mat& b )
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


शून्य cv::hconcat(स्थिरांक Mat* src, माप_प्रकार nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    जर( nsrc == 0 || !src )
    {
        _dst.release();
        परत;
    }

    पूर्णांक totalCols = 0, cols = 0;
    साठी( माप_प्रकार i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    साठी( माप_प्रकार i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

शून्य cv::hconcat(आदान_Array src1, आदान_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

शून्य cv::hconcat(आदान_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

शून्य cv::vconcat(स्थिरांक Mat* src, माप_प्रकार nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    जर( nsrc == 0 || !src )
    {
        _dst.release();
        परत;
    }

    पूर्णांक totalRows = 0, rows = 0;
    साठी( माप_प्रकार i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    साठी( माप_प्रकार i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

शून्य cv::vconcat(आदान_Array src1, आदान_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

शून्य cv::vconcat(आदान_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

नामाकाश cv {

स्थिर तार्किक ocl_setIdentity( आदान_Output_Array _m, स्थिरांक Scalar& s )
{
    पूर्णांक type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    पूर्णांक sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    जर (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        जर (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            जर (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    जर (k.empty())
        परत असत्य;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    माप_प्रकार globalsize[2] = { (माप_प्रकार)m.cols * cn / kercn, ((माप_प्रकार)m.rows + rowsPerWI - 1) / rowsPerWI };
    परत k.run(2, globalsize, nullptr, असत्य);
}

}

#endif

शून्य cv::setIdentity( आदान_Output_Array _m, स्थिरांक Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    पूर्णांक rows = m.rows, cols = m.cols, type = m.type();

    जर( type == CV_32FC1 )
    {
        दशांश* data = m.ptr<दशांश>();
        दशांश val = (दशांश)s[0];
        माप_प्रकार step = m.step/आकार(data[0]);

        साठी( पूर्णांक i = 0; i < rows; i++, data += step )
        {
            साठी( पूर्णांक j = 0; j < cols; j++ )
                data[j] = 0;
            जर( i < cols )
                data[i] = val;
        }
    }
    अन्यथा जर( type == CV_64FC1 )
    {
        द्विगुण* data = m.ptr<द्विगुण>();
        द्विगुण val = s[0];
        माप_प्रकार step = m.step/आकार(data[0]);

        साठी( पूर्णांक i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            जर (i < cols)
                data[i] = val;
        }
    }
    अन्यथा
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


नामाकाश cv {

UMat UMat::eye(पूर्णांक rows, पूर्णांक cols, पूर्णांक type, UMatUsageFlags usageFlags)
{
    परत UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, पूर्णांक type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    परत m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( आदान_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    पूर्णांक type = m.type();
    पूर्णांक nm = std::min(m.rows, m.cols);

    जर( type == CV_32FC1 )
    {
        स्थिरांक दशांश* ptr = m.ptr<दशांश>();
        माप_प्रकार step = m.step/आकार(ptr[0]) + 1;
        द्विगुण _s = 0;
        साठी( पूर्णांक i = 0; i < nm; i++ )
            _s += ptr[i*step];
        परत _s;
    }

    जर( type == CV_64FC1 )
    {
        स्थिरांक द्विगुण* ptr = m.ptr<द्विगुण>();
        माप_प्रकार step = m.step/आकार(ptr[0]) + 1;
        द्विगुण _s = 0;
        साठी( पूर्णांक i = 0; i < nm; i++ )
            _s += ptr[i*step];
        परत _s;
    }

    परत cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

शून्य cv::completeSymm( आदान_Output_Array _m, तार्किक LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    माप_प्रकार step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    पूर्णांक rows = m.rows;
    पूर्णांक j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    साठी( पूर्णांक i = 0; i < rows; i++ )
    {
        जर( !LtoR ) j1 = i; अन्यथा j0 = i+1;
        साठी( पूर्णांक j = j0; j < j1; j++ )
            स_नकल(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(आदान_Array _m) स्थिरांक
{
    Mat m = _m.getMat();
    पूर्णांक tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    जर( d == CV_32F )
    {
        स्थिरांक दशांश *a = (स्थिरांक दशांश*)data, *b = (स्थिरांक दशांश*)m.data;
        दशांश* c = (दशांश*)result.data;
        माप_प्रकार lda = rows > 1 ? step/आकार(a[0]) : 1;
        माप_प्रकार ldb = rows > 1 ? m.step/आकार(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    अन्यथा जर( d == CV_64F )
    {
        स्थिरांक द्विगुण *a = (स्थिरांक द्विगुण*)data, *b = (स्थिरांक द्विगुण*)m.data;
        द्विगुण* c = (द्विगुण*)result.data;
        माप_प्रकार lda = rows > 1 ? step/आकार(a[0]) : 1;
        माप_प्रकार ldb = rows > 1 ? m.step/आकार(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    परत result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

नामाकाश cv
{

प्रकारव्याख्या शून्य (*ReduceSumFunc)(स्थिरांक Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(पूर्णांक sdepth, पूर्णांक ddepth);
ReduceSumFunc getReduceRSumFunc(पूर्णांक sdepth, पूर्णांक ddepth);
ReduceSumFunc getReduceRSum2Func(पूर्णांक sdepth, पूर्णांक ddepth);

ढाँचा <typename T, typename WT, typename Op>
संरचना ReduceR_SIMD
{
    पूर्णांक चालक()(स्थिरांक T*, पूर्णांक start, पूर्णांक, WT*, स्थिरांक Op&) स्थिरांक
    {
        परत start;
    }
};


ढाँचा<typename T, typename ST, typename WT, वर्ग Op, वर्ग OpInit>
वर्ग ReduceR_Invoker : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
  ReduceR_Invoker(स्थिरांक Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  शून्य चालक()(स्थिरांक Range& range) स्थिरांक CV_OVERRIDE
  {
    स्थिरांक T* src = srcmat.ptr<T>();
    स्थिरांक माप_प्रकार srcstep = srcmat.step/आकार(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    पूर्णांक i = 0;

    साठी( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    पूर्णांक height = srcmat.size().height;
    साठी( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #जर CV_ENABLE_UNROLLED
        साठी(; i <= range.end - 4; i += 4 )
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
        साठी( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    साठी( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
खाजगी:
  स्थिरांक Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

ढाँचा<typename T, typename ST, वर्ग Op, वर्ग OpInit = OpNop<ST> > स्थिर शून्य
reduceR_( स्थिरांक Mat& srcmat, Mat& dstmat)
{
    प्रकारव्याख्या typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

ढाँचा<typename T, typename ST, typename WT, वर्ग Op, वर्ग OpInit>
वर्ग ReduceC_Invoker : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
  ReduceC_Invoker(स्थिरांक Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  शून्य चालक()(स्थिरांक Range& range) स्थिरांक CV_OVERRIDE
  {
    स्थिरांक पूर्णांक cn = srcmat.channels();
    स्थिरांक पूर्णांक width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    साठी( पूर्णांक y = range.start; y < range.end; y++ )
    {
        स्थिरांक T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        जर( width == cn )
        {
          साठी( पूर्णांक k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        अन्यथा
        {
            साठी(पूर्णांक k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            साठी(पूर्णांक k = cn ; k < width ; k += cn )
            {
                साठी (पूर्णांक c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            साठी(पूर्णांक k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
खाजगी:
  स्थिरांक Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

ढाँचा<typename T, typename ST, वर्ग Op, वर्ग OpInit = OpNop<ST> > स्थिर शून्य
reduceC_( स्थिरांक Mat& srcmat, Mat& dstmat)
{
    प्रकारव्याख्या typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

प्रकारव्याख्या शून्य (*ReduceFunc)( स्थिरांक Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, पूर्णांक,   OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#define reduceSumR8u32f  reduceR_<uchar, दशांश, OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#define reduceSumR8u64f  reduceR_<uchar, द्विगुण,OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#define reduceSumR16u32f reduceR_<ushort,दशांश, OpAdd<दशांश> >
#define reduceSumR16u64f reduceR_<ushort,द्विगुण,OpAdd<द्विगुण> >
#define reduceSumR16s32f reduceR_<लघु, दशांश, OpAdd<दशांश> >
#define reduceSumR16s64f reduceR_<लघु, द्विगुण,OpAdd<द्विगुण> >
#define reduceSumR32f32f reduceR_<दशांश, दशांश, OpAdd<दशांश> >
#define reduceSumR32f64f reduceR_<दशांश, द्विगुण,OpAdd<द्विगुण> >
#define reduceSumR64f64f reduceR_<द्विगुण,द्विगुण,OpAdd<द्विगुण> >

#define reduceSum2R8u32s  reduceR_<uchar, पूर्णांक,   OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#define reduceSum2R8u32f  reduceR_<uchar, दशांश, OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#define reduceSum2R8u64f  reduceR_<uchar, द्विगुण,OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#define reduceSum2R16u32f reduceR_<ushort,दशांश, OpAddSqr<दशांश>, OpSqr<दशांश> >
#define reduceSum2R16u64f reduceR_<ushort,द्विगुण,OpAddSqr<द्विगुण>,OpSqr<द्विगुण> >
#define reduceSum2R16s32f reduceR_<लघु, दशांश, OpAddSqr<दशांश>, OpSqr<दशांश> >
#define reduceSum2R16s64f reduceR_<लघु, द्विगुण,OpAddSqr<द्विगुण>,OpSqr<द्विगुण> >
#define reduceSum2R32f32f reduceR_<दशांश, दशांश, OpAddSqr<दशांश>, OpSqr<दशांश> >
#define reduceSum2R32f64f reduceR_<दशांश, द्विगुण,OpAddSqr<द्विगुण>,OpSqr<द्विगुण> >
#define reduceSum2R64f64f reduceR_<द्विगुण,द्विगुण,OpAddSqr<द्विगुण>,OpSqr<द्विगुण> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<लघु, लघु, OpMax<लघु> >
#define reduceMaxR32f reduceR_<दशांश, दशांश, OpMax<दशांश> >
#define reduceMaxR64f reduceR_<द्विगुण,द्विगुण,OpMax<द्विगुण> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<लघु, लघु, OpMin<लघु> >
#define reduceMinR32f reduceR_<दशांश, दशांश, OpMin<दशांश> >
#define reduceMinR64f reduceR_<द्विगुण,द्विगुण,OpMin<द्विगुण> >

#जर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
स्थिर ओळीत तार्किक ipp_reduceSumC_8u16u16s32f_64f(स्थिरांक cv::Mat& srcmat, cv::Mat& dstmat)
{
    पूर्णांक sstep = (पूर्णांक)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    प्रकारव्याख्या IppStatus (CV_STDCALL * IppiSum)(स्थिरांक शून्य * pSrc, पूर्णांक srcStep, IppiSize roiSize, Ipp64f* pSum);
    प्रकारव्याख्या IppStatus (CV_STDCALL * IppiSumHint)(स्थिरांक शून्य * pSrc, पूर्णांक srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    जर(ddepth == CV_64F)
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

    जर(ippiSum)
    {
        साठी(पूर्णांक y = 0; y < srcmat.size().height; y++)
        {
            जर(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                परत असत्य;
        }
        परत सत्य;
    }
    अन्यथा जर(ippiSumHint)
    {
        साठी(पूर्णांक y = 0; y < srcmat.size().height; y++)
        {
            जर(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                परत असत्य;
        }
        परत सत्य;
    }

    परत असत्य;
}

स्थिर ओळीत शून्य reduceSumC_8u16u16s32f_64f(स्थिरांक cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    जर(dstmat.depth() == CV_64F)
    {
        पूर्णांक sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, द्विगुण,   cv::OpAdd<द्विगुण> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, द्विगुण,   cv::OpAdd<द्विगुण> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<लघु, द्विगुण,   cv::OpAdd<द्विगुण> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<दशांश, द्विगुण,   cv::OpAdd<द्विगुण> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, पूर्णांक,   OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#define reduceSumC8u32f  reduceC_<uchar, दशांश, OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#define reduceSumC16u32f reduceC_<ushort,दशांश, OpAdd<दशांश> >
#define reduceSumC16s32f reduceC_<लघु, दशांश, OpAdd<दशांश> >
#define reduceSumC32f32f reduceC_<दशांश, दशांश, OpAdd<दशांश> >
#define reduceSumC64f64f reduceC_<द्विगुण,द्विगुण,OpAdd<द्विगुण> >

#define reduceSum2C8u32s  reduceC_<uchar, पूर्णांक,   OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#define reduceSum2C8u32f  reduceC_<uchar, दशांश, OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#define reduceSum2C16u32f reduceC_<ushort,दशांश, OpAddSqr<दशांश>, OpSqr<दशांश> >
#define reduceSum2C16s32f reduceC_<लघु, दशांश, OpAddSqr<दशांश>, OpSqr<दशांश> >
#define reduceSum2C32f32f reduceC_<दशांश, दशांश, OpAddSqr<दशांश>, OpSqr<दशांश> >
#define reduceSum2C64f64f reduceC_<द्विगुण,द्विगुण,OpAddSqr<द्विगुण>,OpSqr<द्विगुण> >

#जर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#अन्यथा
#define reduceSumC8u64f  reduceC_<uchar, द्विगुण,OpAdd<पूर्णांक>, OpNop<पूर्णांक> >
#define reduceSumC16u64f reduceC_<ushort,द्विगुण,OpAdd<द्विगुण> >
#define reduceSumC16s64f reduceC_<लघु, द्विगुण,OpAdd<द्विगुण> >
#define reduceSumC32f64f reduceC_<दशांश, द्विगुण,OpAdd<द्विगुण> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, द्विगुण,OpAddSqr<पूर्णांक>,   OpSqr<पूर्णांक> >
#define reduceSum2C16u64f reduceC_<ushort,द्विगुण,OpAddSqr<द्विगुण>,OpSqr<द्विगुण> >
#define reduceSum2C16s64f reduceC_<लघु, द्विगुण,OpAddSqr<द्विगुण>,OpSqr<द्विगुण> >
#define reduceSum2C32f64f reduceC_<दशांश, द्विगुण,OpAddSqr<द्विगुण>,OpSqr<द्विगुण> >

#जर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
स्थिर ओळीत तार्किक ipp_reduce##optype##C##favor(स्थिरांक cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    जर((srcmat.channels() == 1)) \
    { \
        पूर्णांक sstep = (पूर्णांक)srcmat.step; \
        प्रकारव्याख्या Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        साठी(पूर्णांक y = 0; y < srcmat.size().height; y++)\
        {\
            जर(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                परत असत्य;\
        }\
        परत सत्य;\
    }\
    परत असत्य; \
} \
स्थिर ओळीत शून्य reduce##optype##C##favor(स्थिरांक cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#जर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, लघु, लघु)
REDUCE_OP(32f, Max, दशांश, दशांश)
#अन्यथा
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<लघु, लघु, OpMax<लघु> >
#define reduceMaxC32f reduceC_<दशांश, दशांश, OpMax<दशांश> >
#endif
#define reduceMaxC64f reduceC_<द्विगुण,द्विगुण,OpMax<द्विगुण> >

#जर defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, लघु, लघु)
REDUCE_OP(32f, Min, दशांश, दशांश)
#अन्यथा
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<लघु, लघु, OpMin<लघु> >
#define reduceMinC32f reduceC_<दशांश, दशांश, OpMin<दशांश> >
#endif
#define reduceMinC64f reduceC_<द्विगुण,द्विगुण,OpMin<द्विगुण> >

#ifdef HAVE_OPENCL

नामाकाश cv {

स्थिर तार्किक ocl_reduce(आदान_Array _src, OutputArray _dst,
                       पूर्णांक dim, पूर्णांक op, पूर्णांक op0, पूर्णांक stype, पूर्णांक dtype)
{
    स्थिरांक पूर्णांक min_opt_cols = 128, buf_cols = 32;
    पूर्णांक sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    स्थिरांक ocl::Device &defDev = ocl::Device::getDefault();
    तार्किक doubleSupport = defDev.doubleFPConfig() > 0;

    माप_प्रकार wgs = defDev.maxWorkGroupSize();
    तार्किक useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    जर (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        परत असत्य;

    जर (op == REDUCE_AVG)
    {
        जर (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    स्थिरांक अक्षर * स्थिरांक ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    पूर्णांक wdepth = std::max(ddepth, CV_32F);
    जर (useOptimized)
    {
        माप_प्रकार tileHeight = (माप_प्रकार)(wgs / buf_cols);
        जर (defDev.isIntel())
        {
            स्थिर स्थिरांक माप_प्रकार max_Item_In_Group_मोजणी = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_मोजणी);
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
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], आकार(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], आकार(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], आकार(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        जर (k.empty())
            परत असत्य;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        जर (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        अन्यथा
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        माप_प्रकार localSize[2] = { (माप_प्रकार)buf_cols, (माप_प्रकार)tileHeight};
        माप_प्रकार globalSize[2] = { (माप_प्रकार)buf_cols, (माप_प्रकार)src.rows };
        परत k.run(2, globalSize, localSize, असत्य);
    }
    अन्यथा
    {
        अक्षर cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], आकार(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], आकार(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], आकार(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        जर (k.empty())
            परत असत्य;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        जर (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        अन्यथा
            k.args(srcarg, temparg);

        माप_प्रकार globalsize = std::max(dsize.width, dsize.height);
        परत k.run(1, &globalsize, रिक्त, असत्य);
    }
}

}

#endif

शून्य cv::reduce(आदान_Array _src, OutputArray _dst, पूर्णांक dim, पूर्णांक op, पूर्णांक dtype)
{
    CV_INSTRUMENT_REGION();

    जर (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, सत्य, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        परत;
    }
    पूर्णांक op0 = op;
    पूर्णांक stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    जर( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    पूर्णांक ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    जर (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    जर (src.empty()) {
        src.convertTo(_dst, dtype);
        परत;
    }
    जर (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    जर( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        जर( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    जर( dim == 0 )
    {
        जर( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            जर(simd_func)
                func = (ReduceFunc)simd_func;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            अन्यथा जर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        अन्यथा जर(op == REDUCE_MAX)
        {
            जर(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            अन्यथा जर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        अन्यथा जर(op == REDUCE_MIN)
        {
            जर(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            अन्यथा जर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        अन्यथा जर( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            जर(simd_func)
                func = simd_func;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            अन्यथा जर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    अन्यथा
    {
        जर(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            जर(simd_func)
                func = (ReduceFunc)simd_func;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            अन्यथा जर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        अन्यथा जर(op == REDUCE_MAX)
        {
            जर(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            अन्यथा जर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        अन्यथा जर(op == REDUCE_MIN)
        {
            जर(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            अन्यथा जर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        अन्यथा जर(op == REDUCE_SUM2)
        {
            जर(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            अन्यथा जर(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            अन्यथा जर(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            अन्यथा जर(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            अन्यथा जर(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            अन्यथा जर(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    जर( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    जर( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

नामाकाश cv
{

ढाँचा<typename T> स्थिर शून्य sort_( स्थिरांक Mat& src, Mat& dst, पूर्णांक flags )
{
    पूर्णांक n, len;
    तार्किक sortRows = (flags & 1) == SORT_EVERY_ROW;
    तार्किक inplace = src.data == dst.data;
    तार्किक sortDescending = (flags & SORT_DESCENDING) != 0;

    जर( sortRows )
        n = src.rows, len = src.cols;
    अन्यथा
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](स्थिरांक Range& range)
    {
        AutoBuffer<T> buf;
        जर( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        साठी( पूर्णांक i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            जर( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                जर( !inplace )
                {
                    स्थिरांक T* sptr = src.ptr<T>(i);
                    स_नकल(dptr, sptr, आकार(T) * len);
                }
                ptr = dptr;
            }
            अन्यथा
            {
                साठी( पूर्णांक j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            जर( sortDescending )
            {
                साठी( पूर्णांक j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            जर( !sortRows )
                साठी( पूर्णांक j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#जर defined(HAVE_IPP) && !IPP_DISABLE_SORT
प्रकारव्याख्या IppStatus (CV_STDCALL *IppSortFunc)(शून्य  *pSrcDst, पूर्णांक    len, Ipp8u *pBuffer);

ढाँचा<typename T, IppStatus (CV_STDCALL *fn)(T*, पूर्णांक, Ipp8u*)>
स्थिर IppStatus CV_STDCALL ippSortWrap(शून्य* pSrcDst, पूर्णांक len, Ipp8u* pBuffer)
{
    परत fn((T*)pSrcDst, len, pBuffer);
}

स्थिर IppSortFunc getSortFunc(पूर्णांक depth, तार्किक sortDescending)
{
    जर (!sortDescending)
        परत depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    अन्यथा
        परत depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

स्थिर तार्किक ipp_sort(स्थिरांक Mat& src, Mat& dst, पूर्णांक flags)
{
    CV_INSTRUMENT_REGION_IPP();

    तार्किक        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    तार्किक        sortDescending  = (flags & SORT_DESCENDING) != 0;
    तार्किक        inplace         = (src.data == dst.data);
    पूर्णांक         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    जर(!ippsSortRadix_I)
        परत असत्य;

    पूर्णांक n, len;
    जर( sortRows )
        n = src.rows, len = src.cols;
    अन्यथा
        n = src.cols, len = src.rows;

    पूर्णांक bufferSize;
    जर(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        परत असत्य;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    अस्थिर तार्किक ok = सत्य;
    parallel_for_(Range(0, n), [&](स्थिरांक Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        जर( !sortRows )
            row.create(1, src.rows, src.type());

        साठी( पूर्णांक i = range.start; i < range.end; i++ )
        {
            जर( !ok )
                थांबा;
            शून्य* ptr;
            जर( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                जर( !inplace )
                    स_नकल(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            अन्यथा
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            जर( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = असत्य;
                थांबा;
            }

            जर( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    परत ok;
}
#endif

ढाँचा<typename _Tp> वर्ग LessThanIdx
{
सार्वजनिक:
    LessThanIdx( स्थिरांक _Tp* _arr ) : arr(_arr) {}
    तार्किक चालक()(पूर्णांक a, पूर्णांक b) स्थिरांक { परत arr[a] < arr[b]; }
    स्थिरांक _Tp* arr;
};

ढाँचा<typename T> स्थिर शून्य sortIdx_( स्थिरांक Mat& src, Mat& dst, पूर्णांक flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<पूर्णांक> ibuf;
    तार्किक sortRows = (flags & 1) == SORT_EVERY_ROW;
    तार्किक sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    पूर्णांक n, len;
    जर( sortRows )
        n = src.rows, len = src.cols;
    अन्यथा
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    पूर्णांक* _iptr = ibuf.data();

    साठी( पूर्णांक i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        पूर्णांक* iptr = _iptr;

        जर( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<पूर्णांक>(i);
        }
        अन्यथा
        {
            साठी( पूर्णांक j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        साठी( पूर्णांक j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        जर( sortDescending )
        {
            साठी( पूर्णांक j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        जर( !sortRows )
            साठी( पूर्णांक j = 0; j < len; j++ )
                dst.ptr<पूर्णांक>(j)[i] = iptr[j];
    }
}

#जर defined(HAVE_IPP) && !IPP_DISABLE_SORT
प्रकारव्याख्या IppStatus (CV_STDCALL *IppSortIndexFunc)(स्थिरांक शून्य*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, पूर्णांक len, Ipp8u *pBuffer);

ढाँचा<typename T, IppStatus (CV_STDCALL *fn)(स्थिरांक T*, Ipp32s, Ipp32s*, पूर्णांक, Ipp8u*)>
स्थिर IppStatus CV_STDCALL ippSortIndexWrap(स्थिरांक शून्य* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, पूर्णांक len, Ipp8u *pBuffer)
{
    परत fn((स्थिरांक T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

स्थिर IppSortIndexFunc getSortIndexFunc(पूर्णांक depth, तार्किक sortDescending)
{
    जर (!sortDescending)
        परत depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    अन्यथा
        परत depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

स्थिर तार्किक ipp_sortIdx( स्थिरांक Mat& src, Mat& dst, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION_IPP();

    तार्किक        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    तार्किक        sortDescending  = (flags & SORT_DESCENDING) != 0;
    पूर्णांक         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    जर(!ippsSortRadixIndex)
        परत असत्य;

    जर(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        पूर्णांक               bufferSize;
        जर(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            परत असत्य;

        buffer.allocate(bufferSize);

        साठी(पूर्णांक i = 0; i < src.rows; i++)
        {
            जर(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (स्थिरांक शून्य*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                परत असत्य;
        }
    }
    अन्यथा
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        पूर्णांक               bufferSize;
        जर(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            परत असत्य;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        साठी(पूर्णांक i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            जर(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (स्थिरांक शून्य*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                परत असत्य;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    परत सत्य;
}
#endif

प्रकारव्याख्या शून्य (*SortFunc)(स्थिरांक Mat& src, Mat& dst, पूर्णांक flags);
}

शून्य cv::sort( आदान_Array _src, OutputArray _dst, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#जर !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    स्थिर SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<लघु>,
        sort_<पूर्णांक>, sort_<दशांश>, sort_<द्विगुण>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

शून्य cv::sortIdx( आदान_Array _src, OutputArray _dst, पूर्णांक flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    जर( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#जर !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    स्थिर SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<लघु>,
        sortIdx_<पूर्णांक>, sortIdx_<दशांश>, sortIdx_<द्विगुण>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
