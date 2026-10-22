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

static целое CV_KMEANS_PARALLEL_GRANULARITY = (целое)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

static void generateRandomCenter(целое dims, const Vec2f* box, float* center, RNG& rng)
{
    float margin = 1.f/dims;
    для (целое j = 0; j < dims; j++)
        center[j] = ((float)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

class KMeansPPDistanceComputer : public ParallelLoopBody
{
public:
    KMeansPPDistanceComputer(float *tdist2_, const Mat& data_, const float *dist_, целое ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    void operator()( const cv::Range& range ) const CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        const целое begin = range.start;
        const целое end = range.end;
        const целое dims = data.cols;

        для (целое i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<float>(i), data.ptr<float>(ci), dims), dist[i]);
        }
    }

private:
    KMeansPPDistanceComputer& operator=(const KMeansPPDistanceComputer&); // = delete

    float *tdist2;
    const Mat& data;
    const float *dist;
    const целое ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
static bool generateCentersPP(const Mat& data, Mat& _out_centers,
                              целое K, RNG& rng, целое trials)
{
    CV_TRACE_FUNCTION();
    const целое dims = data.cols, N = data.rows;
    cv::AutoBuffer<целое, 64> _centers(K);
    целое* centers = &_centers[0];
    cv::AutoBuffer<float, 0> _dist(N*3);
    float* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    double sum0 = 0;

    centers[0] = (unsigned)rng % N;

    для (целое i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<float>(i), data.ptr<float>(centers[0]), dims);
        sum0 += dist[i];
    }
    если (!(sum0 < DBL_MAX))
        вернуть false;

    для (целое k = 1; k < K; k++)
    {
        double bestSum = DBL_MAX;
        целое bestCenter = -1;

        для (целое j = 0; j < trials; j++)
        {
            double p = (double)rng*sum0;
            целое ci = 0;
            для (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                если (p <= 0)
                    break;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (double)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            double s = 0;
            целое i = 0;
        #если CV_ENABLE_UNROLLED
            для (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            для (; i < N; i++)
            {
                s += tdist2[i];
            }

            если (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        если (bestCenter < 0)
            вернуть false;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    для (целое k = 0; k < K; k++)
    {
        const float* src = data.ptr<float>(centers[k]);
        float* dst = _out_centers.ptr<float>(k);
        для (целое j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    вернуть true;
}

template<bool onlyDistance>
class KMeansDistanceComputer : public ParallelLoopBody
{
public:
    KMeansDistanceComputer( double *distances_,
                            целое *labels_,
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
        const целое begin = range.start;
        const целое end = range.end;
        const целое K = centers.rows;
        const целое dims = centers.cols;

        для (целое i = begin; i < end; ++i)
        {
            const float *sample = data.ptr<float>(i);
            если (onlyDistance)
            {
                const float* center = centers.ptr<float>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                continue;
            }
            иначе
            {
                целое k_best = 0;
                double min_dist = DBL_MAX;

                для (целое k = 0; k < K; k++)
                {
                    const float* center = centers.ptr<float>(k);
                    const double dist = hal::normL2Sqr_(sample, center, dims);
                    если (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        break;
                    }

                    если (min_dist > dist)
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

    double *distances;
    целое *labels;
    const Mat& data;
    const Mat& centers;
};

}

double cv::kmeans( InputArray _data, целое K,
                   InputOutputArray _bestLabels,
                   TermCriteria criteria, целое attempts,
                   целое flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    const целое SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    const bool isrow = data0.rows == 1;
    const целое N = isrow ? data0.cols : data0.rows;
    const целое dims = (isrow ? 1 : data0.cols)*data0.channels();
    const целое type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * sizeof(float) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, true);

    Mat _labels, best_labels = _bestLabels.getMat();
    если (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        для (целое i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<целое>(i) < (unsigned)K);
        }
    }
    иначе
    {
        если (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    целое* labels = _labels.ptr<целое>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<целое, 64> counters(K);
    cv::AutoBuffer<double, 64> dists(N);
    RNG& rng = theRNG();

    если (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    иначе
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    если (criteria.type & TermCriteria::COUNT)
        criteria.maxCount = std::min(std::max(criteria.maxCount, 2), 100);
    иначе
        criteria.maxCount = 100;

    если (K == 1)
    {
        attempts = 1;
        criteria.maxCount = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    если (!(flags & KMEANS_PP_CENTERS))
    {
        {
            const float* sample = data.ptr<float>(0);
            для (целое j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        для (целое i = 1; i < N; i++)
        {
            const float* sample = data.ptr<float>(i);
            для (целое j = 0; j < dims; j++)
            {
                float v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    double best_compactness = DBL_MAX;
    для (целое a = 0; a < attempts; a++)
    {
        double compactness = DBL_MAX;

        для (целое iter = 0; ;)
        {
            double max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            если (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                если (flags & KMEANS_PP_CENTERS)
                {
                    если (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        break;
                }
                иначе
                {
                    для (целое k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<float>(k), rng);
                }
            }
            иначе
            {
                // compute centers
                centers = Scalar(0);
                для (целое k = 0; k < K; k++)
                    counters[k] = 0;

                для (целое i = 0; i < N; i++)
                {
                    const float* sample = data.ptr<float>(i);
                    целое k = labels[i];
                    float* center = centers.ptr<float>(k);
                    для (целое j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                для (целое k = 0; k < K; k++)
                {
                    если (counters[k] != 0)
                        continue;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    целое max_k = 0;
                    для (целое k1 = 1; k1 < K; k1++)
                    {
                        если (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    double max_dist = 0;
                    целое farthest_i = -1;
                    float* base_center = centers.ptr<float>(max_k);
                    float* _base_center = temp.ptr<float>(); // normalized
                    float scale = 1.f/counters[max_k];
                    для (целое j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    для (целое i = 0; i < N; i++)
                    {
                        если (labels[i] != max_k)
                            continue;
                        если (farthest_i < 0)
                            farthest_i = i;
                        const float* sample = data.ptr<float>(i);
                        double dist = hal::normL2Sqr_(sample, _base_center, dims);

                        если (max_dist <= dist)
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
                    для (целое j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                для (целое k = 0; k < K; k++)
                {
                    float* center = centers.ptr<float>(k);
                    CV_Assert( counters[k] != 0 );

                    float scale = 1.f/counters[k];
                    для (целое j = 0; j < dims; j++)
                        center[j] *= scale;

                    если (iter > 0)
                    {
                        double dist = 0;
                        const float* old_center = old_centers.ptr<float>(k);
                        для (целое j = 0; j < dims; j++)
                        {
                            double t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            bool isLastIter = (++iter == MAX(criteria.maxCount, 2) || max_center_shift <= criteria.epsilon);

            если (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<true>(dists.data(), labels, data, centers), (double)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                break;
            }
            иначе
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<false>(dists.data(), labels, data, centers), (double)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                если (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    break;
            }
        }

        если (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        если (compactness < best_compactness)
        {
            best_compactness = compactness;
            если (_centers.needed())
            {
                если (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                иначе
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        если (compactness < 0.)
            break;
    }

    вернуть best_compactness;
}
