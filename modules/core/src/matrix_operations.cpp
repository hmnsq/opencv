// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ "opencv2/core/mat.hpp"
#অন্তর্ভাৱ "opencl_kernels_core.hpp"

#define IPP_DISABLE_REDUCE 1
#define IPP_DISABLE_SORT   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

শূন্য cv::swap( Mat& a, Mat& b )
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


শূন্য cv::hconcat(ধ্রুৱ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    যদি( nsrc == 0 || !src )
    {
        _dst.release();
        ঘূৰাই;
    }

    পূর্ণাঙ্ক totalCols = 0, cols = 0;
    বাবে( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        totalCols += src[i].cols;
    }
    _dst.create( src[0].rows, totalCols, src[0].type());
    Mat dst = _dst.getMat();
    বাবে( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

শূন্য cv::hconcat(প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

শূন্য cv::hconcat(প্ৰবেশ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

শূন্য cv::vconcat(ধ্রুৱ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    যদি( nsrc == 0 || !src )
    {
        _dst.release();
        ঘূৰাই;
    }

    পূর্ণাঙ্ক totalRows = 0, rows = 0;
    বাবে( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        totalRows += src[i].rows;
    }
    _dst.create( totalRows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    বাবে( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

শূন্য cv::vconcat(প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

শূন্য cv::vconcat(প্ৰবেশ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির তার্কিক ocl_setIdentity( প্ৰবেশ_Output_Array _m, ধ্রুৱ Scalar& s )
{
    পূর্ণাঙ্ক type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    পূর্ণাঙ্ক sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    যদি (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        যদি (cn == 1)
        {
            kercn = std::min(ocl::predictOptimalVectorWidth(_m), 4);
            যদি (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    যদি (k.empty())
        ঘূৰাই মিছা;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    ঘূৰাই k.run(2, globalsize, nullptr, মিছা);
}

}

#endif

শূন্য cv::setIdentity( প্ৰবেশ_Output_Array _m, ধ্রুৱ Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    পূর্ণাঙ্ক rows = m.rows, cols = m.cols, type = m.type();

    যদি( type == CV_32FC1 )
    {
        ভিন্ন* data = m.ptr<ভিন্ন>();
        ভিন্ন val = (ভিন্ন)s[0];
        size_t step = m.step/পরিমাণ(data[0]);

        বাবে( পূর্ণাঙ্ক i = 0; i < rows; i++, data += step )
        {
            বাবে( পূর্ণাঙ্ক j = 0; j < cols; j++ )
                data[j] = 0;
            যদি( i < cols )
                data[i] = val;
        }
    }
    অন্যথা যদি( type == CV_64FC1 )
    {
        দ্ৱিগুণ* data = m.ptr<দ্ৱিগুণ>();
        দ্ৱিগুণ val = s[0];
        size_t step = m.step/পরিমাণ(data[0]);

        বাবে( পূর্ণাঙ্ক i = 0; i < rows; i++, data += step )
        {
            std::fill(data, data + cols, 0.0);
            যদি (i < cols)
                data[i] = val;
        }
    }
    অন্যথা
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


নামাকাশ cv {

UMat UMat::eye(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    ঘূৰাই UMat::eye(Size(cols, rows), type, usageFlags);
}

UMat UMat::eye(Size size, পূর্ণাঙ্ক type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    ঘূৰাই m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( প্ৰবেশ_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    পূর্ণাঙ্ক type = m.type();
    পূর্ণাঙ্ক nm = std::min(m.rows, m.cols);

    যদি( type == CV_32FC1 )
    {
        ধ্রুৱ ভিন্ন* ptr = m.ptr<ভিন্ন>();
        size_t step = m.step/পরিমাণ(ptr[0]) + 1;
        দ্ৱিগুণ _s = 0;
        বাবে( পূর্ণাঙ্ক i = 0; i < nm; i++ )
            _s += ptr[i*step];
        ঘূৰাই _s;
    }

    যদি( type == CV_64FC1 )
    {
        ধ্রুৱ দ্ৱিগুণ* ptr = m.ptr<দ্ৱিগুণ>();
        size_t step = m.step/পরিমাণ(ptr[0]) + 1;
        দ্ৱিগুণ _s = 0;
        বাবে( পূর্ণাঙ্ক i = 0; i < nm; i++ )
            _s += ptr[i*step];
        ঘূৰাই _s;
    }

    ঘূৰাই cv::sum(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

শূন্য cv::completeSymm( প্ৰবেশ_Output_Array _m, তার্কিক LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elemSize();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    পূর্ণাঙ্ক rows = m.rows;
    পূর্ণাঙ্ক j0 = 0, j1 = rows;

    uchar* data = m.ptr();
    বাবে( পূর্ণাঙ্ক i = 0; i < rows; i++ )
    {
        যদি( !LtoR ) j1 = i; অন্যথা j0 = i+1;
        বাবে( পূর্ণাঙ্ক j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(প্ৰবেশ_Array _m) ধ্রুৱ
{
    Mat m = _m.getMat();
    পূর্ণাঙ্ক tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat result(rows, cols, tp);

    যদি( d == CV_32F )
    {
        ধ্রুৱ ভিন্ন *a = (ধ্রুৱ ভিন্ন*)data, *b = (ধ্রুৱ ভিন্ন*)m.data;
        ভিন্ন* c = (ভিন্ন*)result.data;
        size_t lda = rows > 1 ? step/পরিমাণ(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/পরিমাণ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    অন্যথা যদি( d == CV_64F )
    {
        ধ্রুৱ দ্ৱিগুণ *a = (ধ্রুৱ দ্ৱিগুণ*)data, *b = (ধ্রুৱ দ্ৱিগুণ*)m.data;
        দ্ৱিগুণ* c = (দ্ৱিগুণ*)result.data;
        size_t lda = rows > 1 ? step/পরিমাণ(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/পরিমাণ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    ঘূৰাই result;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

নামাকাশ cv
{

প্রকারনির্দেশ শূন্য (*ReduceSumFunc)(ধ্রুৱ Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth);
ReduceSumFunc getReduceRSumFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth);
ReduceSumFunc getReduceRSum2Func(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth);

template <typename T, typename WT, typename Op>
সংরচনা ReduceR_SIMD
{
    পূর্ণাঙ্ক operator()(ধ্রুৱ T*, পূর্ণাঙ্ক start, পূর্ণাঙ্ক, WT*, ধ্রুৱ Op&) ধ্রুৱ
    {
        ঘূৰাই start;
    }
};


template<typename T, typename ST, typename WT, শ্ৰেণী Op, শ্ৰেণী OpInit>
শ্ৰেণী ReduceR_Invoker : সার্ৱজনিক ParallelLoopBody
{
সার্ৱজনিক:
  ReduceR_Invoker(ধ্রুৱ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),buffer(srcmat.size().width*srcmat.channels())
  {
  }
  শূন্য operator()(ধ্রুৱ Range& range) ধ্রুৱ CV_OVERRIDE
  {
    ধ্রুৱ T* src = srcmat.ptr<T>();
    ধ্রুৱ size_t srcstep = srcmat.step/পরিমাণ(src[0]);
    WT* buf = buffer.data();
    ST* dst = dstmat.ptr<ST>();
    পূর্ণাঙ্ক i = 0;

    বাবে( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    পূর্ণাঙ্ক height = srcmat.size().height;
    বাবে( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #যদি CV_ENABLE_UNROLLED
        বাবে(; i <= range.end - 4; i += 4 )
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
        বাবে( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    বাবে( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
গুপ্ত:
  ধ্রুৱ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable AutoBuffer<WT> buffer;
};

template<typename T, typename ST, শ্ৰেণী Op, শ্ৰেণী OpInit = OpNop<ST> > স্থির শূন্য
reduceR_( ধ্রুৱ Mat& srcmat, Mat& dstmat)
{
    প্রকারনির্দেশ typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_SIZE(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, শ্ৰেণী Op, শ্ৰেণী OpInit>
শ্ৰেণী ReduceC_Invoker : সার্ৱজনিক ParallelLoopBody
{
সার্ৱজনিক:
  ReduceC_Invoker(ধ্রুৱ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  শূন্য operator()(ধ্রুৱ Range& range) ধ্রুৱ CV_OVERRIDE
  {
    ধ্রুৱ পূর্ণাঙ্ক cn = srcmat.channels();
    ধ্রুৱ পূর্ণাঙ্ক width = srcmat.size().width*cn;
    AutoBuffer<WT> cumul(cn);
    বাবে( পূর্ণাঙ্ক y = range.start; y < range.end; y++ )
    {
        ধ্রুৱ T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        যদি( width == cn )
        {
          বাবে( পূর্ণাঙ্ক k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        অন্যথা
        {
            বাবে(পূর্ণাঙ্ক k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            বাবে(পূর্ণাঙ্ক k = cn ; k < width ; k += cn )
            {
                বাবে (পূর্ণাঙ্ক c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            বাবে(পূর্ণাঙ্ক k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
গুপ্ত:
  ধ্রুৱ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, শ্ৰেণী Op, শ্ৰেণী OpInit = OpNop<ST> > স্থির শূন্য
reduceC_( ধ্রুৱ Mat& srcmat, Mat& dstmat)
{
    প্রকারনির্দেশ typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

প্রকারনির্দেশ শূন্য (*ReduceFunc)( ধ্রুৱ Mat& src, Mat& dst );

}

#define reduceSumR8u32s  reduceR_<uchar, পূর্ণাঙ্ক,   OpAdd<পূর্ণাঙ্ক>, OpNop<পূর্ণাঙ্ক> >
#define reduceSumR8u32f  reduceR_<uchar, ভিন্ন, OpAdd<পূর্ণাঙ্ক>, OpNop<পূর্ণাঙ্ক> >
#define reduceSumR8u64f  reduceR_<uchar, দ্ৱিগুণ,OpAdd<পূর্ণাঙ্ক>, OpNop<পূর্ণাঙ্ক> >
#define reduceSumR16u32f reduceR_<ushort,ভিন্ন, OpAdd<ভিন্ন> >
#define reduceSumR16u64f reduceR_<ushort,দ্ৱিগুণ,OpAdd<দ্ৱিগুণ> >
#define reduceSumR16s32f reduceR_<short, ভিন্ন, OpAdd<ভিন্ন> >
#define reduceSumR16s64f reduceR_<short, দ্ৱিগুণ,OpAdd<দ্ৱিগুণ> >
#define reduceSumR32f32f reduceR_<ভিন্ন, ভিন্ন, OpAdd<ভিন্ন> >
#define reduceSumR32f64f reduceR_<ভিন্ন, দ্ৱিগুণ,OpAdd<দ্ৱিগুণ> >
#define reduceSumR64f64f reduceR_<দ্ৱিগুণ,দ্ৱিগুণ,OpAdd<দ্ৱিগুণ> >

#define reduceSum2R8u32s  reduceR_<uchar, পূর্ণাঙ্ক,   OpAddSqr<পূর্ণাঙ্ক>,   OpSqr<পূর্ণাঙ্ক> >
#define reduceSum2R8u32f  reduceR_<uchar, ভিন্ন, OpAddSqr<পূর্ণাঙ্ক>,   OpSqr<পূর্ণাঙ্ক> >
#define reduceSum2R8u64f  reduceR_<uchar, দ্ৱিগুণ,OpAddSqr<পূর্ণাঙ্ক>,   OpSqr<পূর্ণাঙ্ক> >
#define reduceSum2R16u32f reduceR_<ushort,ভিন্ন, OpAddSqr<ভিন্ন>, OpSqr<ভিন্ন> >
#define reduceSum2R16u64f reduceR_<ushort,দ্ৱিগুণ,OpAddSqr<দ্ৱিগুণ>,OpSqr<দ্ৱিগুণ> >
#define reduceSum2R16s32f reduceR_<short, ভিন্ন, OpAddSqr<ভিন্ন>, OpSqr<ভিন্ন> >
#define reduceSum2R16s64f reduceR_<short, দ্ৱিগুণ,OpAddSqr<দ্ৱিগুণ>,OpSqr<দ্ৱিগুণ> >
#define reduceSum2R32f32f reduceR_<ভিন্ন, ভিন্ন, OpAddSqr<ভিন্ন>, OpSqr<ভিন্ন> >
#define reduceSum2R32f64f reduceR_<ভিন্ন, দ্ৱিগুণ,OpAddSqr<দ্ৱিগুণ>,OpSqr<দ্ৱিগুণ> >
#define reduceSum2R64f64f reduceR_<দ্ৱিগুণ,দ্ৱিগুণ,OpAddSqr<দ্ৱিগুণ>,OpSqr<দ্ৱিগুণ> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<ভিন্ন, ভিন্ন, OpMax<ভিন্ন> >
#define reduceMaxR64f reduceR_<দ্ৱিগুণ,দ্ৱিগুণ,OpMax<দ্ৱিগুণ> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<ভিন্ন, ভিন্ন, OpMin<ভিন্ন> >
#define reduceMinR64f reduceR_<দ্ৱিগুণ,দ্ৱিগুণ,OpMin<দ্ৱিগুণ> >

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
স্থির অন্তঃস্থ তার্কিক ipp_reduceSumC_8u16u16s32f_64f(ধ্রুৱ cv::Mat& srcmat, cv::Mat& dstmat)
{
    পূর্ণাঙ্ক sstep = (পূর্ণাঙ্ক)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    IppiSize roisize = { srcmat.size().width, 1 };

    প্রকারনির্দেশ IppStatus (CV_STDCALL * IppiSum)(ধ্রুৱ শূন্য * pSrc, পূর্ণাঙ্ক srcStep, IppiSize roiSize, Ipp64f* pSum);
    প্রকারনির্দেশ IppStatus (CV_STDCALL * IppiSumHint)(ধ্রুৱ শূন্য * pSrc, পূর্ণাঙ্ক srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    যদি(ddepth == CV_64F)
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

    যদি(ippiSum)
    {
        বাবে(পূর্ণাঙ্ক y = 0; y < srcmat.size().height; y++)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                ঘূৰাই মিছা;
        }
        ঘূৰাই সঁচা;
    }
    অন্যথা যদি(ippiSumHint)
    {
        বাবে(পূর্ণাঙ্ক y = 0; y < srcmat.size().height; y++)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                ঘূৰাই মিছা;
        }
        ঘূৰাই সঁচা;
    }

    ঘূৰাই মিছা;
}

স্থির অন্তঃস্থ শূন্য reduceSumC_8u16u16s32f_64f(ধ্রুৱ cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    যদি(dstmat.depth() == CV_64F)
    {
        পূর্ণাঙ্ক sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, দ্ৱিগুণ,   cv::OpAdd<দ্ৱিগুণ> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, দ্ৱিগুণ,   cv::OpAdd<দ্ৱিগুণ> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, দ্ৱিগুণ,   cv::OpAdd<দ্ৱিগুণ> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<ভিন্ন, দ্ৱিগুণ,   cv::OpAdd<দ্ৱিগুণ> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, পূর্ণাঙ্ক,   OpAdd<পূর্ণাঙ্ক>, OpNop<পূর্ণাঙ্ক> >
#define reduceSumC8u32f  reduceC_<uchar, ভিন্ন, OpAdd<পূর্ণাঙ্ক>, OpNop<পূর্ণাঙ্ক> >
#define reduceSumC16u32f reduceC_<ushort,ভিন্ন, OpAdd<ভিন্ন> >
#define reduceSumC16s32f reduceC_<short, ভিন্ন, OpAdd<ভিন্ন> >
#define reduceSumC32f32f reduceC_<ভিন্ন, ভিন্ন, OpAdd<ভিন্ন> >
#define reduceSumC64f64f reduceC_<দ্ৱিগুণ,দ্ৱিগুণ,OpAdd<দ্ৱিগুণ> >

#define reduceSum2C8u32s  reduceC_<uchar, পূর্ণাঙ্ক,   OpAddSqr<পূর্ণাঙ্ক>,   OpSqr<পূর্ণাঙ্ক> >
#define reduceSum2C8u32f  reduceC_<uchar, ভিন্ন, OpAddSqr<পূর্ণাঙ্ক>,   OpSqr<পূর্ণাঙ্ক> >
#define reduceSum2C16u32f reduceC_<ushort,ভিন্ন, OpAddSqr<ভিন্ন>, OpSqr<ভিন্ন> >
#define reduceSum2C16s32f reduceC_<short, ভিন্ন, OpAddSqr<ভিন্ন>, OpSqr<ভিন্ন> >
#define reduceSum2C32f32f reduceC_<ভিন্ন, ভিন্ন, OpAddSqr<ভিন্ন>, OpSqr<ভিন্ন> >
#define reduceSum2C64f64f reduceC_<দ্ৱিগুণ,দ্ৱিগুণ,OpAddSqr<দ্ৱিগুণ>,OpSqr<দ্ৱিগুণ> >

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#অন্যথা
#define reduceSumC8u64f  reduceC_<uchar, দ্ৱিগুণ,OpAdd<পূর্ণাঙ্ক>, OpNop<পূর্ণাঙ্ক> >
#define reduceSumC16u64f reduceC_<ushort,দ্ৱিগুণ,OpAdd<দ্ৱিগুণ> >
#define reduceSumC16s64f reduceC_<short, দ্ৱিগুণ,OpAdd<দ্ৱিগুণ> >
#define reduceSumC32f64f reduceC_<ভিন্ন, দ্ৱিগুণ,OpAdd<দ্ৱিগুণ> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, দ্ৱিগুণ,OpAddSqr<পূর্ণাঙ্ক>,   OpSqr<পূর্ণাঙ্ক> >
#define reduceSum2C16u64f reduceC_<ushort,দ্ৱিগুণ,OpAddSqr<দ্ৱিগুণ>,OpSqr<দ্ৱিগুণ> >
#define reduceSum2C16s64f reduceC_<short, দ্ৱিগুণ,OpAddSqr<দ্ৱিগুণ>,OpSqr<দ্ৱিগুণ> >
#define reduceSum2C32f64f reduceC_<ভিন্ন, দ্ৱিগুণ,OpAddSqr<দ্ৱিগুণ>,OpSqr<দ্ৱিগুণ> >

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
স্থির অন্তঃস্থ তার্কিক ipp_reduce##optype##C##favor(ধ্রুৱ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    যদি((srcmat.channels() == 1)) \
    { \
        পূর্ণাঙ্ক sstep = (পূর্ণাঙ্ক)srcmat.step; \
        প্রকারনির্দেশ Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        বাবে(পূর্ণাঙ্ক y = 0; y < srcmat.size().height; y++)\
        {\
            যদি(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                ঘূৰাই মিছা;\
        }\
        ঘূৰাই সঁচা;\
    }\
    ঘূৰাই মিছা; \
} \
স্থির অন্তঃস্থ শূন্য reduce##optype##C##favor(ধ্রুৱ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, ভিন্ন, ভিন্ন)
#অন্যথা
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<ভিন্ন, ভিন্ন, OpMax<ভিন্ন> >
#endif
#define reduceMaxC64f reduceC_<দ্ৱিগুণ,দ্ৱিগুণ,OpMax<দ্ৱিগুণ> >

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, ভিন্ন, ভিন্ন)
#অন্যথা
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<ভিন্ন, ভিন্ন, OpMin<ভিন্ন> >
#endif
#define reduceMinC64f reduceC_<দ্ৱিগুণ,দ্ৱিগুণ,OpMin<দ্ৱিগুণ> >

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির তার্কিক ocl_reduce(প্ৰবেশ_Array _src, OutputArray _dst,
                       পূর্ণাঙ্ক dim, পূর্ণাঙ্ক op, পূর্ণাঙ্ক op0, পূর্ণাঙ্ক stype, পূর্ণাঙ্ক dtype)
{
    ধ্রুৱ পূর্ণাঙ্ক min_opt_cols = 128, buf_cols = 32;
    পূর্ণাঙ্ক sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    ধ্রুৱ ocl::Device &defDev = ocl::Device::getDefault();
    তার্কিক doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    তার্কিক useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    যদি (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        ঘূৰাই মিছা;

    যদি (op == REDUCE_AVG)
    {
        যদি (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ধ্রুৱ ৱর্ণ * ধ্রুৱ ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    পূর্ণাঙ্ক wdepth = std::max(ddepth, CV_32F);
    যদি (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        যদি (defDev.isIntel())
        {
            স্থির ধ্রুৱ size_t max_Item_In_Group_গুনতি = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CV_ELEM_SIZE(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_গুনতি);
        }
        ৱর্ণ cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], পরিমাণ(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], পরিমাণ(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], পরিমাণ(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        যদি (k.empty())
            ঘূৰাই মিছা;
        UMat src = _src.getUMat();
        Size dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        যদি (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src.cols);
        অন্যথা
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src.rows };
        ঘূৰাই k.run(2, globalSize, localSize, মিছা);
    }
    অন্যথা
    {
        ৱর্ণ cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], পরিমাণ(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], পরিমাণ(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], পরিমাণ(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        যদি (k.empty())
            ঘূৰাই মিছা;

        UMat src = _src.getUMat();
        Size dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        যদি (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        অন্যথা
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        ঘূৰাই k.run(1, &globalsize, রিক্ত, মিছা);
    }
}

}

#endif

শূন্য cv::reduce(প্ৰবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক dim, পূর্ণাঙ্ক op, পূর্ণাঙ্ক dtype)
{
    CV_INSTRUMENT_REGION();

    যদি (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, সঁচা, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        ঘূৰাই;
    }
    পূর্ণাঙ্ক op0 = op;
    পূর্ণাঙ্ক stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    যদি( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    পূর্ণাঙ্ক ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_SUM || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_RUN(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    যদি (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    যদি (src.empty()) {
        src.convertTo(_dst, dtype);
        ঘূৰাই;
    }
    যদি (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), temp = dst;

    যদি( op == REDUCE_AVG )
    {
        op = REDUCE_SUM;
        যদি( sdepth < CV_32S && ddepth < CV_32S )
        {
            temp.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    যদি( dim == 0 )
    {
        যদি( op == REDUCE_SUM )
        {
            ReduceSumFunc simd_func = getReduceRSumFunc(sdepth, ddepth);
            যদি(simd_func)
                func = (ReduceFunc)simd_func;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumR8u32s;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumR8u32f;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumR8u64f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumR16u32f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumR16u64f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumR16s32f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumR16s64f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumR32f32f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumR32f64f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumR64f64f;
        }
        অন্যথা যদি(op == REDUCE_MAX)
        {
            যদি(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        অন্যথা যদি(op == REDUCE_MIN)
        {
            যদি(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        অন্যথা যদি( op == REDUCE_SUM2 )
        {
            ReduceSumFunc simd_func = getReduceRSum2Func(sdepth, ddepth);
            যদি(simd_func)
                func = simd_func;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    অন্যথা
    {
        যদি(op == REDUCE_SUM)
        {
            ReduceSumFunc simd_func = getReduceCSumFunc(sdepth, ddepth);
            যদি(simd_func)
                func = (ReduceFunc)simd_func;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSumC8u32s;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSumC8u32f;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSumC8u64f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSumC16u32f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSumC16u64f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSumC16s32f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSumC16s64f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSumC32f32f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSumC32f64f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSumC64f64f;
        }
        অন্যথা যদি(op == REDUCE_MAX)
        {
            যদি(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        অন্যথা যদি(op == REDUCE_MIN)
        {
            যদি(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        অন্যথা যদি(op == REDUCE_SUM2)
        {
            যদি(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    যদি( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    যদি( op0 == REDUCE_AVG )
        temp.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

নামাকাশ cv
{

template<typename T> স্থির শূন্য sort_( ধ্রুৱ Mat& src, Mat& dst, পূর্ণাঙ্ক flags )
{
    পূর্ণাঙ্ক n, len;
    তার্কিক sortRows = (flags & 1) == SORT_EVERY_ROW;
    তার্কিক inplace = src.data == dst.data;
    তার্কিক sortDescending = (flags & SORT_DESCENDING) != 0;

    যদি( sortRows )
        n = src.rows, len = src.cols;
    অন্যথা
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](ধ্রুৱ Range& range)
    {
        AutoBuffer<T> buf;
        যদি( !sortRows )
            buf.allocate(len);
        T* bptr = buf.data();

        বাবে( পূর্ণাঙ্ক i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            যদি( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                যদি( !inplace )
                {
                    ধ্রুৱ T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, পরিমাণ(T) * len);
                }
                ptr = dptr;
            }
            অন্যথা
            {
                বাবে( পূর্ণাঙ্ক j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            যদি( sortDescending )
            {
                বাবে( পূর্ণাঙ্ক j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            যদি( !sortRows )
                বাবে( পূর্ণাঙ্ক j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#যদি defined(HAVE_IPP) && !IPP_DISABLE_SORT
প্রকারনির্দেশ IppStatus (CV_STDCALL *IppSortFunc)(শূন্য  *pSrcDst, পূর্ণাঙ্ক    len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, পূর্ণাঙ্ক, Ipp8u*)>
স্থির IppStatus CV_STDCALL ippSortWrap(শূন্য* pSrcDst, পূর্ণাঙ্ক len, Ipp8u* pBuffer)
{
    ঘূৰাই fn((T*)pSrcDst, len, pBuffer);
}

স্থির IppSortFunc getSortFunc(পূর্ণাঙ্ক depth, তার্কিক sortDescending)
{
    যদি (!sortDescending)
        ঘূৰাই depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    অন্যথা
        ঘূৰাই depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

স্থির তার্কিক ipp_sort(ধ্রুৱ Mat& src, Mat& dst, পূর্ণাঙ্ক flags)
{
    CV_INSTRUMENT_REGION_IPP();

    তার্কিক        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    তার্কিক        sortDescending  = (flags & SORT_DESCENDING) != 0;
    তার্কিক        inplace         = (src.data == dst.data);
    পূর্ণাঙ্ক         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    যদি(!ippsSortRadix_I)
        ঘূৰাই মিছা;

    পূর্ণাঙ্ক n, len;
    যদি( sortRows )
        n = src.rows, len = src.cols;
    অন্যথা
        n = src.cols, len = src.rows;

    পূর্ণাঙ্ক bufferSize;
    যদি(ippsSortRadixGetBufferSize(len, type, &bufferSize) < 0)
        ঘূৰাই মিছা;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    চঞ্চল তার্কিক ok = সঁচা;
    parallel_for_(Range(0, n), [&](ধ্রুৱ Range& range)
    {
        AutoBuffer<Ipp8u> buffer(bufferSize);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        যদি( !sortRows )
            row.create(1, src.rows, src.type());

        বাবে( পূর্ণাঙ্ক i = range.start; i < range.end; i++ )
        {
            যদি( !ok )
                ভঙ্গ;
            শূন্য* ptr;
            যদি( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                যদি( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elemSize());
                ptr = dptr;
            }
            অন্যথা
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            যদি( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, buffer.data()) < 0 )
            {
                ok = মিছা;
                ভঙ্গ;
            }

            যদি( !sortRows )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.rows);
                row.copyTo(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    ঘূৰাই ok;
}
#endif

template<typename _Tp> শ্ৰেণী LessThanIdx
{
সার্ৱজনিক:
    LessThanIdx( ধ্রুৱ _Tp* _arr ) : arr(_arr) {}
    তার্কিক operator()(পূর্ণাঙ্ক a, পূর্ণাঙ্ক b) ধ্রুৱ { ঘূৰাই arr[a] < arr[b]; }
    ধ্রুৱ _Tp* arr;
};

template<typename T> স্থির শূন্য sortIdx_( ধ্রুৱ Mat& src, Mat& dst, পূর্ণাঙ্ক flags )
{
    AutoBuffer<T> buf;
    AutoBuffer<পূর্ণাঙ্ক> ibuf;
    তার্কিক sortRows = (flags & 1) == SORT_EVERY_ROW;
    তার্কিক sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.data != dst.data );

    পূর্ণাঙ্ক n, len;
    যদি( sortRows )
        n = src.rows, len = src.cols;
    অন্যথা
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.data();
    পূর্ণাঙ্ক* _iptr = ibuf.data();

    বাবে( পূর্ণাঙ্ক i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        পূর্ণাঙ্ক* iptr = _iptr;

        যদি( sortRows )
        {
            ptr = (T*)(src.data + src.step*i);
            iptr = dst.ptr<পূর্ণাঙ্ক>(i);
        }
        অন্যথা
        {
            বাবে( পূর্ণাঙ্ক j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        বাবে( পূর্ণাঙ্ক j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        যদি( sortDescending )
        {
            বাবে( পূর্ণাঙ্ক j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        যদি( !sortRows )
            বাবে( পূর্ণাঙ্ক j = 0; j < len; j++ )
                dst.ptr<পূর্ণাঙ্ক>(j)[i] = iptr[j];
    }
}

#যদি defined(HAVE_IPP) && !IPP_DISABLE_SORT
প্রকারনির্দেশ IppStatus (CV_STDCALL *IppSortIndexFunc)(ধ্রুৱ শূন্য*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, পূর্ণাঙ্ক len, Ipp8u *pBuffer);

template<typename T, IppStatus (CV_STDCALL *fn)(ধ্রুৱ T*, Ipp32s, Ipp32s*, পূর্ণাঙ্ক, Ipp8u*)>
স্থির IppStatus CV_STDCALL ippSortIndexWrap(ধ্রুৱ শূন্য* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, পূর্ণাঙ্ক len, Ipp8u *pBuffer)
{
    ঘূৰাই fn((ধ্রুৱ T*)pSrc, srcStrideBytes, pDstIndx, len, pBuffer);
}

স্থির IppSortIndexFunc getSortIndexFunc(পূর্ণাঙ্ক depth, তার্কিক sortDescending)
{
    যদি (!sortDescending)
        ঘূৰাই depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexAscend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexAscend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexAscend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexAscend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexAscend_32f> :
            0;
    অন্যথা
        ঘূৰাই depth == CV_8U ? ippSortIndexWrap<Ipp8u, ippsSortRadixIndexDescend_8u> :
            depth == CV_16U ? ippSortIndexWrap<Ipp16u, ippsSortRadixIndexDescend_16u> :
            depth == CV_16S ? ippSortIndexWrap<Ipp16s, ippsSortRadixIndexDescend_16s> :
            depth == CV_32S ? ippSortIndexWrap<Ipp32s, ippsSortRadixIndexDescend_32s> :
            depth == CV_32F ? ippSortIndexWrap<Ipp32f, ippsSortRadixIndexDescend_32f> :
            0;
}

স্থির তার্কিক ipp_sortIdx( ধ্রুৱ Mat& src, Mat& dst, পূর্ণাঙ্ক flags )
{
    CV_INSTRUMENT_REGION_IPP();

    তার্কিক        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    তার্কিক        sortDescending  = (flags & SORT_DESCENDING) != 0;
    পূর্ণাঙ্ক         depth           = src.depth();
    IppDataType type            = ippiGetDataType(depth);

    IppSortIndexFunc ippsSortRadixIndex = getSortIndexFunc(depth, sortDescending);
    যদি(!ippsSortRadixIndex)
        ঘূৰাই মিছা;

    যদি(sortRows)
    {
        AutoBuffer<Ipp8u> buffer;
        পূর্ণাঙ্ক               bufferSize;
        যদি(ippsSortRadixIndexGetBufferSize(src.cols, type, &bufferSize) < 0)
            ঘূৰাই মিছা;

        buffer.allocate(bufferSize);

        বাবে(পূর্ণাঙ্ক i = 0; i < src.rows; i++)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ধ্রুৱ শূন্য*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, buffer.data()) < 0)
                ঘূৰাই মিছা;
        }
    }
    অন্যথা
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        AutoBuffer<Ipp8u> buffer;
        পূর্ণাঙ্ক               bufferSize;
        যদি(ippsSortRadixIndexGetBufferSize(src.rows, type, &bufferSize) < 0)
            ঘূৰাই মিছা;

        buffer.allocate(bufferSize);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        বাবে(পূর্ণাঙ্ক i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            যদি(CV_INSTRUMENT_FUN_IPP(ippsSortRadixIndex, (ধ্রুৱ শূন্য*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, buffer.data()) < 0)
                ঘূৰাই মিছা;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    ঘূৰাই সঁচা;
}
#endif

প্রকারনির্দেশ শূন্য (*SortFunc)(ধ্রুৱ Mat& src, Mat& dst, পূর্ণাঙ্ক flags);
}

শূন্য cv::sort( প্ৰবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.createSameSize( src, src.type() );
    Mat dst = _dst.getMat();
#যদি !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    স্থির SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<পূর্ণাঙ্ক>, sort_<ভিন্ন>, sort_<দ্ৱিগুণ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

শূন্য cv::sortIdx( প্ৰবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    যদি( dst.data == src.data )
        _dst.release();
    _dst.createSameSize( src, CV_32S );
    dst = _dst.getMat();

#যদি !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    স্থির SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<পূর্ণাঙ্ক>, sortIdx_<ভিন্ন>, sortIdx_<দ্ৱিগুণ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
