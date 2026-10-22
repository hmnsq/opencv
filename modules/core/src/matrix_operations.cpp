// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/mat.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IPP_DISABLE_REDUCE 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IPP_DISABLE_ਛਾਂਟੋ   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

ਖਾਲੀ cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.ਪੰਕਤੀਆਂ, b.ਪੰਕਤੀਆਂ);
    std::swap(a.ਖਾਨੇ_ਗਿਣਤੀ, b.ਖਾਨੇ_ਗਿਣਤੀ);
    std::swap(a.ਅੰਕੜੇ, b.ਅੰਕੜੇ);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.ਕਦਮ, b.ਕਦਮ);
}


ਖਾਲੀ cv::hconcat(ਅਚਲ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    ਜੇ( nsrc == 0 || !src )
    {
        _dst.release();
        ਵਾਪਸ;
    }

    ਪੂਰਨ ਕੁੱਲ_ਖਾਨੇ_ਗਿਣਤੀ = 0, ਖਾਨੇ_ਗਿਣਤੀ = 0;
    ਲਈ( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].ਪੰਕਤੀਆਂ == src[0].ਪੰਕਤੀਆਂ &&
                   src[i].type() == src[0].type());
        ਕੁੱਲ_ਖਾਨੇ_ਗਿਣਤੀ += src[i].ਖਾਨੇ_ਗਿਣਤੀ;
    }
    _dst.create( src[0].ਪੰਕਤੀਆਂ, ਕੁੱਲ_ਖਾਨੇ_ਗਿਣਤੀ, src[0].type());
    Mat dst = _dst.getMat();
    ਲਈ( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(ਖਾਨੇ_ਗਿਣਤੀ, 0, src[i].ਖਾਨੇ_ਗਿਣਤੀ, src[i].ਪੰਕਤੀਆਂ));
        src[i].copy_ਮੰਜ਼ਿਲ(dpart);
        ਖਾਨੇ_ਗਿਣਤੀ += src[i].ਖਾਨੇ_ਗਿਣਤੀ;
    }
}

ਖਾਲੀ cv::hconcat(ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

ਖਾਲੀ cv::hconcat(ਨਿਵੇਸ਼_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ਸਦਿਸ਼(src);
    hconcat(!src.empty() ? &src[0] : ਖਾਲੀ_ਸੂਚਕ, src.size(), dst);
}

ਖਾਲੀ cv::vconcat(ਅਚਲ Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    ਜੇ( nsrc == 0 || !src )
    {
        _dst.release();
        ਵਾਪਸ;
    }

    ਪੂਰਨ ਕੁੱਲ_ਪੰਕਤੀਆਂ = 0, ਪੰਕਤੀਆਂ = 0;
    ਲਈ( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].ਖਾਨੇ_ਗਿਣਤੀ == src[0].ਖਾਨੇ_ਗਿਣਤੀ &&
                  src[i].type() == src[0].type());
        ਕੁੱਲ_ਪੰਕਤੀਆਂ += src[i].ਪੰਕਤੀਆਂ;
    }
    _dst.create( ਕੁੱਲ_ਪੰਕਤੀਆਂ, src[0].ਖਾਨੇ_ਗਿਣਤੀ, src[0].type());
    Mat dst = _dst.getMat();
    ਲਈ( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, ਪੰਕਤੀਆਂ, src[i].ਖਾਨੇ_ਗਿਣਤੀ, src[i].ਪੰਕਤੀਆਂ));
        src[i].copy_ਮੰਜ਼ਿਲ(dpart);
        ਪੰਕਤੀਆਂ += src[i].ਪੰਕਤੀਆਂ;
    }
}

ਖਾਲੀ cv::vconcat(ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

ਖਾਲੀ cv::vconcat(ਨਿਵੇਸ਼_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_ਸਦਿਸ਼(src);
    vconcat(!src.empty() ? &src[0] : ਖਾਲੀ_ਸੂਚਕ, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਤਾਰਕਿਕ ocl_ਸੈੱਟ_ਪਛਾਣ_ਤੱਤ( ਨਿਵੇਸ਼_Output_Array _m, ਅਚਲ Scalar& s )
{
    ਪੂਰਨ type = _m.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type), kercn = cn, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = 1;
    ਪੂਰਨ sctype = CV_MAKE_TYPE(ਡੂੰਘਾਈ, cn == 3 ? 4 : cn);
    ਜੇ (ocl::Device::getDefault().isIntel())
    {
        ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = 4;
        ਜੇ (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(_m), 4);
            ਜੇ (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::ਸੈੱਟ_ਪਛਾਣ_ਤੱਤ_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, kercn)),
                         ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), cn,
                         ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(sctype),
                         kercn, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    UMat m = _m.getUMat();
    k.ਦਲੀਲਾਂ(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.ਖਾਨੇ_ਗਿਣਤੀ * cn / kercn, ((size_t)m.ਪੰਕਤੀਆਂ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਖਾਲੀ_ਸੂਚਕ, ਝੂਠ);
}

}

#ਜੇ_ਅੰਤ

ਖਾਲੀ cv::ਸੈੱਟ_ਪਛਾਣ_ਤੱਤ( ਨਿਵੇਸ਼_Output_Array _m, ਅਚਲ Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_ਲੜੀਵਾਰ(_m.isUMat(),
               ocl_ਸੈੱਟ_ਪਛਾਣ_ਤੱਤ(_m, s))

    Mat m = _m.getMat();
    ਪੂਰਨ ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ, type = m.type();

    ਜੇ( type == CV_32FC1 )
    {
        ਦਸ਼ਮਲਵ* ਅੰਕੜੇ = m.ptr<ਦਸ਼ਮਲਵ>();
        ਦਸ਼ਮਲਵ val = (ਦਸ਼ਮਲਵ)s[0];
        size_t ਕਦਮ = m.ਕਦਮ/ਆਕਾਰ(ਅੰਕੜੇ[0]);

        ਲਈ( ਪੂਰਨ i = 0; i < ਪੰਕਤੀਆਂ; i++, ਅੰਕੜੇ += ਕਦਮ )
        {
            ਲਈ( ਪੂਰਨ j = 0; j < ਖਾਨੇ_ਗਿਣਤੀ; j++ )
                ਅੰਕੜੇ[j] = 0;
            ਜੇ( i < ਖਾਨੇ_ਗਿਣਤੀ )
                ਅੰਕੜੇ[i] = val;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_64FC1 )
    {
        ਦੁੱਗਣਾ* ਅੰਕੜੇ = m.ptr<ਦੁੱਗਣਾ>();
        ਦੁੱਗਣਾ val = s[0];
        size_t ਕਦਮ = m.ਕਦਮ/ਆਕਾਰ(ਅੰਕੜੇ[0]);

        ਲਈ( ਪੂਰਨ i = 0; i < ਪੰਕਤੀਆਂ; i++, ਅੰਕੜੇ += ਕਦਮ )
        {
            std::fill(ਅੰਕੜੇ, ਅੰਕੜੇ + ਖਾਨੇ_ਗਿਣਤੀ, 0.0);
            ਜੇ (i < ਖਾਨੇ_ਗਿਣਤੀ)
                ਅੰਕੜੇ[i] = val;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


ਨਾਮਾਕਾਸ਼ cv {

UMat UMat::eye(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    ਵਾਪਸ UMat::eye(ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ), type, usageFlags);
}

UMat UMat::eye(ਮਾਪ size, ਪੂਰਨ type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    ਸੈੱਟ_ਪਛਾਣ_ਤੱਤ(m);
    ਵਾਪਸ m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( ਨਿਵੇਸ਼_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    ਪੂਰਨ type = m.type();
    ਪੂਰਨ nm = std::min(m.ਪੰਕਤੀਆਂ, m.ਖਾਨੇ_ਗਿਣਤੀ);

    ਜੇ( type == CV_32FC1 )
    {
        ਅਚਲ ਦਸ਼ਮਲਵ* ptr = m.ptr<ਦਸ਼ਮਲਵ>();
        size_t ਕਦਮ = m.ਕਦਮ/ਆਕਾਰ(ptr[0]) + 1;
        ਦੁੱਗਣਾ _s = 0;
        ਲਈ( ਪੂਰਨ i = 0; i < nm; i++ )
            _s += ptr[i*ਕਦਮ];
        ਵਾਪਸ _s;
    }

    ਜੇ( type == CV_64FC1 )
    {
        ਅਚਲ ਦੁੱਗਣਾ* ptr = m.ptr<ਦੁੱਗਣਾ>();
        size_t ਕਦਮ = m.ਕਦਮ/ਆਕਾਰ(ptr[0]) + 1;
        ਦੁੱਗਣਾ _s = 0;
        ਲਈ( ਪੂਰਨ i = 0; i < nm; i++ )
            _s += ptr[i*ਕਦਮ];
        ਵਾਪਸ _s;
    }

    ਵਾਪਸ cv::ਜੋੜ(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

ਖਾਲੀ cv::completeSymm( ਨਿਵੇਸ਼_Output_Array _m, ਤਾਰਕਿਕ LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t ਕਦਮ = m.ਕਦਮ, esz = m.elem_ਮਾਪ();
    CV_Assert( m.dims <= 2 && m.ਪੰਕਤੀਆਂ == m.ਖਾਨੇ_ਗਿਣਤੀ );

    ਪੂਰਨ ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ;
    ਪੂਰਨ j0 = 0, j1 = ਪੰਕਤੀਆਂ;

    uchar* ਅੰਕੜੇ = m.ptr();
    ਲਈ( ਪੂਰਨ i = 0; i < ਪੰਕਤੀਆਂ; i++ )
    {
        ਜੇ( !LtoR ) j1 = i; ਨਹੀਂਤਾਂ j0 = i+1;
        ਲਈ( ਪੂਰਨ j = j0; j < j1; j++ )
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਅੰਕੜੇ + (i*ਕਦਮ + j*esz), ਅੰਕੜੇ + (j*ਕਦਮ + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(ਨਿਵੇਸ਼_Array _m) ਅਚਲ
{
    Mat m = _m.getMat();
    ਪੂਰਨ tp = type(), d = CV_MAT_ਡੂੰਘਾਈ(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((ਪੰਕਤੀਆਂ == 3 && ਖਾਨੇ_ਗਿਣਤੀ == 1) || (ਖਾਨੇ_ਗਿਣਤੀ*channels() == 3 && ਪੰਕਤੀਆਂ == 1)));
    Mat ਨਤੀਜਾ(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, tp);

    ਜੇ( d == CV_32F )
    {
        ਅਚਲ ਦਸ਼ਮਲਵ *a = (ਅਚਲ ਦਸ਼ਮਲਵ*)ਅੰਕੜੇ, *b = (ਅਚਲ ਦਸ਼ਮਲਵ*)m.ਅੰਕੜੇ;
        ਦਸ਼ਮਲਵ* c = (ਦਸ਼ਮਲਵ*)ਨਤੀਜਾ.ਅੰਕੜੇ;
        size_t lda = ਪੰਕਤੀਆਂ > 1 ? ਕਦਮ/ਆਕਾਰ(a[0]) : 1;
        size_t ldb = ਪੰਕਤੀਆਂ > 1 ? m.ਕਦਮ/ਆਕਾਰ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    ਨਹੀਂਤਾਂ ਜੇ( d == CV_64F )
    {
        ਅਚਲ ਦੁੱਗਣਾ *a = (ਅਚਲ ਦੁੱਗਣਾ*)ਅੰਕੜੇ, *b = (ਅਚਲ ਦੁੱਗਣਾ*)m.ਅੰਕੜੇ;
        ਦੁੱਗਣਾ* c = (ਦੁੱਗਣਾ*)ਨਤੀਜਾ.ਅੰਕੜੇ;
        size_t lda = ਪੰਕਤੀਆਂ > 1 ? ਕਦਮ/ਆਕਾਰ(a[0]) : 1;
        size_t ldb = ਪੰਕਤੀਆਂ > 1 ? m.ਕਦਮ/ਆਕਾਰ(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    ਵਾਪਸ ਨਤੀਜਾ;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

ਨਾਮਾਕਾਸ਼ cv
{

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*Reduce_ਜੋੜ_Func)(ਅਚਲ Mat& src, Mat& dst);
Reduce_ਜੋੜ_Func getReduceCSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);
Reduce_ਜੋੜ_Func getReduceRSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);
Reduce_ਜੋੜ_Func getReduceRSum2Func(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ WT, ਕਿਸਮ_ਨਾਂ Op>
ਬਣਤਰ ReduceR_SIMD
{
    ਪੂਰਨ ਸੰਚਾਲਕ()(ਅਚਲ T*, ਪੂਰਨ ਸ਼ੁਰੂ, ਪੂਰਨ, WT*, ਅਚਲ Op&) ਅਚਲ
    {
        ਵਾਪਸ ਸ਼ੁਰੂ;
    }
};


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਕਿਸਮ_ਨਾਂ WT, ਵਰਗ Op, ਵਰਗ OpInit>
ਵਰਗ ReduceR_Invoker : ਜਨਤਕ ParallelLoopBody
{
ਜਨਤਕ:
  ReduceR_Invoker(ਅਚਲ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),ਬਫਰ(srcmat.size().ਚੌੜਾਈ*srcmat.channels())
  {
  }
  ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ Range& range) ਅਚਲ CV_OVERRIDE
  {
    ਅਚਲ T* src = srcmat.ptr<T>();
    ਅਚਲ size_t srcstep = srcmat.ਕਦਮ/ਆਕਾਰ(src[0]);
    WT* buf = ਬਫਰ.ਅੰਕੜੇ();
    ST* dst = dstmat.ptr<ST>();
    ਪੂਰਨ i = 0;

    ਲਈ( i = range.ਸ਼ੁਰੂ ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    ਪੂਰਨ ਉਚਾਈ = srcmat.size().ਉਚਾਈ;
    ਲਈ( ; --ਉਚਾਈ; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.ਸ਼ੁਰੂ, range.end, buf, op);
        #ਜੇ CV_ENABLE_UNROLLED
        ਲਈ(; i <= range.end - 4; i += 4 )
        {
            WT s0, s1;
            s0 = op(buf[i], (WT)src[i]);
            s1 = op(buf[i+1], (WT)src[i+1]);
            buf[i] = s0; buf[i+1] = s1;

            s0 = op(buf[i+2], (WT)src[i+2]);
            s1 = op(buf[i+3], (WT)src[i+3]);
            buf[i+2] = s0; buf[i+3] = s1;
        }
        #ਜੇ_ਅੰਤ
        ਲਈ( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    ਲਈ( i = range.ਸ਼ੁਰੂ ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
ਨਿਜੀ:
  ਅਚਲ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  ਬਦਲਣਯੋਗ Auto_ਬਫਰ<WT> ਬਫਰ;
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਵਰਗ Op, ਵਰਗ OpInit = OpNop<ST> > ਸਥਿਰ ਖਾਲੀ
reduceR_( ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().ਚੌੜਾਈ*srcmat.channels()), body, srcmat.size().ਚੌੜਾਈ*CV_ELEM_ਮਾਪ(srcmat.ਡੂੰਘਾਈ())/64);
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਕਿਸਮ_ਨਾਂ WT, ਵਰਗ Op, ਵਰਗ OpInit>
ਵਰਗ ReduceC_Invoker : ਜਨਤਕ ParallelLoopBody
{
ਜਨਤਕ:
  ReduceC_Invoker(ਅਚਲ Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ Range& range) ਅਚਲ CV_OVERRIDE
  {
    ਅਚਲ ਪੂਰਨ cn = srcmat.channels();
    ਅਚਲ ਪੂਰਨ ਚੌੜਾਈ = srcmat.size().ਚੌੜਾਈ*cn;
    Auto_ਬਫਰ<WT> cumul(cn);
    ਲਈ( ਪੂਰਨ y = range.ਸ਼ੁਰੂ; y < range.end; y++ )
    {
        ਅਚਲ T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        ਜੇ( ਚੌੜਾਈ == cn )
        {
          ਲਈ( ਪੂਰਨ k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਈ(ਪੂਰਨ k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            ਲਈ(ਪੂਰਨ k = cn ; k < ਚੌੜਾਈ ; k += cn )
            {
                ਲਈ (ਪੂਰਨ c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            ਲਈ(ਪੂਰਨ k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
ਨਿਜੀ:
  ਅਚਲ Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ST, ਵਰਗ Op, ਵਰਗ OpInit = OpNop<ST> > ਸਥਿਰ ਖਾਲੀ
reduceC_( ਅਚਲ Mat& srcmat, Mat& dstmat)
{
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().ਉਚਾਈ), body);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ReduceFunc)( ਅਚਲ Mat& src, Mat& dst );

}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R8u32s  reduceR_<uchar, ਪੂਰਨ,   Op_ਜੋੜੋ<ਪੂਰਨ>, OpNop<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R8u32f  reduceR_<uchar, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ<ਪੂਰਨ>, OpNop<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R8u64f  reduceR_<uchar, ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਪੂਰਨ>, OpNop<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R16u32f reduceR_<ushort,ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R16u64f reduceR_<ushort,ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R16s32f reduceR_<ਛੋਟਾ, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R16s64f reduceR_<ਛੋਟਾ, ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R32f32f reduceR_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R32f64f reduceR_<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_R64f64f reduceR_<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਦੁੱਗਣਾ> >

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R8u32s  reduceR_<uchar, ਪੂਰਨ,   Op_ਜੋੜੋ_Sqr<ਪੂਰਨ>,   OpSqr<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R8u32f  reduceR_<uchar, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ_Sqr<ਪੂਰਨ>,   OpSqr<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R8u64f  reduceR_<uchar, ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਪੂਰਨ>,   OpSqr<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R16u32f reduceR_<ushort,ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ_Sqr<ਦਸ਼ਮਲਵ>, OpSqr<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R16u64f reduceR_<ushort,ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਦੁੱਗਣਾ>,OpSqr<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R16s32f reduceR_<ਛੋਟਾ, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ_Sqr<ਦਸ਼ਮਲਵ>, OpSqr<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R16s64f reduceR_<ਛੋਟਾ, ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਦੁੱਗਣਾ>,OpSqr<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R32f32f reduceR_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ_Sqr<ਦਸ਼ਮਲਵ>, OpSqr<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R32f64f reduceR_<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਦੁੱਗਣਾ>,OpSqr<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2R64f64f reduceR_<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਦੁੱਗਣਾ>,OpSqr<ਦੁੱਗਣਾ> >

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxR16s reduceR_<ਛੋਟਾ, ਛੋਟਾ, OpMax<ਛੋਟਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxR32f reduceR_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, OpMax<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxR64f reduceR_<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ,OpMax<ਦੁੱਗਣਾ> >

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinR16s reduceR_<ਛੋਟਾ, ਛੋਟਾ, OpMin<ਛੋਟਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinR32f reduceR_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, OpMin<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinR64f reduceR_<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ,OpMin<ਦੁੱਗਣਾ> >

#ਜੇ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ipp_reduce_ਜੋੜ_C_8u16u16s32f_64f(ਅਚਲ cv::Mat& srcmat, cv::Mat& dstmat)
{
    ਪੂਰਨ sstep = (ਪੂਰਨ)srcmat.ਕਦਮ, stype = srcmat.type(),
            ddepth = dstmat.ਡੂੰਘਾਈ();

    Ippi_ਮਾਪ roisize = { srcmat.size().ਚੌੜਾਈ, 1 };

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * Ippi_ਜੋੜ)(ਅਚਲ ਖਾਲੀ * pSrc, ਪੂਰਨ src_ਕਦਮ_3, Ippi_ਮਾਪ roi_ਮਾਪ, Ipp64f* p_ਜੋੜ);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL * Ippi_ਜੋੜ_Hint)(ਅਚਲ ਖਾਲੀ * pSrc, ਪੂਰਨ src_ਕਦਮ_3, Ippi_ਮਾਪ roi_ਮਾਪ, Ipp64f* p_ਜੋੜ, IppHintAlgorithm hint);
    Ippi_ਜੋੜ ippi_ਜੋੜ = 0;
    Ippi_ਜੋੜ_Hint ippi_ਜੋੜ_Hint = 0;

    ਜੇ(ddepth == CV_64F)
    {
        ippi_ਜੋੜ =
            stype == CV_8UC1 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_ਜੋੜ)ippi_ਜੋੜ_16s_C4_R : 0;
        ippi_ਜੋੜ_Hint =
            stype == CV_32FC1 ? (Ippi_ਜੋੜ_Hint)ippi_ਜੋੜ_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_ਜੋੜ_Hint)ippi_ਜੋੜ_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_ਜੋੜ_Hint)ippi_ਜੋੜ_32f_C4_R : 0;
    }

    ਜੇ(ippi_ਜੋੜ)
    {
        ਲਈ(ਪੂਰਨ y = 0; y < srcmat.size().ਉਚਾਈ; y++)
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(ippi_ਜੋੜ, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
        ਵਾਪਸ ਸੱਚ;
    }
    ਨਹੀਂਤਾਂ ਜੇ(ippi_ਜੋੜ_Hint)
    {
        ਲਈ(ਪੂਰਨ y = 0; y < srcmat.size().ਉਚਾਈ; y++)
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(ippi_ਜੋੜ_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
        ਵਾਪਸ ਸੱਚ;
    }

    ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ reduce_ਜੋੜ_C_8u16u16s32f_64f(ਅਚਲ cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_reduce_ਜੋੜ_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    ਜੇ(dstmat.ਡੂੰਘਾਈ() == CV_64F)
    {
        ਪੂਰਨ sdepth = CV_MAT_ਡੂੰਘਾਈ(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, ਦੁੱਗਣਾ,   cv::Op_ਜੋੜੋ<ਦੁੱਗਣਾ> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, ਦੁੱਗਣਾ,   cv::Op_ਜੋੜੋ<ਦੁੱਗਣਾ> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<ਛੋਟਾ, ਦੁੱਗਣਾ,   cv::Op_ਜੋੜੋ<ਦੁੱਗਣਾ> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ,   cv::Op_ਜੋੜੋ<ਦੁੱਗਣਾ> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C8u32s  reduceC_<uchar, ਪੂਰਨ,   Op_ਜੋੜੋ<ਪੂਰਨ>, OpNop<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C8u32f  reduceC_<uchar, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ<ਪੂਰਨ>, OpNop<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C16u32f reduceC_<ushort,ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C16s32f reduceC_<ਛੋਟਾ, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C32f32f reduceC_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C64f64f reduceC_<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਦੁੱਗਣਾ> >

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C8u32s  reduceC_<uchar, ਪੂਰਨ,   Op_ਜੋੜੋ_Sqr<ਪੂਰਨ>,   OpSqr<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C8u32f  reduceC_<uchar, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ_Sqr<ਪੂਰਨ>,   OpSqr<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C16u32f reduceC_<ushort,ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ_Sqr<ਦਸ਼ਮਲਵ>, OpSqr<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C16s32f reduceC_<ਛੋਟਾ, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ_Sqr<ਦਸ਼ਮਲਵ>, OpSqr<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C32f32f reduceC_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, Op_ਜੋੜੋ_Sqr<ਦਸ਼ਮਲਵ>, OpSqr<ਦਸ਼ਮਲਵ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C64f64f reduceC_<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਦੁੱਗਣਾ>,OpSqr<ਦੁੱਗਣਾ> >

#ਜੇ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C8u64f  reduce_ਜੋੜ_C_8u16u16s32f_64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C16u64f reduce_ਜੋੜ_C_8u16u16s32f_64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C16s64f reduce_ਜੋੜ_C_8u16u16s32f_64f
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C32f64f reduce_ਜੋੜ_C_8u16u16s32f_64f
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C8u64f  reduceC_<uchar, ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਪੂਰਨ>, OpNop<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C16u64f reduceC_<ushort,ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C16s64f reduceC_<ਛੋਟਾ, ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduce_ਜੋੜ_C32f64f reduceC_<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ,Op_ਜੋੜੋ<ਦੁੱਗਣਾ> >
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C8u64f  reduceC_<uchar, ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਪੂਰਨ>,   OpSqr<ਪੂਰਨ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C16u64f reduceC_<ushort,ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਦੁੱਗਣਾ>,OpSqr<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C16s64f reduceC_<ਛੋਟਾ, ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਦੁੱਗਣਾ>,OpSqr<ਦੁੱਗਣਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceSum2C32f64f reduceC_<ਦਸ਼ਮਲਵ, ਦੁੱਗਣਾ,Op_ਜੋੜੋ_Sqr<ਦੁੱਗਣਾ>,OpSqr<ਦੁੱਗਣਾ> >

#ਜੇ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REDUCE_OP(favor, optype, type1, type2) \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ipp_reduce##optype##C##favor(ਅਚਲ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    ਜੇ((srcmat.channels() == 1)) \
    { \
        ਪੂਰਨ sstep = (ਪੂਰਨ)srcmat.ਕਦਮ; \
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp##favor IppType; \
        Ippi_ਮਾਪ roisize = ippi_ਮਾਪ(srcmat.size().ਚੌੜਾਈ, 1);\
        ਲਈ(ਪੂਰਨ y = 0; y < srcmat.size().ਉਚਾਈ; y++)\
        {\
            ਜੇ(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                ਵਾਪਸ ਝੂਠ;\
        }\
        ਵਾਪਸ ਸੱਚ;\
    }\
    ਵਾਪਸ ਝੂਠ; \
} \
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ reduce##optype##C##favor(ਅਚਲ cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, ਛੋਟਾ, ਛੋਟਾ)
REDUCE_OP(32f, Max, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxC16s reduceC_<ਛੋਟਾ, ਛੋਟਾ, OpMax<ਛੋਟਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxC32f reduceC_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, OpMax<ਦਸ਼ਮਲਵ> >
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMaxC64f reduceC_<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ,OpMax<ਦੁੱਗਣਾ> >

#ਜੇ defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, ਛੋਟਾ, ਛੋਟਾ)
REDUCE_OP(32f, Min, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinC16s reduceC_<ਛੋਟਾ, ਛੋਟਾ, OpMin<ਛੋਟਾ> >
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinC32f reduceC_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, OpMin<ਦਸ਼ਮਲਵ> >
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ reduceMinC64f reduceC_<ਦੁੱਗਣਾ,ਦੁੱਗਣਾ,OpMin<ਦੁੱਗਣਾ> >

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਤਾਰਕਿਕ ocl_reduce(ਨਿਵੇਸ਼_Array _src, OutputArray _dst,
                       ਪੂਰਨ dim, ਪੂਰਨ op, ਪੂਰਨ op0, ਪੂਰਨ stype, ਪੂਰਨ dtype)
{
    ਅਚਲ ਪੂਰਨ min_opt_ਖਾਨੇ_ਗਿਣਤੀ = 128, buf_ਖਾਨੇ_ਗਿਣਤੀ = 32;
    ਪੂਰਨ sdepth = CV_MAT_ਡੂੰਘਾਈ(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_ਡੂੰਘਾਈ(dtype), ddepth0 = ddepth;
    ਅਚਲ ocl::Device &defDev = ocl::Device::getDefault();
    ਤਾਰਕਿਕ doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_ਸਮੂਹ_ਮਾਪ();
    ਤਾਰਕਿਕ useOptimized = 1 == dim && _src.ਖਾਨੇ_ਗਿਣਤੀ() > min_opt_ਖਾਨੇ_ਗਿਣਤੀ && (wgs >= buf_ਖਾਨੇ_ਗਿਣਤੀ);

    ਜੇ (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        ਵਾਪਸ ਝੂਠ;

    ਜੇ (op == REDUCE_AVG)
    {
        ਜੇ (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ਅਚਲ ਅੱਖਰ * ਅਚਲ ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    ਪੂਰਨ wdepth = std::max(ddepth, CV_32F);
    ਜੇ (useOptimized)
    {
        size_t tile_ਉਚਾਈ = (size_t)(wgs / buf_ਖਾਨੇ_ਗਿਣਤੀ);
        ਜੇ (defDev.isIntel())
        {
            ਸਥਿਰ ਅਚਲ size_t max_ਵਸਤੂ_ਵਿੱਚ_ਸਮੂਹ_ਗਿਣਤੀ = 16;
            tile_ਉਚਾਈ = min(tile_ਉਚਾਈ, defDev.local_Mem_ਮਾਪ() / buf_ਖਾਨੇ_ਗਿਣਤੀ / CV_ELEM_ਮਾਪ(CV_MAKETYPE(wdepth, cn)) / max_ਵਸਤੂ_ਵਿੱਚ_ਸਮੂਹ_ਗਿਣਤੀ);
        }
        ਅੱਖਰ cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_ਖਾਨੇ_ਗਿਣਤੀ, tile_ਉਚਾਈ, ops[op], cn, ddepth,
                                            ocl::type_ਮੰਜ਼ਿਲ_Str(sdepth),
                                            ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth),
                                            ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], ਆਕਾਰ(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], ਆਕਾਰ(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], ਆਕਾਰ(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        ਜੇ (k.empty())
            ਵਾਪਸ ਝੂਠ;
        UMat src = _src.getUMat();
        ਮਾਪ dsize(1, src.ਪੰਕਤੀਆਂ);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ਜੇ (op0 == REDUCE_AVG)
            k.ਦਲੀਲਾਂ(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_ਮਾਪ(dst), 1.0f / src.ਖਾਨੇ_ਗਿਣਤੀ);
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_ਮਾਪ(dst));

        size_t local_ਮਾਪ[2] = { (size_t)buf_ਖਾਨੇ_ਗਿਣਤੀ, (size_t)tile_ਉਚਾਈ};
        size_t global_ਮਾਪ[2] = { (size_t)buf_ਖਾਨੇ_ਗਿਣਤੀ, (size_t)src.ਪੰਕਤੀਆਂ };
        ਵਾਪਸ k.ਲੜੀਵਾਰ(2, global_ਮਾਪ, local_ਮਾਪ, ਝੂਠ);
    }
    ਨਹੀਂਤਾਂ
    {
        ਅੱਖਰ cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::type_ਮੰਜ਼ਿਲ_Str(useOptimized ? ddepth : sdepth),
                                      ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth), ocl::type_ਮੰਜ਼ਿਲ_Str(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], ਆਕਾਰ(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], ਆਕਾਰ(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], ਆਕਾਰ(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        ਜੇ (k.empty())
            ਵਾਪਸ ਝੂਠ;

        UMat src = _src.getUMat();
        ਮਾਪ dsize(dim == 0 ? src.ਖਾਨੇ_ਗਿਣਤੀ : 1, dim == 0 ? 1 : src.ਪੰਕਤੀਆਂ);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_ਮਾਪ(dst);

        ਜੇ (op0 == REDUCE_AVG)
            k.ਦਲੀਲਾਂ(srcarg, temparg, 1.0f / (dim == 0 ? src.ਪੰਕਤੀਆਂ : src.ਖਾਨੇ_ਗਿਣਤੀ));
        ਨਹੀਂਤਾਂ
            k.ਦਲੀਲਾਂ(srcarg, temparg);

        size_t globalsize = std::max(dsize.ਚੌੜਾਈ, dsize.ਉਚਾਈ);
        ਵਾਪਸ k.ਲੜੀਵਾਰ(1, &globalsize, ਨਿਰਾ, ਝੂਠ);
    }
}

}

#ਜੇ_ਅੰਤ

ਖਾਲੀ cv::reduce(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ dim, ਪੂਰਨ op, ਪੂਰਨ dtype)
{
    CV_INSTRUMENT_REGION();

    ਜੇ (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, ਸੱਚ, dtype < 0 && _dst.fixedType() ? _dst.ਡੂੰਘਾਈ() : dtype));
        ਵਾਪਸ;
    }
    ਪੂਰਨ op0 = op;
    ਪੂਰਨ stype = _src.type(), sdepth = CV_MAT_ਡੂੰਘਾਈ(stype), cn = CV_MAT_CN(stype);
    ਜੇ( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    ਪੂਰਨ ddepth = CV_MAT_ਡੂੰਘਾਈ(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_ਜੋੜ || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_ਲੜੀਵਾਰ(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    ਜੇ (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    ਜੇ (src.empty()) {
        src.convert_ਮੰਜ਼ਿਲ(_dst, dtype);
        ਵਾਪਸ;
    }
    ਜੇ (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.ਪੰਕਤੀਆਂ, dim == 0 ? src.ਖਾਨੇ_ਗਿਣਤੀ : 1, dtype);
    Mat dst = _dst.getMat(), ਆਰਜ਼ੀ_2 = dst;

    ਜੇ( op == REDUCE_AVG )
    {
        op = REDUCE_ਜੋੜ;
        ਜੇ( sdepth < CV_32S && ddepth < CV_32S )
        {
            ਆਰਜ਼ੀ_2.create(dst.ਪੰਕਤੀਆਂ, dst.ਖਾਨੇ_ਗਿਣਤੀ, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    ਜੇ( dim == 0 )
    {
        ਜੇ( op == REDUCE_ਜੋੜ )
        {
            Reduce_ਜੋੜ_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            ਜੇ(simd_func)
                func = (ReduceFunc)simd_func;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_ਜੋੜ_R8u32s;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_ਜੋੜ_R8u32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_ਜੋੜ_R8u64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_ਜੋੜ_R16u32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_ਜੋੜ_R16u64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_ਜੋੜ_R16s32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_ਜੋੜ_R16s64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_ਜੋੜ_R32f32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_ਜੋੜ_R32f64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_ਜੋੜ_R64f64f;
        }
        ਨਹੀਂਤਾਂ ਜੇ(op == REDUCE_MAX)
        {
            ਜੇ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        ਨਹੀਂਤਾਂ ਜੇ(op == REDUCE_MIN)
        {
            ਜੇ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        ਨਹੀਂਤਾਂ ਜੇ( op == REDUCE_SUM2 )
        {
            Reduce_ਜੋੜ_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
            ਜੇ(simd_func)
                func = simd_func;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ(op == REDUCE_ਜੋੜ)
        {
            Reduce_ਜੋੜ_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            ਜੇ(simd_func)
                func = (ReduceFunc)simd_func;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_ਜੋੜ_C8u32s;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_ਜੋੜ_C8u32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_ਜੋੜ_C8u64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_ਜੋੜ_C16u32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_ਜੋੜ_C16u64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_ਜੋੜ_C16s32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_ਜੋੜ_C16s64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_ਜੋੜ_C32f32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_ਜੋੜ_C32f64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_ਜੋੜ_C64f64f;
        }
        ਨਹੀਂਤਾਂ ਜੇ(op == REDUCE_MAX)
        {
            ਜੇ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        ਨਹੀਂਤਾਂ ਜੇ(op == REDUCE_MIN)
        {
            ਜੇ(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        ਨਹੀਂਤਾਂ ਜੇ(op == REDUCE_SUM2)
        {
            ਜੇ(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            ਨਹੀਂਤਾਂ ਜੇ(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    ਜੇ( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, ਆਰਜ਼ੀ_2 );

    ਜੇ( op0 == REDUCE_AVG )
        ਆਰਜ਼ੀ_2.convert_ਮੰਜ਼ਿਲ(dst, dst.type(), 1./(dim == 0 ? src.ਪੰਕਤੀਆਂ : src.ਖਾਨੇ_ਗਿਣਤੀ));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

ਨਾਮਾਕਾਸ਼ cv
{

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ ਛਾਂਟੋ( ਅਚਲ Mat& src, Mat& dst, ਪੂਰਨ flags )
{
    ਪੂਰਨ n, len;
    ਤਾਰਕਿਕ ਛਾਂਟੋ_ਪੰਕਤੀਆਂ = (flags & 1) == ਛਾਂਟੋ_EVERY_ਪੰਕਤੀ;
    ਤਾਰਕਿਕ inplace = src.ਅੰਕੜੇ == dst.ਅੰਕੜੇ;
    ਤਾਰਕਿਕ ਛਾਂਟੋ_Descending = (flags & ਛਾਂਟੋ_DESCENDING) != 0;

    ਜੇ( ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
        n = src.ਪੰਕਤੀਆਂ, len = src.ਖਾਨੇ_ਗਿਣਤੀ;
    ਨਹੀਂਤਾਂ
        n = src.ਖਾਨੇ_ਗਿਣਤੀ, len = src.ਪੰਕਤੀਆਂ;
    parallel_for_(Range(0, n), [&](ਅਚਲ Range& range)
    {
        Auto_ਬਫਰ<T> buf;
        ਜੇ( !ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
            buf.allocate(len);
        T* bptr = buf.ਅੰਕੜੇ();

        ਲਈ( ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; i++ )
        {
            T* ptr = bptr;
            ਜੇ( ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
            {
                T* dptr = dst.ptr<T>(i);
                ਜੇ( !inplace )
                {
                    ਅਚਲ T* sptr = src.ptr<T>(i);
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ(dptr, sptr, ਆਕਾਰ(T) * len);
                }
                ptr = dptr;
            }
            ਨਹੀਂਤਾਂ
            {
                ਲਈ( ਪੂਰਨ j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            ਜੇ( ਛਾਂਟੋ_Descending )
            {
                ਲਈ( ਪੂਰਨ j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            ਜੇ( !ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
                ਲਈ( ਪੂਰਨ j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#ਜੇ defined(HAVE_IPP) && !IPP_DISABLE_ਛਾਂਟੋ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL *Ipp_ਛਾਂਟੋ_Func)(ਖਾਲੀ  *pSrcDst, ਪੂਰਨ    len, Ipp8u *p_ਬਫਰ);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(T*, ਪੂਰਨ, Ipp8u*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ipp_ਛਾਂਟੋ_Wrap(ਖਾਲੀ* pSrcDst, ਪੂਰਨ len, Ipp8u* p_ਬਫਰ)
{
    ਵਾਪਸ fn((T*)pSrcDst, len, p_ਬਫਰ);
}

ਸਥਿਰ Ipp_ਛਾਂਟੋ_Func get_ਛਾਂਟੋ_Func(ਪੂਰਨ ਡੂੰਘਾਈ, ਤਾਰਕਿਕ ਛਾਂਟੋ_Descending)
{
    ਜੇ (!ਛਾਂਟੋ_Descending)
        ਵਾਪਸ ਡੂੰਘਾਈ == CV_8U ? ipp_ਛਾਂਟੋ_Wrap<Ipp8u, ipps_ਛਾਂਟੋ_Radix_Ascend_8u_I> :
            ਡੂੰਘਾਈ == CV_16U ? ipp_ਛਾਂਟੋ_Wrap<Ipp16u, ipps_ਛਾਂਟੋ_Radix_Ascend_16u_I> :
            ਡੂੰਘਾਈ == CV_16S ? ipp_ਛਾਂਟੋ_Wrap<Ipp16s, ipps_ਛਾਂਟੋ_Radix_Ascend_16s_I> :
            ਡੂੰਘਾਈ == CV_32S ? ipp_ਛਾਂਟੋ_Wrap<Ipp32s, ipps_ਛਾਂਟੋ_Radix_Ascend_32s_I> :
            ਡੂੰਘਾਈ == CV_32F ? ipp_ਛਾਂਟੋ_Wrap<Ipp32f, ipps_ਛਾਂਟੋ_Radix_Ascend_32f_I> :
            ਡੂੰਘਾਈ == CV_64F ? ipp_ਛਾਂਟੋ_Wrap<Ipp64f, ipps_ਛਾਂਟੋ_Radix_Ascend_64f_I> :
            0;
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ ਡੂੰਘਾਈ == CV_8U ? ipp_ਛਾਂਟੋ_Wrap<Ipp8u, ipps_ਛਾਂਟੋ_Radix_Descend_8u_I> :
            ਡੂੰਘਾਈ == CV_16U ? ipp_ਛਾਂਟੋ_Wrap<Ipp16u, ipps_ਛਾਂਟੋ_Radix_Descend_16u_I> :
            ਡੂੰਘਾਈ == CV_16S ? ipp_ਛਾਂਟੋ_Wrap<Ipp16s, ipps_ਛਾਂਟੋ_Radix_Descend_16s_I> :
            ਡੂੰਘਾਈ == CV_32S ? ipp_ਛਾਂਟੋ_Wrap<Ipp32s, ipps_ਛਾਂਟੋ_Radix_Descend_32s_I> :
            ਡੂੰਘਾਈ == CV_32F ? ipp_ਛਾਂਟੋ_Wrap<Ipp32f, ipps_ਛਾਂਟੋ_Radix_Descend_32f_I> :
            ਡੂੰਘਾਈ == CV_64F ? ipp_ਛਾਂਟੋ_Wrap<Ipp64f, ipps_ਛਾਂਟੋ_Radix_Descend_64f_I> :
            0;
}

ਸਥਿਰ ਤਾਰਕਿਕ ipp_ਛਾਂਟੋ(ਅਚਲ Mat& src, Mat& dst, ਪੂਰਨ flags)
{
    CV_INSTRUMENT_REGION_IPP();

    ਤਾਰਕਿਕ        ਛਾਂਟੋ_ਪੰਕਤੀਆਂ        = (flags & 1) == ਛਾਂਟੋ_EVERY_ਪੰਕਤੀ;
    ਤਾਰਕਿਕ        ਛਾਂਟੋ_Descending  = (flags & ਛਾਂਟੋ_DESCENDING) != 0;
    ਤਾਰਕਿਕ        inplace         = (src.ਅੰਕੜੇ == dst.ਅੰਕੜੇ);
    ਪੂਰਨ         ਡੂੰਘਾਈ           = src.ਡੂੰਘਾਈ();
    Ipp_ਅੰਕੜੇ_Type type            = ippi_Get_ਅੰਕੜੇ_Type(ਡੂੰਘਾਈ);

    Ipp_ਛਾਂਟੋ_Func ipps_ਛਾਂਟੋ_Radix_I = get_ਛਾਂਟੋ_Func(ਡੂੰਘਾਈ, ਛਾਂਟੋ_Descending);
    ਜੇ(!ipps_ਛਾਂਟੋ_Radix_I)
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ n, len;
    ਜੇ( ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
        n = src.ਪੰਕਤੀਆਂ, len = src.ਖਾਨੇ_ਗਿਣਤੀ;
    ਨਹੀਂਤਾਂ
        n = src.ਖਾਨੇ_ਗਿਣਤੀ, len = src.ਪੰਕਤੀਆਂ;

    ਪੂਰਨ ਬਫਰ_ਮਾਪ;
    ਜੇ(ipps_ਛਾਂਟੋ_Radix_Get_ਬਫਰ_ਮਾਪ(len, type, &ਬਫਰ_ਮਾਪ) < 0)
        ਵਾਪਸ ਝੂਠ;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    ਅਸਥਿਰ ਤਾਰਕਿਕ ok = ਸੱਚ;
    parallel_for_(Range(0, n), [&](ਅਚਲ Range& range)
    {
        Auto_ਬਫਰ<Ipp8u> ਬਫਰ(ਬਫਰ_ਮਾਪ);
        Mat  ਪੰਕਤੀ;
        Rect subRect(0,0,1,src.ਪੰਕਤੀਆਂ);
        ਜੇ( !ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
            ਪੰਕਤੀ.create(1, src.ਪੰਕਤੀਆਂ, src.type());

        ਲਈ( ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; i++ )
        {
            ਜੇ( !ok )
                ਰੋਕੋ;
            ਖਾਲੀ* ptr;
            ਜੇ( ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
            {
                uchar* dptr = dst.ptr(i);
                ਜੇ( !inplace )
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ(dptr, src.ptr(i), src.ਖਾਨੇ_ਗਿਣਤੀ * src.elem_ਮਾਪ());
                ptr = dptr;
            }
            ਨਹੀਂਤਾਂ
            {
                subRect.x = i;
                Mat(src, subRect).copy_ਮੰਜ਼ਿਲ(ਪੰਕਤੀ);
                ptr = ਪੰਕਤੀ.ptr();
            }

            ਜੇ( CV_INSTRUMENT_FUN_IPP(ipps_ਛਾਂਟੋ_Radix_I, ptr, len, ਬਫਰ.ਅੰਕੜੇ()) < 0 )
            {
                ok = ਝੂਠ;
                ਰੋਕੋ;
            }

            ਜੇ( !ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
            {
                Mat dstSub(dst, subRect);
                ਪੰਕਤੀ = ਪੰਕਤੀ.reshape(1, dstSub.ਪੰਕਤੀਆਂ);
                ਪੰਕਤੀ.copy_ਮੰਜ਼ਿਲ(dstSub);
                ਪੰਕਤੀ = ਪੰਕਤੀ.reshape(1, 1);
            }
        }
    });

    ਵਾਪਸ ok;
}
#ਜੇ_ਅੰਤ

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp> ਵਰਗ LessThanIdx
{
ਜਨਤਕ:
    LessThanIdx( ਅਚਲ _Tp* _arr ) : arr(_arr) {}
    ਤਾਰਕਿਕ ਸੰਚਾਲਕ()(ਪੂਰਨ a, ਪੂਰਨ b) ਅਚਲ { ਵਾਪਸ arr[a] < arr[b]; }
    ਅਚਲ _Tp* arr;
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ ਛਾਂਟੋ_Idx_2( ਅਚਲ Mat& src, Mat& dst, ਪੂਰਨ flags )
{
    Auto_ਬਫਰ<T> buf;
    Auto_ਬਫਰ<ਪੂਰਨ> ibuf;
    ਤਾਰਕਿਕ ਛਾਂਟੋ_ਪੰਕਤੀਆਂ = (flags & 1) == ਛਾਂਟੋ_EVERY_ਪੰਕਤੀ;
    ਤਾਰਕਿਕ ਛਾਂਟੋ_Descending = (flags & ਛਾਂਟੋ_DESCENDING) != 0;

    CV_Assert( src.ਅੰਕੜੇ != dst.ਅੰਕੜੇ );

    ਪੂਰਨ n, len;
    ਜੇ( ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
        n = src.ਪੰਕਤੀਆਂ, len = src.ਖਾਨੇ_ਗਿਣਤੀ;
    ਨਹੀਂਤਾਂ
    {
        n = src.ਖਾਨੇ_ਗਿਣਤੀ, len = src.ਪੰਕਤੀਆਂ;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.ਅੰਕੜੇ();
    ਪੂਰਨ* _iptr = ibuf.ਅੰਕੜੇ();

    ਲਈ( ਪੂਰਨ i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        ਪੂਰਨ* iptr = _iptr;

        ਜੇ( ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
        {
            ptr = (T*)(src.ਅੰਕੜੇ + src.ਕਦਮ*i);
            iptr = dst.ptr<ਪੂਰਨ>(i);
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਈ( ਪੂਰਨ j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        ਲਈ( ਪੂਰਨ j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        ਜੇ( ਛਾਂਟੋ_Descending )
        {
            ਲਈ( ਪੂਰਨ j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        ਜੇ( !ਛਾਂਟੋ_ਪੰਕਤੀਆਂ )
            ਲਈ( ਪੂਰਨ j = 0; j < len; j++ )
                dst.ptr<ਪੂਰਨ>(j)[i] = iptr[j];
    }
}

#ਜੇ defined(HAVE_IPP) && !IPP_DISABLE_ਛਾਂਟੋ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ipp_ਸਥਿਤੀ (CV_STDCALL *Ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Func)(ਅਚਲ ਖਾਲੀ*  pSrc, Ipp32s src_Stride_ਬਾਈਟਾਂ, Ipp32s *pDstIndx, ਪੂਰਨ len, Ipp8u *p_ਬਫਰ);

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T, Ipp_ਸਥਿਤੀ (CV_STDCALL *fn)(ਅਚਲ T*, Ipp32s, Ipp32s*, ਪੂਰਨ, Ipp8u*)>
ਸਥਿਰ Ipp_ਸਥਿਤੀ CV_STDCALL ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap(ਅਚਲ ਖਾਲੀ* pSrc, Ipp32s src_Stride_ਬਾਈਟਾਂ, Ipp32s *pDstIndx, ਪੂਰਨ len, Ipp8u *p_ਬਫਰ)
{
    ਵਾਪਸ fn((ਅਚਲ T*)pSrc, src_Stride_ਬਾਈਟਾਂ, pDstIndx, len, p_ਬਫਰ);
}

ਸਥਿਰ Ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Func get_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Func(ਪੂਰਨ ਡੂੰਘਾਈ, ਤਾਰਕਿਕ ਛਾਂਟੋ_Descending)
{
    ਜੇ (!ਛਾਂਟੋ_Descending)
        ਵਾਪਸ ਡੂੰਘਾਈ == CV_8U ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp8u, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Ascend_8u> :
            ਡੂੰਘਾਈ == CV_16U ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp16u, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Ascend_16u> :
            ਡੂੰਘਾਈ == CV_16S ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp16s, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Ascend_16s> :
            ਡੂੰਘਾਈ == CV_32S ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp32s, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Ascend_32s> :
            ਡੂੰਘਾਈ == CV_32F ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp32f, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Ascend_32f> :
            0;
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ ਡੂੰਘਾਈ == CV_8U ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp8u, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Descend_8u> :
            ਡੂੰਘਾਈ == CV_16U ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp16u, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Descend_16u> :
            ਡੂੰਘਾਈ == CV_16S ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp16s, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Descend_16s> :
            ਡੂੰਘਾਈ == CV_32S ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp32s, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Descend_32s> :
            ਡੂੰਘਾਈ == CV_32F ? ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Wrap<Ipp32f, ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Descend_32f> :
            0;
}

ਸਥਿਰ ਤਾਰਕਿਕ ipp_ਛਾਂਟੋ_Idx( ਅਚਲ Mat& src, Mat& dst, ਪੂਰਨ flags )
{
    CV_INSTRUMENT_REGION_IPP();

    ਤਾਰਕਿਕ        ਛਾਂਟੋ_ਪੰਕਤੀਆਂ        = (flags & 1) == ਛਾਂਟੋ_EVERY_ਪੰਕਤੀ;
    ਤਾਰਕਿਕ        ਛਾਂਟੋ_Descending  = (flags & ਛਾਂਟੋ_DESCENDING) != 0;
    ਪੂਰਨ         ਡੂੰਘਾਈ           = src.ਡੂੰਘਾਈ();
    Ipp_ਅੰਕੜੇ_Type type            = ippi_Get_ਅੰਕੜੇ_Type(ਡੂੰਘਾਈ);

    Ipp_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Func ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ = get_ਛਾਂਟੋ_ਸੂਚਕਾਂਕ_Func(ਡੂੰਘਾਈ, ਛਾਂਟੋ_Descending);
    ਜੇ(!ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ(ਛਾਂਟੋ_ਪੰਕਤੀਆਂ)
    {
        Auto_ਬਫਰ<Ipp8u> ਬਫਰ;
        ਪੂਰਨ               ਬਫਰ_ਮਾਪ;
        ਜੇ(ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Get_ਬਫਰ_ਮਾਪ(src.ਖਾਨੇ_ਗਿਣਤੀ, type, &ਬਫਰ_ਮਾਪ) < 0)
            ਵਾਪਸ ਝੂਠ;

        ਬਫਰ.allocate(ਬਫਰ_ਮਾਪ);

        ਲਈ(ਪੂਰਨ i = 0; i < src.ਪੰਕਤੀਆਂ; i++)
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ, (ਅਚਲ ਖਾਲੀ*)src.ptr(i), (Ipp32s)src.ਕਦਮ[1], (Ipp32s*)dst.ptr(i), src.ਖਾਨੇ_ਗਿਣਤੀ, ਬਫਰ.ਅੰਕੜੇ()) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        Mat  dst_ਪੰਕਤੀ(1, dst.ਪੰਕਤੀਆਂ, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.ਪੰਕਤੀਆਂ);

        Auto_ਬਫਰ<Ipp8u> ਬਫਰ;
        ਪੂਰਨ               ਬਫਰ_ਮਾਪ;
        ਜੇ(ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ_Get_ਬਫਰ_ਮਾਪ(src.ਪੰਕਤੀਆਂ, type, &ਬਫਰ_ਮਾਪ) < 0)
            ਵਾਪਸ ਝੂਠ;

        ਬਫਰ.allocate(ਬਫਰ_ਮਾਪ);

        Ipp32s src_ਕਦਮ_3 = (Ipp32s)src.ਕਦਮ[0];
        ਲਈ(ਪੂਰਨ i = 0; i < src.ਖਾਨੇ_ਗਿਣਤੀ; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            ਜੇ(CV_INSTRUMENT_FUN_IPP(ipps_ਛਾਂਟੋ_Radix_ਸੂਚਕਾਂਕ, (ਅਚਲ ਖਾਲੀ*)src.ptr(0, i), src_ਕਦਮ_3, (Ipp32s*)dst_ਪੰਕਤੀ.ptr(), src.ਪੰਕਤੀਆਂ, ਬਫਰ.ਅੰਕੜੇ()) < 0)
                ਵਾਪਸ ਝੂਠ;

            dst_ਪੰਕਤੀ = dst_ਪੰਕਤੀ.reshape(1, dstSub.ਪੰਕਤੀਆਂ);
            dst_ਪੰਕਤੀ.copy_ਮੰਜ਼ਿਲ(dstSub);
        }
    }

    ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*ਛਾਂਟੋ_Func)(ਅਚਲ Mat& src, Mat& dst, ਪੂਰਨ flags);
}

ਖਾਲੀ cv::sort( ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_ਮਾਪ( src, src.type() );
    Mat dst = _dst.getMat();
#ਜੇ !IPP_DISABLE_ਛਾਂਟੋ
    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_ਛਾਂਟੋ(src, dst, flags));
#ਜੇ_ਅੰਤ

    ਸਥਿਰ ਛਾਂਟੋ_Func tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        ਛਾਂਟੋ<uchar>, ਛਾਂਟੋ<schar>, ਛਾਂਟੋ<ushort>, ਛਾਂਟੋ<ਛੋਟਾ>,
        ਛਾਂਟੋ<ਪੂਰਨ>, ਛਾਂਟੋ<ਦਸ਼ਮਲਵ>, ਛਾਂਟੋ<ਦੁੱਗਣਾ>, 0
    };
    ਛਾਂਟੋ_Func func = tab[src.ਡੂੰਘਾਈ()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

ਖਾਲੀ cv::ਛਾਂਟੋ_Idx( ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    ਜੇ( dst.ਅੰਕੜੇ == src.ਅੰਕੜੇ )
        _dst.release();
    _dst.create_Same_ਮਾਪ( src, CV_32S );
    dst = _dst.getMat();

#ਜੇ !IPP_DISABLE_ਛਾਂਟੋ
    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_ਛਾਂਟੋ_Idx(src, dst, flags));
#ਜੇ_ਅੰਤ

    ਸਥਿਰ ਛਾਂਟੋ_Func tab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        ਛਾਂਟੋ_Idx_2<uchar>, ਛਾਂਟੋ_Idx_2<schar>, ਛਾਂਟੋ_Idx_2<ushort>, ਛਾਂਟੋ_Idx_2<ਛੋਟਾ>,
        ਛਾਂਟੋ_Idx_2<ਪੂਰਨ>, ਛਾਂਟੋ_Idx_2<ਦਸ਼ਮਲਵ>, ਛਾਂਟੋ_Idx_2<ਦੁੱਗਣਾ>, 0
    };
    ਛਾਂਟੋ_Func func = tab[src.ਡੂੰਘਾਈ()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
