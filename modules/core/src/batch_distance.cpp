// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "stat.hpp"
#അന്തര്ഭാവ <opencv2/core/hal/hal.hpp>

നാമാകാശ cv
{

template<typename _Tp, typename _Rt>
സ്ഥിതം ശൂന്യം batchDistL1_(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, size_t step2,
                         പൂർണ്ണസംഖ്യ nvecs, പൂർണ്ണസംഖ്യ len, ശൂന്യം* _dist, സ്ഥിരം uchar* mask)
{
    സ്ഥിരം _Tp* src1 = (സ്ഥിരം _Tp*)_src1;
    സ്ഥിരം _Tp* src2 = (സ്ഥിരം _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= വലുപ്പം(src2[0]);
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    അല്ലെങ്കിൽ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
സ്ഥിതം ശൂന്യം batchDistL2Sqr_(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, size_t step2,
                            പൂർണ്ണസംഖ്യ nvecs, പൂർണ്ണസംഖ്യ len, ശൂന്യം* _dist, സ്ഥിരം uchar* mask)
{
    സ്ഥിരം _Tp* src1 = (സ്ഥിരം _Tp*)_src1;
    സ്ഥിരം _Tp* src2 = (സ്ഥിരം _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= വലുപ്പം(src2[0]);
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    അല്ലെങ്കിൽ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
ശൂന്യം batchDistL2Sqr_<ദശാംശം, ദശാംശം>(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, size_t step2,
                                   പൂർണ്ണസംഖ്യ nvecs, പൂർണ്ണസംഖ്യ len, ശൂന്യം* _dist, സ്ഥിരം uchar* mask)
{
    സ്ഥിരം ദശാംശം* src1 = (സ്ഥിരം ദശാംശം*)_src1;
    സ്ഥിരം ദശാംശം* src2 = (സ്ഥിരം ദശാംശം*)_src2;
    ദശാംശം* dist = (ദശാംശം*)_dist;
    step2 /= വലുപ്പം(src2[0]);
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    അല്ലെങ്കിൽ
    {
        ദശാംശം val0 = std::numeric_limits<ദശാംശം>::max();
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
സ്ഥിതം ശൂന്യം batchDistL2_(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, size_t step2,
                         പൂർണ്ണസംഖ്യ nvecs, പൂർണ്ണസംഖ്യ len, ശൂന്യം* _dist, സ്ഥിരം uchar* mask)
{
    സ്ഥിരം _Tp* src1 = (സ്ഥിരം _Tp*)_src1;
    സ്ഥിരം _Tp* src2 = (സ്ഥിരം _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= വലുപ്പം(src2[0]);
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    അല്ലെങ്കിൽ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
ശൂന്യം batchDistL2_<ദശാംശം, ദശാംശം>(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, size_t step2,
                                പൂർണ്ണസംഖ്യ nvecs, പൂർണ്ണസംഖ്യ len, ശൂന്യം* _dist, സ്ഥിരം uchar* mask)
{
    സ്ഥിരം ദശാംശം* src1 = (സ്ഥിരം ദശാംശം*)_src1;
    സ്ഥിരം ദശാംശം* src2 = (സ്ഥിരം ദശാംശം*)_src2;
    ദശാംശം* dist = (ദശാംശം*)_dist;
    step2 /= വലുപ്പം(src2[0]);
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    അല്ലെങ്കിൽ
    {
        ദശാംശം val0 = std::numeric_limits<ദശാംശം>::max();
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

സ്ഥിതം ശൂന്യം batchDistHamming(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, size_t step2,
                             പൂർണ്ണസംഖ്യ nvecs, പൂർണ്ണസംഖ്യ len, ശൂന്യം* _dist, സ്ഥിരം uchar* mask)
{
    സ്ഥിരം uchar* src1 = (സ്ഥിരം uchar*)_src1;
    സ്ഥിരം uchar* src2 = (സ്ഥിരം uchar*)_src2;
    പൂർണ്ണസംഖ്യ* dist = (പൂർണ്ണസംഖ്യ*)_dist;
    step2 /= വലുപ്പം(src2[0]);
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ val0 = INT_MAX;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
        {
            എങ്കിൽ (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            അല്ലെങ്കിൽ
                dist[i] = val0;
        }
    }
}

സ്ഥിതം ശൂന്യം batchDistHamming2(സ്ഥിരം ശൂന്യം* _src1, സ്ഥിരം ശൂന്യം* _src2, size_t step2,
                              പൂർണ്ണസംഖ്യ nvecs, പൂർണ്ണസംഖ്യ len, ശൂന്യം* _dist, സ്ഥിരം uchar* mask)
{
    സ്ഥിരം uchar* src1 = (സ്ഥിരം uchar*)_src1;
    സ്ഥിരം uchar* src2 = (സ്ഥിരം uchar*)_src2;
    പൂർണ്ണസംഖ്യ* dist = (പൂർണ്ണസംഖ്യ*)_dist;
    step2 /= വലുപ്പം(src2[0]);
    എങ്കിൽ( !mask )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    അല്ലെങ്കിൽ
    {
        പൂർണ്ണസംഖ്യ val0 = INT_MAX;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < nvecs; i++ )
        {
            എങ്കിൽ (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            അല്ലെങ്കിൽ
                dist[i] = val0;
        }
    }
}

തരനിർവചനം ശൂന്യം (*BatchDistFunc)(സ്ഥിരം ശൂന്യം* src1, സ്ഥിരം ശൂന്യം* src2, size_t step2,
                              പൂർണ്ണസംഖ്യ nvecs, പൂർണ്ണസംഖ്യ len, ശൂന്യം* dist, സ്ഥിരം uchar* mask);


ഘടന BatchDistInvoker : പൊതു ParallelLoopBody
{
    BatchDistInvoker( സ്ഥിരം Mat& _src1, സ്ഥിരം Mat& _src2,
                      Mat& _dist, Mat& _nidx, പൂർണ്ണസംഖ്യ _K,
                      സ്ഥിരം Mat& _mask, പൂർണ്ണസംഖ്യ _update,
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

    ശൂന്യം operator()(സ്ഥിരം Range& range) സ്ഥിരം CV_OVERRIDE
    {
        AutoBuffer<പൂർണ്ണസംഖ്യ> buf(src2->rows);
        പൂർണ്ണസംഖ്യ* bufptr = buf.data();

        വേണ്ടി( പൂർണ്ണസംഖ്യ i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            എങ്കിൽ( K > 0 )
            {
                പൂർണ്ണസംഖ്യ* nidxptr = nidx->ptr<പൂർണ്ണസംഖ്യ>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                പൂർണ്ണസംഖ്യ* distptr = (പൂർണ്ണസംഖ്യ*)dist->ptr(i);

                പൂർണ്ണസംഖ്യ j, k;

                വേണ്ടി( j = 0; j < src2->rows; j++ )
                {
                    പൂർണ്ണസംഖ്യ d = bufptr[j];
                    എങ്കിൽ( d < distptr[K-1] )
                    {
                        വേണ്ടി( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    സ്ഥിരം Mat *src1;
    സ്ഥിരം Mat *src2;
    Mat *dist;
    Mat *nidx;
    സ്ഥിരം Mat *mask;
    പൂർണ്ണസംഖ്യ K;
    പൂർണ്ണസംഖ്യ update;
    BatchDistFunc func;
};

}

ശൂന്യം cv::batchDistance( നിവേശം_Array _src1, നിവേശം_Array _src2,
                        OutputArray _dist, പൂർണ്ണസംഖ്യ dtype, OutputArray _nidx,
                        പൂർണ്ണസംഖ്യ normType, പൂർണ്ണസംഖ്യ K, നിവേശം_Array _mask,
                        പൂർണ്ണസംഖ്യ update, തർക്കം crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    പൂർണ്ണസംഖ്യ type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    എങ്കിൽ( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    എങ്കിൽ( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    എങ്കിൽ( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (ഇരട്ട)INT_MAX : (ഇരട്ട)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    എങ്കിൽ( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && mask.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batchDistance(src2, src1, tdist, dtype, tidx, normType, K, mask, 0, അസത്യം);
        batchDistance(src1, src2, sdist, dtype, sidx, normType, K, mask, 0, അസത്യം);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        എങ്കിൽ( dtype == CV_32S )
        {
            വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < tdist.rows; i++ )
            {
                പൂർണ്ണസംഖ്യ idx = tidx.at<പൂർണ്ണസംഖ്യ>(i);
                പൂർണ്ണസംഖ്യ d = tdist.at<പൂർണ്ണസംഖ്യ>(i), d0 = dist.at<പൂർണ്ണസംഖ്യ>(idx);
                എങ്കിൽ( d < d0 )
                {
                    dist.at<പൂർണ്ണസംഖ്യ>(idx) = d;
                    nidx.at<പൂർണ്ണസംഖ്യ>(idx) = i + update;
                }
            }
        }
        അല്ലെങ്കിൽ
        {
            വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < tdist.rows; i++ )
            {
                പൂർണ്ണസംഖ്യ idx = tidx.at<പൂർണ്ണസംഖ്യ>(i);
                ദശാംശം d = tdist.at<ദശാംശം>(i), d0 = dist.at<ദശാംശം>(idx);
                എങ്കിൽ( d < d0 )
                {
                    dist.at<ദശാംശം>(idx) = d;
                    nidx.at<പൂർണ്ണസംഖ്യ>(idx) = i + update;
                }
            }
        }
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < sdist.rows; i++ )
        {
            എങ്കിൽ( tidx.at<പൂർണ്ണസംഖ്യ>(sidx.at<പൂർണ്ണസംഖ്യ>(i)) != i )
            {
                nidx.at<പൂർണ്ണസംഖ്യ>(i) = -1;
            }
        }
        തിരികെ;
    }

    BatchDistFunc func = 0;
    എങ്കിൽ( type == CV_8U )
    {
        എങ്കിൽ( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, പൂർണ്ണസംഖ്യ>;
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, ദശാംശം>;
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, പൂർണ്ണസംഖ്യ>;
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, ദശാംശം>;
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, ദശാംശം>;
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    അല്ലെങ്കിൽ എങ്കിൽ( type == CV_32F && dtype == CV_32F )
    {
        എങ്കിൽ( normType == NORM_L1 )
            func = batchDistL1_<ദശാംശം, ദശാംശം>;
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<ദശാംശം, ദശാംശം>;
        അല്ലെങ്കിൽ എങ്കിൽ( normType == NORM_L2 )
            func = batchDistL2_<ദശാംശം, ദശാംശം>;
    }

    എങ്കിൽ( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
