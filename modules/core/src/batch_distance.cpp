// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "stat.hpp"
#అన్తర్భావ <opencv2/core/hal/hal.hpp>

నామాకాశ cv
{

మూస<రకం_పేరు _Tp, రకం_పేరు _Rt>
స్థిర శూన్య batchDistL1_(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, size_t step2,
                         పూర్ణాంకం nvecs, పూర్ణాంకం len, శూన్య* _dist, స్థిరరాశి uchar* ముసుగు)
{
    స్థిరరాశి _Tp* src1 = (స్థిరరాశి _Tp*)_src1;
    స్థిరరాశి _Tp* src2 = (స్థిరరాశి _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= పరిమాణం(src2[0]);
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    లేకపోతే
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = ముసుగు[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

మూస<రకం_పేరు _Tp, రకం_పేరు _Rt>
స్థిర శూన్య batchDistL2Sqr_(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, size_t step2,
                            పూర్ణాంకం nvecs, పూర్ణాంకం len, శూన్య* _dist, స్థిరరాశి uchar* ముసుగు)
{
    స్థిరరాశి _Tp* src1 = (స్థిరరాశి _Tp*)_src1;
    స్థిరరాశి _Tp* src2 = (స్థిరరాశి _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= పరిమాణం(src2[0]);
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    లేకపోతే
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = ముసుగు[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

మూస<>
శూన్య batchDistL2Sqr_<దశాంశం, దశాంశం>(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, size_t step2,
                                   పూర్ణాంకం nvecs, పూర్ణాంకం len, శూన్య* _dist, స్థిరరాశి uchar* ముసుగు)
{
    స్థిరరాశి దశాంశం* src1 = (స్థిరరాశి దశాంశం*)_src1;
    స్థిరరాశి దశాంశం* src2 = (స్థిరరాశి దశాంశం*)_src2;
    దశాంశం* dist = (దశాంశం*)_dist;
    step2 /= పరిమాణం(src2[0]);
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    లేకపోతే
    {
        దశాంశం val0 = std::numeric_limits<దశాంశం>::max();
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = ముసుగు[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

మూస<రకం_పేరు _Tp, రకం_పేరు _Rt>
స్థిర శూన్య batchDistL2_(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, size_t step2,
                         పూర్ణాంకం nvecs, పూర్ణాంకం len, శూన్య* _dist, స్థిరరాశి uchar* ముసుగు)
{
    స్థిరరాశి _Tp* src1 = (స్థిరరాశి _Tp*)_src1;
    స్థిరరాశి _Tp* src2 = (స్థిరరాశి _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= పరిమాణం(src2[0]);
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = std::వర్గమూలం(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    లేకపోతే
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = ముసుగు[i] ? std::వర్గమూలం(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

మూస<>
శూన్య batchDistL2_<దశాంశం, దశాంశం>(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, size_t step2,
                                పూర్ణాంకం nvecs, పూర్ణాంకం len, శూన్య* _dist, స్థిరరాశి uchar* ముసుగు)
{
    స్థిరరాశి దశాంశం* src1 = (స్థిరరాశి దశాంశం*)_src1;
    స్థిరరాశి దశాంశం* src2 = (స్థిరరాశి దశాంశం*)_src2;
    దశాంశం* dist = (దశాంశం*)_dist;
    step2 /= పరిమాణం(src2[0]);
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = std::వర్గమూలం(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    లేకపోతే
    {
        దశాంశం val0 = std::numeric_limits<దశాంశం>::max();
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = ముసుగు[i] ? std::వర్గమూలం(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

స్థిర శూన్య batchDistHamming(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, size_t step2,
                             పూర్ణాంకం nvecs, పూర్ణాంకం len, శూన్య* _dist, స్థిరరాశి uchar* ముసుగు)
{
    స్థిరరాశి uchar* src1 = (స్థిరరాశి uchar*)_src1;
    స్థిరరాశి uchar* src2 = (స్థిరరాశి uchar*)_src2;
    పూర్ణాంకం* dist = (పూర్ణాంకం*)_dist;
    step2 /= పరిమాణం(src2[0]);
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    లేకపోతే
    {
        పూర్ణాంకం val0 = గరిష్ఠ_పూర్ణాంకం;
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
        {
            ఒకవేళ (ముసుగు[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            లేకపోతే
                dist[i] = val0;
        }
    }
}

స్థిర శూన్య batchDistHamming2(స్థిరరాశి శూన్య* _src1, స్థిరరాశి శూన్య* _src2, size_t step2,
                              పూర్ణాంకం nvecs, పూర్ణాంకం len, శూన్య* _dist, స్థిరరాశి uchar* ముసుగు)
{
    స్థిరరాశి uchar* src1 = (స్థిరరాశి uchar*)_src1;
    స్థిరరాశి uchar* src2 = (స్థిరరాశి uchar*)_src2;
    పూర్ణాంకం* dist = (పూర్ణాంకం*)_dist;
    step2 /= పరిమాణం(src2[0]);
    ఒకవేళ( !ముసుగు )
    {
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    లేకపోతే
    {
        పూర్ణాంకం val0 = గరిష్ఠ_పూర్ణాంకం;
        కొరకు( పూర్ణాంకం i = 0; i < nvecs; i++ )
        {
            ఒకవేళ (ముసుగు[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            లేకపోతే
                dist[i] = val0;
        }
    }
}

రకనిర్వచనం శూన్య (*BatchDistFunc)(స్థిరరాశి శూన్య* src1, స్థిరరాశి శూన్య* src2, size_t step2,
                              పూర్ణాంకం nvecs, పూర్ణాంకం len, శూన్య* dist, స్థిరరాశి uchar* ముసుగు);


నిర్మాణం BatchDistInvoker : బహిరంగ ParallelLoopBody
{
    BatchDistInvoker( స్థిరరాశి Mat& _src1, స్థిరరాశి Mat& _src2,
                      Mat& _dist, Mat& _nidx, పూర్ణాంకం _K,
                      స్థిరరాశి Mat& ముసుగు_2, పూర్ణాంకం _update,
                      BatchDistFunc _func)
    {
        src1 = &_src1;
        src2 = &_src2;
        dist = &_dist;
        nidx = &_nidx;
        K = _K;
        ముసుగు = &ముసుగు_2;
        update = _update;
        func = _func;
    }

    శూన్య క్రియాకారకం()(స్థిరరాశి Range& range) స్థిరరాశి CV_OVERRIDE
    {
        Auto_బఫరు<పూర్ణాంకం> buf(src2->అడ్డువరుసలు);
        పూర్ణాంకం* bufptr = buf.దత్తాంశం();

        కొరకు( పూర్ణాంకం i = range.ప్రారంభం; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->అడుగు, src2->అడ్డువరుసలు, src2->నిలువుల_సంఖ్య,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), ముసుగు->దత్తాంశం ? ముసుగు->ptr(i) : 0);

            ఒకవేళ( K > 0 )
            {
                పూర్ణాంకం* nidxptr = nidx->ptr<పూర్ణాంకం>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                పూర్ణాంకం* distptr = (పూర్ణాంకం*)dist->ptr(i);

                పూర్ణాంకం j, k;

                కొరకు( j = 0; j < src2->అడ్డువరుసలు; j++ )
                {
                    పూర్ణాంకం d = bufptr[j];
                    ఒకవేళ( d < distptr[K-1] )
                    {
                        కొరకు( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    స్థిరరాశి Mat *src1;
    స్థిరరాశి Mat *src2;
    Mat *dist;
    Mat *nidx;
    స్థిరరాశి Mat *ముసుగు;
    పూర్ణాంకం K;
    పూర్ణాంకం update;
    BatchDistFunc func;
};

}

శూన్య cv::batch_దూరం( ఆదానం_Array _src1, ఆదానం_Array _src2,
                        OutputArray _dist, పూర్ణాంకం dtype, OutputArray _nidx,
                        పూర్ణాంకం normType, పూర్ణాంకం K, ఆదానం_Array ముసుగు_2,
                        పూర్ణాంకం update, తార్కిక crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), ముసుగు = ముసుగు_2.getMat();
    పూర్ణాంకం type = src1.type();
    CV_Assert( type == src2.type() && src1.నిలువుల_సంఖ్య == src2.నిలువుల_సంఖ్య &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    ఒకవేళ( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.అడ్డువరుసలు);

    _dist.create(src1.అడ్డువరుసలు, (K > 0 ? K : src2.అడ్డువరుసలు), dtype);
    Mat dist = _dist.getMat(), nidx;
    ఒకవేళ( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    ఒకవేళ( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (ద్విగుణ)గరిష్ఠ_పూర్ణాంకం : (ద్విగుణ)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    ఒకవేళ( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && ముసుగు.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batch_దూరం(src2, src1, tdist, dtype, tidx, normType, K, ముసుగు, 0, అబద్ధం);
        batch_దూరం(src1, src2, sdist, dtype, sidx, normType, K, ముసుగు, 0, అబద్ధం);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        ఒకవేళ( dtype == CV_32S )
        {
            కొరకు( పూర్ణాంకం i = 0; i < tdist.అడ్డువరుసలు; i++ )
            {
                పూర్ణాంకం idx = tidx.at<పూర్ణాంకం>(i);
                పూర్ణాంకం d = tdist.at<పూర్ణాంకం>(i), d0 = dist.at<పూర్ణాంకం>(idx);
                ఒకవేళ( d < d0 )
                {
                    dist.at<పూర్ణాంకం>(idx) = d;
                    nidx.at<పూర్ణాంకం>(idx) = i + update;
                }
            }
        }
        లేకపోతే
        {
            కొరకు( పూర్ణాంకం i = 0; i < tdist.అడ్డువరుసలు; i++ )
            {
                పూర్ణాంకం idx = tidx.at<పూర్ణాంకం>(i);
                దశాంశం d = tdist.at<దశాంశం>(i), d0 = dist.at<దశాంశం>(idx);
                ఒకవేళ( d < d0 )
                {
                    dist.at<దశాంశం>(idx) = d;
                    nidx.at<పూర్ణాంకం>(idx) = i + update;
                }
            }
        }
        కొరకు( పూర్ణాంకం i = 0; i < sdist.అడ్డువరుసలు; i++ )
        {
            ఒకవేళ( tidx.at<పూర్ణాంకం>(sidx.at<పూర్ణాంకం>(i)) != i )
            {
                nidx.at<పూర్ణాంకం>(i) = -1;
            }
        }
        తిరిగి;
    }

    BatchDistFunc func = 0;
    ఒకవేళ( type == CV_8U )
    {
        ఒకవేళ( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, పూర్ణాంకం>;
        లేకపోతే ఒకవేళ( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, దశాంశం>;
        లేకపోతే ఒకవేళ( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, పూర్ణాంకం>;
        లేకపోతే ఒకవేళ( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, దశాంశం>;
        లేకపోతే ఒకవేళ( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, దశాంశం>;
        లేకపోతే ఒకవేళ( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        లేకపోతే ఒకవేళ( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    లేకపోతే ఒకవేళ( type == CV_32F && dtype == CV_32F )
    {
        ఒకవేళ( normType == NORM_L1 )
            func = batchDistL1_<దశాంశం, దశాంశం>;
        లేకపోతే ఒకవేళ( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<దశాంశం, దశాంశం>;
        లేకపోతే ఒకవేళ( normType == NORM_L2 )
            func = batchDistL2_<దశాంశం, దశాంశం>;
    }

    ఒకవేళ( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.అడ్డువరుసలు),
                  BatchDistInvoker(src1, src2, dist, nidx, K, ముసుగు, update, func));
}
