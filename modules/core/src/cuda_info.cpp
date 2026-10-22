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

#ضمن "precomp.hpp"

استعمال نطاق cv;
استعمال نطاق cv::cuda;

صحيح cv::cuda::get_Cuda_Enabled_Device_ڳڻپ()
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    أرجع 0;
#وإلا
    صحيح ڳڻپ_2;
    cudaError_t error = cuda_Get_Device_ڳڻپ(&ڳڻپ_2);

    إذا (error == cudaErrorInsufficientDriver)
        أرجع -1;

    إذا (error == cudaErrorNoDevice)
        أرجع 0;

    cuda_محفوظ_Call( error );
    أرجع ڳڻپ_2;
#جيڪڏهن_پورو
}

فراغ cv::cuda::سيٽ_Device(صحيح device)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(device);
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cuda_سيٽ_Device(device) );
    cuda_محفوظ_Call( cudaFree(0) );
#جيڪڏهن_پورو
}

صحيح cv::cuda::getDevice()
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح device;
    cuda_محفوظ_Call( cudaGetDevice(&device) );
    أرجع device;
#جيڪڏهن_پورو
}

فراغ cv::cuda::resetDevice()
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cudaDeviceReset() );
#جيڪڏهن_پورو
}

منطقي cv::cuda::deviceSupports(Feature_سيٽ feature_سيٽ)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(feature_سيٽ);
    throw_no_cuda();
#وإلا
    ساكن صحيح versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    ساكن ثابت صحيح cache_سائيز = static_cast<صحيح>(حجم(versions) / حجم(versions[0]));

    ثابت صحيح devId = getDevice();

    صحيح نسخو;

    إذا (devId < cache_سائيز && versions[devId] >= 0)
    {
        نسخو = versions[devId];
    }
    وإلا
    {
        DeviceInfo dev(devId);
        نسخو = dev.major_نسخو() * 10 + dev.minor_نسخو();
        إذا (devId < cache_سائيز)
            versions[devId] = نسخو;
    }

    أرجع هدف_Archs::builtWith(feature_سيٽ) && (نسخو >= feature_سيٽ);
#جيڪڏهن_پورو
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق
{
    صنف CudaArch
    {
    عام:
        CudaArch();

        منطقي builtWith(Feature_سيٽ feature_سيٽ) ثابت;
        منطقي hasPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasBin(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrLessPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrGreaterPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrGreaterBin(صحيح major, صحيح minor) ثابت;

    خاص:
        ساكن فراغ fromStr(ثابت حرف* سيٽ_as_str, std::vector<صحيح>& arr);

        std::vector<صحيح> bin;
        std::vector<صحيح> ptx;
        std::vector<صحيح> features;
    };

    ثابت CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    منطقي CudaArch::builtWith(Feature_سيٽ feature_سيٽ) ثابت
    {
        أرجع !features.empty() && (features.back() >= feature_سيٽ);
    }

    منطقي CudaArch::hasPtx(صحيح major, صحيح minor) ثابت
    {
        أرجع std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    منطقي CudaArch::hasBin(صحيح major, صحيح minor) ثابت
    {
        أرجع std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    منطقي CudaArch::hasEqualOrLessPtx(صحيح major, صحيح minor) ثابت
    {
        أرجع !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    منطقي CudaArch::hasEqualOrGreaterPtx(صحيح major, صحيح minor) ثابت
    {
        أرجع !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    منطقي CudaArch::hasEqualOrGreaterBin(صحيح major, صحيح minor) ثابت
    {
        أرجع !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    فراغ CudaArch::fromStr(ثابت حرف* سيٽ_as_str, std::vector<صحيح>& arr)
    {
        arr.clear();

        ثابت size_t len = لڙي_ڊيگهه(سيٽ_as_str);

        size_t pos = 0;
        طالما (pos < len)
        {
            إذا (خال_آهي(سيٽ_as_str[pos]))
            {
                ++pos;
            }
            وإلا
            {
                صحيح cur_قدر;
                صحيح chars_read;
                صحيح دليلون_read = لڙي_مان_پڙهو(سيٽ_as_str + pos, "%d%n", &cur_قدر, &chars_read);
                CV_Assert( دليلون_read == 1 );

                arr.push_back(cur_قدر);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#جيڪڏهن_پورو

منطقي cv::cuda::هدف_Archs::builtWith(cv::cuda::Feature_سيٽ feature_سيٽ)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(feature_سيٽ);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.builtWith(feature_سيٽ);
#جيڪڏهن_پورو
}

منطقي cv::cuda::هدف_Archs::hasPtx(صحيح major, صحيح minor)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(major);
    CV_اڻ_استعمال_ٿيل(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasPtx(major, minor);
#جيڪڏهن_پورو
}

منطقي cv::cuda::هدف_Archs::hasBin(صحيح major, صحيح minor)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(major);
    CV_اڻ_استعمال_ٿيل(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasBin(major, minor);
#جيڪڏهن_پورو
}

منطقي cv::cuda::هدف_Archs::hasEqualOrLessPtx(صحيح major, صحيح minor)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(major);
    CV_اڻ_استعمال_ٿيل(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrLessPtx(major, minor);
#جيڪڏهن_پورو
}

منطقي cv::cuda::هدف_Archs::hasEqualOrGreaterPtx(صحيح major, صحيح minor)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(major);
    CV_اڻ_استعمال_ٿيل(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrGreaterPtx(major, minor);
#جيڪڏهن_پورو
}

منطقي cv::cuda::هدف_Archs::hasEqualOrGreaterBin(صحيح major, صحيح minor)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(major);
    CV_اڻ_استعمال_ٿيل(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrGreaterBin(major, minor);
#جيڪڏهن_پورو
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق
{
    صنف DeviceProps
    {
    عام:
        DeviceProps();

        ثابت cudaDeviceProp* get(صحيح devID) ثابت;

    خاص:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        صحيح ڳڻپ_2 = get_Cuda_Enabled_Device_ڳڻپ();

        إذا (ڳڻپ_2 > 0)
        {
            props_.resize(ڳڻپ_2);

            لكل (صحيح devID = 0; devID < ڳڻپ_2; ++devID)
            {
                cuda_محفوظ_Call( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ثابت cudaDeviceProp* DeviceProps::get(صحيح devID) ثابت
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        أرجع &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        ساكن DeviceProps props;
        أرجع props;
    }
}

#جيڪڏهن_پورو

ثابت حرف* cv::cuda::DeviceInfo::name() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->name;
#جيڪڏهن_پورو
}

size_t cv::cuda::DeviceInfo::ڪل_Global_Mem() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->ڪل_Global_Mem;
#جيڪڏهن_پورو
}

size_t cv::cuda::DeviceInfo::shared_Mem_في_بلاڪ() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->shared_Mem_في_بلاڪ;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::regs_في_بلاڪ() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->regs_في_بلاڪ;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::warp_سائيز() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->warp_سائيز;
#جيڪڏهن_پورو
}

size_t cv::cuda::DeviceInfo::memPitch() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->memPitch;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::max_ڌاڳا_في_بلاڪ() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->max_ڌاڳا_في_بلاڪ;
#جيڪڏهن_پورو
}

Vec3i cv::cuda::DeviceInfo::max_ڌاڳا_Dim() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->max_ڌاڳا_Dim);
#جيڪڏهن_پورو
}

Vec3i cv::cuda::DeviceInfo::max_Grid_سائيز() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->max_Grid_سائيز);
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::clock_شرح() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح clock_شرح;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&clock_شرح, cuda_Dev_Attr_Clock_شرح, device_id_));
    أرجع clock_شرح;
#جيڪڏهن_پورو
}

size_t cv::cuda::DeviceInfo::ڪل_Const_Mem() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->ڪل_Const_Mem;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::major_نسخو() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->major;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::minor_نسخو() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->minor;
#جيڪڏهن_پورو
}

size_t cv::cuda::DeviceInfo::textureAlignment() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->textureAlignment;
#جيڪڏهن_پورو
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->texturePitchAlignment;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::multi_Processor_ڳڻپ() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->multi_Processor_ڳڻپ;
#جيڪڏهن_پورو
}

منطقي cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح kernelExecTimeoutEnabled;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    أرجع kernelExecTimeoutEnabled != 0;
#جيڪڏهن_پورو
}

منطقي cv::cuda::DeviceInfo::integrated() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->integrated != 0;
#جيڪڏهن_پورو
}

منطقي cv::cuda::DeviceInfo::can_Map_Host_يادگيري() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->can_Map_Host_يادگيري != 0;
#جيڪڏهن_پورو
}

DeviceInfo::Compute_گهڻائي cv::cuda::DeviceInfo::compute_گهڻائي() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    ساكن ثابت Compute_گهڻائي tbl[] =
    {
        Compute_گهڻائي_Default,
        Compute_گهڻائي_Exclusive,
        Compute_گهڻائي_Prohibited,
        Compute_گهڻائي_Exclusive_Process
    };

    صحيح compute_گهڻائي;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&compute_گهڻائي, cuda_Dev_Attr_Compute_گهڻائي, device_id_));
    أرجع tbl[compute_گهڻائي];
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::maxTexture1D() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxTexture1D;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::maxTexture1DMipmap() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخو >= 5000
        أرجع deviceProps().get(device_id_)->maxTexture1DMipmap;
    #وإلا
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        أرجع 0;
    #جيڪڏهن_پورو
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::maxTexture1DLinear() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخو >= 13000
        size_t max_ويڪر_اندريون_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cuda_محفوظ_Call(cuda_Device_Get_Texture1_DLinear_Max_ويڪر(&max_ويڪر_اندريون_Elements, &fmtDesc, device_id_));
        أرجع max_ويڪر_اندريون_Elements;
    #وإلا
        أرجع deviceProps().get(device_id_)->maxTexture1DLinear;
    #جيڪڏهن_پورو
#جيڪڏهن_پورو
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#جيڪڏهن_پورو
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخو >= 5000
        أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #وإلا
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        أرجع Vec2i();
    #جيڪڏهن_پورو
#جيڪڏهن_پورو
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#جيڪڏهن_پورو
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#جيڪڏهن_پورو
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::maxTextureCubemap() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxTextureCubemap;
#جيڪڏهن_پورو
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#جيڪڏهن_پورو
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#جيڪڏهن_پورو
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::maxSurface1D() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxSurface1D;
#جيڪڏهن_پورو
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#جيڪڏهن_پورو
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#جيڪڏهن_پورو
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#جيڪڏهن_پورو
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::maxSurfaceCubemap() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxSurfaceCubemap;
#جيڪڏهن_پورو
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#جيڪڏهن_پورو
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->surfaceAlignment;
#جيڪڏهن_پورو
}

منطقي cv::cuda::DeviceInfo::concurrentKernels() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->concurrentKernels != 0;
#جيڪڏهن_پورو
}

منطقي cv::cuda::DeviceInfo::ECCEnabled() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->ECCEnabled != 0;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::pciBusID() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciBusID;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::pciDeviceID() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciDeviceID;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::pciDomainID() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciDomainID;
#جيڪڏهن_پورو
}

منطقي cv::cuda::DeviceInfo::tccDriver() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->tccDriver != 0;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::async_Engine_ڳڻپ() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->async_Engine_ڳڻپ;
#جيڪڏهن_پورو
}

منطقي cv::cuda::DeviceInfo::unifiedAddressing() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->unifiedAddressing != 0;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::يادگيري_Clock_شرح() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح يادگيري_Clock_شرح;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&يادگيري_Clock_شرح, cuda_Dev_Attr_يادگيري_Clock_شرح, device_id_));
    أرجع يادگيري_Clock_شرح;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::يادگيري_Bus_ويڪر() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->يادگيري_Bus_ويڪر;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::l2_Cache_سائيز() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->l2_Cache_سائيز;
#جيڪڏهن_پورو
}

صحيح cv::cuda::DeviceInfo::max_ڌاڳا_في_Multi_Processor() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->max_ڌاڳا_في_Multi_Processor;
#جيڪڏهن_پورو
}

فراغ cv::cuda::DeviceInfo::query_يادگيري(size_t& ڪل_يادگيري, size_t& free_يادگيري) ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(ڪل_يادگيري);
    CV_اڻ_استعمال_ٿيل(free_يادگيري);
    throw_no_cuda();
#وإلا
    صحيح prevDeviceID = getDevice();
    إذا (prevDeviceID != device_id_)
        سيٽ_Device(device_id_);

    cuda_محفوظ_Call( cudaMemGetInfo(&free_يادگيري, &ڪل_يادگيري) );

    إذا (prevDeviceID != device_id_)
        سيٽ_Device(prevDeviceID);
#جيڪڏهن_پورو
}

منطقي cv::cuda::DeviceInfo::isCompatible() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    // Check PTX compatibility
    إذا (هدف_Archs::hasEqualOrLessPtx(major_نسخو(), minor_نسخو()))
        أرجع سچ;

    // Check BIN compatibility
    لكل (صحيح i = minor_نسخو(); i >= 0; --i)
        إذا (هدف_Archs::hasBin(major_نسخو(), i))
            أرجع سچ;

    أرجع خطأ;
#جيڪڏهن_پورو
}

////////////////////////////////////////////////////////////////////////
// print info

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق
{
    صحيح convertSMVer2Cores(صحيح major, صحيح minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        تعريفنوع بنية {
            صحيح SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            صحيح Cores;
        } SMtoCores;

        SMtoCores gpu_Arch_Cores_في_SM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        صحيح اشاريو = 0;
        طالما (gpu_Arch_Cores_في_SM[اشاريو].SM != -1)
        {
            إذا (gpu_Arch_Cores_في_SM[اشاريو].SM == ((major << 4) + minor) )
                أرجع gpu_Arch_Cores_في_SM[اشاريو].Cores;
            اشاريو++;
        }

        أرجع -1;
    }
}

#جيڪڏهن_پورو

فراغ cv::cuda::printCudaDeviceInfo(صحيح device)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(device);
    throw_no_cuda();
#وإلا
    صحيح ڳڻپ_2 = get_Cuda_Enabled_Device_ڳڻپ();
    منطقي valid = (device >= 0) && (device < ڳڻپ_2);

    صحيح beg = valid ? device   : 0;
    صحيح end = valid ? device+1 : ڳڻپ_2;

    لکو("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    لکو("Device count: %d\n", ڳڻپ_2);

    صحيح driver_نسخو = 0, runtime_نسخو = 0;
    cuda_محفوظ_Call( cuda_Driver_Get_نسخو(&driver_نسخو) );
    cuda_محفوظ_Call( cuda_Runtime_Get_نسخو(&runtime_نسخو) );

    ثابت حرف *compute_گهڻائي[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        عدم
    };

    لكل(صحيح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_محفوظ_Call( cudaGetDeviceProperties(&prop, dev) );

        لکو("\nDevice %d: \"%s\"\n", dev, prop.name);
        لکو("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_نسخو/1000, driver_نسخو%100, runtime_نسخو/1000, runtime_نسخو%100);
        لکو("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        لکو("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (عائم)prop.ڪل_Global_Mem/1048576.0f, (بنانشان ڊگهو ڊگهو) prop.ڪل_Global_Mem);

        صحيح cores = convertSMVer2Cores(prop.major, prop.minor);
        إذا (cores > 0)
            لکو("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_ڳڻپ, cores, cores * prop.multi_Processor_ڳڻپ);

        صحيح clock_شرح;
        cuda_محفوظ_Call(cudaDeviceGetAttribute(&clock_شرح, cuda_Dev_Attr_Clock_شرح, dev));
        لکو("  GPU Clock Speed:                               %.2f GHz\n", clock_شرح * 1e-6f);

        لکو("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        لکو("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        لکو("  Total amount of constant memory:               %u bytes\n", (صحيح)prop.ڪل_Const_Mem);
        لکو("  Total amount of shared memory per block:       %u bytes\n", (صحيح)prop.shared_Mem_في_بلاڪ);
        لکو("  Total number of registers available per block: %d\n", prop.regs_في_بلاڪ);
        لکو("  Warp size:                                     %d\n", prop.warp_سائيز);
        لکو("  Maximum number of threads per block:           %d\n", prop.max_ڌاڳا_في_بلاڪ);
        لکو("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_ڌاڳا_Dim[0], prop.max_ڌاڳا_Dim[1], prop.max_ڌاڳا_Dim[2]);
        لکو("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_سائيز[0], prop.max_Grid_سائيز[1],  prop.max_Grid_سائيز[2]);
        لکو("  Maximum memory pitch:                          %u bytes\n", (صحيح)prop.memPitch);
        لکو("  Texture alignment:                             %u bytes\n", (صحيح)prop.textureAlignment);

        لکو("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_ڳڻپ ? "Yes" : "No"), prop.async_Engine_ڳڻپ);
        صحيح kernelExecTimeoutEnabled;
        cuda_محفوظ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        لکو("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        لکو("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        لکو("  Support host page-locked memory mapping:       %s\n", prop.can_Map_Host_يادگيري ? "Yes" : "No");

        لکو("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        لکو("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        لکو("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        لکو("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        لکو("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        لکو("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        صحيح prop_Compute_گهڻائي;
        cuda_محفوظ_Call(cudaDeviceGetAttribute(&prop_Compute_گهڻائي, cuda_Dev_Attr_Compute_گهڻائي, dev));
        لکو("  Compute Mode:\n");
        لکو("      %s \n", compute_گهڻائي[prop_Compute_گهڻائي]);
    }

    لکو("\n");
    لکو("deviceQuery, CUDA Driver = CUDART");
    لکو(", CUDA Driver Version  = %d.%d", driver_نسخو / 1000, driver_نسخو % 100);
    لکو(", CUDA Runtime Version = %d.%d", runtime_نسخو/1000, runtime_نسخو%100);
    لکو(", NumDevs = %d\n\n", ڳڻپ_2);

    fflush(stdout);
#جيڪڏهن_پورو
}

فراغ cv::cuda::printShortCudaDeviceInfo(صحيح device)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(device);
    throw_no_cuda();
#وإلا
    صحيح ڳڻپ_2 = get_Cuda_Enabled_Device_ڳڻپ();
    منطقي valid = (device >= 0) && (device < ڳڻپ_2);

    صحيح beg = valid ? device   : 0;
    صحيح end = valid ? device+1 : ڳڻپ_2;

    صحيح driver_نسخو = 0, runtime_نسخو = 0;
    cuda_محفوظ_Call( cuda_Driver_Get_نسخو(&driver_نسخو) );
    cuda_محفوظ_Call( cuda_Runtime_Get_نسخو(&runtime_نسخو) );

    لكل(صحيح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_محفوظ_Call( cudaGetDeviceProperties(&prop, dev) );

        ثابت حرف *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        لکو("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (عائم)prop.ڪل_Global_Mem/1048576.0f);
        لکو(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        صحيح cores = convertSMVer2Cores(prop.major, prop.minor);
        إذا (cores > 0)
            لکو(", %d cores", cores * prop.multi_Processor_ڳڻپ);

        لکو(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_نسخو/1000, driver_نسخو%100, runtime_نسخو/1000, runtime_نسخو%100);
    }

    fflush(stdout);
#جيڪڏهن_پورو
}

////////////////////////////////////////////////////////////////////////
// Error handling

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق
{
    #وصف_ڪريو error_داخلا(داخلا_3)  { داخلا_3, #entry }

    بنية Error_داخلا
    {
        صحيح ڪوڊ;
        ثابت حرف* str;
    };

    بنية Error_داخلا_Comparer
    {
        صحيح ڪوڊ;
        Error_داخلا_Comparer(صحيح ڪوڊ_2) : ڪوڊ(ڪوڊ_2) {}
        منطقي عامل()(ثابت Error_داخلا& e) ثابت { أرجع e.ڪوڊ == ڪوڊ; }
    };

    ثابت Error_داخلا npp_errors [] =
    {
    #إذا defined (_MSC_VER)
        error_داخلا( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #جيڪڏهن_پورو

    #إذا NPP_نسخو < 5500
        error_داخلا( NPP_BAD_ARG_ERROR ),
        error_داخلا( NPP_COEFF_ERROR ),
        error_داخلا( NPP_RECT_ERROR ),
        error_داخلا( NPP_QUAD_ERROR ),
        error_داخلا( NPP_MEMFREE_ERR ),
        error_داخلا( NPP_MEMSET_ERR ),
        error_داخلا( NPP_MEM_ALLOC_ERR ),
        error_داخلا( NPP_HISTO_عدد_جو_سطحون_ERROR ),
        error_داخلا( NPP_MIRROR_FLIP_ERR ),
        error_داخلا( NPP_INVALID_داخلا ),
        error_داخلا( NPP_اشارو_ERROR ),
        error_داخلا( NPP_WARNING ),
        error_داخلا( NPP_ODD_ROI_WARNING ),
    #وإلا
        error_داخلا( NPP_INVALID_HOST_اشارو_ERROR ),
        error_داخلا( NPP_INVALID_DEVICE_اشارو_ERROR ),
        error_داخلا( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_داخلا( NPP_ZC_گهڻائي_NOT_SUPPORTED_ERROR ),
        error_داخلا( NPP_MEMFREE_ERROR ),
        error_داخلا( NPP_MEMSET_ERROR ),
        error_داخلا( NPP_QUALITY_اشاريو_ERROR ),
        error_داخلا( NPP_HISTOGRAM_عدد_جو_سطحون_ERROR ),
        error_داخلا( NPP_CHANNEL_ترتيب_ERROR ),
        error_داخلا( NPP_ZERO_نقاب_قدر_ERROR ),
        error_داخلا( NPP_QUADRANGLE_ERROR ),
        error_داخلا( NPP_RECTANGLE_ERROR ),
        error_داخلا( NPP_COEFFICIENT_ERROR ),
        error_داخلا( NPP_عدد_جو_CHANNELS_ERROR ),
        error_داخلا( NPP_COI_ERROR ),
        error_داخلا( NPP_ورهائيندڙ_ERROR ),
        error_داخلا( NPP_CHANNEL_ERROR ),
        error_داخلا( NPP_STRIDE_ERROR ),
        error_داخلا( NPP_ANCHOR_ERROR ),
        error_داخلا( NPP_نقاب_سائيز_ERROR ),
        error_داخلا( NPP_MIRROR_FLIP_ERROR ),
        error_داخلا( NPP_MOMENT_00_ZERO_ERROR ),
        error_داخلا( NPP_حد_NEGATIVE_LEVEL_ERROR ),
        error_داخلا( NPP_حد_ERROR ),
        error_داخلا( NPP_CONTEXT_ميلاپ_ERROR ),
        error_داخلا( NPP_FFT_FLAG_ERROR ),
        error_داخلا( NPP_FFT_ترتيب_ERROR ),
        error_داخلا( NPP_SCALE_RANGE_ERROR ),
        error_داخلا( NPP_معلومات_TYPE_ERROR ),
        error_داخلا( NPP_ٻاهريون_OFF_RANGE_ERROR ),
        error_داخلا( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_داخلا( NPP_يادگيري_ورهاست_ERR ),
        error_داخلا( NPP_RANGE_ERROR ),
        error_داخلا( NPP_BAD_دليل_ERROR ),
        error_داخلا( NPP_NO_يادگيري_ERROR ),
        error_داخلا( NPP_ERROR_RESERVED ),
        error_داخلا( NPP_NO_OPERATION_WARNING ),
        error_داخلا( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_داخلا( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #جيڪڏهن_پورو

        error_داخلا( NPP_NOT_SUPPORTED_گهڻائي_ERROR ),
        error_داخلا( NPP_ڦيرو_گهڻائي_NOT_SUPPORTED_ERROR ),
        error_داخلا( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_داخلا( NPP_LUT_عدد_جو_سطحون_ERROR ),
        error_داخلا( NPP_TEXTURE_BIND_ERROR ),
        error_داخلا( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_داخلا( NPP_NOT_EVEN_قدم_ERROR ),
        error_داخلا( NPP_INTERPOLATION_ERROR ),
        error_داخلا( NPP_RESIZE_ضرب_ڪندڙ_ERROR ),
        error_داخلا( NPP_HAAR_CLASSIFIER_PIXEL_ميلاپ_ERROR ),
        error_داخلا( NPP_MEMCPY_ERROR ),
        error_داخلا( NPP_ALIGNMENT_ERROR ),
        error_داخلا( NPP_قدم_ERROR ),
        error_داخلا( NPP_سائيز_ERROR ),
        error_داخلا( NPP_NULL_اشارو_ERROR ),
        error_داخلا( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_داخلا( NPP_NOT_IMPLEMENTED_ERROR ),
        error_داخلا( NPP_ERROR ),
        error_داخلا( NPP_NO_ERROR ),
        error_داخلا( NPP_SUCCESS ),
        error_داخلا( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_داخلا( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_داخلا( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_داخلا( NPP_DOUBLE_سائيز_WARNING )
    };

    ثابت size_t npp_error_انگ = حجم(npp_errors) / حجم(npp_errors[0]);

    ثابت Error_داخلا cu_errors [] =
    {
        error_داخلا( CUDA_SUCCESS                              ),
        error_داخلا( CUDA_ERROR_INVALID_قدر                  ),
        error_داخلا( CUDA_ERROR_ٻاهريون_جو_يادگيري                  ),
        error_داخلا( CUDA_ERROR_NOT_INITIALIZED                ),
        error_داخلا( CUDA_ERROR_DEINITIALIZED                  ),
        error_داخلا( CUDA_ERROR_PROFILER_DISABLED              ),
        error_داخلا( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_داخلا( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_داخلا( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_داخلا( CUDA_ERROR_NO_DEVICE                      ),
        error_داخلا( CUDA_ERROR_INVALID_DEVICE                 ),
        error_داخلا( CUDA_ERROR_INVALID_IMAGE                  ),
        error_داخلا( CUDA_ERROR_INVALID_CONTEXT                ),
        error_داخلا( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_داخلا( CUDA_ERROR_MAP_FAILED                     ),
        error_داخلا( CUDA_ERROR_UNMAP_FAILED                   ),
        error_داخلا( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_داخلا( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_داخلا( CUDA_ERROR_NO_ٻٽو_FOR_GPU              ),
        error_داخلا( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_داخلا( CUDA_ERROR_NOT_MAPPED                     ),
        error_داخلا( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_داخلا( CUDA_ERROR_NOT_MAPPED_AS_اشارو          ),
        error_داخلا( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_داخلا( CUDA_ERROR_UNSUPPORTED_حد_بندي              ),
        error_داخلا( CUDA_ERROR_CONTEXT_ALREADY_اندريون_USE         ),
        error_داخلا( CUDA_ERROR_INVALID_ذريعو                 ),
        error_داخلا( CUDA_ERROR_FILE_NOT_مليو                 ),
        error_داخلا( CUDA_ERROR_SHARED_OBJECT_نشان_NOT_مليو ),
        error_داخلا( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_داخلا( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_داخلا( CUDA_ERROR_INVALID_HANDLE                 ),
        error_داخلا( CUDA_ERROR_NOT_مليو                      ),
        error_داخلا( CUDA_ERROR_NOT_READY                      ),
        error_داخلا( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_داخلا( CUDA_ERROR_LAUNCH_ٻاهريون_جو_RESOURCES        ),
        error_داخلا( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_داخلا( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_داخلا( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_داخلا( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_داخلا( CUDA_ERROR_PRIMARY_CONTEXT_سرگرم         ),
        error_داخلا( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_داخلا( CUDA_ERROR_ASSERT                         ),
        error_داخلا( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_داخلا( CUDA_ERROR_HOST_يادگيري_ALREADY_REGISTERED ),
        error_داخلا( CUDA_ERROR_HOST_يادگيري_NOT_REGISTERED     ),
        error_داخلا( CUDA_ERROR_UNKNOWN                        )
    };

    ثابت size_t cu_errors_انگ = حجم(cu_errors) / حجم(cu_errors[0]);

    cv::String getErrorString(صحيح ڪوڊ, ثابت Error_داخلا* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_داخلا_Comparer(ڪوڊ)) - errors;

        ثابت حرف* نياپو = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", نياپو, ڪوڊ);

        أرجع str;
    }
}

#جيڪڏهن_پورو

String cv::cuda::get_Npp_Error_پيغام(صحيح ڪوڊ)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(ڪوڊ);
    أرجع String();
#وإلا
    أرجع getErrorString(ڪوڊ, npp_errors, npp_error_انگ);
#جيڪڏهن_پورو
}

String cv::cuda::get_Cuda_Driver_Api_Error_پيغام(صحيح ڪوڊ)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(ڪوڊ);
    أرجع String();
#وإلا
    أرجع getErrorString(ڪوڊ, cu_errors, cu_errors_انگ);
#جيڪڏهن_پورو
}
