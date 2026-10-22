// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#ವ್ಯಾಖ್ಯಾನಿಸು __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#ಅನ್ತರ್ಭಾವ "opencv2/core/bufferpool.hpp"

ನಾಮಾಕಾಶ cv {

ವರ್ಗ Dummy_ಬಫರ್_Pool_Controller : ಸಾರ್ವಜನಿಕ ಬಫರ್_Pool_Controller
{
ಸಾರ್ವಜನಿಕ:
    Dummy_ಬಫರ್_Pool_Controller() { }
    ವಾಸ್ತವಿಕ ~Dummy_ಬಫರ್_Pool_Controller() { }

    ವಾಸ್ತವಿಕ size_t get_Reserved_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ (size_t)-1; }
    ವಾಸ್ತವಿಕ size_t get_Max_Reserved_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ CV_OVERRIDE { ಹಿಂತಿರುಗಿ (size_t)-1; }
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ ಗಣ_Max_Reserved_ಗಾತ್ರ(size_t size) CV_OVERRIDE { CV_ಬಳಸದ(size); }
    ವಾಸ್ತವಿಕ ಶೂನ್ಯ freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#ಆದರೆ_ಅಂತ್ಯ // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
