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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

ಬಳಸಿ ನಾಮಾಕಾಶ cv;
ಬಳಸಿ ನಾಮಾಕಾಶ cv::cuda;

ಪೂರ್ಣಾಂಕ cv::cuda::get_Cuda_Enabled_Device_ಎಣಿಕೆ()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3;
    cudaError_t error = cuda_Get_Device_ಎಣಿಕೆ(&ಎಣಿಕೆ_3);

    ಆದರೆ (error == cudaErrorInsufficientDriver)
        ಹಿಂತಿರುಗಿ -1;

    ಆದರೆ (error == cudaErrorNoDevice)
        ಹಿಂತಿರುಗಿ 0;

    cuda_ಸುರಕ್ಷಿತ_Call( error );
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_3;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::ಗಣ_Device(ಪೂರ್ಣಾಂಕ device)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(device);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_ಗಣ_Device(device) );
    cuda_ಸುರಕ್ಷಿತ_Call( cudaFree(0) );
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::getDevice()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ device;
    cuda_ಸುರಕ್ಷಿತ_Call( cudaGetDevice(&device) );
    ಹಿಂತಿರುಗಿ device;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::resetDevice()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    cuda_ಸುರಕ್ಷಿತ_Call( cudaDeviceReset() );
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::deviceSupports(Feature_ಗಣ feature_ಗಣ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(feature_ಗಣ);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cache_ಗಾತ್ರ = static_cast<ಪೂರ್ಣಾಂಕ>(ಗಾತ್ರ(versions) / ಗಾತ್ರ(versions[0]));

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ devId = getDevice();

    ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ;

    ಆದರೆ (devId < cache_ಗಾತ್ರ && versions[devId] >= 0)
    {
        ಆವೃತ್ತಿ = versions[devId];
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        DeviceInfo dev(devId);
        ಆವೃತ್ತಿ = dev.major_ಆವೃತ್ತಿ() * 10 + dev.minor_ಆವೃತ್ತಿ();
        ಆದರೆ (devId < cache_ಗಾತ್ರ)
            versions[devId] = ಆವೃತ್ತಿ;
    }

    ಹಿಂತಿರುಗಿ ಗುರಿ_Archs::builtWith(feature_ಗಣ) && (ಆವೃತ್ತಿ >= feature_ಗಣ);
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ
{
    ವರ್ಗ CudaArch
    {
    ಸಾರ್ವಜನಿಕ:
        CudaArch();

        ತಾರ್ಕಿಕ builtWith(Feature_ಗಣ feature_ಗಣ) ಸ್ಥಿರಾಂಕ;
        ತಾರ್ಕಿಕ hasPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ;
        ತಾರ್ಕಿಕ hasBin(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ;
        ತಾರ್ಕಿಕ hasEqualOrLessPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ;
        ತಾರ್ಕಿಕ hasEqualOrGreaterPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ;
        ತಾರ್ಕಿಕ hasEqualOrGreaterBin(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ;

    ಖಾಸಗಿ:
        ಸ್ಥಿರ ಶೂನ್ಯ fromStr(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಗಣ_as_str, std::vector<ಪೂರ್ಣಾಂಕ>& arr);

        std::vector<ಪೂರ್ಣಾಂಕ> bin;
        std::vector<ಪೂರ್ಣಾಂಕ> ptx;
        std::vector<ಪೂರ್ಣಾಂಕ> features;
    };

    ಸ್ಥಿರಾಂಕ CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    ತಾರ್ಕಿಕ CudaArch::builtWith(Feature_ಗಣ feature_ಗಣ) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ !features.empty() && (features.back() >= feature_ಗಣ);
    }

    ತಾರ್ಕಿಕ CudaArch::hasPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    ತಾರ್ಕಿಕ CudaArch::hasBin(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    ತಾರ್ಕಿಕ CudaArch::hasEqualOrLessPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    ತಾರ್ಕಿಕ CudaArch::hasEqualOrGreaterPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    ತಾರ್ಕಿಕ CudaArch::hasEqualOrGreaterBin(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor) ಸ್ಥಿರಾಂಕ
    {
        ಹಿಂತಿರುಗಿ !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    ಶೂನ್ಯ CudaArch::fromStr(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಗಣ_as_str, std::vector<ಪೂರ್ಣಾಂಕ>& arr)
    {
        arr.clear();

        ಸ್ಥಿರಾಂಕ size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಗಣ_as_str);

        size_t pos = 0;
        ಆಗ (pos < len)
        {
            ಆದರೆ (ಖಾಲಿಯೇ(ಗಣ_as_str[pos]))
            {
                ++pos;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಪೂರ್ಣಾಂಕ cur_ಮೌಲ್ಯ;
                ಪೂರ್ಣಾಂಕ chars_read;
                ಪೂರ್ಣಾಂಕ ವಾದಗಳು_read = ಅಕ್ಷರಸರಣಿ_ಓದು(ಗಣ_as_str + pos, "%d%n", &cur_ಮೌಲ್ಯ, &chars_read);
                CV_Assert( ವಾದಗಳು_read == 1 );

                arr.push_back(cur_ಮೌಲ್ಯ);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#ಆದರೆ_ಅಂತ್ಯ

ತಾರ್ಕಿಕ cv::cuda::ಗುರಿ_Archs::builtWith(cv::cuda::Feature_ಗಣ feature_ಗಣ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(feature_ಗಣ);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ cudaArch.builtWith(feature_ಗಣ);
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::ಗುರಿ_Archs::hasPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(major);
    CV_ಬಳಸದ(minor);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ cudaArch.hasPtx(major, minor);
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::ಗುರಿ_Archs::hasBin(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(major);
    CV_ಬಳಸದ(minor);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ cudaArch.hasBin(major, minor);
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::ಗುರಿ_Archs::hasEqualOrLessPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(major);
    CV_ಬಳಸದ(minor);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ cudaArch.hasEqualOrLessPtx(major, minor);
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::ಗುರಿ_Archs::hasEqualOrGreaterPtx(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(major);
    CV_ಬಳಸದ(minor);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ cudaArch.hasEqualOrGreaterPtx(major, minor);
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::ಗುರಿ_Archs::hasEqualOrGreaterBin(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(major);
    CV_ಬಳಸದ(minor);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ cudaArch.hasEqualOrGreaterBin(major, minor);
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ
{
    ವರ್ಗ DeviceProps
    {
    ಸಾರ್ವಜನಿಕ:
        DeviceProps();

        ಸ್ಥಿರಾಂಕ cudaDeviceProp* get(ಪೂರ್ಣಾಂಕ devID) ಸ್ಥಿರಾಂಕ;

    ಖಾಸಗಿ:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = get_Cuda_Enabled_Device_ಎಣಿಕೆ();

        ಆದರೆ (ಎಣಿಕೆ_3 > 0)
        {
            props_.resize(ಎಣಿಕೆ_3);

            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ devID = 0; devID < ಎಣಿಕೆ_3; ++devID)
            {
                cuda_ಸುರಕ್ಷಿತ_Call( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ಸ್ಥಿರಾಂಕ cudaDeviceProp* DeviceProps::get(ಪೂರ್ಣಾಂಕ devID) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        ಹಿಂತಿರುಗಿ &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        ಸ್ಥಿರ DeviceProps props;
        ಹಿಂತಿರುಗಿ props;
    }
}

#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* cv::cuda::DeviceInfo::name() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->name;
#ಆದರೆ_ಅಂತ್ಯ
}

size_t cv::cuda::DeviceInfo::ಒಟ್ಟು_Global_Mem() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->ಒಟ್ಟು_Global_Mem;
#ಆದರೆ_ಅಂತ್ಯ
}

size_t cv::cuda::DeviceInfo::shared_Mem_ಪ್ರತಿ_ಖಂಡ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->shared_Mem_ಪ್ರತಿ_ಖಂಡ;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::regs_ಪ್ರತಿ_ಖಂಡ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->regs_ಪ್ರತಿ_ಖಂಡ;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::warp_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->warp_ಗಾತ್ರ;
#ಆದರೆ_ಅಂತ್ಯ
}

size_t cv::cuda::DeviceInfo::memPitch() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->memPitch;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::max_ಎಳೆಗಳು_ಪ್ರತಿ_ಖಂಡ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->max_ಎಳೆಗಳು_ಪ್ರತಿ_ಖಂಡ;
#ಆದರೆ_ಅಂತ್ಯ
}

Vec3i cv::cuda::DeviceInfo::max_ಎಳೆಗಳು_Dim() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec3i(deviceProps().get(device_id_)->max_ಎಳೆಗಳು_Dim);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec3i cv::cuda::DeviceInfo::max_Grid_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec3i(deviceProps().get(device_id_)->max_Grid_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::clock_ದರ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ clock_ದರ;
    cuda_ಸುರಕ್ಷಿತ_Call(cudaDeviceGetAttribute(&clock_ದರ, cuda_Dev_Attr_Clock_ದರ, device_id_));
    ಹಿಂತಿರುಗಿ clock_ದರ;
#ಆದರೆ_ಅಂತ್ಯ
}

size_t cv::cuda::DeviceInfo::ಒಟ್ಟು_Const_Mem() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->ಒಟ್ಟು_Const_Mem;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::major_ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->major;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::minor_ಆವೃತ್ತಿ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->minor;
#ಆದರೆ_ಅಂತ್ಯ
}

size_t cv::cuda::DeviceInfo::textureAlignment() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->textureAlignment;
#ಆದರೆ_ಅಂತ್ಯ
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->texturePitchAlignment;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::multi_Processor_ಎಣಿಕೆ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->multi_Processor_ಎಣಿಕೆ;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ kernelExecTimeoutEnabled;
    cuda_ಸುರಕ್ಷಿತ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    ಹಿಂತಿರುಗಿ kernelExecTimeoutEnabled != 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::DeviceInfo::integrated() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->integrated != 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::DeviceInfo::can_Map_Host_ಸ್ಮರಣೆ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->can_Map_Host_ಸ್ಮರಣೆ != 0;
#ಆದರೆ_ಅಂತ್ಯ
}

DeviceInfo::Compute_ಬಹುಲಕ cv::cuda::DeviceInfo::compute_ಬಹುಲಕ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Compute_ಬಹುಲಕ tbl[] =
    {
        Compute_ಬಹುಲಕ_Default,
        Compute_ಬಹುಲಕ_Exclusive,
        Compute_ಬಹುಲಕ_Prohibited,
        Compute_ಬಹುಲಕ_Exclusive_Process
    };

    ಪೂರ್ಣಾಂಕ compute_ಬಹುಲಕ;
    cuda_ಸುರಕ್ಷಿತ_Call(cudaDeviceGetAttribute(&compute_ಬಹುಲಕ, cuda_Dev_Attr_Compute_ಬಹುಲಕ, device_id_));
    ಹಿಂತಿರುಗಿ tbl[compute_ಬಹುಲಕ];
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::maxTexture1D() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->maxTexture1D;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::maxTexture1DMipmap() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    #ಆದರೆ CUDA_ಆವೃತ್ತಿ >= 5000
        ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->maxTexture1DMipmap;
    #ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        ಹಿಂತಿರುಗಿ 0;
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::maxTexture1DLinear() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    #ಆದರೆ CUDA_ಆವೃತ್ತಿ >= 13000
        size_t max_ಅಗಲ_ಒಳ_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cuda_ಸುರಕ್ಷಿತ_Call(cuda_Device_Get_Texture1_DLinear_Max_ಅಗಲ(&max_ಅಗಲ_ಒಳ_Elements, &fmtDesc, device_id_));
        ಹಿಂತಿರುಗಿ max_ಅಗಲ_ಒಳ_Elements;
    #ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->maxTexture1DLinear;
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    #ಆದರೆ CUDA_ಆವೃತ್ತಿ >= 5000
        ಹಿಂತಿರುಗಿ Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        ಹಿಂತಿರುಗಿ Vec2i();
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::maxTextureCubemap() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->maxTextureCubemap;
#ಆದರೆ_ಅಂತ್ಯ
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::maxSurface1D() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->maxSurface1D;
#ಆದರೆ_ಅಂತ್ಯ
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#ಆದರೆ_ಅಂತ್ಯ
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::maxSurfaceCubemap() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->maxSurfaceCubemap;
#ಆದರೆ_ಅಂತ್ಯ
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#ಆದರೆ_ಅಂತ್ಯ
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->surfaceAlignment;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::DeviceInfo::concurrentKernels() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->concurrentKernels != 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::DeviceInfo::ECCEnabled() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->ECCEnabled != 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::pciBusID() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->pciBusID;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::pciDeviceID() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->pciDeviceID;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::pciDomainID() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->pciDomainID;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::DeviceInfo::tccDriver() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->tccDriver != 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::async_Engine_ಎಣಿಕೆ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->async_Engine_ಎಣಿಕೆ;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::DeviceInfo::unifiedAddressing() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->unifiedAddressing != 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::ಸ್ಮರಣೆ_Clock_ದರ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ ಸ್ಮರಣೆ_Clock_ದರ;
    cuda_ಸುರಕ್ಷಿತ_Call(cudaDeviceGetAttribute(&ಸ್ಮರಣೆ_Clock_ದರ, cuda_Dev_Attr_ಸ್ಮರಣೆ_Clock_ದರ, device_id_));
    ಹಿಂತಿರುಗಿ ಸ್ಮರಣೆ_Clock_ದರ;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::ಸ್ಮರಣೆ_Bus_ಅಗಲ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->ಸ್ಮರಣೆ_Bus_ಅಗಲ;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::l2_Cache_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->l2_Cache_ಗಾತ್ರ;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::cuda::DeviceInfo::max_ಎಳೆಗಳು_ಪ್ರತಿ_Multi_Processor() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ deviceProps().get(device_id_)->max_ಎಳೆಗಳು_ಪ್ರತಿ_Multi_Processor;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::DeviceInfo::query_ಸ್ಮರಣೆ(size_t& ಒಟ್ಟು_ಸ್ಮರಣೆ, size_t& free_ಸ್ಮರಣೆ) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(ಒಟ್ಟು_ಸ್ಮರಣೆ);
    CV_ಬಳಸದ(free_ಸ್ಮರಣೆ);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ prevDeviceID = getDevice();
    ಆದರೆ (prevDeviceID != device_id_)
        ಗಣ_Device(device_id_);

    cuda_ಸುರಕ್ಷಿತ_Call( cudaMemGetInfo(&free_ಸ್ಮರಣೆ, &ಒಟ್ಟು_ಸ್ಮರಣೆ) );

    ಆದರೆ (prevDeviceID != device_id_)
        ಗಣ_Device(prevDeviceID);
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::DeviceInfo::isCompatible() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    // Check PTX compatibility
    ಆದರೆ (ಗುರಿ_Archs::hasEqualOrLessPtx(major_ಆವೃತ್ತಿ(), minor_ಆವೃತ್ತಿ()))
        ಹಿಂತಿರುಗಿ ನಿಜ;

    // Check BIN compatibility
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = minor_ಆವೃತ್ತಿ(); i >= 0; --i)
        ಆದರೆ (ಗುರಿ_Archs::hasBin(major_ಆವೃತ್ತಿ(), i))
            ಹಿಂತಿರುಗಿ ನಿಜ;

    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// print info

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ
{
    ಪೂರ್ಣಾಂಕ convertSMVer2Cores(ಪೂರ್ಣಾಂಕ major, ಪೂರ್ಣಾಂಕ minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
            ಪೂರ್ಣಾಂಕ SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            ಪೂರ್ಣಾಂಕ Cores;
        } SMtoCores;

        SMtoCores gpu_Arch_Cores_ಪ್ರತಿ_SM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = 0;
        ಆಗ (gpu_Arch_Cores_ಪ್ರತಿ_SM[ಸೂಚ್ಯಂಕ].SM != -1)
        {
            ಆದರೆ (gpu_Arch_Cores_ಪ್ರತಿ_SM[ಸೂಚ್ಯಂಕ].SM == ((major << 4) + minor) )
                ಹಿಂತಿರುಗಿ gpu_Arch_Cores_ಪ್ರತಿ_SM[ಸೂಚ್ಯಂಕ].Cores;
            ಸೂಚ್ಯಂಕ++;
        }

        ಹಿಂತಿರುಗಿ -1;
    }
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cv::cuda::printCudaDeviceInfo(ಪೂರ್ಣಾಂಕ device)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(device);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = get_Cuda_Enabled_Device_ಎಣಿಕೆ();
    ತಾರ್ಕಿಕ valid = (device >= 0) && (device < ಎಣಿಕೆ_3);

    ಪೂರ್ಣಾಂಕ beg = valid ? device   : 0;
    ಪೂರ್ಣಾಂಕ end = valid ? device+1 : ಎಣಿಕೆ_3;

    ಬರೆ("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    ಬರೆ("Device count: %d\n", ಎಣಿಕೆ_3);

    ಪೂರ್ಣಾಂಕ driver_ಆವೃತ್ತಿ = 0, runtime_ಆವೃತ್ತಿ = 0;
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_Driver_Get_ಆವೃತ್ತಿ(&driver_ಆವೃತ್ತಿ) );
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_Runtime_Get_ಆವೃತ್ತಿ(&runtime_ಆವೃತ್ತಿ) );

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *compute_ಬಹುಲಕ[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        ಖಾಲಿ
    };

    ಗಾಗಿ(ಪೂರ್ಣಾಂಕ dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_ಸುರಕ್ಷಿತ_Call( cudaGetDeviceProperties(&prop, dev) );

        ಬರೆ("\nDevice %d: \"%s\"\n", dev, prop.name);
        ಬರೆ("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_ಆವೃತ್ತಿ/1000, driver_ಆವೃತ್ತಿ%100, runtime_ಆವೃತ್ತಿ/1000, runtime_ಆವೃತ್ತಿ%100);
        ಬರೆ("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        ಬರೆ("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (ದಶಮಾಂಶ)prop.ಒಟ್ಟು_Global_Mem/1048576.0f, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) prop.ಒಟ್ಟು_Global_Mem);

        ಪೂರ್ಣಾಂಕ cores = convertSMVer2Cores(prop.major, prop.minor);
        ಆದರೆ (cores > 0)
            ಬರೆ("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_ಎಣಿಕೆ, cores, cores * prop.multi_Processor_ಎಣಿಕೆ);

        ಪೂರ್ಣಾಂಕ clock_ದರ;
        cuda_ಸುರಕ್ಷಿತ_Call(cudaDeviceGetAttribute(&clock_ದರ, cuda_Dev_Attr_Clock_ದರ, dev));
        ಬರೆ("  GPU Clock Speed:                               %.2f GHz\n", clock_ದರ * 1e-6f);

        ಬರೆ("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        ಬರೆ("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        ಬರೆ("  Total amount of constant memory:               %u bytes\n", (ಪೂರ್ಣಾಂಕ)prop.ಒಟ್ಟು_Const_Mem);
        ಬರೆ("  Total amount of shared memory per block:       %u bytes\n", (ಪೂರ್ಣಾಂಕ)prop.shared_Mem_ಪ್ರತಿ_ಖಂಡ);
        ಬರೆ("  Total number of registers available per block: %d\n", prop.regs_ಪ್ರತಿ_ಖಂಡ);
        ಬರೆ("  Warp size:                                     %d\n", prop.warp_ಗಾತ್ರ);
        ಬರೆ("  Maximum number of threads per block:           %d\n", prop.max_ಎಳೆಗಳು_ಪ್ರತಿ_ಖಂಡ);
        ಬರೆ("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_ಎಳೆಗಳು_Dim[0], prop.max_ಎಳೆಗಳು_Dim[1], prop.max_ಎಳೆಗಳು_Dim[2]);
        ಬರೆ("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_ಗಾತ್ರ[0], prop.max_Grid_ಗಾತ್ರ[1],  prop.max_Grid_ಗಾತ್ರ[2]);
        ಬರೆ("  Maximum memory pitch:                          %u bytes\n", (ಪೂರ್ಣಾಂಕ)prop.memPitch);
        ಬರೆ("  Texture alignment:                             %u bytes\n", (ಪೂರ್ಣಾಂಕ)prop.textureAlignment);

        ಬರೆ("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_ಎಣಿಕೆ ? "Yes" : "No"), prop.async_Engine_ಎಣಿಕೆ);
        ಪೂರ್ಣಾಂಕ kernelExecTimeoutEnabled;
        cuda_ಸುರಕ್ಷಿತ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        ಬರೆ("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        ಬರೆ("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        ಬರೆ("  Support host page-locked memory mapping:       %s\n", prop.can_Map_Host_ಸ್ಮರಣೆ ? "Yes" : "No");

        ಬರೆ("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        ಬರೆ("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        ಬರೆ("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        ಬರೆ("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        ಬರೆ("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        ಬರೆ("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        ಪೂರ್ಣಾಂಕ prop_Compute_ಬಹುಲಕ;
        cuda_ಸುರಕ್ಷಿತ_Call(cudaDeviceGetAttribute(&prop_Compute_ಬಹುಲಕ, cuda_Dev_Attr_Compute_ಬಹುಲಕ, dev));
        ಬರೆ("  Compute Mode:\n");
        ಬರೆ("      %s \n", compute_ಬಹುಲಕ[prop_Compute_ಬಹುಲಕ]);
    }

    ಬರೆ("\n");
    ಬರೆ("deviceQuery, CUDA Driver = CUDART");
    ಬರೆ(", CUDA Driver Version  = %d.%d", driver_ಆವೃತ್ತಿ / 1000, driver_ಆವೃತ್ತಿ % 100);
    ಬರೆ(", CUDA Runtime Version = %d.%d", runtime_ಆವೃತ್ತಿ/1000, runtime_ಆವೃತ್ತಿ%100);
    ಬರೆ(", NumDevs = %d\n\n", ಎಣಿಕೆ_3);

    fflush(stdout);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::printShortCudaDeviceInfo(ಪೂರ್ಣಾಂಕ device)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(device);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = get_Cuda_Enabled_Device_ಎಣಿಕೆ();
    ತಾರ್ಕಿಕ valid = (device >= 0) && (device < ಎಣಿಕೆ_3);

    ಪೂರ್ಣಾಂಕ beg = valid ? device   : 0;
    ಪೂರ್ಣಾಂಕ end = valid ? device+1 : ಎಣಿಕೆ_3;

    ಪೂರ್ಣಾಂಕ driver_ಆವೃತ್ತಿ = 0, runtime_ಆವೃತ್ತಿ = 0;
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_Driver_Get_ಆವೃತ್ತಿ(&driver_ಆವೃತ್ತಿ) );
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_Runtime_Get_ಆವೃತ್ತಿ(&runtime_ಆವೃತ್ತಿ) );

    ಗಾಗಿ(ಪೂರ್ಣಾಂಕ dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_ಸುರಕ್ಷಿತ_Call( cudaGetDeviceProperties(&prop, dev) );

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        ಬರೆ("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (ದಶಮಾಂಶ)prop.ಒಟ್ಟು_Global_Mem/1048576.0f);
        ಬರೆ(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        ಪೂರ್ಣಾಂಕ cores = convertSMVer2Cores(prop.major, prop.minor);
        ಆದರೆ (cores > 0)
            ಬರೆ(", %d cores", cores * prop.multi_Processor_ಎಣಿಕೆ);

        ಬರೆ(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_ಆವೃತ್ತಿ/1000, driver_ಆವೃತ್ತಿ%100, runtime_ಆವೃತ್ತಿ/1000, runtime_ಆವೃತ್ತಿ%100);
    }

    fflush(stdout);
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// Error handling

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ
{
    #ವ್ಯಾಖ್ಯಾನಿಸು error_ನಮೂದು(ನಮೂದು)  { ನಮೂದು, #entry }

    ರಚನೆ Error_ನಮೂದು
    {
        ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* str;
    };

    ರಚನೆ Error_ನಮೂದು_Comparer
    {
        ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;
        Error_ನಮೂದು_Comparer(ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_3) : ಸಂಕೇತ_2(ಸಂಕೇತ_3) {}
        ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ Error_ನಮೂದು& e) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ e.ಸಂಕೇತ_2 == ಸಂಕೇತ_2; }
    };

    ಸ್ಥಿರಾಂಕ Error_ನಮೂದು npp_errors [] =
    {
    #ಆದರೆ defined (_MSC_VER)
        error_ನಮೂದು( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ NPP_ಆವೃತ್ತಿ < 5500
        error_ನಮೂದು( NPP_BAD_ARG_ERROR ),
        error_ನಮೂದು( NPP_COEFF_ERROR ),
        error_ನಮೂದು( NPP_RECT_ERROR ),
        error_ನಮೂದು( NPP_QUAD_ERROR ),
        error_ನಮೂದು( NPP_MEMFREE_ERR ),
        error_ನಮೂದು( NPP_MEMSET_ERR ),
        error_ನಮೂದು( NPP_MEM_ALLOC_ERR ),
        error_ನಮೂದು( NPP_HISTO_ಸಂಖ್ಯೆ_ದ_ಮಟ್ಟಗಳು_ERROR ),
        error_ನಮೂದು( NPP_MIRROR_FLIP_ERR ),
        error_ನಮೂದು( NPP_INVALID_ಒಳಹರಿವು ),
        error_ನಮೂದು( NPP_ಸೂಚಕ_ERROR ),
        error_ನಮೂದು( NPP_WARNING ),
        error_ನಮೂದು( NPP_ODD_ROI_WARNING ),
    #ಇಲ್ಲದಿದ್ದರೆ
        error_ನಮೂದು( NPP_INVALID_HOST_ಸೂಚಕ_ERROR ),
        error_ನಮೂದು( NPP_INVALID_DEVICE_ಸೂಚಕ_ERROR ),
        error_ನಮೂದು( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_ನಮೂದು( NPP_ZC_ಬಹುಲಕ_NOT_SUPPORTED_ERROR ),
        error_ನಮೂದು( NPP_MEMFREE_ERROR ),
        error_ನಮೂದು( NPP_MEMSET_ERROR ),
        error_ನಮೂದು( NPP_QUALITY_ಸೂಚ್ಯಂಕ_ERROR ),
        error_ನಮೂದು( NPP_HISTOGRAM_ಸಂಖ್ಯೆ_ದ_ಮಟ್ಟಗಳು_ERROR ),
        error_ನಮೂದು( NPP_CHANNEL_ಕ್ರಮ_ERROR ),
        error_ನಮೂದು( NPP_ZERO_ಮುಖವಾಡ_ಮೌಲ್ಯ_ERROR ),
        error_ನಮೂದು( NPP_QUADRANGLE_ERROR ),
        error_ನಮೂದು( NPP_RECTANGLE_ERROR ),
        error_ನಮೂದು( NPP_COEFFICIENT_ERROR ),
        error_ನಮೂದು( NPP_ಸಂಖ್ಯೆ_ದ_CHANNELS_ERROR ),
        error_ನಮೂದು( NPP_COI_ERROR ),
        error_ನಮೂದು( NPP_ಭಾಜಕ_ERROR ),
        error_ನಮೂದು( NPP_CHANNEL_ERROR ),
        error_ನಮೂದು( NPP_STRIDE_ERROR ),
        error_ನಮೂದು( NPP_ANCHOR_ERROR ),
        error_ನಮೂದು( NPP_ಮುಖವಾಡ_ಗಾತ್ರ_ERROR ),
        error_ನಮೂದು( NPP_MIRROR_FLIP_ERROR ),
        error_ನಮೂದು( NPP_MOMENT_00_ZERO_ERROR ),
        error_ನಮೂದು( NPP_ಮಿತಿ_NEGATIVE_LEVEL_ERROR ),
        error_ನಮೂದು( NPP_ಮಿತಿ_ERROR ),
        error_ನಮೂದು( NPP_CONTEXT_ಹೊಂದಿಕೆ_ERROR ),
        error_ನಮೂದು( NPP_FFT_FLAG_ERROR ),
        error_ನಮೂದು( NPP_FFT_ಕ್ರಮ_ERROR ),
        error_ನಮೂದು( NPP_SCALE_RANGE_ERROR ),
        error_ನಮೂದು( NPP_ದತ್ತಾಂಶ_TYPE_ERROR ),
        error_ನಮೂದು( NPP_ಹೊರಹೋಗು_OFF_RANGE_ERROR ),
        error_ನಮೂದು( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_ನಮೂದು( NPP_ಸ್ಮರಣೆ_ಹಂಚಿಕೆ_ERR ),
        error_ನಮೂದು( NPP_RANGE_ERROR ),
        error_ನಮೂದು( NPP_BAD_ವಾದ_ERROR ),
        error_ನಮೂದು( NPP_NO_ಸ್ಮರಣೆ_ERROR ),
        error_ನಮೂದು( NPP_ERROR_RESERVED ),
        error_ನಮೂದು( NPP_NO_OPERATION_WARNING ),
        error_ನಮೂದು( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_ನಮೂದು( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #ಆದರೆ_ಅಂತ್ಯ

        error_ನಮೂದು( NPP_NOT_SUPPORTED_ಬಹುಲಕ_ERROR ),
        error_ನಮೂದು( NPP_ಸುತ್ತು_ಬಹುಲಕ_NOT_SUPPORTED_ERROR ),
        error_ನಮೂದು( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_ನಮೂದು( NPP_LUT_ಸಂಖ್ಯೆ_ದ_ಮಟ್ಟಗಳು_ERROR ),
        error_ನಮೂದು( NPP_TEXTURE_BIND_ERROR ),
        error_ನಮೂದು( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_ನಮೂದು( NPP_NOT_EVEN_ಹೆಜ್ಜೆ_ERROR ),
        error_ನಮೂದು( NPP_INTERPOLATION_ERROR ),
        error_ನಮೂದು( NPP_RESIZE_ಗುಣಕ_ERROR ),
        error_ನಮೂದು( NPP_HAAR_CLASSIFIER_PIXEL_ಹೊಂದಿಕೆ_ERROR ),
        error_ನಮೂದು( NPP_MEMCPY_ERROR ),
        error_ನಮೂದು( NPP_ALIGNMENT_ERROR ),
        error_ನಮೂದು( NPP_ಹೆಜ್ಜೆ_ERROR ),
        error_ನಮೂದು( NPP_ಗಾತ್ರ_ERROR ),
        error_ನಮೂದು( NPP_NULL_ಸೂಚಕ_ERROR ),
        error_ನಮೂದು( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_ನಮೂದು( NPP_NOT_IMPLEMENTED_ERROR ),
        error_ನಮೂದು( NPP_ERROR ),
        error_ನಮೂದು( NPP_NO_ERROR ),
        error_ನಮೂದು( NPP_SUCCESS ),
        error_ನಮೂದು( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_ನಮೂದು( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_ನಮೂದು( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_ನಮೂದು( NPP_DOUBLE_ಗಾತ್ರ_WARNING )
    };

    ಸ್ಥಿರಾಂಕ size_t npp_error_ಅಂಕೆ = ಗಾತ್ರ(npp_errors) / ಗಾತ್ರ(npp_errors[0]);

    ಸ್ಥಿರಾಂಕ Error_ನಮೂದು cu_errors [] =
    {
        error_ನಮೂದು( CUDA_SUCCESS                              ),
        error_ನಮೂದು( CUDA_ERROR_INVALID_ಮೌಲ್ಯ                  ),
        error_ನಮೂದು( CUDA_ERROR_ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ                  ),
        error_ನಮೂದು( CUDA_ERROR_NOT_INITIALIZED                ),
        error_ನಮೂದು( CUDA_ERROR_DEINITIALIZED                  ),
        error_ನಮೂದು( CUDA_ERROR_PROFILER_DISABLED              ),
        error_ನಮೂದು( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_ನಮೂದು( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_ನಮೂದು( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_ನಮೂದು( CUDA_ERROR_NO_DEVICE                      ),
        error_ನಮೂದು( CUDA_ERROR_INVALID_DEVICE                 ),
        error_ನಮೂದು( CUDA_ERROR_INVALID_IMAGE                  ),
        error_ನಮೂದು( CUDA_ERROR_INVALID_CONTEXT                ),
        error_ನಮೂದು( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_ನಮೂದು( CUDA_ERROR_MAP_FAILED                     ),
        error_ನಮೂದು( CUDA_ERROR_UNMAP_FAILED                   ),
        error_ನಮೂದು( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_ನಮೂದು( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_ನಮೂದು( CUDA_ERROR_NO_ದ್ವಿಮಾನ_FOR_GPU              ),
        error_ನಮೂದು( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_ನಮೂದು( CUDA_ERROR_NOT_MAPPED                     ),
        error_ನಮೂದು( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_ನಮೂದು( CUDA_ERROR_NOT_MAPPED_AS_ಸೂಚಕ          ),
        error_ನಮೂದು( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_ನಮೂದು( CUDA_ERROR_UNSUPPORTED_ಎಲ್ಲೆ              ),
        error_ನಮೂದು( CUDA_ERROR_CONTEXT_ALREADY_ಒಳ_USE         ),
        error_ನಮೂದು( CUDA_ERROR_INVALID_ಮೂಲ                 ),
        error_ನಮೂದು( CUDA_ERROR_FILE_NOT_ಸಿಕ್ಕಿತು                 ),
        error_ನಮೂದು( CUDA_ERROR_SHARED_OBJECT_ಚಿಹ್ನೆ_NOT_ಸಿಕ್ಕಿತು ),
        error_ನಮೂದು( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_ನಮೂದು( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_ನಮೂದು( CUDA_ERROR_INVALID_HANDLE                 ),
        error_ನಮೂದು( CUDA_ERROR_NOT_ಸಿಕ್ಕಿತು                      ),
        error_ನಮೂದು( CUDA_ERROR_NOT_READY                      ),
        error_ನಮೂದು( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_ನಮೂದು( CUDA_ERROR_LAUNCH_ಹೊರಹೋಗು_ದ_RESOURCES        ),
        error_ನಮೂದು( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_ನಮೂದು( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_ನಮೂದು( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_ನಮೂದು( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_ನಮೂದು( CUDA_ERROR_PRIMARY_CONTEXT_ಸಕ್ರಿಯ         ),
        error_ನಮೂದು( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_ನಮೂದು( CUDA_ERROR_ASSERT                         ),
        error_ನಮೂದು( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_ನಮೂದು( CUDA_ERROR_HOST_ಸ್ಮರಣೆ_ALREADY_REGISTERED ),
        error_ನಮೂದು( CUDA_ERROR_HOST_ಸ್ಮರಣೆ_NOT_REGISTERED     ),
        error_ನಮೂದು( CUDA_ERROR_UNKNOWN                        )
    };

    ಸ್ಥಿರಾಂಕ size_t cu_errors_ಅಂಕೆ = ಗಾತ್ರ(cu_errors) / ಗಾತ್ರ(cu_errors[0]);

    cv::String getErrorString(ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2, ಸ್ಥಿರಾಂಕ Error_ನಮೂದು* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_ನಮೂದು_Comparer(ಸಂಕೇತ_2)) - errors;

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸಂದೇಶ = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", ಸಂದೇಶ, ಸಂಕೇತ_2);

        ಹಿಂತಿರುಗಿ str;
    }
}

#ಆದರೆ_ಅಂತ್ಯ

String cv::cuda::get_Npp_Error_ಸಂದೇಶ(ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ String();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ getErrorString(ಸಂಕೇತ_2, npp_errors, npp_error_ಅಂಕೆ);
#ಆದರೆ_ಅಂತ್ಯ
}

String cv::cuda::get_Cuda_Driver_Api_Error_ಸಂದೇಶ(ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(ಸಂಕೇತ_2);
    ಹಿಂತಿರುಗಿ String();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ getErrorString(ಸಂಕೇತ_2, cu_errors, cu_errors_ಅಂಕೆ);
#ಆದರೆ_ಅಂತ್ಯ
}
