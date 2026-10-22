// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#यदि_अपरिभाषितम् __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#परिभाषय __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#अन्तर्भाव "opencv2/core/bufferpool.hpp"

नामाकाश cv {

वर्ग Dummy_बफर_Pool_Controller : सार्वजनिक बफर_Pool_Controller
{
सार्वजनिक:
    Dummy_बफर_Pool_Controller() { }
    आभास ~Dummy_बफर_Pool_Controller() { }

    आभास size_t get_Reserved_आकार() ध्रुव CV_OVERRIDE { प्रत्यागम (size_t)-1; }
    आभास size_t get_Max_Reserved_आकार() ध्रुव CV_OVERRIDE { प्रत्यागम (size_t)-1; }
    आभास शून्य समुच्चयः_Max_Reserved_आकार(size_t size) CV_OVERRIDE { CV_अप्रयुक्तम्(size); }
    आभास शून्य freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#यदि_अन्तः // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
