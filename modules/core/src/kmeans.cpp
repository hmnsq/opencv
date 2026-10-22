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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাব <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

নামাকাশ cv
{

স্থির পূর্ণসংখ্যা CV_KMEANS_PARALLEL_GRANULARITY = (পূর্ণসংখ্যা)utils::get_Configuration_Parameter_আকার_T("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

স্থির শূন্য generateRandomCenter(পূর্ণসংখ্যা dims, ধ্রুবক Vec2f* box, দশমিক* center, RNG& rng)
{
    দশমিক margin = 1.f/dims;
    জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
        center[j] = ((দশমিক)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

শ্রেণী KMeansPPDistanceComputer : সর্বজনীন ParallelLoopBody
{
সর্বজনীন:
    KMeansPPDistanceComputer(দশমিক *tdist2_, ধ্রুবক Mat& তথ্য_2, ধ্রুবক দশমিক *dist_, পূর্ণসংখ্যা ci_) :
        tdist2(tdist2_), তথ্য(তথ্য_2), dist(dist_), ci(ci_)
    { }

    শূন্য operator()( ধ্রুবক cv::Range& range ) ধ্রুবক CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ধ্রুবক পূর্ণসংখ্যা begin = range.start;
        ধ্রুবক পূর্ণসংখ্যা end = range.end;
        ধ্রুবক পূর্ণসংখ্যা dims = তথ্য.cols;

        জন্য (পূর্ণসংখ্যা i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(তথ্য.ptr<দশমিক>(i), তথ্য.ptr<দশমিক>(ci), dims), dist[i]);
        }
    }

ব্যক্তিগত:
    KMeansPPDistanceComputer& operator=(ধ্রুবক KMeansPPDistanceComputer&); // = delete

    দশমিক *tdist2;
    ধ্রুবক Mat& তথ্য;
    ধ্রুবক দশমিক *dist;
    ধ্রুবক পূর্ণসংখ্যা ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
স্থির যৌক্তিক generateCentersPP(ধ্রুবক Mat& তথ্য, Mat& _out_centers,
                              পূর্ণসংখ্যা K, RNG& rng, পূর্ণসংখ্যা trials)
{
    CV_TRACE_FUNCTION();
    ধ্রুবক পূর্ণসংখ্যা dims = তথ্য.cols, N = তথ্য.rows;
    cv::Auto_বাফার<পূর্ণসংখ্যা, 64> _centers(K);
    পূর্ণসংখ্যা* centers = &_centers[0];
    cv::Auto_বাফার<দশমিক, 0> _dist(N*3);
    দশমিক* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    দ্বিগুণ sum0 = 0;

    centers[0] = (unsigned)rng % N;

    জন্য (পূর্ণসংখ্যা i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(তথ্য.ptr<দশমিক>(i), তথ্য.ptr<দশমিক>(centers[0]), dims);
        sum0 += dist[i];
    }
    যদি (!(sum0 < DBL_MAX))
        ফেরত মিথ্যা;

    জন্য (পূর্ণসংখ্যা k = 1; k < K; k++)
    {
        দ্বিগুণ best_যোগ = DBL_MAX;
        পূর্ণসংখ্যা bestCenter = -1;

        জন্য (পূর্ণসংখ্যা j = 0; j < trials; j++)
        {
            দ্বিগুণ p = (দ্বিগুণ)rng*sum0;
            পূর্ণসংখ্যা ci = 0;
            জন্য (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                যদি (p <= 0)
                    বিরতি;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, তথ্য, dist, ci),
                          (দ্বিগুণ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            দ্বিগুণ s = 0;
            পূর্ণসংখ্যা i = 0;
        #যদি CV_ENABLE_UNROLLED
            জন্য (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            জন্য (; i < N; i++)
            {
                s += tdist2[i];
            }

            যদি (s < best_যোগ)
            {
                best_যোগ = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        যদি (bestCenter < 0)
            ফেরত মিথ্যা;
        centers[k] = bestCenter;
        sum0 = best_যোগ;
        std::swap(dist, tdist);
    }

    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
    {
        ধ্রুবক দশমিক* src = তথ্য.ptr<দশমিক>(centers[k]);
        দশমিক* dst = _out_centers.ptr<দশমিক>(k);
        জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    ফেরত সত্য;
}

template<যৌক্তিক onlyDistance>
শ্রেণী KMeansDistanceComputer : সর্বজনীন ParallelLoopBody
{
সর্বজনীন:
    KMeansDistanceComputer( দ্বিগুণ *distances_,
                            পূর্ণসংখ্যা *labels_,
                            ধ্রুবক Mat& তথ্য_2,
                            ধ্রুবক Mat& centers_)
        : distances(distances_),
          labels(labels_),
          তথ্য(তথ্য_2),
          centers(centers_)
    {
    }

    শূন্য operator()(ধ্রুবক Range& range) ধ্রুবক CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        ধ্রুবক পূর্ণসংখ্যা begin = range.start;
        ধ্রুবক পূর্ণসংখ্যা end = range.end;
        ধ্রুবক পূর্ণসংখ্যা K = centers.rows;
        ধ্রুবক পূর্ণসংখ্যা dims = centers.cols;

        জন্য (পূর্ণসংখ্যা i = begin; i < end; ++i)
        {
            ধ্রুবক দশমিক *sample = তথ্য.ptr<দশমিক>(i);
            যদি (onlyDistance)
            {
                ধ্রুবক দশমিক* center = centers.ptr<দশমিক>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                চালু;
            }
            অন্যথা
            {
                পূর্ণসংখ্যা k_best = 0;
                দ্বিগুণ min_dist = DBL_MAX;

                জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
                {
                    ধ্রুবক দশমিক* center = centers.ptr<দশমিক>(k);
                    ধ্রুবক দ্বিগুণ dist = hal::normL2Sqr_(sample, center, dims);
                    যদি (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        বিরতি;
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

ব্যক্তিগত:
    KMeansDistanceComputer& operator=(ধ্রুবক KMeansDistanceComputer&); // = delete

    দ্বিগুণ *distances;
    পূর্ণসংখ্যা *labels;
    ধ্রুবক Mat& তথ্য;
    ধ্রুবক Mat& centers;
};

}

দ্বিগুণ cv::kmeans( প্রবেশ_Array তথ্য_3, পূর্ণসংখ্যা K,
                   প্রবেশ_Output_Array _bestLabels,
                   TermCriteria criteria, পূর্ণসংখ্যা attempts,
                   পূর্ণসংখ্যা flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    ধ্রুবক পূর্ণসংখ্যা SPP_TRIALS = 3;
    Mat data0 = তথ্য_3.getMat();
    ধ্রুবক যৌক্তিক isrow = data0.rows == 1;
    ধ্রুবক পূর্ণসংখ্যা N = isrow ? data0.cols : data0.rows;
    ধ্রুবক পূর্ণসংখ্যা dims = (isrow ? 1 : data0.cols)*data0.channels();
    ধ্রুবক পূর্ণসংখ্যা type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat তথ্য(N, dims, CV_32F, data0.ptr(), isrow ? dims * আকার(দশমিক) : static_cast<size_t>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, সত্য);

    Mat _labels, best_labels = _bestLabels.getMat();
    যদি (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        জন্য (পূর্ণসংখ্যা i = 0; i < N; i++)
        {
            CV_Assert((unsigned)_labels.at<পূর্ণসংখ্যা>(i) < (unsigned)K);
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
    পূর্ণসংখ্যা* labels = _labels.ptr<পূর্ণসংখ্যা>();

    Mat centers(K, dims, type), old_centers(K, dims, type), অস্থায়ী_2(1, dims, type);
    cv::Auto_বাফার<পূর্ণসংখ্যা, 64> counters(K);
    cv::Auto_বাফার<দ্বিগুণ, 64> dists(N);
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

    cv::Auto_বাফার<Vec2f, 64> box(dims);
    যদি (!(flags & KMEANS_PP_CENTERS))
    {
        {
            ধ্রুবক দশমিক* sample = তথ্য.ptr<দশমিক>(0);
            জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        জন্য (পূর্ণসংখ্যা i = 1; i < N; i++)
        {
            ধ্রুবক দশমিক* sample = তথ্য.ptr<দশমিক>(i);
            জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
            {
                দশমিক v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    দ্বিগুণ best_compactness = DBL_MAX;
    জন্য (পূর্ণসংখ্যা a = 0; a < attempts; a++)
    {
        দ্বিগুণ compactness = DBL_MAX;

        জন্য (পূর্ণসংখ্যা iter = 0; ;)
        {
            দ্বিগুণ max_center_shift = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            যদি (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                যদি (flags & KMEANS_PP_CENTERS)
                {
                    যদি (!generateCentersPP(তথ্য, centers, K, rng, SPP_TRIALS))
                        বিরতি;
                }
                অন্যথা
                {
                    জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
                        generateRandomCenter(dims, box.তথ্য(), centers.ptr<দশমিক>(k), rng);
                }
            }
            অন্যথা
            {
                // compute centers
                centers = Scalar(0);
                জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
                    counters[k] = 0;

                জন্য (পূর্ণসংখ্যা i = 0; i < N; i++)
                {
                    ধ্রুবক দশমিক* sample = তথ্য.ptr<দশমিক>(i);
                    পূর্ণসংখ্যা k = labels[i];
                    দশমিক* center = centers.ptr<দশমিক>(k);
                    জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
                {
                    যদি (counters[k] != 0)
                        চালু;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    পূর্ণসংখ্যা max_k = 0;
                    জন্য (পূর্ণসংখ্যা k1 = 1; k1 < K; k1++)
                    {
                        যদি (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    দ্বিগুণ max_dist = 0;
                    পূর্ণসংখ্যা farthest_i = -1;
                    দশমিক* base_center = centers.ptr<দশমিক>(max_k);
                    দশমিক* _base_center = অস্থায়ী_2.ptr<দশমিক>(); // normalized
                    দশমিক scale = 1.f/counters[max_k];
                    জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    জন্য (পূর্ণসংখ্যা i = 0; i < N; i++)
                    {
                        যদি (labels[i] != max_k)
                            চালু;
                        যদি (farthest_i < 0)
                            farthest_i = i;
                        ধ্রুবক দশমিক* sample = তথ্য.ptr<দশমিক>(i);
                        দ্বিগুণ dist = hal::normL2Sqr_(sample, _base_center, dims);

                        যদি (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    ধ্রুবক দশমিক* sample = তথ্য.ptr<দশমিক>(farthest_i);
                    দশমিক* cur_center = centers.ptr<দশমিক>(k);
                    জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                জন্য (পূর্ণসংখ্যা k = 0; k < K; k++)
                {
                    দশমিক* center = centers.ptr<দশমিক>(k);
                    CV_Assert( counters[k] != 0 );

                    দশমিক scale = 1.f/counters[k];
                    জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
                        center[j] *= scale;

                    যদি (iter > 0)
                    {
                        দ্বিগুণ dist = 0;
                        ধ্রুবক দশমিক* old_center = old_centers.ptr<দশমিক>(k);
                        জন্য (পূর্ণসংখ্যা j = 0; j < dims; j++)
                        {
                            দ্বিগুণ t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            যৌক্তিক isLastIter = (++iter == MAX(criteria.max_গুনতি, 2) || max_center_shift <= criteria.epsilon);

            যদি (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<সত্য>(dists.তথ্য(), labels, তথ্য, centers), (দ্বিগুণ)divUp((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = যোগ_2(Mat(আকার_2(N, 1), CV_64F, &dists[0]))[0];
                বিরতি;
            }
            অন্যথা
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<মিথ্যা>(dists.তথ্য(), labels, তথ্য, centers), (দ্বিগুণ)divUp((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                যদি (!(যোগ_2(Mat(আকার_2(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    বিরতি;
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
            বিরতি;
    }

    ফেরত best_compactness;
}
