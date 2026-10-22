// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/bufferpool.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਵਰਗ Dummy_ਬਫਰ_Pool_Controller : ਜਨਤਕ ਬਫਰ_Pool_Controller
{
ਜਨਤਕ:
    Dummy_ਬਫਰ_Pool_Controller() { }
    ਆਭਾਸੀ ~Dummy_ਬਫਰ_Pool_Controller() { }

    ਆਭਾਸੀ size_t get_Reserved_ਮਾਪ() ਅਚਲ CV_OVERRIDE { ਵਾਪਸ (size_t)-1; }
    ਆਭਾਸੀ size_t get_Max_Reserved_ਮਾਪ() ਅਚਲ CV_OVERRIDE { ਵਾਪਸ (size_t)-1; }
    ਆਭਾਸੀ ਖਾਲੀ ਸੈੱਟ_Max_Reserved_ਮਾਪ(size_t size) CV_OVERRIDE { CV_ਅਣਵਰਤਿਆ(size); }
    ਆਭਾਸੀ ਖਾਲੀ freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#ਜੇ_ਅੰਤ // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
