// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ifndef __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#define __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#अन्तर्भाव "opencv2/core/bufferpool.hpp"

नामाकाश cv {

वर्ग DummyBufferPoolController : सार्वजनिक BufferPoolController
{
सार्वजनिक:
    DummyBufferPoolController() { }
    आभास ~DummyBufferPoolController() { }

    आभास size_t getReservedSize() ध्रुव CV_OVERRIDE { प्रत्यागम (size_t)-1; }
    आभास size_t getMaxReservedSize() ध्रुव CV_OVERRIDE { प्रत्यागम (size_t)-1; }
    आभास शून्य setMaxReservedSize(size_t size) CV_OVERRIDE { CV_UNUSED(size); }
    आभास शून्य freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
