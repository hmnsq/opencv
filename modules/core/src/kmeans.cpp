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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

नामाकाश cv
{

स्थिर पूर्णाङ्क CV_KMEANS_PARALLEL_GRANULARITY = (पूर्णाङ्क)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

स्थिर शून्य generateRandomCenter(पूर्णाङ्क dims, ध्रुव Vec2f* box, भिन्न* center, RNG& rng)
{
    भिन्न margin = 1.f/dims;
    प्रति (पूर्णाङ्क j = 0; j < dims; j++)
        center[j] = ((भिन्न)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

वर्ग KMeansPPDistanceComputer : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    KMeansPPDistanceComputer(भिन्न *tdist2_, ध्रुव Mat& data_, ध्रुव भिन्न *dist_, पूर्णाङ्क ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    शून्य operator()( ध्रुव cv::Range& range ) ध्रुव CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ध्रुव पूर्णाङ्क begin = range.start;
        ध्रुव पूर्णाङ्क end = range.end;
        ध्रुव पूर्णाङ्क dims = data.cols;

        प्रति (पूर्णाङ्क i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<भिन्न>(i), data.ptr<भिन्न>(ci), dims), dist[i]);
        }
    }

गुप्त:
    KMeansPPDistanceComputer& operator=(ध्रुव KMeansPPDistanceComputer&); // = delete

    भिन्न *tdist2;
    ध्रुव Mat& data;
    ध्रुव भिन्न *dist;
    ध्रुव पूर्णाङ्क ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
स्थिर तार्किक generateCentersPP(ध्रुव Mat& data, Mat& _out_centers,
                              पूर्णाङ्क K, RNG& rng, पूर्णाङ्क trials)
{
    CV_TRACE_FUNCTION();
    ध्रुव पूर्णाङ्क dims = data.cols, N = data.rows;
    cv::AutoBuffer<पूर्णाङ्क, 64> _centers(K);
    पूर्णाङ्क* centers = &_centers[0];
    cv::AutoBuffer<भिन्न, 0> _dist(N*3);
    भिन्न* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    द्विगुण sum0 = 0;

    centers[0] = (unsigned)rng % N;

    प्रति (पूर्णाङ्क i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<भिन्न>(i), data.ptr<भिन्न>(centers[0]), dims);
        sum0 += dist[i];
    }
    जइ (!(sum0 < DBL_MAX))
        प्रत्यागम मिच्छ;

    प्रति (पूर्णाङ्क k = 1; k < K; k++)
    {
        द्विगुण bestSum = DBL_MAX;
        पूर्णाङ्क bestCenter = -1;

        प्रति (पूर्णाङ्क j = 0; j < trials; j++)
        {
            द्विगुण p = (द्विगुण)rng*sum0;
            पूर्णाङ्क ci = 0;
            प्रति (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                जइ (p <= 0)
                    भङ्ग;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (द्विगुण)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            द्विगुण s = 0;
            पूर्णाङ्क i = 0;
        #जइ CV_ENABLE_UNROLLED
            प्रति (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            प्रति (; i < N; i++)
            {
                s += tdist2[i];
            }

            जइ (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        जइ (bestCenter < 0)
            प्रत्यागम मिच्छ;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    प्रति (पूर्णाङ्क k = 0; k < K; k++)
    {
        ध्रुव भिन्न* src = data.ptr<भिन्न>(centers[k]);
        भिन्न* dst = _out_centers.ptr<भिन्न>(k);
        प्रति (पूर्णाङ्क j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    प्रत्यागम सच्च;
}

template<तार्किक onlyDistance>
वर्ग KMeansDistanceComputer : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    KMeansDistanceComputer( द्विगुण *distances_,
                            पूर्णाङ्क *labels_,
                            ध्रुव Mat& data_,
                            ध्रुव Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    शून्य operator()(ध्रुव Range& range) ध्रुव CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ध्रुव पूर्णाङ्क begin = range.start;
        ध्रुव पूर्णाङ्क end = range.end;
        ध्रुव पूर्णाङ्क K = centers.rows;
        ध्रुव पूर्णाङ्क dims = centers.cols;

        प्रति (पूर्णाङ्क i = begin; i < end; ++i)
        {
            ध्रुव भिन्न *sample = data.ptr<भिन्न>(i);
            जइ (onlyDistance)
            {
                ध्रुव भिन्न* center = centers.ptr<भिन्न>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                अनुवर्त;
            }
            अण्णहा
            {
                पूर्णाङ्क k_best = 0;
                द्विगुण min_dist = DBL_MAX;

                प्रति (पूर्णाङ्क k = 0; k < K; k++)
                {
                    ध्रुव भिन्न* center = centers.ptr<भिन्न>(k);
                    ध्रुव द्विगुण dist = hal::normL2Sqr_(sample, center, dims);
                    जइ (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        भङ्ग;
                    }

                    जइ (min_dist > dist)
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

गुप्त:
    KMeansDistanceComputer& operator=(ध्रुव KMeansDistanceComputer&); // = delete

    द्विगुण *distances;
    पूर्णाङ्क *labels;
    ध्रुव Mat& data;
    ध्रुव Mat& centers;
};

}

द्विगुण cv::kmeans( निवेशः_Array _data, पूर्णाङ्क K,
                   निवेशः_Output_Array _bestLabels,
                   TermCriteria criteria, पूर्णाङ्क attempts,
                   पूर्णाङ्क flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ध्रुव पूर्णाङ्क SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    ध्रुव तार्किक isrow = data0.rows == 1;
    ध्रुव पूर्णाङ्क N = isrow ? data0.cols : data0.rows;
    ध्रुव पूर्णाङ्क dims = (isrow ? 1 : data0.cols)*data0.channels();
    ध्रुव पूर्णाङ्क type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * परिमाण(भिन्न) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, सच्च);

    Mat _labels, best_labels = _bestLabels.getMat();
    जइ (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        प्रति (पूर्णाङ्क i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<पूर्णाङ्क>(i) < (unsigned)K);
        }
    }
    अण्णहा
    {
        जइ (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    पूर्णाङ्क* labels = _labels.ptr<पूर्णाङ्क>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<पूर्णाङ्क, 64> counters(K);
    cv::AutoBuffer<द्विगुण, 64> dists(N);
    RNG& rng = theRNG();

    जइ (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    अण्णहा
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    जइ (criteria.type & TermCriteria::गणनम्_3)
        criteria.max_गणनम् = std::min(std::max(criteria.max_गणनम्, 2), 100);
    अण्णहा
        criteria.max_गणनम् = 100;

    जइ (K == 1)
    {
        attempts = 1;
        criteria.max_गणनम् = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    जइ (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ध्रुव भिन्न* sample = data.ptr<भिन्न>(0);
            प्रति (पूर्णाङ्क j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        प्रति (पूर्णाङ्क i = 1; i < N; i++)
        {
            ध्रुव भिन्न* sample = data.ptr<भिन्न>(i);
            प्रति (पूर्णाङ्क j = 0; j < dims; j++)
            {
                भिन्न v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    द्विगुण best_compactness = DBL_MAX;
    प्रति (पूर्णाङ्क a = 0; a < attempts; a++)
    {
        द्विगुण compactness = DBL_MAX;

        प्रति (पूर्णाङ्क iter = 0; ;)
        {
            द्विगुण max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            जइ (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                जइ (flags & KMEANS_PP_CENTERS)
                {
                    जइ (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        भङ्ग;
                }
                अण्णहा
                {
                    प्रति (पूर्णाङ्क k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<भिन्न>(k), rng);
                }
            }
            अण्णहा
            {
                // compute centers
                centers = Scalar(0);
                प्रति (पूर्णाङ्क k = 0; k < K; k++)
                    counters[k] = 0;

                प्रति (पूर्णाङ्क i = 0; i < N; i++)
                {
                    ध्रुव भिन्न* sample = data.ptr<भिन्न>(i);
                    पूर्णाङ्क k = labels[i];
                    भिन्न* center = centers.ptr<भिन्न>(k);
                    प्रति (पूर्णाङ्क j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                प्रति (पूर्णाङ्क k = 0; k < K; k++)
                {
                    जइ (counters[k] != 0)
                        अनुवर्त;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    पूर्णाङ्क max_k = 0;
                    प्रति (पूर्णाङ्क k1 = 1; k1 < K; k1++)
                    {
                        जइ (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    द्विगुण max_dist = 0;
                    पूर्णाङ्क farthest_i = -1;
                    भिन्न* base_center = centers.ptr<भिन्न>(max_k);
                    भिन्न* _base_center = temp.ptr<भिन्न>(); // normalized
                    भिन्न scale = 1.f/counters[max_k];
                    प्रति (पूर्णाङ्क j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    प्रति (पूर्णाङ्क i = 0; i < N; i++)
                    {
                        जइ (labels[i] != max_k)
                            अनुवर्त;
                        जइ (farthest_i < 0)
                            farthest_i = i;
                        ध्रुव भिन्न* sample = data.ptr<भिन्न>(i);
                        द्विगुण dist = hal::normL2Sqr_(sample, _base_center, dims);

                        जइ (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ध्रुव भिन्न* sample = data.ptr<भिन्न>(farthest_i);
                    भिन्न* cur_center = centers.ptr<भिन्न>(k);
                    प्रति (पूर्णाङ्क j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                प्रति (पूर्णाङ्क k = 0; k < K; k++)
                {
                    भिन्न* center = centers.ptr<भिन्न>(k);
                    CV_Assert( counters[k] != 0 );

                    भिन्न scale = 1.f/counters[k];
                    प्रति (पूर्णाङ्क j = 0; j < dims; j++)
                        center[j] *= scale;

                    जइ (iter > 0)
                    {
                        द्विगुण dist = 0;
                        ध्रुव भिन्न* old_center = old_centers.ptr<भिन्न>(k);
                        प्रति (पूर्णाङ्क j = 0; j < dims; j++)
                        {
                            द्विगुण t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            तार्किक isLastIter = (++iter == MAX(criteria.max_गणनम्, 2) || max_center_shift <= criteria.epsilon);

            जइ (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<सच्च>(dists.data(), labels, data, centers), (द्विगुण)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                भङ्ग;
            }
            अण्णहा
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<मिच्छ>(dists.data(), labels, data, centers), (द्विगुण)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                जइ (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    भङ्ग;
            }
        }

        जइ (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        जइ (compactness < best_compactness)
        {
            best_compactness = compactness;
            जइ (_centers.needed())
            {
                जइ (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                अण्णहा
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        जइ (compactness < 0.)
            भङ्ग;
    }

    प्रत्यागम best_compactness;
}
