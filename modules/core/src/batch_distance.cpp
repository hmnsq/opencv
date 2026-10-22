// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "stat.hpp"
#شامل <opencv2/core/hal/hal.hpp>

نامخلا cv
{

سانچہ<قسم_نام _Tp, قسم_نام _Rt>
ساکن خالی batchDistL1_(ثابت خالی* _src1, ثابت خالی* _src2, size_t step2,
                         صحیح nvecs, صحیح len, خالی* _dist, ثابت uchar* نقاب)
{
    ثابت _Tp* src1 = (ثابت _Tp*)_src1;
    ثابت _Tp* src2 = (ثابت _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= حجم(src2[0]);
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ورنہ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

سانچہ<قسم_نام _Tp, قسم_نام _Rt>
ساکن خالی batchDistL2Sqr_(ثابت خالی* _src1, ثابت خالی* _src2, size_t step2,
                            صحیح nvecs, صحیح len, خالی* _dist, ثابت uchar* نقاب)
{
    ثابت _Tp* src1 = (ثابت _Tp*)_src1;
    ثابت _Tp* src2 = (ثابت _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= حجم(src2[0]);
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ورنہ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

سانچہ<>
خالی batchDistL2Sqr_<اعشاری, اعشاری>(ثابت خالی* _src1, ثابت خالی* _src2, size_t step2,
                                   صحیح nvecs, صحیح len, خالی* _dist, ثابت uchar* نقاب)
{
    ثابت اعشاری* src1 = (ثابت اعشاری*)_src1;
    ثابت اعشاری* src2 = (ثابت اعشاری*)_src2;
    اعشاری* dist = (اعشاری*)_dist;
    step2 /= حجم(src2[0]);
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    ورنہ
    {
        اعشاری val0 = std::numeric_limits<اعشاری>::max();
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

سانچہ<قسم_نام _Tp, قسم_نام _Rt>
ساکن خالی batchDistL2_(ثابت خالی* _src1, ثابت خالی* _src2, size_t step2,
                         صحیح nvecs, صحیح len, خالی* _dist, ثابت uchar* نقاب)
{
    ثابت _Tp* src1 = (ثابت _Tp*)_src1;
    ثابت _Tp* src2 = (ثابت _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= حجم(src2[0]);
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = std::جذر(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    ورنہ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? std::جذر(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

سانچہ<>
خالی batchDistL2_<اعشاری, اعشاری>(ثابت خالی* _src1, ثابت خالی* _src2, size_t step2,
                                صحیح nvecs, صحیح len, خالی* _dist, ثابت uchar* نقاب)
{
    ثابت اعشاری* src1 = (ثابت اعشاری*)_src1;
    ثابت اعشاری* src2 = (ثابت اعشاری*)_src2;
    اعشاری* dist = (اعشاری*)_dist;
    step2 /= حجم(src2[0]);
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = std::جذر(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    ورنہ
    {
        اعشاری val0 = std::numeric_limits<اعشاری>::max();
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = نقاب[i] ? std::جذر(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

ساکن خالی batchDistHamming(ثابت خالی* _src1, ثابت خالی* _src2, size_t step2,
                             صحیح nvecs, صحیح len, خالی* _dist, ثابت uchar* نقاب)
{
    ثابت uchar* src1 = (ثابت uchar*)_src1;
    ثابت uchar* src2 = (ثابت uchar*)_src2;
    صحیح* dist = (صحیح*)_dist;
    step2 /= حجم(src2[0]);
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    ورنہ
    {
        صحیح val0 = زیادہ_سے_زیادہ_عدد;
        برائے( صحیح i = 0; i < nvecs; i++ )
        {
            اگر (نقاب[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            ورنہ
                dist[i] = val0;
        }
    }
}

ساکن خالی batchDistHamming2(ثابت خالی* _src1, ثابت خالی* _src2, size_t step2,
                              صحیح nvecs, صحیح len, خالی* _dist, ثابت uchar* نقاب)
{
    ثابت uchar* src1 = (ثابت uchar*)_src1;
    ثابت uchar* src2 = (ثابت uchar*)_src2;
    صحیح* dist = (صحیح*)_dist;
    step2 /= حجم(src2[0]);
    اگر( !نقاب )
    {
        برائے( صحیح i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    ورنہ
    {
        صحیح val0 = زیادہ_سے_زیادہ_عدد;
        برائے( صحیح i = 0; i < nvecs; i++ )
        {
            اگر (نقاب[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            ورنہ
                dist[i] = val0;
        }
    }
}

قسمتعریف خالی (*BatchDistFunc)(ثابت خالی* src1, ثابت خالی* src2, size_t step2,
                              صحیح nvecs, صحیح len, خالی* dist, ثابت uchar* نقاب);


ساخت BatchDistInvoker : عوامی ParallelLoopBody
{
    BatchDistInvoker( ثابت Mat& _src1, ثابت Mat& _src2,
                      Mat& _dist, Mat& _nidx, صحیح _K,
                      ثابت Mat& نقاب_2, صحیح _update,
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

    خالی عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        Auto_بفر<صحیح> buf(src2->صفیں);
        صحیح* bufptr = buf.ڈیٹا();

        برائے( صحیح i = range.آغاز; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->قدم, src2->صفیں, src2->ستون_تعداد,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), نقاب->ڈیٹا ? نقاب->ptr(i) : 0);

            اگر( K > 0 )
            {
                صحیح* nidxptr = nidx->ptr<صحیح>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                صحیح* distptr = (صحیح*)dist->ptr(i);

                صحیح j, k;

                برائے( j = 0; j < src2->صفیں; j++ )
                {
                    صحیح d = bufptr[j];
                    اگر( d < distptr[K-1] )
                    {
                        برائے( k = K-2; k >= 0 && distptr[k] > d; k-- )
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
    صحیح K;
    صحیح update;
    BatchDistFunc func;
};

}

خالی cv::batch_فاصلہ( داخلہ_Array _src1, داخلہ_Array _src2,
                        OutputArray _dist, صحیح dtype, OutputArray _nidx,
                        صحیح normType, صحیح K, داخلہ_Array نقاب_2,
                        صحیح update, منطقی crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), نقاب = نقاب_2.getMat();
    صحیح type = src1.type();
    CV_Assert( type == src2.type() && src1.ستون_تعداد == src2.ستون_تعداد &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    اگر( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.صفیں);

    _dist.create(src1.صفیں, (K > 0 ? K : src2.صفیں), dtype);
    Mat dist = _dist.getMat(), nidx;
    اگر( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    اگر( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (دوہرا)زیادہ_سے_زیادہ_عدد : (دوہرا)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    اگر( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && نقاب.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batch_فاصلہ(src2, src1, tdist, dtype, tidx, normType, K, نقاب, 0, جھوٹ);
        batch_فاصلہ(src1, src2, sdist, dtype, sidx, normType, K, نقاب, 0, جھوٹ);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        اگر( dtype == CV_32S )
        {
            برائے( صحیح i = 0; i < tdist.صفیں; i++ )
            {
                صحیح idx = tidx.at<صحیح>(i);
                صحیح d = tdist.at<صحیح>(i), d0 = dist.at<صحیح>(idx);
                اگر( d < d0 )
                {
                    dist.at<صحیح>(idx) = d;
                    nidx.at<صحیح>(idx) = i + update;
                }
            }
        }
        ورنہ
        {
            برائے( صحیح i = 0; i < tdist.صفیں; i++ )
            {
                صحیح idx = tidx.at<صحیح>(i);
                اعشاری d = tdist.at<اعشاری>(i), d0 = dist.at<اعشاری>(idx);
                اگر( d < d0 )
                {
                    dist.at<اعشاری>(idx) = d;
                    nidx.at<صحیح>(idx) = i + update;
                }
            }
        }
        برائے( صحیح i = 0; i < sdist.صفیں; i++ )
        {
            اگر( tidx.at<صحیح>(sidx.at<صحیح>(i)) != i )
            {
                nidx.at<صحیح>(i) = -1;
            }
        }
        واپس;
    }

    BatchDistFunc func = 0;
    اگر( type == CV_8U )
    {
        اگر( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, صحیح>;
        ورنہ اگر( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, اعشاری>;
        ورنہ اگر( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, صحیح>;
        ورنہ اگر( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, اعشاری>;
        ورنہ اگر( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, اعشاری>;
        ورنہ اگر( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        ورنہ اگر( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    ورنہ اگر( type == CV_32F && dtype == CV_32F )
    {
        اگر( normType == NORM_L1 )
            func = batchDistL1_<اعشاری, اعشاری>;
        ورنہ اگر( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<اعشاری, اعشاری>;
        ورنہ اگر( normType == NORM_L2 )
            func = batchDistL2_<اعشاری, اعشاری>;
    }

    اگر( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.صفیں),
                  BatchDistInvoker(src1, src2, dist, nidx, K, نقاب, update, func));
}
