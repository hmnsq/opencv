// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "stat.hpp"
#अन्तर्भाव <opencv2/core/hal/hal.hpp>

नामाकाश cv
{

template<typename _Tp, typename _Rt>
स्थिर शून्य batchDistL1_(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, size_t step2,
                         पूर्णाङ्क nvecs, पूर्णाङ्क len, शून्य* _dist, ध्रुव uchar* mask)
{
    ध्रुव _Tp* src1 = (ध्रुव _Tp*)_src1;
    ध्रुव _Tp* src2 = (ध्रुव _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= परिमाण(src2[0]);
    यदि( !mask )
    {
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
स्थिर शून्य batchDistL2Sqr_(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, size_t step2,
                            पूर्णाङ्क nvecs, पूर्णाङ्क len, शून्य* _dist, ध्रुव uchar* mask)
{
    ध्रुव _Tp* src1 = (ध्रुव _Tp*)_src1;
    ध्रुव _Tp* src2 = (ध्रुव _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= परिमाण(src2[0]);
    यदि( !mask )
    {
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
शून्य batchDistL2Sqr_<भिन्न, भिन्न>(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, size_t step2,
                                   पूर्णाङ्क nvecs, पूर्णाङ्क len, शून्य* _dist, ध्रुव uchar* mask)
{
    ध्रुव भिन्न* src1 = (ध्रुव भिन्न*)_src1;
    ध्रुव भिन्न* src2 = (ध्रुव भिन्न*)_src2;
    भिन्न* dist = (भिन्न*)_dist;
    step2 /= परिमाण(src2[0]);
    यदि( !mask )
    {
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        भिन्न val0 = std::numeric_limits<भिन्न>::max();
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
स्थिर शून्य batchDistL2_(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, size_t step2,
                         पूर्णाङ्क nvecs, पूर्णाङ्क len, शून्य* _dist, ध्रुव uchar* mask)
{
    ध्रुव _Tp* src1 = (ध्रुव _Tp*)_src1;
    ध्रुव _Tp* src2 = (ध्रुव _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= परिमाण(src2[0]);
    यदि( !mask )
    {
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    अन्यथा
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
शून्य batchDistL2_<भिन्न, भिन्न>(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, size_t step2,
                                पूर्णाङ्क nvecs, पूर्णाङ्क len, शून्य* _dist, ध्रुव uchar* mask)
{
    ध्रुव भिन्न* src1 = (ध्रुव भिन्न*)_src1;
    ध्रुव भिन्न* src2 = (ध्रुव भिन्न*)_src2;
    भिन्न* dist = (भिन्न*)_dist;
    step2 /= परिमाण(src2[0]);
    यदि( !mask )
    {
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    अन्यथा
    {
        भिन्न val0 = std::numeric_limits<भिन्न>::max();
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

स्थिर शून्य batchDistHamming(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, size_t step2,
                             पूर्णाङ्क nvecs, पूर्णाङ्क len, शून्य* _dist, ध्रुव uchar* mask)
{
    ध्रुव uchar* src1 = (ध्रुव uchar*)_src1;
    ध्रुव uchar* src2 = (ध्रुव uchar*)_src2;
    पूर्णाङ्क* dist = (पूर्णाङ्क*)_dist;
    step2 /= परिमाण(src2[0]);
    यदि( !mask )
    {
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    अन्यथा
    {
        पूर्णाङ्क val0 = INT_MAX;
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
        {
            यदि (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            अन्यथा
                dist[i] = val0;
        }
    }
}

स्थिर शून्य batchDistHamming2(ध्रुव शून्य* _src1, ध्रुव शून्य* _src2, size_t step2,
                              पूर्णाङ्क nvecs, पूर्णाङ्क len, शून्य* _dist, ध्रुव uchar* mask)
{
    ध्रुव uchar* src1 = (ध्रुव uchar*)_src1;
    ध्रुव uchar* src2 = (ध्रुव uchar*)_src2;
    पूर्णाङ्क* dist = (पूर्णाङ्क*)_dist;
    step2 /= परिमाण(src2[0]);
    यदि( !mask )
    {
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    अन्यथा
    {
        पूर्णाङ्क val0 = INT_MAX;
        प्रति( पूर्णाङ्क i = 0; i < nvecs; i++ )
        {
            यदि (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            अन्यथा
                dist[i] = val0;
        }
    }
}

प्रकारनिर्देश शून्य (*BatchDistFunc)(ध्रुव शून्य* src1, ध्रुव शून्य* src2, size_t step2,
                              पूर्णाङ्क nvecs, पूर्णाङ्क len, शून्य* dist, ध्रुव uchar* mask);


संरचना BatchDistInvoker : सार्वजनिक ParallelLoopBody
{
    BatchDistInvoker( ध्रुव Mat& _src1, ध्रुव Mat& _src2,
                      Mat& _dist, Mat& _nidx, पूर्णाङ्क _K,
                      ध्रुव Mat& _mask, पूर्णाङ्क _update,
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

    शून्य operator()(ध्रुव Range& range) ध्रुव CV_OVERRIDE
    {
        AutoBuffer<पूर्णाङ्क> buf(src2->rows);
        पूर्णाङ्क* bufptr = buf.data();

        प्रति( पूर्णाङ्क i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            यदि( K > 0 )
            {
                पूर्णाङ्क* nidxptr = nidx->ptr<पूर्णाङ्क>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                पूर्णाङ्क* distptr = (पूर्णाङ्क*)dist->ptr(i);

                पूर्णाङ्क j, k;

                प्रति( j = 0; j < src2->rows; j++ )
                {
                    पूर्णाङ्क d = bufptr[j];
                    यदि( d < distptr[K-1] )
                    {
                        प्रति( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    ध्रुव Mat *src1;
    ध्रुव Mat *src2;
    Mat *dist;
    Mat *nidx;
    ध्रुव Mat *mask;
    पूर्णाङ्क K;
    पूर्णाङ्क update;
    BatchDistFunc func;
};

}

शून्य cv::batchDistance( निवेश_Array _src1, निवेश_Array _src2,
                        OutputArray _dist, पूर्णाङ्क dtype, OutputArray _nidx,
                        पूर्णाङ्क normType, पूर्णाङ्क K, निवेश_Array _mask,
                        पूर्णाङ्क update, तार्किक crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    पूर्णाङ्क type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    यदि( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    यदि( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    यदि( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (द्विगुण)INT_MAX : (द्विगुण)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    यदि( crosscheck )
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
        यदि( dtype == CV_32S )
        {
            प्रति( पूर्णाङ्क i = 0; i < tdist.rows; i++ )
            {
                पूर्णाङ्क idx = tidx.at<पूर्णाङ्क>(i);
                पूर्णाङ्क d = tdist.at<पूर्णाङ्क>(i), d0 = dist.at<पूर्णाङ्क>(idx);
                यदि( d < d0 )
                {
                    dist.at<पूर्णाङ्क>(idx) = d;
                    nidx.at<पूर्णाङ्क>(idx) = i + update;
                }
            }
        }
        अन्यथा
        {
            प्रति( पूर्णाङ्क i = 0; i < tdist.rows; i++ )
            {
                पूर्णाङ्क idx = tidx.at<पूर्णाङ्क>(i);
                भिन्न d = tdist.at<भिन्न>(i), d0 = dist.at<भिन्न>(idx);
                यदि( d < d0 )
                {
                    dist.at<भिन्न>(idx) = d;
                    nidx.at<पूर्णाङ्क>(idx) = i + update;
                }
            }
        }
        प्रति( पूर्णाङ्क i = 0; i < sdist.rows; i++ )
        {
            यदि( tidx.at<पूर्णाङ्क>(sidx.at<पूर्णाङ्क>(i)) != i )
            {
                nidx.at<पूर्णाङ्क>(i) = -1;
            }
        }
        प्रत्यागम;
    }

    BatchDistFunc func = 0;
    यदि( type == CV_8U )
    {
        यदि( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, पूर्णाङ्क>;
        अन्यथा यदि( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, भिन्न>;
        अन्यथा यदि( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, पूर्णाङ्क>;
        अन्यथा यदि( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, भिन्न>;
        अन्यथा यदि( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, भिन्न>;
        अन्यथा यदि( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        अन्यथा यदि( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    अन्यथा यदि( type == CV_32F && dtype == CV_32F )
    {
        यदि( normType == NORM_L1 )
            func = batchDistL1_<भिन्न, भिन्न>;
        अन्यथा यदि( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<भिन्न, भिन्न>;
        अन्यथा यदि( normType == NORM_L2 )
            func = batchDistL2_<भिन्न, भिन्न>;
    }

    यदि( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
