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

using नामाकाश cv;
using नामाकाश cv::cuda;

पूर्णांक cv::cuda::get_Cuda_Enabled_Device_मोजणी()
{
#ifndef HAVE_CUDA
    परत 0;
#अन्यथा
    पूर्णांक मोजणी_2;
    cudaError_t error = cuda_Get_Device_मोजणी(&मोजणी_2);

    जर (error == cudaErrorInsufficientDriver)
        परत -1;

    जर (error == cudaErrorNoDevice)
        परत 0;

    cudaSafeCall( error );
    परत मोजणी_2;
#endif
}

शून्य cv::cuda::setDevice(पूर्णांक device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#endif
}

पूर्णांक cv::cuda::getDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णांक device;
    cudaSafeCall( cudaGetDevice(&device) );
    परत device;
#endif
}

शून्य cv::cuda::resetDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaDeviceReset() );
#endif
}

तार्किक cv::cuda::deviceSupports(FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#अन्यथा
    स्थिर पूर्णांक versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    स्थिर स्थिरांक पूर्णांक cache_size = static_cast<पूर्णांक>(आकार(versions) / आकार(versions[0]));

    स्थिरांक पूर्णांक devId = getDevice();

    पूर्णांक version;

    जर (devId < cache_size && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    अन्यथा
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        जर (devId < cache_size)
            versions[devId] = version;
    }

    परत TargetArchs::builtWith(feature_set) && (version >= feature_set);
#endif
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ifdef HAVE_CUDA

नामाकाश
{
    वर्ग CudaArch
    {
    सार्वजनिक:
        CudaArch();

        तार्किक builtWith(FeatureSet feature_set) स्थिरांक;
        तार्किक hasPtx(पूर्णांक major, पूर्णांक minor) स्थिरांक;
        तार्किक hasBin(पूर्णांक major, पूर्णांक minor) स्थिरांक;
        तार्किक hasEqualOrLessPtx(पूर्णांक major, पूर्णांक minor) स्थिरांक;
        तार्किक hasEqualOrGreaterPtx(पूर्णांक major, पूर्णांक minor) स्थिरांक;
        तार्किक hasEqualOrGreaterBin(पूर्णांक major, पूर्णांक minor) स्थिरांक;

    खाजगी:
        स्थिर शून्य fromStr(स्थिरांक अक्षर* set_as_str, std::vector<पूर्णांक>& arr);

        std::vector<पूर्णांक> bin;
        std::vector<पूर्णांक> ptx;
        std::vector<पूर्णांक> features;
    };

    स्थिरांक CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    तार्किक CudaArch::builtWith(FeatureSet feature_set) स्थिरांक
    {
        परत !features.empty() && (features.back() >= feature_set);
    }

    तार्किक CudaArch::hasPtx(पूर्णांक major, पूर्णांक minor) स्थिरांक
    {
        परत std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    तार्किक CudaArch::hasBin(पूर्णांक major, पूर्णांक minor) स्थिरांक
    {
        परत std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    तार्किक CudaArch::hasEqualOrLessPtx(पूर्णांक major, पूर्णांक minor) स्थिरांक
    {
        परत !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    तार्किक CudaArch::hasEqualOrGreaterPtx(पूर्णांक major, पूर्णांक minor) स्थिरांक
    {
        परत !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    तार्किक CudaArch::hasEqualOrGreaterBin(पूर्णांक major, पूर्णांक minor) स्थिरांक
    {
        परत !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    शून्य CudaArch::fromStr(स्थिरांक अक्षर* set_as_str, std::vector<पूर्णांक>& arr)
    {
        arr.clear();

        स्थिरांक माप_प्रकार len = म_माप(set_as_str);

        माप_प्रकार pos = 0;
        पर्यंत (pos < len)
        {
            जर (है_खाली(set_as_str[pos]))
            {
                ++pos;
            }
            अन्यथा
            {
                पूर्णांक cur_मूल्य;
                पूर्णांक chars_read;
                पूर्णांक args_read = माला_पूछो(set_as_str + pos, "%d%n", &cur_मूल्य, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_मूल्य);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#endif

तार्किक cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#अन्यथा
    परत cudaArch.builtWith(feature_set);
#endif
}

तार्किक cv::cuda::TargetArchs::hasPtx(पूर्णांक major, पूर्णांक minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    परत cudaArch.hasPtx(major, minor);
#endif
}

तार्किक cv::cuda::TargetArchs::hasBin(पूर्णांक major, पूर्णांक minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    परत cudaArch.hasBin(major, minor);
#endif
}

तार्किक cv::cuda::TargetArchs::hasEqualOrLessPtx(पूर्णांक major, पूर्णांक minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    परत cudaArch.hasEqualOrLessPtx(major, minor);
#endif
}

तार्किक cv::cuda::TargetArchs::hasEqualOrGreaterPtx(पूर्णांक major, पूर्णांक minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    परत cudaArch.hasEqualOrGreaterPtx(major, minor);
#endif
}

तार्किक cv::cuda::TargetArchs::hasEqualOrGreaterBin(पूर्णांक major, पूर्णांक minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    परत cudaArch.hasEqualOrGreaterBin(major, minor);
#endif
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ifdef HAVE_CUDA

नामाकाश
{
    वर्ग DeviceProps
    {
    सार्वजनिक:
        DeviceProps();

        स्थिरांक cudaDeviceProp* get(पूर्णांक devID) स्थिरांक;

    खाजगी:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        पूर्णांक मोजणी_2 = get_Cuda_Enabled_Device_मोजणी();

        जर (मोजणी_2 > 0)
        {
            props_.resize(मोजणी_2);

            साठी (पूर्णांक devID = 0; devID < मोजणी_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    स्थिरांक cudaDeviceProp* DeviceProps::get(पूर्णांक devID) स्थिरांक
    {
        CV_Assert( static_cast<माप_प्रकार>(devID) < props_.size() );

        परत &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        स्थिर DeviceProps props;
        परत props;
    }
}

#endif

स्थिरांक अक्षर* cv::cuda::DeviceInfo::name() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->name;
#endif
}

माप_प्रकार cv::cuda::DeviceInfo::totalGlobalMem() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->totalGlobalMem;
#endif
}

माप_प्रकार cv::cuda::DeviceInfo::sharedMemPerBlock() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->sharedMemPerBlock;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::regsPerBlock() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->regsPerBlock;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::warpSize() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->warpSize;
#endif
}

माप_प्रकार cv::cuda::DeviceInfo::memPitch() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->memPitch;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::maxThreadsPerBlock() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->maxThreadsPerBlock;
#endif
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec3i(deviceProps().get(device_id_)->maxThreadsDim);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxGridSize() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec3i(deviceProps().get(device_id_)->maxGridSize);
#endif
}

पूर्णांक cv::cuda::DeviceInfo::clockRate() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णांक clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    परत clockRate;
#endif
}

माप_प्रकार cv::cuda::DeviceInfo::totalConstMem() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->totalConstMem;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::majorVersion() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->major;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::minorVersion() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->minor;
#endif
}

माप_प्रकार cv::cuda::DeviceInfo::textureAlignment() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->textureAlignment;
#endif
}

माप_प्रकार cv::cuda::DeviceInfo::texturePitchAlignment() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->texturePitchAlignment;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::multi_Processor_मोजणी() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->multi_Processor_मोजणी;
#endif
}

तार्किक cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णांक kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    परत kernelExecTimeoutEnabled != 0;
#endif
}

तार्किक cv::cuda::DeviceInfo::integrated() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->integrated != 0;
#endif
}

तार्किक cv::cuda::DeviceInfo::canMapHostMemory() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->canMapHostMemory != 0;
#endif
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    स्थिर स्थिरांक ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    पूर्णांक computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    परत tbl[computeMode];
#endif
}

पूर्णांक cv::cuda::DeviceInfo::maxTexture1D() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->maxTexture1D;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::maxTexture1DMipmap() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #जर CUDA_VERSION >= 5000
        परत deviceProps().get(device_id_)->maxTexture1DMipmap;
    #अन्यथा
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        परत 0;
    #endif
#endif
}

पूर्णांक cv::cuda::DeviceInfo::maxTexture1DLinear() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #जर CUDA_VERSION >= 13000
        माप_प्रकार maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        परत maxWidthInElements;
    #अन्यथा
        परत deviceProps().get(device_id_)->maxTexture1DLinear;
    #endif
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #जर CUDA_VERSION >= 5000
        परत Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #अन्यथा
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        परत Vec2i();
    #endif
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#endif
}

पूर्णांक cv::cuda::DeviceInfo::maxTextureCubemap() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->maxTextureCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#endif
}

पूर्णांक cv::cuda::DeviceInfo::maxSurface1D() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->maxSurface1D;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#endif
}

पूर्णांक cv::cuda::DeviceInfo::maxSurfaceCubemap() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->maxSurfaceCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#endif
}

माप_प्रकार cv::cuda::DeviceInfo::surfaceAlignment() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->surfaceAlignment;
#endif
}

तार्किक cv::cuda::DeviceInfo::concurrentKernels() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->concurrentKernels != 0;
#endif
}

तार्किक cv::cuda::DeviceInfo::ECCEnabled() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->ECCEnabled != 0;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::pciBusID() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->pciBusID;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::pciDeviceID() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->pciDeviceID;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::pciDomainID() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->pciDomainID;
#endif
}

तार्किक cv::cuda::DeviceInfo::tccDriver() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->tccDriver != 0;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::async_Engine_मोजणी() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->async_Engine_मोजणी;
#endif
}

तार्किक cv::cuda::DeviceInfo::unifiedAddressing() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->unifiedAddressing != 0;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::memoryClockRate() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णांक memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    परत memoryClockRate;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::memoryBusWidth() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->memoryBusWidth;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::l2CacheSize() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->l2CacheSize;
#endif
}

पूर्णांक cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    परत deviceProps().get(device_id_)->maxThreadsPerMultiProcessor;
#endif
}

शून्य cv::cuda::DeviceInfo::queryMemory(माप_प्रकार& _totalMemory, माप_प्रकार& _freeMemory) स्थिरांक
{
#ifndef HAVE_CUDA
    CV_UNUSED(_totalMemory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#अन्यथा
    पूर्णांक prevDeviceID = getDevice();
    जर (prevDeviceID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &_totalMemory) );

    जर (prevDeviceID != device_id_)
        setDevice(prevDeviceID);
#endif
}

तार्किक cv::cuda::DeviceInfo::isCompatible() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    // Check PTX compatibility
    जर (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        परत सत्य;

    // Check BIN compatibility
    साठी (पूर्णांक i = minorVersion(); i >= 0; --i)
        जर (TargetArchs::hasBin(majorVersion(), i))
            परत सत्य;

    परत असत्य;
#endif
}

////////////////////////////////////////////////////////////////////////
// print info

#ifdef HAVE_CUDA

नामाकाश
{
    पूर्णांक convertSMVer2Cores(पूर्णांक major, पूर्णांक minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        प्रकारव्याख्या संरचना {
            पूर्णांक SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            पूर्णांक Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        पूर्णांक index = 0;
        पर्यंत (gpuArchCoresPerSM[index].SM != -1)
        {
            जर (gpuArchCoresPerSM[index].SM == ((major << 4) + minor) )
                परत gpuArchCoresPerSM[index].Cores;
            index++;
        }

        परत -1;
    }
}

#endif

शून्य cv::cuda::printCudaDeviceInfo(पूर्णांक device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#अन्यथा
    पूर्णांक मोजणी_2 = get_Cuda_Enabled_Device_मोजणी();
    तार्किक valid = (device >= 0) && (device < मोजणी_2);

    पूर्णांक beg = valid ? device   : 0;
    पूर्णांक end = valid ? device+1 : मोजणी_2;

    म_लिखो("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    म_लिखो("Device count: %d\n", मोजणी_2);

    पूर्णांक driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    स्थिरांक अक्षर *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        रिक्त
    };

    साठी(पूर्णांक dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        म_लिखो("\nDevice %d: \"%s\"\n", dev, prop.name);
        म_लिखो("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        म_लिखो("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        म_लिखो("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (दशांश)prop.totalGlobalMem/1048576.0f, (अचिन्हित दीर्घ दीर्घ) prop.totalGlobalMem);

        पूर्णांक cores = convertSMVer2Cores(prop.major, prop.minor);
        जर (cores > 0)
            म_लिखो("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_मोजणी, cores, cores * prop.multi_Processor_मोजणी);

        पूर्णांक clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        म_लिखो("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        म_लिखो("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        म_लिखो("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        म_लिखो("  Total amount of constant memory:               %u bytes\n", (पूर्णांक)prop.totalConstMem);
        म_लिखो("  Total amount of shared memory per block:       %u bytes\n", (पूर्णांक)prop.sharedMemPerBlock);
        म_लिखो("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        म_लिखो("  Warp size:                                     %d\n", prop.warpSize);
        म_लिखो("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        म_लिखो("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        म_लिखो("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.maxGridSize[0], prop.maxGridSize[1],  prop.maxGridSize[2]);
        म_लिखो("  Maximum memory pitch:                          %u bytes\n", (पूर्णांक)prop.memPitch);
        म_लिखो("  Texture alignment:                             %u bytes\n", (पूर्णांक)prop.textureAlignment);

        म_लिखो("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_मोजणी ? "Yes" : "No"), prop.async_Engine_मोजणी);
        पूर्णांक kernelExecTimeoutEnabled;
        cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        म_लिखो("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        म_लिखो("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        म_लिखो("  Support host page-locked memory mapping:       %s\n", prop.canMapHostMemory ? "Yes" : "No");

        म_लिखो("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        म_लिखो("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        म_लिखो("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        म_लिखो("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        म_लिखो("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        म_लिखो("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        पूर्णांक propComputeMode;
        cudaSafeCall(cudaDeviceGetAttribute(&propComputeMode, cudaDevAttrComputeMode, dev));
        म_लिखो("  Compute Mode:\n");
        म_लिखो("      %s \n", computeMode[propComputeMode]);
    }

    म_लिखो("\n");
    म_लिखो("deviceQuery, CUDA Driver = CUDART");
    म_लिखो(", CUDA Driver Version  = %d.%d", driverVersion / 1000, driverVersion % 100);
    म_लिखो(", CUDA Runtime Version = %d.%d", runtimeVersion/1000, runtimeVersion%100);
    म_लिखो(", NumDevs = %d\n\n", मोजणी_2);

    ख_साफ(मानक_निकास);
#endif
}

शून्य cv::cuda::printShortCudaDeviceInfo(पूर्णांक device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#अन्यथा
    पूर्णांक मोजणी_2 = get_Cuda_Enabled_Device_मोजणी();
    तार्किक valid = (device >= 0) && (device < मोजणी_2);

    पूर्णांक beg = valid ? device   : 0;
    पूर्णांक end = valid ? device+1 : मोजणी_2;

    पूर्णांक driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    साठी(पूर्णांक dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        स्थिरांक अक्षर *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        म_लिखो("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (दशांश)prop.totalGlobalMem/1048576.0f);
        म_लिखो(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        पूर्णांक cores = convertSMVer2Cores(prop.major, prop.minor);
        जर (cores > 0)
            म_लिखो(", %d cores", cores * prop.multi_Processor_मोजणी);

        म_लिखो(", Driver/Runtime ver.%d.%d/%d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
    }

    ख_साफ(मानक_निकास);
#endif
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ifdef HAVE_CUDA

नामाकाश
{
    #define error_entry(entry)  { entry, #entry }

    संरचना ErrorEntry
    {
        पूर्णांक code;
        स्थिरांक अक्षर* str;
    };

    संरचना ErrorEntryComparer
    {
        पूर्णांक code;
        ErrorEntryComparer(पूर्णांक code_) : code(code_) {}
        तार्किक चालक()(स्थिरांक ErrorEntry& e) स्थिरांक { परत e.code == code; }
    };

    स्थिरांक ErrorEntry npp_errors [] =
    {
    #जर defined (_MSC_VER)
        error_entry( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #endif

    #जर NPP_VERSION < 5500
        error_entry( NPP_BAD_ARG_ERROR ),
        error_entry( NPP_COEFF_ERROR ),
        error_entry( NPP_RECT_ERROR ),
        error_entry( NPP_QUAD_ERROR ),
        error_entry( NPP_MEMFREE_ERR ),
        error_entry( NPP_MEMSET_ERR ),
        error_entry( NPP_MEM_ALLOC_ERR ),
        error_entry( NPP_HISTO_संख्या_OF_LEVELS_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERR ),
        error_entry( NPP_INVALID_आदान ),
        error_entry( NPP_POINTER_ERROR ),
        error_entry( NPP_WARNING ),
        error_entry( NPP_ODD_ROI_WARNING ),
    #अन्यथा
        error_entry( NPP_INVALID_HOST_POINTER_ERROR ),
        error_entry( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_entry( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_entry( NPP_ZC_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_MEMFREE_ERROR ),
        error_entry( NPP_MEMSET_ERROR ),
        error_entry( NPP_QUALITY_INDEX_ERROR ),
        error_entry( NPP_HISTOGRAM_संख्या_OF_LEVELS_ERROR ),
        error_entry( NPP_CHANNEL_ORDER_ERROR ),
        error_entry( NPP_ZERO_MASK_मूल्य_ERROR ),
        error_entry( NPP_QUADRANGLE_ERROR ),
        error_entry( NPP_RECTANGLE_ERROR ),
        error_entry( NPP_COEFFICIENT_ERROR ),
        error_entry( NPP_संख्या_OF_CHANNELS_ERROR ),
        error_entry( NPP_COI_ERROR ),
        error_entry( NPP_भाजक_ERROR ),
        error_entry( NPP_CHANNEL_ERROR ),
        error_entry( NPP_STRIDE_ERROR ),
        error_entry( NPP_ANCHOR_ERROR ),
        error_entry( NPP_MASK_SIZE_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERROR ),
        error_entry( NPP_MOMENT_00_ZERO_ERROR ),
        error_entry( NPP_THRESHOLD_NEGATIVE_LEVEL_ERROR ),
        error_entry( NPP_THRESHOLD_ERROR ),
        error_entry( NPP_CONTEXT_MATCH_ERROR ),
        error_entry( NPP_FFT_FLAG_ERROR ),
        error_entry( NPP_FFT_ORDER_ERROR ),
        error_entry( NPP_SCALE_RANGE_ERROR ),
        error_entry( NPP_DATA_TYPE_ERROR ),
        error_entry( NPP_OUT_OFF_RANGE_ERROR ),
        error_entry( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_entry( NPP_MEMORY_ALLOCATION_ERR ),
        error_entry( NPP_RANGE_ERROR ),
        error_entry( NPP_BAD_ARGUMENT_ERROR ),
        error_entry( NPP_NO_MEMORY_ERROR ),
        error_entry( NPP_ERROR_RESERVED ),
        error_entry( NPP_NO_OPERATION_WARNING ),
        error_entry( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_entry( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #endif

        error_entry( NPP_NOT_SUPPORTED_MODE_ERROR ),
        error_entry( NPP_ROUND_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_entry( NPP_LUT_संख्या_OF_LEVELS_ERROR ),
        error_entry( NPP_TEXTURE_BIND_ERROR ),
        error_entry( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_entry( NPP_NOT_EVEN_STEP_ERROR ),
        error_entry( NPP_INTERPOLATION_ERROR ),
        error_entry( NPP_RESIZE_FACTOR_ERROR ),
        error_entry( NPP_HAAR_CLASSIFIER_PIXEL_MATCH_ERROR ),
        error_entry( NPP_MEMCPY_ERROR ),
        error_entry( NPP_ALIGNMENT_ERROR ),
        error_entry( NPP_STEP_ERROR ),
        error_entry( NPP_SIZE_ERROR ),
        error_entry( NPP_NULL_POINTER_ERROR ),
        error_entry( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_entry( NPP_NOT_IMPLEMENTED_ERROR ),
        error_entry( NPP_ERROR ),
        error_entry( NPP_NO_ERROR ),
        error_entry( NPP_SUCCESS ),
        error_entry( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_entry( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_entry( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_entry( NPP_DOUBLE_SIZE_WARNING )
    };

    स्थिरांक माप_प्रकार npp_error_अंक = आकार(npp_errors) / आकार(npp_errors[0]);

    स्थिरांक ErrorEntry cu_errors [] =
    {
        error_entry( CUDA_SUCCESS                              ),
        error_entry( CUDA_ERROR_INVALID_मूल्य                  ),
        error_entry( CUDA_ERROR_OUT_OF_MEMORY                  ),
        error_entry( CUDA_ERROR_NOT_INITIALIZED                ),
        error_entry( CUDA_ERROR_DEINITIALIZED                  ),
        error_entry( CUDA_ERROR_PROFILER_DISABLED              ),
        error_entry( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_entry( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_entry( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_entry( CUDA_ERROR_NO_DEVICE                      ),
        error_entry( CUDA_ERROR_INVALID_DEVICE                 ),
        error_entry( CUDA_ERROR_INVALID_IMAGE                  ),
        error_entry( CUDA_ERROR_INVALID_CONTEXT                ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_entry( CUDA_ERROR_MAP_FAILED                     ),
        error_entry( CUDA_ERROR_UNMAP_FAILED                   ),
        error_entry( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_entry( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_entry( CUDA_ERROR_NO_द्विमान_FOR_GPU              ),
        error_entry( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_entry( CUDA_ERROR_NOT_MAPPED                     ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_entry( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_entry( CUDA_ERROR_UNSUPPORTED_LIMIT              ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_IN_USE         ),
        error_entry( CUDA_ERROR_INVALID_SOURCE                 ),
        error_entry( CUDA_ERROR_FILE_NOT_FOUND                 ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_चिन्ह_NOT_FOUND ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_entry( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_entry( CUDA_ERROR_INVALID_HANDLE                 ),
        error_entry( CUDA_ERROR_NOT_FOUND                      ),
        error_entry( CUDA_ERROR_NOT_READY                      ),
        error_entry( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_entry( CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES        ),
        error_entry( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_entry( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_entry( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_entry( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_entry( CUDA_ERROR_PRIMARY_CONTEXT_ACTIVE         ),
        error_entry( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_entry( CUDA_ERROR_ASSERT                         ),
        error_entry( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_entry( CUDA_ERROR_HOST_MEMORY_ALREADY_REGISTERED ),
        error_entry( CUDA_ERROR_HOST_MEMORY_NOT_REGISTERED     ),
        error_entry( CUDA_ERROR_UNKNOWN                        )
    };

    स्थिरांक माप_प्रकार cu_errors_अंक = आकार(cu_errors) / आकार(cu_errors[0]);

    cv::String getErrorString(पूर्णांक code, स्थिरांक ErrorEntry* errors, माप_प्रकार n)
    {
        माप_प्रकार idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        स्थिरांक अक्षर* संदेश = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", संदेश, code);

        परत str;
    }
}

#endif

String cv::cuda::getNppErrorMessage(पूर्णांक code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    परत String();
#अन्यथा
    परत getErrorString(code, npp_errors, npp_error_अंक);
#endif
}

String cv::cuda::getCudaDriverApiErrorMessage(पूर्णांक code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    परत String();
#अन्यथा
    परत getErrorString(code, cu_errors, cu_errors_अंक);
#endif
}
