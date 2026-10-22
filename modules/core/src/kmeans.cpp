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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

ਨਾਮਾਕਾਸ਼ cv
{

ਸਥਿਰ ਪੂਰਨ CV_KMEANS_PARALLEL_GRANULARITY = (ਪੂਰਨ)utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

ਸਥਿਰ ਖਾਲੀ generateRandomCenter(ਪੂਰਨ dims, ਅਚਲ Vec2f* box, ਦਸ਼ਮਲਵ* center, RNG& rng)
{
    ਦਸ਼ਮਲਵ margin = 1.f/dims;
    ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
        center[j] = ((ਦਸ਼ਮਲਵ)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

ਵਰਗ KMeansPPDistanceComputer : ਜਨਤਕ ParallelLoopBody
{
ਜਨਤਕ:
    KMeansPPDistanceComputer(ਦਸ਼ਮਲਵ *tdist2_, ਅਚਲ Mat& ਅੰਕੜੇ_2, ਅਚਲ ਦਸ਼ਮਲਵ *dist_, ਪੂਰਨ ci_) :
        tdist2(tdist2_), ਅੰਕੜੇ(ਅੰਕੜੇ_2), dist(dist_), ci(ci_)
    { }

    ਖਾਲੀ ਸੰਚਾਲਕ()( ਅਚਲ cv::Range& range ) ਅਚਲ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ਅਚਲ ਪੂਰਨ begin = range.ਸ਼ੁਰੂ;
        ਅਚਲ ਪੂਰਨ end = range.end;
        ਅਚਲ ਪੂਰਨ dims = ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ;

        ਲਈ (ਪੂਰਨ i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(i), ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(ci), dims), dist[i]);
        }
    }

ਨਿਜੀ:
    KMeansPPDistanceComputer& ਸੰਚਾਲਕ=(ਅਚਲ KMeansPPDistanceComputer&); // = delete

    ਦਸ਼ਮਲਵ *tdist2;
    ਅਚਲ Mat& ਅੰਕੜੇ;
    ਅਚਲ ਦਸ਼ਮਲਵ *dist;
    ਅਚਲ ਪੂਰਨ ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
ਸਥਿਰ ਤਾਰਕਿਕ generateCentersPP(ਅਚਲ Mat& ਅੰਕੜੇ, Mat& ਬਾਹਰਲਾ_centers,
                              ਪੂਰਨ K, RNG& rng, ਪੂਰਨ trials)
{
    CV_TRACE_FUNCTION();
    ਅਚਲ ਪੂਰਨ dims = ਅੰਕੜੇ.ਖਾਨੇ_ਗਿਣਤੀ, N = ਅੰਕੜੇ.ਪੰਕਤੀਆਂ;
    cv::Auto_ਬਫਰ<ਪੂਰਨ, 64> _centers(K);
    ਪੂਰਨ* centers = &_centers[0];
    cv::Auto_ਬਫਰ<ਦਸ਼ਮਲਵ, 0> _dist(N*3);
    ਦਸ਼ਮਲਵ* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    ਦੁੱਗਣਾ sum0 = 0;

    centers[0] = (ਬਿਨਾਨਿਸ਼ਾਨੀ)rng % N;

    ਲਈ (ਪੂਰਨ i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(i), ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(centers[0]), dims);
        sum0 += dist[i];
    }
    ਜੇ (!(sum0 < DBL_MAX))
        ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਪੂਰਨ k = 1; k < K; k++)
    {
        ਦੁੱਗਣਾ ਉੱਤਮ_ਜੋੜ = DBL_MAX;
        ਪੂਰਨ ਉੱਤਮ_Center = -1;

        ਲਈ (ਪੂਰਨ j = 0; j < trials; j++)
        {
            ਦੁੱਗਣਾ p = (ਦੁੱਗਣਾ)rng*sum0;
            ਪੂਰਨ ci = 0;
            ਲਈ (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                ਜੇ (p <= 0)
                    ਰੋਕੋ;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, ਅੰਕੜੇ, dist, ci),
                          (ਦੁੱਗਣਾ)div_ਉੱਪਰ((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            ਦੁੱਗਣਾ s = 0;
            ਪੂਰਨ i = 0;
        #ਜੇ CV_ENABLE_UNROLLED
            ਲਈ (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #ਜੇ_ਅੰਤ
            ਲਈ (; i < N; i++)
            {
                s += tdist2[i];
            }

            ਜੇ (s < ਉੱਤਮ_ਜੋੜ)
            {
                ਉੱਤਮ_ਜੋੜ = s;
                ਉੱਤਮ_Center = ci;
                std::swap(tdist, tdist2);
            }
        }
        ਜੇ (ਉੱਤਮ_Center < 0)
            ਵਾਪਸ ਝੂਠ;
        centers[k] = ਉੱਤਮ_Center;
        sum0 = ਉੱਤਮ_ਜੋੜ;
        std::swap(dist, tdist);
    }

    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
    {
        ਅਚਲ ਦਸ਼ਮਲਵ* src = ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(centers[k]);
        ਦਸ਼ਮਲਵ* dst = ਬਾਹਰਲਾ_centers.ptr<ਦਸ਼ਮਲਵ>(k);
        ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    ਵਾਪਸ ਸੱਚ;
}

ਸਾਂਚਾ<ਤਾਰਕਿਕ only_ਦੂਰੀ>
ਵਰਗ KMeans_ਦੂਰੀ_Computer : ਜਨਤਕ ParallelLoopBody
{
ਜਨਤਕ:
    KMeans_ਦੂਰੀ_Computer( ਦੁੱਗਣਾ *distances_,
                            ਪੂਰਨ *labels_,
                            ਅਚਲ Mat& ਅੰਕੜੇ_2,
                            ਅਚਲ Mat& centers_)
        : distances(distances_),
          labels(labels_),
          ਅੰਕੜੇ(ਅੰਕੜੇ_2),
          centers(centers_)
    {
    }

    ਖਾਲੀ ਸੰਚਾਲਕ()(ਅਚਲ Range& range) ਅਚਲ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ਅਚਲ ਪੂਰਨ begin = range.ਸ਼ੁਰੂ;
        ਅਚਲ ਪੂਰਨ end = range.end;
        ਅਚਲ ਪੂਰਨ K = centers.ਪੰਕਤੀਆਂ;
        ਅਚਲ ਪੂਰਨ dims = centers.ਖਾਨੇ_ਗਿਣਤੀ;

        ਲਈ (ਪੂਰਨ i = begin; i < end; ++i)
        {
            ਅਚਲ ਦਸ਼ਮਲਵ *sample = ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(i);
            ਜੇ (only_ਦੂਰੀ)
            {
                ਅਚਲ ਦਸ਼ਮਲਵ* center = centers.ptr<ਦਸ਼ਮਲਵ>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                ਜਾਰੀ;
            }
            ਨਹੀਂਤਾਂ
            {
                ਪੂਰਨ k_ਉੱਤਮ = 0;
                ਦੁੱਗਣਾ min_dist = DBL_MAX;

                ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
                {
                    ਅਚਲ ਦਸ਼ਮਲਵ* center = centers.ptr<ਦਸ਼ਮਲਵ>(k);
                    ਅਚਲ ਦੁੱਗਣਾ dist = hal::normL2Sqr_(sample, center, dims);
                    ਜੇ (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        ਰੋਕੋ;
                    }

                    ਜੇ (min_dist > dist)
                    {
                        min_dist = dist;
                        k_ਉੱਤਮ = k;
                    }
                }

                distances[i] = min_dist;
                labels[i] = k_ਉੱਤਮ;
            }
        }
    }

ਨਿਜੀ:
    KMeans_ਦੂਰੀ_Computer& ਸੰਚਾਲਕ=(ਅਚਲ KMeans_ਦੂਰੀ_Computer&); // = delete

    ਦੁੱਗਣਾ *distances;
    ਪੂਰਨ *labels;
    ਅਚਲ Mat& ਅੰਕੜੇ;
    ਅਚਲ Mat& centers;
};

}

ਦੁੱਗਣਾ cv::kmeans( ਨਿਵੇਸ਼_Array ਅੰਕੜੇ_3, ਪੂਰਨ K,
                   ਨਿਵੇਸ਼_Output_Array ਉੱਤਮ_Labels,
                   ਪਦ_Criteria criteria, ਪੂਰਨ attempts,
                   ਪੂਰਨ flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ਅਚਲ ਪੂਰਨ SPP_TRIALS = 3;
    Mat data0 = ਅੰਕੜੇ_3.getMat();
    ਅਚਲ ਤਾਰਕਿਕ isrow = data0.ਪੰਕਤੀਆਂ == 1;
    ਅਚਲ ਪੂਰਨ N = isrow ? data0.ਖਾਨੇ_ਗਿਣਤੀ : data0.ਪੰਕਤੀਆਂ;
    ਅਚਲ ਪੂਰਨ dims = (isrow ? 1 : data0.ਖਾਨੇ_ਗਿਣਤੀ)*data0.channels();
    ਅਚਲ ਪੂਰਨ type = data0.ਡੂੰਘਾਈ();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat ਅੰਕੜੇ(N, dims, CV_32F, data0.ptr(), isrow ? dims * ਆਕਾਰ(ਦਸ਼ਮਲਵ) : static_cast<size_t>(data0.ਕਦਮ));

    ਉੱਤਮ_Labels.create(N, 1, CV_32S, -1, ਸੱਚ);

    Mat _labels, ਉੱਤਮ_labels = ਉੱਤਮ_Labels.getMat();
    ਜੇ (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (ਉੱਤਮ_labels.ਖਾਨੇ_ਗਿਣਤੀ == 1 || ਉੱਤਮ_labels.ਪੰਕਤੀਆਂ == 1) &&
                  ਉੱਤਮ_labels.ਖਾਨੇ_ਗਿਣਤੀ*ਉੱਤਮ_labels.ਪੰਕਤੀਆਂ == N &&
                  ਉੱਤਮ_labels.type() == CV_32S &&
                  ਉੱਤਮ_labels.isContinuous());
        ਉੱਤਮ_labels.reshape(1, N).copy_ਮੰਜ਼ਿਲ(_labels);
        ਲਈ (ਪੂਰਨ i = 0; i < N; i++)
        {
            CV_Assert((ਬਿਨਾਨਿਸ਼ਾਨੀ)_labels.at<ਪੂਰਨ>(i) < (ਬਿਨਾਨਿਸ਼ਾਨੀ)K);
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (!((ਉੱਤਮ_labels.ਖਾਨੇ_ਗਿਣਤੀ == 1 || ਉੱਤਮ_labels.ਪੰਕਤੀਆਂ == 1) &&
             ਉੱਤਮ_labels.ਖਾਨੇ_ਗਿਣਤੀ*ਉੱਤਮ_labels.ਪੰਕਤੀਆਂ == N &&
             ਉੱਤਮ_labels.type() == CV_32S &&
             ਉੱਤਮ_labels.isContinuous()))
        {
            ਉੱਤਮ_Labels.create(N, 1, CV_32S);
            ਉੱਤਮ_labels = ਉੱਤਮ_Labels.getMat();
        }
        _labels.create(ਉੱਤਮ_labels.size(), ਉੱਤਮ_labels.type());
    }
    ਪੂਰਨ* labels = _labels.ptr<ਪੂਰਨ>();

    Mat centers(K, dims, type), old_centers(K, dims, type), ਆਰਜ਼ੀ_2(1, dims, type);
    cv::Auto_ਬਫਰ<ਪੂਰਨ, 64> counters(K);
    cv::Auto_ਬਫਰ<ਦੁੱਗਣਾ, 64> dists(N);
    RNG& rng = theRNG();

    ਜੇ (criteria.type & ਪਦ_Criteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    ਨਹੀਂਤਾਂ
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    ਜੇ (criteria.type & ਪਦ_Criteria::ਗਿਣਤੀ_4)
        criteria.max_ਗਿਣਤੀ = std::min(std::max(criteria.max_ਗਿਣਤੀ, 2), 100);
    ਨਹੀਂਤਾਂ
        criteria.max_ਗਿਣਤੀ = 100;

    ਜੇ (K == 1)
    {
        attempts = 1;
        criteria.max_ਗਿਣਤੀ = 2;
    }

    cv::Auto_ਬਫਰ<Vec2f, 64> box(dims);
    ਜੇ (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ਅਚਲ ਦਸ਼ਮਲਵ* sample = ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(0);
            ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        ਲਈ (ਪੂਰਨ i = 1; i < N; i++)
        {
            ਅਚਲ ਦਸ਼ਮਲਵ* sample = ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(i);
            ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
            {
                ਦਸ਼ਮਲਵ v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    ਦੁੱਗਣਾ ਉੱਤਮ_compactness = DBL_MAX;
    ਲਈ (ਪੂਰਨ a = 0; a < attempts; a++)
    {
        ਦੁੱਗਣਾ compactness = DBL_MAX;

        ਲਈ (ਪੂਰਨ iter = 0; ;)
        {
            ਦੁੱਗਣਾ max_center_ਖਿਸਕਾਓ = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            ਜੇ (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                ਜੇ (flags & KMEANS_PP_CENTERS)
                {
                    ਜੇ (!generateCentersPP(ਅੰਕੜੇ, centers, K, rng, SPP_TRIALS))
                        ਰੋਕੋ;
                }
                ਨਹੀਂਤਾਂ
                {
                    ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
                        generateRandomCenter(dims, box.ਅੰਕੜੇ(), centers.ptr<ਦਸ਼ਮਲਵ>(k), rng);
                }
            }
            ਨਹੀਂਤਾਂ
            {
                // compute centers
                centers = Scalar(0);
                ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
                    counters[k] = 0;

                ਲਈ (ਪੂਰਨ i = 0; i < N; i++)
                {
                    ਅਚਲ ਦਸ਼ਮਲਵ* sample = ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(i);
                    ਪੂਰਨ k = labels[i];
                    ਦਸ਼ਮਲਵ* center = centers.ptr<ਦਸ਼ਮਲਵ>(k);
                    ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
                {
                    ਜੇ (counters[k] != 0)
                        ਜਾਰੀ;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    ਪੂਰਨ max_k = 0;
                    ਲਈ (ਪੂਰਨ k1 = 1; k1 < K; k1++)
                    {
                        ਜੇ (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    ਦੁੱਗਣਾ max_dist = 0;
                    ਪੂਰਨ farthest_i = -1;
                    ਦਸ਼ਮਲਵ* ਅਧਾਰ_center = centers.ptr<ਦਸ਼ਮਲਵ>(max_k);
                    ਦਸ਼ਮਲਵ* ਅਧਾਰ_center_2 = ਆਰਜ਼ੀ_2.ptr<ਦਸ਼ਮਲਵ>(); // normalized
                    ਦਸ਼ਮਲਵ scale = 1.f/counters[max_k];
                    ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
                        ਅਧਾਰ_center_2[j] = ਅਧਾਰ_center[j]*scale;

                    ਲਈ (ਪੂਰਨ i = 0; i < N; i++)
                    {
                        ਜੇ (labels[i] != max_k)
                            ਜਾਰੀ;
                        ਜੇ (farthest_i < 0)
                            farthest_i = i;
                        ਅਚਲ ਦਸ਼ਮਲਵ* sample = ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(i);
                        ਦੁੱਗਣਾ dist = hal::normL2Sqr_(sample, ਅਧਾਰ_center_2, dims);

                        ਜੇ (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ਅਚਲ ਦਸ਼ਮਲਵ* sample = ਅੰਕੜੇ.ptr<ਦਸ਼ਮਲਵ>(farthest_i);
                    ਦਸ਼ਮਲਵ* cur_center = centers.ptr<ਦਸ਼ਮਲਵ>(k);
                    ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
                    {
                        ਅਧਾਰ_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                ਲਈ (ਪੂਰਨ k = 0; k < K; k++)
                {
                    ਦਸ਼ਮਲਵ* center = centers.ptr<ਦਸ਼ਮਲਵ>(k);
                    CV_Assert( counters[k] != 0 );

                    ਦਸ਼ਮਲਵ scale = 1.f/counters[k];
                    ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
                        center[j] *= scale;

                    ਜੇ (iter > 0)
                    {
                        ਦੁੱਗਣਾ dist = 0;
                        ਅਚਲ ਦਸ਼ਮਲਵ* old_center = old_centers.ptr<ਦਸ਼ਮਲਵ>(k);
                        ਲਈ (ਪੂਰਨ j = 0; j < dims; j++)
                        {
                            ਦੁੱਗਣਾ t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_ਖਿਸਕਾਓ = std::max(max_center_ਖਿਸਕਾਓ, dist);
                    }
                }
            }

            ਤਾਰਕਿਕ is_ਆਖਰੀ_Iter = (++iter == MAX(criteria.max_ਗਿਣਤੀ, 2) || max_center_ਖਿਸਕਾਓ <= criteria.epsilon);

            ਜੇ (is_ਆਖਰੀ_Iter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeans_ਦੂਰੀ_Computer<ਸੱਚ>(dists.ਅੰਕੜੇ(), labels, ਅੰਕੜੇ, centers), (ਦੁੱਗਣਾ)div_ਉੱਪਰ((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = ਜੋੜ(Mat(ਮਾਪ(N, 1), CV_64F, &dists[0]))[0];
                ਰੋਕੋ;
            }
            ਨਹੀਂਤਾਂ
            {
                // assign labels
                parallel_for_(Range(0, N), KMeans_ਦੂਰੀ_Computer<ਝੂਠ>(dists.ਅੰਕੜੇ(), labels, ਅੰਕੜੇ, centers), (ਦੁੱਗਣਾ)div_ਉੱਪਰ((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                ਜੇ (!(ਜੋੜ(Mat(ਮਾਪ(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    ਰੋਕੋ;
            }
        }

        ਜੇ (!(compactness < DBL_MAX))
        {
            centers.ਸੈੱਟ_Zero();
            _labels.ਸੈੱਟ_Zero();
            compactness = -1.;
        }
        ਜੇ (compactness < ਉੱਤਮ_compactness)
        {
            ਉੱਤਮ_compactness = compactness;
            ਜੇ (_centers.needed())
            {
                ਜੇ (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copy_ਮੰਜ਼ਿਲ(_centers);
                ਨਹੀਂਤਾਂ
                    centers.copy_ਮੰਜ਼ਿਲ(_centers);
            }
            _labels.copy_ਮੰਜ਼ਿਲ(ਉੱਤਮ_labels);
        }
        ਜੇ (compactness < 0.)
            ਰੋਕੋ;
    }

    ਵਾਪਸ ਉੱਤਮ_compactness;
}
