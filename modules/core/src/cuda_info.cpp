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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::cuda;

ਪੂਰਨ cv::cuda::get_Cuda_Enabled_Device_ਗਿਣਤੀ()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ ਗਿਣਤੀ_3;
    cudaError_t error = cuda_Get_Device_ਗਿਣਤੀ(&ਗਿਣਤੀ_3);

    ਜੇ (error == cudaErrorInsufficientDriver)
        ਵਾਪਸ -1;

    ਜੇ (error == cudaErrorNoDevice)
        ਵਾਪਸ 0;

    cuda_ਸੁਰੱਖਿਅਤ_Call( error );
    ਵਾਪਸ ਗਿਣਤੀ_3;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::ਸੈੱਟ_Device(ਪੂਰਨ device)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(device);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_ਸੈੱਟ_Device(device) );
    cuda_ਸੁਰੱਖਿਅਤ_Call( cudaFree(0) );
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::getDevice()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ device;
    cuda_ਸੁਰੱਖਿਅਤ_Call( cudaGetDevice(&device) );
    ਵਾਪਸ device;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::resetDevice()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    cuda_ਸੁਰੱਖਿਅਤ_Call( cudaDeviceReset() );
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::deviceSupports(Feature_ਸੈੱਟ feature_ਸੈੱਟ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(feature_ਸੈੱਟ);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਸਥਿਰ ਪੂਰਨ versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    ਸਥਿਰ ਅਚਲ ਪੂਰਨ cache_ਮਾਪ = static_cast<ਪੂਰਨ>(ਆਕਾਰ(versions) / ਆਕਾਰ(versions[0]));

    ਅਚਲ ਪੂਰਨ devId = getDevice();

    ਪੂਰਨ ਸੰਸਕਰਣ;

    ਜੇ (devId < cache_ਮਾਪ && versions[devId] >= 0)
    {
        ਸੰਸਕਰਣ = versions[devId];
    }
    ਨਹੀਂਤਾਂ
    {
        DeviceInfo dev(devId);
        ਸੰਸਕਰਣ = dev.major_ਸੰਸਕਰਣ() * 10 + dev.minor_ਸੰਸਕਰਣ();
        ਜੇ (devId < cache_ਮਾਪ)
            versions[devId] = ਸੰਸਕਰਣ;
    }

    ਵਾਪਸ ਨਿਸ਼ਾਨਾ_Archs::builtWith(feature_ਸੈੱਟ) && (ਸੰਸਕਰਣ >= feature_ਸੈੱਟ);
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼
{
    ਵਰਗ CudaArch
    {
    ਜਨਤਕ:
        CudaArch();

        ਤਾਰਕਿਕ builtWith(Feature_ਸੈੱਟ feature_ਸੈੱਟ) ਅਚਲ;
        ਤਾਰਕਿਕ hasPtx(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ;
        ਤਾਰਕਿਕ hasBin(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ;
        ਤਾਰਕਿਕ hasEqualOrLessPtx(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ;
        ਤਾਰਕਿਕ hasEqualOrGreaterPtx(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ;
        ਤਾਰਕਿਕ hasEqualOrGreaterBin(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ;

    ਨਿਜੀ:
        ਸਥਿਰ ਖਾਲੀ fromStr(ਅਚਲ ਅੱਖਰ* ਸੈੱਟ_as_str, std::vector<ਪੂਰਨ>& arr);

        std::vector<ਪੂਰਨ> bin;
        std::vector<ਪੂਰਨ> ptx;
        std::vector<ਪੂਰਨ> features;
    };

    ਅਚਲ CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    ਤਾਰਕਿਕ CudaArch::builtWith(Feature_ਸੈੱਟ feature_ਸੈੱਟ) ਅਚਲ
    {
        ਵਾਪਸ !features.empty() && (features.back() >= feature_ਸੈੱਟ);
    }

    ਤਾਰਕਿਕ CudaArch::hasPtx(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ
    {
        ਵਾਪਸ std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    ਤਾਰਕਿਕ CudaArch::hasBin(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ
    {
        ਵਾਪਸ std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    ਤਾਰਕਿਕ CudaArch::hasEqualOrLessPtx(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ
    {
        ਵਾਪਸ !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    ਤਾਰਕਿਕ CudaArch::hasEqualOrGreaterPtx(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ
    {
        ਵਾਪਸ !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    ਤਾਰਕਿਕ CudaArch::hasEqualOrGreaterBin(ਪੂਰਨ major, ਪੂਰਨ minor) ਅਚਲ
    {
        ਵਾਪਸ !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    ਖਾਲੀ CudaArch::fromStr(ਅਚਲ ਅੱਖਰ* ਸੈੱਟ_as_str, std::vector<ਪੂਰਨ>& arr)
    {
        arr.clear();

        ਅਚਲ size_t len = ਲੜੀ_ਲੰਬਾਈ(ਸੈੱਟ_as_str);

        size_t pos = 0;
        ਜਦੋਂ (pos < len)
        {
            ਜੇ (ਖਾਲੀ_ਥਾਂ_ਹੈ(ਸੈੱਟ_as_str[pos]))
            {
                ++pos;
            }
            ਨਹੀਂਤਾਂ
            {
                ਪੂਰਨ cur_ਮੁੱਲ;
                ਪੂਰਨ chars_read;
                ਪੂਰਨ ਦਲੀਲਾਂ_read = ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(ਸੈੱਟ_as_str + pos, "%d%n", &cur_ਮੁੱਲ, &chars_read);
                CV_Assert( ਦਲੀਲਾਂ_read == 1 );

                arr.push_back(cur_ਮੁੱਲ);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#ਜੇ_ਅੰਤ

ਤਾਰਕਿਕ cv::cuda::ਨਿਸ਼ਾਨਾ_Archs::builtWith(cv::cuda::Feature_ਸੈੱਟ feature_ਸੈੱਟ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(feature_ਸੈੱਟ);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ cudaArch.builtWith(feature_ਸੈੱਟ);
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::ਨਿਸ਼ਾਨਾ_Archs::hasPtx(ਪੂਰਨ major, ਪੂਰਨ minor)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(major);
    CV_ਅਣਵਰਤਿਆ(minor);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ cudaArch.hasPtx(major, minor);
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::ਨਿਸ਼ਾਨਾ_Archs::hasBin(ਪੂਰਨ major, ਪੂਰਨ minor)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(major);
    CV_ਅਣਵਰਤਿਆ(minor);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ cudaArch.hasBin(major, minor);
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::ਨਿਸ਼ਾਨਾ_Archs::hasEqualOrLessPtx(ਪੂਰਨ major, ਪੂਰਨ minor)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(major);
    CV_ਅਣਵਰਤਿਆ(minor);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ cudaArch.hasEqualOrLessPtx(major, minor);
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::ਨਿਸ਼ਾਨਾ_Archs::hasEqualOrGreaterPtx(ਪੂਰਨ major, ਪੂਰਨ minor)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(major);
    CV_ਅਣਵਰਤਿਆ(minor);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ cudaArch.hasEqualOrGreaterPtx(major, minor);
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::ਨਿਸ਼ਾਨਾ_Archs::hasEqualOrGreaterBin(ਪੂਰਨ major, ਪੂਰਨ minor)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(major);
    CV_ਅਣਵਰਤਿਆ(minor);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ cudaArch.hasEqualOrGreaterBin(major, minor);
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼
{
    ਵਰਗ DeviceProps
    {
    ਜਨਤਕ:
        DeviceProps();

        ਅਚਲ cudaDeviceProp* get(ਪੂਰਨ devID) ਅਚਲ;

    ਨਿਜੀ:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        ਪੂਰਨ ਗਿਣਤੀ_3 = get_Cuda_Enabled_Device_ਗਿਣਤੀ();

        ਜੇ (ਗਿਣਤੀ_3 > 0)
        {
            props_.resize(ਗਿਣਤੀ_3);

            ਲਈ (ਪੂਰਨ devID = 0; devID < ਗਿਣਤੀ_3; ++devID)
            {
                cuda_ਸੁਰੱਖਿਅਤ_Call( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ਅਚਲ cudaDeviceProp* DeviceProps::get(ਪੂਰਨ devID) ਅਚਲ
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        ਵਾਪਸ &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        ਸਥਿਰ DeviceProps props;
        ਵਾਪਸ props;
    }
}

#ਜੇ_ਅੰਤ

ਅਚਲ ਅੱਖਰ* cv::cuda::DeviceInfo::name() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->name;
#ਜੇ_ਅੰਤ
}

size_t cv::cuda::DeviceInfo::ਕੁੱਲ_Global_Mem() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->ਕੁੱਲ_Global_Mem;
#ਜੇ_ਅੰਤ
}

size_t cv::cuda::DeviceInfo::shared_Mem_ਪ੍ਰਤੀ_ਖੰਡ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->shared_Mem_ਪ੍ਰਤੀ_ਖੰਡ;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::regs_ਪ੍ਰਤੀ_ਖੰਡ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->regs_ਪ੍ਰਤੀ_ਖੰਡ;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::warp_ਮਾਪ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->warp_ਮਾਪ;
#ਜੇ_ਅੰਤ
}

size_t cv::cuda::DeviceInfo::memPitch() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->memPitch;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::max_ਧਾਗੇ_ਪ੍ਰਤੀ_ਖੰਡ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->max_ਧਾਗੇ_ਪ੍ਰਤੀ_ਖੰਡ;
#ਜੇ_ਅੰਤ
}

Vec3i cv::cuda::DeviceInfo::max_ਧਾਗੇ_Dim() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec3i(deviceProps().get(device_id_)->max_ਧਾਗੇ_Dim);
#ਜੇ_ਅੰਤ
}

Vec3i cv::cuda::DeviceInfo::max_Grid_ਮਾਪ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec3i(deviceProps().get(device_id_)->max_Grid_ਮਾਪ);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::clock_ਦਰ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ clock_ਦਰ;
    cuda_ਸੁਰੱਖਿਅਤ_Call(cudaDeviceGetAttribute(&clock_ਦਰ, cuda_Dev_Attr_Clock_ਦਰ, device_id_));
    ਵਾਪਸ clock_ਦਰ;
#ਜੇ_ਅੰਤ
}

size_t cv::cuda::DeviceInfo::ਕੁੱਲ_Const_Mem() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->ਕੁੱਲ_Const_Mem;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::major_ਸੰਸਕਰਣ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->major;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::minor_ਸੰਸਕਰਣ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->minor;
#ਜੇ_ਅੰਤ
}

size_t cv::cuda::DeviceInfo::textureAlignment() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->textureAlignment;
#ਜੇ_ਅੰਤ
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->texturePitchAlignment;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::multi_Processor_ਗਿਣਤੀ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->multi_Processor_ਗਿਣਤੀ;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ kernelExecTimeoutEnabled;
    cuda_ਸੁਰੱਖਿਅਤ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    ਵਾਪਸ kernelExecTimeoutEnabled != 0;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::DeviceInfo::integrated() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->integrated != 0;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::DeviceInfo::can_Map_Host_ਸਮ੍ਰਿਤੀ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->can_Map_Host_ਸਮ੍ਰਿਤੀ != 0;
#ਜੇ_ਅੰਤ
}

DeviceInfo::Compute_ਬਹੁਲਕ cv::cuda::DeviceInfo::compute_ਬਹੁਲਕ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਸਥਿਰ ਅਚਲ Compute_ਬਹੁਲਕ tbl[] =
    {
        Compute_ਬਹੁਲਕ_Default,
        Compute_ਬਹੁਲਕ_Exclusive,
        Compute_ਬਹੁਲਕ_Prohibited,
        Compute_ਬਹੁਲਕ_Exclusive_Process
    };

    ਪੂਰਨ compute_ਬਹੁਲਕ;
    cuda_ਸੁਰੱਖਿਅਤ_Call(cudaDeviceGetAttribute(&compute_ਬਹੁਲਕ, cuda_Dev_Attr_Compute_ਬਹੁਲਕ, device_id_));
    ਵਾਪਸ tbl[compute_ਬਹੁਲਕ];
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::maxTexture1D() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->maxTexture1D;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::maxTexture1DMipmap() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    #ਜੇ CUDA_ਸੰਸਕਰਣ >= 5000
        ਵਾਪਸ deviceProps().get(device_id_)->maxTexture1DMipmap;
    #ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        ਵਾਪਸ 0;
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::maxTexture1DLinear() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    #ਜੇ CUDA_ਸੰਸਕਰਣ >= 13000
        size_t max_ਚੌੜਾਈ_ਵਿੱਚ_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cuda_ਸੁਰੱਖਿਅਤ_Call(cuda_Device_Get_Texture1_DLinear_Max_ਚੌੜਾਈ(&max_ਚੌੜਾਈ_ਵਿੱਚ_Elements, &fmtDesc, device_id_));
        ਵਾਪਸ max_ਚੌੜਾਈ_ਵਿੱਚ_Elements;
    #ਨਹੀਂਤਾਂ
        ਵਾਪਸ deviceProps().get(device_id_)->maxTexture1DLinear;
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#ਜੇ_ਅੰਤ
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    #ਜੇ CUDA_ਸੰਸਕਰਣ >= 5000
        ਵਾਪਸ Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        ਵਾਪਸ Vec2i();
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#ਜੇ_ਅੰਤ
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#ਜੇ_ਅੰਤ
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::maxTextureCubemap() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->maxTextureCubemap;
#ਜੇ_ਅੰਤ
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#ਜੇ_ਅੰਤ
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#ਜੇ_ਅੰਤ
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::maxSurface1D() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->maxSurface1D;
#ਜੇ_ਅੰਤ
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#ਜੇ_ਅੰਤ
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#ਜੇ_ਅੰਤ
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#ਜੇ_ਅੰਤ
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::maxSurfaceCubemap() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->maxSurfaceCubemap;
#ਜੇ_ਅੰਤ
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#ਜੇ_ਅੰਤ
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->surfaceAlignment;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::DeviceInfo::concurrentKernels() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->concurrentKernels != 0;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::DeviceInfo::ECCEnabled() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->ECCEnabled != 0;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::pciBusID() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->pciBusID;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::pciDeviceID() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->pciDeviceID;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::pciDomainID() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->pciDomainID;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::DeviceInfo::tccDriver() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->tccDriver != 0;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::async_Engine_ਗਿਣਤੀ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->async_Engine_ਗਿਣਤੀ;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::DeviceInfo::unifiedAddressing() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->unifiedAddressing != 0;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::ਸਮ੍ਰਿਤੀ_Clock_ਦਰ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ ਸਮ੍ਰਿਤੀ_Clock_ਦਰ;
    cuda_ਸੁਰੱਖਿਅਤ_Call(cudaDeviceGetAttribute(&ਸਮ੍ਰਿਤੀ_Clock_ਦਰ, cuda_Dev_Attr_ਸਮ੍ਰਿਤੀ_Clock_ਦਰ, device_id_));
    ਵਾਪਸ ਸਮ੍ਰਿਤੀ_Clock_ਦਰ;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::ਸਮ੍ਰਿਤੀ_Bus_ਚੌੜਾਈ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->ਸਮ੍ਰਿਤੀ_Bus_ਚੌੜਾਈ;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::l2_Cache_ਮਾਪ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->l2_Cache_ਮਾਪ;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::cuda::DeviceInfo::max_ਧਾਗੇ_ਪ੍ਰਤੀ_Multi_Processor() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ deviceProps().get(device_id_)->max_ਧਾਗੇ_ਪ੍ਰਤੀ_Multi_Processor;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::DeviceInfo::query_ਸਮ੍ਰਿਤੀ(size_t& ਕੁੱਲ_ਸਮ੍ਰਿਤੀ, size_t& free_ਸਮ੍ਰਿਤੀ) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(ਕੁੱਲ_ਸਮ੍ਰਿਤੀ);
    CV_ਅਣਵਰਤਿਆ(free_ਸਮ੍ਰਿਤੀ);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ prevDeviceID = getDevice();
    ਜੇ (prevDeviceID != device_id_)
        ਸੈੱਟ_Device(device_id_);

    cuda_ਸੁਰੱਖਿਅਤ_Call( cudaMemGetInfo(&free_ਸਮ੍ਰਿਤੀ, &ਕੁੱਲ_ਸਮ੍ਰਿਤੀ) );

    ਜੇ (prevDeviceID != device_id_)
        ਸੈੱਟ_Device(prevDeviceID);
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::DeviceInfo::isCompatible() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    // Check PTX compatibility
    ਜੇ (ਨਿਸ਼ਾਨਾ_Archs::hasEqualOrLessPtx(major_ਸੰਸਕਰਣ(), minor_ਸੰਸਕਰਣ()))
        ਵਾਪਸ ਸੱਚ;

    // Check BIN compatibility
    ਲਈ (ਪੂਰਨ i = minor_ਸੰਸਕਰਣ(); i >= 0; --i)
        ਜੇ (ਨਿਸ਼ਾਨਾ_Archs::hasBin(major_ਸੰਸਕਰਣ(), i))
            ਵਾਪਸ ਸੱਚ;

    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// print info

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼
{
    ਪੂਰਨ convertSMVer2Cores(ਪੂਰਨ major, ਪੂਰਨ minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
            ਪੂਰਨ SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            ਪੂਰਨ Cores;
        } SMtoCores;

        SMtoCores gpu_Arch_Cores_ਪ੍ਰਤੀ_SM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        ਪੂਰਨ ਸੂਚਕਾਂਕ = 0;
        ਜਦੋਂ (gpu_Arch_Cores_ਪ੍ਰਤੀ_SM[ਸੂਚਕਾਂਕ].SM != -1)
        {
            ਜੇ (gpu_Arch_Cores_ਪ੍ਰਤੀ_SM[ਸੂਚਕਾਂਕ].SM == ((major << 4) + minor) )
                ਵਾਪਸ gpu_Arch_Cores_ਪ੍ਰਤੀ_SM[ਸੂਚਕਾਂਕ].Cores;
            ਸੂਚਕਾਂਕ++;
        }

        ਵਾਪਸ -1;
    }
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ cv::cuda::printCudaDeviceInfo(ਪੂਰਨ device)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(device);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ ਗਿਣਤੀ_3 = get_Cuda_Enabled_Device_ਗਿਣਤੀ();
    ਤਾਰਕਿਕ valid = (device >= 0) && (device < ਗਿਣਤੀ_3);

    ਪੂਰਨ beg = valid ? device   : 0;
    ਪੂਰਨ end = valid ? device+1 : ਗਿਣਤੀ_3;

    ਲਿਖੋ("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    ਲਿਖੋ("Device count: %d\n", ਗਿਣਤੀ_3);

    ਪੂਰਨ driver_ਸੰਸਕਰਣ = 0, runtime_ਸੰਸਕਰਣ = 0;
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Driver_Get_ਸੰਸਕਰਣ(&driver_ਸੰਸਕਰਣ) );
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Runtime_Get_ਸੰਸਕਰਣ(&runtime_ਸੰਸਕਰਣ) );

    ਅਚਲ ਅੱਖਰ *compute_ਬਹੁਲਕ[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        ਨਿਰਾ
    };

    ਲਈ(ਪੂਰਨ dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_ਸੁਰੱਖਿਅਤ_Call( cudaGetDeviceProperties(&prop, dev) );

        ਲਿਖੋ("\nDevice %d: \"%s\"\n", dev, prop.name);
        ਲਿਖੋ("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_ਸੰਸਕਰਣ/1000, driver_ਸੰਸਕਰਣ%100, runtime_ਸੰਸਕਰਣ/1000, runtime_ਸੰਸਕਰਣ%100);
        ਲਿਖੋ("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        ਲਿਖੋ("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (ਦਸ਼ਮਲਵ)prop.ਕੁੱਲ_Global_Mem/1048576.0f, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) prop.ਕੁੱਲ_Global_Mem);

        ਪੂਰਨ cores = convertSMVer2Cores(prop.major, prop.minor);
        ਜੇ (cores > 0)
            ਲਿਖੋ("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_ਗਿਣਤੀ, cores, cores * prop.multi_Processor_ਗਿਣਤੀ);

        ਪੂਰਨ clock_ਦਰ;
        cuda_ਸੁਰੱਖਿਅਤ_Call(cudaDeviceGetAttribute(&clock_ਦਰ, cuda_Dev_Attr_Clock_ਦਰ, dev));
        ਲਿਖੋ("  GPU Clock Speed:                               %.2f GHz\n", clock_ਦਰ * 1e-6f);

        ਲਿਖੋ("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        ਲਿਖੋ("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        ਲਿਖੋ("  Total amount of constant memory:               %u bytes\n", (ਪੂਰਨ)prop.ਕੁੱਲ_Const_Mem);
        ਲਿਖੋ("  Total amount of shared memory per block:       %u bytes\n", (ਪੂਰਨ)prop.shared_Mem_ਪ੍ਰਤੀ_ਖੰਡ);
        ਲਿਖੋ("  Total number of registers available per block: %d\n", prop.regs_ਪ੍ਰਤੀ_ਖੰਡ);
        ਲਿਖੋ("  Warp size:                                     %d\n", prop.warp_ਮਾਪ);
        ਲਿਖੋ("  Maximum number of threads per block:           %d\n", prop.max_ਧਾਗੇ_ਪ੍ਰਤੀ_ਖੰਡ);
        ਲਿਖੋ("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_ਧਾਗੇ_Dim[0], prop.max_ਧਾਗੇ_Dim[1], prop.max_ਧਾਗੇ_Dim[2]);
        ਲਿਖੋ("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_ਮਾਪ[0], prop.max_Grid_ਮਾਪ[1],  prop.max_Grid_ਮਾਪ[2]);
        ਲਿਖੋ("  Maximum memory pitch:                          %u bytes\n", (ਪੂਰਨ)prop.memPitch);
        ਲਿਖੋ("  Texture alignment:                             %u bytes\n", (ਪੂਰਨ)prop.textureAlignment);

        ਲਿਖੋ("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_ਗਿਣਤੀ ? "Yes" : "No"), prop.async_Engine_ਗਿਣਤੀ);
        ਪੂਰਨ kernelExecTimeoutEnabled;
        cuda_ਸੁਰੱਖਿਅਤ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        ਲਿਖੋ("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        ਲਿਖੋ("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        ਲਿਖੋ("  Support host page-locked memory mapping:       %s\n", prop.can_Map_Host_ਸਮ੍ਰਿਤੀ ? "Yes" : "No");

        ਲਿਖੋ("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        ਲਿਖੋ("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        ਲਿਖੋ("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        ਲਿਖੋ("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        ਲਿਖੋ("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        ਲਿਖੋ("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        ਪੂਰਨ prop_Compute_ਬਹੁਲਕ;
        cuda_ਸੁਰੱਖਿਅਤ_Call(cudaDeviceGetAttribute(&prop_Compute_ਬਹੁਲਕ, cuda_Dev_Attr_Compute_ਬਹੁਲਕ, dev));
        ਲਿਖੋ("  Compute Mode:\n");
        ਲਿਖੋ("      %s \n", compute_ਬਹੁਲਕ[prop_Compute_ਬਹੁਲਕ]);
    }

    ਲਿਖੋ("\n");
    ਲਿਖੋ("deviceQuery, CUDA Driver = CUDART");
    ਲਿਖੋ(", CUDA Driver Version  = %d.%d", driver_ਸੰਸਕਰਣ / 1000, driver_ਸੰਸਕਰਣ % 100);
    ਲਿਖੋ(", CUDA Runtime Version = %d.%d", runtime_ਸੰਸਕਰਣ/1000, runtime_ਸੰਸਕਰਣ%100);
    ਲਿਖੋ(", NumDevs = %d\n\n", ਗਿਣਤੀ_3);

    fflush(stdout);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::printShortCudaDeviceInfo(ਪੂਰਨ device)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(device);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ ਗਿਣਤੀ_3 = get_Cuda_Enabled_Device_ਗਿਣਤੀ();
    ਤਾਰਕਿਕ valid = (device >= 0) && (device < ਗਿਣਤੀ_3);

    ਪੂਰਨ beg = valid ? device   : 0;
    ਪੂਰਨ end = valid ? device+1 : ਗਿਣਤੀ_3;

    ਪੂਰਨ driver_ਸੰਸਕਰਣ = 0, runtime_ਸੰਸਕਰਣ = 0;
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Driver_Get_ਸੰਸਕਰਣ(&driver_ਸੰਸਕਰਣ) );
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Runtime_Get_ਸੰਸਕਰਣ(&runtime_ਸੰਸਕਰਣ) );

    ਲਈ(ਪੂਰਨ dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_ਸੁਰੱਖਿਅਤ_Call( cudaGetDeviceProperties(&prop, dev) );

        ਅਚਲ ਅੱਖਰ *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        ਲਿਖੋ("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (ਦਸ਼ਮਲਵ)prop.ਕੁੱਲ_Global_Mem/1048576.0f);
        ਲਿਖੋ(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        ਪੂਰਨ cores = convertSMVer2Cores(prop.major, prop.minor);
        ਜੇ (cores > 0)
            ਲਿਖੋ(", %d cores", cores * prop.multi_Processor_ਗਿਣਤੀ);

        ਲਿਖੋ(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_ਸੰਸਕਰਣ/1000, driver_ਸੰਸਕਰਣ%100, runtime_ਸੰਸਕਰਣ/1000, runtime_ਸੰਸਕਰਣ%100);
    }

    fflush(stdout);
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼
{
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ error_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ)  { ਇੰਦਰਾਜ, #entry }

    ਬਣਤਰ Error_ਇੰਦਰਾਜ
    {
        ਪੂਰਨ ਕੋਡ;
        ਅਚਲ ਅੱਖਰ* str;
    };

    ਬਣਤਰ Error_ਇੰਦਰਾਜ_Comparer
    {
        ਪੂਰਨ ਕੋਡ;
        Error_ਇੰਦਰਾਜ_Comparer(ਪੂਰਨ ਕੋਡ_2) : ਕੋਡ(ਕੋਡ_2) {}
        ਤਾਰਕਿਕ ਸੰਚਾਲਕ()(ਅਚਲ Error_ਇੰਦਰਾਜ& e) ਅਚਲ { ਵਾਪਸ e.ਕੋਡ == ਕੋਡ; }
    };

    ਅਚਲ Error_ਇੰਦਰਾਜ npp_errors [] =
    {
    #ਜੇ defined (_MSC_VER)
        error_ਇੰਦਰਾਜ( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #ਜੇ_ਅੰਤ

    #ਜੇ NPP_ਸੰਸਕਰਣ < 5500
        error_ਇੰਦਰਾਜ( NPP_BAD_ARG_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_COEFF_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_RECT_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_QUAD_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_MEMFREE_ERR ),
        error_ਇੰਦਰਾਜ( NPP_MEMSET_ERR ),
        error_ਇੰਦਰਾਜ( NPP_MEM_ALLOC_ERR ),
        error_ਇੰਦਰਾਜ( NPP_HISTO_ਸੰਖਿਆ_ਦਾ_ਪੱਧਰ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_MIRROR_FLIP_ERR ),
        error_ਇੰਦਰਾਜ( NPP_INVALID_ਨਿਵੇਸ਼ ),
        error_ਇੰਦਰਾਜ( NPP_ਸੂਚਕ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_WARNING ),
        error_ਇੰਦਰਾਜ( NPP_ODD_ROI_WARNING ),
    #ਨਹੀਂਤਾਂ
        error_ਇੰਦਰਾਜ( NPP_INVALID_HOST_ਸੂਚਕ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_INVALID_DEVICE_ਸੂਚਕ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ZC_ਬਹੁਲਕ_NOT_SUPPORTED_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_MEMFREE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_MEMSET_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_QUALITY_ਸੂਚਕਾਂਕ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_HISTOGRAM_ਸੰਖਿਆ_ਦਾ_ਪੱਧਰ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_CHANNEL_ਕ੍ਰਮ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ZERO_ਨਕਾਬ_ਮੁੱਲ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_QUADRANGLE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_RECTANGLE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_COEFFICIENT_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਸੰਖਿਆ_ਦਾ_CHANNELS_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_COI_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਭਾਜਕ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_CHANNEL_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_STRIDE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ANCHOR_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਨਕਾਬ_ਮਾਪ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_MIRROR_FLIP_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_MOMENT_00_ZERO_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਹੱਦ_NEGATIVE_LEVEL_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਹੱਦ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_CONTEXT_ਮੇਲ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_FFT_FLAG_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_FFT_ਕ੍ਰਮ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_SCALE_RANGE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਅੰਕੜੇ_TYPE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਬਾਹਰਲਾ_OFF_RANGE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਸਮ੍ਰਿਤੀ_ਵੰਡ_ERR ),
        error_ਇੰਦਰਾਜ( NPP_RANGE_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_BAD_ਦਲੀਲ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_NO_ਸਮ੍ਰਿਤੀ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ERROR_RESERVED ),
        error_ਇੰਦਰਾਜ( NPP_NO_OPERATION_WARNING ),
        error_ਇੰਦਰਾਜ( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_ਇੰਦਰਾਜ( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #ਜੇ_ਅੰਤ

        error_ਇੰਦਰਾਜ( NPP_NOT_SUPPORTED_ਬਹੁਲਕ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਗੇੜਾ_ਬਹੁਲਕ_NOT_SUPPORTED_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_LUT_ਸੰਖਿਆ_ਦਾ_ਪੱਧਰ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_TEXTURE_BIND_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_NOT_EVEN_ਕਦਮ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_INTERPOLATION_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_RESIZE_ਗੁਣਕ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_HAAR_CLASSIFIER_PIXEL_ਮੇਲ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_MEMCPY_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ALIGNMENT_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਕਦਮ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ਮਾਪ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_NULL_ਸੂਚਕ_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_NOT_IMPLEMENTED_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_NO_ERROR ),
        error_ਇੰਦਰਾਜ( NPP_SUCCESS ),
        error_ਇੰਦਰਾਜ( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_ਇੰਦਰਾਜ( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_ਇੰਦਰਾਜ( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_ਇੰਦਰਾਜ( NPP_DOUBLE_ਮਾਪ_WARNING )
    };

    ਅਚਲ size_t npp_error_ਅੰਕ = ਆਕਾਰ(npp_errors) / ਆਕਾਰ(npp_errors[0]);

    ਅਚਲ Error_ਇੰਦਰਾਜ cu_errors [] =
    {
        error_ਇੰਦਰਾਜ( CUDA_SUCCESS                              ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_INVALID_ਮੁੱਲ                  ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ                  ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_NOT_INITIALIZED                ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_DEINITIALIZED                  ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_PROFILER_DISABLED              ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_NO_DEVICE                      ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_INVALID_DEVICE                 ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_INVALID_IMAGE                  ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_INVALID_CONTEXT                ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_MAP_FAILED                     ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_UNMAP_FAILED                   ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_NO_ਦੁਆਧਾਰੀ_FOR_GPU              ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_NOT_MAPPED                     ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_NOT_MAPPED_AS_ਸੂਚਕ          ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_UNSUPPORTED_ਹੱਦਬੰਦੀ              ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_CONTEXT_ALREADY_ਵਿੱਚ_USE         ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_INVALID_ਸਰੋਤ                 ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_FILE_NOT_ਮਿਲਿਆ                 ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_SHARED_OBJECT_ਚਿੰਨ੍ਹ_NOT_ਮਿਲਿਆ ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_INVALID_HANDLE                 ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_NOT_ਮਿਲਿਆ                      ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_NOT_READY                      ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_LAUNCH_ਬਾਹਰਲਾ_ਦਾ_RESOURCES        ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_PRIMARY_CONTEXT_ਸਰਗਰਮ         ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_ASSERT                         ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_HOST_ਸਮ੍ਰਿਤੀ_ALREADY_REGISTERED ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_HOST_ਸਮ੍ਰਿਤੀ_NOT_REGISTERED     ),
        error_ਇੰਦਰਾਜ( CUDA_ERROR_UNKNOWN                        )
    };

    ਅਚਲ size_t cu_errors_ਅੰਕ = ਆਕਾਰ(cu_errors) / ਆਕਾਰ(cu_errors[0]);

    cv::String getErrorString(ਪੂਰਨ ਕੋਡ, ਅਚਲ Error_ਇੰਦਰਾਜ* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_ਇੰਦਰਾਜ_Comparer(ਕੋਡ)) - errors;

        ਅਚਲ ਅੱਖਰ* ਸੁਨੇਹਾ = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", ਸੁਨੇਹਾ, ਕੋਡ);

        ਵਾਪਸ str;
    }
}

#ਜੇ_ਅੰਤ

String cv::cuda::get_Npp_Error_ਸੁਨੇਹਾ(ਪੂਰਨ ਕੋਡ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(ਕੋਡ);
    ਵਾਪਸ String();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ getErrorString(ਕੋਡ, npp_errors, npp_error_ਅੰਕ);
#ਜੇ_ਅੰਤ
}

String cv::cuda::get_Cuda_Driver_Api_Error_ਸੁਨੇਹਾ(ਪੂਰਨ ਕੋਡ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(ਕੋਡ);
    ਵਾਪਸ String();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ getErrorString(ਕੋਡ, cu_errors, cu_errors_ਅੰਕ);
#ਜੇ_ਅੰਤ
}
