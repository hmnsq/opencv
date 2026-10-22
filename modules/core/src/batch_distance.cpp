// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "stat.hpp"
#समावेश <opencv2/core/hal/hal.hpp>

namespace cv
{

ढाँचा<typename _Tp, typename _Rt>
अटल व्योम batchDistL1_(स्थिर व्योम* _src1, स्थिर व्योम* _src2, माप_प्रकार step2,
                         पूर्णांक nvecs, पूर्णांक len, व्योम* _dist, स्थिर uchar* mask)
{
    स्थिर _Tp* src1 = (स्थिर _Tp*)_src1;
    स्थिर _Tp* src2 = (स्थिर _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= माप(src2[0]);
    अगर( !mask )
    {
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

ढाँचा<typename _Tp, typename _Rt>
अटल व्योम batchDistL2Sqr_(स्थिर व्योम* _src1, स्थिर व्योम* _src2, माप_प्रकार step2,
                            पूर्णांक nvecs, पूर्णांक len, व्योम* _dist, स्थिर uchar* mask)
{
    स्थिर _Tp* src1 = (स्थिर _Tp*)_src1;
    स्थिर _Tp* src2 = (स्थिर _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= माप(src2[0]);
    अगर( !mask )
    {
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

ढाँचा<>
व्योम batchDistL2Sqr_<भग्न, भग्न>(स्थिर व्योम* _src1, स्थिर व्योम* _src2, माप_प्रकार step2,
                                   पूर्णांक nvecs, पूर्णांक len, व्योम* _dist, स्थिर uchar* mask)
{
    स्थिर भग्न* src1 = (स्थिर भग्न*)_src1;
    स्थिर भग्न* src2 = (स्थिर भग्न*)_src2;
    भग्न* dist = (भग्न*)_dist;
    step2 /= माप(src2[0]);
    अगर( !mask )
    {
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        भग्न val0 = std::numeric_limits<भग्न>::max();
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

ढाँचा<typename _Tp, typename _Rt>
अटल व्योम batchDistL2_(स्थिर व्योम* _src1, स्थिर व्योम* _src2, माप_प्रकार step2,
                         पूर्णांक nvecs, पूर्णांक len, व्योम* _dist, स्थिर uchar* mask)
{
    स्थिर _Tp* src1 = (स्थिर _Tp*)_src1;
    स्थिर _Tp* src2 = (स्थिर _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= माप(src2[0]);
    अगर( !mask )
    {
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = std::वर्ग_मूल(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::वर्ग_मूल(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

ढाँचा<>
व्योम batchDistL2_<भग्न, भग्न>(स्थिर व्योम* _src1, स्थिर व्योम* _src2, माप_प्रकार step2,
                                पूर्णांक nvecs, पूर्णांक len, व्योम* _dist, स्थिर uchar* mask)
{
    स्थिर भग्न* src1 = (स्थिर भग्न*)_src1;
    स्थिर भग्न* src2 = (स्थिर भग्न*)_src2;
    भग्न* dist = (भग्न*)_dist;
    step2 /= माप(src2[0]);
    अगर( !mask )
    {
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = std::वर्ग_मूल(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    अन्यथा
    {
        भग्न val0 = std::numeric_limits<भग्न>::max();
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::वर्ग_मूल(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

अटल व्योम batchDistHamming(स्थिर व्योम* _src1, स्थिर व्योम* _src2, माप_प्रकार step2,
                             पूर्णांक nvecs, पूर्णांक len, व्योम* _dist, स्थिर uchar* mask)
{
    स्थिर uchar* src1 = (स्थिर uchar*)_src1;
    स्थिर uchar* src2 = (स्थिर uchar*)_src2;
    पूर्णांक* dist = (पूर्णांक*)_dist;
    step2 /= माप(src2[0]);
    अगर( !mask )
    {
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        पूर्णांक val0 = पूर्णांक_उच्च;
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
        {
            अगर (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            अन्यथा
                dist[i] = val0;
        }
    }
}

अटल व्योम batchDistHamming2(स्थिर व्योम* _src1, स्थिर व्योम* _src2, माप_प्रकार step2,
                              पूर्णांक nvecs, पूर्णांक len, व्योम* _dist, स्थिर uchar* mask)
{
    स्थिर uchar* src1 = (स्थिर uchar*)_src1;
    स्थिर uchar* src2 = (स्थिर uchar*)_src2;
    पूर्णांक* dist = (पूर्णांक*)_dist;
    step2 /= माप(src2[0]);
    अगर( !mask )
    {
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    अन्यथा
    {
        पूर्णांक val0 = पूर्णांक_उच्च;
        क्रम( पूर्णांक i = 0; i < nvecs; i++ )
        {
            अगर (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            अन्यथा
                dist[i] = val0;
        }
    }
}

प्रकार व्योम (*BatchDistFunc)(स्थिर व्योम* src1, स्थिर व्योम* src2, माप_प्रकार step2,
                              पूर्णांक nvecs, पूर्णांक len, व्योम* dist, स्थिर uchar* mask);


काष्ठा BatchDistInvoker : खुला ParallelLoopBody
{
    BatchDistInvoker( स्थिर Mat& _src1, स्थिर Mat& _src2,
                      Mat& _dist, Mat& _nidx, पूर्णांक _K,
                      स्थिर Mat& _mask, पूर्णांक _update,
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

    व्योम चालक()(स्थिर Range& range) स्थिर CV_OVERRIDE
    {
        Auto_बफर<पूर्णांक> buf(src2->rows);
        पूर्णांक* bufptr = buf.तथ्य();

        क्रम( पूर्णांक i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->तथ्य ? mask->ptr(i) : 0);

            अगर( K > 0 )
            {
                पूर्णांक* nidxptr = nidx->ptr<पूर्णांक>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                पूर्णांक* distptr = (पूर्णांक*)dist->ptr(i);

                पूर्णांक j, k;

                क्रम( j = 0; j < src2->rows; j++ )
                {
                    पूर्णांक d = bufptr[j];
                    अगर( d < distptr[K-1] )
                    {
                        क्रम( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    स्थिर Mat *src1;
    स्थिर Mat *src2;
    Mat *dist;
    Mat *nidx;
    स्थिर Mat *mask;
    पूर्णांक K;
    पूर्णांक update;
    BatchDistFunc func;
};

}

व्योम cv::batchDistance( निवेश_Array _src1, निवेश_Array _src2,
                        OutputArray _dist, पूर्णांक dtype, OutputArray _nidx,
                        पूर्णांक normType, पूर्णांक K, निवेश_Array _mask,
                        पूर्णांक update, bool crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    पूर्णांक type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    अगर( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    अगर( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    अगर( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (द्विगुन)पूर्णांक_उच्च : (द्विगुन)भग्न_उच्च);
        nidx = Scalar::all(-1);
    }


    अगर( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && mask.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batchDistance(src2, src1, tdist, dtype, tidx, normType, K, mask, 0, false);
        batchDistance(src1, src2, sdist, dtype, sidx, normType, K, mask, 0, false);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        अगर( dtype == CV_32S )
        {
            क्रम( पूर्णांक i = 0; i < tdist.rows; i++ )
            {
                पूर्णांक idx = tidx.at<पूर्णांक>(i);
                पूर्णांक d = tdist.at<पूर्णांक>(i), d0 = dist.at<पूर्णांक>(idx);
                अगर( d < d0 )
                {
                    dist.at<पूर्णांक>(idx) = d;
                    nidx.at<पूर्णांक>(idx) = i + update;
                }
            }
        }
        अन्यथा
        {
            क्रम( पूर्णांक i = 0; i < tdist.rows; i++ )
            {
                पूर्णांक idx = tidx.at<पूर्णांक>(i);
                भग्न d = tdist.at<भग्न>(i), d0 = dist.at<भग्न>(idx);
                अगर( d < d0 )
                {
                    dist.at<भग्न>(idx) = d;
                    nidx.at<पूर्णांक>(idx) = i + update;
                }
            }
        }
        क्रम( पूर्णांक i = 0; i < sdist.rows; i++ )
        {
            अगर( tidx.at<पूर्णांक>(sidx.at<पूर्णांक>(i)) != i )
            {
                nidx.at<पूर्णांक>(i) = -1;
            }
        }
        वापस;
    }

    BatchDistFunc func = 0;
    अगर( type == CV_8U )
    {
        अगर( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, पूर्णांक>;
        अन्यथा अगर( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, भग्न>;
        अन्यथा अगर( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, पूर्णांक>;
        अन्यथा अगर( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, भग्न>;
        अन्यथा अगर( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, भग्न>;
        अन्यथा अगर( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        अन्यथा अगर( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    अन्यथा अगर( type == CV_32F && dtype == CV_32F )
    {
        अगर( normType == NORM_L1 )
            func = batchDistL1_<भग्न, भग्न>;
        अन्यथा अगर( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<भग्न, भग्न>;
        अन्यथा अगर( normType == NORM_L2 )
            func = batchDistL2_<भग्न, भग्न>;
    }

    अगर( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
