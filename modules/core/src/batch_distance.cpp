// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "stat.hpp"
#अन्तर्भाव <opencv2/core/hal/hal.hpp>

नामाकाश cv
{

ढाँचा<typename _Tp, typename _Rt>
स्थिर शून्य batchDistL1_(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, माप_प्रकार step2,
                         पूर्णांक nvecs, पूर्णांक len, शून्य* _dist, स्थिरांक uchar* mask)
{
    स्थिरांक _Tp* src1 = (स्थिरांक _Tp*)_src1;
    स्थिरांक _Tp* src2 = (स्थिरांक _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= आकार(src2[0]);
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

ढाँचा<typename _Tp, typename _Rt>
स्थिर शून्य batchDistL2Sqr_(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, माप_प्रकार step2,
                            पूर्णांक nvecs, पूर्णांक len, शून्य* _dist, स्थिरांक uchar* mask)
{
    स्थिरांक _Tp* src1 = (स्थिरांक _Tp*)_src1;
    स्थिरांक _Tp* src2 = (स्थिरांक _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= आकार(src2[0]);
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

ढाँचा<>
शून्य batchDistL2Sqr_<दशांश, दशांश>(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, माप_प्रकार step2,
                                   पूर्णांक nvecs, पूर्णांक len, शून्य* _dist, स्थिरांक uchar* mask)
{
    स्थिरांक दशांश* src1 = (स्थिरांक दशांश*)_src1;
    स्थिरांक दशांश* src2 = (स्थिरांक दशांश*)_src2;
    दशांश* dist = (दशांश*)_dist;
    step2 /= आकार(src2[0]);
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        दशांश val0 = std::numeric_limits<दशांश>::max();
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

ढाँचा<typename _Tp, typename _Rt>
स्थिर शून्य batchDistL2_(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, माप_प्रकार step2,
                         पूर्णांक nvecs, पूर्णांक len, शून्य* _dist, स्थिरांक uchar* mask)
{
    स्थिरांक _Tp* src1 = (स्थिरांक _Tp*)_src1;
    स्थिरांक _Tp* src2 = (स्थिरांक _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= आकार(src2[0]);
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = std::वर्ग_मूल(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::वर्ग_मूल(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

ढाँचा<>
शून्य batchDistL2_<दशांश, दशांश>(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, माप_प्रकार step2,
                                पूर्णांक nvecs, पूर्णांक len, शून्य* _dist, स्थिरांक uchar* mask)
{
    स्थिरांक दशांश* src1 = (स्थिरांक दशांश*)_src1;
    स्थिरांक दशांश* src2 = (स्थिरांक दशांश*)_src2;
    दशांश* dist = (दशांश*)_dist;
    step2 /= आकार(src2[0]);
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = std::वर्ग_मूल(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    अन्यथा
    {
        दशांश val0 = std::numeric_limits<दशांश>::max();
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::वर्ग_मूल(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

स्थिर शून्य batchDistHamming(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, माप_प्रकार step2,
                             पूर्णांक nvecs, पूर्णांक len, शून्य* _dist, स्थिरांक uchar* mask)
{
    स्थिरांक uchar* src1 = (स्थिरांक uchar*)_src1;
    स्थिरांक uchar* src2 = (स्थिरांक uchar*)_src2;
    पूर्णांक* dist = (पूर्णांक*)_dist;
    step2 /= आकार(src2[0]);
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        पूर्णांक val0 = पूर्णांक_उच्च;
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
        {
            जर (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            अन्यथा
                dist[i] = val0;
        }
    }
}

स्थिर शून्य batchDistHamming2(स्थिरांक शून्य* _src1, स्थिरांक शून्य* _src2, माप_प्रकार step2,
                              पूर्णांक nvecs, पूर्णांक len, शून्य* _dist, स्थिरांक uchar* mask)
{
    स्थिरांक uchar* src1 = (स्थिरांक uchar*)_src1;
    स्थिरांक uchar* src2 = (स्थिरांक uchar*)_src2;
    पूर्णांक* dist = (पूर्णांक*)_dist;
    step2 /= आकार(src2[0]);
    जर( !mask )
    {
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    अन्यथा
    {
        पूर्णांक val0 = पूर्णांक_उच्च;
        साठी( पूर्णांक i = 0; i < nvecs; i++ )
        {
            जर (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            अन्यथा
                dist[i] = val0;
        }
    }
}

प्रकारव्याख्या शून्य (*BatchDistFunc)(स्थिरांक शून्य* src1, स्थिरांक शून्य* src2, माप_प्रकार step2,
                              पूर्णांक nvecs, पूर्णांक len, शून्य* dist, स्थिरांक uchar* mask);


संरचना BatchDistInvoker : सार्वजनिक ParallelLoopBody
{
    BatchDistInvoker( स्थिरांक Mat& _src1, स्थिरांक Mat& _src2,
                      Mat& _dist, Mat& _nidx, पूर्णांक _K,
                      स्थिरांक Mat& _mask, पूर्णांक _update,
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

    शून्य चालक()(स्थिरांक Range& range) स्थिरांक CV_OVERRIDE
    {
        AutoBuffer<पूर्णांक> buf(src2->rows);
        पूर्णांक* bufptr = buf.data();

        साठी( पूर्णांक i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            जर( K > 0 )
            {
                पूर्णांक* nidxptr = nidx->ptr<पूर्णांक>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                पूर्णांक* distptr = (पूर्णांक*)dist->ptr(i);

                पूर्णांक j, k;

                साठी( j = 0; j < src2->rows; j++ )
                {
                    पूर्णांक d = bufptr[j];
                    जर( d < distptr[K-1] )
                    {
                        साठी( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    स्थिरांक Mat *src1;
    स्थिरांक Mat *src2;
    Mat *dist;
    Mat *nidx;
    स्थिरांक Mat *mask;
    पूर्णांक K;
    पूर्णांक update;
    BatchDistFunc func;
};

}

शून्य cv::batchDistance( आदान_Array _src1, आदान_Array _src2,
                        OutputArray _dist, पूर्णांक dtype, OutputArray _nidx,
                        पूर्णांक normType, पूर्णांक K, आदान_Array _mask,
                        पूर्णांक update, तार्किक crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    पूर्णांक type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    जर( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    जर( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    जर( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (द्विगुण)पूर्णांक_उच्च : (द्विगुण)भग्न_उच्च);
        nidx = Scalar::all(-1);
    }


    जर( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && mask.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batchDistance(src2, src1, tdist, dtype, tidx, normType, K, mask, 0, असत्य);
        batchDistance(src1, src2, sdist, dtype, sidx, normType, K, mask, 0, असत्य);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        जर( dtype == CV_32S )
        {
            साठी( पूर्णांक i = 0; i < tdist.rows; i++ )
            {
                पूर्णांक idx = tidx.at<पूर्णांक>(i);
                पूर्णांक d = tdist.at<पूर्णांक>(i), d0 = dist.at<पूर्णांक>(idx);
                जर( d < d0 )
                {
                    dist.at<पूर्णांक>(idx) = d;
                    nidx.at<पूर्णांक>(idx) = i + update;
                }
            }
        }
        अन्यथा
        {
            साठी( पूर्णांक i = 0; i < tdist.rows; i++ )
            {
                पूर्णांक idx = tidx.at<पूर्णांक>(i);
                दशांश d = tdist.at<दशांश>(i), d0 = dist.at<दशांश>(idx);
                जर( d < d0 )
                {
                    dist.at<दशांश>(idx) = d;
                    nidx.at<पूर्णांक>(idx) = i + update;
                }
            }
        }
        साठी( पूर्णांक i = 0; i < sdist.rows; i++ )
        {
            जर( tidx.at<पूर्णांक>(sidx.at<पूर्णांक>(i)) != i )
            {
                nidx.at<पूर्णांक>(i) = -1;
            }
        }
        परत;
    }

    BatchDistFunc func = 0;
    जर( type == CV_8U )
    {
        जर( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, पूर्णांक>;
        अन्यथा जर( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, दशांश>;
        अन्यथा जर( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, पूर्णांक>;
        अन्यथा जर( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, दशांश>;
        अन्यथा जर( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, दशांश>;
        अन्यथा जर( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        अन्यथा जर( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    अन्यथा जर( type == CV_32F && dtype == CV_32F )
    {
        जर( normType == NORM_L1 )
            func = batchDistL1_<दशांश, दशांश>;
        अन्यथा जर( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<दशांश, दशांश>;
        अन्यथा जर( normType == NORM_L2 )
            func = batchDistL2_<दशांश, दशांश>;
    }

    जर( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
