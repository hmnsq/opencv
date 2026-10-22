// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ifndef __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#define __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#ଅନ୍ତର୍ଭାଵ "opencv2/core/bufferpool.hpp"

ନାମାକାଶ cv {

ଶ୍ରେଣୀ DummyBufferPoolController : ସର୍ବସାଧାରଣ BufferPoolController
{
ସର୍ବସାଧାରଣ:
    DummyBufferPoolController() { }
    ଆଭାସୀ ~DummyBufferPoolController() { }

    ଆଭାସୀ size_t getReservedSize() ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ (size_t)-1; }
    ଆଭାସୀ size_t getMaxReservedSize() ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ (size_t)-1; }
    ଆଭାସୀ ଶୂନ୍ୟ setMaxReservedSize(size_t size) CV_OVERRIDE { CV_UNUSED(size); }
    ଆଭାସୀ ଶୂନ୍ୟ freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
