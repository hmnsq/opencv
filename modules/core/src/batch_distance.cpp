// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "stat.hpp"
#ଅନ୍ତର୍ଭାଵ <opencv2/core/hal/hal.hpp>

ନାମାକାଶ cv
{

template<typename _Tp, typename _Rt>
ସ୍ଥିର ଶୂନ୍ୟ batchDistL1_(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, size_t step2,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvecs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଶୂନ୍ୟ* _dist, ଅଚଳ uchar* mask)
{
    ଅଚଳ _Tp* src1 = (ଅଚଳ _Tp*)_src1;
    ଅଚଳ _Tp* src2 = (ଅଚଳ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ଆକାର(src2[0]);
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ଅନ୍ୟଥା
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
ସ୍ଥିର ଶୂନ୍ୟ batchDistL2Sqr_(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, size_t step2,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvecs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଶୂନ୍ୟ* _dist, ଅଚଳ uchar* mask)
{
    ଅଚଳ _Tp* src1 = (ଅଚଳ _Tp*)_src1;
    ଅଚଳ _Tp* src2 = (ଅଚଳ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ଆକାର(src2[0]);
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ଅନ୍ୟଥା
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
ଶୂନ୍ୟ batchDistL2Sqr_<ଦଶମିକ, ଦଶମିକ>(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, size_t step2,
                                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvecs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଶୂନ୍ୟ* _dist, ଅଚଳ uchar* mask)
{
    ଅଚଳ ଦଶମିକ* src1 = (ଅଚଳ ଦଶମିକ*)_src1;
    ଅଚଳ ଦଶମିକ* src2 = (ଅଚଳ ଦଶମିକ*)_src2;
    ଦଶମିକ* dist = (ଦଶମିକ*)_dist;
    step2 /= ଆକାର(src2[0]);
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    ଅନ୍ୟଥା
    {
        ଦଶମିକ val0 = std::numeric_limits<ଦଶମିକ>::max();
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
ସ୍ଥିର ଶୂନ୍ୟ batchDistL2_(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, size_t step2,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvecs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଶୂନ୍ୟ* _dist, ଅଚଳ uchar* mask)
{
    ଅଚଳ _Tp* src1 = (ଅଚଳ _Tp*)_src1;
    ଅଚଳ _Tp* src2 = (ଅଚଳ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ଆକାର(src2[0]);
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    ଅନ୍ୟଥା
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
ଶୂନ୍ୟ batchDistL2_<ଦଶମିକ, ଦଶମିକ>(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, size_t step2,
                                ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvecs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଶୂନ୍ୟ* _dist, ଅଚଳ uchar* mask)
{
    ଅଚଳ ଦଶମିକ* src1 = (ଅଚଳ ଦଶମିକ*)_src1;
    ଅଚଳ ଦଶମିକ* src2 = (ଅଚଳ ଦଶମିକ*)_src2;
    ଦଶମିକ* dist = (ଦଶମିକ*)_dist;
    step2 /= ଆକାର(src2[0]);
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    ଅନ୍ୟଥା
    {
        ଦଶମିକ val0 = std::numeric_limits<ଦଶମିକ>::max();
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ batchDistHamming(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, size_t step2,
                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvecs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଶୂନ୍ୟ* _dist, ଅଚଳ uchar* mask)
{
    ଅଚଳ uchar* src1 = (ଅଚଳ uchar*)_src1;
    ଅଚଳ uchar* src2 = (ଅଚଳ uchar*)_src2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dist = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_dist;
    step2 /= ଆକାର(src2[0]);
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା val0 = INT_MAX;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
        {
            ଯଦି (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            ଅନ୍ୟଥା
                dist[i] = val0;
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ batchDistHamming2(ଅଚଳ ଶୂନ୍ୟ* _src1, ଅଚଳ ଶୂନ୍ୟ* _src2, size_t step2,
                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvecs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଶୂନ୍ୟ* _dist, ଅଚଳ uchar* mask)
{
    ଅଚଳ uchar* src1 = (ଅଚଳ uchar*)_src1;
    ଅଚଳ uchar* src2 = (ଅଚଳ uchar*)_src2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* dist = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)_dist;
    step2 /= ଆକାର(src2[0]);
    ଯଦି( !mask )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା val0 = INT_MAX;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nvecs; i++ )
        {
            ଯଦି (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            ଅନ୍ୟଥା
                dist[i] = val0;
        }
    }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*BatchDistFunc)(ଅଚଳ ଶୂନ୍ୟ* src1, ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2,
                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା nvecs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଶୂନ୍ୟ* dist, ଅଚଳ uchar* mask);


ସଂରଚନା BatchDistInvoker : ସର୍ବସାଧାରଣ ParallelLoopBody
{
    BatchDistInvoker( ଅଚଳ Mat& _src1, ଅଚଳ Mat& _src2,
                      Mat& _dist, Mat& _nidx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _K,
                      ଅଚଳ Mat& _mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _update,
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

    ଶୂନ୍ୟ operator()(ଅଚଳ Range& range) ଅଚଳ CV_OVERRIDE
    {
        AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> buf(src2->rows);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା* bufptr = buf.data();

        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            ଯଦି( K > 0 )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା* nidxptr = nidx->ptr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା* distptr = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)dist->ptr(i);

                ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, k;

                ପାଇଁ( j = 0; j < src2->rows; j++ )
                {
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = bufptr[j];
                    ଯଦି( d < distptr[K-1] )
                    {
                        ପାଇଁ( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    ଅଚଳ Mat *src1;
    ଅଚଳ Mat *src2;
    Mat *dist;
    Mat *nidx;
    ଅଚଳ Mat *mask;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା K;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା update;
    BatchDistFunc func;
};

}

ଶୂନ୍ୟ cv::batchDistance( ନିବେଶ_Array _src1, ନିବେଶ_Array _src2,
                        OutputArray _dist, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype, OutputArray _nidx,
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା normType, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ନିବେଶ_Array _mask,
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା update, ତାର୍କିକ crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    ଯଦି( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    ଯଦି( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    ଯଦି( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (ଦ୍ୱିଗୁଣ)INT_MAX : (ଦ୍ୱିଗୁଣ)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    ଯଦି( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && mask.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batchDistance(src2, src1, tdist, dtype, tidx, normType, K, mask, 0, ମିଥ୍ୟା);
        batchDistance(src1, src2, sdist, dtype, sidx, normType, K, mask, 0, ମିଥ୍ୟା);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        ଯଦି( dtype == CV_32S )
        {
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < tdist.rows; i++ )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = tidx.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i);
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = tdist.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i), d0 = dist.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(idx);
                ଯଦି( d < d0 )
                {
                    dist.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(idx) = d;
                    nidx.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(idx) = i + update;
                }
            }
        }
        ଅନ୍ୟଥା
        {
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < tdist.rows; i++ )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = tidx.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i);
                ଦଶମିକ d = tdist.at<ଦଶମିକ>(i), d0 = dist.at<ଦଶମିକ>(idx);
                ଯଦି( d < d0 )
                {
                    dist.at<ଦଶମିକ>(idx) = d;
                    nidx.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(idx) = i + update;
                }
            }
        }
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < sdist.rows; i++ )
        {
            ଯଦି( tidx.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(sidx.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i)) != i )
            {
                nidx.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i) = -1;
            }
        }
        ଫେରନ୍ତୁ;
    }

    BatchDistFunc func = 0;
    ଯଦି( type == CV_8U )
    {
        ଯଦି( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>;
        ଅନ୍ୟଥା ଯଦି( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, ଦଶମିକ>;
        ଅନ୍ୟଥା ଯଦି( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>;
        ଅନ୍ୟଥା ଯଦି( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, ଦଶମିକ>;
        ଅନ୍ୟଥା ଯଦି( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, ଦଶମିକ>;
        ଅନ୍ୟଥା ଯଦି( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        ଅନ୍ୟଥା ଯଦି( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    ଅନ୍ୟଥା ଯଦି( type == CV_32F && dtype == CV_32F )
    {
        ଯଦି( normType == NORM_L1 )
            func = batchDistL1_<ଦଶମିକ, ଦଶମିକ>;
        ଅନ୍ୟଥା ଯଦି( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<ଦଶମିକ, ଦଶମିକ>;
        ଅନ୍ୟଥା ଯଦି( normType == NORM_L2 )
            func = batchDistL2_<ଦଶମିକ, ଦଶମିକ>;
    }

    ଯଦି( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
