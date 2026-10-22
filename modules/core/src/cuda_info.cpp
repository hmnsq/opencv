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

#અન્તર્ભાવ "precomp.hpp"

using નામાકાશ cv;
using નામાકાશ cv::cuda;

પૂર્ણાંક cv::cuda::get_Cuda_Enabled_Device_ગણતરી()
{
#ifndef HAVE_CUDA
    પાછા 0;
#નહીંતર
    પૂર્ણાંક ગણતરી_2;
    cudaError_t error = cuda_Get_Device_ગણતરી(&ગણતરી_2);

    જો (error == cudaErrorInsufficientDriver)
        પાછા -1;

    જો (error == cudaErrorNoDevice)
        પાછા 0;

    cudaSafeCall( error );
    પાછા ગણતરી_2;
#endif
}

શૂન્ય cv::cuda::setDevice(પૂર્ણાંક device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#નહીંતર
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#endif
}

પૂર્ણાંક cv::cuda::getDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પૂર્ણાંક device;
    cudaSafeCall( cudaGetDevice(&device) );
    પાછા device;
#endif
}

શૂન્ય cv::cuda::resetDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    cudaSafeCall( cudaDeviceReset() );
#endif
}

તાર્કિક cv::cuda::deviceSupports(FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#નહીંતર
    સ્થિર પૂર્ણાંક versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    સ્થિર અચલ પૂર્ણાંક cache_size = static_cast<પૂર્ણાંક>(કદ(versions) / કદ(versions[0]));

    અચલ પૂર્ણાંક devId = getDevice();

    પૂર્ણાંક version;

    જો (devId < cache_size && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    નહીંતર
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        જો (devId < cache_size)
            versions[devId] = version;
    }

    પાછા TargetArchs::builtWith(feature_set) && (version >= feature_set);
#endif
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ifdef HAVE_CUDA

નામાકાશ
{
    વર્ગ CudaArch
    {
    જાહેર:
        CudaArch();

        તાર્કિક builtWith(FeatureSet feature_set) અચલ;
        તાર્કિક hasPtx(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ;
        તાર્કિક hasBin(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ;
        તાર્કિક hasEqualOrLessPtx(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ;
        તાર્કિક hasEqualOrGreaterPtx(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ;
        તાર્કિક hasEqualOrGreaterBin(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ;

    ખાનગી:
        સ્થિર શૂન્ય fromStr(અચલ અક્ષર* set_as_str, std::vector<પૂર્ણાંક>& arr);

        std::vector<પૂર્ણાંક> bin;
        std::vector<પૂર્ણાંક> ptx;
        std::vector<પૂર્ણાંક> features;
    };

    અચલ CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    તાર્કિક CudaArch::builtWith(FeatureSet feature_set) અચલ
    {
        પાછા !features.empty() && (features.back() >= feature_set);
    }

    તાર્કિક CudaArch::hasPtx(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ
    {
        પાછા std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    તાર્કિક CudaArch::hasBin(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ
    {
        પાછા std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    તાર્કિક CudaArch::hasEqualOrLessPtx(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ
    {
        પાછા !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    તાર્કિક CudaArch::hasEqualOrGreaterPtx(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ
    {
        પાછા !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    તાર્કિક CudaArch::hasEqualOrGreaterBin(પૂર્ણાંક major, પૂર્ણાંક minor) અચલ
    {
        પાછા !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    શૂન્ય CudaArch::fromStr(અચલ અક્ષર* set_as_str, std::vector<પૂર્ણાંક>& arr)
    {
        arr.clear();

        અચલ size_t len = strlen(set_as_str);

        size_t pos = 0;
        જ્યારે (pos < len)
        {
            જો (isspace(set_as_str[pos]))
            {
                ++pos;
            }
            નહીંતર
            {
                પૂર્ણાંક cur_કિંમત;
                પૂર્ણાંક chars_read;
                પૂર્ણાંક args_read = sscanf(set_as_str + pos, "%d%n", &cur_કિંમત, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_કિંમત);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#endif

તાર્કિક cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#નહીંતર
    પાછા cudaArch.builtWith(feature_set);
#endif
}

તાર્કિક cv::cuda::TargetArchs::hasPtx(પૂર્ણાંક major, પૂર્ણાંક minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#નહીંતર
    પાછા cudaArch.hasPtx(major, minor);
#endif
}

તાર્કિક cv::cuda::TargetArchs::hasBin(પૂર્ણાંક major, પૂર્ણાંક minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#નહીંતર
    પાછા cudaArch.hasBin(major, minor);
#endif
}

તાર્કિક cv::cuda::TargetArchs::hasEqualOrLessPtx(પૂર્ણાંક major, પૂર્ણાંક minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#નહીંતર
    પાછા cudaArch.hasEqualOrLessPtx(major, minor);
#endif
}

તાર્કિક cv::cuda::TargetArchs::hasEqualOrGreaterPtx(પૂર્ણાંક major, પૂર્ણાંક minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#નહીંતર
    પાછા cudaArch.hasEqualOrGreaterPtx(major, minor);
#endif
}

તાર્કિક cv::cuda::TargetArchs::hasEqualOrGreaterBin(પૂર્ણાંક major, પૂર્ણાંક minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#નહીંતર
    પાછા cudaArch.hasEqualOrGreaterBin(major, minor);
#endif
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ifdef HAVE_CUDA

નામાકાશ
{
    વર્ગ DeviceProps
    {
    જાહેર:
        DeviceProps();

        અચલ cudaDeviceProp* get(પૂર્ણાંક devID) અચલ;

    ખાનગી:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        પૂર્ણાંક ગણતરી_2 = get_Cuda_Enabled_Device_ગણતરી();

        જો (ગણતરી_2 > 0)
        {
            props_.resize(ગણતરી_2);

            માટે (પૂર્ણાંક devID = 0; devID < ગણતરી_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    અચલ cudaDeviceProp* DeviceProps::get(પૂર્ણાંક devID) અચલ
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        પાછા &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        સ્થિર DeviceProps props;
        પાછા props;
    }
}

#endif

અચલ અક્ષર* cv::cuda::DeviceInfo::name() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->name;
#endif
}

size_t cv::cuda::DeviceInfo::totalGlobalMem() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->totalGlobalMem;
#endif
}

size_t cv::cuda::DeviceInfo::sharedMemPerBlock() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->sharedMemPerBlock;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::regsPerBlock() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->regsPerBlock;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::warpSize() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->warpSize;
#endif
}

size_t cv::cuda::DeviceInfo::memPitch() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->memPitch;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::maxThreadsPerBlock() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->maxThreadsPerBlock;
#endif
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec3i(deviceProps().get(device_id_)->maxThreadsDim);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxGridSize() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec3i(deviceProps().get(device_id_)->maxGridSize);
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::clockRate() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પૂર્ણાંક clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    પાછા clockRate;
#endif
}

size_t cv::cuda::DeviceInfo::totalConstMem() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->totalConstMem;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::majorVersion() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->major;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::minorVersion() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->minor;
#endif
}

size_t cv::cuda::DeviceInfo::textureAlignment() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->textureAlignment;
#endif
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->texturePitchAlignment;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::multi_Processor_ગણતરી() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->multi_Processor_ગણતરી;
#endif
}

તાર્કિક cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પૂર્ણાંક kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    પાછા kernelExecTimeoutEnabled != 0;
#endif
}

તાર્કિક cv::cuda::DeviceInfo::integrated() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->integrated != 0;
#endif
}

તાર્કિક cv::cuda::DeviceInfo::canMapHostMemory() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->canMapHostMemory != 0;
#endif
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    સ્થિર અચલ ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    પૂર્ણાંક computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    પાછા tbl[computeMode];
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::maxTexture1D() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->maxTexture1D;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::maxTexture1DMipmap() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    #જો CUDA_VERSION >= 5000
        પાછા deviceProps().get(device_id_)->maxTexture1DMipmap;
    #નહીંતર
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        પાછા 0;
    #endif
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::maxTexture1DLinear() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    #જો CUDA_VERSION >= 13000
        size_t maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        પાછા maxWidthInElements;
    #નહીંતર
        પાછા deviceProps().get(device_id_)->maxTexture1DLinear;
    #endif
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    #જો CUDA_VERSION >= 5000
        પાછા Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #નહીંતર
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        પાછા Vec2i();
    #endif
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::maxTextureCubemap() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->maxTextureCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::maxSurface1D() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->maxSurface1D;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::maxSurfaceCubemap() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->maxSurfaceCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#endif
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->surfaceAlignment;
#endif
}

તાર્કિક cv::cuda::DeviceInfo::concurrentKernels() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->concurrentKernels != 0;
#endif
}

તાર્કિક cv::cuda::DeviceInfo::ECCEnabled() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->ECCEnabled != 0;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::pciBusID() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->pciBusID;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::pciDeviceID() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->pciDeviceID;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::pciDomainID() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->pciDomainID;
#endif
}

તાર્કિક cv::cuda::DeviceInfo::tccDriver() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->tccDriver != 0;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::async_Engine_ગણતરી() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->async_Engine_ગણતરી;
#endif
}

તાર્કિક cv::cuda::DeviceInfo::unifiedAddressing() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->unifiedAddressing != 0;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::memoryClockRate() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પૂર્ણાંક memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    પાછા memoryClockRate;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::memoryBusWidth() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->memoryBusWidth;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::l2CacheSize() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->l2CacheSize;
#endif
}

પૂર્ણાંક cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    પાછા deviceProps().get(device_id_)->maxThreadsPerMultiProcessor;
#endif
}

શૂન્ય cv::cuda::DeviceInfo::queryMemory(size_t& _totalMemory, size_t& _freeMemory) અચલ
{
#ifndef HAVE_CUDA
    CV_UNUSED(_totalMemory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#નહીંતર
    પૂર્ણાંક prevDeviceID = getDevice();
    જો (prevDeviceID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &_totalMemory) );

    જો (prevDeviceID != device_id_)
        setDevice(prevDeviceID);
#endif
}

તાર્કિક cv::cuda::DeviceInfo::isCompatible() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    // Check PTX compatibility
    જો (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        પાછા સાચું;

    // Check BIN compatibility
    માટે (પૂર્ણાંક i = minorVersion(); i >= 0; --i)
        જો (TargetArchs::hasBin(majorVersion(), i))
            પાછા સાચું;

    પાછા ખોટું;
#endif
}

////////////////////////////////////////////////////////////////////////
// print info

#ifdef HAVE_CUDA

નામાકાશ
{
    પૂર્ણાંક convertSMVer2Cores(પૂર્ણાંક major, પૂર્ણાંક minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        પ્રકારવ્યાખ્યા સંરચના {
            પૂર્ણાંક SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            પૂર્ણાંક Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        પૂર્ણાંક index = 0;
        જ્યારે (gpuArchCoresPerSM[index].SM != -1)
        {
            જો (gpuArchCoresPerSM[index].SM == ((major << 4) + minor) )
                પાછા gpuArchCoresPerSM[index].Cores;
            index++;
        }

        પાછા -1;
    }
}

#endif

શૂન્ય cv::cuda::printCudaDeviceInfo(પૂર્ણાંક device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#નહીંતર
    પૂર્ણાંક ગણતરી_2 = get_Cuda_Enabled_Device_ગણતરી();
    તાર્કિક valid = (device >= 0) && (device < ગણતરી_2);

    પૂર્ણાંક beg = valid ? device   : 0;
    પૂર્ણાંક end = valid ? device+1 : ગણતરી_2;

    printf("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    printf("Device count: %d\n", ગણતરી_2);

    પૂર્ણાંક driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    અચલ અક્ષર *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        ખાલી
    };

    માટે(પૂર્ણાંક dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        printf("\nDevice %d: \"%s\"\n", dev, prop.name);
        printf("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        printf("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        printf("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (દશાંશ)prop.totalGlobalMem/1048576.0f, (unsigned long long) prop.totalGlobalMem);

        પૂર્ણાંક cores = convertSMVer2Cores(prop.major, prop.minor);
        જો (cores > 0)
            printf("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_ગણતરી, cores, cores * prop.multi_Processor_ગણતરી);

        પૂર્ણાંક clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        printf("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        printf("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        printf("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        printf("  Total amount of constant memory:               %u bytes\n", (પૂર્ણાંક)prop.totalConstMem);
        printf("  Total amount of shared memory per block:       %u bytes\n", (પૂર્ણાંક)prop.sharedMemPerBlock);
        printf("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        printf("  Warp size:                                     %d\n", prop.warpSize);
        printf("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        printf("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.maxGridSize[0], prop.maxGridSize[1],  prop.maxGridSize[2]);
        printf("  Maximum memory pitch:                          %u bytes\n", (પૂર્ણાંક)prop.memPitch);
        printf("  Texture alignment:                             %u bytes\n", (પૂર્ણાંક)prop.textureAlignment);

        printf("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_ગણતરી ? "Yes" : "No"), prop.async_Engine_ગણતરી);
        પૂર્ણાંક kernelExecTimeoutEnabled;
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

        પૂર્ણાંક propComputeMode;
        cudaSafeCall(cudaDeviceGetAttribute(&propComputeMode, cudaDevAttrComputeMode, dev));
        printf("  Compute Mode:\n");
        printf("      %s \n", computeMode[propComputeMode]);
    }

    printf("\n");
    printf("deviceQuery, CUDA Driver = CUDART");
    printf(", CUDA Driver Version  = %d.%d", driverVersion / 1000, driverVersion % 100);
    printf(", CUDA Runtime Version = %d.%d", runtimeVersion/1000, runtimeVersion%100);
    printf(", NumDevs = %d\n\n", ગણતરી_2);

    fflush(stdout);
#endif
}

શૂન્ય cv::cuda::printShortCudaDeviceInfo(પૂર્ણાંક device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#નહીંતર
    પૂર્ણાંક ગણતરી_2 = get_Cuda_Enabled_Device_ગણતરી();
    તાર્કિક valid = (device >= 0) && (device < ગણતરી_2);

    પૂર્ણાંક beg = valid ? device   : 0;
    પૂર્ણાંક end = valid ? device+1 : ગણતરી_2;

    પૂર્ણાંક driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    માટે(પૂર્ણાંક dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        અચલ અક્ષર *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        printf("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (દશાંશ)prop.totalGlobalMem/1048576.0f);
        printf(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        પૂર્ણાંક cores = convertSMVer2Cores(prop.major, prop.minor);
        જો (cores > 0)
            printf(", %d cores", cores * prop.multi_Processor_ગણતરી);

        printf(", Driver/Runtime ver.%d.%d/%d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
    }

    fflush(stdout);
#endif
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ifdef HAVE_CUDA

નામાકાશ
{
    #define error_entry(entry)  { entry, #entry }

    સંરચના ErrorEntry
    {
        પૂર્ણાંક code;
        અચલ અક્ષર* str;
    };

    સંરચના ErrorEntryComparer
    {
        પૂર્ણાંક code;
        ErrorEntryComparer(પૂર્ણાંક code_) : code(code_) {}
        તાર્કિક operator()(અચલ ErrorEntry& e) અચલ { પાછા e.code == code; }
    };

    અચલ ErrorEntry npp_errors [] =
    {
    #જો defined (_MSC_VER)
        error_entry( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #endif

    #જો NPP_VERSION < 5500
        error_entry( NPP_BAD_ARG_ERROR ),
        error_entry( NPP_COEFF_ERROR ),
        error_entry( NPP_RECT_ERROR ),
        error_entry( NPP_QUAD_ERROR ),
        error_entry( NPP_MEMFREE_ERR ),
        error_entry( NPP_MEMSET_ERR ),
        error_entry( NPP_MEM_ALLOC_ERR ),
        error_entry( NPP_HISTO_સંખ્યા_OF_LEVELS_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERR ),
        error_entry( NPP_INVALID_આગત ),
        error_entry( NPP_POINTER_ERROR ),
        error_entry( NPP_WARNING ),
        error_entry( NPP_ODD_ROI_WARNING ),
    #નહીંતર
        error_entry( NPP_INVALID_HOST_POINTER_ERROR ),
        error_entry( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_entry( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_entry( NPP_ZC_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_MEMFREE_ERROR ),
        error_entry( NPP_MEMSET_ERROR ),
        error_entry( NPP_QUALITY_INDEX_ERROR ),
        error_entry( NPP_HISTOGRAM_સંખ્યા_OF_LEVELS_ERROR ),
        error_entry( NPP_CHANNEL_ORDER_ERROR ),
        error_entry( NPP_ZERO_MASK_કિંમત_ERROR ),
        error_entry( NPP_QUADRANGLE_ERROR ),
        error_entry( NPP_RECTANGLE_ERROR ),
        error_entry( NPP_COEFFICIENT_ERROR ),
        error_entry( NPP_સંખ્યા_OF_CHANNELS_ERROR ),
        error_entry( NPP_COI_ERROR ),
        error_entry( NPP_ભાજક_ERROR ),
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
        error_entry( NPP_LUT_સંખ્યા_OF_LEVELS_ERROR ),
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

    અચલ size_t npp_error_અંક = કદ(npp_errors) / કદ(npp_errors[0]);

    અચલ ErrorEntry cu_errors [] =
    {
        error_entry( CUDA_SUCCESS                              ),
        error_entry( CUDA_ERROR_INVALID_કિંમત                  ),
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
        error_entry( CUDA_ERROR_NO_દ્વિઅંકી_FOR_GPU              ),
        error_entry( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_entry( CUDA_ERROR_NOT_MAPPED                     ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_entry( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_entry( CUDA_ERROR_UNSUPPORTED_LIMIT              ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_IN_USE         ),
        error_entry( CUDA_ERROR_INVALID_SOURCE                 ),
        error_entry( CUDA_ERROR_FILE_NOT_FOUND                 ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_પ્રતીક_NOT_FOUND ),
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

    અચલ size_t cu_errors_અંક = કદ(cu_errors) / કદ(cu_errors[0]);

    cv::String getErrorString(પૂર્ણાંક code, અચલ ErrorEntry* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        અચલ અક્ષર* સંદેશ = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", સંદેશ, code);

        પાછા str;
    }
}

#endif

String cv::cuda::getNppErrorMessage(પૂર્ણાંક code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    પાછા String();
#નહીંતર
    પાછા getErrorString(code, npp_errors, npp_error_અંક);
#endif
}

String cv::cuda::getCudaDriverApiErrorMessage(પૂર્ણાંક code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    પાછા String();
#નહીંતર
    પાછા getErrorString(code, cu_errors, cu_errors_અંક);
#endif
}
