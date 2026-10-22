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

#ضمن "precomp.hpp"
#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

نطاق cv
{

ساكن صحيح CV_KMEANS_PARALLEL_GRANULARITY = (صحيح)utils::get_Configuration_Parameter_اندازه_T("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

ساكن فراغ generateRandomCenter(صحيح dims, ثابت Vec2f* box, عائم* center, RNG& rng)
{
    عائم margin = 1.f/dims;
    لكل (صحيح j = 0; j < dims; j++)
        center[j] = ((عائم)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

صنف KMeansPPDistanceComputer : عام ParallelLoopBody
{
عام:
    KMeansPPDistanceComputer(عائم *tdist2_, ثابت Mat& معلومات_2, ثابت عائم *dist_, صحيح ci_) :
        tdist2(tdist2_), معلومات(معلومات_2), dist(dist_), ci(ci_)
    { }

    فراغ چلوونکی()( ثابت cv::Range& range ) ثابت CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ثابت صحيح begin = range.پیل;
        ثابت صحيح end = range.end;
        ثابت صحيح dims = معلومات.ستون_شمېر;

        لكل (صحيح i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(معلومات.ptr<عائم>(i), معلومات.ptr<عائم>(ci), dims), dist[i]);
        }
    }

خاص:
    KMeansPPDistanceComputer& چلوونکی=(ثابت KMeansPPDistanceComputer&); // = delete

    عائم *tdist2;
    ثابت Mat& معلومات;
    ثابت عائم *dist;
    ثابت صحيح ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
ساكن منطقي generateCentersPP(ثابت Mat& معلومات, Mat& بهر_centers,
                              صحيح K, RNG& rng, صحيح trials)
{
    CV_TRACE_FUNCTION();
    ثابت صحيح dims = معلومات.ستون_شمېر, N = معلومات.لیکې;
    cv::Auto_بفر<صحيح, 64> _centers(K);
    صحيح* centers = &_centers[0];
    cv::Auto_بفر<عائم, 0> _dist(N*3);
    عائم* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    مزدوج sum0 = 0;

    centers[0] = (بینښه)rng % N;

    لكل (صحيح i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(معلومات.ptr<عائم>(i), معلومات.ptr<عائم>(centers[0]), dims);
        sum0 += dist[i];
    }
    إذا (!(sum0 < DBL_MAX))
        أرجع خطأ;

    لكل (صحيح k = 1; k < K; k++)
    {
        مزدوج غوره_مجموعه = DBL_MAX;
        صحيح غوره_Center = -1;

        لكل (صحيح j = 0; j < trials; j++)
        {
            مزدوج p = (مزدوج)rng*sum0;
            صحيح ci = 0;
            لكل (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                إذا (p <= 0)
                    اكسر;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, معلومات, dist, ci),
                          (مزدوج)div_پورته((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            مزدوج s = 0;
            صحيح i = 0;
        #إذا CV_ENABLE_UNROLLED
            لكل (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #د_که_پای
            لكل (; i < N; i++)
            {
                s += tdist2[i];
            }

            إذا (s < غوره_مجموعه)
            {
                غوره_مجموعه = s;
                غوره_Center = ci;
                std::swap(tdist, tdist2);
            }
        }
        إذا (غوره_Center < 0)
            أرجع خطأ;
        centers[k] = غوره_Center;
        sum0 = غوره_مجموعه;
        std::swap(dist, tdist);
    }

    لكل (صحيح k = 0; k < K; k++)
    {
        ثابت عائم* src = معلومات.ptr<عائم>(centers[k]);
        عائم* dst = بهر_centers.ptr<عائم>(k);
        لكل (صحيح j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    أرجع ریښتیا;
}

کينډۍ<منطقي only_واټن>
صنف KMeans_واټن_Computer : عام ParallelLoopBody
{
عام:
    KMeans_واټن_Computer( مزدوج *distances_,
                            صحيح *labels_,
                            ثابت Mat& معلومات_2,
                            ثابت Mat& centers_)
        : distances(distances_),
          labels(labels_),
          معلومات(معلومات_2),
          centers(centers_)
    {
    }

    فراغ چلوونکی()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ثابت صحيح begin = range.پیل;
        ثابت صحيح end = range.end;
        ثابت صحيح K = centers.لیکې;
        ثابت صحيح dims = centers.ستون_شمېر;

        لكل (صحيح i = begin; i < end; ++i)
        {
            ثابت عائم *sample = معلومات.ptr<عائم>(i);
            إذا (only_واټن)
            {
                ثابت عائم* center = centers.ptr<عائم>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                استمر;
            }
            وإلا
            {
                صحيح k_غوره = 0;
                مزدوج min_dist = DBL_MAX;

                لكل (صحيح k = 0; k < K; k++)
                {
                    ثابت عائم* center = centers.ptr<عائم>(k);
                    ثابت مزدوج dist = hal::normL2Sqr_(sample, center, dims);
                    إذا (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        اكسر;
                    }

                    إذا (min_dist > dist)
                    {
                        min_dist = dist;
                        k_غوره = k;
                    }
                }

                distances[i] = min_dist;
                labels[i] = k_غوره;
            }
        }
    }

خاص:
    KMeans_واټن_Computer& چلوونکی=(ثابت KMeans_واټن_Computer&); // = delete

    مزدوج *distances;
    صحيح *labels;
    ثابت Mat& معلومات;
    ثابت Mat& centers;
};

}

مزدوج cv::kmeans( ننوت_Array معلومات_3, صحيح K,
                   ننوت_Output_Array غوره_Labels,
                   غړی_Criteria criteria, صحيح attempts,
                   صحيح flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ثابت صحيح SPP_TRIALS = 3;
    Mat data0 = معلومات_3.getMat();
    ثابت منطقي isrow = data0.لیکې == 1;
    ثابت صحيح N = isrow ? data0.ستون_شمېر : data0.لیکې;
    ثابت صحيح dims = (isrow ? 1 : data0.ستون_شمېر)*data0.channels();
    ثابت صحيح type = data0.ژوروالی();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat معلومات(N, dims, CV_32F, data0.ptr(), isrow ? dims * حجم(عائم) : static_cast<size_t>(data0.ګام));

    غوره_Labels.create(N, 1, CV_32S, -1, ریښتیا);

    Mat _labels, غوره_labels = غوره_Labels.getMat();
    إذا (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (غوره_labels.ستون_شمېر == 1 || غوره_labels.لیکې == 1) &&
                  غوره_labels.ستون_شمېر*غوره_labels.لیکې == N &&
                  غوره_labels.type() == CV_32S &&
                  غوره_labels.isContinuous());
        غوره_labels.reshape(1, N).copy_منزل(_labels);
        لكل (صحيح i = 0; i < N; i++)
        {
            CV_Assert((بینښه)_labels.at<صحيح>(i) < (بینښه)K);
        }
    }
    وإلا
    {
        إذا (!((غوره_labels.ستون_شمېر == 1 || غوره_labels.لیکې == 1) &&
             غوره_labels.ستون_شمېر*غوره_labels.لیکې == N &&
             غوره_labels.type() == CV_32S &&
             غوره_labels.isContinuous()))
        {
            غوره_Labels.create(N, 1, CV_32S);
            غوره_labels = غوره_Labels.getMat();
        }
        _labels.create(غوره_labels.size(), غوره_labels.type());
    }
    صحيح* labels = _labels.ptr<صحيح>();

    Mat centers(K, dims, type), old_centers(K, dims, type), لنډمهاله_2(1, dims, type);
    cv::Auto_بفر<صحيح, 64> counters(K);
    cv::Auto_بفر<مزدوج, 64> dists(N);
    RNG& rng = theRNG();

    إذا (criteria.type & غړی_Criteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    وإلا
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    إذا (criteria.type & غړی_Criteria::شمېر_3)
        criteria.max_شمېر = std::min(std::max(criteria.max_شمېر, 2), 100);
    وإلا
        criteria.max_شمېر = 100;

    إذا (K == 1)
    {
        attempts = 1;
        criteria.max_شمېر = 2;
    }

    cv::Auto_بفر<Vec2f, 64> box(dims);
    إذا (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ثابت عائم* sample = معلومات.ptr<عائم>(0);
            لكل (صحيح j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        لكل (صحيح i = 1; i < N; i++)
        {
            ثابت عائم* sample = معلومات.ptr<عائم>(i);
            لكل (صحيح j = 0; j < dims; j++)
            {
                عائم v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    مزدوج غوره_compactness = DBL_MAX;
    لكل (صحيح a = 0; a < attempts; a++)
    {
        مزدوج compactness = DBL_MAX;

        لكل (صحيح iter = 0; ;)
        {
            مزدوج max_center_ښويدنه = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            إذا (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                إذا (flags & KMEANS_PP_CENTERS)
                {
                    إذا (!generateCentersPP(معلومات, centers, K, rng, SPP_TRIALS))
                        اكسر;
                }
                وإلا
                {
                    لكل (صحيح k = 0; k < K; k++)
                        generateRandomCenter(dims, box.معلومات(), centers.ptr<عائم>(k), rng);
                }
            }
            وإلا
            {
                // compute centers
                centers = Scalar(0);
                لكل (صحيح k = 0; k < K; k++)
                    counters[k] = 0;

                لكل (صحيح i = 0; i < N; i++)
                {
                    ثابت عائم* sample = معلومات.ptr<عائم>(i);
                    صحيح k = labels[i];
                    عائم* center = centers.ptr<عائم>(k);
                    لكل (صحيح j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                لكل (صحيح k = 0; k < K; k++)
                {
                    إذا (counters[k] != 0)
                        استمر;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    صحيح max_k = 0;
                    لكل (صحيح k1 = 1; k1 < K; k1++)
                    {
                        إذا (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    مزدوج max_dist = 0;
                    صحيح farthest_i = -1;
                    عائم* بنسټ_center = centers.ptr<عائم>(max_k);
                    عائم* بنسټ_center_2 = لنډمهاله_2.ptr<عائم>(); // normalized
                    عائم scale = 1.f/counters[max_k];
                    لكل (صحيح j = 0; j < dims; j++)
                        بنسټ_center_2[j] = بنسټ_center[j]*scale;

                    لكل (صحيح i = 0; i < N; i++)
                    {
                        إذا (labels[i] != max_k)
                            استمر;
                        إذا (farthest_i < 0)
                            farthest_i = i;
                        ثابت عائم* sample = معلومات.ptr<عائم>(i);
                        مزدوج dist = hal::normL2Sqr_(sample, بنسټ_center_2, dims);

                        إذا (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ثابت عائم* sample = معلومات.ptr<عائم>(farthest_i);
                    عائم* cur_center = centers.ptr<عائم>(k);
                    لكل (صحيح j = 0; j < dims; j++)
                    {
                        بنسټ_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                لكل (صحيح k = 0; k < K; k++)
                {
                    عائم* center = centers.ptr<عائم>(k);
                    CV_Assert( counters[k] != 0 );

                    عائم scale = 1.f/counters[k];
                    لكل (صحيح j = 0; j < dims; j++)
                        center[j] *= scale;

                    إذا (iter > 0)
                    {
                        مزدوج dist = 0;
                        ثابت عائم* old_center = old_centers.ptr<عائم>(k);
                        لكل (صحيح j = 0; j < dims; j++)
                        {
                            مزدوج t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_ښويدنه = std::max(max_center_ښويدنه, dist);
                    }
                }
            }

            منطقي is_وروستی_Iter = (++iter == MAX(criteria.max_شمېر, 2) || max_center_ښويدنه <= criteria.epsilon);

            إذا (is_وروستی_Iter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeans_واټن_Computer<ریښتیا>(dists.معلومات(), labels, معلومات, centers), (مزدوج)div_پورته((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = مجموعه(Mat(اندازه(N, 1), CV_64F, &dists[0]))[0];
                اكسر;
            }
            وإلا
            {
                // assign labels
                parallel_for_(Range(0, N), KMeans_واټن_Computer<خطأ>(dists.معلومات(), labels, معلومات, centers), (مزدوج)div_پورته((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                إذا (!(مجموعه(Mat(اندازه(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    اكسر;
            }
        }

        إذا (!(compactness < DBL_MAX))
        {
            centers.سېټ_Zero();
            _labels.سېټ_Zero();
            compactness = -1.;
        }
        إذا (compactness < غوره_compactness)
        {
            غوره_compactness = compactness;
            إذا (_centers.needed())
            {
                إذا (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copy_منزل(_centers);
                وإلا
                    centers.copy_منزل(_centers);
            }
            _labels.copy_منزل(غوره_labels);
        }
        إذا (compactness < 0.)
            اكسر;
    }

    أرجع غوره_compactness;
}
