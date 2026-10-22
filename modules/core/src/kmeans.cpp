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

#অন্তর্ভাৱ "precomp.hpp"
#অন্তর্ভাৱ <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাৱ <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

নামাকাশ cv
{

স্থির পূর্ণাঙ্ক CV_KMEANS_PARALLEL_GRANULARITY = (পূর্ণাঙ্ক)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

স্থির শূন্য generateRandomCenter(পূর্ণাঙ্ক dims, ধ্রুৱ Vec2f* box, ভিন্ন* center, RNG& rng)
{
    ভিন্ন margin = 1.f/dims;
    বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
        center[j] = ((ভিন্ন)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

শ্ৰেণী KMeansPPDistanceComputer : সার্ৱজনিক ParallelLoopBody
{
সার্ৱজনিক:
    KMeansPPDistanceComputer(ভিন্ন *tdist2_, ধ্রুৱ Mat& data_, ধ্রুৱ ভিন্ন *dist_, পূর্ণাঙ্ক ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    শূন্য operator()( ধ্রুৱ cv::Range& range ) ধ্রুৱ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ধ্রুৱ পূর্ণাঙ্ক begin = range.start;
        ধ্রুৱ পূর্ণাঙ্ক end = range.end;
        ধ্রুৱ পূর্ণাঙ্ক dims = data.cols;

        বাবে (পূর্ণাঙ্ক i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<ভিন্ন>(i), data.ptr<ভিন্ন>(ci), dims), dist[i]);
        }
    }

গুপ্ত:
    KMeansPPDistanceComputer& operator=(ধ্রুৱ KMeansPPDistanceComputer&); // = delete

    ভিন্ন *tdist2;
    ধ্রুৱ Mat& data;
    ধ্রুৱ ভিন্ন *dist;
    ধ্রুৱ পূর্ণাঙ্ক ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
স্থির তার্কিক generateCentersPP(ধ্রুৱ Mat& data, Mat& _out_centers,
                              পূর্ণাঙ্ক K, RNG& rng, পূর্ণাঙ্ক trials)
{
    CV_TRACE_FUNCTION();
    ধ্রুৱ পূর্ণাঙ্ক dims = data.cols, N = data.rows;
    cv::AutoBuffer<পূর্ণাঙ্ক, 64> _centers(K);
    পূর্ণাঙ্ক* centers = &_centers[0];
    cv::AutoBuffer<ভিন্ন, 0> _dist(N*3);
    ভিন্ন* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    দ্ৱিগুণ sum0 = 0;

    centers[0] = (unsigned)rng % N;

    বাবে (পূর্ণাঙ্ক i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<ভিন্ন>(i), data.ptr<ভিন্ন>(centers[0]), dims);
        sum0 += dist[i];
    }
    যদি (!(sum0 < DBL_MAX))
        ঘূৰাই মিছা;

    বাবে (পূর্ণাঙ্ক k = 1; k < K; k++)
    {
        দ্ৱিগুণ bestSum = DBL_MAX;
        পূর্ণাঙ্ক bestCenter = -1;

        বাবে (পূর্ণাঙ্ক j = 0; j < trials; j++)
        {
            দ্ৱিগুণ p = (দ্ৱিগুণ)rng*sum0;
            পূর্ণাঙ্ক ci = 0;
            বাবে (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                যদি (p <= 0)
                    ভঙ্গ;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (দ্ৱিগুণ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            দ্ৱিগুণ s = 0;
            পূর্ণাঙ্ক i = 0;
        #যদি CV_ENABLE_UNROLLED
            বাবে (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            বাবে (; i < N; i++)
            {
                s += tdist2[i];
            }

            যদি (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        যদি (bestCenter < 0)
            ঘূৰাই মিছা;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
    {
        ধ্রুৱ ভিন্ন* src = data.ptr<ভিন্ন>(centers[k]);
        ভিন্ন* dst = _out_centers.ptr<ভিন্ন>(k);
        বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    ঘূৰাই সঁচা;
}

template<তার্কিক onlyDistance>
শ্ৰেণী KMeansDistanceComputer : সার্ৱজনিক ParallelLoopBody
{
সার্ৱজনিক:
    KMeansDistanceComputer( দ্ৱিগুণ *distances_,
                            পূর্ণাঙ্ক *labels_,
                            ধ্রুৱ Mat& data_,
                            ধ্রুৱ Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    শূন্য operator()(ধ্রুৱ Range& range) ধ্রুৱ CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ধ্রুৱ পূর্ণাঙ্ক begin = range.start;
        ধ্রুৱ পূর্ণাঙ্ক end = range.end;
        ধ্রুৱ পূর্ণাঙ্ক K = centers.rows;
        ধ্রুৱ পূর্ণাঙ্ক dims = centers.cols;

        বাবে (পূর্ণাঙ্ক i = begin; i < end; ++i)
        {
            ধ্রুৱ ভিন্ন *sample = data.ptr<ভিন্ন>(i);
            যদি (onlyDistance)
            {
                ধ্রুৱ ভিন্ন* center = centers.ptr<ভিন্ন>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                অনুৱর্ত;
            }
            অন্যথা
            {
                পূর্ণাঙ্ক k_best = 0;
                দ্ৱিগুণ min_dist = DBL_MAX;

                বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
                {
                    ধ্রুৱ ভিন্ন* center = centers.ptr<ভিন্ন>(k);
                    ধ্রুৱ দ্ৱিগুণ dist = hal::normL2Sqr_(sample, center, dims);
                    যদি (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        ভঙ্গ;
                    }

                    যদি (min_dist > dist)
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

গুপ্ত:
    KMeansDistanceComputer& operator=(ধ্রুৱ KMeansDistanceComputer&); // = delete

    দ্ৱিগুণ *distances;
    পূর্ণাঙ্ক *labels;
    ধ্রুৱ Mat& data;
    ধ্রুৱ Mat& centers;
};

}

দ্ৱিগুণ cv::kmeans( প্ৰবেশ_Array _data, পূর্ণাঙ্ক K,
                   প্ৰবেশ_Output_Array _bestLabels,
                   TermCriteria criteria, পূর্ণাঙ্ক attempts,
                   পূর্ণাঙ্ক flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ধ্রুৱ পূর্ণাঙ্ক SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    ধ্রুৱ তার্কিক isrow = data0.rows == 1;
    ধ্রুৱ পূর্ণাঙ্ক N = isrow ? data0.cols : data0.rows;
    ধ্রুৱ পূর্ণাঙ্ক dims = (isrow ? 1 : data0.cols)*data0.channels();
    ধ্রুৱ পূর্ণাঙ্ক type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * পরিমাণ(ভিন্ন) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, সঁচা);

    Mat _labels, best_labels = _bestLabels.getMat();
    যদি (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        বাবে (পূর্ণাঙ্ক i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<পূর্ণাঙ্ক>(i) < (unsigned)K);
        }
    }
    অন্যথা
    {
        যদি (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    পূর্ণাঙ্ক* labels = _labels.ptr<পূর্ণাঙ্ক>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<পূর্ণাঙ্ক, 64> counters(K);
    cv::AutoBuffer<দ্ৱিগুণ, 64> dists(N);
    RNG& rng = theRNG();

    যদি (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    অন্যথা
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    যদি (criteria.type & TermCriteria::গুনতি_3)
        criteria.max_গুনতি = std::min(std::max(criteria.max_গুনতি, 2), 100);
    অন্যথা
        criteria.max_গুনতি = 100;

    যদি (K == 1)
    {
        attempts = 1;
        criteria.max_গুনতি = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    যদি (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ধ্রুৱ ভিন্ন* sample = data.ptr<ভিন্ন>(0);
            বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        বাবে (পূর্ণাঙ্ক i = 1; i < N; i++)
        {
            ধ্রুৱ ভিন্ন* sample = data.ptr<ভিন্ন>(i);
            বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
            {
                ভিন্ন v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    দ্ৱিগুণ best_compactness = DBL_MAX;
    বাবে (পূর্ণাঙ্ক a = 0; a < attempts; a++)
    {
        দ্ৱিগুণ compactness = DBL_MAX;

        বাবে (পূর্ণাঙ্ক iter = 0; ;)
        {
            দ্ৱিগুণ max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            যদি (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                যদি (flags & KMEANS_PP_CENTERS)
                {
                    যদি (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        ভঙ্গ;
                }
                অন্যথা
                {
                    বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<ভিন্ন>(k), rng);
                }
            }
            অন্যথা
            {
                // compute centers
                centers = Scalar(0);
                বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
                    counters[k] = 0;

                বাবে (পূর্ণাঙ্ক i = 0; i < N; i++)
                {
                    ধ্রুৱ ভিন্ন* sample = data.ptr<ভিন্ন>(i);
                    পূর্ণাঙ্ক k = labels[i];
                    ভিন্ন* center = centers.ptr<ভিন্ন>(k);
                    বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
                {
                    যদি (counters[k] != 0)
                        অনুৱর্ত;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    পূর্ণাঙ্ক max_k = 0;
                    বাবে (পূর্ণাঙ্ক k1 = 1; k1 < K; k1++)
                    {
                        যদি (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    দ্ৱিগুণ max_dist = 0;
                    পূর্ণাঙ্ক farthest_i = -1;
                    ভিন্ন* base_center = centers.ptr<ভিন্ন>(max_k);
                    ভিন্ন* _base_center = temp.ptr<ভিন্ন>(); // normalized
                    ভিন্ন scale = 1.f/counters[max_k];
                    বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    বাবে (পূর্ণাঙ্ক i = 0; i < N; i++)
                    {
                        যদি (labels[i] != max_k)
                            অনুৱর্ত;
                        যদি (farthest_i < 0)
                            farthest_i = i;
                        ধ্রুৱ ভিন্ন* sample = data.ptr<ভিন্ন>(i);
                        দ্ৱিগুণ dist = hal::normL2Sqr_(sample, _base_center, dims);

                        যদি (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ধ্রুৱ ভিন্ন* sample = data.ptr<ভিন্ন>(farthest_i);
                    ভিন্ন* cur_center = centers.ptr<ভিন্ন>(k);
                    বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                বাবে (পূর্ণাঙ্ক k = 0; k < K; k++)
                {
                    ভিন্ন* center = centers.ptr<ভিন্ন>(k);
                    CV_Assert( counters[k] != 0 );

                    ভিন্ন scale = 1.f/counters[k];
                    বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
                        center[j] *= scale;

                    যদি (iter > 0)
                    {
                        দ্ৱিগুণ dist = 0;
                        ধ্রুৱ ভিন্ন* old_center = old_centers.ptr<ভিন্ন>(k);
                        বাবে (পূর্ণাঙ্ক j = 0; j < dims; j++)
                        {
                            দ্ৱিগুণ t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            তার্কিক isLastIter = (++iter == MAX(criteria.max_গুনতি, 2) || max_center_shift <= criteria.epsilon);

            যদি (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<সঁচা>(dists.data(), labels, data, centers), (দ্ৱিগুণ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                ভঙ্গ;
            }
            অন্যথা
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<মিছা>(dists.data(), labels, data, centers), (দ্ৱিগুণ)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                যদি (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    ভঙ্গ;
            }
        }

        যদি (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        যদি (compactness < best_compactness)
        {
            best_compactness = compactness;
            যদি (_centers.needed())
            {
                যদি (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                অন্যথা
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        যদি (compactness < 0.)
            ভঙ্গ;
    }

    ঘূৰাই best_compactness;
}
