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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

ନାମାକାଶ cv
{

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା CV_KMEANS_PARALLEL_GRANULARITY = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

ସ୍ଥିର ଶୂନ୍ୟ generateRandomCenter(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ Vec2f* box, ଦଶମିକ* center, RNG& rng)
{
    ଦଶମିକ margin = 1.f/dims;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
        center[j] = ((ଦଶମିକ)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

ଶ୍ରେଣୀ KMeansPPDistanceComputer : ସର୍ବସାଧାରଣ ParallelLoopBody
{
ସର୍ବସାଧାରଣ:
    KMeansPPDistanceComputer(ଦଶମିକ *tdist2_, ଅଚଳ Mat& data_, ଅଚଳ ଦଶମିକ *dist_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    ଶୂନ୍ୟ operator()( ଅଚଳ cv::Range& range ) ଅଚଳ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା begin = range.start;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = range.end;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = data.cols;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<ଦଶମିକ>(i), data.ptr<ଦଶମିକ>(ci), dims), dist[i]);
        }
    }

ବ୍ୟକ୍ତିଗତ:
    KMeansPPDistanceComputer& operator=(ଅଚଳ KMeansPPDistanceComputer&); // = delete

    ଦଶମିକ *tdist2;
    ଅଚଳ Mat& data;
    ଅଚଳ ଦଶମିକ *dist;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
ସ୍ଥିର ତାର୍କିକ generateCentersPP(ଅଚଳ Mat& data, Mat& _out_centers,
                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, RNG& rng, ପୂର୍ଣ୍ଣସଂଖ୍ୟା trials)
{
    CV_TRACE_FUNCTION();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = data.cols, N = data.rows;
    cv::AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 64> _centers(K);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* centers = &_centers[0];
    cv::AutoBuffer<ଦଶମିକ, 0> _dist(N*3);
    ଦଶମିକ* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    ଦ୍ୱିଗୁଣ sum0 = 0;

    centers[0] = (unsigned)rng % N;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<ଦଶମିକ>(i), data.ptr<ଦଶମିକ>(centers[0]), dims);
        sum0 += dist[i];
    }
    ଯଦି (!(sum0 < DBL_MAX))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 1; k < K; k++)
    {
        ଦ୍ୱିଗୁଣ bestSum = DBL_MAX;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା bestCenter = -1;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < trials; j++)
        {
            ଦ୍ୱିଗୁଣ p = (ଦ୍ୱିଗୁଣ)rng*sum0;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ci = 0;
            ପାଇଁ (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                ଯଦି (p <= 0)
                    ଭାଙ୍ଗ;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (ଦ୍ୱିଗୁଣ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            ଦ୍ୱିଗୁଣ s = 0;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
        #ଯଦି CV_ENABLE_UNROLLED
            ପାଇଁ (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            ପାଇଁ (; i < N; i++)
            {
                s += tdist2[i];
            }

            ଯଦି (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        ଯଦି (bestCenter < 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
    {
        ଅଚଳ ଦଶମିକ* src = data.ptr<ଦଶମିକ>(centers[k]);
        ଦଶମିକ* dst = _out_centers.ptr<ଦଶମିକ>(k);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

template<ତାର୍କିକ onlyDistance>
ଶ୍ରେଣୀ KMeansDistanceComputer : ସର୍ବସାଧାରଣ ParallelLoopBody
{
ସର୍ବସାଧାରଣ:
    KMeansDistanceComputer( ଦ୍ୱିଗୁଣ *distances_,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା *labels_,
                            ଅଚଳ Mat& data_,
                            ଅଚଳ Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    ଶୂନ୍ୟ operator()(ଅଚଳ Range& range) ଅଚଳ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା begin = range.start;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = range.end;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା K = centers.rows;
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = centers.cols;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = begin; i < end; ++i)
        {
            ଅଚଳ ଦଶମିକ *sample = data.ptr<ଦଶମିକ>(i);
            ଯଦି (onlyDistance)
            {
                ଅଚଳ ଦଶମିକ* center = centers.ptr<ଦଶମିକ>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                ଜାରି;
            }
            ଅନ୍ୟଥା
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା k_best = 0;
                ଦ୍ୱିଗୁଣ min_dist = DBL_MAX;

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
                {
                    ଅଚଳ ଦଶମିକ* center = centers.ptr<ଦଶମିକ>(k);
                    ଅଚଳ ଦ୍ୱିଗୁଣ dist = hal::normL2Sqr_(sample, center, dims);
                    ଯଦି (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        ଭାଙ୍ଗ;
                    }

                    ଯଦି (min_dist > dist)
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

ବ୍ୟକ୍ତିଗତ:
    KMeansDistanceComputer& operator=(ଅଚଳ KMeansDistanceComputer&); // = delete

    ଦ୍ୱିଗୁଣ *distances;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *labels;
    ଅଚଳ Mat& data;
    ଅଚଳ Mat& centers;
};

}

ଦ୍ୱିଗୁଣ cv::kmeans( ନିବେଶ_Array _data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K,
                   ନିବେଶ_Output_Array _bestLabels,
                   TermCriteria criteria, ପୂର୍ଣ୍ଣସଂଖ୍ୟା attempts,
                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    ଅଚଳ ତାର୍କିକ isrow = data0.rows == 1;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା N = isrow ? data0.cols : data0.rows;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = (isrow ? 1 : data0.cols)*data0.channels();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * ଆକାର(ଦଶମିକ) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, ସତ୍ୟ);

    Mat _labels, best_labels = _bestLabels.getMat();
    ଯଦି (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i) < (unsigned)K);
        }
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* labels = _labels.ptr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 64> counters(K);
    cv::AutoBuffer<ଦ୍ୱିଗୁଣ, 64> dists(N);
    RNG& rng = theRNG();

    ଯଦି (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    ଅନ୍ୟଥା
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    ଯଦି (criteria.type & TermCriteria::ଗଣନା_4)
        criteria.max_ଗଣନା = std::min(std::max(criteria.max_ଗଣନା, 2), 100);
    ଅନ୍ୟଥା
        criteria.max_ଗଣନା = 100;

    ଯଦି (K == 1)
    {
        attempts = 1;
        criteria.max_ଗଣନା = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    ଯଦି (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ଅଚଳ ଦଶମିକ* sample = data.ptr<ଦଶମିକ>(0);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < N; i++)
        {
            ଅଚଳ ଦଶମିକ* sample = data.ptr<ଦଶମିକ>(i);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
            {
                ଦଶମିକ v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    ଦ୍ୱିଗୁଣ best_compactness = DBL_MAX;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = 0; a < attempts; a++)
    {
        ଦ୍ୱିଗୁଣ compactness = DBL_MAX;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା iter = 0; ;)
        {
            ଦ୍ୱିଗୁଣ max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            ଯଦି (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                ଯଦି (flags & KMEANS_PP_CENTERS)
                {
                    ଯଦି (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        ଭାଙ୍ଗ;
                }
                ଅନ୍ୟଥା
                {
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<ଦଶମିକ>(k), rng);
                }
            }
            ଅନ୍ୟଥା
            {
                // compute centers
                centers = Scalar(0);
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
                    counters[k] = 0;

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N; i++)
                {
                    ଅଚଳ ଦଶମିକ* sample = data.ptr<ଦଶମିକ>(i);
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = labels[i];
                    ଦଶମିକ* center = centers.ptr<ଦଶମିକ>(k);
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
                {
                    ଯଦି (counters[k] != 0)
                        ଜାରି;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_k = 0;
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k1 = 1; k1 < K; k1++)
                    {
                        ଯଦି (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    ଦ୍ୱିଗୁଣ max_dist = 0;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା farthest_i = -1;
                    ଦଶମିକ* base_center = centers.ptr<ଦଶମିକ>(max_k);
                    ଦଶମିକ* _base_center = temp.ptr<ଦଶମିକ>(); // normalized
                    ଦଶମିକ scale = 1.f/counters[max_k];
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < N; i++)
                    {
                        ଯଦି (labels[i] != max_k)
                            ଜାରି;
                        ଯଦି (farthest_i < 0)
                            farthest_i = i;
                        ଅଚଳ ଦଶମିକ* sample = data.ptr<ଦଶମିକ>(i);
                        ଦ୍ୱିଗୁଣ dist = hal::normL2Sqr_(sample, _base_center, dims);

                        ଯଦି (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ଅଚଳ ଦଶମିକ* sample = data.ptr<ଦଶମିକ>(farthest_i);
                    ଦଶମିକ* cur_center = centers.ptr<ଦଶମିକ>(k);
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < K; k++)
                {
                    ଦଶମିକ* center = centers.ptr<ଦଶମିକ>(k);
                    CV_Assert( counters[k] != 0 );

                    ଦଶମିକ scale = 1.f/counters[k];
                    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
                        center[j] *= scale;

                    ଯଦି (iter > 0)
                    {
                        ଦ୍ୱିଗୁଣ dist = 0;
                        ଅଚଳ ଦଶମିକ* old_center = old_centers.ptr<ଦଶମିକ>(k);
                        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < dims; j++)
                        {
                            ଦ୍ୱିଗୁଣ t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            ତାର୍କିକ isLastIter = (++iter == MAX(criteria.max_ଗଣନା, 2) || max_center_shift <= criteria.epsilon);

            ଯଦି (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<ସତ୍ୟ>(dists.data(), labels, data, centers), (ଦ୍ୱିଗୁଣ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                ଭାଙ୍ଗ;
            }
            ଅନ୍ୟଥା
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<ମିଥ୍ୟା>(dists.data(), labels, data, centers), (ଦ୍ୱିଗୁଣ)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                ଯଦି (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    ଭାଙ୍ଗ;
            }
        }

        ଯଦି (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        ଯଦି (compactness < best_compactness)
        {
            best_compactness = compactness;
            ଯଦି (_centers.needed())
            {
                ଯଦି (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                ଅନ୍ୟଥା
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        ଯଦି (compactness < 0.)
            ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ best_compactness;
}
