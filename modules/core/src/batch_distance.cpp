// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/hal/hal.hpp>

ਨਾਮਾਕਾਸ਼ cv
{

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp, ਕਿਸਮ_ਨਾਂ _Rt>
ਸਥਿਰ ਖਾਲੀ batchDistL1_(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, size_t step2,
                         ਪੂਰਨ nvecs, ਪੂਰਨ len, ਖਾਲੀ* _dist, ਅਚਲ uchar* ਨਕਾਬ)
{
    ਅਚਲ _Tp* src1 = (ਅਚਲ _Tp*)_src1;
    ਅਚਲ _Tp* src2 = (ਅਚਲ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ਆਕਾਰ(src2[0]);
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ਨਹੀਂਤਾਂ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = ਨਕਾਬ[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp, ਕਿਸਮ_ਨਾਂ _Rt>
ਸਥਿਰ ਖਾਲੀ batchDistL2Sqr_(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, size_t step2,
                            ਪੂਰਨ nvecs, ਪੂਰਨ len, ਖਾਲੀ* _dist, ਅਚਲ uchar* ਨਕਾਬ)
{
    ਅਚਲ _Tp* src1 = (ਅਚਲ _Tp*)_src1;
    ਅਚਲ _Tp* src2 = (ਅਚਲ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ਆਕਾਰ(src2[0]);
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ਨਹੀਂਤਾਂ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = ਨਕਾਬ[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

ਸਾਂਚਾ<>
ਖਾਲੀ batchDistL2Sqr_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, size_t step2,
                                   ਪੂਰਨ nvecs, ਪੂਰਨ len, ਖਾਲੀ* _dist, ਅਚਲ uchar* ਨਕਾਬ)
{
    ਅਚਲ ਦਸ਼ਮਲਵ* src1 = (ਅਚਲ ਦਸ਼ਮਲਵ*)_src1;
    ਅਚਲ ਦਸ਼ਮਲਵ* src2 = (ਅਚਲ ਦਸ਼ਮਲਵ*)_src2;
    ਦਸ਼ਮਲਵ* dist = (ਦਸ਼ਮਲਵ*)_dist;
    step2 /= ਆਕਾਰ(src2[0]);
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    ਨਹੀਂਤਾਂ
    {
        ਦਸ਼ਮਲਵ val0 = std::numeric_limits<ਦਸ਼ਮਲਵ>::max();
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = ਨਕਾਬ[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _Tp, ਕਿਸਮ_ਨਾਂ _Rt>
ਸਥਿਰ ਖਾਲੀ batchDistL2_(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, size_t step2,
                         ਪੂਰਨ nvecs, ਪੂਰਨ len, ਖਾਲੀ* _dist, ਅਚਲ uchar* ਨਕਾਬ)
{
    ਅਚਲ _Tp* src1 = (ਅਚਲ _Tp*)_src1;
    ਅਚਲ _Tp* src2 = (ਅਚਲ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ਆਕਾਰ(src2[0]);
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = std::ਵਰਗਮੂਲ(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    ਨਹੀਂਤਾਂ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = ਨਕਾਬ[i] ? std::ਵਰਗਮੂਲ(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

ਸਾਂਚਾ<>
ਖਾਲੀ batchDistL2_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, size_t step2,
                                ਪੂਰਨ nvecs, ਪੂਰਨ len, ਖਾਲੀ* _dist, ਅਚਲ uchar* ਨਕਾਬ)
{
    ਅਚਲ ਦਸ਼ਮਲਵ* src1 = (ਅਚਲ ਦਸ਼ਮਲਵ*)_src1;
    ਅਚਲ ਦਸ਼ਮਲਵ* src2 = (ਅਚਲ ਦਸ਼ਮਲਵ*)_src2;
    ਦਸ਼ਮਲਵ* dist = (ਦਸ਼ਮਲਵ*)_dist;
    step2 /= ਆਕਾਰ(src2[0]);
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = std::ਵਰਗਮੂਲ(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    ਨਹੀਂਤਾਂ
    {
        ਦਸ਼ਮਲਵ val0 = std::numeric_limits<ਦਸ਼ਮਲਵ>::max();
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = ਨਕਾਬ[i] ? std::ਵਰਗਮੂਲ(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

ਸਥਿਰ ਖਾਲੀ batchDistHamming(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, size_t step2,
                             ਪੂਰਨ nvecs, ਪੂਰਨ len, ਖਾਲੀ* _dist, ਅਚਲ uchar* ਨਕਾਬ)
{
    ਅਚਲ uchar* src1 = (ਅਚਲ uchar*)_src1;
    ਅਚਲ uchar* src2 = (ਅਚਲ uchar*)_src2;
    ਪੂਰਨ* dist = (ਪੂਰਨ*)_dist;
    step2 /= ਆਕਾਰ(src2[0]);
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ val0 = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
        {
            ਜੇ (ਨਕਾਬ[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            ਨਹੀਂਤਾਂ
                dist[i] = val0;
        }
    }
}

ਸਥਿਰ ਖਾਲੀ batchDistHamming2(ਅਚਲ ਖਾਲੀ* _src1, ਅਚਲ ਖਾਲੀ* _src2, size_t step2,
                              ਪੂਰਨ nvecs, ਪੂਰਨ len, ਖਾਲੀ* _dist, ਅਚਲ uchar* ਨਕਾਬ)
{
    ਅਚਲ uchar* src1 = (ਅਚਲ uchar*)_src1;
    ਅਚਲ uchar* src2 = (ਅਚਲ uchar*)_src2;
    ਪੂਰਨ* dist = (ਪੂਰਨ*)_dist;
    step2 /= ਆਕਾਰ(src2[0]);
    ਜੇ( !ਨਕਾਬ )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ val0 = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;
        ਲਈ( ਪੂਰਨ i = 0; i < nvecs; i++ )
        {
            ਜੇ (ਨਕਾਬ[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            ਨਹੀਂਤਾਂ
                dist[i] = val0;
        }
    }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*BatchDistFunc)(ਅਚਲ ਖਾਲੀ* src1, ਅਚਲ ਖਾਲੀ* src2, size_t step2,
                              ਪੂਰਨ nvecs, ਪੂਰਨ len, ਖਾਲੀ* dist, ਅਚਲ uchar* ਨਕਾਬ);


ਬਣਤਰ BatchDistInvoker : ਜਨਤਕ ParallelLoopBody
{
    BatchDistInvoker( ਅਚਲ Mat& _src1, ਅਚਲ Mat& _src2,
                      Mat& _dist, Mat& _nidx, ਪੂਰਨ _K,
                      ਅਚਲ Mat& ਨਕਾਬ_2, ਪੂਰਨ _update,
                      BatchDistFunc _func)
    {
        src1 = &_src1;
        src2 = &_src2;
        dist = &_dist;
        nidx = &_nidx;
        K = _K;
        ਨਕਾਬ = &ਨਕਾਬ_2;
        update = _update;
        func = _func;
    }

    ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ Range& range) ਅਚਲ CV_OVERRIDE
    {
        Auto_ਬਫਰ<ਪੂਰਨ> buf(src2->ਪੰਕਤੀਆਂ);
        ਪੂਰਨ* bufptr = buf.ਅੰਕੜੇ();

        ਲਈ( ਪੂਰਨ i = range.ਸ਼ੁਰੂ; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->ਕਦਮ, src2->ਪੰਕਤੀਆਂ, src2->ਖਾਨੇ_ਗਿਣਤੀ,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), ਨਕਾਬ->ਅੰਕੜੇ ? ਨਕਾਬ->ptr(i) : 0);

            ਜੇ( K > 0 )
            {
                ਪੂਰਨ* nidxptr = nidx->ptr<ਪੂਰਨ>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                ਪੂਰਨ* distptr = (ਪੂਰਨ*)dist->ptr(i);

                ਪੂਰਨ j, k;

                ਲਈ( j = 0; j < src2->ਪੰਕਤੀਆਂ; j++ )
                {
                    ਪੂਰਨ d = bufptr[j];
                    ਜੇ( d < distptr[K-1] )
                    {
                        ਲਈ( k = K-2; k >= 0 && distptr[k] > d; k-- )
                        {
                            nidxptr[k+1] = nidxptr[k];
                            distptr[k+1] = distptr[k];
                        }
                        nidxptr[k+1] = j + update;
                        distptr[k+1] = d;
                    }
                }
            }
        }
    }

    ਅਚਲ Mat *src1;
    ਅਚਲ Mat *src2;
    Mat *dist;
    Mat *nidx;
    ਅਚਲ Mat *ਨਕਾਬ;
    ਪੂਰਨ K;
    ਪੂਰਨ update;
    BatchDistFunc func;
};

}

ਖਾਲੀ cv::batch_ਦੂਰੀ( ਨਿਵੇਸ਼_Array _src1, ਨਿਵੇਸ਼_Array _src2,
                        OutputArray _dist, ਪੂਰਨ dtype, OutputArray _nidx,
                        ਪੂਰਨ normType, ਪੂਰਨ K, ਨਿਵੇਸ਼_Array ਨਕਾਬ_2,
                        ਪੂਰਨ update, ਤਾਰਕਿਕ crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), ਨਕਾਬ = ਨਕਾਬ_2.getMat();
    ਪੂਰਨ type = src1.type();
    CV_Assert( type == src2.type() && src1.ਖਾਨੇ_ਗਿਣਤੀ == src2.ਖਾਨੇ_ਗਿਣਤੀ &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    ਜੇ( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.ਪੰਕਤੀਆਂ);

    _dist.create(src1.ਪੰਕਤੀਆਂ, (K > 0 ? K : src2.ਪੰਕਤੀਆਂ), dtype);
    Mat dist = _dist.getMat(), nidx;
    ਜੇ( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    ਜੇ( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (ਦੁੱਗਣਾ)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ : (ਦੁੱਗਣਾ)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    ਜੇ( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && ਨਕਾਬ.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batch_ਦੂਰੀ(src2, src1, tdist, dtype, tidx, normType, K, ਨਕਾਬ, 0, ਝੂਠ);
        batch_ਦੂਰੀ(src1, src2, sdist, dtype, sidx, normType, K, ਨਕਾਬ, 0, ਝੂਠ);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        ਜੇ( dtype == CV_32S )
        {
            ਲਈ( ਪੂਰਨ i = 0; i < tdist.ਪੰਕਤੀਆਂ; i++ )
            {
                ਪੂਰਨ idx = tidx.at<ਪੂਰਨ>(i);
                ਪੂਰਨ d = tdist.at<ਪੂਰਨ>(i), d0 = dist.at<ਪੂਰਨ>(idx);
                ਜੇ( d < d0 )
                {
                    dist.at<ਪੂਰਨ>(idx) = d;
                    nidx.at<ਪੂਰਨ>(idx) = i + update;
                }
            }
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਈ( ਪੂਰਨ i = 0; i < tdist.ਪੰਕਤੀਆਂ; i++ )
            {
                ਪੂਰਨ idx = tidx.at<ਪੂਰਨ>(i);
                ਦਸ਼ਮਲਵ d = tdist.at<ਦਸ਼ਮਲਵ>(i), d0 = dist.at<ਦਸ਼ਮਲਵ>(idx);
                ਜੇ( d < d0 )
                {
                    dist.at<ਦਸ਼ਮਲਵ>(idx) = d;
                    nidx.at<ਪੂਰਨ>(idx) = i + update;
                }
            }
        }
        ਲਈ( ਪੂਰਨ i = 0; i < sdist.ਪੰਕਤੀਆਂ; i++ )
        {
            ਜੇ( tidx.at<ਪੂਰਨ>(sidx.at<ਪੂਰਨ>(i)) != i )
            {
                nidx.at<ਪੂਰਨ>(i) = -1;
            }
        }
        ਵਾਪਸ;
    }

    BatchDistFunc func = 0;
    ਜੇ( type == CV_8U )
    {
        ਜੇ( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, ਪੂਰਨ>;
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, ਦਸ਼ਮਲਵ>;
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, ਪੂਰਨ>;
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, ਦਸ਼ਮਲਵ>;
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, ਦਸ਼ਮਲਵ>;
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    ਨਹੀਂਤਾਂ ਜੇ( type == CV_32F && dtype == CV_32F )
    {
        ਜੇ( normType == NORM_L1 )
            func = batchDistL1_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>;
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>;
        ਨਹੀਂਤਾਂ ਜੇ( normType == NORM_L2 )
            func = batchDistL2_<ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ>;
    }

    ਜੇ( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.ਪੰਕਤੀਆਂ),
                  BatchDistInvoker(src1, src2, dist, nidx, K, ਨਕਾਬ, update, func));
}
