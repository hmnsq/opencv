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
    आभासी ~DummyBufferPoolController() { }

    आभासी माप_प्रकार getReservedSize() स्थिरांक CV_OVERRIDE { परत (माप_प्रकार)-1; }
    आभासी माप_प्रकार getMaxReservedSize() स्थिरांक CV_OVERRIDE { परत (माप_प्रकार)-1; }
    आभासी शून्य setMaxReservedSize(माप_प्रकार size) CV_OVERRIDE { CV_UNUSED(size); }
    आभासी शून्य freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
