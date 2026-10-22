// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "stat.hpp"
#অন্তর্ভাব <opencv2/core/hal/hal.hpp>

নামাকাশ cv
{

template<typename _Tp, typename _Rt>
স্থির শূন্য batchDistL1_(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, size_t step2,
                         পূর্ণসংখ্যা nvecs, পূর্ণসংখ্যা len, শূন্য* _dist, ধ্রুবক uchar* mask)
{
    ধ্রুবক _Tp* src1 = (ধ্রুবক _Tp*)_src1;
    ধ্রুবক _Tp* src2 = (ধ্রুবক _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= আকার(src2[0]);
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    অন্যথা
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
স্থির শূন্য batchDistL2Sqr_(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, size_t step2,
                            পূর্ণসংখ্যা nvecs, পূর্ণসংখ্যা len, শূন্য* _dist, ধ্রুবক uchar* mask)
{
    ধ্রুবক _Tp* src1 = (ধ্রুবক _Tp*)_src1;
    ধ্রুবক _Tp* src2 = (ধ্রুবক _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= আকার(src2[0]);
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    অন্যথা
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

template<>
শূন্য batchDistL2Sqr_<দশমিক, দশমিক>(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, size_t step2,
                                   পূর্ণসংখ্যা nvecs, পূর্ণসংখ্যা len, শূন্য* _dist, ধ্রুবক uchar* mask)
{
    ধ্রুবক দশমিক* src1 = (ধ্রুবক দশমিক*)_src1;
    ধ্রুবক দশমিক* src2 = (ধ্রুবক দশমিক*)_src2;
    দশমিক* dist = (দশমিক*)_dist;
    step2 /= আকার(src2[0]);
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    অন্যথা
    {
        দশমিক val0 = std::numeric_limits<দশমিক>::max();
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

template<typename _Tp, typename _Rt>
স্থির শূন্য batchDistL2_(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, size_t step2,
                         পূর্ণসংখ্যা nvecs, পূর্ণসংখ্যা len, শূন্য* _dist, ধ্রুবক uchar* mask)
{
    ধ্রুবক _Tp* src1 = (ধ্রুবক _Tp*)_src1;
    ধ্রুবক _Tp* src2 = (ধ্রুবক _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= আকার(src2[0]);
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    অন্যথা
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

template<>
শূন্য batchDistL2_<দশমিক, দশমিক>(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, size_t step2,
                                পূর্ণসংখ্যা nvecs, পূর্ণসংখ্যা len, শূন্য* _dist, ধ্রুবক uchar* mask)
{
    ধ্রুবক দশমিক* src1 = (ধ্রুবক দশমিক*)_src1;
    ধ্রুবক দশমিক* src2 = (ধ্রুবক দশমিক*)_src2;
    দশমিক* dist = (দশমিক*)_dist;
    step2 /= আকার(src2[0]);
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    অন্যথা
    {
        দশমিক val0 = std::numeric_limits<দশমিক>::max();
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = mask[i] ? std::sqrt(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

স্থির শূন্য batchDistHamming(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, size_t step2,
                             পূর্ণসংখ্যা nvecs, পূর্ণসংখ্যা len, শূন্য* _dist, ধ্রুবক uchar* mask)
{
    ধ্রুবক uchar* src1 = (ধ্রুবক uchar*)_src1;
    ধ্রুবক uchar* src2 = (ধ্রুবক uchar*)_src2;
    পূর্ণসংখ্যা* dist = (পূর্ণসংখ্যা*)_dist;
    step2 /= আকার(src2[0]);
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    অন্যথা
    {
        পূর্ণসংখ্যা val0 = INT_MAX;
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
        {
            যদি (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            অন্যথা
                dist[i] = val0;
        }
    }
}

স্থির শূন্য batchDistHamming2(ধ্রুবক শূন্য* _src1, ধ্রুবক শূন্য* _src2, size_t step2,
                              পূর্ণসংখ্যা nvecs, পূর্ণসংখ্যা len, শূন্য* _dist, ধ্রুবক uchar* mask)
{
    ধ্রুবক uchar* src1 = (ধ্রুবক uchar*)_src1;
    ধ্রুবক uchar* src2 = (ধ্রুবক uchar*)_src2;
    পূর্ণসংখ্যা* dist = (পূর্ণসংখ্যা*)_dist;
    step2 /= আকার(src2[0]);
    যদি( !mask )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    অন্যথা
    {
        পূর্ণসংখ্যা val0 = INT_MAX;
        জন্য( পূর্ণসংখ্যা i = 0; i < nvecs; i++ )
        {
            যদি (mask[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            অন্যথা
                dist[i] = val0;
        }
    }
}

প্রকারলেখ শূন্য (*BatchDistFunc)(ধ্রুবক শূন্য* src1, ধ্রুবক শূন্য* src2, size_t step2,
                              পূর্ণসংখ্যা nvecs, পূর্ণসংখ্যা len, শূন্য* dist, ধ্রুবক uchar* mask);


কাঠামো BatchDistInvoker : সর্বজনীন ParallelLoopBody
{
    BatchDistInvoker( ধ্রুবক Mat& _src1, ধ্রুবক Mat& _src2,
                      Mat& _dist, Mat& _nidx, পূর্ণসংখ্যা _K,
                      ধ্রুবক Mat& _mask, পূর্ণসংখ্যা _update,
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

    শূন্য operator()(ধ্রুবক Range& range) ধ্রুবক CV_OVERRIDE
    {
        Auto_বাফার<পূর্ণসংখ্যা> buf(src2->rows);
        পূর্ণসংখ্যা* bufptr = buf.তথ্য();

        জন্য( পূর্ণসংখ্যা i = range.start; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->step, src2->rows, src2->cols,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), mask->তথ্য ? mask->ptr(i) : 0);

            যদি( K > 0 )
            {
                পূর্ণসংখ্যা* nidxptr = nidx->ptr<পূর্ণসংখ্যা>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                পূর্ণসংখ্যা* distptr = (পূর্ণসংখ্যা*)dist->ptr(i);

                পূর্ণসংখ্যা j, k;

                জন্য( j = 0; j < src2->rows; j++ )
                {
                    পূর্ণসংখ্যা d = bufptr[j];
                    যদি( d < distptr[K-1] )
                    {
                        জন্য( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    ধ্রুবক Mat *src1;
    ধ্রুবক Mat *src2;
    Mat *dist;
    Mat *nidx;
    ধ্রুবক Mat *mask;
    পূর্ণসংখ্যা K;
    পূর্ণসংখ্যা update;
    BatchDistFunc func;
};

}

শূন্য cv::batchDistance( প্রবেশ_Array _src1, প্রবেশ_Array _src2,
                        OutputArray _dist, পূর্ণসংখ্যা dtype, OutputArray _nidx,
                        পূর্ণসংখ্যা normType, পূর্ণসংখ্যা K, প্রবেশ_Array _mask,
                        পূর্ণসংখ্যা update, যৌক্তিক crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), mask = _mask.getMat();
    পূর্ণসংখ্যা type = src1.type();
    CV_Assert( type == src2.type() && src1.cols == src2.cols &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    যদি( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.rows);

    _dist.create(src1.rows, (K > 0 ? K : src2.rows), dtype);
    Mat dist = _dist.getMat(), nidx;
    যদি( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    যদি( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (দ্বিগুণ)INT_MAX : (দ্বিগুণ)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    যদি( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && mask.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batchDistance(src2, src1, tdist, dtype, tidx, normType, K, mask, 0, মিথ্যা);
        batchDistance(src1, src2, sdist, dtype, sidx, normType, K, mask, 0, মিথ্যা);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        যদি( dtype == CV_32S )
        {
            জন্য( পূর্ণসংখ্যা i = 0; i < tdist.rows; i++ )
            {
                পূর্ণসংখ্যা idx = tidx.at<পূর্ণসংখ্যা>(i);
                পূর্ণসংখ্যা d = tdist.at<পূর্ণসংখ্যা>(i), d0 = dist.at<পূর্ণসংখ্যা>(idx);
                যদি( d < d0 )
                {
                    dist.at<পূর্ণসংখ্যা>(idx) = d;
                    nidx.at<পূর্ণসংখ্যা>(idx) = i + update;
                }
            }
        }
        অন্যথা
        {
            জন্য( পূর্ণসংখ্যা i = 0; i < tdist.rows; i++ )
            {
                পূর্ণসংখ্যা idx = tidx.at<পূর্ণসংখ্যা>(i);
                দশমিক d = tdist.at<দশমিক>(i), d0 = dist.at<দশমিক>(idx);
                যদি( d < d0 )
                {
                    dist.at<দশমিক>(idx) = d;
                    nidx.at<পূর্ণসংখ্যা>(idx) = i + update;
                }
            }
        }
        জন্য( পূর্ণসংখ্যা i = 0; i < sdist.rows; i++ )
        {
            যদি( tidx.at<পূর্ণসংখ্যা>(sidx.at<পূর্ণসংখ্যা>(i)) != i )
            {
                nidx.at<পূর্ণসংখ্যা>(i) = -1;
            }
        }
        ফেরত;
    }

    BatchDistFunc func = 0;
    যদি( type == CV_8U )
    {
        যদি( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, পূর্ণসংখ্যা>;
        অন্যথা যদি( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, দশমিক>;
        অন্যথা যদি( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, পূর্ণসংখ্যা>;
        অন্যথা যদি( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, দশমিক>;
        অন্যথা যদি( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, দশমিক>;
        অন্যথা যদি( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        অন্যথা যদি( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    অন্যথা যদি( type == CV_32F && dtype == CV_32F )
    {
        যদি( normType == NORM_L1 )
            func = batchDistL1_<দশমিক, দশমিক>;
        অন্যথা যদি( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<দশমিক, দশমিক>;
        অন্যথা যদি( normType == NORM_L2 )
            func = batchDistL2_<দশমিক, দশমিক>;
    }

    যদি( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.rows),
                  BatchDistInvoker(src1, src2, dist, nidx, K, mask, update, func));
}
