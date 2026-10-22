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

स्थिर पूर्णांक CV_KMEANS_PARALLEL_GRANULARITY = (पूर्णांक)utils::getConfigurationParameterSizeT("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

स्थिर शून्य generateRandomCenter(पूर्णांक dims, स्थिरांक Vec2f* box, दशांश* center, RNG& rng)
{
    दशांश margin = 1.f/dims;
    साठी (पूर्णांक j = 0; j < dims; j++)
        center[j] = ((दशांश)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

वर्ग KMeansPPDistanceComputer : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    KMeansPPDistanceComputer(दशांश *tdist2_, स्थिरांक Mat& data_, स्थिरांक दशांश *dist_, पूर्णांक ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    शून्य चालक()( स्थिरांक cv::Range& range ) स्थिरांक CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        स्थिरांक पूर्णांक begin = range.start;
        स्थिरांक पूर्णांक end = range.end;
        स्थिरांक पूर्णांक dims = data.cols;

        साठी (पूर्णांक i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<दशांश>(i), data.ptr<दशांश>(ci), dims), dist[i]);
        }
    }

खाजगी:
    KMeansPPDistanceComputer& चालक=(स्थिरांक KMeansPPDistanceComputer&); // = delete

    दशांश *tdist2;
    स्थिरांक Mat& data;
    स्थिरांक दशांश *dist;
    स्थिरांक पूर्णांक ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
स्थिर तार्किक generateCentersPP(स्थिरांक Mat& data, Mat& _out_centers,
                              पूर्णांक K, RNG& rng, पूर्णांक trials)
{
    CV_TRACE_FUNCTION();
    स्थिरांक पूर्णांक dims = data.cols, N = data.rows;
    cv::AutoBuffer<पूर्णांक, 64> _centers(K);
    पूर्णांक* centers = &_centers[0];
    cv::AutoBuffer<दशांश, 0> _dist(N*3);
    दशांश* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    द्विगुण sum0 = 0;

    centers[0] = (अचिन्हित)rng % N;

    साठी (पूर्णांक i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<दशांश>(i), data.ptr<दशांश>(centers[0]), dims);
        sum0 += dist[i];
    }
    जर (!(sum0 < द्विग_उच्च))
        परत असत्य;

    साठी (पूर्णांक k = 1; k < K; k++)
    {
        द्विगुण bestSum = द्विग_उच्च;
        पूर्णांक bestCenter = -1;

        साठी (पूर्णांक j = 0; j < trials; j++)
        {
            द्विगुण p = (द्विगुण)rng*sum0;
            पूर्णांक ci = 0;
            साठी (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                जर (p <= 0)
                    थांबा;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (द्विगुण)divUp((माप_प्रकार)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            द्विगुण s = 0;
            पूर्णांक i = 0;
        #जर CV_ENABLE_UNROLLED
            साठी (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            साठी (; i < N; i++)
            {
                s += tdist2[i];
            }

            जर (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        जर (bestCenter < 0)
            परत असत्य;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    साठी (पूर्णांक k = 0; k < K; k++)
    {
        स्थिरांक दशांश* src = data.ptr<दशांश>(centers[k]);
        दशांश* dst = _out_centers.ptr<दशांश>(k);
        साठी (पूर्णांक j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    परत सत्य;
}

ढाँचा<तार्किक onlyDistance>
वर्ग KMeansDistanceComputer : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    KMeansDistanceComputer( द्विगुण *distances_,
                            पूर्णांक *labels_,
                            स्थिरांक Mat& data_,
                            स्थिरांक Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    शून्य चालक()(स्थिरांक Range& range) स्थिरांक CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        स्थिरांक पूर्णांक begin = range.start;
        स्थिरांक पूर्णांक end = range.end;
        स्थिरांक पूर्णांक K = centers.rows;
        स्थिरांक पूर्णांक dims = centers.cols;

        साठी (पूर्णांक i = begin; i < end; ++i)
        {
            स्थिरांक दशांश *sample = data.ptr<दशांश>(i);
            जर (onlyDistance)
            {
                स्थिरांक दशांश* center = centers.ptr<दशांश>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                सुरूठेवा;
            }
            अन्यथा
            {
                पूर्णांक k_best = 0;
                द्विगुण min_dist = द्विग_उच्च;

                साठी (पूर्णांक k = 0; k < K; k++)
                {
                    स्थिरांक दशांश* center = centers.ptr<दशांश>(k);
                    स्थिरांक द्विगुण dist = hal::normL2Sqr_(sample, center, dims);
                    जर (!(dist <= भग्न_उच्च))
                    {
                        min_dist = द्विग_उच्च;
                        थांबा;
                    }

                    जर (min_dist > dist)
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

खाजगी:
    KMeansDistanceComputer& चालक=(स्थिरांक KMeansDistanceComputer&); // = delete

    द्विगुण *distances;
    पूर्णांक *labels;
    स्थिरांक Mat& data;
    स्थिरांक Mat& centers;
};

}

द्विगुण cv::kmeans( आदान_Array _data, पूर्णांक K,
                   आदान_Output_Array _bestLabels,
                   TermCriteria criteria, पूर्णांक attempts,
                   पूर्णांक flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    स्थिरांक पूर्णांक SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    स्थिरांक तार्किक isrow = data0.rows == 1;
    स्थिरांक पूर्णांक N = isrow ? data0.cols : data0.rows;
    स्थिरांक पूर्णांक dims = (isrow ? 1 : data0.cols)*data0.channels();
    स्थिरांक पूर्णांक type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * आकार(दशांश) : static_cast<माप_प्रकार>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, सत्य);

    Mat _labels, best_labels = _bestLabels.getMat();
    जर (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        साठी (पूर्णांक i = 0; i < N; i++)
        {
            CV_Assert((अचिन्हित)_labels.at<पूर्णांक>(i) < (अचिन्हित)K);
        }
    }
    अन्यथा
    {
        जर (!((best_labels.cols == 1 || best_labels.rows == 1) &&
             best_labels.cols*best_labels.rows == N &&
             best_labels.type() == CV_32S &&
             best_labels.isContinuous()))
        {
            _bestLabels.create(N, 1, CV_32S);
            best_labels = _bestLabels.getMat();
        }
        _labels.create(best_labels.size(), best_labels.type());
    }
    पूर्णांक* labels = _labels.ptr<पूर्णांक>();

    Mat centers(K, dims, type), old_centers(K, dims, type), temp(1, dims, type);
    cv::AutoBuffer<पूर्णांक, 64> counters(K);
    cv::AutoBuffer<द्विगुण, 64> dists(N);
    RNG& rng = theRNG();

    जर (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    अन्यथा
        criteria.epsilon = भग्न_अंतर;
    criteria.epsilon *= criteria.epsilon;

    जर (criteria.type & TermCriteria::मोजणी_3)
        criteria.max_मोजणी = std::min(std::max(criteria.max_मोजणी, 2), 100);
    अन्यथा
        criteria.max_मोजणी = 100;

    जर (K == 1)
    {
        attempts = 1;
        criteria.max_मोजणी = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    जर (!(flags & KMEANS_PP_CENTERS))
    {
        {
            स्थिरांक दशांश* sample = data.ptr<दशांश>(0);
            साठी (पूर्णांक j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        साठी (पूर्णांक i = 1; i < N; i++)
        {
            स्थिरांक दशांश* sample = data.ptr<दशांश>(i);
            साठी (पूर्णांक j = 0; j < dims; j++)
            {
                दशांश v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    द्विगुण best_compactness = द्विग_उच्च;
    साठी (पूर्णांक a = 0; a < attempts; a++)
    {
        द्विगुण compactness = द्विग_उच्च;

        साठी (पूर्णांक iter = 0; ;)
        {
            द्विगुण max_center_shift = iter == 0 ? द्विग_उच्च : 0.0;

            swap(centers, old_centers);

            जर (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                जर (flags & KMEANS_PP_CENTERS)
                {
                    जर (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        थांबा;
                }
                अन्यथा
                {
                    साठी (पूर्णांक k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<दशांश>(k), rng);
                }
            }
            अन्यथा
            {
                // compute centers
                centers = Scalar(0);
                साठी (पूर्णांक k = 0; k < K; k++)
                    counters[k] = 0;

                साठी (पूर्णांक i = 0; i < N; i++)
                {
                    स्थिरांक दशांश* sample = data.ptr<दशांश>(i);
                    पूर्णांक k = labels[i];
                    दशांश* center = centers.ptr<दशांश>(k);
                    साठी (पूर्णांक j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                साठी (पूर्णांक k = 0; k < K; k++)
                {
                    जर (counters[k] != 0)
                        सुरूठेवा;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    पूर्णांक max_k = 0;
                    साठी (पूर्णांक k1 = 1; k1 < K; k1++)
                    {
                        जर (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    द्विगुण max_dist = 0;
                    पूर्णांक farthest_i = -1;
                    दशांश* base_center = centers.ptr<दशांश>(max_k);
                    दशांश* _base_center = temp.ptr<दशांश>(); // normalized
                    दशांश scale = 1.f/counters[max_k];
                    साठी (पूर्णांक j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    साठी (पूर्णांक i = 0; i < N; i++)
                    {
                        जर (labels[i] != max_k)
                            सुरूठेवा;
                        जर (farthest_i < 0)
                            farthest_i = i;
                        स्थिरांक दशांश* sample = data.ptr<दशांश>(i);
                        द्विगुण dist = hal::normL2Sqr_(sample, _base_center, dims);

                        जर (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    स्थिरांक दशांश* sample = data.ptr<दशांश>(farthest_i);
                    दशांश* cur_center = centers.ptr<दशांश>(k);
                    साठी (पूर्णांक j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                साठी (पूर्णांक k = 0; k < K; k++)
                {
                    दशांश* center = centers.ptr<दशांश>(k);
                    CV_Assert( counters[k] != 0 );

                    दशांश scale = 1.f/counters[k];
                    साठी (पूर्णांक j = 0; j < dims; j++)
                        center[j] *= scale;

                    जर (iter > 0)
                    {
                        द्विगुण dist = 0;
                        स्थिरांक दशांश* old_center = old_centers.ptr<दशांश>(k);
                        साठी (पूर्णांक j = 0; j < dims; j++)
                        {
                            द्विगुण t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            तार्किक isLastIter = (++iter == MAX(criteria.max_मोजणी, 2) || max_center_shift <= criteria.epsilon);

            जर (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<सत्य>(dists.data(), labels, data, centers), (द्विगुण)divUp((माप_प्रकार)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                थांबा;
            }
            अन्यथा
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<असत्य>(dists.data(), labels, data, centers), (द्विगुण)divUp((माप_प्रकार)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                जर (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < द्विग_उच्च))
                    थांबा;
            }
        }

        जर (!(compactness < द्विग_उच्च))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        जर (compactness < best_compactness)
        {
            best_compactness = compactness;
            जर (_centers.needed())
            {
                जर (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                अन्यथा
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        जर (compactness < 0.)
            थांबा;
    }

    परत best_compactness;
}
