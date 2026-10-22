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
                         ຈຳນວນເຕັມ nvecs, ຈຳນວນເຕັມ len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= parimaa_na(src2[0]);
    ຖ້າ( !mask )
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
static void batchDistL2Sqr_(const void* _src1, const void* _src2, size_t step2,
                            ຈຳນວນເຕັມ nvecs, ຈຳນວນເຕັມ len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= parimaa_na(src2[0]);
    ຖ້າ( !mask )
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
void batchDistL2Sqr_<float, float>(const void* _src1, const void* _src2, size_t step2,
                                   ຈຳນວນເຕັມ nvecs, ຈຳນວນເຕັມ len, void* _dist, const uchar* mask)
{
    const float* src1 = (const float*)_src1;
    const float* src2 = (const float*)_src2;
    float* dist = (float*)_dist;
    step2 /= parimaa_na(src2[0]);
    ຖ້າ( !mask )
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        float val0 = std::numeric_limits<float>::max();
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
static void batchDistL2_(const void* _src1, const void* _src2, size_t step2,
                         ຈຳນວນເຕັມ nvecs, ຈຳນວນເຕັມ len, void* _dist, const uchar* mask)
{
    const _Tp* src1 = (const _Tp*)_src1;
    const _Tp* src2 = (const _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= parimaa_na(src2[0]);
    ຖ້າ( !mask )
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
void batchDistL2_<float, float>(const void* _src1, const void* _src2, size_t step2,
                                ຈຳນວນເຕັມ nvecs, ຈຳນວນເຕັມ len, void* _dist, const uchar* mask)
{
    const float* src1 = (const float*)_src1;
    const float* src2 = (const float*)_src2;
    float* dist = (float*)_dist;
    step2 /= parimaa_na(src2[0]);
    ຖ້າ( !mask )
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        float val0 = std::numeric_limits<float>::max();
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

static void batchDistHamming(const void* _src1, const void* _src2, size_t step2,
                             ຈຳນວນເຕັມ nvecs, ຈຳນວນເຕັມ len, void* _dist, const uchar* mask)
{
    const uchar* src1 = (const uchar*)_src1;
    const uchar* src2 = (const uchar*)_src2;
    ຈຳນວນເຕັມ* dist = (ຈຳນວນເຕັມ*)_dist;
    step2 /= parimaa_na(src2[0]);
    ຖ້າ( !mask )
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ຈຳນວນເຕັມ val0 = INT_MAX;
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
        {
            ຖ້າ (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            ບໍ່ດັ່ງນັ້ນ
                dist[i] = val0;
        }
    }
}

static void batchDistHamming2(const void* _src1, const void* _src2, size_t step2,
                              ຈຳນວນເຕັມ nvecs, ຈຳນວນເຕັມ len, void* _dist, const uchar* mask)
{
    const uchar* src1 = (const uchar*)_src1;
    const uchar* src2 = (const uchar*)_src2;
    ຈຳນວນເຕັມ* dist = (ຈຳນວນເຕັມ*)_dist;
    step2 /= parimaa_na(src2[0]);
    ຖ້າ( !mask )
    {
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ຈຳນວນເຕັມ val0 = INT_MAX;
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < nvecs; i++ )
        {
            ຖ້າ (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            ບໍ່ດັ່ງນັ້ນ
                dist[i] = val0;
        }
    }
}

typedef void (*BatchDistFunc)(const void* src1, const void* src2, size_t step2,
                              ຈຳນວນເຕັມ nvecs, ຈຳນວນເຕັມ len, void* dist, const uchar* mask);


sa_nracanaa BatchDistInvoker : public ParallelLoopBody
{
    BatchDistInvoker( const Mat& _src1, const Mat& _src2,
                      Mat& _dist, Mat& _nidx, ຈຳນວນເຕັມ _K,
                      const Mat& _mask, ຈຳນວນເຕັມ _update,
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
        AutoBuffer<ຈຳນວນເຕັມ> buf(src2->rows);
        ຈຳນວນເຕັມ* bufptr = buf.data();

        ສຳລັບ( ຈຳນວນເຕັມ i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->data ? mask->ptr(i) : 0);

            ຖ້າ( K > 0 )
            {
                ຈຳນວນເຕັມ* nidxptr = nidx->ptr<ຈຳນວນເຕັມ>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                ຈຳນວນເຕັມ* distptr = (ຈຳນວນເຕັມ*)dist->ptr(i);

                ຈຳນວນເຕັມ j, k;

                ສຳລັບ( j = 0; j < src2->rows; j++ )
                {
                    ຈຳນວນເຕັມ d = bufptr[j];
                    ຖ້າ( d < distptr[K-1] )
                    {
                        ສຳລັບ( k = K-2; k >= 0 && distptr[k] > d; k-- )
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
    ຈຳນວນເຕັມ K;
    ຈຳນວນເຕັມ update;
    BatchDistFunc func;
};

}

void cv::batchDistance( InputArray _src1, InputArray _src2,
                        OutputArray _dist, ຈຳນວນເຕັມ dtype, OutputArray _nidx,
                        ຈຳນວນເຕັມ normType, ຈຳນວນເຕັມ K, InputArray _mask,
                        ຈຳນວນເຕັມ update, bool crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    ຈຳນວນເຕັມ type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    ຖ້າ( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    ຖ້າ( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    ຖ້າ( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (dviguu_na)INT_MAX : (dviguu_na)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    ຖ້າ( crosscheck )
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
        ຖ້າ( dtype == CV_32S )
        {
            ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < tdist.rows; i++ )
            {
                ຈຳນວນເຕັມ idx = tidx.at<ຈຳນວນເຕັມ>(i);
                ຈຳນວນເຕັມ d = tdist.at<ຈຳນວນເຕັມ>(i), d0 = dist.at<ຈຳນວນເຕັມ>(idx);
                ຖ້າ( d < d0 )
                {
                    dist.at<ຈຳນວນເຕັມ>(idx) = d;
                    nidx.at<ຈຳນວນເຕັມ>(idx) = i + update;
                }
            }
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < tdist.rows; i++ )
            {
                ຈຳນວນເຕັມ idx = tidx.at<ຈຳນວນເຕັມ>(i);
                float d = tdist.at<float>(i), d0 = dist.at<float>(idx);
                ຖ້າ( d < d0 )
                {
                    dist.at<float>(idx) = d;
                    nidx.at<ຈຳນວນເຕັມ>(idx) = i + update;
                }
            }
        }
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < sdist.rows; i++ )
        {
            ຖ້າ( tidx.at<ຈຳນວນເຕັມ>(sidx.at<ຈຳນວນເຕັມ>(i)) != i )
            {
                nidx.at<ຈຳນວນເຕັມ>(i) = -1;
            }
        }
        ສົ່ງຄືນ;
    }

    BatchDistFunc func = 0;
    ຖ້າ( type == CV_8U )
    {
        ຖ້າ( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, ຈຳນວນເຕັມ>;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, float>;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, ຈຳນວນເຕັມ>;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, float>;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, float>;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( type == CV_32F && dtype == CV_32F )
    {
        ຖ້າ( normType == NORM_L1 )
            func = batchDistL1_<float, float>;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<float, float>;
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( normType == NORM_L2 )
            func = batchDistL2_<float, float>;
    }

    ຖ້າ( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
