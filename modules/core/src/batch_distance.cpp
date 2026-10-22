// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "stat.hpp"
#include <opencv2/core/hal/hal.hpp>

namespace cv
{

template<typename _Tp, typename _Rt>
static void batchDistL1_(const void* _src1, const void* _src2, size_t step2,
                         ціле nvecs, ціле len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= sizeof(src2[0]);
    якщо( !mask )
    {
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    інакше
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
static void batchDistL2Sqr_(const void* _src1, const void* _src2, size_t step2,
                            ціле nvecs, ціле len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= sizeof(src2[0]);
    якщо( !mask )
    {
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    інакше
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
void batchDistL2Sqr_<float, float>(const void* _src1, const void* _src2, size_t step2,
                                   ціле nvecs, ціле len, void* _dist, const uchar* mask)
{
    const float* src1 = (const float*)_src1;
    const float* src2 = (const float*)_src2;
    float* dist = (float*)_dist;
    step2 /= sizeof(src2[0]);
    якщо( !mask )
    {
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    інакше
    {
        float val0 = std::numeric_limits<float>::max();
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
static void batchDistL2_(const void* _src1, const void* _src2, size_t step2,
                         ціле nvecs, ціле len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= sizeof(src2[0]);
    якщо( !mask )
    {
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    інакше
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
void batchDistL2_<float, float>(const void* _src1, const void* _src2, size_t step2,
                                ціле nvecs, ціле len, void* _dist, const uchar* mask)
{
    const float* src1 = (const float*)_src1;
    const float* src2 = (const float*)_src2;
    float* dist = (float*)_dist;
    step2 /= sizeof(src2[0]);
    якщо( !mask )
    {
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    інакше
    {
        float val0 = std::numeric_limits<float>::max();
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

static void batchDistHamming(const void* _src1, const void* _src2, size_t step2,
                             ціле nvecs, ціле len, void* _dist, const uchar* mask)
{
    const uchar* src1 = (const uchar*)_src1;
    const uchar* src2 = (const uchar*)_src2;
    ціле* dist = (ціле*)_dist;
    step2 /= sizeof(src2[0]);
    якщо( !mask )
    {
        для( ціле i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    інакше
    {
        ціле val0 = INT_MAX;
        для( ціле i = 0; i < nvecs; i++ )
        {
            якщо (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            інакше
                dist[i] = val0;
        }
    }
}

static void batchDistHamming2(const void* _src1, const void* _src2, size_t step2,
                              ціле nvecs, ціле len, void* _dist, const uchar* mask)
{
    const uchar* src1 = (const uchar*)_src1;
    const uchar* src2 = (const uchar*)_src2;
    ціле* dist = (ціле*)_dist;
    step2 /= sizeof(src2[0]);
    якщо( !mask )
    {
        для( ціле i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    інакше
    {
        ціле val0 = INT_MAX;
        для( ціле i = 0; i < nvecs; i++ )
        {
            якщо (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            інакше
                dist[i] = val0;
        }
    }
}

typedef void (*BatchDistFunc)(const void* src1, const void* src2, size_t step2,
                              ціле nvecs, ціле len, void* dist, const uchar* mask);


struct BatchDistInvoker : public ParallelLoopBody
{
    BatchDistInvoker( const Mat& _src1, const Mat& _src2,
                      Mat& _dist, Mat& _nidx, ціле _K,
                      const Mat& _mask, ціле _update,
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

    void operator()(const Range& range) const CV_OVERRIDE
    {
        AutoBuffer<ціле> buf(src2->rows);
        ціле* bufptr = buf.data();

        для( ціле i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            якщо( K > 0 )
            {
                ціле* nidxptr = nidx->ptr<ціле>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                ціле* distptr = (ціле*)dist->ptr(i);

                ціле j, k;

                для( j = 0; j < src2->rows; j++ )
                {
                    ціле d = bufptr[j];
                    якщо( d < distptr[K-1] )
                    {
                        для( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    const Mat *src1;
    const Mat *src2;
    Mat *dist;
    Mat *nidx;
    const Mat *mask;
    ціле K;
    ціле update;
    BatchDistFunc func;
};

}

void cv::batchDistance( InputArray _src1, InputArray _src2,
                        OutputArray _dist, ціле dtype, OutputArray _nidx,
                        ціле normType, ціле K, InputArray _mask,
                        ціле update, bool crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    ціле type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    якщо( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    якщо( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    якщо( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (double)INT_MAX : (double)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    якщо( crosscheck )
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
        якщо( dtype == CV_32S )
        {
            для( ціле i = 0; i < tdist.rows; i++ )
            {
                ціле idx = tidx.at<ціле>(i);
                ціле d = tdist.at<ціле>(i), d0 = dist.at<ціле>(idx);
                якщо( d < d0 )
                {
                    dist.at<ціле>(idx) = d;
                    nidx.at<ціле>(idx) = i + update;
                }
            }
        }
        інакше
        {
            для( ціле i = 0; i < tdist.rows; i++ )
            {
                ціле idx = tidx.at<ціле>(i);
                float d = tdist.at<float>(i), d0 = dist.at<float>(idx);
                якщо( d < d0 )
                {
                    dist.at<float>(idx) = d;
                    nidx.at<ціле>(idx) = i + update;
                }
            }
        }
        для( ціле i = 0; i < sdist.rows; i++ )
        {
            якщо( tidx.at<ціле>(sidx.at<ціле>(i)) != i )
            {
                nidx.at<ціле>(i) = -1;
            }
        }
        повернути;
    }

    BatchDistFunc func = 0;
    якщо( type == CV_8U )
    {
        якщо( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, ціле>;
        інакше якщо( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, float>;
        інакше якщо( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, ціле>;
        інакше якщо( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, float>;
        інакше якщо( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, float>;
        інакше якщо( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        інакше якщо( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    інакше якщо( type == CV_32F && dtype == CV_32F )
    {
        якщо( normType == NORM_L1 )
            func = batchDistL1_<float, float>;
        інакше якщо( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<float, float>;
        інакше якщо( normType == NORM_L2 )
            func = batchDistL2_<float, float>;
    }

    якщо( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
