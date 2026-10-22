/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2018, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#شامل "precomp.hpp"
#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

نامخلا cv
{

ساکن صحیح CV_KMEANS_PARALLEL_GRANULARITY = (صحیح)utils::get_Configuration_Parameter_سائز_T("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

ساکن خالی generateRandomCenter(صحیح dims, ثابت Vec2f* box, اعشاری* center, RNG& rng)
{
    اعشاری margin = 1.f/dims;
    برائے (صحیح j = 0; j < dims; j++)
        center[j] = ((اعشاری)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

جماعت KMeansPPDistanceComputer : عوامی ParallelLoopBody
{
عوامی:
    KMeansPPDistanceComputer(اعشاری *tdist2_, ثابت Mat& ڈیٹا_2, ثابت اعشاری *dist_, صحیح ci_) :
        tdist2(tdist2_), ڈیٹا(ڈیٹا_2), dist(dist_), ci(ci_)
    { }

    خالی عامل()( ثابت cv::Range& range ) ثابت CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ثابت صحیح begin = range.آغاز;
        ثابت صحیح end = range.end;
        ثابت صحیح dims = ڈیٹا.ستون_تعداد;

        برائے (صحیح i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(ڈیٹا.ptr<اعشاری>(i), ڈیٹا.ptr<اعشاری>(ci), dims), dist[i]);
        }
    }

نجی:
    KMeansPPDistanceComputer& عامل=(ثابت KMeansPPDistanceComputer&); // = delete

    اعشاری *tdist2;
    ثابت Mat& ڈیٹا;
    ثابت اعشاری *dist;
    ثابت صحیح ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
ساکن منطقی generateCentersPP(ثابت Mat& ڈیٹا, Mat& برآمد_centers,
                              صحیح K, RNG& rng, صحیح trials)
{
    CV_TRACE_FUNCTION();
    ثابت صحیح dims = ڈیٹا.ستون_تعداد, N = ڈیٹا.صفیں;
    cv::Auto_بفر<صحیح, 64> _centers(K);
    صحیح* centers = &_centers[0];
    cv::Auto_بفر<اعشاری, 0> _dist(N*3);
    اعشاری* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    دوہرا sum0 = 0;

    centers[0] = (غیرعلامتی)rng % N;

    برائے (صحیح i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(ڈیٹا.ptr<اعشاری>(i), ڈیٹا.ptr<اعشاری>(centers[0]), dims);
        sum0 += dist[i];
    }
    اگر (!(sum0 < DBL_MAX))
        واپس جھوٹ;

    برائے (صحیح k = 1; k < K; k++)
    {
        دوہرا بہترین_مجموع = DBL_MAX;
        صحیح بہترین_Center = -1;

        برائے (صحیح j = 0; j < trials; j++)
        {
            دوہرا p = (دوہرا)rng*sum0;
            صحیح ci = 0;
            برائے (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                اگر (p <= 0)
                    توڑو;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, ڈیٹا, dist, ci),
                          (دوہرا)div_اوپر((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            دوہرا s = 0;
            صحیح i = 0;
        #اگر CV_ENABLE_UNROLLED
            برائے (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #اگر_ختم
            برائے (; i < N; i++)
            {
                s += tdist2[i];
            }

            اگر (s < بہترین_مجموع)
            {
                بہترین_مجموع = s;
                بہترین_Center = ci;
                std::swap(tdist, tdist2);
            }
        }
        اگر (بہترین_Center < 0)
            واپس جھوٹ;
        centers[k] = بہترین_Center;
        sum0 = بہترین_مجموع;
        std::swap(dist, tdist);
    }

    برائے (صحیح k = 0; k < K; k++)
    {
        ثابت اعشاری* src = ڈیٹا.ptr<اعشاری>(centers[k]);
        اعشاری* dst = برآمد_centers.ptr<اعشاری>(k);
        برائے (صحیح j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    واپس سچ;
}

سانچہ<منطقی only_فاصلہ>
جماعت KMeans_فاصلہ_Computer : عوامی ParallelLoopBody
{
عوامی:
    KMeans_فاصلہ_Computer( دوہرا *distances_,
                            صحیح *labels_,
                            ثابت Mat& ڈیٹا_2,
                            ثابت Mat& centers_)
        : distances(distances_),
          labels(labels_),
          ڈیٹا(ڈیٹا_2),
          centers(centers_)
    {
    }

    خالی عامل()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ثابت صحیح begin = range.آغاز;
        ثابت صحیح end = range.end;
        ثابت صحیح K = centers.صفیں;
        ثابت صحیح dims = centers.ستون_تعداد;

        برائے (صحیح i = begin; i < end; ++i)
        {
            ثابت اعشاری *sample = ڈیٹا.ptr<اعشاری>(i);
            اگر (only_فاصلہ)
            {
                ثابت اعشاری* center = centers.ptr<اعشاری>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                جاری;
            }
            ورنہ
            {
                صحیح k_بہترین = 0;
                دوہرا min_dist = DBL_MAX;

                برائے (صحیح k = 0; k < K; k++)
                {
                    ثابت اعشاری* center = centers.ptr<اعشاری>(k);
                    ثابت دوہرا dist = hal::normL2Sqr_(sample, center, dims);
                    اگر (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        توڑو;
                    }

                    اگر (min_dist > dist)
                    {
                        min_dist = dist;
                        k_بہترین = k;
                    }
                }

                distances[i] = min_dist;
                labels[i] = k_بہترین;
            }
        }
    }

نجی:
    KMeans_فاصلہ_Computer& عامل=(ثابت KMeans_فاصلہ_Computer&); // = delete

    دوہرا *distances;
    صحیح *labels;
    ثابت Mat& ڈیٹا;
    ثابت Mat& centers;
};

}

دوہرا cv::kmeans( داخلہ_Array ڈیٹا_3, صحیح K,
                   داخلہ_Output_Array بہترین_Labels,
                   رکن_Criteria criteria, صحیح attempts,
                   صحیح flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ثابت صحیح SPP_TRIALS = 3;
    Mat data0 = ڈیٹا_3.getMat();
    ثابت منطقی isrow = data0.صفیں == 1;
    ثابت صحیح N = isrow ? data0.ستون_تعداد : data0.صفیں;
    ثابت صحیح dims = (isrow ? 1 : data0.ستون_تعداد)*data0.channels();
    ثابت صحیح type = data0.گہرائی();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat ڈیٹا(N, dims, CV_32F, data0.ptr(), isrow ? dims * حجم(اعشاری) : static_cast<size_t>(data0.قدم));

    بہترین_Labels.create(N, 1, CV_32S, -1, سچ);

    Mat _labels, بہترین_labels = بہترین_Labels.getMat();
    اگر (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (بہترین_labels.ستون_تعداد == 1 || بہترین_labels.صفیں == 1) &&
                  بہترین_labels.ستون_تعداد*بہترین_labels.صفیں == N &&
                  بہترین_labels.type() == CV_32S &&
                  بہترین_labels.isContinuous());
        بہترین_labels.reshape(1, N).copy_مقصد(_labels);
        برائے (صحیح i = 0; i < N; i++)
        {
            CV_Assert((غیرعلامتی)_labels.at<صحیح>(i) < (غیرعلامتی)K);
        }
    }
    ورنہ
    {
        اگر (!((بہترین_labels.ستون_تعداد == 1 || بہترین_labels.صفیں == 1) &&
             بہترین_labels.ستون_تعداد*بہترین_labels.صفیں == N &&
             بہترین_labels.type() == CV_32S &&
             بہترین_labels.isContinuous()))
        {
            بہترین_Labels.create(N, 1, CV_32S);
            بہترین_labels = بہترین_Labels.getMat();
        }
        _labels.create(بہترین_labels.size(), بہترین_labels.type());
    }
    صحیح* labels = _labels.ptr<صحیح>();

    Mat centers(K, dims, type), old_centers(K, dims, type), عارضی_2(1, dims, type);
    cv::Auto_بفر<صحیح, 64> counters(K);
    cv::Auto_بفر<دوہرا, 64> dists(N);
    RNG& rng = theRNG();

    اگر (criteria.type & رکن_Criteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    ورنہ
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    اگر (criteria.type & رکن_Criteria::گنتی_3)
        criteria.max_گنتی = std::min(std::max(criteria.max_گنتی, 2), 100);
    ورنہ
        criteria.max_گنتی = 100;

    اگر (K == 1)
    {
        attempts = 1;
        criteria.max_گنتی = 2;
    }

    cv::Auto_بفر<Vec2f, 64> box(dims);
    اگر (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ثابت اعشاری* sample = ڈیٹا.ptr<اعشاری>(0);
            برائے (صحیح j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        برائے (صحیح i = 1; i < N; i++)
        {
            ثابت اعشاری* sample = ڈیٹا.ptr<اعشاری>(i);
            برائے (صحیح j = 0; j < dims; j++)
            {
                اعشاری v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    دوہرا بہترین_compactness = DBL_MAX;
    برائے (صحیح a = 0; a < attempts; a++)
    {
        دوہرا compactness = DBL_MAX;

        برائے (صحیح iter = 0; ;)
        {
            دوہرا max_center_سرکاؤ = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            اگر (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                اگر (flags & KMEANS_PP_CENTERS)
                {
                    اگر (!generateCentersPP(ڈیٹا, centers, K, rng, SPP_TRIALS))
                        توڑو;
                }
                ورنہ
                {
                    برائے (صحیح k = 0; k < K; k++)
                        generateRandomCenter(dims, box.ڈیٹا(), centers.ptr<اعشاری>(k), rng);
                }
            }
            ورنہ
            {
                // compute centers
                centers = Scalar(0);
                برائے (صحیح k = 0; k < K; k++)
                    counters[k] = 0;

                برائے (صحیح i = 0; i < N; i++)
                {
                    ثابت اعشاری* sample = ڈیٹا.ptr<اعشاری>(i);
                    صحیح k = labels[i];
                    اعشاری* center = centers.ptr<اعشاری>(k);
                    برائے (صحیح j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                برائے (صحیح k = 0; k < K; k++)
                {
                    اگر (counters[k] != 0)
                        جاری;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    صحیح max_k = 0;
                    برائے (صحیح k1 = 1; k1 < K; k1++)
                    {
                        اگر (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    دوہرا max_dist = 0;
                    صحیح farthest_i = -1;
                    اعشاری* بنیاد_center = centers.ptr<اعشاری>(max_k);
                    اعشاری* بنیاد_center_2 = عارضی_2.ptr<اعشاری>(); // normalized
                    اعشاری scale = 1.f/counters[max_k];
                    برائے (صحیح j = 0; j < dims; j++)
                        بنیاد_center_2[j] = بنیاد_center[j]*scale;

                    برائے (صحیح i = 0; i < N; i++)
                    {
                        اگر (labels[i] != max_k)
                            جاری;
                        اگر (farthest_i < 0)
                            farthest_i = i;
                        ثابت اعشاری* sample = ڈیٹا.ptr<اعشاری>(i);
                        دوہرا dist = hal::normL2Sqr_(sample, بنیاد_center_2, dims);

                        اگر (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ثابت اعشاری* sample = ڈیٹا.ptr<اعشاری>(farthest_i);
                    اعشاری* cur_center = centers.ptr<اعشاری>(k);
                    برائے (صحیح j = 0; j < dims; j++)
                    {
                        بنیاد_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                برائے (صحیح k = 0; k < K; k++)
                {
                    اعشاری* center = centers.ptr<اعشاری>(k);
                    CV_Assert( counters[k] != 0 );

                    اعشاری scale = 1.f/counters[k];
                    برائے (صحیح j = 0; j < dims; j++)
                        center[j] *= scale;

                    اگر (iter > 0)
                    {
                        دوہرا dist = 0;
                        ثابت اعشاری* old_center = old_centers.ptr<اعشاری>(k);
                        برائے (صحیح j = 0; j < dims; j++)
                        {
                            دوہرا t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_سرکاؤ = std::max(max_center_سرکاؤ, dist);
                    }
                }
            }

            منطقی is_آخری_Iter = (++iter == MAX(criteria.max_گنتی, 2) || max_center_سرکاؤ <= criteria.epsilon);

            اگر (is_آخری_Iter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeans_فاصلہ_Computer<سچ>(dists.ڈیٹا(), labels, ڈیٹا, centers), (دوہرا)div_اوپر((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = مجموع(Mat(سائز(N, 1), CV_64F, &dists[0]))[0];
                توڑو;
            }
            ورنہ
            {
                // assign labels
                parallel_for_(Range(0, N), KMeans_فاصلہ_Computer<جھوٹ>(dists.ڈیٹا(), labels, ڈیٹا, centers), (دوہرا)div_اوپر((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                اگر (!(مجموع(Mat(سائز(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    توڑو;
            }
        }

        اگر (!(compactness < DBL_MAX))
        {
            centers.سیٹ_Zero();
            _labels.سیٹ_Zero();
            compactness = -1.;
        }
        اگر (compactness < بہترین_compactness)
        {
            بہترین_compactness = compactness;
            اگر (_centers.needed())
            {
                اگر (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copy_مقصد(_centers);
                ورنہ
                    centers.copy_مقصد(_centers);
            }
            _labels.copy_مقصد(بہترین_labels);
        }
        اگر (compactness < 0.)
            توڑو;
    }

    واپس بہترین_compactness;
}
