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

ورتوں نطاق cv;
ورتوں نطاق cv::cuda;

صحيح cv::cuda::get_Cuda_Enabled_Device_گنتی()
{
#جے_بنا_تعریف HAVE_CUDA
    أرجع 0;
#وإلا
    صحيح گنتی_2;
    cudaError_t error = cuda_Get_Device_گنتی(&گنتی_2);

    إذا (error == cudaErrorInsufficientDriver)
        أرجع -1;

    إذا (error == cudaErrorNoDevice)
        أرجع 0;

    cuda_محفوظ_Call( error );
    أرجع گنتی_2;
#جے_انت
}

فراغ cv::cuda::سیٹ_Device(صحيح device)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(device);
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cuda_سیٹ_Device(device) );
    cuda_محفوظ_Call( cudaFree(0) );
#جے_انت
}

صحيح cv::cuda::getDevice()
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح device;
    cuda_محفوظ_Call( cudaGetDevice(&device) );
    أرجع device;
#جے_انت
}

فراغ cv::cuda::resetDevice()
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cudaDeviceReset() );
#جے_انت
}

منطقي cv::cuda::deviceSupports(Feature_سیٹ feature_سیٹ)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(feature_سیٹ);
    throw_no_cuda();
#وإلا
    ساكن صحيح versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    ساكن ثابت صحيح cache_ناپ = static_cast<صحيح>(حجم(versions) / حجم(versions[0]));

    ثابت صحيح devId = getDevice();

    صحيح نسخہ;

    إذا (devId < cache_ناپ && versions[devId] >= 0)
    {
        نسخہ = versions[devId];
    }
    وإلا
    {
        DeviceInfo dev(devId);
        نسخہ = dev.major_نسخہ() * 10 + dev.minor_نسخہ();
        إذا (devId < cache_ناپ)
            versions[devId] = نسخہ;
    }

    أرجع نشانہ_Archs::builtWith(feature_سیٹ) && (نسخہ >= feature_سیٹ);
#جے_انت
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#جے_تعریف_شدہ HAVE_CUDA

نطاق
{
    صنف CudaArch
    {
    عام:
        CudaArch();

        منطقي builtWith(Feature_سیٹ feature_سیٹ) ثابت;
        منطقي hasPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasBin(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrLessPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrGreaterPtx(صحيح major, صحيح minor) ثابت;
        منطقي hasEqualOrGreaterBin(صحيح major, صحيح minor) ثابت;

    خاص:
        ساكن فراغ fromStr(ثابت حرف* سیٹ_as_str, std::vector<صحيح>& arr);

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

    منطقي CudaArch::builtWith(Feature_سیٹ feature_سیٹ) ثابت
    {
        أرجع !features.empty() && (features.back() >= feature_سیٹ);
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

    فراغ CudaArch::fromStr(ثابت حرف* سیٹ_as_str, std::vector<صحيح>& arr)
    {
        arr.clear();

        ثابت size_t len = لڑی_لمبائی(سیٹ_as_str);

        size_t pos = 0;
        طالما (pos < len)
        {
            إذا (خلا_اے(سیٹ_as_str[pos]))
            {
                ++pos;
            }
            وإلا
            {
                صحيح cur_قدر;
                صحيح chars_read;
                صحيح دلیلاں_read = لڑی_توں_پڑھو(سیٹ_as_str + pos, "%d%n", &cur_قدر, &chars_read);
                CV_Assert( دلیلاں_read == 1 );

                arr.push_back(cur_قدر);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#جے_انت

منطقي cv::cuda::نشانہ_Archs::builtWith(cv::cuda::Feature_سیٹ feature_سیٹ)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(feature_سیٹ);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.builtWith(feature_سیٹ);
#جے_انت
}

منطقي cv::cuda::نشانہ_Archs::hasPtx(صحيح major, صحيح minor)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(major);
    CV_بن_ورتیا(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasPtx(major, minor);
#جے_انت
}

منطقي cv::cuda::نشانہ_Archs::hasBin(صحيح major, صحيح minor)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(major);
    CV_بن_ورتیا(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasBin(major, minor);
#جے_انت
}

منطقي cv::cuda::نشانہ_Archs::hasEqualOrLessPtx(صحيح major, صحيح minor)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(major);
    CV_بن_ورتیا(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrLessPtx(major, minor);
#جے_انت
}

منطقي cv::cuda::نشانہ_Archs::hasEqualOrGreaterPtx(صحيح major, صحيح minor)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(major);
    CV_بن_ورتیا(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrGreaterPtx(major, minor);
#جے_انت
}

منطقي cv::cuda::نشانہ_Archs::hasEqualOrGreaterBin(صحيح major, صحيح minor)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(major);
    CV_بن_ورتیا(minor);
    throw_no_cuda();
#وإلا
    أرجع cudaArch.hasEqualOrGreaterBin(major, minor);
#جے_انت
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#جے_تعریف_شدہ HAVE_CUDA

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
        صحيح گنتی_2 = get_Cuda_Enabled_Device_گنتی();

        إذا (گنتی_2 > 0)
        {
            props_.resize(گنتی_2);

            لكل (صحيح devID = 0; devID < گنتی_2; ++devID)
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

#جے_انت

ثابت حرف* cv::cuda::DeviceInfo::name() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->name;
#جے_انت
}

size_t cv::cuda::DeviceInfo::کل_Global_Mem() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->کل_Global_Mem;
#جے_انت
}

size_t cv::cuda::DeviceInfo::shared_Mem_فی_بلاک() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->shared_Mem_فی_بلاک;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::regs_فی_بلاک() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->regs_فی_بلاک;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::warp_ناپ() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->warp_ناپ;
#جے_انت
}

size_t cv::cuda::DeviceInfo::memPitch() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->memPitch;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::max_دھاگے_فی_بلاک() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->max_دھاگے_فی_بلاک;
#جے_انت
}

Vec3i cv::cuda::DeviceInfo::max_دھاگے_Dim() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->max_دھاگے_Dim);
#جے_انت
}

Vec3i cv::cuda::DeviceInfo::max_Grid_ناپ() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->max_Grid_ناپ);
#جے_انت
}

صحيح cv::cuda::DeviceInfo::clock_شرح() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح clock_شرح;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&clock_شرح, cuda_Dev_Attr_Clock_شرح, device_id_));
    أرجع clock_شرح;
#جے_انت
}

size_t cv::cuda::DeviceInfo::کل_Const_Mem() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->کل_Const_Mem;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::major_نسخہ() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->major;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::minor_نسخہ() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->minor;
#جے_انت
}

size_t cv::cuda::DeviceInfo::textureAlignment() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->textureAlignment;
#جے_انت
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->texturePitchAlignment;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::multi_Processor_گنتی() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->multi_Processor_گنتی;
#جے_انت
}

منطقي cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح kernelExecTimeoutEnabled;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    أرجع kernelExecTimeoutEnabled != 0;
#جے_انت
}

منطقي cv::cuda::DeviceInfo::integrated() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->integrated != 0;
#جے_انت
}

منطقي cv::cuda::DeviceInfo::can_Map_Host_حافظہ() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->can_Map_Host_حافظہ != 0;
#جے_انت
}

DeviceInfo::Compute_منوال cv::cuda::DeviceInfo::compute_منوال() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    ساكن ثابت Compute_منوال tbl[] =
    {
        Compute_منوال_Default,
        Compute_منوال_Exclusive,
        Compute_منوال_Prohibited,
        Compute_منوال_Exclusive_Process
    };

    صحيح compute_منوال;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&compute_منوال, cuda_Dev_Attr_Compute_منوال, device_id_));
    أرجع tbl[compute_منوال];
#جے_انت
}

صحيح cv::cuda::DeviceInfo::maxTexture1D() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxTexture1D;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::maxTexture1DMipmap() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخہ >= 5000
        أرجع deviceProps().get(device_id_)->maxTexture1DMipmap;
    #وإلا
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        أرجع 0;
    #جے_انت
#جے_انت
}

صحيح cv::cuda::DeviceInfo::maxTexture1DLinear() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخہ >= 13000
        size_t max_چوڑائی_وچ_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cuda_محفوظ_Call(cuda_Device_Get_Texture1_DLinear_Max_چوڑائی(&max_چوڑائی_وچ_Elements, &fmtDesc, device_id_));
        أرجع max_چوڑائی_وچ_Elements;
    #وإلا
        أرجع deviceProps().get(device_id_)->maxTexture1DLinear;
    #جے_انت
#جے_انت
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#جے_انت
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    #إذا CUDA_نسخہ >= 5000
        أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #وإلا
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        أرجع Vec2i();
    #جے_انت
#جے_انت
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#جے_انت
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#جے_انت
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#جے_انت
}

صحيح cv::cuda::DeviceInfo::maxTextureCubemap() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxTextureCubemap;
#جے_انت
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#جے_انت
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#جے_انت
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#جے_انت
}

صحيح cv::cuda::DeviceInfo::maxSurface1D() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxSurface1D;
#جے_انت
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#جے_انت
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#جے_انت
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#جے_انت
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#جے_انت
}

صحيح cv::cuda::DeviceInfo::maxSurfaceCubemap() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->maxSurfaceCubemap;
#جے_انت
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#جے_انت
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->surfaceAlignment;
#جے_انت
}

منطقي cv::cuda::DeviceInfo::concurrentKernels() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->concurrentKernels != 0;
#جے_انت
}

منطقي cv::cuda::DeviceInfo::ECCEnabled() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->ECCEnabled != 0;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::pciBusID() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciBusID;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::pciDeviceID() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciDeviceID;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::pciDomainID() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->pciDomainID;
#جے_انت
}

منطقي cv::cuda::DeviceInfo::tccDriver() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->tccDriver != 0;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::async_Engine_گنتی() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->async_Engine_گنتی;
#جے_انت
}

منطقي cv::cuda::DeviceInfo::unifiedAddressing() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->unifiedAddressing != 0;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::حافظہ_Clock_شرح() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    صحيح حافظہ_Clock_شرح;
    cuda_محفوظ_Call(cudaDeviceGetAttribute(&حافظہ_Clock_شرح, cuda_Dev_Attr_حافظہ_Clock_شرح, device_id_));
    أرجع حافظہ_Clock_شرح;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::حافظہ_Bus_چوڑائی() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->حافظہ_Bus_چوڑائی;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::l2_Cache_ناپ() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->l2_Cache_ناپ;
#جے_انت
}

صحيح cv::cuda::DeviceInfo::max_دھاگے_فی_Multi_Processor() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    أرجع deviceProps().get(device_id_)->max_دھاگے_فی_Multi_Processor;
#جے_انت
}

فراغ cv::cuda::DeviceInfo::query_حافظہ(size_t& کل_حافظہ, size_t& free_حافظہ) ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(کل_حافظہ);
    CV_بن_ورتیا(free_حافظہ);
    throw_no_cuda();
#وإلا
    صحيح prevDeviceID = getDevice();
    إذا (prevDeviceID != device_id_)
        سیٹ_Device(device_id_);

    cuda_محفوظ_Call( cudaMemGetInfo(&free_حافظہ, &کل_حافظہ) );

    إذا (prevDeviceID != device_id_)
        سیٹ_Device(prevDeviceID);
#جے_انت
}

منطقي cv::cuda::DeviceInfo::isCompatible() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    // Check PTX compatibility
    إذا (نشانہ_Archs::hasEqualOrLessPtx(major_نسخہ(), minor_نسخہ()))
        أرجع سچ;

    // Check BIN compatibility
    لكل (صحيح i = minor_نسخہ(); i >= 0; --i)
        إذا (نشانہ_Archs::hasBin(major_نسخہ(), i))
            أرجع سچ;

    أرجع خطأ;
#جے_انت
}

////////////////////////////////////////////////////////////////////////
// print info

#جے_تعریف_شدہ HAVE_CUDA

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

        صحيح اشاریہ = 0;
        طالما (gpu_Arch_Cores_فی_SM[اشاریہ].SM != -1)
        {
            إذا (gpu_Arch_Cores_فی_SM[اشاریہ].SM == ((major << 4) + minor) )
                أرجع gpu_Arch_Cores_فی_SM[اشاریہ].Cores;
            اشاریہ++;
        }

        أرجع -1;
    }
}

#جے_انت

فراغ cv::cuda::printCudaDeviceInfo(صحيح device)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(device);
    throw_no_cuda();
#وإلا
    صحيح گنتی_2 = get_Cuda_Enabled_Device_گنتی();
    منطقي valid = (device >= 0) && (device < گنتی_2);

    صحيح beg = valid ? device   : 0;
    صحيح end = valid ? device+1 : گنتی_2;

    لکھو("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    لکھو("Device count: %d\n", گنتی_2);

    صحيح driver_نسخہ = 0, runtime_نسخہ = 0;
    cuda_محفوظ_Call( cuda_Driver_Get_نسخہ(&driver_نسخہ) );
    cuda_محفوظ_Call( cuda_Runtime_Get_نسخہ(&runtime_نسخہ) );

    ثابت حرف *compute_منوال[] = {
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

        لکھو("\nDevice %d: \"%s\"\n", dev, prop.name);
        لکھو("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_نسخہ/1000, driver_نسخہ%100, runtime_نسخہ/1000, runtime_نسخہ%100);
        لکھو("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        لکھو("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (عائم)prop.کل_Global_Mem/1048576.0f, (بنانشانی لما لما) prop.کل_Global_Mem);

        صحيح cores = convertSMVer2Cores(prop.major, prop.minor);
        إذا (cores > 0)
            لکھو("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_گنتی, cores, cores * prop.multi_Processor_گنتی);

        صحيح clock_شرح;
        cuda_محفوظ_Call(cudaDeviceGetAttribute(&clock_شرح, cuda_Dev_Attr_Clock_شرح, dev));
        لکھو("  GPU Clock Speed:                               %.2f GHz\n", clock_شرح * 1e-6f);

        لکھو("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        لکھو("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        لکھو("  Total amount of constant memory:               %u bytes\n", (صحيح)prop.کل_Const_Mem);
        لکھو("  Total amount of shared memory per block:       %u bytes\n", (صحيح)prop.shared_Mem_فی_بلاک);
        لکھو("  Total number of registers available per block: %d\n", prop.regs_فی_بلاک);
        لکھو("  Warp size:                                     %d\n", prop.warp_ناپ);
        لکھو("  Maximum number of threads per block:           %d\n", prop.max_دھاگے_فی_بلاک);
        لکھو("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_دھاگے_Dim[0], prop.max_دھاگے_Dim[1], prop.max_دھاگے_Dim[2]);
        لکھو("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_ناپ[0], prop.max_Grid_ناپ[1],  prop.max_Grid_ناپ[2]);
        لکھو("  Maximum memory pitch:                          %u bytes\n", (صحيح)prop.memPitch);
        لکھو("  Texture alignment:                             %u bytes\n", (صحيح)prop.textureAlignment);

        لکھو("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_گنتی ? "Yes" : "No"), prop.async_Engine_گنتی);
        صحيح kernelExecTimeoutEnabled;
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

        صحيح prop_Compute_منوال;
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
#جے_انت
}

فراغ cv::cuda::printShortCudaDeviceInfo(صحيح device)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(device);
    throw_no_cuda();
#وإلا
    صحيح گنتی_2 = get_Cuda_Enabled_Device_گنتی();
    منطقي valid = (device >= 0) && (device < گنتی_2);

    صحيح beg = valid ? device   : 0;
    صحيح end = valid ? device+1 : گنتی_2;

    صحيح driver_نسخہ = 0, runtime_نسخہ = 0;
    cuda_محفوظ_Call( cuda_Driver_Get_نسخہ(&driver_نسخہ) );
    cuda_محفوظ_Call( cuda_Runtime_Get_نسخہ(&runtime_نسخہ) );

    لكل(صحيح dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_محفوظ_Call( cudaGetDeviceProperties(&prop, dev) );

        ثابت حرف *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        لکھو("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (عائم)prop.کل_Global_Mem/1048576.0f);
        لکھو(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        صحيح cores = convertSMVer2Cores(prop.major, prop.minor);
        إذا (cores > 0)
            لکھو(", %d cores", cores * prop.multi_Processor_گنتی);

        لکھو(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_نسخہ/1000, driver_نسخہ%100, runtime_نسخہ/1000, runtime_نسخہ%100);
    }

    fflush(stdout);
#جے_انت
}

////////////////////////////////////////////////////////////////////////
// Error handling

#جے_تعریف_شدہ HAVE_CUDA

نطاق
{
    #تعریف_کرو error_اندراج(اندراج)  { اندراج, #entry }

    بنية Error_اندراج
    {
        صحيح کوڈ;
        ثابت حرف* str;
    };

    بنية Error_اندراج_Comparer
    {
        صحيح کوڈ;
        Error_اندراج_Comparer(صحيح کوڈ_2) : کوڈ(کوڈ_2) {}
        منطقي عامل()(ثابت Error_اندراج& e) ثابت { أرجع e.کوڈ == کوڈ; }
    };

    ثابت Error_اندراج npp_errors [] =
    {
    #إذا defined (_MSC_VER)
        error_اندراج( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #جے_انت

    #إذا NPP_نسخہ < 5500
        error_اندراج( NPP_BAD_ARG_ERROR ),
        error_اندراج( NPP_COEFF_ERROR ),
        error_اندراج( NPP_RECT_ERROR ),
        error_اندراج( NPP_QUAD_ERROR ),
        error_اندراج( NPP_MEMFREE_ERR ),
        error_اندراج( NPP_MEMSET_ERR ),
        error_اندراج( NPP_MEM_ALLOC_ERR ),
        error_اندراج( NPP_HISTO_نمبر_دا_پدھراں_ERROR ),
        error_اندراج( NPP_MIRROR_FLIP_ERR ),
        error_اندراج( NPP_INVALID_داخلہ ),
        error_اندراج( NPP_اشارہ_گر_ERROR ),
        error_اندراج( NPP_WARNING ),
        error_اندراج( NPP_ODD_ROI_WARNING ),
    #وإلا
        error_اندراج( NPP_INVALID_HOST_اشارہ_گر_ERROR ),
        error_اندراج( NPP_INVALID_DEVICE_اشارہ_گر_ERROR ),
        error_اندراج( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_اندراج( NPP_ZC_منوال_NOT_SUPPORTED_ERROR ),
        error_اندراج( NPP_MEMFREE_ERROR ),
        error_اندراج( NPP_MEMSET_ERROR ),
        error_اندراج( NPP_QUALITY_اشاریہ_ERROR ),
        error_اندراج( NPP_HISTOGRAM_نمبر_دا_پدھراں_ERROR ),
        error_اندراج( NPP_CHANNEL_ترتیب_ERROR ),
        error_اندراج( NPP_ZERO_نقاب_قدر_ERROR ),
        error_اندراج( NPP_QUADRANGLE_ERROR ),
        error_اندراج( NPP_RECTANGLE_ERROR ),
        error_اندراج( NPP_COEFFICIENT_ERROR ),
        error_اندراج( NPP_نمبر_دا_CHANNELS_ERROR ),
        error_اندراج( NPP_COI_ERROR ),
        error_اندراج( NPP_مقسوم_علیہ_ERROR ),
        error_اندراج( NPP_CHANNEL_ERROR ),
        error_اندراج( NPP_STRIDE_ERROR ),
        error_اندراج( NPP_ANCHOR_ERROR ),
        error_اندراج( NPP_نقاب_ناپ_ERROR ),
        error_اندراج( NPP_MIRROR_FLIP_ERROR ),
        error_اندراج( NPP_MOMENT_00_ZERO_ERROR ),
        error_اندراج( NPP_حد_NEGATIVE_LEVEL_ERROR ),
        error_اندراج( NPP_حد_ERROR ),
        error_اندراج( NPP_CONTEXT_میل_ERROR ),
        error_اندراج( NPP_FFT_FLAG_ERROR ),
        error_اندراج( NPP_FFT_ترتیب_ERROR ),
        error_اندراج( NPP_SCALE_RANGE_ERROR ),
        error_اندراج( NPP_انکڑے_TYPE_ERROR ),
        error_اندراج( NPP_باہرلا_OFF_RANGE_ERROR ),
        error_اندراج( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_اندراج( NPP_حافظہ_ونڈ_ERR ),
        error_اندراج( NPP_RANGE_ERROR ),
        error_اندراج( NPP_BAD_دلیل_ERROR ),
        error_اندراج( NPP_NO_حافظہ_ERROR ),
        error_اندراج( NPP_ERROR_RESERVED ),
        error_اندراج( NPP_NO_OPERATION_WARNING ),
        error_اندراج( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_اندراج( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #جے_انت

        error_اندراج( NPP_NOT_SUPPORTED_منوال_ERROR ),
        error_اندراج( NPP_گیڑا_منوال_NOT_SUPPORTED_ERROR ),
        error_اندراج( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_اندراج( NPP_LUT_نمبر_دا_پدھراں_ERROR ),
        error_اندراج( NPP_TEXTURE_BIND_ERROR ),
        error_اندراج( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_اندراج( NPP_NOT_EVEN_قدم_ERROR ),
        error_اندراج( NPP_INTERPOLATION_ERROR ),
        error_اندراج( NPP_RESIZE_ضارب_ERROR ),
        error_اندراج( NPP_HAAR_CLASSIFIER_PIXEL_میل_ERROR ),
        error_اندراج( NPP_MEMCPY_ERROR ),
        error_اندراج( NPP_ALIGNMENT_ERROR ),
        error_اندراج( NPP_قدم_ERROR ),
        error_اندراج( NPP_ناپ_ERROR ),
        error_اندراج( NPP_NULL_اشارہ_گر_ERROR ),
        error_اندراج( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_اندراج( NPP_NOT_IMPLEMENTED_ERROR ),
        error_اندراج( NPP_ERROR ),
        error_اندراج( NPP_NO_ERROR ),
        error_اندراج( NPP_SUCCESS ),
        error_اندراج( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_اندراج( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_اندراج( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_اندراج( NPP_DOUBLE_ناپ_WARNING )
    };

    ثابت size_t npp_error_عدد = حجم(npp_errors) / حجم(npp_errors[0]);

    ثابت Error_اندراج cu_errors [] =
    {
        error_اندراج( CUDA_SUCCESS                              ),
        error_اندراج( CUDA_ERROR_INVALID_قدر                  ),
        error_اندراج( CUDA_ERROR_باہرلا_دا_حافظہ                  ),
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
        error_اندراج( CUDA_ERROR_CONTEXT_ALREADY_وچ_USE         ),
        error_اندراج( CUDA_ERROR_INVALID_سروت                 ),
        error_اندراج( CUDA_ERROR_FILE_NOT_لبھیا                 ),
        error_اندراج( CUDA_ERROR_SHARED_OBJECT_علامت_NOT_لبھیا ),
        error_اندراج( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_اندراج( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_اندراج( CUDA_ERROR_INVALID_HANDLE                 ),
        error_اندراج( CUDA_ERROR_NOT_لبھیا                      ),
        error_اندراج( CUDA_ERROR_NOT_READY                      ),
        error_اندراج( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_اندراج( CUDA_ERROR_LAUNCH_باہرلا_دا_RESOURCES        ),
        error_اندراج( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_اندراج( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_اندراج( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_اندراج( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_اندراج( CUDA_ERROR_PRIMARY_CONTEXT_سرگرم         ),
        error_اندراج( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_اندراج( CUDA_ERROR_ASSERT                         ),
        error_اندراج( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_اندراج( CUDA_ERROR_HOST_حافظہ_ALREADY_REGISTERED ),
        error_اندراج( CUDA_ERROR_HOST_حافظہ_NOT_REGISTERED     ),
        error_اندراج( CUDA_ERROR_UNKNOWN                        )
    };

    ثابت size_t cu_errors_عدد = حجم(cu_errors) / حجم(cu_errors[0]);

    cv::String getErrorString(صحيح کوڈ, ثابت Error_اندراج* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_اندراج_Comparer(کوڈ)) - errors;

        ثابت حرف* پیغام = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", پیغام, کوڈ);

        أرجع str;
    }
}

#جے_انت

String cv::cuda::get_Npp_Error_سنیہا(صحيح کوڈ)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(کوڈ);
    أرجع String();
#وإلا
    أرجع getErrorString(کوڈ, npp_errors, npp_error_عدد);
#جے_انت
}

String cv::cuda::get_Cuda_Driver_Api_Error_سنیہا(صحيح کوڈ)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(کوڈ);
    أرجع String();
#وإلا
    أرجع getErrorString(کوڈ, cu_errors, cu_errors_عدد);
#جے_انت
}
