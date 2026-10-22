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

#అన్తర్భావ "precomp.hpp"

ఉపయోగించి నామాకాశ cv;
ఉపయోగించి నామాకాశ cv::cuda;

పూర్ణాంకం cv::cuda::get_Cuda_Enabled_Device_లెక్క()
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    తిరిగి 0;
#లేకపోతే
    పూర్ణాంకం లెక్క_2;
    cudaError_t error = cuda_Get_Device_లెక్క(&లెక్క_2);

    ఒకవేళ (error == cudaErrorInsufficientDriver)
        తిరిగి -1;

    ఒకవేళ (error == cudaErrorNoDevice)
        తిరిగి 0;

    cuda_సురక్షిత_Call( error );
    తిరిగి లెక్క_2;
#అయితే_ముగింపు
}

శూన్య cv::cuda::సమితి_Device(పూర్ణాంకం device)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(device);
    throw_no_cuda();
#లేకపోతే
    cuda_సురక్షిత_Call( cuda_సమితి_Device(device) );
    cuda_సురక్షిత_Call( cudaFree(0) );
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::getDevice()
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    పూర్ణాంకం device;
    cuda_సురక్షిత_Call( cudaGetDevice(&device) );
    తిరిగి device;
#అయితే_ముగింపు
}

శూన్య cv::cuda::resetDevice()
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    cuda_సురక్షిత_Call( cudaDeviceReset() );
#అయితే_ముగింపు
}

తార్కిక cv::cuda::deviceSupports(Feature_సమితి feature_సమితి)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(feature_సమితి);
    throw_no_cuda();
#లేకపోతే
    స్థిర పూర్ణాంకం versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    స్థిర స్థిరరాశి పూర్ణాంకం cache_కొలత = static_cast<పూర్ణాంకం>(పరిమాణం(versions) / పరిమాణం(versions[0]));

    స్థిరరాశి పూర్ణాంకం devId = getDevice();

    పూర్ణాంకం సంస్కరణ;

    ఒకవేళ (devId < cache_కొలత && versions[devId] >= 0)
    {
        సంస్కరణ = versions[devId];
    }
    లేకపోతే
    {
        DeviceInfo dev(devId);
        సంస్కరణ = dev.major_సంస్కరణ() * 10 + dev.minor_సంస్కరణ();
        ఒకవేళ (devId < cache_కొలత)
            versions[devId] = సంస్కరణ;
    }

    తిరిగి లక్ష్యం_Archs::builtWith(feature_సమితి) && (సంస్కరణ >= feature_సమితి);
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ
{
    తరగతి CudaArch
    {
    బహిరంగ:
        CudaArch();

        తార్కిక builtWith(Feature_సమితి feature_సమితి) స్థిరరాశి;
        తార్కిక hasPtx(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి;
        తార్కిక hasBin(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి;
        తార్కిక hasEqualOrLessPtx(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి;
        తార్కిక hasEqualOrGreaterPtx(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి;
        తార్కిక hasEqualOrGreaterBin(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి;

    అంతరంగ:
        స్థిర శూన్య fromStr(స్థిరరాశి అక్షరం* సమితి_as_str, std::vector<పూర్ణాంకం>& arr);

        std::vector<పూర్ణాంకం> bin;
        std::vector<పూర్ణాంకం> ptx;
        std::vector<పూర్ణాంకం> features;
    };

    స్థిరరాశి CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    తార్కిక CudaArch::builtWith(Feature_సమితి feature_సమితి) స్థిరరాశి
    {
        తిరిగి !features.empty() && (features.back() >= feature_సమితి);
    }

    తార్కిక CudaArch::hasPtx(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి
    {
        తిరిగి std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    తార్కిక CudaArch::hasBin(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి
    {
        తిరిగి std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    తార్కిక CudaArch::hasEqualOrLessPtx(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి
    {
        తిరిగి !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    తార్కిక CudaArch::hasEqualOrGreaterPtx(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి
    {
        తిరిగి !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    తార్కిక CudaArch::hasEqualOrGreaterBin(పూర్ణాంకం major, పూర్ణాంకం minor) స్థిరరాశి
    {
        తిరిగి !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    శూన్య CudaArch::fromStr(స్థిరరాశి అక్షరం* సమితి_as_str, std::vector<పూర్ణాంకం>& arr)
    {
        arr.clear();

        స్థిరరాశి size_t len = అక్షరశ్రేణి_పొడవు(సమితి_as_str);

        size_t pos = 0;
        అయితే (pos < len)
        {
            ఒకవేళ (ఖాళీనా(సమితి_as_str[pos]))
            {
                ++pos;
            }
            లేకపోతే
            {
                పూర్ణాంకం cur_విలువ;
                పూర్ణాంకం chars_read;
                పూర్ణాంకం వాదనలు_read = అక్షరశ్రేణి_చదువు(సమితి_as_str + pos, "%d%n", &cur_విలువ, &chars_read);
                CV_Assert( వాదనలు_read == 1 );

                arr.push_back(cur_విలువ);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#అయితే_ముగింపు

తార్కిక cv::cuda::లక్ష్యం_Archs::builtWith(cv::cuda::Feature_సమితి feature_సమితి)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(feature_సమితి);
    throw_no_cuda();
#లేకపోతే
    తిరిగి cudaArch.builtWith(feature_సమితి);
#అయితే_ముగింపు
}

తార్కిక cv::cuda::లక్ష్యం_Archs::hasPtx(పూర్ణాంకం major, పూర్ణాంకం minor)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(major);
    CV_వాడని(minor);
    throw_no_cuda();
#లేకపోతే
    తిరిగి cudaArch.hasPtx(major, minor);
#అయితే_ముగింపు
}

తార్కిక cv::cuda::లక్ష్యం_Archs::hasBin(పూర్ణాంకం major, పూర్ణాంకం minor)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(major);
    CV_వాడని(minor);
    throw_no_cuda();
#లేకపోతే
    తిరిగి cudaArch.hasBin(major, minor);
#అయితే_ముగింపు
}

తార్కిక cv::cuda::లక్ష్యం_Archs::hasEqualOrLessPtx(పూర్ణాంకం major, పూర్ణాంకం minor)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(major);
    CV_వాడని(minor);
    throw_no_cuda();
#లేకపోతే
    తిరిగి cudaArch.hasEqualOrLessPtx(major, minor);
#అయితే_ముగింపు
}

తార్కిక cv::cuda::లక్ష్యం_Archs::hasEqualOrGreaterPtx(పూర్ణాంకం major, పూర్ణాంకం minor)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(major);
    CV_వాడని(minor);
    throw_no_cuda();
#లేకపోతే
    తిరిగి cudaArch.hasEqualOrGreaterPtx(major, minor);
#అయితే_ముగింపు
}

తార్కిక cv::cuda::లక్ష్యం_Archs::hasEqualOrGreaterBin(పూర్ణాంకం major, పూర్ణాంకం minor)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(major);
    CV_వాడని(minor);
    throw_no_cuda();
#లేకపోతే
    తిరిగి cudaArch.hasEqualOrGreaterBin(major, minor);
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ
{
    తరగతి DeviceProps
    {
    బహిరంగ:
        DeviceProps();

        స్థిరరాశి cudaDeviceProp* get(పూర్ణాంకం devID) స్థిరరాశి;

    అంతరంగ:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        పూర్ణాంకం లెక్క_2 = get_Cuda_Enabled_Device_లెక్క();

        ఒకవేళ (లెక్క_2 > 0)
        {
            props_.resize(లెక్క_2);

            కొరకు (పూర్ణాంకం devID = 0; devID < లెక్క_2; ++devID)
            {
                cuda_సురక్షిత_Call( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    స్థిరరాశి cudaDeviceProp* DeviceProps::get(పూర్ణాంకం devID) స్థిరరాశి
    {
        CV_Assert( static_cast<size_t>(devID) < props_.size() );

        తిరిగి &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        స్థిర DeviceProps props;
        తిరిగి props;
    }
}

#అయితే_ముగింపు

స్థిరరాశి అక్షరం* cv::cuda::DeviceInfo::name() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->name;
#అయితే_ముగింపు
}

size_t cv::cuda::DeviceInfo::మొత్తం_Global_Mem() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->మొత్తం_Global_Mem;
#అయితే_ముగింపు
}

size_t cv::cuda::DeviceInfo::shared_Mem_చొప్పున_ఖండం() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->shared_Mem_చొప్పున_ఖండం;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::regs_చొప్పున_ఖండం() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->regs_చొప్పున_ఖండం;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::warp_కొలత() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->warp_కొలత;
#అయితే_ముగింపు
}

size_t cv::cuda::DeviceInfo::memPitch() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->memPitch;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::max_దారాలు_చొప్పున_ఖండం() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->max_దారాలు_చొప్పున_ఖండం;
#అయితే_ముగింపు
}

Vec3i cv::cuda::DeviceInfo::max_దారాలు_Dim() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec3i(deviceProps().get(device_id_)->max_దారాలు_Dim);
#అయితే_ముగింపు
}

Vec3i cv::cuda::DeviceInfo::max_Grid_కొలత() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec3i(deviceProps().get(device_id_)->max_Grid_కొలత);
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::clock_రేటు() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    పూర్ణాంకం clock_రేటు;
    cuda_సురక్షిత_Call(cudaDeviceGetAttribute(&clock_రేటు, cuda_Dev_Attr_Clock_రేటు, device_id_));
    తిరిగి clock_రేటు;
#అయితే_ముగింపు
}

size_t cv::cuda::DeviceInfo::మొత్తం_Const_Mem() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->మొత్తం_Const_Mem;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::major_సంస్కరణ() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->major;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::minor_సంస్కరణ() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->minor;
#అయితే_ముగింపు
}

size_t cv::cuda::DeviceInfo::textureAlignment() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->textureAlignment;
#అయితే_ముగింపు
}

size_t cv::cuda::DeviceInfo::texturePitchAlignment() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->texturePitchAlignment;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::multi_Processor_లెక్క() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->multi_Processor_లెక్క;
#అయితే_ముగింపు
}

తార్కిక cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    పూర్ణాంకం kernelExecTimeoutEnabled;
    cuda_సురక్షిత_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    తిరిగి kernelExecTimeoutEnabled != 0;
#అయితే_ముగింపు
}

తార్కిక cv::cuda::DeviceInfo::integrated() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->integrated != 0;
#అయితే_ముగింపు
}

తార్కిక cv::cuda::DeviceInfo::can_Map_Host_జ్ఞాపకం() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->can_Map_Host_జ్ఞాపకం != 0;
#అయితే_ముగింపు
}

DeviceInfo::Compute_బహుళకం cv::cuda::DeviceInfo::compute_బహుళకం() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    స్థిర స్థిరరాశి Compute_బహుళకం tbl[] =
    {
        Compute_బహుళకం_Default,
        Compute_బహుళకం_Exclusive,
        Compute_బహుళకం_Prohibited,
        Compute_బహుళకం_Exclusive_Process
    };

    పూర్ణాంకం compute_బహుళకం;
    cuda_సురక్షిత_Call(cudaDeviceGetAttribute(&compute_బహుళకం, cuda_Dev_Attr_Compute_బహుళకం, device_id_));
    తిరిగి tbl[compute_బహుళకం];
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::maxTexture1D() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->maxTexture1D;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::maxTexture1DMipmap() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    #ఒకవేళ CUDA_సంస్కరణ >= 5000
        తిరిగి deviceProps().get(device_id_)->maxTexture1DMipmap;
    #లేకపోతే
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        తిరిగి 0;
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::maxTexture1DLinear() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    #ఒకవేళ CUDA_సంస్కరణ >= 13000
        size_t max_వెడల్పు_లోపల_Elements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cuda_సురక్షిత_Call(cuda_Device_Get_Texture1_DLinear_Max_వెడల్పు(&max_వెడల్పు_లోపల_Elements, &fmtDesc, device_id_));
        తిరిగి max_వెడల్పు_లోపల_Elements;
    #లేకపోతే
        తిరిగి deviceProps().get(device_id_)->maxTexture1DLinear;
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec2i(deviceProps().get(device_id_)->maxTexture2D);
#అయితే_ముగింపు
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    #ఒకవేళ CUDA_సంస్కరణ >= 5000
        తిరిగి Vec2i(deviceProps().get(device_id_)->maxTexture2DMipmap);
    #లేకపోతే
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        తిరిగి Vec2i();
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec3i(deviceProps().get(device_id_)->maxTexture2DLinear);
#అయితే_ముగింపు
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec2i(deviceProps().get(device_id_)->maxTexture2DGather);
#అయితే_ముగింపు
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec3i(deviceProps().get(device_id_)->maxTexture3D);
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::maxTextureCubemap() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->maxTextureCubemap;
#అయితే_ముగింపు
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec2i(deviceProps().get(device_id_)->maxTexture1DLayered);
#అయితే_ముగింపు
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec3i(deviceProps().get(device_id_)->maxTexture2DLayered);
#అయితే_ముగింపు
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec2i(deviceProps().get(device_id_)->maxTextureCubemapLayered);
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::maxSurface1D() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->maxSurface1D;
#అయితే_ముగింపు
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec2i(deviceProps().get(device_id_)->maxSurface2D);
#అయితే_ముగింపు
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec3i(deviceProps().get(device_id_)->maxSurface3D);
#అయితే_ముగింపు
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec2i(deviceProps().get(device_id_)->maxSurface1DLayered);
#అయితే_ముగింపు
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec3i(deviceProps().get(device_id_)->maxSurface2DLayered);
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::maxSurfaceCubemap() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->maxSurfaceCubemap;
#అయితే_ముగింపు
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి Vec2i(deviceProps().get(device_id_)->maxSurfaceCubemapLayered);
#అయితే_ముగింపు
}

size_t cv::cuda::DeviceInfo::surfaceAlignment() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->surfaceAlignment;
#అయితే_ముగింపు
}

తార్కిక cv::cuda::DeviceInfo::concurrentKernels() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->concurrentKernels != 0;
#అయితే_ముగింపు
}

తార్కిక cv::cuda::DeviceInfo::ECCEnabled() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->ECCEnabled != 0;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::pciBusID() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->pciBusID;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::pciDeviceID() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->pciDeviceID;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::pciDomainID() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->pciDomainID;
#అయితే_ముగింపు
}

తార్కిక cv::cuda::DeviceInfo::tccDriver() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->tccDriver != 0;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::async_Engine_లెక్క() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->async_Engine_లెక్క;
#అయితే_ముగింపు
}

తార్కిక cv::cuda::DeviceInfo::unifiedAddressing() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->unifiedAddressing != 0;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::జ్ఞాపకం_Clock_రేటు() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    పూర్ణాంకం జ్ఞాపకం_Clock_రేటు;
    cuda_సురక్షిత_Call(cudaDeviceGetAttribute(&జ్ఞాపకం_Clock_రేటు, cuda_Dev_Attr_జ్ఞాపకం_Clock_రేటు, device_id_));
    తిరిగి జ్ఞాపకం_Clock_రేటు;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::జ్ఞాపకం_Bus_వెడల్పు() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->జ్ఞాపకం_Bus_వెడల్పు;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::l2_Cache_కొలత() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->l2_Cache_కొలత;
#అయితే_ముగింపు
}

పూర్ణాంకం cv::cuda::DeviceInfo::max_దారాలు_చొప్పున_Multi_Processor() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    తిరిగి deviceProps().get(device_id_)->max_దారాలు_చొప్పున_Multi_Processor;
#అయితే_ముగింపు
}

శూన్య cv::cuda::DeviceInfo::query_జ్ఞాపకం(size_t& మొత్తం_జ్ఞాపకం, size_t& free_జ్ఞాపకం) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(మొత్తం_జ్ఞాపకం);
    CV_వాడని(free_జ్ఞాపకం);
    throw_no_cuda();
#లేకపోతే
    పూర్ణాంకం prevDeviceID = getDevice();
    ఒకవేళ (prevDeviceID != device_id_)
        సమితి_Device(device_id_);

    cuda_సురక్షిత_Call( cudaMemGetInfo(&free_జ్ఞాపకం, &మొత్తం_జ్ఞాపకం) );

    ఒకవేళ (prevDeviceID != device_id_)
        సమితి_Device(prevDeviceID);
#అయితే_ముగింపు
}

తార్కిక cv::cuda::DeviceInfo::isCompatible() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    // Check PTX compatibility
    ఒకవేళ (లక్ష్యం_Archs::hasEqualOrLessPtx(major_సంస్కరణ(), minor_సంస్కరణ()))
        తిరిగి నిజం;

    // Check BIN compatibility
    కొరకు (పూర్ణాంకం i = minor_సంస్కరణ(); i >= 0; --i)
        ఒకవేళ (లక్ష్యం_Archs::hasBin(major_సంస్కరణ(), i))
            తిరిగి నిజం;

    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// print info

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ
{
    పూర్ణాంకం convertSMVer2Cores(పూర్ణాంకం major, పూర్ణాంకం minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        రకనిర్వచనం నిర్మాణం {
            పూర్ణాంకం SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            పూర్ణాంకం Cores;
        } SMtoCores;

        SMtoCores gpu_Arch_Cores_చొప్పున_SM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        పూర్ణాంకం సూచీ = 0;
        అయితే (gpu_Arch_Cores_చొప్పున_SM[సూచీ].SM != -1)
        {
            ఒకవేళ (gpu_Arch_Cores_చొప్పున_SM[సూచీ].SM == ((major << 4) + minor) )
                తిరిగి gpu_Arch_Cores_చొప్పున_SM[సూచీ].Cores;
            సూచీ++;
        }

        తిరిగి -1;
    }
}

#అయితే_ముగింపు

శూన్య cv::cuda::printCudaDeviceInfo(పూర్ణాంకం device)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(device);
    throw_no_cuda();
#లేకపోతే
    పూర్ణాంకం లెక్క_2 = get_Cuda_Enabled_Device_లెక్క();
    తార్కిక valid = (device >= 0) && (device < లెక్క_2);

    పూర్ణాంకం beg = valid ? device   : 0;
    పూర్ణాంకం end = valid ? device+1 : లెక్క_2;

    రాయి("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    రాయి("Device count: %d\n", లెక్క_2);

    పూర్ణాంకం driver_సంస్కరణ = 0, runtime_సంస్కరణ = 0;
    cuda_సురక్షిత_Call( cuda_Driver_Get_సంస్కరణ(&driver_సంస్కరణ) );
    cuda_సురక్షిత_Call( cuda_Runtime_Get_సంస్కరణ(&runtime_సంస్కరణ) );

    స్థిరరాశి అక్షరం *compute_బహుళకం[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        ఖాళీ
    };

    కొరకు(పూర్ణాంకం dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_సురక్షిత_Call( cudaGetDeviceProperties(&prop, dev) );

        రాయి("\nDevice %d: \"%s\"\n", dev, prop.name);
        రాయి("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driver_సంస్కరణ/1000, driver_సంస్కరణ%100, runtime_సంస్కరణ/1000, runtime_సంస్కరణ%100);
        రాయి("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        రాయి("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (దశాంశం)prop.మొత్తం_Global_Mem/1048576.0f, (గుర్తులేని దీర్ఘ దీర్ఘ) prop.మొత్తం_Global_Mem);

        పూర్ణాంకం cores = convertSMVer2Cores(prop.major, prop.minor);
        ఒకవేళ (cores > 0)
            రాయి("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_లెక్క, cores, cores * prop.multi_Processor_లెక్క);

        పూర్ణాంకం clock_రేటు;
        cuda_సురక్షిత_Call(cudaDeviceGetAttribute(&clock_రేటు, cuda_Dev_Attr_Clock_రేటు, dev));
        రాయి("  GPU Clock Speed:                               %.2f GHz\n", clock_రేటు * 1e-6f);

        రాయి("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        రాయి("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        రాయి("  Total amount of constant memory:               %u bytes\n", (పూర్ణాంకం)prop.మొత్తం_Const_Mem);
        రాయి("  Total amount of shared memory per block:       %u bytes\n", (పూర్ణాంకం)prop.shared_Mem_చొప్పున_ఖండం);
        రాయి("  Total number of registers available per block: %d\n", prop.regs_చొప్పున_ఖండం);
        రాయి("  Warp size:                                     %d\n", prop.warp_కొలత);
        రాయి("  Maximum number of threads per block:           %d\n", prop.max_దారాలు_చొప్పున_ఖండం);
        రాయి("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.max_దారాలు_Dim[0], prop.max_దారాలు_Dim[1], prop.max_దారాలు_Dim[2]);
        రాయి("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_కొలత[0], prop.max_Grid_కొలత[1],  prop.max_Grid_కొలత[2]);
        రాయి("  Maximum memory pitch:                          %u bytes\n", (పూర్ణాంకం)prop.memPitch);
        రాయి("  Texture alignment:                             %u bytes\n", (పూర్ణాంకం)prop.textureAlignment);

        రాయి("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_లెక్క ? "Yes" : "No"), prop.async_Engine_లెక్క);
        పూర్ణాంకం kernelExecTimeoutEnabled;
        cuda_సురక్షిత_Call(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        రాయి("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        రాయి("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        రాయి("  Support host page-locked memory mapping:       %s\n", prop.can_Map_Host_జ్ఞాపకం ? "Yes" : "No");

        రాయి("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        రాయి("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        రాయి("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        రాయి("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        రాయి("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        రాయి("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        పూర్ణాంకం prop_Compute_బహుళకం;
        cuda_సురక్షిత_Call(cudaDeviceGetAttribute(&prop_Compute_బహుళకం, cuda_Dev_Attr_Compute_బహుళకం, dev));
        రాయి("  Compute Mode:\n");
        రాయి("      %s \n", compute_బహుళకం[prop_Compute_బహుళకం]);
    }

    రాయి("\n");
    రాయి("deviceQuery, CUDA Driver = CUDART");
    రాయి(", CUDA Driver Version  = %d.%d", driver_సంస్కరణ / 1000, driver_సంస్కరణ % 100);
    రాయి(", CUDA Runtime Version = %d.%d", runtime_సంస్కరణ/1000, runtime_సంస్కరణ%100);
    రాయి(", NumDevs = %d\n\n", లెక్క_2);

    fflush(stdout);
#అయితే_ముగింపు
}

శూన్య cv::cuda::printShortCudaDeviceInfo(పూర్ణాంకం device)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(device);
    throw_no_cuda();
#లేకపోతే
    పూర్ణాంకం లెక్క_2 = get_Cuda_Enabled_Device_లెక్క();
    తార్కిక valid = (device >= 0) && (device < లెక్క_2);

    పూర్ణాంకం beg = valid ? device   : 0;
    పూర్ణాంకం end = valid ? device+1 : లెక్క_2;

    పూర్ణాంకం driver_సంస్కరణ = 0, runtime_సంస్కరణ = 0;
    cuda_సురక్షిత_Call( cuda_Driver_Get_సంస్కరణ(&driver_సంస్కరణ) );
    cuda_సురక్షిత_Call( cuda_Runtime_Get_సంస్కరణ(&runtime_సంస్కరణ) );

    కొరకు(పూర్ణాంకం dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cuda_సురక్షిత_Call( cudaGetDeviceProperties(&prop, dev) );

        స్థిరరాశి అక్షరం *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        రాయి("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (దశాంశం)prop.మొత్తం_Global_Mem/1048576.0f);
        రాయి(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        పూర్ణాంకం cores = convertSMVer2Cores(prop.major, prop.minor);
        ఒకవేళ (cores > 0)
            రాయి(", %d cores", cores * prop.multi_Processor_లెక్క);

        రాయి(", Driver/Runtime ver.%d.%d/%d.%d\n", driver_సంస్కరణ/1000, driver_సంస్కరణ%100, runtime_సంస్కరణ/1000, runtime_సంస్కరణ%100);
    }

    fflush(stdout);
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// Error handling

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ
{
    #నిర్వచించు error_నమోదు(నమోదు_2)  { నమోదు_2, #entry }

    నిర్మాణం Error_నమోదు
    {
        పూర్ణాంకం సంకేతం_2;
        స్థిరరాశి అక్షరం* str;
    };

    నిర్మాణం Error_నమోదు_Comparer
    {
        పూర్ణాంకం సంకేతం_2;
        Error_నమోదు_Comparer(పూర్ణాంకం సంకేతం_3) : సంకేతం_2(సంకేతం_3) {}
        తార్కిక క్రియాకారకం()(స్థిరరాశి Error_నమోదు& e) స్థిరరాశి { తిరిగి e.సంకేతం_2 == సంకేతం_2; }
    };

    స్థిరరాశి Error_నమోదు npp_errors [] =
    {
    #ఒకవేళ defined (_MSC_VER)
        error_నమోదు( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #అయితే_ముగింపు

    #ఒకవేళ NPP_సంస్కరణ < 5500
        error_నమోదు( NPP_BAD_ARG_ERROR ),
        error_నమోదు( NPP_COEFF_ERROR ),
        error_నమోదు( NPP_RECT_ERROR ),
        error_నమోదు( NPP_QUAD_ERROR ),
        error_నమోదు( NPP_MEMFREE_ERR ),
        error_నమోదు( NPP_MEMSET_ERR ),
        error_నమోదు( NPP_MEM_ALLOC_ERR ),
        error_నమోదు( NPP_HISTO_సంఖ్య_యొక్క_స్థాయిలు_ERROR ),
        error_నమోదు( NPP_MIRROR_FLIP_ERR ),
        error_నమోదు( NPP_INVALID_ఆదానం ),
        error_నమోదు( NPP_సూచిక_ERROR ),
        error_నమోదు( NPP_WARNING ),
        error_నమోదు( NPP_ODD_ROI_WARNING ),
    #లేకపోతే
        error_నమోదు( NPP_INVALID_HOST_సూచిక_ERROR ),
        error_నమోదు( NPP_INVALID_DEVICE_సూచిక_ERROR ),
        error_నమోదు( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_నమోదు( NPP_ZC_బహుళకం_NOT_SUPPORTED_ERROR ),
        error_నమోదు( NPP_MEMFREE_ERROR ),
        error_నమోదు( NPP_MEMSET_ERROR ),
        error_నమోదు( NPP_QUALITY_సూచీ_ERROR ),
        error_నమోదు( NPP_HISTOGRAM_సంఖ్య_యొక్క_స్థాయిలు_ERROR ),
        error_నమోదు( NPP_CHANNEL_క్రమం_ERROR ),
        error_నమోదు( NPP_ZERO_ముసుగు_విలువ_ERROR ),
        error_నమోదు( NPP_QUADRANGLE_ERROR ),
        error_నమోదు( NPP_RECTANGLE_ERROR ),
        error_నమోదు( NPP_COEFFICIENT_ERROR ),
        error_నమోదు( NPP_సంఖ్య_యొక్క_CHANNELS_ERROR ),
        error_నమోదు( NPP_COI_ERROR ),
        error_నమోదు( NPP_భాజకం_ERROR ),
        error_నమోదు( NPP_CHANNEL_ERROR ),
        error_నమోదు( NPP_STRIDE_ERROR ),
        error_నమోదు( NPP_ANCHOR_ERROR ),
        error_నమోదు( NPP_ముసుగు_కొలత_ERROR ),
        error_నమోదు( NPP_MIRROR_FLIP_ERROR ),
        error_నమోదు( NPP_MOMENT_00_ZERO_ERROR ),
        error_నమోదు( NPP_పరిమితి_NEGATIVE_LEVEL_ERROR ),
        error_నమోదు( NPP_పరిమితి_ERROR ),
        error_నమోదు( NPP_CONTEXT_సరిపోలిక_ERROR ),
        error_నమోదు( NPP_FFT_FLAG_ERROR ),
        error_నమోదు( NPP_FFT_క్రమం_ERROR ),
        error_నమోదు( NPP_SCALE_RANGE_ERROR ),
        error_నమోదు( NPP_దత్తాంశం_TYPE_ERROR ),
        error_నమోదు( NPP_బహిర్గతం_OFF_RANGE_ERROR ),
        error_నమోదు( NPP_DIVIDE_BY_ZERO_ERROR ),
        error_నమోదు( NPP_జ్ఞాపకం_కేటాయింపు_ERR ),
        error_నమోదు( NPP_RANGE_ERROR ),
        error_నమోదు( NPP_BAD_వాదం_ERROR ),
        error_నమోదు( NPP_NO_జ్ఞాపకం_ERROR ),
        error_నమోదు( NPP_ERROR_RESERVED ),
        error_నమోదు( NPP_NO_OPERATION_WARNING ),
        error_నమోదు( NPP_DIVIDE_BY_ZERO_WARNING ),
        error_నమోదు( NPP_WRONG_INTERSECTION_ROI_WARNING ),
    #అయితే_ముగింపు

        error_నమోదు( NPP_NOT_SUPPORTED_బహుళకం_ERROR ),
        error_నమోదు( NPP_విడత_బహుళకం_NOT_SUPPORTED_ERROR ),
        error_నమోదు( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_నమోదు( NPP_LUT_సంఖ్య_యొక్క_స్థాయిలు_ERROR ),
        error_నమోదు( NPP_TEXTURE_BIND_ERROR ),
        error_నమోదు( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_నమోదు( NPP_NOT_EVEN_అడుగు_ERROR ),
        error_నమోదు( NPP_INTERPOLATION_ERROR ),
        error_నమోదు( NPP_RESIZE_గుణకం_ERROR ),
        error_నమోదు( NPP_HAAR_CLASSIFIER_PIXEL_సరిపోలిక_ERROR ),
        error_నమోదు( NPP_MEMCPY_ERROR ),
        error_నమోదు( NPP_ALIGNMENT_ERROR ),
        error_నమోదు( NPP_అడుగు_ERROR ),
        error_నమోదు( NPP_కొలత_ERROR ),
        error_నమోదు( NPP_NULL_సూచిక_ERROR ),
        error_నమోదు( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_నమోదు( NPP_NOT_IMPLEMENTED_ERROR ),
        error_నమోదు( NPP_ERROR ),
        error_నమోదు( NPP_NO_ERROR ),
        error_నమోదు( NPP_SUCCESS ),
        error_నమోదు( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_నమోదు( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_నమోదు( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_నమోదు( NPP_DOUBLE_కొలత_WARNING )
    };

    స్థిరరాశి size_t npp_error_అంకె = పరిమాణం(npp_errors) / పరిమాణం(npp_errors[0]);

    స్థిరరాశి Error_నమోదు cu_errors [] =
    {
        error_నమోదు( CUDA_SUCCESS                              ),
        error_నమోదు( CUDA_ERROR_INVALID_విలువ                  ),
        error_నమోదు( CUDA_ERROR_బహిర్గతం_యొక్క_జ్ఞాపకం                  ),
        error_నమోదు( CUDA_ERROR_NOT_INITIALIZED                ),
        error_నమోదు( CUDA_ERROR_DEINITIALIZED                  ),
        error_నమోదు( CUDA_ERROR_PROFILER_DISABLED              ),
        error_నమోదు( CUDA_ERROR_PROFILER_NOT_INITIALIZED       ),
        error_నమోదు( CUDA_ERROR_PROFILER_ALREADY_STARTED       ),
        error_నమోదు( CUDA_ERROR_PROFILER_ALREADY_STOPPED       ),
        error_నమోదు( CUDA_ERROR_NO_DEVICE                      ),
        error_నమోదు( CUDA_ERROR_INVALID_DEVICE                 ),
        error_నమోదు( CUDA_ERROR_INVALID_IMAGE                  ),
        error_నమోదు( CUDA_ERROR_INVALID_CONTEXT                ),
        error_నమోదు( CUDA_ERROR_CONTEXT_ALREADY_CURRENT        ),
        error_నమోదు( CUDA_ERROR_MAP_FAILED                     ),
        error_నమోదు( CUDA_ERROR_UNMAP_FAILED                   ),
        error_నమోదు( CUDA_ERROR_ARRAY_IS_MAPPED                ),
        error_నమోదు( CUDA_ERROR_ALREADY_MAPPED                 ),
        error_నమోదు( CUDA_ERROR_NO_ద్విమాన_FOR_GPU              ),
        error_నమోదు( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_నమోదు( CUDA_ERROR_NOT_MAPPED                     ),
        error_నమోదు( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_నమోదు( CUDA_ERROR_NOT_MAPPED_AS_సూచిక          ),
        error_నమోదు( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_నమోదు( CUDA_ERROR_UNSUPPORTED_హద్దు              ),
        error_నమోదు( CUDA_ERROR_CONTEXT_ALREADY_లోపల_USE         ),
        error_నమోదు( CUDA_ERROR_INVALID_ఆధారం                 ),
        error_నమోదు( CUDA_ERROR_FILE_NOT_దొరికింది                 ),
        error_నమోదు( CUDA_ERROR_SHARED_OBJECT_చిహ్నం_NOT_దొరికింది ),
        error_నమోదు( CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      ),
        error_నమోదు( CUDA_ERROR_OPERATING_SYSTEM               ),
        error_నమోదు( CUDA_ERROR_INVALID_HANDLE                 ),
        error_నమోదు( CUDA_ERROR_NOT_దొరికింది                      ),
        error_నమోదు( CUDA_ERROR_NOT_READY                      ),
        error_నమోదు( CUDA_ERROR_LAUNCH_FAILED                  ),
        error_నమోదు( CUDA_ERROR_LAUNCH_బహిర్గతం_యొక్క_RESOURCES        ),
        error_నమోదు( CUDA_ERROR_LAUNCH_TIMEOUT                 ),
        error_నమోదు( CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  ),
        error_నమోదు( CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED    ),
        error_నమోదు( CUDA_ERROR_PEER_ACCESS_NOT_ENABLED        ),
        error_నమోదు( CUDA_ERROR_PRIMARY_CONTEXT_చురుకైన         ),
        error_నమోదు( CUDA_ERROR_CONTEXT_IS_DESTROYED           ),
        error_నమోదు( CUDA_ERROR_ASSERT                         ),
        error_నమోదు( CUDA_ERROR_TOO_MANY_PEERS                 ),
        error_నమోదు( CUDA_ERROR_HOST_జ్ఞాపకం_ALREADY_REGISTERED ),
        error_నమోదు( CUDA_ERROR_HOST_జ్ఞాపకం_NOT_REGISTERED     ),
        error_నమోదు( CUDA_ERROR_UNKNOWN                        )
    };

    స్థిరరాశి size_t cu_errors_అంకె = పరిమాణం(cu_errors) / పరిమాణం(cu_errors[0]);

    cv::String getErrorString(పూర్ణాంకం సంకేతం_2, స్థిరరాశి Error_నమోదు* errors, size_t n)
    {
        size_t idx = std::find_if(errors, errors + n, Error_నమోదు_Comparer(సంకేతం_2)) - errors;

        స్థిరరాశి అక్షరం* సందేశం = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", సందేశం, సంకేతం_2);

        తిరిగి str;
    }
}

#అయితే_ముగింపు

String cv::cuda::get_Npp_Error_సందేశం(పూర్ణాంకం సంకేతం_2)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(సంకేతం_2);
    తిరిగి String();
#లేకపోతే
    తిరిగి getErrorString(సంకేతం_2, npp_errors, npp_error_అంకె);
#అయితే_ముగింపు
}

String cv::cuda::get_Cuda_Driver_Api_Error_సందేశం(పూర్ణాంకం సంకేతం_2)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(సంకేతం_2);
    తిరిగి String();
#లేకపోతే
    తిరిగి getErrorString(సంకేతం_2, cu_errors, cu_errors_అంకె);
#అయితే_ముగింపు
}
