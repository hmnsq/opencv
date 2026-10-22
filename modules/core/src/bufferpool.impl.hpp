// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ifndef __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#define __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#ಅನ್ತರ್ಭಾವ "opencv2/core/bufferpool.hpp"

ನಾಮಾಕಾಶ cv {

ವರ್ಗ DummyBufferPoolController : ಸಾರ್ವಜನಿಕ BufferPoolController
{
ಸಾರ್ವಜನಿಕ:
    DummyBufferPoolController() { }
    ಆಭಾಸ ~DummyBufferPoolController() { }

    ಆಭಾಸ size_t getReservedSize() ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ (size_t)-1; }
    ಆಭಾಸ size_t getMaxReservedSize() ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ (size_t)-1; }
    ಆಭಾಸ ಶೂನ್ಯ setMaxReservedSize(size_t size) CV_OVERRIDE { CV_UNUSED(size); }
    ಆಭಾಸ ಶೂನ್ಯ freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
