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

#include "precomp.hpp"
#include <opencv2/core/utils/configuration.private.hpp>
#include <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

namespace cv
{

static ចំនួនគត់ CV_KMEANS_PARALLEL_GRANULARITY = (ចំនួនគត់)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

static void generateRandomCenter(ចំនួនគត់ dims, const Vec2f* box, float* center, RNG& rng)
{
    float margin = 1.f/dims;
    សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
        center[j] = ((float)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

class KMeansPPDistanceComputer : public ParallelLoopBody
{
public:
    KMeansPPDistanceComputer(float *tdist2_, const Mat& data_, const float *dist_, ចំនួនគត់ ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    void operator()( const cv::Range& range ) const CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        const ចំនួនគត់ begin = range.start;
        const ចំនួនគត់ end = range.end;
        const ចំនួនគត់ dims = data.cols;

        សម្រាប់ (ចំនួនគត់ i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<float>(i), data.ptr<float>(ci), dims), dist[i]);
        }
    }

private:
    KMeansPPDistanceComputer& operator=(const KMeansPPDistanceComputer&); // = delete

    float *tdist2;
    const Mat& data;
    const float *dist;
    const ចំនួនគត់ ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
static bool generateCentersPP(const Mat& data, Mat& _out_centers,
                              ចំនួនគត់ K, RNG& rng, ចំនួនគត់ trials)
{
    CV_TRACE_FUNCTION();
    const ចំនួនគត់ dims = data.cols, N = data.rows;
    cv::AutoBuffer<ចំនួនគត់, 64> _centers(K);
    ចំនួនគត់* centers = &_centers[0];
    cv::AutoBuffer<float, 0> _dist(N*3);
    float* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    dviguu_na sum0 = 0;

    centers[0] = (unsigned)rng % N;

    សម្រាប់ (ចំនួនគត់ i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<float>(i), data.ptr<float>(centers[0]), dims);
        sum0 += dist[i];
    }
    ប្រសិនបើ (!(sum0 < DBL_MAX))
        ត្រឡប់ false;

    សម្រាប់ (ចំនួនគត់ k = 1; k < K; k++)
    {
        dviguu_na bestSum = DBL_MAX;
        ចំនួនគត់ bestCenter = -1;

        សម្រាប់ (ចំនួនគត់ j = 0; j < trials; j++)
        {
            dviguu_na p = (dviguu_na)rng*sum0;
            ចំនួនគត់ ci = 0;
            សម្រាប់ (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                ប្រសិនបើ (p <= 0)
                    bha_nga;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (dviguu_na)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            dviguu_na s = 0;
            ចំនួនគត់ i = 0;
        #ប្រសិនបើ CV_ENABLE_UNROLLED
            សម្រាប់ (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            សម្រាប់ (; i < N; i++)
            {
                s += tdist2[i];
            }

            ប្រសិនបើ (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        ប្រសិនបើ (bestCenter < 0)
            ត្រឡប់ false;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
    {
        const float* src = data.ptr<float>(centers[k]);
        float* dst = _out_centers.ptr<float>(k);
        សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    ត្រឡប់ true;
}

template<bool onlyDistance>
class KMeansDistanceComputer : public ParallelLoopBody
{
public:
    KMeansDistanceComputer( dviguu_na *distances_,
                            ចំនួនគត់ *labels_,
                            const Mat& data_,
                            const Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    void operator()(const Range& range) const CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        const ចំនួនគត់ begin = range.start;
        const ចំនួនគត់ end = range.end;
        const ចំនួនគត់ K = centers.rows;
        const ចំនួនគត់ dims = centers.cols;

        សម្រាប់ (ចំនួនគត់ i = begin; i < end; ++i)
        {
            const float *sample = data.ptr<float>(i);
            ប្រសិនបើ (onlyDistance)
            {
                const float* center = centers.ptr<float>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                continue;
            }
            បើមិនដូច្នេះ
            {
                ចំនួនគត់ k_best = 0;
                dviguu_na min_dist = DBL_MAX;

                សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
                {
                    const float* center = centers.ptr<float>(k);
                    const dviguu_na dist = hal::normL2Sqr_(sample, center, dims);
                    ប្រសិនបើ (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        bha_nga;
                    }

                    ប្រសិនបើ (min_dist > dist)
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

private:
    KMeansDistanceComputer& operator=(const KMeansDistanceComputer&); // = delete

    dviguu_na *distances;
    ចំនួនគត់ *labels;
    const Mat& data;
    const Mat& centers;
};

}

dviguu_na cv::kmeans( InputArray _data, ចំនួនគត់ K,
                   InputOutputArray _bestLabels,
                   TermCriteria criteria, ចំនួនគត់ attempts,
                   ចំនួនគត់ flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    const ចំនួនគត់ SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    const bool isrow = data0.rows == 1;
    const ចំនួនគត់ N = isrow ? data0.cols : data0.rows;
    const ចំនួនគត់ dims = (isrow ? 1 : data0.cols)*data0.channels();
    const ចំនួនគត់ type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * parimaa_na(float) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, true);

    Mat _labels, best_labels = _bestLabels.getMat();
    ប្រសិនបើ (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        សម្រាប់ (ចំនួនគត់ i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<ចំនួនគត់>(i) < (unsigned)K);
        }
    }
    បើមិនដូច្នេះ
    {
        ប្រសិនបើ (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    ចំនួនគត់* labels = _labels.ptr<ចំនួនគត់>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<ចំនួនគត់, 64> counters(K);
    cv::AutoBuffer<dviguu_na, 64> dists(N);
    RNG& rng = theRNG();

    ប្រសិនបើ (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    បើមិនដូច្នេះ
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    ប្រសិនបើ (criteria.type & TermCriteria::COUNT)
        criteria.maxCount = std::min(std::max(criteria.maxCount, 2), 100);
    បើមិនដូច្នេះ
        criteria.maxCount = 100;

    ប្រសិនបើ (K == 1)
    {
        attempts = 1;
        criteria.maxCount = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    ប្រសិនបើ (!(flags & KMEANS_PP_CENTERS))
    {
        {
            const float* sample = data.ptr<float>(0);
            សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        សម្រាប់ (ចំនួនគត់ i = 1; i < N; i++)
        {
            const float* sample = data.ptr<float>(i);
            សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
            {
                float v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    dviguu_na best_compactness = DBL_MAX;
    សម្រាប់ (ចំនួនគត់ a = 0; a < attempts; a++)
    {
        dviguu_na compactness = DBL_MAX;

        សម្រាប់ (ចំនួនគត់ iter = 0; ;)
        {
            dviguu_na max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            ប្រសិនបើ (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                ប្រសិនបើ (flags & KMEANS_PP_CENTERS)
                {
                    ប្រសិនបើ (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        bha_nga;
                }
                បើមិនដូច្នេះ
                {
                    សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<float>(k), rng);
                }
            }
            បើមិនដូច្នេះ
            {
                // compute centers
                centers = Scalar(0);
                សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
                    counters[k] = 0;

                សម្រាប់ (ចំនួនគត់ i = 0; i < N; i++)
                {
                    const float* sample = data.ptr<float>(i);
                    ចំនួនគត់ k = labels[i];
                    float* center = centers.ptr<float>(k);
                    សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
                {
                    ប្រសិនបើ (counters[k] != 0)
                        continue;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    ចំនួនគត់ max_k = 0;
                    សម្រាប់ (ចំនួនគត់ k1 = 1; k1 < K; k1++)
                    {
                        ប្រសិនបើ (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    dviguu_na max_dist = 0;
                    ចំនួនគត់ farthest_i = -1;
                    float* base_center = centers.ptr<float>(max_k);
                    float* _base_center = temp.ptr<float>(); // normalized
                    float scale = 1.f/counters[max_k];
                    សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    សម្រាប់ (ចំនួនគត់ i = 0; i < N; i++)
                    {
                        ប្រសិនបើ (labels[i] != max_k)
                            continue;
                        ប្រសិនបើ (farthest_i < 0)
                            farthest_i = i;
                        const float* sample = data.ptr<float>(i);
                        dviguu_na dist = hal::normL2Sqr_(sample, _base_center, dims);

                        ប្រសិនបើ (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    const float* sample = data.ptr<float>(farthest_i);
                    float* cur_center = centers.ptr<float>(k);
                    សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                សម្រាប់ (ចំនួនគត់ k = 0; k < K; k++)
                {
                    float* center = centers.ptr<float>(k);
                    CV_Assert( counters[k] != 0 );

                    float scale = 1.f/counters[k];
                    សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
                        center[j] *= scale;

                    ប្រសិនបើ (iter > 0)
                    {
                        dviguu_na dist = 0;
                        const float* old_center = old_centers.ptr<float>(k);
                        សម្រាប់ (ចំនួនគត់ j = 0; j < dims; j++)
                        {
                            dviguu_na t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            bool isLastIter = (++iter == MAX(criteria.maxCount, 2) || max_center_shift <= criteria.epsilon);

            ប្រសិនបើ (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<true>(dists.data(), labels, data, centers), (dviguu_na)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                bha_nga;
            }
            បើមិនដូច្នេះ
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<false>(dists.data(), labels, data, centers), (dviguu_na)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                ប្រសិនបើ (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    bha_nga;
            }
        }

        ប្រសិនបើ (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        ប្រសិនបើ (compactness < best_compactness)
        {
            best_compactness = compactness;
            ប្រសិនបើ (_centers.needed())
            {
                ប្រសិនបើ (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                បើមិនដូច្នេះ
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        ប្រសិនបើ (compactness < 0.)
            bha_nga;
    }

    ត្រឡប់ best_compactness;
}
