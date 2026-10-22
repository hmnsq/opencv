// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencv2/core/mat.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

#నిర్వచించు IPP_DISABLE_REDUCE 1
#నిర్వచించు IPP_DISABLE_క్రమీకరించు   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

శూన్య cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.అడ్డువరుసలు, b.అడ్డువరుసలు);
    std::swap(a.నిలువుల_సంఖ్య, b.నిలువుల_సంఖ్య);
    std::swap(a.దత్తాంశం, b.దత్తాంశం);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.అడుగు, b.అడుగు);
}


శూన్య cv::hconcat(స్థిరరాశి Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( nsrc == 0 || !src )
    {
        _dst.release();
        తిరిగి;
    }

    పూర్ణాంకం మొత్తం_నిలువుల_సంఖ్య = 0, నిలువుల_సంఖ్య = 0;
    కొరకు( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].అడ్డువరుసలు == src[0].అడ్డువరుసలు &&
                   src[i].type() == src[0].type());
        మొత్తం_నిలువుల_సంఖ్య += src[i].నిలువుల_సంఖ్య;
    }
    _dst.create( src[0].అడ్డువరుసలు, మొత్తం_నిలువుల_సంఖ్య, src[0].type());
    Mat dst = _dst.getMat();
    కొరకు( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(నిలువుల_సంఖ్య, 0, src[i].నిలువుల_సంఖ్య, src[i].అడ్డువరుసలు));
        src[i].copy_గమ్యం(dpart);
        నిలువుల_సంఖ్య += src[i].నిలువుల_సంఖ్య;
    }
}

శూన్య cv::hconcat(ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

శూన్య cv::hconcat(ఆదానం_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_సదిశ(src);
    hconcat(!src.empty() ? &src[0] : శూన్యసూచిక, src.size(), dst);
}

శూన్య cv::vconcat(స్థిరరాశి Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    ఒకవేళ( nsrc == 0 || !src )
    {
        _dst.release();
        తిరిగి;
    }

    పూర్ణాంకం మొత్తం_అడ్డువరుసలు = 0, అడ్డువరుసలు = 0;
    కొరకు( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].నిలువుల_సంఖ్య == src[0].నిలువుల_సంఖ్య &&
                  src[i].type() == src[0].type());
        మొత్తం_అడ్డువరుసలు += src[i].అడ్డువరుసలు;
    }
    _dst.create( మొత్తం_అడ్డువరుసలు, src[0].నిలువుల_సంఖ్య, src[0].type());
    Mat dst = _dst.getMat();
    కొరకు( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, అడ్డువరుసలు, src[i].నిలువుల_సంఖ్య, src[i].అడ్డువరుసలు));
        src[i].copy_గమ్యం(dpart);
        అడ్డువరుసలు += src[i].అడ్డువరుసలు;
    }
}

శూన్య cv::vconcat(ఆదానం_Array src1, ఆదానం_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

శూన్య cv::vconcat(ఆదానం_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_సదిశ(src);
    vconcat(!src.empty() ? &src[0] : శూన్యసూచిక, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#నిర్వచితమైతే HAVE_OPENCL

నామాకాశ cv {

స్థిర తార్కిక ocl_సమితి_తటస్థం( ఆదానం_Output_Array _m, స్థిరరాశి Scalar& s )
{
    పూర్ణాంకం type = _m.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type), kercn = cn, అడ్డువరుసలు_చొప్పున_WI = 1;
    పూర్ణాంకం sctype = CV_MAKE_TYPE(లోతు, cn == 3 ? 4 : cn);
    ఒకవేళ (ocl::Device::getDefault().isIntel())
    {
        అడ్డువరుసలు_చొప్పున_WI = 4;
        ఒకవేళ (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_సదిశ_వెడల్పు(_m), 4);
            ఒకవేళ (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::సమితి_తటస్థం_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)),
                         ocl::memop_Type_గమ్యం_Str(లోతు), cn,
                         ocl::memop_Type_గమ్యం_Str(sctype),
                         kercn, అడ్డువరుసలు_చొప్పున_WI));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat m = _m.getUMat();
    k.వాదనలు(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.నిలువుల_సంఖ్య * cn / kercn, ((size_t)m.అడ్డువరుసలు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, శూన్యసూచిక, అబద్ధం);
}

}

#అయితే_ముగింపు

శూన్య cv::సమితి_తటస్థం( ఆదానం_Output_Array _m, స్థిరరాశి Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_నిరంతరం(_m.isUMat(),
               ocl_సమితి_తటస్థం(_m, s))

    Mat m = _m.getMat();
    పూర్ణాంకం అడ్డువరుసలు = m.అడ్డువరుసలు, నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య, type = m.type();

    ఒకవేళ( type == CV_32FC1 )
    {
        దశాంశం* దత్తాంశం = m.ptr<దశాంశం>();
        దశాంశం val = (దశాంశం)s[0];
        size_t అడుగు = m.అడుగు/పరిమాణం(దత్తాంశం[0]);

        కొరకు( పూర్ణాంకం i = 0; i < అడ్డువరుసలు; i++, దత్తాంశం += అడుగు )
        {
            కొరకు( పూర్ణాంకం j = 0; j < నిలువుల_సంఖ్య; j++ )
                దత్తాంశం[j] = 0;
            ఒకవేళ( i < నిలువుల_సంఖ్య )
                దత్తాంశం[i] = val;
        }
    }
    లేకపోతే ఒకవేళ( type == CV_64FC1 )
    {
        ద్విగుణ* దత్తాంశం = m.ptr<ద్విగుణ>();
        ద్విగుణ val = s[0];
        size_t అడుగు = m.అడుగు/పరిమాణం(దత్తాంశం[0]);

        కొరకు( పూర్ణాంకం i = 0; i < అడ్డువరుసలు; i++, దత్తాంశం += అడుగు )
        {
            std::fill(దత్తాంశం, దత్తాంశం + నిలువుల_సంఖ్య, 0.0);
            ఒకవేళ (i < నిలువుల_సంఖ్య)
                దత్తాంశం[i] = val;
        }
    }
    లేకపోతే
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


నామాకాశ cv {

UMat UMat::eye(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    తిరిగి UMat::eye(కొలత(నిలువుల_సంఖ్య, అడ్డువరుసలు), type, usageFlags);
}

UMat UMat::eye(కొలత size, పూర్ణాంకం type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    సమితి_తటస్థం(m);
    తిరిగి m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( ఆదానం_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    పూర్ణాంకం type = m.type();
    పూర్ణాంకం nm = std::min(m.అడ్డువరుసలు, m.నిలువుల_సంఖ్య);

    ఒకవేళ( type == CV_32FC1 )
    {
        స్థిరరాశి దశాంశం* ptr = m.ptr<దశాంశం>();
        size_t అడుగు = m.అడుగు/పరిమాణం(ptr[0]) + 1;
        ద్విగుణ _s = 0;
        కొరకు( పూర్ణాంకం i = 0; i < nm; i++ )
            _s += ptr[i*అడుగు];
        తిరిగి _s;
    }

    ఒకవేళ( type == CV_64FC1 )
    {
        స్థిరరాశి ద్విగుణ* ptr = m.ptr<ద్విగుణ>();
        size_t అడుగు = m.అడుగు/పరిమాణం(ptr[0]) + 1;
        ద్విగుణ _s = 0;
        కొరకు( పూర్ణాంకం i = 0; i < nm; i++ )
            _s += ptr[i*అడుగు];
        తిరిగి _s;
    }

    తిరిగి cv::కూడిక(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

శూన్య cv::completeSymm( ఆదానం_Output_Array _m, తార్కిక LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t అడుగు = m.అడుగు, esz = m.elem_కొలత();
    CV_Assert( m.dims <= 2 && m.అడ్డువరుసలు == m.నిలువుల_సంఖ్య );

    పూర్ణాంకం అడ్డువరుసలు = m.అడ్డువరుసలు;
    పూర్ణాంకం j0 = 0, j1 = అడ్డువరుసలు;

    uchar* దత్తాంశం = m.ptr();
    కొరకు( పూర్ణాంకం i = 0; i < అడ్డువరుసలు; i++ )
    {
        ఒకవేళ( !LtoR ) j1 = i; లేకపోతే j0 = i+1;
        కొరకు( పూర్ణాంకం j = j0; j < j1; j++ )
            జ్ఞాపకం_నకలు(దత్తాంశం + (i*అడుగు + j*esz), దత్తాంశం + (j*అడుగు + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(ఆదానం_Array _m) స్థిరరాశి
{
    Mat m = _m.getMat();
    పూర్ణాంకం tp = type(), d = CV_MAT_లోతు(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((అడ్డువరుసలు == 3 && నిలువుల_సంఖ్య == 1) || (నిలువుల_సంఖ్య*channels() == 3 && అడ్డువరుసలు == 1)));
    Mat ఫలితం(అడ్డువరుసలు, నిలువుల_సంఖ్య, tp);

    ఒకవేళ( d == CV_32F )
    {
        స్థిరరాశి దశాంశం *a = (స్థిరరాశి దశాంశం*)దత్తాంశం, *b = (స్థిరరాశి దశాంశం*)m.దత్తాంశం;
        దశాంశం* c = (దశాంశం*)ఫలితం.దత్తాంశం;
        size_t lda = అడ్డువరుసలు > 1 ? అడుగు/పరిమాణం(a[0]) : 1;
        size_t ldb = అడ్డువరుసలు > 1 ? m.అడుగు/పరిమాణం(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    లేకపోతే ఒకవేళ( d == CV_64F )
    {
        స్థిరరాశి ద్విగుణ *a = (స్థిరరాశి ద్విగుణ*)దత్తాంశం, *b = (స్థిరరాశి ద్విగుణ*)m.దత్తాంశం;
        ద్విగుణ* c = (ద్విగుణ*)ఫలితం.దత్తాంశం;
        size_t lda = అడ్డువరుసలు > 1 ? అడుగు/పరిమాణం(a[0]) : 1;
        size_t ldb = అడ్డువరుసలు > 1 ? m.అడుగు/పరిమాణం(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    తిరిగి ఫలితం;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

నామాకాశ cv
{

రకనిర్వచనం శూన్య (*Reduce_కూడిక_Func)(స్థిరరాశి Mat& src, Mat& dst);
Reduce_కూడిక_Func getReduceCSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);
Reduce_కూడిక_Func getReduceRSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);
Reduce_కూడిక_Func getReduceRSum2Func(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);

మూస <రకం_పేరు T, రకం_పేరు WT, రకం_పేరు Op>
నిర్మాణం ReduceR_SIMD
{
    పూర్ణాంకం క్రియాకారకం()(స్థిరరాశి T*, పూర్ణాంకం ప్రారంభం, పూర్ణాంకం, WT*, స్థిరరాశి Op&) స్థిరరాశి
    {
        తిరిగి ప్రారంభం;
    }
};


మూస<రకం_పేరు T, రకం_పేరు ST, రకం_పేరు WT, తరగతి Op, తరగతి OpInit>
తరగతి ReduceR_Invoker : బహిరంగ ParallelLoopBody
{
బహిరంగ:
  ReduceR_Invoker(స్థిరరాశి Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),బఫరు(srcmat.size().వెడల్పు*srcmat.channels())
  {
  }
  శూన్య క్రియాకారకం()(స్థిరరాశి Range& range) స్థిరరాశి CV_OVERRIDE
  {
    స్థిరరాశి T* src = srcmat.ptr<T>();
    స్థిరరాశి size_t srcstep = srcmat.అడుగు/పరిమాణం(src[0]);
    WT* buf = బఫరు.దత్తాంశం();
    ST* dst = dstmat.ptr<ST>();
    పూర్ణాంకం i = 0;

    కొరకు( i = range.ప్రారంభం ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    పూర్ణాంకం ఎత్తు = srcmat.size().ఎత్తు;
    కొరకు( ; --ఎత్తు; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.ప్రారంభం, range.end, buf, op);
        #ఒకవేళ CV_ENABLE_UNROLLED
        కొరకు(; i <= range.end - 4; i += 4 )
        {
            WT s0, s1;
            s0 = op(buf[i], (WT)src[i]);
            s1 = op(buf[i+1], (WT)src[i+1]);
            buf[i] = s0; buf[i+1] = s1;

            s0 = op(buf[i+2], (WT)src[i+2]);
            s1 = op(buf[i+3], (WT)src[i+3]);
            buf[i+2] = s0; buf[i+3] = s1;
        }
        #అయితే_ముగింపు
        కొరకు( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    కొరకు( i = range.ప్రారంభం ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
అంతరంగ:
  స్థిరరాశి Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  మార్చదగిన Auto_బఫరు<WT> బఫరు;
};

మూస<రకం_పేరు T, రకం_పేరు ST, తరగతి Op, తరగతి OpInit = OpNop<ST> > స్థిర శూన్య
reduceR_( స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    రకనిర్వచనం రకం_పేరు Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().వెడల్పు*srcmat.channels()), body, srcmat.size().వెడల్పు*CV_ELEM_కొలత(srcmat.లోతు())/64);
}

మూస<రకం_పేరు T, రకం_పేరు ST, రకం_పేరు WT, తరగతి Op, తరగతి OpInit>
తరగతి ReduceC_Invoker : బహిరంగ ParallelLoopBody
{
బహిరంగ:
  ReduceC_Invoker(స్థిరరాశి Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  శూన్య క్రియాకారకం()(స్థిరరాశి Range& range) స్థిరరాశి CV_OVERRIDE
  {
    స్థిరరాశి పూర్ణాంకం cn = srcmat.channels();
    స్థిరరాశి పూర్ణాంకం వెడల్పు = srcmat.size().వెడల్పు*cn;
    Auto_బఫరు<WT> cumul(cn);
    కొరకు( పూర్ణాంకం y = range.ప్రారంభం; y < range.end; y++ )
    {
        స్థిరరాశి T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        ఒకవేళ( వెడల్పు == cn )
        {
          కొరకు( పూర్ణాంకం k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        లేకపోతే
        {
            కొరకు(పూర్ణాంకం k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            కొరకు(పూర్ణాంకం k = cn ; k < వెడల్పు ; k += cn )
            {
                కొరకు (పూర్ణాంకం c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            కొరకు(పూర్ణాంకం k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
అంతరంగ:
  స్థిరరాశి Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

మూస<రకం_పేరు T, రకం_పేరు ST, తరగతి Op, తరగతి OpInit = OpNop<ST> > స్థిర శూన్య
reduceC_( స్థిరరాశి Mat& srcmat, Mat& dstmat)
{
    రకనిర్వచనం రకం_పేరు Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().ఎత్తు), body);
}

రకనిర్వచనం శూన్య (*ReduceFunc)( స్థిరరాశి Mat& src, Mat& dst );

}

#నిర్వచించు reduce_కూడిక_R8u32s  reduceR_<uchar, పూర్ణాంకం,   Op_కూడు<పూర్ణాంకం>, OpNop<పూర్ణాంకం> >
#నిర్వచించు reduce_కూడిక_R8u32f  reduceR_<uchar, దశాంశం, Op_కూడు<పూర్ణాంకం>, OpNop<పూర్ణాంకం> >
#నిర్వచించు reduce_కూడిక_R8u64f  reduceR_<uchar, ద్విగుణ,Op_కూడు<పూర్ణాంకం>, OpNop<పూర్ణాంకం> >
#నిర్వచించు reduce_కూడిక_R16u32f reduceR_<ushort,దశాంశం, Op_కూడు<దశాంశం> >
#నిర్వచించు reduce_కూడిక_R16u64f reduceR_<ushort,ద్విగుణ,Op_కూడు<ద్విగుణ> >
#నిర్వచించు reduce_కూడిక_R16s32f reduceR_<పొట్టి, దశాంశం, Op_కూడు<దశాంశం> >
#నిర్వచించు reduce_కూడిక_R16s64f reduceR_<పొట్టి, ద్విగుణ,Op_కూడు<ద్విగుణ> >
#నిర్వచించు reduce_కూడిక_R32f32f reduceR_<దశాంశం, దశాంశం, Op_కూడు<దశాంశం> >
#నిర్వచించు reduce_కూడిక_R32f64f reduceR_<దశాంశం, ద్విగుణ,Op_కూడు<ద్విగుణ> >
#నిర్వచించు reduce_కూడిక_R64f64f reduceR_<ద్విగుణ,ద్విగుణ,Op_కూడు<ద్విగుణ> >

#నిర్వచించు reduceSum2R8u32s  reduceR_<uchar, పూర్ణాంకం,   Op_కూడు_Sqr<పూర్ణాంకం>,   OpSqr<పూర్ణాంకం> >
#నిర్వచించు reduceSum2R8u32f  reduceR_<uchar, దశాంశం, Op_కూడు_Sqr<పూర్ణాంకం>,   OpSqr<పూర్ణాంకం> >
#నిర్వచించు reduceSum2R8u64f  reduceR_<uchar, ద్విగుణ,Op_కూడు_Sqr<పూర్ణాంకం>,   OpSqr<పూర్ణాంకం> >
#నిర్వచించు reduceSum2R16u32f reduceR_<ushort,దశాంశం, Op_కూడు_Sqr<దశాంశం>, OpSqr<దశాంశం> >
#నిర్వచించు reduceSum2R16u64f reduceR_<ushort,ద్విగుణ,Op_కూడు_Sqr<ద్విగుణ>,OpSqr<ద్విగుణ> >
#నిర్వచించు reduceSum2R16s32f reduceR_<పొట్టి, దశాంశం, Op_కూడు_Sqr<దశాంశం>, OpSqr<దశాంశం> >
#నిర్వచించు reduceSum2R16s64f reduceR_<పొట్టి, ద్విగుణ,Op_కూడు_Sqr<ద్విగుణ>,OpSqr<ద్విగుణ> >
#నిర్వచించు reduceSum2R32f32f reduceR_<దశాంశం, దశాంశం, Op_కూడు_Sqr<దశాంశం>, OpSqr<దశాంశం> >
#నిర్వచించు reduceSum2R32f64f reduceR_<దశాంశం, ద్విగుణ,Op_కూడు_Sqr<ద్విగుణ>,OpSqr<ద్విగుణ> >
#నిర్వచించు reduceSum2R64f64f reduceR_<ద్విగుణ,ద్విగుణ,Op_కూడు_Sqr<ద్విగుణ>,OpSqr<ద్విగుణ> >

#నిర్వచించు reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#నిర్వచించు reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#నిర్వచించు reduceMaxR16s reduceR_<పొట్టి, పొట్టి, OpMax<పొట్టి> >
#నిర్వచించు reduceMaxR32f reduceR_<దశాంశం, దశాంశం, OpMax<దశాంశం> >
#నిర్వచించు reduceMaxR64f reduceR_<ద్విగుణ,ద్విగుణ,OpMax<ద్విగుణ> >

#నిర్వచించు reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#నిర్వచించు reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#నిర్వచించు reduceMinR16s reduceR_<పొట్టి, పొట్టి, OpMin<పొట్టి> >
#నిర్వచించు reduceMinR32f reduceR_<దశాంశం, దశాంశం, OpMin<దశాంశం> >
#నిర్వచించు reduceMinR64f reduceR_<ద్విగుణ,ద్విగుణ,OpMin<ద్విగుణ> >

#ఒకవేళ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
స్థిర అంతఃవరుస తార్కిక ipp_reduce_కూడిక_C_8u16u16s32f_64f(స్థిరరాశి cv::Mat& srcmat, cv::Mat& dstmat)
{
    పూర్ణాంకం sstep = (పూర్ణాంకం)srcmat.అడుగు, stype = srcmat.type(),
            ddepth = dstmat.లోతు();

    Ippi_కొలత roisize = { srcmat.size().వెడల్పు, 1 };

    రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * Ippi_కూడిక)(స్థిరరాశి శూన్య * pSrc, పూర్ణాంకం src_అడుగు_3, Ippi_కొలత roi_కొలత, Ipp64f* p_కూడిక);
    రకనిర్వచనం Ipp_స్థితి (CV_STDCALL * Ippi_కూడిక_Hint)(స్థిరరాశి శూన్య * pSrc, పూర్ణాంకం src_అడుగు_3, Ippi_కొలత roi_కొలత, Ipp64f* p_కూడిక, IppHintAlgorithm hint);
    Ippi_కూడిక ippi_కూడిక = 0;
    Ippi_కూడిక_Hint ippi_కూడిక_Hint = 0;

    ఒకవేళ(ddepth == CV_64F)
    {
        ippi_కూడిక =
            stype == CV_8UC1 ? (Ippi_కూడిక)ippi_కూడిక_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_కూడిక)ippi_కూడిక_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_కూడిక)ippi_కూడిక_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_కూడిక)ippi_కూడిక_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_కూడిక)ippi_కూడిక_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_కూడిక)ippi_కూడిక_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_కూడిక)ippi_కూడిక_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_కూడిక)ippi_కూడిక_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_కూడిక)ippi_కూడిక_16s_C4_R : 0;
        ippi_కూడిక_Hint =
            stype == CV_32FC1 ? (Ippi_కూడిక_Hint)ippi_కూడిక_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_కూడిక_Hint)ippi_కూడిక_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_కూడిక_Hint)ippi_కూడిక_32f_C4_R : 0;
    }

    ఒకవేళ(ippi_కూడిక)
    {
        కొరకు(పూర్ణాంకం y = 0; y < srcmat.size().ఎత్తు; y++)
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(ippi_కూడిక, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                తిరిగి అబద్ధం;
        }
        తిరిగి నిజం;
    }
    లేకపోతే ఒకవేళ(ippi_కూడిక_Hint)
    {
        కొరకు(పూర్ణాంకం y = 0; y < srcmat.size().ఎత్తు; y++)
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(ippi_కూడిక_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                తిరిగి అబద్ధం;
        }
        తిరిగి నిజం;
    }

    తిరిగి అబద్ధం;
}

స్థిర అంతఃవరుస శూన్య reduce_కూడిక_C_8u16u16s32f_64f(స్థిరరాశి cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_నిరంతరం_FAST(ipp_reduce_కూడిక_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    ఒకవేళ(dstmat.లోతు() == CV_64F)
    {
        పూర్ణాంకం sdepth = CV_MAT_లోతు(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, ద్విగుణ,   cv::Op_కూడు<ద్విగుణ> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, ద్విగుణ,   cv::Op_కూడు<ద్విగుణ> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<పొట్టి, ద్విగుణ,   cv::Op_కూడు<ద్విగుణ> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<దశాంశం, ద్విగుణ,   cv::Op_కూడు<ద్విగుణ> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#అయితే_ముగింపు

#నిర్వచించు reduce_కూడిక_C8u32s  reduceC_<uchar, పూర్ణాంకం,   Op_కూడు<పూర్ణాంకం>, OpNop<పూర్ణాంకం> >
#నిర్వచించు reduce_కూడిక_C8u32f  reduceC_<uchar, దశాంశం, Op_కూడు<పూర్ణాంకం>, OpNop<పూర్ణాంకం> >
#నిర్వచించు reduce_కూడిక_C16u32f reduceC_<ushort,దశాంశం, Op_కూడు<దశాంశం> >
#నిర్వచించు reduce_కూడిక_C16s32f reduceC_<పొట్టి, దశాంశం, Op_కూడు<దశాంశం> >
#నిర్వచించు reduce_కూడిక_C32f32f reduceC_<దశాంశం, దశాంశం, Op_కూడు<దశాంశం> >
#నిర్వచించు reduce_కూడిక_C64f64f reduceC_<ద్విగుణ,ద్విగుణ,Op_కూడు<ద్విగుణ> >

#నిర్వచించు reduceSum2C8u32s  reduceC_<uchar, పూర్ణాంకం,   Op_కూడు_Sqr<పూర్ణాంకం>,   OpSqr<పూర్ణాంకం> >
#నిర్వచించు reduceSum2C8u32f  reduceC_<uchar, దశాంశం, Op_కూడు_Sqr<పూర్ణాంకం>,   OpSqr<పూర్ణాంకం> >
#నిర్వచించు reduceSum2C16u32f reduceC_<ushort,దశాంశం, Op_కూడు_Sqr<దశాంశం>, OpSqr<దశాంశం> >
#నిర్వచించు reduceSum2C16s32f reduceC_<పొట్టి, దశాంశం, Op_కూడు_Sqr<దశాంశం>, OpSqr<దశాంశం> >
#నిర్వచించు reduceSum2C32f32f reduceC_<దశాంశం, దశాంశం, Op_కూడు_Sqr<దశాంశం>, OpSqr<దశాంశం> >
#నిర్వచించు reduceSum2C64f64f reduceC_<ద్విగుణ,ద్విగుణ,Op_కూడు_Sqr<ద్విగుణ>,OpSqr<ద్విగుణ> >

#ఒకవేళ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#నిర్వచించు reduce_కూడిక_C8u64f  reduce_కూడిక_C_8u16u16s32f_64f
#నిర్వచించు reduce_కూడిక_C16u64f reduce_కూడిక_C_8u16u16s32f_64f
#నిర్వచించు reduce_కూడిక_C16s64f reduce_కూడిక_C_8u16u16s32f_64f
#నిర్వచించు reduce_కూడిక_C32f64f reduce_కూడిక_C_8u16u16s32f_64f
#లేకపోతే
#నిర్వచించు reduce_కూడిక_C8u64f  reduceC_<uchar, ద్విగుణ,Op_కూడు<పూర్ణాంకం>, OpNop<పూర్ణాంకం> >
#నిర్వచించు reduce_కూడిక_C16u64f reduceC_<ushort,ద్విగుణ,Op_కూడు<ద్విగుణ> >
#నిర్వచించు reduce_కూడిక_C16s64f reduceC_<పొట్టి, ద్విగుణ,Op_కూడు<ద్విగుణ> >
#నిర్వచించు reduce_కూడిక_C32f64f reduceC_<దశాంశం, ద్విగుణ,Op_కూడు<ద్విగుణ> >
#అయితే_ముగింపు

#నిర్వచించు reduceSum2C8u64f  reduceC_<uchar, ద్విగుణ,Op_కూడు_Sqr<పూర్ణాంకం>,   OpSqr<పూర్ణాంకం> >
#నిర్వచించు reduceSum2C16u64f reduceC_<ushort,ద్విగుణ,Op_కూడు_Sqr<ద్విగుణ>,OpSqr<ద్విగుణ> >
#నిర్వచించు reduceSum2C16s64f reduceC_<పొట్టి, ద్విగుణ,Op_కూడు_Sqr<ద్విగుణ>,OpSqr<ద్విగుణ> >
#నిర్వచించు reduceSum2C32f64f reduceC_<దశాంశం, ద్విగుణ,Op_కూడు_Sqr<ద్విగుణ>,OpSqr<ద్విగుణ> >

#ఒకవేళ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#నిర్వచించు REDUCE_OP(favor, optype, type1, type2) \
స్థిర అంతఃవరుస తార్కిక ipp_reduce##optype##C##favor(స్థిరరాశి cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    ఒకవేళ((srcmat.channels() == 1)) \
    { \
        పూర్ణాంకం sstep = (పూర్ణాంకం)srcmat.అడుగు; \
        రకనిర్వచనం Ipp##favor IppType; \
        Ippi_కొలత roisize = ippi_కొలత(srcmat.size().వెడల్పు, 1);\
        కొరకు(పూర్ణాంకం y = 0; y < srcmat.size().ఎత్తు; y++)\
        {\
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                తిరిగి అబద్ధం;\
        }\
        తిరిగి నిజం;\
    }\
    తిరిగి అబద్ధం; \
} \
స్థిర అంతఃవరుస శూన్య reduce##optype##C##favor(స్థిరరాశి cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_నిరంతరం_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, పొట్టి, పొట్టి)
REDUCE_OP(32f, Max, దశాంశం, దశాంశం)
#లేకపోతే
#నిర్వచించు reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#నిర్వచించు reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#నిర్వచించు reduceMaxC16s reduceC_<పొట్టి, పొట్టి, OpMax<పొట్టి> >
#నిర్వచించు reduceMaxC32f reduceC_<దశాంశం, దశాంశం, OpMax<దశాంశం> >
#అయితే_ముగింపు
#నిర్వచించు reduceMaxC64f reduceC_<ద్విగుణ,ద్విగుణ,OpMax<ద్విగుణ> >

#ఒకవేళ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, పొట్టి, పొట్టి)
REDUCE_OP(32f, Min, దశాంశం, దశాంశం)
#లేకపోతే
#నిర్వచించు reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#నిర్వచించు reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#నిర్వచించు reduceMinC16s reduceC_<పొట్టి, పొట్టి, OpMin<పొట్టి> >
#నిర్వచించు reduceMinC32f reduceC_<దశాంశం, దశాంశం, OpMin<దశాంశం> >
#అయితే_ముగింపు
#నిర్వచించు reduceMinC64f reduceC_<ద్విగుణ,ద్విగుణ,OpMin<ద్విగుణ> >

#నిర్వచితమైతే HAVE_OPENCL

నామాకాశ cv {

స్థిర తార్కిక ocl_reduce(ఆదానం_Array _src, OutputArray _dst,
                       పూర్ణాంకం dim, పూర్ణాంకం op, పూర్ణాంకం op0, పూర్ణాంకం stype, పూర్ణాంకం dtype)
{
    స్థిరరాశి పూర్ణాంకం min_opt_నిలువుల_సంఖ్య = 128, buf_నిలువుల_సంఖ్య = 32;
    పూర్ణాంకం sdepth = CV_MAT_లోతు(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_లోతు(dtype), ddepth0 = ddepth;
    స్థిరరాశి ocl::Device &defDev = ocl::Device::getDefault();
    తార్కిక doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_సమూహం_కొలత();
    తార్కిక useOptimized = 1 == dim && _src.నిలువుల_సంఖ్య() > min_opt_నిలువుల_సంఖ్య && (wgs >= buf_నిలువుల_సంఖ్య);

    ఒకవేళ (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        తిరిగి అబద్ధం;

    ఒకవేళ (op == REDUCE_AVG)
    {
        ఒకవేళ (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    స్థిరరాశి అక్షరం * స్థిరరాశి ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    పూర్ణాంకం wdepth = std::max(ddepth, CV_32F);
    ఒకవేళ (useOptimized)
    {
        size_t tile_ఎత్తు = (size_t)(wgs / buf_నిలువుల_సంఖ్య);
        ఒకవేళ (defDev.isIntel())
        {
            స్థిర స్థిరరాశి size_t max_అంశం_లోపల_సమూహం_లెక్క = 16;
            tile_ఎత్తు = min(tile_ఎత్తు, defDev.local_Mem_కొలత() / buf_నిలువుల_సంఖ్య / CV_ELEM_కొలత(CV_MAKETYPE(wdepth, cn)) / max_అంశం_లోపల_సమూహం_లెక్క);
        }
        అక్షరం cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_నిలువుల_సంఖ్య, tile_ఎత్తు, ops[op], cn, ddepth,
                                            ocl::type_గమ్యం_Str(sdepth),
                                            ocl::type_గమ్యం_Str(ddepth),
                                            ocl::type_గమ్యం_Str(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], పరిమాణం(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], పరిమాణం(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], పరిమాణం(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        ఒకవేళ (k.empty())
            తిరిగి అబద్ధం;
        UMat src = _src.getUMat();
        కొలత dsize(1, src.అడ్డువరుసలు);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ఒకవేళ (op0 == REDUCE_AVG)
            k.వాదనలు(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_కొలత(dst), 1.0f / src.నిలువుల_సంఖ్య);
        లేకపోతే
            k.వాదనలు(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_కొలత(dst));

        size_t local_కొలత[2] = { (size_t)buf_నిలువుల_సంఖ్య, (size_t)tile_ఎత్తు};
        size_t global_కొలత[2] = { (size_t)buf_నిలువుల_సంఖ్య, (size_t)src.అడ్డువరుసలు };
        తిరిగి k.నిరంతరం(2, global_కొలత, local_కొలత, అబద్ధం);
    }
    లేకపోతే
    {
        అక్షరం cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::type_గమ్యం_Str(useOptimized ? ddepth : sdepth),
                                      ocl::type_గమ్యం_Str(ddepth), ocl::type_గమ్యం_Str(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], పరిమాణం(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], పరిమాణం(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], పరిమాణం(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        ఒకవేళ (k.empty())
            తిరిగి అబద్ధం;

        UMat src = _src.getUMat();
        కొలత dsize(dim == 0 ? src.నిలువుల_సంఖ్య : 1, dim == 0 ? 1 : src.అడ్డువరుసలు);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_కొలత(dst);

        ఒకవేళ (op0 == REDUCE_AVG)
            k.వాదనలు(srcarg, temparg, 1.0f / (dim == 0 ? src.అడ్డువరుసలు : src.నిలువుల_సంఖ్య));
        లేకపోతే
            k.వాదనలు(srcarg, temparg);

        size_t globalsize = std::max(dsize.వెడల్పు, dsize.ఎత్తు);
        తిరిగి k.నిరంతరం(1, &globalsize, ఖాళీ, అబద్ధం);
    }
}

}

#అయితే_ముగింపు

శూన్య cv::reduce(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం dim, పూర్ణాంకం op, పూర్ణాంకం dtype)
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, నిజం, dtype < 0 && _dst.fixedType() ? _dst.లోతు() : dtype));
        తిరిగి;
    }
    పూర్ణాంకం op0 = op;
    పూర్ణాంకం stype = _src.type(), sdepth = CV_MAT_లోతు(stype), cn = CV_MAT_CN(stype);
    ఒకవేళ( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    పూర్ణాంకం ddepth = CV_MAT_లోతు(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_కూడిక || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_నిరంతరం(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    ఒకవేళ (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    ఒకవేళ (src.empty()) {
        src.convert_గమ్యం(_dst, dtype);
        తిరిగి;
    }
    ఒకవేళ (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.అడ్డువరుసలు, dim == 0 ? src.నిలువుల_సంఖ్య : 1, dtype);
    Mat dst = _dst.getMat(), తాత్కాలిక_2 = dst;

    ఒకవేళ( op == REDUCE_AVG )
    {
        op = REDUCE_కూడిక;
        ఒకవేళ( sdepth < CV_32S && ddepth < CV_32S )
        {
            తాత్కాలిక_2.create(dst.అడ్డువరుసలు, dst.నిలువుల_సంఖ్య, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    ఒకవేళ( dim == 0 )
    {
        ఒకవేళ( op == REDUCE_కూడిక )
        {
            Reduce_కూడిక_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            ఒకవేళ(simd_func)
                func = (ReduceFunc)simd_func;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_కూడిక_R8u32s;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_కూడిక_R8u32f;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_కూడిక_R8u64f;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_కూడిక_R16u32f;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_కూడిక_R16u64f;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_కూడిక_R16s32f;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_కూడిక_R16s64f;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_కూడిక_R32f32f;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_కూడిక_R32f64f;
            లేకపోతే ఒకవేళ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_కూడిక_R64f64f;
        }
        లేకపోతే ఒకవేళ(op == REDUCE_MAX)
        {
            ఒకవేళ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            లేకపోతే ఒకవేళ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        లేకపోతే ఒకవేళ(op == REDUCE_MIN)
        {
            ఒకవేళ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            లేకపోతే ఒకవేళ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        లేకపోతే ఒకవేళ( op == REDUCE_SUM2 )
        {
            Reduce_కూడిక_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
            ఒకవేళ(simd_func)
                func = simd_func;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            లేకపోతే ఒకవేళ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    లేకపోతే
    {
        ఒకవేళ(op == REDUCE_కూడిక)
        {
            Reduce_కూడిక_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            ఒకవేళ(simd_func)
                func = (ReduceFunc)simd_func;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_కూడిక_C8u32s;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_కూడిక_C8u32f;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_కూడిక_C8u64f;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_కూడిక_C16u32f;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_కూడిక_C16u64f;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_కూడిక_C16s32f;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_కూడిక_C16s64f;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_కూడిక_C32f32f;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_కూడిక_C32f64f;
            లేకపోతే ఒకవేళ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_కూడిక_C64f64f;
        }
        లేకపోతే ఒకవేళ(op == REDUCE_MAX)
        {
            ఒకవేళ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            లేకపోతే ఒకవేళ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        లేకపోతే ఒకవేళ(op == REDUCE_MIN)
        {
            ఒకవేళ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            లేకపోతే ఒకవేళ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        లేకపోతే ఒకవేళ(op == REDUCE_SUM2)
        {
            ఒకవేళ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            లేకపోతే ఒకవేళ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            లేకపోతే ఒకవేళ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            లేకపోతే ఒకవేళ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            లేకపోతే ఒకవేళ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            లేకపోతే ఒకవేళ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    ఒకవేళ( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, తాత్కాలిక_2 );

    ఒకవేళ( op0 == REDUCE_AVG )
        తాత్కాలిక_2.convert_గమ్యం(dst, dst.type(), 1./(dim == 0 ? src.అడ్డువరుసలు : src.నిలువుల_సంఖ్య));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

నామాకాశ cv
{

మూస<రకం_పేరు T> స్థిర శూన్య క్రమీకరించు( స్థిరరాశి Mat& src, Mat& dst, పూర్ణాంకం flags )
{
    పూర్ణాంకం n, len;
    తార్కిక క్రమీకరించు_అడ్డువరుసలు = (flags & 1) == క్రమీకరించు_EVERY_అడ్డువరుస;
    తార్కిక inplace = src.దత్తాంశం == dst.దత్తాంశం;
    తార్కిక క్రమీకరించు_Descending = (flags & క్రమీకరించు_DESCENDING) != 0;

    ఒకవేళ( క్రమీకరించు_అడ్డువరుసలు )
        n = src.అడ్డువరుసలు, len = src.నిలువుల_సంఖ్య;
    లేకపోతే
        n = src.నిలువుల_సంఖ్య, len = src.అడ్డువరుసలు;
    parallel_for_(Range(0, n), [&](స్థిరరాశి Range& range)
    {
        Auto_బఫరు<T> buf;
        ఒకవేళ( !క్రమీకరించు_అడ్డువరుసలు )
            buf.allocate(len);
        T* bptr = buf.దత్తాంశం();

        కొరకు( పూర్ణాంకం i = range.ప్రారంభం; i < range.end; i++ )
        {
            T* ptr = bptr;
            ఒకవేళ( క్రమీకరించు_అడ్డువరుసలు )
            {
                T* dptr = dst.ptr<T>(i);
                ఒకవేళ( !inplace )
                {
                    స్థిరరాశి T* sptr = src.ptr<T>(i);
                    జ్ఞాపకం_నకలు(dptr, sptr, పరిమాణం(T) * len);
                }
                ptr = dptr;
            }
            లేకపోతే
            {
                కొరకు( పూర్ణాంకం j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            ఒకవేళ( క్రమీకరించు_Descending )
            {
                కొరకు( పూర్ణాంకం j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            ఒకవేళ( !క్రమీకరించు_అడ్డువరుసలు )
                కొరకు( పూర్ణాంకం j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#ఒకవేళ defined(HAVE_IPP) && !IPP_DISABLE_క్రమీకరించు
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL *Ipp_క్రమీకరించు_Func)(శూన్య  *pSrcDst, పూర్ణాంకం    len, Ipp8u *p_బఫరు);

మూస<రకం_పేరు T, Ipp_స్థితి (CV_STDCALL *fn)(T*, పూర్ణాంకం, Ipp8u*)>
స్థిర Ipp_స్థితి CV_STDCALL ipp_క్రమీకరించు_Wrap(శూన్య* pSrcDst, పూర్ణాంకం len, Ipp8u* p_బఫరు)
{
    తిరిగి fn((T*)pSrcDst, len, p_బఫరు);
}

స్థిర Ipp_క్రమీకరించు_Func get_క్రమీకరించు_Func(పూర్ణాంకం లోతు, తార్కిక క్రమీకరించు_Descending)
{
    ఒకవేళ (!క్రమీకరించు_Descending)
        తిరిగి లోతు == CV_8U ? ipp_క్రమీకరించు_Wrap<Ipp8u, ipps_క్రమీకరించు_Radix_Ascend_8u_I> :
            లోతు == CV_16U ? ipp_క్రమీకరించు_Wrap<Ipp16u, ipps_క్రమీకరించు_Radix_Ascend_16u_I> :
            లోతు == CV_16S ? ipp_క్రమీకరించు_Wrap<Ipp16s, ipps_క్రమీకరించు_Radix_Ascend_16s_I> :
            లోతు == CV_32S ? ipp_క్రమీకరించు_Wrap<Ipp32s, ipps_క్రమీకరించు_Radix_Ascend_32s_I> :
            లోతు == CV_32F ? ipp_క్రమీకరించు_Wrap<Ipp32f, ipps_క్రమీకరించు_Radix_Ascend_32f_I> :
            లోతు == CV_64F ? ipp_క్రమీకరించు_Wrap<Ipp64f, ipps_క్రమీకరించు_Radix_Ascend_64f_I> :
            0;
    లేకపోతే
        తిరిగి లోతు == CV_8U ? ipp_క్రమీకరించు_Wrap<Ipp8u, ipps_క్రమీకరించు_Radix_Descend_8u_I> :
            లోతు == CV_16U ? ipp_క్రమీకరించు_Wrap<Ipp16u, ipps_క్రమీకరించు_Radix_Descend_16u_I> :
            లోతు == CV_16S ? ipp_క్రమీకరించు_Wrap<Ipp16s, ipps_క్రమీకరించు_Radix_Descend_16s_I> :
            లోతు == CV_32S ? ipp_క్రమీకరించు_Wrap<Ipp32s, ipps_క్రమీకరించు_Radix_Descend_32s_I> :
            లోతు == CV_32F ? ipp_క్రమీకరించు_Wrap<Ipp32f, ipps_క్రమీకరించు_Radix_Descend_32f_I> :
            లోతు == CV_64F ? ipp_క్రమీకరించు_Wrap<Ipp64f, ipps_క్రమీకరించు_Radix_Descend_64f_I> :
            0;
}

స్థిర తార్కిక ipp_క్రమీకరించు(స్థిరరాశి Mat& src, Mat& dst, పూర్ణాంకం flags)
{
    CV_INSTRUMENT_REGION_IPP();

    తార్కిక        క్రమీకరించు_అడ్డువరుసలు        = (flags & 1) == క్రమీకరించు_EVERY_అడ్డువరుస;
    తార్కిక        క్రమీకరించు_Descending  = (flags & క్రమీకరించు_DESCENDING) != 0;
    తార్కిక        inplace         = (src.దత్తాంశం == dst.దత్తాంశం);
    పూర్ణాంకం         లోతు           = src.లోతు();
    Ipp_దత్తాంశం_Type type            = ippi_Get_దత్తాంశం_Type(లోతు);

    Ipp_క్రమీకరించు_Func ipps_క్రమీకరించు_Radix_I = get_క్రమీకరించు_Func(లోతు, క్రమీకరించు_Descending);
    ఒకవేళ(!ipps_క్రమీకరించు_Radix_I)
        తిరిగి అబద్ధం;

    పూర్ణాంకం n, len;
    ఒకవేళ( క్రమీకరించు_అడ్డువరుసలు )
        n = src.అడ్డువరుసలు, len = src.నిలువుల_సంఖ్య;
    లేకపోతే
        n = src.నిలువుల_సంఖ్య, len = src.అడ్డువరుసలు;

    పూర్ణాంకం బఫరు_కొలత;
    ఒకవేళ(ipps_క్రమీకరించు_Radix_Get_బఫరు_కొలత(len, type, &బఫరు_కొలత) < 0)
        తిరిగి అబద్ధం;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    అస్థిర తార్కిక ok = నిజం;
    parallel_for_(Range(0, n), [&](స్థిరరాశి Range& range)
    {
        Auto_బఫరు<Ipp8u> బఫరు(బఫరు_కొలత);
        Mat  అడ్డువరుస;
        Rect subRect(0,0,1,src.అడ్డువరుసలు);
        ఒకవేళ( !క్రమీకరించు_అడ్డువరుసలు )
            అడ్డువరుస.create(1, src.అడ్డువరుసలు, src.type());

        కొరకు( పూర్ణాంకం i = range.ప్రారంభం; i < range.end; i++ )
        {
            ఒకవేళ( !ok )
                ఆపు;
            శూన్య* ptr;
            ఒకవేళ( క్రమీకరించు_అడ్డువరుసలు )
            {
                uchar* dptr = dst.ptr(i);
                ఒకవేళ( !inplace )
                    జ్ఞాపకం_నకలు(dptr, src.ptr(i), src.నిలువుల_సంఖ్య * src.elem_కొలత());
                ptr = dptr;
            }
            లేకపోతే
            {
                subRect.x = i;
                Mat(src, subRect).copy_గమ్యం(అడ్డువరుస);
                ptr = అడ్డువరుస.ptr();
            }

            ఒకవేళ( CV_INSTRUMENT_FUN_IPP(ipps_క్రమీకరించు_Radix_I, ptr, len, బఫరు.దత్తాంశం()) < 0 )
            {
                ok = అబద్ధం;
                ఆపు;
            }

            ఒకవేళ( !క్రమీకరించు_అడ్డువరుసలు )
            {
                Mat dstSub(dst, subRect);
                అడ్డువరుస = అడ్డువరుస.reshape(1, dstSub.అడ్డువరుసలు);
                అడ్డువరుస.copy_గమ్యం(dstSub);
                అడ్డువరుస = అడ్డువరుస.reshape(1, 1);
            }
        }
    });

    తిరిగి ok;
}
#అయితే_ముగింపు

మూస<రకం_పేరు _Tp> తరగతి LessThanIdx
{
బహిరంగ:
    LessThanIdx( స్థిరరాశి _Tp* _arr ) : arr(_arr) {}
    తార్కిక క్రియాకారకం()(పూర్ణాంకం a, పూర్ణాంకం b) స్థిరరాశి { తిరిగి arr[a] < arr[b]; }
    స్థిరరాశి _Tp* arr;
};

మూస<రకం_పేరు T> స్థిర శూన్య క్రమీకరించు_Idx_2( స్థిరరాశి Mat& src, Mat& dst, పూర్ణాంకం flags )
{
    Auto_బఫరు<T> buf;
    Auto_బఫరు<పూర్ణాంకం> ibuf;
    తార్కిక క్రమీకరించు_అడ్డువరుసలు = (flags & 1) == క్రమీకరించు_EVERY_అడ్డువరుస;
    తార్కిక క్రమీకరించు_Descending = (flags & క్రమీకరించు_DESCENDING) != 0;

    CV_Assert( src.దత్తాంశం != dst.దత్తాంశం );

    పూర్ణాంకం n, len;
    ఒకవేళ( క్రమీకరించు_అడ్డువరుసలు )
        n = src.అడ్డువరుసలు, len = src.నిలువుల_సంఖ్య;
    లేకపోతే
    {
        n = src.నిలువుల_సంఖ్య, len = src.అడ్డువరుసలు;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.దత్తాంశం();
    పూర్ణాంకం* _iptr = ibuf.దత్తాంశం();

    కొరకు( పూర్ణాంకం i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        పూర్ణాంకం* iptr = _iptr;

        ఒకవేళ( క్రమీకరించు_అడ్డువరుసలు )
        {
            ptr = (T*)(src.దత్తాంశం + src.అడుగు*i);
            iptr = dst.ptr<పూర్ణాంకం>(i);
        }
        లేకపోతే
        {
            కొరకు( పూర్ణాంకం j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        కొరకు( పూర్ణాంకం j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        ఒకవేళ( క్రమీకరించు_Descending )
        {
            కొరకు( పూర్ణాంకం j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        ఒకవేళ( !క్రమీకరించు_అడ్డువరుసలు )
            కొరకు( పూర్ణాంకం j = 0; j < len; j++ )
                dst.ptr<పూర్ణాంకం>(j)[i] = iptr[j];
    }
}

#ఒకవేళ defined(HAVE_IPP) && !IPP_DISABLE_క్రమీకరించు
రకనిర్వచనం Ipp_స్థితి (CV_STDCALL *Ipp_క్రమీకరించు_సూచీ_Func)(స్థిరరాశి శూన్య*  pSrc, Ipp32s src_Stride_బైట్లు, Ipp32s *pDstIndx, పూర్ణాంకం len, Ipp8u *p_బఫరు);

మూస<రకం_పేరు T, Ipp_స్థితి (CV_STDCALL *fn)(స్థిరరాశి T*, Ipp32s, Ipp32s*, పూర్ణాంకం, Ipp8u*)>
స్థిర Ipp_స్థితి CV_STDCALL ipp_క్రమీకరించు_సూచీ_Wrap(స్థిరరాశి శూన్య* pSrc, Ipp32s src_Stride_బైట్లు, Ipp32s *pDstIndx, పూర్ణాంకం len, Ipp8u *p_బఫరు)
{
    తిరిగి fn((స్థిరరాశి T*)pSrc, src_Stride_బైట్లు, pDstIndx, len, p_బఫరు);
}

స్థిర Ipp_క్రమీకరించు_సూచీ_Func get_క్రమీకరించు_సూచీ_Func(పూర్ణాంకం లోతు, తార్కిక క్రమీకరించు_Descending)
{
    ఒకవేళ (!క్రమీకరించు_Descending)
        తిరిగి లోతు == CV_8U ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp8u, ipps_క్రమీకరించు_Radix_సూచీ_Ascend_8u> :
            లోతు == CV_16U ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp16u, ipps_క్రమీకరించు_Radix_సూచీ_Ascend_16u> :
            లోతు == CV_16S ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp16s, ipps_క్రమీకరించు_Radix_సూచీ_Ascend_16s> :
            లోతు == CV_32S ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp32s, ipps_క్రమీకరించు_Radix_సూచీ_Ascend_32s> :
            లోతు == CV_32F ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp32f, ipps_క్రమీకరించు_Radix_సూచీ_Ascend_32f> :
            0;
    లేకపోతే
        తిరిగి లోతు == CV_8U ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp8u, ipps_క్రమీకరించు_Radix_సూచీ_Descend_8u> :
            లోతు == CV_16U ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp16u, ipps_క్రమీకరించు_Radix_సూచీ_Descend_16u> :
            లోతు == CV_16S ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp16s, ipps_క్రమీకరించు_Radix_సూచీ_Descend_16s> :
            లోతు == CV_32S ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp32s, ipps_క్రమీకరించు_Radix_సూచీ_Descend_32s> :
            లోతు == CV_32F ? ipp_క్రమీకరించు_సూచీ_Wrap<Ipp32f, ipps_క్రమీకరించు_Radix_సూచీ_Descend_32f> :
            0;
}

స్థిర తార్కిక ipp_క్రమీకరించు_Idx( స్థిరరాశి Mat& src, Mat& dst, పూర్ణాంకం flags )
{
    CV_INSTRUMENT_REGION_IPP();

    తార్కిక        క్రమీకరించు_అడ్డువరుసలు        = (flags & 1) == క్రమీకరించు_EVERY_అడ్డువరుస;
    తార్కిక        క్రమీకరించు_Descending  = (flags & క్రమీకరించు_DESCENDING) != 0;
    పూర్ణాంకం         లోతు           = src.లోతు();
    Ipp_దత్తాంశం_Type type            = ippi_Get_దత్తాంశం_Type(లోతు);

    Ipp_క్రమీకరించు_సూచీ_Func ipps_క్రమీకరించు_Radix_సూచీ = get_క్రమీకరించు_సూచీ_Func(లోతు, క్రమీకరించు_Descending);
    ఒకవేళ(!ipps_క్రమీకరించు_Radix_సూచీ)
        తిరిగి అబద్ధం;

    ఒకవేళ(క్రమీకరించు_అడ్డువరుసలు)
    {
        Auto_బఫరు<Ipp8u> బఫరు;
        పూర్ణాంకం               బఫరు_కొలత;
        ఒకవేళ(ipps_క్రమీకరించు_Radix_సూచీ_Get_బఫరు_కొలత(src.నిలువుల_సంఖ్య, type, &బఫరు_కొలత) < 0)
            తిరిగి అబద్ధం;

        బఫరు.allocate(బఫరు_కొలత);

        కొరకు(పూర్ణాంకం i = 0; i < src.అడ్డువరుసలు; i++)
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(ipps_క్రమీకరించు_Radix_సూచీ, (స్థిరరాశి శూన్య*)src.ptr(i), (Ipp32s)src.అడుగు[1], (Ipp32s*)dst.ptr(i), src.నిలువుల_సంఖ్య, బఫరు.దత్తాంశం()) < 0)
                తిరిగి అబద్ధం;
        }
    }
    లేకపోతే
    {
        Mat  dst_అడ్డువరుస(1, dst.అడ్డువరుసలు, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.అడ్డువరుసలు);

        Auto_బఫరు<Ipp8u> బఫరు;
        పూర్ణాంకం               బఫరు_కొలత;
        ఒకవేళ(ipps_క్రమీకరించు_Radix_సూచీ_Get_బఫరు_కొలత(src.అడ్డువరుసలు, type, &బఫరు_కొలత) < 0)
            తిరిగి అబద్ధం;

        బఫరు.allocate(బఫరు_కొలత);

        Ipp32s src_అడుగు_3 = (Ipp32s)src.అడుగు[0];
        కొరకు(పూర్ణాంకం i = 0; i < src.నిలువుల_సంఖ్య; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(ipps_క్రమీకరించు_Radix_సూచీ, (స్థిరరాశి శూన్య*)src.ptr(0, i), src_అడుగు_3, (Ipp32s*)dst_అడ్డువరుస.ptr(), src.అడ్డువరుసలు, బఫరు.దత్తాంశం()) < 0)
                తిరిగి అబద్ధం;

            dst_అడ్డువరుస = dst_అడ్డువరుస.reshape(1, dstSub.అడ్డువరుసలు);
            dst_అడ్డువరుస.copy_గమ్యం(dstSub);
        }
    }

    తిరిగి నిజం;
}
#అయితే_ముగింపు

రకనిర్వచనం శూన్య (*క్రమీకరించు_Func)(స్థిరరాశి Mat& src, Mat& dst, పూర్ణాంకం flags);
}

శూన్య cv::sort( ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_కొలత( src, src.type() );
    Mat dst = _dst.getMat();
#ఒకవేళ !IPP_DISABLE_క్రమీకరించు
    CV_IPP_నిరంతరం_FAST(ipp_క్రమీకరించు(src, dst, flags));
#అయితే_ముగింపు

    స్థిర క్రమీకరించు_Func tab[CV_లోతు_MAX] =
    {
        క్రమీకరించు<uchar>, క్రమీకరించు<schar>, క్రమీకరించు<ushort>, క్రమీకరించు<పొట్టి>,
        క్రమీకరించు<పూర్ణాంకం>, క్రమీకరించు<దశాంశం>, క్రమీకరించు<ద్విగుణ>, 0
    };
    క్రమీకరించు_Func func = tab[src.లోతు()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

శూన్య cv::క్రమీకరించు_Idx( ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    ఒకవేళ( dst.దత్తాంశం == src.దత్తాంశం )
        _dst.release();
    _dst.create_Same_కొలత( src, CV_32S );
    dst = _dst.getMat();

#ఒకవేళ !IPP_DISABLE_క్రమీకరించు
    CV_IPP_నిరంతరం_FAST(ipp_క్రమీకరించు_Idx(src, dst, flags));
#అయితే_ముగింపు

    స్థిర క్రమీకరించు_Func tab[CV_లోతు_MAX] =
    {
        క్రమీకరించు_Idx_2<uchar>, క్రమీకరించు_Idx_2<schar>, క్రమీకరించు_Idx_2<ushort>, క్రమీకరించు_Idx_2<పొట్టి>,
        క్రమీకరించు_Idx_2<పూర్ణాంకం>, క్రమీకరించు_Idx_2<దశాంశం>, క్రమీకరించు_Idx_2<ద్విగుణ>, 0
    };
    క్రమీకరించు_Func func = tab[src.లోతు()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
