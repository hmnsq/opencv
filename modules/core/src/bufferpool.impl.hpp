// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ifndef __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#define __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#അന്തര്ഭാവ "opencv2/core/bufferpool.hpp"

നാമാകാശ cv {

വർഗ്ഗം DummyBufferPoolController : പൊതു BufferPoolController
{
പൊതു:
    DummyBufferPoolController() { }
    മായാ ~DummyBufferPoolController() { }

    മായാ size_t getReservedSize() സ്ഥിരം CV_OVERRIDE { തിരികെ (size_t)-1; }
    മായാ size_t getMaxReservedSize() സ്ഥിരം CV_OVERRIDE { തിരികെ (size_t)-1; }
    മായാ ശൂന്യം setMaxReservedSize(size_t size) CV_OVERRIDE { CV_UNUSED(size); }
    മായാ ശൂന്യം freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#endif // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
