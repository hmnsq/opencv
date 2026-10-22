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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>
#అన్తర్భావ <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

నామాకాశ cv
{

స్థిర పూర్ణాంకం CV_KMEANS_PARALLEL_GRANULARITY = (పూర్ణాంకం)utils::get_Configuration_Parameter_కొలత_T("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

స్థిర శూన్య generateRandomCenter(పూర్ణాంకం dims, స్థిరరాశి Vec2f* box, దశాంశం* center, RNG& rng)
{
    దశాంశం margin = 1.f/dims;
    కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
        center[j] = ((దశాంశం)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

తరగతి KMeansPPDistanceComputer : బహిరంగ ParallelLoopBody
{
బహిరంగ:
    KMeansPPDistanceComputer(దశాంశం *tdist2_, స్థిరరాశి Mat& దత్తాంశం_2, స్థిరరాశి దశాంశం *dist_, పూర్ణాంకం ci_) :
        tdist2(tdist2_), దత్తాంశం(దత్తాంశం_2), dist(dist_), ci(ci_)
    { }

    శూన్య క్రియాకారకం()( స్థిరరాశి cv::Range& range ) స్థిరరాశి CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        స్థిరరాశి పూర్ణాంకం begin = range.ప్రారంభం;
        స్థిరరాశి పూర్ణాంకం end = range.end;
        స్థిరరాశి పూర్ణాంకం dims = దత్తాంశం.నిలువుల_సంఖ్య;

        కొరకు (పూర్ణాంకం i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(దత్తాంశం.ptr<దశాంశం>(i), దత్తాంశం.ptr<దశాంశం>(ci), dims), dist[i]);
        }
    }

అంతరంగ:
    KMeansPPDistanceComputer& క్రియాకారకం=(స్థిరరాశి KMeansPPDistanceComputer&); // = delete

    దశాంశం *tdist2;
    స్థిరరాశి Mat& దత్తాంశం;
    స్థిరరాశి దశాంశం *dist;
    స్థిరరాశి పూర్ణాంకం ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
స్థిర తార్కిక generateCentersPP(స్థిరరాశి Mat& దత్తాంశం, Mat& బహిర్గతం_centers,
                              పూర్ణాంకం K, RNG& rng, పూర్ణాంకం trials)
{
    CV_TRACE_FUNCTION();
    స్థిరరాశి పూర్ణాంకం dims = దత్తాంశం.నిలువుల_సంఖ్య, N = దత్తాంశం.అడ్డువరుసలు;
    cv::Auto_బఫరు<పూర్ణాంకం, 64> _centers(K);
    పూర్ణాంకం* centers = &_centers[0];
    cv::Auto_బఫరు<దశాంశం, 0> _dist(N*3);
    దశాంశం* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    ద్విగుణ sum0 = 0;

    centers[0] = (గుర్తులేని)rng % N;

    కొరకు (పూర్ణాంకం i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(దత్తాంశం.ptr<దశాంశం>(i), దత్తాంశం.ptr<దశాంశం>(centers[0]), dims);
        sum0 += dist[i];
    }
    ఒకవేళ (!(sum0 < DBL_MAX))
        తిరిగి అబద్ధం;

    కొరకు (పూర్ణాంకం k = 1; k < K; k++)
    {
        ద్విగుణ ఉత్తమ_కూడిక = DBL_MAX;
        పూర్ణాంకం ఉత్తమ_Center = -1;

        కొరకు (పూర్ణాంకం j = 0; j < trials; j++)
        {
            ద్విగుణ p = (ద్విగుణ)rng*sum0;
            పూర్ణాంకం ci = 0;
            కొరకు (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                ఒకవేళ (p <= 0)
                    ఆపు;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, దత్తాంశం, dist, ci),
                          (ద్విగుణ)div_పైకి((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            ద్విగుణ s = 0;
            పూర్ణాంకం i = 0;
        #ఒకవేళ CV_ENABLE_UNROLLED
            కొరకు (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #అయితే_ముగింపు
            కొరకు (; i < N; i++)
            {
                s += tdist2[i];
            }

            ఒకవేళ (s < ఉత్తమ_కూడిక)
            {
                ఉత్తమ_కూడిక = s;
                ఉత్తమ_Center = ci;
                std::swap(tdist, tdist2);
            }
        }
        ఒకవేళ (ఉత్తమ_Center < 0)
            తిరిగి అబద్ధం;
        centers[k] = ఉత్తమ_Center;
        sum0 = ఉత్తమ_కూడిక;
        std::swap(dist, tdist);
    }

    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
    {
        స్థిరరాశి దశాంశం* src = దత్తాంశం.ptr<దశాంశం>(centers[k]);
        దశాంశం* dst = బహిర్గతం_centers.ptr<దశాంశం>(k);
        కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    తిరిగి నిజం;
}

మూస<తార్కిక only_దూరం>
తరగతి KMeans_దూరం_Computer : బహిరంగ ParallelLoopBody
{
బహిరంగ:
    KMeans_దూరం_Computer( ద్విగుణ *distances_,
                            పూర్ణాంకం *labels_,
                            స్థిరరాశి Mat& దత్తాంశం_2,
                            స్థిరరాశి Mat& centers_)
        : distances(distances_),
          labels(labels_),
          దత్తాంశం(దత్తాంశం_2),
          centers(centers_)
    {
    }

    శూన్య క్రియాకారకం()(స్థిరరాశి Range& range) స్థిరరాశి CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        స్థిరరాశి పూర్ణాంకం begin = range.ప్రారంభం;
        స్థిరరాశి పూర్ణాంకం end = range.end;
        స్థిరరాశి పూర్ణాంకం K = centers.అడ్డువరుసలు;
        స్థిరరాశి పూర్ణాంకం dims = centers.నిలువుల_సంఖ్య;

        కొరకు (పూర్ణాంకం i = begin; i < end; ++i)
        {
            స్థిరరాశి దశాంశం *sample = దత్తాంశం.ptr<దశాంశం>(i);
            ఒకవేళ (only_దూరం)
            {
                స్థిరరాశి దశాంశం* center = centers.ptr<దశాంశం>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                కొనసాగు;
            }
            లేకపోతే
            {
                పూర్ణాంకం k_ఉత్తమ = 0;
                ద్విగుణ min_dist = DBL_MAX;

                కొరకు (పూర్ణాంకం k = 0; k < K; k++)
                {
                    స్థిరరాశి దశాంశం* center = centers.ptr<దశాంశం>(k);
                    స్థిరరాశి ద్విగుణ dist = hal::normL2Sqr_(sample, center, dims);
                    ఒకవేళ (!(dist <= FLT_MAX))
                    {
                        min_dist = DBL_MAX;
                        ఆపు;
                    }

                    ఒకవేళ (min_dist > dist)
                    {
                        min_dist = dist;
                        k_ఉత్తమ = k;
                    }
                }

                distances[i] = min_dist;
                labels[i] = k_ఉత్తమ;
            }
        }
    }

అంతరంగ:
    KMeans_దూరం_Computer& క్రియాకారకం=(స్థిరరాశి KMeans_దూరం_Computer&); // = delete

    ద్విగుణ *distances;
    పూర్ణాంకం *labels;
    స్థిరరాశి Mat& దత్తాంశం;
    స్థిరరాశి Mat& centers;
};

}

ద్విగుణ cv::kmeans( ఆదానం_Array దత్తాంశం_3, పూర్ణాంకం K,
                   ఆదానం_Output_Array ఉత్తమ_Labels,
                   పదం_Criteria criteria, పూర్ణాంకం attempts,
                   పూర్ణాంకం flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    స్థిరరాశి పూర్ణాంకం SPP_TRIALS = 3;
    Mat data0 = దత్తాంశం_3.getMat();
    స్థిరరాశి తార్కిక isrow = data0.అడ్డువరుసలు == 1;
    స్థిరరాశి పూర్ణాంకం N = isrow ? data0.నిలువుల_సంఖ్య : data0.అడ్డువరుసలు;
    స్థిరరాశి పూర్ణాంకం dims = (isrow ? 1 : data0.నిలువుల_సంఖ్య)*data0.channels();
    స్థిరరాశి పూర్ణాంకం type = data0.లోతు();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat దత్తాంశం(N, dims, CV_32F, data0.ptr(), isrow ? dims * పరిమాణం(దశాంశం) : static_cast<size_t>(data0.అడుగు));

    ఉత్తమ_Labels.create(N, 1, CV_32S, -1, నిజం);

    Mat _labels, ఉత్తమ_labels = ఉత్తమ_Labels.getMat();
    ఒకవేళ (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (ఉత్తమ_labels.నిలువుల_సంఖ్య == 1 || ఉత్తమ_labels.అడ్డువరుసలు == 1) &&
                  ఉత్తమ_labels.నిలువుల_సంఖ్య*ఉత్తమ_labels.అడ్డువరుసలు == N &&
                  ఉత్తమ_labels.type() == CV_32S &&
                  ఉత్తమ_labels.isContinuous());
        ఉత్తమ_labels.reshape(1, N).copy_గమ్యం(_labels);
        కొరకు (పూర్ణాంకం i = 0; i < N; i++)
        {
            CV_Assert((గుర్తులేని)_labels.at<పూర్ణాంకం>(i) < (గుర్తులేని)K);
        }
    }
    లేకపోతే
    {
        ఒకవేళ (!((ఉత్తమ_labels.నిలువుల_సంఖ్య == 1 || ఉత్తమ_labels.అడ్డువరుసలు == 1) &&
             ఉత్తమ_labels.నిలువుల_సంఖ్య*ఉత్తమ_labels.అడ్డువరుసలు == N &&
             ఉత్తమ_labels.type() == CV_32S &&
             ఉత్తమ_labels.isContinuous()))
        {
            ఉత్తమ_Labels.create(N, 1, CV_32S);
            ఉత్తమ_labels = ఉత్తమ_Labels.getMat();
        }
        _labels.create(ఉత్తమ_labels.size(), ఉత్తమ_labels.type());
    }
    పూర్ణాంకం* labels = _labels.ptr<పూర్ణాంకం>();

    Mat centers(K, dims, type), old_centers(K, dims, type), తాత్కాలిక_2(1, dims, type);
    cv::Auto_బఫరు<పూర్ణాంకం, 64> counters(K);
    cv::Auto_బఫరు<ద్విగుణ, 64> dists(N);
    RNG& rng = theRNG();

    ఒకవేళ (criteria.type & పదం_Criteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    లేకపోతే
        criteria.epsilon = FLT_EPSILON;
    criteria.epsilon *= criteria.epsilon;

    ఒకవేళ (criteria.type & పదం_Criteria::లెక్క_3)
        criteria.max_లెక్క = std::min(std::max(criteria.max_లెక్క, 2), 100);
    లేకపోతే
        criteria.max_లెక్క = 100;

    ఒకవేళ (K == 1)
    {
        attempts = 1;
        criteria.max_లెక్క = 2;
    }

    cv::Auto_బఫరు<Vec2f, 64> box(dims);
    ఒకవేళ (!(flags & KMEANS_PP_CENTERS))
    {
        {
            స్థిరరాశి దశాంశం* sample = దత్తాంశం.ptr<దశాంశం>(0);
            కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        కొరకు (పూర్ణాంకం i = 1; i < N; i++)
        {
            స్థిరరాశి దశాంశం* sample = దత్తాంశం.ptr<దశాంశం>(i);
            కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
            {
                దశాంశం v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    ద్విగుణ ఉత్తమ_compactness = DBL_MAX;
    కొరకు (పూర్ణాంకం a = 0; a < attempts; a++)
    {
        ద్విగుణ compactness = DBL_MAX;

        కొరకు (పూర్ణాంకం iter = 0; ;)
        {
            ద్విగుణ max_center_జరుపు = iter == 0 ? DBL_MAX : 0.0;

            swap(centers, old_centers);

            ఒకవేళ (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                ఒకవేళ (flags & KMEANS_PP_CENTERS)
                {
                    ఒకవేళ (!generateCentersPP(దత్తాంశం, centers, K, rng, SPP_TRIALS))
                        ఆపు;
                }
                లేకపోతే
                {
                    కొరకు (పూర్ణాంకం k = 0; k < K; k++)
                        generateRandomCenter(dims, box.దత్తాంశం(), centers.ptr<దశాంశం>(k), rng);
                }
            }
            లేకపోతే
            {
                // compute centers
                centers = Scalar(0);
                కొరకు (పూర్ణాంకం k = 0; k < K; k++)
                    counters[k] = 0;

                కొరకు (పూర్ణాంకం i = 0; i < N; i++)
                {
                    స్థిరరాశి దశాంశం* sample = దత్తాంశం.ptr<దశాంశం>(i);
                    పూర్ణాంకం k = labels[i];
                    దశాంశం* center = centers.ptr<దశాంశం>(k);
                    కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                కొరకు (పూర్ణాంకం k = 0; k < K; k++)
                {
                    ఒకవేళ (counters[k] != 0)
                        కొనసాగు;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    పూర్ణాంకం max_k = 0;
                    కొరకు (పూర్ణాంకం k1 = 1; k1 < K; k1++)
                    {
                        ఒకవేళ (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    ద్విగుణ max_dist = 0;
                    పూర్ణాంకం farthest_i = -1;
                    దశాంశం* పునాది_center = centers.ptr<దశాంశం>(max_k);
                    దశాంశం* పునాది_center_2 = తాత్కాలిక_2.ptr<దశాంశం>(); // normalized
                    దశాంశం scale = 1.f/counters[max_k];
                    కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
                        పునాది_center_2[j] = పునాది_center[j]*scale;

                    కొరకు (పూర్ణాంకం i = 0; i < N; i++)
                    {
                        ఒకవేళ (labels[i] != max_k)
                            కొనసాగు;
                        ఒకవేళ (farthest_i < 0)
                            farthest_i = i;
                        స్థిరరాశి దశాంశం* sample = దత్తాంశం.ptr<దశాంశం>(i);
                        ద్విగుణ dist = hal::normL2Sqr_(sample, పునాది_center_2, dims);

                        ఒకవేళ (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    స్థిరరాశి దశాంశం* sample = దత్తాంశం.ptr<దశాంశం>(farthest_i);
                    దశాంశం* cur_center = centers.ptr<దశాంశం>(k);
                    కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
                    {
                        పునాది_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                కొరకు (పూర్ణాంకం k = 0; k < K; k++)
                {
                    దశాంశం* center = centers.ptr<దశాంశం>(k);
                    CV_Assert( counters[k] != 0 );

                    దశాంశం scale = 1.f/counters[k];
                    కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
                        center[j] *= scale;

                    ఒకవేళ (iter > 0)
                    {
                        ద్విగుణ dist = 0;
                        స్థిరరాశి దశాంశం* old_center = old_centers.ptr<దశాంశం>(k);
                        కొరకు (పూర్ణాంకం j = 0; j < dims; j++)
                        {
                            ద్విగుణ t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_జరుపు = std::max(max_center_జరుపు, dist);
                    }
                }
            }

            తార్కిక is_చివరి_Iter = (++iter == MAX(criteria.max_లెక్క, 2) || max_center_జరుపు <= criteria.epsilon);

            ఒకవేళ (is_చివరి_Iter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeans_దూరం_Computer<నిజం>(dists.దత్తాంశం(), labels, దత్తాంశం, centers), (ద్విగుణ)div_పైకి((size_t)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = కూడిక(Mat(కొలత(N, 1), CV_64F, &dists[0]))[0];
                ఆపు;
            }
            లేకపోతే
            {
                // assign labels
                parallel_for_(Range(0, N), KMeans_దూరం_Computer<అబద్ధం>(dists.దత్తాంశం(), labels, దత్తాంశం, centers), (ద్విగుణ)div_పైకి((size_t)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                ఒకవేళ (!(కూడిక(Mat(కొలత(N, 1), CV_64F, &dists[0]))[0] < DBL_MAX))
                    ఆపు;
            }
        }

        ఒకవేళ (!(compactness < DBL_MAX))
        {
            centers.సమితి_Zero();
            _labels.సమితి_Zero();
            compactness = -1.;
        }
        ఒకవేళ (compactness < ఉత్తమ_compactness)
        {
            ఉత్తమ_compactness = compactness;
            ఒకవేళ (_centers.needed())
            {
                ఒకవేళ (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copy_గమ్యం(_centers);
                లేకపోతే
                    centers.copy_గమ్యం(_centers);
            }
            _labels.copy_గమ్యం(ఉత్తమ_labels);
        }
        ఒకవేళ (compactness < 0.)
            ఆపు;
    }

    తిరిగి ఉత్తమ_compactness;
}
