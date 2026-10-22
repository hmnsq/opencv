// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"
#ضمن "opencl_kernels_core.hpp"

#وصف_ڪريو IPP_DISABLE_REDUCE 1
#وصف_ڪريو IPP_DISABLE_ترتيب_ڏيو   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

فراغ cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.صفون, b.صفون);
    std::swap(a.کانن_ڳڻپ, b.کانن_ڳڻپ);
    std::swap(a.معلومات, b.معلومات);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.قدم, b.قدم);
}


فراغ cv::hconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح ڪل_کانن_ڳڻپ = 0, کانن_ڳڻپ = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].صفون == src[0].صفون &&
                   src[i].type() == src[0].type());
        ڪل_کانن_ڳڻپ += src[i].کانن_ڳڻپ;
    }
    _dst.create( src[0].صفون, ڪل_کانن_ڳڻپ, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(کانن_ڳڻپ, 0, src[i].کانن_ڳڻپ, src[i].صفون));
        src[i].copy_منزل(dpart);
        کانن_ڳڻپ += src[i].کانن_ڳڻپ;
    }
}

فراغ cv::hconcat(داخلا_Array src1, داخلا_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

فراغ cv::hconcat(داخلا_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ويڪٽر(src);
    hconcat(!src.empty() ? &src[0] : خالي_اشارو, src.size(), dst);
}

فراغ cv::vconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح ڪل_صفون = 0, صفون = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].کانن_ڳڻپ == src[0].کانن_ڳڻپ &&
                  src[i].type() == src[0].type());
        ڪل_صفون += src[i].صفون;
    }
    _dst.create( ڪل_صفون, src[0].کانن_ڳڻپ, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, صفون, src[i].کانن_ڳڻپ, src[i].صفون));
        src[i].copy_منزل(dpart);
        صفون += src[i].صفون;
    }
}

فراغ cv::vconcat(داخلا_Array src1, داخلا_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

فراغ cv::vconcat(داخلا_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ويڪٽر(src);
    vconcat(!src.empty() ? &src[0] : خالي_اشارو, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_سيٽ_سڃاڻپ_عنصر( داخلا_Output_Array _m, ثابت Scalar& s )
{
    صحيح type = _m.type(), اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type), kercn = cn, صفون_في_WI = 1;
    صحيح sctype = CV_MAKE_TYPE(اونهائي, cn == 3 ? 4 : cn);
    إذا (ocl::Device::getDefault().isIntel())
    {
        صفون_في_WI = 4;
        إذا (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_ويڪٽر_ويڪر(_m), 4);
            إذا (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::سيٽ_سڃاڻپ_عنصر_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_منزل_Str(CV_MAKE_TYPE(اونهائي, kercn)),
                         ocl::memop_Type_منزل_Str(اونهائي), cn,
                         ocl::memop_Type_منزل_Str(sctype),
                         kercn, صفون_في_WI));
    إذا (k.empty())
        أرجع خطأ;

    UMat m = _m.getUMat();
    k.دليلون(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.کانن_ڳڻپ * cn / kercn, ((size_t)m.صفون + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, خالي_اشارو, خطأ);
}

}

#جيڪڏهن_پورو

فراغ cv::سيٽ_سڃاڻپ_عنصر( داخلا_Output_Array _m, ثابت Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_لاڳيتو(_m.isUMat(),
               ocl_سيٽ_سڃاڻپ_عنصر(_m, s))

    Mat m = _m.getMat();
    صحيح صفون = m.صفون, کانن_ڳڻپ = m.کانن_ڳڻپ, type = m.type();

    إذا( type == CV_32FC1 )
    {
        عائم* معلومات = m.ptr<عائم>();
        عائم val = (عائم)s[0];
        size_t قدم = m.قدم/حجم(معلومات[0]);

        لكل( صحيح i = 0; i < صفون; i++, معلومات += قدم )
        {
            لكل( صحيح j = 0; j < کانن_ڳڻپ; j++ )
                معلومات[j] = 0;
            إذا( i < کانن_ڳڻپ )
                معلومات[i] = val;
        }
    }
    وإلا إذا( type == CV_64FC1 )
    {
        مزدوج* معلومات = m.ptr<مزدوج>();
        مزدوج val = s[0];
        size_t قدم = m.قدم/حجم(معلومات[0]);

        لكل( صحيح i = 0; i < صفون; i++, معلومات += قدم )
        {
            std::fill(معلومات, معلومات + کانن_ڳڻپ, 0.0);
            إذا (i < کانن_ڳڻپ)
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

UMat UMat::eye(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat::eye(سائيز(کانن_ڳڻپ, صفون), type, usageFlags);
}

UMat UMat::eye(سائيز size, صحيح type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    سيٽ_سڃاڻپ_عنصر(m);
    أرجع m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( داخلا_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    صحيح type = m.type();
    صحيح nm = std::min(m.صفون, m.کانن_ڳڻپ);

    إذا( type == CV_32FC1 )
    {
        ثابت عائم* ptr = m.ptr<عائم>();
        size_t قدم = m.قدم/حجم(ptr[0]) + 1;
        مزدوج _s = 0;
        لكل( صحيح i = 0; i < nm; i++ )
            _s += ptr[i*قدم];
        أرجع _s;
    }

    إذا( type == CV_64FC1 )
    {
        ثابت مزدوج* ptr = m.ptr<مزدوج>();
        size_t قدم = m.قدم/حجم(ptr[0]) + 1;
        مزدوج _s = 0;
        لكل( صحيح i = 0; i < nm; i++ )
            _s += ptr[i*قدم];
        أرجع _s;
    }

    أرجع cv::جوڙ(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

فراغ cv::completeSymm( داخلا_Output_Array _m, منطقي LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t قدم = m.قدم, esz = m.elem_سائيز();
    CV_Assert( m.dims <= 2 && m.صفون == m.کانن_ڳڻپ );

    صحيح صفون = m.صفون;
    صحيح j0 = 0, j1 = صفون;

    uchar* معلومات = m.ptr();
    لكل( صحيح i = 0; i < صفون; i++ )
    {
        إذا( !LtoR ) j1 = i; وإلا j0 = i+1;
        لكل( صحيح j = j0; j < j1; j++ )
            يادگيري_نقل(معلومات + (i*قدم + j*esz), معلومات + (j*قدم + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(داخلا_Array _m) ثابت
{
    Mat m = _m.getMat();
    صحيح tp = type(), d = CV_MAT_اونهائي(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((صفون == 3 && کانن_ڳڻپ == 1) || (کانن_ڳڻپ*channels() == 3 && صفون == 1)));
    Mat نتيجو(صفون, کانن_ڳڻپ, tp);

    إذا( d == CV_32F )
    {
        ثابت عائم *a = (ثابت عائم*)معلومات, *b = (ثابت عائم*)m.معلومات;
        عائم* c = (عائم*)نتيجو.معلومات;
        size_t lda = صفون > 1 ? قدم/حجم(a[0]) : 1;
        size_t ldb = صفون > 1 ? m.قدم/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    وإلا إذا( d == CV_64F )
    {
        ثابت مزدوج *a = (ثابت مزدوج*)معلومات, *b = (ثابت مزدوج*)m.معلومات;
        مزدوج* c = (مزدوج*)نتيجو.معلومات;
        size_t lda = صفون > 1 ? قدم/حجم(a[0]) : 1;
        size_t ldb = صفون > 1 ? m.قدم/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    أرجع نتيجو;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

نطاق cv
{

تعريفنوع فراغ (*Reduce_جوڙ_Func)(ثابت Mat& src, Mat& dst);
Reduce_جوڙ_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جوڙ_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جوڙ_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth);

سانچو <قسم_نالو T, قسم_نالو WT, قسم_نالو Op>
بنية ReduceR_SIMD
{
    صحيح عامل()(ثابت T*, صحيح شروعات, صحيح, WT*, ثابت Op&) ثابت
    {
        أرجع شروعات;
    }
};


سانچو<قسم_نالو T, قسم_نالو ST, قسم_نالو WT, صنف Op, صنف OpInit>
صنف ReduceR_Invoker : عام ParallelLoopBody
{
عام:
  ReduceR_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),بفر(srcmat.size().ويڪر*srcmat.channels())
  {
  }
  فراغ عامل()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت T* src = srcmat.ptr<T>();
    ثابت size_t srcstep = srcmat.قدم/حجم(src[0]);
    WT* buf = بفر.معلومات();
    ST* dst = dstmat.ptr<ST>();
    صحيح i = 0;

    لكل( i = range.شروعات ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    صحيح اوچائي = srcmat.size().اوچائي;
    لكل( ; --اوچائي; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.شروعات, range.end, buf, op);
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
        #جيڪڏهن_پورو
        لكل( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    لكل( i = range.شروعات ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
خاص:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  بدلجندڙ Auto_بفر<WT> بفر;
};

سانچو<قسم_نالو T, قسم_نالو ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceR_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع قسم_نالو Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().ويڪر*srcmat.channels()), body, srcmat.size().ويڪر*CV_ELEM_سائيز(srcmat.اونهائي())/64);
}

سانچو<قسم_نالو T, قسم_نالو ST, قسم_نالو WT, صنف Op, صنف OpInit>
صنف ReduceC_Invoker : عام ParallelLoopBody
{
عام:
  ReduceC_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  فراغ عامل()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت صحيح cn = srcmat.channels();
    ثابت صحيح ويڪر = srcmat.size().ويڪر*cn;
    Auto_بفر<WT> cumul(cn);
    لكل( صحيح y = range.شروعات; y < range.end; y++ )
    {
        ثابت T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        إذا( ويڪر == cn )
        {
          لكل( صحيح k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        وإلا
        {
            لكل(صحيح k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            لكل(صحيح k = cn ; k < ويڪر ; k += cn )
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

سانچو<قسم_نالو T, قسم_نالو ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceC_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع قسم_نالو Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().اوچائي), body);
}

تعريفنوع فراغ (*ReduceFunc)( ثابت Mat& src, Mat& dst );

}

#وصف_ڪريو reduce_جوڙ_R8u32s  reduceR_<uchar, صحيح,   Op_جوڙيو<صحيح>, OpNop<صحيح> >
#وصف_ڪريو reduce_جوڙ_R8u32f  reduceR_<uchar, عائم, Op_جوڙيو<صحيح>, OpNop<صحيح> >
#وصف_ڪريو reduce_جوڙ_R8u64f  reduceR_<uchar, مزدوج,Op_جوڙيو<صحيح>, OpNop<صحيح> >
#وصف_ڪريو reduce_جوڙ_R16u32f reduceR_<ushort,عائم, Op_جوڙيو<عائم> >
#وصف_ڪريو reduce_جوڙ_R16u64f reduceR_<ushort,مزدوج,Op_جوڙيو<مزدوج> >
#وصف_ڪريو reduce_جوڙ_R16s32f reduceR_<ننڍو, عائم, Op_جوڙيو<عائم> >
#وصف_ڪريو reduce_جوڙ_R16s64f reduceR_<ننڍو, مزدوج,Op_جوڙيو<مزدوج> >
#وصف_ڪريو reduce_جوڙ_R32f32f reduceR_<عائم, عائم, Op_جوڙيو<عائم> >
#وصف_ڪريو reduce_جوڙ_R32f64f reduceR_<عائم, مزدوج,Op_جوڙيو<مزدوج> >
#وصف_ڪريو reduce_جوڙ_R64f64f reduceR_<مزدوج,مزدوج,Op_جوڙيو<مزدوج> >

#وصف_ڪريو reduceSum2R8u32s  reduceR_<uchar, صحيح,   Op_جوڙيو_Sqr<صحيح>,   OpSqr<صحيح> >
#وصف_ڪريو reduceSum2R8u32f  reduceR_<uchar, عائم, Op_جوڙيو_Sqr<صحيح>,   OpSqr<صحيح> >
#وصف_ڪريو reduceSum2R8u64f  reduceR_<uchar, مزدوج,Op_جوڙيو_Sqr<صحيح>,   OpSqr<صحيح> >
#وصف_ڪريو reduceSum2R16u32f reduceR_<ushort,عائم, Op_جوڙيو_Sqr<عائم>, OpSqr<عائم> >
#وصف_ڪريو reduceSum2R16u64f reduceR_<ushort,مزدوج,Op_جوڙيو_Sqr<مزدوج>,OpSqr<مزدوج> >
#وصف_ڪريو reduceSum2R16s32f reduceR_<ننڍو, عائم, Op_جوڙيو_Sqr<عائم>, OpSqr<عائم> >
#وصف_ڪريو reduceSum2R16s64f reduceR_<ننڍو, مزدوج,Op_جوڙيو_Sqr<مزدوج>,OpSqr<مزدوج> >
#وصف_ڪريو reduceSum2R32f32f reduceR_<عائم, عائم, Op_جوڙيو_Sqr<عائم>, OpSqr<عائم> >
#وصف_ڪريو reduceSum2R32f64f reduceR_<عائم, مزدوج,Op_جوڙيو_Sqr<مزدوج>,OpSqr<مزدوج> >
#وصف_ڪريو reduceSum2R64f64f reduceR_<مزدوج,مزدوج,Op_جوڙيو_Sqr<مزدوج>,OpSqr<مزدوج> >

#وصف_ڪريو reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#وصف_ڪريو reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#وصف_ڪريو reduceMaxR16s reduceR_<ننڍو, ننڍو, OpMax<ننڍو> >
#وصف_ڪريو reduceMaxR32f reduceR_<عائم, عائم, OpMax<عائم> >
#وصف_ڪريو reduceMaxR64f reduceR_<مزدوج,مزدوج,OpMax<مزدوج> >

#وصف_ڪريو reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#وصف_ڪريو reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#وصف_ڪريو reduceMinR16s reduceR_<ننڍو, ننڍو, OpMin<ننڍو> >
#وصف_ڪريو reduceMinR32f reduceR_<عائم, عائم, OpMin<عائم> >
#وصف_ڪريو reduceMinR64f reduceR_<مزدوج,مزدوج,OpMin<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ساكن مضمن منطقي ipp_reduce_جوڙ_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    صحيح sstep = (صحيح)srcmat.قدم, stype = srcmat.type(),
            ddepth = dstmat.اونهائي();

    Ippi_سائيز roisize = { srcmat.size().ويڪر, 1 };

    تعريفنوع Ipp_حالت (CV_STDCALL * Ippi_جوڙ)(ثابت فراغ * pSrc, صحيح src_قدم_3, Ippi_سائيز roi_سائيز, Ipp64f* p_جوڙ);
    تعريفنوع Ipp_حالت (CV_STDCALL * Ippi_جوڙ_Hint)(ثابت فراغ * pSrc, صحيح src_قدم_3, Ippi_سائيز roi_سائيز, Ipp64f* p_جوڙ, IppHintAlgorithm hint);
    Ippi_جوڙ ippi_جوڙ = 0;
    Ippi_جوڙ_Hint ippi_جوڙ_Hint = 0;

    إذا(ddepth == CV_64F)
    {
        ippi_جوڙ =
            stype == CV_8UC1 ? (Ippi_جوڙ)ippi_جوڙ_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_جوڙ)ippi_جوڙ_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_جوڙ)ippi_جوڙ_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_جوڙ)ippi_جوڙ_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_جوڙ)ippi_جوڙ_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_جوڙ)ippi_جوڙ_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_جوڙ)ippi_جوڙ_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_جوڙ)ippi_جوڙ_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_جوڙ)ippi_جوڙ_16s_C4_R : 0;
        ippi_جوڙ_Hint =
            stype == CV_32FC1 ? (Ippi_جوڙ_Hint)ippi_جوڙ_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_جوڙ_Hint)ippi_جوڙ_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_جوڙ_Hint)ippi_جوڙ_32f_C4_R : 0;
    }

    إذا(ippi_جوڙ)
    {
        لكل(صحيح y = 0; y < srcmat.size().اوچائي; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippi_جوڙ, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
    وإلا إذا(ippi_جوڙ_Hint)
    {
        لكل(صحيح y = 0; y < srcmat.size().اوچائي; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippi_جوڙ_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }

    أرجع خطأ;
}

ساكن مضمن فراغ reduce_جوڙ_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_لاڳيتو_FAST(ipp_reduce_جوڙ_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    إذا(dstmat.اونهائي() == CV_64F)
    {
        صحيح sdepth = CV_MAT_اونهائي(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, مزدوج,   cv::Op_جوڙيو<مزدوج> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, مزدوج,   cv::Op_جوڙيو<مزدوج> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<ننڍو, مزدوج,   cv::Op_جوڙيو<مزدوج> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<عائم, مزدوج,   cv::Op_جوڙيو<مزدوج> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#جيڪڏهن_پورو

#وصف_ڪريو reduce_جوڙ_C8u32s  reduceC_<uchar, صحيح,   Op_جوڙيو<صحيح>, OpNop<صحيح> >
#وصف_ڪريو reduce_جوڙ_C8u32f  reduceC_<uchar, عائم, Op_جوڙيو<صحيح>, OpNop<صحيح> >
#وصف_ڪريو reduce_جوڙ_C16u32f reduceC_<ushort,عائم, Op_جوڙيو<عائم> >
#وصف_ڪريو reduce_جوڙ_C16s32f reduceC_<ننڍو, عائم, Op_جوڙيو<عائم> >
#وصف_ڪريو reduce_جوڙ_C32f32f reduceC_<عائم, عائم, Op_جوڙيو<عائم> >
#وصف_ڪريو reduce_جوڙ_C64f64f reduceC_<مزدوج,مزدوج,Op_جوڙيو<مزدوج> >

#وصف_ڪريو reduceSum2C8u32s  reduceC_<uchar, صحيح,   Op_جوڙيو_Sqr<صحيح>,   OpSqr<صحيح> >
#وصف_ڪريو reduceSum2C8u32f  reduceC_<uchar, عائم, Op_جوڙيو_Sqr<صحيح>,   OpSqr<صحيح> >
#وصف_ڪريو reduceSum2C16u32f reduceC_<ushort,عائم, Op_جوڙيو_Sqr<عائم>, OpSqr<عائم> >
#وصف_ڪريو reduceSum2C16s32f reduceC_<ننڍو, عائم, Op_جوڙيو_Sqr<عائم>, OpSqr<عائم> >
#وصف_ڪريو reduceSum2C32f32f reduceC_<عائم, عائم, Op_جوڙيو_Sqr<عائم>, OpSqr<عائم> >
#وصف_ڪريو reduceSum2C64f64f reduceC_<مزدوج,مزدوج,Op_جوڙيو_Sqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#وصف_ڪريو reduce_جوڙ_C8u64f  reduce_جوڙ_C_8u16u16s32f_64f
#وصف_ڪريو reduce_جوڙ_C16u64f reduce_جوڙ_C_8u16u16s32f_64f
#وصف_ڪريو reduce_جوڙ_C16s64f reduce_جوڙ_C_8u16u16s32f_64f
#وصف_ڪريو reduce_جوڙ_C32f64f reduce_جوڙ_C_8u16u16s32f_64f
#وإلا
#وصف_ڪريو reduce_جوڙ_C8u64f  reduceC_<uchar, مزدوج,Op_جوڙيو<صحيح>, OpNop<صحيح> >
#وصف_ڪريو reduce_جوڙ_C16u64f reduceC_<ushort,مزدوج,Op_جوڙيو<مزدوج> >
#وصف_ڪريو reduce_جوڙ_C16s64f reduceC_<ننڍو, مزدوج,Op_جوڙيو<مزدوج> >
#وصف_ڪريو reduce_جوڙ_C32f64f reduceC_<عائم, مزدوج,Op_جوڙيو<مزدوج> >
#جيڪڏهن_پورو

#وصف_ڪريو reduceSum2C8u64f  reduceC_<uchar, مزدوج,Op_جوڙيو_Sqr<صحيح>,   OpSqr<صحيح> >
#وصف_ڪريو reduceSum2C16u64f reduceC_<ushort,مزدوج,Op_جوڙيو_Sqr<مزدوج>,OpSqr<مزدوج> >
#وصف_ڪريو reduceSum2C16s64f reduceC_<ننڍو, مزدوج,Op_جوڙيو_Sqr<مزدوج>,OpSqr<مزدوج> >
#وصف_ڪريو reduceSum2C32f64f reduceC_<عائم, مزدوج,Op_جوڙيو_Sqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#وصف_ڪريو REDUCE_OP(favor, optype, type1, type2) \
ساكن مضمن منطقي ipp_reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    إذا((srcmat.channels() == 1)) \
    { \
        صحيح sstep = (صحيح)srcmat.قدم; \
        تعريفنوع Ipp##favor IppType; \
        Ippi_سائيز roisize = ippi_سائيز(srcmat.size().ويڪر, 1);\
        لكل(صحيح y = 0; y < srcmat.size().اوچائي; y++)\
        {\
            إذا(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                أرجع خطأ;\
        }\
        أرجع سچ;\
    }\
    أرجع خطأ; \
} \
ساكن مضمن فراغ reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_لاڳيتو_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#جيڪڏهن_پورو

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, ننڍو, ننڍو)
REDUCE_OP(32f, Max, عائم, عائم)
#وإلا
#وصف_ڪريو reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#وصف_ڪريو reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#وصف_ڪريو reduceMaxC16s reduceC_<ننڍو, ننڍو, OpMax<ننڍو> >
#وصف_ڪريو reduceMaxC32f reduceC_<عائم, عائم, OpMax<عائم> >
#جيڪڏهن_پورو
#وصف_ڪريو reduceMaxC64f reduceC_<مزدوج,مزدوج,OpMax<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, ننڍو, ننڍو)
REDUCE_OP(32f, Min, عائم, عائم)
#وإلا
#وصف_ڪريو reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#وصف_ڪريو reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#وصف_ڪريو reduceMinC16s reduceC_<ننڍو, ننڍو, OpMin<ننڍو> >
#وصف_ڪريو reduceMinC32f reduceC_<عائم, عائم, OpMin<عائم> >
#جيڪڏهن_پورو
#وصف_ڪريو reduceMinC64f reduceC_<مزدوج,مزدوج,OpMin<مزدوج> >

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_reduce(داخلا_Array _src, OutputArray _dst,
                       صحيح dim, صحيح op, صحيح op0, صحيح stype, صحيح dtype)
{
    ثابت صحيح min_opt_کانن_ڳڻپ = 128, buf_کانن_ڳڻپ = 32;
    صحيح sdepth = CV_MAT_اونهائي(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_اونهائي(dtype), ddepth0 = ddepth;
    ثابت ocl::Device &defDev = ocl::Device::getDefault();
    منطقي doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_گروهه_سائيز();
    منطقي useOptimized = 1 == dim && _src.کانن_ڳڻپ() > min_opt_کانن_ڳڻپ && (wgs >= buf_کانن_ڳڻپ);

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
        size_t tile_اوچائي = (size_t)(wgs / buf_کانن_ڳڻپ);
        إذا (defDev.isIntel())
        {
            ساكن ثابت size_t max_شي_اندريون_گروهه_ڳڻپ = 16;
            tile_اوچائي = min(tile_اوچائي, defDev.local_Mem_سائيز() / buf_کانن_ڳڻپ / CV_ELEM_سائيز(CV_MAKETYPE(wdepth, cn)) / max_شي_اندريون_گروهه_ڳڻپ);
        }
        حرف cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_کانن_ڳڻپ, tile_اوچائي, ops[op], cn, ddepth,
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
        سائيز dsize(1, src.صفون);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        إذا (op0 == REDUCE_AVG)
            k.دليلون(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_سائيز(dst), 1.0f / src.کانن_ڳڻپ);
        وإلا
            k.دليلون(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_سائيز(dst));

        size_t local_سائيز[2] = { (size_t)buf_کانن_ڳڻپ, (size_t)tile_اوچائي};
        size_t global_سائيز[2] = { (size_t)buf_کانن_ڳڻپ, (size_t)src.صفون };
        أرجع k.لاڳيتو(2, global_سائيز, local_سائيز, خطأ);
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
        سائيز dsize(dim == 0 ? src.کانن_ڳڻپ : 1, dim == 0 ? 1 : src.صفون);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_سائيز(dst);

        إذا (op0 == REDUCE_AVG)
            k.دليلون(srcarg, temparg, 1.0f / (dim == 0 ? src.صفون : src.کانن_ڳڻپ));
        وإلا
            k.دليلون(srcarg, temparg);

        size_t globalsize = std::max(dsize.ويڪر, dsize.اوچائي);
        أرجع k.لاڳيتو(1, &globalsize, عدم, خطأ);
    }
}

}

#جيڪڏهن_پورو

فراغ cv::reduce(داخلا_Array _src, OutputArray _dst, صحيح dim, صحيح op, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    إذا (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, سچ, dtype < 0 && _dst.fixedType() ? _dst.اونهائي() : dtype));
        أرجع;
    }
    صحيح op0 = op;
    صحيح stype = _src.type(), sdepth = CV_MAT_اونهائي(stype), cn = CV_MAT_CN(stype);
    إذا( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    صحيح ddepth = CV_MAT_اونهائي(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_جوڙ || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_لاڳيتو(_dst.isUMat(),
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

    _dst.create(dim == 0 ? 1 : src.صفون, dim == 0 ? src.کانن_ڳڻپ : 1, dtype);
    Mat dst = _dst.getMat(), عارضي_2 = dst;

    إذا( op == REDUCE_AVG )
    {
        op = REDUCE_جوڙ;
        إذا( sdepth < CV_32S && ddepth < CV_32S )
        {
            عارضي_2.create(dst.صفون, dst.کانن_ڳڻپ, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    إذا( dim == 0 )
    {
        إذا( op == REDUCE_جوڙ )
        {
            Reduce_جوڙ_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_جوڙ_R8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_جوڙ_R8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_جوڙ_R8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_جوڙ_R16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_جوڙ_R16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_جوڙ_R16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_جوڙ_R16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_جوڙ_R32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_جوڙ_R32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_جوڙ_R64f64f;
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
            Reduce_جوڙ_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
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
        إذا(op == REDUCE_جوڙ)
        {
            Reduce_جوڙ_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_جوڙ_C8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_جوڙ_C8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_جوڙ_C8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_جوڙ_C16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_جوڙ_C16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_جوڙ_C16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_جوڙ_C16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_جوڙ_C32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_جوڙ_C32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_جوڙ_C64f64f;
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

    func( src, عارضي_2 );

    إذا( op0 == REDUCE_AVG )
        عارضي_2.convert_منزل(dst, dst.type(), 1./(dim == 0 ? src.صفون : src.کانن_ڳڻپ));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

نطاق cv
{

سانچو<قسم_نالو T> ساكن فراغ ترتيب_ڏيو( ثابت Mat& src, Mat& dst, صحيح flags )
{
    صحيح n, len;
    منطقي ترتيب_ڏيو_صفون = (flags & 1) == ترتيب_ڏيو_EVERY_صف;
    منطقي inplace = src.معلومات == dst.معلومات;
    منطقي ترتيب_ڏيو_Descending = (flags & ترتيب_ڏيو_DESCENDING) != 0;

    إذا( ترتيب_ڏيو_صفون )
        n = src.صفون, len = src.کانن_ڳڻپ;
    وإلا
        n = src.کانن_ڳڻپ, len = src.صفون;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_بفر<T> buf;
        إذا( !ترتيب_ڏيو_صفون )
            buf.allocate(len);
        T* bptr = buf.معلومات();

        لكل( صحيح i = range.شروعات; i < range.end; i++ )
        {
            T* ptr = bptr;
            إذا( ترتيب_ڏيو_صفون )
            {
                T* dptr = dst.ptr<T>(i);
                إذا( !inplace )
                {
                    ثابت T* sptr = src.ptr<T>(i);
                    يادگيري_نقل(dptr, sptr, حجم(T) * len);
                }
                ptr = dptr;
            }
            وإلا
            {
                لكل( صحيح j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            إذا( ترتيب_ڏيو_Descending )
            {
                لكل( صحيح j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            إذا( !ترتيب_ڏيو_صفون )
                لكل( صحيح j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#إذا defined(HAVE_IPP) && !IPP_DISABLE_ترتيب_ڏيو
تعريفنوع Ipp_حالت (CV_STDCALL *Ipp_ترتيب_ڏيو_Func)(فراغ  *pSrcDst, صحيح    len, Ipp8u *p_بفر);

سانچو<قسم_نالو T, Ipp_حالت (CV_STDCALL *fn)(T*, صحيح, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ipp_ترتيب_ڏيو_Wrap(فراغ* pSrcDst, صحيح len, Ipp8u* p_بفر)
{
    أرجع fn((T*)pSrcDst, len, p_بفر);
}

ساكن Ipp_ترتيب_ڏيو_Func get_ترتيب_ڏيو_Func(صحيح اونهائي, منطقي ترتيب_ڏيو_Descending)
{
    إذا (!ترتيب_ڏيو_Descending)
        أرجع اونهائي == CV_8U ? ipp_ترتيب_ڏيو_Wrap<Ipp8u, ipps_ترتيب_ڏيو_Radix_Ascend_8u_I> :
            اونهائي == CV_16U ? ipp_ترتيب_ڏيو_Wrap<Ipp16u, ipps_ترتيب_ڏيو_Radix_Ascend_16u_I> :
            اونهائي == CV_16S ? ipp_ترتيب_ڏيو_Wrap<Ipp16s, ipps_ترتيب_ڏيو_Radix_Ascend_16s_I> :
            اونهائي == CV_32S ? ipp_ترتيب_ڏيو_Wrap<Ipp32s, ipps_ترتيب_ڏيو_Radix_Ascend_32s_I> :
            اونهائي == CV_32F ? ipp_ترتيب_ڏيو_Wrap<Ipp32f, ipps_ترتيب_ڏيو_Radix_Ascend_32f_I> :
            اونهائي == CV_64F ? ipp_ترتيب_ڏيو_Wrap<Ipp64f, ipps_ترتيب_ڏيو_Radix_Ascend_64f_I> :
            0;
    وإلا
        أرجع اونهائي == CV_8U ? ipp_ترتيب_ڏيو_Wrap<Ipp8u, ipps_ترتيب_ڏيو_Radix_Descend_8u_I> :
            اونهائي == CV_16U ? ipp_ترتيب_ڏيو_Wrap<Ipp16u, ipps_ترتيب_ڏيو_Radix_Descend_16u_I> :
            اونهائي == CV_16S ? ipp_ترتيب_ڏيو_Wrap<Ipp16s, ipps_ترتيب_ڏيو_Radix_Descend_16s_I> :
            اونهائي == CV_32S ? ipp_ترتيب_ڏيو_Wrap<Ipp32s, ipps_ترتيب_ڏيو_Radix_Descend_32s_I> :
            اونهائي == CV_32F ? ipp_ترتيب_ڏيو_Wrap<Ipp32f, ipps_ترتيب_ڏيو_Radix_Descend_32f_I> :
            اونهائي == CV_64F ? ipp_ترتيب_ڏيو_Wrap<Ipp64f, ipps_ترتيب_ڏيو_Radix_Descend_64f_I> :
            0;
}

ساكن منطقي ipp_ترتيب_ڏيو(ثابت Mat& src, Mat& dst, صحيح flags)
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        ترتيب_ڏيو_صفون        = (flags & 1) == ترتيب_ڏيو_EVERY_صف;
    منطقي        ترتيب_ڏيو_Descending  = (flags & ترتيب_ڏيو_DESCENDING) != 0;
    منطقي        inplace         = (src.معلومات == dst.معلومات);
    صحيح         اونهائي           = src.اونهائي();
    Ipp_معلومات_Type type            = ippi_Get_معلومات_Type(اونهائي);

    Ipp_ترتيب_ڏيو_Func ipps_ترتيب_ڏيو_Radix_I = get_ترتيب_ڏيو_Func(اونهائي, ترتيب_ڏيو_Descending);
    إذا(!ipps_ترتيب_ڏيو_Radix_I)
        أرجع خطأ;

    صحيح n, len;
    إذا( ترتيب_ڏيو_صفون )
        n = src.صفون, len = src.کانن_ڳڻپ;
    وإلا
        n = src.کانن_ڳڻپ, len = src.صفون;

    صحيح بفر_سائيز;
    إذا(ipps_ترتيب_ڏيو_Radix_Get_بفر_سائيز(len, type, &بفر_سائيز) < 0)
        أرجع خطأ;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    متطاير منطقي ok = سچ;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_بفر<Ipp8u> بفر(بفر_سائيز);
        Mat  صف;
        Rect subRect(0,0,1,src.صفون);
        إذا( !ترتيب_ڏيو_صفون )
            صف.create(1, src.صفون, src.type());

        لكل( صحيح i = range.شروعات; i < range.end; i++ )
        {
            إذا( !ok )
                اكسر;
            فراغ* ptr;
            إذا( ترتيب_ڏيو_صفون )
            {
                uchar* dptr = dst.ptr(i);
                إذا( !inplace )
                    يادگيري_نقل(dptr, src.ptr(i), src.کانن_ڳڻپ * src.elem_سائيز());
                ptr = dptr;
            }
            وإلا
            {
                subRect.x = i;
                Mat(src, subRect).copy_منزل(صف);
                ptr = صف.ptr();
            }

            إذا( CV_INSTRUMENT_FUN_IPP(ipps_ترتيب_ڏيو_Radix_I, ptr, len, بفر.معلومات()) < 0 )
            {
                ok = خطأ;
                اكسر;
            }

            إذا( !ترتيب_ڏيو_صفون )
            {
                Mat dstSub(dst, subRect);
                صف = صف.reshape(1, dstSub.صفون);
                صف.copy_منزل(dstSub);
                صف = صف.reshape(1, 1);
            }
        }
    });

    أرجع ok;
}
#جيڪڏهن_پورو

سانچو<قسم_نالو _Tp> صنف LessThanIdx
{
عام:
    LessThanIdx( ثابت _Tp* _arr ) : arr(_arr) {}
    منطقي عامل()(صحيح a, صحيح b) ثابت { أرجع arr[a] < arr[b]; }
    ثابت _Tp* arr;
};

سانچو<قسم_نالو T> ساكن فراغ ترتيب_ڏيو_Idx_2( ثابت Mat& src, Mat& dst, صحيح flags )
{
    Auto_بفر<T> buf;
    Auto_بفر<صحيح> ibuf;
    منطقي ترتيب_ڏيو_صفون = (flags & 1) == ترتيب_ڏيو_EVERY_صف;
    منطقي ترتيب_ڏيو_Descending = (flags & ترتيب_ڏيو_DESCENDING) != 0;

    CV_Assert( src.معلومات != dst.معلومات );

    صحيح n, len;
    إذا( ترتيب_ڏيو_صفون )
        n = src.صفون, len = src.کانن_ڳڻپ;
    وإلا
    {
        n = src.کانن_ڳڻپ, len = src.صفون;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.معلومات();
    صحيح* _iptr = ibuf.معلومات();

    لكل( صحيح i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        صحيح* iptr = _iptr;

        إذا( ترتيب_ڏيو_صفون )
        {
            ptr = (T*)(src.معلومات + src.قدم*i);
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
        إذا( ترتيب_ڏيو_Descending )
        {
            لكل( صحيح j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        إذا( !ترتيب_ڏيو_صفون )
            لكل( صحيح j = 0; j < len; j++ )
                dst.ptr<صحيح>(j)[i] = iptr[j];
    }
}

#إذا defined(HAVE_IPP) && !IPP_DISABLE_ترتيب_ڏيو
تعريفنوع Ipp_حالت (CV_STDCALL *Ipp_ترتيب_ڏيو_اشاريو_Func)(ثابت فراغ*  pSrc, Ipp32s src_Stride_بائيٽون, Ipp32s *pDstIndx, صحيح len, Ipp8u *p_بفر);

سانچو<قسم_نالو T, Ipp_حالت (CV_STDCALL *fn)(ثابت T*, Ipp32s, Ipp32s*, صحيح, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ipp_ترتيب_ڏيو_اشاريو_Wrap(ثابت فراغ* pSrc, Ipp32s src_Stride_بائيٽون, Ipp32s *pDstIndx, صحيح len, Ipp8u *p_بفر)
{
    أرجع fn((ثابت T*)pSrc, src_Stride_بائيٽون, pDstIndx, len, p_بفر);
}

ساكن Ipp_ترتيب_ڏيو_اشاريو_Func get_ترتيب_ڏيو_اشاريو_Func(صحيح اونهائي, منطقي ترتيب_ڏيو_Descending)
{
    إذا (!ترتيب_ڏيو_Descending)
        أرجع اونهائي == CV_8U ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp8u, ipps_ترتيب_ڏيو_Radix_اشاريو_Ascend_8u> :
            اونهائي == CV_16U ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp16u, ipps_ترتيب_ڏيو_Radix_اشاريو_Ascend_16u> :
            اونهائي == CV_16S ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp16s, ipps_ترتيب_ڏيو_Radix_اشاريو_Ascend_16s> :
            اونهائي == CV_32S ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp32s, ipps_ترتيب_ڏيو_Radix_اشاريو_Ascend_32s> :
            اونهائي == CV_32F ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp32f, ipps_ترتيب_ڏيو_Radix_اشاريو_Ascend_32f> :
            0;
    وإلا
        أرجع اونهائي == CV_8U ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp8u, ipps_ترتيب_ڏيو_Radix_اشاريو_Descend_8u> :
            اونهائي == CV_16U ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp16u, ipps_ترتيب_ڏيو_Radix_اشاريو_Descend_16u> :
            اونهائي == CV_16S ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp16s, ipps_ترتيب_ڏيو_Radix_اشاريو_Descend_16s> :
            اونهائي == CV_32S ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp32s, ipps_ترتيب_ڏيو_Radix_اشاريو_Descend_32s> :
            اونهائي == CV_32F ? ipp_ترتيب_ڏيو_اشاريو_Wrap<Ipp32f, ipps_ترتيب_ڏيو_Radix_اشاريو_Descend_32f> :
            0;
}

ساكن منطقي ipp_ترتيب_ڏيو_Idx( ثابت Mat& src, Mat& dst, صحيح flags )
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        ترتيب_ڏيو_صفون        = (flags & 1) == ترتيب_ڏيو_EVERY_صف;
    منطقي        ترتيب_ڏيو_Descending  = (flags & ترتيب_ڏيو_DESCENDING) != 0;
    صحيح         اونهائي           = src.اونهائي();
    Ipp_معلومات_Type type            = ippi_Get_معلومات_Type(اونهائي);

    Ipp_ترتيب_ڏيو_اشاريو_Func ipps_ترتيب_ڏيو_Radix_اشاريو = get_ترتيب_ڏيو_اشاريو_Func(اونهائي, ترتيب_ڏيو_Descending);
    إذا(!ipps_ترتيب_ڏيو_Radix_اشاريو)
        أرجع خطأ;

    إذا(ترتيب_ڏيو_صفون)
    {
        Auto_بفر<Ipp8u> بفر;
        صحيح               بفر_سائيز;
        إذا(ipps_ترتيب_ڏيو_Radix_اشاريو_Get_بفر_سائيز(src.کانن_ڳڻپ, type, &بفر_سائيز) < 0)
            أرجع خطأ;

        بفر.allocate(بفر_سائيز);

        لكل(صحيح i = 0; i < src.صفون; i++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ipps_ترتيب_ڏيو_Radix_اشاريو, (ثابت فراغ*)src.ptr(i), (Ipp32s)src.قدم[1], (Ipp32s*)dst.ptr(i), src.کانن_ڳڻپ, بفر.معلومات()) < 0)
                أرجع خطأ;
        }
    }
    وإلا
    {
        Mat  dst_صف(1, dst.صفون, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.صفون);

        Auto_بفر<Ipp8u> بفر;
        صحيح               بفر_سائيز;
        إذا(ipps_ترتيب_ڏيو_Radix_اشاريو_Get_بفر_سائيز(src.صفون, type, &بفر_سائيز) < 0)
            أرجع خطأ;

        بفر.allocate(بفر_سائيز);

        Ipp32s src_قدم_3 = (Ipp32s)src.قدم[0];
        لكل(صحيح i = 0; i < src.کانن_ڳڻپ; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            إذا(CV_INSTRUMENT_FUN_IPP(ipps_ترتيب_ڏيو_Radix_اشاريو, (ثابت فراغ*)src.ptr(0, i), src_قدم_3, (Ipp32s*)dst_صف.ptr(), src.صفون, بفر.معلومات()) < 0)
                أرجع خطأ;

            dst_صف = dst_صف.reshape(1, dstSub.صفون);
            dst_صف.copy_منزل(dstSub);
        }
    }

    أرجع سچ;
}
#جيڪڏهن_پورو

تعريفنوع فراغ (*ترتيب_ڏيو_Func)(ثابت Mat& src, Mat& dst, صحيح flags);
}

فراغ cv::sort( داخلا_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_سائيز( src, src.type() );
    Mat dst = _dst.getMat();
#إذا !IPP_DISABLE_ترتيب_ڏيو
    CV_IPP_لاڳيتو_FAST(ipp_ترتيب_ڏيو(src, dst, flags));
#جيڪڏهن_پورو

    ساكن ترتيب_ڏيو_Func tab[CV_اونهائي_MAX] =
    {
        ترتيب_ڏيو<uchar>, ترتيب_ڏيو<schar>, ترتيب_ڏيو<ushort>, ترتيب_ڏيو<ننڍو>,
        ترتيب_ڏيو<صحيح>, ترتيب_ڏيو<عائم>, ترتيب_ڏيو<مزدوج>, 0
    };
    ترتيب_ڏيو_Func func = tab[src.اونهائي()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

فراغ cv::ترتيب_ڏيو_Idx( داخلا_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    إذا( dst.معلومات == src.معلومات )
        _dst.release();
    _dst.create_Same_سائيز( src, CV_32S );
    dst = _dst.getMat();

#إذا !IPP_DISABLE_ترتيب_ڏيو
    CV_IPP_لاڳيتو_FAST(ipp_ترتيب_ڏيو_Idx(src, dst, flags));
#جيڪڏهن_پورو

    ساكن ترتيب_ڏيو_Func tab[CV_اونهائي_MAX] =
    {
        ترتيب_ڏيو_Idx_2<uchar>, ترتيب_ڏيو_Idx_2<schar>, ترتيب_ڏيو_Idx_2<ushort>, ترتيب_ڏيو_Idx_2<ننڍو>,
        ترتيب_ڏيو_Idx_2<صحيح>, ترتيب_ڏيو_Idx_2<عائم>, ترتيب_ڏيو_Idx_2<مزدوج>, 0
    };
    ترتيب_ڏيو_Func func = tab[src.اونهائي()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
