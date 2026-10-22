// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "stat.hpp"
#அந்தர்பாவ <opencv2/core/hal/hal.hpp>

நாமாகாஶ cv
{

template<typename _Tp, typename _Rt>
நிலையான வெறுமை batchDistL1_(மாறிலி வெறுமை* _src1, மாறிலி வெறுமை* _src2, size_t step2,
                         முழு nvecs, முழு len, வெறுமை* _dist, மாறிலி uchar* mask)
{
    மாறிலி _Tp* src1 = (மாறிலி _Tp*)_src1;
    மாறிலி _Tp* src2 = (மாறிலி _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= அளவு(src2[0]);
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    இல்லையெனில்
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
நிலையான வெறுமை batchDistL2Sqr_(மாறிலி வெறுமை* _src1, மாறிலி வெறுமை* _src2, size_t step2,
                            முழு nvecs, முழு len, வெறுமை* _dist, மாறிலி uchar* mask)
{
    மாறிலி _Tp* src1 = (மாறிலி _Tp*)_src1;
    மாறிலி _Tp* src2 = (மாறிலி _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= அளவு(src2[0]);
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    இல்லையெனில்
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
வெறுமை batchDistL2Sqr_<தசம, தசம>(மாறிலி வெறுமை* _src1, மாறிலி வெறுமை* _src2, size_t step2,
                                   முழு nvecs, முழு len, வெறுமை* _dist, மாறிலி uchar* mask)
{
    மாறிலி தசம* src1 = (மாறிலி தசம*)_src1;
    மாறிலி தசம* src2 = (மாறிலி தசம*)_src2;
    தசம* dist = (தசம*)_dist;
    step2 /= அளவு(src2[0]);
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    இல்லையெனில்
    {
        தசம val0 = std::numeric_limits<தசம>::max();
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
நிலையான வெறுமை batchDistL2_(மாறிலி வெறுமை* _src1, மாறிலி வெறுமை* _src2, size_t step2,
                         முழு nvecs, முழு len, வெறுமை* _dist, மாறிலி uchar* mask)
{
    மாறிலி _Tp* src1 = (மாறிலி _Tp*)_src1;
    மாறிலி _Tp* src2 = (மாறிலி _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= அளவு(src2[0]);
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    இல்லையெனில்
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
வெறுமை batchDistL2_<தசம, தசம>(மாறிலி வெறுமை* _src1, மாறிலி வெறுமை* _src2, size_t step2,
                                முழு nvecs, முழு len, வெறுமை* _dist, மாறிலி uchar* mask)
{
    மாறிலி தசம* src1 = (மாறிலி தசம*)_src1;
    மாறிலி தசம* src2 = (மாறிலி தசம*)_src2;
    தசம* dist = (தசம*)_dist;
    step2 /= அளவு(src2[0]);
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    இல்லையெனில்
    {
        தசம val0 = std::numeric_limits<தசம>::max();
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

நிலையான வெறுமை batchDistHamming(மாறிலி வெறுமை* _src1, மாறிலி வெறுமை* _src2, size_t step2,
                             முழு nvecs, முழு len, வெறுமை* _dist, மாறிலி uchar* mask)
{
    மாறிலி uchar* src1 = (மாறிலி uchar*)_src1;
    மாறிலி uchar* src2 = (மாறிலி uchar*)_src2;
    முழு* dist = (முழு*)_dist;
    step2 /= அளவு(src2[0]);
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    இல்லையெனில்
    {
        முழு val0 = INT_MAX;
        க்கு( முழு i = 0; i < nvecs; i++ )
        {
            எனில் (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            இல்லையெனில்
                dist[i] = val0;
        }
    }
}

நிலையான வெறுமை batchDistHamming2(மாறிலி வெறுமை* _src1, மாறிலி வெறுமை* _src2, size_t step2,
                              முழு nvecs, முழு len, வெறுமை* _dist, மாறிலி uchar* mask)
{
    மாறிலி uchar* src1 = (மாறிலி uchar*)_src1;
    மாறிலி uchar* src2 = (மாறிலி uchar*)_src2;
    முழு* dist = (முழு*)_dist;
    step2 /= அளவு(src2[0]);
    எனில்( !mask )
    {
        க்கு( முழு i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    இல்லையெனில்
    {
        முழு val0 = INT_MAX;
        க்கு( முழு i = 0; i < nvecs; i++ )
        {
            எனில் (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            இல்லையெனில்
                dist[i] = val0;
        }
    }
}

வகைவரை வெறுமை (*BatchDistFunc)(மாறிலி வெறுமை* src1, மாறிலி வெறுமை* src2, size_t step2,
                              முழு nvecs, முழு len, வெறுமை* dist, மாறிலி uchar* mask);


கட்டமைப்பு BatchDistInvoker : பொது ParallelLoopBody
{
    BatchDistInvoker( மாறிலி Mat& _src1, மாறிலி Mat& _src2,
                      Mat& _dist, Mat& _nidx, முழு _K,
                      மாறிலி Mat& _mask, முழு _update,
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

    வெறுமை operator()(மாறிலி Range& range) மாறிலி CV_OVERRIDE
    {
        AutoBuffer<முழு> buf(src2->rows);
        முழு* bufptr = buf.data();

        க்கு( முழு i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            எனில்( K > 0 )
            {
                முழு* nidxptr = nidx->ptr<முழு>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                முழு* distptr = (முழு*)dist->ptr(i);

                முழு j, k;

                க்கு( j = 0; j < src2->rows; j++ )
                {
                    முழு d = bufptr[j];
                    எனில்( d < distptr[K-1] )
                    {
                        க்கு( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    மாறிலி Mat *src1;
    மாறிலி Mat *src2;
    Mat *dist;
    Mat *nidx;
    மாறிலி Mat *mask;
    முழு K;
    முழு update;
    BatchDistFunc func;
};

}

வெறுமை cv::batchDistance( உள்ளீடு_Array _src1, உள்ளீடு_Array _src2,
                        OutputArray _dist, முழு dtype, OutputArray _nidx,
                        முழு normType, முழு K, உள்ளீடு_Array _mask,
                        முழு update, தருக்க crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    முழு type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    எனில்( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    எனில்( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    எனில்( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (இரட்டை)INT_MAX : (இரட்டை)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    எனில்( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && mask.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batchDistance(src2, src1, tdist, dtype, tidx, normType, K, mask, 0, பொய்);
        batchDistance(src1, src2, sdist, dtype, sidx, normType, K, mask, 0, பொய்);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        எனில்( dtype == CV_32S )
        {
            க்கு( முழு i = 0; i < tdist.rows; i++ )
            {
                முழு idx = tidx.at<முழு>(i);
                முழு d = tdist.at<முழு>(i), d0 = dist.at<முழு>(idx);
                எனில்( d < d0 )
                {
                    dist.at<முழு>(idx) = d;
                    nidx.at<முழு>(idx) = i + update;
                }
            }
        }
        இல்லையெனில்
        {
            க்கு( முழு i = 0; i < tdist.rows; i++ )
            {
                முழு idx = tidx.at<முழு>(i);
                தசம d = tdist.at<தசம>(i), d0 = dist.at<தசம>(idx);
                எனில்( d < d0 )
                {
                    dist.at<தசம>(idx) = d;
                    nidx.at<முழு>(idx) = i + update;
                }
            }
        }
        க்கு( முழு i = 0; i < sdist.rows; i++ )
        {
            எனில்( tidx.at<முழு>(sidx.at<முழு>(i)) != i )
            {
                nidx.at<முழு>(i) = -1;
            }
        }
        திரும்பு;
    }

    BatchDistFunc func = 0;
    எனில்( type == CV_8U )
    {
        எனில்( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, முழு>;
        இல்லையெனில் எனில்( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, தசம>;
        இல்லையெனில் எனில்( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, முழு>;
        இல்லையெனில் எனில்( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, தசம>;
        இல்லையெனில் எனில்( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, தசம>;
        இல்லையெனில் எனில்( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        இல்லையெனில் எனில்( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    இல்லையெனில் எனில்( type == CV_32F && dtype == CV_32F )
    {
        எனில்( normType == NORM_L1 )
            func = batchDistL1_<தசம, தசம>;
        இல்லையெனில் எனில்( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<தசம, தசம>;
        இல்லையெனில் எனில்( normType == NORM_L2 )
            func = batchDistL2_<தசம, தசம>;
    }

    எனில்( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
