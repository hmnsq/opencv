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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>
#அந்தர்பாவ <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

நாமாகாஶ cv
{

நிலையான முழு CV_KMEANS_PARALLEL_GRANULARITY = (முழு)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

நிலையான வெறுமை generateRandomCenter(முழு dims, மாறிலி Vec2f* box, தசம* center, RNG& rng)
{
    தசம margin = 1.f/dims;
    க்கு (முழு j = 0; j < dims; j++)
        center[j] = ((தசம)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

வகுப்பு KMeansPPDistanceComputer : பொது ParallelLoopBody
{
பொது:
    KMeansPPDistanceComputer(தசம *tdist2_, மாறிலி Mat& data_, மாறிலி தசம *dist_, முழு ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    வெறுமை operator()( மாறிலி cv::Range& range ) மாறிலி CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        மாறிலி முழு begin = range.start;
        மாறிலி முழு end = range.end;
        மாறிலி முழு dims = data.cols;

        க்கு (முழு i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<தசம>(i), data.ptr<தசம>(ci), dims), dist[i]);
        }
    }

தனிப்பட்ட:
    KMeansPPDistanceComputer& operator=(மாறிலி KMeansPPDistanceComputer&); // = delete

    தசம *tdist2;
    மாறிலி Mat& data;
    மாறிலி தசம *dist;
    மாறிலி முழு ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
நிலையான தருக்க generateCentersPP(மாறிலி Mat& data, Mat& _out_centers,
                              முழு K, RNG& rng, முழு trials)
{
    CV_TRACE_FUNCTION();
    மாறிலி முழு dims = data.cols, N = data.rows;
    cv::AutoBuffer<முழு, 64> _centers(K);
    முழு* centers = &_centers[0];
    cv::AutoBuffer<தசம, 0> _dist(N*3);
    தசம* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    இரட்டை sum0 = 0;

    centers[0] = (unsigned)rng % N;

    க்கு (முழு i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<தசம>(i), data.ptr<தசம>(centers[0]), dims);
        sum0 += dist[i];
    }
    எனில் (!(sum0 < DBL_MAX))
        திரும்பு பொய்;

    க்கு (முழு k = 1; k < K; k++)
    {
        இரட்டை bestSum = DBL_MAX;
        முழு bestCenter = -1;

        க்கு (முழு j = 0; j < trials; j++)
        {
            இரட்டை p = (இரட்டை)rng*sum0;
            முழு ci = 0;
            க்கு (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                எனில் (p <= 0)
                    நிறுத்து;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (இரட்டை)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            இரட்டை s = 0;
            முழு i = 0;
        #எனில் CV_ENABLE_UNROLLED
            க்கு (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            க்கு (; i < N; i++)
            {
                s += tdist2[i];
            }

            எனில் (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        எனில் (bestCenter < 0)
            திரும்பு பொய்;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    க்கு (முழு k = 0; k < K; k++)
    {
        மாறிலி தசம* src = data.ptr<தசம>(centers[k]);
        தசம* dst = _out_centers.ptr<தசம>(k);
        க்கு (முழு j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    திரும்பு உண்மை;
}

template<தருக்க onlyDistance>
வகுப்பு KMeansDistanceComputer : பொது ParallelLoopBody
{
பொது:
    KMeansDistanceComputer( இரட்டை *distances_,
                            முழு *labels_,
                            மாறிலி Mat& data_,
                            மாறிலி Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    வெறுமை operator()(மாறிலி Range& range) மாறிலி CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        மாறிலி முழு begin = range.start;
        மாறிலி முழு end = range.end;
        மாறிலி முழு K = centers.rows;
        மாறிலி முழு dims = centers.cols;

        க்கு (முழு i = begin; i < end; ++i)
        {
            மாறிலி தசம *sample = data.ptr<தசம>(i);
            எனில் (onlyDistance)
            {
                மாறிலி தசம* center = centers.ptr<தசம>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                தொடர்;
            }
            இல்லையெனில்
            {
                முழு k_best = 0;
                இரட்டை min_dist = DBL_MAX;

                க்கு (முழு k = 0; k < K; k++)
                {
                    மாறிலி தசம* center = centers.ptr<தசம>(k);
                    மாறிலி இரட்டை dist = hal::normL2Sqr_(sample, center, dims);
                    எனில் (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        நிறுத்து;
                    }

                    எனில் (min_dist > dist)
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

தனிப்பட்ட:
    KMeansDistanceComputer& operator=(மாறிலி KMeansDistanceComputer&); // = delete

    இரட்டை *distances;
    முழு *labels;
    மாறிலி Mat& data;
    மாறிலி Mat& centers;
};

}

இரட்டை cv::kmeans( உள்ளீடு_Array _data, முழு K,
                   உள்ளீடு_Output_Array _bestLabels,
                   TermCriteria criteria, முழு attempts,
                   முழு flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    மாறிலி முழு SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    மாறிலி தருக்க isrow = data0.rows == 1;
    மாறிலி முழு N = isrow ? data0.cols : data0.rows;
    மாறிலி முழு dims = (isrow ? 1 : data0.cols)*data0.channels();
    மாறிலி முழு type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * அளவு(தசம) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, உண்மை);

    Mat _labels, best_labels = _bestLabels.getMat();
    எனில் (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        க்கு (முழு i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<முழு>(i) < (unsigned)K);
        }
    }
    இல்லையெனில்
    {
        எனில் (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    முழு* labels = _labels.ptr<முழு>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<முழு, 64> counters(K);
    cv::AutoBuffer<இரட்டை, 64> dists(N);
    RNG& rng = theRNG();

    எனில் (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    இல்லையெனில்
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    எனில் (criteria.type & TermCriteria::மொத்தம்_3)
        criteria.max_மொத்தம் = std::min(std::max(criteria.max_மொத்தம், 2), 100);
    இல்லையெனில்
        criteria.max_மொத்தம் = 100;

    எனில் (K == 1)
    {
        attempts = 1;
        criteria.max_மொத்தம் = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    எனில் (!(flags & KMEANS_PP_CENTERS))
    {
        {
            மாறிலி தசம* sample = data.ptr<தசம>(0);
            க்கு (முழு j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        க்கு (முழு i = 1; i < N; i++)
        {
            மாறிலி தசம* sample = data.ptr<தசம>(i);
            க்கு (முழு j = 0; j < dims; j++)
            {
                தசம v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    இரட்டை best_compactness = DBL_MAX;
    க்கு (முழு a = 0; a < attempts; a++)
    {
        இரட்டை compactness = DBL_MAX;

        க்கு (முழு iter = 0; ;)
        {
            இரட்டை max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            எனில் (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                எனில் (flags & KMEANS_PP_CENTERS)
                {
                    எனில் (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        நிறுத்து;
                }
                இல்லையெனில்
                {
                    க்கு (முழு k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<தசம>(k), rng);
                }
            }
            இல்லையெனில்
            {
                // compute centers
                centers = Scalar(0);
                க்கு (முழு k = 0; k < K; k++)
                    counters[k] = 0;

                க்கு (முழு i = 0; i < N; i++)
                {
                    மாறிலி தசம* sample = data.ptr<தசம>(i);
                    முழு k = labels[i];
                    தசம* center = centers.ptr<தசம>(k);
                    க்கு (முழு j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                க்கு (முழு k = 0; k < K; k++)
                {
                    எனில் (counters[k] != 0)
                        தொடர்;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    முழு max_k = 0;
                    க்கு (முழு k1 = 1; k1 < K; k1++)
                    {
                        எனில் (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    இரட்டை max_dist = 0;
                    முழு farthest_i = -1;
                    தசம* base_center = centers.ptr<தசம>(max_k);
                    தசம* _base_center = temp.ptr<தசம>(); // normalized
                    தசம scale = 1.f/counters[max_k];
                    க்கு (முழு j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    க்கு (முழு i = 0; i < N; i++)
                    {
                        எனில் (labels[i] != max_k)
                            தொடர்;
                        எனில் (farthest_i < 0)
                            farthest_i = i;
                        மாறிலி தசம* sample = data.ptr<தசம>(i);
                        இரட்டை dist = hal::normL2Sqr_(sample, _base_center, dims);

                        எனில் (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    மாறிலி தசம* sample = data.ptr<தசம>(farthest_i);
                    தசம* cur_center = centers.ptr<தசம>(k);
                    க்கு (முழு j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                க்கு (முழு k = 0; k < K; k++)
                {
                    தசம* center = centers.ptr<தசம>(k);
                    CV_Assert( counters[k] != 0 );

                    தசம scale = 1.f/counters[k];
                    க்கு (முழு j = 0; j < dims; j++)
                        center[j] *= scale;

                    எனில் (iter > 0)
                    {
                        இரட்டை dist = 0;
                        மாறிலி தசம* old_center = old_centers.ptr<தசம>(k);
                        க்கு (முழு j = 0; j < dims; j++)
                        {
                            இரட்டை t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            தருக்க isLastIter = (++iter == MAX(criteria.max_மொத்தம், 2) || max_center_shift <= criteria.epsilon);

            எனில் (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<உண்மை>(dists.data(), labels, data, centers), (இரட்டை)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                நிறுத்து;
            }
            இல்லையெனில்
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<பொய்>(dists.data(), labels, data, centers), (இரட்டை)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                எனில் (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    நிறுத்து;
            }
        }

        எனில் (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        எனில் (compactness < best_compactness)
        {
            best_compactness = compactness;
            எனில் (_centers.needed())
            {
                எனில் (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                இல்லையெனில்
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        எனில் (compactness < 0.)
            நிறுத்து;
    }

    திரும்பு best_compactness;
}
