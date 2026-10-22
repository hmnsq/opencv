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

#समावेश "precomp.hpp"

using namespace cv;
using namespace cv::cuda;

पूर्णांक cv::cuda::get_Cuda_Enabled_Device_गिनती()
{
#अगर_अघोषित HAVE_CUDA
    वापस 0;
#अन्यथा
    पूर्णांक गिनती_2;
    cudaError_t error = cuda_Get_Device_गिनती(&गिनती_2);

    अगर (error == cudaErrorInsufficientDriver)
        वापस -1;

    अगर (error == cudaErrorNoDevice)
        वापस 0;

    cudaSafeCall( error );
    वापस गिनती_2;
#पूर्ण_अगर
}

व्योम cv::cuda::setDevice(पूर्णांक device)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaSetDevice(device) );
    cudaSafeCall( cudaFree(0) );
#पूर्ण_अगर
}

पूर्णांक cv::cuda::getDevice()
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णांक device;
    cudaSafeCall( cudaGetDevice(&device) );
    वापस device;
#पूर्ण_अगर
}

व्योम cv::cuda::resetDevice()
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaDeviceReset() );
#पूर्ण_अगर
}

bool cv::cuda::deviceSupports(FeatureSet feature_set)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#अन्यथा
    अटल पूर्णांक versions[] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    अटल स्थिर पूर्णांक cache_आकार = static_cast<पूर्णांक>(माप(versions) / माप(versions[0]));

    स्थिर पूर्णांक devId = getDevice();

    पूर्णांक version;

    अगर (devId < cache_आकार && versions[devId] >= 0)
    {
        version = versions[devId];
    }
    अन्यथा
    {
        DeviceInfo dev(devId);
        version = dev.majorVersion() * 10 + dev.minorVersion();
        अगर (devId < cache_आकार)
            versions[devId] = version;
    }

    वापस TargetArchs::builtWith(feature_set) && (version >= feature_set);
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// TargetArchs

#अगर_घोषित HAVE_CUDA

namespace
{
    श्रेणी CudaArch
    {
    खुला:
        CudaArch();

        bool builtWith(FeatureSet feature_set) स्थिर;
        bool hasPtx(पूर्णांक major, पूर्णांक minor) स्थिर;
        bool hasBin(पूर्णांक major, पूर्णांक minor) स्थिर;
        bool hasEqualOrLessPtx(पूर्णांक major, पूर्णांक minor) स्थिर;
        bool hasEqualOrGreaterPtx(पूर्णांक major, पूर्णांक minor) स्थिर;
        bool hasEqualOrGreaterBin(पूर्णांक major, पूर्णांक minor) स्थिर;

    निजी:
        अटल व्योम fromStr(स्थिर अक्षर* set_as_str, std::vector<पूर्णांक>& arr);

        std::vector<पूर्णांक> bin;
        std::vector<पूर्णांक> ptx;
        std::vector<पूर्णांक> features;
    };

    स्थिर CudaArch cudaArch;

    CudaArch::CudaArch()
    {
        fromStr(CUDA_ARCH_BIN, bin);
        fromStr(CUDA_ARCH_PTX, ptx);
        fromStr(CUDA_ARCH_FEATURES, features);
    }

    bool CudaArch::builtWith(FeatureSet feature_set) स्थिर
    {
        वापस !features.empty() && (features.back() >= feature_set);
    }

    bool CudaArch::hasPtx(पूर्णांक major, पूर्णांक minor) स्थिर
    {
        वापस std::find(ptx.begin(), ptx.end(), major * 10 + minor) != ptx.end();
    }

    bool CudaArch::hasBin(पूर्णांक major, पूर्णांक minor) स्थिर
    {
        वापस std::find(bin.begin(), bin.end(), major * 10 + minor) != bin.end();
    }

    bool CudaArch::hasEqualOrLessPtx(पूर्णांक major, पूर्णांक minor) स्थिर
    {
        वापस !ptx.empty() && (ptx.front() <= major * 10 + minor);
    }

    bool CudaArch::hasEqualOrGreaterPtx(पूर्णांक major, पूर्णांक minor) स्थिर
    {
        वापस !ptx.empty() && (ptx.back() >= major * 10 + minor);
    }

    bool CudaArch::hasEqualOrGreaterBin(पूर्णांक major, पूर्णांक minor) स्थिर
    {
        वापस !bin.empty() && (bin.back() >= major * 10 + minor);
    }

    व्योम CudaArch::fromStr(स्थिर अक्षर* set_as_str, std::vector<पूर्णांक>& arr)
    {
        arr.सफा();

        स्थिर माप_प्रकार len = म_माप(set_as_str);

        माप_प्रकार pos = 0;
        जबतक (pos < len)
        {
            अगर (है_खाली(set_as_str[pos]))
            {
                ++pos;
            }
            अन्यथा
            {
                पूर्णांक cur_मूल्य;
                पूर्णांक chars_read;
                पूर्णांक args_read = माला_पूछो(set_as_str + pos, "%d%n", &cur_मूल्य, &chars_read);
                CV_Assert( args_read == 1 );

                arr.push_back(cur_मूल्य);
                pos += chars_read;
            }
        }

        std::sort(arr.begin(), arr.end());
    }
}

#पूर्ण_अगर

bool cv::cuda::TargetArchs::builtWith(cv::cuda::FeatureSet feature_set)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(feature_set);
    throw_no_cuda();
#अन्यथा
    वापस cudaArch.builtWith(feature_set);
#पूर्ण_अगर
}

bool cv::cuda::TargetArchs::hasPtx(पूर्णांक major, पूर्णांक minor)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    वापस cudaArch.hasPtx(major, minor);
#पूर्ण_अगर
}

bool cv::cuda::TargetArchs::hasBin(पूर्णांक major, पूर्णांक minor)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    वापस cudaArch.hasBin(major, minor);
#पूर्ण_अगर
}

bool cv::cuda::TargetArchs::hasEqualOrLessPtx(पूर्णांक major, पूर्णांक minor)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    वापस cudaArch.hasEqualOrLessPtx(major, minor);
#पूर्ण_अगर
}

bool cv::cuda::TargetArchs::hasEqualOrGreaterPtx(पूर्णांक major, पूर्णांक minor)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    वापस cudaArch.hasEqualOrGreaterPtx(major, minor);
#पूर्ण_अगर
}

bool cv::cuda::TargetArchs::hasEqualOrGreaterBin(पूर्णांक major, पूर्णांक minor)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(major);
    CV_UNUSED(minor);
    throw_no_cuda();
#अन्यथा
    वापस cudaArch.hasEqualOrGreaterBin(major, minor);
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// DeviceInfo

#अगर_घोषित HAVE_CUDA

namespace
{
    श्रेणी DeviceProps
    {
    खुला:
        DeviceProps();

        स्थिर cudaDeviceProp* लो(पूर्णांक devID) स्थिर;

    निजी:
        std::vector<cudaDeviceProp> props_;
    };

    DeviceProps::DeviceProps()
    {
        पूर्णांक गिनती_2 = get_Cuda_Enabled_Device_गिनती();

        अगर (गिनती_2 > 0)
        {
            props_.resize(गिनती_2);

            क्रम (पूर्णांक devID = 0; devID < गिनती_2; ++devID)
            {
                cudaSafeCall( cudaGetDeviceProperties(&props_[devID], devID) );
            }
        }
    }

    स्थिर cudaDeviceProp* DeviceProps::लो(पूर्णांक devID) स्थिर
    {
        CV_Assert( static_cast<माप_प्रकार>(devID) < props_.size() );

        वापस &props_[devID];
    }

    DeviceProps& deviceProps()
    {
        अटल DeviceProps props;
        वापस props;
    }
}

#पूर्ण_अगर

स्थिर अक्षर* cv::cuda::DeviceInfo::name() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->name;
#पूर्ण_अगर
}

माप_प्रकार cv::cuda::DeviceInfo::योग_Global_Mem() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->योग_Global_Mem;
#पूर्ण_अगर
}

माप_प्रकार cv::cuda::DeviceInfo::sharedMemPerBlock() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->sharedMemPerBlock;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::regsPerBlock() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->regsPerBlock;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::warp_आकार() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->warp_आकार;
#पूर्ण_अगर
}

माप_प्रकार cv::cuda::DeviceInfo::memPitch() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->memPitch;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::maxThreadsPerBlock() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->maxThreadsPerBlock;
#पूर्ण_अगर
}

Vec3i cv::cuda::DeviceInfo::maxThreadsDim() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec3i(deviceProps().लो(device_id_)->maxThreadsDim);
#पूर्ण_अगर
}

Vec3i cv::cuda::DeviceInfo::max_Grid_आकार() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec3i(deviceProps().लो(device_id_)->max_Grid_आकार);
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::clockRate() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णांक clockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, device_id_));
    वापस clockRate;
#पूर्ण_अगर
}

माप_प्रकार cv::cuda::DeviceInfo::योग_Const_Mem() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->योग_Const_Mem;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::majorVersion() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->major;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::minorVersion() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->minor;
#पूर्ण_अगर
}

माप_प्रकार cv::cuda::DeviceInfo::textureAlignment() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->textureAlignment;
#पूर्ण_अगर
}

माप_प्रकार cv::cuda::DeviceInfo::texturePitchAlignment() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->texturePitchAlignment;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::multi_Processor_गिनती() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->multi_Processor_गिनती;
#पूर्ण_अगर
}

bool cv::cuda::DeviceInfo::kernelExecTimeoutEnabled() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णांक kernelExecTimeoutEnabled;
    cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, device_id_));
    वापस kernelExecTimeoutEnabled != 0;
#पूर्ण_अगर
}

bool cv::cuda::DeviceInfo::integrated() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->integrated != 0;
#पूर्ण_अगर
}

bool cv::cuda::DeviceInfo::canMapHostMemory() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->canMapHostMemory != 0;
#पूर्ण_अगर
}

DeviceInfo::ComputeMode cv::cuda::DeviceInfo::computeMode() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    अटल स्थिर ComputeMode tbl[] =
    {
        ComputeModeDefault,
        ComputeModeExclusive,
        ComputeModeProhibited,
        ComputeModeExclusiveProcess
    };

    पूर्णांक computeMode;
    cudaSafeCall(cudaDeviceGetAttribute(&computeMode, cudaDevAttrComputeMode, device_id_));
    वापस tbl[computeMode];
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::maxTexture1D() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->maxTexture1D;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::maxTexture1DMipmap() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #अगर CUDA_VERSION >= 5000
        वापस deviceProps().लो(device_id_)->maxTexture1DMipmap;
    #अन्यथा
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        वापस 0;
    #पूर्ण_अगर
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::maxTexture1DLinear() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #अगर CUDA_VERSION >= 13000
        माप_प्रकार maxWidthInElements;
        cudaChannelFormatDesc fmtDesc = cudaCreateChannelDesc<float4>();
        cudaSafeCall(cudaDeviceGetTexture1DLinearMaxWidth(&maxWidthInElements, &fmtDesc, device_id_));
        वापस maxWidthInElements;
    #अन्यथा
        वापस deviceProps().लो(device_id_)->maxTexture1DLinear;
    #पूर्ण_अगर
#पूर्ण_अगर
}

Vec2i cv::cuda::DeviceInfo::maxTexture2D() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec2i(deviceProps().लो(device_id_)->maxTexture2D);
#पूर्ण_अगर
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DMipmap() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    #अगर CUDA_VERSION >= 5000
        वापस Vec2i(deviceProps().लो(device_id_)->maxTexture2DMipmap);
    #अन्यथा
        CV_Error(Error::StsNotImplemented, "This function requires CUDA 5.0");
        वापस Vec2i();
    #पूर्ण_अगर
#पूर्ण_अगर
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLinear() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec3i(deviceProps().लो(device_id_)->maxTexture2DLinear);
#पूर्ण_अगर
}

Vec2i cv::cuda::DeviceInfo::maxTexture2DGather() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec2i(deviceProps().लो(device_id_)->maxTexture2DGather);
#पूर्ण_अगर
}

Vec3i cv::cuda::DeviceInfo::maxTexture3D() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec3i(deviceProps().लो(device_id_)->maxTexture3D);
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::maxTextureCubemap() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->maxTextureCubemap;
#पूर्ण_अगर
}

Vec2i cv::cuda::DeviceInfo::maxTexture1DLayered() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec2i(deviceProps().लो(device_id_)->maxTexture1DLayered);
#पूर्ण_अगर
}

Vec3i cv::cuda::DeviceInfo::maxTexture2DLayered() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec3i(deviceProps().लो(device_id_)->maxTexture2DLayered);
#पूर्ण_अगर
}

Vec2i cv::cuda::DeviceInfo::maxTextureCubemapLayered() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec2i(deviceProps().लो(device_id_)->maxTextureCubemapLayered);
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::maxSurface1D() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->maxSurface1D;
#पूर्ण_अगर
}

Vec2i cv::cuda::DeviceInfo::maxSurface2D() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec2i(deviceProps().लो(device_id_)->maxSurface2D);
#पूर्ण_अगर
}

Vec3i cv::cuda::DeviceInfo::maxSurface3D() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec3i(deviceProps().लो(device_id_)->maxSurface3D);
#पूर्ण_अगर
}

Vec2i cv::cuda::DeviceInfo::maxSurface1DLayered() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec2i(deviceProps().लो(device_id_)->maxSurface1DLayered);
#पूर्ण_अगर
}

Vec3i cv::cuda::DeviceInfo::maxSurface2DLayered() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec3i(deviceProps().लो(device_id_)->maxSurface2DLayered);
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::maxSurfaceCubemap() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->maxSurfaceCubemap;
#पूर्ण_अगर
}

Vec2i cv::cuda::DeviceInfo::maxSurfaceCubemapLayered() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस Vec2i(deviceProps().लो(device_id_)->maxSurfaceCubemapLayered);
#पूर्ण_अगर
}

माप_प्रकार cv::cuda::DeviceInfo::surfaceAlignment() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->surfaceAlignment;
#पूर्ण_अगर
}

bool cv::cuda::DeviceInfo::concurrentKernels() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->concurrentKernels != 0;
#पूर्ण_अगर
}

bool cv::cuda::DeviceInfo::ECCEnabled() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->ECCEnabled != 0;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::pciBusID() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->pciBusID;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::pciDeviceID() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->pciDeviceID;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::pciDomainID() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->pciDomainID;
#पूर्ण_अगर
}

bool cv::cuda::DeviceInfo::tccDriver() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->tccDriver != 0;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::async_Engine_गिनती() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->async_Engine_गिनती;
#पूर्ण_अगर
}

bool cv::cuda::DeviceInfo::unifiedAddressing() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->unifiedAddressing != 0;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::memoryClockRate() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    पूर्णांक memoryClockRate;
    cudaSafeCall(cudaDeviceGetAttribute(&memoryClockRate, cudaDevAttrMemoryClockRate, device_id_));
    वापस memoryClockRate;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::memoryBusWidth() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->memoryBusWidth;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::l2_Cache_आकार() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->l2_Cache_आकार;
#पूर्ण_अगर
}

पूर्णांक cv::cuda::DeviceInfo::maxThreadsPerMultiProcessor() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    वापस deviceProps().लो(device_id_)->maxThreadsPerMultiProcessor;
#पूर्ण_अगर
}

व्योम cv::cuda::DeviceInfo::queryMemory(माप_प्रकार& योग_Memory, माप_प्रकार& _freeMemory) स्थिर
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(योग_Memory);
    CV_UNUSED(_freeMemory);
    throw_no_cuda();
#अन्यथा
    पूर्णांक पूर्व_Device_ID = getDevice();
    अगर (पूर्व_Device_ID != device_id_)
        setDevice(device_id_);

    cudaSafeCall( cudaMemGetInfo(&_freeMemory, &योग_Memory) );

    अगर (पूर्व_Device_ID != device_id_)
        setDevice(पूर्व_Device_ID);
#पूर्ण_अगर
}

bool cv::cuda::DeviceInfo::isCompatible() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    // Check PTX compatibility
    अगर (TargetArchs::hasEqualOrLessPtx(majorVersion(), minorVersion()))
        वापस true;

    // Check BIN compatibility
    क्रम (पूर्णांक i = minorVersion(); i >= 0; --i)
        अगर (TargetArchs::hasBin(majorVersion(), i))
            वापस true;

    वापस false;
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// print info

#अगर_घोषित HAVE_CUDA

namespace
{
    पूर्णांक convertSMVer2Cores(पूर्णांक major, पूर्णांक minor)
    {
        // Defines for GPU Architecture types (using the SM version to determine the # of cores per SM
        प्रकार काष्ठा {
            पूर्णांक SM; // 0xMm (hexadecimal notation), M = SM Major version, and m = SM minor version
            पूर्णांक Cores;
        } SMtoCores;

        SMtoCores gpuArchCoresPerSM[] =  { { 0x10,  8 }, { 0x11,  8 }, { 0x12,  8 }, { 0x13,  8 }, { 0x20, 32 }, { 0x21, 48 }, {0x30, 192}, {0x35, 192}, { -1, -1 }  };

        पूर्णांक सूचकांक = 0;
        जबतक (gpuArchCoresPerSM[सूचकांक].SM != -1)
        {
            अगर (gpuArchCoresPerSM[सूचकांक].SM == ((major << 4) + minor) )
                वापस gpuArchCoresPerSM[सूचकांक].Cores;
            सूचकांक++;
        }

        वापस -1;
    }
}

#पूर्ण_अगर

व्योम cv::cuda::printCudaDeviceInfo(पूर्णांक device)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#अन्यथा
    पूर्णांक गिनती_2 = get_Cuda_Enabled_Device_गिनती();
    bool valid = (device >= 0) && (device < गिनती_2);

    पूर्णांक beg = valid ? device   : 0;
    पूर्णांक end = valid ? device+1 : गिनती_2;

    म_लिखो("*** CUDA Device Query (Runtime API) version (CUDART static linking) *** \n\n");
    म_लिखो("Device count: %d\n", गिनती_2);

    पूर्णांक driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    स्थिर अक्षर *computeMode[] = {
        "Default (multiple host threads can use ::cudaSetDevice() with device simultaneously)",
        "Exclusive (only one host thread in one process is able to use ::cudaSetDevice() with this device)",
        "Prohibited (no host thread can use ::cudaSetDevice() with this device)",
        "Exclusive Process (many threads in one process is able to use ::cudaSetDevice() with this device)",
        "Unknown",
        शून्य
    };

    क्रम(पूर्णांक dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        म_लिखो("\nDevice %d: \"%s\"\n", dev, prop.name);
        म_लिखो("  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
        म_लिखो("  CUDA Capability Major/Minor version number:    %d.%d\n", prop.major, prop.minor);
        म_लिखो("  Total amount of global memory:                 %.0f MBytes (%llu bytes)\n", (भग्न)prop.योग_Global_Mem/1048576.0f, (अचिन्हित दीर्घ दीर्घ) prop.योग_Global_Mem);

        पूर्णांक cores = convertSMVer2Cores(prop.major, prop.minor);
        अगर (cores > 0)
            म_लिखो("  (%2d) Multiprocessors x (%2d) CUDA Cores/MP:     %d CUDA Cores\n", prop.multi_Processor_गिनती, cores, cores * prop.multi_Processor_गिनती);

        पूर्णांक clockRate;
        cudaSafeCall(cudaDeviceGetAttribute(&clockRate, cudaDevAttrClockRate, dev));
        म_लिखो("  GPU Clock Speed:                               %.2f GHz\n", clockRate * 1e-6f);

        म_लिखो("  Max Texture Dimension Size (x,y,z)             1D=(%d), 2D=(%d,%d), 3D=(%d,%d,%d)\n",
            prop.maxTexture1D, prop.maxTexture2D[0], prop.maxTexture2D[1],
            prop.maxTexture3D[0], prop.maxTexture3D[1], prop.maxTexture3D[2]);
        म_लिखो("  Max Layered Texture Size (dim) x layers        1D=(%d) x %d, 2D=(%d,%d) x %d\n",
            prop.maxTexture1DLayered[0], prop.maxTexture1DLayered[1],
            prop.maxTexture2DLayered[0], prop.maxTexture2DLayered[1], prop.maxTexture2DLayered[2]);

        म_लिखो("  Total amount of constant memory:               %u bytes\n", (पूर्णांक)prop.योग_Const_Mem);
        म_लिखो("  Total amount of shared memory per block:       %u bytes\n", (पूर्णांक)prop.sharedMemPerBlock);
        म_लिखो("  Total number of registers available per block: %d\n", prop.regsPerBlock);
        म_लिखो("  Warp size:                                     %d\n", prop.warp_आकार);
        म_लिखो("  Maximum number of threads per block:           %d\n", prop.maxThreadsPerBlock);
        म_लिखो("  Maximum sizes of each dimension of a block:    %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
        म_लिखो("  Maximum sizes of each dimension of a grid:     %d x %d x %d\n", prop.max_Grid_आकार[0], prop.max_Grid_आकार[1],  prop.max_Grid_आकार[2]);
        म_लिखो("  Maximum memory pitch:                          %u bytes\n", (पूर्णांक)prop.memPitch);
        म_लिखो("  Texture alignment:                             %u bytes\n", (पूर्णांक)prop.textureAlignment);

        म_लिखो("  Concurrent copy and execution:                 %s with %d copy engine(s)\n", (prop.async_Engine_गिनती ? "Yes" : "No"), prop.async_Engine_गिनती);
        पूर्णांक kernelExecTimeoutEnabled;
        cudaSafeCall(cudaDeviceGetAttribute(&kernelExecTimeoutEnabled, cudaDevAttrKernelExecTimeout, dev));
        म_लिखो("  Run time limit on kernels:                     %s\n", kernelExecTimeoutEnabled ? "Yes" : "No");
        म_लिखो("  Integrated GPU sharing Host Memory:            %s\n", prop.integrated ? "Yes" : "No");
        म_लिखो("  Support host page-locked memory mapping:       %s\n", prop.canMapHostMemory ? "Yes" : "No");

        म_लिखो("  Concurrent kernel execution:                   %s\n", prop.concurrentKernels ? "Yes" : "No");
        म_लिखो("  Alignment requirement for Surfaces:            %s\n", prop.surfaceAlignment ? "Yes" : "No");
        म_लिखो("  Device has ECC support enabled:                %s\n", prop.ECCEnabled ? "Yes" : "No");
        म_लिखो("  Device is using TCC driver mode:               %s\n", prop.tccDriver ? "Yes" : "No");
        म_लिखो("  Device supports Unified Addressing (UVA):      %s\n", prop.unifiedAddressing ? "Yes" : "No");
        म_लिखो("  Device PCI Bus ID / PCI location ID:           %d / %d\n", prop.pciBusID, prop.pciDeviceID );

        पूर्णांक propComputeMode;
        cudaSafeCall(cudaDeviceGetAttribute(&propComputeMode, cudaDevAttrComputeMode, dev));
        म_लिखो("  Compute Mode:\n");
        म_लिखो("      %s \n", computeMode[propComputeMode]);
    }

    म_लिखो("\n");
    म_लिखो("deviceQuery, CUDA Driver = CUDART");
    म_लिखो(", CUDA Driver Version  = %d.%d", driverVersion / 1000, driverVersion % 100);
    म_लिखो(", CUDA Runtime Version = %d.%d", runtimeVersion/1000, runtimeVersion%100);
    म_लिखो(", NumDevs = %d\n\n", गिनती_2);

    ख_साफ(मानक_निकास);
#पूर्ण_अगर
}

व्योम cv::cuda::printShortCudaDeviceInfo(पूर्णांक device)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(device);
    throw_no_cuda();
#अन्यथा
    पूर्णांक गिनती_2 = get_Cuda_Enabled_Device_गिनती();
    bool valid = (device >= 0) && (device < गिनती_2);

    पूर्णांक beg = valid ? device   : 0;
    पूर्णांक end = valid ? device+1 : गिनती_2;

    पूर्णांक driverVersion = 0, runtimeVersion = 0;
    cudaSafeCall( cudaDriverGetVersion(&driverVersion) );
    cudaSafeCall( cudaRuntimeGetVersion(&runtimeVersion) );

    क्रम(पूर्णांक dev = beg; dev < end; ++dev)
    {
        cudaDeviceProp prop;
        cudaSafeCall( cudaGetDeviceProperties(&prop, dev) );

        स्थिर अक्षर *arch_str = prop.major < 2 ? " (not Fermi)" : "";
        म_लिखो("Device %d:  \"%s\"  %.0fMb", dev, prop.name, (भग्न)prop.योग_Global_Mem/1048576.0f);
        म_लिखो(", sm_%d%d%s", prop.major, prop.minor, arch_str);

        पूर्णांक cores = convertSMVer2Cores(prop.major, prop.minor);
        अगर (cores > 0)
            म_लिखो(", %d cores", cores * prop.multi_Processor_गिनती);

        म_लिखो(", Driver/Runtime ver.%d.%d/%d.%d\n", driverVersion/1000, driverVersion%100, runtimeVersion/1000, runtimeVersion%100);
    }

    ख_साफ(मानक_निकास);
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// Error handling

#अगर_घोषित HAVE_CUDA

namespace
{
    #घोषणा error_entry(entry)  { entry, #entry }

    काष्ठा ErrorEntry
    {
        पूर्णांक code;
        स्थिर अक्षर* str;
    };

    काष्ठा ErrorEntryComparer
    {
        पूर्णांक code;
        ErrorEntryComparer(पूर्णांक code_) : code(code_) {}
        bool चालक()(स्थिर ErrorEntry& e) स्थिर { वापस e.code == code; }
    };

    स्थिर ErrorEntry npp_errors [] =
    {
    #अगर defined (_MSC_VER)
        error_entry( NPP_NOT_SUFFICIENT_COMPUTE_CAPABILITY ),
    #पूर्ण_अगर

    #अगर NPP_VERSION < 5500
        error_entry( NPP_BAD_ARG_ERROR ),
        error_entry( NPP_COEFF_ERROR ),
        error_entry( NPP_RECT_ERROR ),
        error_entry( NPP_QUAD_ERROR ),
        error_entry( NPP_MEMFREE_ERR ),
        error_entry( NPP_MEMSET_ERR ),
        error_entry( NPP_MEM_ALLOC_ERR ),
        error_entry( NPP_HISTO_संख्या_OF_LEVELS_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERR ),
        error_entry( NPP_INVALID_निवेश ),
        error_entry( NPP_POINTER_ERROR ),
        error_entry( NPP_WARNING ),
        error_entry( NPP_ODD_ROI_WARNING ),
    #अन्यथा
        error_entry( NPP_INVALID_HOST_POINTER_ERROR ),
        error_entry( NPP_INVALID_DEVICE_POINTER_ERROR ),
        error_entry( NPP_LUT_PALETTE_BITSIZE_ERROR ),
        error_entry( NPP_ZC_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_MEMFREE_ERROR ),
        error_entry( NPP_MEMSET_ERROR ),
        error_entry( NPP_QUALITY_सूचकांक_ERROR ),
        error_entry( NPP_HISTOGRAM_संख्या_OF_LEVELS_ERROR ),
        error_entry( NPP_CHANNEL_ORDER_ERROR ),
        error_entry( NPP_ZERO_MASK_मूल्य_ERROR ),
        error_entry( NPP_QUADRANGLE_ERROR ),
        error_entry( NPP_RECTANGLE_ERROR ),
        error_entry( NPP_COEFFICIENT_ERROR ),
        error_entry( NPP_संख्या_OF_CHANNELS_ERROR ),
        error_entry( NPP_COI_ERROR ),
        error_entry( NPP_भाजक_ERROR ),
        error_entry( NPP_CHANNEL_ERROR ),
        error_entry( NPP_STRIDE_ERROR ),
        error_entry( NPP_ANCHOR_ERROR ),
        error_entry( NPP_MASK_आकार_ERROR ),
        error_entry( NPP_MIRROR_FLIP_ERROR ),
        error_entry( NPP_MOMENT_00_ZERO_ERROR ),
        error_entry( NPP_THRESHOLD_NEGATIVE_LEVEL_ERROR ),
        error_entry( NPP_THRESHOLD_ERROR ),
        error_entry( NPP_CONTEXT_MATCH_ERROR ),
        error_entry( NPP_FFT_FLAG_ERROR ),
        error_entry( NPP_FFT_ORDER_ERROR ),
        error_entry( NPP_SCALE_RANGE_ERROR ),
        error_entry( NPP_तथ्य_TYPE_ERROR ),
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
    #पूर्ण_अगर

        error_entry( NPP_NOT_SUPPORTED_MODE_ERROR ),
        error_entry( NPP_ROUND_MODE_NOT_SUPPORTED_ERROR ),
        error_entry( NPP_RESIZE_NO_OPERATION_ERROR ),
        error_entry( NPP_LUT_संख्या_OF_LEVELS_ERROR ),
        error_entry( NPP_TEXTURE_BIND_ERROR ),
        error_entry( NPP_WRONG_INTERSECTION_ROI_ERROR ),
        error_entry( NPP_NOT_EVEN_STEP_ERROR ),
        error_entry( NPP_INTERPOLATION_ERROR ),
        error_entry( NPP_RESIZE_FACTOR_ERROR ),
        error_entry( NPP_HAAR_CLASSIFIER_PIXEL_MATCH_ERROR ),
        error_entry( NPP_MEMCPY_ERROR ),
        error_entry( NPP_ALIGNMENT_ERROR ),
        error_entry( NPP_STEP_ERROR ),
        error_entry( NPP_आकार_ERROR ),
        error_entry( NPP_NULL_POINTER_ERROR ),
        error_entry( NPP_CUDA_KERNEL_EXECUTION_ERROR ),
        error_entry( NPP_NOT_IMPLEMENTED_ERROR ),
        error_entry( NPP_ERROR ),
        error_entry( NPP_NO_ERROR ),
        error_entry( NPP_SUCCESS ),
        error_entry( NPP_WRONG_INTERSECTION_QUAD_WARNING ),
        error_entry( NPP_MISALIGNED_DST_ROI_WARNING ),
        error_entry( NPP_AFFINE_QUAD_INCORRECT_WARNING ),
        error_entry( NPP_DOUBLE_आकार_WARNING )
    };

    स्थिर माप_प्रकार npp_error_अंक = माप(npp_errors) / माप(npp_errors[0]);

    स्थिर ErrorEntry cu_errors [] =
    {
        error_entry( CUDA_SUCCESS                              ),
        error_entry( CUDA_ERROR_INVALID_मूल्य                  ),
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
        error_entry( CUDA_ERROR_NO_द्विआधारी_FOR_GPU              ),
        error_entry( CUDA_ERROR_ALREADY_ACQUIRED               ),
        error_entry( CUDA_ERROR_NOT_MAPPED                     ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_ARRAY            ),
        error_entry( CUDA_ERROR_NOT_MAPPED_AS_POINTER          ),
        error_entry( CUDA_ERROR_ECC_UNCORRECTABLE              ),
        error_entry( CUDA_ERROR_UNSUPPORTED_LIMIT              ),
        error_entry( CUDA_ERROR_CONTEXT_ALREADY_IN_USE         ),
        error_entry( CUDA_ERROR_INVALID_SOURCE                 ),
        error_entry( CUDA_ERROR_FILE_NOT_FOUND                 ),
        error_entry( CUDA_ERROR_SHARED_OBJECT_प्रतीक_NOT_FOUND ),
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

    स्थिर माप_प्रकार cu_errors_अंक = माप(cu_errors) / माप(cu_errors[0]);

    cv::String getErrorString(पूर्णांक code, स्थिर ErrorEntry* errors, माप_प्रकार n)
    {
        माप_प्रकार idx = std::find_if(errors, errors + n, ErrorEntryComparer(code)) - errors;

        स्थिर अक्षर* संदेश = (idx != n) ? errors[idx].str : "Unknown error code";
        cv::String str = cv::format("%s [Code = %d]", संदेश, code);

        वापस str;
    }
}

#पूर्ण_अगर

String cv::cuda::getNppErrorMessage(पूर्णांक code)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(code);
    वापस String();
#अन्यथा
    वापस getErrorString(code, npp_errors, npp_error_अंक);
#पूर्ण_अगर
}

String cv::cuda::getCudaDriverApiErrorMessage(पूर्णांक code)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(code);
    वापस String();
#अन्यथा
    वापस getErrorString(code, cu_errors, cu_errors_अंक);
#पूर्ण_अगर
}
