// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"
#ضمن "opencl_kernels_core.hpp"

#تعریف_کرو IPP_DISABLE_REDUCE 1
#تعریف_کرو IPP_DISABLE_ترتیب_دیو   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

فراغ cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.صفاں, b.صفاں);
    std::swap(a.خانے_گنتی, b.خانے_گنتی);
    std::swap(a.انکڑے, b.انکڑے);
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

    صحيح کل_خانے_گنتی = 0, خانے_گنتی = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].صفاں == src[0].صفاں &&
                   src[i].type() == src[0].type());
        کل_خانے_گنتی += src[i].خانے_گنتی;
    }
    _dst.create( src[0].صفاں, کل_خانے_گنتی, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(خانے_گنتی, 0, src[i].خانے_گنتی, src[i].صفاں));
        src[i].copy_منزل(dpart);
        خانے_گنتی += src[i].خانے_گنتی;
    }
}

فراغ cv::hconcat(داخلہ_Array src1, داخلہ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

فراغ cv::hconcat(داخلہ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_سمتیہ(src);
    hconcat(!src.empty() ? &src[0] : خالی_سوچک, src.size(), dst);
}

فراغ cv::vconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    إذا( nsrc == 0 || !src )
    {
        _dst.release();
        أرجع;
    }

    صحيح کل_صفاں = 0, صفاں = 0;
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].خانے_گنتی == src[0].خانے_گنتی &&
                  src[i].type() == src[0].type());
        کل_صفاں += src[i].صفاں;
    }
    _dst.create( کل_صفاں, src[0].خانے_گنتی, src[0].type());
    Mat dst = _dst.getMat();
    لكل( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, صفاں, src[i].خانے_گنتی, src[i].صفاں));
        src[i].copy_منزل(dpart);
        صفاں += src[i].صفاں;
    }
}

فراغ cv::vconcat(داخلہ_Array src1, داخلہ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

فراغ cv::vconcat(داخلہ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_سمتیہ(src);
    vconcat(!src.empty() ? &src[0] : خالی_سوچک, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#جے_تعریف_شدہ HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_سیٹ_پچھان_تت( داخلہ_Output_Array _m, ثابت Scalar& s )
{
    صحيح type = _m.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type), kercn = cn, صفاں_فی_WI = 1;
    صحيح sctype = CV_MAKE_TYPE(ڈونگھائی, cn == 3 ? 4 : cn);
    إذا (ocl::Device::getDefault().isIntel())
    {
        صفاں_فی_WI = 4;
        إذا (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_سمتیہ_چوڑائی(_m), 4);
            إذا (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::سیٹ_پچھان_تت_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, kercn)),
                         ocl::memop_Type_منزل_Str(ڈونگھائی), cn,
                         ocl::memop_Type_منزل_Str(sctype),
                         kercn, صفاں_فی_WI));
    إذا (k.empty())
        أرجع خطأ;

    UMat m = _m.getUMat();
    k.دلیلاں(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.خانے_گنتی * cn / kercn, ((size_t)m.صفاں + صفاں_فی_WI - 1) / صفاں_فی_WI };
    أرجع k.لگاتار(2, globalsize, خالی_سوچک, خطأ);
}

}

#جے_انت

فراغ cv::سیٹ_پچھان_تت( داخلہ_Output_Array _m, ثابت Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_لگاتار(_m.isUMat(),
               ocl_سیٹ_پچھان_تت(_m, s))

    Mat m = _m.getMat();
    صحيح صفاں = m.صفاں, خانے_گنتی = m.خانے_گنتی, type = m.type();

    إذا( type == CV_32FC1 )
    {
        عائم* انکڑے = m.ptr<عائم>();
        عائم val = (عائم)s[0];
        size_t قدم = m.قدم/حجم(انکڑے[0]);

        لكل( صحيح i = 0; i < صفاں; i++, انکڑے += قدم )
        {
            لكل( صحيح j = 0; j < خانے_گنتی; j++ )
                انکڑے[j] = 0;
            إذا( i < خانے_گنتی )
                انکڑے[i] = val;
        }
    }
    وإلا إذا( type == CV_64FC1 )
    {
        مزدوج* انکڑے = m.ptr<مزدوج>();
        مزدوج val = s[0];
        size_t قدم = m.قدم/حجم(انکڑے[0]);

        لكل( صحيح i = 0; i < صفاں; i++, انکڑے += قدم )
        {
            std::fill(انکڑے, انکڑے + خانے_گنتی, 0.0);
            إذا (i < خانے_گنتی)
                انکڑے[i] = val;
        }
    }
    وإلا
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


نطاق cv {

UMat UMat::eye(صحيح صفاں, صحيح خانے_گنتی, صحيح type, UMatUsageFlags usageFlags)
{
    أرجع UMat::eye(ناپ(خانے_گنتی, صفاں), type, usageFlags);
}

UMat UMat::eye(ناپ size, صحيح type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    سیٹ_پچھان_تت(m);
    أرجع m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( داخلہ_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    صحيح type = m.type();
    صحيح nm = std::min(m.صفاں, m.خانے_گنتی);

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

    أرجع cv::جوڑ(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

فراغ cv::completeSymm( داخلہ_Output_Array _m, منطقي LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t قدم = m.قدم, esz = m.elem_ناپ();
    CV_Assert( m.dims <= 2 && m.صفاں == m.خانے_گنتی );

    صحيح صفاں = m.صفاں;
    صحيح j0 = 0, j1 = صفاں;

    uchar* انکڑے = m.ptr();
    لكل( صحيح i = 0; i < صفاں; i++ )
    {
        إذا( !LtoR ) j1 = i; وإلا j0 = i+1;
        لكل( صحيح j = j0; j < j1; j++ )
            حافظہ_نقل(انکڑے + (i*قدم + j*esz), انکڑے + (j*قدم + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(داخلہ_Array _m) ثابت
{
    Mat m = _m.getMat();
    صحيح tp = type(), d = CV_MAT_ڈونگھائی(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((صفاں == 3 && خانے_گنتی == 1) || (خانے_گنتی*channels() == 3 && صفاں == 1)));
    Mat نتیجہ(صفاں, خانے_گنتی, tp);

    إذا( d == CV_32F )
    {
        ثابت عائم *a = (ثابت عائم*)انکڑے, *b = (ثابت عائم*)m.انکڑے;
        عائم* c = (عائم*)نتیجہ.انکڑے;
        size_t lda = صفاں > 1 ? قدم/حجم(a[0]) : 1;
        size_t ldb = صفاں > 1 ? m.قدم/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    وإلا إذا( d == CV_64F )
    {
        ثابت مزدوج *a = (ثابت مزدوج*)انکڑے, *b = (ثابت مزدوج*)m.انکڑے;
        مزدوج* c = (مزدوج*)نتیجہ.انکڑے;
        size_t lda = صفاں > 1 ? قدم/حجم(a[0]) : 1;
        size_t ldb = صفاں > 1 ? m.قدم/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    أرجع نتیجہ;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

نطاق cv
{

تعريفنوع فراغ (*Reduce_جوڑ_Func)(ثابت Mat& src, Mat& dst);
Reduce_جوڑ_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جوڑ_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جوڑ_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth);

سانچا <قسم_ناں T, قسم_ناں WT, قسم_ناں Op>
بنية ReduceR_SIMD
{
    صحيح عامل()(ثابت T*, صحيح شروع, صحيح, WT*, ثابت Op&) ثابت
    {
        أرجع شروع;
    }
};


سانچا<قسم_ناں T, قسم_ناں ST, قسم_ناں WT, صنف Op, صنف OpInit>
صنف ReduceR_Invoker : عام ParallelLoopBody
{
عام:
  ReduceR_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),بفر(srcmat.size().چوڑائی*srcmat.channels())
  {
  }
  فراغ عامل()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت T* src = srcmat.ptr<T>();
    ثابت size_t srcstep = srcmat.قدم/حجم(src[0]);
    WT* buf = بفر.انکڑے();
    ST* dst = dstmat.ptr<ST>();
    صحيح i = 0;

    لكل( i = range.شروع ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    صحيح اچائی = srcmat.size().اچائی;
    لكل( ; --اچائی; )
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
        #جے_انت
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
  بدلنجوگ Auto_بفر<WT> بفر;
};

سانچا<قسم_ناں T, قسم_ناں ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceR_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع قسم_ناں Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().چوڑائی*srcmat.channels()), body, srcmat.size().چوڑائی*CV_ELEM_ناپ(srcmat.ڈونگھائی())/64);
}

سانچا<قسم_ناں T, قسم_ناں ST, قسم_ناں WT, صنف Op, صنف OpInit>
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
    ثابت صحيح چوڑائی = srcmat.size().چوڑائی*cn;
    Auto_بفر<WT> cumul(cn);
    لكل( صحيح y = range.شروع; y < range.end; y++ )
    {
        ثابت T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        إذا( چوڑائی == cn )
        {
          لكل( صحيح k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        وإلا
        {
            لكل(صحيح k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            لكل(صحيح k = cn ; k < چوڑائی ; k += cn )
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

سانچا<قسم_ناں T, قسم_ناں ST, صنف Op, صنف OpInit = OpNop<ST> > ساكن فراغ
reduceC_( ثابت Mat& srcmat, Mat& dstmat)
{
    تعريفنوع قسم_ناں Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().اچائی), body);
}

تعريفنوع فراغ (*ReduceFunc)( ثابت Mat& src, Mat& dst );

}

#تعریف_کرو reduce_جوڑ_R8u32s  reduceR_<uchar, صحيح,   Op_جوڑو<صحيح>, OpNop<صحيح> >
#تعریف_کرو reduce_جوڑ_R8u32f  reduceR_<uchar, عائم, Op_جوڑو<صحيح>, OpNop<صحيح> >
#تعریف_کرو reduce_جوڑ_R8u64f  reduceR_<uchar, مزدوج,Op_جوڑو<صحيح>, OpNop<صحيح> >
#تعریف_کرو reduce_جوڑ_R16u32f reduceR_<ushort,عائم, Op_جوڑو<عائم> >
#تعریف_کرو reduce_جوڑ_R16u64f reduceR_<ushort,مزدوج,Op_جوڑو<مزدوج> >
#تعریف_کرو reduce_جوڑ_R16s32f reduceR_<چھوٹا, عائم, Op_جوڑو<عائم> >
#تعریف_کرو reduce_جوڑ_R16s64f reduceR_<چھوٹا, مزدوج,Op_جوڑو<مزدوج> >
#تعریف_کرو reduce_جوڑ_R32f32f reduceR_<عائم, عائم, Op_جوڑو<عائم> >
#تعریف_کرو reduce_جوڑ_R32f64f reduceR_<عائم, مزدوج,Op_جوڑو<مزدوج> >
#تعریف_کرو reduce_جوڑ_R64f64f reduceR_<مزدوج,مزدوج,Op_جوڑو<مزدوج> >

#تعریف_کرو reduceSum2R8u32s  reduceR_<uchar, صحيح,   Op_جوڑو_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کرو reduceSum2R8u32f  reduceR_<uchar, عائم, Op_جوڑو_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کرو reduceSum2R8u64f  reduceR_<uchar, مزدوج,Op_جوڑو_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کرو reduceSum2R16u32f reduceR_<ushort,عائم, Op_جوڑو_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کرو reduceSum2R16u64f reduceR_<ushort,مزدوج,Op_جوڑو_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کرو reduceSum2R16s32f reduceR_<چھوٹا, عائم, Op_جوڑو_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کرو reduceSum2R16s64f reduceR_<چھوٹا, مزدوج,Op_جوڑو_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کرو reduceSum2R32f32f reduceR_<عائم, عائم, Op_جوڑو_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کرو reduceSum2R32f64f reduceR_<عائم, مزدوج,Op_جوڑو_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کرو reduceSum2R64f64f reduceR_<مزدوج,مزدوج,Op_جوڑو_Sqr<مزدوج>,OpSqr<مزدوج> >

#تعریف_کرو reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#تعریف_کرو reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#تعریف_کرو reduceMaxR16s reduceR_<چھوٹا, چھوٹا, OpMax<چھوٹا> >
#تعریف_کرو reduceMaxR32f reduceR_<عائم, عائم, OpMax<عائم> >
#تعریف_کرو reduceMaxR64f reduceR_<مزدوج,مزدوج,OpMax<مزدوج> >

#تعریف_کرو reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#تعریف_کرو reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#تعریف_کرو reduceMinR16s reduceR_<چھوٹا, چھوٹا, OpMin<چھوٹا> >
#تعریف_کرو reduceMinR32f reduceR_<عائم, عائم, OpMin<عائم> >
#تعریف_کرو reduceMinR64f reduceR_<مزدوج,مزدوج,OpMin<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ساكن مضمن منطقي ipp_reduce_جوڑ_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    صحيح sstep = (صحيح)srcmat.قدم, stype = srcmat.type(),
            ddepth = dstmat.ڈونگھائی();

    Ippi_ناپ roisize = { srcmat.size().چوڑائی, 1 };

    تعريفنوع Ipp_حالت (CV_STDCALL * Ippi_جوڑ)(ثابت فراغ * pSrc, صحيح src_قدم_3, Ippi_ناپ roi_ناپ, Ipp64f* p_جوڑ);
    تعريفنوع Ipp_حالت (CV_STDCALL * Ippi_جوڑ_Hint)(ثابت فراغ * pSrc, صحيح src_قدم_3, Ippi_ناپ roi_ناپ, Ipp64f* p_جوڑ, IppHintAlgorithm hint);
    Ippi_جوڑ ippi_جوڑ = 0;
    Ippi_جوڑ_Hint ippi_جوڑ_Hint = 0;

    إذا(ddepth == CV_64F)
    {
        ippi_جوڑ =
            stype == CV_8UC1 ? (Ippi_جوڑ)ippi_جوڑ_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_جوڑ)ippi_جوڑ_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_جوڑ)ippi_جوڑ_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_جوڑ)ippi_جوڑ_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_جوڑ)ippi_جوڑ_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_جوڑ)ippi_جوڑ_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_جوڑ)ippi_جوڑ_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_جوڑ)ippi_جوڑ_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_جوڑ)ippi_جوڑ_16s_C4_R : 0;
        ippi_جوڑ_Hint =
            stype == CV_32FC1 ? (Ippi_جوڑ_Hint)ippi_جوڑ_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_جوڑ_Hint)ippi_جوڑ_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_جوڑ_Hint)ippi_جوڑ_32f_C4_R : 0;
    }

    إذا(ippi_جوڑ)
    {
        لكل(صحيح y = 0; y < srcmat.size().اچائی; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippi_جوڑ, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
    وإلا إذا(ippi_جوڑ_Hint)
    {
        لكل(صحيح y = 0; y < srcmat.size().اچائی; y++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ippi_جوڑ_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }

    أرجع خطأ;
}

ساكن مضمن فراغ reduce_جوڑ_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_لگاتار_FAST(ipp_reduce_جوڑ_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    إذا(dstmat.ڈونگھائی() == CV_64F)
    {
        صحيح sdepth = CV_MAT_ڈونگھائی(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, مزدوج,   cv::Op_جوڑو<مزدوج> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, مزدوج,   cv::Op_جوڑو<مزدوج> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<چھوٹا, مزدوج,   cv::Op_جوڑو<مزدوج> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<عائم, مزدوج,   cv::Op_جوڑو<مزدوج> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#جے_انت

#تعریف_کرو reduce_جوڑ_C8u32s  reduceC_<uchar, صحيح,   Op_جوڑو<صحيح>, OpNop<صحيح> >
#تعریف_کرو reduce_جوڑ_C8u32f  reduceC_<uchar, عائم, Op_جوڑو<صحيح>, OpNop<صحيح> >
#تعریف_کرو reduce_جوڑ_C16u32f reduceC_<ushort,عائم, Op_جوڑو<عائم> >
#تعریف_کرو reduce_جوڑ_C16s32f reduceC_<چھوٹا, عائم, Op_جوڑو<عائم> >
#تعریف_کرو reduce_جوڑ_C32f32f reduceC_<عائم, عائم, Op_جوڑو<عائم> >
#تعریف_کرو reduce_جوڑ_C64f64f reduceC_<مزدوج,مزدوج,Op_جوڑو<مزدوج> >

#تعریف_کرو reduceSum2C8u32s  reduceC_<uchar, صحيح,   Op_جوڑو_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کرو reduceSum2C8u32f  reduceC_<uchar, عائم, Op_جوڑو_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کرو reduceSum2C16u32f reduceC_<ushort,عائم, Op_جوڑو_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کرو reduceSum2C16s32f reduceC_<چھوٹا, عائم, Op_جوڑو_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کرو reduceSum2C32f32f reduceC_<عائم, عائم, Op_جوڑو_Sqr<عائم>, OpSqr<عائم> >
#تعریف_کرو reduceSum2C64f64f reduceC_<مزدوج,مزدوج,Op_جوڑو_Sqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#تعریف_کرو reduce_جوڑ_C8u64f  reduce_جوڑ_C_8u16u16s32f_64f
#تعریف_کرو reduce_جوڑ_C16u64f reduce_جوڑ_C_8u16u16s32f_64f
#تعریف_کرو reduce_جوڑ_C16s64f reduce_جوڑ_C_8u16u16s32f_64f
#تعریف_کرو reduce_جوڑ_C32f64f reduce_جوڑ_C_8u16u16s32f_64f
#وإلا
#تعریف_کرو reduce_جوڑ_C8u64f  reduceC_<uchar, مزدوج,Op_جوڑو<صحيح>, OpNop<صحيح> >
#تعریف_کرو reduce_جوڑ_C16u64f reduceC_<ushort,مزدوج,Op_جوڑو<مزدوج> >
#تعریف_کرو reduce_جوڑ_C16s64f reduceC_<چھوٹا, مزدوج,Op_جوڑو<مزدوج> >
#تعریف_کرو reduce_جوڑ_C32f64f reduceC_<عائم, مزدوج,Op_جوڑو<مزدوج> >
#جے_انت

#تعریف_کرو reduceSum2C8u64f  reduceC_<uchar, مزدوج,Op_جوڑو_Sqr<صحيح>,   OpSqr<صحيح> >
#تعریف_کرو reduceSum2C16u64f reduceC_<ushort,مزدوج,Op_جوڑو_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کرو reduceSum2C16s64f reduceC_<چھوٹا, مزدوج,Op_جوڑو_Sqr<مزدوج>,OpSqr<مزدوج> >
#تعریف_کرو reduceSum2C32f64f reduceC_<عائم, مزدوج,Op_جوڑو_Sqr<مزدوج>,OpSqr<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#تعریف_کرو REDUCE_OP(favor, optype, type1, type2) \
ساكن مضمن منطقي ipp_reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    إذا((srcmat.channels() == 1)) \
    { \
        صحيح sstep = (صحيح)srcmat.قدم; \
        تعريفنوع Ipp##favor IppType; \
        Ippi_ناپ roisize = ippi_ناپ(srcmat.size().چوڑائی, 1);\
        لكل(صحيح y = 0; y < srcmat.size().اچائی; y++)\
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
    CV_IPP_لگاتار_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#جے_انت

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, چھوٹا, چھوٹا)
REDUCE_OP(32f, Max, عائم, عائم)
#وإلا
#تعریف_کرو reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#تعریف_کرو reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#تعریف_کرو reduceMaxC16s reduceC_<چھوٹا, چھوٹا, OpMax<چھوٹا> >
#تعریف_کرو reduceMaxC32f reduceC_<عائم, عائم, OpMax<عائم> >
#جے_انت
#تعریف_کرو reduceMaxC64f reduceC_<مزدوج,مزدوج,OpMax<مزدوج> >

#إذا defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, چھوٹا, چھوٹا)
REDUCE_OP(32f, Min, عائم, عائم)
#وإلا
#تعریف_کرو reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#تعریف_کرو reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#تعریف_کرو reduceMinC16s reduceC_<چھوٹا, چھوٹا, OpMin<چھوٹا> >
#تعریف_کرو reduceMinC32f reduceC_<عائم, عائم, OpMin<عائم> >
#جے_انت
#تعریف_کرو reduceMinC64f reduceC_<مزدوج,مزدوج,OpMin<مزدوج> >

#جے_تعریف_شدہ HAVE_OPENCL

نطاق cv {

ساكن منطقي ocl_reduce(داخلہ_Array _src, OutputArray _dst,
                       صحيح dim, صحيح op, صحيح op0, صحيح stype, صحيح dtype)
{
    ثابت صحيح min_opt_خانے_گنتی = 128, buf_خانے_گنتی = 32;
    صحيح sdepth = CV_MAT_ڈونگھائی(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_ڈونگھائی(dtype), ddepth0 = ddepth;
    ثابت ocl::Device &defDev = ocl::Device::getDefault();
    منطقي doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_گروہ_ناپ();
    منطقي useOptimized = 1 == dim && _src.خانے_گنتی() > min_opt_خانے_گنتی && (wgs >= buf_خانے_گنتی);

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
        size_t tile_اچائی = (size_t)(wgs / buf_خانے_گنتی);
        إذا (defDev.isIntel())
        {
            ساكن ثابت size_t max_شے_وچ_گروہ_گنتی = 16;
            tile_اچائی = min(tile_اچائی, defDev.local_Mem_ناپ() / buf_خانے_گنتی / CV_ELEM_ناپ(CV_MAKETYPE(wdepth, cn)) / max_شے_وچ_گروہ_گنتی);
        }
        حرف cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_خانے_گنتی, tile_اچائی, ops[op], cn, ddepth,
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
        ناپ dsize(1, src.صفاں);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        إذا (op0 == REDUCE_AVG)
            k.دلیلاں(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_ناپ(dst), 1.0f / src.خانے_گنتی);
        وإلا
            k.دلیلاں(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_ناپ(dst));

        size_t local_ناپ[2] = { (size_t)buf_خانے_گنتی, (size_t)tile_اچائی};
        size_t global_ناپ[2] = { (size_t)buf_خانے_گنتی, (size_t)src.صفاں };
        أرجع k.لگاتار(2, global_ناپ, local_ناپ, خطأ);
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
        ناپ dsize(dim == 0 ? src.خانے_گنتی : 1, dim == 0 ? 1 : src.صفاں);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_ناپ(dst);

        إذا (op0 == REDUCE_AVG)
            k.دلیلاں(srcarg, temparg, 1.0f / (dim == 0 ? src.صفاں : src.خانے_گنتی));
        وإلا
            k.دلیلاں(srcarg, temparg);

        size_t globalsize = std::max(dsize.چوڑائی, dsize.اچائی);
        أرجع k.لگاتار(1, &globalsize, عدم, خطأ);
    }
}

}

#جے_انت

فراغ cv::reduce(داخلہ_Array _src, OutputArray _dst, صحيح dim, صحيح op, صحيح dtype)
{
    CV_INSTRUMENT_REGION();

    إذا (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, سچ, dtype < 0 && _dst.fixedType() ? _dst.ڈونگھائی() : dtype));
        أرجع;
    }
    صحيح op0 = op;
    صحيح stype = _src.type(), sdepth = CV_MAT_ڈونگھائی(stype), cn = CV_MAT_CN(stype);
    إذا( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    صحيح ddepth = CV_MAT_ڈونگھائی(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_جوڑ || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_لگاتار(_dst.isUMat(),
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

    _dst.create(dim == 0 ? 1 : src.صفاں, dim == 0 ? src.خانے_گنتی : 1, dtype);
    Mat dst = _dst.getMat(), عارضی_2 = dst;

    إذا( op == REDUCE_AVG )
    {
        op = REDUCE_جوڑ;
        إذا( sdepth < CV_32S && ddepth < CV_32S )
        {
            عارضی_2.create(dst.صفاں, dst.خانے_گنتی, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    إذا( dim == 0 )
    {
        إذا( op == REDUCE_جوڑ )
        {
            Reduce_جوڑ_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_جوڑ_R8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_جوڑ_R8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_جوڑ_R8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_جوڑ_R16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_جوڑ_R16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_جوڑ_R16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_جوڑ_R16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_جوڑ_R32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_جوڑ_R32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_جوڑ_R64f64f;
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
            Reduce_جوڑ_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
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
        إذا(op == REDUCE_جوڑ)
        {
            Reduce_جوڑ_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            إذا(simd_func)
                func = (ReduceFunc)simd_func;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_جوڑ_C8u32s;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_جوڑ_C8u32f;
            وإلا إذا(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_جوڑ_C8u64f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_جوڑ_C16u32f;
            وإلا إذا(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_جوڑ_C16u64f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_جوڑ_C16s32f;
            وإلا إذا(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_جوڑ_C16s64f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_جوڑ_C32f32f;
            وإلا إذا(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_جوڑ_C32f64f;
            وإلا إذا(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_جوڑ_C64f64f;
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

    func( src, عارضی_2 );

    إذا( op0 == REDUCE_AVG )
        عارضی_2.convert_منزل(dst, dst.type(), 1./(dim == 0 ? src.صفاں : src.خانے_گنتی));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

نطاق cv
{

سانچا<قسم_ناں T> ساكن فراغ ترتیب_دیو( ثابت Mat& src, Mat& dst, صحيح flags )
{
    صحيح n, len;
    منطقي ترتیب_دیو_صفاں = (flags & 1) == ترتیب_دیو_EVERY_صف;
    منطقي inplace = src.انکڑے == dst.انکڑے;
    منطقي ترتیب_دیو_Descending = (flags & ترتیب_دیو_DESCENDING) != 0;

    إذا( ترتیب_دیو_صفاں )
        n = src.صفاں, len = src.خانے_گنتی;
    وإلا
        n = src.خانے_گنتی, len = src.صفاں;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_بفر<T> buf;
        إذا( !ترتیب_دیو_صفاں )
            buf.allocate(len);
        T* bptr = buf.انکڑے();

        لكل( صحيح i = range.شروع; i < range.end; i++ )
        {
            T* ptr = bptr;
            إذا( ترتیب_دیو_صفاں )
            {
                T* dptr = dst.ptr<T>(i);
                إذا( !inplace )
                {
                    ثابت T* sptr = src.ptr<T>(i);
                    حافظہ_نقل(dptr, sptr, حجم(T) * len);
                }
                ptr = dptr;
            }
            وإلا
            {
                لكل( صحيح j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            إذا( ترتیب_دیو_Descending )
            {
                لكل( صحيح j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            إذا( !ترتیب_دیو_صفاں )
                لكل( صحيح j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#إذا defined(HAVE_IPP) && !IPP_DISABLE_ترتیب_دیو
تعريفنوع Ipp_حالت (CV_STDCALL *Ipp_ترتیب_دیو_Func)(فراغ  *pSrcDst, صحيح    len, Ipp8u *p_بفر);

سانچا<قسم_ناں T, Ipp_حالت (CV_STDCALL *fn)(T*, صحيح, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ipp_ترتیب_دیو_Wrap(فراغ* pSrcDst, صحيح len, Ipp8u* p_بفر)
{
    أرجع fn((T*)pSrcDst, len, p_بفر);
}

ساكن Ipp_ترتیب_دیو_Func get_ترتیب_دیو_Func(صحيح ڈونگھائی, منطقي ترتیب_دیو_Descending)
{
    إذا (!ترتیب_دیو_Descending)
        أرجع ڈونگھائی == CV_8U ? ipp_ترتیب_دیو_Wrap<Ipp8u, ipps_ترتیب_دیو_Radix_Ascend_8u_I> :
            ڈونگھائی == CV_16U ? ipp_ترتیب_دیو_Wrap<Ipp16u, ipps_ترتیب_دیو_Radix_Ascend_16u_I> :
            ڈونگھائی == CV_16S ? ipp_ترتیب_دیو_Wrap<Ipp16s, ipps_ترتیب_دیو_Radix_Ascend_16s_I> :
            ڈونگھائی == CV_32S ? ipp_ترتیب_دیو_Wrap<Ipp32s, ipps_ترتیب_دیو_Radix_Ascend_32s_I> :
            ڈونگھائی == CV_32F ? ipp_ترتیب_دیو_Wrap<Ipp32f, ipps_ترتیب_دیو_Radix_Ascend_32f_I> :
            ڈونگھائی == CV_64F ? ipp_ترتیب_دیو_Wrap<Ipp64f, ipps_ترتیب_دیو_Radix_Ascend_64f_I> :
            0;
    وإلا
        أرجع ڈونگھائی == CV_8U ? ipp_ترتیب_دیو_Wrap<Ipp8u, ipps_ترتیب_دیو_Radix_Descend_8u_I> :
            ڈونگھائی == CV_16U ? ipp_ترتیب_دیو_Wrap<Ipp16u, ipps_ترتیب_دیو_Radix_Descend_16u_I> :
            ڈونگھائی == CV_16S ? ipp_ترتیب_دیو_Wrap<Ipp16s, ipps_ترتیب_دیو_Radix_Descend_16s_I> :
            ڈونگھائی == CV_32S ? ipp_ترتیب_دیو_Wrap<Ipp32s, ipps_ترتیب_دیو_Radix_Descend_32s_I> :
            ڈونگھائی == CV_32F ? ipp_ترتیب_دیو_Wrap<Ipp32f, ipps_ترتیب_دیو_Radix_Descend_32f_I> :
            ڈونگھائی == CV_64F ? ipp_ترتیب_دیو_Wrap<Ipp64f, ipps_ترتیب_دیو_Radix_Descend_64f_I> :
            0;
}

ساكن منطقي ipp_ترتیب_دیو(ثابت Mat& src, Mat& dst, صحيح flags)
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        ترتیب_دیو_صفاں        = (flags & 1) == ترتیب_دیو_EVERY_صف;
    منطقي        ترتیب_دیو_Descending  = (flags & ترتیب_دیو_DESCENDING) != 0;
    منطقي        inplace         = (src.انکڑے == dst.انکڑے);
    صحيح         ڈونگھائی           = src.ڈونگھائی();
    Ipp_انکڑے_Type type            = ippi_Get_انکڑے_Type(ڈونگھائی);

    Ipp_ترتیب_دیو_Func ipps_ترتیب_دیو_Radix_I = get_ترتیب_دیو_Func(ڈونگھائی, ترتیب_دیو_Descending);
    إذا(!ipps_ترتیب_دیو_Radix_I)
        أرجع خطأ;

    صحيح n, len;
    إذا( ترتیب_دیو_صفاں )
        n = src.صفاں, len = src.خانے_گنتی;
    وإلا
        n = src.خانے_گنتی, len = src.صفاں;

    صحيح بفر_ناپ;
    إذا(ipps_ترتیب_دیو_Radix_Get_بفر_ناپ(len, type, &بفر_ناپ) < 0)
        أرجع خطأ;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    متطاير منطقي ok = سچ;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_بفر<Ipp8u> بفر(بفر_ناپ);
        Mat  صف;
        Rect subRect(0,0,1,src.صفاں);
        إذا( !ترتیب_دیو_صفاں )
            صف.create(1, src.صفاں, src.type());

        لكل( صحيح i = range.شروع; i < range.end; i++ )
        {
            إذا( !ok )
                اكسر;
            فراغ* ptr;
            إذا( ترتیب_دیو_صفاں )
            {
                uchar* dptr = dst.ptr(i);
                إذا( !inplace )
                    حافظہ_نقل(dptr, src.ptr(i), src.خانے_گنتی * src.elem_ناپ());
                ptr = dptr;
            }
            وإلا
            {
                subRect.x = i;
                Mat(src, subRect).copy_منزل(صف);
                ptr = صف.ptr();
            }

            إذا( CV_INSTRUMENT_FUN_IPP(ipps_ترتیب_دیو_Radix_I, ptr, len, بفر.انکڑے()) < 0 )
            {
                ok = خطأ;
                اكسر;
            }

            إذا( !ترتیب_دیو_صفاں )
            {
                Mat dstSub(dst, subRect);
                صف = صف.reshape(1, dstSub.صفاں);
                صف.copy_منزل(dstSub);
                صف = صف.reshape(1, 1);
            }
        }
    });

    أرجع ok;
}
#جے_انت

سانچا<قسم_ناں _Tp> صنف LessThanIdx
{
عام:
    LessThanIdx( ثابت _Tp* _arr ) : arr(_arr) {}
    منطقي عامل()(صحيح a, صحيح b) ثابت { أرجع arr[a] < arr[b]; }
    ثابت _Tp* arr;
};

سانچا<قسم_ناں T> ساكن فراغ ترتیب_دیو_Idx_2( ثابت Mat& src, Mat& dst, صحيح flags )
{
    Auto_بفر<T> buf;
    Auto_بفر<صحيح> ibuf;
    منطقي ترتیب_دیو_صفاں = (flags & 1) == ترتیب_دیو_EVERY_صف;
    منطقي ترتیب_دیو_Descending = (flags & ترتیب_دیو_DESCENDING) != 0;

    CV_Assert( src.انکڑے != dst.انکڑے );

    صحيح n, len;
    إذا( ترتیب_دیو_صفاں )
        n = src.صفاں, len = src.خانے_گنتی;
    وإلا
    {
        n = src.خانے_گنتی, len = src.صفاں;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.انکڑے();
    صحيح* _iptr = ibuf.انکڑے();

    لكل( صحيح i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        صحيح* iptr = _iptr;

        إذا( ترتیب_دیو_صفاں )
        {
            ptr = (T*)(src.انکڑے + src.قدم*i);
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
        إذا( ترتیب_دیو_Descending )
        {
            لكل( صحيح j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        إذا( !ترتیب_دیو_صفاں )
            لكل( صحيح j = 0; j < len; j++ )
                dst.ptr<صحيح>(j)[i] = iptr[j];
    }
}

#إذا defined(HAVE_IPP) && !IPP_DISABLE_ترتیب_دیو
تعريفنوع Ipp_حالت (CV_STDCALL *Ipp_ترتیب_دیو_اشاریہ_Func)(ثابت فراغ*  pSrc, Ipp32s src_Stride_بائٹاں, Ipp32s *pDstIndx, صحيح len, Ipp8u *p_بفر);

سانچا<قسم_ناں T, Ipp_حالت (CV_STDCALL *fn)(ثابت T*, Ipp32s, Ipp32s*, صحيح, Ipp8u*)>
ساكن Ipp_حالت CV_STDCALL ipp_ترتیب_دیو_اشاریہ_Wrap(ثابت فراغ* pSrc, Ipp32s src_Stride_بائٹاں, Ipp32s *pDstIndx, صحيح len, Ipp8u *p_بفر)
{
    أرجع fn((ثابت T*)pSrc, src_Stride_بائٹاں, pDstIndx, len, p_بفر);
}

ساكن Ipp_ترتیب_دیو_اشاریہ_Func get_ترتیب_دیو_اشاریہ_Func(صحيح ڈونگھائی, منطقي ترتیب_دیو_Descending)
{
    إذا (!ترتیب_دیو_Descending)
        أرجع ڈونگھائی == CV_8U ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp8u, ipps_ترتیب_دیو_Radix_اشاریہ_Ascend_8u> :
            ڈونگھائی == CV_16U ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp16u, ipps_ترتیب_دیو_Radix_اشاریہ_Ascend_16u> :
            ڈونگھائی == CV_16S ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp16s, ipps_ترتیب_دیو_Radix_اشاریہ_Ascend_16s> :
            ڈونگھائی == CV_32S ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp32s, ipps_ترتیب_دیو_Radix_اشاریہ_Ascend_32s> :
            ڈونگھائی == CV_32F ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp32f, ipps_ترتیب_دیو_Radix_اشاریہ_Ascend_32f> :
            0;
    وإلا
        أرجع ڈونگھائی == CV_8U ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp8u, ipps_ترتیب_دیو_Radix_اشاریہ_Descend_8u> :
            ڈونگھائی == CV_16U ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp16u, ipps_ترتیب_دیو_Radix_اشاریہ_Descend_16u> :
            ڈونگھائی == CV_16S ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp16s, ipps_ترتیب_دیو_Radix_اشاریہ_Descend_16s> :
            ڈونگھائی == CV_32S ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp32s, ipps_ترتیب_دیو_Radix_اشاریہ_Descend_32s> :
            ڈونگھائی == CV_32F ? ipp_ترتیب_دیو_اشاریہ_Wrap<Ipp32f, ipps_ترتیب_دیو_Radix_اشاریہ_Descend_32f> :
            0;
}

ساكن منطقي ipp_ترتیب_دیو_Idx( ثابت Mat& src, Mat& dst, صحيح flags )
{
    CV_INSTRUMENT_REGION_IPP();

    منطقي        ترتیب_دیو_صفاں        = (flags & 1) == ترتیب_دیو_EVERY_صف;
    منطقي        ترتیب_دیو_Descending  = (flags & ترتیب_دیو_DESCENDING) != 0;
    صحيح         ڈونگھائی           = src.ڈونگھائی();
    Ipp_انکڑے_Type type            = ippi_Get_انکڑے_Type(ڈونگھائی);

    Ipp_ترتیب_دیو_اشاریہ_Func ipps_ترتیب_دیو_Radix_اشاریہ = get_ترتیب_دیو_اشاریہ_Func(ڈونگھائی, ترتیب_دیو_Descending);
    إذا(!ipps_ترتیب_دیو_Radix_اشاریہ)
        أرجع خطأ;

    إذا(ترتیب_دیو_صفاں)
    {
        Auto_بفر<Ipp8u> بفر;
        صحيح               بفر_ناپ;
        إذا(ipps_ترتیب_دیو_Radix_اشاریہ_Get_بفر_ناپ(src.خانے_گنتی, type, &بفر_ناپ) < 0)
            أرجع خطأ;

        بفر.allocate(بفر_ناپ);

        لكل(صحيح i = 0; i < src.صفاں; i++)
        {
            إذا(CV_INSTRUMENT_FUN_IPP(ipps_ترتیب_دیو_Radix_اشاریہ, (ثابت فراغ*)src.ptr(i), (Ipp32s)src.قدم[1], (Ipp32s*)dst.ptr(i), src.خانے_گنتی, بفر.انکڑے()) < 0)
                أرجع خطأ;
        }
    }
    وإلا
    {
        Mat  dst_صف(1, dst.صفاں, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.صفاں);

        Auto_بفر<Ipp8u> بفر;
        صحيح               بفر_ناپ;
        إذا(ipps_ترتیب_دیو_Radix_اشاریہ_Get_بفر_ناپ(src.صفاں, type, &بفر_ناپ) < 0)
            أرجع خطأ;

        بفر.allocate(بفر_ناپ);

        Ipp32s src_قدم_3 = (Ipp32s)src.قدم[0];
        لكل(صحيح i = 0; i < src.خانے_گنتی; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            إذا(CV_INSTRUMENT_FUN_IPP(ipps_ترتیب_دیو_Radix_اشاریہ, (ثابت فراغ*)src.ptr(0, i), src_قدم_3, (Ipp32s*)dst_صف.ptr(), src.صفاں, بفر.انکڑے()) < 0)
                أرجع خطأ;

            dst_صف = dst_صف.reshape(1, dstSub.صفاں);
            dst_صف.copy_منزل(dstSub);
        }
    }

    أرجع سچ;
}
#جے_انت

تعريفنوع فراغ (*ترتیب_دیو_Func)(ثابت Mat& src, Mat& dst, صحيح flags);
}

فراغ cv::sort( داخلہ_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_ناپ( src, src.type() );
    Mat dst = _dst.getMat();
#إذا !IPP_DISABLE_ترتیب_دیو
    CV_IPP_لگاتار_FAST(ipp_ترتیب_دیو(src, dst, flags));
#جے_انت

    ساكن ترتیب_دیو_Func tab[CV_ڈونگھائی_MAX] =
    {
        ترتیب_دیو<uchar>, ترتیب_دیو<schar>, ترتیب_دیو<ushort>, ترتیب_دیو<چھوٹا>,
        ترتیب_دیو<صحيح>, ترتیب_دیو<عائم>, ترتیب_دیو<مزدوج>, 0
    };
    ترتیب_دیو_Func func = tab[src.ڈونگھائی()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

فراغ cv::ترتیب_دیو_Idx( داخلہ_Array _src, OutputArray _dst, صحيح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    إذا( dst.انکڑے == src.انکڑے )
        _dst.release();
    _dst.create_Same_ناپ( src, CV_32S );
    dst = _dst.getMat();

#إذا !IPP_DISABLE_ترتیب_دیو
    CV_IPP_لگاتار_FAST(ipp_ترتیب_دیو_Idx(src, dst, flags));
#جے_انت

    ساكن ترتیب_دیو_Func tab[CV_ڈونگھائی_MAX] =
    {
        ترتیب_دیو_Idx_2<uchar>, ترتیب_دیو_Idx_2<schar>, ترتیب_دیو_Idx_2<ushort>, ترتیب_دیو_Idx_2<چھوٹا>,
        ترتیب_دیو_Idx_2<صحيح>, ترتیب_دیو_Idx_2<عائم>, ترتیب_دیو_Idx_2<مزدوج>, 0
    };
    ترتیب_دیو_Func func = tab[src.ڈونگھائی()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
