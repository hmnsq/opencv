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

#شامل "precomp.hpp"
#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

فضاینام cv
{

ایستا صحیح CV_KMEANS_PARALLEL_GRANULARITY = (صحیح)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

ایستا تهی generateRandomCenter(صحیح dims, ثابت Vec2f* box, اعشاری* center, RNG& rng)
{
    اعشاری margin = 1.f/dims;
    برای (صحیح j = 0; j < dims; j++)
        center[j] = ((اعشاری)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

رده KMeansPPDistanceComputer : عمومی ParallelLoopBody
{
عمومی:
    KMeansPPDistanceComputer(اعشاری *tdist2_, ثابت Mat& data_, ثابت اعشاری *dist_, صحیح ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    تهی operator()( ثابت cv::Range& range ) ثابت CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ثابت صحیح begin = range.start;
        ثابت صحیح end = range.end;
        ثابت صحیح dims = data.cols;

        برای (صحیح i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<اعشاری>(i), data.ptr<اعشاری>(ci), dims), dist[i]);
        }
    }

خصوصی:
    KMeansPPDistanceComputer& operator=(ثابت KMeansPPDistanceComputer&); // = delete

    اعشاری *tdist2;
    ثابت Mat& data;
    ثابت اعشاری *dist;
    ثابت صحیح ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
ایستا منطقی generateCentersPP(ثابت Mat& data, Mat& _out_centers,
                              صحیح K, RNG& rng, صحیح trials)
{
    CV_TRACE_FUNCTION();
    ثابت صحیح dims = data.cols, N = data.rows;
    cv::AutoBuffer<صحیح, 64> _centers(K);
    صحیح* centers = &_centers[0];
    cv::AutoBuffer<اعشاری, 0> _dist(N*3);
    اعشاری* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    دوگانه sum0 = 0;

    centers[0] = (unsigned)rng % N;

    برای (صحیح i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<اعشاری>(i), data.ptr<اعشاری>(centers[0]), dims);
        sum0 += dist[i];
    }
    اگر (!(sum0 < DBL_MAX))
        بازگشت نادرست;

    برای (صحیح k = 1; k < K; k++)
    {
        دوگانه bestSum = DBL_MAX;
        صحیح bestCenter = -1;

        برای (صحیح j = 0; j < trials; j++)
        {
            دوگانه p = (دوگانه)rng*sum0;
            صحیح ci = 0;
            برای (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                اگر (p <= 0)
                    بشکن;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (دوگانه)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            دوگانه s = 0;
            صحیح i = 0;
        #اگر CV_ENABLE_UNROLLED
            برای (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            برای (; i < N; i++)
            {
                s += tdist2[i];
            }

            اگر (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        اگر (bestCenter < 0)
            بازگشت نادرست;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    برای (صحیح k = 0; k < K; k++)
    {
        ثابت اعشاری* src = data.ptr<اعشاری>(centers[k]);
        اعشاری* dst = _out_centers.ptr<اعشاری>(k);
        برای (صحیح j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    بازگشت درست;
}

template<منطقی onlyDistance>
رده KMeansDistanceComputer : عمومی ParallelLoopBody
{
عمومی:
    KMeansDistanceComputer( دوگانه *distances_,
                            صحیح *labels_,
                            ثابت Mat& data_,
                            ثابت Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    تهی operator()(ثابت Range& range) ثابت CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ثابت صحیح begin = range.start;
        ثابت صحیح end = range.end;
        ثابت صحیح K = centers.rows;
        ثابت صحیح dims = centers.cols;

        برای (صحیح i = begin; i < end; ++i)
        {
            ثابت اعشاری *sample = data.ptr<اعشاری>(i);
            اگر (onlyDistance)
            {
                ثابت اعشاری* center = centers.ptr<اعشاری>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                ادامه;
            }
            وگرنه
            {
                صحیح k_best = 0;
                دوگانه min_dist = DBL_MAX;

                برای (صحیح k = 0; k < K; k++)
                {
                    ثابت اعشاری* center = centers.ptr<اعشاری>(k);
                    ثابت دوگانه dist = hal::normL2Sqr_(sample, center, dims);
                    اگر (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        بشکن;
                    }

                    اگر (min_dist > dist)
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

خصوصی:
    KMeansDistanceComputer& operator=(ثابت KMeansDistanceComputer&); // = delete

    دوگانه *distances;
    صحیح *labels;
    ثابت Mat& data;
    ثابت Mat& centers;
};

}

دوگانه cv::kmeans( درون_داد_Array _data, صحیح K,
                   درون_داد_Output_Array _bestLabels,
                   TermCriteria criteria, صحیح attempts,
                   صحیح flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ثابت صحیح SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    ثابت منطقی isrow = data0.rows == 1;
    ثابت صحیح N = isrow ? data0.cols : data0.rows;
    ثابت صحیح dims = (isrow ? 1 : data0.cols)*data0.channels();
    ثابت صحیح type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * اندازه(اعشاری) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, درست);

    Mat _labels, best_labels = _bestLabels.getMat();
    اگر (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        برای (صحیح i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<صحیح>(i) < (unsigned)K);
        }
    }
    وگرنه
    {
        اگر (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    صحیح* labels = _labels.ptr<صحیح>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<صحیح, 64> counters(K);
    cv::AutoBuffer<دوگانه, 64> dists(N);
    RNG& rng = theRNG();

    اگر (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    وگرنه
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    اگر (criteria.type & TermCriteria::شمار_3)
        criteria.max_شمار = std::min(std::max(criteria.max_شمار, 2), 100);
    وگرنه
        criteria.max_شمار = 100;

    اگر (K == 1)
    {
        attempts = 1;
        criteria.max_شمار = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    اگر (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ثابت اعشاری* sample = data.ptr<اعشاری>(0);
            برای (صحیح j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        برای (صحیح i = 1; i < N; i++)
        {
            ثابت اعشاری* sample = data.ptr<اعشاری>(i);
            برای (صحیح j = 0; j < dims; j++)
            {
                اعشاری v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    دوگانه best_compactness = DBL_MAX;
    برای (صحیح a = 0; a < attempts; a++)
    {
        دوگانه compactness = DBL_MAX;

        برای (صحیح iter = 0; ;)
        {
            دوگانه max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            اگر (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                اگر (flags & KMEANS_PP_CENTERS)
                {
                    اگر (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        بشکن;
                }
                وگرنه
                {
                    برای (صحیح k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<اعشاری>(k), rng);
                }
            }
            وگرنه
            {
                // compute centers
                centers = Scalar(0);
                برای (صحیح k = 0; k < K; k++)
                    counters[k] = 0;

                برای (صحیح i = 0; i < N; i++)
                {
                    ثابت اعشاری* sample = data.ptr<اعشاری>(i);
                    صحیح k = labels[i];
                    اعشاری* center = centers.ptr<اعشاری>(k);
                    برای (صحیح j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                برای (صحیح k = 0; k < K; k++)
                {
                    اگر (counters[k] != 0)
                        ادامه;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    صحیح max_k = 0;
                    برای (صحیح k1 = 1; k1 < K; k1++)
                    {
                        اگر (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    دوگانه max_dist = 0;
                    صحیح farthest_i = -1;
                    اعشاری* base_center = centers.ptr<اعشاری>(max_k);
                    اعشاری* _base_center = temp.ptr<اعشاری>(); // normalized
                    اعشاری scale = 1.f/counters[max_k];
                    برای (صحیح j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    برای (صحیح i = 0; i < N; i++)
                    {
                        اگر (labels[i] != max_k)
                            ادامه;
                        اگر (farthest_i < 0)
                            farthest_i = i;
                        ثابت اعشاری* sample = data.ptr<اعشاری>(i);
                        دوگانه dist = hal::normL2Sqr_(sample, _base_center, dims);

                        اگر (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ثابت اعشاری* sample = data.ptr<اعشاری>(farthest_i);
                    اعشاری* cur_center = centers.ptr<اعشاری>(k);
                    برای (صحیح j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                برای (صحیح k = 0; k < K; k++)
                {
                    اعشاری* center = centers.ptr<اعشاری>(k);
                    CV_Assert( counters[k] != 0 );

                    اعشاری scale = 1.f/counters[k];
                    برای (صحیح j = 0; j < dims; j++)
                        center[j] *= scale;

                    اگر (iter > 0)
                    {
                        دوگانه dist = 0;
                        ثابت اعشاری* old_center = old_centers.ptr<اعشاری>(k);
                        برای (صحیح j = 0; j < dims; j++)
                        {
                            دوگانه t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            منطقی isLastIter = (++iter == MAX(criteria.max_شمار, 2) || max_center_shift <= criteria.epsilon);

            اگر (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<درست>(dists.data(), labels, data, centers), (دوگانه)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                بشکن;
            }
            وگرنه
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<نادرست>(dists.data(), labels, data, centers), (دوگانه)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                اگر (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    بشکن;
            }
        }

        اگر (!(compactness < DBL_MAX))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        اگر (compactness < best_compactness)
        {
            best_compactness = compactness;
            اگر (_centers.needed())
            {
                اگر (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                وگرنه
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        اگر (compactness < 0.)
            بشکن;
    }

    بازگشت best_compactness;
}
