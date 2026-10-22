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

#अन्तर्भाव "precomp.hpp"

नामाकाश cv
{

////////////////////// KeyPoint //////////////////////

माप_प्रकार किल्ली_Point::hash() स्थिरांक
{
    माप_प्रकार _Val = 2166136261U, scale = 16777619U;
    Cv32suf u;
    u.f = pt.x; _Val = (scale * _Val) ^ u.u;
    u.f = pt.y; _Val = (scale * _Val) ^ u.u;
    u.f = size; _Val = (scale * _Val) ^ u.u;
    u.f = angle; _Val = (scale * _Val) ^ u.u;
    u.f = response; _Val = (scale * _Val) ^ u.u;
    _Val = (scale * _Val) ^ ((माप_प्रकार) octave);
    _Val = (scale * _Val) ^ ((माप_प्रकार) class_id);
    परत _Val;
}

शून्य किल्ली_Point::convert(स्थिरांक std::vector<किल्ली_Point>& keypoints, std::vector<Point2f>& points2f,
                       स्थिरांक std::vector<पूर्णांक>& keypointIndexes)
{
    CV_INSTRUMENT_REGION();

    जर( keypointIndexes.empty() )
    {
        points2f.resize( keypoints.size() );
        साठी( माप_प्रकार i = 0; i < keypoints.size(); i++ )
            points2f[i] = keypoints[i].pt;
    }
    अन्यथा
    {
        points2f.resize( keypointIndexes.size() );
        साठी( माप_प्रकार i = 0; i < keypointIndexes.size(); i++ )
        {
            पूर्णांक idx = keypointIndexes[i];
            जर( idx >= 0 && idx < (पूर्णांक)keypoints.size() )
                points2f[i] = keypoints[idx].pt;
            अन्यथा
            {
                CV_Error( cv::Error::StsBadArg, "keypointIndexes has element out of range" );
                //points2f[i] = Point2f(-1, -1);
            }
        }
    }
}

शून्य किल्ली_Point::convert( स्थिरांक std::vector<Point2f>& points2f, std::vector<किल्ली_Point>& keypoints,
                        दशांश size, दशांश response, पूर्णांक octave, पूर्णांक class_id )
{
    CV_INSTRUMENT_REGION();

    keypoints.resize(points2f.size());
    साठी( माप_प्रकार i = 0; i < points2f.size(); i++ )
        keypoints[i] = किल्ली_Point(points2f[i], size, -1, response, octave, class_id);
}

दशांश किल्ली_Point::overlap( स्थिरांक किल्ली_Point& kp1, स्थिरांक किल्ली_Point& kp2 )
{
    दशांश a = kp1.size * 0.5f;
    दशांश b = kp2.size * 0.5f;
    दशांश a_2 = a * a;
    दशांश b_2 = b * b;

    Point2f p1 = kp1.pt;
    Point2f p2 = kp2.pt;
    दशांश c = (दशांश)norm( p1 - p2 );

    दशांश ovrl = 0.f;

    // one circle is completely encovered by the other => no intersection points!
    जर( std::min( a, b ) + c <= std::max( a, b ) )
        परत std::min( a_2, b_2 ) / std::max( a_2, b_2 );

    जर( c < a + b ) // circles intersect
    {
        दशांश c_2 = c * c;
        दशांश cosAlpha = ( b_2 + c_2 - a_2 ) / ( kp2.size * c );
        दशांश cosBeta  = ( a_2 + c_2 - b_2 ) / ( kp1.size * c );
        दशांश alpha = acos( cosAlpha );
        दशांश beta = acos( cosBeta );
        दशांश sinAlpha = sin(alpha);
        दशांश sinBeta  = sin(beta);

        दशांश segmentAreaA = a_2 * beta;
        दशांश segmentAreaB = b_2 * alpha;

        दशांश triangleAreaA = a_2 * sinBeta * cosBeta;
        दशांश triangleAreaB = b_2 * sinAlpha * cosAlpha;

        दशांश intersectionArea = segmentAreaA + segmentAreaB - triangleAreaA - triangleAreaB;
        दशांश unionArea = (a_2 + b_2) * (दशांश)CV_PI - intersectionArea;

        ovrl = intersectionArea / unionArea;
    }

    परत ovrl;
}

////////////////////// RotatedRect //////////////////////

RotatedRect::RotatedRect(स्थिरांक Point2f& _point1, स्थिरांक Point2f& _point2, स्थिरांक Point2f& _point3)
{
    Point2f _center = 0.5f * (_point1 + _point3);
    Vec2f vecs[2];
    vecs[0] = Vec2f(_point1 - _point2);
    vecs[1] = Vec2f(_point2 - _point3);
    द्विगुण x = std::max(norm(_point1), std::max(norm(_point2), norm(_point3)));
    द्विगुण a = std::min(norm(vecs[0]), norm(vecs[1]));
    // check that given sides are perpendicular
    CV_Assert( std::भ_असल(vecs[0].ddot(vecs[1])) * a <= भग्न_अंतर * 9 * x * (norm(vecs[0]) * norm(vecs[1])) );

    // wd_i stores which vector (0,1) or (1,2) will make the width
    // One of them will definitely have slope within -1 to 1
    पूर्णांक wd_i = 0;
    जर( std::भ_असल(vecs[1][1]) < std::भ_असल(vecs[1][0]) ) wd_i = 1;
    पूर्णांक ht_i = (wd_i + 1) % 2;

    दशांश _angle = std::atan(vecs[wd_i][1] / vecs[wd_i][0]) * 180.0f / (दशांश) CV_PI;
    दशांश _width = (दशांश) norm(vecs[wd_i]);
    दशांश _height = (दशांश) norm(vecs[ht_i]);

    center = _center;
    size = Size2f(_width, _height);
    angle = _angle;
}

शून्य RotatedRect::points(Point2f pt[]) स्थिरांक
{
    द्विगुण _angle = angle*CV_PI/180.;
    दशांश b = (दशांश)cos(_angle)*0.5f;
    दशांश a = (दशांश)sin(_angle)*0.5f;

    स्थिरांक दशांश ah = a*size.height;
    स्थिरांक दशांश aw = a*size.width;
    स्थिरांक दशांश bh = b*size.height;
    स्थिरांक दशांश bw = b*size.width;
    pt[0].x = center.x - ah - bw;
    pt[0].y = center.y + bh - aw;
    pt[1].x = center.x + ah - bw;
    pt[1].y = center.y - bh - aw;
    pt[2].x = center.x + ah + bw;
    pt[2].y = center.y - bh + aw;
    pt[3].x = center.x - ah + bw;
    pt[3].y = center.y + bh + aw;
}

शून्य RotatedRect::points(std::vector<Point2f>& pts) स्थिरांक {
    pts.resize(4);
    points(pts.data());
}

Rect RotatedRect::boundingRect() स्थिरांक
{
    Point2f pt[4];
    points(pt);
    Rect r(cvFloor(std::min(std::min(std::min(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cvFloor(std::min(std::min(std::min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
           cvCeil(std::max(std::max(std::max(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cvCeil(std::max(std::max(std::max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    r.width -= r.x - 1;
    r.height -= r.y - 1;
    परत r;
}


Rect_<दशांश> RotatedRect::boundingRect2f() स्थिरांक
{
    Point2f pt[4];
    points(pt);
    Rect_<दशांश> r(Point_<दशांश>(min(min(min(pt[0].x, pt[1].x), pt[2].x), pt[3].x), min(min(min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
                   Point_<दशांश>(max(max(max(pt[0].x, pt[1].x), pt[2].x), pt[3].x), max(max(max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    परत r;
}

} // cv
