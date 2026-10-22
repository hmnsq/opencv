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

स्थिर शून्य generateRandomCenter(पूर्णाङ्क dims, अचर Vec2f* box, भिन्न* center, RNG& rng)
{
    भिन्न margin = 1.f/dims;
    लागि (पूर्णाङ्क j = 0; j < dims; j++)
        center[j] = ((भिन्न)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

वर्ग KMeansPPDistanceComputer : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    KMeansPPDistanceComputer(भिन्न *tdist2_, अचर Mat& data_, अचर भिन्न *dist_, पूर्णाङ्क ci_) :
        tdist2(tdist2_), data(data_), dist(dist_), ci(ci_)
    { }

    शून्य चालक()( अचर cv::Range& range ) अचर CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        अचर पूर्णाङ्क begin = range.start;
        अचर पूर्णाङ्क end = range.end;
        अचर पूर्णाङ्क dims = data.cols;

        लागि (पूर्णाङ्क i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(data.ptr<भिन्न>(i), data.ptr<भिन्न>(ci), dims), dist[i]);
        }
    }

गुप्त:
    KMeansPPDistanceComputer& चालक=(अचर KMeansPPDistanceComputer&); // = delete

    भिन्न *tdist2;
    अचर Mat& data;
    अचर भिन्न *dist;
    अचर पूर्णाङ्क ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
स्थिर तार्किक generateCentersPP(अचर Mat& data, Mat& _out_centers,
                              पूर्णाङ्क K, RNG& rng, पूर्णाङ्क trials)
{
    CV_TRACE_FUNCTION();
    अचर पूर्णाङ्क dims = data.cols, N = data.rows;
    cv::AutoBuffer<पूर्णाङ्क, 64> _centers(K);
    पूर्णाङ्क* centers = &_centers[0];
    cv::AutoBuffer<भिन्न, 0> _dist(N*3);
    भिन्न* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    द्विगुण sum0 = 0;

    centers[0] = (अचिन्हित)rng % N;

    लागि (पूर्णाङ्क i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(data.ptr<भिन्न>(i), data.ptr<भिन्न>(centers[0]), dims);
        sum0 += dist[i];
    }
    यदि (!(sum0 < द्विग_उच्च))
        फर्काऊ असत्य;

    लागि (पूर्णाङ्क k = 1; k < K; k++)
    {
        द्विगुण bestSum = द्विग_उच्च;
        पूर्णाङ्क bestCenter = -1;

        लागि (पूर्णाङ्क j = 0; j < trials; j++)
        {
            द्विगुण p = (द्विगुण)rng*sum0;
            पूर्णाङ्क ci = 0;
            लागि (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                यदि (p <= 0)
                    रोक;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, data, dist, ci),
                          (द्विगुण)divUp((माप_प्रकार)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            द्विगुण s = 0;
            पूर्णाङ्क i = 0;
        #यदि CV_ENABLE_UNROLLED
            लागि (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #endif
            लागि (; i < N; i++)
            {
                s += tdist2[i];
            }

            यदि (s < bestSum)
            {
                bestSum = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        यदि (bestCenter < 0)
            फर्काऊ असत्य;
        centers[k] = bestCenter;
        sum0 = bestSum;
        std::swap(dist, tdist);
    }

    लागि (पूर्णाङ्क k = 0; k < K; k++)
    {
        अचर भिन्न* src = data.ptr<भिन्न>(centers[k]);
        भिन्न* dst = _out_centers.ptr<भिन्न>(k);
        लागि (पूर्णाङ्क j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    फर्काऊ सत्य;
}

ढाँचा<तार्किक onlyDistance>
वर्ग KMeansDistanceComputer : सार्वजनिक ParallelLoopBody
{
सार्वजनिक:
    KMeansDistanceComputer( द्विगुण *distances_,
                            पूर्णाङ्क *labels_,
                            अचर Mat& data_,
                            अचर Mat& centers_)
        : distances(distances_),
          labels(labels_),
          data(data_),
          centers(centers_)
    {
    }

    शून्य चालक()(अचर Range& range) अचर CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        अचर पूर्णाङ्क begin = range.start;
        अचर पूर्णाङ्क end = range.end;
        अचर पूर्णाङ्क K = centers.rows;
        अचर पूर्णाङ्क dims = centers.cols;

        लागि (पूर्णाङ्क i = begin; i < end; ++i)
        {
            अचर भिन्न *sample = data.ptr<भिन्न>(i);
            यदि (onlyDistance)
            {
                अचर भिन्न* center = centers.ptr<भिन्न>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                जारी;
            }
            अन्यथा
            {
                पूर्णाङ्क k_best = 0;
                द्विगुण min_dist = द्विग_उच्च;

                लागि (पूर्णाङ्क k = 0; k < K; k++)
                {
                    अचर भिन्न* center = centers.ptr<भिन्न>(k);
                    अचर द्विगुण dist = hal::normL2Sqr_(sample, center, dims);
                    यदि (!(dist <= भग्न_उच्च))
                    {
                        min_dist = द्विग_उच्च;
                        रोक;
                    }

                    यदि (min_dist > dist)
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
    KMeansDistanceComputer& चालक=(अचर KMeansDistanceComputer&); // = delete

    द्विगुण *distances;
    पूर्णाङ्क *labels;
    अचर Mat& data;
    अचर Mat& centers;
};

}

द्विगुण cv::kmeans( आगत_Array _data, पूर्णाङ्क K,
                   आगत_Output_Array _bestLabels,
                   TermCriteria criteria, पूर्णाङ्क attempts,
                   पूर्णाङ्क flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    अचर पूर्णाङ्क SPP_TRIALS = 3;
    Mat data0 = _data.getMat();
    अचर तार्किक isrow = data0.rows == 1;
    अचर पूर्णाङ्क N = isrow ? data0.cols : data0.rows;
    अचर पूर्णाङ्क dims = (isrow ? 1 : data0.cols)*data0.channels();
    अचर पूर्णाङ्क type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat data(N, dims, CV_32F, data0.ptr(), isrow ? dims * परिमाण(भिन्न) : static_cast<माप_प्रकार>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, सत्य);

    Mat _labels, best_labels = _bestLabels.getMat();
    यदि (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        लागि (पूर्णाङ्क i = 0; i < N; i++)
        {
            CV_Assert((अचिन्हित)_labels.at<पूर्णाङ्क>(i) < (अचिन्हित)K);
        }
    }
    अन्यथा
    {
        यदि (!((best_labels.cols == 1 || best_labels.rows == 1) &&
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

    यदि (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    अन्यथा
        criteria.epsilon = भग्न_अंतर;
    criteria.epsilon *= criteria.epsilon;

    यदि (criteria.type & TermCriteria::गन्ती_3)
        criteria.max_गन्ती = std::min(std::max(criteria.max_गन्ती, 2), 100);
    अन्यथा
        criteria.max_गन्ती = 100;

    यदि (K == 1)
    {
        attempts = 1;
        criteria.max_गन्ती = 2;
    }

    cv::AutoBuffer<Vec2f, 64> box(dims);
    यदि (!(flags & KMEANS_PP_CENTERS))
    {
        {
            अचर भिन्न* sample = data.ptr<भिन्न>(0);
            लागि (पूर्णाङ्क j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        लागि (पूर्णाङ्क i = 1; i < N; i++)
        {
            अचर भिन्न* sample = data.ptr<भिन्न>(i);
            लागि (पूर्णाङ्क j = 0; j < dims; j++)
            {
                भिन्न v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    द्विगुण best_compactness = द्विग_उच्च;
    लागि (पूर्णाङ्क a = 0; a < attempts; a++)
    {
        द्विगुण compactness = द्विग_उच्च;

        लागि (पूर्णाङ्क iter = 0; ;)
        {
            द्विगुण max_center_shift = iter == 0 ? द्विग_उच्च : 0.0;

            swap(centers, old_centers);

            यदि (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                यदि (flags & KMEANS_PP_CENTERS)
                {
                    यदि (!generateCentersPP(data, centers, K, rng, SPP_TRIALS))
                        रोक;
                }
                अन्यथा
                {
                    लागि (पूर्णाङ्क k = 0; k < K; k++)
                        generateRandomCenter(dims, box.data(), centers.ptr<भिन्न>(k), rng);
                }
            }
            अन्यथा
            {
                // compute centers
                centers = Scalar(0);
                लागि (पूर्णाङ्क k = 0; k < K; k++)
                    counters[k] = 0;

                लागि (पूर्णाङ्क i = 0; i < N; i++)
                {
                    अचर भिन्न* sample = data.ptr<भिन्न>(i);
                    पूर्णाङ्क k = labels[i];
                    भिन्न* center = centers.ptr<भिन्न>(k);
                    लागि (पूर्णाङ्क j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                लागि (पूर्णाङ्क k = 0; k < K; k++)
                {
                    यदि (counters[k] != 0)
                        जारी;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    पूर्णाङ्क max_k = 0;
                    लागि (पूर्णाङ्क k1 = 1; k1 < K; k1++)
                    {
                        यदि (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    द्विगुण max_dist = 0;
                    पूर्णाङ्क farthest_i = -1;
                    भिन्न* base_center = centers.ptr<भिन्न>(max_k);
                    भिन्न* _base_center = temp.ptr<भिन्न>(); // normalized
                    भिन्न scale = 1.f/counters[max_k];
                    लागि (पूर्णाङ्क j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    लागि (पूर्णाङ्क i = 0; i < N; i++)
                    {
                        यदि (labels[i] != max_k)
                            जारी;
                        यदि (farthest_i < 0)
                            farthest_i = i;
                        अचर भिन्न* sample = data.ptr<भिन्न>(i);
                        द्विगुण dist = hal::normL2Sqr_(sample, _base_center, dims);

                        यदि (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    अचर भिन्न* sample = data.ptr<भिन्न>(farthest_i);
                    भिन्न* cur_center = centers.ptr<भिन्न>(k);
                    लागि (पूर्णाङ्क j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                लागि (पूर्णाङ्क k = 0; k < K; k++)
                {
                    भिन्न* center = centers.ptr<भिन्न>(k);
                    CV_Assert( counters[k] != 0 );

                    भिन्न scale = 1.f/counters[k];
                    लागि (पूर्णाङ्क j = 0; j < dims; j++)
                        center[j] *= scale;

                    यदि (iter > 0)
                    {
                        द्विगुण dist = 0;
                        अचर भिन्न* old_center = old_centers.ptr<भिन्न>(k);
                        लागि (पूर्णाङ्क j = 0; j < dims; j++)
                        {
                            द्विगुण t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            तार्किक isLastIter = (++iter == MAX(criteria.max_गन्ती, 2) || max_center_shift <= criteria.epsilon);

            यदि (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<सत्य>(dists.data(), labels, data, centers), (द्विगुण)divUp((माप_प्रकार)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0];
                रोक;
            }
            अन्यथा
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<असत्य>(dists.data(), labels, data, centers), (द्विगुण)divUp((माप_प्रकार)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                यदि (!(sum(Mat(Size(N, 1), CV_64F, &dists[0]))[0] < द्विग_उच्च))
                    रोक;
            }
        }

        यदि (!(compactness < द्विग_उच्च))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        यदि (compactness < best_compactness)
        {
            best_compactness = compactness;
            यदि (_centers.needed())
            {
                यदि (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                अन्यथा
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        यदि (compactness < 0.)
            रोक;
    }

    फर्काऊ best_compactness;
}
