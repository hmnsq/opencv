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

#شامل "precomp.hpp"

نامخلا cv
{

////////////////////// KeyPoint //////////////////////

size_t کلید_نقطہ::hash() ثابت
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
    واپس _Val;
}

خالی کلید_نقطہ::convert(ثابت std::vector<کلید_نقطہ>& keypoints, std::vector<Point2f>& points2f,
                       ثابت std::vector<صحیح>& keypointIndexes)
{
    CV_INSTRUMENT_REGION();

    اگر( keypointIndexes.empty() )
    {
        points2f.resize( keypoints.size() );
        برائے( size_t i = 0; i < keypoints.size(); i++ )
            points2f[i] = keypoints[i].pt;
    }
    ورنہ
    {
        points2f.resize( keypointIndexes.size() );
        برائے( size_t i = 0; i < keypointIndexes.size(); i++ )
        {
            صحیح idx = keypointIndexes[i];
            اگر( idx >= 0 && idx < (صحیح)keypoints.size() )
                points2f[i] = keypoints[idx].pt;
            ورنہ
            {
                CV_Error( cv::Error::StsBadArg, "keypointIndexes has element out of range" );
                //points2f[i] = Point2f(-1, -1);
            }
        }
    }
}

خالی کلید_نقطہ::convert( ثابت std::vector<Point2f>& points2f, std::vector<کلید_نقطہ>& keypoints,
                        اعشاری size, اعشاری response, صحیح octave, صحیح class_id )
{
    CV_INSTRUMENT_REGION();

    keypoints.resize(points2f.size());
    برائے( size_t i = 0; i < points2f.size(); i++ )
        keypoints[i] = کلید_نقطہ(points2f[i], size, -1, response, octave, class_id);
}

اعشاری کلید_نقطہ::overlap( ثابت کلید_نقطہ& kp1, ثابت کلید_نقطہ& kp2 )
{
    اعشاری a = kp1.size * 0.5f;
    اعشاری b = kp2.size * 0.5f;
    اعشاری a_2 = a * a;
    اعشاری b_2 = b * b;

    Point2f p1 = kp1.pt;
    Point2f p2 = kp2.pt;
    اعشاری c = (اعشاری)norm( p1 - p2 );

    اعشاری ovrl = 0.f;

    // one circle is completely encovered by the other => no intersection points!
    اگر( std::min( a, b ) + c <= std::max( a, b ) )
        واپس std::min( a_2, b_2 ) / std::max( a_2, b_2 );

    اگر( c < a + b ) // circles intersect
    {
        اعشاری c_2 = c * c;
        اعشاری cosAlpha = ( b_2 + c_2 - a_2 ) / ( kp2.size * c );
        اعشاری cosBeta  = ( a_2 + c_2 - b_2 ) / ( kp1.size * c );
        اعشاری alpha = acos( cosAlpha );
        اعشاری beta = acos( cosBeta );
        اعشاری sinAlpha = جیب(alpha);
        اعشاری sinBeta  = جیب(beta);

        اعشاری segmentAreaA = a_2 * beta;
        اعشاری segmentAreaB = b_2 * alpha;

        اعشاری triangleAreaA = a_2 * sinBeta * cosBeta;
        اعشاری triangleAreaB = b_2 * sinAlpha * cosAlpha;

        اعشاری intersectionArea = segmentAreaA + segmentAreaB - triangleAreaA - triangleAreaB;
        اعشاری unionArea = (a_2 + b_2) * (اعشاری)CV_PI - intersectionArea;

        ovrl = intersectionArea / unionArea;
    }

    واپس ovrl;
}

////////////////////// RotatedRect //////////////////////

RotatedRect::RotatedRect(ثابت Point2f& _point1, ثابت Point2f& _point2, ثابت Point2f& _point3)
{
    Point2f _center = 0.5f * (_point1 + _point3);
    Vec2f vecs[2];
    vecs[0] = Vec2f(_point1 - _point2);
    vecs[1] = Vec2f(_point2 - _point3);
    دوہرا x = std::max(norm(_point1), std::max(norm(_point2), norm(_point3)));
    دوہرا a = std::min(norm(vecs[0]), norm(vecs[1]));
    // check that given sides are perpendicular
    CV_Assert( std::fabs(vecs[0].ddot(vecs[1])) * a <= FLT_EPSILON * 9 * x * (norm(vecs[0]) * norm(vecs[1])) );

    // wd_i stores which vector (0,1) or (1,2) will make the width
    // One of them will definitely have slope within -1 to 1
    صحیح wd_i = 0;
    اگر( std::fabs(vecs[1][1]) < std::fabs(vecs[1][0]) ) wd_i = 1;
    صحیح ht_i = (wd_i + 1) % 2;

    اعشاری _angle = std::atan(vecs[wd_i][1] / vecs[wd_i][0]) * 180.0f / (اعشاری) CV_PI;
    اعشاری چوڑائی_3 = (اعشاری) norm(vecs[wd_i]);
    اعشاری اونچائی_3 = (اعشاری) norm(vecs[ht_i]);

    center = _center;
    size = Size2f(چوڑائی_3, اونچائی_3);
    angle = _angle;
}

خالی RotatedRect::points(Point2f pt[]) ثابت
{
    دوہرا _angle = angle*CV_PI/180.;
    اعشاری b = (اعشاری)جیب_تمام(_angle)*0.5f;
    اعشاری a = (اعشاری)جیب(_angle)*0.5f;

    ثابت اعشاری ah = a*size.اونچائی;
    ثابت اعشاری aw = a*size.چوڑائی;
    ثابت اعشاری bh = b*size.اونچائی;
    ثابت اعشاری bw = b*size.چوڑائی;
    pt[0].x = center.x - ah - bw;
    pt[0].y = center.y + bh - aw;
    pt[1].x = center.x + ah - bw;
    pt[1].y = center.y - bh - aw;
    pt[2].x = center.x + ah + bw;
    pt[2].y = center.y - bh + aw;
    pt[3].x = center.x - ah + bw;
    pt[3].y = center.y + bh + aw;
}

خالی RotatedRect::points(std::vector<Point2f>& pts) ثابت {
    pts.resize(4);
    points(pts.ڈیٹا());
}

Rect RotatedRect::boundingRect() ثابت
{
    Point2f pt[4];
    points(pt);
    Rect r(cvFloor(std::min(std::min(std::min(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cvFloor(std::min(std::min(std::min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
           cvCeil(std::max(std::max(std::max(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cvCeil(std::max(std::max(std::max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    r.چوڑائی -= r.x - 1;
    r.اونچائی -= r.y - 1;
    واپس r;
}


Rect_<اعشاری> RotatedRect::boundingRect2f() ثابت
{
    Point2f pt[4];
    points(pt);
    Rect_<اعشاری> r(نقطہ_3<اعشاری>(min(min(min(pt[0].x, pt[1].x), pt[2].x), pt[3].x), min(min(min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
                   نقطہ_3<اعشاری>(max(max(max(pt[0].x, pt[1].x), pt[2].x), pt[3].x), max(max(max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    واپس r;
}

} // cv
