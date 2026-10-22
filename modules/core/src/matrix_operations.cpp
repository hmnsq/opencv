// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"
#ضمن "opencl_kernels_core.hpp"

#تعریف_کړه IPP_DISABLE_REDUCE 1
#تعریف_کړه IPP_DISABLE_ترتیبول   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

فراغ cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.لیکې, b.لیکې);
    std::swap(a.ستون_شمېر, b.ستون_شمېر);
    std::swap(a.معلومات, b.معلومات);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.ګام, b.ګام);
}


فراغ cv::hconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح ټول_ستون_شمېر = 0, ستون_شمېر = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].لیکې == src[0].لیکې &&
                   src[i].type() == src[0].type());
        ټول_ستون_شمېر += src[i].ستون_شمېر;
    }
    _dst.create( src[0].لیکې, ټول_ستون_شمېر, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(ستون_شمېر, 0, src[i].ستون_شمېر, src[i].لیکې));
        src[i].copy_منزل(dpart);
        ستون_شمېر += src[i].ستون_شمېر;
    }
}

فراغ cv::hconcat(ننوت_Array src1, ننوت_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

فراغ cv::hconcat(ننوت_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ویکټور(src);
    hconcat(!src.empty() ? &src[0] : تش_اشاره, src.size(), dst);
}

فراغ cv::vconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح ټول_لیکې = 0, لیکې = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].ستون_شمېر == src[0].ستون_شمېر &&
                  src[i].type() == src[0].type());
        ټول_لیکې += src[i].لیکې;
    }
    _dst.create( ټول_لیکې, src[0].ستون_شمېر, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, لیکې, src[i].ستون_شمېر, src[i].لیکې));
        src[i].copy_منزل(dpart);
        لیکې += src[i].لیکې;
    }
}

فراغ cv::vconcat(ننوت_Array src1, ننوت_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

فراغ cv::vconcat(ننوت_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ویکټور(src);
    vconcat(!src.empty() ? &src[0] : تش_اشاره, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#که_تعریف_شوی HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_سېټ_بې_اغېزه_عنصر( ننوت_Output_Array _m, ثابت Scalar& s )
{
    صحيح type = _m.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type), kercn = cn, لیکې_فی_WI = 1;
    صحيح sctype = CV_MAKE_TYPE(ژوروالی, cn == 3 ? 4 : cn);
    إذا (ocl::Device::getDefault().isIntel())
    {
        لیکې_فی_WI = 4;
        إذا (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_ویکټور_پلنوالی(_m), 4);
            إذا (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::سېټ_بې_اغېزه_عنصر_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_منزل_Str(CV_MAKE_TYPE(ژوروالی, kercn)),
                         ocl::memop_Type_منزل_Str(ژوروالی), cn,
                         ocl::memop_Type_منزل_Str(sctype),
                         kercn, لیکې_فی_WI));
    إذا (k.empty())
        أرجع خطأ;

    UMat m = _m.getUMat();
    k.دلیلونه(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.ستون_شمېر * cn / kercn, ((size_t)m.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, تش_اشاره, خطأ);
}

}

#د_که_پای

فراغ cv::سېټ_بې_اغېزه_عنصر( ننوت_Output_Array _m, ثابت Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_پرلپسې(_m.isUMat(),
               ocl_سېټ_بې_اغېزه_عنصر(_m, s))

    Mat m = _m.getMat();
    صحيح لیکې = m.لیکې, ستون_شمېر = m.ستون_شمېر, type = m.type();

    إذا( type == CV_32FC1 )
    {
        عائم* معلومات = m.ptr<عائم>();
        عائم val = (عائم)s[0];
        size_t ګام = m.ګام/حجم(معلومات[0]);

        لكل( صحيح i = 0; i < لیکې; i++, معلومات += ګام )
        {
            لكل( صحيح j = 0; j < ستون_شمېر; j++ )
                معلومات[j] = 0;
            إذا( i < ستون_شمېر )
                معلومات[i] = val;
        }
    }
    وإلا إذا( type == CV_64FC1 )
    {
        مزدوج* معلومات = m.ptr<مزدوج>();
        مزدوج val = s[0];
        size_t ګام = m.ګام/حجم(معلومات[0]);

        لكل( صحيح i = 0; i < لیکې; i++, معلومات += ګام )
        {
            std::fill(معلومات, معلومات + ستون_شمېر, 0.0);
            إذا (i < ستون_شمېر)
                معلومات[i] = val;
        }
    }
    وإلا
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


نطاق cv {

UMat UMat::eye(صحيح لیکې, صحيح ستون_شمېر, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat::eye(اندازه(ستون_شمېر, لیکې), type, usageFlags);
}

UMat UMat::eye(اندازه size, صحيح type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    سېټ_بې_اغېزه_عنصر(m);
    أرجع m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( ننوت_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    صحيح type = m.type();
    صحيح nm = std::min(m.لیکې, m.ستون_شمېر);

    إذا( type == CV_32FC1 )
    {
        ثابت عائم* ptr = m.ptr<عائم>();
        size_t ګام = m.ګام/حجم(ptr[0]) + 1;
        مزدوج _s = 0;
        لكل( صحيح i = 0; i < nm; i++ )
            _s += ptr[i*ګام];
        أرجع _s;
    }

    إذا( type == CV_64FC1 )
    {
        ثابت مزدوج* ptr = m.ptr<مزدوج>();
        size_t ګام = m.ګام/حجم(ptr[0]) + 1;
        مزدوج _s = 0;
        لكل( صحيح i = 0; i < nm; i++ )
            _s += ptr[i*ګام];
        أرجع _s;
    }

    أرجع cv::مجموعه(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

فراغ cv::completeSymm( ننوت_Output_Array _m, منطقي LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t ګام = m.ګام, esz = m.elem_اندازه();
    CV_Assert( m.dims <= 2 && m.لیکې == m.ستون_شمېر );

    صحيح لیکې = m.لیکې;
    صحيح j0 = 0, j1 = لیکې;

    uchar* معلومات = m.ptr();
    لكل( صحيح i = 0; i < لیکې; i++ )
    {
        إذا( !LtoR ) j1 = i; وإلا j0 = i+1;
        لكل( صحيح j = j0; j < j1; j++ )
            حافظه_نقل(معلومات + (i*ګام + j*esz), معلومات + (j*ګام + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(ننوت_Array _m) ثابت
{
    Mat m = _m.getMat();
    صحيح tp = type(), d = CV_MAT_ژوروالی(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((لیکې == 3 && ستون_شمېر == 1) || (ستون_شمېر*channels() == 3 && لیکې == 1)));
    Mat پایله(لیکې, ستون_شمېر, tp);

    إذا( d == CV_32F )
    {
        ثابت عائم *a = (ثابت عائم*)معلومات, *b = (ثابت عائم*)m.معلومات;
        عائم* c = (عائم*)پایله.معلومات;
        size_t lda = لیکې > 1 ? ګام/حجم(a[0]) : 1;
        size_t ldb = لیکې > 1 ? m.ګام/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    وإلا إذا( d == CV_64F )
    {
        ثابت مزدوج *a = (ثابت مزدوج*)معلومات, *b = (ثابت مزدوج*)m.معلومات;
        مزدوج* c = (مزدوج*)پایله.معلومات;
        size_t lda = لیکې > 1 ? ګام/حجم(a[0]) : 1;
        size_t ldb = لیکې > 1 ? m.ګام/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    أرجع پایله;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

نطاق cv
{

تعريفنوع فراغ (*Reduce_مجموعه_Func)(ثابت Mat& src, Mat& dst);
Reduce_مجموعه_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_مجموعه_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_مجموعه_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth);

کينډۍ <ډول_نوم T, ډول_نوم WT, ډول_نوم Op>
بنية ReduceR_SIMD
{
    صحيح چلوونکی()(ثابت T*, صحيح پیل, صحيح, WT*, ثابت Op&) ثابت
    {
        أرجع پیل;
    }
};


کينډۍ<ډول_نوم T, ډول_نوم ST, ډول_نوم WT, صنف Op, صنف OpInit>
صنف ReduceR_Invoker : عام ParallelLoopBody
{
عام:
  ReduceR_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),بفر(srcmat.size().پلنوالی*srcmat.channels())
  {
  }
  فراغ چلوونکی()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت T* src = srcmat.ptr<T>();
    ثابت size_t srcstep = srcmat.ګام/حجم(src[0]);
    WT* buf = بفر.معلومات();
    ST* dst = dstmat.ptr<ST>();
    صحيح i = 0;

    لكل( i = range.پیل ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    صحيح لوړوالی = srcmat.size().لوړوالی;
    لكل( ; --لوړوالی; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.پیل, range.end, buf, op);
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
        #د_که_پای
        لكل( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    لكل( i = range.پیل ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
خاص:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  بدلیدونکی Auto_بفر<WT> بفر;
};

کينډۍ<ډول_نوم T, ډول_نوم ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceR_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع ډول_نوم Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().پلنوالی*srcmat.channels()), body, srcmat.size().پلنوالی*CV_ELEM_اندازه(srcmat.ژوروالی())/64);
}

کينډۍ<ډول_نوم T, ډول_نوم ST, ډول_نوم WT, صنف Op, صنف OpInit>
صنف ReduceC_Invoker : عام ParallelLoopBody
{
عام:
  ReduceC_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  فراغ چلوونکی()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح پلنوالی = srcmat.size().پلنوالی*cn;
    Auto_بفر<WT> cumul(cn);
    لكل( صحيح y = range.پیل; y < range.end; y++ )
    {
        ثابت T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        إذا( پلنوالی == cn )
        {
          لكل( صحيح k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        وإلا
        {
            لكل(صحيح k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            لكل(صحيح k = cn ; k < پلنوالی ; k += cn )
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

کينډۍ<ډول_نوم T, ډول_نوم ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceC_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع ډول_نوم Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().لوړوالی), body);
}

تعريفنوع فراغ (*ReduceFunc)( ثابت Mat& src, Mat& dst );

}

#تعریف_کړه reduce_مجموعه_R8u32s  reduceR_<uchar, صحيح,   Op_جمع_کړه<صحيح>, OpNop<صحيح> >
#تعریف_کړه reduce_مجموعه_R8u32f  reduceR_<uchar, عائم, Op_جمع_کړه<صحيح>, OpNop<صحيح> >
#تعریف_کړه reduce_مجموعه_R8u64f  reduceR_<uchar, مزدوج,Op_جمع_کړه<صحيح>, OpNop<صحيح> >
#تعریف_کړه reduce_مجموعه_R16u32f reduceR_<ushort,عائم, Op_جمع_کړه<عائم> >
#تعریف_کړه reduce_مجموعه_R16u64f reduceR_<ushort,مزدوج,Op_جمع_کړه<مزدوج> >
#تعریف_کړه reduce_مجموعه_R16s32f reduceR_<لنډ, عائم, Op_جمع_کړه<عائم> >
#تعریف_کړه reduce_مجموعه_R16s64f reduceR_<لنډ, مزدوج,Op_جمع_کړه<مزدوج> >
#تعریف_کړه reduce_مجموعه_R32f32f reduceR_<عائم, عائم, Op_جمع_کړه<عائم> >
#تعریف_کړه reduce_مجموعه_R32f64f reduceR_<عائم, مزدوج,Op_جمع_کړه<مزدوج> >
#تعریف_کړه reduce_مجموعه_R64f64f reduceR_<مزدوج,مزدوج,Op_جمع_کړه<مزدوج> >

#تعریف_کړه reduceSum2R8u32s  reduceR_<uchar, صحيح,   Op_جمع_کړه_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کړه reduceSum2R8u32f  reduceR_<uchar, عائم, Op_جمع_کړه_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کړه reduceSum2R8u64f  reduceR_<uchar, مزدوج,Op_جمع_کړه_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کړه reduceSum2R16u32f reduceR_<ushort,عائم, Op_جمع_کړه_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کړه reduceSum2R16u64f reduceR_<ushort,مزدوج,Op_جمع_کړه_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کړه reduceSum2R16s32f reduceR_<لنډ, عائم, Op_جمع_کړه_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کړه reduceSum2R16s64f reduceR_<لنډ, مزدوج,Op_جمع_کړه_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کړه reduceSum2R32f32f reduceR_<عائم, عائم, Op_جمع_کړه_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کړه reduceSum2R32f64f reduceR_<عائم, مزدوج,Op_جمع_کړه_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کړه reduceSum2R64f64f reduceR_<مزدوج,مزدوج,Op_جمع_کړه_Sqr<مزدوج>,OpSqr<مزدوج> >

#تعریف_کړه reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#تعریف_کړه reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#تعریف_کړه reduceMaxR16s reduceR_<لنډ, لنډ, OpMax<لنډ> >
#تعریف_کړه reduceMaxR32f reduceR_<عائم, عائم, OpMax<عائم> >
#تعریف_کړه reduceMaxR64f reduceR_<مزدوج,مزدوج,OpMax<مزدوج> >

#تعریف_کړه reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#تعریف_کړه reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#تعریف_کړه reduceMinR16s reduceR_<لنډ, لنډ, OpMin<لنډ> >
#تعریف_کړه reduceMinR32f reduceR_<عائم, عائم, OpMin<عائم> >
#تعریف_کړه reduceMinR64f reduceR_<مزدوج,مزدوج,OpMin<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ساكن مضمن منطقي ipp_reduce_مجموعه_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    صحيح sstep = (صحيح)srcmat.ګام, stype = srcmat.type(),
            ddepth = dstmat.ژوروالی();

    Ippi_اندازه roisize = { srcmat.size().پلنوالی, 1 };

    تعريفنوع Ipp_حالت (CV_STDCALL * Ippi_مجموعه)(ثابت فراغ * pSrc, صحيح src_ګام_3, Ippi_اندازه roi_اندازه, Ipp64f* p_مجموعه);
    تعريفنوع Ipp_حالت (CV_STDCALL * Ippi_مجموعه_Hint)(ثابت فراغ * pSrc, صحيح src_ګام_3, Ippi_اندازه roi_اندازه, Ipp64f* p_مجموعه, IppHintAlgorithm hint);
    Ippi_مجموعه ippi_مجموعه = 0;
    Ippi_مجموعه_Hint ippi_مجموعه_Hint = 0;

    إذا(ddepth == CV_64F)
    {
        ippi_مجموعه =
            stype == CV_8UC1 ? (Ippi_مجموعه)ippi_مجموعه_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_مجموعه)ippi_مجموعه_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_مجموعه)ippi_مجموعه_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_مجموعه)ippi_مجموعه_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_مجموعه)ippi_مجموعه_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_مجموعه)ippi_مجموعه_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_مجموعه)ippi_مجموعه_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_مجموعه)ippi_مجموعه_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_مجموعه)ippi_مجموعه_16s_C4_R : 0;
        ippi_مجموعه_Hint =
            stype == CV_32FC1 ? (Ippi_مجموعه_Hint)ippi_مجموعه_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_مجموعه_Hint)ippi_مجموعه_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_مجموعه_Hint)ippi_مجموعه_32f_C4_R : 0;
    }

    إذا(ippi_مجموعه)
    {
        لكل(صحيح y = 0; y < srcmat.size().لوړوالی; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippi_مجموعه, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                أرجع خطأ;
        }
        أرجع ریښتیا;
    }
    وإلا إذا(ippi_مجموعه_Hint)
    {
        لكل(صحيح y = 0; y < srcmat.size().لوړوالی; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippi_مجموعه_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                أرجع خطأ;
        }
        أرجع ریښتیا;
    }

    أرجع خطأ;
}

ساكن مضمن فراغ reduce_مجموعه_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_پرلپسې_FAST(ipp_reduce_مجموعه_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    إذا(dstmat.ژوروالی() == CV_64F)
    {
        صحيح sdepth = CV_MAT_ژوروالی(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, مزدوج,   cv::Op_جمع_کړه<مزدوج> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, مزدوج,   cv::Op_جمع_کړه<مزدوج> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<لنډ, مزدوج,   cv::Op_جمع_کړه<مزدوج> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<عائم, مزدوج,   cv::Op_جمع_کړه<مزدوج> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#د_که_پای

#تعریف_کړه reduce_مجموعه_C8u32s  reduceC_<uchar, صحيح,   Op_جمع_کړه<صحيح>, OpNop<صحيح> >
#تعریف_کړه reduce_مجموعه_C8u32f  reduceC_<uchar, عائم, Op_جمع_کړه<صحيح>, OpNop<صحيح> >
#تعریف_کړه reduce_مجموعه_C16u32f reduceC_<ushort,عائم, Op_جمع_کړه<عائم> >
#تعریف_کړه reduce_مجموعه_C16s32f reduceC_<لنډ, عائم, Op_جمع_کړه<عائم> >
#تعریف_کړه reduce_مجموعه_C32f32f reduceC_<عائم, عائم, Op_جمع_کړه<عائم> >
#تعریف_کړه reduce_مجموعه_C64f64f reduceC_<مزدوج,مزدوج,Op_جمع_کړه<مزدوج> >

#تعریف_کړه reduceSum2C8u32s  reduceC_<uchar, صحيح,   Op_جمع_کړه_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کړه reduceSum2C8u32f  reduceC_<uchar, عائم, Op_جمع_کړه_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کړه reduceSum2C16u32f reduceC_<ushort,عائم, Op_جمع_کړه_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کړه reduceSum2C16s32f reduceC_<لنډ, عائم, Op_جمع_کړه_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کړه reduceSum2C32f32f reduceC_<عائم, عائم, Op_جمع_کړه_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کړه reduceSum2C64f64f reduceC_<مزدوج,مزدوج,Op_جمع_کړه_Sqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#تعریف_کړه reduce_مجموعه_C8u64f  reduce_مجموعه_C_8u16u16s32f_64f
#تعریف_کړه reduce_مجموعه_C16u64f reduce_مجموعه_C_8u16u16s32f_64f
#تعریف_کړه reduce_مجموعه_C16s64f reduce_مجموعه_C_8u16u16s32f_64f
#تعریف_کړه reduce_مجموعه_C32f64f reduce_مجموعه_C_8u16u16s32f_64f
#وإلا
#تعریف_کړه reduce_مجموعه_C8u64f  reduceC_<uchar, مزدوج,Op_جمع_کړه<صحيح>, OpNop<صحيح> >
#تعریف_کړه reduce_مجموعه_C16u64f reduceC_<ushort,مزدوج,Op_جمع_کړه<مزدوج> >
#تعریف_کړه reduce_مجموعه_C16s64f reduceC_<لنډ, مزدوج,Op_جمع_کړه<مزدوج> >
#تعریف_کړه reduce_مجموعه_C32f64f reduceC_<عائم, مزدوج,Op_جمع_کړه<مزدوج> >
#د_که_پای

#تعریف_کړه reduceSum2C8u64f  reduceC_<uchar, مزدوج,Op_جمع_کړه_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کړه reduceSum2C16u64f reduceC_<ushort,مزدوج,Op_جمع_کړه_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کړه reduceSum2C16s64f reduceC_<لنډ, مزدوج,Op_جمع_کړه_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کړه reduceSum2C32f64f reduceC_<عائم, مزدوج,Op_جمع_کړه_Sqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#تعریف_کړه REDUCE_OP(favor, optype, type1, type2) \
ساكن مضمن منطقي ipp_reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    إذا((srcmat.channels() == 1)) \
    { \
        صحيح sstep = (صحيح)srcmat.ګام; \
        تعريفنوع Ipp##favor IppType; \
        Ippi_اندازه roisize = ippi_اندازه(srcmat.size().پلنوالی, 1);\
        لكل(صحيح y = 0; y < srcmat.size().لوړوالی; y++)\
        {\
            إذا(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                أرجع خطأ;\
        }\
        أرجع ریښتیا;\
    }\
    أرجع خطأ; \
} \
ساكن مضمن فراغ reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_پرلپسې_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#د_که_پای

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, لنډ, لنډ)
REDUCE_OP(32f, Max, عائم, عائم)
#وإلا
#تعریف_کړه reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#تعریف_کړه reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#تعریف_کړه reduceMaxC16s reduceC_<لنډ, لنډ, OpMax<لنډ> >
#تعریف_کړه reduceMaxC32f reduceC_<عائم, عائم, OpMax<عائم> >
#د_که_پای
#تعریف_کړه reduceMaxC64f reduceC_<مزدوج,مزدوج,OpMax<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, لنډ, لنډ)
REDUCE_OP(32f, Min, عائم, عائم)
#وإلا
#تعریف_کړه reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#تعریف_کړه reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#تعریف_کړه reduceMinC16s reduceC_<لنډ, لنډ, OpMin<لنډ> >
#تعریف_کړه reduceMinC32f reduceC_<عائم, عائم, OpMin<عائم> >
#د_که_پای
#تعریف_کړه reduceMinC64f reduceC_<مزدوج,مزدوج,OpMin<مزدوج> >

#که_تعریف_شوی HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_reduce(ننوت_Array _src, OutputArray _dst,
                       صحيح dim, صحيح op, صحيح op0, صحيح stype, صحيح dtype)
{
    ثابت صحيح min_opt_ستون_شمېر = 128, buf_ستون_شمېر = 32;
    صحيح sdepth = CV_MAT_ژوروالی(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_ژوروالی(dtype), ddepth0 = ddepth;
    ثابت ocl::Device &defDev = ocl::Device::getDefault();
    منطقي doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_ډله_اندازه();
    منطقي useOptimized = 1 == dim && _src.ستون_شمېر() > min_opt_ستون_شمېر && (wgs >= buf_ستون_شمېر);

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
        size_t tile_لوړوالی = (size_t)(wgs / buf_ستون_شمېر);
        إذا (defDev.isIntel())
        {
            ساكن ثابت size_t max_توکی_دننه_ډله_شمېر = 16;
            tile_لوړوالی = min(tile_لوړوالی, defDev.local_Mem_اندازه() / buf_ستون_شمېر / CV_ELEM_اندازه(CV_MAKETYPE(wdepth, cn)) / max_توکی_دننه_ډله_شمېر);
        }
        حرف cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_ستون_شمېر, tile_لوړوالی, ops[op], cn, ddepth,
                                            ocl::type_منزل_Str(sdepth),
                                            ocl::type_منزل_Str(ddepth),
                                            ocl::type_منزل_Str(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], حجم(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], حجم(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        إذا (k.empty())
            أرجع خطأ;
        UMat src = _src.getUMat();
        اندازه dsize(1, src.لیکې);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        إذا (op0 == REDUCE_AVG)
            k.دلیلونه(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_اندازه(dst), 1.0f / src.ستون_شمېر);
        وإلا
            k.دلیلونه(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_اندازه(dst));

        size_t local_اندازه[2] = { (size_t)buf_ستون_شمېر, (size_t)tile_لوړوالی};
        size_t global_اندازه[2] = { (size_t)buf_ستون_شمېر, (size_t)src.لیکې };
        أرجع k.پرلپسې(2, global_اندازه, local_اندازه, خطأ);
    }
    وإلا
    {
        حرف cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::type_منزل_Str(useOptimized ? ddepth : sdepth),
                                      ocl::type_منزل_Str(ddepth), ocl::type_منزل_Str(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], حجم(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], حجم(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        إذا (k.empty())
            أرجع خطأ;

        UMat src = _src.getUMat();
        اندازه dsize(dim == 0 ? src.ستون_شمېر : 1, dim == 0 ? 1 : src.لیکې);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_اندازه(dst);

        إذا (op0 == REDUCE_AVG)
            k.دلیلونه(srcarg, temparg, 1.0f / (dim == 0 ? src.لیکې : src.ستون_شمېر));
        وإلا
            k.دلیلونه(srcarg, temparg);

        size_t globalsize = std::max(dsize.پلنوالی, dsize.لوړوالی);
        أرجع k.پرلپسې(1, &globalsize, عدم, خطأ);
    }
}

}

#د_که_پای

فراغ cv::reduce(ننوت_Array _src, OutputArray _dst, صحيح dim, صحيح op, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    إذا (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, ریښتیا, dtype < 0 && _dst.fixedType() ? _dst.ژوروالی() : dtype));
        أرجع;
    }
    صحيح op0 = op;
    صحيح stype = _src.type(), sdepth = CV_MAT_ژوروالی(stype), cn = CV_MAT_CN(stype);
    إذا( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    صحيح ddepth = CV_MAT_ژوروالی(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_مجموعه || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_پرلپسې(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    إذا (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    إذا (src.empty()) {
        src.convert_منزل(_dst, dtype);
        أرجع;
    }
    إذا (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.لیکې, dim == 0 ? src.ستون_شمېر : 1, dtype);
    Mat dst = _dst.getMat(), لنډمهاله_2 = dst;

    إذا( op == REDUCE_AVG )
    {
        op = REDUCE_مجموعه;
        إذا( sdepth < CV_32S && ddepth < CV_32S )
        {
            لنډمهاله_2.create(dst.لیکې, dst.ستون_شمېر, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    إذا( dim == 0 )
    {
        إذا( op == REDUCE_مجموعه )
        {
            Reduce_مجموعه_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_مجموعه_R8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_مجموعه_R8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_مجموعه_R8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_مجموعه_R16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_مجموعه_R16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_مجموعه_R16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_مجموعه_R16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_مجموعه_R32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_مجموعه_R32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_مجموعه_R64f64f;
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
            Reduce_مجموعه_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
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
        إذا(op == REDUCE_مجموعه)
        {
            Reduce_مجموعه_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_مجموعه_C8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_مجموعه_C8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_مجموعه_C8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_مجموعه_C16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_مجموعه_C16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_مجموعه_C16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_مجموعه_C16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_مجموعه_C32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_مجموعه_C32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_مجموعه_C64f64f;
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

    func( src, لنډمهاله_2 );

    إذا( op0 == REDUCE_AVG )
        لنډمهاله_2.convert_منزل(dst, dst.type(), 1./(dim == 0 ? src.لیکې : src.ستون_شمېر));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

نطاق cv
{

کينډۍ<ډول_نوم T> ساكن فراغ ترتیبول( ثابت Mat& src, Mat& dst, صحيح flags )
{
    صحيح n, len;
    منطقي ترتیبول_لیکې = (flags & 1) == ترتیبول_EVERY_لیکه;
    منطقي inplace = src.معلومات == dst.معلومات;
    منطقي ترتیبول_Descending = (flags & ترتیبول_DESCENDING) != 0;

    إذا( ترتیبول_لیکې )
        n = src.لیکې, len = src.ستون_شمېر;
    وإلا
        n = src.ستون_شمېر, len = src.لیکې;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_بفر<T> buf;
        إذا( !ترتیبول_لیکې )
            buf.allocate(len);
        T* bptr = buf.معلومات();

        لكل( صحيح i = range.پیل; i < range.end; i++ )
        {
            T* ptr = bptr;
            إذا( ترتیبول_لیکې )
            {
                T* dptr = dst.ptr<T>(i);
                إذا( !inplace )
                {
                    ثابت T* sptr = src.ptr<T>(i);
                    حافظه_نقل(dptr, sptr, حجم(T) * len);
                }
                ptr = dptr;
            }
            وإلا
            {
                لكل( صحيح j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            إذا( ترتیبول_Descending )
            {
                لكل( صحيح j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            إذا( !ترتیبول_لیکې )
                لكل( صحيح j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#إذا defined(HAVE_IPP) && !IPP_DISABLE_ترتیبول
تعريفنوع Ipp_حالت (CV_STDCALL *Ipp_ترتیبول_Func)(فراغ  *pSrcDst, صحيح    len, Ipp8u *p_بفر);

کينډۍ<ډول_نوم T, Ipp_حالت (CV_STDCALL *fn)(T*, صحيح, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ipp_ترتیبول_Wrap(فراغ* pSrcDst, صحيح len, Ipp8u* p_بفر)
{
    أرجع fn((T*)pSrcDst, len, p_بفر);
}

ساكن Ipp_ترتیبول_Func get_ترتیبول_Func(صحيح ژوروالی, منطقي ترتیبول_Descending)
{
    إذا (!ترتیبول_Descending)
        أرجع ژوروالی == CV_8U ? ipp_ترتیبول_Wrap<Ipp8u, ipps_ترتیبول_Radix_Ascend_8u_I> :
            ژوروالی == CV_16U ? ipp_ترتیبول_Wrap<Ipp16u, ipps_ترتیبول_Radix_Ascend_16u_I> :
            ژوروالی == CV_16S ? ipp_ترتیبول_Wrap<Ipp16s, ipps_ترتیبول_Radix_Ascend_16s_I> :
            ژوروالی == CV_32S ? ipp_ترتیبول_Wrap<Ipp32s, ipps_ترتیبول_Radix_Ascend_32s_I> :
            ژوروالی == CV_32F ? ipp_ترتیبول_Wrap<Ipp32f, ipps_ترتیبول_Radix_Ascend_32f_I> :
            ژوروالی == CV_64F ? ipp_ترتیبول_Wrap<Ipp64f, ipps_ترتیبول_Radix_Ascend_64f_I> :
            0;
    وإلا
        أرجع ژوروالی == CV_8U ? ipp_ترتیبول_Wrap<Ipp8u, ipps_ترتیبول_Radix_Descend_8u_I> :
            ژوروالی == CV_16U ? ipp_ترتیبول_Wrap<Ipp16u, ipps_ترتیبول_Radix_Descend_16u_I> :
            ژوروالی == CV_16S ? ipp_ترتیبول_Wrap<Ipp16s, ipps_ترتیبول_Radix_Descend_16s_I> :
            ژوروالی == CV_32S ? ipp_ترتیبول_Wrap<Ipp32s, ipps_ترتیبول_Radix_Descend_32s_I> :
            ژوروالی == CV_32F ? ipp_ترتیبول_Wrap<Ipp32f, ipps_ترتیبول_Radix_Descend_32f_I> :
            ژوروالی == CV_64F ? ipp_ترتیبول_Wrap<Ipp64f, ipps_ترتیبول_Radix_Descend_64f_I> :
            0;
}

ساكن منطقي ipp_ترتیبول(ثابت Mat& src, Mat& dst, صحيح flags)
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        ترتیبول_لیکې        = (flags & 1) == ترتیبول_EVERY_لیکه;
    منطقي        ترتیبول_Descending  = (flags & ترتیبول_DESCENDING) != 0;
    منطقي        inplace         = (src.معلومات == dst.معلومات);
    صحيح         ژوروالی           = src.ژوروالی();
    Ipp_معلومات_Type type            = ippi_Get_معلومات_Type(ژوروالی);

    Ipp_ترتیبول_Func ipps_ترتیبول_Radix_I = get_ترتیبول_Func(ژوروالی, ترتیبول_Descending);
    إذا(!ipps_ترتیبول_Radix_I)
        أرجع خطأ;

    صحيح n, len;
    إذا( ترتیبول_لیکې )
        n = src.لیکې, len = src.ستون_شمېر;
    وإلا
        n = src.ستون_شمېر, len = src.لیکې;

    صحيح بفر_اندازه;
    إذا(ipps_ترتیبول_Radix_Get_بفر_اندازه(len, type, &بفر_اندازه) < 0)
        أرجع خطأ;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    متطاير منطقي ok = ریښتیا;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_بفر<Ipp8u> بفر(بفر_اندازه);
        Mat  لیکه;
        Rect subRect(0,0,1,src.لیکې);
        إذا( !ترتیبول_لیکې )
            لیکه.create(1, src.لیکې, src.type());

        لكل( صحيح i = range.پیل; i < range.end; i++ )
        {
            إذا( !ok )
                اكسر;
            فراغ* ptr;
            إذا( ترتیبول_لیکې )
            {
                uchar* dptr = dst.ptr(i);
                إذا( !inplace )
                    حافظه_نقل(dptr, src.ptr(i), src.ستون_شمېر * src.elem_اندازه());
                ptr = dptr;
            }
            وإلا
            {
                subRect.x = i;
                Mat(src, subRect).copy_منزل(لیکه);
                ptr = لیکه.ptr();
            }

            إذا( CV_INSTRUMENT_FUN_IPP(ipps_ترتیبول_Radix_I, ptr, len, بفر.معلومات()) < 0 )
            {
                ok = خطأ;
                اكسر;
            }

            إذا( !ترتیبول_لیکې )
            {
                Mat dstSub(dst, subRect);
                لیکه = لیکه.reshape(1, dstSub.لیکې);
                لیکه.copy_منزل(dstSub);
                لیکه = لیکه.reshape(1, 1);
            }
        }
    });

    أرجع ok;
}
#د_که_پای

کينډۍ<ډول_نوم _Tp> صنف LessThanIdx
{
عام:
    LessThanIdx( ثابت _Tp* _arr ) : arr(_arr) {}
    منطقي چلوونکی()(صحيح a, صحيح b) ثابت { أرجع arr[a] < arr[b]; }
    ثابت _Tp* arr;
};

کينډۍ<ډول_نوم T> ساكن فراغ ترتیبول_Idx_2( ثابت Mat& src, Mat& dst, صحيح flags )
{
    Auto_بفر<T> buf;
    Auto_بفر<صحيح> ibuf;
    منطقي ترتیبول_لیکې = (flags & 1) == ترتیبول_EVERY_لیکه;
    منطقي ترتیبول_Descending = (flags & ترتیبول_DESCENDING) != 0;

    CV_Assert( src.معلومات != dst.معلومات );

    صحيح n, len;
    إذا( ترتیبول_لیکې )
        n = src.لیکې, len = src.ستون_شمېر;
    وإلا
    {
        n = src.ستون_شمېر, len = src.لیکې;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.معلومات();
    صحيح* _iptr = ibuf.معلومات();

    لكل( صحيح i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        صحيح* iptr = _iptr;

        إذا( ترتیبول_لیکې )
        {
            ptr = (T*)(src.معلومات + src.ګام*i);
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
        إذا( ترتیبول_Descending )
        {
            لكل( صحيح j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        إذا( !ترتیبول_لیکې )
            لكل( صحيح j = 0; j < len; j++ )
                dst.ptr<صحيح>(j)[i] = iptr[j];
    }
}

#إذا defined(HAVE_IPP) && !IPP_DISABLE_ترتیبول
تعريفنوع Ipp_حالت (CV_STDCALL *Ipp_ترتیبول_شاخص_Func)(ثابت فراغ*  pSrc, Ipp32s src_Stride_بایټونه, Ipp32s *pDstIndx, صحيح len, Ipp8u *p_بفر);

کينډۍ<ډول_نوم T, Ipp_حالت (CV_STDCALL *fn)(ثابت T*, Ipp32s, Ipp32s*, صحيح, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ipp_ترتیبول_شاخص_Wrap(ثابت فراغ* pSrc, Ipp32s src_Stride_بایټونه, Ipp32s *pDstIndx, صحيح len, Ipp8u *p_بفر)
{
    أرجع fn((ثابت T*)pSrc, src_Stride_بایټونه, pDstIndx, len, p_بفر);
}

ساكن Ipp_ترتیبول_شاخص_Func get_ترتیبول_شاخص_Func(صحيح ژوروالی, منطقي ترتیبول_Descending)
{
    إذا (!ترتیبول_Descending)
        أرجع ژوروالی == CV_8U ? ipp_ترتیبول_شاخص_Wrap<Ipp8u, ipps_ترتیبول_Radix_شاخص_Ascend_8u> :
            ژوروالی == CV_16U ? ipp_ترتیبول_شاخص_Wrap<Ipp16u, ipps_ترتیبول_Radix_شاخص_Ascend_16u> :
            ژوروالی == CV_16S ? ipp_ترتیبول_شاخص_Wrap<Ipp16s, ipps_ترتیبول_Radix_شاخص_Ascend_16s> :
            ژوروالی == CV_32S ? ipp_ترتیبول_شاخص_Wrap<Ipp32s, ipps_ترتیبول_Radix_شاخص_Ascend_32s> :
            ژوروالی == CV_32F ? ipp_ترتیبول_شاخص_Wrap<Ipp32f, ipps_ترتیبول_Radix_شاخص_Ascend_32f> :
            0;
    وإلا
        أرجع ژوروالی == CV_8U ? ipp_ترتیبول_شاخص_Wrap<Ipp8u, ipps_ترتیبول_Radix_شاخص_Descend_8u> :
            ژوروالی == CV_16U ? ipp_ترتیبول_شاخص_Wrap<Ipp16u, ipps_ترتیبول_Radix_شاخص_Descend_16u> :
            ژوروالی == CV_16S ? ipp_ترتیبول_شاخص_Wrap<Ipp16s, ipps_ترتیبول_Radix_شاخص_Descend_16s> :
            ژوروالی == CV_32S ? ipp_ترتیبول_شاخص_Wrap<Ipp32s, ipps_ترتیبول_Radix_شاخص_Descend_32s> :
            ژوروالی == CV_32F ? ipp_ترتیبول_شاخص_Wrap<Ipp32f, ipps_ترتیبول_Radix_شاخص_Descend_32f> :
            0;
}

ساكن منطقي ipp_ترتیبول_Idx( ثابت Mat& src, Mat& dst, صحيح flags )
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        ترتیبول_لیکې        = (flags & 1) == ترتیبول_EVERY_لیکه;
    منطقي        ترتیبول_Descending  = (flags & ترتیبول_DESCENDING) != 0;
    صحيح         ژوروالی           = src.ژوروالی();
    Ipp_معلومات_Type type            = ippi_Get_معلومات_Type(ژوروالی);

    Ipp_ترتیبول_شاخص_Func ipps_ترتیبول_Radix_شاخص = get_ترتیبول_شاخص_Func(ژوروالی, ترتیبول_Descending);
    إذا(!ipps_ترتیبول_Radix_شاخص)
        أرجع خطأ;

    إذا(ترتیبول_لیکې)
    {
        Auto_بفر<Ipp8u> بفر;
        صحيح               بفر_اندازه;
        إذا(ipps_ترتیبول_Radix_شاخص_Get_بفر_اندازه(src.ستون_شمېر, type, &بفر_اندازه) < 0)
            أرجع خطأ;

        بفر.allocate(بفر_اندازه);

        لكل(صحيح i = 0; i < src.لیکې; i++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ipps_ترتیبول_Radix_شاخص, (ثابت فراغ*)src.ptr(i), (Ipp32s)src.ګام[1], (Ipp32s*)dst.ptr(i), src.ستون_شمېر, بفر.معلومات()) < 0)
                أرجع خطأ;
        }
    }
    وإلا
    {
        Mat  dst_لیکه(1, dst.لیکې, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.لیکې);

        Auto_بفر<Ipp8u> بفر;
        صحيح               بفر_اندازه;
        إذا(ipps_ترتیبول_Radix_شاخص_Get_بفر_اندازه(src.لیکې, type, &بفر_اندازه) < 0)
            أرجع خطأ;

        بفر.allocate(بفر_اندازه);

        Ipp32s src_ګام_3 = (Ipp32s)src.ګام[0];
        لكل(صحيح i = 0; i < src.ستون_شمېر; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            إذا(CV_INSTRUMENT_FUN_IPP(ipps_ترتیبول_Radix_شاخص, (ثابت فراغ*)src.ptr(0, i), src_ګام_3, (Ipp32s*)dst_لیکه.ptr(), src.لیکې, بفر.معلومات()) < 0)
                أرجع خطأ;

            dst_لیکه = dst_لیکه.reshape(1, dstSub.لیکې);
            dst_لیکه.copy_منزل(dstSub);
        }
    }

    أرجع ریښتیا;
}
#د_که_پای

تعريفنوع فراغ (*ترتیبول_Func)(ثابت Mat& src, Mat& dst, صحيح flags);
}

فراغ cv::sort( ننوت_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_اندازه( src, src.type() );
    Mat dst = _dst.getMat();
#إذا !IPP_DISABLE_ترتیبول
    CV_IPP_پرلپسې_FAST(ipp_ترتیبول(src, dst, flags));
#د_که_پای

    ساكن ترتیبول_Func tab[CV_ژوروالی_MAX] =
    {
        ترتیبول<uchar>, ترتیبول<schar>, ترتیبول<ushort>, ترتیبول<لنډ>,
        ترتیبول<صحيح>, ترتیبول<عائم>, ترتیبول<مزدوج>, 0
    };
    ترتیبول_Func func = tab[src.ژوروالی()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

فراغ cv::ترتیبول_Idx( ننوت_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    إذا( dst.معلومات == src.معلومات )
        _dst.release();
    _dst.create_Same_اندازه( src, CV_32S );
    dst = _dst.getMat();

#إذا !IPP_DISABLE_ترتیبول
    CV_IPP_پرلپسې_FAST(ipp_ترتیبول_Idx(src, dst, flags));
#د_که_پای

    ساكن ترتیبول_Func tab[CV_ژوروالی_MAX] =
    {
        ترتیبول_Idx_2<uchar>, ترتیبول_Idx_2<schar>, ترتیبول_Idx_2<ushort>, ترتیبول_Idx_2<لنډ>,
        ترتیبول_Idx_2<صحيح>, ترتیبول_Idx_2<عائم>, ترتیبول_Idx_2<مزدوج>, 0
    };
    ترتیبول_Func func = tab[src.ژوروالی()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
