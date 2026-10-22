// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#که_نه_تعریف_شوی __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#تعریف_کړه __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#ضمن "opencv2/core/bufferpool.hpp"

نطاق cv {

صنف Dummy_بفر_Pool_Controller : عام بفر_Pool_Controller
{
عام:
    Dummy_بفر_Pool_Controller() { }
    مجازي ~Dummy_بفر_Pool_Controller() { }

    مجازي size_t get_Reserved_اندازه() ثابت CV_OVERRIDE { أرجع (size_t)-1; }
    مجازي size_t get_Max_Reserved_اندازه() ثابت CV_OVERRIDE { أرجع (size_t)-1; }
    مجازي فراغ سېټ_Max_Reserved_اندازه(size_t size) CV_OVERRIDE { CV_ناکارول_شوی(size); }
    مجازي فراغ freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#د_که_پای // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
