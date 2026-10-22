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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

ನಾಮಾಕಾಶ cv
{

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ CV_KMEANS_PARALLEL_GRANULARITY = (ಪೂರ್ಣಾಂಕ)utils::get_Configuration_Parameter_ಗಾತ್ರ_T("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

ಸ್ಥಿರ ಶೂನ್ಯ generateRandomCenter(ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ Vec2f* box, ದಶಮಾಂಶ* center, RNG& rng)
{
    ದಶಮಾಂಶ margin = 1.f/dims;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
        center[j] = ((ದಶಮಾಂಶ)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

ವರ್ಗ KMeansPPDistanceComputer : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
    KMeansPPDistanceComputer(ದಶಮಾಂಶ *tdist2_, ಸ್ಥಿರಾಂಕ Mat& ದತ್ತಾಂಶ_2, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *dist_, ಪೂರ್ಣಾಂಕ ci_) :
        tdist2(tdist2_), ದತ್ತಾಂಶ(ದತ್ತಾಂಶ_2), dist(dist_), ci(ci_)
    { }

    ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()( ಸ್ಥಿರಾಂಕ cv::Range& range ) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ begin = range.ಆರಂಭ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ end = range.end;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dims = ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(i), ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(ci), dims), dist[i]);
        }
    }

ಖಾಸಗಿ:
    KMeansPPDistanceComputer& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ KMeansPPDistanceComputer&); // = delete

    ದಶಮಾಂಶ *tdist2;
    ಸ್ಥಿರಾಂಕ Mat& ದತ್ತಾಂಶ;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *dist;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
ಸ್ಥಿರ ತಾರ್ಕಿಕ generateCentersPP(ಸ್ಥಿರಾಂಕ Mat& ದತ್ತಾಂಶ, Mat& ಹೊರಹೋಗು_centers,
                              ಪೂರ್ಣಾಂಕ K, RNG& rng, ಪೂರ್ಣಾಂಕ trials)
{
    CV_TRACE_FUNCTION();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dims = ದತ್ತಾಂಶ.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, N = ದತ್ತಾಂಶ.ಅಡ್ಡಸಾಲುಗಳು;
    cv::Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, 64> _centers(K);
    ಪೂರ್ಣಾಂಕ* centers = &_centers[0];
    cv::Auto_ಬಫರ್<ದಶಮಾಂಶ, 0> _dist(N*3);
    ದಶಮಾಂಶ* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    ದ್ವಿಗುಣ sum0 = 0;

    centers[0] = (ಚಿಹ್ನೆರಹಿತ)rng % N;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(i), ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(centers[0]), dims);
        sum0 += dist[i];
    }
    ಆದರೆ (!(sum0 < DBL_MAX))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 1; k < K; k++)
    {
        ದ್ವಿಗುಣ ಉತ್ತಮ_ಮೊತ್ತ = DBL_MAX;
        ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_Center = -1;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < trials; j++)
        {
            ದ್ವಿಗುಣ p = (ದ್ವಿಗುಣ)rng*sum0;
            ಪೂರ್ಣಾಂಕ ci = 0;
            ಗಾಗಿ (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                ಆದರೆ (p <= 0)
                    ನಿಲ್ಲಿಸು;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, ದತ್ತಾಂಶ, dist, ci),
                          (ದ್ವಿಗುಣ)div_ಮೇಲೆ((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            ದ್ವಿಗುಣ s = 0;
            ಪೂರ್ಣಾಂಕ i = 0;
        #ಆದರೆ CV_ENABLE_UNROLLED
            ಗಾಗಿ (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #ಆದರೆ_ಅಂತ್ಯ
            ಗಾಗಿ (; i < N; i++)
            {
                s += tdist2[i];
            }

            ಆದರೆ (s < ಉತ್ತಮ_ಮೊತ್ತ)
            {
                ಉತ್ತಮ_ಮೊತ್ತ = s;
                ಉತ್ತಮ_Center = ci;
                std::swap(tdist, tdist2);
            }
        }
        ಆದರೆ (ಉತ್ತಮ_Center < 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        centers[k] = ಉತ್ತಮ_Center;
        sum0 = ಉತ್ತಮ_ಮೊತ್ತ;
        std::swap(dist, tdist);
    }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
    {
        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* src = ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(centers[k]);
        ದಶಮಾಂಶ* dst = ಹೊರಹೋಗು_centers.ptr<ದಶಮಾಂಶ>(k);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಮಾದರಿ<ತಾರ್ಕಿಕ only_ದೂರ>
ವರ್ಗ KMeans_ದೂರ_Computer : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
    KMeans_ದೂರ_Computer( ದ್ವಿಗುಣ *distances_,
                            ಪೂರ್ಣಾಂಕ *labels_,
                            ಸ್ಥಿರಾಂಕ Mat& ದತ್ತಾಂಶ_2,
                            ಸ್ಥಿರಾಂಕ Mat& centers_)
        : distances(distances_),
          labels(labels_),
          ದತ್ತಾಂಶ(ದತ್ತಾಂಶ_2),
          centers(centers_)
    {
    }

    ಶೂನ್ಯ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Range& range) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ begin = range.ಆರಂಭ;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ end = range.end;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ K = centers.ಅಡ್ಡಸಾಲುಗಳು;
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dims = centers.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = begin; i < end; ++i)
        {
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *sample = ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(i);
            ಆದರೆ (only_ದೂರ)
            {
                ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* center = centers.ptr<ದಶಮಾಂಶ>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                ಮುಂದುವರಿ;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಪೂರ್ಣಾಂಕ k_ಉತ್ತಮ = 0;
                ದ್ವಿಗುಣ min_dist = DBL_MAX;

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
                {
                    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* center = centers.ptr<ದಶಮಾಂಶ>(k);
                    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ dist = hal::normL2Sqr_(sample, center, dims);
                    ಆದರೆ (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        ನಿಲ್ಲಿಸು;
                    }

                    ಆದರೆ (min_dist > dist)
                    {
                        min_dist = dist;
                        k_ಉತ್ತಮ = k;
                    }
                }

                distances[i] = min_dist;
                labels[i] = k_ಉತ್ತಮ;
            }
        }
    }

ಖಾಸಗಿ:
    KMeans_ದೂರ_Computer& ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ KMeans_ದೂರ_Computer&); // = delete

    ದ್ವಿಗುಣ *distances;
    ಪೂರ್ಣಾಂಕ *labels;
    ಸ್ಥಿರಾಂಕ Mat& ದತ್ತಾಂಶ;
    ಸ್ಥಿರಾಂಕ Mat& centers;
};

}

ದ್ವಿಗುಣ cv::kmeans( ಒಳಹರಿವು_Array ದತ್ತಾಂಶ_3, ಪೂರ್ಣಾಂಕ K,
                   ಒಳಹರಿವು_Output_Array ಉತ್ತಮ_Labels,
                   ಪದ_Criteria criteria, ಪೂರ್ಣಾಂಕ attempts,
                   ಪೂರ್ಣಾಂಕ flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ SPP_TRIALS = 3;
    Mat data0 = ದತ್ತಾಂಶ_3.getMat();
    ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ isrow = data0.ಅಡ್ಡಸಾಲುಗಳು == 1;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ N = isrow ? data0.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ : data0.ಅಡ್ಡಸಾಲುಗಳು;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dims = (isrow ? 1 : data0.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)*data0.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ type = data0.ಆಳ();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat ದತ್ತಾಂಶ(N, dims, CV_32F, data0.ptr(), isrow ? dims * ಗಾತ್ರ(ದಶಮಾಂಶ) : static_cast<size_t>(data0.ಹೆಜ್ಜೆ));

    ಉತ್ತಮ_Labels.create(N, 1, CV_32S, -1, ನಿಜ);

    Mat _labels, ಉತ್ತಮ_labels = ಉತ್ತಮ_Labels.getMat();
    ಆದರೆ (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (ಉತ್ತಮ_labels.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || ಉತ್ತಮ_labels.ಅಡ್ಡಸಾಲುಗಳು == 1) &&
                  ಉತ್ತಮ_labels.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಉತ್ತಮ_labels.ಅಡ್ಡಸಾಲುಗಳು == N &&
                  ಉತ್ತಮ_labels.type() == CV_32S &&
                  ಉತ್ತಮ_labels.isContinuous());
        ಉತ್ತಮ_labels.reshape(1, N).copy_ಗಮ್ಯ(_labels);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N; i++)
        {
            CV_Assert((ಚಿಹ್ನೆರಹಿತ)_labels.at<ಪೂರ್ಣಾಂಕ>(i) < (ಚಿಹ್ನೆರಹಿತ)K);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಆದರೆ (!((ಉತ್ತಮ_labels.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 || ಉತ್ತಮ_labels.ಅಡ್ಡಸಾಲುಗಳು == 1) &&
             ಉತ್ತಮ_labels.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಉತ್ತಮ_labels.ಅಡ್ಡಸಾಲುಗಳು == N &&
             ಉತ್ತಮ_labels.type() == CV_32S &&
             ಉತ್ತಮ_labels.isContinuous()))
        {
            ಉತ್ತಮ_Labels.create(N, 1, CV_32S);
            ಉತ್ತಮ_labels = ಉತ್ತಮ_Labels.getMat();
        }
        _labels.create(ಉತ್ತಮ_labels.size(), ಉತ್ತಮ_labels.type());
    }
    ಪೂರ್ಣಾಂಕ* labels = _labels.ptr<ಪೂರ್ಣಾಂಕ>();

    Mat centers(K, dims, type), old_centers(K, dims, type), ತಾತ್ಕಾಲಿಕ_2(1, dims, type);
    cv::Auto_ಬಫರ್<ಪೂರ್ಣಾಂಕ, 64> counters(K);
    cv::Auto_ಬಫರ್<ದ್ವಿಗುಣ, 64> dists(N);
    RNG& rng = theRNG();

    ಆದರೆ (criteria.type & ಪದ_Criteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    ಇಲ್ಲದಿದ್ದರೆ
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    ಆದರೆ (criteria.type & ಪದ_Criteria::ಎಣಿಕೆ_4)
        criteria.max_ಎಣಿಕೆ = std::min(std::max(criteria.max_ಎಣಿಕೆ, 2), 100);
    ಇಲ್ಲದಿದ್ದರೆ
        criteria.max_ಎಣಿಕೆ = 100;

    ಆದರೆ (K == 1)
    {
        attempts = 1;
        criteria.max_ಎಣಿಕೆ = 2;
    }

    cv::Auto_ಬಫರ್<Vec2f, 64> box(dims);
    ಆದರೆ (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* sample = ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(0);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < N; i++)
        {
            ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* sample = ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(i);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
            {
                ದಶಮಾಂಶ v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    ದ್ವಿಗುಣ ಉತ್ತಮ_compactness = DBL_MAX;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ a = 0; a < attempts; a++)
    {
        ದ್ವಿಗುಣ compactness = DBL_MAX;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ iter = 0; ;)
        {
            ದ್ವಿಗುಣ max_center_ಸರಿಸು = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            ಆದರೆ (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                ಆದರೆ (flags & KMEANS_PP_CENTERS)
                {
                    ಆದರೆ (!generateCentersPP(ದತ್ತಾಂಶ, centers, K, rng, SPP_TRIALS))
                        ನಿಲ್ಲಿಸು;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
                        generateRandomCenter(dims, box.ದತ್ತಾಂಶ(), centers.ptr<ದಶಮಾಂಶ>(k), rng);
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // compute centers
                centers = Scalar(0);
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
                    counters[k] = 0;

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N; i++)
                {
                    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* sample = ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(i);
                    ಪೂರ್ಣಾಂಕ k = labels[i];
                    ದಶಮಾಂಶ* center = centers.ptr<ದಶಮಾಂಶ>(k);
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
                {
                    ಆದರೆ (counters[k] != 0)
                        ಮುಂದುವರಿ;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    ಪೂರ್ಣಾಂಕ max_k = 0;
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k1 = 1; k1 < K; k1++)
                    {
                        ಆದರೆ (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    ದ್ವಿಗುಣ max_dist = 0;
                    ಪೂರ್ಣಾಂಕ farthest_i = -1;
                    ದಶಮಾಂಶ* ತಳ_center = centers.ptr<ದಶಮಾಂಶ>(max_k);
                    ದಶಮಾಂಶ* ತಳ_center_2 = ತಾತ್ಕಾಲಿಕ_2.ptr<ದಶಮಾಂಶ>(); // normalized
                    ದಶಮಾಂಶ scale = 1.f/counters[max_k];
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
                        ತಳ_center_2[j] = ತಳ_center[j]*scale;

                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < N; i++)
                    {
                        ಆದರೆ (labels[i] != max_k)
                            ಮುಂದುವರಿ;
                        ಆದರೆ (farthest_i < 0)
                            farthest_i = i;
                        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* sample = ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(i);
                        ದ್ವಿಗುಣ dist = hal::normL2Sqr_(sample, ತಳ_center_2, dims);

                        ಆದರೆ (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* sample = ದತ್ತಾಂಶ.ptr<ದಶಮಾಂಶ>(farthest_i);
                    ದಶಮಾಂಶ* cur_center = centers.ptr<ದಶಮಾಂಶ>(k);
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
                    {
                        ತಳ_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < K; k++)
                {
                    ದಶಮಾಂಶ* center = centers.ptr<ದಶಮಾಂಶ>(k);
                    CV_Assert( counters[k] != 0 );

                    ದಶಮಾಂಶ scale = 1.f/counters[k];
                    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
                        center[j] *= scale;

                    ಆದರೆ (iter > 0)
                    {
                        ದ್ವಿಗುಣ dist = 0;
                        ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ* old_center = old_centers.ptr<ದಶಮಾಂಶ>(k);
                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < dims; j++)
                        {
                            ದ್ವಿಗುಣ t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_ಸರಿಸು = std::max(max_center_ಸರಿಸು, dist);
                    }
                }
            }

            ತಾರ್ಕಿಕ is_ಕೊನೆಯ_Iter = (++iter == MAX(criteria.max_ಎಣಿಕೆ, 2) || max_center_ಸರಿಸು <= criteria.epsilon);

            ಆದರೆ (is_ಕೊನೆಯ_Iter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeans_ದೂರ_Computer<ನಿಜ>(dists.ದತ್ತಾಂಶ(), labels, ದತ್ತಾಂಶ, centers), (ದ್ವಿಗುಣ)div_ಮೇಲೆ((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = ಮೊತ್ತ(Mat(ಗಾತ್ರ_2(N, 1), CV_64F, &dists[0]))[0];
                ನಿಲ್ಲಿಸು;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                // assign labels
                parallel_for_(Range(0, N), KMeans_ದೂರ_Computer<ಸುಳ್ಳು>(dists.ದತ್ತಾಂಶ(), labels, ದತ್ತಾಂಶ, centers), (ದ್ವಿಗುಣ)div_ಮೇಲೆ((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                ಆದರೆ (!(ಮೊತ್ತ(Mat(ಗಾತ್ರ_2(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    ನಿಲ್ಲಿಸು;
            }
        }

        ಆದರೆ (!(compactness < DBL_MAX))
        {
            centers.ಗಣ_Zero();
            _labels.ಗಣ_Zero();
            compactness = -1.;
        }
        ಆದರೆ (compactness < ಉತ್ತಮ_compactness)
        {
            ಉತ್ತಮ_compactness = compactness;
            ಆದರೆ (_centers.needed())
            {
                ಆದರೆ (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copy_ಗಮ್ಯ(_centers);
                ಇಲ್ಲದಿದ್ದರೆ
                    centers.copy_ಗಮ್ಯ(_centers);
            }
            _labels.copy_ಗಮ್ಯ(ಉತ್ತಮ_labels);
        }
        ಆದರೆ (compactness < 0.)
            ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ ಉತ್ತಮ_compactness;
}
