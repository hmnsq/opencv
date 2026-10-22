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

#அந்தர்பாவ "precomp.hpp"

using நாமாகாஶ cv;
using நாமாகாஶ cv::cuda;

முழு cv::cuda::get_Cuda_Enabled_Device_மொத்தம்()
{
#ifndef HAVE_CUDA
    திரும்பு 0;
#இல்லையெனில்
    முழு மொத்தம்_2;
    cudaError_t error = cuda_Get_Device_மொத்தம்(&மொத்தம்_2);

    எனில் (error == cudaErrorInsufficientDriver)
        திரும்பு -1;

    எனில் (error == cudaErrorNoDevice)
        திரும்பு 0;

    cudaSafeCall( error );
    திரும்பு மொத்தம்_2;
#endif
}

வெறுமை cv::cuda::setDevice(முழு device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#இல்லையெனில்
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#endif
}

முழு cv::cuda::getDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    முழு device;
    cudaSafeCall( cudaGetDevice(&device) );
    திரும்பு device;
#endif
}

வெறுமை cv::cuda::resetDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    cudaSafeCall( cudaDeviceReset() );
#endif
}

தருக்க cv::cuda::deviceSupports(FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#இல்லையெனில்
    நிலையான முழு versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    நிலையான மாறிலி முழு cache_size = static_cast<முழு>(அளவு(versions) / அளவு(versions[0]));

    மாறிலி முழு devId = getDevice();

    முழு version;

    எனில் (devId < cache_size && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    இல்லையெனில்
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        எனில் (devId < cache_size)
            versions[devId] = version;
    }

    திரும்பு TargetArchs::builtWith(feature_set) && (version >= feature_set);
#endif
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ifdef HAVE_CUDA

நாமாகாஶ
{
    வகுப்பு CudaArch
    {
    பொது:
        CudaArch();

        தருக்க builtWith(FeatureSet feature_set) மாறிலி;
        தருக்க hasPtx(முழு major, முழு minor) மாறிலி;
        தருக்க hasBin(முழு major, முழு minor) மாறிலி;
        தருக்க hasEqualOrLessPtx(முழு major, முழு minor) மாறிலி;
        தருக்க hasEqualOrGreaterPtx(முழு major, முழு minor) மாறிலி;
        தருக்க hasEqualOrGreaterBin(முழு major, முழு minor) மாறிலி;

    தனிப்பட்ட:
        நிலையான வெறுமை fromStr(மாறிலி எழுத்து* set_as_str, std::vector<முழு>& arr);

        std::vector<முழு> bin;
        std::vector<முழு> ptx;
        std::vector<முழு> features;
    };

    மாறிலி CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    தருக்க CudaArch::builtWith(FeatureSet feature_set) மாறிலி
    {
        திரும்பு !features.empty() && (features.back() >= feature_set);
    }

    தருக்க CudaArch::hasPtx(முழு major, முழு minor) மாறிலி
    {
        திரும்பு std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    தருக்க CudaArch::hasBin(முழு major, முழு minor) மாறிலி
    {
        திரும்பு std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    தருக்க CudaArch::hasEqualOrLessPtx(முழு major, முழு minor) மாறிலி
    {
        திரும்பு !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    தருக்க CudaArch::hasEqualOrGreaterPtx(முழு major, முழு minor) மாறிலி
    {
        திரும்பு !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    தருக்க CudaArch::hasEqualOrGreaterBin(முழு major, முழு minor) மாறிலி
    {
        திரும்பு !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    வெறுமை CudaArch::fromStr(மாறிலி எழுத்து* set_as_str, std::vector<முழு>& arr)
    {
        arr.clear();

        மாறிலி size_t len = strlen(set_as_str);

        size_t pos = 0;
        போது (pos < len)
        {
            எனில் (isspace(set_as_str[pos]))
            {
                ++pos;
            }
            இல்லையெனில்
            {
                முழு cur_மதிப்பு;
                முழு chars_read;
                முழு args_read = sscanf(set_as_str + pos, "%d%n", &cur_மதிப்பு, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_மதிப்பு);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#endif

தருக்க cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு cudaArch.builtWith(feature_set);
#endif
}

தருக்க cv::cuda::TargetArchs::hasPtx(முழு major, முழு minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு cudaArch.hasPtx(major, minor);
#endif
}

தருக்க cv::cuda::TargetArchs::hasBin(முழு major, முழு minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு cudaArch.hasBin(major, minor);
#endif
}

தருக்க cv::cuda::TargetArchs::hasEqualOrLessPtx(முழு major, முழு minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு cudaArch.hasEqualOrLessPtx(major, minor);
#endif
}

தருக்க cv::cuda::TargetArchs::hasEqualOrGreaterPtx(முழு major, முழு minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு cudaArch.hasEqualOrGreaterPtx(major, minor);
#endif
}

தருக்க cv::cuda::TargetArchs::hasEqualOrGreaterBin(முழு major, முழு minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு cudaArch.hasEqualOrGreaterBin(major, minor);
#endif
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ifdef HAVE_CUDA

நாமாகாஶ
{
    வகுப்பு DeviceProps
    {
    பொது:
        DeviceProps();

        மாறிலி cudaDeviceProp* get(முழு devID) மாறிலி;

    தனிப்பட்ட:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        முழு மொத்தம்_2 = get_Cuda_Enabled_Device_மொத்தம்();

        எனில் (மொத்தம்_2 > 0)
        {
            props_.resize(மொத்தம்_2);

            க்கு (முழு devID = 0; devID < மொத்தம்_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    மாறிலி cudaDeviceProp* DeviceProps::get(முழு devID) மாறிலி
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        திரும்பு &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        நிலையான DeviceProps props;
        திரும்பு props;
    }
}

#endif

மாறிலி எழுத்து* cv::cuda::DeviceInfo::name() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->name;
#endif
}

size_t cv::cuda::DeviceInfo::totalGlobalMem() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->totalGlobalMem;
#endif
}

size_t cv::cuda::DeviceInfo::sharedMemPerBlock() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->sharedMemPerBlock;
#endif
}

முழு cv::cuda::DeviceInfo::regsPerBlock() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->regsPerBlock;
#endif
}

முழு cv::cuda::DeviceInfo::warpSize() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->warpSize;
#endif
}

size_t cv::cuda::DeviceInfo::memPitch() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->memPitch;
#endif
}

முழு cv::cuda::DeviceInfo::maxThreadsPerBlock() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->maxThreadsPerBlock;
#endif
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec3i(deviceProps().get(device_id_)->maxThreadsDim);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxGridSize() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec3i(deviceProps().get(device_id_)->maxGridSize);
#endif
}

முழு cv::cuda::DeviceInfo::clockRate() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    முழு clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    திரும்பு clockRate;
#endif
}

size_t cv::cuda::DeviceInfo::totalConstMem() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->totalConstMem;
#endif
}

முழு cv::cuda::DeviceInfo::majorVersion() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->major;
#endif
}

முழு cv::cuda::DeviceInfo::minorVersion() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->minor;
#endif
}

size_t cv::cuda::DeviceInfo::textureAlignment() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->textureAlignment;
#endif
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->texturePitchAlignment;
#endif
}

முழு cv::cuda::DeviceInfo::multi_Processor_மொத்தம்() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->multi_Processor_மொத்தம்;
#endif
}

தருக்க cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    முழு kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    திரும்பு kernelExecTimeoutEnabled != 0;
#endif
}

தருக்க cv::cuda::DeviceInfo::integrated() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->integrated != 0;
#endif
}

தருக்க cv::cuda::DeviceInfo::canMapHostMemory() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->canMapHostMemory != 0;
#endif
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    நிலையான மாறிலி ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    முழு computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    திரும்பு tbl[computeMode];
#endif
}

முழு cv::cuda::DeviceInfo::maxTexture1D() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->maxTexture1D;
#endif
}

முழு cv::cuda::DeviceInfo::maxTexture1DMipmap() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    #எனில் CUDA_VERSION >= 5000
        திரும்பு deviceProps().get(device_id_)->maxTexture1DMipmap;
    #இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        திரும்பு 0;
    #endif
#endif
}

முழு cv::cuda::DeviceInfo::maxTexture1DLinear() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    #எனில் CUDA_VERSION >= 13000
        size_t maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        திரும்பு maxWidthInElements;
    #இல்லையெனில்
        திரும்பு deviceProps().get(device_id_)->maxTexture1DLinear;
    #endif
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    #எனில் CUDA_VERSION >= 5000
        திரும்பு Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        திரும்பு Vec2i();
    #endif
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#endif
}

முழு cv::cuda::DeviceInfo::maxTextureCubemap() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->maxTextureCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#endif
}

முழு cv::cuda::DeviceInfo::maxSurface1D() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->maxSurface1D;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#endif
}

முழு cv::cuda::DeviceInfo::maxSurfaceCubemap() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->maxSurfaceCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#endif
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->surfaceAlignment;
#endif
}

தருக்க cv::cuda::DeviceInfo::concurrentKernels() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->concurrentKernels != 0;
#endif
}

தருக்க cv::cuda::DeviceInfo::ECCEnabled() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->ECCEnabled != 0;
#endif
}

முழு cv::cuda::DeviceInfo::pciBusID() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->pciBusID;
#endif
}

முழு cv::cuda::DeviceInfo::pciDeviceID() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->pciDeviceID;
#endif
}

முழு cv::cuda::DeviceInfo::pciDomainID() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->pciDomainID;
#endif
}

தருக்க cv::cuda::DeviceInfo::tccDriver() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->tccDriver != 0;
#endif
}

முழு cv::cuda::DeviceInfo::async_Engine_மொத்தம்() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->async_Engine_மொத்தம்;
#endif
}

தருக்க cv::cuda::DeviceInfo::unifiedAddressing() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->unifiedAddressing != 0;
#endif
}

முழு cv::cuda::DeviceInfo::memoryClockRate() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    முழு memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    திரும்பு memoryClockRate;
#endif
}

முழு cv::cuda::DeviceInfo::memoryBusWidth() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->memoryBusWidth;
#endif
}

முழு cv::cuda::DeviceInfo::l2CacheSize() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->l2CacheSize;
#endif
}

முழு cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு deviceProps().get(device_id_)->maxThreadsPerMultiProcessor;
#endif
}

வெறுமை cv::cuda::DeviceInfo::queryMemory(size_t& _totalMemory, size_t& _freeMemory) மாறிலி
{
#ifndef HAVE_CUDA
    CV_UNUSED(_totalMemory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#இல்லையெனில்
    முழு prevDeviceID = getDevice();
    எனில் (prevDeviceID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &_totalMemory) );

    எனில் (prevDeviceID != device_id_)
        setDevice(prevDeviceID);
#endif
}

தருக்க cv::cuda::DeviceInfo::isCompatible() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    // Check PTX compatibility
    எனில் (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        திரும்பு உண்மை;

    // Check BIN compatibility
    க்கு (முழு i = minorVersion(); i >= 0; --i)
        எனில் (TargetArchs::hasBin(majorVersion(), i))
            திரும்பு உண்மை;

    திரும்பு பொய்;
#endif
}

////////////////////////////////////////////////////////////////////////
// print info

#ifdef HAVE_CUDA

நாமாகாஶ
{
    முழு convertSMVer2Cores(முழு major, முழு minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        வகைவரை கட்டமைப்பு {
            முழு SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            முழு Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        முழு index = 0;
        போது (gpuArchCoresPerSM[index].SM != -1)
        {
            எனில் (gpuArchCoresPerSM[index].SM == ((major << 4) + minor) )
                திரும்பு gpuArchCoresPerSM[index].Cores;
            index++;
        }

        திரும்பு -1;
    }
}

#endif

வெறுமை cv::cuda::printCudaDeviceInfo(முழு device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#இல்லையெனில்
    முழு மொத்தம்_2 = get_Cuda_Enabled_Device_மொத்தம்();
    தருக்க valid = (device >= 0) && (device < மொத்தம்_2);

    முழு beg = valid ? device   : 0;
    முழு end = valid ? device+1 : மொத்தம்_2;

    printf("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    printf("Device count: %d\n", மொத்தம்_2);

    முழு driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    மாறிலி எழுத்து *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        வெற்று
    };

    க்கு(முழு dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        printf("\nDevice %d: \"%s\"\n", dev, prop.name);
        printf("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        printf("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        printf("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (தசம)prop.totalGlobalMem/1048576.0f, (unsigned long long) prop.totalGlobalMem);

        முழு cores = convertSMVer2Cores(prop.major, prop.minor);
        எனில் (cores > 0)
            printf("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_மொத்தம், cores, cores * prop.multi_Processor_மொத்தம்);

        முழு clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        printf("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        printf("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        printf("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        printf("  Total amount of constant memory:               %u bytes\n", (முழு)prop.totalConstMem);
        printf("  Total amount of shared memory per block:       %u bytes\n", (முழு)prop.sharedMemPerBlock);
        printf("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        printf("  Warp size:                                     %d\n", prop.warpSize);
        printf("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        printf("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.maxGridSize[0], prop.maxGridSize[1],  prop.maxGridSize[2]);
        printf("  Maximum memory pitch:                          %u bytes\n", (முழு)prop.memPitch);
        printf("  Texture alignment:                             %u bytes\n", (முழு)prop.textureAlignment);

        printf("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_மொத்தம் ? "Yes" : "No"), prop.async_Engine_மொத்தம்);
        முழு kernelExecTimeoutEnabled;
        cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        printf("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        printf("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        printf("  Support host page-locked memory mapping:       %s\n", prop.canMapHostMemory ? "Yes" : "No");

        printf("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        printf("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        printf("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        printf("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        printf("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        printf("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        முழு propComputeMode;
        cudaSafeCall(cudaDeviceGetAttribute(&propComputeMode, cudaDevAttrComputeMode, dev));
        printf("  Compute Mode:\n");
        printf("      %s \n", computeMode[propComputeMode]);
    }

    printf("\n");
    printf("deviceQuery, CUDA Driver = CUDART");
    printf(", CUDA Driver Version  = %d.%d", driverVersion / 1000, driverVersion % 100);
    printf(", CUDA Runtime Version = %d.%d", runtimeVersion/1000, runtimeVersion%100);
    printf(", NumDevs = %d\n\n", மொத்தம்_2);

    fflush(stdout);
#endif
}

வெறுமை cv::cuda::printShortCudaDeviceInfo(முழு device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#இல்லையெனில்
    முழு மொத்தம்_2 = get_Cuda_Enabled_Device_மொத்தம்();
    தருக்க valid = (device >= 0) && (device < மொத்தம்_2);

    முழு beg = valid ? device   : 0;
    முழு end = valid ? device+1 : மொத்தம்_2;

    முழு driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    க்கு(முழு dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        மாறிலி எழுத்து *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        printf("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (தசம)prop.totalGlobalMem/1048576.0f);
        printf(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        முழு cores = convertSMVer2Cores(prop.major, prop.minor);
        எனில் (cores > 0)
            printf(", %d cores", cores * prop.multi_Processor_மொத்தம்);

        printf(", Driver/Runtime ver.%d.%d/%d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
    }

    fflush(stdout);
#endif
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ifdef HAVE_CUDA

நாமாகாஶ
{
    #define error_entry(entry)  { entry, #entry }

    கட்டமைப்பு ErrorEntry
    {
        முழு code;
        மாறிலி எழுத்து* str;
    };

    கட்டமைப்பு ErrorEntryComparer
    {
        முழு code;
        ErrorEntryComparer(முழு code_) : code(code_) {}
        தருக்க operator()(மாறிலி ErrorEntry& e) மாறிலி { திரும்பு e.code == code; }
    };

    மாறிலி ErrorEntry npp_errors [] =
    {
    #எனில் defined (_MSC_VER)
        error_entry( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #endif

    #எனில் NPP_VERSION < 5500
        error_entry( NPP_BAD_ARG_ERROR ),
        error_entry( NPP_COEFF_ERROR ),
        error_entry( NPP_RECT_ERROR ),
        error_entry( NPP_QUAD_ERROR ),
        error_entry( NPP_MEMFREE_ERR ),
        error_entry( NPP_MEMSET_ERR ),
        error_entry( NPP_MEM_ALLOC_ERR ),
        error_entry( NPP_HISTO_எண்_OF_LEVELS_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERR ),
        error_entry( NPP_INVALID_உள்ளீடு ),
        error_entry( NPP_POINTER_ERROR ),
        error_entry( NPP_WARNING ),
        error_entry( NPP_ODD_ROI_WARNING ),
    #இல்லையெனில்
        error_entry( NPP_INVALID_HOST_POINTER_ERROR ),
        error_entry( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_entry( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_entry( NPP_ZC_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_MEMFREE_ERROR ),
        error_entry( NPP_MEMSET_ERROR ),
        error_entry( NPP_QUALITY_INDEX_ERROR ),
        error_entry( NPP_HISTOGRAM_எண்_OF_LEVELS_ERROR ),
        error_entry( NPP_CHANNEL_ORDER_ERROR ),
        error_entry( NPP_ZERO_MASK_மதிப்பு_ERROR ),
        error_entry( NPP_QUADRANGLE_ERROR ),
        error_entry( NPP_RECTANGLE_ERROR ),
        error_entry( NPP_COEFFICIENT_ERROR ),
        error_entry( NPP_எண்_OF_CHANNELS_ERROR ),
        error_entry( NPP_COI_ERROR ),
        error_entry( NPP_வகுஎண்_ERROR ),
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
        error_entry( NPP_LUT_எண்_OF_LEVELS_ERROR ),
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

    மாறிலி size_t npp_error_இலக்கம் = அளவு(npp_errors) / அளவு(npp_errors[0]);

    மாறிலி ErrorEntry cu_errors [] =
    {
        error_entry( CUDA_SUCCESS                              ),
        error_entry( CUDA_ERROR_INVALID_மதிப்பு                  ),
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
        error_entry( CUDA_ERROR_NO_இருமம்_FOR_GPU              ),
        error_entry( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_entry( CUDA_ERROR_NOT_MAPPED                     ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_entry( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_entry( CUDA_ERROR_UNSUPPORTED_LIMIT              ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_IN_USE         ),
        error_entry( CUDA_ERROR_INVALID_SOURCE                 ),
        error_entry( CUDA_ERROR_FILE_NOT_FOUND                 ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_குறியீடு_NOT_FOUND ),
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

    மாறிலி size_t cu_errors_இலக்கம் = அளவு(cu_errors) / அளவு(cu_errors[0]);

    cv::String getErrorString(முழு code, மாறிலி ErrorEntry* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        மாறிலி எழுத்து* செய்தி = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", செய்தி, code);

        திரும்பு str;
    }
}

#endif

String cv::cuda::getNppErrorMessage(முழு code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    திரும்பு String();
#இல்லையெனில்
    திரும்பு getErrorString(code, npp_errors, npp_error_இலக்கம்);
#endif
}

String cv::cuda::getCudaDriverApiErrorMessage(முழு code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    திரும்பு String();
#இல்லையெனில்
    திரும்பு getErrorString(code, cu_errors, cu_errors_இலக்கம்);
#endif
}
