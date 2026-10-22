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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>
#അന്തര്ഭാവ <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

നാമാകാശ cv
{

സ്ഥിതം പൂർണ്ണസംഖ്യ CV_KMEANS_PARALLEL_GRANULARITY = (പൂർണ്ണസംഖ്യ)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

സ്ഥിതം ശൂന്യം generateRandomCenter(പൂർണ്ണസംഖ്യ dims, സ്ഥിരം Vec2f* box, ദശാംശം* center, RNG& rng)
{
    ദശാംശം margin = 1.f/dims;
    വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
        center[j] = ((ദശാംശം)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

വർഗ്ഗം KMeansPPDistanceComputer : പൊതു ParallelLoopBody
{
പൊതു:
    KMeansPPDistanceComputer(ദശാംശം *tdist2_, സ്ഥിരം Mat& data_, സ്ഥിരം ദശാംശം *dist_, പൂർണ്ണസംഖ്യ ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    ശൂന്യം operator()( സ്ഥിരം cv::Range& range ) സ്ഥിരം CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        സ്ഥിരം പൂർണ്ണസംഖ്യ begin = range.start;
        സ്ഥിരം പൂർണ്ണസംഖ്യ end = range.end;
        സ്ഥിരം പൂർണ്ണസംഖ്യ dims = data.cols;

        വേണ്ടി (പൂർണ്ണസംഖ്യ i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<ദശാംശം>(i), data.ptr<ദശാംശം>(ci), dims), dist[i]);
        }
    }

സ്വകാര്യ:
    KMeansPPDistanceComputer& operator=(സ്ഥിരം KMeansPPDistanceComputer&); // = delete

    ദശാംശം *tdist2;
    സ്ഥിരം Mat& data;
    സ്ഥിരം ദശാംശം *dist;
    സ്ഥിരം പൂർണ്ണസംഖ്യ ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
സ്ഥിതം തർക്കം generateCentersPP(സ്ഥിരം Mat& data, Mat& _out_centers,
                              പൂർണ്ണസംഖ്യ K, RNG& rng, പൂർണ്ണസംഖ്യ trials)
{
    CV_TRACE_FUNCTION();
    സ്ഥിരം പൂർണ്ണസംഖ്യ dims = data.cols, N = data.rows;
    cv::AutoBuffer<പൂർണ്ണസംഖ്യ, 64> _centers(K);
    പൂർണ്ണസംഖ്യ* centers = &_centers[0];
    cv::AutoBuffer<ദശാംശം, 0> _dist(N*3);
    ദശാംശം* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    ഇരട്ട sum0 = 0;

    centers[0] = (unsigned)rng % N;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<ദശാംശം>(i), data.ptr<ദശാംശം>(centers[0]), dims);
        sum0 += dist[i];
    }
    എങ്കിൽ (!(sum0 < DBL_MAX))
        തിരികെ അസത്യം;

    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 1; k < K; k++)
    {
        ഇരട്ട bestSum = DBL_MAX;
        പൂർണ്ണസംഖ്യ bestCenter = -1;

        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < trials; j++)
        {
            ഇരട്ട p = (ഇരട്ട)rng*sum0;
            പൂർണ്ണസംഖ്യ ci = 0;
            വേണ്ടി (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                എങ്കിൽ (p <= 0)
                    നിർത്തുക;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (ഇരട്ട)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            ഇരട്ട s = 0;
            പൂർണ്ണസംഖ്യ i = 0;
        #എങ്കിൽ CV_ENABLE_UNROLLED
            വേണ്ടി (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            വേണ്ടി (; i < N; i++)
            {
                s += tdist2[i];
            }

            എങ്കിൽ (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        എങ്കിൽ (bestCenter < 0)
            തിരികെ അസത്യം;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
    {
        സ്ഥിരം ദശാംശം* src = data.ptr<ദശാംശം>(centers[k]);
        ദശാംശം* dst = _out_centers.ptr<ദശാംശം>(k);
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    തിരികെ സത്യം;
}

template<തർക്കം onlyDistance>
വർഗ്ഗം KMeansDistanceComputer : പൊതു ParallelLoopBody
{
പൊതു:
    KMeansDistanceComputer( ഇരട്ട *distances_,
                            പൂർണ്ണസംഖ്യ *labels_,
                            സ്ഥിരം Mat& data_,
                            സ്ഥിരം Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    ശൂന്യം operator()(സ്ഥിരം Range& range) സ്ഥിരം CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        സ്ഥിരം പൂർണ്ണസംഖ്യ begin = range.start;
        സ്ഥിരം പൂർണ്ണസംഖ്യ end = range.end;
        സ്ഥിരം പൂർണ്ണസംഖ്യ K = centers.rows;
        സ്ഥിരം പൂർണ്ണസംഖ്യ dims = centers.cols;

        വേണ്ടി (പൂർണ്ണസംഖ്യ i = begin; i < end; ++i)
        {
            സ്ഥിരം ദശാംശം *sample = data.ptr<ദശാംശം>(i);
            എങ്കിൽ (onlyDistance)
            {
                സ്ഥിരം ദശാംശം* center = centers.ptr<ദശാംശം>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                തുടരുക;
            }
            അല്ലെങ്കിൽ
            {
                പൂർണ്ണസംഖ്യ k_best = 0;
                ഇരട്ട min_dist = DBL_MAX;

                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
                {
                    സ്ഥിരം ദശാംശം* center = centers.ptr<ദശാംശം>(k);
                    സ്ഥിരം ഇരട്ട dist = hal::normL2Sqr_(sample, center, dims);
                    എങ്കിൽ (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        നിർത്തുക;
                    }

                    എങ്കിൽ (min_dist > dist)
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

സ്വകാര്യ:
    KMeansDistanceComputer& operator=(സ്ഥിരം KMeansDistanceComputer&); // = delete

    ഇരട്ട *distances;
    പൂർണ്ണസംഖ്യ *labels;
    സ്ഥിരം Mat& data;
    സ്ഥിരം Mat& centers;
};

}

ഇരട്ട cv::kmeans( നിവേശം_Array _data, പൂർണ്ണസംഖ്യ K,
                   നിവേശം_Output_Array _bestLabels,
                   TermCriteria criteria, പൂർണ്ണസംഖ്യ attempts,
                   പൂർണ്ണസംഖ്യ flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    സ്ഥിരം പൂർണ്ണസംഖ്യ SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    സ്ഥിരം തർക്കം isrow = data0.rows == 1;
    സ്ഥിരം പൂർണ്ണസംഖ്യ N = isrow ? data0.cols : data0.rows;
    സ്ഥിരം പൂർണ്ണസംഖ്യ dims = (isrow ? 1 : data0.cols)*data0.channels();
    സ്ഥിരം പൂർണ്ണസംഖ്യ type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * വലുപ്പം(ദശാംശം) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, സത്യം);

    Mat _labels, best_labels = _bestLabels.getMat();
    എങ്കിൽ (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<പൂർണ്ണസംഖ്യ>(i) < (unsigned)K);
        }
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    പൂർണ്ണസംഖ്യ* labels = _labels.ptr<പൂർണ്ണസംഖ്യ>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<പൂർണ്ണസംഖ്യ, 64> counters(K);
    cv::AutoBuffer<ഇരട്ട, 64> dists(N);
    RNG& rng = theRNG();

    എങ്കിൽ (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    അല്ലെങ്കിൽ
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    എങ്കിൽ (criteria.type & TermCriteria::എണ്ണം_3)
        criteria.max_എണ്ണം = std::min(std::max(criteria.max_എണ്ണം, 2), 100);
    അല്ലെങ്കിൽ
        criteria.max_എണ്ണം = 100;

    എങ്കിൽ (K == 1)
    {
        attempts = 1;
        criteria.max_എണ്ണം = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    എങ്കിൽ (!(flags & KMEANS_PP_CENTERS))
    {
        {
            സ്ഥിരം ദശാംശം* sample = data.ptr<ദശാംശം>(0);
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < N; i++)
        {
            സ്ഥിരം ദശാംശം* sample = data.ptr<ദശാംശം>(i);
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
            {
                ദശാംശം v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    ഇരട്ട best_compactness = DBL_MAX;
    വേണ്ടി (പൂർണ്ണസംഖ്യ a = 0; a < attempts; a++)
    {
        ഇരട്ട compactness = DBL_MAX;

        വേണ്ടി (പൂർണ്ണസംഖ്യ iter = 0; ;)
        {
            ഇരട്ട max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            എങ്കിൽ (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                എങ്കിൽ (flags & KMEANS_PP_CENTERS)
                {
                    എങ്കിൽ (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        നിർത്തുക;
                }
                അല്ലെങ്കിൽ
                {
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<ദശാംശം>(k), rng);
                }
            }
            അല്ലെങ്കിൽ
            {
                // compute centers
                centers = Scalar(0);
                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
                    counters[k] = 0;

                വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < N; i++)
                {
                    സ്ഥിരം ദശാംശം* sample = data.ptr<ദശാംശം>(i);
                    പൂർണ്ണസംഖ്യ k = labels[i];
                    ദശാംശം* center = centers.ptr<ദശാംശം>(k);
                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
                {
                    എങ്കിൽ (counters[k] != 0)
                        തുടരുക;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    പൂർണ്ണസംഖ്യ max_k = 0;
                    വേണ്ടി (പൂർണ്ണസംഖ്യ k1 = 1; k1 < K; k1++)
                    {
                        എങ്കിൽ (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    ഇരട്ട max_dist = 0;
                    പൂർണ്ണസംഖ്യ farthest_i = -1;
                    ദശാംശം* base_center = centers.ptr<ദശാംശം>(max_k);
                    ദശാംശം* _base_center = temp.ptr<ദശാംശം>(); // normalized
                    ദശാംശം scale = 1.f/counters[max_k];
                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < N; i++)
                    {
                        എങ്കിൽ (labels[i] != max_k)
                            തുടരുക;
                        എങ്കിൽ (farthest_i < 0)
                            farthest_i = i;
                        സ്ഥിരം ദശാംശം* sample = data.ptr<ദശാംശം>(i);
                        ഇരട്ട dist = hal::normL2Sqr_(sample, _base_center, dims);

                        എങ്കിൽ (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    സ്ഥിരം ദശാംശം* sample = data.ptr<ദശാംശം>(farthest_i);
                    ദശാംശം* cur_center = centers.ptr<ദശാംശം>(k);
                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < K; k++)
                {
                    ദശാംശം* center = centers.ptr<ദശാംശം>(k);
                    CV_Assert( counters[k] != 0 );

                    ദശാംശം scale = 1.f/counters[k];
                    വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
                        center[j] *= scale;

                    എങ്കിൽ (iter > 0)
                    {
                        ഇരട്ട dist = 0;
                        സ്ഥിരം ദശാംശം* old_center = old_centers.ptr<ദശാംശം>(k);
                        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < dims; j++)
                        {
                            ഇരട്ട t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            തർക്കം isLastIter = (++iter == MAX(criteria.max_എണ്ണം, 2) || max_center_shift <= criteria.epsilon);

            എങ്കിൽ (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<സത്യം>(dists.data(), labels, data, centers), (ഇരട്ട)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                നിർത്തുക;
            }
            അല്ലെങ്കിൽ
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<അസത്യം>(dists.data(), labels, data, centers), (ഇരട്ട)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                എങ്കിൽ (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    നിർത്തുക;
            }
        }

        എങ്കിൽ (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        എങ്കിൽ (compactness < best_compactness)
        {
            best_compactness = compactness;
            എങ്കിൽ (_centers.needed())
            {
                എങ്കിൽ (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                അല്ലെങ്കിൽ
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        എങ്കിൽ (compactness < 0.)
            നിർത്തുക;
    }

    തിരികെ best_compactness;
}
