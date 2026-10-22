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

পূর্ণসংখ্যা cv::cuda::get_Cuda_Enabled_Device_গুনতি()
{
#ifndef HAVE_CUDA
    ফেরত 0;
#অন্যথা
    পূর্ণসংখ্যা গুনতি_2;
    cudaError_t error = cuda_Get_Device_গুনতি(&গুনতি_2);

    যদি (error == cudaErrorInsufficientDriver)
        ফেরত -1;

    যদি (error == cudaErrorNoDevice)
        ফেরত 0;

    cudaSafeCall( error );
    ফেরত গুনতি_2;
#endif
}

শূন্য cv::cuda::setDevice(পূর্ণসংখ্যা device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#অন্যথা
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#endif
}

পূর্ণসংখ্যা cv::cuda::getDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    পূর্ণসংখ্যা device;
    cudaSafeCall( cudaGetDevice(&device) );
    ফেরত device;
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

যৌক্তিক cv::cuda::deviceSupports(FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#অন্যথা
    স্থির পূর্ণসংখ্যা versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    স্থির ধ্রুবক পূর্ণসংখ্যা cache_আকার = static_cast<পূর্ণসংখ্যা>(আকার(versions) / আকার(versions[0]));

    ধ্রুবক পূর্ণসংখ্যা devId = getDevice();

    পূর্ণসংখ্যা version;

    যদি (devId < cache_আকার && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    অন্যথা
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        যদি (devId < cache_আকার)
            versions[devId] = version;
    }

    ফেরত TargetArchs::builtWith(feature_set) && (version >= feature_set);
#endif
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ifdef HAVE_CUDA

নামাকাশ
{
    শ্রেণী CudaArch
    {
    সর্বজনীন:
        CudaArch();

        যৌক্তিক builtWith(FeatureSet feature_set) ধ্রুবক;
        যৌক্তিক hasPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক;
        যৌক্তিক hasBin(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক;
        যৌক্তিক hasEqualOrLessPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক;
        যৌক্তিক hasEqualOrGreaterPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক;
        যৌক্তিক hasEqualOrGreaterBin(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক;

    ব্যক্তিগত:
        স্থির শূন্য fromStr(ধ্রুবক অক্ষর* set_as_str, std::vector<পূর্ণসংখ্যা>& arr);

        std::vector<পূর্ণসংখ্যা> bin;
        std::vector<পূর্ণসংখ্যা> ptx;
        std::vector<পূর্ণসংখ্যা> features;
    };

    ধ্রুবক CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    যৌক্তিক CudaArch::builtWith(FeatureSet feature_set) ধ্রুবক
    {
        ফেরত !features.empty() && (features.back() >= feature_set);
    }

    যৌক্তিক CudaArch::hasPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক
    {
        ফেরত std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    যৌক্তিক CudaArch::hasBin(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক
    {
        ফেরত std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    যৌক্তিক CudaArch::hasEqualOrLessPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক
    {
        ফেরত !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    যৌক্তিক CudaArch::hasEqualOrGreaterPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক
    {
        ফেরত !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    যৌক্তিক CudaArch::hasEqualOrGreaterBin(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor) ধ্রুবক
    {
        ফেরত !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    শূন্য CudaArch::fromStr(ধ্রুবক অক্ষর* set_as_str, std::vector<পূর্ণসংখ্যা>& arr)
    {
        arr.clear();

        ধ্রুবক size_t len = strlen(set_as_str);

        size_t pos = 0;
        যখন (pos < len)
        {
            যদি (isspace(set_as_str[pos]))
            {
                ++pos;
            }
            অন্যথা
            {
                পূর্ণসংখ্যা cur_মান;
                পূর্ণসংখ্যা chars_read;
                পূর্ণসংখ্যা args_read = sscanf(set_as_str + pos, "%d%n", &cur_মান, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_মান);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#endif

যৌক্তিক cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#অন্যথা
    ফেরত cudaArch.builtWith(feature_set);
#endif
}

যৌক্তিক cv::cuda::TargetArchs::hasPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    ফেরত cudaArch.hasPtx(major, minor);
#endif
}

যৌক্তিক cv::cuda::TargetArchs::hasBin(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    ফেরত cudaArch.hasBin(major, minor);
#endif
}

যৌক্তিক cv::cuda::TargetArchs::hasEqualOrLessPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    ফেরত cudaArch.hasEqualOrLessPtx(major, minor);
#endif
}

যৌক্তিক cv::cuda::TargetArchs::hasEqualOrGreaterPtx(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    ফেরত cudaArch.hasEqualOrGreaterPtx(major, minor);
#endif
}

যৌক্তিক cv::cuda::TargetArchs::hasEqualOrGreaterBin(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#অন্যথা
    ফেরত cudaArch.hasEqualOrGreaterBin(major, minor);
#endif
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ifdef HAVE_CUDA

নামাকাশ
{
    শ্রেণী DeviceProps
    {
    সর্বজনীন:
        DeviceProps();

        ধ্রুবক cudaDeviceProp* get(পূর্ণসংখ্যা devID) ধ্রুবক;

    ব্যক্তিগত:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        পূর্ণসংখ্যা গুনতি_2 = get_Cuda_Enabled_Device_গুনতি();

        যদি (গুনতি_2 > 0)
        {
            props_.resize(গুনতি_2);

            জন্য (পূর্ণসংখ্যা devID = 0; devID < গুনতি_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ধ্রুবক cudaDeviceProp* DeviceProps::get(পূর্ণসংখ্যা devID) ধ্রুবক
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        ফেরত &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        স্থির DeviceProps props;
        ফেরত props;
    }
}

#endif

ধ্রুবক অক্ষর* cv::cuda::DeviceInfo::name() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->name;
#endif
}

size_t cv::cuda::DeviceInfo::যোগ_Global_Mem() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->যোগ_Global_Mem;
#endif
}

size_t cv::cuda::DeviceInfo::sharedMemPerBlock() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->sharedMemPerBlock;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::regsPerBlock() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->regsPerBlock;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::warp_আকার() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->warp_আকার;
#endif
}

size_t cv::cuda::DeviceInfo::memPitch() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->memPitch;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::maxThreadsPerBlock() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->maxThreadsPerBlock;
#endif
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec3i(deviceProps().get(device_id_)->maxThreadsDim);
#endif
}

Vec3i cv::cuda::DeviceInfo::max_Grid_আকার() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec3i(deviceProps().get(device_id_)->max_Grid_আকার);
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::clockRate() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    পূর্ণসংখ্যা clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    ফেরত clockRate;
#endif
}

size_t cv::cuda::DeviceInfo::যোগ_Const_Mem() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->যোগ_Const_Mem;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::majorVersion() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->major;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::minorVersion() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->minor;
#endif
}

size_t cv::cuda::DeviceInfo::textureAlignment() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->textureAlignment;
#endif
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->texturePitchAlignment;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::multi_Processor_গুনতি() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->multi_Processor_গুনতি;
#endif
}

যৌক্তিক cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    পূর্ণসংখ্যা kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    ফেরত kernelExecTimeoutEnabled != 0;
#endif
}

যৌক্তিক cv::cuda::DeviceInfo::integrated() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->integrated != 0;
#endif
}

যৌক্তিক cv::cuda::DeviceInfo::canMapHostMemory() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->canMapHostMemory != 0;
#endif
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    স্থির ধ্রুবক ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    পূর্ণসংখ্যা computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    ফেরত tbl[computeMode];
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::maxTexture1D() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->maxTexture1D;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::maxTexture1DMipmap() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    #যদি CUDA_VERSION >= 5000
        ফেরত deviceProps().get(device_id_)->maxTexture1DMipmap;
    #অন্যথা
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        ফেরত 0;
    #endif
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::maxTexture1DLinear() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    #যদি CUDA_VERSION >= 13000
        size_t maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        ফেরত maxWidthInElements;
    #অন্যথা
        ফেরত deviceProps().get(device_id_)->maxTexture1DLinear;
    #endif
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    #যদি CUDA_VERSION >= 5000
        ফেরত Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #অন্যথা
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        ফেরত Vec2i();
    #endif
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::maxTextureCubemap() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->maxTextureCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::maxSurface1D() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->maxSurface1D;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::maxSurfaceCubemap() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->maxSurfaceCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#endif
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->surfaceAlignment;
#endif
}

যৌক্তিক cv::cuda::DeviceInfo::concurrentKernels() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->concurrentKernels != 0;
#endif
}

যৌক্তিক cv::cuda::DeviceInfo::ECCEnabled() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->ECCEnabled != 0;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::pciBusID() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->pciBusID;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::pciDeviceID() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->pciDeviceID;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::pciDomainID() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->pciDomainID;
#endif
}

যৌক্তিক cv::cuda::DeviceInfo::tccDriver() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->tccDriver != 0;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::async_Engine_গুনতি() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->async_Engine_গুনতি;
#endif
}

যৌক্তিক cv::cuda::DeviceInfo::unifiedAddressing() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->unifiedAddressing != 0;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::memoryClockRate() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    পূর্ণসংখ্যা memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    ফেরত memoryClockRate;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::memoryBusWidth() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->memoryBusWidth;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::l2_Cache_আকার() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->l2_Cache_আকার;
#endif
}

পূর্ণসংখ্যা cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ফেরত deviceProps().get(device_id_)->maxThreadsPerMultiProcessor;
#endif
}

শূন্য cv::cuda::DeviceInfo::queryMemory(size_t& যোগ_Memory, size_t& _freeMemory) ধ্রুবক
{
#ifndef HAVE_CUDA
    CV_UNUSED(যোগ_Memory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#অন্যথা
    পূর্ণসংখ্যা পূর্ব_Device_ID = getDevice();
    যদি (পূর্ব_Device_ID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &যোগ_Memory) );

    যদি (পূর্ব_Device_ID != device_id_)
        setDevice(পূর্ব_Device_ID);
#endif
}

যৌক্তিক cv::cuda::DeviceInfo::isCompatible() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    // Check PTX compatibility
    যদি (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        ফেরত সত্য;

    // Check BIN compatibility
    জন্য (পূর্ণসংখ্যা i = minorVersion(); i >= 0; --i)
        যদি (TargetArchs::hasBin(majorVersion(), i))
            ফেরত সত্য;

    ফেরত মিথ্যা;
#endif
}

////////////////////////////////////////////////////////////////////////
// print info

#ifdef HAVE_CUDA

নামাকাশ
{
    পূর্ণসংখ্যা convertSMVer2Cores(পূর্ণসংখ্যা major, পূর্ণসংখ্যা minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        প্রকারলেখ কাঠামো {
            পূর্ণসংখ্যা SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            পূর্ণসংখ্যা Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        পূর্ণসংখ্যা সূচক = 0;
        যখন (gpuArchCoresPerSM[সূচক].SM != -1)
        {
            যদি (gpuArchCoresPerSM[সূচক].SM == ((major << 4) + minor) )
                ফেরত gpuArchCoresPerSM[সূচক].Cores;
            সূচক++;
        }

        ফেরত -1;
    }
}

#endif

শূন্য cv::cuda::printCudaDeviceInfo(পূর্ণসংখ্যা device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#অন্যথা
    পূর্ণসংখ্যা গুনতি_2 = get_Cuda_Enabled_Device_গুনতি();
    যৌক্তিক valid = (device >= 0) && (device < গুনতি_2);

    পূর্ণসংখ্যা beg = valid ? device   : 0;
    পূর্ণসংখ্যা end = valid ? device+1 : গুনতি_2;

    printf("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    printf("Device count: %d\n", গুনতি_2);

    পূর্ণসংখ্যা driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    ধ্রুবক অক্ষর *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        ফাঁকা
    };

    জন্য(পূর্ণসংখ্যা dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        printf("\nDevice %d: \"%s\"\n", dev, prop.name);
        printf("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        printf("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        printf("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (দশমিক)prop.যোগ_Global_Mem/1048576.0f, (unsigned long long) prop.যোগ_Global_Mem);

        পূর্ণসংখ্যা cores = convertSMVer2Cores(prop.major, prop.minor);
        যদি (cores > 0)
            printf("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_গুনতি, cores, cores * prop.multi_Processor_গুনতি);

        পূর্ণসংখ্যা clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        printf("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        printf("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        printf("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        printf("  Total amount of constant memory:               %u bytes\n", (পূর্ণসংখ্যা)prop.যোগ_Const_Mem);
        printf("  Total amount of shared memory per block:       %u bytes\n", (পূর্ণসংখ্যা)prop.sharedMemPerBlock);
        printf("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        printf("  Warp size:                                     %d\n", prop.warp_আকার);
        printf("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        printf("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_আকার[0], prop.max_Grid_আকার[1],  prop.max_Grid_আকার[2]);
        printf("  Maximum memory pitch:                          %u bytes\n", (পূর্ণসংখ্যা)prop.memPitch);
        printf("  Texture alignment:                             %u bytes\n", (পূর্ণসংখ্যা)prop.textureAlignment);

        printf("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_গুনতি ? "Yes" : "No"), prop.async_Engine_গুনতি);
        পূর্ণসংখ্যা kernelExecTimeoutEnabled;
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

        পূর্ণসংখ্যা propComputeMode;
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

শূন্য cv::cuda::printShortCudaDeviceInfo(পূর্ণসংখ্যা device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#অন্যথা
    পূর্ণসংখ্যা গুনতি_2 = get_Cuda_Enabled_Device_গুনতি();
    যৌক্তিক valid = (device >= 0) && (device < গুনতি_2);

    পূর্ণসংখ্যা beg = valid ? device   : 0;
    পূর্ণসংখ্যা end = valid ? device+1 : গুনতি_2;

    পূর্ণসংখ্যা driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    জন্য(পূর্ণসংখ্যা dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        ধ্রুবক অক্ষর *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        printf("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (দশমিক)prop.যোগ_Global_Mem/1048576.0f);
        printf(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        পূর্ণসংখ্যা cores = convertSMVer2Cores(prop.major, prop.minor);
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

    কাঠামো ErrorEntry
    {
        পূর্ণসংখ্যা code;
        ধ্রুবক অক্ষর* str;
    };

    কাঠামো ErrorEntryComparer
    {
        পূর্ণসংখ্যা code;
        ErrorEntryComparer(পূর্ণসংখ্যা code_) : code(code_) {}
        যৌক্তিক operator()(ধ্রুবক ErrorEntry& e) ধ্রুবক { ফেরত e.code == code; }
    };

    ধ্রুবক ErrorEntry npp_errors [] =
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
        error_entry( NPP_QUALITY_সূচক_ERROR ),
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
        error_entry( NPP_MASK_আকার_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERROR ),
        error_entry( NPP_MOMENT_00_ZERO_ERROR ),
        error_entry( NPP_THRESHOLD_NEGATIVE_LEVEL_ERROR ),
        error_entry( NPP_THRESHOLD_ERROR ),
        error_entry( NPP_CONTEXT_MATCH_ERROR ),
        error_entry( NPP_FFT_FLAG_ERROR ),
        error_entry( NPP_FFT_ORDER_ERROR ),
        error_entry( NPP_SCALE_RANGE_ERROR ),
        error_entry( NPP_তথ্য_TYPE_ERROR ),
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
        error_entry( NPP_আকার_ERROR ),
        error_entry( NPP_NULL_POINTER_ERROR ),
        error_entry( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_entry( NPP_NOT_IMPLEMENTED_ERROR ),
        error_entry( NPP_ERROR ),
        error_entry( NPP_NO_ERROR ),
        error_entry( NPP_SUCCESS ),
        error_entry( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_entry( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_entry( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_entry( NPP_DOUBLE_আকার_WARNING )
    };

    ধ্রুবক size_t npp_error_অঙ্ক = আকার(npp_errors) / আকার(npp_errors[0]);

    ধ্রুবক ErrorEntry cu_errors [] =
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

    ধ্রুবক size_t cu_errors_অঙ্ক = আকার(cu_errors) / আকার(cu_errors[0]);

    cv::String getErrorString(পূর্ণসংখ্যা code, ধ্রুবক ErrorEntry* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        ধ্রুবক অক্ষর* বার্তা = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", বার্তা, code);

        ফেরত str;
    }
}

#endif

String cv::cuda::getNppErrorMessage(পূর্ণসংখ্যা code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    ফেরত String();
#অন্যথা
    ফেরত getErrorString(code, npp_errors, npp_error_অঙ্ক);
#endif
}

String cv::cuda::getCudaDriverApiErrorMessage(পূর্ণসংখ্যা code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    ফেরত String();
#অন্যথা
    ফেরত getErrorString(code, cu_errors, cu_errors_অঙ্ক);
#endif
}
