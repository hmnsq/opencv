// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "stat.hpp"
#ضمن <opencv2/core/hal/hal.hpp>

نطاق cv
{

سانچو<قسم_نالو _Tp, قسم_نالو _Rt>
ساكن فراغ batchDistL1_(ثابت فراغ* _src1, ثابت فراغ* _src2, size_t step2,
                         صحيح nvecs, صحيح len, فراغ* _dist, ثابت uchar* نقاب)
{
    ثابت _Tp* src1 = (ثابت _Tp*)_src1;
    ثابت _Tp* src2 = (ثابت _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= حجم(src2[0]);
    إذا( !نقاب )
    {
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    وإلا
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

سانچو<قسم_نالو _Tp, قسم_نالو _Rt>
ساكن فراغ batchDistL2Sqr_(ثابت فراغ* _src1, ثابت فراغ* _src2, size_t step2,
                            صحيح nvecs, صحيح len, فراغ* _dist, ثابت uchar* نقاب)
{
    ثابت _Tp* src1 = (ثابت _Tp*)_src1;
    ثابت _Tp* src2 = (ثابت _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= حجم(src2[0]);
    إذا( !نقاب )
    {
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    وإلا
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

سانچو<>
فراغ batchDistL2Sqr_<عائم, عائم>(ثابت فراغ* _src1, ثابت فراغ* _src2, size_t step2,
                                   صحيح nvecs, صحيح len, فراغ* _dist, ثابت uchar* نقاب)
{
    ثابت عائم* src1 = (ثابت عائم*)_src1;
    ثابت عائم* src2 = (ثابت عائم*)_src2;
    عائم* dist = (عائم*)_dist;
    step2 /= حجم(src2[0]);
    إذا( !نقاب )
    {
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    وإلا
    {
        عائم val0 = std::numeric_limits<عائم>::max();
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

سانچو<قسم_نالو _Tp, قسم_نالو _Rt>
ساكن فراغ batchDistL2_(ثابت فراغ* _src1, ثابت فراغ* _src2, size_t step2,
                         صحيح nvecs, صحيح len, فراغ* _dist, ثابت uchar* نقاب)
{
    ثابت _Tp* src1 = (ثابت _Tp*)_src1;
    ثابت _Tp* src2 = (ثابت _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= حجم(src2[0]);
    إذا( !نقاب )
    {
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = std::جذر(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    وإلا
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? std::جذر(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

سانچو<>
فراغ batchDistL2_<عائم, عائم>(ثابت فراغ* _src1, ثابت فراغ* _src2, size_t step2,
                                صحيح nvecs, صحيح len, فراغ* _dist, ثابت uchar* نقاب)
{
    ثابت عائم* src1 = (ثابت عائم*)_src1;
    ثابت عائم* src2 = (ثابت عائم*)_src2;
    عائم* dist = (عائم*)_dist;
    step2 /= حجم(src2[0]);
    إذا( !نقاب )
    {
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = std::جذر(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    وإلا
    {
        عائم val0 = std::numeric_limits<عائم>::max();
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? std::جذر(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

ساكن فراغ batchDistHamming(ثابت فراغ* _src1, ثابت فراغ* _src2, size_t step2,
                             صحيح nvecs, صحيح len, فراغ* _dist, ثابت uchar* نقاب)
{
    ثابت uchar* src1 = (ثابت uchar*)_src1;
    ثابت uchar* src2 = (ثابت uchar*)_src2;
    صحيح* dist = (صحيح*)_dist;
    step2 /= حجم(src2[0]);
    إذا( !نقاب )
    {
        لكل( صحيح i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    وإلا
    {
        صحيح val0 = وڌ_کان_وڌ_عدد;
        لكل( صحيح i = 0; i < nvecs; i++ )
        {
            إذا (نقاب[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            وإلا
                dist[i] = val0;
        }
    }
}

ساكن فراغ batchDistHamming2(ثابت فراغ* _src1, ثابت فراغ* _src2, size_t step2,
                              صحيح nvecs, صحيح len, فراغ* _dist, ثابت uchar* نقاب)
{
    ثابت uchar* src1 = (ثابت uchar*)_src1;
    ثابت uchar* src2 = (ثابت uchar*)_src2;
    صحيح* dist = (صحيح*)_dist;
    step2 /= حجم(src2[0]);
    إذا( !نقاب )
    {
        لكل( صحيح i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    وإلا
    {
        صحيح val0 = وڌ_کان_وڌ_عدد;
        لكل( صحيح i = 0; i < nvecs; i++ )
        {
            إذا (نقاب[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            وإلا
                dist[i] = val0;
        }
    }
}

تعريفنوع فراغ (*BatchDistFunc)(ثابت فراغ* src1, ثابت فراغ* src2, size_t step2,
                              صحيح nvecs, صحيح len, فراغ* dist, ثابت uchar* نقاب);


بنية BatchDistInvoker : عام ParallelLoopBody
{
    BatchDistInvoker( ثابت Mat& _src1, ثابت Mat& _src2,
                      Mat& _dist, Mat& _nidx, صحيح _K,
                      ثابت Mat& نقاب_2, صحيح _update,
                      BatchDistFunc _func)
    {
        src1 = &_src1;
        src2 = &_src2;
        dist = &_dist;
        nidx = &_nidx;
        K = _K;
        نقاب = &نقاب_2;
        update = _update;
        func = _func;
    }

    فراغ عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        Auto_بفر<صحيح> buf(src2->صفون);
        صحيح* bufptr = buf.معلومات();

        لكل( صحيح i = range.شروعات; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->قدم, src2->صفون, src2->کانن_ڳڻپ,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), نقاب->معلومات ? نقاب->ptr(i) : 0);

            إذا( K > 0 )
            {
                صحيح* nidxptr = nidx->ptr<صحيح>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                صحيح* distptr = (صحيح*)dist->ptr(i);

                صحيح j, k;

                لكل( j = 0; j < src2->صفون; j++ )
                {
                    صحيح d = bufptr[j];
                    إذا( d < distptr[K-1] )
                    {
                        لكل( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    ثابت Mat *src1;
    ثابت Mat *src2;
    Mat *dist;
    Mat *nidx;
    ثابت Mat *نقاب;
    صحيح K;
    صحيح update;
    BatchDistFunc func;
};

}

فراغ cv::batch_مفاصلو( داخلا_Array _src1, داخلا_Array _src2,
                        OutputArray _dist, صحيح dtype, OutputArray _nidx,
                        صحيح normType, صحيح K, داخلا_Array نقاب_2,
                        صحيح update, منطقي crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), نقاب = نقاب_2.getMat();
    صحيح type = src1.type();
    CV_Assert( type == src2.type() && src1.کانن_ڳڻپ == src2.کانن_ڳڻپ &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    إذا( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.صفون);

    _dist.create(src1.صفون, (K > 0 ? K : src2.صفون), dtype);
    Mat dist = _dist.getMat(), nidx;
    إذا( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    إذا( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (مزدوج)وڌ_کان_وڌ_عدد : (مزدوج)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    إذا( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && نقاب.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batch_مفاصلو(src2, src1, tdist, dtype, tidx, normType, K, نقاب, 0, خطأ);
        batch_مفاصلو(src1, src2, sdist, dtype, sidx, normType, K, نقاب, 0, خطأ);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        إذا( dtype == CV_32S )
        {
            لكل( صحيح i = 0; i < tdist.صفون; i++ )
            {
                صحيح idx = tidx.at<صحيح>(i);
                صحيح d = tdist.at<صحيح>(i), d0 = dist.at<صحيح>(idx);
                إذا( d < d0 )
                {
                    dist.at<صحيح>(idx) = d;
                    nidx.at<صحيح>(idx) = i + update;
                }
            }
        }
        وإلا
        {
            لكل( صحيح i = 0; i < tdist.صفون; i++ )
            {
                صحيح idx = tidx.at<صحيح>(i);
                عائم d = tdist.at<عائم>(i), d0 = dist.at<عائم>(idx);
                إذا( d < d0 )
                {
                    dist.at<عائم>(idx) = d;
                    nidx.at<صحيح>(idx) = i + update;
                }
            }
        }
        لكل( صحيح i = 0; i < sdist.صفون; i++ )
        {
            إذا( tidx.at<صحيح>(sidx.at<صحيح>(i)) != i )
            {
                nidx.at<صحيح>(i) = -1;
            }
        }
        أرجع;
    }

    BatchDistFunc func = 0;
    إذا( type == CV_8U )
    {
        إذا( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, صحيح>;
        وإلا إذا( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, عائم>;
        وإلا إذا( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, صحيح>;
        وإلا إذا( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, عائم>;
        وإلا إذا( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, عائم>;
        وإلا إذا( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        وإلا إذا( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    وإلا إذا( type == CV_32F && dtype == CV_32F )
    {
        إذا( normType == NORM_L1 )
            func = batchDistL1_<عائم, عائم>;
        وإلا إذا( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<عائم, عائم>;
        وإلا إذا( normType == NORM_L2 )
            func = batchDistL2_<عائم, عائم>;
    }

    إذا( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.صفون),
                  BatchDistInvoker(src1, src2, dist, nidx, K, نقاب, update, func));
}
