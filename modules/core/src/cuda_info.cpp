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

کارول نطاق cv;
کارول نطاق cv::cuda;

صحيح cv::cuda::get_Cuda_Enabled_Device_شمېر()
{
#که_نه_تعریف_شوی HAVE_CUDA
    أرجع 0;
#وإلا
    صحيح شمېر_2;
    cudaError_t error = cuda_Get_Device_شمېر(&شمېر_2);

    إذا (error == cudaErrorInsufficientDriver)
        أرجع -1;

    إذا (error == cudaErrorNoDevice)
        أرجع 0;

    cuda_خوندي_Call( error );
    أرجع شمېر_2;
#د_که_پای
}

فراغ cv::cuda::سېټ_Device(صحيح device)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(device);
    throw_no_cuda();
#وإلا
    cuda_خوندي_Call( cuda_سېټ_Device(device) );
    cuda_خوندي_Call( cudaFree(0) );
#د_که_پای
}

صحيح cv::cuda::getDevice()
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح device;
    cuda_خوندي_Call( cudaGetDevice(&device) );
    أرجع device;
#د_که_پای
}

فراغ cv::cuda::resetDevice()
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_خوندي_Call( cudaDeviceReset() );
#د_که_پای
}

منطقي cv::cuda::deviceSupports(Feature_سېټ feature_سېټ)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(feature_سېټ);
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

    أرجع موخه_Archs::builtWith(feature_سېټ) && (نسخه >= feature_سېټ);
#د_که_پای
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#که_تعریف_شوی HAVE_CUDA

نطاق
{
    صنف CudaArch
    {
    عام:
        CudaArch();

        منطقي builtWith(Feature_سېټ feature_سېټ) ثابت;
        منطقي hasPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasBin(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrLessPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrGreaterPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrGreaterBin(صحيح major, صحيح minor) ثابت;

    خاص:
        ساكن فراغ fromStr(ثابت حرف* سېټ_as_str, std::vector<صحيح>& arr);

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

    منطقي CudaArch::builtWith(Feature_سېټ feature_سېټ) ثابت
    {
        أرجع !features.empty() && (features.back() >= feature_سېټ);
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

    فراغ CudaArch::fromStr(ثابت حرف* سېټ_as_str, std::vector<صحيح>& arr)
    {
        arr.clear();

        ثابت size_t len = مزي_اوږدوالی(سېټ_as_str);

        size_t pos = 0;
        طالما (pos < len)
        {
            إذا (که_تشه(سېټ_as_str[pos]))
            {
                ++pos;
            }
            وإلا
            {
                صحيح cur_ارزښت;
                صحيح chars_read;
                صحيح دلیلونه_read = له_مزي_ولوله(سېټ_as_str + pos, "%d%n", &cur_ارزښت, &chars_read);
                CV_Assert( دلیلونه_read == 1 );

                arr.push_back(cur_ارزښت);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#د_که_پای

منطقي cv::cuda::موخه_Archs::builtWith(cv::cuda::Feature_سېټ feature_سېټ)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(feature_سېټ);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.builtWith(feature_سېټ);
#د_که_پای
}

منطقي cv::cuda::موخه_Archs::hasPtx(صحيح major, صحيح minor)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(major);
    CV_ناکارول_شوی(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasPtx(major, minor);
#د_که_پای
}

منطقي cv::cuda::موخه_Archs::hasBin(صحيح major, صحيح minor)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(major);
    CV_ناکارول_شوی(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasBin(major, minor);
#د_که_پای
}

منطقي cv::cuda::موخه_Archs::hasEqualOrLessPtx(صحيح major, صحيح minor)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(major);
    CV_ناکارول_شوی(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrLessPtx(major, minor);
#د_که_پای
}

منطقي cv::cuda::موخه_Archs::hasEqualOrGreaterPtx(صحيح major, صحيح minor)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(major);
    CV_ناکارول_شوی(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrGreaterPtx(major, minor);
#د_که_پای
}

منطقي cv::cuda::موخه_Archs::hasEqualOrGreaterBin(صحيح major, صحيح minor)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(major);
    CV_ناکارول_شوی(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrGreaterBin(major, minor);
#د_که_پای
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#که_تعریف_شوی HAVE_CUDA

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
        صحيح شمېر_2 = get_Cuda_Enabled_Device_شمېر();

        إذا (شمېر_2 > 0)
        {
            props_.resize(شمېر_2);

            لكل (صحيح devID = 0; devID < شمېر_2; ++devID)
            {
                cuda_خوندي_Call( cudaGetDeviceProperties(&props_[devID], devID) );
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

#د_که_پای

ثابت حرف* cv::cuda::DeviceInfo::name() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->name;
#د_که_پای
}

size_t cv::cuda::DeviceInfo::ټول_Global_Mem() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->ټول_Global_Mem;
#د_که_پای
}

size_t cv::cuda::DeviceInfo::shared_Mem_فی_بلاک() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->shared_Mem_فی_بلاک;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::regs_فی_بلاک() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->regs_فی_بلاک;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::warp_اندازه() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->warp_اندازه;
#د_که_پای
}

size_t cv::cuda::DeviceInfo::memPitch() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->memPitch;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::max_تارونه_فی_بلاک() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->max_تارونه_فی_بلاک;
#د_که_پای
}

Vec3i cv::cuda::DeviceInfo::max_تارونه_Dim() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->max_تارونه_Dim);
#د_که_پای
}

Vec3i cv::cuda::DeviceInfo::max_Grid_اندازه() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->max_Grid_اندازه);
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::clock_شرح() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح clock_شرح;
    cuda_خوندي_Call(cudaDeviceGetAttribute(&clock_شرح, cuda_Dev_Attr_Clock_شرح, device_id_));
    أرجع clock_شرح;
#د_که_پای
}

size_t cv::cuda::DeviceInfo::ټول_Const_Mem() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->ټول_Const_Mem;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::major_نسخه() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->major;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::minor_نسخه() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->minor;
#د_که_پای
}

size_t cv::cuda::DeviceInfo::textureAlignment() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->textureAlignment;
#د_که_پای
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->texturePitchAlignment;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::multi_Processor_شمېر() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->multi_Processor_شمېر;
#د_که_پای
}

منطقي cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح kernelExecTimeoutEnabled;
    cuda_خوندي_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    أرجع kernelExecTimeoutEnabled != 0;
#د_که_پای
}

منطقي cv::cuda::DeviceInfo::integrated() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->integrated != 0;
#د_که_پای
}

منطقي cv::cuda::DeviceInfo::can_Map_Host_حافظه() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->can_Map_Host_حافظه != 0;
#د_که_پای
}

DeviceInfo::Compute_ډېرپېښ cv::cuda::DeviceInfo::compute_ډېرپېښ() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    ساكن ثابت Compute_ډېرپېښ tbl[] =
    {
        Compute_ډېرپېښ_Default,
        Compute_ډېرپېښ_Exclusive,
        Compute_ډېرپېښ_Prohibited,
        Compute_ډېرپېښ_Exclusive_Process
    };

    صحيح compute_ډېرپېښ;
    cuda_خوندي_Call(cudaDeviceGetAttribute(&compute_ډېرپېښ, cuda_Dev_Attr_Compute_ډېرپېښ, device_id_));
    أرجع tbl[compute_ډېرپېښ];
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::maxTexture1D() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxTexture1D;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::maxTexture1DMipmap() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخه >= 5000
        أرجع deviceProps().get(device_id_)->maxTexture1DMipmap;
    #وإلا
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        أرجع 0;
    #د_که_پای
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::maxTexture1DLinear() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخه >= 13000
        size_t max_پلنوالی_دننه_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cuda_خوندي_Call(cuda_Device_Get_Texture1_DLinear_Max_پلنوالی(&max_پلنوالی_دننه_Elements, &fmtDesc, device_id_));
        أرجع max_پلنوالی_دننه_Elements;
    #وإلا
        أرجع deviceProps().get(device_id_)->maxTexture1DLinear;
    #د_که_پای
#د_که_پای
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#د_که_پای
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخه >= 5000
        أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #وإلا
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        أرجع Vec2i();
    #د_که_پای
#د_که_پای
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#د_که_پای
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#د_که_پای
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::maxTextureCubemap() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxTextureCubemap;
#د_که_پای
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#د_که_پای
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#د_که_پای
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::maxSurface1D() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxSurface1D;
#د_که_پای
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#د_که_پای
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#د_که_پای
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#د_که_پای
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::maxSurfaceCubemap() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxSurfaceCubemap;
#د_که_پای
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#د_که_پای
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->surfaceAlignment;
#د_که_پای
}

منطقي cv::cuda::DeviceInfo::concurrentKernels() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->concurrentKernels != 0;
#د_که_پای
}

منطقي cv::cuda::DeviceInfo::ECCEnabled() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->ECCEnabled != 0;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::pciBusID() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciBusID;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::pciDeviceID() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciDeviceID;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::pciDomainID() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciDomainID;
#د_که_پای
}

منطقي cv::cuda::DeviceInfo::tccDriver() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->tccDriver != 0;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::async_Engine_شمېر() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->async_Engine_شمېر;
#د_که_پای
}

منطقي cv::cuda::DeviceInfo::unifiedAddressing() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->unifiedAddressing != 0;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::حافظه_Clock_شرح() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح حافظه_Clock_شرح;
    cuda_خوندي_Call(cudaDeviceGetAttribute(&حافظه_Clock_شرح, cuda_Dev_Attr_حافظه_Clock_شرح, device_id_));
    أرجع حافظه_Clock_شرح;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::حافظه_Bus_پلنوالی() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->حافظه_Bus_پلنوالی;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::l2_Cache_اندازه() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->l2_Cache_اندازه;
#د_که_پای
}

صحيح cv::cuda::DeviceInfo::max_تارونه_فی_Multi_Processor() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->max_تارونه_فی_Multi_Processor;
#د_که_پای
}

فراغ cv::cuda::DeviceInfo::query_حافظه(size_t& ټول_حافظه, size_t& free_حافظه) ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(ټول_حافظه);
    CV_ناکارول_شوی(free_حافظه);
    throw_no_cuda();
#وإلا
    صحيح prevDeviceID = getDevice();
    إذا (prevDeviceID != device_id_)
        سېټ_Device(device_id_);

    cuda_خوندي_Call( cudaMemGetInfo(&free_حافظه, &ټول_حافظه) );

    إذا (prevDeviceID != device_id_)
        سېټ_Device(prevDeviceID);
#د_که_پای
}

منطقي cv::cuda::DeviceInfo::isCompatible() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    // Check PTX compatibility
    إذا (موخه_Archs::hasEqualOrLessPtx(major_نسخه(), minor_نسخه()))
        أرجع ریښتیا;

    // Check BIN compatibility
    لكل (صحيح i = minor_نسخه(); i >= 0; --i)
        إذا (موخه_Archs::hasBin(major_نسخه(), i))
            أرجع ریښتیا;

    أرجع خطأ;
#د_که_پای
}

////////////////////////////////////////////////////////////////////////
// print info

#که_تعریف_شوی HAVE_CUDA

نطاق
{
    صحيح convertSMVer2Cores(صحيح major, صحيح minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        تعريفنوع بنية {
            صحيح SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            صحيح Cores;
        } SMtoCores;

        SMtoCores gpu_Arch_Cores_فی_SM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        صحيح شاخص = 0;
        طالما (gpu_Arch_Cores_فی_SM[شاخص].SM != -1)
        {
            إذا (gpu_Arch_Cores_فی_SM[شاخص].SM == ((major << 4) + minor) )
                أرجع gpu_Arch_Cores_فی_SM[شاخص].Cores;
            شاخص++;
        }

        أرجع -1;
    }
}

#د_که_پای

فراغ cv::cuda::printCudaDeviceInfo(صحيح device)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(device);
    throw_no_cuda();
#وإلا
    صحيح شمېر_2 = get_Cuda_Enabled_Device_شمېر();
    منطقي valid = (device >= 0) && (device < شمېر_2);

    صحيح beg = valid ? device   : 0;
    صحيح end = valid ? device+1 : شمېر_2;

    چاپ_کړه("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    چاپ_کړه("Device count: %d\n", شمېر_2);

    صحيح driver_نسخه = 0, runtime_نسخه = 0;
    cuda_خوندي_Call( cuda_Driver_Get_نسخه(&driver_نسخه) );
    cuda_خوندي_Call( cuda_Runtime_Get_نسخه(&runtime_نسخه) );

    ثابت حرف *compute_ډېرپېښ[] = {
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
        cuda_خوندي_Call( cudaGetDeviceProperties(&prop, dev) );

        چاپ_کړه("\nDevice %d: \"%s\"\n", dev, prop.name);
        چاپ_کړه("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_نسخه/1000, driver_نسخه%100, runtime_نسخه/1000, runtime_نسخه%100);
        چاپ_کړه("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        چاپ_کړه("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (عائم)prop.ټول_Global_Mem/1048576.0f, (بینښه اوږد اوږد) prop.ټول_Global_Mem);

        صحيح cores = convertSMVer2Cores(prop.major, prop.minor);
        إذا (cores > 0)
            چاپ_کړه("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_شمېر, cores, cores * prop.multi_Processor_شمېر);

        صحيح clock_شرح;
        cuda_خوندي_Call(cudaDeviceGetAttribute(&clock_شرح, cuda_Dev_Attr_Clock_شرح, dev));
        چاپ_کړه("  GPU Clock Speed:                               %.2f GHz\n", clock_شرح * 1e-6f);

        چاپ_کړه("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        چاپ_کړه("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        چاپ_کړه("  Total amount of constant memory:               %u bytes\n", (صحيح)prop.ټول_Const_Mem);
        چاپ_کړه("  Total amount of shared memory per block:       %u bytes\n", (صحيح)prop.shared_Mem_فی_بلاک);
        چاپ_کړه("  Total number of registers available per block: %d\n", prop.regs_فی_بلاک);
        چاپ_کړه("  Warp size:                                     %d\n", prop.warp_اندازه);
        چاپ_کړه("  Maximum number of threads per block:           %d\n", prop.max_تارونه_فی_بلاک);
        چاپ_کړه("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_تارونه_Dim[0], prop.max_تارونه_Dim[1], prop.max_تارونه_Dim[2]);
        چاپ_کړه("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_اندازه[0], prop.max_Grid_اندازه[1],  prop.max_Grid_اندازه[2]);
        چاپ_کړه("  Maximum memory pitch:                          %u bytes\n", (صحيح)prop.memPitch);
        چاپ_کړه("  Texture alignment:                             %u bytes\n", (صحيح)prop.textureAlignment);

        چاپ_کړه("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_شمېر ? "Yes" : "No"), prop.async_Engine_شمېر);
        صحيح kernelExecTimeoutEnabled;
        cuda_خوندي_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        چاپ_کړه("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        چاپ_کړه("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        چاپ_کړه("  Support host page-locked memory mapping:       %s\n", prop.can_Map_Host_حافظه ? "Yes" : "No");

        چاپ_کړه("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        چاپ_کړه("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        چاپ_کړه("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        چاپ_کړه("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        چاپ_کړه("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        چاپ_کړه("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        صحيح prop_Compute_ډېرپېښ;
        cuda_خوندي_Call(cudaDeviceGetAttribute(&prop_Compute_ډېرپېښ, cuda_Dev_Attr_Compute_ډېرپېښ, dev));
        چاپ_کړه("  Compute Mode:\n");
        چاپ_کړه("      %s \n", compute_ډېرپېښ[prop_Compute_ډېرپېښ]);
    }

    چاپ_کړه("\n");
    چاپ_کړه("deviceQuery, CUDA Driver = CUDART");
    چاپ_کړه(", CUDA Driver Version  = %d.%d", driver_نسخه / 1000, driver_نسخه % 100);
    چاپ_کړه(", CUDA Runtime Version = %d.%d", runtime_نسخه/1000, runtime_نسخه%100);
    چاپ_کړه(", NumDevs = %d\n\n", شمېر_2);

    fflush(stdout);
#د_که_پای
}

فراغ cv::cuda::printShortCudaDeviceInfo(صحيح device)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(device);
    throw_no_cuda();
#وإلا
    صحيح شمېر_2 = get_Cuda_Enabled_Device_شمېر();
    منطقي valid = (device >= 0) && (device < شمېر_2);

    صحيح beg = valid ? device   : 0;
    صحيح end = valid ? device+1 : شمېر_2;

    صحيح driver_نسخه = 0, runtime_نسخه = 0;
    cuda_خوندي_Call( cuda_Driver_Get_نسخه(&driver_نسخه) );
    cuda_خوندي_Call( cuda_Runtime_Get_نسخه(&runtime_نسخه) );

    لكل(صحيح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_خوندي_Call( cudaGetDeviceProperties(&prop, dev) );

        ثابت حرف *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        چاپ_کړه("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (عائم)prop.ټول_Global_Mem/1048576.0f);
        چاپ_کړه(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        صحيح cores = convertSMVer2Cores(prop.major, prop.minor);
        إذا (cores > 0)
            چاپ_کړه(", %d cores", cores * prop.multi_Processor_شمېر);

        چاپ_کړه(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_نسخه/1000, driver_نسخه%100, runtime_نسخه/1000, runtime_نسخه%100);
    }

    fflush(stdout);
#د_که_پای
}

////////////////////////////////////////////////////////////////////////
// Error handling

#که_تعریف_شوی HAVE_CUDA

نطاق
{
    #تعریف_کړه error_ثبت(ثبت)  { ثبت, #entry }

    بنية Error_ثبت
    {
        صحيح کوډ;
        ثابت حرف* str;
    };

    بنية Error_ثبت_Comparer
    {
        صحيح کوډ;
        Error_ثبت_Comparer(صحيح کوډ_2) : کوډ(کوډ_2) {}
        منطقي چلوونکی()(ثابت Error_ثبت& e) ثابت { أرجع e.کوډ == کوډ; }
    };

    ثابت Error_ثبت npp_errors [] =
    {
    #إذا defined (_MSC_VER)
        error_ثبت( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #د_که_پای

    #إذا NPP_نسخه < 5500
        error_ثبت( NPP_BAD_ARG_ERROR ),
        error_ثبت( NPP_COEFF_ERROR ),
        error_ثبت( NPP_RECT_ERROR ),
        error_ثبت( NPP_QUAD_ERROR ),
        error_ثبت( NPP_MEMFREE_ERR ),
        error_ثبت( NPP_MEMSET_ERR ),
        error_ثبت( NPP_MEM_ALLOC_ERR ),
        error_ثبت( NPP_HISTO_شمېره_د_پوړونه_ERROR ),
        error_ثبت( NPP_MIRROR_FLIP_ERR ),
        error_ثبت( NPP_INVALID_ننوت ),
        error_ثبت( NPP_اشاره_ګر_ERROR ),
        error_ثبت( NPP_WARNING ),
        error_ثبت( NPP_ODD_ROI_WARNING ),
    #وإلا
        error_ثبت( NPP_INVALID_HOST_اشاره_ګر_ERROR ),
        error_ثبت( NPP_INVALID_DEVICE_اشاره_ګر_ERROR ),
        error_ثبت( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_ثبت( NPP_ZC_ډېرپېښ_NOT_SUPPORTED_ERROR ),
        error_ثبت( NPP_MEMFREE_ERROR ),
        error_ثبت( NPP_MEMSET_ERROR ),
        error_ثبت( NPP_QUALITY_شاخص_ERROR ),
        error_ثبت( NPP_HISTOGRAM_شمېره_د_پوړونه_ERROR ),
        error_ثبت( NPP_CHANNEL_ترتیب_ERROR ),
        error_ثبت( NPP_ZERO_ماسک_ارزښت_ERROR ),
        error_ثبت( NPP_QUADRANGLE_ERROR ),
        error_ثبت( NPP_RECTANGLE_ERROR ),
        error_ثبت( NPP_COEFFICIENT_ERROR ),
        error_ثبت( NPP_شمېره_د_CHANNELS_ERROR ),
        error_ثبت( NPP_COI_ERROR ),
        error_ثبت( NPP_وېشونکی_ERROR ),
        error_ثبت( NPP_CHANNEL_ERROR ),
        error_ثبت( NPP_STRIDE_ERROR ),
        error_ثبت( NPP_ANCHOR_ERROR ),
        error_ثبت( NPP_ماسک_اندازه_ERROR ),
        error_ثبت( NPP_MIRROR_FLIP_ERROR ),
        error_ثبت( NPP_MOMENT_00_ZERO_ERROR ),
        error_ثبت( NPP_پوله_NEGATIVE_LEVEL_ERROR ),
        error_ثبت( NPP_پوله_ERROR ),
        error_ثبت( NPP_CONTEXT_سمون_ERROR ),
        error_ثبت( NPP_FFT_FLAG_ERROR ),
        error_ثبت( NPP_FFT_ترتیب_ERROR ),
        error_ثبت( NPP_SCALE_RANGE_ERROR ),
        error_ثبت( NPP_معلومات_TYPE_ERROR ),
        error_ثبت( NPP_بهر_OFF_RANGE_ERROR ),
        error_ثبت( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_ثبت( NPP_حافظه_ویش_ERR ),
        error_ثبت( NPP_RANGE_ERROR ),
        error_ثبت( NPP_BAD_دلیل_ERROR ),
        error_ثبت( NPP_NO_حافظه_ERROR ),
        error_ثبت( NPP_ERROR_RESERVED ),
        error_ثبت( NPP_NO_OPERATION_WARNING ),
        error_ثبت( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_ثبت( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #د_که_پای

        error_ثبت( NPP_NOT_SUPPORTED_ډېرپېښ_ERROR ),
        error_ثبت( NPP_پړاو_ډېرپېښ_NOT_SUPPORTED_ERROR ),
        error_ثبت( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_ثبت( NPP_LUT_شمېره_د_پوړونه_ERROR ),
        error_ثبت( NPP_TEXTURE_BIND_ERROR ),
        error_ثبت( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_ثبت( NPP_NOT_EVEN_ګام_ERROR ),
        error_ثبت( NPP_INTERPOLATION_ERROR ),
        error_ثبت( NPP_RESIZE_ضریب_ERROR ),
        error_ثبت( NPP_HAAR_CLASSIFIER_PIXEL_سمون_ERROR ),
        error_ثبت( NPP_MEMCPY_ERROR ),
        error_ثبت( NPP_ALIGNMENT_ERROR ),
        error_ثبت( NPP_ګام_ERROR ),
        error_ثبت( NPP_اندازه_ERROR ),
        error_ثبت( NPP_NULL_اشاره_ګر_ERROR ),
        error_ثبت( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_ثبت( NPP_NOT_IMPLEMENTED_ERROR ),
        error_ثبت( NPP_ERROR ),
        error_ثبت( NPP_NO_ERROR ),
        error_ثبت( NPP_SUCCESS ),
        error_ثبت( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_ثبت( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_ثبت( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_ثبت( NPP_DOUBLE_اندازه_WARNING )
    };

    ثابت size_t npp_error_عدد = حجم(npp_errors) / حجم(npp_errors[0]);

    ثابت Error_ثبت cu_errors [] =
    {
        error_ثبت( CUDA_SUCCESS                              ),
        error_ثبت( CUDA_ERROR_INVALID_ارزښت                  ),
        error_ثبت( CUDA_ERROR_بهر_د_حافظه                  ),
        error_ثبت( CUDA_ERROR_NOT_INITIALIZED                ),
        error_ثبت( CUDA_ERROR_DEINITIALIZED                  ),
        error_ثبت( CUDA_ERROR_PROFILER_DISABLED              ),
        error_ثبت( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_ثبت( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_ثبت( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_ثبت( CUDA_ERROR_NO_DEVICE                      ),
        error_ثبت( CUDA_ERROR_INVALID_DEVICE                 ),
        error_ثبت( CUDA_ERROR_INVALID_IMAGE                  ),
        error_ثبت( CUDA_ERROR_INVALID_CONTEXT                ),
        error_ثبت( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_ثبت( CUDA_ERROR_MAP_FAILED                     ),
        error_ثبت( CUDA_ERROR_UNMAP_FAILED                   ),
        error_ثبت( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_ثبت( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_ثبت( CUDA_ERROR_NO_دوګونی_FOR_GPU              ),
        error_ثبت( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_ثبت( CUDA_ERROR_NOT_MAPPED                     ),
        error_ثبت( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_ثبت( CUDA_ERROR_NOT_MAPPED_AS_اشاره_ګر          ),
        error_ثبت( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_ثبت( CUDA_ERROR_UNSUPPORTED_حد              ),
        error_ثبت( CUDA_ERROR_CONTEXT_ALREADY_دننه_USE         ),
        error_ثبت( CUDA_ERROR_INVALID_سرچینه                 ),
        error_ثبت( CUDA_ERROR_FILE_NOT_وموندل                 ),
        error_ثبت( CUDA_ERROR_SHARED_OBJECT_نښه_NOT_وموندل ),
        error_ثبت( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_ثبت( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_ثبت( CUDA_ERROR_INVALID_HANDLE                 ),
        error_ثبت( CUDA_ERROR_NOT_وموندل                      ),
        error_ثبت( CUDA_ERROR_NOT_READY                      ),
        error_ثبت( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_ثبت( CUDA_ERROR_LAUNCH_بهر_د_RESOURCES        ),
        error_ثبت( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_ثبت( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_ثبت( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_ثبت( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_ثبت( CUDA_ERROR_PRIMARY_CONTEXT_فعال         ),
        error_ثبت( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_ثبت( CUDA_ERROR_ASSERT                         ),
        error_ثبت( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_ثبت( CUDA_ERROR_HOST_حافظه_ALREADY_REGISTERED ),
        error_ثبت( CUDA_ERROR_HOST_حافظه_NOT_REGISTERED     ),
        error_ثبت( CUDA_ERROR_UNKNOWN                        )
    };

    ثابت size_t cu_errors_عدد = حجم(cu_errors) / حجم(cu_errors[0]);

    cv::String getErrorString(صحيح کوډ, ثابت Error_ثبت* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_ثبت_Comparer(کوډ)) - errors;

        ثابت حرف* پیغام = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", پیغام, کوډ);

        أرجع str;
    }
}

#د_که_پای

String cv::cuda::get_Npp_Error_پیغام(صحيح کوډ)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(کوډ);
    أرجع String();
#وإلا
    أرجع getErrorString(کوډ, npp_errors, npp_error_عدد);
#د_که_پای
}

String cv::cuda::get_Cuda_Driver_Api_Error_پیغام(صحيح کوډ)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(کوډ);
    أرجع String();
#وإلا
    أرجع getErrorString(کوډ, cu_errors, cu_errors_عدد);
#د_که_پای
}
