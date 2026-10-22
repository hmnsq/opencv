// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#اگر_تعریف_نشده __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#تعریف_کن __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#ضمن "opencv2/core/bufferpool.hpp"

نطاق cv {

صنف Dummy_میانگیر_Pool_Controller : عام میانگیر_Pool_Controller
{
عام:
    Dummy_میانگیر_Pool_Controller() { }
    مجازی ~Dummy_میانگیر_Pool_Controller() { }

    مجازی size_t get_Reserved_اندازه() ثابت CV_OVERRIDE { أرجع (size_t)-1; }
    مجازی size_t get_Max_Reserved_اندازه() ثابت CV_OVERRIDE { أرجع (size_t)-1; }
    مجازی فراغ مجموعه_Max_Reserved_اندازه(size_t size) CV_OVERRIDE { CV_استفادهنشده(size); }
    مجازی فراغ freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#پایان_اگر // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
