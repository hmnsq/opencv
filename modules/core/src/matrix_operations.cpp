// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencv2/core/mat.hpp"
#شامل "opencl_kernels_core.hpp"

#تعریف_کرو IPP_DISABLE_REDUCE 1
#تعریف_کرو IPP_DISABLE_ترتیب_دو   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

خالی cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.صفیں, b.صفیں);
    std::swap(a.ستون_تعداد, b.ستون_تعداد);
    std::swap(a.ڈیٹا, b.ڈیٹا);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.قدم, b.قدم);
}


خالی cv::hconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    اگر( nsrc == 0 || !src )
    {
        _dst.release();
        واپس;
    }

    صحیح کل_ستون_تعداد = 0, ستون_تعداد = 0;
    برائے( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].صفیں == src[0].صفیں &&
                   src[i].type() == src[0].type());
        کل_ستون_تعداد += src[i].ستون_تعداد;
    }
    _dst.create( src[0].صفیں, کل_ستون_تعداد, src[0].type());
    Mat dst = _dst.getMat();
    برائے( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(ستون_تعداد, 0, src[i].ستون_تعداد, src[i].صفیں));
        src[i].copy_مقصد(dpart);
        ستون_تعداد += src[i].ستون_تعداد;
    }
}

خالی cv::hconcat(داخلہ_Array src1, داخلہ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

خالی cv::hconcat(داخلہ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_سمتیہ(src);
    hconcat(!src.empty() ? &src[0] : خالی_اشاریہ, src.size(), dst);
}

خالی cv::vconcat(ثابت Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    اگر( nsrc == 0 || !src )
    {
        _dst.release();
        واپس;
    }

    صحیح کل_صفیں = 0, صفیں = 0;
    برائے( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].ستون_تعداد == src[0].ستون_تعداد &&
                  src[i].type() == src[0].type());
        کل_صفیں += src[i].صفیں;
    }
    _dst.create( کل_صفیں, src[0].ستون_تعداد, src[0].type());
    Mat dst = _dst.getMat();
    برائے( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, صفیں, src[i].ستون_تعداد, src[i].صفیں));
        src[i].copy_مقصد(dpart);
        صفیں += src[i].صفیں;
    }
}

خالی cv::vconcat(داخلہ_Array src1, داخلہ_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

خالی cv::vconcat(داخلہ_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_سمتیہ(src);
    vconcat(!src.empty() ? &src[0] : خالی_اشاریہ, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#اگر_تعریف_شدہ HAVE_OPENCL

نامخلا cv {

ساکن منطقی ocl_سیٹ_شناختی_عنصر( داخلہ_Output_Array _m, ثابت Scalar& s )
{
    صحیح type = _m.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type), kercn = cn, صفیں_فی_WI = 1;
    صحیح sctype = CV_MAKE_TYPE(گہرائی, cn == 3 ? 4 : cn);
    اگر (ocl::Device::getDefault().isIntel())
    {
        صفیں_فی_WI = 4;
        اگر (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_سمتیہ_چوڑائی(_m), 4);
            اگر (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::سیٹ_شناختی_عنصر_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_مقصد_Str(CV_MAKE_TYPE(گہرائی, kercn)),
                         ocl::memop_Type_مقصد_Str(گہرائی), cn,
                         ocl::memop_Type_مقصد_Str(sctype),
                         kercn, صفیں_فی_WI));
    اگر (k.empty())
        واپس جھوٹ;

    UMat m = _m.getUMat();
    k.دلائل(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.ستون_تعداد * cn / kercn, ((size_t)m.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, خالی_اشاریہ, جھوٹ);
}

}

#اگر_ختم

خالی cv::سیٹ_شناختی_عنصر( داخلہ_Output_Array _m, ثابت Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_تسلسل(_m.isUMat(),
               ocl_سیٹ_شناختی_عنصر(_m, s))

    Mat m = _m.getMat();
    صحیح صفیں = m.صفیں, ستون_تعداد = m.ستون_تعداد, type = m.type();

    اگر( type == CV_32FC1 )
    {
        اعشاری* ڈیٹا = m.ptr<اعشاری>();
        اعشاری val = (اعشاری)s[0];
        size_t قدم = m.قدم/حجم(ڈیٹا[0]);

        برائے( صحیح i = 0; i < صفیں; i++, ڈیٹا += قدم )
        {
            برائے( صحیح j = 0; j < ستون_تعداد; j++ )
                ڈیٹا[j] = 0;
            اگر( i < ستون_تعداد )
                ڈیٹا[i] = val;
        }
    }
    ورنہ اگر( type == CV_64FC1 )
    {
        دوہرا* ڈیٹا = m.ptr<دوہرا>();
        دوہرا val = s[0];
        size_t قدم = m.قدم/حجم(ڈیٹا[0]);

        برائے( صحیح i = 0; i < صفیں; i++, ڈیٹا += قدم )
        {
            std::fill(ڈیٹا, ڈیٹا + ستون_تعداد, 0.0);
            اگر (i < ستون_تعداد)
                ڈیٹا[i] = val;
        }
    }
    ورنہ
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


نامخلا cv {

UMat UMat::eye(صحیح صفیں, صحیح ستون_تعداد, صحیح type, UMatUsageFlags usageFlags)
{
    واپس UMat::eye(سائز(ستون_تعداد, صفیں), type, usageFlags);
}

UMat UMat::eye(سائز size, صحیح type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    سیٹ_شناختی_عنصر(m);
    واپس m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( داخلہ_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    صحیح type = m.type();
    صحیح nm = std::min(m.صفیں, m.ستون_تعداد);

    اگر( type == CV_32FC1 )
    {
        ثابت اعشاری* ptr = m.ptr<اعشاری>();
        size_t قدم = m.قدم/حجم(ptr[0]) + 1;
        دوہرا _s = 0;
        برائے( صحیح i = 0; i < nm; i++ )
            _s += ptr[i*قدم];
        واپس _s;
    }

    اگر( type == CV_64FC1 )
    {
        ثابت دوہرا* ptr = m.ptr<دوہرا>();
        size_t قدم = m.قدم/حجم(ptr[0]) + 1;
        دوہرا _s = 0;
        برائے( صحیح i = 0; i < nm; i++ )
            _s += ptr[i*قدم];
        واپس _s;
    }

    واپس cv::مجموع(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

خالی cv::completeSymm( داخلہ_Output_Array _m, منطقی LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t قدم = m.قدم, esz = m.elem_سائز();
    CV_Assert( m.dims <= 2 && m.صفیں == m.ستون_تعداد );

    صحیح صفیں = m.صفیں;
    صحیح j0 = 0, j1 = صفیں;

    uchar* ڈیٹا = m.ptr();
    برائے( صحیح i = 0; i < صفیں; i++ )
    {
        اگر( !LtoR ) j1 = i; ورنہ j0 = i+1;
        برائے( صحیح j = j0; j < j1; j++ )
            حافظہ_نقل(ڈیٹا + (i*قدم + j*esz), ڈیٹا + (j*قدم + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(داخلہ_Array _m) ثابت
{
    Mat m = _m.getMat();
    صحیح tp = type(), d = CV_MAT_گہرائی(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((صفیں == 3 && ستون_تعداد == 1) || (ستون_تعداد*channels() == 3 && صفیں == 1)));
    Mat نتیجہ(صفیں, ستون_تعداد, tp);

    اگر( d == CV_32F )
    {
        ثابت اعشاری *a = (ثابت اعشاری*)ڈیٹا, *b = (ثابت اعشاری*)m.ڈیٹا;
        اعشاری* c = (اعشاری*)نتیجہ.ڈیٹا;
        size_t lda = صفیں > 1 ? قدم/حجم(a[0]) : 1;
        size_t ldb = صفیں > 1 ? m.قدم/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    ورنہ اگر( d == CV_64F )
    {
        ثابت دوہرا *a = (ثابت دوہرا*)ڈیٹا, *b = (ثابت دوہرا*)m.ڈیٹا;
        دوہرا* c = (دوہرا*)نتیجہ.ڈیٹا;
        size_t lda = صفیں > 1 ? قدم/حجم(a[0]) : 1;
        size_t ldb = صفیں > 1 ? m.قدم/حجم(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    واپس نتیجہ;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

نامخلا cv
{

قسمتعریف خالی (*Reduce_مجموع_Func)(ثابت Mat& src, Mat& dst);
Reduce_مجموع_Func getReduceCSumFunc(صحیح sdepth, صحیح ddepth);
Reduce_مجموع_Func getReduceRSumFunc(صحیح sdepth, صحیح ddepth);
Reduce_مجموع_Func getReduceRSum2Func(صحیح sdepth, صحیح ddepth);

سانچہ <قسم_نام T, قسم_نام WT, قسم_نام Op>
ساخت ReduceR_SIMD
{
    صحیح عامل()(ثابت T*, صحیح آغاز, صحیح, WT*, ثابت Op&) ثابت
    {
        واپس آغاز;
    }
};


سانچہ<قسم_نام T, قسم_نام ST, قسم_نام WT, جماعت Op, جماعت OpInit>
جماعت ReduceR_Invoker : عوامی ParallelLoopBody
{
عوامی:
  ReduceR_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),بفر(srcmat.size().چوڑائی*srcmat.channels())
  {
  }
  خالی عامل()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت T* src = srcmat.ptr<T>();
    ثابت size_t srcstep = srcmat.قدم/حجم(src[0]);
    WT* buf = بفر.ڈیٹا();
    ST* dst = dstmat.ptr<ST>();
    صحیح i = 0;

    برائے( i = range.آغاز ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    صحیح اونچائی = srcmat.size().اونچائی;
    برائے( ; --اونچائی; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.آغاز, range.end, buf, op);
        #اگر CV_ENABLE_UNROLLED
        برائے(; i <= range.end - 4; i += 4 )
        {
            WT s0, s1;
            s0 = op(buf[i], (WT)src[i]);
            s1 = op(buf[i+1], (WT)src[i+1]);
            buf[i] = s0; buf[i+1] = s1;

            s0 = op(buf[i+2], (WT)src[i+2]);
            s1 = op(buf[i+3], (WT)src[i+3]);
            buf[i+2] = s0; buf[i+3] = s1;
        }
        #اگر_ختم
        برائے( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    برائے( i = range.آغاز ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
نجی:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  تغیرپذیر Auto_بفر<WT> بفر;
};

سانچہ<قسم_نام T, قسم_نام ST, جماعت Op, جماعت OpInit = OpNop<ST> > ساکن خالی
reduceR_( ثابت Mat& srcmat, Mat& dstmat)
{
    قسمتعریف قسم_نام Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().چوڑائی*srcmat.channels()), body, srcmat.size().چوڑائی*CV_ELEM_سائز(srcmat.گہرائی())/64);
}

سانچہ<قسم_نام T, قسم_نام ST, قسم_نام WT, جماعت Op, جماعت OpInit>
جماعت ReduceC_Invoker : عوامی ParallelLoopBody
{
عوامی:
  ReduceC_Invoker(ثابت Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  خالی عامل()(ثابت Range& range) ثابت CV_OVERRIDE
  {
    ثابت صحیح cn = srcmat.channels();
    ثابت صحیح چوڑائی = srcmat.size().چوڑائی*cn;
    Auto_بفر<WT> cumul(cn);
    برائے( صحیح y = range.آغاز; y < range.end; y++ )
    {
        ثابت T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        اگر( چوڑائی == cn )
        {
          برائے( صحیح k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        ورنہ
        {
            برائے(صحیح k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            برائے(صحیح k = cn ; k < چوڑائی ; k += cn )
            {
                برائے (صحیح c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            برائے(صحیح k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
نجی:
  ثابت Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

سانچہ<قسم_نام T, قسم_نام ST, جماعت Op, جماعت OpInit = OpNop<ST> > ساکن خالی
reduceC_( ثابت Mat& srcmat, Mat& dstmat)
{
    قسمتعریف قسم_نام Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().اونچائی), body);
}

قسمتعریف خالی (*ReduceFunc)( ثابت Mat& src, Mat& dst );

}

#تعریف_کرو reduce_مجموع_R8u32s  reduceR_<uchar, صحیح,   Op_جوڑو<صحیح>, OpNop<صحیح> >
#تعریف_کرو reduce_مجموع_R8u32f  reduceR_<uchar, اعشاری, Op_جوڑو<صحیح>, OpNop<صحیح> >
#تعریف_کرو reduce_مجموع_R8u64f  reduceR_<uchar, دوہرا,Op_جوڑو<صحیح>, OpNop<صحیح> >
#تعریف_کرو reduce_مجموع_R16u32f reduceR_<ushort,اعشاری, Op_جوڑو<اعشاری> >
#تعریف_کرو reduce_مجموع_R16u64f reduceR_<ushort,دوہرا,Op_جوڑو<دوہرا> >
#تعریف_کرو reduce_مجموع_R16s32f reduceR_<مختصر, اعشاری, Op_جوڑو<اعشاری> >
#تعریف_کرو reduce_مجموع_R16s64f reduceR_<مختصر, دوہرا,Op_جوڑو<دوہرا> >
#تعریف_کرو reduce_مجموع_R32f32f reduceR_<اعشاری, اعشاری, Op_جوڑو<اعشاری> >
#تعریف_کرو reduce_مجموع_R32f64f reduceR_<اعشاری, دوہرا,Op_جوڑو<دوہرا> >
#تعریف_کرو reduce_مجموع_R64f64f reduceR_<دوہرا,دوہرا,Op_جوڑو<دوہرا> >

#تعریف_کرو reduceSum2R8u32s  reduceR_<uchar, صحیح,   Op_جوڑو_Sqr<صحیح>,   OpSqr<صحیح> >
#تعریف_کرو reduceSum2R8u32f  reduceR_<uchar, اعشاری, Op_جوڑو_Sqr<صحیح>,   OpSqr<صحیح> >
#تعریف_کرو reduceSum2R8u64f  reduceR_<uchar, دوہرا,Op_جوڑو_Sqr<صحیح>,   OpSqr<صحیح> >
#تعریف_کرو reduceSum2R16u32f reduceR_<ushort,اعشاری, Op_جوڑو_Sqr<اعشاری>, OpSqr<اعشاری> >
#تعریف_کرو reduceSum2R16u64f reduceR_<ushort,دوہرا,Op_جوڑو_Sqr<دوہرا>,OpSqr<دوہرا> >
#تعریف_کرو reduceSum2R16s32f reduceR_<مختصر, اعشاری, Op_جوڑو_Sqr<اعشاری>, OpSqr<اعشاری> >
#تعریف_کرو reduceSum2R16s64f reduceR_<مختصر, دوہرا,Op_جوڑو_Sqr<دوہرا>,OpSqr<دوہرا> >
#تعریف_کرو reduceSum2R32f32f reduceR_<اعشاری, اعشاری, Op_جوڑو_Sqr<اعشاری>, OpSqr<اعشاری> >
#تعریف_کرو reduceSum2R32f64f reduceR_<اعشاری, دوہرا,Op_جوڑو_Sqr<دوہرا>,OpSqr<دوہرا> >
#تعریف_کرو reduceSum2R64f64f reduceR_<دوہرا,دوہرا,Op_جوڑو_Sqr<دوہرا>,OpSqr<دوہرا> >

#تعریف_کرو reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#تعریف_کرو reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#تعریف_کرو reduceMaxR16s reduceR_<مختصر, مختصر, OpMax<مختصر> >
#تعریف_کرو reduceMaxR32f reduceR_<اعشاری, اعشاری, OpMax<اعشاری> >
#تعریف_کرو reduceMaxR64f reduceR_<دوہرا,دوہرا,OpMax<دوہرا> >

#تعریف_کرو reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#تعریف_کرو reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#تعریف_کرو reduceMinR16s reduceR_<مختصر, مختصر, OpMin<مختصر> >
#تعریف_کرو reduceMinR32f reduceR_<اعشاری, اعشاری, OpMin<اعشاری> >
#تعریف_کرو reduceMinR64f reduceR_<دوہرا,دوہرا,OpMin<دوہرا> >

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ساکن اندرونی منطقی ipp_reduce_مجموع_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    صحیح sstep = (صحیح)srcmat.قدم, stype = srcmat.type(),
            ddepth = dstmat.گہرائی();

    Ippi_سائز roisize = { srcmat.size().چوڑائی, 1 };

    قسمتعریف Ipp_حالت (CV_STDCALL * Ippi_مجموع)(ثابت خالی * pSrc, صحیح src_قدم_3, Ippi_سائز roi_سائز, Ipp64f* p_مجموع);
    قسمتعریف Ipp_حالت (CV_STDCALL * Ippi_مجموع_Hint)(ثابت خالی * pSrc, صحیح src_قدم_3, Ippi_سائز roi_سائز, Ipp64f* p_مجموع, IppHintAlgorithm hint);
    Ippi_مجموع ippi_مجموع = 0;
    Ippi_مجموع_Hint ippi_مجموع_Hint = 0;

    اگر(ddepth == CV_64F)
    {
        ippi_مجموع =
            stype == CV_8UC1 ? (Ippi_مجموع)ippi_مجموع_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_مجموع)ippi_مجموع_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_مجموع)ippi_مجموع_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_مجموع)ippi_مجموع_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_مجموع)ippi_مجموع_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_مجموع)ippi_مجموع_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_مجموع)ippi_مجموع_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_مجموع)ippi_مجموع_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_مجموع)ippi_مجموع_16s_C4_R : 0;
        ippi_مجموع_Hint =
            stype == CV_32FC1 ? (Ippi_مجموع_Hint)ippi_مجموع_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_مجموع_Hint)ippi_مجموع_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_مجموع_Hint)ippi_مجموع_32f_C4_R : 0;
    }

    اگر(ippi_مجموع)
    {
        برائے(صحیح y = 0; y < srcmat.size().اونچائی; y++)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(ippi_مجموع, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                واپس جھوٹ;
        }
        واپس سچ;
    }
    ورنہ اگر(ippi_مجموع_Hint)
    {
        برائے(صحیح y = 0; y < srcmat.size().اونچائی; y++)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(ippi_مجموع_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                واپس جھوٹ;
        }
        واپس سچ;
    }

    واپس جھوٹ;
}

ساکن اندرونی خالی reduce_مجموع_C_8u16u16s32f_64f(ثابت cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_تسلسل_FAST(ipp_reduce_مجموع_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    اگر(dstmat.گہرائی() == CV_64F)
    {
        صحیح sdepth = CV_MAT_گہرائی(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, دوہرا,   cv::Op_جوڑو<دوہرا> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, دوہرا,   cv::Op_جوڑو<دوہرا> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<مختصر, دوہرا,   cv::Op_جوڑو<دوہرا> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<اعشاری, دوہرا,   cv::Op_جوڑو<دوہرا> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#اگر_ختم

#تعریف_کرو reduce_مجموع_C8u32s  reduceC_<uchar, صحیح,   Op_جوڑو<صحیح>, OpNop<صحیح> >
#تعریف_کرو reduce_مجموع_C8u32f  reduceC_<uchar, اعشاری, Op_جوڑو<صحیح>, OpNop<صحیح> >
#تعریف_کرو reduce_مجموع_C16u32f reduceC_<ushort,اعشاری, Op_جوڑو<اعشاری> >
#تعریف_کرو reduce_مجموع_C16s32f reduceC_<مختصر, اعشاری, Op_جوڑو<اعشاری> >
#تعریف_کرو reduce_مجموع_C32f32f reduceC_<اعشاری, اعشاری, Op_جوڑو<اعشاری> >
#تعریف_کرو reduce_مجموع_C64f64f reduceC_<دوہرا,دوہرا,Op_جوڑو<دوہرا> >

#تعریف_کرو reduceSum2C8u32s  reduceC_<uchar, صحیح,   Op_جوڑو_Sqr<صحیح>,   OpSqr<صحیح> >
#تعریف_کرو reduceSum2C8u32f  reduceC_<uchar, اعشاری, Op_جوڑو_Sqr<صحیح>,   OpSqr<صحیح> >
#تعریف_کرو reduceSum2C16u32f reduceC_<ushort,اعشاری, Op_جوڑو_Sqr<اعشاری>, OpSqr<اعشاری> >
#تعریف_کرو reduceSum2C16s32f reduceC_<مختصر, اعشاری, Op_جوڑو_Sqr<اعشاری>, OpSqr<اعشاری> >
#تعریف_کرو reduceSum2C32f32f reduceC_<اعشاری, اعشاری, Op_جوڑو_Sqr<اعشاری>, OpSqr<اعشاری> >
#تعریف_کرو reduceSum2C64f64f reduceC_<دوہرا,دوہرا,Op_جوڑو_Sqr<دوہرا>,OpSqr<دوہرا> >

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#تعریف_کرو reduce_مجموع_C8u64f  reduce_مجموع_C_8u16u16s32f_64f
#تعریف_کرو reduce_مجموع_C16u64f reduce_مجموع_C_8u16u16s32f_64f
#تعریف_کرو reduce_مجموع_C16s64f reduce_مجموع_C_8u16u16s32f_64f
#تعریف_کرو reduce_مجموع_C32f64f reduce_مجموع_C_8u16u16s32f_64f
#ورنہ
#تعریف_کرو reduce_مجموع_C8u64f  reduceC_<uchar, دوہرا,Op_جوڑو<صحیح>, OpNop<صحیح> >
#تعریف_کرو reduce_مجموع_C16u64f reduceC_<ushort,دوہرا,Op_جوڑو<دوہرا> >
#تعریف_کرو reduce_مجموع_C16s64f reduceC_<مختصر, دوہرا,Op_جوڑو<دوہرا> >
#تعریف_کرو reduce_مجموع_C32f64f reduceC_<اعشاری, دوہرا,Op_جوڑو<دوہرا> >
#اگر_ختم

#تعریف_کرو reduceSum2C8u64f  reduceC_<uchar, دوہرا,Op_جوڑو_Sqr<صحیح>,   OpSqr<صحیح> >
#تعریف_کرو reduceSum2C16u64f reduceC_<ushort,دوہرا,Op_جوڑو_Sqr<دوہرا>,OpSqr<دوہرا> >
#تعریف_کرو reduceSum2C16s64f reduceC_<مختصر, دوہرا,Op_جوڑو_Sqr<دوہرا>,OpSqr<دوہرا> >
#تعریف_کرو reduceSum2C32f64f reduceC_<اعشاری, دوہرا,Op_جوڑو_Sqr<دوہرا>,OpSqr<دوہرا> >

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#تعریف_کرو REDUCE_OP(favor, optype, type1, type2) \
ساکن اندرونی منطقی ipp_reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    اگر((srcmat.channels() == 1)) \
    { \
        صحیح sstep = (صحیح)srcmat.قدم; \
        قسمتعریف Ipp##favor IppType; \
        Ippi_سائز roisize = ippi_سائز(srcmat.size().چوڑائی, 1);\
        برائے(صحیح y = 0; y < srcmat.size().اونچائی; y++)\
        {\
            اگر(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                واپس جھوٹ;\
        }\
        واپس سچ;\
    }\
    واپس جھوٹ; \
} \
ساکن اندرونی خالی reduce##optype##C##favor(ثابت cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_تسلسل_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#اگر_ختم

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, مختصر, مختصر)
REDUCE_OP(32f, Max, اعشاری, اعشاری)
#ورنہ
#تعریف_کرو reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#تعریف_کرو reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#تعریف_کرو reduceMaxC16s reduceC_<مختصر, مختصر, OpMax<مختصر> >
#تعریف_کرو reduceMaxC32f reduceC_<اعشاری, اعشاری, OpMax<اعشاری> >
#اگر_ختم
#تعریف_کرو reduceMaxC64f reduceC_<دوہرا,دوہرا,OpMax<دوہرا> >

#اگر defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, مختصر, مختصر)
REDUCE_OP(32f, Min, اعشاری, اعشاری)
#ورنہ
#تعریف_کرو reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#تعریف_کرو reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#تعریف_کرو reduceMinC16s reduceC_<مختصر, مختصر, OpMin<مختصر> >
#تعریف_کرو reduceMinC32f reduceC_<اعشاری, اعشاری, OpMin<اعشاری> >
#اگر_ختم
#تعریف_کرو reduceMinC64f reduceC_<دوہرا,دوہرا,OpMin<دوہرا> >

#اگر_تعریف_شدہ HAVE_OPENCL

نامخلا cv {

ساکن منطقی ocl_reduce(داخلہ_Array _src, OutputArray _dst,
                       صحیح dim, صحیح op, صحیح op0, صحیح stype, صحیح dtype)
{
    ثابت صحیح min_opt_ستون_تعداد = 128, buf_ستون_تعداد = 32;
    صحیح sdepth = CV_MAT_گہرائی(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_گہرائی(dtype), ddepth0 = ddepth;
    ثابت ocl::Device &defDev = ocl::Device::getDefault();
    منطقی doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_گروہ_سائز();
    منطقی useOptimized = 1 == dim && _src.ستون_تعداد() > min_opt_ستون_تعداد && (wgs >= buf_ستون_تعداد);

    اگر (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        واپس جھوٹ;

    اگر (op == REDUCE_AVG)
    {
        اگر (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ثابت حرف * ثابت ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    صحیح wdepth = std::max(ddepth, CV_32F);
    اگر (useOptimized)
    {
        size_t tile_اونچائی = (size_t)(wgs / buf_ستون_تعداد);
        اگر (defDev.isIntel())
        {
            ساکن ثابت size_t max_شے_اندر_گروہ_گنتی = 16;
            tile_اونچائی = min(tile_اونچائی, defDev.local_Mem_سائز() / buf_ستون_تعداد / CV_ELEM_سائز(CV_MAKETYPE(wdepth, cn)) / max_شے_اندر_گروہ_گنتی);
        }
        حرف cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_ستون_تعداد, tile_اونچائی, ops[op], cn, ddepth,
                                            ocl::type_مقصد_Str(sdepth),
                                            ocl::type_مقصد_Str(ddepth),
                                            ocl::type_مقصد_Str(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], حجم(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], حجم(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], حجم(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        اگر (k.empty())
            واپس جھوٹ;
        UMat src = _src.getUMat();
        سائز dsize(1, src.صفیں);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        اگر (op0 == REDUCE_AVG)
            k.دلائل(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_سائز(dst), 1.0f / src.ستون_تعداد);
        ورنہ
            k.دلائل(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_سائز(dst));

        size_t local_سائز[2] = { (size_t)buf_ستون_تعداد, (size_t)tile_اونچائی};
        size_t global_سائز[2] = { (size_t)buf_ستون_تعداد, (size_t)src.صفیں };
        واپس k.تسلسل(2, global_سائز, local_سائز, جھوٹ);
    }
    ورنہ
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
        اگر (k.empty())
            واپس جھوٹ;

        UMat src = _src.getUMat();
        سائز dsize(dim == 0 ? src.ستون_تعداد : 1, dim == 0 ? 1 : src.صفیں);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_سائز(dst);

        اگر (op0 == REDUCE_AVG)
            k.دلائل(srcarg, temparg, 1.0f / (dim == 0 ? src.صفیں : src.ستون_تعداد));
        ورنہ
            k.دلائل(srcarg, temparg);

        size_t globalsize = std::max(dsize.چوڑائی, dsize.اونچائی);
        واپس k.تسلسل(1, &globalsize, تہی, جھوٹ);
    }
}

}

#اگر_ختم

خالی cv::reduce(داخلہ_Array _src, OutputArray _dst, صحیح dim, صحیح op, صحیح dtype)
{
    CV_INSTRUMENT_REGION();

    اگر (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, سچ, dtype < 0 && _dst.fixedType() ? _dst.گہرائی() : dtype));
        واپس;
    }
    صحیح op0 = op;
    صحیح stype = _src.type(), sdepth = CV_MAT_گہرائی(stype), cn = CV_MAT_CN(stype);
    اگر( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    صحیح ddepth = CV_MAT_گہرائی(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_مجموع || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_تسلسل(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    اگر (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    اگر (src.empty()) {
        src.convert_مقصد(_dst, dtype);
        واپس;
    }
    اگر (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.صفیں, dim == 0 ? src.ستون_تعداد : 1, dtype);
    Mat dst = _dst.getMat(), عارضی_2 = dst;

    اگر( op == REDUCE_AVG )
    {
        op = REDUCE_مجموع;
        اگر( sdepth < CV_32S && ddepth < CV_32S )
        {
            عارضی_2.create(dst.صفیں, dst.ستون_تعداد, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    اگر( dim == 0 )
    {
        اگر( op == REDUCE_مجموع )
        {
            Reduce_مجموع_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            اگر(simd_func)
                func = (ReduceFunc)simd_func;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_مجموع_R8u32s;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_مجموع_R8u32f;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_مجموع_R8u64f;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_مجموع_R16u32f;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_مجموع_R16u64f;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_مجموع_R16s32f;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_مجموع_R16s64f;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_مجموع_R32f32f;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_مجموع_R32f64f;
            ورنہ اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_مجموع_R64f64f;
        }
        ورنہ اگر(op == REDUCE_MAX)
        {
            اگر(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            ورنہ اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        ورنہ اگر(op == REDUCE_MIN)
        {
            اگر(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            ورنہ اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        ورنہ اگر( op == REDUCE_SUM2 )
        {
            Reduce_مجموع_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
            اگر(simd_func)
                func = simd_func;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            ورنہ اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    ورنہ
    {
        اگر(op == REDUCE_مجموع)
        {
            Reduce_مجموع_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            اگر(simd_func)
                func = (ReduceFunc)simd_func;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_مجموع_C8u32s;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_مجموع_C8u32f;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_مجموع_C8u64f;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_مجموع_C16u32f;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_مجموع_C16u64f;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_مجموع_C16s32f;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_مجموع_C16s64f;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_مجموع_C32f32f;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_مجموع_C32f64f;
            ورنہ اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_مجموع_C64f64f;
        }
        ورنہ اگر(op == REDUCE_MAX)
        {
            اگر(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            ورنہ اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        ورنہ اگر(op == REDUCE_MIN)
        {
            اگر(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            ورنہ اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        ورنہ اگر(op == REDUCE_SUM2)
        {
            اگر(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            ورنہ اگر(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            ورنہ اگر(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            ورنہ اگر(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            ورنہ اگر(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            ورنہ اگر(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    اگر( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, عارضی_2 );

    اگر( op0 == REDUCE_AVG )
        عارضی_2.convert_مقصد(dst, dst.type(), 1./(dim == 0 ? src.صفیں : src.ستون_تعداد));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

نامخلا cv
{

سانچہ<قسم_نام T> ساکن خالی ترتیب_دو( ثابت Mat& src, Mat& dst, صحیح flags )
{
    صحیح n, len;
    منطقی ترتیب_دو_صفیں = (flags & 1) == ترتیب_دو_EVERY_صف;
    منطقی inplace = src.ڈیٹا == dst.ڈیٹا;
    منطقی ترتیب_دو_Descending = (flags & ترتیب_دو_DESCENDING) != 0;

    اگر( ترتیب_دو_صفیں )
        n = src.صفیں, len = src.ستون_تعداد;
    ورنہ
        n = src.ستون_تعداد, len = src.صفیں;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_بفر<T> buf;
        اگر( !ترتیب_دو_صفیں )
            buf.allocate(len);
        T* bptr = buf.ڈیٹا();

        برائے( صحیح i = range.آغاز; i < range.end; i++ )
        {
            T* ptr = bptr;
            اگر( ترتیب_دو_صفیں )
            {
                T* dptr = dst.ptr<T>(i);
                اگر( !inplace )
                {
                    ثابت T* sptr = src.ptr<T>(i);
                    حافظہ_نقل(dptr, sptr, حجم(T) * len);
                }
                ptr = dptr;
            }
            ورنہ
            {
                برائے( صحیح j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            اگر( ترتیب_دو_Descending )
            {
                برائے( صحیح j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            اگر( !ترتیب_دو_صفیں )
                برائے( صحیح j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#اگر defined(HAVE_IPP) && !IPP_DISABLE_ترتیب_دو
قسمتعریف Ipp_حالت (CV_STDCALL *Ipp_ترتیب_دو_Func)(خالی  *pSrcDst, صحیح    len, Ipp8u *p_بفر);

سانچہ<قسم_نام T, Ipp_حالت (CV_STDCALL *fn)(T*, صحیح, Ipp8u*)>
ساکن Ipp_حالت CV_STDCALL ipp_ترتیب_دو_Wrap(خالی* pSrcDst, صحیح len, Ipp8u* p_بفر)
{
    واپس fn((T*)pSrcDst, len, p_بفر);
}

ساکن Ipp_ترتیب_دو_Func get_ترتیب_دو_Func(صحیح گہرائی, منطقی ترتیب_دو_Descending)
{
    اگر (!ترتیب_دو_Descending)
        واپس گہرائی == CV_8U ? ipp_ترتیب_دو_Wrap<Ipp8u, ipps_ترتیب_دو_Radix_Ascend_8u_I> :
            گہرائی == CV_16U ? ipp_ترتیب_دو_Wrap<Ipp16u, ipps_ترتیب_دو_Radix_Ascend_16u_I> :
            گہرائی == CV_16S ? ipp_ترتیب_دو_Wrap<Ipp16s, ipps_ترتیب_دو_Radix_Ascend_16s_I> :
            گہرائی == CV_32S ? ipp_ترتیب_دو_Wrap<Ipp32s, ipps_ترتیب_دو_Radix_Ascend_32s_I> :
            گہرائی == CV_32F ? ipp_ترتیب_دو_Wrap<Ipp32f, ipps_ترتیب_دو_Radix_Ascend_32f_I> :
            گہرائی == CV_64F ? ipp_ترتیب_دو_Wrap<Ipp64f, ipps_ترتیب_دو_Radix_Ascend_64f_I> :
            0;
    ورنہ
        واپس گہرائی == CV_8U ? ipp_ترتیب_دو_Wrap<Ipp8u, ipps_ترتیب_دو_Radix_Descend_8u_I> :
            گہرائی == CV_16U ? ipp_ترتیب_دو_Wrap<Ipp16u, ipps_ترتیب_دو_Radix_Descend_16u_I> :
            گہرائی == CV_16S ? ipp_ترتیب_دو_Wrap<Ipp16s, ipps_ترتیب_دو_Radix_Descend_16s_I> :
            گہرائی == CV_32S ? ipp_ترتیب_دو_Wrap<Ipp32s, ipps_ترتیب_دو_Radix_Descend_32s_I> :
            گہرائی == CV_32F ? ipp_ترتیب_دو_Wrap<Ipp32f, ipps_ترتیب_دو_Radix_Descend_32f_I> :
            گہرائی == CV_64F ? ipp_ترتیب_دو_Wrap<Ipp64f, ipps_ترتیب_دو_Radix_Descend_64f_I> :
            0;
}

ساکن منطقی ipp_ترتیب_دو(ثابت Mat& src, Mat& dst, صحیح flags)
{
    CV_INSTRUMENT_REGION_IPP();

    منطقی        ترتیب_دو_صفیں        = (flags & 1) == ترتیب_دو_EVERY_صف;
    منطقی        ترتیب_دو_Descending  = (flags & ترتیب_دو_DESCENDING) != 0;
    منطقی        inplace         = (src.ڈیٹا == dst.ڈیٹا);
    صحیح         گہرائی           = src.گہرائی();
    Ipp_ڈیٹا_Type type            = ippi_Get_ڈیٹا_Type(گہرائی);

    Ipp_ترتیب_دو_Func ipps_ترتیب_دو_Radix_I = get_ترتیب_دو_Func(گہرائی, ترتیب_دو_Descending);
    اگر(!ipps_ترتیب_دو_Radix_I)
        واپس جھوٹ;

    صحیح n, len;
    اگر( ترتیب_دو_صفیں )
        n = src.صفیں, len = src.ستون_تعداد;
    ورنہ
        n = src.ستون_تعداد, len = src.صفیں;

    صحیح بفر_سائز;
    اگر(ipps_ترتیب_دو_Radix_Get_بفر_سائز(len, type, &بفر_سائز) < 0)
        واپس جھوٹ;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    متلون منطقی ok = سچ;
    parallel_for_(Range(0, n), [&](ثابت Range& range)
    {
        Auto_بفر<Ipp8u> بفر(بفر_سائز);
        Mat  صف;
        Rect subRect(0,0,1,src.صفیں);
        اگر( !ترتیب_دو_صفیں )
            صف.create(1, src.صفیں, src.type());

        برائے( صحیح i = range.آغاز; i < range.end; i++ )
        {
            اگر( !ok )
                توڑو;
            خالی* ptr;
            اگر( ترتیب_دو_صفیں )
            {
                uchar* dptr = dst.ptr(i);
                اگر( !inplace )
                    حافظہ_نقل(dptr, src.ptr(i), src.ستون_تعداد * src.elem_سائز());
                ptr = dptr;
            }
            ورنہ
            {
                subRect.x = i;
                Mat(src, subRect).copy_مقصد(صف);
                ptr = صف.ptr();
            }

            اگر( CV_INSTRUMENT_FUN_IPP(ipps_ترتیب_دو_Radix_I, ptr, len, بفر.ڈیٹا()) < 0 )
            {
                ok = جھوٹ;
                توڑو;
            }

            اگر( !ترتیب_دو_صفیں )
            {
                Mat dstSub(dst, subRect);
                صف = صف.reshape(1, dstSub.صفیں);
                صف.copy_مقصد(dstSub);
                صف = صف.reshape(1, 1);
            }
        }
    });

    واپس ok;
}
#اگر_ختم

سانچہ<قسم_نام _Tp> جماعت LessThanIdx
{
عوامی:
    LessThanIdx( ثابت _Tp* _arr ) : arr(_arr) {}
    منطقی عامل()(صحیح a, صحیح b) ثابت { واپس arr[a] < arr[b]; }
    ثابت _Tp* arr;
};

سانچہ<قسم_نام T> ساکن خالی ترتیب_دو_Idx_2( ثابت Mat& src, Mat& dst, صحیح flags )
{
    Auto_بفر<T> buf;
    Auto_بفر<صحیح> ibuf;
    منطقی ترتیب_دو_صفیں = (flags & 1) == ترتیب_دو_EVERY_صف;
    منطقی ترتیب_دو_Descending = (flags & ترتیب_دو_DESCENDING) != 0;

    CV_Assert( src.ڈیٹا != dst.ڈیٹا );

    صحیح n, len;
    اگر( ترتیب_دو_صفیں )
        n = src.صفیں, len = src.ستون_تعداد;
    ورنہ
    {
        n = src.ستون_تعداد, len = src.صفیں;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.ڈیٹا();
    صحیح* _iptr = ibuf.ڈیٹا();

    برائے( صحیح i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        صحیح* iptr = _iptr;

        اگر( ترتیب_دو_صفیں )
        {
            ptr = (T*)(src.ڈیٹا + src.قدم*i);
            iptr = dst.ptr<صحیح>(i);
        }
        ورنہ
        {
            برائے( صحیح j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        برائے( صحیح j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        اگر( ترتیب_دو_Descending )
        {
            برائے( صحیح j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        اگر( !ترتیب_دو_صفیں )
            برائے( صحیح j = 0; j < len; j++ )
                dst.ptr<صحیح>(j)[i] = iptr[j];
    }
}

#اگر defined(HAVE_IPP) && !IPP_DISABLE_ترتیب_دو
قسمتعریف Ipp_حالت (CV_STDCALL *Ipp_ترتیب_دو_اشاریہ_Func)(ثابت خالی*  pSrc, Ipp32s src_Stride_بائٹیں, Ipp32s *pDstIndx, صحیح len, Ipp8u *p_بفر);

سانچہ<قسم_نام T, Ipp_حالت (CV_STDCALL *fn)(ثابت T*, Ipp32s, Ipp32s*, صحیح, Ipp8u*)>
ساکن Ipp_حالت CV_STDCALL ipp_ترتیب_دو_اشاریہ_Wrap(ثابت خالی* pSrc, Ipp32s src_Stride_بائٹیں, Ipp32s *pDstIndx, صحیح len, Ipp8u *p_بفر)
{
    واپس fn((ثابت T*)pSrc, src_Stride_بائٹیں, pDstIndx, len, p_بفر);
}

ساکن Ipp_ترتیب_دو_اشاریہ_Func get_ترتیب_دو_اشاریہ_Func(صحیح گہرائی, منطقی ترتیب_دو_Descending)
{
    اگر (!ترتیب_دو_Descending)
        واپس گہرائی == CV_8U ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp8u, ipps_ترتیب_دو_Radix_اشاریہ_Ascend_8u> :
            گہرائی == CV_16U ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp16u, ipps_ترتیب_دو_Radix_اشاریہ_Ascend_16u> :
            گہرائی == CV_16S ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp16s, ipps_ترتیب_دو_Radix_اشاریہ_Ascend_16s> :
            گہرائی == CV_32S ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp32s, ipps_ترتیب_دو_Radix_اشاریہ_Ascend_32s> :
            گہرائی == CV_32F ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp32f, ipps_ترتیب_دو_Radix_اشاریہ_Ascend_32f> :
            0;
    ورنہ
        واپس گہرائی == CV_8U ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp8u, ipps_ترتیب_دو_Radix_اشاریہ_Descend_8u> :
            گہرائی == CV_16U ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp16u, ipps_ترتیب_دو_Radix_اشاریہ_Descend_16u> :
            گہرائی == CV_16S ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp16s, ipps_ترتیب_دو_Radix_اشاریہ_Descend_16s> :
            گہرائی == CV_32S ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp32s, ipps_ترتیب_دو_Radix_اشاریہ_Descend_32s> :
            گہرائی == CV_32F ? ipp_ترتیب_دو_اشاریہ_Wrap<Ipp32f, ipps_ترتیب_دو_Radix_اشاریہ_Descend_32f> :
            0;
}

ساکن منطقی ipp_ترتیب_دو_Idx( ثابت Mat& src, Mat& dst, صحیح flags )
{
    CV_INSTRUMENT_REGION_IPP();

    منطقی        ترتیب_دو_صفیں        = (flags & 1) == ترتیب_دو_EVERY_صف;
    منطقی        ترتیب_دو_Descending  = (flags & ترتیب_دو_DESCENDING) != 0;
    صحیح         گہرائی           = src.گہرائی();
    Ipp_ڈیٹا_Type type            = ippi_Get_ڈیٹا_Type(گہرائی);

    Ipp_ترتیب_دو_اشاریہ_Func ipps_ترتیب_دو_Radix_اشاریہ = get_ترتیب_دو_اشاریہ_Func(گہرائی, ترتیب_دو_Descending);
    اگر(!ipps_ترتیب_دو_Radix_اشاریہ)
        واپس جھوٹ;

    اگر(ترتیب_دو_صفیں)
    {
        Auto_بفر<Ipp8u> بفر;
        صحیح               بفر_سائز;
        اگر(ipps_ترتیب_دو_Radix_اشاریہ_Get_بفر_سائز(src.ستون_تعداد, type, &بفر_سائز) < 0)
            واپس جھوٹ;

        بفر.allocate(بفر_سائز);

        برائے(صحیح i = 0; i < src.صفیں; i++)
        {
            اگر(CV_INSTRUMENT_FUN_IPP(ipps_ترتیب_دو_Radix_اشاریہ, (ثابت خالی*)src.ptr(i), (Ipp32s)src.قدم[1], (Ipp32s*)dst.ptr(i), src.ستون_تعداد, بفر.ڈیٹا()) < 0)
                واپس جھوٹ;
        }
    }
    ورنہ
    {
        Mat  dst_صف(1, dst.صفیں, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.صفیں);

        Auto_بفر<Ipp8u> بفر;
        صحیح               بفر_سائز;
        اگر(ipps_ترتیب_دو_Radix_اشاریہ_Get_بفر_سائز(src.صفیں, type, &بفر_سائز) < 0)
            واپس جھوٹ;

        بفر.allocate(بفر_سائز);

        Ipp32s src_قدم_3 = (Ipp32s)src.قدم[0];
        برائے(صحیح i = 0; i < src.ستون_تعداد; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            اگر(CV_INSTRUMENT_FUN_IPP(ipps_ترتیب_دو_Radix_اشاریہ, (ثابت خالی*)src.ptr(0, i), src_قدم_3, (Ipp32s*)dst_صف.ptr(), src.صفیں, بفر.ڈیٹا()) < 0)
                واپس جھوٹ;

            dst_صف = dst_صف.reshape(1, dstSub.صفیں);
            dst_صف.copy_مقصد(dstSub);
        }
    }

    واپس سچ;
}
#اگر_ختم

قسمتعریف خالی (*ترتیب_دو_Func)(ثابت Mat& src, Mat& dst, صحیح flags);
}

خالی cv::sort( داخلہ_Array _src, OutputArray _dst, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_سائز( src, src.type() );
    Mat dst = _dst.getMat();
#اگر !IPP_DISABLE_ترتیب_دو
    CV_IPP_تسلسل_FAST(ipp_ترتیب_دو(src, dst, flags));
#اگر_ختم

    ساکن ترتیب_دو_Func tab[CV_گہرائی_MAX] =
    {
        ترتیب_دو<uchar>, ترتیب_دو<schar>, ترتیب_دو<ushort>, ترتیب_دو<مختصر>,
        ترتیب_دو<صحیح>, ترتیب_دو<اعشاری>, ترتیب_دو<دوہرا>, 0
    };
    ترتیب_دو_Func func = tab[src.گہرائی()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

خالی cv::ترتیب_دو_Idx( داخلہ_Array _src, OutputArray _dst, صحیح flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    اگر( dst.ڈیٹا == src.ڈیٹا )
        _dst.release();
    _dst.create_Same_سائز( src, CV_32S );
    dst = _dst.getMat();

#اگر !IPP_DISABLE_ترتیب_دو
    CV_IPP_تسلسل_FAST(ipp_ترتیب_دو_Idx(src, dst, flags));
#اگر_ختم

    ساکن ترتیب_دو_Func tab[CV_گہرائی_MAX] =
    {
        ترتیب_دو_Idx_2<uchar>, ترتیب_دو_Idx_2<schar>, ترتیب_دو_Idx_2<ushort>, ترتیب_دو_Idx_2<مختصر>,
        ترتیب_دو_Idx_2<صحیح>, ترتیب_دو_Idx_2<اعشاری>, ترتیب_دو_Idx_2<دوہرا>, 0
    };
    ترتیب_دو_Func func = tab[src.گہرائی()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
