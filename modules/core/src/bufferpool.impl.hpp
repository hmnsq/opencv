// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#اگر_غیر_تعریف_شدہ __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#تعریف_کرو __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#شامل "opencv2/core/bufferpool.hpp"

نامخلا cv {

جماعت Dummy_بفر_Pool_Controller : عوامی بفر_Pool_Controller
{
عوامی:
    Dummy_بفر_Pool_Controller() { }
    مجازی ~Dummy_بفر_Pool_Controller() { }

    مجازی size_t get_Reserved_سائز() ثابت CV_OVERRIDE { واپس (size_t)-1; }
    مجازی size_t get_Max_Reserved_سائز() ثابت CV_OVERRIDE { واپس (size_t)-1; }
    مجازی خالی سیٹ_Max_Reserved_سائز(size_t size) CV_OVERRIDE { CV_غیر_مستعمل(size); }
    مجازی خالی freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#اگر_ختم // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
