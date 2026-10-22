// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ifndef __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#define __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#অন্তর্ভাৱ "opencv2/core/bufferpool.hpp"

নামাকাশ cv {

শ্ৰেণী DummyBufferPoolController : সার্ৱজনিক BufferPoolController
{
সার্ৱজনিক:
    DummyBufferPoolController() { }
    আভাস ~DummyBufferPoolController() { }

    আভাস size_t getReservedSize() ধ্রুৱ CV_OVERRIDE { ঘূৰাই (size_t)-1; }
    আভাস size_t getMaxReservedSize() ধ্রুৱ CV_OVERRIDE { ঘূৰাই (size_t)-1; }
    আভাস শূন্য setMaxReservedSize(size_t size) CV_OVERRIDE { CV_UNUSED(size); }
    আভাস শূন্য freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
