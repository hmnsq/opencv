// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"
#ಅನ್ತರ್ಭಾವ <opencv2/core/hal/hal.hpp>

ನಾಮಾಕಾಶ cv
{

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp, ಬಗೆ_ಹೆಸರು _Rt>
ಸ್ಥಿರ ಶೂನ್ಯ batchDistL1_(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, size_t step2,
                         ಪೂರ್ಣಾಂಕ nvecs, ಪೂರ್ಣಾಂಕ len, ಶೂನ್ಯ* _dist, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ)
{
    ಸ್ಥಿರಾಂಕ _Tp* src1 = (ಸ್ಥಿರಾಂಕ _Tp*)_src1;
    ಸ್ಥಿರಾಂಕ _Tp* src2 = (ಸ್ಥಿರಾಂಕ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ಗಾತ್ರ(src2[0]);
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = normL1<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = ಮುಖವಾಡ[i] ? normL1<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp, ಬಗೆ_ಹೆಸರು _Rt>
ಸ್ಥಿರ ಶೂನ್ಯ batchDistL2Sqr_(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, size_t step2,
                            ಪೂರ್ಣಾಂಕ nvecs, ಪೂರ್ಣಾಂಕ len, ಶೂನ್ಯ* _dist, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ)
{
    ಸ್ಥಿರಾಂಕ _Tp* src1 = (ಸ್ಥಿರಾಂಕ _Tp*)_src1;
    ಸ್ಥಿರಾಂಕ _Tp* src2 = (ಸ್ಥಿರಾಂಕ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ಗಾತ್ರ(src2[0]);
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = ಮುಖವಾಡ[i] ? normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len) : val0;
    }
}

ಮಾದರಿ<>
ಶೂನ್ಯ batchDistL2Sqr_<ದಶಮಾಂಶ, ದಶಮಾಂಶ>(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, size_t step2,
                                   ಪೂರ್ಣಾಂಕ nvecs, ಪೂರ್ಣಾಂಕ len, ಶೂನ್ಯ* _dist, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ)
{
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1 = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)_src1;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2 = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)_src2;
    ದಶಮಾಂಶ* dist = (ದಶಮಾಂಶ*)_dist;
    step2 /= ಗಾತ್ರ(src2[0]);
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = hal::normL2Sqr_(src1, src2 + step2*i, len);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ದಶಮಾಂಶ val0 = std::numeric_limits<ದಶಮಾಂಶ>::max();
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = ಮುಖವಾಡ[i] ? hal::normL2Sqr_(src1, src2 + step2*i, len) : val0;
    }
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು _Tp, ಬಗೆ_ಹೆಸರು _Rt>
ಸ್ಥಿರ ಶೂನ್ಯ batchDistL2_(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, size_t step2,
                         ಪೂರ್ಣಾಂಕ nvecs, ಪೂರ್ಣಾಂಕ len, ಶೂನ್ಯ* _dist, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ)
{
    ಸ್ಥಿರಾಂಕ _Tp* src1 = (ಸ್ಥಿರಾಂಕ _Tp*)_src1;
    ಸ್ಥಿರಾಂಕ _Tp* src2 = (ಸ್ಥಿರಾಂಕ _Tp*)_src2;
    _Rt* dist = (_Rt*)_dist;
    step2 /= ಗಾತ್ರ(src2[0]);
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = std::ವರ್ಗಮೂಲ(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len));
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        _Rt val0 = std::numeric_limits<_Rt>::max();
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = ಮುಖವಾಡ[i] ? std::ವರ್ಗಮೂಲ(normL2Sqr<_Tp, _Rt>(src1, src2 + step2*i, len)) : val0;
    }
}

ಮಾದರಿ<>
ಶೂನ್ಯ batchDistL2_<ದಶಮಾಂಶ, ದಶಮಾಂಶ>(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, size_t step2,
                                ಪೂರ್ಣಾಂಕ nvecs, ಪೂರ್ಣಾಂಕ len, ಶೂನ್ಯ* _dist, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ)
{
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src1 = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)_src1;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src2 = (ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)_src2;
    ದಶಮಾಂಶ* dist = (ದಶಮಾಂಶ*)_dist;
    step2 /= ಗಾತ್ರ(src2[0]);
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = std::ವರ್ಗಮೂಲ(hal::normL2Sqr_(src1, src2 + step2*i, len));
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ದಶಮಾಂಶ val0 = std::numeric_limits<ದಶಮಾಂಶ>::max();
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = ಮುಖವಾಡ[i] ? std::ವರ್ಗಮೂಲ(hal::normL2Sqr_(src1, src2 + step2*i, len)) : val0;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ batchDistHamming(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, size_t step2,
                             ಪೂರ್ಣಾಂಕ nvecs, ಪೂರ್ಣಾಂಕ len, ಶೂನ್ಯ* _dist, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ)
{
    ಸ್ಥಿರಾಂಕ uchar* src1 = (ಸ್ಥಿರಾಂಕ uchar*)_src1;
    ಸ್ಥಿರಾಂಕ uchar* src2 = (ಸ್ಥಿರಾಂಕ uchar*)_src2;
    ಪೂರ್ಣಾಂಕ* dist = (ಪೂರ್ಣಾಂಕ*)_dist;
    step2 /= ಗಾತ್ರ(src2[0]);
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
             dist[i] = hal::normHamming(src1, src2 + step2*i, len);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ val0 = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
        {
            ಆದರೆ (ಮುಖವಾಡ[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len);
            ಇಲ್ಲದಿದ್ದರೆ
                dist[i] = val0;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ batchDistHamming2(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* _src2, size_t step2,
                              ಪೂರ್ಣಾಂಕ nvecs, ಪೂರ್ಣಾಂಕ len, ಶೂನ್ಯ* _dist, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ)
{
    ಸ್ಥಿರಾಂಕ uchar* src1 = (ಸ್ಥಿರಾಂಕ uchar*)_src1;
    ಸ್ಥಿರಾಂಕ uchar* src2 = (ಸ್ಥಿರಾಂಕ uchar*)_src2;
    ಪೂರ್ಣಾಂಕ* dist = (ಪೂರ್ಣಾಂಕ*)_dist;
    step2 /= ಗಾತ್ರ(src2[0]);
    ಆದರೆ( !ಮುಖವಾಡ )
    {
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
            dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ val0 = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < nvecs; i++ )
        {
            ಆದರೆ (ಮುಖವಾಡ[i])
                dist[i] = hal::normHamming(src1, src2 + step2*i, len, 2);
            ಇಲ್ಲದಿದ್ದರೆ
                dist[i] = val0;
        }
    }
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*BatchDistFunc)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2,
                              ಪೂರ್ಣಾಂಕ nvecs, ಪೂರ್ಣಾಂಕ len, ಶೂನ್ಯ* dist, ಸ್ಥಿರಾಂಕ uchar* ಮುಖವಾಡ);


ರಚನೆ BatchDistInvoker : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
    BatchDistInvoker( ಸ್ಥಿರಾಂಕ Mat& _src1, ಸ್ಥಿರಾಂಕ Mat& _src2,
                      Mat& _dist, Mat& _nidx, ಪೂರ್ಣಾಂಕ _K,
                      ಸ್ಥಿರಾಂಕ Mat& ಮುಖವಾಡ_2, ಪೂರ್ಣಾಂಕ _update,
                      BatchDistFunc _func)
    {
        src1 = &_src1;
        src2 = &_src2;
        dist = &_dist;
        nidx = &_nidx;
        K = _K;
        ಮುಖವಾಡ = &ಮುಖವಾಡ_2;
        update = _update;
        func = _func;
    }

    ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Range& range) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ> buf(src2->ಅಡ್ಡಸಾಲುಗಳು);
        ಪೂರ್ಣಾಂಕ* bufptr = buf.ದತ್ತಾಂಶ();

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = range.ಆರಂಭ; i < range.end; i++ )
        {
            func(src1->ptr(i), src2->ptr(), src2->ಹೆಜ್ಜೆ, src2->ಅಡ್ಡಸಾಲುಗಳು, src2->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ,
                 K > 0 ? (uchar*)bufptr : dist->ptr(i), ಮುಖವಾಡ->ದತ್ತಾಂಶ ? ಮುಖವಾಡ->ptr(i) : 0);

            ಆದರೆ( K > 0 )
            {
                ಪೂರ್ಣಾಂಕ* nidxptr = nidx->ptr<ಪೂರ್ಣಾಂಕ>(i);
                // since positive float's can be compared just like int's,
                // we handle both CV_32S and CV_32F cases with a single branch
                ಪೂರ್ಣಾಂಕ* distptr = (ಪೂರ್ಣಾಂಕ*)dist->ptr(i);

                ಪೂರ್ಣಾಂಕ j, k;

                ಗಾಗಿ( j = 0; j < src2->ಅಡ್ಡಸಾಲುಗಳು; j++ )
                {
                    ಪೂರ್ಣಾಂಕ d = bufptr[j];
                    ಆದರೆ( d < distptr[K-1] )
                    {
                        ಗಾಗಿ( k = K-2; k >= 0 && distptr[k] > d; k-- )
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

    ಸ್ಥಿರಾಂಕ Mat *src1;
    ಸ್ಥಿರಾಂಕ Mat *src2;
    Mat *dist;
    Mat *nidx;
    ಸ್ಥಿರಾಂಕ Mat *ಮುಖವಾಡ;
    ಪೂರ್ಣಾಂಕ K;
    ಪೂರ್ಣಾಂಕ update;
    BatchDistFunc func;
};

}

ಶೂನ್ಯ cv::batch_ದೂರ( ಒಳಹರಿವು_Array _src1, ಒಳಹರಿವು_Array _src2,
                        OutputArray _dist, ಪೂರ್ಣಾಂಕ dtype, OutputArray _nidx,
                        ಪೂರ್ಣಾಂಕ normType, ಪೂರ್ಣಾಂಕ K, ಒಳಹರಿವು_Array ಮುಖವಾಡ_2,
                        ಪೂರ್ಣಾಂಕ update, ತಾರ್ಕಿಕ crosscheck )
{
    CV_INSTRUMENT_REGION();

    Mat src1 = _src1.getMat(), src2 = _src2.getMat(), ಮುಖವಾಡ = ಮುಖವಾಡ_2.getMat();
    ಪೂರ್ಣಾಂಕ type = src1.type();
    CV_Assert( type == src2.type() && src1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == src2.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ &&
               (type == CV_32F || type == CV_8U));
    CV_Assert( _nidx.needed() == (K > 0) );

    ಆದರೆ( dtype == -1 )
    {
        dtype = normType == NORM_HAMMING || normType == NORM_HAMMING2 ? CV_32S : CV_32F;
    }
    CV_Assert( (type == CV_8U && dtype == CV_32S) || dtype == CV_32F);

    K = std::min(K, src2.ಅಡ್ಡಸಾಲುಗಳು);

    _dist.create(src1.ಅಡ್ಡಸಾಲುಗಳು, (K > 0 ? K : src2.ಅಡ್ಡಸಾಲುಗಳು), dtype);
    Mat dist = _dist.getMat(), nidx;
    ಆದರೆ( _nidx.needed() )
    {
        _nidx.create(dist.size(), CV_32S);
        nidx = _nidx.getMat();
    }

    ಆದರೆ( update == 0 && K > 0 )
    {
        dist = Scalar::all(dtype == CV_32S ? (ದ್ವಿಗುಣ)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ : (ದ್ವಿಗುಣ)FLT_MAX);
        nidx = Scalar::all(-1);
    }


    ಆದರೆ( crosscheck )
    {
        CV_Assert( K == 1 && update == 0 && ಮುಖವಾಡ.empty() );
        CV_Assert(!nidx.empty());
        Mat tdist, tidx, sdist, sidx;
        batch_ದೂರ(src2, src1, tdist, dtype, tidx, normType, K, ಮುಖವಾಡ, 0, ಸುಳ್ಳು);
        batch_ದೂರ(src1, src2, sdist, dtype, sidx, normType, K, ಮುಖವಾಡ, 0, ಸುಳ್ಳು);

        // if an idx-th element from src1 appeared to be the nearest to i-th element of src2,
        // we update the minimum mutual distance between idx-th element of src1 and the whole src2 set.
        // As a result, if nidx[idx] = i*, it means that idx-th element of src1 is the nearest
        // to i*-th element of src2 and i*-th element of src2 is the closest to idx-th element of src1.
        // If nidx[idx] = -1, it means that there is no such ideal couple for it in src2.
        // This O(2N) procedure is called cross-check and it helps to eliminate some false matches.
        ಆದರೆ( dtype == CV_32S )
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < tdist.ಅಡ್ಡಸಾಲುಗಳು; i++ )
            {
                ಪೂರ್ಣಾಂಕ idx = tidx.at<ಪೂರ್ಣಾಂಕ>(i);
                ಪೂರ್ಣಾಂಕ d = tdist.at<ಪೂರ್ಣಾಂಕ>(i), d0 = dist.at<ಪೂರ್ಣಾಂಕ>(idx);
                ಆದರೆ( d < d0 )
                {
                    dist.at<ಪೂರ್ಣಾಂಕ>(idx) = d;
                    nidx.at<ಪೂರ್ಣಾಂಕ>(idx) = i + update;
                }
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < tdist.ಅಡ್ಡಸಾಲುಗಳು; i++ )
            {
                ಪೂರ್ಣಾಂಕ idx = tidx.at<ಪೂರ್ಣಾಂಕ>(i);
                ದಶಮಾಂಶ d = tdist.at<ದಶಮಾಂಶ>(i), d0 = dist.at<ದಶಮಾಂಶ>(idx);
                ಆದರೆ( d < d0 )
                {
                    dist.at<ದಶಮಾಂಶ>(idx) = d;
                    nidx.at<ಪೂರ್ಣಾಂಕ>(idx) = i + update;
                }
            }
        }
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < sdist.ಅಡ್ಡಸಾಲುಗಳು; i++ )
        {
            ಆದರೆ( tidx.at<ಪೂರ್ಣಾಂಕ>(sidx.at<ಪೂರ್ಣಾಂಕ>(i)) != i )
            {
                nidx.at<ಪೂರ್ಣಾಂಕ>(i) = -1;
            }
        }
        ಹಿಂತಿರುಗಿ;
    }

    BatchDistFunc func = 0;
    ಆದರೆ( type == CV_8U )
    {
        ಆದರೆ( normType == NORM_L1 && dtype == CV_32S )
            func = batchDistL1_<uchar, ಪೂರ್ಣಾಂಕ>;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_L1 && dtype == CV_32F )
            func = batchDistL1_<uchar, ದಶಮಾಂಶ>;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_L2SQR && dtype == CV_32S )
            func = batchDistL2Sqr_<uchar, ಪೂರ್ಣಾಂಕ>;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_L2SQR && dtype == CV_32F )
            func = batchDistL2Sqr_<uchar, ದಶಮಾಂಶ>;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_L2 && dtype == CV_32F )
            func = batchDistL2_<uchar, ದಶಮಾಂಶ>;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_HAMMING && dtype == CV_32S )
            func = batchDistHamming;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_HAMMING2 && dtype == CV_32S )
            func = batchDistHamming2;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( type == CV_32F && dtype == CV_32F )
    {
        ಆದರೆ( normType == NORM_L1 )
            func = batchDistL1_<ದಶಮಾಂಶ, ದಶಮಾಂಶ>;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_L2SQR )
            func = batchDistL2Sqr_<ದಶಮಾಂಶ, ದಶಮಾಂಶ>;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( normType == NORM_L2 )
            func = batchDistL2_<ದಶಮಾಂಶ, ದಶಮಾಂಶ>;
    }

    ಆದರೆ( func == 0 )
        CV_Error_(cv::Error::StsUnsupportedFormat,
                  ("The combination of type=%d, dtype=%d and normType=%d is not supported",
                   type, dtype, normType));

    parallel_for_(Range(0, src1.ಅಡ್ಡಸಾಲುಗಳು),
                  BatchDistInvoker(src1, src2, dist, nidx, K, ಮುಖವಾಡ, update, func));
}
