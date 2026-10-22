// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#परिभाषय IPP_DISABLE_REDUCE 1
#परिभाषय IPP_DISABLE_क्रमय   0

/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

शून्य cv::swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.पङ्क्तिकाः, b.पङ्क्तिकाः);
    std::swap(a.स्तम्भसङ्ख्या, b.स्तम्भसङ्ख्या);
    std::swap(a.दत्त, b.दत्त);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size, b.size);
    std::swap(a.चरणः, b.चरणः);
}


शून्य cv::hconcat(ध्रुव Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    यदि( nsrc == 0 || !src )
    {
        _dst.release();
        प्रत्यागम;
    }

    पूर्णाङ्क योग_स्तम्भसङ्ख्या = 0, स्तम्भसङ्ख्या = 0;
    प्रति( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert( src[i].dims <= 2 &&
                   src[i].पङ्क्तिकाः == src[0].पङ्क्तिकाः &&
                   src[i].type() == src[0].type());
        योग_स्तम्भसङ्ख्या += src[i].स्तम्भसङ्ख्या;
    }
    _dst.create( src[0].पङ्क्तिकाः, योग_स्तम्भसङ्ख्या, src[0].type());
    Mat dst = _dst.getMat();
    प्रति( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart = dst(Rect(स्तम्भसङ्ख्या, 0, src[i].स्तम्भसङ्ख्या, src[i].पङ्क्तिकाः));
        src[i].copy_गन्तव्यम्(dpart);
        स्तम्भसङ्ख्या += src[i].स्तम्भसङ्ख्या;
    }
}

शून्य cv::hconcat(निवेशः_Array src1, निवेशः_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    hconcat(src, 2, dst);
}

शून्य cv::hconcat(निवेशः_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_सदिशम्(src);
    hconcat(!src.empty() ? &src[0] : शून्यसूचकः, src.size(), dst);
}

शून्य cv::vconcat(ध्रुव Mat* src, size_t nsrc, OutputArray _dst)
{
    CV_TRACE_FUNCTION_SKIP_NESTED()

    यदि( nsrc == 0 || !src )
    {
        _dst.release();
        प्रत्यागम;
    }

    पूर्णाङ्क योग_पङ्क्तिकाः = 0, पङ्क्तिकाः = 0;
    प्रति( size_t i = 0; i < nsrc; i++ )
    {
        CV_Assert(src[i].dims <= 2 &&
                  src[i].स्तम्भसङ्ख्या == src[0].स्तम्भसङ्ख्या &&
                  src[i].type() == src[0].type());
        योग_पङ्क्तिकाः += src[i].पङ्क्तिकाः;
    }
    _dst.create( योग_पङ्क्तिकाः, src[0].स्तम्भसङ्ख्या, src[0].type());
    Mat dst = _dst.getMat();
    प्रति( size_t i = 0; i < nsrc; i++ )
    {
        Mat dpart(dst, Rect(0, पङ्क्तिकाः, src[i].स्तम्भसङ्ख्या, src[i].पङ्क्तिकाः));
        src[i].copy_गन्तव्यम्(dpart);
        पङ्क्तिकाः += src[i].पङ्क्तिकाः;
    }
}

शून्य cv::vconcat(निवेशः_Array src1, निवेशः_Array src2, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    Mat src[] = {src1.getMat(), src2.getMat()};
    vconcat(src, 2, dst);
}

शून्य cv::vconcat(निवेशः_Array _src, OutputArray dst)
{
    CV_INSTRUMENT_REGION();

    std::vector<Mat> src;
    _src.get_Mat_सदिशम्(src);
    vconcat(!src.empty() ? &src[0] : शून्यसूचकः, src.size(), dst);
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#यदि_परिभाषितम् HAVE_OPENCL

नामाकाश cv {

स्थिर तार्किक ocl_समुच्चयः_तटस्थः( निवेशः_Output_Array _m, ध्रुव Scalar& s )
{
    पूर्णाङ्क type = _m.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type), kercn = cn, पङ्क्तिकाः_प्रत्येकम्_WI = 1;
    पूर्णाङ्क sctype = CV_MAKE_TYPE(गभीरता, cn == 3 ? 4 : cn);
    यदि (ocl::Device::getDefault().isIntel())
    {
        पङ्क्तिकाः_प्रत्येकम्_WI = 4;
        यदि (cn == 1)
        {
            kercn = std::min(ocl::predict_Optimal_सदिशम्_विस्तारः(_m), 4);
            यदि (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::समुच्चयः_तटस्थः_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memop_Type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, kercn)),
                         ocl::memop_Type_गन्तव्यम्_Str(गभीरता), cn,
                         ocl::memop_Type_गन्तव्यम्_Str(sctype),
                         kercn, पङ्क्तिकाः_प्रत्येकम्_WI));
    यदि (k.empty())
        प्रत्यागम असत्य;

    UMat m = _m.getUMat();
    k.तर्काः(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(Mat(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m.स्तम्भसङ्ख्या * cn / kercn, ((size_t)m.पङ्क्तिकाः + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, शून्यसूचकः, असत्य);
}

}

#यदि_अन्तः

शून्य cv::समुच्चयः_तटस्थः( निवेशः_Output_Array _m, ध्रुव Scalar& s )
{
    CV_INSTRUMENT_REGION();

    CV_Assert( _m.dims() <= 2 );

    CV_OCL_सततम्(_m.isUMat(),
               ocl_समुच्चयः_तटस्थः(_m, s))

    Mat m = _m.getMat();
    पूर्णाङ्क पङ्क्तिकाः = m.पङ्क्तिकाः, स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या, type = m.type();

    यदि( type == CV_32FC1 )
    {
        भिन्न* दत्त = m.ptr<भिन्न>();
        भिन्न val = (भिन्न)s[0];
        size_t चरणः = m.चरणः/परिमाण(दत्त[0]);

        प्रति( पूर्णाङ्क i = 0; i < पङ्क्तिकाः; i++, दत्त += चरणः )
        {
            प्रति( पूर्णाङ्क j = 0; j < स्तम्भसङ्ख्या; j++ )
                दत्त[j] = 0;
            यदि( i < स्तम्भसङ्ख्या )
                दत्त[i] = val;
        }
    }
    अन्यथा यदि( type == CV_64FC1 )
    {
        द्विगुण* दत्त = m.ptr<द्विगुण>();
        द्विगुण val = s[0];
        size_t चरणः = m.चरणः/परिमाण(दत्त[0]);

        प्रति( पूर्णाङ्क i = 0; i < पङ्क्तिकाः; i++, दत्त += चरणः )
        {
            std::fill(दत्त, दत्त + स्तम्भसङ्ख्या, 0.0);
            यदि (i < स्तम्भसङ्ख्या)
                दत्त[i] = val;
        }
    }
    अन्यथा
    {
        m = Scalar(0);
        m.diag() = s;
    }
}


नामाकाश cv {

UMat UMat::eye(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    प्रत्यागम UMat::eye(आकार(स्तम्भसङ्ख्या, पङ्क्तिकाः), type, usageFlags);
}

UMat UMat::eye(आकार size, पूर्णाङ्क type, UMatUsageFlags usageFlags)
{
    UMat m(size, type, usageFlags);
    समुच्चयः_तटस्थः(m);
    प्रत्यागम m;
}

}  // namespace

//////////////////////////////////////////// trace ///////////////////////////////////////////

cv::Scalar cv::trace( निवेशः_Array _m )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    CV_Assert( m.dims <= 2 );
    पूर्णाङ्क type = m.type();
    पूर्णाङ्क nm = std::min(m.पङ्क्तिकाः, m.स्तम्भसङ्ख्या);

    यदि( type == CV_32FC1 )
    {
        ध्रुव भिन्न* ptr = m.ptr<भिन्न>();
        size_t चरणः = m.चरणः/परिमाण(ptr[0]) + 1;
        द्विगुण _s = 0;
        प्रति( पूर्णाङ्क i = 0; i < nm; i++ )
            _s += ptr[i*चरणः];
        प्रत्यागम _s;
    }

    यदि( type == CV_64FC1 )
    {
        ध्रुव द्विगुण* ptr = m.ptr<द्विगुण>();
        size_t चरणः = m.चरणः/परिमाण(ptr[0]) + 1;
        द्विगुण _s = 0;
        प्रति( पूर्णाङ्क i = 0; i < nm; i++ )
            _s += ptr[i*चरणः];
        प्रत्यागम _s;
    }

    प्रत्यागम cv::योग_2(m.diag());
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

शून्य cv::completeSymm( निवेशः_Output_Array _m, तार्किक LtoR )
{
    CV_INSTRUMENT_REGION();

    Mat m = _m.getMat();
    size_t चरणः = m.चरणः, esz = m.elem_आकार();
    CV_Assert( m.dims <= 2 && m.पङ्क्तिकाः == m.स्तम्भसङ्ख्या );

    पूर्णाङ्क पङ्क्तिकाः = m.पङ्क्तिकाः;
    पूर्णाङ्क j0 = 0, j1 = पङ्क्तिकाः;

    uchar* दत्त = m.ptr();
    प्रति( पूर्णाङ्क i = 0; i < पङ्क्तिकाः; i++ )
    {
        यदि( !LtoR ) j1 = i; अन्यथा j0 = i+1;
        प्रति( पूर्णाङ्क j = j0; j < j1; j++ )
            स्मृतिं_प्रतिलिख(दत्त + (i*चरणः + j*esz), दत्त + (j*चरणः + i*esz), esz);
    }
}


cv::Mat cv::Mat::cross(निवेशः_Array _m) ध्रुव
{
    Mat m = _m.getMat();
    पूर्णाङ्क tp = type(), d = CV_MAT_गभीरता(tp);
    CV_Assert( dims <= 2 && m.dims <= 2 && size() == m.size() && tp == m.type() &&
        ((पङ्क्तिकाः == 3 && स्तम्भसङ्ख्या == 1) || (स्तम्भसङ्ख्या*channels() == 3 && पङ्क्तिकाः == 1)));
    Mat परिणाम(पङ्क्तिकाः, स्तम्भसङ्ख्या, tp);

    यदि( d == CV_32F )
    {
        ध्रुव भिन्न *a = (ध्रुव भिन्न*)दत्त, *b = (ध्रुव भिन्न*)m.दत्त;
        भिन्न* c = (भिन्न*)परिणाम.दत्त;
        size_t lda = पङ्क्तिकाः > 1 ? चरणः/परिमाण(a[0]) : 1;
        size_t ldb = पङ्क्तिकाः > 1 ? m.चरणः/परिमाण(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    अन्यथा यदि( d == CV_64F )
    {
        ध्रुव द्विगुण *a = (ध्रुव द्विगुण*)दत्त, *b = (ध्रुव द्विगुण*)m.दत्त;
        द्विगुण* c = (द्विगुण*)परिणाम.दत्त;
        size_t lda = पङ्क्तिकाः > 1 ? चरणः/परिमाण(a[0]) : 1;
        size_t ldb = पङ्क्तिकाः > 1 ? m.चरणः/परिमाण(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    प्रत्यागम परिणाम;
}


////////////////////////////////////////// reduce ////////////////////////////////////////////

नामाकाश cv
{

प्रकारनिर्देश शून्य (*Reduce_योग_Func)(ध्रुव Mat& src, Mat& dst);
Reduce_योग_Func getReduceCSumFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);
Reduce_योग_Func getReduceRSumFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);
Reduce_योग_Func getReduceRSum2Func(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);

प्रतिरूपम् <प्रकारनाम T, प्रकारनाम WT, प्रकारनाम Op>
संरचना ReduceR_SIMD
{
    पूर्णाङ्क संक्रियकः()(ध्रुव T*, पूर्णाङ्क start, पूर्णाङ्क, WT*, ध्रुव Op&) ध्रुव
    {
        प्रत्यागम start;
    }
};


प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST, प्रकारनाम WT, वर्ग Op, वर्ग OpInit>
वर्ग ReduceR_Invoker : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
  ReduceR_Invoker(ध्रुव Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit),बफर(srcmat.size().विस्तारः*srcmat.channels())
  {
  }
  शून्य संक्रियकः()(ध्रुव Range& range) ध्रुव CV_OVERRIDE
  {
    ध्रुव T* src = srcmat.ptr<T>();
    ध्रुव size_t srcstep = srcmat.चरणः/परिमाण(src[0]);
    WT* buf = बफर.दत्त();
    ST* dst = dstmat.ptr<ST>();
    पूर्णाङ्क i = 0;

    प्रति( i = range.start ; i < range.end; i++ )
        buf[i] = opInit(src[i]);

    पूर्णाङ्क height = srcmat.size().height;
    प्रति( ; --height; )
    {
        src += srcstep;
        ReduceR_SIMD<T, WT, Op> simd_op;
        i = simd_op(src, range.start, range.end, buf, op);
        #यदि CV_ENABLE_UNROLLED
        प्रति(; i <= range.end - 4; i += 4 )
        {
            WT s0, s1;
            s0 = op(buf[i], (WT)src[i]);
            s1 = op(buf[i+1], (WT)src[i+1]);
            buf[i] = s0; buf[i+1] = s1;

            s0 = op(buf[i+2], (WT)src[i+2]);
            s1 = op(buf[i+3], (WT)src[i+3]);
            buf[i+2] = s0; buf[i+3] = s1;
        }
        #यदि_अन्तः
        प्रति( ; i < range.end; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    प्रति( i = range.start ; i < range.end; i++ )
        dst[i] = (ST)buf[i];
  }
गुप्त:
  ध्रुव Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
  परिवर्तनीय Auto_बफर<WT> बफर;
};

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST, वर्ग Op, वर्ग OpInit = OpNop<ST> > स्थिर शून्य
reduceR_( ध्रुव Mat& srcmat, Mat& dstmat)
{
    प्रकारनिर्देश प्रकारनाम Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceR_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    //group columns by 64 bytes for data locality
    parallel_for_(Range(0, srcmat.size().विस्तारः*srcmat.channels()), body, srcmat.size().विस्तारः*CV_ELEM_आकार(srcmat.गभीरता())/64);
}

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST, प्रकारनाम WT, वर्ग Op, वर्ग OpInit>
वर्ग ReduceC_Invoker : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
  ReduceC_Invoker(ध्रुव Mat& aSrcmat, Mat& aDstmat, Op& aOp, OpInit& aOpInit)
                 :srcmat(aSrcmat),dstmat(aDstmat),op(aOp),opInit(aOpInit)
  {
  }
  शून्य संक्रियकः()(ध्रुव Range& range) ध्रुव CV_OVERRIDE
  {
    ध्रुव पूर्णाङ्क cn = srcmat.channels();
    ध्रुव पूर्णाङ्क विस्तारः = srcmat.size().विस्तारः*cn;
    Auto_बफर<WT> cumul(cn);
    प्रति( पूर्णाङ्क y = range.start; y < range.end; y++ )
    {
        ध्रुव T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        यदि( विस्तारः == cn )
        {
          प्रति( पूर्णाङ्क k = 0; k < cn; k++ )
              dst[k] = (ST)opInit(src[k]);
        }
        अन्यथा
        {
            प्रति(पूर्णाङ्क k = 0; k < cn ; ++k )
              cumul[k] = opInit(src[k]);
            प्रति(पूर्णाङ्क k = cn ; k < विस्तारः ; k += cn )
            {
                प्रति (पूर्णाङ्क c = 0 ; c < cn ; ++c)
                  cumul[c] = op(cumul[c], src[k+c]);
            }
            प्रति(पूर्णाङ्क k = 0 ; k < cn ; ++k )
              dst[k] = (ST)cumul[k];
        }
    }
  }
गुप्त:
  ध्रुव Mat& srcmat;
  Mat& dstmat;
  Op& op;
  OpInit& opInit;
};

प्रतिरूपम्<प्रकारनाम T, प्रकारनाम ST, वर्ग Op, वर्ग OpInit = OpNop<ST> > स्थिर शून्य
reduceC_( ध्रुव Mat& srcmat, Mat& dstmat)
{
    प्रकारनिर्देश प्रकारनाम Op::rtype WT;
    Op op;
    OpInit opInit;

    ReduceC_Invoker<T, ST, WT, Op, OpInit> body(srcmat, dstmat, op, opInit);
    parallel_for_(Range(0, srcmat.size().height), body);
}

प्रकारनिर्देश शून्य (*ReduceFunc)( ध्रुव Mat& src, Mat& dst );

}

#परिभाषय reduce_योग_R8u32s  reduceR_<uchar, पूर्णाङ्क,   Op_सङ्कलय<पूर्णाङ्क>, OpNop<पूर्णाङ्क> >
#परिभाषय reduce_योग_R8u32f  reduceR_<uchar, भिन्न, Op_सङ्कलय<पूर्णाङ्क>, OpNop<पूर्णाङ्क> >
#परिभाषय reduce_योग_R8u64f  reduceR_<uchar, द्विगुण,Op_सङ्कलय<पूर्णाङ्क>, OpNop<पूर्णाङ्क> >
#परिभाषय reduce_योग_R16u32f reduceR_<ushort,भिन्न, Op_सङ्कलय<भिन्न> >
#परिभाषय reduce_योग_R16u64f reduceR_<ushort,द्विगुण,Op_सङ्कलय<द्विगुण> >
#परिभाषय reduce_योग_R16s32f reduceR_<ह्रस्व, भिन्न, Op_सङ्कलय<भिन्न> >
#परिभाषय reduce_योग_R16s64f reduceR_<ह्रस्व, द्विगुण,Op_सङ्कलय<द्विगुण> >
#परिभाषय reduce_योग_R32f32f reduceR_<भिन्न, भिन्न, Op_सङ्कलय<भिन्न> >
#परिभाषय reduce_योग_R32f64f reduceR_<भिन्न, द्विगुण,Op_सङ्कलय<द्विगुण> >
#परिभाषय reduce_योग_R64f64f reduceR_<द्विगुण,द्विगुण,Op_सङ्कलय<द्विगुण> >

#परिभाषय reduceSum2R8u32s  reduceR_<uchar, पूर्णाङ्क,   Op_सङ्कलय_Sqr<पूर्णाङ्क>,   OpSqr<पूर्णाङ्क> >
#परिभाषय reduceSum2R8u32f  reduceR_<uchar, भिन्न, Op_सङ्कलय_Sqr<पूर्णाङ्क>,   OpSqr<पूर्णाङ्क> >
#परिभाषय reduceSum2R8u64f  reduceR_<uchar, द्विगुण,Op_सङ्कलय_Sqr<पूर्णाङ्क>,   OpSqr<पूर्णाङ्क> >
#परिभाषय reduceSum2R16u32f reduceR_<ushort,भिन्न, Op_सङ्कलय_Sqr<भिन्न>, OpSqr<भिन्न> >
#परिभाषय reduceSum2R16u64f reduceR_<ushort,द्विगुण,Op_सङ्कलय_Sqr<द्विगुण>,OpSqr<द्विगुण> >
#परिभाषय reduceSum2R16s32f reduceR_<ह्रस्व, भिन्न, Op_सङ्कलय_Sqr<भिन्न>, OpSqr<भिन्न> >
#परिभाषय reduceSum2R16s64f reduceR_<ह्रस्व, द्विगुण,Op_सङ्कलय_Sqr<द्विगुण>,OpSqr<द्विगुण> >
#परिभाषय reduceSum2R32f32f reduceR_<भिन्न, भिन्न, Op_सङ्कलय_Sqr<भिन्न>, OpSqr<भिन्न> >
#परिभाषय reduceSum2R32f64f reduceR_<भिन्न, द्विगुण,Op_सङ्कलय_Sqr<द्विगुण>,OpSqr<द्विगुण> >
#परिभाषय reduceSum2R64f64f reduceR_<द्विगुण,द्विगुण,Op_सङ्कलय_Sqr<द्विगुण>,OpSqr<द्विगुण> >

#परिभाषय reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#परिभाषय reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#परिभाषय reduceMaxR16s reduceR_<ह्रस्व, ह्रस्व, OpMax<ह्रस्व> >
#परिभाषय reduceMaxR32f reduceR_<भिन्न, भिन्न, OpMax<भिन्न> >
#परिभाषय reduceMaxR64f reduceR_<द्विगुण,द्विगुण,OpMax<द्विगुण> >

#परिभाषय reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#परिभाषय reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#परिभाषय reduceMinR16s reduceR_<ह्रस्व, ह्रस्व, OpMin<ह्रस्व> >
#परिभाषय reduceMinR32f reduceR_<भिन्न, भिन्न, OpMin<भिन्न> >
#परिभाषय reduceMinR64f reduceR_<द्विगुण,द्विगुण,OpMin<द्विगुण> >

#यदि defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
स्थिर अन्तःस्थ तार्किक ipp_reduce_योग_C_8u16u16s32f_64f(ध्रुव cv::Mat& srcmat, cv::Mat& dstmat)
{
    पूर्णाङ्क sstep = (पूर्णाङ्क)srcmat.चरणः, stype = srcmat.type(),
            ddepth = dstmat.गभीरता();

    Ippi_आकार roisize = { srcmat.size().विस्तारः, 1 };

    प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * Ippi_योग)(ध्रुव शून्य * pSrc, पूर्णाङ्क src_चरणः_3, Ippi_आकार roi_आकार, Ipp64f* p_योग);
    प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL * Ippi_योग_Hint)(ध्रुव शून्य * pSrc, पूर्णाङ्क src_चरणः_3, Ippi_आकार roi_आकार, Ipp64f* p_योग, IppHintAlgorithm hint);
    Ippi_योग ippi_योग = 0;
    Ippi_योग_Hint ippi_योग_Hint = 0;

    यदि(ddepth == CV_64F)
    {
        ippi_योग =
            stype == CV_8UC1 ? (Ippi_योग)ippi_योग_8u_C1_R :
            stype == CV_8UC3 ? (Ippi_योग)ippi_योग_8u_C3_R :
            stype == CV_8UC4 ? (Ippi_योग)ippi_योग_8u_C4_R :
            stype == CV_16UC1 ? (Ippi_योग)ippi_योग_16u_C1_R :
            stype == CV_16UC3 ? (Ippi_योग)ippi_योग_16u_C3_R :
            stype == CV_16UC4 ? (Ippi_योग)ippi_योग_16u_C4_R :
            stype == CV_16SC1 ? (Ippi_योग)ippi_योग_16s_C1_R :
            stype == CV_16SC3 ? (Ippi_योग)ippi_योग_16s_C3_R :
            stype == CV_16SC4 ? (Ippi_योग)ippi_योग_16s_C4_R : 0;
        ippi_योग_Hint =
            stype == CV_32FC1 ? (Ippi_योग_Hint)ippi_योग_32f_C1_R :
            stype == CV_32FC3 ? (Ippi_योग_Hint)ippi_योग_32f_C3_R :
            stype == CV_32FC4 ? (Ippi_योग_Hint)ippi_योग_32f_C4_R : 0;
    }

    यदि(ippi_योग)
    {
        प्रति(पूर्णाङ्क y = 0; y < srcmat.size().height; y++)
        {
            यदि(CV_INSTRUMENT_FUN_IPP(ippi_योग, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }
    अन्यथा यदि(ippi_योग_Hint)
    {
        प्रति(पूर्णाङ्क y = 0; y < srcmat.size().height; y++)
        {
            यदि(CV_INSTRUMENT_FUN_IPP(ippi_योग_Hint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }

    प्रत्यागम असत्य;
}

स्थिर अन्तःस्थ शून्य reduce_योग_C_8u16u16s32f_64f(ध्रुव cv::Mat& srcmat, cv::Mat& dstmat)
{
    CV_IPP_सततम्_FAST(ipp_reduce_योग_C_8u16u16s32f_64f(srcmat, dstmat));

    cv::ReduceFunc func = 0;

    यदि(dstmat.गभीरता() == CV_64F)
    {
        पूर्णाङ्क sdepth = CV_MAT_गभीरता(srcmat.type());
        func =
            sdepth == CV_8U ? (cv::ReduceFunc)cv::reduceC_<uchar, द्विगुण,   cv::Op_सङ्कलय<द्विगुण> > :
            sdepth == CV_16U ? (cv::ReduceFunc)cv::reduceC_<ushort, द्विगुण,   cv::Op_सङ्कलय<द्विगुण> > :
            sdepth == CV_16S ? (cv::ReduceFunc)cv::reduceC_<ह्रस्व, द्विगुण,   cv::Op_सङ्कलय<द्विगुण> > :
            sdepth == CV_32F ? (cv::ReduceFunc)cv::reduceC_<भिन्न, द्विगुण,   cv::Op_सङ्कलय<द्विगुण> > : 0;
    }
    CV_Assert(func);

    func(srcmat, dstmat);
}

#यदि_अन्तः

#परिभाषय reduce_योग_C8u32s  reduceC_<uchar, पूर्णाङ्क,   Op_सङ्कलय<पूर्णाङ्क>, OpNop<पूर्णाङ्क> >
#परिभाषय reduce_योग_C8u32f  reduceC_<uchar, भिन्न, Op_सङ्कलय<पूर्णाङ्क>, OpNop<पूर्णाङ्क> >
#परिभाषय reduce_योग_C16u32f reduceC_<ushort,भिन्न, Op_सङ्कलय<भिन्न> >
#परिभाषय reduce_योग_C16s32f reduceC_<ह्रस्व, भिन्न, Op_सङ्कलय<भिन्न> >
#परिभाषय reduce_योग_C32f32f reduceC_<भिन्न, भिन्न, Op_सङ्कलय<भिन्न> >
#परिभाषय reduce_योग_C64f64f reduceC_<द्विगुण,द्विगुण,Op_सङ्कलय<द्विगुण> >

#परिभाषय reduceSum2C8u32s  reduceC_<uchar, पूर्णाङ्क,   Op_सङ्कलय_Sqr<पूर्णाङ्क>,   OpSqr<पूर्णाङ्क> >
#परिभाषय reduceSum2C8u32f  reduceC_<uchar, भिन्न, Op_सङ्कलय_Sqr<पूर्णाङ्क>,   OpSqr<पूर्णाङ्क> >
#परिभाषय reduceSum2C16u32f reduceC_<ushort,भिन्न, Op_सङ्कलय_Sqr<भिन्न>, OpSqr<भिन्न> >
#परिभाषय reduceSum2C16s32f reduceC_<ह्रस्व, भिन्न, Op_सङ्कलय_Sqr<भिन्न>, OpSqr<भिन्न> >
#परिभाषय reduceSum2C32f32f reduceC_<भिन्न, भिन्न, Op_सङ्कलय_Sqr<भिन्न>, OpSqr<भिन्न> >
#परिभाषय reduceSum2C64f64f reduceC_<द्विगुण,द्विगुण,Op_सङ्कलय_Sqr<द्विगुण>,OpSqr<द्विगुण> >

#यदि defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#परिभाषय reduce_योग_C8u64f  reduce_योग_C_8u16u16s32f_64f
#परिभाषय reduce_योग_C16u64f reduce_योग_C_8u16u16s32f_64f
#परिभाषय reduce_योग_C16s64f reduce_योग_C_8u16u16s32f_64f
#परिभाषय reduce_योग_C32f64f reduce_योग_C_8u16u16s32f_64f
#अन्यथा
#परिभाषय reduce_योग_C8u64f  reduceC_<uchar, द्विगुण,Op_सङ्कलय<पूर्णाङ्क>, OpNop<पूर्णाङ्क> >
#परिभाषय reduce_योग_C16u64f reduceC_<ushort,द्विगुण,Op_सङ्कलय<द्विगुण> >
#परिभाषय reduce_योग_C16s64f reduceC_<ह्रस्व, द्विगुण,Op_सङ्कलय<द्विगुण> >
#परिभाषय reduce_योग_C32f64f reduceC_<भिन्न, द्विगुण,Op_सङ्कलय<द्विगुण> >
#यदि_अन्तः

#परिभाषय reduceSum2C8u64f  reduceC_<uchar, द्विगुण,Op_सङ्कलय_Sqr<पूर्णाङ्क>,   OpSqr<पूर्णाङ्क> >
#परिभाषय reduceSum2C16u64f reduceC_<ushort,द्विगुण,Op_सङ्कलय_Sqr<द्विगुण>,OpSqr<द्विगुण> >
#परिभाषय reduceSum2C16s64f reduceC_<ह्रस्व, द्विगुण,Op_सङ्कलय_Sqr<द्विगुण>,OpSqr<द्विगुण> >
#परिभाषय reduceSum2C32f64f reduceC_<भिन्न, द्विगुण,Op_सङ्कलय_Sqr<द्विगुण>,OpSqr<द्विगुण> >

#यदि defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
#परिभाषय REDUCE_OP(favor, optype, type1, type2) \
स्थिर अन्तःस्थ तार्किक ipp_reduce##optype##C##favor(ध्रुव cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    यदि((srcmat.channels() == 1)) \
    { \
        पूर्णाङ्क sstep = (पूर्णाङ्क)srcmat.चरणः; \
        प्रकारनिर्देश Ipp##favor IppType; \
        Ippi_आकार roisize = ippi_आकार(srcmat.size().विस्तारः, 1);\
        प्रति(पूर्णाङ्क y = 0; y < srcmat.size().height; y++)\
        {\
            यदि(CV_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                प्रत्यागम असत्य;\
        }\
        प्रत्यागम सत्य;\
    }\
    प्रत्यागम असत्य; \
} \
स्थिर अन्तःस्थ शून्य reduce##optype##C##favor(ध्रुव cv::Mat& srcmat, cv::Mat& dstmat) \
{ \
    CV_IPP_सततम्_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    cv::reduceC_ < type1, type2, cv::Op##optype < type2 > >(srcmat, dstmat); \
}
#यदि_अन्तः

#यदि defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, ह्रस्व, ह्रस्व)
REDUCE_OP(32f, Max, भिन्न, भिन्न)
#अन्यथा
#परिभाषय reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#परिभाषय reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#परिभाषय reduceMaxC16s reduceC_<ह्रस्व, ह्रस्व, OpMax<ह्रस्व> >
#परिभाषय reduceMaxC32f reduceC_<भिन्न, भिन्न, OpMax<भिन्न> >
#यदि_अन्तः
#परिभाषय reduceMaxC64f reduceC_<द्विगुण,द्विगुण,OpMax<द्विगुण> >

#यदि defined(HAVE_IPP) && !IPP_DISABLE_REDUCE
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, ह्रस्व, ह्रस्व)
REDUCE_OP(32f, Min, भिन्न, भिन्न)
#अन्यथा
#परिभाषय reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#परिभाषय reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#परिभाषय reduceMinC16s reduceC_<ह्रस्व, ह्रस्व, OpMin<ह्रस्व> >
#परिभाषय reduceMinC32f reduceC_<भिन्न, भिन्न, OpMin<भिन्न> >
#यदि_अन्तः
#परिभाषय reduceMinC64f reduceC_<द्विगुण,द्विगुण,OpMin<द्विगुण> >

#यदि_परिभाषितम् HAVE_OPENCL

नामाकाश cv {

स्थिर तार्किक ocl_reduce(निवेशः_Array _src, OutputArray _dst,
                       पूर्णाङ्क dim, पूर्णाङ्क op, पूर्णाङ्क op0, पूर्णाङ्क stype, पूर्णाङ्क dtype)
{
    ध्रुव पूर्णाङ्क min_opt_स्तम्भसङ्ख्या = 128, buf_स्तम्भसङ्ख्या = 32;
    पूर्णाङ्क sdepth = CV_MAT_गभीरता(stype), cn = CV_MAT_CN(stype),
            ddepth = CV_MAT_गभीरता(dtype), ddepth0 = ddepth;
    ध्रुव ocl::Device &defDev = ocl::Device::getDefault();
    तार्किक doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.max_Work_गणः_आकार();
    तार्किक useOptimized = 1 == dim && _src.स्तम्भसङ्ख्या() > min_opt_स्तम्भसङ्ख्या && (wgs >= buf_स्तम्भसङ्ख्या);

    यदि (!doubleSupport && (sdepth == CV_64F || ddepth == CV_64F))
        प्रत्यागम असत्य;

    यदि (op == REDUCE_AVG)
    {
        यदि (sdepth < CV_32S && ddepth < CV_32S)
            ddepth = CV_32S;
    }

    ध्रुव वर्ण * ध्रुव ops[5] = { "OCL_CV_REDUCE_SUM", "OCL_CV_REDUCE_AVG",
                                  "OCL_CV_REDUCE_MAX", "OCL_CV_REDUCE_MIN",
                                  "OCL_CV_REDUCE_SUM2"};
    पूर्णाङ्क wdepth = std::max(ddepth, CV_32F);
    यदि (useOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_स्तम्भसङ्ख्या);
        यदि (defDev.isIntel())
        {
            स्थिर ध्रुव size_t max_Item_अन्तर्_गणः_गणनम् = 16;
            tileHeight = min(tileHeight, defDev.local_Mem_आकार() / buf_स्तम्भसङ्ख्या / CV_ELEM_आकार(CV_MAKETYPE(wdepth, cn)) / max_Item_अन्तर्_गणः_गणनम्);
        }
        वर्ण cvt[3][50];
        cv::String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%zu -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_स्तम्भसङ्ख्या, tileHeight, ops[op], cn, ddepth,
                                            ocl::type_गन्तव्यम्_Str(sdepth),
                                            ocl::type_गन्तव्यम्_Str(ddepth),
                                            ocl::type_गन्तव्यम्_Str(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], परिमाण(cvt[0])),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1], परिमाण(cvt[1])),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2], परिमाण(cvt[2])),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        यदि (k.empty())
            प्रत्यागम असत्य;
        UMat src = _src.getUMat();
        आकार dsize(1, src.पङ्क्तिकाः);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        यदि (op0 == REDUCE_AVG)
            k.तर्काः(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_आकार(dst), 1.0f / src.स्तम्भसङ्ख्या);
        अन्यथा
            k.तर्काः(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::Write_Only_No_आकार(dst));

        size_t local_आकार[2] = { (size_t)buf_स्तम्भसङ्ख्या, (size_t)tileHeight};
        size_t global_आकार[2] = { (size_t)buf_स्तम्भसङ्ख्या, (size_t)src.पङ्क्तिकाः };
        प्रत्यागम k.सततम्(2, global_आकार, local_आकार, असत्य);
    }
    अन्यथा
    {
        वर्ण cvt[2][50];
        cv::String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::type_गन्तव्यम्_Str(useOptimized ? ddepth : sdepth),
                                      ocl::type_गन्तव्यम्_Str(ddepth), ocl::type_गन्तव्यम्_Str(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0], परिमाण(cvt[0])),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0], परिमाण(cvt[0])),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1], परिमाण(cvt[1])),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        यदि (k.empty())
            प्रत्यागम असत्य;

        UMat src = _src.getUMat();
        आकार dsize(dim == 0 ? src.स्तम्भसङ्ख्या : 1, dim == 0 ? 1 : src.पङ्क्तिकाः);
        _dst.create(dsize, dtype);
        UMat dst = _dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::Write_Only_No_आकार(dst);

        यदि (op0 == REDUCE_AVG)
            k.तर्काः(srcarg, temparg, 1.0f / (dim == 0 ? src.पङ्क्तिकाः : src.स्तम्भसङ्ख्या));
        अन्यथा
            k.तर्काः(srcarg, temparg);

        size_t globalsize = std::max(dsize.विस्तारः, dsize.height);
        प्रत्यागम k.सततम्(1, &globalsize, रिक्त, असत्य);
    }
}

}

#यदि_अन्तः

शून्य cv::reduce(निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क dim, पूर्णाङ्क op, पूर्णाङ्क dtype)
{
    CV_INSTRUMENT_REGION();

    यदि (_src.dims() > 2 || op > REDUCE_SUM2)
    {
        CV_CheckGE(dim, -_src.dims(), "reduce: dim is out of range");
        CV_CheckLT(dim, _src.dims(), "reduce: dim is out of range");
        cv::reduce(_src, _dst, cv::ReduceParams(op, {dim}, सत्य, dtype < 0 && _dst.fixedType() ? _dst.गभीरता() : dtype));
        प्रत्यागम;
    }
    पूर्णाङ्क op0 = op;
    पूर्णाङ्क stype = _src.type(), sdepth = CV_MAT_गभीरता(stype), cn = CV_MAT_CN(stype);
    यदि( dtype < 0 )
        dtype = _dst.fixedType() ? _dst.type() : stype;
    dtype = CV_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    पूर्णाङ्क ddepth = CV_MAT_गभीरता(dtype);

    CV_Assert( cn == CV_MAT_CN(dtype) );
    CV_Assert( op == REDUCE_योग || op == REDUCE_MAX ||
               op == REDUCE_MIN || op == REDUCE_AVG ||
               op == REDUCE_SUM2);

    CV_OCL_सततम्(_dst.isUMat(),
               ocl_reduce(_src, _dst, dim, op, op0, stype, dtype))

    // Fake reference to source. Resolves issue 8693 in case of src == dst.
    UMat srcUMat;
    यदि (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    यदि (src.empty()) {
        src.convert_गन्तव्यम्(_dst, dtype);
        प्रत्यागम;
    }
    यदि (src.dims <= 1) {
        CV_Assert(dim == 0);
        dim = 1;
    }

    _dst.create(dim == 0 ? 1 : src.पङ्क्तिकाः, dim == 0 ? src.स्तम्भसङ्ख्या : 1, dtype);
    Mat dst = _dst.getMat(), अस्थायी_2 = dst;

    यदि( op == REDUCE_AVG )
    {
        op = REDUCE_योग;
        यदि( sdepth < CV_32S && ddepth < CV_32S )
        {
            अस्थायी_2.create(dst.पङ्क्तिकाः, dst.स्तम्भसङ्ख्या, CV_32SC(cn));
            ddepth = CV_32S;
        }
    }

    ReduceFunc func = 0;
    यदि( dim == 0 )
    {
        यदि( op == REDUCE_योग )
        {
            Reduce_योग_Func simd_func = getReduceRSumFunc(sdepth, ddepth);
            यदि(simd_func)
                func = (ReduceFunc)simd_func;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_योग_R8u32s;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_योग_R8u32f;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_योग_R8u64f;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_योग_R16u32f;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_योग_R16u64f;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_योग_R16s32f;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_योग_R16s64f;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_योग_R32f32f;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_योग_R32f64f;
            अन्यथा यदि(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_योग_R64f64f;
        }
        अन्यथा यदि(op == REDUCE_MAX)
        {
            यदि(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxR8u;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxR16u;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxR16s;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxR32f;
            अन्यथा यदि(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxR64f;
        }
        अन्यथा यदि(op == REDUCE_MIN)
        {
            यदि(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinR8u;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinR16u;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinR16s;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinR32f;
            अन्यथा यदि(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinR64f;
        }
        अन्यथा यदि( op == REDUCE_SUM2 )
        {
            Reduce_योग_Func simd_func = getReduceRSum2Func(sdepth, ddepth);
            यदि(simd_func)
                func = simd_func;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2R8u32s;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2R8u32f;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2R8u64f;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2R16u32f;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2R16u64f;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2R16s32f;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2R16s64f;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2R32f32f;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2R32f64f;
            अन्यथा यदि(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2R64f64f;
        }
    }
    अन्यथा
    {
        यदि(op == REDUCE_योग)
        {
            Reduce_योग_Func simd_func = getReduceCSumFunc(sdepth, ddepth);
            यदि(simd_func)
                func = (ReduceFunc)simd_func;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_32S)
                func = reduce_योग_C8u32s;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_32F)
                func = reduce_योग_C8u32f;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_64F)
                func = reduce_योग_C8u64f;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_32F)
                func = reduce_योग_C16u32f;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_64F)
                func = reduce_योग_C16u64f;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_32F)
                func = reduce_योग_C16s32f;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_64F)
                func = reduce_योग_C16s64f;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_32F)
                func = reduce_योग_C32f32f;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_64F)
                func = reduce_योग_C32f64f;
            अन्यथा यदि(sdepth == CV_64F && ddepth == CV_64F)
                func = reduce_योग_C64f64f;
        }
        अन्यथा यदि(op == REDUCE_MAX)
        {
            यदि(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMaxC8u;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMaxC16u;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMaxC16s;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMaxC32f;
            अन्यथा यदि(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMaxC64f;
        }
        अन्यथा यदि(op == REDUCE_MIN)
        {
            यदि(sdepth == CV_8U && ddepth == CV_8U)
                func = reduceMinC8u;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_16U)
                func = reduceMinC16u;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_16S)
                func = reduceMinC16s;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceMinC32f;
            अन्यथा यदि(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceMinC64f;
        }
        अन्यथा यदि(op == REDUCE_SUM2)
        {
            यदि(sdepth == CV_8U && ddepth == CV_32S)
                func = reduceSum2C8u32s;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_32F)
                func = reduceSum2C8u32f;
            अन्यथा यदि(sdepth == CV_8U && ddepth == CV_64F)
                func = reduceSum2C8u64f;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_32F)
                func = reduceSum2C16u32f;
            अन्यथा यदि(sdepth == CV_16U && ddepth == CV_64F)
                func = reduceSum2C16u64f;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_32F)
                func = reduceSum2C16s32f;
            अन्यथा यदि(sdepth == CV_16S && ddepth == CV_64F)
                func = reduceSum2C16s64f;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_32F)
                func = reduceSum2C32f32f;
            अन्यथा यदि(sdepth == CV_32F && ddepth == CV_64F)
                func = reduceSum2C32f64f;
            अन्यथा यदि(sdepth == CV_64F && ddepth == CV_64F)
                func = reduceSum2C64f64f;
        }
    }

    यदि( !func )
        CV_Error( cv::Error::StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, अस्थायी_2 );

    यदि( op0 == REDUCE_AVG )
        अस्थायी_2.convert_गन्तव्यम्(dst, dst.type(), 1./(dim == 0 ? src.पङ्क्तिकाः : src.स्तम्भसङ्ख्या));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

नामाकाश cv
{

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य क्रमय( ध्रुव Mat& src, Mat& dst, पूर्णाङ्क flags )
{
    पूर्णाङ्क n, len;
    तार्किक क्रमय_पङ्क्तिकाः = (flags & 1) == क्रमय_EVERY_ROW;
    तार्किक inplace = src.दत्त == dst.दत्त;
    तार्किक क्रमय_Descending = (flags & क्रमय_DESCENDING) != 0;

    यदि( क्रमय_पङ्क्तिकाः )
        n = src.पङ्क्तिकाः, len = src.स्तम्भसङ्ख्या;
    अन्यथा
        n = src.स्तम्भसङ्ख्या, len = src.पङ्क्तिकाः;
    parallel_for_(Range(0, n), [&](ध्रुव Range& range)
    {
        Auto_बफर<T> buf;
        यदि( !क्रमय_पङ्क्तिकाः )
            buf.allocate(len);
        T* bptr = buf.दत्त();

        प्रति( पूर्णाङ्क i = range.start; i < range.end; i++ )
        {
            T* ptr = bptr;
            यदि( क्रमय_पङ्क्तिकाः )
            {
                T* dptr = dst.ptr<T>(i);
                यदि( !inplace )
                {
                    ध्रुव T* sptr = src.ptr<T>(i);
                    स्मृतिं_प्रतिलिख(dptr, sptr, परिमाण(T) * len);
                }
                ptr = dptr;
            }
            अन्यथा
            {
                प्रति( पूर्णाङ्क j = 0; j < len; j++ )
                    ptr[j] = src.ptr<T>(j)[i];
            }

            std::sort( ptr, ptr + len );
            यदि( क्रमय_Descending )
            {
                प्रति( पूर्णाङ्क j = 0; j < len/2; j++ )
                    std::swap(ptr[j], ptr[len-1-j]);
            }

            यदि( !क्रमय_पङ्क्तिकाः )
                प्रति( पूर्णाङ्क j = 0; j < len; j++ )
                    dst.ptr<T>(j)[i] = ptr[j];
        }
    });
}


#यदि defined(HAVE_IPP) && !IPP_DISABLE_क्रमय
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL *Ipp_क्रमय_Func)(शून्य  *pSrcDst, पूर्णाङ्क    len, Ipp8u *p_बफर);

प्रतिरूपम्<प्रकारनाम T, Ipp_स्थितिः (CV_STDCALL *fn)(T*, पूर्णाङ्क, Ipp8u*)>
स्थिर Ipp_स्थितिः CV_STDCALL ipp_क्रमय_Wrap(शून्य* pSrcDst, पूर्णाङ्क len, Ipp8u* p_बफर)
{
    प्रत्यागम fn((T*)pSrcDst, len, p_बफर);
}

स्थिर Ipp_क्रमय_Func get_क्रमय_Func(पूर्णाङ्क गभीरता, तार्किक क्रमय_Descending)
{
    यदि (!क्रमय_Descending)
        प्रत्यागम गभीरता == CV_8U ? ipp_क्रमय_Wrap<Ipp8u, ipps_क्रमय_Radix_Ascend_8u_I> :
            गभीरता == CV_16U ? ipp_क्रमय_Wrap<Ipp16u, ipps_क्रमय_Radix_Ascend_16u_I> :
            गभीरता == CV_16S ? ipp_क्रमय_Wrap<Ipp16s, ipps_क्रमय_Radix_Ascend_16s_I> :
            गभीरता == CV_32S ? ipp_क्रमय_Wrap<Ipp32s, ipps_क्रमय_Radix_Ascend_32s_I> :
            गभीरता == CV_32F ? ipp_क्रमय_Wrap<Ipp32f, ipps_क्रमय_Radix_Ascend_32f_I> :
            गभीरता == CV_64F ? ipp_क्रमय_Wrap<Ipp64f, ipps_क्रमय_Radix_Ascend_64f_I> :
            0;
    अन्यथा
        प्रत्यागम गभीरता == CV_8U ? ipp_क्रमय_Wrap<Ipp8u, ipps_क्रमय_Radix_Descend_8u_I> :
            गभीरता == CV_16U ? ipp_क्रमय_Wrap<Ipp16u, ipps_क्रमय_Radix_Descend_16u_I> :
            गभीरता == CV_16S ? ipp_क्रमय_Wrap<Ipp16s, ipps_क्रमय_Radix_Descend_16s_I> :
            गभीरता == CV_32S ? ipp_क्रमय_Wrap<Ipp32s, ipps_क्रमय_Radix_Descend_32s_I> :
            गभीरता == CV_32F ? ipp_क्रमय_Wrap<Ipp32f, ipps_क्रमय_Radix_Descend_32f_I> :
            गभीरता == CV_64F ? ipp_क्रमय_Wrap<Ipp64f, ipps_क्रमय_Radix_Descend_64f_I> :
            0;
}

स्थिर तार्किक ipp_क्रमय(ध्रुव Mat& src, Mat& dst, पूर्णाङ्क flags)
{
    CV_INSTRUMENT_REGION_IPP();

    तार्किक        क्रमय_पङ्क्तिकाः        = (flags & 1) == क्रमय_EVERY_ROW;
    तार्किक        क्रमय_Descending  = (flags & क्रमय_DESCENDING) != 0;
    तार्किक        inplace         = (src.दत्त == dst.दत्त);
    पूर्णाङ्क         गभीरता           = src.गभीरता();
    Ipp_दत्त_Type type            = ippi_Get_दत्त_Type(गभीरता);

    Ipp_क्रमय_Func ipps_क्रमय_Radix_I = get_क्रमय_Func(गभीरता, क्रमय_Descending);
    यदि(!ipps_क्रमय_Radix_I)
        प्रत्यागम असत्य;

    पूर्णाङ्क n, len;
    यदि( क्रमय_पङ्क्तिकाः )
        n = src.पङ्क्तिकाः, len = src.स्तम्भसङ्ख्या;
    अन्यथा
        n = src.स्तम्भसङ्ख्या, len = src.पङ्क्तिकाः;

    पूर्णाङ्क बफर_आकार;
    यदि(ipps_क्रमय_Radix_Get_बफर_आकार(len, type, &बफर_आकार) < 0)
        प्रत्यागम असत्य;

    // Each row/column sorts independently, so parallelize over n like sort_ does.
    // IPP scratch (buffer) and per-column temporaries are thread-local; a shared flag
    // records IPP failures since the lambda cannot return from ipp_sort.
    चञ्चल तार्किक ok = सत्य;
    parallel_for_(Range(0, n), [&](ध्रुव Range& range)
    {
        Auto_बफर<Ipp8u> बफर(बफर_आकार);
        Mat  row;
        Rect subRect(0,0,1,src.पङ्क्तिकाः);
        यदि( !क्रमय_पङ्क्तिकाः )
            row.create(1, src.पङ्क्तिकाः, src.type());

        प्रति( पूर्णाङ्क i = range.start; i < range.end; i++ )
        {
            यदि( !ok )
                भङ्ग;
            शून्य* ptr;
            यदि( क्रमय_पङ्क्तिकाः )
            {
                uchar* dptr = dst.ptr(i);
                यदि( !inplace )
                    स्मृतिं_प्रतिलिख(dptr, src.ptr(i), src.स्तम्भसङ्ख्या * src.elem_आकार());
                ptr = dptr;
            }
            अन्यथा
            {
                subRect.x = i;
                Mat(src, subRect).copy_गन्तव्यम्(row);
                ptr = row.ptr();
            }

            यदि( CV_INSTRUMENT_FUN_IPP(ipps_क्रमय_Radix_I, ptr, len, बफर.दत्त()) < 0 )
            {
                ok = असत्य;
                भङ्ग;
            }

            यदि( !क्रमय_पङ्क्तिकाः )
            {
                Mat dstSub(dst, subRect);
                row = row.reshape(1, dstSub.पङ्क्तिकाः);
                row.copy_गन्तव्यम्(dstSub);
                row = row.reshape(1, 1);
            }
        }
    });

    प्रत्यागम ok;
}
#यदि_अन्तः

प्रतिरूपम्<प्रकारनाम _Tp> वर्ग LessThanIdx
{
सार्वजनिक:
    LessThanIdx( ध्रुव _Tp* _arr ) : arr(_arr) {}
    तार्किक संक्रियकः()(पूर्णाङ्क a, पूर्णाङ्क b) ध्रुव { प्रत्यागम arr[a] < arr[b]; }
    ध्रुव _Tp* arr;
};

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य क्रमय_Idx_2( ध्रुव Mat& src, Mat& dst, पूर्णाङ्क flags )
{
    Auto_बफर<T> buf;
    Auto_बफर<पूर्णाङ्क> ibuf;
    तार्किक क्रमय_पङ्क्तिकाः = (flags & 1) == क्रमय_EVERY_ROW;
    तार्किक क्रमय_Descending = (flags & क्रमय_DESCENDING) != 0;

    CV_Assert( src.दत्त != dst.दत्त );

    पूर्णाङ्क n, len;
    यदि( क्रमय_पङ्क्तिकाः )
        n = src.पङ्क्तिकाः, len = src.स्तम्भसङ्ख्या;
    अन्यथा
    {
        n = src.स्तम्भसङ्ख्या, len = src.पङ्क्तिकाः;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = buf.दत्त();
    पूर्णाङ्क* _iptr = ibuf.दत्त();

    प्रति( पूर्णाङ्क i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        पूर्णाङ्क* iptr = _iptr;

        यदि( क्रमय_पङ्क्तिकाः )
        {
            ptr = (T*)(src.दत्त + src.चरणः*i);
            iptr = dst.ptr<पूर्णाङ्क>(i);
        }
        अन्यथा
        {
            प्रति( पूर्णाङ्क j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        प्रति( पूर्णाङ्क j = 0; j < len; j++ )
            iptr[j] = j;

        std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
        यदि( क्रमय_Descending )
        {
            प्रति( पूर्णाङ्क j = 0; j < len/2; j++ )
                std::swap(iptr[j], iptr[len-1-j]);
        }

        यदि( !क्रमय_पङ्क्तिकाः )
            प्रति( पूर्णाङ्क j = 0; j < len; j++ )
                dst.ptr<पूर्णाङ्क>(j)[i] = iptr[j];
    }
}

#यदि defined(HAVE_IPP) && !IPP_DISABLE_क्रमय
प्रकारनिर्देश Ipp_स्थितिः (CV_STDCALL *Ipp_क्रमय_सूचकांक_Func)(ध्रुव शून्य*  pSrc, Ipp32s src_Stride_अष्टकानि, Ipp32s *pDstIndx, पूर्णाङ्क len, Ipp8u *p_बफर);

प्रतिरूपम्<प्रकारनाम T, Ipp_स्थितिः (CV_STDCALL *fn)(ध्रुव T*, Ipp32s, Ipp32s*, पूर्णाङ्क, Ipp8u*)>
स्थिर Ipp_स्थितिः CV_STDCALL ipp_क्रमय_सूचकांक_Wrap(ध्रुव शून्य* pSrc, Ipp32s src_Stride_अष्टकानि, Ipp32s *pDstIndx, पूर्णाङ्क len, Ipp8u *p_बफर)
{
    प्रत्यागम fn((ध्रुव T*)pSrc, src_Stride_अष्टकानि, pDstIndx, len, p_बफर);
}

स्थिर Ipp_क्रमय_सूचकांक_Func get_क्रमय_सूचकांक_Func(पूर्णाङ्क गभीरता, तार्किक क्रमय_Descending)
{
    यदि (!क्रमय_Descending)
        प्रत्यागम गभीरता == CV_8U ? ipp_क्रमय_सूचकांक_Wrap<Ipp8u, ipps_क्रमय_Radix_सूचकांक_Ascend_8u> :
            गभीरता == CV_16U ? ipp_क्रमय_सूचकांक_Wrap<Ipp16u, ipps_क्रमय_Radix_सूचकांक_Ascend_16u> :
            गभीरता == CV_16S ? ipp_क्रमय_सूचकांक_Wrap<Ipp16s, ipps_क्रमय_Radix_सूचकांक_Ascend_16s> :
            गभीरता == CV_32S ? ipp_क्रमय_सूचकांक_Wrap<Ipp32s, ipps_क्रमय_Radix_सूचकांक_Ascend_32s> :
            गभीरता == CV_32F ? ipp_क्रमय_सूचकांक_Wrap<Ipp32f, ipps_क्रमय_Radix_सूचकांक_Ascend_32f> :
            0;
    अन्यथा
        प्रत्यागम गभीरता == CV_8U ? ipp_क्रमय_सूचकांक_Wrap<Ipp8u, ipps_क्रमय_Radix_सूचकांक_Descend_8u> :
            गभीरता == CV_16U ? ipp_क्रमय_सूचकांक_Wrap<Ipp16u, ipps_क्रमय_Radix_सूचकांक_Descend_16u> :
            गभीरता == CV_16S ? ipp_क्रमय_सूचकांक_Wrap<Ipp16s, ipps_क्रमय_Radix_सूचकांक_Descend_16s> :
            गभीरता == CV_32S ? ipp_क्रमय_सूचकांक_Wrap<Ipp32s, ipps_क्रमय_Radix_सूचकांक_Descend_32s> :
            गभीरता == CV_32F ? ipp_क्रमय_सूचकांक_Wrap<Ipp32f, ipps_क्रमय_Radix_सूचकांक_Descend_32f> :
            0;
}

स्थिर तार्किक ipp_क्रमय_Idx( ध्रुव Mat& src, Mat& dst, पूर्णाङ्क flags )
{
    CV_INSTRUMENT_REGION_IPP();

    तार्किक        क्रमय_पङ्क्तिकाः        = (flags & 1) == क्रमय_EVERY_ROW;
    तार्किक        क्रमय_Descending  = (flags & क्रमय_DESCENDING) != 0;
    पूर्णाङ्क         गभीरता           = src.गभीरता();
    Ipp_दत्त_Type type            = ippi_Get_दत्त_Type(गभीरता);

    Ipp_क्रमय_सूचकांक_Func ipps_क्रमय_Radix_सूचकांक = get_क्रमय_सूचकांक_Func(गभीरता, क्रमय_Descending);
    यदि(!ipps_क्रमय_Radix_सूचकांक)
        प्रत्यागम असत्य;

    यदि(क्रमय_पङ्क्तिकाः)
    {
        Auto_बफर<Ipp8u> बफर;
        पूर्णाङ्क               बफर_आकार;
        यदि(ipps_क्रमय_Radix_सूचकांक_Get_बफर_आकार(src.स्तम्भसङ्ख्या, type, &बफर_आकार) < 0)
            प्रत्यागम असत्य;

        बफर.allocate(बफर_आकार);

        प्रति(पूर्णाङ्क i = 0; i < src.पङ्क्तिकाः; i++)
        {
            यदि(CV_INSTRUMENT_FUN_IPP(ipps_क्रमय_Radix_सूचकांक, (ध्रुव शून्य*)src.ptr(i), (Ipp32s)src.चरणः[1], (Ipp32s*)dst.ptr(i), src.स्तम्भसङ्ख्या, बफर.दत्त()) < 0)
                प्रत्यागम असत्य;
        }
    }
    अन्यथा
    {
        Mat  dstRow(1, dst.पङ्क्तिकाः, dst.type());
        Mat  dstSub;
        Rect subRect(0,0,1,src.पङ्क्तिकाः);

        Auto_बफर<Ipp8u> बफर;
        पूर्णाङ्क               बफर_आकार;
        यदि(ipps_क्रमय_Radix_सूचकांक_Get_बफर_आकार(src.पङ्क्तिकाः, type, &बफर_आकार) < 0)
            प्रत्यागम असत्य;

        बफर.allocate(बफर_आकार);

        Ipp32s src_चरणः_3 = (Ipp32s)src.चरणः[0];
        प्रति(पूर्णाङ्क i = 0; i < src.स्तम्भसङ्ख्या; i++)
        {
            subRect.x = i;
            dstSub = Mat(dst, subRect);

            यदि(CV_INSTRUMENT_FUN_IPP(ipps_क्रमय_Radix_सूचकांक, (ध्रुव शून्य*)src.ptr(0, i), src_चरणः_3, (Ipp32s*)dstRow.ptr(), src.पङ्क्तिकाः, बफर.दत्त()) < 0)
                प्रत्यागम असत्य;

            dstRow = dstRow.reshape(1, dstSub.पङ्क्तिकाः);
            dstRow.copy_गन्तव्यम्(dstSub);
        }
    }

    प्रत्यागम सत्य;
}
#यदि_अन्तः

प्रकारनिर्देश शून्य (*क्रमय_Func)(ध्रुव Mat& src, Mat& dst, पूर्णाङ्क flags);
}

शून्य cv::sort( निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    _dst.create_Same_आकार( src, src.type() );
    Mat dst = _dst.getMat();
#यदि !IPP_DISABLE_क्रमय
    CV_IPP_सततम्_FAST(ipp_क्रमय(src, dst, flags));
#यदि_अन्तः

    स्थिर क्रमय_Func tab[CV_गभीरता_MAX] =
    {
        क्रमय<uchar>, क्रमय<schar>, क्रमय<ushort>, क्रमय<ह्रस्व>,
        क्रमय<पूर्णाङ्क>, क्रमय<भिन्न>, क्रमय<द्विगुण>, 0
    };
    क्रमय_Func func = tab[src.गभीरता()];
    CV_Assert( func != 0 );

    func( src, dst, flags );
}

शून्य cv::क्रमय_Idx( निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क flags )
{
    CV_INSTRUMENT_REGION();

    Mat src = _src.getMat();
    CV_Assert( src.dims <= 2 && src.channels() == 1 );
    Mat dst = _dst.getMat();
    यदि( dst.दत्त == src.दत्त )
        _dst.release();
    _dst.create_Same_आकार( src, CV_32S );
    dst = _dst.getMat();

#यदि !IPP_DISABLE_क्रमय
    CV_IPP_सततम्_FAST(ipp_क्रमय_Idx(src, dst, flags));
#यदि_अन्तः

    स्थिर क्रमय_Func tab[CV_गभीरता_MAX] =
    {
        क्रमय_Idx_2<uchar>, क्रमय_Idx_2<schar>, क्रमय_Idx_2<ushort>, क्रमय_Idx_2<ह्रस्व>,
        क्रमय_Idx_2<पूर्णाङ्क>, क्रमय_Idx_2<भिन्न>, क्रमय_Idx_2<द्विगुण>, 0
    };
    क्रमय_Func func = tab[src.गभीरता()];
    CV_Assert( func != 0 );
    func( src, dst, flags );
}
