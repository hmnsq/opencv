// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#अगर_अघोषित __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#घोषणा __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#समावेश "opencv2/core/bufferpool.hpp"

namespace cv {

श्रेणी Dummy_बफर_Pool_Controller : खुला बफर_Pool_Controller
{
खुला:
    Dummy_बफर_Pool_Controller() { }
    भव ~Dummy_बफर_Pool_Controller() { }

    भव माप_प्रकार get_Reserved_आकार() स्थिर CV_OVERRIDE { वापस (माप_प्रकार)-1; }
    भव माप_प्रकार get_Max_Reserved_आकार() स्थिर CV_OVERRIDE { वापस (माप_प्रकार)-1; }
    भव व्योम set_Max_Reserved_आकार(माप_प्रकार size) CV_OVERRIDE { CV_UNUSED(size); }
    भव व्योम freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#पूर्ण_अगर // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
