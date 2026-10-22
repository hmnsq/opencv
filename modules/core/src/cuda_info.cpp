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

استعمال نامخلا cv;
استعمال نامخلا cv::cuda;

صحیح cv::cuda::get_Cuda_Enabled_Device_گنتی()
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    واپس 0;
#ورنہ
    صحیح گنتی_2;
    cudaError_t error = cuda_Get_Device_گنتی(&گنتی_2);

    اگر (error == cudaErrorInsufficientDriver)
        واپس -1;

    اگر (error == cudaErrorNoDevice)
        واپس 0;

    cuda_محفوظ_Call( error );
    واپس گنتی_2;
#اگر_ختم
}

خالی cv::cuda::سیٹ_Device(صحیح device)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(device);
    throw_no_cuda();
#ورنہ
    cuda_محفوظ_Call( cuda_سیٹ_Device(device) );
    cuda_محفوظ_Call( cudaFree(0) );
#اگر_ختم
}

صحیح cv::cuda::getDevice()
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    صحیح device;
    cuda_محفوظ_Call( cudaGetDevice(&device) );
    واپس device;
#اگر_ختم
}

خالی cv::cuda::resetDevice()
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    cuda_محفوظ_Call( cudaDeviceReset() );
#اگر_ختم
}

منطقی cv::cuda::deviceSupports(Feature_سیٹ feature_سیٹ)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(feature_سیٹ);
    throw_no_cuda();
#ورنہ
    ساکن صحیح versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    ساکن ثابت صحیح cache_سائز = static_cast<صحیح>(حجم(versions) / حجم(versions[0]));

    ثابت صحیح devId = getDevice();

    صحیح نسخہ;

    اگر (devId < cache_سائز && versions[devId] >= 0)
    {
        نسخہ = versions[devId];
    }
    ورنہ
    {
        DeviceInfo dev(devId);
        نسخہ = dev.major_نسخہ() * 10 + dev.minor_نسخہ();
        اگر (devId < cache_سائز)
            versions[devId] = نسخہ;
    }

    واپس ہدف_Archs::builtWith(feature_سیٹ) && (نسخہ >= feature_سیٹ);
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا
{
    جماعت CudaArch
    {
    عوامی:
        CudaArch();

        منطقی builtWith(Feature_سیٹ feature_سیٹ) ثابت;
        منطقی hasPtx(صحیح major, صحیح minor) ثابت;
        منطقی hasBin(صحیح major, صحیح minor) ثابت;
        منطقی hasEqualOrLessPtx(صحیح major, صحیح minor) ثابت;
        منطقی hasEqualOrGreaterPtx(صحیح major, صحیح minor) ثابت;
        منطقی hasEqualOrGreaterBin(صحیح major, صحیح minor) ثابت;

    نجی:
        ساکن خالی fromStr(ثابت حرف* سیٹ_as_str, std::vector<صحیح>& arr);

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

    منطقی CudaArch::builtWith(Feature_سیٹ feature_سیٹ) ثابت
    {
        واپس !features.empty() && (features.back() >= feature_سیٹ);
    }

    منطقی CudaArch::hasPtx(صحیح major, صحیح minor) ثابت
    {
        واپس std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    منطقی CudaArch::hasBin(صحیح major, صحیح minor) ثابت
    {
        واپس std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    منطقی CudaArch::hasEqualOrLessPtx(صحیح major, صحیح minor) ثابت
    {
        واپس !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    منطقی CudaArch::hasEqualOrGreaterPtx(صحیح major, صحیح minor) ثابت
    {
        واپس !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    منطقی CudaArch::hasEqualOrGreaterBin(صحیح major, صحیح minor) ثابت
    {
        واپس !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    خالی CudaArch::fromStr(ثابت حرف* سیٹ_as_str, std::vector<صحیح>& arr)
    {
        arr.clear();

        ثابت size_t len = لڑی_لمبائی(سیٹ_as_str);

        size_t pos = 0;
        جب (pos < len)
        {
            اگر (خلا_ہے(سیٹ_as_str[pos]))
            {
                ++pos;
            }
            ورنہ
            {
                صحیح cur_قدر;
                صحیح chars_read;
                صحیح دلائل_read = لڑی_سے_پڑھو(سیٹ_as_str + pos, "%d%n", &cur_قدر, &chars_read);
                CV_Assert( دلائل_read == 1 );

                arr.push_back(cur_قدر);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#اگر_ختم

منطقی cv::cuda::ہدف_Archs::builtWith(cv::cuda::Feature_سیٹ feature_سیٹ)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(feature_سیٹ);
    throw_no_cuda();
#ورنہ
    واپس cudaArch.builtWith(feature_سیٹ);
#اگر_ختم
}

منطقی cv::cuda::ہدف_Archs::hasPtx(صحیح major, صحیح minor)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(major);
    CV_غیر_مستعمل(minor);
    throw_no_cuda();
#ورنہ
    واپس cudaArch.hasPtx(major, minor);
#اگر_ختم
}

منطقی cv::cuda::ہدف_Archs::hasBin(صحیح major, صحیح minor)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(major);
    CV_غیر_مستعمل(minor);
    throw_no_cuda();
#ورنہ
    واپس cudaArch.hasBin(major, minor);
#اگر_ختم
}

منطقی cv::cuda::ہدف_Archs::hasEqualOrLessPtx(صحیح major, صحیح minor)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(major);
    CV_غیر_مستعمل(minor);
    throw_no_cuda();
#ورنہ
    واپس cudaArch.hasEqualOrLessPtx(major, minor);
#اگر_ختم
}

منطقی cv::cuda::ہدف_Archs::hasEqualOrGreaterPtx(صحیح major, صحیح minor)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(major);
    CV_غیر_مستعمل(minor);
    throw_no_cuda();
#ورنہ
    واپس cudaArch.hasEqualOrGreaterPtx(major, minor);
#اگر_ختم
}

منطقی cv::cuda::ہدف_Archs::hasEqualOrGreaterBin(صحیح major, صحیح minor)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(major);
    CV_غیر_مستعمل(minor);
    throw_no_cuda();
#ورنہ
    واپس cudaArch.hasEqualOrGreaterBin(major, minor);
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا
{
    جماعت DeviceProps
    {
    عوامی:
        DeviceProps();

        ثابت cudaDeviceProp* get(صحیح devID) ثابت;

    نجی:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        صحیح گنتی_2 = get_Cuda_Enabled_Device_گنتی();

        اگر (گنتی_2 > 0)
        {
            props_.resize(گنتی_2);

            برائے (صحیح devID = 0; devID < گنتی_2; ++devID)
            {
                cuda_محفوظ_Call( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    ثابت cudaDeviceProp* DeviceProps::get(صحیح devID) ثابت
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        واپس &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        ساکن DeviceProps props;
        واپس props;
    }
}

#اگر_ختم

ثابت حرف* cv::cuda::DeviceInfo::name() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->name;
#اگر_ختم
}

size_t cv::cuda::DeviceInfo::کل_Global_Mem() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->کل_Global_Mem;
#اگر_ختم
}

size_t cv::cuda::DeviceInfo::shared_Mem_فی_بلاک() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->shared_Mem_فی_بلاک;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::regs_فی_بلاک() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->regs_فی_بلاک;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::warp_سائز() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->warp_سائز;
#اگر_ختم
}

size_t cv::cuda::DeviceInfo::memPitch() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->memPitch;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::max_دھاگے_فی_بلاک() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->max_دھاگے_فی_بلاک;
#اگر_ختم
}

Vec3i cv::cuda::DeviceInfo::max_دھاگے_Dim() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec3i(deviceProps().get(device_id_)->max_دھاگے_Dim);
#اگر_ختم
}

Vec3i cv::cuda::DeviceInfo::max_Grid_سائز() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec3i(deviceProps().get(device_id_)->max_Grid_سائز);
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::clock_شرح() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    صحیح clock_شرح;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&clock_شرح, cuda_Dev_Attr_Clock_شرح, device_id_));
    واپس clock_شرح;
#اگر_ختم
}

size_t cv::cuda::DeviceInfo::کل_Const_Mem() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->کل_Const_Mem;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::major_نسخہ() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->major;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::minor_نسخہ() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->minor;
#اگر_ختم
}

size_t cv::cuda::DeviceInfo::textureAlignment() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->textureAlignment;
#اگر_ختم
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->texturePitchAlignment;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::multi_Processor_گنتی() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->multi_Processor_گنتی;
#اگر_ختم
}

منطقی cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    صحیح kernelExecTimeoutEnabled;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    واپس kernelExecTimeoutEnabled != 0;
#اگر_ختم
}

منطقی cv::cuda::DeviceInfo::integrated() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->integrated != 0;
#اگر_ختم
}

منطقی cv::cuda::DeviceInfo::can_Map_Host_حافظہ() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->can_Map_Host_حافظہ != 0;
#اگر_ختم
}

DeviceInfo::Compute_منوال cv::cuda::DeviceInfo::compute_منوال() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    ساکن ثابت Compute_منوال tbl[] =
    {
        Compute_منوال_Default,
        Compute_منوال_Exclusive,
        Compute_منوال_Prohibited,
        Compute_منوال_Exclusive_Process
    };

    صحیح compute_منوال;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&compute_منوال, cuda_Dev_Attr_Compute_منوال, device_id_));
    واپس tbl[compute_منوال];
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::maxTexture1D() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->maxTexture1D;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::maxTexture1DMipmap() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    #اگر CUDA_نسخہ >= 5000
        واپس deviceProps().get(device_id_)->maxTexture1DMipmap;
    #ورنہ
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        واپس 0;
    #اگر_ختم
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::maxTexture1DLinear() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    #اگر CUDA_نسخہ >= 13000
        size_t max_چوڑائی_اندر_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cuda_محفوظ_Call(cuda_Device_Get_Texture1_DLinear_Max_چوڑائی(&max_چوڑائی_اندر_Elements, &fmtDesc, device_id_));
        واپس max_چوڑائی_اندر_Elements;
    #ورنہ
        واپس deviceProps().get(device_id_)->maxTexture1DLinear;
    #اگر_ختم
#اگر_ختم
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#اگر_ختم
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    #اگر CUDA_نسخہ >= 5000
        واپس Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #ورنہ
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        واپس Vec2i();
    #اگر_ختم
#اگر_ختم
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#اگر_ختم
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#اگر_ختم
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::maxTextureCubemap() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->maxTextureCubemap;
#اگر_ختم
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#اگر_ختم
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#اگر_ختم
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::maxSurface1D() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->maxSurface1D;
#اگر_ختم
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#اگر_ختم
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#اگر_ختم
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#اگر_ختم
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::maxSurfaceCubemap() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->maxSurfaceCubemap;
#اگر_ختم
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#اگر_ختم
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->surfaceAlignment;
#اگر_ختم
}

منطقی cv::cuda::DeviceInfo::concurrentKernels() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->concurrentKernels != 0;
#اگر_ختم
}

منطقی cv::cuda::DeviceInfo::ECCEnabled() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->ECCEnabled != 0;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::pciBusID() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->pciBusID;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::pciDeviceID() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->pciDeviceID;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::pciDomainID() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->pciDomainID;
#اگر_ختم
}

منطقی cv::cuda::DeviceInfo::tccDriver() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->tccDriver != 0;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::async_Engine_گنتی() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->async_Engine_گنتی;
#اگر_ختم
}

منطقی cv::cuda::DeviceInfo::unifiedAddressing() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->unifiedAddressing != 0;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::حافظہ_Clock_شرح() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    صحیح حافظہ_Clock_شرح;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&حافظہ_Clock_شرح, cuda_Dev_Attr_حافظہ_Clock_شرح, device_id_));
    واپس حافظہ_Clock_شرح;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::حافظہ_Bus_چوڑائی() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->حافظہ_Bus_چوڑائی;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::l2_Cache_سائز() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->l2_Cache_سائز;
#اگر_ختم
}

صحیح cv::cuda::DeviceInfo::max_دھاگے_فی_Multi_Processor() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    واپس deviceProps().get(device_id_)->max_دھاگے_فی_Multi_Processor;
#اگر_ختم
}

خالی cv::cuda::DeviceInfo::query_حافظہ(size_t& کل_حافظہ, size_t& free_حافظہ) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(کل_حافظہ);
    CV_غیر_مستعمل(free_حافظہ);
    throw_no_cuda();
#ورنہ
    صحیح پچھلا_Device_ID = getDevice();
    اگر (پچھلا_Device_ID != device_id_)
        سیٹ_Device(device_id_);

    cuda_محفوظ_Call( cudaMemGetInfo(&free_حافظہ, &کل_حافظہ) );

    اگر (پچھلا_Device_ID != device_id_)
        سیٹ_Device(پچھلا_Device_ID);
#اگر_ختم
}

منطقی cv::cuda::DeviceInfo::isCompatible() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    // Check PTX compatibility
    اگر (ہدف_Archs::hasEqualOrLessPtx(major_نسخہ(), minor_نسخہ()))
        واپس سچ;

    // Check BIN compatibility
    برائے (صحیح i = minor_نسخہ(); i >= 0; --i)
        اگر (ہدف_Archs::hasBin(major_نسخہ(), i))
            واپس سچ;

    واپس جھوٹ;
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// print info

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا
{
    صحیح convertSMVer2Cores(صحیح major, صحیح minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        قسمتعریف ساخت {
            صحیح SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            صحیح Cores;
        } SMtoCores;

        SMtoCores gpu_Arch_Cores_فی_SM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        صحیح اشاریہ = 0;
        جب (gpu_Arch_Cores_فی_SM[اشاریہ].SM != -1)
        {
            اگر (gpu_Arch_Cores_فی_SM[اشاریہ].SM == ((major << 4) + minor) )
                واپس gpu_Arch_Cores_فی_SM[اشاریہ].Cores;
            اشاریہ++;
        }

        واپس -1;
    }
}

#اگر_ختم

خالی cv::cuda::printCudaDeviceInfo(صحیح device)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(device);
    throw_no_cuda();
#ورنہ
    صحیح گنتی_2 = get_Cuda_Enabled_Device_گنتی();
    منطقی valid = (device >= 0) && (device < گنتی_2);

    صحیح beg = valid ? device   : 0;
    صحیح end = valid ? device+1 : گنتی_2;

    لکھو("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    لکھو("Device count: %d\n", گنتی_2);

    صحیح driver_نسخہ = 0, runtime_نسخہ = 0;
    cuda_محفوظ_Call( cuda_Driver_Get_نسخہ(&driver_نسخہ) );
    cuda_محفوظ_Call( cuda_Runtime_Get_نسخہ(&runtime_نسخہ) );

    ثابت حرف *compute_منوال[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        تہی
    };

    برائے(صحیح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_محفوظ_Call( cudaGetDeviceProperties(&prop, dev) );

        لکھو("\nDevice %d: \"%s\"\n", dev, prop.name);
        لکھو("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_نسخہ/1000, driver_نسخہ%100, runtime_نسخہ/1000, runtime_نسخہ%100);
        لکھو("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        لکھو("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (اعشاری)prop.کل_Global_Mem/1048576.0f, (غیرعلامتی طویل طویل) prop.کل_Global_Mem);

        صحیح cores = convertSMVer2Cores(prop.major, prop.minor);
        اگر (cores > 0)
            لکھو("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_گنتی, cores, cores * prop.multi_Processor_گنتی);

        صحیح clock_شرح;
        cuda_محفوظ_Call(cudaDeviceGetAttribute(&clock_شرح, cuda_Dev_Attr_Clock_شرح, dev));
        لکھو("  GPU Clock Speed:                               %.2f GHz\n", clock_شرح * 1e-6f);

        لکھو("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        لکھو("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        لکھو("  Total amount of constant memory:               %u bytes\n", (صحیح)prop.کل_Const_Mem);
        لکھو("  Total amount of shared memory per block:       %u bytes\n", (صحیح)prop.shared_Mem_فی_بلاک);
        لکھو("  Total number of registers available per block: %d\n", prop.regs_فی_بلاک);
        لکھو("  Warp size:                                     %d\n", prop.warp_سائز);
        لکھو("  Maximum number of threads per block:           %d\n", prop.max_دھاگے_فی_بلاک);
        لکھو("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_دھاگے_Dim[0], prop.max_دھاگے_Dim[1], prop.max_دھاگے_Dim[2]);
        لکھو("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_سائز[0], prop.max_Grid_سائز[1],  prop.max_Grid_سائز[2]);
        لکھو("  Maximum memory pitch:                          %u bytes\n", (صحیح)prop.memPitch);
        لکھو("  Texture alignment:                             %u bytes\n", (صحیح)prop.textureAlignment);

        لکھو("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_گنتی ? "Yes" : "No"), prop.async_Engine_گنتی);
        صحیح kernelExecTimeoutEnabled;
        cuda_محفوظ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        لکھو("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        لکھو("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        لکھو("  Support host page-locked memory mapping:       %s\n", prop.can_Map_Host_حافظہ ? "Yes" : "No");

        لکھو("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        لکھو("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        لکھو("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        لکھو("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        لکھو("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        لکھو("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        صحیح prop_Compute_منوال;
        cuda_محفوظ_Call(cudaDeviceGetAttribute(&prop_Compute_منوال, cuda_Dev_Attr_Compute_منوال, dev));
        لکھو("  Compute Mode:\n");
        لکھو("      %s \n", compute_منوال[prop_Compute_منوال]);
    }

    لکھو("\n");
    لکھو("deviceQuery, CUDA Driver = CUDART");
    لکھو(", CUDA Driver Version  = %d.%d", driver_نسخہ / 1000, driver_نسخہ % 100);
    لکھو(", CUDA Runtime Version = %d.%d", runtime_نسخہ/1000, runtime_نسخہ%100);
    لکھو(", NumDevs = %d\n\n", گنتی_2);

    fflush(stdout);
#اگر_ختم
}

خالی cv::cuda::printShortCudaDeviceInfo(صحیح device)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(device);
    throw_no_cuda();
#ورنہ
    صحیح گنتی_2 = get_Cuda_Enabled_Device_گنتی();
    منطقی valid = (device >= 0) && (device < گنتی_2);

    صحیح beg = valid ? device   : 0;
    صحیح end = valid ? device+1 : گنتی_2;

    صحیح driver_نسخہ = 0, runtime_نسخہ = 0;
    cuda_محفوظ_Call( cuda_Driver_Get_نسخہ(&driver_نسخہ) );
    cuda_محفوظ_Call( cuda_Runtime_Get_نسخہ(&runtime_نسخہ) );

    برائے(صحیح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_محفوظ_Call( cudaGetDeviceProperties(&prop, dev) );

        ثابت حرف *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        لکھو("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (اعشاری)prop.کل_Global_Mem/1048576.0f);
        لکھو(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        صحیح cores = convertSMVer2Cores(prop.major, prop.minor);
        اگر (cores > 0)
            لکھو(", %d cores", cores * prop.multi_Processor_گنتی);

        لکھو(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_نسخہ/1000, driver_نسخہ%100, runtime_نسخہ/1000, runtime_نسخہ%100);
    }

    fflush(stdout);
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// Error handling

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا
{
    #تعریف_کرو error_اندراج(اندراج_2)  { اندراج_2, #entry }

    ساخت Error_اندراج
    {
        صحیح رمز;
        ثابت حرف* str;
    };

    ساخت Error_اندراج_Comparer
    {
        صحیح رمز;
        Error_اندراج_Comparer(صحیح رمز_2) : رمز(رمز_2) {}
        منطقی عامل()(ثابت Error_اندراج& e) ثابت { واپس e.رمز == رمز; }
    };

    ثابت Error_اندراج npp_errors [] =
    {
    #اگر defined (_MSC_VER)
        error_اندراج( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #اگر_ختم

    #اگر NPP_نسخہ < 5500
        error_اندراج( NPP_BAD_ARG_ERROR ),
        error_اندراج( NPP_COEFF_ERROR ),
        error_اندراج( NPP_RECT_ERROR ),
        error_اندراج( NPP_QUAD_ERROR ),
        error_اندراج( NPP_MEMFREE_ERR ),
        error_اندراج( NPP_MEMSET_ERR ),
        error_اندراج( NPP_MEM_ALLOC_ERR ),
        error_اندراج( NPP_HISTO_نمبر_کا_سطحیں_ERROR ),
        error_اندراج( NPP_MIRROR_FLIP_ERR ),
        error_اندراج( NPP_INVALID_داخلہ ),
        error_اندراج( NPP_اشارہ_گر_ERROR ),
        error_اندراج( NPP_WARNING ),
        error_اندراج( NPP_ODD_ROI_WARNING ),
    #ورنہ
        error_اندراج( NPP_INVALID_HOST_اشارہ_گر_ERROR ),
        error_اندراج( NPP_INVALID_DEVICE_اشارہ_گر_ERROR ),
        error_اندراج( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_اندراج( NPP_ZC_منوال_NOT_SUPPORTED_ERROR ),
        error_اندراج( NPP_MEMFREE_ERROR ),
        error_اندراج( NPP_MEMSET_ERROR ),
        error_اندراج( NPP_QUALITY_اشاریہ_ERROR ),
        error_اندراج( NPP_HISTOGRAM_نمبر_کا_سطحیں_ERROR ),
        error_اندراج( NPP_CHANNEL_ترتیب_ERROR ),
        error_اندراج( NPP_ZERO_نقاب_قدر_ERROR ),
        error_اندراج( NPP_QUADRANGLE_ERROR ),
        error_اندراج( NPP_RECTANGLE_ERROR ),
        error_اندراج( NPP_COEFFICIENT_ERROR ),
        error_اندراج( NPP_نمبر_کا_CHANNELS_ERROR ),
        error_اندراج( NPP_COI_ERROR ),
        error_اندراج( NPP_مقسوم_علیہ_ERROR ),
        error_اندراج( NPP_CHANNEL_ERROR ),
        error_اندراج( NPP_STRIDE_ERROR ),
        error_اندراج( NPP_ANCHOR_ERROR ),
        error_اندراج( NPP_نقاب_سائز_ERROR ),
        error_اندراج( NPP_MIRROR_FLIP_ERROR ),
        error_اندراج( NPP_MOMENT_00_ZERO_ERROR ),
        error_اندراج( NPP_حد_NEGATIVE_LEVEL_ERROR ),
        error_اندراج( NPP_حد_ERROR ),
        error_اندراج( NPP_CONTEXT_مطابقت_ERROR ),
        error_اندراج( NPP_FFT_FLAG_ERROR ),
        error_اندراج( NPP_FFT_ترتیب_ERROR ),
        error_اندراج( NPP_SCALE_RANGE_ERROR ),
        error_اندراج( NPP_ڈیٹا_TYPE_ERROR ),
        error_اندراج( NPP_برآمد_OFF_RANGE_ERROR ),
        error_اندراج( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_اندراج( NPP_حافظہ_تخصیص_ERR ),
        error_اندراج( NPP_RANGE_ERROR ),
        error_اندراج( NPP_BAD_دلیل_ERROR ),
        error_اندراج( NPP_NO_حافظہ_ERROR ),
        error_اندراج( NPP_ERROR_RESERVED ),
        error_اندراج( NPP_NO_OPERATION_WARNING ),
        error_اندراج( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_اندراج( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #اگر_ختم

        error_اندراج( NPP_NOT_SUPPORTED_منوال_ERROR ),
        error_اندراج( NPP_باری_منوال_NOT_SUPPORTED_ERROR ),
        error_اندراج( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_اندراج( NPP_LUT_نمبر_کا_سطحیں_ERROR ),
        error_اندراج( NPP_TEXTURE_BIND_ERROR ),
        error_اندراج( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_اندراج( NPP_NOT_EVEN_قدم_ERROR ),
        error_اندراج( NPP_INTERPOLATION_ERROR ),
        error_اندراج( NPP_RESIZE_ضربی_عدد_ERROR ),
        error_اندراج( NPP_HAAR_CLASSIFIER_PIXEL_مطابقت_ERROR ),
        error_اندراج( NPP_MEMCPY_ERROR ),
        error_اندراج( NPP_ALIGNMENT_ERROR ),
        error_اندراج( NPP_قدم_ERROR ),
        error_اندراج( NPP_سائز_ERROR ),
        error_اندراج( NPP_NULL_اشارہ_گر_ERROR ),
        error_اندراج( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_اندراج( NPP_NOT_IMPLEMENTED_ERROR ),
        error_اندراج( NPP_ERROR ),
        error_اندراج( NPP_NO_ERROR ),
        error_اندراج( NPP_SUCCESS ),
        error_اندراج( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_اندراج( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_اندراج( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_اندراج( NPP_DOUBLE_سائز_WARNING )
    };

    ثابت size_t npp_error_عدد = حجم(npp_errors) / حجم(npp_errors[0]);

    ثابت Error_اندراج cu_errors [] =
    {
        error_اندراج( CUDA_SUCCESS                              ),
        error_اندراج( CUDA_ERROR_INVALID_قدر                  ),
        error_اندراج( CUDA_ERROR_برآمد_کا_حافظہ                  ),
        error_اندراج( CUDA_ERROR_NOT_INITIALIZED                ),
        error_اندراج( CUDA_ERROR_DEINITIALIZED                  ),
        error_اندراج( CUDA_ERROR_PROFILER_DISABLED              ),
        error_اندراج( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_اندراج( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_اندراج( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_اندراج( CUDA_ERROR_NO_DEVICE                      ),
        error_اندراج( CUDA_ERROR_INVALID_DEVICE                 ),
        error_اندراج( CUDA_ERROR_INVALID_IMAGE                  ),
        error_اندراج( CUDA_ERROR_INVALID_CONTEXT                ),
        error_اندراج( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_اندراج( CUDA_ERROR_MAP_FAILED                     ),
        error_اندراج( CUDA_ERROR_UNMAP_FAILED                   ),
        error_اندراج( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_اندراج( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_اندراج( CUDA_ERROR_NO_ثنائی_FOR_GPU              ),
        error_اندراج( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_اندراج( CUDA_ERROR_NOT_MAPPED                     ),
        error_اندراج( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_اندراج( CUDA_ERROR_NOT_MAPPED_AS_اشارہ_گر          ),
        error_اندراج( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_اندراج( CUDA_ERROR_UNSUPPORTED_حد_بندی              ),
        error_اندراج( CUDA_ERROR_CONTEXT_ALREADY_اندر_USE         ),
        error_اندراج( CUDA_ERROR_INVALID_ماخذ                 ),
        error_اندراج( CUDA_ERROR_FILE_NOT_ملا                 ),
        error_اندراج( CUDA_ERROR_SHARED_OBJECT_علامت_NOT_ملا ),
        error_اندراج( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_اندراج( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_اندراج( CUDA_ERROR_INVALID_HANDLE                 ),
        error_اندراج( CUDA_ERROR_NOT_ملا                      ),
        error_اندراج( CUDA_ERROR_NOT_READY                      ),
        error_اندراج( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_اندراج( CUDA_ERROR_LAUNCH_برآمد_کا_RESOURCES        ),
        error_اندراج( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_اندراج( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_اندراج( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_اندراج( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_اندراج( CUDA_ERROR_PRIMARY_CONTEXT_فعال         ),
        error_اندراج( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_اندراج( CUDA_ERROR_ASSERT                         ),
        error_اندراج( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_اندراج( CUDA_ERROR_HOST_حافظہ_ALREADY_REGISTERED ),
        error_اندراج( CUDA_ERROR_HOST_حافظہ_NOT_REGISTERED     ),
        error_اندراج( CUDA_ERROR_UNKNOWN                        )
    };

    ثابت size_t cu_errors_عدد = حجم(cu_errors) / حجم(cu_errors[0]);

    cv::String getErrorString(صحیح رمز, ثابت Error_اندراج* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_اندراج_Comparer(رمز)) - errors;

        ثابت حرف* پیغام = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", پیغام, رمز);

        واپس str;
    }
}

#اگر_ختم

String cv::cuda::get_Npp_Error_پیغام(صحیح رمز)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(رمز);
    واپس String();
#ورنہ
    واپس getErrorString(رمز, npp_errors, npp_error_عدد);
#اگر_ختم
}

String cv::cuda::get_Cuda_Driver_Api_Error_پیغام(صحیح رمز)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(رمز);
    واپس String();
#ورنہ
    واپس getErrorString(رمز, cu_errors, cu_errors_عدد);
#اگر_ختم
}
