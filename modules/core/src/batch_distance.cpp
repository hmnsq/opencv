// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "stat.hpp"
#અન્તર્ભાવ <opencv2/core/hal/hal.hpp>

નામાકાશ cv
{

template<typename _Tp, typename _Rt>
સ્થિર શૂન્ય batchDistL1_(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, size_t step2,
                         પૂર્ણાંક nvecs, પૂર્ણાંક len, શૂન્ય* _dist, અચલ uchar* mask)
{
    અચલ _Tp* src1 = (અચલ _Tp*)_src1;
    અચલ _Tp* src2 = (અચલ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= કદ(src2[0]);
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    નહીંતર
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
સ્થિર શૂન્ય batchDistL2Sqr_(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, size_t step2,
                            પૂર્ણાંક nvecs, પૂર્ણાંક len, શૂન્ય* _dist, અચલ uchar* mask)
{
    અચલ _Tp* src1 = (અચલ _Tp*)_src1;
    અચલ _Tp* src2 = (અચલ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= કદ(src2[0]);
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    નહીંતર
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
શૂન્ય batchDistL2Sqr_<દશાંશ, દશાંશ>(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, size_t step2,
                                   પૂર્ણાંક nvecs, પૂર્ણાંક len, શૂન્ય* _dist, અચલ uchar* mask)
{
    અચલ દશાંશ* src1 = (અચલ દશાંશ*)_src1;
    અચલ દશાંશ* src2 = (અચલ દશાંશ*)_src2;
    દશાંશ* dist = (દશાંશ*)_dist;
    step2 /= કદ(src2[0]);
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    નહીંતર
    {
        દશાંશ val0 = std::numeric_limits<દશાંશ>::max();
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
સ્થિર શૂન્ય batchDistL2_(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, size_t step2,
                         પૂર્ણાંક nvecs, પૂર્ણાંક len, શૂન્ય* _dist, અચલ uchar* mask)
{
    અચલ _Tp* src1 = (અચલ _Tp*)_src1;
    અચલ _Tp* src2 = (અચલ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= કદ(src2[0]);
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    નહીંતર
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
શૂન્ય batchDistL2_<દશાંશ, દશાંશ>(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, size_t step2,
                                પૂર્ણાંક nvecs, પૂર્ણાંક len, શૂન્ય* _dist, અચલ uchar* mask)
{
    અચલ દશાંશ* src1 = (અચલ દશાંશ*)_src1;
    અચલ દશાંશ* src2 = (અચલ દશાંશ*)_src2;
    દશાંશ* dist = (દશાંશ*)_dist;
    step2 /= કદ(src2[0]);
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    નહીંતર
    {
        દશાંશ val0 = std::numeric_limits<દશાંશ>::max();
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

સ્થિર શૂન્ય batchDistHamming(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, size_t step2,
                             પૂર્ણાંક nvecs, પૂર્ણાંક len, શૂન્ય* _dist, અચલ uchar* mask)
{
    અચલ uchar* src1 = (અચલ uchar*)_src1;
    અચલ uchar* src2 = (અચલ uchar*)_src2;
    પૂર્ણાંક* dist = (પૂર્ણાંક*)_dist;
    step2 /= કદ(src2[0]);
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    નહીંતર
    {
        પૂર્ણાંક val0 = INT_MAX;
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
        {
            જો (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            નહીંતર
                dist[i] = val0;
        }
    }
}

સ્થિર શૂન્ય batchDistHamming2(અચલ શૂન્ય* _src1, અચલ શૂન્ય* _src2, size_t step2,
                              પૂર્ણાંક nvecs, પૂર્ણાંક len, શૂન્ય* _dist, અચલ uchar* mask)
{
    અચલ uchar* src1 = (અચલ uchar*)_src1;
    અચલ uchar* src2 = (અચલ uchar*)_src2;
    પૂર્ણાંક* dist = (પૂર્ણાંક*)_dist;
    step2 /= કદ(src2[0]);
    જો( !mask )
    {
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    નહીંતર
    {
        પૂર્ણાંક val0 = INT_MAX;
        માટે( પૂર્ણાંક i = 0; i < nvecs; i++ )
        {
            જો (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            નહીંતર
                dist[i] = val0;
        }
    }
}

પ્રકારવ્યાખ્યા શૂન્ય (*BatchDistFunc)(અચલ શૂન્ય* src1, અચલ શૂન્ય* src2, size_t step2,
                              પૂર્ણાંક nvecs, પૂર્ણાંક len, શૂન્ય* dist, અચલ uchar* mask);


સંરચના BatchDistInvoker : જાહેર ParallelLoopBody
{
    BatchDistInvoker( અચલ Mat& _src1, અચલ Mat& _src2,
                      Mat& _dist, Mat& _nidx, પૂર્ણાંક _K,
                      અચલ Mat& _mask, પૂર્ણાંક _update,
                      BatchDistFunc _func)
    {
        src1 = &_src1;
        src2 = &_src2;
        dist = &_dist;
        nidx = &_nidx;
        K = _K;
        mask = &_mask;
        update = _update;
        func = _func;
    }

    શૂન્ય operator()(અચલ Range& range) અચલ CV_OVERRIDE
    {
        AutoBuffer<પૂર્ણાંક> buf(src2->rows);
        પૂર્ણાંક* bufptr = buf.data();

        માટે( પૂર્ણાંક i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            જો( K > 0 )
            {
                પૂર્ણાંક* nidxptr = nidx->ptr<પૂર્ણાંક>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                પૂર્ણાંક* distptr = (પૂર્ણાંક*)dist->ptr(i);

                પૂર્ણાંક j, k;

                માટે( j = 0; j < src2->rows; j++ )
                {
                    પૂર્ણાંક d = bufptr[j];
                    જો( d < distptr[K-1] )
                    {
                        માટે( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    અચલ Mat *src1;
    અચલ Mat *src2;
    Mat *dist;
    Mat *nidx;
    અચલ Mat *mask;
    પૂર્ણાંક K;
    પૂર્ણાંક update;
    BatchDistFunc func;
};

}

શૂન્ય cv::batchDistance( આગત_Array _src1, આગત_Array _src2,
                        OutputArray _dist, પૂર્ણાંક dtype, OutputArray _nidx,
                        પૂર્ણાંક normType, પૂર્ણાંક K, આગત_Array _mask,
                        પૂર્ણાંક update, તાર્કિક crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    પૂર્ણાંક type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    જો( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    જો( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    જો( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (દ્વિગુણ)INT_MAX : (દ્વિગુણ)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    જો( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && mask.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batchDistance(src2, src1, tdist, dtype, tidx, normType, K, mask, 0, ખોટું);
        batchDistance(src1, src2, sdist, dtype, sidx, normType, K, mask, 0, ખોટું);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        જો( dtype == CV_32S )
        {
            માટે( પૂર્ણાંક i = 0; i < tdist.rows; i++ )
            {
                પૂર્ણાંક idx = tidx.at<પૂર્ણાંક>(i);
                પૂર્ણાંક d = tdist.at<પૂર્ણાંક>(i), d0 = dist.at<પૂર્ણાંક>(idx);
                જો( d < d0 )
                {
                    dist.at<પૂર્ણાંક>(idx) = d;
                    nidx.at<પૂર્ણાંક>(idx) = i + update;
                }
            }
        }
        નહીંતર
        {
            માટે( પૂર્ણાંક i = 0; i < tdist.rows; i++ )
            {
                પૂર્ણાંક idx = tidx.at<પૂર્ણાંક>(i);
                દશાંશ d = tdist.at<દશાંશ>(i), d0 = dist.at<દશાંશ>(idx);
                જો( d < d0 )
                {
                    dist.at<દશાંશ>(idx) = d;
                    nidx.at<પૂર્ણાંક>(idx) = i + update;
                }
            }
        }
        માટે( પૂર્ણાંક i = 0; i < sdist.rows; i++ )
        {
            જો( tidx.at<પૂર્ણાંક>(sidx.at<પૂર્ણાંક>(i)) != i )
            {
                nidx.at<પૂર્ણાંક>(i) = -1;
            }
        }
        પાછા;
    }

    BatchDistFunc func = 0;
    જો( type == CV_8U )
    {
        જો( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, પૂર્ણાંક>;
        નહીંતર જો( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, દશાંશ>;
        નહીંતર જો( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, પૂર્ણાંક>;
        નહીંતર જો( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, દશાંશ>;
        નહીંતર જો( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, દશાંશ>;
        નહીંતર જો( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        નહીંતર જો( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    નહીંતર જો( type == CV_32F && dtype == CV_32F )
    {
        જો( normType == NORM_L1 )
            func = batchDistL1_<દશાંશ, દશાંશ>;
        નહીંતર જો( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<દશાંશ, દશાંશ>;
        નહીંતર જો( normType == NORM_L2 )
            func = batchDistL2_<દશાંશ, દશાંશ>;
    }

    જો( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
