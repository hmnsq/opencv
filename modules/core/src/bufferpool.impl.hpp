// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ifndef __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#define __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#অন্তর্ভাব "opencv2/core/bufferpool.hpp"

নামাকাশ cv {

শ্রেণী Dummy_বাফার_Pool_Controller : সর্বজনীন বাফার_Pool_Controller
{
সর্বজনীন:
    Dummy_বাফার_Pool_Controller() { }
    আভাসী ~Dummy_বাফার_Pool_Controller() { }

    আভাসী size_t get_Reserved_আকার() ধ্রুবক CV_OVERRIDE { ফেরত (size_t)-1; }
    আভাসী size_t get_Max_Reserved_আকার() ধ্রুবক CV_OVERRIDE { ফেরত (size_t)-1; }
    আভাসী শূন্য set_Max_Reserved_আকার(size_t size) CV_OVERRIDE { CV_UNUSED(size); }
    আভাসী শূন্য freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
