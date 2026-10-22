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

استفاده نطاق cv;
استفاده نطاق cv::cuda;

صحيح cv::cuda::get_Cuda_Enabled_Device_شمار()
{
#اگر_تعریف_نشده HAVE_CUDA
    أرجع 0;
#وإلا
    صحيح شمار_2;
    cudaError_t error = cuda_Get_Device_شمار(&شمار_2);

    إذا (error == cudaErrorInsufficientDriver)
        أرجع -1;

    إذا (error == cudaErrorNoDevice)
        أرجع 0;

    cuda_امن_Call( error );
    أرجع شمار_2;
#پایان_اگر
}

فراغ cv::cuda::مجموعه_Device(صحيح device)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(device);
    throw_no_cuda();
#وإلا
    cuda_امن_Call( cuda_مجموعه_Device(device) );
    cuda_امن_Call( cudaFree(0) );
#پایان_اگر
}

صحيح cv::cuda::getDevice()
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح device;
    cuda_امن_Call( cudaGetDevice(&device) );
    أرجع device;
#پایان_اگر
}

فراغ cv::cuda::resetDevice()
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_امن_Call( cudaDeviceReset() );
#پایان_اگر
}

منطقي cv::cuda::deviceSupports(Feature_مجموعه feature_مجموعه)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(feature_مجموعه);
    throw_no_cuda();
#وإلا
    ساكن صحيح versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    ساكن ثابت صحيح cache_اندازه = static_cast<صحيح>(حجم(versions) / حجم(versions[0]));

    ثابت صحيح devId = getDevice();

    صحيح نسخه;

    إذا (devId < cache_اندازه && versions[devId] >= 0)
    {
        نسخه = versions[devId];
    }
    وإلا
    {
        DeviceInfo dev(devId);
        نسخه = dev.major_نسخه() * 10 + dev.minor_نسخه();
        إذا (devId < cache_اندازه)
            versions[devId] = نسخه;
    }

    أرجع هدف_Archs::builtWith(feature_مجموعه) && (نسخه >= feature_مجموعه);
#پایان_اگر
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#اگر_تعریف_شده HAVE_CUDA

نطاق
{
    صنف CudaArch
    {
    عام:
        CudaArch();

        منطقي builtWith(Feature_مجموعه feature_مجموعه) ثابت;
        منطقي hasPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasBin(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrLessPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrGreaterPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrGreaterBin(صحيح major, صحيح minor) ثابت;

    خاص:
        ساكن فراغ fromStr(ثابت حرف* مجموعه_as_str, std::vector<صحيح>& arr);

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

    منطقي CudaArch::builtWith(Feature_مجموعه feature_مجموعه) ثابت
    {
        أرجع !features.empty() && (features.back() >= feature_مجموعه);
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

    فراغ CudaArch::fromStr(ثابت حرف* مجموعه_as_str, std::vector<صحيح>& arr)
    {
        arr.clear();

        ثابت size_t len = طول_رشته(مجموعه_as_str);

        size_t pos = 0;
        طالما (pos < len)
        {
            إذا (آیا_فاصله(مجموعه_as_str[pos]))
            {
                ++pos;
            }
            وإلا
            {
                صحيح cur_مقدار;
                صحيح chars_read;
                صحيح آرگومانها_read = از_رشته_بخوان(مجموعه_as_str + pos, "%d%n", &cur_مقدار, &chars_read);
                CV_Assert( آرگومانها_read == 1 );

                arr.push_back(cur_مقدار);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#پایان_اگر

منطقي cv::cuda::هدف_Archs::builtWith(cv::cuda::Feature_مجموعه feature_مجموعه)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(feature_مجموعه);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.builtWith(feature_مجموعه);
#پایان_اگر
}

منطقي cv::cuda::هدف_Archs::hasPtx(صحيح major, صحيح minor)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(major);
    CV_استفادهنشده(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasPtx(major, minor);
#پایان_اگر
}

منطقي cv::cuda::هدف_Archs::hasBin(صحيح major, صحيح minor)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(major);
    CV_استفادهنشده(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasBin(major, minor);
#پایان_اگر
}

منطقي cv::cuda::هدف_Archs::hasEqualOrLessPtx(صحيح major, صحيح minor)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(major);
    CV_استفادهنشده(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrLessPtx(major, minor);
#پایان_اگر
}

منطقي cv::cuda::هدف_Archs::hasEqualOrGreaterPtx(صحيح major, صحيح minor)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(major);
    CV_استفادهنشده(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrGreaterPtx(major, minor);
#پایان_اگر
}

منطقي cv::cuda::هدف_Archs::hasEqualOrGreaterBin(صحيح major, صحيح minor)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(major);
    CV_استفادهنشده(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrGreaterBin(major, minor);
#پایان_اگر
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#اگر_تعریف_شده HAVE_CUDA

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
        صحيح شمار_2 = get_Cuda_Enabled_Device_شمار();

        إذا (شمار_2 > 0)
        {
            props_.resize(شمار_2);

            لكل (صحيح devID = 0; devID < شمار_2; ++devID)
            {
                cuda_امن_Call( cudaGetDeviceProperties(&props_[devID], devID) );
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

#پایان_اگر

ثابت حرف* cv::cuda::DeviceInfo::name() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->name;
#پایان_اگر
}

size_t cv::cuda::DeviceInfo::مجموع_Global_Mem() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->مجموع_Global_Mem;
#پایان_اگر
}

size_t cv::cuda::DeviceInfo::shared_Mem_در_هر_بلوک() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->shared_Mem_در_هر_بلوک;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::regs_در_هر_بلوک() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->regs_در_هر_بلوک;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::warp_اندازه() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->warp_اندازه;
#پایان_اگر
}

size_t cv::cuda::DeviceInfo::memPitch() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->memPitch;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::max_نخها_در_هر_بلوک() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->max_نخها_در_هر_بلوک;
#پایان_اگر
}

Vec3i cv::cuda::DeviceInfo::max_نخها_Dim() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->max_نخها_Dim);
#پایان_اگر
}

Vec3i cv::cuda::DeviceInfo::max_Grid_اندازه() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->max_Grid_اندازه);
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::clock_نرخ() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح clock_نرخ;
    cuda_امن_Call(cudaDeviceGetAttribute(&clock_نرخ, cuda_Dev_Attr_Clock_نرخ, device_id_));
    أرجع clock_نرخ;
#پایان_اگر
}

size_t cv::cuda::DeviceInfo::مجموع_Const_Mem() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->مجموع_Const_Mem;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::major_نسخه() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->major;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::minor_نسخه() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->minor;
#پایان_اگر
}

size_t cv::cuda::DeviceInfo::textureAlignment() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->textureAlignment;
#پایان_اگر
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->texturePitchAlignment;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::multi_Processor_شمار() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->multi_Processor_شمار;
#پایان_اگر
}

منطقي cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح kernelExecTimeoutEnabled;
    cuda_امن_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    أرجع kernelExecTimeoutEnabled != 0;
#پایان_اگر
}

منطقي cv::cuda::DeviceInfo::integrated() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->integrated != 0;
#پایان_اگر
}

منطقي cv::cuda::DeviceInfo::can_Map_Host_حافظه() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->can_Map_Host_حافظه != 0;
#پایان_اگر
}

DeviceInfo::Compute_نما cv::cuda::DeviceInfo::compute_نما() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    ساكن ثابت Compute_نما tbl[] =
    {
        Compute_نما_Default,
        Compute_نما_Exclusive,
        Compute_نما_Prohibited,
        Compute_نما_Exclusive_Process
    };

    صحيح compute_نما;
    cuda_امن_Call(cudaDeviceGetAttribute(&compute_نما, cuda_Dev_Attr_Compute_نما, device_id_));
    أرجع tbl[compute_نما];
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::maxTexture1D() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxTexture1D;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::maxTexture1DMipmap() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخه >= 5000
        أرجع deviceProps().get(device_id_)->maxTexture1DMipmap;
    #وإلا
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        أرجع 0;
    #پایان_اگر
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::maxTexture1DLinear() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخه >= 13000
        size_t max_عرض_درون_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cuda_امن_Call(cuda_Device_Get_Texture1_DLinear_Max_عرض(&max_عرض_درون_Elements, &fmtDesc, device_id_));
        أرجع max_عرض_درون_Elements;
    #وإلا
        أرجع deviceProps().get(device_id_)->maxTexture1DLinear;
    #پایان_اگر
#پایان_اگر
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#پایان_اگر
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخه >= 5000
        أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #وإلا
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        أرجع Vec2i();
    #پایان_اگر
#پایان_اگر
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#پایان_اگر
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#پایان_اگر
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::maxTextureCubemap() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxTextureCubemap;
#پایان_اگر
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#پایان_اگر
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#پایان_اگر
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::maxSurface1D() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxSurface1D;
#پایان_اگر
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#پایان_اگر
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#پایان_اگر
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#پایان_اگر
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::maxSurfaceCubemap() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxSurfaceCubemap;
#پایان_اگر
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#پایان_اگر
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->surfaceAlignment;
#پایان_اگر
}

منطقي cv::cuda::DeviceInfo::concurrentKernels() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->concurrentKernels != 0;
#پایان_اگر
}

منطقي cv::cuda::DeviceInfo::ECCEnabled() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->ECCEnabled != 0;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::pciBusID() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciBusID;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::pciDeviceID() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciDeviceID;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::pciDomainID() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciDomainID;
#پایان_اگر
}

منطقي cv::cuda::DeviceInfo::tccDriver() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->tccDriver != 0;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::async_Engine_شمار() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->async_Engine_شمار;
#پایان_اگر
}

منطقي cv::cuda::DeviceInfo::unifiedAddressing() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->unifiedAddressing != 0;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::حافظه_Clock_نرخ() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح حافظه_Clock_نرخ;
    cuda_امن_Call(cudaDeviceGetAttribute(&حافظه_Clock_نرخ, cuda_Dev_Attr_حافظه_Clock_نرخ, device_id_));
    أرجع حافظه_Clock_نرخ;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::حافظه_Bus_عرض() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->حافظه_Bus_عرض;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::l2_Cache_اندازه() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->l2_Cache_اندازه;
#پایان_اگر
}

صحيح cv::cuda::DeviceInfo::max_نخها_در_هر_Multi_Processor() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->max_نخها_در_هر_Multi_Processor;
#پایان_اگر
}

فراغ cv::cuda::DeviceInfo::query_حافظه(size_t& مجموع_حافظه, size_t& free_حافظه) ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(مجموع_حافظه);
    CV_استفادهنشده(free_حافظه);
    throw_no_cuda();
#وإلا
    صحيح prevDeviceID = getDevice();
    إذا (prevDeviceID != device_id_)
        مجموعه_Device(device_id_);

    cuda_امن_Call( cudaMemGetInfo(&free_حافظه, &مجموع_حافظه) );

    إذا (prevDeviceID != device_id_)
        مجموعه_Device(prevDeviceID);
#پایان_اگر
}

منطقي cv::cuda::DeviceInfo::isCompatible() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    // Check PTX compatibility
    إذا (هدف_Archs::hasEqualOrLessPtx(major_نسخه(), minor_نسخه()))
        أرجع درست;

    // Check BIN compatibility
    لكل (صحيح i = minor_نسخه(); i >= 0; --i)
        إذا (هدف_Archs::hasBin(major_نسخه(), i))
            أرجع درست;

    أرجع خطأ;
#پایان_اگر
}

////////////////////////////////////////////////////////////////////////
// print info

#اگر_تعریف_شده HAVE_CUDA

نطاق
{
    صحيح convertSMVer2Cores(صحيح major, صحيح minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        تعريفنوع بنية {
            صحيح SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            صحيح Cores;
        } SMtoCores;

        SMtoCores gpu_Arch_Cores_در_هر_SM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        صحيح شاخص = 0;
        طالما (gpu_Arch_Cores_در_هر_SM[شاخص].SM != -1)
        {
            إذا (gpu_Arch_Cores_در_هر_SM[شاخص].SM == ((major << 4) + minor) )
                أرجع gpu_Arch_Cores_در_هر_SM[شاخص].Cores;
            شاخص++;
        }

        أرجع -1;
    }
}

#پایان_اگر

فراغ cv::cuda::printCudaDeviceInfo(صحيح device)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(device);
    throw_no_cuda();
#وإلا
    صحيح شمار_2 = get_Cuda_Enabled_Device_شمار();
    منطقي valid = (device >= 0) && (device < شمار_2);

    صحيح beg = valid ? device   : 0;
    صحيح end = valid ? device+1 : شمار_2;

    چاپ_کن("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    چاپ_کن("Device count: %d\n", شمار_2);

    صحيح driver_نسخه = 0, runtime_نسخه = 0;
    cuda_امن_Call( cuda_Driver_Get_نسخه(&driver_نسخه) );
    cuda_امن_Call( cuda_Runtime_Get_نسخه(&runtime_نسخه) );

    ثابت حرف *compute_نما[] = {
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
        cuda_امن_Call( cudaGetDeviceProperties(&prop, dev) );

        چاپ_کن("\nDevice %d: \"%s\"\n", dev, prop.name);
        چاپ_کن("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_نسخه/1000, driver_نسخه%100, runtime_نسخه/1000, runtime_نسخه%100);
        چاپ_کن("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        چاپ_کن("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (عائم)prop.مجموع_Global_Mem/1048576.0f, (بیعلامت دراز دراز) prop.مجموع_Global_Mem);

        صحيح cores = convertSMVer2Cores(prop.major, prop.minor);
        إذا (cores > 0)
            چاپ_کن("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_شمار, cores, cores * prop.multi_Processor_شمار);

        صحيح clock_نرخ;
        cuda_امن_Call(cudaDeviceGetAttribute(&clock_نرخ, cuda_Dev_Attr_Clock_نرخ, dev));
        چاپ_کن("  GPU Clock Speed:                               %.2f GHz\n", clock_نرخ * 1e-6f);

        چاپ_کن("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        چاپ_کن("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        چاپ_کن("  Total amount of constant memory:               %u bytes\n", (صحيح)prop.مجموع_Const_Mem);
        چاپ_کن("  Total amount of shared memory per block:       %u bytes\n", (صحيح)prop.shared_Mem_در_هر_بلوک);
        چاپ_کن("  Total number of registers available per block: %d\n", prop.regs_در_هر_بلوک);
        چاپ_کن("  Warp size:                                     %d\n", prop.warp_اندازه);
        چاپ_کن("  Maximum number of threads per block:           %d\n", prop.max_نخها_در_هر_بلوک);
        چاپ_کن("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_نخها_Dim[0], prop.max_نخها_Dim[1], prop.max_نخها_Dim[2]);
        چاپ_کن("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_اندازه[0], prop.max_Grid_اندازه[1],  prop.max_Grid_اندازه[2]);
        چاپ_کن("  Maximum memory pitch:                          %u bytes\n", (صحيح)prop.memPitch);
        چاپ_کن("  Texture alignment:                             %u bytes\n", (صحيح)prop.textureAlignment);

        چاپ_کن("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_شمار ? "Yes" : "No"), prop.async_Engine_شمار);
        صحيح kernelExecTimeoutEnabled;
        cuda_امن_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        چاپ_کن("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        چاپ_کن("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        چاپ_کن("  Support host page-locked memory mapping:       %s\n", prop.can_Map_Host_حافظه ? "Yes" : "No");

        چاپ_کن("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        چاپ_کن("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        چاپ_کن("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        چاپ_کن("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        چاپ_کن("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        چاپ_کن("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        صحيح prop_Compute_نما;
        cuda_امن_Call(cudaDeviceGetAttribute(&prop_Compute_نما, cuda_Dev_Attr_Compute_نما, dev));
        چاپ_کن("  Compute Mode:\n");
        چاپ_کن("      %s \n", compute_نما[prop_Compute_نما]);
    }

    چاپ_کن("\n");
    چاپ_کن("deviceQuery, CUDA Driver = CUDART");
    چاپ_کن(", CUDA Driver Version  = %d.%d", driver_نسخه / 1000, driver_نسخه % 100);
    چاپ_کن(", CUDA Runtime Version = %d.%d", runtime_نسخه/1000, runtime_نسخه%100);
    چاپ_کن(", NumDevs = %d\n\n", شمار_2);

    fflush(stdout);
#پایان_اگر
}

فراغ cv::cuda::printShortCudaDeviceInfo(صحيح device)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(device);
    throw_no_cuda();
#وإلا
    صحيح شمار_2 = get_Cuda_Enabled_Device_شمار();
    منطقي valid = (device >= 0) && (device < شمار_2);

    صحيح beg = valid ? device   : 0;
    صحيح end = valid ? device+1 : شمار_2;

    صحيح driver_نسخه = 0, runtime_نسخه = 0;
    cuda_امن_Call( cuda_Driver_Get_نسخه(&driver_نسخه) );
    cuda_امن_Call( cuda_Runtime_Get_نسخه(&runtime_نسخه) );

    لكل(صحيح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_امن_Call( cudaGetDeviceProperties(&prop, dev) );

        ثابت حرف *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        چاپ_کن("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (عائم)prop.مجموع_Global_Mem/1048576.0f);
        چاپ_کن(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        صحيح cores = convertSMVer2Cores(prop.major, prop.minor);
        إذا (cores > 0)
            چاپ_کن(", %d cores", cores * prop.multi_Processor_شمار);

        چاپ_کن(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_نسخه/1000, driver_نسخه%100, runtime_نسخه/1000, runtime_نسخه%100);
    }

    fflush(stdout);
#پایان_اگر
}

////////////////////////////////////////////////////////////////////////
// Error handling

#اگر_تعریف_شده HAVE_CUDA

نطاق
{
    #تعریف_کن error_مدخل(مدخل)  { مدخل, #entry }

    بنية Error_مدخل
    {
        صحيح کد;
        ثابت حرف* str;
    };

    بنية Error_مدخل_Comparer
    {
        صحيح کد;
        Error_مدخل_Comparer(صحيح کد_2) : کد(کد_2) {}
        منطقي عملگر()(ثابت Error_مدخل& e) ثابت { أرجع e.کد == کد; }
    };

    ثابت Error_مدخل npp_errors [] =
    {
    #إذا defined (_MSC_VER)
        error_مدخل( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #پایان_اگر

    #إذا NPP_نسخه < 5500
        error_مدخل( NPP_BAD_ARG_ERROR ),
        error_مدخل( NPP_COEFF_ERROR ),
        error_مدخل( NPP_RECT_ERROR ),
        error_مدخل( NPP_QUAD_ERROR ),
        error_مدخل( NPP_MEMFREE_ERR ),
        error_مدخل( NPP_MEMSET_ERR ),
        error_مدخل( NPP_MEM_ALLOC_ERR ),
        error_مدخل( NPP_HISTO_عدد_از_سطوح_ERROR ),
        error_مدخل( NPP_MIRROR_FLIP_ERR ),
        error_مدخل( NPP_INVALID_درون_داد ),
        error_مدخل( NPP_اشاره_گر_ERROR ),
        error_مدخل( NPP_WARNING ),
        error_مدخل( NPP_ODD_ROI_WARNING ),
    #وإلا
        error_مدخل( NPP_INVALID_HOST_اشاره_گر_ERROR ),
        error_مدخل( NPP_INVALID_DEVICE_اشاره_گر_ERROR ),
        error_مدخل( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_مدخل( NPP_ZC_نما_NOT_SUPPORTED_ERROR ),
        error_مدخل( NPP_MEMFREE_ERROR ),
        error_مدخل( NPP_MEMSET_ERROR ),
        error_مدخل( NPP_QUALITY_شاخص_ERROR ),
        error_مدخل( NPP_HISTOGRAM_عدد_از_سطوح_ERROR ),
        error_مدخل( NPP_CHANNEL_ترتیب_ERROR ),
        error_مدخل( NPP_ZERO_ماسک_مقدار_ERROR ),
        error_مدخل( NPP_QUADRANGLE_ERROR ),
        error_مدخل( NPP_RECTANGLE_ERROR ),
        error_مدخل( NPP_COEFFICIENT_ERROR ),
        error_مدخل( NPP_عدد_از_CHANNELS_ERROR ),
        error_مدخل( NPP_COI_ERROR ),
        error_مدخل( NPP_مقسوم_علیه_ERROR ),
        error_مدخل( NPP_CHANNEL_ERROR ),
        error_مدخل( NPP_STRIDE_ERROR ),
        error_مدخل( NPP_ANCHOR_ERROR ),
        error_مدخل( NPP_ماسک_اندازه_ERROR ),
        error_مدخل( NPP_MIRROR_FLIP_ERROR ),
        error_مدخل( NPP_MOMENT_00_ZERO_ERROR ),
        error_مدخل( NPP_آستانه_NEGATIVE_LEVEL_ERROR ),
        error_مدخل( NPP_آستانه_ERROR ),
        error_مدخل( NPP_CONTEXT_تطابق_ERROR ),
        error_مدخل( NPP_FFT_FLAG_ERROR ),
        error_مدخل( NPP_FFT_ترتیب_ERROR ),
        error_مدخل( NPP_SCALE_RANGE_ERROR ),
        error_مدخل( NPP_داده_TYPE_ERROR ),
        error_مدخل( NPP_بیرون_OFF_RANGE_ERROR ),
        error_مدخل( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_مدخل( NPP_حافظه_تخصیص_ERR ),
        error_مدخل( NPP_RANGE_ERROR ),
        error_مدخل( NPP_BAD_آرگومان_ERROR ),
        error_مدخل( NPP_NO_حافظه_ERROR ),
        error_مدخل( NPP_ERROR_RESERVED ),
        error_مدخل( NPP_NO_OPERATION_WARNING ),
        error_مدخل( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_مدخل( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #پایان_اگر

        error_مدخل( NPP_NOT_SUPPORTED_نما_ERROR ),
        error_مدخل( NPP_دور_نما_NOT_SUPPORTED_ERROR ),
        error_مدخل( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_مدخل( NPP_LUT_عدد_از_سطوح_ERROR ),
        error_مدخل( NPP_TEXTURE_BIND_ERROR ),
        error_مدخل( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_مدخل( NPP_NOT_EVEN_گام_ERROR ),
        error_مدخل( NPP_INTERPOLATION_ERROR ),
        error_مدخل( NPP_RESIZE_ضریب_ERROR ),
        error_مدخل( NPP_HAAR_CLASSIFIER_PIXEL_تطابق_ERROR ),
        error_مدخل( NPP_MEMCPY_ERROR ),
        error_مدخل( NPP_ALIGNMENT_ERROR ),
        error_مدخل( NPP_گام_ERROR ),
        error_مدخل( NPP_اندازه_ERROR ),
        error_مدخل( NPP_NULL_اشاره_گر_ERROR ),
        error_مدخل( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_مدخل( NPP_NOT_IMPLEMENTED_ERROR ),
        error_مدخل( NPP_ERROR ),
        error_مدخل( NPP_NO_ERROR ),
        error_مدخل( NPP_SUCCESS ),
        error_مدخل( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_مدخل( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_مدخل( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_مدخل( NPP_DOUBLE_اندازه_WARNING )
    };

    ثابت size_t npp_error_رقم = حجم(npp_errors) / حجم(npp_errors[0]);

    ثابت Error_مدخل cu_errors [] =
    {
        error_مدخل( CUDA_SUCCESS                              ),
        error_مدخل( CUDA_ERROR_INVALID_مقدار                  ),
        error_مدخل( CUDA_ERROR_بیرون_از_حافظه                  ),
        error_مدخل( CUDA_ERROR_NOT_INITIALIZED                ),
        error_مدخل( CUDA_ERROR_DEINITIALIZED                  ),
        error_مدخل( CUDA_ERROR_PROFILER_DISABLED              ),
        error_مدخل( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_مدخل( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_مدخل( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_مدخل( CUDA_ERROR_NO_DEVICE                      ),
        error_مدخل( CUDA_ERROR_INVALID_DEVICE                 ),
        error_مدخل( CUDA_ERROR_INVALID_IMAGE                  ),
        error_مدخل( CUDA_ERROR_INVALID_CONTEXT                ),
        error_مدخل( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_مدخل( CUDA_ERROR_MAP_FAILED                     ),
        error_مدخل( CUDA_ERROR_UNMAP_FAILED                   ),
        error_مدخل( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_مدخل( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_مدخل( CUDA_ERROR_NO_دودویی_FOR_GPU              ),
        error_مدخل( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_مدخل( CUDA_ERROR_NOT_MAPPED                     ),
        error_مدخل( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_مدخل( CUDA_ERROR_NOT_MAPPED_AS_اشاره_گر          ),
        error_مدخل( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_مدخل( CUDA_ERROR_UNSUPPORTED_حد              ),
        error_مدخل( CUDA_ERROR_CONTEXT_ALREADY_درون_USE         ),
        error_مدخل( CUDA_ERROR_INVALID_منبع                 ),
        error_مدخل( CUDA_ERROR_FILE_NOT_یافت                 ),
        error_مدخل( CUDA_ERROR_SHARED_OBJECT_نماد_NOT_یافت ),
        error_مدخل( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_مدخل( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_مدخل( CUDA_ERROR_INVALID_HANDLE                 ),
        error_مدخل( CUDA_ERROR_NOT_یافت                      ),
        error_مدخل( CUDA_ERROR_NOT_READY                      ),
        error_مدخل( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_مدخل( CUDA_ERROR_LAUNCH_بیرون_از_RESOURCES        ),
        error_مدخل( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_مدخل( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_مدخل( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_مدخل( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_مدخل( CUDA_ERROR_PRIMARY_CONTEXT_فعال         ),
        error_مدخل( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_مدخل( CUDA_ERROR_ASSERT                         ),
        error_مدخل( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_مدخل( CUDA_ERROR_HOST_حافظه_ALREADY_REGISTERED ),
        error_مدخل( CUDA_ERROR_HOST_حافظه_NOT_REGISTERED     ),
        error_مدخل( CUDA_ERROR_UNKNOWN                        )
    };

    ثابت size_t cu_errors_رقم = حجم(cu_errors) / حجم(cu_errors[0]);

    cv::String getErrorString(صحيح کد, ثابت Error_مدخل* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_مدخل_Comparer(کد)) - errors;

        ثابت حرف* پیام = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", پیام, کد);

        أرجع str;
    }
}

#پایان_اگر

String cv::cuda::get_Npp_Error_پیام(صحيح کد)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(کد);
    أرجع String();
#وإلا
    أرجع getErrorString(کد, npp_errors, npp_error_رقم);
#پایان_اگر
}

String cv::cuda::get_Cuda_Driver_Api_Error_پیام(صحيح کد)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(کد);
    أرجع String();
#وإلا
    أرجع getErrorString(کد, cu_errors, cu_errors_رقم);
#پایان_اگر
}
