// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ifndef __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#define __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#અન્તર્ભાવ "opencv2/core/bufferpool.hpp"

નામાકાશ cv {

વર્ગ DummyBufferPoolController : જાહેર BufferPoolController
{
જાહેર:
    DummyBufferPoolController() { }
    આભાસી ~DummyBufferPoolController() { }

    આભાસી size_t getReservedSize() અચલ CV_OVERRIDE { પાછા (size_t)-1; }
    આભાસી size_t getMaxReservedSize() અચલ CV_OVERRIDE { પાછા (size_t)-1; }
    આભાસી શૂન્ય setMaxReservedSize(size_t size) CV_OVERRIDE { CV_UNUSED(size); }
    આભાસી શૂન્ય freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
