// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/mat.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

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
    std::swap(a.তথ্য, b.তথ্য);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.step, b.step);
}


শূন্য cv::hconcat(ধ্রুবক Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    যদি( nsrc == 0 || !src )
    {
        _dst.release();
        ফেরত;
    }

    পূর্ণসংখ্যা যোগ_Cols = 0, cols = 0;
    জন্য( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].rows == src[0].rows &&
                   src[i].type() == src[0].type());
        যোগ_Cols += src[i].cols;
    }
    _dst.create( src[0].rows, যোগ_Cols, src[0].type());
    Mat dst = _dst.getMat();
    জন্য( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(cols, 0, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        cols += src[i].cols;
    }
}

শূন্য cv::hconcat(প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

শূন্য cv::hconcat(প্রবেশ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    hconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

শূন্য cv::vconcat(ধ্রুবক Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    যদি( nsrc == 0 || !src )
    {
        _dst.release();
        ফেরত;
    }

    পূর্ণসংখ্যা যোগ_Rows = 0, rows = 0;
    জন্য( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].cols == src[0].cols &&
                  src[i].type() == src[0].type());
        যোগ_Rows += src[i].rows;
    }
    _dst.create( যোগ_Rows, src[0].cols, src[0].type());
    Mat dst = _dst.getMat();
    জন্য( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, rows, src[i].cols, src[i].rows));
        src[i].copyTo(dpart);
        rows += src[i].rows;
    }
}

শূন্য cv::vconcat(প্রবেশ_Array src1, প্রবেশ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

শূন্য cv::vconcat(প্রবেশ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.getMatVector(src);
    vconcat(!src.empty() ? &src[0] : nullptr, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির যৌক্তিক ocl_setIdentity( প্রবেশ_Output_Array _m, ধ্রুবক Scalar& s )
{
    পূর্ণসংখ্যা type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    পূর্ণসংখ্যা sctype = CV_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
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
        ফেরত মিথ্যা;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.cols * cn / kercn, ((size_t)m.rows + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, nullptr, মিথ্যা);
}

}

#endif

শূন্য cv::setIdentity( প্রবেশ_Output_Array _m, ধ্রুবক Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_RUN(_m.isUMat(),
               ocl_setIdentity(_m, s))

    Mat m = _m.getMat();
    পূর্ণসংখ্যা rows = m.rows, cols = m.cols, type = m.type();

    যদি( type == CV_32FC1 )
    {
        দশমিক* তথ্য = m.ptr<দশমিক>();
        দশমিক val = (দশমিক)s[0];
        size_t step = m.step/আকার(তথ্য[0]);

        জন্য( পূর্ণসংখ্যা i = 0; i < rows; i++, তথ্য += step )
        {
            জন্য( পূর্ণসংখ্যা j = 0; j < cols; j++ )
                তথ্য[j] = 0;
            যদি( i < cols )
                তথ্য[i] = val;
        }
    }
    অন্যথা যদি( type == CV_64FC1 )
    {
        দ্বিগুণ* তথ্য = m.ptr<দ্বিগুণ>();
        দ্বিগুণ val = s[0];
        size_t step = m.step/আকার(তথ্য[0]);

        জন্য( পূর্ণসংখ্যা i = 0; i < rows; i++, তথ্য += step )
        {
            std::fill(তথ্য, তথ্য + cols, 0.0);
            যদি (i < cols)
                তথ্য[i] = val;
        }
    }
    অন্যথা
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


নামাকাশ cv {

UMat UMat::eye(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    ফেরত UMat::eye(আকার_2(cols, rows), type, usageFlags);
}

UMat UMat::eye(আকার_2 size, পূর্ণসংখ্যা type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    setIdentity(m);
    ফেরত m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( প্রবেশ_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    পূর্ণসংখ্যা type = m.type();
    পূর্ণসংখ্যা nm = std::min(m.rows, m.cols);

    যদি( type == CV_32FC1 )
    {
        ধ্রুবক দশমিক* ptr = m.ptr<দশমিক>();
        size_t step = m.step/আকার(ptr[0]) + 1;
        দ্বিগুণ _s = 0;
        জন্য( পূর্ণসংখ্যা i = 0; i < nm; i++ )
            _s += ptr[i*step];
        ফেরত _s;
    }

    যদি( type == CV_64FC1 )
    {
        ধ্রুবক দ্বিগুণ* ptr = m.ptr<দ্বিগুণ>();
        size_t step = m.step/আকার(ptr[0]) + 1;
        দ্বিগুণ _s = 0;
        জন্য( পূর্ণসংখ্যা i = 0; i < nm; i++ )
            _s += ptr[i*step];
        ফেরত _s;
    }

    ফেরত cv::যোগ_2(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

শূন্য cv::completeSymm( প্রবেশ_Output_Array _m, যৌক্তিক LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t step = m.step, esz = m.elem_আকার();
    CV_Assert( m.dims <= 2 && m.rows == m.cols );

    পূর্ণসংখ্যা rows = m.rows;
    পূর্ণসংখ্যা j0 = 0, j1 = rows;

    uchar* তথ্য = m.ptr();
    জন্য( পূর্ণসংখ্যা i = 0; i < rows; i++ )
    {
        যদি( !LtoR ) j1 = i; অন্যথা j0 = i+1;
        জন্য( পূর্ণসংখ্যা j = j0; j < j1; j++ )
            memcpy(তথ্য + (i*step + j*esz), তথ্য + (j*step + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(প্রবেশ_Array _m) ধ্রুবক
{
    Mat m = _m.getMat();
    পূর্ণসংখ্যা tp = type(), d = CV_MAT_DEPTH(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((rows == 3 && cols == 1) || (cols*channels() == 3 && rows == 1)));
    Mat ফলাফল(rows, cols, tp);

    যদি( d == CV_32F )
    {
        ধ্রুবক দশমিক *a = (ধ্রুবক দশমিক*)তথ্য, *b = (ধ্রুবক দশমিক*)m.তথ্য;
        দশমিক* c = (দশমিক*)ফলাফল.তথ্য;
        size_t lda = rows > 1 ? step/আকার(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/আকার(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    অন্যথা যদি( d == CV_64F )
    {
        ধ্রুবক দ্বিগুণ *a = (ধ্রুবক দ্বিগুণ*)তথ্য, *b = (ধ্রুবক দ্বিগুণ*)m.তথ্য;
        দ্বিগুণ* c = (দ্বিগুণ*)ফলাফল.তথ্য;
        size_t lda = rows > 1 ? step/আকার(a[0]) : 1;
        size_t ldb = rows > 1 ? m.step/আকার(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    ফেরত ফলাফল;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

নামাকাশ cv
{

প্রকারলেখ শূন্য (*Reduce_যোগ_Func)(ধ্রুবক Mat& src, Mat& dst);
Reduce_যোগ_Func getReduceCSumFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth);
Reduce_যোগ_Func getReduceRSumFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth);
Reduce_যোগ_Func getReduceRSum2Func(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth);

template <typename T, typename WT, typename Op>
কাঠামো ReduceR_SIMD
{
    পূর্ণসংখ্যা operator()(ধ্রুবক T*, পূর্ণসংখ্যা start, পূর্ণসংখ্যা, WT*, ধ্রুবক Op&) ধ্রুবক
    {
        ফেরত start;
    }
};


template<typename T, typename ST, typename WT, শ্রেণী Op, শ্রেণী OpInit>
শ্রেণী ReduceR_Invoker : সর্বজনীন ParallelLoopBody
{
সর্বজনীন:
  ReduceR_Invoker(ধ্রুবক Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),বাফার(srcmat.size().width*srcmat.channels())
  {
  }
  শূন্য operator()(ধ্রুবক Range& range) ধ্রুবক CV_OVERRIDE
  {
    ধ্রুবক T* src = srcmat.ptr<T>();
    ধ্রুবক size_t srcstep = srcmat.step/আকার(src[0]);
    WT* buf = বাফার.তথ্য();
    ST* dst = dstmat.ptr<ST>();
    পূর্ণসংখ্যা i = 0;

    জন্য( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    পূর্ণসংখ্যা height = srcmat.size().height;
    জন্য( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #যদি CV_ENABLE_UNROLLED
        জন্য(; i <= range.end - 4; i += 4 )
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
        জন্য( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    জন্য( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
ব্যক্তিগত:
  ধ্রুবক Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  mutable Auto_বাফার<WT> বাফার;
};

template<typename T, typename ST, শ্রেণী Op, শ্রেণী OpInit = OpNop<ST> > স্থির শূন্য
reduceR_( ধ্রুবক Mat& srcmat, Mat& dstmat)
{
    প্রকারলেখ typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().width*srcmat.channels()), body, srcmat.size().width*CV_ELEM_আকার(srcmat.depth())/64);
}

template<typename T, typename ST, typename WT, শ্রেণী Op, শ্রেণী OpInit>
শ্রেণী ReduceC_Invoker : সর্বজনীন ParallelLoopBody
{
সর্বজনীন:
  ReduceC_Invoker(ধ্রুবক Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  শূন্য operator()(ধ্রুবক Range& range) ধ্রুবক CV_OVERRIDE
  {
    ধ্রুবক পূর্ণসংখ্যা cn = srcmat.channels();
    ধ্রুবক পূর্ণসংখ্যা width = srcmat.size().width*cn;
    Auto_বাফার<WT> cumul(cn);
    জন্য( পূর্ণসংখ্যা y = range.start; y < range.end; y++ )
    {
        ধ্রুবক T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        যদি( width == cn )
        {
          জন্য( পূর্ণসংখ্যা k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        অন্যথা
        {
            জন্য(পূর্ণসংখ্যা k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            জন্য(পূর্ণসংখ্যা k = cn ; k < width ; k += cn )
            {
                জন্য (পূর্ণসংখ্যা c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            জন্য(পূর্ণসংখ্যা k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
ব্যক্তিগত:
  ধ্রুবক Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

template<typename T, typename ST, শ্রেণী Op, শ্রেণী OpInit = OpNop<ST> > স্থির শূন্য
reduceC_( ধ্রুবক Mat& srcmat, Mat& dstmat)
{
    প্রকারলেখ typename Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

প্রকারলেখ শূন্য (*ReduceFunc)( ধ্রুবক Mat& src, Mat& dst );

}

#define reduce_যোগ_R8u32s  reduceR_<uchar, পূর্ণসংখ্যা,   OpAdd<পূর্ণসংখ্যা>, OpNop<পূর্ণসংখ্যা> >
#define reduce_যোগ_R8u32f  reduceR_<uchar, দশমিক, OpAdd<পূর্ণসংখ্যা>, OpNop<পূর্ণসংখ্যা> >
#define reduce_যোগ_R8u64f  reduceR_<uchar, দ্বিগুণ,OpAdd<পূর্ণসংখ্যা>, OpNop<পূর্ণসংখ্যা> >
#define reduce_যোগ_R16u32f reduceR_<ushort,দশমিক, OpAdd<দশমিক> >
#define reduce_যোগ_R16u64f reduceR_<ushort,দ্বিগুণ,OpAdd<দ্বিগুণ> >
#define reduce_যোগ_R16s32f reduceR_<short, দশমিক, OpAdd<দশমিক> >
#define reduce_যোগ_R16s64f reduceR_<short, দ্বিগুণ,OpAdd<দ্বিগুণ> >
#define reduce_যোগ_R32f32f reduceR_<দশমিক, দশমিক, OpAdd<দশমিক> >
#define reduce_যোগ_R32f64f reduceR_<দশমিক, দ্বিগুণ,OpAdd<দ্বিগুণ> >
#define reduce_যোগ_R64f64f reduceR_<দ্বিগুণ,দ্বিগুণ,OpAdd<দ্বিগুণ> >

#define reduceSum2R8u32s  reduceR_<uchar, পূর্ণসংখ্যা,   OpAddSqr<পূর্ণসংখ্যা>,   OpSqr<পূর্ণসংখ্যা> >
#define reduceSum2R8u32f  reduceR_<uchar, দশমিক, OpAddSqr<পূর্ণসংখ্যা>,   OpSqr<পূর্ণসংখ্যা> >
#define reduceSum2R8u64f  reduceR_<uchar, দ্বিগুণ,OpAddSqr<পূর্ণসংখ্যা>,   OpSqr<পূর্ণসংখ্যা> >
#define reduceSum2R16u32f reduceR_<ushort,দশমিক, OpAddSqr<দশমিক>, OpSqr<দশমিক> >
#define reduceSum2R16u64f reduceR_<ushort,দ্বিগুণ,OpAddSqr<দ্বিগুণ>,OpSqr<দ্বিগুণ> >
#define reduceSum2R16s32f reduceR_<short, দশমিক, OpAddSqr<দশমিক>, OpSqr<দশমিক> >
#define reduceSum2R16s64f reduceR_<short, দ্বিগুণ,OpAddSqr<দ্বিগুণ>,OpSqr<দ্বিগুণ> >
#define reduceSum2R32f32f reduceR_<দশমিক, দশমিক, OpAddSqr<দশমিক>, OpSqr<দশমিক> >
#define reduceSum2R32f64f reduceR_<দশমিক, দ্বিগুণ,OpAddSqr<দ্বিগুণ>,OpSqr<দ্বিগুণ> >
#define reduceSum2R64f64f reduceR_<দ্বিগুণ,দ্বিগুণ,OpAddSqr<দ্বিগুণ>,OpSqr<দ্বিগুণ> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<দশমিক, দশমিক, OpMax<দশমিক> >
#define reduceMaxR64f reduceR_<দ্বিগুণ,দ্বিগুণ,OpMax<দ্বিগুণ> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<দশমিক, দশমিক, OpMin<দশমিক> >
#define reduceMinR64f reduceR_<দ্বিগুণ,দ্বিগুণ,OpMin<দ্বিগুণ> >

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
স্থির অন্তঃরেখ যৌক্তিক ipp_reduce_যোগ_C_8u16u16s32f_64f(ধ্রুবক cv::Mat& srcmat, cv::Mat& dstmat)
{
    পূর্ণসংখ্যা sstep = (পূর্ণসংখ্যা)srcmat.step, stype = srcmat.type(),
            ddepth = dstmat.depth();

    Ippi_আকার roisize = { srcmat.size().width, 1 };

    প্রকারলেখ IppStatus (CV_STDCALL * Ippi_যোগ)(ধ্রুবক শূন্য * pSrc, পূর্ণসংখ্যা srcStep, Ippi_আকার roi_আকার, Ipp64f* p_যোগ);
    প্রকারলেখ IppStatus (CV_STDCALL * Ippi_যোগ_Hint)(ধ্রুবক শূন্য * pSrc, পূর্ণসংখ্যা srcStep, Ippi_আকার roi_আকার, Ipp64f* p_যোগ, IppHintAlgorithm hint);
    Ippi_যোগ ippi_যোগ = 0;
    Ippi_যোগ_Hint ippi_যোগ_Hint = 0;

    যদি(ddepth == CV_64F)
    {
        ippi_যোগ =
            stype == CV_8UC1 ? (Ippi_যোগ)ippi_যোগ_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_যোগ)ippi_যোগ_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_যোগ)ippi_যোগ_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_যোগ)ippi_যোগ_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_যোগ)ippi_যোগ_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_যোগ)ippi_যোগ_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_যোগ)ippi_যোগ_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_যোগ)ippi_যোগ_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_যোগ)ippi_যোগ_16s_C4_R : 0;
        ippi_যোগ_Hint =
            stype == CV_32FC1 ? (Ippi_যোগ_Hint)ippi_যোগ_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_যোগ_Hint)ippi_যোগ_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_যোগ_Hint)ippi_যোগ_32f_C4_R : 0;
    }

    যদি(ippi_যোগ)
    {
        জন্য(পূর্ণসংখ্যা y = 0; y < srcmat.size().height; y++)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(ippi_যোগ, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                ফেরত মিথ্যা;
        }
        ফেরত সত্য;
    }
    অন্যথা যদি(ippi_যোগ_Hint)
    {
        জন্য(পূর্ণসংখ্যা y = 0; y < srcmat.size().height; y++)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(ippi_যোগ_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                ফেরত মিথ্যা;
        }
        ফেরত সত্য;
    }

    ফেরত মিথ্যা;
}

স্থির অন্তঃরেখ শূন্য reduce_যোগ_C_8u16u16s32f_64f(ধ্রুবক cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_RUN_FAST(ipp_reduce_যোগ_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    যদি(dstmat.depth() == CV_64F)
    {
        পূর্ণসংখ্যা sdepth = CV_MAT_DEPTH(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, দ্বিগুণ,   cv::OpAdd<দ্বিগুণ> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, দ্বিগুণ,   cv::OpAdd<দ্বিগুণ> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<short, দ্বিগুণ,   cv::OpAdd<দ্বিগুণ> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<দশমিক, দ্বিগুণ,   cv::OpAdd<দ্বিগুণ> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduce_যোগ_C8u32s  reduceC_<uchar, পূর্ণসংখ্যা,   OpAdd<পূর্ণসংখ্যা>, OpNop<পূর্ণসংখ্যা> >
#define reduce_যোগ_C8u32f  reduceC_<uchar, দশমিক, OpAdd<পূর্ণসংখ্যা>, OpNop<পূর্ণসংখ্যা> >
#define reduce_যোগ_C16u32f reduceC_<ushort,দশমিক, OpAdd<দশমিক> >
#define reduce_যোগ_C16s32f reduceC_<short, দশমিক, OpAdd<দশমিক> >
#define reduce_যোগ_C32f32f reduceC_<দশমিক, দশমিক, OpAdd<দশমিক> >
#define reduce_যোগ_C64f64f reduceC_<দ্বিগুণ,দ্বিগুণ,OpAdd<দ্বিগুণ> >

#define reduceSum2C8u32s  reduceC_<uchar, পূর্ণসংখ্যা,   OpAddSqr<পূর্ণসংখ্যা>,   OpSqr<পূর্ণসংখ্যা> >
#define reduceSum2C8u32f  reduceC_<uchar, দশমিক, OpAddSqr<পূর্ণসংখ্যা>,   OpSqr<পূর্ণসংখ্যা> >
#define reduceSum2C16u32f reduceC_<ushort,দশমিক, OpAddSqr<দশমিক>, OpSqr<দশমিক> >
#define reduceSum2C16s32f reduceC_<short, দশমিক, OpAddSqr<দশমিক>, OpSqr<দশমিক> >
#define reduceSum2C32f32f reduceC_<দশমিক, দশমিক, OpAddSqr<দশমিক>, OpSqr<দশমিক> >
#define reduceSum2C64f64f reduceC_<দ্বিগুণ,দ্বিগুণ,OpAddSqr<দ্বিগুণ>,OpSqr<দ্বিগুণ> >

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define reduce_যোগ_C8u64f  reduce_যোগ_C_8u16u16s32f_64f
#define reduce_যোগ_C16u64f reduce_যোগ_C_8u16u16s32f_64f
#define reduce_যোগ_C16s64f reduce_যোগ_C_8u16u16s32f_64f
#define reduce_যোগ_C32f64f reduce_যোগ_C_8u16u16s32f_64f
#অন্যথা
#define reduce_যোগ_C8u64f  reduceC_<uchar, দ্বিগুণ,OpAdd<পূর্ণসংখ্যা>, OpNop<পূর্ণসংখ্যা> >
#define reduce_যোগ_C16u64f reduceC_<ushort,দ্বিগুণ,OpAdd<দ্বিগুণ> >
#define reduce_যোগ_C16s64f reduceC_<short, দ্বিগুণ,OpAdd<দ্বিগুণ> >
#define reduce_যোগ_C32f64f reduceC_<দশমিক, দ্বিগুণ,OpAdd<দ্বিগুণ> >
#endif

#define reduceSum2C8u64f  reduceC_<uchar, দ্বিগুণ,OpAddSqr<পূর্ণসংখ্যা>,   OpSqr<পূর্ণসংখ্যা> >
#define reduceSum2C16u64f reduceC_<ushort,দ্বিগুণ,OpAddSqr<দ্বিগুণ>,OpSqr<দ্বিগুণ> >
#define reduceSum2C16s64f reduceC_<short, দ্বিগুণ,OpAddSqr<দ্বিগুণ>,OpSqr<দ্বিগুণ> >
#define reduceSum2C32f64f reduceC_<দশমিক, দ্বিগুণ,OpAddSqr<দ্বিগুণ>,OpSqr<দ্বিগুণ> >

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#define REDUCE_OP(favor, optype, type1, type2) \
স্থির অন্তঃরেখ যৌক্তিক ipp_reduce##optype##C##favor(ধ্রুবক cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    যদি((srcmat.channels() == 1)) \
    { \
        পূর্ণসংখ্যা sstep = (পূর্ণসংখ্যা)srcmat.step; \
        প্রকারলেখ Ipp##favor IppType; \
        Ippi_আকার roisize = ippi_আকার(srcmat.size().width, 1);\
        জন্য(পূর্ণসংখ্যা y = 0; y < srcmat.size().height; y++)\
        {\
            যদি(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                ফেরত মিথ্যা;\
        }\
        ফেরত সত্য;\
    }\
    ফেরত মিথ্যা; \
} \
স্থির অন্তঃরেখ শূন্য reduce##optype##C##favor(ধ্রুবক cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, দশমিক, দশমিক)
#অন্যথা
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<দশমিক, দশমিক, OpMax<দশমিক> >
#endif
#define reduceMaxC64f reduceC_<দ্বিগুণ,দ্বিগুণ,OpMax<দ্বিগুণ> >

#যদি defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, দশমিক, দশমিক)
#অন্যথা
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<দশমিক, দশমিক, OpMin<দশমিক> >
#endif
#define reduceMinC64f reduceC_<দ্বিগুণ,দ্বিগুণ,OpMin<দ্বিগুণ> >

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির যৌক্তিক ocl_reduce(প্রবেশ_Array _src, OutputArray _dst,
                       পূর্ণসংখ্যা dim, পূর্ণসংখ্যা op, পূর্ণসংখ্যা op0, পূর্ণসংখ্যা stype, পূর্ণসংখ্যা dtype)
{
    ধ্রুবক পূর্ণসংখ্যা min_opt_cols = 128, buf_cols = 32;
    পূর্ণসংখ্যা sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_DEPTH(dtype), ddepth0 = ddepth;
    ধ্রুবক ocl::Device &defDev = ocl::Device::getDefault();
    যৌক্তিক doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_Group_আকার();
    যৌক্তিক useOptimized = 1 == dim && _src.cols() > min_opt_cols && (wgs >= buf_cols);

    যদি (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        ফেরত মিথ্যা;

    যদি (op == REDUCE_AVG)
    {
        যদি (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ধ্রুবক অক্ষর * ধ্রুবক ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    পূর্ণসংখ্যা wdepth = std::max(ddepth, CV_32F);
    যদি (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        যদি (defDev.isIntel())
        {
            স্থির ধ্রুবক size_t max_Item_In_Group_গুনতি = 16;
            tileHeight = min(tileHeight, defDev.local_Mem_আকার() / buf_cols / CV_ELEM_আকার(CV_MAKETYPE(wdepth, cn)) / max_Item_In_Group_গুনতি);
        }
        অক্ষর cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], আকার(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], আকার(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], আকার(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        যদি (k.empty())
            ফেরত মিথ্যা;
        UMat src = _src.getUMat();
        আকার_2 dsize(1, src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        যদি (op0 == REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_আকার(dst), 1.0f / src.cols);
        অন্যথা
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_আকার(dst));

        size_t local_আকার[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t global_আকার[2] = { (size_t)buf_cols, (size_t)src.rows };
        ফেরত k.run(2, global_আকার, local_আকার, মিথ্যা);
    }
    অন্যথা
    {
        অক্ষর cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(useOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], আকার(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], আকার(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], আকার(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        যদি (k.empty())
            ফেরত মিথ্যা;

        UMat src = _src.getUMat();
        আকার_2 dsize(dim == 0 ? src.cols : 1, dim == 0 ? 1 : src.rows);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_আকার(dst);

        যদি (op0 == REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src.rows : src.cols));
        অন্যথা
            k.args(srcarg, temparg);

        size_t globalsize = std::max(dsize.width, dsize.height);
        ফেরত k.run(1, &globalsize, ফাঁকা, মিথ্যা);
    }
}

}

#endif

শূন্য cv::reduce(প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা dim, পূর্ণসংখ্যা op, পূর্ণসংখ্যা dtype)
{
    CV_INSTRUMENT_REGION();

    যদি (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, সত্য, dtype < 0 && _dst.fixedType() ? _dst.depth() : dtype));
        ফেরত;
    }
    পূর্ণসংখ্যা op0 = op;
    পূর্ণসংখ্যা stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), cn = CV_MAT_CN(stype);
    যদি( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    পূর্ণসংখ্যা ddepth = CV_MAT_DEPTH(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_যোগ || op == REDUCE_MAX ||
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
        ফেরত;
    }
    যদি (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.rows, dim == 0 ? src.cols : 1, dtype);
    Mat dst = _dst.getMat(), অস্থায়ী_2 = dst;

    যদি( op == REDUCE_AVG )
    {
        op = REDUCE_যোগ;
        যদি( sdepth < CV_32S && ddepth < CV_32S )
        {
            অস্থায়ী_2.create(dst.rows, dst.cols, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    যদি( dim == 0 )
    {
        যদি( op == REDUCE_যোগ )
        {
            Reduce_যোগ_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            যদি(simd_func)
                func = (ReduceFunc)simd_func;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_যোগ_R8u32s;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_যোগ_R8u32f;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_যোগ_R8u64f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_যোগ_R16u32f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_যোগ_R16u64f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_যোগ_R16s32f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_যোগ_R16s64f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_যোগ_R32f32f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_যোগ_R32f64f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_যোগ_R64f64f;
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
            Reduce_যোগ_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
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
        যদি(op == REDUCE_যোগ)
        {
            Reduce_যোগ_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            যদি(simd_func)
                func = (ReduceFunc)simd_func;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_যোগ_C8u32s;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_যোগ_C8u32f;
            অন্যথা যদি(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_যোগ_C8u64f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_যোগ_C16u32f;
            অন্যথা যদি(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_যোগ_C16u64f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_যোগ_C16s32f;
            অন্যথা যদি(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_যোগ_C16s64f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_যোগ_C32f32f;
            অন্যথা যদি(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_যোগ_C32f64f;
            অন্যথা যদি(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_যোগ_C64f64f;
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

    func( src, অস্থায়ী_2 );

    যদি( op0 == REDUCE_AVG )
        অস্থায়ী_2.convertTo(dst, dst.type(), 1./(dim == 0 ? src.rows : src.cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

নামাকাশ cv
{

template<typename T> স্থির শূন্য sort_( ধ্রুবক Mat& src, Mat& dst, পূর্ণসংখ্যা flags )
{
    পূর্ণসংখ্যা n, len;
    যৌক্তিক sortRows = (flags & 1) == SORT_EVERY_ROW;
    যৌক্তিক inplace = src.তথ্য == dst.তথ্য;
    যৌক্তিক sortDescending = (flags & SORT_DESCENDING) != 0;

    যদি( sortRows )
        n = src.rows, len = src.cols;
    অন্যথা
        n = src.cols, len = src.rows;
    parallel_for_(Range(0, n), [&](ধ্রুবক Range& range)
    {
        Auto_বাফার<T> buf;
        যদি( !sortRows )
            buf.allocate(len);
        T* bptr = buf.তথ্য();

        জন্য( পূর্ণসংখ্যা i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            যদি( sortRows )
            {
                T* dptr = dst.ptr<T>(i);
                যদি( !inplace )
                {
                    ধ্রুবক T* sptr = src.ptr<T>(i);
                    memcpy(dptr, sptr, আকার(T) * len);
                }
                ptr = dptr;
            }
            অন্যথা
            {
                জন্য( পূর্ণসংখ্যা j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            যদি( sortDescending )
            {
                জন্য( পূর্ণসংখ্যা j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            যদি( !sortRows )
                জন্য( পূর্ণসংখ্যা j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#যদি defined(HAVE_IPP) && !IPP_DISABLE_SORT
প্রকারলেখ IppStatus (CV_STDCALL *IppSortFunc)(শূন্য  *pSrcDst, পূর্ণসংখ্যা    len, Ipp8u *p_বাফার);

template<typename T, IppStatus (CV_STDCALL *fn)(T*, পূর্ণসংখ্যা, Ipp8u*)>
স্থির IppStatus CV_STDCALL ippSortWrap(শূন্য* pSrcDst, পূর্ণসংখ্যা len, Ipp8u* p_বাফার)
{
    ফেরত fn((T*)pSrcDst, len, p_বাফার);
}

স্থির IppSortFunc getSortFunc(পূর্ণসংখ্যা depth, যৌক্তিক sortDescending)
{
    যদি (!sortDescending)
        ফেরত depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixAscend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixAscend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixAscend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixAscend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixAscend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixAscend_64f_I> :
            0;
    অন্যথা
        ফেরত depth == CV_8U ? ippSortWrap<Ipp8u, ippsSortRadixDescend_8u_I> :
            depth == CV_16U ? ippSortWrap<Ipp16u, ippsSortRadixDescend_16u_I> :
            depth == CV_16S ? ippSortWrap<Ipp16s, ippsSortRadixDescend_16s_I> :
            depth == CV_32S ? ippSortWrap<Ipp32s, ippsSortRadixDescend_32s_I> :
            depth == CV_32F ? ippSortWrap<Ipp32f, ippsSortRadixDescend_32f_I> :
            depth == CV_64F ? ippSortWrap<Ipp64f, ippsSortRadixDescend_64f_I> :
            0;
}

স্থির যৌক্তিক ipp_sort(ধ্রুবক Mat& src, Mat& dst, পূর্ণসংখ্যা flags)
{
    CV_INSTRUMENT_REGION_IPP();

    যৌক্তিক        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    যৌক্তিক        sortDescending  = (flags & SORT_DESCENDING) != 0;
    যৌক্তিক        inplace         = (src.তথ্য == dst.তথ্য);
    পূর্ণসংখ্যা         depth           = src.depth();
    Ipp_তথ্য_Type type            = ippi_Get_তথ্য_Type(depth);

    IppSortFunc ippsSortRadix_I = getSortFunc(depth, sortDescending);
    যদি(!ippsSortRadix_I)
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা n, len;
    যদি( sortRows )
        n = src.rows, len = src.cols;
    অন্যথা
        n = src.cols, len = src.rows;

    পূর্ণসংখ্যা বাফার_আকার;
    যদি(ipps_Sort_Radix_Get_বাফার_আকার(len, type, &বাফার_আকার) < 0)
        ফেরত মিথ্যা;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    অস্থির যৌক্তিক ok = সত্য;
    parallel_for_(Range(0, n), [&](ধ্রুবক Range& range)
    {
        Auto_বাফার<Ipp8u> বাফার(বাফার_আকার);
        Mat  row;
        Rect subRect(0,0,1,src.rows);
        যদি( !sortRows )
            row.create(1, src.rows, src.type());

        জন্য( পূর্ণসংখ্যা i = range.start; i < range.end; i++ )
        {
            যদি( !ok )
                বিরতি;
            শূন্য* ptr;
            যদি( sortRows )
            {
                uchar* dptr = dst.ptr(i);
                যদি( !inplace )
                    memcpy(dptr, src.ptr(i), src.cols * src.elem_আকার());
                ptr = dptr;
            }
            অন্যথা
            {
                subRect.x = i;
                Mat(src, subRect).copyTo(row);
                ptr = row.ptr();
            }

            যদি( CV_INSTRUMENT_FUN_IPP(ippsSortRadix_I, ptr, len, বাফার.তথ্য()) < 0 )
            {
                ok = মিথ্যা;
                বিরতি;
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

    ফেরত ok;
}
#endif

template<typename _Tp> শ্রেণী LessThanIdx
{
সর্বজনীন:
    LessThanIdx( ধ্রুবক _Tp* _arr ) : arr(_arr) {}
    যৌক্তিক operator()(পূর্ণসংখ্যা a, পূর্ণসংখ্যা b) ধ্রুবক { ফেরত arr[a] < arr[b]; }
    ধ্রুবক _Tp* arr;
};

template<typename T> স্থির শূন্য sortIdx_( ধ্রুবক Mat& src, Mat& dst, পূর্ণসংখ্যা flags )
{
    Auto_বাফার<T> buf;
    Auto_বাফার<পূর্ণসংখ্যা> ibuf;
    যৌক্তিক sortRows = (flags & 1) == SORT_EVERY_ROW;
    যৌক্তিক sortDescending = (flags & SORT_DESCENDING) != 0;

    CV_Assert( src.তথ্য != dst.তথ্য );

    পূর্ণসংখ্যা n, len;
    যদি( sortRows )
        n = src.rows, len = src.cols;
    অন্যথা
    {
        n = src.cols, len = src.rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.তথ্য();
    পূর্ণসংখ্যা* _iptr = ibuf.তথ্য();

    জন্য( পূর্ণসংখ্যা i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        পূর্ণসংখ্যা* iptr = _iptr;

        যদি( sortRows )
        {
            ptr = (T*)(src.তথ্য + src.step*i);
            iptr = dst.ptr<পূর্ণসংখ্যা>(i);
        }
        অন্যথা
        {
            জন্য( পূর্ণসংখ্যা j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        জন্য( পূর্ণসংখ্যা j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        যদি( sortDescending )
        {
            জন্য( পূর্ণসংখ্যা j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        যদি( !sortRows )
            জন্য( পূর্ণসংখ্যা j = 0; j < len; j++ )
                dst.ptr<পূর্ণসংখ্যা>(j)[i] = iptr[j];
    }
}

#যদি defined(HAVE_IPP) && !IPP_DISABLE_SORT
প্রকারলেখ IppStatus (CV_STDCALL *Ipp_Sort_সূচক_Func)(ধ্রুবক শূন্য*  pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, পূর্ণসংখ্যা len, Ipp8u *p_বাফার);

template<typename T, IppStatus (CV_STDCALL *fn)(ধ্রুবক T*, Ipp32s, Ipp32s*, পূর্ণসংখ্যা, Ipp8u*)>
স্থির IppStatus CV_STDCALL ipp_Sort_সূচক_Wrap(ধ্রুবক শূন্য* pSrc, Ipp32s srcStrideBytes, Ipp32s *pDstIndx, পূর্ণসংখ্যা len, Ipp8u *p_বাফার)
{
    ফেরত fn((ধ্রুবক T*)pSrc, srcStrideBytes, pDstIndx, len, p_বাফার);
}

স্থির Ipp_Sort_সূচক_Func get_Sort_সূচক_Func(পূর্ণসংখ্যা depth, যৌক্তিক sortDescending)
{
    যদি (!sortDescending)
        ফেরত depth == CV_8U ? ipp_Sort_সূচক_Wrap<Ipp8u, ipps_Sort_Radix_সূচক_Ascend_8u> :
            depth == CV_16U ? ipp_Sort_সূচক_Wrap<Ipp16u, ipps_Sort_Radix_সূচক_Ascend_16u> :
            depth == CV_16S ? ipp_Sort_সূচক_Wrap<Ipp16s, ipps_Sort_Radix_সূচক_Ascend_16s> :
            depth == CV_32S ? ipp_Sort_সূচক_Wrap<Ipp32s, ipps_Sort_Radix_সূচক_Ascend_32s> :
            depth == CV_32F ? ipp_Sort_সূচক_Wrap<Ipp32f, ipps_Sort_Radix_সূচক_Ascend_32f> :
            0;
    অন্যথা
        ফেরত depth == CV_8U ? ipp_Sort_সূচক_Wrap<Ipp8u, ipps_Sort_Radix_সূচক_Descend_8u> :
            depth == CV_16U ? ipp_Sort_সূচক_Wrap<Ipp16u, ipps_Sort_Radix_সূচক_Descend_16u> :
            depth == CV_16S ? ipp_Sort_সূচক_Wrap<Ipp16s, ipps_Sort_Radix_সূচক_Descend_16s> :
            depth == CV_32S ? ipp_Sort_সূচক_Wrap<Ipp32s, ipps_Sort_Radix_সূচক_Descend_32s> :
            depth == CV_32F ? ipp_Sort_সূচক_Wrap<Ipp32f, ipps_Sort_Radix_সূচক_Descend_32f> :
            0;
}

স্থির যৌক্তিক ipp_sortIdx( ধ্রুবক Mat& src, Mat& dst, পূর্ণসংখ্যা flags )
{
    CV_INSTRUMENT_REGION_IPP();

    যৌক্তিক        sortRows        = (flags & 1) == SORT_EVERY_ROW;
    যৌক্তিক        sortDescending  = (flags & SORT_DESCENDING) != 0;
    পূর্ণসংখ্যা         depth           = src.depth();
    Ipp_তথ্য_Type type            = ippi_Get_তথ্য_Type(depth);

    Ipp_Sort_সূচক_Func ipps_Sort_Radix_সূচক = get_Sort_সূচক_Func(depth, sortDescending);
    যদি(!ipps_Sort_Radix_সূচক)
        ফেরত মিথ্যা;

    যদি(sortRows)
    {
        Auto_বাফার<Ipp8u> বাফার;
        পূর্ণসংখ্যা               বাফার_আকার;
        যদি(ipps_Sort_Radix_সূচক_Get_বাফার_আকার(src.cols, type, &বাফার_আকার) < 0)
            ফেরত মিথ্যা;

        বাফার.allocate(বাফার_আকার);

        জন্য(পূর্ণসংখ্যা i = 0; i < src.rows; i++)
        {
            যদি(CV_INSTRUMENT_FUN_IPP(ipps_Sort_Radix_সূচক, (ধ্রুবক শূন্য*)src.ptr(i), (Ipp32s)src.step[1], (Ipp32s*)dst.ptr(i), src.cols, বাফার.তথ্য()) < 0)
                ফেরত মিথ্যা;
        }
    }
    অন্যথা
    {
        Mat  dstRow(1, dst.rows, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.rows);

        Auto_বাফার<Ipp8u> বাফার;
        পূর্ণসংখ্যা               বাফার_আকার;
        যদি(ipps_Sort_Radix_সূচক_Get_বাফার_আকার(src.rows, type, &বাফার_আকার) < 0)
            ফেরত মিথ্যা;

        বাফার.allocate(বাফার_আকার);

        Ipp32s srcStep = (Ipp32s)src.step[0];
        জন্য(পূর্ণসংখ্যা i = 0; i < src.cols; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            যদি(CV_INSTRUMENT_FUN_IPP(ipps_Sort_Radix_সূচক, (ধ্রুবক শূন্য*)src.ptr(0, i), srcStep, (Ipp32s*)dstRow.ptr(), src.rows, বাফার.তথ্য()) < 0)
                ফেরত মিথ্যা;

            dstRow = dstRow.reshape(1, dstSub.rows);
            dstRow.copyTo(dstSub);
        }
    }

    ফেরত সত্য;
}
#endif

প্রকারলেখ শূন্য (*SortFunc)(ধ্রুবক Mat& src, Mat& dst, পূর্ণসংখ্যা flags);
}

শূন্য cv::sort( প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_আকার( src, src.type() );
    Mat dst = _dst.getMat();
#যদি !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sort(src, dst, flags));
#endif

    স্থির SortFunc tab[CV_DEPTH_MAX] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<পূর্ণসংখ্যা>, sort_<দশমিক>, sort_<দ্বিগুণ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

শূন্য cv::sortIdx( প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    যদি( dst.তথ্য == src.তথ্য )
        _dst.release();
    _dst.create_Same_আকার( src, CV_32S );
    dst = _dst.getMat();

#যদি !IPP_DISABLE_SORT
    CV_IPP_RUN_FAST(ipp_sortIdx(src, dst, flags));
#endif

    স্থির SortFunc tab[CV_DEPTH_MAX] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<পূর্ণসংখ্যা>, sortIdx_<দশমিক>, sortIdx_<দ্বিগুণ>, 0
    };
    SortFunc func = tab[src.depth()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
