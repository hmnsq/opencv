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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ CV_KMEANS_PARALLEL_GRANULARITY = (ಪೂರ್ಣಾಙ್ಕ)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

ಸ್ಥಿರ ಶೂನ್ಯ generateRandomCenter(ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ Vec2f* box, ಭಿನ್ನ* center, RNG& rng)
{
    ಭಿನ್ನ margin = 1.f/dims;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
        center[j] = ((ಭಿನ್ನ)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

ವರ್ಗ KMeansPPDistanceComputer : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
    KMeansPPDistanceComputer(ಭಿನ್ನ *tdist2_, ಧ್ರುವ Mat& data_, ಧ್ರುವ ಭಿನ್ನ *dist_, ಪೂರ್ಣಾಙ್ಕ ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    ಶೂನ್ಯ operator()( ಧ್ರುವ cv::Range& range ) ಧ್ರುವ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ begin = range.start;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ end = range.end;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dims = data.cols;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<ಭಿನ್ನ>(i), data.ptr<ಭಿನ್ನ>(ci), dims), dist[i]);
        }
    }

ಗುಪ್ತ:
    KMeansPPDistanceComputer& operator=(ಧ್ರುವ KMeansPPDistanceComputer&); // = delete

    ಭಿನ್ನ *tdist2;
    ಧ್ರುವ Mat& data;
    ಧ್ರುವ ಭಿನ್ನ *dist;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
ಸ್ಥಿರ ತಾರ್ಕಿಕ generateCentersPP(ಧ್ರುವ Mat& data, Mat& _out_centers,
                              ಪೂರ್ಣಾಙ್ಕ K, RNG& rng, ಪೂರ್ಣಾಙ್ಕ trials)
{
    CV_TRACE_FUNCTION();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dims = data.cols, N = data.rows;
    cv::AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, 64> _centers(K);
    ಪೂರ್ಣಾಙ್ಕ* centers = &_centers[0];
    cv::AutoBuffer<ಭಿನ್ನ, 0> _dist(N*3);
    ಭಿನ್ನ* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    ದ್ವಿಗುಣ sum0 = 0;

    centers[0] = (unsigned)rng % N;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<ಭಿನ್ನ>(i), data.ptr<ಭಿನ್ನ>(centers[0]), dims);
        sum0 += dist[i];
    }
    ಯದಿ (!(sum0 < DBL_MAX))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 1; k < K; k++)
    {
        ದ್ವಿಗುಣ bestSum = DBL_MAX;
        ಪೂರ್ಣಾಙ್ಕ bestCenter = -1;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < trials; j++)
        {
            ದ್ವಿಗುಣ p = (ದ್ವಿಗುಣ)rng*sum0;
            ಪೂರ್ಣಾಙ್ಕ ci = 0;
            ಪ್ರತಿ (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                ಯದಿ (p <= 0)
                    ಭಙ್ಗ;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (ದ್ವಿಗುಣ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            ದ್ವಿಗುಣ s = 0;
            ಪೂರ್ಣಾಙ್ಕ i = 0;
        #ಯದಿ CV_ENABLE_UNROLLED
            ಪ್ರತಿ (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            ಪ್ರತಿ (; i < N; i++)
            {
                s += tdist2[i];
            }

            ಯದಿ (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        ಯದಿ (bestCenter < 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
    {
        ಧ್ರುವ ಭಿನ್ನ* src = data.ptr<ಭಿನ್ನ>(centers[k]);
        ಭಿನ್ನ* dst = _out_centers.ptr<ಭಿನ್ನ>(k);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

template<ತಾರ್ಕಿಕ onlyDistance>
ವರ್ಗ KMeansDistanceComputer : ಸಾರ್ವಜನಿಕ ParallelLoopBody
{
ಸಾರ್ವಜನಿಕ:
    KMeansDistanceComputer( ದ್ವಿಗುಣ *distances_,
                            ಪೂರ್ಣಾಙ್ಕ *labels_,
                            ಧ್ರುವ Mat& data_,
                            ಧ್ರುವ Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    ಶೂನ್ಯ operator()(ಧ್ರುವ Range& range) ಧ್ರುವ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ begin = range.start;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ end = range.end;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ K = centers.rows;
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dims = centers.cols;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = begin; i < end; ++i)
        {
            ಧ್ರುವ ಭಿನ್ನ *sample = data.ptr<ಭಿನ್ನ>(i);
            ಯದಿ (onlyDistance)
            {
                ಧ್ರುವ ಭಿನ್ನ* center = centers.ptr<ಭಿನ್ನ>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                ಅನುವರ್ತ;
            }
            ಅನ್ಯಥಾ
            {
                ಪೂರ್ಣಾಙ್ಕ k_best = 0;
                ದ್ವಿಗುಣ min_dist = DBL_MAX;

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
                {
                    ಧ್ರುವ ಭಿನ್ನ* center = centers.ptr<ಭಿನ್ನ>(k);
                    ಧ್ರುವ ದ್ವಿಗುಣ dist = hal::normL2Sqr_(sample, center, dims);
                    ಯದಿ (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        ಭಙ್ಗ;
                    }

                    ಯದಿ (min_dist > dist)
                    {
                        min_dist = dist;
                        k_best = k;
                    }
                }

                distances[i] = min_dist;
                labels[i] = k_best;
            }
        }
    }

ಗುಪ್ತ:
    KMeansDistanceComputer& operator=(ಧ್ರುವ KMeansDistanceComputer&); // = delete

    ದ್ವಿಗುಣ *distances;
    ಪೂರ್ಣಾಙ್ಕ *labels;
    ಧ್ರುವ Mat& data;
    ಧ್ರುವ Mat& centers;
};

}

ದ್ವಿಗುಣ cv::kmeans( ಒಳಹರಿವು_Array _data, ಪೂರ್ಣಾಙ್ಕ K,
                   ಒಳಹರಿವು_Output_Array _bestLabels,
                   TermCriteria criteria, ಪೂರ್ಣಾಙ್ಕ attempts,
                   ಪೂರ್ಣಾಙ್ಕ flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    ಧ್ರುವ ತಾರ್ಕಿಕ isrow = data0.rows == 1;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ N = isrow ? data0.cols : data0.rows;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dims = (isrow ? 1 : data0.cols)*data0.channels();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * ಪರಿಮಾಣ(ಭಿನ್ನ) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, ಸತ್ಯ);

    Mat _labels, best_labels = _bestLabels.getMat();
    ಯದಿ (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<ಪೂರ್ಣಾಙ್ಕ>(i) < (unsigned)K);
        }
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    ಪೂರ್ಣಾಙ್ಕ* labels = _labels.ptr<ಪೂರ್ಣಾಙ್ಕ>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<ಪೂರ್ಣಾಙ್ಕ, 64> counters(K);
    cv::AutoBuffer<ದ್ವಿಗುಣ, 64> dists(N);
    RNG& rng = theRNG();

    ಯದಿ (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    ಅನ್ಯಥಾ
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    ಯದಿ (criteria.type & TermCriteria::ಎಣಿಕೆ_3)
        criteria.max_ಎಣಿಕೆ = std::min(std::max(criteria.max_ಎಣಿಕೆ, 2), 100);
    ಅನ್ಯಥಾ
        criteria.max_ಎಣಿಕೆ = 100;

    ಯದಿ (K == 1)
    {
        attempts = 1;
        criteria.max_ಎಣಿಕೆ = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    ಯದಿ (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ಧ್ರುವ ಭಿನ್ನ* sample = data.ptr<ಭಿನ್ನ>(0);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < N; i++)
        {
            ಧ್ರುವ ಭಿನ್ನ* sample = data.ptr<ಭಿನ್ನ>(i);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
            {
                ಭಿನ್ನ v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    ದ್ವಿಗುಣ best_compactness = DBL_MAX;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ a = 0; a < attempts; a++)
    {
        ದ್ವಿಗುಣ compactness = DBL_MAX;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ iter = 0; ;)
        {
            ದ್ವಿಗುಣ max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            ಯದಿ (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                ಯದಿ (flags & KMEANS_PP_CENTERS)
                {
                    ಯದಿ (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        ಭಙ್ಗ;
                }
                ಅನ್ಯಥಾ
                {
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<ಭಿನ್ನ>(k), rng);
                }
            }
            ಅನ್ಯಥಾ
            {
                // compute centers
                centers = Scalar(0);
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
                    counters[k] = 0;

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < N; i++)
                {
                    ಧ್ರುವ ಭಿನ್ನ* sample = data.ptr<ಭಿನ್ನ>(i);
                    ಪೂರ್ಣಾಙ್ಕ k = labels[i];
                    ಭಿನ್ನ* center = centers.ptr<ಭಿನ್ನ>(k);
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
                {
                    ಯದಿ (counters[k] != 0)
                        ಅನುವರ್ತ;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    ಪೂರ್ಣಾಙ್ಕ max_k = 0;
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k1 = 1; k1 < K; k1++)
                    {
                        ಯದಿ (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    ದ್ವಿಗುಣ max_dist = 0;
                    ಪೂರ್ಣಾಙ್ಕ farthest_i = -1;
                    ಭಿನ್ನ* base_center = centers.ptr<ಭಿನ್ನ>(max_k);
                    ಭಿನ್ನ* _base_center = temp.ptr<ಭಿನ್ನ>(); // normalized
                    ಭಿನ್ನ scale = 1.f/counters[max_k];
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < N; i++)
                    {
                        ಯದಿ (labels[i] != max_k)
                            ಅನುವರ್ತ;
                        ಯದಿ (farthest_i < 0)
                            farthest_i = i;
                        ಧ್ರುವ ಭಿನ್ನ* sample = data.ptr<ಭಿನ್ನ>(i);
                        ದ್ವಿಗುಣ dist = hal::normL2Sqr_(sample, _base_center, dims);

                        ಯದಿ (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ಧ್ರುವ ಭಿನ್ನ* sample = data.ptr<ಭಿನ್ನ>(farthest_i);
                    ಭಿನ್ನ* cur_center = centers.ptr<ಭಿನ್ನ>(k);
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < K; k++)
                {
                    ಭಿನ್ನ* center = centers.ptr<ಭಿನ್ನ>(k);
                    CV_Assert( counters[k] != 0 );

                    ಭಿನ್ನ scale = 1.f/counters[k];
                    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
                        center[j] *= scale;

                    ಯದಿ (iter > 0)
                    {
                        ದ್ವಿಗುಣ dist = 0;
                        ಧ್ರುವ ಭಿನ್ನ* old_center = old_centers.ptr<ಭಿನ್ನ>(k);
                        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < dims; j++)
                        {
                            ದ್ವಿಗುಣ t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            ತಾರ್ಕಿಕ isLastIter = (++iter == MAX(criteria.max_ಎಣಿಕೆ, 2) || max_center_shift <= criteria.epsilon);

            ಯದಿ (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<ಸತ್ಯ>(dists.data(), labels, data, centers), (ದ್ವಿಗುಣ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                ಭಙ್ಗ;
            }
            ಅನ್ಯಥಾ
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<ಅಸತ್ಯ>(dists.data(), labels, data, centers), (ದ್ವಿಗುಣ)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                ಯದಿ (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    ಭಙ್ಗ;
            }
        }

        ಯದಿ (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        ಯದಿ (compactness < best_compactness)
        {
            best_compactness = compactness;
            ಯದಿ (_centers.needed())
            {
                ಯದಿ (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                ಅನ್ಯಥಾ
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        ಯದಿ (compactness < 0.)
            ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ best_compactness;
}
