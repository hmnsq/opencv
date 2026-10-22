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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

using ନାମାକାଶ cv;
using ନାମାକାଶ cv::cuda;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::get_Cuda_Enabled_Device_ଗଣନା()
{
#ifndef HAVE_CUDA
    ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3;
    cudaError_t error = cuda_Get_Device_ଗଣନା(&ଗଣନା_3);

    ଯଦି (error == cudaErrorInsufficientDriver)
        ଫେରନ୍ତୁ -1;

    ଯଦି (error == cudaErrorNoDevice)
        ଫେରନ୍ତୁ 0;

    cudaSafeCall( error );
    ଫେରନ୍ତୁ ଗଣନା_3;
#endif
}

ଶୂନ୍ୟ cv::cuda::setDevice(ପୂର୍ଣ୍ଣସଂଖ୍ୟା device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#ଅନ୍ୟଥା
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::getDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା device;
    cudaSafeCall( cudaGetDevice(&device) );
    ଫେରନ୍ତୁ device;
#endif
}

ଶୂନ୍ୟ cv::cuda::resetDevice()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    cudaSafeCall( cudaDeviceReset() );
#endif
}

ତାର୍କିକ cv::cuda::deviceSupports(FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cache_size = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(ଆକାର(versions) / ଆକାର(versions[0]));

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା devId = getDevice();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା version;

    ଯଦି (devId < cache_size && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    ଅନ୍ୟଥା
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        ଯଦି (devId < cache_size)
            versions[devId] = version;
    }

    ଫେରନ୍ତୁ TargetArchs::builtWith(feature_set) && (version >= feature_set);
#endif
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ifdef HAVE_CUDA

ନାମାକାଶ
{
    ଶ୍ରେଣୀ CudaArch
    {
    ସର୍ବସାଧାରଣ:
        CudaArch();

        ତାର୍କିକ builtWith(FeatureSet feature_set) ଅଚଳ;
        ତାର୍କିକ hasPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ;
        ତାର୍କିକ hasBin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ;
        ତାର୍କିକ hasEqualOrLessPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ;
        ତାର୍କିକ hasEqualOrGreaterPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ;
        ତାର୍କିକ hasEqualOrGreaterBin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ;

    ବ୍ୟକ୍ତିଗତ:
        ସ୍ଥିର ଶୂନ୍ୟ fromStr(ଅଚଳ ଅକ୍ଷର* set_as_str, std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& arr);

        std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> bin;
        std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ptx;
        std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> features;
    };

    ଅଚଳ CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    ତାର୍କିକ CudaArch::builtWith(FeatureSet feature_set) ଅଚଳ
    {
        ଫେରନ୍ତୁ !features.empty() && (features.back() >= feature_set);
    }

    ତାର୍କିକ CudaArch::hasPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ
    {
        ଫେରନ୍ତୁ std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    ତାର୍କିକ CudaArch::hasBin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ
    {
        ଫେରନ୍ତୁ std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    ତାର୍କିକ CudaArch::hasEqualOrLessPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ
    {
        ଫେରନ୍ତୁ !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    ତାର୍କିକ CudaArch::hasEqualOrGreaterPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ
    {
        ଫେରନ୍ତୁ !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    ତାର୍କିକ CudaArch::hasEqualOrGreaterBin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor) ଅଚଳ
    {
        ଫେରନ୍ତୁ !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    ଶୂନ୍ୟ CudaArch::fromStr(ଅଚଳ ଅକ୍ଷର* set_as_str, std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& arr)
    {
        arr.clear();

        ଅଚଳ size_t len = strlen(set_as_str);

        size_t pos = 0;
        ଯେତେବେଳେ (pos < len)
        {
            ଯଦି (isspace(set_as_str[pos]))
            {
                ++pos;
            }
            ଅନ୍ୟଥା
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା cur_ମୂଲ୍ୟ;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_read;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା args_read = sscanf(set_as_str + pos, "%d%n", &cur_ମୂଲ୍ୟ, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_ମୂଲ୍ୟ);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#endif

ତାର୍କିକ cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#ifndef HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ cudaArch.builtWith(feature_set);
#endif
}

ତାର୍କିକ cv::cuda::TargetArchs::hasPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ cudaArch.hasPtx(major, minor);
#endif
}

ତାର୍କିକ cv::cuda::TargetArchs::hasBin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ cudaArch.hasBin(major, minor);
#endif
}

ତାର୍କିକ cv::cuda::TargetArchs::hasEqualOrLessPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ cudaArch.hasEqualOrLessPtx(major, minor);
#endif
}

ତାର୍କିକ cv::cuda::TargetArchs::hasEqualOrGreaterPtx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ cudaArch.hasEqualOrGreaterPtx(major, minor);
#endif
}

ତାର୍କିକ cv::cuda::TargetArchs::hasEqualOrGreaterBin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor)
{
#ifndef HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ cudaArch.hasEqualOrGreaterBin(major, minor);
#endif
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ifdef HAVE_CUDA

ନାମାକାଶ
{
    ଶ୍ରେଣୀ DeviceProps
    {
    ସର୍ବସାଧାରଣ:
        DeviceProps();

        ଅଚଳ cudaDeviceProp* get(ପୂର୍ଣ୍ଣସଂଖ୍ୟା devID) ଅଚଳ;

    ବ୍ୟକ୍ତିଗତ:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = get_Cuda_Enabled_Device_ଗଣନା();

        ଯଦି (ଗଣନା_3 > 0)
        {
            props_.resize(ଗଣନା_3);

            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା devID = 0; devID < ଗଣନା_3; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ଅଚଳ cudaDeviceProp* DeviceProps::get(ପୂର୍ଣ୍ଣସଂଖ୍ୟା devID) ଅଚଳ
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        ଫେରନ୍ତୁ &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        ସ୍ଥିର DeviceProps props;
        ଫେରନ୍ତୁ props;
    }
}

#endif

ଅଚଳ ଅକ୍ଷର* cv::cuda::DeviceInfo::name() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->name;
#endif
}

size_t cv::cuda::DeviceInfo::totalGlobalMem() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->totalGlobalMem;
#endif
}

size_t cv::cuda::DeviceInfo::sharedMemPerBlock() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->sharedMemPerBlock;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::regsPerBlock() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->regsPerBlock;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::warpSize() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->warpSize;
#endif
}

size_t cv::cuda::DeviceInfo::memPitch() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->memPitch;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::maxThreadsPerBlock() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->maxThreadsPerBlock;
#endif
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec3i(deviceProps().get(device_id_)->maxThreadsDim);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxGridSize() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec3i(deviceProps().get(device_id_)->maxGridSize);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::clockRate() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    ଫେରନ୍ତୁ clockRate;
#endif
}

size_t cv::cuda::DeviceInfo::totalConstMem() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->totalConstMem;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::majorVersion() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->major;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::minorVersion() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->minor;
#endif
}

size_t cv::cuda::DeviceInfo::textureAlignment() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->textureAlignment;
#endif
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->texturePitchAlignment;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::multi_Processor_ଗଣନା() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->multi_Processor_ଗଣନା;
#endif
}

ତାର୍କିକ cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    ଫେରନ୍ତୁ kernelExecTimeoutEnabled != 0;
#endif
}

ତାର୍କିକ cv::cuda::DeviceInfo::integrated() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->integrated != 0;
#endif
}

ତାର୍କିକ cv::cuda::DeviceInfo::canMapHostMemory() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->canMapHostMemory != 0;
#endif
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ସ୍ଥିର ଅଚଳ ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    ଫେରନ୍ତୁ tbl[computeMode];
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::maxTexture1D() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->maxTexture1D;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::maxTexture1DMipmap() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    #ଯଦି CUDA_VERSION >= 5000
        ଫେରନ୍ତୁ deviceProps().get(device_id_)->maxTexture1DMipmap;
    #ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        ଫେରନ୍ତୁ 0;
    #endif
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::maxTexture1DLinear() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    #ଯଦି CUDA_VERSION >= 13000
        size_t maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        ଫେରନ୍ତୁ maxWidthInElements;
    #ଅନ୍ୟଥା
        ଫେରନ୍ତୁ deviceProps().get(device_id_)->maxTexture1DLinear;
    #endif
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    #ଯଦି CUDA_VERSION >= 5000
        ଫେରନ୍ତୁ Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        ଫେରନ୍ତୁ Vec2i();
    #endif
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::maxTextureCubemap() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->maxTextureCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::maxSurface1D() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->maxSurface1D;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#endif
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::maxSurfaceCubemap() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->maxSurfaceCubemap;
#endif
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#endif
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->surfaceAlignment;
#endif
}

ତାର୍କିକ cv::cuda::DeviceInfo::concurrentKernels() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->concurrentKernels != 0;
#endif
}

ତାର୍କିକ cv::cuda::DeviceInfo::ECCEnabled() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->ECCEnabled != 0;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::pciBusID() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->pciBusID;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::pciDeviceID() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->pciDeviceID;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::pciDomainID() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->pciDomainID;
#endif
}

ତାର୍କିକ cv::cuda::DeviceInfo::tccDriver() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->tccDriver != 0;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::async_Engine_ଗଣନା() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->async_Engine_ଗଣନା;
#endif
}

ତାର୍କିକ cv::cuda::DeviceInfo::unifiedAddressing() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->unifiedAddressing != 0;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::memoryClockRate() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    ଫେରନ୍ତୁ memoryClockRate;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::memoryBusWidth() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->memoryBusWidth;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::l2CacheSize() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->l2CacheSize;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ deviceProps().get(device_id_)->maxThreadsPerMultiProcessor;
#endif
}

ଶୂନ୍ୟ cv::cuda::DeviceInfo::queryMemory(size_t& _totalMemory, size_t& _freeMemory) ଅଚଳ
{
#ifndef HAVE_CUDA
    CV_UNUSED(_totalMemory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା prevDeviceID = getDevice();
    ଯଦି (prevDeviceID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &_totalMemory) );

    ଯଦି (prevDeviceID != device_id_)
        setDevice(prevDeviceID);
#endif
}

ତାର୍କିକ cv::cuda::DeviceInfo::isCompatible() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    // Check PTX compatibility
    ଯଦି (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        ଫେରନ୍ତୁ ସତ୍ୟ;

    // Check BIN compatibility
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = minorVersion(); i >= 0; --i)
        ଯଦି (TargetArchs::hasBin(majorVersion(), i))
            ଫେରନ୍ତୁ ସତ୍ୟ;

    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

////////////////////////////////////////////////////////////////////////
// print info

#ifdef HAVE_CUDA

ନାମାକାଶ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା convertSMVer2Cores(ପୂର୍ଣ୍ଣସଂଖ୍ୟା major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = 0;
        ଯେତେବେଳେ (gpuArchCoresPerSM[index].SM != -1)
        {
            ଯଦି (gpuArchCoresPerSM[index].SM == ((major << 4) + minor) )
                ଫେରନ୍ତୁ gpuArchCoresPerSM[index].Cores;
            index++;
        }

        ଫେରନ୍ତୁ -1;
    }
}

#endif

ଶୂନ୍ୟ cv::cuda::printCudaDeviceInfo(ପୂର୍ଣ୍ଣସଂଖ୍ୟା device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = get_Cuda_Enabled_Device_ଗଣନା();
    ତାର୍କିକ valid = (device >= 0) && (device < ଗଣନା_3);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା beg = valid ? device   : 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = valid ? device+1 : ଗଣନା_3;

    printf("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    printf("Device count: %d\n", ଗଣନା_3);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    ଅଚଳ ଅକ୍ଷର *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        ଖାଲି
    };

    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        printf("\nDevice %d: \"%s\"\n", dev, prop.name);
        printf("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        printf("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        printf("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (ଦଶମିକ)prop.totalGlobalMem/1048576.0f, (unsigned long long) prop.totalGlobalMem);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cores = convertSMVer2Cores(prop.major, prop.minor);
        ଯଦି (cores > 0)
            printf("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_ଗଣନା, cores, cores * prop.multi_Processor_ଗଣନା);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        printf("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        printf("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        printf("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        printf("  Total amount of constant memory:               %u bytes\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prop.totalConstMem);
        printf("  Total amount of shared memory per block:       %u bytes\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prop.sharedMemPerBlock);
        printf("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        printf("  Warp size:                                     %d\n", prop.warpSize);
        printf("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        printf("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        printf("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.maxGridSize[0], prop.maxGridSize[1],  prop.maxGridSize[2]);
        printf("  Maximum memory pitch:                          %u bytes\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prop.memPitch);
        printf("  Texture alignment:                             %u bytes\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)prop.textureAlignment);

        printf("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_ଗଣନା ? "Yes" : "No"), prop.async_Engine_ଗଣନା);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା kernelExecTimeoutEnabled;
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

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା propComputeMode;
        cudaSafeCall(cudaDeviceGetAttribute(&propComputeMode, cudaDevAttrComputeMode, dev));
        printf("  Compute Mode:\n");
        printf("      %s \n", computeMode[propComputeMode]);
    }

    printf("\n");
    printf("deviceQuery, CUDA Driver = CUDART");
    printf(", CUDA Driver Version  = %d.%d", driverVersion / 1000, driverVersion % 100);
    printf(", CUDA Runtime Version = %d.%d", runtimeVersion/1000, runtimeVersion%100);
    printf(", NumDevs = %d\n\n", ଗଣନା_3);

    fflush(stdout);
#endif
}

ଶୂନ୍ୟ cv::cuda::printShortCudaDeviceInfo(ପୂର୍ଣ୍ଣସଂଖ୍ୟା device)
{
#ifndef HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3 = get_Cuda_Enabled_Device_ଗଣନା();
    ତାର୍କିକ valid = (device >= 0) && (device < ଗଣନା_3);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା beg = valid ? device   : 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = valid ? device+1 : ଗଣନା_3;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        ଅଚଳ ଅକ୍ଷର *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        printf("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (ଦଶମିକ)prop.totalGlobalMem/1048576.0f);
        printf(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cores = convertSMVer2Cores(prop.major, prop.minor);
        ଯଦି (cores > 0)
            printf(", %d cores", cores * prop.multi_Processor_ଗଣନା);

        printf(", Driver/Runtime ver.%d.%d/%d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
    }

    fflush(stdout);
#endif
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ifdef HAVE_CUDA

ନାମାକାଶ
{
    #define error_entry(entry)  { entry, #entry }

    ସଂରଚନା ErrorEntry
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;
        ଅଚଳ ଅକ୍ଷର* str;
    };

    ସଂରଚନା ErrorEntryComparer
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;
        ErrorEntryComparer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା code_) : code(code_) {}
        ତାର୍କିକ operator()(ଅଚଳ ErrorEntry& e) ଅଚଳ { ଫେରନ୍ତୁ e.code == code; }
    };

    ଅଚଳ ErrorEntry npp_errors [] =
    {
    #ଯଦି defined (_MSC_VER)
        error_entry( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #endif

    #ଯଦି NPP_VERSION < 5500
        error_entry( NPP_BAD_ARG_ERROR ),
        error_entry( NPP_COEFF_ERROR ),
        error_entry( NPP_RECT_ERROR ),
        error_entry( NPP_QUAD_ERROR ),
        error_entry( NPP_MEMFREE_ERR ),
        error_entry( NPP_MEMSET_ERR ),
        error_entry( NPP_MEM_ALLOC_ERR ),
        error_entry( NPP_HISTO_ସଂଖ୍ୟା_OF_LEVELS_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERR ),
        error_entry( NPP_INVALID_ନିବେଶ ),
        error_entry( NPP_POINTER_ERROR ),
        error_entry( NPP_WARNING ),
        error_entry( NPP_ODD_ROI_WARNING ),
    #ଅନ୍ୟଥା
        error_entry( NPP_INVALID_HOST_POINTER_ERROR ),
        error_entry( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_entry( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_entry( NPP_ZC_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_MEMFREE_ERROR ),
        error_entry( NPP_MEMSET_ERROR ),
        error_entry( NPP_QUALITY_INDEX_ERROR ),
        error_entry( NPP_HISTOGRAM_ସଂଖ୍ୟା_OF_LEVELS_ERROR ),
        error_entry( NPP_CHANNEL_ORDER_ERROR ),
        error_entry( NPP_ZERO_MASK_ମୂଲ୍ୟ_ERROR ),
        error_entry( NPP_QUADRANGLE_ERROR ),
        error_entry( NPP_RECTANGLE_ERROR ),
        error_entry( NPP_COEFFICIENT_ERROR ),
        error_entry( NPP_ସଂଖ୍ୟା_OF_CHANNELS_ERROR ),
        error_entry( NPP_COI_ERROR ),
        error_entry( NPP_ଭାଜକ_ERROR ),
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
        error_entry( NPP_LUT_ସଂଖ୍ୟା_OF_LEVELS_ERROR ),
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

    ଅଚଳ size_t npp_error_ଅଙ୍କ = ଆକାର(npp_errors) / ଆକାର(npp_errors[0]);

    ଅଚଳ ErrorEntry cu_errors [] =
    {
        error_entry( CUDA_SUCCESS                              ),
        error_entry( CUDA_ERROR_INVALID_ମୂଲ୍ୟ                  ),
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
        error_entry( CUDA_ERROR_NO_ଦ୍ୱିମିକ_FOR_GPU              ),
        error_entry( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_entry( CUDA_ERROR_NOT_MAPPED                     ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_entry( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_entry( CUDA_ERROR_UNSUPPORTED_LIMIT              ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_IN_USE         ),
        error_entry( CUDA_ERROR_INVALID_SOURCE                 ),
        error_entry( CUDA_ERROR_FILE_NOT_FOUND                 ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_ପ୍ରତୀକ_NOT_FOUND ),
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

    ଅଚଳ size_t cu_errors_ଅଙ୍କ = ଆକାର(cu_errors) / ଆକାର(cu_errors[0]);

    cv::String getErrorString(ପୂର୍ଣ୍ଣସଂଖ୍ୟା code, ଅଚଳ ErrorEntry* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        ଅଚଳ ଅକ୍ଷର* ସନ୍ଦେଶ = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", ସନ୍ଦେଶ, code);

        ଫେରନ୍ତୁ str;
    }
}

#endif

String cv::cuda::getNppErrorMessage(ପୂର୍ଣ୍ଣସଂଖ୍ୟା code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    ଫେରନ୍ତୁ String();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ getErrorString(code, npp_errors, npp_error_ଅଙ୍କ);
#endif
}

String cv::cuda::getCudaDriverApiErrorMessage(ପୂର୍ଣ୍ଣସଂଖ୍ୟା code)
{
#ifndef HAVE_CUDA
    CV_UNUSED(code);
    ଫେରନ୍ତୁ String();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ getErrorString(code, cu_errors, cu_errors_ଅଙ୍କ);
#endif
}
