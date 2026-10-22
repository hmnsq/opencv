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

#അന്തര്ഭാവ "precomp.hpp"

using നാമാകാശ cv;
using നാമാകാശ cv::cuda;

പൂർണ്ണസംഖ്യ cv::cuda::get_Cuda_Enabled_Device_എണ്ണം()
{
#ifndef HAVE_CUDA
    തിരികെ 0;
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ എണ്ണം_2;
    cudaError_t error = cuda_Get_Device_എണ്ണം(&എണ്ണം_2);

    എങ്കിൽ (error == cudaErrorInsufficientDriver)
        തിരികെ -1;

    എങ്കിൽ (error == cudaErrorNoDevice)
        തിരികെ 0;

    cudaSafeCall( error );
    തിരികെ എണ്ണം_2;
#endif
}

ശൂന്യം cv::cuda::setDevice(പൂർണ്ണസംഖ്യ device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::getDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ device;
    cudaSafeCall( cudaGetDevice(&device) );
    തിരികെ device;
#endif
}

ശൂന്യം cv::cuda::resetDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    cudaSafeCall( cudaDeviceReset() );
#endif
}

തർക്കം cv::cuda::deviceSupports(FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    സ്ഥിതം പൂർണ്ണസംഖ്യ versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ cache_size = static_cast<പൂർണ്ണസംഖ്യ>(വലുപ്പം(versions) / വലുപ്പം(versions[0]));

    സ്ഥിരം പൂർണ്ണസംഖ്യ devId = getDevice();

    പൂർണ്ണസംഖ്യ version;

    എങ്കിൽ (devId < cache_size && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    അല്ലെങ്കിൽ
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        എങ്കിൽ (devId < cache_size)
            versions[devId] = version;
    }

    തിരികെ TargetArchs::builtWith(feature_set) && (version >= feature_set);
#endif
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ifdef HAVE_CUDA

നാമാകാശ
{
    വർഗ്ഗം CudaArch
    {
    പൊതു:
        CudaArch();

        തർക്കം builtWith(FeatureSet feature_set) സ്ഥിരം;
        തർക്കം hasPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം;
        തർക്കം hasBin(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം;
        തർക്കം hasEqualOrLessPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം;
        തർക്കം hasEqualOrGreaterPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം;
        തർക്കം hasEqualOrGreaterBin(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം;

    സ്വകാര്യ:
        സ്ഥിതം ശൂന്യം fromStr(സ്ഥിരം അക്ഷരം* set_as_str, std::vector<പൂർണ്ണസംഖ്യ>& arr);

        std::vector<പൂർണ്ണസംഖ്യ> bin;
        std::vector<പൂർണ്ണസംഖ്യ> ptx;
        std::vector<പൂർണ്ണസംഖ്യ> features;
    };

    സ്ഥിരം CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    തർക്കം CudaArch::builtWith(FeatureSet feature_set) സ്ഥിരം
    {
        തിരികെ !features.empty() && (features.back() >= feature_set);
    }

    തർക്കം CudaArch::hasPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം
    {
        തിരികെ std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    തർക്കം CudaArch::hasBin(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം
    {
        തിരികെ std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    തർക്കം CudaArch::hasEqualOrLessPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം
    {
        തിരികെ !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    തർക്കം CudaArch::hasEqualOrGreaterPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം
    {
        തിരികെ !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    തർക്കം CudaArch::hasEqualOrGreaterBin(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor) സ്ഥിരം
    {
        തിരികെ !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    ശൂന്യം CudaArch::fromStr(സ്ഥിരം അക്ഷരം* set_as_str, std::vector<പൂർണ്ണസംഖ്യ>& arr)
    {
        arr.clear();

        സ്ഥിരം size_t len = strlen(set_as_str);

        size_t pos = 0;
        ആകുമ്പോൾ (pos < len)
        {
            എങ്കിൽ (isspace(set_as_str[pos]))
            {
                ++pos;
            }
            അല്ലെങ്കിൽ
            {
                പൂർണ്ണസംഖ്യ cur_മൂല്യം;
                പൂർണ്ണസംഖ്യ chars_read;
                പൂർണ്ണസംഖ്യ args_read = sscanf(set_as_str + pos, "%d%n", &cur_മൂല്യം, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_മൂല്യം);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#endif

തർക്കം cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ cudaArch.builtWith(feature_set);
#endif
}

തർക്കം cv::cuda::TargetArchs::hasPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ cudaArch.hasPtx(major, minor);
#endif
}

തർക്കം cv::cuda::TargetArchs::hasBin(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ cudaArch.hasBin(major, minor);
#endif
}

തർക്കം cv::cuda::TargetArchs::hasEqualOrLessPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ cudaArch.hasEqualOrLessPtx(major, minor);
#endif
}

തർക്കം cv::cuda::TargetArchs::hasEqualOrGreaterPtx(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ cudaArch.hasEqualOrGreaterPtx(major, minor);
#endif
}

തർക്കം cv::cuda::TargetArchs::hasEqualOrGreaterBin(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ cudaArch.hasEqualOrGreaterBin(major, minor);
#endif
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ifdef HAVE_CUDA

നാമാകാശ
{
    വർഗ്ഗം DeviceProps
    {
    പൊതു:
        DeviceProps();

        സ്ഥിരം cudaDeviceProp* get(പൂർണ്ണസംഖ്യ devID) സ്ഥിരം;

    സ്വകാര്യ:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        പൂർണ്ണസംഖ്യ എണ്ണം_2 = get_Cuda_Enabled_Device_എണ്ണം();

        എങ്കിൽ (എണ്ണം_2 > 0)
        {
            props_.resize(എണ്ണം_2);

            വേണ്ടി (പൂർണ്ണസംഖ്യ devID = 0; devID < എണ്ണം_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    സ്ഥിരം cudaDeviceProp* DeviceProps::get(പൂർണ്ണസംഖ്യ devID) സ്ഥിരം
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        തിരികെ &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        സ്ഥിതം DeviceProps props;
        തിരികെ props;
    }
}

#endif

സ്ഥിരം അക്ഷരം* cv::cuda::DeviceInfo::name() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->name;
#endif
}

size_t cv::cuda::DeviceInfo::totalGlobalMem() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->totalGlobalMem;
#endif
}

size_t cv::cuda::DeviceInfo::sharedMemPerBlock() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->sharedMemPerBlock;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::regsPerBlock() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->regsPerBlock;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::warpSize() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->warpSize;
#endif
}

size_t cv::cuda::DeviceInfo::memPitch() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->memPitch;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::maxThreadsPerBlock() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->maxThreadsPerBlock;
#endif
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec3i(deviceProps().get(device_id_)->maxThreadsDim);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxGridSize() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec3i(deviceProps().get(device_id_)->maxGridSize);
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::clockRate() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    തിരികെ clockRate;
#endif
}

size_t cv::cuda::DeviceInfo::totalConstMem() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->totalConstMem;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::majorVersion() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->major;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::minorVersion() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->minor;
#endif
}

size_t cv::cuda::DeviceInfo::textureAlignment() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->textureAlignment;
#endif
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->texturePitchAlignment;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::multi_Processor_എണ്ണം() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->multi_Processor_എണ്ണം;
#endif
}

തർക്കം cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    തിരികെ kernelExecTimeoutEnabled != 0;
#endif
}

തർക്കം cv::cuda::DeviceInfo::integrated() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->integrated != 0;
#endif
}

തർക്കം cv::cuda::DeviceInfo::canMapHostMemory() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->canMapHostMemory != 0;
#endif
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    സ്ഥിതം സ്ഥിരം ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    പൂർണ്ണസംഖ്യ computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    തിരികെ tbl[computeMode];
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::maxTexture1D() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->maxTexture1D;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::maxTexture1DMipmap() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    #എങ്കിൽ CUDA_VERSION >= 5000
        തിരികെ deviceProps().get(device_id_)->maxTexture1DMipmap;
    #അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        തിരികെ 0;
    #endif
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::maxTexture1DLinear() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    #എങ്കിൽ CUDA_VERSION >= 13000
        size_t maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        തിരികെ maxWidthInElements;
    #അല്ലെങ്കിൽ
        തിരികെ deviceProps().get(device_id_)->maxTexture1DLinear;
    #endif
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    #എങ്കിൽ CUDA_VERSION >= 5000
        തിരികെ Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #അല്ലെങ്കിൽ
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        തിരികെ Vec2i();
    #endif
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::maxTextureCubemap() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->maxTextureCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::maxSurface1D() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->maxSurface1D;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::maxSurfaceCubemap() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->maxSurfaceCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#endif
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->surfaceAlignment;
#endif
}

തർക്കം cv::cuda::DeviceInfo::concurrentKernels() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->concurrentKernels != 0;
#endif
}

തർക്കം cv::cuda::DeviceInfo::ECCEnabled() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->ECCEnabled != 0;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::pciBusID() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->pciBusID;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::pciDeviceID() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->pciDeviceID;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::pciDomainID() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->pciDomainID;
#endif
}

തർക്കം cv::cuda::DeviceInfo::tccDriver() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->tccDriver != 0;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::async_Engine_എണ്ണം() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->async_Engine_എണ്ണം;
#endif
}

തർക്കം cv::cuda::DeviceInfo::unifiedAddressing() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->unifiedAddressing != 0;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::memoryClockRate() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    തിരികെ memoryClockRate;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::memoryBusWidth() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->memoryBusWidth;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::l2CacheSize() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->l2CacheSize;
#endif
}

പൂർണ്ണസംഖ്യ cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ deviceProps().get(device_id_)->maxThreadsPerMultiProcessor;
#endif
}

ശൂന്യം cv::cuda::DeviceInfo::queryMemory(size_t& _totalMemory, size_t& _freeMemory) സ്ഥിരം
{
#ifndef HAVE_CUDA
    CV_UNUSED(_totalMemory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ prevDeviceID = getDevice();
    എങ്കിൽ (prevDeviceID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &_totalMemory) );

    എങ്കിൽ (prevDeviceID != device_id_)
        setDevice(prevDeviceID);
#endif
}

തർക്കം cv::cuda::DeviceInfo::isCompatible() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    // Check PTX compatibility
    എങ്കിൽ (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        തിരികെ സത്യം;

    // Check BIN compatibility
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = minorVersion(); i >= 0; --i)
        എങ്കിൽ (TargetArchs::hasBin(majorVersion(), i))
            തിരികെ സത്യം;

    തിരികെ അസത്യം;
#endif
}

////////////////////////////////////////////////////////////////////////
// print info

#ifdef HAVE_CUDA

നാമാകാശ
{
    പൂർണ്ണസംഖ്യ convertSMVer2Cores(പൂർണ്ണസംഖ്യ major, പൂർണ്ണസംഖ്യ minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        തരനിർവചനം ഘടന {
            പൂർണ്ണസംഖ്യ SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            പൂർണ്ണസംഖ്യ Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        പൂർണ്ണസംഖ്യ index = 0;
        ആകുമ്പോൾ (gpuArchCoresPerSM[index].SM != -1)
        {
            എങ്കിൽ (gpuArchCoresPerSM[index].SM == ((major << 4) + minor) )
                തിരികെ gpuArchCoresPerSM[index].Cores;
            index++;
        }

        തിരികെ -1;
    }
}

#endif

ശൂന്യം cv::cuda::printCudaDeviceInfo(പൂർണ്ണസംഖ്യ device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ എണ്ണം_2 = get_Cuda_Enabled_Device_എണ്ണം();
    തർക്കം valid = (device >= 0) && (device < എണ്ണം_2);

    പൂർണ്ണസംഖ്യ beg = valid ? device   : 0;
    പൂർണ്ണസംഖ്യ end = valid ? device+1 : എണ്ണം_2;

    printf("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    printf("Device count: %d\n", എണ്ണം_2);

    പൂർണ്ണസംഖ്യ driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    സ്ഥിരം അക്ഷരം *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        ശൂന്യ
    };

    വേണ്ടി(പൂർണ്ണസംഖ്യ dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        printf("\nDevice %d: \"%s\"\n", dev, prop.name);
        printf("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        printf("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        printf("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (ദശാംശം)prop.totalGlobalMem/1048576.0f, (unsigned long long) prop.totalGlobalMem);

        പൂർണ്ണസംഖ്യ cores = convertSMVer2Cores(prop.major, prop.minor);
        എങ്കിൽ (cores > 0)
            printf("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_എണ്ണം, cores, cores * prop.multi_Processor_എണ്ണം);

        പൂർണ്ണസംഖ്യ clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        printf("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        printf("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        printf("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        printf("  Total amount of constant memory:               %u bytes\n", (പൂർണ്ണസംഖ്യ)prop.totalConstMem);
        printf("  Total amount of shared memory per block:       %u bytes\n", (പൂർണ്ണസംഖ്യ)prop.sharedMemPerBlock);
        printf("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        printf("  Warp size:                                     %d\n", prop.warpSize);
        printf("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        printf("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.maxGridSize[0], prop.maxGridSize[1],  prop.maxGridSize[2]);
        printf("  Maximum memory pitch:                          %u bytes\n", (പൂർണ്ണസംഖ്യ)prop.memPitch);
        printf("  Texture alignment:                             %u bytes\n", (പൂർണ്ണസംഖ്യ)prop.textureAlignment);

        printf("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_എണ്ണം ? "Yes" : "No"), prop.async_Engine_എണ്ണം);
        പൂർണ്ണസംഖ്യ kernelExecTimeoutEnabled;
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

        പൂർണ്ണസംഖ്യ propComputeMode;
        cudaSafeCall(cudaDeviceGetAttribute(&propComputeMode, cudaDevAttrComputeMode, dev));
        printf("  Compute Mode:\n");
        printf("      %s \n", computeMode[propComputeMode]);
    }

    printf("\n");
    printf("deviceQuery, CUDA Driver = CUDART");
    printf(", CUDA Driver Version  = %d.%d", driverVersion / 1000, driverVersion % 100);
    printf(", CUDA Runtime Version = %d.%d", runtimeVersion/1000, runtimeVersion%100);
    printf(", NumDevs = %d\n\n", എണ്ണം_2);

    fflush(stdout);
#endif
}

ശൂന്യം cv::cuda::printShortCudaDeviceInfo(പൂർണ്ണസംഖ്യ device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ എണ്ണം_2 = get_Cuda_Enabled_Device_എണ്ണം();
    തർക്കം valid = (device >= 0) && (device < എണ്ണം_2);

    പൂർണ്ണസംഖ്യ beg = valid ? device   : 0;
    പൂർണ്ണസംഖ്യ end = valid ? device+1 : എണ്ണം_2;

    പൂർണ്ണസംഖ്യ driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    വേണ്ടി(പൂർണ്ണസംഖ്യ dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        സ്ഥിരം അക്ഷരം *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        printf("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (ദശാംശം)prop.totalGlobalMem/1048576.0f);
        printf(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        പൂർണ്ണസംഖ്യ cores = convertSMVer2Cores(prop.major, prop.minor);
        എങ്കിൽ (cores > 0)
            printf(", %d cores", cores * prop.multi_Processor_എണ്ണം);

        printf(", Driver/Runtime ver.%d.%d/%d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
    }

    fflush(stdout);
#endif
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ifdef HAVE_CUDA

നാമാകാശ
{
    #define error_entry(entry)  { entry, #entry }

    ഘടന ErrorEntry
    {
        പൂർണ്ണസംഖ്യ code;
        സ്ഥിരം അക്ഷരം* str;
    };

    ഘടന ErrorEntryComparer
    {
        പൂർണ്ണസംഖ്യ code;
        ErrorEntryComparer(പൂർണ്ണസംഖ്യ code_) : code(code_) {}
        തർക്കം operator()(സ്ഥിരം ErrorEntry& e) സ്ഥിരം { തിരികെ e.code == code; }
    };

    സ്ഥിരം ErrorEntry npp_errors [] =
    {
    #എങ്കിൽ defined (_MSC_VER)
        error_entry( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #endif

    #എങ്കിൽ NPP_VERSION < 5500
        error_entry( NPP_BAD_ARG_ERROR ),
        error_entry( NPP_COEFF_ERROR ),
        error_entry( NPP_RECT_ERROR ),
        error_entry( NPP_QUAD_ERROR ),
        error_entry( NPP_MEMFREE_ERR ),
        error_entry( NPP_MEMSET_ERR ),
        error_entry( NPP_MEM_ALLOC_ERR ),
        error_entry( NPP_HISTO_സംഖ്യ_OF_LEVELS_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERR ),
        error_entry( NPP_INVALID_നിവേശം ),
        error_entry( NPP_POINTER_ERROR ),
        error_entry( NPP_WARNING ),
        error_entry( NPP_ODD_ROI_WARNING ),
    #അല്ലെങ്കിൽ
        error_entry( NPP_INVALID_HOST_POINTER_ERROR ),
        error_entry( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_entry( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_entry( NPP_ZC_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_MEMFREE_ERROR ),
        error_entry( NPP_MEMSET_ERROR ),
        error_entry( NPP_QUALITY_INDEX_ERROR ),
        error_entry( NPP_HISTOGRAM_സംഖ്യ_OF_LEVELS_ERROR ),
        error_entry( NPP_CHANNEL_ORDER_ERROR ),
        error_entry( NPP_ZERO_MASK_മൂല്യം_ERROR ),
        error_entry( NPP_QUADRANGLE_ERROR ),
        error_entry( NPP_RECTANGLE_ERROR ),
        error_entry( NPP_COEFFICIENT_ERROR ),
        error_entry( NPP_സംഖ്യ_OF_CHANNELS_ERROR ),
        error_entry( NPP_COI_ERROR ),
        error_entry( NPP_ഹാരകം_ERROR ),
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
        error_entry( NPP_LUT_സംഖ്യ_OF_LEVELS_ERROR ),
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

    സ്ഥിരം size_t npp_error_അക്കം = വലുപ്പം(npp_errors) / വലുപ്പം(npp_errors[0]);

    സ്ഥിരം ErrorEntry cu_errors [] =
    {
        error_entry( CUDA_SUCCESS                              ),
        error_entry( CUDA_ERROR_INVALID_മൂല്യം                  ),
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
        error_entry( CUDA_ERROR_NO_ദ്വിമാന_FOR_GPU              ),
        error_entry( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_entry( CUDA_ERROR_NOT_MAPPED                     ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_entry( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_entry( CUDA_ERROR_UNSUPPORTED_LIMIT              ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_IN_USE         ),
        error_entry( CUDA_ERROR_INVALID_SOURCE                 ),
        error_entry( CUDA_ERROR_FILE_NOT_FOUND                 ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_ചിഹ്നം_NOT_FOUND ),
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

    സ്ഥിരം size_t cu_errors_അക്കം = വലുപ്പം(cu_errors) / വലുപ്പം(cu_errors[0]);

    cv::String getErrorString(പൂർണ്ണസംഖ്യ code, സ്ഥിരം ErrorEntry* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        സ്ഥിരം അക്ഷരം* സന്ദേശം = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", സന്ദേശം, code);

        തിരികെ str;
    }
}

#endif

String cv::cuda::getNppErrorMessage(പൂർണ്ണസംഖ്യ code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    തിരികെ String();
#അല്ലെങ്കിൽ
    തിരികെ getErrorString(code, npp_errors, npp_error_അക്കം);
#endif
}

String cv::cuda::getCudaDriverApiErrorMessage(പൂർണ്ണസംഖ്യ code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    തിരികെ String();
#അല്ലെങ്കിൽ
    തിരികെ getErrorString(code, cu_errors, cu_errors_അക്കം);
#endif
}
