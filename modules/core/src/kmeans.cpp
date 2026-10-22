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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>
#અન્તર્ભાવ <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

નામાકાશ cv
{

સ્થિર પૂર્ણાંક CV_KMEANS_PARALLEL_GRANULARITY = (પૂર્ણાંક)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

સ્થિર શૂન્ય generateRandomCenter(પૂર્ણાંક dims, અચલ Vec2f* box, દશાંશ* center, RNG& rng)
{
    દશાંશ margin = 1.f/dims;
    માટે (પૂર્ણાંક j = 0; j < dims; j++)
        center[j] = ((દશાંશ)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

વર્ગ KMeansPPDistanceComputer : જાહેર ParallelLoopBody
{
જાહેર:
    KMeansPPDistanceComputer(દશાંશ *tdist2_, અચલ Mat& data_, અચલ દશાંશ *dist_, પૂર્ણાંક ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    શૂન્ય operator()( અચલ cv::Range& range ) અચલ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        અચલ પૂર્ણાંક begin = range.start;
        અચલ પૂર્ણાંક end = range.end;
        અચલ પૂર્ણાંક dims = data.cols;

        માટે (પૂર્ણાંક i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<દશાંશ>(i), data.ptr<દશાંશ>(ci), dims), dist[i]);
        }
    }

ખાનગી:
    KMeansPPDistanceComputer& operator=(અચલ KMeansPPDistanceComputer&); // = delete

    દશાંશ *tdist2;
    અચલ Mat& data;
    અચલ દશાંશ *dist;
    અચલ પૂર્ણાંક ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
સ્થિર તાર્કિક generateCentersPP(અચલ Mat& data, Mat& _out_centers,
                              પૂર્ણાંક K, RNG& rng, પૂર્ણાંક trials)
{
    CV_TRACE_FUNCTION();
    અચલ પૂર્ણાંક dims = data.cols, N = data.rows;
    cv::AutoBuffer<પૂર્ણાંક, 64> _centers(K);
    પૂર્ણાંક* centers = &_centers[0];
    cv::AutoBuffer<દશાંશ, 0> _dist(N*3);
    દશાંશ* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    દ્વિગુણ sum0 = 0;

    centers[0] = (unsigned)rng % N;

    માટે (પૂર્ણાંક i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<દશાંશ>(i), data.ptr<દશાંશ>(centers[0]), dims);
        sum0 += dist[i];
    }
    જો (!(sum0 < DBL_MAX))
        પાછા ખોટું;

    માટે (પૂર્ણાંક k = 1; k < K; k++)
    {
        દ્વિગુણ bestSum = DBL_MAX;
        પૂર્ણાંક bestCenter = -1;

        માટે (પૂર્ણાંક j = 0; j < trials; j++)
        {
            દ્વિગુણ p = (દ્વિગુણ)rng*sum0;
            પૂર્ણાંક ci = 0;
            માટે (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                જો (p <= 0)
                    તોડો;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (દ્વિગુણ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            દ્વિગુણ s = 0;
            પૂર્ણાંક i = 0;
        #જો CV_ENABLE_UNROLLED
            માટે (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            માટે (; i < N; i++)
            {
                s += tdist2[i];
            }

            જો (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        જો (bestCenter < 0)
            પાછા ખોટું;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    માટે (પૂર્ણાંક k = 0; k < K; k++)
    {
        અચલ દશાંશ* src = data.ptr<દશાંશ>(centers[k]);
        દશાંશ* dst = _out_centers.ptr<દશાંશ>(k);
        માટે (પૂર્ણાંક j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    પાછા સાચું;
}

template<તાર્કિક onlyDistance>
વર્ગ KMeansDistanceComputer : જાહેર ParallelLoopBody
{
જાહેર:
    KMeansDistanceComputer( દ્વિગુણ *distances_,
                            પૂર્ણાંક *labels_,
                            અચલ Mat& data_,
                            અચલ Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    શૂન્ય operator()(અચલ Range& range) અચલ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        અચલ પૂર્ણાંક begin = range.start;
        અચલ પૂર્ણાંક end = range.end;
        અચલ પૂર્ણાંક K = centers.rows;
        અચલ પૂર્ણાંક dims = centers.cols;

        માટે (પૂર્ણાંક i = begin; i < end; ++i)
        {
            અચલ દશાંશ *sample = data.ptr<દશાંશ>(i);
            જો (onlyDistance)
            {
                અચલ દશાંશ* center = centers.ptr<દશાંશ>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                ચાલુ;
            }
            નહીંતર
            {
                પૂર્ણાંક k_best = 0;
                દ્વિગુણ min_dist = DBL_MAX;

                માટે (પૂર્ણાંક k = 0; k < K; k++)
                {
                    અચલ દશાંશ* center = centers.ptr<દશાંશ>(k);
                    અચલ દ્વિગુણ dist = hal::normL2Sqr_(sample, center, dims);
                    જો (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        તોડો;
                    }

                    જો (min_dist > dist)
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

ખાનગી:
    KMeansDistanceComputer& operator=(અચલ KMeansDistanceComputer&); // = delete

    દ્વિગુણ *distances;
    પૂર્ણાંક *labels;
    અચલ Mat& data;
    અચલ Mat& centers;
};

}

દ્વિગુણ cv::kmeans( આગત_Array _data, પૂર્ણાંક K,
                   આગત_Output_Array _bestLabels,
                   TermCriteria criteria, પૂર્ણાંક attempts,
                   પૂર્ણાંક flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    અચલ પૂર્ણાંક SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    અચલ તાર્કિક isrow = data0.rows == 1;
    અચલ પૂર્ણાંક N = isrow ? data0.cols : data0.rows;
    અચલ પૂર્ણાંક dims = (isrow ? 1 : data0.cols)*data0.channels();
    અચલ પૂર્ણાંક type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * કદ(દશાંશ) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, સાચું);

    Mat _labels, best_labels = _bestLabels.getMat();
    જો (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        માટે (પૂર્ણાંક i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<પૂર્ણાંક>(i) < (unsigned)K);
        }
    }
    નહીંતર
    {
        જો (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    પૂર્ણાંક* labels = _labels.ptr<પૂર્ણાંક>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<પૂર્ણાંક, 64> counters(K);
    cv::AutoBuffer<દ્વિગુણ, 64> dists(N);
    RNG& rng = theRNG();

    જો (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    નહીંતર
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    જો (criteria.type & TermCriteria::ગણતરી_3)
        criteria.max_ગણતરી = std::min(std::max(criteria.max_ગણતરી, 2), 100);
    નહીંતર
        criteria.max_ગણતરી = 100;

    જો (K == 1)
    {
        attempts = 1;
        criteria.max_ગણતરી = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    જો (!(flags & KMEANS_PP_CENTERS))
    {
        {
            અચલ દશાંશ* sample = data.ptr<દશાંશ>(0);
            માટે (પૂર્ણાંક j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        માટે (પૂર્ણાંક i = 1; i < N; i++)
        {
            અચલ દશાંશ* sample = data.ptr<દશાંશ>(i);
            માટે (પૂર્ણાંક j = 0; j < dims; j++)
            {
                દશાંશ v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    દ્વિગુણ best_compactness = DBL_MAX;
    માટે (પૂર્ણાંક a = 0; a < attempts; a++)
    {
        દ્વિગુણ compactness = DBL_MAX;

        માટે (પૂર્ણાંક iter = 0; ;)
        {
            દ્વિગુણ max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            જો (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                જો (flags & KMEANS_PP_CENTERS)
                {
                    જો (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        તોડો;
                }
                નહીંતર
                {
                    માટે (પૂર્ણાંક k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<દશાંશ>(k), rng);
                }
            }
            નહીંતર
            {
                // compute centers
                centers = Scalar(0);
                માટે (પૂર્ણાંક k = 0; k < K; k++)
                    counters[k] = 0;

                માટે (પૂર્ણાંક i = 0; i < N; i++)
                {
                    અચલ દશાંશ* sample = data.ptr<દશાંશ>(i);
                    પૂર્ણાંક k = labels[i];
                    દશાંશ* center = centers.ptr<દશાંશ>(k);
                    માટે (પૂર્ણાંક j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                માટે (પૂર્ણાંક k = 0; k < K; k++)
                {
                    જો (counters[k] != 0)
                        ચાલુ;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    પૂર્ણાંક max_k = 0;
                    માટે (પૂર્ણાંક k1 = 1; k1 < K; k1++)
                    {
                        જો (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    દ્વિગુણ max_dist = 0;
                    પૂર્ણાંક farthest_i = -1;
                    દશાંશ* base_center = centers.ptr<દશાંશ>(max_k);
                    દશાંશ* _base_center = temp.ptr<દશાંશ>(); // normalized
                    દશાંશ scale = 1.f/counters[max_k];
                    માટે (પૂર્ણાંક j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    માટે (પૂર્ણાંક i = 0; i < N; i++)
                    {
                        જો (labels[i] != max_k)
                            ચાલુ;
                        જો (farthest_i < 0)
                            farthest_i = i;
                        અચલ દશાંશ* sample = data.ptr<દશાંશ>(i);
                        દ્વિગુણ dist = hal::normL2Sqr_(sample, _base_center, dims);

                        જો (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    અચલ દશાંશ* sample = data.ptr<દશાંશ>(farthest_i);
                    દશાંશ* cur_center = centers.ptr<દશાંશ>(k);
                    માટે (પૂર્ણાંક j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                માટે (પૂર્ણાંક k = 0; k < K; k++)
                {
                    દશાંશ* center = centers.ptr<દશાંશ>(k);
                    CV_Assert( counters[k] != 0 );

                    દશાંશ scale = 1.f/counters[k];
                    માટે (પૂર્ણાંક j = 0; j < dims; j++)
                        center[j] *= scale;

                    જો (iter > 0)
                    {
                        દ્વિગુણ dist = 0;
                        અચલ દશાંશ* old_center = old_centers.ptr<દશાંશ>(k);
                        માટે (પૂર્ણાંક j = 0; j < dims; j++)
                        {
                            દ્વિગુણ t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            તાર્કિક isLastIter = (++iter == MAX(criteria.max_ગણતરી, 2) || max_center_shift <= criteria.epsilon);

            જો (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<સાચું>(dists.data(), labels, data, centers), (દ્વિગુણ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                તોડો;
            }
            નહીંતર
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<ખોટું>(dists.data(), labels, data, centers), (દ્વિગુણ)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                જો (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    તોડો;
            }
        }

        જો (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        જો (compactness < best_compactness)
        {
            best_compactness = compactness;
            જો (_centers.needed())
            {
                જો (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                નહીંતર
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        જો (compactness < 0.)
            તોડો;
    }

    પાછા best_compactness;
}
