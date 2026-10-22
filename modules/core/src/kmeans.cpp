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

#समावेश "precomp.hpp"
#समावेश <opencv2/core/utils/configuration.private.hpp>
#समावेश <opencv2/core/hal/hal.hpp>

////////////////////////////////////////// kmeans ////////////////////////////////////////////

namespace cv
{

अटल पूर्णांक CV_KMEANS_PARALLEL_GRANULARITY = (पूर्णांक)utils::get_Configuration_Parameter_आकार_T("OPENCV_KMEANS_PARALLEL_GRANULARITY", 1000);

अटल व्योम generateRandomCenter(पूर्णांक dims, स्थिर Vec2f* box, भग्न* center, RNG& rng)
{
    भग्न margin = 1.f/dims;
    क्रम (पूर्णांक j = 0; j < dims; j++)
        center[j] = ((भग्न)rng*(1.f+margin*2.f)-margin)*(box[j][1] - box[j][0]) + box[j][0];
}

श्रेणी KMeansPPDistanceComputer : खुला ParallelLoopBody
{
खुला:
    KMeansPPDistanceComputer(भग्न *tdist2_, स्थिर Mat& तथ्य_2, स्थिर भग्न *dist_, पूर्णांक ci_) :
        tdist2(tdist2_), तथ्य(तथ्य_2), dist(dist_), ci(ci_)
    { }

    व्योम चालक()( स्थिर cv::Range& range ) स्थिर CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        स्थिर पूर्णांक begin = range.start;
        स्थिर पूर्णांक end = range.end;
        स्थिर पूर्णांक dims = तथ्य.cols;

        क्रम (पूर्णांक i = begin; i<end; i++)
        {
            tdist2[i] = std::min(hal::normL2Sqr_(तथ्य.ptr<भग्न>(i), तथ्य.ptr<भग्न>(ci), dims), dist[i]);
        }
    }

निजी:
    KMeansPPDistanceComputer& चालक=(स्थिर KMeansPPDistanceComputer&); // = delete

    भग्न *tdist2;
    स्थिर Mat& तथ्य;
    स्थिर भग्न *dist;
    स्थिर पूर्णांक ci;
};

/*
k-means center initialization using the following algorithm:
Arthur & Vassilvitskii (2007) k-means++: The Advantages of Careful Seeding
*/
अटल bool generateCentersPP(स्थिर Mat& तथ्य, Mat& _out_centers,
                              पूर्णांक K, RNG& rng, पूर्णांक trials)
{
    CV_TRACE_FUNCTION();
    स्थिर पूर्णांक dims = तथ्य.cols, N = तथ्य.rows;
    cv::Auto_बफर<पूर्णांक, 64> _centers(K);
    पूर्णांक* centers = &_centers[0];
    cv::Auto_बफर<भग्न, 0> _dist(N*3);
    भग्न* dist = &_dist[0], *tdist = dist + N, *tdist2 = tdist + N;
    द्विगुन sum0 = 0;

    centers[0] = (अचिन्हित)rng % N;

    क्रम (पूर्णांक i = 0; i < N; i++)
    {
        dist[i] = hal::normL2Sqr_(तथ्य.ptr<भग्न>(i), तथ्य.ptr<भग्न>(centers[0]), dims);
        sum0 += dist[i];
    }
    अगर (!(sum0 < द्विग_उच्च))
        वापस false;

    क्रम (पूर्णांक k = 1; k < K; k++)
    {
        द्विगुन best_योग = द्विग_उच्च;
        पूर्णांक bestCenter = -1;

        क्रम (पूर्णांक j = 0; j < trials; j++)
        {
            द्विगुन p = (द्विगुन)rng*sum0;
            पूर्णांक ci = 0;
            क्रम (; ci < N - 1; ci++)
            {
                p -= dist[ci];
                अगर (p <= 0)
                    अवरोध;
            }

            parallel_for_(Range(0, N),
                          KMeansPPDistanceComputer(tdist2, तथ्य, dist, ci),
                          (द्विगुन)divUp((माप_प्रकार)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
            द्विगुन s = 0;
            पूर्णांक i = 0;
        #अगर CV_ENABLE_UNROLLED
            क्रम (; i + 7 < N; i += 8)
            {
                s += tdist2[i + 0] + tdist2[i + 1] + tdist2[i + 2] + tdist2[i + 3] +
                     tdist2[i + 4] + tdist2[i + 5] + tdist2[i + 6] + tdist2[i + 7];
            }
        #पूर्ण_अगर
            क्रम (; i < N; i++)
            {
                s += tdist2[i];
            }

            अगर (s < best_योग)
            {
                best_योग = s;
                bestCenter = ci;
                std::swap(tdist, tdist2);
            }
        }
        अगर (bestCenter < 0)
            वापस false;
        centers[k] = bestCenter;
        sum0 = best_योग;
        std::swap(dist, tdist);
    }

    क्रम (पूर्णांक k = 0; k < K; k++)
    {
        स्थिर भग्न* src = तथ्य.ptr<भग्न>(centers[k]);
        भग्न* dst = _out_centers.ptr<भग्न>(k);
        क्रम (पूर्णांक j = 0; j < dims; j++)
            dst[j] = src[j];
    }
    वापस true;
}

ढाँचा<bool onlyDistance>
श्रेणी KMeansDistanceComputer : खुला ParallelLoopBody
{
खुला:
    KMeansDistanceComputer( द्विगुन *distances_,
                            पूर्णांक *labels_,
                            स्थिर Mat& तथ्य_2,
                            स्थिर Mat& centers_)
        : distances(distances_),
          labels(labels_),
          तथ्य(तथ्य_2),
          centers(centers_)
    {
    }

    व्योम चालक()(स्थिर Range& range) स्थिर CV_OVERRIDE
    {
        CV_TRACE_FUNCTION();
        स्थिर पूर्णांक begin = range.start;
        स्थिर पूर्णांक end = range.end;
        स्थिर पूर्णांक K = centers.rows;
        स्थिर पूर्णांक dims = centers.cols;

        क्रम (पूर्णांक i = begin; i < end; ++i)
        {
            स्थिर भग्न *sample = तथ्य.ptr<भग्न>(i);
            अगर (onlyDistance)
            {
                स्थिर भग्न* center = centers.ptr<भग्न>(labels[i]);
                distances[i] = hal::normL2Sqr_(sample, center, dims);
                जारी;
            }
            अन्यथा
            {
                पूर्णांक k_best = 0;
                द्विगुन min_dist = द्विग_उच्च;

                क्रम (पूर्णांक k = 0; k < K; k++)
                {
                    स्थिर भग्न* center = centers.ptr<भग्न>(k);
                    स्थिर द्विगुन dist = hal::normL2Sqr_(sample, center, dims);
                    अगर (!(dist <= भग्न_उच्च))
                    {
                        min_dist = द्विग_उच्च;
                        अवरोध;
                    }

                    अगर (min_dist > dist)
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

निजी:
    KMeansDistanceComputer& चालक=(स्थिर KMeansDistanceComputer&); // = delete

    द्विगुन *distances;
    पूर्णांक *labels;
    स्थिर Mat& तथ्य;
    स्थिर Mat& centers;
};

}

द्विगुन cv::kmeans( निवेश_Array तथ्य_3, पूर्णांक K,
                   निवेश_Output_Array _bestLabels,
                   TermCriteria criteria, पूर्णांक attempts,
                   पूर्णांक flags, OutputArray _centers )
{
    CV_INSTRUMENT_REGION();
    स्थिर पूर्णांक SPP_TRIALS = 3;
    Mat data0 = तथ्य_3.getMat();
    स्थिर bool isrow = data0.rows == 1;
    स्थिर पूर्णांक N = isrow ? data0.cols : data0.rows;
    स्थिर पूर्णांक dims = (isrow ? 1 : data0.cols)*data0.channels();
    स्थिर पूर्णांक type = data0.depth();

    attempts = std::max(attempts, 1);
    CV_Assert( data0.dims <= 2 && type == CV_32F && K > 0 );
    CV_CheckGE(N, K, "There can't be more clusters than elements");

    Mat तथ्य(N, dims, CV_32F, data0.ptr(), isrow ? dims * माप(भग्न) : static_cast<माप_प्रकार>(data0.step));

    _bestLabels.create(N, 1, CV_32S, -1, true);

    Mat _labels, best_labels = _bestLabels.getMat();
    अगर (flags & KMEANS_USE_INITIAL_LABELS)
    {
        CV_Assert( (best_labels.cols == 1 || best_labels.rows == 1) &&
                  best_labels.cols*best_labels.rows == N &&
                  best_labels.type() == CV_32S &&
                  best_labels.isContinuous());
        best_labels.reshape(1, N).copyTo(_labels);
        क्रम (पूर्णांक i = 0; i < N; i++)
        {
            CV_Assert((अचिन्हित)_labels.at<पूर्णांक>(i) < (अचिन्हित)K);
        }
    }
    अन्यथा
    {
        अगर (!((best_labels.cols == 1 || best_labels.rows == 1) &&
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

    Mat centers(K, dims, type), old_centers(K, dims, type), अस्थायी_2(1, dims, type);
    cv::Auto_बफर<पूर्णांक, 64> counters(K);
    cv::Auto_बफर<द्विगुन, 64> dists(N);
    RNG& rng = theRNG();

    अगर (criteria.type & TermCriteria::EPS)
        criteria.epsilon = std::max(criteria.epsilon, 0.);
    अन्यथा
        criteria.epsilon = भग्न_अंतर;
    criteria.epsilon *= criteria.epsilon;

    अगर (criteria.type & TermCriteria::गिनती_3)
        criteria.max_गिनती = std::min(std::max(criteria.max_गिनती, 2), 100);
    अन्यथा
        criteria.max_गिनती = 100;

    अगर (K == 1)
    {
        attempts = 1;
        criteria.max_गिनती = 2;
    }

    cv::Auto_बफर<Vec2f, 64> box(dims);
    अगर (!(flags & KMEANS_PP_CENTERS))
    {
        {
            स्थिर भग्न* sample = तथ्य.ptr<भग्न>(0);
            क्रम (पूर्णांक j = 0; j < dims; j++)
                box[j] = Vec2f(sample[j], sample[j]);
        }
        क्रम (पूर्णांक i = 1; i < N; i++)
        {
            स्थिर भग्न* sample = तथ्य.ptr<भग्न>(i);
            क्रम (पूर्णांक j = 0; j < dims; j++)
            {
                भग्न v = sample[j];
                box[j][0] = std::min(box[j][0], v);
                box[j][1] = std::max(box[j][1], v);
            }
        }
    }

    द्विगुन best_compactness = द्विग_उच्च;
    क्रम (पूर्णांक a = 0; a < attempts; a++)
    {
        द्विगुन compactness = द्विग_उच्च;

        क्रम (पूर्णांक iter = 0; ;)
        {
            द्विगुन max_center_shift = iter == 0 ? द्विग_उच्च : 0.0;

            swap(centers, old_centers);

            अगर (iter == 0 && (a > 0 || !(flags & KMEANS_USE_INITIAL_LABELS)))
            {
                अगर (flags & KMEANS_PP_CENTERS)
                {
                    अगर (!generateCentersPP(तथ्य, centers, K, rng, SPP_TRIALS))
                        अवरोध;
                }
                अन्यथा
                {
                    क्रम (पूर्णांक k = 0; k < K; k++)
                        generateRandomCenter(dims, box.तथ्य(), centers.ptr<भग्न>(k), rng);
                }
            }
            अन्यथा
            {
                // compute centers
                centers = Scalar(0);
                क्रम (पूर्णांक k = 0; k < K; k++)
                    counters[k] = 0;

                क्रम (पूर्णांक i = 0; i < N; i++)
                {
                    स्थिर भग्न* sample = तथ्य.ptr<भग्न>(i);
                    पूर्णांक k = labels[i];
                    भग्न* center = centers.ptr<भग्न>(k);
                    क्रम (पूर्णांक j = 0; j < dims; j++)
                        center[j] += sample[j];
                    counters[k]++;
                }

                क्रम (पूर्णांक k = 0; k < K; k++)
                {
                    अगर (counters[k] != 0)
                        जारी;

                    // if some cluster appeared to be empty then:
                    //   1. find the biggest cluster
                    //   2. find the farthest from the center point in the biggest cluster
                    //   3. exclude the farthest point from the biggest cluster and form a new 1-point cluster.
                    पूर्णांक max_k = 0;
                    क्रम (पूर्णांक k1 = 1; k1 < K; k1++)
                    {
                        अगर (counters[max_k] < counters[k1])
                            max_k = k1;
                    }

                    द्विगुन max_dist = 0;
                    पूर्णांक farthest_i = -1;
                    भग्न* base_center = centers.ptr<भग्न>(max_k);
                    भग्न* _base_center = अस्थायी_2.ptr<भग्न>(); // normalized
                    भग्न scale = 1.f/counters[max_k];
                    क्रम (पूर्णांक j = 0; j < dims; j++)
                        _base_center[j] = base_center[j]*scale;

                    क्रम (पूर्णांक i = 0; i < N; i++)
                    {
                        अगर (labels[i] != max_k)
                            जारी;
                        अगर (farthest_i < 0)
                            farthest_i = i;
                        स्थिर भग्न* sample = तथ्य.ptr<भग्न>(i);
                        द्विगुन dist = hal::normL2Sqr_(sample, _base_center, dims);

                        अगर (max_dist <= dist)
                        {
                            max_dist = dist;
                            farthest_i = i;
                        }
                    }

                    counters[max_k]--;
                    counters[k]++;
                    labels[farthest_i] = k;

                    स्थिर भग्न* sample = तथ्य.ptr<भग्न>(farthest_i);
                    भग्न* cur_center = centers.ptr<भग्न>(k);
                    क्रम (पूर्णांक j = 0; j < dims; j++)
                    {
                        base_center[j] -= sample[j];
                        cur_center[j] += sample[j];
                    }
                }

                क्रम (पूर्णांक k = 0; k < K; k++)
                {
                    भग्न* center = centers.ptr<भग्न>(k);
                    CV_Assert( counters[k] != 0 );

                    भग्न scale = 1.f/counters[k];
                    क्रम (पूर्णांक j = 0; j < dims; j++)
                        center[j] *= scale;

                    अगर (iter > 0)
                    {
                        द्विगुन dist = 0;
                        स्थिर भग्न* old_center = old_centers.ptr<भग्न>(k);
                        क्रम (पूर्णांक j = 0; j < dims; j++)
                        {
                            द्विगुन t = center[j] - old_center[j];
                            dist += t*t;
                        }
                        max_center_shift = std::max(max_center_shift, dist);
                    }
                }
            }

            bool isLastIter = (++iter == MAX(criteria.max_गिनती, 2) || max_center_shift <= criteria.epsilon);

            अगर (isLastIter)
            {
                // don't re-assign labels to avoid creation of empty clusters
                parallel_for_(Range(0, N), KMeansDistanceComputer<true>(dists.तथ्य(), labels, तथ्य, centers), (द्विगुन)divUp((माप_प्रकार)(dims * N), CV_KMEANS_PARALLEL_GRANULARITY));
                compactness = योग_2(Mat(आकार(N, 1), CV_64F, &dists[0]))[0];
                अवरोध;
            }
            अन्यथा
            {
                // assign labels
                parallel_for_(Range(0, N), KMeansDistanceComputer<false>(dists.तथ्य(), labels, तथ्य, centers), (द्विगुन)divUp((माप_प्रकार)(dims * N * K), CV_KMEANS_PARALLEL_GRANULARITY));
                अगर (!(योग_2(Mat(आकार(N, 1), CV_64F, &dists[0]))[0] < द्विग_उच्च))
                    अवरोध;
            }
        }

        अगर (!(compactness < द्विग_उच्च))
        {
            centers.setZero();
            _labels.setZero();
            compactness = -1.;
        }
        अगर (compactness < best_compactness)
        {
            best_compactness = compactness;
            अगर (_centers.needed())
            {
                अगर (_centers.fixedType() && _centers.channels() == dims)
                    centers.reshape(dims).copyTo(_centers);
                अन्यथा
                    centers.copyTo(_centers);
            }
            _labels.copyTo(best_labels);
        }
        अगर (compactness < 0.)
            अवरोध;
    }

    वापस best_compactness;
}
