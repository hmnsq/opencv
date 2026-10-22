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

#অন্তর্ভাব "precomp.hpp"

using নামাকাশ cv;
using নামাকাশ cv::cuda;

পূর্ণাঙ্ক cv::cuda::get_Cuda_Enabled_Device_গুনতি()
{
#ifndef HAVE_CUDA
    প্রত্যাগম 0;
#অন্যথা
    পূর্ণাঙ্ক গুনতি_2;
    cudaError_t error = cuda_Get_Device_গুনতি(&গুনতি_2);

    যদি (error == cudaErrorInsufficientDriver)
        প্রত্যাগম -1;

    যদি (error == cudaErrorNoDevice)
        প্রত্যাগম 0;

    cudaSafeCall( error );
    প্রত্যাগম গুনতি_2;
#endif
}

শূন্য cv::cuda::setDevice(পূর্ণাঙ্ক device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#অন্যথা
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#endif
}

পূর্ণাঙ্ক cv::cuda::getDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    পূর্ণাঙ্ক device;
    cudaSafeCall( cudaGetDevice(&device) );
    প্রত্যাগম device;
#endif
}

শূন্য cv::cuda::resetDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    cudaSafeCall( cudaDeviceReset() );
#endif
}

তার্কিক cv::cuda::deviceSupports(FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#অন্যথা
    স্থির পূর্ণাঙ্ক versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    স্থির ধ্রুব পূর্ণাঙ্ক cache_size = static_cast<পূর্ণাঙ্ক>(পরিমাণ(versions) / পরিমাণ(versions[0]));

    ধ্রুব পূর্ণাঙ্ক devId = getDevice();

    পূর্ণাঙ্ক version;

    যদি (devId < cache_size && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    অন্যথা
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        যদি (devId < cache_size)
            versions[devId] = version;
    }

    প্রত্যাগম TargetArchs::builtWith(feature_set) && (version >= feature_set);
#endif
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ifdef HAVE_CUDA

নামাকাশ
{
    বর্গ CudaArch
    {
    সার্বজনিক:
        CudaArch();

        তার্কিক builtWith(FeatureSet feature_set) ধ্রুব;
        তার্কিক hasPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব;
        তার্কিক hasBin(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব;
        তার্কিক hasEqualOrLessPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব;
        তার্কিক hasEqualOrGreaterPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব;
        তার্কিক hasEqualOrGreaterBin(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব;

    গুপ্ত:
        স্থির শূন্য fromStr(ধ্রুব বর্ণ* set_as_str, std::vector<পূর্ণাঙ্ক>& arr);

        std::vector<পূর্ণাঙ্ক> bin;
        std::vector<পূর্ণাঙ্ক> ptx;
        std::vector<পূর্ণাঙ্ক> features;
    };

    ধ্রুব CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    তার্কিক CudaArch::builtWith(FeatureSet feature_set) ধ্রুব
    {
        প্রত্যাগম !features.empty() && (features.back() >= feature_set);
    }

    তার্কিক CudaArch::hasPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব
    {
        প্রত্যাগম std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    তার্কিক CudaArch::hasBin(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব
    {
        প্রত্যাগম std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    তার্কিক CudaArch::hasEqualOrLessPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব
    {
        প্রত্যাগম !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    তার্কিক CudaArch::hasEqualOrGreaterPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব
    {
        প্রত্যাগম !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    তার্কিক CudaArch::hasEqualOrGreaterBin(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor) ধ্রুব
    {
        প্রত্যাগম !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    শূন্য CudaArch::fromStr(ধ্রুব বর্ণ* set_as_str, std::vector<পূর্ণাঙ্ক>& arr)
    {
        arr.clear();

        ধ্রুব size_t len = strlen(set_as_str);

        size_t pos = 0;
        যাবত্ (pos < len)
        {
            যদি (isspace(set_as_str[pos]))
            {
                ++pos;
            }
            অন্যথা
            {
                পূর্ণাঙ্ক cur_মান;
                পূর্ণাঙ্ক chars_read;
                পূর্ণাঙ্ক args_read = sscanf(set_as_str + pos, "%d%n", &cur_মান, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_মান);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#endif

তার্কিক cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম cudaArch.builtWith(feature_set);
#endif
}

তার্কিক cv::cuda::TargetArchs::hasPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম cudaArch.hasPtx(major, minor);
#endif
}

তার্কিক cv::cuda::TargetArchs::hasBin(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম cudaArch.hasBin(major, minor);
#endif
}

তার্কিক cv::cuda::TargetArchs::hasEqualOrLessPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম cudaArch.hasEqualOrLessPtx(major, minor);
#endif
}

তার্কিক cv::cuda::TargetArchs::hasEqualOrGreaterPtx(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম cudaArch.hasEqualOrGreaterPtx(major, minor);
#endif
}

তার্কিক cv::cuda::TargetArchs::hasEqualOrGreaterBin(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম cudaArch.hasEqualOrGreaterBin(major, minor);
#endif
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ifdef HAVE_CUDA

নামাকাশ
{
    বর্গ DeviceProps
    {
    সার্বজনিক:
        DeviceProps();

        ধ্রুব cudaDeviceProp* get(পূর্ণাঙ্ক devID) ধ্রুব;

    গুপ্ত:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        পূর্ণাঙ্ক গুনতি_2 = get_Cuda_Enabled_Device_গুনতি();

        যদি (গুনতি_2 > 0)
        {
            props_.resize(গুনতি_2);

            প্রতি (পূর্ণাঙ্ক devID = 0; devID < গুনতি_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ধ্রুব cudaDeviceProp* DeviceProps::get(পূর্ণাঙ্ক devID) ধ্রুব
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        প্রত্যাগম &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        স্থির DeviceProps props;
        প্রত্যাগম props;
    }
}

#endif

ধ্রুব বর্ণ* cv::cuda::DeviceInfo::name() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->name;
#endif
}

size_t cv::cuda::DeviceInfo::totalGlobalMem() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->totalGlobalMem;
#endif
}

size_t cv::cuda::DeviceInfo::sharedMemPerBlock() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->sharedMemPerBlock;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::regsPerBlock() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->regsPerBlock;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::warpSize() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->warpSize;
#endif
}

size_t cv::cuda::DeviceInfo::memPitch() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->memPitch;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::maxThreadsPerBlock() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->maxThreadsPerBlock;
#endif
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec3i(deviceProps().get(device_id_)->maxThreadsDim);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxGridSize() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec3i(deviceProps().get(device_id_)->maxGridSize);
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::clockRate() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    পূর্ণাঙ্ক clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    প্রত্যাগম clockRate;
#endif
}

size_t cv::cuda::DeviceInfo::totalConstMem() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->totalConstMem;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::majorVersion() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->major;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::minorVersion() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->minor;
#endif
}

size_t cv::cuda::DeviceInfo::textureAlignment() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->textureAlignment;
#endif
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->texturePitchAlignment;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::multi_Processor_গুনতি() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->multi_Processor_গুনতি;
#endif
}

তার্কিক cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    পূর্ণাঙ্ক kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    প্রত্যাগম kernelExecTimeoutEnabled != 0;
#endif
}

তার্কিক cv::cuda::DeviceInfo::integrated() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->integrated != 0;
#endif
}

তার্কিক cv::cuda::DeviceInfo::canMapHostMemory() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->canMapHostMemory != 0;
#endif
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    স্থির ধ্রুব ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    পূর্ণাঙ্ক computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    প্রত্যাগম tbl[computeMode];
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::maxTexture1D() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->maxTexture1D;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::maxTexture1DMipmap() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    #যদি CUDA_VERSION >= 5000
        প্রত্যাগম deviceProps().get(device_id_)->maxTexture1DMipmap;
    #অন্যথা
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        প্রত্যাগম 0;
    #endif
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::maxTexture1DLinear() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    #যদি CUDA_VERSION >= 13000
        size_t maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        প্রত্যাগম maxWidthInElements;
    #অন্যথা
        প্রত্যাগম deviceProps().get(device_id_)->maxTexture1DLinear;
    #endif
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    #যদি CUDA_VERSION >= 5000
        প্রত্যাগম Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #অন্যথা
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        প্রত্যাগম Vec2i();
    #endif
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::maxTextureCubemap() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->maxTextureCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::maxSurface1D() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->maxSurface1D;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::maxSurfaceCubemap() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->maxSurfaceCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#endif
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->surfaceAlignment;
#endif
}

তার্কিক cv::cuda::DeviceInfo::concurrentKernels() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->concurrentKernels != 0;
#endif
}

তার্কিক cv::cuda::DeviceInfo::ECCEnabled() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->ECCEnabled != 0;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::pciBusID() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->pciBusID;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::pciDeviceID() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->pciDeviceID;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::pciDomainID() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->pciDomainID;
#endif
}

তার্কিক cv::cuda::DeviceInfo::tccDriver() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->tccDriver != 0;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::async_Engine_গুনতি() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->async_Engine_গুনতি;
#endif
}

তার্কিক cv::cuda::DeviceInfo::unifiedAddressing() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->unifiedAddressing != 0;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::memoryClockRate() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    পূর্ণাঙ্ক memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    প্রত্যাগম memoryClockRate;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::memoryBusWidth() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->memoryBusWidth;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::l2CacheSize() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->l2CacheSize;
#endif
}

পূর্ণাঙ্ক cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম deviceProps().get(device_id_)->maxThreadsPerMultiProcessor;
#endif
}

শূন্য cv::cuda::DeviceInfo::queryMemory(size_t& _totalMemory, size_t& _freeMemory) ধ্রুব
{
#ifndef HAVE_CUDA
    CV_UNUSED(_totalMemory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#অন্যথা
    পূর্ণাঙ্ক prevDeviceID = getDevice();
    যদি (prevDeviceID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &_totalMemory) );

    যদি (prevDeviceID != device_id_)
        setDevice(prevDeviceID);
#endif
}

তার্কিক cv::cuda::DeviceInfo::isCompatible() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    // Check PTX compatibility
    যদি (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        প্রত্যাগম সত্য;

    // Check BIN compatibility
    প্রতি (পূর্ণাঙ্ক i = minorVersion(); i >= 0; --i)
        যদি (TargetArchs::hasBin(majorVersion(), i))
            প্রত্যাগম সত্য;

    প্রত্যাগম অসত্য;
#endif
}

////////////////////////////////////////////////////////////////////////
// print info

#ifdef HAVE_CUDA

নামাকাশ
{
    পূর্ণাঙ্ক convertSMVer2Cores(পূর্ণাঙ্ক major, পূর্ণাঙ্ক minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        প্রকারনির্দেশ সংরচনা {
            পূর্ণাঙ্ক SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            পূর্ণাঙ্ক Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        পূর্ণাঙ্ক index = 0;
        যাবত্ (gpuArchCoresPerSM[index].SM != -1)
        {
            যদি (gpuArchCoresPerSM[index].SM == ((major << 4) + minor) )
                প্রত্যাগম gpuArchCoresPerSM[index].Cores;
            index++;
        }

        প্রত্যাগম -1;
    }
}

#endif

শূন্য cv::cuda::printCudaDeviceInfo(পূর্ণাঙ্ক device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#অন্যথা
    পূর্ণাঙ্ক গুনতি_2 = get_Cuda_Enabled_Device_গুনতি();
    তার্কিক valid = (device >= 0) && (device < গুনতি_2);

    পূর্ণাঙ্ক beg = valid ? device   : 0;
    পূর্ণাঙ্ক end = valid ? device+1 : গুনতি_2;

    printf("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    printf("Device count: %d\n", গুনতি_2);

    পূর্ণাঙ্ক driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    ধ্রুব বর্ণ *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        রিক্ত
    };

    প্রতি(পূর্ণাঙ্ক dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        printf("\nDevice %d: \"%s\"\n", dev, prop.name);
        printf("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        printf("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        printf("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (ভিন্ন)prop.totalGlobalMem/1048576.0f, (unsigned long long) prop.totalGlobalMem);

        পূর্ণাঙ্ক cores = convertSMVer2Cores(prop.major, prop.minor);
        যদি (cores > 0)
            printf("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_গুনতি, cores, cores * prop.multi_Processor_গুনতি);

        পূর্ণাঙ্ক clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        printf("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        printf("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        printf("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        printf("  Total amount of constant memory:               %u bytes\n", (পূর্ণাঙ্ক)prop.totalConstMem);
        printf("  Total amount of shared memory per block:       %u bytes\n", (পূর্ণাঙ্ক)prop.sharedMemPerBlock);
        printf("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        printf("  Warp size:                                     %d\n", prop.warpSize);
        printf("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        printf("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.maxGridSize[0], prop.maxGridSize[1],  prop.maxGridSize[2]);
        printf("  Maximum memory pitch:                          %u bytes\n", (পূর্ণাঙ্ক)prop.memPitch);
        printf("  Texture alignment:                             %u bytes\n", (পূর্ণাঙ্ক)prop.textureAlignment);

        printf("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_গুনতি ? "Yes" : "No"), prop.async_Engine_গুনতি);
        পূর্ণাঙ্ক kernelExecTimeoutEnabled;
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

        পূর্ণাঙ্ক propComputeMode;
        cudaSafeCall(cudaDeviceGetAttribute(&propComputeMode, cudaDevAttrComputeMode, dev));
        printf("  Compute Mode:\n");
        printf("      %s \n", computeMode[propComputeMode]);
    }

    printf("\n");
    printf("deviceQuery, CUDA Driver = CUDART");
    printf(", CUDA Driver Version  = %d.%d", driverVersion / 1000, driverVersion % 100);
    printf(", CUDA Runtime Version = %d.%d", runtimeVersion/1000, runtimeVersion%100);
    printf(", NumDevs = %d\n\n", গুনতি_2);

    fflush(stdout);
#endif
}

শূন্য cv::cuda::printShortCudaDeviceInfo(পূর্ণাঙ্ক device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#অন্যথা
    পূর্ণাঙ্ক গুনতি_2 = get_Cuda_Enabled_Device_গুনতি();
    তার্কিক valid = (device >= 0) && (device < গুনতি_2);

    পূর্ণাঙ্ক beg = valid ? device   : 0;
    পূর্ণাঙ্ক end = valid ? device+1 : গুনতি_2;

    পূর্ণাঙ্ক driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    প্রতি(পূর্ণাঙ্ক dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        ধ্রুব বর্ণ *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        printf("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (ভিন্ন)prop.totalGlobalMem/1048576.0f);
        printf(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        পূর্ণাঙ্ক cores = convertSMVer2Cores(prop.major, prop.minor);
        যদি (cores > 0)
            printf(", %d cores", cores * prop.multi_Processor_গুনতি);

        printf(", Driver/Runtime ver.%d.%d/%d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
    }

    fflush(stdout);
#endif
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ifdef HAVE_CUDA

নামাকাশ
{
    #define error_entry(entry)  { entry, #entry }

    সংরচনা ErrorEntry
    {
        পূর্ণাঙ্ক code;
        ধ্রুব বর্ণ* str;
    };

    সংরচনা ErrorEntryComparer
    {
        পূর্ণাঙ্ক code;
        ErrorEntryComparer(পূর্ণাঙ্ক code_) : code(code_) {}
        তার্কিক operator()(ধ্রুব ErrorEntry& e) ধ্রুব { প্রত্যাগম e.code == code; }
    };

    ধ্রুব ErrorEntry npp_errors [] =
    {
    #যদি defined (_MSC_VER)
        error_entry( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #endif

    #যদি NPP_VERSION < 5500
        error_entry( NPP_BAD_ARG_ERROR ),
        error_entry( NPP_COEFF_ERROR ),
        error_entry( NPP_RECT_ERROR ),
        error_entry( NPP_QUAD_ERROR ),
        error_entry( NPP_MEMFREE_ERR ),
        error_entry( NPP_MEMSET_ERR ),
        error_entry( NPP_MEM_ALLOC_ERR ),
        error_entry( NPP_HISTO_সংখ্যা_OF_LEVELS_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERR ),
        error_entry( NPP_INVALID_প্রবেশ ),
        error_entry( NPP_POINTER_ERROR ),
        error_entry( NPP_WARNING ),
        error_entry( NPP_ODD_ROI_WARNING ),
    #অন্যথা
        error_entry( NPP_INVALID_HOST_POINTER_ERROR ),
        error_entry( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_entry( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_entry( NPP_ZC_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_MEMFREE_ERROR ),
        error_entry( NPP_MEMSET_ERROR ),
        error_entry( NPP_QUALITY_INDEX_ERROR ),
        error_entry( NPP_HISTOGRAM_সংখ্যা_OF_LEVELS_ERROR ),
        error_entry( NPP_CHANNEL_ORDER_ERROR ),
        error_entry( NPP_ZERO_MASK_মান_ERROR ),
        error_entry( NPP_QUADRANGLE_ERROR ),
        error_entry( NPP_RECTANGLE_ERROR ),
        error_entry( NPP_COEFFICIENT_ERROR ),
        error_entry( NPP_সংখ্যা_OF_CHANNELS_ERROR ),
        error_entry( NPP_COI_ERROR ),
        error_entry( NPP_ভাজক_ERROR ),
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
        error_entry( NPP_LUT_সংখ্যা_OF_LEVELS_ERROR ),
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

    ধ্রুব size_t npp_error_অঙ্ক = পরিমাণ(npp_errors) / পরিমাণ(npp_errors[0]);

    ধ্রুব ErrorEntry cu_errors [] =
    {
        error_entry( CUDA_SUCCESS                              ),
        error_entry( CUDA_ERROR_INVALID_মান                  ),
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
        error_entry( CUDA_ERROR_NO_দ্বিমিক_FOR_GPU              ),
        error_entry( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_entry( CUDA_ERROR_NOT_MAPPED                     ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_entry( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_entry( CUDA_ERROR_UNSUPPORTED_LIMIT              ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_IN_USE         ),
        error_entry( CUDA_ERROR_INVALID_SOURCE                 ),
        error_entry( CUDA_ERROR_FILE_NOT_FOUND                 ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_প্রতীক_NOT_FOUND ),
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

    ধ্রুব size_t cu_errors_অঙ্ক = পরিমাণ(cu_errors) / পরিমাণ(cu_errors[0]);

    cv::String getErrorString(পূর্ণাঙ্ক code, ধ্রুব ErrorEntry* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        ধ্রুব বর্ণ* বার্তা = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", বার্তা, code);

        প্রত্যাগম str;
    }
}

#endif

String cv::cuda::getNppErrorMessage(পূর্ণাঙ্ক code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    প্রত্যাগম String();
#অন্যথা
    প্রত্যাগম getErrorString(code, npp_errors, npp_error_অঙ্ক);
#endif
}

String cv::cuda::getCudaDriverApiErrorMessage(পূর্ণাঙ্ক code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    প্রত্যাগম String();
#অন্যথা
    প্রত্যাগম getErrorString(code, cu_errors, cu_errors_অঙ্ক);
#endif
}
