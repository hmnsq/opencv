// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2014, Advanced Micro Devices, Inc., all rights reserved.

#నిర్వచితం_కాకపోతే __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
#నిర్వచించు __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__

#అన్తర్భావ "opencv2/core/bufferpool.hpp"

నామాకాశ cv {

తరగతి Dummy_బఫరు_Pool_Controller : బహిరంగ బఫరు_Pool_Controller
{
బహిరంగ:
    Dummy_బఫరు_Pool_Controller() { }
    మాయా ~Dummy_బఫరు_Pool_Controller() { }

    మాయా size_t get_Reserved_కొలత() స్థిరరాశి CV_OVERRIDE { తిరిగి (size_t)-1; }
    మాయా size_t get_Max_Reserved_కొలత() స్థిరరాశి CV_OVERRIDE { తిరిగి (size_t)-1; }
    మాయా శూన్య సమితి_Max_Reserved_కొలత(size_t size) CV_OVERRIDE { CV_వాడని(size); }
    మాయా శూన్య freeAllReservedBuffers() CV_OVERRIDE { }
};

} // namespace

#అయితే_ముగింపు // __OPENCV_CORE_BUFFER_POOL_IMPL_HPP__
