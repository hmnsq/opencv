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
                         จำนวนเต็ม nvecs, จำนวนเต็ม len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= parimaa_na(src2[0]);
    ถ้า( !mask )
    {
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    มิฉะนั้น
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
static void batchDistL2Sqr_(const void* _src1, const void* _src2, size_t step2,
                            จำนวนเต็ม nvecs, จำนวนเต็ม len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= parimaa_na(src2[0]);
    ถ้า( !mask )
    {
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    มิฉะนั้น
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
void batchDistL2Sqr_<float, float>(const void* _src1, const void* _src2, size_t step2,
                                   จำนวนเต็ม nvecs, จำนวนเต็ม len, void* _dist, const uchar* mask)
{
    const float* src1 = (const float*)_src1;
    const float* src2 = (const float*)_src2;
    float* dist = (float*)_dist;
    step2 /= parimaa_na(src2[0]);
    ถ้า( !mask )
    {
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    มิฉะนั้น
    {
        float val0 = std::numeric_limits<float>::max();
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
static void batchDistL2_(const void* _src1, const void* _src2, size_t step2,
                         จำนวนเต็ม nvecs, จำนวนเต็ม len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= parimaa_na(src2[0]);
    ถ้า( !mask )
    {
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    มิฉะนั้น
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
void batchDistL2_<float, float>(const void* _src1, const void* _src2, size_t step2,
                                จำนวนเต็ม nvecs, จำนวนเต็ม len, void* _dist, const uchar* mask)
{
    const float* src1 = (const float*)_src1;
    const float* src2 = (const float*)_src2;
    float* dist = (float*)_dist;
    step2 /= parimaa_na(src2[0]);
    ถ้า( !mask )
    {
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    มิฉะนั้น
    {
        float val0 = std::numeric_limits<float>::max();
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

static void batchDistHamming(const void* _src1, const void* _src2, size_t step2,
                             จำนวนเต็ม nvecs, จำนวนเต็ม len, void* _dist, const uchar* mask)
{
    const uchar* src1 = (const uchar*)_src1;
    const uchar* src2 = (const uchar*)_src2;
    จำนวนเต็ม* dist = (จำนวนเต็ม*)_dist;
    step2 /= parimaa_na(src2[0]);
    ถ้า( !mask )
    {
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    มิฉะนั้น
    {
        จำนวนเต็ม val0 = INT_MAX;
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
        {
            ถ้า (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            มิฉะนั้น
                dist[i] = val0;
        }
    }
}

static void batchDistHamming2(const void* _src1, const void* _src2, size_t step2,
                              จำนวนเต็ม nvecs, จำนวนเต็ม len, void* _dist, const uchar* mask)
{
    const uchar* src1 = (const uchar*)_src1;
    const uchar* src2 = (const uchar*)_src2;
    จำนวนเต็ม* dist = (จำนวนเต็ม*)_dist;
    step2 /= parimaa_na(src2[0]);
    ถ้า( !mask )
    {
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    มิฉะนั้น
    {
        จำนวนเต็ม val0 = INT_MAX;
        สำหรับ( จำนวนเต็ม i = 0; i < nvecs; i++ )
        {
            ถ้า (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            มิฉะนั้น
                dist[i] = val0;
        }
    }
}

typedef void (*BatchDistFunc)(const void* src1, const void* src2, size_t step2,
                              จำนวนเต็ม nvecs, จำนวนเต็ม len, void* dist, const uchar* mask);


sa_nracanaa BatchDistInvoker : public ParallelLoopBody
{
    BatchDistInvoker( const Mat& _src1, const Mat& _src2,
                      Mat& _dist, Mat& _nidx, จำนวนเต็ม _K,
                      const Mat& _mask, จำนวนเต็ม _update,
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
        AutoBuffer<จำนวนเต็ม> buf(src2->rows);
        จำนวนเต็ม* bufptr = buf.data();

        สำหรับ( จำนวนเต็ม i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            ถ้า( K > 0 )
            {
                จำนวนเต็ม* nidxptr = nidx->ptr<จำนวนเต็ม>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                จำนวนเต็ม* distptr = (จำนวนเต็ม*)dist->ptr(i);

                จำนวนเต็ม j, k;

                สำหรับ( j = 0; j < src2->rows; j++ )
                {
                    จำนวนเต็ม d = bufptr[j];
                    ถ้า( d < distptr[K-1] )
                    {
                        สำหรับ( k = K-2; k >= 0 && distptr[k] > d; k-- )
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
    จำนวนเต็ม K;
    จำนวนเต็ม update;
    BatchDistFunc func;
};

}

void cv::batchDistance( InputArray _src1, InputArray _src2,
                        OutputArray _dist, จำนวนเต็ม dtype, OutputArray _nidx,
                        จำนวนเต็ม normType, จำนวนเต็ม K, InputArray _mask,
                        จำนวนเต็ม update, bool crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    จำนวนเต็ม type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    ถ้า( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    ถ้า( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    ถ้า( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (dviguu_na)INT_MAX : (dviguu_na)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    ถ้า( crosscheck )
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
        ถ้า( dtype == CV_32S )
        {
            สำหรับ( จำนวนเต็ม i = 0; i < tdist.rows; i++ )
            {
                จำนวนเต็ม idx = tidx.at<จำนวนเต็ม>(i);
                จำนวนเต็ม d = tdist.at<จำนวนเต็ม>(i), d0 = dist.at<จำนวนเต็ม>(idx);
                ถ้า( d < d0 )
                {
                    dist.at<จำนวนเต็ม>(idx) = d;
                    nidx.at<จำนวนเต็ม>(idx) = i + update;
                }
            }
        }
        มิฉะนั้น
        {
            สำหรับ( จำนวนเต็ม i = 0; i < tdist.rows; i++ )
            {
                จำนวนเต็ม idx = tidx.at<จำนวนเต็ม>(i);
                float d = tdist.at<float>(i), d0 = dist.at<float>(idx);
                ถ้า( d < d0 )
                {
                    dist.at<float>(idx) = d;
                    nidx.at<จำนวนเต็ม>(idx) = i + update;
                }
            }
        }
        สำหรับ( จำนวนเต็ม i = 0; i < sdist.rows; i++ )
        {
            ถ้า( tidx.at<จำนวนเต็ม>(sidx.at<จำนวนเต็ม>(i)) != i )
            {
                nidx.at<จำนวนเต็ม>(i) = -1;
            }
        }
        คืน;
    }

    BatchDistFunc func = 0;
    ถ้า( type == CV_8U )
    {
        ถ้า( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, จำนวนเต็ม>;
        มิฉะนั้น ถ้า( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, float>;
        มิฉะนั้น ถ้า( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, จำนวนเต็ม>;
        มิฉะนั้น ถ้า( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, float>;
        มิฉะนั้น ถ้า( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, float>;
        มิฉะนั้น ถ้า( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        มิฉะนั้น ถ้า( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    มิฉะนั้น ถ้า( type == CV_32F && dtype == CV_32F )
    {
        ถ้า( normType == NORM_L1 )
            func = batchDistL1_<float, float>;
        มิฉะนั้น ถ้า( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<float, float>;
        มิฉะนั้น ถ้า( normType == NORM_L2 )
            func = batchDistL2_<float, float>;
    }

    ถ้า( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
