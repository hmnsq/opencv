// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"
#ಅನ್ತರ್ಭಾವ <opencv2/core/hal/hal.hpp>

ನಾಮಾಕಾಶ cv
{

template<typename _Tp, typename _Rt>
ಸ್ಥಿರ ಶೂನ್ಯ batchDistL1_(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, size_t step2,
                         ಪೂರ್ಣಾಙ್ಕ nvecs, ಪೂರ್ಣಾಙ್ಕ len, ಶೂನ್ಯ* _dist, ಧ್ರುವ uchar* mask)
{
    ಧ್ರುವ _Tp* src1 = (ಧ್ರುವ _Tp*)_src1;
    ಧ್ರುವ _Tp* src2 = (ಧ್ರುವ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ಪರಿಮಾಣ(src2[0]);
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ಅನ್ಯಥಾ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
ಸ್ಥಿರ ಶೂನ್ಯ batchDistL2Sqr_(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, size_t step2,
                            ಪೂರ್ಣಾಙ್ಕ nvecs, ಪೂರ್ಣಾಙ್ಕ len, ಶೂನ್ಯ* _dist, ಧ್ರುವ uchar* mask)
{
    ಧ್ರುವ _Tp* src1 = (ಧ್ರುವ _Tp*)_src1;
    ಧ್ರುವ _Tp* src2 = (ಧ್ರುವ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ಪರಿಮಾಣ(src2[0]);
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ಅನ್ಯಥಾ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
ಶೂನ್ಯ batchDistL2Sqr_<ಭಿನ್ನ, ಭಿನ್ನ>(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, size_t step2,
                                   ಪೂರ್ಣಾಙ್ಕ nvecs, ಪೂರ್ಣಾಙ್ಕ len, ಶೂನ್ಯ* _dist, ಧ್ರುವ uchar* mask)
{
    ಧ್ರುವ ಭಿನ್ನ* src1 = (ಧ್ರುವ ಭಿನ್ನ*)_src1;
    ಧ್ರುವ ಭಿನ್ನ* src2 = (ಧ್ರುವ ಭಿನ್ನ*)_src2;
    ಭಿನ್ನ* dist = (ಭಿನ್ನ*)_dist;
    step2 /= ಪರಿಮಾಣ(src2[0]);
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    ಅನ್ಯಥಾ
    {
        ಭಿನ್ನ val0 = std::numeric_limits<ಭಿನ್ನ>::max();
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
ಸ್ಥಿರ ಶೂನ್ಯ batchDistL2_(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, size_t step2,
                         ಪೂರ್ಣಾಙ್ಕ nvecs, ಪೂರ್ಣಾಙ್ಕ len, ಶೂನ್ಯ* _dist, ಧ್ರುವ uchar* mask)
{
    ಧ್ರುವ _Tp* src1 = (ಧ್ರುವ _Tp*)_src1;
    ಧ್ರುವ _Tp* src2 = (ಧ್ರುವ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ಪರಿಮಾಣ(src2[0]);
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    ಅನ್ಯಥಾ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
ಶೂನ್ಯ batchDistL2_<ಭಿನ್ನ, ಭಿನ್ನ>(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, size_t step2,
                                ಪೂರ್ಣಾಙ್ಕ nvecs, ಪೂರ್ಣಾಙ್ಕ len, ಶೂನ್ಯ* _dist, ಧ್ರುವ uchar* mask)
{
    ಧ್ರುವ ಭಿನ್ನ* src1 = (ಧ್ರುವ ಭಿನ್ನ*)_src1;
    ಧ್ರುವ ಭಿನ್ನ* src2 = (ಧ್ರುವ ಭಿನ್ನ*)_src2;
    ಭಿನ್ನ* dist = (ಭಿನ್ನ*)_dist;
    step2 /= ಪರಿಮಾಣ(src2[0]);
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    ಅನ್ಯಥಾ
    {
        ಭಿನ್ನ val0 = std::numeric_limits<ಭಿನ್ನ>::max();
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ batchDistHamming(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, size_t step2,
                             ಪೂರ್ಣಾಙ್ಕ nvecs, ಪೂರ್ಣಾಙ್ಕ len, ಶೂನ್ಯ* _dist, ಧ್ರುವ uchar* mask)
{
    ಧ್ರುವ uchar* src1 = (ಧ್ರುವ uchar*)_src1;
    ಧ್ರುವ uchar* src2 = (ಧ್ರುವ uchar*)_src2;
    ಪೂರ್ಣಾಙ್ಕ* dist = (ಪೂರ್ಣಾಙ್ಕ*)_dist;
    step2 /= ಪರಿಮಾಣ(src2[0]);
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ val0 = INT_MAX;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
        {
            ಯದಿ (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            ಅನ್ಯಥಾ
                dist[i] = val0;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ batchDistHamming2(ಧ್ರುವ ಶೂನ್ಯ* _src1, ಧ್ರುವ ಶೂನ್ಯ* _src2, size_t step2,
                              ಪೂರ್ಣಾಙ್ಕ nvecs, ಪೂರ್ಣಾಙ್ಕ len, ಶೂನ್ಯ* _dist, ಧ್ರುವ uchar* mask)
{
    ಧ್ರುವ uchar* src1 = (ಧ್ರುವ uchar*)_src1;
    ಧ್ರುವ uchar* src2 = (ಧ್ರುವ uchar*)_src2;
    ಪೂರ್ಣಾಙ್ಕ* dist = (ಪೂರ್ಣಾಙ್ಕ*)_dist;
    step2 /= ಪರಿಮಾಣ(src2[0]);
    ಯದಿ( !mask )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ val0 = INT_MAX;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < nvecs; i++ )
        {
            ಯದಿ (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            ಅನ್ಯಥಾ
                dist[i] = val0;
        }
    }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*BatchDistFunc)(ಧ್ರುವ ಶೂನ್ಯ* src1, ಧ್ರುವ ಶೂನ್ಯ* src2, size_t step2,
                              ಪೂರ್ಣಾಙ್ಕ nvecs, ಪೂರ್ಣಾಙ್ಕ len, ಶೂನ್ಯ* dist, ಧ್ರುವ uchar* mask);


ಸಂರಚನಾ BatchDistInvoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
    BatchDistInvoker( ಧ್ರುವ Mat& _src1, ಧ್ರುವ Mat& _src2,
                      Mat& _dist, Mat& _nidx, ಪೂರ್ಣಾಙ್ಕ _K,
                      ಧ್ರುವ Mat& _mask, ಪೂರ್ಣಾಙ್ಕ _update,
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

    ಶೂನ್ಯ operator()(ಧ್ರುವ Range& range) ಧ್ರುವ CV_OVERRIDE
    {
        AutoBuffer<ಪೂರ್ಣಾಙ್ಕ> buf(src2->rows);
        ಪೂರ್ಣಾಙ್ಕ* bufptr = buf.data();

        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            ಯದಿ( K > 0 )
            {
                ಪೂರ್ಣಾಙ್ಕ* nidxptr = nidx->ptr<ಪೂರ್ಣಾಙ್ಕ>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                ಪೂರ್ಣಾಙ್ಕ* distptr = (ಪೂರ್ಣಾಙ್ಕ*)dist->ptr(i);

                ಪೂರ್ಣಾಙ್ಕ j, k;

                ಪ್ರತಿ( j = 0; j < src2->rows; j++ )
                {
                    ಪೂರ್ಣಾಙ್ಕ d = bufptr[j];
                    ಯದಿ( d < distptr[K-1] )
                    {
                        ಪ್ರತಿ( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    ಧ್ರುವ Mat *src1;
    ಧ್ರುವ Mat *src2;
    Mat *dist;
    Mat *nidx;
    ಧ್ರುವ Mat *mask;
    ಪೂರ್ಣಾಙ್ಕ K;
    ಪೂರ್ಣಾಙ್ಕ update;
    BatchDistFunc func;
};

}

ಶೂನ್ಯ cv::batchDistance( ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2,
                        OutputArray _dist, ಪೂರ್ಣಾಙ್ಕ dtype, OutputArray _nidx,
                        ಪೂರ್ಣಾಙ್ಕ normType, ಪೂರ್ಣಾಙ್ಕ K, ಒಳಹರಿವು_Array _mask,
                        ಪೂರ್ಣಾಙ್ಕ update, ತಾರ್ಕಿಕ crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    ಪೂರ್ಣಾಙ್ಕ type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    ಯದಿ( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    ಯದಿ( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    ಯದಿ( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (ದ್ವಿಗುಣ)INT_MAX : (ದ್ವಿಗುಣ)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    ಯದಿ( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && mask.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batchDistance(src2, src1, tdist, dtype, tidx, normType, K, mask, 0, ಅಸತ್ಯ);
        batchDistance(src1, src2, sdist, dtype, sidx, normType, K, mask, 0, ಅಸತ್ಯ);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        ಯದಿ( dtype == CV_32S )
        {
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < tdist.rows; i++ )
            {
                ಪೂರ್ಣಾಙ್ಕ idx = tidx.at<ಪೂರ್ಣಾಙ್ಕ>(i);
                ಪೂರ್ಣಾಙ್ಕ d = tdist.at<ಪೂರ್ಣಾಙ್ಕ>(i), d0 = dist.at<ಪೂರ್ಣಾಙ್ಕ>(idx);
                ಯದಿ( d < d0 )
                {
                    dist.at<ಪೂರ್ಣಾಙ್ಕ>(idx) = d;
                    nidx.at<ಪೂರ್ಣಾಙ್ಕ>(idx) = i + update;
                }
            }
        }
        ಅನ್ಯಥಾ
        {
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < tdist.rows; i++ )
            {
                ಪೂರ್ಣಾಙ್ಕ idx = tidx.at<ಪೂರ್ಣಾಙ್ಕ>(i);
                ಭಿನ್ನ d = tdist.at<ಭಿನ್ನ>(i), d0 = dist.at<ಭಿನ್ನ>(idx);
                ಯದಿ( d < d0 )
                {
                    dist.at<ಭಿನ್ನ>(idx) = d;
                    nidx.at<ಪೂರ್ಣಾಙ್ಕ>(idx) = i + update;
                }
            }
        }
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < sdist.rows; i++ )
        {
            ಯದಿ( tidx.at<ಪೂರ್ಣಾಙ್ಕ>(sidx.at<ಪೂರ್ಣಾಙ್ಕ>(i)) != i )
            {
                nidx.at<ಪೂರ್ಣಾಙ್ಕ>(i) = -1;
            }
        }
        ಪ್ರತ್ಯಾಗಮ;
    }

    BatchDistFunc func = 0;
    ಯದಿ( type == CV_8U )
    {
        ಯದಿ( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, ಪೂರ್ಣಾಙ್ಕ>;
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, ಭಿನ್ನ>;
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, ಪೂರ್ಣಾಙ್ಕ>;
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, ಭಿನ್ನ>;
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, ಭಿನ್ನ>;
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    ಅನ್ಯಥಾ ಯದಿ( type == CV_32F && dtype == CV_32F )
    {
        ಯದಿ( normType == NORM_L1 )
            func = batchDistL1_<ಭಿನ್ನ, ಭಿನ್ನ>;
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<ಭಿನ್ನ, ಭಿನ್ನ>;
        ಅನ್ಯಥಾ ಯದಿ( normType == NORM_L2 )
            func = batchDistL2_<ಭಿನ್ನ, ಭಿನ್ನ>;
    }

    ಯದಿ( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
