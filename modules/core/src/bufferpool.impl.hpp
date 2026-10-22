// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ifndef __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#define __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#அந்தர்பாவ "opencv2/core/bufferpool.hpp"

நாமாகாஶ cv {

வகுப்பு DummyBufferPoolController : பொது BufferPoolController
{
பொது:
    DummyBufferPoolController() { }
    மெய்நிகர் ~DummyBufferPoolController() { }

    மெய்நிகர் size_t getReservedSize() மாறிலி CV_OVERRIDE { திரும்பு (size_t)-1; }
    மெய்நிகர் size_t getMaxReservedSize() மாறிலி CV_OVERRIDE { திரும்பு (size_t)-1; }
    மெய்நிகர் வெறுமை setMaxReservedSize(size_t size) CV_OVERRIDE { CV_UNUSED(size); }
    மெய்நிகர் வெறுமை freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
