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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਨਾਮਾਕਾਸ਼ cv
{

////////////////////// KeyPoint //////////////////////

size_t ਕੁੰਜੀ_ਬਿੰਦੂ::hash() ਅਚਲ
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
    ਵਾਪਸ _Val;
}

ਖਾਲੀ ਕੁੰਜੀ_ਬਿੰਦੂ::convert(ਅਚਲ std::vector<ਕੁੰਜੀ_ਬਿੰਦੂ>& keypoints, std::vector<Point2f>& points2f,
                       ਅਚਲ std::vector<ਪੂਰਨ>& keypointIndexes)
{
    CV_INSTRUMENT_REGION();

    ਜੇ( keypointIndexes.empty() )
    {
        points2f.resize( keypoints.size() );
        ਲਈ( size_t i = 0; i < keypoints.size(); i++ )
            points2f[i] = keypoints[i].pt;
    }
    ਨਹੀਂਤਾਂ
    {
        points2f.resize( keypointIndexes.size() );
        ਲਈ( size_t i = 0; i < keypointIndexes.size(); i++ )
        {
            ਪੂਰਨ idx = keypointIndexes[i];
            ਜੇ( idx >= 0 && idx < (ਪੂਰਨ)keypoints.size() )
                points2f[i] = keypoints[idx].pt;
            ਨਹੀਂਤਾਂ
            {
                CV_Error( cv::Error::StsBadArg, "keypointIndexes has element out of range" );
                //points2f[i] = Point2f(-1, -1);
            }
        }
    }
}

ਖਾਲੀ ਕੁੰਜੀ_ਬਿੰਦੂ::convert( ਅਚਲ std::vector<Point2f>& points2f, std::vector<ਕੁੰਜੀ_ਬਿੰਦੂ>& keypoints,
                        ਦਸ਼ਮਲਵ size, ਦਸ਼ਮਲਵ response, ਪੂਰਨ octave, ਪੂਰਨ class_id )
{
    CV_INSTRUMENT_REGION();

    keypoints.resize(points2f.size());
    ਲਈ( size_t i = 0; i < points2f.size(); i++ )
        keypoints[i] = ਕੁੰਜੀ_ਬਿੰਦੂ(points2f[i], size, -1, response, octave, class_id);
}

ਦਸ਼ਮਲਵ ਕੁੰਜੀ_ਬਿੰਦੂ::overlap( ਅਚਲ ਕੁੰਜੀ_ਬਿੰਦੂ& kp1, ਅਚਲ ਕੁੰਜੀ_ਬਿੰਦੂ& kp2 )
{
    ਦਸ਼ਮਲਵ a = kp1.size * 0.5f;
    ਦਸ਼ਮਲਵ b = kp2.size * 0.5f;
    ਦਸ਼ਮਲਵ a_2 = a * a;
    ਦਸ਼ਮਲਵ b_2 = b * b;

    Point2f p1 = kp1.pt;
    Point2f p2 = kp2.pt;
    ਦਸ਼ਮਲਵ c = (ਦਸ਼ਮਲਵ)norm( p1 - p2 );

    ਦਸ਼ਮਲਵ ovrl = 0.f;

    // one circle is completely encovered by the other => no intersection points!
    ਜੇ( std::min( a, b ) + c <= std::max( a, b ) )
        ਵਾਪਸ std::min( a_2, b_2 ) / std::max( a_2, b_2 );

    ਜੇ( c < a + b ) // circles intersect
    {
        ਦਸ਼ਮਲਵ c_2 = c * c;
        ਦਸ਼ਮਲਵ cosAlpha = ( b_2 + c_2 - a_2 ) / ( kp2.size * c );
        ਦਸ਼ਮਲਵ cosBeta  = ( a_2 + c_2 - b_2 ) / ( kp1.size * c );
        ਦਸ਼ਮਲਵ alpha = acos( cosAlpha );
        ਦਸ਼ਮਲਵ beta = acos( cosBeta );
        ਦਸ਼ਮਲਵ sinAlpha = ਜੀਵਾ(alpha);
        ਦਸ਼ਮਲਵ sinBeta  = ਜੀਵਾ(beta);

        ਦਸ਼ਮਲਵ segmentAreaA = a_2 * beta;
        ਦਸ਼ਮਲਵ segmentAreaB = b_2 * alpha;

        ਦਸ਼ਮਲਵ triangleAreaA = a_2 * sinBeta * cosBeta;
        ਦਸ਼ਮਲਵ triangleAreaB = b_2 * sinAlpha * cosAlpha;

        ਦਸ਼ਮਲਵ intersectionArea = segmentAreaA + segmentAreaB - triangleAreaA - triangleAreaB;
        ਦਸ਼ਮਲਵ unionArea = (a_2 + b_2) * (ਦਸ਼ਮਲਵ)CV_PI - intersectionArea;

        ovrl = intersectionArea / unionArea;
    }

    ਵਾਪਸ ovrl;
}

////////////////////// RotatedRect //////////////////////

RotatedRect::RotatedRect(ਅਚਲ Point2f& _point1, ਅਚਲ Point2f& _point2, ਅਚਲ Point2f& _point3)
{
    Point2f _center = 0.5f * (_point1 + _point3);
    Vec2f vecs[2];
    vecs[0] = Vec2f(_point1 - _point2);
    vecs[1] = Vec2f(_point2 - _point3);
    ਦੁੱਗਣਾ x = std::max(norm(_point1), std::max(norm(_point2), norm(_point3)));
    ਦੁੱਗਣਾ a = std::min(norm(vecs[0]), norm(vecs[1]));
    // check that given sides are perpendicular
    CV_Assert( std::fabs(vecs[0].ddot(vecs[1])) * a <= FLT_EPSILON * 9 * x * (norm(vecs[0]) * norm(vecs[1])) );

    // wd_i stores which vector (0,1) or (1,2) will make the width
    // One of them will definitely have slope within -1 to 1
    ਪੂਰਨ wd_i = 0;
    ਜੇ( std::fabs(vecs[1][1]) < std::fabs(vecs[1][0]) ) wd_i = 1;
    ਪੂਰਨ ht_i = (wd_i + 1) % 2;

    ਦਸ਼ਮਲਵ _angle = std::atan(vecs[wd_i][1] / vecs[wd_i][0]) * 180.0f / (ਦਸ਼ਮਲਵ) CV_PI;
    ਦਸ਼ਮਲਵ ਚੌੜਾਈ_3 = (ਦਸ਼ਮਲਵ) norm(vecs[wd_i]);
    ਦਸ਼ਮਲਵ ਉਚਾਈ_3 = (ਦਸ਼ਮਲਵ) norm(vecs[ht_i]);

    center = _center;
    size = Size2f(ਚੌੜਾਈ_3, ਉਚਾਈ_3);
    angle = _angle;
}

ਖਾਲੀ RotatedRect::points(Point2f pt[]) ਅਚਲ
{
    ਦੁੱਗਣਾ _angle = angle*CV_PI/180.;
    ਦਸ਼ਮਲਵ b = (ਦਸ਼ਮਲਵ)ਕੋਜੀਵਾ(_angle)*0.5f;
    ਦਸ਼ਮਲਵ a = (ਦਸ਼ਮਲਵ)ਜੀਵਾ(_angle)*0.5f;

    ਅਚਲ ਦਸ਼ਮਲਵ ah = a*size.ਉਚਾਈ;
    ਅਚਲ ਦਸ਼ਮਲਵ aw = a*size.ਚੌੜਾਈ;
    ਅਚਲ ਦਸ਼ਮਲਵ bh = b*size.ਉਚਾਈ;
    ਅਚਲ ਦਸ਼ਮਲਵ bw = b*size.ਚੌੜਾਈ;
    pt[0].x = center.x - ah - bw;
    pt[0].y = center.y + bh - aw;
    pt[1].x = center.x + ah - bw;
    pt[1].y = center.y - bh - aw;
    pt[2].x = center.x + ah + bw;
    pt[2].y = center.y - bh + aw;
    pt[3].x = center.x - ah + bw;
    pt[3].y = center.y + bh + aw;
}

ਖਾਲੀ RotatedRect::points(std::vector<Point2f>& pts) ਅਚਲ {
    pts.resize(4);
    points(pts.ਅੰਕੜੇ());
}

Rect RotatedRect::boundingRect() ਅਚਲ
{
    Point2f pt[4];
    points(pt);
    Rect r(cvFloor(std::min(std::min(std::min(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cvFloor(std::min(std::min(std::min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
           cvCeil(std::max(std::max(std::max(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cvCeil(std::max(std::max(std::max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    r.ਚੌੜਾਈ -= r.x - 1;
    r.ਉਚਾਈ -= r.y - 1;
    ਵਾਪਸ r;
}


Rect_<ਦਸ਼ਮਲਵ> RotatedRect::boundingRect2f() ਅਚਲ
{
    Point2f pt[4];
    points(pt);
    Rect_<ਦਸ਼ਮਲਵ> r(ਬਿੰਦੂ_3<ਦਸ਼ਮਲਵ>(min(min(min(pt[0].x, pt[1].x), pt[2].x), pt[3].x), min(min(min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
                   ਬਿੰਦੂ_3<ਦਸ਼ਮਲਵ>(max(max(max(pt[0].x, pt[1].x), pt[2].x), pt[3].x), max(max(max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    ਵਾਪਸ r;
}

} // cv
