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

#شامل "precomp.hpp"

using فضاینام cv;
using فضاینام cv::cuda;

صحیح cv::cuda::get_Cuda_Enabled_Device_شمار()
{
#ifndef HAVE_CUDA
    بازگشت 0;
#وگرنه
    صحیح شمار_2;
    cudaError_t error = cuda_Get_Device_شمار(&شمار_2);

    اگر (error == cudaErrorInsufficientDriver)
        بازگشت -1;

    اگر (error == cudaErrorNoDevice)
        بازگشت 0;

    cudaSafeCall( error );
    بازگشت شمار_2;
#endif
}

تهی cv::cuda::setDevice(صحیح device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#وگرنه
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#endif
}

صحیح cv::cuda::getDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    صحیح device;
    cudaSafeCall( cudaGetDevice(&device) );
    بازگشت device;
#endif
}

تهی cv::cuda::resetDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    cudaSafeCall( cudaDeviceReset() );
#endif
}

منطقی cv::cuda::deviceSupports(FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#وگرنه
    ایستا صحیح versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    ایستا ثابت صحیح cache_size = static_cast<صحیح>(اندازه(versions) / اندازه(versions[0]));

    ثابت صحیح devId = getDevice();

    صحیح version;

    اگر (devId < cache_size && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    وگرنه
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        اگر (devId < cache_size)
            versions[devId] = version;
    }

    بازگشت TargetArchs::builtWith(feature_set) && (version >= feature_set);
#endif
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ifdef HAVE_CUDA

فضاینام
{
    رده CudaArch
    {
    عمومی:
        CudaArch();

        منطقی builtWith(FeatureSet feature_set) ثابت;
        منطقی hasPtx(صحیح major, صحیح minor) ثابت;
        منطقی hasBin(صحیح major, صحیح minor) ثابت;
        منطقی hasEqualOrLessPtx(صحیح major, صحیح minor) ثابت;
        منطقی hasEqualOrGreaterPtx(صحیح major, صحیح minor) ثابت;
        منطقی hasEqualOrGreaterBin(صحیح major, صحیح minor) ثابت;

    خصوصی:
        ایستا تهی fromStr(ثابت نویسه* set_as_str, std::vector<صحیح>& arr);

        std::vector<صحیح> bin;
        std::vector<صحیح> ptx;
        std::vector<صحیح> features;
    };

    ثابت CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    منطقی CudaArch::builtWith(FeatureSet feature_set) ثابت
    {
        بازگشت !features.empty() && (features.back() >= feature_set);
    }

    منطقی CudaArch::hasPtx(صحیح major, صحیح minor) ثابت
    {
        بازگشت std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    منطقی CudaArch::hasBin(صحیح major, صحیح minor) ثابت
    {
        بازگشت std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    منطقی CudaArch::hasEqualOrLessPtx(صحیح major, صحیح minor) ثابت
    {
        بازگشت !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    منطقی CudaArch::hasEqualOrGreaterPtx(صحیح major, صحیح minor) ثابت
    {
        بازگشت !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    منطقی CudaArch::hasEqualOrGreaterBin(صحیح major, صحیح minor) ثابت
    {
        بازگشت !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    تهی CudaArch::fromStr(ثابت نویسه* set_as_str, std::vector<صحیح>& arr)
    {
        arr.clear();

        ثابت size_t len = strlen(set_as_str);

        size_t pos = 0;
        وقتی (pos < len)
        {
            اگر (isspace(set_as_str[pos]))
            {
                ++pos;
            }
            وگرنه
            {
                صحیح cur_مقدار;
                صحیح chars_read;
                صحیح args_read = sscanf(set_as_str + pos, "%d%n", &cur_مقدار, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_مقدار);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#endif

منطقی cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#وگرنه
    بازگشت cudaArch.builtWith(feature_set);
#endif
}

منطقی cv::cuda::TargetArchs::hasPtx(صحیح major, صحیح minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#وگرنه
    بازگشت cudaArch.hasPtx(major, minor);
#endif
}

منطقی cv::cuda::TargetArchs::hasBin(صحیح major, صحیح minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#وگرنه
    بازگشت cudaArch.hasBin(major, minor);
#endif
}

منطقی cv::cuda::TargetArchs::hasEqualOrLessPtx(صحیح major, صحیح minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#وگرنه
    بازگشت cudaArch.hasEqualOrLessPtx(major, minor);
#endif
}

منطقی cv::cuda::TargetArchs::hasEqualOrGreaterPtx(صحیح major, صحیح minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#وگرنه
    بازگشت cudaArch.hasEqualOrGreaterPtx(major, minor);
#endif
}

منطقی cv::cuda::TargetArchs::hasEqualOrGreaterBin(صحیح major, صحیح minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#وگرنه
    بازگشت cudaArch.hasEqualOrGreaterBin(major, minor);
#endif
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ifdef HAVE_CUDA

فضاینام
{
    رده DeviceProps
    {
    عمومی:
        DeviceProps();

        ثابت cudaDeviceProp* get(صحیح devID) ثابت;

    خصوصی:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        صحیح شمار_2 = get_Cuda_Enabled_Device_شمار();

        اگر (شمار_2 > 0)
        {
            props_.resize(شمار_2);

            برای (صحیح devID = 0; devID < شمار_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ثابت cudaDeviceProp* DeviceProps::get(صحیح devID) ثابت
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        بازگشت &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        ایستا DeviceProps props;
        بازگشت props;
    }
}

#endif

ثابت نویسه* cv::cuda::DeviceInfo::name() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->name;
#endif
}

size_t cv::cuda::DeviceInfo::totalGlobalMem() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->totalGlobalMem;
#endif
}

size_t cv::cuda::DeviceInfo::sharedMemPerBlock() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->sharedMemPerBlock;
#endif
}

صحیح cv::cuda::DeviceInfo::regsPerBlock() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->regsPerBlock;
#endif
}

صحیح cv::cuda::DeviceInfo::warpSize() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->warpSize;
#endif
}

size_t cv::cuda::DeviceInfo::memPitch() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->memPitch;
#endif
}

صحیح cv::cuda::DeviceInfo::maxThreadsPerBlock() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->maxThreadsPerBlock;
#endif
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec3i(deviceProps().get(device_id_)->maxThreadsDim);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxGridSize() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec3i(deviceProps().get(device_id_)->maxGridSize);
#endif
}

صحیح cv::cuda::DeviceInfo::clockRate() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    صحیح clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    بازگشت clockRate;
#endif
}

size_t cv::cuda::DeviceInfo::totalConstMem() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->totalConstMem;
#endif
}

صحیح cv::cuda::DeviceInfo::majorVersion() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->major;
#endif
}

صحیح cv::cuda::DeviceInfo::minorVersion() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->minor;
#endif
}

size_t cv::cuda::DeviceInfo::textureAlignment() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->textureAlignment;
#endif
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->texturePitchAlignment;
#endif
}

صحیح cv::cuda::DeviceInfo::multi_Processor_شمار() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->multi_Processor_شمار;
#endif
}

منطقی cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    صحیح kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    بازگشت kernelExecTimeoutEnabled != 0;
#endif
}

منطقی cv::cuda::DeviceInfo::integrated() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->integrated != 0;
#endif
}

منطقی cv::cuda::DeviceInfo::canMapHostMemory() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->canMapHostMemory != 0;
#endif
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    ایستا ثابت ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    صحیح computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    بازگشت tbl[computeMode];
#endif
}

صحیح cv::cuda::DeviceInfo::maxTexture1D() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->maxTexture1D;
#endif
}

صحیح cv::cuda::DeviceInfo::maxTexture1DMipmap() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    #اگر CUDA_VERSION >= 5000
        بازگشت deviceProps().get(device_id_)->maxTexture1DMipmap;
    #وگرنه
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        بازگشت 0;
    #endif
#endif
}

صحیح cv::cuda::DeviceInfo::maxTexture1DLinear() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    #اگر CUDA_VERSION >= 13000
        size_t maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        بازگشت maxWidthInElements;
    #وگرنه
        بازگشت deviceProps().get(device_id_)->maxTexture1DLinear;
    #endif
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    #اگر CUDA_VERSION >= 5000
        بازگشت Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #وگرنه
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        بازگشت Vec2i();
    #endif
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#endif
}

صحیح cv::cuda::DeviceInfo::maxTextureCubemap() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->maxTextureCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#endif
}

صحیح cv::cuda::DeviceInfo::maxSurface1D() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->maxSurface1D;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#endif
}

صحیح cv::cuda::DeviceInfo::maxSurfaceCubemap() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->maxSurfaceCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#endif
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->surfaceAlignment;
#endif
}

منطقی cv::cuda::DeviceInfo::concurrentKernels() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->concurrentKernels != 0;
#endif
}

منطقی cv::cuda::DeviceInfo::ECCEnabled() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->ECCEnabled != 0;
#endif
}

صحیح cv::cuda::DeviceInfo::pciBusID() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->pciBusID;
#endif
}

صحیح cv::cuda::DeviceInfo::pciDeviceID() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->pciDeviceID;
#endif
}

صحیح cv::cuda::DeviceInfo::pciDomainID() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->pciDomainID;
#endif
}

منطقی cv::cuda::DeviceInfo::tccDriver() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->tccDriver != 0;
#endif
}

صحیح cv::cuda::DeviceInfo::async_Engine_شمار() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->async_Engine_شمار;
#endif
}

منطقی cv::cuda::DeviceInfo::unifiedAddressing() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->unifiedAddressing != 0;
#endif
}

صحیح cv::cuda::DeviceInfo::memoryClockRate() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    صحیح memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    بازگشت memoryClockRate;
#endif
}

صحیح cv::cuda::DeviceInfo::memoryBusWidth() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->memoryBusWidth;
#endif
}

صحیح cv::cuda::DeviceInfo::l2CacheSize() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->l2CacheSize;
#endif
}

صحیح cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    بازگشت deviceProps().get(device_id_)->maxThreadsPerMultiProcessor;
#endif
}

تهی cv::cuda::DeviceInfo::queryMemory(size_t& _totalMemory, size_t& _freeMemory) ثابت
{
#ifndef HAVE_CUDA
    CV_UNUSED(_totalMemory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#وگرنه
    صحیح prevDeviceID = getDevice();
    اگر (prevDeviceID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &_totalMemory) );

    اگر (prevDeviceID != device_id_)
        setDevice(prevDeviceID);
#endif
}

منطقی cv::cuda::DeviceInfo::isCompatible() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    // Check PTX compatibility
    اگر (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        بازگشت درست;

    // Check BIN compatibility
    برای (صحیح i = minorVersion(); i >= 0; --i)
        اگر (TargetArchs::hasBin(majorVersion(), i))
            بازگشت درست;

    بازگشت نادرست;
#endif
}

////////////////////////////////////////////////////////////////////////
// print info

#ifdef HAVE_CUDA

فضاینام
{
    صحیح convertSMVer2Cores(صحیح major, صحیح minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        تعریفنوع ساختار {
            صحیح SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            صحیح Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        صحیح index = 0;
        وقتی (gpuArchCoresPerSM[index].SM != -1)
        {
            اگر (gpuArchCoresPerSM[index].SM == ((major << 4) + minor) )
                بازگشت gpuArchCoresPerSM[index].Cores;
            index++;
        }

        بازگشت -1;
    }
}

#endif

تهی cv::cuda::printCudaDeviceInfo(صحیح device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#وگرنه
    صحیح شمار_2 = get_Cuda_Enabled_Device_شمار();
    منطقی valid = (device >= 0) && (device < شمار_2);

    صحیح beg = valid ? device   : 0;
    صحیح end = valid ? device+1 : شمار_2;

    printf("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    printf("Device count: %d\n", شمار_2);

    صحیح driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    ثابت نویسه *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        پوچ
    };

    برای(صحیح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        printf("\nDevice %d: \"%s\"\n", dev, prop.name);
        printf("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        printf("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        printf("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (اعشاری)prop.totalGlobalMem/1048576.0f, (unsigned long long) prop.totalGlobalMem);

        صحیح cores = convertSMVer2Cores(prop.major, prop.minor);
        اگر (cores > 0)
            printf("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_شمار, cores, cores * prop.multi_Processor_شمار);

        صحیح clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        printf("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        printf("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        printf("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        printf("  Total amount of constant memory:               %u bytes\n", (صحیح)prop.totalConstMem);
        printf("  Total amount of shared memory per block:       %u bytes\n", (صحیح)prop.sharedMemPerBlock);
        printf("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        printf("  Warp size:                                     %d\n", prop.warpSize);
        printf("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        printf("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.maxGridSize[0], prop.maxGridSize[1],  prop.maxGridSize[2]);
        printf("  Maximum memory pitch:                          %u bytes\n", (صحیح)prop.memPitch);
        printf("  Texture alignment:                             %u bytes\n", (صحیح)prop.textureAlignment);

        printf("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_شمار ? "Yes" : "No"), prop.async_Engine_شمار);
        صحیح kernelExecTimeoutEnabled;
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

        صحیح propComputeMode;
        cudaSafeCall(cudaDeviceGetAttribute(&propComputeMode, cudaDevAttrComputeMode, dev));
        printf("  Compute Mode:\n");
        printf("      %s \n", computeMode[propComputeMode]);
    }

    printf("\n");
    printf("deviceQuery, CUDA Driver = CUDART");
    printf(", CUDA Driver Version  = %d.%d", driverVersion / 1000, driverVersion % 100);
    printf(", CUDA Runtime Version = %d.%d", runtimeVersion/1000, runtimeVersion%100);
    printf(", NumDevs = %d\n\n", شمار_2);

    fflush(stdout);
#endif
}

تهی cv::cuda::printShortCudaDeviceInfo(صحیح device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#وگرنه
    صحیح شمار_2 = get_Cuda_Enabled_Device_شمار();
    منطقی valid = (device >= 0) && (device < شمار_2);

    صحیح beg = valid ? device   : 0;
    صحیح end = valid ? device+1 : شمار_2;

    صحیح driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    برای(صحیح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        ثابت نویسه *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        printf("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (اعشاری)prop.totalGlobalMem/1048576.0f);
        printf(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        صحیح cores = convertSMVer2Cores(prop.major, prop.minor);
        اگر (cores > 0)
            printf(", %d cores", cores * prop.multi_Processor_شمار);

        printf(", Driver/Runtime ver.%d.%d/%d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
    }

    fflush(stdout);
#endif
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ifdef HAVE_CUDA

فضاینام
{
    #define error_entry(entry)  { entry, #entry }

    ساختار ErrorEntry
    {
        صحیح code;
        ثابت نویسه* str;
    };

    ساختار ErrorEntryComparer
    {
        صحیح code;
        ErrorEntryComparer(صحیح code_) : code(code_) {}
        منطقی operator()(ثابت ErrorEntry& e) ثابت { بازگشت e.code == code; }
    };

    ثابت ErrorEntry npp_errors [] =
    {
    #اگر defined (_MSC_VER)
        error_entry( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #endif

    #اگر NPP_VERSION < 5500
        error_entry( NPP_BAD_ARG_ERROR ),
        error_entry( NPP_COEFF_ERROR ),
        error_entry( NPP_RECT_ERROR ),
        error_entry( NPP_QUAD_ERROR ),
        error_entry( NPP_MEMFREE_ERR ),
        error_entry( NPP_MEMSET_ERR ),
        error_entry( NPP_MEM_ALLOC_ERR ),
        error_entry( NPP_HISTO_عدد_OF_LEVELS_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERR ),
        error_entry( NPP_INVALID_درون_داد ),
        error_entry( NPP_POINTER_ERROR ),
        error_entry( NPP_WARNING ),
        error_entry( NPP_ODD_ROI_WARNING ),
    #وگرنه
        error_entry( NPP_INVALID_HOST_POINTER_ERROR ),
        error_entry( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_entry( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_entry( NPP_ZC_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_MEMFREE_ERROR ),
        error_entry( NPP_MEMSET_ERROR ),
        error_entry( NPP_QUALITY_INDEX_ERROR ),
        error_entry( NPP_HISTOGRAM_عدد_OF_LEVELS_ERROR ),
        error_entry( NPP_CHANNEL_ORDER_ERROR ),
        error_entry( NPP_ZERO_MASK_مقدار_ERROR ),
        error_entry( NPP_QUADRANGLE_ERROR ),
        error_entry( NPP_RECTANGLE_ERROR ),
        error_entry( NPP_COEFFICIENT_ERROR ),
        error_entry( NPP_عدد_OF_CHANNELS_ERROR ),
        error_entry( NPP_COI_ERROR ),
        error_entry( NPP_مقسوم_علیه_ERROR ),
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
        error_entry( NPP_LUT_عدد_OF_LEVELS_ERROR ),
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

    ثابت size_t npp_error_رقم = اندازه(npp_errors) / اندازه(npp_errors[0]);

    ثابت ErrorEntry cu_errors [] =
    {
        error_entry( CUDA_SUCCESS                              ),
        error_entry( CUDA_ERROR_INVALID_مقدار                  ),
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
        error_entry( CUDA_ERROR_NO_دودویی_FOR_GPU              ),
        error_entry( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_entry( CUDA_ERROR_NOT_MAPPED                     ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_entry( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_entry( CUDA_ERROR_UNSUPPORTED_LIMIT              ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_IN_USE         ),
        error_entry( CUDA_ERROR_INVALID_SOURCE                 ),
        error_entry( CUDA_ERROR_FILE_NOT_FOUND                 ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_نماد_NOT_FOUND ),
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

    ثابت size_t cu_errors_رقم = اندازه(cu_errors) / اندازه(cu_errors[0]);

    cv::String getErrorString(صحیح code, ثابت ErrorEntry* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        ثابت نویسه* پیام = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", پیام, code);

        بازگشت str;
    }
}

#endif

String cv::cuda::getNppErrorMessage(صحیح code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    بازگشت String();
#وگرنه
    بازگشت getErrorString(code, npp_errors, npp_error_رقم);
#endif
}

String cv::cuda::getCudaDriverApiErrorMessage(صحیح code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    بازگشت String();
#وگرنه
    بازگشت getErrorString(code, cu_errors, cu_errors_رقم);
#endif
}
