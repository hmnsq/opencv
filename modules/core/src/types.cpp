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
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
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

#ضمن "precomp.hpp"

نطاق cv
{

////////////////////// KeyPoint //////////////////////

size_t ڪنجي_نقطو::hash() ثابت
{
    size_t _Val = 2166136261U, scale = 16777619U;
    Cv32suf u;
    u.f = pt.x; _Val = (scale * _Val) ^ u.u;
    u.f = pt.y; _Val = (scale * _Val) ^ u.u;
    u.f = size; _Val = (scale * _Val) ^ u.u;
    u.f = angle; _Val = (scale * _Val) ^ u.u;
    u.f = response; _Val = (scale * _Val) ^ u.u;
    _Val = (scale * _Val) ^ ((size_t) octave);
    _Val = (scale * _Val) ^ ((size_t) class_id);
    أرجع _Val;
}

فراغ ڪنجي_نقطو::convert(ثابت std::vector<ڪنجي_نقطو>& keypoints, std::vector<Point2f>& points2f,
                       ثابت std::vector<صحيح>& keypointIndexes)
{
    CV_INSTRUMENT_REGION();

    إذا( keypointIndexes.empty() )
    {
        points2f.resize( keypoints.size() );
        لكل( size_t i = 0; i < keypoints.size(); i++ )
            points2f[i] = keypoints[i].pt;
    }
    وإلا
    {
        points2f.resize( keypointIndexes.size() );
        لكل( size_t i = 0; i < keypointIndexes.size(); i++ )
        {
            صحيح idx = keypointIndexes[i];
            إذا( idx >= 0 && idx < (صحيح)keypoints.size() )
                points2f[i] = keypoints[idx].pt;
            وإلا
            {
                CV_Error( cv::Error::StsBadArg, "keypointIndexes has element out of range" );
                //points2f[i] = Point2f(-1, -1);
            }
        }
    }
}

فراغ ڪنجي_نقطو::convert( ثابت std::vector<Point2f>& points2f, std::vector<ڪنجي_نقطو>& keypoints,
                        عائم size, عائم response, صحيح octave, صحيح class_id )
{
    CV_INSTRUMENT_REGION();

    keypoints.resize(points2f.size());
    لكل( size_t i = 0; i < points2f.size(); i++ )
        keypoints[i] = ڪنجي_نقطو(points2f[i], size, -1, response, octave, class_id);
}

عائم ڪنجي_نقطو::overlap( ثابت ڪنجي_نقطو& kp1, ثابت ڪنجي_نقطو& kp2 )
{
    عائم a = kp1.size * 0.5f;
    عائم b = kp2.size * 0.5f;
    عائم a_2 = a * a;
    عائم b_2 = b * b;

    Point2f p1 = kp1.pt;
    Point2f p2 = kp2.pt;
    عائم c = (عائم)norm( p1 - p2 );

    عائم ovrl = 0.f;

    // one circle is completely encovered by the other => no intersection points!
    إذا( std::min( a, b ) + c <= std::max( a, b ) )
        أرجع std::min( a_2, b_2 ) / std::max( a_2, b_2 );

    إذا( c < a + b ) // circles intersect
    {
        عائم c_2 = c * c;
        عائم cosAlpha = ( b_2 + c_2 - a_2 ) / ( kp2.size * c );
        عائم cosBeta  = ( a_2 + c_2 - b_2 ) / ( kp1.size * c );
        عائم alpha = acos( cosAlpha );
        عائم beta = acos( cosBeta );
        عائم sinAlpha = جيب(alpha);
        عائم sinBeta  = جيب(beta);

        عائم segmentAreaA = a_2 * beta;
        عائم segmentAreaB = b_2 * alpha;

        عائم triangleAreaA = a_2 * sinBeta * cosBeta;
        عائم triangleAreaB = b_2 * sinAlpha * cosAlpha;

        عائم intersectionArea = segmentAreaA + segmentAreaB - triangleAreaA - triangleAreaB;
        عائم unionArea = (a_2 + b_2) * (عائم)CV_PI - intersectionArea;

        ovrl = intersectionArea / unionArea;
    }

    أرجع ovrl;
}

////////////////////// RotatedRect //////////////////////

RotatedRect::RotatedRect(ثابت Point2f& _point1, ثابت Point2f& _point2, ثابت Point2f& _point3)
{
    Point2f _center = 0.5f * (_point1 + _point3);
    Vec2f vecs[2];
    vecs[0] = Vec2f(_point1 - _point2);
    vecs[1] = Vec2f(_point2 - _point3);
    مزدوج x = std::max(norm(_point1), std::max(norm(_point2), norm(_point3)));
    مزدوج a = std::min(norm(vecs[0]), norm(vecs[1]));
    // check that given sides are perpendicular
    CV_Assert( std::fabs(vecs[0].ddot(vecs[1])) * a <= FLT_EPSILON * 9 * x * (norm(vecs[0]) * norm(vecs[1])) );

    // wd_i stores which vector (0,1) or (1,2) will make the width
    // One of them will definitely have slope within -1 to 1
    صحيح wd_i = 0;
    إذا( std::fabs(vecs[1][1]) < std::fabs(vecs[1][0]) ) wd_i = 1;
    صحيح ht_i = (wd_i + 1) % 2;

    عائم _angle = std::atan(vecs[wd_i][1] / vecs[wd_i][0]) * 180.0f / (عائم) CV_PI;
    عائم ويڪر_3 = (عائم) norm(vecs[wd_i]);
    عائم اوچائي_3 = (عائم) norm(vecs[ht_i]);

    center = _center;
    size = Size2f(ويڪر_3, اوچائي_3);
    angle = _angle;
}

فراغ RotatedRect::points(Point2f pt[]) ثابت
{
    مزدوج _angle = angle*CV_PI/180.;
    عائم b = (عائم)جيب_تمام(_angle)*0.5f;
    عائم a = (عائم)جيب(_angle)*0.5f;

    ثابت عائم ah = a*size.اوچائي;
    ثابت عائم aw = a*size.ويڪر;
    ثابت عائم bh = b*size.اوچائي;
    ثابت عائم bw = b*size.ويڪر;
    pt[0].x = center.x - ah - bw;
    pt[0].y = center.y + bh - aw;
    pt[1].x = center.x + ah - bw;
    pt[1].y = center.y - bh - aw;
    pt[2].x = center.x + ah + bw;
    pt[2].y = center.y - bh + aw;
    pt[3].x = center.x - ah + bw;
    pt[3].y = center.y + bh + aw;
}

فراغ RotatedRect::points(std::vector<Point2f>& pts) ثابت {
    pts.resize(4);
    points(pts.معلومات());
}

Rect RotatedRect::boundingRect() ثابت
{
    Point2f pt[4];
    points(pt);
    Rect r(cvFloor(std::min(std::min(std::min(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cvFloor(std::min(std::min(std::min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
           cvCeil(std::max(std::max(std::max(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cvCeil(std::max(std::max(std::max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    r.ويڪر -= r.x - 1;
    r.اوچائي -= r.y - 1;
    أرجع r;
}


Rect_<عائم> RotatedRect::boundingRect2f() ثابت
{
    Point2f pt[4];
    points(pt);
    Rect_<عائم> r(نقطو_3<عائم>(min(min(min(pt[0].x, pt[1].x), pt[2].x), pt[3].x), min(min(min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
                   نقطو_3<عائم>(max(max(max(pt[0].x, pt[1].x), pt[2].x), pt[3].x), max(max(max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    أرجع r;
}

} // cv
