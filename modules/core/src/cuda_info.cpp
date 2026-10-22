/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#अन्तर्भाव "precomp.hpp"

उपयुज्य नामाकाश cv;
उपयुज्य नामाकाश cv::cuda;

पूर्णाङ्क cv::cuda::get_Cuda_Enabled_Device_गणनम्()
{
#यदि_अपरिभाषितम् HAVE_CUDA
    प्रत्यागम 0;
#अन्यथा
    पूर्णाङ्क गणनम्_2;
    cudaError_t error = cuda_Get_Device_गणनम्(&गणनम्_2);

    यदि (error == cudaErrorInsufficientDriver)
        प्रत्यागम -1;

    यदि (error == cudaErrorNoDevice)
        प्रत्यागम 0;

    cudaSafeCall( error );
    प्रत्यागम गणनम्_2;
#यदि_अन्तः
}

शून्य cv::cuda::समुच्चयः_Device(पूर्णाङ्क device)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(device);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cuda_समुच्चयः_Device(device) );
    cudaSafeCall( cudaFree(0) );
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::getDevice()
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णाङ्क device;
    cudaSafeCall( cudaGetDevice(&device) );
    प्रत्यागम device;
#यदि_अन्तः
}

शून्य cv::cuda::resetDevice()
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaDeviceReset() );
#यदि_अन्तः
}

तार्किक cv::cuda::deviceSupports(Feature_समुच्चयः feature_समुच्चयः)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(feature_समुच्चयः);
    throw_no_cuda();
#अन्यथा
    स्थिर पूर्णाङ्क versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    स्थिर ध्रुव पूर्णाङ्क cache_आकार = static_cast<पूर्णाङ्क>(परिमाण(versions) / परिमाण(versions[0]));

    ध्रुव पूर्णाङ्क devId = getDevice();

    पूर्णाङ्क संस्करणम्;

    यदि (devId < cache_आकार && versions[devId] >= 0)
    {
        संस्करणम् = versions[devId];
    }
    अन्यथा
    {
        DeviceInfo dev(devId);
        संस्करणम् = dev.major_संस्करणम्() * 10 + dev.minor_संस्करणम्();
        यदि (devId < cache_आकार)
            versions[devId] = संस्करणम्;
    }

    प्रत्यागम TargetArchs::builtWith(feature_समुच्चयः) && (संस्करणम् >= feature_समुच्चयः);
#यदि_अन्तः
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश
{
    वर्ग CudaArch
    {
    सार्वजनिक:
        CudaArch();

        तार्किक builtWith(Feature_समुच्चयः feature_समुच्चयः) ध्रुव;
        तार्किक hasPtx(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव;
        तार्किक hasBin(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव;
        तार्किक hasEqualOrLessPtx(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव;
        तार्किक hasEqualOrGreaterPtx(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव;
        तार्किक hasEqualOrGreaterBin(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव;

    गुप्त:
        स्थिर शून्य fromStr(ध्रुव वर्ण* समुच्चयः_as_str, std::vector<पूर्णाङ्क>& arr);

        std::vector<पूर्णाङ्क> bin;
        std::vector<पूर्णाङ्क> ptx;
        std::vector<पूर्णाङ्क> features;
    };

    ध्रुव CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    तार्किक CudaArch::builtWith(Feature_समुच्चयः feature_समुच्चयः) ध्रुव
    {
        प्रत्यागम !features.empty() && (features.back() >= feature_समुच्चयः);
    }

    तार्किक CudaArch::hasPtx(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव
    {
        प्रत्यागम std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    तार्किक CudaArch::hasBin(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव
    {
        प्रत्यागम std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    तार्किक CudaArch::hasEqualOrLessPtx(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव
    {
        प्रत्यागम !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    तार्किक CudaArch::hasEqualOrGreaterPtx(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव
    {
        प्रत्यागम !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    तार्किक CudaArch::hasEqualOrGreaterBin(पूर्णाङ्क major, पूर्णाङ्क minor) ध्रुव
    {
        प्रत्यागम !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    शून्य CudaArch::fromStr(ध्रुव वर्ण* समुच्चयः_as_str, std::vector<पूर्णाङ्क>& arr)
    {
        arr.clear();

        ध्रुव size_t len = वाक्यदैर्घ्यम्(समुच्चयः_as_str);

        size_t pos = 0;
        यावत् (pos < len)
        {
            यदि (रिक्तम्_वा(समुच्चयः_as_str[pos]))
            {
                ++pos;
            }
            अन्यथा
            {
                पूर्णाङ्क cur_मानम्;
                पूर्णाङ्क chars_read;
                पूर्णाङ्क तर्काः_read = वाक्यात्_पठ(समुच्चयः_as_str + pos, "%d%n", &cur_मानम्, &chars_read);
                CV_Assert( तर्काः_read == 1 );

                arr.push_back(cur_मानम्);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#यदि_अन्तः

तार्किक cv::cuda::TargetArchs::builtWith(cv::cuda::Feature_समुच्चयः feature_समुच्चयः)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(feature_समुच्चयः);
    throw_no_cuda();
#अन्यथा
    प्रत्यागम cudaArch.builtWith(feature_समुच्चयः);
#यदि_अन्तः
}

तार्किक cv::cuda::TargetArchs::hasPtx(पूर्णाङ्क major, पूर्णाङ्क minor)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(major);
    CV_अप्रयुक्तम्(minor);
    throw_no_cuda();
#अन्यथा
    प्रत्यागम cudaArch.hasPtx(major, minor);
#यदि_अन्तः
}

तार्किक cv::cuda::TargetArchs::hasBin(पूर्णाङ्क major, पूर्णाङ्क minor)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(major);
    CV_अप्रयुक्तम्(minor);
    throw_no_cuda();
#अन्यथा
    प्रत्यागम cudaArch.hasBin(major, minor);
#यदि_अन्तः
}

तार्किक cv::cuda::TargetArchs::hasEqualOrLessPtx(पूर्णाङ्क major, पूर्णाङ्क minor)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(major);
    CV_अप्रयुक्तम्(minor);
    throw_no_cuda();
#अन्यथा
    प्रत्यागम cudaArch.hasEqualOrLessPtx(major, minor);
#यदि_अन्तः
}

तार्किक cv::cuda::TargetArchs::hasEqualOrGreaterPtx(पूर्णाङ्क major, पूर्णाङ्क minor)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(major);
    CV_अप्रयुक्तम्(minor);
    throw_no_cuda();
#अन्यथा
    प्रत्यागम cudaArch.hasEqualOrGreaterPtx(major, minor);
#यदि_अन्तः
}

तार्किक cv::cuda::TargetArchs::hasEqualOrGreaterBin(पूर्णाङ्क major, पूर्णाङ्क minor)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(major);
    CV_अप्रयुक्तम्(minor);
    throw_no_cuda();
#अन्यथा
    प्रत्यागम cudaArch.hasEqualOrGreaterBin(major, minor);
#यदि_अन्तः
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश
{
    वर्ग DeviceProps
    {
    सार्वजनिक:
        DeviceProps();

        ध्रुव cudaDeviceProp* get(पूर्णाङ्क devID) ध्रुव;

    गुप्त:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        पूर्णाङ्क गणनम्_2 = get_Cuda_Enabled_Device_गणनम्();

        यदि (गणनम्_2 > 0)
        {
            props_.resize(गणनम्_2);

            प्रति (पूर्णाङ्क devID = 0; devID < गणनम्_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ध्रुव cudaDeviceProp* DeviceProps::get(पूर्णाङ्क devID) ध्रुव
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        प्रत्यागम &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        स्थिर DeviceProps props;
        प्रत्यागम props;
    }
}

#यदि_अन्तः

ध्रुव वर्ण* cv::cuda::DeviceInfo::name() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->name;
#यदि_अन्तः
}

size_t cv::cuda::DeviceInfo::योग_Global_Mem() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->योग_Global_Mem;
#यदि_अन्तः
}

size_t cv::cuda::DeviceInfo::shared_Mem_प्रत्येकम्_खण्डम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->shared_Mem_प्रत्येकम्_खण्डम्;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::regs_प्रत्येकम्_खण्डम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->regs_प्रत्येकम्_खण्डम्;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::warp_आकार() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->warp_आकार;
#यदि_अन्तः
}

size_t cv::cuda::DeviceInfo::memPitch() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->memPitch;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::max_तन्तवः_प्रत्येकम्_खण्डम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->max_तन्तवः_प्रत्येकम्_खण्डम्;
#यदि_अन्तः
}

Vec3i cv::cuda::DeviceInfo::max_तन्तवः_Dim() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec3i(deviceProps().get(device_id_)->max_तन्तवः_Dim);
#यदि_अन्तः
}

Vec3i cv::cuda::DeviceInfo::max_Grid_आकार() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec3i(deviceProps().get(device_id_)->max_Grid_आकार);
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::clock_दरः() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णाङ्क clock_दरः;
    cudaSafeCall(cudaDeviceGetAttribute(&clock_दरः, cuda_Dev_Attr_Clock_दरः, device_id_));
    प्रत्यागम clock_दरः;
#यदि_अन्तः
}

size_t cv::cuda::DeviceInfo::योग_Const_Mem() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->योग_Const_Mem;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::major_संस्करणम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->major;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::minor_संस्करणम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->minor;
#यदि_अन्तः
}

size_t cv::cuda::DeviceInfo::textureAlignment() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->textureAlignment;
#यदि_अन्तः
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->texturePitchAlignment;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::multi_Processor_गणनम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->multi_Processor_गणनम्;
#यदि_अन्तः
}

तार्किक cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णाङ्क kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    प्रत्यागम kernelExecTimeoutEnabled != 0;
#यदि_अन्तः
}

तार्किक cv::cuda::DeviceInfo::integrated() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->integrated != 0;
#यदि_अन्तः
}

तार्किक cv::cuda::DeviceInfo::can_Map_Host_स्मृतिः() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->can_Map_Host_स्मृतिः != 0;
#यदि_अन्तः
}

DeviceInfo::Compute_बहुलकम् cv::cuda::DeviceInfo::compute_बहुलकम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    स्थिर ध्रुव Compute_बहुलकम् tbl[] =
    {
        Compute_बहुलकम्_Default,
        Compute_बहुलकम्_Exclusive,
        Compute_बहुलकम्_Prohibited,
        Compute_बहुलकम्_Exclusive_Process
    };

    पूर्णाङ्क compute_बहुलकम्;
    cudaSafeCall(cudaDeviceGetAttribute(&compute_बहुलकम्, cuda_Dev_Attr_Compute_बहुलकम्, device_id_));
    प्रत्यागम tbl[compute_बहुलकम्];
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::maxTexture1D() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->maxTexture1D;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::maxTexture1DMipmap() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #यदि CUDA_संस्करणम् >= 5000
        प्रत्यागम deviceProps().get(device_id_)->maxTexture1DMipmap;
    #अन्यथा
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        प्रत्यागम 0;
    #यदि_अन्तः
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::maxTexture1DLinear() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #यदि CUDA_संस्करणम् >= 13000
        size_t max_विस्तारः_अन्तर्_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cuda_Device_Get_Texture1_DLinear_Max_विस्तारः(&max_विस्तारः_अन्तर्_Elements, &fmtDesc, device_id_));
        प्रत्यागम max_विस्तारः_अन्तर्_Elements;
    #अन्यथा
        प्रत्यागम deviceProps().get(device_id_)->maxTexture1DLinear;
    #यदि_अन्तः
#यदि_अन्तः
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#यदि_अन्तः
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #यदि CUDA_संस्करणम् >= 5000
        प्रत्यागम Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #अन्यथा
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        प्रत्यागम Vec2i();
    #यदि_अन्तः
#यदि_अन्तः
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#यदि_अन्तः
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#यदि_अन्तः
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::maxTextureCubemap() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->maxTextureCubemap;
#यदि_अन्तः
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#यदि_अन्तः
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#यदि_अन्तः
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::maxSurface1D() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->maxSurface1D;
#यदि_अन्तः
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#यदि_अन्तः
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#यदि_अन्तः
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#यदि_अन्तः
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::maxSurfaceCubemap() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->maxSurfaceCubemap;
#यदि_अन्तः
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#यदि_अन्तः
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->surfaceAlignment;
#यदि_अन्तः
}

तार्किक cv::cuda::DeviceInfo::concurrentKernels() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->concurrentKernels != 0;
#यदि_अन्तः
}

तार्किक cv::cuda::DeviceInfo::ECCEnabled() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->ECCEnabled != 0;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::pciBusID() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->pciBusID;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::pciDeviceID() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->pciDeviceID;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::pciDomainID() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->pciDomainID;
#यदि_अन्तः
}

तार्किक cv::cuda::DeviceInfo::tccDriver() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->tccDriver != 0;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::async_Engine_गणनम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->async_Engine_गणनम्;
#यदि_अन्तः
}

तार्किक cv::cuda::DeviceInfo::unifiedAddressing() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->unifiedAddressing != 0;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::स्मृतिः_Clock_दरः() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णाङ्क स्मृतिः_Clock_दरः;
    cudaSafeCall(cudaDeviceGetAttribute(&स्मृतिः_Clock_दरः, cuda_Dev_Attr_स्मृतिः_Clock_दरः, device_id_));
    प्रत्यागम स्मृतिः_Clock_दरः;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::स्मृतिः_Bus_विस्तारः() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->स्मृतिः_Bus_विस्तारः;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::l2_Cache_आकार() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->l2_Cache_आकार;
#यदि_अन्तः
}

पूर्णाङ्क cv::cuda::DeviceInfo::max_तन्तवः_प्रत्येकम्_Multi_Processor() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    प्रत्यागम deviceProps().get(device_id_)->max_तन्तवः_प्रत्येकम्_Multi_Processor;
#यदि_अन्तः
}

शून्य cv::cuda::DeviceInfo::query_स्मृतिः(size_t& योग_स्मृतिः, size_t& free_स्मृतिः) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(योग_स्मृतिः);
    CV_अप्रयुक्तम्(free_स्मृतिः);
    throw_no_cuda();
#अन्यथा
    पूर्णाङ्क पूर्व_Device_ID = getDevice();
    यदि (पूर्व_Device_ID != device_id_)
        समुच्चयः_Device(device_id_);

    cudaSafeCall( cudaMemGetInfo(&free_स्मृतिः, &योग_स्मृतिः) );

    यदि (पूर्व_Device_ID != device_id_)
        समुच्चयः_Device(पूर्व_Device_ID);
#यदि_अन्तः
}

तार्किक cv::cuda::DeviceInfo::isCompatible() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    // Check PTX compatibility
    यदि (TargetArchs::hasEqualOrLessPtx(major_संस्करणम्(), minor_संस्करणम्()))
        प्रत्यागम सत्य;

    // Check BIN compatibility
    प्रति (पूर्णाङ्क i = minor_संस्करणम्(); i >= 0; --i)
        यदि (TargetArchs::hasBin(major_संस्करणम्(), i))
            प्रत्यागम सत्य;

    प्रत्यागम असत्य;
#यदि_अन्तः
}

////////////////////////////////////////////////////////////////////////
// print info

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश
{
    पूर्णाङ्क convertSMVer2Cores(पूर्णाङ्क major, पूर्णाङ्क minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        प्रकारनिर्देश संरचना {
            पूर्णाङ्क SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            पूर्णाङ्क Cores;
        } SMtoCores;

        SMtoCores gpu_Arch_Cores_प्रत्येकम्_SM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        पूर्णाङ्क सूचकांक = 0;
        यावत् (gpu_Arch_Cores_प्रत्येकम्_SM[सूचकांक].SM != -1)
        {
            यदि (gpu_Arch_Cores_प्रत्येकम्_SM[सूचकांक].SM == ((major << 4) + minor) )
                प्रत्यागम gpu_Arch_Cores_प्रत्येकम्_SM[सूचकांक].Cores;
            सूचकांक++;
        }

        प्रत्यागम -1;
    }
}

#यदि_अन्तः

शून्य cv::cuda::printCudaDeviceInfo(पूर्णाङ्क device)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(device);
    throw_no_cuda();
#अन्यथा
    पूर्णाङ्क गणनम्_2 = get_Cuda_Enabled_Device_गणनम्();
    तार्किक valid = (device >= 0) && (device < गणनम्_2);

    पूर्णाङ्क beg = valid ? device   : 0;
    पूर्णाङ्क end = valid ? device+1 : गणनम्_2;

    प्रारूप_लिख("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    प्रारूप_लिख("Device count: %d\n", गणनम्_2);

    पूर्णाङ्क driver_संस्करणम् = 0, runtime_संस्करणम् = 0;
    cudaSafeCall( cuda_Driver_Get_संस्करणम्(&driver_संस्करणम्) );
    cudaSafeCall( cuda_Runtime_Get_संस्करणम्(&runtime_संस्करणम्) );

    ध्रुव वर्ण *compute_बहुलकम्[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        रिक्त
    };

    प्रति(पूर्णाङ्क dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        प्रारूप_लिख("\nDevice %d: \"%s\"\n", dev, prop.name);
        प्रारूप_लिख("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_संस्करणम्/1000, driver_संस्करणम्%100, runtime_संस्करणम्/1000, runtime_संस्करणम्%100);
        प्रारूप_लिख("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        प्रारूप_लिख("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (भिन्न)prop.योग_Global_Mem/1048576.0f, (अचिह्नित दीर्घ दीर्घ) prop.योग_Global_Mem);

        पूर्णाङ्क cores = convertSMVer2Cores(prop.major, prop.minor);
        यदि (cores > 0)
            प्रारूप_लिख("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_गणनम्, cores, cores * prop.multi_Processor_गणनम्);

        पूर्णाङ्क clock_दरः;
        cudaSafeCall(cudaDeviceGetAttribute(&clock_दरः, cuda_Dev_Attr_Clock_दरः, dev));
        प्रारूप_लिख("  GPU Clock Speed:                               %.2f GHz\n", clock_दरः * 1e-6f);

        प्रारूप_लिख("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        प्रारूप_लिख("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        प्रारूप_लिख("  Total amount of constant memory:               %u bytes\n", (पूर्णाङ्क)prop.योग_Const_Mem);
        प्रारूप_लिख("  Total amount of shared memory per block:       %u bytes\n", (पूर्णाङ्क)prop.shared_Mem_प्रत्येकम्_खण्डम्);
        प्रारूप_लिख("  Total number of registers available per block: %d\n", prop.regs_प्रत्येकम्_खण्डम्);
        प्रारूप_लिख("  Warp size:                                     %d\n", prop.warp_आकार);
        प्रारूप_लिख("  Maximum number of threads per block:           %d\n", prop.max_तन्तवः_प्रत्येकम्_खण्डम्);
        प्रारूप_लिख("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_तन्तवः_Dim[0], prop.max_तन्तवः_Dim[1], prop.max_तन्तवः_Dim[2]);
        प्रारूप_लिख("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_आकार[0], prop.max_Grid_आकार[1],  prop.max_Grid_आकार[2]);
        प्रारूप_लिख("  Maximum memory pitch:                          %u bytes\n", (पूर्णाङ्क)prop.memPitch);
        प्रारूप_लिख("  Texture alignment:                             %u bytes\n", (पूर्णाङ्क)prop.textureAlignment);

        प्रारूप_लिख("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_गणनम् ? "Yes" : "No"), prop.async_Engine_गणनम्);
        पूर्णाङ्क kernelExecTimeoutEnabled;
        cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        प्रारूप_लिख("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        प्रारूप_लिख("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        प्रारूप_लिख("  Support host page-locked memory mapping:       %s\n", prop.can_Map_Host_स्मृतिः ? "Yes" : "No");

        प्रारूप_लिख("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        प्रारूप_लिख("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        प्रारूप_लिख("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        प्रारूप_लिख("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        प्रारूप_लिख("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        प्रारूप_लिख("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        पूर्णाङ्क prop_Compute_बहुलकम्;
        cudaSafeCall(cudaDeviceGetAttribute(&prop_Compute_बहुलकम्, cuda_Dev_Attr_Compute_बहुलकम्, dev));
        प्रारूप_लिख("  Compute Mode:\n");
        प्रारूप_लिख("      %s \n", compute_बहुलकम्[prop_Compute_बहुलकम्]);
    }

    प्रारूप_लिख("\n");
    प्रारूप_लिख("deviceQuery, CUDA Driver = CUDART");
    प्रारूप_लिख(", CUDA Driver Version  = %d.%d", driver_संस्करणम् / 1000, driver_संस्करणम् % 100);
    प्रारूप_लिख(", CUDA Runtime Version = %d.%d", runtime_संस्करणम्/1000, runtime_संस्करणम्%100);
    प्रारूप_लिख(", NumDevs = %d\n\n", गणनम्_2);

    fflush(stdout);
#यदि_अन्तः
}

शून्य cv::cuda::printShortCudaDeviceInfo(पूर्णाङ्क device)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(device);
    throw_no_cuda();
#अन्यथा
    पूर्णाङ्क गणनम्_2 = get_Cuda_Enabled_Device_गणनम्();
    तार्किक valid = (device >= 0) && (device < गणनम्_2);

    पूर्णाङ्क beg = valid ? device   : 0;
    पूर्णाङ्क end = valid ? device+1 : गणनम्_2;

    पूर्णाङ्क driver_संस्करणम् = 0, runtime_संस्करणम् = 0;
    cudaSafeCall( cuda_Driver_Get_संस्करणम्(&driver_संस्करणम्) );
    cudaSafeCall( cuda_Runtime_Get_संस्करणम्(&runtime_संस्करणम्) );

    प्रति(पूर्णाङ्क dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        ध्रुव वर्ण *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        प्रारूप_लिख("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (भिन्न)prop.योग_Global_Mem/1048576.0f);
        प्रारूप_लिख(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        पूर्णाङ्क cores = convertSMVer2Cores(prop.major, prop.minor);
        यदि (cores > 0)
            प्रारूप_लिख(", %d cores", cores * prop.multi_Processor_गणनम्);

        प्रारूप_लिख(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_संस्करणम्/1000, driver_संस्करणम्%100, runtime_संस्करणम्/1000, runtime_संस्करणम्%100);
    }

    fflush(stdout);
#यदि_अन्तः
}

////////////////////////////////////////////////////////////////////////
// Error handling

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश
{
    #परिभाषय error_प्रविष्टिः(प्रविष्टिः)  { प्रविष्टिः, #entry }

    संरचना Error_प्रविष्टिः
    {
        पूर्णाङ्क संहिता;
        ध्रुव वर्ण* str;
    };

    संरचना Error_प्रविष्टिः_Comparer
    {
        पूर्णाङ्क संहिता;
        Error_प्रविष्टिः_Comparer(पूर्णाङ्क संहिता_2) : संहिता(संहिता_2) {}
        तार्किक संक्रियकः()(ध्रुव Error_प्रविष्टिः& e) ध्रुव { प्रत्यागम e.संहिता == संहिता; }
    };

    ध्रुव Error_प्रविष्टिः npp_errors [] =
    {
    #यदि defined (_MSC_VER)
        error_प्रविष्टिः( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #यदि_अन्तः

    #यदि NPP_संस्करणम् < 5500
        error_प्रविष्टिः( NPP_BAD_ARG_ERROR ),
        error_प्रविष्टिः( NPP_COEFF_ERROR ),
        error_प्रविष्टिः( NPP_RECT_ERROR ),
        error_प्रविष्टिः( NPP_QUAD_ERROR ),
        error_प्रविष्टिः( NPP_MEMFREE_ERR ),
        error_प्रविष्टिः( NPP_MEMSET_ERR ),
        error_प्रविष्टिः( NPP_MEM_ALLOC_ERR ),
        error_प्रविष्टिः( NPP_HISTO_सङ्ख्या_सम्बन्धि_स्तराः_ERROR ),
        error_प्रविष्टिः( NPP_MIRROR_FLIP_ERR ),
        error_प्रविष्टिः( NPP_INVALID_निवेशः ),
        error_प्रविष्टिः( NPP_POINTER_ERROR ),
        error_प्रविष्टिः( NPP_WARNING ),
        error_प्रविष्टिः( NPP_ODD_ROI_WARNING ),
    #अन्यथा
        error_प्रविष्टिः( NPP_INVALID_HOST_POINTER_ERROR ),
        error_प्रविष्टिः( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_प्रविष्टिः( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_प्रविष्टिः( NPP_ZC_बहुलकम्_NOT_SUPPORTED_ERROR ),
        error_प्रविष्टिः( NPP_MEMFREE_ERROR ),
        error_प्रविष्टिः( NPP_MEMSET_ERROR ),
        error_प्रविष्टिः( NPP_QUALITY_सूचकांक_ERROR ),
        error_प्रविष्टिः( NPP_HISTOGRAM_सङ्ख्या_सम्बन्धि_स्तराः_ERROR ),
        error_प्रविष्टिः( NPP_CHANNEL_क्रमः_ERROR ),
        error_प्रविष्टिः( NPP_ZERO_आवरणम्_मानम्_ERROR ),
        error_प्रविष्टिः( NPP_QUADRANGLE_ERROR ),
        error_प्रविष्टिः( NPP_RECTANGLE_ERROR ),
        error_प्रविष्टिः( NPP_COEFFICIENT_ERROR ),
        error_प्रविष्टिः( NPP_सङ्ख्या_सम्बन्धि_CHANNELS_ERROR ),
        error_प्रविष्टिः( NPP_COI_ERROR ),
        error_प्रविष्टिः( NPP_भाजकः_ERROR ),
        error_प्रविष्टिः( NPP_CHANNEL_ERROR ),
        error_प्रविष्टिः( NPP_STRIDE_ERROR ),
        error_प्रविष्टिः( NPP_ANCHOR_ERROR ),
        error_प्रविष्टिः( NPP_आवरणम्_आकार_ERROR ),
        error_प्रविष्टिः( NPP_MIRROR_FLIP_ERROR ),
        error_प्रविष्टिः( NPP_MOMENT_00_ZERO_ERROR ),
        error_प्रविष्टिः( NPP_देहली_NEGATIVE_LEVEL_ERROR ),
        error_प्रविष्टिः( NPP_देहली_ERROR ),
        error_प्रविष्टिः( NPP_CONTEXT_मेलनम्_ERROR ),
        error_प्रविष्टिः( NPP_FFT_FLAG_ERROR ),
        error_प्रविष्टिः( NPP_FFT_क्रमः_ERROR ),
        error_प्रविष्टिः( NPP_SCALE_RANGE_ERROR ),
        error_प्रविष्टिः( NPP_दत्त_TYPE_ERROR ),
        error_प्रविष्टिः( NPP_बहिर्गमनम्_OFF_RANGE_ERROR ),
        error_प्रविष्टिः( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_प्रविष्टिः( NPP_स्मृतिः_आवण्टनम्_ERR ),
        error_प्रविष्टिः( NPP_RANGE_ERROR ),
        error_प्रविष्टिः( NPP_BAD_तर्कः_ERROR ),
        error_प्रविष्टिः( NPP_NO_स्मृतिः_ERROR ),
        error_प्रविष्टिः( NPP_ERROR_RESERVED ),
        error_प्रविष्टिः( NPP_NO_OPERATION_WARNING ),
        error_प्रविष्टिः( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_प्रविष्टिः( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #यदि_अन्तः

        error_प्रविष्टिः( NPP_NOT_SUPPORTED_बहुलकम्_ERROR ),
        error_प्रविष्टिः( NPP_आवर्तनम्_बहुलकम्_NOT_SUPPORTED_ERROR ),
        error_प्रविष्टिः( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_प्रविष्टिः( NPP_LUT_सङ्ख्या_सम्बन्धि_स्तराः_ERROR ),
        error_प्रविष्टिः( NPP_TEXTURE_BIND_ERROR ),
        error_प्रविष्टिः( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_प्रविष्टिः( NPP_NOT_EVEN_चरणः_ERROR ),
        error_प्रविष्टिः( NPP_INTERPOLATION_ERROR ),
        error_प्रविष्टिः( NPP_RESIZE_गुणकः_ERROR ),
        error_प्रविष्टिः( NPP_HAAR_CLASSIFIER_PIXEL_मेलनम्_ERROR ),
        error_प्रविष्टिः( NPP_MEMCPY_ERROR ),
        error_प्रविष्टिः( NPP_ALIGNMENT_ERROR ),
        error_प्रविष्टिः( NPP_चरणः_ERROR ),
        error_प्रविष्टिः( NPP_आकार_ERROR ),
        error_प्रविष्टिः( NPP_NULL_POINTER_ERROR ),
        error_प्रविष्टिः( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_प्रविष्टिः( NPP_NOT_IMPLEMENTED_ERROR ),
        error_प्रविष्टिः( NPP_ERROR ),
        error_प्रविष्टिः( NPP_NO_ERROR ),
        error_प्रविष्टिः( NPP_SUCCESS ),
        error_प्रविष्टिः( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_प्रविष्टिः( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_प्रविष्टिः( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_प्रविष्टिः( NPP_DOUBLE_आकार_WARNING )
    };

    ध्रुव size_t npp_error_अङ्कः = परिमाण(npp_errors) / परिमाण(npp_errors[0]);

    ध्रुव Error_प्रविष्टिः cu_errors [] =
    {
        error_प्रविष्टिः( CUDA_SUCCESS                              ),
        error_प्रविष्टिः( CUDA_ERROR_INVALID_मानम्                  ),
        error_प्रविष्टिः( CUDA_ERROR_बहिर्गमनम्_सम्बन्धि_स्मृतिः                  ),
        error_प्रविष्टिः( CUDA_ERROR_NOT_INITIALIZED                ),
        error_प्रविष्टिः( CUDA_ERROR_DEINITIALIZED                  ),
        error_प्रविष्टिः( CUDA_ERROR_PROFILER_DISABLED              ),
        error_प्रविष्टिः( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_प्रविष्टिः( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_प्रविष्टिः( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_प्रविष्टिः( CUDA_ERROR_NO_DEVICE                      ),
        error_प्रविष्टिः( CUDA_ERROR_INVALID_DEVICE                 ),
        error_प्रविष्टिः( CUDA_ERROR_INVALID_IMAGE                  ),
        error_प्रविष्टिः( CUDA_ERROR_INVALID_CONTEXT                ),
        error_प्रविष्टिः( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_प्रविष्टिः( CUDA_ERROR_MAP_FAILED                     ),
        error_प्रविष्टिः( CUDA_ERROR_UNMAP_FAILED                   ),
        error_प्रविष्टिः( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_प्रविष्टिः( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_प्रविष्टिः( CUDA_ERROR_NO_द्विमान_FOR_GPU              ),
        error_प्रविष्टिः( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_प्रविष्टिः( CUDA_ERROR_NOT_MAPPED                     ),
        error_प्रविष्टिः( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_प्रविष्टिः( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_प्रविष्टिः( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_प्रविष्टिः( CUDA_ERROR_UNSUPPORTED_सीमा              ),
        error_प्रविष्टिः( CUDA_ERROR_CONTEXT_ALREADY_अन्तर्_USE         ),
        error_प्रविष्टिः( CUDA_ERROR_INVALID_SOURCE                 ),
        error_प्रविष्टिः( CUDA_ERROR_FILE_NOT_प्राप्तम्                 ),
        error_प्रविष्टिः( CUDA_ERROR_SHARED_OBJECT_चिह्नम्_NOT_प्राप्तम् ),
        error_प्रविष्टिः( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_प्रविष्टिः( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_प्रविष्टिः( CUDA_ERROR_INVALID_HANDLE                 ),
        error_प्रविष्टिः( CUDA_ERROR_NOT_प्राप्तम्                      ),
        error_प्रविष्टिः( CUDA_ERROR_NOT_READY                      ),
        error_प्रविष्टिः( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_प्रविष्टिः( CUDA_ERROR_LAUNCH_बहिर्गमनम्_सम्बन्धि_RESOURCES        ),
        error_प्रविष्टिः( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_प्रविष्टिः( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_प्रविष्टिः( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_प्रविष्टिः( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_प्रविष्टिः( CUDA_ERROR_PRIMARY_CONTEXT_सक्रिय         ),
        error_प्रविष्टिः( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_प्रविष्टिः( CUDA_ERROR_ASSERT                         ),
        error_प्रविष्टिः( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_प्रविष्टिः( CUDA_ERROR_HOST_स्मृतिः_ALREADY_REGISTERED ),
        error_प्रविष्टिः( CUDA_ERROR_HOST_स्मृतिः_NOT_REGISTERED     ),
        error_प्रविष्टिः( CUDA_ERROR_UNKNOWN                        )
    };

    ध्रुव size_t cu_errors_अङ्कः = परिमाण(cu_errors) / परिमाण(cu_errors[0]);

    cv::String getErrorString(पूर्णाङ्क संहिता, ध्रुव Error_प्रविष्टिः* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_प्रविष्टिः_Comparer(संहिता)) - errors;

        ध्रुव वर्ण* सन्देशः = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", सन्देशः, संहिता);

        प्रत्यागम str;
    }
}

#यदि_अन्तः

String cv::cuda::get_Npp_Error_सन्देशः(पूर्णाङ्क संहिता)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(संहिता);
    प्रत्यागम String();
#अन्यथा
    प्रत्यागम getErrorString(संहिता, npp_errors, npp_error_अङ्कः);
#यदि_अन्तः
}

String cv::cuda::get_Cuda_Driver_Api_Error_सन्देशः(पूर्णाङ्क संहिता)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(संहिता);
    प्रत्यागम String();
#अन्यथा
    प्रत्यागम getErrorString(संहिता, cu_errors, cu_errors_अङ्कः);
#यदि_अन्तः
}
