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
// Copyright (C) 2010-2013, Advanced Micro Devices, Inc., all rights reserved.
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
// This software is provided by the copyright holders and contributors as is and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the copyright holders or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#समावेश "precomp.hpp"

#समावेश "opencv2/core.hpp"
#समावेश "opencv2/core/ocl.hpp"
#समावेश "opencv2/core/directx.hpp"
#समावेश "opencl_kernels_core.hpp"

#अगर_घोषित HAVE_DIRECTX
#समावेश <vector>
#समावेश "directx.inc.hpp"
#अन्यथा // HAVE_DIRECTX
#घोषणा NO_DIRECTX_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without DirectX support")
#पूर्ण_अगर

#अगर_अघोषित HAVE_OPENCL
#घोषणा NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#पूर्ण_अगर // HAVE_OPENCL

using namespace cv::ocl;

namespace cv { namespace directx {

पूर्णांक getTypeFromDXGI_FORMAT(स्थिर पूर्णांक iDXGI_FORMAT)
{
    CV_UNUSED(iDXGI_FORMAT);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#अन्यथा
    स्थिर पूर्णांक errorType = -1;
    चयन ((क्रमागत DXGI_FORMAT)iDXGI_FORMAT)
    {
    //case DXGI_FORMAT_UNKNOWN:
    //case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    हाल DXGI_FORMAT_R32G32B32A32_FLOAT: वापस CV_32FC4;
    हाल DXGI_FORMAT_R32G32B32A32_UINT:
    हाल DXGI_FORMAT_R32G32B32A32_SINT:  वापस CV_32SC4;
    //case DXGI_FORMAT_R32G32B32_TYPELESS:
    हाल DXGI_FORMAT_R32G32B32_FLOAT: वापस CV_32FC3;
    हाल DXGI_FORMAT_R32G32B32_UINT:
    हाल DXGI_FORMAT_R32G32B32_SINT: वापस CV_32SC3;
    //case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    हाल DXGI_FORMAT_R16G16B16A16_FLOAT: वापस CV_16FC4;
    हाल DXGI_FORMAT_R16G16B16A16_UNORM:
    हाल DXGI_FORMAT_R16G16B16A16_UINT: वापस CV_16UC4;
    हाल DXGI_FORMAT_R16G16B16A16_SNORM:
    हाल DXGI_FORMAT_R16G16B16A16_SINT: वापस CV_16SC4;
    //case DXGI_FORMAT_R32G32_TYPELESS:
    हाल DXGI_FORMAT_R32G32_FLOAT: वापस CV_32FC2;
    हाल DXGI_FORMAT_R32G32_UINT:
    हाल DXGI_FORMAT_R32G32_SINT: वापस CV_32SC2;
    //case DXGI_FORMAT_R32G8X24_TYPELESS:
    //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    //case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    //case DXGI_FORMAT_R10G10B10A2_UNORM:
    //case DXGI_FORMAT_R10G10B10A2_UINT:
    //case DXGI_FORMAT_R11G11B10_FLOAT:
    //case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    हाल DXGI_FORMAT_R8G8B8A8_UNORM:
    हाल DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    हाल DXGI_FORMAT_R8G8B8A8_UINT: वापस CV_8UC4;
    हाल DXGI_FORMAT_R8G8B8A8_SNORM:
    हाल DXGI_FORMAT_R8G8B8A8_SINT: वापस CV_8SC4;
    //case DXGI_FORMAT_R16G16_TYPELESS:
    हाल DXGI_FORMAT_R16G16_FLOAT: वापस CV_16FC2;
    हाल DXGI_FORMAT_R16G16_UNORM:
    हाल DXGI_FORMAT_R16G16_UINT: वापस CV_16UC2;
    हाल DXGI_FORMAT_R16G16_SNORM:
    हाल DXGI_FORMAT_R16G16_SINT: वापस CV_16SC2;
    //case DXGI_FORMAT_R32_TYPELESS:
    हाल DXGI_FORMAT_D32_FLOAT:
    हाल DXGI_FORMAT_R32_FLOAT: वापस CV_32FC1;
    हाल DXGI_FORMAT_R32_UINT:
    हाल DXGI_FORMAT_R32_SINT: वापस CV_32SC1;
    //case DXGI_FORMAT_R24G8_TYPELESS:
    //case DXGI_FORMAT_D24_UNORM_S8_UINT:
    //case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    //case DXGI_FORMAT_R8G8_TYPELESS:
    हाल DXGI_FORMAT_R8G8_UNORM:
    हाल DXGI_FORMAT_R8G8_UINT: वापस CV_8UC2;
    हाल DXGI_FORMAT_R8G8_SNORM:
    हाल DXGI_FORMAT_R8G8_SINT: वापस CV_8SC2;
    //case DXGI_FORMAT_R16_TYPELESS:
    हाल DXGI_FORMAT_R16_FLOAT: वापस CV_16FC1;
    हाल DXGI_FORMAT_D16_UNORM:
    हाल DXGI_FORMAT_R16_UNORM:
    हाल DXGI_FORMAT_R16_UINT: वापस CV_16UC1;
    हाल DXGI_FORMAT_R16_SNORM:
    हाल DXGI_FORMAT_R16_SINT: वापस CV_16SC1;
    //case DXGI_FORMAT_R8_TYPELESS:
    हाल DXGI_FORMAT_R8_UNORM:
    हाल DXGI_FORMAT_R8_UINT: वापस CV_8UC1;
    हाल DXGI_FORMAT_R8_SNORM:
    हाल DXGI_FORMAT_R8_SINT: वापस CV_8SC1;
    हाल DXGI_FORMAT_A8_UNORM: वापस CV_8UC1;
    //case DXGI_FORMAT_R1_UNORM:
    //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    हाल DXGI_FORMAT_R8G8_B8G8_UNORM:
    हाल DXGI_FORMAT_G8R8_G8B8_UNORM: वापस CV_8UC4;
    //case DXGI_FORMAT_BC1_TYPELESS:
    //case DXGI_FORMAT_BC1_UNORM:
    //case DXGI_FORMAT_BC1_UNORM_SRGB:
    //case DXGI_FORMAT_BC2_TYPELESS:
    //case DXGI_FORMAT_BC2_UNORM:
    //case DXGI_FORMAT_BC2_UNORM_SRGB:
    //case DXGI_FORMAT_BC3_TYPELESS:
    //case DXGI_FORMAT_BC3_UNORM:
    //case DXGI_FORMAT_BC3_UNORM_SRGB:
    //case DXGI_FORMAT_BC4_TYPELESS:
    //case DXGI_FORMAT_BC4_UNORM:
    //case DXGI_FORMAT_BC4_SNORM:
    //case DXGI_FORMAT_BC5_TYPELESS:
    //case DXGI_FORMAT_BC5_UNORM:
    //case DXGI_FORMAT_BC5_SNORM:
    //case DXGI_FORMAT_B5G6R5_UNORM:
    //case DXGI_FORMAT_B5G5R5A1_UNORM:
    हाल DXGI_FORMAT_B8G8R8A8_UNORM:
    हाल DXGI_FORMAT_B8G8R8X8_UNORM: वापस CV_8UC4;
    //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    हाल DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: वापस CV_8UC4;
    //case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    हाल DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: वापस CV_8UC4;
    //case DXGI_FORMAT_BC6H_TYPELESS:
    //case DXGI_FORMAT_BC6H_UF16:
    //case DXGI_FORMAT_BC6H_SF16:
    //case DXGI_FORMAT_BC7_TYPELESS:
    //case DXGI_FORMAT_BC7_UNORM:
    //case DXGI_FORMAT_BC7_UNORM_SRGB:
#अगर_घोषित HAVE_DIRECTX_NV12 //D3DX11 should support DXGI_FORMAT_NV12.
    हाल DXGI_FORMAT_NV12: वापस CV_8UC3;
#पूर्ण_अगर
    शेष: अवरोध;
    }
    वापस errorType;
#पूर्ण_अगर
}

पूर्णांक getTypeFromD3DFORMAT(स्थिर पूर्णांक iD3DFORMAT)
{
    CV_UNUSED(iD3DFORMAT);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#अन्यथा
    स्थिर पूर्णांक errorType = -1;
    चयन ((क्रमागत _D3DFORMAT)iD3DFORMAT)
    {
    //case D3DFMT_UNKNOWN:
    हाल D3DFMT_R8G8B8: वापस CV_8UC3;
    हाल D3DFMT_A8R8G8B8:
    हाल D3DFMT_X8R8G8B8: वापस CV_8UC4;
    //case D3DFMT_R5G6B5:
    //case D3DFMT_X1R5G5B5:
    //case D3DFMT_A1R5G5B5:
    //case D3DFMT_A4R4G4B4:
    //case D3DFMT_R3G3B2:
    हाल D3DFMT_A8: वापस CV_8UC1;
    //case D3DFMT_A8R3G3B2:
    //case D3DFMT_X4R4G4B4:
    //case D3DFMT_A2B10G10R10:
    हाल D3DFMT_A8B8G8R8:
    हाल D3DFMT_X8B8G8R8: वापस CV_8UC4;
    //case D3DFMT_G16R16:
    //case D3DFMT_A2R10G10B10:
    //case D3DFMT_A16B16G16R16:

    हाल D3DFMT_A8P8: वापस CV_8UC2;
    हाल D3DFMT_P8: वापस CV_8UC1;

    हाल D3DFMT_L8: वापस CV_8UC1;
    हाल D3DFMT_A8L8: वापस CV_8UC2;
    //case D3DFMT_A4L4:

    हाल D3DFMT_V8U8: वापस CV_8UC2;
    //case D3DFMT_L6V5U5:
    हाल D3DFMT_X8L8V8U8:
    हाल D3DFMT_Q8W8V8U8: वापस CV_8UC4;
    हाल D3DFMT_V16U16: वापस CV_16UC4; // TODO 16SC4 ?
    //case D3DFMT_A2W10V10U10:

    हाल D3DFMT_D16_LOCKABLE: वापस CV_16UC1;
    हाल D3DFMT_D32: वापस CV_32SC1;
    //case D3DFMT_D15S1:
    //case D3DFMT_D24S8:
    //case D3DFMT_D24X8:
    //case D3DFMT_D24X4S4:
    हाल D3DFMT_D16: वापस CV_16UC1;

    हाल D3DFMT_D32F_LOCKABLE: वापस CV_32FC1;
    शेष: अवरोध;
    }
    वापस errorType;
#पूर्ण_अगर
}

#अगर defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)

#अगर_घोषित HAVE_OPENCL_D3D11_NV
श्रेणी OpenCL_D3D11_NV : खुला ocl::Context::UserContext
{
खुला:
    OpenCL_D3D11_NV(cl_platform_id platform, ID3D11Device*_device) : device(_device)
    {
        clCreateFromD3D11Texture2DNV = (clCreateFromD3D11Texture2DNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D11Texture2DNV");
        clEnqueueAcquireD3D11ObjectsNV = (clEnqueueAcquireD3D11ObjectsNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D11ObjectsNV");
        clEnqueueReleaseD3D11ObjectsNV = (clEnqueueReleaseD3D11ObjectsNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D11ObjectsNV");
        अगर (!clCreateFromD3D11Texture2DNV || !clEnqueueAcquireD3D11ObjectsNV || !clEnqueueReleaseD3D11ObjectsNV)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11_NV");
        }
        device->AddRef();
    }
    ~OpenCL_D3D11_NV() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DNV_fn clCreateFromD3D11Texture2DNV;
    clEnqueueAcquireD3D11ObjectsNV_fn clEnqueueAcquireD3D11ObjectsNV;
    clEnqueueReleaseD3D11ObjectsNV_fn clEnqueueReleaseD3D11ObjectsNV;
};
#पूर्ण_अगर

श्रेणी OpenCL_D3D11 : खुला ocl::Context::UserContext
{
खुला:
    OpenCL_D3D11(cl_platform_id platform, ID3D11Device* _device) : device(_device)
    {
        clCreateFromD3D11Texture2DKHR = (clCreateFromD3D11Texture2DKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D11Texture2DKHR");
        clEnqueueAcquireD3D11ObjectsKHR = (clEnqueueAcquireD3D11ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D11ObjectsKHR");
        clEnqueueReleaseD3D11ObjectsKHR = (clEnqueueReleaseD3D11ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D11ObjectsKHR");
        अगर (!clCreateFromD3D11Texture2DKHR || !clEnqueueAcquireD3D11ObjectsKHR || !clEnqueueReleaseD3D11ObjectsKHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11");
        }
        device->AddRef();
    }
    ~OpenCL_D3D11() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DKHR_fn clCreateFromD3D11Texture2DKHR;
    clEnqueueAcquireD3D11ObjectsKHR_fn clEnqueueAcquireD3D11ObjectsKHR;
    clEnqueueReleaseD3D11ObjectsKHR_fn clEnqueueReleaseD3D11ObjectsKHR;
};

श्रेणी OpenCL_D3D9 : खुला ocl::Context::UserContext
{
खुला:
    OpenCL_D3D9(cl_platform_id platform, IDirect3DDevice9* _device, IDirect3DDevice9Ex* _deviceEx)
        : device(_device)
        , deviceEx(_deviceEx)
    {
        clCreateFromDX9MediaSurfaceKHR = (clCreateFromDX9MediaSurfaceKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromDX9MediaSurfaceKHR");
        clEnqueueAcquireDX9MediaSurfacesKHR = (clEnqueueAcquireDX9MediaSurfacesKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireDX9MediaSurfacesKHR");
        clEnqueueReleaseDX9MediaSurfacesKHR = (clEnqueueReleaseDX9MediaSurfacesKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseDX9MediaSurfacesKHR");
        अगर (!clCreateFromDX9MediaSurfaceKHR || !clEnqueueAcquireDX9MediaSurfacesKHR || !clEnqueueReleaseDX9MediaSurfacesKHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D9");
        }
        अगर (device)
            device->AddRef();
        अगर (deviceEx)
            deviceEx->AddRef();
    }
    ~OpenCL_D3D9() {
        अगर (device)
            device->Release();
        अगर (deviceEx)
            deviceEx->Release();
    }
    IDirect3DDevice9* device;
    IDirect3DDevice9Ex* deviceEx;
    clCreateFromDX9MediaSurfaceKHR_fn clCreateFromDX9MediaSurfaceKHR;
    clEnqueueAcquireDX9MediaSurfacesKHR_fn clEnqueueAcquireDX9MediaSurfacesKHR;
    clEnqueueReleaseDX9MediaSurfacesKHR_fn clEnqueueReleaseDX9MediaSurfacesKHR;
};

श्रेणी OpenCL_D3D10 : खुला ocl::Context::UserContext
{
खुला:
    OpenCL_D3D10(cl_platform_id platform, ID3D10Device* _device) : device(_device)
    {
        clCreateFromD3D10Texture2DKHR = (clCreateFromD3D10Texture2DKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D10Texture2DKHR");
        clEnqueueAcquireD3D10ObjectsKHR = (clEnqueueAcquireD3D10ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D10ObjectsKHR");
        clEnqueueReleaseD3D10ObjectsKHR = (clEnqueueReleaseD3D10ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D10ObjectsKHR");
        अगर (!clCreateFromD3D10Texture2DKHR || !clEnqueueAcquireD3D10ObjectsKHR || !clEnqueueReleaseD3D10ObjectsKHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D10");
        }
        device->AddRef();
    }
    ~OpenCL_D3D10() {
        device->Release();
    }
    ID3D10Device* device;
    clCreateFromD3D10Texture2DKHR_fn clCreateFromD3D10Texture2DKHR;
    clEnqueueAcquireD3D10ObjectsKHR_fn clEnqueueAcquireD3D10ObjectsKHR;
    clEnqueueReleaseD3D10ObjectsKHR_fn clEnqueueReleaseD3D10ObjectsKHR;
};
#पूर्ण_अगर

namespace ocl {

Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device)
{
    CV_UNUSED(pD3D11Device);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अन्यथा
    cl_uint अंक_Platforms;
    cl_int status = clGetPlatformIDs(0, शून्य, &अंक_Platforms);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    अगर (अंक_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(अंक_Platforms);
    status = clGetPlatformIDs(अंक_Platforms, &platforms[0], शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    क्रम (पूर्णांक i = 0; i < (पूर्णांक)अंक_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();

        पूर्णांक found = -1;
        cl_device_id device = शून्य;
        cl_uint अंक_Devices = 0;
        cl_context context = शून्य;

#अगर_घोषित HAVE_OPENCL_D3D11_NV
        // Get extension function "clGetDeviceIDsFromD3D11NV" (part of OpenCL extension "cl_nv_d3d11_sharing")
        clGetDeviceIDsFromD3D11NV_fn clGetDeviceIDsFromD3D11NV = (clGetDeviceIDsFromD3D11NV_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D11NV");
        अगर (clGetDeviceIDsFromD3D11NV) {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_NV
            करो {
                device = शून्य;
                अंक_Devices = 0;
                status = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, &device, &अंक_Devices);
                अगर (status != CL_SUCCESS)
                    अवरोध;
                अगर (अंक_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };

                    context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                    अगर (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    अन्यथा
                    {
                        found = i;
                    }
                }
            } जबतक (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_NV
            अगर (found < 0) करो {
                device = शून्य;
                अंक_Devices = 0;
                status = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_NV, 1, &device, &अंक_Devices);
                अगर (status != CL_SUCCESS)
                    अवरोध;
                अगर (अंक_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };
                    context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                    अगर (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    अन्यथा
                    {
                        found = i;
                    }
                }
            } जबतक (0);
            अगर (found >= 0) {
                OpenCLExecutionContext clExecCtx;
                प्रयास
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D11_NV>(platform, pD3D11Device));
                }
                पकड़ो (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    फेंको;
                }
                clExecCtx.bind();
                वापस const_cast<Context&>(clExecCtx.getContext());
            }
        }
#पूर्ण_अगर
        // Get extension function "clGetDeviceIDsFromD3D11KHR" (part of OpenCL extension "cl_khr_d3d11_sharing")
        clGetDeviceIDsFromD3D11KHR_fn clGetDeviceIDsFromD3D11KHR = (clGetDeviceIDsFromD3D11KHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D11KHR");
        अगर (clGetDeviceIDsFromD3D11KHR)
        {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_KHR
            करो {

                device = शून्य;
                अंक_Devices = 0;

                status = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_KHR, 1, &device, &अंक_Devices);

                अगर (status != CL_SUCCESS)
                    अवरोध;
                अगर (अंक_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            शून्य, शून्य
                    };
                    context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                    अगर (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    अन्यथा
                    {
                        found = i;
                    }
                }
            } जबतक (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_KHR
            अगर (found < 0) करो {
                device = शून्य;
                अंक_Devices = 0;
                status = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &device, &अंक_Devices);
                अगर (status != CL_SUCCESS)
                    अवरोध;
                अगर (अंक_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            शून्य, शून्य
                    };
                    context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                    अगर (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    अन्यथा
                    {
                        found = i;
                    }
                }
            } जबतक (0);

            अगर (found >= 0) {
                OpenCLExecutionContext clExecCtx;
                प्रयास
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D11>(platform, pD3D11Device));
                }
                पकड़ो (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    फेंको;
                }
                clExecCtx.bind();
                वापस const_cast<Context&>(clExecCtx.getContext());
            }
        }
    }

    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#पूर्ण_अगर
}

Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device)
{
    CV_UNUSED(pD3D10Device);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अन्यथा
    cl_uint अंक_Platforms;
    cl_int status = clGetPlatformIDs(0, शून्य, &अंक_Platforms);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    अगर (अंक_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(अंक_Platforms);
    status = clGetPlatformIDs(अंक_Platforms, &platforms[0], शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    क्रम (पूर्णांक i = 0; i < (पूर्णांक)अंक_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        पूर्णांक found = -1;
        cl_device_id device = शून्य;
        cl_uint अंक_Devices = 0;
        cl_context context = शून्य;

        clGetDeviceIDsFromD3D10KHR_fn clGetDeviceIDsFromD3D10KHR = (clGetDeviceIDsFromD3D10KHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D10KHR");
        अगर (!clGetDeviceIDsFromD3D10KHR)
            जारी;

        // try with CL_PREFERRED_DEVICES_FOR_D3D10_KHR
        करो {
            device = शून्य;
            अंक_Devices = 0;
            status = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_PREFERRED_DEVICES_FOR_D3D10_KHR, 1, &device, &अंक_Devices);
            अगर (status != CL_SUCCESS)
                अवरोध;
            अगर (अंक_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        शून्य, शून्य
                };
                context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                अगर (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अन्यथा
                {
                    found = i;
                }
            }
        } जबतक (0);
        // try with CL_ALL_DEVICES_FOR_D3D10_KHR
        अगर (found < 0) करो
        {
            device = शून्य;
            अंक_Devices = 0;
            status = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_ALL_DEVICES_FOR_D3D10_KHR, 1, &device, &अंक_Devices);
            अगर (status != CL_SUCCESS)
                अवरोध;
            अगर (अंक_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        शून्य, शून्य
                };
                context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                अगर (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अन्यथा
                {
                    found = i;
                }
            }
        } जबतक (0);

        अगर (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            प्रयास
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D10>(platform, pD3D10Device));
            }
            पकड़ो (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                फेंको;
            }
            clExecCtx.bind();
            वापस const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#पूर्ण_अगर
}

Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex)
{
    CV_UNUSED(pDirect3DDevice9Ex);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अन्यथा
    cl_uint अंक_Platforms;
    cl_int status = clGetPlatformIDs(0, शून्य, &अंक_Platforms);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    अगर (अंक_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(अंक_Platforms);
    status = clGetPlatformIDs(अंक_Platforms, &platforms[0], शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    क्रम (पूर्णांक i = 0; i < (पूर्णांक)अंक_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        पूर्णांक found = -1;
        cl_device_id device = शून्य;
        cl_uint अंक_Devices = 0;
        cl_context context = शून्य;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        अगर (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            जारी;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        करो {
            device = शून्य;
            अंक_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &अंक_Devices);
            अगर (status != CL_SUCCESS)
                अवरोध;
            अगर (अंक_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        शून्य, शून्य
                };
                context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                अगर (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अन्यथा
                {
                    found = i;
                }
            }
        } जबतक (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        अगर (found < 0) करो
        {
            device = शून्य;
            अंक_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &अंक_Devices);
            अगर (status != CL_SUCCESS)
                अवरोध;
            अगर (अंक_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        शून्य, शून्य
                };
                context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                अगर (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अन्यथा
                {
                    found = i;
                }
            }
        } जबतक (0);

        अगर (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            प्रयास
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D9>(platform, nullptr, pDirect3DDevice9Ex));
            }
            पकड़ो (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                फेंको;
            }
            clExecCtx.bind();
            वापस const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#पूर्ण_अगर
}

Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9)
{
    CV_UNUSED(pDirect3DDevice9);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अन्यथा
    cl_uint अंक_Platforms;
    cl_int status = clGetPlatformIDs(0, शून्य, &अंक_Platforms);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    अगर (अंक_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(अंक_Platforms);
    status = clGetPlatformIDs(अंक_Platforms, &platforms[0], शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    क्रम (पूर्णांक i = 0; i < (पूर्णांक)अंक_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        पूर्णांक found = -1;
        cl_device_id device = शून्य;
        cl_uint अंक_Devices = 0;
        cl_context context = शून्य;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        अगर (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            जारी;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        करो {
            device = शून्य;
            अंक_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &अंक_Devices);
            अगर (status != CL_SUCCESS)
                अवरोध;
            अगर (अंक_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        शून्य, शून्य
                };
                context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                अगर (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अन्यथा
                {
                    found = i;
                    अवरोध;
                }
            }
        } जबतक (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        अगर (found < 0) करो
        {
            device = शून्य;
            अंक_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &अंक_Devices);
            अगर (status != CL_SUCCESS)
                अवरोध;
            अगर (अंक_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        शून्य, शून्य
                };
                context = clCreateContext(properties, 1, &device, शून्य, शून्य, &status);
                अगर (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अन्यथा
                {
                    found = i;
                    अवरोध;
                }
            }
        } जबतक (0);

        अगर (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            प्रयास
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D9>(platform, pDirect3DDevice9, nullptr));
            }
            पकड़ो (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                फेंको;
            }
            clExecCtx.bind();
            वापस const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#पूर्ण_अगर
}

} // namespace cv::ocl

} // namespace directx


namespace ocl {

#अगर defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
#अगर_घोषित HAVE_DIRECTX_NV12

अटल
bool ocl_convert_nv12_to_bgr(
    cl_mem clImageY,
    cl_mem clImageUV,
    cl_mem cl_बफर,
    पूर्णांक step,
    पूर्णांक cols,
    पूर्णांक rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    अगर (k.empty())
        वापस false;

    k.args(clImageY, clImageUV, cl_बफर, step, cols, rows);

    माप_प्रकार globalsize[] = { (माप_प्रकार)cols/2, (माप_प्रकार)rows/2 };
    वापस k.run(2, globalsize, 0, false);
}


अटल
bool ocl_convert_bgr_to_nv12(
    cl_mem cl_बफर,
    पूर्णांक step,
    पूर्णांक cols,
    पूर्णांक rows,
    cl_mem clImageY,
    cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    अगर (k.empty())
        वापस false;

    k.args(cl_बफर, step, cols, rows, clImageY, clImageUV);

    माप_प्रकार globalsize[] = { (माप_प्रकार)cols/2, (माप_प्रकार)rows/2 };
    वापस k.run(2, globalsize, 0, false);
}

#पूर्ण_अगर // HAVE_DIRECTX_NV12
#पूर्ण_अगर // HAVE_DIRECTX && HAVE_OPENCL

} // namespace ocl


namespace directx {

#अगर defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
अटल व्योम __convertToD3D11Texture2DKHR(निवेश_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    पूर्णांक srcType = src.type();
    पूर्णांक textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    आकार src_आकार = src.size();
    CV_Assert(src_आकार.width == (पूर्णांक)desc.Width && src_आकार.height == (पूर्णांक)desc.Height);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().लो();
    अगर (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;
#अगर_घोषित HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#पूर्ण_अगर
    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &status);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &status);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#पूर्ण_अगर

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImageUV, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        अगर(!ocl::ocl_convert_bgr_to_nv12(cl_बफर, (पूर्णांक)u.step[0], u.cols, u.rows, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImageUV, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    अन्यथा
#पूर्ण_अगर
    {
        माप_प्रकार offset = 0; // TODO
        माप_प्रकार origin[3] = { 0, 0, 0 };
        माप_प्रकार region[3] = { (माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1 };

        status = cl_Enqueue_Copy_बफर_To_Image(q, cl_बफर, clImage, offset, origin, region, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#पूर्ण_अगर
}
#पूर्ण_अगर

#अगर defined(HAVE_OPENCL_D3D11_NV)
अटल व्योम __convertToD3D11Texture2DNV(निवेश_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    पूर्णांक srcType = src.type();
    पूर्णांक textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    आकार src_आकार = src.size();
    CV_Assert(src_आकार.width == (पूर्णांक)desc.Width && src_आकार.height == (पूर्णांक)desc.Height);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().लो();
    अगर (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;
#अगर_घोषित HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#पूर्ण_अगर
    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &status);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर (DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &status);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#पूर्ण_अगर
    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImageUV, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        अगर(!ocl::ocl_convert_bgr_to_nv12(cl_बफर, (पूर्णांक)u.step[0], u.cols, u.rows, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImageUV, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    अन्यथा
#पूर्ण_अगर
    {
        माप_प्रकार offset = 0; // TODO
        माप_प्रकार origin[3] = { 0, 0, 0 };
        माप_प्रकार region[3] = { (माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1 };

        status = cl_Enqueue_Copy_बफर_To_Image(q, cl_बफर, clImage, offset, origin, region, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#पूर्ण_अगर
}
#पूर्ण_अगर

#अगर defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
अटल व्योम __convertFromD3D11Texture2DKHR(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    पूर्णांक textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(आकार(desc.Width, desc.Height), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().लो();
    अगर (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &status);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &status);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#पूर्ण_अगर

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImageUV, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        अगर(!ocl::ocl_convert_nv12_to_bgr(clImage, clImageUV, cl_बफर, (पूर्णांक)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImageUV, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    अन्यथा
#पूर्ण_अगर
    {
        माप_प्रकार offset = 0; // TODO
        माप_प्रकार origin[3] = { 0, 0, 0 };
        माप_प्रकार region[3] = { (माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1 };

        status = cl_Enqueue_Copy_Image_To_बफर(q, clImage, cl_बफर, origin, region, offset, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#पूर्ण_अगर
}
#पूर्ण_अगर

#अगर defined(HAVE_OPENCL_D3D11_NV)
अटल व्योम __convertFromD3D11Texture2DNV(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    पूर्णांक textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(आकार(desc.Width, desc.Height), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().लो();
    अगर (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &status);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &status);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#पूर्ण_अगर

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर (DXGI_FORMAT::DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImageUV, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        अगर (!ocl::ocl_convert_nv12_to_bgr(clImage, clImageUV, cl_बफर, (पूर्णांक)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImageUV, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    अन्यथा
#पूर्ण_अगर
    {
        माप_प्रकार offset = 0; // TODO
        माप_प्रकार origin[3] = { 0, 0, 0 };
        माप_प्रकार region[3] = { (माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1 };

        status = cl_Enqueue_Copy_Image_To_बफर(q, clImage, cl_बफर, origin, region, offset, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#अगर_घोषित HAVE_DIRECTX_NV12
    अगर(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#पूर्ण_अगर
}
#पूर्ण_अगर

व्योम convertToD3D11Texture2D(निवेश_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    CV_UNUSED(src); CV_UNUSED(pD3D11Texture2D);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अन्यथा

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#अगर_घोषित HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().लो();
    अगर (impl_nv) {
        __convertToD3D11Texture2DNV(src,pD3D11Texture2D);
        वापस;
    }
#पूर्ण_अगर
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().लो();
    अगर (impl) {
        __convertToD3D11Texture2DKHR(src, pD3D11Texture2D);
    }
    अन्यथा {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#पूर्ण_अगर
}

व्योम convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    CV_UNUSED(pD3D11Texture2D); CV_UNUSED(dst);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अन्यथा

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#अगर_घोषित HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().लो();
    अगर (impl_nv) {
        __convertFromD3D11Texture2DNV(pD3D11Texture2D,dst);
        वापस;
    }
#पूर्ण_अगर
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().लो();
    अगर (impl) {
        __convertFromD3D11Texture2DKHR(pD3D11Texture2D, dst);
    }
    अन्यथा {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#पूर्ण_अगर
}

व्योम convertToD3D10Texture2D(निवेश_Array src, ID3D10Texture2D* pD3D10Texture2D)
{
    CV_UNUSED(src); CV_UNUSED(pD3D10Texture2D);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().लो();
    अगर (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    पूर्णांक srcType = src.type();
    पूर्णांक textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    आकार src_आकार = src.size();
    CV_Assert(src_आकार.width == (पूर्णांक)desc.Width && src_आकार.height == (पूर्णांक)desc.Height);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D10Texture2D, 0, &status);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D10ObjectsKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    माप_प्रकार offset = 0; // TODO
    माप_प्रकार dst_origin[3] = {0, 0, 0};
    माप_प्रकार region[3] = {(माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1};
    status = cl_Enqueue_Copy_बफर_To_Image(q, cl_बफर, clImage, offset, dst_origin, region, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    status = impl->clEnqueueReleaseD3D10ObjectsKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#अन्यथा
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#पूर्ण_अगर
}

व्योम convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst)
{
    CV_UNUSED(pD3D10Texture2D); CV_UNUSED(dst);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर defined(HAVE_OPENCL)
    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().लो();
    अगर (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    पूर्णांक textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(आकार(desc.Width, desc.Height), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D10Texture2D, 0, &status);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D10ObjectsKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    माप_प्रकार offset = 0; // TODO
    माप_प्रकार src_origin[3] = {0, 0, 0};
    माप_प्रकार region[3] = {(माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1};
    status = cl_Enqueue_Copy_Image_To_बफर(q, clImage, cl_बफर, src_origin, region, offset, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    status = impl->clEnqueueReleaseD3D10ObjectsKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#अन्यथा
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#पूर्ण_अगर
}

व्योम convertToDirect3DSurface9(निवेश_Array src, IDirect3DSurface9* pDirect3DSurface9, व्योम* surfaceSharedHandle)
{
    CV_UNUSED(src); CV_UNUSED(pDirect3DSurface9); CV_UNUSED(surfaceSharedHandle);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().लो();
    अगर (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    अगर (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    पूर्णांक srcType = src.type();
    पूर्णांक surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType == srcType);

    आकार src_आकार = src.size();
    CV_Assert(src_आकार.width == (पूर्णांक)desc.Width && src_आकार.height == (पूर्णांक)desc.Height);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_WRITE_ONLY,
        impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &status);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireDX9MediaSurfacesKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    माप_प्रकार offset = 0; // TODO
    माप_प्रकार dst_origin[3] = {0, 0, 0};
    माप_प्रकार region[3] = {(माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1};
    status = cl_Enqueue_Copy_बफर_To_Image(q, cl_बफर, clImage, offset, dst_origin, region, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    status = impl->clEnqueueReleaseDX9MediaSurfacesKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#अन्यथा
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#पूर्ण_अगर
}

व्योम convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, व्योम* surfaceSharedHandle)
{
    CV_UNUSED(pDirect3DSurface9); CV_UNUSED(dst); CV_UNUSED(surfaceSharedHandle);
#अगर !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#या_अगर defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().लो();
    अगर (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    अगर (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    पूर्णांक surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(आकार(desc.Width, desc.Height), surfaceType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_READ_ONLY,
            impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &status);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_WRITE);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireDX9MediaSurfacesKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    माप_प्रकार offset = 0; // TODO
    माप_प्रकार src_origin[3] = {0, 0, 0};
    माप_प्रकार region[3] = {(माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1};
    status = cl_Enqueue_Copy_Image_To_बफर(q, clImage, cl_बफर, src_origin, region, offset, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    status = impl->clEnqueueReleaseDX9MediaSurfacesKHR(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#अन्यथा
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#पूर्ण_अगर
}

} } // namespace cv::directx
