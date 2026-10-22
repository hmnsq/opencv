// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"
#ضمن "opencl_kernels_core.hpp"

#تعریف_کن IPP_DISABLE_REDUCE 1
#تعریف_کن IPP_DISABLE_مرتب_سازی   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

فراغ cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.ردیفها, b.ردیفها);
    std::swap(a.تعداد_ستون, b.تعداد_ستون);
    std::swap(a.داده, b.داده);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.گام, b.گام);
}


فراغ cv::hconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح مجموع_تعداد_ستون = 0, تعداد_ستون = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].ردیفها == src[0].ردیفها &&
                   src[i].type() == src[0].type());
        مجموع_تعداد_ستون += src[i].تعداد_ستون;
    }
    _dst.create( src[0].ردیفها, مجموع_تعداد_ستون, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(تعداد_ستون, 0, src[i].تعداد_ستون, src[i].ردیفها));
        src[i].copy_مقصد(dpart);
        تعداد_ستون += src[i].تعداد_ستون;
    }
}

فراغ cv::hconcat(درون_داد_Array src1, درون_داد_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

فراغ cv::hconcat(درون_داد_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_وکتور(src);
    hconcat(!src.empty() ? &src[0] : اشاره_تهی, src.size(), dst);
}

فراغ cv::vconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح مجموع_ردیفها = 0, ردیفها = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].تعداد_ستون == src[0].تعداد_ستون &&
                  src[i].type() == src[0].type());
        مجموع_ردیفها += src[i].ردیفها;
    }
    _dst.create( مجموع_ردیفها, src[0].تعداد_ستون, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, ردیفها, src[i].تعداد_ستون, src[i].ردیفها));
        src[i].copy_مقصد(dpart);
        ردیفها += src[i].ردیفها;
    }
}

فراغ cv::vconcat(درون_داد_Array src1, درون_داد_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

فراغ cv::vconcat(درون_داد_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_وکتور(src);
    vconcat(!src.empty() ? &src[0] : اشاره_تهی, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#اگر_تعریف_شده HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_مجموعه_عنصر_خنثی( درون_داد_Output_Array _m, ثابت Scalar& s )
{
    صحيح type = _m.type(), عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type), kercn = cn, ردیفها_در_هر_WI = 1;
    صحيح sctype = CV_MAKE_TYPE(عمق, cn == 3 ? 4 : cn);
    إذا (ocl::Device::getDefault().isIntel())
    {
        ردیفها_در_هر_WI = 4;
        إذا (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_وکتور_عرض(_m), 4);
            إذا (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::مجموعه_عنصر_خنثی_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_مقصد_Str(CV_MAKE_TYPE(عمق, kercn)),
                         ocl::memop_Type_مقصد_Str(عمق), cn,
                         ocl::memop_Type_مقصد_Str(sctype),
                         kercn, ردیفها_در_هر_WI));
    إذا (k.empty())
        أرجع خطأ;

    UMat m = _m.getUMat();
    k.آرگومانها(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.تعداد_ستون * cn / kercn, ((size_t)m.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, اشاره_تهی, خطأ);
}

}

#پایان_اگر

فراغ cv::مجموعه_عنصر_خنثی( درون_داد_Output_Array _m, ثابت Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_پیاپی(_m.isUMat(),
               ocl_مجموعه_عنصر_خنثی(_m, s))

    Mat m = _m.getMat();
    صحيح ردیفها = m.ردیفها, تعداد_ستون = m.تعداد_ستون, type = m.type();

    إذا( type == CV_32FC1 )
    {
        عائم* داده = m.ptr<عائم>();
        عائم val = (عائم)s[0];
        size_t گام = m.گام/حجم(داده[0]);

        لكل( صحيح i = 0; i < ردیفها; i++, داده += گام )
        {
            لكل( صحيح j = 0; j < تعداد_ستون; j++ )
                داده[j] = 0;
            إذا( i < تعداد_ستون )
                داده[i] = val;
        }
    }
    وإلا إذا( type == CV_64FC1 )
    {
        مزدوج* داده = m.ptr<مزدوج>();
        مزدوج val = s[0];
        size_t گام = m.گام/حجم(داده[0]);

        لكل( صحيح i = 0; i < ردیفها; i++, داده += گام )
        {
            std::fill(داده, داده + تعداد_ستون, 0.0);
            إذا (i < تعداد_ستون)
                داده[i] = val;
        }
    }
    وإلا
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


نطاق cv {

UMat UMat::eye(صحيح ردیفها, صحيح تعداد_ستون, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat::eye(اندازه(تعداد_ستون, ردیفها), type, usageFlags);
}

UMat UMat::eye(اندازه size, صحيح type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    مجموعه_عنصر_خنثی(m);
    أرجع m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( درون_داد_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    صحيح type = m.type();
    صحيح nm = std::min(m.ردیفها, m.تعداد_ستون);

    إذا( type == CV_32FC1 )
    {
        ثابت عائم* ptr = m.ptr<عائم>();
        size_t گام = m.گام/حجم(ptr[0]) + 1;
        مزدوج _s = 0;
        لكل( صحيح i = 0; i < nm; i++ )
            _s += ptr[i*گام];
        أرجع _s;
    }

    إذا( type == CV_64FC1 )
    {
        ثابت مزدوج* ptr = m.ptr<مزدوج>();
        size_t گام = m.گام/حجم(ptr[0]) + 1;
        مزدوج _s = 0;
        لكل( صحيح i = 0; i < nm; i++ )
            _s += ptr[i*گام];
        أرجع _s;
    }

    أرجع cv::جمع(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

فراغ cv::completeSymm( درون_داد_Output_Array _m, منطقي LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t گام = m.گام, esz = m.elem_اندازه();
    CV_Assert( m.dims <= 2 && m.ردیفها == m.تعداد_ستون );

    صحيح ردیفها = m.ردیفها;
    صحيح j0 = 0, j1 = ردیفها;

    uchar* داده = m.ptr();
    لكل( صحيح i = 0; i < ردیفها; i++ )
    {
        إذا( !LtoR ) j1 = i; وإلا j0 = i+1;
        لكل( صحيح j = j0; j < j1; j++ )
            کپی_حافظه(داده + (i*گام + j*esz), داده + (j*گام + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(درون_داد_Array _m) ثابت
{
    Mat m = _m.getMat();
    صحيح tp = type(), d = CV_MAT_عمق(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((ردیفها == 3 && تعداد_ستون == 1) || (تعداد_ستون*channels() == 3 && ردیفها == 1)));
    Mat نتیجه(ردیفها, تعداد_ستون, tp);

    إذا( d == CV_32F )
    {
        ثابت عائم *a = (ثابت عائم*)داده, *b = (ثابت عائم*)m.داده;
        عائم* c = (عائم*)نتیجه.داده;
        size_t lda = ردیفها > 1 ? گام/حجم(a[0]) : 1;
        size_t ldb = ردیفها > 1 ? m.گام/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    وإلا إذا( d == CV_64F )
    {
        ثابت مزدوج *a = (ثابت مزدوج*)داده, *b = (ثابت مزدوج*)m.داده;
        مزدوج* c = (مزدوج*)نتیجه.داده;
        size_t lda = ردیفها > 1 ? گام/حجم(a[0]) : 1;
        size_t ldb = ردیفها > 1 ? m.گام/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    أرجع نتیجه;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

نطاق cv
{

تعريفنوع فراغ (*Reduce_جمع_Func)(ثابت Mat& src, Mat& dst);
Reduce_جمع_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جمع_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جمع_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth);

قالب <نام_نوع T, نام_نوع WT, نام_نوع Op>
بنية ReduceR_SIMD
{
    صحيح عملگر()(ثابت T*, صحيح شروع, صحيح, WT*, ثابت Op&) ثابت
    {
        أرجع شروع;
    }
};


قالب<نام_نوع T, نام_نوع ST, نام_نوع WT, صنف Op, صنف OpInit>
صنف ReduceR_Invoker : عام ParallelLoopBody
{
عام:
  ReduceR_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),میانگیر(srcmat.size().عرض*srcmat.channels())
  {
  }
  فراغ عملگر()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت T* src = srcmat.ptr<T>();
    ثابت size_t srcstep = srcmat.گام/حجم(src[0]);
    WT* buf = میانگیر.داده();
    ST* dst = dstmat.ptr<ST>();
    صحيح i = 0;

    لكل( i = range.شروع ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    صحيح ارتفاع = srcmat.size().ارتفاع;
    لكل( ; --ارتفاع; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.شروع, range.end, buf, op);
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
        #پایان_اگر
        لكل( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    لكل( i = range.شروع ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
خاص:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  تغییرپذیر Auto_میانگیر<WT> میانگیر;
};

قالب<نام_نوع T, نام_نوع ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceR_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع نام_نوع Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().عرض*srcmat.channels()), body, srcmat.size().عرض*CV_ELEM_اندازه(srcmat.عمق())/64);
}

قالب<نام_نوع T, نام_نوع ST, نام_نوع WT, صنف Op, صنف OpInit>
صنف ReduceC_Invoker : عام ParallelLoopBody
{
عام:
  ReduceC_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  فراغ عملگر()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح عرض = srcmat.size().عرض*cn;
    Auto_میانگیر<WT> cumul(cn);
    لكل( صحيح y = range.شروع; y < range.end; y++ )
    {
        ثابت T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        إذا( عرض == cn )
        {
          لكل( صحيح k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        وإلا
        {
            لكل(صحيح k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            لكل(صحيح k = cn ; k < عرض ; k += cn )
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

قالب<نام_نوع T, نام_نوع ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceC_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع نام_نوع Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().ارتفاع), body);
}

تعريفنوع فراغ (*ReduceFunc)( ثابت Mat& src, Mat& dst );

}

#تعریف_کن reduce_جمع_R8u32s  reduceR_<uchar, صحيح,   Op_جمع_کن<صحيح>, OpNop<صحيح> >
#تعریف_کن reduce_جمع_R8u32f  reduceR_<uchar, عائم, Op_جمع_کن<صحيح>, OpNop<صحيح> >
#تعریف_کن reduce_جمع_R8u64f  reduceR_<uchar, مزدوج,Op_جمع_کن<صحيح>, OpNop<صحيح> >
#تعریف_کن reduce_جمع_R16u32f reduceR_<ushort,عائم, Op_جمع_کن<عائم> >
#تعریف_کن reduce_جمع_R16u64f reduceR_<ushort,مزدوج,Op_جمع_کن<مزدوج> >
#تعریف_کن reduce_جمع_R16s32f reduceR_<کوتاه, عائم, Op_جمع_کن<عائم> >
#تعریف_کن reduce_جمع_R16s64f reduceR_<کوتاه, مزدوج,Op_جمع_کن<مزدوج> >
#تعریف_کن reduce_جمع_R32f32f reduceR_<عائم, عائم, Op_جمع_کن<عائم> >
#تعریف_کن reduce_جمع_R32f64f reduceR_<عائم, مزدوج,Op_جمع_کن<مزدوج> >
#تعریف_کن reduce_جمع_R64f64f reduceR_<مزدوج,مزدوج,Op_جمع_کن<مزدوج> >

#تعریف_کن reduceSum2R8u32s  reduceR_<uchar, صحيح,   Op_جمع_کن_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کن reduceSum2R8u32f  reduceR_<uchar, عائم, Op_جمع_کن_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کن reduceSum2R8u64f  reduceR_<uchar, مزدوج,Op_جمع_کن_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کن reduceSum2R16u32f reduceR_<ushort,عائم, Op_جمع_کن_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کن reduceSum2R16u64f reduceR_<ushort,مزدوج,Op_جمع_کن_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کن reduceSum2R16s32f reduceR_<کوتاه, عائم, Op_جمع_کن_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کن reduceSum2R16s64f reduceR_<کوتاه, مزدوج,Op_جمع_کن_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کن reduceSum2R32f32f reduceR_<عائم, عائم, Op_جمع_کن_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کن reduceSum2R32f64f reduceR_<عائم, مزدوج,Op_جمع_کن_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کن reduceSum2R64f64f reduceR_<مزدوج,مزدوج,Op_جمع_کن_Sqr<مزدوج>,OpSqr<مزدوج> >

#تعریف_کن reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#تعریف_کن reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#تعریف_کن reduceMaxR16s reduceR_<کوتاه, کوتاه, OpMax<کوتاه> >
#تعریف_کن reduceMaxR32f reduceR_<عائم, عائم, OpMax<عائم> >
#تعریف_کن reduceMaxR64f reduceR_<مزدوج,مزدوج,OpMax<مزدوج> >

#تعریف_کن reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#تعریف_کن reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#تعریف_کن reduceMinR16s reduceR_<کوتاه, کوتاه, OpMin<کوتاه> >
#تعریف_کن reduceMinR32f reduceR_<عائم, عائم, OpMin<عائم> >
#تعریف_کن reduceMinR64f reduceR_<مزدوج,مزدوج,OpMin<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ساكن مضمن منطقي ipp_reduce_جمع_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    صحيح sstep = (صحيح)srcmat.گام, stype = srcmat.type(),
            ddepth = dstmat.عمق();

    Ippi_اندازه roisize = { srcmat.size().عرض, 1 };

    تعريفنوع Ipp_وضعیت (CV_STDCALL * Ippi_جمع)(ثابت فراغ * pSrc, صحيح src_گام_3, Ippi_اندازه roi_اندازه, Ipp64f* p_جمع);
    تعريفنوع Ipp_وضعیت (CV_STDCALL * Ippi_جمع_Hint)(ثابت فراغ * pSrc, صحيح src_گام_3, Ippi_اندازه roi_اندازه, Ipp64f* p_جمع, IppHintAlgorithm hint);
    Ippi_جمع ippi_جمع = 0;
    Ippi_جمع_Hint ippi_جمع_Hint = 0;

    إذا(ddepth == CV_64F)
    {
        ippi_جمع =
            stype == CV_8UC1 ? (Ippi_جمع)ippi_جمع_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_جمع)ippi_جمع_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_جمع)ippi_جمع_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_جمع)ippi_جمع_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_جمع)ippi_جمع_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_جمع)ippi_جمع_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_جمع)ippi_جمع_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_جمع)ippi_جمع_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_جمع)ippi_جمع_16s_C4_R : 0;
        ippi_جمع_Hint =
            stype == CV_32FC1 ? (Ippi_جمع_Hint)ippi_جمع_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_جمع_Hint)ippi_جمع_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_جمع_Hint)ippi_جمع_32f_C4_R : 0;
    }

    إذا(ippi_جمع)
    {
        لكل(صحيح y = 0; y < srcmat.size().ارتفاع; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippi_جمع, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                أرجع خطأ;
        }
        أرجع درست;
    }
    وإلا إذا(ippi_جمع_Hint)
    {
        لكل(صحيح y = 0; y < srcmat.size().ارتفاع; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippi_جمع_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                أرجع خطأ;
        }
        أرجع درست;
    }

    أرجع خطأ;
}

ساكن مضمن فراغ reduce_جمع_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_پیاپی_FAST(ipp_reduce_جمع_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    إذا(dstmat.عمق() == CV_64F)
    {
        صحيح sdepth = CV_MAT_عمق(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, مزدوج,   cv::Op_جمع_کن<مزدوج> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, مزدوج,   cv::Op_جمع_کن<مزدوج> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<کوتاه, مزدوج,   cv::Op_جمع_کن<مزدوج> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<عائم, مزدوج,   cv::Op_جمع_کن<مزدوج> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#پایان_اگر

#تعریف_کن reduce_جمع_C8u32s  reduceC_<uchar, صحيح,   Op_جمع_کن<صحيح>, OpNop<صحيح> >
#تعریف_کن reduce_جمع_C8u32f  reduceC_<uchar, عائم, Op_جمع_کن<صحيح>, OpNop<صحيح> >
#تعریف_کن reduce_جمع_C16u32f reduceC_<ushort,عائم, Op_جمع_کن<عائم> >
#تعریف_کن reduce_جمع_C16s32f reduceC_<کوتاه, عائم, Op_جمع_کن<عائم> >
#تعریف_کن reduce_جمع_C32f32f reduceC_<عائم, عائم, Op_جمع_کن<عائم> >
#تعریف_کن reduce_جمع_C64f64f reduceC_<مزدوج,مزدوج,Op_جمع_کن<مزدوج> >

#تعریف_کن reduceSum2C8u32s  reduceC_<uchar, صحيح,   Op_جمع_کن_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کن reduceSum2C8u32f  reduceC_<uchar, عائم, Op_جمع_کن_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کن reduceSum2C16u32f reduceC_<ushort,عائم, Op_جمع_کن_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کن reduceSum2C16s32f reduceC_<کوتاه, عائم, Op_جمع_کن_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کن reduceSum2C32f32f reduceC_<عائم, عائم, Op_جمع_کن_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کن reduceSum2C64f64f reduceC_<مزدوج,مزدوج,Op_جمع_کن_Sqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#تعریف_کن reduce_جمع_C8u64f  reduce_جمع_C_8u16u16s32f_64f
#تعریف_کن reduce_جمع_C16u64f reduce_جمع_C_8u16u16s32f_64f
#تعریف_کن reduce_جمع_C16s64f reduce_جمع_C_8u16u16s32f_64f
#تعریف_کن reduce_جمع_C32f64f reduce_جمع_C_8u16u16s32f_64f
#وإلا
#تعریف_کن reduce_جمع_C8u64f  reduceC_<uchar, مزدوج,Op_جمع_کن<صحيح>, OpNop<صحيح> >
#تعریف_کن reduce_جمع_C16u64f reduceC_<ushort,مزدوج,Op_جمع_کن<مزدوج> >
#تعریف_کن reduce_جمع_C16s64f reduceC_<کوتاه, مزدوج,Op_جمع_کن<مزدوج> >
#تعریف_کن reduce_جمع_C32f64f reduceC_<عائم, مزدوج,Op_جمع_کن<مزدوج> >
#پایان_اگر

#تعریف_کن reduceSum2C8u64f  reduceC_<uchar, مزدوج,Op_جمع_کن_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کن reduceSum2C16u64f reduceC_<ushort,مزدوج,Op_جمع_کن_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کن reduceSum2C16s64f reduceC_<کوتاه, مزدوج,Op_جمع_کن_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کن reduceSum2C32f64f reduceC_<عائم, مزدوج,Op_جمع_کن_Sqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#تعریف_کن REDUCE_OP(favor, optype, type1, type2) \
ساكن مضمن منطقي ipp_reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    إذا((srcmat.channels() == 1)) \
    { \
        صحيح sstep = (صحيح)srcmat.گام; \
        تعريفنوع Ipp##favor IppType; \
        Ippi_اندازه roisize = ippi_اندازه(srcmat.size().عرض, 1);\
        لكل(صحيح y = 0; y < srcmat.size().ارتفاع; y++)\
        {\
            إذا(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                أرجع خطأ;\
        }\
        أرجع درست;\
    }\
    أرجع خطأ; \
} \
ساكن مضمن فراغ reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_پیاپی_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#پایان_اگر

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, کوتاه, کوتاه)
REDUCE_OP(32f, Max, عائم, عائم)
#وإلا
#تعریف_کن reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#تعریف_کن reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#تعریف_کن reduceMaxC16s reduceC_<کوتاه, کوتاه, OpMax<کوتاه> >
#تعریف_کن reduceMaxC32f reduceC_<عائم, عائم, OpMax<عائم> >
#پایان_اگر
#تعریف_کن reduceMaxC64f reduceC_<مزدوج,مزدوج,OpMax<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, کوتاه, کوتاه)
REDUCE_OP(32f, Min, عائم, عائم)
#وإلا
#تعریف_کن reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#تعریف_کن reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#تعریف_کن reduceMinC16s reduceC_<کوتاه, کوتاه, OpMin<کوتاه> >
#تعریف_کن reduceMinC32f reduceC_<عائم, عائم, OpMin<عائم> >
#پایان_اگر
#تعریف_کن reduceMinC64f reduceC_<مزدوج,مزدوج,OpMin<مزدوج> >

#اگر_تعریف_شده HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_reduce(درون_داد_Array _src, OutputArray _dst,
                       صحيح dim, صحيح op, صحيح op0, صحيح stype, صحيح dtype)
{
    ثابت صحيح min_opt_تعداد_ستون = 128, buf_تعداد_ستون = 32;
    صحيح sdepth = CV_MAT_عمق(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_عمق(dtype), ddepth0 = ddepth;
    ثابت ocl::Device &defDev = ocl::Device::getDefault();
    منطقي doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_گروه_اندازه();
    منطقي useOptimized = 1 == dim && _src.تعداد_ستون() > min_opt_تعداد_ستون && (wgs >= buf_تعداد_ستون);

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
        size_t tile_ارتفاع = (size_t)(wgs / buf_تعداد_ستون);
        إذا (defDev.isIntel())
        {
            ساكن ثابت size_t max_قلم_درون_گروه_شمار = 16;
            tile_ارتفاع = min(tile_ارتفاع, defDev.local_Mem_اندازه() / buf_تعداد_ستون / CV_ELEM_اندازه(CV_MAKETYPE(wdepth, cn)) / max_قلم_درون_گروه_شمار);
        }
        حرف cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_تعداد_ستون, tile_ارتفاع, ops[op], cn, ddepth,
                                            ocl::type_مقصد_Str(sdepth),
                                            ocl::type_مقصد_Str(ddepth),
                                            ocl::type_مقصد_Str(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], حجم(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], حجم(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        إذا (k.empty())
            أرجع خطأ;
        UMat src = _src.getUMat();
        اندازه dsize(1, src.ردیفها);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        إذا (op0 == REDUCE_AVG)
            k.آرگومانها(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_اندازه(dst), 1.0f / src.تعداد_ستون);
        وإلا
            k.آرگومانها(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_اندازه(dst));

        size_t local_اندازه[2] = { (size_t)buf_تعداد_ستون, (size_t)tile_ارتفاع};
        size_t global_اندازه[2] = { (size_t)buf_تعداد_ستون, (size_t)src.ردیفها };
        أرجع k.پیاپی(2, global_اندازه, local_اندازه, خطأ);
    }
    وإلا
    {
        حرف cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::type_مقصد_Str(useOptimized ? ddepth : sdepth),
                                      ocl::type_مقصد_Str(ddepth), ocl::type_مقصد_Str(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], حجم(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], حجم(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        إذا (k.empty())
            أرجع خطأ;

        UMat src = _src.getUMat();
        اندازه dsize(dim == 0 ? src.تعداد_ستون : 1, dim == 0 ? 1 : src.ردیفها);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_اندازه(dst);

        إذا (op0 == REDUCE_AVG)
            k.آرگومانها(srcarg, temparg, 1.0f / (dim == 0 ? src.ردیفها : src.تعداد_ستون));
        وإلا
            k.آرگومانها(srcarg, temparg);

        size_t globalsize = std::max(dsize.عرض, dsize.ارتفاع);
        أرجع k.پیاپی(1, &globalsize, عدم, خطأ);
    }
}

}

#پایان_اگر

فراغ cv::reduce(درون_داد_Array _src, OutputArray _dst, صحيح dim, صحيح op, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    إذا (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, درست, dtype < 0 && _dst.fixedType() ? _dst.عمق() : dtype));
        أرجع;
    }
    صحيح op0 = op;
    صحيح stype = _src.type(), sdepth = CV_MAT_عمق(stype), cn = CV_MAT_CN(stype);
    إذا( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    صحيح ddepth = CV_MAT_عمق(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_جمع || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_پیاپی(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    إذا (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    إذا (src.empty()) {
        src.convert_مقصد(_dst, dtype);
        أرجع;
    }
    إذا (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.ردیفها, dim == 0 ? src.تعداد_ستون : 1, dtype);
    Mat dst = _dst.getMat(), موقت_2 = dst;

    إذا( op == REDUCE_AVG )
    {
        op = REDUCE_جمع;
        إذا( sdepth < CV_32S && ddepth < CV_32S )
        {
            موقت_2.create(dst.ردیفها, dst.تعداد_ستون, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    إذا( dim == 0 )
    {
        إذا( op == REDUCE_جمع )
        {
            Reduce_جمع_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_جمع_R8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_جمع_R8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_جمع_R8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_جمع_R16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_جمع_R16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_جمع_R16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_جمع_R16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_جمع_R32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_جمع_R32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_جمع_R64f64f;
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
            Reduce_جمع_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
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
        إذا(op == REDUCE_جمع)
        {
            Reduce_جمع_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_جمع_C8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_جمع_C8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_جمع_C8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_جمع_C16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_جمع_C16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_جمع_C16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_جمع_C16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_جمع_C32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_جمع_C32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_جمع_C64f64f;
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

    func( src, موقت_2 );

    إذا( op0 == REDUCE_AVG )
        موقت_2.convert_مقصد(dst, dst.type(), 1./(dim == 0 ? src.ردیفها : src.تعداد_ستون));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

نطاق cv
{

قالب<نام_نوع T> ساكن فراغ مرتب_سازی( ثابت Mat& src, Mat& dst, صحيح flags )
{
    صحيح n, len;
    منطقي مرتب_سازی_ردیفها = (flags & 1) == مرتب_سازی_EVERY_ردیف;
    منطقي inplace = src.داده == dst.داده;
    منطقي مرتب_سازی_Descending = (flags & مرتب_سازی_DESCENDING) != 0;

    إذا( مرتب_سازی_ردیفها )
        n = src.ردیفها, len = src.تعداد_ستون;
    وإلا
        n = src.تعداد_ستون, len = src.ردیفها;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_میانگیر<T> buf;
        إذا( !مرتب_سازی_ردیفها )
            buf.allocate(len);
        T* bptr = buf.داده();

        لكل( صحيح i = range.شروع; i < range.end; i++ )
        {
            T* ptr = bptr;
            إذا( مرتب_سازی_ردیفها )
            {
                T* dptr = dst.ptr<T>(i);
                إذا( !inplace )
                {
                    ثابت T* sptr = src.ptr<T>(i);
                    کپی_حافظه(dptr, sptr, حجم(T) * len);
                }
                ptr = dptr;
            }
            وإلا
            {
                لكل( صحيح j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            إذا( مرتب_سازی_Descending )
            {
                لكل( صحيح j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            إذا( !مرتب_سازی_ردیفها )
                لكل( صحيح j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#إذا defined(HAVE_IPP) && !IPP_DISABLE_مرتب_سازی
تعريفنوع Ipp_وضعیت (CV_STDCALL *Ipp_مرتب_سازی_Func)(فراغ  *pSrcDst, صحيح    len, Ipp8u *p_میانگیر);

قالب<نام_نوع T, Ipp_وضعیت (CV_STDCALL *fn)(T*, صحيح, Ipp8u*)>
ساكن Ipp_وضعیت CV_STDCALL ipp_مرتب_سازی_Wrap(فراغ* pSrcDst, صحيح len, Ipp8u* p_میانگیر)
{
    أرجع fn((T*)pSrcDst, len, p_میانگیر);
}

ساكن Ipp_مرتب_سازی_Func get_مرتب_سازی_Func(صحيح عمق, منطقي مرتب_سازی_Descending)
{
    إذا (!مرتب_سازی_Descending)
        أرجع عمق == CV_8U ? ipp_مرتب_سازی_Wrap<Ipp8u, ipps_مرتب_سازی_Radix_Ascend_8u_I> :
            عمق == CV_16U ? ipp_مرتب_سازی_Wrap<Ipp16u, ipps_مرتب_سازی_Radix_Ascend_16u_I> :
            عمق == CV_16S ? ipp_مرتب_سازی_Wrap<Ipp16s, ipps_مرتب_سازی_Radix_Ascend_16s_I> :
            عمق == CV_32S ? ipp_مرتب_سازی_Wrap<Ipp32s, ipps_مرتب_سازی_Radix_Ascend_32s_I> :
            عمق == CV_32F ? ipp_مرتب_سازی_Wrap<Ipp32f, ipps_مرتب_سازی_Radix_Ascend_32f_I> :
            عمق == CV_64F ? ipp_مرتب_سازی_Wrap<Ipp64f, ipps_مرتب_سازی_Radix_Ascend_64f_I> :
            0;
    وإلا
        أرجع عمق == CV_8U ? ipp_مرتب_سازی_Wrap<Ipp8u, ipps_مرتب_سازی_Radix_Descend_8u_I> :
            عمق == CV_16U ? ipp_مرتب_سازی_Wrap<Ipp16u, ipps_مرتب_سازی_Radix_Descend_16u_I> :
            عمق == CV_16S ? ipp_مرتب_سازی_Wrap<Ipp16s, ipps_مرتب_سازی_Radix_Descend_16s_I> :
            عمق == CV_32S ? ipp_مرتب_سازی_Wrap<Ipp32s, ipps_مرتب_سازی_Radix_Descend_32s_I> :
            عمق == CV_32F ? ipp_مرتب_سازی_Wrap<Ipp32f, ipps_مرتب_سازی_Radix_Descend_32f_I> :
            عمق == CV_64F ? ipp_مرتب_سازی_Wrap<Ipp64f, ipps_مرتب_سازی_Radix_Descend_64f_I> :
            0;
}

ساكن منطقي ipp_مرتب_سازی(ثابت Mat& src, Mat& dst, صحيح flags)
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        مرتب_سازی_ردیفها        = (flags & 1) == مرتب_سازی_EVERY_ردیف;
    منطقي        مرتب_سازی_Descending  = (flags & مرتب_سازی_DESCENDING) != 0;
    منطقي        inplace         = (src.داده == dst.داده);
    صحيح         عمق           = src.عمق();
    Ipp_داده_Type type            = ippi_Get_داده_Type(عمق);

    Ipp_مرتب_سازی_Func ipps_مرتب_سازی_Radix_I = get_مرتب_سازی_Func(عمق, مرتب_سازی_Descending);
    إذا(!ipps_مرتب_سازی_Radix_I)
        أرجع خطأ;

    صحيح n, len;
    إذا( مرتب_سازی_ردیفها )
        n = src.ردیفها, len = src.تعداد_ستون;
    وإلا
        n = src.تعداد_ستون, len = src.ردیفها;

    صحيح میانگیر_اندازه;
    إذا(ipps_مرتب_سازی_Radix_Get_میانگیر_اندازه(len, type, &میانگیر_اندازه) < 0)
        أرجع خطأ;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    متطاير منطقي ok = درست;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_میانگیر<Ipp8u> میانگیر(میانگیر_اندازه);
        Mat  ردیف;
        Rect subRect(0,0,1,src.ردیفها);
        إذا( !مرتب_سازی_ردیفها )
            ردیف.create(1, src.ردیفها, src.type());

        لكل( صحيح i = range.شروع; i < range.end; i++ )
        {
            إذا( !ok )
                اكسر;
            فراغ* ptr;
            إذا( مرتب_سازی_ردیفها )
            {
                uchar* dptr = dst.ptr(i);
                إذا( !inplace )
                    کپی_حافظه(dptr, src.ptr(i), src.تعداد_ستون * src.elem_اندازه());
                ptr = dptr;
            }
            وإلا
            {
                subRect.x = i;
                Mat(src, subRect).copy_مقصد(ردیف);
                ptr = ردیف.ptr();
            }

            إذا( CV_INSTRUMENT_FUN_IPP(ipps_مرتب_سازی_Radix_I, ptr, len, میانگیر.داده()) < 0 )
            {
                ok = خطأ;
                اكسر;
            }

            إذا( !مرتب_سازی_ردیفها )
            {
                Mat dstSub(dst, subRect);
                ردیف = ردیف.reshape(1, dstSub.ردیفها);
                ردیف.copy_مقصد(dstSub);
                ردیف = ردیف.reshape(1, 1);
            }
        }
    });

    أرجع ok;
}
#پایان_اگر

قالب<نام_نوع _Tp> صنف LessThanIdx
{
عام:
    LessThanIdx( ثابت _Tp* _arr ) : arr(_arr) {}
    منطقي عملگر()(صحيح a, صحيح b) ثابت { أرجع arr[a] < arr[b]; }
    ثابت _Tp* arr;
};

قالب<نام_نوع T> ساكن فراغ مرتب_سازی_Idx_2( ثابت Mat& src, Mat& dst, صحيح flags )
{
    Auto_میانگیر<T> buf;
    Auto_میانگیر<صحيح> ibuf;
    منطقي مرتب_سازی_ردیفها = (flags & 1) == مرتب_سازی_EVERY_ردیف;
    منطقي مرتب_سازی_Descending = (flags & مرتب_سازی_DESCENDING) != 0;

    CV_Assert( src.داده != dst.داده );

    صحيح n, len;
    إذا( مرتب_سازی_ردیفها )
        n = src.ردیفها, len = src.تعداد_ستون;
    وإلا
    {
        n = src.تعداد_ستون, len = src.ردیفها;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.داده();
    صحيح* _iptr = ibuf.داده();

    لكل( صحيح i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        صحيح* iptr = _iptr;

        إذا( مرتب_سازی_ردیفها )
        {
            ptr = (T*)(src.داده + src.گام*i);
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
        إذا( مرتب_سازی_Descending )
        {
            لكل( صحيح j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        إذا( !مرتب_سازی_ردیفها )
            لكل( صحيح j = 0; j < len; j++ )
                dst.ptr<صحيح>(j)[i] = iptr[j];
    }
}

#إذا defined(HAVE_IPP) && !IPP_DISABLE_مرتب_سازی
تعريفنوع Ipp_وضعیت (CV_STDCALL *Ipp_مرتب_سازی_شاخص_Func)(ثابت فراغ*  pSrc, Ipp32s src_Stride_بایتها, Ipp32s *pDstIndx, صحيح len, Ipp8u *p_میانگیر);

قالب<نام_نوع T, Ipp_وضعیت (CV_STDCALL *fn)(ثابت T*, Ipp32s, Ipp32s*, صحيح, Ipp8u*)>
ساكن Ipp_وضعیت CV_STDCALL ipp_مرتب_سازی_شاخص_Wrap(ثابت فراغ* pSrc, Ipp32s src_Stride_بایتها, Ipp32s *pDstIndx, صحيح len, Ipp8u *p_میانگیر)
{
    أرجع fn((ثابت T*)pSrc, src_Stride_بایتها, pDstIndx, len, p_میانگیر);
}

ساكن Ipp_مرتب_سازی_شاخص_Func get_مرتب_سازی_شاخص_Func(صحيح عمق, منطقي مرتب_سازی_Descending)
{
    إذا (!مرتب_سازی_Descending)
        أرجع عمق == CV_8U ? ipp_مرتب_سازی_شاخص_Wrap<Ipp8u, ipps_مرتب_سازی_Radix_شاخص_Ascend_8u> :
            عمق == CV_16U ? ipp_مرتب_سازی_شاخص_Wrap<Ipp16u, ipps_مرتب_سازی_Radix_شاخص_Ascend_16u> :
            عمق == CV_16S ? ipp_مرتب_سازی_شاخص_Wrap<Ipp16s, ipps_مرتب_سازی_Radix_شاخص_Ascend_16s> :
            عمق == CV_32S ? ipp_مرتب_سازی_شاخص_Wrap<Ipp32s, ipps_مرتب_سازی_Radix_شاخص_Ascend_32s> :
            عمق == CV_32F ? ipp_مرتب_سازی_شاخص_Wrap<Ipp32f, ipps_مرتب_سازی_Radix_شاخص_Ascend_32f> :
            0;
    وإلا
        أرجع عمق == CV_8U ? ipp_مرتب_سازی_شاخص_Wrap<Ipp8u, ipps_مرتب_سازی_Radix_شاخص_Descend_8u> :
            عمق == CV_16U ? ipp_مرتب_سازی_شاخص_Wrap<Ipp16u, ipps_مرتب_سازی_Radix_شاخص_Descend_16u> :
            عمق == CV_16S ? ipp_مرتب_سازی_شاخص_Wrap<Ipp16s, ipps_مرتب_سازی_Radix_شاخص_Descend_16s> :
            عمق == CV_32S ? ipp_مرتب_سازی_شاخص_Wrap<Ipp32s, ipps_مرتب_سازی_Radix_شاخص_Descend_32s> :
            عمق == CV_32F ? ipp_مرتب_سازی_شاخص_Wrap<Ipp32f, ipps_مرتب_سازی_Radix_شاخص_Descend_32f> :
            0;
}

ساكن منطقي ipp_مرتب_سازی_Idx( ثابت Mat& src, Mat& dst, صحيح flags )
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        مرتب_سازی_ردیفها        = (flags & 1) == مرتب_سازی_EVERY_ردیف;
    منطقي        مرتب_سازی_Descending  = (flags & مرتب_سازی_DESCENDING) != 0;
    صحيح         عمق           = src.عمق();
    Ipp_داده_Type type            = ippi_Get_داده_Type(عمق);

    Ipp_مرتب_سازی_شاخص_Func ipps_مرتب_سازی_Radix_شاخص = get_مرتب_سازی_شاخص_Func(عمق, مرتب_سازی_Descending);
    إذا(!ipps_مرتب_سازی_Radix_شاخص)
        أرجع خطأ;

    إذا(مرتب_سازی_ردیفها)
    {
        Auto_میانگیر<Ipp8u> میانگیر;
        صحيح               میانگیر_اندازه;
        إذا(ipps_مرتب_سازی_Radix_شاخص_Get_میانگیر_اندازه(src.تعداد_ستون, type, &میانگیر_اندازه) < 0)
            أرجع خطأ;

        میانگیر.allocate(میانگیر_اندازه);

        لكل(صحيح i = 0; i < src.ردیفها; i++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ipps_مرتب_سازی_Radix_شاخص, (ثابت فراغ*)src.ptr(i), (Ipp32s)src.گام[1], (Ipp32s*)dst.ptr(i), src.تعداد_ستون, میانگیر.داده()) < 0)
                أرجع خطأ;
        }
    }
    وإلا
    {
        Mat  dst_ردیف(1, dst.ردیفها, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.ردیفها);

        Auto_میانگیر<Ipp8u> میانگیر;
        صحيح               میانگیر_اندازه;
        إذا(ipps_مرتب_سازی_Radix_شاخص_Get_میانگیر_اندازه(src.ردیفها, type, &میانگیر_اندازه) < 0)
            أرجع خطأ;

        میانگیر.allocate(میانگیر_اندازه);

        Ipp32s src_گام_3 = (Ipp32s)src.گام[0];
        لكل(صحيح i = 0; i < src.تعداد_ستون; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            إذا(CV_INSTRUMENT_FUN_IPP(ipps_مرتب_سازی_Radix_شاخص, (ثابت فراغ*)src.ptr(0, i), src_گام_3, (Ipp32s*)dst_ردیف.ptr(), src.ردیفها, میانگیر.داده()) < 0)
                أرجع خطأ;

            dst_ردیف = dst_ردیف.reshape(1, dstSub.ردیفها);
            dst_ردیف.copy_مقصد(dstSub);
        }
    }

    أرجع درست;
}
#پایان_اگر

تعريفنوع فراغ (*مرتب_سازی_Func)(ثابت Mat& src, Mat& dst, صحيح flags);
}

فراغ cv::sort( درون_داد_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_اندازه( src, src.type() );
    Mat dst = _dst.getMat();
#إذا !IPP_DISABLE_مرتب_سازی
    CV_IPP_پیاپی_FAST(ipp_مرتب_سازی(src, dst, flags));
#پایان_اگر

    ساكن مرتب_سازی_Func tab[CV_عمق_MAX] =
    {
        مرتب_سازی<uchar>, مرتب_سازی<schar>, مرتب_سازی<ushort>, مرتب_سازی<کوتاه>,
        مرتب_سازی<صحيح>, مرتب_سازی<عائم>, مرتب_سازی<مزدوج>, 0
    };
    مرتب_سازی_Func func = tab[src.عمق()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

فراغ cv::مرتب_سازی_Idx( درون_داد_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    إذا( dst.داده == src.داده )
        _dst.release();
    _dst.create_Same_اندازه( src, CV_32S );
    dst = _dst.getMat();

#إذا !IPP_DISABLE_مرتب_سازی
    CV_IPP_پیاپی_FAST(ipp_مرتب_سازی_Idx(src, dst, flags));
#پایان_اگر

    ساكن مرتب_سازی_Func tab[CV_عمق_MAX] =
    {
        مرتب_سازی_Idx_2<uchar>, مرتب_سازی_Idx_2<schar>, مرتب_سازی_Idx_2<ushort>, مرتب_سازی_Idx_2<کوتاه>,
        مرتب_سازی_Idx_2<صحيح>, مرتب_سازی_Idx_2<عائم>, مرتب_سازی_Idx_2<مزدوج>, 0
    };
    مرتب_سازی_Func func = tab[src.عمق()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
